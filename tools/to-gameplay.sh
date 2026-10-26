#!/usr/bin/env bash
# Runs the cartridge through the title screen and a new game's intro to
# Ceres (much faster with a build configured -DSM_SKIP_INTRO=ON), then holds KEYS (counted from the start of gameplay) and saves
# SHOTS (frames, counted likewise) as OUT_PREFIX<frame>.png:
#
#     KEYS=... tools/to-gameplay.sh SMRUN ROM FRAMES SHOTS OUT_PREFIX
set -euo pipefail
none() { printf -- '-,%.0s' $(seq "$1"); }
# Start every 300 frames from frame 1000: skips the attract mode, starts
# at the title, picks Samus A at the file select, and starts the game from
# the options screen that follows. Then A every 200 frames, for the intro's
# pauses for a button.
intro=$(for i in $(seq 300); do printf 'A,%s' "$(none 19)"; done)
export PRE_KEYS="1000:10:START,$(none 29)START,$(none 29)START,$(none 29)START,$(none 29)START,$(none 29)${intro%,}"
export AFTER="sm: game state 8"
exec "$1" "$2" "$3" "$4" "$5"
