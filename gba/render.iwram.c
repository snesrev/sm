// See render.h.
//
// ARM code in IWRAM, as it runs every frame. (So no division: see
// cmake/compiler-rt.cmake.)
//
// The SNES's Mode 1 maps onto the GBA's Mode 0 like this, by layer:
//   GBA BG0: SNES BG3 (2bpp, the HUD and effects), priority 0 or 3
//   GBA BG1: SNES BG1's priority tiles, priority 1
//   GBA BG2: SNES BG1's other tiles, priority 2
//   GBA BG3: SNES BG2, priority 2
// and SNES sprites at priority 3, 2, 1, 0 become GBA priority 1, 2, 3, 3.
// The GBA draws a sprite in front of a background of the same priority, so
// this keeps SNES Mode 1's order (BG2's priority tiles aside). In a frame
// where BG2 shows more priority tiles than BG1 (split_bg2), it's BG2 that's
// split, and BG1's priority tiles go behind sprites of priority 2:
//   GBA BG1: SNES BG2's priority tiles, priority 1
//   GBA BG2: SNES BG1, priority 2
//   GBA BG3: SNES BG2's other tiles, priority 2
// In a frame where BG2 shows priority tiles but no others (bg2_over_lo:
// Kraid's body), the SNES's order is BG1's priority tiles, BG2, sprites of
// priority 2, BG1's others: BG2 is at priority 1 behind GBA BG1, which comes
// first at that priority,
//   GBA BG1: SNES BG1's priority tiles, priority 1
//   GBA BG2: SNES BG1's other tiles, priority 2
//   GBA BG3: SNES BG2, priority 1
// In a frame that adds BG3 to BG2 but not BG1 (bg3_mid), BG1's layers and
// sprites go a priority higher (0 and 1; sprites 0-3), for BG3 to go
// between BG1 and BG2.
// (The SNES orders sprites among themselves by their places in OAM only, the GBA by priority
// first: see KeepSamusInFront.)
//
// GBA VRAM:
//   0x0000-0x7FFF  BG1/BG2 tiles (the SNES's 1024 at BG1's base)
//   0x8000-0xAFFF  BG3 tiles, converted to 4bpp (the first 384)
//   0xB000-0xBFFF  BG3 map, slot 0 (screen blocks 22-23)
//   0xC000-0xCFFF  BG1 priority tiles' map (24-25)
//   0xD000-0xDFFF  BG1 other tiles' map (26-27)
//   0xE000-0xEFFF  BG2 map (28-29)
//                  (with split_bg2: GBA BG1's, BG2's and BG3's, as above)
//   0xF000-0xFFFF  BG3 map, slot 1 (30-31): the HUD and the effects layer
//                  are different maps.
// In Mode 7, the SNES's one background is the GBA's affine BG2 in Mode 1,
// tiles at 0x0000 and its 128x128 map at 0x4000, in place of BG1 and BG2.
// A frame can have both: Ceres has Mode 7 under a Mode 1 HUD, which is BG3
// on GBA BG0 as usual.
// Sprite tiles keep the SNES's layout of 16 to a row, in the left half of
// the GBA's rows of 32 (2D mapping), with a copy in the right half, so that
// sprites whose tiles wrap around a row come out right.
#include "render.h"

#include <stdio.h>
#include <string.h>

#include "convert.h"
#include "display.h"
#include "gba_hw.h"
#include "snes_hw.h"
#include "sm_rtl.h"
#include "variables.h"

enum {
  // Lines at the top of the SNES screen with the HUD, the first 8 of
  // them blank, shown on the GBA as they are (but for those); the rest of
  // the GBA screen shows part of the SNES's gameplay area, following Samus.
  kHudLines = 32,
  kHudBlankLines = 8,
  kGbaHudLines = kHudLines - kHudBlankLines,
  kMaxYOffset = kSnesLines - kHudBlankLines - kGbaLines,
  // The gameplay area from the SNES's line 64 (see UpdateYOffset).
  kCeresMessageYOffset = 64 - kHudLines,
  // The SNES's 256 pixel lines lose 8 pixels at each end.
  kXOffset = 8,

  kBg3Tiles = 384,
  kSbBg3Slot0 = 22,
  kSbBg1Hi = 24,
  kSbBg1Lo = 26,
  kSbBg2 = 28,
  kSbBg3Slot1 = 30,
  kSbMode7 = 8,
};

#define VRAM_SB(n) (GBA_VRAM + (n) * 0x400)

extern volatile uint32_t g_vblank_count;  // (irq.iwram.c)
extern volatile bool g_backdrop_zeroed;   // (irq.iwram.c)

// A line's GBA registers, before they're split into display.h's stream
// and segments.
typedef struct LineRegs {
  DisplaySegment ctl;  // but for first_line
  uint32_t stream[4];
} LineRegs;

// The PPU's registers for each SNES line: line_states[line_state[line]],
// with a state for each time they change down the screen.
static PpuRegs line_states[kSnesLines] __attribute__((aligned(4)));
static uint8_t line_state[kSnesLines];
static uint32_t line_wh[kSnesLines];  // WH0-WH3, by line
// BGnHOFS | BGnVOFS << 16, by line, in place of the states'.
static uint32_t line_ofs[kSnesLines][4];
#define LINE_REGS(s) (&line_states[line_state[s]])
#define HOFS(s, bg) (line_ofs[s][bg] & 0xffff)
#define VOFS(s, bg) (line_ofs[s][bg] >> 16)
// Two of each, for the frame on show and the next. (One of the streams in
// EWRAM, for room in IWRAM: HBlank DMA reads only 16 bytes a line.) Only
// BuildLine writes them: ScrollRuns keeps what runs they hold, to skip.
static uint32_t stream0[kGbaLines + 1][4] IWRAM_BSS, stream1[kGbaLines + 1][4];
static uint32_t (*const streams[2])[4] = {stream0, stream1};
// The streams of frames with windows (see display.h), and their windows
// line by line, for them.
static uint32_t window_streams[2][kGbaLines + 1][kWindowStreamWords];
static uint32_t line_windows[kGbaLines];
// Segments: a frame has a few, as a rule (each costs an interrupt). Past
// the most, the last goes on to the end. (The tour and the intro have 3 at
// most.)
enum { kMaxSegments = 16 };
static DisplaySegment segments[2][kMaxSegments];
static DisplayFrame frames[2];
static int back;

static bool hud;
// Outside gameplay (the title, menus, the pause screen), the SNES screen
// squashed into the GBA's: 6 of each 8 lines (the SNES's 224 lines make
// 168, less 4 at the top and bottom; see SetSquashLines). kSquashLine[g]
// is the SNES line on GBA line g, and sprites are scaled to match.
enum { kSquashTop = 4 };
static bool squash = true;  // (the boot's Nintendo logo is a menu's)
static uint8_t kSquashLine[kGbaLines];
// Whether lines on show use Mode 7, and one that uses another mode, or -1.
static bool frame_mode7;
static int text_line;
static int y_offset = kMaxYOffset / 2;
// SNES BG3 maps in GBA map slots 0 and 1, by BG3SC.
static uint8_t bg3_slot_sc[2] = {0xff, 0xff};
// Whether one BG3 map serves all the lines of the frame, and so the other
// slot is spare: Mother Brain's rainbow beam borrows it for a map of one solid
// tile (see ColorMath). And whether a line of the frame did, which slot, and
// the color (the fixed color's channels).
static bool bg3_single;
static bool beam_on;
static uint8_t beam_slot, beam_color[3];
// How many of each slot's entries, by chunk of 16, have the priority bit.
// The GBA has no background to spare for BG3's priority tiles (in front of
// everything, with BGMODE's bit 3) and the others (at the back): lines
// whose rows have any go in front (Bg3Front, which reads a row's two
// chunks together).
// (By byte for the writers; by halfword, a row's two chunks, for Bg3Front.)
static union {
  uint8_t chunks[128];
  uint16_t rows[64];
} bg3_hi[2];
static int bg3_hi_total[2];
// Likewise for BG1's and BG2's maps (by screen, 64 chunks each), as last
// converted; and whether to split BG2, not BG1, into the GBA's BG1 and BG3
// (see the top): Ceres's Ridley room has its walls and door in BG2's
// priority tiles, in front of a BG1 of all one tile (but for the escape's
// timer).
static uint8_t bg12_hi[2][128];
// Their sums by column of chunks: by screen, the left and right halves.
static uint16_t bg12_hi_cols[2][4];
static bool split_bg2;
// Whether the frame puts BG2 in front of the sprites of priority 2 and BG1's
// other tiles, BG1's priority tiles in front of it (see the top): BG2 has
// priority tiles on show and no others, which behind BG1's others would be
// hidden by what is all wall, and in front of BG1's priority tiles would hide
// what's on the floor.
static bool bg2_over_lo;
// The GBA layers of BG1 and BG2, as split_bg2 has them.
static uint8_t bg12_layers[2] = {6, 8};
// Whether the frame adds BG3 (on the sub screen) to BG2 but not BG1, as in
// some of Brinstar's and the Wrecked Ship's rooms: then BG3 goes between
// them, BG1 a priority higher and sprites too (see ColorMath).
static bool bg3_mid;
// How a power bomb's blast, the fixed color in the window that hides BG3, is
// tinted: 0 for not (see Bg3Kind).
static uint8_t blast_bg3;
// Whether the lines being built are a power bomb's with no window of the
// blast's on them: BG3 added to the room as it is, all along them (see
// ColorMath).
static bool blast_nowin;
// Whether the frame is a power bomb's (FindBlast).
static bool blast_on;
enum { kBg3Mixed, kBg3Black, kBg3Clear };
// With the sub screen subtracted, how much the main screen keeps where the
// sub screen's layers are, in 16ths (see SubtractWeight).
static int subtract_weight;
static uint8_t subtract_layer = 4;
// Or, with bit 15 set, a color the palette subtracts, as it does the fixed
// color: the sub screen's average, when it's all over the screen; or when
// it's BG3 all over the screen (Maridia's shadows), the brightest of its
// colors, with the BG3 palettes it uses in subtract_palettes. Then BG3 goes
// on top with each of its colors c as that color less c, added: the SNES's
// subtraction, but for main screen colors darker than that color. (See
// SubtractWeight.)
static uint16_t subtract_color;
static uint8_t subtract_palettes;
// The palette's colors for that in the gameplay area (see DisplayFrame).
static int num_patches;
static uint8_t patch_index[24];
static uint16_t patch_color[24], restore_color[24];

// What render.c last converted, to see when it has to start over.
static struct {
  uint16_t bg12_base, bg3_base, obj_base[2];
  uint8_t bgsc[2];
  uint8_t bg2_nba;  // BG12NBA: where BG2's tiles are, from BG1's
  uint8_t bg_row[2];  // of 64x64 maps, which row of screens each tile row comes from (Map64Key)
  uint8_t bg3_slot_sc[2];
  int8_t beam_slot;  // where the rainbow beam's map is, or -1 (see ShowBeam)
  int blank_tile;
  int brightness;
  uint32_t fixed_math;
  bool mode7, bg3_palettes, split_bg2, squashed;
  uint8_t subtract_palettes;
} conv = {.beam_slot = -1, .blank_tile = -1, .brightness = -1};

static inline __attribute__((always_inline)) int SnesLine(int g) {
  if (squash)
    return kSquashLine[g];
  if (!hud)
    return g + y_offset;
  return g < kGbaHudLines ? g + kHudBlankLines : g + kHudBlankLines + y_offset;
}

static int Luma(uint16_t color);

// The lines that go, squashed: those that show rows 0 and 4 of the text's
// layer's tiles (BG3's where it's on the main screen, or else BG1's), by
// each line's registers and scrolling, so that they move with the text.
// They're merged into rows 1 and 3 as the tiles are converted
// (Convert_Tiles4bppSquashed), and text loses none of its strokes. And
// kSquashTop of the rest off the top. Changes sprites' places, which go
// with the lines.
ROM_CODE static void SetSquashLines(void) {
  // Unless which of each 4 lines go changed, at a line of each 8, as is
  // usual, it's as it was.
  static uint32_t sampled[2];
  uint32_t now[2] = {0, 0};
  for (int s = 4; s < kSnesLines; s += 8) {
    int bg = line_states[line_state[s]].tm & 4 ? 2 : 0;
    now[s >> 7] = now[s >> 7] << 2 | ((s + 1 + (line_ofs[s][bg] >> 16)) & 3);
  }
  if (now[0] == sampled[0] && now[1] == sampled[1])
    return;
  sampled[0] = now[0], sampled[1] = now[1];
  bool changed = false;
  int state = -1, bg = 0;
  for (int s = 0, g = -kSquashTop; g < kGbaLines && s < kSnesLines; s++) {
    if (line_state[s] != state) {
      state = line_state[s];
      bg = line_states[state].tm & 4 ? 2 : 0;
    }
    // (Line s shows row (s + 1 + VOFS) & 7.)
    if (!((s + 1 + (line_ofs[s][bg] >> 16)) & 3))
      continue;
    if (g >= 0) {
      changed |= kSquashLine[g] != s;
      kSquashLine[g] = s;
    }
    g++;
  }
  if (changed)
    g_snes_hw.oam_changed = ~0u;
}

ROM_CODE static void SetSquashRanks(void) {
  // BG palette 0's colors by brightness, for merging rows: once a screen,
  // when its colors aren't dark (as they fade in; then they may change, as
  // the game animates them, but not their order, as a rule), and if it's
  // another order, the tiles again.
  static uint32_t rank_key;
  const PpuRegs *r = LINE_REGS(kSnesLines / 2);
  uint32_t key = r->tm | r->bgsc[0] << 8 | r->bgsc[1] << 16 | r->bgsc[2] << 24;
  if (key != rank_key) {
    static uint8_t rank[16];
    uint8_t luma[16], now[16];
    int most = 0;
    for (int c = 0; c < 16; c++) {
      luma[c] = Luma(g_cgram[c]);
      most = luma[c] > most ? luma[c] : most;
    }
    if (most >= 16) {
      rank_key = key;
      for (int c = 0; c < 16; c++) {
        now[c] = 0;
        for (int d = 0; d < 16; d++)
          now[c] += luma[d] < luma[c] || (luma[d] == luma[c] && d > c);
      }
      if (memcmp(now, rank, sizeof(rank))) {
        memcpy(rank, now, sizeof(rank));
        Convert_SetSquashRanks(rank);
        conv.bg12_base = conv.bg3_base = 0xffff;
      }
    }
  }
}

ROM_CODE void Render_Init(void) {
  for (int g = 0; g < kGbaLines; g++)
    kSquashLine[g] = g + kSquashTop;
  REG_DISPCNT = kDispcntForcedBlank;
  DmaFill32(GBA_VRAM, 0, 0x18000);
  for (int i = 0; i < 128; i++)
    GBA_OAM[i * 4] = 0x200;  // hidden
  REG_DISPSTAT = kDispstatVBlankIrq;
}

// Logs the SNES's screen layout when it changes, as what's supported grows.
ROM_CODE static void LogLayoutChange(const PpuRegs *r) {
  static uint32_t last[2];
  uint32_t now[2] = {
    r->bgmode | r->bgsc[0] << 8 | r->bgsc[1] << 16 | (uint32_t)r->bgsc[2] << 24,
    r->bgnba[0] | r->bgnba[1] << 8 | r->tm << 16 | (uint32_t)g_snes_hw.obsel << 24,
  };
  if (now[0] != last[0] || now[1] != last[1]) {
    printf("render: mode %02x bgsc %02x %02x %02x nba %02x %02x tm %02x obsel %02x\n", r->bgmode,
           r->bgsc[0], r->bgsc[1], r->bgsc[2], r->bgnba[0], r->bgnba[1], r->tm, g_snes_hw.obsel);
    last[0] = now[0];
    last[1] = now[1];
  }
}

static int Bg3Slot(uint8_t sc) {
  return sc == bg3_slot_sc[1] ? 1 : 0;
}

static uint16_t BgSize(uint8_t sc) {
  // 64x64 maps get half, the row of two screens on show (UpdateMaps12):
  // there are two screens per map.
  return (sc & 3) == 3 ? 1 : (sc & 3);
}

static int Sext13(int v) {
  return (int16_t)(v << 3) >> 3;
}

// A Mode 7 scroll relative to its center, as the SNES's hardware takes it: 10 bits, keeping the
// sign (the map wraps at 1024 pixels, which only a matrix with a 256 (one pixel a pixel) has
// the same after: Ceres's rotating rooms don't).
ROM_CODE static inline __attribute__((always_inline)) int Clip10(int v) {
  return v & 0x2000 ? v | ~1023 : v & 1023;
}

ROM_CODE static void BuildLineMode7(LineRegs *l, const PpuRegs *r, int s) {
  uint16_t dispcnt = kDispcntMode1;
  if (!(r->inidisp & 0x80)) {
    if (r->tm & 1) dispcnt |= kDispcntBg0 << 2;
    if (r->tm & 16) dispcnt |= kDispcntObj;
  }
  l->ctl.dispcnt = dispcnt;
  l->ctl.bgcnt[0] = l->ctl.bgcnt[1] = l->ctl.bgcnt[3] = 0;
  // 1024x1024, wrapping around unless the SNES's field is "large".
  l->ctl.bgcnt[2] = 3 | kSbMode7 << 8 | 3 << 14 | (r->m7sel & 0x80 ? 0 : 1 << 13);
  l->ctl.bld[0] = l->ctl.bld[1] = l->ctl.bld[2] = 0;
  l->ctl.win[0] = l->ctl.win[1] = 0;
  for (int i = 0; i < 8; i++)
    l->ctl.ofs[i] = 0;
  // The SNES's screen pixel (sx, sy) shows the map's pixel
  //   ((a b) (c d)) (sx + hofs - x0, sy + vofs - y0) + (x0, y0),
  // and GBA pixel (gx, line) is SNES pixel (gx + kXOffset, s): of the
  // SNES's scanline s + 1, as the first scanline drawn is 1 (as for the
  // tiled layers' rows, ScrollStream).
  int a = r->m7[0], b = r->m7[1], c = r->m7[2], d = r->m7[3];
  int x0 = r->m7[4], y0 = r->m7[5];
  // (The hardware drops the low 6 bits of the products with the scroll, a quarter of a pixel.)
  int h = Clip10(Sext13(r->m7hofs) - x0), v = Clip10(Sext13(r->m7vofs) - y0);
  l->stream[0] = (uint16_t)a | (uint32_t)(uint16_t)b << 16;
  l->stream[1] = (uint16_t)c | (uint32_t)(uint16_t)d << 16;
  l->stream[2] = (a * h & ~63) + (b * v & ~63) + a * kXOffset + b * (s + 1) + (x0 << 8);
  l->stream[3] = (c * h & ~63) + (d * v & ~63) + c * kXOffset + d * (s + 1) + (y0 << 8);
}

// |n| (at least 1) lines of a Mode 7 stream with the same registers, from
// the first line's |in|: the reference point moves down the map by (b, d).
// (Or, with those 0, the same line.) In assembly, like ScrollStream.
__attribute__((noinline)) static void Mode7Stream(uint32_t (*out)[4], int n, const uint32_t *in,
                                                  int b, int d) {
  __asm__ volatile(
      "ldmia %[in], {r4-r7}\n\t"
      "1: stmia %[out]!, {r4-r7}\n\t"
      "add r6, r6, %[b]\n\t"
      "add r7, r7, %[d]\n\t"
      "subs %[n], %[n], #1\n\t"
      "bne 1b"
      : [out] "+r"(out), [n] "+r"(n)
      : [in] "r"(in), [b] "r"(b), [d] "r"(d)
      : "r4", "r5", "r6", "r7", "cc", "memory");
}

// The GBA layers (BLDCNT's bits) of the SNES's layers in |snes| (CGADSUB's
// or TM's bits: BG1-BG4, OBJ, backdrop).
static uint16_t BlendLayers(uint8_t snes) {
  return (snes & 1 ? bg12_layers[0] : 0) | (snes & 2 ? bg12_layers[1] : 0) | (snes & 4 ? 1 : 0) |
         (snes & 0x30);
}

// The SNES's color math, as the GBA's blending can do it, for a line with
// the tiled layers. In place of the SNES's sub screen added to the main
// screen, which the GBA can't do, its layers that aren't on the main
// screen show above the others, blended with the layer below them: the
// sum's the same. (The game's rooms add BG3 that way, for rain, fog, water
// and the like.) The fixed color, added or subtracted, is in the palette
// (UpdatePalette), the same for all the frame's lines. Without windows yet,
// color math "inside the window" is nowhere, and "outside", everywhere. The
// sub screen subtracted only darkens, by about what it takes away
// (SubtractWeight).
// Whether a layer is in the SNES's windows, as its |sel| (W12SEL's,
// W34SEL's or WOBJSEL's 4 bits for it) and |logic| (WBGLOG's or WOBJLOG's 2
// bits) have it, at a pixel in window 1 or not, and 2 or not.
ROM_CODE static bool InWindows(int sel, int logic, bool in1, bool in2) {
  bool on1 = sel & 2, on2 = sel & 8;
  bool w1 = in1 ^ (sel & 1), w2 = in2 ^ ((sel >> 2) & 1);
  if (!on1 || !on2)
    return on1 ? w1 : on2 ? w2 : false;
  switch (logic) {
  case 0: return w1 || w2;
  case 1: return w1 && w2;
  case 2: return w1 ^ w2;
  default: return !(w1 ^ w2);
  }
}

// Whether color math is off in a region (CGWSEL's bits 4-5, |region|), as
// the color window has it there.
ROM_CODE static bool NoMath(const PpuRegs *r, int region, bool in1, bool in2) {
  bool color = InWindows(r->wsel[2] >> 4, (r->wobjlog >> 2) & 3, in1, in2);
  return region == 3 || (region == 1 && !color) || (region == 2 && color);
}

// Whether the windows hide all the sub screen's layers in |windowed|
// somewhere color math's on: where the fixed color shows.
ROM_CODE static bool FixedShows(const PpuRegs *r, uint8_t windowed) {
  for (int k = 0; k < 3; k++) {
    bool in1 = k == 0, in2 = k == 1, hidden = true;
    for (int i = 0; i < 3; i++) {
      if (windowed & (1 << i))
        hidden &= InWindows((r->wsel[i >> 1] >> ((i & 1) * 4)) & 15, (r->wbglog >> (2 * i)) & 3,
                            in1, in2);
    }
    if (hidden && !NoMath(r, (r->cgwsel >> 4) & 3, in1, in2))
      return true;
  }
  return false;
}

// The sub screen's layers that the line |r|'s color math uses, as
// ColorMath has them. Subtracted, it's one of them, that SubtractWeight
// chose: the GBA can't have several on top, as they are on the SNES's sub
// screen, with nothing to blend between them (Tourian's darkness).
ROM_CODE static uint8_t SubLayers(const PpuRegs *r) {
  uint8_t sub = r->ts & ~r->tm & 7;
  return (r->cgadsub & 0x80) && (sub & subtract_layer) ? subtract_layer : sub;
}

// The sub screen's layers in |sub| that a window can hide (TSW, with a
// window on the layer).
ROM_CODE static uint8_t WindowedSub(const PpuRegs *r, uint8_t sub) {
  uint8_t windowed = 0;
  for (int i = 0; i < 3; i++) {
    if ((sub & r->tsw & (1 << i)) && ((r->wsel[i >> 1] >> ((i & 1) * 4)) & 0xa))
      windowed |= 1 << i;
  }
  return windowed;
}

// Whether line |r| is a power bomb's: BG3 alone on the sub screen, added
// whole to the main screen, and hidden by a window (the blast's) where the
// fixed color is what's added. (With the fixed color not in the window
// alone, but where BG3 is transparent as well, that's not the blast's.)
ROM_CODE static bool PowerBombRegs(const PpuRegs *r) {
  uint8_t sub = r->ts & ~r->tm & 7;
  if (!(r->cgwsel & 2) || (r->cgwsel & 0x30) || (r->cgadsub & 0xc0) || !(r->cgadsub & 0x3f) ||
      sub != 4 || frame_mode7 || (r->inidisp & 0x80) ||
      !(r->coldata[0] | r->coldata[1] | r->coldata[2]))
    return false;
  uint8_t windowed = WindowedSub(r, sub);
  return windowed && FixedShows(r, windowed);
}

// The blast's tint (blast_bg3, a kBg3Black or kBg3Clear) on line |r|, or 0.
ROM_CODE static int BlastTint(const PpuRegs *r) {
  return blast_bg3 != kBg3Mixed && PowerBombRegs(r) ? blast_bg3 : 0;
}

// Returns the sub screen's layers it shows (CGADSUB's bits), or with
// kFixedInWindows, the ones that windows hide, for the fixed color there. Or
// with kBlastTint, BG3 as the blast's tint (see Bg3Kind): it shows where
// windows hide it on the SNES, and not elsewhere.
enum { kFixedInWindows = 0x80, kBlastTint = 0x40, kBeamAdd = 0x20 };
ROM_CODE static uint8_t ColorMath(DisplaySegment *c, const PpuRegs *r) {
  c->bld[0] = c->bld[1] = c->bld[2] = 0;
  uint8_t math = r->cgadsub & 0x3f;
  // Color math only inside the color window, or not at all; with the
  // window, Windows has the GBA's windows do it.
  int region = (r->cgwsel >> 4) & 3;
  if (!math || (region == 1 && !(r->wsel[2] & 0xa0)) || region == 3 || frame_mode7 ||
      (r->inidisp & 0x80))
    return 0;
  bool subtract = r->cgadsub & 0x80, half = r->cgadsub & 0x40;
  if (!(r->cgwsel & 2) && !subtract && region == 1) {
    // The fixed color added inside the color window only (Mother Brain's
    // rainbow beam: HDMA gives the window its shape, and the color its
    // cycle).
    int fixed = r->coldata[0] + r->coldata[1] + r->coldata[2];
    if (!fixed)
      return 0;
    if (bg3_single && !(r->tm & 4)) {
      // The GBA's blending adds, too: a layer of one solid color (BG0,
      // over everything, from a map of one tile in the slot no BG3 uses:
      // ShowBeam) added to the layers the math has, where Windows shows it
      // (the color window).
      beam_on = true;
      beam_slot = r->bgsc[2] == bg3_slot_sc[1] ? 0 : 1;  // (the other than Bg3Slot says)
      for (int i = 0; i < 3; i++)
        beam_color[i] = r->coldata[i];
      c->dispcnt |= kDispcntBg0;
      c->bgcnt[0] = 2 << 2 | ((beam_slot ? kSbBg3Slot1 : kSbBg3Slot0) + 1) << 8;
      c->bld[0] = 1 | BlendLayers(math & ~4) << 8 | 1 << 6;
      c->bld[1] = 16 | 16 << 8;
      return 4 | kBeamAdd;
    }
    // Without one, the GBA brightens the layers inside its window, by the
    // mean of the color's channels (the beam over a dark room: white, as
    // bright as the color).
    c->bld[0] = BlendLayers(math) | 2 << 6;
    c->bld[2] = fixed * 11 >> 6;  // /93 * 16
    return kFixedInWindows;
  }
  if (r->cgwsel & 2) {
    uint8_t sub = SubLayers(r);
    // A power bomb's blast, with BG3 transparent: the fixed color's added
    // everywhere, in the palettes (FixedMath). Or BG3 solid black: BG3
    // itself is the fixed color, in the window that hides it on the SNES
    // and nowhere else (UpdatePalette patches its colors).
    int tint = BlastTint(r);
    if (tint == kBg3Clear)
      return 0;
    // (Or the palette subtracts, as SubtractWeight chose.)
    if (subtract && (subtract_color & 0x8000) && !subtract_palettes)
      return 0;
    if (subtract && (subtract_color & 0x8000)) {
      // The palette subtracts the brightest of BG3's colors, and BG3 adds
      // back what's too much (see subtract_color).
      c->dispcnt |= kDispcntBg0;
      c->bgcnt[0] &= ~3;
      c->bld[0] = 1 | BlendLayers(math & (r->tm | 0x20)) << 8 | 1 << 6;
      c->bld[1] = 16 | 16 << 8;
      return 4;
    }
    if (!sub || (subtract && !(r->tm & 3)))
      return 0;
    // Where it's transparent, the sub screen's the fixed color. Where
    // windows hide its layers (a power bomb's, all but its explosion), it's
    // that: the main screen brightens, by the brightest of the color's
    // channels (the GBA can't make it a color), where Windows has the
    // GBA's windows do it. Its layers don't show, here, elsewhere. (But on
    // the lines a blast's window isn't on, BG3 shows all along them, the
    // water's or the fog's tint, below.)
    uint8_t windowed = WindowedSub(r, sub);
    int fixed = r->coldata[0] > r->coldata[1] ? r->coldata[0] : r->coldata[1];
    fixed = fixed > r->coldata[2] ? fixed : r->coldata[2];
    if (windowed && fixed && !subtract && !tint && !blast_nowin && FixedShows(r, windowed)) {
      c->bld[0] = BlendLayers(math & ~sub) | 2 << 6;
      c->bld[2] = fixed * 33 >> 6;  // /31 * 16
      return windowed | kFixedInWindows;
    }
    if (!(r->tm & 3) && (math & r->tm & 4) && !subtract) {
      // Only BG3 on the main screen, and the room on the sub screen
      // (Crateria's and Maridia's fog, for one). BG3 is GBA BG0, which
      // would be in front of the others at the same priority: the room
      // shows as usual (and Samus), with BG3 on top added to it.
      uint8_t room = r->ts & ~r->tm & 0x13;
      c->dispcnt |= BlendLayers(room) << 8;
      c->bgcnt[0] &= ~3;
      c->bld[0] = 1 | BlendLayers(room | 0x20) << 8 | 1 << 6;
      c->bld[1] = half ? 8 | 8 << 8 : 16 | 16 << 8;
      return sub;
    }
    // The sub screen's layers on top.
    uint16_t on_top = BlendLayers(sub);
    c->dispcnt |= on_top << 8;
    for (int i = 0; i < 4; i++) {
      if (on_top & (1 << i))
        c->bgcnt[i] &= ~3;
    }
    if (bg3_mid)
      c->bgcnt[0] |= 2;  // in front of BG2 only
    // Outside gameplay, behind the sprites of SNES priority 3, GBA 1: the
    // text of the planet map (BG3's hexagons are on its sub screen).
    if (squash && !bg3_mid) {
      for (int i = 0; i < 4; i++) {
        if (on_top & (1 << i))
          c->bgcnt[i] |= 1;
      }
    }
    c->bld[0] = BlendLayers(sub) | BlendLayers(math & ~sub) << 8 | 1 << 6;
    // Subtracted (Maridia's and Tourian's shadows): the GBA can't, but
    // where the sub screen's layers are, it darkens what's under them,
    // about as much as their colors do, and they don't show.
    c->bld[1] = subtract ? subtract_weight << 8 : half ? 8 | 8 << 8 : 16 | 16 << 8;
    return tint ? sub | kBlastTint : sub;
  }
  return 0;
}

// What the SNES's windows do in each of the GBA's regions (see Windows):
// WININ, WINOUT, or 0 if nothing.
ROM_CODE static uint32_t WindowRegions(const PpuRegs *r, uint8_t sub, bool blending) {
  bool fixed = sub & kFixedInWindows, tint = sub & kBlastTint, beam = sub & kBeamAdd;
  sub &= 7;
  int math_region = (r->cgwsel >> 4) & 3, clip_region = (r->cgwsel >> 6) & 3;
  bool used = false;
  uint8_t bits[3];
  for (int k = 0; k < 3; k++) {
    bool in1 = k == 0, in2 = k == 1;
    // SNES BG1-BG4 and OBJ masked, by its main or sub screen.
    uint8_t masked = 0;
    for (int i = 0; i < 5; i++) {
      int sel = (i < 4 ? r->wsel[i >> 1] >> ((i & 1) * 4) : r->wsel[2]) & 15;
      int logic = i < 4 ? (r->wbglog >> (2 * i)) & 3 : r->wobjlog & 3;
      uint8_t w = sub & (1 << i) ? r->tsw : r->tmw;
      if ((w & (1 << i)) && InWindows(sel, logic, in1, in2))
        masked |= 1 << i;
    }
    bool color = InWindows(r->wsel[2] >> 4, (r->wobjlog >> 2) & 3, in1, in2);
    bool no_math = NoMath(r, math_region, in1, in2);
    bool clip = clip_region == 3 || (clip_region == 1 && !color) || (clip_region == 2 && color);
    if (clip)
      masked |= 0x1f & ~sub;
    // The tint, where the SNES hides BG3.
    if (tint)
      masked ^= sub;
    // The beam's layer isn't the SNES's BG3 (not on its sub screen at all):
    // only the color window hides it.
    if (beam)
      masked &= ~sub;
    if (fixed) {
      // The fixed color where the sub screen's layers are hidden.
      no_math |= (masked & sub) != sub;
      masked &= ~sub;
      used = true;
    }
    if (no_math)
      masked |= sub;  // as the sub screen doesn't show
    used |= masked || (no_math && blending);
    bits[k] = (uint8_t)(0x1f & ~BlendLayers(masked)) | (no_math ? 0 : 0x20);
  }
  return used ? (bits[0] | bits[1] << 8) | (uint32_t)bits[2] << 16 : 0;
}

// A window's GBA WINxH, from the SNES's edges (WH0-WH3). For none, from
// the right edge to it: the GBA's (0, 0) is all the line.
ROM_CODE static uint16_t WindowH(int left, int right) {
  left -= kXOffset;
  right += 1 - kXOffset;
  left = left < 0 ? 0 : left;
  right = right > 240 ? 240 : right;
  return left < right ? left << 8 | right : 240 << 8 | 240;
}

// The SNES's windows, as the GBA's: window 1 is WIN0 and 2 is WIN1 (where
// they overlap, the GBA's in WIN0 only). For each GBA region, which layers
// show (the SNES's main screen layers but where TMW masks them, the sub
// screen's on top, |sub|, but where TSW does), and whether color math is
// on (CGWSEL's region, by the color window), or the main screen's black
// (it's clipped: showing the backdrop, here). Only with the tiled layers.
ROM_CODE static void Windows(DisplaySegment *c, const PpuRegs *r, uint8_t sub) {
  // The regions, for the settings: they change seldom, unlike the edges.
  static uint32_t last_key[3] IWRAM_BSS, regions IWRAM_BSS;
  bool blending = c->bld[0];
  uint32_t key[3] = {
    r->wsel[0] | r->wsel[1] << 8 | r->wsel[2] << 16 | (uint32_t)r->wbglog << 24,
    r->wobjlog | r->tmw << 8 | r->tsw << 16 | (uint32_t)(r->cgwsel & 0xf0) << 24,
    sub | blending << 8 | split_bg2 << 9,
  };
  if (key[0] != last_key[0] || key[1] != last_key[1] || key[2] != last_key[2]) {
    last_key[0] = key[0];
    last_key[1] = key[1];
    last_key[2] = key[2];
    regions = WindowRegions(r, sub, blending);
  }
  if (!regions)
    return;
  c->dispcnt |= 3 << 13;
  c->win[0] = regions;
  c->win[1] = regions >> 16;
}

// The windows' GBA WIN0H and WIN1H, from the SNES's edges, for the GBA lines
// from |g| to |end| (the SNES's from |s|).
// (A window's edges often stay as they were from line to line, as the other's go.)
ROM_CODE static void LineWindows(int g, int end, int s) {
  uint32_t in[2] = {~0u, ~0u}, out[2] = {0, 0};
  for (; g < end; g++, s++) {
    uint32_t wh = line_wh[s];
    for (int w = 0; w < 2; w++) {
      uint32_t edges = (wh >> (16 * w)) & 0xffff;
      if (edges != in[w]) {
        in[w] = edges;
        out[w] = WindowH(edges & 0xff, edges >> 8);
      }
    }
    line_windows[g] = out[0] | out[1] << 16;
  }
}

// Whether the SNES line |s| has a window (either) on the GBA's screen.
ROM_CODE static bool LineHasWindow(int s) {
  uint32_t wh = line_wh[s];
  return WindowH(wh & 0xff, wh >> 8 & 0xff) != (240 << 8 | 240) ||
         WindowH(wh >> 16 & 0xff, wh >> 24) != (240 << 8 | 240);
}

// The streams of |n| (at least 1) lines (not in frames with Mode 7), from GBA
// line |g| and its SNES line |s|: their scrolling, as the GBA's. (The SNES
// shows a background's row VOFS + 1 on its first line.) In assembly, for
// one stm to store a line, from registers that go up: the compiler stores
// the words one or two at a time.
_Static_assert(sizeof(line_ofs[0]) == 16 && sizeof(*(uint32_t(*)[4])0) == 16,
               "ScrollStream steps a line_ofs row and a stream line by 4 words (16 bytes)");
__attribute__((noinline)) static void ScrollStream(uint32_t (*out)[4], int n, int s, int g) {
  const uint32_t(*ofs)[4] = &line_ofs[s];
  const uint32_t add = kXOffset | (uint32_t)(s + 1 - g) << 16;
  // A line's words, in r4-r7, are BG3's, the split BG's (BG1's, or BG2's
  // if split_bg2), BG1's and BG2's: from r8-r10, BG1's, BG2's and BG3's.
  if (!split_bg2) {
    __asm__ volatile(
        "1: ldmia %[ofs], {r8, r9, r10}\n\t"
        "add r4, r10, %[add]\n\t"
        "add r6, r8, %[add]\n\t"
        "add r7, r9, %[add]\n\t"
        "mov r5, r6\n\t"
        "stmia %[out]!, {r4-r7}\n\t"
        "add %[ofs], %[ofs], #16\n\t"
        "subs %[n], %[n], #1\n\t"
        "bne 1b"
        : [out] "+r"(out), [ofs] "+r"(ofs), [n] "+r"(n)
        : [add] "r"(add)
        : "r4", "r5", "r6", "r7", "r8", "r9", "r10", "cc", "memory");
  } else {
    __asm__ volatile(
        "1: ldmia %[ofs], {r8, r9, r10}\n\t"
        "add r4, r10, %[add]\n\t"
        "add r6, r8, %[add]\n\t"
        "add r7, r9, %[add]\n\t"
        "mov r5, r7\n\t"
        "stmia %[out]!, {r4-r7}\n\t"
        "add %[ofs], %[ofs], #16\n\t"
        "subs %[n], %[n], #1\n\t"
        "bne 1b"
        : [out] "+r"(out), [ofs] "+r"(ofs), [n] "+r"(n)
        : [add] "r"(add)
        : "r4", "r5", "r6", "r7", "r8", "r9", "r10", "cc", "memory");
  }
}

// |n| lines of one stream line |row|, in stm of 8 registers (two lines).
__attribute__((noinline)) static void FillLines(uint32_t (*out)[4], int n, const uint32_t *row) {
  __asm__ volatile(
      "ldmia %[row], {r4-r7}\n\t"
      "mov r8, r4\n\t"
      "mov r9, r5\n\t"
      "mov r10, r6\n\t"
      "mov r11, r7\n\t"
      "subs %[n], %[n], #2\n\t"
      "bmi 2f\n\t"
      "1: stmia %[out]!, {r4-r11}\n\t"
      "subs %[n], %[n], #2\n\t"
      "bpl 1b\n\t"
      "2: cmn %[n], #1\n\t"
      "stmiaeq %[out]!, {r4-r7}"
      : [out] "+r"(out), [n] "+r"(n)
      : [row] "r"(row)
      : "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "cc", "memory");
}

// And |n| (at least 1) lines whose rows differ in one word only: |row| (its fifth word the ADD
// to the scrolling) but for that word, which is each line's in |ofs| (the first's, then a row
// on: 16 bytes) plus ADD. In assembly, like ScrollStream; one for each word (BG3's, BG2's, and
// BG2's when it is the split BG, which is in the stream twice).
#define FILL_VARYING(name, T, COPY)                                                            \
  __attribute__((naked, noinline)) static void name(uint32_t(*out)[4], int n,                 \
                                                    const uint32_t *ofs, const uint32_t *row) { \
    __asm__ volatile("push {r4-r7}\n\t"                                                        \
                     "ldr r12, [r3, #16]\n\t"                                                  \
                     "ldmia r3, {r4-r7}\n\t"                                                   \
                     "1: ldr " T ", [r2], #16\n\t"                                             \
                     "add " T ", " T ", r12\n\t" COPY "stmia r0!, {r4-r7}\n\t"                 \
                     "subs r1, r1, #1\n\t"                                                     \
                     "bne 1b\n\t"                                                              \
                     "pop {r4-r7}\n\t"                                                         \
                     "bx lr");                                                                 \
  }
FILL_VARYING(FillBg3, "r4", "")
FILL_VARYING(FillBg2, "r7", "")
FILL_VARYING(FillBg2Split, "r7", "mov r5, r7\n\t")
#undef FILL_VARYING

// The lines of ScrollStream's, in runs where line_ofs has the one row (see SnesHw_OfsRunStarts),
// but for one word at most: those are one stream line, filled by FillLines (or FillBg2/FillBg3). The
// same run is often there already, from the frame before last (this stream's last): then it isn't
// written again, EWRAM's stores being what they cost. (|fill_runs|, by stream, are the runs as
// written: first line and lines; |new_runs| this frame's so far.)
enum { kFillRuns = 8 };
static uint8_t fill_runs[2][kFillRuns][2], new_runs[kFillRuns][2];
static uint8_t num_fill_runs[2], num_new_runs, old_run;

ROM_CODE static void ScrollRuns(uint32_t (*out)[4], int n, int s, int g) {
  int count, varying;
  const uint8_t *starts = SnesHw_OfsRunStarts(&count, &varying);
  // (Only BG2's and BG3's, that differ with HDMA in the game.)
  if (!starts || (varying != 0 && varying != 2 && varying != 4)) {
    ScrollStream(out, n, s, g);
    return;
  }
  int k = 0;
  while (k + 1 < count && starts[k + 1] <= s)
    k++;
  for (;;) {
    int next = k + 1 < count ? starts[k + 1] : kSnesLines;
    int m = next - s < n ? next - s : n;
    if (m < 3) {
      ScrollStream(out, m, s, g);
    } else {
      const uint32_t *ofs = line_ofs[s];
      const uint32_t add = kXOffset | (uint32_t)(s + 1 - g) << 16;
      const uint32_t row[5] = {ofs[2] + add, (split_bg2 ? ofs[1] : ofs[0]) + add, ofs[0] + add,
                               ofs[1] + add, add};
      if (varying == 4) {
        FillBg3(out, m, &ofs[2], row);
      } else if (varying) {
        (split_bg2 ? FillBg2Split : FillBg2)(out, m, &ofs[1], row);
      } else {
        bool same = false;
        if (m >= 4 && num_new_runs < kFillRuns) {
          new_runs[num_new_runs][0] = g;
          new_runs[num_new_runs++][1] = m;
          while (old_run < num_fill_runs[back] && fill_runs[back][old_run][0] < g)
            old_run++;
          same = old_run < num_fill_runs[back] && fill_runs[back][old_run][0] == g &&
                 fill_runs[back][old_run][1] == m && out[0][0] == row[0] && out[0][1] == row[1] &&
                 out[0][2] == row[2] && out[0][3] == row[3];
        }
        if (!same)
          FillLines(out, m, row);
      }
    }
    n -= m;
    if (!n)
      return;
    out += m;
    s += m;
    g += m;
    k++;
  }
}

// Whether two lines' registers are the same, but for the scrolling: then
// so are their GBA registers but for the stream. (Maridia's water and
// Norfair's heat have different scrolling on every line.)
_Static_assert(sizeof(PpuRegs) == 64 && offsetof(PpuRegs, hofs) == 10 &&
                   offsetof(PpuRegs, vofs) + sizeof(((PpuRegs *)0)->vofs) == 26,
               "SameButScrolling's layout");
static bool SameButScrolling(const PpuRegs *a, const PpuRegs *b) {
  // A word at a time (line_states is aligned): halfwords 0-4 and 13-31.
  // (From the last: it's the layers on (TM), at the end, that differ most.)
  const uint32_t *x = (const uint32_t *)a, *y = (const uint32_t *)b;
  for (int i = 15; i >= 7; i--) {
    if (x[i] != y[i])
      return false;
  }
  return x[0] == y[0] && x[1] == y[1] && !((x[2] ^ y[2]) & 0xffff) && !((x[6] ^ y[6]) >> 16);
}

// Whether BG3 goes in front (see bg3_hi), for the line |r| on GBA line |g|
// (SNES line |s|): if the rows of its map that the lines with its state
// (to the GBA line before |end|) show from there on have priority tiles.
// (The HUD's do, and some rooms' BG3 below it in the same map, all behind,
// doesn't: Phantoon's room's, opaque black.)
__attribute__((noinline, minsize)) static bool Bg3Front(const PpuRegs *r, int s, int g, int end) {
  int slot = Bg3Slot(r->bgsc[2]);
  if (!(r->bgmode & 8) || !bg3_hi_total[slot])
    return false;
  // 64 rows high, the second screen's rows 32-63; wider, both screens'
  // rows (as UpdateMaps has them: of 64x64, the top two). A row has two
  // chunks, in a halfword, and the other screen's are 32 halfwords on.
  uint8_t sc = r->bgsc[2];
  int rows = (sc & 3) == 2 ? 63 : 31, wide = sc & 1 ? 32 : 0;
  const uint16_t *hi = bg3_hi[slot].rows;
  // A line every 8 (a row's height), and the last.
  for (int t = s;;) {
    int row = ((t + 1 + VOFS(t, 2)) >> 3) & rows;
    if (hi[row] | (wide ? hi[wide + row] : 0))
      return true;
    if (g == end - 1)
      return false;
    g = g + 8 < end ? g + 8 : end - 1;
    t = SnesLine(g);
  }
}

// The scrolling of a line with another mode in a frame with Mode 7, as the
// segment's, in place of the stream, which is BG2's transformation.
// (The SNES shows a background's row VOFS + 1 on its first line.)
ROM_CODE static void BuildLineBesideMode7(LineRegs *l, int s, int g) {
  int split = split_bg2;
  // (By halfwords: ofs isn't word aligned, and the ARM would store a word
  // at the word it's in.)
  const uint32_t *ofs = line_ofs[s], add = kXOffset | (uint32_t)(s + 1 - g) << 16;
  const uint32_t in[4] = {ofs[2] + add, ofs[split] + add, ofs[0] + add, ofs[1] + add};
  for (int i = 0; i < 4; i++) {
    l->ctl.ofs[2 * i] = (uint16_t)in[i];
    l->ctl.ofs[2 * i + 1] = in[i] >> 16;
  }
  // BG2's identity, for what it's worth.
  l->stream[0] = 0x100;
  l->stream[1] = 0x100 << 16;
  l->stream[2] = l->stream[3] = 0;
}

// The registers BuildLine last made the GBA's of, but for the scrolling,
// in this frame.
static const PpuRegs *built_regs;

// The lines from |g|, SNES line |s|, to the one before |end| have the state
// |r|: |l| gets the first's registers, and for Mode 7 or a frame with it,
// its stream (see ScrollStream and Mode7Stream for the others').
__attribute__((noinline)) static void BuildLine(LineRegs *l, const PpuRegs *r, int s, int g,
                                                int end) {
  if ((r->bgmode & 7) == 7) {
    BuildLineMode7(l, r, s);
    return;
  }
  int split = split_bg2;
  if (!frame_mode7 && built_regs && (built_regs == r || SameButScrolling(built_regs, r)))
    goto scrolling;
  built_regs = r;
  uint16_t dispcnt = frame_mode7 ? kDispcntMode1 : kDispcntMode0;
  if (!(r->inidisp & 0x80)) {
    // With Mode 7 lines in the frame, BG1 and BG2's VRAM holds Mode 7's.
    if (!frame_mode7) dispcnt |= BlendLayers(r->tm & 3) << 8;
    if (r->tm & 4) dispcnt |= kDispcntBg0;
    if (r->tm & 16) dispcnt |= kDispcntObj;
  }
  l->ctl.dispcnt = dispcnt;
  int bg3_sb = Bg3Slot(r->bgsc[2]) ? kSbBg3Slot1 : kSbBg3Slot0;
  int bg3_front = Bg3Front(r, s, g, end);
  l->ctl.bgcnt[0] = (bg3_front ? 0 : 3) | 2 << 2 | bg3_sb << 8 | BgSize(r->bgsc[2]) << 14;
  // The SNES background split into GBA BG1 and BG2 or BG3.
  l->ctl.bgcnt[1] = (1 - bg3_mid) | kSbBg1Hi << 8 | BgSize(r->bgsc[split]) << 14;
  l->ctl.bgcnt[2] = (2 - bg3_mid) | kSbBg1Lo << 8 | BgSize(r->bgsc[0]) << 14;
  l->ctl.bgcnt[3] = (2 - bg2_over_lo) | kSbBg2 << 8 | BgSize(r->bgsc[1]) << 14;
  uint8_t sub = ColorMath(&l->ctl, r);
  l->ctl.win[0] = l->ctl.win[1] = 0;
  // (As a rule, no window is on for anything.)
  if (((r->wsel[0] | r->wsel[1] | r->wsel[2]) & 0xaa) && !frame_mode7 && !(r->inidisp & 0x80))
    Windows(&l->ctl, r, sub);
scrolling:;
  if (frame_mode7)
    BuildLineBesideMode7(l, s, g);
}

// A power bomb's blast in a room whose BG3 has something in it: its window's
// lines go on as the fixed color; the others', BG3 as it is (blast_nowin).
// The end of the lines from |g| (SNES line |s|) that are alike, no later
// than |end|.
ROM_CODE __attribute__((noinline)) static int BlastLineEnd(const PpuRegs *r, int s, int g, int end) {
  bool nowin = false;
  if (blast_on && blast_bg3 == kBg3Mixed && PowerBombRegs(r)) {
    bool has = LineHasWindow(s);
    int t = g + 1;
    while (t < end && LineHasWindow(s + t - g) == has)
      t++;
    end = t;
    nowin = !has;
  }
  if (nowin != blast_nowin)
    built_regs = NULL;
  blast_nowin = nowin;
  return end;
}

// Whether all of the main screen's colors are black: the backdrop, BG1's and
// BG2's palettes and the sprites' (a door's fade clears them, and leaves
// BG3's, the HUD's). Subtracting from black is black, but SubtractWeight's
// color is of the room before, and BG3 would add all of it back: a purple
// screen.
ROM_CODE static bool MainScreenBlack(void) {
  if (g_cgram[0])
    return false;
  const uint32_t *c = (const uint32_t *)&g_cgram[32];
  for (int i = 0; i < 112; i++) {
    if (c[i])
      return false;
  }
  return true;
}

// But for first_line. (Not with memcmp: it's in Thumb code, and slow.)
static bool SameSegment(const DisplaySegment *a, const DisplaySegment *b) {
  const uint16_t *x = &a->dispcnt, *y = &b->dispcnt;
  for (int i = 0; i < 18; i++) {
    if (x[i] != y[i])
      return false;
  }
  return true;
}

ROM_CODE int Render_GameplayYOffset(void) {
  return hud ? y_offset : 0;
}

ROM_CODE static void UpdateYOffset(void) {
  int target;
  if (hud) {
    // Samus in the middle of the gameplay area, as far as it goes: by her
    // feet, standing's 21 lines below her middle (which moves as she
    // crouches or curls up), and only once she's 12 lines from where she
    // would be (not as she ducks, or walks over a bump).
    int feet = (int16_t)(samus_y_pos + samus_y_radius - layer1_y_pos);
    int middle = kHudLines + (kGbaLines - kGbaHudLines) / 2 + y_offset;
    int away = feet - 21 - middle;
    target = y_offset + (away > 12 ? away - 12 : away < -12 ? away + 12 : 0);
    // Ceres's escape, in Ridley's room: its message (TIME BOMB SET!) is on
    // the SNES's lines 64-87, out of sight under the HUD as Samus stands on
    // the floor. In sight, but for her feet.
    if (ceres_status == 2 && room_ptr == 0xe0b5 && target > kCeresMessageYOffset)
      target = kCeresMessageYOffset;
    target = target < 0 ? 0 : target > kMaxYOffset ? kMaxYOffset : target;
  } else {
    target = kMaxYOffset / 2;
  }
  int d = target - y_offset;
  y_offset += d > 4 ? 4 : d < -4 ? -4 : d;
}

// The 16 low bits of |x| as 32, each twice (a mask of tile rows as one of
// chunks, two to a row).
static inline uint32_t SpreadBits(uint32_t x) {
  x &= 0xffff;
  x = (x | x << 8) & 0x00ff00ff;
  x = (x | x << 4) & 0x0f0f0f0f;
  x = (x | x << 2) & 0x33333333;
  x = (x | x << 1) & 0x55555555;
  return x | x << 1;
}

ROM_CODE static void SetSplit(bool split) {
  split_bg2 = split;
  bg2_over_lo = false;
  bg12_layers[0] = split ? 4 : 6;
  bg12_layers[1] = split ? 10 : 8;
}

// The pause menu's equipment screen, and its fades.
static inline bool EquipmentScreen(void) {
  return game_state >= 13 && game_state <= 16 && pause_screen_mode == 1;
}

ROM_CODE static bool TileIsBlank(const uint16_t *t);

// Whether BG2 shows tiles without the priority bit, but the transparent
// ones, as of the line |r| (the gameplay area's: its scrolling and the maps as
// converted). It stops at the first.
ROM_CODE static bool Bg2HasLoTiles(const PpuRegs *r) {
  uint8_t sc = r->bgsc[1];
  uint16_t base = (r->bgnba[0] >> 4) << 12;
  int top = SnesLine(hud ? kGbaHudLines : 0);
  int x = HOFS(text_line, 1) + kXOffset, y = VOFS(text_line, 1) + top + 1;
  int rows = (((y & 7) + SnesLine(kGbaLines - 1) - top) >> 3) + 1;
  int cols = (((x & 7) + 239) >> 3) + 1;
  int last = -1;
  for (int i = 0; i < rows; i++) {
    int ty = ((y >> 3) + i) & 63;
    for (int j = 0; j < cols; j++) {
      int tx = ((x >> 3) + j) & 63;
      int screen = (tx >> 5) & (sc & 1);
      screen += (ty >> 5) & (sc >> 1 & 1) ? (sc & 1) + 1 : 0;
      uint16_t entry = g_vram[(((sc & 0xfc) << 8) + screen * 0x400 + (ty & 31) * 32 + (tx & 31)) & 0x7fff];
      int tile = entry & 0x3ff;
      if ((entry & 0x2000) || tile == last)
        continue;
      if (!TileIsBlank(&g_vram[(base + tile * 16) & 0x7fff]))
        return true;
      last = tile;
    }
  }
  return false;
}

// The entry of background |bg|'s map at (|bx|, |by|) in its pixels (wrapped to 1024).
ROM_CODE static inline uint16_t BgEntry(const PpuRegs *r, int bg, int bx, int by) {
  uint8_t sc = r->bgsc[bg];
  int screen = (bx >> 8) & (sc & 1);
  screen += (by >> 8) & (sc >> 1 & 1) ? (sc & 1) + 1 : 0;
  return g_vram[((sc & 0xfc) << 8 | screen << 10 | ((by >> 3) & 31) << 5 | ((bx >> 3) & 31)) &
                0x7fff];
}

// Whether the 4bpp tile of background 1 or 2 (|bg|, by BG12NBA) |tile| has no pixel.
ROM_CODE static bool BgTileBlank(const PpuRegs *r, int bg, int tile) {
  const uint32_t *t = (const uint32_t *)&g_vram[(((r->bgnba[0] >> (bg * 4) & 15) << 12) + tile * 16) & 0x7fff];
  for (int i = 0; i < 8; i++) {
    if (t[i])
      return false;
  }
  return true;
}

// Which of the two ways of splitting BG1 and BG2 into the GBA's layers gets
// more of the picture wrong, by the tiles on show where a priority tile of
// BG2 (with a pixel) lies over one of BG1's, in the middle of each
// tile, in every other row: BG2 split puts its priority tiles in front of
// all of BG1's (wrong over BG1's priority tiles), BG2 not split puts them
// behind all of BG1's (wrong over BG1's other tiles). True if the second is
// worse; or, with none of BG2's over one of BG1's, |by_count|: the count of their
// priority tiles says (as sprites' order with BG2's priority tiles depends on it too).
ROM_CODE static bool Bg2HiHiddenByBg1(const PpuRegs *r, bool by_count) {
  int front = 0, behind = 0;
  int y0 = SnesLine(hud ? kGbaHudLines : 0), y1 = SnesLine(kGbaLines - 1);
  for (int y = y0 + 4; y < y1; y += 16) {
    int by1 = (VOFS(text_line, 0) + y + 1) & 0x3ff, by2 = (VOFS(text_line, 1) + y + 1) & 0x3ff;
    for (int x = kXOffset + 4; x < kXOffset + 240; x += 8) {
      uint16_t e2 = BgEntry(r, 1, (HOFS(text_line, 1) + x) & 0x3ff, by2);
      if (!(e2 & 0x2000) || BgTileBlank(r, 1, e2 & 0x3ff))
        continue;
      uint16_t e1 = BgEntry(r, 0, (HOFS(text_line, 0) + x) & 0x3ff, by1);
      if (BgTileBlank(r, 0, e1 & 0x3ff))
        continue;
      if (e1 & 0x2000)
        front++;
      else
        behind++;
    }
  }
  return front | behind ? behind > front : by_count;
}

// Chooses split_bg2 (or bg2_over_lo) from BG1's and BG2's priority tiles (as the
// frame before converted their maps) and the line |r|'s scrolling.
ROM_CODE static void ChooseSplit(const PpuRegs *r) {
  int shown[2];
  for (int bg = 0; bg < 2; bg++) {
    uint8_t sc = r->bgsc[bg];
    int screens = (sc & 3) ? 2 : 1;
    // The columns the GBA shows, 128 pixels wide, two to a screen (in all
    // the rows, and both screens of a map two screens high).
    int wide = sc & 1, n = 0;
    int first = (HOFS(text_line, bg) + kXOffset) >> 7, last = (HOFS(text_line, bg) + kXOffset + 239) >> 7;
    for (int j = first; j <= last; j++) {
      int col = j & (wide ? 3 : 1);
      n += bg12_hi_cols[bg][col];
      if (!wide && screens == 2)
        n += bg12_hi_cols[bg][2 + col];
    }
    shown[bg] = n;
  }
  // The pause menu's equipment screen (and its fades) puts its title (SAMUS) on
  // BG1's priority tiles, over BG2's priority tiles' (the map's, BRINSTAR),
  // which split_bg2 would put in front.
  // BG2 only on the sub screen, added to BG3 on the main one (Crocomire's
  // fight): its priority tiles are his body, in front of BG1's others (the
  // wall), behind BG1's (the floor); there are more of BG1's, but the
  // floor is out of his way.
  // Otherwise by the picture (Bg2HiHiddenByBg1): BG2's priority tiles are
  // Kraid's body, the whole of him, in front of the cave's wall in BG1 and
  // behind its few priority tiles, and Mother Brain's, as she stands, in front
  // of a room that's all but a few of BG1's tiles; a count of each layer's
  // priority tiles, which went one way and the other as the screen shook
  // (Kraid, gone for a second or so) or stayed the wrong one (Mother Brain,
  // gone from the waist up through the fight: 128 of BG2's to 268 of BG1's),
  // can't tell. Every fourth frame, as the scrolling and the tiles change
  // slowly.
  // Better yet, if BG2 has no others than priority tiles (and no sub screen
  // adds layers that go in front): BG2 in front of BG1's others only, as the
  // SNES has it (bg2_over_lo: Kraid's room, where his body goes behind the
  // floor, in front of the wall).
  // And a BG2 that the game turns off and on every other frame (Mother
  // Brain's body, as she dies) keeps the split it had: each change of it
  // converts both maps again, twice the frame's other work.
  static uint8_t age, off;
  static bool hidden;
  bool sub_only = (r->cgwsel & 2) && (r->ts & 2) && !(r->tm & 2);
  bool both = !sub_only && shown[1] && (r->tm & 2);
  if (both && !(age++ & 3))
    hidden = !shown[0] || Bg2HiHiddenByBg1(r, 2 * shown[1] >= shown[0]);
  bool over = !EquipmentScreen() && both && !(r->ts & 3) && !Bg2HasLoTiles(r);
  bool split = !EquipmentScreen() && (sub_only ? shown[1] > 0 : both && hidden) && !over;
  if (!sub_only && shown[1] && !(r->tm & 2) && off < 3)
    split = split_bg2, off++;
  else if (r->tm & 2)
    off = 0;
  SetSplit(split);
  bg2_over_lo = over;
}

// The CGRAM index of background |bg|'s pixel at SNES (x, line), with the
// line |r|'s registers, or 0 where it's transparent.
ROM_CODE static int BgPixel(const PpuRegs *r, int bg, int x, int line) {
  uint8_t sc = r->bgsc[bg];
  int bx = (HOFS(line, bg) + x) & 0x3ff, by = (VOFS(line, bg) + line + 1) & 0x3ff;
  int screen = (bx >> 8) & (sc & 1);
  screen += (by >> 8) & (sc >> 1 & 1) ? (sc & 1) + 1 : 0;
  uint16_t e = g_vram[((sc & 0xfc) << 8 | screen << 10 | ((by >> 3) & 31) << 5 | ((bx >> 3) & 31)) &
                      0x7fff];
  int tx = e & 0x4000 ? 7 - (bx & 7) : bx & 7, ty = e & 0x8000 ? 7 - (by & 7) : by & 7;
  int nba = (bg == 2 ? r->bgnba[1] : r->bgnba[0] >> (bg * 4)) & 15;
  int words = bg == 2 ? 8 : 16;
  const uint16_t *t = &g_vram[((nba << 12) + (e & 0x3ff) * words + ty) & 0x7fff];
  int shift = 7 - tx;
  int c = (t[0] >> shift & 1) | (t[0] >> (shift + 8) & 1) << 1;
  if (bg == 2)
    return c ? ((e >> 10) & 7) * 4 + c : 0;
  c |= (t[8] >> shift & 1) << 2 | (t[8] >> (shift + 8) & 1) << 3;
  return c ? ((e >> 10) & 7) * 16 + c : 0;
}

ROM_CODE static int Luma(uint16_t color) {
  return (color & 31) + (color >> 5 & 31) + (color >> 10 & 31);
}

// What's left of |m|'s channels after |s|'s are taken from them.
ROM_CODE static int LumaLeft(uint16_t m, uint16_t s) {
  int left = 0;
  for (int i = 0; i < 15; i += 5) {
    int d = (m >> i & 31) - (s >> i & 31);
    left += d > 0 ? d : 0;
  }
  return left;
}

// SubtractWeight's samples, k of 40 in 5 rows of 8: staggered, so that
// they fall on all of a tile's rows and columns, not one (BG3's shadows
// can be a pattern of thin lines).
ROM_CODE static int SampleX(int k) {
  return kXOffset + 11 + (k & 7) * 29 + (k >> 3) * 5;
}

ROM_CODE static int SampleLine(int k) {
  return SnesLine(9 + (k >> 3) * 29 + (k & 7) * 3);
}

// What's left of each of |m|'s channels with |s|'s taken away, |add|'s
// added back (to 31 at most), against what the SNES makes of |m| less |c|:
// how far off, in all.
ROM_CODE static int SubtractError(uint16_t m, uint16_t s, uint16_t add, uint16_t c) {
  int error = 0;
  for (int i = 0; i < 15; i += 5) {
    int v = (m >> i & 31) - (s >> i & 31), want = (m >> i & 31) - (c >> i & 31);
    v = (v < 0 ? 0 : v) + (add >> i & 31);
    v = v > 31 ? 31 : v;
    want = want < 0 ? 0 : want;
    error += v > want ? v - want : want - v;
  }
  return error;
}

// How far off adding back would be, in a channel, with |v| subtracted in
// the palette, at SubtractWeight's 40 samples: their main screen's values
// (|m|), the sub screen's (|c|, or 32 where it's transparent, and nothing's
// added back) and what the SNES makes of them (|want|). (In IWRAM: a
// search runs it a dozen times a frame.) By the squares of the errors: with
// BG3's shadow a dither of two colors (Maridia's sand), a few big ones are
// worse than many small. (AddedBackError has the errors themselves, to
// compare with darkening's and the average's.)
static int AddBackError(const uint8_t *m, const uint8_t *c, const uint8_t *want, int v) {
  int error = 0;
  for (int k = 0; k < 40; k++) {
    int got = (m[k] > v ? m[k] - v : 0) + (v > c[k] ? v - c[k] : 0);
    got = got > 31 ? 31 : got;
    error += (got - want[k]) * (got - want[k]);
  }
  return error;
}


// For the line |r|, which subtracts the sub screen, by a few dozen of its
// pixels on show and the main screen's there: the GBA's way that comes
// closest, of
// - darkening where the sub screen's layer with the most pixels
//   (subtract_layer) is, by how much of the main screen's brightness is left
//   there, in 16ths (the result);
// - if the sub screen is all over the screen, subtracting the average of
//   its colors in the palette (subtract_color, with subtract_palettes 0);
// - if it's BG3 all over the screen, subtracting a color in the palette,
//   and BG3 adding back what's too much, by its colors: the color, channel
//   by channel, that's closest (subtract_color, with the BG3 palettes it
//   uses, subtract_palettes).
// Over sixteen frames, a slice a frame: the colors and the scrolling change
// slowly. (No division: see cmake/compiler-rt.cmake.)
ROM_CODE static int SubtractWeight(const PpuRegs *r) {
  // A slice a frame, so that no frame does much (it's some 150k cycles in
  // all, from the cartridge): frames 0-4 take 8 samples each, 5 and 6 look
  // at them, 7 sees how far off darkening and the average would be, 8-13
  // search a channel each two, 14 says what's best; 15 rests. If the
  // registers change partway, it starts over.
  enum { kColumns = 8, kRows = 5, kSamples = kColumns * kRows, kPerFrame = 8 };
  enum {
    kLook = kSamples / kPerFrame,
    kErrors = kLook + 2,
    kSearch,
    kDecide = kSearch + 6,
  };
  static uint8_t phase;
  static uint32_t key;
  static uint8_t pixels[3][kSamples], mains[kSamples];
  static uint16_t main_colors[kSamples], sub_colors[kSamples];
  static uint8_t opaque_at[kSamples];
  static int best, weight, darkened, averaged, added_back, darkest[3], brightest[3];
  static int opaque, bg3, sums[3], left_luma, main_luma, lo, hi;
  static uint16_t mean, top;
  static bool add_back;
  uint8_t sub = r->ts & ~r->tm & 7, main = r->tm & 3;
  uint32_t now = r->ts | r->tm << 8 | (uint32_t)r->cgadsub << 16;
  // (Whether the errors are of these registers' first look, or the room's:
  // doors are game states 9-11.)
  static bool fresh = true;
  if (game_state != 8)
    fresh = true;
  if (now != key) {
    key = now;
    phase = 0;
    fresh = true;
  }
  int p = phase;
  phase = (phase + 1) & 15;
  if (!sub) {
    subtract_color = 0;
    return 16;
  }
  if (p < kSamples / kPerFrame) {
    for (int k = p * kPerFrame; k < (p + 1) * kPerFrame; k++) {
      int x = SampleX(k), line = SampleLine(k), m = 0;
      for (int bg = 0; bg < 3; bg++)
        pixels[bg][k] = sub & (1 << bg) ? BgPixel(r, bg, x, line) : 0;
      for (int bg = 0; bg < 2 && !m; bg++) {
        if (main & (1 << bg))
          m = BgPixel(r, bg, x, line);
      }
      mains[k] = m;
    }
    return subtract_weight;
  }
  if (p == kLook) {
    // The layer that takes the most away: with the most pixels on show (in
    // front of the others) that aren't black.
    int count[3] = {0, 0, 0};
    best = 2;
    for (int k = 0; k < kSamples; k++) {
      for (int bg = 0; bg < 3; bg++) {
        if ((sub & (1 << bg)) && pixels[bg][k]) {
          count[bg] += g_cgram[pixels[bg][k]] != 0;
          break;
        }
      }
    }
    for (int bg = 0; bg < 3; bg++) {
      if ((sub & (1 << bg)) && (count[bg] > count[best] || !(sub & (1 << best))))
        best = bg;
    }
    opaque = bg3 = left_luma = main_luma = 0;
    for (int c = 0; c < 3; c++)
      sums[c] = 0, darkest[c] = 31, brightest[c] = 0;
  }
  if (p <= kLook + 1) {
    // The main and sub screens' pixels; and of the sub screen's, how many,
    // how many BG3's, their sums, the darkest and the brightest: half the
    // samples a frame.
    int from_k = p == kLook ? 0 : kSamples / 2;
    for (int k = from_k; k < from_k + kSamples / 2; k++) {
      int m = mains[k], q = 0, from = 0;
      for (int bg = 0; bg < 3 && !q; bg++) {
        if (sub & (1 << bg))
          q = pixels[bg][k], from = bg;
      }
      main_colors[k] = g_cgram[m];
      sub_colors[k] = q ? g_cgram[q] : 0;
      opaque_at[k] = q != 0;
      if (pixels[best][k]) {
        left_luma += LumaLeft(g_cgram[m], g_cgram[pixels[best][k]]);
        main_luma += Luma(g_cgram[m]);
      }
      if (!q)
        continue;
      opaque++;
      bg3 += from == 2;
      for (int c = 0; c < 3; c++) {
        int v = g_cgram[q] >> (5 * c) & 31;
        sums[c] += v;
        brightest[c] = v > brightest[c] ? v : brightest[c];
        darkest[c] = v < darkest[c] ? v : darkest[c];
      }
    }
    if (p == kLook)
      return subtract_weight;
    // (left_luma << 4) / main_luma: at most 16, as what's left is at most
    // what was there.
    weight = 16;
    if (main_luma) {
      weight = 0;
      for (int left = left_luma << 4; left >= main_luma; left -= main_luma)
        weight++;
    }
    if (r->cgadsub & 0x40)
      weight >>= 1;
    // Where the sub screen's transparent, nothing's subtracted (it's the
    // fixed color, black as a rule), nor added back.
    if (opaque < kSamples)
      darkest[0] = darkest[1] = darkest[2] = 0;
    mean = top = 0;
    _Static_assert(kSamples == 40, "the division by 40");
    for (int c = 0; c < 3; c++)
      mean |= ((sums[c] + kSamples / 2) * 1639 >> 16) << (5 * c);
    add_back = opaque && bg3 == opaque;
    return subtract_weight;
  }
  if (p == kErrors) {
    // How far off each would be, at the pixels. (Not halved.)
    darkened = averaged = 0;
    for (int k = 0; k < kSamples; k++) {
      uint16_t m = main_colors[k], c = sub_colors[k];
      uint16_t dark = pixels[best][k] ? 0 : m;
      for (int i = 0; i < 15 && pixels[best][k]; i += 5)
        dark |= ((m >> i & 31) * weight >> 4) << i;
      darkened += SubtractError(dark, 0, 0, c);
      averaged += SubtractError(m, mean, 0, c);
    }
    return subtract_weight;
  }
  if (p < kDecide) {
    // Adding back: a channel each two frames, the color between BG3's
    // darkest and brightest that's closest. (A ternary search: the error
    // falls, then rises. Its first few steps in the first frame.)
    int ch = 5 * ((p - kSearch) >> 1);
    bool first = !((p - kSearch) & 1);
    if (!add_back)
      return subtract_weight;
    uint8_t m[kSamples], c[kSamples], want[kSamples];
    for (int k = 0; k < kSamples; k++) {
      m[k] = main_colors[k] >> ch & 31;
      c[k] = opaque_at[k] ? sub_colors[k] >> ch & 31 : 32;
      int w = m[k] - (opaque_at[k] ? c[k] : 0);
      want[k] = w > 0 ? w : 0;
    }
    if (first)
      lo = darkest[ch / 5], hi = brightest[ch / 5];
    for (int steps = 0; hi - lo > 2 && (!first || steps < 4); steps++) {
      int third = (hi - lo) * 11 >> 5;  // (hi - lo) / 3, as they're 5 bits
      if (AddBackError(m, c, want, lo + third) <= AddBackError(m, c, want, hi - third))
        hi = hi - third;
      else
        lo = lo + third;
    }
    if (first)
      return subtract_weight;
    int best_error = 0x7fffffff, best_s = lo;
    for (int v = lo; v <= hi; v++) {
      int error = AddBackError(m, c, want, v);
      if (error < best_error)
        best_error = error, best_s = v;
    }
    top |= best_s << ch;
    return subtract_weight;
  }
  if (p == kDecide) {
    // Darkening, the average or adding back: the one that's least off,
    // over the last few times (40 pixels are only a sample, and the fog
    // moves; they can be close, and the colors would flicker between
    // them).
    static int error[3];
    // (What adding back, with the colors the search found, would be off by.)
    added_back = 0;
    for (int k = 0; add_back && k < kSamples; k++) {
      uint16_t back = 0;
      for (int i = 0; i < 15 && opaque_at[k]; i += 5) {
        int d = (top >> i & 31) - (sub_colors[k] >> i & 31);
        back |= (d > 0 ? d : 0) << i;
      }
      added_back += SubtractError(main_colors[k], top, back, sub_colors[k]);
    }
    int now_error[3] = {darkened, averaged, add_back ? added_back : 0x1000000};
    for (int i = 0; i < 3; i++)
      error[i] = fresh ? now_error[i] : error[i] + (now_error[i] - error[i]) / 4;
    fresh = false;
    // (Adding back is favored by a quarter: it keeps BG3's pattern and
    // color, which darkening can't, and the errors can be close.)
    int back_error = error[2] - (error[2] >> 2);
    int way = error[1] <= error[0] && error[1] <= back_error ? 1 : back_error < error[0] ? 2 : 0;
    subtract_layer = 1 << best;
    subtract_color = 0;
    if (way == 1) {
      subtract_color = 0x8000 | mean;
      subtract_palettes = 0;
    } else if (way == 2 && add_back) {
      // All of BG3's palettes: those not among the pixels, too.
      subtract_color = 0x8000 | top;
      subtract_palettes = 0xff;
    }
    return weight;
  }
  return subtract_weight;
}

// Whether the line |r| adds BG3 alone, on the sub screen, to BG2 but not
// BG1 (see bg3_mid).
ROM_CODE static bool Bg3Between(const PpuRegs *r) {
  return (r->cgwsel & 2) && !(r->cgadsub & 0x80) && (r->ts & ~r->tm & 7) == 4 &&
         (r->tm & 3) == 3 && (r->cgadsub & 3) == 2;
}

// The GBA shows a background only three lines after DISPCNT turns it on
// partway down the screen (it was the backdrop at the top of the gameplay
// area, below the HUD): segments before one that turns on BG1-BG3 already
// have them on, but blacked out, with BG0 in front of them. (Not in
// segments with sprites, color math or windows: the HUD has none.)
ROM_CODE static void KeepLayersOn(DisplaySegment *segs, int n) {
  uint16_t later = 0;
  for (int i = n - 1; i >= 0; i--) {
    DisplaySegment *c = &segs[i];
    uint16_t hidden = later & ~c->dispcnt;
    if (hidden && !c->bld[0] && !(c->dispcnt & (kDispcntObj | 3 << 13))) {
      c->dispcnt |= hidden;
      c->bgcnt[0] &= ~3;
      c->bld[0] = hidden >> 8 | 3 << 6;
      c->bld[2] = 16;
    }
    later |= c->dispcnt & 0x0e00;
  }
}

// Squashed outside gameplay: switched while the screen's dark, between
// gameplay and the pause screen, the title or a cutscene (a frame of one
// shown as the other would be squashed, or not, wrongly), or at most a
// second after the game state did. Pausing fades the room out (12), and
// unpausing fades it back in (17, 18), as gameplay: the room unsquashed.
ROM_CODE static void SetSquash(void) {
  static uint8_t waited;
  bool want = game_state < 7 || (game_state > 12 && game_state < 17) || game_state > 18;
  if (want == squash) {
    waited = 0;
    return;
  }
  const PpuRegs *r = LINE_REGS(kSnesLines / 2);
  if ((r->inidisp & 0x80) || !(r->inidisp & 15) || ++waited >= 60) {
    squash = want;
    waited = 0;
  }
}

// The GBA line after those from |g| with the state |state|. (A state's
// lines follow each other, and SnesLine goes up: SnesHw_RunDisplay numbers
// the states as the lines go down the screen.)
__attribute__((noinline, minsize)) static int StateEnd(int g, int state) {
  int lo = g + 1, hi = kGbaLines;
  while (lo < hi) {
    int mid = (lo + hi) >> 1;
    if (line_state[SnesLine(mid)] == state)
      lo = mid + 1;
    else
      hi = mid;
  }
  return lo;
}

// The stream for a frame with windows: the scrolling's and their edges.
ROM_CODE static void WindowStream(const uint32_t (*stream)[4], uint32_t (*w)[kWindowStreamWords]) {
  for (int g = 0; g <= kGbaLines; g++) {
    for (int i = 0; i < 4; i++)
      w[g][i] = stream[g][i];
    w[g][kWindowStreamWords - 1] = line_windows[g < kGbaLines ? g : kGbaLines - 1];
  }
}

// Where the lines from |g| to |end|, all with one state, are no longer
// alike for the stream: in the squashed screens, which drop some of the
// SNES's lines every few, the offset from the SNES's lines changes; and in
// a frame with Mode 7, a line with another mode, whose scrolling is in its
// segment, is a new segment when it changes.
ROM_CODE static int OffsetEnd(int g, int end, bool mode7) {
  int s = SnesLine(g), t = g + 1;
  if (squash) {
    while (t < end && SnesLine(t) - t == s - g)
      t++;
    end = t;
  }
  if (frame_mode7 && !mode7) {
    // (The lines from here to the end are the SNES's from s.)
    const uint32_t *x = line_ofs[s];
    for (t = g + 1; t < end && x[4] == x[0] && x[5] == x[1] && x[6] == x[2] && x[7] == x[3];
         t++)
      x += 4;
    end = t;
  }
  return end;
}

// What the BG3 on show in the gameplay area (the SNES lines |s0| to |s1|, at
// line |s|'s scrolling) is: kBg3Clear if it's transparent, kBg3Black if it's
// solid black, or else kBg3Mixed. They're how a power bomb's blast shows in
// most rooms: the sub screen has BG3 added, the fixed color (the blast's)
// where a window hides it, and where it's transparent. The GBA can't add a
// color to the main screen where a window is. But a BG3 that's clear has the
// fixed color everywhere (the palettes do that); and a BG3 that's black is
// solid, so on top, added, in the fixed color, where the window hides it, it
// does what the SNES's does.
ROM_CODE static int Bg3Kind(const PpuRegs *r, int s, int s0, int s1) {
  int sc = r->bgsc[2], wide = sc & 1, high = sc & 2, kind = -1;
  const uint16_t *tiles = &g_vram[(r->bgnba[1] & 15) << 12];
  uint16_t last = 0xffff;
  // (Its rows by the VOFS + 1 rule.)
  int row0 = (VOFS(s, 2) + 1 + s0) >> 3, row1 = (VOFS(s, 2) + 1 + s1) >> 3;
  int col0 = HOFS(s, 2) >> 3;
  for (int row = row0; row <= row1; row++) {
    // (The row's first word; a screen to the right is 0x400 words on, and one below it is
    // that or 0x800.)
    int base = ((sc & 0xfc) << 8) + (row & 31) * 32 +
               (high && (row & 32) ? (wide ? 0x800 : 0x400) : 0);
    for (int col = col0; col <= col0 + 32; col++) {
      int i = base + (col & 31) + (wide && (col & 32) ? 0x400 : 0);
      uint16_t e = g_vram[i & 0x7fff] & 0x1fff;
      if (e == last)
        continue;
      last = e;
      const uint16_t *t = &tiles[((e & 0x3ff) * 8) & 0x7fff];
      uint16_t p0 = t[0] & 0xff, p1 = t[0] >> 8;
      if ((p0 && p0 != 0xff) || (p1 && p1 != 0xff))
        return kBg3Mixed;
      for (int y = 1; y < 8; y++) {
        if (t[y] != t[0])
          return kBg3Mixed;
      }
      int k = (p0 ? 1 : 0) | (p1 ? 2 : 0);
      int this_kind = !k ? kBg3Clear : g_cgram[(e >> 10) * 4 + k] ? kBg3Mixed : kBg3Black;
      if (this_kind == kBg3Mixed || (kind >= 0 && kind != this_kind))
        return kBg3Mixed;
      kind = this_kind;
    }
  }
  return kind < 0 ? kBg3Mixed : kind;
}

// Whether the frame is a power bomb's, and how its blast is tinted (blast_bg3).
// BG3's map is looked at every 8th frame of one.
ROM_CODE static void FindBlast(void) {
  static int kind_age;
  static uint8_t kind;
  if (text_line >= 0 && !frame_mode7 && PowerBombRegs(LINE_REGS(text_line))) {
    if (kind_age-- <= 0) {
      kind = Bg3Kind(LINE_REGS(text_line), text_line, SnesLine(hud ? kGbaHudLines : 0),
                     SnesLine(kGbaLines - 1));
      kind_age = 7;
    }
    blast_bg3 = kind;
    blast_on = true;
  } else {
    blast_on = false;
    blast_bg3 = kBg3Mixed;
    kind_age = 0;
  }
}

void Render_CaptureFrame(void) {
  SnesHw_RunDisplay(line_states, line_state, line_wh, line_ofs);
  // (The palette's line, as it was last frame: see FixedMath.)
  SnesHw_HdmaColdata(line_states, line_state, SnesLine(kGbaLines / 2));

  SetSquash();
  if (squash) {
    SetSquashLines();
    if (g_snes_hw.cgram_dirty)
      SetSquashRanks();
  }
  // The HUD: BG3 alone, and below it, other layers or another BG3 (in
  // rooms with fog, BG3 alone again, the room on the sub screen, and
  // color math, unlike the HUD's).
  const PpuRegs *top = LINE_REGS(8), *below = LINE_REGS(kHudLines + 8);
  hud = !squash && top->tm == 4 &&
        (below->tm != 4 || below->cgadsub != top->cgadsub || below->bgsc[2] != top->bgsc[2] ||
         HOFS(kHudLines + 8, 2) != HOFS(8, 2) || VOFS(kHudLines + 8, 2) != VOFS(8, 2));
  UpdateYOffset();

  // The BG3 maps the lines use (in slots), whether any line is Mode 7's,
  // and a line of text (not Mode 7), the middle one if it can be. A state
  // at a time: its lines follow each other. (The first few's ends are kept
  // for the loop below.)
  uint8_t first = LINE_REGS(SnesLine(0))->bgsc[2];
  uint8_t second = first;
  enum { kKeptEnds = 8 };
  uint8_t state_ends[kKeptEnds];
  _Static_assert(kGbaLines <= 255, "state_ends keeps GBA lines as bytes");
  int last_state = line_state[SnesLine(kGbaLines - 1)];
  frame_mode7 = false;
  text_line = -1;
  for (int g = 0, end, run = 0; g < kGbaLines; g = end, run++) {
    int s = SnesLine(g), state = line_state[s];
    const PpuRegs *r = &line_states[state];
    end = state == last_state ? kGbaLines : StateEnd(g, state);
    if (run < kKeptEnds)
      state_ends[run] = end;
    if (second == first)
      second = r->bgsc[2];
    if ((r->bgmode & 7) == 7)
      frame_mode7 = true;
    else if (g <= kGbaLines / 2 && kGbaLines / 2 < end)
      text_line = SnesLine(kGbaLines / 2);
    else if (text_line < 0)
      text_line = s;
  }
  if (first != bg3_slot_sc[0] && first != bg3_slot_sc[1]) {
    // Keep the other one where it is.
    bg3_slot_sc[second == bg3_slot_sc[0] ? 1 : 0] = first;
  }
  if (second != bg3_slot_sc[0] && second != bg3_slot_sc[1])
    bg3_slot_sc[first == bg3_slot_sc[0] ? 1 : 0] = second;

  bg3_single = first == second;
  beam_on = false;
  if (text_line >= 0 && !frame_mode7)
    ChooseSplit(LINE_REGS(text_line));
  else
    SetSplit(false);
  bg3_mid = text_line >= 0 && !frame_mode7 && !split_bg2 && !bg2_over_lo &&
            Bg3Between(LINE_REGS(text_line));
  FindBlast();
  if (text_line >= 0 && !frame_mode7 && (LINE_REGS(text_line)->cgadsub & 0x80) &&
      (LINE_REGS(text_line)->cgwsel & 2))
    subtract_weight = SubtractWeight(LINE_REGS(text_line));
  else
    subtract_color = 0;
  if ((subtract_color & 0x8000) && MainScreenBlack())
    subtract_color = 0;

  // Lines with the same registers (and the same offset from the SNES's
  // lines) have the same GBA registers, but for the scrolling, and Mode 7's
  // reference point, which moves down the map a line at a time.
  uint32_t(*stream)[4] = streams[back];
  DisplaySegment *segs = segments[back];
  int num_segs = 0;
  LineRegs l;
  bool windows = false;
  built_regs = NULL;
  num_new_runs = old_run = 0;
  for (int g = 0, state_end, run = 0; g < kGbaLines; g = state_end, run++) {
    int state = line_state[SnesLine(g)];
    const PpuRegs *r = &line_states[state];
    bool mode7 = (r->bgmode & 7) == 7;
    state_end = run < kKeptEnds ? state_ends[run] : StateEnd(g, state);
    for (int end; g < state_end; g = end) {
      int s = SnesLine(g);
      end = state_end;
      if (hud && g < kGbaHudLines && kGbaHudLines < end && y_offset)
        end = kGbaHudLines;  // below the HUD, the gameplay area is y_offset lines on
      if (squash || (frame_mode7 && !mode7))
        end = OffsetEnd(g, end, mode7);
      end = BlastLineEnd(r, s, g, end);
      BuildLine(&l, r, s, g, state_end);
      if (!num_segs || (num_segs < kMaxSegments && !SameSegment(&segs[num_segs - 1], &l.ctl))) {
        segs[num_segs] = l.ctl;
        segs[num_segs++].first_line = g;
      }
      if (mode7)
        Mode7Stream(stream + g, end - g, l.stream, (int16_t)(l.stream[0] >> 16),
                    (int16_t)(l.stream[1] >> 16));
      else if (frame_mode7)
        Mode7Stream(stream + g, end - g, l.stream, 0, 0);
      else if (squash)  // (whose runs are a few lines: OffsetEnd)
        ScrollStream(stream + g, end - g, s, g);
      else
        ScrollRuns(stream + g, end - g, s, g);
      if (l.ctl.dispcnt & (3 << 13)) {
        LineWindows(g, end, s);
        windows = true;
      }
    }
  }
  memcpy(fill_runs[back], new_runs, sizeof(new_runs));
  num_fill_runs[back] = num_new_runs;
  for (int i = 0; i < 4; i++)
    stream[kGbaLines][i] = stream[kGbaLines - 1][i];
  KeepLayersOn(segs, num_segs);
  DisplayFrame *f = &frames[back];
  f->stream = &stream[0][0];
  f->stream_words = 4;
  f->stream_dest = frame_mode7 ? 0x04000020 : 0x04000010;
  if (windows) {
    // (Not with Mode 7: see Windows.) The transformations stay 0.
    WindowStream(stream, window_streams[back]);
    f->stream = &window_streams[back][0][0];
    f->stream_words = kWindowStreamWords;
  }
  f->num_segments = num_segs;
  f->segments = segs;
  // The gameplay area's first segment, for the palette's patches.
  f->patch_segment = 0;
  while (hud && f->patch_segment < num_segs - 1 && segs[f->patch_segment].first_line < kGbaHudLines - 1)
    f->patch_segment++;
  LogLayoutChange(LINE_REGS(SnesLine(kGbaLines / 2)));
}

// The color math with the fixed color that line |m| does, which the
// palette does in its place (see ColorMath): its CGADSUB and the color, or
// 0 for none.
ROM_CODE static uint32_t FixedMath(const PpuRegs *m) {
  int region = (m->cgwsel >> 4) & 3;
  bool sub = m->cgwsel & 2;
  // A power bomb's with BG3 transparent: the fixed color's added to all the
  // layers but BG3, which isn't on the main screen.
  if (BlastTint(m) == kBg3Clear)
    return (m->cgadsub & 0x3b) | (m->coldata[0] | m->coldata[1] << 5 | m->coldata[2] << 10) << 8;
  if ((sub && !((m->cgadsub & 0x80) && (subtract_color & 0x8000))) || !(m->cgadsub & 0x3f) ||
      region == 1 || region == 3)
    return 0;
  // The sub screen's one color (see subtract_color), or the fixed color.
  uint32_t color = sub ? subtract_color & 0x7fff
                       : m->coldata[0] | m->coldata[1] << 5 | m->coldata[2] << 10;
  if (!color && !(m->cgadsub & 0x40))
    return 0;
  return m->cgadsub | color << 8;
}

ROM_CODE static int Bright(int v, int brightness) {
  return brightness == 15 ? v : brightness ? (v * (brightness + 1)) >> 4 : 0;
}

// |n| colors from |src| to |dst|, by |t|'s channels, unless they're as they
// were (|same|).
static void Recolor(volatile uint16_t *dst, const uint16_t *src, int n, const uint8_t (*t)[32],
                    bool same) {
  if (same) {
    for (int i = 0; i < n; i++)
      dst[i] = src[i];
    return;
  }
  for (int i = 0; i < n; i++) {
    uint16_t c = src[i];
    dst[i] = t[0][c & 31] | t[1][(c >> 5) & 31] << 5 | t[2][(c >> 10) & 31] << 10;
  }
}

// The colors of the blocks of 8 in |blocks| (a bit each, from color 0's), as
// UpdatePalette has them in the GBA's palettes.
ROM_CODE static void RecolorBlocks(uint32_t blocks, const uint8_t (*bd)[32],
                                   const uint8_t (*bg12)[32], const uint8_t (*o)[32],
                                   const uint8_t (*b3)[32], const uint8_t (*same)[32], bool full,
                                   bool bg3_palettes) {
  for (; blocks; blocks &= blocks - 1) {
    int c = __builtin_ctz(blocks) * 8;
    if (c >= 128) {
      const uint8_t(*t)[32] = c < 192 ? same : o;
      Recolor(GBA_OBJ_PALETTE + c - 128, g_cgram + c, 8, t, full && t == same);
      continue;
    }
    // (BG palette 0's first color is the backdrop's.)
    if (!c)
      Recolor(GBA_BG_PALETTE, g_cgram, 1, bd, full && bd == same);
    Recolor(GBA_BG_PALETTE + c + !c, g_cgram + c + !c, 8 - !c, bg12, full && bg12 == same);
    for (int p = c / 4; p < c / 4 + 2 && p < 8 && bg3_palettes; p++)
      Recolor(GBA_BG_PALETTE + 128 + p * 16, g_cgram + p * 4, 4, b3, full && b3 == same);
  }
}

// A color as Recolor makes it by |t|'s channels (not when they're as they were: see its |same|,
// and the tables in UpdatePalette).
ROM_CODE static uint16_t BgColor(const uint8_t (*t)[32], uint16_t c) {
  return t[0][c & 31] | t[1][(c >> 5) & 31] << 5 | t[2][(c >> 10) & 31] << 10;
}

// The palette, at the brightness of line |r|, with line |m|'s color math
// with the fixed color.
ROM_CODE static void UpdatePalette(const PpuRegs *r, const PpuRegs *m, bool bg3_palettes) {
  static uint32_t patches_tint;  // (the blast's color the patches have: see below)
  SnesHw *h = &g_snes_hw;
  int b = (r->inidisp & 0x80) ? 0 : (r->inidisp & 15);
  uint32_t math = FixedMath(m);
  // The blast's color, if BG3 (black) is its tint, for patching BG3's.
  uint32_t tint = BlastTint(m) == kBg3Black ? m->coldata[0] | m->coldata[1] << 5 | m->coldata[2] << 10 : 0;
  uint8_t subtracted = (subtract_color & 0x8000) && (m->cgwsel & 2) ? subtract_palettes : 0;
  // The colors to convert: those the game changed, or all, if what they're
  // converted by did, or if that's a lot of them. And BG3's, with colors
  // subtracted and for two frames more: the VBlank interrupt puts back the
  // colors of those it set (Display_Start), which can fall after this, if the
  // frame it did that for was slow to present.
  uint32_t blocks = h->cgram_changed, game_changed = blocks;
  h->cgram_changed = 0;
  h->cgram_dirty = false;
  // The VBlank interrupt blacked the backdrop (color 0) of a blank frame: convert it again.
  if (g_backdrop_zeroed) {
    g_backdrop_zeroed = false;
    blocks = game_changed = blocks | 1;
  }
  static uint8_t subtracted_frames;
  subtracted_frames = subtracted ? 3 : subtracted_frames ? subtracted_frames - 1 : 0;
  int changed = 0;
  for (uint32_t left = blocks; left; left &= left - 1)
    changed++;
  if (b != conv.brightness || bg3_palettes != conv.bg3_palettes || math != conv.fixed_math ||
      subtracted != conv.subtract_palettes || changed > 16)
    blocks = game_changed = ~0u;
  if (subtracted_frames)
    blocks |= 0xf;
  if (!blocks && tint == patches_tint)
    return;
  conv.subtract_palettes = subtracted;
  conv.brightness = b;
  conv.bg3_palettes = bg3_palettes;
  conv.fixed_math = math;
  // Each channel's value, by what it was, at the brightness, and with the
  // color math too. (Tables, as the palette often changes every frame.)
  static uint8_t plain[3][32], with[3][32];
  static int tables_b = -1;
  static uint32_t tables_math;
  if (b != tables_b || math != tables_math) {
    tables_b = b;
    tables_math = math;
    for (int c = 0; c < 3; c++) {
      int f = (math >> (8 + 5 * c)) & 31;
      for (int i = 0; i < 32; i++) {
        int v = math & 0x80 ? (i > f ? i - f : 0) : (i + f < 31 ? i + f : 31);
        plain[c][i] = Bright(i, b);
        with[c][i] = Bright(math & 0x40 ? v >> 1 : v, b);
      }
    }
  }
  // The backgrounds', the sprites', and BG3's 2bpp palettes of 4 colors in
  // GBA palettes 8-15 (Mode 7's background has all 256 colors: frames with
  // both are a compromise); with the color math on the layers it's on (the
  // sprites': in palettes 4-7). By tables on the stack, in IWRAM, for all of
  // them; for a few, copying them would cost more. Copied only when something
  // reads them (at full brightness, the plain ones' colors are as they were;
  // and with no color math, so are all).
  bool full = b == 15;
  uint8_t tables[2][3][32];
  const uint8_t(*t[2])[32] = {plain, with};
  if (blocks == ~0u) {
    if (!full)
      memcpy(tables[0], plain, sizeof(plain));
    if (math & 0x37)
      memcpy(tables[1], with, sizeof(with));
    t[0] = tables[0];
    t[1] = tables[1];
  }
  const uint8_t(*same)[32] = t[0];
  const uint8_t(*bd)[32] = t[(math & 0x20) != 0], (*bg12)[32] = t[(math & 3) != 0];
  const uint8_t(*o)[32] = t[(math & 0x10) != 0], (*b3)[32] = t[(math & 4) != 0];
  if (blocks != ~0u) {
    RecolorBlocks(blocks, bd, bg12, o, b3, same, full, bg3_palettes);
  } else {
    Recolor(GBA_BG_PALETTE, g_cgram, 1, bd, full && bd == same);
    Recolor(GBA_BG_PALETTE + 1, g_cgram + 1, 127, bg12, full && bg12 == same);
    Recolor(GBA_OBJ_PALETTE, g_cgram + 128, 64, same, full);
    Recolor(GBA_OBJ_PALETTE + 64, g_cgram + 192, 64, o, full && o == same);
    for (int p = 0; p < 8 && bg3_palettes; p++)
      Recolor(GBA_BG_PALETTE + 128 + p * 16, g_cgram + p * 4, 4, b3, full && b3 == same);
  }
  // BG3's colors c that add back the subtracted color less c (see
  // subtract_color), in the gameplay area only: the HUD has some of the
  // same palettes. (They're as they were unless BG3's colors, those
  // subtracted, the brightness or the halving changed.)
  static uint32_t patches_key;
  uint32_t key = subtract_color | b << 16 | (math >> 6 & 1) << 20 | subtracted << 21 |
                 bg3_palettes << 29;
  if ((game_changed & 0xf) == 0 && key == patches_key && tint == patches_tint)
    return;
  patches_key = key;
  patches_tint = tint;
  num_patches = 0;
  if (tint) {
    // BG3's colors all the blast's, the fixed color (see Bg3Kind).
    uint16_t color = 0;
    for (int ch = 0; ch < 15; ch += 5)
      color |= Bright(tint >> ch & 31, b) << ch;
    for (int p = 0; p < 8 && bg3_palettes; p++) {
      for (int i = 1; i < 4; i++) {
        patch_index[num_patches] = p * 16 + i;
        patch_color[num_patches] = color;
        restore_color[num_patches++] =
          full && b3 == same ? g_cgram[p * 4 + i] : BgColor(b3, g_cgram[p * 4 + i]);
      }
    }
    return;
  }
  for (int p = 0; p < 8 && bg3_palettes; p++) {
    if (!(subtracted & (1 << p)))
      continue;
    for (int i = 1; i < 4; i++) {
      uint16_t color = 0;
      for (int ch = 0; ch < 15; ch += 5) {
        int d = (subtract_color >> ch & 31) - (g_cgram[p * 4 + i] >> ch & 31);
        d = d < 0 ? 0 : (math & 0x40) ? d >> 1 : d;
        color |= Bright(d, b) << ch;
      }
      patch_index[num_patches] = p * 16 + i;
      patch_color[num_patches] = color;
      restore_color[num_patches++] =
          full && b3 == same ? g_cgram[p * 4 + i] : BgColor(b3, g_cgram[p * 4 + i]);
    }
  }
}

ROM_CODE static bool TileIsBlank(const uint16_t *t) {
  for (int i = 0; i < 16; i++) {
    if (t[i])
      return false;
  }
  return true;
}

// The last transparent tile of BG1's, or |blank| if it still is one.
ROM_CODE static int FindBlankTile(uint16_t bg12_base, int blank) {
  if (blank >= 0 && TileIsBlank(&g_vram[(bg12_base + blank * 16) & 0x7fff]))
    return blank;
  for (int i = 1023; i > 0; i--) {
    if (TileIsBlank(&g_vram[(bg12_base + i * 16) & 0x7fff]))
      return i;
  }
  return 0;
}

static bool ChunkDirty(const uint32_t *dirty, uint32_t chunk) {
  return (dirty[chunk >> 5] >> (chunk & 31)) & 1;
}

ROM_CODE static void UpdateTilesMode7(const uint32_t *dirty) {
  // Each word of the first 32 KB is a map entry (low byte) and a pixel
  // (high byte).
  volatile uint16_t *tiles = GBA_VRAM, *map = VRAM_SB(kSbMode7);
  for (uint32_t w = 0; w < 0x4000 >> (kVramChunkShift + 5); w++) {
    uint32_t bits = dirty[w];
    while (bits) {
      uint32_t c = w * 32 + __builtin_ctz(bits);
      bits &= bits - 1;
      uint32_t addr = c << kVramChunkShift;
      const uint16_t *src = &g_vram[addr];
      for (int i = 0; i < 16; i += 2) {
        uint16_t w0 = src[i], w1 = src[i + 1];
        map[(addr + i) >> 1] = (w0 & 0xff) | (w1 & 0xff) << 8;
        tiles[(addr + i) >> 1] = (w0 >> 8) | (w1 & 0xff00);
      }
    }
  }
}

// The tiles in VRAM chunk |c| (16 words), in all the GBA's tile sets that use
// them: BG1 and BG2's, BG3's, and the sprites' (two ranges).
static inline __attribute__((always_inline)) void ChunkTiles(uint32_t c, bool squashed, bool bg12,
                                                             bool bg3, uint16_t bg12_base,
                                                             uint16_t bg3_base) {
  volatile uint32_t *vram32 = (volatile uint32_t *)GBA_VRAM;
  volatile uint32_t *obj32 = (volatile uint32_t *)GBA_OBJ_VRAM;
  uint16_t addr = c << kVramChunkShift;
  const uint16_t *src = &g_vram[addr];
  uint16_t off;
  if (bg12) {
    off = (addr - bg12_base) & 0x7fff;
    if (off < 0x4000)
      (squashed ? Convert_Tiles4bppSquashed : Convert_Tiles4bpp)(vram32 + (off >> 4) * 8, src, 1);
  }
  if (bg3) {
    off = (addr - bg3_base) & 0x7fff;
    if (off < kBg3Tiles * 8) {
      volatile uint32_t *dst = vram32 + 0x8000 / 4 + (off >> 3) * 8;
      (squashed ? Convert_Tiles2bppSquashed : Convert_Tiles2bpp)(dst, src, 2);
    }
  }
  for (int t = 0; t < 2; t++) {
    off = (addr - conv.obj_base[t]) & 0x7fff;
    if (off < 0x1000) {
      int i = off >> 4;
      int tile = t * 512 + (i >> 4) * 32 + (i & 15);
      if (squashed) {
        Convert_Tile4bppAndMirrored(obj32 + tile * 8, obj32 + (tile - 2 * (i & 15) + 31) * 8, src);
      } else {
        Convert_Tiles4bpp(obj32 + tile * 8, src, 1);
        Convert_Tiles4bpp(obj32 + (tile + 16) * 8, src, 1);
      }
    }
  }
}

// (The squashed screens' are in ROM: menus'.)
__attribute__((noinline)) static void UpdateChunkTiles(uint32_t c, bool bg12, bool bg3,
                                                        uint16_t bg12_base, uint16_t bg3_base) {
  ChunkTiles(c, false, bg12, bg3, bg12_base, bg3_base);
}

ROM_CODE static void UpdateChunkTilesSquashed(uint32_t c, bool bg12, bool bg3, uint16_t bg12_base,
                                              uint16_t bg3_base) {
  ChunkTiles(c, true, bg12, bg3, bg12_base, bg3_base);
}

// Says once per address that BG2's tiles are somewhere other than BG1's.
// (BG2's tiles later than BG1's, within the range converted, are supported:
// see UpdateMaps12.)
ROM_CODE static void WarnBg2Tiles(uint16_t bg2_base, uint16_t bg12_base) {
  static uint16_t warned_bg2_base = 0xffff;
  if (warned_bg2_base != bg2_base && !(bg2_base > bg12_base && bg2_base - bg12_base < 0x4000)) {
    printf("render: BG2 tiles at 0x%x, BG1's at 0x%x (not supported yet)\n", bg2_base, bg12_base);
    warned_bg2_base = bg2_base;
  }
}

static void UpdateTiles(const PpuRegs *r, uint32_t *dirty, bool bg12, bool bg3) {
  SnesHw *h = &g_snes_hw;
  uint16_t bg12_base = (r->bgnba[0] & 0xf) << 12;
  uint16_t bg2_base = (r->bgnba[0] >> 4) << 12;
  uint16_t bg3_base = (r->bgnba[1] & 0xf) << 12;
  uint16_t obj0 = (h->obsel & 7) << 13;
  uint16_t obj1 = (obj0 + ((((h->obsel >> 3) & 3) + 1) << 12)) & 0x7fff;
  if (bg2_base != bg12_base && (r->tm & 2))
    WarnBg2Tiles(bg2_base, bg12_base);
  if (bg12_base != conv.bg12_base || bg3_base != conv.bg3_base || obj0 != conv.obj_base[0] ||
      obj1 != conv.obj_base[1] || squash != conv.squashed) {
    // Nothing shows (black, or forced blank): convert all the tiles when it does, not now and
    // again then. A menu loads its graphics after SetSquash flips, in the dark (the pause
    // screen: 14 vblanks of converting the gameplay's tiles first), and they are all marked
    // when it does show (a base that can't be anything makes this branch run again).
    const PpuRegs *dark = LINE_REGS(kSnesLines / 2);
    if ((dark->inidisp & 0x80) || !(dark->inidisp & 15)) {
      conv.bg12_base = 0xffff;
      return;
    }
    conv.squashed = squash;
    conv.bg12_base = bg12_base;
    conv.bg3_base = bg3_base;
    conv.obj_base[0] = obj0;
    conv.obj_base[1] = obj1;
    conv.blank_tile = -1;
    memset(dirty, 0xff, kVramChunks / 8);
    h->vram_dirty = true;
  }

  for (uint32_t w = 0; w < kVramChunks / 32 && h->vram_dirty; w++) {
    // (Mostly there's none: four words at a time.)
    if (!(w & 3) && !(dirty[w] | dirty[w + 1] | dirty[w + 2] | dirty[w + 3])) {
      w += 3;
      continue;
    }
    for (uint32_t bits = dirty[w]; bits; bits &= bits - 1) {
      uint32_t c = w * 32 + __builtin_ctz(bits);
      if (squash)
        UpdateChunkTilesSquashed(c, bg12, bg3, bg12_base, bg3_base);
      else
        UpdateChunkTiles(c, bg12, bg3, bg12_base, bg3_base);
    }
  }

  // A transparent tile, for the halves of BG1's map that don't show.
  int blank = conv.blank_tile;
  if (blank < 0 || ChunkDirty(dirty, ((bg12_base + blank * 16) & 0x7fff) >> kVramChunkShift))
    conv.blank_tile = FindBlankTile(bg12_base, blank);
}

// Which of a 64x64 map's two rows of screens, of tiles 32 high, a GBA map of
// 32 rows takes each tile row from (the GBA wraps at 256 lines, the SNES at
// 512): the lines on show start in the row |key| & 1, at its tile row |key|
// >> 1, and those before it are of the other, which the lines past the 256th
// of it show. (A key of 0 or 1 has all of one: nothing wraps.)
// The bits of the tile rows that come from the row of 0.
ROM_CODE static uint32_t Map64Rows(uint32_t key) {
  uint32_t from_top = ~0u << (key >> 1);  // the lines' row
  return key & 1 ? ~from_top : from_top;
}

// The key of BG |bg|'s map (see Map64Rows): from the first line on show,
// by the middle line's scrolling, and 0 or 1 if the lines on show stay in one
// row.
ROM_CODE static uint32_t Map64Key(int bg) {
  int top = SnesLine(hud ? kGbaHudLines : 0);
  int m = (VOFS(text_line, bg) + top + 1) & 511;
  if ((m & 255) + SnesLine(kGbaLines - 1) - top < 256)
    return m >> 8;
  return (m >> 3 & 31) << 1 | m >> 8;
}

// The chunks of 32 of |counts| that aren't 0, as bits.
ROM_CODE static uint32_t ChunksWithCounts(const uint8_t *counts) {
  uint32_t bits = 0;
  for (int c = 0; c < 32; c++)
    bits |= (counts[c] != 0) << c;
  return bits;
}

// (In ROM: what it calls does the work.)
ROM_CODE static void UpdateMaps12(const PpuRegs *r, const uint32_t *dirty, bool force_split) {
  bool split_changed = split_bg2 != conv.split_bg2;
  conv.split_bg2 = split_bg2;
  for (int bg = 0; bg < 2; bg++) {
    uint8_t sc = r->bgsc[bg];
    bool split = bg == split_bg2;
    // A 64x64 map's two rows of screens go into the GBA's one, a row of tiles
    // from the one the lines on show have there (Kraid's room: his body
    // wraps from the bottom row to the top): |key| (see Map64Key) says from
    // which row each of the GBA's 32 tile rows comes.
    uint32_t key = (sc & 3) == 3 ? Map64Key(bg) : 0;
    bool moved = sc != conv.bgsc[bg] || (bg && r->bgnba[0] != conv.bg2_nba);
    // BG2's tiles at another base than BG1's (the area map coming in over
    // the planet's, on the file select's way to the game): the same tiles
    // converted, BG2's entries that many later (Convert_MapChunks wraps them at
    // 1024: a base 4, 8 or 12 on is none). (Not if the base is before BG1's, whose
    // tiles go on from BG1's.)
    uint32_t tile_add = 0;
    if (bg && (r->bgnba[0] >> 4) > (r->bgnba[0] & 15))
      tile_add = (((r->bgnba[0] >> 4) - (r->bgnba[0] & 15)) << 8) & 0x3ff;
    if (bg)
      conv.bg2_nba = r->bgnba[0];
    bool changed = moved || (split && force_split) || (split_changed && (sc & 3) == 3);
    // The map goes into or out of a split, but is as converted otherwise: only
    // the chunks with priority tiles differ. (Its own GBA layer has all of a chunk's
    // entries unsplit, and those without the priority bit split; and the layer
    // in front, that a split map has the others in, is blank but for them: so
    // that, if it's this map's now, starts out blank.)
    bool flipped = split_changed && !changed;
    if (flipped && split) {
      uint32_t blank = (uint16_t)conv.blank_tile;
      for (int k = 0; k < ((sc & 3) ? 2 : 1); k++)
        DmaFill32(VRAM_SB(kSbBg1Hi + k), blank | blank << 16, 0x800);
    }
    if (moved) {
      memset(bg12_hi[bg], 0, sizeof(bg12_hi[bg]));
      memset(bg12_hi_cols[bg], 0, sizeof(bg12_hi_cols[bg]));
    }
    // The tile rows from each SNES row of screens, and those that come from
    // another than when last converted.
    uint32_t from[2] = {~0u, 0}, redo = 0;
    if ((sc & 3) == 3) {
      from[0] = Map64Rows(key);
      from[1] = ~from[0];
      redo = from[0] ^ Map64Rows(conv.bg_row[bg]);
    }
    conv.bgsc[bg] = sc;
    conv.bg_row[bg] = key;
    int screens = (sc & 3) ? 2 : 1;
    for (int row = 0; row < ((sc & 3) == 3 ? 2 : 1); row++) {
      for (int k = 0; k < screens; k++) {
        uint16_t addr = (((sc & 0xfc) << 8) + (row * 2 + k) * 0x400) & 0x7fff;
        uint8_t *counts = &bg12_hi[bg][k * 64];
        uint16_t *cols = &bg12_hi_cols[bg][k * 2];
        // The chunks to convert, 32 to a word of the dirty bitmap: those of
        // the tile rows from this row of screens that changed, or are from
        // another row than when last converted, or (flipped) have priority tiles.
        for (int w = 0; w < 2; w++) {
          uint32_t todo = SpreadBits(from[row] >> (w * 16));
          if (!changed)
            todo &= SpreadBits(redo >> (w * 16)) | dirty[(addr >> 9) + w] |
                    (flipped ? ChunksWithCounts(counts + w * 32) : 0);
          if (todo) {
            uint32_t o = w * 512;
            Convert_MapChunks(todo, VRAM_SB((bg ? kSbBg2 : kSbBg1Lo) + k) + o,
                              split ? VRAM_SB(kSbBg1Hi + k) + o : NULL, &g_vram[addr + o],
                              counts + w * 32, cols, conv.blank_tile, tile_add);
          }
        }
      }
    }
  }
}

// Whether UpdateMaps12 has nothing to do: BG1's and BG2's maps are where they
// were (but for 64x64 ones', whose rows it looks at), and unchanged.
static bool Maps12Quiet(const PpuRegs *r, const uint32_t *dirty, bool force_split) {
  uint32_t busy = force_split | (split_bg2 != conv.split_bg2);
  for (int bg = 0; bg < 2; bg++) {
    uint8_t sc = r->bgsc[bg];
    busy |= (sc != conv.bgsc[bg]) | ((sc & 3) == 3);
    busy |= bg && r->bgnba[0] != conv.bg2_nba;
    // (A screen of 1024 words is 2 of the bitmap's.)
    for (int i = 0; i < ((sc & 3) ? 4 : 2) && g_snes_hw.vram_dirty; i++)
      busy |= dirty[((sc >> 1 & 0x3e) + i) & 63];
  }
  return !busy;
}

static void UpdateMaps(const PpuRegs *r, const uint32_t *dirty, bool force_split, bool bg12) {
  if (bg12 && !Maps12Quiet(r, dirty, force_split))
    UpdateMaps12(r, dirty, force_split);
  for (int slot = 0; slot < 2; slot++) {
    uint8_t sc = bg3_slot_sc[slot];
    if (sc == 0xff)
      continue;
    bool changed = sc != conv.bg3_slot_sc[slot];
    conv.bg3_slot_sc[slot] = sc;
    int total = bg3_hi_total[slot];
    if (changed) {
      memset(&bg3_hi[slot], 0, sizeof(bg3_hi[slot]));
      total = 0;
    }
    int screens = (sc & 3) ? 2 : 1;
    for (int k = 0; k < screens; k++) {
      uint16_t addr = (((sc & 0xfc) << 8) + k * 0x400) & 0x7fff;
      uint8_t *counts = &bg3_hi[slot].chunks[k * 64];
      for (int w = 0; w < 2; w++) {
        uint32_t todo = changed ? ~0u : dirty[(addr >> 9) + w];
        // (The change of the total, as Convert_MapChunks sums it for two halves of rows.)
        uint16_t change[2] = {0, 0};
        if (todo) {
          Convert_MapChunks(todo, VRAM_SB((slot ? kSbBg3Slot1 : kSbBg3Slot0) + k) + w * 512, NULL,
                            &g_vram[addr + w * 512], counts + w * 32, change, 0, 8 << 12);
          total += (int16_t)(change[0] + change[1]);
        }
      }
    }
    bg3_hi_total[slot] = total;
  }
}

// SNES sprite sizes by OBSEL, small and large, as the GBA's shape << 2 |
// size: 0-3 are squares of 8-64 pixels, 10 is 16x32 and 11 is 32x64.
static const uint8_t kObjShapes[8][2] = {
  {0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}, {10, 11}, {10, 2},
};
// The GBA priorities of SNES sprite priorities 0-3, 2 bits each: 3, 3, 2, 1.
// With bg3_mid, 3 less: 3, 2, 1, 0.
enum { kObjPriorities = 3 | 3 << 2 | 2 << 4 | 1 << 6, kObjPrioritiesMid = 3 | 2 << 2 | 1 << 4 };

// The GBA's sprite heights and widths, by kObjShapes's codes.
static const uint8_t kObjHeight[12] = {8, 16, 32, 64, 0, 0, 0, 0, 0, 0, 32, 64};
static const uint8_t kObjWidth[12] = {8, 16, 32, 64, 0, 0, 0, 0, 0, 0, 16, 32};

// Squashed sprites' transformations, in OAM: unflipped and flipped top to
// bottom. Lines are 3/4 as many.
ROM_CODE static void SetSquashMatrices(void) {
  for (int m = 0; m < 2; m++) {
    int16_t p[4] = {256, 0, 0, m ? -341 : 341};
    for (int i = 0; i < 4; i++)
      GBA_OAM[(m * 4 + i) * 4 + 3] = p[i];
  }
}

// Sprite |e| (at |x|, of GBA shape |shape|) squashed, in OAM entry |o|; and
// its first tile. Its middle goes where the SNES's would be, squashed
// (sprites below line 224 are partly above the screen).
// - 8 lines high (mostly text), as it is: squashed, its row 3 would go. But
//   |stacked| kScaled, as higher ones (the frame around the equipment
//   screen's selection: unscaled, it came out taller than what it frames and
//   cut the item's letters); or kTopHalf or kBottomHalf, the halves of a
//   16-line letter, unsquashed, about where its middle would go (squashed,
//   with each half's middle put where it would go, they overlapped by 2
//   lines: B and S came out as D and 8).
// - Higher, scaled by a matrix. Not flipped left to right by one, though:
//   the GBA flips by a scale of -1 about the middle, which takes columns
//   width..1, not width - 1..0. UpdateTiles puts each tile flipped at 31 -
//   its column, and those, in reverse order, are the sprite flipped.
enum { kScaled = 1, kTopHalf, kBottomHalf };
ROM_CODE static int SquashSprite(volatile uint16_t *o, const uint8_t *e, int x, int shape,
                                 int stacked) {
  uint8_t attr = e[3];
  int height = kObjHeight[shape], y = e[1] >= kSnesLines ? e[1] - 256 : e[1];
  int middle = (((y + height / 2) * 3) >> 2) - kSquashTop;
  int column = e[2] & 15;
  if (stacked >= kTopHalf) {
    // Where the 16-line sprite's middle would go: its line 8, or this one's
    // 0 or 8.
    int joint = (((stacked == kTopHalf ? y + 8 : y) * 3) >> 2) - kSquashTop;
    o[0] = ((stacked == kTopHalf ? joint - 8 : joint) & 0xff) | (shape >> 2) << 14;
    o[1] = ((x - kXOffset) & 0x1ff) | (attr & 0xc0) << 6 | (shape & 3) << 14;
  } else if (height == 8 && !stacked) {
    o[0] = ((middle - 4) & 0xff) | (shape >> 2) << 14;
    o[1] = ((x - kXOffset) & 0x1ff) | (attr & 0xc0) << 6 | (shape & 3) << 14;
  } else {
    if (attr & 0x40)
      column = 32 - column - kObjWidth[shape] / 8;
    o[0] = ((middle - height / 2) & 0xff) | 1 << 8 | (shape >> 2) << 14;
    o[1] = ((x - kXOffset) & 0x1ff) | (attr >> 7) << 9 | (shape & 3) << 14;
  }
  return (attr & 1) * 512 + (e[2] >> 4) * 32 + column;
}

// The 8-line sprites with another 8-line sprite 8 lines above them at the
// same x, as bits, set in |above|, and below, in |below|: the halves of the
// 16-line letters of headings (the planet map's PLANET ZEBES), and on the
// equipment screen, the frame around the selection. In one pass, by a table
// of the sprites' places (|x| | |y| << 9, plus 1; 0 for none), not by
// comparing each with the others: the pause screens show over a hundred.
ROM_CODE static void FindStackedSprites(const uint8_t *shapes, uint32_t above[4],
                                        uint32_t below[4]) {
  // (Kept a slot short of full, or a search for a place not in it wouldn't
  // end: on the stack, which has room for 512 bytes more, not twice that.)
  enum { kSlots = 128 };
  uint32_t places[kSlots] = {0};
  uint32_t eights[4] = {0, 0, 0, 0};
  int n = 0;
  for (int i = 0; i < 128 && n < kSlots - 1; i++) {
    const uint8_t *e = &g_oam[i * 4];
    int hi = (g_oam[512 + (i >> 2)] >> ((i & 3) * 2)) & 3;
    if (e[1] >= kSnesLines || kObjHeight[shapes[hi >> 1]] != 8)
      continue;
    n++;
    eights[i >> 5] |= 1u << (i & 31);
    uint32_t place = (e[0] | (hi & 1) << 8 | e[1] << 9) + 1;
    unsigned slot = (place * 0x9e5u >> 4) & (kSlots - 1);
    while (places[slot] && places[slot] != place)
      slot = (slot + 1) & (kSlots - 1);
    places[slot] = place;
  }
  for (int i = 0; i < 128; i++) {
    if (!(eights[i >> 5] >> (i & 31) & 1))
      continue;
    const uint8_t *e = &g_oam[i * 4];
    int hi = (g_oam[512 + (i >> 2)] >> ((i & 3) * 2)) & 3;
    uint32_t place = (e[0] | (hi & 1) << 8 | e[1] << 9) + 1;
    for (int dy = -8; dy <= 8; dy += 16) {
      if (e[1] + dy < 0)
        continue;
      uint32_t near = place + (dy << 9);
      unsigned slot = (near * 0x9e5u >> 4) & (kSlots - 1);
      while (places[slot] && places[slot] != near)
        slot = (slot + 1) & (kSlots - 1);
      if (places[slot])
        (dy < 0 ? above : below)[i >> 5] |= 1u << (i & 31);
    }
  }
}

// The sprites FindStackedSprites found. Finding them takes some 150000 cycles from
// ROM when the sprites are a hundred 8-line ones, as the credits' text is, and
// those change every frame (it scrolls: that went over the vblank, which made the
// next frame convert them all again, at half speed). So a scene that changes 48
// sprites or more two frames running has them found on its first frame only
// (its text has none stacked, or the same ones as it scrolls).
static uint32_t stacked_above[4], stacked_below[4];

// UpdateOam's work for the sprites in |blocks| (4 each) when squashed, in
// ROM: it's menus' and the like.
ROM_CODE static void UpdateOamSquashed(uint32_t blocks, bool unused_hide) {
  const uint8_t *shapes = kObjShapes[g_snes_hw.obsel >> 5];
  static uint8_t busy;
  int changed = 0;
  for (uint32_t b = blocks; b; b &= b - 1)
    changed++;
  busy = changed < 12 ? 0 : busy + (busy < 2);
  if (busy < 2) {
    memset(stacked_above, 0, sizeof(stacked_above));
    memset(stacked_below, 0, sizeof(stacked_below));
    FindStackedSprites(shapes, stacked_above, stacked_below);
  }
  const uint32_t *above = stacked_above, *below = stacked_below;
  bool equipment = EquipmentScreen();
  for (int i = 0; i < 128; i++) {
    if (!(blocks >> (i >> 2) & 1))
      continue;
    const uint8_t *e = &g_oam[i * 4];
    int hi = (g_oam[512 + (i >> 2)] >> ((i & 3) * 2)) & 3;
    if (e[1] == 0xf0 && !(hi & 2) && unused_hide) {
      GBA_OAM[i * 4] = 1 << 9;  // hidden
      continue;
    }
    volatile uint16_t *o = &GBA_OAM[i * 4];
    int x = e[0] | (hi & 1) << 8, shape = shapes[hi >> 1];
    int up = above[i >> 5] >> (i & 31) & 1, down = below[i >> 5] >> (i & 31) & 1;
    // Scaled, on the equipment screen; elsewhere a letter's halves kept
    // whole, as the top or bottom of a 16-line sprite.
    int stacked = equipment ? (up | down ? kScaled : 0)
                            : up == down ? 0 : down ? kTopHalf : kBottomHalf;
    int tile = SquashSprite(o, e, x, shape, stacked);
    int priority = (e[3] >> 4) & 3;
    o[2] = tile | (bg3_mid ? 3 - priority : (kObjPriorities >> (priority * 2)) & 3) << 10 |
           ((e[3] >> 1) & 7) << 12;
  }
}

// The SNES draws sprites over each other by their places in OAM alone, and the GBA by priority
// first (see kObjPriorities): so Ridley's body (priority 3, in later places than Samus's, of 2)
// came out over her. The sprites after Samus's (|first| to |end|, from rtl.c) of a higher
// priority than hers, that overlap their box, are put to hers, and noted in |kept| (a bit each).
ROM_CODE static void PutSamusInFront(uint32_t first, uint32_t end, uint32_t drawn,
                                     uint32_t priorities, const uint8_t *shapes, uint32_t *kept) {
  const uint32_t *sprites = (const uint32_t *)g_oam;
  int x0 = 512, x1 = -512, y0 = 256, y1 = -1;
  uint32_t samus = 0;
  for (uint32_t i = first; i < end; i++) {
    uint32_t e = sprites[i], high = g_oam[512 + (i >> 2)] >> ((i & 3) * 2);
    if ((e & 0xff00) == 0xf000)
      continue;
    int x = (e & 0xff) | (high & 1) << 8, y = e >> 8 & 0xff, shape = shapes[high >> 1 & 1];
    x -= x >= 256 ? 512 : 0;
    if (y >= kSnesLines || x + kObjWidth[shape] <= 0 || x >= 256)
      continue;  // (not on the screen: the game draws her off it, as the room tour does)
    x0 = x < x0 ? x : x0;
    x1 = x + kObjWidth[shape] > x1 ? x + kObjWidth[shape] : x1;
    y0 = y < y0 ? y : y0;
    y1 = y + kObjHeight[shape] > y1 ? y + kObjHeight[shape] : y1;
    samus = priorities >> (e >> 27 & 6) & 3;
  }
  for (uint32_t i = end; i < drawn && x1 > x0; i++) {
    uint32_t e = sprites[i], high = g_oam[512 + (i >> 2)] >> ((i & 3) * 2);
    uint32_t priority = priorities >> (e >> 27 & 6) & 3;
    if ((e & 0xff00) == 0xf000 || priority >= samus)
      continue;
    int x = (e & 0xff) | (high & 1) << 8, y = e >> 8 & 0xff, shape = shapes[high >> 1 & 1];
    x -= x >= 256 ? 512 : 0;
    if (x < x1 && x + kObjWidth[shape] > x0 && y < y1 && y + kObjHeight[shape] > y0) {
      GBA_OAM[i * 4 + 2] = (GBA_OAM[i * 4 + 2] & ~0xc00) | samus << 10;
      kept[i >> 5] |= 1u << (i & 31);
    }
  }
}

// Whether any of the sprites from |end| to |drawn| has the priority 3 (mostly, none has): then
// KeepSamusInFront may have work.
ROM_CODE static bool InFrontOfSamus(uint32_t end, uint32_t drawn) {
  const uint32_t *sprites = (const uint32_t *)g_oam;
  for (uint32_t i = end; i < drawn; i++) {
    if ((sprites[i] >> 28 & 3) == 3 && (sprites[i] & 0xff00) != 0xf000)
      return true;
  }
  return false;
}

// The sprites the last call of KeepSamusInFront put to Samus's priority (a bit each).
static uint32_t samus_kept[4] EWRAM_BSS;

// Called by UpdateOam after it converted the sprites, to put the ones that came out over Samus's
// behind her (from rtl.c's note of her sprites). It is the one call in IWRAM's UpdateOam, to
// keep it small: IWRAM is short, and this is in ROM. The sprites it put to Samus's priority
// before, that UpdateOam may not have converted again, are put back first.
ROM_CODE static void KeepSamusInFront(void) {
  const uint8_t *shapes = kObjShapes[g_snes_hw.obsel >> 5];
  uint32_t priorities = bg3_mid ? kObjPrioritiesMid : kObjPriorities;
  const uint32_t *sprites = (const uint32_t *)g_oam;
  for (int w = 0; w < 4; w++) {
    for (uint32_t bits = samus_kept[w]; bits; bits &= bits - 1) {
      uint32_t i = w * 32 + __builtin_ctz(bits);
      GBA_OAM[i * 4 + 2] = (GBA_OAM[i * 4 + 2] & ~0xc00) |
                           (priorities >> (sprites[i] >> 27 & 6) & 3) << 10;
    }
    samus_kept[w] = 0;
  }
  uint32_t first = g_samus_sprites[0], end = g_samus_sprites[1];
  uint32_t drawn = oam_prev_blocks ? (oam_prev_blocks - 1) * 4 : 128;
  if (end > first && InFrontOfSamus(end, drawn))
    PutSamusInFront(first, end, drawn, priorities, shapes, samus_kept);
}

static void UpdateOam(void) {
  SnesHw *h = &g_snes_hw;
  static int last_y_off = -1;
  static bool last_squash, last_bg3_mid;
  int y_off = hud ? kHudBlankLines + y_offset : y_offset;
  // The sprites to convert: those the game changed (in blocks of 4: see
  // SnesHw), or all, if what they're converted by did.
  uint32_t blocks = h->oam_changed;
  if (y_off != last_y_off || squash != last_squash || bg3_mid != last_bg3_mid)
    blocks = ~0u;
  if (!blocks)
    return;
  h->oam_changed = 0;
  last_y_off = y_off;
  last_bg3_mid = bg3_mid;
  if (squash && !last_squash)
    SetSquashMatrices();
  last_squash = squash;
  uint32_t vblanks = g_vblank_count;
  const uint8_t *shapes = kObjShapes[h->obsel >> 5];
  // The game puts the sprites it doesn't use at line 240, small, where
  // they don't show: most of them, most of the time.
  bool unused_hide = kObjHeight[shapes[0]] <= 256 - 0xf0;
  if (squash) {
    UpdateOamSquashed(blocks, unused_hide);
    if (g_vblank_count != vblanks || REG_VCOUNT < kGbaLines)
      h->oam_changed = ~0u;
    return;
  }
  // The GBA's shape bits of small and large sprites, in attributes 0 and 1
  // (the high half of the word).
  uint32_t shape[2] = {(shapes[0] >> 2) << 14 | (uint32_t)(shapes[0] & 3) << 30,
                       (shapes[1] >> 2) << 14 | (uint32_t)(shapes[1] & 3) << 30};
  uint32_t priorities = bg3_mid ? kObjPrioritiesMid : kObjPriorities;
  const uint32_t *sprites = (const uint32_t *)g_oam;
  for (; blocks; blocks &= blocks - 1) {
    int block = __builtin_ctz(blocks);
    // The sprites' x's ninth bits and sizes, 2 bits each.
    uint32_t high = g_oam[512 + block];
    volatile uint32_t *o = (volatile uint32_t *)&GBA_OAM[block * 16];
#pragma clang loop unroll(disable)
    for (int i = 0; i < 4; i++, high >>= 2, o += 2) {
      // x, y, tile and attributes (vhoopppN), a byte each.
      uint32_t e = sprites[block * 4 + i];
      if (!(((e ^ 0xf000) & 0xff00) | (high & 2) | !unused_hide)) {
        *(volatile uint16_t *)o = 1 << 9;  // hidden
        continue;
      }
      uint32_t large = high >> 1 & 1, x = (e & 0xff) | (high & 1) << 8;
      o[0] = (((e >> 8) - y_off) & 0xff) | (((x - kXOffset) & 0x1ff) | (e >> 18 & 0x3000)) << 16 |
             shape[large];
      // The tile (the name table's bit, and the tile's row and column), the
      // priority and the palette.
      ((volatile uint16_t *)o)[2] = (e >> 15 & 0x3e0) | (e >> 16 & 15) | (e >> 13 & 0x7000) |
                                    (priorities >> (e >> 27 & 6) & 3) << 10;
    }
  }
  KeepSamusInFront();
  // The GBA takes writes to OAM in VBlank (or HBlank, with DISPCNT's bit 5)
  // only: if this ran past it, do all of them again next frame.
  if (g_vblank_count != vblanks || REG_VCOUNT < kGbaLines)
    h->oam_changed = ~0u;
}

// VRAM's changes on frames with no line showing BG1-BG3 (as a door loads
// the next room's graphics, or in Mode 7), for when lines show them: then
// their tiles and maps are converted.
static uint32_t later_dirty[kVramChunks / 32];
static uint32_t later_marks;
static bool later_any;

ROM_CODE static void KeepForLater(const uint32_t *dirty) {
  for (int i = 0; i < kVramChunks / 32; i++)
    later_dirty[i] |= dirty[i];
  later_marks += g_vram_dirty_marks;
  later_any = true;
}

ROM_CODE static void AddFromBefore(uint32_t *dirty) {
  for (int i = 0; i < kVramChunks / 32; i++)
    dirty[i] |= later_dirty[i];
  memset(later_dirty, 0, sizeof(later_dirty));
  later_any = false;
  later_marks = 0;
}

// About how many lines Render_Present will take, for presenting late in
// VBlank (main.c): some 22 with nothing to convert (the palette, OAM, a
// map's rows), and 3/8 of a line a dirty chunk of VRAM, measured in
// Ridley's fights; a map that moved is converted whole (a door's scroll
// into the next room), and tiles at another base all over again. (A
// little over is no harm: Display_Resume.)
ROM_CODE int Render_PresentLines(void) {
  int lines = 22 + ((g_vram_dirty_marks + later_marks) * 3 >> 3);
  if (text_line < 0)
    return lines;
  const PpuRegs *r = LINE_REGS(text_line);
  if (r->bgsc[0] != conv.bgsc[0] || r->bgsc[1] != conv.bgsc[1] || split_bg2 != conv.split_bg2 ||
      bg3_slot_sc[0] != conv.bg3_slot_sc[0] || bg3_slot_sc[1] != conv.bg3_slot_sc[1])
    lines += 48;
  if (((r->bgnba[0] & 0xf) << 12) != conv.bg12_base || ((r->bgnba[1] & 0xf) << 12) != conv.bg3_base)
    lines += 160;
  return lines;
}

// Shows nothing (the backdrop) until the next Display_Start, but for the
// last frame's HUD, if it had one: its segments go on, over the black. (Its
// tiles are converted again the while, the same.)
ROM_CODE static void HideUntilPresented(void) {
  DisplayFrame *last = &frames[back ^ 1];
  REG_IME = 0;
  if (g_display == last && last->patch_segment > 0) {
    DisplaySegment *s = (DisplaySegment *)&last->segments[last->patch_segment];
    uint16_t first_line = s->first_line;
    memset(s, 0, sizeof(*s));
    s->first_line = first_line;
    last->num_segments = last->patch_segment + 1;
    Display_Start();
  } else {
    g_display = 0;
    REG_DISPCNT = kDispcntMode0;
  }
  REG_IME = 1;
}

// Mother Brain's rainbow beam (see ColorMath): the spare slot's map of one
// tile, the solid one (in the slot's first screen block, the map in its
// second), and the tile's color in BG3's palette 8, whose colors 4-15 are
// free: the fixed color, as the other palettes have it.
ROM_CODE static void ShowBeam(int brightness) {
  // The slot isn't BG3's any more (UpdateMaps leaves it be), and the map
  // is as it was unless UpdateMaps converted BG3's over it since: that sets
  // the slot's conv.bg3_slot_sc.
  bg3_slot_sc[beam_slot] = 0xff;
  if (conv.bg3_slot_sc[beam_slot] != 0xff || conv.beam_slot != beam_slot) {
    conv.bg3_slot_sc[beam_slot] = 0xff;
    conv.beam_slot = beam_slot;
    int sb = beam_slot ? kSbBg3Slot1 : kSbBg3Slot0;
    volatile uint32_t *tile = (volatile uint32_t *)VRAM_SB(sb);
    volatile uint32_t *map = (volatile uint32_t *)VRAM_SB(sb + 1);
    uint32_t entry = ((sb - kSbBg3Slot0) * 64 + kBg3Tiles) | 8 << 12;
    for (int i = 0; i < 8; i++)
      tile[i] = 0x44444444;
    for (int i = 0; i < 512; i++)
      map[i] = entry | entry << 16;
  }
  uint16_t color = 0;
  for (int i = 0; i < 3; i++)
    color |= Bright(beam_color[i], brightness) << (5 * i);
  GBA_BG_PALETTE[128 + 4] = color;
}

void Render_Present(void) {
  bool mode7 = frame_mode7, text = text_line >= 0;
  // The registers of a line with a tiled background mode, if any, for
  // where their tiles and maps are.
  const PpuRegs *r = LINE_REGS(text ? text_line : SnesLine(kGbaLines / 2));
  // (The changes since last time, where the game left them: nothing marks
  // any while this runs.)
  SnesHw *h = &g_snes_hw;
  uint32_t *dirty = g_vram_dirty;
  uint32_t marks = g_vram_dirty_marks + later_marks;
  g_vram_dirty_marks = 0;

  if (mode7 != conv.mode7) {
    // Mode 7 and the other modes use the GBA's VRAM differently.
    memset(dirty, 0xff, sizeof(g_vram_dirty));
    h->vram_dirty = true;
    conv.bg12_base = conv.bg3_base = 0xffff;
    conv.bgsc[0] = conv.bgsc[1] = conv.bg3_slot_sc[0] = conv.bg3_slot_sc[1] = 0xff;
    conv.beam_slot = -1;
  }
  // Converting all of VRAM for a change of mode, or a room's worth of it,
  // takes frames: the VBlank interrupt would show the last frame's
  // registers and maps over the new tiles the while (Ceres's doors, with
  // the screen on: a Mode 7 room's picture scrambled by the next room's
  // tiles). Nothing, the backdrop (UpdatePalette sets it), until it's done.
  // (Tiles at another base too, all converted again: the area map's, after
  // the planet's, on the way from the file select into the game.)
  enum { kHeavyMarks = 512 };
  bool rebased = text && (((r->bgnba[0] & 0xf) << 12) != conv.bg12_base ||
                          ((r->bgnba[1] & 0xf) << 12) != conv.bg3_base);
  if (mode7 != conv.mode7 || marks > kHeavyMarks || rebased)
    HideUntilPresented();
  UpdatePalette(LINE_REGS(SnesLine(0)), LINE_REGS(SnesLine(kGbaLines / 2)), text);
  conv.mode7 = mode7;
  int blank_before = conv.blank_tile;
  if (mode7)
    UpdateTilesMode7(dirty);
  if (!text) {
    if (h->vram_dirty)
      KeepForLater(dirty);
  } else if (later_any) {
    AddFromBefore(dirty);
    h->vram_dirty = true;
  }
  UpdateTiles(r, dirty, text && !mode7, text);
  if (beam_on)
    ShowBeam(LINE_REGS(SnesLine(0))->inidisp & 15);
  if (text)
    UpdateMaps(r, dirty, conv.blank_tile != blank_before, !mode7);
  if (h->vram_dirty) {
    memset(dirty, 0, sizeof(g_vram_dirty));
    h->vram_dirty = false;
  }
  UpdateOam();

  // From now on, the VBlank interrupt sets up this frame's registers: from
  // now, if it's still VBlank. (Not partway down the screen.)
  DisplayFrame *f = &frames[back];
  f->num_patches = num_patches;
  for (int i = 0; i < num_patches; i++) {
    f->patch_index[i] = patch_index[i];
    f->patch_color[i] = patch_color[i];
    f->restore_color[i] = restore_color[i];
  }
  REG_IME = 0;
  g_display = &frames[back];
  if (REG_VCOUNT >= kGbaLines)
    Display_Start();
  else
    Display_Resume();
  REG_IME = 1;
  back ^= 1;
}



