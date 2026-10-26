#!/usr/bin/env python3
# Prints the distinct colors in a PNG written by smrun, most common first.
import collections, struct, sys, zlib

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

w, h, rows = read(sys.argv[1])
c = collections.Counter(bytes(r[x * 3:x * 3 + 3]) for r in rows for x in range(w))
for col, n in c.most_common(8):
    print(col.hex(), n)
