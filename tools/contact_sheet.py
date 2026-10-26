#!/usr/bin/env python3
# Tiles PNG screenshots (as smrun writes them) into one PNG, 4 to a row:
#     contact_sheet.py OUT.png IN1.png IN2.png ...
import struct, sys, zlib

def read(path):
    d = open(path, 'rb').read()
    pos, idat, w, h = 8, b'', 0, 0
    while pos < len(d):
        n, = struct.unpack('>I', d[pos:pos + 4])
        t = d[pos + 4:pos + 8]
        if t == b'IHDR':
            w, h = struct.unpack('>II', d[pos + 8:pos + 16])
        elif t == b'IDAT':
            idat += d[pos + 8:pos + 8 + n]
        pos += 12 + n
    raw = zlib.decompress(idat)
    stride = w * 3 + 1
    return w, h, [raw[y * stride + 1:(y + 1) * stride] for y in range(h)]

def chunk(t, d):
    return struct.pack('>I', len(d)) + t + d + struct.pack('>I', zlib.crc32(t + d))

imgs = [read(p) for p in sys.argv[2:]]
w, h = imgs[0][0], imgs[0][1]
cols = min(4, len(imgs))
nrows = (len(imgs) + cols - 1) // cols
gap = b'\xff\xff\xff' * 2
lines = []
for r in range(nrows):
    for y in range(h):
        line = b''
        for c in range(cols):
            i = r * cols + c
            line += (bytes(imgs[i][2][y]) if i < len(imgs) else b'\0' * (w * 3)) + gap
        lines.append(line)
    lines.append(gap * (len(lines[-1]) // 6))
W = len(lines[0]) // 3
raw = b''.join(b'\0' + l for l in lines)
png = (b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', W, len(lines), 8, 2, 0, 0, 0)) +
       chunk(b'IDAT', zlib.compress(raw)) + chunk(b'IEND', b''))
open(sys.argv[1], 'wb').write(png)
