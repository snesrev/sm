# Building the Game Boy Advance cartridge

The repository has none of the game's data, so the cartridge, `sm.gba`, is
made on your machine from your own Super Metroid ROM. There are two ways:

- **[From a release](#from-a-release):** download the engine and add the
  ROM's data with Python 3 alone. No compiler.
- **[From source](#from-source):** clone this repository and build
  everything with `./build-gba.sh`. Needs LLVM 22.

## The ROM

It must be the Japan/USA release, "Super Metroid (Japan, USA) (En,Ja)": 3 MB
(3,145,728 bytes), sha1 `da957f0d63d14cb441d215462904c4fa8519c613`. A copier
header (512 extra bytes at the start, 3,146,240 in all, left by old SNES
copiers) is fine: it is dropped. Hacks, other releases and bad dumps are not
accepted; the cartridge reads the ROM's own tables and code.

To check yours:

```
python3 tools/make_cartridge.py --check your-rom.sfc   # says what is wrong, if anything
sha1sum your-rom.sfc                                    # Linux
shasum -a 1 your-rom.sfc                                # macOS
certutil -hashfile your-rom.sfc SHA1                    # Windows
```

The file can be named anything, and can be in a zip. Wherever the scripts
take the ROM, they also take a directory: they look in it, and in the
directories in it two levels down, for the file with the right checksum, and
say which it was (or, if none, which files were the right size but not it,
and why). `./build-gba.sh` with no argument looks in the repository's
directory.

## From a release

No compiler needed, only Python 3.

Download `sm-engine.gba`, `make_cartridge.py` and `squash_font.py` from one
release (the
[latest](https://github.com/nickdesaulniers/sm/releases/latest), or
[any](https://github.com/nickdesaulniers/sm/releases)) into one folder, put
the ROM there, and run:

```
python3 make_cartridge.py sm-engine.gba . sm.gba
```

Python 3 (any recent version) is all it needs; it takes a second or so. Use all
three files from the same release: a cartridge made with a script from another
version doesn't start (see below).

## From source

You need clang and lld (LLVM 22 was used), llvm-objcopy, CMake 3.20 or newer,
Python 3 and git, on a Unix-like system. The compiler must be LLVM's own with
the ARM target: Apple's clang from Xcode won't do. No devkitARM or GCC.

| System          | Install                                                          |
|-----------------|------------------------------------------------------------------|
| Debian, Ubuntu  | LLVM 22 from [apt.llvm.org](https://apt.llvm.org) (`wget https://apt.llvm.org/llvm.sh && sudo bash llvm.sh 22`), then `sudo apt install cmake python3 git` and `export PATH=/usr/lib/llvm-22/bin:$PATH` |
| Fedora          | `sudo dnf install clang lld llvm cmake python3 git`              |
| Arch            | `sudo pacman -S clang lld llvm cmake python git`                 |
| macOS (Homebrew)| `brew install llvm lld cmake python git`, then `export PATH="$(brew --prefix llvm)/bin:$PATH"` |
| Windows         | Use WSL2 with Ubuntu and follow the Debian line, or use a release (above) |

(Fedora was tried here with clang 22.1.8; CI builds on Ubuntu 24.04 with
apt.llvm.org's 22.1.8, and the two engines are the same bytes. An older clang
may not fit: Ubuntu 24.04's own clang 18 overflows IWRAM. macOS and WSL are
not tested.)

Then clone the `gba-port` branch, put the ROM in it (any name), and build:

```
git clone -b gba-port https://github.com/nickdesaulniers/sm.git sm
cd sm
./build-gba.sh
```

(Or give the script the ROM's path, or a directory it's in.)

It checks the tools and the ROM first and stops with a message if one is
missing or wrong. The first build fetches the sources of LLVM's compiler-rt
builtins (about 30 MB, from github.com, into `build-gba/_deps`) for the
division routines the ARM7TDMI lacks; later builds don't need the network. The
build takes a few seconds to a couple of minutes, by machine (6 s on a
4-core desktop). It leaves:

- `sm.gba` next to the script: the cartridge, about 5.2 MB.
- `build-gba/`: the build tree. `build-gba/sm-engine.gba` is the engine alone,
  without the game's data.

Building again after changing the code, or the ROM, only redoes what changed.
To build by hand:

```
cmake -B build-gba -DCMAKE_TOOLCHAIN_FILE=cmake/gba-toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Release -DSM_ROM=path/to/your/rom
cmake --build build-gba -j
```

Without `SM_ROM` and no ROM in the repository's directory, only
`sm-engine.gba` is made. CMake options that matter:

| Option               | Meaning                                                       |
|----------------------|---------------------------------------------------------------|
| `SM_ROM`             | The ROM to make `sm.gba` from, or a directory to find it in (default: this one). |
| `SM_LLVM_SOURCE`     | An existing llvm-project checkout to take compiler-rt from, for building offline. |
| `SM_LLVM_TAG`        | The llvm-project tag to fetch (default: your clang's version). |

The others (`SM_SCENARIOS`, `SM_ROOM_TOUR`, ...) are for testing; see the
[porting guide](PORTING-GUIDE.md).

## Running it

- **Emulators.** mGBA, and others that run GBA games (the cartridge is a plain
  `.gba` file; mGBA needs no BIOS file). The save is the cartridge's 8 KB of
  SRAM, which the emulator keeps in a `.sav` file named after the `.gba`
  (`sm.sav`). The header says `SRAM_V113`, which emulators use to choose the
  save type.
- **Flash cartridges** (EverDrive GBA, EZ-Flash and their kind). Copy `sm.gba`
  to the card. The cartridge is SRAM-saving: if yours asks, choose SRAM (32 KB)
  or let it detect the type from the header. The 5.2 MB image is far below the
  GBA's 32 MB limit.

## When something goes wrong

| What you see | Why |
|--------------|-----|
| `not found: clang ld.lld ...` | Install them (above). The script suggests the command for your system. |
| `this clang can't compile for ARM` | Apple's clang, or one built without ARM. Use LLVM's. |
| `Couldn't fetch compiler-rt's sources` | No network, or no llvm-project tag for your clang's version. Pass `-DSM_LLVM_SOURCE=` or `-DSM_LLVM_TAG=` (above). |
| `not the Super Metroid ROM this needs` | The wrong ROM; the message says its size or sha1 and why it differs. |
| `already has the game's data in it` | You gave `make_cartridge.py` `sm.gba` as the engine. Give `sm-engine.gba`. |
| `squash_font.py is missing` | Keep that file next to `make_cartridge.py`. |
| A red screen on start | The engine found no usable data in the cartridge. mGBA's log (Tools, View logs) says which: the data is missing (this is `sm-engine.gba`, not `sm.gba`), or it was made by a `make_cartridge.py` of another version. Make the cartridge again with the script from the same release or commit as the engine. |

## For developers: a BPS patch

`tools/make_bps.py` encodes and applies BPS patches, the format of Floating
IPS, beat and RomPatcher.js, and its `stats` command measures what a patch from
`sm.smc` to `sm.gba` would have to hold. It is a measuring tool only: whether to
publish a patch (which would carry data derived from the ROM) is undecided, and
nothing builds or releases one.

```
python3 tools/make_bps.py stats sm.smc sm.gba
python3 tools/make_bps.py make sm.smc sm.gba sm.bps
python3 tools/make_bps.py apply sm.smc sm.bps check.gba
```

A patch is only good for the engine it was made with: another clang gives other
bytes.
