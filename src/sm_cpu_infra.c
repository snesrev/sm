#include "sm_cpu_infra.h"
#include "types.h"
#include "snes/cpu.h"
#include "snes/snes.h"
#include "tracing.h"





#include "ida_types.h"
#include "variables.h"
#include "funcs.h"
#include "sm_rtl.h"
#include "util.h"

#include <time.h>

void RtlRunFrameCompare(uint16 input, int run_what);

enum RunMode { RM_BOTH, RM_MINE, RM_THEIRS };
uint8 g_runmode = RM_BOTH;

extern int g_got_mismatch_count;

Snes *g_snes;
Cpu *g_cpu;

bool g_calling_asm_from_c;
int g_calling_asm_from_c_ret;
bool g_fail;
bool g_use_my_apu_code = true;
extern bool g_other_image;

typedef struct Snapshot {
  uint16 a, x, y, sp, dp, pc;
  uint8 k, db, flags;

  uint16_t vTimer;

  uint8 ram[0x20000];
  uint16 vram[0x8000];
  uint8 sram[0x2000];

  uint16 oam[0x120];
} Snapshot;

static Snapshot g_snapshot_mine, g_snapshot_theirs, g_snapshot_before;
static uint32 hookmode, hookcnt, hookadr;
static uint32 hooked_func_pc;
static uint8 hook_orgbyte[1024];
static uint8 hook_fixbug_orgbyte[1024];

static void VerifySnapshotsEq(Snapshot *b, Snapshot *a, Snapshot *prev);
static void MakeSnapshot(Snapshot *s);
static void RestoreSnapshot(Snapshot *s);

void Call(uint32 addr) {
  assert(addr & 0x8000);
  RunAsmCode(addr, 0, 0, 0, 0);
}

uint8_t *SnesRomPtr(uint32 v) {
  return (uint8*)RomPtr(v);
}

bool ProcessHook(uint32 v) {
  uint8_t *rombyte = SnesRomPtr(v);
  switch (hookmode) {
  case 0: // remove hooks
    *rombyte = hook_orgbyte[hookcnt++];
    return false;
  case 1: // install hooks
    hook_orgbyte[hookcnt++] = *rombyte;
    *rombyte = 0;
    return false;
  case 2:  // run hook
    if (v == hookadr) {
      hookmode = 3;
      return true;
    }
    return false;
  }
  return false;
}

bool FixBugHook(uint32 addr) {
  switch (hookmode) {
  case 1: { // install hooks
    uint8_t *rombyte = SnesRomPtr(addr);
    hook_fixbug_orgbyte[hookcnt++] = *rombyte;
    *rombyte = 0;
    return false;
  }
  case 2:  // run hook
    if (addr == hookadr) {
      hookmode = 3;
      return true;
    }
    hookcnt++;
    return false;
  }
  return false;
}

static const  uint32 kPatchedCarrys[] = {
  0xa7ac33,
  0xa7ac36,
  0xa7ac39,
  0xa7ac42,
  0xa7ac45,
  // Ridley_Func_107
  0xa6d6d1,
  0xa6d6d3,
  0xa6d6d5,
  0xa6d700,
  0xa6d702,
  0xa6d704,
  // Ridley_Func_106
  0xa6d665,
  0xa6d667,
  0xa6d669,
  0xa6d694,
  0xa6d696,
  0xa6d698,
  // DrawSpritemapWithBaseTile2
  0x818b65,
  0x818b6B,
  // DrawSpritemapWithBaseTileOffscreen
  0x818ba7,
  0x818bd9,
  0x818bdf,

  // EprojInit_BombTorizoLowHealthInitialDrool
  0x86a671,
  0x86a680,
  0x86a6a9,
  0x86a6ba,
  // HandleEarthquakeSoundEffect
  0x88B245,
  // Ridley_Func_104
  0xA6D565,
  0xA6D567,
  0xA6D599,
  0xA6D59B,
  // Ridley_Func_105
  0xA6D5DB,
  0xA6D5DD,
  0xA6D60F,
  0xA6D611,
  // Ridley_Func_86
  0xA6CEFF,
  // Shitroid_GraduallyAccelerateTowardsPt
  0xa9f4a5,
  0xa9f4a7,
  0xa9f4d6,
  0xa9f4d8,
  // Shitroid_GraduallyAccelerateHoriz
  0xa9f519,
  0xa9f51f,
  0xa9f521,
  0xa9f554,
  0xa9f55a,
  0xa9f55c,
  // Shitroid_Func_16
  0xA9F24D,

  // Lots of ADC
  0x80AA6A,
  0x80A592,
  0x80A720,
  0x80A7D6,
  0x80A99B,
  0x818AA7,
  0x94B176,
  0x94B156,

  // MotherBrain
  0xA99413,

  // room_width_in_blocks etc
  0x80ab5d,
  0x84865c,
  0x848d90,
  0x84ab60,
  0x84b567,
  0x84b588,
  0x84b606,
  0x84b615,
  0x84b624,
  0x84b9d3,
  0x84b9e2,
  0x84ba07,
  0x84ba1e,
  0x84ba35,
  0x84d6ae,
  0x84d6bf,
  0x84d7f4,
  0x84d803,
  0x84d812,
  0x84daae,
  0x84dbaa,
  0x84dbe1,
  0x84dc20,
  0x84dc52,
  0x84dc89,
  0x84dcc8,
  0x84deae,
  0x84dedd,
  0x84df0a,
  0x84df39,
  0x86893a,
  0x9483a7,
  0x948405,
  0x949592,
  0x94a13f,
  0x94a2b2,
  0x94a3d8,
  0xa0bc33,
  0xa0bdac,
  0xa0bf45,
  0xa0c725,
  0x88B486,
  0x88C578,
  0xA292E8,
  0x86F18E,
  0x888CB6,
  0x888FAA,
  0x88A483,
  0x91CC35,
  0x91CBFF,
  0xA09541,
  0xA09552,
  0xA49AE8,
  0xA6C297,
  0xA6C3AD,
  0xA9C5EC,
  0xA9D500,
  0xA9D537,
  0xA9DCDB,


  0xA0A31B,
  0x91D064,
  0x91D07A,

  0x90C719,

  0xA6A80E,
  0xA6A816,

  0xA4906E,
  0xA49071,

  0x90BC75,
  0x90BC93,
};
static uint8 kPatchedCarrysOrg[arraysize(kPatchedCarrys)];

uint32 PatchBugs(uint32 mode, uint32 addr) {
  hookmode = mode, hookadr = addr, hookcnt = 0;
  if (FixBugHook(0x86EF35)) {
    g_cpu->x = g_cpu->y;
  } else if (FixBugHook(0x86EF45)) {
    g_cpu->z = (g_cpu->a == 0); // EprojInit_F337 doesn't compare A and assumes Z is in it.
  } else if (FixBugHook(0x818ab8)) {
    if (g_cpu->y == 0)
      g_cpu->pc = 0x8b1f;
  } else if (FixBugHook(0xa794ba)) { // Kraid_Arm_Shot - y undefined
    g_cpu->y = g_cpu->x;
  } else if (FixBugHook(0xa7b968)) {  // KraidEnemy_ProcessInstrEnemyTimer - X is undefined
    g_cpu->x = cur_enemy_index;
  } else if (FixBugHook(0xa7b963)) {  // KraidFoot_FirstPhase_Thinking - X is undefined
    g_cpu->x = cur_enemy_index;
  } else if (FixBugHook(0xA496C8)) { // Crocomire_Func_67 assumes A is zero
    g_cpu->a = 0;
  } else if (FixBugHook(0x9085AA)) { // Samus_HandleSpeedBoosterAnimDelay doesn't preserve A
    g_cpu->a = speed_boost_counter;
  } else if (FixBugHook(0xA29044) || FixBugHook(0xA2905D)) { // MaridiaBeybladeTurtle_Func8 thinks INC sets carry
    g_cpu->c = (g_cpu->a == 0);
  } else if (FixBugHook(0xa29051)) {  /// MaridiaBeybladeTurtle_Func8 does an INC too much
    g_cpu->a--;
  } else if (FixBugHook(0xA5931C)) {  // Draygon_Func_35 needs cur_enemy_index in X
    g_cpu->x = cur_enemy_index;
  } else if (FixBugHook(0x80ADA4)) {  // DoorTransitionScrollingSetup_Down
    g_cpu->a = layer2_y_pos;
  } else if (FixBugHook(0x80ADD9)) {  // DoorTransitionScrollingSetup_Up
    g_cpu->a = layer2_y_pos;
  } else if (FixBugHook(0x80AD4d)) {  //  DoorTransitionScrollingSetup_Right
    g_cpu->a = layer2_x_pos;
  } else if (FixBugHook(0x80AD77)) {  //  DoorTransitionScrollingSetup_Left
    g_cpu->a = layer2_x_pos;
  } else if (FixBugHook(0x9381db)) {  // ProjectileInsts_GetValue reading from invalid memory for newly started ones
    int k = g_cpu->x;
    int ip = projectile_bomb_instruction_ptr[k >> 1];
    int delta = (projectile_bomb_instruction_timers[k >> 1] == 1 && !sign16(get_ProjectileInstr(ip)->timer)) ? 0 : -8;
    g_cpu->a += 8 + delta;
  } else if (FixBugHook(0x86b701)) { // EprojPreInstr_EyeDoorProjectile using destroyed X
    g_cpu->x = g_cpu->y;
  } else if (FixBugHook(0x8FC1B0)) {  // RoomCode_GenRandomExplodes X is garbage
    g_cpu->x = g_cpu->a;
  } else if (FixBugHook(0x80804F)) {
    //for(int i = 0; i < 5000; i++)
    //  snes_readBBusOrg(g_snes, (uint8_t)APUI00);
  } else if (FixBugHook(0x829325)) {
    // forgot to change bank
    g_cpu->db = 0x82;
  } else if (FixBugHook(0x848ACD)) {
    // PlmInstr_IncrementArgumentAndJGE A is not zeroed
    g_cpu->a = 0;
  } else if (FixBugHook(0xA7CEB2)) {
    // Phantoon_Main forgots to reload x
    g_cpu->x = cur_enemy_index;
  } else if (FixBugHook(0x91CD44)) {
    // Xray_SetupStage4_Func2 passes a bad value to Xray_GetXrayedBlock
    if (g_cpu->x == 0)
      g_cpu->pc = 0xCD52;

  // Fix VAR BEAM etc.
  // Prevent EquipmentScreenCategory_ButtonResponse from getting called when category changed
  } else if (FixBugHook(0x82AFD3)) {
    if ((uint8)pausemenu_equipment_category_item != 1)
      return 0x82AFD9;
  } else if (FixBugHook(0x82B0CD)) {
    if ((uint8)pausemenu_equipment_category_item != 2)
      return 0x82AFD9;
  } else if (FixBugHook(0x82B15B)) {
    if ((uint8)pausemenu_equipment_category_item != 3)
      return 0x82AFD9;
  } else if (FixBugHook(0xA2D38C)) {
    // MaridiaLargeSnail_Touch uses uninitialized X
    g_cpu->x = cur_enemy_index;
  } else if (FixBugHook(0xA4970F)) {
    // Crocomire_Func_67 does weird things
    g_cpu->a &= 0xff;
    g_cpu->y = g_cpu->x & 0x7;
  } else if (FixBugHook(0xA496E0)) {
    if (g_cpu->x > 48) {
      croco_cur_vline_idx = g_cpu->x;
      g_cpu->mf = 0;
      return 0xA497CE;
    }
  } else if (FixBugHook(0x91DA89)) {
    // Samus_HandleScrewAttackSpeedBoostingPals reads OOB
    if (special_samus_palette_frame > 6)
      special_samus_palette_frame = 6;
  } else if (FixBugHook(0x828D56)) {
    WriteReg(VMAIN, 0x80); // BackupBG2TilemapForPauseMenu lacks this
  } else if (FixBugHook(0x88AFCF)) {
    if (g_cpu->a & 0x8000)  // RoomMainAsm_ScrollingSky reads oob
      g_cpu->a = 0;
  } else if (FixBugHook(0x88AFF2)) {
    if (g_cpu->a < 256)  // RoomMainAsm_ScrollingSky reads oob
      g_cpu->a = 256;
  } else if (FixBugHook(0x8189bd)) {
    if (g_cpu->y == 0)  // DrawSamusSpritemap reads invalid ptr
      return 0x818A35;
  } else if (FixBugHook(0xA29BC1)) {
    g_cpu->a = 1;  // ThinHoppingBlobs_Func8 reads from R1 instead of #1
  } else if (FixBugHook(0x82E910)) {
    WORD(g_ram[22]) = 0; // SpawnDoorClosingPLM doesn't zero R22
  } else if (FixBugHook(0x90A4C8)) {
    WORD(g_ram[18]) = 0;  // Samus_InitJump overwrites R18 in Samus_Movement_03_SpinJumping
  } else if (FixBugHook(0xA99F60)) {
    WORD(g_ram[22]) = 1; // MotherBrain_Instr_SpawnLaserEproj doesn't set R22
  } else if (FixBugHook(0x94A85B)) {
    memset(g_ram + 0xd82, 0, 8); // grapple_beam_tmpD82 not cleared in BlockCollGrappleBeam
  } else if (FixBugHook(0xA0A35C)) {
    // ProcessEnemyPowerBombInteraction - R18 may get overwritten by the enemy death routine
    REMOVED_R18 = HIBYTE(power_bomb_explosion_radius);
    REMOVED_R20 = (REMOVED_R18 + (REMOVED_R18 >> 1)) >> 1;
  } else if (FixBugHook(0xA7B049)) {
    // Kraid_Shot_Mouth: The real game doesn't preserve R18, R20 so they're junk at this point.
    // Force getting out of the loop.
    g_cpu->x = 0;
  } else if (FixBugHook(0xa5a018)) {
    // Draygon_Func_42 uses undefined varE24 value
    REMOVED_varE24 = 0;
  }

  return 0;
}

int RunPatchBugHook(uint32 addr) {
  uint32 new_pc = PatchBugs(2, addr);
  if (hookmode == 3) {
    if (new_pc == 0) {
      return hook_fixbug_orgbyte[hookcnt];
    } else {
      g_cpu->k = new_pc >> 16;
      g_cpu->pc = (new_pc & 0xffff) + 1;
      return *SnesRomPtr(new_pc);
    }
  }

  return -1;
}

int CpuOpcodeHook(uint32 addr) {
  for (size_t i = 0; i != arraysize(kPatchedCarrys); i++) {
    if (addr == kPatchedCarrys[i])
      return kPatchedCarrysOrg[i];
  }
  {
    int i = RunPatchBugHook(addr);
    if (i >= 0) return i;
  }
  assert(0);
  return 0;
}

bool HookedFunctionRts(int is_long) {
  if (g_calling_asm_from_c) {
    g_calling_asm_from_c_ret = is_long;
    g_calling_asm_from_c = false;
    return false;
  }
  assert(0);
  return false;
}

static void VerifySnapshotsEq(Snapshot *b, Snapshot *a, Snapshot *prev) {
  memcpy(&b->ram[0x0], &a->ram[0x0], 0x34);  // r18, r20, R22 etc

  memcpy(&b->ram[0x1f5b], &a->ram[0x1f5b], 0x100 - 0x5b);  // stacck
  memcpy(&a->ram[0x3c], &b->ram[0x3c], 2);  // nmicopy1_var_d
  memcpy(&b->ram[0x44], &a->ram[0x44], 13);  // decompress temp
  memcpy(&b->ram[0xad], &a->ram[0xad], 2);  // ptr_to_retaddr_parameters
  memcpy(&b->ram[0x5e7], &a->ram[0x5e7], 14);  // bitmask, mult_tmp, mult_product_lo etc

  memcpy(&a->ram[0x60B], &b->ram[0x60B], 6);  // enemy_projectile_init_param_2, remaining_enemy_hitbox_entries, REMOVED_num_projectiles_to_check_enemy_coll
  memcpy(&a->ram[0x611], &b->ram[0x611], 6);  // coroutine_state (copy from mine to theirs)
  memcpy(&b->ram[0x641], &a->ram[0x641], 2);  // apu_attempts_countdown
  memcpy(&a->ram[0x77e], &b->ram[0x77e], 5);  // my counter
  memcpy(&b->ram[0xA82], &a->ram[0xA82], 2);  // xray_angle
  memcpy(&a->ram[0xd1e], &b->ram[0xd1e], 2);  // grapple_beam_unkD1E
  memcpy(&a->ram[0xd82], &b->ram[0xd82], 8);  // grapple_beam_tmpD82

  memcpy(&a->ram[0xd9c], &b->ram[0xd9c], 2);  // grapple_beam_tmpD82
  memcpy(&a->ram[0xdd2], &b->ram[0xdd2], 6);  // temp_collision_DD2 etc
  memcpy(&a->ram[0xd8a], &b->ram[0xd8a], 6);  // grapple_beam_tmpD8A
  memcpy(&a->ram[0xe20], &b->ram[0xe20], 0xe46 - 0xe20);  // temp vars
  memcpy(&a->ram[0xe54], &b->ram[0xe54], 2);  // cur_enemy_index
  
  memcpy(&a->ram[0xe02], &b->ram[0xe02], 2);  // samus_bottom_boundary_position
  memcpy(&a->ram[0xe4a], &b->ram[0xe4a], 2);  // new_enemy_index
  memcpy(&a->ram[0xe56], &b->ram[0xe56], 4);  // REMOVED_cur_enemy_index_backup etc
  
  
  memcpy(&a->ram[0x1784], &b->ram[0x1784], 8);  // enemy_ai_pointer etc
  memcpy(&a->ram[0x1790], &b->ram[0x1790], 4);  // set_to_rtl_when_loading_enemies_unused etc
  memcpy(&a->ram[0x17a8], &b->ram[0x17a8], 4);  // interactive_enemy_indexes_index
  
  memcpy(&a->ram[0x1834], &b->ram[0x1834], 8);  // distance_to_enemy_colliding_dirs
  memcpy(&a->ram[0x184A], &b->ram[0x184A], 18);  // samus_x_pos_colliding_solid etc
  memcpy(&a->ram[0x186E], &b->ram[0x186E], 16+8);  // REMOVED_enemy_spritemap_entry_pointer etc
  memcpy(&a->ram[0x18A6], &b->ram[0x18A6], 2);  // collision_detection_index
  memcpy(&a->ram[0x189A], &b->ram[0x189A], 12);  // samus_target_x_pos etc
  
  memcpy(&b->ram[0x1993], &a->ram[0x1993], 2);  // enemy_projectile_init_param
  memcpy(&b->ram[0x19b3], &a->ram[0x19b3], 2);  // mode7_spawn_param
  memcpy(&b->ram[0x1a93], &a->ram[0x1a93], 2);  // cinematic_spawn_param
  memcpy(&b->ram[0x1B9D], &a->ram[0x1B9D], 2);  // cinematic_spawn_param
  memcpy(&a->ram[0x1E77], &b->ram[0x1E77], 2);  // current_slope_bts

  memcpy(&a->ram[0x9100], &b->ram[0x9100], 0x1cc + 2);  // XrayHdmaFunc has some bug that i couldn't fix in asm
  memcpy(&a->ram[0x9800], &b->ram[0x9800], 0x1cc+2);  // XrayHdmaFunc has some bug that i couldn't fix in asm
  memcpy(&a->ram[0x99cc], &b->ram[0x99cc], 2);  // XrayHdmaFunc_BeamAimedL writes outside
  memcpy(&a->ram[0xEF74], &b->ram[0xEF74], 4);  // next_enemy_tiles_index
  memcpy(&a->ram[0xF37A], &b->ram[0xF37A], 6);  // word_7EF37A etc
  

  if (memcmp(b->ram, a->ram, 0x20000)) {
    fprintf(stderr, "@%d: Memory compare failed (mine != theirs, prev):\n", snes_frame_counter);
    int j = 0;
    for (size_t i = 0; i < 0x20000; i++) {
      if (a->ram[i] != b->ram[i]) {
        if (++j < 256) {
          if (((i & 1) == 0 || i < 0x10000) && a->ram[i + 1] != b->ram[i + 1]) {
            fprintf(stderr, "0x%.6X: %.4X != %.4X (%.4X)\n", (int)i,
                    WORD(b->ram[i]), WORD(a->ram[i]), WORD(prev->ram[i]));
            i++, j++;
          } else {
            fprintf(stderr, "0x%.6X: %.2X != %.2X (%.2X)\n", (int)i, b->ram[i], a->ram[i], prev->ram[i]);
          }
        }
      }
    }
    if (j)
      g_fail = true;
    fprintf(stderr, "  total of %d failed bytes\n", (int)j);
  }

  if (memcmp(b->sram, a->sram, 0x2000)) {
    fprintf(stderr, "@%d: SRAM compare failed (mine != theirs, prev):\n", snes_frame_counter);
    int j = 0;
    for (size_t i = 0; i < 0x2000; i++) {
      if (a->sram[i] != b->sram[i]) {
        if (++j < 128) {
          if ((i & 1) == 0 && a->sram[i + 1] != b->sram[i + 1]) {
            fprintf(stderr, "0x%.6X: %.4X != %.4X (%.4X)\n", (int)i,
                    WORD(b->sram[i]), WORD(a->sram[i]), WORD(prev->sram[i]));
            i++, j++;
          } else {
            fprintf(stderr, "0x%.6X: %.2X != %.2X (%.2X)\n", (int)i, b->sram[i], a->sram[i], prev->sram[i]);
          }
        }
      }
    }
    if (j)
      g_fail = true;
    fprintf(stderr, "  total of %d failed bytes\n", (int)j);
  }
#if 1
  if (memcmp(b->vram, a->vram, sizeof(uint16) * 0x8000)) {
    fprintf(stderr, "@%d: VRAM compare failed (mine != theirs, prev):\n", snes_frame_counter);
    for (size_t i = 0, j = 0; i < 0x8000; i++) {
      if (a->vram[i] != b->vram[i]) {
        fprintf(stderr, "0x%.6X: %.4X != %.4X (%.4X)\n", (int)i, b->vram[i], a->vram[i], prev->vram[i]);
        g_fail = true;
        if (++j >= 32)
          break;
      }
    }
  }
  if (memcmp(b->oam, a->oam, sizeof(uint16) * 0x120)) {
    fprintf(stderr, "@%d: VRAM OAM compare failed (mine != theirs, prev):\n", snes_frame_counter);
    for (size_t i = 0, j = 0; i < 0x120; i++) {
      if (a->oam[i] != b->oam[i]) {
        fprintf(stderr, "0x%.6X: %.4X != %.4X (%.4X)\n", (int)i, b->oam[i], a->oam[i], prev->oam[i]);
        g_fail = true;
        if (++j >= 16)
          break;
      }
    }
  }

#endif
}

static void MakeSnapshot(Snapshot *s) {
  Cpu *c = g_cpu;
  s->a = c->a, s->x = c->x, s->y = c->y;
  s->sp = c->sp, s->dp = c->dp, s->db = c->db;
  s->pc = c->pc, s->k = c->k;
  s->flags = cpu_getFlags(c);
  s->vTimer = g_snes->vTimer;
  memcpy(s->ram, g_snes->ram, 0x20000);
  memcpy(s->sram, g_snes->cart->ram, g_snes->cart->ramSize);
  memcpy(s->vram, g_snes->ppu->vram, sizeof(uint16) * 0x8000);
  memcpy(s->oam, g_snes->ppu->oam, sizeof(uint16) * 0x120);
}

static void MakeMySnapshot(Snapshot *s) {
  memcpy(s->ram, g_snes->ram, 0x20000);
  memcpy(s->sram, g_snes->cart->ram, g_snes->cart->ramSize);
  memcpy(s->vram, g_snes->ppu->vram, sizeof(uint16) * 0x8000);
  memcpy(s->oam, g_snes->ppu->oam, sizeof(uint16) * 0x120);
}

static void RestoreSnapshot(Snapshot *s) {
  Cpu *c = g_cpu;
  c->a = s->a, c->x = s->x, c->y = s->y;
  c->sp = s->sp, c->dp = s->dp, c->db = s->db;
  c->pc = s->pc, c->k = s->k;
  g_snes->vTimer = s->vTimer;
  cpu_setFlags(c, s->flags);
  memcpy(g_snes->ram, s->ram, 0x20000);
  memcpy(g_snes->cart->ram, s->sram, 0x2000);
  memcpy(g_snes->ppu->vram, s->vram, sizeof(uint16) * 0x8000);
  memcpy(g_snes->ppu->oam, s->oam, sizeof(uint16) * 0x120);
}

int RunAsmCode(uint32 pc, uint16 a, uint16 x, uint16 y, int flags) {
  uint16 org_sp = g_cpu->sp;
  uint16 org_pc = g_cpu->pc;
  uint8 org_b = g_cpu->db;
  uint16 org_dp = g_cpu->dp;

  printf("RunAsmCode!\n");
  g_ram[0x1ffff] = 1;

  bool dc = g_snes->debug_cycles;
  g_cpu->db = pc >> 16;

  g_cpu->a = a;
  g_cpu->x = x;
  g_cpu->y = y;
  g_cpu->spBreakpoint = g_cpu->sp;
  g_cpu->k = (pc >> 16);
  g_cpu->pc = (pc & 0xffff);
  g_cpu->mf = (flags & 1);
  g_cpu->xf = (flags & 2) >> 1;
  g_calling_asm_from_c = true;
  while (g_calling_asm_from_c) {
    uint32 pc = g_snes->cpu->k << 16 | g_snes->cpu->pc;
    if (g_snes->debug_cycles) {
      char line[80];
      getProcessorStateCpu(g_snes, line);
      puts(line);
      line[0] = 0;
    }
    cpu_runOpcode(g_cpu);
    while (g_snes->dma->dmaBusy)
      dma_doDma(g_snes->dma);

    if (flags & 1) {
      for(int i = 0; i < 10; i++)
        apu_cycle(g_snes->apu);
    }
  }
  g_cpu->dp = org_dp;
  g_cpu->sp = org_sp;
  g_cpu->db = org_b;
  g_cpu->pc = org_pc;

  g_snes->debug_cycles = dc;

  return g_calling_asm_from_c_ret;
}

static bool loadRom(const char *name, Snes *snes) {
  size_t length = 0;
  uint8_t *file = NULL;
  file = ReadWholeFile(name, &length);
  if (file == NULL) {
    puts("Failed to read file");
    return false;
  }
  bool result = snes_loadRom(snes, file, (int)length);
  free(file);
  return result;
}

void PatchBytes(uint32 addr, const uint8 *value, size_t n) {
  for(size_t i = 0; i != n; i++)
    SnesRomPtr(addr)[i] = value[i];
}

// Patches add/sub to ignore carry
void FixupCarry(uint32 addr) {
  *SnesRomPtr(addr) = 0;
}

void RtlUpdateSnesPatchForBugfix() {
  // Patch HandleMessageBoxInteraction logic
  { uint8 t[] = { 0x20, 0x50, 0x96, 0x60 }; PatchBytes(0x8584A3, t, sizeof(t)); }
  // while ((bug_fix_counter < 1 ? joypad1_newkeys : joypad1_lastkeys) == 0);
  { uint8 t[] = { 0x20, 0x36, 0x81, 0x22, 0x59, 0x94, 0x80, 0xc2, 0x30, 0xa5, (bug_fix_counter < 1) ? 0x8f : 0x8b, 0xf0, 0xf3, 0x60 }; PatchBytes(0x859650, t, sizeof(t)); }
  { uint8 t[] = { 0x18, 0x18 }; PatchBytes(0x8584CC, t, sizeof(t)); }  // Don't wait 2 loops
}

Snes *SnesInit(const char *filename) {
  g_snes = snes_init(g_ram);

  g_cpu = g_snes->cpu;

  bool loaded = loadRom(filename, g_snes);
  if (!loaded) {
    return NULL;
  }

  g_sram = g_snes->cart->ram;
  g_rom = g_snes->cart->rom;

  RtlSetupEmuCallbacks(NULL, &RtlRunFrameCompare, NULL);

  // Ensure it will run reset first.
  coroutine_state_0 = 1;

#if 1
  { uint8 t[3] = { 0x20, 0x0f, 0xf7 }; PatchBytes(0x82896b, t, 3); }
  { uint8 t[3] = { 0x7c, 0x81, 0x89 }; PatchBytes(0x82F70F, t, 3); }

  // Some code called by GameState_37_CeresGoesBoomWithSamus_ forgets to clear the M flag
  { uint8 t[] = { 0x5f, 0xf7 }; PatchBytes(0x8BA362, t, sizeof(t)); }
  { uint8 t[] = { 0xc2, 0x20, 0x4c, 0x67, 0xa3 }; PatchBytes(0x8BF760, t, sizeof(t)); }
  //{ uint8 t[3] = { 0x60 }; PatchBytes(0x808028, t, 1); }  // Apu_UploadBank hangs
  { uint8 t[2] = { 0x0a, 0x0a }; PatchBytes(0x8584B2, t, 2); }  // HandleMessageBoxInteraction has a loop

  // LoadRoomPlmGfx passes bad value
  { uint8 t[] = { 0xc0, 0x00, 0x00, 0xf0, 0x03, 0x20, 0x64, 0x87, 0x60}; PatchBytes(0x84efd3, t, sizeof(t)); }
  { uint8 t[] = { 0xd3, 0xef }; PatchBytes(0x848243, t, sizeof(t)); }

  // EprojColl_8676 doesn't initialize Y
  { uint8 t[] = { 0xac, 0x91, 0x19, 0x4c, 0x76, 0x86 }; PatchBytes(0x86f4a6, t, sizeof(t)); }
  { uint8 t[] = { 0xa6, 0xf4 }; PatchBytes(0x8685bd, t, sizeof(t)); }

  // Fix so main code is in a function.
  { uint8 t[] = { 0xc2, 0x30, 0x22, 0x59, 0x94, 0x80, 0x20, 0x48, 0x89, 0x22, 0x38, 0x83, 0x80, 0x4C, 0x13, 0xF7 }; PatchBytes(0x82f713, t, sizeof(t)); }
  { uint8 t[] = { 0x58, 0x4c, 0x13, 0xf7 }; PatchBytes(0x828944, t, sizeof(t)); }
  { uint8 t[] = { 0x28, 0x60 }; PatchBytes(0x82897a, t, sizeof(t)); }

  // Remove IO_HVBJOY loop in ReadJoypadInput
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x80945C, t, sizeof(t)); }

  // Fix so NorfairLavaMan_Func_12 initializes Y
  { uint8 t[] = { 0xbc, 0xaa, 0x0f, 0xc9, 0x6c, 0x00, 0x10, 0x1a }; PatchBytes(0xa8b237, t, sizeof(t)); }

  // MaridiaBeybladeTurtle_Func8 negate
  { uint8 t[] = { 0x49, 0xff, 0xff, 0x69, 0x00, 0x00 }; PatchBytes(0xa2904b, t, sizeof(t)); }
  { uint8 t[] = { 0x49, 0xff, 0xff, 0x69, 0x00, 0x00 }; PatchBytes(0xa29065, t, sizeof(t)); }

  // Remove DebugLoadEnemySetData
  { uint8 t[] = { 0x6b }; PatchBytes(0xA0896F, t, sizeof(t)); }
  // MotherBrainsTubesFalling_Falling wrong X value
  { uint8 t[] = { 0x18, 0x18, 0x18 }; PatchBytes(0xA98C12, t, sizeof(t)); }

  { uint8 t[] = { 0x60 }; PatchBytes(0x8085F6, t, sizeof(t)); }

  // Remove 4 frames of delay in reset routine
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x80843C, t, sizeof(t)); }
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x808475, t, sizeof(t)); }
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x808525, t, sizeof(t)); }

  // Remove WaitUntilEndOfVblank in WaitUntilEndOfVblankAndClearHdma - We run frame by frame.
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x8882A1, t, sizeof(t)); }

  // Remove WaitForNMI in GameState_41_TransitionToDemo.
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x828533, t, sizeof(t)); }

  // WaitForNMI in ScreenOfWaitNmi / ScreenOnWaitNMI
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x80837B, t, sizeof(t)); }
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x80838E, t, sizeof(t)); }

  // WaitUntilEndOfVblankAndEnableIrq
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x82DF6C, t, sizeof(t)); }

  // Remove loops based on door_transition_vram_update_enabled
  // Replace with a call to Irq_DoorTransitionVramUpdate
  { uint8 t[] = { 0x20, 0x32, 0x96, 0x6b }; PatchBytes(0x80d000, t, sizeof(t)); }
  { uint8 t[] = { 0x22, 0x00, 0xd0, 0x80, 0x18 }; PatchBytes(0x82E02C, t, sizeof(t)); }
  { uint8 t[] = { 0x22, 0x00, 0xd0, 0x80, 0x18 }; PatchBytes(0x82E06B, t, sizeof(t)); }
  { uint8 t[] = { 0x22, 0x00, 0xd0, 0x80, 0x18 }; PatchBytes(0x82E50D, t, sizeof(t)); }
  { uint8 t[] = { 0x22, 0x00, 0xd0, 0x80, 0x18 }; PatchBytes(0x82E609, t, sizeof(t)); }

  // Remove infinite loop polling door_transition_flag (AD 31 09 10 FB)
  { uint8 t[] = { 0x22, 0x04, 0xd0, 0x80, 0x18 }; PatchBytes(0x82E526, t, sizeof(t)); }
  { uint8 t[] = { 0x22, 0x38, 0x83, 0x80, 0xad, 0x31, 0x09, 0x10, 0xf7, 0x6b }; PatchBytes(0x80d004, t, sizeof(t)); }

  // Remove WaitForNMI in DoorTransitionFunction_LoadMoreThings_Async
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x82E540, t, sizeof(t)); }

  // Remove WaitForNMI in CinematicFunctionBlackoutFromCeres
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x8BC11E, t, sizeof(t)); }

  // Remove WaitForNMI in CinematicFunctionEscapeFromCeres
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x8BD487, t, sizeof(t)); }

  // Patch InitializePpuForMessageBoxes
  { uint8 t[] = { 0x18, 0x18, 0x18 }; PatchBytes(0x858148, t, sizeof(t)); } // WaitForLagFrame
  { uint8 t[] = { 0x18, 0x18, 0x18 }; PatchBytes(0x8581b2, t, sizeof(t)); } // WaitForLagFrame
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x8581EA, t, sizeof(t)); } // HandleMusicQueue etc

  // Patch ClearMessageBoxBg3Tilemap
  { uint8 t[] = { 0x18, 0x18, 0x18 }; PatchBytes(0x858203, t, sizeof(t)); } // WaitForLagFrame
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x858236, t, sizeof(t)); } // HandleMusicQueue etc

  // Patch WriteMessageTilemap
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x8582B8, t, sizeof(t)); }

  // Patch SetupPpuForActiveMessageBox
  { uint8 t[] = { 0x18, 0x18, 0x18 }; PatchBytes(0x858321, t, sizeof(t)); } // WaitForLagFrame
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x85835A, t, sizeof(t)); } // InitializePpuForMessageBoxes

  // Patch ToggleSaveConfirmationSelection
  { uint8 t[] = { 0x18, 0x18, 0x18 }; PatchBytes(0x858532, t, sizeof(t)); } // WaitForNMI_NoUpdate
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x85856b, t, sizeof(t)); } // HandleMusicQueue etc.

  // Patch DisplayMessageBox
  { uint8 t[] = { 0x18, 0x18, 0x18 }; PatchBytes(0x858096, t, sizeof(t)); } // Remove MsgBoxDelayFrames_2
  { uint8 t[] = { 0x18, 0x18, 0x18 }; PatchBytes(0x8580B4, t, sizeof(t)); } // Remove MsgBoxDelayFrames_2
  { uint8 t[] = { 0x18, 0x18, 0x18 }; PatchBytes(0x8580DC, t, sizeof(t)); } // Remove MsgBoxDelayFrames_2
  { uint8 t[] = { 0x18, 0x18, 0x18 }; PatchBytes(0x8580F2, t, sizeof(t)); } // Remove MsgBoxDelayFrames_2

  // Patch RestorePpuForMessageBox
  { uint8 t[] = { 0x18, 0x18, 0x18 }; PatchBytes(0x85861C, t, sizeof(t)); } // WaitForNMI_NoUpdate
  { uint8 t[] = { 0x18, 0x18, 0x18 }; PatchBytes(0x858651, t, sizeof(t)); } // WaitForNMI_NoUpdate
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x858692, t, sizeof(t)); } // HdmaObjectHandler
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x858696, t, sizeof(t)); } // HandleSoundEffects

  // Patch Fix_MsgBoxMakeHdmaTable_NoSleep
  { uint8 t[] = { 0x08, 0xc2, 0x30, 0x4c, 0xa9, 0x85 }; PatchBytes(0x859660, t, sizeof(t)); }
  { uint8 t[] = { 0x20, 0x60, 0x96 }; PatchBytes(0x8583BA, t, sizeof(t)); } // MsgBoxMakeHdmaTable

  // Patch GunshipTop_13 to not block
  { uint8 t[] = { 0x22, 0x81, 0x96, 0x85, 0xc9, 0xff, 0xff, 0xd0, 0x04, 0x5c, 0x5f, 0xab, 0xa2, 0x5c, 0x26, 0xab, 0xa2 }; PatchBytes(0x859670, t, sizeof(t)); } // DisplayMessageBox_DoubleRet
  { uint8 t[] = { 0xcd, 0x1f, 0x1c, 0xd0, 0x08, 0x9c, 0x1f, 0x1c, 0xad, 0xf9, 0x05, 0x6b, 0xff, 0x8d, 0xc8, 0x0d, 0xa9, 0xff, 0xff, 0x6b }; PatchBytes(0x859681, t, sizeof(t)); } // DisplayMessageBox_Poll
  { uint8 t[] = { 0x5c, 0x70, 0x96, 0x85 }; PatchBytes(0xa2ab22, t, sizeof(t)); } // GunshipTop_13

  // EnemyMain_WithCheckMsgBox
  { uint8 t[] = { 0x22, 0xd4, 0x8f, 0xa0, 0xad, 0xc8, 0x0d, 0xf0, 0x07, 0x22, 0x95, 0x96, 0x85, 0x9c, 0xc8, 0x0d, 0x6b }; PatchBytes(0x8596a0, t, sizeof(t)); }
  { uint8 t[] = { 0x22, 0xa0, 0x96, 0x85 }; PatchBytes(0x828b65, t, sizeof(t)); } // EnemyMain -> EnemyMain_WithCheckMsgBox

  // CloseMessageBox_ResetMsgBoxIdx
  { uint8 t[] = { 0x20, 0x89, 0x85, 0xa9, 0x1c, 0x00, 0x8d, 0x1f, 0x1c, 0x60 }; PatchBytes(0x8596C0, t, sizeof(t)); }
  { uint8 t[] = { 0x20, 0xC0, 0x96 }; PatchBytes(0x8580E5, t, sizeof(t)); }

  // ProcessPlm_CheckMessage
  { uint8 t[] = { 0xad, 0xc8, 0x0d, 0xf0, 0x11, 0x98, 0x9d, 0x27, 0x1d, 0xad, 0xc8, 0x0d, 0x22, 0x95, 0x96, 0x85, 0x9c, 0xc8, 0x0d, 0xbc, 0x27, 0x1d, 0x4c, 0xee, 0x85 }; PatchBytes(0x84EFDC, t, sizeof(t)); }
  { uint8 t[] = { 0xf4, 0xdb, 0xef }; PatchBytes(0x8485f7, t, sizeof(t)); }

  // Hook DisplayMessageBox so it writes to queued_message_box_index instead
  { uint8 t[] = { 0x08, 0x8b, 0xda, 0x5a, 0x5c, 0x84, 0x80, 0x85 }; PatchBytes(0x859695, t, sizeof(t)); } // DisplayMessageBox_Org
  { uint8 t[] = { 0x8d, 0xc8, 0x0d, 0x6b }; PatchBytes(0x858080, t, sizeof(t)); } // Hook

  // PlmInstr_ActivateSaveStationAndGotoIfNo_Fixed
  { uint8 t[] = { 0x22, 0x81, 0x96, 0x85, 0xc9, 0xff, 0xff, 0xf0, 0x04, 0x5c, 0xfa, 0x8c, 0x84, 0x7a, 0xfa, 0x88, 0x88, 0x60 }; PatchBytes(0x84f000, t, sizeof(t)); } // Restart if -1
  { uint8 t[] = { 0x5c, 0x00, 0xf0, 0x84 }; PatchBytes(0x848cf6, t, sizeof(t)); } // PlmInstr_ActivateSaveStationAndGotoIfNo

  // SoftReset
  { uint8 t[] = { 0xa9, 0xff, 0xff, 0x8d, 0x98, 0x09, 0x60 }; PatchBytes(0x81F000, t, sizeof(t)); }
  { uint8 t[] = { 0x5c, 0x00, 0xf0, 0x81 }; PatchBytes(0x819027, t, sizeof(t)); }
  { uint8 t[] = { 0x5c, 0x00, 0xf0, 0x81 }; PatchBytes(0x819112, t, sizeof(t)); }
  { uint8 t[] = { 0x5c, 0x00, 0xf0, 0x81 }; PatchBytes(0x8194e9, t, sizeof(t)); }

  // Remove ReadJoypadInputs from Vector_NMI
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x8095E1, t, sizeof(t)); } // callf   ReadJoypadInputs

  // Remove APU_UploadBank
  if (g_use_my_apu_code)
    { uint8 t[] = { 0x60 }; PatchBytes(0x808028, t, sizeof(t)); }

  // Remove reads from IO_APUI01 etc
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x80 }; PatchBytes(0x828A59, t, sizeof(t)); } // SfxHandlers_1_WaitForAck
  { uint8 t[] = { 0x18, 0x18, 0x18 }; PatchBytes(0x828A72, t, sizeof(t)); } // SfxHandlers_2_ClearRequest
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x80 }; PatchBytes(0x828A80, t, sizeof(t)); } // SfxHandlers_3_WaitForAck
  { uint8 t[] = { 0x06 }; PatchBytes(0x828A67, t, sizeof(t)); } // sfx_clear_delay

  // LoadStdBG3andSpriteTilesClearTilemaps does DMA from RAM
  { uint8 t[] = { 0x00, 0x2E }; PatchBytes(0x82831E, t, sizeof(t)); }

  { uint8 t[] = { 0xa5, 0x25 }; PatchBytes(0x91C234, t, sizeof(t)); } // Bugfix in XrayHdmaFunc_BeamAimedUUL

  // Remove call to InitializeMiniMapBroken
  { uint8 t[] = { 0x18, 0x18, 0x18, 0x18 }; PatchBytes(0x809AF3, t, sizeof(t)); } // callf   InitializeMiniMapBroken

  // NormalEnemyShotAiSkipDeathAnim_CurEnemy version that preserves R18 etc.
  { uint8 t[] = { 0xA5, 0x12, 0x48, 0xA5, 0x14, 0x48, 0xA5, 0x16, 0x48, 0x22, 0xA7, 0xA6, 0xA0, 0x68, 0x85, 0x16, 0x68, 0x85, 0x14, 0x68, 0x85, 0x12, 0x6B }; PatchBytes(0xA7FF82, t, sizeof(t)); }
  { uint8 t[] = { 0x22, 0x82, 0xff, 0xa7 }; PatchBytes(0xa7b03a, t, sizeof(t)); }


  RtlUpdateSnesPatchForBugfix();

  for (size_t i = 0; i != arraysize(kPatchedCarrys); i++) {
    uint8 t = *SnesRomPtr(kPatchedCarrys[i]);
    if (t) {
      kPatchedCarrysOrg[i] = t;
      FixupCarry(kPatchedCarrys[i]);
    } else {
      printf("0x%x double patched!\n", kPatchedCarrys[i]);
    }
  }

  PatchBugs(1, 0);
#endif
  return g_snes;
}

void DebugGameOverMenu(void) {
  assert(0);
}

uint32 RunCpuUntilPC(uint32 pc1, uint32 pc2) {
  for(;;) {
    if (g_snes->debug_cycles) {
      char line[80];
      getProcessorStateCpu(g_snes, line);
      puts(line);
    }
    cpu_runOpcode(g_cpu);

    uint32 addr = g_snes->cpu->k << 16 | g_snes->cpu->pc;
    if (addr == pc1 || addr == pc2)
      return addr;
  }
}

void RunOneFrameOfGame_Emulated(void) {
  uint16 bug_fix_bak = bug_fix_counter;
  // Execute until either WaitForNMI or WaitForLagFrame
  RunCpuUntilPC(0x808343, 0x85813C);

  // Trigger nmi, then run until WaitForNMI or WaitForLagFrame returns
  g_snes->cpu->nmiWanted = true;
  RunCpuUntilPC(0x80834A, 0x858142);

  bug_fix_counter = bug_fix_bak;
}

void DrawFrameToPpu(void) {
  g_snes->hPos = g_snes->vPos = 0;
  while (!g_snes->cpu->nmiWanted) {
    do {
      snes_handle_pos_stuff(g_snes);
    } while (g_snes->hPos != 0);
    if (g_snes->vIrqEnabled && (g_snes->vPos - 1) == g_snes->vTimer) {
      Vector_IRQ();
    }
  }
  g_snes->cpu->nmiWanted = false;
}

void SaveBugSnapshot() {
  if (!g_debug_flag && g_got_mismatch_count == 0) {
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "saves/bug-%d.sav", (int)time(NULL));
    RtlSaveSnapshot(buffer, true);
  }
  g_got_mismatch_count = 5 * 60;
}

void RunOneFrameOfGame_Both(void) {
  g_snes->ppu = g_snes->snes_ppu;
  MakeSnapshot(&g_snapshot_before);

  // Run orig version then snapshot
again_theirs:
  g_snes->runningWhichVersion = 1;
  RunOneFrameOfGame_Emulated();
  DrawFrameToPpu();
  MakeSnapshot(&g_snapshot_theirs);

  // Run my version and snapshot
//again_mine:
  g_snes->ppu = g_snes->my_ppu;
  RestoreSnapshot(&g_snapshot_before);

  g_snes->runningWhichVersion = 2;
  RunOneFrameOfGame();
  DrawFrameToPpu();
  MakeSnapshot(&g_snapshot_mine);

  g_snes->runningWhichVersion = 0xff;

  // Compare both snapshots
  VerifySnapshotsEq(&g_snapshot_mine, &g_snapshot_theirs, &g_snapshot_before);

  if (g_fail) {
    g_fail = false;

    printf("Verify failure!\n");

    g_snes->ppu = g_snes->snes_ppu;
    RestoreSnapshot(&g_snapshot_before);

    if (g_debug_flag)
      goto again_theirs;

    SaveBugSnapshot();
    RunOneFrameOfGame_Emulated();
    goto getout;
  }

  g_snes->ppu = g_snes->snes_ppu;
  RestoreSnapshot(&g_snapshot_theirs);
getout:
  g_snes->ppu = g_other_image ? g_snes->my_ppu : g_snes->snes_ppu;
  g_snes->runningWhichVersion = 0;

  // Trigger soft reset?
  if (game_state == 0xffff) {
    g_snes->cpu->k = 0x80;
    g_snes->cpu->pc = 0x8462;
    coroutine_state_0 = 3;
  }

  if (menu_index & 0xff00) {
    printf("MENU INDEX TOO BIG!\n");
    SaveBugSnapshot();
    menu_index &= 0xff;
  }


  if (g_got_mismatch_count)
    g_got_mismatch_count--;
}

void RtlRunFrameCompare(uint16 input, int run_what) {
  g_snes->input1->currentState = input;

  if (g_runmode == RM_THEIRS) {
    RunOneFrameOfGame_Emulated();
    DrawFrameToPpu();

  } else if (g_runmode == RM_MINE) {
    g_use_my_apu_code = true;

    g_snes->runningWhichVersion = 0xff;
    RunOneFrameOfGame();
    DrawFrameToPpu();
    g_snes->runningWhichVersion = 0;
  } else {
    g_use_my_apu_code = true;
    RunOneFrameOfGame_Both();
  }
}
