// Converting SNES graphics to the GBA's formats, in ARM code in IWRAM, as
// it's done for every tile the game changes. (No division in here: the
// compiler-rt helpers are Thumb, see cmake/compiler-rt.cmake.)
#include "convert.h"

#include <stdbool.h>
#include <string.h>

#include "cart_data.h"
#include "gba_hw.h"

// Spreads a byte's bits a nibble apart, the leftmost pixel (bit 7) into
// the low nibble: a bitplane of a row of SNES pixels, as GBA 4bpp pixels.
static uint32_t expand[256] IWRAM_BSS;

ROM_CODE void Convert_Init(void) {
  for (int b = 0; b < 256; b++) {
    uint32_t v = 0;
    for (int k = 0; k < 8; k++) {
      if (b & (0x80 >> k))
        v |= 1u << (4 * k);
    }
    expand[b] = v;
  }
}

// A SNES 4bpp tile is 16 words: rows of bitplanes 0 and 1, then rows of
// bitplanes 2 and 3.
void Convert_Tiles4bpp(volatile uint32_t *dst, const uint16_t *src, int count) {
  for (; count > 0; count--, src += 16, dst += 8) {
#pragma clang loop unroll(disable)
    for (int r = 0; r < 8; r++) {
      uint32_t p01 = src[r], p23 = src[r + 8];
      dst[r] = expand[p01 & 0xff] | expand[p01 >> 8] << 1 | expand[p23 & 0xff] << 2 |
               expand[p23 >> 8] << 3;
    }
  }
}

// As Convert_Tiles4bpp, a tile, and at |mirrored|, the tile flipped left to
// right (for sprites outside gameplay: see render.c's SquashSprite; in ROM,
// then).
ROM_CODE void Convert_Tile4bppAndMirrored(volatile uint32_t *dst, volatile uint32_t *mirrored,
                                          const uint16_t *src) {
  // (Unrolled, it would take 400 more bytes of IWRAM.)
#pragma clang loop unroll(disable)
  for (int r = 0; r < 8; r++) {
    uint32_t p01 = src[r], p23 = src[r + 8];
    uint32_t w = expand[p01 & 0xff] | expand[p01 >> 8] << 1 | expand[p23 & 0xff] << 2 |
                 expand[p23 >> 8] << 3;
    dst[r] = w;
    // Its nibbles (pixels) in reverse order.
    w = w >> 16 | w << 16;
    w = (w >> 8 & 0x00ff00ff) | (w & 0x00ff00ff) << 8;
    mirrored[r] = (w >> 4 & 0x0f0f0f0f) | (w & 0x0f0f0f0f) << 4;
  }
}

// For tiles squashed, the colors by brightness, 0-15 (0, transparent,
// least): 4bpp tiles' in BG palette 0, where the menus' text is; 2bpp
// tiles' (BG3's) by the colors' order in its palettes as a rule: 2, the
// HUD's letters, over 1, the intro's letters and the HUD's shadows, over 3,
// outlines and backgrounds.
static uint8_t squash_rank[16];
static const uint8_t kRank2bpp[16] = {0, 2, 3, 1};

ROM_CODE void Convert_SetSquashRanks(const uint8_t rank[16]) {
  memcpy(squash_rank, rank, sizeof(squash_rank));
  squash_rank[0] = 0;
}

// A row of pixels' ranks, a nibble each.
ROM_CODE static uint32_t Ranks(uint32_t row, const uint8_t *rank) {
  uint32_t ranks = 0;
  for (int x = 0; x < 32; x += 4)
    ranks |= (uint32_t)rank[row >> x & 15] << x;
  return ranks;
}

// Of two rows' ranks, a nibble each, those where |a|'s is higher: 15 in
// those nibbles. (Four at a time, in bytes, so that a subtraction has a
// bit to borrow from.)
ROM_CODE static uint32_t Higher(uint32_t a, uint32_t b) {
  uint32_t mask = 0;
  for (int shift = 0; shift < 8; shift += 4) {
    uint32_t x = (a >> shift & 0x0f0f0f0f) | 0x10101010, y = (b >> shift & 0x0f0f0f0f) + 0x01010101;
    mask |= ((x - y) >> 4 & 0x01010101) * 15 << shift;
  }
  return mask;
}

// Merges row 0 of a converted tile into row 1, and row 4 into row 3: their
// pixels show where they're brighter (by |rank|, transparent least), so
// that text keeps its strokes over its shadows and outlines. Outside
// gameplay, the lines showing rows 0 and 4 don't show (see render.c's
// SetSquashLines): the rows next to the 8x8 fonts' middle bars, and next
// to the 16-pixel ones' top bars and before their middle ones.
ROM_CODE static void MergeSquashedRows(volatile uint32_t *dst, const uint8_t *rank) {
  for (int r = 0; r < 8; r += 4) {
    int into = r ? r - 1 : 1;
    uint32_t kept = dst[into], gone = dst[r];
    uint32_t wins = Higher(Ranks(gone, rank), Ranks(kept, rank));
    dst[into] = (kept & ~wins) | (gone & wins);
  }
}

// The ranks for a converted 4bpp tile of one opaque color and another that
// fills its bottom right pixel, where the 8x8 fonts have their gaps between
// letters and lines: that one is the background, and loses to the other. The
// tiles' palettes aren't known here, and the pause screen's letters use all
// of them: ranked by BG palette 0's order, whose colors don't match, they
// lost their strokes to their backgrounds. Else (outlines, shading, and the
// text on nothing), squash_rank's order.
ROM_CODE static const uint8_t *TileRanks(const volatile uint32_t *tile, uint8_t rank[16]) {
  uint32_t colors = 0;
  for (int r = 0; r < 8; r++) {
    uint32_t row = tile[r];
    for (int x = 0; x < 32; x += 4)
      colors |= 1u << (row >> x & 15);
  }
  int gap = tile[7] >> 28;
  colors &= ~(1u | 1u << gap);
  // (One color left, a power of two.)
  if (!gap || !colors || (colors & (colors - 1)))
    return squash_rank;
  memset(rank, 0, 16);
  rank[gap] = 1;
  rank[__builtin_ctz(colors)] = 2;
  return rank;
}

// The tile the cartridge has redrawn for the squash in place of the SNES tile
// at |src| (see cart_data.h's SquashTile), or NULL: the font tiles, which are
// drawn to read with 6 of their 8 rows (tools/squash_font.py).
ROM_CODE static const SquashTile *FindSquashTile(const uint16_t *src, uint32_t bpp) {
  const uint8_t *section = (const uint8_t *)&__cart_data + __cart_data.squash_offset;
  const SquashHeader *header = (const SquashHeader *)section;
  const uint16_t *slots = (const uint16_t *)(header + 1);
  const SquashTile *tiles = (const SquashTile *)(slots + (1u << header->slot_bits));
  uint32_t mask = (1u << header->slot_bits) - 1;
  for (uint32_t i = SquashHash(src, bpp) >> (32 - header->slot_bits);; i = (i + 1) & mask) {
    if (!slots[i])
      return NULL;
    const SquashTile *t = &tiles[slots[i] - 1];
    if (t->bpp != bpp)
      continue;
    uint32_t j = 0;
    while (j < bpp * 4 && t->src[j] == src[j])
      j++;
    if (j == bpp * 4)
      return t;
  }
}

ROM_CODE static void CopySquashTile(volatile uint32_t *dst, const SquashTile *t) {
  for (int r = 0; r < 8; r++)
    dst[r] = t->dst[r];
}

ROM_CODE void Convert_Tiles4bppSquashed(volatile uint32_t *dst, const uint16_t *src, int count) {
  for (; count > 0; count--, src += 16, dst += 8) {
    const SquashTile *redrawn = FindSquashTile(src, 4);
    if (redrawn) {
      CopySquashTile(dst, redrawn);
      continue;
    }
    Convert_Tiles4bpp(dst, src, 1);
    uint8_t rank[16];
    MergeSquashedRows(dst, TileRanks(dst, rank));
  }
}

ROM_CODE void Convert_Tiles2bppSquashed(volatile uint32_t *dst, const uint16_t *src, int count) {
  for (; count > 0; count--, src += 8, dst += 8) {
    const SquashTile *redrawn = FindSquashTile(src, 2);
    if (redrawn) {
      CopySquashTile(dst, redrawn);
      continue;
    }
    Convert_Tiles2bpp(dst, src, 1);
    MergeSquashedRows(dst, kRank2bpp);
  }
}

// A SNES 2bpp tile is 8 words: rows of bitplanes 0 and 1.
ROM_CODE void Convert_Tiles2bpp(volatile uint32_t *dst, const uint16_t *src, int count) {
  for (; count > 0; count--, src += 8, dst += 8) {
    for (int r = 0; r < 8; r++) {
      uint32_t p01 = src[r];
      dst[r] = expand[p01 & 0xff] | expand[p01 >> 8] << 1;
    }
  }
}

