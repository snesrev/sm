#!/usr/bin/env bash
# Takes Samus into each room (gba/tour.c) and compares what the GBA shows
# of each with what the SNES would (tools/room_compare.py):
#
#     tools/room-tour.sh SMRUN SNESREF BUILD_DIR OUT_DIR
#
# BUILD_DIR is configured with -DSM_SKIP_INTRO=ON -DSM_START_ZEBES=ON
# -DSM_ROOM_TOUR=ON and built; SMRUN is tools/mgba/smrun and SNESREF
# tools/snesref, built. OUT_DIR gets each room's screenshot and memory,
# cmp<room>.png, and rank.txt: the rooms by how much of the picture differs.
set -euo pipefail
here=$(dirname "$0")
mkdir -p "$4"
SHOT_ON="tour: shot" KEYS="1:1:-" "$here/to-gameplay.sh" "$1" "$3/sm.gba" 110000 "" "$4/r" \
    > "$4/log.txt"
llvm-nm "$3/gba/sm.elf" > "$4/syms.txt"
python3 "$here/room_compare.py" "$2" "$4/syms.txt" "$4" > "$4/rank.txt"
echo "$(grep -c 'tour: shot' "$4/log.txt") rooms; the most different:"
head "$4/rank.txt"
