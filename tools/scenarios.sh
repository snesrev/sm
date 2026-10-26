#!/usr/bin/env bash
# Runs the cartridge's scenarios (gba/scenarios.c: weapons and beam sets,
# and the bosses' fights, each played by a script of buttons, from a spawn
# of the developer tools) and reports on each one's picture, speed and,
# optionally, sound:
#
#     tools/scenarios.sh SMRUN SNESREF BUILD_DIR OUT_DIR [SHARD]
#
# BUILD_DIR is configured with -DSM_SCENARIOS=ON (and -DSM_SCENARIO_ONLY=
# name,prefix*,... for only some; neither -DSM_ROOM_TOUR nor -DSM_SKIP_INTRO)
# and built; SMRUN is tools/mgba/smrun (with STOP_ON) and SNESREF tools/snesref,
# built. Needs llvm-nm, Pillow and NumPy. SHARD, "I/N" (default 0/1), runs
# only the I'th of N shares of the chunks (below), to divide a run between
# machines, or runs: they write to different OUT_DIRs, or the same, apart.
#
# FRAMES (default 3000 + 12000 a scenario) is smrun's limit on the frames a boot runs: a scenario
# with many shots needs more (each shot holds a few; the long dense fights, boss-ridley-* and
# ceres-ridley-full, with CHUNK=1 for a boot each).
#
# The scenarios are split into chunks of CHUNK (default 6) that run one after
# another in a boot of the cartridge, JOBS (default 4) of them at once. Then:
#
#   OUT_DIR/shots/NAME-N.png    the GBA's picture at the script's Nth shot
#                               (and its memory dump beside it)
#   OUT_DIR/compare/NAME-N.png  that, the SNES's picture of the same memory,
#                               and where they differ in red
#   OUT_DIR/compare.txt         every shot's share of pixels far off, worst first
#   OUT_DIR/summary.txt         a line for each scenario: shots, worst and
#                               mean difference (%), frames per second (the
#                               GBA's screen is 59.73: slowest and mean of the
#                               scenario's 60-frame windows) and the game's
#                               cycles a frame; also printed
#   OUT_DIR/raw/cN.log          smrun's output for chunk N: the game's own
#                               "sm: 60 frames in N vblanks" lines, a line
#                               "scenario: NAME" as each begins and "scenario:
#                               end NAME", "tour: shot NAME-N" at each shot
#
# A scenario spawns, plays and ends in about 5 to 200 s of the game's time
# (a beam's, 4; a boss's, 70; Mother Brain's whole fight, 200), after 20 s of it for the
# spawn. smrun runs
# about 2000 frames a second, so all 44 scenarios (34 of weapons, 10 of
# bosses, with their 465 shots and comparisons) take about 25 s at JOBS=4, on
# 12 cores. A scenario that never gets to its script (the spawn failed) is
# missing from the summary.
#
# Sound: AUDIO=1 records the sound instead of taking pictures: each scenario
# in a boot of its own, to OUT_DIR/raw/NAME.wav, no shots (the frames aren't
# held), and compares it, as it plays from the script's start, with what the
# SNES's sound driver would play of the same requests: tools/spcref.c replays
# them to a reference, and tools/compare_audio.py compares the two. It
# needs BUILD_DIR configured with -DSM_AUDIO_LOG=ON too, SPCREF (the built
# tools/spcref: clang -O2 -I. -o spcref tools/spcref.c src/spc_player.c
# src/snes/dsp.c) and ROM (the game's, sm.smc) in the environment. The
# summary gets a column of each one's loudness correlation (1 is the same),
# the lag between them and the ratio of their RMS levels; OUT_DIR/audio/
# NAME.txt has the spectrum by octave too, and NAME.gba.wav and NAME.snes.wav
# the two recordings from the scenario's start, to listen to. Sound in a
# scenario is the game's: the room's music and the weapon's noises together.
set -euo pipefail
if [ $# -lt 4 ]; then
    sed -n '2,/^set -e/p' "$0" | sed '$d' | cut -c3-
    exit 2
fi
smrun=$(realpath "$1")
snesref=$(realpath "$2")
build=$(realpath "$3")
mkdir -p "$4"
out=$(realpath "$4")
shard=${5:-0/1}
here=$(dirname "$(realpath "$0")")
chunk=${CHUNK:-6}
jobs=${JOBS:-4}
audio=${AUDIO:-}
mkdir -p "$out/raw" "$out/shots"

# A number as the buttons smrun's KEYS holds (A is 1, B 2, ... L 512).
keys() {
    local n=$1 s="" i
    local names=(A B SELECT START RIGHT LEFT UP DOWN R L)
    for i in 0 1 2 3 4 5 6 7 8 9; do
        if (( n >> i & 1 )); then s+="${s:+,}${names[i]}"; fi
    done
    echo "${s:--}" | tr ',' '+'
}

# The scenarios' names, as the cartridge lists them as it starts.
env -u SHOT_ON -u STOP_ON -u AUDIO KEYS="1:100:-,-" "$smrun" "$build/sm.gba" 300 "" "$out/list_" \
    | grep -o 'scenario-list: .*' | cut -d' ' -f2- > "$out/list.txt" || true
count=$(wc -l < "$out/list.txt")
if [ "$count" -eq 0 ]; then
    echo "no scenarios: is $build built with -DSM_SCENARIOS=ON?" >&2
    exit 1
fi
echo "$count scenarios"

# One boot: from the first, for COUNT of them (the buttons held at the
# start say which: see gba/scenarios.c).
run_chunk() {  # run_chunk FIRST COUNT [NAME]
    local first=$1 n=$2 name=${3:-c$1}
    local frames=${FRAMES:-$((3000 + n * 12000))}
    if [ -n "$audio" ]; then
        KEYS="1:100:$(keys "$first"),$(keys $((n | 512)))" STOP_ON="scenario: all done" \
            AUDIO="$out/raw/$name.wav" "$smrun" "$build/sm.gba" "$frames" "" "$out/list_" \
            > "$out/raw/$name.log"
    else
        KEYS="1:100:$(keys "$first"),$(keys "$n")" SHOT_ON="tour: shot" STOP_ON="scenario: all done" \
            "$smrun" "$build/sm.gba" "$frames" "" "$out/shots/" > "$out/raw/$name.log"
    fi
}
export -f run_chunk keys
export smrun build out audio

if [ -n "$audio" ]; then
    # Each in a boot of its own, named.
    tasks=()
    while read -r i name; do
        if (( i % ${shard#*/} == ${shard%/*} )); then tasks+=("$i 1 $name"); fi
    done < "$out/list.txt"
else
    tasks=()
    for ((first = 0, k = 0; first < count; first += chunk, k++)); do
        n=$((count - first < chunk ? count - first : chunk))
        if (( k % ${shard#*/} == ${shard%/*} )); then tasks+=("$first $n"); fi
    done
fi
echo "${#tasks[@]} boots, $jobs at once"
printf '%s\n' "${tasks[@]}" | xargs -P "$jobs" -L 1 bash -c 'run_chunk $0 $1 $2'

llvm-nm "$build/gba/sm.elf" > "$out/syms.txt"
extra=()
if [ -n "$audio" ]; then extra=("${SPCREF:?SPCREF}" "${ROM:?ROM}"); fi
python3 "$here/scenario_report.py" "$out" "$snesref" "$out/syms.txt" "${extra[@]}"
