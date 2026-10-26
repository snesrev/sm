# Porting a SNES game to the GBA: a guide for agents

This guide is for an agent (or a person) setting out to port a SNES game to
the Game Boy Advance as this repository ported Super Metroid. It records the
approach that worked, how the hardware maps across, the tools that made the
work checkable, and the pitfalls that cost the most time. File paths refer
to this repository. Read the code alongside this guide: the comments in
`gba/` explain the details.

## 1. Start from C, not from an emulator

Emulating a SNES on a GBA is far too slow for a demanding game. The GBA has
about 280,000 CPU cycles per frame, and the SNES's CPU, graphics chip and
sound CPU all need emulating at once. This port works because a
**decompilation** exists ([snesrev/sm](https://github.com/snesrev/sm)): the
game's code as C, checked frame by frame against the original. Look for one
first (snesrev has others, such as Zelda 3). Without one, budget for writing
it, which is a far larger project than the port.

Keep the game's logic untouched, compile it for the ARM7TDMI, and replace
only the platform layer: video, sound, input, saves and timing. When the
game's own code has a real bug that the PC's C library hides, fix the game
source (see §7), and keep the fix to the smallest change.

A thin compatibility layer is fine to start with. Here `gba/snes_hw.c` keeps
the game's `WriteReg`/`ReadReg` calls working by modelling VRAM, CGRAM, OAM,
DMA and HDMA. Make it fast later; don't rewrite the game to avoid it.

## 2. The toolchain

- **clang and lld only**, `--target=armv4t-none-eabi -mcpu=arm7tdmi`. See
  `cmake/gba-toolchain.cmake`. No GCC or devkitARM is needed.
- **No sysroot.** You write the startup code (`gba/crt0.S`), the linker
  script (`gba/gba.ld`), the cartridge header (`gba/header.c`) and a
  minimal C library (`gba/libc/`).
- **compiler-rt from source** (`cmake/compiler-rt.cmake`), for division
  and 64-bit operations. It is built as Thumb, because ARMv4T's `pop {pc}`
  doesn't switch between ARM and Thumb. **ARM-mode code must never call
  these helpers**: keep division out of `*.iwram.c`, and check with
  `llvm-nm` when in doubt.
- **Treat every compiler-rt call as expensive**, in Thumb too. The ARM7TDMI
  has no divide instruction, so `/` and `%` are a software loop (plus a
  `__clzsi2` call). As the Linux kernel advises, rethink the algorithm
  instead: shifts for powers of two, stepping or accumulating instead of
  dividing, a reciprocal multiply (`a / b` as the top word of `a * ceil(2^32
  / b)`, exact for 16-bit `a` and 8-bit `b`: `gba/rtl.c`'s SNES divider),
  or a table worked out at build time (`tools/gen_fade_table.py`). A divide
  by 3 in a hot loop once cost a million cycles every two seconds. After
  each build, check the disassembly of hot functions for `bl __udivsi3`,
  `__aeabi_*` or `__clzsi2`.
- **Thumb by default, ARM for hot code.** Everything is `-mthumb` except
  `*.iwram.c`, which is `-marm` and linked into IWRAM. ARM runs well from
  the 32-bit, zero-wait-state IWRAM, while Thumb suits the 16-bit cartridge
  bus. `ROM_CODE` (`gba/gba_hw.h`) moves cold functions in an IWRAM file
  back to ROM as Thumb.
- **Never ship the ROM's data.** The build makes `sm-engine.gba` with no
  game data, and `tools/make_cartridge.py` appends the player's own ROM
  (checked by sha1) along with preprocessed assets. CI builds and releases
  the engine only.

## 3. Memory: plan IWRAM like a budget

| Region | Size   | Speed            | Use here |
|--------|--------|------------------|----------|
| IWRAM  | 32 KB  | 32-bit, 0 wait   | Hot ARM code, the SNES's registers, DMA state, a 2.5 KB stack |
| EWRAM  | 256 KB | 16-bit, 2 wait   | The game's 128 KB of RAM, SNES VRAM (64 KB), CGRAM, OAM |
| VRAM   | 96 KB  | 16-bit           | GBA tiles and maps, converted |
| ROM    | 32 MB  | 16-bit, 3/1 wait | Code (Thumb), the game's data, preprocessed assets |

IWRAM always runs out first. The linker script fails the build when IWRAM
code and data run into the stack (`IWRAM overflows into the stack`).
Measure with `llvm-nm sm.elf | grep __iwram_bss_end`, and move once-a-frame
code to `ROM_CODE` to make room.

Where the SNES has memory the GBA can't spare, find out why the game needs
it. The SNES's 64 KB of sound RAM only ever holds data uploaded from the
ROM, so the sound driver reads the ROM directly (`gba/audio.c`).

## 4. Mapping the hardware

### Alignment

The game casts byte pointers to `uint16*` and `uint32*` at any address. The
ARM7TDMI rotates misaligned loads, so they return garbage without
crashing. `src/types.h` makes the game's integer types 1-byte aligned on
the GBA, so the compiler emits byte loads. Your own code that reads ROM
data must do the same, or read bytes explicitly, as `gba/tour.c` does:
SNES data structures sit at odd addresses.

The same trap waits in the port's own code, for **stores**. Casting a
`uint16_t` array to `uint32_t *` and storing through it compiles to `str`,
and if the array isn't 4-byte aligned (a member at offset 22 of a struct of
halfwords, say) the ARM7TDMI doesn't fault: it stores at the word below, two
bytes early. In Ceres's Mode 7 rooms that scrolled the HUD 264 lines down.
Store by halfwords, or give the struct `__attribute__((aligned(4)))` when
you compare or copy it by words.

### Backgrounds

- SNES Mode 1 maps to GBA Mode 0. One 4bpp SNES layer is split into two
  GBA layers by the tile priority bit, so that sprites can sit between its
  halves. The other 4bpp layer takes a third GBA layer, and the 2bpp layer
  is widened to 4bpp on the fourth (`gba/render.iwram.c`'s header comment
  has the full mapping).
- Pick which layer to split per frame: some rooms put their scenery in the
  second layer's priority tiles. Count priority tiles while converting maps.
- **Equal priorities:** on the GBA, the lower-numbered background wins a
  tie. Any time you raise a layer to priority 0, check what else is there.
- **The 2bpp layer's priority bit.** With BGMODE's bit 3, SNES BG3's
  priority tiles go in front of everything and the rest behind everything,
  and there's no GBA background left to split it. Decide per part of the
  screen, by the map rows it shows: the HUD's rows have priority tiles, but
  a room's BG3 below it in the same map may be an opaque black fill meant
  to be behind (Phantoon's room, the pause screen). Deciding per map put
  that fill in front of everything.
- SNES Mode 7 maps to GBA Mode 1's affine BG2, with a tiled layer kept for
  the HUD in the same frame.
- Convert tiles and maps incrementally, from a dirty bitmap of SNES VRAM
  in 16-entry chunks. Full reconversion is far too slow. Mark a chunk dirty
  only when its contents change: games re-upload sprite tiles every frame,
  mostly unchanged. And keep changes made on frames that show no tiled
  layer (Mode 7, a door's blank frames) for the next frame that does.
- 64x64 maps don't fit the screenblocks you'll have left. Convert the half
  on screen.
- **The SNES's first visible line shows background row VOFS + 1.** Add 1
  to every vertical scroll, or per-line scrolling effects come out a line
  off.

### HDMA and per-line effects

Run HDMA once per frame into a table of register states per line, recording
only real changes. Then:

- Registers that change every line, such as scrolling, Mode 7 parameters
  and window edges, go in a stream that GBA HBlank DMA copies in. Write
  line 0's entry directly, then point DMA at entry 1. HBlank DMA after line
  *n* affects line *n* + 1.
- Registers that change at a few lines, such as the HUD's bottom edge, go
  in "segments" set by the VCount interrupt.
- Keep per-line scrolling apart from the other registers' states.
  Wavy-water and heat-shimmer effects write a scroll register on every
  line, and a full register state per line costs most of a frame.
- Don't emulate HDMA tables that only scroll: read each table once and
  cache what it writes on which lines and where in RAM the values are, then
  fill the lines in one pass each frame (`gba/snes_hw.iwram.c`'s `Baked`).
  Games slide such tables by an entry or two a frame while the camera
  moves, so a cache keyed on the start address misses every frame. Slide
  the cached lines instead and bake only the new ones. Check any such
  shortcut against the line-by-line walk over every room.
- **Two channels, one register.** The later channel wins a line, but a table
  that ends (the HUD's BG3 scroll stops after line 31) stops writing, and the
  earlier channel's value shows again from the next line. Folding a channel's
  few values into "patches" by letting the later one hold forever hid Norfair's
  lava and Lower Norfair's acid (the lava's BG3 scroll channel is before the
  HUD's, and both write BG3's VOFS), and neither the tour nor `snesref` saw it:
  the reference draws from the same captured lines. To check a capture, read
  the per-line registers out of a dump, or move the lava next to Samus
  (`scenarios.c`'s `~N`) and look.

### Color math and windows

The SNES adds or subtracts a whole sub screen. The GBA blends one layer with
the layer directly beneath it (BLDCNT, BLDALPHA, BLDY). The mappings that
work:

- **Adding the sub screen:** draw its layers on top and blend them with the
  layer below. The sum is the same, because addition is commutative.
- **Main screen = the effect layer only, room on the sub screen:** draw the
  room normally and the effect layer on top, blended.
- **Effect added to some main layers only:** move the effect layer between
  them in priority, shifting the others and the sprites.
- **Subtracting:** the GBA can't. Three approximations, picked every few frames by
  their error at a few dozen sampled pixels (main and sub screen, per
  channel, clamped at 0 as the SNES does):
  - darken under the sub layer: EVA = 0, EVB < 16;
  - subtract the sub screen's average colour in the palette;
  - for an effect layer covering the screen, subtract a colour S in the
    palette, and draw the layer on top, with each colour c replaced by
    S − c, blended additively. This is exact wherever the main colour is at
    least S.

  Stagger the sample points so they don't all fall on the same row of
  their tiles. If the effect layer shares palettes with the HUD, switch its
  colours in mid-frame.
- **The fixed color:** fold it into the palette. In a window where the
  sub screen's layers are hidden, only the whole screen's brightening
  (BLDY) is left, and white is no color: a power bomb's blast, yellow on the
  SNES, came out pale grey. Its room's BG3 is the sub screen, solid black
  but for the blast's window; so BG3 itself is the fixed color: its colors
  patched to it, shown (and blended in) only where the window hides it on
  the SNES, and not elsewhere. (If BG3 is clear, the fixed color is all over
  the screen, and the palette has it.) Look at what the layer is before
  trusting that: a BG3 with anything in it keeps the brightening.
  (Water's and fog's rooms are that: on the lines the blast's window is on,
  the brightening and no BG3; on the others, BG3 added whole, so the
  tint stays there.)
- **Windows:** SNES window 1 → WIN0, window 2 → WIN1, with their edges
  in the HBlank stream.

### Sprites

Map SNES OAM to GBA OAM every frame that OAM changes. SNES sprite priorities
map to GBA priorities relative to the background layout. Keep a copy of
each sprite tile row so that rows that wrap around still show correctly.

The two machines stack sprites differently. The SNES draws one sprite over
another by their places in OAM alone; the GBA by priority first, then by
place. A boss whose sprites come after the player's in OAM but have a
higher priority against the backgrounds (Ridley's body here) is drawn over
the player on the GBA and under on the SNES. The port notes which sprites
are Samus's as the game draws them, and gives the sprites after them that
overlap her box her priority.

### Screen size

The GBA's screen is 240x160 and the SNES's is 256x224. Crop 1:1 during
gameplay, following the player, with the HUD pinned. Follow the player's
feet, with some slack, or crouching moves the screen. On menus and text
screens, squash by dropping two rows of each tile row: the lines that show
rows 0 and 4 of the text layer's tiles (by each line's scrolling, and the
VOFS + 1 rule). Merge each dropped row into its neighbor as the tiles are
converted, keeping the brighter pixel, so letters keep their bars. That is
not enough for glyphs whose shape is in the rows it moves (a C's lower
terminal in row 4 closes it into an O, a 3's middle into a 2): redraw the
font tiles for the squash. At cartridge-build time, from the player's ROM,
delete the two rows of each font tile that cost its glyphs the least, the
same two for every glyph of a font, and put the other six where the squash
shows them (tools/squash_font.py). The cartridge holds the redrawn tiles
under a hash of the original's bytes, and the converter, on squashed screens
only, uses the redrawn tile when a tile matches. Leave
8-line sprites (mostly text) unscaled. When the GBA flips a scaled sprite,
it flips about the middle and takes columns `width..1`, so store flipped
copies of the tiles and use an unflipped matrix. Switch between crop and
squash only while the screen is dark.

### Sound

- If the game's sound driver is decompiled, run it on the main CPU.
- Replace the DSP with a mixer: ARM assembly in IWRAM, at a modest rate
  (here 10512 Hz), with stereo through DMA sound A and B.
- Decode BRR samples to PCM at cartridge-build time.
- Run the mixer from a timer interrupt after VBlank, not from the game
  loop, so a slow game frame doesn't cascade into slow sound frames.
- Keep a voice's volume times its envelope in 8 bits or more. In 5 bits,
  quiet voices fade in coarse steps and lose a third of their loudness.
- Interpolate samples played well below the mixer's rate. Held samples
  make a staircase that buzzes in the 1-5 kHz range, where the SNES (which
  interpolates) has next to nothing.
- Step each envelope state at its own rate, even within a mixing chunk. A
  note's attack running on at its rate through decay and sustain makes
  notes fade too soon.
- Advance silent voices (envelope or volume at 0) without mixing them:
  they were 28% of the voices mixed here.

### Input

The GBA has no X or Y. In gameplay, design the controls the way a GBA game
in the same genre would (here *Metroid: Zero Mission*). Translate them into
the SNES joypad bits the game reads (`gba/input.c`). Keep the SNES buttons'
own names in menus.

### The game's own debug code

A decompilation keeps the developers' leftovers, and they're the cheapest
developer tools a port can have: grep `src/` for the debug switches
(`enable_debug`, `debug_flag`, the `debug_*` variables, `joypad2_*`) before
writing any. In Super Metroid, `enable_debug` (RAM $7E05D1, cleared at
reset, set by nothing: the developers poked it in) turns on:

- starting a file goes to the load-game map (state 5), new files too; the
  map's area select steps with Select and the directions, and the room map
  steps with controller 2's Select through every load station of the area
  the file has used, then the **debug stations** (indexes 16 and up of
  `kLoadStationLists`, with their own map icons): 20 spawn points, in rooms
  next to the bosses' and in the Golden Torizo's;
- a soft reset on Start + Select + L + R, and a pad-1 filter that moves
  Select + L or R into debug variables (no use here);
- debug station icons on the pause map;
- invincibility (`debug_invincibility` of 7 or more: no knockback, health
  kept) with L + R + A on controller 2 while standing facing front, or the
  sequence in `word_82F204` entered on controller 2, in the controller
  settings screen (it counts up the same variable; all of it needs the
  switch on, as controller 2 is only read then);
- the Golden Torizo's room gives a loadout for X + Y + A + B held as it
  loads (`sm_aa.c`), and death skips its white flash with controller 2's A +
  L + R.

Code nothing reaches stays dead: the Draygon puppet on controller 2
(`Draygon_Func_37`), the scroll save and load (`DebugScrollPosSaveLoad`) and
the demo recorder's pause (`HandleDemoRecorder_1`) are never called.

The port reaches what it can with glue in `gba/` and no change to `src/`
beyond one guard (`SaveToSram`): a button combination sets `enable_debug`
(`gba/dev_tools.c`), the GBA's Select reads as controller 2's in the map
(`ReadReg`'s JOY2L), and a loadout toggle draws its text into the map's own
tilemap. Three things to know. The map hides stations a file hasn't used, and
a new file has none, and the room map hides the rooms of an area without its
map station, so the glue sets the used bits and the map stations' bytes while
the map shows and puts them back (and `has_area_map`, which the room map's
load took from them). The SNES's debug pad (`ReadJoypadInputs`: Select with L
or R, then A toggles the IRQ, B swaps the missiles away) would be reached by
the GBA's buttons, so `joypad_dbg_flags` has its off bit set. And its font: the game draws its menus' text in tiles (here
the map's letters are tiles $30 to $49), so text for a screen is a tilemap
write, with no font of the port's own, and it has to survive the squash
(`kSquashLine`): the game's own 8x8 letters do.

## 5. Verification: make every claim checkable

The work went fastest whenever a bug could be reproduced and compared
against ground truth headlessly. Build these tools early.

- **A headless runner** (`tools/mgba/smrun.c`, on libmgba): it scripts
  buttons (`KEYS`), takes screenshots at frames or on log lines (`SHOTS`,
  `SHOT_ON`), dumps memory (`DUMP`), records audio (`AUDIO`), profiles
  cycles by function and instruction (`PROFILE`, `PCS`), and shows the
  instructions before a jump to address 0 (`TRAP0`), for null function
  pointers.
- **A logic oracle** (`tools/hostrun.c`): the cartridge built with
  `-DSM_TRACE=ON` logs the joypad and a hash of game state each frame.
  `hostrun` replays that joypad on the PC build, which is itself verified
  against the original game, and prints the same log to diff. The first
  divergent frame points at the bug.
- **A picture oracle** (`tools/snesref.c`): it takes a memory dump, finds
  the SNES's VRAM, CGRAM, OAM and per-line registers in it by symbol, and
  draws the frame with the PC build's PPU. That is what a SNES would have
  shown from the very same state.
- **A coverage sweep** (`tools/room-tour.sh`): `-DSM_ROOM_TOUR=ON` walks
  the ROM's door graph and sends the player into every room. Then
  `tools/room_compare.py` ranks the rooms by the pixel difference from
  `snesref`. Re-run it after every renderer change and check that nothing
  regressed. One sweep found half a dozen renderer bugs that playing never had.
- **Scenarios** (`tools/scenarios.sh`): `-DSM_SCENARIOS=ON` plays a list of
  scripted scenarios without a player: the weapons (each of the 24 beam sets,
  uncharged and charged, missiles, supers, bombs, power bombs, X-ray, grapple,
  screw attack, space jump, speed booster, shinespark) and the ten bosses'
  fights, to check each one's picture, speed and sound. See below.
- **A sound oracle** (`tools/spcref.c`, `tools/compare_audio.py`): replay
  the game's sound requests through the original driver and an emulated
  DSP, then compare the loudness envelopes and spectra.
- **Sample-exact audio, between builds:** hashing the mixed samples of an
  old and a new build parts as soon as one runs at another speed (a busy
  loop of 40 iterations in the mixer does it, a few seconds in): the game's
  sound requests meet the driver's ticks at times that depend on the cycles.
  To prove a faster mixer the same, put the old routine (renamed, in ROM)
  beside the new in one build, run both on the same inputs every chunk, and
  compare what they leave, besides the same on random inputs.
- **Shortcuts:** `-DSM_SKIP_INTRO`, `-DSM_START_ZEBES` and `-DSM_TEST_ITEMS`
  get to the interesting state in seconds. `-DSM_TOUR_ONLY=0x...,0x...`
  tours only some rooms, `-DSM_TOUR_SETTLE` stays longer in each (bosses
  need seconds to wake), and `-DSM_TOUR_FIRST` starts elsewhere (Ceres).
- **A tour that shards:** the tour goes from room to room, so where Samus
  stands in a room, where the camera is, the frame and random counters, the
  scroll registers all come from the rooms before it: a room's picture and
  its cost depend on them, and no room can be run without going through all
  that precede it (a shard that starts in the middle gets other pictures
  for nearly every room, however many rooms it warms up with). For
  parallel runs, `smrun`'s `FORK=ADDR:V1,V2,...` keeps the machine's whole
  state when the first `SHOT_ON` picture is saved (the landing site), and
  runs on from it once per value, with the value written at `ADDR` first
  (the tour's `tour_next`, from `llvm-nm`: the room it goes to next), each
  until its next picture. Each room is then entered from the landing site's
  state, whichever process runs it: pictures and cycle counts are the same
  with 1 shard or 10 (to the cycle), and with the rooms in any order. Its
  pictures are not the chained tour's (245 of 252 differ, Samus's place
  among them, though d2aa and d646 still have her outside the room), so a
  baseline for it is made by it.
- **Sanitizers on the PC build:** ASan and UBSan on the PC build and
  `hostrun` find the game's out-of-bounds indexing before it bites the GBA.
- **The game's memory, watched:** `smrun`'s `MEMCHECK` (ranges from
  `tools/memcheck_ranges.py`) logs every load and store the game's own code
  makes outside its RAM, the cartridge, the stack and its own variables;
  `WATCH=LO:HI` logs the writes to one range, for a victim you know;
  `STACKCHECK` reports each stack's low-water mark. `STATE` and `STATE_OUT`
  load and save mGBA save states, so a player's state can be replayed.
- **A bot beside the original:** `hostrun` can also drive the PC build
  itself, with a bot that walks every room by its doors, fights, pauses and
  dies, comparing with the original game every frame. 2.5 million frames of
  it found four more upstream bugs and none of the port's, and its crashes
  under ASan found one that wrote far past the game's RAM.

When the comparison tool and the port disagree, suspect both. The
comparison script once silently compensated for the renderer's off-by-one
line.

When a picture looks wrong, render the same frame's dump with `snesref`
before debugging the renderer. The tour forces doors that the game would
have locked, and leaving a boss fight that way (Kraid's) corrupts the next
rooms' graphics on the SNES too. Bugs that show in `snesref`'s picture are
the test's, not the port's.

### The scenario tour

`gba/scenarios.c` (built only with `-DSM_SCENARIOS=ON`, so the cartridge is
byte for byte the same without it) spawns Samus through the developer tools
(`dev_tools.c`: a load station or a boss's door, with ALL ITEMS and, for the weapons, a beam set of the scenario's own), then
feeds the GBA's buttons by a script, a word per run of frames
(`B*2 -*10`, `R+B*3`, `( ... )xN`, `@` for a shot). The buttons go in where
`input.c` reads the GBA's, so the real mapping is what's tested (R and Select
for the other weapons, L to aim). The cartridge prints `scenario: NAME`,
`tour: shot NAME-N` (for smrun's `SHOT_ON`) and the game's own "60 frames in N
vblanks" lines; Samus's health and ammunition are always full. After a script
the game resets to the title for the next scenario. The list is data in the
file: add a scenario with a line. Which run is chosen by the buttons smrun
holds at the start (or `-DSM_SCENARIO_ONLY=name,prefix*` at build time).

```
cmake -S . -B build-scn -DCMAKE_TOOLCHAIN_FILE=cmake/gba-toolchain.cmake \
      -DSM_ROM=sm.smc -DSM_LLVM_SOURCE=... -DSM_SCENARIOS=ON    # (+ -DSM_AUDIO_LOG=ON for sound)
cmake --build build-scn -j4
tools/scenarios.sh tools/mgba/smrun tools/snesref build-scn out-scn   # JOBS=4 CHUNK=6
```

Results are in `out-scn`: `summary.txt` (a line a scenario: shots, worst and
mean share of pixels that differ from `snesref`'s picture of the same memory,
the slowest and mean frames per second of its 60-frame windows, game cycles a
frame), `compare/NAME-N.png` (GBA, SNES and difference side by side),
`compare.txt` (every shot, worst first), `shots/` (the pictures and memory
dumps) and `raw/` (smrun's logs). All 44 scenarios, 465 shots, take about 25
seconds. With `AUDIO=1 SPCREF=... ROM=sm.smc` (needs the `-DSM_AUDIO_LOG=ON`
build) it records each scenario's sound instead, replays the same requests
through the SNES's driver (`spcref`), and puts the loudness correlation, lag
and level ratio in the summary (`audio/NAME.txt` has the spectra). Limits:
the speed windows around a spawn are not the scenario's, so the first of each
is dropped; the SNES reference runs at 59.94 frames a second and the GBA at
59.73, so correlation falls as a scenario lengthens (the bosses' 80 seconds
read lower than the weapons' 4); the picture comparison skips squashed
screens; a script is open loop, so a fight that goes differently (a boss that
doesn't start) still gets its shots.

## 6. Performance

- **Profile before optimizing.** The game's own logic was never the
  bottleneck. The compatibility layer was: byte-at-a-time DMA, recording
  redundant HDMA writes, and rebuilding line registers.
- Budget: about 280,000 cycles per frame. Log each 60 frames' game, render
  and audio cycles, and the VBlanks taken, as `gba/main.c` does.
- Put hot loops in IWRAM as ARM, and use the GBA's DMA for bulk copies from
  ROM.
- Precompute everything that can be computed at cartridge-build time.
- Cache derived state keyed on its inputs (window regions, palette by
  brightness), and convert only what changed.
- Spread occasional heavy work over frames. Here the subtraction
  estimate samples a slice per frame over 16 frames, never all at once.
- Measure before keeping a "known good" option. Here `-ffunction-sections`
  with `--gc-sections` shrank the engine by a fifth, and making the
  decompilation's single-file functions `static` stopped the compiler
  keeping copies nobody called. `-O2`, `-Os`, `-Oz` and LTO were all
  slower: LTO's cross-file inlining doesn't fit in IWRAM.
- In a synchronous main loop (game, capture, wait for VBlank, present), a
  frame even slightly over budget waits for a second VBlank. Don't wait
  for the next VBlank interrupt if one began since the last present and
  there's still time in it to present (`gba/main.c`'s `WaitForVBlank`).
  Slow frames then borrow from fast ones: in Ridley's Ceres fight that was
  47 fps to 58 without saving a cycle. Profile "time in the BIOS" as a
  symptom: idle time in a game that can't keep up is time lost to waiting.

- The scenario tour finds the one-off frames that a 60-frame mean hides
  (the slowest frame of a window is in the log). The X-ray scope had three:
  its setup reads the tilemaps back from VRAM by DMA (a byte at a time: 3
  frames), its window HDMA is a table of a line an entry (generic code: a
  third of a frame), and the game's own search for each block's X-ray entry
  (272 of them in one frame; a small cache of the answers did it).

## 7. Pitfalls that cost the most

1. **Undefined behavior that the PC hides.** The game copied overlapping
   buffers with `memcpy`. glibc copies them safely, but a fast forward
   `memcpy` corrupts them. The symptom, Samus unable to climb slopes, was
   far from the cause. Find such cases with an overlap-checking `memcpy`
   on the PC build, and fix the game source with `memmove`.
2. **Misaligned reads** of ROM structures, and misaligned word stores in
   your own code (see §4).
3. **The VOFS + 1 rule** (see §4).
4. **Priority ties** between GBA backgrounds (see §4).
5. **Division in ARM code** links compiler-rt's Thumb helpers into ARM
   callers. The result is wrong returns, not link errors. In Thumb it works,
   but slowly (see §2).
6. **IWRAM creep.** Every feature costs bytes. Check `__iwram_bss_end`
   after each change.
7. **Detecting the HUD by heuristics.** Rules like "BG3 alone means HUD"
   break in rooms that use BG3 alone for fog. Compare several registers.
8. **Test scripts that hang.** A test player who walks into a door can
   bounce between two rooms forever. Pin the player and cancel doors you
   didn't trigger. Give them enough health that one hit in a single frame
   can't kill them.
9. **Backgrounds enabled mid-frame.** A background switched on in DISPCNT
   partway down the screen appears three scanlines late. Keep the layers on
   from line 0 and hide them in the lines above (darken to black with BLDY).
10. **Testing a stale build.** Check the cartridge's timestamp before
    trusting a regression or an improvement. One "regression" here came
    from a build step that never ran.
11. **Measuring speed during transitions.** Per-room timings that include
    the room load are noise, and so is the camera's scroll to the player
    after it, which can take seconds with pauses. The tour times 60 frames
    once the camera has stood still for 60 (`tour: speed` in its log).
12. **Tables indexed past their end.** The game indexes handler tables by
    level data that can exceed them. The SNES reads the next code's bytes;
    the port called a null pointer, at a place that depended on the
    build's layout. If a crash moves when unrelated code changes, suspect
    this.
13. **One rule, two places.** The BG3 priority rule that fixed the pause
    screen (§4) broke Phantoon's room a day later. When a heuristic fixes
    one screen, ask which other screen it decides for.
14. **Faster loads move the game's timeline.** The sound driver runs by the
    clock, and the game waits for it (the music queue, in door transitions),
    so a shorter load can leave a wait a frame longer, and a test script's
    button presses, at fixed frames, land elsewhere in the title screen.
    Every later picture then differs from one taken before in the phase of
    what blinks, the minimap's marker first, and nothing else. For a change
    that speeds a load up, compare what it replaced on every call of a tour
    (the decompressed bytes, the fade's colors) rather than pictures.
15. **The game's RAM has the port's state after it.** On the SNES an index
    past the end of a bank wraps (the 65816's X is 16 bits) or reads open
    bus. In the port, what's after `g_ram` is the port's own memory: a
    block index of -1 read the copy of VRAM, so a garbage block spawned a
    PLM and stalled a room; the X-ray scope's tables, with the beam's row
    above the screen, were written 128 KB past their start, over the sound
    driver's state. Keep such indices as wide as the ROM's registers
    (`uint16`), and run `MEMCHECK` over a tour.
16. **Restore from the source, not the hardware.** Colors the VBlank
    handler puts back after a mid-frame patch were read from the GBA's
    palette, which a slow frame could have left patched: the HUD then kept
    a power bomb's yellow for good. Recompute them from the SNES's CGRAM.

## 8. Working practice

- Commit each working step on a branch, with a message explaining the
  cause and the check. Don't push unless the human asks.
- Never commit the ROM, cartridges built from it, or extracted assets.
  Screenshots for documentation are the exception.
- Report results with numbers: frames per VBlank, cycles, rooms matching,
  frames traced identical. Say plainly what is approximated and what is
  still slow.
