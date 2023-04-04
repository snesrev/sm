// Enemy AI - inc. ki-hunter

#include "ida_types.h"
#include "variables.h"
#include "sm_rtl.h"
#include "funcs.h"
#include "enemy_types.h"


#define g_off_A890DA ((uint16*)RomFixedPtr(0xa890da))
#define g_word_A8A0A7 (*(uint16*)RomFixedPtr(0xa8a0a7))
#define g_word_A8A0A9 (*(uint16*)RomFixedPtr(0xa8a0a9))
#define g_word_A8A0AB (*(uint16*)RomFixedPtr(0xa8a0ab))
#define g_word_A8A0AD (*(uint16*)RomFixedPtr(0xa8a0ad))
#define g_word_A8A0B3 (*(uint16*)RomFixedPtr(0xa8a0b3))
#define g_word_A8A0B5 (*(uint16*)RomFixedPtr(0xa8a0b5))
#define g_word_A8A0B7 (*(uint16*)RomFixedPtr(0xa8a0b7))
#define g_word_A8A0B9 (*(uint16*)RomFixedPtr(0xa8a0b9))
#define g_word_A8A0BB (*(uint16*)RomFixedPtr(0xa8a0bb))
#define g_word_A8A0BD (*(uint16*)RomFixedPtr(0xa8a0bd))
#define g_word_A8A0C3 (*(uint16*)RomFixedPtr(0xa8a0c3))
#define g_word_A8A0C5 (*(uint16*)RomFixedPtr(0xa8a0c5))
#define g_off_A8A097 ((uint16*)RomFixedPtr(0xa8a097))
#define kNorfairLavaMan_Palette ((uint16*)RomFixedPtr(0xa8ac1c))
#define g_word_A8AF79 ((uint16*)RomFixedPtr(0xa8af79))
#define g_word_A8AF55 ((uint16*)RomFixedPtr(0xa8af55))
#define g_off_A8AF67 ((uint16*)RomFixedPtr(0xa8af67))
#define kBeetom_Ilist_B74E ((uint16*)RomFixedPtr(0xa8b74e))
#define g_off_A8C599 ((uint16*)RomFixedPtr(0xa8c599))
#define g_off_A8CC30 ((uint16*)RomFixedPtr(0xa8cc30))
#define g_off_A8E380 ((uint16*)RomFixedPtr(0xa8e380))
#define g_off_A8E682 ((uint16*)RomFixedPtr(0xa8e682))
#define g_off_A8E688 ((uint16*)RomFixedPtr(0xa8e688))
#define g_word_A8E7CC ((uint16*)RomFixedPtr(0xa8e7cc))
#define g_off_A8F3B0 ((uint16*)RomFixedPtr(0xa8f3b0))



static const int16 g_word_A890CA[4] = { -8, 8, 0, 0 };
static const int16 g_word_A890D2[4] = { 0, 0, -8, 8 };
static const uint16 g_word_A89050 = 0x80;
static const uint16 g_word_A89052 = 0xb0;
static const uint16 g_word_A89054 = 0x80;
static const uint16 g_word_A89056 = 0x80;

static const uint16 g_word_A89A9C = 0x10;
static const uint16 g_word_A89A9E = 0x40;
static const uint16 g_word_A89AA0 = 0x1800;
static const uint16 g_word_A89AA2 = 1;
static const uint16 g_word_A89AA4 = 0x78;
static const uint16 g_word_A89AA6 = 0x78;
static const uint16 g_word_A89AA8[18] = { 0xffc0, 0xffc0, 0, 0xffc0, 0x40, 0, 0xffc0, 0, 0, 0, 0x40, 0, 0xffc0, 0x40, 0, 0x40, 0x40, 0x40 };
static const uint16 g_word_A89ACC[17] = { 1, 8, 1, 8, 1, 7, 1, 7, 2, 6, 2, 6, 3, 5, 3, 5, 0xffff };
static const uint16 kWreckedShipGhost_Palette[16] = { 0x3800, 0x57ff, 0x42f7, 0x929, 0xa5, 0x4f5a, 0x36b5, 0x2610, 0x1dce, 0x1df, 0x1f, 0x18, 0xa, 0x6b9, 0xea, 0x45 };
static const uint16 g_word_A89D32 = 1;
static const uint16 g_word_A89D34 = 1;



static const int16 g_word_A8C277[3] = { -12, -16, -20 };
static const int16 g_word_A8C27D[3] = { -20, -16, -12 };
static const uint16 g_word_A8C19F = 0x40;
static const uint16 g_word_A8C1A1[12] = { 0, 1, 2, 3, 2, 1, 0, 0xffff, 0xfffe, 0xfffd, 0xfffe, 0xffff };
static const uint16 g_word_A8C1B9 = 0;
static const uint16 g_word_A8C1BB = 0;
static const uint16 g_word_A8C1BD = 0;
static const uint16 g_word_A8C1BF = 0;
static const uint16 g_word_A8C1C1 = 1;
static const uint16 g_word_A8C1C3 = 0;
static const uint16 g_word_A8C1C5 = 0xffff;
static const uint16 g_word_A8C1C7 = 0x8000;

static const int16 g_word_A8CCC1[31] = {
  0x1f, 0x18,  0xf,    8, 0x40,
  0x18,  0xf,    8, 0x1f, 0x10,
   0xf,    8, 0x1f, 0x18, 0x10,
     8, 0x1f, 0x18,  0xf, 0x40,
   0xf,    8, 0x1f, 0x18, 0x10,
  0x18,  0xf,    8, 0x1f, 0x10,
    -1,
};
static const int16 g_word_A8D871[10] = {
  0xc0, 0xe0,    0, 0x20, 0x40,
  0x40, 0x60, 0x80, 0xa0, 0xc0,
};
static const int16 g_word_A8D885[8] = {
  0x3ff, 0x4ff, 0x5ff, 0x6ff, 0x7ff,
  0x8ff, 0x9ff, 0xaff,
};
static const int16 g_word_A8D895[26] = {
    3,   1,   4,   1,   5,
    2,   6,   2,   7,   2,
    8,   3,   9,   3, 0xa,
    4, 0xb,   4, 0xc,   5,
  0xd,   5, 0xe,   6, 0xf,
    6,
};
static const uint16 g_word_A8DCC7 = 0x50;
static const uint16 g_word_A8DCCB = 0x70;
static const uint16 g_word_A8F180 = 0x60;
static const uint16 g_word_A8F182 = 0xe000;
static const uint16 g_word_A8F184 = 0;
static const uint8 g_byte_A8F186 = 0x30;

void Enemy_GrappleReact_NoInteract_A8(void) {  // 0xA88000
  SwitchEnemyAiToMainAi();
}

void Enemy_GrappleReact_KillEnemy_A8(void) {  // 0xA8800A
  EnemyGrappleDeath();
}

void Enemy_GrappleReact_CancelBeam_A8(void) {  // 0xA8800F
  Enemy_SwitchToFrozenAi();
}

void Enemy_GrappleReact_SamusLatchesNoInvinc_A8(void) {  // 0xA88014
  SamusLatchesOnWithGrappleNoInvinc();
}

void Enemy_GrappleReact_HurtSamus_A8(void) {  // 0xA8801E
  SamusHurtFromGrapple();
}

void Enemy_NormalTouchAI_A8(void) {  // 0xA88023
  NormalEnemyTouchAi();
}

void Enemy_NormalShotAI_A8(void) {  // 0xA8802D
  NormalEnemyShotAi();
}

void Enemy_NormalPowerBombAI_A8(void) {  // 0xA88037
  NormalEnemyPowerBombAi();
}

void Enemy_NormalFrozenAI_A8(void) {  // 0xA88041
  NormalEnemyFrozenAI();
}

const uint16 *MiniDraygon_Instr_2(uint16 k, const uint16 *jp) {  // 0xA8878F
  QueueSfx2_Max6(0x5E);
  return jp;
}

const uint16 *MiniDraygon_Instr_1(uint16 k, const uint16 *jp) {  // 0xA8879B
  if (gEnemySpawnData(cur_enemy_index)[30].cause_of_death == addr_kMiniDraygon_Ilist_870B)
    Get_MiniDraygon(cur_enemy_index)->mdn_var_F = -8;
  else
    Get_MiniDraygon(cur_enemy_index)->mdn_var_F = 8;
  return jp;
}

const uint16 *MiniDraygon_Instr_3(uint16 k, const uint16 *jp) {  // 0xA887B6
  Enemy_MiniDraygon *E = Get_MiniDraygon(cur_enemy_index);
  if (gEnemySpawnData(cur_enemy_index)[30].cause_of_death == addr_kMiniDraygon_Ilist_870B) {
    ++E->mdn_var_F;
  } else {
    --E->mdn_var_F;
  }
  return jp;
}

const uint16 *MiniDraygon_Instr_4(uint16 k, const uint16 *jp) {  // 0xA887CB
  Enemy_MiniDraygon *E = Get_MiniDraygon(cur_enemy_index);
  E->mdn_var_0C = 0;
  E->mdn_var_0B = 0;
  E->mdn_var_C = FUNC16(MiniDraygon_Func_8);
  return jp;
}

void MiniDraygon_Init(void) {  // 0xA887E0
  Enemy_MiniDraygon *E = Get_MiniDraygon(cur_enemy_index);
  if (E->mdn_parameter_1) {
    MiniDraygon_Func_2();
    E->base.layer = 4;
  } else {
    MiniDraygon_Func_1();
    uint16 v2 = 8 * LOBYTE(E->mdn_parameter_2);
    int v3 = v2 >> 1;
    E->mdn_var_04 = kCommonEnemySpeeds_Linear[v3];
    E->mdn_var_03 = kCommonEnemySpeeds_Linear[v3 + 1];
    E->mdn_var_06 = kCommonEnemySpeeds_Linear[v3 + 2];
    E->mdn_var_05 = kCommonEnemySpeeds_Linear[v3 + 3];
    E->mdn_var_E = HIBYTE(Get_MiniDraygon(v2)->mdn_parameter_2) >> 1;
  }
  E->mdn_var_00 = 0;
  E->mdn_var_01 = 0;
  E->mdn_var_C = FUNC16(MiniDraygon_Func_4);
}

void MiniDraygon_Func_1(void) {  // 0xA88838
  Get_MiniDraygon(cur_enemy_index)->mdn_var_B = (GetSamusEnemyDelta_X(cur_enemy_index) & 0x8000) == 0;
  Enemy_MiniDraygon *E = Get_MiniDraygon(cur_enemy_index);
  E->mdn_var_02 = addr_kMiniDraygon_Ilist_86A7;
  if (E->mdn_var_B)
    E->mdn_var_02 = addr_kMiniDraygon_Ilist_870B;
  MiniDraygon_Func_12();
}

void MiniDraygon_Func_2(void) {  // 0xA88866
  int v0 = cur_enemy_index >> 1;
  uint16 v1 = enemy_drawing_queue_sizes[v0 + 1];
  Enemy_MiniDraygon *E = Get_MiniDraygon(cur_enemy_index);
  E->mdn_var_B = v1;
  if (v1) {
    E->base.x_pos = enemy_drawing_queue[v0 + 91] + 4;
    E->base.y_pos = enemy_drawing_queue[v0 + 93] + 10;
    E->mdn_var_02 = addr_kMiniDraygon_Ilist_8727;
  } else {
    E->base.x_pos = enemy_drawing_queue[v0 + 91] - 4;
    E->base.y_pos = enemy_drawing_queue[v0 + 93] + 10;
    E->mdn_var_02 = addr_kMiniDraygon_Ilist_86C3;
  }
  MiniDraygon_Func_12();
}

void EvirProjectile_Init(void) {  // 0xA888B0
  Enemy_MiniDraygon *E = Get_MiniDraygon(cur_enemy_index);
  E->mdn_var_02 = addr_kMiniDraygon_Ilist_876F;
  MiniDraygon_Func_12();
  int v2 = cur_enemy_index >> 1;
  E->base.palette_index = enemy_drawing_queue[v2 + 73];
  E->base.vram_tiles_index = enemy_drawing_queue[v2 + 74];
  MiniDraygon_Func_3();
  E->mdn_var_01 = 0;
  E->mdn_var_0C = 0;
  E->mdn_var_0B = 0;
  E->mdn_var_F = 0;
  E->mdn_var_C = FUNC16(MiniDraygon_Func_8);
}

void MiniDraygon_Func_3(void) {  // 0xA888E5
  int v0 = cur_enemy_index >> 1;
  uint16 v1 = enemy_drawing_queue[v0 + 83];
  Enemy_MiniDraygon *E = Get_MiniDraygon(cur_enemy_index);
  E->mdn_var_B = v1;
  if (v1)
    E->base.x_pos = enemy_drawing_queue[v0 + 59] + 4;
  else
    E->base.x_pos = enemy_drawing_queue[v0 + 59] - 4;
  E->base.y_pos = enemy_drawing_queue[v0 + 61] + 18;
}


void CallMiniDraygonFunc(uint32 ea) {
  switch (ea) {
  case fnMiniDraygon_Func_4: MiniDraygon_Func_4(); return;
  case fnMiniDraygon_Func_8: MiniDraygon_Func_8(); return;
  case fnMiniDraygon_Func_9: MiniDraygon_Func_9(); return;
  case fnMiniDraygon_Func_10: MiniDraygon_Func_10(); return;
  default: Unreachable();
  }
}
void MiniDraygon_Main(void) {  // 0xA8891B
  Enemy_MiniDraygon *E = Get_MiniDraygon(cur_enemy_index);
  CallMiniDraygonFunc(E->mdn_var_C | 0xA80000);
}

void MiniDraygon_Func_4(void) {  // 0xA88922
  if (Get_MiniDraygon(cur_enemy_index)->mdn_parameter_1)
    MiniDraygon_Func_6();
  else
    MiniDraygon_Func_5();
}

void MiniDraygon_Func_5(void) {  // 0xA88933
  if (!Get_MiniDraygon(cur_enemy_index + 128)->mdn_var_0C)
    MiniDraygon_Func_1();
  Enemy_MiniDraygon *E = Get_MiniDraygon(cur_enemy_index);
  if (E->mdn_var_00) {
    AddToHiLo(&E->base.y_pos, &E->base.y_subpos, __PAIR32__(E->mdn_var_04, E->mdn_var_03));
  } else {
    AddToHiLo(&E->base.y_pos, &E->base.y_subpos, __PAIR32__(E->mdn_var_06, E->mdn_var_05));
  }
  if ((--E->mdn_var_E & 0x8000) != 0) {
    E->mdn_var_E = HIBYTE(E->mdn_parameter_2);
    E->mdn_var_00 ^= 1;
  }
}

void MiniDraygon_Func_6(void) {  // 0xA88997
  MiniDraygon_Func_2();
}


void EvirProjectile_Main(void) {  // 0xA8899E
  Enemy_MiniDraygon *E = Get_MiniDraygon(cur_enemy_index);
  if (!E->base.frozen_timer) {
    if (E->mdn_var_0B) {
      E->mdn_var_02 = addr_kMiniDraygon_Ilist_876F;
      MiniDraygon_Func_12();
    } else if (E->mdn_var_0C) {
      E->mdn_var_02 = addr_kMiniDraygon_Ilist_8775;
      MiniDraygon_Func_12();
    } else {
      MiniDraygon_Func_7();
    }
  }
  CallMiniDraygonFunc(E->mdn_var_C | 0xA80000);
}

void MiniDraygon_Func_7(void) {  // 0xA889D4
  if (IsSamusWithinEnemy_X(cur_enemy_index - 128, 0x80)) {
    uint16 r22 = -(uint8)(CalculateAngleOfSamusFromEnemy(cur_enemy_index) - 64);
    Enemy_MiniDraygon *E = Get_MiniDraygon(cur_enemy_index);
    SetHiLo(&E->mdn_var_07, &E->mdn_var_08, CosineMult8bitFull(r22, 4));
    SetHiLo(&E->mdn_var_09, &E->mdn_var_0A, SineMult8bitFull(r22, 4));
    E->mdn_var_02 = addr_kMiniDraygon_Ilist_876F;
    MiniDraygon_Func_12();
    E->mdn_var_0B = 1;
    E->mdn_var_C = FUNC16(MiniDraygon_Func_9);
  }
}

void MiniDraygon_Func_8(void) {  // 0xA88A34
  MiniDraygon_Func_3();
}

void MiniDraygon_Func_9(void) {  // 0xA88A3B
  MiniDraygon_Func_11();
  Enemy_MiniDraygon *E = Get_MiniDraygon(cur_enemy_index);
  AddToHiLo(&E->base.x_pos, &E->base.x_subpos, __PAIR32__(E->mdn_var_07, E->mdn_var_08));
  AddToHiLo(&E->base.y_pos, &E->base.y_subpos, __PAIR32__(E->mdn_var_09, E->mdn_var_0A));
}

void MiniDraygon_Func_10(void) {  // 0xA88A78
  if (!enemy_drawing_queue[(cur_enemy_index >> 1) + 77]) {
    Enemy_MiniDraygon *E = Get_MiniDraygon(cur_enemy_index);
    if (E->mdn_var_0C) {
      MiniDraygon_Func_3();
      E->base.x_pos += E->mdn_var_F;
    } else {
      E->mdn_var_02 = addr_kMiniDraygon_Ilist_876F;
      MiniDraygon_Func_12();
      E->mdn_var_0C = 0;
      E->mdn_var_0B = 0;
      E->mdn_var_C = FUNC16(MiniDraygon_Func_8);
    }
  }
}

void MiniDraygon_Func_11(void) {  // 0xA88AB1
  if (EnemyFunc_ADA3(0x100)) {
    if (!enemy_drawing_queue[(cur_enemy_index >> 1) + 77]) {
      Enemy_MiniDraygon *E = Get_MiniDraygon(cur_enemy_index);
      E->mdn_var_0B = 0;
      E->mdn_var_0C = 1;
      E->mdn_var_C = FUNC16(MiniDraygon_Func_10);
      E->mdn_var_0C = 1;
      E->mdn_var_02 = addr_kMiniDraygon_Ilist_8775;
      MiniDraygon_Func_12();
    }
  }
}

void MiniDraygon_Func_12(void) {  // 0xA88AE8
  Enemy_MiniDraygon *E = Get_MiniDraygon(cur_enemy_index);
  uint16 mdn_var_02 = E->mdn_var_02;
  if (mdn_var_02 != E->mdn_var_01) {
    E->base.current_instruction = mdn_var_02;
    E->mdn_var_01 = mdn_var_02;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

void MiniDraygon_Touch(void) {  // 0xA88B06
  Enemy_NormalTouchAI_A8();
  MiniDraygon_Func_13();
}

void MiniDraygon_Powerbomb(void) {  // 0xA88B0C
  Enemy_NormalPowerBombAI_A8();
  MiniDraygon_Func_13();
}

void MiniDraygon_Shot(void) {  // 0xA88B12
  NormalEnemyShotAi();
  MiniDraygon_Func_13();
}

void MiniDraygon_Func_13(void) {  // 0xA88B16
  Enemy_MiniDraygon *E = Get_MiniDraygon(cur_enemy_index);
  Enemy_MiniDraygon *E1 = Get_MiniDraygon(cur_enemy_index + 64);
  Enemy_MiniDraygon *E2 = Get_MiniDraygon(cur_enemy_index + 128);
  if (!E->base.health) {
    E1->base.properties |= 0x200;
    E2->base.properties |= 0x200;
  }
  uint16 frozen_timer = E->base.frozen_timer;
  if (frozen_timer) {
    E1->base.frozen_timer = frozen_timer;
    E1->base.ai_handler_bits |= 4;
    if (E2->mdn_var_C != FUNC16(MiniDraygon_Func_9)) {
      E2->base.ai_handler_bits |= 4;
      E2->base.frozen_timer = E->base.frozen_timer;
    }
  }
}

void MorphBallEye_Init(void) {  // 0xA89058
  Enemy_MorphBallEye *E = Get_MorphBallEye(cur_enemy_index);
  E->base.properties |= kEnemyProps_DisableSamusColl;
  E->base.spritemap_pointer = addr_kSpritemap_Nothing_A8;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  if ((E->mbee_parameter_2 & 0x8000) != 0) {
    int v1 = E->mbee_parameter_2 & 0xF;
    E->base.x_pos += g_word_A890CA[v1];
    E->base.y_pos += g_word_A890D2[v1];
    E->mbee_var_F = FUNC16(nullsub_244);
    E->base.current_instruction = g_off_A890DA[v1];
    for (int i = 510; i >= 0; i -= 2)
      *(uint16 *)((uint8 *)&g_word_7E9100 + (uint16)i) = 255;
  } else {
    E->mbee_var_F = FUNC16(MorphBallEye_Func_1);
    if ((E->mbee_parameter_1 & 1) != 0)
      E->base.current_instruction = addr_stru_A8900E;
    else
      E->base.current_instruction = addr_stru_A88FFC;
  }
}

void MorphBallEye_Main(void) {  // 0xA890E2
  if ((collected_items & 4) != 0) {
    Enemy_MorphBallEye *E = Get_MorphBallEye(cur_enemy_index);
    CallEnemyPreInstr(E->mbee_var_F | 0xA80000);
  }
}

void MorphBallEye_Func_1(uint16 k) {  // 0xA890F1
  if (IsSamusWithinEnemy_Y(k, g_word_A89054) && IsSamusWithinEnemy_X(k, g_word_A89050)) {
    Enemy_MorphBallEye *E = Get_MorphBallEye(k);
    E->mbee_var_E = 32;
    E->base.instruction_timer = 1;
    if ((E->mbee_parameter_1 & 1) != 0)
      E->base.current_instruction = addr_kMorphBallEye_Ilist_9026;
    else
      E->base.current_instruction = addr_kMorphBallEye_Ilist_9014;
    E->mbee_var_F = FUNC16(MorphBallEye_Func_2);
  }
}

void MorphBallEye_Func_2(uint16 k) {  // 0xA8912E
  Enemy_MorphBallEye *E = Get_MorphBallEye(k);
  bool v2 = E->mbee_var_E == 1;
  bool v3 = (--E->mbee_var_E & 0x8000) != 0;
  if (v2 || v3) {
    QueueSfx2_Max6(0x17);
    SpawnMorphBallEyeBeamHdma();
    E->mbee_var_F = FUNC16(MorphBallEye_Func_3);
    E->mbee_var_D = CalculateAngleFromXY(samus_x_pos - E->base.x_pos, samus_y_pos - E->base.y_pos);
  }
}

void MorphBallEye_Func_3(uint16 k) {  // 0xA89160
  Enemy_MorphBallEye *E = Get_MorphBallEye(k);
  if (IsSamusWithinEnemy_Y(k, g_word_A89056) && IsSamusWithinEnemy_X(k, g_word_A89052)) {
    uint16 v3 = CalculateAngleFromXY(samus_x_pos - E->base.x_pos, samus_y_pos - E->base.y_pos);
    E->mbee_var_D = v3;
    E->base.current_instruction = ((v3 & 0xF0) >> 2) - 28756;
  } else {
    QueueSfx2_Max6(0x71);
    E->mbee_var_C = 0;
    E->mbee_var_E = 32;
    if ((E->mbee_parameter_1 & 1) != 0)
      E->base.current_instruction = addr_kMorphBallEye_Ilist_9002;
    else
      E->base.current_instruction = addr_kMorphBallEye_Ilist_8FF0;
    E->mbee_var_F = FUNC16(MorphBallEye_Func_4);
  }
  E->base.instruction_timer = 1;
}

void MorphBallEye_Func_4(uint16 k) {  // 0xA891CE
  Enemy_MorphBallEye *E = Get_MorphBallEye(k);
  bool v2 = E->mbee_var_E == 1;
  bool v3 = (--E->mbee_var_E & 0x8000) != 0;
  if (v2 || v3)
    E->mbee_var_F = FUNC16(MorphBallEye_Func_1);
}

const uint16 *Fune_Instr_2(uint16 k, const uint16 *jp) {  // 0xA89625
  QueueSfx2_Max9(0x1F);
  return jp;
}

const uint16 *Fune_Instr_6(uint16 k, const uint16 *jp) {  // 0xA89631
  enemy_projectile_unk1995 = LOBYTE(Get_Fune(cur_enemy_index)->fune_parameter_2);
  SpawnEnemyProjectileWithGfx(0, cur_enemy_index, addr_stru_86DFBC);
  return jp;
}

const uint16 *Fune_Instr_7(uint16 k, const uint16 *jp) {  // 0xA8964A
  enemy_projectile_unk1995 = LOBYTE(Get_Fune(cur_enemy_index)->fune_parameter_2);
  SpawnEnemyProjectileWithGfx(1, cur_enemy_index, addr_stru_86DFBC);
  return jp;
}

const uint16 *Fune_Instr_1(uint16 k, const uint16 *jp) {  // 0xA89663
  enemy_projectile_unk1995 = LOBYTE(Get_Fune(cur_enemy_index)->fune_parameter_2);
  SpawnEnemyProjectileWithGfx(0, cur_enemy_index, addr_stru_86DFCA);
  return jp;
}

const uint16 *Fune_Instr_4(uint16 k, const uint16 *jp) {  // 0xA8967C
  enemy_projectile_unk1995 = LOBYTE(Get_Fune(cur_enemy_index)->fune_parameter_2);
  SpawnEnemyProjectileWithGfx(1, cur_enemy_index, addr_stru_86DFCA);
  return jp;
}

const uint16 *Fune_Instr_3(uint16 k, const uint16 *jp) {  // 0xA89695
  Enemy_Fune *E = Get_Fune(cur_enemy_index);
  E->fune_var_A += 4;
  E->fune_var_B = FUNC16(Fune_Func_1);
  if (E->fune_var_D)
    E->fune_var_B = FUNC16(Fune_Func_2);
  return jp;
}

const uint16 *Fune_Instr_5(uint16 k, const uint16 *jp) {  // 0xA896B4
  Enemy_Fune *E = Get_Fune(cur_enemy_index);
  E->fune_var_A += 4;
  E->fune_var_B = FUNC16(Fune_Func_1);
  if (E->fune_var_D)
    E->fune_var_B = FUNC16(Fune_Func_2);
  return jp;
}

void Fune_Init(void) {  // 0xA896E3
  Enemy_Fune *E = Get_Fune(cur_enemy_index);
  E->fune_var_A = addr_off_A896D7;
  E->fune_var_B = FUNC16(Fune_Func_1);
  uint16 v1 = E->fune_parameter_1 & 0xF;
  E->fune_var_D = v1;
  if (v1) {
    E->fune_var_A = addr_off_A896DF;
    E->fune_var_B = FUNC16(Fune_Func_2);
  }
  if ((E->fune_parameter_1 & 0xF0) != 0)
    E->fune_var_A += 2;
  Fune_Func_4();
  E->fune_var_C = HIBYTE(E->fune_parameter_2);
  E->fune_var_F = HIBYTE(E->fune_parameter_1);
  E->fune_var_E = 0;
}

void CallFuneFunc(uint32 ea) {
  switch (ea) {
  case fnFune_Func_1: Fune_Func_1(); return;  // 0xa89737
  case fnFune_Func_2: Fune_Func_2(); return;  // 0xa8975c
  case fnnullsub_247: return;  // 0xa8978e
  case fnnullsub_248: return;  // 0xa8978f
  default: Unreachable();
  }
}

void Fune_Main(void) {  // 0xA89730
  Enemy_Fune *E = Get_Fune(cur_enemy_index);
  CallFuneFunc(E->fune_var_B | 0xA80000);
}

void Fune_Func_1(void) {  // 0xA89737
  Enemy_Fune *E = Get_Fune(cur_enemy_index);
  if ((int16)(++E->fune_var_E - E->fune_var_F) >= 0) {
    E->fune_var_A -= 4;
    Fune_Func_4();
    E->fune_var_B = FUNC16(nullsub_247);
    E->fune_var_E = 0;
  }
}

void Fune_Func_2(void) {  // 0xA8975C
  Enemy_Fune *E = Get_Fune(cur_enemy_index);
  E->fune_var_00 = E->fune_var_A;
  if (Fune_Func_3()) {
    uint16 fune_var_00 = E->fune_var_00;
    E->fune_var_A = fune_var_00;
    fune_var_00 -= 4;
    E->fune_var_00 = fune_var_00;
    E->fune_var_A = fune_var_00;
    Fune_Func_4();
    E->fune_var_B = FUNC16(nullsub_248);
  }
  E->fune_var_A = E->fune_var_00;
}

uint16 Fune_Func_3(void) {  // 0xA89790
  Enemy_Fune *E = Get_Fune(cur_enemy_index);
  return IsSamusWithinEnemy_Y(cur_enemy_index, E->fune_var_C);
}

void Fune_Func_4(void) {  // 0xA8979B
  Enemy_Fune *E = Get_Fune(cur_enemy_index);
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->base.current_instruction = *(uint16 *)RomPtr_A8(E->fune_var_A);
}

void WreckedShipGhost_Init(void) {  // 0xA89AEE
  Enemy_WreckedShipGhost *E = Get_WreckedShipGhost(cur_enemy_index);
  E->base.properties |= kEnemyProps_DisableSamusColl | kEnemyProps_Tangible | kEnemyProps_Invisible;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->base.current_instruction = addr_kWreckedShipGhost_Ilist_9A8C;
  E->wsgt_var_A = FUNC16(WreckedShipGhost_Func_6);
  E->wsgt_var_B = g_word_A89AA4 + 160;

  uint16 v2 = swap16(E->base.palette_index);
  uint16 v3 = 16 * v2 + 256;
  int n = 16;
  do {
    target_palettes[v3 >> 1] = 0;
    v3 += 2;
  } while (--n >= 0);
}

void CallWreckedShipGhost(uint32 ea, uint16 k) {
  switch (ea) {
  case fnWreckedShipGhost_Func_1: WreckedShipGhost_Func_1(k); return;
  case fnWreckedShipGhost_Func_2: WreckedShipGhost_Func_2(k); return;
  case fnWreckedShipGhost_Func_4: WreckedShipGhost_Func_4(k); return;
  case fnWreckedShipGhost_Func_5: WreckedShipGhost_Func_5(k); return;
  case fnWreckedShipGhost_Func_6: WreckedShipGhost_Func_6(k); return;
  case fnWreckedShipGhost_Func_7: WreckedShipGhost_Func_7(k); return;
  default: Unreachable();
  }
}

void WreckedShipGhost_Main(void) {  // 0xA89B3C
  Enemy_WreckedShipGhost *E = Get_WreckedShipGhost(cur_enemy_index);
  CallWreckedShipGhost(E->wsgt_var_A | 0xA80000, cur_enemy_index);
}

void WreckedShipGhost_Func_1(uint16 k) {  // 0xA89B42
  WreckedShipGhost_Func_3(k);
  Enemy_WreckedShipGhost *E = Get_WreckedShipGhost(cur_enemy_index);
  uint16 v3 = swap16(E->base.palette_index);
  uint16 v4 = 16 * v3 + 256;
  int n = 16, v1 = 16;
  do {
    int v5 = v4 >> 1;
    if (sign16((palette_buffer[v5] & 0x1F) - 31)) {
      palette_buffer[v5] += 1057;
      --v1;
    }
    v4 += 2;
  } while (--n);
  if ((int16)(v1 - 16) >= 0) {
    E->wsgt_var_A = FUNC16(WreckedShipGhost_Func_2);
    uint16 v7 = swap16(E->base.palette_index);
    uint16 v8 = 16 * v7 + 256;
    uint16 v9 = 0;
    do {
      target_palettes[v8 >> 1] = kWreckedShipGhost_Palette[v9 >> 1];
      v8 += 2;
      v9 += 2;
    } while ((int16)(v9 - 32) < 0);
  }
}

void WreckedShipGhost_Func_2(uint16 k) {  // 0xA89BAD
  uint16 v4 = WreckedShipGhost_Func_8();
  WreckedShipGhost_Func_3(cur_enemy_index);
  Enemy_WreckedShipGhost *E = Get_WreckedShipGhost(cur_enemy_index);
  if (!(E->wsgt_var_B | v4)) {
    E->base.properties &= 0xFAFF;
    E->wsgt_var_A = FUNC16(WreckedShipGhost_Func_5);
    E->wsgt_var_00 = E->base.y_pos;
    E->wsgt_var_B = 1;
    E->wsgt_var_C = 2;
    E->wsgt_var_01 = 0;
    E->wsgt_var_02 = g_word_A89AA2;
    E->wsgt_var_B = g_word_A89AA6;
    E->wsgt_var_05 = 4;
    uint16 v2 = samus_x_pos;
    E->wsgt_var_06 = samus_x_pos;
    E->wsgt_var_07 = v2;
    E->wsgt_var_08 = v2;
    E->wsgt_var_09 = 12;
    uint16 v3 = *(uint16 *)((uint8 *)&samus_y_pos + cur_enemy_index);
    E->wsgt_var_0A = v3;
    E->wsgt_var_0B = v3;
    E->wsgt_var_0C = v3;
    E->wsgt_var_0D = g_word_A89A9E;
    E->wsgt_var_0E = g_word_A89A9C;
  }
}

void WreckedShipGhost_Func_3(uint16 k) {  // 0xA89C31
  int16 v5;

  Enemy_WreckedShipGhost *E = Get_WreckedShipGhost(k);
  uint16 wsgt_var_B = E->wsgt_var_B;
  if (!wsgt_var_B)
    goto LABEL_6;
  uint16 v3;
  v3 = wsgt_var_B - 1;
  E->wsgt_var_B = v3;
  if (!v3) {
    uint16 wsgt_var_C;
    wsgt_var_C = E->wsgt_var_C;
    v5 = g_word_A89ACC[wsgt_var_C >> 1];
    if (v5 < 0) {
      E->wsgt_var_B = 0;
      E->wsgt_var_C = 0;
      return;
    }
    E->wsgt_var_B = v5;
    E->wsgt_var_C = wsgt_var_C + 2;
    if ((wsgt_var_C & 2) == 0) {
LABEL_6:
      E->base.properties &= ~kEnemyProps_Invisible;
    }
  }
}

void WreckedShipGhost_Func_4(uint16 k) {  // 0xA89C69
  if (!WreckedShipGhost_Func_8()) {
    Enemy_WreckedShipGhost *E = Get_WreckedShipGhost(cur_enemy_index);
    E->wsgt_var_A = FUNC16(WreckedShipGhost_Func_6);
    E->base.properties |= kEnemyProps_Invisible;
    E->wsgt_var_B = g_word_A89AA4;
  }
}

void WreckedShipGhost_Func_5(uint16 k) {  // 0xA89C8A
  Enemy_WreckedShipGhost *E = Get_WreckedShipGhost(k);
  AddToHiLo(&E->base.y_pos, &E->base.y_subpos, __PAIR32__(E->wsgt_var_02, E->wsgt_var_01));
  AddToHiLo(&E->wsgt_var_02, &E->wsgt_var_01, ((int16)(E->base.y_pos - E->wsgt_var_00) < 0) ? g_word_A89AA0 : -g_word_A89AA0);
  if (--E->wsgt_var_B == 0) {
    E->wsgt_var_A = FUNC16(WreckedShipGhost_Func_4);
    E->base.properties |= kEnemyProps_Tangible;
    // todo: this must be swap16 it seems
    int t = swap16(E->base.palette_index);
    uint16 v9 = 16 * t + 256;
    int n = 16;
    do {
      target_palettes[v9 >> 1] = 0x7FFF;
      v9 += 2;
    } while ((--n & 0x8000) == 0);
  }
}

void WreckedShipGhost_Func_6(uint16 k) {  // 0xA89D13
  uint16 v3;

  Enemy_WreckedShipGhost *E = Get_WreckedShipGhost(k);
  uint16 wsgt_var_B = E->wsgt_var_B;
  if (!wsgt_var_B || (v3 = wsgt_var_B - 1, (E->wsgt_var_B = v3) == 0)) {
    E->wsgt_var_B = 1;
    E->wsgt_var_C = 2;
    E->wsgt_var_A = FUNC16(WreckedShipGhost_Func_7);
  }
}

void WreckedShipGhost_Func_7(uint16 k) {  // 0xA89D36
  int16 v2;
  int16 v4;
  int16 v7;
  int16 v9;

  Enemy_WreckedShipGhost *E = Get_WreckedShipGhost(k);
  if ((int16)(samus_x_pos - E->wsgt_var_07) < 0
      || (int16)(samus_x_pos - E->wsgt_var_08) >= 0
      || (v2 = *(uint16 *)((uint8 *)&samus_y_pos + k), (int16)(v2 - E->wsgt_var_0B) < 0)
      || (int16)(v2 - E->wsgt_var_0C) >= 0) {
    E->wsgt_var_0D = g_word_A89A9E;
    v4 = 0;
    if ((int16)(samus_x_pos - E->wsgt_var_06) >= 0) {
      v4 = (samus_x_pos == E->wsgt_var_06) ? 4 : 8;
    }
    if (v4 == E->wsgt_var_05) {
      v7 = 0;
      v9 = samus_y_pos - E->wsgt_var_0A;
      if (v9 >= 0) {
        v7 = v9 ? 24 : 12;
      }
      if (v7 == E->wsgt_var_09) {
        uint16 v10 = E->wsgt_var_0E - 1;
        E->wsgt_var_0E = v10;
        if (!v10)
          goto LABEL_13;
      } else {
        E->wsgt_var_09 = v7;
        E->wsgt_var_0E = g_word_A89A9C;
      }
    } else {
      E->wsgt_var_05 = v4;
      E->wsgt_var_0E = g_word_A89A9C;
    }
  } else {
    uint16 v3;
    v3 = E->wsgt_var_0D - 1;
    E->wsgt_var_0D = v3;
    if (!v3) {
      E->wsgt_var_05 = 4;
      E->wsgt_var_09 = 12;
LABEL_13:
      E->wsgt_var_A = FUNC16(WreckedShipGhost_Func_1);
      E->wsgt_var_0D = g_word_A89A9E;
      E->wsgt_var_0E = g_word_A89A9C;
      int v6 = (uint16)(E->wsgt_var_09 + E->wsgt_var_05) >> 1;
      E->base.x_pos = g_word_A89AA8[v6] + *(uint16 *)((uint8 *)&samus_x_pos + k);
      E->base.y_pos = g_word_A89AA8[v6 + 1] + *(uint16 *)((uint8 *)&samus_y_pos + k);
      return;
    }
  }
  uint16 v11 = samus_x_pos;
  E->wsgt_var_06 = samus_x_pos;
  E->wsgt_var_07 = v11 - g_word_A89D32;
  E->wsgt_var_08 = g_word_A89D32 + samus_x_pos;
  uint16 v13 = samus_y_pos;
  E->wsgt_var_0A = samus_y_pos;
  E->wsgt_var_0B = v13 - g_word_A89D34;
  E->wsgt_var_0C = g_word_A89D34 + samus_y_pos;
}

uint16 WreckedShipGhost_Func_8(void) {  // 0xA89E88
  uint16 v0 = 0;
  if (!door_transition_flag_enemies) {
    Enemy_WreckedShipGhost *E = Get_WreckedShipGhost(cur_enemy_index);
    uint16 v2 = swap16(E->base.palette_index);
    uint16 v3 = 16 * v2 + 256;
    uint16 r20 = 16 * v2 + 288;
    do {
      int v4 = v3 >> 1;
      if (target_palettes[v4] != palette_buffer[v4]) {
        uint16 r18 = target_palettes[v4] & 0x1F;
        uint16 v5 = palette_buffer[v4] & 0x1F;
        if (v5 != r18) {
          uint16 v6;
          if ((int16)(v5 - r18) >= 0)
            v6 = v5 - 1;
          else
            v6 = v5 + 1;
          r18 = v6;
          palette_buffer[v4] = v6 | palette_buffer[v4] & 0xFFE0;
          ++v0;
        }
        r18 = target_palettes[v4] & 0x3E0;
        uint16 v7 = palette_buffer[v4] & 0x3E0;
        if (v7 != r18) {
          uint16 v8;
          if ((int16)(v7 - r18) >= 0)
            v8 = v7 - 32;
          else
            v8 = v7 + 32;
          palette_buffer[v4] = v8 | palette_buffer[v4] & 0xFC1F;
          ++v0;
        }
        r18 = target_palettes[v4] & 0x7C00;
        uint16 v9 = palette_buffer[v4] & 0x7C00;
        if (v9 != r18) {
          uint16 v10;
          if ((int16)(v9 - r18) >= 0)
            v10 = v9 - 1024;
          else
            v10 = v9 + 1024;
          r18 = v10;
          palette_buffer[v4] = v10 | palette_buffer[v4] & 0x83FF;
          ++v0;
        }
      }
      v3 += 2;
    } while ((int16)(v3 - r20) < 0);
  }
  return v0;
}

const uint16 *YappingMaw_Instr_2(uint16 k, const uint16 *jp) {  // 0xA8A0C7
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  E->ymw_var_32 = g_word_A8A0AB;
  E->ymw_var_33 = g_word_A8A0AD;
  return jp;
}

const uint16 *YappingMaw_Instr_4(uint16 k, const uint16 *jp) {  // 0xA8A0D9
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  E->ymw_var_32 = g_word_A8A0C3;
  E->ymw_var_33 = g_word_A8A0C5;
  return jp;
}

const uint16 *YappingMaw_Instr_5(uint16 k, const uint16 *jp) {  // 0xA8A0EB
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  E->ymw_var_32 = g_word_A8A0B3;
  E->ymw_var_33 = g_word_A8A0B5;
  return jp;
}

const uint16 *YappingMaw_Instr_7(uint16 k, const uint16 *jp) {  // 0xA8A0FD
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  E->ymw_var_32 = g_word_A8A0BB;
  E->ymw_var_33 = g_word_A8A0BD;
  return jp;
}

const uint16 *YappingMaw_Instr_3(uint16 k, const uint16 *jp) {  // 0xA8A10F
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  E->ymw_var_32 = g_word_A8A0A7;
  E->ymw_var_33 = g_word_A8A0A9;
  return jp;
}

const uint16 *YappingMaw_Instr_6(uint16 k, const uint16 *jp) {  // 0xA8A121
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  E->ymw_var_32 = g_word_A8A0B7;
  E->ymw_var_33 = g_word_A8A0B9;
  return jp;
}

const uint16 *YappingMaw_Instr_1(uint16 k, const uint16 *jp) {  // 0xA8A133
  if (!Get_YappingMaw(cur_enemy_index)->ymw_var_36)
    QueueSfx2_Max6(0x2F);
  return jp;
}

void YappingMaw_Init(void) {  // 0xA8A148
  int16 v1;

  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  E->ymw_var_0C = E->base.x_pos;
  E->ymw_var_0D = E->base.y_pos;
  E->ymw_var_00 = 0;
  E->ymw_var_01 = 0;
  E->ymw_var_02 = 0;
  E->ymw_var_03 = 0;
  E->ymw_var_04 = 0;
  E->ymw_var_05 = 0;
  E->ymw_var_06 = 0;
  E->ymw_var_07 = 0;
  E->ymw_var_30 = 0;
  E->ymw_var_F = E->ymw_parameter_1;
  E->ymw_var_E = 64;
  E->base.current_instruction = addr_kYappingMaw_Ilist_9F6F;
  uint16 R36 = 57;
  uint16 R34 = 8;
  if (!E->ymw_parameter_2) {
    E->base.current_instruction = addr_kYappingMaw_Ilist_9FC7;
    R36 = 56;
    R34 = -8;
  }
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->ymw_var_A = FUNC16(YappingMaw_Func_1);
  E->ymw_var_45 = E->base.palette_index & 0xE00;
  v1 = 3;
  E->ymw_var_44 = 3;
  do {
    SpawnEnemyProjectileWithGfx(v1, cur_enemy_index, addr_kEproj_YappingMawsBody);
    v1 = E->ymw_var_44 - 1;
    E->ymw_var_44 = v1;
  } while (v1 >= 0);
  uint16 v3 = E->base.vram_tiles_index | E->base.palette_index;
  E->ymw_var_47 = v3;
  E->ymw_var_46 = CreateSpriteAtPos(E->base.x_pos, R34 + E->base.y_pos, R36, v3);
}

void CallYappingMawFunc(uint32 ea) {
  switch (ea) {
  case fnYappingMaw_Func_1: YappingMaw_Func_1(); return;  // 0xa8a235
  case fnYappingMaw_Func_2: YappingMaw_Func_2(); return;  // 0xa8a28c
  case fnYappingMaw_Func_8: YappingMaw_Func_8(); return;  // 0xa8a445
  case fnYappingMaw_Func_11: YappingMaw_Func_11(); return;  // 0xa8a68a
  default: Unreachable();
  }
}

void YappingMaw_Main(void) {  // 0xA8A211
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  --E->ymw_var_35;
  E->ymw_var_36 = CheckIfEnemyIsOnScreen();
  CallYappingMawFunc(E->ymw_var_A | 0xA80000);
  YappingMaw_Func_15();
  YappingMaw_Func_14();
  YappingMaw_Func_13();
  YappingMaw_Func_12();
}

void YappingMaw_Func_1(void) {  // 0xA8A235
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  PairU16 pair = EnemyFunc_ACA8((Point16U) { E->base.x_pos, E->base.y_pos}, (Point16U) { samus_x_pos, samus_y_pos });
  uint16 v2 = Abs16(pair.k);
  E->ymw_var_08 = v2;
  if (sign16(v2 - 32)) {
    E->ymw_var_35 = 48;
  } else if ((int16)(v2 - E->ymw_var_F) < 0) {
    if (!sign16(E->ymw_var_08 - 64))
      E->ymw_var_08 = 64;
    E->ymw_var_0A = pair.j;
    E->ymw_var_A = FUNC16(YappingMaw_Func_2);
  }
}

void YappingMaw_Func_2(void) {  // 0xA8A28C
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  E->ymw_var_B = 0;
  E->ymw_var_C = 0;
  E->ymw_var_D = 0;
  E->ymw_var_09 = E->ymw_var_08 >> 1;
  uint16 v1 = (uint8)(64 - E->ymw_var_0A);
  E->ymw_var_0B = v1;
  if (sign16(v1 - 128))
    E->ymw_var_2F = 0;
  else
    E->ymw_var_2F = 1;
  YappingMaw_Func_3();
  uint16 v2 = 2 * ((uint8)(E->ymw_var_0A + 16) >> 5);
  E->ymw_var_34 = v2;
  E->base.current_instruction = g_off_A8A097[v2 >> 1];
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->ymw_var_32 = *(uint16 *)((uint8 *)&g_word_A8A0A7 + (uint16)(2 * v2));
  E->ymw_var_33 = *(uint16 *)((uint8 *)&g_word_A8A0A9 + (uint16)(2 * v2));
  E->ymw_var_A = FUNC16(YappingMaw_Func_8);
}

void YappingMaw_Func_3(void) {  // 0xA8A310
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  E->ymw_var_0E = YappingMaw_Func_16(0x80, E->ymw_var_09);
  E->ymw_var_0F = YappingMaw_Func_17(0x80, E->ymw_var_09 >> 1);
}

void YappingMaw_Func_4(void) {  // 0xA8A339
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  uint16 varE32 = E->ymw_var_00;
  E->ymw_var_10 = YappingMaw_Func_16(0, varE32);
  E->ymw_var_11 = YappingMaw_Func_17(0, varE32);
  E->ymw_var_27 = YappingMaw_Func_16(E->ymw_var_0B, varE32) - E->ymw_var_10;
  E->ymw_var_28 = YappingMaw_Func_17(E->ymw_var_0B, varE32) - E->ymw_var_11;
}

void YappingMaw_Func_5(void) {  // 0xA8A37C
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  uint16 varE32 = E->ymw_var_02;
  E->ymw_var_10 = YappingMaw_Func_16(0, varE32);
  E->ymw_var_11 = YappingMaw_Func_17(0, varE32);
  E->ymw_var_29 = YappingMaw_Func_16(E->ymw_var_0B, varE32) - E->ymw_var_10;
  E->ymw_var_2A = YappingMaw_Func_17(E->ymw_var_0B, varE32) - E->ymw_var_11;
}

void YappingMaw_Func_6(void) {  // 0xA8A3BF
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  uint16 varE32 = E->ymw_var_04;
  E->ymw_var_10 = YappingMaw_Func_16(0, varE32);
  E->ymw_var_11 = YappingMaw_Func_17(0, varE32);
  E->ymw_var_2B = YappingMaw_Func_16(E->ymw_var_0B, varE32) - E->ymw_var_10;
  E->ymw_var_2C = YappingMaw_Func_17(E->ymw_var_0B, varE32) - E->ymw_var_11;
}

void YappingMaw_Func_7(void) {  // 0xA8A402
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  uint16 varE32 = E->ymw_var_06;
  E->ymw_var_10 = YappingMaw_Func_16(0, varE32);
  E->ymw_var_11 = YappingMaw_Func_17(0, varE32);
  E->ymw_var_2D = YappingMaw_Func_16(E->ymw_var_0B, varE32) - E->ymw_var_10;
  E->ymw_var_2E = YappingMaw_Func_17(E->ymw_var_0B, varE32) - E->ymw_var_11;
}

void YappingMaw_Func_8(void) {  // 0xA8A445
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  uint16 r20 = E->ymw_var_B >> 2;
  if (E->ymw_var_2F) {
    E->ymw_var_21 = 128 + r20;
    E->ymw_var_22 = 128 + r20 * 2;
    E->ymw_var_23 = 128 + r20 * 3;
    E->ymw_var_24 = 128 + r20 * 4;
  } else {
    E->ymw_var_21 = 128 - r20;
    E->ymw_var_22 = 128 - r20 * 2;
    E->ymw_var_23 = 128 - r20 * 3;
    E->ymw_var_24 = 128 - r20 * 4;
  }
  uint16 varE32 = E->ymw_var_09;
  E->ymw_var_00 = YappingMaw_Func_16(E->ymw_var_21, varE32) - E->ymw_var_0E;
  E->ymw_var_02 = YappingMaw_Func_16(E->ymw_var_22, varE32) - E->ymw_var_0E;
  E->ymw_var_04 = YappingMaw_Func_16(E->ymw_var_23, varE32) - E->ymw_var_0E;
  E->ymw_var_06 = YappingMaw_Func_16(E->ymw_var_24, varE32) - E->ymw_var_0E;
  varE32 = E->ymw_var_09 >> 1;
  E->ymw_var_01 = YappingMaw_Func_17(E->ymw_var_21, varE32) - E->ymw_var_0F;
  E->ymw_var_03 = YappingMaw_Func_17(E->ymw_var_22, varE32) - E->ymw_var_0F;
  E->ymw_var_05 = YappingMaw_Func_17(E->ymw_var_23, varE32) - E->ymw_var_0F;
  E->ymw_var_07 = YappingMaw_Func_17(E->ymw_var_24, varE32) - E->ymw_var_0F;
  YappingMaw_Func_4();
  YappingMaw_Func_5();
  YappingMaw_Func_6();
  YappingMaw_Func_7();
  E->ymw_var_00 += E->ymw_var_27;
  E->ymw_var_01 += E->ymw_var_28;
  E->ymw_var_02 += E->ymw_var_29;
  E->ymw_var_03 += E->ymw_var_2A;
  E->ymw_var_04 += E->ymw_var_2B;
  E->ymw_var_05 += E->ymw_var_2C;
  E->ymw_var_06 += E->ymw_var_2D;
  E->ymw_var_07 += E->ymw_var_2E;
  E->base.x_pos = E->ymw_var_06 + E->ymw_var_0C;
  E->base.y_pos = E->ymw_var_07 + E->ymw_var_0D;
  YappingMaw_Func_9(cur_enemy_index);
  if (!sign16(E->ymw_var_B)) {
    if (!sign16(E->ymw_var_B - 128)) {
      E->ymw_var_B = 128;
      E->ymw_var_C = 0;
      E->ymw_var_D += 4;
    }
LABEL_19:
    if (E->ymw_var_30)
      YappingMaw_Func_10();
    return;
  }
  E->ymw_var_A = FUNC16(YappingMaw_Func_11);
  E->ymw_var_35 = 48;
  if (E->ymw_var_34 == 4) {
    E->base.current_instruction = addr_kYappingMaw_Ilist_A01F;
  } else if (E->ymw_var_34 == 12) {
    E->base.current_instruction = addr_kYappingMaw_Ilist_A03D;
  } else {
    E->base.current_instruction = addr_off_A8A025;
  }
  if (!E->ymw_parameter_2) {
    if (E->ymw_var_34 == 4) {
      E->base.current_instruction = addr_kYappingMaw_Ilist_A05B;
    } else {
      if (E->ymw_var_34 != 12) {
        E->base.current_instruction = addr_off_A8A061;
        return;
      }
      E->base.current_instruction = addr_kYappingMaw_Ilist_A079;
    }
    goto LABEL_19;
  }
}

void YappingMaw_Func_9(uint16 k) {  // 0xA8A63E
  Enemy_YappingMaw *E = Get_YappingMaw(k);
  int v2 = E->ymw_var_D >> 1;
  AddToHiLo(&E->ymw_var_B, &E->ymw_var_C, __PAIR32__(kCommonEnemySpeeds_Quadratic[v2 + 1], kCommonEnemySpeeds_Quadratic[v2]));
  E->ymw_var_D += 8;
}

void YappingMaw_Func_10(void) {  // 0xA8A665
  CallSomeSamusCode(3);
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  samus_x_pos = E->ymw_var_32 + E->base.x_pos;
  samus_y_pos = E->ymw_var_33 + E->base.y_pos;
  EnemyFunc_B7A1();
}

void YappingMaw_Func_11(void) {  // 0xA8A68A
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  if (E->ymw_var_30)
    YappingMaw_Func_10();
  if ((--E->ymw_var_E & 0x8000) != 0 && samus_input_handler != FUNC16(Samus_InputHandler_E91D)) {
    samus_input_handler = FUNC16(Samus_InputHandler_E913);
    E->ymw_var_30 = 0;
    E->ymw_var_35 = 48;
    E->ymw_var_E = 64;
    E->ymw_var_A = FUNC16(YappingMaw_Func_1);
  }
}

void YappingMaw_Func_12(void) {  // 0xA8A6C4
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  int v1 = E->ymw_var_40 >> 1;
  enemy_projectile_x_pos[v1] = E->ymw_var_0C;
  enemy_projectile_y_pos[v1] = E->ymw_var_0D;
}

void YappingMaw_Func_13(void) {  // 0xA8A6DB
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  int v1 = E->ymw_var_41 >> 1;
  enemy_projectile_x_pos[v1] = E->ymw_var_00 + E->ymw_var_0C;
  enemy_projectile_y_pos[v1] = E->ymw_var_01 + E->ymw_var_0D;
}

void YappingMaw_Func_14(void) {  // 0xA8A6FC
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  int v1 = E->ymw_var_42 >> 1;
  enemy_projectile_x_pos[v1] = E->ymw_var_02 + E->ymw_var_0C;
  enemy_projectile_y_pos[v1] = E->ymw_var_03 + E->ymw_var_0D;
}

void YappingMaw_Func_15(void) {  // 0xA8A71D
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  int v1 = E->ymw_var_43 >> 1;
  enemy_projectile_x_pos[v1] = E->ymw_var_04 + E->ymw_var_0C;
  enemy_projectile_y_pos[v1] = E->ymw_var_05 + E->ymw_var_0D;
}

uint16 YappingMaw_Func_16(uint16 a, uint16 varE32) {  // 0xA8A73E
  return YappingMaw_Func_17(a - 64, varE32);
}

uint16 YappingMaw_Func_17(uint16 a, uint16 varE32) {  // 0xA8A742
  int16 v1;
  uint16 r20 = 0;

  uint16 r22 = 0;
  v1 = kSine16bit[(uint8)-a];
  if (v1 < 0) {
    v1 = -v1;
    ++r20;
  }
  WriteReg(WRMPYA, HIBYTE(v1));
  if ((uint8)varE32) {
    uint16 prod = Mult8x8(HIBYTE(v1), varE32);
    r22 = 2 * ((prod & 0xFF00) >> 8);
    if (r20)
      r22 = (2 * ((uint16)-prod >> 8)) | 0xFF00;
  }
  return r22;
}

void YappingMaw_Touch(void) {  // 0xA8A799
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  if ((E->ymw_var_35 & 0x8000) != 0 && !E->ymw_var_30) {
    E->ymw_var_35 = 0;
    E->ymw_var_30 = 1;
    samus_input_handler = FUNC16(nullsub_152);
  }
}

void YappingMaw_Shot(void) {  // 0xA8A7BD
  NormalEnemyShotAi();
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  if (E->base.health) {
    if (E->base.frozen_timer) {
      if (samus_input_handler != FUNC16(Samus_InputHandler_E91D))
        samus_input_handler = FUNC16(Samus_InputHandler_E913);
      E->ymw_var_30 = 0;
    }
  } else {
    *(uint16 *)((uint8 *)enemy_projectile_id + E->ymw_var_40) = 0;
    *(uint16 *)((uint8 *)enemy_projectile_id + E->ymw_var_41) = 0;
    *(uint16 *)((uint8 *)enemy_projectile_id + E->ymw_var_42) = 0;
    *(uint16 *)((uint8 *)enemy_projectile_id + E->ymw_var_43) = 0;
    sprite_instr_list_ptrs[E->ymw_var_46 >> 1] = 0;
    if (samus_input_handler != FUNC16(Samus_InputHandler_E91D))
      samus_input_handler = FUNC16(Samus_InputHandler_E913);
    Get_YappingMaw(cur_enemy_index)->ymw_var_30 = 0;
  }
}

void YappingMaw_Frozen(void) {  // 0xA8A835
  Enemy_NormalFrozenAI_A8();
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  YappingMaw_Func_18(E->ymw_var_40);
  YappingMaw_Func_18(E->ymw_var_41);
  YappingMaw_Func_18(E->ymw_var_42);
  YappingMaw_Func_18(E->ymw_var_43);
  YappingMaw_Func_19();
}

void YappingMaw_Func_18(uint16 j) {  // 0xA8A85D
  int v1 = j >> 1;
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  enemy_projectile_gfx_idx[v1] = E->ymw_var_45 | enemy_projectile_gfx_idx[v1] & 0xF1FF;
  if (E->base.frozen_timer) {
    enemy_projectile_gfx_idx[v1] = enemy_projectile_gfx_idx[v1] & 0xF1FF | 0xC00;
    uint16 frozen_timer = E->base.frozen_timer;
    if (sign16(frozen_timer - 90)) {
      if ((frozen_timer & 2) == 0)
        enemy_projectile_gfx_idx[v1] = E->ymw_var_45 | enemy_projectile_gfx_idx[v1] & 0xF1FF;
    }
  }
}

void YappingMaw_Func_19(void) {  // 0xA8A899
  Enemy_YappingMaw *E = Get_YappingMaw(cur_enemy_index);
  uint16 r18 = E->ymw_var_45;
  uint16 r20 = E->ymw_var_46;
  sprite_palettes[r20 >> 1] = r18 | sprite_palettes[r20 >> 1] & 0xF1FF;
  if (E->base.frozen_timer) {
    sprite_palettes[r20 >> 1] = sprite_palettes[r20 >> 1] & 0xF1FF | 0xC00;
    uint16 frozen_timer = E->base.frozen_timer;
    if (sign16(frozen_timer - 90)) {
      if ((frozen_timer & 2) == 0)
        sprite_palettes[r20 >> 1] = r18 | sprite_palettes[r20 >> 1] & 0xF1FF;
    }
  }
}

void Kago_Init(void) {  // 0xA8AB46
  Enemy_Kago *E = Get_Kago(cur_enemy_index);
  E->base.properties |= kEnemyProps_DisableSamusColl;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->kago_var_E = 0;
  E->base.current_instruction = addr_kKago_Ilist_AB1E;
  E->kago_var_A = FUNC16(Kago_Func_1);
  E->kago_var_F = 0;
  E->kago_var_04 = E->kago_parameter_1;
}

void CallKagoFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnKago_Func_1: Kago_Func_1(k); return;
  case fnnullsub_306: return;
  default: Unreachable();
  }
}

void Kago_Main(void) {  // 0xA8AB75
  Enemy_Kago *E = Get_Kago(cur_enemy_index);
  CallKagoFunc(E->kago_var_A | 0xA80000, cur_enemy_index);
}

void Kago_Func_1(uint16 k) {  // 0xA8AB7B
  Get_Kago(k)->kago_var_A = FUNC16(nullsub_306);
}

void Kago_Shot(void) {  // 0xA8AB83
  int16 v2;

  NormalEnemyShotAi();
  earthquake_type = 2;
  earthquake_timer = 16;
  Enemy_Kago *E = Get_Kago(cur_enemy_index);
  if (!E->kago_var_B) {
    E->kago_var_B = 1;
    E->base.current_instruction = addr_kKago_Ilist_AB32;
    E->base.instruction_timer = 1;
  }
  v2 = E->kago_var_04 - 1;
  E->kago_var_04 = v2;
  if (v2 < 0) {
    EnemyDeathAnimation(cur_enemy_index, 4);
    E->kago_var_F = 1;
  }
  SpawnEnemyProjectileWithGfx(E->base.y_pos, cur_enemy_index, addr_loc_A8D02E);
}

const uint16 *NorfairLavaMan_Instr_1(uint16 k, const uint16 *jp) {  // 0xA8AE12
  if (!CheckIfEnemyIsOnScreen()) {
    QueueSfx2_Max6(jp[0]);
  }
  return jp + 1;
}

const uint16 *NorfairLavaMan_Instr_8(uint16 k, const uint16 *jp) {  // 0xA8AE26
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  ++E->base.y_pos;
  ++E->base.y_pos;
  return jp;
}

const uint16 *NorfairLavaMan_Instr_14(uint16 k, const uint16 *jp) {  // 0xA8AE30
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  --E->base.y_pos;
  --E->base.y_pos;
  return jp;
}

const uint16 *NorfairLavaMan_Instr_2(uint16 k, const uint16 *jp) {  // 0xA8AE3A
  Get_NorfairLavaMan(cur_enemy_index)->nlmn_var_01 = 1;
  return jp;
}

const uint16 *NorfairLavaMan_Instr_7(uint16 k, const uint16 *jp) {  // 0xA8AE45
  Get_NorfairLavaMan(cur_enemy_index)->nlmn_var_01 = 0;
  return jp;
}

const uint16 *NorfairLavaMan_Instr_10(uint16 k, const uint16 *jp) {  // 0xA8AE50
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  --E->base.y_pos;
  Enemy_NorfairLavaMan *E1 = Get_NorfairLavaMan(cur_enemy_index + 64);
  --E1->base.y_pos;
  return jp;
}

const uint16 *NorfairLavaMan_Instr_12(uint16 k, const uint16 *jp) {  // 0xA8AE5A
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  ++E->base.y_pos;
  Enemy_NorfairLavaMan *E1 = Get_NorfairLavaMan(cur_enemy_index + 64);
  ++E1->base.y_pos;
  return jp;
}

const uint16 *NorfairLavaMan_Instr_9(uint16 k, const uint16 *jp) {  // 0xA8AE64
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  uint16 v4 = E->nlmn_var_03 + 24;
  E->base.y_pos = v4;
  Enemy_NorfairLavaMan *E1 = Get_NorfairLavaMan(cur_enemy_index + 64);
  E1->base.y_pos = v4;
  E1->base.properties &= ~0x100;
  Enemy_NorfairLavaMan *E2 = Get_NorfairLavaMan(cur_enemy_index + 128);
  E2->base.properties &= ~0x100;
  return jp;
}

const uint16 *NorfairLavaMan_Instr_11(uint16 k, const uint16 *jp) {  // 0xA8AE88
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  uint16 nlmn_var_03 = E->nlmn_var_03;
  E->base.y_pos = nlmn_var_03;
  Get_NorfairLavaMan(cur_enemy_index + 64)->base.y_pos = nlmn_var_03;
  return jp;
}

const uint16 *NorfairLavaMan_Instr_13(uint16 k, const uint16 *jp) {  // 0xA8AE96
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  uint16 v4 = E->nlmn_var_03 + 4;
  E->base.y_pos = v4;
  Enemy_NorfairLavaMan *E1 = Get_NorfairLavaMan(cur_enemy_index + 64);
  E1->base.y_pos = v4;
  E1->base.properties |= 0x100;
  Enemy_NorfairLavaMan *E2 = Get_NorfairLavaMan(cur_enemy_index + 128);
  E2->base.properties |= 0x100;
  return jp;
}

const uint16 *NorfairLavaMan_Instr_5(uint16 k, const uint16 *jp) {  // 0xA8AEBA
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  SpawnEnemyProjectileWithGfx(E->nlmn_var_B, cur_enemy_index, addr_kEproj_LavaThrownByLavaman);
  return jp;
}

const uint16 *NorfairLavaMan_Instr_15(uint16 k, const uint16 *jp) {  // 0xA8AECA
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  E->base.x_pos = E->nlmn_var_12 + 8;
  E->base.y_pos = E->nlmn_var_13 - 4;
  return jp;
}

const uint16 *NorfairLavaMan_Instr_4(uint16 k, const uint16 *jp) {  // 0xA8AEE4
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  E->base.x_pos = E->nlmn_var_12 - 8;
  E->base.y_pos = E->nlmn_var_13 - 4;
  return jp;
}

const uint16 *NorfairLavaMan_Instr_16(uint16 k, const uint16 *jp) {  // 0xA8AEFE
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  E->base.x_pos = E->nlmn_var_12 + 8;
  E->base.y_pos = E->nlmn_var_13 - 8;
  return jp;
}

const uint16 *NorfairLavaMan_Instr_6(uint16 k, const uint16 *jp) {  // 0xA8AF18
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  E->base.x_pos = E->nlmn_var_12 - 8;
  E->base.y_pos = E->nlmn_var_13 - 4;
  return jp;
}

void sub_A8AF32(void) {  // 0xA8AF32
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  E->base.x_pos = E->nlmn_var_12;
  E->base.y_pos = E->nlmn_var_13;
}

const uint16 *NorfairLavaMan_Instr_3(uint16 k, const uint16 *jp) {  // 0xA8AF44
  Get_NorfairLavaMan(cur_enemy_index)->nlmn_var_04 = 256;
  return jp;
}
static Func_V *const g_off_A8AF4F[3] = { NorfairLavaMan_Func_1, NorfairLavaMan_Func_2, NorfairLavaMan_Func_3 };
void NorfairLavaMan_Init(void) {  // 0xA8AF8B
  uint16 v0 = 2 * Get_NorfairLavaMan(cur_enemy_index)->nlmn_parameter_1;
  g_off_A8AF4F[v0 >> 1]();
  NorfairLavaMan_Func_4();
  NorfairLavaMan_Func_5(cur_enemy_index);
}

void NorfairLavaMan_Func_1(void) {  // 0xA8AF9D
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  E->nlmn_var_C = 0;
  E->nlmn_var_00 = 0;
  E->nlmn_var_02 = 0;
  E->nlmn_var_03 = E->base.y_pos;
  if ((GetSamusEnemyDelta_X(cur_enemy_index) & 0x8000) != 0)
    E->nlmn_var_00 = 1;
  E->nlmn_var_D = addr_kNorfairLavaMan_Ilist_AC9C;
  if (!E->nlmn_var_00)
    E->nlmn_var_D = addr_kNorfairLavaMan_Ilist_AD3C;
  NorfairLavaMan_Func_20();
  E->nlmn_var_F = FUNC16(NorfairLavaMan_Func_7);
}

void NorfairLavaMan_Func_2(void) {  // 0xA8AFE2
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  E->nlmn_var_03 = E->base.y_pos;
  E->nlmn_var_C = 0;
  E->nlmn_var_0C = 0;
  E->nlmn_var_0D = 1;
  E->nlmn_var_D = addr_kNorfairLavaMan_Ilist_ADDC;
  NorfairLavaMan_Func_20();
  E->base.y_pos += 32;
  E->nlmn_var_F = FUNC16(NorfairLavaMan_Func_10);
  E->base.properties |= kEnemyProps_Invisible;
}

void NorfairLavaMan_Func_3(void) {  // 0xA8B020
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  E->nlmn_var_03 = E->base.y_pos;
  E->nlmn_var_11 = E->base.x_pos;
  E->nlmn_var_C = 0;
  E->nlmn_var_04 = 0;
  E->nlmn_var_D = addr_kNorfairLavaMan_Ilist_AE0C;
  NorfairLavaMan_Func_20();
  E->base.y_pos += 32;
  E->nlmn_var_F = FUNC16(NorfairLavaMan_Func_15);
  E->base.properties |= kEnemyProps_Invisible;
}

void NorfairLavaMan_Func_4(void) {  // 0xA8B05E
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  int v1 = (8 * HIBYTE(E->nlmn_parameter_2)) >> 1;
  E->nlmn_var_08 = kCommonEnemySpeeds_Linear[v1];
  E->nlmn_var_07 = kCommonEnemySpeeds_Linear[v1 + 1];
  E->nlmn_var_0A = kCommonEnemySpeeds_Linear[v1 + 2];
  E->nlmn_var_09 = kCommonEnemySpeeds_Linear[v1 + 3];
}

void NorfairLavaMan_Func_5(uint16 k) {  // 0xA8B088
  enemy_gfx_drawn_hook.addr = FUNC16(NorfairLavaMan_Func_6);
  *(uint16 *)&enemy_gfx_drawn_hook.bank = 168;
  variables_for_enemy_graphics_drawn_hook[0] = ((uint16)(Get_NorfairLavaMan(k)->base.palette_index & 0xE00) >> 4)
    + 256;
  variables_for_enemy_graphics_drawn_hook[2] = 8;
  variables_for_enemy_graphics_drawn_hook[1] = 0;
}

void NorfairLavaMan_Func_6(void) {  // 0xA8B0B2
  if (!door_transition_flag_enemies && !--variables_for_enemy_graphics_drawn_hook[2]) {
    variables_for_enemy_graphics_drawn_hook[2] = 8;
    ++variables_for_enemy_graphics_drawn_hook[1];
    int v0 = (uint16)(32 * (variables_for_enemy_graphics_drawn_hook[1] & 3)) >> 1;
    uint16 r18 = kNorfairLavaMan_Palette[v0 + 9];
    uint16 r20 = kNorfairLavaMan_Palette[v0 + 10];
    uint16 r22 = kNorfairLavaMan_Palette[v0 + 11];
    uint16 r24 = kNorfairLavaMan_Palette[v0 + 12];
    int v1 = variables_for_enemy_graphics_drawn_hook[0] >> 1;
    palette_buffer[v1 + 9] = r18;
    palette_buffer[v1 + 10] = r20;
    palette_buffer[v1 + 11] = r22;
    palette_buffer[v1 + 12] = r24;
  }
}

void NorfairLavaMan_Main(void) {  // 0xA8B10A
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  --E->nlmn_var_04;
  CallEnemyPreInstr(E->nlmn_var_F | 0xA80000);
}

void NorfairLavaMan_Func_7(uint16 k) {  // 0xA8B11A
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  E->nlmn_var_00 = 0;
  E->nlmn_var_D = addr_kNorfairLavaMan_Ilist_AC9C;
  if ((GetSamusEnemyDelta_X(cur_enemy_index) & 0x8000) == 0) {
    E->nlmn_var_D = addr_kNorfairLavaMan_Ilist_AD3C;
    E->nlmn_var_00 = 1;
  }
  NorfairLavaMan_Func_20();
  Enemy_NorfairLavaMan *E2 = Get_NorfairLavaMan(cur_enemy_index + 128);
  if ((E2->nlmn_var_04 & 0x8000) != 0) {
    E2->nlmn_var_04 = 0;
    if (IsSamusWithinEnemy_X(cur_enemy_index, LOBYTE(E->nlmn_parameter_2))) {
      E->nlmn_var_D = addr_kNorfairLavaMan_Ilist_ACDE;
      if (E->nlmn_var_00)
        E->nlmn_var_D = addr_kNorfairLavaMan_Ilist_AD7E;
      NorfairLavaMan_Func_20();
      E->nlmn_var_F = FUNC16(NorfairLavaMan_Func_8);
    }
  }
}

void NorfairLavaMan_Func_8(uint16 k) {  // 0xA8B175
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
   if (!E->nlmn_var_01) {
    E->nlmn_var_02 = 1;
    Get_NorfairLavaMan(cur_enemy_index + 64)->nlmn_var_0D = 0;
    E->nlmn_var_F = FUNC16(sub_A8B193);
  }
}

void sub_A8B193(uint16 k) {  // 0xA8B193
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  if (!E->nlmn_var_02) {
    E->nlmn_var_D = addr_kNorfairLavaMan_Ilist_AD0C;
    if ((GetSamusEnemyDelta_X(cur_enemy_index) & 0x8000) == 0)
      E->nlmn_var_D = addr_kNorfairLavaMan_Ilist_ADAC;
    NorfairLavaMan_Func_20();
    E->nlmn_var_F = FUNC16(NorfairLavaMan_Func_9);
  }
}

void NorfairLavaMan_Func_9(uint16 k) {  // 0xA8B1B8
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  if (!E->nlmn_var_01) {
    E->nlmn_var_D = addr_kNorfairLavaMan_Ilist_AC9C;
    if ((GetSamusEnemyDelta_X(cur_enemy_index) & 0x8000) == 0)
      E->nlmn_var_D = addr_kNorfairLavaMan_Ilist_AD3C;
    NorfairLavaMan_Func_20();
    E->nlmn_var_F = FUNC16(NorfairLavaMan_Func_7);
  }
}

void NorfairLavaMan_Func_10(uint16 k) {  // 0xA8B1DD
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  if (!E->nlmn_var_0D) {
    E->nlmn_var_F = FUNC16(NorfairLavaMan_Func_11);
    E->nlmn_var_E = 0;
    E->nlmn_var_0D = 0;
    E->nlmn_var_B = 2;
    E->base.y_pos = E->nlmn_var_03;
  }
}

void NorfairLavaMan_Func_11(uint16 k) {  // 0xA8B204
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  EnemySpawnData *v3 = gEnemySpawnData(cur_enemy_index);

  uint32 v = v3[31].ypos2 + E->base.y_subpos;
  E->base.y_subpos = v;
  E->base.y_pos += v3[31].field_14 + (v >> 16);
  E->nlmn_var_E += v3[31].field_14 + (v >> 16);
  NorfairLavaMan_Func_12(cur_enemy_index);
}

void NorfairLavaMan_Func_12(uint16 k) {  // 0xA8B230
  int v3;

  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(k);
  if (!sign16(-E->nlmn_var_E - 108))
    printf("Y undefined %d!\n", -(int16)E->nlmn_var_E);
  uint16 nlmn_var_B = E->nlmn_var_B;

  if (!sign16(-E->nlmn_var_E - 108)
      || (v3 = nlmn_var_B >> 1,
          (int16)(E->base.y_pos - g_word_A8AF79[v3] - samus_y_pos) < 0)) {
    E->nlmn_var_F = FUNC16(sub_A8B291);
    E->nlmn_var_0C = 1;
    if ((int16)(-E->nlmn_var_E - g_word_A8AF55[nlmn_var_B >> 1]) < 0)
      return;
    goto LABEL_6;
  }
  if ((int16)(-E->nlmn_var_E - g_word_A8AF55[v3]) >= 0) {
LABEL_6:
    ++E->nlmn_var_B;
    ++E->nlmn_var_B;
    E->base.y_pos += 8;
    E->nlmn_var_D = g_off_A8AF67[E->nlmn_var_B >> 1];
    NorfairLavaMan_Func_20();
  }
}

void sub_A8B291(uint16 k) {  // 0xA8B291
  ;
}

void NorfairLavaMan_Func_13(void) {  // 0xA8B295
  EnemySpawnData *v2;

  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  v2 = gEnemySpawnData(cur_enemy_index);
  bool v4 = __CFADD__uint16(v2[31].field_E, E->base.y_subpos);
  E->base.y_subpos = v2[31].field_E + E->base.y_subpos;
  E->base.y_pos += v2[31].xpos2 + v4;
  E->nlmn_var_E += v2[31].xpos2 + v4;
  NorfairLavaMan_Func_14(cur_enemy_index);
}

void NorfairLavaMan_Func_14(uint16 k) {  // 0xA8B2C5
  int16 nlmn_var_E;

  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(k);
  nlmn_var_E = E->nlmn_var_E;
  if (nlmn_var_E >= 0) {
    E->nlmn_var_0D = 1;
    gEnemySpawnData(k)[31].field_4 = 0;
    E->nlmn_var_F = FUNC16(NorfairLavaMan_Func_10);
  } else if ((int16)(-nlmn_var_E - g_word_A8AF55[(uint16)(E->nlmn_var_B - 2) >> 1]) < 0) {
    E->nlmn_var_B -= 2;
    E->base.y_pos -= 8;
    E->nlmn_var_D = g_off_A8AF67[E->nlmn_var_B >> 1];
    NorfairLavaMan_Func_20();
  }
}

void NorfairLavaMan_Func_15(uint16 k) {  // 0xA8B30D
  if (enemy_drawing_queue[(cur_enemy_index >> 1) + 87] == FUNC16(NorfairLavaMan_Func_8))
    Get_NorfairLavaMan(cur_enemy_index)->nlmn_var_F = FUNC16(NorfairLavaMan_Func_17);
}

void NorfairLavaMan_Func_16(uint16 k) {  // 0xA8B31F
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  if (!E->nlmn_var_01) {
    E->nlmn_var_D = addr_kNorfairLavaMan_Ilist_AE0C;
    NorfairLavaMan_Func_20();
    enemy_drawing_queue_sizes[(cur_enemy_index >> 1) + 5] = FUNC16(NorfairLavaMan_Func_13);
    E->nlmn_var_F = FUNC16(NorfairLavaMan_Func_18);
    gEnemySpawnData(cur_enemy_index)[31].field_18 = 0;
    E->base.x_pos = E->nlmn_var_12;
    E->base.y_pos = E->nlmn_var_13;
  }
  NorfairLavaMan_Func_19(cur_enemy_index);
}

void NorfairLavaMan_Func_17(uint16 k) {  // 0xA8B356
  if (gEnemySpawnData(cur_enemy_index)[31].field_18) {
    Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
    E->nlmn_var_D = addr_kNorfairLavaMan_Ilist_ACB0;
    E->nlmn_var_B = 0;
    if ((GetSamusEnemyDelta_X(cur_enemy_index) & 0x8000) == 0) {
      E->nlmn_var_D = addr_kNorfairLavaMan_Ilist_AD50;
      E->nlmn_var_B = 1;
    }
    NorfairLavaMan_Func_20();
    E->nlmn_var_F = FUNC16(NorfairLavaMan_Func_16);
    E->nlmn_var_12 = E->base.x_pos;
    E->nlmn_var_13 = E->base.y_pos;
  } else {
    Get_NorfairLavaMan(cur_enemy_index)->base.y_pos = enemy_drawing_queue[(cur_enemy_index >> 1) + 93]
      - g_word_A8AF79[enemy_drawing_queue_sizes[(cur_enemy_index >> 1) + 1] >> 1];
  }
  NorfairLavaMan_Func_19(cur_enemy_index);
}

void NorfairLavaMan_Func_18(uint16 k) {  // 0xA8B3A7
  int v1 = cur_enemy_index >> 1;
  if (enemy_drawing_queue[v1 + 87] == FUNC16(NorfairLavaMan_Func_7))
    Get_NorfairLavaMan(cur_enemy_index)->nlmn_var_F = FUNC16(NorfairLavaMan_Func_15);
  else
    Get_NorfairLavaMan(cur_enemy_index)->base.y_pos = enemy_drawing_queue[v1 + 93]
    - g_word_A8AF79[enemy_drawing_queue_sizes[v1 + 1] >> 1];
  NorfairLavaMan_Func_19(cur_enemy_index);
}

void NorfairLavaMan_Func_19(uint16 k) {  // 0xA8B3CB
  int v1 = k >> 1;
  uint16 v2 = enemy_drawing_queue[v1 + 61] - Get_NorfairLavaMan(k)->base.y_pos + 2;
  enemy_drawing_queue[v1 + 64] = v2;
  if (sign16(v2 - 8))
    enemy_drawing_queue[v1 + 64] = 8;
}

void NorfairLavaMan_Func_20(void) {  // 0xA8B3E5
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  uint16 nlmn_var_D = E->nlmn_var_D;
  if (nlmn_var_D != E->nlmn_var_C) {
    E->base.current_instruction = nlmn_var_D;
    E->nlmn_var_C = nlmn_var_D;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

void NorfairLavaMan_Powerbomb(void) {  // 0xA8B400
  Enemy_NormalPowerBombAI_A8();
  NorfairLavaMan_Common();
}

void NorfairLavaMan_Touch(void) {  // 0xA8B406
  Enemy_NormalTouchAI_A8();
  NorfairLavaMan_Common();
}

void NorfairLavaMan_Shot(void) {  // 0xA8B40C
  NormalEnemyShotAi();
  NorfairLavaMan_Common();
}

void NorfairLavaMan_Common(void) {  // 0xA8B410
  Enemy_NorfairLavaMan *E = Get_NorfairLavaMan(cur_enemy_index);
  Enemy_NorfairLavaMan *E1 = Get_NorfairLavaMan(cur_enemy_index + 64);
  Enemy_NorfairLavaMan *E2 = Get_NorfairLavaMan(cur_enemy_index + 128);
  if (!E->base.health) {
    E1->base.properties |= 0x200;
    E2->base.properties |= 0x200;
  }
  uint16 frozen_timer = E->base.frozen_timer;
  if (frozen_timer) {
    E1->base.frozen_timer = frozen_timer;
    E1->base.ai_handler_bits |= 4;
    E2->base.frozen_timer = frozen_timer;
    E2->base.ai_handler_bits |= 4;
  }
}

const uint16 *Beetom_Instr_1(uint16 k, const uint16 *jp) {  // 0xA8B75E
  return jp;
}

void Beetom_Func_1(void) {  // 0xA8B762
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  E->base.current_instruction = E->beetom_var_00;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
}

void Beetom_Init(void) {  // 0xA8B776
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  E->beetom_var_D = 0;
  E->beetom_var_C = 0;
  E->beetom_var_05 = 0;
  E->beetom_var_08 = 0;
  E->beetom_var_A = 0;
  E->beetom_var_E = 64;
  E->beetom_var_F = joypad1_lastkeys;
  random_number = 23;
  E->beetom_var_02 = Beetom_Func_2(12288, 4);
  E->beetom_var_03 = Beetom_Func_2(0x4000, 5);
  E->beetom_var_04 = Beetom_Func_2(12288, 3);
  E->beetom_var_00 = addr_kBeetom_Ilist_B6F2;
  if ((GetSamusEnemyDelta_X(cur_enemy_index) & 0x8000) != 0)
    E->beetom_var_00 = addr_kBeetom_Ilist_B696;
  Beetom_Func_1();
  E->beetom_var_C = FUNC16(Beetom_Func_3);
}

uint16 Beetom_Func_2(uint16 r22, uint16 r24) {  // 0xA8B7EF
  uint16 r18 = 0, r20 = 0;
  do {
    r18 += r24;
    r20 += *(uint16 *)((uint8 *)kCommonEnemySpeeds_Quadratic + (8 * r18) + 1);
  } while (sign16(r20 - r22));
  return r18;
}

void Beetom_Func_3(void) {  // 0xA8B814
  if (IsSamusWithinEnemy_X(cur_enemy_index, 0x60))
    Get_Beetom(cur_enemy_index)->beetom_var_C = FUNC16(Beetom_Func_12);
  else
    Get_Beetom(cur_enemy_index)->beetom_var_C = FUNC16(Beetom_Func_4);
}

void Beetom_Func_4(void) {  // 0xA8B82F
  NextRandom();
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  E->beetom_var_C = kBeetom_Ilist_B74E[random_number & 7];
  E->beetom_var_09 = random_number & 1;
}

void Beetom_Func_5(void) {  // 0xA8B84F
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  E->beetom_var_D = 32;
  E->beetom_var_C = FUNC16(Beetom_Func_17);
}

void Beetom_Func_6(void) {  // 0xA8B85F
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  E->beetom_var_C = FUNC16(Beetom_Func_18);
  E->beetom_var_00 = addr_kBeetom_Ilist_B696;
  Beetom_Func_1();
}

void Beetom_Func_7(void) {  // 0xA8B873
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  E->beetom_var_C = FUNC16(Beetom_Func_19);
  E->beetom_var_00 = addr_kBeetom_Ilist_B6F2;
  Beetom_Func_1();
}

void Beetom_Func_8(void) {  // 0xA8B887
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  E->beetom_var_B = E->beetom_var_02;
  E->beetom_var_C = FUNC16(Beetom_Func_20);
  E->beetom_var_05 = 0;
  E->beetom_var_00 = addr_kBeetom_Ilist_B6AC;
  Beetom_Func_1();
}

void Beetom_Func_9(void) {  // 0xA8B8A9
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  E->beetom_var_B = E->beetom_var_02;
  E->beetom_var_C = FUNC16(Beetom_Func_21);
  E->beetom_var_05 = 0;
  E->beetom_var_00 = addr_kBeetom_Ilist_B708;
  Beetom_Func_1();
}

void Beetom_Func_10(void) {  // 0xA8B8CB
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  E->beetom_var_B = E->beetom_var_03;
  E->beetom_var_C = FUNC16(Beetom_Func_24);
  E->beetom_var_05 = 0;
  E->beetom_var_00 = addr_kBeetom_Ilist_B6AC;
  Beetom_Func_1();
}

void Beetom_Func_11(void) {  // 0xA8B8ED
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  E->beetom_var_B = E->beetom_var_03;
  E->beetom_var_C = FUNC16(Beetom_Func_25);
  E->beetom_var_05 = 0;
  E->beetom_var_00 = addr_kBeetom_Ilist_B708;
  Beetom_Func_1();
}

void Beetom_Func_12(void) {  // 0xA8B90F
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  E->beetom_var_B = E->beetom_var_04;
  E->beetom_var_00 = addr_kBeetom_Ilist_B708;
  E->beetom_var_C = FUNC16(Beetom_Func_29);
  E->beetom_var_09 = 1;
  if ((GetSamusEnemyDelta_X(cur_enemy_index) & 0x8000) != 0) {
    E->beetom_var_00 = addr_kBeetom_Ilist_B6AC;
    E->beetom_var_C = FUNC16(Beetom_Func_28);
    E->beetom_var_09 = 0;
  }
  Beetom_Func_1();
  E->beetom_var_05 = 0;
}

void Beetom_Func_13(void) {  // 0xA8B952
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  E->beetom_var_00 = addr_kBeetom_Ilist_B6CC;
  Beetom_Func_1();
  E->beetom_var_C = FUNC16(Beetom_Func_32);
}

void Beetom_Func_14(void) {  // 0xA8B966
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  E->beetom_var_00 = addr_kBeetom_Ilist_B728;
  Beetom_Func_1();
  E->beetom_var_C = FUNC16(Beetom_Func_33);
}

void Beetom_Func_15(void) {  // 0xA8B97A
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  E->beetom_var_00 = addr_kBeetom_Ilist_B696;
  if (E->beetom_var_09)
    E->beetom_var_00 = addr_kBeetom_Ilist_B6F2;
  Beetom_Func_1();
  E->beetom_var_05 = 0;
  E->beetom_var_C = FUNC16(Beetom_Func_35);
}

void Beetom_Func_16(void) {  // 0xA8B9A2
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  E->beetom_var_B = 0;
  E->beetom_var_C = FUNC16(Beetom_Func_36);
}

void Beetom_Func_17(void) {  // 0xA8B9B2
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  if ((--E->beetom_var_D & 0x8000) != 0)
    E->beetom_var_C = FUNC16(Beetom_Func_3);
}

void Beetom_Func_18(void) {  // 0xA8B9C1
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  if ((--E->beetom_var_D & 0x8000) != 0) {
    E->beetom_var_D = 64;
    E->beetom_var_C = FUNC16(Beetom_Func_3);
  } else {
    E->base.x_pos -= 8;
    if (Enemy_MoveDown(cur_enemy_index, INT16_SHL16(1))) {
      E->base.x_pos += 8;
      if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, -16384))
        E->beetom_var_C = FUNC16(Beetom_Func_7);
    } else {
      E->beetom_var_C = FUNC16(Beetom_Func_7);
      --E->base.y_pos;
      E->base.x_pos += 8;
    }
  }
}

void Beetom_Func_19(void) {  // 0xA8BA24
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  if ((--E->beetom_var_D & 0x8000) != 0) {
    E->beetom_var_D = 64;
    E->beetom_var_C = FUNC16(Beetom_Func_3);
  } else {
    E->base.x_pos += 8;
    if (Enemy_MoveDown(cur_enemy_index, INT16_SHL16(1))) {
      E->base.x_pos -= 8;
      if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, 0x4000))
        E->beetom_var_C = FUNC16(Beetom_Func_6);
    } else {
      E->beetom_var_C = FUNC16(Beetom_Func_6);
      --E->base.y_pos;
      E->base.x_pos -= 8;
    }
  }
}

void Beetom_Func_22(uint16 k) {  // 0xA8BAE7
  Enemy_Beetom *E = Get_Beetom(k);
  int v2 = (8 * E->beetom_var_B) >> 1;
  if (Enemy_MoveDown(k, kCommonEnemySpeeds_Quadratic32[(v2 + 2) >> 1])) {
    E->beetom_var_C = FUNC16(Beetom_Func_15);
  } else {
    E->beetom_var_B -= 4;
    if (sign16(E->beetom_var_B)) {
      E->beetom_var_B = 0;
      E->beetom_var_05 = 1;
    }
  }
}

void Beetom_Func_23(uint16 k) {  // 0xA8BB20
  Enemy_Beetom *E = Get_Beetom(k);
  int v2 = (8 * E->beetom_var_B) >> 1;
  if (Enemy_MoveDown(k, kCommonEnemySpeeds_Quadratic32[v2 >> 1])) {
    E->beetom_var_C = FUNC16(Beetom_Func_3);
  } else {
    E->beetom_var_B += 4;
    if (!sign16(E->beetom_var_B - 64))
      E->beetom_var_B = 64;
  }
}

void Beetom_Func_20(void) {  // 0xA8BA84
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  if (E->beetom_var_05)
    Beetom_Func_23(cur_enemy_index);
  else
    Beetom_Func_22(cur_enemy_index);
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, -16384)) {
    E->beetom_var_09 ^= 1;
    E->beetom_var_C = FUNC16(Beetom_Func_15);
  }
}

void Beetom_Func_21(void) {  // 0xA8BAB7
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  if (E->beetom_var_05)
    Beetom_Func_23(cur_enemy_index);
  else
    Beetom_Func_22(cur_enemy_index);
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, 0x4000)) {
    E->beetom_var_09 ^= 1;
    E->beetom_var_C = FUNC16(Beetom_Func_15);
  }
}

void Beetom_Func_26(uint16 k) {  // 0xA8BBB8
  Enemy_Beetom *E = Get_Beetom(k);
  int v2 = (8 * E->beetom_var_B) >> 1;
  if (Enemy_MoveDown(k, kCommonEnemySpeeds_Quadratic32[(v2 + 2) >> 1])) {
    E->beetom_var_C = FUNC16(Beetom_Func_15);
  } else {
    E->beetom_var_B -= 5;
    if (sign16(E->beetom_var_B)) {
      E->beetom_var_B = 0;
      E->beetom_var_05 = 1;
    }
  }
}

void Beetom_Func_27(uint16 k) {  // 0xA8BBF1
  Enemy_Beetom *E = Get_Beetom(k);
  int v2 = (8 * E->beetom_var_B) >> 1;
  if (Enemy_MoveDown(k, kCommonEnemySpeeds_Quadratic32[v2 >> 1])) {
    E->beetom_var_C = FUNC16(Beetom_Func_3);
  } else {
    E->beetom_var_B += 5;
    if (!sign16(E->beetom_var_B - 64))
      E->beetom_var_B = 64;
  }
}

void Beetom_Func_24(void) {  // 0xA8BB55
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  if (E->beetom_var_05)
    Beetom_Func_27(cur_enemy_index);
  else
    Beetom_Func_26(cur_enemy_index);
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, -16384)) {
    E->beetom_var_09 ^= 1;
    E->beetom_var_C = FUNC16(Beetom_Func_15);
  }
}

void Beetom_Func_25(void) {  // 0xA8BB88
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  if (E->beetom_var_05)
    Beetom_Func_27(cur_enemy_index);
  else
    Beetom_Func_26(cur_enemy_index);
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, 0x4000)) {
    E->beetom_var_09 ^= 1;
    E->beetom_var_C = FUNC16(Beetom_Func_15);
  }
}


void Beetom_Func_30(uint16 k) {  // 0xA8BC8A
  Enemy_Beetom *E = Get_Beetom(k);
  int v2 = (8 * E->beetom_var_B) >> 1;
  if (Enemy_MoveDown(k, kCommonEnemySpeeds_Quadratic32[(v2 + 2) >> 1])) {
    E->beetom_var_C = FUNC16(Beetom_Func_15);
  } else {
    int16 v3 = E->beetom_var_B - 5;
    E->beetom_var_B = v3;
    if (v3 < 0) {
      E->beetom_var_B = 0;
      E->beetom_var_05 = 1;
    }
  }
}

void Beetom_Func_31(uint16 k) {  // 0xA8BCC3
  Enemy_Beetom *E = Get_Beetom(k);
  int v2 = (8 * E->beetom_var_B) >> 1;
  if (Enemy_MoveDown(k, kCommonEnemySpeeds_Quadratic32[v2 >> 1])) {
    E->beetom_var_C = FUNC16(Beetom_Func_3);
  } else {
    uint16 v3 = E->beetom_var_B + 3;
    E->beetom_var_B = v3;
    if (!sign16(v3 - 64))
      E->beetom_var_B = 64;
  }
}
void Beetom_Func_28(void) {  // 0xA8BC26
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  if (E->beetom_var_05)
    Beetom_Func_31(cur_enemy_index);
  else
    Beetom_Func_30(cur_enemy_index);
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, __PAIR32__(-3, 0))) {
    E->beetom_var_09 ^= 1;
    E->beetom_var_C = FUNC16(Beetom_Func_15);
  }
}

void Beetom_Func_29(void) {  // 0xA8BC5A
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  if (E->beetom_var_05)
    Beetom_Func_31(cur_enemy_index);
  else
    Beetom_Func_30(cur_enemy_index);
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, __PAIR32__(3, 0))) {
    E->beetom_var_09 ^= 1;
    E->beetom_var_C = FUNC16(Beetom_Func_15);
  }
}

void Beetom_Func_34(void) {  // 0xA8BD8C
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  if (joypad1_lastkeys != E->beetom_var_F) {
    E->beetom_var_F = joypad1_lastkeys;
    --E->beetom_var_E;
  }
}

void Beetom_Func_32(void) {  // 0xA8BCF8
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  if (E->beetom_var_E) {
    E->base.x_pos = samus_x_pos;
    E->base.y_pos = samus_y_pos - 4;
    Beetom_Func_34();
  } else {
    if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, __PAIR32__(16, 0))) {
      E->beetom_var_09 = 1;
      Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, __PAIR32__(-32, 0));
    }
    E->beetom_var_08 = 0;
    E->beetom_var_C = FUNC16(Beetom_Func_16);
  }
}

void Beetom_Func_33(void) {  // 0xA8BD42
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  if (E->beetom_var_E) {
    E->base.x_pos = samus_x_pos;
    E->base.y_pos = samus_y_pos - 4;
    Beetom_Func_34();
  } else {
    if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, __PAIR32__(-16, 0))) {
      E->beetom_var_09 = 0;
      Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, __PAIR32__(32, 0));
    }
    E->beetom_var_08 = 0;
    E->beetom_var_C = FUNC16(Beetom_Func_16);
  }
}

void Beetom_Func_35(void) {  // 0xA8BD9D
  if (Enemy_MoveDown(cur_enemy_index, __PAIR32__(3, 0))) {
    Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
    if (E->beetom_var_09)
      E->beetom_var_C = FUNC16(Beetom_Func_7);
    else
      E->beetom_var_C = FUNC16(Beetom_Func_6);
  }
}

void Beetom_Func_37(uint16 k) {  // 0xA8BDCC
  Enemy_Beetom *E = Get_Beetom(k);
  int v2 = (8 * E->beetom_var_B) >> 1;
  if (Enemy_MoveDown(k, kCommonEnemySpeeds_Quadratic32[v2 >> 1])) {
    E->beetom_var_C = FUNC16(Beetom_Func_3);
  } else {
    uint16 v3 = E->beetom_var_B + 1;
    E->beetom_var_B = v3;
    if (!sign16(v3 - 64))
      E->beetom_var_B = 64;
  }
  uint16 v4 = (E->beetom_var_09) ? -2 : 2;
  if (Enemy_MoveRight_IgnoreSlopes(k, __PAIR32__(v4, 0))) {
    E->beetom_var_09 ^= 1;
    E->beetom_var_C = FUNC16(Beetom_Func_15);
  }
}

void Beetom_Func_36(void) {  // 0xA8BDC5
  Beetom_Func_37(cur_enemy_index);
}

void CallBeetomFunc(uint32 ea) {
  switch (ea) {
  case fnBeetom_Func_3: Beetom_Func_3(); return;  // 0xa8b814
  case fnBeetom_Func_4: Beetom_Func_4(); return;  // 0xa8b82f
  case fnBeetom_Func_5: Beetom_Func_5(); return;  // 0xa8b84f
  case fnBeetom_Func_6: Beetom_Func_6(); return;  // 0xa8b85f
  case fnBeetom_Func_7: Beetom_Func_7(); return;  // 0xa8b873
  case fnBeetom_Func_8: Beetom_Func_8(); return;  // 0xa8b887
  case fnBeetom_Func_9: Beetom_Func_9(); return;  // 0xa8b8a9
  case fnBeetom_Func_10: Beetom_Func_10(); return;  // 0xa8b8cb
  case fnBeetom_Func_11: Beetom_Func_11(); return;  // 0xa8b8ed
  case fnBeetom_Func_12: Beetom_Func_12(); return;  // 0xa8b90f
  case fnBeetom_Func_13: Beetom_Func_13(); return;  // 0xa8b952
  case fnBeetom_Func_14: Beetom_Func_14(); return;  // 0xa8b966
  case fnBeetom_Func_15: Beetom_Func_15(); return;  // 0xa8b97a
  case fnBeetom_Func_16: Beetom_Func_16(); return;  // 0xa8b9a2
  case fnBeetom_Func_17: Beetom_Func_17(); return;  // 0xa8b9b2
  case fnBeetom_Func_18: Beetom_Func_18(); return;  // 0xa8b9c1
  case fnBeetom_Func_19: Beetom_Func_19(); return;  // 0xa8ba24
  case fnBeetom_Func_20: Beetom_Func_20(); return;  // 0xa8ba84
  case fnBeetom_Func_21: Beetom_Func_21(); return;  // 0xa8bab7
  case fnBeetom_Func_24: Beetom_Func_24(); return;  // 0xa8bb55
  case fnBeetom_Func_25: Beetom_Func_25(); return;  // 0xa8bb88
  case fnBeetom_Func_28: Beetom_Func_28(); return;  // 0xa8bc26
  case fnBeetom_Func_29: Beetom_Func_29(); return;  // 0xa8bc5a
  case fnBeetom_Func_32: Beetom_Func_32(); return;  // 0xa8bcf8
  case fnBeetom_Func_33: Beetom_Func_33(); return;  // 0xa8bd42
  case fnBeetom_Func_35: Beetom_Func_35(); return;  // 0xa8bd9d
  case fnBeetom_Func_36: Beetom_Func_36(); return;  // 0xa8bdc5
  default: Unreachable();
  }
}

void Beetom_Main(void) {  // 0xA8B80D
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  CallBeetomFunc(E->beetom_var_C | 0xA80000);
}

void Beetom_Touch(void) {  // 0xA8BE2E
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  if (!E->beetom_var_08) {
    if (E->beetom_var_09)
      E->beetom_var_C = FUNC16(Beetom_Func_14);
    else
      E->beetom_var_C = FUNC16(Beetom_Func_13);
    E->beetom_var_08 = 1;
    E->beetom_var_E = 64;
    E->base.layer = 2;
    E->beetom_var_06 = samus_x_pos - E->base.x_pos;
    E->beetom_var_07 = samus_y_pos - E->base.y_pos;
  }
  if (samus_contact_damage_index)
    goto LABEL_11;
  if ((random_enemy_counter & 7) == 7 && !sign16(samus_health - 30))
    QueueSfx3_Max6(0x2D);
  if ((E->base.frame_counter & 0x3F) == 63) {
LABEL_11:
    Enemy_NormalTouchAI_A8();
    samus_invincibility_timer = 0;
    samus_knockback_timer = 0;
  }
}

void Beetom_Shot(void) {  // 0xA8BEAC
  Enemy_NormalShotAI_A8();
  Enemy_Beetom *E = Get_Beetom(cur_enemy_index);
  if (E->base.frozen_timer
      && (E->beetom_var_C == FUNC16(Beetom_Func_33)
          || E->beetom_var_C == FUNC16(Beetom_Func_32))) {
    E->beetom_var_C = FUNC16(Beetom_Func_15);
  }
  E->beetom_var_08 = 0;
}

void MaridiaFloater_Init(void) {  // 0xA8C1C9
  Enemy_MaridiaFloater *E = Get_MaridiaFloater(cur_enemy_index);
  E->base.properties |= kEnemyProps_DisableSamusColl;
  E->base.spritemap_pointer = addr_kSpritemap_Nothing_A8;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  if (E->mfr_parameter_1) {
    E->mfr_var_A = E->base.x_pos;
    E->mfr_var_B = E->base.y_pos;
    E->mfr_var_F = FUNC16(nullsub_256);
    E->base.current_instruction = addr_stru_A8C199;
    E->mfr_var_D = E->mfr_parameter_2;
  } else {
    E->mfr_var_E = 60;
    E->mfr_var_F = FUNC16(MaridiaFloater_Func_3);
    E->base.current_instruction = addr_kMaridiaFloater_Ilist_C173;
  }
}

void MaridiaFloater_Main(void) {  // 0xA8C21C
  Enemy_MaridiaFloater *E = Get_MaridiaFloater(cur_enemy_index);
  CallEnemyPreInstr(E->mfr_var_F | 0xA80000);
}

void MaridiaFloater_Func_1(uint16 k) {  // 0xA8C223
  for (int i = 7; i >= 0; --i)
    SpawnEnemyProjectileWithGfx(i, k, addr_kEproj_MaridiaFloatersSpikes);
}

void MaridiaFloater_Func_2(uint16 k) {  // 0xA8C234
  int v1 = k >> 1;
  uint16 v2 = enemy_drawing_queue[v1 + 103];
  if (sign16(v2 + 0x3E6F)) {
    uint16 v3 = (uint16)(v2 - 4 + 15997) >> 1;
    if (!sign16(v3 - 6))
      v3 = 0;
    enemy_drawing_queue[v1 + 93] = g_word_A8C277[v3 >> 1] + Get_MaridiaFloater(k)->base.y_pos;
  } else {
    uint16 v4 = (uint16)(v2 - 4 + 15983) >> 1;
    if (!sign16(v4 - 6))
      v4 = 0;
    enemy_drawing_queue[v1 + 93] = g_word_A8C27D[v4 >> 1] + Get_MaridiaFloater(k)->base.y_pos;
  }
}

void MaridiaFloater_Func_3(uint16 k) {  // 0xA8C283
  Enemy_MaridiaFloater *E = Get_MaridiaFloater(k);
  bool v2 = E->mfr_var_E == 1;
  bool v3 = (--E->mfr_var_E & 0x8000) != 0;
  if (v2 || v3) {
    int v4 = k >> 1;
    enemy_drawing_queue[v4 + 104] = 1;
    enemy_drawing_queue[v4 + 103] = addr_kMaridiaFloater_Ilist_C183;
    E->mfr_var_F = FUNC16(MaridiaFloater_Func_4);
    E->mfr_var_E = 10;
  }
  MaridiaFloater_Func_2(k);
}

void MaridiaFloater_Func_4(uint16 k) {  // 0xA8C2A6
  Enemy_MaridiaFloater *E = Get_MaridiaFloater(k);
  bool v2 = E->mfr_var_E == 1;
  bool v3 = (--E->mfr_var_E & 0x8000) != 0;
  if (v2 || v3) {
    E->mfr_var_F = FUNC16(MaridiaFloater_Func_5);
    E->mfr_var_A = g_word_A8C1C5;
    E->mfr_var_B = g_word_A8C1C7;
    E->base.instruction_timer = 1;
    E->base.current_instruction = addr_kMaridiaFloater_Ilist_C163;
  }
  MaridiaFloater_Func_2(k);
}

void MaridiaFloater_Func_5(uint16 k) {  // 0xA8C2CF
  Enemy_MaridiaFloater *E = Get_MaridiaFloater(k);
  if ((E->base.ai_handler_bits & 1) != 0) {
    E->mfr_var_F = FUNC16(MaridiaFloater_Func_7);
    MaridiaFloater_Func_7(k);
  } else {
    bool v2 = E->mfr_var_D == 1;
    bool v3 = (--E->mfr_var_D & 0x8000) != 0;
    if (v2 || v3) {
      E->mfr_var_D = 5;
      int v4 = k >> 1;
      uint16 v5 = g_word_A8C1A1[E->mfr_var_C] + enemy_drawing_queue_sizes[v4];
      enemy_drawing_queue[v4 + 91] = v5;
      E->base.x_pos = v5;
      uint16 v6 = E->mfr_var_C + 1;
      if (!sign16(E->mfr_var_C - 11))
        v6 = 0;
      E->mfr_var_C = v6;
    }
    AddToHiLo(&E->mfr_var_A, &E->mfr_var_B, __PAIR32__(g_word_A8C1B9, g_word_A8C1BB));
    if (Enemy_MoveDown(k, __PAIR32__(E->mfr_var_A, E->mfr_var_B))
        || (int16)(enemy_drawing_queue_sizes[(k >> 1) + 1] - g_word_A8C19F - E->base.y_pos) >= 0) {
      uint16 mfr_var_C = E->mfr_var_C;
      if (!mfr_var_C || mfr_var_C == 6) {
        E->mfr_var_F = FUNC16(MaridiaFloater_Func_10);
        E->mfr_var_E = 10;
        int v10 = k >> 1;
        enemy_drawing_queue[v10 + 104] = 1;
        enemy_drawing_queue[v10 + 103] = addr_kMaridiaFloater_Ilist_C191;
      } else {
        E->mfr_var_F = FUNC16(MaridiaFloater_Func_6);
      }
    }
    MaridiaFloater_Func_2(k);
  }
}

void MaridiaFloater_Func_6(uint16 k) {  // 0xA8C36B
  Enemy_MaridiaFloater *E = Get_MaridiaFloater(k);
  bool v2 = E->mfr_var_D == 1;
  bool v3 = (--E->mfr_var_D & 0x8000) != 0;
  if (!v2 && !v3) {
LABEL_9:;
    AddToHiLo(&E->mfr_var_A, &E->mfr_var_B, __PAIR32__(g_word_A8C1B9, g_word_A8C1BB));
    Enemy_MoveDown(k, __PAIR32__(E->mfr_var_A, E->mfr_var_B));
    goto LABEL_10;
  }
  E->mfr_var_D = 5;
  int v4;
  v4 = k >> 1;
  {
    uint16 v5 = g_word_A8C1A1[E->mfr_var_C] + enemy_drawing_queue_sizes[v4];
    enemy_drawing_queue[v4 + 91] = v5;
    E->base.x_pos = v5;
  }
  if (E->mfr_var_C && E->mfr_var_C != 6) {
    uint16 v7 = E->mfr_var_C + 1;
    if (!sign16(v7 - 12))
      v7 = 0;
    E->mfr_var_C = v7;
    goto LABEL_9;
  }
  E->mfr_var_F = FUNC16(MaridiaFloater_Func_10);
  E->mfr_var_E = 10;
  enemy_drawing_queue[v4 + 104] = 1;
  enemy_drawing_queue[v4 + 103] = addr_kMaridiaFloater_Ilist_C191;
LABEL_10:
  MaridiaFloater_Func_2(k);
}

void MaridiaFloater_Func_7(uint16 k) {  // 0xA8C3E1
  Enemy_MaridiaFloater *E = Get_MaridiaFloater(k);
  if ((E->base.ai_handler_bits & 1) != 0) {
    bool v2 = E->mfr_var_D == 1;
    bool v3 = (--E->mfr_var_D & 0x8000) != 0;
    if (v2 || v3) {
      E->mfr_var_D = 5;
      int v4 = k >> 1;
      uint16 v5 = g_word_A8C1A1[E->mfr_var_C] + enemy_drawing_queue_sizes[v4];
      enemy_drawing_queue[v4 + 91] = v5;
      E->base.x_pos = v5;
      uint16 v6 = E->mfr_var_C + 1;
      if (!sign16(E->mfr_var_C - 11))
        v6 = 0;
      E->mfr_var_C = v6;
    }
    AddToHiLo(&E->mfr_var_A, &E->mfr_var_B, __PAIR32__(g_word_A8C1B9, g_word_A8C1BB));
    if (Enemy_MoveDown(k, __PAIR32__(E->mfr_var_A, E->mfr_var_B))
        || (int16)(enemy_drawing_queue_sizes[(k >> 1) + 1] - enemy_drawing_queue_sizes[(k >> 1) + 3] - E->base.y_pos) >= 0) {
      if (!E->mfr_var_C || E->mfr_var_C == 6)
        E->mfr_var_F = FUNC16(MaridiaFloater_Func_9);
      else
        E->mfr_var_F = FUNC16(MaridiaFloater_Func_8);
    }
    MaridiaFloater_Func_2(k);
  } else {
    E->mfr_var_F = FUNC16(MaridiaFloater_Func_6);
  }
}

void MaridiaFloater_Func_8(uint16 k) {  // 0xA8C469
  Enemy_MaridiaFloater *E = Get_MaridiaFloater(k);
  if ((E->base.ai_handler_bits & 1) == 0) {
    E->mfr_var_F = FUNC16(MaridiaFloater_Func_6);
    return;
  }
  bool v2 = E->mfr_var_D == 1;
  bool v3 = (--E->mfr_var_D & 0x8000) != 0;
  if (!v2 && !v3)
    goto LABEL_11;
  E->mfr_var_D = 5;
  int v4;
  v4 = k >> 1;
  uint16 v5;
  v5 = g_word_A8C1A1[E->mfr_var_C] + enemy_drawing_queue_sizes[v4];
  enemy_drawing_queue[v4 + 91] = v5;
  E->base.x_pos = v5;
  if (E->mfr_var_C && E->mfr_var_C != 6) {
    {
      uint16 v7 = E->mfr_var_C + 1;
      if (!sign16(v7 - 12))
        v7 = 0;
      E->mfr_var_C = v7;
    }
LABEL_11:;
    AddToHiLo(&E->mfr_var_A, &E->mfr_var_B, __PAIR32__(g_word_A8C1B9, g_word_A8C1BB));
    Enemy_MoveDown(k, __PAIR32__(E->mfr_var_A, E->mfr_var_B));
    goto LABEL_12;
  }
  E->mfr_var_F = FUNC16(MaridiaFloater_Func_9);
LABEL_12:
  MaridiaFloater_Func_2(k);
}

void MaridiaFloater_Func_9(uint16 k) {  // 0xA8C4DC
  Enemy_MaridiaFloater *E = Get_MaridiaFloater(k);
  if ((E->base.ai_handler_bits & 1) == 0) {
    E->mfr_var_F = FUNC16(MaridiaFloater_Func_10);
    E->mfr_var_E = 10;
    int v2 = k >> 1;
    enemy_drawing_queue[v2 + 104] = 1;
    enemy_drawing_queue[v2 + 103] = addr_kMaridiaFloater_Ilist_C191;
  }
  MaridiaFloater_Func_2(k);
}

void MaridiaFloater_Func_10(uint16 k) {  // 0xA8C500
  Enemy_MaridiaFloater *E = Get_MaridiaFloater(k);
  bool v2 = E->mfr_var_E == 1;
  bool v3 = (--E->mfr_var_E & 0x8000) != 0;
  if (v2 || v3) {
    E->mfr_var_F = FUNC16(MaridiaFloater_Func_11);
    E->mfr_var_A = g_word_A8C1C1;
    E->mfr_var_B = g_word_A8C1C3;
  }
  MaridiaFloater_Func_2(k);
}

void MaridiaFloater_Func_11(uint16 k) {  // 0xA8C51D
  Enemy_MaridiaFloater *E = Get_MaridiaFloater(k);
  AddToHiLo(&E->mfr_var_A, &E->mfr_var_B, __PAIR32__(g_word_A8C1BD, g_word_A8C1BF));
  Enemy_MoveDown(k, __PAIR32__(E->mfr_var_A, E->mfr_var_B));
  int v4 = k >> 1;
  if ((int16)(E->base.y_pos - enemy_drawing_queue_sizes[v4 + 1]) >= 0) {
    E->base.y_pos = enemy_drawing_queue_sizes[v4 + 1];
    E->mfr_var_F = FUNC16(MaridiaFloater_Func_3);
    E->mfr_var_E = 60;
    E->base.instruction_timer = 1;
    E->base.current_instruction = addr_kMaridiaFloater_Ilist_C173;
  }
  MaridiaFloater_Func_2(k);
}

void MaridiaFloater_Func_12(uint16 k) {  // 0xA8C569
  int v1 = k >> 1;
  if (!sign16(enemy_drawing_queue[v1 + 103] + 0x3E6F)) {
    uint16 v2 = (uint16)(enemy_drawing_queue[v1 + 103] - 4 + 15983) >> 1;
    if (v2) {
      enemy_drawing_queue[v1 + 103] = g_off_A8C599[v2 >> 1];
      enemy_drawing_queue[v1 + 104] = 1;
    }
  }
  Enemy_MaridiaFloater *E = Get_MaridiaFloater(k);
  E->mfr_var_F = FUNC16(MaridiaFloater_Func_13);
  E->mfr_var_E = 32;
  MaridiaFloater_Func_2(k);
}

void MaridiaFloater_Func_13(uint16 k) {  // 0xA8C59F
  uint16 v5;

  Enemy_MaridiaFloater *E = Get_MaridiaFloater(k);
  bool v2 = E->mfr_var_E == 1;
  bool v3 = (--E->mfr_var_E & 0x8000) != 0;
  if (v2 || v3) {
    int v4 = k >> 1;
    enemy_drawing_queue[v4 + 100] = 0;
    enemy_drawing_queue[v4 + 97] |= 0x200;
    MaridiaFloater_Func_1(k);
    //printf("A undefined!\n");
    v5 = 0;
    EnemyDeathAnimation(k, v5);
  } else {
    MaridiaFloater_Func_2(k);
  }
}

void MaridiaFloater_Touch(void) {  // 0xA8C5BE
  Enemy_MaridiaFloater *E = Get_MaridiaFloater(cur_enemy_index);
  if (!E->mfr_parameter_2) {
    uint16 palette_index = E->base.palette_index;
    NormalEnemyTouchAi();
    if (!E->base.health) {
      enemy_drawing_queue[(cur_enemy_index >> 1) + 97] |= 0x200;
      E->base.palette_index = palette_index;
      MaridiaFloater_Func_1(cur_enemy_index);
      E->base.palette_index = 2560;
    }
  }
}

void MaridiaFloater_Shot(void) {  // 0xA8C5EF
  Enemy_MaridiaFloater *E = Get_MaridiaFloater(cur_enemy_index);
  if (!E->mfr_parameter_2) {
    NormalEnemyShotAiSkipDeathAnim_CurEnemy();
    if ((E->base.ai_handler_bits & 4) != 0) {
      int v2 = cur_enemy_index >> 1;
      enemy_drawing_queue[v2 + 109] = E->base.frozen_timer;
      enemy_drawing_queue[v2 + 99] |= 4;
    }
    if ((E->base.ai_handler_bits & 2) != 0) {
      int v3 = cur_enemy_index >> 1;
      enemy_drawing_queue[v3 + 108] = E->base.flash_timer;
      enemy_drawing_queue[v3 + 99] |= 2;
    }
    if (!E->base.health) {
      E->mfr_var_F = FUNC16(MaridiaFloater_Func_12);
      E->mfr_parameter_2 = 1;
    }
  }
}

void MaridiaFloater_Powerbomb(void) {  // 0xA8C63F
  Enemy_NormalPowerBombAI_A8();
  Enemy_MaridiaFloater *E = Get_MaridiaFloater(cur_enemy_index);
  if (E->base.health) {
    int v1 = cur_enemy_index >> 1;
    enemy_drawing_queue[v1 + 111] = E->base.shake_timer;
    enemy_drawing_queue[v1 + 110] = E->base.invincibility_timer;
    enemy_drawing_queue[v1 + 108] = E->base.flash_timer;
    enemy_drawing_queue[v1 + 109] = E->base.frozen_timer;
    enemy_drawing_queue[v1 + 99] = E->base.ai_handler_bits;
  } else {
    enemy_drawing_queue[(cur_enemy_index >> 1) + 97] |= 0x200;
  }
}

void WreckedShipRobot_Init(void) {  // 0xA8CB77
  if ((*(uint16 *)&boss_bits_for_area[area_index] & 1) != 0) {
    enemy_gfx_drawn_hook.bank = -88;
    enemy_gfx_drawn_hook.addr = FUNC16(WreckedShipRobot_Func_1);
    Enemy_WreckedShipRobot *E = Get_WreckedShipRobot(cur_enemy_index);
    E->base.properties |= 0xA000;
    E->base.instruction_timer = 4;
    E->base.timer = 0;
    E->base.current_instruction = addr_kWreckedShipRobot_Ilist_C6E5;
    E->wsrt_var_A = -512;
    wrecked_ship_robot_palanim_timer = 1;
    *(uint16 *)&wrecked_ship_robot_palanim_table_index = 0;
    E->wsrt_var_B = 0;
    wrecked_ship_robot_palanim_palindex = E->base.palette_index;
  } else {
    WreckedShipRobotDeactivated_Init();
  }
}

void WreckedShipRobotDeactivated_Init(void) {  // 0xA8CBCC
  EnemyData *v0 = gEnemyData(cur_enemy_index);
  uint16 parameter_1 = v0->parameter_1;
  if (sign16(parameter_1) || !sign16(parameter_1 - 4))
    parameter_1 = 0;
  v0->parameter_1 = parameter_1;
  v0->current_instruction = g_off_A8CC30[parameter_1];
  v0->properties |= 0x8000;
  v0->instruction_timer = 1;
  v0->timer = 0;
  wrecked_ship_robot_palanim_timer = 0;
  v0->ai_var_E = 0;
  v0->ai_preinstr = 1;
  int v2 = (16 * ((wrecked_ship_robot_palanim_palindex & 0xFF00) >> 8)) >> 1;
  palette_buffer[v2 + 137] = 10;
  palette_buffer[v2 + 138] = 10;
  palette_buffer[v2 + 139] = 10;
  palette_buffer[v2 + 140] = 10;
}

void WreckedShipRobot_Main(void) {  // 0xA8CC36
  Enemy_WreckedShipRobot *E = Get_WreckedShipRobot(cur_enemy_index);
  if (!Enemy_MoveDown(cur_enemy_index, __PAIR32__(E->wsrt_var_F, E->wsrt_var_E))) {
    ++E->base.instruction_timer;
    AddToHiLo(&E->wsrt_var_F, &E->wsrt_var_E, 0x8000);
  }
}

void WreckedShipRobot_Func_1(void) {  // 0xA8CC67
  uint16 i;
  int16 v2;

  if (!door_transition_flag_enemies) {
    if (wrecked_ship_robot_palanim_timer) {
      if (!--wrecked_ship_robot_palanim_timer) {
        int v1;
        for (i = *(uint16 *)&wrecked_ship_robot_palanim_table_index; ; i = 0) {
          v1 = i >> 1;
          v2 = g_word_A8CCC1[v1];
          if (v2 >= 0)
            break;
        }
        int v3 = (16 * ((wrecked_ship_robot_palanim_palindex & 0xFF00) >> 8)) >> 1;
        palette_buffer[v3 + 137] = v2;
        palette_buffer[v3 + 138] = g_word_A8CCC1[v1 + 1];
        palette_buffer[v3 + 139] = g_word_A8CCC1[v1 + 2];
        palette_buffer[v3 + 140] = g_word_A8CCC1[v1 + 3];
        wrecked_ship_robot_palanim_timer = g_word_A8CCC1[v1 + 4];
        *(uint16 *)&wrecked_ship_robot_palanim_table_index = i + 10;
      }
    }
  }
}

void WreckedShipRobot_Func_2(uint16 k) {  // 0xA8CCFF
  Enemy_WreckedShipRobot *E = Get_WreckedShipRobot(k);
  uint16 wsrt_var_B = E->wsrt_var_B;
  if (wsrt_var_B)
    E->wsrt_var_B = wsrt_var_B - 1;
}

const uint16 *WreckedShipRobot_Instr_4(uint16 k, const uint16 *jp) {  // 0xA8CD09
  WreckedShipRobot_Func_2(cur_enemy_index);
  Enemy_WreckedShipRobot *E = Get_WreckedShipRobot(cur_enemy_index);
  E->wsrt_var_A = -512;
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, INT16_SHL16(-4))) {
    E->wsrt_var_B += 8;
    return INSTR_RETURN_ADDR(addr_kWreckedShipRobot_Ilist_C73F);
  } else {
    if (CheckIfEnemyTouchesSamus(cur_enemy_index)) {
      extra_samus_x_subdisplacement = 0;
      extra_samus_x_displacement = -4;
    }
    E->wsrt_var_D = E->base.y_pos;
    uint16 x_pos = E->base.x_pos;
    E->wsrt_var_C = x_pos;
    E->base.x_pos = x_pos - E->base.x_width - E->base.x_width;
    if (Enemy_MoveDown(cur_enemy_index, INT16_SHL16(1))) {
      E->base.x_pos = E->wsrt_var_C;
      E->base.y_pos = E->wsrt_var_D;
    } else {
      E->wsrt_var_B += 8;
      E->base.x_pos = E->wsrt_var_C;
      E->base.y_pos = E->wsrt_var_D;
      E->wsrt_var_A = 512;
      return INSTR_RETURN_ADDR(addr_kWreckedShipRobot_Ilist_CB65);
    }
  }
  return jp;
}

const uint16 *WreckedShipRobot_Instr_9(uint16 k, const uint16 *jp) {  // 0xA8CDA4
  WreckedShipRobot_Func_2(cur_enemy_index);
  Enemy_WreckedShipRobot *E = Get_WreckedShipRobot(cur_enemy_index);
  E->wsrt_var_A = -512;
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, INT16_SHL16(-4))) {
    E->wsrt_var_B += 8;
    return INSTR_RETURN_ADDR(addr_kWreckedShipRobot_Ilist_C73F);
  } else if (CheckIfEnemyTouchesSamus(cur_enemy_index)) {
    extra_samus_x_subdisplacement = 0;
    extra_samus_x_displacement = -4;
  }
  return jp;
}

const uint16 *WreckedShipRobot_Instr_6(uint16 k, const uint16 *jp) {  // 0xA8CDEA
  WreckedShipRobot_Func_2(cur_enemy_index);
  Enemy_WreckedShipRobot *E = Get_WreckedShipRobot(cur_enemy_index);
  E->wsrt_var_A = -512;
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, INT16_SHL16(4))) {
    E->wsrt_var_B += 8;
    return INSTR_RETURN_ADDR(addr_stru_A8C6E9);
  } else {
    if (CheckIfEnemyTouchesSamus(cur_enemy_index)) {
      extra_samus_x_subdisplacement = 0;
      extra_samus_x_displacement = 4;
    }
    E->wsrt_var_D = E->base.y_pos;
    uint16 x_pos = E->base.x_pos;
    E->wsrt_var_C = x_pos;
    E->base.x_pos = E->base.x_width + E->base.x_width + x_pos;
    if (Enemy_MoveDown(cur_enemy_index, INT16_SHL16(1))) {
      E->base.x_pos = E->wsrt_var_C;
      E->base.y_pos = E->wsrt_var_D;
    } else {
      E->wsrt_var_B += 8;
      E->base.x_pos = E->wsrt_var_C;
      E->base.y_pos = E->wsrt_var_D;
      E->wsrt_var_A = -512;
      return INSTR_RETURN_ADDR(addr_kWreckedShipRobot_Ilist_C91B);
    }
  }
  return jp;
}

const uint16 *WreckedShipRobot_Instr_8(uint16 k, const uint16 *jp) {  // 0xA8CE85
  WreckedShipRobot_Func_2(cur_enemy_index);
  Enemy_WreckedShipRobot *E = Get_WreckedShipRobot(cur_enemy_index);
  E->wsrt_var_A = -512;
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, INT16_SHL16(4))) {
    E->wsrt_var_B += 8;
    return INSTR_RETURN_ADDR(addr_stru_A8C6E9);
  } else if (CheckIfEnemyTouchesSamus(cur_enemy_index)) {
    extra_samus_x_subdisplacement = 0;
    extra_samus_x_displacement = 4;
  }
  return jp;
}

const uint16 *WreckedShipRobot_Instr_7(uint16 k, const uint16 *jp) {  // 0xA8CECB
  return INSTR_RETURN_ADDR(addr_kWreckedShipRobot_Ilist_C92D);
}

const uint16 *WreckedShipRobot_Instr_15(uint16 k, const uint16 *jp) {  // 0xA8CECF
  WreckedShipRobot_Func_2(cur_enemy_index);
  Enemy_WreckedShipRobot *E = Get_WreckedShipRobot(cur_enemy_index);
  E->wsrt_var_A = 512;
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, INT16_SHL16(4))) {
    E->wsrt_var_B += 8;
    return INSTR_RETURN_ADDR(addr_kWreckedShipRobot_Ilist_C985);
  } else {
    if (CheckIfEnemyTouchesSamus(cur_enemy_index)) {
      extra_samus_x_subdisplacement = 0;
      extra_samus_x_displacement = 4;
    }
    E->wsrt_var_D = E->base.y_pos;
    uint16 x_pos = E->base.x_pos;
    E->wsrt_var_C = x_pos;
    E->base.x_pos = E->base.x_width + E->base.x_width + x_pos;
    if (Enemy_MoveDown(cur_enemy_index, INT16_SHL16(1))) {
      E->base.x_pos = E->wsrt_var_C;
      E->base.y_pos = E->wsrt_var_D;
    } else {
      E->wsrt_var_B += 8;
      E->base.x_pos = E->wsrt_var_C;
      E->base.y_pos = E->wsrt_var_D;
      E->wsrt_var_A = -512;
      return INSTR_RETURN_ADDR(addr_kWreckedShipRobot_Ilist_C91B);
    }
  }
  return jp;
}

const uint16 *WreckedShipRobot_Instr_18(uint16 k, const uint16 *jp) {  // 0xA8CF6A
  WreckedShipRobot_Func_2(cur_enemy_index);
  Enemy_WreckedShipRobot *E = Get_WreckedShipRobot(cur_enemy_index);
  E->wsrt_var_A = 512;
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, INT16_SHL16(4))) {
    E->wsrt_var_B += 8;
    return INSTR_RETURN_ADDR(addr_kWreckedShipRobot_Ilist_C985);
  } else if (CheckIfEnemyTouchesSamus(cur_enemy_index)) {
    extra_samus_x_subdisplacement = 0;
    extra_samus_x_displacement = 4;
  }
  return jp;
}

const uint16 *WreckedShipRobot_Instr_16(uint16 k, const uint16 *jp) {  // 0xA8CFB0
  WreckedShipRobot_Func_2(cur_enemy_index);
  Enemy_WreckedShipRobot *E = Get_WreckedShipRobot(cur_enemy_index);
  E->wsrt_var_A = 512;
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, INT16_SHL16(-4))) {
    E->wsrt_var_B += 8;
    return INSTR_RETURN_ADDR(addr_stru_A8C6E9);
  } else {
    if (CheckIfEnemyTouchesSamus(cur_enemy_index)) {
      extra_samus_x_subdisplacement = 0;
      extra_samus_x_displacement = -4;
    }
    E->wsrt_var_D = E->base.y_pos;
    uint16 x_pos = E->base.x_pos;
    E->wsrt_var_C = x_pos;
    E->base.x_pos = x_pos - E->base.x_width - E->base.x_width;
    if (Enemy_MoveDown(cur_enemy_index, INT16_SHL16(1))) {
      E->base.y_pos = E->wsrt_var_D;
      E->base.x_pos = E->wsrt_var_C;
    } else {
      E->wsrt_var_B += 8;
      E->base.y_pos = E->wsrt_var_D;
      E->base.x_pos = E->wsrt_var_C;
      E->wsrt_var_A = 512;
      return INSTR_RETURN_ADDR(addr_kWreckedShipRobot_Ilist_CB65);
    }
  }
  return jp;
}

const uint16 *WreckedShipRobot_Instr_17(uint16 k, const uint16 *jp) {  // 0xA8D04B
  WreckedShipRobot_Func_2(cur_enemy_index);
  Enemy_WreckedShipRobot *E = Get_WreckedShipRobot(cur_enemy_index);
  E->wsrt_var_A = 512;
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, INT16_SHL16(-4))) {
    E->wsrt_var_B += 8;
    return INSTR_RETURN_ADDR(addr_stru_A8C6E9);
  } else if (CheckIfEnemyTouchesSamus(cur_enemy_index)) {
    extra_samus_x_subdisplacement = 0;
    extra_samus_x_displacement = -4;
  }
  return jp;
}

const uint16 *WreckedShipRobot_Instr_3(uint16 k, const uint16 *jp) {  // 0xA8D091
  Enemy_WreckedShipRobot *E = Get_WreckedShipRobot(cur_enemy_index);
  if ((int16)(layer1_x_pos - E->base.x_pos) < 0
      && (int16)(layer1_x_pos + 256 - E->base.x_pos) >= 0
      && (int16)(layer1_y_pos - E->base.y_pos) < 0
      && (int16)(layer1_y_pos + 224 - E->base.y_pos) >= 0) {
    QueueSfx2_Max6(0x68);
  }
  return jp;
}

const uint16 *WreckedShipRobot_Instr_10(uint16 k, const uint16 *jp) {  // 0xA8D0C2
  return INSTR_RETURN_ADDR(addr_stru_A8C6E9);
}

const uint16 *WreckedShipRobot_Instr_14(uint16 k, const uint16 *jp) {  // 0xA8D0C6
  return WreckedShipRobot_CommonInstr(k, jp,
      addr_kWreckedShipRobot_Ilist_CB1D, addr_kEproj_WreckedShipRobotLaserUpRight);
}

const uint16 *WreckedShipRobot_Instr_2(uint16 k, const uint16 *jp) {  // 0xA8D0D2
  return WreckedShipRobot_CommonInstr(k, jp,
      addr_kWreckedShipRobot_Ilist_C8D1, addr_kEproj_WreckedShipRobotLaserUpLeft);
}

const uint16 *WreckedShipRobot_CommonInstr(uint16 k, const uint16 *jp, uint16 r50, uint16 r48) {  // 0xA8D0DC
  Enemy_WreckedShipRobot *E = Get_WreckedShipRobot(k);
  if (E->wsrt_var_B) {
    WreckedShipRobot_Func_2(k);
  } else {
    uint16 v3 = (random_number & 0x1F) + 16;
    E->wsrt_var_B = v3;
    SpawnEnemyProjectileWithGfx(v3, cur_enemy_index, r48);
    return INSTR_RETURN_ADDR(r50);
  }
  return jp;
}

const uint16 *WreckedShipRobot_Instr_13(uint16 k, const uint16 *jp) {  // 0xA8D100
  return WreckedShipRobot_D10C(k, jp, addr_kWreckedShipRobot_Ilist_CB09);
}

const uint16 *WreckedShipRobot_Instr_1(uint16 k, const uint16 *jp) {  // 0xA8D107
  return WreckedShipRobot_D10C(k, jp, addr_kWreckedShipRobot_Ilist_C8BD);
}

const uint16 *WreckedShipRobot_D10C(uint16 k, const uint16 *jp, uint16 r50) {  // 0xA8D10C
  Enemy_WreckedShipRobot *E = Get_WreckedShipRobot(k);
  if (E->wsrt_var_B) {
    WreckedShipRobot_Func_2(k);
  } else {
    uint16 v3 = (random_number & 0x1F) + 16;
    E->wsrt_var_B = v3;
    SpawnEnemyProjectileWithGfx(v3, cur_enemy_index, addr_kEproj_WreckedShipRobotLaserHorizontal);
    return INSTR_RETURN_ADDR(r50);
  }
  return jp;
}

const uint16 *WreckedShipRobot_Instr_12(uint16 k, const uint16 *jp) {  // 0xA8D131
  return WreckedShipRobot_D147(k, jp,
      addr_kEproj_WreckedShipRobotLaserDownRight, addr_kWreckedShipRobot_Ilist_CAFD);
}

const uint16 *WreckedShipRobot_Instr_5(uint16 k, const uint16 *jp) {  // 0xA8D13D
  return WreckedShipRobot_D147(k, jp,
      addr_kEproj_WreckedShipRobotLaserDownLeft, addr_kWreckedShipRobot_Ilist_C8B1);
}

const uint16 *WreckedShipRobot_D147(uint16 k, const uint16 *jp, uint16 r48, uint16 r50) {  // 0xA8D147
  Enemy_WreckedShipRobot *E = Get_WreckedShipRobot(k);
  if (E->wsrt_var_B) {
    WreckedShipRobot_Func_2(k);
  } else {
    uint16 v3 = (random_number & 0x1F) + 16;
    E->wsrt_var_B = v3;
    SpawnEnemyProjectileWithGfx(v3, cur_enemy_index, r48);
    return INSTR_RETURN_ADDR(r50);
  }
  return jp;
}

const uint16 *WreckedShipRobot_Instr_11(uint16 k, const uint16 *jp) {  // 0xA8D16B
  WreckedShipRobot_Func_2(cur_enemy_index);
  return jp;
}

void WreckedShipRobotDeactivated_Touch(void) {  // 0xA8D174
  if ((int16)(gEnemyData(cur_enemy_index)->x_pos - samus_x_pos) < 0)
    extra_samus_x_displacement = 4;
  else
    extra_samus_x_displacement = -4;
}

void WreckedShipRobotDeactivated_Shot(void) {  // 0xA8D18D
  NormalEnemyShotAi();
}

void WreckedShipRobot_Shot(void) {  // 0xA8D192
  if ((*(uint16 *)&boss_bits_for_area[area_index] & 1) != 0) {
    NormalEnemyShotAi();
    Enemy_WreckedShipRobot *E = Get_WreckedShipRobot(cur_enemy_index);
    if (E->base.health) {
      uint16 v1;
      if ((E->wsrt_var_A & 0x8000) != 0) {
        if ((int16)(samus_x_pos - E->base.x_pos) >= 0)
          v1 = addr_kWreckedShipRobot_Ilist_C833;
        else
          v1 = addr_kWreckedShipRobot_Ilist_C7BB;
      } else if ((int16)(samus_x_pos - E->base.x_pos) < 0) {
        v1 = addr_kWreckedShipRobot_Ilist_CA7D;
      } else {
        v1 = addr_kWreckedShipRobot_Ilist_CA01;
      }
      E->base.current_instruction = v1;
      E->base.instruction_timer = 1;
      E->wsrt_var_B += 64;
    }
  }
}

void MaridiaPuffer_Init(void) {  // 0xA8D8C9
  Enemy_MaridiaPuffer *E = Get_MaridiaPuffer(cur_enemy_index);
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->base.current_instruction = addr_kMaridiaPuffer_Ilist_D841;
  int v1 = (uint16)(4 * E->mpr_parameter_1) >> 1;
  uint16 v2 = g_word_A8D895[v1];
  E->mpr_var_05 = v2;
  E->mpr_var_F = v2;
  E->mpr_var_06 = g_word_A8D895[v1 + 1];
  E->mpr_var_E = 16;
  E->mpr_var_A = FUNC16(MaridiaPuffer_Func_1);
  E->mpr_var_00 = g_word_A8D885[E->mpr_parameter_2];
}

void CallMaridiaPuffer(uint32 ea) {
  switch (ea) {
  case fnMaridiaPuffer_Func_1: MaridiaPuffer_Func_1(); return;
  case fnMaridiaPuffer_Func_2: MaridiaPuffer_Func_2(); return;
  case fnMaridiaPuffer_Func_3: MaridiaPuffer_Func_3(); return;
  case fnMaridiaPuffer_Func_4: MaridiaPuffer_Func_4(); return;
  default: Unreachable();
  }
}

void MaridiaPuffer_Main(void) {  // 0xA8D90B
  Enemy_MaridiaPuffer *E = Get_MaridiaPuffer(cur_enemy_index);
  uint16 v1 = E->mpr_var_07 - 1;
  E->mpr_var_07 = v1;
  if (!v1) {
    E->mpr_var_07 = 1;
    E->mpr_var_03 = 0;
  }
  CallMaridiaPuffer(E->mpr_var_A | 0xA80000);
}

void MaridiaPuffer_Func_1(void) {  // 0xA8D92B
  Enemy_MaridiaPuffer *E = Get_MaridiaPuffer(cur_enemy_index);
  if (E->mpr_var_E-- == 1) {
    E->mpr_var_E = 16;
    E->mpr_var_A = FUNC16(MaridiaPuffer_Func_2);
  }
}

void MaridiaPuffer_Func_2(void) {  // 0xA8D940
  uint8 v1 = CalculateAngleOfSamusFromEnemy(cur_enemy_index) - 64;
  Enemy_MaridiaPuffer *E = Get_MaridiaPuffer(cur_enemy_index);
  E->mpr_var_01 = v1;
  E->mpr_var_02 = v1;
  E->mpr_var_A = FUNC16(MaridiaPuffer_Func_3);
  E->mpr_var_C = 24;
}

void MaridiaPuffer_Func_3(void) {  // 0xA8D963
  Enemy_MaridiaPuffer *E = Get_MaridiaPuffer(cur_enemy_index);
  if ((int16)(E->mpr_var_D - E->mpr_var_00) < 0)
    MaridiaPuffer_Func_8(cur_enemy_index);
  MaridiaPuffer_Func_6();
  MaridiaPuffer_Func_7();
  MaridiaPuffer_Func_5();
}

void MaridiaPuffer_Func_4(void) {  // 0xA8D97C
  Enemy_MaridiaPuffer *E = Get_MaridiaPuffer(cur_enemy_index);
  if ((int16)E->mpr_var_D > 0 && (int16)E->mpr_var_B > 0) {
    MaridiaPuffer_Func_6();
    MaridiaPuffer_Func_7();
    MaridiaPuffer_Func_9(cur_enemy_index);
  } else {
    E->mpr_var_A = FUNC16(MaridiaPuffer_Func_1);
    E->mpr_var_B = 0;
    E->mpr_var_C = 0;
    E->mpr_var_D = 0;
  }
}

void MaridiaPuffer_Func_5(void) {  // 0xA8D9AA
  uint8 v1 = CalculateAngleOfSamusFromEnemy(cur_enemy_index) - 64;
  Enemy_MaridiaPuffer *E = Get_MaridiaPuffer(cur_enemy_index);
  E->mpr_var_01 = v1;
  uint16 v3 = SignExtend8(v1 - E->mpr_var_02);
  uint16 v4 = Abs16(v3);
  if (!sign16(v4 - 48)) {
    E->mpr_var_A = FUNC16(MaridiaPuffer_Func_4);
    E->mpr_var_C = 24;
  }
}

void MaridiaPuffer_Func_6(void) {  // 0xA8D9DB
  Enemy_MaridiaPuffer *E = Get_MaridiaPuffer(cur_enemy_index);
  uint16 R26 = 0;
  uint16 r18 = kSine16bit[(uint8)(E->mpr_var_02 + 64)];
  if ((r18 & 0x8000) != 0)
    ++R26;
  uint32 t = ((Abs16(r18) & 0xFF00) >> 8) * E->mpr_var_D;
  if (R26)
    t = -(int32)t;
  AddToHiLo(&E->base.x_pos, &E->base.x_subpos, t);
}

void MaridiaPuffer_Func_7(void) {  // 0xA8DA28
  Enemy_MaridiaPuffer *E = Get_MaridiaPuffer(cur_enemy_index);
  uint16 R26 = 0;
  uint16 r18 = kSine16bit[(uint8)E->mpr_var_02];
  if ((r18 & 0x8000) != 0)
    ++R26;
  uint32 t = ((Abs16(r18) & 0xFF00) >> 8) * E->mpr_var_D;
  if (R26)
    t = -(int32)t;
  AddToHiLo(&E->base.y_pos, &E->base.y_subpos, t);
}

void MaridiaPuffer_Func_8(uint16 k) {  // 0xA8DA71
  Enemy_MaridiaPuffer *E = Get_MaridiaPuffer(k);
  if (E->mpr_var_F-- == 1) {
    E->mpr_var_F = E->mpr_var_05;
    E->mpr_var_B += E->mpr_var_C;
    E->mpr_var_D += E->mpr_var_B;
  }
}

void MaridiaPuffer_Func_9(uint16 k) {  // 0xA8DA92
  Enemy_MaridiaPuffer *E = Get_MaridiaPuffer(k);
  if (E->mpr_var_F-- == 1) {
    E->mpr_var_F = E->mpr_var_06;
    E->mpr_var_B -= E->mpr_var_C;
    E->mpr_var_D -= E->mpr_var_B;
  }
}

void MaridiaPuffer_Shot(void) {  // 0xA8DB14
  Enemy_MaridiaPuffer *E = Get_MaridiaPuffer(cur_enemy_index);
  E->mpr_var_40 = E->base.health;
  Enemy_NormalShotAI_A8();
  if (E->base.health == E->mpr_var_40 && !E->mpr_var_03) {
    E->base.instruction_timer = 1;
    E->base.timer = 0;
    E->base.current_instruction = addr_kMaridiaPuffer_Ilist_D855;
    E->mpr_var_02 = g_word_A8D871[projectile_dir[collision_detection_index] & 0xF];
    E->mpr_var_B = 256;
    E->mpr_var_D = 1536;
    E->mpr_var_A = FUNC16(MaridiaPuffer_Func_4);
    E->mpr_var_07 = 48;
    E->mpr_var_03 = 1;
  }
}

void WalkingLavaSeahorse_Init(void) {  // 0xA8DCCD
  Enemy_WalkingLavaSeahorse *E = Get_WalkingLavaSeahorse(cur_enemy_index);
  E->wlse_var_04 = 0;
  E->wlse_var_F = E->base.y_pos;
  E->wlse_var_02 = E->base.x_pos;
  E->base.properties |= kEnemyProps_DisableSamusColl;
  E->base.current_instruction = addr_kWalkingLavaSeahorse_Ilist_DBE7;
  E->wlse_var_A = FUNC16(WalkingLavaSeahorse_Func_3);
  WalkingLavaSeahorse_Func_1(cur_enemy_index);
  do {
    AddToHiLo(&E->base.y_pos, &E->base.y_subpos, __PAIR32__(E->wlse_var_B, E->wlse_var_C));
  } while ((WalkingLavaSeahorse_Func_2(cur_enemy_index) & 0x8000) != 0);
  while (1) {
    if (Enemy_MoveDown(cur_enemy_index, __PAIR32__(E->wlse_var_B, E->wlse_var_C)))
      break;
    WalkingLavaSeahorse_Func_2(cur_enemy_index);
  }
  E->wlse_var_03 = E->base.y_pos;
  E->base.y_pos = E->wlse_var_F;
}

void WalkingLavaSeahorse_Func_1(uint16 k) {  // 0xA8DD37
  Enemy_WalkingLavaSeahorse *E = Get_WalkingLavaSeahorse(k);
  E->wlse_var_B = -12;
  E->wlse_var_C = 0;
  E->wlse_var_D = 0;
  E->wlse_var_E = 0;
  E->wlse_var_00 = 0;
  E->wlse_var_01 = 0x8000;
}

uint16 WalkingLavaSeahorse_Func_2(uint16 k) {  // 0xA8DD55
  Enemy_WalkingLavaSeahorse *E = Get_WalkingLavaSeahorse(k);
  AddToHiLo(&E->wlse_var_B, &E->wlse_var_C, __PAIR32__(E->wlse_var_00, E->wlse_var_01));
  return E->wlse_var_B;
}

void CallWalkingLavaSeahorseFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnnullsub_257: return;
  case fnWalkingLavaSeahorse_Func_3: WalkingLavaSeahorse_Func_3(k); return;
  case fnWalkingLavaSeahorse_Func_4: WalkingLavaSeahorse_Func_4(k); return;
  case fnWalkingLavaSeahorse_Func_6: WalkingLavaSeahorse_Func_6(k); return;
  case fnWalkingLavaSeahorse_Func_7: WalkingLavaSeahorse_Func_7(k); return;
  case fnWalkingLavaSeahorse_Func_8: WalkingLavaSeahorse_Func_8(k); return;
  case fnWalkingLavaSeahorse_Func_9: WalkingLavaSeahorse_Func_9(k); return;
  default: Unreachable();
  }
}

void WalkingLavaSeahorse_Main(void) {  // 0xA8DD6B
  Enemy_WalkingLavaSeahorse *E = Get_WalkingLavaSeahorse(cur_enemy_index);
  CallWalkingLavaSeahorseFunc(E->wlse_var_A | 0xA80000, cur_enemy_index);
}

void WalkingLavaSeahorse_Func_3(uint16 k) {  // 0xA8DD71
  uint16 v4;
  int16 v5;

  Enemy_WalkingLavaSeahorse *E = Get_WalkingLavaSeahorse(k);
  uint16 xxx;
  uint16 v3 = abs16(E->wlse_var_03 - samus_y_pos);
  if (sign16(v3 - 32) && (int16)(abs16((xxx = samus_x_pos - E->base.x_pos)) - g_word_A8DCC7) < 0) {
    WalkingLavaSeahorse_Func_1(k);
    v4 = addr_stru_A8DC4B;
    v5 = -2;
    if (!sign16(xxx)) {
      v4 = addr_stru_A8DCBB;
      v5 = 2;
    }
    E->wlse_var_D = v5;
    E->base.current_instruction = v4;
    E->base.instruction_timer = 1;
    E->wlse_var_A = FUNC16(WalkingLavaSeahorse_Func_4);
    QueueSfx2_Max6(0x5E);
  }
}

void WalkingLavaSeahorse_Func_4(uint16 k) {  // 0xA8DDC6
  Enemy_WalkingLavaSeahorse *E = Get_WalkingLavaSeahorse(k);
  AddToHiLo(&E->base.y_pos, &E->base.y_subpos, __PAIR32__(E->wlse_var_B, E->wlse_var_C));
  if ((WalkingLavaSeahorse_Func_2(k) & 0x8000) == 0)
    sub_A8DDDE(k);
}

void sub_A8DDDE(uint16 k) {  // 0xA8DDDE
  EnemyData *v1 = gEnemyData(k);
  v1->ai_var_A = FUNC16(WalkingLavaSeahorse_Func_6);
  uint16 v2 = addr_kWalkingLavaSeahorse_Ilist_DC51;
  if ((v1->ai_var_D & 0x8000) == 0)
    v2 = addr_kWalkingLavaSeahorse_Ilist_DCC1;
  v1->current_instruction = v2;
  v1->instruction_timer = 1;
}

void WalkingLavaSeahorse_Func_5(uint16 k) {  // 0xA8DDFA
  Enemy_WalkingLavaSeahorse *E = Get_WalkingLavaSeahorse(k);
  E->wlse_var_B = 0;
  E->wlse_var_C = 0;
  sub_A8DDDE(k);
}

void WalkingLavaSeahorse_Func_6(uint16 k) {  // 0xA8DE05
  int16 v3;

  Enemy_WalkingLavaSeahorse *E = Get_WalkingLavaSeahorse(k);
  if (Enemy_MoveDown(k, __PAIR32__(E->wlse_var_B, E->wlse_var_C))) {
    int t = samus_x_pos - E->base.x_pos;
    uint16 v2 = addr_kWalkingLavaSeahorse_Ilist_DBE7;
    v3 = -2;
    if (!sign16(t)) {
      v3 = 2;
      v2 = addr_kWalkingLavaSeahorse_Ilist_DC57;
    }
    E->wlse_var_D = v3;
    E->base.current_instruction = v2;
    E->base.instruction_timer = 1;
    E->wlse_var_A = FUNC16(WalkingLavaSeahorse_Func_7);
    E->wlse_var_04 = 1;
  } else {
    WalkingLavaSeahorse_Func_2(k);
  }
}

void WalkingLavaSeahorse_Func_7(uint16 k) {  // 0xA8DE4B
  Enemy_MoveDown(k, INT16_SHL16(2));
  Enemy_WalkingLavaSeahorse *E = Get_WalkingLavaSeahorse(k);
  if ((int16)(abs16(E->wlse_var_02 - E->base.x_pos) - g_word_A8DCCB) >= 0) {
    E->wlse_var_A = FUNC16(WalkingLavaSeahorse_Func_8);
    E->wlse_var_B = -4;
    E->wlse_var_C = 0;
    uint16 v4 = addr_kWalkingLavaSeahorse_Ilist_DC51;
    if ((E->wlse_var_D & 0x8000) == 0)
      v4 = addr_kWalkingLavaSeahorse_Ilist_DCC1;
    E->base.current_instruction = v4;
    E->base.instruction_timer = 1;
  } else {
    if (E->wlse_var_04)
      return;
    int t = samus_x_pos - E->base.x_pos;
    uint16 v3;
    if (!sign16(t)) {
      v3 = addr_kWalkingLavaSeahorse_Ilist_DC73;
      if ((E->wlse_var_D & 0x8000) != 0)
        return;
    } else {
      v3 = addr_kWalkingLavaSeahorse_Ilist_DC03;
      if ((E->wlse_var_D & 0x8000) == 0)
        return;
    }
    E->base.current_instruction = v3;
    E->base.instruction_timer = 1;
    E->wlse_var_A = FUNC16(nullsub_257);
  }
}

void WalkingLavaSeahorse_Func_8(uint16 k) {  // 0xA8DECD
  Enemy_WalkingLavaSeahorse *E = Get_WalkingLavaSeahorse(k);
  AddToHiLo(&E->base.y_pos, &E->base.y_subpos, __PAIR32__(E->wlse_var_B, E->wlse_var_C));
  if ((WalkingLavaSeahorse_Func_2(k) & 0x8000) == 0)
    E->wlse_var_A = FUNC16(WalkingLavaSeahorse_Func_9);
}

void WalkingLavaSeahorse_Func_9(uint16 k) {  // 0xA8DEEC
  Enemy_WalkingLavaSeahorse *E = Get_WalkingLavaSeahorse(k);
  AddToHiLo(&E->base.y_pos, &E->base.y_subpos, __PAIR32__(E->wlse_var_B, E->wlse_var_C));
  if ((int16)(E->base.y_pos - E->wlse_var_F) >= 0) {
    E->base.y_pos = E->wlse_var_F;
    E->base.x_pos = E->wlse_var_02;
    E->wlse_var_A = FUNC16(WalkingLavaSeahorse_Func_3);
  } else {
    WalkingLavaSeahorse_Func_2(k);
  }
}

const uint16 *WalkingLavaSeahorse_Instr_4(uint16 k, const uint16 *jp) {  // 0xA8DF1C
  WalkingLavaSeahorse_DF20(0);
  return jp;
}

void WalkingLavaSeahorse_DF20(uint16 a) {  // 0xA8DF20
  SpawnEnemyProjectileWithGfx(a, cur_enemy_index, addr_loc_A89E90);
  QueueSfx2_Max6(0x3F);
}

const uint16 *WalkingLavaSeahorse_Instr_3(uint16 k, const uint16 *jp) {  // 0xA8DF33
  WalkingLavaSeahorse_DF20(2);
  return jp;
}

const uint16 *WalkingLavaSeahorse_Instr_5(uint16 k, const uint16 *jp) {  // 0xA8DF39
  WalkingLavaSeahorse_DF20(4);
  return jp;
}

const uint16 *WalkingLavaSeahorse_Instr_6(uint16 k, const uint16 *jp) {  // 0xA8DF3F
  int16 v3;

  Enemy_WalkingLavaSeahorse *E = Get_WalkingLavaSeahorse(cur_enemy_index);
  E->wlse_var_A = FUNC16(WalkingLavaSeahorse_Func_7);
  v3 = random_number & 3;
  if ((random_number & 3) == 0)
    v3 = 2;
  E->wlse_var_04 = v3;
  if ((E->wlse_var_D & 0x8000) == 0)
    return INSTR_RETURN_ADDR(addr_kWalkingLavaSeahorse_Ilist_DC57);
  return INSTR_RETURN_ADDR(addr_kWalkingLavaSeahorse_Ilist_DBE7);
}

const uint16 *WalkingLavaSeahorse_Instr_2(uint16 k, const uint16 *jp) {  // 0xA8DF63
  Enemy_WalkingLavaSeahorse *E = Get_WalkingLavaSeahorse(cur_enemy_index);
  uint16 wlse_var_04 = E->wlse_var_04;
  if (wlse_var_04)
    E->wlse_var_04 = wlse_var_04 - 1;
  return WalkingLavaSeahorse_Instr_1(cur_enemy_index, jp);
}

const uint16 *WalkingLavaSeahorse_Instr_1(uint16 k, const uint16 *jp) {  // 0xA8DF71
  int16 wlse_var_D;

  Enemy_WalkingLavaSeahorse *E = Get_WalkingLavaSeahorse(cur_enemy_index);
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, INT16_SHL16(E->wlse_var_D))) {
    jp = INSTR_RETURN_ADDR(addr_stru_A8DBE9);
    wlse_var_D = E->wlse_var_D;
    if (wlse_var_D < 0)
      jp = INSTR_RETURN_ADDR(addr_stru_A8DC59);
    E->wlse_var_D = -wlse_var_D;
  } else {
    EnemyFunc_C8AD(cur_enemy_index);
  }
  return jp;
}

void WalkingLavaSeahorse_Func_10(void) {  // 0xA8DF9D
  NormalEnemyShotAi();
}

void WreckedShipOrbs_Init(void) {  // 0xA8E388
  Enemy_WreckedShipOrbs *E = Get_WreckedShipOrbs(cur_enemy_index);
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->base.current_instruction = g_off_A8E380[E->wsos_parameter_1];
  int v1 = (8 * E->wsos_parameter_2) >> 1;
  E->wsos_var_01 = kCommonEnemySpeeds_Linear[v1];
  E->wsos_var_00 = kCommonEnemySpeeds_Linear[v1 + 1];
  E->wsos_var_03 = kCommonEnemySpeeds_Linear[v1 + 2];
  E->wsos_var_02 = kCommonEnemySpeeds_Linear[v1 + 3];
}

void CallWreckedShipOrbsA(uint32 ea) {
  switch (ea) {
  case fnWreckedShipOrbs_Func_3: WreckedShipOrbs_Func_3(); return;
  case fnWreckedShipOrbs_Func_4: WreckedShipOrbs_Func_4(); return;
  default: Unreachable();
  }
}
void CallWreckedShipOrbsB(uint32 ea) {
  switch (ea) {
  case fnWreckedShipOrbs_Func_5: WreckedShipOrbs_Func_5(); return;
  case fnWreckedShipOrbs_Func_6: WreckedShipOrbs_Func_6(); return;
  default: Unreachable();
  }
}
void WreckedShipOrbs_Main(void) {  // 0xA8E3C3
  WreckedShipOrbs_Func_1();
  WreckedShipOrbs_Func_2();
  Enemy_WreckedShipOrbs *E = Get_WreckedShipOrbs(cur_enemy_index);
  CallWreckedShipOrbsA(E->wsos_var_A | 0xA80000);
  CallWreckedShipOrbsB(E->wsos_var_B | 0xA80000);
}

void WreckedShipOrbs_Func_1(void) {  // 0xA8E3D9
  Enemy_WreckedShipOrbs *E = Get_WreckedShipOrbs(cur_enemy_index);
  E->wsos_var_A = FUNC16(WreckedShipOrbs_Func_3);
  if ((GetSamusEnemyDelta_Y(cur_enemy_index) & 0x8000) == 0)
    E->wsos_var_A = FUNC16(WreckedShipOrbs_Func_4);
}

void WreckedShipOrbs_Func_2(void) {  // 0xA8E3EF
  Enemy_WreckedShipOrbs *E = Get_WreckedShipOrbs(cur_enemy_index);
  E->wsos_var_B = FUNC16(WreckedShipOrbs_Func_5);
  if ((GetSamusEnemyDelta_X(cur_enemy_index) & 0x8000) == 0)
    E->wsos_var_B = FUNC16(WreckedShipOrbs_Func_6);
}

void WreckedShipOrbs_Func_3(void) {  // 0xA8E405
  Enemy_WreckedShipOrbs *E = Get_WreckedShipOrbs(cur_enemy_index);
  AddToHiLo(&E->base.y_pos, &E->base.y_subpos, __PAIR32__(E->wsos_var_03, E->wsos_var_02));
}

void WreckedShipOrbs_Func_4(void) {  // 0xA8E424
  Enemy_WreckedShipOrbs *E = Get_WreckedShipOrbs(cur_enemy_index);
  AddToHiLo(&E->base.y_pos, &E->base.y_subpos, __PAIR32__(E->wsos_var_01, E->wsos_var_00));
}

void WreckedShipOrbs_Func_5(void) {  // 0xA8E443
  Enemy_WreckedShipOrbs *E = Get_WreckedShipOrbs(cur_enemy_index);
  AddToHiLo(&E->base.x_pos, &E->base.x_subpos, __PAIR32__(E->wsos_var_03, E->wsos_var_02));
}

void WreckedShipOrbs_Func_6(void) {  // 0xA8E462
  Enemy_WreckedShipOrbs *E = Get_WreckedShipOrbs(cur_enemy_index);
  AddToHiLo(&E->base.x_pos, &E->base.x_subpos, __PAIR32__(E->wsos_var_01, E->wsos_var_00));
}

const uint16 *WreckedShipSpark_Instr_2(uint16 k, const uint16 *jp) {  // 0xA8E61D
  Enemy_WreckedShipSpark *E = Get_WreckedShipSpark(cur_enemy_index);
  E->base.properties |= kEnemyProps_Tangible;
  return jp;
}

const uint16 *WreckedShipSpark_Instr_1(uint16 k, const uint16 *jp) {  // 0xA8E62A
  Enemy_WreckedShipSpark *E = Get_WreckedShipSpark(cur_enemy_index);
  E->base.properties &= ~kEnemyProps_Tangible;
  return jp;
}


static void WreckedShipSpark_Func_4(uint16 k, uint16 r18) {  // 0xA8E6F6
  int16 wssk_var_E;

  Enemy_WreckedShipSpark *E = Get_WreckedShipSpark(k);
  wssk_var_E = E->wssk_var_E;
  if (wssk_var_E < 0)
    wssk_var_E = (NextRandom() & 0x3F) + 4;
  E->wssk_var_F = r18 + wssk_var_E;
}

void WreckedShipSpark_Init(void) {  // 0xA8E637
  Enemy_WreckedShipSpark *E = Get_WreckedShipSpark(cur_enemy_index);
  int v2 = E->wssk_parameter_1 & 3;
  E->wssk_var_B = g_off_A8E688[v2];
  E->wssk_var_E = E->wssk_parameter_2;
  WreckedShipSpark_Func_4(cur_enemy_index, 0);
  E->base.instruction_timer = 1;
  E->base.current_instruction = g_off_A8E682[v2];
  E->base.timer = 0;
  E->base.instruction_timer = 1;
  if ((boss_bits_for_area[area_index] & 1) == 0) {
    E->base.properties |= *(uint16 *)((uint8 *)&gVramWriteEntry(0)[6].vram_dst + 1);
  }
}

void CallWreckedShipSpark(uint32 ea, uint16 k) {
  switch (ea) {
  case fnWreckedShipSpark_Func_1: WreckedShipSpark_Func_1(k); return;
  case fnWreckedShipSpark_Func_2: WreckedShipSpark_Func_2(k); return;
  case fnWreckedShipSpark_Func_3: WreckedShipSpark_Func_3(k); return;
  case fnnullsub_259: return;
  default: Unreachable();
  }
}

void WreckedShipSpark_Main(void) {  // 0xA8E68E
  Enemy_WreckedShipSpark *E = Get_WreckedShipSpark(cur_enemy_index);
  CallWreckedShipSpark(E->wssk_var_B | 0xA80000, cur_enemy_index);
}

void WreckedShipSpark_Func_1(uint16 k) {  // 0xA8E695
  Enemy_WreckedShipSpark *E = Get_WreckedShipSpark(k);
  if (E->wssk_var_F == 1) {
    E->wssk_var_B = FUNC16(WreckedShipSpark_Func_2);
    E->base.current_instruction = addr_kWreckedShipSpark_Ilist_E5A7;
    E->base.instruction_timer = 1;
    WreckedShipSpark_Func_4(k, 0);
  } else {
    --E->wssk_var_F;
  }
}

void WreckedShipSpark_Func_2(uint16 k) {  // 0xA8E6B7
  Enemy_WreckedShipSpark *E = Get_WreckedShipSpark(k);
  if (E->wssk_var_F == 1) {
    E->wssk_var_B = FUNC16(WreckedShipSpark_Func_1);
    E->base.current_instruction = addr_kWreckedShipSpark_Ilist_E5E5;
    E->base.instruction_timer = 1;
    WreckedShipSpark_Func_4(k, 8);
  } else {
    --E->wssk_var_F;
  }
}

void WreckedShipSpark_Func_3(uint16 k) {  // 0xA8E6DC
  Enemy_WreckedShipSpark *E = Get_WreckedShipSpark(k);
  if (E->wssk_var_F == 1) {
    uint16 v2 = cur_enemy_index;
    SpawnEnemyProjectileWithGfx(0, cur_enemy_index, addr_kEproj_Sparks);
    WreckedShipSpark_Func_4(v2, 0);
  } else {
    --E->wssk_var_F;
  }
}


void WreckedShipSpark_Shot(void) {  // 0xA8E70E
  projectile_dir[collision_detection_index] &= ~0x10;
}

void BlueBrinstarFaceBlock_Init(void) {  // 0xA8E82E
  EnemyData *v0 = gEnemyData(cur_enemy_index);
  v0->current_instruction = addr_kBlueBrinstarFaceBlock_Ilist_E828;
  uint16 v1 = FUNC16(BlueBrinstarFaceBlock_Func_1);
  if ((collected_items & 4) == 0)
    v1 = FUNC16(nullsub_170_A8);
  enemy_gfx_drawn_hook.addr = v1;
  *(uint16 *)&enemy_gfx_drawn_hook.bank = 168;
  variables_for_enemy_graphics_drawn_hook[0] = ((16 * v0->palette_index) & 0xFF00) >> 8;
  variables_for_enemy_graphics_drawn_hook[2] = 16;
  v0->parameter_2 = ((v0->parameter_2 & 1) >> 2) | ((v0->parameter_2 & 1) << 15);
}

void BlueBrinstarFaceBlock_Func_1(void) {  // 0xA8E86E
  if (!door_transition_flag_enemies && !--variables_for_enemy_graphics_drawn_hook[2]) {
    variables_for_enemy_graphics_drawn_hook[2] = 16;
    uint16 v0 = variables_for_enemy_graphics_drawn_hook[0];
    uint16 v1 = 8 * variables_for_enemy_graphics_drawn_hook[1];
    int n = 4;
    do {
      palette_buffer[(v0 >> 1) + 137] = g_word_A8E7CC[v1 >> 1];
      v1 += 2;
      v0 += 2;
    } while (--n);
    variables_for_enemy_graphics_drawn_hook[1] = (LOBYTE(variables_for_enemy_graphics_drawn_hook[1]) + 1) & 7;
  }
}

void BlueBrinstarFaceBlock_Main(void) {  // 0xA8E8AE
  if ((collected_items & 4) != 0) {
    enemy_gfx_drawn_hook.addr = FUNC16(BlueBrinstarFaceBlock_Func_1);
    EnemyData *v1 = gEnemyData(cur_enemy_index);
    variables_for_enemy_graphics_drawn_hook[0] = ((16 * v1->palette_index) & 0xFF00) >> 8;
    if (!v1->ai_var_A) {
      uint16 SamusEnemyDelta_Y = GetSamusEnemyDelta_Y(cur_enemy_index);
      if ((int16)(Abs16(SamusEnemyDelta_Y) - v1->parameter_1) < 0) {
        uint16 SamusEnemyDelta_X = GetSamusEnemyDelta_X(cur_enemy_index);
        v1->ai_var_B = SamusEnemyDelta_X;
        if ((int16)(Abs16(SamusEnemyDelta_X) - v1->parameter_1) < 0 && (v1->ai_var_B & 0x8000) != v1->parameter_2) {
          uint16 v4 = addr_kBlueBrinstarFaceBlock_Ilist_E80C;
          if ((v1->ai_var_B & 0x8000) == 0)
            v4 = addr_kBlueBrinstarFaceBlock_Ilist_E81A;
          v1->current_instruction = v4;
          v1->instruction_timer = 1;
          v1->ai_var_A = 1;
          variables_for_enemy_graphics_drawn_hook[2] = 16;
        }
      }
    }
  }
}

void BlueBrinstarFaceBlock_Shot(void) {  // 0xA8E91D
  projectile_dir[collision_detection_index] &= ~0x10;
}

void KiHunter_Init(void) {  // 0xA8F188
  Enemy_KiHunter *E = Get_KiHunter(cur_enemy_index);
  E->base.properties |= kEnemyProps_DisableSamusColl;
  E->khr_var_14 = 0;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->base.current_instruction = addr_kKiHunter_Ilist_E9FA;
  E->khr_var_F = 0;
  E->khr_var_A = FUNC16(KiHunter_Func_1);
  E->khr_var_08 = 0;
  E->khr_var_09 = 1;
  E->khr_var_06 = 0;
  E->khr_var_07 = -1;
  uint16 v1 = E->base.y_pos - 16;
  E->khr_var_0A = v1;
  E->khr_var_0B = v1 + 32;
  E->khr_var_0C = E->base.x_pos;
  E->khr_var_0D = E->base.y_pos;
  if ((E->khr_parameter_1 & 0x8000) != 0) {
    E->khr_var_14 = 1;
    E->khr_var_A = FUNC16(KiHunter_Func_4);
    E->khr_var_08 = 0;
    E->khr_var_09 = 1;
  }
}

void KiHunterWings_Init(void) {  // 0xA8F214
  Enemy_KiHunter *E = Get_KiHunter(cur_enemy_index);
  E->base.properties |= kEnemyProps_DisableSamusColl;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->base.current_instruction = addr_kKiHunter_Ilist_EA4E;
  int v2 = cur_enemy_index >> 1;
  E->base.y_pos = enemy_drawing_queue[v2 + 93];
  E->base.x_pos = enemy_drawing_queue[v2 + 91];
  E->khr_var_A = FUNC16(KiHunter_Func_9);
  E->base.palette_index = enemy_drawing_queue[v2 + 105];
  E->base.vram_tiles_index = enemy_drawing_queue[v2 + 106];
  if ((enemy_drawing_queue_sizes[v2 + 6] & 0x8000) != 0)
    E->base.properties |= kEnemyProps_Deleted;
}

void CallKiHunterFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnKiHunter_Func_1: KiHunter_Func_1(k); return;  // 0xa8f268
  case fnKiHunter_Func_2: KiHunter_Func_2(k); return;  // 0xa8f3b8
  case fnKiHunter_Func_3: KiHunter_Func_3(k); return;  // 0xa8f4ed
  case fnKiHunter_Func_4: KiHunter_Func_4(k); return;  // 0xa8f55a
  case fnKiHunter_Func_5: KiHunter_Func_5(k); return;  // 0xa8f58b
  case fnKiHunter_Func_6: KiHunter_Func_6(k); return;  // 0xa8f5f0
  case fnKiHunter_Func_7: KiHunter_Func_7(k); return;  // 0xa8f68b
  case fnKiHunter_Func_8: KiHunter_Func_8(k); return;
  case fnKiHunter_Func_9: KiHunter_Func_9(k); return;  // 0xa8f6f3
  case fnKiHunter_Func_10: KiHunter_Func_10(k); return;  // 0xa8f7cf
  case fnnullsub_346: return;
  default: Unreachable();
  }
}

void KiHunter_Main(void) {  // 0xA8F25C
  Enemy_KiHunter *E = Get_KiHunter(cur_enemy_index);
  CallKiHunterFunc(E->khr_var_A | 0xA80000, cur_enemy_index);
}

void KiHunterWings_Main(void) {  // 0xA8F262
  Enemy_KiHunter *E = Get_KiHunter(cur_enemy_index);
  CallKiHunterFunc(E->khr_var_A | 0xA80000, cur_enemy_index);
}

void KiHunter_Func_1(uint16 k) {  // 0xA8F268
  int16 v4;

  Enemy_KiHunter *E = Get_KiHunter(k);
  if (Enemy_MoveDown(k, __PAIR32__(E->khr_var_09, E->khr_var_08))) {
    v4 = -E->khr_var_09;
  } else {
    uint16 y_pos = E->base.y_pos;
    if ((int16)(y_pos - E->khr_var_0A) < 0) {
      v4 = 1;
    } else {
      if ((int16)(y_pos - E->khr_var_0B) < 0)
        goto LABEL_8;
      v4 = -1;
    }
  }
  E->khr_var_09 = v4;
LABEL_8:
  if (Enemy_MoveRight_IgnoreSlopes(k, __PAIR32__(E->khr_var_07, E->khr_var_06))) {
    uint16 v5 = 0;
    bool v6 = (-E->khr_var_07 & 0x8000) != 0;
    E->khr_var_07 = -E->khr_var_07;
    if (!v6)
      v5 = 4;
    int v7 = v5 >> 1;
    E->base.current_instruction = g_off_A8F3B0[v7];
    Enemy_KiHunter *E1 = Get_KiHunter(k + 64);
    E1->base.current_instruction = g_off_A8F3B0[v7 + 1];
    E->base.instruction_timer = 1;
    E1->base.instruction_timer = 1;
  }
  EnemyFunc_C8AD(k);
  uint16 r18 = samus_x_pos - E->base.x_pos;
  uint16 r20 = abs16(r18);
  if ((int16)(r20 - g_word_A8F180) < 0 && !sign16(samus_y_pos - E->base.y_pos - 32)) {
    uint16 r24 = samus_y_pos - E->base.y_pos;
    uint16 v9;
    if ((r18 & 0x8000) != 0) {
      E->khr_var_00 = -2;
      E->khr_var_02 = 0;
      E->khr_var_03 = 0;
      E->khr_var_04 = -1;
      E->khr_var_07 = -1;
      E->khr_var_05 = -8192;
      E->khr_var_F = 255;
      E->khr_var_0E = 240;
      v9 = 0;
    } else {
      E->khr_var_00 = 2;
      E->khr_var_02 = 0;
      E->khr_var_03 = 0;
      E->khr_var_04 = 0;
      E->khr_var_05 = 0x2000;
      E->khr_var_F = 128;
      E->khr_var_07 = 1;
      E->khr_var_0E = 144;
      v9 = 4;
    }
    int v10 = v9 >> 1;
    E->base.current_instruction = g_off_A8F3B0[v10];
    Enemy_KiHunter *E1 = Get_KiHunter(k + 64);
    E1->base.current_instruction = g_off_A8F3B0[v10 + 1];
    E->base.instruction_timer = 1;
    E1->base.instruction_timer = 1;
    E->khr_var_B = r18 + E->base.x_pos;
    E->khr_var_C = E->base.y_pos;
    E->khr_var_A = FUNC16(KiHunter_Func_2);
    E->khr_var_12 = r24;
    E->khr_var_11 = r20;
    E->khr_var_10 = 0;
  }
}

void KiHunter_Func_2(uint16 k) {  // 0xA8F3B8
  Enemy_KiHunter *E = Get_KiHunter(k);
  if ((E->khr_var_04 & 0x8000) == 0) {
    if ((int16)(E->khr_var_F - E->khr_var_0E) < 0)
      goto LABEL_9;
  } else if ((int16)(E->khr_var_F - E->khr_var_0E) >= 0) {
    goto LABEL_9;
  }
  if (!E->khr_var_10) {
    E->khr_var_10 = 1;
    uint16 v2 = addr_kKiHunter_Ilist_EA32;
    if ((E->khr_var_04 & 0x8000) != 0)
      v2 = addr_kKiHunter_Ilist_EA08;
    E->base.current_instruction = v2;
    E->base.instruction_timer = 1;
  }
LABEL_9:
  if ((E->khr_var_04 & 0x8000) == 0) {
    AddToHiLo(&E->khr_var_02, &E->khr_var_03, __PAIR32__(E->khr_var_04, E->khr_var_05));
    if ((int16)(E->khr_var_02 - E->khr_var_00) >= 0)
      E->khr_var_02 = E->khr_var_00;
    E->khr_var_F += E->khr_var_02;
    if (!sign16(E->khr_var_F - 256)) {
      E->khr_var_A = FUNC16(KiHunter_Func_1);
      return;
    }
  } else {
    AddToHiLo(&E->khr_var_02, &E->khr_var_03, __PAIR32__(E->khr_var_04, E->khr_var_05));
    if ((int16)(E->khr_var_02 - E->khr_var_00) < 0)
      E->khr_var_02 = E->khr_var_00;
    E->khr_var_F += E->khr_var_02;
    if (sign16(E->khr_var_F - 128)) {
      E->khr_var_A = FUNC16(KiHunter_Func_1);
      return;
    }
  }
  uint16 r20 = E->khr_var_B + CosineMult8bit(E->khr_var_F, E->khr_var_11) - E->base.x_pos;
  if (Enemy_MoveRight_IgnoreSlopes(k, INT16_SHL16(r20))) {
    if ((E->khr_var_04 & 0x8000) == 0) {
      E->khr_var_06 = 0;
      E->khr_var_07 = -1;
    } else {
      E->khr_var_06 = 0;
      E->khr_var_07 = 1;
    }
    goto LABEL_24;
  }
  EnemyFunc_C8AD(k);
  r20 = E->khr_var_C + SineMult8bit(E->khr_var_F, E->khr_var_12) - E->base.y_pos;
  if (Enemy_MoveDown(k, INT16_SHL16(r20))) {
LABEL_24:
    E->khr_var_A = FUNC16(KiHunter_Func_3);
    E->khr_var_08 = 0;
    E->khr_var_09 = -1;
  }
}

void KiHunter_Func_3(uint16 k) {  // 0xA8F4ED
  Enemy_KiHunter *E = Get_KiHunter(k);
  if (Enemy_MoveRight_IgnoreSlopes(k, __PAIR32__(E->khr_var_07, E->khr_var_06))
      || (EnemyFunc_C8AD(k), Enemy_MoveDown(k, __PAIR32__(E->khr_var_09, E->khr_var_08)))
      || (int16)(E->base.y_pos - E->khr_var_0D) < 0) {
    E->khr_var_A = FUNC16(KiHunter_Func_1);
  }
}

const uint16 *KiHunter_Instr_1(uint16 k, const uint16 *jp) {  // 0xA8F526
  uint16 result = addr_kKiHunter_Ilist_E9FA;
  uint16 r18 = addr_kKiHunter_Ilist_EA4E;
  Enemy_KiHunter *E = Get_KiHunter(k);
  if ((E->khr_var_07 & 0x8000) == 0) {
    result = addr_kKiHunter_Ilist_EA24;
    r18 = addr_kKiHunter_Ilist_EA5E;
  }
  E->base.current_instruction = r18;
  E->base.instruction_timer = 1;
  Enemy_KiHunter *E1 = Get_KiHunter(k + 64);
  if (E1->khr_var_A == FUNC16(KiHunter_Func_9)) {
    E1->base.current_instruction = r18;
    E1->base.instruction_timer = 1;
  }
  return INSTR_RETURN_ADDR(result);
}

void KiHunter_Func_4(uint16 k) {  // 0xA8F55A
  Enemy_KiHunter *E = Get_KiHunter(k);
  if (Enemy_MoveDown(k, __PAIR32__(E->khr_var_09, E->khr_var_08))) {
    E->khr_var_A = FUNC16(KiHunter_Func_5);
  } else {
    AddToHiLo(&E->khr_var_09, &E->khr_var_08, __PAIR32__(g_word_A8F184, g_word_A8F182));
  }
}

void KiHunter_Func_5(uint16 k) {  // 0xA8F58B
  Enemy_KiHunter *E = Get_KiHunter(k);
  E->khr_var_A = addr_locret_A8F5E3;
  E->khr_var_08 = 0;
  E->khr_var_09 = (random_number & 1) - 8;
  if ((int16)(E->base.x_pos - samus_x_pos) >= 0) {
    E->khr_var_06 = 0;
    E->khr_var_07 = -2;
    E->base.current_instruction = addr_kKiHunter_Ilist_EA8A;
  } else {
    E->khr_var_06 = 0;
    E->khr_var_07 = 2;
    E->base.current_instruction = addr_kKiHunter_Ilist_EAA6;
  }
  E->base.instruction_timer = 1;
}

const uint16 *KiHunter_Instr_2(uint16 k, const uint16 *jp) {  // 0xA8F5E4
  Get_KiHunter(cur_enemy_index)->khr_var_A = FUNC16(KiHunter_Func_6);
  return jp;
}

void KiHunter_Func_6(uint16 k) {  // 0xA8F5F0
  Enemy_KiHunter *E = Get_KiHunter(k);
  if (Enemy_MoveDown(k, __PAIR32__(E->khr_var_09, E->khr_var_08))) {
    if ((E->khr_var_09 & 0x8000) != 0) {
      E->khr_var_09 = 1;
    } else {
      E->khr_var_08 = 0;
      E->khr_var_09 = -4;
      E->khr_var_A = addr_locret_A8F5E3;
      E->khr_var_0F = 12;
      uint16 v4 = addr_kKiHunter_Ilist_EAC2;
      if (!sign16(E->base.current_instruction + 0x155A))
        v4 = addr_kKiHunter_Ilist_EADA;
      E->base.current_instruction = v4;
      E->base.instruction_timer = 1;
    }
  } else {
    if (Enemy_MoveRight_IgnoreSlopes(k, __PAIR32__(E->khr_var_07, E->khr_var_06))) {
      E->khr_var_07 = -E->khr_var_07;
    } else {
      EnemyFunc_C8AD(k);
      AddToHiLo(&E->khr_var_09, &E->khr_var_08, __PAIR32__(g_word_A8F184, g_word_A8F182));
    }
  }
}

const uint16 *KiHunter_Instr_3(uint16 k, const uint16 *jp) {  // 0xA8F67F
  Get_KiHunter(cur_enemy_index)->khr_var_A = FUNC16(KiHunter_Func_7);
  return jp;
}

void KiHunter_Func_7(uint16 k) {  // 0xA8F68B
  Enemy_KiHunter *E = Get_KiHunter(k);
  uint16 v2 = E->khr_var_0F - 1;
  E->khr_var_0F = v2;
  if (!v2) {
    uint16 v3 = FUNC16(KiHunter_Func_5);
    uint16 v4 = abs16(E->base.x_pos - samus_x_pos);
    if (sign16(v4 - 96))
      v3 = FUNC16(KiHunter_Func_8);
    E->khr_var_A = v3;
  }
}

void KiHunter_Func_8(uint16 k) {  // 0xA8F6B3
  uint16 v1 = addr_kKiHunter_Ilist_EAF2;
  Enemy_KiHunter *E = Get_KiHunter(k);
  if ((int16)(E->base.x_pos - samus_x_pos) < 0)
    v1 = addr_kKiHunter_Ilist_EB10;
  E->base.current_instruction = v1;
  E->base.instruction_timer = 1;
  E->khr_var_A = addr_locret_A8F5E3;
}

const uint16 *KiHunter_Instr_4(uint16 k, const uint16 *jp) {  // 0xA8F6D2
  sub_A8F6DC(k, addr_loc_A8CF18);
  return jp;
}

const uint16 *KiHunter_Instr_5(uint16 k, const uint16 *jp) {  // 0xA8F6D8
  sub_A8F6DC(k, addr_loc_A8CF26);
  return jp;
}

void sub_A8F6DC(uint16 k, uint16 j) {  // 0xA8F6DC
  QueueSfx2_Max6(0x4C);
  SpawnEnemyProjectileWithGfx(0, cur_enemy_index, j);
  Get_KiHunter(cur_enemy_index)->khr_var_0F = 24;
}

void KiHunter_Func_9(uint16 k) {  // 0xA8F6F3
  int v1 = k >> 1;
  Enemy_KiHunter *E = Get_KiHunter(k);
  E->base.x_pos = enemy_drawing_queue[v1 + 91];
  E->base.y_pos = enemy_drawing_queue[v1 + 93];
}

void KiHunter_Shot(void) {  // 0xA8F701
  NormalEnemyShotAi();
  uint16 old_cur_enemy_index = cur_enemy_index;
  Enemy_KiHunter *EK = Get_KiHunter(cur_enemy_index);
  Enemy_KiHunter *E1 = Get_KiHunter(cur_enemy_index + 64);
  uint16 health = EK->base.health;
  if (health) {
    if (health == E1->khr_parameter_1 || (int16)(health - E1->khr_parameter_1) < 0) {
      if (!EK->khr_var_14) {
        EK->khr_var_14 = 1;
        EK->khr_var_A = FUNC16(KiHunter_Func_4);
        EK->khr_var_08 = 0;
        EK->khr_var_09 = 1;
        cur_enemy_index = old_cur_enemy_index + 64;
        if (E1->khr_var_A != FUNC16(KiHunter_Func_10)) {
          E1->khr_var_07 = E1->base.y_pos;
          E1->khr_var_08 = E1->base.x_pos;
          KiHunter_Func_17();
          KiHunter_Func_12();
          KiHunter_Func_13();
          E1->khr_var_F = -8192;
          E1->khr_var_A = FUNC16(KiHunter_Func_10);
          E1->khr_var_00 = FUNC16(KiHunter_Func_11);
          E1->khr_var_06 = E1->khr_var_07 - E1->khr_var_0B;
          E1->khr_var_05 = E1->base.x_pos;
          E1->khr_var_B = E1->khr_var_0A;
          E1->base.current_instruction = addr_kKiHunter_Ilist_EA7E;
          E1->base.spritemap_pointer = addr_kSpritemap_Nothing_A8;
          E1->base.instruction_timer = 1;
          E1->base.timer = 0;
          E1->base.properties |= kEnemyProps_ProcessedOffscreen;
        }
        cur_enemy_index = old_cur_enemy_index;
      }
    } else {
      E1->base.ai_handler_bits = EK->base.ai_handler_bits;
      E1->base.frozen_timer = EK->base.frozen_timer;
      E1->base.invincibility_timer = EK->base.invincibility_timer;
      E1->base.flash_timer = EK->base.flash_timer;
    }
  } else {
    E1->base.properties = 512;
  }
}

void CallKiHunterBFunc(uint32 ea) {
  switch (ea) {
  case fnKiHunter_Func_11: KiHunter_Func_11(); return;
  case fnKiHunter_Func_14: KiHunter_Func_14(); return;
  default: Unreachable();
  }
}
void KiHunter_Func_10(uint16 k) {  // 0xA8F7CF
  uint16 r18 = Get_KiHunter(cur_enemy_index)->khr_var_00;
  CallKiHunterBFunc(r18 | 0xA80000);
}

void KiHunter_Func_11(void) {  // 0xA8F7DB
  int16 v2;

  Enemy_KiHunter *E = Get_KiHunter(cur_enemy_index);
  E->khr_var_F += *(uint16 *)((uint8 *)&kCommonEnemySpeeds_Quadratic[2] + (8 * HIBYTE(E->khr_var_B)) + 1);
  E->base.y_pos = E->khr_var_06 + SineMult8bit(HIBYTE(E->khr_var_F), g_byte_A8F186) - E->khr_var_04;
  E->base.x_pos = E->khr_var_05 + CosineMult8bit(HIBYTE(E->khr_var_F), g_byte_A8F186) - E->khr_var_03;
  if (sign16(E->khr_var_F + 0x4000)) {
    KiHunter_Func_16(cur_enemy_index);
  } else {
    v2 = E->khr_var_B - 384;
    if (v2 < 0)
      v2 = 256;
    E->khr_var_B = v2;
  }
}

void KiHunter_Func_12(void) {  // 0xA8F851
  uint16 v1 = CosineMult8bit(0xE0, g_byte_A8F186);
  Enemy_KiHunter *E = Get_KiHunter(cur_enemy_index);
  E->khr_var_03 = v1;
  E->khr_var_04 = SineMult8bit(0xE0, g_byte_A8F186);
}

void KiHunter_Func_13(void) {  // 0xA8F87F
  uint16 v1 = CosineMult8bit(0xA0, g_byte_A8F186);
  Enemy_KiHunter *E = Get_KiHunter(cur_enemy_index);
  E->khr_var_01 = v1;
  E->khr_var_02 = SineMult8bit(0xA0, g_byte_A8F186);
}

void KiHunter_Func_14(void) {  // 0xA8F8AD
  int16 khr_var_07;

  Enemy_KiHunter *E = Get_KiHunter(cur_enemy_index);
  E->khr_var_F += *(uint16 *)((uint8 *)kCommonEnemySpeeds_Quadratic + (8 * HIBYTE(E->khr_var_B)) + 1);
  uint16 r20 = E->khr_var_06 + SineMult8bit(HIBYTE(E->khr_var_F), g_byte_A8F186) - E->khr_var_02 - E->base.y_pos;
  if (Enemy_MoveDown(cur_enemy_index, INT16_SHL16(r20))) {
    E->base.properties |= kEnemyProps_Deleted;
    E->base.x_pos = E->khr_var_08;
    khr_var_07 = E->khr_var_07;
    E->base.y_pos = khr_var_07;
  } else {
    E->base.x_pos = E->khr_var_05 + CosineMult8bit(HIBYTE(E->khr_var_F), g_byte_A8F186) - E->khr_var_01;
    if (!sign16(E->khr_var_F + 0x4000)) {
      KiHunter_Func_15(cur_enemy_index);
      return;
    }
    khr_var_07 = E->khr_var_B - 384;
    if (khr_var_07 < 0)
      khr_var_07 = 256;
  }
  Get_KiHunter(cur_enemy_index)->khr_var_B = khr_var_07;
}

void KiHunter_Func_15(uint16 k) {  // 0xA8F947
  Enemy_KiHunter *E = Get_KiHunter(k);
  E->khr_var_00 = FUNC16(KiHunter_Func_11);
  E->khr_var_B = E->khr_var_0A;
  E->khr_var_F = -8192;
  E->khr_var_05 = E->base.x_pos;
  E->khr_var_06 = E->base.y_pos;
}

void KiHunter_Func_16(uint16 k) {  // 0xA8F96A
  Enemy_KiHunter *E = Get_KiHunter(k);
  E->khr_var_00 = FUNC16(KiHunter_Func_14);
  E->khr_var_B = E->khr_var_0A;
  E->khr_var_F = -24576;
  E->khr_var_05 = E->base.x_pos;
  E->khr_var_06 = E->base.y_pos;
}

void KiHunter_Func_17(void) {  // 0xA8F98D
  uint16 v5;

  Enemy_KiHunter *E = Get_KiHunter(cur_enemy_index);
  E->khr_var_0A = 0;
  E->khr_var_B = 0;
  do {
    uint16 v3 = E->khr_var_0A + 384;
    E->khr_var_0A = v3;
    v3 >>= 8;
    v5 = *(uint16 *)((uint8 *)kCommonEnemySpeeds_Quadratic + (8 * v3) + 1) + E->khr_var_D;
    E->khr_var_D = v5;
  } while (sign16(v5 - 0x2000));
}
