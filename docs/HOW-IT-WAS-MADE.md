# Super Metroid GBA Progress Report: October 2026

Super Metroid was never meant to fit in your pocket. It was written in 1994
for a console with 128 KB of RAM, a graphics chip built for layered,
scrolling worlds, and a whole second computer just for its music. The Game
Boy Advance has 32 KB of fast RAM, a smaller screen, and a sound system that
amounts to two 8-bit DACs and a timer.

This is the first progress report, and it covers everything: six days in
which the port went from a black screen to the whole game, every room and
every boss, at 60 frames per second. That's about 150 commits on the
`gba-port` branch. Some of them are big features, some fix bugs that took
an afternoon to find and a line to fix, and a few are the tools that made
the rest checkable. One fixes the same bug twice, a day apart.

There's no emulator anywhere in this. The
[snesrev/sm](https://github.com/snesrev/sm) project decompiled the whole
game into C, function by function, and its PC build checks that C against
the original game every frame. That makes it a faithful copy of the game
that compiles for any CPU, including the GBA's ARM7TDMI. So the plan was to
compile the game's logic for the GBA, leave it alone, and write new code
only where the game meets the hardware: the picture, the sound, the controls
and the memory.

Let's get to it.

## Notable Changes

### [17dad34](https://github.com/nickdesaulniers/sm/commit/17dad34) - Port to the Game Boy Advance

The toolchain is clang and lld, with no GCC, devkitARM or vendor SDK. That
means no sysroot either, so the port brings its own startup code, linker
script, cartridge header and small C library. The game's logic in
`src/sm_*.c` compiles unchanged; `gba/` replaces the PC build's platform
layer.

Before the game could boot at all, it had to get past alignment. The
decompiled C is faithful to a CPU that has no alignment rules: the 65816
reads a 16-bit value from any address, and the game casts byte pointers to
wider types everywhere. The ARM7TDMI reads a 32-bit value from an odd
address by quietly rotating it, which returns garbage without crashing. The
fix was to make the game's `uint16` and `uint32` types 1-byte aligned on the
GBA. The compiler then emits byte loads wherever the game might misbehave,
and the C stays the same as the PC build's.

The rest of this first commit is the translation layer. The game talks to
the SNES's graphics chip as every SNES game does: it writes registers,
uploads tiles over DMA, and reprograms the chip partway down the screen
with HDMA. The renderer keeps a model of the SNES's video memory and
registers, and once a frame turns it into the GBA's terms:

- SNES Mode 1 has two 16-color layers and one 4-color layer, each tile with
  a priority bit. On the GBA it's four tiled backgrounds, with one SNES layer
  split in two by tile priority, so that sprites can sit between its halves
  as they do on the SNES.
- Mode 7, the rotating, scaling layer of the intro and of Ceres station,
  becomes the GBA's affine background.
- HDMA becomes a stream of GBA registers, copied in before each line by the
  GBA's own HBlank DMA.
- The GBA shows 240x160 pixels, the SNES 256x224. In gameplay the picture
  is a 1:1 crop that follows Samus, with the HUD pinned at the top.

It reached the title screen, the file select and Ceres station. No sound, no
transparency, and a frame rate in the twenties, but it was the real game,
making its own decisions, on a handheld.

### [3b23ee4](https://github.com/nickdesaulniers/sm/commit/3b23ee4) - Run at 60 frames per second

The first guess was that the game's own logic was too slow for a 16.8 MHz
CPU. A profiler proved that wrong.

That profiler is `smrun`, a small headless runner built on mGBA's library.
It runs the cartridge with a script of button presses and charges every
cycle to a function, or to an instruction. It showed that the game's logic
was never the problem. The time went to the layer between the game and the
hardware: DMA copied a byte at a time through emulated ports, every HDMA
write was recorded even when it changed nothing, and each line's GBA
registers were rebuilt from scratch.

So the fixes went into that layer. DMA to video memory copies in bulk. The
display records the SNES's registers only when their values change. The
translation layer runs from IWRAM, the GBA's 32 KB of fast internal RAM, as
32-bit ARM code instead of the 16-bit Thumb used elsewhere. Maps convert 16
entries at a time, only where the game changed them.

Ceres went from 2 to 2.5 GBA frames per game frame to about 180,000 of
the 280,000 cycles in one, and the landing site on Zebes to about 240,000. Both
run at 60 fps.

From here on, IWRAM was the scarcest thing in the project. Every feature
since has had to earn its bytes, and the linker refuses to build when IWRAM
would run into the stack.

### [4f6a44d](https://github.com/nickdesaulniers/sm/commit/4f6a44d) - The whole screen outside gameplay

The title, menus and pause screen have text, and a 240x160 crop of them
cuts words in half. Outside gameplay the port shows the whole SNES screen,
squashed: 6 of every 8 lines, always the same two rows of each tile, and
sprites scaled by 3/4.

![Before and after: the title screen cropped, then the whole screen squashed](images/progress/squash-title.png)

*The title screen, before (left) and after.*

This commit also fixed a black pause screen. Its BG3 is a black fill behind
everything, in tiles without the priority bit, but the GBA has no fifth
background to split BG3 by priority, so the renderer had put all of BG3 in
front. The fix: a BG3 map goes in front only if it has priority tiles. Keep
that rule in mind, because it comes back.

### [c4efc8f](https://github.com/nickdesaulniers/sm/commit/c4efc8f), [ed46f8f](https://github.com/nickdesaulniers/sm/commit/ed46f8f) - Teaching the GBA to sing

Super Metroid's music comes from the SNES's SPC700, a sound CPU with 64 KB
of RAM and an 8-voice sampler chip. The decompilation had translated the
game's sound driver to C, so it runs on the GBA's main CPU like everything
else.

The GBA has no 64 KB to spare. But everything the driver reads from that
RAM was uploaded from the ROM in the first place, so on the GBA it reads the
ROM directly, through a map of where each upload came from. The 175 samples,
compressed in the SNES's BRR format, are decoded to 8-bit PCM when the
cartridge is built. In place of the sampler chip, a mixer in ARM assembly
mixes eight voices, with the SNES's envelopes, noise and echo, at 10512 Hz.

The second commit moved the mixing into a timer interrupt after each VBlank.
Mixing a frame's sound once per game frame had a nasty failure mode: after
one slow frame the next had two frames of sound to mix, so it was slow too,
and so on. Driven by the GBA's own frames, a slow game frame no longer
stalls the music.

Checking it was its own project. `tools/spcref.c` plays the same sound
requests through the PC build's driver and emulated sound chip, and
`tools/compare_audio.py` compares the two recordings' loudness over time and
across octaves.

### [6a22248](https://github.com/nickdesaulniers/sm/commit/6a22248), [1fe60ca](https://github.com/nickdesaulniers/sm/commit/1fe60ca) - Color math and windows

The SNES can add or subtract a whole second picture, the "sub screen", to or
from the main one, pixel by pixel. Super Metroid does it constantly: rain on
Crateria, fog, water, the glow of lava, darkness. The GBA can only blend a
layer with the one directly beneath it.

Addition doesn't care about order, though. Draw the sub screen's layers on
top, blend them down, and the sum is the same. The fixed color the SNES adds
or subtracts is folded into the palette. The SNES's two windows, which shape
the X-ray scope's cone and power bombs' blasts, map onto the GBA's two, with
their edges moving every line in the same DMA stream as the scrolling.

![Before and after: the landing site with a flat sky, then with fog and rain](images/progress/color-math.png)

*The landing on Zebes, before (left) and after.*

Subtraction had to wait.

### [eb37393](https://github.com/nickdesaulniers/sm/commit/eb37393), [1078097](https://github.com/nickdesaulniers/sm/commit/1078097) - The hills that wouldn't let Samus climb

With the picture and sound working, Samus landed on Zebes, set off, and
stopped on the first hill. She couldn't climb a slope.

![Before and after: Samus stuck on the hill, then over it and onto the next](images/progress/slopes.png)

*The same steps from the ship, before (left) and after.*

Playing and guessing wasn't going to find this one, so the port gained a
flight recorder. A trace build logs the joypad and a hash of the game's
state every frame. A host tool, `hostrun`, replays that joypad on the PC
build, which is itself checked frame by frame against the original game.
The two logs went their separate ways at the exact frame Samus loaded the
room.

The cause was a single `memcpy`. As a room loads, the game copies its slope
data (BTS) out of the level data to just past it, and in big rooms the
source and destination overlap. That's undefined behavior. The PC's C
library happens to copy such buffers safely; the GBA's fast `memcpy`,
copying front to back, overwrote the data before reading it. The fix went
into the game source, `memmove` for the overlapping copies, and `memcpy`
went back to being fast. It's reported upstream as
[snesrev/sm#51](https://github.com/snesrev/sm/issues/51), with the
[other issues found while porting](UPSTREAM-ISSUES.md).

### [8fc7722](https://github.com/nickdesaulniers/sm/commit/8fc7722) - Ridley's door

Near the end of the Ceres escape, the player has to find the door out of
Ridley's room. On the GBA it was nearly invisible: Ridley, the floor, and the
door's blue cap floating in darkness.

That room draws its walls, blocks and door frame on the SNES's second
background, in priority tiles, behind a first background that's one solid
dark tile across the screen. The renderer had assumed what's true of nearly
every room, that the first background is the one worth splitting by
priority. Now it counts each background's priority tiles as it converts
their maps, and in a frame where the second shows more, it splits that one
instead.

![Before and after: Ridley's room as an empty blue field, then with its walls and blocks](images/progress/ridley-door.png)

*Ridley's room after he leaves, before (left) and after.*

### [229d58e](https://github.com/nickdesaulniers/sm/commit/229d58e), [b8cbbbc](https://github.com/nickdesaulniers/sm/commit/b8cbbbc) - Every room, one by one

Bugs like Ridley's door raised a question: how many other rooms are broken
somewhere nobody has walked yet?

To answer it, the port gained a tour. A test build reads the door lists out
of the ROM, finds all 254 rooms on Zebes by walking from door to door, and
sends Samus through a door into each in turn. She can't die on the way, and
she's pinned where she comes in; otherwise she falls back through the door
she came up through and bounces between two rooms forever. In each room
`smrun` saves a screenshot and the GBA's memory.

Then `tools/snesref.c` takes that memory dump, finds the SNES's video state
in it, and draws the frame with the PC build's SNES graphics emulator: what
a real SNES would have shown from the very same state. A script crops it as
the GBA does and ranks the rooms by how much differs.

The first ranking was humbling. Only 130 of the 254 rooms were within 5% of
the SNES's picture. The tour ran again after every fix:

- **Fog rooms.** A third of the game puts the fog alone on the main screen
  and the room on the sub screen. On the GBA the fog won a priority tie and
  covered the room, so some rooms were pitch black. Now the room is drawn
  as usual with the fog blended on top.
- **Fog between layers.** Brinstar's dark rooms and the Wrecked Ship add
  fog to the background but not the foreground. The fog layer now slides
  between them.
- **Kraid's lair**, the only room with a 64x64 tile map, showed its top
  half. Now it shows the half on screen.
- **One line off.** In Norfair's heat shimmer every line of the background
  was its neighbor's. The SNES shows background row `scroll + 1` on its
  first visible line, and the GBA row `scroll`. With a still background
  that's an invisible one-pixel shift, and the comparison script had been
  quietly compensating for it.
- **The HUD's seam.** Three lines under the HUD showed only the backdrop.
  When `DISPCNT` switches a background on partway down the screen, the GBA
  waits three lines before drawing it. The gameplay layers now stay on from
  the top of the frame, blacked out behind the HUD.
- **Subtraction.** Maridia's and Tourian's shadows subtract, and the GBA
  can't. But subtraction can be split in two: the palette subtracts one
  color from the whole room, and the shadow layer, drawn on top, adds back
  the difference between that color and its own. A few dozen sampled pixels
  pick which of three approximations comes closest, and which color to
  subtract, channel by channel. The samples are staggered: the first grid
  stepped down the screen in multiples of eight lines, so every sample
  landed on the same row of its tile and missed the thin lines of Maridia's
  shadow pattern entirely.

![Before and after: a fog room all black, then the room with Samus](images/progress/fog-room.png)

*A room with fog, before (left) and after.*

After all that, 252 of the 254 rooms matched the SNES to within 5%, most of
them pixel for pixel. The total difference was a ninth of what it was.

![Before and after: Maridia, with the door drawn twice and the room too dark, then matching the SNES](images/progress/doors.png)

*Before the tour (left) and now. The door was drawn twice over, the colors
were off, and the HUD had lost its energy tanks.*

### [5691898](https://github.com/nickdesaulniers/sm/commit/5691898), [ce1002f](https://github.com/nickdesaulniers/sm/commit/ce1002f), [2078cd2](https://github.com/nickdesaulniers/sm/commit/2078cd2) - Water and heat at full speed

The tour could also time each room, and the timings showed what screenshots
couldn't. Most of Norfair and Maridia ran at 25-40 fps.

Norfair's heat shimmer and Maridia's water scroll a background differently
on every line. HDMA writes them from tables in ROM, one entry a line, each
pointing at the wave the game computes in RAM. The port replayed that HDMA a
line at a time, with a full copy of the graphics chip's registers for every
line. 160 lines of that took most of a frame.

The first fix gave the scrolling states of its own, apart from the other
registers, which stay few. That saved about 70,000 cycles a frame, and
wasn't enough. The second fix stopped emulating those tables and read them
as the game means them: each table is walked once and cached ("baked") as
the lines it writes and where their values are in RAM, and each frame
those channels fill in the lines' scrolling in one tight pass. Norfair went
from 32 to 52 fps on average and Maridia from 32 to 53.

Then the cache missed in an interesting way. While the camera scrolls, the
game starts the wave table an entry or two further along every frame, so
each frame asked for a "new" table, missed the cache, and fell back to the
slow path. In Maridia's `d913` the camera's scroll down to Samus took 269
VBlanks for 180 frames. Now a table that's a whole number of entries off
from a cached one is slid into place, and only the lines that came on
screen are baked. Those 180 frames take 209 VBlanks. Over the whole tour,
122,880 tables built this way were checked against the old line-by-line
walk, and every one matched.

### [4326a01](https://github.com/nickdesaulniers/sm/commit/4326a01), [2cb9028](https://github.com/nickdesaulniers/sm/commit/2cb9028) - Fading without dividing

A door fades the screen out and in again, working out each of up to 512
colors' channels a step at a time with the SNES's hardware divider: some
1,500 divisions a frame. The ARM7TDMI has no divide instruction, so each was
a call into compiler-rt's software division, and a door spent 18% of its
time dividing.

First the divider became a multiply: `a / b` is the top word of `a` times a
rounded-up reciprocal of `b`, exact for every 16-bit `a` and every `b` from 2
to 255 (all of them checked). Then the fades stopped dividing at all. For a
given step of a fade, a channel's result depends only on its value and its
target, 0-31 each, and the game's fades are 6, 8, 12 or 15 steps long. So
the build works out all 41,984 results ahead of time, into 41 KB of ROM.

The tour's 254 doors went from a median of 172 GBA frames each to 145. The
SNES takes 105.

### [5742f8f](https://github.com/nickdesaulniers/sm/commit/5742f8f), [b74d1c0](https://github.com/nickdesaulniers/sm/commit/b74d1c0) - Smaller, and things that didn't work

Compiling each function into its own section let the linker drop the unused
ones: the engine went from 1.26 MB to 1.02 MB. The decompilation also had
2,111 functions used in only one file but declared for all of them, so the
compiler kept a copy of each that nothing called. They're `static` now,
another fix offered upstream ([snesrev/sm#52](https://github.com/snesrev/sm/issues/52)).

Some ideas were measured and dropped. `-O2`, `-Os` and `-Oz` were all slower
than `-O3`. Full LTO placed the IWRAM code in ROM and ran at half speed;
thin LTO inlined so much across files that IWRAM overflowed. One commit
([675c3f0](https://github.com/nickdesaulniers/sm/commit/675c3f0)) moved code placement from file names to sections to make LTO
possible, and was reverted ([18622d1](https://github.com/nickdesaulniers/sm/commit/18622d1)) once LTO turned out slower anyway.

### [30ce5dd](https://github.com/nickdesaulniers/sm/commit/30ce5dd), [22c5ef4](https://github.com/nickdesaulniers/sm/commit/22c5ef4) - Squashed screens that keep their text

Squashing 224 lines into 160 means dropping two of every eight, and which
two decides whether you can read the menus. The plan had been to drop rows 3
and 7 of each tile, sparing the tops and bottoms of letters. But the SNES
shows background row `scroll + 1` on its first line (that rule again), so
the port was really dropping rows 0 and 4: the tops of letters and the bars
of E and A. "BOTTLED", "NO DATA" and EXIT were cut off.

The first fix analyzed the text on screen to pick which lines to drop, band
by band. Letters came out uneven, and they changed shape as more of a
screen was analyzed. The second fix is simpler and better. It drops the
lines that show rows 0 and 4 of the text layer's tiles, following the
scrolling. As tiles are converted, row 0 is merged into row 1 and row 4 into
row 3, keeping whichever pixel is brighter. Letters keep their strokes over
their outlines and shadows, and nothing changes as you look at it.

![Before and after: the file select reading NO UAIA, then NO DATA](images/progress/squash-text.png)

*The file select, before (left) and after.*

Sprites got the same care. Eight-line sprites (mostly text, like "©1994
Nintendo") aren't scaled any more. Sprites flipped left to right had lost a
column, because the GBA flips a scaled sprite about its middle and takes
columns `width..1`. The copies of the tiles in the right half of sprite VRAM
are now stored flipped, in reverse order. The intro's gunship used to have a
gap every 16 pixels.

### [b20e159](https://github.com/nickdesaulniers/sm/commit/b20e159), [60860ae](https://github.com/nickdesaulniers/sm/commit/60860ae), [f8f98fc](https://github.com/nickdesaulniers/sm/commit/f8f98fc), [2753549](https://github.com/nickdesaulniers/sm/commit/2753549) - The GBA's own buttons

The GBA has no X or Y, so gameplay uses the GBA Metroids' layout: A jumps,
B shoots, L aims diagonally, R readies missiles, and Samus always runs.
Then the details:

- Message boxes come up during gameplay, where the port remaps the
  buttons. The save question's "no" did nothing, and held buttons closed
  item boxes the moment they appeared. While a box is up, the buttons are
  now the SNES's of the same name.
- The X-ray scope shines while the run button is held, and Samus always
  runs, so picking the scope turned it on. While it's picked, R is the run
  button.
- Select used to pick missiles, which R already readies. It now cycles R's
  item through super missiles, the grapple beam and the X-ray scope.
- The item pickup boxes told you the SNES's buttons. They're rewritten for
  the GBA's, in the message font's own letters and icons: "hold R (missile)
  & press the B button."
- The options menu's Controller Setting Mode showed the SNES's buttons and
  let you remap them, though the GBA's controls are fixed. It now draws the
  GBA's buttons, with round A and B, shoulder tabs and a d-pad, and only END
  and RESET do anything.

![Before and after: the super missile box naming the X button, then the GBA buttons](images/progress/item-box.png)

*The super missile pickup, before (left) and after.*

![Before and after: CONTROLLER SETTING MODE with the SNES's buttons, then CONTROLS with the GBA's](images/progress/controls.png)

*The options menu, before (left) and today.*

### [be704b4](https://github.com/nickdesaulniers/sm/commit/be704b4) - The tour finds a crash

The tour pins Samus where she comes in, and in one Maridia room that was
inside a block. The GBA reset. A new `smrun` option, `TRAP0`, shows the
instructions before any jump to address 0, and found a null function
pointer. The spike and grapple blocks' reactions look up a table by the
block's BTS byte, which can be past the table's end; the C read whatever
followed. Which pointer it read depended on the build's layout, so an
unrelated commit set it off. A BTS past the table now does nothing, and it's
reported upstream ([snesrev/sm#55](https://github.com/snesrev/sm/issues/55)).

### [04c8dfe](https://github.com/nickdesaulniers/sm/commit/04c8dfe), [400b345](https://github.com/nickdesaulniers/sm/commit/400b345) - Ceres sounded awful

Compared with the SNES, the GBA had 32 to 46 dB too much between 1.6 and 5
kHz in Ceres's elevator room, where the SNES has next to nothing. Besides the
8-bit DAC, there were two causes:

- A voice's volume times its envelope was kept in 5 bits. Ceres's quiet
  voices were 1s to 4s, so their fades moved in steps of a quarter or more,
  and rounding down lost a third of their loudness. Now it's 8 bits.
- Ceres's ambience plays its samples slowly, at a seventh to a third of
  the mixer's rate, each held for several output samples. That's a
  staircase, and it buzzes at the sample rate with images at 1-3 kHz. The
  SNES interpolates, and now the mixer does too, for voices played slower
  than half its rate.

Then the envelopes. They ran each few-millisecond chunk at the rate their
state had at its start, so a note's attack, done in a step or two, went on
at its quick rate through the decay and sustain. Notes faded too soon, and
the music was 3.5 dB quieter than it should be. Each envelope state now
steps at its own rate.

Faster too. Silent voices still advance without being mixed ([5b3d4d2](https://github.com/nickdesaulniers/sm/commit/5b3d4d2)):
28% of the voices mixed had been silent, 42% in Maridia. The driver now
runs four ticks at a time, at 125 Hz, and the envelopes once each 250 Hz.
Sound in Ridley's Ceres fight went from 79,000 cycles a frame to 61,000.

### [50825c8](https://github.com/nickdesaulniers/sm/commit/50825c8), [91873f3](https://github.com/nickdesaulniers/sm/commit/91873f3) - Less work for the same picture

The game uploads Samus's and the enemies' tiles every frame, mostly the
same as the frame before, and each upload was converted again. Now a chunk
of VRAM is marked dirty only when its contents actually differ. In
Brinstar's morph ball room with sidehoppers the picture takes 86,000 cycles
a frame, down from 101,000. A merged scan of the lines' states saved another
13,000 a frame in Ridley's Ceres fight.

### [e5d5c71](https://github.com/nickdesaulniers/sm/commit/e5d5c71) - The HUD over Mode 7, and the BG3 rule, again

Taking the README's new screenshots turned up two more bugs.

![Before and after: Ridley in Ceres, with the HUD showing stray text, then the real HUD](images/progress/ceres-hud.png)

*Ridley's fight in Ceres, before (left) and after.*

In Ceres's Mode 7 rooms, the HUD showed "press hold button" text. The GBA's
VRAM was right; the HUD layer was scrolled 264 lines down, onto an old map.
The code stored the scroll registers four bytes at a time into a field of
the display segment that sits at an address that isn't a multiple of four.
The ARM7TDMI doesn't fault on that. It rounds the address down, so every
value landed two bytes early: the window settings got the HUD's horizontal
scroll, and the HUD's vertical scroll got another layer's. The alignment
rule from the very first commit, again, this time in the port's own code.

![Before and after: Phantoon appearing over a black screen, then over his room](images/progress/phantoon.png)

*Phantoon appears, before (left) and after.*

When Phantoon appeared, the whole gameplay area went black. Remember the
pause screen's rule from earlier: a BG3 map goes in front if it has priority
tiles. In Phantoon's room the HUD's priority tiles share a map with the
room's own BG3, an opaque black fill that the SNES draws behind everything.
The HUD's tiles put it all in front. Now BG3 goes in front only where the
map rows a part of the screen shows have priority tiles. Re-running the
tour after both fixes, no room's ranking moved.

### [3c1c6b3](https://github.com/nickdesaulniers/sm/commit/3c1c6b3) - Late, but not a whole frame late

Ridley's fight in Ceres was painful: 47 fps on average, and down to 35 in
its worst seconds. The profiler showed something odd. Over a fifth of the
CPU's time went to sleeping in the BIOS, waiting for VBlank, in a fight
that couldn't keep up.

The main loop runs a frame of the game, captures the picture, waits for
VBlank, then presents the picture. Game, picture and sound together took
about 257,000 cycles a frame on average, under the 280,000 there are. But
some frames went a little over, finishing a few lines into VBlank, and the
wait was for the *next* VBlank interrupt. A frame a few hundred cycles late
waited a whole frame more.

There's time to spare inside VBlank, though. It lasts 68 lines, and
presenting a frame takes about 23 of them. Now, if a VBlank has begun since
the last frame was presented and it's before line 192, the frame is
presented right away. `Render_Present` already knew how to start the
display partway through VBlank. The slow frames now borrow a little time
from the fast ones that follow, as long as they fit on average.

Ridley's fight went from 47.4 fps to 57.9 on average, and its worst second
from 35.6 to 49.3. Across the whole tour, the rooms taking more than 60
VBlanks for 60 frames went from 24 to 6, and the slowest room went from
55.4 fps to 59.0. Not a single cycle of work was saved.

### [04593b4](https://github.com/nickdesaulniers/sm/commit/04593b4), [7253795](https://github.com/nickdesaulniers/sm/commit/7253795), [e045cbb](https://github.com/nickdesaulniers/sm/commit/e045cbb) - The last slow rooms

With presenting late in VBlank, the tour's slow rooms came down to a
handful, and each had its own reason.

A power bomb shapes its blast with two HDMA channels writing a window's
edges on every line. The port replayed them a byte at a time from the
cartridge, each byte through a function call, and in the Glass Tunnel,
with Maridia's water and shadows on top, the blast's two seconds ran at 25
and 29 fps. Now those channels read the lines from the same baked tables as
the water's scrolling, in an ARM loop: 47-49 fps, and checked against the
old code every frame of the scene.

The Glass Tunnel also ran over every few frames when nothing was happening.
Maridia's water table slides a line every eight frames or so, and baking its
one new line walked all 224 entries from the table's start: 28,000 cycles,
now 1,100. The shadows' search for the color to subtract did all its work
in four of every sixteen frames; it's spread over fifteen now.

Ridley's fights were last. Ceres's background is a gradient written to the
fixed color on every line, so every line was a new state for the renderer
to stop at: some 30,000 cycles a frame, for a gradient the GBA can't show
anyway. Those channels now run on their own, after the others. Ceres Ridley
went to 59.7 fps on average and Zebes's to 59.5, and the tour's 255 rooms
all took 60 VBlanks for 60 frames, for the first time.

### [f1694a0](https://github.com/nickdesaulniers/sm/commit/f1694a0), [30bff63](https://github.com/nickdesaulniers/sm/commit/30bff63) - White flashes at doors

Presenting late had a cost, found by play testing: white flashes and
scrambled tiles at doors. A slow frame with the screen off had set the
GBA's forced blank, which shows white, not the SNES's black. And a late
present with a room's worth of tiles to convert ran on into the picture,
which showed the last frame's maps over the new tiles.

The first fix only presented late when the work would fit in VBlank, and
lost the doors their speed: 57 seconds of the tour ran slow, from 14. The
second made running over safe instead. A present that runs into the
picture takes the display over from the line it's at, and may do so on
purpose as long as it ends within the HUD's lines, where the old frame and
the new look the same.

### [bbb5df7](https://github.com/nickdesaulniers/sm/commit/bbb5df7), [0a6692d](https://github.com/nickdesaulniers/sm/commit/0a6692d), and seven more - A day of speed

With every room at 60, the next goal was headroom. About ten branches
worked at once on different parts of the frame, each measured over the
whole tour and each checked to change no picture:

| Change | Cycles a frame saved, over the tour |
|---|---|
| Capture builds the line streams a run of lines at a time | 26,094 |
| HDMA decoded only when its inputs change; scroll channels as patches | 11,900 |
| The scroll stream fills runs of equal lines with one store multiple | 8,766 |
| Game code compiled with `g_ram` as a constant, loops unrolled by 4 | 7,288 |
| The mixer, echo and envelopes reworked | 7,200 |
| Only changed palette blocks and sprites converted | 7.2% |
| Echo skipped when it can't be heard | 6,175 |
| Faster SNES register and DMA writes | 5,700 |
| `memcpy` and `memset` in assembly; ARM entry points for Thumb callers | 5,205 |

The tour's average frame went from about 196,000 cycles to 112,000, of the
280,000 there are. The unrolling was measured at 2, 4 and 8: 4 kept 95% of
8's gain for a fifth of its ROM. Doors got their own branch: the room's
decompression in ARM from IWRAM, 4.7 times faster, and the fades from
tables, for a median of 109 GBA frames, from 116.

### [d8137f8](https://github.com/nickdesaulniers/sm/commit/d8137f8), [da9bf1b](https://github.com/nickdesaulniers/sm/commit/da9bf1b), [d170f2c](https://github.com/nickdesaulniers/sm/commit/d170f2c) - Text on the squashed screens, again

![Before and after: the equipment screen, with cut-off labels under a stale CRATERIA heading, then SAMUS and whole labels](images/progress/pause-text.png)

*The equipment screen, before (left) and after.*

The squashed screens had kept the menus' text readable, but the pause
screen's was in other palettes, and its ink lost to its background when
two rows merged. The planet map's heading is letters made of two 8-line
sprites, one above the other; squashed, they overlapped by two lines, and
PLANET ZEBES read as ZEDE8.

Then the font itself. Merging row 4 into row 3 closed a C's lower stroke
into an O ("DATA OOPY"), turned an R into an A, and the HUD's 3 into a 2.
So the fonts are redrawn when the cartridge is made from your ROM: of each
tile's 8 rows, `tools/squash_font.py` drops the two that lose the least,
the same two for every letter of a font, and puts the other six where the
squash will show them. The squashed screens use a redrawn tile where the
game's matches it exactly; gameplay is untouched.

### [32ca124](https://github.com/nickdesaulniers/sm/commit/32ca124), [fc53912](https://github.com/nickdesaulniers/sm/commit/fc53912) - Developer tools and a scenario tour

Super Metroid still has its developers' debug mode, with a map that spawns
Samus at any save station, the debug ones included. Nothing in the game
turns it on; on the GBA, L, R and Start on the title screen do. The port
adds a loadout picker, every boss's door, and in gameplay invincibility,
refills, frozen enemies and a quick death.

That made a second tour possible. The room tour visits every room but
fights nothing. The scenario tour spawns Samus with a loadout and plays a
script: each of the 24 beam combinations charged and uncharged, missiles,
bombs, power bombs, the X-ray scope, the grapple, the screw attack, the
speed booster, and every boss's fight, played to the boss's death and out
of the room, with a picture every few frames. Each picture is compared with
`snesref`'s, and each second is timed. It found most of what follows.

### [597ed01](https://github.com/nickdesaulniers/sm/commit/597ed01), [ea0916b](https://github.com/nickdesaulniers/sm/commit/ea0916b), [fef73b1](https://github.com/nickdesaulniers/sm/commit/fef73b1) - Which half of BG2

The GBA has four backgrounds and the SNES's Mode 1 needs five, so each
frame one SNES layer, BG1 or BG2, is split in two by tile priority and the
other isn't. The renderer chose by counting priority tiles. Three bosses
proved the count wrong.

![Before and after: Crocomire's legs alone, then his whole body](images/progress/crocomire.png)

*Crocomire, before (left) and after.*

Crocomire's head and torso are BG2's priority tiles, but BG1's floor and
ceiling had more, so BG2 wasn't split, and his body went behind BG1's wall.

![Before and after: Kraid's cave with his body missing, then Kraid filling the right half](images/progress/kraid.png)

*Kraid, mid-fight, before (left) and after.*

Kraid's whole body is BG2's priority tiles, and his cave's wall is BG1's.
Whenever the screen's shaking showed four more of the wall's priority tiles
than his, the split went to BG1, and Kraid vanished for a second or so at a
time. A play tester found that one, and the boss scenarios, which took a
picture every ten seconds, had missed it. They take one every few frames
now.

Mother Brain was hidden from the waist up, 128 of BG2's tiles to 268 of
BG1's. The split now samples the screen where BG2's priority tiles lie over
BG1's, every fourth frame, and looks at what you'd actually see.

### [be95371](https://github.com/nickdesaulniers/sm/commit/be95371) - Power bombs in color, the X-ray scope at 60

![Before and after: a power bomb's blast in grey, then yellow](images/progress/power-bomb.png)

*A power bomb's blast, before (left) and after.*

A power bomb's blast is yellow on the SNES, stepping through white to
orange, and it was pale grey on the GBA. The SNES makes it with the fixed
color, shown through a window in a solid black BG3, and the GBA can only
brighten toward white. But with BG3 solid black, BG3 can *be* the fixed
color: its palettes are patched to it and shown only where the SNES's
window would show it. Every scenario picture through a blast now matches
`snesref`'s, from 62.9% different at the worst.

The X-ray scope's slowest second was 50.5 fps. Its setup read two tilemaps
back from VRAM a byte at a time, and the game searched the X-ray table for
each block, 272 times in one frame. Words, and a small cache of the
search's answers, brought it to 59.7.

### [102f836](https://github.com/nickdesaulniers/sm/commit/102f836) - The lava nobody saw

![Before and after: a Norfair room with no lava, then lava up to Samus's knees](images/progress/lava.png)

*A room in Norfair, before (left) and after.*

Norfair's lava and Lower Norfair's acid weren't drawn at all, anywhere.
Two HDMA channels write BG3's vertical scroll, the lava's and then the
HUD's, and the HUD's table ends at line 31. The renderer let the later
channel's value hold to the bottom of the screen, so BG3 stayed at 0 and
the lava never appeared.

The tour never showed it because `snesref` draws from the same captured
lines as the GBA, so both were wrong together. It only matters where the
lava's surface is on screen, which the camera rarely shows when Samus
isn't near it. The lava scenarios now raise the surface to her.

### [db48d13](https://github.com/nickdesaulniers/sm/commit/db48d13) - Maridia's sand

![Before and after: Maridia's sand pale, then yellow](images/progress/maridia-sand.png)

*Maridia, before (left) and after. The sand is the SNES's now; the wall above it is a little too purple, still.*

Maridia's shadows subtract, which the GBA approximates by subtracting one
color from the room in the palette and adding the shadow's difference back
on top. The color is picked from samples of the screen. On sand dithered
from two colors, the search picked a color between them that suited
neither, and the sand came out pale. It now picks by squared error.

### [1340e0c](https://github.com/nickdesaulniers/sm/commit/1340e0c) - The rainbow beam

![Before and after: Mother Brain's beam as a grey wedge, then solid orange](images/progress/rainbow.png)

*The rainbow beam, before (left) and after. It cycles through the rainbow.*

Mother Brain's rainbow beam is drawn on the SNES by color math: a fixed
color, cycling through the rainbow, added inside a window shaped like the
beam. On the GBA it was a pale grey wedge. Now it's a solid layer of that
color, shown through the window and added by the GBA's own blending.

### [20a7b94](https://github.com/nickdesaulniers/sm/commit/20a7b94) - Samus in front

![Before and after, zoomed: Ridley's body over Samus, then Samus in front](images/progress/samus-front.png)

*Ridley and Samus in Ceres, zoomed in, before (left) and after.*

The SNES stacks sprites by their places in OAM; the GBA by their priority
against the backgrounds first. Ridley's body comes after Samus in OAM but
has the higher priority, so on the GBA he was drawn over her. The port now
notes which sprites are Samus's as the game draws them, and gives the
sprites after them that overlap her the same priority.

### [f4ed537](https://github.com/nickdesaulniers/sm/commit/f4ed537), [cf483c4](https://github.com/nickdesaulniers/sm/commit/cf483c4), [712ad25](https://github.com/nickdesaulniers/sm/commit/712ad25) - Reading past the end

Two of the strangest bugs had the same shape. The game's 128 KB of RAM is
one array in the port, and what's after it is the port's own memory, where
on the SNES an index wraps around in a 16-bit register.

The room tour sometimes stalled in one room, but only when an
unrelated change moved the build's layout. Samus, mid-door, was outside
the room, so the game looked up her block at index -1, read the byte
before the level data, and on the GBA got the port's copy of VRAM. A
garbage block type spawned a garbage object. A new `smrun` option,
`MEMCHECK`, logs every load and store the game's code makes outside its
own memory, and found it at once. The read now gets what the SNES's 16-bit
index gets.

Then a bot. `hostrun` drove the PC build through 2.5 million frames, every
room by its doors, bombs, missiles, the X-ray scope, pausing, deaths, the
intro, Ceres and the bosses, comparing with the original game every frame.
Nothing the port changed differed. It found four more bugs that upstream
has too ([listed here](UPSTREAM-ISSUES.md)). One of them, the X-ray scope
with its beam starting above the screen, wrote its tables up to 128 KB past
where they belong. On the PC that crashed. On the GBA it overwrote the
sound driver's state, which sits right after the game's RAM. The indices
are 16 bits now, as the ROM's are, and every write stays in the game's RAM.

## Other changes

- **[23234ee](https://github.com/nickdesaulniers/sm/commit/23234ee)** The build makes `sm-engine.gba` with none of the game's data,
  and `tools/make_cartridge.py` appends the data from your own ROM. CI
  releases the engine only.
- **[ccf907e](https://github.com/nickdesaulniers/sm/commit/ccf907e)** The intro's playback of the Mother Brain fight, like the
  attract demos, plays recorded SNES buttons. The GBA's item picking ignored
  them, so Samus never switched to missiles and the playback never ended.
- **[254ad66](https://github.com/nickdesaulniers/sm/commit/254ad66)** While a door loads the next room with the SNES's screen off,
  the GBA's is off too, instead of showing the last frame over graphics
  being replaced.
- **[d6b2bc5](https://github.com/nickdesaulniers/sm/commit/d6b2bc5)** VRAM changes made while no tiled layer is on screen wait for
  the next frame that shows one. Converting everything instead had the
  Mode 7 flight to Zebes running at a third of full speed.
- **[22c5ef4](https://github.com/nickdesaulniers/sm/commit/22c5ef4)** The gameplay area follows Samus's feet and only moves once
  she's 12 lines from the middle, so crouching doesn't move the screen. The
  Ceres escape's timer stays put as it does.
- **[f6c58ee](https://github.com/nickdesaulniers/sm/commit/f6c58ee), [b68773f](https://github.com/nickdesaulniers/sm/commit/b68773f)** Running the PC build and `hostrun` under ASan and
  UBSan found the menu tilemap declared a quarter of its size
  ([snesrev/sm#54](https://github.com/snesrev/sm/issues/54)), and `hostrun` drawing through an uninitialized
  pointer.
- **[bdcfa87](https://github.com/nickdesaulniers/sm/commit/bdcfa87), [e9e18cf](https://github.com/nickdesaulniers/sm/commit/e9e18cf)** The tour can start in Ceres, and it now times each
  room for 60 frames once the camera has stood still for 60. Coming into a
  room, the camera scrolls to Samus for seconds and with pauses, and timing
  that measured the scroll, not the room.
- **[70b7986](https://github.com/nickdesaulniers/sm/commit/70b7986)** Ceres's escape message was under the HUD while Samus stood on
  the floor of Ridley's room, and its timer's last digit was off the right
  edge. Both are in view now.
- **[2904b92](https://github.com/nickdesaulniers/sm/commit/2904b92)** Pausing and unpausing fade the room out and in at its own
  size, rather than squashed, then popping back.
- **[1ad30cf](https://github.com/nickdesaulniers/sm/commit/1ad30cf)** B on the file select hung the game: the port didn't resume
  the reset code where the PC build does.
- **[9a59c33](https://github.com/nickdesaulniers/sm/commit/9a59c33)** Select only works with R held, and cycles what R readies:
  missiles, super missiles, power bombs, the grapple beam, the X-ray scope.
- **[e4d7e77](https://github.com/nickdesaulniers/sm/commit/e4d7e77)** Running through the landing site, every 10-15 frames one
  frame cost a whole frame's cycles, as BG1's and BG2's split flipped and
  both maps were converted again. A flip converts only the priority tiles'
  chunks now, with a hand-written map converter in assembly.
- **[b6a3b9b](https://github.com/nickdesaulniers/sm/commit/b6a3b9b)** After a power bomb or a fade, the HUD could keep the blast's
  yellow or the fade's grey for good: the colors put back after the HUD's
  lines were read from the GBA's palette, which a slow frame had left
  patched. They're computed from the SNES's palette now.
- **[b17b86e](https://github.com/nickdesaulniers/sm/commit/b17b86e), [922c632](https://github.com/nickdesaulniers/sm/commit/922c632)** The options menu's controller screen is CONTROLS,
  showing the GBA's buttons, with no remapping to reset.
- **[fb2ea2e](https://github.com/nickdesaulniers/sm/commit/fb2ea2e), [61d0605](https://github.com/nickdesaulniers/sm/commit/61d0605)** Making the cartridge from your ROM is friendlier:
  any file name, a zip, or a directory searched by checksum, and an error
  that says what's wrong.

## What's left

As of October 25, timed over the whole tour and settled in each room, every
room runs at 60 fps: all 255 take 60 VBlanks for 60 frames. Every boss's
fight, played to the end, averages 59.7 fps, and doors take a median of 107
GBA frames where the SNES takes 105. Some things are still slow, for a
second or so at a time:

- Kraid's worst second is 53 fps, Crocomire's 56, and one of the 22 seconds
  of Ridley's fight in Ceres runs at 55.
- Pausing, a game over and the escapes' first seconds drop to between 38
  and 45 fps, as their scenes load.

And some things aren't the SNES's yet:

- The SNES does color math only on sprites with palettes 4-7. The GBA
  blends all of them, so bosses come out pale under the fog layer, and
  Mother Brain's rainbow beam tints every sprite.
- In Ridley's Ceres lair the background gets lighter toward the bottom on
  the SNES, and is one flat color on the GBA.
- The GBA can't subtract colors, so dark rooms are approximated: the edges
  of Botwoon's and Draygon's darkened rooms are a little off.
- The GBA's 8-bit sound has a hiss the SNES never had.

But all of Zebes is there, at full speed, in your hands.

## This Report's Contributors

Every commit is by Nick Desaulniers, co-authored with Claude Opus 5.5. The
port stands on the [snesrev/sm](https://github.com/snesrev/sm)
decompilation and its authors, and on [mGBA](https://mgba.io), whose library
runs every test here.
