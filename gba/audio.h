// The game's sound, on the GBA: its sound driver (src/spc_player.c, the
// SNES's driver translated to C), with a GBA mixer in place of the SNES's
// DSP, playing the samples make_cartridge.py decoded. See audio.c.
#pragma once

#include <stdbool.h>
#include <stdint.h>

enum {
  // 10512 Hz: a timer period of 1596 cycles, and 176 samples a GBA frame.
  kAudioTimerPeriod = 1596,
  kAudioRate = 16777216 / kAudioTimerPeriod,
  kAudioFrameSamples = 280896 / kAudioTimerPeriod,
  // The DSP's 32000 Hz samples a mixer sample, in 8.8 fixed point.
  kAudioDspSamples = 32000 * 256 / kAudioRate,
  kAudioBuffers = 4,
  // The mixer mixes at most this many samples at a time: about as many as
  // between the driver's ticks (audio.c).
  kAudioChunk = 48,
  // The longest echo the DSP has, 15 * 16 ms (EDL), in the mixer's samples
  // and in the pairs of them the echo works on.
  kAudioEchoSamples16ms = kAudioRate * 16 / 1000,
  kAudioEchoMax = 15 * kAudioEchoSamples16ms / 2,
};

// A DSP voice, as the mixer plays it.
typedef struct AudioVoice {
  const int8_t *pcm;
  // In samples, 16.16: where it's at, how far it goes a sample, where it
  // ends, and how far back it loops from there (0: it doesn't).
  uint32_t pos, step, end, loop_len;
  // Its volume times its envelope, /2048, both sides in a word: right <<
  // 16 + left. Its samples (-128 to 127) times it are at half the scale of
  // the SNES's 16-bit samples. The voices' sums fit in 16 bits until three
  // or four times as loud as what plays unclipped: the title's music and
  // Ridley's fight get to 9700.
  int32_t vol;
  bool on, noise, echo;
} AudioVoice;

// The DSP's volumes, master and echo, as the mixer mixes with them.
typedef struct AudioMix {
  int8_t master_l, master_r;
  int8_t echo_l, echo_r, feedback;
  // The echo runs at half the mixer's rate (it's the music's low end,
  // mostly), on pairs of samples. Its filter (FIR): the DSP's 8 taps at 32
  // kHz, about 6 of its samples a tap here, newest first.
  int16_t fir[2];
  // Whether the voices' echo goes into the echo buffer (FLG), and how many
  // pairs of the mixer's samples it delays them (EDL).
  bool echo_writes;
  // Whether there's echo to mix: echo or feedback volume, or voices with
  // echo on (audio.c works it out with the envelopes).
  bool echo_on;
  uint16_t echo_len;
} AudioMix;

void Audio_Init(void);
// Once a frame of the GBA's, from the timer 1 interrupt (irq.iwram.c,
// irq_entry.S), with interrupts on: runs the driver and mixes ahead.
void Audio_Background(void);
// After Render_Present, in VBlank: Audio_Background can run now, rather
// than later in the frame.
void Audio_Kick(void);
// While nonzero, the game is busy with the driver, and Audio_Background
// waits for the next frame.
extern volatile uint8_t g_audio_lock;
extern volatile uint8_t g_audio_background_running;
// The cycles Audio_Background has taken, all told.
extern volatile uint32_t g_audio_cycles;

// audio_mix.iwram.c:
extern AudioVoice g_audio_voices[8];
extern int8_t g_audio_buffers[2][kAudioBuffers][kAudioFrameSamples];
extern volatile uint8_t g_audio_play, g_audio_queued;
// The DSP's samples between steps of its noise (by FLG), or 0 to hold.
extern uint16_t g_audio_noise_period;
extern AudioMix g_audio_mix;
// Mixes |n| (even, up to kAudioChunk) samples of the voices into |left| and
// |right|, with g_audio_mix's volumes and echo.
void Audio_MixChunk(int8_t *left, int8_t *right, int n);
// Empties the echo buffer.
void Audio_ClearEcho(void);
// In the VBlank interrupt: plays the next buffer this frame.
void Audio_VBlank(void);
