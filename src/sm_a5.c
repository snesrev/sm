//  Enemy AI - Draygon & Spore Spawn

#include "ida_types.h"
#include "variables.h"
#include "funcs.h"
#include "enemy_types.h"
#define kDraygon_MorePalettes ((uint16*)RomPtr(0xa5a217))
#define kDraygon_MorePalettes2 ((uint16*)RomPtr(0xa5a277))
#define g_word_A5A297 ((uint16*)RomPtr(0xa5a297))
#define g_word_A587DC ((uint16*)RomPtr(0xa587dc))
#define g_word_A596AF ((uint16*)RomPtr(0xa596af))
#define g_word_A596EF ((uint16*)RomPtr(0xa596ef))
#define g_word_A5A19F ((uint16*)RomPtr(0xa5a19f))
#define g_word_A5A1AF ((uint16*)RomPtr(0xa5a1af))
#define g_word_A5A1C7 ((uint16*)RomPtr(0xa5a1c7))
#define g_word_A5A1DF ((uint16*)RomPtr(0xa5a1df))
#define g_byte_A5CE07 ((uint8*)RomPtr(0xa5ce07))
#define g_word_A5E379 ((uint16*)RomPtr(0xa5e379))
#define g_word_A5E3F9 ((uint16*)RomPtr(0xa5e3f9))
#define g_word_A5E4F9 ((uint16*)RomPtr(0xa5e4f9))
#define g_word_A5E5D9 ((uint16*)RomPtr(0xa5e5d9))

#define kSporeSpawn_Palette ((uint16*)RomPtr(0xa5e359))

uint16 EnemyInstr_Goto_A5(uint16 k, uint16 j) {  // 0xA580ED
  return *(uint16 *)RomPtr_A5(j);
}

uint16 EnemyInstr_DecTimerAndGoto2_A5(uint16 k, uint16 j) {  // 0xA58110
  EnemyData *v2 = gEnemyData(k);
  if (v2->timer-- == 1)
    return j + 2;
  else
    return EnemyInstr_Goto_A5(k, j);
}

uint16 EnemyInstr_SetTimer_A5(uint16 k, uint16 j) {  // 0xA58123
  uint16 v2 = *(uint16 *)RomPtr_A5(j);
  gEnemyData(k)->timer = v2;
  return j + 2;
}
uint16 EnemyInstr_Sleep_A5(uint16 k, uint16 j) {  // 0xA5812F
  gEnemyData(k)->current_instruction = j - 2;
  return 0;
}
uint16 EnemyInstr_StopScript_A5(uint16 k, uint16 j) {  // 0xA5807C
  EnemyData *v2 = gEnemyData(k);
  v2->properties |= kEnemyProps_Deleted;
  return 0;
}

uint16 EnemyInstr_WaitNframes_A5(uint16 k, uint16 j) {  // 0xA5813A
  uint16 v2 = *(uint16 *)RomPtr_A5(j);
  EnemyData *v3 = gEnemyData(k);
  v3->instruction_timer = v2;
  v3->current_instruction = j + 2;
  return 0;
}

void Enemy_GrappleReact_NoInteract_A5(void) {  // 0xA58000
  SwitchEnemyAiToMainAi();
}

void Enemy_GrappleReact_CancelBeam_A5(void) {  // 0xA5800F
  Enemy_SwitchToFrozenAi();
}

void Enemy_NormalTouchAI_A5(void) {  // 0xA58023
  NormalEnemyTouchAi();
}

void Enemy_NormalShotAI_A5(void) {  // 0xA5802D
  NormalEnemyShotAi();
}

void Enemy_NormalFrozenAI_A5(void) {  // 0xA58041
  NormalEnemyFrozenAI();
}

void Draygon_Init(void) {  // 0xA58687
  uint16 j;

  for (int i = 48; i >= 0; i -= 2)
    target_palettes[(i >> 1) + 144] = kDraygon_MorePalettes[i >> 1];
  for (j = 4094; (j & 0x8000u) == 0; j -= 2)
    tilemap_stuff[j >> 1] = 824;
  Get_Draygon(cur_enemy_index)->base.palette_index = 3584;
  *(uint16 *)&enemy_gfx_drawn_hook.bank = 165;
  enemy_gfx_drawn_hook.addr = FUNC16(Draygon_Func_36);
  flag_process_all_enemies = -1;
  Get_Draygon(0)->base.current_instruction = addr_kDraygon_Ilist_9889;
  Get_Draygon(0x40u)->base.current_instruction = addr_kDraygon_Ilist_9944;
  Get_Draygon(0x80)->base.current_instruction = addr_kDraygon_Ilist_99FC;
  Get_Draygon(0xC0u)->base.current_instruction = addr_kDraygon_Ilist_9813;
  enemy_bg2_tilemap_size = 1024;
  Get_Draygon(cur_enemy_index)->draygon_var_A = FUNC16(Draygon_Func_1);
  sub_88DF34();
  room_loading_irq_handler = 12;
  SpawnBG3ScrollHdmaObject();
  Get_Draygon(0)->draygon_var_45 = 1;
  DisableMinimapAndMarkBossRoomAsExplored();
}

void CallDraygonFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnnullsub_169_A5: return;
  case fnDraygon_Func_1: Draygon_Func_1(); return;
  case fnDraygon_Func_2: Draygon_Func_2(); return;
  case fnDraygon_Func_4: Draygon_Func_4(); return;
  case fnDraygon_Func_6: Draygon_Func_6(); return;
  case fnDraygon_Func_8: Draygon_Func_8(); return;
  case fnDraygon_Func_9: Draygon_Func_9(); return;
  case fnDraygon_Func_10: Draygon_Func_10(); return;
  case fnDraygon_Func_11: Draygon_Func_11(); return;
  case fnDraygon_Func_12: Draygon_Func_12(); return;
  case fnDraygon_Func_13: Draygon_Func_13(); return;
  case fnDraygon_Func_14: Draygon_Func_14(); return;
  case fnDraygon_Func_15: Draygon_Func_15(); return;
  case fnDraygon_Func_16: Draygon_Func_16(); return;
  case fnDraygon_Func_17: Draygon_Func_17(k); return;
  case fnDraygon_Func_18: Draygon_Func_18(); return;
  case fnDraygon_Func_19: Draygon_Func_19(); return;
  case fnDraygon_Func_20: Draygon_Func_20(); return;
  case fnDraygon_Func_21: Draygon_Func_21(k); return;
  case fnDraygon_Func_22: Draygon_Func_22(); return;
  case fnDraygon_Func_23: Draygon_Func_23(); return;
  case fnDraygon_Func_24: Draygon_Func_24(k); return;
  case fnDraygon_Func_25: Draygon_Func_25(k); return;
  case fnDraygon_Func_26: Draygon_Func_26(k); return;
  case fnDraygon_Func_27: Draygon_Func_27(k); return;
  case fnDraygon_Func_28: Draygon_Func_28(k); return;
  case fnDraygon_Func_29: Draygon_Func_29(); return;
  case fnDraygon_Func_30: Draygon_Func_30(); return;
  case fnDraygon_Func_31: Draygon_Func_31(); return;
  case fnDraygon_Func_32: Draygon_Func_32(); return;
  case fnDraygon_Func_33: Draygon_Func_33(); return;
  case fnDraygon_Func_48: Draygon_Func_48(k); return;
  case fnDraygon_Func_49: Draygon_Func_49(k); return;
  case fnnullsub_223: return;
  case fnSporeSpawn_Func_1: SporeSpawn_Func_1(); return;
  case fnSporeSpawn_Func_2: SporeSpawn_Func_2(k); return;
  case fnSporeSpawn_Func_3: SporeSpawn_Func_3(); return;
  case fnSporeSpawn_Func_4: SporeSpawn_Func_4(); return;
  default: Unreachable();
  }
}

void Draygon_Main(void) {  // 0xA586FC
  Enemy_Draygon *EK = Get_Draygon(cur_enemy_index);
  CallDraygonFunc(EK->draygon_var_A | 0xA50000, cur_enemy_index);

  Enemy_Draygon *E0 = Get_Draygon(0);
  uint16 x_pos = E0->base.x_pos;
  Enemy_Draygon *E1 = Get_Draygon(0x40u);
  E1->base.x_pos = x_pos;
  Enemy_Draygon *E2 = Get_Draygon(0x80);
  E2->base.x_pos = x_pos;
  Enemy_Draygon *E3 = Get_Draygon(0xC0u);
  E3->base.x_pos = x_pos;
  uint16 y_pos = E0->base.y_pos;
  E1->base.y_pos = y_pos;
  E2->base.y_pos = y_pos;
  E3->base.y_pos = y_pos;
}

void Draygon_Func_1(void) {  // 0xA5871B
  VramWriteEntry *v2;

  Draygon_Func_3();
  Enemy_Draygon *E = Get_Draygon(0);
  if (sign16(E->draygon_var_B - 256)) {
    if (!E->draygon_var_B) {
      v2 = gVramWriteEntry(vram_write_queue_tail);
      v2->size = 1536;
      v2->src.addr = -27648;
      *(uint16 *)&v2->src.bank = 177;
      v2->vram_dst = 27904;
      vram_write_queue_tail += 7;
      Draygon_Func_45();
    }
    ++E->draygon_var_B;
  } else {
    E->draygon_var_A = FUNC16(Draygon_Func_2);
    E->draygon_var_B = 0;
    uint16 x_pos = E->base.x_pos;
    E->draygon_var_00 = x_pos;
    E->draygon_var_02 = x_pos + 672;
    E->draygon_var_01 = E->base.y_pos;
    E->base.x_pos = E->draygon_var_00;
    E->base.y_pos = E->draygon_var_01;
    E->draygon_var_0F = 24;
  }
}

void Draygon_Func_2(void) {  // 0xA5878B
  Draygon_Func_3();
  Enemy_Draygon *E = Get_Draygon(0);
  if (sign16(E->draygon_var_B - 1232)) {
    Draygon_Func_46();
    ++E->draygon_var_B;
  } else {
    E->draygon_var_A = FUNC16(Draygon_Func_4);
    E->draygon_var_B = 0;
  }
}

void Draygon_Func_3(void) {  // 0xA587AA
  char Random;

  if ((nmi_frame_counter_word & 0x3F) == 0) {
    Random = NextRandom();
    uint16 result = 2 * ((Random & 3) + 2);
    if (!Get_Draygon(result)->draygon_var_40) {
      int v2 = (uint16)(4 * ((Random & 3) + 2)) >> 1;
      R18_ = g_word_A587DC[v2];
      R20_ = g_word_A587DC[v2 + 1];
      SpawnEnemyProjectileWithGfx(3u, result, addr_stru_868E5E);
    }
  }
}

void Draygon_Func_4(void) {  // 0xA587F4
  Draygon_Func_3();
  Draygon_Func_5();
  Enemy_Draygon *E = Get_Draygon(0);
  E->draygon_var_A = FUNC16(Draygon_Func_6);
  E->base.current_instruction = addr_kDraygon_Ilist_97D1;
  E->base.instruction_timer = 1;
  E->draygon_var_20 = 1;
}

void Draygon_Func_5(void) {  // 0xA58817
  Enemy_Draygon *E = Get_Draygon(0);
  int16 draygon_var_00;

  varE28 = 384;
  varE24 = 384;
  draw_oam_x_offset = 0;
  varE26 = 0;
  while (1) {

    draw_oam_x_offset += E->draygon_var_0F;
    uint16 v1 = varE24 - HIBYTE(draw_oam_x_offset);
    if ((int16)(v1 - E->draygon_var_01) < 0)
      break;
    varE24 -= HIBYTE(draw_oam_x_offset);
    *(uint16 *)((char *)&g_word_7E9002 + varE26++) = v1;
    ++varE26;
    ++varE26;
    if (!sign16(++varE26 - 2048)) {
      while (1)
        ;
    }
  }
  draygon_var_00 = E->draygon_var_00;
  uint16 v4;
  if (draygon_var_00 < 0) {
    uint16 v5 = Abs16(draygon_var_00);
    v4 = Abs16(samus_x_pos + v5);
  } else {
    v4 = Abs16(draygon_var_00 - samus_x_pos);
  }
  R44 = v4;
  R42 = 0;
  R46 = varE26 >> 2;
  R48 = 0;
  EnemyFunc_B761();
  E->draygon_var_D = R44;
  E->draygon_var_E = R42;
  uint16 v6 = varE26;
  *(uint16 *)((char *)&g_word_7E9002 + varE26) = E->base.y_pos;
  E->draygon_var_B = v6;
  E->draygon_var_C = v6;
}

void Draygon_Func_6(void) {  // 0xA588B1
  Draygon_Func_3();
  Draygon_Func_35();
  Enemy_Draygon *E = Get_Draygon(0);
  uint16 draygon_var_B = E->draygon_var_B;
  if (draygon_var_B == 104) {
    Enemy_Draygon *E3 = Get_Draygon(0xC0u);
    E3->base.current_instruction = addr_kDraygon_Ilist_9C06;
    E3->base.instruction_timer = 1;
  }
  Get_Draygon(cur_enemy_index)->base.y_pos = *(uint16 *)((char *)&g_word_7E9002 + draygon_var_B);
  E->draygon_var_B -= 4;
  if (E->draygon_var_B) {
    uint16 x_subpos = E->base.x_subpos;
    bool v6 = __CFADD__uint16(E->draygon_var_E, x_subpos);
    E->base.x_subpos = E->draygon_var_E + x_subpos;
    E->base.x_pos += E->draygon_var_D + v6;
  } else {
    E->draygon_var_A = FUNC16(Draygon_Func_8);
  }
}

void Draygon_Func_7(void) {  // 0xA58901
  Enemy_Draygon *E = Get_Draygon(0);
  if ((nmi_frame_counter_word & 0xF) == 0) {
    E->base.current_instruction = addr_kDraygon_Ilist_98FE;
    if (E->draygon_var_20)
      E->base.current_instruction = addr_kDraygon_Ilist_9C90;
    E->base.instruction_timer = 1;
  }
}

void Draygon_Func_8(void) {  // 0xA58922
  Draygon_Func_3();
  Enemy_Draygon *E = Get_Draygon(0);
  R44 = 672 - E->base.x_pos;
  R42 = 0;
  R46 = E->draygon_var_C >> 2;
  R48 = 0;
  EnemyFunc_B761();
  E->draygon_var_D = R44;
  E->draygon_var_E = R42;
  E->draygon_var_A = FUNC16(Draygon_Func_9);
}

void Draygon_Func_9(void) {  // 0xA58951
  Draygon_Func_3();
  Enemy_Draygon *E = Get_Draygon(0);
  uint16 draygon_var_B = E->draygon_var_B;
  if (draygon_var_B == 104) {
    Enemy_Draygon *E3 = Get_Draygon(0xC0u);
    E3->base.current_instruction = addr_kDraygon_Ilist_9BDA;
    E3->base.instruction_timer = 1;
  }
  Get_Draygon(cur_enemy_index)->base.y_pos = *(uint16 *)((char *)&g_word_7E9002 + draygon_var_B);
  E->draygon_var_B += 4;
  if (E->draygon_var_B == E->draygon_var_C) {
    E->draygon_var_B = 0;
    if ((random_number & 1) != 0)
      E->draygon_var_A = FUNC16(Draygon_Func_10);
    else
      E->draygon_var_A = FUNC16(Draygon_Func_18);
  } else {
    uint16 x_subpos = E->base.x_subpos;
    bool v5 = __CFADD__uint16(E->draygon_var_E, x_subpos);
    E->base.x_subpos = E->draygon_var_E + x_subpos;
    E->base.x_pos += E->draygon_var_D + v5;
  }
}

void Draygon_Func_10(void) {  // 0xA589B3
  Draygon_Func_3();
  Enemy_Draygon *E = Get_Draygon(0);
  R44 = Abs16(E->draygon_var_02 - samus_x_pos);
  R42 = 0;
  R46 = varE26 >> 2;
  R48 = 0;
  EnemyFunc_B761();
  E->draygon_var_D = R44;
  E->draygon_var_E = R42;
  E->draygon_var_A = FUNC16(Draygon_Func_11);
  E->base.current_instruction = addr_kDraygon_Ilist_97BB;
  E->base.instruction_timer = 1;
  E->draygon_var_20 = 0;
  E->draygon_var_B = E->draygon_var_C;
}

void Draygon_Func_11(void) {  // 0xA58A00
  Draygon_Func_3();
  Draygon_Func_35();
  Enemy_Draygon *E = Get_Draygon(0);
  uint16 draygon_var_B = E->draygon_var_B;
  if (draygon_var_B == 104) {
    Enemy_Draygon *E3 = Get_Draygon(0xC0u);
    E3->base.current_instruction = addr_kDraygon_Ilist_9813;
    E3->base.instruction_timer = 1;
  }
  Get_Draygon(cur_enemy_index)->base.y_pos = *(uint16 *)((char *)&g_word_7E9002 + draygon_var_B);
  E->draygon_var_B -= 4;
  if (E->draygon_var_B) {
    uint16 x_subpos = E->base.x_subpos;
    bool v6 = x_subpos < E->draygon_var_E;
    E->base.x_subpos = x_subpos - E->draygon_var_E;
    E->base.x_pos -= v6 + E->draygon_var_D;
  } else {
    E->draygon_var_A = FUNC16(Draygon_Func_12);
  }
}

void Draygon_Func_12(void) {  // 0xA58A50
  int16 draygon_var_00;

  Draygon_Func_3();
  Enemy_Draygon *E = Get_Draygon(0);
  draygon_var_00 = E->draygon_var_00;
  uint16 v2;
  if (draygon_var_00 < 0)
    v2 = E->base.x_pos - draygon_var_00;
  else
    v2 = E->base.x_pos - E->draygon_var_00;
  R44 = v2;
  R42 = 0;
  R46 = E->draygon_var_C >> 2;
  R48 = 0;
  EnemyFunc_B761();
  E->draygon_var_D = R44;
  E->draygon_var_E = R42;
  E->draygon_var_A = FUNC16(Draygon_Func_13);
}

void Draygon_Func_13(void) {  // 0xA58A90
  Draygon_Func_3();
  Draygon_Func_35();
  Enemy_Draygon *E = Get_Draygon(0);
  uint16 draygon_var_B = E->draygon_var_B;
  if (draygon_var_B == 104) {
    Enemy_Draygon *E3 = Get_Draygon(0xC0u);
    E3->base.current_instruction = addr_kDraygon_Ilist_97E7;
    E3->base.instruction_timer = 1;
  }
  Get_Draygon(cur_enemy_index)->base.y_pos = *(uint16 *)((char *)&g_word_7E9002 + draygon_var_B);
  ++E->draygon_var_B;
  ++E->draygon_var_B;
  ++E->draygon_var_B;
  if (++E->draygon_var_B == E->draygon_var_C) {
    if ((random_number & 1) != 0) {
      E->draygon_var_A = FUNC16(Draygon_Func_4);
      E->draygon_var_B = 0;
      E->base.x_pos = E->draygon_var_00;
      E->base.y_pos = E->draygon_var_01;
    } else {
      E->draygon_var_A = FUNC16(Draygon_Func_14);
      E->base.x_pos = E->draygon_var_00;
    }
  } else {
    uint16 x_subpos = E->base.x_subpos;
    bool v6 = x_subpos < E->draygon_var_E;
    E->base.x_subpos = x_subpos - E->draygon_var_E;
    E->base.x_pos -= v6 + E->draygon_var_D;
  }
}

void Draygon_Func_14(void) {  // 0xA58B0A
  Draygon_Func_3();
  Enemy_Draygon *E = Get_Draygon(0);
  E->base.x_pos = -80;
  E->base.y_pos = 384;
  E->draygon_var_E = 0;
  E->draygon_var_D = 1;
  E->draygon_var_0D = 0;
  Enemy_Draygon *E3 = Get_Draygon(0xC0u);
  E3->base.current_instruction = addr_kDraygon_Ilist_9C06;
  E3->base.instruction_timer = 1;
  E->draygon_var_A = FUNC16(Draygon_Func_15);
  E->base.current_instruction = addr_kDraygon_Ilist_97D1;
  E->base.instruction_timer = 1;
  E->draygon_var_20 = 1;
}

void Draygon_Func_15(void) {  // 0xA58B52
  Draygon_Func_3();
  Draygon_Func_35();
  Enemy_Draygon *E = Get_Draygon(0);
  uint16 v2 = Abs16(E->base.x_pos - samus_x_pos);
  if (sign16(v2 - 208)) {
    E->draygon_var_A = FUNC16(Draygon_Func_16);
    E->draygon_var_03 = 16;
  } else {
    draw_enemy_layer = 32;
    E->base.y_pos = CosineMult8bit(E->draygon_var_0D) + 384;
    E->draygon_var_0D = (uint8)(E->draygon_var_0D + 1);
    uint16 x_subpos = E->base.x_subpos;
    bool v4 = __CFADD__uint16(E->draygon_var_E, x_subpos);
    E->base.x_subpos = E->draygon_var_E + x_subpos;
    E->base.x_pos += E->draygon_var_D + v4;
  }
}

void Draygon_Func_16(void) {  // 0xA58BAE
  Enemy_Draygon *E = Get_Draygon(0);
  int16 v5;

  Draygon_Func_3();
  if (samus_x_speed_divisor)
    goto LABEL_9;
  if ((random_number & 0xF) == 0) {
    uint16 v1 = E->draygon_var_03 - 1;
    E->draygon_var_03 = v1;
    if (!v1)
      goto LABEL_8;
    E->base.current_instruction = addr_kDraygon_Ilist_9C90;
    E->base.instruction_timer = 1;
  }
  draw_enemy_layer = 32;

  E->base.y_pos = CosineMult8bit(E->draygon_var_0D) + 384;
  E->draygon_var_0D = (uint8)(E->draygon_var_0D + 1);
  uint16 x_subpos;
  x_subpos = E->base.x_subpos;
  bool v4;
  v4 = __CFADD__uint16(E->draygon_var_E, x_subpos);
  E->base.x_subpos = E->draygon_var_E + x_subpos;
  v5 = E->draygon_var_D + v4 + E->base.x_pos;
  E->base.x_pos = v5;
  if (v5 >= 0 && !sign16(v5 - 672)) {
LABEL_8:
    if (!samus_x_speed_divisor) {
      Enemy_Draygon *E3 = Get_Draygon(0xC0u);
      E3->base.current_instruction = addr_kDraygon_Ilist_9BDA;
      E3->base.instruction_timer = 1;
      E->draygon_var_A = FUNC16(Draygon_Func_17);
      return;
    }
LABEL_9:
    E->draygon_var_A = FUNC16(Draygon_Func_22);
  }
}

void Draygon_Func_17(uint16 k) {  // 0xA58C33
  int16 v4;

  Draygon_Func_35();
  draw_enemy_layer = 32;
  Enemy_Draygon *E = Get_Draygon(0);
  E->base.y_pos = CosineMult8bit(E->draygon_var_0D) + 384;
  E->draygon_var_0D = (uint8)(E->draygon_var_0D + 1);
  uint16 x_subpos = E->base.x_subpos;
  bool v3 = __CFADD__uint16(E->draygon_var_E, x_subpos);
  E->base.x_subpos = E->draygon_var_E + x_subpos;
  v4 = E->draygon_var_D + v3 + E->base.x_pos;
  E->base.x_pos = v4;
  if (v4 >= 0 && !sign16(v4 - 672)) {
    E->draygon_var_A = FUNC16(Draygon_Func_10);
    E->base.x_pos = 592;
    E->base.y_pos = -80;
    E->draygon_var_B = E->draygon_var_C;
  }
}

void Draygon_Func_18(void) {  // 0xA58C8E
  Enemy_Draygon *E = Get_Draygon(0);
  E->base.x_pos = E->draygon_var_02;
  E->base.y_pos = 384;
  E->draygon_var_E = 0;
  E->draygon_var_D = 1;
  E->draygon_var_0D = 0;
  Enemy_Draygon *E3 = Get_Draygon(0xC0u);
  E3->base.current_instruction = addr_kDraygon_Ilist_9C06;
  E3->base.instruction_timer = 1;
  E->draygon_var_A = FUNC16(Draygon_Func_19);
  E->base.current_instruction = addr_kDraygon_Ilist_97BB;
  E->base.instruction_timer = 1;
  E->draygon_var_20 = 0;
}

void Draygon_Func_19(void) {  // 0xA58CD4
  Draygon_Func_3();
  Draygon_Func_35();
  Enemy_Draygon *E = Get_Draygon(0);
  uint16 v2 = Abs16(E->base.x_pos - samus_x_pos);
  if (sign16(v2 - 208)) {
    E->draygon_var_A = FUNC16(Draygon_Func_20);
    E->draygon_var_03 = 16;
  } else {
    draw_enemy_layer = 32;
    E->base.y_pos = CosineMult8bit(E->draygon_var_0D) + 384;
    E->draygon_var_0D = (uint8)(E->draygon_var_0D + 1);
    uint16 x_subpos = E->base.x_subpos;
    bool v4 = x_subpos < E->draygon_var_E;
    E->base.x_subpos = x_subpos - E->draygon_var_E;
    E->base.x_pos -= v4 + E->draygon_var_D;
  }
}

void Draygon_Func_20(void) {  // 0xA58D30
  Enemy_Draygon *E = Get_Draygon(0);
  int16 v5;

  if (samus_x_speed_divisor)
    goto LABEL_9;
  if ((random_number & 0xF) == 0) {
    uint16 v1 = E->draygon_var_03 - 1;
    E->draygon_var_03 = v1;
    if (!v1)
      goto LABEL_8;
    E->base.current_instruction = addr_kDraygon_Ilist_98FE;
    E->base.instruction_timer = 1;
  }
  draw_enemy_layer = 32;

  E->base.y_pos = CosineMult8bit(E->draygon_var_0D) + 384;
  E->draygon_var_0D = (uint8)(E->draygon_var_0D + 1);
  uint16 x_subpos;
  x_subpos = E->base.x_subpos;
  bool v4;
  v4 = x_subpos < E->draygon_var_E;
  E->base.x_subpos = x_subpos - E->draygon_var_E;
  v5 = E->base.x_pos - (v4 + E->draygon_var_D);
  E->base.x_pos = v5;
  if (v5 < 0 && sign16(v5 + 80)) {
LABEL_8:
    if (!samus_x_speed_divisor) {
      Enemy_Draygon *E3 = Get_Draygon(0xC0u);
      E3->base.current_instruction = addr_kDraygon_Ilist_97E7;
      E3->base.instruction_timer = 1;
      Get_Draygon(0)->draygon_var_A = FUNC16(Draygon_Func_21);
      return;
    }
LABEL_9:
    Get_Draygon(0)->draygon_var_A = FUNC16(Draygon_Func_22);
  }
}

void Draygon_Func_21(uint16 k) {  // 0xA58DB2
  Enemy_Draygon *E = Get_Draygon(0);
  int16 v4;

  Draygon_Func_35();
  if (samus_x_speed_divisor) {
    E->draygon_var_A = FUNC16(Draygon_Func_22);
  } else {
    draw_enemy_layer = 32;
    E->base.y_pos = CosineMult8bit(E->draygon_var_0D) + 384;
    E->draygon_var_0D = (uint8)(E->draygon_var_0D + 1);
    uint16 x_subpos = E->base.x_subpos;
    bool v3 = x_subpos < E->draygon_var_E;
    E->base.x_subpos = x_subpos - E->draygon_var_E;
    v4 = E->base.x_pos - (v3 + E->draygon_var_D);
    E->base.x_pos = v4;
    if (v4 < 0) {
      if (sign16(v4 + 80)) {
        E->draygon_var_A = FUNC16(Draygon_Func_4);
        E->base.x_pos = -80;
        E->base.y_pos = -80;
        E->draygon_var_B = E->draygon_var_C;
      }
    }
  }
}

void Draygon_Func_22(void) {  // 0xA58E19
  Enemy_Draygon *E = Get_Draygon(0);
  int16 v2;
  uint16 v4;

  Draygon_Func_3();
  if (samus_x_speed_divisor) {
    E->base.properties |= kEnemyProps_Tangible;
    v2 = 8;
    if (!E->draygon_var_20)
      v2 = -8;
    uint16 v3 = Abs16(E->base.x_pos + v2 - samus_x_pos);
    if (sign16(v3 - 8) && (v4 = Abs16(E->base.y_pos - samus_y_pos), sign16(v4 - 8))) {
      uint16 v5 = addr_kDraygon_Ilist_9845;
      if (E->draygon_var_20)
        v5 = addr_kDraygon_Ilist_9C38;
      Enemy_Draygon *E3 = Get_Draygon(0xC0u);
      E3->base.current_instruction = v5;
      E3->base.instruction_timer = 1;
      if (CallSomeSamusCode(0xDu)) {
        E->draygon_var_A = FUNC16(Draygon_Func_23);
      } else {
        Samus_SetGrabbedByDraygonPose(E->draygon_var_20 != 0);
        E->draygon_var_06 = 256;
        E->draygon_var_07 = 384;
        E->draygon_var_05 = 0;
        E->draygon_var_09 = 0;
        E->draygon_var_08 = 192;
        E->draygon_var_0B = 2048;
        E->draygon_var_A = FUNC16(Draygon_Func_24);
      }
    } else {
      R18_ = samus_x_pos - E->base.x_pos;
      R20_ = samus_y_pos - E->base.y_pos;
      enemy_population_ptr = (uint8)(64 - CalculateAngleFromXY());
      R18_ = enemy_population_ptr;
      R20_ = 2;
      ConvertAngleToXy();
      varE24 = R22_;
      varE26 = R24_;
      varE28 = R26_;
      varE2A = R28_;
      EnemyFunc_B691();
    }
  } else {
    E->draygon_var_A = FUNC16(Draygon_Func_30);
  }
}

void Draygon_Func_23(void) {  // 0xA58F10
  grapple_beam_function = FUNC16(GrappleBeam_Func2);
  Get_Draygon(0)->draygon_var_A = FUNC16(Draygon_Func_29);
}

void Draygon_Func_24(uint16 k) {  // 0xA58F1E
  Enemy_Draygon *E = Get_Draygon(0);
  uint16 v5;

  if ((samus_grapple_flags & 1) != 0) {
    E->base.flash_timer = get_EnemyDef_A2(E->base.enemy_ptr)->hurt_ai_time + 8;
    E->base.ai_handler_bits |= 2u;
  } else {
    uint16 v4 = Abs16(E->base.x_pos - 256);
    if (sign16(v4 - 2) && (v5 = Abs16(E->base.y_pos - 384), sign16(v5 - 2))) {
      E->draygon_var_A = FUNC16(Draygon_Func_25);
      uint16 v6 = addr_kDraygon_Ilist_9922;
      if (E->draygon_var_20)
        v6 = addr_kDraygon_Ilist_9CB4;
      Enemy_Draygon *EK = Get_Draygon(k);
      EK->base.current_instruction = v6;
      EK->base.instruction_timer = 1;
      E->base.properties |= kEnemyProps_Tangible;
    } else {
      R18_ = 256 - E->base.x_pos;
      R20_ = 384 - E->base.y_pos;
      enemy_population_ptr = (uint8)(64 - CalculateAngleFromXY());
      R18_ = enemy_population_ptr;
      R20_ = 2;
      ConvertAngleToXy();
      varE24 = R22_;
      varE26 = R24_;
      varE28 = R26_;
      varE2A = R28_;
      EnemyFunc_B691();
      Draygon_Func_40(k);
    }
  }
}

void Draygon_Func_25(uint16 k) {  // 0xA58FD6
  Enemy_Draygon *E = Get_Draygon(0);
  uint16 v6;

  if ((samus_grapple_flags & 1) != 0) {
    E->base.flash_timer = get_EnemyDef_A2(E->base.enemy_ptr)->hurt_ai_time + 8;
    E->base.ai_handler_bits |= 2u;
  } else if ((uint8)random_number) {
    draw_enemy_layer = E->draygon_var_05;
    E->base.x_pos = E->draygon_var_06 + CosineMult8bit(E->draygon_var_08);
    draw_enemy_layer = E->draygon_var_05 >> 2;
    E->base.y_pos = E->draygon_var_07 + SineMult8bitNegative(E->draygon_var_08);
    if ((Get_Draygon(k)->base.frame_counter & 7) == 0) {
      R18_ = E->base.x_pos - 32;
      if (E->draygon_var_20)
        R18_ = E->base.x_pos + 32;
      R20_ = E->base.y_pos - 16;
      R22_ = 61;
      R24_ = 0;
      CreateSpriteAtPos();
    }
    uint16 draygon_var_09 = E->draygon_var_09;
    E->draygon_var_09 = draygon_var_09 + 0x2000;
    uint16 v5 = __CFADD__uint16(draygon_var_09, 0x2000) + E->draygon_var_05;
    E->draygon_var_05 = v5;
    uint16 draygon_var_0A, v8;
    if (!sign16(v5 - 160)
        || (v6 = E->draygon_var_0B - 1,
            E->draygon_var_0B = v6,
            E->draygon_var_08 = (uint8)(LOBYTE(E->draygon_var_08) + HIBYTE(v6)),
            draygon_var_0A = E->draygon_var_0A,
            E->draygon_var_0A = draygon_var_0A - 0x4000,
            v8 = (__PAIR32__(E->draygon_var_07, draygon_var_0A) - 0x4000) >> 16,
            E->draygon_var_07 = v8,
            sign16(v8 - 64))) {
      E->draygon_var_A = FUNC16(Draygon_Func_27);
    } else {
      Draygon_Func_40(k);
    }
  } else {
    E->draygon_var_0C = 64;
    E->draygon_var_A = FUNC16(Draygon_Func_26);
  }
}

void Draygon_Func_26(uint16 k) {  // 0xA590D4
  uint16 v0 = k;
  uint16 v3;

  Draygon_Func_40(v0);
  Enemy_Draygon *E = Get_Draygon(0);
  uint16 v2 = E->draygon_var_0C - 1;
  E->draygon_var_0C = v2;
  if (v2) {
    if (v2 == 63) {
      v3 = addr_kDraygon_Ilist_9AE8;
      if (E->draygon_var_20)
        v3 = addr_kDraygon_Ilist_9EA1;
      Enemy_Draygon *E2 = Get_Draygon(0x80);
      E2->base.current_instruction = v3;
      E2->base.instruction_timer = 1;
    }
  } else {
    E->draygon_var_A = FUNC16(Draygon_Func_25);
  }
}

void Draygon_Func_27(uint16 k) {  // 0xA59105
  Draygon_Func_40(k);
  uint16 v1 = addr_kDraygon_Ilist_9A68;
  if (Get_Draygon(k)->draygon_var_20)
    v1 = addr_kDraygon_Ilist_9E21;
  Enemy_Draygon *E = Get_Draygon(0x80);
  E->base.current_instruction = v1;
  E->base.instruction_timer = 1;
  Get_Draygon(0)->draygon_var_A = FUNC16(Draygon_Func_28);
}

void Draygon_Func_28(uint16 k) {  // 0xA59124
  Draygon_Func_40(k);
}

void Draygon_Func_29(void) {  // 0xA59128
  Samus_ReleaseFromDraygon_();
  samus_grapple_flags = 0;
  Enemy_Draygon *E = Get_Draygon(0);
  E->draygon_var_A = FUNC16(Draygon_Func_30);
  E->base.properties &= ~kEnemyProps_Tangible;
  uint16 v1 = addr_kDraygon_Ilist_9B5A;
  if (E->draygon_var_20)
    v1 = addr_kDraygon_Ilist_9F15;
  Enemy_Draygon *E2 = Get_Draygon(0x80);
  E2->base.current_instruction = v1;
  E2->base.instruction_timer = 1;
}

void Draygon_Func_30(void) {  // 0xA59154
  int16 v1;

  Draygon_Func_3();
  Enemy_Draygon *E = Get_Draygon(0);
  v1 = E->base.y_pos - 4;
  E->base.y_pos = v1;
  if (v1 < 0) {
    E->base.properties &= ~kEnemyProps_Tangible;
    E->draygon_var_A = FUNC16(Draygon_Func_4);
    E->draygon_var_B = 0;
    E->base.x_pos = E->draygon_var_00;
    E->base.y_pos = E->draygon_var_01;
  }
}

void Draygon_Func_31(void) {  // 0xA59185
  Enemy_Draygon *E = Get_Draygon(0);
  if ((Get_Draygon(cur_enemy_index)->base.frame_counter & 0xF) == 0) {
    R18_ = E->base.x_pos - 32;
    if (E->draygon_var_20)
      R18_ = E->base.x_pos + 32;
    R20_ = E->base.y_pos - 16;
    R22_ = 61;
    R24_ = 0;
    CreateSpriteAtPos();
  }
  R18_ = E->base.x_pos >> 2;
  R18_ = 64 - R18_;
  R20_ = E->base.y_pos >> 2;
  R20_ = 120 - R20_;
  E->draygon_var_43 = (uint8)(64 - CalculateAngleFromXY());
  R20_ = 1;
  R18_ = LOBYTE(E->draygon_var_43);
  ConvertAngleToXy();
  varE24 = R22_;
  varE26 = R24_;
  varE28 = R26_;
  varE2A = R28_;
  enemy_population_ptr = LOBYTE(E->draygon_var_43);
  EnemyFunc_B691();
  uint16 v3 = Abs16(E->base.x_pos - 256);
  if (sign16(v3 - 4)) {
    uint16 v4 = Abs16(E->base.y_pos - 480);
    if (sign16(v4 - 4)) {
      Draygon_Func_43();
      E->draygon_var_A = FUNC16(Draygon_Func_32);
      QueueMusic_Delayed8(3u);
      E->draygon_var_B = 416;
      E->base.current_instruction = addr_kDraygon_Ilist_97B9;
      E->base.instruction_timer = 1;
      uint16 v5 = E->base.properties | kEnemyProps_Deleted;
      Get_Draygon(0x80)->base.properties = v5;
      Get_Draygon(0xC0u)->base.properties = v5;
      uint16 v6 = addr_kDraygon_Ilist_999C;
      if (E->draygon_var_20)
        v6 = addr_kDraygon_Ilist_9D3E;
      Enemy_Draygon *E1 = Get_Draygon(0x40u);
      E1->base.current_instruction = v6;
      E1->base.instruction_timer = 1;
      E1->draygon_var_A = FUNC16(nullsub_169_A5);
    }
  }
}

void Draygon_Func_32(void) {  // 0xA59294
  Draygon_Func_34();
  Enemy_Draygon *E = Get_Draygon(0);
  if (--E->draygon_var_B)
    Draygon_Func_42();
  else
    E->draygon_var_A = FUNC16(Draygon_Func_33);
}

void Draygon_Func_33(void) {  // 0xA592AB
  Draygon_Func_34();
  Draygon_Func_42();
  Enemy_Draygon *E = Get_Draygon(0);
  if (!sign16(++E->base.y_pos - 576)) {
    E->base.current_instruction = addr_kDraygon_Ilist_98ED;
    E->base.instruction_timer = 1;
    uint16 v1 = E->base.properties | kEnemyProps_Deleted;
    E->base.properties = v1;
    Get_Draygon(0x40u)->base.properties = v1;
    uint16 v2 = area_index;
    *(uint16 *)&boss_bits_for_area[area_index] |= 1u;
    Enemy_ItemDrop_Draygon(v2);
    Draygon_Func_44();
  }
}

void Draygon_Func_34(void) {  // 0xA592EA
  if ((nmi_frame_counter_word & 7) == 0) {
    R18_ = (NextRandom() & 0x7F) + 192;
    R20_ = ((uint16)(random_number & 0x3F00) >> 8) + 400;
    R22_ = 21;
    R24_ = 0;
    CreateSpriteAtPos();
  }
}

void Draygon_Func_35(void) {  // 0xA5931C
  Enemy_Draygon *Draygon = Get_Draygon(cur_enemy_index);

  if ((Draygon->base.frame_counter & 0x7F) == 0) {
    Draygon = Get_Draygon(0);
    R18_ = Draygon->base.x_pos - 16;
    R20_ = Draygon->base.y_pos - 16;
    R22_ = 24;
    R24_ = 0;
    CreateSpriteAtPos();
  }
}

void Draygon_Func_36(void) {  // 0xA59342
  Enemy_Draygon *E = Get_Draygon(0);
  reg_BG2HOFS = E->draygon_var_5E + layer1_x_pos - E->base.x_pos - 450;
  reg_BG2VOFS = E->draygon_var_5F + layer1_y_pos - E->base.y_pos - 192;
}

void Draygon_Func_37(void) {  // 0xA59367
  Enemy_Draygon *E = Get_Draygon(0);
  if ((joypad2_last & 0x40) != 0)
    Draygon_Func_39();
  else
    Draygon_Func_38();
  if ((joypad2_new_keys & 0x4000) != 0) {
    uint16 v0 = addr_kDraygon_Ilist_98FE;
    if (E->draygon_var_20)
      v0 = addr_kDraygon_Ilist_9C90;
    E->base.current_instruction = v0;
    E->base.instruction_timer = 1;
  } else if ((joypad2_new_keys & 0x8000u) == 0) {
    if ((joypad2_new_keys & 0x80) != 0) {
      uint16 v4 = addr_kDraygon_Ilist_9825;
      if (E->draygon_var_20)
        v4 = addr_kDraygon_Ilist_9C18;
      Enemy_Draygon *E3 = Get_Draygon(0xC0u);
      E3->base.current_instruction = v4;
      E3->base.instruction_timer = 1;
    }
  } else {
    uint16 v2 = addr_kDraygon_Ilist_99FC;
    if (E->draygon_var_20)
      v2 = addr_kDraygon_Ilist_9D9E;
    Enemy_Draygon *E2 = Get_Draygon(0x80);
    E2->base.current_instruction = v2;
    E2->base.instruction_timer = 1;
  }
}

void Draygon_Func_38(void) {  // 0xA593DA
  Enemy_Draygon *E = Get_Draygon(0);
  uint16 v0 = joypad2_last, v2;
  if ((joypad2_last & 0x200) == 0 || (--E->base.x_pos, v0 == debug_enemy_population_pointer)) {
    if ((v0 & 0x100) == 0)
      goto LABEL_8;
    ++E->base.x_pos;
    if (v0 == debug_enemy_population_pointer)
      goto LABEL_8;
    debug_enemy_population_pointer = v0;
    v2 = addr_kDraygon_Ilist_97D1;
    E->draygon_var_20 = 1;
  } else {
    debug_enemy_population_pointer = v0;
    v2 = addr_kDraygon_Ilist_97BB;
    E->draygon_var_20 = 0;
  }
  E->base.current_instruction = v2;
  E->base.instruction_timer = 1;
LABEL_8:
  if ((joypad2_last & 0x800) != 0) {
    --E->base.y_pos;
  } else if ((joypad2_last & 0x400) != 0) {
    ++E->base.y_pos;
  }
}

void Draygon_Func_39(void) {  // 0xA59430
  Enemy_Draygon *E = Get_Draygon(0);
  uint16 v0 = joypad2_last, v2;
  if ((joypad2_last & 0x200) == 0
      || (E->base.x_pos -= 4, v0 = joypad2_last, joypad2_last == debug_enemy_population_pointer)) {
    if ((v0 & 0x100) == 0)
      goto LABEL_8;
    E->base.x_pos += 4;
    if (joypad2_last == debug_enemy_population_pointer)
      goto LABEL_8;
    debug_enemy_population_pointer = joypad2_last;
    v2 = addr_kDraygon_Ilist_97D1;
    E->draygon_var_20 = 1;
  } else {
    debug_enemy_population_pointer = joypad2_last;
    v2 = addr_kDraygon_Ilist_97BB;
    E->draygon_var_20 = 0;
  }
  E->base.current_instruction = v2;
  E->base.instruction_timer = 1;
LABEL_8:
  if ((joypad2_last & 0x800) != 0) {
    E->base.y_pos -= 4;
  } else if ((joypad2_last & 0x400) != 0) {
    ++E->base.y_pos;
    E->base.y_pos += 4;
  }
}

void Draygon_Func_40(uint16 k) {  // 0xA594A9
  int16 v1;

  v1 = 8;
  if (!Get_Draygon(k)->draygon_var_20)
    v1 = -8;
  Enemy_Draygon *E = Get_Draygon(0);
  samus_x_pos = E->base.x_pos + v1;
  samus_y_pos = E->base.y_pos + 40;
  EnemyFunc_B7A1();
  if ((samus_grapple_flags & 2) != 0) {
    samus_grapple_flags = 0;
    E->draygon_var_A = FUNC16(Draygon_Func_30);
  }
}

uint16 Draygon_Instr_1(uint16 k, uint16 j) {  // 0xA594DD
  uint16 *v2 = (uint16 *)RomPtr_A5(j);
  Get_Draygon(0)->base.current_instruction = *v2;
  Get_Draygon(0x40u)->base.current_instruction = v2[1];
  Get_Draygon(0x80)->base.current_instruction = v2[2];
  Enemy_Draygon *E = Get_Draygon(0xC0u);
  E->base.current_instruction = v2[3];
  Get_Draygon(0)->base.instruction_timer = 1;
  Get_Draygon(0x40u)->base.instruction_timer = 1;
  Get_Draygon(0x80)->base.instruction_timer = 1;
  E->base.instruction_timer = 1;
  return j + 8;
}

void Draygon_Hurt(void) {  // 0xA5954D
  int16 v2;
  int16 v7;
  int16 v9;

  uint16 v0 = addr_kDraygon_MorePalettes2;
  if ((Get_Draygon(cur_enemy_index)->base.flash_timer & 2) != 0)
    v0 = addr_word_A5A297;
  uint16 v1 = addr_kDraygon_BigSprmap_C08F__plus__17;
  remaining_enemy_spritemap_entries = 16;
  do {
    v2 = *(uint16 *)RomPtr_A5(v0);
    *(uint16 *)RomPtr_7E(v1) = v2;
    v1 += 2;
    v0 += 2;
    --remaining_enemy_spritemap_entries;
  } while (remaining_enemy_spritemap_entries);
  if ((Get_Draygon(cur_enemy_index)->base.flash_timer & 2) == 0) {
    uint16 v3 = 4 * Get_Draygon(0)->draygon_var_0E;
    for (int i = 0; i != 8; i += 2) {
      palette_buffer[(i >> 1) + 89] = g_word_A596AF[v3 >> 1];
      v3 += 2;
    }
  }
  uint16 v5 = addr_kDraygon_Palette;
  if ((Get_Draygon(cur_enemy_index)->base.flash_timer & 2) != 0)
    v5 = addr_word_A5A297;
  uint16 v6 = addr_kDraygon_BigSprmap_C11B__plus__197;
  remaining_enemy_spritemap_entries = 16;
  do {
    v7 = *(uint16 *)RomPtr_A5(v5);
    *(uint16 *)RomPtr_7E(v6) = v7;
    v6 += 2;
    v5 += 2;
    --remaining_enemy_spritemap_entries;
  } while (remaining_enemy_spritemap_entries);
  if ((samus_grapple_flags & 1) != 0) {
    Enemy_Draygon *E = Get_Draygon(0);
    if ((E->base.frame_counter & 7) == 0) {
      v9 = E->base.health - 256;
      if (v9 < 0)
        v9 = 0;
      E->base.health = v9;
      sub_A5960D();
    }
  }
}

void Draygon_Touch(void) {  // 0xA595EA
  NormalEnemyTouchAiSkipDeathAnim_CurEnemy();
  sub_A5960D();
}

void Draygon_Shot(void) {  // 0xA595F0
  Enemy_Draygon *E = Get_Draygon(0);
  if (sign16(E->draygon_var_0F - 152))
    E->draygon_var_0F += 8;
  NormalEnemyShotAiSkipDeathAnim_CurEnemy();
  sub_A5960D();
}

void Draygon_Powerbomb(void) {  // 0xA59607
  NormalEnemyPowerBombAiSkipDeathAnim_CurEnemy();
  sub_A5960D();
}

void sub_A5960D(void) {  // 0xA5960D
  if (Get_Draygon(cur_enemy_index)->base.health) {
    Draygon_Func_41();
  } else {
    grapple_beam_function = FUNC16(GrappleBeam_Func2);
    uint16 v0 = addr_kDraygon_Ilist_9867;
    Enemy_Draygon *E = Get_Draygon(0);
    if (E->draygon_var_20)
      v0 = addr_kDraygon_Ilist_9C5A;
    E->base.current_instruction = v0;
    E->base.instruction_timer = 1;
    uint16 v3 = addr_kDraygon_Ilist_997A;
    if (E->draygon_var_20)
      v3 = addr_kDraygon_Ilist_9D1C;
    Enemy_Draygon *E1 = Get_Draygon(0x40u);
    E1->base.current_instruction = v3;
    E1->base.instruction_timer = 1;
    E1->draygon_var_A = FUNC16(nullsub_169_A5);
    E->draygon_var_A = FUNC16(Draygon_Func_31);
    Samus_ReleaseFromDraygon_();
    samus_grapple_flags = 0;
    R18_ = 256 - E->base.x_pos;
    R20_ = 480 - E->base.y_pos;
    E->draygon_var_43 = (uint8)(64 - CalculateAngleFromXY());
    R20_ = 1;
    R18_ = LOBYTE(E->draygon_var_43);
    ConvertAngleToXy();
    E->draygon_var_28 = R22_;
    E->draygon_var_29 = R24_;
    E->draygon_var_2A = R26_;
    E->draygon_var_2B = R28_;
  }
}

void Draygon_Func_41(void) {  // 0xA59701
  int i;
  uint16 j;
  Enemy_Draygon *E = Get_Draygon(0);
  for (i = 0; ; i += 2) {
    if ((int16)(E->base.health - g_word_A596EF[i >> 1]) >= 0)
      break;
  }
  if (i != E->draygon_var_0E) {
    E->draygon_var_0E = i;
    uint16 v2 = 4 * E->draygon_var_0E;
    for (j = 0; j != 8; j += 2) {
      palette_buffer[(j >> 1) + 89] = g_word_A596AF[v2 >> 1];
      v2 += 2;
    }
  }
}

uint16 Draygon_Instr_13(uint16 k, uint16 j) {  // 0xA59736
  uint16 v2 = *(uint16 *)RomPtr_A5(j);
  Get_Draygon(k)->draygon_var_A = v2;
  return j + 2;
}

uint16 Draygon_Instr_8(uint16 k, uint16 j) {  // 0xA5973F
  Draygon_Func_47();
  R22_ = 21;
  R24_ = 0;
  CreateSpriteAtPos();
  return j;
}

uint16 Draygon_Instr_7(uint16 k, uint16 j) {  // 0xA59752
  Draygon_Func_47();
  R22_ = 3;
  R24_ = 0;
  CreateSpriteAtPos();
  return j;
}

uint16 Draygon_Instr_6(uint16 k, uint16 j) {  // 0xA59765
  Draygon_Func_47();
  R22_ = 29;
  R24_ = 0;
  CreateSpriteAtPos();
  return j;
}

uint16 Draygon_Instr_9(uint16 k, uint16 j) {  // 0xA59778
  Draygon_Func_47();
  R22_ = 24;
  R24_ = 0;
  CreateSpriteAtPos();
  return j;
}

void Draygon_Func_47(void) {  // 0xA5978B
  NextRandom();
  R18_ = (random_number & 0x7F) - 64;
  Enemy_Draygon *E = Get_Draygon(0);
  R18_ += E->base.x_pos;
  R20_ = ((uint16)(random_number & 0x7F00) >> 8) - 64;
  R20_ += E->base.y_pos;
}

uint16 Draygon_Instr_2(uint16 k, uint16 j) {  // 0xA59895
  room_loading_irq_handler = 12;
  return j;
}

uint16 Draygon_Instr_11(uint16 k, uint16 j) {  // 0xA598D3
  Enemy_Draygon *E2 = Get_Draygon(0x80);
  E2->base.instruction_timer = 1;
  E2->base.current_instruction = addr_kDraygon_Ilist_97B9;

  Enemy_Draygon *E3 = Get_Draygon(0xC0u);
  E3->base.instruction_timer = 1;
  E3->base.current_instruction = addr_kDraygon_Ilist_97B9;
  return j;
}

uint16 Draygon_Instr_5(uint16 k, uint16 j) {  // 0xA598EF
  Enemy_Draygon *E = Get_Draygon(0);
  E->base.properties |= kEnemyProps_Tangible;
  return j;
}

uint16 Draygon_Instr_15(uint16 k, uint16 j) {  // 0xA59B9A
  Enemy_Draygon *E = Get_Draygon(0);
  E->draygon_var_0F = 24;
  uint8 *v3 = RomPtr_A0(E->base.enemy_ptr);
  uint16 v4 = SuitDamageDivision(*((uint16 *)v3 + 3));
  Samus_DealDamage(v4);
  earthquake_timer = 32;
  earthquake_type = 7;
  R18_ = samus_x_pos;
  R20_ = samus_y_pos + 16;
  R22_ = 21;
  R24_ = 0;
  CreateSpriteAtPos();
  return j;
}

uint16 Draygon_Instr_17(uint16 k, uint16 j) {  // 0xA59C8A
  room_loading_irq_handler = 12;
  return j;
}

uint16 Draygon_Instr_14(uint16 k, uint16 j) {  // 0xA59E0A
  uint8 *v2 = RomPtr_A5(j);
  Enemy_Draygon *E = Get_Draygon(0);
  E->draygon_var_5E = *(uint16 *)v2;
  E->draygon_var_5F = *((uint16 *)v2 + 1);
  return j + 4;
}

uint16 Draygon_Instr_16(uint16 k, uint16 j) {  // 0xA59F57
  uint16 v2 = *(uint16 *)RomPtr_A5(j);
  Get_Draygon(0)->draygon_var_A = v2;
  return j + 2;
}

uint16 Draygon_Instr_10(uint16 k, uint16 j) {  // 0xA59F60
  uint16 *v2 = (uint16 *)RomPtr_A5(j);
  QueueSfx2_Max6(*v2);
  return j + 2;
}

uint16 Draygon_Instr_4(uint16 k, uint16 j) {  // 0xA59F6E
  uint16 *v2 = (uint16 *)RomPtr_A5(j);
  QueueSfx3_Max6(*v2);
  return j + 2;
}

uint16 Draygon_Instr_12(uint16 k, uint16 j) {  // 0xA59F7C
  Enemy_Draygon *E = Get_Draygon(0);
  R18_ = E->base.x_pos - 28;
  R20_ = E->base.y_pos - 16;
  enemy_projectile_unk1995 = (NextRandom() & 0x3F) + 128;
  SpawnEnemyProjectileWithGfx(2u, cur_enemy_index, addr_stru_868E50);
  return j;
}

uint16 Draygon_Instr_18(uint16 k, uint16 j) {  // 0xA59FAE
  Enemy_Draygon *E = Get_Draygon(0);
  R18_ = E->base.x_pos + 24;
  R20_ = E->base.y_pos - 16;
  enemy_projectile_unk1995 = (NextRandom() & 0x3F) + 192;
  SpawnEnemyProjectileWithGfx(2u, cur_enemy_index, addr_stru_868E50);
  return j;
}

void Draygon_Func_42(void) {  // 0xA59FE0
  uint16 v0 = 62;
  for (int i = 20; i >= 0; i -= 4) {
    int v2 = i >> 1;
    int v3 = v0 >> 1;
    uint16 v4 = sprite_x_subpos[v3], v6;
    if (((g_word_A5A1DF[v2] + 64) & 0x80) != 0) {
      bool v5 = __CFADD__uint16(g_word_A5A1AF[v2], v4);
      sprite_x_subpos[v3] = g_word_A5A1AF[v2] + v4;
      v6 = v5 + sprite_x_pos[v3];
    } else {
      bool v5 = v4 < g_word_A5A1AF[v2];
      sprite_x_subpos[v3] = v4 - g_word_A5A1AF[v2];
      v6 = sprite_x_pos[v3] - (v5 + varE24);
    }
    sprite_x_pos[v3] = v6;
    int v7 = v0 >> 1;
    uint16 v8 = sprite_y_subpos[v7], v9;
    if (((g_word_A5A1DF[v2] + 128) & 0x80) != 0) {
      bool v5 = __CFADD__uint16(g_word_A5A1AF[v2 + 1], v8);
      sprite_y_subpos[v7] = g_word_A5A1AF[v2 + 1] + v8;
      v9 = v5 + sprite_y_pos[v7];
    } else {
      bool v5 = v8 < g_word_A5A1AF[v2 + 1];
      sprite_y_subpos[v7] = v8 - g_word_A5A1AF[v2 + 1];
      v9 = sprite_y_pos[v7] - v5;
    }
    sprite_y_pos[v7] = v9;
    v0 -= 2;
  }
}

void Draygon_Func_43(void) {  // 0xA5A06C
  int16 v1;
  int16 j;

  for (int i = 62; i >= 0; i -= 2)
    sprite_instr_list_ptrs[i >> 1] = 0;
  v1 = 2;
  uint16 v2 = 20;
  do {
    int v3 = v2 >> 1;
    R18_ = g_word_A5A1C7[v3];
    R20_ = g_word_A5A1C7[v3 + 1];
    R22_ = 59;
    R24_ = 3584;
    CreateSpriteAtPos();
    v2 -= 4;
    --v1;
  } while (v1 >= 0);
  for (j = 2; j >= 0; --j) {
    int v5 = v2 >> 1;
    R18_ = g_word_A5A1C7[v5];
    R20_ = g_word_A5A1C7[v5 + 1];
    R22_ = 60;
    R24_ = 3584;
    CreateSpriteAtPos();
    v2 -= 4;
  }
}

void Draygon_Func_44(void) {  // 0xA5A0C6
  for (int i = 62; i >= 0; i -= 2)
    sprite_instr_list_ptrs[i >> 1] = 0;
}

void Draygon_Func_45(void) {  // 0xA5A0D9
  R18_ = 16;
  R20_ = 384;
  R22_ = 59;
  R24_ = 3584;
  CreateSpriteAtPos();
  R18_ = 16;
  R20_ = 384;
  R22_ = 59;
  R24_ = 3584;
  CreateSpriteAtPos();
  R18_ = 16;
  R20_ = 384;
  R22_ = 59;
  R24_ = 3584;
  CreateSpriteAtPos();
  R18_ = 16;
  R20_ = 384;
  R22_ = 59;
  R24_ = 3584;
  CreateSpriteAtPos();
}

void Draygon_Func_46(void) {  // 0xA5A13E
  uint16 v0 = 62;
  do {
    uint16 v1 = Get_Draygon(0)->draygon_var_46 + g_word_A5A19F[(uint16)(v0 - 56) >> 1];
    if ((v1 & 0x8000u) == 0) {
      uint16 v2 = v1;
      if (*(uint16 *)&g_byte_A5CE07[v1] == 0x8080) {
        sprite_instr_list_ptrs[v0 >> 1] = 0;
      } else {
        int v3 = v0 >> 1;
        sprite_x_pos[v3] += SignExtend8(g_byte_A5CE07[v1]);
        sprite_y_pos[v3] += SignExtend8(g_byte_A5CE07[v2 + 1]);
      }
    }
    v0 -= 2;
  } while ((int16)(v0 - 56) >= 0);
  Enemy_Draygon *E = Get_Draygon(0);
  E->draygon_var_46 += 4;
}

void DraygonsEye_Init(void) {  // 0xA5C46B
  Enemy_Draygon *E = Get_Draygon(cur_enemy_index);
  E->base.current_instruction = addr_kDraygon_Ilist_9944;
  E->draygon_var_A = FUNC16(nullsub_169_A5);
}

uint16 Draygon_Instr_3(uint16 k, uint16 j) {  // 0xA5C47B
  uint16 v2 = *(uint16 *)RomPtr_A5(j);
  Get_Draygon(0x40u)->draygon_var_A = v2;
  return j + 2;
}

void DraygonsEye_Main(void) {  // 0xA5C486
  Enemy_Draygon *E = Get_Draygon(cur_enemy_index);
  CallDraygonFunc(E->draygon_var_A | 0xA50000, cur_enemy_index);
}

void Draygon_Func_48(uint16 k) {  // 0xA5C48D
  if ((Get_Draygon(k)->base.frame_counter & 0x7F) == 0) {
    Enemy_Draygon *E1 = Get_Draygon(0x40u);
    R18_ = E1->base.x_pos - 24;
    R20_ = E1->base.y_pos - 32;
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 0x18);
  }
  Enemy_Draygon *E0 = Get_Draygon(0);
  R18_ = E0->base.x_pos - 24;
  R18_ = samus_x_pos - R18_;
  R20_ = E0->base.y_pos - 32;
  R20_ = samus_y_pos - R20_;
  uint16 v4 = CalculateAngleFromXY();
  Enemy_Draygon *E = Get_Draygon(cur_enemy_index);
  if (v4 != E->draygon_var_F) {
    uint16 v6 = addr_kDraygon_Ilist_99BA;
    if (!sign16(v4 - 32)) {
      v6 = addr_kDraygon_Ilist_99B4;
      if (!sign16(v4 - 96)) {
        v6 = addr_kDraygon_Ilist_99C0;
        if (!sign16(v4 - 160)) {
          v6 = addr_kDraygon_Ilist_99AE;
          if (!sign16(v4 - 224))
            v6 = addr_kDraygon_Ilist_99BA;
        }
      }
    }
    E->draygon_var_F = v4;
    E->base.current_instruction = v6;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

void Draygon_Func_49(uint16 k) {  // 0xA5C513
  Enemy_Draygon *E0 = Get_Draygon(0);

  if ((Get_Draygon(k)->base.frame_counter & 0x7F) == 0) {
    R18_ = E0->base.x_pos + 24;
    R20_ = E0->base.y_pos - 32;
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 0x18);
  }

  R18_ = E0->base.x_pos + 24;
  R18_ = samus_x_pos - R18_;
  R20_ = E0->base.y_pos - 32;
  R20_ = samus_y_pos - R20_;
  uint16 v4 = CalculateAngleFromXY();
  Enemy_Draygon *E = Get_Draygon(cur_enemy_index);
  if (v4 != E->draygon_var_F) {
    uint16 v6 = addr_kDraygon_Ilist_9D5C;
    if (!sign16(v4 - 32)) {
      v6 = addr_kDraygon_Ilist_9D50;
      if (!sign16(v4 - 96)) {
        v6 = addr_kDraygon_Ilist_9D62;
        if (!sign16(v4 - 160)) {
          v6 = addr_kDraygon_Ilist_9D56;
          if (!sign16(v4 - 224))
            v6 = addr_kDraygon_Ilist_9D5C;
        }
      }
    }
    E->draygon_var_F = v4;
    E->base.current_instruction = v6;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

void DraygonsTail_Init(void) {  // 0xA5C599
  EnemyData *v0 = gEnemyData(cur_enemy_index);
  v0->current_instruction = addr_kDraygon_Ilist_99FC;
  v0->palette_index = 3584;
}

void DraygonsArms_Init(void) {  // 0xA5C5AD
  EnemyData *v0 = gEnemyData(cur_enemy_index);
  v0->current_instruction = addr_kDraygon_Ilist_97E7;
  v0->palette_index = 3584;
  v0->layer = 2;
}

uint16 Draygon_Instr_25(uint16 k, uint16 j) {  // 0xA5E75F
  Enemy_Draygon *E = Get_Draygon(0);
  if (sign16(E->draygon_var_0B - 40))
    E->draygon_var_0B += 8;
  return j;
}

uint16 Draygon_Instr_24(uint16 k, uint16 j) {  // 0xA5E771
  Get_Draygon(0)->draygon_var_2F = 0;
  return j;
}

void Draygon_Func_50(uint16 j) {  // 0xA5E811
  uint8 *v1 = RomPtr_A5(j);
  Enemy_Draygon *E = Get_Draygon(0);
  E->draygon_var_0B = *(uint16 *)v1;
  E->draygon_var_0C = *((uint16 *)v1 + 1);
  E->draygon_var_0A = *((uint16 *)v1 + 2);
}

uint16 Draygon_Instr_21(uint16 k, uint16 j) {  // 0xA5E82D
  uint8 *v2 = RomPtr_A5(j);
  Enemy_Draygon *E = Get_Draygon(0);
  E->draygon_var_0B = *(uint16 *)v2;
  E->draygon_var_0C = *((uint16 *)v2 + 1);
  return j + 4;
}

void Draygon_Func_51(uint16 j) {  // 0xA5E840
  uint16 v1 = *(uint16 *)RomPtr_A5(j);
  Get_Draygon(0)->draygon_var_0B = v1;
}

void Draygon_Func_52(uint16 j) {  // 0xA5E84A
  uint16 v1 = *(uint16 *)RomPtr_A5(j);
  Get_Draygon(0)->draygon_var_0C = v1;
}

uint16 Draygon_Instr_53(uint16 k, uint16 j) {  // 0xA5E854
  Enemy_Draygon *E = Get_Draygon(0);
  E->draygon_var_0B += *(uint16 *)RomPtr_A5(j);
  return j + 2;
}

uint16 Draygon_Instr_54(uint16 k, uint16 j) {  // 0xA5E863
  Enemy_Draygon *E = Get_Draygon(0);
  E->draygon_var_0C += *(uint16 *)RomPtr_A5(j);
  return j + 2;
}

uint16 Draygon_Instr_22(uint16 k, uint16 j) {  // 0xA5E872
  kraid_unk9000 = *(uint16 *)RomPtr_A5(j);
  return j + 2;
}

uint16 Draygon_Instr_27(uint16 k, uint16 j) {  // 0xA5E87C
  Enemy_Draygon *E = Get_Draygon(0);
  E->base.x_pos = 128;
  E->base.y_pos = 624;
  E->base.properties = E->base.properties & 0x5BFF | 0xA000;
  return j;
}

uint16 Draygon_Instr_23(uint16 k, uint16 j) {  // 0xA5E895
  uint16 *v2 = (uint16 *)RomPtr_A5(j);
  QueueSfx2_Max6(*v2);
  return j + 2;
}

uint16 Draygon_Instr_55(uint16 k, uint16 j) {  // 0xA5E8A3
  uint16 *v2 = (uint16 *)RomPtr_A5(j);
  QueueSfx3_Max6(*v2);
  return j + 2;
}

uint16 Draygon_Instr_30(uint16 k, uint16 j) {  // 0xA5E8B1
  Enemy_ItemDrop_SporeSpawn(k);
  return j;
}

uint16 Draygon_Instr_20(uint16 k, uint16 j) {  // 0xA5E8BA
  uint16 v2 = *(uint16 *)RomPtr_A5(j);
  Get_Draygon(cur_enemy_index)->draygon_var_A = v2;
  return j + 2;
}

uint16 Draygon_Instr_29(uint16 k, uint16 j) {  // 0xA5E8CA
  uint16 m;
  uint16 n;

  R18_ = j;
  uint16 v2 = *(uint16 *)RomPtr_A5(j);
  for (int i = 0; i != 32; i += 2) {
    palette_buffer[(i >> 1) + 144] = g_word_A5E3F9[v2 >> 1];
    v2 += 2;
  }
  uint16 v4 = *(uint16 *)RomPtr_A5(R18_);
  for (m = 0; m != 32; m += 2) {
    palette_buffer[(m >> 1) + 64] = g_word_A5E4F9[v4 >> 1];
    v4 += 2;
  }
  uint16 v6 = *(uint16 *)RomPtr_A5(R18_);
  for (n = 0; n != 32; n += 2) {
    palette_buffer[(n >> 1) + 112] = g_word_A5E5D9[v6 >> 1];
    v6 += 2;
  }
  return j + 2;
}

uint16 Draygon_Instr_19(uint16 k, uint16 j) {  // 0xA5E91C
  uint16 m;
  uint16 n;

  R18_ = j;
  uint16 v2 = *(uint16 *)RomPtr_A5(j);
  for (int i = 0; i != 32; i += 2) {
    target_palettes[(i >> 1) + 144] = g_word_A5E3F9[v2 >> 1];
    v2 += 2;
  }
  uint16 v4 = *(uint16 *)RomPtr_A5(R18_);
  for (m = 0; m != 32; m += 2) {
    target_palettes[(m >> 1) + 64] = g_word_A5E4F9[v4 >> 1];
    v4 += 2;
  }
  uint16 v6 = *(uint16 *)RomPtr_A5(R18_);
  for (n = 0; n != 32; n += 2) {
    target_palettes[(n >> 1) + 112] = g_word_A5E5D9[v6 >> 1];
    v6 += 2;
  }
  return j + 2;
}

uint16 Draygon_Instr_28(uint16 k, uint16 j) {  // 0xA5E96E
  NextRandom();
  R18_ = (random_number & 0x7F) - 64;
  Enemy_Draygon *E = Get_Draygon(0);
  R18_ += E->base.x_pos;
  R20_ = ((uint16)(random_number & 0x7F00) >> 8) - 64;
  R20_ += E->base.y_pos;
  SpawnEnemyProjectileWithRoomGfx(0xE509, 0x15);
  QueueSfx2_Max6(0x29u);
  return j;
}

uint16 Draygon_Instr_26(uint16 k, uint16 j) {  // 0xA5E9B1
  NextRandom();
  R18_ = (random_number & 0x7F) - 64;
  Enemy_Draygon *E = Get_Draygon(0);
  R18_ += E->base.x_pos;
  R20_ = ((uint16)(random_number & 0x3F00) >> 8) - 32;
  R20_ += E->base.y_pos;
  R22_ = 3;
  R24_ = 0;
  CreateSpriteAtPos();
  QueueSfx2_Max6(0x25u);
  return j;
}

void sub_A5E9F5(void) {  // 0xA5E9F5
  if ((nmi_frame_counter_word & 0xF) == 0) {
    NextRandom();
    R18_ = (random_number & 0x3F) + 96;
    R20_ = ((uint16)(random_number & 0xF00) >> 8) + 480;
    R22_ = 21;
    R24_ = 0;
    CreateSpriteAtPos();
  }
}

void SporeSpawn_Init(void) {  // 0xA5EA2A
  static const SpawnHardcodedPlmArgs unk_A5EAB7 = { 0x07, 0x1e, 0xb793 };

  uint16 v0 = 0;
  for (int i = 0; i != 32; i += 2) {
    target_palettes[(i >> 1) + 240] = kSporeSpawn_Palette[v0 >> 1];
    v0 += 2;
  }
  SpawnEnemyProjectileWithGfx(0, cur_enemy_index, addr_kEproj_SporeSpawnsStalk);
  SpawnEnemyProjectileWithGfx(1u, cur_enemy_index, addr_kEproj_SporeSpawnsStalk);
  SpawnEnemyProjectileWithGfx(2u, cur_enemy_index, addr_kEproj_SporeSpawnsStalk);
  SpawnEnemyProjectileWithGfx(3u, cur_enemy_index, addr_kEproj_SporeSpawnsStalk);
  Enemy_SporeSpawn *E = Get_SporeSpawn(cur_enemy_index);
  Enemy_SporeSpawn *E0 = Get_SporeSpawn(0);
  E->ssn_var_04 = E->base.x_pos;
  E->ssn_var_05 = E->base.y_pos - 72;
  E->ssn_var_C = E->base.x_pos;
  E->ssn_var_D = E->base.y_pos;
  E->ssn_var_F = 0;
  if ((boss_bits_for_area[area_index] & 2) != 0) {
    E0->base.current_instruction = addr_kDraygon_Ilist_E6B9;
    E0->ssn_var_A = FUNC16(nullsub_223);
    E0->base.properties |= 0x8000u;
    SporeSpawn_Func_5();
    SpawnHardcodedPlm(&unk_A5EAB7);
    scrolling_finished_hook = 0;
  } else {
    E0->base.current_instruction = addr_kDraygon_Ilist_E6C7;
    flag_process_all_enemies = -1;
    uint16 v5 = cur_enemy_index;
    E->ssn_var_A = FUNC16(nullsub_223);
    scrolling_finished_hook = FUNC16(Samus_ScrollFinishedHook_SporeSpawnFight);
    E->base.y_pos -= 128;
    SpawnEnemyProjectileWithGfx(0, v5, addr_kEproj_SporeSpawners);
    SpawnEnemyProjectileWithGfx(1u, v5, addr_kEproj_SporeSpawners);
    SpawnEnemyProjectileWithGfx(2u, v5, addr_kEproj_SporeSpawners);
    SpawnEnemyProjectileWithGfx(3u, v5, addr_kEproj_SporeSpawners);
    SporeSpawn_Func_5();
  }
}

void CallSporeSpawnFunc(uint32 ea) {
  switch (ea) {
  case fnnullsub_223: return;
  case fnSporeSpawn_Func_1: SporeSpawn_Func_1(); return;
  case fnSporeSpawn_Func_2: SporeSpawn_Func_2(cur_enemy_index); return;
  case fnSporeSpawn_Func_3: SporeSpawn_Func_3(); return;
  case fnSporeSpawn_Func_4: SporeSpawn_Func_4(); return;
  default: Unreachable();
  }
}

void SporeSpawn_Main(void) {  // 0xA5EB13
  Enemy_SporeSpawn *E = Get_SporeSpawn(cur_enemy_index);
  CallSporeSpawnFunc(E->ssn_var_A | 0xA50000);
}

void SporeSpawn_Func_1(void) {  // 0xA5EB1B
  SporeSpawn_Func_5();
  Enemy_SporeSpawn *E = Get_SporeSpawn(cur_enemy_index);
  uint16 v1 = E->base.y_pos + 1;
  E->base.y_pos = v1;
  if (!sign16(v1 - 624)) {
    E->base.current_instruction = addr_kDraygon_Ilist_E6D5;
    E->base.instruction_timer = 1;
  }
  Enemy_SporeSpawn *E0 = Get_SporeSpawn(0);
  E0->ssn_var_0B = 48;
  E0->ssn_var_0C = 1;
  E0->ssn_var_0A = 192;
}

void SporeSpawn_Func_2(uint16 k) {  // 0xA5EB52
  SporeSpawn_Func_5();
  Enemy_SporeSpawn *E0 = Get_SporeSpawn(0);
  draw_enemy_layer = E0->ssn_var_0B;
  uint16 v2 = CosineMult8bit(E0->ssn_var_0A);
  Enemy_SporeSpawn *E = Get_SporeSpawn(k);
  E->base.x_pos = E->ssn_var_C + v2;
  draw_enemy_layer = E0->ssn_var_0B - 16;
  E->base.y_pos = E->ssn_var_D + SineMult8bitNegative(2 * (E0->ssn_var_0A - 64));
  E0->ssn_var_0A = (uint8)(LOBYTE(E0->ssn_var_0C) + E0->ssn_var_0A);
}

void SporeSpawn_Func_3(void) {  // 0xA5EB9B
  Enemy_SporeSpawn *E0 = Get_SporeSpawn(0);
  R18_ = 128 - E0->base.x_pos;
  R20_ = 624 - E0->base.y_pos;
  E0->ssn_var_43 = (uint8)(64 - CalculateAngleFromXY());
  R20_ = 1;
  R18_ = LOBYTE(E0->ssn_var_43);
  ConvertAngleToXy();
  Enemy_SporeSpawn *E = Get_SporeSpawn(cur_enemy_index);
  E->ssn_var_28 = R22_;
  E->ssn_var_29 = R24_;
  E->ssn_var_2A = R26_;
  E->ssn_var_2B = R28_;
}

void SporeSpawn_Func_4(void) {  // 0xA5EBEE
  Enemy_SporeSpawn *E = Get_SporeSpawn(cur_enemy_index);
  varE24 = E->ssn_var_28;
  varE26 = E->ssn_var_29;
  varE28 = E->ssn_var_2A;
  varE2A = E->ssn_var_2B;
  Enemy_SporeSpawn *E0 = Get_SporeSpawn(0);
  enemy_population_ptr = LOBYTE(E0->ssn_var_43);
  EnemyFunc_B691();
  uint16 v2 = Abs16(E0->base.x_pos - 128);
  if (sign16(v2 - 8)) {
    uint16 v3 = Abs16(E0->base.y_pos - 624);
    if (sign16(v3 - 8))
      E0->ssn_var_A = FUNC16(nullsub_223);
  }
  SporeSpawn_Func_5();
  sub_A5E9F5();
}

void SporeSpawn_Func_5(void) {  // 0xA5EC49
  Enemy_SporeSpawn *E = Get_SporeSpawn(0);
  uint16 v1 = E->base.x_pos - E->ssn_var_04, v2;
  if ((v1 & 0x8000u) == 0) {
    R18_ = v1 >> 1;
    R20_ = v1 >> 2;
    R22_ = (v1 >> 1) + (v1 >> 2);
    enemy_projectile_x_pos[14] = 128;
    enemy_projectile_x_pos[15] = E->ssn_var_04 + (v1 >> 2);
    E->ssn_var_20 = enemy_projectile_x_pos[15];
    enemy_projectile_x_pos[16] = E->ssn_var_04 + R18_;
    E->ssn_var_21 = enemy_projectile_x_pos[16];
    v2 = E->ssn_var_04 + R22_;
  } else {
    R18_ = (uint16)(E->ssn_var_04 - E->base.x_pos) >> 1;
    R20_ = (uint16)-v1 >> 2;
    R22_ = R18_ + R20_;
    enemy_projectile_x_pos[14] = 128;
    enemy_projectile_x_pos[15] = E->ssn_var_04 - R20_;
    E->ssn_var_20 = enemy_projectile_x_pos[15];
    enemy_projectile_x_pos[16] = E->ssn_var_04 - R18_;
    E->ssn_var_21 = enemy_projectile_x_pos[16];
    v2 = E->ssn_var_04 - R22_;
  }
  enemy_projectile_x_pos[17] = v2;
  E->ssn_var_22 = v2;
  uint16 v3 = E->base.y_pos - 40 - E->ssn_var_05, v4;
  if ((v3 & 0x8000u) == 0) {
    R18_ = v3 >> 1;
    R20_ = v3 >> 2;
    R22_ = (v3 >> 1) + (v3 >> 2);
    enemy_projectile_y_pos[14] = 560;
    enemy_projectile_y_pos[15] = E->ssn_var_05 + (v3 >> 2);
    E->ssn_var_23 = enemy_projectile_y_pos[15];
    enemy_projectile_y_pos[16] = E->ssn_var_05 + R18_;
    E->ssn_var_24 = enemy_projectile_y_pos[16];
    v4 = E->ssn_var_05 + R22_;
  } else {
    R18_ = (uint16)(E->ssn_var_05 - (E->base.y_pos - 40)) >> 1;
    R20_ = (uint16)-v3 >> 2;
    R22_ = R18_ + R20_;
    enemy_projectile_y_pos[14] = 560;
    enemy_projectile_y_pos[15] = E->ssn_var_05 - R20_;
    E->ssn_var_23 = enemy_projectile_y_pos[15];
    enemy_projectile_y_pos[16] = E->ssn_var_05 - R18_;
    E->ssn_var_24 = enemy_projectile_y_pos[16];
    v4 = E->ssn_var_05 - R22_;
  }
  enemy_projectile_y_pos[17] = v4;
  E->ssn_var_25 = v4;
}

void SporeSpawn_Shot(void) {  // 0xA5ED5A
  int16 v4;

  uint16 v0 = 2 * collision_detection_index;
  uint16 v1 = projectile_type[collision_detection_index];
  if ((v1 & 0x700) != 0 || (v1 & 0x10) != 0) {
    EnemyFunc_A6B4_UsedBySporeSpawn();
    v0 = cur_enemy_index;
    Enemy_SporeSpawn *EK = Get_SporeSpawn(v0);
    if (EK->base.flash_timer) {
      Enemy_SporeSpawn *E = Get_SporeSpawn(0);
      EK->ssn_var_A = FUNC16(SporeSpawn_Func_2);
      v4 = 2;
      if (sign16(EK->base.health - 400)) {
        if ((E->ssn_var_0C & 0x8000u) != 0)
          v4 = -2;
        E->ssn_var_0C = v4;
      }
      if (!E->ssn_var_2F) {
        E->ssn_var_0C = -E->ssn_var_0C;
        E->ssn_var_2F = 1;
        EK->base.current_instruction = addr_stru_A5E729;
        EK->base.instruction_timer = 1;
        uint16 v7 = 96;
        uint16 health = EK->base.health;
        if (!sign16(health - 70)) {
          v7 = 64;
          if (!sign16(health - 410)) {
            v7 = 32;
            if (!sign16(health - 770))
              v7 = 0;
          }
        }
        if (health != E->ssn_var_40) {
          E->ssn_var_40 = health;
          SporeSpawn_Func_7(v7);
        }
      }
    }
    SporeSpawn_Func_6();
  }
}

void SporeSpawn_Touch(void) {  // 0xA5EDEC
  NormalEnemyTouchAiSkipDeathAnim_CurEnemy();
  SporeSpawn_Func_6();
}

void SporeSpawn_Func_6(void) {  // 0xA5EDF3
  static const SpawnHardcodedPlmArgs unk_A5EE45 = { 0x07, 0x1e, 0xb78f };

  if (!Get_SporeSpawn(cur_enemy_index)->base.health) {
    Enemy_SporeSpawn *E = Get_SporeSpawn(cur_enemy_index);
    Enemy_SporeSpawn *E0 = Get_SporeSpawn(0);
    E0->ssn_var_0E = 0;
    E->base.invincibility_timer = 0;
    E->base.flash_timer = 0;
    E->base.ai_handler_bits = 0;
    E->base.properties |= kEnemyProps_Tangible;
    for (int i = 26; i >= 0; i -= 2)
      *(uint16 *)((char *)enemy_projectile_id + (uint16)i) = 0;
    E0->base.current_instruction = addr_kDraygon_Ilist_E77D;
    E0->base.instruction_timer = 1;
    *(uint16 *)&boss_bits_for_area[area_index] |= 2u;
    scrolling_finished_hook = 0;
    SpawnHardcodedPlm(&unk_A5EE45);
  }
}

void SporeSpawn_Func_7(uint16 a) {  // 0xA5EE4A
  R18_ = a;
  uint16 v1 = a;
  for (int i = 0; i != 32; i += 2) {
    palette_buffer[(i >> 1) + 144] = g_word_A5E379[v1 >> 1];
    v1 += 2;
  }
}
