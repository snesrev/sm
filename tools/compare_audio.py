#!/usr/bin/env python3
"""Compares two recordings of the same music: the GBA's (smrun's AUDIO) and
the SNES reference (tools/spcref.c).

    compare_audio.py GBA.wav REFERENCE.wav

Aligns them by their loudness over time, then prints how well that and
their spectra match, and their levels.
"""
import struct
import sys

import numpy as np


def read_wav(path):
    d = open(path, 'rb').read()
    rate = struct.unpack_from('<I', d, 24)[0]
    pcm = np.frombuffer(d[44:], dtype='<i2').reshape(-1, 2).astype(np.float64)
    return rate, pcm.mean(axis=1)


def envelope(x, rate, ms=20):
    n = int(rate * ms / 1000)
    x = x[:len(x) // n * n].reshape(-1, n)
    return np.sqrt((x ** 2).mean(axis=1))


def spectrum(x, rate, size=4096):
    frames = len(x) // size
    w = np.hanning(size)
    s = np.zeros(size // 2 + 1)
    for i in range(frames):
        s += np.abs(np.fft.rfft(x[i * size:(i + 1) * size] * w))
    freqs = np.fft.rfftfreq(size, 1 / rate)
    return freqs, s / max(frames, 1)


def main():
    rg, g = read_wav(sys.argv[1])
    rr, r = read_wav(sys.argv[2])
    eg, er = envelope(g, rg), envelope(r, rr)
    eg_n = (eg - eg.mean()) / (eg.std() + 1e-9)
    er_n = (er - er.mean()) / (er.std() + 1e-9)
    best = (-2, 0)
    for lag in range(-200, 200):
        a = eg_n[max(lag, 0):]
        b = er_n[max(-lag, 0):]
        n = min(len(a), len(b))
        if n > 100:
            c = float(np.dot(a[:n], b[:n]) / n)
            best = max(best, (c, lag))
    corr, lag = best
    print(f'loudness over time: correlation {corr:.3f} at a lag of {lag * 20} ms')
    # Spectra in octave bands, over the aligned stretch.
    fg, sg = spectrum(g[max(lag, 0) * rg // 50:], rg)
    fr, sr = spectrum(r[max(-lag, 0) * rr // 50:], rr)
    bands = [(50, 100), (100, 200), (200, 400), (400, 800), (800, 1600), (1600, 3200),
             (3200, 5000)]
    print('octave bands, GBA/SNES level (dB):')
    for lo, hi in bands:
        a = sg[(fg >= lo) & (fg < hi)].mean()
        b = sr[(fr >= lo) & (fr < hi)].mean()
        print(f'  {lo:5d}-{hi:5d} Hz: {20 * np.log10((a + 1e-9) / (b + 1e-9)):+6.1f}')
    print(f'RMS: GBA {np.sqrt((g ** 2).mean()):.0f}, SNES {np.sqrt((r ** 2).mean()):.0f}')


if __name__ == '__main__':
    main()
