// Enemy AI - inc. Ridley & zebetites
#include "ida_types.h"
#include "variables.h"
#include "sm_rtl.h"
#include "funcs.h"
#include "enemy_types.h"


#define g_byte_A6E269 ((uint8*)RomFixedPtr(0xa6e269))
#define g_word_A6E2AA ((uint16*)RomFixedPtr(0xa6e2aa))
#define g_word_A6E30A ((uint16*)RomFixedPtr(0xa6e30a))
#define g_word_A6E46A ((uint16*)RomFixedPtr(0xa6e46a))
#define g_off_A6A4EB ((uint16*)RomFixedPtr(0xa6a4eb))
#define g_off_A6A743 ((uint16*)RomFixedPtr(0xa6a743))
#define g_word_A6AE4D ((uint16*)RomFixedPtr(0xa6ae4d))
#define g_word_A6AF2F ((uint16*)RomFixedPtr(0xa6af2f))
#define g_word_A6B00F ((uint16*)RomFixedPtr(0xa6b00f))
#define g_off_A6ACDA ((uint16*)RomFixedPtr(0xa6acda))
#define g_off_A6AD45 ((uint16*)RomFixedPtr(0xa6ad45))
#define g_word_A6B60D ((uint16*)RomFixedPtr(0xa6b60d))
#define g_word_A6B63B ((uint16*)RomFixedPtr(0xa6b63b))
#define g_word_A6B6C8 ((uint16*)RomFixedPtr(0xa6b6c8))
#define g_word_A6B9D5 ((uint16*)RomFixedPtr(0xa6b9d5))
#define g_word_A6B9DB ((uint16*)RomFixedPtr(0xa6b9db))
#define g_off_A6B965 ((uint16*)RomFixedPtr(0xa6b965))
#define g_off_A6B96D ((uint16*)RomFixedPtr(0xa6b96d))
#define g_word_A6B9D5 ((uint16*)RomFixedPtr(0xa6b9d5))
#define g_word_A6B9DB ((uint16*)RomFixedPtr(0xa6b9db))
#define g_word_A6B94D ((uint16*)RomFixedPtr(0xa6b94d))
#define g_word_A6B959 ((uint16*)RomFixedPtr(0xa6b959))
#define g_word_A6BB48 ((uint16*)RomFixedPtr(0xa6bb48))
#define g_word_A6BB4E ((uint16*)RomFixedPtr(0xa6bb4e))
#define g_word_A6C1DF ((uint16*)RomFixedPtr(0xa6c1df))
#define g_off_A6C7BA ((uint16*)RomFixedPtr(0xa6c7ba))
#define g_word_A6C804 ((uint16*)RomFixedPtr(0xa6c804))
#define g_off_A6C808 ((uint16*)RomFixedPtr(0xa6c808))
#define g_word_A6C836 ((uint16*)RomFixedPtr(0xa6c836))
#define g_off_A6C83A ((uint16*)RomFixedPtr(0xa6c83a))
#define g_word_A6C868 ((uint16*)RomFixedPtr(0xa6c868))
#define g_off_A6C86C ((uint16*)RomFixedPtr(0xa6c86c))
#define g_word_A6C89A ((uint16*)RomFixedPtr(0xa6c89a))
#define g_off_A6C89E ((uint16*)RomFixedPtr(0xa6c89e))
#define g_word_A6C8CC ((uint16*)RomFixedPtr(0xa6c8cc))
#define g_off_A6C8D0 ((uint16*)RomFixedPtr(0xa6c8d0))
#define g_word_A6C6CE ((uint16*)RomFixedPtr(0xa6c6ce))
#define g_off_A6C6E6 ((uint16*)RomFixedPtr(0xa6c6e6))
#define g_word_A6CC12 ((uint16*)RomFixedPtr(0xa6cc12))
#define g_word_A6CC18 ((uint16*)RomFixedPtr(0xa6cc18))
#define g_off_A6DB02 ((uint16*)RomFixedPtr(0xa6db02))
#define kRidley_Ilist_DCBA ((uint16*)RomFixedPtr(0xa6dcba))
#define g_off_A6EFF5 ((uint16*)RomFixedPtr(0xa6eff5))
#define g_off_A6F001 ((uint16*)RomFixedPtr(0xa6f001))
#define g_off_A6F72B ((uint16*)RomFixedPtr(0xa6f72b))
#define g_off_A6F52C ((uint16*)RomFixedPtr(0xa6f52c))
#define g_word_A6F840 ((uint16*)RomFixedPtr(0xa6f840))
#define g_off_A6F900 ((uint16*)RomFixedPtr(0xa6f900))
#define g_word_A6FC03 ((uint16*)RomFixedPtr(0xa6fc03))
#define g_word_A6FC0B ((uint16*)RomFixedPtr(0xa6fc0b))
#define g_off_A6FC13 ((uint16*)RomFixedPtr(0xa6fc13))
#define g_word_A6FC1B ((uint16*)RomFixedPtr(0xa6fc1b))
#define g_word_A6FC23 ((uint16*)RomFixedPtr(0xa6fc23))
#define g_word_A6FC2B ((uint16*)RomFixedPtr(0xa6fc2b))
#define g_off_A6FD4A ((uint16*)RomFixedPtr(0xa6fd4a))
#define g_off_A6FD54 ((uint16*)RomFixedPtr(0xa6fd54))



int BabyMetroid_DBCB_DoubleRetEx(uint16 a);
uint8 Ridley_Func_40_Carry();
uint8 Ridley_Func_40_Sign();

static const uint16 g_word_A686F1[2] = { 0x1000, 0x1800 };
static const uint16 g_word_A68DBB = 5;
static const uint16 g_word_A68DBD = 0xa;
static const uint16 g_word_A68DBF = 0xf;
static const uint16 g_word_A68DC1 = 0x14;
static const uint16 g_word_A68DC3 = 0x19;
static const uint16 g_word_A68DC5 = 0x1e;
static const uint16 g_word_A68DC7 = 0x23;
static const uint16 g_word_A68DC9 = 0x28;
static const uint16 g_word_A68DCB = 0x2d;
static const uint16 g_word_A68DCD = 0x32;
static const uint16 g_word_A68DCF = 0x37;
static const uint16 g_word_A68DD1 = 0x3c;
static const uint16 g_word_A68DD3 = 0x41;
static const uint16 g_word_A68DD5 = 0x46;
static const uint16 g_word_A68DD7 = 0x4b;
static const uint16 g_word_A68DD9 = 0x50;
static const uint16 g_word_A68DDB = 0x55;
static const uint16 g_word_A68DDD = 0x5a;
static const uint16 g_word_A68DDF = 0x5f;
static const uint16 g_word_A68DE1 = 0x64;
static const uint16 g_word_A68DE3 = 0x69;
static const uint16 g_word_A68DE5 = 0x6e;
static const uint16 g_word_A68DE7 = 0x18;
static const uint16 g_word_A68DE9 = 0x18;
static const uint16 g_word_A68DEB = 0x18;
static const uint16 g_word_A68DED = 0x18;
static const uint16 g_word_A68DEF = 0x18;
static const uint16 g_word_A68DF1 = 0x18;
static const uint16 g_word_A68DF3 = 0x18;
static const uint16 g_word_A68DF5 = 0x18;
static const uint16 g_word_A68DF7 = 0x18;
static const uint16 g_word_A68DF9 = 0x18;
static const uint16 g_word_A68DFB = 0x18;
static const uint16 g_word_A68DFD = 0x18;
static const uint16 g_word_A68DFF = 0x18;
static const uint16 g_word_A68E01 = 0x18;
static const uint16 g_word_A68E03 = 0x18;
static const uint16 g_word_A68E05 = 0x18;
static const uint16 g_word_A68E07 = 0x18;
static const uint16 g_word_A68E09 = 0x18;
static const uint16 g_word_A68E0B = 0x14;
static const uint16 g_word_A68E0D = 0x10;
static const uint16 g_word_A68E0F = 0xc;
static const uint16 g_word_A68E11 = 8;
static const uint16 g_word_A695F6[4] = { 0x190, 0xf0, 0xf0, 0x190 };
static const uint16 g_word_A695FE[4] = { 0xffe8, 0xfff4, 0x18, 0xc };
static const uint16 g_word_A69606[4] = { 0x180, 0x100, 0x100, 0x180 };
static const uint16 g_word_A69A48[8] = { 0xfe00, 0xfb00, 0xfc00, 0xfb00, 0x200, 0xfb00, 0x400, 0xfb00 };

static const uint16 g_word_A6B288 = 8;
static const uint16 g_word_A6BBEB[3] = { 0x40, 0, 0xd0 };
static const uint16 g_word_A6BC62[3] = { 0xb0, 0, 0x50 };
static const int16 g_word_A6C66E[20] = { -24, -24, -20, 20, 16, -30, 30, -3, 14, -13, -2, 18, -2, -32, -31, 8, -4, -10, 19, 19 };
static const int16 g_word_A6CF54[3] = { 0x20, 0, -0x20 };
static const uint8 g_byte_A6D61F[16] = { 0x10, 0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
static const uint8 g_byte_A6D712[16] = { 0x10, 0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
static const uint16 g_word_A6D9ED[8] = { 0xc, 0xe, 0x10, 0x12, 0x1c, 0x20, 0x28, 0x30 };

void Enemy_GrappleReact_NoInteract_A6(void) {  // 0xA68000
  SwitchEnemyAiToMainAi();
}

void Enemy_GrappleReact_CancelBeam_A6(void) {  // 0xA6800F
  Enemy_SwitchToFrozenAi();
}

void Enemy_NormalTouchAI_A6(void) {  // 0xA68023
  NormalEnemyTouchAi();
}

void Enemy_NormalShotAI_A6(void) {  // 0xA6802D
  NormalEnemyShotAi();
}

void Enemy_NormalFrozenAI_A6(void) {  // 0xA68041
  NormalEnemyFrozenAI();
}

void Boulder_Init(void) {  // 0xA686F5
  int16 boulder_parameter_2_high;

  Enemy_Boulder *E = Get_Boulder(cur_enemy_index);
  E->boulder_var_C = 0;
  E->boulder_var_B = 0;
  E->boulder_var_D = 2;
  E->boulder_var_A = FUNC16(Boulder_Func_1);
  E->boulder_var_04 = E->base.x_pos;
  uint16 y_pos = E->base.y_pos;
  E->boulder_var_05 = y_pos;
  E->boulder_var_03 = y_pos;
  E->boulder_var_02 = E->base.y_subpos;
  boulder_parameter_2_high = HIBYTE(E->boulder_parameter_2);
  if (!HIBYTE(E->boulder_parameter_2)) {
    boulder_parameter_2_high = 1;
    E->boulder_var_07 = 1;
  }
  E->base.y_pos = E->boulder_var_05 - boulder_parameter_2_high;
  E->boulder_var_00 = -LOBYTE(E->boulder_parameter_2);
  E->boulder_var_06 = LOBYTE(E->base.current_instruction);
  E->base.current_instruction = addr_kBoulder_Ilist_86A7;
  uint16 boulder_parameter_1_high = HIBYTE(E->boulder_parameter_1);
  E->boulder_var_E = boulder_parameter_1_high;
  if (!boulder_parameter_1_high) {
    E->boulder_var_00 = -E->boulder_var_00;
    E->base.current_instruction = addr_kBoulder_Ilist_86CB;
  }
  E->boulder_var_01 = 2;
  if (LOBYTE(E->boulder_parameter_1))
    E->boulder_var_01 = 5;
}

void CallBoulderFunc(uint32 ea) {
  switch (ea) {
  case fnBoulder_Func_1: Boulder_Func_1(); return;
  case fnBoulder_Func_2: Boulder_Func_2(); return;
  case fnBoulder_Func_3: Boulder_Func_3(); return;
  case fnBoulder_Func_4: Boulder_Func_4(); return;
  case fnBoulder_Func_5: Boulder_Func_5(); return;
  case fnBoulder_Func_6: Boulder_Func_6(); return;
  default: Unreachable();
  }
}

void Boulder_Main(void) {  // 0xA68793
  Enemy_Boulder *E = Get_Boulder(cur_enemy_index);
  CallBoulderFunc(E->boulder_var_A | 0xA60000);
}

void Boulder_Func_1(void) {  // 0xA6879A
  int16 SamusEnemyDelta_Y;
  int16 v3;
  int16 SamusEnemyDelta_X;

  SamusEnemyDelta_Y = GetSamusEnemyDelta_Y(cur_enemy_index);
  if (SamusEnemyDelta_Y >= 0) {
    Enemy_Boulder *E = Get_Boulder(cur_enemy_index);
    if ((int16)(SamusEnemyDelta_Y - E->boulder_var_06) < 0) {
      if (E->boulder_var_E) {
        SamusEnemyDelta_X = GetSamusEnemyDelta_X(cur_enemy_index);
        if (SamusEnemyDelta_X < 0 && (int16)(SamusEnemyDelta_X - E->boulder_var_00) >= 0) {
          E->boulder_var_A = FUNC16(Boulder_Func_2);
          if (E->boulder_var_07)
            E->boulder_var_A = FUNC16(Boulder_Func_5);
        }
      } else {
        v3 = GetSamusEnemyDelta_X(cur_enemy_index);
        if (v3 >= 0 && (int16)(v3 - E->boulder_var_00) < 0) {
          E->boulder_var_A = FUNC16(Boulder_Func_2);
          if (E->boulder_var_07)
            E->boulder_var_A = FUNC16(Boulder_Func_5);
        }
      }
    }
  }
}

void Boulder_Func_2(void) {  // 0xA687ED
  Enemy_Boulder *E = Get_Boulder(cur_enemy_index);
  Boulder_Func_9(8 * HIBYTE(E->boulder_var_C));
  if ((int16)(E->base.y_pos - E->boulder_var_05) < 0) {
    uint16 v1 = E->boulder_var_C + 256;
    E->boulder_var_C = v1;
    if (!sign16(v1 - 20480))
      E->boulder_var_C = 20480;
  } else {
    E->base.y_pos = E->boulder_var_05;
    E->boulder_var_A = FUNC16(Boulder_Func_3);
    E->boulder_var_C = 0x2000;
  }
}

void Boulder_Func_3(void) {  // 0xA68832
  int16 v1;

  Enemy_Boulder *E = Get_Boulder(cur_enemy_index);
  Boulder_Func_9(8 * HIBYTE(E->boulder_var_C) + 4);
  v1 = E->boulder_var_C - 256;
  E->boulder_var_C = v1;
  if (v1 >= 0) {
    uint16 v2 = 8 * HIBYTE(E->boulder_var_B);
    if (E->boulder_var_E)
      v2 += 4;
    Boulder_Func_7(v2);
    uint16 v3 = E->boulder_var_B + 32;
    E->boulder_var_B = v3;
    if (!sign16(v3 - 20480))
      E->boulder_var_B = 20480;
  } else {
    E->boulder_var_C = 0;
    E->boulder_var_A = FUNC16(Boulder_Func_4);
  }
}

void Boulder_Func_4(void) {  // 0xA6888B
  Enemy_Boulder *E = Get_Boulder(cur_enemy_index);
  int v1 = (uint16)(8 * HIBYTE(E->boulder_var_C)) >> 1;
  R18_ = kCommonEnemySpeeds_Quadratic[v1];
  R20_ = kCommonEnemySpeeds_Quadratic[v1 + 1];
  if (Enemy_MoveDown(cur_enemy_index) & 1) {
    QueueSfx2_Max6(0x42u);
    if (E->boulder_var_E == 2) {
      E->base.properties |= kEnemyProps_Deleted;
      R18_ = E->base.x_pos;
      R20_ = E->base.y_pos;
      SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 0x11);
      QueueSfx2_Max6(0x43u);
    } else {
      E->boulder_var_A = FUNC16(Boulder_Func_3);
      E->boulder_var_C = g_word_A686F1[E->boulder_var_D - 1];
      if ((--E->boulder_var_D & 0x8000u) != 0) {
        E->boulder_var_03 = E->base.y_pos;
        E->boulder_var_02 = E->base.y_subpos;
        E->boulder_var_A = FUNC16(Boulder_Func_5);
      }
    }
  } else {
    E->boulder_var_C += 256;
    uint16 v3 = 8 * HIBYTE(E->boulder_var_B);
    if (E->boulder_var_E)
      v3 += 4;
    Boulder_Func_7(v3);
    uint16 v4 = E->boulder_var_B + 32;
    E->boulder_var_B = v4;
    if (!sign16(v4 - 20480))
      E->boulder_var_B = 20480;
  }
}

void Boulder_Func_5(void) {  // 0xA68942
  Enemy_Boulder *E = Get_Boulder(cur_enemy_index);
  uint16 v2 = 8 * HIBYTE(E->boulder_var_B);
  int v3 = v2 >> 1;
  R18_ = kCommonEnemySpeeds_Quadratic[v3];
  R20_ = E->boulder_var_01 + kCommonEnemySpeeds_Quadratic[v3 + 1];
  Enemy_MoveDown(cur_enemy_index);
  E->base.y_pos -= E->boulder_var_01;
  if (E->boulder_var_E)
    v2 += 4;
  int v4 = v2 >> 1;
  R18_ = kCommonEnemySpeeds_Quadratic[v4];
  R20_ = kCommonEnemySpeeds_Quadratic[v4 + 1];
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index) & 1) {
    E->base.properties |= 0x300u;
    E->boulder_var_A = FUNC16(Boulder_Func_6);
    QueueSfx2_Max6(0x42u);
    R18_ = E->base.x_pos;
    R20_ = E->base.y_pos;
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 0x11);
    QueueSfx2_Max6(0x43u);
  } else {
    uint16 v5 = E->boulder_var_B + 64;
    E->boulder_var_B = v5;
    if (!sign16(v5 - 0x4000))
      E->boulder_var_B = 0x4000;
    if (E->base.y_pos == E->boulder_var_03 && E->base.y_subpos == E->boulder_var_02)
      E->boulder_var_01 = 0;
  }
  E->boulder_var_03 = E->base.y_pos;
  E->boulder_var_02 = E->base.y_subpos;
}

void Boulder_Func_6(void) {  // 0xA689FC
  ;
}

void Boulder_Func_7(uint16 j) {  // 0xA68A00
  Enemy_Boulder *E = Get_Boulder(cur_enemy_index);
  uint16 x_subpos = E->base.x_subpos;
  int v3 = j >> 1;
  bool v4 = __CFADD__uint16(kCommonEnemySpeeds_Quadratic[v3], x_subpos);
  uint16 v5 = kCommonEnemySpeeds_Quadratic[v3] + x_subpos;
  if (v4)
    ++E->base.x_pos;
  E->base.x_subpos = v5;
  E->base.x_pos += kCommonEnemySpeeds_Quadratic[v3 + 1];
}

void Boulder_Func_8(uint16 j) {  // 0xA68A1D
  Enemy_Boulder *E = Get_Boulder(cur_enemy_index);
  uint16 x_subpos = E->base.x_subpos;
  int v3 = j >> 1;
  bool v4 = __CFADD__uint16(kCommonEnemySpeeds_Linear[v3 + 1], x_subpos);
  uint16 v5 = kCommonEnemySpeeds_Linear[v3 + 1] + x_subpos;
  if (v4)
    ++E->base.x_pos;
  E->base.x_subpos = v5;
  E->base.x_pos += kCommonEnemySpeeds_Linear[v3];
}

void Boulder_Func_9(uint16 j) {  // 0xA68A3A
  Enemy_Boulder *E = Get_Boulder(cur_enemy_index);
  uint16 y_subpos = E->base.y_subpos;
  int v3 = j >> 1;
  bool v4 = __CFADD__uint16(kCommonEnemySpeeds_Quadratic[v3], y_subpos);
  uint16 v5 = kCommonEnemySpeeds_Quadratic[v3] + y_subpos;
  if (v4)
    ++E->base.y_pos;
  E->base.y_subpos = v5;
  E->base.y_pos += kCommonEnemySpeeds_Quadratic[v3 + 1];
}


void SpikeyPlatform_Init(void) {  // 0xA68B2F
  Enemy_SpikeyPlatform *E = Get_SpikeyPlatform(cur_enemy_index);
  E->base.current_instruction = addr_kSpikeyPlatform_Ilist_8B29;
  E->spm_var_A = FUNC16(SpikeyPlatform_Func_1);
  uint16 v1 = 8 * LOBYTE(E->spm_parameter_1);
  E->spm_var_02 = v1;
  int v2 = v1 >> 1;
  R18_ = kCommonEnemySpeeds_Linear[v2];
  R20_ = kCommonEnemySpeeds_Linear[v2 + 1];
  E->spm_var_E = R20_;
  E->spm_var_F = R18_;
  E->spm_var_D = E->base.y_pos + HIBYTE(E->spm_parameter_2);
  E->spm_var_C = E->base.y_pos;
  uint16 spm_parameter_2_low = LOBYTE(E->spm_parameter_2);
  E->spm_var_00 = spm_parameter_2_low;
  E->spm_var_B = spm_parameter_2_low;
}

void SpikeyPlatform2ndEnemy_Init(void) {  // 0xA68B85
  int v0 = cur_enemy_index >> 1;
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  v1->x_pos = enemy_drawing_queue[v0 + 91];
  v1->y_pos = enemy_drawing_queue[v0 + 93] + 12;
}

void SpikeyPlatform2ndEnemy_Main(void) {  // 0xA68B99
  int v0 = cur_enemy_index >> 1;
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  v1->x_pos = enemy_drawing_queue[v0 + 91];
  v1->y_pos = enemy_drawing_queue[v0 + 93] + 12;
}

void CallSpikeyPlatformFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnSpikeyPlatform_Func_1: SpikeyPlatform_Func_1(); return;
  case fnSpikeyPlatform_Func_2: SpikeyPlatform_Func_2(); return;
  case fnSpikeyPlatform_Func_3: SpikeyPlatform_Func_3(k); return;
  case fnSpikeyPlatform_Func_4: SpikeyPlatform_Func_4(); return;
  default: Unreachable();
  }
}

void SpikeyPlatform_Main(void) {  // 0xA68BAD
  Enemy_SpikeyPlatform *E = Get_SpikeyPlatform(cur_enemy_index);
  CallSpikeyPlatformFunc(E->spm_var_A | 0xA60000, cur_enemy_index);
}

void SpikeyPlatform_Func_1(void) {  // 0xA68BB4
  Enemy_SpikeyPlatform *E = Get_SpikeyPlatform(cur_enemy_index);
  if (!--E->spm_var_B) {
    E->spm_var_B = E->spm_var_00;
    E->spm_var_02 = 8 * LOBYTE(E->spm_parameter_1);
    E->spm_var_A = FUNC16(SpikeyPlatform_Func_2);
  }
}

void SpikeyPlatform_Func_2(void) {  // 0xA68BDC
  int16 v4;

  Enemy_SpikeyPlatform *E = Get_SpikeyPlatform(cur_enemy_index);
  E->spm_var_01 = E->base.y_pos;
  int v1 = E->spm_var_02 >> 1;
  R20_ = kCommonEnemySpeeds_Linear[v1];
  R18_ = kCommonEnemySpeeds_Linear[v1 + 1];
  Enemy_AddPos_Y(cur_enemy_index);
  if ((int16)(E->base.y_pos - E->spm_var_D) >= 0) {
    E->spm_var_03 = 64;
    E->spm_var_A = FUNC16(SpikeyPlatform_Func_3);
    E->base.y_pos = E->spm_var_D;
    QueueSfx2_Max6(0x1Bu);
  }
  if (SpikeyPlatform_Func_5(cur_enemy_index))
    extra_samus_y_displacement += E->base.y_pos - E->spm_var_01;
  v4 = E->spm_var_02 + 8;
  if (!sign16(E->spm_var_02 - 504))
    v4 = 512;
  E->spm_var_02 = v4;
}

void SpikeyPlatform_Func_3(uint16 k) {  // 0xA68C4A
  Enemy_SpikeyPlatform *E = Get_SpikeyPlatform(k);
  uint16 v2 = E->spm_var_03 - 1;
  E->spm_var_03 = v2;
  if (!v2)
    E->spm_var_A = FUNC16(SpikeyPlatform_Func_4);
}

void SpikeyPlatform_Func_4(void) {  // 0xA68C5D
  Enemy_SpikeyPlatform *E = Get_SpikeyPlatform(cur_enemy_index);
  E->spm_var_01 = E->base.y_pos;
  R18_ = FUNC16(Enemy_GrappleReact_NoInteract_A6);
  R20_ = 0;
  Enemy_SubPos_Y(cur_enemy_index);
  if ((int16)(E->base.y_pos - E->spm_var_C) < 0) {
    E->spm_var_A = FUNC16(SpikeyPlatform_Func_1);
    E->base.y_pos = E->spm_var_C;
  }
  if (SpikeyPlatform_Func_5(cur_enemy_index))
    extra_samus_y_displacement += E->base.y_pos - E->spm_var_01;
}

uint16 SpikeyPlatform_Func_5(uint16 k) {  // 0xA68CA1
  Enemy_SpikeyPlatform *E = Get_SpikeyPlatform(k);
  uint16 v2 = abs16(samus_x_pos - E->base.x_pos);
  bool v3 = v2 < samus_x_radius;
  uint16 v4 = v2 - samus_x_radius;
  if (!v3 && v4 >= E->base.x_width)
    return 0;
  if ((int16)(samus_y_pos + 5 - E->base.y_pos) < 0) {
    uint16 v6 = E->base.y_pos - (samus_y_pos + 5);
    v3 = v6 < samus_y_radius;
    uint16 v7 = v6 - samus_y_radius;
    if (v3 || v7 == E->base.y_height || v7 < E->base.y_height)
      return -1;
  }
  return 0;
}

const uint16 *FireGeyser_Instr_1(uint16 k, const uint16 *jp) {  // 0xA68DAF
  QueueSfx2_Max6(0x61u);
  return jp;
}

const uint16 *FireGeyser_Instr_2(uint16 k, const uint16 *jp) {  // 0xA68E13
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DBB;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68DE7;
  E->base.x_width = 8;
  return jp;
}

const uint16 *FireGeyser_Instr_3(uint16 k, const uint16 *jp) {  // 0xA68E2D
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DBD;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68DE9;
  return jp;
}

const uint16 *FireGeyser_Instr_4(uint16 k, const uint16 *jp) {  // 0xA68E41
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DBF;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68DEB;
  return jp;
}

const uint16 *FireGeyser_Instr_5(uint16 k, const uint16 *jp) {  // 0xA68E55
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DC1;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68DED;
  return jp;
}

const uint16 *FireGeyser_Instr_6(uint16 k, const uint16 *jp) {  // 0xA68E69
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DC3;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68DEF;
  return jp;
}

const uint16 *FireGeyser_Instr_7(uint16 k, const uint16 *jp) {  // 0xA68E7D
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DC5;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68DF1;
  return jp;
}

const uint16 *FireGeyser_Instr_8(uint16 k, const uint16 *jp) {  // 0xA68E91
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DC7;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68DF3;
  return jp;
}

const uint16 *FireGeyser_Instr_9(uint16 k, const uint16 *jp) {  // 0xA68EA5
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DC9;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68DF5;
  return jp;
}

const uint16 *FireGeyser_Instr_10(uint16 k, const uint16 *jp) {  // 0xA68EB9
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DCB;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68DF7;
  return jp;
}

const uint16 *FireGeyser_Instr_11(uint16 k, const uint16 *jp) {  // 0xA68ECD
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DCD;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68DF9;
  return jp;
}

const uint16 *FireGeyser_Instr_12(uint16 k, const uint16 *jp) {  // 0xA68EE1
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DCF;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68DFB;
  return jp;
}

const uint16 *FireGeyser_Instr_13(uint16 k, const uint16 *jp) {  // 0xA68EF5
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DD1;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68DFD;
  return jp;
}

const uint16 *FireGeyser_Instr_14(uint16 k, const uint16 *jp) {  // 0xA68F09
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DD3;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68DFF;
  return jp;
}

const uint16 *FireGeyser_Instr_15(uint16 k, const uint16 *jp) {  // 0xA68F1D
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DD5;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68E01;
  return jp;
}

const uint16 *FireGeyser_Instr_16(uint16 k, const uint16 *jp) {  // 0xA68F31
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DD7;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68E03;
  return jp;
}

const uint16 *FireGeyser_Instr_17(uint16 k, const uint16 *jp) {  // 0xA68F45
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DD9;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68E05;
  return jp;
}

const uint16 *FireGeyser_Instr_18(uint16 k, const uint16 *jp) {  // 0xA68F59
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DDB;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68E07;
  return jp;
}

const uint16 *FireGeyser_Instr_19(uint16 k, const uint16 *jp) {  // 0xA68F6D
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DDD;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68E09;
  return jp;
}

const uint16 *FireGeyser_Instr_20(uint16 k, const uint16 *jp) {  // 0xA68F81
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DDF;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68E0B;
  return jp;
}

const uint16 *FireGeyser_Instr_21(uint16 k, const uint16 *jp) {  // 0xA68F95
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DE1;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68E0D;
  return jp;
}

const uint16 *FireGeyser_Instr_22(uint16 k, const uint16 *jp) {  // 0xA68FA9
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DE3;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68E0F;
  return jp;
}

const uint16 *FireGeyser_Instr_23(uint16 k, const uint16 *jp) {  // 0xA68FBD
  uint16 v2 = Get_FireGeyser(cur_enemy_index)->fgr_var_D - g_word_A68DE5;
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index + 64);
  E->base.y_pos = v2;
  E->base.y_height = g_word_A68E11;
  return jp;
}

const uint16 *FireGeyser_Instr_24(uint16 k, const uint16 *jp) {  // 0xA68FD1
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index);
  E->fgr_var_C = 1;
  Enemy_FireGeyser *E1 = Get_FireGeyser(cur_enemy_index + 64);
  E1->base.x_width = 0;
  E1->base.y_height = 0;
  E->base.y_pos = E->fgr_var_D;
  E->base.properties |= kEnemyProps_Invisible;
  E1->base.properties |= 0x400u;
  return jp;
}

void FireGeyser_Init(void) {  // 0xA68FFC
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index);
  E->base.current_instruction = addr_kFireGeyser_Ilist_8DA9;
  if (!E->fgr_parameter_2) {
    E->base.current_instruction = addr_kFireGeyser_Ilist_8D1B;
    E->fgr_var_A = FUNC16(FireGeyser_Func_1);
    E->fgr_var_D = E->base.y_pos;
    E->base.x_width = 0;
  }
}

void CallFireGeyserFunc(uint32 ea) {
  switch (ea) {
  case fnFireGeyser_Func_1: FireGeyser_Func_1(); return;
  case fnFireGeyser_Func_2: FireGeyser_Func_2(); return;
  default: Unreachable();
  }
}

void FireGeyser_Main(void) {  // 0xA69023
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index);
  if (!E->fgr_parameter_2)
    CallFireGeyserFunc(E->fgr_var_A | 0xA60000);
}

void FireGeyser_Func_1(void) {  // 0xA6902F
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index);
  if ((--E->fgr_var_B & 0x8000u) != 0) {
    E->fgr_var_A = FUNC16(FireGeyser_Func_2);
    E->fgr_var_C = 0;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
    E->base.current_instruction = addr_kFireGeyser_Ilist_8D1B;
    E->base.properties &= ~kEnemyProps_Invisible;
    Enemy_FireGeyser *E1 = Get_FireGeyser(cur_enemy_index + 64);
    E1->base.properties &= ~0x400u;
  }
}

void FireGeyser_Func_2(void) {  // 0xA69062
  Enemy_FireGeyser *E = Get_FireGeyser(cur_enemy_index);
  if (E->fgr_var_C) {
    E->fgr_var_B = E->fgr_parameter_1;
    E->base.properties |= kEnemyProps_Invisible;
    E->fgr_var_A = FUNC16(FireGeyser_Func_1);
  }
}

void NuclearWaffle_Init(void) {  // 0xA694C4
  Enemy_NuclearWaffle *E = Get_NuclearWaffle(cur_enemy_index);
  E->base.current_instruction = addr_kNuclearWaffle_Ilist_9490;
  E->nwe_var_C = LOBYTE(E->nwe_parameter_1);
  E->nwe_var_D = HIBYTE(E->nwe_parameter_1);
  E->nwe_var_E = LOBYTE(E->nwe_parameter_2);
  uint16 nwe_parameter_2_high = HIBYTE(E->nwe_parameter_2);
  E->nwe_var_F = nwe_parameter_2_high;
  E->nwe_var_B = nwe_parameter_2_high;
  E->nwe_var_A = FUNC16(NuclearWaffle_Func_1);
  int v2 = (uint16)(4 * E->nwe_var_E) >> 1;
  uint16 v3 = g_word_A695F6[v2];
  E->nwe_var_23 = v3;
  E->nwe_var_21 = v3;
  E->nwe_var_2E = g_word_A695F6[v2 + 1];
  E->nwe_var_2B = g_word_A695FE[v2];
  E->nwe_var_2C = g_word_A695FE[v2 + 1];
  E->nwe_var_30 = g_word_A69606[v2];
  E->nwe_var_2F = g_word_A69606[v2 + 1];
  uint16 v4 = 8 * E->nwe_var_C;
  if (!E->nwe_var_E)
    v4 += 4;
  int v5 = v4 >> 1;
  E->nwe_var_25 = kCommonEnemySpeeds_Linear[v5];
  E->nwe_var_24 = kCommonEnemySpeeds_Linear[v5 + 1];
  E->nwe_var_26 = E->base.x_pos;
  E->nwe_var_27 = E->base.y_pos;
  draw_enemy_layer = E->nwe_var_D;
  uint16 v6 = E->nwe_var_26 + CosineMult8bit(E->nwe_var_23);
  E->nwe_var_28 = v6;
  E->base.x_pos = v6;
  draw_enemy_layer = E->nwe_var_D;
  uint16 v7 = E->nwe_var_27 + SineMult8bitNegative(E->nwe_var_23);
  E->nwe_var_29 = v7;
  E->base.y_pos = v7;
  uint16 v8 = 8;
  E->nwe_var_2A = 8;
  do {
    SpawnEnemyProjectileWithGfx(v8, cur_enemy_index, addr_kEproj_NuclearWaffleBody);
    v8 = E->nwe_var_2A - 2;
    E->nwe_var_2A = v8;
  } while (v8);
  E->nwe_var_2A = 6;
  uint16 v15;
  do {
    R18_ = E->base.x_pos;
    R20_ = E->base.y_pos;
    R22_ = 43;
    uint16 v13 = E->base.vram_tiles_index | E->base.palette_index;
    E->nwe_var_34 = v13;
    R24_ = v13;
    CreateSpriteAtPos();
    Get_NuclearWaffle(cur_enemy_index + E->nwe_var_2A)->nwe_var_10 = R18_;
    v15 = E->nwe_var_2A - 2;
    E->nwe_var_2A = v15;
  } while (v15);
}

void CallNuclearWaffleFunc(uint32 ea) {
  switch (ea) {
  case fnNuclearWaffle_Func_1: NuclearWaffle_Func_1(); return;
  case fnNuclearWaffle_Func_2: NuclearWaffle_Func_2(); return;
  default: Unreachable();
  }
}

void NuclearWaffle_Main(void) {  // 0xA6960E
  Enemy_NuclearWaffle *E = Get_NuclearWaffle(cur_enemy_index);
  CallNuclearWaffleFunc(E->nwe_var_A | 0xA60000);
}

void NuclearWaffle_Func_1(void) {  // 0xA69615
  Enemy_NuclearWaffle *E = Get_NuclearWaffle(cur_enemy_index);
  if ((--E->nwe_var_B & 0x8000u) != 0) {
    E->nwe_var_B = E->nwe_var_F;
    E->nwe_var_21 = E->nwe_var_23;
    E->nwe_var_A = FUNC16(NuclearWaffle_Func_2);
    E->nwe_var_33 = 0;
    E->nwe_var_08 = 0;
    E->nwe_var_09 = 0;
    E->nwe_var_0A = 0;
    E->nwe_var_0B = 0;
    E->nwe_var_0C = 0;
    E->nwe_var_0D = 0;
    E->nwe_var_0E = 0;
    E->nwe_var_0F = 0;
    E->nwe_var_18 = 0;
    E->nwe_var_19 = 0;
    E->nwe_var_1A = 0;
    E->nwe_var_1B = 0;
    E->nwe_var_1C = 0;
    E->nwe_var_1D = 0;
    E->nwe_var_1E = 0;
    E->nwe_var_1F = 0;
    E->base.properties |= kEnemyProps_ProcessedOffscreen;
  }
}

void NuclearWaffle_Func_2(void) {  // 0xA69682
  Enemy_NuclearWaffle *E = Get_NuclearWaffle(cur_enemy_index);
  draw_enemy_layer = E->nwe_var_D;
  g_word_7E001E = NuclearWaffle_Func_6(E->nwe_var_21);
  uint16 v8 = g_word_7E001E;
  if (g_word_7E001E != E->nwe_var_33) {
    R18_ = E->base.x_pos;
    R20_ = E->base.y_pos;
    R22_ = 46;
    R24_ = E->nwe_var_34;
    CreateSpriteAtPos();
    R18_ = E->base.x_pos;
    R20_ = E->base.y_pos;
    R22_ = R26_ + 44;
    R24_ = E->nwe_var_34;
    CreateSpriteAtPos();
    NuclearWaffle_Func_7();
  }
  E->nwe_var_33 = v8;
  uint16 v4 = NuclearWaffle_Func_5(E->nwe_var_21);
  E->base.x_pos = E->nwe_var_26 + CosineMult8bit(v4);
  uint16 v5 = NuclearWaffle_Func_5(E->nwe_var_21);
  E->base.y_pos = E->nwe_var_27 + SineMult8bitNegative(v5);
  NuclearWaffle_Func_3();
  NuclearWaffle_Func_4();
  uint16 nwe_var_20 = E->nwe_var_20;
  bool v7 = __CFADD__uint16(E->nwe_var_24, nwe_var_20);
  E->nwe_var_20 = E->nwe_var_24 + nwe_var_20;
  E->nwe_var_21 += E->nwe_var_25 + v7;
}

void NuclearWaffle_Func_3(void) {  // 0xA69721
  Enemy_NuclearWaffle *E = Get_NuclearWaffle(cur_enemy_index);
  uint16 v12;
  E->nwe_var_2A = 8;
  R36 = E->nwe_var_2C + E->nwe_var_21;
  do {
    R36 -= E->nwe_var_2B;
    Enemy_NuclearWaffle *ET = Get_NuclearWaffle(cur_enemy_index + E->nwe_var_2A);
    uint16 nwe_var_00 = ET->nwe_var_00;
    g_word_7E001E = NuclearWaffle_Func_6(R36);
    uint16 v13 = g_word_7E001E;
    if (g_word_7E001E != ET->nwe_var_08) {
      int v5 = nwe_var_00 >> 1;
      R18_ = enemy_projectile_x_pos[v5];
      R20_ = enemy_projectile_y_pos[v5];
      R22_ = 46;
      R24_ = E->nwe_var_34;
      CreateSpriteAtPos();
      R18_ = enemy_projectile_x_pos[v5];
      R20_ = enemy_projectile_y_pos[v5];
      R22_ = R26_ + 44;
      R24_ = E->nwe_var_34;
      CreateSpriteAtPos();
      NuclearWaffle_Func_7();
    }
    ET->nwe_var_08 = v13;
    uint16 v7 = NuclearWaffle_Func_5(R36);
    uint16 v8 = CosineMult8bit(v7);
    int v10 = nwe_var_00 >> 1;
    enemy_projectile_x_pos[v10] = E->nwe_var_26 + v8;
    uint16 v11 = NuclearWaffle_Func_5(R36);
    enemy_projectile_y_pos[v10] = E->nwe_var_27 + SineMult8bitNegative(v11);
    v12 = E->nwe_var_2A - 2;
    E->nwe_var_2A = v12;
  } while (v12);
  if (R28_) {
    E->nwe_var_A = FUNC16(NuclearWaffle_Func_1);
    E->base.properties &= ~kEnemyProps_ProcessedOffscreen;
  }
}

void NuclearWaffle_Func_4(void) {  // 0xA697E9
  Enemy_NuclearWaffle *E = Get_NuclearWaffle(cur_enemy_index);
  E->nwe_var_2A = 6;
  R36 = E->nwe_var_21;
  uint16 v12;
  do {
    R36 -= E->nwe_var_2B;
    Enemy_NuclearWaffle *ET = Get_NuclearWaffle(cur_enemy_index + E->nwe_var_2A);
    uint16 nwe_var_10 = ET->nwe_var_10;
    g_word_7E001E = NuclearWaffle_Func_6(R36);
    uint16 v13 = g_word_7E001E;
    if (g_word_7E001E != ET->nwe_var_18) {
      R38 = E->nwe_var_34;
      int v5 = nwe_var_10 >> 1;
      R18_ = sprite_x_pos[v5];
      R20_ = sprite_y_pos[v5];
      R22_ = 46;
      R24_ = R38;
      CreateSpriteAtPos();
      R18_ = sprite_x_pos[v5];
      R20_ = sprite_y_pos[v5];
      R22_ = R26_ + 44;
      R24_ = R38;
      CreateSpriteAtPos();
      NuclearWaffle_Func_7();
    }
    ET->nwe_var_18 = v13;
    uint16 v6 = NuclearWaffle_Func_5(R36);
    uint16 v7 = CosineMult8bit(v6);
    R32 = E->nwe_var_26 + v7;
    uint16 v9 = NuclearWaffle_Func_5(R36);
    R34 = E->nwe_var_27 + SineMult8bitNegative(v9);
    sprite_x_pos[nwe_var_10 >> 1] = R32;
    sprite_y_pos[nwe_var_10 >> 1] = R34;
    v12 = E->nwe_var_2A - 2;
    E->nwe_var_2A = v12;
  } while (v12);
}

uint16 NuclearWaffle_Func_5(uint16 a) {  // 0xA698AD
  R28_ = 0;
  Enemy_NuclearWaffle *E = Get_NuclearWaffle(cur_enemy_index);
  if (!E->nwe_var_E) {
    if ((int16)(a - E->nwe_var_2E) >= 0) {
      if ((int16)(a - E->nwe_var_23) < 0)
        return a;
      return E->nwe_var_23;
    }
LABEL_8:
    ++R28_;
    return E->nwe_var_2E;
  }
  if ((int16)(a - E->nwe_var_2E) >= 0)
    goto LABEL_8;
  if ((int16)(a - E->nwe_var_23) < 0)
    return E->nwe_var_23;
  return a;
}

uint16 NuclearWaffle_Func_6(uint16 a) {  // 0xA698E7
  Enemy_NuclearWaffle *E = Get_NuclearWaffle(cur_enemy_index);
  if (E->nwe_var_E) {
    if ((int16)(a - E->nwe_var_2F) >= 0) {
      R26_ = 1;
      return 2;
    } else if ((int16)(a - E->nwe_var_30) >= 0) {
      R26_ = 0;
      return 1;
    } else {
      return 0;
    }
  } else if ((int16)(a - E->nwe_var_2F) < 0) {
    R26_ = 0;
    return 2;
  } else if ((int16)(a - E->nwe_var_30) < 0) {
    R26_ = 1;
    return 1;
  } else {
    return 0;
  }
}

void NuclearWaffle_Func_7(void) {  // 0xA6993F
  if (g_word_7E001E != 2)
    QueueSfx2_Max6(0x5Eu);
}

void FakeKraid_Init(void) {  // 0xA69A58
  uint16 v0 = (random_number & 3) + 2;
  Enemy_FakeKraid *E = Get_FakeKraid(cur_enemy_index);
  E->fkd_var_D = v0;
  E->fkd_var_E = v0;
  v0 += 64;
  E->fkd_var_03 = v0;
  v0 += 32;
  E->fkd_var_04 = v0;
  E->fkd_var_05 = v0 - 48;
  E->fkd_var_07 = 0;
  E->base.properties |= kEnemyProps_DisableSamusColl;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->fkd_var_B = -4;
  E->fkd_var_C = -4;
  uint16 v2 = addr_stru_A699AE;
  if ((int16)(E->base.x_pos - samus_x_pos) < 0) {
    E->fkd_var_B = 4;
    E->fkd_var_C = 4;
    v2 = addr_stru_A699FC;
  }
  E->base.current_instruction = v2;
}

void FakeKraid_Main(void) {  // 0xA69AC2
  Enemy_FakeKraid *E = Get_FakeKraid(cur_enemy_index);
  uint16 fkd_var_07 = E->fkd_var_07;
  uint16 v3 = fkd_var_07 + 2;
  if (!sign16(fkd_var_07 - 4))
    v3 = 0;
  E->fkd_var_07 = v3;
  FakeKraid_Func_1(cur_enemy_index, fkd_var_07);
}

void FakeKraid_Func_1(uint16 k, uint16 j) {  // 0xA69ADC
  R18_ = k;
  Enemy_FakeKraid *ET = Get_FakeKraid(k + j);
  uint16 fkd_var_03 = ET->fkd_var_03;
  if (fkd_var_03) {
    ET->fkd_var_03 = fkd_var_03 - 1;
  } else {
    ET->fkd_var_03 = (random_number & 0x3F) + 16;
    Enemy_FakeKraid *E = Get_FakeKraid(k);
    E->fkd_var_06 = j;
    uint16 v6 = addr_kEproj_MiniKraidSpikesLeft;
    int16 fkd_var_C = E->fkd_var_C;
    if (fkd_var_C >= 0)
      v6 = addr_kEproj_MiniKraidSpikesRight;
    SpawnEnemyProjectileWithGfx(fkd_var_C, k, v6);
    if (!CheckIfEnemyIsOnScreen())
      QueueSfx2_Max6(0x3Fu);
  }
}

const uint16 *FakeKraid_Instr_2(uint16 k, const uint16 *jp) {  // 0xA69B26
  Enemy_FakeKraid *E = Get_FakeKraid(cur_enemy_index);
  if (E->fkd_var_E)
    --E->fkd_var_E;
  if (E->fkd_var_D-- == 1) {
    E->fkd_var_D = (random_number & 3) + 7;
  } else {
    R20_ = E->fkd_var_B;
    R18_ = 0;
    if (!(Enemy_MoveRight_IgnoreSlopes(cur_enemy_index) & 1))
      goto LABEL_7;
  }
  E->fkd_var_B = -E->fkd_var_B;
LABEL_7:
  E->fkd_var_C = -4;
  if ((int16)(E->base.x_pos - samus_x_pos) < 0)
    E->fkd_var_C = 4;
  return jp;
}

const uint16 *FakeKraid_Instr_1(uint16 k, const uint16 *jp) {  // 0xA69B74
  Enemy_FakeKraid *E = Get_FakeKraid(cur_enemy_index);
  if (E->fkd_var_E) {
    if ((E->fkd_var_C & 0x8000u) != 0) {
      if ((Get_FakeKraid(cur_enemy_index)->fkd_var_B & 0x8000u) == 0)
        return INSTR_RETURN_ADDR(addr_stru_A699C6);
      return INSTR_RETURN_ADDR(addr_stru_A699AE);
    } else {
      if ((E->fkd_var_B & 0x8000u) != 0)
        return INSTR_RETURN_ADDR(addr_stru_A69A14);
      return INSTR_RETURN_ADDR(addr_stru_A699FC);
    }
  } else {
    E->fkd_var_E = (random_number & 3) + 3;
    if ((E->fkd_var_C & 0x8000u) != 0)
      return INSTR_RETURN_ADDR(addr_kFakeKraid_Ilist_99DC);
    return INSTR_RETURN_ADDR(addr_kFakeKraid_Ilist_9A2A);
  }
}

const uint16 *FakeKraid_Instr_3(uint16 k, const uint16 *jp) {  // 0xA69BB2
  if (CheckIfEnemyIsOnScreen() == 0)
    QueueSfx2_Max6(0x16u);
  return jp;
}

const uint16 *FakeKraid_Instr_4(uint16 k, const uint16 *jp) {  // 0xA69BC4
  FakeKraid_InstrHelper_45(k, 0, 0xFFFC);
  return jp;
}

void FakeKraid_InstrHelper_45(uint16 k, uint16 j, uint16 a) {  // 0xA69BCB
  uint16 ka = cur_enemy_index;
  Enemy_FakeKraid *E = Get_FakeKraid(cur_enemy_index);
  E->fkd_var_02 = a;
  int v5 = j >> 1;
  E->fkd_var_00 = g_word_A69A48[v5];
  uint16 v6 = g_word_A69A48[v5 + 1];
  E->fkd_var_01 = v6;
  SpawnEnemyProjectileWithGfx(v6, cur_enemy_index, addr_kEproj_MiniKraidSpit);
  E->fkd_var_00 = g_word_A69A48[v5 + 2];
  uint16 v8 = g_word_A69A48[v5 + 3];
  E->fkd_var_01 = v8;
  SpawnEnemyProjectileWithGfx(v8, ka, addr_kEproj_MiniKraidSpit);
}

const uint16 *FakeKraid_Instr_5(uint16 k, const uint16 *jp) {  // 0xA69C02
  FakeKraid_InstrHelper_45(k, 8u, 4u);
  return jp;
}

void sub_A69C0B(void) {  // 0xA69C0B
  EnemyData *v0 = gEnemyData(cur_enemy_index);
  special_death_item_drop_x_origin_pos = v0->x_pos;
  special_death_item_drop_y_origin_pos = v0->y_pos;
  NormalEnemyPowerBombAiSkipDeathAnim_CurEnemy();
  FakeKraid_9C50();
}

void FakeKraid_Touch(void) {  // 0xA69C22
  Enemy_FakeKraid *E = Get_FakeKraid(cur_enemy_index);
  special_death_item_drop_x_origin_pos = E->base.x_pos;
  special_death_item_drop_y_origin_pos = E->base.y_pos;
  NormalEnemyTouchAiSkipDeathAnim_CurEnemy();
  FakeKraid_9C50();
}

void FakeKraid_Shot(void) {  // 0xA69C39
  Enemy_FakeKraid *E = Get_FakeKraid(cur_enemy_index);
  special_death_item_drop_x_origin_pos = E->base.x_pos;
  special_death_item_drop_y_origin_pos = E->base.y_pos;
  NormalEnemyShotAiSkipDeathAnim_CurEnemy();
  FakeKraid_9C50();
}

void FakeKraid_9C50(void) {  // 0xA69C50
  if (!Get_FakeKraid(cur_enemy_index)->base.health) {
    EnemyDeathAnimation(cur_enemy_index, 3u);
    Enemy_ItemDrop_MiniKraid(cur_enemy_index);
  }
}

void CeresRidley_Init(void) {  // 0xA6A0F5
  int16 v4;
  int16 v8;
  Enemy_CeresRidley *E = Get_CeresRidley(0);

  if ((boss_bits_for_area[area_index] & 1) != 0) {
    E->base.properties |= kEnemyProps_Tangible | kEnemyProps_Deleted | kEnemyProps_Invisible;
  } else {
    for (int i = 4094; i >= 0; i -= 2)
      tilemap_stuff[i >> 1] = 0;
    DisableMinimapAndMarkBossRoomAsExplored();
    E->cry_parameter_1 = 0;
    E->cry_parameter_2 = 0;
    Ridley_Func_99(addr_kRidley_Ilist_E538);
    E->base.palette_index = 3584;
    E->cry_var_0C = 3584;
    E->base.extra_properties |= 4u;
    E->cry_var_01 = 0;
    E->cry_var_0D = 0;
    earthquake_type = 0;
    earthquake_timer = 0;
    Ridley_Func_92();
    Ridley_Func_117();
    if (area_index == 2) {
      E->base.properties |= kEnemyProps_BlockPlasmaBeam | kEnemyProps_Tangible;
      E->base.x_pos = 96;
      E->base.y_pos = 394;
      E->cry_var_A = FUNC16(CeresRidley_Func_3);
      E->cry_var_B = 0;
      E->cry_var_C = 0;
      E->base.layer = 5;
      E->cry_var_01 = 0;
      E->cry_var_02 = 1;
      E->cry_var_20 = 64;
      E->cry_var_21 = 416;
      E->cry_var_22 = 64;
      E->cry_var_23 = 224;
      E->cry_var_10 = 2;
      E->cry_var_1C = 120;
      WriteColorsToTargetPalette(0xa6, 0x140, addr_word_A6E1CF, 0x20);
      uint16 v3 = 0;
      v4 = 15;
      do {
        int v5 = v3 >> 1;
        target_palettes[v5 + 113] = 0;
        target_palettes[v5 + 241] = 0;
        v3 += 2;
        --v4;
      } while (v4);
    } else {
      E->base.properties |= kEnemyProps_BlockPlasmaBeam | kEnemyProps_Tangible;
      E->base.x_pos = 186;
      E->base.y_pos = 169;
      ceres_status = 0;
      E->cry_var_10 = 0;
      E->cry_var_02 = 0;
      tilemap_stuff[1] = 1;
      E->cry_var_1C = 15;
      E->cry_var_A = FUNC16(CeresRidley_Func_3);
      E->cry_var_B = 0;
      E->cry_var_C = 0;
      E->cry_var_20 = -32;
      E->cry_var_21 = 176;
      E->cry_var_22 = 40;
      E->cry_var_23 = 224;
      WriteColorsToTargetPalette(0xa6, 0x140, addr_word_A6E16F, 0x20);
      uint16 v7 = 482;
      v8 = 15;
      do {
        target_palettes[v7 >> 1] = 0;
        v7 += 2;
        --v8;
      } while (v8);
      E->cry_var_03 = addr_kBabyMetroid_Ilist_BF31;
      E->cry_var_04 = 1;
      gRam8800_Default(0)->var_40 = FUNC16(Ridley_Func_50);
      E->cry_var_07 = 5;
      QueueMusic_Delayed8(0);
    }
  }
}

void CallRidleyFunc(uint32 ea) {
  switch (ea) {
  case fnCeresRidley_Func_2: CeresRidley_Func_2(); return;
  case fnCeresRidley_Func_3: CeresRidley_Func_3(); return;
  case fnCeresRidley_A377: CeresRidley_A377(); return;
  case fnCeresRidley_Func_4: CeresRidley_Func_4(); return;
  case fnCeresRidley_Func_5: CeresRidley_Func_5(); return;
  case fnCeresRidley_Func_6: CeresRidley_Func_6(); return;
  case fnCeresRidley_Func_7: CeresRidley_Func_7(); return;
  case fnCeresRidley_Func_9: CeresRidley_Func_9(); return;
  case fnCeresRidley_Func_10: CeresRidley_Func_10(); return;
  case fnCeresRidley_Func_11: CeresRidley_Func_11(); return;
  case fnCeresRidley_Func_13: CeresRidley_Func_13(); return;
  case fnCeresRidley_A7F9: CeresRidley_A7F9(); return;
  case fnCeresRidley_Func_14: CeresRidley_Func_14(); return;
  case fnCeresRidley_Func_15: CeresRidley_Func_15(); return;
  case fnCeresRidley_Func_16: CeresRidley_Func_16(); return;
  case fnCeresRidley_Func_17: CeresRidley_Func_17(); return;
  case fnCeresRidley_Func_18: CeresRidley_Func_18(); return;
  case fnCeresRidley_Func_19: CeresRidley_Func_19(); return;
  case fnCeresRidley_Func_20: CeresRidley_Func_20(); return;
  case fnCeresRidley_Func_21: CeresRidley_Func_21(); return;
  case fnCeresRidley_Func_22: CeresRidley_Func_22(); return;
  case fnCeresRidley_Func_23: CeresRidley_Func_23(); return;
  case fnCeresRidley_Func_24: CeresRidley_Func_24(); return;
  case fnnullsub_233: return;
  case fnCeresRidley_Func_26: CeresRidley_Func_26(); return;
  case fnRidley_Func_3: Ridley_Func_3(); return;
  case fnRidley_Func_3b: Ridley_Func_3b(); return;
  case fnRidley_Func_5: Ridley_Func_5(); return;
  case fnRidley_Func_6: Ridley_Func_6(); return;
  case fnRidley_Func_8: Ridley_Func_8(); return;
  case fnRidley_B455: Ridley_B455(); return;
  case fnRidley_Func_9: Ridley_Func_9(); return;
  case fnRidley_Func_10: Ridley_Func_10(); return;
  case fnRidley_Func_11: Ridley_Func_11(); return;
  case fnRidley_Func_12: Ridley_Func_12(); return;
  case fnRidley_Func_13: Ridley_Func_13(); return;
  case fnRidley_Func_14: Ridley_Func_14(); return;
  case fnRidley_Func_15: Ridley_Func_15(); return;
  case fnRidley_Func_16: Ridley_Func_16(); return;
  case fnRidley_Func_19: Ridley_Func_19(); return;
  case fnRidley_Func_20: Ridley_Func_20(); return;
  case fnRidley_Func_21: Ridley_Func_21(); return;
  case fnRidley_Func_22: Ridley_Func_22(); return;
  case fnRidley_Func_33: Ridley_Func_33(); return;
  case fnRidley_Func_33b: Ridley_Func_33b(); return;
  case fnRidley_Func_34: Ridley_Func_34(); return;
  case fnRidley_Func_35: Ridley_Func_35(); return;
  case fnRidley_Func_36: Ridley_Func_36(); return;
  case fnRidley_Func_43: Ridley_Func_43(); return;
  case fnRidley_Func_44: Ridley_Func_44(); return;
  case fnRidley_Func_45: Ridley_Func_45(); return;
  case fnRidley_Func_46: Ridley_Func_46(); return;
  case fnRidley_Func_47: Ridley_Func_47(); return;
  case fnRidley_Func_54: Ridley_Func_54(); return;
  case fnRidley_Func_63: Ridley_Func_63(); return;
  case fnRidley_C53E: Ridley_C53E(); return;
  case fnRidley_C551: Ridley_C551(); return;
  case fnRidley_Func_64: Ridley_Func_64(); return;
  case fnRidley_Func_65: Ridley_Func_65(); return;
  case fnRidley_Func_66: Ridley_Func_66(); return;
  case fnRidley_Func_67: Ridley_Func_67(); return;
  case fnnullsub_349: return;
  default: Unreachable();
  }
}
void CeresRidley_Main(void) {  // 0xA6A288
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  E->base.health = 0x7FFF;
  CallRidleyFunc(E->cry_var_A | 0xA60000);
  if (!ceres_status) {
    if (Get_CeresRidley(0)->cry_var_02) {
      Ridley_Func_102();
      Ridley_Func_112();
      Ridley_Func_115();
      CeresRidley_Func_1();
      Ridley_Func_70();
      Ridley_Func_101();
      Ridley_Func_118();
      sub_A6DB2A();
      Ridley_Func_120();
    }
    Ridley_A2DC();
  }
}

void CeresRidley_Func_1(void) {  // 0xA6A2BD
  if (random_number >= 0xFF00u)
    tilemap_stuff[15] = (random_number & 0xF) + 8;
}

void CeresRidley_Hurt(void) {  // 0xA6A2D3
  Ridley_Func_102();
  sub_A6DB2A();
  Ridley_Func_120();
  Ridley_A2DC();
}

void Ridley_A2DC(void) {  // 0xA6A2DC
  Ridley_Func_49();
  if (Get_CeresRidley(0)->cry_var_02) {
    mov24(&enemy_gfx_drawn_hook, 0xA6A2F2);
  } else {
    Ridley_A2F2();
  }
}

void Ridley_A2F2(void) {  // 0xA6A2F2
  if (!ceres_status)
    DrawBabyMetroid_0();
  Enemy_CeresRidley *E = Get_CeresRidley(0x40u);
  if (E->cry_var_B) {
    // bug
    static const int16 g_word_A6A321[4] = { 0, -1024, -4, -1 };

    R20_ = E->base.x_pos + g_word_A6A321[(uint8)(earthquake_timer & 3)];
    R18_ = E->base.y_pos;
    R22_ = 1024;
    DrawSpritemap(0xA6, addr_kCeresRidley_Sprmap_A329);
  }
}

void CeresRidley_Func_2(void) {  // 0xA6A354
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  E->cry_var_B = 0;
  E->cry_var_C = 0;
}

void CeresRidley_Func_3(void) {  // 0xA6A35B
  int16 v1;

  if (door_transition_flag_enemies) {
  } else {
    Enemy_CeresRidley *E = Get_CeresRidley(0);
    E->cry_var_A = FUNC16(CeresRidley_A377);
    v1 = 512;
    if (area_index == 2)
      v1 = 170;
    E->cry_var_F = v1;
    CeresRidley_A377();
  }
}

void CeresRidley_A377(void) {  // 0xA6A377
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  if ((--E->cry_var_F & 0x8000u) != 0) {
    E->cry_var_A = FUNC16(CeresRidley_Func_4);
    E->cry_var_E = 0;
    E->cry_var_F = 0;
  }
}

void CeresRidley_Func_4(void) {  // 0xA6A389
  int16 v2;

  Enemy_CeresRidley *E = Get_CeresRidley(0);
  if ((E->cry_var_E & 0x8000u) == 0) {
    uint16 cry_var_E = E->cry_var_E;
    if (++E->cry_var_F) {
      E->cry_var_F = 0;
      v2 = g_byte_A6E269[cry_var_E];
      if (v2 == 255) {
        E->cry_var_E = 0;
        E->cry_var_A = FUNC16(CeresRidley_Func_5);
        E->cry_var_02 = 1;
      } else {
        R18_ = g_byte_A6E269[cry_var_E];
        E->cry_var_E = cry_var_E + 1;
        int v3 = (uint16)(6 * v2) >> 1;
        palette_buffer[252] = g_word_A6E2AA[v3];
        palette_buffer[253] = g_word_A6E2AA[v3 + 1];
        palette_buffer[254] = g_word_A6E2AA[v3 + 2];
      }
    }
  }
}

void CeresRidley_Func_5(void) {  // 0xA6A3DF
  int16 v2;
  int16 v5;

  Enemy_CeresRidley *E = Get_CeresRidley(0);
  if (++E->cry_var_F >= 2u) {
    E->cry_var_F = 0;
    uint16 cry_var_E = E->cry_var_E;
    R18_ = 290;
    R20_ = 482;
    v2 = 11;
    do {
      v5 = v2;
      uint16 v3 = g_word_A6E30A[cry_var_E >> 1];
      palette_buffer[R18_ >> 1] = v3;
      palette_buffer[R20_ >> 1] = v3;
      cry_var_E += 2;
      ++R18_;
      ++R18_;
      ++R20_;
      ++R20_;
      v2 = v5 - 1;
    } while (v5 != 1);
    if (cry_var_E >= 0x160u) {
      if (area_index == 2)
        E->base.layer = 2;
      E->base.properties &= ~kEnemyProps_Tangible;
      E->cry_var_E = 0;
      E->cry_var_A = FUNC16(CeresRidley_Func_6);
      E->cry_var_F = 4;
      QueueMusic_Delayed8(5u);
    } else {
      E->cry_var_E = cry_var_E;
    }
  }
}

void CeresRidley_Func_6(void) {  // 0xA6A455
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  if ((--E->cry_var_F & 0x8000u) != 0) {
    Ridley_Func_99(addr_kRidley_Ilist_E690);
    E->cry_var_F = 0;
    E->cry_var_A = FUNC16(CeresRidley_Func_7);
    if (area_index != 2)
      E->cry_var_F = 252;
  }
}

void CeresRidley_Func_7(void) {  // 0xA6A478
  Enemy_CeresRidley *E = Get_CeresRidley(0);

  if (area_index == 2) {
    bool v1 = (--E->cry_var_F & 0x8000u) != 0;
    if (!v1)
      return;
    E->cry_var_F = 2;
    uint16 v2 = E->cry_var_E + 1;
    E->cry_var_E = v2;
    if (!(CeresRidley_Func_8(v2 - 1) & 1))
      return;
    fx_target_y_pos = 440;
    fx_y_vel = -96;
    fx_timer = 32;
  } else {
    bool v1 = (--E->cry_var_F & 0x8000u) != 0;
    if (!v1)
      return;
  }
  E->cry_var_E = 0;
  Ridley_Func_99(addr_kRidley_Ilist_E91D);
  E->cry_var_08 = 8;
  E->cry_var_09 = 8;
  Ridley_Func_95();
  tilemap_stuff[0] = 1;
  E->cry_var_A = FUNC16(CeresRidley_Func_2);
}

uint8 CeresRidley_Func_8(uint16 a) {  // 0xA6A4D6
  uint16 v1 = g_off_A6A4EB[a];
  if (!v1)
    return 1;
  WriteColorsToPalette(0xE2, 0xa6, v1, 0xE);
  return 0;
}

void CeresRidley_Func_9(void) {  // 0xA6A6AF
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  E->cry_var_C -= 16;
  if (sign16(E->base.y_pos - 112)) {
    E->cry_var_A = FUNC16(CeresRidley_Func_10);
    CeresRidley_Func_10();
  }
}

void CeresRidley_Func_10(void) {  // 0xA6A6C8
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  E->cry_var_C += 20;
  if (sign16(E->base.y_pos - 80)) {
    E->cry_var_A = FUNC16(CeresRidley_Func_11);
    E->cry_var_01 = 1;
  }
}

void CeresRidley_Func_11(void) {  // 0xA6A6E8
  uint16 v1;

  Enemy_CeresRidley *E = Get_CeresRidley(0);
  if (E->cry_var_0D >= 0x64u) {
    E->cry_var_01 = 0;
    E->cry_var_A = FUNC16(Ridley_Func_44);
    Ridley_Func_44();
  } else if (sign16(samus_health - 30)) {
    E->cry_var_01 = 0;
    E->cry_var_A = FUNC16(CeresRidley_Func_22);
    CeresRidley_Func_22();
  } else if (!(CeresRidley_Func_12() & 1)
             || (v1 = E->cry_var_00 + 1, E->cry_var_00 = v1, v1 >= 0x7Cu)) {
    E->cry_var_A = g_off_A6A743[random_number & 0xF];
    E->cry_var_00 = 0;
  }
}

uint8 CeresRidley_Func_12(void) {  // 0xA6A763
  R18_ = 192;
  R20_ = 100;
  Ridley_Func_106(0, 0);
  R22_ = 8;
  R24_ = 8;
  return Shitroid_Func_2(0) & 1;
}

void CeresRidley_Func_13(void) {  // 0xA6A782
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  uint16 v1 = abs16(E->cry_var_C);
  if (v1 < 0x80)
    v1 = 128;
  E->cry_var_C = sign16(E->cry_var_C) ? -v1 : v1;
  R18_ = E->base.x_pos;
  R20_ = 88;
  Ridley_Func_106(0, 0);
  if (sign16(E->base.y_pos - 80)) {
    uint16 v3 = E->cry_var_00 + 1;
    E->cry_var_00 = v3;
    if (v3 >= 0x30u)
      E->cry_var_A = FUNC16(CeresRidley_Func_16);
  } else if (sign16(E->base.y_pos - 128)) {
    E->cry_var_17 = E->base.x_pos;
    E->cry_var_18 = E->base.y_pos;
    Ridley_Func_99(addr_kRidley_Ilist_E73A);
    E->cry_var_A = addr_loc_A6A7F9;
    E->cry_var_00 = 224;
    CeresRidley_A7F9();
  }
}

void CeresRidley_A7F9(void) {  // 0xA6A7F9
  unsigned int v3; // kr00_4
  int16 v4;

  R22_ = sign16(random_number) ? -(random_number & 7) : (random_number & 7);
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  uint16 cry_var_17 = E->cry_var_17;
  R18_ = R22_ + cry_var_17;
  v3 = __PAIR32__(R22_, R22_) + __PAIR32__(E->cry_var_18, cry_var_17);
  uint16 v2 = (__PAIR32__(R22_, R22_) + __PAIR32__(E->cry_var_18, cry_var_17)) >> 16;
  R18_ = v3;
  R20_ = v2;
  Ridley_Func_106(0, 0);
  v4 = E->cry_var_00 - 1;
  E->cry_var_00 = v4;
  if (v4 < 0) {
    E->cry_var_00 = 0;
    E->cry_var_A = FUNC16(CeresRidley_Func_11);
  }
}

void CeresRidley_Func_14(void) {  // 0xA6A83C
  Ridley_Func_99(addr_kRidley_Ilist_E548);
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  E->cry_var_A = FUNC16(CeresRidley_Func_15);
  E->cry_var_F = 64;
  CeresRidley_Func_15();
}

void CeresRidley_Func_15(void) {  // 0xA6A84E
  bool v2; // sf

  Enemy_CeresRidley *E = Get_CeresRidley(0);
  R18_ = samus_x_pos;
  uint16 v0 = samus_y_pos - 68;
  if (sign16(samus_y_pos - 132))
    v0 = 64;
  R20_ = v0;
  Ridley_Func_106(0, 0xD);
  R22_ = 2;
  R24_ = 2;
  if (!(Shitroid_Func_2(0) & 1)
      || (v2 = (int16)(E->cry_var_F - 1) < 0, --E->cry_var_F, v2)) {
    E->cry_var_00 = 0;
    E->cry_var_A = FUNC16(CeresRidley_Func_11);
  }
}

void CeresRidley_Func_16(void) {  // 0xA6A88D
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  E->cry_var_A = FUNC16(CeresRidley_Func_17);
  E->cry_var_F = 10;
  E->cry_var_0A = 0;
  tilemap_stuff[1] = 0;
  CeresRidley_Func_17();
}

void CeresRidley_Func_17(void) {  // 0xA6A8A4
  R18_ = 192;
  R20_ = 80;
  Ridley_Func_106(0, 1u);
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  if (sign16(E->base.y_pos - 96)) {
    E->cry_var_A = FUNC16(CeresRidley_Func_18);
    E->cry_var_F = 10;
    E->cry_var_0A = 0;
  }
}

void CeresRidley_Func_18(void) {  // 0xA6A8D4
  R18_ = -32;
  R20_ = -1024;
  R22_ = 768;
  Ridley_Func_111();
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  if ((--E->cry_var_F & 0x8000u) != 0) {
    E->cry_var_A = FUNC16(CeresRidley_Func_19);
    E->cry_var_F = 36;
  }
}

void CeresRidley_Func_19(void) {  // 0xA6A8F8
  R18_ = -512;
  R20_ = -16384;
  R22_ = 768;
  Ridley_Func_111();
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  if ((--E->cry_var_F & 0x8000u) != 0) {
    E->cry_var_A = FUNC16(CeresRidley_Func_20);
    E->cry_var_F = 28;
    tilemap_stuff[2] = 1;
  }
}

void CeresRidley_Func_20(void) {  // 0xA6A923
  R18_ = -512;
  R20_ = -30720;
  R22_ = 768;
  Ridley_Func_111();
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  if ((--E->cry_var_F & 0x8000u) != 0) {
    E->cry_var_A = FUNC16(CeresRidley_Func_21);
    E->cry_var_F = 1;
  }
}

void CeresRidley_Func_21(void) {  // 0xA6A947
  R18_ = -768;
  R20_ = -30720;
  R22_ = 768;
  Ridley_Func_111();
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  if ((--E->cry_var_F & 0x8000u) != 0) {
    E->cry_var_A = FUNC16(CeresRidley_Func_11);
    E->cry_var_00 = 0;
    tilemap_stuff[1] = 1;
  }
}

void CeresRidley_Func_22(void) {  // 0xA6A971
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  E->cry_var_20 = -192;
  R18_ = 192;
  R20_ = -128;
  Ridley_Func_106(0, 1u);
  if (sign16(E->base.y_pos + 128))
    CeresRidley_A994();
}

void CeresRidley_A994(void) {  // 0xA6A994
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  E->cry_var_A = FUNC16(CeresRidley_Func_23);
  E->cry_var_F = 64;
  CeresRidley_Func_23();
}

void CeresRidley_Func_23(void) {  // 0xA6A9A0
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  if ((--E->cry_var_F & 0x8000u) != 0) {
    CeresRidley_Func_25();
    E->cry_var_B = 0;
    E->cry_var_C = 0;
    tilemap_stuff[0] = 0;
    E->cry_var_A = FUNC16(CeresRidley_Func_24);
    WriteColorsToPalette(0xA2, 0xa6, addr_word_A6A9E3, 0xF);
    WriteColorsToPalette(0x42, 0xa6, addr_word_A6AA01, 8u);
    WriteColorsToPalette(0x1E2, 0xa6, addr_word_A6AA01, 8u);
  }
}

void CeresRidley_Func_24(void) {  // 0xA6AA11
  Get_CeresRidley(0)->cry_var_A = FUNC16(nullsub_233);
  ceres_status = 1;
  CeresRidley_Func_27();
}

void CeresRidley_Func_25(void) {  // 0xA6AA20
  SpawnEnemy(0xA6, addr_stru_A6AA2F);
  SpawnEnemy(0xA6, addr_stru_A6AA3F);
}

void CeresRidley_Func_26(void) {  // 0xA6AA50
  Ridley_Func_61();
}

void CeresRidley_Func_27(void) {  // 0xA6AA54
  reg_BGMODE_fake = 7;
  irq_enable_mode7 = 1;
  reg_M7SEL = 0x80;
  reg_M7A = 256;
  reg_M7B = 256;
  reg_M7C = 256;
  reg_M7D = 256;
  reg_M7X = 64;
  reg_M7Y = 64;
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  E->cry_var_32 = 0;
  E->cry_var_33 = 0;
  E->cry_var_0A = 1;
  E->cry_var_34 = -128;
  reg_BG1HOFS = -128;
  E->cry_var_35 = 32;
  reg_BG1VOFS = 32;
  E->cry_var_30 = 2048;
  E->cry_var_31 = 1024;
}

void HandleCeresRidleyGetawayCutscene(void) {  // 0xA6AAAF
  if (!Get_CeresRidley(0)->cry_var_32)
    CeresRidley_Func_28();
}

void CeresRidley_Func_28(void) {  // 0xA6AABD
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  uint16 cry_var_33 = E->cry_var_33;
  E->cry_var_33 = cry_var_33 + 2;
  if (!cry_var_33)
    QueueSfx2_Max6(0x4Eu);
  if (cry_var_33 == 208) {
    Samus_SetPushedOutOfCeresRidley();
    earthquake_type = 35;
    earthquake_timer = 64;
  }
  int v2 = cry_var_33 >> 1;
  if (g_word_A6AE4D[v2] == 0xFFFF) {
    ++E->cry_var_32;
    reg_BGMODE_fake = 9;
    reg_M7SEL = 0;
    reg_M7A = 0;
    reg_M7B = 0;
    reg_M7C = 0;
    reg_M7D = 0;
    reg_M7X = 0;
    reg_M7Y = 0;
    reg_BG1HOFS = 0;
    reg_BG1VOFS = 0;
    E->cry_var_A = FUNC16(Ridley_Func_54);
    E->cry_var_F = 0;
  } else {
    E->cry_var_30 = g_word_A6AE4D[v2];
    uint16 v3 = g_word_A6AF2F[v2] + E->cry_var_35;
    E->cry_var_35 = v3;
    reg_BG1VOFS = v3;
    uint16 v4 = E->cry_var_34 - g_word_A6B00F[v2];
    E->cry_var_34 = v4;
    reg_BG1HOFS = v4;
    CeresRidley_Func_35(E->cry_var_30);
    E->cry_var_0A += 48;
    CeresRidley_Func_29();
    CeresRidley_Func_33();
    CeresRidley_Func_34();
  }
}

uint16 MaybeNegate(uint16 cond, uint16 value) {
  return cond & 0x8000 ? -value : value;
}

void CeresRidley_Func_29(void) {  // 0xA6AB5F
  Enemy_CeresRidley *E = Get_CeresRidley(0);
  R18_ = HIBYTE(E->cry_var_0A);
  R24_ = CeresRidley_Func_31(0x100u);
  R38 = abs16(R24_);
  R40 = E->cry_var_30;
  CeresRidley_Func_32();
  reg_M7A = MaybeNegate(R24_, *(uint16 *)((char *)&R42 + 1));

  R24_ = CeresRidley_Func_30(0x100u);
  R38 = abs16(R24_);
  R40 = E->cry_var_30;
  CeresRidley_Func_32();
  reg_M7B = MaybeNegate(R24_, *(uint16 *)((char *)&R42 + 1));

  R24_ = -CeresRidley_Func_30(0x100u);
  R38 = abs16(R24_);
  R40 = E->cry_var_30;
  CeresRidley_Func_32();
  reg_M7C = MaybeNegate(R24_, *(uint16 *)((char *)&R42 + 1));

  R24_ = CeresRidley_Func_31(0x100u);
  R38 = abs16(R24_);
  R40 = E->cry_var_30;
  CeresRidley_Func_32();
  reg_M7D = MaybeNegate(R24_, *(uint16 *)((char *)&R42 + 1));
}

uint16 CeresRidley_Func_30(uint16 a) {  // 0xA6AC0E
  R20_ = a;
  R38 = abs16(a);
  return CeresRidley_AC30(R18_);
}

uint16 CeresRidley_Func_31(uint16 a) {  // 0xA6AC1E
  R20_ = a;
  R38 = abs16(a);
  return CeresRidley_AC30(R18_ + 64);
}

uint16 CeresRidley_AC30(uint16 a) {  // 0xA6AC30
  int v1 = (uint16)((2 * a) & 0x1FE) >> 1;
  uint16 result = kSinCosTable8bit_Sext[v1 + 64];
  if (result) {
    R22_ = kSinCosTable8bit_Sext[v1 + 64];
    R40 = abs16(result);
    CeresRidley_Func_32();
    if (((R22_ ^ R20_) & 0x8000u) != 0)
      return -*(uint16 *)((char *)&R42 + 1);
    else
      return *(uint16 *)((char *)&R42 + 1);
  }
  return result;
}

void CeresRidley_Func_32(void) {  // 0xA6AC58
  uint32 t = (uint32)R38 * (uint32)R40;
  R42 = (uint16)t;
  R44 = (uint16)(t >> 16);
}

void CeresRidley_Func_33(void) {  // 0xA6ACBC
  if ((nmi_frame_counter_word & 3) == 0) {
    Enemy_CeresRidley *E = Get_CeresRidley(0);
    uint16 v1 = ((uint8)E->cry_var_36 + 1) & 3;
    E->cry_var_36 = v1;
    QueueMode7Transfers(0xA6, g_off_A6ACDA[v1]);
  }
}

void CeresRidley_Func_34(void) {  // 0xA6AD27
  if ((nmi_frame_counter_word & 7) == 0) {
    Enemy_CeresRidley *E = Get_CeresRidley(0);
    uint16 v1 = ((uint8)E->cry_var_37 + 1) & 1;
    E->cry_var_37 = v1;
    QueueMode7Transfers(0xA6, g_off_A6AD45[v1]);
  }
}

void CeresRidley_Func_35(uint16 a) {  // 0xA6B0EF
  WriteColorsToPalette(0xA2, 0xa6, 32 * HIBYTE(a) - 0x4EF9, 0xF);
}

void Ridley_Main(void) {  // 0xA6B227
  int16 v1;

  Enemy_Ridley *E = Get_Ridley(0);
  v1 = E->ridley_var_24 - 4;
  if (v1 < 0)
    v1 = 0;
  E->ridley_var_24 = v1;
  Ridley_Func_39();
  Ridley_Func_42();
  CallRidleyFunc(E->ridley_var_A | 0xA60000);
  if (E->ridley_var_02) {
    Ridley_Func_102();
    Ridley_Func_112();
    Ridley_Func_115();
    Ridley_Func_70();
    Ridley_Func_129();
    sub_A6DB2A();
    Ridley_Func_120();
    Ridley_Func_118();
    if (E->ridley_var_1B)
      Ridley_Func_30();
  }
  Ridley_Func_100();
}

void Ridley_Func_1(void) {  // 0xA6B26F
  Enemy_Ridley *E = Get_Ridley(0);
  if (!(E->ridley_var_1B | E->ridley_var_1E)) {
    if (Ridley_Func_26(4u, 4u) & 1)
      Ridley_Func_37();
  }
}

void Ridley_Func_2(void) {  // 0xA6B28A
  Enemy_Ridley *E = Get_Ridley(0);
  E->ridley_var_24 = 0;
  E->base.frame_counter = 1;
  Ridley_Hurt();
}

void Ridley_Hurt(void) {  // 0xA6B297
  Enemy_Ridley *E = Get_Ridley(0);
  if ((E->base.frame_counter & 1) == 0) {
    Ridley_Func_39();
    Ridley_Func_42();
    CallRidleyFunc(E->ridley_var_A | 0xA60000);
    if (!Get_Ridley(0)->ridley_var_02) {
LABEL_7:
      if (Get_Ridley(0)->ridley_var_1B)
        Ridley_Func_30();
      return;
    }
    Ridley_Func_112();
    Ridley_Func_115();
    Ridley_Func_70();
  }
  Ridley_Func_129();
  Ridley_Func_102();
  sub_A6DB2A();
  Ridley_Func_120();
  Ridley_Func_100();
  Ridley_Func_118();
  uint16 v2 = E->ridley_var_24 + 1;
  if ((int16)(v2 - g_word_A6B288) >= 0)
    v2 = g_word_A6B288;
  E->ridley_var_24 = v2;
  if ((int16)(E->ridley_var_24 - g_word_A6B288) >= 0)
    goto LABEL_7;
}

void Ridley_Func_3(void) {  // 0xA6B2F3
  R18_ = 64;
  R20_ = 256;
  Ridley_Func_104_0(0, 14);
  R22_ = 8;
  R24_ = 8;
  if (!(Shitroid_Func_2(0) & 1)) {
    Enemy_Ridley *E = Get_Ridley(0);
    E->ridley_var_01 = 1;
    E->ridley_var_A = FUNC16(Ridley_Func_3b);
    Ridley_Func_3b();
  }
}

void Ridley_Func_3b(void) {  // 0xA6B321
  Ridley_Func_4();
  int v0 = (uint16)(2 * (NextRandom() & 7));
  uint16 v1 = *(uint16 *)&RomPtr_A6(R18_)[v0];
  Enemy_Ridley *E = Get_Ridley(0);
  E->ridley_var_A = v1;
  CallRidleyFunc(E->ridley_var_A | 0xA60000);
}

void Ridley_Func_4(void) {  // 0xA6B335
  if (samus_movement_type == 3) {
    R18_ = addr_off_A6B3CC;
  } else {
    Enemy_Ridley *E = Get_Ridley(0);
    uint16 health = E->base.health;
    if (health) {
      if (sign16(health - 14400)) {
        R18_ = addr_off_A6B38C;
      } else if (Ridley_Func_41() & 1) {
        R18_ = addr_off_A6B3BC;
      } else if ((Ridley_Func_40_Carry() & 1) != 0) {
        R18_ = addr_off_A6B3AC;
      } else {
        uint16 v2 = addr_off_A6B38C;
        if (sign16(E->base.health - 9000))
          v2 = addr_off_A6B39C;
        R18_ = v2;
      }
    } else {
      R18_ = addr_off_A6B3DC;
      ++E->ridley_var_25;
    }
  }
}

void Ridley_Func_5(void) {  // 0xA6B3EC
  Enemy_Ridley *E = Get_Ridley(0);
  E->ridley_var_A = FUNC16(Ridley_Func_6);
  E->ridley_var_F = 128;
  Ridley_Func_6();
}

void Ridley_Func_6(void) {  // 0xA6B3F8
  Enemy_Ridley *E = Get_Ridley(0);
  if ((--E->ridley_var_F & 0x8000u) != 0)
    goto LABEL_5;
  uint16 v2;
  v2 = 192;
  if (E->ridley_var_10)
    v2 = 96;
  R18_ = v2;
  R20_ = 256;
  uint16 v3;
  v3 = Ridley_Func_7();
  Ridley_Func_104_0(0, v3);
  R22_ = 8;
  R24_ = 8;
  if (!(Shitroid_Func_2(0) & 1))
    LABEL_5:
  E->ridley_var_A = FUNC16(Ridley_Func_3b);
}

uint16 Ridley_Func_7(void) {  // 0xA6B42E
  static const uint16 g_word_A6B439[4] = { 4, 8, 0xa, 0xc };
  return g_word_A6B439[Get_Ridley(0)->ridley_var_12];
}

void Ridley_Func_8(void) {  // 0xA6B441
  Enemy_Ridley *E = Get_Ridley(0);
  E->ridley_var_A = addr_loc_A6B455;
  E->ridley_var_00 = 10;
  E->ridley_var_0A = 0;
  Ridley_B455();
}

void Ridley_B455(void) {  // 0xA6B455
  Enemy_Ridley *E = Get_Ridley(0);
  uint16 v1 = 192;
  if (E->ridley_var_10)
    v1 = 64;
  R18_ = v1;
  R20_ = 128;
  Ridley_Func_104_0(0, 1u);
  R22_ = 8;
  R24_ = 8;
  if (!(Shitroid_Func_2(0) & 1)) {
    E->ridley_var_A = FUNC16(Ridley_Func_9);
    E->ridley_var_00 = 32;
    E->ridley_var_0A = 0;
  }
}

void Ridley_Func_9(void) {  // 0xA6B493
  Enemy_Ridley *E = Get_Ridley(0);
  if (E->ridley_var_10) {
    R18_ = 32;
    R20_ = 512;
  } else {
    R18_ = -32;
    R20_ = -512;
  }
  R22_ = 1152;
  Ridley_Func_111();
  uint16 ridley_var_00 = E->ridley_var_00;
  if (ridley_var_00) {
    E->ridley_var_00 = ridley_var_00 - 1;
  } else {
    E->ridley_var_A = FUNC16(Ridley_Func_10);
    E->ridley_var_00 = 20;
  }
}

void Ridley_Func_10(void) {  // 0xA6B4D1
  Enemy_Ridley *E = Get_Ridley(0);
  if (E->ridley_var_10) {
    R18_ = 320;
    R20_ = 0x4000;
  } else {
    R18_ = -320;
    R20_ = -16384;
  }
  R22_ = 1280;
  Ridley_Func_111();
  uint16 ridley_var_00 = E->ridley_var_00;
  if (ridley_var_00) {
    E->ridley_var_00 = ridley_var_00 - 1;
  } else {
    E->ridley_var_A = FUNC16(Ridley_Func_11);
    E->ridley_var_00 = 16;
    tilemap_stuff[2] = 1;
  }
}

void Ridley_Func_11(void) {  // 0xA6B516
  Enemy_Ridley *E = Get_Ridley(0);
  if (E->ridley_var_10) {
    R18_ = 512;
    R20_ = 30720;
  } else {
    R18_ = -512;
    R20_ = -30720;
  }
  R22_ = 768;
  Ridley_Func_111();
  uint16 ridley_var_00 = E->ridley_var_00;
  if (ridley_var_00) {
    E->ridley_var_00 = ridley_var_00 - 1;
  } else {
    E->ridley_var_A = FUNC16(Ridley_Func_12);
    E->ridley_var_00 = 32;
  }
}

void Ridley_Func_12(void) {  // 0xA6B554
  Enemy_Ridley *E = Get_Ridley(0);
  if (E->ridley_var_10) {
    R18_ = 1024;
    R20_ = 30720;
  } else {
    R18_ = -1024;
    R20_ = -30720;
  }
  R22_ = 768;
  Ridley_Func_111();
  uint16 ridley_var_00 = E->ridley_var_00;
  if (ridley_var_00) {
    E->ridley_var_00 = ridley_var_00 - 1;
  } else {
    E->ridley_var_A = FUNC16(Ridley_Func_13);
    E->ridley_var_00 = 32;
    Ridley_Func_114();
  }
}

void Ridley_Func_13(void) {  // 0xA6B594
  R18_ = 0;
  R20_ = FUNC16(Enemy_GrappleReact_NoInteract_A6);
  R22_ = 448;
  Ridley_Func_111();
  Enemy_Ridley *E = Get_Ridley(0);
  uint16 ridley_var_00 = E->ridley_var_00;
  if (ridley_var_00) {
    E->ridley_var_00 = ridley_var_00 - 1;
  } else {
    E->ridley_var_A = Ridley_Func_40_Carry() ? FUNC16(Ridley_Func_33) : FUNC16(Ridley_Func_3b);;
  }
}

void Ridley_Func_14(void) {  // 0xA6B5C4
  tilemap_stuff[15] = 11;
  tilemap_stuff[9] = 384;
  Enemy_Ridley *E = Get_Ridley(0);
  E->ridley_var_A = FUNC16(Ridley_Func_15);
  E->ridley_var_F = (random_number & 0x1F) + 32;
  Ridley_Func_15();
}

void Ridley_Func_15(void) {  // 0xA6B5E5
  Enemy_Ridley *E = Get_Ridley(0);
  R18_ = g_word_A6B60D[E->ridley_var_10];
  if (Ridley_Func_17() & 1) {
    Ridley_Func_18();
  } else if ((--E->ridley_var_F & 0x8000u) != 0) {
    E->ridley_var_A = FUNC16(Ridley_Func_16);
    E->ridley_var_F = 128;
    Ridley_Func_114();
  }
}

void Ridley_Func_16(void) {  // 0xA6B613
  Enemy_Ridley *E = Get_Ridley(0);
  R18_ = g_word_A6B63B[E->ridley_var_10];
  if (Ridley_Func_17() & 1) {
    Ridley_Func_18();
  } else if ((--E->ridley_var_F & 0x8000u) != 0) {
    E->ridley_var_A = FUNC16(Ridley_Func_15);
    E->ridley_var_F = 128;
    Ridley_Func_114();
  }
}

uint8 Ridley_Func_17(void) {  // 0xA6B641
  uint16 v0 = samus_y_pos;
  if (!sign16(samus_y_pos - 352))
    v0 = 352;
  R20_ = v0;
  uint16 v1 = Ridley_Func_7();
  Ridley_Func_104_0(0, v1);
  tilemap_stuff[2] = 1;
  if (samus_movement_type != 3)
    return 1;
  if ((uint8)random_number >= 0x80) {
    Enemy_Ridley *E = Get_Ridley(0);
    if (!E->ridley_var_0F && E->ridley_var_10 != 1)
      Ridley_Func_99(addr_kRidley_Ilist_E73A);
  }
  return 0;
}

void Ridley_Func_18(void) {  // 0xA6B68B
  tilemap_stuff[9] = 240;
  tilemap_stuff[15] = 16;
  tilemap_stuff[0] = 1;
  Get_Ridley(0)->ridley_var_A = FUNC16(Ridley_Func_19);
}

void Ridley_Func_19(void) {  // 0xA6B6A7
  Enemy_Ridley *E = Get_Ridley(0);
  if (sign16(E->base.y_pos - 288)) {
    Ridley_Func_114();
    E->ridley_var_A = FUNC16(Ridley_Func_20);
    E->ridley_var_F = 32;
    Ridley_Func_20();
  } else {
    R18_ = g_word_A6B6C8[E->ridley_var_10];
    R20_ = 288;
    Ridley_Func_104_0(0, 0);
  }
}

void Ridley_Func_20(void) {  // 0xA6B6DD
  Enemy_Ridley *E = Get_Ridley(0);
  R18_ = E->base.x_pos;
  R20_ = 288;
  Ridley_Func_104_0(0, 0);
  if ((--E->ridley_var_F & 0x8000u) != 0) {
    Ridley_Func_71();
    Ridley_Func_29();
    E->ridley_var_A = FUNC16(Ridley_Func_21);
    E->ridley_var_F = (random_number & 0x3F) + 128;
  }
}

void Ridley_Func_21(void) {  // 0xA6B70E
  int16 v1;

  if (Ridley_Func_25() & 1) {
    Ridley_Func_24();
    Ridley_Func_27();
  } else {
    Enemy_Ridley *E = Get_Ridley(0);
    v1 = tilemap_stuff[6] + E->ridley_var_C;
    if (!sign16(v1 - 1536))
      v1 = 1536;
    E->ridley_var_C = v1;
    if (Ridley_Func_23() & 1) {
      R18_ = tilemap_stuff[82];
      R20_ = tilemap_stuff[83] + 12;
      SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 9u);
      QueueSfx2_Max6(0x76u);
      earthquake_type = 13;
      earthquake_timer = 4;
      Ridley_Func_28();
      Ridley_Func_29();
      Ridley_Func_93(0x2026, addr_word_A6D37C);
      tilemap_stuff[20] = 3072;
      tilemap_stuff[30] = 3072;
      tilemap_stuff[40] = 3072;
      tilemap_stuff[50] = 3072;
      tilemap_stuff[60] = 3072;
      tilemap_stuff[70] = 3072;
      tilemap_stuff[80] = 3072;
      tilemap_stuff[0] = 4;
      uint16 v2 = E->ridley_var_26 + 1;
      if (!sign16(E->ridley_var_26 - 1)) {
        if (E->ridley_var_10 != 1)
          Ridley_Func_99(addr_kRidley_Ilist_E73A);
        v2 = 0;
      }
      E->ridley_var_26 = v2;
      E->ridley_var_A = FUNC16(Ridley_Func_22);
    }
  }
}

void Ridley_Func_22(void) {  // 0xA6B7B9
  Enemy_Ridley *E = Get_Ridley(0);
  bool v1;
  if (!(Ridley_Func_41() & 1)
      || (v1 = (int16)(E->ridley_var_F - 1) < 0, --E->ridley_var_F, v1)) {
    Ridley_Func_24();
    E->ridley_var_A = FUNC16(Ridley_Func_3b);
  } else {
    uint16 ridley_var_C = E->ridley_var_C;
    v1 = (int16)(tilemap_stuff[7] + ridley_var_C) < 0;
    E->ridley_var_C = tilemap_stuff[7] + ridley_var_C;
    if (!v1) {
      E->ridley_var_C = 0;
      E->ridley_var_A = FUNC16(Ridley_Func_21);
    }
  }
}

uint8 Ridley_Func_23(void) {  // 0xA6B7E7
  uint8 v0 = Ridley_Func_103(tilemap_stuff[82], tilemap_stuff[83] + 16) & 1;
  if (!v0) {
    v0 = Ridley_Func_103(tilemap_stuff[72], tilemap_stuff[73] + 18) & 1;
    if (!v0) {
      v0 = Ridley_Func_103(tilemap_stuff[62], tilemap_stuff[63] + 18) & 1;
      if (!v0) {
        v0 = Ridley_Func_103(tilemap_stuff[52], tilemap_stuff[53] + 18) & 1;
        if (!v0)
          return Ridley_Func_103(tilemap_stuff[42], tilemap_stuff[43] + 18) & 1;
      }
    }
  }
  return v0;
}

void Ridley_Func_24(void) {  // 0xA6B84D
  tilemap_stuff[0] = 1;
  tilemap_stuff[10] = 1;
}

uint8 Ridley_Func_25(void) {  // 0xA6B859
  if ((Ridley_Func_40_Carry() & 1) != 0)
    return Ridley_Func_26(4u, 4u) & 1;
  else
    return 0;
}

uint8 Ridley_Func_26(uint16 k, uint16 j) {  // 0xA6B865
  R22_ = k;
  R24_ = j;
  Enemy_Ridley *E = Get_Ridley(0);
  R18_ = E->base.x_pos + g_word_A6B9D5[E->ridley_var_10];
  R20_ = E->base.y_pos + g_word_A6B9DB[E->ridley_var_1D >> 1];
  return Ridley_Func_124() & 1;
}

void Ridley_Func_27(void) {  // 0xA6B889
  int16 ridley_var_C;

  Enemy_Ridley *E = Get_Ridley(0);
  ridley_var_C = E->ridley_var_C;
  if (ridley_var_C >= 0)
    ridley_var_C = -ridley_var_C;
  if (!sign16(ridley_var_C + 512))
    ridley_var_C = -512;
  E->ridley_var_C = ridley_var_C;
  Ridley_Func_24();
  E->ridley_var_A = FUNC16(Ridley_Func_33b);
  Ridley_Func_33b();
}

void Ridley_Func_28(void) {  // 0xA6B8A9
  Enemy_Ridley *E = Get_Ridley(0);
  if (!E->ridley_var_B)
    E->ridley_var_B = E->base.x_pos & 0x80 ? -0xc0 : 0xc0;
  if (((E->ridley_var_B ^ (E->base.x_pos - samus_x_pos)) & 0x8000u) == 0) {
    uint16 x_pos;
    x_pos = E->base.x_pos;
    if ((int16)(x_pos - E->ridley_var_22) >= 0) {
      if ((int16)(x_pos - E->ridley_var_23) < 0) {
        if (random_number < 0x555u)
          return;
        goto LABEL_7;
      }
      goto LABEL_15;
    }
LABEL_13:
    if ((E->ridley_var_B & 0x8000u) == 0)
      return;
LABEL_7:
    E->ridley_var_B = -E->ridley_var_B;
    return;
  }
  uint16 v4;
  v4 = E->base.x_pos;
  if ((int16)(v4 - E->ridley_var_22) < 0)
    goto LABEL_13;
  if ((int16)(v4 - E->ridley_var_23) >= 0) {
LABEL_15:
    if ((E->ridley_var_B & 0x8000u) != 0)
      return;
    goto LABEL_7;
  }
  if (random_number < 0x555u)
    goto LABEL_7;
}

void Ridley_Func_29(void) {  // 0xA6B90F
  int v0 = random_number & 3;
  R18_ = g_off_A6B965[v0];
  R20_ = g_off_A6B96D[v0];
  Enemy_Ridley *E = Get_Ridley(0);
  uint16 v2 = 2 * (E->ridley_var_12 + 2);
  int v3 = v2 >> 1;
  tilemap_stuff[7] = g_word_A6B94D[v3];
  tilemap_stuff[6] = g_word_A6B959[v3];
  E->ridley_var_C = *(uint16 *)&RomPtr_A6(R20_)[v2];
  const uint8 *v4 = RomPtr_A6(R18_);
  uint16 t = *(uint16 *)&v4[v2];
  E->ridley_var_B = sign16(E->ridley_var_B) ? -t : t;
}

void Ridley_Func_30(void) {  // 0xA6B9E1
  int16 v2;
  int16 v5;

  Enemy_Ridley *E = Get_Ridley(0);
  uint16 ridley_var_14 = E->ridley_var_14, v3, v6;
  if (ridley_var_14) {
    R18_ = E->ridley_var_14;
    v2 = abs16(ridley_var_14) - 4;
    if (v2 >= 0)
      v3 = sign16(ridley_var_14) ? -v2 : v2;
    else
      v3 = 0;
    E->ridley_var_14 = v3;
  }
  uint16 ridley_var_15 = E->ridley_var_15;
  if (ridley_var_15) {
    R18_ = E->ridley_var_15;
    v5 = abs16(ridley_var_15) - 4;
    if (v5 >= 0)
      v6 = sign16(ridley_var_15) ? -v5 : v5;
    else
      v6 = 0;
    E->ridley_var_15 = v6;
  }
  samus_x_pos = E->ridley_var_14
    + E->base.x_pos
    + g_word_A6B9D5[E->ridley_var_10];
  samus_y_pos = E->ridley_var_15 + E->base.y_pos + g_word_A6B9DB[E->ridley_var_1D >> 1];
}

void Ridley_Func_31(void) {  // 0xA6BA54
  Enemy_Ridley *E = Get_Ridley(0);
  E->ridley_var_14 = samus_x_pos
    - (E->base.x_pos
       + g_word_A6B9D5[E->ridley_var_10]);
  E->ridley_var_15 = samus_y_pos - (E->base.y_pos + g_word_A6B9DB[E->ridley_var_1D >> 1]);
}

void Ridley_Func_32(void) {  // 0xA6BA85
  Enemy_Ridley *E = Get_Ridley(0);
  if (sign16(E->ridley_var_25 - 10)) {
    if (power_bomb_flag) {
      E->ridley_var_A = FUNC16(Ridley_Func_43);
      Ridley_Func_43();
    } else {
      E->ridley_var_A = FUNC16(Ridley_Func_5);
      tilemap_stuff[2] = 1;
    }
  } else {
    Ridley_Func_126();
    E->ridley_var_A = FUNC16(Ridley_C53E);
    Ridley_C53E();
  }
}

void Ridley_Func_33(void) {  // 0xA6BAB7
  Enemy_Ridley *E = Get_Ridley(0);
  uint16 v1;
  int16 v2;

  if ((Ridley_Func_40_Carry() & 1) != 0
      && (!E->ridley_var_1F)
      && ((R18_ = g_word_A6BB48[E->ridley_var_10],
           R20_ = E->base.x_pos - samus_x_pos,
           ((R18_ ^ R20_) & 0x8000u) == 0)
          || (v1 = abs16(R20_), sign16(v1 - 32)))
      && (int16)(E->base.y_pos + 35 - samus_y_pos) < 0) {
    v2 = -16;
    if (E->ridley_var_10)
      v2 = 16;
    R18_ = samus_x_pos + v2;
    R20_ = samus_y_pos - 4;
    Ridley_Func_104_0(0, g_word_A6BB4E[E->ridley_var_12]);
    R18_ = E->base.x_pos + g_word_A6B9D5[E->ridley_var_10];
    R20_ = E->base.y_pos + 35;
    R22_ = 8;
    R24_ = 12;
    if (Ridley_Func_124() & 1) {
      E->ridley_var_C = -E->ridley_var_C;
      if (E->base.health) {
        if (power_bomb_flag) {
          Ridley_Func_37();
          E->ridley_var_A = FUNC16(Ridley_Func_43);
          Ridley_Func_43();
        } else {
          Ridley_Func_33b();
        }
      } else {
        if (!E->ridley_var_1B)
          Ridley_Func_37();
        Ridley_Func_126();
        E->ridley_var_A = FUNC16(Ridley_Func_63);
        Ridley_Func_63();
      }
    }
  } else {
    Ridley_Func_32();
  }
}

void Ridley_Func_33b(void) {  // 0xA6BB8F
  int16 v1;

  Enemy_Ridley *E = Get_Ridley(0);
  E->ridley_var_17 = g_word_A6BBEB[E->ridley_var_10];
  v1 = E->base.y_pos - 64;
  if (sign16(E->base.y_pos - 320))
    v1 = 256;
  E->ridley_var_18 = v1;
  if (!E->ridley_var_1B)
    Ridley_Func_37();
  E->ridley_var_A = FUNC16(Ridley_Func_34);
  E->ridley_var_F = 32;
  Ridley_Func_34();
}

void Ridley_Func_34(void) {  // 0xA6BBC4
  Enemy_Ridley *E = Get_Ridley(0);
  R18_ = E->ridley_var_17;
  R20_ = E->ridley_var_18;
  Ridley_Func_104_0(0, 0);
  if ((--E->ridley_var_F & 0x8000u) != 0) {
    E->ridley_var_A = FUNC16(Ridley_Func_35);
    E->ridley_var_F = 32;
  }
}

void Ridley_Func_35(void) {  // 0xA6BBF1
  Enemy_Ridley *E = Get_Ridley(0);
  if ((--E->ridley_var_F & 0x8000u) != 0) {
    tilemap_stuff[15] = 8;
    tilemap_stuff[9] = 240;
    Ridley_Func_38();
    E->ridley_var_A = FUNC16(Ridley_Func_36);
    E->ridley_var_F = 64;
  } else {
    R18_ = E->ridley_var_17;
    R20_ = 256;
    Ridley_Func_104_0(0, 0);
  }
}

void Ridley_Func_36(void) {  // 0xA6BC2E
  Enemy_Ridley *E = Get_Ridley(0);
  if ((--E->ridley_var_F & 0x8000u) != 0) {
    tilemap_stuff[15] = 16;
    tilemap_stuff[9] = 240;
    E->ridley_var_A = FUNC16(Ridley_Func_3b);
  } else {
    R18_ = g_word_A6BC62[E->ridley_var_10];
    R20_ = 224;
    Ridley_Func_104_0(0, 0);
  }
}

void Ridley_Func_37(void) {  // 0xA6BC68
  Ridley_Func_31();
  Enemy_Ridley *E = Get_Ridley(0);
  E->base.properties |= kEnemyProps_Tangible;
  E->ridley_var_1B = 1;
  CallSomeSamusCode(0);
  Ridley_Func_119(1);
}

void Ridley_Func_38(void) {  // 0xA6BC84
  tilemap_stuff[2] = 1;
  tilemap_stuff[0] = 1;
  Enemy_Ridley *E = Get_Ridley(0);
  if ((E->ridley_var_01 & 0x8000u) == 0) {
    E->ridley_var_1E = Ridley_Func_40_Sign() ? 6 : 10;
  }
  E->ridley_var_1B = 0;
  CallSomeSamusCode(1u);
  Ridley_Func_119(0);
}

void Ridley_Func_39(void) {  // 0xA6BCB4
  int16 ridley_var_01;

  Enemy_Ridley *E = Get_Ridley(0);
  ridley_var_01 = E->ridley_var_01;
  if (ridley_var_01 >= 0) {
    if (ridley_var_01) {
      if (Ridley_Func_121() & 1) {
        E->base.properties |= kEnemyProps_Tangible;
        return;
      }
      E->base.properties &= ~kEnemyProps_Tangible;
    }
    uint16 ridley_var_1E = E->ridley_var_1E;
    if (ridley_var_1E) {
      uint16 v3 = ridley_var_1E - 1;
      E->ridley_var_1E = v3;
      if (!v3 && (E->ridley_var_01 & 0x8000u) == 0)
        E->base.properties &= ~kEnemyProps_Tangible;
    }
  }
}

static const uint8 byte_A6BD04[28] = {
  0x80, 0x80, 0x80,    0, 0xff, 0x80, 0x80, 0xff, 0xff, 0xff, 0x80,    0, 0, 0x80, 0x80, 0x80,
  0x80, 0xff, 0xff, 0xff, 0x80, 0x80,    0, 0x80, 0x80,    0,    0, 0x80,
};

uint8 Ridley_Func_40_Carry(void) {
  return (byte_A6BD04[samus_movement_type] & 0x80) != 0;
}

uint8 Ridley_Func_40_Sign(void) {
  return (byte_A6BD04[samus_movement_type] & 0x40) != 0;
}


uint8 Ridley_Func_41(void) {  // 0xA6BD20
  return sign16(samus_y_pos - 352) == 0;
}

void Ridley_Func_42(void) {  // 0xA6BD2C
  int16 ridley_var_01;

  Enemy_Ridley *E = Get_Ridley(0);
  ridley_var_01 = E->ridley_var_01;
  if (ridley_var_01 > 0 && ridley_var_01 != 2 && power_bomb_flag && !E->ridley_var_1B) {
    Ridley_Func_24();
    E->ridley_var_A = FUNC16(Ridley_Func_33);
  }
}

void Ridley_Func_43(void) {  // 0xA6BD4E
  if (power_bomb_flag) {
    Get_Ridley(0)->ridley_var_01 = 2;
    uint16 v0 = 80;
    if (sign16(power_bomb_explosion_x_pos - 128))
      v0 = 192;
    R18_ = v0;
    uint16 v1 = 192;
    if (sign16(power_bomb_explosion_y_pos - 256))
      v1 = 384;
    R20_ = v1;
    uint16 v2 = Ridley_Func_7();
    Ridley_Func_104_0(0, v2);
  } else {
    Enemy_Ridley *E = Get_Ridley(0);
    E->ridley_var_01 = 1;
    E->ridley_var_A = E->ridley_var_1B ? FUNC16(Ridley_Func_33b) : FUNC16(Ridley_Func_3b);
  }
}

void Ridley_Func_44(void) {  // 0xA6BD9A
  R18_ = 192;
  R20_ = 128;
  Ridley_Func_106(0, 1u);
  Enemy_Ridley *E = Get_Ridley(0);
  if (!sign16(E->base.x_pos - 192))
    E->ridley_var_A = FUNC16(Ridley_Func_45);
}

void Ridley_Func_45(void) {  // 0xA6BDBC
  Enemy_Ridley *E = Get_Ridley(0);
  E->ridley_var_20 = -192;
  R18_ = 192;
  R20_ = -128;
  Ridley_Func_106(0, 1u);
  if (sign16(E->base.y_pos - 32)) {
    E->ridley_var_40 = FUNC16(Ridley_Func_52);
    E->ridley_var_A = FUNC16(Ridley_Func_46);
    E->ridley_var_F = 21;
  }
}

void Ridley_Func_46(void) {  // 0xA6BDF2
  Enemy_Ridley *E = Get_Ridley(0);
  if ((--E->ridley_var_F & 0x8000u) != 0) {
    Ridley_Func_99(addr_kRidley_Ilist_E658);
    E->ridley_var_A = FUNC16(Ridley_Func_47);
    Ridley_Func_47();
  }
}

void Ridley_Func_47(void) {  // 0xA6BE03
  Enemy_Ridley *E = Get_Ridley(0);
  R18_ = E->ridley_var_42 - 10;
  R20_ = E->ridley_var_44 - 56;
  Ridley_Func_104_0(0, 12);
  R18_ = E->base.x_pos + 14;
  R20_ = E->base.y_pos + 66;
  R22_ = 4;
  R24_ = 4;
  if (Ridley_Func_48() & 1) {
    E->ridley_var_1B = 1;
    Ridley_Func_119(1);
    E->ridley_var_C = -512;
    E->ridley_var_40 = FUNC16(Ridley_Func_51);
    E->ridley_var_A = FUNC16(CeresRidley_Func_22);
  }
}

uint8 Ridley_Func_48(void) {  // 0xA6BE61
  Enemy_Ridley *E = Get_Ridley(0);
  uint16 v1 = abs16(E->ridley_var_42 - R18_);
  bool v2 = v1 < 4u;
  uint16 v3 = v1 - 4;
  uint8 result = 0;
  if (v2 || v3 < R22_) {
    uint16 v4 = abs16(E->ridley_var_44 - R20_);
    v2 = v4 < 4u;
    uint16 v5 = v4 - 4;
    if (v2 || v5 < R24_)
      return 1;
  }
  return result;
}

void CallRidleyFunc_var40(uint32 ea) {
  switch (ea) {
  case fnRidley_Func_50: Ridley_Func_50(); return;
  case fnRidley_Func_51: Ridley_Func_51(); return;  // 0xa6beb3
  case fnRidley_Func_52: Ridley_Func_52(); return;  // 0xa6beca
  case fnRidley_Func_53: Ridley_Func_53(); return;  // 0xa6bedc
  case fnnullsub_348: return;  // 0xa6bf19
  default: Unreachable();
  }
}

void Ridley_Func_49(void) {  // 0xA6BE93
  R18_ = Get_Ridley(0)->ridley_var_40;
  CallRidleyFunc_var40(R18_ | 0xA60000);
}

void Ridley_Func_50(void) {  // 0xA6BE9C
  Enemy_Ridley *E = Get_Ridley(0);
  E->ridley_var_42 = E->base.x_pos - 16;
  E->ridley_var_44 = E->base.y_pos + 22;
}

void Ridley_Func_51(void) {  // 0xA6BEB3
  Enemy_Ridley *E = Get_Ridley(0);
  E->ridley_var_42 = E->base.x_pos + 14;
  E->ridley_var_44 = E->base.y_pos + 66;
}

void Ridley_Func_52(void) {  // 0xA6BECA
  Enemy_Ridley *E = Get_Ridley(0);
  E->ridley_var_45 = 0;
  E->ridley_var_46 = 0;
  E->ridley_var_40 = FUNC16(Ridley_Func_53);
  Ridley_Func_53();
}

void Ridley_Func_53(void) {  // 0xA6BEDC
  Enemy_Ridley *E = Get_Ridley(0);
  uint16 v1 = E->ridley_var_46 + 8;
  E->ridley_var_46 = v1;
  int t = HIBYTE(E->ridley_var_43) + (v1&0xff);
  HIBYTE(E->ridley_var_43) = t;
  E->ridley_var_44 += (int8)(v1 >> 8) + (t >> 8);
  if (!sign16(E->ridley_var_44 - 192)) {
    E->ridley_var_44 = 192;
    E->ridley_var_40 = addr_locret_A6BF19;
  }
}

void DrawBabyMetroid_0(void) {  // 0xA6BF1A
  int v0 = BabyMetroid_DBCB_DoubleRetEx(ADDR16_OF_RAM(*enemy_ram7800) + 6);
  if (v0 < 0)
    return;
  Enemy_Ridley *E = Get_Ridley(0);
  R18_ = E->ridley_var_42;
  R20_ = E->ridley_var_44;
  R22_ = 0;
  sub_A6DC13(v0);
}

uint16 BabyMetroid_Instr_2(uint16 k) {  // 0xA6BFC9
  if (!Get_Ridley(0)->ridley_var_46 && (random_number & 1) != 0)
    return BabyMetroid_Instr_4(k);
  QueueSfx3_Max6(0x24u);
  return BabyMetroid_Incr2(k);
}

uint16 BabyMetroid_Incr2(uint16 k) {  // 0xA6BFDE
  return k + 2;
}

uint16 BabyMetroid_Instr_3(uint16 k) {  // 0xA6BFE1
  uint16 v1 = *(uint16 *)RomPtr_A6(k);
  WriteColorsToPalette(0x162, 0xa6, v1, 0xF);
  return BabyMetroid_Incr2(k);
}

uint16 BabyMetroid_Instr_1(uint16 k) {  // 0xA6BFF2
  if (Get_Ridley(0)->ridley_var_46)
    return BabyMetroid_Instr_4(k);
  else
    return BabyMetroid_Incr2(k);
}

uint16 BabyMetroid_Instr_4(uint16 k) {  // 0xA6BFF8
  return *(uint16 *)RomPtr_A6(k);
}

void Ridley_Func_54(void) {  // 0xA6C04E
  uint16 ridley_var_F = Get_Ridley(0)->ridley_var_F;
  if (ridley_var_F) {
    switch (ridley_var_F) {
    case 2u:
      Ridley_C08E();
      break;
    case 4u:
      Ridley_C09F();
      break;
    case 6u:
      Ridley_Func_56();
      break;
    case 8u:
      Ridley_Func_57();
      break;
    case 0xAu:
      Ridley_C104();
      break;
    case 0xCu:
      Ridley_Func_58();
      break;
    default:
      Unreachable();
      while (1)
        ;
    }
  } else {
    Ridley_Func_55();
  }
}

void Ridley_Func_55(void) {  // 0xA6C062
  palette_buffer[97] = palette_buffer[1];
  palette_buffer[99] = palette_buffer[3];
  palette_buffer[81] = palette_buffer[17];
  palette_buffer[83] = palette_buffer[19];
  Enemy_Ridley *E = Get_Ridley(0);
  E->ridley_var_E = -15157;
  ++E->ridley_var_F;
  ++E->ridley_var_F;
  Ridley_C08E();
}

void Ridley_C08E(void) {  // 0xA6C08E
  if (ProcessEscapeTimerTileTransfers() & 1) {
    Enemy_Ridley *E = Get_Ridley(0);
    E->ridley_var_E = -15106;
    ++E->ridley_var_F;
    ++E->ridley_var_F;
    Ridley_C09F();
  }
}

void Ridley_C09F(void) {  // 0xA6C09F
  if (ProcessEscapeTimerTileTransfers() & 1) {
    Enemy_Ridley *E = Get_Ridley(0);
    ++E->ridley_var_F;
    ++E->ridley_var_F;
    Ridley_Func_59();
    E->ridley_var_E = 128;
    QueueMusic_Delayed8(7u);
  }
}

void Ridley_Func_56(void) {  // 0xA6C0BB
  Ridley_Func_61();
  Enemy_Ridley *E = Get_Ridley(0);
  if (E->ridley_var_E-- == 1) {
    E->ridley_var_3B = -15280;
    E->ridley_var_3C = 0;
    E->ridley_var_3D = 0;
    E->ridley_var_3E = 0;
    E->ridley_var_3F = 0;
    E->ridley_var_E = 32;
    if (!japanese_text_flag) {
      ++E->ridley_var_F;
      ++E->ridley_var_F;
    }
    ++E->ridley_var_F;
    ++E->ridley_var_F;
  }
}

void Ridley_Func_57(void) {  // 0xA6C0F5
  Enemy_Ridley *E = Get_Ridley(0);
  if (E->ridley_var_E-- == 1) {
    ++E->ridley_var_F;
    ++E->ridley_var_F;
    Ridley_Func_62();
  }
  Ridley_C104();
}

void Ridley_C104(void) {  // 0xA6C104
  Ridley_Func_61();
  if (HandleTypewriterText_Ext(0x3582u) & 1) {
    Enemy_Ridley *E = Get_Ridley(0);
    ++E->ridley_var_F;
    ++E->ridley_var_F;
  }
}

void Ridley_Func_58(void) {  // 0xA6C117
  Ridley_Func_61();
  CeresRidley_Func_2();
  Enemy_Ridley *E = Get_Ridley(0);
  E->ridley_var_F = 0;
  E->ridley_var_A = FUNC16(CeresRidley_Func_26);
  timer_status = 1;
  ceres_status = 2;
  SetBossBitForCurArea(1u);
}

void Ridley_Func_59(void) {  // 0xA6C136
  VramWriteEntry *v2;

  uint16 v0 = vram_write_queue_tail;
  const uint8 *v1 = RomPtr_A6(addr_byte_A6C15D);
  v2 = gVramWriteEntry(vram_write_queue_tail);
  v2->size = GET_WORD(v1);
  *(VoidP *)((char *)&v2->src.addr + 1) = GET_WORD(v1 + 3);
  v2->src.addr = GET_WORD(v1 + 2);
  v2->vram_dst = GET_WORD(v1 + 5);
  vram_write_queue_tail = v0 + 7;
}

void Ridley_Func_60(void) {  // 0xA6C176
  int16 v1;

  Enemy_Ridley *E = Get_Ridley(0);
  v1 = E->ridley_var_39 + 1;
  if (!sign16(E->ridley_var_39 - 31))
    v1 = -31;
  E->ridley_var_39 = v1;
  palette_buffer[11] = abs16(E->ridley_var_39) & 0x1F;
}

void Ridley_Func_61(void) {  // 0xA6C19C
  if (!(door_transition_flag_enemies | palette_change_num) && (nmi_frame_counter_word & 3) == 0) {
    Enemy_Ridley *E = Get_Ridley(0);
    uint16 v1 = E->ridley_var_38 + 1;
    if (v1 >= 0x10u)
      v1 = 0;
    E->ridley_var_38 = v1;
    int v2 = (uint16)(2 * (E->ridley_var_38 + 2 * v1)) >> 1;
    palette_buffer[97] = g_word_A6C1DF[v2];
    palette_buffer[98] = g_word_A6C1DF[v2 + 1];
    palette_buffer[99] = g_word_A6C1DF[v2 + 2];
  }
}

void SetupZebesEscapeTypewriter(void) {  // 0xA6C23F
  palette_buffer[157] = palette_buffer[125];
  palette_buffer[158] = palette_buffer[126];
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->mbn_var_3B = addr_loc_A9C49C;
  E->mbn_var_3C = 0;
  E->mbn_var_3D = 0;
  E->mbn_var_3E = 0;
  E->mbn_var_3F = 0;
  reg_BG2HOFS = 0;
  reg_BG2VOFS = 0;
}

uint8 ProcessEscapeTimerTileTransfers(void) {  // 0xA6C26E
  VramWriteEntry *v4;

  EnemyData *v0 = gEnemyData(0);
  uint16 ai_var_E = v0->ai_var_E;
  uint16 v2 = vram_write_queue_tail;
  const uint8 *v3 = RomPtr_A6(ai_var_E);
  uint8 result = 1;
  if (GET_WORD(v3)) {
    v4 = gVramWriteEntry(vram_write_queue_tail);
    v4->size = GET_WORD(v3);
    *(VoidP *)((char *)&v4->src.addr + 1) = GET_WORD(v3 + 3);
    v4->src.addr = GET_WORD(v3 + 2);
    v4->vram_dst = GET_WORD(v3 + 5);
    vram_write_queue_tail = v2 + 7;
    v0->ai_var_E = ai_var_E + 7;
    if (*(uint16 *)RomPtr_A6(v0->ai_var_E))
      return 0;
  }
  return result;
}

uint8 HandleTypewriterText_Ext(uint16 a) {  // 0xA6C2A7
  R18_ = a;
  return HandleTypewriterText() & 1;
}

uint8 HandleTypewriterText(void) {  // 0xA6C2B1
  int16 v4;
  VramWriteEntry *v12;

  Enemy_MotherBrain *E = Get_MotherBrain(0);
  uint16 mbn_var_3D = E->mbn_var_3D;
  if (mbn_var_3D) {
    E->mbn_var_3D = mbn_var_3D - 1;
    return 0;
  } else {
    E->mbn_var_3D = E->mbn_var_3E;
    int i, v7;
    for (i = E->mbn_var_3B; ; i = v7 + 2) {
      while (1) {
        v4 = *(uint16 *)RomPtr_A6(i);
        if (!v4)
          return 1;
        if (v4 != 1)
          break;
        uint16 v5 = i + 2;
        uint16 v6 = *(uint16 *)RomPtr_A6(v5);
        E->mbn_var_3E = v6;
        i = v5 + 2;
      }
      if (v4 != 13)
        break;
      v7 = i + 2;
      uint16 v8 = *(uint16 *)RomPtr_A6(v7);
      E->mbn_var_3C = v8;
    }
    v4 = (uint8)v4;
    if ((uint8)v4 == 32) {
      ++E->mbn_var_3C;
      E->mbn_var_3B = i + 1;
      return 0;
    } else {
      if ((uint8)v4 == 33)
        v4 = 91;
      E->mbn_var_3B = i + 1;
      uint16 v11 = vram_write_queue_tail;
      v12 = gVramWriteEntry(vram_write_queue_tail);
      v12->size = 2;
      *(VoidP *)((char *)&v12->src.addr + 1) = 32256;
      E->mbn_var_3A = R18_ + v4 - 65;
      v12->src.addr = ADDR16_OF_RAM(*extra_enemy_ram8000) + 52;
      uint16 mbn_var_3C = E->mbn_var_3C;
      v12->vram_dst = mbn_var_3C;
      E->mbn_var_3C = mbn_var_3C + 1;
      vram_write_queue_tail = v11 + 7;
      uint16 v14 = E->mbn_var_3F + 1;
      E->mbn_var_3F = v14;
      if (!sign16(v14 - 2)) {
        E->mbn_var_3F = 0;
        if (area_index == 6)
          QueueSfx2_Max3(0x45u);
        else
          QueueSfx3_Max3(0xDu);
      }
      return 0;
    }
  }
}

void Ridley_Func_62(void) {  // 0xA6C383
  int i;
  VramWriteEntry *v3;

  uint16 v0 = addr_stru_A6C3B8;
  for (i = vram_write_queue_tail; ; i += 7) {
    const uint8 *v2 = RomPtr_A6(v0);
    if (!GET_WORD(v2))
      break;
    v3 = gVramWriteEntry(i);
    v3->size = GET_WORD(v2);
    *(VoidP *)((char *)&v3->src.addr + 1) = GET_WORD(v2 + 3);
    v3->src.addr = GET_WORD(v2 + 2);
    v3->vram_dst = GET_WORD(v2 + 5);
    v0 += 7;
  }
  vram_write_queue_tail = i;
}

void Ridley_Func_63(void) {  // 0xA6C538
  if (!(Ridley_Func_68() & 1))
    Ridley_C53E();
}

void Ridley_C53E(void) {  // 0xA6C53E
  Ridley_Func_99(addr_kRidley_Ilist_E6C8);
  Enemy_Ridley *E = Get_Ridley(0);
  E->ridley_var_A = FUNC16(Ridley_C551);
  E->ridley_var_F = 32;
}

void Ridley_C551(void) {  // 0xA6C551
  Ridley_Func_68();
  Enemy_Ridley *E = Get_Ridley(0);
  if ((--E->ridley_var_F & 0x8000u) != 0) {
    E->ridley_var_B = 0;
    E->ridley_var_C = 0;
    fx_target_y_pos = 528;
    fx_y_vel = 64;
    fx_timer = 1;
    E->ridley_var_27 = 0;
    E->ridley_var_28 = 0;
    E->ridley_var_A = FUNC16(Ridley_Func_64);
    E->ridley_var_F = 160;
    Ridley_Func_64();
  }
}

void Ridley_Func_64(void) {  // 0xA6C588
  Ridley_Func_69();
  Enemy_Ridley *E = Get_Ridley(0);
  if ((--E->ridley_var_F & 0x8000u) != 0) {
    if (E->ridley_var_1B)
      Ridley_Func_38();
    E->ridley_var_A = FUNC16(Ridley_Func_65);
    E->ridley_var_B = 0;
    E->ridley_var_C = 0;
    RidleysExplosion_Func_2();
  }
}

void Ridley_Func_65(void) {  // 0xA6C5A8
  Ridley_Func_69();
  Enemy_Ridley *E = Get_Ridley(0);
  E->ridley_var_02 = 0;
  E->base.properties |= kEnemyProps_Invisible;
  E->ridley_var_A = FUNC16(Ridley_Func_66);
  E->ridley_var_F = 32;
}

void Ridley_Func_66(void) {  // 0xA6C5C8
  Enemy_Ridley *E = Get_Ridley(0);
  if ((--E->ridley_var_F & 0x8000u) != 0) {
    E->ridley_var_A = FUNC16(Ridley_Func_67);
    E->ridley_var_F = 256;
  }
}

void Ridley_Func_67(void) {  // 0xA6C5DA
  Enemy_Ridley *E = Get_Ridley(0);
  if ((--E->ridley_var_F & 0x8000u) != 0) {
    uint16 k = 0;
    printf("Warning: X undefined\n");
    SetBossBitForCurArea(1u);
    Enemy_ItemDrop_Ridley(k);
    QueueMusic_Delayed8(3u);
    E->base.properties |= kEnemyProps_Deleted;
    E->ridley_var_A = addr_locret_A6C600;
  }
}

uint8 Ridley_Func_68(void) {  // 0xA6C601
  R18_ = 128;
  R20_ = 328;
  Ridley_Func_104(0, 0, 0x10);
  R22_ = 4;
  R24_ = 4;
  return Shitroid_Func_2(0) & 1;
}

void Ridley_Func_69(void) {  // 0xA6C623
  int16 v1;

  Enemy_Ridley *E = Get_Ridley(0);
  v1 = E->ridley_var_27 - 1;
  if (v1 < 0) {
    E->ridley_var_27 = 4;
    uint16 v2 = E->ridley_var_28 + 1;
    if (!sign16(E->ridley_var_28 - 9))
      v2 = 0;
    E->ridley_var_28 = v2;
    int v3 = (uint16)(4 * v2) >> 1;
    R18_ = E->base.x_pos + g_word_A6C66E[v3];
    R20_ = E->base.y_pos + g_word_A6C66E[v3 + 1];
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 3u);
    QueueSfx2_Max3(0x24u);
  } else {
    E->ridley_var_27 = v1;
  }
}

void RidleysExplosion_Init(void) {  // 0xA6C696
  Enemy_Ridley *E = Get_Ridley(cur_enemy_index);
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->base.vram_tiles_index = 0;
  E->base.palette_index = 3584;
  uint16 ridley_parameter_1 = E->ridley_parameter_1;
  E->ridley_var_F = g_word_A6C6CE[ridley_parameter_1 >> 1];
  uint16 Random = NextRandom();
  E->ridley_var_B = sign16(Random) ? -(Random & 0x130) : (Random & 0x130);
  E->ridley_var_C = 0;
  if (ridley_parameter_1) {
    switch (ridley_parameter_1) {
    case 2u:
      E->base.x_pos = tilemap_stuff[32];
      E->base.y_pos = tilemap_stuff[33];
      E->base.current_instruction = addr_kRidleysExplosion_Ilist_CA47;
      break;
    case 4u:
      E->base.x_pos = tilemap_stuff[42];
      E->base.y_pos = tilemap_stuff[43];
      E->base.current_instruction = addr_kRidleysExplosion_Ilist_CA4D;
      break;
    case 6u:
      E->base.x_pos = tilemap_stuff[52];
      E->base.y_pos = tilemap_stuff[53];
      E->base.current_instruction = addr_kRidleysExplosion_Ilist_CA4D;
      break;
    case 8u:
      E->base.x_pos = tilemap_stuff[62];
      E->base.y_pos = tilemap_stuff[63];
      E->base.current_instruction = addr_kRidleysExplosion_Ilist_CA53;
      break;
    case 0xAu:
      E->base.x_pos = tilemap_stuff[72];
      E->base.y_pos = tilemap_stuff[73];
      E->base.current_instruction = addr_kRidleysExplosion_Ilist_CA53;
      break;
    case 0xCu:
      E->base.x_pos = tilemap_stuff[82];
      E->base.y_pos = tilemap_stuff[83];
      E->base.current_instruction = g_off_A6C7BA[(uint16)((uint8)((LOBYTE(tilemap_stuff[71])
                                                                   + LOBYTE(tilemap_stuff[81])
                                                                   + 8) & 0xF0) >> 3) >> 1];
      break;
    case 0xEu: {
      uint16 v10 = 0;
      Enemy_Ridley *E0 = Get_Ridley(0);
      if (E0->ridley_var_10)
        v10 = 2;
      int v13 = v10 >> 1;
      uint16 v14 = E0->base.x_pos + g_word_A6C804[v13];
      E->base.x_pos = v14;
      E->base.y_pos = E0->base.y_pos;
      E->base.current_instruction = g_off_A6C808[v13];
      break;
    }
    case 0x10u: {
      uint16 v16 = 0;
      Enemy_Ridley *E0 = Get_Ridley(0);
      if (E0->ridley_var_10)
        v16 = 2;
      int v19 = v16 >> 1;
      uint16 v20 = E0->base.x_pos + g_word_A6C836[v19];
      E->base.x_pos = v20;
      E->base.y_pos = E0->base.y_pos + 22;
      E->base.current_instruction = g_off_A6C83A[v19];
      break;
    }
    case 0x12u: {
      uint16 v22 = 0;
      Enemy_Ridley *E0 = Get_Ridley(0);
      if (E0->ridley_var_10)
        v22 = 2;
      int v25 = v22 >> 1;
      uint16 v26 = E0->base.x_pos + g_word_A6C868[v25];
      E->base.x_pos = v26;
      E->base.y_pos = E0->base.y_pos - 24;
      E->base.current_instruction = g_off_A6C86C[v25];
      break;
    }
    case 0x14u: {
      uint16 v28 = 0;
      Enemy_Ridley *E0 = Get_Ridley(0);
      if (E0->ridley_var_10)
        v28 = 2;
      int v31 = v28 >> 1;
      uint16 v32 = E0->base.x_pos + g_word_A6C89A[v31];
      E->base.x_pos = v32;
      E->base.y_pos = E0->base.y_pos;
      E->base.current_instruction = g_off_A6C89E[v31];
      break;
    }
    case 0x16u: {
      uint16 v34 = 0;
      Enemy_Ridley *E0 = Get_Ridley(0);
      if (E0->ridley_var_10)
        v34 = 2;
      int v37 = v34 >> 1;
      uint16 v38 = E0->base.x_pos + g_word_A6C8CC[v37];
      E->base.x_pos = v38;
      E->base.y_pos = E0->base.y_pos + 7;
      E->base.current_instruction = g_off_A6C8D0[v37];
      break;
    }
    default:
      Unreachable();
      while (1)
        ;
    }
  } else {
    E->base.x_pos = tilemap_stuff[22];
    E->base.y_pos = tilemap_stuff[23];
    E->base.current_instruction = addr_kRidleysExplosion_Ilist_CA47;
  }
}

void RidleysExplosion_Main(void) {  // 0xA6C8D4
  int16 v2;

  RidleysExplosion_Func_1(cur_enemy_index);
  Enemy_RidleysExplosion *E = Get_RidleysExplosion(cur_enemy_index);
  v2 = abs16(E->ren_var_B) - 4;
  if (v2 < 0)
    v2 = 0;
  E->ren_var_B = sign16(E->ren_var_B) ? -v2 : v2;
  E->ren_var_C += 4;
  MoveEnemyWithVelocity();
  if ((--E->ren_var_F & 0x8000u) != 0)
    EnemyDeathAnimation(cur_enemy_index, 0);
}

void RidleysExplosion_Func_1(uint16 k) {  // 0xA6C913
  Enemy_RidleysExplosion *E = Get_RidleysExplosion(k);
  uint16 v2 = E->base.frame_counter & 1, v3;
  E->base.frame_counter = v2;
  if (v2)
    v3 = E->base.properties | kEnemyProps_Invisible;
  else
    v3 = E->base.properties & ~kEnemyProps_Invisible;
  E->base.properties = v3;
}

void RidleysExplosion_Func_2(void) {  // 0xA6C932
  SpawnEnemy(0xA6, addr_stru_A6C9E7);
  SpawnEnemy(0xA6, addr_stru_A6C9D7);
  SpawnEnemy(0xA6, addr_stru_A6C9C7);
  SpawnEnemy(0xA6, addr_stru_A6C9B7);
  SpawnEnemy(0xA6, addr_stru_A6C9A7);
  SpawnEnemy(0xA6, addr_stru_A6C997);
  SpawnEnemy(0xA6, addr_stru_A6C987);
  SpawnEnemy(0xA6, addr_stru_A6C9F7);
  SpawnEnemy(0xA6, addr_stru_A6CA07);
  SpawnEnemy(0xA6, addr_stru_A6CA27);
  SpawnEnemy(0xA6, addr_stru_A6CA17);
  SpawnEnemy(0xA6, addr_stru_A6CA37);
}

void locret_A6CB20(void) {}
static Func_V *const off_A6CB21[9] = { locret_A6CB20, Ridley_CBC0, Ridley_Func_71, Ridley_Func_72, Ridley_CBC7, Ridley_CBCE, Ridley_Func_73, Ridley_Func_74, Ridley_CBD5 };

void Ridley_Func_70(void) {  // 0xA6CAF5
  Ridley_Func_91();
  off_A6CB21[tilemap_stuff[0]]();
  Ridley_Func_86();
  Ridley_Func_87();
  Enemy_Ridley *E = Get_Ridley(0);
  if (!(E->ridley_var_1B | (uint16)(samus_invincibility_timer | E->base.properties & kEnemyProps_Tangible)))
    Ridley_Func_127();
}

void Ridley_Func_71(void) {  // 0xA6CB33
  tilemap_stuff[10] = 8;
  Ridley_Func_75();
  tilemap_stuff[0] = 3;
}

void Ridley_Func_72(void) {  // 0xA6CB45
  tilemap_stuff[10] = 8;
  Ridley_Func_75();
}

void Ridley_Func_73(void) {  // 0xA6CB4E
  tilemap_stuff[10] = 3;
  Ridley_Func_75();
  tilemap_stuff[0] = 6;
}

void Ridley_Func_74(void) {  // 0xA6CB60
  tilemap_stuff[10] = 2;
  Ridley_Func_75();
  tilemap_stuff[0] = 7;
}

void Ridley_Func_75(void) {  // 0xA6CB72
  if (Get_Ridley(0)->ridley_var_10) {
    tilemap_stuff[13] = 0x4000;
    tilemap_stuff[14] = -1;
  } else {
    tilemap_stuff[14] = 0x4000;
    tilemap_stuff[13] = -1;
  }
  Ridley_CBC7();
  if (!(tilemap_stuff[76] | (uint16)(tilemap_stuff[66] | tilemap_stuff[56] | tilemap_stuff[46] | tilemap_stuff[36] | tilemap_stuff[26] | tilemap_stuff[16])))
    tilemap_stuff[0] = 4;
}

void Ridley_CBC0(void) {  // 0xA6CBC0
  R18_ = FUNC16(Ridley_Func_79);
  Ridley_Func_79();
  Ridley_CBDC();
}

void Ridley_CBC7(void) {  // 0xA6CBC7
  R18_ = FUNC16(Ridley_Func_83);
  Ridley_Func_83();
  Ridley_CBDC();
}

void Ridley_CBCE(void) {  // 0xA6CBCE
  R18_ = FUNC16(Ridley_Func_84);
  Ridley_Func_84();
  Ridley_CBDC();
}

void Ridley_CBD5(void) {  // 0xA6CBD5
  R18_ = FUNC16(Ridley_Func_81);
  Ridley_Func_81();
  Ridley_CBDC();
}

void Ridley_CBDC(void) {  // 0xA6CBDC
  int16 v0;
  int16 v2;

  v0 = 7;
  uint16 v1 = 0;
  do {
    v2 = v0;
    Ridley_Func_88(v1);
    v1 += 20;
    v0 = v2 - 1;
  } while (v2 != 1);
}

void Ridley_Func_77(void) {  // 0xA6CBFE
  int v0 = (uint16)(2 * Get_Ridley(0)->ridley_var_10) >> 1;
  tilemap_stuff[11] = g_word_A6CC12[v0];
  tilemap_stuff[12] = g_word_A6CC18[v0];
}

uint8 Ridley_Func_78(void) {  // 0xA6CC1E
  return (tilemap_stuff[76] & (uint16)(tilemap_stuff[66] & tilemap_stuff[56] & tilemap_stuff[46] & tilemap_stuff[36] & tilemap_stuff[26] & tilemap_stuff[16])) != 0;
}

void Ridley_Func_79(void) {  // 0xA6CC39
  Ridley_Func_77();
  if (Ridley_Func_78()) {
    if (Ridley_Func_80_DoubleRet())
      return;
  }
  if (!(tilemap_stuff[76] | (uint16)(tilemap_stuff[66] | tilemap_stuff[56] | tilemap_stuff[46] | tilemap_stuff[36] | tilemap_stuff[26] | tilemap_stuff[16]))) {
    tilemap_stuff[16] = FUNC16(Enemy_GrappleReact_NoInteract_A6);
    tilemap_stuff[13] = -1;
    tilemap_stuff[14] = -1;
    uint16 v0 = 2;
    if (samus_x_pos < 0x70u)
      v0 = 1;
    tilemap_stuff[10] = v0;
  }
}

uint8 Ridley_Func_80_DoubleRet(void) {  // 0xA6CC7D
  Enemy_Ridley *E = Get_Ridley(0);
  bool retval = false;
  uint16 v1;
  if (tilemap_stuff[2] && (tilemap_stuff[13] & tilemap_stuff[14] & 0x8000u) != 0)
    goto LABEL_7;
  if (!tilemap_stuff[1])
    goto LABEL_9;
  if ((uint8)random_number >= 0xF0u) {
LABEL_7:
    v1 = tilemap_stuff[2] - 1;
    goto LABEL_8;
  }
  if (abs16(samus_x_pos - E->base.x_pos) < 0x80) {
    v1 = 0;
LABEL_8:
    Ridley_Func_89(v1);
    retval = true;
  }
LABEL_9:
  tilemap_stuff[2] = 0;
  return retval;
}

bool Ridley_Func_82_DoubleRet();

void Ridley_Func_81(void) {  // 0xA6CCBD
  Ridley_Func_77();
  if (Ridley_Func_78() & 1) {
    if (Ridley_Func_82_DoubleRet())
      return;
  }
  if (!(tilemap_stuff[76] | (uint16)(tilemap_stuff[66] | tilemap_stuff[56] | tilemap_stuff[46] | tilemap_stuff[36] | tilemap_stuff[26] | tilemap_stuff[16]))) {
    tilemap_stuff[16] = FUNC16(Enemy_GrappleReact_NoInteract_A6);
    tilemap_stuff[13] = -1;
    tilemap_stuff[14] = -1;
    uint16 v0 = 2;
    if (samus_x_pos < 0x70u)
      v0 = 1;
    tilemap_stuff[10] = v0;
  }
}

bool Ridley_Func_82_DoubleRet(void) {
  if (tilemap_stuff[2]) {
    if ((tilemap_stuff[13] & tilemap_stuff[14] & 0x8000u) != 0) {
      Ridley_Func_89(1u);
      tilemap_stuff[2] = 0;
      return true;
    }
  }
  return false;
}

void Ridley_Func_83(void) {  // 0xA6CD24
  Enemy_Ridley *E = Get_Ridley(0);
  Ridley_Func_77();
  if (Ridley_Func_78() & 1
      && ((uint8)random_number >= 0xF0u
          || (abs16(samus_x_pos - E->base.x_pos) < 0x80))
      && (tilemap_stuff[13] & tilemap_stuff[14] & 0x8000u) != 0) {
    tilemap_stuff[13] = 16128;
    tilemap_stuff[10] = 8;
  } else if (!(tilemap_stuff[76] | (uint16)(tilemap_stuff[66] | tilemap_stuff[56] | tilemap_stuff[46] | tilemap_stuff[36] | tilemap_stuff[26] | tilemap_stuff[16]))) {
    if ((Get_Ridley(0)->ridley_var_C & 0x8000u) == 0)
      tilemap_stuff[0] = 5;
    tilemap_stuff[13] = -1;
    tilemap_stuff[14] = -1;
    uint16 v1 = tilemap_stuff[8];
    if (!tilemap_stuff[8] || (v1 = tilemap_stuff[8] - 1, (tilemap_stuff[8] = v1) != 0)) {
      tilemap_stuff[10] = v1;
    } else {
      tilemap_stuff[16] = FUNC16(Enemy_GrappleReact_NoInteract_A6);
      Ridley_Func_85();
      tilemap_stuff[10] = 8;
    }
  }
}

void Ridley_Func_84(void) {  // 0xA6CDAA
  Enemy_Ridley *E = Get_Ridley(0);
  Ridley_Func_77();
  if (Ridley_Func_78() & 1
      && ((uint8)random_number >= 0xF0u
          || (abs16(samus_x_pos - E->base.x_pos) < 0x80))
      && (tilemap_stuff[13] & tilemap_stuff[14] & 0x8000u) != 0) {
    tilemap_stuff[13] = 16128;
    tilemap_stuff[10] = 8;
  } else if (!(tilemap_stuff[76] | (uint16)(tilemap_stuff[66] | tilemap_stuff[56] | tilemap_stuff[46] | tilemap_stuff[36] | tilemap_stuff[26] | tilemap_stuff[16]))) {
    if ((E->ridley_var_C & 0x8000u) == 0) {
      tilemap_stuff[0] = 6;
      tilemap_stuff[20] = 2560;
      tilemap_stuff[30] = 2560;
      tilemap_stuff[40] = 2560;
      tilemap_stuff[50] = 2560;
      tilemap_stuff[60] = 2560;
      tilemap_stuff[70] = 2560;
      tilemap_stuff[80] = 2560;
      tilemap_stuff[21] = 0x4000;
      tilemap_stuff[31] = 0x4000;
      tilemap_stuff[41] = 0x4000;
      tilemap_stuff[51] = 0x4000;
      tilemap_stuff[61] = 0x4000;
      tilemap_stuff[71] = 0x4000;
      tilemap_stuff[81] = 0x4000;
      tilemap_stuff[16] = FUNC16(Enemy_GrappleReact_NoInteract_A6);
    }
    tilemap_stuff[13] = -1;
    tilemap_stuff[14] = -1;
    uint16 v1 = tilemap_stuff[8];
    if (tilemap_stuff[8]) {
      v1 = tilemap_stuff[8] - 1;
      tilemap_stuff[8] = v1;
      if (!v1) {
        tilemap_stuff[16] = FUNC16(Enemy_GrappleReact_NoInteract_A6);
        Ridley_Func_85();
        v1 = 8;
      }
    }
    tilemap_stuff[10] = v1;
  }
}

void Ridley_Func_85(void) {  // 0xA6CE65
  tilemap_stuff[21] = LOBYTE(tilemap_stuff[21]) | 0x4000;
  tilemap_stuff[31] = LOBYTE(tilemap_stuff[31]) | 0x4000;
  tilemap_stuff[41] = LOBYTE(tilemap_stuff[41]) | 0x4000;
  tilemap_stuff[51] = LOBYTE(tilemap_stuff[51]) | 0x4000;
  tilemap_stuff[61] = tilemap_stuff[21] & 0x4FF | 0x4000;
  tilemap_stuff[71] = LOBYTE(tilemap_stuff[71]) | 0x4000;
  tilemap_stuff[81] = LOBYTE(tilemap_stuff[81]) | 0x4000;
}

void Ridley_Func_86(void) {  // 0xA6CEBA
  int16 v1;

  Enemy_Ridley *E = Get_Ridley(0);
  tilemap_stuff[23] = tilemap_stuff[25] + E->base.y_pos + 16;
  tilemap_stuff[33] = tilemap_stuff[35] + tilemap_stuff[23];
  tilemap_stuff[43] = tilemap_stuff[45] + tilemap_stuff[35] + tilemap_stuff[23];
  tilemap_stuff[53] = tilemap_stuff[55] + tilemap_stuff[43];
  tilemap_stuff[63] = tilemap_stuff[65] + tilemap_stuff[55] + tilemap_stuff[43];
  tilemap_stuff[73] = tilemap_stuff[75] + tilemap_stuff[63];
  tilemap_stuff[83] = tilemap_stuff[85] + tilemap_stuff[75] + tilemap_stuff[63];
  v1 = E->ridley_var_10 - 1;
  if (E->ridley_var_10 != 1)
    v1 = tilemap_stuff[24] + g_word_A6CF54[E->ridley_var_10];
  tilemap_stuff[22] = E->base.x_pos + v1;
  if (E->ridley_var_10 == 1) {
    tilemap_stuff[32] = E->base.x_pos;
    tilemap_stuff[42] = tilemap_stuff[32];
    tilemap_stuff[52] = tilemap_stuff[32];
    tilemap_stuff[62] = tilemap_stuff[32];
    tilemap_stuff[72] = tilemap_stuff[32];
    tilemap_stuff[82] = tilemap_stuff[32];
  } else {
    tilemap_stuff[32] = tilemap_stuff[34] + tilemap_stuff[22];
    tilemap_stuff[42] = tilemap_stuff[44] + tilemap_stuff[34] + tilemap_stuff[22];
    tilemap_stuff[52] = tilemap_stuff[54] + tilemap_stuff[42];
    tilemap_stuff[62] = tilemap_stuff[64] + tilemap_stuff[54] + tilemap_stuff[42];
    tilemap_stuff[72] = tilemap_stuff[74] + tilemap_stuff[62];
    tilemap_stuff[82] = tilemap_stuff[84] + tilemap_stuff[74] + tilemap_stuff[62];
  }
}

void Ridley_Func_87(void) {  // 0xA6CF5A
  if (tilemap_stuff[30]) {
    if ((int16)(tilemap_stuff[30] - tilemap_stuff[29]) < 0)
      tilemap_stuff[30] = 0;
    uint16 v0 = tilemap_stuff[9] + tilemap_stuff[29];
    if (!sign16(tilemap_stuff[9] + tilemap_stuff[29] - 6144))
      v0 = 6144;
    tilemap_stuff[29] = v0;
  } else if ((int16)(2048 - tilemap_stuff[29]) < 0) {
    tilemap_stuff[29] -= 128;
  }
  if (tilemap_stuff[40]) {
    if ((int16)(tilemap_stuff[40] - tilemap_stuff[39]) < 0)
      tilemap_stuff[40] = 0;
    uint16 v1 = tilemap_stuff[9] + tilemap_stuff[39];
    if (!sign16(tilemap_stuff[9] + tilemap_stuff[39] - 6144))
      v1 = 6144;
    tilemap_stuff[39] = v1;
  } else if ((int16)(2048 - tilemap_stuff[39]) < 0) {
    tilemap_stuff[39] -= 128;
  }
  if (tilemap_stuff[50]) {
    if ((int16)(tilemap_stuff[50] - tilemap_stuff[49]) < 0)
      tilemap_stuff[50] = 0;
    uint16 v2 = tilemap_stuff[9] + tilemap_stuff[49];
    if (!sign16(tilemap_stuff[9] + tilemap_stuff[49] - 5632))
      v2 = 5632;
    tilemap_stuff[49] = v2;
  } else if ((int16)(2048 - tilemap_stuff[49]) < 0) {
    tilemap_stuff[49] -= 128;
  }
  if (tilemap_stuff[60]) {
    if ((int16)(tilemap_stuff[60] - tilemap_stuff[59]) < 0)
      tilemap_stuff[60] = 0;
    uint16 v3 = tilemap_stuff[9] + tilemap_stuff[59];
    if (!sign16(tilemap_stuff[9] + tilemap_stuff[59] - 5632))
      v3 = 5632;
    tilemap_stuff[59] = v3;
  } else if ((int16)(2048 - tilemap_stuff[59]) < 0) {
    tilemap_stuff[59] -= 128;
  }
  if (tilemap_stuff[70]) {
    if ((int16)(tilemap_stuff[70] - tilemap_stuff[69]) < 0)
      tilemap_stuff[70] = 0;
    uint16 v4 = tilemap_stuff[9] + tilemap_stuff[69];
    if (!sign16(tilemap_stuff[9] + tilemap_stuff[69] - 4608))
      v4 = 4608;
    tilemap_stuff[69] = v4;
  } else if ((int16)(2048 - tilemap_stuff[69]) < 0) {
    tilemap_stuff[69] -= 128;
  }
  if (tilemap_stuff[80]) {
    if ((int16)(tilemap_stuff[80] - tilemap_stuff[79]) < 0)
      tilemap_stuff[80] = 0;
    uint16 v5 = tilemap_stuff[9] + tilemap_stuff[79];
    if (!sign16(tilemap_stuff[9] + tilemap_stuff[79] - 1280))
      v5 = 1280;
    tilemap_stuff[79] = v5;
  } else if ((int16)(1280 - tilemap_stuff[79]) < 0) {
    tilemap_stuff[79] -= 128;
  }
}

void Ridley_Func_88(uint16 k) {  // 0xA6D09F
  int16 v6;
  uint16 v2;
  int v1 = k >> 1;
  if ((tilemap_stuff[v1 + 16] & 0x8000u) != 0) {
    int v4 = k >> 1;
    uint16 v5 = tilemap_stuff[v4 + 17];
    if (v5 < tilemap_stuff[15]) {
      tilemap_stuff[(k >> 1) + 17] += tilemap_stuff[10];
      return;
    }
    if (v5 != 0xFFFF) {
      tilemap_stuff[v4 + 17] = -1;
      tilemap_stuff[v4 + 26] = FUNC16(Enemy_GrappleReact_NoInteract_A6);
      tilemap_stuff[v4 + 28] = tilemap_stuff[v4 + 18];
    }
    if ((tilemap_stuff[v4 + 18] & 0x8000u) == 0) {
      if ((tilemap_stuff[14] & 0x8000u) != 0) {
        v2 = tilemap_stuff[10] + tilemap_stuff[v4 + 21];
        if ((int16)(v2 - tilemap_stuff[12]) >= 0) {
LABEL_15:
          tilemap_stuff[v4 + 18] = FUNC16(Enemy_GrappleReact_NoInteract_A6);
          v2 = tilemap_stuff[12];
        }
      } else {
        tilemap_stuff[v4 + 20] = 3072;
        v2 = tilemap_stuff[10] + tilemap_stuff[v4 + 21];
        if ((int16)(v2 - tilemap_stuff[14]) >= 0) {
          if (!k || !tilemap_stuff[v4 + 6]) {
            tilemap_stuff[v4 + 21] = tilemap_stuff[14];
LABEL_3:;
            int v3 = k >> 1;
            tilemap_stuff[v3 + 16] = 0;
            tilemap_stuff[v3 + 17] = 0;
            tilemap_stuff[v3 + 18] ^= 0x8000u;
            return;
          }
          goto LABEL_15;
        }
      }
LABEL_24:
      tilemap_stuff[v4 + 21] = v2;
      goto LABEL_25;
    }
    if ((tilemap_stuff[13] & 0x8000u) != 0) {
      v6 = tilemap_stuff[v4 + 21] - tilemap_stuff[10] - 1;
      if ((int16)(v6 - tilemap_stuff[11]) < 0) {
LABEL_22:
        tilemap_stuff[v4 + 18] = 0;
        v2 = tilemap_stuff[11];
        goto LABEL_24;
      }
    } else {
      tilemap_stuff[v4 + 20] = 3072;
      v6 = tilemap_stuff[v4 + 21] - tilemap_stuff[10] - 1;
      if ((int16)(v6 - tilemap_stuff[13]) < 0) {
        if (!k || !tilemap_stuff[v4 + 6]) {
          tilemap_stuff[v4 + 21] = tilemap_stuff[13];
          goto LABEL_3;
        }
        goto LABEL_22;
      }
    }
    v2 = v6 + 1;
    goto LABEL_24;
  }
  v2 = tilemap_stuff[v1 + 21];
LABEL_25:
  R18_ = (uint8)v2;
  if (k)
    R18_ = (uint8)(LOBYTE(tilemap_stuff[(k >> 1) + 11]) + R18_);
  int v7 = k >> 1;
  tilemap_stuff[v7 + 24] = ComputeSinMult(HIBYTE(tilemap_stuff[v7 + 19]));
  tilemap_stuff[v7 + 25] = ComputeCosMult(HIBYTE(tilemap_stuff[v7 + 19]));
}

void Ridley_Func_89(uint16 a) {  // 0xA6D19D
  uint16 v1;
  char v2; // t0
  char v4; // cf

  v2 = a;
  LOBYTE(v1) = 0;
  HIBYTE(v1) = v2;
  tilemap_stuff[5] = v1;
  Enemy_Ridley *E = Get_Ridley(0);
  if (E->ridley_var_10 != 1 && E->ridley_var_01) {
    v4 = Ridley_Func_90() & 1;
    if (v4) {
      R18_ -= !v4 + tilemap_stuff[22];
      R20_ -= tilemap_stuff[23];
    } else {
      R18_ = samus_x_pos - tilemap_stuff[22];
      R20_ = samus_y_pos + 24 - tilemap_stuff[23];
    }
    R18_ = (uint8)-(int8)(CalculateAngleFromXY() + 0x80);
    if (E->ridley_var_10) {
      uint16 v7 = R18_;
      if (R18_ >= 0x18u && R18_ < 0xE8u)
        v7 = 24;
      uint16 v8 = tilemap_stuff[5] + v7 + 0x4000;
      if (v8 >= tilemap_stuff[21]) {
        tilemap_stuff[14] = v8;
        tilemap_stuff[10] = 8;
      }
    } else {
      uint16 v5 = R18_;
      if (R18_ >= 0x18u && R18_ < 0xE8u)
        v5 = 232;
      uint16 v6 = v5 + 16128 - tilemap_stuff[5];
      if (v6 < tilemap_stuff[21]) {
        tilemap_stuff[13] = v6;
        tilemap_stuff[10] = 8;
      }
    }
  }
}

uint8 Ridley_Func_90(void) {  // 0xA6D242
  R18_ = tilemap_stuff[82];
  R20_ = tilemap_stuff[83];
  R22_ = 64;
  R24_ = 64;
  if (!projectile_counter)
    return 0;
  uint16 v1 = 0;
  while (1) {
    int v2 = v1 >> 1;
    if ((HIBYTE(projectile_type[v2]) & 0xF) == 1 || (HIBYTE(projectile_type[v2]) & 0xF) == 2) {
      uint16 v3 = abs16(projectile_x_pos[v2] - R18_);
      bool v4 = v3 < projectile_x_radius[v2];
      uint16 v5 = v3 - projectile_x_radius[v2];
      if (v4 || v5 < R22_) {
        uint16 v6 = abs16(projectile_y_pos[v2] - R20_);
        v4 = v6 < projectile_y_radius[v2];
        uint16 v7 = v6 - projectile_y_radius[v2];
        if (v4 || v7 < R24_)
          break;
      }
    }
    v1 += 2;
    if ((int16)(v1 - 10) >= 0)
      return 0;
  }
  int v8 = v1 >> 1;
  R18_ = projectile_x_pos[v8];
  R20_ = projectile_y_pos[v8];
  return 1;
}

void Ridley_Func_91(void) {  // 0xA6D2AA
  Enemy_Ridley *E = Get_Ridley(0);
  if (!sign16(tilemap_stuff[10] - 8)) {
    uint16 v1 = E->ridley_var_0E + 1;
    E->ridley_var_0E = v1;
    if (sign16(v1 - 16))
      return;
    if (!E->ridley_var_0F)
      QueueSfx3_Max6(0x21u);
  }
  E->ridley_var_0E = 0;
}

void Ridley_Func_92(void) {  // 0xA6D2D6
  tilemap_stuff[10] = 1;
  tilemap_stuff[11] = 16368;
  tilemap_stuff[12] = 16448;
  tilemap_stuff[13] = -1;
  tilemap_stuff[14] = -1;
  tilemap_stuff[9] = 240;
  tilemap_stuff[15] = 16;
  tilemap_stuff[17] = 17;
  tilemap_stuff[27] = 17;
  tilemap_stuff[37] = 17;
  tilemap_stuff[47] = 17;
  tilemap_stuff[57] = 17;
  tilemap_stuff[67] = 17;
  tilemap_stuff[77] = 17;
  Ridley_Func_93(0x2024, addr_word_A6D36E);
  Ridley_Func_93(0x2026, addr_word_A6D37C);
  Ridley_Func_93(0x202A, addr_word_A6D38A);
  Ridley_Func_93(0x202C, addr_word_A6D398);
  Ridley_Func_93(0x202E, addr_word_A6D3A6);
  tilemap_stuff[16] = 0;
  tilemap_stuff[26] = 0;
  tilemap_stuff[36] = 0;
  tilemap_stuff[46] = 0;
  tilemap_stuff[56] = 0;
  tilemap_stuff[66] = 0;
  tilemap_stuff[76] = 0;
}

void Ridley_Func_93(uint16 j, uint16 k) {  // 0xA6D3B4
  int16 v3;
  int16 v5;

  R20_ = 126;
  R18_ = j;
  uint16 v2 = 0;
  v3 = 7;
  do {
    v5 = v3;
    uint16 *v4 = (uint16 *)RomPtr_A6(k);
    IndirWriteWord(&R18_, v2, *v4);
    k += 2;
    v2 += 20;
    --v3;
  } while (v5 != 1);
}

void Ridley_Func_94(void) {  // 0xA6D3D4
  Ridley_Func_D3DC(0);
}

void Ridley_Func_95(void) {  // 0xA6D3D9
  Ridley_Func_D3DC(FUNC16(Enemy_GrappleReact_NoInteract_A6));
}

void Ridley_Func_D3DC(uint16 a) {  // 0xA6D3DC
  tilemap_stuff[16] = a;
  tilemap_stuff[26] = a;
  tilemap_stuff[36] = a;
  tilemap_stuff[46] = a;
  tilemap_stuff[56] = a;
  tilemap_stuff[66] = a;
  tilemap_stuff[76] = a;
}

void Ridley_Func_96(void) {  // 0xA6D3F9
  int16 v0;

  Ridley_Func_93(0x2026, addr_word_A6D37C);
  v0 = 7;
  uint16 v1 = 0;
  do {
    int v2 = v1 >> 1;
    tilemap_stuff[v2 + 21] = 0x8000 - tilemap_stuff[v2 + 21];
    tilemap_stuff[v2 + 18] |= 0x8000u;
    v1 += 20;
    --v0;
  } while (v0);
}

uint8 Ridley_Func_97(void) {  // 0xA6D431
  return (tilemap_stuff[76] & (uint16)(tilemap_stuff[66] & tilemap_stuff[56] & tilemap_stuff[46] & tilemap_stuff[36] & tilemap_stuff[26] & tilemap_stuff[16])) != 0;
}

void Ridley_Func_98(void) {  // 0xA6D453
  uint16 enemy_ptr = Get_Ridley(cur_enemy_index)->base.enemy_ptr;
  const uint8 *v1 = RomPtr_A0(enemy_ptr);
  uint16 v2 = SuitDamageDivision(GET_WORD(v1 + 6));
  Samus_DealDamage(v2);
}

void Ridley_Func_99(uint16 a) {  // 0xA6D467
  Enemy_Ridley *E = Get_Ridley(0);
  E->base.current_instruction = a;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
}

void Ridley_Func_100(void) {  // 0xA6D474
  int16 v0;

  v0 = 0;
  Enemy_Ridley *E = Get_Ridley(0);
  uint16 health = E->base.health;
  if (health < 0x2328u) {
    v0 = 1;
    if (health < 0x1518u) {
      v0 = 2;
      if (health < 0x708u)
        v0 = 3;
    }
  }
  E->ridley_var_12 = v0;
  R18_ = v0 - 1;
  if ((int16)(v0 - 1) >= 0)
    Ridley_D495();
}

void Ridley_D495(void) {  // 0xA6D495
  if (!palette_change_num)
    WriteColorsToPalette(0x1E2, 0xa6, 28 * R18_ - 7062, 0xE);
}

void Ridley_Func_101(void) {  // 0xA6D4B5
  Enemy_Ridley *E = Get_Ridley(0);
  if (E->ridley_var_01 && E->ridley_var_0D >= 0x32u) {
    uint16 v1 = 0;
    if (E->ridley_var_0D >= 0x46u)
      v1 = 2;
    R18_ = v1;
    Ridley_D495();
  }
}

void Ridley_Func_102(void) {  // 0xA6D4DA
  int16 v0;

  v0 = 3584;
  Enemy_Ridley *E = Get_Ridley(0);
  if (E->base.flash_timer >= 2u && (((uint8)enemy_damage_routine_exec_count + 1) & 2) != 0)
    v0 = 0;
  E->ridley_var_0C = v0;
}

uint8 Ridley_Func_103(uint16 k, uint16 j) {  // 0xA6D4F9
  uint16 prod = Mult8x8(j >> 4, room_width_in_blocks);
  return (level_data[prod + (k >> 4)] & 0xF000) != 0;
}

void Ridley_Func_104_0(uint16 k, uint16 j) {  // 0xA6D523
  Ridley_Func_104(k, j, 0);
}
void Ridley_Func_104(uint16 k, uint16 j, uint16 a) {  // 0xA6D526
  int16 v4;
  int16 v6;
  int16 v8;
  int16 v9;
  int16 v11;
  int16 ridley_var_C;
  int16 v17;

  R26_ = a;
  R24_ = g_byte_A6D61F[j];
  Ridley_Func_105(k);
  Enemy_Ridley *E = Get_Ridley(k);
  v4 = E->base.y_pos - R20_;
  if (v4) {
    if (v4 >= 0) {
      uint16 RegWord = SnesDivide(v4, R24_);
      if (!RegWord)
        RegWord = 1;
      R22_ = RegWord;
      ridley_var_C = E->ridley_var_C;
      bool v14 = 0;
      if (ridley_var_C >= 0) {
        uint16 v15 = ridley_var_C - R26_;
        v15 -= 8;
        uint16 v16 = R22_;
        ridley_var_C = v15 - v16;
      }
      v17 = ridley_var_C - R22_;
      if (sign16(v17 + 1280))
        v17 = -1280;
      E->ridley_var_C = v17;
    } else {
      uint16 v5 = SnesDivide(R20_ - E->base.y_pos, R24_);
      if (!v5)
        v5 = 1;
      R22_ = v5;
      v6 = E->ridley_var_C;
      bool v7 = 0;
      if (v6 < 0) {
        v8 = R26_ + v6;
        v8 += 8;
        v9 = v8;
        v6 = R22_ + v9;
      }
      v11 = R22_ + v6;
      if (!sign16(v11 - 1280))
        v11 = 1280;
      E->ridley_var_C = v11;
    }
  }
}

void Ridley_Func_105(uint16 k) {  // 0xA6D5A9
  int16 v2;
  int16 v4;
  int16 v6;
  int16 v7;
  int16 v9;
  int16 ridley_var_B;
  int16 v15;

  Enemy_Ridley *E = Get_Ridley(k);
  v2 = E->base.x_pos - R18_;
  if (v2) {
    if (v2 >= 0) {
      uint16 RegWord = SnesDivide(v2, R24_);
      if (!RegWord)
        RegWord = 1;
      R22_ = RegWord;
      ridley_var_B = E->ridley_var_B;
      bool v12 = 0;
      if (ridley_var_B >= 0) {
        uint16 v13 = ridley_var_B - R26_;
        v13 -= 8;
        uint16 v14 = R22_;
        ridley_var_B = v13 - v14;
      }
      v15 = ridley_var_B - R22_;
      if (sign16(v15 + 1280))
        v15 = -1280;
      E->ridley_var_B = v15;
    } else {
      uint16 v3 = SnesDivide(R18_ - E->base.x_pos, R24_);
      if (!v3)
        v3 = 1;
      R22_ = v3;
      v4 = E->ridley_var_B;
      if (v4 < 0) {
        v6 = R26_ + v4;
        v6 += 8;
        v7 = v6;
        v4 = R22_ + v7;
      }
      v9 = R22_ + v4;
      if (!sign16(v9 - 1280))
        v9 = 1280;
      E->ridley_var_B = v9;
    }
  }
}

void Ridley_Func_106(uint16 k, uint16 j) {  // 0xA6D62F
  int16 v3;
  int16 v5;
  int16 v6;
  int16 ridley_var_C;
  int16 v9;

  R24_ = g_byte_A6D712[j];
  Ridley_Func_107(k);
  Enemy_Ridley *E = Get_Ridley(k);
  v3 = E->base.y_pos - R20_;
  if (v3) {
    if (v3 >= 0) {
      uint16 RegWord = SnesDivide(v3, R24_);
      if (!RegWord)
        RegWord = 1;
      R22_ = RegWord;
      ridley_var_C = E->ridley_var_C;
      if (ridley_var_C >= 0)
        ridley_var_C = ridley_var_C - R22_ - R22_;
      v9 = ridley_var_C - R22_;
      if (sign16(v9 + 1280))
        v9 = -1280;
      E->ridley_var_C = v9;
    } else {
      uint16 v4 = SnesDivide(R20_ - E->base.y_pos, R24_);
      if (!v4)
        v4 = 1;
      R22_ = v4;
      v5 = E->ridley_var_C;
      if (v5 < 0)
        v5 += R22_ + R22_;
      v6 = R22_ + v5;
      if (!sign16(v6 - 1280))
        v6 = 1280;
      E->ridley_var_C = v6;
    }
  }
}

void Ridley_Func_107(uint16 k) {  // 0xA6D6A6
  Enemy_Ridley *E = Get_Ridley(k);
  int16 v2 = E->base.x_pos - R18_;
  if (!v2)
    return;
  if (v2 >= 0) {
    uint16 RegWord = SnesDivide(v2, R24_);
    R22_ = RegWord = RegWord ? RegWord : 1;
    int16 s = E->ridley_var_B;
    if (s >= 0)
      s -= 2 * RegWord;
    s -= RegWord;
    E->ridley_var_B = sign16(s + 1280) ? -1280 : s;
  } else {
    uint16 v3 = SnesDivide(R18_ - E->base.x_pos, R24_);
    R22_ = v3 ? v3 : 1;
    int16 s = E->ridley_var_B;
    if (s < 0)
      s += 2 * R22_;
    s += R22_;
    E->ridley_var_B = !sign16(s - 1280) ? 1280 : s;
  }
}

void Ridley_Func_108(uint16 k) {  // 0xA6D722
  R22_ = 512;
  R28_ = 0;
  Ridley_Func_110(k);
  Ridley_Func_109(k);
  R28_ >>= 1;
  R28_ >>= 1;
}

void Ridley_Func_109(uint16 k) {  // 0xA6D734
  int16 v2;
  int16 v6;
  int16 v10;

  Enemy_Ridley *E = Get_Ridley(k);
  if ((int16)(E->base.y_pos - R20_) >= 0) {
    v6 = E->ridley_var_C - R22_;
    if (sign16(v6 + 1280))
      v6 = -1280;
    E->ridley_var_C = v6;
    v10 = E->base.y_pos + (int8)(E->ridley_var_C >> 8);
    if ((int16)(v10 - R20_) <= 0) {
      E->ridley_var_C = 0;
      E->base.y_pos = R20_;
      ++R28_;
    }
  } else {
    v2 = E->ridley_var_C + R22_;
    if (!sign16(v2 - 1280))
      v2 = 1280;
    E->ridley_var_C = v2;
    v10 = E->base.y_pos + (int8)(E->ridley_var_C >> 8);
    if (!sign16(v10 - R20_)) {
      E->ridley_var_C = 0;
      E->base.y_pos = R20_;
      ++R28_;
    }
  }
}

void Ridley_Func_110(uint16 k) {  // 0xA6D798
  int16 v2;
  int16 v6;
  int16 v10;

  Enemy_Ridley *E = Get_Ridley(k);
  if ((int16)(E->base.x_pos - R18_) >= 0) {
    v6 = E->ridley_var_B - R22_;
    if (sign16(v6 + 1280))
      v6 = -1280;
    E->ridley_var_B = v6;
    v10 = E->base.x_pos + (int8)(E->ridley_var_B >> 8);
    if ((int16)(v10 - R18_) <= 0) {
      E->ridley_var_B = 0;
      E->base.x_pos = R18_;
      ++R28_;
    }
  } else {
    v2 = R22_ + E->ridley_var_B;
    if (!sign16(v2 - 1280))
      v2 = 1280;
    E->ridley_var_B = v2;
    if (!sign16(E->base.x_pos + (int8)(E->ridley_var_B >> 8) - R18_)) {
      E->ridley_var_B = 0;
      E->base.x_pos = R18_;
      ++R28_;
    }
  }
}

void Ridley_Func_111(void) {  // 0xA6D800
  Enemy_Ridley *E = Get_Ridley(0);
  if (R22_ != E->ridley_var_0B) {
    if ((int16)(R22_ - E->ridley_var_0B) >= 0) {
      uint16 v2 = E->ridley_var_0B + 32;
      if (!sign16(v2 - R22_))
        v2 = R22_;
      E->ridley_var_0B = v2;
    } else {
      uint16 v1 = E->ridley_var_0B - 32;
      if (sign16(v1 - R22_))
        v1 = R22_;
      E->ridley_var_0B = v1;
    }
  }
  uint16 v3;
  if ((R18_ & 0x8000u) != 0) {
    v3 = E->ridley_var_0A + R18_;
    if (sign16(v3 - R20_))
      LABEL_13:
    v3 = R20_;
  } else {
    v3 = E->ridley_var_0A + R18_;
    if (!sign16(v3 - R20_))
      goto LABEL_13;
  }
  E->ridley_var_0A = v3;
  R18_ = HIBYTE(v3);
  E->ridley_var_B = Math_MultBySin(E->ridley_var_0B);
  E->ridley_var_C = Math_MultByCos(E->ridley_var_0B);
}

void Ridley_Func_112(void) {  // 0xA6D86B
  Enemy_Ridley *Ridley;
  uint16 v1;
  Enemy_Ridley *v2;
  uint16 ridley_var_23;
  uint16 ridley_var_21;

  Ridley = Get_Ridley(0);
  Ridley->ridley_var_1F = 0;
  v1 = cur_enemy_index;
  v2 = Get_Ridley(cur_enemy_index);

  int with_carry = HIBYTE(v2->base.x_subpos) + LOBYTE(v2->ridley_var_B);
  HIBYTE(v2->base.x_subpos) = with_carry;
  ridley_var_23 = v2->base.x_pos + (int8)(v2->ridley_var_B >> 8) + (with_carry >> 8);
  if ((int16)(ridley_var_23 - Ridley->ridley_var_22) >= 0) {
    if ((int16)(ridley_var_23 - Ridley->ridley_var_23) >= 0) {
      v2->ridley_var_B = 0;
      Ridley->ridley_var_1F = 2;
      ridley_var_23 = Ridley->ridley_var_23;
    }
    v2->base.x_pos = ridley_var_23;
  } else {
    Ridley_Func_113(v1);
    v2->base.x_pos = Ridley->ridley_var_22;
    v2->ridley_var_B = 0;
    Ridley->ridley_var_1F = 1;
  }

  with_carry = HIBYTE(v2->base.y_subpos) + LOBYTE(v2->ridley_var_C);
  HIBYTE(v2->base.y_subpos) = with_carry;

  ridley_var_21 = v2->base.y_pos + (int8)(v2->ridley_var_C >> 8) + (with_carry >> 8);
  if ((int16)(ridley_var_21 - Ridley->ridley_var_20) >= 0) {
    if ((int16)(ridley_var_21 - Ridley->ridley_var_21) >= 0) {
      v2->ridley_var_C = 0;
      Ridley->ridley_var_1F = 8;
      ridley_var_21 = Ridley->ridley_var_21;
    }
    v2->base.y_pos = ridley_var_21;
  } else {
    v2->base.y_pos = Ridley->ridley_var_20;
    v2->ridley_var_C = 0;
    Ridley->ridley_var_1F = 4;
  }
}

void Ridley_Func_113(uint16 k) {  // 0xA6D914
  if (area_index != 2) {
    Enemy_Ridley *E = Get_Ridley(k);
    R18_ = abs16(E->ridley_var_B);
    uint16 v2 = abs16(E->ridley_var_C);
    if (v2 < R18_)
      v2 = R18_;
    if (v2 >= 0x280u) {
      uint16 v3;
      if (area_index == 2)
        v3 = 24;
      else
        v3 = 33;
      earthquake_type = v3;
      earthquake_timer = 12;
    }
  }
}

void Ridley_Func_114(void) {  // 0xA6D955
  Enemy_Ridley *E = Get_Ridley(0);
  uint16 ridley_var_10 = E->ridley_var_10, v2;
  if (!ridley_var_10) {
    if (*(int16 *)((char *)&E->base.enemy_ptr + 1) < 0)
      return;
    v2 = addr_kRidley_Ilist_E6F0;
    goto LABEL_7;
  }
  if (ridley_var_10 != 1 && *(int16 *)((char *)&E->base.enemy_ptr + 1) < 0) {
    v2 = addr_kRidley_Ilist_E706;
LABEL_7:
    E->base.current_instruction = v2;
    E->base.instruction_timer = 2;
    E->base.timer = 0;
  }
}

void Ridley_Func_115(void) {  // 0xA6D97D
  int16 v1;

  Ridley_Func_116();
  Enemy_Ridley *E = Get_Ridley(0);
  v1 = E->ridley_var_09 - E->ridley_var_08;
  E->ridley_var_09 = v1;
  if (v1 < 0) {
    E->ridley_var_09 = 32;
    uint16 v2 = E->ridley_var_07 + 1;
    if (v2 >= 0xAu)
      v2 = 0;
    E->ridley_var_07 = v2;
  }
}

void Ridley_Func_116(void) {  // 0xA6D9A8
  int16 v1;
  int16 v3;
  char v4; // t0

  Enemy_Ridley *E = Get_Ridley(0);
  R18_ = abs16(E->ridley_var_B);
  v1 = R18_ + abs16(E->ridley_var_C);
  if (v1) {
    uint16 v2 = v1 - R18_;
    if (sign16(v2 - R18_))
      v2 = R18_;
    v3 = (4 * v2) & 0xF00;
    v4 = v3;
    LOBYTE(v3) = HIBYTE(v3);
    HIBYTE(v3) = v4;
    uint16 v5 = 2 * v3;
    if (v5 >= 0xEu)
      v5 = 14;
    uint16 v6 = g_word_A6D9ED[v5 >> 1];
    if ((E->ridley_var_C & 0x8000u) == 0)
      v6 >>= 1;
    E->ridley_var_08 = v6;
  } else {
    E->ridley_var_08 = 0;
  }
}

void Ridley_Func_117(void) {  // 0xA6D9FD
  Enemy_Ridley *E = Get_Ridley(0);
  E->ridley_var_05 = addr_word_A6DA71;
  E->ridley_var_06 = 1;
}

void Ridley_Func_118(void) {  // 0xA6DA0C
  VramWriteEntry *v7;

  Enemy_Ridley *E = Get_Ridley(0);
  uint16 v1 = E->ridley_var_06 - 1;
  E->ridley_var_06 = v1;
  if (!v1) {
    int i;
    uint16 v3, *v4;
    for (i = E->ridley_var_05; ; E->ridley_var_05 = i) {
      v3 = i;
      v4 = (uint16 *)RomPtr_A6(i);
      i = *v4;
      if ((*v4 & 0x8000u) == 0)
        break;
    }
    E->ridley_var_06 = i;
    uint16 v6 = vram_write_queue_tail;
    v7 = gVramWriteEntry(vram_write_queue_tail);
    *(VoidP *)((char *)&v7->src.addr + 1) = -20480;
    *(VoidP *)((char *)&v7[1].src.addr + 1) = -20480;
    v7->src.addr = v4[1];
    v7[1].src.addr = v4[2];
    v7->vram_dst = 29216;
    v7[1].vram_dst = 29472;
    v7->size = 64;
    v7[1].size = 64;
    vram_write_queue_tail = v6 + 14;
    gVramWriteEntry(v6 + 14)->size = 0;
    E->ridley_var_05 = v3 + 6;
  }
}

void Ridley_Func_119(uint8 carry) {  // 0xA6DA8B
  VramWriteEntry *v3;

  uint16 v1 = addr_off_A6DAD0;
  if (carry)
    v1 = addr_off_A6DAD4;
  uint16 v2 = vram_write_queue_tail;
  v3 = gVramWriteEntry(vram_write_queue_tail);
  *(VoidP *)((char *)&v3->src.addr + 1) = -20480;
  *(VoidP *)((char *)&v3[1].src.addr + 1) = -20480;
  const uint8 *v4 = RomPtr_A6(v1);
  v3->src.addr = GET_WORD(v4);
  v3[1].src.addr = GET_WORD(v4 + 2);
  v3->vram_dst = 31424;
  v3[1].vram_dst = 31680;
  v3->size = 128;
  v3[1].size = 128;
  vram_write_queue_tail = v2 + 14;
  gVramWriteEntry(v2 + 14)->size = 0;
}

void Ridley_Func_120(void) {  // 0xA6DAD8
  int16 ridley_var_10;

  Enemy_Ridley *E = Get_Ridley(0);
  ridley_var_10 = E->ridley_var_10;
  if (ridley_var_10) {
    if (ridley_var_10 == 1)
      return;
    ridley_var_10 = 10;
  }
  uint16 v2 = g_off_A6DB02[E->ridley_var_07 + ridley_var_10];
  R22_ = E->ridley_var_0C;
  R18_ = E->base.x_pos;
  R20_ = E->base.y_pos;
  sub_A6DC13(v2);
}

void sub_A6DB2A(void) {  // 0xA6DB2A
  if ((gEnemyData(0)->properties & kEnemyProps_Invisible) == 0) {
    R18_ = tilemap_stuff[82];
    R20_ = tilemap_stuff[83];
    sub_A6DBC2(kRidley_Ilist_DCBA[(uint16)((uint8)((LOBYTE(tilemap_stuff[71])
                                                    + LOBYTE(tilemap_stuff[81])
                                                    + 8) & 0xF0) >> 3) >> 1]);
    R20_ = tilemap_stuff[73];
    R18_ = tilemap_stuff[72];
    sub_A6DBC2(0xDC9Eu);
    R18_ = tilemap_stuff[62];
    R20_ = tilemap_stuff[63];
    sub_A6DBC2(0xDC9Eu);
    R18_ = tilemap_stuff[52];
    R20_ = tilemap_stuff[53];
    sub_A6DBC2(0xDC97u);
    R18_ = tilemap_stuff[42];
    R20_ = tilemap_stuff[43];
    sub_A6DBC2(0xDC97u);
    R18_ = tilemap_stuff[32];
    R20_ = tilemap_stuff[33];
    sub_A6DBC2(0xDC90u);
    R18_ = tilemap_stuff[22];
    R20_ = tilemap_stuff[23];
    sub_A6DBC2(0xDC90u);
  }
}

void sub_A6DBC2(uint16 j) {  // 0xA6DBC2
  R22_ = Get_Ridley(0)->ridley_var_0C;
  sub_A6DC13(j);
}
uint16 CallBabyMetroidInstr(uint32 ea, uint16 k) {
  switch (ea) {
  case fnBabyMetroid_Instr_2: return BabyMetroid_Instr_2(k);
  case fnBabyMetroid_Instr_3: return BabyMetroid_Instr_3(k);
  case fnBabyMetroid_Instr_1: return BabyMetroid_Instr_1(k);
  case fnBabyMetroid_Instr_4: return BabyMetroid_Instr_4(k);
  default: return Unreachable();
  }
}
int BabyMetroid_DBCB_DoubleRetEx(uint16 a) {
  int16 v3;

  R0_.addr = a;
  R3_.addr = a + 2;
  R0_.bank = 126;
  R3_.bank = 126;

  uint16 *instr_ptr = (uint16 *)IndirPtr(&R0_, 0);
  uint16 *timer_ptr = (uint16 *)IndirPtr(&R3_, 0);

  if ((*instr_ptr & 0x8000u) == 0)
    return -1;  // double ret

  uint16 v2 = *instr_ptr;
  v3 = *(uint16 *)RomPtr_A6(v2);
  if (v3 < 0)
    goto LABEL_7;
  if (*timer_ptr != v3) {
    (*timer_ptr)++;
    return *((uint16 *)RomPtr_A6(v2) + 1);
  }
  v2 += 4;
  for (; ; ) {
    v3 = *(uint16 *)RomPtr_A6(v2);
    if (v3 >= 0)
      break;
LABEL_7:
    R6_ = v3;
    v2 = CallBabyMetroidInstr((uint16)v3 | 0xA60000, v2 + 2);
  }
  *timer_ptr = 1;
  *instr_ptr = v2;
  return *((uint16 *)RomPtr_A6(v2) + 1);
}

void sub_A6DC13(uint16 j) {  // 0xA6DC13
  int16 *v5;
  int16 v6;
  int16 v7;
  char v8; // t0
  int16 v9;
  int16 v10;
  OamEnt *v11;
  int16 v13;
  int16 v15;

  uint16 *v1 = (uint16 *)RomPtr_A6(j);
  uint16 v2 = j + 2;
  R24_ = *v1;
  uint16 v3 = oam_next_ptr;
  do {
    const uint8 *v4 = RomPtr_A6(v2);
    v5 = (int16 *)v4;
    v6 = v4[2] << 8;
    if ((v4[2] & 0x80) != 0)
      v6 |= 0xFFu;
    v8 = v6;
    LOBYTE(v7) = HIBYTE(v6);
    HIBYTE(v7) = v8;
    v9 = R20_ + v7 - layer1_y_pos;
    if (v9 >= 0 && sign16(v9 - 224)) {
      R26_ = v9;
      v10 = R18_ + GET_WORD(v4) - layer1_x_pos;
      v11 = gOamEnt(v3);
      *(uint16 *)&v11->xcoord = v10;
      if ((v10 & 0x100) != 0) {
        int v12 = v3 >> 1;
        R28_ = kOamExtra_Address_And_X8Large[v12];
        v13 = kOamExtra_X8Small_And_Large[v12] | *(uint16 *)RomPtr_RAM(R28_);
        *(uint16 *)RomPtr_RAM(R28_) = v13;
      }
      if (*v5 < 0) {
        int v14 = v3 >> 1;
        R28_ = kOamExtra_Address_And_X8Large[v14];
        v15 = kOamExtra_X8Small_And_Large[v14 + 1] | *(uint16 *)RomPtr_RAM(R28_);
        *(uint16 *)RomPtr_RAM(R28_) = v15;
      }
      *(uint16 *)&v11->ycoord = R26_;
      *(uint16 *)&v11->charnum = R22_ | *(int16 *)((char *)v5 + 3);
      v3 = (v3 + 4) & 0x1FF;
    }
    v2 += 5;
    --R24_;
  } while (R24_);
  oam_next_ptr = v3;
}

uint8 Ridley_Func_121(void) {  // 0xA6DE7A
  int16 y_pos;
  int16 v2;
  int16 x_pos;
  int16 v4;

  Enemy_Ridley *E = Get_Ridley(0);
  y_pos = E->base.y_pos;
  uint8 result = 1;
  if (y_pos >= 0) {
    v2 = y_pos + 32 - layer1_y_pos;
    if (v2 >= 0) {
      if (sign16(v2 - 288)) {
        x_pos = E->base.x_pos;
        if (x_pos >= 0) {
          v4 = x_pos + 32 - layer1_x_pos;
          if (v4 >= 0) {
            if (sign16(v4 - 320))
              return 0;
          }
        }
      }
    }
  }
  return result;
}

uint16 Ridley_Func_122(void) {  // 0xA6DEA6
  int16 v2;

  if (!projectile_counter)
    return 0xffff;
  uint16 result = 0;
  while (1) {
    int v1 = result >> 1;
    v2 = projectile_type[v1];
    if (v2 < 0) {
      if (sign16((HIBYTE(v2) & 0xF) - 3)) {
        uint16 v3 = abs16(projectile_x_pos[v1] - R18_);
        bool v4 = v3 < projectile_x_radius[v1];
        uint16 v5 = v3 - projectile_x_radius[v1];
        if (v4 || v5 < R22_) {
          uint16 v6 = abs16(projectile_y_pos[v1] - R20_);
          v4 = v6 < projectile_y_radius[v1];
          uint16 v7 = v6 - projectile_y_radius[v1];
          if (v4 || v7 < R24_)
            break;
        }
      }
    }
    result += 2;
    if ((int16)(result - 10) >= 0)
      return 0xffff;
  }
  int v8 = result >> 1;
  projectile_x_pos[v8] = R18_;
  projectile_y_pos[v8] = R20_;
  projectile_dir[v8] |= 0x10u;
  return result;
}

void Ridley_Func_123(uint16 j) {  // 0xA6DF08
  int v1 = j >> 1;
  uint16 v2;
  if ((projectile_dir[v1] & 0xF) == 7) {
    v2 = 1;
  } else if ((projectile_dir[v1] & 0xF) == 2) {
    v2 = 8;
  } else {
    v2 = 5;
  }
  projectile_dir[v1] = v2;
}

uint8 Ridley_Func_124(void) {  // 0xA6DF29
  uint16 v0 = abs16(samus_x_pos - R18_);
  bool v1 = v0 < samus_x_radius;
  uint16 v2 = v0 - samus_x_radius;
  uint8 result = 0;
  if (v1 || v2 < R22_) {
    uint16 v3 = abs16(samus_y_pos - R20_);
    v1 = v3 < samus_y_radius;
    uint16 v4 = v3 - samus_y_radius;
    if (v1 || v4 < R24_)
      return 1;
  }
  return result;
}

void sub_A6DF59(void) {  // 0xA6DF59
  NormalEnemyTouchAiSkipDeathAnim_CurEnemy();
}

void sub_A6DF60(void) {  // 0xA6DF60
  Ridley_Func_125();
}

void Ridley_Func_125(void) {  // 0xA6DF66
  Ridley_Func_98();
  samus_invincibility_timer = 96;
  samus_knockback_timer = 5;
  knockback_x_dir = (int16)(samus_x_pos - Get_Ridley(cur_enemy_index)->base.x_pos) >= 0;
}

void Ridley_Shot(void) {  // 0xA6DF8A
  int16 v0;

  if (area_index == 2) {
    NormalEnemyShotAiSkipDeathAnim_CurEnemy();
  } else {
    v0 = 13;
    Enemy_Ridley *E = Get_Ridley(0);
    uint16 flash_timer = E->base.flash_timer;
    if (flash_timer) {
      if (flash_timer & 1)
        v0 = 14;
    }
    E->base.flash_timer = v0;
    ++E->ridley_var_0D;
  }
}

void Ridley_Powerbomb(void) {  // 0xA6DFB2
  NormalEnemyPowerBombAiSkipDeathAnim_CurEnemy();
}

void Ridley_Func_126(void) {  // 0xA6DFB7
  Enemy_Ridley *E = Get_Ridley(0);
  if (!E->base.health && (E->ridley_var_01 & 0x8000u) == 0) {
    E->ridley_var_01 = -1;
    E->base.properties |= kEnemyProps_Tangible;
    E->ridley_var_A = FUNC16(Ridley_Func_63);
  }
}

void Ridley_Func_127(void) {  // 0xA6DFD9
  R22_ = 14;
  R24_ = 14;
  R18_ = tilemap_stuff[82];
  R20_ = tilemap_stuff[83];
  if (Ridley_Func_124() & 1) {
    Enemy_Ridley *E = Get_Ridley(0);
    uint16 v1 = SuitDamageDivision(E->ridley_var_1C);
    Samus_DealDamage(v1);
    samus_invincibility_timer = 96;
    samus_knockback_timer = 5;
    knockback_x_dir = (int16)(samus_x_pos - tilemap_stuff[82]) >= 0;
  }
}

void Ridley_Func_128(void) {  // 0xA6E01B
  Enemy_Ridley *E = Get_Ridley(cur_enemy_index);
  R18_ = projectile_x_pos[0] - E->base.x_pos;
  R20_ = projectile_y_pos[0] - E->base.y_pos;
  R18_ = (uint8)-CalculateAngleFromXY();
  uint16 v1 = 4 * projectile_damage[collision_detection_index];
  if (v1 >= 0x300u)
    v1 = 768;
  uint16 a = v1;
  R18_ = Math_MultBySin(v1);
  if (((E->ridley_var_B ^ R18_) & 0x8000u) != 0)
    E->ridley_var_B += R18_;
  R18_ = Math_MultByCos(a);
  if (((E->ridley_var_C ^ R18_) & 0x8000u) != 0)
    E->ridley_var_C += R18_;
}

void Ridley_Func_129(void) {  // 0xA6E088
  if ((Get_Ridley(0)->base.properties & kEnemyProps_Tangible) == 0) {
    R22_ = 14;
    R24_ = 14;
    R18_ = tilemap_stuff[82];
    R20_ = tilemap_stuff[83];
    uint16 v0 = Ridley_Func_122();
    if (!sign16(v0)
        || (R22_ = 10, R24_ = 10, R18_ = tilemap_stuff[72], R20_ = tilemap_stuff[73], !sign16(v0 = Ridley_Func_122()))) {
      int v1 = v0 >> 1;
      R18_ = projectile_x_pos[v1];
      R20_ = projectile_y_pos[v1];
      uint16 v2 = 12;
      if ((HIBYTE(projectile_type[v1]) & 0xF) == 1) {
        QueueSfx1_Max6(0x3Du);
        v2 = 6;
      }
      SpawnEnemyProjectileWithRoomGfx(0xE509, v2);
    }
  }
}

const uint16 *Ridley_Instr_5(uint16 k, const uint16 *jp) {  // 0xA6E4BE
  Get_Ridley(0)->ridley_var_0F = 89;
  QueueSfx2_Max6(0x59u);
  return jp;
}

const uint16 *Ridley_Instr_6(uint16 k, const uint16 *jp) {  // 0xA6E4CA
  Get_Ridley(0)->ridley_var_0F = 0;
  return jp;
}

const uint16 *Ridley_Instr_10(uint16 k, const uint16 *jp) {  // 0xA6E4D2
  if (area_index == 2 || !sign16(samus_health - 30))
    return jp + 1;
  Get_Ridley(0)->ridley_var_00 = 8;
  return INSTR_RETURN_ADDR(jp[0]);
}

const uint16 *Ridley_Instr_4(uint16 k, const uint16 *jp) {  // 0xA6E4EE
  if (Get_Ridley(0)->ridley_var_1B)
    return INSTR_RETURN_ADDR(jp[0]);
  else
    return INSTR_RETURN_ADDR(jp[1]);
}

const uint16 *Ridley_Instr_3(uint16 k, const uint16 *jp) {  // 0xA6E4F8
  if (Get_Ridley(0)->ridley_var_1B)
    return jp + 1;
  else
    return INSTR_RETURN_ADDR(jp[0]);
}

const uint16 *Ridley_Instr_2(uint16 k, const uint16 *jp) {  // 0xA6E501
  samus_prev_x_pos = samus_x_pos;
  samus_prev_y_pos = samus_y_pos;
  Get_Ridley(0)->ridley_var_1D = jp[0];
  return jp + 1;
}

const uint16 *Ridley_Instr_1(uint16 k, const uint16 *jp) {  // 0xA6E517
  if (Get_Ridley(0)->ridley_var_10)
    return INSTR_RETURN_ADDR(jp[0]);
  else
    return jp + 1;
}

const uint16 *Ridley_Instr_14(uint16 k, const uint16 *jp) {  // 0xA6E51F
  Enemy_Ridley *E = Get_Ridley(0);
  E->base.x_pos += jp[0];
  E->base.y_pos += jp[1];
  return jp + 2;
}

const uint16 *Ridley_Instr_9(uint16 k, const uint16 *jp) {  // 0xA6E71C
  Get_Ridley(0)->ridley_var_10 = 0;
  Ridley_Func_96();
  return jp;
}

const uint16 *Ridley_Instr_7(uint16 k, const uint16 *jp) {  // 0xA6E727
  Get_Ridley(0)->ridley_var_10 = 1;
  return jp;
}

const uint16 *Ridley_Instr_8(uint16 k, const uint16 *jp) {  // 0xA6E72F
  Get_Ridley(0)->ridley_var_10 = 2;
  Ridley_Func_96();
  return jp;
}

void Ridley_Func_131(uint16 k) {  // 0xA6E828
  Ridley_Func_132(k, 0);
  Ridley_Func_132(k, 1u);
  Ridley_Func_132(k, 2u);
  Ridley_Func_132(k, 3u);
}

void Ridley_Func_132(uint16 k, uint16 a) {  // 0xA6E840
  Get_Ridley(0)->ridley_parameter_1 = a;
  SpawnEnemyProjectileWithGfx(a, k, addr_stru_869634);
}

const uint16 *Ridley_Instr_11(uint16 k, const uint16 *jp) {  // 0xA6E84D
  Enemy_Ridley *E = Get_Ridley(0);
  if (E->ridley_var_10) {
    R18_ = E->base.x_pos + 25;
    R18_ = samus_x_pos - R18_;
    R20_ = E->base.y_pos - 43;
    R20_ = samus_y_pos - R20_;
    uint16 v4 = (uint8)-(int8)(CalculateAngleFromXY() + 0x80);
    if (v4 < 0x50u) {
      if (v4 >= 0x15u)
        goto LABEL_13;
    } else if (v4 < 0xC0u) {
      v4 = 80;
LABEL_13:
      R18_ = v4;
      goto LABEL_14;
    }
    v4 = 21;
    goto LABEL_13;
  }
  R18_ = E->base.x_pos - 25;
  R18_ = samus_x_pos - R18_;
  R20_ = E->base.y_pos - 43;
  R20_ = samus_y_pos - R20_;
  uint16 v3;
  v3 = (uint8)-(int8)(CalculateAngleFromXY() + 0x80);
  if (v3 >= 0xB0u) {
    if (v3 < 0xEBu)
      goto LABEL_7;
LABEL_6:
    v3 = 235;
    goto LABEL_7;
  }
  if (v3 < 0x40u)
    goto LABEL_6;
  v3 = 176;
LABEL_7:
  R18_ = v3;
LABEL_14:
  E->ridley_var_19 = Math_MultBySin(0x500u);
  E->ridley_var_1A = Math_MultByCos(0x500u);
  return jp;
}

const uint16 *Ridley_Instr_12(uint16 k, const uint16 *jp) {  // 0xA6E904
  return Ridley_E90C(k, jp, 0);
}

const uint16 *Ridley_Instr_13(uint16 k, const uint16 *jp) {  // 0xA6E909
  return Ridley_E90C(k, jp, 0xe);
}

const uint16 *Ridley_E90C(uint16 k, const uint16 *jp, uint16 a) {  // 0xA6E90C
  enemy_projectile_unk1995 = a;
  Enemy_Ridley *E = Get_Ridley(0);
  SpawnEnemyProjectileWithRoomGfx(0x9642, E->ridley_var_10);
  return jp;
}


const uint16 *Ridley_Instr_15(uint16 k, const uint16 *jp) {  // 0xA6E969
  Enemy_Ridley *E = Get_Ridley(0);
  E->ridley_var_A = FUNC16(CeresRidley_Func_9);
  E->ridley_var_C = -352;
  return jp;
}

const uint16 *Ridley_Instr_16(uint16 k, const uint16 *jp) {  // 0xA6E976
  Enemy_Ridley *E = Get_Ridley(0);
  E->ridley_var_A = FUNC16(Ridley_Func_3);
  E->ridley_var_C = -352;
  return jp;
}

void CeresSteam_Init(void) {  // 0xA6EFB1
  Enemy_CeresSteam *E = Get_CeresSteam(cur_enemy_index);
  E->base.vram_tiles_index = 0;
  E->base.properties |= kEnemyProps_DisableSamusColl;
  E->base.extra_properties |= 4u;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->base.palette_index = 2560;
  E->csm_var_D = (NextRandom() & 0x1F) + 1;
  int v1 = (uint16)(2 * E->csm_parameter_1) >> 1;
  E->base.current_instruction = g_off_A6EFF5[v1];
  E->csm_var_A = g_off_A6F001[v1];
}

void CallCeresSteamFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnnullsub_350: return;  // 0xa6eff4
  case fnCeresSteam_Func_1: CeresSteam_Func_1(k); return;  // 0xa6f019
  default: Unreachable();
  }
}

void CeresSteam_Main(void) {  // 0xA6F00D
  Enemy_CeresSteam *E = Get_CeresSteam(cur_enemy_index);
  E->base.health = 0x7FFF;
  CallCeresSteamFunc(E->csm_var_A | 0xA60000, cur_enemy_index);
}

void CeresSteam_Func_1(uint16 k) {  // 0xA6F019
  EnemySpawnData *v3;

  Enemy_CeresSteam *E = Get_CeresSteam(k);
  R18_ = E->base.x_pos;
  R20_ = E->base.y_pos;
  CalcCeresSteamPos_Mode7();
  v3 = gEnemySpawnData(cur_enemy_index);
  v3->xpos2 = R18_ - E->base.x_pos;
  v3->ypos2 = R20_ - E->base.y_pos;
}

void CeresSteam_Touch(void) {  // 0xA6F03F
  Get_CeresSteam(cur_enemy_index)->base.health = 0x7FFF;
  NormalEnemyTouchAi();
}

const uint16 *CeresSteam_Instr_1(uint16 k, const uint16 *jp) {  // 0xA6F11D
  Enemy_CeresSteam *E = Get_CeresSteam(k);
  E->base.properties |= kEnemyProps_Tangible | kEnemyProps_Invisible;
  return jp;
}

const uint16 *CeresSteam_Instr_2(uint16 k, const uint16 *jp) {  // 0xA6F127
  Enemy_CeresSteam *E = Get_CeresSteam(k);
  if (E->csm_var_D-- != 1)
    return INSTR_RETURN_ADDR(jp[0]);
  return CeresSteam_Instr_3(k, (uint16 *)RomPtr_A6(jp[1]));
}

const uint16 *CeresSteam_Instr_3(uint16 k, const uint16 *jp) {  // 0xA6F135
  Enemy_CeresSteam *E = Get_CeresSteam(k);
  E->base.properties &= 0xFAFFu;
  return jp;
}

const uint16 *CeresDoor_Instr_6(uint16 k, const uint16 *jp) {  // 0xA6F63E
  Enemy_CeresDoor *E = Get_CeresDoor(k);
  if (abs16(E->base.x_pos - samus_x_pos) >= 0x30u || abs16(E->base.y_pos - samus_y_pos) >= 0x30u)
    return INSTR_RETURN_ADDR(*jp);
  else
    return jp + 1;
}

const uint16 *CeresDoor_Instr_4(uint16 k, const uint16 *jp) {  // 0xA6F66A
  if (*(uint16 *)&boss_bits_for_area[area_index] & 1)
    return jp + 1;
  else
    return INSTR_RETURN_ADDR(*jp);
}

const uint16 *CeresDoor_Instr_8(uint16 k, const uint16 *jp) {  // 0xA6F678
  if (ceres_status)
    return jp + 1;
  else
    return INSTR_RETURN_ADDR(*jp);
}

void CeresDoor_Func_6b(void) {  // 0xA6F67F
  if (ceres_status)
    ceres_status = FUNC16(Enemy_GrappleReact_NoInteract_A6);
}

const uint16 *CeresSteam_Instr_4(uint16 k, const uint16 *jp) {  // 0xA6F68B
  Enemy_CeresSteam *E = Get_CeresSteam(k);
  E->base.properties |= kEnemyProps_Tangible;
  return jp;
}

const uint16 *CeresDoor_Instr_1(uint16 k, const uint16 *jp) {  // 0xA6F695
  Enemy_CeresDoor *E = Get_CeresDoor(k);
  E->base.properties &= ~kEnemyProps_Tangible;
  return jp;
}

const uint16 *CeresDoor_Instr_3(uint16 k, const uint16 *jp) {  // 0xA6F69F
  Get_CeresDoor(k)->cdr_var_B = 1;
  return jp;
}

const uint16 *CeresSteam_Instr_5(uint16 k, const uint16 *jp) {  // 0xA6F6A6
  Enemy_CeresSteam *E = Get_CeresSteam(k);
  E->base.properties |= kEnemyProps_Invisible;
  return jp;
}

const uint16 *CeresDoor_Instr_5(uint16 k, const uint16 *jp) {  // 0xA6F6B0
  Get_CeresDoor(k)->cdr_var_B = 0;
  return CeresDoor_Instr_2(k, jp);
}

const uint16 *CeresDoor_Instr_2(uint16 k, const uint16 *jp) {  // 0xA6F6B3
  Enemy_CeresDoor *E = Get_CeresDoor(k);
  E->base.properties &= ~kEnemyProps_Invisible;
  return jp;
}

const uint16 *CeresDoor_Instr_7(uint16 k, const uint16 *jp) {  // 0xA6F6BD
  QueueSfx3_Max6(0x2Cu);
  return jp;
}

void CeresDoor_Init(void) {  // 0xA6F6C5
  Enemy_CeresDoor *E = Get_CeresDoor(cur_enemy_index);
  E->base.spritemap_pointer = addr_kCeresDoor_Sprmap_FAC7;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->base.vram_tiles_index = 0;
  E->base.palette_index = 1024;
  int v2 = (uint16)(2 * E->cdr_parameter_1) >> 1;
  E->cdr_var_A = g_off_A6F72B[v2];
  E->base.current_instruction = g_off_A6F52C[v2];
  E->cdr_var_B = 0;
  CeresDoor_Func_1(cur_enemy_index);
  uint16 v3;
  if (ceres_status) {
    v3 = addr_word_A6F50C__plus__2;
  } else {
    if (E->cdr_parameter_1 == 3) {
      WriteColorsToTargetPalette(0xa6, 0x142, addr_kCeresDoor_Palette__plus__2, 0xF);
      return;
    }
    v3 = addr_kCeresDoor_Palette__plus__2;
  }
  Get_CeresDoor(cur_enemy_index)->base.palette_index = 3584;
  WriteColorsToTargetPalette(0xa6, 0x1E2, v3, 0xF);
}

void CeresDoor_Func_1(uint16 k) {  // 0xA6F739
  VramWriteEntry *v2;

  if (Get_CeresDoor(k)->cdr_parameter_1 == 2) {
    uint16 v1 = vram_write_queue_tail;
    v2 = gVramWriteEntry(vram_write_queue_tail);
    v2->size = 1024;
    *(VoidP *)((char *)&v2->src.addr + 1) = -20480;
    v2->src.addr = -15360;
    v2->vram_dst = 28672;
    vram_write_queue_tail = v1 + 7;
  }
}

void CallCeresDoor(uint32 ea) {
  uint16 k = cur_enemy_index;
  switch (ea) {
  case fnCeresDoor_Func_2: CeresDoor_Func_2(); return;  // 0xa6f76b
  case fnCeresDoor_Func_3: CeresDoor_Func_3(); return;  // 0xa6f770
  case fnCeresDoor_Func_4: CeresDoor_Func_4(); return;  // 0xa6f7a5
  case fnCeresDoor_Func_5: CeresDoor_Func_5(k); return;  // 0xa6f7bd
  case fnCeresDoor_Func_6: CeresDoor_Func_6(k); return;  // 0xa6f7dc
  case fnCeresDoor_Func_7: CeresDoor_Func_7(); return;  // 0xa6f850
  default: Unreachable();
  }
}

void CeresDoor_Main(void) {  // 0xA6F765
  Enemy_CeresDoor *E = Get_CeresDoor(cur_enemy_index);
  CallCeresDoor(E->cdr_var_A | 0xA60000);
}

void CeresDoor_Func_2(void) {  // 0xA6F76B
  CeresDoor_F773(0x14u);
}

void CeresDoor_Func_3(void) {  // 0xA6F770
  CeresDoor_F773(0x1Du);
}

void CeresDoor_F773(uint16 j) {  // 0xA6F773
  if (ceres_status >= 2u && !earthquake_timer) {
    if ((random_number & 0xFFFu) < 0x80) {
      earthquake_timer = 4;
      earthquake_type = j + 6;
    } else {
      earthquake_timer = 2;
      earthquake_type = j;
    }
  }
}

void CeresDoor_Func_4(void) {  // 0xA6F7A5
  CeresSteam_Instr_5(cur_enemy_index, 0);
  if (ceres_status & 1) {
    Get_CeresDoor(cur_enemy_index)->base.palette_index = 3584;
    CeresDoor_Instr_2(cur_enemy_index, 0);
  }
}

void CeresDoor_Func_5(uint16 k) {  // 0xA6F7BD
  CeresDoor_Func_7();
  if (ceres_status >= 2u) {
    Enemy_CeresDoor *E = Get_CeresDoor(k);
    E->cdr_var_A = FUNC16(CeresDoor_Func_6);
    E->cdr_var_D = 48;
    Enemy_CeresDoor *E0 = Get_CeresDoor(0);
    E0->cdr_var_E = 0;
    E0->cdr_var_F = 0;
  }
}

void CeresDoor_Func_6(uint16 k) {  // 0xA6F7DC
  Enemy_CeresDoor *E = Get_CeresDoor(k);
  bool v3 = (--E->cdr_var_D & 0x8000u) != 0;
  if (v3) {
    E->base.properties |= kEnemyProps_Invisible;
    E->cdr_var_A = FUNC16(CeresDoor_Func_7);
    CeresDoor_Func_6b();
  } else {
    Enemy_CeresDoor *E0 = Get_CeresDoor(0);
    v3 = (--E0->cdr_var_E & 0x8000u) != 0;
    if (v3) {
      E0->cdr_var_E = 4;
      v3 = (--E0->cdr_var_F & 0x8000u) != 0;
      if (v3)
        E0->cdr_var_F = 3;
      int v5 = (uint16)(4 * E0->cdr_var_F) >> 1;
      R18_ = E->base.x_pos + g_word_A6F840[v5];
      R20_ = E->base.y_pos + g_word_A6F840[v5 + 1];
      uint16 v6 = 3;
      if (NextRandom() < 0x4000u)
        v6 = 12;
      SpawnEnemyProjectileWithRoomGfx(0xE509, v6);
      QueueSfx2_Max6(0x25u);
    }
  }
}

void CeresDoor_Func_7(void) {  // 0xA6F850
  CeresDoor_Func_8();
  if (!palette_change_num)
    WriteColorsToPalette(
      0x52u,
      0xa6, 2 * (nmi_frame_counter_word & 0x38) - 0x78F,
      6u);
}

void CeresDoor_Func_8(void) {  // 0xA6F8F1
  QueueMode7Transfers(0xA6, g_off_A6F900[(uint8)(nmi_frame_counter_word & 2) >> 1]);
}

void Zebetites_Init(void) {  // 0xA6FB72
  Enemy_Zebetites *E = Get_Zebetites(cur_enemy_index);
  E->base.properties |= 0xA000u;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->base.palette_index = 1024;
  E->base.vram_tiles_index = 128;
  E->zebet_var_C = 0;
  uint16 v1 = FUNC16(Zebetites_Func_1);
  if (E->zebet_parameter_1)
    v1 = FUNC16(Zebetites_Func_2);
  E->zebet_var_A = v1;
  R18_ = 0;
  R18_ = (R18_ << 1) | CheckEventHappened(5u);
  R18_ = (R18_ << 1) | CheckEventHappened(4u);
  R18_ = (R18_ << 1) | CheckEventHappened(3u);
  uint16 v2 = R18_;
  E->zebet_var_D = R18_;
  if (sign16(v2 - 4)) {
    int v3 = v2;
    E->zebet_var_F = g_word_A6FC03[v3];
    E->base.y_height = g_word_A6FC0B[v3];
    E->base.current_instruction = g_off_A6FC13[v3];
    E->base.x_pos = g_word_A6FC1B[v3];
    uint16 v4;
    if (E->zebet_parameter_1)
      v4 = g_word_A6FC2B[v3];
    else
      v4 = g_word_A6FC23[v3];
    E->base.y_pos = v4;
  } else {
    E->base.properties |= kEnemyProps_Deleted;
  }
}

void CallZebetitesFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnZebetites_Func_1: Zebetites_Func_1(k); return;
  case fnZebetites_Func_2: Zebetites_Func_2(k); return;
  case fnZebetites_Func_3: Zebetites_Func_3(); return;
  default: Unreachable();
  }
}

void Zebetites_Main(void) {  // 0xA6FC33
  if (!earthquake_timer)
    Get_Zebetites(cur_enemy_index)->base.shake_timer = 0;
  Enemy_Zebetites *E = Get_Zebetites(cur_enemy_index);
  CallZebetitesFunc(E->zebet_var_A | 0xA60000, cur_enemy_index);
}

void Zebetites_Func_1(uint16 k) {  // 0xA6FC41
  Enemy_Zebetites *E = Get_Zebetites(k);
  if ((E->zebet_var_F & 0x8000u) != 0) {
    uint16 new_k = Zebetites_Func_9();
    Get_Zebetites(new_k)->zebet_parameter_2 = k;
    Get_Zebetites(k)->zebet_parameter_2 = new_k;
  }
  Get_Zebetites(k)->zebet_var_A = FUNC16(Zebetites_Func_2);
  Zebetites_Func_2(k);
}

void Zebetites_Func_2(uint16 k) {  // 0xA6FC5B
  if (!door_transition_flag_elevator_zebetites) {
    Get_Zebetites(k)->zebet_var_A = FUNC16(Zebetites_Func_3);
    Zebetites_Func_3();
  }
}

void Zebetites_Func_3(void) {  // 0xA6FC67
  int16 v3;

  Zebetites_Func_4();
  Zebetites_Func_5();
  Enemy_Zebetites *E = Get_Zebetites(cur_enemy_index);
  uint16 health = E->base.health;
  if (health) {
    v3 = health + 1;
    if (!sign16(v3 - 1000))
      v3 = 1000;
    E->base.health = v3;
  } else if (E->zebet_parameter_1
             || (Zebetites_Func_6(), !sign16(E->zebet_var_D - 4))) {
    EnemyDeathAnimation(cur_enemy_index, 0);
  } else {
    EnemyDeathAnimation(cur_enemy_index, 0);
    Zebetites_Func_8();
  }
}

void Zebetites_Func_6(void) {  // 0xA6FCAA
  int16 v1;

  Enemy_Zebetites *E = Get_Zebetites(cur_enemy_index);
  v1 = E->zebet_var_D + 1;
  E->zebet_var_D = v1;
  R18_ = v1;
  Zebetites_Func_7(3u, (R18_ >> 0) & 1);
  Zebetites_Func_7(4u, (R18_ >> 1) & 1);
  Zebetites_Func_7(5u, (R18_ >> 2) & 1);
}

void Zebetites_Func_7(uint16 j, uint8 a) {  // 0xA6FCCB
  if (a)
    SetEventHappened(j);
  else
    ClearEventHappened(j);
}

void Zebetites_Func_8(void) {  // 0xA6FCD9
  SpawnEnemy(0xA6, addr_stru_A6FCE1);
}

uint16 Zebetites_Func_9(void) {  // 0xA6FCF1
  return SpawnEnemy(0xA6, addr_stru_A6FCF9);
}

void Zebetites_Func_5(void) {  // 0xA6FD09
  uint16 v0 = 0;
  Enemy_Zebetites *E = Get_Zebetites(cur_enemy_index);
  uint16 health = E->base.health;
  if (sign16(health - 800)) {
    v0 = 2;
    if (sign16(health - 600)) {
      v0 = 4;
      if (sign16(health - 400)) {
        v0 = 6;
        if (sign16(health - 200))
          v0 = 8;
      }
    }
  }
  int v3 = v0 >> 1;
  uint16 v4 = g_off_A6FD4A[v3];
  if ((E->zebet_var_F & 0x8000u) != 0)
    v4 = g_off_A6FD54[v3];
  E->base.current_instruction = v4;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
}

void Zebetites_Func_4(void) {  // 0xA6FD5E
  if (!palette_change_num && !Get_Zebetites(cur_enemy_index)->zebet_parameter_1) {
    Enemy_Zebetites *E = Get_Zebetites(0);
    uint16 v1 = ((uint8)E->zebet_var_C + 1) & 7;
    E->zebet_var_C = v1;
    WriteColorsToPalette(0x158, 0xa6, 4 * v1 - 0x279, 2u);
  }
}

void Zebetites_Touch(void) {  // 0xA6FDA7
  NormalEnemyTouchAiSkipDeathAnim_CurEnemy();
}

void Zebetites_Shot(void) {  // 0xA6FDAC
  QueueSfx3_Max6(9u);
  NormalEnemyShotAiSkipDeathAnim_CurEnemy();
  Enemy_Zebetites *E = Get_Zebetites(cur_enemy_index);
  Enemy_Zebetites *G = Get_Zebetites(E->zebet_parameter_2);
  G->base.health = E->base.health;
  G->base.flash_timer = E->base.flash_timer;
}