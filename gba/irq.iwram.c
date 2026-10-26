// The interrupt handler, which irq_entry.S calls in ARM state.
#include "audio.h"
#include "display.h"
#include "gba_hw.h"
#include "snes_hw.h"

volatile uint32_t g_vblank_count IWRAM_BSS;
const DisplayFrame *volatile g_display IWRAM_BSS;
volatile bool g_frame_presented IWRAM_BSS;
volatile bool g_backdrop_zeroed IWRAM_BSS;  // (UpdatePalette puts it back)
static int next_segment IWRAM_BSS;

static void SetSegment(const DisplayFrame *d, const DisplaySegment *s) {
  REG_DISPCNT = s->dispcnt;
  for (int i = 0; i < 4; i++)
    REG_BGCNT(i) = s->bgcnt[i];
  REG_BLDCNT = s->bld[0];
  REG_BLDALPHA = s->bld[1];
  REG_BLDY = s->bld[2];
  REG_WININ = s->win[0];
  REG_WINOUT = s->win[1];
  if (d->stream_dest != 0x04000010) {
    volatile uint16_t *ofs = &REG_BGHOFS(0);
    for (int i = 0; i < 8; i++)
      ofs[i] = s->ofs[i];
  }
}

// The VCount interrupt at the line before the next segment's, if any.
static void ScheduleSegment(const DisplayFrame *d) {
  uint16_t stat = REG_DISPSTAT & (kDispstatVBlankIrq | kDispstatHBlankIrq);
  if (next_segment < d->num_segments)
    stat |= kDispstatVCountIrq | (d->segments[next_segment].first_line - 1) << 8;
  REG_DISPSTAT = stat;
}

static void Patch(const DisplayFrame *d, const uint16_t *colors) {
  for (int i = 0; i < d->num_patches; i++)
    GBA_BG_PALETTE[d->patch_index[i] + 128] = colors[i];
}

ROM_CODE void Display_Start(void) {
  const DisplayFrame *d = g_display;
  if (!d)
    return;
  REG_DMACNT(0) = 0;
  SetSegment(d, &d->segments[0]);
  Patch(d, d->patch_segment == 0 ? d->patch_color : d->restore_color);
  volatile uint32_t *dest = (volatile uint32_t *)d->stream_dest;
  int words = d->stream_words;
  for (int i = 0; i < words; i++)
    dest[i] = d->stream[i];
  // The windows are all the lines high: the stream has their widths.
  REG_WIN0V = REG_WIN1V = kGbaLines;
  REG_DMASAD(0) = (uintptr_t)(d->stream + words);
  REG_DMADAD(0) = d->stream_dest;
  REG_DMACNT(0) = words | kDma32 | kDmaDestReload | kDmaRepeat | kDmaAtHBlank | kDmaEnable;
  next_segment = 1;
  ScheduleSegment(d);
}

// Starts g_display partway down the picture, from the line after this one:
// for a present that ran on past VBlank (see main.c's WaitForVBlank). The
// lines before showed the last frame's registers, over the new tiles where
// they changed.
//
// The DMA copies the stream's next line at this line's HBlank, so this has
// to be set up before it: begun in HBlank, or running past the line's end,
// it would copy that line's registers a line late, and every line from here
// on would show its neighbor's row of tiles (the rows the squashed menus
// leave out, as black dashes; the planet map's, with the sprites of every
// area's label to convert). So it waits out an HBlank, and starts again if
// the line changed under it: a few times at most, as this runs with the
// interrupts off (it fits in a line; the second time, it starts at one's
// beginning).
ROM_CODE void Display_Resume(void) {
  const DisplayFrame *d = g_display;
  if (!d)
    return;
  unsigned line;
  int tries = 3;
  do {
    while (REG_DISPSTAT & 2) {}  // HBlank
    line = REG_VCOUNT;
    if (line >= kGbaLines) {
      Display_Start();
      return;
    }
    REG_DMACNT(0) = 0;
    // The segment the next line is in: its registers now, and the stream
    // from that line, at this line's HBlank.
    int i = 0;
    while (i + 1 < d->num_segments && d->segments[i + 1].first_line <= line + 1)
      i++;
    SetSegment(d, &d->segments[i]);
    Patch(d, i >= d->patch_segment ? d->patch_color : d->restore_color);
    int words = d->stream_words;
    REG_WIN0V = REG_WIN1V = kGbaLines;
    REG_DMASAD(0) = (uintptr_t)(d->stream + words * (line + 1));
    REG_DMADAD(0) = d->stream_dest;
    REG_DMACNT(0) = words | kDma32 | kDmaDestReload | kDmaRepeat | kDmaAtHBlank | kDmaEnable;
    next_segment = i + 1;
  } while (--tries && (REG_VCOUNT != line || (REG_DISPSTAT & 2)));
  ScheduleSegment(d);
}

// When in the frame the sound's work runs: once Render_Present is done
// with VBlank (Audio_Kick), or if it's busy, this long after VBlank begins,
// in 64s of cycles (60 lines).
enum { kAudioDelay = 60 * 1232 / 64 };
volatile uint8_t g_audio_background_running IWRAM_BSS;
static volatile bool audio_kicked IWRAM_BSS;

static void StartAudioTimer(int delay) {
  REG_TMCNT_H(1) = 0;
  REG_TMCNT_L(1) = 0x10000 - delay;
  REG_TMCNT_H(1) = 0xc1;  // enabled, with its interrupt, counting 64 cycles
}

void Audio_Kick(void) {
  REG_IME = 0;
  if (!audio_kicked) {
    audio_kicked = true;
    StartAudioTimer(1);
  }
  REG_IME = 1;
}

static void VBlank(void) {
  g_vblank_count++;
  Audio_VBlank();
  audio_kicked = false;
  StartAudioTimer(kAudioDelay);
  // A frame of the game taking longer, with the screen turned off (as it
  // loads a room's graphics, NMI off): black on the GBA too, not the last
  // frame with its graphics going. (Not the GBA's forced blank, which
  // shows white: no layers, and the backdrop black.)
  if (!g_frame_presented && (g_snes_hw.r.inidisp & 0x80)) {
    g_display = 0;
    REG_DISPCNT = kDispcntMode0;
    GBA_BG_PALETTE[0] = 0;
    g_backdrop_zeroed = true;
  }
  g_frame_presented = false;
  // Every frame, as a frame of the game can take more than one of the
  // GBA's.
  Display_Start();
}

static void VCount(void) {
  const DisplayFrame *d = g_display;
  if (!d)
    return;
  // This segment, and the next if it starts a line later. (One that
  // starts later still gets an interrupt of its own; one that's late
  // starts as soon as can be.)
  while (next_segment < d->num_segments) {
    unsigned line = d->segments[next_segment].first_line - 1, now = REG_VCOUNT;
    if (line > now + 1)
      break;
    if (line >= now) {
      while (REG_VCOUNT < line) {}
      while (!(REG_DISPSTAT & 2)) {}  // HBlank
    }
    if (next_segment == d->patch_segment)
      Patch(d, d->patch_color);
    SetSegment(d, &d->segments[next_segment++]);
  }
  ScheduleSegment(d);
}

// Whether irq_entry.S is to run Audio_Background: once a frame, unless
// it's still running, or the game's busy with the sound driver.
bool IrqHandler(void) {
  uint16_t flags = REG_IF & REG_IE;
  if (flags & kIrqVBlank)
    VBlank();
  if (flags & kIrqVCount)
    VCount();
  bool audio = false;
  if (flags & kIrqTimer1) {
    REG_TMCNT_H(1) = 0;
    audio_kicked = true;
    audio = !g_audio_background_running && !g_audio_lock;
    g_audio_background_running |= audio;
  }
  REG_IFBIOS |= flags;
  REG_IF = flags;
  return audio;
}
