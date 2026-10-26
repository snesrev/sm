// The game's data, which tools/make_cartridge.py appends to the engine's
// image (sm-engine.gba) from the player's own copy of the SNES ROM: the ROM
// can't be distributed, so neither can a cartridge with it built in.
//
// The data starts at the end of the image (gba.ld's __cart_data), with this
// header, then the ROM, the audio samples, decoded, and the squashed screens' font
// tiles. Keep this in step
// with make_cartridge.py.
#pragma once

#include <stdint.h>

#define CART_DATA_MAGIC "SMGBA\0\0\0"
enum {
  kCartDataVersion = 3,
  kCartDataHeaderSize = 256,
  kSnesRomSize = 0x300000,
};

typedef struct CartData {
  char magic[8];
  uint32_t version;
  uint32_t snes_rom_size;
  // From the start of the header: a uint32_t count of AudioSamples, them,
  // then their PCM.
  uint32_t audio_offset, audio_size;
  // From the start of the header: the font tiles redrawn for the squashed
  // screens (SquashFont).
  uint32_t squash_offset, squash_size;
} CartData;

// A sample the game's uploads to the APU put in the sample directory, as
// 8-bit PCM. In order of where its BRR is in the ROM.
typedef struct AudioSample {
  uint32_t rom_offset;  // of its BRR
  uint32_t pcm_offset;  // from the start of the audio
  uint32_t length;
  uint32_t loops;
} AudioSample;

// The squashed screens' font tiles (tools/squash_font.py): a SquashHeader, a
// table of 1 << slot_bits uint16_t (0 empty, or the index of a SquashTile
// plus 1), found by SquashHash of the SNES tile's bytes and probing on
// linearly; then the SquashTiles. A tile of |bpp| bits per pixel that matches
// |src| (its first bpp * 4 halfwords) is converted as |dst| instead: the GBA's
// 4bpp form, as the converters make it.
typedef struct SquashHeader {
  uint32_t count;
  uint32_t slot_bits;
} SquashHeader;

typedef struct SquashTile {
  uint32_t bpp;  // 2 or 4
  uint16_t src[16];
  uint32_t dst[8];
} SquashTile;

// Of a tile's bpp * 4 halfwords: a multiplicative hash. (Keep this in step
// with make_cartridge.py. Always inlined: its one caller is in ROM, and a copy
// of its own would take IWRAM, where the file's code goes by default.)
static inline __attribute__((always_inline)) uint32_t SquashHash(const uint16_t *src,
                                                                  uint32_t bpp) {
  uint32_t h = bpp;
  for (uint32_t i = 0; i < bpp * 4; i++)
    h = (h ^ src[i]) * 0x9E3779B1u;
  return h;
}

extern const CartData __cart_data;
