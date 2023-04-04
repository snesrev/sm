// Enemy AI - inc. Kraid (complete) & Phantoon
#include "ida_types.h"
#include "variables.h"
#include "funcs.h"
#include "enemy_types.h"
#include "sm_rtl.h"

#undef r18

#define kKraid_Palette2 ((uint16*)RomFixedPtr(0xa786c7))
#define kKraid_Palette2 ((uint16*)RomFixedPtr(0xa786c7))
#define kKraid_BgTargetPalette3 ((uint16*)RomFixedPtr(0xa7aaa6))
#define g_word_A7ACB3 ((uint16*)RomFixedPtr(0xa7acb3))
#define g_stru_A796D2 (*(KraidInstrList*)RomFixedPtr(0xa796d2))
#define g_stru_A796DA (*(KraidInstrList*)RomFixedPtr(0xa796da))
#define g_word_A7B161 ((uint16*)RomFixedPtr(0xa7b161))
#define kKraid_BgPalette7 ((uint16*)RomFixedPtr(0xa7b3d3))
#define kKraid_BgPalette7_KraidDeath ((uint16*)RomFixedPtr(0xa7b4f3))
#define kKraid_SprPalette7_KraidDeath ((uint16*)RomFixedPtr(0xa7b513))
#define g_stru_A7974A ((KraidInstrList*)RomFixedPtr(0xa7974a))
#define g_stru_A79764 ((KraidInstrList*)RomFixedPtr(0xa79764))
#define g_stru_A792B7 (*(Hitbox*)RomFixedPtr(0xa792b7))
#define g_word_A7BA7D ((uint16*)RomFixedPtr(0xa7ba7d))
#define g_word_A7BC65 ((uint16*)RomFixedPtr(0xa7bc65))
#define kKraid_Ilist_8B0A (*(SpriteDrawInstr*)RomFixedPtr(0xa78b0a))
#define g_off_A7BE3E ((uint16*)RomFixedPtr(0xa7be3e))
#define g_off_A7BE46 ((uint16*)RomFixedPtr(0xa7be46))
#define g_word_A7BF1D ((uint16*)RomFixedPtr(0xa7bf1d))
#define kKraidSinkEntry ((KraidSinkTable*)RomFixedPtr(0xa7c5e7))
#define g_off_A7CE8E ((uint16*)RomFixedPtr(0xa7ce8e))
#define g_word_A7CDED ((uint16*)RomFixedPtr(0xa7cded))
#define g_stru_A7902D ((ExtendedSpriteMap*)RomFixedPtr(0xa7902d))
#define g_byte_A7CFC2 ((uint8*)RomFixedPtr(0xa7cfc2))
#define g_off_A7CCFD ((uint16*)RomFixedPtr(0xa7ccfd))
#define g_word_A7CD41 ((uint16*)RomFixedPtr(0xa7cd41))
#define g_word_A7CD53 ((uint16*)RomFixedPtr(0xa7cd53))
#define g_word_A7CD63 ((uint16*)RomFixedPtr(0xa7cd63))
#define g_off_A7D40D ((uint16*)RomFixedPtr(0xa7d40d))
#define g_word_A7CDAD ((uint16*)RomFixedPtr(0xa7cdad))
#define g_byte_A7DA1D ((uint8*)RomFixedPtr(0xa7da1d))
#define g_off_A7DC4A ((uint16*)RomFixedPtr(0xa7dc4a))
#define g_off_A7F787 ((uint16*)RomFixedPtr(0xa7f787))
#define g_off_A7F92D ((uint16*)RomFixedPtr(0xa7f92d))
#define g_word_A7CA41 ((uint16*)RomFixedPtr(0xa7ca41))
#define g_word_A7CA61 ((uint16*)RomFixedPtr(0xa7ca61))
#define g_off_A7F55F ((uint16*)RomFixedPtr(0xa7f55f))

static const uint16 g_word_A7A916 = 0x120;
static const uint16 g_word_A7A918 = 0xa0;
static const uint16 g_word_A7A91A = 0x40;
static const uint16 g_word_A7A91C = 3;
static const uint16 g_word_A7A920 = 3;
static const uint16 g_word_A7A922 = 4;
static const uint16 g_word_A7A926 = 0x8000;
static const uint16 g_word_A7A928 = 3;
static const uint16 g_word_A7CD73 = 0x600;
static const uint16 g_word_A7CD75 = 0;
static const uint16 g_word_A7CD77 = 0x1000;
static const uint16 g_word_A7CD79 = 0;
static const uint16 g_word_A7CD7B = 2;
static const uint16 g_word_A7CD7D = 7;
static const uint16 g_word_A7CD7F = 0;
static const uint16 g_word_A7CD81 = 0x600;
static const uint16 g_word_A7CD83 = 0;
static const uint16 g_word_A7CD85 = 0x1000;
static const uint16 g_word_A7CD87 = 0;
static const uint16 g_word_A7CD89 = 0xfffe;
static const uint16 g_word_A7CD8B = 0xfff9;
static const uint16 g_word_A7CD8D = 0;
static const uint16 g_word_A7CD9B = 0x40;
static const uint16 g_word_A7CD9D = 0xc00;
static const uint16 g_word_A7CD9F = 0x100;
static const uint16 g_word_A7CDA1 = 0xf000;
static const uint16 g_word_A7CDA3 = 8;
static const uint8 g_byte_A7CDA5[8] = { 6, 6, 8, 8, 6, 8, 6, 8 };
static const uint16 g_word_A7E900 = 0xfffd;
static const uint16 g_word_A7E902 = 0;
static const uint16 g_word_A7E906 = 0;
static const uint16 g_word_A7E904 = 0xfffc;
static const uint16 g_word_A7E908 = 2;
static const uint16 g_word_A7E90A = 0;
static const uint16 g_word_A7E90C = 0xfffe;
static const uint16 g_word_A7E90E = 0;
static const uint16 g_word_A7E910 = 0x40;

static const uint16 g_word_A7F4C9 = 0x60;
static const uint16 g_word_A7F4CD = 0x78;
static const uint16 g_word_A7F4CF = 0x3c;
static const uint16 g_word_A7F4D1 = 1;
static const uint16 g_word_A7F4D3 = 8;
static const uint16 g_word_A7F4D5 = 8;
static const uint16 g_word_A7F4D7 = 0;
static const uint16 g_word_A7F4D9 = 0;
static const uint16 g_word_A7F4DB = 0x1000;

void Kraid_SpawnExplosionEproj(uint16 k);
void Kraid_SpawnPlmToClearCeiling(void);
void Kraid_SpawnRandomQuakeProjs(void);
void Kraid_ClearSomeSpikes(void);
void Kraid_SetupGfxWithTilePrioClear(uint16 r18);
void Kraid_Shot_Mouth(void);
void Kraid_Death_UpdateBG2TilemapBottomHalf(void);
void Kraid_Death_UpdateBG2TilemapTopHalf(void);
void Kraid_Palette_Handling(void);
void Kraid_Shot_Body(void);
void Kraid_Enemy_Touch(void);
void Kraid_UpdateBG2TilemapBottomHalf(void);
void Kraid_UpdateBg2TilemapTopHalf(void);
void Kraid_HurtFlash_Handling(void);
void Kraid_HealthBasedPaletteHandling(void);
void Kraid_Shot_GlowHisEye(void);
void Kraid_Shot_MouthIsOpen(void);
void Kraid_Shot_UnglowEye(void);
void Kraid_HandleFirstPhase(void);
void Kraid_TransferTopHalfToVram(void);
void Kraid_RestrictSamusXtoFirstScreen_2(void);
void Kraid_EnemyTouch_Lint(uint16 k);
void CallKraidSinkTableFunc(uint32 ea);
void CallKraidFunc(uint32 ea);

void CallEnemyInstrExtFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnPhantoon_Func_1: Phantoon_Func_1(); return;
  case fnPhantoon_Func_4: Phantoon_Func_4(k); return;
  case fnPhantoon_StartTrackingSamusAndInitEyeTimer: Phantoon_StartTrackingSamusAndInitEyeTimer(); return;
  case fnPhantoon_PickPatternForRound2: Phantoon_PickPatternForRound2(); return;
  default: Unreachable();
  }
}

const uint16 *EnemyInstr_Call_A7(uint16 k, const uint16 *jp) {  // 0xA7808A
  CallEnemyInstrExtFunc(jp[0] | 0xA70000, k);
  return jp + 1;
}


void Enemy_GrappleReact_NoInteract_A7(void) {  // 0xA78000
  SwitchEnemyAiToMainAi();
}

void Enemy_GrappleReact_CancelBeam_A7(void) {  // 0xA7800F
  Enemy_SwitchToFrozenAi();
}

void Enemy_NormalShotAI_A7(void) {  // 0xA7802D
  NormalEnemyShotAi();
}

void Enemy_NormalPowerBombAI_SkipDeathAnim_A7(void) {  // 0xA7803C
  NormalEnemyPowerBombAiSkipDeathAnim_CurEnemy();
}

void Enemy_NormalFrozenAI_A7(void) {  // 0xA78041
  NormalEnemyFrozenAI();
}

const uint16 *Kraid_Instr_9(uint16 k, const uint16 *jp) {  // 0xA78A8F
  Enemy_Kraid *E = Get_Kraid(0);
  if ((int16)(E->base.health - E->kraid_healths_8ths[3]) < 0) {
    Enemy_Kraid *E1 = Get_Kraid(0x40);
    if (sign16(E1->base.current_instruction + 0x75BF))
      return INSTR_RETURN_ADDR(addr_kKraid_Ilist_8A41);
  }
  return jp;
}

void Kraid_Touch_ArmFoot(void) {  // 0xA7948B
  NormalEnemyTouchAi();
}

void Kraid_Func_1(void) {  // 0xA794A4
  extra_samus_x_displacement = 4;
  extra_samus_y_displacement = -8;
}

void Kraid_Touch(void) {  // 0xA7949F
  NormalEnemyTouchAiSkipDeathAnim_CurEnemy();
}

void Kraid_Shot(void) {  // 0xA794B1
  NormalEnemyShotAi();
}

void KraidsArm_Touch(void) {  // 0xA79490
  if (!samus_invincibility_timer) {
    Kraid_Func_1();
    gEnemyData(0)[4].ai_var_A = FUNC16(KraidLint_FireLint);
    Kraid_Touch();
  }
}


void Kraid_Arm_Shot(uint16 j) {  // 0xA794B6
  Kraid_SpawnExplosionEproj(j);
  projectile_dir[j >> 1] |= 0x10;
}

void sub_A7A92A(void) {  // 0xA7A92A
  Kraid_CheckIfDead();
}

uint16 Kraid_CheckIfDead(void) {  // 0xA7A92C
  return (*(uint16 *)&boss_bits_for_area[area_index] & 1) != 0;
}

void Kraid_SetEnemyPropsToDead(void) {  // 0xA7A943
  Enemy_Kraid *E = Get_Kraid(cur_enemy_index);
  E->base.properties = E->base.properties & 0x50FF | 0x700;
}

void Kraid_Init(void) {  // 0xA7A959
  VramWriteEntry *v5;
  uint16 j;

  unpause_hook.bank = -89;
  pause_hook.bank = -89;
  unpause_hook.addr = FUNC16(UnpauseHook_Kraid_IsAlive);
  pause_hook.addr = FUNC16(PauseHook_Kraid);
  if (Kraid_CheckIfDead()) {
    uint16 v0 = 192;
    uint16 v1 = 0;
    do {
      target_palettes[v0 >> 1] = kKraid_Palette2[v1 >> 1];
      v0 += 2;
      v1 += 2;
    } while ((int16)(v1 - 32) < 0);
    for (int i = 2046; i >= 0; i -= 2)
      tilemap_stuff[i >> 1] = 824;
    Enemy_Kraid *E = Get_Kraid(0);
    E->field_4 = 0;
    uint16 v4 = vram_write_queue_tail;
    v5 = gVramWriteEntry(vram_write_queue_tail);
    v5->size = 512;
    v5->src.addr = addr_kKraidsRoomBg;
    v5->src.bank = -89;
    v5->vram_dst = ((reg_BG12NBA & 0xF) << 8) + 16128;
    vram_write_queue_tail = v4 + 7;
    Kraid_SpawnPlmToClearCeiling();
    Kraid_ClearSomeSpikes();
    E->kraid_var_A = FUNC16(Kraid_FadeInBg_ClearBg2TilemapTopHalf);
  } else {
    reg_BG2SC = 67;
    camera_distance_index = 2;
    *(uint16 *)scrolls = 0;
    *(uint16 *)&scrolls[2] = 1;
    Enemy_Kraid *E = Get_Kraid(0);
    E->kraid_min_y_pos_eject = 324;
    uint16 v7 = 0;
    uint16 v8 = E->base.health >> 3;
    uint16 r18 = v8;
    do {
      Get_Kraid(v7)->kraid_healths_8ths[0] = v8;
      v8 += r18;
      v7 += 2;
    } while ((int16)(v7 - 16) < 0);
    Kraid_SetupGfxWithTilePrioClear(0xDFFF);
    uint16 v10 = E->base.health >> 2;
    E->kraid_healths_4ths[0] = v10;
    uint16 v11 = E->kraid_healths_4ths[0] + v10;
    E->kraid_healths_4ths[1] = v11;
    uint16 v12 = E->kraid_healths_4ths[0] + v11;
    E->kraid_healths_4ths[2] = v12;
    E->kraid_healths_4ths[3] = E->kraid_healths_4ths[0] + v12;
    E->field_2E[8] = 0;
    E->base.x_pos = 176;
    E->base.y_pos = 592;
    E->base.properties |= kEnemyProps_Tangible;
    E->kraid_var_A = FUNC16(Kraid_RestrictSamusXtoFirstScreen);
    E->kraid_var_F = 300;
    E->kraid_next = FUNC16(Kraid_RaiseKraidThroughFloor);
    E->kraid_var_C = 64;
    DisableMinimapAndMarkBossRoomAsExplored();
    for (j = 62; (j & 0x8000) == 0; j -= 2)
      tilemap_stuff[(j >> 1) + 2016] = 824;
    earthquake_type = 5;
    uint16 v14 = 0;
    do {
      target_palettes[(v14 >> 1) + 176] = kKraid_BgTargetPalette3[v14 >> 1];
      v14 += 2;
    } while ((int16)(v14 - 32) < 0);
  }
}

void Kraid_SetupGfxWithTilePrioClear(uint16 r18) {  // 0xA7AAC6
  DecompressToMem(0xB9FA38, g_ram + 0x4000);
  DecompressToMem(0xB9FE3E, g_ram + 0x2000);
  Enemy_Kraid *E = Get_Kraid(0);
  E->kraid_hurt_frame = 0;
  E->kraid_hurt_frame_timer = 0;
  uint16 v1 = 0;
  do {
    tilemap_stuff[(v1 >> 1) + 1024] = tilemap_stuff[v1 >> 1] & 0xDFFF;
    v1 += 2;
  } while ((int16)(v1 - 1536) < 0);
  uint16 v2 = 0;
  do {
    tilemap_stuff[v2] = r18 & ram4000.xray_tilemaps[v2];
    ++v2;
  } while ((int16)(v2 * 2 - 2048) < 0);
}

void KraidsArm_Init(void) {  // 0xA7AB43
  if (Kraid_CheckIfDead()) {
    Kraid_SetEnemyPropsToDead();
  } else {
    EnemyData *v0 = gEnemyData(0);
    v0[1].palette_index = v0->palette_index;
    v0[1].ai_var_A = FUNC16(nullsub_234);
    v0[1].current_instruction = addr_kKraid_Ilist_8AA4;
    v0[1].instruction_timer = 1;
    v0[1].ai_var_B = 0;
  }
}

void KraidsTopLint_Init(void) {  // 0xA7AB68
  if (Kraid_CheckIfDead()) {
    Kraid_SetEnemyPropsToDead();
  } else {
    EnemyData *v0 = gEnemyData(0);
    v0[2].palette_index = v0->palette_index;
    v0[2].instruction_timer = 0x7FFF;
    v0[2].current_instruction = addr_kKraid_Ilist_8AFE;
    v0[2].spritemap_pointer = addr_kKraid_Sprmap_A5DF;
    v0[2].ai_var_A = addr_locret_A7B831;
    v0[2].ai_preinstr = 0x7FFF;
    v0[2].ai_var_C = 0;
  }
}

void KraidsMiddleLint_Init(void) {  // 0xA7AB9C
  if (Kraid_CheckIfDead()) {
    Kraid_SetEnemyPropsToDead();
  } else {
    EnemyData *v0 = gEnemyData(0);
    v0[3].palette_index = v0->palette_index;
    v0[3].instruction_timer = 0x7FFF;
    v0[3].current_instruction = addr_kKraid_Ilist_8AFE;
    v0[3].spritemap_pointer = addr_kKraid_Sprmap_A5DF;
    v0[3].ai_var_A = addr_locret_A7B831;
    v0[3].ai_var_C = -16;
  }
}

void KraidsBottomLint_Init(void) {  // 0xA7ABCA
  if (Kraid_CheckIfDead()) {
    Kraid_SetEnemyPropsToDead();
  } else {
    EnemyData *v0 = gEnemyData(0);
    v0[4].palette_index = v0->palette_index;
    v0[4].instruction_timer = 0x7FFF;
    v0[4].current_instruction = addr_kKraid_Ilist_8AFE;
    v0[4].spritemap_pointer = addr_kKraid_Sprmap_A5DF;
    v0[4].ai_var_A = addr_locret_A7B831;
    v0[4].ai_var_C = -16;
  }
}

void KraidsFoot_Init(void) {  // 0xA7ABF8
  if (Kraid_CheckIfDead()) {
    Kraid_SetEnemyPropsToDead();
  } else {
    uint16 palette_index = Get_Kraid(0)->base.palette_index;
    Enemy_Kraid *E = Get_Kraid(0x140);
    E->base.palette_index = palette_index;
    E->base.current_instruction = addr_kKraid_Ilist_86E7;
    E->base.instruction_timer = 1;
    E->kraid_var_A = FUNC16(nullsub_234);
    E->kraid_next = 0;
  }
}

void Kraid_Main(void) {  // 0xA7AC21
  Kraid_Shot_Mouth();
  Kraid_Palette_Handling();
  Kraid_Shot_Body();
  Kraid_Enemy_Touch();
  Enemy_Kraid *E = Get_Kraid(0);
  reg_BG2HOFS = E->base.x_width + reg_BG1HOFS - bg1_x_offset - E->base.x_pos;
  reg_BG2VOFS = layer1_y_pos - E->base.y_pos + 152;
  CallKraidFunc(E->kraid_var_A | 0xA70000);
}

void Kraid_GetsBig_BreakCeilingPlatforms(void) {  // 0xA7AC4D
  uint16 kraid_var_F;

  if ((nmi_frame_counter_word & 7) == 0)
    Kraid_SpawnRandomQuakeProjs();
  uint16 v0 = 1;
  Enemy_Kraid *E = Get_Kraid(0);
  if ((E->base.y_pos & 2) != 0)
    v0 = -1;
  E->base.x_pos += v0;
  --E->base.y_pos;
  if ((E->base.y_pos & 3) != 0 || (kraid_var_F = E->kraid_var_F, (int16)(kraid_var_F - 18) >= 0)) {

  } else {
    SpawnEnemyProjectileWithGfx(g_word_A7ACB3[kraid_var_F >> 1], cur_enemy_index, addr_kEproj_RocksFallingKraidCeiling);
    switch (E->kraid_var_F >> 1) {
    case 0:
      SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x06, 0x12, 0xb7b3 });
      break;
    case 1:
      SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x0d, 0x12, 0xb7ab });
      break;
    case 2:
      SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x02, 0x12, 0xb7a3 });
      break;
    case 3:
      SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x0a, 0x12, 0xb7b3 });
      break;
    case 4:
      SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x05, 0x12, 0xb7ab });
      break;
    case 5:
      SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x0c, 0x12, 0xb7b3 });
      break;
    case 6:
      SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x03, 0x12, 0xb7ab });
      break;
    case 7:
      SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x0b, 0x12, 0xb7ab });
      break;
    case 8:
      SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x04, 0x12, 0xb7b3 });
      break;
    }
    E->kraid_var_F += 2;
  }
  if (sign16(E->base.y_pos - 296))
    E->kraid_var_A = FUNC16(Kraid_GetsBig_SetBG2TilemapPrioBits);
}

void Kraid_GetsBig_SetBG2TilemapPrioBits(void) {  // 0xA7AD3A
  uint16 v0 = 0;
  do {
    tilemap_stuff[v0 >> 1] |= 0x2000;
    v0 += 2;
  } while ((int16)(v0 - 4096) < 0);
  Enemy_Kraid *E = Get_Kraid(0x40);
  E->base.properties &= ~0x400;
  Get_Kraid(0)->kraid_var_A = FUNC16(Kraid_GetsBig_FinishUpdateBg2Tilemap);
  Kraid_UpdateBg2TilemapTopHalf();
}

void Kraid_GetsBig_FinishUpdateBg2Tilemap(void) {  // 0xA7AD61
  Get_Kraid(0)->kraid_var_A = FUNC16(Kraid_GetsBig_DrawRoomBg);
  Enemy_Kraid *E5 = Get_Kraid(0x140);
  E5->base.instruction_timer = 1;
  E5->base.current_instruction = addr_kKraid_Ilist_86ED;
  Enemy_Kraid *E2 = Get_Kraid(0x80);
  E2->base.current_instruction = addr_kKraid_Ilist_8B04;
  Enemy_Kraid *E3 = Get_Kraid(0xC0);
  E3->base.current_instruction = addr_kKraid_Ilist_8B04;
  Enemy_Kraid *E = Get_Kraid(0x100);
  E->base.current_instruction = addr_kKraid_Ilist_8B04;
  E2->base.spritemap_pointer = addr_kKraid_Sprmap_8C6C;
  E3->base.spritemap_pointer = addr_kKraid_Sprmap_8C6C;
  E->base.spritemap_pointer = addr_kKraid_Sprmap_8C6C;
  Kraid_UpdateBG2TilemapBottomHalf();
}

void Kraid_DrawRoomBg(void) {  // 0xA7AD9A
  VramWriteEntry *v3;

  uint16 v0 = 192;
  uint16 v1 = 0;
  do {
    target_palettes[v0 >> 1] = kKraid_Palette2[v1 >> 1];
    v0 += 2;
    v1 += 2;
  } while ((int16)(v1 - 32) < 0);
  palette_change_num = 0;
  uint16 v2 = vram_write_queue_tail;
  v3 = gVramWriteEntry(vram_write_queue_tail);
  v3->size = 512;
  v3->src.addr = addr_kKraidsRoomBg;
  v3->src.bank = -89;
  v3->vram_dst = ((reg_BG12NBA & 0xF) << 8) + 16128;
  vram_write_queue_tail = v2 + 7;
}

void Kraid_GetsBig_DrawRoomBg(void) {  // 0xA7AD8E
  Enemy_Kraid *E = Get_Kraid(0);
  E->kraid_var_A = FUNC16(Kraid_GetsBig_FadeInRoomBg);
  E->kraid_var_E = 0;
  E->kraid_var_F = 0;
  Kraid_DrawRoomBg();
}

void Kraid_SetLintYAndRandomThinkTimer(void) {  // 0xA7ADEF
  int16 v1;

  Enemy_Kraid *E = Get_Kraid(0);
  Get_Kraid(0x80)->base.y_pos = E->base.y_pos - 20;
  Get_Kraid(0xC0)->base.y_pos = E->base.y_pos + 46;
  Get_Kraid(0x100)->base.y_pos = E->base.y_pos + 112;
  v1 = random_number & 7;
  if ((random_number & 7) == 0)
    v1 = 2;
  E->kraid_thinking = v1 << 6;
}

void Kraid_GetsBig_Thinking_Setup(void) {  // 0xA7ADE1
  Get_Kraid(0)->kraid_var_A = FUNC16(Kraid_GetsBig_Thinking);
  Kraid_SetLintYAndRandomThinkTimer();
}

void Kraid_Mainloop_Thinking_Setup(void) {  // 0xA7ADE9
  Get_Kraid(0)->kraid_var_A = FUNC16(Kraid_Mainloop_Thinking);
  Kraid_SetLintYAndRandomThinkTimer();
}

void Kraid_Lints_Enable(uint16 k, uint16 a) {  // 0xA7AE90
  Enemy_Kraid *E = Get_Kraid(k);
  E->kraid_var_F = a;
  E->kraid_var_A = FUNC16(Kraid_AlignEnemyToKraid);
  E->kraid_next = FUNC16(KraidLint_ProduceLint);
  E->kraid_var_B = 0;
}

void Kraid_GetsBig_FadeInRoomBg(void) {  // 0xA7AE23
  if (AdvancePaletteFade_BgPalette6() & 1) {
    Kraid_GetsBig_Thinking_Setup();
    Kraid_Lints_Enable(0x80, g_word_A7A916);
    Kraid_Lints_Enable(0xc0, g_word_A7A918);
    Kraid_Lints_Enable(0x100, g_word_A7A91A);
    Enemy_Kraid *E6 = Get_Kraid(0x180);
    E6->kraid_next = FUNC16(KraidsFingernail_Init);
    Enemy_Kraid *E7 = Get_Kraid(0x1C0);
    E7->kraid_next = FUNC16(KraidsFingernail_Init);
    E6->kraid_var_A = FUNC16(KraidEnemy_HandleFunctionTimer);
    E7->kraid_var_A = FUNC16(KraidEnemy_HandleFunctionTimer);
    E6->kraid_var_F = 64;
    E7->kraid_var_F = 128;
    Get_Kraid(0x40)->kraid_var_C = 1;
    Enemy_Kraid *E0 = Get_Kraid(0);
    E0->kraid_var_B = addr_stru_A796DA;
    E0->kraid_target_x = 288;
    Enemy_Kraid *E5 = Get_Kraid(0x140);
    E5->kraid_var_A = FUNC16(KraidsFoot_SecondPhaseSetup_WalkToStartPt);
    E5->base.instruction_timer = 1;
    E5->base.current_instruction = addr_kKraid_Ilist_8887;
  }
}

void Kraid_Mainloop_Thinking(void) {  // 0xA7AEA4
  Enemy_Kraid *E = Get_Kraid(0);
  uint16 kraid_thinking = E->kraid_thinking;
  if (kraid_thinking) {
    uint16 v2 = kraid_thinking - 1;
    E->kraid_thinking = v2;
    if (!v2) {
      E->kraid_var_A = FUNC16(Kraid_Main_AttackWithMouthOpen);
      E->kraid_var_B = addr_stru_A796DA;
      E->kraid_var_C = g_stru_A796D2.timer;
    }
  }
}

void Kraid_GetsBig_Thinking(void) {  // 0xA7AEC4
  Enemy_Kraid *E = Get_Kraid(0);
  uint16 kraid_thinking = E->kraid_thinking;
  if (kraid_thinking) {
    uint16 v2 = kraid_thinking - 1;
    E->kraid_thinking = v2;
    if (!v2) {
      E->kraid_var_A = FUNC16(Kraid_Shot_MouthIsOpen);
      E->kraid_var_B = addr_stru_A796DA;
      E->kraid_var_C = g_stru_A796D2.timer;
    }
  }
}

void Kraid_Shot_MouthIsOpen(void) {  // 0xA7AEE4
  uint16 v2;
  Enemy_Kraid *E = Get_Kraid(0);

  if (Kraid_ProcessKraidInstr() == 0xFFFF) {
    E->kraid_var_A = FUNC16(Kraid_Mainloop_Thinking);
    E->kraid_var_C = 90;
    uint16 kraid_mouth_flags = E->kraid_mouth_flags;
    if ((kraid_mouth_flags & 4) != 0
        && (v2 = kraid_mouth_flags - 256, E->kraid_mouth_flags = v2, (v2 & 0xFF00) != 0)) {
      E->kraid_var_A = FUNC16(KraidEnemy_HandleFunctionTimer);
      E->kraid_var_F = 64;
      E->kraid_next = FUNC16(Kraid_InitEyeGlowing);
      E->field_2 = 2;
    } else {
      E->kraid_mouth_flags = 0;
    }
  }
}

uint16 Kraid_ProcessKraidInstr(void) {  // 0xA7AF32
  Enemy_Kraid *E = Get_Kraid(0);
  uint16 result = E->kraid_var_C;
  if (result) {
    if (E->kraid_var_C-- == 1)
      return Kraid_ExecuteInstr();
  }
  return result;
}

void KraidInstr_PlayRoarSfx(void) {  // 0xA7AF94
  QueueSfx2_Max6(0x2D);
}

void KraidInstr_PlayDyingSfx(void) {  // 0xA7AF9F
  QueueSfx2_Max15(0x2E);
}

void CallKraidInstr(uint32 ea) {
  switch (ea) {
  case fnKraidInstr_PlayRoarSfx: KraidInstr_PlayRoarSfx(); return;
  case fnKraidInstr_PlayDyingSfx: KraidInstr_PlayDyingSfx(); return;
  default: Unreachable();
  }
}

uint16 Kraid_ExecuteInstr(void) {  // 0xA7AF3D
  Enemy_Kraid *E = Get_Kraid(0);
RESTART:;
  uint16 kraid_var_B = E->kraid_var_B;
  const uint16 *v2 = (const uint16 *)RomPtr_A7(kraid_var_B);
  uint16 result = *v2;
  if (*v2 != 0xFFFF) {
    if ((int16)(*v2 + 1) < 0) {
      CallKraidInstr(result | 0xA70000);
      E->kraid_var_B += 2;
      goto RESTART;
    } else {
      E->kraid_var_C = result;
      E->kraid_var_B = kraid_var_B + 8;
      uint16 v4 = v2[1];
      uint16 v5 = vram_write_queue_tail;
      gVramWriteEntry(vram_write_queue_tail)->size = 704;
      v5 += 2;
      gVramWriteEntry(v5)->size = v4;
      v5 += 2;
      LOBYTE(gVramWriteEntry(v5++)->size) = -89;
      gVramWriteEntry(v5)->size = (reg_BG2SC & 0xFC) << 8;
      vram_write_queue_tail = v5 + 2;
      return 1;
    }
  }
  return result;
}

void Kraid_Shot_Mouth(void) {  // 0xA7AFAA
  int16 v3;

  Enemy_Kraid *E = Get_Kraid(0);
  if (!sign16(E->kraid_var_A + 0x3AC9))
    return;
  const uint8 *v2 = RomPtr_A7(E->kraid_var_B - 8);
  if (GET_WORD(v2 + 6) == 0xFFFF) {
    v3 = 0;
    goto LABEL_14;
  }
  uint16 v4;
  v4 = GET_WORD(v2 + 6);
  E->kraid_var_E = 1;
  v3 = 0;
  const uint8 *v5;
  v5 = RomPtr_A7(v4);
  uint16 r22 = E->base.x_pos + GET_WORD(v5);
  uint16 r20 = E->base.y_pos + GET_WORD(v5 + 2);
  uint16 r18 = E->base.y_pos + GET_WORD(v5 + 6);
  if (projectile_counter) {
    uint16 v6 = 2 * projectile_counter;
    while (1) {
      int v7;
      v7 = v6 >> 1;
      if (!sign16(projectile_y_pos[v7] - projectile_y_radius[v7] - 1 - r18)
          || sign16(projectile_y_radius[v7] + projectile_y_pos[v7] - r20)
          || sign16(projectile_x_radius[v7] + projectile_x_pos[v7] - r22)) {
        goto LABEL_13;
      }
      uint16 v8;
      v8 = projectile_type[v7];
      if ((v8 & 0xF00) != 0)
        goto LABEL_12;
      if ((v8 & 0x10) != 0)
        break;
LABEL_13:
      v6 -= 2;
      if ((v6 & 0x8000) != 0)
        goto LABEL_14;
    }
    E->kraid_mouth_flags |= 1;
LABEL_12:
    collision_detection_index = v6 >> 1;
    NormalEnemyShotAiSkipDeathAnim_CurEnemy();
    projectile_dir[v6 >> 1] |= 0x10;
    v3 = 1;
    // The real game doesn't preserve R18, R20 so they're junk at this point.
    // Force getting out of the loop.
    v6 = 0; 
    goto LABEL_13;
  }
LABEL_14:
  if (v3) {
    E->kraid_hurt_frame = 6;
    E->kraid_hurt_frame_timer = 2;
    uint16 kraid_mouth_flags = E->kraid_mouth_flags;
    if ((kraid_mouth_flags & 2) != 0)
      E->kraid_mouth_flags = kraid_mouth_flags | 4;
    if (sign16(E->base.health - 1) && sign16(E->kraid_var_A + 0x3CA0)) {
      E->kraid_var_A = FUNC16(Kraid_Death_Init);
      E->kraid_mouth_flags = 0;
      E->base.properties |= kEnemyProps_Tangible;
      Kraid_SetupGfxWithTilePrioClear(~0x2000);
      uint16 v12 = 0;
      do {
        Enemy_Kraid *EL = Get_Kraid(v12);
        EL->base.properties |= kEnemyProps_Tangible;
        v12 += 64;
      } while ((int16)(v12 - 384) < 0);
      if (sign16(E->kraid_var_B + 0x68F2))
        E->kraid_var_B += 60;
    }
  }
}

void Kraid_SpawnExplosionEproj(uint16 k) {  // 0xA7B0CB
  int v1 = k >> 1;
  eproj_spawn_pt = (Point16U){ projectile_x_pos[v1], projectile_y_pos[v1] };
  uint16 v2 = ((projectile_type[v1] & 0x200) == 0) ? 6 : 29;
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, v2);
  QueueSfx1_Max6(0x3D);
}

void Kraid_Enemy_Touch(void) {  // 0xA7B0F3
  int16 v1;

  Enemy_Kraid *E = Get_Kraid(0);
  if (sign16(E->kraid_var_A + 0x3CA0)) {
    uint16 r18 = samus_x_radius + samus_x_pos;
    v1 = samus_y_pos - E->base.y_pos;
    int i;
    for (i = 0; ; i += 4) {
      int v3 = i >> 1;
      if ((int16)(v1 - g_word_A7B161[v3]) >= 0 || (int16)(v1 - g_word_A7B161[v3 + 2]) >= 0)
        break;
    }
    if ((int16)(E->base.x_pos + g_word_A7B161[(i >> 1) + 1] - r18) < 0) {
      if (!sign16(samus_x_pos - 40)) {
        samus_x_pos -= 8;
        samus_prev_x_pos = samus_x_pos;
      }
      uint16 kraid_min_y_pos_eject = samus_y_pos - 8;
      if ((int16)(samus_y_pos - 8 - E->kraid_min_y_pos_eject) < 0)
        kraid_min_y_pos_eject = E->kraid_min_y_pos_eject;
      samus_y_pos = kraid_min_y_pos_eject;
      samus_prev_y_pos = kraid_min_y_pos_eject;
      Kraid_Func_1();
      if (!samus_invincibility_timer)
        NormalEnemyTouchAi();
    }
  }
}

void Kraid_Shot_Body(void) {  // 0xA7B181
  int16 v11;
  uint16 j;

  Enemy_Kraid *E = Get_Kraid(0);
  if (sign16(E->kraid_var_A + 0x3AC9)) {
    E->kraid_var_E = 0;
    E->kraid_mouth_flags &= ~1;
    uint16 R48 = 0;
    const uint8 *v2 = RomPtr_A7(E->kraid_var_B - 8);
    const uint8 *v3 = RomPtr_A7(GET_WORD(v2 + 4));
    uint16 r22 = E->base.x_pos + GET_WORD(v3);
    uint16 r20 = E->base.y_pos + GET_WORD(v3 + 2);
    uint16 r18 = E->base.y_pos + GET_WORD(v3 + 6);
    if (projectile_counter) {
      for (int i = 2 * projectile_counter; i >= 0; i -= 2) {
        int v5 = i >> 1;
        if (sign16(projectile_y_pos[v5] - projectile_y_radius[v5] - 1 - r18)) {
          if (!sign16(projectile_y_radius[v5] + projectile_y_pos[v5] - r20)
              && !sign16(projectile_x_radius[v5] + projectile_x_pos[v5] - r22)) {
            goto LABEL_7;
          }
        } else {
          int v10;
          v10 = i >> 1;
          r18 = projectile_x_radius[v10] + projectile_x_pos[v10];
          v11 = projectile_y_pos[v10] - E->base.y_pos;
          for (j = 0; ; j += 4) {
            int v13 = j >> 1;
            if ((int16)(v11 - g_word_A7B161[v13]) >= 0 || (int16)(v11 - g_word_A7B161[v13 + 2]) >= 0)
              break;
          }
          if ((int16)(E->base.x_pos + g_word_A7B161[(j >> 1) + 1] - r18) < 0) {
LABEL_7:
            Kraid_SpawnExplosionEproj(i);
            int v6 = i >> 1;
            projectile_dir[v6] |= 0x10;
            if ((projectile_type[v6] & 0x10) != 0) {
              E->kraid_mouth_flags |= 1;
            }
            ++R48;
          }
        }
      }
    }
    if (R48) {
      if (E->kraid_var_A == FUNC16(Kraid_Mainloop_Thinking)) {
        E->kraid_var_A = FUNC16(Kraid_InitEyeGlowing);
        uint16 kraid_mouth_flags = E->kraid_mouth_flags;
        if ((kraid_mouth_flags & 1) != 0)
          E->kraid_mouth_flags = kraid_mouth_flags | 0x302;
      }
    }
  }
}

void Kraid_Palette_Handling(void) {  // 0xA7B337
  Enemy_Kraid *E = Get_Kraid(0);
  if (sign16(E->base.health - 1)) {
    E->kraid_hurt_frame = E->base.health;
LABEL_6:
    Kraid_HurtFlash_Handling();
    Kraid_HealthBasedPaletteHandling();
    return;
  }
  if (E->kraid_hurt_frame) {
    uint16 v1 = E->kraid_hurt_frame_timer - 1;
    E->kraid_hurt_frame_timer = v1;
    if (!v1) {
      E->kraid_hurt_frame_timer = 2;
      --E->kraid_hurt_frame;
      goto LABEL_6;
    }
  }
}

void Kraid_HurtFlash_Handling(void) {  // 0xA7B371
  uint16 v0 = 0;
  if ((Get_Kraid(0)->kraid_hurt_frame & 1) == 0)
    v0 = 32;
  uint16 v1 = 0;
  do {
    palette_buffer[(v1 >> 1) + 240] = kKraid_SprPalette7_KraidDeath[v0 >> 1];
    v1 += 2;
    v0 += 2;
  } while ((int16)(v1 - 32) < 0);
}

void Kraid_HealthBasedPaletteHandling(void) {  // 0xA7B394
  uint16 v0 = 0;
  Enemy_Kraid *E = Get_Kraid(0);
  if ((E->kraid_hurt_frame & 1) == 0) {
    uint16 v2 = 14;
    uint16 health = E->base.health;
    do {
      if ((int16)(health - Get_Kraid(v2)->kraid_healths_8ths[0]) >= 0)
        break;
      v2 -= 2;
    } while (v2);
    v0 = 16 * (v2 + 2);
  }
  uint16 v4 = 0;
  do {
    int v5 = v0 >> 1;
    int v6 = v4 >> 1;
    palette_buffer[v6 + 112] = kKraid_BgPalette7[v5];
    palette_buffer[v6 + 240] = kKraid_SprPalette7_KraidDeath[v5];
    v0 += 2;
    v4 += 2;
  } while ((int16)(v4 - 32) < 0);
}

const uint16 *Kraid_Instr_1(uint16 k, const uint16 *jp) {  // 0xA7B633
  return jp;
}

const uint16 *Kraid_Instr_DecYpos(uint16 k, const uint16 *jp) {  // 0xA7B636
  Enemy_Kraid *E = Get_Kraid(0);
  --E->base.y_pos;
  return jp;
}

const uint16 *Kraid_Instr_IncrYpos_Shake(uint16 k, const uint16 *jp) {  // 0xA7B63C
  Enemy_Kraid *E = Get_Kraid(0);
  ++E->base.y_pos;
  earthquake_type = 1;
  earthquake_timer = 10;
  return jp;
}

const uint16 *Kraid_Instr_PlaySound_0x76(uint16 k, const uint16 *jp) {  // 0xA7B64E
  QueueSfx2_Max6(0x76);
  return jp;
}

const uint16 *Kraid_Instr_XposMinus3(uint16 k, const uint16 *jp) {  // 0xA7B65A
  Enemy_Kraid *E = Get_Kraid(0);
  E->base.x_pos -= g_word_A7A91C;
  return jp;
}

const uint16 *Kraid_Instr_XposMinus3b(uint16 k, const uint16 *jp) {  // 0xA7B667
  Enemy_Kraid *E = Get_Kraid(0);
  E->base.x_pos -= g_word_A7A91C;
  return jp;
}

const uint16 *Kraid_Instr_XposPlus3(uint16 k, const uint16 *jp) {  // 0xA7B674
  Enemy_Kraid *E = Get_Kraid(0);
  E->base.x_pos += g_word_A7A920;
  return jp;
}

const uint16 *Kraid_Instr_MoveHimRight(uint16 k, const uint16 *jp) {  // 0xA7B683
  uint16 v3;

  Enemy_Kraid *E = Get_Kraid(0);
  if (sign16(E->base.x_pos - 320) || (v3 = E->kraid_target_x - 1, (E->kraid_target_x = v3) == 0)) {
    if (Enemy_MoveRight_IgnoreSlopes(0, INT16_SHL16(g_word_A7A922))) {
      earthquake_type = 0;
      earthquake_timer = 7;
      uint16 x_pos = Get_Kraid(0)->base.x_pos;
      Get_Kraid(0x140)->base.x_pos = x_pos;
    }
  }
  return jp;
}

void Kraid_InitEyeGlowing(void) {  // 0xA7B6BF
  Enemy_Kraid *E = Get_Kraid(0);
  E->kraid_var_A = FUNC16(Kraid_Shot_MouthIsOpen);
  E->kraid_var_A = FUNC16(Kraid_Shot_GlowHisEye);
  E->kraid_var_B = addr_stru_A7974A + 8;
  E->kraid_var_C = g_stru_A7974A[0].timer;
  Kraid_Shot_GlowHisEye();
}

void Kraid_Shot_GlowHisEye(void) {  // 0xA7B6D7
  int16 v1;

  Kraid_ProcessKraidInstr();
  uint16 v0 = 226;
  v1 = 0;
  do {
    int v2 = v0 >> 1;
    uint16 v3 = (palette_buffer[v2] & 0x1F) + 1;
    if (!sign16((palette_buffer[v2] & 0x1F) - 30)) {
      ++v1;
      v3 = 31;
    }
    uint16 v4 = (palette_buffer[v2] & 0x3E0) + 32;
    if (!sign16((palette_buffer[v2] & 0x3E0) - 960)) {
      ++v1;
      v4 = 992;
    }
    palette_buffer[v2] = v4 | v3 | palette_buffer[v2] & 0xFC00;
    v0 += 2;
  } while ((int16)(v0 - 232) < 0);
  if ((int16)(v1 - 6) >= 0)
    Get_Kraid(0)->kraid_var_A = FUNC16(Kraid_Shot_UnglowEye);
}

void Kraid_Shot_UnglowEye(void) {  // 0xA7B73D
  Enemy_Kraid *E = Get_Kraid(0);
  uint16 v0 = 14;
  uint16 health = E->base.health;
  do {
    if ((int16)(health - Get_Kraid(v0)->kraid_healths_8ths[0]) >= 0)
      break;
    v0 -= 2;
  } while (v0);
  uint16 v2 = 16 * (v0 + 2);
  uint16 v3 = 226;
  uint16 r20 = 0, r18;
  do {
    int v4 = v3 >> 1;
    r18 = palette_buffer[v4] & 0x1F;
    int v5 = v2 >> 1;
    if ((kKraid_BgPalette7[v5 + 1] & 0x1F) != r18) {
      ++r20;
      --palette_buffer[v4];
    }
    r18 = palette_buffer[v4] & 0x3E0;
    if ((kKraid_BgPalette7[v5 + 1] & 0x3E0) != r18) {
      ++r20;
      palette_buffer[v4] -= 32;
    }
    v3 += 2;
    v2 += 2;
  } while ((int16)(v3 - 232) < 0);
  if (!r20) {
    E->kraid_var_A = FUNC16(Kraid_Shot_MouthIsOpen);
    E->kraid_var_B = addr_stru_A796DA;
    E->kraid_var_C = g_stru_A796D2.timer;
  }
}

void KraidsArm_Main(void) {  // 0xA7B7BD
  uint16 r18 = layer1_y_pos + 224;
  Enemy_Kraid *E = Get_Kraid(0);
  uint16 v1 = E->base.y_pos - 44;
  Enemy_Kraid *E1 = Get_Kraid(0x40);
  E1->base.y_pos = v1;
  uint16 v3 = v1;
  uint16 v4 = E1->base.properties | 0x100;
  if ((int16)(v3 - layer1_y_pos) >= 0 && (int16)(v3 - r18) < 0)
    v4 = E1->base.properties & 0xFEFF;
  E1->base.properties = v4;
  E1->base.x_pos = E->base.x_pos;
  if (HIBYTE(E->kraid_mouth_flags))
    ++E1->base.instruction_timer;
}

void KraidLintCommon_Main(uint16 k) {  // 0xA7B822
  Kraid_EnemyTouch_Lint(k);
  // r18 = layer1_y_pos + 224;
  EnemyData *v1 = gEnemyData(k);
  CallKraidFunc(v1->ai_var_A | 0xA70000);
}

void KraidsTopLint_Main(void) {  // 0xA7B801
  gEnemyData(0x80)->instruction_timer = 0x7FFF;
  KraidLintCommon_Main(0x80);
}

void KraidsMiddleLint_Main(void) {  // 0xA7B80D
  gEnemyData(0xC0)->instruction_timer = 0x7FFF;
  KraidLintCommon_Main(0xC0);
}

void KraidsBottomLint_Main(void) {  // 0xA7B819
  gEnemyData(0x100)->instruction_timer = 0x7FFF;
  KraidLintCommon_Main(0x100);
}

void KraidLint_ProduceLint(uint16 k) {  // 0xA7B832
  EnemyData *v1 = gEnemyData(k);
  v1->properties &= 0xFAFF;
  v1->x_pos = v1->ai_var_C + gEnemyData(0)->x_pos - v1->ai_var_B;
  uint16 v2 = v1->ai_var_B + 1;
  v1->ai_var_B = v2;
  if (!sign16(v2 - 32)) {
    v1->ai_var_A = FUNC16(KraidLint_ChargeLint);
    v1->ai_preinstr = 30;
  }
}

void KraidLint_ChargeLint(uint16 k) {  // 0xA7B868
  EnemyData *E = gEnemyData(k);
  int16 v1 = 0;
  if ((E->ai_preinstr & 1) != 0)
    v1 = 3584;
  E->palette_index = v1;
  E->x_pos = E->ai_var_C + gEnemyData(0)->x_pos - E->ai_var_B;
  if (E->ai_preinstr-- == 1) {
    E->ai_var_A = FUNC16(KraidLint_FireLint);
    QueueSfx3_Max6(0x1F);
  }
}

void KraidLint_FireLint(uint16 k) {  // 0xA7B89B
  Enemy_Kraid *E = Get_Kraid(k);
  AddToHiLo(&E->base.x_pos, &E->base.x_subpos, -IPAIR32(g_word_A7A928, g_word_A7A926));
  if (sign16(E->base.x_pos - 56))
    E->base.properties |= kEnemyProps_Tangible;
  if (sign16(E->base.x_pos - 32)) {
    E->base.properties |= kEnemyProps_Invisible;
    E->kraid_var_A = FUNC16(Kraid_AlignEnemyToKraid);
    E->kraid_var_F = 300;
    E->kraid_next = FUNC16(KraidLint_ProduceLint);
    E->kraid_var_B = 0;
  }
  if (CheckIfEnemyTouchesSamus(k)) {
    AddToHiLo(&extra_samus_x_displacement, &extra_samus_x_subdisplacement, -IPAIR32(g_word_A7A928, g_word_A7A926));
    if (sign16(extra_samus_x_displacement + 16))
      extra_samus_x_displacement = -16;
  }
}

void KraidFingernail_WaitForLintXpos(uint16 k) {  // 0xA7B907
  Enemy_Kraid *E2 = Get_Kraid(0x80);
  if (!sign16(E2->base.x_pos - 256)) {
    Enemy_Kraid *E = Get_Kraid(k);
    E->kraid_var_A = E->kraid_next;
    E->base.properties &= 0xFAFF;
  }
}

void KraidEnemy_HandleFunctionTimer(uint16 k) {  // 0xA7B92D
  Enemy_Kraid *E = Get_Kraid(k);
  if (E->kraid_var_F && E->kraid_var_F-- == 1)
    E->kraid_var_A = E->kraid_next;
}

void Kraid_AlignEnemyToKraid(uint16 k) {  // 0xA7B923
  uint16 x_pos = Get_Kraid(0)->base.x_pos;
  Enemy_Kraid *E = Get_Kraid(k);
  E->base.x_pos = x_pos - E->base.x_width;
  KraidEnemy_HandleFunctionTimer(k);
}

void KraidEnemy_DecrementEnemyFunctionTimer(void) {  // 0xA7B93F
  Enemy_Kraid *E = Get_Kraid(cur_enemy_index);
  if (E->kraid_var_F) {
    if (E->kraid_var_F-- == 1) {
      E->kraid_var_A = E->kraid_next;
      E->base.current_instruction = addr_kKraid_Ilist_8887;
      E->base.instruction_timer = 1;
    }
  }
}

void KraidFoot_FirstPhase_Thinking(uint16 k) {  // 0xA7B960
  Kraid_HandleFirstPhase();
  KraidEnemy_HandleFunctionTimer(k);
}

void KraidEnemy_ProcessInstrEnemyTimer(uint16 k) {  // 0xA7B965
  Kraid_ProcessKraidInstr();
  KraidEnemy_HandleFunctionTimer(k);
}

void Kraid_EnemyTouch_Lint(uint16 k) {  // 0xA7B96A
  Enemy_Kraid *E = Get_Kraid(k);
  if ((E->base.properties & kEnemyProps_Tangible) == 0 && !samus_invincibility_timer) {
    uint16 r18 = g_stru_A792B7.left + E->base.x_pos - 2;
    if (!sign16(samus_x_radius + samus_x_pos - r18)) {
      if (sign16(samus_x_pos - samus_x_radius - r18)) {
        uint16 r22 = g_stru_A792B7.top + E->base.y_pos + 2;
        if (!sign16(samus_y_radius + samus_y_pos - r22)) {
          uint16 r24 = g_stru_A792B7.bottom + E->base.y_pos - 2;
          if (sign16(samus_y_pos - samus_y_radius - r24)) {
            uint16 v2 = extra_samus_x_displacement + ~(samus_x_radius + 16);
            if (!sign16(v2 - 16))
              v2 = 16;
            extra_samus_x_displacement = v2;
            NormalEnemyTouchAi();
            E->base.properties |= kEnemyProps_Tangible;
          }
        }
      }
    }
  }
}

void KraidsFoot_Main(void) {  // 0xA7B9F6
  int16 v3;

  EnemyData *v0 = gEnemyData(0);
  v0[5].x_pos = v0->x_pos;
  uint16 v1 = v0->y_pos + 100;
  v0[5].y_pos = v1;
  uint16 v2 = v1;
  v3 = v1 - 224;
  uint16 v4 = v0[5].properties & 0xFEFF;
  if ((int16)(v2 - layer1_y_pos) >= 0) {
    if ((int16)(v3 - layer1_y_pos) >= 0)
      v4 |= 0x100;
  } else {
    v4 |= 0x100;
  }
  v0[5].properties = v4;
  CallKraidFunc(v0[5].ai_var_A | 0xA70000);
}

void Kraid_SetWalkingBackwards(uint16 j, uint16 a) {  // 0xA7BB0D
  Get_Kraid(0)->kraid_target_x = a;
  Enemy_Kraid *E = Get_Kraid(0x140);
  E->kraid_next = j;
  E->kraid_var_A = FUNC16(KraidsFoot_SecondPhase_WalkingBackwards);
  E->base.instruction_timer = 1;
  E->base.current_instruction = addr_kKraid_Ilist_8887;
}

void Kraid_SetWalkingForwards(uint16 j, uint16 a) {  // 0xA7BB29
  Get_Kraid(0)->kraid_target_x = a;
  Enemy_Kraid *E = Get_Kraid(0x140);
  E->kraid_next = j;
  E->kraid_var_A = FUNC16(KraidsFoot_SecondPhase_WalkForward);
  E->base.instruction_timer = 1;
  E->base.current_instruction = addr_kKraid_Ilist_86F3;
}

void KraidsFoot_SecondPhase_Thinking(void) {  // 0xA7BA2E
  int16 v4;

  Enemy_Kraid *E5 = Get_Kraid(0x140);
  uint16 v1 = E5->kraid_next - 1;
  E5->kraid_next = v1;
  if (!v1) {
    uint16 v2 = 0;
    Enemy_Kraid *E0;
    while (1) {
      E0 = Get_Kraid(0);
      if (E0->base.x_pos == g_word_A7BA7D[v2 >> 1])
        break;
      v2 += 4;
      if ((int16)(v2 - 24) >= 0) {
        v2 = 4;
        break;
      }
    }
    v4 = random_number & 0x1C;
    if (!sign16(v4 - 16))
      v4 = 16;
    const uint16 *v5 = (const uint16 *)RomPtr_A7(g_word_A7BA7D[(v2 >> 1) + 1] + v4);
    uint16 v6 = v5[1];
    if ((int16)(*v5 - E0->base.x_pos) >= 0)
      Kraid_SetWalkingBackwards(v6, *v5);
    else
      Kraid_SetWalkingForwards(v6, *v5);
  }
}

void KraidsFoot_SecondPhase_WalkingBackwards(void) {  // 0xA7BB45
  Enemy_Kraid *E0 = Get_Kraid(0);
  uint16 kraid_target_x = E0->kraid_target_x;
  if (kraid_target_x != E0->base.x_pos) {
    if ((int16)(kraid_target_x - E0->base.x_pos) >= 0)
      return;
    E0->base.x_pos = kraid_target_x;
  }
  Enemy_Kraid *E5 = Get_Kraid(0x140);
  if (!sign16(E5->base.current_instruction + 0x76C7)) {
    E5->kraid_var_A = FUNC16(KraidsFoot_SecondPhase_Thinking);
    E5->base.instruction_timer = 1;
    E5->base.current_instruction = addr_kKraid_Ilist_86ED;
  }
}

void KraidsFoot_SecondPhaseSetup_WalkToStartPt(void) {  // 0xA7BB6E
  Enemy_Kraid *E0 = Get_Kraid(0);
  uint16 kraid_target_x = E0->kraid_target_x;
  if (kraid_target_x != E0->base.x_pos) {
    if ((int16)(kraid_target_x - E0->base.x_pos) >= 0)
      return;
    E0->base.x_pos = kraid_target_x;
  }
  Enemy_Kraid *E5 = Get_Kraid(0x140);
  if (!sign16(E5->base.current_instruction + 0x76C7)) {
    E5->kraid_var_A = FUNC16(KraidEnemy_HandleFunctionTimer);
    E5->kraid_var_F = 180;
    E5->kraid_next = FUNC16(KraidsFoot_SecondPhase_Init);
    E5->base.instruction_timer = 1;
    E5->base.current_instruction = addr_kKraid_Ilist_86ED;
  }
}

void KraidsFoot_SecondPhase_Init(void) {  // 0xA7BBA4
  Kraid_SetWalkingBackwards(0xB4, 0x160);
}

void KraidsFoot_SecondPhase_WalkForward(void) {  // 0xA7BBAE
  Enemy_Kraid *E0 = Get_Kraid(0);
  uint16 kraid_target_x = E0->kraid_target_x;
  if ((int16)(kraid_target_x - E0->base.x_pos) < 0) {
    Enemy_Kraid *E5 = Get_Kraid(0x140);
    if (E5->base.current_instruction == (uint16)addr_off_A787BB) {
      E5->base.current_instruction = addr_kKraid_Ilist_86F3;
      E5->base.instruction_timer = 1;
    }
  } else {
    E0->base.x_pos = kraid_target_x;
    Enemy_Kraid *E5 = Get_Kraid(0x140);
    if (E5->base.current_instruction == (uint16)addr_off_A787BB) {
      E5->kraid_var_A = FUNC16(KraidsFoot_SecondPhase_Thinking);
      E5->base.instruction_timer = 1;
      E5->base.current_instruction = addr_kKraid_Ilist_86ED;
    }
  }
}

void Kraid_Main_AttackWithMouthOpen(void) {  // 0xA7BBEA
  uint16 v3;
  Enemy_Kraid *E = Get_Kraid(0);

  if (Kraid_ProcessKraidInstr() == 0xFFFF) {
    Kraid_Mainloop_Thinking_Setup();
    E->kraid_var_C = 90;
    uint16 kraid_mouth_flags = E->kraid_mouth_flags;
    if ((kraid_mouth_flags & 4) != 0 && (v3 = kraid_mouth_flags - 256, E->kraid_mouth_flags = v3, (v3 & 0xFF00) != 0)) {
      E->kraid_var_A = FUNC16(KraidEnemy_HandleFunctionTimer);
      E->kraid_var_F = 64;
      E->kraid_next = FUNC16(Kraid_InitEyeGlowing);
      E->field_2 = 2;
    } else {
      E->kraid_mouth_flags = 0;
    }
  } else {
    if (*((uint16 *)RomPtr_A7(E->kraid_var_B - 8) + 1) == addr_kKraidTilemaps_3 && (E->kraid_var_C & 0xF) == 0) {
      SpawnEnemyProjectileWithGfx(g_word_A7BC65[(random_number & 0xE) >> 1], random_number & 0xE, addr_kEproj_RocksKraidSpits);
      QueueSfx3_Max6(0x1E);
    }
  }
}

void KraidsGoodFingernail_Touch(void) {  // 0xA7BCCF
  uint16 v0 = 0;
  printf("X undefined\n");
  NormalEnemyTouchAi();
  EnemyDeathAnimation(cur_enemy_index, v0);
}

void KraidsBadFingernail_Touch(void) {  // 0xA7BCDE
  uint16 v0 = 0;

  printf("X undefined\n");
  NormalEnemyTouchAi();
  EnemyDeathAnimation(cur_enemy_index, v0);
}

void KraidFingernailInit(uint16 k) {  // 0xA7BCF2
  uint16 palette_index = Get_Kraid(0)->base.palette_index;
  Enemy_Kraid *E = Get_Kraid(k);
  E->base.palette_index = palette_index;
  E->kraid_var_B = 40;
  E->base.properties |= kEnemyProps_Invisible;
  E->base.instruction_timer = 0x7FFF;
  E->base.current_instruction = addr_kKraid_Ilist_8B0A;
  E->base.spritemap_pointer = kKraid_Ilist_8B0A.field_2;
  E->kraid_next = FUNC16(KraidsFingernail_Init);
  E->kraid_var_A = FUNC16(KraidEnemy_HandleFunctionTimer);
  E->kraid_var_F = 64;
}

void KraidsGoodFingernail_Init(void) {  // 0xA7BCEF
  KraidFingernailInit(cur_enemy_index);
}

void KraidsBadFingernail_Init(void) {  // 0xA7BD2D
  KraidFingernailInit(cur_enemy_index);
}

void KraidsGoodFingernail_Main(void) {  // 0xA7BD32
  EnemyData *v0 = gEnemyData(0);
  if (sign16(v0->health - 1)) {
    EnemyData *v1 = gEnemyData(0);
    v1[6].properties |= kEnemyProps_Deleted | kEnemyProps_Invisible;
  } else {
    CallKraidFunc(v0[6].ai_var_A | 0xA70000);
  }
}

void KraidsBadFingernail_Main(void) {  // 0xA7BD49
  EnemyData *v0 = gEnemyData(0);
  if (sign16(v0->health - 1)) {
    EnemyData *v1 = gEnemyData(0);
    v1[7].properties |= kEnemyProps_Deleted | kEnemyProps_Invisible;
  } else {
    CallKraidFunc(v0[7].ai_var_A | 0xA70000);
  }
}

void KraidsFingernail_Init(void) {  // 0xA7BD60
  uint16 v2;

  uint16 kraid_var_E = Get_Kraid(0x180)->kraid_var_E;
  if (cur_enemy_index == 384)
    kraid_var_E = Get_Kraid(0x1C0)->kraid_var_E;
  if (sign16(kraid_var_E))
    v2 = g_off_A7BE3E[(random_number & 6) >> 1];
  else
    v2 = g_off_A7BE46[(random_number & 6) >> 1];
  const uint8 *v3 = RomPtr_A7(v2);
  Enemy_Kraid *E = Get_Kraid(cur_enemy_index);
  E->kraid_var_B = GET_WORD(v3);
  E->kraid_var_C = GET_WORD(v3 + 2);
  E->kraid_var_D = GET_WORD(v3 + 4);
  E->kraid_var_E = GET_WORD(v3 + 6);
  E->kraid_parameter_1 = 1;
  E->base.properties &= ~(kEnemyProps_Tangible | kEnemyProps_Invisible);
  E->base.instruction_timer = 1;
  E->base.current_instruction = addr_kKraid_Ilist_8B0A;
  E->kraid_var_A = FUNC16(KraidsFingernail_Fire);
  if ((random_number & 1) == 0)
    goto LABEL_7;
  uint16 v7;
  v7 = Get_Kraid(0x180)->kraid_healths_8ths[1];
  if (cur_enemy_index != 448)
    v7 = Get_Kraid(0x1C0)->kraid_healths_8ths[1];
  if (v7 == 1) {
LABEL_7:
    E->kraid_healths_8ths[1] = 0;
    Enemy_Kraid *E0 = Get_Kraid(0);
    E->base.x_pos = (E0->base.x_pos - E0->base.x_width - E->base.x_width) & 0xFFF0;
    E->base.y_pos = Get_Kraid(0x40)->base.y_pos + 128;
  } else {
    E->kraid_healths_8ths[1] = 1;
    E->base.x_pos = 50;
    E->base.y_pos = 240;
    E->kraid_var_B = 0;
    E->kraid_var_C = 1;
    E->kraid_var_D = 0;
    E->kraid_var_E = 0;
    E->kraid_var_A = FUNC16(KraidFingernail_WaitForLintXpos);
    E->kraid_next = FUNC16(KraidsFingernail_Fire);
    E->base.properties |= kEnemyProps_Tangible | kEnemyProps_Invisible;
  }
}

void KraidsFingernail_Fire(uint16 k) {  // 0xA7BE8E
  EnemyData *v1 = gEnemyData(k);
  if (Enemy_MoveRight_IgnoreSlopes(k, __PAIR32__(v1->ai_var_C, v1->ai_var_B))) {
    v1->ai_var_B = -v1->ai_var_B;
    v1->ai_var_C = -v1->ai_var_C;
  } else {
    int i;
    EnemyData *v3, *v4;
    for (i = 0; ; i += 4) {
      v3 = gEnemyData(0);
      v4 = gEnemyData(k);
      if ((int16)(g_word_A7BF1D[(i >> 1) + 1] + v3->y_pos - v4->y_pos) < 0)
        break;
    }
    uint16 r18 = g_word_A7BF1D[i >> 1] + v3->x_pos;
    if (!sign16(v4->x_width + v4->x_pos - r18) && (v4->ai_var_C & 0x8000) == 0) {
      v4->ai_var_B = -v4->ai_var_B;
      v4->ai_var_C = -v4->ai_var_C;
    }
  }
  EnemyData *v5 = gEnemyData(k);
  if (Enemy_MoveDown(k, __PAIR32__(v5->ai_var_E, v5->ai_var_D)))
    SetHiLo(&v5->ai_var_E, &v5->ai_var_D, -IPAIR32(v5->ai_var_E, v5->ai_var_D));
}

void KraidsFoot_PrepareToLungeForward(void) {  // 0xA7BF2D
  Kraid_HandleFirstPhase();
  EnemyData *v0 = gEnemyData(0);
  if (!sign16(v0[1].current_instruction + 0x75C9)) {
    v0[1].current_instruction = addr_kKraid_Ilist_8AF0;
    v0[1].instruction_timer = 1;
    v0[5].instruction_timer = 1;
    v0[5].current_instruction = addr_kKraid_Ilist_87BD;
    v0[5].ai_var_A = FUNC16(KraidsFoot_FirstPhase_LungeForward);
    v0[5].ai_preinstr = 0;
  }
}

void KraidsFoot_FirstPhase_LungeForward(void) {  // 0xA7BF5D
  Kraid_HandleFirstPhase();
  Enemy_Kraid *E = Get_Kraid(0);
  if (sign16(E->base.x_pos - 92))
    E->base.x_pos = 92;
  Enemy_Kraid *E5 = Get_Kraid(0x140);
  if (E5->base.current_instruction == (uint16)addr_off_A78885) {
    if (E->base.x_pos == 92) {
      E5->kraid_next = FUNC16(KraidsFoot_FirstPhase_RetreatFromLunge);
      E5->kraid_var_A = FUNC16(KraidEnemy_DecrementEnemyFunctionTimer);
      E5->kraid_var_F = 1;
      E5->base.instruction_timer = 1;
      E5->base.current_instruction = addr_kKraid_Ilist_86ED;
    } else {
      E5->base.instruction_timer = 1;
      E5->base.current_instruction = addr_kKraid_Ilist_87BD;
    }
  }
}

void KraidsFoot_FirstPhase_RetreatFromLunge(void) {  // 0xA7BFAB
  Kraid_HandleFirstPhase();
  Enemy_Kraid *E0 = Get_Kraid(0);
  if (!sign16(E0->base.x_pos - 176))
    E0->base.x_pos = 176;
  Enemy_Kraid *E5 = Get_Kraid(0x140);
  if (!sign16(E5->base.current_instruction + 0x76C7)) {
    if (E0->base.x_pos == 176) {
      Enemy_Kraid *E1 = Get_Kraid(0x40);
      E1->base.current_instruction = addr_kKraid_Ilist_89F3;
      E1->base.instruction_timer = 1;
      E5->base.instruction_timer = 1;
      E5->base.current_instruction = addr_kKraid_Ilist_86ED;
      E5->kraid_var_A = FUNC16(KraidFoot_FirstPhase_Thinking);
      E5->kraid_var_F = 300;
      E5->kraid_next = FUNC16(KraidsFoot_PrepareToLungeForward);
    } else {
      printf("X undefined\n");
      Enemy_Kraid *E = Get_Kraid(cur_enemy_index);
      E->base.current_instruction = addr_kKraid_Ilist_8887;
      E->base.instruction_timer = 1;
    }
  }
}

void Kraid_HandleFirstPhase(void) {  // 0xA7C005
  Enemy_Kraid *E0 = Get_Kraid(0);
  if ((int16)(E0->base.health - E0->kraid_healths_8ths[6]) < 0) {
    E0->kraid_var_A = FUNC16(KraidEnemy_ProcessInstrEnemyTimer);
    E0->kraid_var_F = 180;
    E0->kraid_next = FUNC16(Kraid_GetsBig_ReleaseCamera);
    uint16 v2 = *((uint16 *)RomPtr_A7(E0->kraid_var_B) + 1);
    uint16 v3 = 50;
    if (v2 != addr_kKraidTilemaps_0) {
      v3 = 42;
      if (v2 != addr_kKraidTilemaps_1) {
        v3 = 34;
        if (v2 != addr_kKraidTilemaps_2)
          v3 = 26;
      }
    }
    E0->kraid_var_B = v3 - 26918;
    E0->kraid_var_C = *(uint16 *)((uint8 *)&g_stru_A796D2.timer + v3);
    earthquake_type = 4;
    earthquake_timer = 340;
    Enemy_Kraid *E5 = Get_Kraid(0x140);
    E5->base.current_instruction = addr_kKraid_Ilist_86E7;
    E5->base.instruction_timer = 1;
    E5->kraid_var_A = FUNC16(nullsub_234);
    Enemy_Kraid *E1 = Get_Kraid(0x40);
    E1->base.current_instruction = addr_kKraid_Ilist_89F3;
    E1->base.instruction_timer = 1;
    Enemy_Kraid *E2 = Get_Kraid(0x80);
    E2->base.properties |= 0x100;
    Enemy_Kraid *E3 = Get_Kraid(0xC0);
    E3->base.properties |= 0x100;
    Enemy_Kraid *E4 = Get_Kraid(0x100);
    E4->base.properties |= 0x100;
    E1->base.properties |= 0x400;
  }
}

void Kraid_GetsBig_ReleaseCamera(void) {  // 0xA7C0A1
  Enemy_Kraid *E = Get_Kraid(0);
  E->kraid_var_A = FUNC16(Kraid_GetsBig_BreakCeilingPlatforms);
  *(uint16 *)scrolls = 514;
  *(uint16 *)&scrolls[2] = 257;
  E->kraid_min_y_pos_eject = 164;
}

void Kraid_SpawnPlmToClearCeiling(void) {  // 0xA7C168
  SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x02, 0x12, 0xb7b7 });
}

void Kraid_ClearSomeSpikes(void) {  // 0xA7C171
  SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x05, 0x1b, 0xb7bb });
}

CoroutineRet UnpauseHook_Kraid_IsDead(void) {  // 0xA7C1FB
  static const StartDmaCopy unk_A7C21E = { 1, 1, 0x18, LONGPTR(0xa7a716), 0x0200 };
  static const StartDmaCopy unk_A7C23E = { 1, 1, 0x18, LONGPTR(0x9ab200), 0x0800 };
  ScreenOff();
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 16 * (reg_BG12NBA & 0xF) + 63);
  WriteReg(VMAIN, 0x80);
  SetupDmaTransfer(&unk_A7C21E);
  WriteReg(MDMAEN, 2);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x40);
  WriteReg(VMAIN, 0x80);
  SetupDmaTransfer(&unk_A7C23E);
  WriteReg(MDMAEN, 2);
  Kraid_TransferTopHalfToVram();
  return kCoroutineNone;
}

static const StartDmaCopy unk_A7C26B = { 1, 1, 0x18, LONGPTR(0x7e5000), 0x0400 };
static const StartDmaCopy unk_A7C28D = { 1, 1, 0x18, LONGPTR(0x7e2000), 0x0800 };

CoroutineRet UnpauseHook_Kraid_IsAlive(void) {  // 0xA7C24E
  ScreenOff();
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, reg_BG12NBA + 62);
  WriteReg(VMAIN, 0x80);
  SetupDmaTransfer(&unk_A7C26B);
  WriteReg(MDMAEN, 2);
  Kraid_TransferTopHalfToVram();
  return kCoroutineNone;
}

void Kraid_TransferTopHalfToVram(void) {  // 0xA7C278
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, reg_BG2SC & 0xFC);
  WriteReg(VMAIN, 0x80);
  SetupDmaTransfer(&unk_A7C28D);
  WriteReg(MDMAEN, 2);
  ScreenOn();
}

static const StartDmaCopy unk_A7C2BD = { 1, 1, 0x18, LONGPTR(0x7e5000), 0x0400 };

CoroutineRet Kraid_UnpauseHook_IsSinking(void) {  // 0xA7C2A0
  COROUTINE_BEGIN(coroutine_state_2, 0);
  ScreenOff();
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, reg_BG12NBA + 62);
  WriteReg(VMAIN, 0x80);
  SetupDmaTransfer(&unk_A7C2BD);
  WriteReg(MDMAEN, 2);
  if ((int16)(Get_Kraid(0)->base.y_pos - kKraidSinkEntry[0].field_0) >= 0) {
    my_counter = 0;
    my_counter2 = vram_write_queue_tail;
    while (kKraidSinkEntry[my_counter].field_0 != 0xFFFF
           && (int16)(Get_Kraid(0)->base.y_pos - kKraidSinkEntry[my_counter].field_0) >= 0) {
      VramWriteEntry *v2;
      v2 = gVramWriteEntry(my_counter2);
      v2->size = 64;
      v2->src.addr = 0x2FC0;
      v2->src.bank = 0x7E;
      v2->vram_dst = kKraidSinkEntry[my_counter].field_2 + ((reg_BG2SC & 0xFC) << 8);
      vram_write_queue_tail = my_counter2 + 7;
      COROUTINE_AWAIT(1, WaitForNMI_Async());
      my_counter++;
    }
  }
  COROUTINE_END(0);
}

void PauseHook_Kraid(void) {  // 0xA7C325
  unsigned int v1;

  uint16 v0 = vram_read_queue_tail;
  v1 = vram_read_queue_tail;
  *(uint16 *)((uint8 *)&vram_read_queue[0].vram_target + vram_read_queue_tail) = ((reg_BG12NBA & 0xFC) << 8) + 15872;
  *(uint16 *)((uint8 *)&vram_read_queue[0].dma_parameters + v1) = 129;
  *(uint16 *)((uint8 *)&vram_read_queue[0].dma_parameters + v0 + 1) = 57;
  *(VoidP *)((uint8 *)&vram_read_queue[0].src.addr + v0) = 20480;
  *(uint16 *)(&vram_read_queue[0].src.bank + v0) = 126;
  *(uint16 *)((uint8 *)&vram_read_queue[0].size + v0) = 1024;
  vram_read_queue_tail = v0 + 9;
}

void Kraid_Death_Init(void) {  // 0xA7C360
  Enemy_Kraid *E0 = Get_Kraid(0);
  if (!E0->kraid_hurt_frame) {
    uint16 v0 = 192;
    do {
      target_palettes[v0 >> 1] = 0;
      v0 += 2;
    } while ((int16)(v0 - 224) < 0);
    for (int i = 30; i >= 0; i -= 2)
      palette_buffer[(i >> 1) + 112] = kKraid_BgPalette7_KraidDeath[i >> 1];
    Enemy_Kraid *E1 = Get_Kraid(0x40);
    E1->base.current_instruction = addr_kKraid_Ilist_8AF0;
    E1->base.instruction_timer = 1;
    E0->kraid_var_A = FUNC16(Kraid_Death_Fadeout);
    E0->kraid_var_B = addr_stru_A79764 + 8;
    E0->kraid_var_C = g_stru_A79764[0].timer;
    uint16 v4 = cur_enemy_index;
    uint16 v7 = cur_enemy_index;
    Enemy_Kraid *E6 = Get_Kraid(0x180);
    E6->base.properties &= ~0x4000;
    cur_enemy_index = 384;
    EnemyDeathAnimation(v4, 0x180);
    Enemy_Kraid *E7 = Get_Kraid(0x1C0);
    E7->base.properties &= ~0x4000;
    cur_enemy_index = 448;
    EnemyDeathAnimation(v4, 0x1C0);
    cur_enemy_index = 128;
    EnemyDeathAnimation(v4, 0x80);
    cur_enemy_index = 192;
    EnemyDeathAnimation(v4, 0xC0);
    cur_enemy_index = 256;
    EnemyDeathAnimation(v4, 0x100);
    cur_enemy_index = v7;
    SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x05, 0x1b, 0xb7bf });
  }
}

void Kraid_Death_Fadeout(void) {  // 0xA7C3F9
  VramWriteEntry *v3;

  Kraid_ProcessKraidInstr();
  if (AdvancePaletteFade_BgPalette6() & 1) {
    Enemy_Kraid *E = Get_Kraid(0);
    E->kraid_var_A = FUNC16(Kraid_Death_UpdateBG2TilemapTopHalf);
    E->kraid_hurt_frame_timer = 1;
    E->kraid_hurt_frame = 1;
    Kraid_Palette_Handling();
    uint16 v1 = 0;
    do {
      ram4000.xray_tilemaps[v1] = 0;
      ram4000.xray_tilemaps[v1 + 1] = 0;
      v1 += 2;
    } while ((int16)(v1 * 2 - 512) < 0);
    uint16 v2 = vram_write_queue_tail;
    v3 = gVramWriteEntry(vram_write_queue_tail);
    v3->size = 512;
    v3->src.addr = 0x4000;
    v3->src.bank = 126;
    v3->vram_dst = ((reg_BG12NBA & 0xF) << 8) + 16128;
    vram_write_queue_tail = v2 + 7;
  }
}

void Kraid_Death_UpdateBG2TilemapTopHalf(void) {  // 0xA7C4A4
  Kraid_ProcessKraidInstr();
  Get_Kraid(0)->kraid_var_A = FUNC16(Kraid_Death_UpdateBG2TilemapBottomHalf);
  Enemy_Kraid *E2 = Get_Kraid(0x80);
  E2->kraid_var_A = FUNC16(Kraid_AlignEnemyToKraid);
  E2->kraid_var_F = 0x7FFF;
  Enemy_Kraid *E3 = Get_Kraid(0xC0);
  E3->kraid_var_A = FUNC16(Kraid_AlignEnemyToKraid);
  E3->kraid_var_F = 0x7FFF;
  Enemy_Kraid *E4 = Get_Kraid(0x100);
  E4->kraid_var_A = FUNC16(Kraid_AlignEnemyToKraid);
  E4->kraid_var_F = 0x7FFF;
  Kraid_UpdateBg2TilemapTopHalf();
}

void Kraid_Death_UpdateBG2TilemapBottomHalf(void) {  // 0xA7C4C8
  Kraid_ProcessKraidInstr();
  unpause_hook.bank = -89;
  unpause_hook.addr = FUNC16(Kraid_UnpauseHook_IsSinking);
  Enemy_Kraid *E = Get_Kraid(0);
  E->kraid_var_A = FUNC16(Kraid_Death_SinkThroughFloor);
  kraid_unk9000 = 43;
  E->base.properties |= 0x8000;
  earthquake_type = 1;
  earthquake_timer = 256;
  Enemy_Kraid *E1 = Get_Kraid(0x40);
  E1->base.current_instruction = addr_kKraid_Ilist_8AA4;
  E1->base.instruction_timer = 1;
  Enemy_Kraid *E5 = Get_Kraid(0x140);
  E5->base.current_instruction = addr_kKraid_Ilist_86E7;
  E5->base.instruction_timer = 1;
  E5->kraid_var_A = FUNC16(nullsub_234);
  Kraid_UpdateBG2TilemapBottomHalf();
}

void Kraid_PlaySoundEveryHalfSecond(void) {  // 0xA7C51D
  if (!--kraid_unk9000) {
    QueueSfx3_Max6(0x1E);
    kraid_unk9000 = 30;
  }
}

void Kraid_HandleSinking(void) {  // 0xA7C59F
  int16 v1;
  VramWriteEntry *v3;

  for (int i = 0; ; ++i) {
    v1 = kKraidSinkEntry[i].field_0;
    if (v1 < 0)
      break;
    if (v1 == Get_Kraid(0)->base.y_pos) {
      if ((kKraidSinkEntry[i].field_2 & 0x8000) == 0) {
        uint16 v2 = vram_write_queue_tail;
        v3 = gVramWriteEntry(vram_write_queue_tail);
        v3->size = 64;
        v3->src.addr = 12224;
        v3->src.bank = 126;
        v3->vram_dst = kKraidSinkEntry[i].field_2 + ((reg_BG2SC & 0xFC) << 8);
        vram_write_queue_tail = v2 + 7;
      }
      CallKraidSinkTableFunc(kKraidSinkEntry[i].field_4 | 0xA70000);
      return;
    }
  }
}

void Kraid_Death_SinkThroughFloor(void) {  // 0xA7C537
  Kraid_ProcessKraidInstr();
  Kraid_PlaySoundEveryHalfSecond();
  Kraid_HandleSinking();
  Enemy_Kraid *E0 = Get_Kraid(0);
  if (!sign16(++E0->base.y_pos - 608)) {
    E0->base.properties &= ~kEnemyProps_Tangible;
    enemy_bg2_tilemap_size = 2;
    uint16 enemy_ptr = Get_Kraid(cur_enemy_index)->base.enemy_ptr;
    get_EnemyDef_A2(enemy_ptr)->shot_ai = FUNC16(nullsub_170_A7);
    Enemy_Kraid *E1 = Get_Kraid(0x40);
    uint16 v3 = E1->base.properties | kEnemyProps_Tangible | kEnemyProps_Deleted;
    E1->base.properties = v3;
    uint16 v4 = v3 & 0x51FF | 0x600;
    Get_Kraid(0x80)->base.properties = v4;
    Get_Kraid(0xC0)->base.properties = v4;
    Get_Kraid(0x100)->base.properties = v4;
    Get_Kraid(0x140)->base.properties = v4;
    Get_Kraid(0)->kraid_var_A = FUNC16(Kraid_FadeInBg_ClearBg2TilemapTopHalf);
    camera_distance_index = 0;
    Enemy_ItemDrop_Kraid(enemy_ptr);
    Kraid_DrawRoomBg();
  }
}

void Kraid_CrumbleLeftPlatform_Left(void) {  // 0xA7C691
  SpawnEnemyProjectileWithGfx(0x70, cur_enemy_index, addr_kEproj_RocksFallingKraidCeiling);
  SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x07, 0x12, 0xb7a7 });
}

void Kraid_CrumbleRightPlatform_Middle(void) {  // 0xA7C6A7
  SpawnEnemyProjectileWithGfx(0xF0, cur_enemy_index, addr_kEproj_RocksFallingKraidCeiling);
  SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x0f, 0x12, 0xb7a7 });
}

void Kraid_CrumbleRightPlatform_Left(void) {  // 0xA7C6BD
  SpawnEnemyProjectileWithGfx(0xE0, cur_enemy_index, addr_kEproj_RocksFallingKraidCeiling);
  SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x0e, 0x12, 0xb7af });
}

void Kraid_CrumbleLeftPlatform_Right(void) {  // 0xA7C6D3
  SpawnEnemyProjectileWithGfx(0x90, cur_enemy_index, addr_kEproj_RocksFallingKraidCeiling);
  SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x09, 0x12, 0xb7a7 });
}

void Kraid_CrumbleLeftPlatform_Middle(void) {  // 0xA7C6E9
  SpawnEnemyProjectileWithGfx(0x80, cur_enemy_index, addr_kEproj_RocksFallingKraidCeiling);
  SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x08, 0x12, 0xb7af });
}

void Kraid_CrumbleRightPlatform_Right(void) {  // 0xA7C6FF
  SpawnEnemyProjectileWithGfx(0x100, cur_enemy_index, addr_kEproj_RocksFallingKraidCeiling);
  SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x10, 0x12, 0xb7af });
}

void CallKraidSinkTableFunc(uint32 ea) {
  switch (ea) {
  case fnKraid_CrumbleLeftPlatform_Left: Kraid_CrumbleLeftPlatform_Left(); return;
  case fnnullsub_356: return;
  case fnKraid_CrumbleRightPlatform_Middle: Kraid_CrumbleRightPlatform_Middle(); return;
  case fnKraid_CrumbleRightPlatform_Left: Kraid_CrumbleRightPlatform_Left(); return;
  case fnKraid_CrumbleLeftPlatform_Right: Kraid_CrumbleLeftPlatform_Right(); return;
  case fnKraid_CrumbleLeftPlatform_Middle: Kraid_CrumbleLeftPlatform_Middle(); return;
  case fnKraid_CrumbleRightPlatform_Right: Kraid_CrumbleRightPlatform_Right(); return;
  default: Unreachable();
  }
}

void Kraid_FadeInBg_ClearBg2TilemapTopHalf(void) {  // 0xA7C715
  VramWriteEntry *v2;

  reg_BG2SC = 72;
  for (int i = 2046; i >= 0; i -= 2)
    tilemap_stuff[i >> 1] = 824;
  uint16 v1 = vram_write_queue_tail;
  v2 = gVramWriteEntry(vram_write_queue_tail);
  v2->size = 1024;
  v2->src.addr = 0x2000;
  *(uint16 *)&v2->src.bank = 126;
  v2->vram_dst = 18432;
  vram_write_queue_tail = v1 + 7;
  Get_Kraid(0)->kraid_var_A = FUNC16(Kraid_FadeInBg_ClearBg2TilemapBottomHalf);
}

void Kraid_FadeInBg_ClearBg2TilemapBottomHalf(void) {  // 0xA7C751
  VramWriteEntry *v1;

  uint16 v0 = vram_write_queue_tail;
  v1 = gVramWriteEntry(vram_write_queue_tail);
  v1->size = 1024;
  v1->src.addr = 0x2000;
  *(uint16 *)&v1->src.bank = 126;
  v1->vram_dst = 18944;
  vram_write_queue_tail = v0 + 7;
  Get_Kraid(0)->kraid_var_A = FUNC16(Kraid_FadeInBg_LoadBg3Tiles1of4);
}

void Kraid_FadeInBg_LoadBg3Tiles1of4(void) {  // 0xA7C777
  VramWriteEntry *v1;

  unpause_hook.addr = FUNC16(UnpauseHook_Kraid_IsDead);
  Get_Kraid(0)->kraid_var_A = FUNC16(Kraid_FadeInBg_LoadBg3Tiles2of4);
  uint16 v0 = vram_write_queue_tail;
  v1 = gVramWriteEntry(vram_write_queue_tail);
  v1->size = 1024;
  v1->src.addr = -19968;
  *(uint16 *)&v1->src.bank = 154;
  v1->vram_dst = 0x4000;
  vram_write_queue_tail = v0 + 7;
}

void Kraid_FadeInBg_LoadBg3Tiles2of4(void) {  // 0xA7C7A3
  VramWriteEntry *v1;

  Get_Kraid(0)->kraid_var_A = FUNC16(Kraid_FadeInBg_LoadBg3Tiles3of4);
  uint16 v0 = vram_write_queue_tail;
  v1 = gVramWriteEntry(vram_write_queue_tail);
  v1->size = 1024;
  v1->src.addr = -18944;
  *(uint16 *)&v1->src.bank = 154;
  v1->vram_dst = 16896;
  vram_write_queue_tail = v0 + 7;
}

void Kraid_FadeInBg_LoadBg3Tiles3of4(void) {  // 0xA7C7C9
  VramWriteEntry *v1;

  Get_Kraid(0)->kraid_var_A = FUNC16(Kraid_FadeInBg_LoadBg3Tiles4of4);
  uint16 v0 = vram_write_queue_tail;
  v1 = gVramWriteEntry(vram_write_queue_tail);
  v1->size = 1024;
  v1->src.addr = -17920;
  *(uint16 *)&v1->src.bank = 154;
  v1->vram_dst = 17408;
  vram_write_queue_tail = v0 + 7;
}

void Kraid_FadeInBg_LoadBg3Tiles4of4(void) {  // 0xA7C7EF
  VramWriteEntry *v1;

  Get_Kraid(0)->kraid_var_A = FUNC16(Kraid_FadeInBg_FadeInBp6);
  uint16 v0 = vram_write_queue_tail;
  v1 = gVramWriteEntry(vram_write_queue_tail);
  v1->size = 1024;
  v1->src.addr = -16896;
  *(uint16 *)&v1->src.bank = 154;
  v1->vram_dst = 17920;
  vram_write_queue_tail = v0 + 7;
}

void Kraid_FadeInBg_FadeInBp6(void) {  // 0xA7C815
  int16 v0;

  if (AdvancePaletteFade_BgPalette6() & 1) {
    QueueMusic_Delayed8(3);
    v0 = *(uint16 *)&boss_bits_for_area[area_index];
    if ((v0 & 1) != 0) {
      Get_Kraid(0)->kraid_var_A = FUNC16(Kraid_FadeInBg_SetEnemyDead_KraidWasDead);
    } else {
      *(uint16 *)&boss_bits_for_area[area_index] = v0 | 1;
      Get_Kraid(0)->kraid_var_A = FUNC16(Kraid_FadeInBg_SetEnemyDead_KraidWasAlive);
    }
  }
}

void Kraid_FadeInBg_SetEnemyDead_KraidWasAlive(void) {  // 0xA7C843
  if (!Kraid_CheckIfDead() || layer1_x_pos)
    Kraid_SetEnemyPropsToDead();
}

void Kraid_FadeInBg_SetEnemyDead_KraidWasDead(void) {  // 0xA7C851
  if (!Kraid_CheckIfDead() || layer1_x_pos)
    Kraid_SetEnemyPropsToDead();
  previous_layer1_x_block = -1;
}

void Kraid_RestrictSamusXtoFirstScreen(uint16 k) {  // 0xA7C865
  Kraid_RestrictSamusXtoFirstScreen_2();
  KraidEnemy_HandleFunctionTimer(k);
}

void Kraid_UpdateBg2TilemapTopHalf(void) {  // 0xA7C874
  VramWriteEntry *v1;

  uint16 v0 = vram_write_queue_tail;
  v1 = gVramWriteEntry(vram_write_queue_tail);
  v1->size = 2048;
  v1->src.addr = 0x2000;
  v1->src.bank = 126;
  v1->vram_dst = (reg_BG2SC & 0xFC) << 8;
  vram_write_queue_tail = v0 + 7;
}

void Kraid_UpdateBG2TilemapBottomHalf(void) {  // 0xA7C8B6
  VramWriteEntry *v1;

  uint16 v0 = vram_write_queue_tail;
  v1 = gVramWriteEntry(vram_write_queue_tail);
  v1->size = 2048;
  v1->src.addr = 10240;
  v1->src.bank = 126;
  v1->vram_dst = ((reg_BG2SC & 0xFC) << 8) + 2048;
  vram_write_queue_tail = v0 + 7;
}

void Kraid_RaiseKraidThroughFloor(uint16 k) {  // 0xA7C86B
  Kraid_RestrictSamusXtoFirstScreen_2();
  Get_Kraid(0)->kraid_var_A = FUNC16(Kraid_Raise_LoadTilemapBottomAndShake);
  Kraid_UpdateBg2TilemapTopHalf();
}

void Kraid_Raise_LoadTilemapBottomAndShake(void) {  // 0xA7C89A
  Kraid_RestrictSamusXtoFirstScreen_2();
  Enemy_Kraid *E = Get_Kraid(0);
  E->kraid_var_A = FUNC16(Kraid_Raise_SpawnRandomEarthquakeProjs16);
  E->kraid_var_F = 120;
  earthquake_timer = 496;
  QueueMusic_Delayed8(5);
  Kraid_UpdateBG2TilemapBottomHalf();
}

void Kraid_Raise_SpawnRandomEarthquakeProjs16(void) {  // 0xA7C8E0
  Kraid_RestrictSamusXtoFirstScreen_2();
  Enemy_Kraid *E = Get_Kraid(0);
  uint16 v1 = E->kraid_var_F - 1;
  E->kraid_var_F = v1;
  if (v1) {
    if ((v1 & 0xF) == 0)
      Kraid_SpawnRandomQuakeProjs();
  } else {
    E->kraid_var_A = FUNC16(Kraid_Raise_SpawnRandomEarthquakeProjs8);
    E->kraid_var_F = 96;
  }
}

void Kraid_Raise_SpawnRandomEarthquakeProjs8(void) {  // 0xA7C902
  Kraid_RestrictSamusXtoFirstScreen_2();
  Enemy_Kraid *E = Get_Kraid(0);
  uint16 v1 = E->kraid_var_F - 1;
  E->kraid_var_F = v1;
  if (v1) {
    if ((v1 & 7) == 0)
      Kraid_SpawnRandomQuakeProjs();
  } else {
    E->kraid_var_A = FUNC16(Kraid_Raise_Handler);
    E->kraid_var_F = 288;
  }
}

void Kraid_Raise_Handler(void) {  // 0xA7C924
  Kraid_RestrictSamusXtoFirstScreen_2();
  if ((earthquake_timer & 5) == 0)
    Kraid_SpawnRandomQuakeProjs();
  Enemy_Kraid *E0 = Get_Kraid(0);
  E0->base.x_pos += (E0->base.y_pos & 2) == 0 ? -1 : 1;
  AddToHiLo(&E0->base.y_pos, &E0->base.y_subpos, -0x8000);
  if (sign16(E0->base.y_pos - 457)) {
    E0->base.x_pos = 176;
    Enemy_Kraid *E5 = Get_Kraid(0x140);
    E5->kraid_var_A = FUNC16(KraidFoot_FirstPhase_Thinking);
    E5->kraid_var_F = 300;
    E5->kraid_next = FUNC16(KraidsFoot_PrepareToLungeForward);
    E0->kraid_var_B = addr_stru_A796DA;
    Kraid_Mainloop_Thinking_Setup();
    Enemy_Kraid *E1 = Get_Kraid(0x40);
    E1->base.current_instruction = addr_kKraid_Ilist_89F3;
    E1->base.instruction_timer = 1;
  }
}

void Kraid_SpawnRandomQuakeProjs(void) {  // 0xA7C995
  uint16 v0 = random_number & 0x3F;
  if ((random_number & 2) == 0)
    v0 = ~v0;
  uint16 x = Get_Kraid(0)->base.x_pos + v0;
  uint16 y = 448 - ((uint16)(random_number & 0x3F00) >> 8);
  CreateSpriteAtPos(x, y, 21, 0);
  uint16 v1 = ((random_number & 0x10) != 0) ? addr_kEproj_RocksWhenKraidRisesRight : addr_kEproj_RocksWhenKraidRisesLeft;
  SpawnEnemyProjectileWithGfx(random_number & 0x3F0, cur_enemy_index, v1);
}

void Kraid_RestrictSamusXtoFirstScreen_2(void) {  // 0xA7C9EE
  if ((int16)(samus_x_pos - 256) >= 0) {
    samus_x_pos = 256;
    samus_prev_x_pos = 256;
  }
}

void CallKraidFunc(uint32 ea) {
  uint16 k = cur_enemy_index;
  switch (ea) {
  case fnKraid_GetsBig_BreakCeilingPlatforms: Kraid_GetsBig_BreakCeilingPlatforms(); return;  // 0x9bc35
  case fnKraid_GetsBig_SetBG2TilemapPrioBits: Kraid_GetsBig_SetBG2TilemapPrioBits(); return;  // 0x9bde6
  case fnKraid_GetsBig_FinishUpdateBg2Tilemap: Kraid_GetsBig_FinishUpdateBg2Tilemap(); return;  // 0x9be51
  case fnKraid_GetsBig_DrawRoomBg: Kraid_GetsBig_DrawRoomBg(); return;  // 0x9bee5
  case fnKraid_GetsBig_FadeInRoomBg: Kraid_GetsBig_FadeInRoomBg(); return;  // 0x9c08c
  case fnKraid_Mainloop_Thinking: Kraid_Mainloop_Thinking(); return;  // 0x9c1b8
  case fnKraid_GetsBig_Thinking: Kraid_GetsBig_Thinking(); return;  // 0x9c207
  case fnKraid_Shot_MouthIsOpen: Kraid_Shot_MouthIsOpen(); return;  // 0x9c256
  case fnKraid_InitEyeGlowing: Kraid_InitEyeGlowing(); return;  // 0x9cf64
  case fnKraid_Shot_GlowHisEye: Kraid_Shot_GlowHisEye(); return;  // 0x9cf9f
  case fnKraid_Shot_UnglowEye: Kraid_Shot_UnglowEye(); return;  // 0x9d0b4
  case fnKraidLint_ProduceLint: KraidLint_ProduceLint(k); return;  // 0x9d371
  case fnKraidLint_FireLint: KraidLint_FireLint(k); return;  // 0x9d44f
  case fnKraidFingernail_WaitForLintXpos: KraidFingernail_WaitForLintXpos(k); return;  // 0x9d556
  case fnKraid_AlignEnemyToKraid: Kraid_AlignEnemyToKraid(k); return;  // 0x9d5b5
  case fnKraidEnemy_HandleFunctionTimer: KraidEnemy_HandleFunctionTimer(k); return;  // 0x9d5e5
  case fnKraidEnemy_DecrementEnemyFunctionTimer: KraidEnemy_DecrementEnemyFunctionTimer(); return;  // 0x9d61c
  case fnKraidFoot_FirstPhase_Thinking: KraidFoot_FirstPhase_Thinking(k); return;  // 0x9d66a
  case fnKraidEnemy_ProcessInstrEnemyTimer: KraidEnemy_ProcessInstrEnemyTimer(k); return;  // 0x9d67d
  case fnKraidsFoot_SecondPhase_Thinking: KraidsFoot_SecondPhase_Thinking(); return;  // 0x9d8b2
  case fnKraidsFoot_SecondPhase_WalkingBackwards: KraidsFoot_SecondPhase_WalkingBackwards(); return;  // 0x9da2d
  case fnKraidsFoot_SecondPhaseSetup_WalkToStartPt: KraidsFoot_SecondPhaseSetup_WalkToStartPt(); return;  // 0x9daa2
  case fnKraidsFoot_SecondPhase_Init: KraidsFoot_SecondPhase_Init(); return;  // 0x9db2a
  case fnKraidsFoot_SecondPhase_WalkForward: KraidsFoot_SecondPhase_WalkForward(); return;  // 0x9db40
  case fnKraid_Main_AttackWithMouthOpen: Kraid_Main_AttackWithMouthOpen(); return;  // 0x9dbe0
  case fnKraidsFingernail_Init: KraidsFingernail_Init(); return;  // 0x9df94
  case fnKraidsFingernail_Fire: KraidsFingernail_Fire(k); return;  // 0x9e1ac
  case fnKraidsFoot_PrepareToLungeForward: KraidsFoot_PrepareToLungeForward(); return;  // 0x9e312
  case fnKraidsFoot_FirstPhase_RetreatFromLunge: KraidsFoot_FirstPhase_RetreatFromLunge(); return;  // 0x9e43a
  case fnKraid_GetsBig_ReleaseCamera: Kraid_GetsBig_ReleaseCamera(); return;  // 0x9e6a7
  case fnKraid_Death_Init: Kraid_Death_Init(); return;  // 0x9ec0b
  case fnKraid_Death_Fadeout: Kraid_Death_Fadeout(); return;  // 0x9ed75
  case fnKraid_Death_UpdateBG2TilemapTopHalf: Kraid_Death_UpdateBG2TilemapTopHalf(); return;  // 0x9ef19
  case fnKraid_Death_UpdateBG2TilemapBottomHalf: Kraid_Death_UpdateBG2TilemapBottomHalf(); return;  // 0x9ef92
  case fnKraid_Death_SinkThroughFloor: Kraid_Death_SinkThroughFloor(); return;  // 0x9f074
  case fnKraid_FadeInBg_ClearBg2TilemapTopHalf: Kraid_FadeInBg_ClearBg2TilemapTopHalf(); return;  // 0x9f369
  case fnKraid_FadeInBg_ClearBg2TilemapBottomHalf: Kraid_FadeInBg_ClearBg2TilemapBottomHalf(); return;  // 0x9f3ed
  case fnKraid_FadeInBg_LoadBg3Tiles1of4: Kraid_FadeInBg_LoadBg3Tiles1of4(); return;  // 0x9f448
  case fnKraid_FadeInBg_LoadBg3Tiles2of4: Kraid_FadeInBg_LoadBg3Tiles2of4(); return;  // 0x9f4ad
  case fnKraid_FadeInBg_LoadBg3Tiles3of4: Kraid_FadeInBg_LoadBg3Tiles3of4(); return;  // 0x9f507
  case fnKraid_FadeInBg_LoadBg3Tiles4of4: Kraid_FadeInBg_LoadBg3Tiles4of4(); return;  // 0x9f561
  case fnKraid_FadeInBg_FadeInBp6: Kraid_FadeInBg_FadeInBp6(); return;  // 0x9f5bb
  case fnKraid_FadeInBg_SetEnemyDead_KraidWasAlive: Kraid_FadeInBg_SetEnemyDead_KraidWasAlive(); return;  // 0x9f62d
  case fnKraid_FadeInBg_SetEnemyDead_KraidWasDead: Kraid_FadeInBg_SetEnemyDead_KraidWasDead(); return;  // 0x9f65b
  case fnKraid_RestrictSamusXtoFirstScreen: Kraid_RestrictSamusXtoFirstScreen(k); return;  // 0x9f694
  case fnKraid_RaiseKraidThroughFloor: Kraid_RaiseKraidThroughFloor(k); return;  // 0x9f6a7
  case fnKraid_Raise_LoadTilemapBottomAndShake: Kraid_Raise_LoadTilemapBottomAndShake(); return;  // 0x9f71f
  case fnKraid_Raise_SpawnRandomEarthquakeProjs16: Kraid_Raise_SpawnRandomEarthquakeProjs16(); return;  // 0x9f7b8
  case fnKraid_Raise_SpawnRandomEarthquakeProjs8: Kraid_Raise_SpawnRandomEarthquakeProjs8(); return;  // 0x9f805
  case fnKraid_Raise_Handler: Kraid_Raise_Handler(); return;  // 0x9f852
  case fnKraidLint_ChargeLint: KraidLint_ChargeLint(k); return;  // 0xa7b868
  case fnnullsub_234: return;  // 0xa7ba2d
  case fnKraidsFoot_FirstPhase_LungeForward: KraidsFoot_FirstPhase_LungeForward(); return;  // 0xa7bf5d
  case fnnullsub_347: return;
  default: Unreachable();
  }
}

void Phantoon_Init(void) {  // 0xA7CDF3
  int16 j;
  uint16 k;

  for (int i = 4094; i >= 0; i -= 2)
    tilemap_stuff[i >> 1] = 824;
  for (j = 2046; j >= 0; j -= 2)
    *(uint16 *)((uint8 *)&kraid_unk9000 + (uint16)j) = 0;
  for (k = 30; (k & 0x8000) == 0; k -= 2)
    target_palettes[(k >> 1) + 112] = 0;
  enemy_bg2_tilemap_size = 864;
  DisableMinimapAndMarkBossRoomAsExplored();
  Enemy_Phantoon *E = Get_Phantoon(cur_enemy_index);
  E->phant_var_E = 120;
  E->phant_var_A = 0;
  E->phant_var_B = 0;
  g_word_7E9032 = 0;
  Enemy_Phantoon *E1 = Get_Phantoon(0x40);
  E1->phant_parameter_1 = 0;
  E1->phant_parameter_2 = 0;
  Enemy_Phantoon *E0 = Get_Phantoon(0);
  E0->base.properties |= kEnemyProps_Tangible;
  static const SpawnHdmaObject_Args unk_A7CE51 = { 0x01, 0x14, 0xce96 };
  SpawnHdmaObject(0xa7, &unk_A7CE51);
  Phantoon2_Init();
}

void Phantoon2_Init(void) {  // 0xA7CE55
  EnemyData *v6 = gEnemyData(cur_enemy_index);
  v6->spritemap_pointer = addr_kSpritemap_Nothing_A7;
  v6->instruction_timer = 1;
  v6->timer = 0;
  EnemyData *v7 = gEnemyData(0);
  v6->palette_index = v7->palette_index;
  v6->vram_tiles_index = v7->vram_tiles_index;
  v6->current_instruction = g_off_A7CE8E[v6->parameter_2];
  v6->ai_preinstr = FUNC16(Phantoon_Spawn8FireballsInCircleAtStart);
  v7[3].parameter_1 = 0;
  v7[3].ai_var_C = -1;
}

void Phantoon_Main(void) {  // 0xA7CEA6
  Phantoon_Func_2(cur_enemy_index);
  Enemy_Phantoon *EK = Get_Phantoon(cur_enemy_index);
  CallEnemyPreInstr(EK->phant_var_F | 0xA70000);
  if (cur_enemy_index == 0) { // code bug: X is overwritten
    Enemy_Phantoon *E0 = Get_Phantoon(0);
    Enemy_Phantoon *E1 = Get_Phantoon(0x40);
    Enemy_Phantoon *E2 = Get_Phantoon(0x80);
    Enemy_Phantoon *E3 = Get_Phantoon(0xC0);
    uint16 x_pos = E0->base.x_pos;
    E1->base.x_pos = x_pos;
    E2->base.x_pos = x_pos;
    E3->base.x_pos = x_pos;
    uint16 y_pos = E0->base.y_pos;
    E1->base.y_pos = y_pos;
    E2->base.y_pos = y_pos;
    E3->base.y_pos = y_pos;
    if (!E1->phant_parameter_1) {
      reg_BG2HOFS = layer1_x_pos - E0->base.x_pos + 40;
      reg_BG2VOFS = layer1_y_pos - E0->base.y_pos + 40;
    }
  }



}

void Phantoon_Func_1(void) {  // 0xA7CEED
  QueueSfx2_Max6(g_word_A7CDED[g_word_7E9032]);
  uint16 v0 = g_word_7E9032 + 1;
  if (!sign16(g_word_7E9032 - 2))
    v0 = 0;
  g_word_7E9032 = v0;
}

void Phantoon_Func_2(uint16 k) {  // 0xA7CF0C
  if (!k && (joypad1_newkeys & 0x4000) != 0)
    *(uint16 *)((uint8 *)&g_stru_A7902D[0].ypos + 1) = *(uint16 *)((uint8 *)&g_stru_A7902D[0].ypos + 1) == 0;
}

uint8 Phantoon_Func_3(int32 amt) {  // 0xA7CF27
  Enemy_Phantoon *E = Get_Phantoon(0xC0);
  uint16 r18 = amt, r20 = amt >> 16;
  if (E->phant_var_E) {
    uint16 phant_var_D = E->phant_var_D;
    bool v4 = phant_var_D < r18;
    E->phant_var_D = phant_var_D - r18;
    if (v4) {
      E->phant_var_D = 0;
      return 1;
    }
  } else {
    uint16 r22 = (r20 & 0xFF00) >> 8;
    uint16 v1 = r18 + E->phant_var_D;
    E->phant_var_D = v1;
    v1 = (v1 & 0xFF00) >> 8;
    if (!sign16(v1 - r22))
      E->phant_var_D = r20;
  }
  return 0;
}

void Phantoon_Func_4(uint16 k) {  // 0xA7CF5E
  SpawnEnemyProjectileWithGfx(0, k, addr_kEproj_DestroyableFireballs);
  QueueSfx3_Max6(0x1D);
}

void Phantoon_Func_5(uint16 k) {  // 0xA7CF70
  for (int i = 7; i >= 0; --i)
    SpawnEnemyProjectileWithGfx(i | 0x600, k, addr_kEproj_DestroyableFireballs);
  QueueSfx3_Max6(0x28);
}

void Phantoon_Func_6(uint16 k, uint16 a) {  // 0xA7CF8B
  int16 v2;
  int16 v3;

  v2 = g_byte_A7CFC2[a];
  int n = 7;
  int r20 = 16;
  do {
    v3 = v2;
    SpawnEnemyProjectileWithGfx(r20 | v2++ | 0x400, k, addr_kEproj_DestroyableFireballs);
    if ((int16)(v3 - 8) >= 0)
      v2 = 0;
    r20 += 16;
  } while (--n >= 0);
}

void Phantoon_Func_7(uint16 k) {  // 0xA7CFCA
  Enemy_Phantoon *E = Get_Phantoon(k + 192);
  bool v3 = E->phant_var_B == 1;
  bool v4 = (--E->phant_var_B & 0x8000) != 0;
  if (v3 || v4) {
    if ((E->phant_var_C & 0x8000) == 0) {
      v3 = E->phant_var_C == 1;
      bool v8 = (--E->phant_var_C & 0x8000) != 0;
      if (v3 || v8) {
        E->phant_var_C = -1;
        E->phant_var_B = *((uint16 *)RomPtr_A7(g_off_A7CCFD[E->phant_var_A]) + 1);
      } else {
        E->phant_var_B = *((uint16 *)RomPtr_A7(2 * E->phant_var_C + g_off_A7CCFD[E->phant_var_A]) + 1);
      }
      E->base.instruction_timer = 1;
      E->base.current_instruction = addr_kKraid_Ilist_CCEB;
    } else {
      uint16 v5 = NextRandom() & 3;
      E->phant_var_A = v5;
      uint16 v6 = g_off_A7CCFD[v5];
      uint16 v7 = *(uint16 *)RomPtr_A7(v6);
      E->phant_var_C = v7;
      E->phant_var_B = *((uint16 *)RomPtr_A7(2 * v7 + v6) + 1);
    }
  }
}

void Phantoon_StartTrackingSamusAndInitEyeTimer(void) {  // 0xA7D03F
  Get_Phantoon(0x80)->phant_var_A = 0;
  Enemy_Phantoon *E = Get_Phantoon(0);
  E->base.instruction_timer = 1;
  E->base.current_instruction = addr_kKraid_Ilist_CC4D;
  E->base.properties &= ~kEnemyProps_Tangible;
  E->phant_var_E = g_word_A7CD41[NextRandom() & 7];
  E->phant_var_F = FUNC16(Phantoon_EyeFollowsSamusUntilTimerRunsOut);

  Enemy_Phantoon *E1 = Get_Phantoon(0x40);
  E1->base.instruction_timer = 1;
  E1->base.current_instruction = addr_kKraid_Ilist_CC9D;
}

void Phantoon_PickPatternForRound2(void) {  // 0xA7D076
  int16 v4;

  Enemy_Phantoon *E = Get_Phantoon(0);
  E->phant_var_E = 60;
  uint16 v1 = g_word_A7CD53[NextRandom() & 7];
  Enemy_Phantoon *E1 = Get_Phantoon(0x40);
  E1->phant_var_A = v1;
  if ((nmi_frame_counter_word & 1) != 0) {
    if (!E1->phant_var_C) {
      v4 = E->phant_var_A - 1;
      E->phant_var_A = v4;
      if (v4 < 0)
        E->phant_var_A = 533;
    }
    E->phant_var_C = 0;
    E->phant_var_B = 0;
    E->phant_var_D = 0;
    E1->phant_var_C = 1;
  } else {
    if (E1->phant_var_C) {
      uint16 v3 = E->phant_var_A + 1;
      E->phant_var_A = v3;
      if (!sign16(v3 - 534))
        E->phant_var_A = 0;
    }
    E->phant_var_C = 1;
    E->phant_var_B = 0;
    E->phant_var_D = 0;
    E1->phant_var_C = 0;
  }
  if (E->phant_parameter_2) {
    E1->phant_var_F = 0;
    E->phant_var_F = FUNC16(Phantoon_FadeOutBeforeFirstFireballRain);
  } else {
    E->phant_var_F = FUNC16(Phantoon_MovePhantoonInFigure8ThenOpenEye);
  }
}

void Phantoon_AdjustSpeedAndMoveInFigure8(void) {  // 0xA7D0F1
  if (Get_Phantoon(0x40)->phant_var_C) {
    Phantoon_AdjustSpeedRightSideClockwise();
    Phantoon_MoveInFigure8_RightSideClockwise(addr_kPhantoonMoveData, 533);
  } else {
    Phantoon_AdjustSpeedLeftSideClockwise();
    Phantoon_MoveInFigure8_LeftSideClockwise(addr_kPhantoonMoveData, 534);
  }
}

void Phantoon_AdjustSpeedLeftSideClockwise(void) {  // 0xA7D114
  Enemy_Phantoon *E = Get_Phantoon(0);
  uint16 phant_var_D = E->phant_var_D;
  if (phant_var_D) {
    if ((phant_var_D & 1) != 0) {
      AddToHiLo(&E->phant_var_C, &E->phant_var_B, __PAIR32__(g_word_A7CD79, g_word_A7CD77));
      if ((int16)(E->phant_var_C - g_word_A7CD7D) >= 0) {
        E->phant_var_C = g_word_A7CD7D;
        E->phant_var_B = 0;
        ++E->phant_var_D;
      }
    } else {
      AddToHiLo(&E->phant_var_C, &E->phant_var_B, -IPAIR32(g_word_A7CD79, g_word_A7CD77));
      if (E->phant_var_C == g_word_A7CD7F || (int16)(E->phant_var_C - g_word_A7CD7F) < 0) {
        E->phant_var_C = g_word_A7CD7F + 1;
        E->phant_var_B = 0;
        E->phant_var_D = 0;
      }
    }
  } else {
    AddToHiLo(&E->phant_var_C, &E->phant_var_B, __PAIR32__(g_word_A7CD75, g_word_A7CD73));
    if ((int16)(E->phant_var_C - g_word_A7CD7B) >= 0) {
      E->phant_var_C = g_word_A7CD7B - 1;
      E->phant_var_B = 0;
      ++E->phant_var_D;
    }
  }
}

void Phantoon_AdjustSpeedRightSideClockwise(void) {  // 0xA7D193
  Enemy_Phantoon *E = Get_Phantoon(0);
  uint16 phant_var_D = E->phant_var_D;
  if (phant_var_D) {
    if ((phant_var_D & 1) != 0) {
      AddToHiLo(&E->phant_var_C, &E->phant_var_B, -IPAIR32(g_word_A7CD87, g_word_A7CD85));
      if (E->phant_var_C == g_word_A7CD8B || (int16)(E->phant_var_C - g_word_A7CD8B) < 0) {
        E->phant_var_C = g_word_A7CD8B + 1;
        E->phant_var_B = 0;
        ++E->phant_var_D;
      }
    } else {
      AddToHiLo(&E->phant_var_C, &E->phant_var_B, __PAIR32__(g_word_A7CD87, g_word_A7CD85));
      if ((int16)(E->phant_var_C - g_word_A7CD8D) >= 0) {
        E->phant_var_C = g_word_A7CD8D;
        E->phant_var_B = 0;
        E->phant_var_D = 0;
      }
    }
  } else {
    AddToHiLo(&E->phant_var_C, &E->phant_var_B, -IPAIR32(g_word_A7CD83, g_word_A7CD81));
    if (E->phant_var_C == g_word_A7CD89 || (int16)(E->phant_var_C - g_word_A7CD89) < 0) {
      E->phant_var_C = (uint16)(g_word_A7CD89 + 2);
      E->phant_var_B = 0;
      ++E->phant_var_D;
    }
  }
}

void Phantoon_MoveInFigure8_LeftSideClockwise(uint16 j, uint16 r20) {  // 0xA7D215
  Enemy_Phantoon *E = Get_Phantoon(0);
  for (int n = E->phant_var_C; n; --n) {
    const uint8 *v2 = RomPtr_A7(2 * E->phant_var_A + j);
    E->base.x_pos += (int8)v2[0];
    E->base.y_pos += (int8)v2[1];
    if (!sign16(++E->phant_var_A - r20))
      E->phant_var_A = 0;
  }
}

void Phantoon_MoveInFigure8_RightSideClockwise(uint16 j, uint16 r20) {  // 0xA7D271
  Enemy_Phantoon *E = Get_Phantoon(0);
  for(int n = (uint16)-E->phant_var_C; n; n--) {
    const uint8 *v4 = RomPtr_A7(2 * E->phant_var_A + j);
    E->base.x_pos -= (int8)v4[0];
    E->base.y_pos -= (int8)v4[1];
    if (sign16(--E->phant_var_A))
      E->phant_var_A = r20;
  }
}

void Phantoon_MoveInSwoopingPattern(uint16 k) {  // 0xA7D2D1
  int16 phant_var_E;
  uint16 v4;
  uint16 v5;
  uint16 v6;
  int16 v13;

  Enemy_Phantoon *E2 = Get_Phantoon(0x80);
  phant_var_E = E2->phant_var_E;
  if (phant_var_E < 0) {
    v6 = phant_var_E - 2;
    E2->phant_var_E = v6;
    v4 = v6 & 0x7FFF;
    if (!v4) {
      v4 = 0;
      E2->phant_var_E = 0;
    }
  } else {
    v4 = phant_var_E + 2;
    E2->phant_var_E = v4;
    if (!sign16(v4 - 256)) {
      v5 = v4 | 0x8000;
      E2->phant_var_E = v5;
      v4 = v5 & 0x7FFF;
    }
  }
  Enemy_Phantoon *E0 = Get_Phantoon(0);
  if ((int16)(v4 - E0->base.x_pos) < 0) {
    if (!sign16(E2->phant_var_C + 2047))
      E2->phant_var_C -= 32;
  } else if (sign16(E2->phant_var_C - 2048)) {
    E2->phant_var_C += 32;
  }
  AddToHiLo(&E0->base.x_pos, &E0->base.x_subpos, INT16_SHL8(E2->phant_var_C));
  if (sign16(E0->base.x_pos + 64)) {
    E0->base.x_pos = -64;
  } else if (!sign16(E0->base.x_pos - 448)) {
    E0->base.x_pos = 448;
  }
  if (Get_Phantoon(k)->phant_var_F == FUNC16(Phantoon_CompleteSwoopAfterFatalShot))
    v13 = 112;
  else
    v13 = samus_y_pos - 48;
  if ((int16)(v13 - E0->base.y_pos) < 0) {
    if (!sign16(E2->phant_var_D + 1535))
      E2->phant_var_D -= 64;
  } else if (sign16(E2->phant_var_D - 1536)) {
    E2->phant_var_D += 64;
  }
  AddToHiLo(&E0->base.y_pos, &E0->base.y_subpos, INT16_SHL8(E2->phant_var_D));
  if (sign16(E0->base.y_pos - 64)) {
    E0->base.y_pos = 64;
  } else if (!sign16(E0->base.y_pos - 216)) {
    E0->base.y_pos = 216;
  }
}

void Phantoon_BeginSwoopingPattern(uint16 k) {  // 0xA7D3E1
  Enemy_Phantoon *E2 = Get_Phantoon(0x80);
  E2->phant_var_C = 1024;
  E2->phant_var_D = 1024;
  E2->phant_var_E = 0;
  Enemy_Phantoon *E = Get_Phantoon(k);
  E->phant_var_F = FUNC16(Phantoon_IsSwooping);
  E->phant_var_E = 360;
}

void Phantoon_ChangeEyeSpriteBasedOnSamusDist(void) {  // 0xA7D3FA
  uint16 v0 = 2 * DetermineDirectionOfSamusFromEnemy();
  Enemy_Phantoon *E = Get_Phantoon(0x40);
  E->base.instruction_timer = 1;
  E->base.current_instruction = g_off_A7D40D[v0 >> 1];
}

void Phantoon_StartDeathSequence(uint16 k) {  // 0xA7D421
  int16 v3;

  Enemy_Phantoon *E = Get_Phantoon(k);
  if (E->phant_var_F == FUNC16(Phantoon_IsSwooping)
      || E->phant_var_F == FUNC16(Phantoon_FadeoutWithSwoop)) {
    E->phant_var_F = FUNC16(Phantoon_CompleteSwoopAfterFatalShot);
  } else {
    E->phant_var_F = FUNC16(Phantoon_DyingPhantoonFadeInOut);
  }
  Enemy_Phantoon *E1 = Get_Phantoon(0x40);
  E1->phant_var_C = 0;
  E1->phant_var_F = 0;
  Phantoon_ChangeEyeSpriteBasedOnSamusDist();
  v3 = 510;
  uint16 v4 = reg_BG2HOFS;
  do {
    *(uint16 *)((uint8 *)&g_word_7E9100 + (uint16)v3) = v4;
    v3 -= 2;
  } while (v3 >= 0);
  phantom_related_layer_flag |= 0x4000;
  Get_Phantoon(0xC0)->phant_parameter_2 = 1;
}

void Phantoon_FadeOut(uint16 a) {  // 0xA7D464
  if ((nmi_frame_counter_word & 1) == 0) {
    Enemy_Phantoon *E = Get_Phantoon(0x40);
    if (!E->phant_var_F) {
      E->phant_var_D = a;
      if (Phantoon_Func_8() & 1)
        E->phant_var_F = 1;
    }
  }
}

void Phantoon_FadeIn(uint16 a) {  // 0xA7D486
  if ((nmi_frame_counter_word & 1) == 0) {
    Enemy_Phantoon *E = Get_Phantoon(0x40);
    if (!E->phant_var_F) {
      E->phant_var_D = a;
      if (Phantoon_SetColorBasedOnHp() & 1)
        E->phant_var_F = 1;
    }
  }
}

void Phantoon_Spawn8FireballsInCircleAtStart(uint16 k) {  // 0xA7D4A9
  Enemy_Phantoon *EK = Get_Phantoon(k);
  bool v2 = EK->phant_var_E == 1;
  bool v3 = (--EK->phant_var_E & 0x8000) != 0;
  if (v2 || v3) {
    Enemy_Phantoon *E0 = Get_Phantoon(0);
    SpawnEnemyProjectileWithGfx(E0->phant_var_A, k, addr_kEproj_StartingFireballs);
    QueueSfx3_Max6(0x1D);
    EK->phant_var_E = 30;
    uint16 v5 = E0->phant_var_A + 1;
    E0->phant_var_A = v5;
    if (!sign16(v5 - 8)) {
      E0->phant_var_A = 0;
      Get_Phantoon(k + 128)->phant_var_B = 0;
      EK->phant_var_F = FUNC16(Phantoon_WaitBetweenSpawningAndSpinningFireballs);
      EK->phant_var_E = 30;
      SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x00, 0x06, 0xb781 });
    }
  }
}

void Phantoon_WaitBetweenSpawningAndSpinningFireballs(uint16 k) {  // 0xA7D4EE
  Enemy_Phantoon *E = Get_Phantoon(k);
  bool v2 = E->phant_var_E == 1;
  bool v3 = (--E->phant_var_E & 0x8000) != 0;
  if (v2 || v3) {
    E->phant_var_E = 240;
    E->phant_var_B = 1;
    E->phant_var_F = FUNC16(Phantoon_SpawnFireballsBeforeFight);
  }
}

void Phantoon_SpawnFireballsBeforeFight(uint16 k) {  // 0xA7D508
  Enemy_Phantoon *EK = Get_Phantoon(k);
  bool v3 = EK->phant_var_E == 1;
  bool v4 = (--EK->phant_var_E & 0x8000) != 0;
  if (v3 || v4) {
    EK->phant_var_B = 0;
    phantom_related_layer_flag |= 0x4000;
    EK->phant_var_F = FUNC16(Phantoon_WavyFadeIn);
    Enemy_Phantoon *E3 = Get_Phantoon(0xC0);
    E3->phant_parameter_1 = -32767;
    EK->phant_var_E = 120;
    sub_88E487(2, g_word_A7CDA3);
    E3->phant_var_D = g_word_A7CD9D;
    Get_Phantoon(0x40)->phant_var_F = 0;
    QueueMusic_Delayed8(5);
  }
}

void Phantoon_WavyFadeIn(uint16 k) {  // 0xA7D54A
  Phantoon_FadeIn(0xC);
  if (Phantoon_Func_3(__PAIR32__(g_word_A7CD9D, g_word_A7CD9B))) {
    Enemy_Phantoon *E = Get_Phantoon(k);
    E->phant_var_F = FUNC16(Phantoon_PickPatternForRound1);
    Get_Phantoon(0xC0)->phant_parameter_1 = 1;
    E->phant_var_E = 30;
  } else {
    Enemy_Phantoon *E = Get_Phantoon(k);
    bool v2 = E->phant_var_E == 1;
    bool v3 = (--E->phant_var_E & 0x8000) != 0;
    if (v2 || v3)
      Get_Phantoon(0xC0)->phant_var_E = 1;
  }
}

void Phantoon_PickPatternForRound1(uint16 k) {  // 0xA7D596
  Enemy_Phantoon *EK = Get_Phantoon(k);
  bool v2 = EK->phant_var_E == 1;
  bool v3 = (--EK->phant_var_E & 0x8000) != 0;
  if (v2 || v3) {
    Enemy_Phantoon *E1 = Get_Phantoon(0x40);
    E1->phant_parameter_1 = 0;
    EK->phant_var_F = FUNC16(Phantoon_MovePhantoonInFigure8ThenOpenEye);
    E1->phant_var_A = g_word_A7CD53[(nmi_frame_counter_word >> 1) & 3];
    if ((NextRandom() & 1) != 0) {
      Enemy_Phantoon *E = Get_Phantoon(0);
      E->phant_var_C = 0;
      E->phant_var_B = 0;
      E->phant_var_D = 0;
      E1->phant_var_C = 1;
      E->phant_var_A = 533;
    } else {
      Enemy_Phantoon *E = Get_Phantoon(0);
      E->phant_var_C = 1;
      E->phant_var_B = 0;
      E->phant_var_D = 0;
      E1->phant_var_C = 0;
      E->phant_var_A = 0;
    }
  }
}

void Phantoon_MovePhantoonInFigure8ThenOpenEye(uint16 k) {  // 0xA7D5E7
  Phantoon_AdjustSpeedAndMoveInFigure8();
  Phantoon_Func_7(k);
  Enemy_Phantoon *E = Get_Phantoon(0x40);
  bool v2 = E->phant_var_A == 1;
  bool v3 = (--E->phant_var_A & 0x8000) != 0;
  if (v2 || v3) {
    Get_Phantoon(k)->phant_var_F = FUNC16(nullsub_237);
    E->base.instruction_timer = 1;
    E->base.current_instruction = addr_kKraid_Ilist_CC53;
    Get_Phantoon(0)->phant_parameter_2 = 0;
    Phantoon_Func_5(k);
  }
}

void Phantoon_EyeFollowsSamusUntilTimerRunsOut(uint16 k) {  // 0xA7D60D
  Enemy_Phantoon *EK = Get_Phantoon(k);
  bool v2 = EK->phant_var_E == 1;
  bool v3 = (--EK->phant_var_E & 0x8000) != 0;
  if (v2 || v3) {
    Get_Phantoon(k + 128)->phant_var_B = 0;
    Enemy_Phantoon *E2 = Get_Phantoon(0x80);
    if (!E2->phant_var_A) {
      EK->phant_var_F = FUNC16(nullsub_237);
      Enemy_Phantoon *E0 = Get_Phantoon(0);
      E0->base.instruction_timer = 1;
      Enemy_Phantoon *E1 = Get_Phantoon(0x40);
      E1->base.instruction_timer = 1;
      E0->base.current_instruction = addr_kKraid_Ilist_CC41;
      E1->base.current_instruction = addr_kKraid_Ilist_CC81;
      E0->base.properties |= kEnemyProps_Tangible;
      E0->phant_parameter_2 = 1;
      return;
    }
    E2->phant_var_A = 0;
    EK->phant_var_E = 60;
    EK->phant_var_F = FUNC16(Phantoon_BecomesSolidAndBodyVuln);
  }
  Phantoon_ChangeEyeSpriteBasedOnSamusDist();
}

void Phantoon_BecomesSolidAndBodyVuln(uint16 v0) {  // 0xA7D65C
  Phantoon_ChangeEyeSpriteBasedOnSamusDist();
  phantom_related_layer_flag &= ~0x4000;
  Phantoon_BeginSwoopingPattern(v0);
  Enemy_Phantoon *E = Get_Phantoon(0);
  E->base.instruction_timer = 1;
  E->base.current_instruction = addr_kKraid_Ilist_CC47;
}

void Phantoon_IsSwooping(uint16 k) {  // 0xA7D678
  Phantoon_ChangeEyeSpriteBasedOnSamusDist();
  Phantoon_MoveInSwoopingPattern(k);
  Enemy_Phantoon *EK = Get_Phantoon(k);
  bool v2 = EK->phant_var_E == 1;
  bool v3 = (--EK->phant_var_E & 0x8000) != 0;
  if (v2 || v3) {
    EK->phant_var_F = FUNC16(Phantoon_FadeoutWithSwoop);
    phantom_related_layer_flag |= 0x4000;
    Enemy_Phantoon *E0 = Get_Phantoon(0);
    E0->base.instruction_timer = 1;
    Enemy_Phantoon *E1 = Get_Phantoon(0x40);
    E1->base.instruction_timer = 1;
    E0->base.current_instruction = addr_kKraid_Ilist_CC41;
    E1->base.current_instruction = addr_kKraid_Ilist_CC91;
    E0->base.properties |= kEnemyProps_Tangible;
    E1->phant_var_F = 0;
    Get_Phantoon(k + 128)->phant_var_B = 0;
  }
}

void Phantoon_FadeoutWithSwoop(uint16 k) {  // 0xA7D6B9
  Phantoon_MoveInSwoopingPattern(k);
  Phantoon_FadeOut(0xC);
  if (Get_Phantoon(0x40)->phant_var_F) {
    Enemy_Phantoon *E = Get_Phantoon(k);
    E->phant_var_F = FUNC16(Phantoon_WaitAfterFadeOut);
    E->phant_var_E = 120;
  }
}

void Phantoon_WaitAfterFadeOut(uint16 k) {  // 0xA7D6D4
  Enemy_Phantoon *E = Get_Phantoon(k);
  bool v2 = E->phant_var_E == 1;
  bool v3 = (--E->phant_var_E & 0x8000) != 0;
  if (v2 || v3)
    E->phant_var_F = FUNC16(Phantoon_MoveLeftOrRightAndPickEyeOpenPatt);
}

void Phantoon_MoveLeftOrRightAndPickEyeOpenPatt(uint16 k) {  // 0xA7D6E2
  Enemy_Phantoon *E = Get_Phantoon(0);
  if ((NextRandom() & 1) != 0) {
    E->phant_var_A = 136;
    E->base.x_pos = 208;
  } else {
    E->phant_var_A = 399;
    E->base.x_pos = 48;
  }
  E->base.y_pos = 96;
  Enemy_Phantoon *E1 = Get_Phantoon(0x40);
  E1->phant_var_C = 0;
  E->phant_var_C = 1;
  E->phant_var_B = 0;
  E->phant_parameter_2 = 0;
  Phantoon_PickPatternForRound2();
  Get_Phantoon(k)->phant_var_F = FUNC16(Phantoon_FadeInBeforeFigure8);
  E1->phant_var_F = 0;
}

void Phantoon_FadeInBeforeFigure8(uint16 k) {  // 0xA7D72D
  Phantoon_FadeIn(0xC);
  if (Get_Phantoon(0x40)->phant_var_F)
    Get_Phantoon(0)->phant_var_F = FUNC16(Phantoon_MovePhantoonInFigure8ThenOpenEye);
}

void Phantoon_BecomeSolidAfterRainingFireballs(uint16 k) {  // 0xA7D73F
  Enemy_Phantoon *E1 = Get_Phantoon(0x40);
  E1->phant_var_F = 0;
  phantom_related_layer_flag &= ~0x4000;
  Enemy_Phantoon *E = Get_Phantoon(0);
  E->base.instruction_timer = 1;
  E1->base.instruction_timer = 1;
  E->base.current_instruction = addr_kKraid_Ilist_CC47;
  E1->base.current_instruction = addr_kKraid_Ilist_CC9D;
  Get_Phantoon(k)->phant_var_F = FUNC16(Phantoon_FadeInDuringFireballRain);
}

void Phantoon_FadeInDuringFireballRain(uint16 k) {  // 0xA7D767
  Phantoon_FadeIn(1);
  if (Get_Phantoon(0x40)->phant_var_F) {
    Enemy_Phantoon *E = Get_Phantoon(0);
    E->base.properties &= ~kEnemyProps_Tangible;
    Get_Phantoon(k)->phant_var_F = FUNC16(Phantoon_FollowSamusWithEyeDuringFireballRain);
    E->phant_var_E = 90;
  }
}

void Phantoon_FollowSamusWithEyeDuringFireballRain(uint16 k) {  // 0xA7D788
  Enemy_Phantoon *EK = Get_Phantoon(k);
  bool v2 = EK->phant_var_E == 1;
  bool v3 = (--EK->phant_var_E & 0x8000) != 0;
  if (v2 || v3) {
    Get_Phantoon(k + 128)->phant_var_B = 0;
    Enemy_Phantoon *E2 = Get_Phantoon(0x80);
    if (E2->phant_var_A) {
      E2->phant_var_A = 0;
      Get_Phantoon(0)->phant_parameter_2 = 1;
      Phantoon_BeginSwoopingPattern(k);
    } else {
      EK->phant_var_F = FUNC16(Phantoon_FadeOutDuringFireballRain);
      Enemy_Phantoon *E1 = Get_Phantoon(0x40);
      E1->phant_var_F = 0;
      Enemy_Phantoon *E0 = Get_Phantoon(0);
      E0->base.instruction_timer = 1;
      E1->base.instruction_timer = 1;
      E0->base.current_instruction = addr_kKraid_Ilist_CC41;
      E1->base.current_instruction = addr_kKraid_Ilist_CC91;
      E0->base.properties |= kEnemyProps_Tangible;
      phantom_related_layer_flag |= 0x4000;
    }
  }
}

void Phantoon_FadeOutDuringFireballRain(uint16 k) {  // 0xA7D7D5
  Phantoon_FadeOut(0xC);
  if (Get_Phantoon(0x40)->phant_var_F) {
    Enemy_Phantoon *E = Get_Phantoon(k);
    E->phant_var_F = FUNC16(Phantoon_SpawnRainingFireballs);
    E->phant_var_E = g_word_A7CD63[NextRandom() & 7];
  }
}

void Phantoon_SpawnRainingFireballs(uint16 k) {  // 0xA7D7F7
  Enemy_Phantoon *EK = Get_Phantoon(k);
  bool v2 = EK->phant_var_E == 1;
  bool v3 = (--EK->phant_var_E & 0x8000) != 0;
  if (v2 || v3) {
    uint16 a = NextRandom() & 7;
    int v4 = (8 * a) >> 1;
    Enemy_Phantoon *E0 = Get_Phantoon(0);
    E0->phant_var_A = g_word_A7CDAD[v4];
    E0->base.x_pos = g_word_A7CDAD[v4 + 1];
    E0->base.y_pos = g_word_A7CDAD[v4 + 2];
    Get_Phantoon(0x40)->phant_var_C = 0;
    EK->phant_var_F = FUNC16(Phantoon_BecomeSolidAfterRainingFireballs);
    Phantoon_Func_6(k, a);
  }
}

void Phantoon_FadeOutBeforeFirstFireballRain(uint16 k) {  // 0xA7D82A
  Phantoon_FadeOut(0xC);
  Phantoon_AdjustSpeedAndMoveInFigure8();
  Phantoon_Func_7(k);
  Enemy_Phantoon *E = Get_Phantoon(0x40);
  bool v2 = E->phant_var_A == 1;
  bool v3 = (--E->phant_var_A & 0x8000) != 0;
  if (v2 || v3) {
    Get_Phantoon(0x80)->phant_var_A = 0;
    Get_Phantoon(k)->phant_var_F = FUNC16(Phantoon_BecomeSolidAfterRainingFireballs);
    if (sign16(Get_Phantoon(0)->base.x_pos - 128))
      Phantoon_Func_6(k, 0);
    else
      Phantoon_Func_6(k, 2);
  }
}

void Phantoon_FadeOutBeforeEnrage(uint16 k) {  // 0xA7D85C
  Phantoon_FadeOut(0xC);
  if (Get_Phantoon(0x40)->phant_var_F) {
    Enemy_Phantoon *E = Get_Phantoon(k);
    E->phant_var_F = FUNC16(Phantoon_MoveEnragedPhantoonToTopCenter);
    E->phant_var_E = 120;
  }
}

void Phantoon_MoveEnragedPhantoonToTopCenter(uint16 k) {  // 0xA7D874
  Enemy_Phantoon *E = Get_Phantoon(k);
  bool v2 = E->phant_var_E == 1;
  bool v3 = (--E->phant_var_E & 0x8000) != 0;
  if (v2 || v3) {
    E->phant_var_F = FUNC16(Phantoon_FadeInEnragedPhantoon);
    Enemy_Phantoon *E = Get_Phantoon(0);
    E->base.x_pos = 128;
    E->base.y_pos = 32;
    Get_Phantoon(0x40)->phant_var_F = 0;
  }
}

void Phantoon_FadeInEnragedPhantoon(uint16 k) {  // 0xA7D891
  Phantoon_FadeIn(0xC);
  Enemy_Phantoon *E = Get_Phantoon(0x40);
  if (E->phant_var_F) {
    Get_Phantoon(0)->phant_var_F = FUNC16(Phantoon_Enraged);
    Get_Phantoon(k)->phant_var_E = 4;
    E->phant_var_F = 0;
  }
}

void Phantoon_Enraged(uint16 k) {  // 0xA7D8AC
  int16 v5;
  int16 v8;

  Enemy_Phantoon *E = Get_Phantoon(k);
  bool v2 = E->phant_var_E == 1;
  bool v3 = (--E->phant_var_E & 0x8000) != 0;
  if (v2 || v3) {
    if ((Get_Phantoon(0x40)->phant_var_F & 1) != 0) {
      v5 = 15;
      do {
        v8 = v5;
        SpawnEnemyProjectileWithGfx(v5-- | 0x200, k, addr_kEproj_DestroyableFireballs);
      } while ((int16)(v8 - 9) >= 0);
    } else {
      for (int i = 6; i >= 0; --i)
        SpawnEnemyProjectileWithGfx(i | 0x200, k, addr_kEproj_DestroyableFireballs);
    }
    QueueSfx3_Max6(0x29);
    Enemy_Phantoon *E1 = Get_Phantoon(0x40);
    uint16 v7 = E1->phant_var_F + 1;
    E1->phant_var_F = v7;
    if (sign16(v7 - 8)) {
      Get_Phantoon(k)->phant_var_E = 128;
    } else {
      E1->base.instruction_timer = 1;
      E1->base.current_instruction = addr_kKraid_Ilist_CC91;
      E1->phant_var_F = 0;
      Get_Phantoon(k)->phant_var_F = FUNC16(Phantoon_FadeoutAfterEnrage);
    }
  }
}

void Phantoon_FadeoutAfterEnrage(uint16 k) {  // 0xA7D916
  Phantoon_FadeOut(0xC);
  if (Get_Phantoon(0x40)->phant_var_F) {
    Enemy_Phantoon *E = Get_Phantoon(k);
    E->phant_var_F = FUNC16(Phantoon_WaitAfterFadeOut);
    E->phant_var_E = 120;
  }
}

void Phantoon_CompleteSwoopAfterFatalShot(uint16 k) {  // 0xA7D92E
  Phantoon_ChangeEyeSpriteBasedOnSamusDist();
  Phantoon_MoveInSwoopingPattern(k);
  uint16 x_pos = Get_Phantoon(0)->base.x_pos;
  if (!sign16(x_pos - 96)) {
    if (sign16(x_pos - 160))
      Get_Phantoon(k)->phant_var_F = FUNC16(Phantoon_DyingPhantoonFadeInOut);
  }
}

void Phantoon_DyingPhantoonFadeInOut(uint16 k) {  // 0xA7D948
  Enemy_Phantoon *E1 = Get_Phantoon(0x40);
  if ((E1->phant_var_C & 1) != 0) {
    Phantoon_FadeIn(0xC);
    if (!E1->phant_var_F)
      return;
  } else {
    Phantoon_FadeOut(0xC);
    if (!E1->phant_var_F)
      return;
  }
  E1->phant_var_F = 0;
  uint16 v2 = E1->phant_var_C + 1;
  E1->phant_var_C = v2;
  if (!sign16(v2 - 10)) {
    Enemy_Phantoon *E = Get_Phantoon(k);
    E->phant_var_F = FUNC16(Phantoon_DyingPhantoonExplosions);
    E->phant_var_E = 15;
    Get_Phantoon(0x80)->phant_var_F = 0;
    Get_Phantoon(0)->phant_var_A = 0;
  }
}

void Phantoon_DyingPhantoonExplosions(uint16 k) {  // 0xA7D98B
  Enemy_Phantoon *EK = Get_Phantoon(k);
  bool v2 = EK->phant_var_E == 1;
  bool v3 = (--EK->phant_var_E & 0x8000) != 0;
  if (v2 || v3) {
    Enemy_Phantoon *E2 = Get_Phantoon(0x80);
    uint16 v5 = 4 * E2->phant_var_F;
    Enemy_Phantoon *E0 = Get_Phantoon(0);
    eproj_spawn_pt = (Point16U){ (int8)g_byte_A7DA1D[v5] + E0->base.x_pos, (int8)g_byte_A7DA1D[v5 + 1] + E0->base.y_pos };
    uint16 v11 = g_byte_A7DA1D[v5 + 2];
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, v11);
    if (v11 == 29)
      QueueSfx2_Max6(0x24);
    else
      QueueSfx2_Max6(0x2B);
    EK->phant_var_E = g_byte_A7DA1D[v5 + 3];
    uint16 v9 = E2->phant_var_F + 1;
    E2->phant_var_F = v9;
    if (!sign16(v9 - 13)) {
      E2->phant_var_F = 5;
      uint16 v10 = E0->phant_var_A + 1;
      E0->phant_var_A = v10;
      if (!sign16(v10 - 3))
        EK->phant_var_F = FUNC16(Phantoon_WavyDyingPhantoonAndCry);
    }
  }
}

void Phantoon_WavyDyingPhantoonAndCry(uint16 k) {  // 0xA7DA51
  Enemy_Phantoon *Phantoon; // r10

  sub_88E487(1, g_word_A7CDA3);
  Get_Phantoon(0xC0)->phant_var_D = 0;
  Get_Phantoon(k)->phant_var_F = FUNC16(Phantoon_DyingFadeOut);
  Phantoon = Get_Phantoon(0x40);
  Phantoon->phant_var_C = 2;
  uint16 v2 = Get_Phantoon(0)->base.properties & ~(kEnemyProps_DisableSamusColl | kEnemyProps_Tangible | kEnemyProps_Invisible) | kEnemyProps_Tangible | kEnemyProps_Invisible;
  Phantoon->base.properties = v2;
  Get_Phantoon(0x80)->base.properties = v2;
  Get_Phantoon(0xC0)->base.properties = v2;
  QueueSfx2_Max6(0x7E);
}

void Phantoon_DyingFadeOut(uint16 k) {  // 0xA7DA86
  Phantoon_Func_3(__PAIR32__(g_word_A7CDA1, g_word_A7CD9F));
  Enemy_Phantoon *E = Get_Phantoon(0x40);
  if (E->phant_var_C == 0xFFFF) {
    Phantoon_FadeOut(0xC);
    if (E->phant_var_F)
      Get_Phantoon(k)->phant_var_F = FUNC16(Phantoon_AlmostDead);
  } else if ((nmi_frame_counter_word & 0xF) == 0) {
    if (LOBYTE(E->phant_var_C) == 0xF2) {
      E->phant_var_C = -1;
      E->phant_var_F = 0;
    } else {
      uint8 v2 = LOBYTE(E->phant_var_C) + 16;
      LOBYTE(E->phant_var_C) = v2;
      reg_MOSAIC = v2;
    }
  }
}

void Phantoon_AlmostDead(uint16 k) {  // 0xA7DAD7
  reg_MOSAIC = 0;
  Enemy_Phantoon *E1 = Get_Phantoon(0x40);
  E1->phant_parameter_1 = 0;
  phantom_related_layer_flag &= ~0x4000;
  Get_Phantoon(0xC0)->phant_parameter_1 = -1;
  Enemy_Phantoon *EK = Get_Phantoon(k);
  EK->phant_var_F = FUNC16(Phantoon_Dead);
  EK->phant_var_E = 60;
  E1->phant_var_F = 0;
  Enemy_Phantoon *E0 = Get_Phantoon(0);
  E0->base.x_pos = 384;
  E0->base.y_pos = 128;
  for (int i = 1022; i >= 0; i -= 2)
    tilemap_stuff[i >> 1] = 824;
  uint16 v5 = vram_write_queue_tail;
  gVramWriteEntry(vram_write_queue_tail)->size = 1024;
  v5 += 2;
  gVramWriteEntry(v5)->size = 0x2000;
  v5 += 2;
  LOBYTE(gVramWriteEntry(v5++)->size) = 126;
  gVramWriteEntry(v5)->size = 18432;
  vram_write_queue_tail = v5 + 2;
}

void Phantoon_Dead(uint16 k) {  // 0xA7DB3D
  Enemy_Phantoon *EK = Get_Phantoon(k);
  if (EK->phant_var_E) {
    --EK->phant_var_E;
  } else if ((nmi_frame_counter_word & 3) == 0) {
    Enemy_Phantoon *E1 = Get_Phantoon(0x40);
    E1->phant_var_D = 12;
    if (Phantoon_Func_9() & 1) {
      reg_TM |= 2;
      Enemy_ItemDrop_Phantoon(k);
      Enemy_Phantoon *E0 = Get_Phantoon(0);
      uint16 v4 = E0->base.properties | kEnemyProps_Deleted;
      E0->base.properties = v4;
      E1->base.properties = v4;
      Get_Phantoon(0x80)->base.properties = v4;
      Get_Phantoon(0xC0)->base.properties = v4;
      *(uint16 *)&boss_bits_for_area[area_index] |= 1;
      SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x00, 0x06, 0xb78b });
      QueueMusic_Delayed8(3);
    }
  }
}

uint8 Phantoon_Func_8(void) {  // 0xA7DB9A
  Enemy_Phantoon *E = Get_Phantoon(0x40);
  if ((uint16)(E->phant_var_D + 1) >= E->phant_var_E) {
    for (int i = 0; i < 0x20; i += 2) {
      palette_buffer[(i >> 1) + 112] =
        Phantoon_Func_10_CalculateNthTransitionColorFromXtoY(E->phant_var_E,
          palette_buffer[(i >> 1) + 112],
          g_word_A7CA41[i >> 1]);
    }
    ++E->phant_var_E;
    return 0;
  } else {
    E->phant_var_E = 0;
    return 1;
  }
}

uint8 Phantoon_SetColorBasedOnHp(void) {  // 0xA7DBD5
  PairU16 Entry;

  Enemy_Phantoon *E = Get_Phantoon(0x40);
  if ((uint16)(E->phant_var_D + 1) >= E->phant_var_E) {
    uint16 v2 = 0, v7;
    do {
      v7 = v2;
      Entry = Phantoon_SetColorBasedOnHp_FindEntry(v2);
      uint16 j = Entry.j;
      uint16 v5 = palette_buffer[(Entry.k >> 1) + 112];
      palette_buffer[(v7 >> 1) + 112] = Phantoon_Func_10_CalculateNthTransitionColorFromXtoY(E->phant_var_E, v5, j);
      v2 = v7 + 2;
    } while ((uint16)(v7 + 2) < 0x20);
    ++E->phant_var_E;
    return 0;
  } else {
    E->phant_var_E = 0;
    return 1;
  }
}

PairU16 Phantoon_SetColorBasedOnHp_FindEntry(uint16 k) {  // 0xA7DC0F
  uint16 r24 = k;
  uint16 r18 = 312, r20 = 312;
  uint16 r22 = 0;
  do {
    if ((int16)(r20 - Get_Phantoon(cur_enemy_index)->base.health) >= 0)
      break;
    r20 += r18;
    ++r22;
  } while (sign16(r22 - 7));
  const uint16 *v1 = (const uint16 *)RomPtr_A7(r24 + g_off_A7DC4A[r22]);
  return MakePairU16(r24, *v1);
}

uint8 Phantoon_Func_9(void) {  // 0xA7DC5A
  Enemy_Phantoon *E = Get_Phantoon(0x40);
  if ((uint16)(E->phant_var_D + 1) >= E->phant_var_E) {
    for (int i = 0; i < 0xE0; i += 2) {
      palette_buffer[i >> 1] = Phantoon_Func_10_CalculateNthTransitionColorFromXtoY(
        E->phant_var_E,
        palette_buffer[i >> 1],
        g_word_A7CA61[i >> 1]);
    }
    ++E->phant_var_E;
    return 0;
  } else {
    E->phant_var_E = 0;
    return 1;
  }
}

uint16 Phantoon_Func_10_CalculateNthTransitionColorFromXtoY(uint16 a, uint16 k, uint16 j) {  // 0xA7DC95
  return Phantoon_CalculateNthTransitionColorComponentFromXtoY(a, k & 0x1F, j & 0x1F) + 
         (Phantoon_CalculateNthTransitionColorComponentFromXtoY(a, (k >> 5) & 0x1F, (j >> 5) & 0x1F) << 5) +
         (Phantoon_CalculateNthTransitionColorComponentFromXtoY(a, (k >> 10) & 0x1F, (j >> 10) & 0x1F) << 10);
}

uint16 Phantoon_CalculateNthTransitionColorComponentFromXtoY(uint16 a, uint16 k, uint16 j) {  // 0xA7DCF1
  if (!a)
    return k;
  uint16 v4 = a - 1;
  Enemy_Phantoon *E = Get_Phantoon(0x40);
  if (v4 == E->phant_var_D)
    return j;
  uint16 r20 = v4 + 1;
  uint16 r18 = j - k;
  uint8 v6 = abs16(r18);
  uint16 RegWord = SnesDivide(v6 << 8, LOBYTE(E->phant_var_D) - r20 + 1);
  r18 = sign16(r18) ? -RegWord : RegWord;
  return (uint16)(r18 + swap16(k)) >> 8;
}

void Phantoon_Hurt(void) {  // 0xA7DD3F
  PairU16 Entry;

  Enemy_Phantoon *E0 = Get_Phantoon(0);
  if (E0->base.flash_timer == 8) {
LABEL_4:;
    uint16 v1 = 30;
    do {
      Entry = Phantoon_SetColorBasedOnHp_FindEntry(v1);
      palette_buffer[(Entry.k >> 1) + 112] = Entry.j;
      v1 = Entry.k - 2;
    } while ((int16)(Entry.k - 2) >= 0);
    Enemy_Phantoon *E3 = Get_Phantoon(0x80);
    E3->phant_parameter_2 = LOBYTE(E3->phant_parameter_2);
    return;
  }
  if ((E0->base.frame_counter & 2) == 0) {
    if (!HIBYTE(Get_Phantoon(0x80)->phant_parameter_2))
      return;
    goto LABEL_4;
  }
  if (!HIBYTE(Get_Phantoon(0x80)->phant_parameter_2)) {
    for (int i = 30; i >= 0; i -= 2)
      palette_buffer[(i >> 1) + 112] = 0x7FFF;
    Enemy_Phantoon *E3 = Get_Phantoon(0x80);
    E3->phant_parameter_2 |= 0x100;
  }
}

void Phantoon_Touch(void) {  // 0xA7DD95
  NormalEnemyTouchAiSkipDeathAnim_CurEnemy();
}

void Phantoon_Shot(void) {  // 0xA7DD9B

  Enemy_Phantoon *E0 = Get_Phantoon(0);
  if (!sign16(E0->phant_var_F + 0x26B8))
    return;
  uint16 v1 = cur_enemy_index;
  Enemy_Phantoon *EK = Get_Phantoon(cur_enemy_index);
  uint16 health = EK->base.health;
  NormalEnemyShotAiSkipDeathAnim_CurEnemy();
  if (!EK->base.health) {
    QueueSfx2_Max6(0x73);
    Get_Phantoon(0x80)->phant_parameter_2 = 1;
    E0->base.properties |= kEnemyProps_Tangible;
    Phantoon_StartDeathSequence(v1);
    return;
  }
  if ((EK->base.ai_handler_bits & 2) != 0) {
    QueueSfx2_Max6(0x73);
    uint16 phanto_var_F = EK->phant_var_F;
    if (phanto_var_F != FUNC16(Phantoon_EyeFollowsSamusUntilTimerRunsOut)
        && phanto_var_F != FUNC16(Phantoon_FollowSamusWithEyeDuringFireballRain)) {
      if (phanto_var_F != FUNC16(Phantoon_IsSwooping)) {
LABEL_20:
        Get_Phantoon(0x80)->phant_parameter_2 = 2;
        return;
      }
      health -= EK->base.health;
      if (sign16(health - 300)
          || (projectile_type[collision_detection_index] & 0xF00) != 512) {
        Enemy_Phantoon *E2 = Get_Phantoon(v1 + 0x80);
        uint16 v7 = health + E2->phant_var_B;
        E2->phant_var_B = v7;
        if (!sign16(v7 - 300))
          EK->phant_var_E = 1;
        goto LABEL_20;
      }
      goto LABEL_23;
    }
    health -= EK->base.health;
    if (!sign16(health - 300) && (projectile_type[collision_detection_index] & 0xF00) == 512) {
LABEL_23:
      EK->phant_var_F = FUNC16(Phantoon_FadeOutBeforeEnrage);
      goto LABEL_22;
    }
    Enemy_Phantoon *E2;
    E2 = Get_Phantoon(v1 + 0x80);
    uint16 v9;
    v9 = health + E2->phant_var_B;
    E2->phant_var_B = v9;
    if (!sign16(v9 - 300)) {
      EK->phant_var_F = FUNC16(Phantoon_FadeoutWithSwoop);
LABEL_22:
      EK->phant_var_E = 0;
      Get_Phantoon(0x80)->phant_var_A = 0;
      Get_Phantoon(v1 + 0x80)->phant_var_B = 0;
      phantom_related_layer_flag |= 0x4000;
      E0->base.instruction_timer = 1;
      Enemy_Phantoon *E1 = Get_Phantoon(0x40);
      E1->base.instruction_timer = 1;
      E0->base.current_instruction = addr_kKraid_Ilist_CC41;
      E1->base.current_instruction = addr_kKraid_Ilist_CC91;
      E0->base.properties |= kEnemyProps_Tangible;
      E1->phant_var_F = 0;
      goto LABEL_20;
    }
    uint16 v10 = NextRandom() & 7;
    Get_Phantoon(0x40)->phant_var_B = *(g_byte_A7CDA5 + v10);
    Get_Phantoon(0xC0)->phant_parameter_2 = v10;
    Enemy_Phantoon *E2x = Get_Phantoon(0x80);
    E2x->phant_parameter_2 = 1;
    if (!E2x->phant_var_A) {
      E2x->phant_var_A = 1;
      if (!sign16(EK->phant_var_E - 16))
        EK->phant_var_E = 16;
    }
  }
}

void Etecoon_Init(void) {  // 0xA7E912
  Enemy_Etecoon *E = Get_Etecoon(cur_enemy_index);
  E->base.properties |= kEnemyProps_DisableSamusColl;
  E->base.spritemap_pointer = addr_kSpritemap_Nothing_A7;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->base.current_instruction = addr_kEtecoon_Ilist_E8CE;
  E->etecoon_var_F = FUNC16(Etecoon_Func_4);
  E->etecoon_var_E = -1;
}

void Etecoon_Main(void) {  // 0xA7E940
  Enemy_Etecoon *E = Get_Etecoon(cur_enemy_index);
  if (HIBYTE(E->etecoon_parameter_2))
    E->etecoon_parameter_2 -= 256;
  else
    CallEnemyPreInstr(E->etecoon_var_F | 0xA70000);
}

void Etecoon_Func_1(uint16 k) {  // 0xA7E958
  if (earthquake_timer) {
    Enemy_Etecoon *E = Get_Etecoon(k);
    E->etecoon_parameter_2 = LOBYTE(E->etecoon_parameter_2) | 0x8000;
    E->base.instruction_timer += 128;
  }
}

uint8 Etecoon_Func_2(uint16 k) {  // 0xA7E974
  Enemy_Etecoon *E = Get_Etecoon(k);
  return Enemy_MoveRight_IgnoreSlopes(k, __PAIR32__(E->etecoon_var_C, E->etecoon_var_D));
}

uint8 Etecoon_Func_3(uint16 k) {  // 0xA7E983
  Enemy_Etecoon *E = Get_Etecoon(k);
  int32 amt = __PAIR32__(E->etecoon_var_A, E->etecoon_var_B);
  if (sign16(E->etecoon_var_A - 5))
    AddToHiLo(&E->etecoon_var_A, &E->etecoon_var_B, __PAIR32__(samus_y_accel, samus_y_subaccel));
  return Enemy_MoveDown(k, amt);
}

void Etecoon_Func_4(uint16 k) {  // 0xA7E9AF
  if (!door_transition_flag_enemies) {
    Enemy_Etecoon *E = Get_Etecoon(k);
    if ((E->etecoon_var_E & 0x8000) == 0) {
      bool v2 = E->etecoon_var_E == 1;
      bool v3 = (--E->etecoon_var_E & 0x8000) != 0;
      if (v2 || v3) {
        E->base.current_instruction = addr_kEtecoon_Ilist_E854;
        E->etecoon_var_F = FUNC16(Etecoon_Func_5);
        E->etecoon_var_E = 11;
      }
    } else if (IsSamusWithinEnemy_Y(k, 0x80)) {
      if ((E->etecoon_parameter_2 & 3) == 0)
        QueueSfx2_Max15(0x35);
      E->base.instruction_timer = 1;
      E->base.current_instruction = addr_kEtecoon_Ilist_E8D6;
      E->etecoon_var_E = 256;
    }
  }
}

void Etecoon_Func_5(uint16 k) {  // 0xA7EA00
  Enemy_Etecoon *E = Get_Etecoon(k);
  bool v2 = E->etecoon_var_E == 1;
  bool v3 = (--E->etecoon_var_E & 0x8000) != 0;
  if (v2 || v3) {
    E->etecoon_var_A = g_word_A7E900;
    E->etecoon_var_B = g_word_A7E902;
    E->base.current_instruction += 2;
    E->base.instruction_timer = 1;
    E->etecoon_var_F = FUNC16(Etecoon_Func_6);
    if (!sign16(samus_x_pos - 256))
      QueueSfx2_Max6(0x33);
  }
}

void Etecoon_Func_6(uint16 k) {  // 0xA7EA37
  if (Etecoon_Func_3(k) & 1) {
    Enemy_Etecoon *E = Get_Etecoon(k);
    if ((E->etecoon_var_A & 0x8000) == 0) {
      if (IsSamusWithinEnemy_Y(k, 0x40) && IsSamusWithinEnemy_X(k, g_word_A7E910)) {
        uint16 v2 = DetermineDirectionOfSamusFromEnemy();
        if (sign16(v2 - 5)) {
          E->base.current_instruction = addr_kEtecoon_Ilist_E81E;
          E->etecoon_parameter_1 = 0;
        } else {
          E->base.current_instruction = addr_kEtecoon_Ilist_E876;
          E->etecoon_parameter_1 = 1;
        }
        E->etecoon_var_E = 32;
        E->base.instruction_timer = 1;
        E->etecoon_var_F = FUNC16(Etecoon_Func_7);
      } else {
        E->etecoon_var_E = 11;
        E->etecoon_var_F = FUNC16(Etecoon_Func_5);
        E->base.instruction_timer = 1;
        E->base.current_instruction = addr_kEtecoon_Ilist_E854;
      }
    } else {
      E->etecoon_var_A = 0;
      E->etecoon_var_B = 0;
      E->base.instruction_timer = 3;
      E->base.current_instruction = addr_stru_A7E862;
    }
  }
}

void Etecoon_Func_7(uint16 k) {  // 0xA7EAB5
  Enemy_Etecoon *E = Get_Etecoon(k);
  bool v2 = E->etecoon_var_E == 1;
  bool v3 = (--E->etecoon_var_E & 0x8000) != 0;
  if (v2 || v3) {
    E->base.current_instruction += 2;
    E->base.instruction_timer = 1;
    if (E->etecoon_parameter_1) {
      E->etecoon_var_C = g_word_A7E908;
      E->etecoon_var_D = g_word_A7E90A;
      E->etecoon_var_F = FUNC16(Etecoon_Func_9);
    } else {
      E->etecoon_var_C = g_word_A7E90C;
      E->etecoon_var_D = g_word_A7E90E;
      E->etecoon_var_F = FUNC16(Etecoon_Func_8);
    }
    E->etecoon_var_A = g_word_A7E900;
    E->etecoon_var_B = g_word_A7E902;
  }
}

void Etecoon_Func_8(uint16 k) {  // 0xA7EB02
  if (Etecoon_Func_2(k) & 1) {
    Enemy_Etecoon *E = Get_Etecoon(k);
    E->etecoon_var_C = g_word_A7E908;
    E->etecoon_var_D = g_word_A7E90A;
    E->etecoon_var_F = FUNC16(Etecoon_Func_9);
    E->base.instruction_timer = 1;
    E->base.current_instruction = addr_stru_A7E880;
    E->etecoon_parameter_1 = 1;
  }
}

void Etecoon_Func_9(uint16 k) {  // 0xA7EB2C
  if (EnemyFunc_BBBF(k, __PAIR32__(32, 0))) {
    Enemy_Etecoon *E = Get_Etecoon(k);
    E->base.instruction_timer = 1;
    E->base.current_instruction = addr_stru_A7E898;
    E->etecoon_var_F = FUNC16(Etecoon_Func_10);
  } else {
    Etecoon_Func_2(k);
  }
}

void Etecoon_Func_10(uint16 k) {  // 0xA7EB50
  Etecoon_Func_1(k);
  if (Etecoon_Func_2(k) & 1) {
    Enemy_Etecoon *E = Get_Etecoon(k);
    if (E->etecoon_parameter_1) {
      E->base.current_instruction = addr_kEtecoon_Ilist_E870;
      E->etecoon_parameter_1 = 0;
    } else {
      E->base.current_instruction = addr_kEtecoon_Ilist_E8C8;
      E->etecoon_parameter_1 = 1;
    }
    E->base.instruction_timer = 1;
    E->etecoon_var_F = FUNC16(Etecoon_Func_11);
    E->etecoon_var_E = 8;
    if (!sign16(samus_x_pos - 256))
      QueueSfx2_Max6(0x32);
  } else if (Etecoon_Func_3(k) & 1) {
    Enemy_Etecoon *E = Get_Etecoon(k);
    if (E->etecoon_parameter_1)
      E->base.current_instruction = addr_kEtecoon_Ilist_E854;
    else
      E->base.current_instruction = addr_kEtecoon_Ilist_E8AC;
    E->base.instruction_timer = 1;
    E->etecoon_var_E = 11;
    E->etecoon_var_F = FUNC16(Etecoon_Func_12);
    E->etecoon_var_A = g_word_A7E900;
    E->etecoon_var_B = g_word_A7E902;
  }
}

void Etecoon_Func_11(uint16 k) {  // 0xA7EBCD
  Etecoon_Func_1(k);
  Enemy_Etecoon *E = Get_Etecoon(k);
  bool v2 = E->etecoon_var_E == 1;
  bool v3 = (--E->etecoon_var_E & 0x8000) != 0;
  if (v2 || v3) {
    if (E->etecoon_parameter_1) {
      E->base.current_instruction = addr_kEtecoon_Ilist_E894;
      E->etecoon_var_C = g_word_A7E908;
      E->etecoon_var_D = g_word_A7E90A;
    } else {
      E->base.current_instruction = addr_kEtecoon_Ilist_E83C;
      E->etecoon_var_C = g_word_A7E90C;
      E->etecoon_var_D = g_word_A7E90E;
    }
    E->base.instruction_timer = 1;
    E->etecoon_var_F = FUNC16(Etecoon_Func_10);
    E->etecoon_var_A = g_word_A7E900;
    E->etecoon_var_B = g_word_A7E902;
  }
}

static Func_Y_V *const funcs_A2A79[3] = { Etecoon_Func_13, Etecoon_Func_14, Etecoon_Func_15 };

void Etecoon_Func_12(uint16 k) {  // 0xA7EC1B
  Enemy_Etecoon *E = Get_Etecoon(k);
  bool v2 = E->etecoon_var_E == 1;
  bool v3 = (--E->etecoon_var_E & 0x8000) != 0;
  if (v2 || v3) {
    E->etecoon_var_A = g_word_A7E900;
    E->etecoon_var_B = g_word_A7E902;
    funcs_A2A79[LOBYTE(E->etecoon_parameter_2)](k);
    Get_Etecoon(k)->base.instruction_timer = 1;
  }
}

void Etecoon_Func_13(uint16 j) {  // 0xA7EC47
  Enemy_Etecoon *E = Get_Etecoon(j);
  E->etecoon_var_F = FUNC16(Etecoon_Func_16);
  E->base.current_instruction = addr_stru_A7E828;
  E->etecoon_var_C = g_word_A7E90C;
  E->etecoon_var_D = g_word_A7E90E;
}

void Etecoon_Func_14(uint16 j) {  // 0xA7EC61
  Enemy_Etecoon *E = Get_Etecoon(j);
  E->etecoon_var_F = FUNC16(Etecoon_Func_17);
  E->base.current_instruction = addr_stru_A7E880;
  E->etecoon_var_C = g_word_A7E908;
  E->etecoon_var_D = g_word_A7E90A;
}

void Etecoon_Func_15(uint16 j) {  // 0xA7EC7B
  Enemy_Etecoon *E = Get_Etecoon(j);
  E->etecoon_var_F = FUNC16(Etecoon_Func_22);
  E->base.current_instruction += 2;
  E->etecoon_var_C = g_word_A7E908;
  E->etecoon_var_D = g_word_A7E90A;
}

void Etecoon_Func_16(uint16 k) {  // 0xA7EC97
  Etecoon_Func_2(k);
  Enemy_Etecoon *E = Get_Etecoon(k);
  if (sign16(E->base.x_pos - 537)) {
    E->etecoon_var_E = 11;
    E->etecoon_var_F = FUNC16(Etecoon_Func_23);
    E->base.instruction_timer = 1;
    E->base.current_instruction = addr_kEtecoon_Ilist_E854;
  }
}

void Etecoon_Func_17(uint16 k) {  // 0xA7ECBB
  Etecoon_Func_2(k);
  Enemy_Etecoon *E = Get_Etecoon(k);
  if (!sign16(E->base.x_pos - 600)) {
    E->etecoon_var_E = 11;
    E->etecoon_var_F = FUNC16(Etecoon_Func_23);
    E->base.instruction_timer = 1;
    E->base.current_instruction = addr_kEtecoon_Ilist_E854;
  }
}

void Etecoon_Func_18(uint16 k) {  // 0xA7ECDF
  Etecoon_Func_2(k);
  Enemy_Etecoon *E = Get_Etecoon(k);
  if (!sign16(E->base.x_pos - 600)) {
    E->etecoon_var_F = FUNC16(Etecoon_Func_19);
    E->etecoon_var_A = g_word_A7E904;
    E->etecoon_var_B = g_word_A7E906;
    E->base.instruction_timer = 1;
    E->base.current_instruction = addr_stru_A7E898;
  }
}

void Etecoon_Func_19(uint16 k) {  // 0xA7ED09
  Etecoon_Func_2(k);
  Etecoon_Func_3(k);
  Enemy_Etecoon *E = Get_Etecoon(k);
  if (!sign16(E->base.x_pos - 680)) {
    E->base.instruction_timer = 1;
    E->base.current_instruction = addr_stru_A7E880;
    E->etecoon_var_F = FUNC16(Etecoon_Func_20);
  }
}

void Etecoon_Func_20(uint16 k) {  // 0xA7ED2A
  Etecoon_Func_2(k);
  Enemy_Etecoon *E = Get_Etecoon(k);
  if (!sign16(E->base.x_pos - 840)) {
    E->base.instruction_timer = 1;
    E->base.current_instruction = addr_stru_A7E898;
    E->etecoon_var_F = FUNC16(Etecoon_Func_21);
    E->etecoon_var_A = -1;
    E->etecoon_var_B = g_word_A7E906;
  }
}

void Etecoon_Func_21(uint16 k) {  // 0xA7ED54
  Etecoon_Func_2(k);
  if (Etecoon_Func_3(k) & 1) {
    Enemy_Etecoon *E = Get_Etecoon(k);
    E->etecoon_var_E = 11;
    E->base.instruction_timer = 1;
    E->base.current_instruction = addr_kEtecoon_Ilist_E854;
    E->etecoon_var_F = FUNC16(Etecoon_Func_23);
  }
}

void Etecoon_Func_22(uint16 k) {  // 0xA7ED75
  Etecoon_Func_1(k);
  if (Etecoon_Func_3(k) & 1) {
    Enemy_Etecoon *E = Get_Etecoon(k);
    if ((E->etecoon_var_A & 0x8000) == 0) {
      E->etecoon_var_E = 11;
      E->base.instruction_timer = 1;
      E->base.current_instruction = addr_kEtecoon_Ilist_E854;
      if ((E->etecoon_parameter_2 & 2) != 0 && (sign16(E->base.x_pos - 832)))
        E->etecoon_var_F = FUNC16(Etecoon_Func_24);
      else
        E->etecoon_var_F = FUNC16(Etecoon_Func_23);
    } else {
      E->etecoon_var_A = 0;
      E->etecoon_var_B = 0;
      E->base.instruction_timer = 3;
      E->base.current_instruction = addr_stru_A7E862;
    }
  }
}

void Etecoon_Func_23(uint16 k) {  // 0xA7EDC7
  Etecoon_Func_1(k);
  Enemy_Etecoon *E = Get_Etecoon(k);
  bool v2 = E->etecoon_var_E == 1;
  bool v3 = (--E->etecoon_var_E & 0x8000) != 0;
  if (v2 || v3) {
    uint16 v4 = E->etecoon_parameter_1 + 256;
    E->etecoon_parameter_1 = v4;
    if (sign16((v4 & 0xFF00) - 1024)
        || (E->etecoon_parameter_1 = LOBYTE(E->etecoon_parameter_1), (E->etecoon_parameter_2 & 2) != 0)) {
      E->etecoon_var_F = FUNC16(Etecoon_Func_22);
      E->base.current_instruction += 2;
    } else {
      E->etecoon_var_F = FUNC16(Etecoon_Func_25);
      E->base.current_instruction = addr_stru_A7E880;
      E->etecoon_var_C = g_word_A7E908;
      E->etecoon_var_D = g_word_A7E90A;
    }
    E->etecoon_var_A = g_word_A7E900;
    E->etecoon_var_B = g_word_A7E902;
    E->base.instruction_timer = 1;
    if (!sign16(samus_x_pos - 256))
      QueueSfx2_Max6(0x33);
  }
}

void Etecoon_Func_24(uint16 k) {  // 0xA7EE3E
  Etecoon_Func_1(k);
  Enemy_Etecoon *E = Get_Etecoon(k);
  bool v2 = E->etecoon_var_E == 1;
  bool v3 = (--E->etecoon_var_E & 0x8000) != 0;
  if (v2 || v3) {
    if (IsSamusWithinEnemy_Y(k, 0x40) && IsSamusWithinEnemy_X(k, 0x30)) {
      E->base.current_instruction = addr_stru_A7E880;
      E->etecoon_var_F = FUNC16(Etecoon_Func_18);
    } else {
      E->etecoon_var_A = g_word_A7E900;
      E->etecoon_var_B = g_word_A7E902;
      E->base.current_instruction += 2;
      E->etecoon_var_F = FUNC16(Etecoon_Func_22);
      if (!sign16(samus_x_pos - 256))
        QueueSfx2_Max6(0x33);
    }
    E->base.instruction_timer = 1;
  }
}

void Etecoon_Func_25(uint16 k) {  // 0xA7EE9A
  Etecoon_Func_2(k);
  Enemy_Etecoon *E = Get_Etecoon(k);
  if (!sign16(E->base.x_pos - 600)) {
    E->etecoon_var_F = FUNC16(Etecoon_Func_26);
    E->base.instruction_timer = 1;
    E->base.current_instruction = addr_stru_A7E898;
  }
}

void Etecoon_Func_26(uint16 k) {  // 0xA7EEB8
  Etecoon_Func_2(k);
  if (Etecoon_Func_3(k) & 1) {
    Enemy_Etecoon *E = Get_Etecoon(k);
    E->etecoon_var_C = g_word_A7E90C;
    E->etecoon_var_D = g_word_A7E90E;
    E->etecoon_var_F = FUNC16(Etecoon_Func_8);
    E->etecoon_var_A = g_word_A7E900;
    E->etecoon_var_B = g_word_A7E902;
    E->base.instruction_timer = 1;
    E->base.current_instruction = addr_stru_A7E828;
  }
}

void Dachora_Init(void) {  // 0xA7F4DD
  int16 dachor_parameter_1;

  Enemy_Dachora *E = Get_Dachora(cur_enemy_index);
  E->base.properties |= kEnemyProps_DisableSamusColl;
  E->base.spritemap_pointer = addr_kSpritemap_Nothing_A7;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  dachor_parameter_1 = E->dachor_parameter_1;
  if (dachor_parameter_1 < 0) {
    if ((dachor_parameter_1 & 1) != 0)
      E->base.current_instruction = addr_kDachora_Ilist_F4B9;
    else
      E->base.current_instruction = addr_kDachora_Ilist_F3F7;
    E->dachor_var_F = FUNC16(Dachora_Func_12);
  } else {
    if (dachor_parameter_1)
      E->base.current_instruction = addr_kDachora_Ilist_F45B;
    else
      E->base.current_instruction = addr_kDachora_Ilist_F399;
    E->dachor_var_F = FUNC16(Dachora_Func_2);
  }
}

void Dachora_Main(void) {  // 0xA7F52E
  Enemy_Dachora *E = Get_Dachora(cur_enemy_index);
  CallEnemyPreInstr(E->dachor_var_F | 0xA70000);
}
void Dachora_Func_1(uint16 j, uint16 k) {  // 0xA7F535
  Enemy_Dachora *E = Get_Dachora(k);
  uint16 v3 = swap16(E->base.palette_index);
  memcpy(g_ram + g_off_A7F55F[v3 >> 1], RomPtr_A7(j), 32);
}

void Dachora_Func_2(uint16 k) {  // 0xA7F570
  Enemy_MoveDown(k, __PAIR32__(1, 0));
  if (IsSamusWithinEnemy_Y(k, 0x40) && IsSamusWithinEnemy_X(k, g_word_A7F4C9)) {
    Enemy_Dachora *E = Get_Dachora(k);
    if (E->dachor_parameter_1)
      E->base.current_instruction = addr_kDachora_Ilist_F48B;
    else
      E->base.current_instruction = addr_kDachora_Ilist_F3C9;
    E->base.instruction_timer = 1;
    E->dachor_var_F = FUNC16(Dachora_Func_3);
    E->dachor_var_A = g_word_A7F4CD;
    QueueSfx2_Max15(0x1D);
  }
}

void Dachora_Func_3(uint16 k) {  // 0xA7F5BC
  Enemy_Dachora *E = Get_Dachora(k);
  if (E->dachor_var_A-- == 1) {
    if (E->dachor_parameter_1) {
      E->base.current_instruction = addr_kDachora_Ilist_F407;
      E->dachor_var_F = FUNC16(Dachora_Func_5);
    } else {
      E->base.current_instruction = addr_kDachora_Ilist_F345;
      E->dachor_var_F = FUNC16(Dachora_Func_4);
    }
    E->base.instruction_timer = 1;
    E->dachor_var_E = 1;
  }
}

void Dachora_Func_4(uint16 k) {  // 0xA7F5ED
  int32 amt = -Dachora_Func_6(k);
  Enemy_Dachora *E = Get_Dachora(k);
  if (Enemy_MoveRight_IgnoreSlopes(k, amt) || (EnemyFunc_C8AD(k), sign16(E->base.x_pos - 96))) {
    E->base.current_instruction = addr_kDachora_Ilist_F407;
    E->dachor_var_F = FUNC16(Dachora_Func_5);
    E->dachor_var_E = 1;
    E->dachor_parameter_1 = 1;
    E->base.instruction_timer = 1;
    E->dachor_var_A = 0;
    E->dachor_var_B = 0;
    Dachora_Func_1(addr_kDachora_Palette, k);
  }
}

void Dachora_Func_5(uint16 k) {  // 0xA7F65E
  int32 amt = Dachora_Func_6(k);
  Enemy_Dachora *E = Get_Dachora(k);
  if (Enemy_MoveRight_IgnoreSlopes(k, amt)) {
    QueueSfx2_Max15(0x71);
    E->base.current_instruction = addr_kDachora_Ilist_F345;
    E->dachor_var_F = FUNC16(Dachora_Func_4);
    E->dachor_parameter_1 = 0;
    E->dachor_var_A = 0;
    Dachora_Func_1(addr_kDachora_Palette, k);
LABEL_4:
    E->dachor_var_E = 0;
    E->dachor_var_B = 0;
    E->base.instruction_timer = 1;
    return;
  }
  EnemyFunc_C8AD(k);
  if (!sign16(E->base.x_pos - 1152)) {
    E->base.current_instruction = addr_kDachora_Ilist_F4B3;
    E->dachor_var_F = FUNC16(Dachora_Func_7);
    E->dachor_var_A = g_word_A7F4CF;
    E->base.y_pos += 8;
    QueueSfx2_Max6(0x3D);
    goto LABEL_4;
  }
}

int32 Dachora_Func_6(uint16 k) {  // 0xA7F6D5
  Enemy_Dachora *E = Get_Dachora(k);
  if ((int16)(E->dachor_var_A - g_word_A7F4D5) >= 0) {
    if (E->dachor_var_E == 1)
      QueueSfx2_Max6(0x39);
    uint16 v2 = E->dachor_var_E - 1;
    E->dachor_var_E = v2;
    if (!(uint8)v2) {
      Dachora_Func_1(g_off_A7F787[HIBYTE(E->dachor_var_E)], k);
      uint16 v3 = E->dachor_var_E + 272;
      E->dachor_var_E = v3;
      if (!sign16(v3 - 1040))
        E->dachor_var_E = 784;
    }
  }
  Enemy_MoveDown(k, INT16_SHL16(1));
  if ((int16)(E->dachor_var_A - g_word_A7F4D5) >= 0 && (int16)(E->dachor_var_B - g_word_A7F4D7) >= 0) {
    E->dachor_var_A = g_word_A7F4D5;
    E->dachor_var_B = g_word_A7F4D7;
    return __PAIR32__(g_word_A7F4D5, g_word_A7F4D7);
  }
  AddToHiLo(&E->dachor_var_A, &E->dachor_var_B, __PAIR32__(g_word_A7F4D9, g_word_A7F4DB));
  if ((E->dachor_var_A == 4 || E->dachor_var_A == 8) && !E->dachor_var_B)
    E->base.current_instruction += 28;
  return __PAIR32__(E->dachor_var_A, E->dachor_var_B);
}

void Dachora_Func_7(uint16 k) {  // 0xA7F78F
  Dachora_Func_10(k);
  Enemy_Dachora *E = Get_Dachora(k);
  if (E->dachor_var_A-- == 1) {
    E->base.current_instruction += 2;
    E->base.instruction_timer = 1;
    E->dachor_var_F = FUNC16(Dachora_Func_8);
    Enemy_Dachora *E1 = Get_Dachora(k + 64);
    E1->dachor_var_A = 0;
    E1->dachor_var_D = 0;
    Get_Dachora(k + 128)->dachor_var_D = 0;
    Get_Dachora(k + 192)->dachor_var_D = 0;
    Get_Dachora(k + 256)->dachor_var_D = 0;
    E->dachor_var_C = 0;
    E->dachor_var_D = 0;
    E->base.y_pos -= 8;
    QueueSfx2_Max6(0x3B);
    if (E->dachor_parameter_1) {
      Get_Dachora(k + 64)->base.current_instruction = addr_kDachora_Ilist_F4B9;
      Get_Dachora(k + 128)->base.current_instruction = addr_kDachora_Ilist_F4B9;
      Get_Dachora(k + 192)->base.current_instruction = addr_kDachora_Ilist_F4B9;
      Get_Dachora(k + 256)->base.current_instruction = addr_kDachora_Ilist_F4B9;
    } else {
      Get_Dachora(k + 64)->base.current_instruction = addr_kDachora_Ilist_F3F7;
      Get_Dachora(k + 128)->base.current_instruction = addr_kDachora_Ilist_F3F7;
      Get_Dachora(k + 192)->base.current_instruction = addr_kDachora_Ilist_F3F7;
      Get_Dachora(k + 256)->base.current_instruction = addr_kDachora_Ilist_F3F7;
    }
    Get_Dachora(k + 64)->base.instruction_timer = 1;
    Get_Dachora(k + 128)->base.instruction_timer = 1;
    Get_Dachora(k + 192)->base.instruction_timer = 1;
    Get_Dachora(k + 256)->base.instruction_timer = 1;
  }
}

void Dachora_Func_8(uint16 k) {  // 0xA7F806
  Dachora_Func_10(k);
  Dachora_Func_9(k);
  Enemy_Dachora *E = Get_Dachora(k);
  AddToHiLo(&E->dachor_var_C, &E->dachor_var_D, __PAIR32__(samus_y_accel, samus_y_subaccel));
  AddToHiLo(&E->dachor_var_A, &E->dachor_var_B, __PAIR32__(E->dachor_var_C, E->dachor_var_D));
  uint16 v6 = E->dachor_var_A;
  if (!sign16(v6 - 15))
    v6 = 15;
  int32 amt = __PAIR32__(v6, E->dachor_var_B);
  if (Enemy_MoveDown(k, -amt)) {
    if (E->dachor_parameter_1) {
      E->base.current_instruction = addr_kDachora_Ilist_F3FF;
      E->dachor_parameter_1 = 0;
    } else {
      E->base.current_instruction = addr_kDachora_Ilist_F4C1;
      E->dachor_parameter_1 = 1;
    }
    E->dachor_var_F = FUNC16(Dachora_Func_11);
    E->base.instruction_timer = 1;
    E->dachor_var_A = 0;
    E->dachor_var_B = 0;
    E->dachor_var_E = 0;
    Dachora_Func_1(addr_kDachora_Palette, k);
    QueueSfx2_Max6(0x3C);
  }
}

void Dachora_Func_9(uint16 k) {  // 0xA7F89A

  Enemy_Dachora *EK = Get_Dachora(k);
  Enemy_Dachora *E1 = Get_Dachora(k + 64);
  uint16 dachor_var_A = E1->dachor_var_A;
  if (dachor_var_A) {
    E1->dachor_var_A = dachor_var_A - 1;
  } else {
    E1->dachor_var_A = g_word_A7F4D1;
    if (E1->dachor_var_D) {
      Enemy_Dachora *E2 = Get_Dachora(k + 128);
      if (E2->dachor_var_D) {
        Enemy_Dachora *E3 = Get_Dachora(k + 192);
        if (E3->dachor_var_D) {
          Enemy_Dachora *E4 = Get_Dachora(k + 256);
          if (!E4->dachor_var_D) {
            E4->base.x_pos = EK->base.x_pos;
            E4->base.y_pos = EK->base.y_pos;
            E4->dachor_var_D = g_word_A7F4D3;
          }
        } else {
          E3->base.x_pos = EK->base.x_pos;
          E3->base.y_pos = EK->base.y_pos;
          E3->dachor_var_D = g_word_A7F4D3;
        }
      } else {
        E2->base.x_pos = EK->base.x_pos;
        E2->base.y_pos = EK->base.y_pos;
        E2->dachor_var_D = g_word_A7F4D3;
      }
    } else {
      E1->base.x_pos = EK->base.x_pos;
      E1->base.y_pos = EK->base.y_pos;
      E1->dachor_var_D = g_word_A7F4D3;
    }
  }
}

void Dachora_Func_10(uint16 k) {  // 0xA7F90A
  Enemy_Dachora *E = Get_Dachora(k);
  Dachora_Func_1(g_off_A7F92D[HIBYTE(E->dachor_var_E)], k);
  uint16 v2 = E->dachor_var_E + 256;
  E->dachor_var_E = v2;
  if (!sign16(v2 - 1024))
    E->dachor_var_E = 0;
}

void Dachora_Func_11(uint16 k) {  // 0xA7F935
  Enemy_Dachora *E = Get_Dachora(k);
  AddToHiLo(&E->dachor_var_A, &E->dachor_var_B, __PAIR32__(samus_y_accel, samus_y_subaccel));
  int32 amt = __PAIR32__(E->dachor_var_A, E->dachor_var_B);
  if (!sign16((amt >> 16) - 10))
    amt = INT16_SHL16(10);
  if (Enemy_MoveDown(k, amt)) {
    if (E->dachor_parameter_1) {
      E->base.current_instruction = addr_kDachora_Ilist_F407;
      E->dachor_var_F = FUNC16(Dachora_Func_5);
    } else {
      E->base.current_instruction = addr_kDachora_Ilist_F345;
      E->dachor_var_F = FUNC16(Dachora_Func_4);
    }
    E->base.instruction_timer = 1;
    E->dachor_var_A = 0;
    E->dachor_var_B = 0;
  }
}

void Dachora_Func_12(uint16 k) {  // 0xA7F98C
  Enemy_Dachora *E = Get_Dachora(k);
  uint16 dachor_var_D = E->dachor_var_D;
  if (!dachor_var_D)
    goto LABEL_6;
  E->dachor_var_D = dachor_var_D - 1;
  if ((k & 0x40) != 0) {
    if ((nmi_frame_counter_word & 1) == 0)
      goto LABEL_6;
  } else if ((nmi_frame_counter_word & 1) != 0) {
LABEL_6:
    E->base.properties |= kEnemyProps_Invisible;
    return;
  }
  E->base.properties &= ~kEnemyProps_Invisible;
}
