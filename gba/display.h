// A frame's GBA display registers, line by line, as render.iwram.c builds
// them and irq.iwram.c sets them while the GBA draws the frame.
//
// Registers that change from line to line (the backgrounds' scrolling, or
// in frames with Mode 7, BG2's transformation; in frames with windows, the
// scrolling and the windows) are a stream that HBlank DMA copies in before
// each line. The others (which layers show, and their
// settings) only change at a few lines, such as the HUD's last: they're in
// segments, which the VCount interrupt sets in the HBlank before their
// first line.
#pragma once

#include <stdbool.h>
#include <stdint.h>

enum {
  kGbaLines = 160,
};

// SameSegment (render.iwram.c) compares all after first_line.
typedef struct DisplaySegment {
  uint16_t first_line;
  uint16_t dispcnt;
  uint16_t bgcnt[4];
  // Color math: BLDCNT, BLDALPHA, BLDY.
  uint16_t bld[3];
  // With windows (DISPCNT's bits 13 and 14): WININ, WINOUT.
  uint16_t win[2];
  // The scroll registers, in frames whose stream is BG2's transformation.
  uint16_t ofs[8];
} DisplaySegment;

enum {
  // In frames with windows: BG0HOFS-BG3VOFS, BG2's and BG3's
  // transformations (unused), WIN0H and WIN1H.
  kWindowStreamWords = 13,
};

typedef struct DisplayFrame {
  // |stream_words| words per line, and one more line: DMA copies in an
  // entry at the end of the last line. Four words, in IWRAM, as DMA reads
  // it every line; or in frames with windows, kWindowStreamWords, in EWRAM.
  const uint32_t *stream;
  int stream_words;
  // 0x04000010 (BG0HOFS-BG3VOFS, or more) or 0x04000020 (BG2PA-BG2Y).
  uint32_t stream_dest;
  int num_segments;
  const DisplaySegment *segments;
  // Colors of the GBA's palette to set from a segment on (|patch_segment|,
  // or with none, -1), and back at VBlank: BG3's for the gameplay area that
  // differ from the HUD's (see render.iwram.c's subtract_color).
  int patch_segment, num_patches;
  uint8_t patch_index[24];
  uint16_t patch_color[24], restore_color[24];
} DisplayFrame;

// The frame on show, or null: irq.iwram.c sets it up at every VBlank.
extern const DisplayFrame *volatile g_display;
// Whether render.c presented a frame since the last VBlank (main.c sets
// it).
extern volatile bool g_frame_presented;

// Sets up g_display now, in VBlank, with interrupts disabled.
void Display_Start(void);
// Or, in the picture, from the next line on: for a present that ran on
// past VBlank.
void Display_Resume(void);
