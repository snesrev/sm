#!/usr/bin/env python3
# Writes the ranges smrun's MEMCHECK takes (see tools/mgba/smrun.c), for a
# GBA build:
#
#     memcheck_ranges.py BUILD_DIR > ranges.txt
#
# The game's code is the functions of libsm_game.a (the decompiled game, and
# the sound driver); what it may touch is the game's RAM (g_ram), the
# cartridge, the stack, and the variables libsm_game.a defines itself.
# Anything else it reads or writes is the port's (the sound's, the
# display's), which on the SNES would have been another bank: a read or
# write past the end of the game's RAM, most likely. memcpy and the like are
# checked when the game calls them.
import os
import re
import subprocess
import sys


def nm(path, *args):
    out = subprocess.run(['llvm-nm', *args, path], capture_output=True, text=True, check=True)
    return out.stdout.splitlines()


def main():
    build = sys.argv[1]
    lib = os.path.join(build, 'gba', 'libsm_game.a')
    elf = os.path.join(build, 'gba', 'sm.elf')
    game_funcs, game_data = set(), set()
    for line in nm(lib, '--defined-only'):
        f = line.split()
        if len(f) != 3:
            continue
        kind, name = f[1], f[2]
        if kind in 'Tt':
            game_funcs.add(name)
        elif kind in 'BbDdRr':
            game_data.add(name)
    syms = {}
    stack_lo = None
    for line in nm(elf, '-S', '--defined-only'):
        f = line.split()
        if len(f) == 3 and f[2] == '__iwram_bss_end':
            stack_lo = int(f[0], 16)
        if len(f) != 4:
            continue
        addr, size, name = int(f[0], 16), int(f[1], 16), f[3]
        syms.setdefault(name, []).append((addr, size))
    out = []
    for name in sorted(game_funcs):
        for addr, size in syms.get(name, []):
            out.append(('code', addr & ~1, (addr & ~1) + size))
    for name in sorted(game_data):
        for addr, size in syms.get(name, []):
            out.append(('data', addr, addr + max(size, 1)))
    g_ram = syms['g_ram'][0][0]
    out.append(('data', g_ram, g_ram + 0x20000))
    out.append(('data', 0x08000000, 0x0a000000))
    out.append(('data', stack_lo or 0x03007500, 0x03008000))
    for name, places in syms.items():
        if re.fullmatch(r'(__aeabi_)?mem(cpy|set|move|cmp|clr)\w*|__aeabi_memclr\w*|strlen', name):
            for addr, size in places:
                out.append(('helper', addr & ~1, (addr & ~1) + size))
    for kind, lo, hi in out:
        print(f'{kind} {lo:x} {hi:x}')


if __name__ == '__main__':
    main()
