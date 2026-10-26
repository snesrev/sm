// The GBA cartridge's main loop: a frame of the game, then its picture.
#include <stdio.h>

#include "audio.h"
#include "convert.h"
#include "display.h"
#include "gba_hw.h"
#include "input.h"
#include "render.h"
#if SM_SCENARIOS
#include "scenarios.h"
#endif
#include "snes_hw.h"
#include "tour.h"
#include "sm_rtl.h"
#include "variables.h"

void RtlInit(void);
void RunOneFrameOfGame(void);
void IrqEntry(void);
extern volatile uint32_t g_vblank_count;
extern uint32_t g_audio_underruns;

static void VBlankIntrWait(void) {
  __asm__ volatile("swi 0x05" ::: "r0", "r1", "r2", "r3", "memory");
}

// Waits for the VBlank to show the next frame in: the next one, unless one
// began since the last frame was presented (|presented_at|, g_vblank_count
// then), it's gameplay, and there's about time in it for Render_Present
// (|lines|, its estimate of what it will take). A frame of the game just
// over the GBA's would wait a whole frame more; this way the time it
// takes evens out over the frames. A present that runs on into the
// picture is resumed from the line it's at (Display_Resume); the lines
// before it show the last frame's registers over the new frame's tiles,
// which is nothing to see within the HUD's (it doesn't scroll), so it may
// run that far over. (In gameplay and its doors, states 8-11: the title and
// the menus have no HUD, and frames with a room's worth of tiles to
// convert.)
enum { kLastLine = 227, kMayRunOver = 24 /* the HUD's lines, kGbaHudLines */ };
static bool WaitForVBlank(uint32_t presented_at, int lines) {
  unsigned line = REG_VCOUNT;
  if (g_vblank_count != presented_at && game_state >= 8 && game_state <= 11 &&
      line >= kGbaLines && line + lines <= kLastLine + kMayRunOver)
    return true;
  VBlankIntrWait();
  return false;
}

// The stack's low-water mark, to see that gba.ld reserves enough: the stack
// is painted at the start, and the paint left shows how deep it went.
extern char __sp_sys[], __stack_size[];
static void PaintStack(void) {
  volatile uint32_t *p = (volatile uint32_t *)(__sp_sys - (uintptr_t)__stack_size);
  uint32_t here = (uintptr_t)&p - 64;
  for (; (uintptr_t)p < here; p++)
    *p = 0xDEADBEEF;
}
static unsigned StackUsed(void) {
  const uint32_t *p = (const uint32_t *)(__sp_sys - (uintptr_t)__stack_size);
  while (*p == 0xDEADBEEF)
    p++;
  return (uintptr_t)__sp_sys - (uintptr_t)p;
}

void gba_main(void) {
  PaintStack();
  // Cartridge ROM: 3/1 wait states, prefetch. SRAM: 8.
  REG_WAITCNT = 0x4317;
  REG_TMCNT_H(3) = 0x84;  // cascade, enabled
  REG_TMCNT_H(2) = 0x80;

  RtlInit();
  SnesHw_Reset();
  Convert_Init();
  Render_Init();
  Audio_Init();

  REG_IRQ_VECTOR = (uintptr_t)IrqEntry;
  REG_IE = kIrqVBlank | kIrqVCount | kIrqTimer1;
  REG_IME = 1;

  printf("sm: starting\n");
  coroutine_state_0 = 1;
  bug_fix_counter = 1;

  // Per 60 frames: CPU cycles per frame of the game, its sound (per frame of
  // the GBA's), and render.c's work on its picture (not counting the wait
  // for VBlank); and how often the sound ran out.
  uint32_t frames = 0, game_cycles = 0, audio_cycles = 0, render_cycles = 0;
  uint32_t vblanks = g_vblank_count, audio_start = g_audio_cycles;
  // And the frames that took more than one of the GBA's, with the slowest's
  // work, for players' logs: cheap, as the line's printed anyway. And the
  // frames presented late in VBlank that ran on past it (none, if
  // Render_PresentLines is right).
  uint32_t slow = 0, worst[4] = {0, 0, 0, 0}, presented_audio = g_audio_cycles, overran = 0;
  for (;;) {
#if SM_ROOM_TOUR
    g_snes_hw.joy1 = Tour_Joypad(Input_ReadJoypad());
#else
    g_snes_hw.joy1 = Input_ReadJoypad();
#endif
    uint16_t state = game_state;
#if SM_SKIP_INTRO
    // For testing: a new game's intro goes straight to its end, which fades
    // out and starts the game at Ceres, or with SM_START_ZEBES, as after
    // escaping Ceres, on Zebes.
    static uint16_t intro_frames;
    if (game_state == 30 && ++intro_frames == 60)
      cinematic_function = (uint16_t)0x8BC0C5;  // CinematicFunction_Intro_Func72
#if SM_START_ZEBES
    if (game_state == 31 && loading_game_state == 0x1f)
      loading_game_state = 0x22;  // kLoadingGameState_22_EscapingCeres
#endif
#endif
#if SM_TEST_ITEMS
    // For testing the windows, which the X-ray scope and power bombs use.
    if (game_state == 8 && !(collected_items & 0x8000)) {
      collected_items |= 0xc004;
      equipped_items |= 0xc004;
      samus_power_bombs = samus_max_power_bombs = 10;
    }
#endif
#if SM_TRACE
    // For tools/hostrun.c, which replays the joypad on the PC build.
    static uint32_t trace_frame;
    static uint16_t trace_joy = 0xffff;
    if (g_snes_hw.joy1 != trace_joy)
      printf("joy: %u %04x\n", (unsigned)trace_frame, g_snes_hw.joy1);
    trace_joy = g_snes_hw.joy1;
#endif
#if SM_ROOM_TOUR
    Tour_BeforeFrame();
#endif
#if SM_SCENARIOS
    Scn_BeforeFrame();
#endif
    // (Less the sound's, which interrupts them: see Audio_Background.)
    uint32_t t0 = Cycles(), a0 = g_audio_cycles;
    RunOneFrameOfGame();
    // SoftReset (the file select's B, the game over menu) only sets game_state to
    // 0xffff: on the SNES the reset vector runs, and the PC build resumes the boot
    // coroutine after its logo (see RtlRunFrameCompare). Without this the next frame
    // indexes the state table with -1 and jumps into the weeds.
    if (game_state == 0xffff)
      coroutine_state_0 = 3;
#if SM_TRACE
    {
      uint32_t hash = 0;
      // Of the room's data in RAM, which is the same on the PC, unlike
      // some of the other RAM: the sound driver's ports, for one.
      for (int i = 0x10000; i < 0x20000; i += 64)
        hash = (hash * 31) ^ *(uint32_t *)&g_ram[i];
      printf("t: %u %08x %04x %d.%04x %d.%04x\n", (unsigned)trace_frame, (unsigned)hash, room_ptr,
             samus_x_pos, samus_x_subpos, samus_y_pos, samus_y_subpos);
#ifdef SM_TRACE_DUMP
      // The game's variables after a frame, to diff, and a hash of each KB
      // of its RAM.
      if (trace_frame == SM_TRACE_DUMP - 1)
        for (int i = 0; i < 0x20000; i += 0x400) {
          uint32_t h = 0;
          for (int j = 0; j < 0x400; j += 4)
            h = (h * 31) ^ *(uint32_t *)&g_ram[i + j];
          printf("k: %05x %08x\n", i, (unsigned)h);
        }
      if (trace_frame >= SM_TRACE_DUMP - 2 && trace_frame <= SM_TRACE_DUMP)
        for (int i = 0; i < 0x2000; i += 32) {
          printf("m: %u %04x", (unsigned)trace_frame, i);
          for (int j = 0; j < 32; j++)
            printf(" %02x", g_ram[i + j]);
          printf("\n");
        }
#endif
      trace_frame++;
    }
#endif
    if (game_state != state)
      printf("sm: game state %u\n", (unsigned)game_state);
    uint32_t t1 = Cycles(), a1 = g_audio_cycles;
    Render_CaptureFrame();
    uint32_t t2 = Cycles(), a2 = g_audio_cycles;
    static uint32_t presented_at;
    uint32_t last_presented = presented_at;
    bool late = WaitForVBlank(presented_at, Render_PresentLines());
    uint32_t t3 = Cycles(), a3 = g_audio_cycles;
    Render_Present();
    if (late && REG_VCOUNT < kGbaLines)
      overran++;
    presented_at = g_vblank_count;
    g_frame_presented = true;
    uint32_t t4 = Cycles(), a4 = g_audio_cycles;
    Audio_Kick();
#if SM_ROOM_TOUR
    // Holds the frame on screen for smrun's SHOT_ON.
    if (Tour_AfterFrame())
      for (int i = 0; i < 4; i++) {
        VBlankIntrWait();
        Audio_Kick();
      }
#endif
#if SM_SCENARIOS
    // As the tour's hold, but not counted in the speed.
    if (Scn_AfterFrame()) {
      uint32_t held_from = g_vblank_count;
      for (int i = 0; i < 4; i++) {
        VBlankIntrWait();
        Audio_Kick();
      }
      vblanks += g_vblank_count - held_from;
      presented_at = g_vblank_count;
    }
#endif
    uint32_t game = (t1 - t0) - (a1 - a0), render = (t2 - t1) - (a2 - a1) + (t4 - t3) - (a4 - a3);
    uint32_t audio_now = a4 - presented_audio;
    presented_audio = a4;
    game_cycles += game;
    render_cycles += render;
#ifdef SM_FRAME_LOG
    // For finding stalls (-DCMAKE_C_FLAGS=-DSM_FRAME_LOG): every frame that took more than one of
    // the GBA's, with where its time went; the line below only keeps the slowest of a second.
    if (presented_at - last_presented > 1)
      printf("frame: %u vblanks game %u audio %u render %u state %u room %04x\n",
             (unsigned)(presented_at - last_presented), (unsigned)game, (unsigned)audio_now,
             (unsigned)render, (unsigned)game_state, room_ptr);
#endif
    if (presented_at - last_presented > 1) {
      slow++;
      if (game + render + audio_now > worst[0] + worst[1] + worst[2]) {
        worst[0] = game, worst[1] = render, worst[2] = audio_now;
        worst[3] = presented_at - last_presented;
      }
    }
    if (++frames == 60) {
      uint32_t v = g_vblank_count, audio = g_audio_cycles;
      audio_cycles = (audio - audio_start) / (v - vblanks);
      audio_start = audio;
      printf("sm: 60 frames in %u vblanks; cycles/frame: game %u, audio %u, render %u; "
             "state %u; stack %u; underruns %u; room %04x",
             (unsigned)(v - vblanks), (unsigned)(game_cycles / 60), (unsigned)audio_cycles,
             (unsigned)(render_cycles / 60), (unsigned)game_state, StackUsed(), (unsigned)g_audio_underruns,
             room_ptr);
      if (slow)
        printf("; slow %u, the slowest %u vblanks: game %u, audio %u, render %u", (unsigned)slow,
               (unsigned)worst[3], (unsigned)worst[0], (unsigned)worst[2], (unsigned)worst[1]);
      if (overran)
        printf("; %u late presents overran", (unsigned)overran);
      printf("\n");
      vblanks = v;
      frames = game_cycles = audio_cycles = render_cycles = slow = overran = 0;
      worst[0] = worst[1] = worst[2] = worst[3] = 0;
    }
  }
}
