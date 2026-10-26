// GBA hardware registers and memory used by the port.
// https://problemkaputt.de/gbatek.htm
#pragma once

#include <stdint.h>

#define REG16(a) (*(volatile uint16_t *)(a))
#define REG32(a) (*(volatile uint32_t *)(a))

#define REG_DISPCNT REG16(0x04000000)
#define REG_DISPSTAT REG16(0x04000004)
#define REG_VCOUNT REG16(0x04000006)
#define REG_BGCNT(n) REG16(0x04000008 + 2 * (n))
#define REG_BGHOFS(n) REG16(0x04000010 + 4 * (n))
#define REG_BGVOFS(n) REG16(0x04000012 + 4 * (n))
#define REG_BG2PA REG16(0x04000020)
#define REG_BG2PB REG16(0x04000022)
#define REG_BG2PC REG16(0x04000024)
#define REG_BG2PD REG16(0x04000026)
#define REG_BG2X REG32(0x04000028)
#define REG_BG2Y REG32(0x0400002C)
#define REG_WIN0H REG16(0x04000040)
#define REG_WIN1H REG16(0x04000042)
#define REG_WIN0V REG16(0x04000044)
#define REG_WIN1V REG16(0x04000046)
#define REG_WININ REG16(0x04000048)
#define REG_WINOUT REG16(0x0400004A)
#define REG_MOSAIC REG16(0x0400004C)
#define REG_BLDCNT REG16(0x04000050)
#define REG_BLDALPHA REG16(0x04000052)
#define REG_BLDY REG16(0x04000054)

#define REG_SOUNDCNT_L REG16(0x04000080)
#define REG_SOUNDCNT_H REG16(0x04000082)
#define REG_SOUNDCNT_X REG16(0x04000084)
#define REG_SOUNDBIAS REG16(0x04000088)
#define REG_FIFO_A REG32(0x040000A0)

#define REG_DMASAD(n) REG32(0x040000B0 + 12 * (n))
#define REG_DMADAD(n) REG32(0x040000B4 + 12 * (n))
#define REG_DMACNT(n) REG32(0x040000B8 + 12 * (n))

#define REG_TMCNT_L(n) REG16(0x04000100 + 4 * (n))
#define REG_TMCNT_H(n) REG16(0x04000102 + 4 * (n))

#define REG_KEYINPUT REG16(0x04000130)
#define REG_IE REG16(0x04000200)
#define REG_IF REG16(0x04000202)
#define REG_WAITCNT REG16(0x04000204)
#define REG_IME REG16(0x04000208)

// The BIOS's copy of IF, which IntrWait checks, and the IRQ vector.
#define REG_IFBIOS REG16(0x03007FF8)
#define REG_IRQ_VECTOR REG32(0x03007FFC)

enum {
  kDispcntMode0 = 0,
  kDispcntMode1 = 1,
  kDispcntObj1d = 1 << 6,
  kDispcntForcedBlank = 1 << 7,
  kDispcntBg0 = 1 << 8,
  kDispcntObj = 1 << 12,
  kDispcntWin0 = 1 << 13,
  kDispcntWin1 = 1 << 14,

  kDispstatVBlankIrq = 1 << 3,
  kDispstatHBlankIrq = 1 << 4,
  kDispstatVCountIrq = 1 << 5,

  kIrqVBlank = 1 << 0,
  kIrqHBlank = 1 << 1,
  kIrqVCount = 1 << 2,
  kIrqTimer0 = 1 << 3,
  kIrqTimer1 = 1 << 4,
  kIrqDma1 = 1 << 9,

  kDmaDestInc = 0 << 21,
  kDmaDestFixed = 2 << 21,
  kDmaDestReload = 3 << 21,
  kDmaSrcInc = 0 << 23,
  kDmaSrcFixed = 2 << 23,
  kDmaRepeat = 1 << 25,
  kDma16 = 0 << 26,
  kDma32 = 1 << 26,
  kDmaNow = 0 << 28,
  kDmaAtVBlank = 1 << 28,
  kDmaAtHBlank = 2 << 28,
  kDmaAtSoundFifo = 3 << 28,
  kDmaEnable = 1u << 31,

  kKeyA = 1 << 0,
  kKeyB = 1 << 1,
  kKeySelect = 1 << 2,
  kKeyStart = 1 << 3,
  kKeyRight = 1 << 4,
  kKeyLeft = 1 << 5,
  kKeyUp = 1 << 6,
  kKeyDown = 1 << 7,
  kKeyR = 1 << 8,
  kKeyL = 1 << 9,
};

#define GBA_VRAM ((volatile uint16_t *)0x06000000)
#define GBA_OBJ_VRAM ((volatile uint16_t *)0x06010000)
#define GBA_BG_PALETTE ((volatile uint16_t *)0x05000000)
#define GBA_OBJ_PALETTE ((volatile uint16_t *)0x05000200)
#define GBA_OAM ((volatile uint16_t *)0x07000000)
#define GBA_SRAM ((volatile uint8_t *)0x0E000000)

// Code and data in IWRAM: 32-bit, no wait states. Code there is ARM (see
// CMakeLists.txt): put it in a *.iwram.c file.
#define IWRAM_DATA __attribute__((section(".iwram.data")))
// In a *.iwram.c file, a function that runs too seldom to take IWRAM: Thumb
// code in the cartridge after all.
#define ROM_CODE __attribute__((target("thumb"), section(".romtext")))
#define IWRAM_BSS __attribute__((section(".iwram_bss")))
#define EWRAM_BSS __attribute__((section(".ewram_bss")))

static inline void DmaCopy32(volatile void *dst, const void *src, uint32_t bytes) {
  REG_DMASAD(3) = (uintptr_t)src;
  REG_DMADAD(3) = (uintptr_t)dst;
  REG_DMACNT(3) = (bytes >> 2) | kDma32 | kDmaEnable;
}

ROM_CODE static inline void DmaFill32(volatile void *dst, uint32_t value, uint32_t bytes) {
  static volatile uint32_t fill;
  fill = value;
  REG_DMASAD(3) = (uintptr_t)&fill;
  REG_DMADAD(3) = (uintptr_t)dst;
  REG_DMACNT(3) = (bytes >> 2) | kDma32 | kDmaSrcFixed | kDmaEnable;
}

// Timers 2 and 3, cascaded (main.c starts them): CPU cycles.
static inline uint32_t Cycles(void) {
  uint16_t hi = REG_TMCNT_L(3), lo = REG_TMCNT_L(2);
  if (REG_TMCNT_L(3) != hi)
    hi = REG_TMCNT_L(3), lo = REG_TMCNT_L(2);
  return (uint32_t)hi << 16 | lo;
}
