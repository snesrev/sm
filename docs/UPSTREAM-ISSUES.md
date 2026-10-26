# Issues to report to snesrev/sm

Problems in the decompilation itself (`src/`), found while porting it to
the GBA, that affect the upstream PC build too or would affect any other
port. Each has the fix this branch made, to offer upstream (issues 8 to 11,
found by running the C beside the original, say where they have a fix checked
in a copy instead), linked to its commit.

Issues 1 to 5 are reported upstream, each linked under its heading. 6 to 11
aren't yet.

## 1. Overlapping `memcpy` when a room loads

**Reported:** [snesrev/sm#51](https://github.com/snesrev/sm/issues/51)

**Where:** `src/sm_82.c`, `LoadLevelDataAndOtherThings` and
`LoadLevelScrollAndCre`.

**What:** each copies the room's custom background and BTS (block types)
out of its decompressed level data to just after it:

```c
memcpy(custom_background, (uint8 *)level_data + size + (size >> 1), size);
memcpy(BTS, (uint8 *)level_data + size, size >> 1);
```

In large rooms the source and destination overlap. That's undefined
behavior for `memcpy`. glibc happens to copy such buffers correctly, so the
PC build works. A plain front-to-back `memcpy` (as on the GBA) corrupts the
data: on the landing site, Samus couldn't climb the hills, because their
slopes' BTS came out wrong.

**Fix:** `memmove` for these four calls (commit [1078097](https://github.com/nickdesaulniers/sm/commit/1078097) on this branch).
A Zebes playthrough on the PC build with an overlap-checking `memcpy`
caught the overlaps (in `LoadLevelDataAndOtherThings`; the other function
copies the same way) and found no other overlapping copies.

## 2. Functions used in only one file aren't `static`

**Reported:** [snesrev/sm#52](https://github.com/snesrev/sm/issues/52)

**Where:** throughout `src/`, with prototypes in `src/funcs.h`.

**What:** 2111 functions are only called (or named, as by `FUNC16`) in
the file that defines them, but are declared in `funcs.h` and defined
without `static`. At `-O3` many are inlined at every call site, yet the
compiler must still emit a standalone copy of each, as it has external
linkage: about 240 KB of code nothing calls. Being non-static also hides
from the compiler that it sees every caller.

**Fix:** make them `static`, and move their prototypes from `funcs.h` into
their files (commit [b74d1c0](https://github.com/nickdesaulniers/sm/commit/b74d1c0)). The PC build (gcc, `-Werror`) still builds,
and the C still matches the original frame by frame.

## 3. A leftover debugging `printf`

**Reported:** [snesrev/sm#53](https://github.com/snesrev/sm/issues/53)

**Where:** `src/sm_90.c`, `Samus_HandleAnimDelay`.

**What:** `printf("v1=%x\n", v1);` runs whenever Samus's animation reaches
a delay marker, which happens every few frames during play, and prints to
stdout.

**Fix:** remove it (commit [5691898](https://github.com/nickdesaulniers/sm/commit/5691898)).

## 4. The menu tilemap is declared a quarter of its size

**Reported:** [snesrev/sm#54](https://github.com/snesrev/sm/issues/54)

**Where:** `src/ida_types.h`, `Ram3000_Menu`; used in `src/sm_81.c`.

**What:** `menu_tilemap` ($7E:3600) is declared `uint8 menu_tilemap[512]`,
but every use treats it as 2048 bytes: the menus DMA 2048 bytes from it,
`ClearMenuTilemap`'s loop fills 1024 words of it, and
`FileSelectMenu_1_LoadFileSelectMenuBG2` fills it through another member
of the union, `ram3000.pause_menu_map_tilemap[i + 768]` for `i` up to 1023,
past that array's 1024 entries. It's all within `g_ram`, so nothing goes
wrong, but UBSan reports the index out of bounds.

**Fix:** declare it `uint8 menu_tilemap[2048]`, and fill it as itself in
`FileSelectMenu_1_LoadFileSelectMenuBG2` (commit [f6c58ee](https://github.com/nickdesaulniers/sm/commit/f6c58ee)). The C still
matches the original frame by frame.

## 5. Tables indexed by a block's BTS, past their end

**Reported:** [snesrev/sm#55](https://github.com/snesrev/sm/issues/55)
(the spike and grapple functions; `BlockInsideReact_SpecialAir`, found
later, isn't in it yet)

**Where:** `src/sm_94.c`: `BlockInsideReact_SpikeAir`,
`BlockColl_Horiz_SpikeBlock` and `BlockColl_Vert_SpikeBlock`,
`BlockReactGrapple_GrappleBlock` and `BlockReactGrapple_SpikeBlock`, and
`BlockInsideReact_SpecialAir`.

**What:** each indexes a table of handlers (or PLMs) by the block's BTS
byte, which can be anything: the C's tables have 16 entries (the grapple
block's 4; the special air table's, at `$94:9966`, 80), and the ROM's are
as short (the spike air table at `$94:98CC` is 8 entries: the next
function starts 16 bytes on; the special air table is followed by
data), so a larger BTS reads past the table, where the SNES would take
the next code's bytes for addresses. In C it's undefined; on the GBA, with
Samus held inside a block in a Maridia room by a test that walks every room,
it called a null pointer and reset (which pointer depended on the build).

**Fix:** a BTS past the table does nothing (commit [be704b4](https://github.com/nickdesaulniers/sm/commit/be704b4); and for
`BlockInsideReact_SpecialAir`, whose BTS 80 to 127 read the data after its
table, [32ca124](https://github.com/nickdesaulniers/sm/commit/32ca124), the commit that adds the developer tools). That one showed up
when adding a few statics moved the others: with Samus outside the room (the
tour's door transitions run her handler with her at x = -185 for a moment;
the developers' debug station Norfair 21 starts her out of bounds), her
block index is far past the room's, and `BTS[cur_block_index]` reads past
the end of `g_ram`, into whatever follows. The tour reset in Maridia's
room D2D9 and, once the new statics went last, its pictures didn't move.
Keeping Samus's block inside the room's would be the root fix; issue 7 is
the one made.

## 6. The debug soft reset runs a state that isn't

**Where:** `src/sm_80.c`, `ReadJoypadInputs`, and `src/sm_82.c`,
`RunOneFrameOfGameInner`.

**What:** with `enable_debug` set (the developers' switch, which the load-game
map's station list and more depend on), holding Start, Select, L and R calls
`SoftReset` from `ReadJoypadInputs`. That sets `game_state` to 0xffff, as the
other callers do, but they're inside a state's function: this one is at the
top of the frame, which then goes on to `kGameStateFuncs[game_state]`, 65535
entries past a table of 45. On the PC build that's undefined; on the GBA it
jumped into the weeds. (The SNES's reset vector runs at once.)

**Fix:** `RunOneFrameOfGameInner` returns after `ReadJoypadInputs` if
`game_state` is 0xffff, and the frame ends as the other soft resets do
(commit [32ca124](https://github.com/nickdesaulniers/sm/commit/32ca124), the one that adds the developer tools).

## 7. A block index of -1 reads past `level_data` and `BTS`

**Where:** `src/sm_94.c` `CalculateBlockAt`, and every read of
`level_data[cur_block_index]` and `BTS[cur_block_index]` (94 of them, in
`src/sm_94.c`, `src/sm_a0.c`, `src/sm_86.c` and `src/sm_a3.c`: Samus's,
the enemies' and the enemy projectiles' block collisions, slopes).

**What:** `CalculateBlockAt` sets `cur_block_index` to -1 for a point
outside the room (Samus's, for a moment in a door transition, or spawned
out of bounds), and the callers go on to read the block there. The SNES
reads with a 16-bit X (`LDA $7F0002,X`, `LDA $7F6402,X`): `level_data`'s
read wraps to `$80:0000`, the mirror of `$7E:0000`, and `BTS`'s is
`$80:6401`, the open bus. In C, `level_data[0xffff]` and `BTS[0xffff]` are
128 KB and 64 KB past the start of the arrays, past the end of `g_ram`:
undefined. On the GBA they read whatever follows `g_ram` (the copy of
VRAM), so the block's type and BTS were garbage: a BTS of 0x80 or more in
`BlockInsideReact_SpecialAir` spawns a PLM from a table by it, which is how
the tour reset in room D2D9 (issue 5) and could leave a door transition on
a black screen.

**Fix:** `LevelDataAt` and `BtsAt` (`src/variables.h`, commit
[f4ed537](https://github.com/nickdesaulniers/sm/commit/f4ed537)) read as the SNES does, `$7E:0000` and the open bus's `$7F` (the instruction's bank byte, the
last thing on the bus, as emulators have it), and the reads go through
them; in the room, they are the arrays. Found by smrun's MEMCHECK
(`tools/memcheck_ranges.py`), which logs every load and store the game's
code makes outside the game's RAM: with this, none in the room tour or the
239 scenarios.


## 8. A bomb on a room's top row: the neighbours' offsets lose the carry

**Where:** `src/sm_94.c`, `BlockColl_GetBlockIndexAbove`, `...Right`,
`...Below` and `BlockBombedReact` (the bomb's explosion, in
`BlockColl_BombExplosion`).

**What:** the explosion reacts to the bomb's block and the four around it,
by their byte offsets into `level_data`, which the ROM works out with
`SEC; SBC w; SBC w` (above) and `SEC; ADC w; ADC w; INC` (right, below):
the first subtraction's borrow, or addition's carry, is the second's. The
C does `k - w - w` and `k + 2w + 2`. They differ when the first step wraps
the 16 bits, which is for a bomb on a room's top row (the offset above it is
negative): then the ROM's offset to the right is one more (the C's `0x20`
is the ROM's `0x21`, for block 15 of a room 16 blocks wide), and so is odd.
`BlockBombedReact` reads the block there with `LDA $7F0002,X` (a word from
the high byte of one block and the low byte of the next, here `0xff83`, a
bomb block, which spawns the respawning bomb block PLM `$CFFC`), where the C
halves the offset to a block index and reads the aligned word at `0x20` (a
solid block), and does nothing. Above, with `k < w`, the ROM's offset is one
less than the C's.

hostrun (the C beside the original, over a few hundred thousand frames of a
bot playing from random doors, with every item) found it: a RAM mismatch
(`$1C24`-`$1D6B`, the PLMs', and the pictures') at each bomb on a top row
(room $99BD, $D1A3). It is the same on `main`.

**Fix:** not made here; checked in a copy, which then had no mismatch in
the runs that had 13 and 2:

```c
static uint16 BlockColl_GetBlockIndexAbove(uint16 k) {  // 0x949D34
  return k - room_width_in_blocks - room_width_in_blocks - (k < room_width_in_blocks);
}

// SEC; ADC w; ADC w; INC
static uint16 BlockColl_AddWidthTwiceInc(uint16 k) {
  uint32 t = (uint32)k + room_width_in_blocks + 1;
  return (uint16)(t + room_width_in_blocks + (t >> 16) + 1);
}
// (Right and Below return BlockColl_AddWidthTwiceInc(k).)

// LDA $7F0002,X, X odd: the word's across two blocks.
static uint16 LevelDataAtByte(uint16 x) {
  uint32 a = 0x10002 + x;
  return g_ram[a & 0x1ffff] | g_ram[(a + 1) & 0x1ffff] << 8;
}

static void BlockBombedReact(CollInfo *ci, uint16 v0) {
  cur_block_index = v0 >> 1;
  uint8 rv = kBlockBombedReact[(LevelDataAtByte(v0) & 0xF000) >> 12](ci);
  while (rv & 0x80)
    rv = kBlockBombedReact[(LevelDataAt(cur_block_index) & 0xF000) >> 12](ci);
}
```

## 9. The X-ray scope's tables are written past their end

**Where:** `src/sm_91.c`, `XrayHdmaFunc_*` and `XrayHdmaOnScreen_*`, and
`XrayFillUpRamped` and `XrayFillDownRamped`.

**What:** `int p = 2 * ctx->r24;`, where `r24` is the beam's row on the
screen, which is 0xff00 or more when the beam starts above the screen. The ROM's index is 16 bits
(`X`, the table's address plus `X` stays in `$7E:9800`-`$7F:97FF`); the C's
`p` is 0x1ff96 for 0xffcb, 128 KB past the table. In the PC build that
overwrote `g_rom` (the ROM's pointer, 0xf8 bytes past the end of `g_ram`)
and the frame after crashed in `HdmaobjInstructionHandler` (Crocomire's room
$A98D, Samus at y 219, with the scope on); AddressSanitizer names
`XrayHdmaOnScreen_BeamAimedR`. On
the GBA it would write over whatever follows `g_ram` in EWRAM. The ramped
fills' `pos` is an `int` too, where the ROM's `BPL` and `CPX #$01CC` take
it as 16 bits, signed and unsigned.

`main` crashes the same way.

**Fix:** made here (`src/sm_91.c`, commit [712ad25](https://github.com/nickdesaulniers/sm/commit/712ad25)): `uint16` for `p`, `t`, `v0`
(and `(uint16)(p - 2) >> 1` where it's an index) and for the fills' `pos`,
with `(int16)(pos -= 2) >= 0` in the up fills, keep every write in `g_ram`,
and the run goes on (an AddressSanitizer test of every angle, with the
beam's row above, on and below the screen, crashes before and passes after). What the ROM writes there, from `$7F:9798` on, still
differs from the C's: the note in `VerifySnapshotsEq` (src/sm_cpu_infra.c)
that "XrayHdmaFunc has some bug that i couldn't fix in asm" is about the
same functions.

## 10. Periodic damage of 256 or more in a frame

**Where:** `src/sm_90.c`, `Samus_HandlePeriodicDamage`.

**What:** with the Varia or Gravity suit the ROM halves or quarters the
frame's damage with `LDA $0A4F`, a 16-bit load of the subdamage's high byte
and the damage's low byte: the damage's high byte is dropped. The C uses all
of it (`__PAIR32__(samus_periodic_damage, samus_periodic_subdamage) >> 2`).
Same below 256 a frame, which the game never reaches by itself; hostrun found
a frame of 0x018A (health 784 to 749 in the ROM, to 685 in the C) with
Samus teleported into a room by the bot's door, twice. Unchanged from `main`.

**Fix:** not made here (and not worth it unless the game's damage can reach
it): `t &= 0xffffff;` before the shifts, the 24 bits the ROM's load covers.

## 11. Kraid's mouth, shot by several things at once

**Where:** `src/sm_a7.c`, `Kraid_Shot_Body`.

**What:** with two super missiles and a missile's explosion at his mouth in
the same frame, the ROM (as `sm_cpu_infra.c` patches it, so that
`NormalEnemyShotAiSkipDeathAnim_CurEnemy` keeps R18-R22) spawns the dust
cloud explosion projectile for each shot, three in a frame; the C stops
after the first hit (`v6 = 0;`, "force getting out of the loop") and spawns
two. The OAM differs by one sprite from then on in that frame. (Without the
`v6 = 0;` the mismatch is the same, so the loop isn't the whole of it.)
hostrun's bot fights Kraid with it in a few frames of every 40,000, the same
frames on `main`.

**Fix:** none yet; only reported.
