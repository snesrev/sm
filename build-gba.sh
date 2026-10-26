#!/usr/bin/env bash
# Builds the Game Boy Advance cartridge, sm.gba, from your own Super Metroid
# ROM, in one command:
#
#     ./build-gba.sh [ROM]
#
# ROM is your Super Metroid ROM, by any name (or a zip with it in), or a
# directory it's in, found by its checksum; by default, this directory. It's
# the Japan/USA release (sha1 da957f0d63d14cb441d215462904c4fa8519c613), with
# or without a 512-byte copier header. Needs clang, lld, llvm-objcopy, CMake, Python 3 and git (the
# first build also fetches LLVM's compiler-rt sources, about 30 MB, so it
# needs the network once). The cartridge comes out as sm.gba, here, and the
# build's files are in build-gba/. See docs/BUILDING-GBA.md.
set -euo pipefail
cd "$(dirname "$0")"
rom=${1:-.}

die() {
    echo "error: $*" >&2
    exit 1
}

# What to install, by what's on this machine (the packages' names differ).
install_hint() {
    if command -v apt-get >/dev/null; then
        echo "  wget https://apt.llvm.org/llvm.sh && sudo bash llvm.sh 22"
        echo "  sudo apt install cmake python3 git"
        echo "  export PATH=/usr/lib/llvm-22/bin:\$PATH"
        echo "  (LLVM 22: the distribution's own clang may be too old to fit the code in IWRAM)"
    elif command -v dnf >/dev/null; then
        echo "  sudo dnf install clang lld llvm cmake python3 git"
    elif command -v pacman >/dev/null; then
        echo "  sudo pacman -S clang lld llvm cmake python git"
    elif command -v brew >/dev/null; then
        echo "  brew install llvm lld cmake python git"
        echo "  (and put \$(brew --prefix llvm)/bin first in your PATH: Apple's own clang can't do this)"
    else
        echo "  install clang, lld, llvm, cmake, python3 and git with your system's package manager"
    fi
}

missing=
for tool in clang ld.lld llvm-objcopy cmake python3 git; do
    command -v "$tool" >/dev/null || missing="$missing $tool"
done
if [ -n "$missing" ]; then
    echo "error: not found:$missing" >&2
    echo "Install them with, for example:" >&2
    install_hint >&2
    exit 1
fi
if ! clang --print-targets 2>/dev/null | grep -q '^ *arm '; then
    die "this clang ($(command -v clang)) can't compile for ARM. Use LLVM's own clang (22 was used), not Apple's or a build with ARM left out."
fi

# (It says what's wrong: no ROM in the directory, or the file isn't the one.)
if ! rom=$(python3 tools/make_cartridge.py --find "$rom"); then
    [ "${1:-}" ] || echo "Put your Super Metroid ROM in $PWD (any name), or run ./build-gba.sh path/to/your/rom (or a directory with it)." >&2
    exit 1
fi

if [ ! -d build-gba/_deps/llvm-project ]; then
    echo "The first build fetches LLVM's compiler-rt sources (about 30 MB) from github.com."
fi
cmake -B build-gba -DCMAKE_TOOLCHAIN_FILE=cmake/gba-toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Release -DSM_ROM="$(cd "$(dirname "$rom")" && pwd)/$(basename "$rom")"
cmake --build build-gba -j"$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)"
cp build-gba/sm.gba sm.gba
echo
echo "Made $PWD/sm.gba ($(wc -c <sm.gba | tr -d ' ') bytes)."
echo "Run it in mGBA (or another GBA emulator), or copy it to a flash cartridge's card."
echo "Saves are the cartridge's SRAM: sm.sav next to it in an emulator."
