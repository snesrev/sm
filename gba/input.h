// The GBA's buttons, as the SNES joypad and the item the game should have
// selected: see input.c.
#pragma once

#include <stdint.h>

// The SNES joypad's buttons, as the game reads them.
enum {
  kSnesB = 0x8000, kSnesY = 0x4000, kSnesSelect = 0x2000, kSnesStart = 0x1000,
  kSnesUp = 0x0800, kSnesDown = 0x0400, kSnesLeft = 0x0200, kSnesRight = 0x0100,
  kSnesA = 0x0080, kSnesX = 0x0040, kSnesL = 0x0020, kSnesR = 0x0010,
};

// Reads the buttons, once a frame of the game, before it.
uint16_t Input_ReadJoypad(void);

// What the SNES's second controller reads, for the developer tools.
uint16_t Input_ReadJoypad2(void);
