#!/usr/bin/env python3
"""Makes and applies BPS patches (the format of Floating IPS, beat and
RomPatcher.js), and measures what a patch from the ROM to the cartridge holds.

    make_bps.py make SOURCE TARGET PATCH.bps   encode the patch
    make_bps.py apply SOURCE PATCH.bps OUT     decode it (checks all CRCs)
    make_bps.py stats SOURCE TARGET            what the patch would hold, by section

A developer tool. The cartridge (tools/make_cartridge.py) is built on the
player's machine because the ROM can't be distributed; a patch from sm.smc to
sm.gba would carry the engine (ours) and whatever of the cartridge is not a
copy of the ROM, which is derived from it (the decoded audio, the redrawn
fonts). Whether to publish one is not decided: nothing here puts one in CI or
in a release. `stats` says how much there would be to publish.

The encoder works greedily: it finds the longest run in the source (SourceCopy,
or SourceRead where it is at the same offset) or earlier in the target
(TargetCopy) that matches, and the rest is TargetRead. It indexes the source
at every 4th byte by the 16 bytes there, so it finds runs of 19 bytes or more.
"""
import struct
import sys
import zlib

KEY = 16        # bytes hashed to find a match
STRIDE = 4      # the source is indexed at every STRIDE-th offset
MIN_MATCH = 8   # a shorter match costs more as an action than as literals

SOURCE_READ, TARGET_READ, SOURCE_COPY, TARGET_COPY = range(4)


def encode_number(n):
    out = bytearray()
    while True:
        x = n & 0x7f
        n >>= 7
        if n == 0:
            out.append(0x80 | x)
            return bytes(out)
        out.append(x)
        n -= 1


def decode_number(data, pos):
    n = 0
    shift = 1
    while True:
        x = data[pos]
        pos += 1
        n += (x & 0x7f) * shift
        if x & 0x80:
            return n, pos
        shift <<= 7
        n += shift


def match_length(a, i, b, j, limit):
    """How many bytes a[i:] and b[j:] have in common, up to limit."""
    n = 0
    step = 4096
    while n < limit:
        m = min(step, limit - n)
        if a[i + n:i + n + m] == b[j + n:j + n + m]:
            n += m
            continue
        while a[i + n] == b[j + n]:  # (m > 0 here, and the slices differ)
            n += 1
        return n
    return limit


def diff(source, target):
    """Returns the actions as a list of (kind, target offset, length, source/target offset)."""
    index = {}
    for p in range(0, len(source) - KEY + 1, STRIDE):
        index.setdefault(source[p:p + KEY], p)
    tindex = {}     # the same for the target so far, as TargetCopy sources
    tindexed = 0    # target offsets below this are indexed
    actions = []
    lit = 0         # the start of the literals not yet written
    t = 0
    n = len(target)
    while t < n:
        key = target[t:t + KEY]
        best = (0, 0, 0)  # length, kind, offset
        if len(key) == KEY:
            p = index.get(key)
            if p is not None:
                best = (match_length(source, p, target, t, min(len(source) - p, n - t)),
                        SOURCE_COPY, p)
            # The target's own earlier bytes: indexed up to what is already behind t.
            while tindexed + KEY <= t:
                tindex.setdefault(target[tindexed:tindexed + KEY], tindexed)
                tindexed += STRIDE
            q = tindex.get(key)
            if q is not None:
                # (a TargetCopy may overlap its own output, so no limit but the end)
                ln = 0
                while t + ln < n and target[q + ln] == target[t + ln]:
                    ln += 1
                    if ln >= 1 << 20:
                        break
                if ln > best[0]:
                    best = (ln, TARGET_COPY, q)
        if best[0] < MIN_MATCH:
            t += 1
            continue
        length, kind, off = best
        # Take in the literals just before it that match too.
        back = 0
        src = source if kind == SOURCE_COPY else target
        while (t - back > lit and off - back > 0 and src[off - back - 1] == target[t - back - 1]):
            back += 1
        t -= back
        off -= back
        length += back
        if t > lit:
            actions.append((TARGET_READ, lit, t - lit, 0))
        if kind == SOURCE_COPY and off == t:
            kind = SOURCE_READ
        actions.append((kind, t, length, off))
        t += length
        lit = t
    if lit < n:
        actions.append((TARGET_READ, lit, n - lit, 0))
    return actions


def encode_actions(target, actions):
    out = bytearray()
    src_rel = tgt_rel = 0
    for kind, t, length, off in actions:
        out += encode_number((length - 1) << 2 | kind)
        if kind == TARGET_READ:
            out += target[t:t + length]
        elif kind == SOURCE_COPY:
            d = off - src_rel
            out += encode_number(abs(d) << 1 | (d < 0))
            src_rel = off + length
        elif kind == TARGET_COPY:
            d = off - tgt_rel
            out += encode_number(abs(d) << 1 | (d < 0))
            tgt_rel = off + length
    return bytes(out)


def make_patch(source, target, metadata=b''):
    body = (b'BPS1' + encode_number(len(source)) + encode_number(len(target)) +
            encode_number(len(metadata)) + metadata +
            encode_actions(target, diff(source, target)))
    body += struct.pack('<II', zlib.crc32(source), zlib.crc32(target))
    return body + struct.pack('<I', zlib.crc32(body))


def apply_patch(source, patch):
    """Decodes a BPS patch against source, as the format describes; exits on any mismatch."""
    if patch[:4] != b'BPS1':
        sys.exit('not a BPS patch')
    if zlib.crc32(patch[:-4]) != struct.unpack('<I', patch[-4:])[0]:
        sys.exit('the patch is damaged (its CRC32 is wrong)')
    src_crc, tgt_crc = struct.unpack('<II', patch[-12:-4])
    pos = 4
    src_size, pos = decode_number(patch, pos)
    tgt_size, pos = decode_number(patch, pos)
    meta_size, pos = decode_number(patch, pos)
    pos += meta_size
    if len(source) != src_size or zlib.crc32(source) != src_crc:
        sys.exit('this patch is for another source file (size or CRC32 differs)')
    out = bytearray()
    src_rel = tgt_rel = 0
    end = len(patch) - 12
    while pos < end:
        data, pos = decode_number(patch, pos)
        kind, length = data & 3, (data >> 2) + 1
        if kind == SOURCE_READ:
            out += source[len(out):len(out) + length]
        elif kind == TARGET_READ:
            out += patch[pos:pos + length]
            pos += length
        else:
            d, pos = decode_number(patch, pos)
            d = -(d >> 1) if d & 1 else d >> 1
            if kind == SOURCE_COPY:
                src_rel += d
                out += source[src_rel:src_rel + length]
                src_rel += length
            else:
                tgt_rel += d
                for _ in range(length):  # (may read what it writes)
                    out.append(out[tgt_rel])
                    tgt_rel += 1
    if len(out) != tgt_size or zlib.crc32(out) != tgt_crc:
        sys.exit('the result is not what the patch expects (size or CRC32 differs)')
    return bytes(out)


def sections(target):
    """(name, start, end) of the parts of a cartridge from make_cartridge.py."""
    magic = b'SMGBA\0\0\0'
    for o in range(0x1000, len(target) - 256, 256):
        if target[o:o + 8] == magic:
            _, rom_size, audio_off, audio_size, squash_off, squash_size = \
                struct.unpack_from('<IIIIII', target, o + 8)
            return [('engine', 0, o), ('cartridge header', o, o + 256),
                    ('ROM copy', o + 256, o + 256 + rom_size),
                    ('audio (decoded PCM)', o + audio_off, o + audio_off + audio_size),
                    ('font tiles (redrawn)', o + squash_off, o + squash_off + squash_size)]
    return [('target', 0, len(target))]


def stats(source, target):
    actions = diff(source, target)
    parts = sections(target)
    print(f'{"section":24} {"size":>9} {"copied":>9} {"repeated":>9} {"literal":>9}')
    total = {SOURCE_READ: 0, SOURCE_COPY: 0, TARGET_COPY: 0, TARGET_READ: 0}
    for name, lo, hi in parts:
        c = {SOURCE_READ: 0, SOURCE_COPY: 0, TARGET_COPY: 0, TARGET_READ: 0}
        for kind, t, length, _ in actions:
            n = min(t + length, hi) - max(t, lo)
            if n > 0:
                c[kind] += n
        for k in c:
            total[k] += c[k]
        print(f'{name:24} {hi - lo:9} {c[SOURCE_READ] + c[SOURCE_COPY]:9} '
              f'{c[TARGET_COPY]:9} {c[TARGET_READ]:9}')
    print(f'{"all":24} {len(target):9} {total[SOURCE_READ] + total[SOURCE_COPY]:9} '
          f'{total[TARGET_COPY]:9} {total[TARGET_READ]:9}')
    print('copied: from the source ROM; repeated: from earlier in the target; literal: in the patch.')
    print(f'The patch is {len(make_patch(source, target))} bytes, '
          f'{sum(1 for a in actions)} actions.')


def read(path):
    try:
        with open(path, 'rb') as f:
            return f.read()
    except OSError as e:
        sys.exit(f"{path}: can't be read ({e.strerror})")


def main():
    a = sys.argv[1:]
    if len(a) == 4 and a[0] == 'make':
        patch = make_patch(read(a[1]), read(a[2]))
        with open(a[3], 'wb') as f:
            f.write(patch)
        print(f'{a[3]}: {len(patch)} bytes')
    elif len(a) == 4 and a[0] == 'apply':
        out = apply_patch(read(a[1]), read(a[2]))
        with open(a[3], 'wb') as f:
            f.write(out)
        print(f'{a[3]}: {len(out)} bytes, CRC32 checked')
    elif len(a) == 3 and a[0] == 'stats':
        stats(read(a[1]), read(a[2]))
    else:
        sys.exit(__doc__.strip())


if __name__ == '__main__':
    main()
