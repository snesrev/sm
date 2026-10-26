// See audio.h.
//
// The SNES's sound driver runs on its sound CPU, with 64 KB of RAM, and
// plays through its DSP: eight voices of BRR samples, with envelopes,
// echo, noise, at 32 kHz. Here, the driver (src/spc_player.c) runs on
// the GBA's CPU, at its timer's 500 Hz. The GBA has no room for the RAM:
// all the driver reads was uploaded from the ROM, so it reads the ROM
// (SpcRam_Read). In place of the DSP, the driver's writes to its registers
// set up the GBA's mixer: its voices play the samples make_cartridge.py
// decoded, at 10512 Hz, with the DSP's envelopes and echo. No pitch
// modulation, and only samples played slower than half that rate are
// interpolated (audio_mix.S).
#include "audio.h"

#include <stdio.h>
#include <string.h>

#include "cart_data.h"
#include "gba_hw.h"
#include "sm_rtl.h"
#include "spc_player.h"

static SpcPlayer player;

// The APU's RAM: the blocks of the ROM the game uploaded to it, newest
// last.
typedef struct AramBlock {
  uint16_t dest, length;
  const uint8_t *src;
} AramBlock;
enum { kMaxAramBlocks = 32 };
static AramBlock aram[kMaxAramBlocks];
static int aram_count;
// The bytes the latest reads found, as far as no newer block covers them
// (the latest first; spc_player.h): the driver reads on from where it read,
// mostly, from a few places in turn (the music, the sound effects, its
// tables). (In IWRAM: the driver looks at the first of them for each byte.)
AramCache g_aram_cache[kAramCacheEntries] IWRAM_BSS;

static void AddAramBlock(uint16_t dest, uint16_t length, const uint8_t *src) {
  // Older blocks this one covers are gone.
  int n = 0;
  for (int i = 0; i < aram_count; i++) {
    if (aram[i].dest < dest || aram[i].dest + aram[i].length > dest + length)
      aram[n++] = aram[i];
  }
  if (n == kMaxAramBlocks) {
    memmove(aram, aram + 1, sizeof(aram) - sizeof(aram[0]));
    n--;
  }
  aram[n++] = (AramBlock){dest, length, src};
  aram_count = n;
  for (int i = 0; i < kAramCacheEntries; i++)
    g_aram_cache[i].length = 0;
}

static int FindAramBlockIndex(uint16_t addr) {
  for (int i = aram_count - 1; i >= 0; i--) {
    if ((uint16_t)(addr - aram[i].dest) < aram[i].length)
      return i;
  }
  return -1;
}

static const AramBlock *FindAramBlock(uint16_t addr) {
  int i = FindAramBlockIndex(addr);
  return i < 0 ? NULL : &aram[i];
}

void SpcRam_Upload(const uint8 *data) {
  for (;;) {
    uint16_t length = data[0] | data[1] << 8;
    if (!length)
      return;
    AddAramBlock(data[2] | data[3] << 8, length, data + 4);
    data += 4 + length;
  }
}

static uint8 SpcRam_ReadUncached(uint16 addr) {
  // From the newest block on: the newer ones that don't have |addr| bound
  // the bytes around it that the one that has it can give.
  int start = 0, end = 0x10000;
  for (int i = aram_count - 1; i >= 0; i--) {
    const AramBlock *b = &aram[i];
    int s = b->dest, e = s + b->length;
    if ((uint16_t)(addr - s) < b->length) {
      if (s > start)
        start = s;
      if (e < end)
        end = e;
      for (int j = kAramCacheEntries - 1; j > 0; j--)
        g_aram_cache[j] = g_aram_cache[j - 1];
      g_aram_cache[0] = (AramCache){start, end - start, b->src + (start - s)};
      return b->src[(uint16_t)(addr - s)];
    }
    if (e <= addr && e > start)
      start = e;
    else if (s > addr && s < end)
      end = s;
  }
  return 0;
}

// What SPC_RAM (spc_player.c) reads when the first of the cache has missed.
uint8 SpcRam_Read(uint16 addr) {
  for (int i = 0; i < kAramCacheEntries; i++) {
    uint16_t offset = addr - g_aram_cache[i].addr;
    if (offset < g_aram_cache[i].length) {
      AramCache hit = g_aram_cache[i];
      for (int j = i; j > 0; j--)
        g_aram_cache[j] = g_aram_cache[j - 1];
      g_aram_cache[0] = hit;
      return hit.src[offset];
    }
  }
  return SpcRam_ReadUncached(addr);
}

static uint16_t SpcRam_Read16(uint16_t addr) {
  return SpcRam_Read(addr) | SpcRam_Read(addr + 1) << 8;
}

// The DSP: its registers (src/spc_player.c looks at them to see what the
// driver changes), and each voice's envelope. (In EWRAM: the mixer's code
// needs IWRAM's room more.)
uint8_t g_audio_dsp[128];

enum { kAttack, kDecay, kSustain, kGain, kRelease };
typedef struct Envelope {
  uint8_t state;
  uint16_t level;  // 0-0x7FF
  uint16_t counter;
} Envelope;
static Envelope envelopes[8];
// In 8.8 fixed point: the DSP's samples (32 kHz) since the envelopes ran.
static uint32_t envelope_time;

// The DSP's samples between envelope steps, by rate.
static const uint16_t kRates[32] = {
  0, 2048, 1536, 1280, 1024, 768, 640, 512, 384, 320, 256, 192, 160, 128, 96, 80,
  64, 48, 40, 32, 24, 20, 16, 12, 10, 8, 6, 5, 4, 3, 2, 1,
};

static const AudioSample *FindSample(uint32_t rom_offset) {
  const uint32_t *audio = (const uint32_t *)((const uint8_t *)&__cart_data + __cart_data.audio_offset);
  const AudioSample *samples = (const AudioSample *)(audio + 1);
  int lo = 0, hi = (int)audio[0] - 1;
  while (lo <= hi) {
    int mid = (lo + hi) / 2;
    if (samples[mid].rom_offset == rom_offset)
      return &samples[mid];
    if (samples[mid].rom_offset < rom_offset)
      lo = mid + 1;
    else
      hi = mid - 1;
  }
  return NULL;
}

__attribute__((noinline)) static void KeyOn(int v) {
  AudioVoice *voice = &g_audio_voices[v];
  Envelope *e = &envelopes[v];
  uint16_t entry = (g_audio_dsp[0x5d] << 8) + g_audio_dsp[v * 16 + 4] * 4;
  uint16_t start = SpcRam_Read16(entry), loop = SpcRam_Read16(entry + 2);
  const AramBlock *b = FindAramBlock(start);
  const AudioSample *s = b ? FindSample(b->src + (uint16_t)(start - b->dest) - snes_rom) : NULL;
  voice->noise = (g_audio_dsp[0x3d] >> v) & 1;
  voice->echo = (g_audio_dsp[0x4d] >> v) & 1;
  if (!s && !voice->noise) {
    printf("audio: no sample at 0x%04x (srcn %u)\n", start, g_audio_dsp[v * 16 + 4]);
    voice->on = false;
    return;
  }
  if (s) {
    voice->pcm = (const int8_t *)&__cart_data + __cart_data.audio_offset + s->pcm_offset;
    voice->end = s->length << 16;
    // BRR is 16 samples in 9 bytes.
    uint32_t loop_start = (uint16_t)(loop - start) / 9 * 16;
    voice->loop_len = s->loops && loop_start < s->length ? (s->length - loop_start) << 16 : 0;
  }
  voice->pos = 0;
  voice->on = true;
  e->level = 0;
  e->counter = 0;
  e->state = (g_audio_dsp[v * 16 + 5] & 0x80) ? kAttack : kGain;
}

// The master and echo volumes, or none while the DSP's muted.
__attribute__((noinline)) static void UpdateVolumes(void) {
  bool mute = g_audio_dsp[0x6c] & 0x40;
  g_audio_mix.master_l = mute ? 0 : g_audio_dsp[0x0c];
  g_audio_mix.master_r = mute ? 0 : g_audio_dsp[0x1c];
  g_audio_mix.echo_l = mute ? 0 : g_audio_dsp[0x2c];
  g_audio_mix.echo_r = mute ? 0 : g_audio_dsp[0x3c];
}

// The registers the driver changes now and then, in ROM.
__attribute__((noinline)) static void DspWriteChanged(uint8_t reg, uint8_t value) {
  switch (reg) {
  case 0x4c:  // KON
    for (int i = 0; i < 8; i++) {
      if (value & (1 << i))
        KeyOn(i);
    }
    break;
  case 0x5c:  // KOF
    for (int i = 0; i < 8; i++) {
      if (value & (1 << i))
        envelopes[i].state = kRelease;
    }
    break;
  case 0x4d:  // EON
    for (int i = 0; i < 8; i++)
      g_audio_voices[i].echo = (value >> i) & 1;
    break;
  case 0x7d: {  // EDL
    unsigned len = (value & 0xf) * kAudioEchoSamples16ms / 2;
    g_audio_mix.echo_len = len ? len : 1;
    Audio_ClearEcho();
    break;
  }
  case 0x0d:  // EFB
    g_audio_mix.feedback = value;
    break;
  case 0x0f: case 0x1f: case 0x2f: case 0x3f:
  case 0x4f: case 0x5f: case 0x6f: case 0x7f: {  // FIR
    // Its taps, oldest first, at 32 kHz; here, at half the mixer's rate,
    // six of them a tap.
    const int8_t *c = (const int8_t *)&g_audio_dsp[0x0f];
    g_audio_mix.fir[0] = c[0x70] + c[0x60] + c[0x50] + c[0x40] + c[0x30] + c[0x20];
    g_audio_mix.fir[1] = c[0x10] + c[0x00];
    break;
  }
  case 0x0c: case 0x1c: case 0x2c: case 0x3c:  // MVOL, EVOL
    UpdateVolumes();
    break;
  case 0x6c:  // FLG: soft reset, mute, echo writes, noise rate
    g_audio_mix.echo_writes = !(value & 0x20);
    g_audio_noise_period = kRates[value & 0x1f];
    UpdateVolumes();
    if (value & 0x80) {
      for (int i = 0; i < 8; i++) {
        envelopes[i].state = kRelease;
        envelopes[i].level = 0;
      }
    }
    break;
  }
}

// The driver's write of |value| to |reg| (up to 0x7f), when it changes
// something: src/spc_player.c's Dsp_Write leaves out the rest, most of the
// 500 a second.
void Audio_DspWrite(uint8 reg, uint8 value) {
  g_audio_dsp[reg] = value;
  int v = reg >> 4;
  if ((reg & 0xe) == 0x2) {
    // 0x1000 is the DSP's 32 kHz: in the mixer's samples, 16.16.
    uint32_t pitch = (g_audio_dsp[v * 16 + 2] | g_audio_dsp[v * 16 + 3] << 8) & 0x3fff;
    g_audio_voices[v].step = pitch * (16 * kAudioDspSamples) >> 8;
    return;
  }
  DspWriteChanged(reg, value);
}

// One step of an envelope, as src/snes/dsp.c's dsp_handleGain: its level
// after |level|. |r| is its voice's registers.
static int EnvelopeStep(Envelope *e, const uint8_t *r, int level) {
  switch (e->state) {
  case kAttack:
    level += (r[5] & 0xf) == 0xf ? 1024 : 32;
    if (level >= 0x7e0)
      e->state = kDecay;
    if (level > 0x7ff)
      level = 0x7ff;
    break;
  case kDecay:
    level -= ((level - 1) >> 8) + 1;
    if (level < (((r[6] >> 5) + 1) << 8))
      e->state = kSustain;
    break;
  case kSustain:
    level -= ((level - 1) >> 8) + 1;
    break;
  case kGain:
    switch ((r[7] >> 5) & 3) {
    case 0: level = level >= 32 ? level - 32 : 0; break;
    case 1: level -= ((level - 1) >> 8) + 1; break;
    case 2: level = level + 32 > 0x7ff ? 0x7ff : level + 32; break;
    case 3: level += level < 0x600 ? 32 : 8; if (level > 0x7ff) level = 0x7ff; break;
    }
    break;
  }
  return level < 0 ? 0 : level;
}

static unsigned EnvelopeRate(const Envelope *e, const uint8_t *r) {
  switch (e->state) {
  case kAttack: return kRates[(r[5] & 0xf) * 2 + 1];
  case kDecay: return kRates[((r[5] >> 4) & 7) * 2 + 16];
  case kSustain: return kRates[r[6] & 0x1f];
  default: return kRates[r[7] & 0x1f];
  }
}

// A voice's envelope level after |n| of the DSP's samples, for its states
// but the sustain, which RunEnvelopes does itself. (Thumb code, from the
// cartridge.)
static int EnvelopeLevel(AudioVoice *voice, Envelope *e, const uint8_t *r, unsigned n) {
  int level = e->level;
  if (e->state == kRelease) {
    level -= 8 * n;
    if (level <= 0) {
      level = 0;
      voice->on = false;
    }
  } else if (e->state == kGain && !(r[7] & 0x80)) {
    level = (r[7] & 0x7f) * 16;
  } else {
    // Its steps in the time, at the rate of its state, which can change as
    // it goes: the attack's quick steps would go on into the decay and the
    // sustain, and the notes fade too soon. (No division: the GBA does it in
    // software, slowly.)
    unsigned rate, t = e->counter + n;
    while ((rate = EnvelopeRate(e, r)) && t >= rate) {
      if (e->state == kSustain || (e->state == kGain && (r[7] & 0x60) == 0x20)) {
        // Falling exponentially, as most are, with nothing to change it:
        // EnvelopeStep's, quicker.
        for (; t >= rate; t -= rate)
          level -= ((level - 1) >> 8) + 1;
      } else {
        int state = e->state;
        do {
          level = EnvelopeStep(e, r, level);
          t -= rate;
        } while (t >= rate && e->state == state);
      }
    }
    e->counter = rate ? t : 0;
  }
  return level;
}

// Runs the envelopes for |n| of the DSP's samples, and sets the mixer's
// volumes from them, and whether it has echo to mix. (ARM code in IWRAM, as
// it runs 250 times a second, for every voice: with pointers, not indexes.)
__attribute__((target("arm"), section(".iwram.text"), noinline))
static void RunEnvelopes(unsigned n) {
  AudioMix *m = &g_audio_mix;
  bool echo = m->echo_l || m->echo_r || m->feedback;
  AudioVoice *voice = g_audio_voices;
  Envelope *e = envelopes;
  const uint8_t *r = g_audio_dsp;
  for (int v = 0; v < 8; v++, voice++, e++, r += 16) {
    if (!voice->on)
      continue;
    int level;
    if (e->state == kSustain) {
      // (Most are, most of the time, with a step every few hundred of the
      // DSP's samples, if any.)
      unsigned rate = kRates[r[6] & 0x1f], t = e->counter + n;
      level = e->level;
      for (; rate && t >= rate; t -= rate)
        level -= ((level - 1) >> 8) + 1;
      e->counter = rate ? t : 0;
    } else {
      level = EnvelopeLevel(voice, e, r, n);
    }
    e->level = level;
    // Volume (/128) times envelope (/2048), in 8 bits (see AudioVoice), so
    // that quiet voices keep their envelopes and fades. (In 5, as they
    // were, Ceres's were 1s to 4s, stepping as they faded.)
    int left = (int8_t)r[0] * level >> 11, right = (int8_t)r[1] * level >> 11;
    voice->vol = (int32_t)((uint32_t)right << 16) + left;
    echo |= voice->on & voice->echo;
  }
  m->echo_on = echo;
}

void Audio_Init(void) {
  g_audio_mix.echo_len = 1;
  Audio_ClearEcho();
  g_audio_dsp[0x6c] = 0xe0;  // as the DSP resets: muted, no echo writes
  SpcPlayer_Initialize(&player);

  REG_SOUNDCNT_X = 0x80;  // on (first: the rest ignore writes while off)
  // DMA sound A on the left and B on the right, at full volume, by timer 0,
  // their FIFOs emptied.
  REG_SOUNDCNT_H = 1 << 2 | 1 << 3 | 1 << 9 | 1 << 11 | 1 << 12 | 1 << 15;
  REG_DMADAD(1) = (uintptr_t)&REG_FIFO_A;
  REG_DMADAD(2) = (uintptr_t)&REG_FIFO_A + 4;
  REG_TMCNT_L(0) = 0x10000 - kAudioTimerPeriod;
  REG_TMCNT_H(0) = 0x80;
}

#if SM_AUDIO_LOG
// For tools/spcref.c: what the game asks of the sound driver, frame by frame.
static uint32_t log_frame;
#endif

void RtlApuWrite(uint32 adr, uint8 val) {
#if SM_AUDIO_LOG
  if (player.input_ports[adr & 3] != val)
    printf("audio: %u port %u %u\n", (unsigned)log_frame, (unsigned)(adr & 3), val);
#endif
  player.input_ports[adr & 3] = val;
}

void RtlApuUpload(const uint8 *p) {
#if SM_AUDIO_LOG
  printf("audio: %u upload %u\n", (unsigned)log_frame, (unsigned)(p - snes_rom));
#endif
  // Not while Audio_Background runs the driver.
  g_audio_lock++;
  SpcPlayer_Upload(&player, p);
  g_audio_lock--;
}

volatile uint8_t g_audio_lock;
volatile uint32_t g_audio_cycles;

void Audio_Background(void) {
  uint32_t t0 = Cycles();
#if SM_AUDIO_LOG
  log_frame++;
#endif
  // The VBlank interrupt plays the buffers a frame at a time; this keeps
  // about two and a half frames ahead of it: a frame's worth a frame, and
  // two after a frame when the game was busy with the driver.
  enum { kFrame = kAudioFrameSamples, kAhead = kFrame * 5 / 2 & ~1 };
  static uint8_t next;
  static int filled;  // samples, in buffer |next|
  int ahead = g_audio_queued * kFrame + filled;
  int n = kAhead - ahead;
  if (n > 2 * kFrame)
    n = 2 * kFrame;
  // The driver's timer ticks at 500 Hz; it runs four ticks at a time, at
  // 125 Hz, for the time: its tempo, effects and envelopes count ticks, and
  // its slides and vibrato interpolate between them, so they come out the
  // same, if coarser. (The music's tempo, 32 in the songs looked at, steps
  // it every 8 ticks, so notes keep their time.) The DSP's envelopes run
  // twice as often, at 250 Hz, so that notes don't stop with a click (the
  // quickest go in 8 ms). In 16.16, the mixer's samples to the next time
  // the envelopes run.
  enum { kTicksPerRun = 4, kSamplesPerEnvelopes = 65536u * kAudioRate / 250 };
  static int32_t until_envelopes;
  static bool tick_next;
  while (n > 0) {
    // (In twos: the echo runs at half the rate.)
    if (until_envelopes < 0x20000) {
      tick_next = !tick_next;
      if (tick_next)
        SpcPlayer_Tick(&player, kTicksPerRun);
      until_envelopes += kSamplesPerEnvelopes;
      // For the time since they last ran (as long as the time to come).
      RunEnvelopes(envelope_time >> 8);
      envelope_time &= 0xff;
    }
    // Mixed in chunks from one to the next, across the buffers (they
    // follow each other) but for the last.
    int k = (until_envelopes >> 16) & ~1;
    if (k > n)
      k = n;
    if (k > kAudioChunk)
      k = kAudioChunk;
    if (k > (kAudioBuffers - next) * kFrame - filled)
      k = (kAudioBuffers - next) * kFrame - filled;
    Audio_MixChunk(g_audio_buffers[0][next] + filled, g_audio_buffers[1][next] + filled, k);
    envelope_time += k * kAudioDspSamples;
    until_envelopes -= k << 16;
    n -= k;
    for (filled += k; filled >= kFrame; filled -= kFrame) {
      next = (next + 1) & (kAudioBuffers - 1);
      REG_IME = 0;
      g_audio_queued++;
      REG_IME = 1;
    }
  }
  g_audio_cycles += Cycles() - t0;
}
