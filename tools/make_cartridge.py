#!/usr/bin/env python3
"""Makes the playable GBA cartridge from the engine and your Super Metroid ROM.

    make_cartridge.py sm-engine.gba ROM sm.gba
    make_cartridge.py --check ROM
    make_cartridge.py --find ROM

ROM is your Super Metroid ROM, by any name, or a zip with it in, or a
directory: then it and the directories in it (two levels down) are searched
for the ROM, by its checksum. The second form only checks the ROM, and says
what is wrong with it; the third prints the file it is in.

The engine (the build's sm-engine.gba, or a release's) has none of the
game's data: this appends it, from your ROM, as gba/cart_data.h describes.
The ROM must be the Japan/USA release, "Super Metroid (Japan, USA) (En,Ja)"
(sha1 da957f0d63d14cb441d215462904c4fa8519c613), with or without a copier's
512-byte header (which is dropped).

Besides the ROM itself, the data has the game's audio samples, decoded from
the SNES's BRR to the 8-bit PCM the GBA plays (gba/audio.c), and the menu
fonts' tiles redrawn for the squashed screens (tools/squash_font.py).
"""
import hashlib
import os
import struct
import sys
import zipfile

sys.dont_write_bytecode = True  # (no tools/__pycache__ for squash_font)
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
try:
    import squash_font  # noqa: E402
except ImportError:
    sys.exit('squash_font.py is missing: keep it next to make_cartridge.py '
             '(a release has both)')

SNES_ROM_SHA1 = 'da957f0d63d14cb441d215462904c4fa8519c613'
SNES_ROM_SIZE = 0x300000
MAGIC = b'SMGBA\0\0\0'
VERSION = 3
HEADER_SIZE = 256
COPIER_HEADER_SIZE = 512
ROM_SIZES = (SNES_ROM_SIZE, SNES_ROM_SIZE + COPIER_HEADER_SIZE)
GBA_MAX_SIZE = 32 << 20

# The SNES addresses of the game's uploads to the APU: the first is the
# sound driver with its common samples, the others the music for each
# part of the game (kMusicPointers, 0x8FE7E1, which also starts with the
# first).
MUSIC_POINTERS = 0x8FE7E1
NUM_MUSIC_BANKS = 25
# Where the driver keeps the sample directory (DSP register DIR).
SAMPLE_DIRECTORY = 0x6D00
NUM_SAMPLES = 64


def rom_offset(addr):
    """The offset in the ROM of a LoROM address."""
    return ((addr >> 16 & 0x7f) << 15 | addr & 0x7fff) % SNES_ROM_SIZE


def upload_blocks(rom, addr):
    """The blocks an upload writes: (APU address, length, offset in the ROM).

    An upload is a list of blocks (length, address, the bytes), ending with
    a length of 0. It reads the ROM in order, which in LoROM is in order in
    the file too.
    """
    o = rom_offset(addr)
    blocks = []
    while True:
        length, dest = struct.unpack_from('<HH', rom, o)
        if not length:
            return blocks
        blocks.append((dest, length, o + 4))
        o += 4 + length


def decode_brr(rom, o):
    """Decodes a BRR sample from the ROM at o, as src/snes/dsp.c does.

    Returns the samples (15-bit) and whether it loops.
    """
    out = []
    old = older = 0
    while o + 9 <= len(rom) and len(out) < 0x20000:
        header = rom[o]
        shift, filt = header >> 4, header >> 2 & 3
        for i in range(16):
            byte = rom[o + 1 + i // 2]
            s = byte >> 4 if i % 2 == 0 else byte & 0xf
            if s > 7:
                s -= 16
            s = (s << shift) >> 1 if shift <= 12 else (s >> 3) << 12
            if filt == 1:
                s += old + (-old >> 4)
            elif filt == 2:
                s += 2 * old + ((3 * -old) >> 5) - older + (older >> 4)
            elif filt == 3:
                s += 2 * old + ((13 * -old) >> 6) - older + ((3 * older) >> 4)
            s = max(-0x8000, min(0x7fff, s))
            s = ((s & 0x7fff) << 1)
            s = (s - 0x10000 if s & 0x8000 else s) >> 1
            older, old = old, s
            out.append(s)
        o += 9
        if header & 1:
            return out, bool(header & 2)
    return out, False


def audio_samples(rom):
    """The samples the game's uploads put in the sample directory, decoded.

    Returns {offset of the BRR in the ROM: (PCM bytes, loops)}.
    """
    pointers = rom_offset(MUSIC_POINTERS)
    banks = [int.from_bytes(rom[pointers + 3 * i:pointers + 3 * i + 3], 'little')
             for i in range(NUM_MUSIC_BANKS)]
    base = upload_blocks(rom, banks[0])
    samples = {}
    for bank in banks:
        blocks = base + (upload_blocks(rom, bank) if bank != banks[0] else [])

        def where(apu_addr):
            for dest, length, o in reversed(blocks):
                if dest <= apu_addr < dest + length:
                    return o + apu_addr - dest
            return None

        for i in range(NUM_SAMPLES):
            entry = where(SAMPLE_DIRECTORY + 4 * i)
            if entry is None:
                continue
            start = struct.unpack_from('<H', rom, entry)[0]
            o = where(start)
            if o is None or o in samples:
                continue
            pcm, loops = decode_brr(rom, o)
            samples[o] = (bytes((s >> 7) & 0xff for s in pcm), loops)
    return samples


def audio_section(rom):
    """The audio samples, as gba/cart_data.h's AudioSample table and PCM."""
    samples = audio_samples(rom)
    table = struct.pack('<I', len(samples))
    pcm = b''
    pcm_start = 4 + 16 * len(samples)
    for o in sorted(samples):
        data, loops = samples[o]
        table += struct.pack('<IIII', o, pcm_start + len(pcm), len(data), loops)
        pcm += data
        pcm += bytes(-len(pcm) % 4)
    return table + pcm


def squash_hash(data, bpp):
    """gba/cart_data.h's SquashHash of a tile's bytes."""
    h = bpp
    for w in struct.unpack('<%dH' % (bpp * 4), data[:bpp * 8]):
        h = ((h ^ w) * 0x9E3779B1) & 0xffffffff
    return h


def squash_section(rom):
    """The redrawn font tiles, as gba/cart_data.h's SquashHeader describes."""
    tiles = squash_font.tuned_tiles(rom, rom_offset)
    bits = 4
    while (1 << bits) < 4 * len(tiles):
        bits += 1
    slots = [0] * (1 << bits)
    entries = b''
    for n, (bpp, src, dst) in enumerate(tiles):
        i = squash_hash(src, bpp) >> (32 - bits)
        while slots[i]:
            i = (i + 1) & ((1 << bits) - 1)
        slots[i] = n + 1
        entries += struct.pack('<I16H', bpp, *struct.unpack('<%dH' % (bpp * 4), src),
                               *[0] * (16 - bpp * 4)) + dst
    return (struct.pack('<II', len(tiles), bits) + struct.pack('<%dH' % len(slots), *slots) +
            entries)


def rom_entries(path):
    """The ROMs a file may hold: (what to call it, its bytes), the file's own, or a zip's
    members of a ROM's size."""
    with open(path, 'rb') as f:
        head = f.read(4)
        if head != b'PK\x03\x04':
            return [(path, head + f.read())]
    try:
        with zipfile.ZipFile(path) as z:
            return [(f'{path} ({i.filename})', z.read(i)) for i in z.infolist()
                    if i.file_size in ROM_SIZES]
    except (zipfile.BadZipFile, OSError, RuntimeError, NotImplementedError):
        return []


def why_not(rom):
    """What is wrong with ROM bytes, or None: the right ROM."""
    if len(rom) == SNES_ROM_SIZE + COPIER_HEADER_SIZE:
        rom = rom[COPIER_HEADER_SIZE:]
    if len(rom) != SNES_ROM_SIZE:
        return (f'its size is {len(rom)} bytes, not {SNES_ROM_SIZE} (3 MB): another game, '
                'a hack or a bad dump')
    have = hashlib.sha1(rom).hexdigest()
    if have != SNES_ROM_SHA1:
        return f'its sha1 is {have}: a hack, another release or a bad dump'
    return None


def find_rom(path):
    """The ROM at path (a file, a zip or a directory to search), as (where it is: a file,
    what to call it, its bytes without a copier's header); or exits saying what's wrong."""
    if not os.path.exists(path):
        sys.exit(f'{path}: no such file or directory. Give the path of your Super Metroid ROM '
                 '(any name), or of a directory with it in.')
    if os.path.isdir(path):
        return search_dir(path)
    try:
        entries = rom_entries(path)
    except OSError as e:
        sys.exit(f"{path}: can't be read ({e.strerror})")
    for name, rom in entries:
        if why_not(rom) is None:
            return path, name, strip_header(name, rom)
    if not entries:
        sys.exit(f'{path}: a zip archive with no ROM of the right size in it '
                 f'({SNES_ROM_SIZE} bytes, or {SNES_ROM_SIZE + COPIER_HEADER_SIZE} with a '
                 'copier header)')
    why = [f'{name}: {why_not(rom)}' if len(entries) > 1 else why_not(rom)
           for name, rom in entries]
    why.append('this port is made for the Japan/USA release (En,Ja)')
    sys.exit(f'{path}: not the Super Metroid ROM this needs (sha1 {SNES_ROM_SHA1}):\n  ' +
             '\n  '.join(why))


def search_dir(top):
    """The ROM in directory top, or in the directories in it, two levels down (but hidden
    ones): the files of a ROM's size, and zips, by their checksum."""
    near = []
    for root, dirs, files in os.walk(top):
        depth = os.path.relpath(root, top).count(os.sep) + (root != top)
        dirs[:] = sorted(d for d in dirs if not d.startswith('.')) if depth < 2 else []
        for name in sorted(files):
            path = os.path.join(root, name)
            try:
                size = os.path.getsize(path)
                if size not in ROM_SIZES and not (name.lower().endswith('.zip') and
                                                  size < 64 << 20):
                    continue
                entries = rom_entries(path)
            except OSError:
                continue
            for what, rom in entries:
                why = why_not(rom)
                if why is None:
                    print(f'{top}: the ROM is {what}', file=sys.stderr)
                    return path, what, strip_header(what, rom)
                near.append(f'{what}: {why}')
    msg = f'{top}: no Super Metroid ROM (sha1 {SNES_ROM_SHA1}) in it or the directories in it'
    if near:
        msg += ' (these are a ROM\'s size, but not it):\n  ' + '\n  '.join(near[:8])
        if len(near) > 8:
            msg += f'\n  and {len(near) - 8} more'
    sys.exit(msg)


def strip_header(name, rom):
    """The ROM without a copier's 512-byte header, if it has one."""
    if len(rom) == SNES_ROM_SIZE + COPIER_HEADER_SIZE:
        print(f'{name}: dropping its {COPIER_HEADER_SIZE}-byte copier header', file=sys.stderr)
        rom = rom[COPIER_HEADER_SIZE:]
    return rom


def check_engine(path, engine):
    """Exits if the engine image can't be one this makes a cartridge from."""
    if len(engine) < 0x1000 or engine[0xB2] != 0x96:
        sys.exit(f"{path}: not a GBA image (the header's fixed byte, 0x96, is not at 0xB2). "
                 "Give sm-engine.gba (the build's, or a release's).")
    # The data starts at a multiple of 256 (gba.ld), with the magic, the
    # version and the ROM's size; the engine has the magic elsewhere too, as
    # a string it compares with, and that can fall on a multiple of 256.
    rom_size = struct.pack('<I', SNES_ROM_SIZE)
    if any(engine[o:o + 8] == MAGIC and engine[o + 12:o + 16] == rom_size
           for o in range(0x1000, len(engine), 256)):
        sys.exit(f"{path}: already has the game's data in it. Give sm-engine.gba, not sm.gba.")
    if len(engine) % 256:
        sys.exit(f'{path}: not an engine image (its size is not a multiple of 256)')


def main():
    args = sys.argv[1:]
    if len(args) == 2 and args[0] == '--check':
        _, name, _ = find_rom(args[1])
        print(f'{name}: the right ROM')
        return
    if len(args) == 2 and args[0] == '--find':
        print(find_rom(args[1])[0])
        return
    if len(args) != 3 or args[0].startswith('-'):
        sys.exit(__doc__.strip())
    engine_path, rom_path, out_path = args
    try:
        with open(engine_path, 'rb') as f:
            engine = f.read()
    except OSError as e:
        sys.exit(f"{engine_path}: can't be read ({e.strerror}). Build the engine first "
                 '(build-gba.sh does) or take sm-engine.gba from a release.')
    check_engine(engine_path, engine)
    rom_file, _, rom = find_rom(rom_path)
    if os.path.abspath(out_path) in (os.path.abspath(engine_path), os.path.abspath(rom_file)):
        sys.exit(f'{out_path}: the output would overwrite an input; name another file')
    audio = audio_section(rom)
    audio_offset = HEADER_SIZE + len(rom)
    squash = squash_section(rom)
    squash_offset = audio_offset + len(audio)
    header = MAGIC + struct.pack('<IIIIII', VERSION, len(rom), audio_offset, len(audio),
                                 squash_offset, len(squash))
    header += bytes(HEADER_SIZE - len(header))
    image = engine + header + rom + audio + squash
    if len(image) > GBA_MAX_SIZE:
        sys.exit(f"the cartridge would be {len(image)} bytes, more than a GBA's 32 MB")
    tmp = out_path + '.tmp'  # (no half-written cartridge if this stops)
    try:
        with open(tmp, 'wb') as f:
            f.write(image)
        os.replace(tmp, out_path)
    except OSError as e:
        sys.exit(f"{out_path}: can't be written ({e.strerror})")
    print(f'{out_path}: {len(image)} bytes (engine {len(engine)}, ROM {len(rom)}, '
          f'audio {len(audio)}, fonts {len(squash)}); data version {VERSION}')


if __name__ == '__main__':
    main()
