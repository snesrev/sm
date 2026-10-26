// See snes_hw.h. Register behavior follows src/snes/ppu.c, snes.c and
// dma.c, the emulator the PC build checks the game against.
//
// ARM code in IWRAM, as the game's DMA and the SNES's display of each frame
// run through here. (So no division: see cmake/compiler-rt.cmake.)
#include "snes_hw.h"

#include <stddef.h>
#include <string.h>

#include "gba_hw.h"
#include "input.h"
#include "sm_rtl.h"

// (Aligned: SameRegs compares its registers' words.)
SnesHw g_snes_hw IWRAM_BSS __attribute__((aligned(4)));
uint16_t g_cgram[256] __attribute__((aligned(4)));
uint8_t g_oam[544] __attribute__((aligned(4)));
uint8_t g_oam_used;
// The blocks of sprites the last upload of all of OAM compared, with 1
// added, or 0 if g_oam might differ from the game's anywhere (see FastDma).
uint8_t oam_prev_blocks;
uint16_t g_vram[kVramWords] __attribute__((aligned(4)));
uint32_t g_vram_dirty[kVramChunks / 32];
// The game wrote HDMAEN: HDMA's decoded channels are out of date.
static bool hdma_dirty IWRAM_BSS;
// Set when a line found hdma_dirty and decoded the channels again, partway down the screen.
static bool hdma_redecoded IWRAM_BSS;

static uint32_t wram_port_addr;

ROM_CODE ROM_CODE void SnesHw_Reset(void) {
  memset(&g_snes_hw, 0, sizeof(g_snes_hw));
  g_snes_hw.vram_inc = 1;
  for (int i = 0; i < 8; i++) {
    SnesDmaChannel *c = &g_snes_hw.dma[i];
    c->dmap = 0xff;
    c->bbad = 0xff;
    c->a1t = 0xffff;
    c->a1b = 0xff;
    c->das = 0xffff;
    c->dasb = 0xff;
    c->a2a = 0xffff;
    c->ntrl = 0xff;
  }
  memset(g_vram_dirty, 0xff, sizeof(g_vram_dirty));
  g_snes_hw.vram_dirty = g_snes_hw.cgram_dirty = true;
  g_snes_hw.cgram_changed = g_snes_hw.oam_changed = ~0u;
  oam_prev_blocks = 0;
}

// The chunks that a whole chunk of ROM was last uploaded to and that nothing
// has written since (so they're still those bytes: ROM doesn't change), by
// chunk % 32: where in the cartridge, and in bits 25-30 which chunk (/ 32), or
// 0 for none. The game re-uploads Samus's and the enemies' tiles from ROM
// every frame; these needn't be compared with it.
static uint32_t vram_from_rom[32] IWRAM_BSS;

static inline void MarkVramDirty(uint16_t addr) {
  uint32_t chunk = (addr & 0x7fff) >> kVramChunkShift;
  g_vram_dirty[chunk >> 5] |= 1u << (chunk & 31);
  g_snes_hw.vram_dirty = true;
  vram_from_rom[chunk & 31] = 0;
}

// And counts the chunk (g_vram_dirty_marks) if it wasn't dirty: for the
// paths that go a word at a time. (A function: they're the rare ones.)
uint32_t g_vram_dirty_marks IWRAM_BSS;
__attribute__((noinline)) static void MarkVramDirtyCounted(uint16_t addr) {
  uint32_t chunk = (addr & 0x7fff) >> kVramChunkShift, bit = 1u << (chunk & 31);
  uint32_t *w = &g_vram_dirty[chunk >> 5];
  g_snes_hw.vram_dirty = true;
  vram_from_rom[chunk & 31] = 0;
  if (!(*w & bit)) {
    *w |= bit;
    g_vram_dirty_marks++;
  }
}

ROM_CODE void SnesHw_WriteVram(uint16_t addr, const uint16_t *src, int words) {
  for (int i = 0; i < words; i++) {
    g_vram[(addr + i) & 0x7fff] = src[i];
    MarkVramDirtyCounted(addr + i);
  }
}

ROM_CODE uint8_t SnesHw_ReadA(uint32_t addr) {
  uint8_t bank = addr >> 16;
  uint16_t a = addr;
  if (bank == 0x7e || bank == 0x7f)
    return g_ram[((bank & 1) << 16) | a];
  if (a < 0x2000 && ((bank & 0x7f) < 0x40))
    return g_ram[a];
  return *RomFixedPtr(addr);
}

ROM_CODE static void WriteA(uint32_t addr, uint8_t val) {
  uint8_t bank = addr >> 16;
  uint16_t a = addr;
  if (bank == 0x7e || bank == 0x7f)
    g_ram[((bank & 1) << 16) | a] = val;
  else if (a < 0x2000 && ((bank & 0x7f) < 0x40))
    g_ram[a] = val;
}

__attribute__((noinline)) static uint16_t VramRemap(void) {
  uint16_t adr = g_snes_hw.vram_ptr;
  switch ((g_snes_hw.vmain >> 2) & 3) {
  case 0: return adr;
  case 1: return (adr & 0xff00) | ((adr & 0xe0) >> 5) | ((adr & 0x1f) << 3);
  case 2: return (adr & 0xfe00) | ((adr & 0x1c0) >> 6) | ((adr & 0x3f) << 3);
  default: return (adr & 0xfc00) | ((adr & 0x380) >> 7) | ((adr & 0x7f) << 3);
  }
}

_Static_assert(offsetof(PpuRegs, bgnba[1]) - offsetof(PpuRegs, bgmode) == 7 &&
                   offsetof(PpuRegs, cgadsub) - offsetof(PpuRegs, wsel) == 14,
               "WriteBBus: PpuRegs's bytes aren't in the registers' order");

// The registers that aren't WriteBBus's hot ones. Of those, the ones that
// are fields of PpuRegs and change the picture as it is (INIDISP, M7SEL, the
// Mode 7 offsets and center, COLDATA, SETINI) set regs_dirty. The others set
// their own flag: OBSEL and the OAM port oam_changed, the BG scroll registers
// scroll_dirty, CGRAM's port cgram_dirty, and VRAM's port its chunk's.
__attribute__((noinline)) static void WriteBBusRest(uint8_t adr, uint8_t val) {
  SnesHw *h = &g_snes_hw;
  PpuRegs *r = &h->r;
  // (The registers from COLDATA up are a few, far apart: a table of jumps for them would be
  // as big as all the code for them.)
  if (adr >= 0x32) {
    if (adr == 0x32) {
      if (val & 0x20) r->coldata[0] = val & 0x1f;
      if (val & 0x40) r->coldata[1] = val & 0x1f;
      if (val & 0x80) r->coldata[2] = val & 0x1f;
      h->regs_dirty = true;
    } else if (adr == 0x33) {
      r->setini = val;
      h->regs_dirty = true;
    } else if (adr - 0x40u < 4) {
      RtlApuWrite(0x2100 + adr, val);
    } else if (adr == 0x80) {
      g_ram[wram_port_addr] = val;
      wram_port_addr = (wram_port_addr + 1) & 0x1ffff;
    } else if (adr == 0x81) {
      wram_port_addr = (wram_port_addr & 0x1ff00) | val;
    } else if (adr == 0x82) {
      wram_port_addr = (wram_port_addr & 0x100ff) | (val << 8);
    } else if (adr == 0x83) {
      wram_port_addr = (wram_port_addr & 0x0ffff) | ((val & 1) << 16);
    }
    return;
  }
  switch (adr) {
  case 0x00: r->inidisp = val; h->regs_dirty = true; break;
  case 0x01:
    if (val != h->obsel)
      h->oam_changed = ~0u;  // (the sprites' sizes)
    h->obsel = val;
    break;
  case 0x02:
    h->oam_addr_reload = (h->oam_addr_reload & 0x200) | (val << 1);
    h->oam_addr = h->oam_addr_reload;
    break;
  case 0x03:
    h->oam_addr_reload = (h->oam_addr_reload & 0x1fe) | ((val & 1) << 9);
    h->oam_addr = h->oam_addr_reload;
    break;
  case 0x04: {
    uint16_t a = h->oam_addr;
    if (a < 0x200) {
      if (!(a & 1)) {
        h->oam_latch = val;
      } else {
        g_oam[a - 1] = h->oam_latch;
        g_oam[a] = val;
      }
    } else {
      g_oam[0x200 + (a & 0x1f)] = val;
    }
    h->oam_changed = ~0u;
    oam_prev_blocks = 0;
    h->oam_addr = (a + 1) & 0x3ff;
    break;
  }
  case 0x0d:
    r->m7hofs = ((val << 8) | h->m7_latch) & 0x1fff;
    h->m7_latch = val;
    h->regs_dirty = true;
    // fallthrough
  case 0x0f: case 0x11: case 0x13:
    r->hofs[(adr - 0xd) >> 1] = ((val << 8) | (h->bgofs_latch & 0xf8) | (h->bghofs_latch & 7)) & 0x3ff;
    h->bgofs_latch = val;
    h->bghofs_latch = val;
    h->scroll_dirty = true;
    h->scroll_written |= 1 << ((adr - 0xd) >> 1);
    break;
  case 0x0e:
    r->m7vofs = ((val << 8) | h->m7_latch) & 0x1fff;
    h->m7_latch = val;
    h->regs_dirty = true;
    // fallthrough
  case 0x10: case 0x12: case 0x14:
    r->vofs[(adr - 0xe) >> 1] = ((val << 8) | h->bgofs_latch) & 0x3ff;
    h->bgofs_latch = val;
    h->scroll_dirty = true;
    h->scroll_written |= 0x10 << ((adr - 0xe) >> 1);
    break;
  case 0x15:
    h->vram_inc = (val & 3) == 0 ? 1 : (val & 3) == 1 ? 32 : 128;
    h->vmain = val;
    break;
  case 0x16:
    h->vram_ptr = (h->vram_ptr & 0xff00) | val;
    h->vram_read_buf = g_vram[VramRemap() & 0x7fff];
    break;
  case 0x17:
    h->vram_ptr = (h->vram_ptr & 0x00ff) | (val << 8);
    h->vram_read_buf = g_vram[VramRemap() & 0x7fff];
    break;
  case 0x18: {
    uint16_t a = VramRemap() & 0x7fff;

    g_vram[a] = (g_vram[a] & 0xff00) | val;
    MarkVramDirtyCounted(a);
    if (!(h->vmain & 0x80))
      h->vram_ptr += h->vram_inc;
    break;
  }
  case 0x19: {
    uint16_t a = VramRemap() & 0x7fff;

    g_vram[a] = (g_vram[a] & 0x00ff) | (val << 8);
    MarkVramDirtyCounted(a);
    if (h->vmain & 0x80)
      h->vram_ptr += h->vram_inc;
    break;
  }
  case 0x1a: r->m7sel = val; h->regs_dirty = true; break;
  case 0x1f: case 0x20:
    // 13-bit signed.
    r->m7[adr - 0x1b] = (int16_t)(((val << 8) | h->m7_latch) << 3) >> 3;
    h->m7_latch = val;
    h->regs_dirty = true;
    break;
  case 0x21:
    h->cgram_ptr = val;
    h->cgram_high = false;
    break;
  case 0x22:
    if (!h->cgram_high) {
      h->cgram_latch = val;
    } else {
      h->cgram_changed = ~0u;
      g_cgram[h->cgram_ptr++] = ((val & 0x7f) << 8) | h->cgram_latch;
      h->cgram_dirty = true;
    }
    h->cgram_high = !h->cgram_high;
    break;
  }
}

// (A leaf, with no stack frame, for the hot registers: it's inlined into the
// callers that are ARM.)
static inline void WriteBBus(uint8_t adr, uint8_t val) {
  SnesHw *h = &g_snes_hw;
  PpuRegs *r = &h->r;
  // The registers that are a byte of PpuRegs as they are, and in its order
  // (BGMODE-BG34NBA, then W12SEL-CGADSUB), as the game writes them every
  // frame: no need to go through the switch. (Indexed from the start of the
  // struct: each run is part of several arrays and fields, not one array.)
  if (adr - 5u < 8 || adr - 0x23u < 15) {
    ((uint8_t *)r)[adr < 0x20 ? offsetof(PpuRegs, bgmode) + (adr - 5)
                              : offsetof(PpuRegs, wsel) + (adr - 0x23)] = val;
    h->regs_dirty = true;
  } else if (adr - 0x1bu < 4) {
    // The Mode 7 matrix's: the game multiplies with M7A and M7B a few dozen
    // times a frame in some fights.
    r->m7[adr - 0x1b] = (val << 8) | h->m7_latch;
    h->m7_latch = val;
    h->regs_dirty = true;
  } else {
    WriteBBusRest(adr, val);
  }
}

// What MPYL, MPYM and MPYH read, a byte each: M7A times M7B's high byte.
static inline int32_t Mode7Product(void) {
  return g_snes_hw.r.m7[0] * (int8_t)(g_snes_hw.r.m7[1] >> 8);
}

ROM_CODE static uint8_t ReadBBus(uint8_t adr) {
  SnesHw *h = &g_snes_hw;
  switch (adr) {
  case 0x34: case 0x35: case 0x36:
    return Mode7Product() >> (8 * (adr - 0x34));
  case 0x39: {
    uint16_t val = h->vram_read_buf;
    if (!(h->vmain & 0x80)) {
      h->vram_read_buf = g_vram[VramRemap() & 0x7fff];
      h->vram_ptr += h->vram_inc;
    }
    return val;
  }
  case 0x3a: {
    uint16_t val = h->vram_read_buf;
    if (h->vmain & 0x80) {
      h->vram_read_buf = g_vram[VramRemap() & 0x7fff];
      h->vram_ptr += h->vram_inc;
    }
    return val >> 8;
  }
  case 0x80: {
    uint8_t v = g_ram[wram_port_addr];
    wram_port_addr = (wram_port_addr + 1) & 0x1ffff;
    return v;
  }
  default:
    return 0;
  }
}

static const uint8_t kBbadOffsets[8][4] = {
  {0, 0, 0, 0}, {0, 1, 0, 1}, {0, 0, 0, 0}, {0, 0, 1, 1},
  {0, 1, 2, 3}, {0, 1, 0, 1}, {0, 0, 0, 0}, {0, 0, 1, 1},
};
static const uint8_t kTransferLength[8] = {1, 2, 2, 4, 4, 4, 2, 4};

// Returns a pointer to |count| bytes at the A-bus address, if they're all
// in the same block of RAM or ROM.
static const uint8_t *ABusSpan(uint8_t bank, uint16_t a, uint32_t count) {
  if (a + count > 0x10000)
    return NULL;
  if (bank == 0x7e || bank == 0x7f)
    return &g_ram[((bank & 1) << 16) | a];
  if ((bank & 0x7f) < 0x40 && a + count <= 0x2000)
    return &g_ram[a];
  if (a >= 0x8000)
    return RomFixedPtr((bank << 16) | a);
  return NULL;
}

// Copies |bytes| (even) with the GBA's DMA when both are halfword-aligned:
// the cartridge then reads sequentially, which is several times faster
// than a load at a time.
static void CopyBytes(void *dst, const uint8_t *src, uint32_t bytes) {
  if (!(((uintptr_t)dst | (uintptr_t)src) & 1)) {
    REG_DMASAD(3) = (uintptr_t)src;
    REG_DMADAD(3) = (uintptr_t)dst;
    REG_DMACNT(3) = (bytes >> 1) | kDma16 | kDmaEnable;
  } else {
    memcpy(dst, src, bytes);
  }
}

// Copies |blocks| blocks of 16 bytes from |src| to |dst|, both aligned to
// words, but those that are as they were: the game uploads all of CGRAM and
// OAM every frame, mostly the same, and render.c converts only what's new.
// Returns the blocks that changed, as bits from |first| on. (Reading both
// takes about what DMA's reading and writing does.) In assembly: a block is
// a load of four words from each side, a compare and a conditional store,
// with no branch for those that are the same (the compiler loaded the
// source's in two pieces, and branched on each word).
__attribute__((naked, noinline)) static uint32_t CopyChanged(uint32_t *dst, const uint32_t *src,
                                                             uint32_t blocks, uint32_t first) {
  __asm__(
      "cmp r2, #0\n"
      "moveq r0, #0\n"
      "bxeq lr\n"
      "push {r4-r11}\n"
      "mov r12, #1\n"
      "mov r3, r12, lsl r3\n"  // the bit of the block
      "mov r12, #0\n"          // the changed
      "1:\n"
      "ldmia r1!, {r4-r7}\n"
      "ldmia r0, {r8-r11}\n"
      "teq r4, r8\n"
      "teqeq r5, r9\n"
      "teqeq r6, r10\n"
      "teqeq r7, r11\n"
      "stmne r0, {r4-r7}\n"
      "orrne r12, r12, r3\n"
      "add r0, r0, #16\n"
      "mov r3, r3, lsl #1\n"
      "subs r2, r2, #1\n"
      "bne 1b\n"
      "mov r0, r12\n"
      "pop {r4-r11}\n"
      "bx lr\n");
}

// Whether the |n| words at |src| (bytes) are those at |a|, a pair at a
// time; false (not known) when they aren't 4-byte aligned or n is odd.
static bool SameWords(const uint16_t *a, const uint8_t *src, uint32_t n) {
  if ((((uintptr_t)a | (uintptr_t)src) & 3) | (n & 1))
    return false;
  const uint32_t *a32 = (const uint32_t *)a, *b32 = (const uint32_t *)src;
  for (n >>= 1; n; n--) {
    if (*a32++ != *b32++)
      return false;
  }
  return true;
}

// The rest of FastDma's palettes and OAM: odd counts or places, a byte at
// a time through the latches, as seldom happens. (In ROM, for room in
// IWRAM.)
ROM_CODE static void PaletteOrOamBytes(uint8_t bbad, const uint8_t *src, uint32_t count) {
  SnesHw *h = &g_snes_hw;
  if (bbad == 0x22) {
    for (uint32_t i = 0; i < count; i++) {
      if (!h->cgram_high)
        h->cgram_latch = src[i];
      else
        g_cgram[h->cgram_ptr++] = ((src[i] & 0x7f) << 8) | h->cgram_latch;
      h->cgram_high = !h->cgram_high;
    }
    h->cgram_dirty = true;
    h->cgram_changed = ~0u;
    return;
  }
  uint16_t a = h->oam_addr;
  for (uint32_t i = 0; i < count; i++, a = (a + 1) & 0x3ff) {
    if (a >= 0x200)
      g_oam[0x200 + (a & 0x1f)] = src[i];
    else if (!(a & 1))
      h->oam_latch = src[i];
    else
      g_oam[a - 1] = h->oam_latch, g_oam[a] = src[i];
  }
  h->oam_addr = a;
  h->oam_changed = ~0u;
  oam_prev_blocks = 0;
}

// DMA from VRAM (VMDATALREAD and VMDATAHREAD) to RAM, as the X-ray scope's
// setup reads the room's tilemaps, 4 KB in two frames, and the message boxes
// back up BG3's map: a byte at a time, in DmaBytes, that took most of three
// frames. Whole words, with the data
// port's read ahead: the first is the word it holds, then VRAM's from the
// address on. Only as the game does it: the address going up by a word after
// the high byte, with no remapping. Returns false for any other.
ROM_CODE static bool VramReadDma(SnesDmaChannel *c, uint32_t count) {
  SnesHw *h = &g_snes_hw;
  uint16_t p = h->vram_ptr;
  if ((c->dmap & 0x9f) != 0x81 || c->bbad != 0x39 || (c->a1b != 0x7e && c->a1b != 0x7f) ||
      (h->vmain & 0x8c) != 0x80 || h->vram_inc != 1 || (count & 1) ||
      c->a1t + count > 0x10000 || 0x8000 - (p & 0x7fff) < count / 2)
    return false;
  uint8_t *dst = &g_ram[(c->a1b & 1) << 16 | c->a1t];
  uint16_t buf = h->vram_read_buf;
  const uint16_t *src = &g_vram[p & 0x7fff];
  if (!(c->a1t & 1)) {
    *(uint16_t *)dst = buf;
    CopyBytes(dst + 2, (const uint8_t *)src, count - 2);
    buf = src[count / 2 - 1];
  } else {
    for (uint32_t i = 0; i < count; i += 2) {
      dst[i] = buf;
      dst[i + 1] = buf >> 8;
      buf = *src++;
    }
  }
  h->vram_read_buf = buf;
  h->vram_ptr = p + count / 2;
  c->a1t += count;
  return true;
}

// DMA a byte at a time, through the B bus's registers: what FastDma
// doesn't do, as seldom happens. (In ROM, for room in IWRAM.)
ROM_CODE static void DmaBytes(SnesDmaChannel *c, uint32_t count) {
  if ((c->dmap & 0x80) && VramReadDma(c, count))
    return;
  uint8_t mode = c->dmap & 7;
  int step = (c->dmap & 0x08) ? 0 : (c->dmap & 0x10) ? -1 : 1;
  for (uint32_t j = 0; j < count; j++) {
    uint8_t b = c->bbad + kBbadOffsets[mode][j & 3];
    uint32_t a = (c->a1b << 16) | c->a1t;
    if (c->dmap & 0x80)
      WriteA(a, ReadBBus(b));
    else
      WriteBBus(b, SnesHw_ReadA(a));
    c->a1t += step;
  }
}

// Writes |n| words of VRAM from |p| (and not past its end) from |src|. Only the
// chunks that change are dirty, and copied: the game uploads Samus's and the
// enemies' tiles every frame, mostly the same as before. Those from ROM that
// are known to be there already (vram_from_rom) aren't even compared.
static void UploadVram(uint32_t p, const uint8_t *src, uint32_t n) {
  bool rom = (uintptr_t)src >> 25 == 4;
  uint16_t *dst = &g_vram[p & 0x7fff];
  // The words just behind |dst| and |src| that are of chunks that changed, to
  // copy in one go.
  uint32_t copy_words = 0, marked = 0;
  for (uint32_t q = p, end = p + n; q < end;) {
    // The part of q's chunk that's uploaded: usually all 16 words.
    uint32_t words = 16 - (q & 15);
    words = words < end - q ? words : end - q;
    uint32_t *known = NULL, key = 0;
    if (rom && words == 16) {
      known = &vram_from_rom[(q >> 4) & 31];
      key = ((uintptr_t)src & 0x1ffffff) | ((q & 0x7fff) >> 9) << 25;
    }
    if ((known && *known == key) || SameWords(dst, src, words)) {
      if (copy_words)
        CopyBytes(dst - copy_words, src - copy_words * 2, copy_words * 2);
      copy_words = 0;
    } else {
      MarkVramDirty(q);
      marked++;
      copy_words += words;
    }
    if (known)
      *known = key;
    dst += words;
    src += words * 2;
    q += words;
  }
  if (copy_words)
    CopyBytes(dst - copy_words, src - copy_words * 2, copy_words * 2);
  g_vram_dirty_marks += marked;
}

// FastDma's other uploads to VRAM, as seldom happens (in ROM, for room in
// IWRAM): words at a stride (a column of a map), and bytes to one half of each
// word: Mode 7's maps and tiles, say. Return the new address.
ROM_CODE static uint16_t VramWordsStrided(uint16_t p, uint16_t inc, const uint8_t *src,
                                          uint32_t count) {
  for (uint32_t i = 0; i < count; i += 2, p += inc) {
    g_vram[p & 0x7fff] = src[i] | (src[i + 1] << 8);
    MarkVramDirtyCounted(p);
  }
  return p;
}

ROM_CODE static uint16_t VramBytes(uint16_t p, uint16_t inc, const uint8_t *src, uint32_t count,
                                   bool high) {
  uint16_t first = p;
  for (uint32_t i = 0; i < count; i++, p += inc) {
    uint16_t *w = &g_vram[p & 0x7fff];
    *w = high ? (*w & 0x00ff) | (src[i] << 8) : (*w & 0xff00) | src[i];
    if (!(p & 15) || inc != 1)
      MarkVramDirtyCounted(p);
  }
  MarkVramDirtyCounted(first);
  MarkVramDirtyCounted(p - 1);
  return p;
}

// The rarer ways of FastDma, out of IWRAM (one function, for one thunk): |what| is kCgramBytes
// for a palette not in whole 16-color blocks of aligned words, kOamBytes for OAM in other than
// whole uploads, kVramWrap for words to VRAM that wrap around its end or at a stride, or else
// the palette or OAM a byte at a time, and Mode 7's bytes to one half of each word of VRAM
// (maps and tiles, say). False for anything else.
enum { kOtherDma, kCgramBytes, kOamBytes, kVramWrap };

ROM_CODE static bool FastDmaRare(SnesDmaChannel *c, int what, const uint8_t *src,
                                 uint32_t count) {
  SnesHw *h = &g_snes_hw;
  uint8_t mode = c->dmap & 7;
  if (what == kCgramBytes) {
    CopyBytes(&g_cgram[h->cgram_ptr], src, count);
    h->cgram_changed = ~0u;
  } else if (what == kOamBytes) {
    oam_prev_blocks = 0;
    uint32_t low = h->oam_addr < 0x200 ? 0x200 - h->oam_addr : 0;
    if (low > count)
      low = count;
    CopyBytes(&g_oam[h->oam_addr], src, low);
    CopyBytes(&g_oam[h->oam_addr + low], src + low, count - low);
    h->oam_changed = ~0u;
  } else if (what == kVramWrap) {
    uint16_t p = h->vram_ptr, inc = h->vram_inc;
    if (inc != 1) {
      h->vram_ptr = VramWordsStrided(p, inc, src, count);
      return true;
    }
    // In up to two pieces, as VRAM wraps around.
    uint32_t words = count / 2;
    while (words) {
      uint32_t n = 0x8000 - (p & 0x7fff);
      n = n < words ? n : words;
      UploadVram(p, src, n);
      src += n * 2;
      p += n;
      words -= n;
    }
    h->vram_ptr = p;
  } else if ((c->bbad == 0x22 || c->bbad == 0x04) && (mode == 0 || mode == 2)) {
    PaletteOrOamBytes(c->bbad, src, count);
  } else if ((c->bbad == 0x18 || c->bbad == 0x19) && mode == 0 && !(h->vmain & 0x0c) &&
             !!(h->vmain & 0x80) == (c->bbad == 0x19)) {
    h->vram_ptr = VramBytes(h->vram_ptr, h->vram_inc, src, count, c->bbad == 0x19);
  } else {
    return false;
  }
  return true;
}

// DMA from the A bus to the PPU's memories without going through WriteBBus
// a byte at a time: palettes, OAM and VRAM, from a span of RAM or ROM, as
// the game does almost all its DMA. Returns false for anything else.
static bool FastDma(SnesDmaChannel *c, uint32_t count) {
  SnesHw *h = &g_snes_hw;
  // B to A, or a fixed or decrementing A address.
  if (c->dmap & 0x98)
    return false;
  const uint8_t *src = ABusSpan(c->a1b, c->a1t, count);
  if (!src)
    return false;
  uint8_t mode = c->dmap & 7;
  bool no_remap = !(h->vmain & 0x0c), inc_on_high = h->vmain & 0x80;
  if (c->bbad == 0x22 && (mode == 0 || mode == 2) && !h->cgram_high && !(count & 1) &&
      h->cgram_ptr + count / 2 <= 256) {
    // Whole colors (the unused top bit doesn't matter to the GBA).
    if (!(((uintptr_t)src & 3) | (h->cgram_ptr & 7) | (count & 15))) {
      h->cgram_changed |= CopyChanged((uint32_t *)&g_cgram[h->cgram_ptr], (const uint32_t *)src,
                                      count >> 4, h->cgram_ptr >> 3);
    } else {
      FastDmaRare(c, kCgramBytes, src, count);
    }
    h->cgram_ptr += count / 2;
    h->cgram_dirty = true;
  } else if (c->bbad == 0x04 && (mode == 0 || mode == 2) && !(h->oam_addr & 1) &&
             h->oam_addr + count <= 0x220) {
    // The low table and then the high table, as the game uploads OAM.
    if (!h->oam_addr && count == 0x220 && !((uintptr_t)src & 3)) {
      // All of it. (The high table's two blocks have the bits of half of
      // the low table's blocks each.)
      uint32_t high = CopyChanged((uint32_t *)&g_oam[0x200], (const uint32_t *)(src + 0x200), 2, 0);
      // The low table's blocks past those that the last two frames of gameplay drew: the game
      // hid the sprites it didn't use (by Y), and left the rest of their bytes as they were, so
      // they're as they were the last time (when they were the same in g_oam).
      uint32_t used = g_oam_used, blocks = 32;
      g_oam_used = 0;
      if (used && used != kOamUsedUnknown) {
        uint32_t drawn = (used + 2) >> 2;  // (g_oam_used is the sprites + 1)
        if (oam_prev_blocks)
          blocks = oam_prev_blocks - 1 > drawn ? oam_prev_blocks - 1 : drawn;
        oam_prev_blocks = drawn + 1;
      } else {
        oam_prev_blocks = 0;
      }
      h->oam_changed |= CopyChanged((uint32_t *)g_oam, (const uint32_t *)src, blocks, 0) |
                        (high & 1) * 0xffffu | (high >> 1 & 1) * 0xffff0000u;
    } else {
      FastDmaRare(c, kOamBytes, src, count);
    }
    h->oam_addr = (h->oam_addr + count) & 0x3ff;
  } else if (c->bbad == 0x18 && mode == 1 && no_remap && inc_on_high && !(count & 1)) {
    uint16_t p = h->vram_ptr, inc = h->vram_inc;
    uint32_t words = count / 2;
    if (inc == 1 && 0x8000 - (p & 0x7fff) >= words) {
      UploadVram(p, src, words);
      h->vram_ptr = p + words;
    } else {
      FastDmaRare(c, kVramWrap, src, count);
    }
  } else if (!FastDmaRare(c, kOtherDma, src, count)) {
    return false;
  }
  c->a1t += count;
  return true;
}

static void RunDma(uint8_t channels) {
  // (Mostly channel 0, 1 or both: no need to go through them all.)
  for (int i = 0; channels; i++, channels >>= 1) {
    if (!(channels & 1))
      continue;
    SnesDmaChannel *c = &g_snes_hw.dma[i];
    uint32_t count = c->das ? c->das : 0x10000;
    if (!FastDma(c, count))
      DmaBytes(c, count);
    c->das = 0;
  }
}

// Where each of a DMA channel's registers ($43x0-$43xf) is in SnesDmaChannel, or
// 0xff for none: a byte of the 16-bit ones at a time, the low one first. (In
// IWRAM, with WriteReg.)
#define DMA_REG(field) offsetof(SnesDmaChannel, field)
static const uint8_t kDmaRegOffset[16] IWRAM_DATA = {
  DMA_REG(dmap), DMA_REG(bbad), DMA_REG(a1t), DMA_REG(a1t) + 1, DMA_REG(a1b), DMA_REG(das),
  DMA_REG(das) + 1, DMA_REG(dasb), DMA_REG(a2a), DMA_REG(a2a) + 1, DMA_REG(ntrl),
  DMA_REG(unused), 0xff, 0xff, 0xff, DMA_REG(unused),
};
#undef DMA_REG
// (WriteRegWord stores the 16-bit ones whole.)
_Static_assert(offsetof(SnesHw, dma) % 2 == 0 && sizeof(SnesDmaChannel) % 2 == 0 &&
                   offsetof(SnesDmaChannel, a1t) % 2 == 0 &&
                   offsetof(SnesDmaChannel, das) % 2 == 0 &&
                   offsetof(SnesDmaChannel, a2a) % 2 == 0,
               "DMA registers' 16-bit fields aren't halfword-aligned");
// (DMAP with BBAD is one store too: BBAD must follow DMAP, which must be even.)
_Static_assert(offsetof(SnesDmaChannel, dmap) % 2 == 0 &&
                   offsetof(SnesDmaChannel, bbad) == offsetof(SnesDmaChannel, dmap) + 1 &&
                   sizeof(((SnesDmaChannel *)0)->dmap) == 1 &&
                   sizeof(((SnesDmaChannel *)0)->bbad) == 1,
               "WriteRegWord: DMAP and BBAD aren't one halfword");
// (And A1T, DAS and A2A are each the 16 bits of their two registers.)
_Static_assert(sizeof(((SnesDmaChannel *)0)->a1t) == 2 && sizeof(((SnesDmaChannel *)0)->das) == 2 &&
                   sizeof(((SnesDmaChannel *)0)->a2a) == 2,
               "WriteRegWord: a 16-bit DMA register isn't 16 bits wide");

ROM_CODE static uint8_t ReadDmaReg(uint16_t reg) {
  SnesDmaChannel *c = &g_snes_hw.dma[(reg >> 4) & 7];
  switch (reg & 0xf) {
  case 0x0: return c->dmap;
  case 0x1: return c->bbad;
  case 0x2: return c->a1t;
  case 0x3: return c->a1t >> 8;
  case 0x4: return c->a1b;
  case 0x5: return c->das;
  case 0x6: return c->das >> 8;
  case 0x7: return c->dasb;
  case 0x8: return c->a2a;
  case 0x9: return c->a2a >> 8;
  case 0xa: return c->ntrl;
  default: return c->unused;
  }
}

// (Apart, so that WriteReg needs no stack frame to call it.)
__attribute__((noinline)) static void StartDivision(uint8_t divisor) {
  SnesHw *h = &g_snes_hw;
  h->rddiv = SnesDivide(h->wrdiv, divisor);
  h->rdmpy = SnesModulus(h->wrdiv, divisor);
}

// (The game calls it through the Thumb entry in thumb_entries.S.)
void WriteRegArm(uint16 reg, uint8 value) {
  SnesHw *h = &g_snes_hw;
  if (reg >= 0x2100 && reg < 0x2200) {
    WriteBBus(reg & 0xff, value);
    return;
  }
  if (reg >= 0x4300 && reg < 0x4380) {
    uint8_t offset = kDmaRegOffset[reg & 0xf];
    if (offset != 0xff)
      ((uint8_t *)&h->dma[(reg >> 4) & 7])[offset] = value;
    return;
  }
  switch (reg) {
  case NMITIMEN: h->nmitimen = value; break;
  case WRMPYA: h->wrmpya = value; break;
  case WRMPYB: h->rdmpy = h->wrmpya * value; break;
  case WRDIVL: h->wrdiv = (h->wrdiv & 0xff00) | value; break;
  case WRDIVH: h->wrdiv = (h->wrdiv & 0x00ff) | (value << 8); break;
  case WRDIVB: StartDivision(value); break;
  case HTIMEL: h->htime = (h->htime & 0x100) | value; break;
  case HTIMEH: h->htime = (h->htime & 0xff) | ((value & 1) << 8); break;
  case VTIMEL: h->vtime = (h->vtime & 0x100) | value; break;
  case VTIMEH: h->vtime = (h->vtime & 0xff) | ((value & 1) << 8); break;
  case MDMAEN: RunDma(value); break;
  case HDMAEN:
    h->hdmaen = value;
    hdma_dirty = true;
    break;
  }
}

ROM_CODE uint8 ReadReg(uint16 reg) {
  SnesHw *h = &g_snes_hw;
  // The product first: the game multiplies like that a few dozen times a
  // frame in some fights.
  if (reg - 0x2134u < 3)
    return Mode7Product() >> (8 * (reg - 0x2134));
  if (reg >= 0x2100 && reg < 0x2200)
    return ReadBBus(reg & 0xff);
  if (reg >= 0x4300 && reg < 0x4380)
    return ReadDmaReg(reg);
  switch (reg) {
  case RDNMI: return 0x02;
  case HVBJOY: return 0;
  case RDDIVL: return h->rddiv;
  case RDDIVH: return h->rddiv >> 8;
  case RDMPYL: return h->rdmpy;
  case RDMPYH: return h->rdmpy >> 8;
  case JOY1L: return h->joy1;
  case JOY1H: return h->joy1 >> 8;
  // Only read with the developer tools on (dev_tools.c).
  case JOY2L: return Input_ReadJoypad2();
  case JOY2H: return Input_ReadJoypad2() >> 8;
  default: return 0;
  }
}

ROM_CODE uint16 ReadRegWord(uint16 reg) {
  // (The game reads the middle of the product, as a word.)
  if (reg == MPYM)
    return Mode7Product() >> 8;
  uint16 lo = ReadReg(reg);
  return lo | (ReadReg(reg + 1) << 8);
}

void WriteRegWordArm(uint16 reg, uint16 value) {
  // DMAP and BBAD, A1T, DAS and A2A (registers 0, 2, 5 and 8 of a channel) are
  // both bytes of a field: one store.
  if (reg - 0x4300u < 0x80 && ((0x125 >> (reg & 0xf)) & 1)) {
    *(uint16_t *)((uint8_t *)&g_snes_hw.dma[(reg >> 4) & 7] + kDmaRegOffset[reg & 0xf]) = value;
    return;
  }
  WriteRegArm(reg, (uint8)value);
  WriteRegArm(reg + 1, value >> 8);
}

// The channels HDMA is running, decoded once a frame (and if the game
// writes HDMAEN partway down the screen), in channel order.
typedef struct HdmaChannel {
  SnesDmaChannel *c;
  uint8_t bytes;    // per line
  bool indirect;
  // Writes whole words to scroll registers (see WriteScroll).
  bool scroll;
  uint8_t regs[4];  // the B-bus registers they go to, in order
  // For scroll: the registers its words go to, and Mode 7's (or NULL).
  uint16_t *ofs[2], *m7ofs[2];
  // Writes the windows' edges only (WH0-WH3), or the fixed color only
  // (COLDATA).
  bool window, coldata;
  // Pointers to the table's bank and the indirect data's: see BankPtr.
  const uint8_t *table, *data;
  bool table_ram, data_ram;
} HdmaChannel;

// The channels on: the first hdma_count run line by line; then up to
// hdma_scrolls, those that write only scroll registers (but BG1's, which
// Mode 7 shares); up to hdma_windows, those that write only the windows'
// edges; and last, those that write only the fixed color: those run on
// their own, for all the frame (HdmaScrolls, HdmaWindows,
// SnesHw_HdmaColdata).
static HdmaChannel hdma[8] IWRAM_BSS;
static int hdma_count IWRAM_BSS, hdma_scrolls IWRAM_BSS, hdma_windows IWRAM_BSS,
    hdma_total IWRAM_BSS;

// What HdmaDecode last decoded them from: HDMAEN, and of each channel on, DMAP, BBAD, A1B and DASB
// (a word each), and whether it left any out (for a table that ended).
static uint8_t decoded_hdmaen IWRAM_BSS;
static bool decoded_all IWRAM_BSS;
static uint32_t decoded_regs[8];

ROM_CODE static inline uint32_t HdmaRegs(const SnesDmaChannel *c) {
  return c->dmap | c->bbad << 8 | c->a1b << 16 | (uint32_t)c->dasb << 24;
}

// A pointer to a bank of the A bus, in which [addr] is the byte at addr: all
// of a bank of RAM (*ram), or the upper half of a LoROM bank's.
ROM_CODE static const uint8_t *BankPtr(uint8_t bank, bool *ram) {
  *ram = bank == 0x7e || bank == 0x7f;
  if (*ram)
    return &g_ram[(bank & 1) << 16];
  return (const uint8_t *)((uintptr_t)RomFixedPtr(bank << 16 | 0x8000) - 0x8000);
}

// The lower half of a bank that isn't RAM: never read in the scenes profiled, so out of IWRAM.
ROM_CODE static uint8_t ReadBankLow(uint8_t bank, uint16_t addr) {
  // The first 8 KB of RAM, in the lower banks: HDMA tables, often.
  if (addr < 0x2000 && !(bank & 0x40))
    return g_ram[addr];
  return SnesHw_ReadA(bank << 16 | addr);
}

// (For the code in the cartridge, which can't inline ReadBank from IWRAM.)
ROM_CODE static inline uint8_t ReadBankRom(const uint8_t *p, bool ram, uint8_t bank,
                                           uint16_t addr) {
  if (ram || addr >= 0x8000)
    return p[addr];
  return ReadBankLow(bank, addr);
}

static inline uint8_t ReadBank(const uint8_t *p, bool ram, uint8_t bank, uint16_t addr) {
  if (ram || addr >= 0x8000)
    return p[addr];
  return ReadBankLow(bank, addr);
}

ROM_CODE static void HdmaDecode(void) {
  hdma_count = 0;
  hdma_dirty = false;
  decoded_hdmaen = g_snes_hw.hdmaen;
  decoded_all = true;
  for (int i = 0; i < 8; i++) {
    SnesDmaChannel *c = &g_snes_hw.dma[i];
    if (!(g_snes_hw.hdmaen & (1 << i)))
      continue;
    decoded_regs[i] = HdmaRegs(c);
    if (c->hdma_terminated) {
      decoded_all = false;
      continue;
    }
    HdmaChannel *d = &hdma[hdma_count++];
    uint8_t mode = c->dmap & 7;
    d->c = c;
    d->bytes = kTransferLength[mode];
    d->indirect = c->dmap & 0x40;
    for (int j = 0; j < 4; j++)
      d->regs[j] = c->bbad + kBbadOffsets[mode][j];
    // Modes 2 and 3 write each register twice: the scroll registers' two
    // halves, as most of the game's HDMA does.
    d->scroll = (mode == 2 || mode == 3) && c->bbad >= 0x0d && d->regs[3] <= 0x14;
    d->window = c->bbad >= 0x26 && c->bbad + kBbadOffsets[mode][3] <= 0x29 && !d->scroll;
    // (Modes 0, 2 and 6: a register.)
    d->coldata = c->bbad == 0x32 && !kBbadOffsets[mode][1] && !kBbadOffsets[mode][3];
    for (int k = 0; d->scroll && k < 2; k++) {
      PpuRegs *r = &g_snes_hw.r;
      uint8_t adr = d->regs[2 * k];
      d->ofs[k] = adr & 1 ? &r->hofs[(adr - 0xd) >> 1] : &r->vofs[(adr - 0xe) >> 1];
      d->m7ofs[k] = adr == 0x0d ? &r->m7hofs : adr == 0x0e ? &r->m7vofs : NULL;
    }
    d->table = BankPtr(c->a1b, &d->table_ram);

    d->data = BankPtr(c->dasb, &d->data_ram);
  }
  hdma_total = hdma_count;
  // The scrolling's, then the windows', then the fixed color's, last.
  HdmaChannel sorted[8];
  int n = 0;
  for (int group = 0; group < 4; group++) {
    if (group == 1)
      hdma_count = n;
    if (group == 2)
      hdma_scrolls = n;
    if (group == 3)
      hdma_windows = n;
    for (int k = 0; k < hdma_total; k++) {
      HdmaChannel *d = &hdma[k];
      bool scroll = d->scroll && !d->m7ofs[0] && !d->m7ofs[1];
      int in = scroll ? 1 : d->window ? 2 : d->coldata ? 3 : 0;
      if (in == group)
        sorted[n++] = *d;
    }
  }
  for (int k = 0; k < n; k++)
    hdma[k] = sorted[k];
}

// HdmaNextEntry for a table not in RAM or the upper half of a LoROM bank (which none of the
// scenes profiled has).
ROM_CODE static void HdmaNextEntrySlow(HdmaChannel *d) {
  SnesDmaChannel *c = d->c;
  c->ntrl = ReadBankRom(d->table, d->table_ram, c->a1b, c->a2a++);
  if (d->indirect) {
    c->das = ReadBankRom(d->table, d->table_ram, c->a1b, c->a2a++);
    c->das |= ReadBankRom(d->table, d->table_ram, c->a1b, c->a2a++) << 8;
  }
}

// Reads a channel's next table entry: its line count, and the address of
// its data if indirect.
static void HdmaNextEntry(HdmaChannel *d) {
  SnesDmaChannel *c = d->c;
  uint16_t a = c->a2a;
  if (d->table_ram || (a >= 0x8000 && a <= 0xfffd)) {
    // The usual: all of it from a pointer.
    const uint8_t *p = d->table + a;
    c->ntrl = p[0];
    if (d->indirect) {
      c->das = p[1] | p[2] << 8;
      c->a2a = a + 3;
    } else {
      c->a2a = a + 1;
    }
  } else {
    HdmaNextEntrySlow(d);
  }
  c->hdma_terminated = c->ntrl == 0;
  c->hdma_do_transfer = true;
}

// The times SnesHw_HdmaInit has run: the game doesn't write HDMA tables between that and the
// frame's end.
static uint32_t display_runs;

// The fixed color as it was before line 0, for SnesHw_HdmaColdata.
static uint8_t coldata_before[3];

ROM_CODE void SnesHw_HdmaInit(void) {
  display_runs++;
  for (int i = 0; i < 8; i++) {
    SnesDmaChannel *c = &g_snes_hw.dma[i];
    c->hdma_terminated = false;
    c->hdma_do_transfer = false;
  }
  // As last frame's, as a rule: the game sets its HDMA up once.
  bool same = decoded_all && g_snes_hw.hdmaen == decoded_hdmaen;
  for (int i = 0; same && i < 8; i++)
    same = !(decoded_hdmaen >> i & 1) || HdmaRegs(&g_snes_hw.dma[i]) == decoded_regs[i];
  if (same)
    hdma_dirty = false;
  else
    HdmaDecode();
  bool empty = false;
  for (int k = 0; k < hdma_total; k++) {
    hdma[k].c->a2a = hdma[k].c->a1t;
    HdmaNextEntry(&hdma[k]);
    empty |= hdma[k].c->hdma_terminated;
  }
  // Without the ones whose table is empty.
  if (empty)
    HdmaDecode();
  memcpy(coldata_before, g_snes_hw.r.coldata, 3);
}

// Writes a scroll register's two bytes, low then high, as WriteBBus would.
static inline void WriteScroll(uint8_t adr, uint8_t lo, uint8_t hi) {
  SnesHw *h = &g_snes_hw;
  PpuRegs *r = &h->r;
  uint16_t value = hi << 8 | lo;
  if (adr & 1) {
    if (adr == 0x0d)
      r->m7hofs = value & 0x1fff;
    r->hofs[(adr - 0xd) >> 1] = value & 0x3ff;
    h->bghofs_latch = hi;
  } else {
    if (adr == 0x0e)
      r->m7vofs = value & 0x1fff;
    r->vofs[(adr - 0xe) >> 1] = value & 0x3ff;
  }
  if (adr <= 0x0e) {
    h->m7_latch = hi;
    h->regs_dirty = true;  // Mode 7's
  }
  h->bgofs_latch = hi;
  h->scroll_dirty = true;
}

void SnesHw_HdmaRunLine(void) {
  if (hdma_dirty) {
    hdma_redecoded = true;
    HdmaDecode();
    decoded_all = false;  // (so the next frame decodes again)
  }
  bool terminated = false;
  for (int k = 0; k < hdma_count; k++) {
    HdmaChannel *d = &hdma[k];
    SnesDmaChannel *c = d->c;
    // (Only BG1's scrolling, here: see HdmaScrolls.)
    if (c->hdma_do_transfer && d->scroll) {
      for (int j = 0; j < d->bytes; j += 2) {
        uint8_t lo, hi;
        if (d->indirect) {
          lo = ReadBank(d->data, d->data_ram, c->dasb, c->das++);
          hi = ReadBank(d->data, d->data_ram, c->dasb, c->das++);
        } else {
          lo = ReadBank(d->table, d->table_ram, c->a1b, c->a2a++);
          hi = ReadBank(d->table, d->table_ram, c->a1b, c->a2a++);
        }
        WriteScroll(d->regs[j], lo, hi);
      }
    } else if (c->hdma_do_transfer) {
      for (int j = 0; j < d->bytes; j++) {
        uint8_t v = d->indirect ? ReadBank(d->data, d->data_ram, c->dasb, c->das++)
                                : ReadBank(d->table, d->table_ram, c->a1b, c->a2a++);
        WriteBBus(d->regs[j], v);
      }
    }
    c->ntrl--;
    c->hdma_do_transfer = c->ntrl & 0x80;
    if (!(c->ntrl & 0x7f)) {
      HdmaNextEntry(d);
      terminated |= c->hdma_terminated;
    }
  }
  if (terminated)
    HdmaDecode();
}

void Vector_IRQ(void);

// The channels that write only scroll registers, for all the frame, on
// their own, after the others: Norfair's heat and Maridia's water write
// them on every line, from tables of a line an entry. A channel at a time,
// its state in registers, as HdmaWindows: from a line it writes a
// register on, line_ofs has that value for it, until a line where something
// else (the IRQ, after HDMA on its line) wrote the register (|written|).
// An HDMA table in ROM, indirect, as it runs from its start over a frame:
// on which lines it transfers, and the data's address then. (Norfair's and
// Maridia's are of a line an entry: going through them a byte at a time,
// from the cartridge, was most of HDMA's work.)
enum { kMaxSegs = 4, kUnknownSegs = 0xfe, kTooManySegs = 0xff };
typedef struct BakedTable {
  uint32_t key;   // bank << 16 | address | bytes per line << 24, or 0
  uint32_t used;  // the frame (g_vblank_count) it was last asked for
  uint16_t end;   // the line its ending entry (of 0) starts on, or kNoEnd
  // The entry the last line is in (or 0, not known), and the line it
  // starts on: where to go on from when the table slides up a few lines.
  uint16_t last_a;
  int16_t last_v;
  // The lines (|seg|, nseg of them) from the first it transfers on that the data's address
  // changes on, or kTooManySegs: the data is the same over each, as far as the next. (Most
  // tables keep a register at a few values, not a value a line.) Found when BuildPatches asks
  // (kUnknownSegs until then): the channels it isn't asked of don't need them.
  uint8_t nseg, seg[kMaxSegs];
  uint16_t das[kSnesLines];  // or kNoTransfer
} BakedTable;
enum { kNoTransfer = 0xffff, kNoEnd = 0xffff };

// (In IWRAM: a table that slides, as Norfair's lava does, is gone through every few frames, and
// the cartridge's loop took 40 cycles a line.)
__attribute__((noinline, minsize)) static void BakedSegments(const BakedTable *table) {
  BakedTable *b = (BakedTable *)table;
  int n = 0, a = kNoTransfer;
  for (int v = 0; v < kSnesLines; v++) {
    if (b->das[v] == kNoTransfer || b->das[v] == a)
      continue;
    a = b->das[v];
    if (n == kMaxSegs) {
      n = kTooManySegs;
      break;
    }
    b->seg[n++] = v;
  }
  b->nseg = n;
}

extern volatile uint32_t g_vblank_count;

// The lines |from| to |to| of a table in the bank's ROM |table|, from its
// entry at |a|, which starts on line |v| (by |from|), into |das| (from its
// start), as HdmaInit, SnesHw_HdmaRunLine and HdmaNextEntry run it; the
// line its ending entry starts on, if by |to| + 1, into |end|; and the
// entry |to| is in and the line it starts on, into |last_a| and |last_v|
// (or 0, if none is). False if it goes out of the bank's ROM.
ROM_CODE static bool BakeLines(const uint8_t *table, uint32_t a, int v, int bytes, int from,
                               int to, uint16_t *das, uint16_t *end, uint16_t *last_a,
                               int16_t *last_v) {
  for (int w = from; w <= to; w++)
    das[w - from] = kNoTransfer;
  *end = kNoEnd;
  *last_a = 0;
  // |v|: the line the entry at |a| starts on. Of its |n| lines, the first
  // transfers, and with a count over 0x80 (as ntrl counts down, its top bit
  // still set) the others.
  while (v <= to + 1) {
    if (a < 0x8000 || a > 0xfffd)
      return false;  // (out of the bank's ROM)
    const uint8_t *p = table + a;
    uint8_t ntrl = p[0];
    if (!ntrl) {
      *end = v;
      break;
    }
    int n = ntrl & 0x7f ? ntrl & 0x7f : 128;
    bool repeat = ntrl > 0x80;
    uint16_t first = p[1] | p[2] << 8;
    if (v <= to && to < v + n)
      *last_a = a, *last_v = v;
    a += 3;
    int j = from > v ? from - v : 0;
    for (; j < n && v + j <= to && (!j || repeat); j++) {
      uint16_t x = first + bytes * j;
      if (x == kNoTransfer)
        return false;  // (not seen in the game)
      das[v + j - from] = x;
    }
    v += n;
  }
  return true;
}

// The lines the table from the entry at |a| takes to the one at |to|, after
// it (with entries of 3 bytes), if fewer than a frame's; or -1.
ROM_CODE static int LinesTo(const uint8_t *table, uint32_t a, uint32_t to) {
  // (A multiple of 3 times 3's inverse, mod 2^32, is at most 0xffffffff / 3:
  // no division, which the ARM7TDMI hasn't.)
  uint32_t bytes = to - a;
  if (bytes >= 3 * kSnesLines || bytes * 0xaaaaaaabu > 0x55555555u)
    return -1;
  int v = 0;
  for (; a < to; a += 3) {
    if (a < 0x8000 || a > 0xfffd || !table[a])
      return -1;
    v += table[a] & 0x7f ? table[a] & 0x7f : 128;
    if (v >= kSnesLines)
      return -1;
  }
  return v;
}

// memmove's, of halfwords: it goes a byte at a time when they're 2 bytes
// out of step, as a slide of an odd number of lines leaves them.
__attribute__((no_builtin("memmove"))) ROM_CODE static void MoveLines(uint16_t *to,
                                                                    const uint16_t *from, int n) {
  if (to < from) {
    for (int i = 0; i < n; i++)
      to[i] = from[i];
  } else {
    for (int i = n; i-- > 0;)
      to[i] = from[i];
  }
}

// Of the |n| tables, the one asked for least lately, but for |keep|.
ROM_CODE static BakedTable *LeastUsed(BakedTable *baked, int n, const BakedTable *keep,
                                      uint32_t now) {
  BakedTable *b = NULL;
  for (int i = 0; i < n; i++) {
    if (&baked[i] != keep && (!b || now - baked[i].used > now - b->used))
      b = &baked[i];
  }
  return b;
}

// Whether |n| bytes at |b| are |a|'s (aligned to a word): memcmp is in Thumb, and slow.
ROM_CODE static bool SameBytes(const uint8_t *a, const uint8_t *b, int n) {
  if ((uintptr_t)b & 3)
    return !memcmp(a, b, n);
  const uint32_t *x = (const uint32_t *)a, *y = (const uint32_t *)b;
  for (; n >= 4; n -= 4) {
    if (*x++ != *y++)
      return false;
  }
  return !memcmp(x, y, n);
}

// A scroll table in RAM that the game sets up and leaves be, baked: Phantoon's ripple has its entries
// (of 32 lines each, with the data's addresses) written once, and the data they point to every
// frame. It's baked when it's seen the same twice, and no longer when it's not, byte for
// byte: a copy of the table (up to its end, or the frame's last line) is what it's compared
// with, once a frame. The ones that change every frame (Ridley's lava) are only copied, every
// 16th frame, and go the slow way.
ROM_CODE static const BakedTable *BakedRam(const HdmaChannel *d, BakedTable *baked, int n,
                                           uint32_t key, uint32_t now) {
  enum { kSnapshot = 48 };
  static struct {
    uint32_t key;
    int len;
    uint32_t checked;  // the display_runs the table was last found the same on
    bool baked;        // (b, below, is of it)
    uint8_t bytes[kSnapshot] __attribute__((aligned(4)));
  } snap;
  BakedTable *b = NULL;
  const SnesDmaChannel *c = d->c;
  const uint8_t *p = d->table + c->a1t;
  if (snap.baked && snap.key == key) {
    for (int i = 0; i < n; i++) {
      if (baked[i].key == key)
        b = &baked[i];
    }
    if (b && (snap.checked == display_runs || SameBytes(snap.bytes, p, snap.len))) {
      snap.checked = display_runs;
      b->used = now;
      return b;
    }
    snap.baked = false;
    if (b)
      b->key = 0;
    b = NULL;
  }
  // (Not baked: a table that changes every frame would cost the copy every one, so it's looked
  // at every 16th, to see that it's the same as then.)
  if (now & 15)
    return NULL;
  int len = 0, lines = 0, transfers = 0;
  for (;;) {
    if (len > kSnapshot - 3 || c->a1t + len > 0xfffd)
      return NULL;
    uint8_t ntrl = p[len];
    len += ntrl ? 3 : 1;
    if (!ntrl)
      break;
    int count = ntrl & 0x7f ? ntrl & 0x7f : 128;
    lines += count;
    transfers += ntrl > 0x80 ? count : 1;
    if (lines > kSnesLines)  // (BakeLines reads one entry past the frame's lines)
      break;
  }
  // (A table of a few long entries is quicker as it is, an entry at a time: Crateria's haze.)
  if (transfers < kSnesLines / 2)
    return NULL;
  if (snap.key != key || snap.len != len || !SameBytes(snap.bytes, p, len)) {
    memcpy(snap.bytes, p, len);
    snap.key = key;
    snap.len = len;
    return NULL;
  }
  b = LeastUsed(baked, n, NULL, now);
  b->key = 0;
  if (!BakeLines(d->table, c->a1t, 0, d->bytes, 0, kSnesLines - 1, b->das, &b->end, &b->last_a,
                 &b->last_v))
    return NULL;
  b->nseg = kUnknownSegs;
  b->key = key;
  b->used = now;
  snap.baked = true;
  snap.checked = display_runs;
  return b;
}

// The channel's table, baked, if it's indirect and in ROM (or RAM that stays, see BakedRam); or
// NULL.
ROM_CODE static const BakedTable *Baked(const HdmaChannel *d) {
  enum { kBaked = 6 };  // (Norfair has as many going)
  static BakedTable baked[kBaked];
  const SnesDmaChannel *c = d->c;
  if (!d->indirect || c->a1t < 0x8000)
    return NULL;
  uint32_t key = (uint32_t)c->a1b << 16 | c->a1t | (uint32_t)d->bytes << 24;
  uint32_t now = g_vblank_count;
  if (d->table_ram)
    return d->scroll ? BakedRam(d, baked, kBaked, key, now) : NULL;
  for (int i = 0; i < kBaked; i++) {
    if (baked[i].key == key) {
      baked[i].used = now;
      return &baked[i];
    }
  }
  BakedTable *keep = NULL, *b;
  // Another entry of a table baked: Maridia's water's starts at another of
  // them as the water's surface scrolls. Its lines, slid, and the lines
  // that brings in.
  int slide = 0;
  for (int i = 0; i < kBaked; i++) {
    uint32_t other = baked[i].key;
    if (!other || (other ^ key) >> 16)
      continue;
    int ahead = LinesTo(d->table, other & 0xffff, c->a1t);
    int behind = ahead < 0 ? LinesTo(d->table, c->a1t, other & 0xffff) : -1;
    int lines = ahead >= 0 ? ahead : behind;
    if (lines > 0 && (!keep || lines < (slide < 0 ? -slide : slide)))
      keep = &baked[i], slide = ahead >= 0 ? lines : -lines;
  }
  if (keep) {
    // Into another, if the other table went this frame (for another
    // channel).
    if (keep->used == now) {
      b = LeastUsed(baked, kBaked, keep, now);
      memcpy(b->das, keep->das, sizeof(b->das));
      b->end = keep->end;
      b->last_a = keep->last_a;
      b->last_v = keep->last_v;
    } else {
      b = keep;
    }
    b->key = 0;
    if (slide > 0) {
      MoveLines(b->das, b->das + slide, kSnesLines - slide);
      if (b->end != kNoEnd) {
        b->end -= slide;
        b->last_a = 0;
        for (int v = kSnesLines - slide; v < kSnesLines; v++)
          b->das[v] = kNoTransfer;
      } else {
        // From the entry the last line was in, now |slide| lines up, if
        // known, rather than through the lines before (Maridia's water's
        // are of a line an entry).
        uint32_t a = b->last_a ? b->last_a : c->a1t;
        int v = b->last_a ? b->last_v - slide : 0;
        if (!BakeLines(d->table, a, v, d->bytes, kSnesLines - slide, kSnesLines - 1,
                       b->das + kSnesLines - slide, &b->end, &b->last_a, &b->last_v))
          return NULL;
      }
    } else {
      slide = -slide;
      MoveLines(b->das + slide, b->das, kSnesLines - slide);
      uint16_t end, last_a;
      int16_t last_v;
      if (!BakeLines(d->table, c->a1t, 0, d->bytes, 0, slide - 1, b->das, &end, &last_a, &last_v))
        return NULL;
      b->end = b->end != kNoEnd && b->end + slide <= kSnesLines ? b->end + slide : kNoEnd;
      // The last line's entry, now: it's |slide| lines back, and as the
      // entries are of 3 bytes, the ones before it are at 3 bytes less.
      if (b->last_a) {
        uint32_t a = b->last_a;
        int v = b->last_v;
        while (v > kSnesLines - 1 - slide) {
          a -= 3;
          v -= d->table[a] & 0x7f ? d->table[a] & 0x7f : 128;
        }
        b->last_a = a;
        b->last_v = v + slide;
      }
    }
    b->nseg = kUnknownSegs;
    b->key = key;
    b->used = now;
    return b;
  }
  // Baked the second time a table's asked for, or when it starts a little from one
  // that was (a power bomb's blast moves its table by a few entries a frame, and
  // would never be asked for at the same place; the next frames slide it): some
  // start at another place every frame (as a door's fade scrolls), and baking
  // them would only double the work of going through them.
  static uint32_t seen[8];
  static int next_seen;
  bool again = false;
  for (int i = 0; i < 8; i++) {
    uint32_t other = seen[i];
    // (Within 0x300 bytes of it, in the same bank and at the same bytes a line.)
    if (other && !((other ^ key) >> 16))
      again |= (other & 0xffff) - (key & 0xffff) + 0x2ff < 0x5ff;
  }
  if (!again) {
    seen[next_seen] = key;
    next_seen = (next_seen + 1) & 7;
    return NULL;
  }
  b = LeastUsed(baked, kBaked, NULL, now);
  b->key = 0;
  if (!BakeLines(d->table, c->a1t, 0, d->bytes, 0, kSnesLines - 1, b->das, &b->end, &b->last_a,
                 &b->last_v))
    return NULL;
  b->nseg = kUnknownSegs;
  b->key = key;
  b->used = now;
  return b;
}

// Each line's byte of a register, into every fourth byte of |out|, from
// the data at the baked lines' addresses |das| (plus |j|), in a bank of
// RAM |data|; or the value before (|value|, at first). Returns the last.
// (In IWRAM, for HdmaWindows: a power bomb's explosion's edges.)
__attribute__((noinline)) static uint8_t BakedBytes(uint8_t *out, const uint16_t *das,
                                                    const uint8_t *data, int j, uint8_t value) {
  for (int v = 0; v < kSnesLines; v++) {
    uint16_t a = das[v];
    if (a != kNoTransfer)
      value = data[(uint16_t)(a + j)];
    out[v * 4] = value;
  }
  return value;
}

// The channels that write only the windows' edges, for all the frame's
// lines: as their edges aren't in the states, they don't make every line
// a line HDMA has to stop at (see SnesHw_RunDisplay). A channel at a time,
// its state in registers. Where the game writes the edges itself partway
// down the screen, though, it's lost.
ROM_CODE static void HdmaWindows(uint32_t *line_windows) {
  uint8_t *wh = g_snes_hw.r.wh;
  uint32_t start = wh[0] | wh[1] << 8 | wh[2] << 16 | (uint32_t)wh[3] << 24;
  // (As a rule, there are none, and the edges are as they were.)
  static uint32_t filled IWRAM_BSS;
  static bool uniform IWRAM_BSS;
  if (hdma_scrolls == hdma_windows && uniform && filled == start)
    return;
  for (int v = 0; v < kSnesLines; v++)
    line_windows[v] = start;
  filled = start;
  uniform = hdma_scrolls == hdma_windows;
  // A channel at a time, each of its registers' bytes of every line's word
  // (WH0 in the low byte), in turn.
  uint8_t *out = (uint8_t *)line_windows;
  // (Bytes through the pointers as a rule, as ReadBankRom has them.)
#define WINDOW_READ(p, ram, bank, addr) \
  ((ram) || (addr) >= 0x8000 ? (p)[addr] : ReadBankLow(bank, addr))
  for (int k = hdma_scrolls; k < hdma_windows; k++) {
    HdmaChannel *d = &hdma[k];
    SnesDmaChannel *c = d->c;
    int bytes = d->bytes;
    const BakedTable *b = d->data_ram ? Baked(d) : NULL;
    if (b) {
      // A power bomb's explosion: a table in ROM, of a line an entry, with
      // the edges in RAM. From the baked table, a register at a time.
      for (int j = 0; j < bytes; j++) {
        int reg = d->regs[j] - 0x26;
        wh[reg] = BakedBytes(out + reg, b->das, d->data, j, wh[reg]);
      }
      c->hdma_terminated = b->end <= kSnesLines;
      continue;
    }
    const uint8_t *table = d->table, *data = d->data;
    bool table_ram = d->table_ram, data_ram = d->data_ram, indirect = d->indirect;
    uint8_t a1b = c->a1b, dasb = c->dasb, ntrl = c->ntrl;
    uint16_t a2a = c->a2a, das = c->das;
    bool transfer = c->hdma_do_transfer, terminated = c->hdma_terminated;
    // The first 8 KB of RAM in the lower banks, as a bank of RAM: if the
    // table's there, and the frame's lines of it (3 bytes each at most).
    if (!table_ram && !(a1b & 0x40) && a2a < 0x2000 - 3 * kSnesLines) {
      table = g_ram;
      table_ram = true;
    }
    if (!data_ram && !(dasb & 0x40) && das < 0x2000 - 4 * kSnesLines) {
      data = g_ram;
      data_ram = true;
    }
    // The edges the channel writes (|shift|'s bytes of a line's word, |mask|), and the line's
    // word as it has it, |cur|, which a transfer changes. The X-ray scope's takes a line an entry,
    // 2 bytes a line (from RAM, a pair of edges: WH2's and WH3's), and the rest of the lines just
    // as the last were; a power bomb's a line an entry too, for each edge (through Baked, if it
    // can).
    uint32_t shift[4], mask = 0, cur = wh[0] | wh[1] << 8 | wh[2] << 16 | (uint32_t)wh[3] << 24;
    for (int j = 0; j < bytes; j++) {
      shift[j] = (d->regs[j] - 0x26) * 8;
      mask |= 0xffu << shift[j];
    }
    // Fast, for one edge (bytes 1) or a pair (WH0's and WH1's, or WH2's and WH3's) from RAM: the
    // edges a line are |edge|, stored by |out_byte| or |out_pair| (a line's word, 4 bytes, is 2
    // halfwords on).
    bool fast = indirect && data_ram &&
                (bytes == 1 || (bytes == 2 && shift[1] == shift[0] + 8 && !(shift[0] & 8)));
    uint8_t *out_byte = (uint8_t *)line_windows + shift[0] / 8;
    uint16_t *out_pair = (uint16_t *)line_windows + shift[0] / 16;
    uint32_t edge = cur >> shift[0] & (bytes == 1 ? 0xff : 0xffff);
    // The lines of an entry at a time: the count's, from the first line the frame has of them.
    // The first line transfers if the channel is to, and the rest only if the entry repeats.
    for (int v = 0; v < kSnesLines;) {
      int n = ntrl & 0x7f ? ntrl & 0x7f : 128;
      bool repeat = (ntrl & 0x80) && (ntrl & 0x7f);
      if (terminated)
        n = kSnesLines;
      int lines = n < kSnesLines - v ? n : kSnesLines - v;
      if (fast) {
        // (Whole halfwords of data, if it starts on one.)
        const uint8_t *p = data + das;
        bool half = !(das & 1);
        int i = 0;
        if (!terminated && transfer) {
          edge = bytes == 1 ? *p : half ? *(const uint16_t *)p : p[0] | p[1] << 8;
          p += bytes, das += bytes;
          if (bytes == 1)
            out_byte[4 * v++] = edge;
          else
            out_pair[2 * v++] = edge;
          i = 1;
        }
        if (repeat && !terminated && das + bytes * lines <= 0x10000) {
          das += bytes * (lines - i);
          for (; i < lines; i++, p += bytes) {
            if (bytes == 1)
              out_byte[4 * v++] = edge = *p;
            else
              out_pair[2 * v++] = edge = half ? *(const uint16_t *)p : p[0] | p[1] << 8;
          }
        } else {
          for (; i < lines; i++, v++) {
            if (bytes == 1)
              out_byte[4 * v] = edge;
            else
              out_pair[2 * v] = edge;
          }
        }
      } else {
        for (int i = 0; i < lines; i++, v++) {
          if (!terminated && (i ? repeat : transfer)) {
            for (int j = 0; j < bytes; j++) {
              uint32_t value;
              if (indirect)
                value = WINDOW_READ(data, data_ram, dasb, das), das++;
              else
                value = WINDOW_READ(table, table_ram, a1b, a2a), a2a++;
              cur = (cur & ~(0xffu << shift[j])) | value << shift[j];
            }
          }
          line_windows[v] = (line_windows[v] & ~mask) | (cur & mask);
        }
      }
      if (terminated)
        break;
      ntrl -= lines;
      transfer = ntrl & 0x80;
      if (!(ntrl & 0x7f)) {
        // The next entry, as HdmaNextEntry.
        ntrl = WINDOW_READ(table, table_ram, a1b, a2a), a2a++;
        if (indirect) {
          das = WINDOW_READ(table, table_ram, a1b, a2a), a2a++;
          das |= WINDOW_READ(table, table_ram, a1b, a2a) << 8, a2a++;
        }
        terminated = ntrl == 0;
        transfer = true;
      }
    }
    if (fast)
      cur = (cur & ~mask) | edge << shift[0];
    for (int i = 0; i < 4; i++)
      wh[i] = cur >> (8 * i);
    c->ntrl = ntrl;
    c->a2a = a2a;
    c->das = das;
    c->hdma_do_transfer = transfer;
    c->hdma_terminated = terminated;
  }
#undef WINDOW_READ
}

// The scrolling and states of the lines (line_ofs, line_state) are kept from frame to frame, and
// written in pieces: stretches of lines that have the same, as a rule a few (the HUD, and the room
// below it). A piece is written only where it differs from what the last frame left, so a
// camera that stands still, or a room that scrolls only some of its backgrounds, costs little.
// |last_start| is where the last frame's pieces started, |last_pieces| how many (0: not known, as
// after a frame in which the CPU wrote scroll registers), and |last_varying| the words of line_ofs
// (bits 0-3: BG1-BG4's) that HdmaScrolls wrote a line at a time, which aren't the same over a piece.
enum { kMaxPieces = 8, kMaxPatches = 8 };
static uint8_t last_start[kMaxPieces], last_pieces, last_varying;

// Most of the channels that write only scroll registers hold them at a few values over the frame,
// not a value a line: the HUD's, in nearly every room, keeps BG3 still above the room's. Those
// are patches to the pieces' words, from the line each starts on (in order of lines): a register's
// |half| of line_ofs[line] (BG1's HOFS, VOFS, BG2's...). The rest are HdmaScrolls', a line at a time.
typedef struct Patch {
  uint8_t line, half;
  uint16_t value;
} Patch;
static Patch patches[kMaxPatches], lefts[kMaxPatches];
// The channel (by number) of each of the patches, for BuildPatches.
static uint8_t patch_channel[kMaxPatches];
static uint8_t num_patches, num_lefts;
// The channels (by number) that are patches, and the words (bits 0-3) that the rest write.
static uint8_t patched, varying;

// A piece's lines are written by EmitPiece, which goes along the patches (the next, and the values
// they have given the halves so far, or -1) and the last frame's pieces (the one it's in), and
// makes the pieces of this frame (|next_start|, as last_start) from the changes in what it writes.
static uint8_t patch_at, last_at, next_start[kMaxPieces], next_pieces;
static bool next_over;
static int16_t patch_values[8];
static uint32_t next_words[4];
static int next_state;
static uint32_t (*piece_ofs)[4];
static uint8_t *piece_state;

// A channel's lines are written in turn, a word (BG1-BG4's HOFS | VOFS << 16) at a time, or all four.
static void FillWord(uint32_t (*line_ofs)[4], int from, int to, int word, uint32_t value) {
  uint32_t *p = &line_ofs[from][word], *end = &line_ofs[to][word];
  // (Four lines to the loop: each store to EWRAM costs 7 cycles, and the loop around it 4.)
  for (; end - p >= 16; p += 16) {
    p[0] = value;
    p[4] = value;
    p[8] = value;
    p[12] = value;
  }
  for (; p < end; p += 4)
    *p = value;
}

static void FillWords(uint32_t (*line_ofs)[4], int from, int to, const uint32_t *words) {
  uint32_t w0 = words[0], w1 = words[1], w2 = words[2], w3 = words[3];
  for (uint32_t *p = line_ofs[from], *end = line_ofs[to]; p < end; p += 4) {
    p[0] = w0;
    p[1] = w1;
    p[2] = w2;
    p[3] = w3;
  }
}

ROM_CODE static void BeginPieces(uint32_t (*line_ofs)[4], uint8_t *line_state) {
  piece_ofs = line_ofs;
  piece_state = line_state;
  patch_at = last_at = next_pieces = 0;
  next_over = false;
  next_state = -1;
  for (int i = 0; i < 8; i++)
    patch_values[i] = -1;
}

// Writes lines |a| to |b| (of up to the last), which have |state| and the scrolling |ofs| but for
// the patches, as far as they differ from the last frame's.
ROM_CODE static void EmitPiece(int a, int b, int state, const uint32_t *ofs) {
  while (a < b) {
    while (patch_at < num_patches && patches[patch_at].line <= a) {
      patch_values[patches[patch_at].half] = patches[patch_at].value;
      patch_at++;
    }
    int end = b;
    if (patch_at < num_patches && patches[patch_at].line < end)
      end = patches[patch_at].line;
    // Where the last frame's piece it's in ends.
    while (last_at + 1 < last_pieces && last_start[last_at + 1] <= a)
      last_at++;
    bool known = last_pieces != 0;
    if (known && last_at + 1 < last_pieces && last_start[last_at + 1] < end)
      end = last_start[last_at + 1];
    uint32_t words[4];
    for (int i = 0; i < 4; i++) {
      words[i] = ofs[i];
      if (patch_values[2 * i] >= 0)
        words[i] = (words[i] & 0xffff0000) | patch_values[2 * i];
      if (patch_values[2 * i + 1] >= 0)
        words[i] = (words[i] & 0xffff) | (uint32_t)patch_values[2 * i + 1] << 16;
    }
    // The words that aren't the same as the last frame's over the piece, which is as they are on
    // its first line. (Or, written a line at a time, not the same on any.)
    int changed = 0;
    for (int i = 0; i < 4; i++) {
      if (!known || (last_varying >> i & 1) || piece_ofs[a][i] != words[i])
        changed |= 1 << i;
    }
    if (changed == 15) {
      FillWords(piece_ofs, a, end, words);
    } else {
      for (int i = 0; i < 4; i++) {
        if (changed >> i & 1)
          FillWord(piece_ofs, a, end, i, words[i]);
      }
    }
    if (!known || piece_state[a] != state)
      memset(&piece_state[a], state, end - a);
    // A new piece, if it's not the last one's.
    if (next_state != state || memcmp(next_words, words, sizeof(words))) {
      if (next_pieces == kMaxPieces)
        next_over = true;
      else
        next_start[next_pieces++] = a;
      memcpy(next_words, words, sizeof(words));
      next_state = state;
    }
    a = end;
  }
}

// Puts the patch where the lines go on from (it's one of a channel's few values).
ROM_CODE static void AddPatch(int line, int half, int value, int channel) {
  int i = num_patches++;
  for (; i > 0 && patches[i - 1].line > line; i--) {
    patches[i] = patches[i - 1];
    patch_channel[i] = patch_channel[i - 1];
  }
  patches[i].line = line;
  patches[i].half = half;
  patches[i].value = value;
  patch_channel[i] = channel;
}

// A scroll register's halfword of line_ofs[line] (BG1's HOFS: 0, VOFS: 1, BG2's: 2, 3...)
// for a channel's register |i|, the word it's in (0-3) and its mask in |written|.
ROM_CODE static int ScrollHalf(const HdmaChannel *d, int i, int *bit) {
  uint8_t adr = d->regs[d->bytes == 4 ? 2 * i : 0];
  int reg = adr & 1 ? (adr - 0xd) >> 1 : 4 + ((adr - 0xe) >> 1);
  *bit = 1 << reg;
  return (reg & 3) * 2 + (reg >> 2);
}

// Looks at the channels HdmaScrolls has: which are patches, and what they are.
ROM_CODE static void BuildPatches(void) {
  num_patches = num_lefts = patched = varying = 0;
  // The registers that channels not patches write: a channel on one of them is a line at a time
  // too, as the lines come after the patches. (On other registers the order doesn't matter.)
  int taken_lines = 0;
  uint16_t chan_end[8];  // the line each patching channel's table ends on
  for (int k = hdma_count; k < hdma_scrolls; k++) {
    HdmaChannel *d = &hdma[k];
    SnesDmaChannel *c = d->c;
    int channel = c - g_snes_hw.dma, bit0, bit1 = 0, half0 = ScrollHalf(d, 0, &bit0), half1 = 0;
    bool two = d->bytes == 4;
    if (two)
      half1 = ScrollHalf(d, 1, &bit1);
    const BakedTable *b = Baked(d);
    if (b && !(taken_lines & (bit0 | bit1)) && b->nseg == kUnknownSegs)
      BakedSegments(b);
    if (!b || (taken_lines & (bit0 | bit1)) || b->nseg > kMaxSegs ||
        num_patches + b->nseg * (1 + two) > kMaxPatches) {
      taken_lines |= bit0 | bit1;
      varying |= 1 << (half0 >> 1);
      if (two)
        varying |= 1 << (half1 >> 1);
      continue;
    }
    patched |= 1 << channel;
    c->hdma_terminated = b->end <= kSnesLines;
    // From the line it first transfers on, the channel's value holds over an earlier channel's
    // later patches of the registers (the line path has it so: HUD and room each write BG3's),
    // as far as it goes on transferring. Past its table's end the registers keep its value only
    // until the earlier channel transfers again, which is what that channel's patch in effect
    // there says: Norfair's lava writes BG3's VOFS on every line, and the BG3 scroll channel
    // after it only on the first 32, for the HUD's.
    chan_end[channel] = b->end;
    if (b->nseg) {
      int last = b->end < kSnesLines ? b->end : kSnesLines;  // (the line after its last)
      int n = 0;
      Patch carry[2];
      uint8_t carry_channel[2];
      bool carried[2] = {false, false};
      for (int i = 0; i < num_patches; i++) {
        Patch *p = &patches[i];
        bool mine = p->half == half0 || (two && p->half == half1);
        if (p->line < b->seg[0] || !mine || p->line >= last) {
          patch_channel[n] = patch_channel[i];
          patches[n++] = *p;
        } else if (chan_end[patch_channel[i]] > last) {
          // (The latest, as they're in order of lines.)
          carry[two && p->half == half1] = *p;
          carry_channel[two && p->half == half1] = patch_channel[i];
          carried[two && p->half == half1] = true;
        }
      }
      num_patches = n;
      for (int j = 0; j <= two && last < kSnesLines; j++) {
        bool taken = false;
        for (int i = 0; i < n; i++)
          taken |= patches[i].line == last && patches[i].half == carry[j].half;
        if (carried[j] && !taken)
          AddPatch(last, carry[j].half, carry[j].value, carry_channel[j]);
      }
    }
    for (int i = 0; i < b->nseg; i++) {
      int line = b->seg[i], a = b->das[line];
      for (int j = 0; j <= two; j++) {
        int x = a + 2 * j;
        // (From RAM or ROM's upper half, a halfword in place, or else ReadBank has to.)
        int value = (d->data_ram || ((uint16_t)x >= 0x8000 && (uint16_t)x != 0xffff)
                         ? d->data[(uint16_t)x] | d->data[(uint16_t)(x + 1)] << 8
                         : ReadBankRom(d->data, d->data_ram, c->dasb, (uint16_t)x) |
                               ReadBankRom(d->data, d->data_ram, c->dasb, (uint16_t)(x + 1)) << 8) &
                    0x3ff;
        AddPatch(line, j ? half1 : half0, value, channel);
        // The last is what HDMA leaves in the registers.
        if (i == b->nseg - 1) {
          lefts[num_lefts].half = j ? half1 : half0;
          lefts[num_lefts++].value = value;
        }
      }
    }
  }
}

// A baked table's data in RAM, a line at a time: the halfword at each line's address, held on
// those with none, into |out| (a half of a line_ofs line every 8 halfwords). Returns the bits of the
// addresses ORed, to see whether any was odd (the ARM7TDMI would rotate the halfword). (Their own
// functions, so the loops keep their constants in registers; and the walk is over pointers.)
__attribute__((noinline)) static uint32_t ScrollLine1(uint16_t *out, const uint16_t *das,
                                                      const uint8_t *data, int lines,
                                                      uint32_t *last) {
  uint32_t odd = 0, value = *last;
  const uint16_t *end = das + lines;
  do {
    uint32_t a = *das++;
    if (a != kNoTransfer) {
      odd |= a;
      value = *(const uint16_t *)(data + a) & 0x3ff;
    }
    *out = value;
    out += 8;
  } while (das != end);
  *last = value;
  return odd;
}

// And for two registers, from the halfword at the address and the next. (An address of 0xfffe has
// the next wrap around the bank: that counts as odd.)
__attribute__((noinline)) static uint32_t ScrollLine2(uint16_t *out0, uint16_t *out1,
                                                      const uint16_t *das, const uint8_t *data,
                                                      int lines, uint32_t *last) {
  uint32_t odd = 0, value0 = last[0], value1 = last[1];
  const uint16_t *end = das + lines;
  do {
    uint32_t a = *das++;
    if (a != kNoTransfer) {
      const uint8_t *p = data + a;
      odd |= a | (a + 2) >> 16;
      value0 = *(const uint16_t *)p & 0x3ff;
      value1 = *(const uint16_t *)(p + 2) & 0x3ff;
    }
    *out0 = value0;
    *out1 = value1;
    out0 += 8;
    out1 += 8;
  } while (das != end);
  last[0] = value0;
  last[1] = value1;
  return odd;
}

// A channel that isn't patches, with its table baked and its data in RAM, when nothing else
// wrote the registers: a line at a time, from the first line it transfers on. False if it
// can't (see ScrollLine1).
static bool HdmaScrollsBaked(const HdmaChannel *d, uint32_t (*line_ofs)[4]) {
  const BakedTable *b = Baked(d);
  if (!b || !d->data_ram)
    return false;
  SnesDmaChannel *c = d->c;
  PpuRegs *r = &g_snes_hw.r;
  bool two = d->bytes == 4;
  int bit, half0 = ScrollHalf(d, 0, &bit), half1 = two ? ScrollHalf(d, 1, &bit) : 0;
  // (No lines to write if it never transfers: nothing is left in the registers either.)
  int first = 0;
  while (first < kSnesLines && b->das[first] == kNoTransfer)
    first++;
  if (first < kSnesLines) {
    uint16_t *ofs16 = (uint16_t *)line_ofs;
    uint32_t value[2] = {0, 0};
    uint32_t odd;
    if (two)
      odd = ScrollLine2(ofs16 + first * 8 + half0, ofs16 + first * 8 + half1, b->das + first,
                        d->data, kSnesLines - first, value);
    else
      odd = ScrollLine1(ofs16 + first * 8 + half0, b->das + first, d->data, kSnesLines - first,
                        value);
    if (odd & 1)
      return false;
    // As HDMA left the registers, for the next frame.
    ((half0 & 1) ? r->vofs : r->hofs)[half0 >> 1] = value[0];
    if (two)
      ((half1 & 1) ? r->vofs : r->hofs)[half1 >> 1] = value[1];
  }
  c->hdma_terminated = b->end <= kSnesLines;
  return true;
}

// Lines |v| to |v| + |run| of a channel's registers (|half0|, and |half1| if |two|; |bit0| and
// |bit1| in |written|), with the values it has (-1: it hasn't written the register, or the CPU
// has since). The CPU's write of a register ends the value, until the channel transfers another.
static inline void ScrollRun(uint16_t *ofs16, int v, int run, const uint8_t *written, bool two,
                             int half0, int half1, int bit0, int bit1, int *value0, int *value1) {
  int v0 = *value0, v1 = *value1;
  if (!written) {
    uint16_t *p = ofs16 + v * 8;
    if (v0 >= 0) {
      for (int i = 0; i < run; i++)
        p[i * 8 + half0] = v0;
    }
    if (two && v1 >= 0) {
      for (int i = 0; i < run; i++)
        p[i * 8 + half1] = v1;
    }
  } else {
    for (int end = v + run; v < end; v++) {
      uint8_t w = written[v];
      if (w & bit0)
        v0 = -1;
      else if (v0 >= 0)
        ofs16[v * 8 + half0] = v0;
      if (two) {
        if (w & bit1)
          v1 = -1;
        else if (v1 >= 0)
          ofs16[v * 8 + half1] = v1;
      }
    }
  }
  *value0 = v0;
  *value1 = v1;
}

// (|written|: the scroll registers the CPU wrote on each line, or NULL if it wrote none.) A channel
// that isn't baked (its first frame, or its table in RAM, as Ridley's lava): in ROM. It goes along
// the table an entry at a time, not a line: the lines of an entry have the same value, but where
// the channel transfers on every one (an entry that repeats).
ROM_CODE static void HdmaScrollsUnbaked(HdmaChannel *d, uint32_t (*line_ofs)[4],
                                        const uint8_t *written) {
  PpuRegs *r = &g_snes_hw.r;
  SnesDmaChannel *c = d->c;
  const uint8_t *table = d->table, *data = d->data;
  bool table_ram = d->table_ram, data_ram = d->data_ram, indirect = d->indirect;
  uint8_t a1b = c->a1b, dasb = c->dasb, ntrl = c->ntrl;
  uint16_t a2a = c->a2a, das = c->das;
  bool transfer = c->hdma_do_transfer, terminated = c->hdma_terminated;
  bool two = d->bytes == 4;
  // Its registers (one, or two): their bits in |written|, their halves of
  // line_ofs, and their values (or -1, before it writes them).
  int bit0, bit1 = 0, half0 = ScrollHalf(d, 0, &bit0), half1 = two ? ScrollHalf(d, 1, &bit1) : 0;
  int value0 = -1, value1 = -1;
  uint16_t *ofs16 = (uint16_t *)line_ofs;
  for (int v = 0; v < kSnesLines;) {
    if (terminated) {
      // What it left in the registers stays, unless the CPU wrote them.
      if (value0 < 0 && (!two || value1 < 0))
        break;
      ScrollRun(ofs16, v, kSnesLines - v, written, two, half0, half1, bit0, bit1, &value0,
                &value1);
      break;
    }
    // This entry's lines, or as many as are left of the frame.
    int count = ntrl & 0x7f, lines = count ? count : 128;
    bool repeat = count && (ntrl & 0x80);
    int left = lines < kSnesLines - v ? lines : kSnesLines - v;
    for (int done = 0; done < left;) {
      if (transfer) {
        for (int i = 0; i <= two; i++) {
          uint8_t lo, hi;
          if (indirect) {
            if (data_ram || das >= 0x8000) {
              lo = data[das];
              hi = data[(uint16_t)(das + 1)];
            } else {
              lo = ReadBankRom(data, data_ram, dasb, das);
              hi = ReadBankRom(data, data_ram, dasb, (uint16_t)(das + 1));
            }
            das += 2;
          } else {
            lo = ReadBankRom(table, table_ram, a1b, a2a);
            hi = ReadBankRom(table, table_ram, a1b, (uint16_t)(a2a + 1));
            a2a += 2;
          }
          if (i)
            value1 = (hi << 8 | lo) & 0x3ff;
          else
            value0 = (hi << 8 | lo) & 0x3ff;
        }
      }
      // The same value to the entry's end, or the next line it transfers on.
      int run = repeat ? 1 : left - done;
      ScrollRun(ofs16, v, run, written, two, half0, half1, bit0, bit1, &value0, &value1);
      v += run;
      done += run;
      transfer = repeat;
    }
    if (left < lines) {
      ntrl -= left;
      transfer = ntrl & 0x80;
      break;
    }
    // The next entry, as HdmaNextEntry.
    if (table_ram || (a2a >= 0x8000 && a2a <= 0xfffd)) {
      const uint8_t *p = table + a2a;
      ntrl = p[0];
      if (indirect)
        das = p[1] | p[2] << 8;
      a2a += indirect ? 3 : 1;
    } else {
      ntrl = ReadBankRom(table, table_ram, a1b, a2a++);
      if (indirect) {
        das = ReadBankRom(table, table_ram, a1b, a2a++);
        das |= ReadBankRom(table, table_ram, a1b, a2a++) << 8;
      }
    }
    terminated = ntrl == 0;
    transfer = true;
  }
  c->ntrl = ntrl;
  c->a2a = a2a;
  c->das = das;
  c->hdma_do_transfer = transfer;
  c->hdma_terminated = terminated;
  // As HDMA left the registers, for the next frame.
  if (value0 >= 0)
    ((half0 & 1) ? r->vofs : r->hofs)[half0 >> 1] = value0;
  if (two && value1 >= 0)
    ((half1 & 1) ? r->vofs : r->hofs)[half1 >> 1] = value1;
}

// And a channel with its table baked, which is all but the first frame's: from the baked
// table, with the lines where the CPU wrote a register left out (|num_written| of them).
static void HdmaScrollsOne(HdmaChannel *d, uint32_t (*line_ofs)[4], const uint8_t *written,
                           int num_written) {
  const BakedTable *b = Baked(d);
  if (!b) {
    HdmaScrollsUnbaked(d, line_ofs, num_written ? written : NULL);
    return;
  }
  PpuRegs *r = &g_snes_hw.r;
  SnesDmaChannel *c = d->c;
  const uint8_t *data = d->data;
  bool data_ram = d->data_ram;
  uint8_t dasb = c->dasb;
  bool two = d->bytes == 4;
  int bit0, bit1 = 0, half0 = ScrollHalf(d, 0, &bit0), half1 = two ? ScrollHalf(d, 1, &bit1) : 0;
  int value0 = -1, value1 = -1;
  if (b) {
    // From the baked table: the data, on the lines it transfers; in runs
    // of lines between those where something else wrote the registers.
    uint8_t mask = bit0 | (two ? bit1 : 0);
    uint16_t *ofs16 = (uint16_t *)line_ofs;
    const uint16_t *das_at = b->das;
    for (int v = 0; v < kSnesLines;) {
      int end = kSnesLines;
      if (num_written) {
        for (end = v; end < kSnesLines && !(written[end] & mask);)
          end++;
      }
      if (data_ram && !two) {
        for (; v < end; v++) {
          uint16_t a = das_at[v];
          if (a != kNoTransfer)
            value0 = (data[a] | data[(uint16_t)(a + 1)] << 8) & 0x3ff;
          if (value0 >= 0)
            ofs16[v * 8 + half0] = value0;
        }
      } else if (data_ram) {
        for (; v < end; v++) {
          uint16_t a = das_at[v];
          if (a != kNoTransfer) {
            value0 = (data[a] | data[(uint16_t)(a + 1)] << 8) & 0x3ff;
            value1 = (data[(uint16_t)(a + 2)] | data[(uint16_t)(a + 3)] << 8) & 0x3ff;
          }
          if (value0 >= 0)
            ofs16[v * 8 + half0] = value0;
          if (value1 >= 0)
            ofs16[v * 8 + half1] = value1;
        }
      }
      // Lines with data elsewhere than RAM, or where something else wrote
      // a register.
      for (; v < kSnesLines && (v < end || v == end); v++) {
        uint16_t a = das_at[v];
        if (a != kNoTransfer) {
          value0 = (ReadBankRom(data, data_ram, dasb, a) |
                    ReadBankRom(data, data_ram, dasb, (uint16_t)(a + 1)) << 8) & 0x3ff;
          if (two)
            value1 = (ReadBankRom(data, data_ram, dasb, (uint16_t)(a + 2)) |
                      ReadBankRom(data, data_ram, dasb, (uint16_t)(a + 3)) << 8) & 0x3ff;
        }
        uint8_t w = written[v];
        if (w & bit0)
          value0 = -1;
        else if (value0 >= 0)
          ofs16[v * 8 + half0] = value0;
        if (two) {
          if (w & bit1)
            value1 = -1;
          else if (value1 >= 0)
            ofs16[v * 8 + half1] = value1;
        }
      }
    }
    c->hdma_terminated = b->end <= kSnesLines;
  }
  // As HDMA left the registers, for the next frame.
  if (value0 >= 0)
    ((half0 & 1) ? r->vofs : r->hofs)[half0 >> 1] = value0;
  if (two && value1 >= 0)
    ((half1 & 1) ? r->vofs : r->hofs)[half1 >> 1] = value1;
}

// The channels that write only scroll registers, for all the frame, on their own, after the
// others: Norfair's heat and Maridia's water write them on every line, from tables of a line an
// entry. A channel at a time, its state in registers, as HdmaWindows: from a line it writes a
// register on, line_ofs has that value for it, until a line where something else (the IRQ, after
// HDMA on its line) wrote the register (|written|). Not those that are patches, unless |all|
// (the CPU wrote a register, or the channels aren't those BuildPatches saw).
static void HdmaScrolls(uint32_t (*line_ofs)[4], const uint8_t *written, int num_written,
                        bool all) {
  PpuRegs *r = &g_snes_hw.r;
  for (int i = 0; !all && i < num_lefts; i++) {
    const Patch *p = &lefts[i];
    ((p->half & 1) ? r->vofs : r->hofs)[p->half >> 1] = p->value;
  }
  for (int k = hdma_count; k < hdma_scrolls; k++) {
    HdmaChannel *d = &hdma[k];
    if (!all && (patched >> (d->c - g_snes_hw.dma) & 1))
      continue;
    if (all || num_written || !HdmaScrollsBaked(d, line_ofs))
      HdmaScrollsOne(d, line_ofs, written, num_written);
  }
}

// A write to COLDATA, of the channels it has bits 5-7 set for.
ROM_CODE static void WriteColdata(uint8_t *coldata, uint8_t val) {
  for (int i = 0; i < 3; i++) {
    if (val & (0x20 << i))
      coldata[i] = val & 0x1f;
  }
}

// The fixed color, from a channel of a byte a line with its table baked
// (|das|): into the states, as SnesHw_HdmaColdata, and |coldata|. False if
// the data's below |lowest| (0, or 0x8000 in ROM), where ReadBank has to
// read it.
ROM_CODE static bool BakedColdata(
    const uint16_t *das, const uint8_t *data, uint32_t lowest, PpuRegs *states,
    const uint8_t *line_state, int color_line, uint8_t *coldata) {
  uint32_t color = coldata[0] | coldata[1] << 8 | coldata[2] << 16;
  int prev = -1;
  for (int v = 0; v < kSnesLines; v++) {
    uint16_t a = das[v];
    if (a != kNoTransfer) {
      if (a < lowest)
        return false;
      uint32_t val = data[a];
      for (int i = 0; i < 3; i++) {
        if (val & (0x20 << i))
          color = (color & ~(0xffu << (8 * i))) | (val & 0x1f) << (8 * i);
      }
    }
    int s = line_state[v];
    if (s != prev || v == color_line) {
      uint8_t *o = states[s].coldata;
      o[0] = color, o[1] = color >> 8, o[2] = color >> 16;
    }
    prev = s;
  }
  coldata[0] = color, coldata[1] = color >> 8, coldata[2] = color >> 16;
  return true;
}

// The channels that write only the fixed color (COLDATA), for all the
// frame's lines, from coldata_before, after the others (see snes_hw.h):
// Ceres's backgrounds' gradients write it on every line, which, were it in
// the states, would make every line a state, and one HDMA has to stop at.
// The GBA can't show a gradient: the palette has one line's color
// (|color_line|'s: see FixedMath) and color math a segment's. So each state
// has the color of its first line, but for |color_line|'s, which has that
// line's. (As in HdmaWindows, where the game writes the color itself
// partway down the screen, it's lost.)
ROM_CODE void SnesHw_HdmaColdata(PpuRegs *states, const uint8_t *line_state, int color_line) {
  if (hdma_total == hdma_windows)
    return;
  uint8_t *coldata = coldata_before;
  // As a rule, a channel of a byte a line, its table baked. Or, a channel
  // at a time, a line at a time.
  const HdmaChannel *only = &hdma[hdma_windows];
  const BakedTable *b =
      hdma_total - hdma_windows == 1 && only->bytes == 1 ? Baked(only) : NULL;
  if (b && BakedColdata(b->das, only->data, only->data_ram ? 0 : 0x8000, states, line_state,
                        color_line, coldata)) {
    only->c->hdma_terminated = b->end <= kSnesLines;
    memcpy(g_snes_hw.r.coldata, coldata, 3);  // as HDMA left it, for the next frame
    return;
  }
  // The tables in ROM give the same colors every frame the channels start as they did: the
  // lines the color changes on, remembered (a door room's, of the Bomb Torizo's: two channels
  // of 2 and 1 bytes a line, which the loop below takes 53k cycles a frame to run).
  enum { kKeyWords = 17, kChanges = 16 };
  static uint32_t cached_key[kKeyWords];
  static uint32_t cached_change[kChanges];  // line << 24 | color
  static int cached_changes = -1;           // (-1: none)
  static uint8_t cached_terminated;         // the channels', a bit each
  uint32_t key[kKeyWords] = {0};
  bool rom = hdma_total - hdma_windows < 5;
  for (int k = hdma_windows; k < hdma_total && rom; k++) {
    const HdmaChannel *d = &hdma[k];
    const SnesDmaChannel *c = d->c;
    int i = 4 * (k - hdma_windows);
    key[i] = c->a1b | c->a2a << 8 | c->ntrl << 24;
    key[i + 1] = c->das | c->dasb << 16 | d->bytes << 24 | d->indirect << 28 |
                 c->hdma_terminated << 29 | c->hdma_do_transfer << 30;
    // (Past the lower half of the bank, where ReadBankRom goes to other places than ROM.)
    rom = !d->table_ram && !d->data_ram && c->a2a >= 0x8000 && (!d->indirect || c->das >= 0x8000);
  }
  key[16] = coldata[0] | coldata[1] << 8 | coldata[2] << 16;
  if (rom && cached_changes >= 0 && !memcmp(key, cached_key, sizeof(key))) {
    // A state's color is the one its first line (or color_line) ends with. Its lines are
    // together, so a word of four of the previous state's is skipped.
    uint32_t color = key[16];
    int next = 0, prev = -1;
    bool aligned = !((uintptr_t)line_state & 3);
    for (int v = 0; v < kSnesLines;) {
      uint32_t distance = color_line - v;
      if (aligned && prev >= 0 && distance >= 4 && v + 4 <= kSnesLines &&
          *(const uint32_t *)(line_state + v) == (uint32_t)prev * 0x01010101u) {
        v += 4;
        continue;
      }
      int s = line_state[v];
      if (s != prev || v == color_line) {
        while (next < cached_changes && (cached_change[next] >> 24) <= (uint32_t)v)
          color = cached_change[next++] & 0xffffff;
        uint8_t *o = states[s].coldata;
        o[0] = color, o[1] = color >> 8, o[2] = color >> 16;
      }
      prev = s;
      v++;
    }
    if (cached_changes)
      color = cached_change[cached_changes - 1] & 0xffffff;
    coldata[0] = color, coldata[1] = color >> 8, coldata[2] = color >> 16;
    for (int k = hdma_windows; k < hdma_total; k++)
      hdma[k].c->hdma_terminated = cached_terminated >> (k - hdma_windows) & 1;
    memcpy(g_snes_hw.r.coldata, coldata, 3);
    return;
  }
  int changes = rom ? 0 : -1;
  uint32_t last = key[16];
  int prev = -1;
  for (int v = 0; v < kSnesLines; v++) {
    for (int k = hdma_windows; k < hdma_total; k++) {
      HdmaChannel *d = &hdma[k];
      SnesDmaChannel *c = d->c;
      if (c->hdma_terminated)
        continue;
      if (c->hdma_do_transfer) {
        for (int j = 0; j < d->bytes; j++)
          WriteColdata(coldata, d->indirect
                                    ? ReadBankRom(d->data, d->data_ram, c->dasb, c->das++)
                                    : ReadBankRom(d->table, d->table_ram, c->a1b, c->a2a++));
      }
      c->ntrl--;
      c->hdma_do_transfer = c->ntrl & 0x80;
      if (!(c->ntrl & 0x7f))
        HdmaNextEntry(d);
    }
    if (changes >= 0) {
      uint32_t color = coldata[0] | coldata[1] << 8 | coldata[2] << 16;
      if (color != last) {
        last = color;
        if (changes == kChanges)
          changes = -1;
        else
          cached_change[changes++] = (uint32_t)v << 24 | color;
      }
    }
    int s = line_state[v];
    if (s != prev || v == color_line) {
      PpuRegs *r = &states[s];
      r->coldata[0] = coldata[0], r->coldata[1] = coldata[1], r->coldata[2] = coldata[2];
    }
    prev = s;
  }
  if (changes >= 0) {
    memcpy(cached_key, key, sizeof(key));
    cached_terminated = 0;
    for (int k = hdma_windows; k < hdma_total; k++)
      cached_terminated |= hdma[k].c->hdma_terminated << (k - hdma_windows);
  }
  cached_changes = changes;
  memcpy(g_snes_hw.r.coldata, coldata, 3);  // as HDMA left it, for the next frame
}

// How many lines from now HDMA does nothing but count down: for each
// channel, the lines left in a table entry that doesn't repeat, after its
// first. Up to |max|.
static int HdmaQuietLines(int max) {
  for (int k = 0; k < hdma_count && max; k++) {
    SnesDmaChannel *c = hdma[k].c;
    int quiet = (c->hdma_do_transfer || (c->ntrl & 0x80)) ? 0 : (c->ntrl & 0x7f) - 1;
    if (quiet < max)
      max = quiet;
  }
  return max;
}

static void HdmaSkipLines(int lines) {
  for (int k = 0; k < hdma_count; k++)
    hdma[k].c->ntrl -= lines;
}

// But for the scrolling (bytes 10-25: the top of word 2, words 3-5 and the
// bottom of word 6), which has states of its own, and the windows' edges
// (bytes 47-50: the top of word 11 and the bottom of word 12).
_Static_assert(offsetof(PpuRegs, hofs) == 10 && offsetof(PpuRegs, vofs) == 18 &&
                   offsetof(PpuRegs, wh) == 47 && sizeof(PpuRegs) == 64,
               "PpuRegs's layout");
static bool SameRegs(const PpuRegs *a, const PpuRegs *b) {
  const uint32_t *x = (const uint32_t *)a, *y = (const uint32_t *)b;
  if (x[0] != y[0] || x[1] != y[1] || ((x[2] ^ y[2]) & 0xffff) || ((x[6] ^ y[6]) & 0xffff0000))
    return false;
  for (unsigned i = 7; i < 11; i++) {
    if (x[i] != y[i])
      return false;
  }
  for (unsigned i = 13; i < sizeof(PpuRegs) / 4; i++) {
    if (x[i] != y[i])
      return false;
  }
  return !((x[11] ^ y[11]) & 0x00ffffff) && !((x[12] ^ y[12]) & 0xff000000);
}

// SnesHw_RunDisplay from line |v| (of which the first |quiet| are done with HDMA) to the
// frame's end a line at a time, once the CPU has written scroll registers or HDMA changed:
// a state per change and a scrolling per line. Returns the number of states, and of lines the
// CPU wrote scroll registers on (|written|'s) in the high half.
ROM_CODE static uint32_t RunDisplayAll(PpuRegs *states, int n, uint8_t *line_state,
                                       uint32_t (*line_ofs)[4], int v, int quiet, uint32_t *ofs,
                                       uint8_t *written) {
  SnesHw *h = &g_snes_hw;
  int num_written = 0;
  for (;;) {
    if (h->scroll_written | hdma_redecoded) {
      hdma_redecoded = false;
      written[v] = h->scroll_written;
      num_written++;
      h->scroll_written = 0;
    }
    for (int end = v + quiet; v < end; v++) {
      line_state[v] = n - 1;
      line_ofs[v][0] = ofs[0];
      line_ofs[v][1] = ofs[1];
      line_ofs[v][2] = ofs[2];
      line_ofs[v][3] = ofs[3];
    }
    if (v >= kSnesLines)
      break;
    // The same line by line walk as SnesHw_RunDisplay's.
    bool irq = (h->nmitimen & 0x30) && h->vtime < kSnesLines;
    quiet = HdmaQuietLines((irq && h->vtime >= v ? h->vtime : kSnesLines) - v);
    if (quiet) {
      HdmaSkipLines(quiet);
    } else {
      SnesHw_HdmaRunLine();
      if (irq && v == h->vtime)
        Vector_IRQ();
      quiet = 1;
    }
    if (h->regs_dirty) {
      h->regs_dirty = false;
      if (!n || !SameRegs(&states[n - 1], &h->r))
        states[n++] = h->r;
    }
    if (h->scroll_dirty) {
      h->scroll_dirty = false;
      for (int i = 0; i < 4; i++)
        ofs[i] = h->r.hofs[i] | (uint32_t)h->r.vofs[i] << 16;
    }
  }
  return n | (uint32_t)num_written << 16;
}

int SnesHw_RunDisplay(PpuRegs *states, uint8_t *line_state, uint32_t *line_windows,
                      uint32_t (*line_ofs)[4]) {
  SnesHw *h = &g_snes_hw;
  SnesHw_HdmaInit();
  HdmaWindows(line_windows);
  BuildPatches();
  BeginPieces(line_ofs, line_state);
  // The scroll registers the CPU wrote on each line, for HdmaScrolls (as a rule it writes none:
  // the IRQ's registers aren't these). When it does, the rest of the frame's lines are written
  // one at a time (|all|).
  static uint8_t written[kSnesLines];
  int num_written = 0;
  bool all = false;
  int n = 0;
  hdma_redecoded = false;
  h->regs_dirty = h->scroll_dirty = true;
  h->scroll_written = 0;
  // The scrolling, as line_ofs has it.
  uint32_t ofs[4];
  // The lines since |piece| that have the same state and scrolling, as one piece.
  int piece = 0, piece_state = -1;
  uint32_t piece_ofs[4] = {0, 0, 0, 0};
  // A line shows the registers as HDMA and the IRQ at the end of the line
  // before it left them (as src/snes/snes.c draws a frame).
  for (int v = 0; v < kSnesLines;) {
    bool irq = (h->nmitimen & 0x30) && h->vtime < kSnesLines;
    // Lines where HDMA only counts down, before the IRQ's, if any.
    int quiet = HdmaQuietLines((irq && h->vtime >= v ? h->vtime : kSnesLines) - v);
    if (quiet) {
      HdmaSkipLines(quiet);
    } else {
      SnesHw_HdmaRunLine();
      if (irq && v == h->vtime)
        Vector_IRQ();
      quiet = 1;
    }
    // HDMA often writes registers with the values they have.
    if (h->regs_dirty) {
      h->regs_dirty = false;
      if (!n || !SameRegs(&states[n - 1], &h->r))
        states[n++] = h->r;
    }
    if (h->scroll_dirty) {
      h->scroll_dirty = false;
      for (int i = 0; i < 4; i++)
        ofs[i] = h->r.hofs[i] | (uint32_t)h->r.vofs[i] << 16;
    }
    // (Or the channels changed partway down, so they aren't those BuildPatches decided on: from
    // here every channel is a line at a time, and nothing is kept.)
    if (h->scroll_written | hdma_redecoded) {
      if (v > piece)
        EmitPiece(piece, v, piece_state, piece_ofs);
      all = true;
      uint32_t done = RunDisplayAll(states, n, line_state, line_ofs, v, quiet, ofs, written);
      n = done & 0xffff;
      num_written = done >> 16;
      break;
    }
    if (n - 1 != piece_state || ofs[0] != piece_ofs[0] || ofs[1] != piece_ofs[1] ||
        ofs[2] != piece_ofs[2] || ofs[3] != piece_ofs[3]) {
      if (v > piece)
        EmitPiece(piece, v, piece_state, piece_ofs);
      piece = v;
      piece_state = n - 1;
      for (int i = 0; i < 4; i++)
        piece_ofs[i] = ofs[i];
    }
    v += quiet;
  }
  if (!all)
    EmitPiece(piece, kSnesLines, piece_state, piece_ofs);
  if (hdma_scrolls > hdma_count)
    HdmaScrolls(line_ofs, written, num_written, all);
  if (num_written)
    memset(written, 0, sizeof(written));
  if (all || next_over) {
    last_pieces = last_varying = 0;
  } else {
    memcpy(last_start, next_start, sizeof(last_start));
    last_pieces = next_pieces;
    last_varying = varying;
  }
  return n;
}

// (The pieces are lines with the same scrolling, as RunDisplay left them; none if it wrote the
// lines one at a time.)
ROM_CODE const uint8_t *SnesHw_OfsRunStarts(int *count, int *words) {
  *count = last_pieces;
  *words = last_varying;
  return last_pieces ? last_start : NULL;
}
