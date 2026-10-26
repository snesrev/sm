#!/usr/bin/env python3
# Compares what the GBA showed of each room on gba/tour.c's tour with what
# the SNES would have (tools/snesref.c, from the same frame's memory), and
# ranks the rooms by how much differs:
#
#     room_compare.py SNESREF SYMS DIR [OUT_DIR]
#
# DIR holds smrun's SHOT_ON output: r<room>.png and r<room>.ewram and so on.
# For each room, OUT_DIR (default DIR) gets cmp<room>.png: the GBA's
# picture, the SNES's cropped as the GBA's is (gba/render.iwram.c's
# SnesLine), and where they differ, in red. Needs Pillow and NumPy.
import glob, os, subprocess, sys

import numpy as np
from PIL import Image

# gba/render.iwram.c's.
kHudLines, kHudBlankLines, kGbaLines, kXOffset = 32, 8, 160, 8
kGbaHudLines = kHudLines - kHudBlankLines


def syms(path):
    out = {}
    for line in open(path):
        parts = line.split()
        if len(parts) == 3:
            out[parts[2]] = int(parts[0], 16)
    return out


def read_var(prefix, addr, size):
    region = {2: ('ewram', 0x3ffff), 3: ('iwram', 0x7fff)}[addr >> 24]
    data = open(f'{prefix}.{region[0]}', 'rb').read()
    return int.from_bytes(data[addr & region[1]:(addr & region[1]) + size], 'little', signed=True)


def compare(snesref, sympath, s, shot, ref_path, out_png):
    """Compares the picture |shot| (PNG) with the SNES's of the memory
    dumped beside it (shot minus .png): draws the SNES's to |ref_path| and
    the three-way sheet to |out_png|. Returns the fraction of pixels far off,
    or None for a squashed screen (the GBA's picture isn't a crop of the
    SNES's there)."""
    prefix = shot[:-4]
    subprocess.run([snesref, sympath, prefix, ref_path], check=True)
    gba = np.asarray(Image.open(shot).convert('RGB')).astype(int)
    ref = np.asarray(Image.open(ref_path).convert('RGB')).astype(int)
    if read_var(prefix, s['squash'], 1):
        return None
    hud = read_var(prefix, s['hud'], 1)
    y_offset = read_var(prefix, s['y_offset'], 4)
    lines = [g + kHudBlankLines if hud and g < kGbaHudLines else
             g + (kHudBlankLines if hud else 0) + y_offset for g in range(kGbaLines)]
    crop = ref[lines, kXOffset:kXOffset + 240]
    # The GBA's colors are 5 bits a channel, too, but its color math
    # isn't all the SNES's: count what's far off.
    bad = np.abs(gba - crop).max(axis=2) > 48
    diff = crop // 3
    diff[bad] = (255, 0, 0)
    sheet = np.concatenate([gba, np.full((kGbaLines, 4, 3), 255), crop,
                            np.full((kGbaLines, 4, 3), 255), diff], axis=1)
    Image.fromarray(sheet.astype(np.uint8)).save(out_png)
    return bad.mean()


def main():
    snesref, sympath, d = sys.argv[1:4]
    out_dir = sys.argv[4] if len(sys.argv) > 4 else d
    s = syms(sympath)
    results = []
    for shot in sorted(glob.glob(os.path.join(d, 'r' + '[0-9a-f]' * 4 + '.png'))):
        room = os.path.basename(shot)[1:-4]
        frac = compare(snesref, sympath, s, shot, os.path.join(out_dir, f'ref{room}.png'),
                       os.path.join(out_dir, f'cmp{room}.png'))
        if frac is not None:
            results.append((frac, room))
    for frac, room in sorted(results, reverse=True):
        print(f'{room} {100 * frac:5.1f}%')


if __name__ == '__main__':
    main()
