// Enemies
#include "sm_rtl.h"
#include "ida_types.h"
#include "variables.h"
#include "funcs.h"
#include "enemy_types.h"

#define kEnemyLayerToQueuePtr ((uint16*)RomPtr(0xa0b133))

void Enemy_GrappleReact_NoInteract_A0(void) {  // 0xA08000
  SwitchEnemyAiToMainAi();
}

void Enemy_GrappleReact_SamusLatchesOn_A0(void) {  // 0xA08005
  SamusLatchesOnWithGrapple();
}

void Enemy_GrappleReact_KillEnemy_A0(void) {  // 0xA0800A
  EnemyGrappleDeath();
}

void Enemy_GrappleReact_CancelBeam_A0(void) {  // 0xA0800F
  Enemy_SwitchToFrozenAi();
}

void Enemy_GrappleReact_SamusLatchesNoInvinc_A0(void) {  // 0xA08014
  SamusLatchesOnWithGrappleNoInvinc();
}

void Enemy_GrappleReact_SamusLatchesParalyze_A0(void) {  // 0xA08019
  SamusLatchesOnWithGrappleParalyze();
}

void Enemy_GrappleReact_HurtSamus_A0(void) {  // 0xA0801E
  SamusHurtFromGrapple();
}

void Enemy_NormalTouchAI_A0(void) {  // 0xA08023
  NormalEnemyTouchAi();
}

void Enemy_NormalTouchAI_SkipDeathAnim_A0(void) {  // 0xA08028
  NormalEnemyTouchAiSkipDeathAnim_CurEnemy();
}

void Enemy_NormalShotAI_A0(void) {  // 0xA0802D
  NormalEnemyShotAi();
}

void Enemy_NormalShotAI_SkipSomeParts_A0(void) {  // 0xA08032
  NormalEnemyShotAiSkipDeathAnim_CurEnemy();
}

void Enemy_NormalPowerBombAI_A0(void) {  // 0xA08037
  NormalEnemyPowerBombAi();
}

void Enemy_NormalPowerBombAI_SkipDeathAnim_A0(void) {  // 0xA0803C
  NormalEnemyPowerBombAiSkipDeathAnim_CurEnemy();
}

void Enemy_NormalFrozenAI(void) {  // 0xA08041
  NormalEnemyFrozenAI();
}

void CreateADudShot(void) {  // 0xA08046
  CreateDudShot();
}

void func_nullsub_169(void) {
  ;
}

void func_nullsub_170(void) {
  ;
}

const uint16 *EnemyInstr_SetAiPreInstr(uint16 k, const uint16 *jp) {  // 0xA0806B
  gEnemyData(k)->ai_preinstr = jp[0];
  return jp + 1;
}

const uint16 *EnemyInstr_ClearAiPreInstr(uint16 k, const uint16 *jp) {  // 0xA08074
  gEnemyData(k)->ai_preinstr = FUNC16(nullsub_171);
  return jp;
}

void func_nullsub_171(void) {
  ;
}

const uint16 *EnemyInstr_StopScript(uint16 k, const uint16 *jp) {  // 0xA0807C
  EnemyData *v2 = gEnemyData(k);
  v2->properties |= kEnemyProps_Deleted;
  return 0;
}

const uint16 *EnemyInstr_Goto(uint16 k, const uint16 *jp) {  // 0xA280ED
  return INSTR_RETURN_ADDR(*jp);
}

const uint16 *EnemyInstr_GotoRel(uint16 k, const uint16 *jp) {  // 0xA080F2
  return (const uint16 * )((uint8*)jp + *(int8*)jp);
}

const uint16 *EnemyInstr_DecTimerAndGoto(uint16 k, const uint16 *jp) {  // 0xA08108
  EnemyData *v2 = gEnemyData(k);
  if (v2->timer-- == 1)
    return jp + 1;
  else
    return EnemyInstr_Goto(k, jp);
}

const uint16 *EnemyInstr_DecTimerAndGotoRel(uint16 k, const uint16 *jp) {  // 0xA08118
  EnemyData *v2 = gEnemyData(k);
  if (LOBYTE(v2->timer)-- == 1)
    return (const uint16 *)((uint8 *)jp + 1);
  else
    return EnemyInstr_GotoRel(k, jp);
}

const uint16 *EnemyInstr_SetTimer(uint16 k, const uint16 *jp) {  // 0xA08123
  gEnemyData(k)->timer = *jp;
  return jp + 1;
}

const uint16 *EnemyInstr_Skip2bytes(uint16 k, const uint16 *jp) {  // 0xA0812C
  return jp + 1;
}

const uint16 *EnemyInstr_Sleep(uint16 k, const uint16 *jp) {  // 0xA2812F
  EnemyData *ED = gEnemyData(k);
  uint8 *base_ptr = RomPtrWithBank(ED->bank, 0x8000) - 0x8000;
  ED->current_instruction = (const uint8 *)jp - 2 - base_ptr;
  return 0;
}

const uint16 *EnemyInstr_WaitNframes(uint16 k, const uint16 *jp) {  // 0xA0813A
  EnemyData *ED = gEnemyData(k);
  uint8 *base_ptr = RomPtrWithBank(ED->bank, 0x8000) - 0x8000;
  ED->instruction_timer = jp[0];
  ED->current_instruction = (const uint8 *)jp + 2 - base_ptr;
  return 0;
}

const uint16 *EnemyInstr_CopyToVram(uint16 k, const uint16 *jp) {  // 0xA0814B
  VramWriteEntry *v4;

  uint16 v2 = vram_write_queue_tail;
  uint8 *v3 = (uint8*)jp;
  v4 = gVramWriteEntry(vram_write_queue_tail);
  v4->size = *(uint16 *)v3;
  v4->src.addr = *((uint16 *)v3 + 1);
  *(VoidP *)((char *)&v4->src.addr + 1) = *(uint16 *)(v3 + 3);
  v4->vram_dst = *(uint16 *)(v3 + 5);
  vram_write_queue_tail = v2 + 7;
  return INSTR_INCR_BYTES(jp, 7);
}

const uint16 *EnemyInstr_EnableOffScreenProcessing(uint16 k, const uint16 *jp) {  // 0xA08173
  EnemyData *v2 = gEnemyData(k);
  v2->properties |= 0x800u;
  return jp;
}

const uint16 *EnemyInstr_DisableOffScreenProcessing(uint16 k, const uint16 *jp) {  // 0xA0817D
  EnemyData *v2 = gEnemyData(k);
  v2->properties &= ~0x800u;
  return jp;
}

static const uint16 kRoomShakes[144] = {  // 0xA08687
  1, 0, 0, 0,
  0, 1, 0, 0,
  1, 1, 0, 0,
  2, 0, 0, 0,
  0, 2, 0, 0,
  2, 2, 0, 0,
  3, 0, 0, 0,
  0, 3, 0, 0,
  3, 3, 0, 0,
  1, 0, 1, 0,
  0, 1, 0, 1,
  1, 1, 1, 1,
  2, 0, 2, 0,
  0, 2, 0, 2,
  2, 2, 2, 2,
  3, 0, 3, 0,
  0, 3, 0, 3,
  3, 3, 3, 3,
  1, 0, 1, 0,
  0, 1, 0, 1,
  1, 1, 1, 1,
  2, 0, 2, 0,
  0, 2, 0, 2,
  2, 2, 2, 2,
  3, 0, 3, 0,
  0, 3, 0, 3,
  3, 3, 3, 3,
  0, 0, 1, 0,
  0, 0, 0, 1,
  0, 0, 1, 1,
  0, 0, 2, 0,
  0, 0, 0, 2,
  0, 0, 2, 2,
  0, 0, 3, 0,
  0, 0, 0, 3,
  0, 0, 3, 3,
};

void HandleRoomShaking(void) {

  if (earthquake_timer && !time_is_frozen_flag && sign16(earthquake_type - 36)) {
    int v0 = (uint16)(8 * earthquake_type) >> 1;
    if ((earthquake_timer & 2) != 0) {
      reg_BG1HOFS -= kRoomShakes[v0];
      reg_BG1VOFS -= kRoomShakes[v0 + 1];
      reg_BG2HOFS -= kRoomShakes[v0 + 2];
      reg_BG2VOFS -= kRoomShakes[v0 + 3];
    } else {
      reg_BG1HOFS += kRoomShakes[v0];
      reg_BG1VOFS += kRoomShakes[v0 + 1];
      reg_BG2HOFS += kRoomShakes[v0 + 2];
      reg_BG2VOFS += kRoomShakes[v0 + 3];
    }
    --earthquake_timer;
    if (!sign16(earthquake_type - 18))
      SetAllEnemiesToShakeFor2Frames();
  }
  ++frame_counter_every_frame;
}

void SetAllEnemiesToShakeFor2Frames(void) {  // 0xA08712
  for (int i = 0; ; i += 2) {
    int v1 = i >> 1;
    if (active_enemy_indexes[v1] == 0xFFFF)
      break;
    gEnemyData(active_enemy_indexes[v1])->shake_timer = 2;
  }
}

void CallEnemyGfxDrawHook(uint32 ea) {
  switch (ea) {
  case fnnullsub_170: return;  // 0xa0804c
  case fnReflec_Func_1: Reflec_Func_1(); return;  // 0xa3db0c
  case fnDraygon_Func_36: Draygon_Func_36(); return;  // 0xa59342
  case fnRidley_A2F2: Ridley_A2F2(); return;  // 0xa6a2f2
  case fnnullsub_170_A8: return;  // 0xa8804c
  case fnNorfairLavaMan_Func_6: NorfairLavaMan_Func_6(); return;  // 0xa8b0b2
  case fnWreckedShipRobot_Func_1: WreckedShipRobot_Func_1(); return;  // 0xa8cc67
  case fnBlueBrinstarFaceBlock_Func_1: BlueBrinstarFaceBlock_Func_1(); return;  // 0xa8e86e
  case fnnullsub_264: return;  // 0xa98786
  case fnMotherBrain_DrawBrainNeck_EnemyGfxDrawHook: MotherBrain_DrawBrainNeck_EnemyGfxDrawHook(); return;  // 0xa987c9
  case fnMotherBrainsBrain_GfxDrawHook: MotherBrainsBrain_GfxDrawHook(); return;  // 0xa987dd
  case fnDeadTorizo_MainGfxHook: DeadTorizo_MainGfxHook(); return;  // 0xa9d39a
  default: Unreachable();
  }
}

void DrawSamusEnemiesAndProjectiles(void) {  // 0xA0884D
  DrawSpriteObjects();
  DrawBombAndProjectileExplosions();
  DrawLowPriorityEnemyProjectiles();
  for (draw_enemy_layer = 0; draw_enemy_layer != 8; ++draw_enemy_layer) {
    if (draw_enemy_layer == 3) {
      DrawSamusAndProjectiles();
    } else if (draw_enemy_layer == 6) {
      DrawHighPriorityEnemyProjectiles();
    }
    int v0 = draw_enemy_layer;
    if (enemy_drawing_queue_sizes[v0]) {
      loop_index_end = enemy_drawing_queue_sizes[v0];
      enemy_drawing_queue_base = kEnemyLayerToQueuePtr[v0];
      uint16 v1 = 0;
      enemy_drawing_queue_sizes[v0] = 0;
      do {
        loop_index = v1;
        uint8 *v2 = RomPtr_RAM(enemy_drawing_queue_base + v1);
        uint16 v3 = *(uint16 *)v2;
        *(uint16 *)v2 = 0;
        cur_enemy_index = v3;
        WriteEnemyOams();
        v1 = loop_index + 2;
      } while (loop_index + 2 != loop_index_end);
    }
  }
  CallEnemyGfxDrawHook(Load24(&enemy_gfx_drawn_hook));
}

void RecordEnemySpawnData(uint16 j) {  // 0xA088D0
  EnemySpawnData *v2;
  EnemySpawnData *v5;

  EnemyData *v1 = gEnemyData(j);
  v2 = gEnemySpawnData(j);
  v2->id = v1->enemy_ptr;
  v2->x_pos = v1->x_pos;
  v2->y_pos = v1->y_pos;
  v2->init_param = v1->current_instruction;
  v2->properties = v1->properties;
  v2->extra_properties = v1->extra_properties;
  v2->param_1 = v1->parameter_1;
  v2->param_2 = v1->parameter_2;
  R18_ = 0;
  R20_ = 0;
  R22_ = 0;
  R24_ = 0;
  R26_ = 0;
  R28_ = 0;
  uint16 name_ptr = get_EnemyDef_A2(v1->enemy_ptr)->name_ptr;
  if (name_ptr) {
    uint16 *v4 = (uint16 *)RomPtr_B4(name_ptr);
    R18_ = *v4;
    R20_ = v4[1];
    R22_ = v4[2];
    R24_ = v4[3];
    R26_ = v4[4];
    R28_ = v4[6];
  }
  v5 = gEnemySpawnData(j);
  *(uint16 *)v5->name = R18_;
  *(uint16 *)&v5->name[2] = R20_;
  *(uint16 *)&v5->name[4] = R22_;
  *(uint16 *)&v5->name[6] = R24_;
  *(uint16 *)&v5->name[8] = R26_;
  *(uint16 *)&v5->name[10] = R28_;
}

void DebugLoadEnemySetData(void) {  // 0xA0896F
  int16 v1;

  uint16 v0 = 0;
  v1 = 160;
  do {
    *(uint16 *)&debug_enemy_set_name[v0] = 0;
    v0 += 2;
    v1 -= 2;
  } while (v1);
  uint8 *v2 = RomPtr_B4(room_enemy_tilesets_ptr - 7);
  *(uint16 *)debug_enemy_set_name = *(uint16 *)v2;
  *(uint16 *)&debug_enemy_set_name[2] = *((uint16 *)v2 + 1);
  *(uint16 *)&debug_enemy_set_name[4] = *((uint16 *)v2 + 2);
  *(uint16 *)&debug_enemy_set_name[6] = *((uint16 *)v2 + 3);
  uint16 v3 = 7;
  for (int i = room_enemy_tilesets_ptr; ; i += 4) {
    uint16 v5 = *(uint16 *)RomPtr_B4(i);
    if (v5 == 0xFFFF)
      break;
    uint16 v6 = *((uint16 *)RomPtr_A0(v5) + 31);
    if (!v6)
      v6 = addr_asc_B4DD89;
    uint16 *v7 = (uint16 *)RomPtr_B4(v6);
    R18_ = *v7;
    R20_ = v7[1];
    R22_ = v7[2];
    R24_ = v7[3];
    R26_ = v7[4];
    *(uint16 *)&debug_enemy_set_name[v3] = R18_;
    *(uint16 *)&debug_enemy_set_name[v3 + 2] = R20_;
    *(uint16 *)&debug_enemy_set_name[v3 + 4] = R22_;
    *(uint16 *)&debug_enemy_set_name[v3 + 6] = R24_;
    *(uint16 *)&debug_enemy_set_data[v3] = R26_;
    *(uint16 *)&debug_enemy_set_data[v3 + 2] = *((uint16 *)RomPtr_B4(i) + 1);
    v3 += 12;
  }
}

void LoadEnemies(void) {  // 0xA08A1E
  //  DebugLoadEnemySetData();
  debug_time_frozen_for_enemies = 0;
  *(uint16 *)&enemy_gfx_drawn_hook.bank = 160;
  enemy_gfx_drawn_hook.addr = FUNC16(nullsub_170);
  *(uint16 *)&set_to_rtl_when_loading_enemies_unused.bank = 160;
  set_to_rtl_when_loading_enemies_unused.addr = FUNC16(nullsub_170);
  enemy_bg2_tilemap_size = 2048;
  UNUSED_word_7E179E = 0;
  UNUSED_word_7E17A0 = 0;
  boss_id = 0;
  ClearEnemyDataAndProcessEnemySet();
  LoadEnemyTileData();
  enemy_tile_vram_src = 0;
  flag_disable_projectile_interaction = 0;
  ClearSpriteObjects();
}

void ClearEnemyDataAndProcessEnemySet(void) {  // 0xA08A6D
  int16 v0;

  v0 = 2048;
  uint16 v1 = 0;
  do {
    gEnemyData(v1)->enemy_ptr = 0;
    v1 += 2;
    v0 -= 2;
  } while (v0);
  if (*(uint16 *)RomPtr_A1(room_enemy_population_ptr) != 0xFFFF)
    ProcessEnemyTilesets();
}

void InitializeEnemies(void) {  // 0xA08A9E
  int16 v0;
  uint16 j;
  EnemyPopulation *v9;
  EnemyDef_A2 *v10;
  EnemyPopulation *EnemyPopulation;
  EnemyDef_A2 *EnemyDef_A2;

  v0 = 5120;
  uint16 v1 = 0;
  do {
    gEnemySpawnData(v1)->some_flag = 0;
    v1 += 2;
    --v0;
  } while (v0);
  num_enemies_in_room = 0;
  num_enemies_killed_in_room = 0;
  flag_process_all_enemies = 0;
  for (int i = 286; i >= 0; i -= 2)
    enemy_projectile_flags[i >> 1] = 0;
  for (j = 34; (j & 0x8000u) == 0; j -= 2)
    enemy_projectile_killed_enemy_index[j >> 1] = -1;
  uint16 v4 = room_enemy_population_ptr;
  if (get_EnemyPopulation(0xa1, room_enemy_population_ptr)->enemy_ptr != 0xFFFF) {
    UNUSED_word_7E0E48 = 0;
    uint16 v5 = 0;
    do {
      LoadEnemyGfxIndexes(v4, v5);
      uint16 v13 = v4;
      EnemyPopulation = get_EnemyPopulation(0xa1, v4);
      EnemyDef_A2 = get_EnemyDef_A2(EnemyPopulation->enemy_ptr);
      uint16 *v8 = (uint16 *)gEnemyData(v5);
      v8[5] = EnemyDef_A2->x_radius;
      v8[6] = EnemyDef_A2->y_radius;
      v8[10] = EnemyDef_A2->health;
      v8[17] = EnemyDef_A2->layer;
      v8[23] = *(uint16 *)&EnemyDef_A2->bank;
      if (EnemyDef_A2->boss_fight_value)
        boss_id = EnemyDef_A2->boss_fight_value;
      v9 = get_EnemyPopulation(0xa1, v4);
      *v8 = v9->enemy_ptr;
      v8[1] = v9->x_pos;
      v8[3] = v9->y_pos;
      v8[13] = v9->init_param;
      v8[7] = v9->properties;
      v8[8] = v9->extra_properties;
      v8[30] = v9->parameter1;
      v8[31] = v9->parameter2;
      v8[22] = 0;
      v8[12] = 0;
      v8[14] = 1;
      v8[22] = 0;
      RecordEnemySpawnData(v5);
      cur_enemy_index = v5;
      v10 = get_EnemyDef_A2(*v8);
      enemy_ai_pointer.addr = v10->ai_init;
      *(uint16 *)&enemy_ai_pointer.bank = *(uint16 *)&v10->bank;
      CallEnemyAi(Load24(&enemy_ai_pointer));
      EnemyData *v11 = gEnemyData(v5);
      v11->spritemap_pointer = 0;
      if ((v11->properties & 0x2000) != 0) {
        uint16 v12 = addr_kSpritemap_Nothing_A4;
        if ((v11->extra_properties & 4) != 0)
          v12 = addr_kExtendedSpritemap_Nothing_A4;
        v11->spritemap_pointer = v12;
        v4 = v13;
      }
      v5 += 64;
      v4 += 16;
    } while (get_EnemyPopulation(0xa1, v4)->enemy_ptr != 0xFFFF);
    first_free_enemy_index = v5;
    num_enemies_in_room = v5 >> 6;
    num_enemy_deaths_left_to_clear = RomPtr_A1(v4)[2];
  }
}

void LoadEnemyGfxIndexes(uint16 k, uint16 j) {  // 0xA08BF3
  VoidP enemy_ptr;
  EnemyTileset *EnemyTileset;
  EnemySpawnData *v6;
  EnemySpawnData *v10;

  R18_ = k;
  R20_ = j;
  R28_ = room_enemy_tilesets_ptr;
  g_word_7E001E = 0;
  while (1) {
    enemy_ptr = get_EnemyPopulation(0xa1, R18_)->enemy_ptr;
    EnemyTileset = get_EnemyTileset(R28_);
    if (enemy_ptr == EnemyTileset->enemy_def)
      break;
    if (EnemyTileset->enemy_def == 0xFFFF) {
      uint16 v4 = R20_;
      EnemyData *v5 = gEnemyData(R20_);
      v5->vram_tiles_index = 0;
      v6 = gEnemySpawnData(v4);
      v6->vram_tiles_index = 0;
      v5->palette_index = 2560;
      v6->palette_index = 2560;
      return;
    }
    g_word_7E001E += get_EnemyDef_A2(EnemyTileset->enemy_def)->tile_data_size >> 5;
    R28_ += 4;
  }
  uint16 v7 = (get_EnemyTileset(R28_)->vram_dst & 0xF) << 9;
  uint16 v8 = R20_;
  EnemyData *v9 = gEnemyData(R20_);
  v9->palette_index = v7;
  v10 = gEnemySpawnData(v8);
  v10->palette_index = v7;
  uint16 v11 = g_word_7E001E;
  v9->vram_tiles_index = g_word_7E001E;
  v10->vram_tiles_index = v11;
}
#define kStandardSpriteTiles ((uint16*)RomPtr(0x9ad200))
void LoadEnemyTileData(void) {  // 0xA08C6C
  for (int i = 510; i >= 0; i -= 2)
    gEnemySpawnData(i)->some_flag = kStandardSpriteTiles[(i >> 1) + 3072];
  if (enemy_tile_load_data_write_pos) {
    uint16 v1 = 0;
    EnemyTileLoadData *load_data = enemy_tile_load_data;
    do {
      uint16 v2 = load_data->tile_data_ptr.addr;
      R18_ = load_data->tile_data_size + v2;
      uint16 v3 = load_data->offset_into_ram;
      uint8 db = load_data->tile_data_ptr.bank;
      do {
        memcpy(&g_ram[0x7000 + v3], RomPtrWithBank(db, v2), 8);
        v3 += 8;
        v2 += 8;
      } while (v2 != R18_);
      load_data++;
      v1 += 7;
    } while (v1 != enemy_tile_load_data_write_pos);
    enemy_tile_load_data_write_pos = 0;
  }
}

void TransferEnemyTilesToVramAndInit(void) {  // 0xA08CD7
  VramWriteEntry *v2;

  uint16 v0 = enemy_tile_vram_src;
  if (!enemy_tile_vram_src) {
    v0 = ADDR16_OF_RAM(*enemy_spawn_data);
    enemy_tile_vram_src = ADDR16_OF_RAM(*enemy_spawn_data);
    enemy_tile_vram_dst = 0x6c00;
  }
  if (v0 != 0xFFFF) {
    if (v0 == 0xFFFE) {
      InitializeEnemies();
      enemy_tile_vram_src = -1;
    } else if (v0 == 0x9800) {
      enemy_tile_vram_src = -2;
    } else {
      uint16 v1 = vram_write_queue_tail;
      v2 = gVramWriteEntry(vram_write_queue_tail);
      v2->size = 2048;
      uint16 v3 = enemy_tile_vram_src;
      v2->src.addr = enemy_tile_vram_src;
      enemy_tile_vram_src = v3 + 2048;
      *(uint16 *)&v2->src.bank = 126;
      uint16 v4 = enemy_tile_vram_dst;
      v2->vram_dst = enemy_tile_vram_dst;
      enemy_tile_vram_dst = v4 + 1024;
      vram_write_queue_tail = v1 + 7;
    }
  }
}

void ProcessEnemyTilesets(void) {  // 0xA08D64
  enemy_tile_load_data_write_pos = 0;
  g_word_7E001E = 2048;
  enemy_def_ptr[0] = 0;
  enemy_def_ptr[1] = 0;
  enemy_def_ptr[2] = 0;
  enemy_def_ptr[3] = 0;
  enemy_gfxdata_tiles_index[0] = 0;
  enemy_gfxdata_tiles_index[1] = 0;
  enemy_gfxdata_tiles_index[2] = 0;
  enemy_gfxdata_tiles_index[3] = 0;
  enemy_gfxdata_vram_ptr[0] = 0;
  enemy_gfxdata_vram_ptr[1] = 0;
  enemy_gfxdata_vram_ptr[2] = 0;
  enemy_gfxdata_vram_ptr[3] = 0;
  enemy_gfx_data_write_ptr = 0;
  next_enemy_tiles_index = 0;
  EnemyTileset *ET = get_EnemyTileset(room_enemy_tilesets_ptr);
  EnemyTileLoadData *LD = enemy_tile_load_data;
  for (; ET->enemy_def != 0xffff; ET++) {
    EnemyDef_A2 *ED = get_EnemyDef_A2(ET->enemy_def);
    memcpy(&target_palettes[(LOBYTE(ET->vram_dst) + 8) * 16], 
           RomPtrWithBank(ED->bank, ED->palette_ptr), 32);
    varE2E = 0;
    LD->tile_data_size = ED->tile_data_size & 0x7FFF;
    LD->tile_data_ptr = ED->tile_data;
    uint16 v10 = g_word_7E001E;
    if ((ED->tile_data_size & 0x8000u) != 0)
      v10 = (uint16)(ET->vram_dst & 0x3000) >> 3;
    LD->offset_into_ram = v10;
    enemy_tile_load_data_write_pos += 7;
    LD++;
    uint16 v11 = enemy_gfx_data_write_ptr;
    enemy_gfxdata_tiles_index[enemy_gfx_data_write_ptr >> 1] = next_enemy_tiles_index;
    enemy_def_ptr[v11 >> 1] = ET->enemy_def;
    enemy_gfxdata_vram_ptr[v11 >> 1] = ET->vram_dst;
    enemy_gfx_data_write_ptr += 2;
    next_enemy_tiles_index += ED->tile_data_size >> 5;
    g_word_7E001E += ED->tile_data_size;
  }
}

void DetermineWhichEnemiesToProcess(void) {  // 0xA08EB6
  ++UNUSED_word_7E0E46;
  cur_enemy_index = 0;
  active_enemy_indexes_write_ptr = 0;
  interactive_enemy_indexes_write_ptr = 0;
  if (flag_process_all_enemies) {
    do {
      uint16 v5 = cur_enemy_index;
      EnemyData *v6 = gEnemyData(cur_enemy_index);
      if (v6->enemy_ptr && v6->enemy_ptr != (uint16)addr_kEnemyDef_DAFF) {
        if ((v6->properties & 0x200) != 0) {
          v6->enemy_ptr = 0;
        } else {
          uint16 v7 = active_enemy_indexes_write_ptr;
          int v8 = active_enemy_indexes_write_ptr >> 1;
          active_enemy_indexes[v8] = cur_enemy_index;
          interactive_enemy_indexes[v8] = v5;
          active_enemy_indexes_write_ptr = v7 + 2;
          if ((v6->properties & 0x400) == 0) {
            uint16 v9 = interactive_enemy_indexes_write_ptr;
            interactive_enemy_indexes[interactive_enemy_indexes_write_ptr >> 1] = v5;
            interactive_enemy_indexes_write_ptr = v9 + 2;
          }
        }
      }
      cur_enemy_index += 64;
    } while (sign16(cur_enemy_index - 2048));
    active_enemy_indexes[active_enemy_indexes_write_ptr >> 1] = -1;
    interactive_enemy_indexes[interactive_enemy_indexes_write_ptr >> 1] = -1;
  } else {
    do {
      uint16 v0 = cur_enemy_index;
      EnemyData *v1 = gEnemyData(cur_enemy_index);
      if (v1->enemy_ptr && v1->enemy_ptr != (uint16)addr_kEnemyDef_DAFF) {
        uint16 properties = v1->properties;
        if ((properties & 0x200) != 0) {
          v1->enemy_ptr = 0;
        } else if ((properties & 0x800) != 0
                   || (v1->ai_handler_bits & 4) != 0
                   || (int16)(v1->x_width + v1->x_pos - layer1_x_pos) >= 0
                   && (int16)(v1->x_width + layer1_x_pos + 256 - v1->x_pos) >= 0
                   && (int16)(v1->y_pos + 8 - layer1_y_pos) >= 0
                   && (int16)(layer1_y_pos + 248 - v1->y_pos) >= 0) {
          uint16 v3 = active_enemy_indexes_write_ptr;
          active_enemy_indexes[active_enemy_indexes_write_ptr >> 1] = cur_enemy_index;
          active_enemy_indexes_write_ptr = v3 + 2;
          if ((v1->properties & 0x400) == 0) {
            uint16 v4 = interactive_enemy_indexes_write_ptr;
            interactive_enemy_indexes[interactive_enemy_indexes_write_ptr >> 1] = v0;
            interactive_enemy_indexes_write_ptr = v4 + 2;
          }
        }
      }
      cur_enemy_index += 64;
    } while (sign16(cur_enemy_index - 2048));
    active_enemy_indexes[active_enemy_indexes_write_ptr >> 1] = -1;
    interactive_enemy_indexes[interactive_enemy_indexes_write_ptr >> 1] = -1;
  }
}

void CallEnemyAi(uint32 ea) {
  switch (ea) {
  case fnEnemy_GrappleReact_SamusLatchesOn_A2: Enemy_GrappleReact_SamusLatchesOn_A2(); return;
  case fnEnemy_GrappleReact_KillEnemy_A2: Enemy_GrappleReact_KillEnemy_A2(); return;
  case fnEnemy_GrappleReact_CancelBeam_A2: Enemy_GrappleReact_CancelBeam_A2(); return;
  case fnEnemy_NormalTouchAI_A2: Enemy_NormalTouchAI_A2(); return;
  case fnEnemy_NormalShotAI_A2: Enemy_NormalShotAI_A2(); return;
  case fnEnemy_NormalFrozenAI_A2: Enemy_NormalFrozenAI_A2(); return;
  case fnnullsub_170_A2: return;
  case fnBouncingGoofball_Init: BouncingGoofball_Init(); return;
  case fnBouncingGoofball_Main: BouncingGoofball_Main(); return;
  case fnMiniCrocomire_Init: MiniCrocomire_Init(); return;
  case fnMiniCrocomire_Main: MiniCrocomire_Main(); return;
  case fnMaridiaBeybladeTurtle_Init: MaridiaBeybladeTurtle_Init(); return;
  case fnMiniMaridiaBeybladeTurtle_Init: MiniMaridiaBeybladeTurtle_Init(); return;
  case fnMaridiaBeybladeTurtle_Main: MaridiaBeybladeTurtle_Main(); return;
  case fnMiniMaridiaBeybladeTurtle_Main: MiniMaridiaBeybladeTurtle_Main(); return;
  case fnMaridiaBeybladeTurtle_Touch: MaridiaBeybladeTurtle_Touch(); return;
  case fnMiniMaridiaBeybladeTurtle_Touch: MiniMaridiaBeybladeTurtle_Touch(); return;
  case fnMiniMaridiaBeybladeTurtle_Shot: MiniMaridiaBeybladeTurtle_Shot(); return;
  case fnThinHoppingBlobs_Init: ThinHoppingBlobs_Init(); return;
  case fnThinHoppingBlobs_Main: ThinHoppingBlobs_Main(); return;
  case fnSpikeShootingPlant_Init: SpikeShootingPlant_Init(); return;
  case fnSpikeShootingPlant_Main: SpikeShootingPlant_Main(); return;
  case fnMaridiaSpikeyShell_Init: MaridiaSpikeyShell_Init(); return;
  case fnMaridiaSpikeyShell_Main: MaridiaSpikeyShell_Main(); return;
  case fnMaridiaSpikeyShell_Shot: MaridiaSpikeyShell_Shot(); return;
  case fnGunshipTop_Init: GunshipTop_Init(); return;
  case fnGunshipBottom_Init: GunshipBottom_Init(); return;
  case fnGunshipTop_Main: GunshipTop_Main(); return;
  case fnFlies_Init: Flies_Init(); return;
  case fnFlies_Main: Flies_Main(); return;
  case fnNorfairErraticFireball_Init: NorfairErraticFireball_Init(); return;
  case fnNorfairErraticFireball_Main: NorfairErraticFireball_Main(); return;
  case fnLavaquakeRocks_Init: LavaquakeRocks_Init(); return;
  case fnLavaquakeRocks_Main: LavaquakeRocks_Main(); return;
  case fnRinka_Init: Rinka_Init(); return;
  case fnRinka_Main: Rinka_Main(); return;
  case fnRinka_Frozen: Rinka_Frozen(cur_enemy_index); return;
  case fnRinka_Touch: Rinka_Touch(); return;
  case fnRinka_Shot: Rinka_Shot(); return;
  case fnRinka_Powerbomb: Rinka_Powerbomb(cur_enemy_index); return;
  case fnRio_Init: Rio_Init(); return;
  case fnRio_Main: Rio_Main(); return;
  case fnNorfairLavajumpingEnemy_Init: NorfairLavajumpingEnemy_Init(); return;
  case fnNorfairLavajumpingEnemy_Main: NorfairLavajumpingEnemy_Main(); return;
  case fnNorfairRio_Init: NorfairRio_Init(); return;
  case fnNorfairRio_Main: NorfairRio_Main(); return;
  case fnLowerNorfairRio_Init: LowerNorfairRio_Init(); return;
  case fnLowerNorfairRio_Main: LowerNorfairRio_Main(); return;
  case fnMaridiaLargeSnail_Init: MaridiaLargeSnail_Init(); return;
  case fnMaridiaLargeSnail_Main: MaridiaLargeSnail_Main(); return;
  case fnMaridiaLargeSnail_Touch: MaridiaLargeSnail_Touch(); return;
  case fnMaridiaLargeSnail_Shot: MaridiaLargeSnail_Shot(); return;
  case fnHirisingSlowfalling_Init: HirisingSlowfalling_Init(); return;
  case fnHirisingSlowfalling_Main: HirisingSlowfalling_Main(); return;
  case fnGripper_Init: Gripper_Init(); return;
  case fnGripper_Main: Gripper_Main(); return;
  case fnJetPowerRipper_Init: JetPowerRipper_Init(); return;
  case fnJetPowerRipper_Main: JetPowerRipper_Main(); return;
  case fnJetPowerRipper_Shot: JetPowerRipper_Shot(); return;
  case fnRipper_Init: Ripper_Init(); return;
  case fnRipper_Main: Ripper_Main(); return;
  case fnLavaSeahorse_Init: LavaSeahorse_Init(); return;
  case fnLavaSeahorse_Main: LavaSeahorse_Main(); return;
  case fnLavaSeahorse_Touch: LavaSeahorse_Touch(); return;
  case fnLavaSeahorse_Shot: LavaSeahorse_Shot(); return;
  case fnLavaSeahorse_Powerbomb: LavaSeahorse_Powerbomb(); return;
  case fnTimedShutter_Init: TimedShutter_Init(); return;
  case fnTimedShutter_Main: TimedShutter_Main(); return;
  case fnRisingFallingPlatform_Init: RisingFallingPlatform_Init(); return;
  case fnShootableShutter_Init: ShootableShutter_Init(); return;
  case fnRisingFallingPlatform_Main: RisingFallingPlatform_Main(); return;
  case fnRisingFallingPlatform_Touch: RisingFallingPlatform_Touch(); return;
  case fnRisingFallingPlatform_Shot: RisingFallingPlatform_Shot(); return;
  case fnShootableShutter_Shot: ShootableShutter_Shot(); return;
  case fnRisingFallingPlatform_Powerbomb: RisingFallingPlatform_Powerbomb(); return;
  case fnHorizontalShootableShutter_Init: HorizontalShootableShutter_Init(); return;
  case fnHorizontalShootableShutter_Main: HorizontalShootableShutter_Main(); return;
  case fnHorizontalShootableShutter_Touch: HorizontalShootableShutter_Touch(); return;
  case fnHorizontalShootableShutter_Shot: HorizontalShootableShutter_Shot(); return;
  case fnHorizontalShootableShutter_Powerbomb: HorizontalShootableShutter_Powerbomb(); return;
  case fnEnemy_GrappleReact_NoInteract_A3: Enemy_GrappleReact_NoInteract_A3(); return;
  case fnEnemy_GrappleReact_KillEnemy_A3: Enemy_GrappleReact_KillEnemy_A3(); return;
  case fnEnemy_GrappleReact_CancelBeam_A3: Enemy_GrappleReact_CancelBeam_A3(); return;
  case fnEnemy_NormalTouchAI_A3: Enemy_NormalTouchAI_A3(); return;
  case fnEnemy_NormalShotAI_A3: Enemy_NormalShotAI_A3(); return;
  case fnEnemy_NormalFrozenAI_A3: Enemy_NormalFrozenAI_A3(); return;
  case fnnullsub_170_A3: return;
  case fnWaver_Init: Waver_Init(); return;
  case fnWaver_Main: Waver_Main(); return;
  case fnMetalee_Init: Metalee_Init(); return;
  case fnMetalee_Main: Metalee_Main(); return;
  case fnMetalee_Shot: Metalee_Shot(); return;
  case fnFireflea_Init: Fireflea_Init(); return;
  case fnFireflea_Main: Fireflea_Main(); return;
  case fnFireflea_Touch: Fireflea_Touch(cur_enemy_index); return;
  case fnFireflea_Powerbomb: Fireflea_Powerbomb(); return;
  case fnFireflea_Shot: Fireflea_Shot(); return;
  case fnMaridiaFish_Init: MaridiaFish_Init(); return;
  case fnMaridiaFish_Main: MaridiaFish_Main(); return;
  case fnElevator_Init: Elevator_Init(); return;
  case fnElevator_Frozen: Elevator_Frozen(); return;
  case fnCrab_Init: Crab_Init(); return;
  case fnSlug_Init: Slug_Init(); return;
  case fnPlatformThatFallsWithSamus_Init: PlatformThatFallsWithSamus_Init(); return;
  case fnFastMovingSlowSinkingPlatform_Init: FastMovingSlowSinkingPlatform_Init(); return;
  case fnPlatformThatFallsWithSamus_Main: PlatformThatFallsWithSamus_Main(); return;
  case fnnullsub_32: return;
  case fnFastMovingSlowSinkingPlatform_Shot: FastMovingSlowSinkingPlatform_Shot(); return;
  case fnRoach_Init: Roach_Init(); return;
  case fnRoach_Main: Roach_Main(); return;
  case fnMochtroid_Init: Mochtroid_Init(); return;
  case fnMochtroid_Main: Mochtroid_Main(); return;
  case fnMochtroid_Touch: Mochtroid_Touch(); return;
  case fnMochtroid_Shot: Mochtroid_Shot(); return;
  case fnSidehopper_Init: Sidehopper_Init(); return;
  case fnSidehopper_Main: Sidehopper_Main(); return;
  case fnMaridiaRefillCandy_Init: MaridiaRefillCandy_Init(); return;
  case fnMaridiaRefillCandy_Main: MaridiaRefillCandy_Main(); return;
  case fnNorfairSlowFireball_Init: NorfairSlowFireball_Init(); return;
  case fnBang_Init: Bang_Init(); return;
  case fnBang_Main: Bang_Main(); return;
  case fnBang_Shot: Bang_Shot(); return;
  case fnSkree_Init: Skree_Init(); return;
  case fnSkree_Main: Skree_Main(); return;
  case fnSkree_Shot: Skree_Shot(); return;
  case fnMaridiaSnail_Init: MaridiaSnail_Init(); return;
  case fnMaridiaSnail_Main: MaridiaSnail_Main(); return;
  case fnMaridiaSnail_Touch: MaridiaSnail_Touch(); return;
  case fnMaridiaSnail_Shot: MaridiaSnail_Shot(); return;
  case fnReflec_Init: Reflec_Init(); return;
  case fnnullsub_33: return;
  case fnReflec_Shot: Reflec_Shot(); return;
  case fnWreckedShipOrangeZoomer_Init: WreckedShipOrangeZoomer_Init(); return;
  case fnWreckedShipOrangeZoomer_Main: WreckedShipOrangeZoomer_Main(); return;
  case fnBigEyeBugs_Init: BigEyeBugs_Init(); return;
  case fnFireZoomer_Init: FireZoomer_Init(); return;
  case fnStoneZoomer_Init: StoneZoomer_Init(); return;
  case fnStoneZoomer_Main: StoneZoomer_Main(); return;
  case fnMetroid_Init: Metroid_Init(); return;
  case fnMetroid_Frozen: Metroid_Frozen(); return;
  case fnMetroid_Hurt: Metroid_Hurt(); return;
  case fnMetroid_Main: Metroid_Main(); return;
  case fnMetroid_Touch: Metroid_Touch(); return;
  case fnMetroid_Shot: Metroid_Shot(); return;
  case fnMetroid_Powerbomb: Metroid_Powerbomb(cur_enemy_index); return;
  case fnEnemy_GrappleReact_SamusLatchesOn_A4: Enemy_GrappleReact_SamusLatchesOn_A4(); return;
  case fnEnemy_NormalTouchAI_A4: Enemy_NormalTouchAI_A4(); return;
  case fnEnemy_NormalShotAI_A4: Enemy_NormalShotAI_A4(); return;
  case fnEnemy_NormalFrozenAI_A4: Enemy_NormalFrozenAI_A4(); return;
  case fnnullsub_170_A4: return;
  case fnCrocomire_Hurt: Crocomire_Hurt(); return;
  case fnCrocomire_Init: Crocomire_Init(); return;
  case fnCrocomire_Main: Crocomire_Main(); return;
  case fnnullsub_34: return;
  case fnCrocomire_Powerbomb: Crocomire_Powerbomb(); return;
  case fnCrocomireTongue_Init: CrocomireTongue_Init(); return;
  case fnCrocomireTongue_Main: CrocomireTongue_Main(); return;
  case fnEnemy_GrappleReact_NoInteract_A5: Enemy_GrappleReact_NoInteract_A5(); return;
  case fnEnemy_GrappleReact_CancelBeam_A5: Enemy_GrappleReact_CancelBeam_A5(); return;
  case fnEnemy_NormalTouchAI_A5: Enemy_NormalTouchAI_A5(); return;
  case fnEnemy_NormalShotAI_A5: Enemy_NormalShotAI_A5(); return;
  case fnEnemy_NormalFrozenAI_A5: Enemy_NormalFrozenAI_A5(); return;
  case fnnullsub_170_A5: return;
  case fnDraygon_Init: Draygon_Init(); return;
  case fnDraygon_Main: Draygon_Main(); return;
  case fnDraygon_Hurt: Draygon_Hurt(); return;
  case fnDraygon_Touch: Draygon_Touch(); return;
  case fnDraygon_Shot: Draygon_Shot(); return;
  case fnDraygon_Powerbomb: Draygon_Powerbomb(); return;
  case fnDraygonsEye_Init: DraygonsEye_Init(); return;
  case fnDraygonsEye_Main: DraygonsEye_Main(); return;
  case fnDraygonsTail_Init: DraygonsTail_Init(); return;
  case fnnullsub_37: return;
  case fnDraygonsArms_Init: DraygonsArms_Init(); return;
  case fnnullsub_38: return;
  case fnSporeSpawn_Init: SporeSpawn_Init(); return;
  case fnSporeSpawn_Main: SporeSpawn_Main(); return;
  case fnSporeSpawn_Shot: SporeSpawn_Shot(); return;
  case fnSporeSpawn_Touch: SporeSpawn_Touch(); return;
  case fnnullsub_39: return;
  case fnEnemy_GrappleReact_NoInteract_A6: Enemy_GrappleReact_NoInteract_A6(); return;
  case fnEnemy_GrappleReact_CancelBeam_A6: Enemy_GrappleReact_CancelBeam_A6(); return;
  case fnEnemy_NormalTouchAI_A6: Enemy_NormalTouchAI_A6(); return;
  case fnEnemy_NormalShotAI_A6: Enemy_NormalShotAI_A6(); return;
  case fnEnemy_NormalFrozenAI_A6: Enemy_NormalFrozenAI_A6(); return;
  case fnnullsub_170_A6: return;
  case fnBoulder_Init: Boulder_Init(); return;
  case fnBoulder_Main: Boulder_Main(); return;
  case fnSpikeyPlatform_Init: SpikeyPlatform_Init(); return;
  case fnSpikeyPlatform2ndEnemy_Init: SpikeyPlatform2ndEnemy_Init(); return;
  case fnSpikeyPlatform2ndEnemy_Main: SpikeyPlatform2ndEnemy_Main(); return;
  case fnSpikeyPlatform_Main: SpikeyPlatform_Main(); return;
  case fnFireGeyser_Init: FireGeyser_Init(); return;
  case fnFireGeyser_Main: FireGeyser_Main(); return;
  case fnNuclearWaffle_Init: NuclearWaffle_Init(); return;
  case fnNuclearWaffle_Main: NuclearWaffle_Main(); return;
  case fnFakeKraid_Init: FakeKraid_Init(); return;
  case fnFakeKraid_Main: FakeKraid_Main(); return;
  case fnFakeKraid_Touch: FakeKraid_Touch(); return;
  case fnFakeKraid_Shot: FakeKraid_Shot(); return;
  case fnCeresRidley_Init: CeresRidley_Init(); return;
  case fnCeresRidley_Main: CeresRidley_Main(); return;
  case fnCeresRidley_Hurt: CeresRidley_Hurt(); return;
  case fnRidley_Main: Ridley_Main(); return;
  case fnRidley_Func_2: Ridley_Func_2(); return;
  case fnRidley_Hurt: Ridley_Hurt(); return;
  case fnRidleysExplosion_Init: RidleysExplosion_Init(); return;
  case fnRidleysExplosion_Main: RidleysExplosion_Main(); return;
  case fnRidley_Shot: Ridley_Shot(); return;
  case fnRidley_Powerbomb: Ridley_Powerbomb(); return;
  case fnCeresSteam_Init: CeresSteam_Init(); return;
  case fnCeresSteam_Main: CeresSteam_Main(); return;
  case fnCeresSteam_Touch: CeresSteam_Touch(); return;
  case fnCeresDoor_Init: CeresDoor_Init(); return;
  case fnCeresDoor_Main: CeresDoor_Main(); return;
  case fnnullsub_41: return;
  case fnZebetites_Init: Zebetites_Init(); return;
  case fnZebetites_Main: Zebetites_Main(); return;
  case fnZebetites_Touch: Zebetites_Touch(); return;
  case fnZebetites_Shot: Zebetites_Shot(); return;
  case fnEnemy_GrappleReact_NoInteract_A7: Enemy_GrappleReact_NoInteract_A7(); return;
  case fnEnemy_GrappleReact_CancelBeam_A7: Enemy_GrappleReact_CancelBeam_A7(); return;
  case fnEnemy_NormalShotAI_A7: Enemy_NormalShotAI_A7(); return;
  case fnEnemy_NormalPowerBombAI_SkipDeathAnim_A7: Enemy_NormalPowerBombAI_SkipDeathAnim_A7(); return;
  case fnEnemy_NormalFrozenAI_A7: Enemy_NormalFrozenAI_A7(); return;
  case fnnullsub_170_A7: return;
  case fnnullsub_44: return;
  case fnKraidsArm_Touch: KraidsArm_Touch(); return;
  case fnKraid_Touch: Kraid_Touch(); return;
  case fnnullsub_43: return;
  case fnKraid_Init: Kraid_Init(); return;
  case fnKraidsArm_Init: KraidsArm_Init(); return;
  case fnKraidsTopLint_Init: KraidsTopLint_Init(); return;
  case fnKraidsMiddleLint_Init: KraidsMiddleLint_Init(); return;
  case fnKraidsBottomLint_Init: KraidsBottomLint_Init(); return;
  case fnKraidsFoot_Init: KraidsFoot_Init(); return;
  case fnKraid_Main: Kraid_Main(); return;
  case fnKraidsArm_Main: KraidsArm_Main(); return;
  case fnKraidsTopLint_Main: KraidsTopLint_Main(); return;
  case fnKraidsMiddleLint_Main: KraidsMiddleLint_Main(); return;
  case fnKraidsBottomLint_Main: KraidsBottomLint_Main(); return;
  case fnKraidsFoot_Main: KraidsFoot_Main(); return;
  case fnKraidsGoodFingernail_Touch: KraidsGoodFingernail_Touch(); return;
  case fnKraidsBadFingernail_Touch: KraidsBadFingernail_Touch(); return;
  case fnKraidsGoodFingernail_Init: KraidsGoodFingernail_Init(); return;
  case fnKraidsBadFingernail_Init: KraidsBadFingernail_Init(); return;
  case fnKraidsGoodFingernail_Main: KraidsGoodFingernail_Main(); return;
  case fnKraidsBadFingernail_Main: KraidsBadFingernail_Main(); return;
  case fnPhantoon_Init: Phantoon_Init(); return;
  case fnPhantoon2_Init: Phantoon2_Init(); return;
  case fnPhantoon_Main: Phantoon_Main(); return;
  case fnPhantoon_Hurt: Phantoon_Hurt(); return;
  case fnPhantoon_Touch: Phantoon_Touch(); return;
  case fnnullsub_358: return;
  case fnPhantoon_Shot: Phantoon_Shot(); return;
  case fnnullsub_45: return;
  case fnEtecoon_Init: Etecoon_Init(); return;
  case fnEtecoon_Main: Etecoon_Main(); return;
  case fnDachora_Init: Dachora_Init(); return;
  case fnDachora_Main: Dachora_Main(); return;
  case fnEnemy_GrappleReact_NoInteract_A8: Enemy_GrappleReact_NoInteract_A8(); return;
  case fnEnemy_GrappleReact_KillEnemy_A8: Enemy_GrappleReact_KillEnemy_A8(); return;
  case fnEnemy_GrappleReact_CancelBeam_A8: Enemy_GrappleReact_CancelBeam_A8(); return;
  case fnEnemy_GrappleReact_SamusLatchesNoInvinc_A8: Enemy_GrappleReact_SamusLatchesNoInvinc_A8(); return;
  case fnEnemy_GrappleReact_HurtSamus_A8: Enemy_GrappleReact_HurtSamus_A8(); return;
  case fnEnemy_NormalTouchAI_A8: Enemy_NormalTouchAI_A8(); return;
  case fnEnemy_NormalShotAI_A8: Enemy_NormalShotAI_A8(); return;
  case fnEnemy_NormalFrozenAI_A8: Enemy_NormalFrozenAI_A8(); return;
  case fnnullsub_170_A8: return;
  case fnMiniDraygon_Init: MiniDraygon_Init(); return;
  case fnEvirProjectile_Init: EvirProjectile_Init(); return;
  case fnMiniDraygon_Main: MiniDraygon_Main(); return;
  case fnEvirProjectile_Main: EvirProjectile_Main(); return;
  case fnMiniDraygon_Touch: MiniDraygon_Touch(); return;
  case fnMiniDraygon_Powerbomb: MiniDraygon_Powerbomb(); return;
  case fnMiniDraygon_Shot: MiniDraygon_Shot(); return;
  case fnMorphBallEye_Init: MorphBallEye_Init(); return;
  case fnMorphBallEye_Main: MorphBallEye_Main(); return;
  case fnFune_Init: Fune_Init(); return;
  case fnFune_Main: Fune_Main(); return;
  case fnWreckedShipGhost_Init: WreckedShipGhost_Init(); return;
  case fnWreckedShipGhost_Main: WreckedShipGhost_Main(); return;
  case fnYappingMaw_Init: YappingMaw_Init(); return;
  case fnYappingMaw_Main: YappingMaw_Main(); return;
  case fnYappingMaw_Touch: YappingMaw_Touch(); return;
  case fnYappingMaw_Shot: YappingMaw_Shot(); return;
  case fnYappingMaw_Frozen: YappingMaw_Frozen(); return;
  case fnKago_Init: Kago_Init(); return;
  case fnKago_Main: Kago_Main(); return;
  case fnKago_Shot: Kago_Shot(); return;
  case fnNorfairLavaMan_Init: NorfairLavaMan_Init(); return;
  case fnNorfairLavaMan_Main: NorfairLavaMan_Main(); return;
  case fnNorfairLavaMan_Powerbomb: NorfairLavaMan_Powerbomb(); return;
  case fnNorfairLavaMan_Touch: NorfairLavaMan_Touch(); return;
  case fnNorfairLavaMan_Shot: NorfairLavaMan_Shot(); return;
  case fnBeetom_Init: Beetom_Init(); return;
  case fnBeetom_Main: Beetom_Main(); return;
  case fnBeetom_Touch: Beetom_Touch(); return;
  case fnBeetom_Shot: Beetom_Shot(); return;
  case fnMaridiaFloater_Init: MaridiaFloater_Init(); return;
  case fnMaridiaFloater_Main: MaridiaFloater_Main(); return;
  case fnMaridiaFloater_Touch: MaridiaFloater_Touch(); return;
  case fnMaridiaFloater_Shot: MaridiaFloater_Shot(); return;
  case fnMaridiaFloater_Powerbomb: MaridiaFloater_Powerbomb(); return;
  case fnWreckedShipRobot_Init: WreckedShipRobot_Init(); return;
  case fnWreckedShipRobotDeactivated_Init: WreckedShipRobotDeactivated_Init(); return;
  case fnWreckedShipRobot_Main: WreckedShipRobot_Main(); return;
  case fnnullsub_342: return;
  case fnWreckedShipRobotDeactivated_Touch: WreckedShipRobotDeactivated_Touch(); return;
  case fnWreckedShipRobotDeactivated_Shot: WreckedShipRobotDeactivated_Shot(); return;
  case fnWreckedShipRobot_Shot: WreckedShipRobot_Shot(); return;
  case fnMaridiaPuffer_Init: MaridiaPuffer_Init(); return;
  case fnMaridiaPuffer_Main: MaridiaPuffer_Main(); return;
  case fnMaridiaPuffer_Shot: MaridiaPuffer_Shot(); return;
  case fnWalkingLavaSeahorse_Init: WalkingLavaSeahorse_Init(); return;
  case fnWalkingLavaSeahorse_Main: WalkingLavaSeahorse_Main(); return;
  case fnWreckedShipOrbs_Init: WreckedShipOrbs_Init(); return;
  case fnWreckedShipOrbs_Main: WreckedShipOrbs_Main(); return;
  case fnWreckedShipSpark_Init: WreckedShipSpark_Init(); return;
  case fnWreckedShipSpark_Main: WreckedShipSpark_Main(); return;
  case fnWreckedShipSpark_Shot: WreckedShipSpark_Shot(); return;
  case fnBlueBrinstarFaceBlock_Init: BlueBrinstarFaceBlock_Init(); return;
  case fnBlueBrinstarFaceBlock_Main: BlueBrinstarFaceBlock_Main(); return;
  case fnBlueBrinstarFaceBlock_Shot: BlueBrinstarFaceBlock_Shot(); return;
  case fnKiHunter_Init: KiHunter_Init(); return;
  case fnKiHunterWings_Init: KiHunterWings_Init(); return;
  case fnKiHunter_Main: KiHunter_Main(); return;
  case fnKiHunterWings_Main: KiHunterWings_Main(); return;
  case fnKiHunter_Shot: KiHunter_Shot(); return;
  case fnEnemy_GrappleReact_CancelBeam_A9: Enemy_GrappleReact_CancelBeam_A9(); return;
  case fnEnemy_NormalFrozenAI_A9: Enemy_NormalFrozenAI_A9(); return;
  case fnnullsub_170_A9: return;
  case fnMotherBrainsBody_Init: MotherBrainsBody_Init(); return;
  case fnMotherBrainsBrain_Init: MotherBrainsBrain_Init(); return;
  case fnMotherBrainsBody_Hurt: MotherBrainsBody_Hurt(); return;
  case fnMotherBrainsBody_Powerbomb: MotherBrainsBody_Powerbomb(); return;
  case fnMotherBrainsBrain_Hurt: MotherBrainsBrain_Hurt(); return;
  case fnMotherBrainsTubesFalling_Init: MotherBrainsTubesFalling_Init(); return;
  case fnMotherBrainsTubesFalling_Main: MotherBrainsTubesFalling_Main(cur_enemy_index); return;
  case fnMotherBrainsBody_Shot: MotherBrainsBody_Shot(); return;
  case fnMotherBrainsBrain_Shot: MotherBrainsBrain_Shot(); return;
  case fnnullsub_47: return;
  case fnMotherBrainsBrain_Touch: MotherBrainsBrain_Touch(); return;
  case fnShitroidInCutscene_Init: ShitroidInCutscene_Init(); return;
  case fnShitroidInCutscene_Main: ShitroidInCutscene_Main(); return;
  case fnShitroidInCutscene_Touch: ShitroidInCutscene_Touch(); return;
  case fnDeadTorizo_Init: DeadTorizo_Init(); return;
  case fnDeadTorizo_Main: DeadTorizo_Main(); return;
  case fnDeadTorizo_Powerbomb: DeadTorizo_Powerbomb(); return;
  case fnDeadTorizo_Shot: DeadTorizo_Shot(); return;
  case fnDeadSidehopper_Init: DeadSidehopper_Init(); return;
  case fnDeadZoomer_Init: DeadZoomer_Init(); return;
  case fnDeadRipper_Init: DeadRipper_Init(); return;
  case fnDeadSkree_Init: DeadSkree_Init(); return;
  case fnDeadSidehopper_Powerbomb: DeadSidehopper_Powerbomb(); return;
  case fnDeadSidehopper_Main: DeadSidehopper_Main(); return;
  case fnDeadZoomer_Powerbomb: DeadZoomer_Powerbomb(); return;
  case fnDeadZoomer_Shot: DeadZoomer_Shot(); return;
  case fnDeadRipper_Powerbomb: DeadRipper_Powerbomb(); return;
  case fnDeadRipper_Shot: DeadRipper_Shot(); return;
  case fnDeadSkree_Powerbomb: DeadSkree_Powerbomb(); return;
  case fnDeadSkree_Shot: DeadSkree_Shot(); return;
  case fnDeadSidehopper_Shot: DeadSidehopper_Shot(); return;
  case fnDeadSidehopper_Touch: DeadSidehopper_Touch(); return;
  case fnShitroid_Init: Shitroid_Init(); return;
  case fnShitroid_Powerbomb: Shitroid_Powerbomb(); return;
  case fnShitroid_Main: Shitroid_Main(); return;
  case fnShitroid_Touch: Shitroid_Touch(); return;
  case fnShitroid_Shot: Shitroid_Shot(); return;
  case fnEnemy_GrappleReact_CancelBeam_AA: Enemy_GrappleReact_CancelBeam_AA(); return;
  case fnEnemy_NormalFrozenAI_AA: Enemy_NormalFrozenAI_AA(); return;
  case fnnullsub_170_AA: return;
  case fnTorizo_Hurt: Torizo_Hurt(); return;
  case fnTorizo_Main: Torizo_Main(); return;
  case fnTorizo_Init: Torizo_Init(); return;
  case fnGoldTorizo_Touch: GoldTorizo_Touch(); return;
  case fnTorizo_Shot: Torizo_Shot(); return;
  case fnGoldTorizo_Main: GoldTorizo_Main(); return;
  case fnGoldTorizo_Hurt: GoldTorizo_Hurt(); return;
  case fnGoldTorizo_Shot: GoldTorizo_Shot(); return;
  case fnnullsub_49: return;
  case fnTourianEntranceStatue_Init: TourianEntranceStatue_Init(); return;
  case fnShaktool_Hurt: Shaktool_Hurt(); return;
  case fnShaktool_Init: Shaktool_Init(); return;
  case fnShaktool_Touch: Shaktool_Touch(); return;
  case fnShaktool_Shot: Shaktool_Shot(); return;
  case fnN00bTubeCracks_Init: N00bTubeCracks_Init(); return;
  case fnChozoStatue_Init: ChozoStatue_Init(); return;
  case fnChozoStatue_Main: ChozoStatue_Main(); return;
  case fnnullsub_51: return;
  case fnnullsub_52: return;
  case fnEnemy_GrappleReact_CancelBeam_B2: Enemy_GrappleReact_CancelBeam_B2(); return;
  case fnEnemy_NormalFrozenAI_B2: Enemy_NormalFrozenAI_B2(); return;
  case fnnullsub_170_B2: return;
  case fnWalkingSpacePirates_Powerbomb: WalkingSpacePirates_Powerbomb(); return;
  case fnWalkingSpacePirates_Touch: WalkingSpacePirates_Touch(); return;
  case fnWalkingSpacePirates_Shot: WalkingSpacePirates_Shot(); return;
  case fnWallSpacePirates_Init: WallSpacePirates_Init(); return;
  case fnWallSpacePirates_Main: WallSpacePirates_Main(); return;
  case fnNinjaSpacePirates_Init: NinjaSpacePirates_Init(); return;
  case fnNinjaSpacePirates_Main: NinjaSpacePirates_Main(); return;
  case fnWalkingSpacePirates_Init: WalkingSpacePirates_Init(); return;
  case fnWalkingSpacePirates_Main: WalkingSpacePirates_Main(); return;
  case fnEnemy_GrappleReact_NoInteract_B3: Enemy_GrappleReact_NoInteract_B3(); return;
  case fnEnemy_GrappleReact_KillEnemy_B3: Enemy_GrappleReact_KillEnemy_B3(); return;
  case fnEnemy_GrappleReact_CancelBeam_B3: Enemy_GrappleReact_CancelBeam_B3(); return;
  case fnEnemy_NormalTouchAI_B3: Enemy_NormalTouchAI_B3(); return;
  case fnEnemy_NormalShotAI_B3: Enemy_NormalShotAI_B3(); return;
  case fnEnemy_NormalFrozenAI_B3: Enemy_NormalFrozenAI_B3(); return;
  case fnnullsub_170_B3: return;
  case fnUnusedSpinningTurtleEye_Init: UnusedSpinningTurtleEye_Init(); return;
  case fnUnusedSpinningTurtleEye_Main: UnusedSpinningTurtleEye_Main(); return;
  case fnBrinstarPipeBug_Init: BrinstarPipeBug_Init(); return;
  case fnBrinstarPipeBug_Main: BrinstarPipeBug_Main(); return;
  case fnNorfairPipeBug_Init: NorfairPipeBug_Init(); return;
  case fnNorfairPipeBug_Main: NorfairPipeBug_Main(); return;
  case fnBrinstarYellowPipeBug_Init: BrinstarYellowPipeBug_Init(); return;
  case fnBrinstarYellowPipeBug_Main: BrinstarYellowPipeBug_Main(); return;
  case fnBotwoon_Init: Botwoon_Init(); return;
  case fnBotwoon_Main: Botwoon_Main(); return;
  case fnBotwoon_Touch: Botwoon_Touch(); return;
  case fnBotwoon_Shot: Botwoon_Shot(); return;
  case fnBotwoon_Powerbomb: Botwoon_Powerbomb(); return;
  case fnEscapeEtecoon_Main: EscapeEtecoon_Main(); return;
  case fnEscapeEtecoon_Init: EscapeEtecoon_Init(); return;
  case fnEscapeDachora_Init: EscapeDachora_Init(); return;
  case fnnullsub_54: return;
  case fnEnemy_NormalPowerBombAI_A0:
  case fnEnemy_NormalPowerBombAI_A2:
  case fnEnemy_NormalPowerBombAI_A3:
  case fnEnemy_NormalPowerBombAI_A4:
  case fnEnemy_NormalPowerBombAI_A5:
  case fnEnemy_NormalPowerBombAI_A6:
  case fnEnemy_NormalPowerBombAI_A7:
  case fnEnemy_NormalPowerBombAI_A8:
  case fnEnemy_NormalPowerBombAI_A9:
  case fnEnemy_NormalPowerBombAI_AA:
  case fnEnemy_NormalPowerBombAI_B2:
  case fnEnemy_NormalPowerBombAI_B3: NormalEnemyPowerBombAi(); return;
  default: Unreachable();
  }
}
void CallEnemyPreInstr(uint32 ea) {
  uint16 k = cur_enemy_index;
  switch (ea) {
  case fnnullsub_171: return;  // 0xa0807b
  case fnnullsub_171_A2: return;  // 0xa2807b
  case fnMiniCrocomire_PreInstr5: MiniCrocomire_PreInstr5(k); return;  // 0xa28a43
  case fnMiniCrocomire_PreInstr6: MiniCrocomire_PreInstr6(k); return;  // 0xa28a5c
  case fnnullsub_175: return;  // 0xa28a75
  case fnSpikeShootingPlant_2: SpikeShootingPlant_2(k); return;  // 0xa29fba
  case fnSpikeShootingPlant_3: SpikeShootingPlant_3(k); return;  // 0xa29fec
  case fnnullsub_182: return;  // 0xa2a01b
  case fnnullsub_187: return;  // 0xa2a7d7
  case fnGunshipTop_3: GunshipTop_3(k); return;  // 0xa2a80c
  case fnGunshipTop_4: GunshipTop_4(k); return;  // 0xa2a8d0
  case fnGunshipTop_5: GunshipTop_5(k); return;  // 0xa2a942
  case fnGunshipTop_6: GunshipTop_6(k); return;  // 0xa2a950
  case fnGunshipTop_7: GunshipTop_7(k); return;  // 0xa2a987
  case fnGunshipTop_8: GunshipTop_8(k); return;  // 0xa2a9bd
  case fnGunshipTop_9: GunshipTop_9(k); return;  // 0xa2aa4f
  case fnGunshipTop_10: GunshipTop_10(k); return;  // 0xa2aa5d
  case fnGunshipTop_11: GunshipTop_11(k); return;  // 0xa2aa94
  case fnGunshipTop_12: GunshipTop_12(k); return;  // 0xa2aaa2
  case fnGunshipTop_13: GunshipTop_13(k); return;  // 0xa2ab1f
  case fnGunshipTop_14: GunshipTop_14(k); return;  // 0xa2ab60
  case fnGunshipTop_15: GunshipTop_15(k); return;  // 0xa2ab6e
  case fnGunshipTop_16: GunshipTop_16(k); return;  // 0xa2aba5
  case fnGunshipTop_17: GunshipTop_17(k); return;  // 0xa2abc7
  case fnGunshipTop_18: GunshipTop_18(k); return;  // 0xa2ac1b
  case fnGunshipTop_19: GunshipTop_19(k); return;  // 0xa2acd7
  case fnGunshipTop_20: GunshipTop_20(k); return;  // 0xa2ad0e
  case fnGunshipTop_21: GunshipTop_21(k); return;  // 0xa2ad2d
  case fnFlies_4: Flies_4(k); return;  // 0xa2b14e
  case fnFlies_5: Flies_5(k); return;  // 0xa2b17c
  case fnFlies_6: Flies_6(k); return;  // 0xa2b1aa
  case fnFlies_7: Flies_7(k); return;  // 0xa2b1d2
  case fnNorfairLavajumpingEnemy_Func_1: NorfairLavajumpingEnemy_Func_1(k); return;  // 0xa2bedc
  case fnNorfairLavajumpingEnemy_Func_2: NorfairLavajumpingEnemy_Func_2(k); return;  // 0xa2bf1a
  case fnNorfairLavajumpingEnemy_Func_3: NorfairLavajumpingEnemy_Func_3(k); return;  // 0xa2bf3e
  case fnNorfairLavajumpingEnemy_Func_4: NorfairLavajumpingEnemy_Func_4(k); return;  // 0xa2bf7c
  case fnNorfairLavajumpingEnemy_Func_5: NorfairLavajumpingEnemy_Func_5(k); return;  // 0xa2bfbc
  case fnNorfairRio_Func_1: NorfairRio_Func_1(k); return;  // 0xa2c281
  case fnNorfairRio_Func_2: NorfairRio_Func_2(k); return;  // 0xa2c2e7
  case fnNorfairRio_Func_3: NorfairRio_Func_3(k); return;  // 0xa2c33f
  case fnNorfairRio_Func_4: NorfairRio_Func_4(k); return;  // 0xa2c361
  case fnNorfairRio_Func_5: NorfairRio_Func_5(k); return;  // 0xa2c3b1
  case fnNorfairRio_Func_6: NorfairRio_Func_6(k); return;  // 0xa2c406
  case fnLowerNorfairRio_Func_1: LowerNorfairRio_Func_1(k); return;  // 0xa2c72e
  case fnLowerNorfairRio_Func_2: LowerNorfairRio_Func_2(k); return;  // 0xa2c771
  case fnLowerNorfairRio_Func_3: LowerNorfairRio_Func_3(k); return;  // 0xa2c7bb
  case fnLowerNorfairRio_Func_4: LowerNorfairRio_Func_4(k); return;  // 0xa2c7d6
  case fnLowerNorfairRio_Func_5: LowerNorfairRio_Func_5(k); return;  // 0xa2c82d
  case fnLowerNorfairRio_Func_6: LowerNorfairRio_Func_6(k); return;  // 0xa2c888
  case fnMaridiaLargeSnail_Func_7: MaridiaLargeSnail_Func_7(k); return;  // 0xa2cf66
  case fnMaridiaLargeSnail_Func_8: MaridiaLargeSnail_Func_8(k); return;  // 0xa2cfa9
  case fnLavaSeahorse_Func_1: LavaSeahorse_Func_1(k); return;  // 0xa2e654
  case fnLavaSeahorse_Func_2: LavaSeahorse_Func_2(k); return;  // 0xa2e6ad
  case fnLavaSeahorse_Func_3: LavaSeahorse_Func_3(k); return;  // 0xa2e6f1
  case fnLavaSeahorse_Func_4: LavaSeahorse_Func_4(k); return;  // 0xa2e734
  case fnLavaSeahorse_Func_5: LavaSeahorse_Func_5(k); return;  // 0xa2e749
  case fnnullsub_196: return;  // 0xa2e781
  case fnBang_Func_6: Bang_Func_6(k); return;  // 0xa3bca5
  case fnBang_Func_7: Bang_Func_7(k); return;  // 0xa3bcc1
  case fnBang_Func_8: Bang_Func_8(k); return;  // 0xa3bcc5
  case fnBang_Func_10: Bang_Func_10(k); return;  // 0xa3bd1c
  case fnBang_Func_11: Bang_Func_11(k); return;  // 0xa3bd2c
  case fnnullsub_215: return;  // 0xa3cf5f
  case fnMaridiaSnail_Func_15: MaridiaSnail_Func_15(k); return;  // 0xa3d1b3
  case fnnullsub_343: return;  // 0xa3e08a
  case fnWreckedShipOrangeZoomer_Func_2: WreckedShipOrangeZoomer_Func_2(k); return;
  case fnnullsub_304: return;  // 0xa3e6c1
  case fnFireZoomer_Func_1: FireZoomer_Func_1(k); return;  // 0xa3e6c8
  case fnFireZoomer_Func_2: FireZoomer_Func_2(k); return;  // 0xa3e785
  case fnFireZoomer_Func_3: FireZoomer_Func_3(k); return;  // 0xa3e7f2
  case fnnullsub_237: return;  // 0xa7d4a8
  case fnPhantoon_Spawn8FireballsInCircleAtStart: Phantoon_Spawn8FireballsInCircleAtStart(k); return;  // 0xa7d4a9
  case fnPhantoon_WaitBetweenSpawningAndSpinningFireballs: Phantoon_WaitBetweenSpawningAndSpinningFireballs(k); return;  // 0xa7d4ee
  case fnPhantoon_SpawnFireballsBeforeFight: Phantoon_SpawnFireballsBeforeFight(k); return;  // 0xa7d508
  case fnPhantoon_WavyFadeIn: Phantoon_WavyFadeIn(k); return;  // 0xa7d54a
  case fnPhantoon_PickPatternForRound1: Phantoon_PickPatternForRound1(k); return;  // 0xa7d596
  case fnPhantoon_MovePhantoonInFigure8ThenOpenEye: Phantoon_MovePhantoonInFigure8ThenOpenEye(k); return;  // 0xa7d5e7
  case fnPhantoon_EyeFollowsSamusUntilTimerRunsOut: Phantoon_EyeFollowsSamusUntilTimerRunsOut(k); return;  // 0xa7d60d
  case fnPhantoon_BecomesSolidAndBodyVuln: Phantoon_BecomesSolidAndBodyVuln(k); return;  // 0xa7d65c
  case fnPhantoon_IsSwooping: Phantoon_IsSwooping(k); return;  // 0xa7d678
  case fnPhantoon_FadeoutWithSwoop: Phantoon_FadeoutWithSwoop(k); return;  // 0xa7d6b9
  case fnPhantoon_WaitAfterFadeOut: Phantoon_WaitAfterFadeOut(k); return;  // 0xa7d6d4
  case fnPhantoon_MoveLeftOrRightAndPickEyeOpenPatt: Phantoon_MoveLeftOrRightAndPickEyeOpenPatt(k); return;  // 0xa7d6e2
  case fnPhantoon_FadeInBeforeFigure8: Phantoon_FadeInBeforeFigure8(k); return;  // 0xa7d72d
  case fnPhantoon_BecomeSolidAfterRainingFireballs: Phantoon_BecomeSolidAfterRainingFireballs(k); return;  // 0xa7d73f
  case fnPhantoon_FadeInDuringFireballRain: Phantoon_FadeInDuringFireballRain(k); return;  // 0xa7d767
  case fnPhantoon_FollowSamusWithEyeDuringFireballRain: Phantoon_FollowSamusWithEyeDuringFireballRain(k); return;  // 0xa7d788
  case fnPhantoon_FadeOutDuringFireballRain: Phantoon_FadeOutDuringFireballRain(k); return;  // 0xa7d7d5
  case fnPhantoon_SpawnRainingFireballs: Phantoon_SpawnRainingFireballs(k); return;  // 0xa7d7f7
  case fnPhantoon_FadeOutBeforeFirstFireballRain: Phantoon_FadeOutBeforeFirstFireballRain(k); return;  // 0xa7d82a
  case fnPhantoon_FadeOutBeforeEnrage: Phantoon_FadeOutBeforeEnrage(k); return;  // 0xa7d85c
  case fnPhantoon_MoveEnragedPhantoonToTopCenter: Phantoon_MoveEnragedPhantoonToTopCenter(k); return;  // 0xa7d874
  case fnPhantoon_FadeInEnragedPhantoon: Phantoon_FadeInEnragedPhantoon(k); return;  // 0xa7d891
  case fnPhantoon_Enraged: Phantoon_Enraged(k); return;  // 0xa7d8ac
  case fnPhantoon_FadeoutAfterEnrage: Phantoon_FadeoutAfterEnrage(k); return;  // 0xa7d916
  case fnPhantoon_CompleteSwoopAfterFatalShot: Phantoon_CompleteSwoopAfterFatalShot(k); return;  // 0xa7d92e
  case fnPhantoon_DyingPhantoonFadeInOut: Phantoon_DyingPhantoonFadeInOut(k); return;  // 0xa7d948
  case fnPhantoon_DyingPhantoonExplosions: Phantoon_DyingPhantoonExplosions(k); return;  // 0xa7d98b
  case fnPhantoon_WavyDyingPhantoonAndCry: Phantoon_WavyDyingPhantoonAndCry(k); return;  // 0xa7da51
  case fnPhantoon_DyingFadeOut: Phantoon_DyingFadeOut(k); return;  // 0xa7da86
  case fnPhantoon_AlmostDead: Phantoon_AlmostDead(k); return;  // 0xa7dad7
  case fnPhantoon_Dead: Phantoon_Dead(k); return;  // 0xa7db3d
  case fnEtecoon_Func_4: Etecoon_Func_4(k); return;  // 0xa7e9af
  case fnEtecoon_Func_5: Etecoon_Func_5(k); return;  // 0xa7ea00
  case fnEtecoon_Func_6: Etecoon_Func_6(k); return;  // 0xa7ea37
  case fnEtecoon_Func_7: Etecoon_Func_7(k); return;  // 0xa7eab5
  case fnEtecoon_Func_8: Etecoon_Func_8(k); return;  // 0xa7eb02
  case fnEtecoon_Func_9: Etecoon_Func_9(k); return;  // 0xa7eb2c
  case fnEtecoon_Func_10: Etecoon_Func_10(k); return;  // 0xa7eb50
  case fnEtecoon_Func_11: Etecoon_Func_11(k); return;  // 0xa7ebcd
  case fnEtecoon_Func_12: Etecoon_Func_12(k); return;  // 0xa7ec1b
  case fnEtecoon_Func_16: Etecoon_Func_16(k); return;  // 0xa7ec97
  case fnEtecoon_Func_17: Etecoon_Func_17(k); return;  // 0xa7ecbb
  case fnEtecoon_Func_18: Etecoon_Func_18(k); return;  // 0xa7ecdf
  case fnEtecoon_Func_19: Etecoon_Func_19(k); return;  // 0xa7ed09
  case fnEtecoon_Func_20: Etecoon_Func_20(k); return;  // 0xa7ed2a
  case fnEtecoon_Func_21: Etecoon_Func_21(k); return;  // 0xa7ed54
  case fnEtecoon_Func_22: Etecoon_Func_22(k); return;  // 0xa7ed75
  case fnEtecoon_Func_23: Etecoon_Func_23(k); return;  // 0xa7edc7
  case fnEtecoon_Func_24: Etecoon_Func_24(k); return;  // 0xa7ee3e
  case fnEtecoon_Func_25: Etecoon_Func_25(k); return;  // 0xa7ee9a
  case fnEtecoon_Func_26: Etecoon_Func_26(k); return;  // 0xa7eeb8
  case fnDachora_Func_2: Dachora_Func_2(k); return;  // 0xa7f570
  case fnDachora_Func_3: Dachora_Func_3(k); return;  // 0xa7f5bc
  case fnDachora_Func_4: Dachora_Func_4(k); return;  // 0xa7f5ed
  case fnDachora_Func_5: Dachora_Func_5(k); return;  // 0xa7f65e
  case fnDachora_Func_7: Dachora_Func_7(k); return;  // 0xa7f78f
  case fnDachora_Func_8: Dachora_Func_8(k); return;  // 0xa7f806
  case fnDachora_Func_11: Dachora_Func_11(k); return;  // 0xa7f935
  case fnDachora_Func_12: Dachora_Func_12(k); return;  // 0xa7f98c
  case fnMorphBallEye_Func_1: MorphBallEye_Func_1(k); return;  // 0xa890f1
  case fnMorphBallEye_Func_2: MorphBallEye_Func_2(k); return;  // 0xa8912e
  case fnMorphBallEye_Func_3: MorphBallEye_Func_3(k); return;  // 0xa89160
  case fnMorphBallEye_Func_4: MorphBallEye_Func_4(k); return;  // 0xa891ce
  case fnnullsub_244: return;  // 0xa891dc
  case fnNorfairLavaMan_Func_7: NorfairLavaMan_Func_7(k); return;  // 0xa8b11a
  case fnNorfairLavaMan_Func_8: NorfairLavaMan_Func_8(k); return;  // 0xa8b175
  case fnsub_A8B193: sub_A8B193(k); return;  // 0xa8b193
  case fnNorfairLavaMan_Func_9: NorfairLavaMan_Func_9(k); return;  // 0xa8b1b8
  case fnNorfairLavaMan_Func_10: NorfairLavaMan_Func_10(k); return;  // 0xa8b1dd
  case fnNorfairLavaMan_Func_11: NorfairLavaMan_Func_11(k); return;  // 0xa8b204
  case fnsub_A8B291: sub_A8B291(k); return;  // 0xa8b291
  case fnNorfairLavaMan_Func_13: NorfairLavaMan_Func_13(); return;
  case fnNorfairLavaMan_Func_15: NorfairLavaMan_Func_15(k); return;  // 0xa8b30d
  case fnNorfairLavaMan_Func_16: NorfairLavaMan_Func_16(k); return;  // 0xa8b31f
  case fnNorfairLavaMan_Func_17: NorfairLavaMan_Func_17(k); return;  // 0xa8b356
  case fnNorfairLavaMan_Func_18: NorfairLavaMan_Func_18(k); return;  // 0xa8b3a7
  case fnMaridiaFloater_Func_3: MaridiaFloater_Func_3(k); return;  // 0xa8c283
  case fnMaridiaFloater_Func_4: MaridiaFloater_Func_4(k); return;  // 0xa8c2a6
  case fnMaridiaFloater_Func_5: MaridiaFloater_Func_5(k); return;  // 0xa8c2cf
  case fnMaridiaFloater_Func_6: MaridiaFloater_Func_6(k); return;  // 0xa8c36b
  case fnMaridiaFloater_Func_7: MaridiaFloater_Func_7(k); return;  // 0xa8c3e1
  case fnMaridiaFloater_Func_8: MaridiaFloater_Func_8(k); return;  // 0xa8c469
  case fnMaridiaFloater_Func_9: MaridiaFloater_Func_9(k); return;  // 0xa8c4dc
  case fnMaridiaFloater_Func_10: MaridiaFloater_Func_10(k); return;  // 0xa8c500
  case fnMaridiaFloater_Func_11: MaridiaFloater_Func_11(k); return;  // 0xa8c51d
  case fnnullsub_256: return;  // 0xa8c568
  case fnMaridiaFloater_Func_12: MaridiaFloater_Func_12(k); return;  // 0xa8c569
  case fnMaridiaFloater_Func_13: MaridiaFloater_Func_13(k); return;  // 0xa8c59f
  case fnnullsub_344: return;  // 0xaac95e
  case fnnullsub_274: return;  // 0xaadcaa
  case fnnullsub_276: return;  // 0xaae7a6
  case fnnullsub_171_AA: return;  // 0xaa807b
  case fnnullsub_171_B3: return;  // 0xb3807b
  case fnBrinstarPipeBug_PreInstr_1: BrinstarPipeBug_PreInstr_1(k); return;  // 0xb38880
  case fnBrinstarPipeBug_PreInstr_2: BrinstarPipeBug_PreInstr_2(k); return;  // 0xb38890
  case fnBrinstarPipeBug_PreInstr_3: BrinstarPipeBug_PreInstr_3(k); return;  // 0xb388e3
  case fnBrinstarPipeBug_PreInstr_4: BrinstarPipeBug_PreInstr_4(k); return;  // 0xb3891c
  case fnBrinstarPipeBug_PreInstr_5: BrinstarPipeBug_PreInstr_5(k); return;  // 0xb3897e
  case fnNorfairPipeBug_Func_5: NorfairPipeBug_Func_5(k); return;  // 0xb38cff
  case fnBotwoon_Func_26: Botwoon_Func_26(k); return;  // 0xb39dc0
  case fnBotwoon_Func_27: Botwoon_Func_27(k); return;  // 0xb39e7d
  case fnBotwoon_Func_28: Botwoon_Func_28(k); return;  // 0xb39ee0
  case fnBotwoon_Func_29: Botwoon_Func_29(k); return;  // 0xb39f34
  case fnBotwoon_Func_30: Botwoon_Func_30(k); return;  // 0xb39f7a
  case fnTorizo_Func_5: Torizo_Func_5(k); return;
  case fnTorizo_Func_6: Torizo_Func_6(k); return;
  case fnTorizo_Func_7: Torizo_Func_7(k); return;
  case fnTorizo_D5ED: Torizo_D5ED(k); return;
  case fnTorizo_D5F1: Torizo_D5F1(k); return;
  case fnsub_AAE445: sub_AAE445(k); return;
  case fnShaktool_PreInstr_0: Shaktool_PreInstr_0(k); return;
  case fnnullsub_277: return;
  case fnEscapeEtecoon_E65C: EscapeEtecoon_E65C(k); return;
  case fnEscapeEtecoon_E670: EscapeEtecoon_E670(k); return;
  case fnEscapeEtecoon_E680: EscapeEtecoon_E680(k); return;
  case fnsub_A3E168: sub_A3E168(k); return;
  case fnMaridiaSnail_Func_7: MaridiaSnail_Func_7(k); return;
  case fnMaridiaSnail_Func_9: MaridiaSnail_Func_9(k); return;
  case fnMaridiaSnail_CFB7: MaridiaSnail_CFB7(k); return;
  case fnMaridiaSnail_CFBD: MaridiaSnail_CFBD(k); return;
  case fnMaridiaSnail_CFCE: MaridiaSnail_CFCE(k); return;
  case fnMaridiaSnail_CFD4: MaridiaSnail_CFD4(k); return;
  case fnMaridiaSnail_CFE5: MaridiaSnail_CFE5(k); return;
  case fnMaridiaSnail_CFEB: MaridiaSnail_CFEB(k); return;
  case fnMaridiaSnail_CFFC: MaridiaSnail_CFFC(k); return;
  case fnnullsub_275: return;
  case fnShaktool_DCAC: Shaktool_DCAC(k); return;
  case fnShaktool_DCD7: Shaktool_DCD7(k); return;
  case fnShaktool_DD25: Shaktool_DD25(k); return;
  default: Unreachable();
  }
}

const uint16 *CallEnemyInstr(uint32 ea, uint16 k, const uint16 *j) {
  switch (ea) {
  case fnEnemyInstr_Goto_A2: return EnemyInstr_Goto(k, j);
  case fnEnemyInstr_DecTimerAndGoto2_A2: return EnemyInstr_DecTimerAndGoto(k, j);
  case fnEnemyInstr_SetTimer_A2: return EnemyInstr_SetTimer(k, j);
  case fnEnemyInstr_Sleep_A2: return EnemyInstr_Sleep(k, j);
  case fnEnemyInstr_EnableOffScreenProcessing_A2: return EnemyInstr_EnableOffScreenProcessing(k, j);
  case fnEnemyInstr_DisableOffScreenProcessing_A2: return EnemyInstr_DisableOffScreenProcessing(k, j);
  case fnBouncingGoofball_Instr_88C5: return BouncingGoofball_Instr_88C5(k, j);
  case fnBouncingGoofball_Instr_88C6: return BouncingGoofball_Instr_88C6(k, j);
  case fnMiniCrocomire_Instr_897E: return MiniCrocomire_Instr_897E(k, j);
  case fnMiniCrocomire_Instr_8990: return MiniCrocomire_Instr_8990(k, j);
  case fnMiniCrocomire_Instr_899D: return MiniCrocomire_Instr_899D(k, j);
  case fnMaridiaBeybladeTurtle_Instr_9381: return MaridiaBeybladeTurtle_Instr_9381(k, j);
  case fnMaridiaBeybladeTurtle_Instr_9412: return MaridiaBeybladeTurtle_Instr_9412(k, j);
  case fnMaridiaBeybladeTurtle_Instr_9447: return MaridiaBeybladeTurtle_Instr_9447(k, j);
  case fnMaridiaBeybladeTurtle_Instr_9451: return MaridiaBeybladeTurtle_Instr_9451(k, j);
  case fnMaridiaBeybladeTurtle_Instr_946B: return MaridiaBeybladeTurtle_Instr_946B(k, j);
  case fnMaridiaBeybladeTurtle_Instr_9485: return MaridiaBeybladeTurtle_Instr_9485(k, j);
  case fnMaridiaBeybladeTurtle_Instr_94A1: return MaridiaBeybladeTurtle_Instr_94A1(k, j);
  case fnMaridiaBeybladeTurtle_Instr_94C7: return MaridiaBeybladeTurtle_Instr_94C7(k, j);
  case fnMaridiaBeybladeTurtle_Instr_94D1: return MaridiaBeybladeTurtle_Instr_94D1(k, j);
  case fnSpikeShootingPlant_Instr_9F2A: return SpikeShootingPlant_Instr_9F2A(k, j);
  case fnSpikeShootingPlant_Instr_A095: return SpikeShootingPlant_Instr_A095(k, j);
  case fnSpikeShootingPlant_Instr_A0A7: return SpikeShootingPlant_Instr_A0A7(k, j);
  case fnMaridiaSpikeyShell_Instr_A56D: return MaridiaSpikeyShell_Instr_A56D(k, j);
  case fnMaridiaSpikeyShell_Instr_A571: return MaridiaSpikeyShell_Instr_A571(k, j);
  case fnRinka_Instr_B9B3: return Rinka_Instr_B9B3(k, j);
  case fnRinka_Instr_B9BD: return Rinka_Instr_B9BD(k, j);
  case fnRinka_Instr_B9C7: return Rinka_Instr_B9C7(k, j);
  case fnEnemyInstr_Rio_Instr_1: return EnemyInstr_Rio_Instr_1(k, j);
  case fnNorfairLavajumpingEnemy_Instr_BE8E: return NorfairLavajumpingEnemy_Instr_BE8E(k, j);
  case fnNorfairRio_Instr_C1C9: return NorfairRio_Instr_C1C9(k, j);
  case fnNorfairRio_Instr_C1D4: return NorfairRio_Instr_C1D4(k, j);
  case fnNorfairRio_Instr_C1DF: return NorfairRio_Instr_C1DF(k, j);
  case fnNorfairRio_Instr_C1EA: return NorfairRio_Instr_C1EA(k, j);
  case fnNorfairRio_Instr_C1F5: return NorfairRio_Instr_C1F5(k, j);
  case fnNorfairRio_Instr_C200: return NorfairRio_Instr_C200(k, j);
  case fnNorfairRio_Instr_C20B: return NorfairRio_Instr_C20B(k, j);
  case fnNorfairRio_Instr_C216: return NorfairRio_Instr_C216(k, j);
  case fnNorfairRio_Instr_C221: return NorfairRio_Instr_C221(k, j);
  case fnNorfairRio_Instr_C22C: return NorfairRio_Instr_C22C(k, j);
  case fnNorfairRio_Instr_C237: return NorfairRio_Instr_C237(k, j);
  case fnLowerNorfairRio_Instr_C6D2: return LowerNorfairRio_Instr_C6D2(k, j);
  case fnLowerNorfairRio_Instr_C6DD: return LowerNorfairRio_Instr_C6DD(k, j);
  case fnLowerNorfairRio_Instr_C6E8: return LowerNorfairRio_Instr_C6E8(k, j);
  case fnMaridiaLargeSnail_Instr_CB6B: return MaridiaLargeSnail_Instr_CB6B(k, j);
  case fnMaridiaLargeSnail_Instr_CCB3: return MaridiaLargeSnail_Instr_CCB3(k, j);
  case fnMaridiaLargeSnail_Instr_CCBE: return MaridiaLargeSnail_Instr_CCBE(k, j);
  case fnMaridiaLargeSnail_Instr_CCC9: return MaridiaLargeSnail_Instr_CCC9(k, j);
  case fnLavaSeahorse_Instr_E5FB: return LavaSeahorse_Instr_E5FB(k, j);
  case fnEnemyInstr_Goto_A3: return EnemyInstr_Goto(k, j);
  case fnEnemyInstr_Sleep_A3: return EnemyInstr_Sleep(k, j);
  case fnEnemyInstr_EnableOffScreenProcessing_A3: return EnemyInstr_EnableOffScreenProcessing(k, j);
  case fnEnemyInstr_DisableOffScreenProcessing_A3: return EnemyInstr_DisableOffScreenProcessing(k, j);
  case fnWaver_Instr_1: return Waver_Instr_1(k, j);
  case fnMetalee_Instr_1: return Metalee_Instr_1(k, j);
  case fnMaridiaFish_Instr_3: return MaridiaFish_Instr_3(k, j);
  case fnMaridiaFish_Instr_1: return MaridiaFish_Instr_1(k, j);
  case fnMaridiaFish_Instr_2: return MaridiaFish_Instr_2(k, j);
  case fnPlatformThatFallsWithSamus_Instr_3: return PlatformThatFallsWithSamus_Instr_3(k, j);
  case fnPlatformThatFallsWithSamus_Instr_4: return PlatformThatFallsWithSamus_Instr_4(k, j);
  case fnPlatformThatFallsWithSamus_Instr_1: return PlatformThatFallsWithSamus_Instr_1(k, j);
  case fnPlatformThatFallsWithSamus_Instr_2: return PlatformThatFallsWithSamus_Instr_2(k, j);
  case fnSidehopper_Func_1: return Sidehopper_Func_1(k, j);
  case fnSidehopper_Instr_1: return Sidehopper_Instr_1(k, j);
  case fnMaridiaRefillCandy_Instr_1: return MaridiaRefillCandy_Instr_1(k, j);
  case fnMaridiaRefillCandy_Instr_2: return MaridiaRefillCandy_Instr_2(k, j);
  case fnMaridiaRefillCandy_Instr_3: return MaridiaRefillCandy_Instr_3(k, j);
  case fnBang_Instr_1: return Bang_Instr_1(k, j);
  case fnBang_Instr_2: return Bang_Instr_2(k, j);
  case fnSkree_Instr_1: return Skree_Instr_1(k, j);
  case fnMaridiaSnail_Instr_1: return MaridiaSnail_Instr_1(k, j);
  case fnMaridiaSnail_Instr_2: return MaridiaSnail_Instr_2(k, j);
  case fnMaridiaSnail_Instr_4: return MaridiaSnail_Instr_4(k, j);
  case fnMaridiaSnail_Instr_3: return MaridiaSnail_Instr_3(k, j);
  case fnMaridiaSnail_Instr_5: return MaridiaSnail_Instr_5(k, j);
  case fnReflec_Instr_1: return Reflec_Instr_1(k, j);
  case fnWreckedShipOrangeZoomer_Func_1: return WreckedShipOrangeZoomer_Func_1(k, j);
  case fnZoomer_Instr_SetPreinstr: return Zoomer_Instr_SetPreinstr(k, j);
  case fnMetroid_Instr_2: return Metroid_Instr_2(k, j);
  case fnMetroid_Instr_1: return Metroid_Instr_1(k, j);
  case fnEnemyInstr_Goto_A4: return EnemyInstr_Goto(k, j);
  case fnEnemyInstr_Sleep_A4: return EnemyInstr_Sleep(k, j);
  case fnCrocomire_Instr_1: return Crocomire_Instr_1(k, j);
  case fnCrocomire_Instr_14: return Crocomire_Instr_14(k, j);
  case fnCrocomire_Instr_11: return Crocomire_Instr_11(k, j);
  case fnCrocomire_Instr_7: return Crocomire_Instr_7(k, j);
  case fnCrocomire_Instr_19: return Crocomire_Instr_19(k, j);
  case fnCrocomire_Instr_2: return Crocomire_Instr_2(k, j);
  case fnCrocomire_Instr_4: return Crocomire_Instr_4(k, j);
  case fnCrocomire_Instr_3: return Crocomire_Instr_3(k, j);
  case fnCrocomire_Instr_15: return Crocomire_Instr_15(k, j);
  case fnCrocomire_Instr_16: return Crocomire_Instr_16(k, j);
  case fnCrocomire_Instr_13: return Crocomire_Instr_13(k, j);
  case fnCrocomire_Instr_18: return Crocomire_Instr_18(k, j);
  case fnCrocomire_Instr_12: return Crocomire_Instr_12(k, j);
  case fnCrocomire_Instr_17: return Crocomire_Instr_17(k, j);
  case fnCrocomire_Instr_8: return Crocomire_Instr_8(k, j);
  case fnCrocomire_Instr_6: return Crocomire_Instr_6(k, j);
  case fnCrocomire_Instr_9: return Crocomire_Instr_9(k, j);
  case fnCrocomire_Instr_5: return Crocomire_Instr_5(k, j);
  case fnCrocomire_Instr_20: return Crocomire_Instr_20(k, j);
  case fnCrocomire_Instr_21: return Crocomire_Instr_21(k, j);
  case fnCrocomire_Instr_22: return Crocomire_Instr_22(k, j);
  case fnCrocomire_Instr_23: return Crocomire_Instr_23(k, j);
  case fnCrocomire_Instr_24: return Crocomire_Instr_24(k, j);
  case fnCrocomire_Instr_10: return Crocomire_Instr_10(k, j);
  case fnCrocomire_Instr_25: return Crocomire_Instr_25(k, j);
  case fnCrocomire_Instr_26: return Crocomire_Instr_26(k, j);
  case fnCrocomire_Instr_27: return Crocomire_Instr_27(k, j);
  case fnEnemyInstr_StopScript_A5: return EnemyInstr_StopScript(k, j);
  case fnEnemyInstr_Goto_A5: return EnemyInstr_Goto(k, j);
  case fnEnemyInstr_DecTimerAndGoto2_A5: return EnemyInstr_DecTimerAndGoto(k, j);
  case fnEnemyInstr_SetTimer_A5: return EnemyInstr_SetTimer(k, j);
  case fnEnemyInstr_Sleep_A5: return EnemyInstr_Sleep(k, j);
  case fnEnemyInstr_WaitNframes_A5: return EnemyInstr_WaitNframes(k, j);
  case fnDraygon_Instr_1: return Draygon_Instr_1(k, j);
  case fnDraygon_Instr_13: return Draygon_Instr_13(k, j);
  case fnDraygon_Instr_8: return Draygon_Instr_8(k, j);
  case fnDraygon_Instr_7: return Draygon_Instr_7(k, j);
  case fnDraygon_Instr_6: return Draygon_Instr_6(k, j);
  case fnDraygon_Instr_9: return Draygon_Instr_9(k, j);
  case fnDraygon_Instr_2: return Draygon_Instr_2(k, j);
  case fnDraygon_Instr_11: return Draygon_Instr_11(k, j);
  case fnDraygon_Instr_5: return Draygon_Instr_5(k, j);
  case fnDraygon_Instr_15: return Draygon_Instr_15(k, j);
  case fnDraygon_Instr_17: return Draygon_Instr_17(k, j);
  case fnDraygon_Instr_14: return Draygon_Instr_14(k, j);
  case fnDraygon_Instr_16: return Draygon_Instr_16(k, j);
  case fnDraygon_Instr_10: return Draygon_Instr_10(k, j);
  case fnDraygon_Instr_4: return Draygon_Instr_4(k, j);
  case fnDraygon_Instr_12: return Draygon_Instr_12(k, j);
  case fnDraygon_Instr_18: return Draygon_Instr_18(k, j);
  case fnDraygon_Instr_3: return Draygon_Instr_3(k, j);
  case fnDraygon_Instr_25: return Draygon_Instr_25(k, j);
  case fnDraygon_Instr_24: return Draygon_Instr_24(k, j);
  case fnDraygon_Instr_21: return Draygon_Instr_21(k, j);
  case fnDraygon_Instr_22: return Draygon_Instr_22(k, j);
  case fnDraygon_Instr_27: return Draygon_Instr_27(k, j);
  case fnDraygon_Instr_23: return Draygon_Instr_23(k, j);
  case fnDraygon_Instr_30: return Draygon_Instr_30(k, j);
  case fnDraygon_Instr_20: return Draygon_Instr_20(k, j);
  case fnDraygon_Instr_29: return Draygon_Instr_29(k, j);
  case fnDraygon_Instr_19: return Draygon_Instr_19(k, j);
  case fnDraygon_Instr_28: return Draygon_Instr_28(k, j);
  case fnDraygon_Instr_26: return Draygon_Instr_26(k, j);
  case fnEnemyInstr_Goto_A6: return EnemyInstr_Goto(k, j);
  case fnEnemyInstr_Sleep_A6: return EnemyInstr_Sleep(k, j);
  case fnFireGeyser_Instr_1: return FireGeyser_Instr_1(k, j);
  case fnFireGeyser_Instr_2: return FireGeyser_Instr_2(k, j);
  case fnFireGeyser_Instr_3: return FireGeyser_Instr_3(k, j);
  case fnFireGeyser_Instr_4: return FireGeyser_Instr_4(k, j);
  case fnFireGeyser_Instr_5: return FireGeyser_Instr_5(k, j);
  case fnFireGeyser_Instr_6: return FireGeyser_Instr_6(k, j);
  case fnFireGeyser_Instr_7: return FireGeyser_Instr_7(k, j);
  case fnFireGeyser_Instr_8: return FireGeyser_Instr_8(k, j);
  case fnFireGeyser_Instr_9: return FireGeyser_Instr_9(k, j);
  case fnFireGeyser_Instr_10: return FireGeyser_Instr_10(k, j);
  case fnFireGeyser_Instr_11: return FireGeyser_Instr_11(k, j);
  case fnFireGeyser_Instr_12: return FireGeyser_Instr_12(k, j);
  case fnFireGeyser_Instr_13: return FireGeyser_Instr_13(k, j);
  case fnFireGeyser_Instr_14: return FireGeyser_Instr_14(k, j);
  case fnFireGeyser_Instr_15: return FireGeyser_Instr_15(k, j);
  case fnFireGeyser_Instr_16: return FireGeyser_Instr_16(k, j);
  case fnFireGeyser_Instr_17: return FireGeyser_Instr_17(k, j);
  case fnFireGeyser_Instr_18: return FireGeyser_Instr_18(k, j);
  case fnFireGeyser_Instr_19: return FireGeyser_Instr_19(k, j);
  case fnFireGeyser_Instr_20: return FireGeyser_Instr_20(k, j);
  case fnFireGeyser_Instr_21: return FireGeyser_Instr_21(k, j);
  case fnFireGeyser_Instr_22: return FireGeyser_Instr_22(k, j);
  case fnFireGeyser_Instr_23: return FireGeyser_Instr_23(k, j);
  case fnFireGeyser_Instr_24: return FireGeyser_Instr_24(k, j);
  case fnFakeKraid_Instr_2: return FakeKraid_Instr_2(k, j);
  case fnFakeKraid_Instr_1: return FakeKraid_Instr_1(k, j);
  case fnFakeKraid_Instr_3: return FakeKraid_Instr_3(k, j);
  case fnFakeKraid_Instr_4: return FakeKraid_Instr_4(k, j);
  case fnFakeKraid_Instr_5: return FakeKraid_Instr_5(k, j);
  case fnRidley_Instr_5: return Ridley_Instr_5(k, j);
  case fnRidley_Instr_6: return Ridley_Instr_6(k, j);
  case fnRidley_Instr_10: return Ridley_Instr_10(k, j);
  case fnRidley_Instr_4: return Ridley_Instr_4(k, j);
  case fnRidley_Instr_3: return Ridley_Instr_3(k, j);
  case fnRidley_Instr_2: return Ridley_Instr_2(k, j);
  case fnRidley_Instr_1: return Ridley_Instr_1(k, j);
  case fnRidley_Instr_14: return Ridley_Instr_14(k, j);
  case fnRidley_Instr_9: return Ridley_Instr_9(k, j);
  case fnRidley_Instr_7: return Ridley_Instr_7(k, j);
  case fnRidley_Instr_8: return Ridley_Instr_8(k, j);
  case fnRidley_Instr_11: return Ridley_Instr_11(k, j);
  case fnRidley_Instr_12: return Ridley_Instr_12(k, j);
  case fnRidley_Instr_13: return Ridley_Instr_13(k, j);
  case fnRidley_Instr_15: return Ridley_Instr_15(k, j);
  case fnRidley_Instr_16: return Ridley_Instr_16(k, j);
  case fnCeresSteam_Instr_1: return CeresSteam_Instr_1(k, j);
  case fnCeresSteam_Instr_2: return CeresSteam_Instr_2(k, j);
  case fnCeresSteam_Instr_3: return CeresSteam_Instr_3(k, j);
  case fnCeresDoor_Instr_6: return CeresDoor_Instr_6(k, j);
  case fnCeresDoor_Instr_4: return CeresDoor_Instr_4(k, j);
  case fnCeresDoor_Instr_8: return CeresDoor_Instr_8(k, j);
  case fnCeresSteam_Instr_4: return CeresSteam_Instr_4(k, j);
  case fnCeresDoor_Instr_1: return CeresDoor_Instr_1(k, j);
  case fnCeresDoor_Instr_3: return CeresDoor_Instr_3(k, j);
  case fnCeresSteam_Instr_5: return CeresSteam_Instr_5(k, j);
  case fnCeresDoor_Instr_5: return CeresDoor_Instr_5(k, j);
  case fnCeresDoor_Instr_2: return CeresDoor_Instr_2(k, j);
  case fnCeresDoor_Instr_7: return CeresDoor_Instr_7(k, j);
  case fnEnemyInstr_Call_A7: return EnemyInstr_Call_A7(k, j);
  case fnEnemyInstr_Goto_A7: return EnemyInstr_Goto(k, j);
  case fnEnemyInstr_DecTimerAndGoto2_A7: return EnemyInstr_DecTimerAndGoto(k, j);
  case fnEnemyInstr_SetTimer_A7: return EnemyInstr_SetTimer(k, j);
  case fnEnemyInstr_Sleep_A7: return EnemyInstr_Sleep(k, j);
  case fnKraid_Instr_9: return Kraid_Instr_9(k, j);
  case fnKraid_Instr_1: return Kraid_Instr_1(k, j);
  case fnKraid_Instr_DecYpos: return Kraid_Instr_DecYpos(k, j);
  case fnKraid_Instr_IncrYpos_Shake: return Kraid_Instr_IncrYpos_Shake(k, j);
  case fnKraid_Instr_PlaySound_0x76: return Kraid_Instr_PlaySound_0x76(k, j);
  case fnKraid_Instr_XposMinus3: return Kraid_Instr_XposMinus3(k, j);
  case fnKraid_Instr_XposMinus3b: return Kraid_Instr_XposMinus3b(k, j);
  case fnKraid_Instr_XposPlus3: return Kraid_Instr_XposPlus3(k, j);
  case fnKraid_Instr_MoveHimRight: return Kraid_Instr_MoveHimRight(k, j);
  case fnEnemyInstr_Goto_A8: return EnemyInstr_Goto(k, j);
  case fnEnemyInstr_DecTimerAndGoto2_A8: return EnemyInstr_DecTimerAndGoto(k, j);
  case fnEnemyInstr_SetTimer_A8: return EnemyInstr_SetTimer(k, j);
  case fnEnemyInstr_Sleep_A8: return EnemyInstr_Sleep(k, j);
  case fnEnemyInstr_EnableOffScreenProcessing_A8: return EnemyInstr_EnableOffScreenProcessing(k, j);
  case fnEnemyInstr_DisableOffScreenProcessing_A8: return EnemyInstr_DisableOffScreenProcessing(k, j);
  case fnMiniDraygon_Instr_2: return MiniDraygon_Instr_2(k, j);
  case fnMiniDraygon_Instr_1: return MiniDraygon_Instr_1(k, j);
  case fnMiniDraygon_Instr_3: return MiniDraygon_Instr_3(k, j);
  case fnMiniDraygon_Instr_4: return MiniDraygon_Instr_4(k, j);
  case fnFune_Instr_2: return Fune_Instr_2(k, j);
  case fnFune_Instr_6: return Fune_Instr_6(k, j);
  case fnFune_Instr_7: return Fune_Instr_7(k, j);
  case fnFune_Instr_1: return Fune_Instr_1(k, j);
  case fnFune_Instr_4: return Fune_Instr_4(k, j);
  case fnFune_Instr_3: return Fune_Instr_3(k, j);
  case fnFune_Instr_5: return Fune_Instr_5(k, j);
  case fnYappingMaw_Instr_2: return YappingMaw_Instr_2(k, j);
  case fnYappingMaw_Instr_4: return YappingMaw_Instr_4(k, j);
  case fnYappingMaw_Instr_5: return YappingMaw_Instr_5(k, j);
  case fnYappingMaw_Instr_7: return YappingMaw_Instr_7(k, j);
  case fnYappingMaw_Instr_3: return YappingMaw_Instr_3(k, j);
  case fnYappingMaw_Instr_6: return YappingMaw_Instr_6(k, j);
  case fnYappingMaw_Instr_1: return YappingMaw_Instr_1(k, j);
  case fnNorfairLavaMan_Instr_1: return NorfairLavaMan_Instr_1(k, j);
  case fnNorfairLavaMan_Instr_8: return NorfairLavaMan_Instr_8(k, j);
  case fnNorfairLavaMan_Instr_14: return NorfairLavaMan_Instr_14(k, j);
  case fnNorfairLavaMan_Instr_2: return NorfairLavaMan_Instr_2(k, j);
  case fnNorfairLavaMan_Instr_7: return NorfairLavaMan_Instr_7(k, j);
  case fnNorfairLavaMan_Instr_10: return NorfairLavaMan_Instr_10(k, j);
  case fnNorfairLavaMan_Instr_12: return NorfairLavaMan_Instr_12(k, j);
  case fnNorfairLavaMan_Instr_9: return NorfairLavaMan_Instr_9(k, j);
  case fnNorfairLavaMan_Instr_11: return NorfairLavaMan_Instr_11(k, j);
  case fnNorfairLavaMan_Instr_13: return NorfairLavaMan_Instr_13(k, j);
  case fnNorfairLavaMan_Instr_5: return NorfairLavaMan_Instr_5(k, j);
  case fnNorfairLavaMan_Instr_15: return NorfairLavaMan_Instr_15(k, j);
  case fnNorfairLavaMan_Instr_4: return NorfairLavaMan_Instr_4(k, j);
  case fnNorfairLavaMan_Instr_16: return NorfairLavaMan_Instr_16(k, j);
  case fnNorfairLavaMan_Instr_6: return NorfairLavaMan_Instr_6(k, j);
  case fnNorfairLavaMan_Instr_3: return NorfairLavaMan_Instr_3(k, j);
  case fnBeetom_Instr_1: return Beetom_Instr_1(k, j);
  case fnWreckedShipRobot_Instr_4: return WreckedShipRobot_Instr_4(k, j);
  case fnWreckedShipRobot_Instr_9: return WreckedShipRobot_Instr_9(k, j);
  case fnWreckedShipRobot_Instr_6: return WreckedShipRobot_Instr_6(k, j);
  case fnWreckedShipRobot_Instr_8: return WreckedShipRobot_Instr_8(k, j);
  case fnWreckedShipRobot_Instr_7: return WreckedShipRobot_Instr_7(k, j);
  case fnWreckedShipRobot_Instr_15: return WreckedShipRobot_Instr_15(k, j);
  case fnWreckedShipRobot_Instr_18: return WreckedShipRobot_Instr_18(k, j);
  case fnWreckedShipRobot_Instr_16: return WreckedShipRobot_Instr_16(k, j);
  case fnWreckedShipRobot_Instr_17: return WreckedShipRobot_Instr_17(k, j);
  case fnWreckedShipRobot_Instr_3: return WreckedShipRobot_Instr_3(k, j);
  case fnWreckedShipRobot_Instr_10: return WreckedShipRobot_Instr_10(k, j);
  case fnWreckedShipRobot_Instr_14: return WreckedShipRobot_Instr_14(k, j);
  case fnWreckedShipRobot_Instr_2: return WreckedShipRobot_Instr_2(k, j);
  case fnWreckedShipRobot_Instr_13: return WreckedShipRobot_Instr_13(k, j);
  case fnWreckedShipRobot_Instr_1: return WreckedShipRobot_Instr_1(k, j);
  case fnWreckedShipRobot_Instr_12: return WreckedShipRobot_Instr_12(k, j);
  case fnWreckedShipRobot_Instr_5: return WreckedShipRobot_Instr_5(k, j);
  case fnWreckedShipRobot_Instr_11: return WreckedShipRobot_Instr_11(k, j);
  case fnWalkingLavaSeahorse_Instr_4: return WalkingLavaSeahorse_Instr_4(k, j);
  case fnWalkingLavaSeahorse_Instr_3: return WalkingLavaSeahorse_Instr_3(k, j);
  case fnWalkingLavaSeahorse_Instr_5: return WalkingLavaSeahorse_Instr_5(k, j);
  case fnWalkingLavaSeahorse_Instr_6: return WalkingLavaSeahorse_Instr_6(k, j);
  case fnWalkingLavaSeahorse_Instr_2: return WalkingLavaSeahorse_Instr_2(k, j);
  case fnWalkingLavaSeahorse_Instr_1: return WalkingLavaSeahorse_Instr_1(k, j);
  case fnWreckedShipSpark_Instr_2: return WreckedShipSpark_Instr_2(k, j);
  case fnWreckedShipSpark_Instr_1: return WreckedShipSpark_Instr_1(k, j);
  case fnKiHunter_Instr_1: return KiHunter_Instr_1(k, j);
  case fnKiHunter_Instr_2: return KiHunter_Instr_2(k, j);
  case fnKiHunter_Instr_3: return KiHunter_Instr_3(k, j);
  case fnKiHunter_Instr_4: return KiHunter_Instr_4(k, j);
  case fnKiHunter_Instr_5: return KiHunter_Instr_5(k, j);
  case fnEnemyInstr_Sleep_A9: return EnemyInstr_Sleep(k, j);
  case fnShitroid_Instr_1: return Shitroid_Instr_1(k, j);
  case fnShitroid_Instr_2: return Shitroid_Instr_2(k, j);
  case fnsub_A9ECD0: return sub_A9ECD0(k, j);
  case fnShitroid_Instr_3: return Shitroid_Instr_3(k, j);
  case fnShitroid_Instr_4: return Shitroid_Instr_4(k, j);
  case fnShitroid_Instr_6: return Shitroid_Instr_6(k, j);
  case fnShitroid_Instr_5: return Shitroid_Instr_5(k, j);
  case fnEnemy_SetAiPreInstr_AA: return Enemy_SetAiPreInstr_AA(k, j);
  case fnEnemy_ClearAiPreInstr_AA: return Enemy_ClearAiPreInstr_AA(k, j);
  case fnEnemyInstr_StopScript_AA: return EnemyInstr_StopScript(k, j);
  case fnEnemyInstr_Goto_AA: return EnemyInstr_Goto(k, j);
  case fnEnemyInstr_DecTimerAndGoto2_AA: return EnemyInstr_DecTimerAndGoto(k, j);
  case fnEnemyInstr_SetTimer_AA: return EnemyInstr_SetTimer(k, j);
  case fnEnemyInstr_Sleep_AA: return EnemyInstr_Sleep(k, j);
  case fnEnemyInstr_WaitNframes_AA: return EnemyInstr_WaitNframes(k, j);
  case fnEnemyInstr_CopyToVram_AA: return EnemyInstr_CopyToVram(k, j);
  case fnTorizo_Instr_3: return Torizo_Instr_3(k, j);
  case fnTorizo_Instr_31: return Torizo_Instr_31(k, j);
  case fnTorizo_Instr_33: return Torizo_Instr_33(k, j);
  case fnTorizo_Instr_36: return Torizo_Instr_36(k, j);
  case fnTorizo_Instr_37: return Torizo_Instr_37(k, j);
  case fnTorizo_Instr_35: return Torizo_Instr_35(k, j);
  case fnTorizo_Instr_38: return Torizo_Instr_38(k, j);
  case fnTorizo_Instr_6: return Torizo_Instr_6(k, j);
  case fnTorizo_Instr_5: return Torizo_Instr_5(k, j);
  case fnTorizo_Instr_9: return Torizo_Instr_9(k, j);
  case fnTorizo_Instr_7: return Torizo_Instr_7(k, j);
  case fnTorizo_Instr_2: return Torizo_Instr_2(k, j);
  case fnTorizo_Instr_8: return Torizo_Instr_8(k, j);
  case fnTorizo_Instr_25: return Torizo_Instr_25(k, j);
  case fnTorizo_Instr_22: return Torizo_Instr_22(k, j);
  case fnTorizo_Instr_19: return Torizo_Instr_19(k, j);
  case fnTorizo_Instr_32: return Torizo_Instr_32(k, j);
  case fnTorizo_Instr_30: return Torizo_Instr_30(k, j);
  case fnTorizo_Instr_34: return Torizo_Instr_34(k, j);
  case fnTorizo_Instr_24: return Torizo_Instr_24(k, j);
  case fnTorizo_Instr_12: return Torizo_Instr_12(k, j);
  case fnTorizo_Instr_10: return Torizo_Instr_10(k, j);
  case fnTorizo_Instr_11: return Torizo_Instr_11(k, j);
  case fnTorizo_Instr_29: return Torizo_Instr_29(k, j);
  case fnTorizo_Instr_1: return Torizo_Instr_1(k, j);
  case fnTorizo_Instr_28: return Torizo_Instr_28(k, j);
  case fnTorizo_Instr_4: return Torizo_Instr_4(k, j);
  case fnTorizo_Instr_40: return Torizo_Instr_40(k, j);
  case fnTorizo_Instr_16: return Torizo_Instr_16(k, j);
  case fnTorizo_Instr_27: return Torizo_Instr_27(k, j);
  case fnTorizo_Instr_23: return Torizo_Instr_23(k, j);
  case fnTorizo_Instr_14: return Torizo_Instr_14(k, j);
  case fnTorizo_Instr_15: return Torizo_Instr_15(k, j);
  case fnTorizo_Instr_26: return Torizo_Instr_26(k, j);
  case fnTorizo_Instr_18: return Torizo_Instr_18(k, j);
  case fnTorizo_Instr_20: return Torizo_Instr_20(k, j);
  case fnTorizo_Instr_44: return Torizo_Instr_44(k, j);
  case fnTorizo_Instr_21: return Torizo_Instr_21(k, j);
  case fnTorizo_Instr_17: return Torizo_Instr_17(k, j);
  case fnTorizo_Instr_13: return Torizo_Instr_13(k, j);
  case fnTorizo_Instr_39: return Torizo_Instr_39(k, j);
  case fnTorizo_Instr_41: return Torizo_Instr_41(k, j);
  case fnTorizo_Instr_42: return Torizo_Instr_42(k, j);
  case fnTorizo_Instr_48: return Torizo_Instr_48(k, j);
  case fnTorizo_Instr_57: return Torizo_Instr_57(k, j);
  case fnTorizo_Instr_58: return Torizo_Instr_58(k, j);
  case fnTorizo_Instr_59: return Torizo_Instr_59(k, j);
  case fnTorizo_Instr_62: return Torizo_Instr_62(k, j);
  case fnTorizo_Instr_63: return Torizo_Instr_63(k, j);
  case fnTorizo_Instr_56: return Torizo_Instr_56(k, j);
  case fnTorizo_Instr_60: return Torizo_Instr_60(k, j);
  case fnTorizo_Instr_46: return Torizo_Instr_46(k, j);
  case fnTorizo_Instr_47: return Torizo_Instr_47(k, j);
  case fnTorizo_Instr_49: return Torizo_Instr_49(k, j);
  case fnTorizo_Instr_61: return Torizo_Instr_61(k, j);
  case fnTorizo_Instr_53: return Torizo_Instr_53(k, j);
  case fnTorizo_Instr_55: return Torizo_Instr_55(k, j);
  case fnTorizo_Instr_52: return Torizo_Instr_52(k, j);
  case fnTorizo_Instr_50: return Torizo_Instr_50(k, j);
  case fnTorizo_Instr_43: return Torizo_Instr_43(k, j);
  case fnTorizo_Instr_51: return Torizo_Instr_51(k, j);
  case fnTorizo_Instr_45: return Torizo_Instr_45(k, j);
  case fnTorizo_Instr_54: return Torizo_Instr_54(k, j);
  case fnShaktool_Instr_2: return Shaktool_Instr_2(k, j);
  case fnShaktool_Instr_3: return Shaktool_Instr_3(k, j);
  case fnShaktool_Instr_4: return Shaktool_Instr_4(k, j);
  case fnShaktool_Instr_5: return Shaktool_Instr_5(k, j);
  case fnShaktool_Instr_6: return Shaktool_Instr_6(k, j);
  case fnShaktool_Instr_1: return Shaktool_Instr_1(k, j);
  case fnShaktool_Instr_9: return Shaktool_Instr_9(k, j);
  case fnShaktool_Instr_11: return Shaktool_Instr_11(k, j);
  case fnShaktool_Instr_10: return Shaktool_Instr_10(k, j);
  case fnShaktool_Instr_8: return Shaktool_Instr_8(k, j);
  case fnShaktool_Instr_13: return Shaktool_Instr_13(k, j);
  case fnShaktool_Instr_12: return Shaktool_Instr_12(k, j);
  case fnShaktool_Instr_7: return Shaktool_Instr_7(k, j);
  case fnShaktool_Instr_14: return Shaktool_Instr_14(k, j);
  case fnEnemyInstr_Goto_B2: return EnemyInstr_Goto(k, j);
  case fnEnemyInstr_DecTimerAndGoto2_B2: return EnemyInstr_DecTimerAndGoto(k, j);
  case fnEnemyInstr_SetTimer_B2: return EnemyInstr_SetTimer(k, j);
  case fnEnemyInstr_Sleep_B2: return EnemyInstr_Sleep(k, j);
  case fnEnemyInstr_WaitNframes_B2: return EnemyInstr_WaitNframes(k, j);
  case fnSpacePirates_Instr_MovePixelsDownAndChangeDirFaceRight: return SpacePirates_Instr_MovePixelsDownAndChangeDirFaceRight(k, j);
  case fnSpacePirates_Instr_MovePixelsDownAndChangeDirFaceLeft: return SpacePirates_Instr_MovePixelsDownAndChangeDirFaceLeft(k, j);
  case fnSpacePirates_Instr_RandomNewDirFaceR: return SpacePirates_Instr_RandomNewDirFaceR(k, j);
  case fnSpacePirates_Instr_RandomNewDirFaceL: return SpacePirates_Instr_RandomNewDirFaceL(k, j);
  case fnSpacePirates_Instr_PrepareWallJumpR: return SpacePirates_Instr_PrepareWallJumpR(k, j);
  case fnSpacePirates_Instr_PrepareWallJumpL: return SpacePirates_Instr_PrepareWallJumpL(k, j);
  case fnSpacePirates_Instr_FireLaserL: return SpacePirates_Instr_FireLaserL(k, j);
  case fnSpacePirates_Instr_FireLaserR: return SpacePirates_Instr_FireLaserR(k, j);
  case fnSpacePirates_Instr_SetEnemyFunc: return SpacePirates_Instr_SetEnemyFunc(k, j);
  case fnSpacePirates_Instr_PlaySfx: return SpacePirates_Instr_PlaySfx(k, j);
  case fnSpacePirates_Instr_20: return SpacePirates_Instr_20(k, j);
  case fnSpacePirates_Instr_16: return SpacePirates_Instr_16(k, j);
  case fnSpacePirates_Instr_15: return SpacePirates_Instr_15(k, j);
  case fnSpacePirates_Instr_18: return SpacePirates_Instr_18(k, j);
  case fnSpacePirates_Instr_17: return SpacePirates_Instr_17(k, j);
  case fnSpacePirates_Instr_19: return SpacePirates_Instr_19(k, j);
  case fnSpacePirates_Instr_21: return SpacePirates_Instr_21(k, j);
  case fnSpacePirates_Instr_12: return SpacePirates_Instr_12(k, j);
  case fnSpacePirates_Instr_14: return SpacePirates_Instr_14(k, j);
  case fnSpacePirates_Instr_11: return SpacePirates_Instr_11(k, j);
  case fnSpacePirates_Instr_13: return SpacePirates_Instr_13(k, j);
  case fnEnemy_SetAiPreInstr_B3: return Enemy_SetAiPreInstr_B3(k, j);
  case fnEnemy_ClearAiPreInstr_B3: return Enemy_ClearAiPreInstr_B3(k, j);
  case fnEnemyInstr_Goto_B3: return EnemyInstr_Goto(k, j);
  case fnEnemyInstr_DecTimerAndGoto2_B3: return EnemyInstr_DecTimerAndGoto(k, j);
  case fnEnemyInstr_SetTimer_B3: return EnemyInstr_SetTimer(k, j);
  case fnEnemyInstr_Sleep_B3: return EnemyInstr_Sleep(k, j);
  case fnBotwoon_Instr_1: return Botwoon_Instr_1(k, j);
  case fnBotwoon_Instr_2: return Botwoon_Instr_2(k, j);
  case fnBotwoon_Instr_3: return Botwoon_Instr_3(k, j);
  case fnBotwoon_Instr_4: return Botwoon_Instr_4(k, j);
  case fnBotwoon_Instr_5: return Botwoon_Instr_5(k, j);
  case fnBotwoon_Instr_6: return Botwoon_Instr_6(k, j);
  case fnBotwoon_Instr_7: return Botwoon_Instr_7(k, j);
  case fnBotwoon_Instr_8: return Botwoon_Instr_8(k, j);
  case fnBotwoon_Instr_9: return Botwoon_Instr_9(k, j);
  case fnBotwoon_Instr_10: return Botwoon_Instr_10(k, j);
  case fnBotwoon_Instr_SetSpitting: return Botwoon_Instr_SetSpitting(k, j);
  case fnBotwoon_Instr_QueueSpitSfx: return Botwoon_Instr_QueueSpitSfx(k, j);
  case fnEscapeEtecoon_Instr_1: return EscapeEtecoon_Instr_1(k, j);
  case fnEscapeEtecoon_Instr_2: return EscapeEtecoon_Instr_2(k, j);
  case fnEscapeDachora_Instr_2: return EscapeDachora_Instr_2(k, j);
  case fnEscapeDachora_Instr_3: return EscapeDachora_Instr_3(k, j);
  case fnEscapeDachora_Instr_1: return EscapeDachora_Instr_1(k, j);
  case fnEscapeDachora_Instr_4: return EscapeDachora_Instr_4(k, j);
  case fnMotherBrain_Instr_MoveBodyUp10Left4: return MotherBrain_Instr_MoveBodyUp10Left4(k, j);
  case fnMotherBrain_Instr_MoveBodyUp16Left4: return MotherBrain_Instr_MoveBodyUp16Left4(k, j);
  case fnMotherBrain_Instr_MoveBodyUp12Right2: return MotherBrain_Instr_MoveBodyUp12Right2(k, j);
  case fnMotherBrain_Instr_MoveDown12Left4: return MotherBrain_Instr_MoveDown12Left4(k, j);
  case fnMotherBrain_Instr_MoveDown16Right2: return MotherBrain_Instr_MoveDown16Right2(k, j);
  case fnMotherBrain_Instr_MoveDown10Right2: return MotherBrain_Instr_MoveDown10Right2(k, j);
  case fnMotherBrain_Instr_MoveUp2Right1: return MotherBrain_Instr_MoveUp2Right1(k, j);
  case fnMotherBrain_Instr_MoveRight2: return MotherBrain_Instr_MoveRight2(k, j);
  case fnMotherBrain_Instr_MoveUp1: return MotherBrain_Instr_MoveUp1(k, j);
  case fnMotherBrain_Instr_MoveUp1Right3_Sfx: return MotherBrain_Instr_MoveUp1Right3_Sfx(k, j);
  case fnMotherBrain_Instr_Down2Right15: return MotherBrain_Instr_Down2Right15(k, j);
  case fnMotherBrain_Instr_Down4Right6: return MotherBrain_Instr_Down4Right6(k, j);
  case fnMotherBrain_Instr_Up4Left2: return MotherBrain_Instr_Up4Left2(k, j);
  case fnMotherBrain_Instr_Up2Left1_Sfx: return MotherBrain_Instr_Up2Left1_Sfx(k, j);
  case fnMotherBrain_Instr_Up2Left1_Sfx2: return MotherBrain_Instr_Up2Left1_Sfx2(k, j);
  case fnMotherBrain_Instr_MoveLeft2: return MotherBrain_Instr_MoveLeft2(k, j);
  case fnMotherBrain_Instr_MoveDown1: return MotherBrain_Instr_MoveDown1(k, j);
  case fnMotherBrain_Instr_MoveDown1Left3: return MotherBrain_Instr_MoveDown1Left3(k, j);
  case fnMotherBrain_Instr_MoveUp2Left15_Sfx: return MotherBrain_Instr_MoveUp2Left15_Sfx(k, j);
  case fnMotherBrain_Instr_MoveUp4Left6: return MotherBrain_Instr_MoveUp4Left6(k, j);
  case fnMotherBrain_Instr_MoveDown4Right2: return MotherBrain_Instr_MoveDown4Right2(k, j);
  case fnMotherBrain_Instr_MoveDown2Right1: return MotherBrain_Instr_MoveDown2Right1(k, j);
  case fnMotherBrain_Instr_SetPose_Standing: return MotherBrain_Instr_SetPose_Standing(k, j);
  case fnMotherBrain_Instr_SetPose_Walking: return MotherBrain_Instr_SetPose_Walking(k, j);
  case fnMotherBrain_Instr_SetPose_Crouched: return MotherBrain_Instr_SetPose_Crouched(k, j);
  case fnMotherBrain_Instr_SetPose_CrouchedTrans: return MotherBrain_Instr_SetPose_CrouchedTrans(k, j);
  case fnMotherBrain_Instr_SetPose_DeathBeamMode: return MotherBrain_Instr_SetPose_DeathBeamMode(k, j);
  case fnMotherBrain_Instr_SetPose_LeaningDown: return MotherBrain_Instr_SetPose_LeaningDown(k, j);
  case fnMotherBrain_Instr_SpawnEprojToOffset: return MotherBrain_Instr_SpawnEprojToOffset(k, j);
  case fnMotherBrain_Instr_SpawnDeathBeamEproj: return MotherBrain_Instr_SpawnDeathBeamEproj(k, j);
  case fnMotherBrain_Instr_IncrBeamAttackPhase: return MotherBrain_Instr_IncrBeamAttackPhase(k, j);

  default: Unreachable(); return NULL;
  }
}

void EnemyMain(void) {  // 0xA08FD4
  EnemyDef_A2 *EnemyDef_A2;
  int16 v6;
  char v8; // cf

  if (first_free_enemy_index) {
    if (enemy_index_to_shake != 0xFFFF) {
      gEnemyData(enemy_index_to_shake)->shake_timer = 64;
      enemy_index_to_shake = -1;
    }
    interactive_enemy_indexes_index = 0;
    for (active_enemy_indexes_index = 0; ; ++active_enemy_indexes_index) {
      int v0 = active_enemy_indexes_index >> 1;
      uint16 v1 = active_enemy_indexes[v0];
      if (v1 == 0xFFFF)
        break;
      cur_enemy_index = active_enemy_indexes[v0];
      uint16 v2 = v1;
      enemy_data_ptr = v1 + 3960;
      EnemyData *v3 = gEnemyData(v1);
      *(uint16 *)&enemy_ai_pointer.bank = *(uint16 *)&v3->bank;
      if ((v3->properties & 0x400) == 0) {
        if (v3->invincibility_timer) {
          --v3->invincibility_timer;
        } else if (!debug_disable_sprite_interact) {
          if (!(debug_time_frozen_for_enemies | time_is_frozen_flag)) {
            EnemyCollisionHandler();
            if (!gEnemyData(cur_enemy_index)->enemy_ptr)
              goto LABEL_32;
          }
          v2 = cur_enemy_index;
          if ((gEnemyData(cur_enemy_index)->extra_properties & 1) != 0)
            goto LABEL_23;
        }
      }
      UNUSED_word_7E17A2 = 0;
      if (!(debug_time_frozen_for_enemies | time_is_frozen_flag)) {
        v6 = 0;
        uint16 ai_handler_bits = gEnemyData(cur_enemy_index)->ai_handler_bits;
        if (ai_handler_bits) {
          do {
            ++v6;
            v8 = ai_handler_bits & 1;
            ai_handler_bits >>= 1;
          } while (!v8);
        }
        EnemyData *v9 = gEnemyData(cur_enemy_index);
        enemy_ai_pointer.addr = get_EnemyDef_A2(v9->enemy_ptr + 2 * v6)->main_ai;
        goto LABEL_20;
      }
      EnemyData *v4;
      v4 = gEnemyData(v2);
      EnemyDef_A2 = get_EnemyDef_A2(v4->enemy_ptr);
      if (EnemyDef_A2->time_is_frozen_ai) {
        enemy_ai_pointer.addr = EnemyDef_A2->time_is_frozen_ai;
LABEL_20:
        enemy_ai_pointer.bank = gEnemyData(cur_enemy_index)->bank;
        CallEnemyAi(Load24(&enemy_ai_pointer));
        if (!(debug_time_frozen_for_enemies | time_is_frozen_flag)) {
          EnemyData *v10 = gEnemyData(cur_enemy_index);
          ++v10->frame_counter;
          if ((v10->properties & 0x2000) != 0) {
            enemy_processing_stage = 2;
            ProcessEnemyInstructions();
          }
        }
      }
LABEL_23:;
      uint16 v11;
      v11 = cur_enemy_index;
      EnemyData *v12;
      v12 = gEnemyData(cur_enemy_index);
      if ((v12->extra_properties & 1) != 0 && (v12->flash_timer == 1 || v12->frozen_timer == 1)) {
        gEnemySpawnData(cur_enemy_index)->cause_of_death = 0;
        EnemyDeathAnimation(v11, 0);
      }
      if (((gEnemyData(cur_enemy_index)->extra_properties & 4) != 0 || !EnemyWithNormalSpritesIsOffScreen())
          && (gEnemyData(cur_enemy_index)->properties & 0x300) == 0
          && (UNUSED_word_7E17A2 & 1) == 0) {
        DrawOneEnemy();
      }
LABEL_32:;
      EnemyData *v13 = gEnemyData(cur_enemy_index);
      if (v13->flash_timer && !(debug_time_frozen_for_enemies | time_is_frozen_flag)) {
        if (sign16(--v13->flash_timer - 8))
          v13->ai_handler_bits &= ~2u;
      }
      ++active_enemy_indexes_index;
    }
  }
  HandleSpriteObjects();
  ++enemy_damage_routine_exec_count;
  enemy_index_colliding_dirs[0] = -1;
  enemy_index_colliding_dirs[1] = -1;
  enemy_index_colliding_dirs[2] = -1;
  enemy_index_colliding_dirs[3] = -1;
  distance_to_enemy_colliding_dirs[0] = 0;
  distance_to_enemy_colliding_dirs[1] = 0;
  distance_to_enemy_colliding_dirs[2] = 0;
  distance_to_enemy_colliding_dirs[3] = 0;
}

void DecrementSamusTimers(void) {  // 0xA09169
  if (samus_invincibility_timer)
    --samus_invincibility_timer;
  if (samus_knockback_timer)
    --samus_knockback_timer;
  if (projectile_invincibility_timer)
    --projectile_invincibility_timer;
  interactive_enemy_indexes[0] = -1;
  active_enemy_indexes[0] = -1;
}

void SpawnEnemyDrops(uint16 a, uint16 k) {  // 0xA0920E
  varE24 = a;
  SpawnEnemyProjectileWithGfx(enemy_population_ptr, k, addr_kEproj_Pickup);
}

void DeleteEnemyAndConnectedEnemies(void) {  // 0xA0922B
  EnemyData *v0 = gEnemyData(cur_enemy_index);
  uint16 num_parts = get_EnemyDef_A2(v0->enemy_ptr)->num_parts;
  if (!num_parts)
    num_parts = 1;
  uint16 v2 = num_parts;
  do {
    gEnemyData(cur_enemy_index)->enemy_ptr = 0;
    cur_enemy_index += 64;
    --v2;
  } while (v2);
}

uint16 SpawnEnemy(uint8 db, uint16 k) {  // 0xA09275
  int16 v3;
  VoidP v7;
  int16 v12;
  EnemyPopulation *v13;
  EnemyPopulation *v16;
  EnemyDef_A2 *v17;
  EnemyDef_A2 *EnemyDef_A2;
  EnemyPopulation *EnemyPopulation;

  uint8 v19 = db;
  enemy_population_ptr = k;
  cur_enemy_index_backup = cur_enemy_index;
  enemy_ai_pointer_backup.addr = enemy_ai_pointer.addr;
  *(uint16 *)&enemy_ai_pointer_backup.bank = *(uint16 *)&enemy_ai_pointer.bank;
  uint16 enemy_ptr = get_EnemyPopulation(db, k)->enemy_ptr;
  v3 = get_EnemyDef_A2(enemy_ptr)->num_parts - 1;
  if (v3 < 0)
    v3 = 0;
  draw_oam_x_offset = v3;
  varE26 = v3;
  new_enemy_index = 0;
  do {
    uint16 v4 = new_enemy_index;
    if (!gEnemyData(new_enemy_index)->enemy_ptr) {
      while (!gEnemyData(v4)->enemy_ptr) {
        if (!draw_oam_x_offset) {
          while (1) {
            uint16 v5 = new_enemy_index;
            EnemyPopulation = get_EnemyPopulation(v19, enemy_population_ptr);
            v7 = EnemyPopulation->enemy_ptr;
            uint16 v8 = 0;
            if (EnemyPopulation->enemy_ptr == enemy_def_ptr[0]
                || (v8 = 2, v7 == enemy_def_ptr[1])
                || (v8 = 4, v7 == enemy_def_ptr[2])
                || (v8 = 6, v7 == enemy_def_ptr[3])) {
              int v10 = v8 >> 1;
              EnemyData *v11 = gEnemyData(new_enemy_index);
              v11->vram_tiles_index = enemy_gfxdata_tiles_index[v10];
              LOBYTE(v12) = HIBYTE(enemy_gfxdata_vram_ptr[v10]);
              HIBYTE(v12) = enemy_gfxdata_vram_ptr[v10];
              v11->palette_index = 2 * v12;
            } else {
              EnemyData *v9 = gEnemyData(new_enemy_index);
              v9->vram_tiles_index = 0;
              v9->palette_index = 0;
            }
            v13 = get_EnemyPopulation(v19, enemy_population_ptr);
            EnemyDef_A2 = get_EnemyDef_A2(v13->enemy_ptr);
            EnemyData *v15 = gEnemyData(v5);
            v15->x_width = EnemyDef_A2->x_radius;
            v15->y_height = EnemyDef_A2->y_radius;
            v15->health = EnemyDef_A2->health;
            v15->layer = EnemyDef_A2->layer;
            *(uint16 *)&v15->bank = *(uint16 *)&EnemyDef_A2->bank;
            v16 = get_EnemyPopulation(v19, enemy_population_ptr);
            v15->enemy_ptr = v16->enemy_ptr;
            v15->x_pos = v16->x_pos;
            v15->y_pos = v16->y_pos;
            v15->current_instruction = v16->init_param;
            v15->properties = v16->properties;
            v15->extra_properties = v16->extra_properties;
            v15->parameter_1 = v16->parameter1;
            v15->parameter_2 = v16->parameter2;
            v15->frame_counter = 0;
            v15->timer = 0;
            v15->ai_var_A = 0;
            v15->ai_var_B = 0;
            v15->ai_var_C = 0;
            v15->ai_var_D = 0;
            v15->ai_var_E = 0;
            v15->ai_preinstr = 0;
            v15->instruction_timer = 1;
            v15->frame_counter = 0;
            RecordEnemySpawnData(v5);
            cur_enemy_index = v5;
            v17 = get_EnemyDef_A2(v15->enemy_ptr);
            if (!sign16(v17->ai_init + 0x8000)) {
              enemy_ai_pointer.addr = v17->ai_init;
              *(uint16 *)&enemy_ai_pointer.bank = *(uint16 *)&v17->bank;
              CallEnemyAi(Load24(&enemy_ai_pointer));
            }
            EnemyData *v18 = gEnemyData(v5);
            if ((v18->properties & 0x2000) != 0)
              v18->spritemap_pointer = addr_kSpritemap_Nothing_A0;
            if (!varE26)
              break;
            if (!--varE26)
              break;
            new_enemy_index += 64;
            enemy_population_ptr += 16;
          }
          enemy_ai_pointer.addr = enemy_ai_pointer_backup.addr;
          *(uint16 *)&enemy_ai_pointer.bank = *(uint16 *)&enemy_ai_pointer_backup.bank;
          cur_enemy_index = cur_enemy_index_backup;
          return new_enemy_index;
        }
        --draw_oam_x_offset;
        v4 += 64;
        if ((int16)(v4 - 2048) >= 0)
          return 0xffff;
      }
    }
    new_enemy_index += 64;
  } while (sign16(new_enemy_index - 2048));
  return 0xffff;
}

void DrawOneEnemy(void) {  // 0xA09423
  enemy_drawing_queue_index = 2 * gEnemyData(cur_enemy_index)->layer;
  *(uint16 *)RomPtr_RAM(
    enemy_drawing_queue_sizes[enemy_drawing_queue_index >> 1]
    + kEnemyLayerToQueuePtr[enemy_drawing_queue_index >> 1]) = cur_enemy_index;
  int v0 = enemy_drawing_queue_index >> 1;
  ++enemy_drawing_queue_sizes[v0];
  ++enemy_drawing_queue_sizes[v0];
}

void WriteEnemyOams(void) {  // 0xA0944A
  EnemySpawnData *v1;
  VoidP palette_index;
  ExtendedSpriteMap *ExtendedSpriteMap;

  uint8 db = gEnemyData(cur_enemy_index)->bank;
  EnemyData *v0 = gEnemyData(cur_enemy_index);
  v1 = gEnemySpawnData(cur_enemy_index);
  draw_oam_x_offset = v1->xpos2 + v0->x_pos - layer1_x_pos;
  R20_ = draw_oam_x_offset;
  enemy_population_ptr = v1->ypos2 + v0->y_pos - layer1_y_pos;
  R18_ = enemy_population_ptr;
  if (v0->shake_timer) {
    if ((v0->frame_counter & 2) != 0) {
      --R20_;
      --draw_oam_x_offset;
    } else {
      ++R20_;
      ++draw_oam_x_offset;
    }
    --v0->shake_timer;
  }
  if (v0->flash_timer && (enemy_damage_routine_exec_count & 2) != 0) {
    palette_index = 0;
  } else {
    uint16 frozen_timer = v0->frozen_timer;
    if (frozen_timer && (frozen_timer >= 0x5Au || (frozen_timer & 2) != 0))
      palette_index = 3072;
    else
      palette_index = v0->palette_index;
  }
  R3_.addr = palette_index;
  R0_.addr = v0->vram_tiles_index;
  if ((v0->extra_properties & 4) != 0) {
    while ((int16)(v0->spritemap_pointer + 0x8000) < 0)
      ;
    uint16 spritemap_pointer = v0->spritemap_pointer;
    remaining_enemy_spritemap_entries = *RomPtrWithBank(db, spritemap_pointer);
    uint16 v5 = spritemap_pointer + 2;
    do {
      ExtendedSpriteMap = get_ExtendedSpriteMap(db, v5);
      R22_ = ExtendedSpriteMap->spritemap;
      if (*(uint16 *)RomPtrWithBank(db, R22_) == 0xFFFE) {
        R20_ = draw_oam_x_offset + ExtendedSpriteMap->xpos;
        R18_ = enemy_population_ptr + ExtendedSpriteMap->ypos;
        if ((gEnemyData(cur_enemy_index)->extra_properties & 0x8000u) != 0)
          ProcessExtendedTilemap(db);
      } else {
        R20_ = draw_oam_x_offset + ExtendedSpriteMap->xpos;
        if (((R20_ + 128) & 0xFE00) == 0) {
          uint16 ypos = ExtendedSpriteMap->ypos;
          R18_ = enemy_population_ptr + ypos;
          if (((R18_ + 128) & 0xFE00) == 0) {
            if (HIBYTE(R18_))
              DrawSpritemapWithBaseTileOffscreen(db, R22_);
            else
              DrawSpritemapWithBaseTile2(db, R22_);
          }
        }
      }
      v5 += 8;
      --remaining_enemy_spritemap_entries;
    } while (remaining_enemy_spritemap_entries);
  } else {
    g_word_7EF37E = v0->enemy_ptr;
    g_word_7EF37A = v0->current_instruction;
    g_word_7EF37C = cur_enemy_index;
    enemy_processing_stage = 1;
    DrawSpritemapWithBaseTile(db, v0->spritemap_pointer);
  }
}

void NormalEnemyFrozenAI(void) {  // 0xA0957E
  EnemyData *v0 = gEnemyData(cur_enemy_index);
  v0->flash_timer = 0;
  if (!v0->frozen_timer || (--v0->frozen_timer, (equipped_beams & 2) == 0)) {
    uint16 v1 = v0->ai_handler_bits & 0xFFFB;
    v0->ai_handler_bits = v1;
    v0->frozen_timer = v1;
  }
}

void ProcessExtendedTilemap(uint8 db) {  // 0xA096CA
  uint8 *p = RomPtrWithBank(db, R22_ + 2);
  while (1) {
    uint16 v2 = *(uint16 *)p;
    if (v2 == 0xFFFF)
      break;
    int n = *((uint16 *)p + 1);
    p += 4;
    memcpy(g_ram + v2, p, n * 2);
    p += n * 2;
  }
  ++nmi_flag_bg2_enemy_vram_transfer;
  remaining_enemy_hitbox_entries = 0;
}

void QueueEnemyBG2TilemapTransfers(void) {  // 0xA09726
  VramWriteEntry *v0;

  if (nmi_flag_bg2_enemy_vram_transfer && !(debug_time_frozen_for_enemies | time_is_frozen_flag)) {
    v0 = gVramWriteEntry(vram_write_queue_tail);
    v0->size = enemy_bg2_tilemap_size;
    v0->src.addr = ADDR16_OF_RAM(*tilemap_stuff);
    *(uint16 *)&v0->src.bank = 126;
    v0->vram_dst = addr_unk_604800;
    vram_write_queue_tail += 7;
  }
  nmi_flag_bg2_enemy_vram_transfer = 0;
}

void EnemyCollisionHandler(void) {  // 0xA09758
  if ((gEnemyData(cur_enemy_index)->extra_properties & 4) != 0) {
    EnemyProjectileCollHandler_Multibox();
    EnemyBombCollHandler_Multibox();
    EnemySamusCollHandler_Multibox();
  } else {
    EnemyProjectileCollHandler();
    EnemyBombCollHandler();
    EnemySamusCollHandler();
  }
}

void func_nullsub_4(void) {
  ;
}

void SamusProjectileInteractionHandler(void) {  // 0xA09785
  int16 v2;

  enemy_processing_stage = 10;
  if (!flag_disable_projectile_interaction) {
    num_projs_to_check = 5;
    if (bomb_counter) {
      num_projs_to_check = 10;
    } else if (!projectile_counter) {
      return;
    }
    if (!projectile_invincibility_timer && !samus_contact_damage_index) {
      collision_detection_index = 0;
      do {
        int v1 = collision_detection_index;
        if (projectile_damage[v1]) {
          v2 = projectile_type[v1];
          if (v2 >= 0) {
            if (sign16((v2 & 0xF00) - 1792)) {
              int v3 = collision_detection_index;
              if ((projectile_dir[v3] & 0x10) == 0) {
                uint16 v4 = abs16(projectile_x_pos[v3] - samus_x_pos);
                bool v5 = v4 < projectile_x_radius[v3];
                uint16 v6 = v4 - projectile_x_radius[v3];
                if (v5 || v6 < samus_x_radius) {
                  uint16 v7 = abs16(projectile_y_pos[v3] - samus_y_pos);
                  v5 = v7 < projectile_y_radius[v3];
                  uint16 v8 = v7 - projectile_y_radius[v3];
                  if (v5 || v8 < samus_y_radius) {
                    if ((projectile_type[v3] & 0xFF00) != 768 && (projectile_type[v3] & 0xFF00) != 1280) {
                      projectile_dir[v3] |= 0x10u;
                      uint16 v10 = SuitDamageDivision(projectile_damage[v3]);
                      Samus_DealDamage(v10);
                      samus_invincibility_timer = 96;
                      samus_knockback_timer = 5;
                      assert(0);
                      uint16 v0 = 0;
                      knockback_x_dir = (int16)(samus_x_pos - enemy_projectile_x_pos[v0 >> 1]) >= 0;
                      return;
                    }
                    if (projectile_variables[v3] == 8) {
                      uint16 v9;
                      if (samus_x_pos == projectile_x_pos[v3]) {
                        v9 = 2;
                      } else if ((int16)(samus_x_pos - projectile_x_pos[v3]) < 0) {
                        v9 = 1;
                      } else {
                        v9 = 3;
                      }
                      bomb_jump_dir = v9;
                    }
                  }
                }
              }
            }
          }
        }
        ++collision_detection_index;
      } while (collision_detection_index != num_projs_to_check);
    }
  }
}

void EprojSamusCollDetect(void) {  // 0xA09894
  enemy_processing_stage = 11;
  if (!samus_invincibility_timer && !samus_contact_damage_index) {
    collision_detection_index = 34;
    do {
      if (*(uint16 *)((char *)enemy_projectile_id + collision_detection_index)) {
        int v0 = collision_detection_index >> 1;
        if ((enemy_projectile_properties[v0] & 0x2000) == 0) {
          if (LOBYTE(enemy_projectile_radius[v0])) {
            enemy_population_ptr = LOBYTE(enemy_projectile_radius[v0]);
            if (HIBYTE(enemy_projectile_radius[v0])) {
              draw_oam_x_offset = HIBYTE(enemy_projectile_radius[v0]);
              uint16 v1 = abs16(samus_x_pos - enemy_projectile_x_pos[v0]);
              bool v2 = v1 < samus_x_radius;
              uint16 v3 = v1 - samus_x_radius;
              if (v2 || v3 < enemy_population_ptr) {
                uint16 v4 = abs16(samus_y_pos - enemy_projectile_y_pos[v0]);
                v2 = v4 < samus_y_radius;
                uint16 v5 = v4 - samus_y_radius;
                if (v2 || v5 < draw_oam_x_offset)
                  HandleEprojCollWithSamus(collision_detection_index);
              }
            }
          }
        }
      }
      --collision_detection_index;
      --collision_detection_index;
    } while ((collision_detection_index & 0x8000u) == 0);
  }
}

void HandleEprojCollWithSamus(uint16 k) {  // 0xA09923
  samus_invincibility_timer = 96;
  samus_knockback_timer = 5;
  uint16 v1 = *((uint16 *)RomPtr_86(*(uint16 *)((char *)enemy_projectile_id + k)) + 5);
  if (v1) {
    int v2 = k >> 1;
    enemy_projectile_instr_list_ptr[v2] = v1;
    enemy_projectile_instr_timers[v2] = 1;
  }
  int v3 = k >> 1;
  if ((enemy_projectile_properties[v3] & 0x4000) == 0)
    *(uint16 *)((char *)enemy_projectile_id + k) = 0;
  uint16 v4 = SuitDamageDivision(enemy_projectile_properties[v3] & 0xFFF);
  Samus_DealDamage(v4);
  knockback_x_dir = (int16)(samus_x_pos - enemy_projectile_x_pos[v3]) >= 0;
}

void EprojProjCollDet(void) {  // 0xA0996C
  int16 v5;

  enemy_processing_stage = 12;
  if (projectile_counter) {
    collision_detection_index = 34;
    do {
      uint16 v0 = collision_detection_index;
      if (*(uint16 *)((char *)enemy_projectile_id + collision_detection_index)
          && (enemy_projectile_properties[collision_detection_index >> 1] & 0x8000u) != 0) {
        uint16 v1 = 0;
        do {
          int v2 = v0 >> 1;
          if (enemy_projectile_flags[v2] == 2)
            break;
          int v3 = v1 >> 1;
          uint16 v4 = projectile_type[v3];
          if (v4) {
            v5 = v4 & 0xF00;
            if (v5 != 768 && v5 != 1280) {
              if (sign16(v5 - 1792)) {
                R18_ = projectile_x_pos[v3] & 0xFFE0;
                if ((enemy_projectile_x_pos[v2] & 0xFFE0) == R18_) {
                  R18_ = projectile_y_pos[v3] & 0xFFE0;
                  if ((enemy_projectile_y_pos[v2] & 0xFFE0) == R18_)
                    HandleEprojCollWithProj(v0, v1);
                }
              }
            }
          }
          v1 += 2;
        } while ((int16)(v1 - 10) < 0);
      }
      --collision_detection_index;
      --collision_detection_index;
    } while ((collision_detection_index & 0x8000u) == 0);
  }
}

void HandleEprojCollWithProj(uint16 k, uint16 j) {  // 0xA099F9
  int v2 = j >> 1;
  if ((projectile_type[v2] & 8) == 0)
    projectile_dir[v2] |= 0x10u;
  if (enemy_projectile_flags[k >> 1] == 1) {
    int v4 = k >> 1;
    R18_ = projectile_x_pos[v4];
    R20_ = projectile_y_pos[v4];
    R22_ = 6;
    R24_ = 0;
    CreateSpriteAtPos();
    QueueSfx1_Max6(0x3Du);
  } else {
    int v3 = k >> 1;
    enemy_projectile_G[v3] = projectile_type[v2];
    enemy_projectile_instr_list_ptr[v3] = get_EnemyProjectileDef(*(uint16 *)((char *)enemy_projectile_id + k))->shot_instruction_list;
    enemy_projectile_instr_timers[v3] = 1;
    enemy_projectile_pre_instr[v3] = FUNC16(EprojPreInstr_nullsub_83);
    enemy_projectile_properties[v3] &= 0xFFFu;
  }
}

void CallHitboxTouch(uint32 ea) {
  switch (ea) {
  case fnEnemy_NormalTouchAI_A0: Enemy_NormalTouchAI_A0(); return;  // 0xa08023
  case fnMaridiaLargeSnail_Func_12: MaridiaLargeSnail_Func_12(); return;  // 0xa2d388
  case fnMaridiaLargeSnail_Touch: MaridiaLargeSnail_Touch(); return;  // 0xa2d38c
  case fnEnemy_NormalTouchAI_A4: Enemy_NormalTouchAI_A4(); return;  // 0xa48023
  case fnCrocomire_Func_92: Crocomire_Func_92(); return;  // 0xa4b93d
  case fnnullsub_34: return;  // 0xa4b950
  case fnnullsub_170_A5: return;  // 0xa5804c
  case fnDraygon_Touch: Draygon_Touch(); return;  // 0xa595ea
  case fnSporeSpawn_Touch: SporeSpawn_Touch(); return;  // 0xa5edec
  case fnsub_A6DF59: sub_A6DF59(); return;  // 0xa6df59
  case fnCeresSteam_Touch: CeresSteam_Touch(); return;  // 0xa6f03f
  case fnnullsub_170_A7: return;  // 0xa7804c
  case fnKraid_Touch_ArmFoot: Kraid_Touch_ArmFoot(); return;  // 0xa7948b
  case fnKraidsArm_Touch: KraidsArm_Touch(); return;  // 0xa79490
  case fnPhantoon_Touch: Phantoon_Touch(); return;  // 0xa7dd95
  case fnnullsub_47: return;  // 0xa9b5c5
  case fnMotherBrainsBrain_Touch: MotherBrainsBrain_Touch(); return;  // 0xa9b5c6
  case fnGoldTorizo_Touch: GoldTorizo_Touch(); return;  // 0xaac977
  case fnWalkingSpacePirates_Touch: WalkingSpacePirates_Touch(); return;  // 0xb2876c
  default: Unreachable();
  }
}
void CallHitboxShot(uint32 ea, uint16 j) {  // 0xA09D17
  switch (ea) {
  case fnEnemy_NormalShotAI_A0: Enemy_NormalShotAI_A0(); return;  // 0xa0802d
  case fnnullsub_170_A2: return;  // 0xa2804c
  case fnMaridiaLargeSnail_Shot: MaridiaLargeSnail_Shot(); return;  // 0xa2d3b4
  case fnEnemy_NormalShotAI_A4: Enemy_NormalShotAI_A4(); return;  // 0xa4802d
  case fnCrocomire_Func_93: Crocomire_Func_93(); return;  // 0xa4b951
  case fnCrocomire_Func_94: Crocomire_Func_94(); return;  // 0xa4b968
  case fnCrocomire_Func_95: Crocomire_Func_95(); return;  // 0xa4ba05
  case fnCrocomire_Func_1: Crocomire_Func_1(); return;  // 0xa4bab4
  case fnEnemy_NormalShotAI_A5: Enemy_NormalShotAI_A5(); return;  // 0xa5802d
  case fnCreateADudShot_A5: CreateADudShot(); return;  // 0xa58046
  case fnnullsub_170_A5: return;  // 0xa5804c
  case fnDraygon_Shot: Draygon_Shot(); return;  // 0xa595f0
  case fnSporeSpawn_Shot: SporeSpawn_Shot(); return;  // 0xa5ed5a
  case fnnullsub_170_A6: return;  // 0xa6804c
  case fnRidley_Shot: Ridley_Shot(); return;  // 0xa6df8a
  case fnnullsub_170_A7: return;  // 0xa7804c
  case fnnullsub_43: return;  // 0xa794b5
  case fnKraid_Arm_Shot: Kraid_Arm_Shot(j); return;  // 0xa794b6
  case fnPhantoon_Shot: Phantoon_Shot(); return;  // 0xa7dd9b
  case fnMotherBrainsBody_Shot: MotherBrainsBody_Shot(); return;  // 0xa9b503
  case fnMotherBrainsBrain_Shot: MotherBrainsBrain_Shot(); return;  // 0xa9b507
  case fnTorizo_Shot: Torizo_Shot(); return;  // 0xaac97c
  case fnnullsub_271: return;  // 0xaac9c1
  case fnTorizo_Func_8: Torizo_Func_8(); return;  // 0xaac9c2
  case fnWalkingSpacePirates_Shot: WalkingSpacePirates_Shot(); return;  // 0xb28779
  case fnWalkingSpacePirates_87C8: WalkingSpacePirates_87C8(); return;  // 0xb287c8
  case fnWalkingSpacePirates_883E: WalkingSpacePirates_883E(); return;  // 0xb2883e
  default: Unreachable();
  }
}

void EnemySamusCollHandler_Multibox(void) {  // 0xA09A5A
  VoidP touch_ai;
  ExtendedSpriteMap *ExtendedSpriteMap;
  VoidP hitbox_ptr_;
  Hitbox *Hitbox;

  uint16 *v0 = (uint16 *)gEnemyData(cur_enemy_index);
  *(uint16 *)&enemy_ai_pointer.bank = v0[23];
  enemy_processing_stage = 6;
  if (v0[11]) {
    touch_ai = get_EnemyDef_A2(*v0)->touch_ai;
    if (touch_ai != (uint16)FUNC16(nullsub_170) && touch_ai != (uint16)FUNC16(nullsub_169)) {
      if (samus_contact_damage_index) {
        samus_invincibility_timer = 0;
      } else if (samus_invincibility_timer) {
        return;
      }
      EnemyData *v2 = gEnemyData(cur_enemy_index);
      if (!sign16(v2->spritemap_pointer + 0x8000)) {
        samus_right_border_coll = samus_x_radius + samus_x_pos;
        samus_left_border_coll = samus_x_pos - samus_x_radius;
        samus_bottom_border_coll = samus_y_radius + samus_y_pos;
        samus_top_border_coll = samus_y_pos - samus_y_radius;
        uint16 spritemap_pointer = v2->spritemap_pointer;
        remaining_enemy_spritemap_entries = *RomPtrWithBank(enemy_ai_pointer.bank, spritemap_pointer);
        enemy_spritemap_entry_pointer = spritemap_pointer + 2;
        uint16 *v7;
        while (1) {
          EnemyData *v4;
          v4 = gEnemyData(cur_enemy_index);
          ExtendedSpriteMap = get_ExtendedSpriteMap(enemy_ai_pointer.bank, enemy_spritemap_entry_pointer);
          enemy_spritemap_entry_coll_x_pos = ExtendedSpriteMap->xpos + v4->x_pos;
          num_projs_to_check = ExtendedSpriteMap->ypos + v4->y_pos;
          hitbox_ptr_ = ExtendedSpriteMap->hitbox_ptr_;
          v7 = (uint16 *)RomPtrWithBank(enemy_ai_pointer.bank, hitbox_ptr_);
          if (*v7)
            break;
LABEL_18:
          enemy_spritemap_entry_pointer += 8;
          if ((int16)--remaining_enemy_spritemap_entries <= 0)
            return;
        }
        remaining_enemy_hitbox_entries = *v7;
        hitbox_ptr = hitbox_ptr_ + 2;
        while (1) {
          Hitbox = get_Hitbox(enemy_ai_pointer.bank, hitbox_ptr);
          if ((int16)(Hitbox->left + enemy_spritemap_entry_coll_x_pos - samus_right_border_coll) < 0
              && (int16)(Hitbox->right + enemy_spritemap_entry_coll_x_pos - samus_left_border_coll) >= 0
              && (int16)(Hitbox->top + num_projs_to_check - samus_bottom_border_coll) < 0
              && (int16)(Hitbox->bottom + num_projs_to_check - samus_top_border_coll) >= 0) {
            break;
          }
          hitbox_ptr += 12;
          if ((int16)--remaining_enemy_hitbox_entries <= 0)
            goto LABEL_18;
        }
        enemy_ai_pointer.addr = get_Hitbox(enemy_ai_pointer.bank, hitbox_ptr)->func_ptr;
        CallHitboxTouch(Load24(&enemy_ai_pointer));
      }
    }
  }
}

void EnemyProjectileCollHandler_Multibox(void) {  // 0xA09B7F
  VoidP shot_ai;
  int16 v5;
  Hitbox *v11;
  Hitbox *v12;
  Hitbox *Hitbox;

  *(uint16 *)&enemy_ai_pointer.bank = *(uint16 *)&gEnemyData(cur_enemy_index)->bank;
  enemy_processing_stage = 3;
  if (projectile_counter) {
    num_projectiles_to_check_enemy_coll = projectile_counter;
    EnemyData *v0 = gEnemyData(cur_enemy_index);
    uint16 spritemap_pointer = v0->spritemap_pointer;
    if (spritemap_pointer) {
      if (spritemap_pointer != (uint16)addr_kExtendedSpritemap_Nothing_A0) {
        shot_ai = get_EnemyDef_A2(v0->enemy_ptr)->shot_ai;
        if (shot_ai != (uint16)FUNC16(nullsub_170) && shot_ai != (uint16)FUNC16(nullsub_169)) {
          EnemyData *v3 = gEnemyData(cur_enemy_index);
          if ((v3->properties & 0x400) == 0
              && !v3->invincibility_timer
              && v3->enemy_ptr != (uint16)addr_kEnemyDef_DAFF) {
            collision_detection_index = 0;
            while (1) {
              uint16 v4;
              v4 = projectile_type[collision_detection_index];
              if (v4) {
                v5 = v4 & 0xF00;
                if (v5 != 768 && v5 != 1280 && sign16(v5 - 1792))
                  break;
              }
LABEL_37:
              if (!sign16(++collision_detection_index - 5))
                return;
            }
            EnemyData *v6 = gEnemyData(cur_enemy_index);
            while ((int16)(v6->spritemap_pointer + 0x8000) < 0)
              ;
            uint16 v7 = v6->spritemap_pointer;
            uint16 bottom;
            remaining_enemy_spritemap_entries = *RomPtrWithBank(enemy_ai_pointer.bank, v7);
            enemy_spritemap_entry_pointer = v7 + 2;
            while (1) {
              EnemyData *v8;
              v8 = gEnemyData(cur_enemy_index);
              Hitbox = get_Hitbox(enemy_ai_pointer.bank, enemy_spritemap_entry_pointer);
              enemy_spritemap_entry_coll_x_pos = Hitbox->left + v8->x_pos;
              num_projs_to_check = Hitbox->top + v8->y_pos;
              bottom = Hitbox->bottom;
              v11 = get_Hitbox(enemy_ai_pointer.bank, bottom);
              if (v11->left)
                break;
LABEL_34:
              enemy_spritemap_entry_pointer += 8;
              bool v15 = (--remaining_enemy_spritemap_entries & 0x8000u) != 0;
              if (!remaining_enemy_spritemap_entries || v15)
                goto LABEL_37;
            }
            remaining_enemy_hitbox_entries = v11->left;
            hitbox_ptr = bottom + 2;
            int v13;
            while (1) {
              v12 = get_Hitbox(enemy_ai_pointer.bank, hitbox_ptr);
              enemy_left_border_collision = enemy_spritemap_entry_coll_x_pos + v12->left;
              v13 = collision_detection_index;
              if ((int16)(projectile_x_radius[v13] + projectile_x_pos[v13] - enemy_left_border_collision) >= 0) {
                enemy_right_border_collision = enemy_spritemap_entry_coll_x_pos + v12->right;
                if ((int16)(projectile_x_pos[v13] - projectile_x_radius[v13] - enemy_right_border_collision) < 0) {
                  enemy_bottom_border_collision = num_projs_to_check + v12->top;
                  if ((int16)(projectile_y_radius[v13] + projectile_y_pos[v13] - enemy_bottom_border_collision) >= 0) {
                    enemy_top_border_collision = num_projs_to_check + v12->bottom;
                    if ((int16)(projectile_y_pos[v13] - projectile_y_radius[v13] - enemy_top_border_collision) < 0)
                      break;
                  }
                }
              }
              hitbox_ptr += 12;
              bool v14 = (--remaining_enemy_hitbox_entries & 0x8000u) != 0;
              if (!remaining_enemy_hitbox_entries || v14)
                goto LABEL_34;
            }
            if ((projectile_type[v13] & 0xF00) == 512) {
              earthquake_timer = 30;
              earthquake_type = 18;
            }
            if ((gEnemyData(cur_enemy_index)->properties & 0x1000) != 0 || (projectile_type[v13] & 8) == 0)
              projectile_dir[v13] |= 0x10u;
            enemy_ai_pointer.addr = get_Hitbox(enemy_ai_pointer.bank, hitbox_ptr)->func_ptrA;
            CallHitboxShot(Load24(&enemy_ai_pointer), collision_detection_index * 2);
          }
        }
      }
    }
  }
}

void EnemyBombCollHandler_Multibox(void) {  // 0xA09D23
  VoidP shot_ai;
  Hitbox *v11;
  ExtendedSpriteMap *ExtendedSpriteMap;
  Hitbox *Hitbox;

  *(uint16 *)&enemy_ai_pointer.bank = *(uint16 *)&gEnemyData(cur_enemy_index)->bank;
  enemy_processing_stage = 4;
  if (gEnemyData(cur_enemy_index)->spritemap_pointer) {
    EnemyData *v0 = gEnemyData(cur_enemy_index);
    if ((v0->properties & 0x400) == 0 && !v0->invincibility_timer) {
      uint16 enemy_ptr = gEnemyData(cur_enemy_index)->enemy_ptr;
      shot_ai = get_EnemyDef_A2(enemy_ptr)->shot_ai;
      if (shot_ai != (uint16)FUNC16(nullsub_170)
          && shot_ai != (uint16)FUNC16(nullsub_169)
          && bomb_counter) {
        collision_detection_index = 5;
        while (1) {
          int v3;
          v3 = collision_detection_index;
          if (projectile_x_pos[v3]) {
            uint16 v4 = projectile_type[v3];
            if (v4) {
              if ((v4 & 0xF00) == 1280 && !projectile_variables[v3])
                break;
            }
          }
LABEL_26:
          if (++collision_detection_index == 10)
            return;
        }
        EnemyData *v5 = gEnemyData(cur_enemy_index);
        while ((int16)(v5->spritemap_pointer + 0x8000) < 0)
          ;
        uint16 spritemap_pointer = v5->spritemap_pointer;
        remaining_enemy_spritemap_entries = *RomPtrWithBank(enemy_ai_pointer.bank, spritemap_pointer);
        enemy_spritemap_entry_pointer = spritemap_pointer + 2;
        uint16 hitbox_ptr_;
        while (1) {
          EnemyData *v7;
          v7 = gEnemyData(cur_enemy_index);
          ExtendedSpriteMap = get_ExtendedSpriteMap(enemy_ai_pointer.bank, enemy_spritemap_entry_pointer);
          enemy_spritemap_entry_coll_x_pos = ExtendedSpriteMap->xpos + v7->x_pos;
          num_projs_to_check = ExtendedSpriteMap->ypos + v7->y_pos;
          hitbox_ptr_ = ExtendedSpriteMap->hitbox_ptr_;
          Hitbox = get_Hitbox(enemy_ai_pointer.bank, hitbox_ptr_);
          if (Hitbox->left)
            break;
LABEL_25:
          enemy_spritemap_entry_pointer += 8;
          if ((int16)--remaining_enemy_spritemap_entries <= 0)
            goto LABEL_26;
        }
        remaining_enemy_hitbox_entries = Hitbox->left;
        hitbox_ptr = hitbox_ptr_ + 2;
        int v12;
        while (1) {
          v11 = get_Hitbox(enemy_ai_pointer.bank, hitbox_ptr);
          enemy_left_border_collision = enemy_spritemap_entry_coll_x_pos + v11->left;
          v12 = collision_detection_index;
          if ((int16)(projectile_x_radius[v12] + projectile_x_pos[v12] - enemy_left_border_collision) >= 0) {
            enemy_right_border_collision = enemy_spritemap_entry_coll_x_pos + v11->right;
            if ((int16)(projectile_x_pos[v12] - projectile_x_radius[v12] - enemy_right_border_collision) < 0) {
              enemy_bottom_border_collision = num_projs_to_check + v11->top;
              if ((int16)(projectile_y_radius[v12] + projectile_y_pos[v12] - enemy_bottom_border_collision) >= 0) {
                enemy_top_border_collision = num_projs_to_check + v11->bottom;
                if ((int16)(projectile_y_pos[v12] - projectile_y_radius[v12] - enemy_top_border_collision) < 0)
                  break;
              }
            }
          }
          hitbox_ptr += 12;
          if ((int16)--remaining_enemy_hitbox_entries <= 0)
            goto LABEL_25;
        }
        projectile_dir[v12] |= 0x10u;
        enemy_ai_pointer.addr = get_Hitbox(enemy_ai_pointer.bank, hitbox_ptr)->func_ptrA;
        CallHitboxShot(Load24(&enemy_ai_pointer), collision_detection_index * 2);
      }
    }
  }
}

uint16 GrappleBeam_CollDetect_Enemy(void) {  // 0xA09E9A
  VoidP grapple_ai;
  EnemyData *v4;

  CallSomeSamusCode(0xDu);
  collision_detection_index = 0;
  for (interactive_enemy_indexes_index = 0; ; ++interactive_enemy_indexes_index) {
    uint16 v2 = interactive_enemy_indexes[interactive_enemy_indexes_index >> 1];
    cur_enemy_index = v2;
    if (v2 == 0xFFFF) {
      R18_ = 0;
      return 0;
    }
    v4 = gEnemyData(v2);
    if (!v4->invincibility_timer) {
      uint16 v5 = abs16(v4->x_pos - grapple_beam_end_x_pos);
      bool v6 = v5 < v4->x_width;
      uint16 v7 = v5 - v4->x_width;
      if (v6 || v7 < 8u) {
        uint16 v8 = abs16(v4->y_pos - grapple_beam_end_y_pos);
        v6 = v8 < v4->y_height;
        uint16 v9 = v8 - v4->y_height;
        if (v6 || v9 < 8u)
          break;
      }
    }
    ++interactive_enemy_indexes_index;
  }
  v4->ai_handler_bits = 1;
  uint16 v0 = 0;
  uint16 enemy_ptr = v4->enemy_ptr;
  grapple_ai = get_EnemyDef_A2(v4->enemy_ptr)->grapple_ai;
  if (grapple_ai + (uint16)FUNC16(Enemy_GrappleReact_NoInteract_A0)) {
    v0 = 1;
    if (grapple_ai != (uint16)FUNC16(Enemy_GrappleReact_SamusLatchesOn_A0)) {
      v0 = 2;
      if (grapple_ai != (uint16)FUNC16(Enemy_GrappleReact_KillEnemy_A0)) {
        v0 = 3;
        if (grapple_ai != (uint16)FUNC16(Enemy_GrappleReact_CancelBeam_A0)) {
          v0 = 4;
          if (grapple_ai != (uint16)FUNC16(Enemy_GrappleReact_SamusLatchesNoInvinc_A0)) {
            v0 = 5;
            if (grapple_ai != (uint16)FUNC16(Enemy_GrappleReact_SamusLatchesParalyze_A0)) {
              v0 = 6;
              if (grapple_ai != (uint16)FUNC16(Enemy_GrappleReact_HurtSamus_A0))
                v0 = 0;
            }
          }
        }
      }
    }
  }
  R18_ = enemy_ptr;
  uint16 result = v0;
  if (v0 == 1 || v0 == 4 || v0 == 5) {
    EnemyData *v11 = gEnemyData(cur_enemy_index);
    grapple_beam_end_x_pos = v11->x_pos;
    grapple_beam_end_y_pos = v11->y_pos;
    return v0;
  }
  return result;
}

void SwitchEnemyAiToMainAi(void) {  // 0xA09F6D
  EnemyData *v0 = gEnemyData(cur_enemy_index);
  v0->ai_handler_bits = 0;
  v0->invincibility_timer = 0;
  v0->frozen_timer = 0;
  v0->shake_timer = 0;
}

void SamusLatchesOnWithGrapple(void) {  // 0xA09F7D
  EnemyDef_A2 *EnemyDef_A2;

  EnemyData *v0 = gEnemyData(cur_enemy_index);
  grapple_beam_end_x_pos = v0->x_pos;
  grapple_beam_end_y_pos = v0->y_pos;
  if (v0->frozen_timer) {
    gEnemyData(cur_enemy_index)->ai_handler_bits = 4;
  } else {
    EnemyData *v1 = gEnemyData(cur_enemy_index);
    EnemyDef_A2 = get_EnemyDef_A2(v1->enemy_ptr);
    uint16 hurt_ai_time = EnemyDef_A2->hurt_ai_time;
    if (!EnemyDef_A2->hurt_ai_time)
      hurt_ai_time = 4;
    gEnemyData(cur_enemy_index)->flash_timer = hurt_ai_time;
    gEnemyData(cur_enemy_index)->ai_handler_bits = 0;
  }
}

void EnemyGrappleDeath(void) {  // 0xA09FC4
  gEnemySpawnData(cur_enemy_index)->cause_of_death = 4;
  EnemyDeathAnimation(cur_enemy_index, 0);
  gEnemyData(cur_enemy_index)->ai_handler_bits = 0;
}

void Enemy_SwitchToFrozenAi(void) {  // 0xA09FDF
  gEnemyData(cur_enemy_index)->ai_handler_bits = 4;
}

void SamusLatchesOnWithGrappleNoInvinc(void) {  // 0xA09FE9
  EnemyData *v0;

  if (gEnemyData(cur_enemy_index)->frozen_timer) {
    v0 = gEnemyData(cur_enemy_index);
    grapple_beam_end_x_pos = v0->x_pos;
    grapple_beam_end_y_pos = v0->y_pos;
    v0->ai_handler_bits = 4;
  } else {
    v0 = gEnemyData(cur_enemy_index);
    enemy_ai_pointer.addr = get_EnemyDef_A2(v0->enemy_ptr)->main_ai;
    *(uint16 *)&enemy_ai_pointer.bank = *(uint16 *)&gEnemyData(cur_enemy_index)->bank;
    CallEnemyAi(Load24(&enemy_ai_pointer));
    EnemyData *v1 = gEnemyData(cur_enemy_index);
    grapple_beam_end_x_pos = v1->x_pos;
    grapple_beam_end_y_pos = v1->y_pos;
    v1->ai_handler_bits = 0;
  }
}

void SamusLatchesOnWithGrappleParalyze(void) {  // 0xA0A03E
  EnemyDef_A2 *EnemyDef_A2;

  EnemyData *v0 = gEnemyData(cur_enemy_index);
  EnemyDef_A2 = get_EnemyDef_A2(v0->enemy_ptr);
  uint16 hurt_ai_time = EnemyDef_A2->hurt_ai_time;
  if (!EnemyDef_A2->hurt_ai_time)
    hurt_ai_time = 4;
  gEnemyData(cur_enemy_index)->flash_timer = hurt_ai_time;
  gEnemyData(cur_enemy_index)->ai_handler_bits = 0;
  EnemyData *v3 = gEnemyData(cur_enemy_index);
  v3->extra_properties |= 1u;
}

void SamusHurtFromGrapple(void) {  // 0xA0A070
  gEnemyData(cur_enemy_index)->ai_handler_bits = 4;
}

void EnemySamusCollHandler(void) {  // 0xA0A07A
  VoidP touch_ai;

  enemy_processing_stage = 9;
  if (gEnemyData(cur_enemy_index)->spritemap_pointer) {
    if (samus_contact_damage_index) {
      samus_invincibility_timer = 0;
    } else if (samus_invincibility_timer) {
      if (gEnemyData(cur_enemy_index)->enemy_ptr != (uint16)addr_kEnemyDef_DAFF)
        return;
      uint16 some_flag = gEnemySpawnData(cur_enemy_index)->some_flag;
      if (!some_flag || some_flag == 8)
        return;
    }
    uint16 enemy_ptr = gEnemyData(cur_enemy_index)->enemy_ptr;
    touch_ai = get_EnemyDef_A2(enemy_ptr)->touch_ai;
    if (touch_ai != (uint16)FUNC16(nullsub_170) && touch_ai != (uint16)FUNC16(nullsub_169)) {
      EnemyData *v3 = gEnemyData(cur_enemy_index);
      uint16 v4 = abs16(samus_x_pos - v3->x_pos);
      bool v5 = v4 < samus_x_radius;
      uint16 v6 = v4 - samus_x_radius;
      if (v5 || v6 < v3->x_width) {
        uint16 v7 = abs16(samus_y_pos - v3->y_pos);
        v5 = v7 < samus_y_radius;
        uint16 v8 = v7 - samus_y_radius;
        if (v5 || v8 < v3->y_height) {
          R20_ = 2 * gEnemyData(cur_enemy_index)->spritemap_pointer;
          if (gEnemyData(cur_enemy_index)->enemy_ptr == (uint16)addr_kEnemyDef_DAFF
              || !gEnemyData(cur_enemy_index)->frozen_timer) {
            enemy_ptr = gEnemyData(cur_enemy_index)->enemy_ptr;
            enemy_ai_pointer.addr = get_EnemyDef_A2(enemy_ptr)->touch_ai;
            enemy_ai_pointer.bank = gEnemyData(cur_enemy_index)->bank;
            CallEnemyAi(Load24(&enemy_ai_pointer));
          }
        }
      }
    }
  }
}

void EnemyProjectileCollHandler(void) {  // 0xA0A143
  int16 v4;

  *(uint16 *)&enemy_ai_pointer.bank = *(uint16 *)&gEnemyData(cur_enemy_index)->bank;
  enemy_processing_stage = 7;
  if (projectile_counter) {
    EnemyData *v0 = gEnemyData(cur_enemy_index);
    uint16 spritemap_pointer = v0->spritemap_pointer;
    if (spritemap_pointer) {
      if (spritemap_pointer != (uint16)addr_kSpritemap_Nothing_A0
          && (v0->properties & 0x400) == 0
          && v0->enemy_ptr != (uint16)addr_kEnemyDef_DAFF
          && !v0->invincibility_timer) {
        collision_detection_index = 0;
        int v2;
        while (1) {
          v2 = collision_detection_index;
          uint16 v3 = projectile_type[v2];
          if (v3) {
            v4 = v3 & 0xF00;
            if (v4 != 768 && v4 != 1280) {
              if (sign16(v4 - 1792)) {
                EnemyData *v5 = gEnemyData(cur_enemy_index);
                uint16 v6 = abs16(projectile_x_pos[v2] - v5->x_pos);
                bool v7 = v6 < projectile_x_radius[v2];
                uint16 v8 = v6 - projectile_x_radius[v2];
                if (v7 || v8 < v5->x_width) {
                  uint16 v9 = abs16(projectile_y_pos[v2] - v5->y_pos);
                  v7 = v9 < projectile_y_radius[v2];
                  uint16 v10 = v9 - projectile_y_radius[v2];
                  if (v7 || v10 < v5->y_height)
                    break;
                }
              }
            }
          }
          if (++collision_detection_index == 5)
            return;
        }
        if ((projectile_type[v2] & 0xF00) == 512) {
          earthquake_timer = 30;
          earthquake_type = 18;
        }
        uint16 v11 = 2 * collision_detection_index;
        if ((gEnemyData(cur_enemy_index)->properties & 0x1000) != 0 || (projectile_type[v11 >> 1] & 8) == 0)
          projectile_dir[v11 >> 1] |= 0x10u;
        uint16 enemy_ptr;
        enemy_ptr = gEnemyData(cur_enemy_index)->enemy_ptr;
        enemy_ai_pointer.addr = get_EnemyDef_A2(enemy_ptr)->shot_ai;
        CallEnemyAi(Load24(&enemy_ai_pointer));
      }
    }
  }
}

void EnemyBombCollHandler(void) {  // 0xA0A236
  int v8;
  *(uint16 *)&enemy_ai_pointer.bank = *(uint16 *)&gEnemyData(cur_enemy_index)->bank;
  enemy_processing_stage = 8;
  if (bomb_counter) {
    if (gEnemyData(cur_enemy_index)->spritemap_pointer) {
      EnemyData *v0 = gEnemyData(cur_enemy_index);
      if (!v0->invincibility_timer && v0->enemy_ptr != (uint16)addr_kEnemyDef_DAFF) {
        collision_detection_index = 5;
        while (1) {
          int v1 = collision_detection_index;
          if (projectile_type[v1]
              && !projectile_variables[v1]
              && ((projectile_type[v1] & 0xF00) == 1280 || (projectile_type[v1] & 0x8000u) != 0)) {
            EnemyData *v2 = gEnemyData(cur_enemy_index);
            uint16 v3 = abs16(projectile_x_pos[v1] - v2->x_pos);
            bool v4 = v3 < projectile_x_radius[v1];
            uint16 v5 = v3 - projectile_x_radius[v1];
            if (v4 || v5 < v2->x_width) {
              uint16 v6 = abs16(projectile_y_pos[v1] - v2->y_pos);
              v4 = v6 < projectile_y_radius[v1];
              uint16 v7 = v6 - projectile_y_radius[v1];
              if (v4 || v7 < v2->y_height) {
                v8 = collision_detection_index;
                if (!projectile_variables[v8])
                  break;
              }
            }
          }
          if (++collision_detection_index == 10)
            return;
        }
        projectile_dir[v8] |= 0x10u;
        uint16 enemy_ptr = gEnemyData(cur_enemy_index)->enemy_ptr;
        enemy_ai_pointer.addr = get_EnemyDef_A2(enemy_ptr)->shot_ai;
        CallEnemyAi(Load24(&enemy_ai_pointer));
      }
    }
  }
}

void ProcessEnemyPowerBombInteraction(void) {  // 0xA0A306
  VoidP powerbomb_reaction;

  enemy_processing_stage = 5;
  R18_ = HIBYTE(power_bomb_explosion_radius);
  if (HIBYTE(power_bomb_explosion_radius)) {
    R20_ = (uint16)(R18_ + (HIBYTE(power_bomb_explosion_radius) & 1) + (power_bomb_explosion_radius >> 9)) >> 1;
    cur_enemy_index = 1984;
    do {
      EnemyData *v0 = gEnemyData(cur_enemy_index);
      if (!v0->invincibility_timer) {
        uint16 enemy_ptr = v0->enemy_ptr;
        if (v0->enemy_ptr) {
          if (enemy_ptr != (uint16)addr_kEnemyDef_DAFF) {
            uint16 vulnerability_ptr = get_EnemyDef_A2(enemy_ptr)->vulnerability_ptr;
            if (!vulnerability_ptr)
              vulnerability_ptr = addr_stru_B4EC1C;
            if ((get_Vulnerability(vulnerability_ptr)->power_bomb & 0x7F) != 0) {
              EnemyData *v3 = gEnemyData(cur_enemy_index);
              if (abs16(power_bomb_explosion_x_pos - v3->x_pos) < R18_
                  && abs16(power_bomb_explosion_y_pos - v3->y_pos) < R20_) {
                powerbomb_reaction = get_EnemyDef_A2(v3->enemy_ptr)->powerbomb_reaction;
                if (!powerbomb_reaction)
                  powerbomb_reaction = FUNC16(Enemy_NormalPowerBombAI_A0);
                enemy_ai_pointer.addr = powerbomb_reaction;
                *(uint16 *)&enemy_ai_pointer.bank = *(uint16 *)&gEnemyData(cur_enemy_index)->bank;
                CallEnemyAi(Load24(&enemy_ai_pointer));
                EnemyData *v5 = gEnemyData(cur_enemy_index);
                v5->properties |= 0x800u;
              }
            }
          }
        }
      }
      cur_enemy_index -= 64;
    } while ((cur_enemy_index & 0x8000u) == 0);
  }
}

void EnemyDeathAnimation(uint16 k, uint16 a) {  // 0xA0A3AF
  int16 v3;

  if (gEnemyData(k)->ai_handler_bits == 1)
    grapple_beam_function = FUNC16(GrappleBeam_Func2);
  if (!sign16(a - 5))
    a = 0;
  enemy_population_ptr = a;
  SpawnEnemyProjectileWithGfx(a, cur_enemy_index, addr_kEproj_EnemyDeathExplosion);
  R18_ = gEnemyData(cur_enemy_index)->properties & 0x4000;
  v3 = 62;
  int v4 = cur_enemy_index;
  do {
    gEnemyData(v4)->enemy_ptr = 0;
    v4 += 2;
    v3 -= 2;
  } while (v3 >= 0);
  if (R18_) {
    EnemyData *v5 = gEnemyData(cur_enemy_index);
    v5->enemy_ptr = addr_kEnemyDef_DAFF;
    *(uint16 *)&v5->bank = 0xa3;
  }
  ++num_enemies_killed_in_room;
}

void RinkasDeathAnimation(uint16 a) {  // 0xA0A410
  int16 v2;

  if (!sign16(a - 3))
    a = 0;
  enemy_population_ptr = a;
  SpawnEnemyProjectileWithGfx(a, cur_enemy_index, addr_kEproj_EnemyDeathExplosion);
  R18_ = gEnemyData(cur_enemy_index)->properties & 0x4000;
  v2 = 62;
  int v4 = cur_enemy_index;
  do {
    gEnemyData(v4)->enemy_ptr = 0;
    v4 += 2;
    v2 -= 2;
  } while (v2 >= 0);
  if (R18_) {
    EnemyData *v4 = gEnemyData(cur_enemy_index);
    v4->enemy_ptr = addr_kEnemyDef_DAFF;
    *(uint16 *)&v4->bank = 163;
  }
}

uint16 SuitDamageDivision(uint16 a) {  // 0xA0A45E
  R18_ = a;
  if ((equipped_items & 0x20) != 0) {
    R18_ >>= 1;
    R18_ >>= 1;
    return R18_;
  } else {
    if (equipped_items & 1)
      R18_ >>= 1;
    return R18_;
  }
}

void NormalEnemyTouchAi(void) {  // 0xA0A477

  NormalEnemyTouchAiSkipDeathAnim();
  if (!gEnemyData(cur_enemy_index)->health) {
    gEnemySpawnData(cur_enemy_index)->cause_of_death = 6;
    EnemyDeathAnimation(cur_enemy_index, 1u);
  }
}

void NormalEnemyTouchAiSkipDeathAnim_CurEnemy(void) {  // 0xA0A497
  NormalEnemyTouchAiSkipDeathAnim();
}

void NormalEnemyTouchAiSkipDeathAnim(void) {  // 0xA0A4A1
  VoidP vulnerability_ptr;
  int16 hurt_ai_time;
  int16 v7;
  EnemyDef_A2 *v9;
  EnemyDef_A2 *EnemyDef_A2;

  if (samus_contact_damage_index) {
    R20_ = samus_contact_damage_index + 15;
    uint16 v0 = 500;
    if (samus_contact_damage_index != 1) {
      v0 = 300;
      if (samus_contact_damage_index != 2) {
        v0 = 2000;
        if (samus_contact_damage_index != 3) {
          ++R20_;
          v0 = 200;
          if (samus_contact_damage_index == 4)
            CallSomeSamusCode(4u);
          else
            v0 = 200;
        }
      }
    }
    R22_ = v0;
    EnemyData *v1 = gEnemyData(cur_enemy_index);
    vulnerability_ptr = get_EnemyDef_A2(v1->enemy_ptr)->vulnerability_ptr;
    if (!vulnerability_ptr)
      vulnerability_ptr = addr_stru_B4EC1C;
    enemy_damage_multiplier = *(uint16 *)&get_Vulnerability(R20_ + vulnerability_ptr)->power;
    draw_enemy_layer = enemy_damage_multiplier & 0x7F;
    if ((enemy_damage_multiplier & 0x7F) != 0) {
      R40 = draw_enemy_layer;
      R38 = R22_ >> 1;
      Mult32bit();
      if (R42) {
        R18_ = R42;
        EnemyData *v3 = gEnemyData(cur_enemy_index);
        EnemyDef_A2 = get_EnemyDef_A2(v3->enemy_ptr);
        hurt_ai_time = EnemyDef_A2->hurt_ai_time;
        if (!EnemyDef_A2->hurt_ai_time)
          hurt_ai_time = 4;
        EnemyData *v6 = gEnemyData(cur_enemy_index);
        v6->flash_timer = hurt_ai_time;
        v6->ai_handler_bits |= 2u;
        samus_invincibility_timer = 0;
        samus_knockback_timer = 0;
        v7 = v6->health - R18_;
        if (v7 < 0)
          v7 = 0;
        v6->health = v7;
        QueueSfx2_Max1(0xBu);
      }
    }
  } else {
    EnemyData *v8 = gEnemyData(cur_enemy_index);
    v9 = get_EnemyDef_A2(v8->enemy_ptr);
    uint16 v10 = SuitDamageDivision(v9->damage);
    Samus_DealDamage(v10);
    samus_invincibility_timer = 96;
    samus_knockback_timer = 5;
    knockback_x_dir = (int16)(samus_x_pos - gEnemyData(cur_enemy_index)->x_pos) >= 0;
  }
}
// 72078: conditional instruction was optimized away because ax.2<80u

void NormalEnemyPowerBombAi(void) {  // 0xA0A597
  NormalEnemyPowerBombAiSkipDeathAnim();
  if (!gEnemyData(cur_enemy_index)->health) {
    gEnemySpawnData(cur_enemy_index)->cause_of_death = 3;
    EnemyDeathAnimation(cur_enemy_index, 0);
  }
}

void NormalEnemyPowerBombAiSkipDeathAnim_CurEnemy(void) {  // 0xA0A5B7
  NormalEnemyPowerBombAiSkipDeathAnim();
}

void NormalEnemyPowerBombAiSkipDeathAnim(void) {  // 0xA0A5C1
  EnemyDef_A2 *EnemyDef_A2;
  int16 hurt_ai_time;

  EnemyData *v0 = gEnemyData(cur_enemy_index);
  uint16 vulnerability_ptr = get_EnemyDef_A2(v0->enemy_ptr)->vulnerability_ptr;
  if (!vulnerability_ptr)
    vulnerability_ptr = -5092;
  uint8 power_bomb = get_Vulnerability(vulnerability_ptr)->power_bomb;
  if (power_bomb != 255) {
    draw_enemy_layer = power_bomb & 0x7F;
    if ((power_bomb & 0x7F) != 0) {
      R38 = 100;
      R40 = draw_enemy_layer;
      Mult32bit();
      enemy_spritemap_entry_coll_x_pos = R42;
      if (R42) {
        gEnemyData(cur_enemy_index)->invincibility_timer = 48;
        EnemyData *v3 = gEnemyData(cur_enemy_index);
        EnemyDef_A2 = get_EnemyDef_A2(v3->enemy_ptr);
        hurt_ai_time = EnemyDef_A2->hurt_ai_time;
        if (!EnemyDef_A2->hurt_ai_time)
          hurt_ai_time = 4;
        EnemyData *v6 = gEnemyData(cur_enemy_index);
        v6->flash_timer = hurt_ai_time + 8;
        v6->ai_handler_bits |= 2u;
        uint16 health = v6->health;
        bool v9 = health < enemy_spritemap_entry_coll_x_pos;
        uint16 v8 = health - enemy_spritemap_entry_coll_x_pos;
        v9 = !v9;
        if (!v8 || !v9)
          v8 = 0;
        v6->health = v8;
      }
    }
  }
}

void NormalEnemyShotAi(void) {  // 0xA0A63D
  EnemyDef_A2 *EnemyDef_A2;

  varE2E = 0;
  NormalEnemyShotAiSkipDeathAnim();
  if (varE2E) {
    EnemyData *v1 = gEnemyData(cur_enemy_index);
    R18_ = v1->x_pos;
    R20_ = v1->y_pos;
    R22_ = 55;
    R24_ = 0;
    CreateSpriteAtPos();
  }
  EnemyData *v2 = gEnemyData(cur_enemy_index);
  if (!v2->health) {
    uint16 v3 = HIBYTE(projectile_type[collision_detection_index]) & 0xF;
    gEnemySpawnData(cur_enemy_index)->cause_of_death = v3;
    uint16 death_anim = 2;
    uint16 enemy_ptr;
    if (v3 == 2) {
      enemy_ptr = gEnemyData(cur_enemy_index)->enemy_ptr;
      EnemyDef_A2 = get_EnemyDef_A2(enemy_ptr);
      if (!sign16(EnemyDef_A2->death_anim - 3))
        death_anim = EnemyDef_A2->death_anim;
    } else {
      enemy_ptr = v2->enemy_ptr;
      death_anim = get_EnemyDef_A2(v2->enemy_ptr)->death_anim;
    }
    EnemyDeathAnimation(enemy_ptr, death_anim);
  }
}

void NormalEnemyShotAiSkipDeathAnim_CurEnemy(void) {  // 0xA0A6A7
  varE2E = 0;
  NormalEnemyShotAiSkipDeathAnim();
}

void EnemyFunc_A6B4_UsedBySporeSpawn(void) {  // 0xA0A6B4
  varE2E = 0;
  NormalEnemyShotAiSkipDeathAnim();
  if (varE2E) {
    EnemyData *v0 = gEnemyData(cur_enemy_index);
    R18_ = v0->x_pos;
    R20_ = v0->y_pos;
    R22_ = 55;
    R24_ = 0;
    CreateSpriteAtPos();
  }
}

void NormalEnemyShotAiSkipDeathAnim(void) {  // 0xA0A6DE
  int16 v5;
  int16 v6;
  int16 v9;
  EnemyDef_A2 *EnemyDef_A2;
  int16 hurt_ai_time;
  int16 v20;

  int v0 = collision_detection_index;
  enemy_spritemap_entry_coll_x_pos = projectile_damage[v0];
  R18_ = projectile_type[v0];
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  uint16 vulnerability_ptr = get_EnemyDef_A2(v1->enemy_ptr)->vulnerability_ptr;
  if (!vulnerability_ptr)
    vulnerability_ptr = addr_stru_B4EC1C;
  R20_ = vulnerability_ptr;
  if ((R18_ & 0xF00) != 0) {
    v5 = R18_ & 0xF00;
    if ((R18_ & 0xF00) == 256 || v5 == 512) {
      LOBYTE(v6) = (uint16)(R18_ & 0xF00) >> 8;
      HIBYTE(v6) = 0;
      draw_enemy_layer = get_Vulnerability(R20_ + v6)->plasma_ice_wave & 0x7F;
    } else if (v5 == 1280) {
      draw_enemy_layer = get_Vulnerability(R20_)->bomb & 0x7F;
    } else {
      if (v5 != 768)
        goto LABEL_18;
      draw_enemy_layer = get_Vulnerability(R20_)->power_bomb & 0x7F;
    }
LABEL_9:
    R38 = enemy_spritemap_entry_coll_x_pos >> 1;
    R40 = draw_enemy_layer;
    Mult32bit();
    if (R42) {
      enemy_spritemap_entry_coll_x_pos = R42;
      EnemyData *v10 = gEnemyData(cur_enemy_index);
      EnemyDef_A2 = get_EnemyDef_A2(v10->enemy_ptr);
      hurt_ai_time = EnemyDef_A2->hurt_ai_time;
      if (!EnemyDef_A2->hurt_ai_time)
        hurt_ai_time = 4;
      EnemyData *v13 = gEnemyData(cur_enemy_index);
      v13->flash_timer = hurt_ai_time + 8;
      v13->ai_handler_bits |= 2u;
      if (!v13->frozen_timer) {
        uint16 hurt_sfx = get_EnemyDef_A2(v13->enemy_ptr)->hurt_sfx;
        if (hurt_sfx)
          QueueSfx2_Max3(hurt_sfx);
        ++varE2E;
      }
      uint16 v15 = cur_enemy_index;
      if ((projectile_type[collision_detection_index] & 8) != 0)
        gEnemyData(cur_enemy_index)->invincibility_timer = 16;
      EnemyData *v16 = gEnemyData(v15);
      uint16 health = v16->health;
      bool v19 = health < enemy_spritemap_entry_coll_x_pos;
      uint16 v18 = health - enemy_spritemap_entry_coll_x_pos;
      v19 = !v19;
      if (!v18 || !v19) {
        if ((projectile_type[collision_detection_index] & 2) != 0
            && (enemy_damage_multiplier & 0xF0) != 128
            && !v16->frozen_timer) {
          v20 = 400;
          if (area_index == 2)
            v20 = 300;
          v16->frozen_timer = v20;
          v16->ai_handler_bits |= 4u;
          v16->invincibility_timer = 10;
          QueueSfx3_Max3(0xAu);
          return;
        }
        v18 = 0;
      }
      v16->health = v18;
      return;
    }
LABEL_18:;
    int v7 = collision_detection_index;
    projectile_dir[v7] |= 0x10u;
    R18_ = projectile_x_pos[v7];
    R20_ = projectile_y_pos[v7];
    R22_ = 6;
    R24_ = 0;
    CreateSpriteAtPos();
    QueueSfx1_Max3(0x3Du);
    return;
  }
  enemy_damage_multiplier = get_Vulnerability(R20_ + ((uint8)R18_ & 0xFu))->power;
  draw_enemy_layer = enemy_damage_multiplier & 0x7F;
  if (enemy_damage_multiplier != 255) {
    if ((R18_ & 0x10) != 0) {
      uint8 charged_beam = get_Vulnerability(R20_)->charged_beam;
      if (charged_beam == 255)
        goto LABEL_18;
      uint16 v4 = charged_beam & 0xF;
      if (!v4)
        goto LABEL_18;
      draw_enemy_layer = v4;
    }
    goto LABEL_9;
  }
  EnemyData *v8 = gEnemyData(cur_enemy_index);
  if (!v8->frozen_timer)
    QueueSfx3_Max3(0xAu);
  v9 = 400;
  if (area_index == 2)
    v9 = 300;
  v8->frozen_timer = v9;
  v8->ai_handler_bits |= 4u;
  v8->invincibility_timer = 10;
}

void CreateDudShot(void) {  // 0xA0A8BC
  int v0 = collision_detection_index;
  R18_ = projectile_x_pos[v0];
  R20_ = projectile_y_pos[v0];
  R22_ = 6;
  R24_ = 0;
  CreateSpriteAtPos();
  QueueSfx1_Max3(0x3Du);
  projectile_dir[collision_detection_index] |= 0x10u;
}

uint16 Samus_CheckSolidEnemyColl(void) {  // 0xA0A8F0
  int16 v1;
  int16 v16;
  int16 v18;

  if (!interactive_enemy_indexes_write_ptr)
    return 0;
  v1 = 2 * (samus_collision_direction & 3);
  if (v1) {
    switch (v1) {
    case 2: {
      samus_target_x_pos = samus_x_pos + R18_;
      bool v3 = samus_x_subpos + R20_ == 0;
      if (__CFADD__uint16(samus_x_subpos, R20_))
        v3 = samus_target_x_pos++ == 0xFFFF;
      if (!v3)
        ++samus_target_x_pos;
      samus_target_y_pos = samus_y_pos;
      samus_target_y_subpos = samus_y_subpos;
      break;
    }
    case 4: {
      samus_target_y_pos = samus_y_pos - R18_;
      bool v4 = samus_y_subpos == R20_;
      if (samus_y_subpos < R20_)
        v4 = samus_target_y_pos-- == 1;
      if (!v4)
        --samus_target_y_pos;
      samus_target_x_pos = samus_x_pos;
      samus_target_x_subpos = samus_x_subpos;
      break;
    }
    case 6: {
      samus_target_y_pos = samus_y_pos + R18_;
      bool v5 = samus_y_subpos + R20_ == 0;
      if (__CFADD__uint16(samus_y_subpos, R20_))
        v5 = samus_target_y_pos++ == 0xFFFF;
      if (!v5)
        ++samus_target_y_pos;
      samus_target_x_pos = samus_x_pos;
      samus_target_x_subpos = samus_x_subpos;
      break;
    }
    default:
      Unreachable();
      while (1)
        ;
    }
  } else {
    samus_target_x_pos = samus_x_pos - R18_;
    bool v2 = samus_x_subpos == R20_;
    if (samus_x_subpos < R20_)
      v2 = samus_target_x_pos-- == 1;
    if (!v2)
      --samus_target_x_pos;
    samus_target_y_pos = samus_y_pos;
    samus_target_y_subpos = samus_y_subpos;
  }
  samus_x_radius_mirror = samus_x_radius;
  samus_y_radius_mirror = samus_y_radius;
  collision_detection_index = 0;
  for (interactive_enemy_indexes_index = 0; ; ++interactive_enemy_indexes_index) {
    int v6 = interactive_enemy_indexes_index >> 1;
    uint16 v7 = interactive_enemy_indexes[v6];
    if (v7 == 0xFFFF)
      break;
    collision_detection_index = interactive_enemy_indexes[v6];
    EnemyData *v8 = gEnemyData(v7);
    if (v8->frozen_timer || (v8->properties & 0x8000u) != 0) {
      uint8 *v9 = RomPtr_7E(v7 + 3962);
      uint8 *v10 = (uint8*)&samus_target_x_pos;
      uint16 v11 = abs16(*(uint16 *)v9 - *(uint16 *)v10);
      bool v12 = v11 < *((uint16 *)v9 + 4);
      uint16 v13 = v11 - *((uint16 *)v9 + 4);
      if (v12 || v13 < *((uint16 *)v10 + 4)) {
        uint16 v14 = abs16(*((uint16 *)v9 + 2) - *((uint16 *)v10 + 2));
        v12 = v14 < *((uint16 *)v9 + 5);
        uint16 v15 = v14 - *((uint16 *)v9 + 5);
        if (v12 || v15 < *((uint16 *)v10 + 5)) {
          v16 = 2 * (samus_collision_direction & 3);
          if (v16) {
            switch (v16) {
            case 2: {
              draw_enemy_layer = samus_x_radius + samus_x_pos;
              EnemyData *v19 = gEnemyData(collision_detection_index);
              v18 = v19->x_pos - v19->x_width - (samus_x_radius + samus_x_pos);
              if (!v18)
                goto LABEL_57;
              if (v18 >= 0)
                goto LABEL_58;
              break;
            }
            case 4: {
              EnemyData *v20 = gEnemyData(collision_detection_index);
              draw_enemy_layer = v20->y_height + v20->y_pos;
              v18 = samus_y_pos - samus_y_radius - draw_enemy_layer;
              if (samus_y_pos - samus_y_radius == draw_enemy_layer)
                goto LABEL_57;
              if ((int16)(samus_y_pos - samus_y_radius - draw_enemy_layer) >= 0)
                goto LABEL_58;
              break;
            }
            case 6: {
              draw_enemy_layer = samus_y_radius + samus_y_pos;
              EnemyData *v21;
              v21 = gEnemyData(collision_detection_index);
              v18 = v21->y_pos - v21->y_height - (samus_y_radius + samus_y_pos);
              if (!v18) {
LABEL_57:
                samus_y_subpos = 0;
                samus_x_pos_colliding_solid = samus_x_pos;
                samus_x_subpos_colliding_solid = samus_x_subpos;
                EnemyData *v22 = gEnemyData(collision_detection_index);
                enemy_x_pos_colliding_solid = v22->x_pos;
                enemy_x_subpos_colliding_solid = v22->x_subpos;
                samus_pos_delta_colliding_solid = R18_;
                samus_subpos_delta_colliding_solid = R20_;
                samus_y_pos_colliding_solid = samus_y_pos;
                samus_y_subpos_colliding_solid = 0;
                solid_enemy_collision_type = 1;
                R18_ = 0;
                R20_ = 0;
                R22_ = collision_detection_index;
                int v23 = samus_collision_direction & 3;
                enemy_index_colliding_dirs[v23] = collision_detection_index;
                distance_to_enemy_colliding_dirs[v23] = 0;
                return -1;
              }
              if (v18 >= 0) {
LABEL_58:
                samus_x_pos_colliding_solid = samus_x_pos;
                samus_x_subpos_colliding_solid = samus_x_subpos;
                EnemyData *v24 = gEnemyData(collision_detection_index);
                enemy_x_pos_colliding_solid = v24->x_pos;
                enemy_x_subpos_colliding_solid = v24->x_subpos;
                samus_pos_delta_colliding_solid = R18_;
                samus_subpos_delta_colliding_solid = R20_;
                samus_y_pos_colliding_solid = samus_y_pos;
                samus_y_subpos_colliding_solid = samus_y_subpos;
                solid_enemy_collision_type = 2;
                R18_ = v18;
                int v25 = samus_collision_direction & 3;
                distance_to_enemy_colliding_dirs[v25] = v18;
                R20_ = 0;
                R22_ = collision_detection_index;
                enemy_index_colliding_dirs[v25] = collision_detection_index;
                return -1;
              }
              break;
            }
            default:
              Unreachable();
              while (1)
                ;
            }
          } else {
            EnemyData *v17 = gEnemyData(collision_detection_index);
            draw_enemy_layer = v17->x_width + v17->x_pos;
            v18 = samus_x_pos - samus_x_radius - draw_enemy_layer;
            if (samus_x_pos - samus_x_radius == draw_enemy_layer)
              goto LABEL_57;
            if ((int16)(samus_x_pos - samus_x_radius - draw_enemy_layer) >= 0)
              goto LABEL_58;
          }
        }
      }
    }
    ++interactive_enemy_indexes_index;
  }
  return 0;
}

uint16 CheckIfEnemyTouchesSamus(uint16 k) {  // 0xA0ABE7
  EnemyData *v1 = gEnemyData(k);
  uint16 v2 = abs16(samus_x_pos - v1->x_pos);
  bool v3 = v2 < samus_x_radius;
  uint16 v4 = v2 - samus_x_radius;
  if (!v3 && v4 >= v1->x_width)
    return 0;
  if ((int16)(samus_y_pos + 3 - v1->y_pos) < 0) {
    uint16 v6 = v1->y_pos - (samus_y_pos + 3);
    v3 = v6 < samus_y_radius;
    uint16 v7 = v6 - samus_y_radius;
    if (v3 || v7 == v1->y_height || v7 < v1->y_height)
      return -1;
  }
  return 0;
}

uint16 EnemyFunc_AC67(uint16 k) {  // 0xA0AC67
  EnemyData *v1 = gEnemyData(k);
  uint16 v2 = abs16(samus_x_pos - v1->x_pos);
  bool v3 = v2 < samus_x_radius;
  uint16 v4 = v2 - samus_x_radius;
  if (!v3 && v4 >= v1->x_width && v4 >= 8u)
    return 0;
  uint16 v6 = abs16(samus_y_pos - v1->y_pos);
  v3 = v6 < samus_y_radius;
  uint16 v7 = v6 - samus_y_radius;
  if (v3 || v7 < v1->y_height)
    return -1;
  else
    return 0;
}

uint16 EnemyFunc_ACA8(void) {  // 0xA0ACA8
  int16 v1;
  int16 v2;

  g_word_7E0E3C = loop_index_end - draw_enemy_layer;
  uint16 result = abs16(loop_index_end - draw_enemy_layer);
  if (sign16(result - 255)) {
    R18_ = result;
    enemy_population_ptr = result;
    g_word_7E0E3E = loop_index - enemy_drawing_queue_index;
    result = abs16(loop_index - enemy_drawing_queue_index);
    if (sign16(result - 255)) {
      R20_ = result;
      draw_oam_x_offset = result;
      varE24 = CalculateAngleFromXY();
      draw_enemy_layer = enemy_population_ptr;
      v1 = SineMult8bitNegative(varE24);
      if (v1 < 0)
        v1 = -v1;
      varE26 = v1;
      draw_enemy_layer = draw_oam_x_offset;
      v2 = CosineMult8bit(varE24);
      if (v2 < 0)
        v2 = -v2;
      uint16 v3 = varE26 + v2;
      R18_ = g_word_7E0E3C;
      R20_ = g_word_7E0E3E;
      enemy_drawing_queue_base = CalculateAngleFromXY();
      return v3;
    }
  }
  return result;
}

uint16 CheckIfEnemyIsOnScreen(void) {  // 0xA0AD70
  EnemyData *v0 = gEnemyData(cur_enemy_index);
  return (int16)(v0->x_pos - layer1_x_pos) < 0
    || (int16)(layer1_x_pos + 256 - v0->x_pos) < 0
    || (int16)(v0->y_pos - layer1_y_pos) < 0
    || (int16)(layer1_y_pos + 256 - v0->y_pos) < 0;
}

uint16 EnemyFunc_ADA3(uint16 a) {  // 0xA0ADA3
  R18_ = a;
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  return (int16)(a + v1->x_pos - layer1_x_pos) < 0
    || (int16)(R18_ + layer1_x_pos + 256 - v1->x_pos) < 0
    || (int16)(R18_ + v1->y_pos - layer1_y_pos) < 0
    || (int16)(R18_ + layer1_y_pos + 256 - v1->y_pos) < 0;
}

uint16 EnemyWithNormalSpritesIsOffScreen(void) {  // 0xA0ADE7
  EnemyData *v0 = gEnemyData(cur_enemy_index);
  return (int16)(v0->x_width + v0->x_pos - layer1_x_pos) < 0
    || (int16)(v0->x_width + layer1_x_pos + 256 - v0->x_pos) < 0
    || (int16)(v0->y_pos + 8 - layer1_y_pos) < 0
    || (int16)(layer1_y_pos + 248 - v0->y_pos) < 0;
}

uint16 DetermineDirectionOfSamusFromEnemy(void) {  // 0xA0AE29
  if (IsSamusWithinEnemy_Y(cur_enemy_index, 0x20)) {
    uint16 v1 = 2;
    if ((GetSamusEnemyDelta_X(cur_enemy_index) & 0x8000u) != 0)
      return 7;
    return v1;
  } else if (IsSamusWithinEnemy_X(cur_enemy_index, 0x20)) {
    uint16 v3 = 4;
    if ((GetSamusEnemyDelta_Y(cur_enemy_index) & 0x8000u) != 0)
      return 0;
    return v3;
  } else if ((GetSamusEnemyDelta_X(cur_enemy_index) & 0x8000u) != 0) {
    uint16 v5 = 6;
    if ((GetSamusEnemyDelta_Y(cur_enemy_index) & 0x8000u) != 0)
      return 8;
    return v5;
  } else {
    uint16 v4 = 3;
    if ((GetSamusEnemyDelta_Y(cur_enemy_index) & 0x8000u) != 0)
      return 1;
    return v4;
  }
}


uint16 GetSamusEnemyDelta_Y(uint16 k) {  // 0xA0AEDD
  return samus_y_pos - gEnemyData(k)->y_pos;
}

uint16 GetSamusEnemyDelta_X(uint16 k) {  // 0xA0AEE5
  return samus_x_pos - gEnemyData(k)->x_pos;
}

uint16 IsSamusWithinEnemy_Y(uint16 k, uint16 a) {  // 0xA0AEED
  enemy_population_ptr = a;
  EnemyData *v2 = gEnemyData(k);
  return (int16)(SubtractThenAbs16(v2->y_pos, samus_y_pos) - a) < 0;
}

uint16 IsSamusWithinEnemy_X(uint16 k, uint16 a) {  // 0xA0AF0B
  enemy_population_ptr = a;
  EnemyData *v2 = gEnemyData(k);
  return (int16)(SubtractThenAbs16(v2->x_pos, samus_x_pos) - a) < 0;
}

void Enemy_SubPos_X(uint16 k) {  // 0xA0AF5A
  EnemyData *v1 = gEnemyData(k);
  uint16 x_subpos = v1->x_subpos;
  bool v3 = x_subpos < R18_;
  v1->x_subpos = x_subpos - R18_;
  v1->x_pos -= v3 + R20_;
}

void Enemy_AddPos_X(uint16 k) {  // 0xA0AF6C
  EnemyData *v1 = gEnemyData(k);
  uint16 x_subpos = v1->x_subpos;
  bool v3 = __CFADD__uint16(R18_, x_subpos);
  v1->x_subpos = R18_ + x_subpos;
  v1->x_pos += R20_ + v3;
}

void Enemy_SubPos_Y(uint16 k) {  // 0xA0AF7E
  EnemyData *v1 = gEnemyData(k);
  uint16 y_subpos = v1->y_subpos;
  bool v3 = y_subpos < R18_;
  v1->y_subpos = y_subpos - R18_;
  v1->y_pos -= v3 + R20_;
}

void Enemy_AddPos_Y(uint16 k) {  // 0xA0AF90
  EnemyData *v1 = gEnemyData(k);
  uint16 y_subpos = v1->y_subpos;
  bool v3 = __CFADD__uint16(R18_, y_subpos);
  v1->y_subpos = R18_ + y_subpos;
  v1->y_pos += R20_ + v3;
}

uint16 SignExtend8(uint16 a) {  // 0xA0AFEA
  draw_enemy_layer = a;
  if ((a & 0x80) != 0)
    return (uint8)draw_enemy_layer | 0xFF00;
  else
    return draw_enemy_layer;
}

uint16 Mult32(uint16 a) {  // 0xA0B002
  return 32 * a;
}

uint16 Abs16(uint16 a) {  // 0xA0B067
  draw_enemy_layer = a;
  if ((a & 0x8000) != 0)
    draw_enemy_layer = -draw_enemy_layer;
  return draw_enemy_layer;
}

uint16 SubtractThenAbs16(uint16 k, uint16 j) {  // 0xA0B07D
  draw_enemy_layer = k;
  enemy_drawing_queue_index = j - k;
  if ((int16)(j - k) < 0)
    enemy_drawing_queue_index = -enemy_drawing_queue_index;
  return enemy_drawing_queue_index;
}

uint16 CosineMult8bit(uint16 a) {  // 0xA0B0B2
  enemy_drawing_queue_index = (uint8)(a + 64);
  return SineMult8bit();
}

uint16 SineMult8bitNegative(uint16 a) {  // 0xA0B0C6
  enemy_drawing_queue_index = (uint8)(a + 0x80);
  return SineMult8bit();
}

#define kSine8bit ((uint8*)RomPtr(0xa0b143))
#define kEquationForQuarterCircle ((uint16*)RomPtr(0xa0b7ee))

uint16 SineMult8bit(void) {  // 0xA0B0DA
  int16 v1;

  uint16 RegWord = Mult8x8(kSine8bit[enemy_drawing_queue_index & 0x7F], draw_enemy_layer);
  LOBYTE(v1) = HIBYTE(RegWord);
  HIBYTE(v1) = RegWord;
  loop_index_end = HIBYTE(RegWord);
  loop_index = v1 & 0xFF00;
  if ((enemy_drawing_queue_index & 0x80) != 0) {
    loop_index_end = -loop_index_end;
    loop_index = -loop_index;
  }
  return loop_index_end;
}

void ConvertAngleToXy(void) {  // 0xA0B643
  R38 = kEquationForQuarterCircle[((uint8)R18_ + 0x80) & 0x7F];
  R40 = R20_;
  Mult32bit();
  R26_ = R44;
  R28_ = R42;
  R38 = kEquationForQuarterCircle[((uint8)R18_ + 64) & 0x7F];
  R40 = R20_;
  Mult32bit();
  R22_ = R44;
  R24_ = R42;
}

void EnemyFunc_B691(void) {  // 0xA0B691
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  uint16 x_subpos = v1->x_subpos;
  if (((enemy_population_ptr + 64) & 0x80) != 0) {
    bool v3 = x_subpos < varE26;
    v1->x_subpos = x_subpos - varE26;
    v1->x_pos -= v3 + varE24;
  } else {
    bool v3 = __CFADD__uint16(varE26, x_subpos);
    v1->x_subpos = varE26 + x_subpos;
    v1->x_pos += varE24 + v3;
  }
  EnemyData *v4 = gEnemyData(cur_enemy_index);
  uint16 y_subpos = v4->y_subpos;
  if (((enemy_population_ptr + 128) & 0x80) != 0) {
    bool v3 = y_subpos < varE2A;
    v4->y_subpos = y_subpos - varE2A;
    v4->y_pos -= v3 + varE28;
  } else {
    bool v3 = __CFADD__uint16(varE2A, y_subpos);
    v4->y_subpos = varE2A + y_subpos;
    v4->y_pos += varE28 + v3;
  }
}

void Mult32bit(void) {  // 0xA0B6FF
  uint32 t = (uint32)R38 * (uint32)R40;
  R42 = t;
  R44 = t >> 16;
}

void EnemyFunc_B761(void) {  // 0xA0B761
  int16 v0;
  unsigned int v1; // kr00_4

  R40 = 0;
  R38 = 0;
  if (__PAIR32__(R48, R46)) {
    v0 = 33;
    uint8 carry = 0, carry2;

    while (1) {
      carry2 = R44 >> 15;
      R44 = (R44 << 1) | (R42 >> 15);
      R42 = (R42 << 1) | carry;
      if (!--v0)
        break;
      carry = R38 >> 15;
      R40 = (R40 << 1) | (R38 >> 15);
      R38 = (R38 << 1) | carry2;

      if (__PAIR32__(R40, R38)) {
        carry = 0;
        v1 = __PAIR32__(R40, R38) - __PAIR32__(R48, R46);
        if (__PAIR32__(R40, R38) >= __PAIR32__(R48, R46)) {
          R40 = v1 >> 16;
          R38 = v1;
          carry = 1;
        }
      }
    }
  } else {
    R44 = 0;
    R42 = 0;
  }
}

void EnemyFunc_B7A1(void) {  // 0xA0B7A1
  int16 v1;
  int16 v3;

  R18_ = samus_y_pos - samus_prev_y_pos;
  uint16 v0 = Abs16(samus_y_pos - samus_prev_y_pos);
  if (!sign16(v0 - 12)) {
    v1 = -12;
    if ((R18_ & 0x8000u) == 0)
      v1 = 12;
    samus_prev_y_pos = samus_y_pos + v1;
  }
  R18_ = samus_x_pos - samus_prev_x_pos;
  uint16 v2 = Abs16(samus_x_pos - samus_prev_x_pos);
  if (!sign16(v2 - 12)) {
    v3 = -12;
    if ((R18_ & 0x8000u) == 0)
      v3 = 12;
    samus_prev_x_pos = samus_x_pos + v3;
  }
}

void Enemy_ItemDrop_MiniKraid(uint16 k) {  // 0xA0B8EE
  remaining_enemy_spritemap_entries = 4;
  do {
    R18_ = special_death_item_drop_x_origin_pos + (NextRandom() & 0x1F) - 16;
    R20_ = special_death_item_drop_y_origin_pos + ((uint16)(random_number & 0x1F00) >> 8) - 16;
    SpawnEnemyDrops(addr_kEnemyDef_E0FF, k);
    --remaining_enemy_spritemap_entries;
  } while (remaining_enemy_spritemap_entries);
}

void Enemy_ItemDrop_LowerNorfairSpacePirate(uint16 k) {  // 0xA0B92B
  remaining_enemy_spritemap_entries = 5;
  do {
    R18_ = special_death_item_drop_x_origin_pos + (NextRandom() & 0x1F) - 16;
    R20_ = special_death_item_drop_y_origin_pos + ((uint16)(random_number & 0x1F00) >> 8) - 16;
    SpawnEnemyDrops(addr_kEnemyDef_F593, k);
    --remaining_enemy_spritemap_entries;
  } while (remaining_enemy_spritemap_entries);
}

void Enemy_ItemDrop_Metroid(uint16 k) {  // 0xA0B968
  remaining_enemy_spritemap_entries = 5;
  do {
    R18_ = special_death_item_drop_x_origin_pos + (NextRandom() & 0x1F) - 16;
    R20_ = special_death_item_drop_y_origin_pos + ((uint16)(random_number & 0x1F00) >> 8) - 16;
    SpawnEnemyDrops(addr_kEnemyDef_DD7F, k);
    --remaining_enemy_spritemap_entries;
  } while (remaining_enemy_spritemap_entries);
}

void Enemy_ItemDrop_Ridley(uint16 k) {  // 0xA0B9A5
  remaining_enemy_spritemap_entries = 16;
  do {
    R18_ = (NextRandom() & 0x7F) + 64;
    R20_ = ((uint16)(random_number & 0x3F00) >> 8) + 320;
    SpawnEnemyDrops(addr_kEnemyDef_E17F, k);
    --remaining_enemy_spritemap_entries;
  } while (remaining_enemy_spritemap_entries);
}

void Enemy_ItemDrop_Crocomire(uint16 k) {  // 0xA0B9D8
  remaining_enemy_spritemap_entries = 16;
  do {
    R18_ = (NextRandom() & 0x7F) + 576;
    R20_ = ((uint16)(random_number & 0x3F00) >> 8) + 96;
    SpawnEnemyDrops(addr_kEnemyDef_DDBF, k);
    --remaining_enemy_spritemap_entries;
  } while (remaining_enemy_spritemap_entries);
}

void Enemy_ItemDrop_Phantoon(uint16 k) {  // 0xA0BA0B
  remaining_enemy_spritemap_entries = 16;
  do {
    R18_ = (NextRandom() & 0x7F) + 64;
    R20_ = ((uint16)(random_number & 0x3F00) >> 8) + 96;
    SpawnEnemyDrops(addr_kEnemyDef_E4BF, k);
    --remaining_enemy_spritemap_entries;
  } while (remaining_enemy_spritemap_entries);
}

void Enemy_ItemDrop_Botwoon(uint16 k) {  // 0xA0BA3E
  remaining_enemy_spritemap_entries = 16;
  do {
    R18_ = (NextRandom() & 0x7F) + 64;
    R20_ = ((uint16)(random_number & 0x3F00) >> 8) + 128;
    SpawnEnemyDrops(addr_kEnemyDef_F293, k);
    --remaining_enemy_spritemap_entries;
  } while (remaining_enemy_spritemap_entries);
}

void Enemy_ItemDrop_Kraid(uint16 k) {  // 0xA0BA71
  remaining_enemy_spritemap_entries = 16;
  do {
    R18_ = (uint8)NextRandom() + 128;
    R20_ = ((uint16)(random_number & 0x3F00) >> 8) + 352;
    SpawnEnemyDrops(addr_kEnemyDef_E2BF, k);
    --remaining_enemy_spritemap_entries;
  } while (remaining_enemy_spritemap_entries);
}

void Enemy_ItemDrop_BombTorizo(uint16 k) {  // 0xA0BAA4
  remaining_enemy_spritemap_entries = 16;
  do {
    R18_ = (NextRandom() & 0x7F) + 64;
    R20_ = ((uint16)(random_number & 0x3F00) >> 8) + 96;
    SpawnEnemyDrops(addr_kEnemyDef_EEFF, k);
    --remaining_enemy_spritemap_entries;
  } while (remaining_enemy_spritemap_entries);
}

void Enemy_ItemDrop_GoldenTorizo(uint16 k) {  // 0xA0BAD7
  remaining_enemy_spritemap_entries = 16;
  do {
    R18_ = (uint8)NextRandom() + 128;
    R20_ = ((uint16)(random_number & 0x3F00) >> 8) + 288;
    SpawnEnemyDrops(addr_kEnemyDef_EEFF, k);
    --remaining_enemy_spritemap_entries;
  } while (remaining_enemy_spritemap_entries);
}

void Enemy_ItemDrop_SporeSpawn(uint16 k) {  // 0xA0BB0A
  remaining_enemy_spritemap_entries = 16;
  do {
    R18_ = (NextRandom() & 0x7F) + 64;
    R20_ = ((uint16)(random_number & 0x3F00) >> 8) + 528;
    SpawnEnemyDrops(addr_kEnemyDef_DF3F, k);
    --remaining_enemy_spritemap_entries;
  } while (remaining_enemy_spritemap_entries);
}

void Enemy_ItemDrop_Draygon(uint16 k) {  // 0xA0BB3D
  remaining_enemy_spritemap_entries = 16;
  do {
    R18_ = (uint8)NextRandom() + 128;
    R20_ = ((uint16)(random_number & 0x3F00) >> 8) + 352;
    SpawnEnemyDrops(addr_kEnemyDef_DE3F, k);
    --remaining_enemy_spritemap_entries;
  } while (remaining_enemy_spritemap_entries);
}

uint8 CompareDistToSamus_X(uint16 k, uint16 a) {  // 0xA0BB9B
  EnemyData *v2 = gEnemyData(k);
  return abs16(samus_x_pos - v2->x_pos) >= a;
}

uint8 CompareDistToSamus_Y(uint16 k, uint16 a) {  // 0xA0BBAD
  EnemyData *v2 = gEnemyData(k);
  return abs16(samus_y_pos - v2->y_pos) >= a;
}

uint8 EnemyFunc_BBBF(uint16 k) {  // 0xA0BBBF
  int16 v7;
  int16 v8;
  int16 v11;

  EnemyData *v1 = gEnemyData(k);
  R26_ = (v1->y_pos - v1->y_height) & 0xFFF0;
  R26_ = (uint16)(v1->y_height + v1->y_pos - 1 - R26_) >> 4;
  uint16 prod = Mult8x8((uint16)(v1->y_pos - v1->y_height) >> 4, room_width_in_blocks);
  uint16 v2 = (__PAIR32__(R20_, R18_) + __PAIR32__(v1->x_pos, v1->x_subpos)) >> 16;
  uint16 v3;
  R22_ = R18_ + v1->x_subpos;
  R24_ = v2;
  if ((R20_ & 0x8000u) != 0)
    v3 = v2 - v1->x_width;
  else
    v3 = v1->x_width + v2 - 1;
  R34 = v3;
  uint16 v4 = 2 * (prod + (v3 >> 4));
  while ((level_data[v4 >> 1] & 0x8000u) == 0) {
    v4 += room_width_in_blocks * 2;
    if ((--R26_ & 0x8000u) != 0)
      return 0;
  }
  R18_ = 0;
  if ((R20_ & 0x8000u) != 0) {
    v8 = R34 | 0xF;
    EnemyData *v9 = gEnemyData(k);
    v11 = v9->x_width + 1 + v8 - v9->x_pos;
    if (v11 >= 0)
      v11 = 0;
    R20_ = -v11;
    return 1;
  } else {
    EnemyData *v6 = gEnemyData(k);
    v7 = (R34 & 0xFFF0) - v6->x_width - v6->x_pos;
    if (v7 < 0)
      v7 = 0;
    R20_ = v7;
    return 1;
  }
}

uint8 EnemyFunc_BC76(uint16 k) {  // 0xA0BC76
  int16 v4;
  int16 v8;
  int16 v9;
  int16 v12;

  EnemyData *v1 = gEnemyData(k);
  R26_ = (v1->x_pos - v1->x_width) & 0xFFF0;
  R26_ = (uint16)(v1->x_width + v1->x_pos - 1 - R26_) >> 4;
  uint16 v2 = (__PAIR32__(R20_, R18_) + __PAIR32__(v1->y_pos, v1->y_subpos)) >> 16;
  uint16 v3;
  R22_ = R18_ + v1->y_subpos;
  R24_ = v2;
  if ((R20_ & 0x8000u) != 0)
    v3 = v2 - v1->y_height;
  else
    v3 = v1->y_height + v2 - 1;
  R34 = v3;
  uint16 prod = Mult8x8(v3 >> 4, room_width_in_blocks);
  v4 = (uint16)(v1->x_pos - v1->x_width) >> 4;
  for (int i = 2 * (prod + v4); (level_data[i >> 1] & 0x8000u) == 0; i += 2) {
    if ((--R26_ & 0x8000u) != 0)
      return 0;
  }
  R18_ = 0;
  if ((R20_ & 0x8000u) != 0) {
    v9 = R34 | 0xF;
    EnemyData *v10 = gEnemyData(k);
    v12 = v10->y_height + 1 + v9 - v10->y_pos;
    if (v12 >= 0)
      v12 = 0;
    R20_ = -v12;
    return 1;
  } else {
    EnemyData *v7 = gEnemyData(k);
    v8 = (R34 & 0xFFF0) - v7->y_height - v7->y_pos;
    if (v8 < 0)
      v8 = 0;
    R20_ = v8;
    return 1;
  }
}

uint8 EnemyFunc_BF8A(uint16 k, uint16 a) {  // 0xA0BF8A
  int16 v6;
  int16 v10;
  int16 v13;
  uint16 v4;

  R28_ = a;
  EnemyData *v2 = gEnemyData(k);
  R26_ = (v2->x_pos - v2->x_width) & 0xFFF0;
  R26_ = (uint16)(v2->x_width + v2->x_pos - 1 - R26_) >> 4;

  if (a & 1) {
    uint16 v3 = (__PAIR32__(R20_, R18_) + __PAIR32__(v2->y_pos, v2->y_subpos)) >> 16;
    R22_ = R18_ + v2->y_subpos;
    R24_ = v3;
    v4 = v2->y_height + v3 - 1;
  } else {
    uint16 v5 = (__PAIR32__(v2->y_pos, v2->y_subpos) - __PAIR32__(R20_, R18_)) >> 16;
    R22_ = v2->y_subpos - R18_;
    R24_ = v5;
    v4 = v5 - v2->y_height;
  }
  R34 = v4;
  uint16 prod = Mult8x8(v4 >> 4, room_width_in_blocks);
  v6 = (uint16)(v2->x_pos - v2->x_width) >> 4;
  for (int i = 2 * (prod + v6); (level_data[i >> 1] & 0x8000u) == 0; i += 2) {
    if ((--R26_ & 0x8000u) != 0)
      return 0;
  }
  R18_ = 0;
  if (R28_ & 1) {
    EnemyData *v9 = gEnemyData(k);
    v10 = (__PAIR32__((R34 & 0xFFF0) - v9->y_height, R34 & 0xFFF0) - __PAIR32__(v9->y_pos, v9->y_height)) >> 16;
    if (v10 < 0)
      v10 = 0;
    R20_ = v10;
    return 1;
  } else {
    EnemyData *v11 = gEnemyData(k);
    v13 = v11->y_height + 1 + (R34 | 0xF) - v11->y_pos;
    if (v13 >= 0)
      v13 = 0;
    R20_ = -v13;
    return 1;
  }
}

uint16 CalculateAngleOfSamusFromEproj(uint16 k) {  // 0xA0C04E
  int v1 = k >> 1;
  R18_ = samus_x_pos - enemy_projectile_x_pos[v1];
  R20_ = samus_y_pos - enemy_projectile_y_pos[v1];
  return CalculateAngleFromXY_();
}

uint16 CalculateAngleOfSamusFromEnemy(uint16 k) {  // 0xA0C066
  EnemyData *v1 = gEnemyData(k);
  R18_ = samus_x_pos - v1->x_pos;
  R20_ = samus_y_pos - v1->y_pos;
  return CalculateAngleFromXY_();
}

uint16 CalculateAngleOfEnemyXfromEnemyY(uint16 k, uint16 j) {  // 0xA0C096
  EnemyData *v2 = gEnemyData(k);
  EnemyData *v3 = gEnemyData(j);
  R18_ = v2->x_pos - v3->x_pos;
  R20_ = v2->y_pos - v3->y_pos;
  return CalculateAngleFromXY_();
}

uint16 CalculateAngleFromXY(void) {  // 0xA0C0AE
  return CalculateAngleFromXY_();
}

static Func_Y_Y *const funcs_758EE[4] = { CalculateAngleFromXY_1, CalculateAngleFromXY_2, CalculateAngleFromXY_4, CalculateAngleFromXY_7 };
static Func_Y_Y *const funcs_7587D[4] = { CalculateAngleFromXY_0, CalculateAngleFromXY_3, CalculateAngleFromXY_5, CalculateAngleFromXY_6 };
uint16 CalculateAngleFromXY_(void) {  // 0xA0C0B1
  uint16 v0 = 0;
  uint16 v1 = R18_;
  uint16 r;

  if ((R18_ & 0x8000u) != 0) {
    v0 = 4;
    v1 = -R18_;
  }
  R18_ = v1;
  uint16 v2 = R20_;
  if ((R20_ & 0x8000u) != 0) {
    v0 += 2;
    v2 = -R20_;
  }
  R20_ = v2;
  if (v2 < R18_) {
    uint16 div = SnesDivide(R20_ << 8, R18_);
    r = funcs_7587D[v0 >> 1](div);
  } else {
    uint16 div = SnesDivide(R18_ << 8, R20_);
    r = funcs_758EE[v0 >> 1](div);
  }

  return r;
}

uint16 CalculateAngleFromXY_0(uint16 div) {  // 0xA0C112
  return (uint8)((div >> 3) + 64);
}

uint16 CalculateAngleFromXY_1(uint16 div) {  // 0xA0C120
  R18_ = div >> 3;
  return (uint8)(0x80 - R18_);
}

uint16 CalculateAngleFromXY_2(uint16 div) {  // 0xA0C132
  return (uint8)(div >> 3);
}

uint16 CalculateAngleFromXY_3(uint16 div) {  // 0xA0C13C
  R18_ = div >> 3;
  return (uint8)(64 - R18_);
}

uint16 CalculateAngleFromXY_4(uint16 div) {  // 0xA0C14E
  return (div >> 3) + 0x80 & 0xff;
}

uint16 CalculateAngleFromXY_5(uint16 div) {  // 0xA0C15C
  R18_ = div >> 3;
  return (uint8)(-64 - R18_);
}

uint16 CalculateAngleFromXY_6(uint16 div) {  // 0xA0C16E
  return (uint8)((div >> 3) - 64);
}

uint16 CalculateAngleFromXY_7(uint16 div) {  // 0xA0C17C
  R18_ = div >> 3;
  return (uint8)-(int8)R18_;
}

uint8 IsEnemyLeavingScreen(uint16 k) {  // 0xA0C18E
  int16 x_pos;
  int16 v3;

  EnemyData *v1 = gEnemyData(k);
  x_pos = v1->x_pos;
  uint8 result = 1;
  if (x_pos >= 0) {
    v3 = v1->x_width + x_pos - layer1_x_pos;
    if (v3 >= 0 && (int16)(v3 - 256 - v1->x_width) < 0)
      return 0;
  }
  return result;
}

void ProcessEnemyInstructions(void) {  // 0xA0C26A
  EnemyData *ED = gEnemyData(cur_enemy_index);
  if ((ED->ai_handler_bits & 4) == 0) {
    if (ED->instruction_timer-- == 1) {
      assert(ED->current_instruction & 0x8000);
      uint8 *base_ptr = RomPtrWithBank(ED->bank, 0x8000) - 0x8000;
      const uint16 *pc = (const uint16 *)(base_ptr + ED->current_instruction);
      while ((*pc & 0x8000u) != 0) {
        enemy_ai_pointer.addr = *pc;
        pc = CallEnemyInstr(Load24(&enemy_ai_pointer), cur_enemy_index, pc + 1);
        if (!pc)
          return;
        if ((uintptr_t)pc < 0x10000)
          pc = (const uint16*)(base_ptr + (uintptr_t)pc);
      }
      ED->instruction_timer = pc[0];
      ED->spritemap_pointer = pc[1];
      ED->current_instruction = (uint8 *)pc + 4 - base_ptr;
      ED->extra_properties |= 0x8000;
    } else {
      ED->extra_properties &= ~0x8000;
    }
  }
}

uint8 ClearCarry_13(void) {  // 0xA0C2BC
  return 0;
}

uint8 SetCarry_4(void) {  // 0xA0C2BE
  return 1;
}

#define g_off_A0C2DA ((uint16*)RomPtr(0xa0c2da))

uint8 EnemyBlockCollReact_Spike(void) {  // 0xA0C2C0
  uint16 v0 = g_off_A0C2DA[BTS[cur_block_index] & 0x7F];
  if (!v0)
    return 1;
  SpawnPLM(v0);
  return 0;
}

uint8 EnemyBlockCollHorizReact_Slope(void) {  // 0xA0C2FA
  if ((BTS[cur_block_index] & 0x1Fu) >= 5) {
    uint16 v0 = BTS[cur_block_index];
    current_slope_bts = v0;
    return EnemyBlockCollHorizReact_Slope_NonSquare();
  } else {
    return EnemyBlockCollHorizReact_Slope_Square(cur_block_index, BTS[cur_block_index] & 0x1F);
  }
}

uint8 EnemyBlockCollVertReact_Slope(void) {  // 0xA0C319
  uint16 v0 = BTS[cur_block_index] & 0x1F;
  if (v0 >= 5u)
    return EnemyBlockCollVertReact_Slope_NonSquare();
  else
    return EnemyBlockCollVertReact_Slope_Square(v0, cur_block_index);
}

static const uint8 byte_A0C435[20] = {  // 0xA0C32E
     0,    1, 0x82, 0x83,
     0, 0x81,    2, 0x83,
     0,    1,    2, 0x83,
     0, 0x81, 0x82, 0x83,
  0x80, 0x81, 0x82, 0x83,
};

#define CHECK_locret_A0C434(i) (byte_A0C435[i] & 0x80 ? -1 : 0)

uint8 EnemyBlockCollHorizReact_Slope_Square(uint16 k, uint16 a) {
  EnemyData *v4;

  temp_collision_DD4 = 4 * a;
  temp_collision_DD6 = BTS[k] >> 6;
  uint16 v2 = 4 * a + (temp_collision_DD6 ^ ((uint8)(R26_ & 8) >> 3));
  if (!R28_) {
    EnemyData *v3 = gEnemyData(cur_enemy_index);
    if (((LOBYTE(v3->y_height) + LOBYTE(v3->y_pos) - 1) & 8) == 0)
      return CHECK_locret_A0C434(v2) < 0;
    goto LABEL_7;
  }
  if (R28_ != g_word_7E001E || (v4 = gEnemyData(cur_enemy_index), ((v4->y_pos - v4->y_height) & 8) == 0)) {
LABEL_7:
    if (CHECK_locret_A0C434(v2) < 0)
      return 1;
  }
  return CHECK_locret_A0C434(v2 ^ 2) < 0;
}

void Enemy_SetXpos_Aligned(uint16 j) {  // 0xA0C390
  EnemyData *v1 = gEnemyData(j);
  uint16 v2;
  v1->x_subpos = 0;
  if ((R20_ & 0x8000u) != 0)
    v2 = v1->x_width + (R26_ | 7) + 1;
  else
    v2 = (R26_ & 0xFFF8) - v1->x_width;
  v1->x_pos = v2;
}

uint8 EnemyBlockCollVertReact_Slope_Square(uint16 a, uint16 k) {  // 0xA0C3B2
  EnemyData *v4;

  temp_collision_DD4 = 4 * a;
  temp_collision_DD6 = BTS[k] >> 6;
  uint16 v2 = 4 * a + (temp_collision_DD6 ^ ((uint8)(R26_ & 8) >> 2));
  if (!R28_) {
    EnemyData *v3 = gEnemyData(cur_enemy_index);
    if (((LOBYTE(v3->x_width) + LOBYTE(v3->x_pos) - 1) & 8) == 0)
      return CHECK_locret_A0C434(v2) < 0;
    goto LABEL_7;
  }
  if (R28_ != g_word_7E001E || (v4 = gEnemyData(cur_enemy_index), ((v4->x_pos - v4->x_width) & 8) == 0)) {
LABEL_7:
    if (CHECK_locret_A0C434(v2) < 0)
      return 1;
  }
  return CHECK_locret_A0C434(v2 ^ 1) < 0;
}

uint8 Enemy_SetYpos_Aligned(uint16 j) {  // 0xA0C413
  EnemyData *v1 = gEnemyData(j);
  uint16 v2;
  v1->y_subpos = 0;
  if ((R20_ & 0x8000u) != 0)
    v2 = v1->y_height + (R26_ | 7) + 1;
  else
    v2 = (R26_ & 0xFFF8) - v1->y_height;
  v1->y_pos = v2;
  return 1;
}
#define g_word_A0C49F ((uint16*)RomPtr(0xa0c49f))
uint8 EnemyBlockCollHorizReact_Slope_NonSquare(void) {  // 0xA0C449
  if ((R32 & 0x8000u) == 0)
    return (R32 & 0x4000) != 0;
  uint16 v2 = 4 * (current_slope_bts & 0x1F);
  if ((R20_ & 0x8000u) == 0) {
    Multiply16x16(*(uint16 *)((char *)&R18_ + 1), g_word_A0C49F[(v2 >> 1) + 1]);
    R18_ = mult_product_lo;
    R20_ = mult_product_hi;
  } else {
    Multiply16x16(-*(uint16 *)((char *)&R18_ + 1), g_word_A0C49F[(v2 >> 1) + 1]);
    Negate32(&mult_product_hi, &mult_product_lo, &R20_, &R18_);
  }
  return 0;
}

#define kAlignYPos_Tab0 ((uint8*)RomPtr(0x948b2b))

uint8 EnemyBlockCollVertReact_Slope_NonSquare(void) {  // 0xA0C51F
  int16 v3;
  int16 v5;
  int16 v6;
  uint16 v7;
  int16 v8;
  int16 v11;
  int16 v12;
  int16 x_pos;
  uint16 v14;
  int16 v15;

  if ((R20_ & 0x8000u) != 0) {
    uint16 v9 = cur_block_index;
    uint16 mod = SnesModulus(cur_block_index, room_width_in_blocks);
    EnemyData *v10 = gEnemyData(cur_enemy_index);
    v11 = v10->x_pos >> 4;
    if (v11 == mod) {
      temp_collision_DD4 = (R24_ - v10->y_height) & 0xF ^ 0xF;
      temp_collision_DD6 = 16 * (BTS[v9] & 0x1F);
      v12 = BTS[v9] << 8;
      if (v12 < 0
          && ((v12 & 0x4000) != 0 ? (x_pos = v10->x_pos ^ 0xF) : (x_pos = v10->x_pos),
              (v14 = temp_collision_DD6 + (x_pos & 0xF),
               v15 = (kAlignYPos_Tab0[v14] & 0x1F) - temp_collision_DD4 - 1,
               (kAlignYPos_Tab0[v14] & 0x1F) - temp_collision_DD4 == 1)
              || v15 < 0)) {
        v10->y_pos = R24_ - v15;
        v10->y_subpos = 0;
        return 1;
      } else {
        return 0;
      }
    } else {
      return 0;
    }
  } else {
    uint16 v1 = cur_block_index;
    uint16 mod = SnesModulus(cur_block_index, room_width_in_blocks);
    EnemyData *v2 = gEnemyData(cur_enemy_index);
    v3 = v2->x_pos >> 4;
    if (v3 == mod) {
      temp_collision_DD4 = (LOBYTE(v2->y_height) + (uint8)R24_ - 1) & 0xF;
      temp_collision_DD6 = 16 * (BTS[v1] & 0x1F);
      v5 = BTS[v1] << 8;
      if (v5 >= 0
          && ((v5 & 0x4000) != 0 ? (v6 = v2->x_pos ^ 0xF) : (v6 = v2->x_pos),
              (v7 = temp_collision_DD6 + (v6 & 0xF),
               v8 = (kAlignYPos_Tab0[v7] & 0x1F) - temp_collision_DD4 - 1,
               (kAlignYPos_Tab0[v7] & 0x1F) - temp_collision_DD4 == 1)
              || v8 < 0)) {
        v2->y_pos = R24_ + v8;
        v2->y_subpos = -1;
        return 1;
      } else {
        return 0;
      }
    } else {
      return 0;
    }
  }
}

uint8 EnemyBlockCollReact_HorizExt(void) {  // 0xA0C619
  uint8 t = BTS[cur_block_index];
  if (t) {
    cur_block_index += (int8)t;
    return 0xff;
  }
  return 0;
}

uint8 EnemyBlockCollReact_VertExt(void) {  // 0xA0C64F
  uint16 v0;
  if (BTS[cur_block_index]) {
    if ((BTS[cur_block_index] & 0x80) != 0) {
      temp_collision_DD4 = BTS[cur_block_index] | 0xFF00;
      v0 = cur_block_index;
      do {
        v0 -= room_width_in_blocks;
        ++temp_collision_DD4;
      } while (temp_collision_DD4);
    } else {
      temp_collision_DD4 = BTS[cur_block_index];
      v0 = cur_block_index;
      do {
        v0 += room_width_in_blocks;
        --temp_collision_DD4;
      } while (temp_collision_DD4);
    }
    cur_block_index = v0;
    return 0xff;
  }
  return 0;
}

uint8 Enemy_MoveRight_SlopesAsWalls(uint16 k) {  // 0xA0C69D
  R32 = 0x4000;
  return Enemy_MoveRight_IgnoreSlopes_Inner(k);
}

uint8 Enemy_MoveRight_ProcessSlopes(uint16 k) {  // 0xA0C6A4
  R32 = 0x8000;
  return Enemy_MoveRight_IgnoreSlopes_Inner(k);
}

uint8 Enemy_MoveRight_IgnoreSlopes(uint16 k) {  // 0xA0C6AB
  R32 = 0;
  return Enemy_MoveRight_IgnoreSlopes_Inner(k);
}

uint8 Enemy_MoveRight_IgnoreSlopes_Inner(uint16 k) {  // 0xA0C6AD
  if (!__PAIR32__(R20_, R18_))
    return 0;
  EnemyData *v3 = gEnemyData(k);
  R28_ = (v3->y_pos - v3->y_height) & 0xFFF0;
  R28_ = (uint16)(v3->y_height + v3->y_pos - 1 - R28_) >> 4;
  g_word_7E001E = R28_;
  uint16 prod = Mult8x8((uint16)(v3->y_pos - v3->y_height) >> 4, room_width_in_blocks);
  uint16 v4 = (__PAIR32__(R20_, R18_) + __PAIR32__(v3->x_pos, v3->x_subpos)) >> 16;
  R22_ = R18_ + v3->x_subpos;
  R24_ = v4;
  uint16 v5;
  if ((R20_ & 0x8000u) != 0)
    v5 = v4 - v3->x_width;
  else
    v5 = v3->x_width + v4 - 1;
  R26_ = v5;
  uint16 v6 = 2 * (prod + (v5 >> 4));
  while (!(EnemyBlockCollReact_Horiz(v6) & 1)) {
    v6 += room_width_in_blocks * 2;
    if ((--R28_ & 0x8000u) != 0) {
      EnemyData *v7 = gEnemyData(k);
      bool v8 = __CFADD__uint16(v7->x_subpos, R18_);
      v7->x_subpos += R18_;
      v7->x_pos += v8 + R20_;
      return 0;
    }
  }
  if ((R20_ & 0x8000u) != 0) {
    EnemyData *v11 = gEnemyData(k);
    uint16 v12 = v11->x_width + 1 + (R26_ | 0xF);
    if (v12 == v11->x_pos || v12 < v11->x_pos)
      v11->x_pos = v12;
    v11->x_subpos = 0;
    return 1;
  } else {
    EnemyData *v9 = gEnemyData(k);
    uint16 v10 = (R26_ & 0xFFF0) - v9->x_width;
    if (v10 >= v9->x_pos)
      v9->x_pos = v10;
    v9->x_subpos = -1;
    return 1;
  }
}

uint8 Enemy_MoveDown(uint16 k) {  // 0xA0C786
  R32 = 0;
  return Enemy_MoveDownInner(k);
}

uint8 Enemy_MoveDownInner(uint16 k) {  // 0xA0C788
  int16 v6;
  uint16 v5;

  if (!__PAIR32__(R20_, R18_))
    return 0;
  EnemyData *v3 = gEnemyData(k);
  R28_ = (v3->x_pos - v3->x_width) & 0xFFF0;
  R28_ = (uint16)(v3->x_width + v3->x_pos - 1 - R28_) >> 4;
  g_word_7E001E = R28_;
  uint16 v4 = (__PAIR32__(R20_, R18_) + __PAIR32__(v3->y_pos, v3->y_subpos)) >> 16;
  R22_ = R18_ + v3->y_subpos;
  R24_ = v4;
  if ((R20_ & 0x8000u) != 0)
    v5 = v4 - v3->y_height;
  else
    v5 = v3->y_height + v4 - 1;
  R26_ = v5;
  uint16 prod = Mult8x8(v5 >> 4, room_width_in_blocks);
  v6 = (uint16)(v3->x_pos - v3->x_width) >> 4;
  for (int i = 2 * (prod + v6); !(EnemyBlockCollReact_Vert(i) & 1); i += 2) {
    if ((--R28_ & 0x8000u) != 0) {
      EnemyData *v8 = gEnemyData(k);
      v8->y_subpos = R22_;
      v8->y_pos = R24_;
      return 0;
    }
  }
  if ((R20_ & 0x8000u) != 0) {
    EnemyData *v11 = gEnemyData(k);
    uint16 v13 = v11->y_height + 1 + (R26_ | 0xF);
    if (v13 == v11->y_pos || v13 < v11->y_pos)
      v11->y_pos = v13;
    v11->y_subpos = 0;
    return 1;
  } else {
    EnemyData *v9 = gEnemyData(k);
    uint16 v10 = (R26_ & 0xFFF0) - v9->y_height;
    if (v10 >= v9->y_pos)
      v9->y_pos = v10;
    v9->y_subpos = -1;
    return 1;
  }
}

static Func_U8 *const off_A0C859[16] = {  // 0xA0C845
  ClearCarry_13,
  EnemyBlockCollHorizReact_Slope,
  ClearCarry_13,
  ClearCarry_13,
  ClearCarry_13,
  EnemyBlockCollReact_HorizExt,
  ClearCarry_13,
  ClearCarry_13,
  SetCarry_4,
  SetCarry_4,
  EnemyBlockCollReact_Spike,
  SetCarry_4,
  SetCarry_4,
  EnemyBlockCollReact_VertExt,
  SetCarry_4,
  SetCarry_4,
};

uint8 EnemyBlockCollReact_Horiz(uint16 k) {
  cur_block_index = k >> 1;
  uint8 rv = 0;
  do {
    rv = off_A0C859[(level_data[cur_block_index] & 0xf000) >> 12]();
  } while (rv & 0x80);
  return rv;
}

static Func_U8 *const off_A0C88D[16] = {  // 0xA0C879
  ClearCarry_13,
  EnemyBlockCollVertReact_Slope,
  ClearCarry_13,
  ClearCarry_13,
  ClearCarry_13,
  EnemyBlockCollReact_HorizExt,
  ClearCarry_13,
  ClearCarry_13,
  SetCarry_4,
  SetCarry_4,
  EnemyBlockCollReact_Spike,
  SetCarry_4,
  SetCarry_4,
  EnemyBlockCollReact_VertExt,
  SetCarry_4,
  SetCarry_4,
};

uint8 EnemyBlockCollReact_Vert(uint16 k) {
  cur_block_index = k >> 1;
  uint8 rv = 0;
  do {
    rv = off_A0C88D[(level_data[cur_block_index] & 0xf000) >> 12]();
  } while (rv & 0x80);
  return rv;
}

void CalculateBlockContainingPixelPos(uint16 xpos, uint16 ypos) {
  uint16 prod = Mult8x8(ypos >> 4, room_width_in_blocks);
  cur_block_index = prod + (xpos >> 4);
}

uint8 EnemyFunc_C8AD(uint16 k) {  // 0xA0C8AD
  int16 v2;
  int16 v3;
  int16 v4;
  int16 v5;
  int16 x_pos;
  int16 v7;

  uint8 result = 0;

  EnemyData *v1 = gEnemyData(k);
  CalculateBlockContainingPixelPos(v1->x_pos, v1->y_pos + v1->y_height - 1);
  if ((level_data[cur_block_index] & 0xF000) == 4096
      && (BTS[cur_block_index] & 0x1Fu) >= 5) {
    result = 1;
    temp_collision_DD4 = (LOBYTE(v1->y_height) + LOBYTE(v1->y_pos) - 1) & 0xF;
    temp_collision_DD6 = 16 * (BTS[cur_block_index] & 0x1F);
    v2 = BTS[cur_block_index] << 8;
    if (v2 >= 0) {
      v3 = (v2 & 0x4000) != 0 ? v1->x_pos ^ 0xF : v1->x_pos;
      v4 = (kAlignYPos_Tab0[(uint16)(temp_collision_DD6 + (v3 & 0xF))] & 0x1F) - temp_collision_DD4 - 1;
      if (v4 < 0)
        v1->y_pos += v4;
    }
  }
  CalculateBlockContainingPixelPos(v1->x_pos, v1->y_pos - v1->y_height);
  if ((level_data[cur_block_index] & 0xF000) == 4096
      && (BTS[cur_block_index] & 0x1Fu) >= 5) {
    result = 1;
    temp_collision_DD4 = (v1->y_pos - v1->y_height) & 0xF ^ 0xF;
    temp_collision_DD6 = 16 * (BTS[cur_block_index] & 0x1F);
    v5 = BTS[cur_block_index] << 8;
    if (v5 < 0) {
      if ((v5 & 0x4000) != 0)
        x_pos = v1->x_pos ^ 0xF;
      else
        x_pos = v1->x_pos;
      v7 = (kAlignYPos_Tab0[(uint16)(temp_collision_DD6 + (x_pos & 0xF))] & 0x1F) - temp_collision_DD4 - 1;
      if (v7 <= 0)
        v1->y_pos -= v7;
    }
  }
  return result;
}