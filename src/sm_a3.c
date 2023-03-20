// Enemy AI - inc. elevator & metroid
#include "ida_types.h"
#include "variables.h"
#include "sm_rtl.h"
#include "funcs.h"
#include "enemy_types.h"


#define g_off_A386DB ((uint16*)RomFixedPtr(0xa386db))
#define g_off_A3894E ((uint16*)RomFixedPtr(0xa3894e))
#define g_word_A38D1D ((uint16*)RomFixedPtr(0xa38d1d))
#define g_word_A394E2 ((uint16*)RomFixedPtr(0xa394e2))
#define g_off_A396DB ((uint16*)RomFixedPtr(0xa396db))
#define g_off_A3992B ((uint16*)RomFixedPtr(0xa3992b))
#define g_off_A3A111 ((uint16*)RomFixedPtr(0xa3a111))
#define g_off_A3A121 ((uint16*)RomFixedPtr(0xa3a121))
#define g_off_A3AAC2 ((uint16*)RomFixedPtr(0xa3aac2))
#define g_off_A3AACA ((uint16*)RomFixedPtr(0xa3aaca))
#define g_off_A3AAD2 ((uint16*)RomFixedPtr(0xa3aad2))
#define g_off_A3AADA ((uint16*)RomFixedPtr(0xa3aada))
#define g_off_A3AAE2 ((uint16*)RomFixedPtr(0xa3aae2))
#define g_word_A3AAE6 ((uint16*)RomFixedPtr(0xa3aae6))
#define g_word_A3AAEA ((uint16*)RomFixedPtr(0xa3aaea))
#define g_word_A3AAEE ((uint16*)RomFixedPtr(0xa3aaee))
#define g_word_A3AAF2 ((uint16*)RomFixedPtr(0xa3aaf2))
#define g_word_A3AAF6 ((uint16*)RomFixedPtr(0xa3aaf6))
#define g_word_A3AAFA ((uint16*)RomFixedPtr(0xa3aafa))
#define g_word_A3B415 ((uint16*)RomFixedPtr(0xa3b415))
#define g_off_A3B40D ((uint16*)RomFixedPtr(0xa3b40d))
#define g_off_A3B667 ((uint16*)RomFixedPtr(0xa3b667))
#define g_word_A3BA84 ((uint16*)RomFixedPtr(0xa3ba84))
#define g_word_A3BA94 ((uint16*)RomFixedPtr(0xa3ba94))
#define g_word_A3BC4A ((uint16*)RomFixedPtr(0xa3bc4a))
#define g_word_A3BC6A ((uint16*)RomFixedPtr(0xa3bc6a))
#define g_off_A3B722 ((uint16*)RomFixedPtr(0xa3b722))
#define g_off_A3C69C ((uint16*)RomFixedPtr(0xa3c69c))
#define g_stru_A3CD42 ((MaridiaSnailData2*)RomFixedPtr(0xa3cd42))
#define g_word_A3CD82 ((uint16*)RomFixedPtr(0xa3cd82))
#define g_word_A3CDC2 ((uint16*)RomFixedPtr(0xa3cdc2))
#define g_word_A3CCA2 ((uint16*)RomFixedPtr(0xa3cca2))
#define g_off_A3CDD2 ((uint16*)RomFixedPtr(0xa3cdd2))
#define g_off_A3D1AB ((uint16*)RomFixedPtr(0xa3d1ab))
#define g_off_A3D30D ((uint16*)RomFixedPtr(0xa3d30d))
#define g_off_A3D50F ((uint16*)RomFixedPtr(0xa3d50f))
#define g_word_A3D517 ((uint16*)RomFixedPtr(0xa3d517))
#define g_off_A3D5A4 ((uint16*)RomFixedPtr(0xa3d5a4))
#define g_word_A3DABC ((uint16*)RomFixedPtr(0xa3dabc))
#define g_off_A3DC0B ((uint16*)RomFixedPtr(0xa3dc0b))
#define g_word_A3DCAE ((uint16*)RomFixedPtr(0xa3dcae))
#define g_off_A3DCA6 ((uint16*)RomFixedPtr(0xa3dca6))
#define g_off_A3E03B ((uint16*)RomFixedPtr(0xa3e03b))
#define g_word_A3E5F0 ((uint16*)RomFixedPtr(0xa3e5f0))
#define g_off_A3E2CC ((uint16*)RomFixedPtr(0xa3e2cc))
#define g_off_A3E630 ((uint16*)RomFixedPtr(0xa3e630))
#define g_off_A3E63C ((uint16*)RomFixedPtr(0xa3e63c))
#define g_off_A3E648 ((uint16*)RomFixedPtr(0xa3e648))
#define g_off_A3E654 ((uint16*)RomFixedPtr(0xa3e654))
#define g_word_A3E931 ((uint16*)RomFixedPtr(0xa3e931))
#define g_word_A3EAD6 ((uint16*)RomFixedPtr(0xa3ead6))
#define g_word_A3EA3F ((uint16*)RomFixedPtr(0xa3ea3f))



static const int16 g_word_A3A76D[4] = { 2, 0, -2, 0 };
static const int16 g_word_A3A775[4] = { 0, -2, 0, 2 };


void Enemy_GrappleReact_NoInteract_A3(void) {  // 0xA38000
  SwitchEnemyAiToMainAi();
}

void Enemy_GrappleReact_KillEnemy_A3(void) {  // 0xA3800A
  EnemyGrappleDeath();
}

void Enemy_GrappleReact_CancelBeam_A3(void) {  // 0xA3800F
  Enemy_SwitchToFrozenAi();
}

void Enemy_NormalTouchAI_A3(void) {  // 0xA38023
  NormalEnemyTouchAi();
}

void Enemy_NormalShotAI_A3(void) {  // 0xA3802D
  NormalEnemyShotAi();
}

void Enemy_NormalShotAI_SkipSomeParts_A3(void) {  // 0xA38032
  NormalEnemyShotAiSkipDeathAnim_CurEnemy();
}

void Enemy_NormalPowerBombAI_A3(void) {  // 0xA38037
  NormalEnemyPowerBombAi();
}

void Enemy_NormalFrozenAI_A3(void) {  // 0xA38041
  NormalEnemyFrozenAI();
}

const uint16 *Waver_Instr_1(uint16 k, const uint16 *jp) {  // 0xA386E3
  Get_Waver(cur_enemy_index)->waver_var_E = 1;
  return jp;
}

void Waver_Init(void) {  // 0xA386ED
  Enemy_Waver *E = Get_Waver(cur_enemy_index);
  E->waver_var_B = 1;
  E->waver_var_A = 0x8000;
  if ((E->waver_parameter_1 & 1) == 0) {
    E->waver_var_B = SignExtend8(0xFEu);
    E->waver_var_A = SignExtend8(0x8000u);
  }
  E->waver_var_F = 0;
  E->waver_var_C = 0;
  E->waver_var_E = 0;
  E->base.current_instruction = addr_kWaver_Ilist_86A7;
  E->waver_var_F = E->waver_parameter_1 & 1;
  Waver_Func_1();
}

void Waver_Main(void) {  // 0xA3874C
  uint16 v2;

  Enemy_Waver *E = Get_Waver(cur_enemy_index);
  R18_ = E->waver_var_A;
  R20_ = E->waver_var_B;
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index) & 1) {
    R18_ = *(uint16 *)((char *)&E->waver_var_A + 1);
    E->waver_var_B = SignExtend8((uint8)((uint16)(-R18_ & 0xFF00) >> 8));
    LOBYTE(v2) = 0;
    HIBYTE(v2) = -(int8)R18_;
    E->waver_var_A = SignExtend8(v2);
    E->waver_var_F = ((uint8)E->waver_var_F ^ 1) & 1;
    Waver_Func_1();
  } else {
    draw_enemy_layer = 4;
    R20_ = SineMult8bitNegative(LOBYTE(E->waver_var_D));
    R18_ = 0;
    if (Enemy_MoveDown(cur_enemy_index) & 1) {
      E->waver_var_D = (uint8)(E->waver_var_D + 0x80);
    } else {
      E->waver_var_D += 2;
    }
  }
  if ((E->waver_var_D & 0x7F) == 56) {
    E->waver_var_F |= 2u;
    Waver_Func_1();
  }
  if (E->waver_var_E) {
    E->waver_var_E = 0;
    E->waver_var_F = E->waver_var_F & 1;
    Waver_Func_1();
  }
}

void Waver_Func_1(void) {  // 0xA387FE
  Enemy_Waver *E = Get_Waver(cur_enemy_index);
  uint16 waver_var_F = E->waver_var_F;
  if (waver_var_F != E->waver_var_C) {
    E->waver_var_C = waver_var_F;
    E->base.current_instruction = g_off_A386DB[waver_var_F];
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

const uint16 *Metalee_Instr_1(uint16 k, const uint16 *jp) {  // 0xA38956
  Get_Metalee(cur_enemy_index)->metalee_var_E = 1;
  return jp;
}

void Metalee_Init(void) {  // 0xA38960
  Enemy_Metalee *E = Get_Metalee(cur_enemy_index);
  E->metalee_var_C = 0;
  E->metalee_var_D = 0;
  E->metalee_var_E = 0;
  E->base.current_instruction = addr_kMetalee_Ilist_8910;
  E->metalee_var_B = FUNC16(Metalee_Func_1);
}

void CallMetaleeFunc(uint32 ea) {
  switch (ea) {
  case fnMetalee_Func_1: Metalee_Func_1(); return;
  case fnMetalee_Func_3: Metalee_Func_3(cur_enemy_index); return;
  case fnMetalee_Func_4: Metalee_Func_4(); return;
  case fnMetalee_Func_5: Metalee_Func_5(); return;
  default: Unreachable();
  }
}

void Metalee_Main(void) {  // 0xA38979
  Enemy_Metalee *E = Get_Metalee(cur_enemy_index);
  CallMetaleeFunc(E->metalee_var_B | 0xA30000);
}

void Metalee_Func_1(void) {  // 0xA38987
  Enemy_Metalee *E = Get_Metalee(cur_enemy_index);
  if (abs16(E->base.x_pos - samus_x_pos) < 0x48u) {
    Metalee_Func_2(cur_enemy_index);
    ++E->metalee_var_C;
    Metalee_Func_6();
    E->metalee_var_B = FUNC16(Metalee_Func_3);
  }
}

void Metalee_Func_2(uint16 k) {  // 0xA389AC
  Enemy_Metalee *E = Get_Metalee(k);
  uint16 div = (uint8)SnesDivide(samus_y_pos - E->base.y_pos, 24);
  Get_Metalee(k)->metalee_var_F = div + 4;
}

void Metalee_Func_3(uint16 k) {  // 0xA389D4
  Enemy_Metalee *E = Get_Metalee(k);
  if (E->metalee_var_E) {
    E->metalee_var_E = 0;
    ++E->metalee_var_C;
    Metalee_Func_6();
    E->metalee_var_B = FUNC16(Metalee_Func_4);
    QueueSfx2_Max6(0x5Bu);
  }
}

void Metalee_Func_4(void) {  // 0xA389F3
  Enemy_Metalee *E = Get_Metalee(cur_enemy_index);
  E->metalee_var_A = 21;
  R20_ = E->metalee_var_F;
  R18_ = 0;
  uint16 v1 = E->base.properties | 3;
  E->base.properties = v1;
  if (EnemyFunc_BF8A(cur_enemy_index, v1) & 1) {
    E->base.instruction_timer = 1;
    E->base.timer = 0;
    E->metalee_var_B = FUNC16(Metalee_Func_5);
    QueueSfx2_Max6(0x5Cu);
  } else {
    E->base.y_pos += E->metalee_var_F;
    varE24 = 2;
    if ((int16)(E->base.x_pos - samus_x_pos) >= 0)
      varE24 = -2;
    E->base.x_pos += varE24;
  }
}

void Metalee_Func_5(void) {  // 0xA38A5C
  Enemy_Metalee *E = Get_Metalee(cur_enemy_index);
  if (E->metalee_var_A-- == 1) {
    gEnemySpawnData(cur_enemy_index)->vram_tiles_index = E->base.vram_tiles_index | E->base.palette_index;
    E->base.palette_index = 2560;
    E->base.vram_tiles_index = 0;
    E->base.properties |= kEnemyProps_Deleted;
  } else {
    if (E->metalee_var_A == 8) {
      uint16 v2 = cur_enemy_index;
      SpawnEnemyProjectileWithGfx(0, v2, addr_stru_868BFA);
      SpawnEnemyProjectileWithGfx(0, v2, addr_stru_868C08);
      SpawnEnemyProjectileWithGfx(0, v2, addr_stru_868C16);
      SpawnEnemyProjectileWithGfx(0, v2, addr_stru_868C24);
    }
    ++E->base.y_pos;
  }
}

void Metalee_Func_6(void) {  // 0xA38AB2
  Enemy_Metalee *E = Get_Metalee(cur_enemy_index);
  uint16 metalee_var_C = E->metalee_var_C;
  if (metalee_var_C != E->metalee_var_D) {
    E->metalee_var_D = metalee_var_C;
    E->base.current_instruction = g_off_A3894E[metalee_var_C];
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

void Metalee_Shot(void) {  // 0xA38B0F
  Enemy_Metalee *E = Get_Metalee(cur_enemy_index);
  varE2A = E->base.vram_tiles_index;
  varE2C = E->base.palette_index;
  Enemy_NormalShotAI_A3();
  if (!Get_Metalee(cur_enemy_index)->base.health) {
    E->base.vram_tiles_index = varE2A;
    E->base.palette_index = varE2C;
    uint16 v2 = cur_enemy_index;
    SpawnEnemyProjectileWithGfx(E->metalee_var_A, cur_enemy_index, addr_stru_868BFA);
    SpawnEnemyProjectileWithGfx(0, v2, addr_stru_868C08);
    SpawnEnemyProjectileWithGfx(0, v2, addr_stru_868C16);
    SpawnEnemyProjectileWithGfx(0, v2, addr_stru_868C24);
    E->base.vram_tiles_index = 0;
    E->base.palette_index = 0;
  }
}

void Fireflea_Init(void) {  // 0xA38D2D
  Enemy_Fireflea *E = Get_Fireflea(cur_enemy_index);
  E->base.current_instruction = addr_kFireflea_Ilist_8C2F;
  R18_ = LOBYTE(E->firefl_parameter_1);
  if ((R18_ & 2) != 0) {
    Fireflea_Func_1(cur_enemy_index);
    Fireflea_Func_2(cur_enemy_index);
    Fireflea_Func_3(cur_enemy_index);
    Fireflea_Func_4(cur_enemy_index);
    Fireflea_Func_5(cur_enemy_index);
  } else {
    Fireflea_Func_3(cur_enemy_index);
    Fireflea_Func_4(cur_enemy_index);
    Fireflea_Func_6(cur_enemy_index);
  }
}

void Fireflea_Func_1(uint16 k) {  // 0xA38D5D
  Enemy_Fireflea *E = Get_Fireflea(k);
  E->firefl_var_E = E->base.x_pos;
  E->firefl_var_F = E->base.y_pos;
}

void Fireflea_Func_2(uint16 k) {  // 0xA38D6A
  Enemy_Fireflea *E = Get_Fireflea(k);
  E->firefl_var_D = HIBYTE(E->firefl_parameter_1) << 8;
}

void Fireflea_Func_3(uint16 k) {  // 0xA38D75
  Enemy_FireFlea *E = Get_FireFlea(k);
  uint16 v2 = 8 * LOBYTE(E->ffa_parameter_2);
  if ((R18_ & 1) == 0)
    v2 += 4;
  E->ffa_var_02 = v2;
  int v3 = v2 >> 1;
  E->ffa_var_B = kCommonEnemySpeeds_Linear[v3];
  E->ffa_var_A = kCommonEnemySpeeds_Linear[v3 + 1];
}

void Fireflea_Func_4(uint16 k) {  // 0xA38D9C
  Enemy_Fireflea *E = Get_Fireflea(k);
  E->firefl_var_C = LOBYTE(g_word_A38D1D[HIBYTE(E->firefl_parameter_2)]);
}

void Fireflea_Func_5(uint16 k) {  // 0xA38DAE
  Enemy_Fireflea *E = Get_Fireflea(k);
  draw_enemy_layer = E->firefl_var_C;
  E->base.x_pos = E->firefl_var_E + CosineMult8bit(E->firefl_var_D);
  draw_enemy_layer = E->firefl_var_C;
  E->base.y_pos = E->firefl_var_F + SineMult8bitNegative(E->firefl_var_D);
}

void Fireflea_Func_6(uint16 k) {  // 0xA38DD7
  Enemy_FireFlea *E = Get_FireFlea(k);
  E->ffa_var_00 = E->base.y_pos - E->ffa_var_C;
  E->ffa_var_01 = E->ffa_var_C + E->base.y_pos;
}

void Fireflea_Main(void) {  // 0xA38DEE
  Enemy_FireFlea *E = Get_FireFlea(cur_enemy_index);
  if ((E->ffa_parameter_1 & 2) != 0) {
    draw_enemy_layer = E->ffa_var_C;
    E->base.x_pos = E->ffa_var_E + CosineMult8bit(HIBYTE(E->ffa_var_D));
    draw_enemy_layer = E->ffa_var_C;
    E->base.y_pos = E->ffa_var_F + SineMult8bitNegative(HIBYTE(E->ffa_var_D));
    E->ffa_var_D += *(uint16 *)((char *)&E->ffa_var_A + 1);
  } else {
    uint16 y_subpos = E->base.y_subpos;
    int v2 = E->ffa_var_02 >> 1;
    bool v3 = __CFADD__uint16(kCommonEnemySpeeds_Linear[v2 + 1], y_subpos);
    E->base.y_subpos = kCommonEnemySpeeds_Linear[v2 + 1] + y_subpos;
    E->base.y_pos += kCommonEnemySpeeds_Linear[v2] + v3;
    if ((int16)(E->base.y_pos - E->ffa_var_00) < 0
        || (int16)(E->base.y_pos - E->ffa_var_01) >= 0) {
      E->ffa_var_02 ^= 4u;
    }
  }
}

void Fireflea_Touch(uint16 k) {  // 0xA38E6B
  uint16 v1 = 0;

  Enemy_NormalTouchAI_A3();
  EnemyDeathAnimation(k, v1);
  if (sign16(fireflea_darkness_level - 12))
    fireflea_darkness_level += 2;
}

void Fireflea_Powerbomb(void) {  // 0xA38E83
  Enemy_NormalPowerBombAI_A3();
  Fireflea_Common();
}

void Fireflea_Shot(void) {  // 0xA38E89
  Enemy_NormalShotAI_A3();
  Fireflea_Common();
}

void Fireflea_Common(void) {  // 0xA38E8D
  if (!Get_Fireflea(cur_enemy_index)->base.health) {
    if (sign16(fireflea_darkness_level - 12))
      fireflea_darkness_level += 2;
  }
}

const uint16 *MaridiaFish_Instr_3(uint16 k, const uint16 *jp) {  // 0xA39096
  Get_MaridiaFish(cur_enemy_index)->base.layer = 6;
  return jp;
}

const uint16 *MaridiaFish_Instr_1(uint16 k, const uint16 *jp) {  // 0xA390A0
  Get_MaridiaFish(cur_enemy_index)->base.layer = 2;
  return jp;
}

const uint16 *MaridiaFish_Instr_2(uint16 k, const uint16 *jp) {  // 0xA390AA
  Get_MaridiaFish(cur_enemy_index)->mfh_var_01 = 1;
  return jp;
}

void MaridiaFish_Init(void) {  // 0xA390B5
  Enemy_MaridiaFish *E = Get_MaridiaFish(cur_enemy_index);
  E->base.current_instruction = addr_kMaridiaFish_Ilist_902A;
  E->mfh_var_A = FUNC16(MaridiaFish_Func_1);
  if (!HIBYTE(E->mfh_parameter_1)) {
    E->base.current_instruction = addr_kMaridiaFish_Ilist_9060;
    E->mfh_var_A = FUNC16(MaridiaFish_Func_2);
  }
  int v1 = (uint16)(8 * LOBYTE(E->mfh_parameter_1)) >> 1;
  E->mfh_var_C = kCommonEnemySpeeds_Linear[v1];
  E->mfh_var_B = kCommonEnemySpeeds_Linear[v1 + 1];
  E->mfh_var_E = kCommonEnemySpeeds_Linear[v1 + 2];
  E->mfh_var_D = kCommonEnemySpeeds_Linear[v1 + 3];
  E->mfh_var_00 = LOBYTE(E->mfh_parameter_2);
  E->mfh_var_02 = HIBYTE(E->mfh_parameter_2);
  E->mfh_var_F = 0;
  E->mfh_var_01 = 0;
  draw_enemy_layer = E->mfh_var_00;
  E->mfh_var_03 = SineMult8bitNegative(E->mfh_var_F);
}

void CallMaridiaFishFunc(uint32 ea) {
  switch (ea) {
  case fnMaridiaFish_Func_1: MaridiaFish_Func_1(); return;
  case fnMaridiaFish_Func_2: MaridiaFish_Func_2(); return;
  case fnMaridiaFish_Func_3: MaridiaFish_Func_3(); return;
  case fnMaridiaFish_Func_4: MaridiaFish_Func_4(); return;
  default: Unreachable();
  }
}

void MaridiaFish_Main(void) {  // 0xA3912B
  Enemy_MaridiaFish *E = Get_MaridiaFish(cur_enemy_index);
  CallMaridiaFishFunc(E->mfh_var_A | 0xA30000);
}

void MaridiaFish_Func_1(void) {  // 0xA39132
  Enemy_MaridiaFish *E = Get_MaridiaFish(cur_enemy_index);
  R18_ = E->mfh_var_D;
  R20_ = E->mfh_var_E;
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index) & 1) {
    E->mfh_var_A = FUNC16(MaridiaFish_Func_3);
    E->base.instruction_timer = 1;
    E->base.timer = 0;
    E->base.current_instruction = addr_kMaridiaFish_Ilist_903C;
  } else {
    draw_enemy_layer = E->mfh_var_00;
    uint16 v2 = SineMult8bitNegative(E->mfh_var_F);
    E->mfh_var_04 = v2;
    R20_ = v2 - E->mfh_var_03;
    R18_ = 0;
    if (Enemy_MoveDown(cur_enemy_index) & 1) {
      E->mfh_var_A = FUNC16(MaridiaFish_Func_3);
      E->base.instruction_timer = 1;
      E->base.timer = 0;
      E->base.current_instruction = addr_kMaridiaFish_Ilist_903C;
    } else {
      E->mfh_var_F = (uint8)(LOBYTE(E->mfh_var_02) + E->mfh_var_F);
    }
  }
  E->mfh_var_03 = E->mfh_var_04;
}

void MaridiaFish_Func_2(void) {  // 0xA391AB
  Enemy_MaridiaFish *E = Get_MaridiaFish(cur_enemy_index);
  R18_ = E->mfh_var_B;
  R20_ = E->mfh_var_C;
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index) & 1) {
    E->mfh_var_A = FUNC16(MaridiaFish_Func_4);
    E->base.instruction_timer = 1;
    E->base.timer = 0;
    E->base.current_instruction = addr_kMaridiaFish_Ilist_9072;
  } else {
    draw_enemy_layer = E->mfh_var_00;
    uint16 v2 = SineMult8bitNegative(E->mfh_var_F);
    E->mfh_var_04 = v2;
    R20_ = v2 - E->mfh_var_03;
    R18_ = 0;
    if (Enemy_MoveDown(cur_enemy_index) & 1) {
      E->mfh_var_A = FUNC16(MaridiaFish_Func_4);
      E->base.instruction_timer = 1;
      E->base.timer = 0;
      E->base.current_instruction = addr_kMaridiaFish_Ilist_9072;
    } else {
      E->mfh_var_F = (uint8)(LOBYTE(E->mfh_var_02) + E->mfh_var_F);
    }
  }
  E->mfh_var_03 = E->mfh_var_04;
}

void MaridiaFish_Func_3(void) {  // 0xA39224
  Enemy_MaridiaFish *E = Get_MaridiaFish(cur_enemy_index);
  if (E->mfh_var_01) {
    E->mfh_var_01 = 0;
    E->mfh_var_A = FUNC16(MaridiaFish_Func_2);
    E->mfh_var_02 = -E->mfh_var_02;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
    E->base.current_instruction = addr_kMaridiaFish_Ilist_9060;
  }
}

void MaridiaFish_Func_4(void) {  // 0xA39256
  Enemy_MaridiaFish *E = Get_MaridiaFish(cur_enemy_index);
  if (E->mfh_var_01) {
    E->mfh_var_01 = 0;
    E->mfh_var_A = FUNC16(MaridiaFish_Func_1);
    E->mfh_var_02 = -E->mfh_var_02;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
    E->base.current_instruction = addr_kMaridiaFish_Ilist_902A;
  }
}

void Elevator_Init(void) {  // 0xA394E6
  Enemy_Elevator *E = Get_Elevator(cur_enemy_index);
  E->base.spritemap_pointer = addr_kSpritemap_Nothing_A3;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->base.current_instruction = addr_kElevator_Ilist_94D6;
  E->elevat_parameter_1 *= 2;
  E->elevat_var_A = E->base.y_pos;
  if (elevator_status != 2) {
    elevator_flags = 0;
    elevator_status = 0;
  }
  if (__PAIR32__(elevator_status, elevator_flags)) {
    E->base.y_pos = E->elevat_parameter_2;
    Elevator_Func_4();
  }
}
static Func_V *const off_A39540[4] = { Elevator_Func_1, Elevator_Func_2, Elevator_Func_3, Elevator_Func3b };
void Elevator_Frozen(void) {  // 0xA3952A
  if (!door_transition_flag_elevator_zebetites) {
    if (__PAIR32__(elevator_status, elevator_flags))
      off_A39540[elevator_status]();
  }
}

void Elevator_Func_1(void) {  // 0xA39548
  if ((g_word_A394E2[Get_Elevator(cur_enemy_index)->elevat_parameter_1 >> 1] & joypad1_newkeys) != 0) {
    QueueSfx3_Max6(0xBu);
    QueueSfx1_Max6(0x32u);
    CallSomeSamusCode(7u);
    ResetProjectileData();
    Elevator_Func_4();
    ++elevator_status;
  } else {
    elevator_flags = 0;
  }
}

void Elevator_Func_2(void) {  // 0xA39579
  Enemy_Elevator *E = Get_Elevator(cur_enemy_index);
  uint16 v2;
  if (E->elevat_parameter_1) {
    elevator_direction = FUNC16(Enemy_GrappleReact_NoInteract_A3);
    uint16 y_subpos = E->base.y_subpos;
    E->base.y_subpos = y_subpos + 0x8000;
    v2 = (__PAIR32__(E->base.y_pos, y_subpos) - 98304) >> 16;
  } else {
    elevator_direction = 0;
    uint16 v1 = E->base.y_subpos;
    E->base.y_subpos = v1 + 0x8000;
    v2 = __CFADD__uint16(v1, 0x8000) + E->base.y_pos + 1;
  }
  E->base.y_pos = v2;
  Elevator_Func_4();
}

void Elevator_Func_3(void) {  // 0xA395B9
  ++elevator_status;
  Elevator_Func3b();
}

void Elevator_Func3b(void) {  // 0xA395BC
  Enemy_Elevator *E = Get_Elevator(cur_enemy_index);
  if (E->elevat_parameter_1) {
    uint16 y_subpos;
    y_subpos = E->base.y_subpos;
    E->base.y_subpos = y_subpos + 0x8000;
    uint16 v2;
    v2 = __CFADD__uint16(y_subpos, 0x8000) + E->base.y_pos + 1;
    E->base.y_pos = v2;
    if (v2 < E->elevat_var_A) {
LABEL_3:
      Elevator_Func_4();
      return;
    }
  } else {
    uint16 v3 = E->base.y_subpos;
    E->base.y_subpos = v3 + 0x8000;
    uint16 v4 = (__PAIR32__(E->base.y_pos, v3) - 98304) >> 16;
    E->base.y_pos = v4;
    if (v4 >= E->elevat_var_A)
      goto LABEL_3;
  }
  elevator_status = 0;
  elevator_flags = 0;
  QueueSfx3_Max6(0x25u);
  E->base.y_pos = E->elevat_var_A;
  CallSomeSamusCode(0xBu);
  Elevator_Func_4();
}

void Elevator_Func_4(void) {  // 0xA39612
  Enemy_Elevator *E = Get_Elevator(cur_enemy_index);
  samus_y_pos = E->base.y_pos - 26;
  samus_y_subpos = 0;
  samus_x_pos = E->base.x_pos;
  samus_y_speed = 0;
  samus_y_subspeed = 0;
}

void Crab_Init(void) {  // 0xA396E3
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  v1->parameter_2 = 8;
  v1->current_instruction = g_off_A396DB[v1->current_instruction & 3];
  StoneZoomer_E67A(cur_enemy_index);
}

void Crab_Func_1(void) {  // 0xA396FD
  ;
}

void Slug_Init(void) {  // 0xA3993B
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  v1->parameter_2 = 10;
  v1->current_instruction = g_off_A3992B[v1->current_instruction & 3];
  StoneZoomer_E67A(cur_enemy_index);
}

void Slug_Func_1(void) {  // 0xA39955
  ;
}

const uint16 *PlatformThatFallsWithSamus_Instr_3(uint16 k, const uint16 *jp) {  // 0xA39C6B
  Get_PlatformThatFallsWithSamus(cur_enemy_index)->ptfwss_var_02 = 0;
  return jp;
}

const uint16 *PlatformThatFallsWithSamus_Instr_4(uint16 k, const uint16 *jp) {  // 0xA39C76
  Get_PlatformThatFallsWithSamus(cur_enemy_index)->ptfwss_var_02 = 1;
  return jp;
}

const uint16 *PlatformThatFallsWithSamus_Instr_1(uint16 k, const uint16 *jp) {  // 0xA39C81
  Get_PlatformThatFallsWithSamus(cur_enemy_index)->ptfwss_var_02 = 0;
  return jp;
}

const uint16 *PlatformThatFallsWithSamus_Instr_2(uint16 k, const uint16 *jp) {  // 0xA39C8C
  Get_PlatformThatFallsWithSamus(cur_enemy_index)->ptfwss_var_02 = 1;
  return jp;
}

void PlatformThatFallsWithSamus_Init(void) {  // 0xA39C9F
  Enemy_PlatformThatFallsWithSamus *E = Get_PlatformThatFallsWithSamus(cur_enemy_index);
  E->ptfwss_var_1F = -1;
  uint16 ptfwss_parameter_1 = E->ptfwss_parameter_1;
  E->ptfwss_var_02 = ptfwss_parameter_1;
  if (ptfwss_parameter_1)
    PlatformThatFalls_Init(cur_enemy_index, addr_kPlatformThatFallsWithSamus_Ilist_9BFD);
  else
    PlatformThatFalls_Init(cur_enemy_index, addr_kPlatformThatFallsWithSamus_Ilist_9BE7);
}

void FastMovingSlowSinkingPlatform_Init(void) {  // 0xA39CBA
  Enemy_PlatformThatFallsWithSamus *E = Get_PlatformThatFallsWithSamus(cur_enemy_index);
  uint16 ptfwss_parameter_1 = E->ptfwss_parameter_1;
  E->ptfwss_var_02 = ptfwss_parameter_1;
  if (ptfwss_parameter_1)
    PlatformThatFalls_Init(cur_enemy_index, addr_kPlatformThatFallsWithSamus_Ilist_9C55);
  else
    PlatformThatFalls_Init(cur_enemy_index, addr_kPlatformThatFallsWithSamus_Ilist_9C3F);
}

void PlatformThatFalls_Init(uint16 k, uint16 j) {  // 0xA39CCC
  Enemy_PlatformThatFallsWithSamus *E = Get_PlatformThatFallsWithSamus(k);
  E->base.current_instruction = j;
  int v3 = (uint16)(8 * LOBYTE(E->ptfwss_parameter_2)) >> 1;
  E->ptfwss_var_C = kCommonEnemySpeeds_Linear[v3];
  E->ptfwss_var_B = kCommonEnemySpeeds_Linear[v3 + 1];
  E->ptfwss_var_E = kCommonEnemySpeeds_Linear[v3 + 2];
  E->ptfwss_var_D = kCommonEnemySpeeds_Linear[v3 + 3];
  E->ptfwss_var_00 = 0;
  E->ptfwss_var_03 = 0;
  E->ptfwss_var_04 = 0;
  E->ptfwss_var_A = E->base.y_pos + 1;
  E->ptfwss_var_F = 0;
  E->ptfwss_var_05 = HIBYTE(E->ptfwss_parameter_2);
}
static Func_V *const off_A39C97[2] = { PlatformThatFallsWithSamus_Func_1, PlatformThatFallsWithSamus_Func_2 };
static Func_V *const off_A39C9B[2] = { PlatformThatFallsWithSamus_Func_3, PlatformThatFallsWithSamus_Func_4 };
void PlatformThatFallsWithSamus_Main(void) {  // 0xA39D16
  Enemy_PlatformThatFallsWithSamus *E = Get_PlatformThatFallsWithSamus(cur_enemy_index);
  E->ptfwss_var_00 = 0;
  if (CheckIfEnemyTouchesSamus(cur_enemy_index))
    E->ptfwss_var_00 = 1;
  off_A39C97[E->ptfwss_var_02]();
  int v3 = (uint16)(2 * Get_PlatformThatFallsWithSamus(cur_enemy_index)->ptfwss_var_00) >> 1;
  off_A39C9B[v3]();
  if (E->ptfwss_var_00 != E->ptfwss_var_06)
    E->ptfwss_var_F = 0;
  E->ptfwss_var_06 = E->ptfwss_var_00;
}

void PlatformThatFallsWithSamus_Func_1(void) {  // 0xA39D5E
  Enemy_PlatformThatFallsWithSamus *E = Get_PlatformThatFallsWithSamus(cur_enemy_index);
  E->ptfwss_var_01 = E->base.x_pos;
  R18_ = E->ptfwss_var_D;
  R20_ = E->ptfwss_var_E;
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index) & 1) {
    E->ptfwss_var_02 = 1;
    PlatformThatFallsWithSamus_Func_8();
  }
}

void PlatformThatFallsWithSamus_Func_2(void) {  // 0xA39D83
  Enemy_PlatformThatFallsWithSamus *E = Get_PlatformThatFallsWithSamus(cur_enemy_index);
  E->ptfwss_var_01 = E->base.x_pos;
  R18_ = E->ptfwss_var_B;
  R20_ = E->ptfwss_var_C;
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index) & 1) {
    E->ptfwss_var_02 = 0;
    PlatformThatFallsWithSamus_Func_7();
  }
}

void PlatformThatFallsWithSamus_Func_3(void) {  // 0xA39DA8
  Enemy_PlatformThatFallsWithSamus *E = Get_PlatformThatFallsWithSamus(cur_enemy_index);
  if ((int16)(E->base.y_pos - E->ptfwss_var_A) < 0)
    goto LABEL_5;
  PlatformThatFallsWithSamus_Func_9();
  uint16 ptfwss_var_05;
  ptfwss_var_05 = ++E->ptfwss_var_F;
  if ((int16)(ptfwss_var_05 - E->ptfwss_var_05) >= 0) {
    ptfwss_var_05 = E->ptfwss_var_05;
    E->ptfwss_var_F = ptfwss_var_05;
  }
  int v3;
  v3 = (uint16)(8 * ptfwss_var_05) >> 1;
  R18_ = kCommonEnemySpeeds_Quadratic[v3 + 2];
  R20_ = kCommonEnemySpeeds_Quadratic[v3 + 3];
  if (Enemy_MoveDown(cur_enemy_index) & 1) {
LABEL_5:
    E->ptfwss_var_F = 0;
    PlatformThatFallsWithSamus_Func_10();
  }
}

void PlatformThatFallsWithSamus_Func_4(void) {  // 0xA39DE4
  Enemy_PlatformThatFallsWithSamus *E = Get_PlatformThatFallsWithSamus(cur_enemy_index);
  if ((int16)(++E->ptfwss_var_F - E->ptfwss_var_05) >= 0)
    E->ptfwss_var_F = E->ptfwss_var_05;
  extra_samus_x_displacement += E->base.x_pos - E->ptfwss_var_01;
  E->ptfwss_var_01 = E->base.y_pos;
  int v2 = (uint16)(8 * E->ptfwss_var_F) >> 1;
  R18_ = kCommonEnemySpeeds_Quadratic[v2];
  R20_ = kCommonEnemySpeeds_Quadratic[v2 + 1];
  if (Enemy_MoveDown(cur_enemy_index) & 1) {
    E->ptfwss_var_F = 0;
    PlatformThatFallsWithSamus_Func_10();
  }
  extra_samus_y_displacement += E->base.y_pos - E->ptfwss_var_01;
}

void PlatformThatFallsWithSamus_Func_5(void) {  // 0xA39E47
  Enemy_PlatformThatFallsWithSamus *E = Get_PlatformThatFallsWithSamus(cur_enemy_index);
  uint16 v0 = addr_kPlatformThatFallsWithSamus_Ilist_9C13;
  if ((E->ptfwss_var_1F & 0x8000u) != 0)
    v0 = addr_kPlatformThatFallsWithSamus_Ilist_9BBB;
  E->base.current_instruction = v0;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
}

void PlatformThatFallsWithSamus_Func_6(void) {  // 0xA39E64
  Enemy_PlatformThatFallsWithSamus *E = Get_PlatformThatFallsWithSamus(cur_enemy_index);
  uint16 v0 = addr_kPlatformThatFallsWithSamus_Ilist_9C29;
  if ((E->ptfwss_var_1F & 0x8000u) != 0)
    v0 = addr_kPlatformThatFallsWithSamus_Ilist_9BD1;
  E->base.current_instruction = v0;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
}

void PlatformThatFallsWithSamus_Func_7(void) {  // 0xA39E81
  Enemy_PlatformThatFallsWithSamus *E = Get_PlatformThatFallsWithSamus(cur_enemy_index);
  uint16 v0 = addr_kPlatformThatFallsWithSamus_Ilist_9C3F;
  if ((E->ptfwss_var_1F & 0x8000u) != 0)
    v0 = addr_kPlatformThatFallsWithSamus_Ilist_9BE7;
  E->base.current_instruction = v0;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
}

void PlatformThatFallsWithSamus_Func_8(void) {  // 0xA39E9E
  Enemy_PlatformThatFallsWithSamus *E = Get_PlatformThatFallsWithSamus(cur_enemy_index);
  uint16 v0 = addr_kPlatformThatFallsWithSamus_Ilist_9C55;
  if ((E->ptfwss_var_1F & 0x8000u) != 0)
    v0 = addr_kPlatformThatFallsWithSamus_Ilist_9BFD;
  E->base.current_instruction = v0;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
}

void PlatformThatFallsWithSamus_Func_9(void) {  // 0xA39EBB
  Enemy_PlatformThatFallsWithSamus *E = Get_PlatformThatFallsWithSamus(cur_enemy_index);
  if (!E->ptfwss_var_03) {
    E->ptfwss_var_03 = 1;
    if (E->ptfwss_var_02)
      PlatformThatFallsWithSamus_Func_6();
    else
      PlatformThatFallsWithSamus_Func_5();
  }
  E->ptfwss_var_04 = 0;
}

void PlatformThatFallsWithSamus_Func_10(void) {  // 0xA39EE1
  Enemy_PlatformThatFallsWithSamus *E = Get_PlatformThatFallsWithSamus(cur_enemy_index);
  if (!E->ptfwss_var_04) {
    E->ptfwss_var_04 = 1;
    if (E->ptfwss_var_02)
      PlatformThatFallsWithSamus_Func_8();
    else
      PlatformThatFallsWithSamus_Func_7();
  }
  E->ptfwss_var_03 = 0;
}

void FastMovingSlowSinkingPlatform_Shot(void) {  // 0xA39F08
  Enemy_NormalShotAI_A3();
  Enemy_PlatformThatFallsWithSamus *E = Get_PlatformThatFallsWithSamus(cur_enemy_index);
  if (E->base.frozen_timer) {
    if (E->ptfwss_var_02)
      E->base.spritemap_pointer = addr_kPlatformThatFallsWithSamus_Sprmap_A015;
    else
      E->base.spritemap_pointer = addr_kPlatformThatFallsWithSamus_Sprmap_A009;
  }
}

void Roach_Func_1(void) {  // 0xA3A12F
  Enemy_Roach *E = Get_Roach(cur_enemy_index);
  uint16 roach_var_24 = E->roach_var_24;
  if (roach_var_24 != E->roach_var_25) {
    E->roach_var_25 = roach_var_24;
    E->base.current_instruction = roach_var_24;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

void Roach_Init(void) {  // 0xA3A14D
  Enemy_Roach *E = Get_Roach(cur_enemy_index);
  E->roach_var_25 = 0;
  E->roach_var_24 = 0;
  Roach_Func_2(cur_enemy_index);
  Roach_Func_4(cur_enemy_index);
  Roach_Func_5(cur_enemy_index);
  Roach_Func_6(cur_enemy_index);
  Roach_Func_7(cur_enemy_index);
  Roach_Func_8(cur_enemy_index);
  E->roach_var_24 = g_off_A3A111[E->roach_var_20 >> 1];
  Roach_Func_1();
  E->roach_var_B = FUNC16(Roach_Func_9);
}

void Roach_Func_2(uint16 k) {  // 0xA3A183
  Enemy_Roach *E = Get_Roach(k);
  R20_ = LOBYTE(E->roach_parameter_1);
  R18_ = HIBYTE(E->roach_parameter_1);
  ConvertAngleToXy();
  E->roach_var_01 = R22_;
  E->roach_var_00 = R24_;
  E->roach_var_03 = R26_;
  E->roach_var_02 = R28_;
}

void Roach_Func_3(uint16 k) {  // 0xA3A1B0
  Enemy_Roach *E = Get_Roach(k);
  draw_enemy_layer = LOBYTE(E->roach_parameter_1);
  CosineMult8bit(HIBYTE(E->roach_parameter_1));
  E->roach_var_01 = loop_index_end;
  E->roach_var_00 = loop_index;
  draw_enemy_layer = LOBYTE(E->roach_parameter_1);
  SineMult8bitNegative(HIBYTE(E->roach_parameter_1));
  E->roach_var_03 = loop_index_end;
  E->roach_var_02 = loop_index;
}

void Roach_Func_4(uint16 k) {  // 0xA3A1F3
  Enemy_Roach *E = Get_Roach(k);
  draw_enemy_layer = LOBYTE(E->roach_parameter_1);
  CosineMult8bit((uint8)(*(uint16 *)((char *)&E->roach_parameter_1 + 1) - 32));
  E->roach_var_05 = loop_index_end;
  E->roach_var_04 = loop_index;
  draw_enemy_layer = LOBYTE(E->roach_parameter_1);
  SineMult8bitNegative((uint8)(*(uint16 *)((char *)&E->roach_parameter_1 + 1) - 32));
  E->roach_var_07 = loop_index_end;
  E->roach_var_06 = loop_index;
}

void Roach_Func_5(uint16 k) {  // 0xA3A23E
  Enemy_Roach *E = Get_Roach(k);
  draw_enemy_layer = LOBYTE(E->roach_parameter_1);
  CosineMult8bit((uint8)(*(uint16 *)((char *)&E->roach_parameter_1 + 1) + 32));
  E->roach_var_09 = loop_index_end;
  E->roach_var_08 = loop_index;
  draw_enemy_layer = LOBYTE(E->roach_parameter_1);
  SineMult8bitNegative((uint8)(*(uint16 *)((char *)&E->roach_parameter_1 + 1) + 32));
  E->roach_var_0B = loop_index_end;
  E->roach_var_0A = loop_index;
}

void Roach_Func_6(uint16 k) {  // 0xA3A289
  Enemy_Roach *E = Get_Roach(k);
  E->roach_var_20 = 2 * ((uint8)(*(uint16 *)((char *)&E->roach_parameter_1 + 1) - 48) >> 5);
}

void Roach_Func_7(uint16 k) {  // 0xA3A29E
  Enemy_Roach *E = Get_Roach(k);
  E->roach_var_21 = 2 * ((uint8)(*(uint16 *)((char *)&E->roach_parameter_1 + 1) - 80) >> 5);
}

void Roach_Func_8(uint16 k) {  // 0xA3A2B7
  Enemy_Roach *E = Get_Roach(k);
  E->roach_var_22 = 2 * ((uint8)(*(uint16 *)((char *)&E->roach_parameter_1 + 1) - 48 + 32) >> 5);
}
void CallRoachFunc(uint32 ea) {
  switch (ea) {
  case fnRoach_Func_9: Roach_Func_9(); return;  // 0xa3a2d7
  case fnRoach_Func_10: Roach_Func_10(); return;  // 0xa3a301
  case fnRoach_Func_11: Roach_Func_11(); return;  // 0xa3a30b
  case fnRoach_Func_12: Roach_Func_12(); return;  // 0xa3a315
  case fnRoach_Func_13: Roach_Func_13(); return;  // 0xa3a325
  case fnRoach_Func_14: Roach_Func_14(); return;  // 0xa3a33b
  case fnRoach_Func_15: Roach_Func_15(); return;  // 0xa3a34b
  case fnRoach_Func_16: Roach_Func_16(); return;  // 0xa3a380
  case fnRoach_Func_19: Roach_Func_19(); return;  // 0xa3a407
  case fnRoach_Func_20: Roach_Func_20(); return;  // 0xa3a40e
  case fnRoach_Func_21: Roach_Func_21(); return;  // 0xa3a440
  case fnRoach_Func_22: Roach_Func_22(); return;  // 0xa3a447
  case fnRoach_Func_23: Roach_Func_23(); return;  // 0xa3a44e
  case fnRoach_Func_24: Roach_Func_24(); return;  // 0xa3a462
  case fnRoach_Func_25: Roach_Func_25(); return;  // 0xa3a476
  case fnRoach_Func_26: Roach_Func_26(); return;  // 0xa3a4b6
  case fnRoach_Func_27: Roach_Func_27(); return;  // 0xa3a4f0
  default: Unreachable();
  }
}


void Roach_Main(void) {  // 0xA3A2D0
  Enemy_Roach *E = Get_Roach(cur_enemy_index);
  CallRoachFunc(E->roach_var_B | 0xA30000);
}

void Roach_Func_9(void) {  // 0xA3A2D7
  Enemy_Roach *E = Get_Roach(cur_enemy_index);
  if (IsSamusWithinEnemy_X(cur_enemy_index, LOBYTE(E->roach_parameter_2))) {
    if (IsSamusWithinEnemy_Y(cur_enemy_index, LOBYTE(E->roach_parameter_2)))
      E->roach_var_B = g_off_A3A121[HIBYTE(E->roach_parameter_2)];
  }
}

void Roach_Func_10(void) {  // 0xA3A301
  Get_Roach(cur_enemy_index)->roach_var_B = FUNC16(Roach_Func_19);
}

void Roach_Func_11(void) {  // 0xA3A30B
  Get_Roach(cur_enemy_index)->roach_var_B = FUNC16(Roach_Func_20);
}

void Roach_Func_12(void) {  // 0xA3A315
  random_number = 11;
  Get_Roach(cur_enemy_index)->roach_var_B = FUNC16(Roach_Func_27);
}

void Roach_Func_13(void) {  // 0xA3A325
  Enemy_Roach *E = Get_Roach(cur_enemy_index);
  E->roach_var_A = 512;
  random_number = 11;
  E->roach_var_B = FUNC16(Roach_Func_26);
}

void Roach_Func_14(void) {  // 0xA3A33B
  Enemy_Roach *E = Get_Roach(cur_enemy_index);
  E->roach_var_F = 32;
  E->roach_var_B = FUNC16(Roach_Func_23);
}

void Roach_Func_15(void) {  // 0xA3A34B
  uint8 v1 = 64 - CalculateAngleOfSamusFromEnemy(cur_enemy_index);
  Enemy_Roach *E = Get_Roach(cur_enemy_index);
  E->roach_var_C = v1;
  Roach_Func_18(cur_enemy_index);
  Roach_Func_17(cur_enemy_index);
  E->roach_var_24 = g_off_A3A111[E->roach_var_23 >> 1];
  Roach_Func_1();
  E->roach_var_B = FUNC16(Roach_Func_21);
}

void Roach_Func_16(void) {  // 0xA3A380
  uint16 v1 = (uint8)(64 - CalculateAngleOfSamusFromEnemy(cur_enemy_index) + 0x80);
  Enemy_Roach *E = Get_Roach(cur_enemy_index);
  E->roach_var_C = v1;
  Roach_Func_18(cur_enemy_index);
  Roach_Func_17(cur_enemy_index);
  E->roach_var_24 = g_off_A3A111[E->roach_var_23 >> 1];
  Roach_Func_1();
  E->roach_var_B = FUNC16(Roach_Func_22);
}

void Roach_Func_17(uint16 k) {  // 0xA3A3B5
  Enemy_Roach *E = Get_Roach(k);
  E->roach_var_23 = 2 * ((uint8)(E->roach_var_C - 48) >> 5);
}

void Roach_Func_18(uint16 k) {  // 0xA3A3CA
  Enemy_Roach *E = Get_Roach(k);
  draw_enemy_layer = LOBYTE(E->roach_parameter_1);
  CosineMult8bit(E->roach_var_C);
  E->roach_var_0D = loop_index_end;
  E->roach_var_0C = loop_index;
  draw_enemy_layer = LOBYTE(E->roach_parameter_1);
  SineMult8bitNegative(E->roach_var_C);
  E->roach_var_0F = loop_index_end;
  E->roach_var_0E = loop_index;
}

void Roach_Func_19(void) {  // 0xA3A407
  Roach_Func_29(cur_enemy_index);
}

void Roach_Func_20(void) {  // 0xA3A40E
  Enemy_Roach *E = Get_Roach(cur_enemy_index);
  if ((E->base.frame_counter & 0x10) != 0) {
    E->roach_var_24 = g_off_A3A111[E->roach_var_22 >> 1];
    Roach_Func_1();
    Roach_Func_31(cur_enemy_index);
  } else {
    E->roach_var_24 = g_off_A3A111[E->roach_var_21 >> 1];
    Roach_Func_1();
    Roach_Func_30(cur_enemy_index);
  }
}

void Roach_Func_21(void) {  // 0xA3A440
  Roach_Func_32(cur_enemy_index);
}

void Roach_Func_22(void) {  // 0xA3A447
  Roach_Func_32(cur_enemy_index);
}

void Roach_Func_23(void) {  // 0xA3A44E
  Enemy_Roach *E = Get_Roach(cur_enemy_index);
  if ((--E->roach_var_F & 0x8000u) != 0)
    E->roach_var_B = FUNC16(Roach_Func_9);
  else
    Roach_Func_29(cur_enemy_index);
}

void Roach_Func_24(void) {  // 0xA3A462
  Enemy_Roach *E = Get_Roach(cur_enemy_index);
  if ((--E->roach_var_F & 0x8000u) != 0)
    E->roach_var_B = FUNC16(Roach_Func_27);
  else
    Roach_Func_33(cur_enemy_index);
}

void Roach_Func_25(void) {  // 0xA3A476
  Enemy_Roach *E = Get_Roach(cur_enemy_index);
  bool v2 = (--E->roach_var_A & 0x8000u) != 0;
  if (v2) {
LABEL_6:
    Roach_Func_32(cur_enemy_index);
    return;
  }
  v2 = (--E->roach_var_F & 0x8000u) != 0;
  if (!v2) {
    uint16 v3 = Abs16(E->base.x_pos - samus_x_pos);
    if (!sign16(v3 - 96)) {
      uint16 v4 = Abs16(E->base.y_pos - samus_y_pos);
      if (!sign16(v4 - 96))
        Roach_Func_28();
    }
    goto LABEL_6;
  }
  E->roach_var_B = FUNC16(Roach_Func_26);
}

void Roach_Func_26(void) {  // 0xA3A4B6
  uint8 v1 = NextRandom() - 64;
  Enemy_Roach *E = Get_Roach(cur_enemy_index);
  E->roach_var_C += v1;
  Roach_Func_18(cur_enemy_index);
  Roach_Func_17(cur_enemy_index);
  E->roach_var_24 = g_off_A3A111[E->roach_var_23 >> 1];
  Roach_Func_1();
  E->roach_var_F = 32;
  E->roach_var_B = FUNC16(Roach_Func_25);
}

void Roach_Func_27(void) {  // 0xA3A4F0
  uint8 v1 = NextRandom() - 64;
  Enemy_Roach *E = Get_Roach(cur_enemy_index);
  E->roach_var_C += v1;
  Roach_Func_18(cur_enemy_index);
  Roach_Func_17(cur_enemy_index);
  E->roach_var_24 = g_off_A3A111[E->roach_var_23 >> 1];
  Roach_Func_1();
  E->roach_var_F = 32;
  E->roach_var_B = FUNC16(Roach_Func_24);
}

void Roach_Func_28(void) {  // 0xA3A52A
  Enemy_Roach *E = Get_Roach(cur_enemy_index);
  E->roach_var_0D = -E->roach_var_0D;
  E->roach_var_0C = -E->roach_var_0C;
  E->roach_var_0F = -E->roach_var_0F;
  E->roach_var_0E = -E->roach_var_0E;
  uint16 v1 = ((uint8)E->roach_var_23 + 4) & 7;
  E->roach_var_23 = v1;
  E->roach_var_24 = g_off_A3A111[v1 >> 1];
  Roach_Func_1();
}

void Roach_Func_29(uint16 k) {  // 0xA3A578
  Enemy_Roach *E = Get_Roach(k);
  varE24 = E->roach_var_01;
  varE26 = E->roach_var_00;
  varE28 = E->roach_var_03;
  varE2A = E->roach_var_02;
  enemy_population_ptr = HIBYTE(E->roach_parameter_1);
  EnemyFunc_B691();
}

void Roach_Func_30(uint16 k) {  // 0xA3A5A3
  Enemy_Roach *E = Get_Roach(k);
  E->base.x_pos += E->roach_var_05;
  uint16 x_subpos = E->base.x_subpos;
  bool v3 = __CFADD__uint16(E->roach_var_04, x_subpos);
  uint16 v4 = E->roach_var_04 + x_subpos;
  if (v3)
    ++E->base.x_pos;
  E->base.x_subpos = v4;
  E->base.y_pos += E->roach_var_07;
  uint16 y_subpos = E->base.y_subpos;
  v3 = __CFADD__uint16(E->roach_var_06, y_subpos);
  uint16 v6 = E->roach_var_06 + y_subpos;
  if (v3)
    ++E->base.y_pos;
  E->base.y_subpos = v6;
}

void Roach_Func_31(uint16 k) {  // 0xA3A5DA
  Enemy_Roach *E = Get_Roach(k);
  E->base.x_pos += E->roach_var_09;
  uint16 x_subpos = E->base.x_subpos;
  bool v3 = __CFADD__uint16(E->roach_var_08, x_subpos);
  uint16 v4 = E->roach_var_08 + x_subpos;
  if (v3)
    ++E->base.x_pos;
  E->base.x_subpos = v4;
  E->base.y_pos += E->roach_var_0B;
  uint16 y_subpos = E->base.y_subpos;
  v3 = __CFADD__uint16(E->roach_var_0A, y_subpos);
  uint16 v6 = E->roach_var_0A + y_subpos;
  if (v3)
    ++E->base.y_pos;
  E->base.y_subpos = v6;
}

void Roach_Func_32(uint16 k) {  // 0xA3A611
  Enemy_Roach *E = Get_Roach(k);
  E->base.x_pos += E->roach_var_0D;
  uint16 x_subpos = E->base.x_subpos;
  bool v3 = __CFADD__uint16(E->roach_var_0C, x_subpos);
  uint16 v4 = E->roach_var_0C + x_subpos;
  if (v3)
    ++E->base.x_pos;
  E->base.x_subpos = v4;
  E->base.y_pos += E->roach_var_0F;
  uint16 y_subpos = E->base.y_subpos;
  v3 = __CFADD__uint16(E->roach_var_0E, y_subpos);
  uint16 v6 = E->roach_var_0E + y_subpos;
  if (v3)
    ++E->base.y_pos;
  E->base.y_subpos = v6;
}

void Roach_Func_33(uint16 k) {  // 0xA3A648
  Enemy_Roach *E = Get_Roach(k);
  R18_ = E->roach_var_0C;
  R20_ = E->roach_var_0D;
  if (Enemy_MoveRight_IgnoreSlopes(k) & 1) {
    E->roach_var_B = FUNC16(Roach_Func_9);
  } else {
    R18_ = E->roach_var_0E;
    R20_ = E->roach_var_0F;
    if (Enemy_MoveDown(k) & 1)
      E->roach_var_B = FUNC16(Roach_Func_9);
  }
}

void Mochtroid_Init(void) {  // 0xA3A77D
  Enemy_Mochtroid *E = Get_Mochtroid(cur_enemy_index);
  E->base.layer = 2;
  Mochtroid_Func_4(cur_enemy_index, addr_kMochtroid_Ilist_A745);
  E->mochtr_var_F = 0;
}
static Func_V *const off_A3A7A4[3] = { Mochtroid_Func_1, Mochtroid_Func_3, Mochtroid_Func_2 };
void Mochtroid_Main(void) {  // 0xA3A790
  uint16 v0 = 2 * Get_Mochtroid(cur_enemy_index)->mochtr_var_F;
  Get_Mochtroid(cur_enemy_index)->mochtr_var_F = 0;
  off_A3A7A4[v0 >> 1]();
}

void Mochtroid_Func_1(void) {  // 0xA3A7AA
  int16 v4;
  int16 v5;
  int16 v9;
  int16 v10;

  R18_ = 0;
  R20_ = 0;
  Enemy_Mochtroid *E = Get_Mochtroid(cur_enemy_index);
  *(uint16 *)((char *)&R18_ + 1) = (uint16)(E->base.y_pos - samus_y_pos) >> 2;
  if ((*(uint16 *)((char *)&R18_ + 1) & 0x2000) != 0)
    R20_ = (uint8)R20_ | 0xFFC0;
  uint16 mochtr_var_C = E->mochtr_var_C;
  bool v3 = mochtr_var_C < R18_;
  E->mochtr_var_C = mochtr_var_C - R18_;
  v4 = E->mochtr_var_D - (v3 + R20_);
  E->mochtr_var_D = v4;
  if (v4 < 0) {
    if ((uint16)v4 < 0xFFFDu) {
      v5 = -3;
      goto LABEL_8;
    }
  } else if ((uint16)v4 >= 3u) {
    v5 = 3;
LABEL_8:
    E->mochtr_var_D = v5;
    E->mochtr_var_C = 0;
  }
  if (!E->mochtr_var_C && !E->mochtr_var_D) {
    E->mochtr_var_C = 0;
    E->mochtr_var_D = 0;
  }
  R18_ = E->mochtr_var_C;
  R20_ = E->mochtr_var_D;
  if (Enemy_MoveDown(cur_enemy_index) & 1) {
    E->mochtr_var_C = 0;
    E->mochtr_var_D = 0;
  }
  R18_ = 0;
  R20_ = 0;
  *(uint16 *)((char *)&R18_ + 1) = (uint16)(E->base.x_pos - samus_x_pos) >> 2;
  if ((*(uint16 *)((char *)&R18_ + 1) & 0x2000) != 0)
    R20_ = (uint8)R20_ | 0xFFC0;
  uint16 mochtr_var_A = E->mochtr_var_A;
  v3 = mochtr_var_A < R18_;
  E->mochtr_var_A = mochtr_var_A - R18_;
  v9 = E->mochtr_var_B - (v3 + R20_);
  E->mochtr_var_B = v9;
  if (v9 < 0) {
    if ((uint16)v9 < 0xFFFDu) {
      v10 = -3;
      goto LABEL_21;
    }
  } else if ((uint16)v9 >= 3u) {
    v10 = 3;
LABEL_21:
    E->mochtr_var_B = v10;
    E->mochtr_var_A = 0;
  }
  if (!E->mochtr_var_A && !E->mochtr_var_B) {
    E->mochtr_var_A = 0;
    E->mochtr_var_B = 0;
  }
  R18_ = E->mochtr_var_A;
  R20_ = E->mochtr_var_B;
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index) & 1) {
    E->mochtr_var_A = 0;
    E->mochtr_var_B = 0;
  }
  Mochtroid_Func_4(cur_enemy_index, addr_kMochtroid_Ilist_A745);
}

void Mochtroid_Func_2(void) {  // 0xA3A88F
  Enemy_Mochtroid *E = Get_Mochtroid(cur_enemy_index);
  int v2 = (uint8)(E->mochtr_var_E & 6) >> 1;
  E->base.x_pos += g_word_A3A76D[v2];
  E->base.y_pos += g_word_A3A775[v2];
  E->mochtr_var_A = 0;
  E->mochtr_var_B = 0;
  E->mochtr_var_C = 0;
  E->mochtr_var_D = 0;
  if (E->mochtr_var_E-- == 1)
    E->mochtr_var_F = 0;
  Mochtroid_Func_4(cur_enemy_index, addr_kMochtroid_Ilist_A745);
}

void Mochtroid_Func_3(void) {  // 0xA3A8C8
  Enemy_Mochtroid *E = Get_Mochtroid(cur_enemy_index);
  uint16 x_pos = E->base.x_pos;
  if (x_pos == samus_x_pos) {
    E->mochtr_var_A = 0;
    E->mochtr_var_B = 0;
  } else if ((int16)(x_pos - samus_x_pos) >= 0) {
    E->mochtr_var_A = 0;
    E->mochtr_var_B = -1;
  } else {
    E->mochtr_var_A = 0;
    E->mochtr_var_B = 1;
  }
  R18_ = E->mochtr_var_A;
  R20_ = E->mochtr_var_B;
  Enemy_MoveRight_IgnoreSlopes(cur_enemy_index);
  uint16 y_pos = E->base.y_pos;
  if (y_pos == samus_y_pos) {
    E->mochtr_var_C = 0;
    E->mochtr_var_D = 0;
  } else if ((int16)(y_pos - samus_y_pos) >= 0) {
    E->mochtr_var_C = 0;
    E->mochtr_var_D = -1;
  } else {
    E->mochtr_var_C = 0;
    E->mochtr_var_D = 1;
  }
  R18_ = E->mochtr_var_C;
  R20_ = E->mochtr_var_D;
  Enemy_MoveDown(cur_enemy_index);
}

void Mochtroid_Func_4(uint16 k, uint16 a) {  // 0xA3A93C
  Enemy_Mochtroid *E = Get_Mochtroid(k);
  if (a != E->mochtr_var_01) {
    E->mochtr_var_01 = a;
    E->base.current_instruction = a;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

void Mochtroid_Touch(void) {  // 0xA3A953
  Enemy_Mochtroid *E = Get_Mochtroid(cur_enemy_index);
  E->mochtr_var_F = 1;
  Mochtroid_Func_4(cur_enemy_index, addr_kMochtroid_Ilist_A759);
  ++E->mochtr_var_20;
  if (samus_contact_damage_index)
    goto LABEL_7;
  if ((enemy_damage_routine_exec_count & 7) == 7 && !sign16(samus_health - 30))
    QueueSfx3_Max6(0x2Du);
  if (!sign16(E->mochtr_var_20 - 80)) {
    E->mochtr_var_20 = 0;
LABEL_7:
    Enemy_NormalTouchAI_A3();
    samus_invincibility_timer = 0;
    samus_knockback_timer = 0;
  }
}

void Mochtroid_Shot(void) {  // 0xA3A9A8
  Enemy_NormalShotAI_A3();
}

const uint16 *Sidehopper_Func_1(uint16 k, const uint16 *jp) {  // 0xA3AA68
  QueueSfx2_Max3(*jp);
  return jp + 1;
}

const uint16 *Sidehopper_Instr_1(uint16 k, const uint16 *jp) {  // 0xA3AAFE
  Get_Sidehopper(cur_enemy_index)->sideh_var_04 = 1;
  return jp;
}

void Sidehopper_Init(void) {  // 0xA3AB09
  random_number = 37;
  NextRandom();
  Enemy_Sidehopper *E = Get_Sidehopper(cur_enemy_index);
  E->sideh_var_03 = 0;
  E->sideh_var_04 = 0;
  E->sideh_var_05 = 0;
  uint16 v2 = 2 * get_EnemyDef_A2(E->base.enemy_ptr)->field_2A;
  E->sideh_var_06 = v2;
  uint16 v4;
  if (E->sideh_parameter_1)
    v4 = g_off_A3AACA[E->sideh_var_06 >> 1];
  else
    v4 = g_off_A3AAC2[E->sideh_var_06 >> 1];
  E->sideh_var_00 = v4;
  Sidehopper_Func_3();
  if (get_EnemyDef_A2(E->base.enemy_ptr)->field_2A)
    Get_Sidehopper(cur_enemy_index)->sideh_var_05 = 2;
  int v6 = E->sideh_var_05 >> 1;
  R22_ = g_word_A3AAEE[v6];
  R24_ = g_word_A3AAE6[v6];
  E->sideh_var_01 = Sidehopper_Func_2();
  int v8 = E->sideh_var_05 >> 1;
  R22_ = g_word_A3AAFA[v8];
  R24_ = g_word_A3AAF2[v8];
  E->sideh_var_02 = Sidehopper_Func_2();
  E->sideh_var_B = FUNC16(Sidehopper_Func_4);
}

uint16 Sidehopper_Func_2(void) {  // 0xA3AB9D
  R18_ = 0;
  R20_ = 0;
  do {
    R18_ += R24_;
    R20_ += *(uint16 *)((char *)kCommonEnemySpeeds_Quadratic + (uint16)(8 * R18_) + 1);
  } while (sign16(R20_ - R22_));
  return R18_;
}

void Sidehopper_Func_3(void) {  // 0xA3ABBB
  Enemy_Sidehopper *E = Get_Sidehopper(cur_enemy_index);
  E->base.current_instruction = E->sideh_var_00;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
}

void CallSidehopperFunc(uint32 ea) {
  switch (ea) {
  case fnSidehopper_Func_4: Sidehopper_Func_4(cur_enemy_index); return;  // 0xa3abd6
  case fnSidehopper_Func_5: Sidehopper_Func_5(); return;  // 0xa3abe6
  case fnSidehopper_Func_6: Sidehopper_Func_6(); return;  // 0xa3ac13
  case fnSidehopper_Func_7: Sidehopper_Func_7(); return;  // 0xa3ac40
  case fnSidehopper_Func_8: Sidehopper_Func_8(); return;  // 0xa3ac56
  case fnSidehopper_Func_9: Sidehopper_Func_9(); return;  // 0xa3ac6c
  case fnSidehopper_Func_10: Sidehopper_Func_10(); return;  // 0xa3ac8f
  case fnSidehopper_Func_11: Sidehopper_Func_11(); return;  // 0xa3aca8
  case fnSidehopper_Func_12: Sidehopper_Func_12(); return;  // 0xa3accb
  case fnSidehopper_Func_13: Sidehopper_Func_13(); return;  // 0xa3ace4
  case fnSidehopper_Func_14: Sidehopper_Func_14(); return;  // 0xa3ad0e
  case fnSidehopper_Func_15: Sidehopper_Func_15(); return;  // 0xa3ad20
  case fnSidehopper_Func_16: Sidehopper_Func_16(); return;  // 0xa3ad32
  case fnSidehopper_Func_17: Sidehopper_Func_17(); return;  // 0xa3ad44
  case fnSidehopper_Func_18: Sidehopper_Func_18(); return;  // 0xa3ad56
  default: Unreachable();
  }
}

void Sidehopper_Main(void) {  // 0xA3ABCF
  Enemy_Sidehopper *E = Get_Sidehopper(cur_enemy_index);
  CallSidehopperFunc(E->sideh_var_B | 0xA30000);
}

void Sidehopper_Func_4(uint16 k) {  // 0xA3ABD6
  uint16 v1 = g_off_A3AAE2[NextRandom() & 1];
  Get_Sidehopper(k)->sideh_var_B = v1;
}

void Sidehopper_Func_5(void) {  // 0xA3ABE6
  Enemy_Sidehopper *E = Get_Sidehopper(cur_enemy_index);
  int v1 = E->sideh_var_05 >> 1;
  E->sideh_var_E = g_word_A3AAE6[v1];
  E->sideh_var_D = g_word_A3AAEA[v1];
  E->sideh_var_C = E->sideh_var_01;
  E->sideh_var_B = FUNC16(Sidehopper_Func_7);
  if (E->sideh_parameter_1)
    E->sideh_var_B = FUNC16(Sidehopper_Func_8);
}

void Sidehopper_Func_6(void) {  // 0xA3AC13
  Enemy_Sidehopper *E = Get_Sidehopper(cur_enemy_index);
  int v1 = E->sideh_var_05 >> 1;
  E->sideh_var_E = g_word_A3AAF2[v1];
  E->sideh_var_D = g_word_A3AAF6[v1];
  E->sideh_var_C = E->sideh_var_02;
  E->sideh_var_B = FUNC16(Sidehopper_Func_7);
  if (E->sideh_parameter_1)
    E->sideh_var_B = FUNC16(Sidehopper_Func_8);
}

void Sidehopper_Func_7(void) {  // 0xA3AC40
  Enemy_Sidehopper *E = Get_Sidehopper(cur_enemy_index);
  E->sideh_var_B = FUNC16(Sidehopper_Func_9);
  if ((GetSamusEnemyDelta_X(cur_enemy_index) & 0x8000u) == 0)
    E->sideh_var_B = FUNC16(Sidehopper_Func_10);
}

void Sidehopper_Func_8(void) {  // 0xA3AC56
  Enemy_Sidehopper *E = Get_Sidehopper(cur_enemy_index);
  E->sideh_var_B = FUNC16(Sidehopper_Func_11);
  if ((GetSamusEnemyDelta_X(cur_enemy_index) & 0x8000u) == 0)
    E->sideh_var_B = FUNC16(Sidehopper_Func_12);
}

void Sidehopper_Func_9(void) {  // 0xA3AC6C
  Enemy_Sidehopper *E = Get_Sidehopper(cur_enemy_index);
  E->sideh_var_D = -E->sideh_var_D;
  E->sideh_var_00 = g_off_A3AAD2[E->sideh_var_06 >> 1];
  Sidehopper_Func_3();
  E->sideh_var_B = FUNC16(Sidehopper_Func_14);
}

void Sidehopper_Func_10(void) {  // 0xA3AC8F
  Enemy_Sidehopper *E = Get_Sidehopper(cur_enemy_index);
  E->sideh_var_00 = g_off_A3AAD2[E->sideh_var_06 >> 1];
  Sidehopper_Func_3();
  E->sideh_var_B = FUNC16(Sidehopper_Func_15);
}

void Sidehopper_Func_11(void) {  // 0xA3ACA8
  Enemy_Sidehopper *E = Get_Sidehopper(cur_enemy_index);
  E->sideh_var_D = -E->sideh_var_D;
  E->sideh_var_00 = g_off_A3AADA[E->sideh_var_06 >> 1];
  Sidehopper_Func_3();
  E->sideh_var_B = FUNC16(Sidehopper_Func_16);
}

void Sidehopper_Func_12(void) {  // 0xA3ACCB
  Enemy_Sidehopper *E = Get_Sidehopper(cur_enemy_index);
  E->sideh_var_00 = g_off_A3AADA[E->sideh_var_06 >> 1];
  Sidehopper_Func_3();
  E->sideh_var_B = FUNC16(Sidehopper_Func_17);
}

void Sidehopper_Func_13(void) {  // 0xA3ACE4
  Enemy_Sidehopper *E = Get_Sidehopper(cur_enemy_index);
  E->sideh_var_00 = g_off_A3AAC2[E->sideh_var_06 >> 1];
  if (E->sideh_parameter_1)
    E->sideh_var_00 = g_off_A3AACA[E->sideh_var_06 >> 1];
  Sidehopper_Func_3();
  E->sideh_var_B = FUNC16(Sidehopper_Func_18);
}

void Sidehopper_Func_14(void) {  // 0xA3AD0E
  if (Get_Sidehopper(cur_enemy_index)->sideh_var_03)
    Sidehopper_Func_20();
  else
    Sidehopper_Func_19();
}

void Sidehopper_Func_15(void) {  // 0xA3AD20
  if (Get_Sidehopper(cur_enemy_index)->sideh_var_03)
    Sidehopper_Func_20();
  else
    Sidehopper_Func_19();
}

void Sidehopper_Func_16(void) {  // 0xA3AD32
  if (Get_Sidehopper(cur_enemy_index)->sideh_var_03)
    Sidehopper_Func_22();
  else
    Sidehopper_Func_21();
}

void Sidehopper_Func_17(void) {  // 0xA3AD44
  if (Get_Sidehopper(cur_enemy_index)->sideh_var_03)
    Sidehopper_Func_22();
  else
    Sidehopper_Func_21();
}

void Sidehopper_Func_18(void) {  // 0xA3AD56
  Enemy_Sidehopper *E = Get_Sidehopper(cur_enemy_index);
  if (E->sideh_var_04) {
    E->sideh_var_04 = 0;
    E->sideh_var_B = FUNC16(Sidehopper_Func_4);
  }
}

void Sidehopper_Func_19(void) {  // 0xA3AD6D
  int16 v3;

  Enemy_Sidehopper *E = Get_Sidehopper(cur_enemy_index);
  int v2 = (uint16)(8 * E->sideh_var_C) >> 1;
  R20_ = kCommonEnemySpeeds_Quadratic[v2 + 3];
  R18_ = kCommonEnemySpeeds_Quadratic[v2 + 2];
  if (Enemy_MoveDown(cur_enemy_index) & 1) {
    E->sideh_var_D = -E->sideh_var_D;
    E->sideh_var_03 = 1;
  } else {
    R20_ = E->sideh_var_D;
    R18_ = 0;
    if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index) & 1) {
      E->sideh_var_D = -E->sideh_var_D;
      E->sideh_var_03 = 1;
    } else {
      v3 = E->sideh_var_C - E->sideh_var_E;
      E->sideh_var_C = v3;
      if (v3 < 0) {
        E->sideh_var_03 = 1;
        E->sideh_var_C = 0;
      }
    }
  }
}

void Sidehopper_Func_20(void) {  // 0xA3ADD4
  int16 v3;

  Enemy_Sidehopper *E = Get_Sidehopper(cur_enemy_index);
  int v2 = (uint16)(8 * E->sideh_var_C) >> 1;
  R20_ = kCommonEnemySpeeds_Quadratic[v2 + 1];
  R18_ = kCommonEnemySpeeds_Quadratic[v2];
  if (Enemy_MoveDown(cur_enemy_index) & 1) {
    E->sideh_var_03 = 0;
    E->sideh_var_B = FUNC16(Sidehopper_Func_13);
  } else {
    R20_ = E->sideh_var_D;
    R18_ = 0;
    if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index) & 1)
      E->sideh_var_D = -E->sideh_var_D;
    v3 = E->sideh_var_E + E->sideh_var_C;
    if (!sign16(v3 - 64))
      v3 = 64;
    E->sideh_var_C = v3;
  }
}

void Sidehopper_Func_21(void) {  // 0xA3AE27
  int16 v3;

  Enemy_Sidehopper *E = Get_Sidehopper(cur_enemy_index);
  int v2 = (uint16)(8 * E->sideh_var_C) >> 1;
  R20_ = kCommonEnemySpeeds_Quadratic[v2 + 1];
  R18_ = kCommonEnemySpeeds_Quadratic[v2];
  if (Enemy_MoveDown(cur_enemy_index) & 1) {
    E->sideh_var_D = -E->sideh_var_D;
    E->sideh_var_03 = 1;
  } else {
    R20_ = E->sideh_var_D;
    R18_ = 0;
    if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index) & 1) {
      E->sideh_var_D = -E->sideh_var_D;
      E->sideh_var_03 = 1;
    } else {
      v3 = E->sideh_var_C - E->sideh_var_E;
      E->sideh_var_C = v3;
      if (v3 < 0) {
        E->sideh_var_03 = 1;
        E->sideh_var_C = 0;
      }
    }
  }
}

void Sidehopper_Func_22(void) {  // 0xA3AE8E
  int16 v3;

  Enemy_Sidehopper *E = Get_Sidehopper(cur_enemy_index);
  int v2 = (uint16)(8 * E->sideh_var_C) >> 1;
  R20_ = kCommonEnemySpeeds_Quadratic[v2 + 3];
  R18_ = kCommonEnemySpeeds_Quadratic[v2 + 2];
  if (Enemy_MoveDown(cur_enemy_index) & 1) {
    E->sideh_var_03 = 0;
    E->sideh_var_B = FUNC16(Sidehopper_Func_13);
  } else {
    R20_ = E->sideh_var_D;
    R18_ = 0;
    if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index) & 1)
      E->sideh_var_D = -E->sideh_var_D;
    v3 = E->sideh_var_E + E->sideh_var_C;
    if (!sign16(v3 - 64))
      v3 = 64;
    E->sideh_var_C = v3;
  }
}

const uint16 *MaridiaRefillCandy_Instr_1(uint16 k, const uint16 *jp) {  // 0xA3B429
  Get_MaridiaRefillCandy(cur_enemy_index)->mrcy_var_00 = 4;
  return jp;
}

const uint16 *MaridiaRefillCandy_Instr_2(uint16 k, const uint16 *jp) {  // 0xA3B434
  Get_MaridiaRefillCandy(cur_enemy_index)->mrcy_var_00 = 8;
  return jp;
}

const uint16 *MaridiaRefillCandy_Instr_3(uint16 k, const uint16 *jp) {  // 0xA3B43F
  Get_MaridiaRefillCandy(cur_enemy_index)->mrcy_var_00 = 12;
  return jp;
}

void MaridiaRefillCandy_Init(void) {  // 0xA3B44A
  Enemy_MaridiaRefillCandy *E = Get_MaridiaRefillCandy(cur_enemy_index);
  E->mrcy_parameter_1 = FUNC16(MaridiaRefillCandy_Func_1);
  E->mrcy_var_D = 0;
  E->mrcy_var_E = 0;
  E->mrcy_var_00 = 0;
  E->base.current_instruction = addr_kMaridiaRefillCandy_Ilist_B3C1;
  E->base.properties |= kEnemyProps_Invisible;
  E->mrcy_var_B = E->base.x_pos;
  E->mrcy_var_C = E->base.y_pos;
}

void CallMaridiaRefillCandyFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnMaridiaRefillCandy_Func_1: MaridiaRefillCandy_Func_1(); return;
  case fnMaridiaRefillCandy_Func_2: MaridiaRefillCandy_Func_2(k); return;
  case fnMaridiaRefillCandy_Func_3: MaridiaRefillCandy_Func_3(k); return;
  default: Unreachable();
  }
}

void MaridiaRefillCandy_Main(void) {  // 0xA3B47C
  Enemy_MaridiaRefillCandy *E = Get_MaridiaRefillCandy(cur_enemy_index);
  CallMaridiaRefillCandyFunc(E->mrcy_parameter_1 | 0xA30000, cur_enemy_index);
}

void MaridiaRefillCandy_Func_1(void) {  // 0xA3B482
  int16 v1;

  if (IsSamusWithinEnemy_X(cur_enemy_index, 0x80)) {
    v1 = 1;
    if ((GetSamusEnemyDelta_X(cur_enemy_index) & 0x8000u) == 0)
      v1 = 3;
    Enemy_MaridiaRefillCandy *E = Get_MaridiaRefillCandy(cur_enemy_index);
    E->mrcy_var_D = v1;
    MaridiaRefillCandy_Func_4();
    E->mrcy_parameter_1 = FUNC16(MaridiaRefillCandy_Func_2);
  }
}

void MaridiaRefillCandy_Func_2(uint16 k) {  // 0xA3B4A8
  Enemy_MaridiaRefillCandy *E = Get_MaridiaRefillCandy(k);
  E->base.properties &= ~kEnemyProps_Invisible;
  if ((GetSamusEnemyDelta_Y(k) & 0x8000u) != 0) {
    R20_ = 0;
    R18_ = FUNC16(Enemy_GrappleReact_NoInteract_A3);
    Enemy_SubPos_Y(k);
  } else {
    --E->mrcy_var_D;
    MaridiaRefillCandy_Func_4();
    E->mrcy_var_F = 0;
    E->mrcy_parameter_1 = FUNC16(MaridiaRefillCandy_Func_3);
  }
}

void MaridiaRefillCandy_Func_3(uint16 k) {  // 0xA3B4D6
  Enemy_MaridiaRefillCandy *E = Get_MaridiaRefillCandy(k);
  if (E->mrcy_var_D) {
    int v2;
    v2 = E->mrcy_var_00 >> 1;
    R20_ = g_word_A3B415[v2];
    R18_ = g_word_A3B415[v2 + 1];
    Enemy_AddPos_X(k);
    if (!CheckIfEnemyIsOnScreen()) {
LABEL_3:
      MaridiaRefillCandy_Func_4();
      return;
    }
  } else {
    int v3 = E->mrcy_var_00 >> 1;
    R20_ = g_word_A3B415[v3];
    R18_ = g_word_A3B415[v3 + 1];
    Enemy_SubPos_X(k);
    if (!CheckIfEnemyIsOnScreen())
      goto LABEL_3;
  }
  E->base.properties |= kEnemyProps_Invisible;
  E->base.x_pos = E->mrcy_var_B;
  E->base.y_pos = E->mrcy_var_C;
  E->mrcy_var_D = 0;
  MaridiaRefillCandy_Func_4();
  E->mrcy_parameter_1 = FUNC16(MaridiaRefillCandy_Func_1);
}

void MaridiaRefillCandy_Func_4(void) {  // 0xA3B537
  Enemy_MaridiaRefillCandy *E = Get_MaridiaRefillCandy(cur_enemy_index);
  uint16 mrcy_var_D = E->mrcy_var_D;
  if (mrcy_var_D != E->mrcy_var_E) {
    E->mrcy_var_E = mrcy_var_D;
    E->base.current_instruction = g_off_A3B40D[mrcy_var_D];
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

void MaridiaRefillCandy_Func_5(void) {  // 0xA3B557
  ;
}

void MaridiaRefillCandy_Func_6(void) {  // 0xA3B55B
  ;
}

void NorfairSlowFireball_Init(void) {  // 0xA3B66F
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  v1->parameter_2 = 6;
  v1->current_instruction = g_off_A3B667[v1->properties & 3];
  StoneZoomer_E67A(cur_enemy_index);
}

void NorfairSlowFireball_Func_1(void) {  // 0xA3B6F9
  uint16 current_instruction = gEnemyData(cur_enemy_index)->current_instruction;
  while (current_instruction)
    ;
}

const uint16 *Bang_Instr_1(uint16 k, const uint16 *jp) {  // 0xA3BA78
  QueueSfx2_Max6(0x56u);
  return jp;
}

const uint16 *Bang_Instr_2(uint16 k, const uint16 *jp) {  // 0xA3BAA8
  Get_Bang(cur_enemy_index)->bang_var_22 = 1;
  return jp;
}

void Bang_Init(void) {  // 0xA3BAB3
  Enemy_Bang *E = Get_Bang(cur_enemy_index);
  E->bang_var_B = E->base.palette_index;
  E->base.properties |= kEnemyProps_BlockPlasmaBeam;
  E->bang_var_F = FUNC16(Bang_Func_6);
  E->bang_var_00 = 16;
  E->bang_var_01 = 0;
  E->bang_var_02 = 0;
  E->bang_var_20 = 0;
  E->bang_var_21 = 0;
  E->bang_var_22 = 0;
  E->bang_var_0B = g_word_A3BA84[(uint16)(2 * LOBYTE(E->bang_parameter_2)) >> 1];
  int v1 = (uint16)(4 * HIBYTE(E->bang_parameter_2)) >> 1;
  uint16 v2 = g_word_A3BC6A[v1];
  E->bang_var_0C = v2;
  E->bang_var_0D = v2;
  E->bang_var_0E = g_word_A3BC6A[v1 + 1];
  if (!E->base.current_instruction)
    E->bang_var_F = FUNC16(Bang_Func_7);
  E->base.current_instruction = addr_kBang_Ilist_B75E;
}

void Bang_Main(void) {  // 0xA3BB25
  Enemy_Bang *E = Get_Bang(cur_enemy_index);
  Call(E->bang_parameter_1 | 0xA30000);
}

void Bang_Func_1(void) {  // 0xA3BB2B
  uint16 v0 = Get_Bang(cur_enemy_index + 64)->bang_var_20 + 10;
  Get_Bang(cur_enemy_index)->bang_var_20 = v0;
  Bang_Func_18();
  Enemy_Bang *E = Get_Bang(cur_enemy_index);
  E->base.properties |= kEnemyProps_Tangible;
}

void Bang_Func_2(void) {  // 0xA3BB4A
  uint16 v0 = Get_Bang(cur_enemy_index + 1984)->bang_var_00 + 20;
  Enemy_Bang *E = Get_Bang(cur_enemy_index);
  E->bang_var_20 = v0;
  Bang_Func_18();
  E->base.properties |= kEnemyProps_Tangible;
}

void Bang_Func_3(void) {  // 0xA3BB66
  int16 bang_var_B;

  Bang_Func_5();
  Enemy_Bang *E0 = Get_Bang(cur_enemy_index);
  uint16 x_pos = E0->base.x_pos;
  int v3 = cur_enemy_index >> 1;
  enemy_drawing_queue[v3 + 91] = x_pos;
  Enemy_Bang *E1 = Get_Bang(cur_enemy_index + 64);
  E1->base.x_pos = x_pos;
  uint16 y_pos = E0->base.y_pos;
  enemy_drawing_queue[v3 + 93] = y_pos;
  E1->base.y_pos = y_pos;
  bang_var_B = E0->bang_var_B;
  if ((E0->bang_var_20 & 1) != 0)
    bang_var_B = 3072;
  E0->base.palette_index = bang_var_B;
  Bang_Func_18();
  if (E0->bang_var_22) {
    E0->bang_var_22 = 0;
    if (E0->bang_var_20 == 9) {
      E0->base.invincibility_timer = 16;
      E0->base.properties |= kEnemyProps_Tangible;
      uint16 v7 = DetermineDirectionOfSamusFromEnemy();
      uint16 v8 = Bang_Func_4(v7);
      EnemyDeathAnimation(cur_enemy_index, v8);
      uint16 v9 = cur_enemy_index;
      Enemy_Bang *E = Get_Bang(cur_enemy_index + 64);
      E->base.properties |= 0x200u;
      enemy_drawing_queue[(v9 >> 1) + 97] |= 0x200u;
    } else {
      ++E0->bang_var_20;
      Bang_Func_18();
    }
  }
}

uint16 Bang_Func_4(uint16 a) {  // 0xA3BBEB
  int i;

  enemy_bottom_border_collision = a;
  if (!sign16(projectile_counter - 5))
    return 1;
  for (i = 0; projectile_damage[i >> 1]; i += 2)
    ;
  Enemy_Bang *E = Get_Bang(cur_enemy_index);
  int v3 = i >> 1;
  projectile_x_pos[v3] = E->base.x_pos;
  projectile_y_pos[v3] = E->base.y_pos;
  projectile_dir[v3] = enemy_bottom_border_collision;
  projectile_type[v3] = equipped_beams & 0xF | 0x10;
  ++projectile_counter;
  R20_ = i;
  ProjectileReflection();
  projectile_damage[v3] = E->bang_var_E;
  QueueSfx1_Max6(g_word_A3BC4A[projectile_type[v3] & 0xF]);
  return 0;
}

void Bang_Func_5(void) {  // 0xA3BC9E
  Enemy_Bang *E = Get_Bang(cur_enemy_index);
  CallEnemyPreInstr(E->bang_var_F | 0xA30000);
}

void Bang_Func_6(uint16 k) {  // 0xA3BCA5
  Enemy_Bang *E = Get_Bang(cur_enemy_index);
  uint16 v1 = E->bang_var_00 - 1;
  E->bang_var_00 = v1;
  if (!v1) {
    E->bang_var_00 = 16;
    E->bang_var_F = FUNC16(Bang_Func_8);
  }
}

void Bang_Func_7(uint16 k) {  // 0xA3BCC1
  ;
}

void Bang_Func_8(uint16 k) {  // 0xA3BCC5
  uint8 v1 = CalculateAngleOfSamusFromEnemy(cur_enemy_index) - 64;
  Enemy_Bang *E = Get_Bang(cur_enemy_index);
  E->bang_var_01 = v1;
  E->bang_var_F = FUNC16(Bang_Func_10);
  E->bang_var_07 = 0;
  E->bang_var_08 = 0;
  E->bang_var_09 = 0;
  E->bang_var_0A = 0;
}

void Bang_Func_9(void) {  // 0xA3BCF1
  uint8 v1 = CalculateAngleOfSamusFromEnemy(cur_enemy_index) - 64;
  Enemy_Bang *E = Get_Bang(cur_enemy_index);
  E->bang_var_02 = v1;
  uint16 v3 = SignExtend8(v1 - E->bang_var_01);
  uint16 v4 = Abs16(v3);
  if (!sign16(v4 - 48))
    E->bang_var_F = FUNC16(Bang_Func_11);
}

void Bang_Func_10(uint16 k) {  // 0xA3BD1C
  Bang_Func_14();
  Bang_Func_15();
  Bang_Func_12();
  Bang_Func_9();
}

void Bang_Func_11(uint16 k) {  // 0xA3BD2C
  Bang_Func_14();
  Bang_Func_15();
  Bang_Func_13();
  Enemy_Bang *E = Get_Bang(cur_enemy_index);
  if (E->bang_var_0A || (int16)E->bang_var_09 <= 0)
    E->bang_var_F = FUNC16(Bang_Func_6);
}

void Bang_Func_12(void) {  // 0xA3BD4F
  int16 v1;

  Enemy_Bang *E = Get_Bang(cur_enemy_index);
  v1 = E->bang_var_0C - 1;
  E->bang_var_0C = v1;
  if (v1 < 0) {
    E->bang_var_0C = E->bang_var_0D;
    if ((int16)(E->bang_var_0A - E->bang_var_0B) < 0) {
      E->bang_var_09 += 22;
      E->bang_var_0A += E->bang_var_09;
    }
  }
}

void Bang_Func_13(void) {  // 0xA3BD89
  int16 v1;

  Enemy_Bang *E = Get_Bang(cur_enemy_index);
  v1 = E->bang_var_0C - 1;
  E->bang_var_0C = v1;
  if (v1 < 0) {
    E->bang_var_0C = E->bang_var_0E;
    E->bang_var_09 -= 22;
    E->bang_var_0A -= E->bang_var_09;
  }
}

void Bang_Func_14(void) {  // 0xA3BDB9
  char v2; // t0

  R18_ = kSine16bit[(uint8)(Get_Bang(cur_enemy_index)->bang_var_01 + 64)];
  if ((R18_ & 0x8000u) != 0)
    Get_Bang(cur_enemy_index)->bang_var_07 = 1;
  uint16 v1 = Abs16(R18_) & 0xFF00;
  v2 = v1;
  LOBYTE(v1) = HIBYTE(v1);
  HIBYTE(v1) = v2;
  R22_ = v1;
  Enemy_Bang *E = Get_Bang(cur_enemy_index);
  R24_ = E->bang_var_0A;
  Bang_Func_16();
  R22_ = R28_;
  R24_ = g_word_7E001E;
  if (E->bang_var_07) {
    Bang_Func_17();
    R22_ = R28_;
    R24_ = g_word_7E001E;
  }
  uint16 x_subpos = E->base.x_subpos;
  bool v5 = __CFADD__uint16(R22_, x_subpos);
  E->base.x_subpos = R22_ + x_subpos;
  E->base.x_pos += R24_ + v5;
}

void Bang_Func_15(void) {  // 0xA3BE1C
  char v2; // t0

  R18_ = kSine16bit[(uint8)Get_Bang(cur_enemy_index)->bang_var_01];
  if ((R18_ & 0x8000u) != 0)
    Get_Bang(cur_enemy_index)->bang_var_08 = 1;
  uint16 v1 = Abs16(R18_) & 0xFF00;
  v2 = v1;
  LOBYTE(v1) = HIBYTE(v1);
  HIBYTE(v1) = v2;
  R22_ = v1;
  Enemy_Bang *E = Get_Bang(cur_enemy_index);
  R24_ = E->bang_var_0A;
  Bang_Func_16();
  R22_ = R28_;
  R24_ = g_word_7E001E;
  if (E->bang_var_08) {
    Bang_Func_17();
    R22_ = R28_;
    R24_ = g_word_7E001E;
  }
  uint16 y_subpos = E->base.y_subpos;
  bool v5 = __CFADD__uint16(R22_, y_subpos);
  E->base.y_subpos = R22_ + y_subpos;
  E->base.y_pos += R24_ + v5;
}

void Bang_Func_16(void) {  // 0xA3BE7B
  int16 v1;

  R28_ = Mult8x8(R22_, R24_);
  uint16 prod = Mult8x8(R22_, HIBYTE(R24_));
  g_word_7E001E = (uint8)((uint16)(prod & 0xFF00) >> 8);
  uint16 RegWord = (uint8)prod;
  LOBYTE(v1) = HIBYTE(RegWord);
  HIBYTE(v1) = RegWord;
  bool v2 = __CFADD__uint16(R28_, v1);
  R28_ += v1;
  if (v2)
    ++g_word_7E001E;
}

void Bang_Func_17(void) {  // 0xA3BEBE
  if (R22_ || R24_) {
    R28_ = -R22_;
    g_word_7E001E = ~R24_;
  }
}

void Bang_Func_18(void) {  // 0xA3BEDA
  Enemy_Bang *E = Get_Bang(cur_enemy_index);
  uint16 bang_var_20 = E->bang_var_20;
  if (bang_var_20 != E->bang_var_21) {
    E->bang_var_21 = bang_var_20;
    E->base.current_instruction = g_off_A3B722[bang_var_20];
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

void Bang_Shot(void) {  // 0xA3BEFD
  Enemy_Bang *E = Get_Bang(cur_enemy_index);
  if (E->bang_var_F != 0xBCC1) {
    E->bang_var_01 = g_word_A3BA94[(uint16)(2
                                            * (projectile_dir[collision_detection_index] & 0xF)) >> 1];
    E->bang_var_F = FUNC16(Bang_Func_11);
    E->bang_var_07 = 0;
    E->bang_var_08 = 0;
    E->bang_var_09 = 256;
    E->bang_var_0A = 1536;
  }
  if (E->bang_var_20 != 9) {
    ++E->bang_var_20;
    Bang_Func_18();
    int v1 = collision_detection_index;
    E->bang_var_E += projectile_damage[v1];
    projectile_dir[v1] |= 0x10u;
    if (E->bang_var_20 == 9)
      E->bang_var_D = 1;
  }
}

const uint16 *Skree_Instr_1(uint16 k, const uint16 *jp) {  // 0xA3C6A4
  Get_Skree(cur_enemy_index)->skree_var_E = 1;
  return jp;
}

void Skree_Init(void) {  // 0xA3C6AE
  Enemy_Skree *E = Get_Skree(cur_enemy_index);
  E->skree_var_C = 0;
  E->skree_var_D = 0;
  E->skree_var_E = 0;
  E->base.current_instruction = addr_kSkree_Ilist_C65E;
  E->skree_var_B = FUNC16(Skree_Func_1);
}

void CallSkreeFunc(uint32 ea) {
  switch (ea) {
  case fnSkree_Func_1: Skree_Func_1(); return;
  case fnSkree_Func_2: Skree_Func_2(cur_enemy_index); return;
  case fnSkree_Func_3: Skree_Func_3(); return;
  case fnSkree_Func_4: Skree_Func_4(); return;
  default: Unreachable();
  }
}
void Skree_Main(void) {  // 0xA3C6C7
  Enemy_Skree *E = Get_Skree(cur_enemy_index);
  CallSkreeFunc(E->skree_var_B | 0xA30000);
}

void Skree_Func_1(void) {  // 0xA3C6D5
  Enemy_Skree *E = Get_Skree(cur_enemy_index);
  if (abs16(E->base.x_pos - samus_x_pos) < 0x30u) {
    ++E->skree_var_C;
    Skree_Func_5();
    E->skree_var_B = FUNC16(Skree_Func_2);
  }
}

void Skree_Func_2(uint16 k) {  // 0xA3C6F7
  Enemy_Skree *E = Get_Skree(k);
  if (E->skree_var_E) {
    E->skree_var_E = 0;
    ++E->skree_var_C;
    Skree_Func_5();
    E->skree_var_B = FUNC16(Skree_Func_3);
    QueueSfx2_Max6(0x5Bu);
  }
}

void Skree_Func_3(void) {  // 0xA3C716
  Enemy_Skree *E = Get_Skree(cur_enemy_index);
  E->skree_var_A = 21;
  R20_ = 6;
  R18_ = 0;
  uint16 v1 = E->base.properties | 3;
  E->base.properties = v1;
  if (EnemyFunc_BF8A(cur_enemy_index, v1) & 1) {
    E->base.instruction_timer = 1;
    E->base.timer = 0;
    E->skree_var_B = FUNC16(Skree_Func_4);
    QueueSfx2_Max6(0x5Cu);
  } else {
    E->base.y_pos += 6;
    varE24 = 1;
    if ((int16)(E->base.x_pos - samus_x_pos) >= 0)
      varE24 = -1;
    E->base.x_pos += varE24;
  }
}

void Skree_Func_4(void) {  // 0xA3C77F
  Enemy_Skree *E = Get_Skree(cur_enemy_index);
  if (E->skree_var_A-- == 1) {
    gEnemySpawnData(cur_enemy_index)->vram_tiles_index = E->base.vram_tiles_index | E->base.palette_index;
    E->base.palette_index = 2560;
    E->base.vram_tiles_index = 0;
    E->base.properties |= kEnemyProps_Deleted;
  } else {
    if (E->skree_var_A == 8) {
      uint16 v2 = cur_enemy_index;
      SpawnEnemyProjectileWithGfx(8u, cur_enemy_index, addr_stru_868BC2);
      SpawnEnemyProjectileWithGfx(0, v2, addr_stru_868BD0);
      SpawnEnemyProjectileWithGfx(0, v2, addr_stru_868BDE);
      SpawnEnemyProjectileWithGfx(0, v2, addr_stru_868BEC);
    }
    ++E->base.y_pos;
  }
}

void Skree_Func_5(void) {  // 0xA3C7D5
  Enemy_Skree *E = Get_Skree(cur_enemy_index);
  uint16 skree_var_C = E->skree_var_C;
  if (skree_var_C != E->skree_var_D) {
    E->skree_var_D = skree_var_C;
    E->base.current_instruction = g_off_A3C69C[skree_var_C];
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

void Skree_Shot(void) {  // 0xA3C7F5
  Enemy_NormalShotAI_SkipSomeParts_A3();
  Enemy_Skree *E = Get_Skree(cur_enemy_index);
  if (!E->base.health) {
    uint16 v1 = cur_enemy_index;
    SpawnEnemyProjectileWithGfx(E->skree_var_A, cur_enemy_index, addr_stru_868BC2);
    SpawnEnemyProjectileWithGfx(0, v1, addr_stru_868BD0);
    SpawnEnemyProjectileWithGfx(0, v1, addr_stru_868BDE);
    SpawnEnemyProjectileWithGfx(0, v1, addr_stru_868BEC);
    uint16 v5 = 2;
    if ((projectile_type[collision_detection_index] & 0xF00) != 512)
      v5 = 0;
    EnemyDeathAnimation(2 * collision_detection_index, v5);
  }
}

const uint16 *MaridiaSnail_Instr_1(uint16 k, const uint16 *jp) {  // 0xA3CC36
  Get_MaridiaSnail(k)->msl_var_F = *jp;
  return jp + 1;
}

const uint16 *MaridiaSnail_Instr_2(uint16 k, const uint16 *jp) {  // 0xA3CC3F
  Get_MaridiaSnail(k)->msl_var_D = *jp;
  return jp + 1;
}

const uint16 *MaridiaSnail_Instr_4(uint16 k, const uint16 *jp) {  // 0xA3CC48
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  E->msl_var_07 = jp[0];
  E->msl_var_C = *(uint16 *)((char *)&g_stru_A3CD42[0].field_6 + 8 * E->msl_var_07);
  return jp + 1;
}

const uint16 *MaridiaSnail_Instr_3(uint16 k, const uint16 *jp) {  // 0xA3CC5F
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  E->base.x_pos += jp[0];
  E->base.y_pos += jp[1];
  return jp + 2;
}

const uint16 *MaridiaSnail_Instr_5(uint16 k, const uint16 *jp) {  // 0xA3CC78
  if (Get_MaridiaSnail(k)->msl_var_08 == 2 || (NextRandom() & 1) != 0)
    jp -= 3;
  return jp;
}

void MaridiaSnail_Func_1(uint16 k) {  // 0xA3CC92
  MaridiaSnail_Func_17(k);
}

void MaridiaSnail_Init(void) {  // 0xA3CDE2
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(cur_enemy_index);
  E->msl_var_F = FUNC16(nullsub_215);
  E->base.spritemap_pointer = addr_kSpritemap_Nothing_A3;
  E->base.instruction_timer = 1;
  uint16 current_instruction = E->base.current_instruction;
  E->base.current_instruction = g_stru_A3CD42[current_instruction].field_0;
  E->base.properties |= g_stru_A3CD42[current_instruction].field_2;
  E->msl_var_D = g_stru_A3CD42[current_instruction].field_4;
  E->msl_var_C = g_stru_A3CD42[current_instruction].field_6;
  E->msl_var_08 = 0;
  E->msl_var_06 = E->msl_parameter_1;
  MaridiaSnail_Func_2(cur_enemy_index, current_instruction * 8);
}

void MaridiaSnail_Func_2(uint16 k, uint16 j) {  // 0xA3CE27
  int v2 = j >> 1;
  R18_ = g_word_A3CD82[v2];
  R20_ = g_word_A3CD82[v2 + 1];
  R22_ = g_word_A3CD82[v2 + 2];
  R24_ = g_word_A3CD82[v2 + 3];
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  int v4 = (uint16)(2 * E->msl_parameter_1) >> 1;
  E->msl_var_A = R20_ + (R18_ ^ g_word_A3CCA2[v4]);
  E->msl_var_B = R24_ + (R22_ ^ g_word_A3CCA2[v4]);
}

void MaridiaSnail_Func_3(uint16 k) {  // 0xA3CE57
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  E->msl_var_F = g_off_A3CDD2[E->msl_var_07];
}

void MaridiaSnail_Main(void) {  // 0xA3CE64
  MaridiaSnail_Func_4(cur_enemy_index);
  MaridiaSnail_Func_5(cur_enemy_index);
  MaridiaSnail_Func_6(cur_enemy_index);
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(cur_enemy_index);
  CallEnemyPreInstr(E->msl_var_F | 0xA30000);
}

void MaridiaSnail_Func_4(uint16 k) {  // 0xA3CE73
  uint16 msl_var_08 = Get_MaridiaSnail(k)->msl_var_08;
  if (msl_var_08 != 3 && msl_var_08 != 4 && msl_var_08 != 5 && earthquake_timer == 30 && earthquake_type == 20)
    MaridiaSnail_Func_14(k);
}

void MaridiaSnail_Func_5(uint16 k) {  // 0xA3CE9A
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  uint16 msl_var_08 = E->msl_var_08;
  if (msl_var_08 != 1 && msl_var_08 != 3 && msl_var_08 != 4 && msl_var_08 != 5) {
    if (!sign16(E->base.y_pos - samus_y_pos + 96)) {
      if ((int16)(E->base.x_pos - samus_x_pos) < 0) {
        if (samus_pose_x_dir != 4)
          goto LABEL_14;
      } else if (samus_pose_x_dir != 8) {
        goto LABEL_14;
      }
      if (E->msl_var_08 == 2)
        return;
      if (E->msl_var_D != FUNC16(nullsub_215)) {
        E->base.current_instruction = E->msl_var_D;
        E->base.spritemap_pointer = addr_kSpritemap_Nothing_A3;
        E->base.instruction_timer = 1;
        E->base.timer = 0;
        E->msl_var_08 = 2;
        return;
      }
    }
LABEL_14:
    E->msl_var_08 = 0;
  }
}

void MaridiaSnail_Func_6(uint16 k) {  // 0xA3CF11
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  if (E->msl_var_F == FUNC16(MaridiaSnail_Func_15))
    goto LABEL_11;
  if (E->msl_var_08 == 1) {
    if (samus_has_momentum_flag || E->msl_var_F != FUNC16(nullsub_215))
      goto LABEL_11;
LABEL_10:
    E->base.properties |= 0x8000u;
    return;
  }
  if (!samus_has_momentum_flag) {
    uint16 msl_var_08 = E->msl_var_08;
    if (msl_var_08 == 2 || msl_var_08 != 3 && msl_var_08 != 5)
      goto LABEL_10;
  }
LABEL_11:
  E->base.properties &= 0x7FFFu;
}

void MaridiaSnail_Func_7(uint16 k) {  // 0xA3CF60
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  if (E->msl_var_07 >= 4u) {
    R18_ = 0;
    R20_ = 0;
    MaridiaSnail_Func_8(E->msl_var_B);
    if (EnemyFunc_BC76(k) & 1)
      return;
    goto LABEL_5;
  }
  R18_ = 0;
  R20_ = 0;
  MaridiaSnail_Func_8(E->msl_var_A);
  if (!(EnemyFunc_BBBF(k) & 1))
    LABEL_5:
  MaridiaSnail_Func_14(k);
}

void MaridiaSnail_Func_8(uint16 a) {  // 0xA3CF8F
  if ((a & 0x8000u) != 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = a;
  uint16 v1;
  if ((R20_ & 0x8000u) == 0)
    v1 = R20_ + 7;
  else
    v1 = R20_ - 7;
  R20_ = v1;
}

void MaridiaSnail_Func_9(uint16 k) {  // 0xA3CFA6
  uint16 v1;
  if (Get_MaridiaSnail(k)->msl_var_05)
    v1 = addr_stru_A3CD22;
  else
    v1 = addr_stru_A3CCE2;
  MaridiaSnail_D07E(k, v1);
}

void MaridiaSnail_CFB7(uint16 k) {  // 0xA3CFB7
  MaridiaSnail_D002(k, addr_stru_A3CCEA);
}

void MaridiaSnail_CFBD(uint16 k) {  // 0xA3CFBD
  uint16 v1;
  if (Get_MaridiaSnail(k)->msl_var_05)
    v1 = addr_stru_A3CD2A;
  else
    v1 = addr_stru_A3CCF2;
  MaridiaSnail_D07E(k, v1);
}

void MaridiaSnail_CFCE(uint16 k) {  // 0xA3CFCE
  MaridiaSnail_D002(k, addr_stru_A3CCFA);
}

void MaridiaSnail_CFD4(uint16 k) {  // 0xA3CFD4
  uint16 v1;
  if (Get_MaridiaSnail(k)->msl_var_05)
    v1 = addr_stru_A3CD32;
  else
    v1 = addr_stru_A3CD02;
  MaridiaSnail_D07E(k, v1);
}

void MaridiaSnail_CFE5(uint16 k) {  // 0xA3CFE5
  MaridiaSnail_D002(k, addr_stru_A3CD0A);
}

void MaridiaSnail_CFEB(uint16 k) {  // 0xA3CFEB
  uint16 v1;
  if (Get_MaridiaSnail(k)->msl_var_05)
    v1 = addr_stru_A3CD3A;
  else
    v1 = addr_stru_A3CD12;
  MaridiaSnail_D07E(k, v1);
}

void MaridiaSnail_CFFC(uint16 k) {  // 0xA3CFFC
  MaridiaSnail_D002(k, addr_stru_A3CD1A);
}

void MaridiaSnail_D002(uint16 k, uint16 j) {  // 0xA3D002
  int16 msl_var_A;
  int16 msl_var_B;

  MaridiaSnail_Func_10(k, j);
  R18_ = 0;
  R20_ = 0;
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  msl_var_A = E->msl_var_A;
  if (msl_var_A < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = msl_var_A;
  uint16 v4;
  if ((R20_ & 0x8000u) == 0)
    v4 = R20_ + 1;
  else
    v4 = R20_ - 1;
  R20_ = v4;
  uint8 rv = Enemy_MoveRight_ProcessSlopes(k);
  MaridiaSnail_Func_11(k, j);
  if (rv) {
    E->msl_var_E = 0;
    uint8 carry = EnemyFunc_C8AD(k);
    MaridiaSnail_Func_12(k, carry);
    R18_ = 0;
    R20_ = 0;
    msl_var_B = E->msl_var_B;
    if (msl_var_B < 0)
      --R20_;
    *(uint16 *)((char *)&R18_ + 1) = msl_var_B;
    if (Enemy_MoveDown(k) & 1) {
      E->msl_var_A = -E->msl_var_A;
      const uint8 *v6 = RomPtr_A3(j);
      MaridiaSnail_Func_13(k, *((uint16 *)v6 + 3));
    }
  } else {
    uint16 v7 = E->msl_var_E + 1;
    E->msl_var_E = v7;
    if (sign16(v7 - 4)) {
      E->msl_var_B = -E->msl_var_B;
      const uint8 *v8 = RomPtr_A3(j);
      MaridiaSnail_Func_13(k, *((uint16 *)v8 + 2));
    } else {
      MaridiaSnail_Func_14(k);
    }
  }
}

void MaridiaSnail_D07E(uint16 k, uint16 j) {  // 0xA3D07E
  int16 msl_var_B;
  int16 msl_var_A;

  MaridiaSnail_Func_10(k, j);
  R18_ = 0;
  R20_ = 0;
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  msl_var_B = E->msl_var_B;
  if (msl_var_B < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = msl_var_B;
  uint16 v4;
  if ((R20_ & 0x8000u) == 0)
    v4 = R20_ + 1;
  else
    v4 = R20_ - 1;
  R20_ = v4;
  uint8 rv = Enemy_MoveDown(k);
  MaridiaSnail_Func_11(k, j);
  if (rv) {
    E->msl_var_E = 0;
    R18_ = 0;
    R20_ = 0;
    msl_var_A = E->msl_var_A;
    if (msl_var_A < 0)
      --R20_;
    *(uint16 *)((char *)&R18_ + 1) = msl_var_A;
    if (Enemy_MoveRight_ProcessSlopes(k) & 1) {
      E->msl_var_B = -E->msl_var_B;
      const uint8 *v6 = RomPtr_A3(j);
      MaridiaSnail_Func_13(k, *((uint16 *)v6 + 3));
    } else {
      uint8 carry = EnemyFunc_C8AD(k);
      MaridiaSnail_Func_12(k, carry);
    }
  } else {
    uint16 v7 = E->msl_var_E + 1;
    E->msl_var_E = v7;
    if (sign16(v7 - 4)) {
      E->msl_var_A = -E->msl_var_A;
      const uint8 *v8 = RomPtr_A3(j);
      MaridiaSnail_Func_13(k, *((uint16 *)v8 + 2));
    } else {
      MaridiaSnail_Func_14(k);
    }
  }
}

void MaridiaSnail_Func_10(uint16 k, uint16 j) {  // 0xA3D0F8
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  const uint8 *v3 = RomPtr_A3(j);
  E->base.x_pos += *(uint16 *)v3;
  E->base.y_pos += *((uint16 *)v3 + 1);
}

void MaridiaSnail_Func_11(uint16 k, uint16 j) {  // 0xA3D10D
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  const uint8 *v3 = RomPtr_A3(j);
  E->base.x_pos -= *(uint16 *)v3;
  E->base.y_pos = E->base.y_pos - *((uint16 *)v3 + 1);
}

void MaridiaSnail_Func_12(uint16 k, uint16 carry) {  // 0xA3D124
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  if (!carry) {
    if ((uint16)(E->msl_var_04 + 1) >= 0x10u)
      E->msl_var_05 = 0;
    else
      ++E->msl_var_04;
  } else {
    E->msl_var_05 = 1;
    E->msl_var_04 = 0;
  }
}

void MaridiaSnail_Func_13(uint16 k, uint16 a) {  // 0xA3D14C
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  E->base.current_instruction = a;
  E->base.instruction_timer = 1;
  E->msl_var_05 = 1;
  E->msl_var_04 = 0;
}

void MaridiaSnail_Func_14(uint16 k) {  // 0xA3D164
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  if (E->msl_var_08 != 3) {
    E->msl_var_08 = 3;
    E->msl_var_F = FUNC16(MaridiaSnail_Func_15);
    int v2 = (uint16)(4 * E->msl_var_C) >> 1;
    E->base.current_instruction = g_off_A3D1AB[v2];
    E->msl_var_D = g_off_A3D1AB[v2 + 1];
    E->base.instruction_timer = 1;
    E->base.timer = 0;
    E->msl_var_02 = 0;
    E->msl_var_03 = 0;
    E->msl_var_00 = 0;
    E->msl_var_01 = 0;
  }
}

void MaridiaSnail_Func_15(uint16 k) {  // 0xA3D1B3
  int16 msl_var_01;

  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  if (E->msl_var_08 != 3) {
    R18_ = E->msl_var_02;
    R20_ = E->msl_var_03;
    if (Enemy_MoveRight_IgnoreSlopes(k) & 1) {
      E->msl_var_02 = -E->msl_var_02;
      E->msl_var_03 = -E->msl_var_03;
      E->msl_var_20 = 1;
      QueueSfx2_Max3(0x70u);
    } else {
      if ((E->msl_var_03 & 0x8000u) != 0) {
        R22_ = 4096;
        R24_ = 0;
      } else {
        R22_ = -4096;
        R24_ = -1;
      }
      uint16 msl_var_02 = E->msl_var_02;
      bool v3 = __CFADD__uint16(R22_, msl_var_02);
      E->msl_var_02 = R22_ + msl_var_02;
      uint16 v4 = R24_ + v3 + E->msl_var_03;
      if (v4)
        E->msl_var_03 = v4;
    }
  }
  R18_ = E->msl_var_00;
  R20_ = E->msl_var_01;
  if (Enemy_MoveDown(k) & 1) {
    msl_var_01 = E->msl_var_01;
    if (msl_var_01 >= 0 && sign16(msl_var_01 - 3)) {
      E->msl_var_03 = 0;
      E->msl_var_02 = 0;
      E->msl_var_01 = 0;
      E->msl_var_00 = 0;
      E->msl_var_E = 0;
      E->msl_var_04 = 0;
      E->msl_var_05 = 1;
      if (E->msl_var_08 == 3) {
        E->msl_var_08 = 0;
      } else {
        E->msl_var_08 = 1;
        E->msl_parameter_1 = 8;
        MaridiaSnail_Func_18(k);
        MaridiaSnail_Func_16(k);
      }
      E->base.properties = E->msl_var_C | E->base.properties & ~3u;
      StoneZoomer_E67A(k);
      E->msl_var_F = FUNC16(nullsub_215);
      E->base.current_instruction = E->msl_var_D;
      E->base.instruction_timer = 1;
      E->base.timer = 0;
    } else {
      E->msl_var_00 = -E->msl_var_00;
      E->msl_var_01 = -E->msl_var_01;
      E->msl_var_20 = 0;
    }
  } else {
    uint16 msl_var_00 = E->msl_var_00;
    E->msl_var_00 = msl_var_00 + 0x2000;
    uint16 v6 = __CFADD__uint16(msl_var_00, 0x2000) + E->msl_var_01;
    if (sign16(v6 - 4))
      E->msl_var_01 = v6;
  }
}

void MaridiaSnail_Func_16(uint16 k) {  // 0xA3D2FA
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  int v2 = (uint16)(4 * E->msl_var_C) >> 1;
  E->base.current_instruction = g_off_A3D30D[v2];
  E->msl_var_D = g_off_A3D30D[v2 + 1];
}

uint8 MaridiaSnail_Func_17(uint16 k) {  // 0xA3D315
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  if (E->msl_var_05)
    return 0;
  uint16 msl_var_07 = E->msl_var_07;
  if (msl_var_07 >= 4u)
    return MaridiaSnail_Func_18(k) & 1;
  if ((msl_var_07 & 1) != 0) {
    uint16 y_pos = E->base.y_pos;
    if (y_pos >= samus_y_pos)
      return MaridiaSnail_Func_19(k) & 1;
    return y_pos >= samus_y_pos;
  } else {
    if (E->base.y_pos < samus_y_pos)
      return MaridiaSnail_Func_19(k) & 1;
    return 0;
  }
}

uint8 MaridiaSnail_Func_18(uint16 k) {  // 0xA3D33E
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  if (E->msl_var_C) {
    uint16 x_pos = E->base.x_pos;
    if (x_pos >= samus_x_pos)
      return MaridiaSnail_Func_19(k) & 1;
    return x_pos >= samus_x_pos;
  } else {
    if (E->base.x_pos < samus_x_pos)
      return MaridiaSnail_Func_19(k) & 1;
    return 0;
  }
}

uint8 MaridiaSnail_Func_19(uint16 k) {  // 0xA3D356
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  if (E->msl_var_08 == 2 || E->msl_var_F == 0xCF5F)
    return 0;
  uint16 v2 = g_word_A3CDC2[E->msl_var_07];
  E->msl_var_07 = v2;
  E->base.current_instruction = *(VoidP *)((char *)&g_stru_A3CD42[0].field_0 + (uint16)(8 * v2));
  E->base.properties = *(uint16 *)((char *)&g_stru_A3CD42[0].field_2 + (uint16)(8 * v2)) | E->base.properties & ~3u;
  E->msl_var_D = *(VoidP *)((char *)&g_stru_A3CD42[0].field_4 + (uint16)(8 * v2));
  E->msl_var_C = *(uint16 *)((char *)&g_stru_A3CD42[0].field_6 + (uint16)(8 * v2));
  MaridiaSnail_Func_2(k, 8 * v2);
  MaridiaSnail_Func_3(k);
  E->msl_var_05 = 1;
  E->msl_var_04 = 0;
  return 1;
}

void MaridiaSnail_Touch(void) {  // 0xA3D3B0
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(cur_enemy_index);
  if ((E->msl_var_08 != 1
       || E->msl_var_F == 0xCF5F
       || !(MaridiaSnail_Func_20(cur_enemy_index) & 1))
      && (E->msl_var_F == 0xD1B3 || samus_has_momentum_flag)) {
    MaridiaSnail_Func_22(cur_enemy_index);
    if (E->msl_var_F == 0xD1B3)
      QueueSfx2_Max3(0x70u);
  } else if (E->msl_var_F != 0xCF5F && E->msl_var_08 != 4 && E->msl_var_08 != 3) {
    Enemy_NormalTouchAI_A3();
    E->msl_parameter_1 = E->msl_var_06;
    if (E->msl_var_08)
      MaridiaSnail_Func_19(cur_enemy_index);
    E->msl_var_08 = 0;
  }
}

uint8 MaridiaSnail_Func_20(uint16 k) {  // 0xA3D421
  R18_ = ((uint16)(joypad1_lastkeys & 0x300) >> 8) - 1;
  R20_ = Get_MaridiaSnail(k)->msl_var_C & 1;
  if ((uint16)(joypad1_lastkeys & 0x300) >> 8 == 1) {
    if (R20_)
      return 0;
  } else if (!R20_) {
    return 0;
  }
  return 1;
}

uint8 MaridiaSnail_Func_21(uint16 k) {  // 0xA3D446
  if ((Get_MaridiaSnail(k)->msl_var_A & 0x8000u) != 0)
    return samus_pose_x_dir == 8;
  return samus_pose_x_dir == 4;
}

void MaridiaSnail_Shot(void) {  // 0xA3D469
  int16 v0;

  v0 = projectile_type[collision_detection_index] & 0xFF00;
  if (v0 == 768 || v0 == 1280) {
    Enemy_NormalShotAI_A3();
  } else {
    uint16 msl_var_08 = Get_MaridiaSnail(cur_enemy_index)->msl_var_08;
    if (msl_var_08 != 3 && msl_var_08 != 4)
      MaridiaSnail_Func_23(cur_enemy_index);
    QueueSfx2_Max3(0x70u);
  }
}

void MaridiaSnail_Func_22(uint16 k) {  // 0xA3D49F
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  E->msl_var_08 = 4;
  E->msl_var_F = FUNC16(MaridiaSnail_Func_15);
  int v2 = (uint16)(4 * E->msl_var_C) >> 1;
  E->base.current_instruction = g_off_A3D50F[v2];
  E->msl_var_D = g_off_A3D50F[v2 + 1];
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->msl_var_02 = absolute_moved_last_frame_x_fract;
  uint16 v3 = absolute_moved_last_frame_x;
  E->msl_var_03 = absolute_moved_last_frame_x;
  if (v3 >= 0x10u)
    v3 = 15;
  int v4 = (uint16)(4 * v3) >> 1;
  E->msl_var_00 = g_word_A3D517[v4];
  E->msl_var_01 = g_word_A3D517[v4 + 1];
  if ((samus_pose_x_dir & 4) != 0) {
    E->msl_var_02 = -E->msl_var_02;
    E->msl_var_03 = -E->msl_var_03;
  }
}

void MaridiaSnail_Func_23(uint16 k) {  // 0xA3D557
  Enemy_MaridiaSnail *E = Get_MaridiaSnail(k);
  E->msl_var_08 = 5;
  E->msl_var_F = FUNC16(MaridiaSnail_Func_15);
  int v2 = (uint16)(4 * E->msl_var_C) >> 1;
  E->base.current_instruction = g_off_A3D5A4[v2];
  E->msl_var_D = g_off_A3D5A4[v2 + 1];
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->msl_var_01 = -1;
  if (samus_pose_x_dir == 4)
    E->msl_var_03 = -1;
  else
    E->msl_var_03 = 1;
}

void Reflec_Func_1(void) {  // 0xA3DB0C
  if (!door_transition_flag_enemies && !--variables_for_enemy_graphics_drawn_hook[2]) {
    variables_for_enemy_graphics_drawn_hook[2] = 16;
    uint16 v0 = variables_for_enemy_graphics_drawn_hook[0];
    uint16 v1 = 8 * variables_for_enemy_graphics_drawn_hook[1];
    remaining_enemy_spritemap_entries = 4;
    do {
      palette_buffer[(v0 >> 1) + 137] = g_word_A3DABC[v1 >> 1];
      v1 += 2;
      v0 += 2;
      --remaining_enemy_spritemap_entries;
    } while (remaining_enemy_spritemap_entries);
    variables_for_enemy_graphics_drawn_hook[1] = (LOBYTE(variables_for_enemy_graphics_drawn_hook[1]) + 1) & 7;
  }
}

const uint16 *Reflec_Instr_1(uint16 k, const uint16 *jp) {  // 0xA3DBC8
  Get_Reflec(k)->reflec_parameter_2 = jp[0];
  return jp + 1;
}

void Reflec_Init(void) {  // 0xA3DBD3
  Enemy_Reflec *E = Get_Reflec(cur_enemy_index);
  E->base.properties |= kEnemyProps_BlockPlasmaBeam;
  E->base.current_instruction = g_off_A3DC0B[E->reflec_parameter_1];
  enemy_gfx_drawn_hook.addr = FUNC16(Reflec_Func_1);
  *(uint16 *)&enemy_gfx_drawn_hook.bank = 163;
  variables_for_enemy_graphics_drawn_hook[0] = (uint8)((uint16)((16 * E->base.palette_index) & 0xFF00) >> 8);
  variables_for_enemy_graphics_drawn_hook[2] = 16;
}

static Func_V *const off_A3DD2E[64] = {  // 0xA3DC1C
               0, Reflec_Func_2,              0, Reflec_Func_2,              0,              0, Reflec_Func_3,              0, Reflec_Func_3,              0, 0, 0, 0, 0, 0, 0,
   Reflec_Func_4,             0,  Reflec_Func_5,             0,  Reflec_Func_6,  Reflec_Func_6,             0,  Reflec_Func_7,             0,  Reflec_Func_4, 0, 0, 0, 0, 0, 0,
               0, Reflec_Func_8,              0, Reflec_Func_9,              0,              0, Reflec_Func_9,              0, Reflec_Func_8,              0, 0, 0, 0, 0, 0, 0,
  Reflec_Func_10,             0, Reflec_Func_11,             0, Reflec_Func_12, Reflec_Func_12,             0, Reflec_Func_13,             0, Reflec_Func_10, 0, 0, 0, 0, 0, 0,
};

void Reflec_Shot(void) {
  int16 v4;

  uint16 v0 = 2 * collision_detection_index;
  Enemy_Reflec *EK = Get_Reflec(cur_enemy_index);
  EK->base.invincibility_timer = 10;
  draw_enemy_layer = 32 * EK->reflec_parameter_2;
  uint16 v2 = draw_enemy_layer + 2 * (projectile_dir[v0 >> 1] & 0xF);
  draw_enemy_layer = v2;
  int v3 = v2 >> 1;
  if (g_word_A3DCAE[v3] == 0x8000) {
    projectile_dir[v0 >> 1] |= 0x10u;
    printf("Possible bug. What is X?\n");
    Enemy_Reflec *ET = Get_Reflec(v2);
    if (ET->base.health) {
      Enemy_NormalShotAI_SkipSomeParts_A3();
      if (!ET->base.health) {
        ET->base.current_instruction = g_off_A3DCA6[ET->reflec_parameter_2];
        ET->base.instruction_timer = 1;
        ET->base.timer = 0;
      }
    }
  } else {
    v4 = g_word_A3DCAE[v3];
    if (v4 < 0) {
      off_A3DD2E[v3]();
      v4 = -g_word_A3DCAE[draw_enemy_layer >> 1];
    }
    int v5 = v0 >> 1;
    projectile_dir[v5] = v4;
    projectile_type[v5] &= ~0x8000u;
    R20_ = v0;
    ProjectileReflection();
    QueueSfx2_Max6(0x57u);
  }
}

void Reflec_Func_2(void) {  // 0xA3DDAE
  loop_index_end = Get_Reflec(cur_enemy_index)->base.x_pos - 8;
}

void Reflec_Func_3(void) {  // 0xA3DDCF
  loop_index_end = Get_Reflec(cur_enemy_index)->base.x_pos + 8;
}

void Reflec_Func_4(void) {  // 0xA3DDF0
  loop_index_end = Get_Reflec(cur_enemy_index)->base.x_pos;
}

void Reflec_Func_5(void) {  // 0xA3DE0D
  loop_index_end = Get_Reflec(cur_enemy_index)->base.y_pos;
}

void Reflec_Func_6(void) {  // 0xA3DE2A
  loop_index_end = Get_Reflec(cur_enemy_index)->base.x_pos;
}

void Reflec_Func_7(void) {  // 0xA3DE47
  loop_index_end = Get_Reflec(cur_enemy_index)->base.y_pos;
}

void Reflec_Func_8(void) {  // 0xA3DE64
  loop_index_end = Get_Reflec(cur_enemy_index)->base.y_pos + 8;
}

void Reflec_Func_9(void) {  // 0xA3DE85
  loop_index_end = Get_Reflec(cur_enemy_index)->base.y_pos - 8;
}

void Reflec_Func_10(void) {  // 0xA3DEA6
  loop_index_end = Get_Reflec(cur_enemy_index)->base.x_pos;
}

void Reflec_Func_11(void) {  // 0xA3DEC3
  loop_index_end = Get_Reflec(cur_enemy_index)->base.y_pos;
}

void Reflec_Func_12(void) {  // 0xA3DEE0
  loop_index_end = Get_Reflec(cur_enemy_index)->base.x_pos;
}

void Reflec_Func_13(void) {  // 0xA3DEFD
  loop_index_end = Get_Reflec(cur_enemy_index)->base.y_pos;
}

const uint16 *WreckedShipOrangeZoomer_Func_1(uint16 k, const uint16 *jp) {  // 0xA3DFC2
  Get_WreckedShipOrangeZoomer(k)->wsozr_var_F = jp[0];
  return jp + 1;
}

void WreckedShipOrangeZoomer_Init(void) {  // 0xA3E043
  Enemy_WreckedShipOrangeZoomer *E = Get_WreckedShipOrangeZoomer(cur_enemy_index);
  E->base.current_instruction = g_off_A3E03B[(uint16)(2
                                                      * (E->base.current_instruction & 3)) >> 1];
  E->wsozr_var_F = addr_locret_A3E08A;
  uint16 v1 = g_word_A3E5F0[E->wsozr_parameter_1];
  E->wsozr_var_A = v1;
  E->wsozr_var_B = v1;
  if ((E->base.properties & 3) != 0) {
    if ((E->base.properties & 3) == 2)
      E->wsozr_var_B = -E->wsozr_var_B;
  } else {
    E->wsozr_var_A = -E->wsozr_var_A;
  }
}

void WreckedShipOrangeZoomer_Main(void) {  // 0xA3E08B
  Enemy_WreckedShipOrangeZoomer *E = Get_WreckedShipOrangeZoomer(cur_enemy_index);
  CallEnemyPreInstr(E->wsozr_var_F | 0xA30000);
}

void WreckedShipOrangeZoomer_Func_2(uint16 k) {  // 0xA3E091
  int16 wsozr_var_A;
  int16 wsozr_var_B;
  int16 v7;
  uint16 v9;
  uint16 v12;
  Enemy_WreckedShipOrangeZoomer *E = Get_WreckedShipOrangeZoomer(k);

  if (earthquake_timer == 30 && earthquake_type == 20) {
    E->wsozr_var_03 = E->wsozr_var_F;
    E->wsozr_var_F = FUNC16(FireZoomer_Func_2);
  }
  R18_ = 0;
  R20_ = 0;
  wsozr_var_A = E->wsozr_var_A;
  if (wsozr_var_A < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = wsozr_var_A;
  uint16 v4;
  if ((R20_ & 0x8000u) == 0)
    v4 = R20_ + 1;
  else
    v4 = R20_ - 1;
  R20_ = v4;
  if (Enemy_MoveRight_ProcessSlopes(k) & 1) {
    E->wsozr_var_04 = 0;
    EnemyFunc_C8AD(k);
    R18_ = 0;
    R20_ = 0;
    wsozr_var_B = E->wsozr_var_B;
    if (wsozr_var_B < 0)
      --R20_;
    *(uint16 *)((char *)&R18_ + 1) = wsozr_var_B;
    if (!(Enemy_MoveDown(k) & 1)) {
      if ((int16)(samus_y_pos - E->base.y_pos) >= 0) {
        v7 = E->wsozr_var_B;
        if (v7 < 0)
          LABEL_17:
        v7 = -v7;
      } else {
        v7 = E->wsozr_var_B;
        if (v7 >= 0)
          goto LABEL_17;
      }
      E->wsozr_var_B = v7;
      return;
    }
    E->wsozr_var_A = -E->wsozr_var_A;
    v9 = addr_kWreckedShipOrangeZoomer_Ilist_E01F;
    if ((E->wsozr_var_B & 0x8000u) != 0)
      v9 = addr_kWreckedShipOrangeZoomer_Ilist_E003;
    E->base.current_instruction = v9;
    E->base.instruction_timer = 1;
  } else {
    uint16 v11 = E->wsozr_var_04 + 1;
    E->wsozr_var_04 = v11;
    if (sign16(v11 - 4)) {
      E->wsozr_var_B = -E->wsozr_var_B;
      v12 = addr_kWreckedShipOrangeZoomer_Ilist_E01F;
      if ((E->wsozr_var_B & 0x8000u) != 0)
        v12 = addr_kWreckedShipOrangeZoomer_Ilist_E003;
      E->base.current_instruction = v12;
      E->base.instruction_timer = 1;
    } else {
      E->wsozr_var_03 = E->wsozr_var_F;
      E->wsozr_var_F = FUNC16(FireZoomer_Func_2);
    }
  }
}

void sub_A3E168(uint16 k) {  // 0xA3E168
  int16 wsozr_var_B;
  int16 wsozr_var_A;
  int16 v7;
  uint16 v9;
  uint16 v12;
  Enemy_WreckedShipOrangeZoomer *E = Get_WreckedShipOrangeZoomer(k);

  if (earthquake_timer == 30 && earthquake_type == 20) {
    E->wsozr_var_03 = E->wsozr_var_F;
    E->wsozr_var_F = FUNC16(FireZoomer_Func_2);
  }
  R18_ = 0;
  R20_ = 0;

  wsozr_var_B = E->wsozr_var_B;
  if (wsozr_var_B < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = wsozr_var_B;
  uint16 v4;
  if ((R20_ & 0x8000u) == 0)
    v4 = R20_ + 1;
  else
    v4 = R20_ - 1;
  R20_ = v4;
  if (Enemy_MoveDown(k) & 1) {
    E->wsozr_var_04 = 0;
    R18_ = 0;
    R20_ = 0;
    wsozr_var_A = E->wsozr_var_A;
    if (wsozr_var_A < 0)
      --R20_;
    *(uint16 *)((char *)&R18_ + 1) = wsozr_var_A;
    if (!(Enemy_MoveRight_ProcessSlopes(k) & 1)) {
      EnemyFunc_C8AD(k);
      if ((int16)(samus_x_pos - E->base.x_pos) >= 0) {
        v7 = E->wsozr_var_A;
        if (v7 < 0)
          LABEL_17:
        v7 = -v7;
      } else {
        v7 = E->wsozr_var_A;
        if (v7 >= 0)
          goto LABEL_17;
      }
      E->wsozr_var_A = v7;
      return;
    }
    E->wsozr_var_B = -E->wsozr_var_B;
    v9 = addr_kWreckedShipOrangeZoomer_Ilist_DFE7;
    if ((E->wsozr_var_A & 0x8000u) != 0)
      v9 = addr_kWreckedShipOrangeZoomer_Ilist_DFCB;
    E->base.current_instruction = v9;
    E->base.instruction_timer = 1;
  } else {
    uint16 v11 = E->wsozr_var_04 + 1;
    E->wsozr_var_04 = v11;
    if (sign16(v11 - 4)) {
      E->wsozr_var_A = -E->wsozr_var_A;
      v12 = addr_kWreckedShipOrangeZoomer_Ilist_DFE7;
      if ((E->wsozr_var_A & 0x8000u) != 0)
        v12 = addr_kWreckedShipOrangeZoomer_Ilist_DFCB;
      E->base.current_instruction = v12;
      E->base.instruction_timer = 1;
    } else {
      E->wsozr_var_03 = E->wsozr_var_F;
      E->wsozr_var_F = FUNC16(FireZoomer_Func_2);
    }
  }
}

void BigEyeBugs_Init(void) {  // 0xA3E2D4
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  v1->current_instruction = g_off_A3E2CC[v1->current_instruction & 3];
  StoneZoomer_E67A(cur_enemy_index);
}

void FireZoomer_Init(void) {  // 0xA3E59C
  Enemy_FireZoomer *E = Get_FireZoomer(cur_enemy_index);
  E->base.current_instruction = g_off_A3E2CC[(E->base.current_instruction & 3)];
  StoneZoomer_E67A(cur_enemy_index);
}

const uint16 *Zoomer_Instr_SetPreinstr(uint16 k, const uint16 *jp) {  // 0xA3E660
  gEnemyData(k)->ai_preinstr = jp[0];
  return jp + 1;
}

void StoneZoomer_Init(void) {  // 0xA3E669
  Enemy_StoneZoomer *E = Get_StoneZoomer(cur_enemy_index);
  E->base.current_instruction = g_off_A3E2CC[(E->base.current_instruction & 3)];
  StoneZoomer_E67A(cur_enemy_index);
}

void StoneZoomer_E67A(uint16 k) {  // 0xA3E67A
  Enemy_StoneZoomer *E = Get_StoneZoomer(k);
  E->base.spritemap_pointer = addr_kSpritemap_Nothing_A3;
  E->base.instruction_timer = 1;
  E->szr_var_F = FUNC16(nullsub_304);
  if (E->szr_parameter_1 != 255) {
    uint16 v2 = g_word_A3E5F0[E->szr_parameter_1];
    E->szr_var_A = v2;
    E->szr_var_B = v2;
  }
  if ((E->base.properties & 3) != 0) {
    if ((E->base.properties & 3) == 2)
      E->szr_var_B = -E->szr_var_B;
  } else {
    E->szr_var_A = -E->szr_var_A;
  }
}

void StoneZoomer_Main(void) {  // 0xA3E6C2
  Enemy_StoneZoomer *E = Get_StoneZoomer(cur_enemy_index);
  CallEnemyPreInstr(E->szr_var_F | 0xA30000);
}

void FireZoomer_Func_1(uint16 k) {  // 0xA3E6C8
  int16 fzr_var_A;
  int16 fzr_var_B;

  Enemy_FireZoomer *E = Get_FireZoomer(k);
  if (earthquake_timer == 30 && earthquake_type == 20) {
    E->fzr_var_03 = E->fzr_var_F;
    E->fzr_var_F = FUNC16(FireZoomer_Func_2);
  }
  R18_ = 0;
  R20_ = 0;

  fzr_var_A = E->fzr_var_A;
  if (fzr_var_A < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = fzr_var_A;
  uint16 v4;
  if ((R20_ & 0x8000u) == 0)
    v4 = R20_ + 1;
  else
    v4 = R20_ - 1;
  R20_ = v4;
  if (Enemy_MoveRight_IgnoreSlopes(k) & 1) {
    E->fzr_var_04 = 0;
    EnemyFunc_C8AD(k);
    R18_ = 0;
    R20_ = 0;
    fzr_var_B = E->fzr_var_B;
    if (fzr_var_B < 0)
      --R20_;
    *(uint16 *)((char *)&R18_ + 1) = fzr_var_B;
    if (Enemy_MoveDown(k) & 1) {
      E->fzr_var_A = -E->fzr_var_A;
      uint16 fzr_parameter_2 = E->fzr_parameter_2, v7;
      if ((E->fzr_var_B & 0x8000u) == 0)
        v7 = g_off_A3E63C[fzr_parameter_2 >> 1];
      else
        v7 = g_off_A3E630[fzr_parameter_2 >> 1];
      E->base.current_instruction = v7;
      E->base.instruction_timer = 1;
    }
  } else {
    uint16 v8 = E->fzr_var_04 + 1;
    E->fzr_var_04 = v8;
    if (sign16(v8 - 4)) {
      E->fzr_var_B = -E->fzr_var_B;
      uint16 v9 = E->fzr_parameter_2, v10;
      if ((E->fzr_var_B & 0x8000u) == 0)
        v10 = g_off_A3E63C[v9 >> 1];
      else
        v10 = g_off_A3E630[v9 >> 1];
      E->base.current_instruction = v10;
      E->base.instruction_timer = 1;
    } else {
      E->fzr_var_03 = E->fzr_var_F;
      E->fzr_var_F = FUNC16(FireZoomer_Func_2);
    }
  }
}

void FireZoomer_Func_2(uint16 k) {  // 0xA3E785
  Enemy_FireZoomer *E = Get_FireZoomer(k);
  R18_ = E->fzr_var_01;
  R20_ = E->fzr_var_02;
  if (Enemy_MoveDown(k) & 1) {
    if (E->fzr_parameter_1 == 255) {
      E->fzr_var_A = 128;
      E->fzr_var_B = 128;
    }
    E->fzr_var_01 = 0;
    E->fzr_var_02 = 0;
    E->fzr_var_04 = 0;
    E->fzr_var_F = E->fzr_var_03;
  } else {
    if (sign16(E->fzr_var_02 - 4)) {
      uint16 fzr_var_01 = E->fzr_var_01;
      E->fzr_var_01 = fzr_var_01 + 0x8000;
      E->fzr_var_02 += __CFADD__uint16(fzr_var_01, 0x8000);
    }
    if (!E->fzr_var_01 && !E->fzr_var_02)
      E->fzr_var_F = FUNC16(FireZoomer_Func_1);
  }
}

void FireZoomer_Func_3(uint16 k) {  // 0xA3E7F2
  int16 fzr_var_B;
  Enemy_FireZoomer *E = Get_FireZoomer(k);

  if (earthquake_timer == 30 && earthquake_type == 20) {
    E->fzr_var_03 = E->fzr_var_F;
    E->fzr_var_F = FUNC16(FireZoomer_Func_2);
  }
  R18_ = 0;
  R20_ = 0;

  fzr_var_B = E->fzr_var_B;
  if (fzr_var_B < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = fzr_var_B;
  uint16 v4;
  if ((R20_ & 0x8000u) == 0)
    v4 = R20_ + 1;
  else
    v4 = R20_ - 1;
  R20_ = v4;
  if (Enemy_MoveDown(k) & 1) {
    E->fzr_var_04 = 0;
    FireZoomer_E8A5(k);
    if (Enemy_MoveRight_IgnoreSlopes(k) & 1) {
      E->fzr_var_B = -E->fzr_var_B;
      uint16 fzr_parameter_2 = E->fzr_parameter_2, v6;
      if ((E->fzr_var_A & 0x8000u) == 0)
        v6 = g_off_A3E654[fzr_parameter_2 >> 1];
      else
        v6 = g_off_A3E648[fzr_parameter_2 >> 1];
      E->base.current_instruction = v6;
      E->base.instruction_timer = 1;
    } else {
      EnemyFunc_C8AD(k);
    }
  } else {
    uint16 v7 = E->fzr_var_04 + 1;
    E->fzr_var_04 = v7;
    if (sign16(v7 - 4)) {
      E->fzr_var_A = -E->fzr_var_A;
      uint16 v8 = E->fzr_parameter_2, v9;
      if ((E->fzr_var_A & 0x8000u) == 0)
        v9 = g_off_A3E654[v8 >> 1];
      else
        v9 = g_off_A3E648[v8 >> 1];
      E->base.current_instruction = v9;
      E->base.instruction_timer = 1;
    } else {
      E->fzr_var_03 = E->fzr_var_F;
      E->fzr_var_F = FUNC16(FireZoomer_Func_2);
    }
  }
}

void FireZoomer_E8A5(uint16 k) {  // 0xA3E8A5
  int16 fzr_var_A;
  int16 v3;
  Enemy_FireZoomer *E = Get_FireZoomer(cur_enemy_index);

  uint16 xpos = E->base.x_pos;
  uint16 ypos = sign16(E->fzr_var_B) ?
    E->base.y_pos - E->base.y_height :
    E->base.y_pos + E->base.y_height - 1;
  CalculateBlockContainingPixelPos(xpos, ypos);

  if ((level_data[cur_block_index] & 0xF000) == 4096
      && (BTS[cur_block_index] & 0x1Fu) >= 5) {
    uint16 v1 = g_word_A3E931[(uint16)(4 * (BTS[cur_block_index] & 0x1F)) >> 1];
    fzr_var_A = E->fzr_var_A;
    if (fzr_var_A >= 0) {
      Multiply16x16(fzr_var_A, v1);
      R18_ = mult_product_lo;
      R20_ = mult_product_hi;
    } else {
      Multiply16x16(-fzr_var_A, v1);
      Negate32(&mult_product_hi, &mult_product_lo, &R20_, &R18_);
    }
  } else {
    R18_ = 0;
    R20_ = 0;
    v3 = E->fzr_var_A;
    if (v3 < 0)
      --R20_;
    *(uint16 *)((char *)&R18_ + 1) = v3;
  }
}

void Metroid_Init(void) {  // 0xA3EA4F
  Enemy_Metroid *E = Get_Metroid(cur_enemy_index);
  E->base.current_instruction = addr_kMetroid_Ilist_E9CF;
  R24_ = E->base.vram_tiles_index | E->base.palette_index;
  R18_ = E->base.x_pos;
  R20_ = E->base.y_pos;
  R22_ = 50;
  CreateSpriteAtPos();
  E->metroid_var_00 = R18_;
  R24_ = E->base.vram_tiles_index | E->base.palette_index;
  R18_ = E->base.x_pos;
  R20_ = E->base.y_pos;
  R22_ = 52;
  CreateSpriteAtPos();
  E->metroid_var_01 = R18_;
  E->metroid_var_02 = 0;
}

const uint16 *Metroid_Instr_2(uint16 k, const uint16 *jp) {  // 0xA3EAA5
  QueueSfx2_Max6(0x50u);
  return jp;
}

const uint16 *Metroid_Instr_1(uint16 k, const uint16 *jp) {  // 0xA3EAB1
  int v2 = (uint16)(2 * (NextRandom() & 7)) >> 1;
  QueueSfx2_Max6(g_word_A3EAD6[v2]);
  return jp;
}

void Metroid_Frozen(void) {  // 0xA3EAE6
  Enemy_NormalFrozenAI_A3();
  Enemy_Metroid *E = Get_Metroid(cur_enemy_index);
  if (E->metroid_var_E) {
    --E->metroid_var_E;
    E->base.flash_timer = 2;
  }
  int v1 = E->metroid_var_00 >> 1;
  sprite_palettes[v1] = 3072;
  sprite_disable_flag[v1] = 1;
  sprite_instr_list_ptrs[v1] = addr_kSpriteObject_Ilist_C3BA;
  int v2 = E->metroid_var_01 >> 1;
  sprite_palettes[v2] = 3072;
  sprite_disable_flag[v2] = 1;
  sprite_instr_list_ptrs[v2] = addr_kSpriteObject_Ilist_C4B6;
}

void Metroid_Hurt(void) {  // 0xA3EB33
  Enemy_Metroid *E = Get_Metroid(cur_enemy_index);
  if ((E->base.flash_timer & 2) != 0) {
    R18_ = E->base.palette_index;
    int v1 = E->metroid_var_00 >> 1;
    sprite_palettes[v1] = R18_ | sprite_palettes[v1] & 0xF1FF;
    int v2 = E->metroid_var_01 >> 1;
    sprite_palettes[v2] = R18_ | sprite_palettes[v2] & 0xF1FF;
  } else {
    int v3 = E->metroid_var_00 >> 1;
    sprite_palettes[v3] &= 0xF1FFu;
    uint16 metroid_var_01 = E->metroid_var_01;
    sprite_palettes[metroid_var_01 >> 1] &= 0xF1FFu;
  }
}

static Func_V *const off_A3EC09[4] = { Metroid_Func_1, Metroid_Func_2, Metroid_Func_3, Metroid_Func_4 };
void Metroid_Main(void) {  // 0xA3EB98
  Enemy_Metroid *E = Get_Metroid(cur_enemy_index);
  draw_enemy_layer = samus_y_pos - 8;
  uint16 v0 = 2 * E->metroid_var_F;
  off_A3EC09[v0 >> 1]();
  R24_ = E->base.vram_tiles_index | E->base.palette_index;
  R18_ = E->base.x_pos;
  R20_ = E->base.y_pos;
  int v2 = E->metroid_var_00 >> 1;
  sprite_x_pos[v2] = R18_;
  sprite_y_pos[v2] = R20_;
  sprite_palettes[v2] = R24_;
  sprite_disable_flag[v2] = 0;
  R18_ = E->base.x_pos;
  R20_ = E->base.y_pos;
  int v4 = E->metroid_var_01 >> 1;
  sprite_x_pos[v4] = R18_;
  sprite_y_pos[v4] = R20_;
  sprite_palettes[v4] = R24_;
  sprite_disable_flag[v4] = 0;
}

void Metroid_Func_1(void) {  // 0xA3EC11
  int16 v3;
  int16 v4;
  int16 v8;
  int16 v9;

  R18_ = 0;
  R20_ = 0;
  Enemy_Metroid *E = Get_Metroid(cur_enemy_index);
  *(uint16 *)((char *)&R18_ + 1) = (uint16)(E->base.y_pos - draw_enemy_layer) >> 2;
  if ((*(uint16 *)((char *)&R18_ + 1) & 0x2000) != 0)
    R20_ = (uint8)R20_ | 0xFFC0;
  uint16 metroid_var_C = E->metroid_var_C;
  bool v2 = metroid_var_C < R18_;
  E->metroid_var_C = metroid_var_C - R18_;
  v3 = E->metroid_var_D - (v2 + R20_);
  E->metroid_var_D = v3;
  if (v3 < 0) {
    if ((uint16)v3 >= 0xFFFDu)
      goto LABEL_9;
    v4 = -3;
  } else {
    if ((uint16)v3 < 3u)
      goto LABEL_9;
    v4 = 3;
  }
  E->metroid_var_D = v4;
  E->metroid_var_C = 0;
LABEL_9:
  R18_ = E->metroid_var_C;
  R20_ = E->metroid_var_D;
  if (Enemy_MoveDown(cur_enemy_index) & 1) {
    E->metroid_var_C = 0;
    E->metroid_var_D = 0;
  }
  R18_ = 0;
  R20_ = 0;
  *(uint16 *)((char *)&R18_ + 1) = (uint16)(E->base.x_pos - samus_x_pos) >> 2;
  if ((*(uint16 *)((char *)&R18_ + 1) & 0x2000) != 0)
    R20_ = (uint8)R20_ | 0xFFC0;
  uint16 metroid_var_A = E->metroid_var_A;
  v2 = metroid_var_A < R18_;
  E->metroid_var_A = metroid_var_A - R18_;
  v8 = E->metroid_var_B - (v2 + R20_);
  E->metroid_var_B = v8;
  if (v8 < 0) {
    if ((uint16)v8 >= 0xFFFDu)
      goto LABEL_19;
    v9 = -3;
  } else {
    if ((uint16)v8 < 3u)
      goto LABEL_19;
    v9 = 3;
  }
  E->metroid_var_B = v9;
  E->metroid_var_A = 0;
LABEL_19:
  R18_ = E->metroid_var_A;
  R20_ = E->metroid_var_B;
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index) & 1) {
    E->metroid_var_A = 0;
    E->metroid_var_B = 0;
  }
}

void Metroid_Func_2(void) {  // 0xA3ECDC
  int16 v1;
  int16 v5;
  uint16 v2, v6;
  R18_ = 0;
  R20_ = 0;
  Enemy_Metroid *E = Get_Metroid(cur_enemy_index);
  LOBYTE(v1) = (uint16)(draw_enemy_layer - E->base.y_pos) >> 8;
  HIBYTE(v1) = draw_enemy_layer - LOBYTE(E->base.y_pos);
  R20_ = (uint16)(v1 & 0xFF00) >> 3;
  if ((R20_ & 0x1000) != 0)
    R20_ |= 0xE000u;
  if ((R20_ & 0x8000u) != 0) {
    if (R20_ >= 0xFFFDu)
      goto LABEL_9;
    v2 = -3;
  } else {
    if (R20_ < 3u)
      goto LABEL_9;
    v2 = 3;
  }
  R20_ = v2;
  R18_ = 0;
LABEL_9:
  E->metroid_var_C = R18_;
  E->metroid_var_D = R20_;
  if (Enemy_MoveDown(cur_enemy_index) & 1) {
    E->metroid_var_C = 0;
    E->metroid_var_D = 0;
  }
  R18_ = 0;
  R20_ = 0;
  LOBYTE(v5) = (uint16)(samus_x_pos - E->base.x_pos) >> 8;
  HIBYTE(v5) = samus_x_pos - LOBYTE(E->base.x_pos);
  R20_ = (uint16)(v5 & 0xFF00) >> 3;
  if ((R20_ & 0x1000) != 0)
    R20_ |= 0xE000u;
  if ((R20_ & 0x8000u) != 0) {
    if (R20_ >= 0xFFFDu)
      goto LABEL_19;
    v6 = -3;
  } else {
    if (R20_ < 3u)
      goto LABEL_19;
    v6 = 3;
  }
  R20_ = v6;
  R18_ = 0;
LABEL_19:
  E->metroid_var_A = R18_;
  E->metroid_var_B = R20_;
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index) & 1) {
    E->metroid_var_A = 0;
    E->metroid_var_B = 0;
  }
}

void Metroid_Func_3(void) {  // 0xA3ED8F
  Enemy_Metroid *E = Get_Metroid(cur_enemy_index);
  E->base.x_pos = samus_x_pos;
  E->base.y_pos = draw_enemy_layer;
  E->metroid_var_A = 0;
  E->metroid_var_B = 0;
  E->metroid_var_C = 0;
  E->metroid_var_D = 0;
}

void Metroid_Func_4(void) {  // 0xA3EDAB
  Enemy_Metroid *E = Get_Metroid(cur_enemy_index);
  int v1 = E->metroid_var_E & 3;
  E->base.x_pos += g_word_A3EA3F[v1];
  E->base.y_pos += g_word_A3EA3F[v1 + 4];
  E->metroid_var_A = 0;
  E->metroid_var_B = 0;
  E->metroid_var_C = 0;
  E->metroid_var_D = 0;
  if (E->metroid_var_E-- == 1) {
    E->metroid_var_F = 0;
    E->base.current_instruction = addr_kMetroid_Ilist_E9CF;
    E->base.instruction_timer = 1;
  }
}

void Metroid_Touch(void) {  // 0xA3EDEB
  int16 v1;
  int16 v2;
  int16 v3;
  int16 v4;
  int16 v7;

  Enemy_Metroid *E = Get_Metroid(cur_enemy_index);
  draw_enemy_layer = samus_y_pos - 8;
  if (samus_contact_damage_index) {
    if (E->metroid_var_F != 2) {
      E->metroid_var_A = 0;
      E->metroid_var_C = 0;
      v1 = 0;
      v2 = E->base.x_pos - samus_x_pos;
      if (v2 < 0)
        v1 = -256;
      E->metroid_var_B = v1;
      *(uint16 *)((char *)&E->metroid_var_A + 1) = v2 << 6;
      v3 = 0;
      v4 = E->base.y_pos - draw_enemy_layer;
      if (v4 < 0)
        v3 = -256;
      E->metroid_var_D = v3;
      *(uint16 *)((char *)&E->metroid_var_C + 1) = v4 << 6;
      E->metroid_var_F = 0;
      E->base.current_instruction = addr_kMetroid_Ilist_E9CF;
      E->base.instruction_timer = 1;
    }
  } else {
    uint16 v5 = cur_enemy_index;
    if (E->metroid_var_F != 3) {
      if ((enemy_damage_routine_exec_count & 7) == 7 && !sign16(samus_health - 30))
        QueueSfx3_Max6(0x2Du);
      Metroid_Func_5(v5);
    }
    if (E->metroid_var_F < 2u) {
      v7 = 1;
      if (abs16(E->base.x_pos - samus_x_pos) < 8u && abs16(E->base.y_pos - draw_enemy_layer) < 8u) {
        v7 = 2;
        CallSomeSamusCode(0x12u);
      }
      E->metroid_var_F = v7;
      if (v7 == 2) {
        E->base.current_instruction = addr_kMetroid_Ilist_EA25;
        E->base.instruction_timer = 1;
      }
    }
  }
}

void Metroid_Func_5(uint16 k) {  // 0xA3EECE
  uint16 v1;
  draw_enemy_layer = samus_y_pos - 8;
  if ((equipped_items & 0x20) != 0) {
    v1 = 12288;
  } else if (equipped_items & 1) {
    v1 = 24576;
  } else {
    v1 = -16384;
  }
  R18_ = v1;
  Enemy_Metroid *E = Get_Metroid(k);
  uint16 metroid_var_02 = E->metroid_var_02;
  bool v4 = metroid_var_02 < R18_;
  E->metroid_var_02 = metroid_var_02 - R18_;
  if (v4)
    Samus_DealDamage(1u);
}

void Metroid_Shot(void) {  // 0xA3EF07
  int16 v3;
  int16 v6;
  int16 v7;
  int16 v8;
  int16 v9;

  draw_enemy_layer = samus_y_pos - 8;
  uint16 v0 = 2 * collision_detection_index;
  Enemy_Metroid *E = Get_Metroid(cur_enemy_index);
  if (E->base.frozen_timer) {
    v3 = projectile_type[v0 >> 1] & 0xF00;
    if (v3 == 256 || v3 == 512) {
      special_death_item_drop_x_origin_pos = E->base.x_pos;
      special_death_item_drop_y_origin_pos = E->base.y_pos;
      Enemy_NormalShotAI_SkipSomeParts_A3();
      if (!E->base.health) {
        E->metroid_var_B = 0;
        EnemyDeathAnimation(cur_enemy_index, 4u);
        CallSomeSamusCode(0x13u);
        sprite_instr_list_ptrs[E->metroid_var_00 >> 1] = 0;
        uint16 metroid_var_01 = E->metroid_var_01;
        sprite_instr_list_ptrs[metroid_var_01 >> 1] = 0;
        Enemy_ItemDrop_Metroid(metroid_var_01);
      }
    }
  } else {
    if (E->metroid_var_F == 2) {
      if ((projectile_type[v0 >> 1] & 0xF00) == 1280) {
        E->metroid_var_E = 4;
        E->metroid_var_F = 3;
        E->base.current_instruction = addr_kMetroid_Ilist_E9CF;
        E->base.instruction_timer = 1;
        CallSomeSamusCode(0x13u);
      }
    } else {
      E->metroid_var_A = 0;
      E->metroid_var_C = 0;
      v6 = 0;
      v7 = E->base.x_pos - projectile_x_pos[0];
      if (v7 < 0)
        v6 = -256;
      E->metroid_var_B = v6;
      *(uint16 *)((char *)&E->metroid_var_A + 1) = 32 * v7;
      v8 = 0;
      v9 = E->base.y_pos - projectile_y_pos[0];
      if (v9 < 0)
        v8 = -256;
      E->metroid_var_D = v8;
      *(uint16 *)((char *)&E->metroid_var_C + 1) = 32 * v9;
      E->metroid_var_F = 0;
      E->base.current_instruction = addr_kMetroid_Ilist_E9CF;
      E->base.instruction_timer = 1;
      int v10 = collision_detection_index;
      if ((projectile_type[v10] & 2) != 0) {
        QueueSfx3_Max6(0xAu);
        R18_ = projectile_damage[v10];
        E->base.flash_timer = 4;
        uint16 metroid_parameter_2 = E->metroid_parameter_2;
        bool v13 = metroid_parameter_2 < R18_;
        uint16 v12 = metroid_parameter_2 - R18_;
        v13 = !v13;
        if (!v12 || !v13) {
          E->metroid_parameter_2 = 0;
          E->base.frozen_timer = 400;
          E->base.ai_handler_bits |= 4u;
          return;
        }
        E->metroid_parameter_2 = v12;
      }
      QueueSfx2_Max6(0x5Au);
    }
  }
}

void Metroid_Powerbomb(uint16 k) {  // 0xA3F042
  NormalEnemyPowerBombAi();
  if (!Get_Metroid(k)->base.health) {
    CallSomeSamusCode(0x13u);
    sprite_instr_list_ptrs[Get_Metroid(cur_enemy_index)->metroid_var_00 >> 1] = 0;
    sprite_instr_list_ptrs[Get_Metroid(cur_enemy_index)->metroid_var_01 >> 1] = 0;
  }
}