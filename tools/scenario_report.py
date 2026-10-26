#!/usr/bin/env python3
"""Reports on a run of gba/scenarios.c's scenarios (tools/scenarios.sh runs
this after smrun):

    scenario_report.py OUT_DIR SNESREF SYMS [SPCREF ROM]

OUT_DIR has what the run left: raw/*.log (smrun's, one per boot), shots/
(smrun's SHOT_ON output: NAME-N.png and its memory dumps) and, for sound,
raw/*.wav (the GBA's). It writes:

  compare/NAME-N.png  the GBA's picture, the SNES's (tools/snesref.c, from the
                      same memory, cropped as the GBA's is) and where they
                      differ, in red (tools/room_compare.py)
  summary.txt         a line per scenario: its shots' worst and mean share of
                      pixels that differ, its speed (frames per second: the
                      GBA's screen runs at 59.73; the slowest and mean of the
                      scenario's 60-frame windows), the game's cycles a
                      frame, and, with SPCREF (tools/spcref.c, built) and the
                      ROM, its sound against what the SNES's driver would
                      play (tools/compare_audio.py: the loudness correlation
                      and the levels), in audio/NAME.txt.
Needs Pillow and NumPy.
"""
import glob
import multiprocessing
import os
import re
import struct
import subprocess
import sys

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import room_compare  # noqa: E402

kGbaHz = 59.7275  # the GBA's frames a second
kLead = 0.5       # seconds of sound before a scenario starts, to compare too


def parse_logs(out):
    """Each scenario's, from the logs: its name, the frames (smrun's) it starts
    and ends at, and its speed windows' vblanks and cycles."""
    scenarios = []
    for path in sorted(glob.glob(os.path.join(out, 'raw', '*.log'))):
        cur = None
        for line in open(path, errors='replace'):
            m = re.match(r'\[(\d+)\] (.*)', line)
            if not m:
                continue
            frame, text = int(m.group(1)), m.group(2)
            if text.startswith('scenario: end '):
                if cur:
                    cur['end'] = frame
                    scenarios.append(cur)
                cur = None
            elif text.startswith('scenario: ') and not re.match(r'scenario: (\d+ to run|all done)', text):
                cur = {'name': text[10:], 'start': frame, 'log': path, 'vblanks': [],
                       'game': [], 'end': frame}
            elif text.startswith('scenario-at:') and cur:
                cur['at'] = text[13:]
            elif text.startswith('sm: 60 frames in ') and cur:
                n = re.match(r'sm: 60 frames in (\d+) vblanks; cycles/frame: game (\d+), audio (\d+), render (\d+)', text)
                cur['vblanks'].append(int(n.group(1)))
                cur['game'].append((int(n.group(2)), int(n.group(3)), int(n.group(4))))
        if cur:
            cur['truncated'] = True
            scenarios.append(cur)
    return scenarios


def compare_one(args):
    snesref, syms_path, s, shot, out = args
    label = os.path.basename(shot)[:-4]
    try:
        frac = room_compare.compare(snesref, syms_path, s, shot,
                                    os.path.join(out, 'compare', f'ref-{label}.png'),
                                    os.path.join(out, 'compare', f'{label}.png'))
    except Exception as e:  # a bad dump: say so, go on
        print(f'{label}: {e}', file=sys.stderr)
        return label, None
    os.remove(os.path.join(out, 'compare', f'ref-{label}.png'))
    return label, frac


def read_wav(path):
    d = open(path, 'rb').read()
    rate = struct.unpack_from('<I', d, 24)[0]
    return rate, np.frombuffer(d[44:], dtype='<i2').reshape(-1, 2)


def write_wav(path, rate, pcm):
    data = pcm.astype('<i2').tobytes()
    with open(path, 'wb') as f:
        f.write(b'RIFF' + struct.pack('<I', 36 + len(data)) + b'WAVEfmt ')
        f.write(struct.pack('<IHHIIHH', 16, 1, 2, rate, rate * 4, 4, 16))
        f.write(b'data' + struct.pack('<I', len(data)) + data)


def audio_one(args):
    """The sound of one scenario, run on its own (smrun's raw/NAME.wav and
    .log, with the game's requests in it), against the driver's."""
    out, spcref, rom, sc = args
    name = sc['name']
    wav = os.path.join(out, 'raw', f'{name}.wav')
    if not os.path.exists(wav):
        return name, None
    os.makedirs(os.path.join(out, 'audio'), exist_ok=True)
    # The game's frame (which the log's audio lines count) at the scenario's
    # start, from the audio line nearest it: the game runs a frame a frame.
    pairs = [(int(m.group(1)), int(m.group(2))) for m in
             (re.match(r'\[(\d+)\] audio: (\d+) ', l) for l in open(sc['log'], errors='replace')) if m]
    near = min(pairs, key=lambda p: abs(p[0] - sc['start']))
    game_start = sc['start'] - (near[0] - near[1])
    game_end = game_start + (sc['end'] - sc['start']) + 60
    ref = os.path.join(out, 'audio', f'{name}.ref.wav')
    subprocess.run([spcref, rom, sc['log'], str(game_end), ref], check=True)
    rg, g = read_wav(wav)
    rr, r = read_wav(ref)
    lead = int(kLead * kGbaHz)
    g0 = int(max(sc['start'] - lead, 0) / kGbaHz * rg)
    g1 = int((sc['end'] + 30) / kGbaHz * rg)
    r0 = int(max(game_start - lead, 0) * 534)
    r1 = game_end * 534
    write_wav(os.path.join(out, 'audio', f'{name}.gba.wav'), rg, g[g0:g1])
    write_wav(os.path.join(out, 'audio', f'{name}.snes.wav'), rr, r[r0:r1])
    os.remove(ref)
    here = os.path.dirname(os.path.abspath(__file__))
    res = subprocess.run([sys.executable, os.path.join(here, 'compare_audio.py'),
                          os.path.join(out, 'audio', f'{name}.gba.wav'),
                          os.path.join(out, 'audio', f'{name}.snes.wav')],
                         capture_output=True, text=True)
    open(os.path.join(out, 'audio', f'{name}.txt'), 'w').write(res.stdout + res.stderr)
    m = re.search(r'correlation (-?[\d.]+) at a lag of (-?\d+) ms', res.stdout)
    rms = re.search(r'RMS: GBA (\d+), SNES (\d+)', res.stdout)
    if not m or not rms:
        return name, 'n/a'
    return name, f'{m.group(1)} lag {m.group(2)}ms, RMS GBA/SNES {int(rms.group(1)) / max(int(rms.group(2)), 1):.2f}'


def main():
    out, snesref, syms_path = sys.argv[1:4]
    jobs = int(os.environ.get('JOBS', os.cpu_count() or 1))
    os.makedirs(os.path.join(out, 'compare'), exist_ok=True)
    s = room_compare.syms(syms_path)
    shots = sorted(glob.glob(os.path.join(out, 'shots', '*.png')))
    with multiprocessing.Pool(jobs) as pool:
        results = dict(pool.map(compare_one, [(snesref, syms_path, s, p, out) for p in shots]))
        scenarios = parse_logs(out)
        # In the list's order (smrun's boots ran in parallel).
        names = [l.split(' ', 1)[1].strip() for l in open(os.path.join(out, 'list.txt'))]
        scenarios.sort(key=lambda sc: names.index(sc['name']) if sc['name'] in names else len(names))
        audio = {}
        if len(sys.argv) > 5:
            audio = dict(pool.map(audio_one, [(out, sys.argv[4], sys.argv[5], sc) for sc in scenarios]))
    with open(os.path.join(out, 'compare.txt'), 'w') as f:
        for label, frac in sorted(results.items(), key=lambda kv: -(kv[1] or 0)):
            f.write(f'{label} ' + ('squashed' if frac is None else f'{100 * frac:.1f}%') + '\n')
    lines = []
    head = f'{"scenario":28} {"shots":>5} {"worst%":>7} {"mean%":>6} {"fps min":>7} {"fps avg":>7} {"game cyc":>9}'
    lines.append(head + ('  sound (correlation, RMS)' if audio else ''))
    for sc in scenarios:
        name = sc['name']
        fr = [v for k, v in results.items() if re.fullmatch(re.escape(name) + r'-\d+', k) and v is not None]
        # The windows that begin before the script are the spawn's.
        v = sc['vblanks'][1:] or sc['vblanks']
        gcyc = [g[0] for g in sc['game'][1:] or sc['game']]
        fps = [60 * kGbaHz / n for n in v]
        line = f'{name:28} {len(fr):5d} '
        line += f'{100 * max(fr):7.1f} {100 * sum(fr) / len(fr):6.1f} ' if fr else f'{"-":>7} {"-":>6} '
        line += f'{min(fps):7.1f} {sum(fps) / len(fps):7.1f} {sum(gcyc) // len(gcyc):9d}' if fps else f'{"-":>7} {"-":>7} {"-":>9}'
        if sc.get('truncated'):
            line += '  (cut short)'
        if audio:
            line += '  ' + str(audio.get(name))
        lines.append(line)
    open(os.path.join(out, 'summary.txt'), 'w').write('\n'.join(lines) + '\n')
    print('\n'.join(lines))
    squashed = [k for k, v in results.items() if v is None]
    if squashed:
        print(f'{len(squashed)} shots on squashed screens (not compared): see compare.txt')


if __name__ == '__main__':
    main()
