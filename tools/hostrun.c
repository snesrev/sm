// hostrun: replays a GBA run's joypad on the PC build of the game, to find
// where the GBA's game logic goes another way.
//
//     hostrun sm.smc TRACE FRAMES [mine|both]
//
// TRACE is the log of a cartridge built with -DSM_TRACE=ON (from smrun):
// its "joy: FRAME VALUE" lines are the joypad (JOY1), frame by frame. With
// the same skipping of the intro as gba/main.c (-DSM_SKIP_INTRO=ON
// -DSM_START_ZEBES=ON; with CERES=1, without SM_START_ZEBES; with INTRO=1,
// neither, for the whole intro), this runs
// FRAMES frames of the PC's C code (mine), or of it and the original,
// emulated, compared every frame (both, the default: a mismatch prints
// "Verify failure!"), and prints the same "t:" lines as the GBA's, to
// diff: the frame, a hash of the room's data in RAM, the room, and Samus's
// position. With DUMP=FRAME (and the cartridge built with
// -DCMAKE_C_FLAGS=-DSM_TRACE_DUMP=FRAME), both print the game's variables
// (RAM's first 8 KB, "m:" lines) for the frames up to it, and a hash of
// each KB of RAM ("k:" lines) for the one before.
//
// Build (from the top of the repository):
//     clang -O1 -w -I. -o hostrun tools/hostrun.c \
//         $(ls src/*.c src/snes/*.c | grep -v -e main.c -e opengl.c -e glsl_shader.c \
//           -e config.c) $(sdl2-config --cflags) -lm
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/sm_rtl.h"
#include "src/snes/ppu.h"
#include "src/snes/snes.h"
#include "src/spc_player.h"
#include "src/variables.h"

Snes *SnesInit(const char *filename);
extern uint8 g_runmode;
extern SpcPlayer *g_spc_player;

// What the PC's main.c has for the others.
bool g_debug_flag;
bool g_is_turbo;
bool g_want_dump_memmap_flags;
bool g_new_ppu = true;
bool g_other_image;
int g_got_mismatch_count;
struct SpcPlayer *g_spc_player;

void RtlApuLock(void) {}
void RtlApuUnlock(void) {}

void NORETURN Die(const char *error) {
  fprintf(stderr, "Error: %s\n", error);
  exit(1);
}

void Warning(const char *error) {
  fprintf(stderr, "Warning: %s\n", error);
}

int main(int argc, char **argv) {
  if (argc < 4) {
    fprintf(stderr, "usage: %s sm.smc TRACE FRAMES [mine|both]\n", argv[0]);
    return 2;
  }
  int frames = atoi(argv[3]);
  g_runmode = argc > 4 && !strcmp(argv[4], "mine") ? 1 : 0;
  static uint16 joy[1 << 20];
  FILE *f = fopen(argv[2], "r");
  if (!f) {
    fprintf(stderr, "can't read %s\n", argv[2]);
    return 1;
  }
  char line[256];
  int last = 0;
  uint16 value = 0;
  while (fgets(line, sizeof line, f)) {
    const char *p = strstr(line, "joy: ");
    unsigned fr, v;
    if (p && sscanf(p, "joy: %u %x", &fr, &v) == 2 && fr < (1 << 20)) {
      for (; last < (int)fr; last++)
        joy[last] = value;
      value = v;
    }
  }
  for (; last < (1 << 20); last++)
    joy[last] = value;
  fclose(f);

  Snes *snes = SnesInit(argv[1]);
  if (!snes) {
    fprintf(stderr, "can't load %s\n", argv[1]);
    return 1;
  }
  // Somewhere to draw, as main.c has, and drawing on (otherwise these are
  // what malloc left: the PPU writes through a null pointer, or never
  // draws).
  static uint8_t pixels[256 * 4 * 240], my_pixels[256 * 4 * 240];
  PpuBeginDrawing(snes->snes_ppu, pixels, 256 * 4, 0);
  PpuBeginDrawing(snes->my_ppu, my_pixels, 256 * 4, 0);
  snes->disableRender = false;
  g_spc_player = SpcPlayer_Create();
  SpcPlayer_Initialize(g_spc_player);
  coroutine_state_0 = 1;
  bug_fix_counter = 1;
  uint16 intro_frames = 0;
  bool ceres = getenv("CERES") != NULL;
  bool intro = getenv("INTRO") != NULL;
  for (int fr = 0; fr < frames; fr++) {
    if (!intro && game_state == 30 && ++intro_frames == 60)
      cinematic_function = (uint16)0x8BC0C5;  // CinematicFunction_Intro_Func72
    if (!intro && !ceres && game_state == 31 && loading_game_state == 0x1f)
      loading_game_state = 0x22;  // kLoadingGameState_22_EscapingCeres
    // JOY1's bits, B first, are the emulated joypad's from bit 0.
    int input = 0;
    for (int i = 0; i < 16; i++)
      if (joy[fr] & (0x8000 >> i))
        input |= 1 << i;
    RtlRunFrame(input);
    uint32 hash = 0;
    for (int i = 0x10000; i < 0x20000; i += 64)
      hash = (hash * 31) ^ *(uint32 *)&g_ram[i];
    printf("t: %u %08x %04x %d.%04x %d.%04x\n", fr, hash, room_ptr, samus_x_pos, samus_x_subpos,
           samus_y_pos, samus_y_subpos);
    // With DUMP=FRAME, the game's variables then, as SM_TRACE_DUMP does.
    static int dump = -1;
    if (dump < 0)
      dump = getenv("DUMP") ? atoi(getenv("DUMP")) : 0;
    if (dump && fr == dump - 1)
      for (int i = 0; i < 0x20000; i += 0x400) {
        uint32 h = 0;
        for (int j = 0; j < 0x400; j += 4)
          h = (h * 31) ^ *(uint32 *)&g_ram[i + j];
        printf("k: %05x %08x\n", i, h);
      }
    if (dump && fr >= dump - 2 && fr <= dump)
      for (int i = 0; i < 0x2000; i += 32) {
        printf("m: %u %04x", fr, i);
        for (int j = 0; j < 32; j++)
          printf(" %02x", g_ram[i + j]);
        printf("\n");
      }
  }
  return 0;
}
