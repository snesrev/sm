#!/usr/bin/env python3
"""Checks gba/decompress.S against the game's decompressor, byte for byte.

    python3 -m venv /tmp/venv && /tmp/venv/bin/pip install unicorn
    /tmp/venv/bin/python tools/decompress_test.py [--rom sm.smc] [--random N] [--seed S]

Needs clang and llvm-objcopy (to assemble decompress.S for the ARM7TDMI: the
same flags as the build's) and the Unicorn emulator's Python bindings. --rom
(default sm.smc, headerless 3 MB LoROM) adds the ROM's own compressed
streams. Prints "N streams, 0 differ" and exits 0 when they all match.

What it runs:
  * the GBA's Decompress, as assembled from gba/decompress.S, in Unicorn's
    ARM mode (instruction-accurate, not cycle-accurate, and with no IWRAM or
    cartridge timing: it tests the logic only), with the stream in emulated
    memory and a destination buffer that has 128 KB before and after it;
  * a Python port of DecompressToMem (src/sm_80.c, the non-SM_GBA path),
    command for command. It is a port, not the compiled C: if that function
    changes, change this to match. One deliberate difference: the original's
    DecompNextByte wraps its reads inside a ROM bank, and the port (like the
    assembly) reads straight on, so streams that cross a bank's end are
    not the same thing there; none of the game's do.
Both get the same destination buffer, filled with something other than
zeros (so copies from bytes not yet written, which the game never does but
a stream can, must agree too), and must give the same buffer and the same
end of output (Decompress's return value).

The streams:
  * random ones, with every kind of command (short and long), run lengths
    from 1 to 1024, copies from before, at and past the output's end, and
    overlapping their own output (--random, default 3000);
  * from the ROM: the 29 tilesets' tiles, tile tables and palettes
    ($8F:E6A2), the fixed addresses DecompressToMem(0x......) calls in src/,
    and every plausible room level data pointer in bank $8F's room headers
    (a 3-byte pointer into banks $C2-$CF; the odd false one is just another
    stream, and is skipped if it runs off the buffer).
"""
import argparse
import os
import random
import re
import struct
import subprocess
import sys
import tempfile

from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM
from unicorn.arm_const import UC_ARM_REG_LR, UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_SP

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

CODE, STACK, RET = 0x1000, 0x8000, 0x9000  # in the first 64 KB of the emulated memory
SRC = 0x100000       # the stream is copied here (64 KB; streams are cut to that)
DST = 0x400000       # the destination buffer
BUF = 0x40000        # its size
BASE = 0x20000       # where in it the output starts: copies can reach before it
ROM_AT = 0x1000000   # the whole ROM, for the real streams


class Overrun(Exception):
    pass


def reference(src, dst):
    """DecompressToMem (src/sm_80.c) on bytes |src|, writing |dst| from BASE.
    Returns the output's length."""
    p = dp = 0

    def next_byte():
        nonlocal p
        b = src[p]
        p += 1
        return b

    while True:
        b = next_byte()
        if b == 0xFF:
            return dp
        if (b & 0xE0) == 0xE0:
            cmd = (8 * b) & 0xE0
            ln = ((b & 3) << 8 | next_byte()) + 1
        else:
            cmd = b & 0xE0
            ln = (b & 0x1F) + 1
        if dp + ln > BUF - BASE - 0x1000:
            raise Overrun
        if cmd & 0x80:
            want_xor = 0xFF if cmd & 0x20 else 0
            if cmd >= 0xC0:
                sp = dp - next_byte()
            else:
                sp = next_byte()
                sp += next_byte() * 256
            while True:
                dst[BASE + dp] = dst[BASE + sp] ^ want_xor
                sp += 1
                dp += 1
                ln -= 1
                if not ln:
                    break
        elif cmd == 0x20:
            b = next_byte()
            for _ in range(ln):
                dst[BASE + dp] = b
                dp += 1
        elif cmd == 0x40:
            b = next_byte()
            b2 = next_byte()
            while True:
                dst[BASE + dp] = b
                dp += 1
                ln -= 1
                if not ln:
                    break
                dst[BASE + dp] = b2
                dp += 1
                ln -= 1
                if not ln:
                    break
        elif cmd == 0x60:
            b = next_byte()
            for _ in range(ln):
                dst[BASE + dp] = b
                b = (b + 1) & 0xFF
                dp += 1
        else:
            for _ in range(ln):
                dst[BASE + dp] = next_byte()
                dp += 1


def assemble():
    """The machine code of Decompress, from gba/decompress.S."""
    with tempfile.TemporaryDirectory() as d:
        obj, out = os.path.join(d, "d.o"), os.path.join(d, "d.bin")
        subprocess.check_call(["clang", "--target=armv4t-none-eabi", "-mcpu=arm7tdmi", "-c",
                               os.path.join(ROOT, "gba/decompress.S"), "-o", obj])
        subprocess.check_call(["llvm-objcopy", "-O", "binary", "-j", ".iwram.text.decomp",
                               obj, out])
        return open(out, "rb").read()


def make_emulator(code, rom):
    mu = Uc(UC_ARCH_ARM, UC_MODE_ARM)
    mu.mem_map(0, 0x10000)
    mu.mem_map(SRC, 0x10000)
    mu.mem_map(DST, BUF)
    if rom:
        mu.mem_map(ROM_AT, (len(rom) + 0xFFF) & ~0xFFF)
        mu.mem_write(ROM_AT, rom)
    mu.mem_write(CODE, code)
    return mu


def run_gba(mu, src_addr, init):
    mu.mem_write(DST, init)
    mu.reg_write(UC_ARM_REG_R0, src_addr)
    mu.reg_write(UC_ARM_REG_R1, DST + BASE)
    mu.reg_write(UC_ARM_REG_SP, STACK)
    mu.reg_write(UC_ARM_REG_LR, RET)
    mu.emu_start(CODE, RET)
    return bytes(mu.mem_read(DST, BUF)), mu.reg_read(UC_ARM_REG_R0) - (DST + BASE)


def random_stream(rnd):
    s = bytearray()
    for _ in range(rnd.randint(0, 40)):
        kind = rnd.randrange(7)
        ln = rnd.choice([1, 2, 3, 5, 31, 32, rnd.randint(1, 1024)])
        if ln > 32 or rnd.random() < 0.3:
            ln = min(ln, 1024)
            s += bytes([0xE0 | kind << 2 | (ln - 1) >> 8, (ln - 1) & 255])
        else:
            ln = min(ln, 32)
            s.append(kind << 5 | ln - 1)
        if kind == 0:
            s += bytes(rnd.randrange(256) for _ in range(ln))
        elif kind in (1, 3):
            s.append(rnd.randrange(256))
        elif kind == 2:
            s += bytes([rnd.randrange(256), rnd.randrange(256)])
        elif kind in (4, 5):
            s += struct.pack("<H", rnd.choice([0, 1, 2, rnd.randrange(300), rnd.randrange(65536)]))
        else:
            s.append(rnd.choice([0, 1, 2, rnd.randrange(256)]))
    s.append(0xFF)
    return bytes(s)


def rom_offset(addr):
    return (addr >> 16 & 0x7F) * 0x8000 + (addr & 0x7FFF)


def rom_streams(rom):
    """The addresses ($bb:aaaa) of the ROM's compressed streams."""
    ptrs = {0xB9A09D, 0xB98000}
    base = rom_offset(0x8FE6A2)  # 29 tilesets: tiles, tile table, palette pointers
    for i in range(29):
        for field in (0, 3, 6):
            p = rom[base + i * 9 + field:base + i * 9 + field + 3]
            ptrs.add(p[0] | p[1] << 8 | p[2] << 16)
    for name in os.listdir(os.path.join(ROOT, "src")):
        if name.endswith(".c"):
            text = open(os.path.join(ROOT, "src", name), errors="replace").read()
            for m in re.finditer(r"DecompressToMem\(\s*(0x[0-9a-fA-F]{6})\s*,", text):
                ptrs.add(int(m.group(1), 16))
    lo, hi = rom_offset(0x8F91F8), rom_offset(0x8FE000)  # the room headers
    for o in range(lo, hi):
        p = rom[o] | rom[o + 1] << 8 | rom[o + 2] << 16
        if 0xC2 <= p >> 16 <= 0xCF and p & 0x8000:
            ptrs.add(p)
    return sorted(p for p in ptrs if p & 0x8000)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--rom", default="sm.smc")
    ap.add_argument("--random", type=int, default=3000)
    ap.add_argument("--seed", type=int, default=1)
    args = ap.parse_args()
    rom = open(args.rom, "rb").read() if os.path.exists(args.rom) else None
    if rom is None:
        print("no ROM at %s: random streams only" % args.rom)
    elif len(rom) != 0x300000:
        sys.exit("%s isn't a headerless 3 MB ROM" % args.rom)
    mu = make_emulator(assemble(), rom)
    rnd = random.Random(args.seed)
    total = bad = skipped = 0

    def check(name, stream, src_addr, init):
        nonlocal total, bad, skipped
        want = bytearray(init)
        try:
            n = reference(stream + bytes(4), want)
        except (IndexError, Overrun):
            skipped += 1
            return
        got, end = run_gba(mu, src_addr, init)
        total += 1
        if got != bytes(want) or end != n:
            bad += 1
            print("DIFFER:", name, "length", n, "vs", end)

    for t in range(args.random):
        s = random_stream(rnd)
        init = (bytes(rnd.randrange(256) for _ in range(BUF)) if t % 10 == 0
                else bytes([rnd.randrange(256)]) * BUF)
        mu.mem_write(SRC, s)
        check("random #%d %s" % (t, s.hex()[:60]), s, SRC, init)
    nrandom = total
    if rom:
        init = bytes((i * 7 + 3) & 255 for i in range(BUF))
        for p in rom_streams(rom):
            o = rom_offset(p)
            check("ROM $%06x" % p, rom[o:o + 0x10000], ROM_AT + o, init)
    print("%d streams (%d random, %d from the ROM), %d differ; %d skipped as not streams"
          % (total, nrandom, total - nrandom, bad, skipped))
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
