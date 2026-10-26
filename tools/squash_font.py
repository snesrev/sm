"""Font tiles redrawn for the GBA port's squashed screens.

Outside gameplay the port squashes the SNES's 224 lines into the GBA's 160 by
dropping the lines that show rows 0 and 4 of the text layer's tiles, and
merging those rows into rows 1 and 3 as the tiles are converted (gba/render.iwram.c
SetSquashLines, gba/convert.iwram.c MergeSquashedRows). The merge keeps the
brighter pixel, which is right for strokes that run across a tile but not for a
glyph whose shape is in the rows it moves: the C of "COPY" has its lower
terminal in row 4 and its opening in row 3, so the merge closes it into an O.

Here each font's tiles are redrawn for that: from the 8 rows of a tile, two are
deleted, those that cost the glyphs the least (the rows to keep were chosen for
each font by counting the strokes each choice loses over all its glyphs, and are
written down in FONTS), and the other 6 are put in the rows the squash shows
(1, 2, 3, 5, 6, 7), the rows it drops left empty. What's left is a tile the
merge changes in no way. Every glyph of a font loses the same rows, so letters
side by side keep their baselines and bars (OVERRIDES has the few glyphs
that need their own rows).

make_cartridge.py puts the result in the cartridge (gba/cart_data.h, SquashFont):
the engine finds a tile it converts for a squashed screen in it by the tile's
bytes (gba/convert.iwram.c), and uses the redrawn one. It is made from the
player's ROM when the cartridge is: the tiles are the game's, so none of them
are in this repository, only the choice of rows is.
"""
import struct

# The rows of a tile the squash shows, in order.
SHOWN = (1, 2, 3, 5, 6, 7)

# The fonts, by the tiles' addresses in the SNES ROM (the game copies them to
# VRAM from there, as they are, for the screens that squash): (address, bits per
# pixel, first tile, last tile, the 6 rows kept of 8). Tiles that are all
# blank are left as they are.
FONTS = [
    # Menus (file select, options...): the big font in pairs of tiles (a
    # glyph's top and bottom halves: 6 rows in the top, below 2 empty ones,
    # and 6 and the shadow in the bottom, above 1), and the 8x8 one.
    (0x8E8000, 4, 0, 15, (2, 3, 4, 5, 6, 7)),
    (0x8E8000, 4, 16, 31, (0, 1, 2, 4, 5, 6)),
    (0x8E8000, 4, 32, 47, (2, 3, 4, 5, 6, 7)),
    (0x8E8000, 4, 48, 63, (0, 1, 2, 4, 5, 6)),
    (0x8E8000, 4, 96, 143, (0, 1, 3, 4, 5, 6)),
    (0xB6C000, 4, 96, 143, (0, 1, 3, 4, 5, 6)),
    # The pause screen's: its 8x8 capitals (on a background colour), the
    # labels (SELECT START, RESERVE TANK, TIME, BOOTS) and the equipment's
    # names.
    (0xB68000, 4, 48, 76, (0, 1, 3, 4, 5, 6)),
    (0xB68000, 4, 112, 119, (0, 1, 3, 4, 5, 6)),
    (0xB68000, 4, 128, 135, (1, 2, 3, 4, 5, 6)),
    (0xB68000, 4, 144, 147, (0, 1, 3, 4, 5, 6)),
    (0xB68000, 4, 160, 166, (0, 1, 2, 3, 5, 6)),
    (0xB68000, 4, 208, 268, (1, 2, 3, 4, 5, 6)),
    (0xB68000, 4, 271, 303, (1, 2, 3, 4, 5, 6)),
    # BG3's 2bpp tiles (the HUD's digits and letters, shown in the pause
    # screen; the controller screen's text).
    (0x9AB200, 2, 0, 14, (1, 2, 3, 4, 5, 6)),
    (0x9AB200, 2, 60, 69, (0, 1, 2, 4, 5, 6)),
    (0x9AB200, 2, 176, 199, (0, 1, 2, 3, 4, 5)),
    (0x9AB200, 2, 224, 255, (0, 1, 2, 3, 4, 5)),
    # The intro's text (white, in 2bpp): the 16-line font as above, in
    # compressed tiles. And the digits and punctuation before the letters.
    (0x95D089, 2, 32, 40, (1, 2, 3, 4, 5, 6)),
    (0x95D089, 2, 48, 63, (2, 3, 4, 5, 6, 7)),
    (0x95D089, 2, 64, 79, (0, 1, 2, 4, 5, 6)),
    (0x95D089, 2, 80, 95, (2, 3, 4, 5, 6, 7)),
    (0x95D089, 2, 96, 111, (0, 1, 2, 4, 5, 6)),
]

# Glyphs the rows of their font would spoil, with the rows to keep instead
# ((address, tile): rows): the colon's two dots are in rows 2 and 5.
OVERRIDES = {
    (0x8E8000, 140): (0, 1, 2, 3, 5, 6),
    (0xB6C000, 140): (0, 1, 2, 3, 5, 6),
}

# The fonts above at these addresses are compressed (the game's DecompressToMem
# reads them, to RAM, from where it copies them to VRAM).
COMPRESSED = {0x95D089}


def decompress(rom, rom_offset, addr):
    """The game's decompressor (src/sm_80.c's DecompressToMem), on the stream at |addr|."""
    src = rom_offset(addr)
    out = bytearray()

    def next_byte():
        nonlocal src
        src += 1
        return rom[src - 1]

    while True:
        b = next_byte()
        if b == 0xFF:
            return bytes(out)
        if b & 0xE0 == 0xE0:
            cmd = 8 * b & 0xE0
            length = ((b & 3) << 8 | next_byte()) + 1
        else:
            cmd = b & 0xE0
            length = (b & 0x1F) + 1
        if cmd & 0x80:
            invert = 0xFF if cmd & 0x20 else 0
            if cmd >= 0xC0:
                pos = len(out) - next_byte()
            else:
                pos = next_byte()
                pos += next_byte() * 256
            for _ in range(length):
                out.append(out[pos] ^ invert)
                pos += 1
        elif cmd == 0x20:
            out += bytes([next_byte()]) * length
        elif cmd == 0x40:
            pair = (next_byte(), next_byte())
            out += bytes(pair[i & 1] for i in range(length))
        elif cmd == 0x60:
            first = next_byte()
            out += bytes((first + i) & 0xFF for i in range(length))
        else:
            for _ in range(length):
                out.append(next_byte())


def tile_pixels(data, bpp):
    """The 8x8 pixels of a SNES tile (bytes: rows of bitplane pairs)."""
    rows = []
    for r in range(8):
        planes = [data[r * 2], data[r * 2 + 1]]
        if bpp == 4:
            planes += [data[16 + r * 2], data[17 + r * 2]]
        rows.append([sum((planes[k] >> (7 - x) & 1) << k for k in range(bpp))
                     for x in range(8)])
    return rows


def gba_words(rows):
    """The GBA's 4bpp form of rows of pixels: a word a row, the leftmost pixel in the low nibble."""
    return b''.join(struct.pack('<I', sum(p << (4 * x) for x, p in enumerate(row))) for row in rows)


def redraw(rows, keep):
    """The rows of a tile with those not in |keep| deleted, put where the squash shows them."""
    out = [[0] * 8 for _ in range(8)]
    for dst, src in zip(SHOWN, keep):
        out[dst] = rows[src]
    return out


def tuned_tiles(rom, rom_offset):
    """The redrawn tiles: [(bits per pixel, the SNES tile's bytes, the GBA tile's 32 bytes)].

    In the order of FONTS, a tile once: the same tile in two places (the menu's
    8x8 font is in the pause screen's tiles too) is redrawn by the first.
    """
    seen = {}
    for addr, bpp, first, last, keep in FONTS:
        size = 8 * bpp
        if addr in COMPRESSED:
            source, base = decompress(rom, rom_offset, addr), 0
        else:
            source, base = rom, rom_offset(addr)
        for i in range(first, last + 1):
            data = bytes(source[base + i * size:base + (i + 1) * size])
            if not any(data) or (bpp, data) in seen:
                continue
            rows = redraw(tile_pixels(data, bpp), OVERRIDES.get((addr, i), keep))
            seen[(bpp, data)] = gba_words(rows)
    return [(bpp, data, tuned) for (bpp, data), tuned in seen.items()]
