#!/usr/bin/env bash
# Builds smrun against a libmgba build:
#
#     tools/mgba/build.sh MGBA_SOURCE MGBA_BUILD [OUT_DIR]
#
# MGBA_BUILD is a CMake build of mGBA's source (built with clang:
# CC=clang CXX=clang++), which makes libmgba.a.
set -euo pipefail
if [ $# -lt 2 ]; then
    echo "usage: $0 MGBA_SOURCE MGBA_BUILD [OUT_DIR]" >&2
    exit 2
fi
src=$1
build=$2
out=${3:-$build}
here=$(dirname "$0")
# The defines must match the library's, or libmgba's structs are laid out
# differently here (USE_DEBUGGERS, for one, changes struct ARMCore): take
# them from its build.
mapfile -t defines < <(grep -o -- '-D[A-Z0-9_]*' "$build/CMakeFiles/mgba.dir/flags.make" | sort -u)
libs=("$build/libmgba.a")
[ -e "$build/libpng/libpng16.a" ] && libs+=("$build/libpng/libpng16.a")
clang -O2 -o "$out/smrun" "$here/smrun.c" "${defines[@]}" \
    -I"$build/include" -I"$src/include" "${libs[@]}" -lm -lz -lpthread -lGL -lEGL
echo "built $out/smrun"
