// Converting SNES tiles and maps to the GBA's formats: see convert.iwram.c.
#pragma once

#include <stdint.h>

void Convert_Init(void);
void Convert_Tiles4bpp(volatile uint32_t *dst, const uint16_t *src, int count);
void Convert_Tile4bppAndMirrored(volatile uint32_t *dst, volatile uint32_t *mirrored,
                                 const uint16_t *src);
void Convert_Tiles2bpp(volatile uint32_t *dst, const uint16_t *src, int count);
// As Convert_Tiles4bpp and 2bpp, for squashed screens: rows 0 and 4
// merged into rows 1 and 3.
void Convert_Tiles4bppSquashed(volatile uint32_t *dst, const uint16_t *src, int count);
void Convert_Tiles2bppSquashed(volatile uint32_t *dst, const uint16_t *src, int count);
// For those, BG palette 0's colors by brightness: |rank|[color], higher's
// brighter.
void Convert_SetSquashRanks(const uint8_t rank[16]);
// The maps of BG1, BG2 and BG3, 32 chunks of 16 entries at a time: see convert_map.S.
void Convert_MapChunks(uint32_t todo, volatile uint16_t *lo, volatile uint16_t *hi,
                       const uint16_t *src, uint8_t *counts, uint16_t *cols, uint32_t blank,
                       uint32_t add);
