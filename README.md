# Super Metroid for the Game Boy Advance

<p align="center">
  <img src="docs/images/title.png" width="240" alt="The title screen">
  <img src="docs/images/ceres-ridley.png" width="240" alt="Ridley takes the baby Metroid on Ceres station">
  <img src="docs/images/ceres-explodes.png" width="240" alt="Ceres station explodes as Samus escapes">
  <br>
  <img src="docs/images/landing.png" width="240" alt="Samus leaves her ship in the rain">
  <img src="docs/images/bomb-torizo.png" width="240" alt="Bomb Torizo throws bombs at Samus">
  <img src="docs/images/spore-spawn.png" width="240" alt="Spore Spawn opens its core as Samus clings to the wall">
  <br>
  <img src="docs/images/kraid.png" width="240" alt="Kraid">
  <img src="docs/images/crocomire.png" width="240" alt="Crocomire swings at Samus">
  <img src="docs/images/golden-torizo.png" width="240" alt="Samus shoots the Golden Torizo, who throws his orbs and crescents">
  <br>
  <img src="docs/images/ridley.png" width="240" alt="Ridley">
  <img src="docs/images/phantoon.png" width="240" alt="Phantoon">
  <img src="docs/images/botwoon.png" width="240" alt="Botwoon">
  <br>
  <img src="docs/images/draygon.png" width="240" alt="Draygon swoops and grabs Samus">
  <img src="docs/images/golden-statue.png" width="240" alt="The golden statue at the entrance to Tourian">
  <img src="docs/images/mother-brain.png" width="240" alt="Mother Brain">
</p>

Super Metroid, running natively on the Game Boy Advance. The game itself is
the [snesrev/sm](https://github.com/snesrev/sm) decompilation, the SNES game's
code rewritten as C. This port compiles that C for the GBA's ARM7TDMI and
replaces the SNES's graphics, sound and input with the GBA's own. Nothing is
emulated: the GBA's 16.8 MHz CPU has no time to spare for an emulator.
Every room runs at 60 frames per second.

The port includes none of the game's data. You build the cartridge from your
own copy of the SNES ROM.

## Making the cartridge

You need your own copy of the Super Metroid ROM (the Japan/USA release). The
cartridge contains the ROM's data, so it can't be shared.

- **[From a release](docs/BUILDING-GBA.md#from-a-release):** download three
  files from the
  [latest release](https://github.com/nickdesaulniers/sm/releases/latest)
  and run one Python script. No compiler needed.
- **[From source](docs/BUILDING-GBA.md#from-source):** clone this repository
  and run `./build-gba.sh`. Needs LLVM 22.

Then play `sm.gba` in [mGBA](https://mgba.io) or another GBA emulator, or on
a flash cartridge. [docs/BUILDING-GBA.md](docs/BUILDING-GBA.md) has the
steps, the ROM's checksum, saves, flash cartridges and what each error means.

## How it differs from the SNES

- **The screen.** The GBA's screen is 240x160, and the SNES's is 256x224.
  During gameplay the GBA shows a 1:1 crop that follows Samus vertically, with
  the HUD at the top. The title screen, menus and pause screen show the whole
  SNES screen, squashed to fit.
- **Controls.** The GBA has no X or Y buttons, so gameplay uses the layout
  of the GBA Metroids: A jumps, B shoots, L aims diagonally, R readies
  missiles, and Samus always runs. See [Controls](#controls).
- **Sound.** The SNES's sound driver runs on the GBA's CPU, with the SNES's
  echo, and a mixer in place of the SNES's sound chip. It sounds right but
  has the GBA's 8-bit hiss.
- **Effects.** The SNES's color math and windows are mapped onto the GBA's
  blending and windows: rain, fog, water, heat shimmer, shadows, the X-ray
  scope's cone and power bombs. The GBA can't subtract colors, so darkened
  rooms are approximated.
- **Speed.** Timed in every room, all run at 60 fps. Every boss's fight
  averages 59.7 fps, though Kraid's slowest second is 53. Doors take about
  as long as on the SNES (a median of 107 frames, against 105). Pausing, a
  game over and the escapes drop to about 40 fps for a second as their
  scenes load.

## Controls

The SNES pad has X and Y buttons and the GBA doesn't, so gameplay uses the
layout of the GBA Metroids, *Zero Mission* and *Fusion*:

| Action                 | SNES (default)        | GBA                                  |
|------------------------|-----------------------|--------------------------------------|
| Jump                   | A                     | A                                    |
| Shoot                  | X                     | B                                    |
| Run                    | hold B                | always on                            |
| Aim diagonally up      | R                     | hold L                               |
| Aim diagonally down    | L                     | hold L + Down                        |
| Pick an item           | Select cycles through them | hold R, Select picks what R readies instead of missiles |
| Missiles (or the item) | pick, then X          | hold R, then B                       |
| X-ray scope            | pick, then hold B     | pick, then hold R                    |
| Power bombs            | pick, then X in morph ball | hold R in morph ball, then B    |
| Put the item away      | Y                     | let go of R                          |
| Pause, map             | Start                 | Start                                |

So, as on the GBA Metroids, missiles are a shoulder button away: hold R and
press B. With R held, Select cycles R's item through missiles, super
missiles, power bombs, the grapple beam and the X-ray scope, whichever Samus
has found, shown on the HUD; the pick is kept for the next time R is held.
Select does nothing without R, or in morph ball, where R readies power bombs
whatever was picked. Letting go of R doesn't drop Samus from the grapple beam.
The charge beam's special attacks work as on the SNES: with power bombs
readied (R held, power bombs picked), hold B until the beam is fully charged
(about two seconds) and Samus fires the wave, ice, spazer or plasma attack,
for a power bomb each. They need the charge beam and exactly one of those four
beams; with several beams together there is no special attack.
Because Samus always runs, the speed booster charges whenever she runs long
enough.

On the title screen, the menus and the pause screen, each button is the SNES
button of the same name (B goes back).

The options menu's CONTROLS screen shows these controls, with the GBA's
buttons. They can't be changed there: END goes back.

<p align="center">
  <img src="docs/images/controls.png" width="240" alt="The CONTROLS screen, with the GBA's buttons">
</p>

## Reporting issues

Found a bug, a glitch, a slowdown or a sound problem? Please open an issue
on [the issue tracker](https://github.com/nickdesaulniers/sm/issues). It helps to include:

- The release, or the commit you built (`git rev-parse --short HEAD`).
- Where you play it: the emulator and its version, or the flash cartridge.
- Where it happened (area, room, boss) and what you were doing.
- If you can, an mGBA save state from just before it, or your `.sav` file,
  and a screenshot or a short video.

Please don't attach ROMs or cartridges (`sm.gba`): they contain Nintendo's
data. `sm-engine.gba` is fine.

Some bugs are the original game's, kept faithfully by the decompilation;
those found while porting are listed in
[docs/UPSTREAM-ISSUES.md](docs/UPSTREAM-ISSUES.md).

## Reading more

- [Building the cartridge](docs/BUILDING-GBA.md): every system, the ROM,
  flash cartridges, saves, and what to do when something goes wrong.
- [Developer tools](docs/DEVELOPER-TOOLS.md): start Samus at any station or
  boss, with any loadout, to test a room or a fight without playing up to it.
- [Progress report](docs/HOW-IT-WAS-MADE.md): from a black screen to the
  whole game, the notable changes and the bugs found on the way, with
  before-and-after pictures.
- [Issues to report upstream](docs/UPSTREAM-ISSUES.md): bugs found in the
  decompilation while porting it, with their fixes.
- [Porting a SNES game to the GBA](docs/PORTING-GUIDE.md): a guide for agents
  (and people) attempting a similar port. It covers the approach, the
  hardware mapping, the tools for checking the result, and the pitfalls.

## The PC version

This branch keeps the decompilation's PC build too, which runs the C code and
the original game side by side and compares them frame by frame. See
[BUILDING.md](BUILDING.md) for its build, and the
[snesrev Discord](https://discord.gg/AJJbJAzNNJ) for the project.

## Credits

- **The game:** Super Metroid is Nintendo's (1994). This port and its
  repository include none of the game's code or data: the cartridge is built
  from your own ROM.
- **The decompilation:** the [snesrev/sm](https://github.com/snesrev/sm)
  authors, who rewrote the game as C and check it against the original
  frame by frame. Its SNES emulation, which those checks and this port's
  tests run on, is from elzo_d. The decompilation and this port are under
  its [MIT license](LICENSE.txt).
- **The toolchain:** [LLVM](https://llvm.org): clang and lld build the
  cartridge, and compiler-rt's builtins are part of it.
- **The tests:** [mGBA](https://mgba.io), whose library runs the cartridge
  headless for the room tour and the scenarios (`tools/mgba/`).
- **The port:** Nick Desaulniers, with Claude Opus 5.5 as co-author of every
  commit. [How it was made](docs/HOW-IT-WAS-MADE.md) tells the story.
