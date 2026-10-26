// The item pickup message boxes that tell the SNES's buttons, told with the
// GBA's (input.c):
//   MISSILE         hold [R] (missile) & press the [B] button.
//   SUPER MISSILE   select (super missile) with [SEL].
//                   hold [R] & press the [B] button.
//   POWER BOMB      hold [R] (power bomb) & set it with [B].
//   GRAPPLING BEAM  select (grapple) with [SEL].
//                   press & hold [R] & [B].
//   X-RAY SCOPE     select (X-ray) with [SEL].
//                   press & hold [R].
//   SPEED BOOSTER   hold (arrow) to run.
// (the bomb's only needs its button, which sm_85.c draws as B). They're
// written in the message box font's words and icons, as the SNES's are: it
// has no lowercase letters, but has press, hold, button, select, with, the,
// set it, to and run, and the buttons' letters. (Its "the" ends, and its
// "button" starts, in the tiles either side of a button's letter.)
#include "ida_types.h"

enum {
  kBlank = 0x284e,
  kFirstColumn = 3,  // of the box; the last is 28
  kColumns = 26,
};

// A piece of a line: a word, a button, or an item's icon, whose top half is
// on the row above.
typedef struct Piece {
  char c;
  uint8 width;
  uint16 tiles[4], top[3];
} Piece;

// A piece without an icon's top.
#define TEXT(c, width, ...) {c, width, {__VA_ARGS__}, {0}}

static const Piece kPieces[] = {
  TEXT(' ', 1, kBlank),
  TEXT('.', 1, 0x28cb),
  TEXT('&', 1, 0x28c8),
  TEXT('p', 3, 0x28b0, 0x28b1, 0x28b2),          // press
  TEXT('h', 2, 0x28b3, 0x28b4),                  // hold
  TEXT('b', 4, 0x28d3, 0x28b5, 0x28b6, 0x28b7),  // button
  TEXT('s', 3, 0x28b9, 0x28ba, 0x28bb),          // select
  TEXT('r', 2, 0x28bc, 0x28bd),                  // run
  TEXT('w', 2, 0x28be, 0x28bf),                  // with
  TEXT('t', 3, 0x28c0, 0x28c1, 0x28d1),          // the
  TEXT('o', 1, 0x28c2),                          // to
  TEXT('i', 3, 0x28c5, 0x28c6, 0x28c7),          // set it
  TEXT('>', 2, 0x3ccc, 0x38cd),                  // an arrow
  TEXT('B', 1, 0x3ce1),
  TEXT('R', 1, 0x38f1),
  TEXT('S', 1, 0x38d0),  // Select
  {'1', 3, {0x304c, 0x304a, 0x704c}, {0x304b, 0x3049, 0x704b}},  // missile
  {'2', 2, {0x3035, 0x7035}, {0x3034, 0x7034}},  // super missile
  {'3', 2, {0x3037, 0x7037}, {0x3036, 0x7036}},  // power bomb
  {'4', 2, {0x3039, 0x7039}, {0x3038, 0x7038}},  // grapple beam
  {'5', 2, {0x303b, 0x703b}, {0x303a, 0x703a}},  // X-ray scope
};

static const Piece *FindPiece(char c) {
  for (int i = 0; i < (int)(sizeof(kPieces) / sizeof(kPieces[0])); i++)
    if (kPieces[i].c == c)
      return &kPieces[i];
  return &kPieces[0];
}

// Writes a line, centered as the SNES's are, to a row of the box, and its
// icons' tops to the row above.
static void WriteLine(uint16 *row, const char *text) {
  int width = 0;
  for (const char *c = text; *c; c++)
    width += FindPiece(*c)->width;
  int x = (32 - width) / 2;
  for (const char *c = text; *c; c++) {
    const Piece *piece = FindPiece(*c);
    for (int i = 0; i < piece->width; i++, x++) {
      row[x] = piece->tiles[i];
      if (piece->top[0])
        row[x - 32] = piece->top[i];
    }
  }
}

// For message box `index`, its lines below its title, as pieces' letters:
// for rows 2 and 3 of the box's 4.
static const struct {
  uint8 index;
  const char *lines[2];
} kMessages[] = {
  {2, {"", "h R 1 & ptBb."}},
  {3, {"s 2 w S.", "h R & ptBb."}},
  {4, {"", "h R 3 & i w B."}},
  {5, {"s 4 w S.", "p & h R & B."}},
  {6, {"s 5 w S.", "p & h R."}},
  {13, {"", "h > o r."}},
};

// Writes message box `index`'s text for the GBA over the message's tilemap
// (its 4 rows, after the box's top row), if the GBA has its own.
bool GbaWriteMessageBoxText(uint16 index, uint16 *tilemap) {
  for (int m = 0; m < (int)(sizeof(kMessages) / sizeof(kMessages[0])); m++) {
    if (kMessages[m].index != index)
      continue;
    for (int r = 1; r < 4; r++)
      for (int x = 0; x < kColumns; x++)
        tilemap[32 * r + kFirstColumn + x] = kBlank;
    for (int line = 0; line < 2; line++)
      WriteLine(tilemap + 32 * (line + 2), kMessages[m].lines[line]);
    return true;
  }
  return false;
}
