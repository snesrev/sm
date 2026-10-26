// The options menu's controller setting screen, for the GBA's buttons
// (input.c): the game's screen, its rows of actions, with the GBA's buttons
// drawn in place of the SNES's. They're fixed, so it's CONTROLS, on the
// option menu and as its title (in OPTION MODE's box), and only END does
// anything there: RESET TO DEFAULT is left out (see sm_82.c).
//   SHOT          B
//   JUMP          A
//   DASH          AUTO: Samus always runs
//   ITEM SELECT   SELECT, with R held: picks what R readies instead of missiles
//   USE ITEM      R: hold to ready it (ITEM CANCEL on the SNES)
//   ANGLE UP      L
//   ANGLE DOWN    L + down
// The buttons are drawn here as text, a character a pixel, in the menu's
// BG palette 0, their letters the menu's font's, and made into tiles, in
// tiles the menu's tileset leaves blank, when the screen is drawn. (Outside
// gameplay, 2 lines of each 8 don't show, chosen to keep text's bars, which
// the letters line up with: see render.c's SquashAnalysis.)
#include "controls_screen.h"

#include <stdbool.h>
#include <string.h>

#include "ida_types.h"
#include "snes_hw.h"
#include "variables.h"

// Colors in BG palette 0: white, the lavender of the GBA's buttons and its
// shade, dark plum and dark blue (the text's shadow).
static int Color(char c) {
  switch (c) {
  case 'W': return 1;
  case 'G': return 9;
  case 'g': return 10;
  case 'D': return 12;
  case 'B': return 13;
  default: return 0;
  }
}

typedef struct Icon {
  int width;  // in tiles; 2 high
  // A letter of the menu's font on it: its top and bottom tiles, at x (a
  // tile's, as text's are: its strokes then count as text's, kept in the
  // squash).
  uint8_t letter_top, letter_bottom, letter_x;
  const char *rows[16];
} Icon;

enum { kIconA, kIconB, kIconL, kIconR, kIconSelect, kIconDown, kIconPlus, kIcons };

static const Icon kIconArt[kIcons] = {
  [kIconA] = {3, 0x0a, 0x1a, 8, {
    "........BBBBBBBB........",
    "......BBGGGGGGGGBB......",
    ".....BGGGGGGGGGGGgB.....",
    ".....BGGGGGGGGGGGgB.....",
    "....BGGGGGGGGGGGGggB....",
    "....BGGGGGGGGGGGGggB....",
    "....BGGGGGGGGGGGGggB....",
    "....BGGGGGGGGGGGGggB....",
    "....BGGGGGGGGGGGGggB....",
    "....BGGGGGGGGGGGGggB....",
    "....BGGGGGGGGGGGGggB....",
    "....BGGGGGGGGGGGGggB....",
    ".....BGGGGGGGGGGGgB.....",
    ".....BgGGGGGGGGGggB.....",
    "......BBggggggggBB......",
    "........BBBBBBBB........",
  }},
  [kIconB] = {3, 0x0b, 0x1b, 8, {
    "........BBBBBBBB........",
    "......BBGGGGGGGGBB......",
    ".....BGGGGGGGGGGGgB.....",
    ".....BGGGGGGGGGGGgB.....",
    "....BGGGGGGGGGGGGggB....",
    "....BGGGGGGGGGGGGggB....",
    "....BGGGGGGGGGGGGggB....",
    "....BGGGGGGGGGGGGggB....",
    "....BGGGGGGGGGGGGggB....",
    "....BGGGGGGGGGGGGggB....",
    "....BGGGGGGGGGGGGggB....",
    "....BGGGGGGGGGGGGggB....",
    ".....BGGGGGGGGGGGgB.....",
    ".....BgGGGGGGGGGggB.....",
    "......BBggggggggBB......",
    "........BBBBBBBB........",
  }},
  // The shoulder buttons: tabs, rounded on the outer corner.
  [kIconL] = {3, 0x25, 0x35, 8, {
    "......BBBBBBBBBBBBBBBB..",
    "....BBGGGGGGGGGGGGGGGGB.",
    "...BGGGGGGGGGGGGGGGGGGB.",
    "..BGGGGGGGGGGGGGGGGGGgB.",
    ".BGGGGGGGGGGGGGGGGGGGgB.",
    ".BGGGGGGGGGGGGGGGGGGGgB.",
    "BGGGGGGGGGGGGGGGGGGGGgB.",
    "BGGGGGGGGGGGGGGGGGGGGgB.",
    "BGGGGGGGGGGGGGGGGGGGGgB.",
    "BGGGGGGGGGGGGGGGGGGGGgB.",
    "BGGGGGGGGGGGGGGGGGGGGgB.",
    "BGGGGGGGGGGGGGGGGGGGGgB.",
    "BGGGGGGGGGGGGGGGGGGGggB.",
    "BgGGGGGGGGGGGGGGGGGgggB.",
    "BgggggggggggggggggggggB.",
    ".BBBBBBBBBBBBBBBBBBBBB..",
  }},
  [kIconR] = {3, 0, 0, 0, {
    "..BBBBBBBBBBBBBBBB......",
    ".BGGGGGGWWWWWWGGGGBB....",
    ".BGGGGGGWWWWWWWGGGGGB...",
    ".BGGGGGGWWBBBWWBGGGGGB..",
    ".BGGGGGGWWBGGWWBGGGGGgB.",
    ".BGGGGGGWWBGGWWBGGGGGgB.",
    ".BGGGGGGWWWWWWWBGGGGGGgB",
    ".BGGGGGGWWWWWWBBGGGGGGgB",
    ".BGGGGGGWWBWWBBGGGGGGGgB",
    ".BGGGGGGWWBWWBGGGGGGGGgB",
    ".BGGGGGGWWBGWWGGGGGGGGgB",
    ".BGGGGGGWWBGWWBGGGGGGGgB",
    ".BGGGGGGWWBGGWWGGGGGGggB",
    ".BgGGGGGWWBGGWWBGGGGgggB",
    ".BggggggWWBgggWWgggggggB",
    "..BBBBBBBBBBBBBBBBBBBBB.",
  }},
  // The small oval buttons (labeled after, in the menu's font).
  [kIconSelect] = {2, 0, 0, 0, {
    "................",
    "................",
    "................",
    "....BBBBBBBB....",
    "..BBWWWWWWWWBB..",
    ".BWWGGGGGGGGGgB.",
    ".BWGGGGGGGGGGgB.",
    ".BWGGGGGGGGGGgB.",
    ".BgGGGGGGGGGggB.",
    ".BggggggggggggB.",
    "..BBggggggggBB..",
    "....BBBBBBBB....",
    "................",
    "................",
    "................",
    "................",
  }},
  // The d-pad, down held.
  [kIconDown] = {2, 0, 0, 0, {
    "......BBBB......",
    ".....BWWWWB.....",
    ".....BWGGgB.....",
    ".....BWGGgB.....",
    ".BBBBBWGGgBBBBB.",
    "BWWWWWGGGGWWWWgB",
    "BWGGGGGGGGGGGGgB",
    "BWGGGGGGGGGGGGgB",
    "BgggggGGGGgggggB",
    ".BBBBBWDDDBBBBB.",
    ".....BWDDDB.....",
    ".....BDDDDB.....",
    ".....BDDDDB.....",
    ".....BDDDDB.....",
    "......BBBB......",
    "................",
  }},
  [kIconPlus] = {1, 0, 0, 0, {
    "........",
    "........",
    "........",
    "........",
    "...WW...",
    "...WW...",
    ".WWWWWW.",
    ".WWWWWW.",
    "...WW...",
    "...WW...",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
  }},
};

// Tiles the options menu's tileset leaves blank, on all its screens (the
// icons need nearly all of them; any more would be left blank).
static const uint16_t kFreeTiles[] = {
  487, 488, 489, 490, 491, 492, 493, 494, 495, 503, 504, 505, 506, 507, 508, 509,
  510, 511, 636, 637, 684, 685, 686, 687, 768, 782, 783, 799, 846, 40, 41, 60, 61, 70,
};
enum { kFreeTileCount = sizeof(kFreeTiles) / sizeof(kFreeTiles[0]) };

// The menu's font: the top and bottom halves of each letter, as map entries.
static uint32_t Letter(char c) {
  switch (c) {
  case 'A': return 0x0a | 0x1a << 16;
  case 'C': return 0x0c | 0x1c << 16;
  case 'E': return 0x0e | 0x1e << 16;
  case 'I': return 0x22 | 0x11 << 16;
  case 'L': return 0x25 | 0x35 << 16;
  case 'M': return 0x26 | 0x36 << 16;
  case 'N': return 0x27 | 0x37 << 16;
  case 'O': return 0x00 | 0x10 << 16;
  case 'R': return 0x0d | 0x3a << 16;  // (D's top)
  case 'S': return 0x2b | 0x3b << 16;
  case 'T': return 0x2c | 0x11 << 16;
  case 'U': return 0x2d | 0x10 << 16;
  default: return 0x0f | 0x0f << 16;  // a space
  }
}

enum { kBlankTile = 0x0f, kRowWords = 32 };

// The first map row of each action's, and the column of its button; the
// title's row and columns (centered in them), and RESET TO DEFAULT's row.
static const uint8_t kActionRows[7] = {5, 8, 11, 14, 17, 20, 23};
enum { kTitleRow = 1, kTitleColumn = 5, kTitleColumns = 23, kResetRow = 29 };
// The option menu's row of CONTROLLER SETTING MODE, and its columns.
enum { kOptionRow = 17, kOptionColumn = 4, kOptionColumns = 23 };
enum { kButtonColumn = 23, kButtonColumns = 8, kLabelColumn = 6, kLabelColumns = 11 };

static uint16_t *Tilemap(int row, int column) {
  return &ram3000.pause_menu_map_tilemap[row * kRowWords + column];
}

static void Text(int row, int column, const char *text, int width) {
  for (int i = 0; i < width; i++) {
    uint32_t letter = Letter(*text ? *text++ : ' ');
    Tilemap(row, column + i)[0] = letter & 0xffff;
    Tilemap(row + 1, column + i)[0] = letter >> 16;
  }
}

// A pixel (its color) of tile |tile| of the menu's tileset, at |base|.
static int TilePixel(uint16_t base, int tile, int x, int y) {
  const uint16_t *t = &g_vram[(base + tile * 16) & 0x7fff];
  int bit = 7 - x;
  return (t[y] >> bit & 1) | (t[y] >> (bit + 8) & 1) << 1 | (t[y + 8] >> bit & 1) << 2 |
         (t[y + 8] >> (bit + 8) & 1) << 3;
}

// A pixel (its color) of an icon: its letter's, or else its own.
static int IconPixel(const Icon *icon, uint16_t base, int x, int y) {
  int lx = x - icon->letter_x;
  if (icon->letter_top && lx >= 0 && lx < 8) {
    int c = TilePixel(base, y < 8 ? icon->letter_top : icon->letter_bottom, lx, y & 7);
    if (c)
      return c;
  }
  return Color(icon->rows[y][x]);
}

// Makes the icons' tiles, in VRAM (each different one once), and their map
// entries: entries[icon][t] for its tiles, the top row's then the bottom's.
// (No copies kept: the stack is small.)
static void MakeIcons(uint16_t entries[kIcons][6]) {
  uint16_t base = (reg_BG12NBA & 15) << 12;
  int count = 0;
  for (int i = 0; i < kIcons; i++) {
    const Icon *icon = &kIconArt[i];
    for (int t = 0; t < icon->width * 2; t++) {
      int tx = t % icon->width * 8, ty = t / icon->width * 8;
      uint16_t tile[16] = {0};
      bool blank = true;
      for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
          int c = IconPixel(icon, base, tx + x, ty + y), bit = 7 - x;
          blank &= !c;
          tile[y] |= (c & 1) << bit | (c >> 1 & 1) << (bit + 8);
          tile[y + 8] |= (c >> 2 & 1) << bit | (c >> 3 & 1) << (bit + 8);
        }
      }
      int k = 0;
      while (k < count && memcmp(&g_vram[base + kFreeTiles[k] * 16], tile, sizeof(tile)))
        k++;
      if (blank || k == kFreeTileCount) {
        entries[i][t] = kBlankTile;
        continue;
      }
      if (k == count++)
        SnesHw_WriteVram(base + kFreeTiles[k] * 16, tile, 16);
      entries[i][t] = kFreeTiles[k];
    }
  }
}

static void Button(int row, int column, const uint16_t *entries, int width) {
  for (int t = 0; t < width * 2; t++)
    Tilemap(row + t / width, column + t % width)[0] = entries[t];
}

void GbaControllerSettingsScreen(void) {
  uint16_t entries[kIcons][6];
  MakeIcons(entries);
  for (int i = 0; i < 7; i++) {
    for (int c = 0; c < kButtonColumns; c++) {
      Tilemap(kActionRows[i], kButtonColumn + c)[0] = kBlankTile;
      Tilemap(kActionRows[i] + 1, kButtonColumn + c)[0] = kBlankTile;
    }
  }
  const uint8_t *rows = kActionRows;
  Button(rows[0], kButtonColumn, entries[kIconB], 3);
  Button(rows[1], kButtonColumn, entries[kIconA], 3);
  Text(rows[2], kButtonColumn, "AUTO", 4);
  Button(rows[3], kButtonColumn - 2, entries[kIconSelect], 2);
  Text(rows[3], kButtonColumn, "SELECT", 6);
  Button(rows[4], kButtonColumn, entries[kIconR], 3);
  Button(rows[5], kButtonColumn, entries[kIconL], 3);
  Button(rows[6], kButtonColumn, entries[kIconL], 3);
  Button(rows[6], kButtonColumn + 3, entries[kIconPlus], 1);
  Button(rows[6], kButtonColumn + 4, entries[kIconDown], 2);
  Text(kResetRow, 0, "", kRowWords);
  // ITEM CANCEL is R's, holding it: the Japanese text's left as it is.
  if (!japanese_text_flag) {
    Text(rows[4], kLabelColumn, "USE ITEM", kLabelColumns);
    Text(kTitleRow, kTitleColumn, "", kTitleColumns);
    Text(kTitleRow, kTitleColumn + (kTitleColumns - 8 + 1) / 2, "CONTROLS", 8);
  }
}

void GbaOptionsMenuScreen(void) {
  Text(kOptionRow, kOptionColumn, "CONTROLS", kOptionColumns);
}
