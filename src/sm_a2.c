// Enemy AI - inc. gunship & shutters
#include "ida_types.h"
#include "sm_rtl.h"
#include "funcs.h"
#include "enemy_types.h"
#include "variables.h"


#define kEnemyInit_BouncingGoofball_Tab0 ((uint16*)RomFixedPtr(0xa286df))
#define kEnemyInit_BouncingGoofball_Tab1 ((uint16*)RomFixedPtr(0xa286ef))
#define kBouncingGoofball_Tab0 ((uint8*)RomFixedPtr(0xa28701))
#define g_word_A28D56 (*(uint16*)RomFixedPtr(0xa28d56))
#define g_word_A28D58 (*(uint16*)RomFixedPtr(0xa28d58))
#define g_word_A29A07 (*(uint16*)RomFixedPtr(0xa29a07))
#define g_word_A29A09 (*(uint16*)RomFixedPtr(0xa29a09))
#define g_word_A29A0B (*(uint16*)RomFixedPtr(0xa29a0b))
#define g_word_A29A0D (*(uint16*)RomFixedPtr(0xa29a0d))
#define g_word_A29F36 ((uint16*)RomFixedPtr(0xa29f36))
#define g_off_A29F42 ((uint16*)RomFixedPtr(0xa29f42))
#define g_word_A2A3DD ((uint16*)RomFixedPtr(0xa2a3dd))
#define g_word_A2A3ED ((uint16*)RomFixedPtr(0xa2a3ed))
#define g_byte_A2A7CF ((uint8*)RomFixedPtr(0xa2a7cf))
#define g_word_A2A622 ((uint16*)RomFixedPtr(0xa2a622))
#define g_word_A2AC07 ((uint16*)RomFixedPtr(0xa2ac07))
#define g_word_A2AC11 ((uint16*)RomFixedPtr(0xa2ac11))
#define g_word_A2B520 ((uint16*)RomFixedPtr(0xa2b520))
#define g_word_A2B530 ((uint16*)RomFixedPtr(0xa2b530))
#define g_word_A2B550 ((uint16*)RomFixedPtr(0xa2b550))
#define g_word_A2B75B ((uint16*)RomFixedPtr(0xa2b75b))
#define g_word_A2BBBB (*(uint16*)RomFixedPtr(0xa2bbbb))
#define g_word_A2BBBF (*(uint16*)RomFixedPtr(0xa2bbbf))
#define g_word_A2BE86 ((uint16*)RomFixedPtr(0xa2be86))
#define g_word_A2C1C1 ((uint16*)RomFixedPtr(0xa2c1c1))
#define g_word_A2C1C5 (*(uint16*)RomFixedPtr(0xa2c1c5))
#define g_word_A2C6CA (*(uint16*)RomFixedPtr(0xa2c6ca))
#define g_word_A2C6CE (*(uint16*)RomFixedPtr(0xa2c6ce))
#define g_off_A2CB77 ((uint16*)RomFixedPtr(0xa2cb77))
#define g_off_A2DF5E ((uint16*)RomFixedPtr(0xa2df5e))
#define g_off_A2DF6A ((uint16*)RomFixedPtr(0xa2df6a))
#define g_off_A2E5EF ((uint16*)RomFixedPtr(0xa2e5ef))
#define g_word_A2EA56 ((uint16*)RomFixedPtr(0xa2ea56))
#define g_off_A2EA4E ((uint16*)RomFixedPtr(0xa2ea4e))
#define g_off_A2EC3A ((uint16*)RomFixedPtr(0xa2ec3a))
#define g_off_A2EDFB ((uint16*)RomFixedPtr(0xa2edfb))
#define g_off_A2F107 ((uint16*)RomFixedPtr(0xa2f107))




static const uint16 g_word_A28D50 = 0x30;
static const uint16 g_word_A28D52 = 1;
static const uint16 g_word_A28D5E = 3;
static const uint16 g_word_A28D54 = 0x20;
static const uint16 g_word_A28D60 = 0x1e8;
static const uint16 g_word_A28D62 = 7;
static const uint16 g_word_A28D64 = 0x1e;
static const uint16 g_word_A28D66 = 4;
static const uint16 g_word_A28D68 = 0xfffd;
static const uint16 g_word_A28D6A = 3;

static const int16 g_word_A28E80[48] = {
  -16, -16, -16, -16, -15, -15, -15, -15,
  -15, -14, -13, -13, -12, -11, -10,  -9,
   -8,  -7,  -6,  -5,  -4,  -4,   0,   0,
  -16, -16, -16, -15, -15, -15, -14, -13,
  -12, -11, -10,  -9,  -8,  -7,  -6,  -5,
   -4,  -3,  -3,  -2,   0,   0,   0,   0,
};

void Enemy_GrappleReact_CancelBeam_A2(void) {  // 0xA2800F
  Enemy_SwitchToFrozenAi();
}

void Enemy_GrappleReact_KillEnemy_A2(void) {  // 0xA2800A
  EnemyGrappleDeath();
}

void Enemy_GrappleReact_SamusLatchesOn_A2(void) {  // 0xA28005
  SamusLatchesOnWithGrapple();
}

void Enemy_NormalFrozenAI_A2(void) {  // 0xA28041
  NormalEnemyFrozenAI();
}

void Enemy_NormalPowerBombAI_A2(void) {  // 0xA28037
  NormalEnemyPowerBombAi();
}

void Enemy_NormalTouchAI_A2(void) {  // 0xA28023
  NormalEnemyTouchAi();
}

void Enemy_NormalShotAI_A2(void) {  // 0xA2802D
  NormalEnemyShotAi();
}

void BouncingGoofball_Init(void) {  // 0xA2871C
  Enemy_BouncingGoofball *E = Get_BouncingGoofball(cur_enemy_index);
  E->base.spritemap_pointer = addr_kSpritemap_Nothing_A2;
  BouncingGoofball_Func2();
  E->bgl_var_01 = 1;
  E->bgl_var_A = kEnemyInit_BouncingGoofball_Tab0[(uint16)(2
                                                           * LOBYTE(E->bgl_parameter_1)) >> 1];
  E->bgl_var_C = kEnemyInit_BouncingGoofball_Tab1[(uint16)(2
                                                           * HIBYTE(E->bgl_parameter_1)) >> 1];
  E->bgl_var_D = 0;
  E->bgl_var_E = 0;
  E->bgl_var_F = 0;
}

void BouncingGoofball_Func1(uint16 k) {  // 0xA28755
  Enemy_BouncingGoofball *E = Get_BouncingGoofball(k);
  uint16 v5;
  do {
    uint16 bgl_var_E = E->bgl_var_E;
    uint8 v3 = kBouncingGoofball_Tab0[bgl_var_E];
    if ((int16)(bgl_var_E - 23) >= 0)
      v3 = -1;
    uint16 prod = Mult8x8(v3, E->bgl_var_A);
    uint16 RegWord = prod;
    E->bgl_var_B = RegWord;
    v5 = E->bgl_var_D + RegWord;
    E->bgl_var_D = v5;
    ++E->bgl_var_E;
  } while ((int16)(v5 - E->bgl_var_C) < 0);
  E->bgl_var_00 = E->bgl_var_E;
  E->bgl_var_02 = 1;
  E->bgl_var_04 = 1;
}

static Func_V *const off_A28718[2] = { BouncingGoofball_State0, BouncingGoofball_State1 };

void BouncingGoofball_Main(void) {  // 0xA2879C
  Enemy_BouncingGoofball *E = Get_BouncingGoofball(cur_enemy_index);
  if (!E->bgl_var_05) {
    BouncingGoofball_Func1(cur_enemy_index);
    E->bgl_var_05 = 1;
    return;
  }
  E->bgl_var_03 = E->bgl_var_A;
  if (BouncingGoofball_SamusCloseX()) {
    E->bgl_var_02 = 0;
    E->bgl_var_04 = 0;
    if (!E->bgl_var_F) {
      E->bgl_var_F = 1;
      BouncingGoofball_Func3();
    }
    goto LABEL_9;
  }
  if (!E->bgl_var_02) {
LABEL_9:
    E->bgl_var_A = E->bgl_var_03;
    off_A28718[E->bgl_var_01]();
    return;
  }
  if (!E->bgl_var_04) {
    E->bgl_var_04 = 1;
    BouncingGoofball_Func2();
  }
}

void BouncingGoofball_State0(void) {  // 0xA28801
  Enemy_BouncingGoofball *E = Get_BouncingGoofball(cur_enemy_index);
  uint16 v1 = ++E->bgl_var_E;
  uint8 v2 = kBouncingGoofball_Tab0[v1];
  if ((int16)(v1 - 23) >= 0)
    v2 = -1;
  uint16 RegWord = Mult8x8(v2, E->bgl_var_A);
  E->bgl_var_B = RegWord;
  E->base.y_pos += HIBYTE(RegWord);
  if ((int16)(E->bgl_var_E - E->bgl_var_00) >= 0) {
    E->bgl_var_01 = 1;
    E->bgl_var_02 = 1;
    E->bgl_var_F = 0;
  }
}

void BouncingGoofball_State1(void) {  // 0xA28850
  Enemy_BouncingGoofball *E = Get_BouncingGoofball(cur_enemy_index);
  uint16 bgl_var_E = E->bgl_var_E;
  uint8 v2 = kBouncingGoofball_Tab0[bgl_var_E];
  if ((int16)(bgl_var_E - 23) >= 0)
    v2 = -1;
  uint16 RegWord = Mult8x8(v2, E->bgl_var_A);
  E->bgl_var_B = RegWord;
  E->base.y_pos -= HIBYTE(RegWord);
  if ((--E->bgl_var_E & 0x8000) != 0)
    E->bgl_var_01 = 0;
}

uint16 BouncingGoofball_SamusCloseX(void) {  // 0xA28894
  Enemy_BouncingGoofball *E = Get_BouncingGoofball(cur_enemy_index);
  return IsSamusWithinEnemy_X(cur_enemy_index, E->bgl_parameter_2);
}

void BouncingGoofball_Func2(void) {  // 0xA2889F
  Enemy_BouncingGoofball *E = Get_BouncingGoofball(cur_enemy_index);
  E->base.current_instruction = addr_kBouncingGoofball_Ilist_86A7;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
}

void BouncingGoofball_Func3(void) {  // 0xA288B2
  Enemy_BouncingGoofball *E = Get_BouncingGoofball(cur_enemy_index);
  E->base.current_instruction = addr_kBouncingGoofball_Ilist_86BF;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
}

const uint16 *BouncingGoofball_Instr_88C5(uint16 k, const uint16 *jp) {  // 0xA288C5
  return jp;
}

const uint16 *BouncingGoofball_Instr_88C6(uint16 k, const uint16 *jp) {  // 0xA288C6
  Get_BouncingGoofball(cur_enemy_index)->bgl_var_02 = 0;
  QueueSfx2_Max6(0xE);
  return jp;
}

const uint16 *MiniCrocomire_Instr_897E(uint16 k, const uint16 *jp) {  // 0xA2897E
  SpawnEprojWithGfx(*jp, cur_enemy_index, addr_Eproj_DBF2);
  return jp + 1;
}

const uint16 *MiniCrocomire_Instr_8990(uint16 k, const uint16 *jp) {  // 0xA28990
  Enemy_MiniCrocomire *E = Get_MiniCrocomire(cur_enemy_index);
  E->mce_var_F = FUNC16(MiniCrocomire_PreInstr5);
  E->mce_var_E = 0;
  return jp;
}

const uint16 *MiniCrocomire_Instr_899D(uint16 k, const uint16 *jp) {  // 0xA2899D
  Enemy_MiniCrocomire *E = Get_MiniCrocomire(cur_enemy_index);
  E->mce_var_F = FUNC16(MiniCrocomire_PreInstr6);
  E->mce_var_E = 1;
  return jp;
}

void MiniCrocomire_Init(void) {  // 0xA289AD
  Enemy_MiniCrocomire *E = Get_MiniCrocomire(cur_enemy_index);
  E->base.spritemap_pointer = addr_kSpritemap_Nothing_A2;
  int v1 = (8 * E->mce_parameter_2) >> 1;
  E->mce_var_B = kCommonEnemySpeeds_Linear[v1];
  E->mce_var_A = kCommonEnemySpeeds_Linear[v1 + 1];
  E->mce_var_D = kCommonEnemySpeeds_Linear[v1 + 2];
  E->mce_var_C = kCommonEnemySpeeds_Linear[v1 + 3];
  MiniCrocomire_Func1();
  E->mce_var_F = FUNC16(MiniCrocomire_PreInstr5);
  uint16 mce_parameter_1 = E->mce_parameter_1;
  E->mce_var_E = mce_parameter_1;
  if (mce_parameter_1) {
    MiniCrocomire_Func3();
    E->mce_var_F = FUNC16(MiniCrocomire_PreInstr6);
  }
}

void MiniCrocomire_Main(void) {  // 0xA289F0
  Enemy_MiniCrocomire *E = Get_MiniCrocomire(cur_enemy_index);
  CallEnemyPreInstr(E->mce_var_F | 0xA20000);
}

void MiniCrocomire_Func1(void) {  // 0xA289F7
  Enemy_MiniCrocomire *E = Get_MiniCrocomire(cur_enemy_index);
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->base.current_instruction = addr_kMiniCrocomire_Ilist_8932;
}

void MiniCrocomire_Func2(void) {  // 0xA28A0A
  Enemy_MiniCrocomire *E = Get_MiniCrocomire(cur_enemy_index);
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->base.current_instruction = addr_kMiniCrocomire_Ilist_8948;
}

void MiniCrocomire_Func3(void) {  // 0xA28A1D
  Enemy_MiniCrocomire *E = Get_MiniCrocomire(cur_enemy_index);
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->base.current_instruction = addr_kMiniCrocomire_Ilist_8958;
}

void MiniCrocomire_Func4(void) {  // 0xA28A30
  Enemy_MiniCrocomire *E = Get_MiniCrocomire(cur_enemy_index);
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->base.current_instruction = addr_kMiniCrocomire_Ilist_896E;
}

void MiniCrocomire_PreInstr5(uint16 k) {  // 0xA28A43
  Enemy_MiniCrocomire *E = Get_MiniCrocomire(cur_enemy_index);
  MiniCrocomire_Func7(__PAIR32__(E->mce_var_D, E->mce_var_C));
  if (MiniCrocomire_Func9())
    MiniCrocomire_Func2();
}

void MiniCrocomire_PreInstr6(uint16 k) {  // 0xA28A5C
  Enemy_MiniCrocomire *E = Get_MiniCrocomire(cur_enemy_index);
  MiniCrocomire_Func7(__PAIR32__(E->mce_var_B, E->mce_var_A));
  if (MiniCrocomire_Func9())
    MiniCrocomire_Func4();
}

void MiniCrocomire_Func7(int32 amt) {  // 0xA28A76
  if (!Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, amt)) {
    if (EnemyFunc_BC76(cur_enemy_index, INT16_SHL16(2)))
      return;
  }
  MiniCrocomire_Func8();
}

void MiniCrocomire_Func8(void) {  // 0xA28A95
  MiniCrocomire_Func1();
  if (Get_MiniCrocomire(cur_enemy_index)->mce_var_E != 1)
    MiniCrocomire_Func3();
}

uint8 MiniCrocomire_Func9(void) {  // 0xA28AA7
  NextRandom();
  Enemy_MiniCrocomire *E = Get_MiniCrocomire(cur_enemy_index);
  if (sign16((uint8)(LOBYTE(E->base.frame_counter) + random_number) - 2)) {
    E->mce_var_F = FUNC16(nullsub_175);
    return 1;
  }
  return 0;
}

void MaridiaBeybladeTurtle_Init(void) {  // 0xA28D6C
  Enemy_MaridiaBeybladeTurtle *E = Get_MaridiaBeybladeTurtle(cur_enemy_index);
  E->base.properties |= kEnemyProps_DisableSamusColl;
  E->base.spritemap_pointer = addr_kSpritemap_Nothing_A2;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->base.y_height = 0;
  E->base.current_instruction = addr_kMaridiaBeybladeTurtle_Ilist_8C44;
  E->mbte_var_A = FUNC16(MaridiaBeybladeTurtle_Func1);
  E->mbte_var_F = g_word_A28D52;
}

void MiniMaridiaBeybladeTurtle_Init(void) {  // 0xA28D9D
  int16 mmbte_parameter_1;

  Enemy_MiniMaridiaBeybladeTurtle *E = Get_MiniMaridiaBeybladeTurtle(cur_enemy_index);
  E->mmbte_var_C = E->base.x_pos;
  E->mmbte_var_D = E->base.y_pos
    - E->base.y_height;
  E->mmbte_var_A = FUNC16(MiniMaridiaBeybladeTurtle_Func1);
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  uint16 v1 = addr_kMaridiaBeybladeTurtle_Ilist_8B80;
  mmbte_parameter_1 = E->mmbte_parameter_1;
  E->mmbte_var_E = mmbte_parameter_1;
  if (mmbte_parameter_1 >= 0)
    v1 = addr_kMaridiaBeybladeTurtle_Ilist_8C72;
  E->base.current_instruction = v1;
}

void CallMaridiaBeybladeTurtleFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnMaridiaBeybladeTurtle_Func1: MaridiaBeybladeTurtle_Func1(k); return;
  case fnMaridiaBeybladeTurtle_Func2: MaridiaBeybladeTurtle_Func2(k); return;
  case fnMaridiaBeybladeTurtle_Func3: MaridiaBeybladeTurtle_Func3(k); return;
  case fnMaridiaBeybladeTurtle_Func5: MaridiaBeybladeTurtle_Func5(k); return;
  case fnMaridiaBeybladeTurtle_Func7: MaridiaBeybladeTurtle_Func7(k); return;
  case fnMaridiaBeybladeTurtle_Func8: MaridiaBeybladeTurtle_Func8(k); return;
  case fnMaridiaBeybladeTurtle_Func9: MaridiaBeybladeTurtle_Func9(k); return;
  case fnMaridiaBeybladeTurtle_Func10: MaridiaBeybladeTurtle_Func10(k); return;
  case fnMaridiaBeybladeTurtle_Func11: MaridiaBeybladeTurtle_Func11(k); return;
  case fnnullsub_360: return;
  case fnMiniMaridiaBeybladeTurtle_Func1: MiniMaridiaBeybladeTurtle_Func1(k); return;
  case fnMiniMaridiaBeybladeTurtle_Func2: MiniMaridiaBeybladeTurtle_Func2(k); return;
  case fnMiniMaridiaBeybladeTurtle_Func3: MiniMaridiaBeybladeTurtle_Func3(k); return;
  case fnMiniMaridiaBeybladeTurtle_Func4: MiniMaridiaBeybladeTurtle_Func4(k); return;
  case fnMiniMaridiaBeybladeTurtle_Func6: MiniMaridiaBeybladeTurtle_Func6(k); return;
  case fnMiniMaridiaBeybladeTurtle_Func7: MiniMaridiaBeybladeTurtle_Func7(); return;
  default: Unreachable();
  }
}

void MaridiaBeybladeTurtle_Main(void) {  // 0xA28DD2
  Enemy_MaridiaBeybladeTurtle *E = Get_MaridiaBeybladeTurtle(cur_enemy_index);
  CallMaridiaBeybladeTurtleFunc(E->mbte_var_A | 0xA20000, cur_enemy_index);
}

void MaridiaBeybladeTurtle_Func1(uint16 k) {  // 0xA28DD8
  uint16 palette_index = Get_MaridiaBeybladeTurtle(k)->base.palette_index;
  Get_MaridiaBeybladeTurtle(k + 64)->base.palette_index = palette_index;
  Get_MaridiaBeybladeTurtle(k + 128)->base.palette_index = palette_index;
  Get_MaridiaBeybladeTurtle(k + 192)->base.palette_index = palette_index;
  Get_MaridiaBeybladeTurtle(k + 256)->base.palette_index = palette_index;
  uint16 vram_tiles_index = Get_MaridiaBeybladeTurtle(k)->base.vram_tiles_index;
  Enemy_MaridiaBeybladeTurtle *E1 = Get_MaridiaBeybladeTurtle(k + 64);
  E1->base.vram_tiles_index = vram_tiles_index;
  Enemy_MaridiaBeybladeTurtle *E2 = Get_MaridiaBeybladeTurtle(k + 128);
  E2->base.vram_tiles_index = vram_tiles_index;
  Enemy_MaridiaBeybladeTurtle *E3 = Get_MaridiaBeybladeTurtle(k + 192);
  E3->base.vram_tiles_index = vram_tiles_index;
  Enemy_MaridiaBeybladeTurtle *E4 = Get_MaridiaBeybladeTurtle(k + 256);
  E4->base.vram_tiles_index = vram_tiles_index;
  E1->mbte_var_B = k;
  E2->mbte_var_B = k;
  E3->mbte_var_B = k;
  E4->mbte_var_B = k;
  Get_MaridiaBeybladeTurtle(k)->mbte_var_A = FUNC16(MaridiaBeybladeTurtle_Func2);
}

void MaridiaBeybladeTurtle_Func2(uint16 k) {  // 0xA28E0A
  Enemy_MaridiaBeybladeTurtle *E = Get_MaridiaBeybladeTurtle(k);
  if (E->mbte_var_F) {
    E->base.y_height = 0;
    uint16 xd = E->base.x_pos - samus_x_pos;
    uint16 v2 = abs16(xd);
    if (sign16(v2 - 24)) {
      if (sign16(xd))
        v2 += 24;
      uint16 v3 = cur_enemy_index;
      E->base.y_height = -g_word_A28E80[v2];
      E->base.properties |= 0x8000;
      if (CheckIfEnemyTouchesSamus(v3)) {
        uint16 r18 = E->base.y_pos - E->base.y_height;
        if ((int16)(samus_y_radius + samus_y_pos - r18) >= 0)
          extra_samus_y_displacement += r18 - (samus_y_radius + samus_y_pos);
      }
    }
  } else {
    E->mbte_var_A = FUNC16(MaridiaBeybladeTurtle_Func3);
    E->base.properties &= ~kEnemyProps_Tangible;
  }
}

void MaridiaBeybladeTurtle_Func3(uint16 k) {  // 0xA28EE0
  MaridiaBeybladeTurtle_Func4();
  if ((nmi_frame_counter_byte & 1) == 0) {
    if (CheckIfEnemyTouchesSamus(k))
      --extra_samus_x_displacement;
    Enemy_MaridiaBeybladeTurtle *E = Get_MaridiaBeybladeTurtle(k);
    --E->base.y_pos;
    E->base.y_height = 16;
    if ((E->base.y_pos & 1) != 0)
      --E->base.x_pos;
    else
      ++E->base.x_pos;
    if (!(Enemy_MoveRight_IgnoreSlopes(k, INT16_SHL16(1)))) {
      E->base.current_instruction = addr_kMaridiaBeybladeTurtle_Ilist_8C4A;
      E->base.instruction_timer = 1;
      //*((uint16 *)RomPtr_A2(k) + 3) = g_word_A28D54; // WTF?
      E->mbte_var_A = addr_locret_A28E09;
    }
  }
}

void MaridiaBeybladeTurtle_Func5(uint16 k) {  // 0xA28F3F
  uint16 v1 = addr_kMaridiaBeybladeTurtle_Ilist_8C1C;
  Enemy_MaridiaBeybladeTurtle *E = Get_MaridiaBeybladeTurtle(k);
  if ((int16)(E->base.x_pos - samus_x_pos) < 0)
    v1 = addr_kMaridiaBeybladeTurtle_Ilist_8D00;
  E->base.current_instruction = v1;
  E->base.instruction_timer = 1;
  E->mbte_var_A = addr_locret_A28E09;
}

void MaridiaBeybladeTurtle_Func6(uint16 k) {  // 0xA28F5F
  if (CheckIfEnemyTouchesSamus(k)) {
    Enemy_MaridiaTurtle *E = Get_MaridiaTurtle(k);
    E->mte_var_A = FUNC16(MaridiaBeybladeTurtle_Func9);
    AddToHiLo(&extra_samus_x_displacement, &extra_samus_x_subdisplacement, -IPAIR32(E->mte_var_E, E->mte_var_03));
    if (sign16(extra_samus_x_displacement + 16))
      extra_samus_x_displacement = -16;
  }
}

void MaridiaBeybladeTurtle_Func7(uint16 k) {  // 0xA28F8D
  MaridiaBeybladeTurtle_Func4();
  if (!(Enemy_MoveDown(k, INT16_SHL16(-1)))) {
    if (CheckIfEnemyTouchesSamus(k))
      --extra_samus_y_displacement;
    Enemy_MaridiaTurtle *E = Get_MaridiaTurtle(k);
    uint16 v2 = E->mte_var_00 - 1;
    E->mte_var_00 = v2;
    if (!v2) {
      uint16 v3 = 0;
      if ((int16)(E->base.x_pos - samus_x_pos) < 0)
        v3 = 4;
      E->mte_var_01 = *(uint16 *)((uint8 *)&g_word_A28D56 + v3);
      E->mte_var_02 = *(uint16 *)((uint8 *)&g_word_A28D58 + v3);
      E->mte_var_E = 0;
      E->mte_var_03 = 0;
      E->mte_var_A = FUNC16(MaridiaBeybladeTurtle_Func8);
    }
  }
}

void MaridiaBeybladeTurtle_Func8(uint16 k) {  // 0xA28FEB
  MaridiaBeybladeTurtle_Func4();
  Enemy_MaridiaTurtle *E = Get_MaridiaTurtle(k);
  if (Enemy_MoveRight_IgnoreSlopes(k, __PAIR32__(E->mte_var_E, E->mte_var_03))) {
    SetHiLo(&E->mte_var_E, &E->mte_var_03, -IPAIR32(E->mte_var_E, E->mte_var_03));
    SetHiLo(&E->mte_var_02, &E->mte_var_01, -IPAIR32(E->mte_var_02, E->mte_var_01));
    earthquake_type = 0;
    earthquake_timer = 16;
    QueueSfx2_Max6(0x1B);
  } else {
    MaridiaBeybladeTurtle_Func6(k);
    AddToHiLo(&E->mte_var_E, &E->mte_var_03, __PAIR32__(E->mte_var_02, E->mte_var_01));
    if ((int16)(abs16(E->mte_var_E) - g_word_A28D5E) >= 0)
      E->mte_var_E = (E->mte_var_E & 0x8000) != 0 ? -g_word_A28D5E : g_word_A28D5E;
  }
}

void MaridiaBeybladeTurtle_Func9(uint16 k) {  // 0xA29083
  Enemy_MaridiaTurtle *E = Get_MaridiaTurtle(k);
  uint16 v2;
  MaridiaBeybladeTurtle_Func4();
  if ((int16)(E->base.y_pos - g_word_A28D60) < 0) {
    E->mte_var_00 = g_word_A28D64;
    v2 = FUNC16(MaridiaBeybladeTurtle_Func10);
  } else {
    if (CheckIfEnemyTouchesSamus(k)) {
      E->base.y_pos -= g_word_A28D62;
      extra_samus_y_displacement -= g_word_A28D62;
      return;
    }
    v2 = FUNC16(MaridiaBeybladeTurtle_Func11);
  }
  E->mte_var_A = v2;
  E->mte_var_07 = 0;
  E->mte_var_04 = 0;
}

void MaridiaBeybladeTurtle_Func10(uint16 k) {  // 0xA290CC
  MaridiaBeybladeTurtle_Func4();
  Enemy_MaridiaTurtle *E = Get_MaridiaTurtle(k);
  uint16 v2 = E->mte_var_00 - 1;
  E->mte_var_00 = v2;
  if (!v2)
    E->mte_var_A = FUNC16(MaridiaBeybladeTurtle_Func11);
}

void MaridiaBeybladeTurtle_Func11(uint16 k) {  // 0xA290E1
  MaridiaBeybladeTurtle_Func4();
  Enemy_MaridiaTurtle *E = Get_MaridiaTurtle(k);
  if ((int16)(Get_MaridiaTurtle(0)->mte_var_04 - g_word_A28D66) < 0)
    AddToHiLo(&E->mte_var_04, &E->mte_var_07, 0x2000);
  if (Enemy_MoveDown(k, INT16_SHL16(E->mte_var_04))) {
    E->base.current_instruction = (E->mte_var_E & 0x8000) == 0 ?
        addr_kMaridiaBeybladeTurtle_Ilist_8D28 : addr_kMaridiaBeybladeTurtle_Ilist_8C4A;
    E->base.instruction_timer = 1;
    E->mte_var_A = addr_locret_A28E09;
  }
}

void MiniMaridiaBeybladeTurtle_Main(void) {  // 0xA2912E
  Enemy_MiniMaridiaTurtle *E = Get_MiniMaridiaTurtle(cur_enemy_index);
  Get_MiniMaridiaTurtle(E->mmte_var_B)->mmte_var_06 = 0;
  CallMaridiaBeybladeTurtleFunc(E->mmte_var_A | 0xA20000, cur_enemy_index);
}

void MiniMaridiaBeybladeTurtle_Func1(uint16 k) {  // 0xA29142
  if (CheckIfEnemyTouchesSamus(k)) {
    Enemy_MiniMaridiaTurtle *E = Get_MiniMaridiaTurtle(k);
    E->mmte_var_A = FUNC16(MiniMaridiaBeybladeTurtle_Func2);
    E->mmte_var_05 = 4;
    uint16 v2 = addr_kMaridiaBeybladeTurtle_Ilist_8C30;
    if ((E->mmte_var_E & 0x8000) == 0)
      v2 = addr_kMaridiaBeybladeTurtle_Ilist_8D14;
    E->base.current_instruction = v2;
    E->base.instruction_timer = 1;
  }
}

void MiniMaridiaBeybladeTurtle_Func2(uint16 k) {  // 0xA2916E
  Enemy_MiniMaridiaTurtle *E = Get_MiniMaridiaTurtle(k);
  if (CheckIfEnemyTouchesSamus(k)) {
    E->mmte_var_05 = 4;
  } else {
    uint16 v2 = E->mmte_var_05 - 1;
    E->mmte_var_05 = v2;
    if (!v2) {
      E->mmte_var_A = FUNC16(MiniMaridiaBeybladeTurtle_Func3);
      E->mmte_var_00 = 60;
    }
  }
}

void MiniMaridiaBeybladeTurtle_Func3(uint16 k) {  // 0xA29198
  Enemy_MiniMaridiaTurtle *E = Get_MiniMaridiaTurtle(k);
  if (CheckIfEnemyTouchesSamus(k)) {
    E->mmte_var_A = FUNC16(MiniMaridiaBeybladeTurtle_Func4);
    E->base.current_instruction = addr_kMaridiaBeybladeTurtle_Ilist_8BD2;
    E->base.instruction_timer = 1;
    E->mmte_var_04 = 1;
    uint16 v2 = g_word_A28D68;
    if ((samus_pose_x_dir & 0xF) == 8)
      v2 = g_word_A28D6A;
    E->mmte_var_E = v2;
  } else {
    uint16 v4 = E->mmte_var_00 - 1;
    E->mmte_var_00 = v4;
    if (!v4) {
      uint16 v5 = addr_kMaridiaBeybladeTurtle_Ilist_8B80;
      if ((E->mmte_var_E & 0x8000) == 0)
        v5 = addr_kMaridiaBeybladeTurtle_Ilist_8C72;
      E->base.current_instruction = v5;
      E->base.instruction_timer = 1;
      E->mmte_var_A = FUNC16(MiniMaridiaBeybladeTurtle_Func1);
    }
  }
}

void MiniMaridiaBeybladeTurtle_Func4(uint16 k) {  // 0xA291F8
  Enemy_MiniMaridiaTurtle *E = Get_MiniMaridiaTurtle(k);
  if (Enemy_MoveRight_IgnoreSlopes(k, __PAIR32__(E->mmte_var_E, 0))) {
    E->mmte_var_E = -E->mmte_var_E;
  } else {
    Enemy_MoveDown(k, INT16_SHL16(E->mmte_var_04));
  }
}

void MiniMaridiaBeybladeTurtle_Func5(uint16 k) {  // 0xA2921D
  uint16 v1 = addr_kMaridiaBeybladeTurtle_Ilist_8B80;
  Enemy_MiniMaridiaBeybladeTurtle *E = Get_MiniMaridiaBeybladeTurtle(k);
  if ((E->mmbte_var_E & 0x8000) == 0)
    v1 = addr_kMaridiaBeybladeTurtle_Ilist_8C72;
  E->base.current_instruction = v1;
  E->base.instruction_timer = 1;
  E->mmbte_var_A = FUNC16(MiniMaridiaBeybladeTurtle_Func1);
}

void MiniMaridiaBeybladeTurtle_Func6(uint16 k) {  // 0xA29239
  if (CheckIfEnemyTouchesSamus(k)) {
    uint16 v1 = addr_kMaridiaBeybladeTurtle_Ilist_8B80;
    Enemy_MiniMaridiaBeybladeTurtle *E = Get_MiniMaridiaBeybladeTurtle(k);
    if ((E->mmbte_var_E & 0x8000) == 0)
      v1 = addr_kMaridiaBeybladeTurtle_Ilist_8C72;
    E->base.current_instruction = v1;
    E->base.instruction_timer = 1;
    E->mmbte_var_A = FUNC16(MiniMaridiaBeybladeTurtle_Func1);
  } else {
    MiniMaridiaBeybladeTurtle_Func4(k);
  }
}

void MiniMaridiaBeybladeTurtle_Func7(void) {  // 0xA2925E
  Enemy_MiniMaridiaTurtle *E = Get_MiniMaridiaTurtle(cur_enemy_index);
  Get_MiniMaridiaTurtle(E->mmte_var_B)->mmte_var_06 = E->base.y_height;
  if (!CheckIfEnemyTouchesSamus(cur_enemy_index))
    Get_MiniMaridiaTurtle(cur_enemy_index)->mmte_var_A = FUNC16(MiniMaridiaBeybladeTurtle_Func1);
}

void MaridiaBeybladeTurtle_Touch(void) {  // 0xA29281
  Enemy_MiniMaridiaTurtle *E = Get_MiniMaridiaTurtle(cur_enemy_index);
  if ((E->base.properties & 0x8000) == 0) {
    Enemy_NormalTouchAI_A2();
    E->mmte_var_A = FUNC16(MaridiaBeybladeTurtle_Func11);
    E->mmte_var_04 = 2;
  }
}

void MiniMaridiaBeybladeTurtle_Touch(void) {  // 0xA2929F
  int16 v2;

  Enemy_MiniMaridiaBeybladeTurtle *E = Get_MiniMaridiaBeybladeTurtle(cur_enemy_index);
  if (E->mmbte_var_A != 0x925E) {
    if ((E->mmbte_var_E & 0x8000) != 0) {
      E->base.current_instruction = addr_kMaridiaBeybladeTurtle_Ilist_8C72;
      v2 = 1;
    } else {
      E->base.current_instruction = addr_kMaridiaBeybladeTurtle_Ilist_8B80;
      v2 = -1;
    }
    E->mmbte_var_E = v2;
    E->base.instruction_timer = 1;
    uint16 v3;
    if ((int16)(E->base.x_pos - samus_x_pos) >= 0)
      v3 = E->base.x_width + samus_x_radius + samus_x_pos;
    else
      v3 = (__PAIR32__(samus_x_pos - samus_x_radius, samus_x_pos)
            - __PAIR32__(E->base.x_width, samus_x_radius)) >> 16;
    E->base.x_pos = v3;
    E->mmbte_var_A = FUNC16(MiniMaridiaBeybladeTurtle_Func1);
    Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, __PAIR32__(E->mmbte_var_E, 0));
    MaridiaBeybladeTurtle_Func21();
  }
}

void MaridiaBeybladeTurtle_Func21(void) {  // 0xA292FF
  uint16 mbte_var_B = Get_MaridiaBeybladeTurtle(cur_enemy_index)->mbte_var_B;
  Enemy_MaridiaBeybladeTurtle *E = Get_MaridiaBeybladeTurtle(mbte_var_B);
  if (E->mbte_var_F)
    --E->mbte_var_F;
}

void MiniMaridiaBeybladeTurtle_Shot(void) {  // 0xA2930F
  NormalEnemyShotAi();
  MaridiaBeybladeTurtle_Func21();
}

void MaridiaBeybladeTurtle_Func4(void) {  // 0xA29315
  Enemy_MaridiaBeybladeTurtle *E = Get_MaridiaBeybladeTurtle(0);
  uint16 r18 = E->base.x_pos - E->base.x_width - 8;
  uint16 r20 = E->base.x_width + E->base.x_pos + 8;
  uint16 r22 = E->base.y_pos - E->base.y_height + 4;
  uint16 r24 = E->base.y_height + E->base.y_pos - 4;
  if (sign16(samus_x_pos - samus_x_radius - 1 - r20)
      && !sign16(samus_x_radius + samus_x_pos - r18)
      && sign16(samus_y_pos - samus_y_radius + 1 - r24)
      && !sign16(samus_y_radius + samus_y_pos - r22)
      && !samus_invincibility_timer) {
    NormalEnemyTouchAi();
  }
}

const uint16 *MaridiaBeybladeTurtle_Instr_9381(uint16 k, const uint16 *jp) {  // 0xA29381
  uint16 R48 = 0;
  if (CheckIfEnemyTouchesSamus(cur_enemy_index)) {
    extra_samus_x_displacement += Get_MaridiaBeybladeTurtle(cur_enemy_index)->mbte_var_E;
    R48 = 1;
  }
  Enemy_MaridiaBeybladeTurtle *E = Get_MaridiaBeybladeTurtle(cur_enemy_index);
  uint16 R50 = E->base.y_pos;
  E->base.y_pos = E->mbte_var_D;
  Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, INT16_SHL16(E->mbte_var_E));
  if (Get_MaridiaBeybladeTurtle(E->mbte_var_B)->mbte_var_A == 0x8E0A) {
    uint16 x_pos = Get_MaridiaBeybladeTurtle(E->mbte_var_B)->base.x_pos;
    uint16 xd = x_pos - E->base.x_pos;
    uint16 v7 = abs16(xd);
    uint16 v8;
    if (sign16(v7 - 24)) {
      if (sign16(xd))
        v7 += 24;
      v8 = g_word_A28E80[v7];
    } else {
      v8 = 1;
    }
    Enemy_MoveDown(cur_enemy_index, INT16_SHL16(v8));
    if (R48)
      extra_samus_y_displacement += Get_MaridiaBeybladeTurtle(cur_enemy_index)->base.y_pos - R50;
  }
  return jp;
}

const uint16 *MaridiaBeybladeTurtle_Instr_9412(uint16 k, const uint16 *jp) {  // 0xA29412
  int16 v4;

  Enemy_MaridiaBeybladeTurtle *E = Get_MaridiaBeybladeTurtle(cur_enemy_index);
  uint16 xd = E->mbte_var_C - E->base.x_pos;
  if ((int16)(abs16(xd) - g_word_A28D50) >= 0) {
    if (!sign16(xd))
      v4 = 1;
    else
      v4 = -1;
    E->mbte_var_E = v4;
  }
  if ((Get_MaridiaBeybladeTurtle(cur_enemy_index)->mbte_var_E & 0x8000) != 0)
    return INSTR_RETURN_ADDR(addr_kMaridiaBeybladeTurtle_Ilist_8B80);
  return INSTR_RETURN_ADDR(addr_kMaridiaBeybladeTurtle_Ilist_8C72);
}

const uint16 *MaridiaBeybladeTurtle_Instr_9447(uint16 k, const uint16 *jp) {  // 0xA29447
  Get_MaridiaBeybladeTurtle(cur_enemy_index)->mbte_var_A = FUNC16(MaridiaBeybladeTurtle_Func5);
  return jp;
}

const uint16 *MaridiaBeybladeTurtle_Instr_9451(uint16 k, const uint16 *jp) {  // 0xA29451
  Enemy_MaridiaTurtle *E = Get_MaridiaTurtle(cur_enemy_index);
  E->mte_var_A = FUNC16(MaridiaBeybladeTurtle_Func7);
  E->mte_var_E = -1;
  E->mte_var_00 = 16;
  return INSTR_RETURN_ADDR(addr_kMaridiaBeybladeTurtle_Ilist_8C02);
}

const uint16 *MaridiaBeybladeTurtle_Instr_946B(uint16 k, const uint16 *jp) {  // 0xA2946B
  Enemy_MaridiaTurtle *E = Get_MaridiaTurtle(cur_enemy_index);
  E->mte_var_A = FUNC16(MaridiaBeybladeTurtle_Func7);
  E->mte_var_E = 1;
  E->mte_var_00 = 16;
  return INSTR_RETURN_ADDR(addr_kMaridiaBeybladeTurtle_Ilist_8C02);
}

const uint16 *MaridiaBeybladeTurtle_Instr_9485(uint16 k, const uint16 *jp) {  // 0xA29485
  if (CheckIfEnemyTouchesSamus(cur_enemy_index)) {
    if ((Get_MaridiaBeybladeTurtle(cur_enemy_index)->mbte_var_E & 0x8000) == 0)
      return INSTR_RETURN_ADDR(addr_kMaridiaBeybladeTurtle_Ilist_8D40);
    else
      return INSTR_RETURN_ADDR(addr_kMaridiaBeybladeTurtle_Ilist_8C62);
  }
  return jp;
}

const uint16 *MaridiaBeybladeTurtle_Instr_94A1(uint16 k, const uint16 *jp) {  // 0xA294A1
  Enemy_MaridiaBeybladeTurtle *E = Get_MaridiaBeybladeTurtle(cur_enemy_index);
  if (CheckIfEnemyTouchesSamus(cur_enemy_index))
    E->mbte_var_A = FUNC16(MiniMaridiaBeybladeTurtle_Func7);
  else
    E->mbte_var_A = FUNC16(MiniMaridiaBeybladeTurtle_Func1);
  if ((E->mbte_var_E & 0x8000) == 0)
    return INSTR_RETURN_ADDR(addr_kMaridiaBeybladeTurtle_Ilist_8C72);
  return INSTR_RETURN_ADDR(addr_kMaridiaBeybladeTurtle_Ilist_8B80);
}

const uint16 *MaridiaBeybladeTurtle_Instr_94C7(uint16 k, const uint16 *jp) {  // 0xA294C7
  Enemy_MaridiaBeybladeTurtle *E = Get_MaridiaBeybladeTurtle(cur_enemy_index);
  E->mbte_var_A = FUNC16(MiniMaridiaBeybladeTurtle_Func6);
  return jp;
}

const uint16 *MaridiaBeybladeTurtle_Instr_94D1(uint16 k, const uint16 *jp) {  // 0xA294D1
  QueueSfx2_Max6(0x3A);
  return jp;
}

void ThinHoppingBlobs_Init(void) {  // 0xA29A3F
  Enemy_ThinHoppingBlobs *E = Get_ThinHoppingBlobs(cur_enemy_index);
  E->base.spritemap_pointer = addr_kSpritemap_Nothing_A2;
  E->thbs_var_A = 0;
  sub_A29A6C(addr_kThinHoppingBlobs_Ilist_99AD);
  E->thbs_var_00 = 0;
  E->thbs_var_D = FUNC16(ThinHoppingBlobs_Func6);
  E->thbs_var_C = E->thbs_parameter_1;
  E->thbs_var_04 = 0;
}

void sub_A29A6C(uint16 a) {  // 0xA29A6C
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  v1->current_instruction = a;
  v1->instruction_timer = 1;
  v1->timer = 0;
}

void CallThinHoppingBlobs(uint32 ea) {
  switch (ea) {
  case fnThinHoppingBlobs_Func6: ThinHoppingBlobs_Func6(cur_enemy_index); return;
  case fnThinHoppingBlobs_Func7: ThinHoppingBlobs_Func7(); return;
  default: Unreachable();
  }
}

void ThinHoppingBlobs_Main(void) {  // 0xA29A7D
  Enemy_ThinHoppingBlobs *E = Get_ThinHoppingBlobs(cur_enemy_index);
  CallThinHoppingBlobs(E->thbs_var_D | 0xA20000);
}

void ThinHoppingBlobs_Func1(uint16 k) {  // 0xA29A84
  Enemy_ThinHoppingBlobs *E = Get_ThinHoppingBlobs(k);
  if (sign16(E->thbs_var_00 - 3))
    TwinHoppingBlobs_Func2();
  ThinHoppingBlobs_Func3();
  ThinHoppingBlobs_Func5();
}

void TwinHoppingBlobs_Func2(void) {  // 0xA29A9B
  Enemy_ThinHoppingBlobs *E = Get_ThinHoppingBlobs(cur_enemy_index);
  E->thbs_var_00 = IsSamusWithinEnemy_X(cur_enemy_index, E->thbs_parameter_2);
}

void ThinHoppingBlobs_Func3(void) {  // 0xA29AAA
  Enemy_ThinHoppingBlobs *E = Get_ThinHoppingBlobs(cur_enemy_index);
  E->thbs_var_02 = 1;
  if ((GetSamusEnemyDelta_X(cur_enemy_index) & 0x8000) == 0)
    E->thbs_var_02 = 0;
  if (E->thbs_var_04)
    E->thbs_var_02 = E->thbs_var_05;
  E->thbs_var_04 = 0;
  uint16 R28 = ThinHoppingBlobs_Func4();
  uint16 thbs_var_00 = E->thbs_var_00;
  if (sign16(thbs_var_00 - 3)) {
    if (!thbs_var_00)
      R28 &= 1;
    thbs_var_00 = R28;
    if (!sign16(R28 - 2))
      thbs_var_00 = 2;
  }
  uint16 v3 = 8 * thbs_var_00;
  E->thbs_var_F = v3;
  E->thbs_var_E = *(uint16 *)((uint8 *)&g_word_A29A0D + v3);
}

uint16 ThinHoppingBlobs_Func4(void) {  // 0xA29B06
  NextRandom();
  return (Get_ThinHoppingBlobs(cur_enemy_index)->base.frame_counter + random_number) & 7;
}

void ThinHoppingBlobs_Func5(void) {  // 0xA29B1A
  uint16 v2;
  Enemy_ThinHoppingBlobs *E = Get_ThinHoppingBlobs(cur_enemy_index);
  uint16 thbs_var_F = E->thbs_var_F;
  uint16 r22 = 0;
  uint16 r24 = 0;
  do {
    r22 += *(uint16 *)((uint8 *)&g_word_A29A09 + thbs_var_F);
    r24 += *(uint16 *)((uint8 *)kCommonEnemySpeeds_Quadratic + (8 * ((r22 & 0xFF00) >> 8)) + 1);
    thbs_var_F = E->thbs_var_F;
    v2 = swap16(*(uint16 *)((uint8 *)&g_word_A29A07 + thbs_var_F));
  } while (!sign16(v2 - r24));
  E->thbs_var_B = r22;
  E->thbs_var_03 = 0;
  uint16 v3 = r22 >> 1;
  E->thbs_var_07 = r22 >> 1;
  E->thbs_var_06 = E->thbs_var_07 + (v3 >> 1);
}

void ThinHoppingBlobs_Func6(uint16 k) {  // 0xA29B65
  Enemy_ThinHoppingBlobs *E = Get_ThinHoppingBlobs(k);
  if ((--E->thbs_var_C & 0x8000) != 0) {
    E->thbs_var_D = FUNC16(ThinHoppingBlobs_Func7);
    E->thbs_var_C = E->thbs_parameter_1;
    E->thbs_var_01 = 1;
    ThinHoppingBlobs_Func1(k);
  }
}

void CallThinHoppingBlobsVarE(uint32 ea) {
  switch (ea) {
  case fnThinHoppingBlobs_Func11: ThinHoppingBlobs_Func11(); return;
  case fnThinHoppingBlobs_Func12: ThinHoppingBlobs_Func12(); return;
  case fnThinHoppingBlobs_Func13: ThinHoppingBlobs_Func13(); return;
  case fnThinHoppingBlobs_Func14: ThinHoppingBlobs_Func14(); return;
  case fnThinHoppingBlobs_Func15: ThinHoppingBlobs_Func15(); return;
  case fnThinHoppingBlobs_Func16: ThinHoppingBlobs_Func16(); return;
  default: Unreachable();
  }
}

void ThinHoppingBlobs_Func7(void) {  // 0xA29B81
  Enemy_ThinHoppingBlobs *E = Get_ThinHoppingBlobs(cur_enemy_index);
  CallThinHoppingBlobsVarE(E->thbs_var_E | 0xA20000);
}

void ThinHoppingBlobs_Func8(void) {  // 0xA29B88
  Enemy_ThinHoppingBlobs *E = Get_ThinHoppingBlobs(cur_enemy_index);
  uint16 thbs_var_B = E->thbs_var_B;
  if (!sign16(thbs_var_B - 0x4000))
    thbs_var_B = 0x4000;
  uint16 v6 = 8 * (thbs_var_B >> 8);
  if (!E->thbs_var_03)
    v6 += 4;
  if (Enemy_MoveDown(cur_enemy_index, kCommonEnemySpeeds_Quadratic32[v6 >> 2])) {
    if (E->thbs_var_03) {
      E->thbs_var_D = FUNC16(ThinHoppingBlobs_Func6);
    } else {
      E->thbs_var_04 = 1;
      E->thbs_var_05 = E->thbs_var_02 ^ 1;
      E->thbs_var_00 = 4;
      E->thbs_var_E = FUNC16(ThinHoppingBlobs_Func15);
    }
    E->thbs_var_01 = 0;
  } else {
    uint16 thbs_var_F = E->thbs_var_F;
    if (E->thbs_var_03) {
      ThinHoppingBlobs_Func10();
      E->thbs_var_B += *(uint16 *)((uint8 *)&g_word_A29A0B + thbs_var_F);
    } else {
      ThinHoppingBlobs_Func9();
      E->thbs_var_B -= *(uint16 *)((uint8 *)&g_word_A29A0B + thbs_var_F);
    }
    if (sign16(E->thbs_var_B)) {
      E->thbs_var_03 = 1;
      E->thbs_var_B = 0;
    }
    uint16 R20 = (*(uint16 *)((uint8 *)&g_word_A29A09 + E->thbs_var_F) & 0xFF00) >> 8;
    if (E->thbs_var_02)
      R20 = -R20;
    if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, INT16_SHL16(R20))) {
      E->thbs_var_04 = 1;
      E->thbs_var_05 = E->thbs_var_02 ^ 1;
      E->thbs_var_01 = 0;
      E->thbs_var_00 = 4;
      E->thbs_var_E = FUNC16(ThinHoppingBlobs_Func15);
    }
  }
}

void ThinHoppingBlobs_Func9(void) {  // 0xA29C71
  Enemy_ThinHoppingBlobs *E = Get_ThinHoppingBlobs(cur_enemy_index);
  uint16 v2;
  if (E->thbs_var_02) {
    uint16 thbs_var_B = E->thbs_var_B;
    if ((int16)(thbs_var_B - E->thbs_var_06) >= 0) {
      v2 = addr_kThinHoppingBlobs_Ilist_9A01;
    } else if ((int16)(thbs_var_B - E->thbs_var_07) >= 0) {
      v2 = addr_kThinHoppingBlobs_Ilist_99FB;
    } else {
      v2 = addr_kThinHoppingBlobs_Ilist_99F5;
    }
  } else {
    uint16 v1 = E->thbs_var_B;
    if ((int16)(v1 - E->thbs_var_06) >= 0) {
      v2 = addr_kThinHoppingBlobs_Ilist_99E9;
    } else if ((int16)(v1 - E->thbs_var_07) >= 0) {
      v2 = addr_kThinHoppingBlobs_Ilist_99EF;
    } else {
      v2 = addr_kThinHoppingBlobs_Ilist_99F5;
    }
  }
  sub_A29A6C(v2);
}

void ThinHoppingBlobs_Func10(void) {  // 0xA29CBE
  Enemy_ThinHoppingBlobs *E = Get_ThinHoppingBlobs(cur_enemy_index);
  uint16 v2;
  if (E->thbs_var_02) {
    uint16 thbs_var_B = E->thbs_var_B;
    if ((int16)(thbs_var_B - E->thbs_var_07) < 0) {
      v2 = addr_kThinHoppingBlobs_Ilist_99F5;
    } else if ((int16)(thbs_var_B - E->thbs_var_06) < 0) {
      v2 = addr_kThinHoppingBlobs_Ilist_99EF;
    } else {
      v2 = addr_kThinHoppingBlobs_Ilist_99E9;
    }
  } else {
    uint16 v1 = E->thbs_var_B;
    if ((int16)(v1 - E->thbs_var_07) < 0) {
      v2 = addr_kThinHoppingBlobs_Ilist_99F5;
    } else if ((int16)(v1 - E->thbs_var_06) < 0) {
      v2 = addr_kThinHoppingBlobs_Ilist_99FB;
    } else {
      v2 = addr_kThinHoppingBlobs_Ilist_9A01;
    }
  }
  sub_A29A6C(v2);
}

void ThinHoppingBlobs_Func11(void) {  // 0xA29D0B

  ThinHoppingBlobs_Func8();
  Enemy_ThinHoppingBlobs *E = Get_ThinHoppingBlobs(cur_enemy_index);
  if (E->thbs_var_04 || !E->thbs_var_01) {
    E->thbs_var_01 = 0;
    sub_A29A6C(addr_kThinHoppingBlobs_Ilist_99D5);
  }
}

void ThinHoppingBlobs_Func12(void) {  // 0xA29D2B

  ThinHoppingBlobs_Func8();
  Enemy_ThinHoppingBlobs *E = Get_ThinHoppingBlobs(cur_enemy_index);
  if (E->thbs_var_04 || !E->thbs_var_01) {
    E->thbs_var_01 = 0;
    sub_A29A6C(addr_kThinHoppingBlobs_Ilist_99C1);
  }
}

void ThinHoppingBlobs_Func13(void) {  // 0xA29D4B

  ThinHoppingBlobs_Func8();
  Enemy_ThinHoppingBlobs *E = Get_ThinHoppingBlobs(cur_enemy_index);
  if (E->thbs_var_04 || !E->thbs_var_01) {
    E->thbs_var_01 = 0;
    sub_A29A6C(addr_kThinHoppingBlobs_Ilist_99AD);
  }
}

void ThinHoppingBlobs_Func14(void) {  // 0xA29D6B

  ThinHoppingBlobs_Func8();
  Enemy_ThinHoppingBlobs *E = Get_ThinHoppingBlobs(cur_enemy_index);
  if (!E->thbs_var_04) {
    if (E->thbs_var_01)
      return;
    E->thbs_var_00 = 0;
    E->thbs_var_D = FUNC16(ThinHoppingBlobs_Func6);
  }
  E->thbs_var_01 = 0;
  sub_A29A6C(addr_kThinHoppingBlobs_Ilist_99D5);
}

void ThinHoppingBlobs_Func15(void) {  // 0xA29D98
  Enemy_ThinHoppingBlobs *E = Get_ThinHoppingBlobs(cur_enemy_index);
  uint16 thbs_var_F = E->thbs_var_F;
  uint16 R20 = (*(uint16 *)((uint8 *)&g_word_A29A0B + thbs_var_F) & 0xFF00) >> 8;
  uint16 R18 = *((uint8 *)&g_word_A29A0B + thbs_var_F) << 8;
  if (Enemy_MoveDown(cur_enemy_index, __PAIR32__(R20, R18))) {
    uint16 R28 = ThinHoppingBlobs_Func4();
    E->thbs_var_00 = (R28 & 1) + 5;
    E->thbs_var_D = FUNC16(ThinHoppingBlobs_Func6);
  }
}

void ThinHoppingBlobs_Func16(void) {  // 0xA29DCD

  ThinHoppingBlobs_Func8();
  Enemy_ThinHoppingBlobs *E = Get_ThinHoppingBlobs(cur_enemy_index);
  if (!E->thbs_var_01) {
    E->thbs_var_01 = 0;
    E->thbs_var_00 = 3;
    E->thbs_var_D = FUNC16(ThinHoppingBlobs_Func6);
    sub_A29A6C(addr_kThinHoppingBlobs_Ilist_99D5);
  }
}

const uint16 *SpikeShootingPlant_Instr_9F2A(uint16 k, const uint16 *jp) {  // 0xA29F2A
  QueueSfx2_Max6(0x34);
  return jp;
}

void SpikeShootingPlant_Init(void) {  // 0xA29F48
  Enemy_SpikeShootingPlant *E = Get_SpikeShootingPlant(cur_enemy_index);
  E->base.spritemap_pointer = addr_kSpritemap_Nothing_A2;
  if (HIBYTE(E->sspt_parameter_1))
    SpikeShootingPlant_5();
  else
    SpikeShootingPlant_7();
  uint16 sspt_parameter_1_low = LOBYTE(E->sspt_parameter_1);
  E->sspt_var_E = sspt_parameter_1_low;
  E->sspt_var_F = g_off_A29F42[sspt_parameter_1_low];
  int v2 = LOBYTE(E->sspt_parameter_2);
  E->sspt_var_01 = g_word_A29F36[v2] + E->base.x_pos;
  E->sspt_var_00 = E->base.x_pos - g_word_A29F36[v2];
  int v3 = (8 * HIBYTE(E->sspt_parameter_2)) >> 1;
  E->sspt_var_B = kCommonEnemySpeeds_Linear[v3];
  E->sspt_var_A = kCommonEnemySpeeds_Linear[v3 + 1];
  E->sspt_var_D = kCommonEnemySpeeds_Linear[v3 + 2];
  E->sspt_var_C = kCommonEnemySpeeds_Linear[v3 + 3];
}

void SpikeShootingPlant_Main(void) {  // 0xA29FB3
  Enemy_SpikeShootingPlant *E = Get_SpikeShootingPlant(cur_enemy_index);
  CallEnemyPreInstr(E->sspt_var_F | 0xA20000);
}

void SpikeShootingPlant_2(uint16 k) {  // 0xA29FBA
  Enemy_SpikeShootingPlant *E = Get_SpikeShootingPlant(cur_enemy_index);
  AddToHiLo(&E->base.x_pos, &E->base.x_subpos, __PAIR32__(E->sspt_var_D, E->sspt_var_C));
  if ((int16)(E->base.x_pos - E->sspt_var_00) < 0) {
    E->sspt_var_F = FUNC16(SpikeShootingPlant_3);
    E->sspt_var_E = 1;
  }
  SpikeShootingPlant_4();
}

void SpikeShootingPlant_3(uint16 k) {  // 0xA29FEC
  Enemy_SpikeShootingPlant *E = Get_SpikeShootingPlant(cur_enemy_index);
  AddToHiLo(&E->base.x_pos, &E->base.x_subpos, __PAIR32__(E->sspt_var_B, E->sspt_var_A));
  if ((int16)(E->base.x_pos - E->sspt_var_01) >= 0) {
    E->sspt_var_F = FUNC16(SpikeShootingPlant_2);
    E->sspt_var_E = 0;
  }
  SpikeShootingPlant_4();
}

void SpikeShootingPlant_4(void) {  // 0xA2A01C

  NextRandom();
  Enemy_SpikeShootingPlant *E = Get_SpikeShootingPlant(cur_enemy_index);
  if (sign16((uint8)(LOBYTE(E->base.frame_counter) + random_number) - 3)) {
    E->sspt_var_F = FUNC16(nullsub_182);
    if (HIBYTE(E->sspt_parameter_1))
      SpikeShootingPlant_6();
    else
      SpikeShootingPlant_8();
  }
}

void SpikeShootingPlant_5(void) {  // 0xA2A049
  Enemy_SpikeShootingPlant *E = Get_SpikeShootingPlant(cur_enemy_index);
  E->base.current_instruction = addr_kSpikeShootingPlant_Ilist_9E8A;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
}

void SpikeShootingPlant_6(void) {  // 0xA2A05C
  Enemy_SpikeShootingPlant *E = Get_SpikeShootingPlant(cur_enemy_index);
  E->base.current_instruction = addr_kSpikeShootingPlant_Ilist_9EB0;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
}

void SpikeShootingPlant_7(void) {  // 0xA2A06F
  Enemy_SpikeShootingPlant *E = Get_SpikeShootingPlant(cur_enemy_index);
  E->base.current_instruction = addr_kSpikeShootingPlant_Ilist_9EDA;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
}

void SpikeShootingPlant_8(void) {  // 0xA2A082
  Enemy_SpikeShootingPlant *E = Get_SpikeShootingPlant(cur_enemy_index);
  E->base.current_instruction = addr_kSpikeShootingPlant_Ilist_9F00;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
}

const uint16 *SpikeShootingPlant_Instr_A095(uint16 k, const uint16 *jp) {  // 0xA2A095
  Enemy_SpikeShootingPlant *E = Get_SpikeShootingPlant(k);
  E->sspt_var_F = FUNC16(SpikeShootingPlant_2);
  if (E->sspt_var_E)
    E->sspt_var_F = FUNC16(SpikeShootingPlant_3);
  return jp;
}

const uint16 *SpikeShootingPlant_Instr_A0A7(uint16 k, const uint16 *jp) {  // 0xA2A0A7
  SpawnEprojWithGfx(*jp, cur_enemy_index, addr_kEproj_SpikeShootingPlantSpikes);
  return jp + 1;
}

static Func_V *const off_A2A3D3[7] = {  // 0xA2A3F9
  MaridiaSpikeyShell_1, MaridiaSpikeyShell_2,

  MaridiaSpikeyShell_3, MaridiaSpikeyShell_4,
  MaridiaSpikeyShell_5, MaridiaSpikeyShell_6,
  MaridiaSpikeyShell_7,
};

void MaridiaSpikeyShell_Init(void) {

  Enemy_MaridiaSpikeyShell *E = Get_MaridiaSpikeyShell(cur_enemy_index);
  E->base.spritemap_pointer = addr_kSpritemap_Nothing_A2;
  uint16 mssl_parameter_1_low = LOBYTE(E->mssl_parameter_1);
  E->mssl_var_E = mssl_parameter_1_low;
  off_A2A3D3[mssl_parameter_1_low & 1]();
  int v3 = 4 * LOBYTE(E->mssl_parameter_2);
  E->mssl_var_B = kCommonEnemySpeeds_Linear[v3];
  E->mssl_var_A = kCommonEnemySpeeds_Linear[v3 + 1];
  E->mssl_var_D = kCommonEnemySpeeds_Linear[v3 + 2];
  E->mssl_var_C = kCommonEnemySpeeds_Linear[v3 + 3];
  int v4 = HIBYTE(E->mssl_parameter_2);
  E->mssl_var_02 = g_word_A2A3DD[v4] + E->base.x_pos;
  E->mssl_var_01 = E->base.x_pos - g_word_A2A3DD[v4];
  E->mssl_var_00 = g_word_A2A3ED[HIBYTE(E->mssl_parameter_1)];
  E->mssl_var_F = 0;
  if (E->mssl_var_E == 2) {
    E->mssl_var_F = 16;
    E->base.y_pos += 16;
  }
}

void MaridiaSpikeyShell_Main(void) {  // 0xA2A47E
  Enemy_MaridiaSpikeyShell *e = Get_MaridiaSpikeyShell(cur_enemy_index);
  off_A2A3D3[(int16)e->mssl_var_E + 2]();
}

void MaridiaSpikeyShell_1(void) {  // 0xA2A48A
  Enemy_MaridiaSpikeyShell *E = Get_MaridiaSpikeyShell(cur_enemy_index);
  E->base.current_instruction = addr_kMaridiaSpikeyShell_Ilist_A3AB;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
}

void MaridiaSpikeyShell_2(void) {  // 0xA2A49D
  Enemy_MaridiaSpikeyShell *E = Get_MaridiaSpikeyShell(cur_enemy_index);
  E->base.current_instruction = addr_kMaridiaSpikeyShell_Ilist_A3BD;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
}

void MaridiaSpikeyShell_3(void) {  // 0xA2A4B0
  Enemy_MaridiaSpikeyShell *E = Get_MaridiaSpikeyShell(cur_enemy_index);
  AddToHiLo(&E->base.x_pos, &E->base.x_subpos, __PAIR32__(E->mssl_var_D, E->mssl_var_C));
  if ((int16)(E->base.x_pos - E->mssl_var_01) < 0)
    --E->mssl_var_E;
  MaridiaSpikeyShell_8(cur_enemy_index);
}

void MaridiaSpikeyShell_4(void) {  // 0xA2A4D9
  Enemy_MaridiaSpikeyShell *E = Get_MaridiaSpikeyShell(cur_enemy_index);
  AddToHiLo(&E->base.x_pos, &E->base.x_subpos, __PAIR32__(E->mssl_var_B, E->mssl_var_A));
  if ((int16)(E->base.x_pos - E->mssl_var_02) >= 0)
    E->mssl_var_E = 0;
  MaridiaSpikeyShell_8(cur_enemy_index);
}

void MaridiaSpikeyShell_5(void) {  // 0xA2A502
  Enemy_MaridiaSpikeyShell *E = Get_MaridiaSpikeyShell(cur_enemy_index);
  uint16 v1 = E->mssl_var_00 - 1;
  E->mssl_var_00 = v1;
  if (!v1)
    E->mssl_var_E = 4;
}

void MaridiaSpikeyShell_6(void) {  // 0xA2A517
  Enemy_MaridiaSpikeyShell *E = Get_MaridiaSpikeyShell(cur_enemy_index);
  ++E->base.y_pos;
  if (!sign16(++E->mssl_var_F - 16)) {
    E->mssl_var_E = 2;
    E->mssl_var_00 = g_word_A2A3ED[HIBYTE(E->mssl_parameter_1)];
  }
}

void MaridiaSpikeyShell_7(void) {  // 0xA2A53E
  Enemy_MaridiaSpikeyShell *E = Get_MaridiaSpikeyShell(cur_enemy_index);
  --E->base.y_pos;
  if (E->mssl_var_F-- == 1)
    E->mssl_var_E = HIBYTE(random_number) & 1;
}

void MaridiaSpikeyShell_8(uint16 k) {  // 0xA2A553
  NextRandom();
  Enemy_MaridiaSpikeyShell *E = Get_MaridiaSpikeyShell(k);
  if (sign16((uint8)(LOBYTE(E->base.frame_counter) + random_number) - 6))
    E->mssl_var_E = 3;
}

const uint16 *MaridiaSpikeyShell_Instr_A56D(uint16 k, const uint16 *jp) {  // 0xA2A56D
  Get_MaridiaSpikeyShell(k)->mssl_var_E = 0;
  return jp;
}

const uint16 *MaridiaSpikeyShell_Instr_A571(uint16 k, const uint16 *jp) {  // 0xA2A571
  Get_MaridiaSpikeyShell(k)->mssl_var_E = 1;
  return jp;
}

void MaridiaSpikeyShell_Shot(void) {  // 0xA2A579
  Enemy_MaridiaSpikeyShell *E = Get_MaridiaSpikeyShell(cur_enemy_index);
  if (sign16(E->mssl_var_E - 1))
    Enemy_NormalShotAI_A2();
}

void GunshipTop_Init(void) {  // 0xA2A644
  Enemy_GunshipTop *E = Get_GunshipTop(cur_enemy_index);
  E->base.properties |= kEnemyProps_DisableSamusColl | kEnemyProps_Tangible;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->base.current_instruction = addr_kGunshipTop_Ilist_A616;
  E->base.palette_index = 3584;
  if (game_state != 40 || demo_set) {
    if (loading_game_state == kLoadingGameState_22_EscapingCeres) {
      E->base.y_pos = samus_y_pos - 17;
      E->gtp_var_F = FUNC16(GunshipTop_3);
    } else {
      uint16 v3 = E->base.y_pos - 25;
      E->base.y_pos = v3;
      E->gtp_var_E = v3;
      E->gtp_var_F = FUNC16(GunshipTop_8);
    }
  } else {
    samus_y_pos = 1138;
    uint16 v5 = E->base.y_pos - 25;
    E->base.y_pos = v5;
    E->gtp_var_E = v5;
    E->gtp_var_F = FUNC16(GunshipTop_14);
    Get_GunshipTop(0)->gtp_var_A = 144;
  }
  SpawnPalfxObject(addr_stru_8DE1C0);
  Get_GunshipTop(cur_enemy_index)->gtp_var_D = 1;
  Get_GunshipTop(0)->gtp_var_C = 0;
}

void GunshipBottom_Init(void) {  // 0xA2A6D2
  Enemy_GunshipBottom *E = Get_GunshipBottom(cur_enemy_index);
  E->base.properties |= kEnemyProps_DisableSamusColl | kEnemyProps_Tangible;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  if (E->gbm_parameter_2)
    E->base.current_instruction = addr_kGunshipTop_Ilist_A60E;
  else
    E->base.current_instruction = addr_kGunshipTop_Ilist_A61C;
  int v2 = cur_enemy_index >> 1;
  E->base.vram_tiles_index = enemy_drawing_queue[v2 + 106];
  E->base.palette_index = 3584;
  if (E->gbm_parameter_2) {
    E->base.y_pos = enemy_drawing_queue[v2 + 61] - 1;
    if (game_state == kPose_28_FaceL_Crouch && !demo_set) {
      E->base.instruction_timer = 1;
      E->base.current_instruction = addr_kGunshipTop_Ilist_A5BE;
    }
  } else if (loading_game_state == kGameState_34_CeresGoesBoom) {
    E->base.y_pos = samus_y_pos + 23;
  } else {
    E->base.y_pos += 15;
    E->gbm_var_D = 71;
  }
  E->gbm_var_F = FUNC16(nullsub_187);
}

void GunshipTop_Main(void) {  // 0xA2A759
  Enemy_GunshipTop *E1 = Get_GunshipTop(cur_enemy_index + 64);
  bool v2 = E1->gtp_var_D == 1;
  bool v3 = (--E1->gtp_var_D & 0x8000) != 0;
  if (v2 || v3) {
    QueueSfx2_Max6(0x4D);
    E1->gtp_var_D = 70;
  }
  Enemy_GunshipTop *E0 = Get_GunshipTop(cur_enemy_index);
  if (!sign16(E0->gtp_var_F + 0x56BE) && sign16(E0->gtp_var_F + 0x53E5))
    GunshipTop_1(cur_enemy_index);
  CallEnemyPreInstr(E0->gtp_var_F | 0xA20000);
}

void GunshipTop_1(uint16 k) {  // 0xA2A784
  Enemy_GunshipTop *E0 = Get_GunshipTop(k);
  bool v3 = E0->gtp_var_D == 1;
  bool v4 = (--E0->gtp_var_D & 0x8000) != 0;
  if (v3 || v4) {
    uint16 v5 = 2 * E0->gtp_var_C;
    E0->gtp_var_D = g_byte_A2A7CF[v5];
    uint16 v6 = (int8)g_byte_A2A7CF[v5 + 1];
    E0->base.y_pos += v6;
    Enemy_GunshipTop *E1 = Get_GunshipTop(k + 64);
    E1->base.y_pos += v6;
    Enemy_GunshipTop *E2 = Get_GunshipTop(k + 128);
    E2->base.y_pos += v6;
    E0->gtp_var_C = (E0->gtp_var_C + 1) & 3;
  }
}

void GunshipTop_2(uint16 k) {  // 0xA2A7D8
  samus_y_pos -= 8;
  Enemy_GunshipTop *E2 = Get_GunshipTop(k + 128);
  E2->base.y_pos -= 8;
  Enemy_GunshipTop *E1 = Get_GunshipTop(k + 64);
  E1->base.y_pos -= 8;
  Enemy_GunshipTop *E0 = Get_GunshipTop(k);
  uint16 v4 = E0->base.y_pos - 8;
  E0->base.y_pos = v4;
  if (sign16(v4 - 128))
    E0->gtp_var_F = FUNC16(GunshipTop_3);
}

void GunshipTop_3(uint16 k) {  // 0xA2A80C
  Enemy_GunshipTop *E = Get_GunshipTop(k);
  if (sign16(E->base.y_pos - 768)) {
    AddToHiLo(&samus_y_pos, &samus_y_subpos, 0x48000);
    Enemy_GunshipTop *v3 = Get_GunshipTop(k + 128);
    AddToHiLo(&v3->base.y_pos, &v3->base.y_subpos, 0x48000);
    Enemy_GunshipTop *v5 = Get_GunshipTop(k + 64);
    AddToHiLo(&v5->base.y_pos, &v5->base.y_subpos, 0x48000);
    AddToHiLo(&E->base.y_pos, &E->base.y_subpos, 0x48000);
  } else {
    AddToHiLo(&samus_y_pos, &samus_y_subpos, 0x28000);
    Enemy_GunshipTop *v8 = Get_GunshipTop(k + 128);
    AddToHiLo(&v8->base.y_pos, &v8->base.y_subpos, 0x28000);
    Enemy_GunshipTop *v10 = Get_GunshipTop(k + 64);
    AddToHiLo(&v10->base.y_pos, &v10->base.y_subpos, 0x28000);
    AddToHiLo(&E->base.y_pos, &E->base.y_subpos, 0x28000);
    if (!sign16(E->base.y_pos - 1119)) {
      E->base.y_pos = 1119;
      v10->base.y_pos = 1159;
      v8->base.y_pos = E->base.y_pos - 1;
      E->gtp_var_F = FUNC16(GunshipTop_4);
      E->gtp_var_E = 0;
    }
  }
}

void GunshipTop_4(uint16 k) {  // 0xA2A8D0
  int v2;
  uint16 v5;

  Enemy_GunshipTop *GunshipTop = Get_GunshipTop(k);
  v2 = GunshipTop->gtp_var_E;
  samus_y_pos += g_word_A2A622[v2];
  Enemy_GunshipTop *v3 = Get_GunshipTop(k + 128);
  v3->base.y_pos += g_word_A2A622[v2];
  Enemy_GunshipTop *v4 = Get_GunshipTop(k + 64);
  v4->base.y_pos += g_word_A2A622[v2];
  GunshipTop->base.y_pos += g_word_A2A622[v2];
  v5 = GunshipTop->gtp_var_E + 1;
  GunshipTop->gtp_var_E = v5;
  if (!sign16(v5 - 17)) {
    GunshipTop->gtp_var_F = FUNC16(GunshipTop_5);
    GunshipTop->gtp_var_E = GunshipTop->base.y_pos;
    GunshipTop->gtp_var_D = 1;
    Enemy_GunshipTop *v6 = Get_GunshipTop(0);
    v6->gtp_var_C = 0;
    samus_x_pos = GunshipTop->base.x_pos + 1;
    samus_prev_x_pos = samus_x_pos;
    Enemy_GunshipTop *v7 = Get_GunshipTop(k + 128);
    v7->base.instruction_timer = 1;
    v7->base.current_instruction = addr_kGunshipTop_Ilist_A5BE;
    v6->gtp_var_A = 144;
    QueueSfx3_Max6(0x14);
  }
}

void GunshipTop_5(uint16 k) {  // 0xA2A942
  Enemy_GunshipTop *E = Get_GunshipTop(0);
  bool v2 = E->gtp_var_A == 1;
  bool v3 = (--E->gtp_var_A & 0x8000) != 0;
  if (v2 || v3)
    Get_GunshipTop(k)->gtp_var_F = FUNC16(GunshipTop_6);
}

void GunshipTop_6(uint16 k) {  // 0xA2A950
  Enemy_GunshipTop *E = Get_GunshipTop(k);
  uint16 r18 = E->gtp_var_E - 30;
  if (sign16(--samus_y_pos - r18)) {
    E->gtp_var_F = FUNC16(GunshipTop_7);
    Enemy_GunshipTop *E2 = Get_GunshipTop(k + 128);
    E2->base.instruction_timer = 1;
    E2->base.current_instruction = addr_kGunshipTop_Ilist_A5EE;
    Get_GunshipTop(0)->gtp_var_A = 144;
    QueueSfx3_Max6(0x15);
  }
}

void GunshipTop_7(uint16 k) {  // 0xA2A987
  Enemy_GunshipTop *E = Get_GunshipTop(0);
  bool v2 = E->gtp_var_A == 1;
  bool v3 = (--E->gtp_var_A & 0x8000) != 0;
  if (v2 || v3) {
    Get_GunshipTop(k)->gtp_var_F = FUNC16(GunshipTop_8);
    frame_handler_alfa = FUNC16(Samus_FrameHandlerAlfa_Func11);
    frame_handler_beta = FUNC16(Samus_FrameHandlerBeta_Func17);
    loading_game_state = kLoadingGameState_5_Main;
    *(uint16 *)used_save_stations_and_elevators |= 1;
    load_station_index = 0;
    SaveToSram(selected_save_slot);
  }
}

void GunshipTop_8(uint16 k) {  // 0xA2A9BD
  if (game_state == 8 && frame_handler_alfa == FUNC16(Samus_FrameHandlerAlfa_Func11)) {
    Enemy_GunshipTop *E = Get_GunshipTop(k);
    if ((int16)(E->base.x_pos - 8 - samus_x_pos) < 0
        && (int16)(E->base.x_pos + 8 - samus_x_pos) >= 0
        && (int16)(E->base.y_pos - 64 - samus_y_pos) < 0
        && (int16)(E->base.y_pos - samus_y_pos) >= 0
        && !samus_movement_type
        && (joypad1_newkeys & kButton_Down) != 0) {
      E->gtp_var_F = FUNC16(GunshipTop_9);
      if (samus_x_pos != 1152) {
        samus_x_pos = E->base.x_pos;
        samus_prev_x_pos = samus_x_pos;
      }
      MakeSamusFaceForward();
      CallSomeSamusCode(0x1A);
      elevator_status = 0;
      uint16 v3 = E->base.y_pos - 1;
      Enemy_GunshipTop *E2 = Get_GunshipTop(k + 128);
      E2->base.y_pos = v3;
      E2->base.instruction_timer = 1;
      E2->base.current_instruction = addr_kGunshipTop_Ilist_A5BE;
      Get_GunshipTop(0)->gtp_var_A = 144;
      QueueSfx3_Max6(0x14);
    }
  }
}

void GunshipTop_9(uint16 k) {  // 0xA2AA4F
  Enemy_GunshipTop *E = Get_GunshipTop(0);
  bool v2 = E->gtp_var_A == 1;
  bool v3 = (--E->gtp_var_A & 0x8000) != 0;
  if (v2 || v3)
    Get_GunshipTop(k)->gtp_var_F = FUNC16(GunshipTop_10);
}

void GunshipTop_10(uint16 k) {  // 0xA2AA5D
  Enemy_GunshipTop *E = Get_GunshipTop(k);
  uint16 r18 = E->gtp_var_E + 18;
  samus_y_pos += 2;
  if (!sign16(samus_y_pos - r18)) {
    E->gtp_var_F = FUNC16(GunshipTop_11);
    Enemy_GunshipTop *E2 = Get_GunshipTop(k + 128);
    E2->base.instruction_timer = 1;
    E2->base.current_instruction = addr_kGunshipTop_Ilist_A5EE;
    Get_GunshipTop(0)->gtp_var_A = 144;
    QueueSfx3_Max6(0x15);
  }
}

void GunshipTop_11(uint16 k) {  // 0xA2AA94
  Enemy_GunshipTop *E = Get_GunshipTop(0);
  bool v2 = E->gtp_var_A == 1;
  bool v3 = (--E->gtp_var_A & 0x8000) != 0;
  if (v2 || v3)
    Get_GunshipTop(k)->gtp_var_F = FUNC16(GunshipTop_12);
}

void GunshipTop_12(uint16 k) {  // 0xA2AAA2
  if (CheckEventHappened(0xE)) {
    Get_GunshipTop(k)->gtp_var_F = FUNC16(GunshipTop_17);
    Enemy_GunshipTop *E = Get_GunshipTop(k + 64);
    E->gtp_var_F = 0;
    E->gtp_var_E = 0;
    substate = 0;
    suit_pickup_light_beam_pos = 0;
    *(uint16 *)&suit_pickup_color_math_R = 0;
    *(uint16 *)&suit_pickup_color_math_B = 0;
    CallSomeSamusCode(0xA);
  } else {
    Samus_RestoreHealth(2);
    Samus_RestoreMissiles(2);
    Samus_RestoreSuperMissiles(2);
    Samus_RestorePowerBombs(2);
    if ((int16)(samus_reserve_health - samus_max_reserve_health) >= 0
        && (int16)(samus_health - samus_max_health) >= 0
        && (int16)(samus_missiles - samus_max_missiles) >= 0
        && (int16)(samus_super_missiles - samus_max_super_missiles) >= 0
        && (int16)(samus_power_bombs - samus_max_power_bombs) >= 0) {
      Get_GunshipTop(k)->gtp_var_F = FUNC16(GunshipTop_13);
    }
  }
}

void GunshipTop_13(uint16 k) {  // 0xA2AB1F
  // Returns rv >= 0 once it's done
  int rv = DisplayMessageBox_Poll(0x1c);
  if (rv < 0)
    return;

  if (rv != 2) {
    *(uint16 *)used_save_stations_and_elevators |= 1;
    load_station_index = 0;
    SaveToSram(selected_save_slot);
  }
  Get_GunshipTop(k)->gtp_var_F = FUNC16(GunshipTop_14);
  Enemy_GunshipTop *E = Get_GunshipTop(k + 128);
  E->base.instruction_timer = 1;
  E->base.current_instruction = addr_kGunshipTop_Ilist_A5BE;
  Get_GunshipTop(0)->gtp_var_A = 144;
  QueueSfx3_Max6(0x14);
}

void GunshipTop_14(uint16 k) {  // 0xA2AB60
  Enemy_GunshipTop *E = Get_GunshipTop(0);
  bool v2 = E->gtp_var_A == 1;
  bool v3 = (--E->gtp_var_A & 0x8000) != 0;
  if (v2 || v3)
    Get_GunshipTop(k)->gtp_var_F = FUNC16(GunshipTop_15);
}

void GunshipTop_15(uint16 k) {  // 0xA2AB6E
  Enemy_GunshipTop *E = Get_GunshipTop(k);
  uint16 r18 = E->gtp_var_E - 30;
  samus_y_pos -= 2;
  if (sign16(samus_y_pos - r18)) {
    E->gtp_var_F = FUNC16(GunshipTop_16);
    Enemy_GunshipTop *E2 = Get_GunshipTop(k + 128);
    E2->base.instruction_timer = 1;
    E2->base.current_instruction = addr_kGunshipTop_Ilist_A5EE;
    Get_GunshipTop(0)->gtp_var_A = 144;
    QueueSfx3_Max6(0x15);
  }
}

void GunshipTop_16(uint16 k) {  // 0xA2ABA5
  Enemy_GunshipTop *E = Get_GunshipTop(0);
  bool v2 = E->gtp_var_A == 1;
  bool v3 = (--E->gtp_var_A & 0x8000) != 0;
  if (v2 || v3) {
    Get_GunshipTop(k)->gtp_var_F = FUNC16(GunshipTop_8);
    if (sign16(game_state - kGameState_40_TransitionToDemo)) {
      frame_handler_alfa = FUNC16(Samus_FrameHandlerAlfa_Func11);
      frame_handler_beta = FUNC16(Samus_FrameHandlerBeta_Func17);
    }
  }
}

void GunshipTop_17(uint16 k) {  // 0xA2ABC7
  uint16 v1 = substate;
  uint16 v2 = vram_write_queue_tail;
  gVramWriteEntry(vram_write_queue_tail)->size = 1024;
  v2 += 2;
  int v3 = v1 >> 1;
  gVramWriteEntry(v2)->size = g_word_A2AC07[v3];
  v2 += 2;
  LOBYTE(gVramWriteEntry(v2++)->size) = -108;
  gVramWriteEntry(v2)->size = g_word_A2AC11[v3];
  vram_write_queue_tail = v2 + 2;
  substate += 2;
  if (!sign16(substate - 10)) {
    Get_GunshipTop(k)->gtp_var_F = FUNC16(GunshipTop_18);
    substate = 0;
  }
}

void GunshipTop_18(uint16 k) {  // 0xA2AC1B
  int16 v5;

  Enemy_GunshipTop *E1 = Get_GunshipTop(k + 64);
  uint16 gtp_var_E = E1->gtp_var_E;
  if (sign16(gtp_var_E - 64)) {
    uint16 v3;
    if ((gtp_var_E & 1) != 0)
      v3 = samus_y_pos + 1;
    else
      v3 = samus_y_pos - 1;
    samus_y_pos = v3;
    samus_prev_y_pos = v3;
  } else {
    uint16 v4;
    if ((gtp_var_E & 1) != 0)
      v4 = samus_y_pos + 2;
    else
      v4 = samus_y_pos - 2;
    samus_y_pos = v4;
    samus_prev_y_pos = v4;
  }
  v5 = samus_y_pos - 17;
  Enemy_GunshipTop *E0 = Get_GunshipTop(k);
  E0->base.y_pos = samus_y_pos - 17;
  Get_GunshipTop(k + 128)->base.y_pos = v5 - 1;
  E1->base.y_pos = samus_y_pos + 23;
  uint16 v7 = E1->gtp_var_E + 1;
  E1->gtp_var_E = v7;
  if (sign16(v7 - 128)) {
    if (v7 == 64) {
      SpawnEprojWithRoomGfx(addr_stru_86A379, 0);
      SpawnEprojWithRoomGfx(addr_stru_86A379, 2);
      SpawnEprojWithRoomGfx(addr_stru_86A379, 4);
      SpawnEprojWithRoomGfx(addr_stru_86A379, 6);
      SpawnEprojWithRoomGfx(addr_stru_86A379, 8);
      SpawnEprojWithRoomGfx(addr_stru_86A379, 0xA);
    }
  } else {
    E0->gtp_var_F = FUNC16(GunshipTop_19);
    Get_GunshipTop(0)->gtp_var_A = 0;
  }
}

void GunshipTop_19(uint16 k) {  // 0xA2ACD7
  int16 v1;

  samus_y_pos -= 2;
  v1 = samus_y_pos - 17;
  Enemy_GunshipTop *E0 = Get_GunshipTop(k);
  E0->base.y_pos = samus_y_pos - 17;
  Get_GunshipTop(k + 128)->base.y_pos = v1 - 1;
  Enemy_GunshipTop *E1 = Get_GunshipTop(k + 64);
  E1->base.y_pos = samus_y_pos + 23;
  if (sign16(E0->base.y_pos - 896)) {
    E0->gtp_var_F = FUNC16(GunshipTop_20);
    E1->gtp_var_F = 512;
  }
}

void GunshipTop_20(uint16 k) {  // 0xA2AD0E
  GunshipTop_21(k);
  Enemy_GunshipTop *E = Get_GunshipTop(k);
  if (sign16(E->base.y_pos - 256)) {
    E->gtp_var_F = FUNC16(GunshipTop_21);
    game_state = 38;
    screen_fade_delay = 0;
    screen_fade_counter = 0;
  }
}

void GunshipTop_21(uint16 k) {  // 0xA2AD2D
  Enemy_GunshipTop *E = Get_GunshipTop(k + 64);
  uint16 v3 = E->gtp_var_F + 64;
  E->gtp_var_F = v3;
  if (!sign16((v3 & 0xFF00) - 2560))
    E->gtp_var_F = 2304;
  uint32 v6 = __PAIR32__(samus_y_pos, samus_y_subpos) - __PAIR32__(E->gtp_var_F >> 8, E->gtp_var_F << 8);
  samus_y_subpos = v6;
  samus_y_pos = v6 >> 16;
  uint16 v5 = samus_y_pos - 17;
  Get_GunshipTop(k)->base.y_pos = v5;
  Get_GunshipTop(k + 128)->base.y_pos = v5 - 1;
  E->base.y_pos = samus_y_pos + 23;
}

void Flies_Init(void) {  // 0xA2B06B
  Enemy_Flies *E = Get_Flies(cur_enemy_index);
  E->flies_var_E = 0;
  E->flies_var_F = FUNC16(Flies_4);
  E->base.current_instruction = addr_kFlies_Ilist_B013;
  E->base.spritemap_pointer = addr_kSpritemap_Nothing_A2;
  E->base.instruction_timer = 1;
  E->base.spritemap_pointer = addr_kSpritemap_Nothing_A2;
}

bool CarryAdd16(uint16 *rv, uint16 v) {
  int i = *rv + v;
  *rv = i;
  return (i >> 16);
}

void Flies_1(uint16 k) {  // 0xA2B090
  Enemy_Flies *E = Get_Flies(k);
  int v2 = E->flies_var_E >> 1;
  bool carry = CarryAdd16(&E->base.x_subpos, LOBYTE(kSinCosTable8bit_Sext[v2 + 64]) << 8);
  E->base.x_pos += (int8)HIBYTE(kSinCosTable8bit_Sext[v2 + 64]) + carry;
  carry = CarryAdd16(&E->base.y_subpos, LOBYTE(kSinCosTable8bit_Sext[v2]) << 8);
  E->base.y_pos += (int8)HIBYTE(kSinCosTable8bit_Sext[v2]) + carry;
}

uint16 Flies_2(uint16 k) {  // 0xA2B0DC
  Enemy_Flies *E = Get_Flies(k);
  bool carry = CarryAdd16(&E->base.x_subpos, LOBYTE(E->flies_var_B) << 8);
  E->base.x_pos += (int8)HIBYTE(E->flies_var_B) + carry;
  carry = CarryAdd16(&E->base.y_subpos, LOBYTE(E->flies_var_C) << 8);
  uint16 result = E->base.y_pos + (int8)HIBYTE(E->flies_var_C) + carry;
  E->base.y_pos = result;
  return result;
}

void Flies_Main(void) {  // 0xA2B11F
  NextRandom();
  Enemy_Flies *E = Get_Flies(cur_enemy_index);
  CallEnemyPreInstr(E->flies_var_F | 0xA20000);
}

void Flies_3(uint16 k) {  // 0xA2B129
  int v1 = CalculateAngleOfSamusFromEnemy(k);
  Enemy_Flies *E = Get_Flies(k);
  E->flies_var_B = 2 * kSinCosTable8bit_Sext[v1 + 64];
  E->flies_var_C = 4 * kSinCosTable8bit_Sext[v1];
  E->flies_var_D = samus_y_pos;
  E->flies_var_F = FUNC16(Flies_6);
}

void Flies_4(uint16 k) {  // 0xA2B14E
  Enemy_Flies *E = Get_Flies(k);
  uint16 flies_var_A = E->flies_var_A;
  if (flies_var_A) {
    E->flies_var_A = flies_var_A - 1;
  } else if (!(CompareDistToSamus_X(k, 0x70) & 1)) {
    Flies_3(k);
    return;
  }
  Flies_1(k);
  bool v3 = ((E->flies_var_E + 32) & 0x1FF) == 0;
  E->flies_var_E = (E->flies_var_E + 32) & 0x1FF;
  if (v3)
    E->flies_var_F = FUNC16(Flies_5);
}

void Flies_5(uint16 k) {  // 0xA2B17C
  Enemy_Flies *E = Get_Flies(k);
  uint16 flies_var_A = E->flies_var_A;
  if (flies_var_A) {
    E->flies_var_A = flies_var_A - 1;
  } else if (!(CompareDistToSamus_X(k, 0x70) & 1)) {
    Flies_3(k);
    return;
  }
  Flies_1(k);
  bool v3 = ((E->flies_var_E - 32) & 0x1FF) == 0;
  E->flies_var_E = (E->flies_var_E - 32) & 0x1FF;
  if (v3)
    E->flies_var_F = FUNC16(Flies_4);
}

void Flies_6(uint16 k) {  // 0xA2B1AA
  uint16 v1 = Flies_2(k);
  Enemy_Flies *E = Get_Flies(k);
  ++E->flies_var_A;
  if ((E->flies_var_C & 0x8000) == 0) {
    if (v1 < E->flies_var_D)
      return;
    goto LABEL_5;
  }
  if (v1 < E->flies_var_D) {
LABEL_5:
    E->flies_var_C = -E->flies_var_C;
    E->flies_var_F = FUNC16(Flies_7);
  }
}

void Flies_7(uint16 k) {  // 0xA2B1D2
  Flies_2(k);
  Enemy_Flies *E = Get_Flies(k);
  if ((--E->flies_var_A & 0x8000) != 0) {
    E->flies_var_A = 24;
    E->flies_var_F = FUNC16(Flies_4);
  }
}

void NorfairErraticFireball_Init(void) {  // 0xA2B3E0
  Enemy_NorfairErraticFireball *E = Get_NorfairErraticFireball(cur_enemy_index);
  Point32 pt = ConvertAngleToXy(E->nefl_parameter_1, LOBYTE(E->nefl_parameter_2));
  E->nefl_var_C = pt.x >> 16;
  E->nefl_var_D = pt.x;
  E->nefl_var_E = pt.y >> 16;
  E->nefl_var_F = pt.y;
  E->base.current_instruction = addr_kNorfairErraticFireball_Ilist_B2DC;
}

void NorfairErraticFireball_Main(void) {  // 0xA2B40F
  Enemy_NorfairErraticFireball *E = Get_NorfairErraticFireball(cur_enemy_index);
  Point32 pt = ConvertAngleToXy(E->nefl_parameter_1, LOBYTE(E->nefl_parameter_2));
  E->nefl_var_C = pt.x >> 16;
  E->nefl_var_D = pt.x;
  E->nefl_var_E = pt.y >> 16;
  E->nefl_var_F = pt.y;
  if (((E->nefl_parameter_1 + 64) & 0x80) == 0) {
    E->nefl_var_C = ~E->nefl_var_C;
    E->nefl_var_D = -E->nefl_var_D;
  }
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, __PAIR32__(E->nefl_var_C, E->nefl_var_D))) {
    E->nefl_parameter_1 ^= 0x40;
  } else {
    if (((E->nefl_parameter_1 + 128) & 0x80) == 0) {
      E->nefl_var_E = ~E->nefl_var_E;
      E->nefl_var_F = -E->nefl_var_F;
    }
    if (Enemy_MoveDown(cur_enemy_index, __PAIR32__(E->nefl_var_E, E->nefl_var_F)))
      E->nefl_parameter_1 ^= 0xC0;
  }
}

void LavaquakeRocks_Init(void) {  // 0xA2B570
  Enemy_LavaquakeRocks *E = Get_LavaquakeRocks(cur_enemy_index);
  E->base.current_instruction = addr_kNorfairErraticFireball_Ilist_B51A;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->lrs_var_A = FUNC16(LavaquakeRocks_1);
  random_number = 17;
}

void CallLavaQuakeRocksFunc(uint32 ea) {
  switch (ea) {
  case fnLavaquakeRocks_1: LavaquakeRocks_1(); return;
  case fnLavaquakeRocks_2: LavaquakeRocks_2(); return;
  case fnLavaquakeRocks_3: LavaquakeRocks_3(); return;
  default: Unreachable();
  }
}

void LavaquakeRocks_Main(void) {  // 0xA2B58F
  Enemy_LavaquakeRocks *E = Get_LavaquakeRocks(cur_enemy_index);
  CallLavaQuakeRocksFunc(E->lrs_var_A | 0xA20000);
}

void LavaquakeRocks_1(void) {  // 0xA2B596
  if (IsSamusWithinEnemy_X(cur_enemy_index, 0x40)) {
    if (IsSamusWithinEnemy_Y(cur_enemy_index, 0x40))
      Get_LavaquakeRocks(cur_enemy_index)->lrs_var_A = FUNC16(LavaquakeRocks_2);
  }
}

void LavaquakeRocks_2(void) {  // 0xA2B5B2
  eproj_unk1995 = g_word_A2B550[(NextRandom() & 0x1E) >> 1];
  int v1 = (NextRandom() & 0x1E) >> 1;
  SpawnEprojWithGfx(g_word_A2B530[v1], cur_enemy_index, addr_stru_86BD5A);
  Enemy_LavaquakeRocks *E = Get_LavaquakeRocks(cur_enemy_index);
  E->lrs_var_A = FUNC16(LavaquakeRocks_3);
  E->lrs_var_B = g_word_A2B520[(uint16)(NextRandom() & 0xE) >> 1];
}

void LavaquakeRocks_3(void) {  // 0xA2B5EA
  Enemy_LavaquakeRocks *E = Get_LavaquakeRocks(cur_enemy_index);
  if ((--E->lrs_var_B & 0x8000) != 0)
    E->lrs_var_A = FUNC16(LavaquakeRocks_1);
}

void Rinka_Init(void) {  // 0xA2B602
  Enemy_Rinka *E = Get_Rinka(cur_enemy_index);
  if (E->rinka_parameter_1) {
    Rinka_1(cur_enemy_index);
    E->base.properties = E->base.properties & ~(kEnemyProps_RespawnIfKilled | kEnemyProps_DisableSamusColl | kEnemyProps_ProcessedOffscreen | kEnemyProps_Tangible) | 0x2C00;
  } else {
    E->base.properties = E->base.properties & ~(kEnemyProps_RespawnIfKilled | kEnemyProps_DisableSamusColl | kEnemyProps_ProcessedOffscreen | kEnemyProps_Tangible) | 0x6400;
  }
  E->base.palette_index = 1024;
  Rinka_Init3(cur_enemy_index);
}

void Rinka_Init2(uint16 k) {  // 0xA2B63E
  EnemySpawnData *v2;

  Enemy_Rinka *E = Get_Rinka(k);
  if (E->rinka_parameter_1)
    Rinka_1(k);
  v2 = gEnemySpawnData(k);
  E->base.x_pos = v2->x_pos;
  E->base.y_pos = v2->y_pos;
  Rinka_Init3(k);
}

void Rinka_Init3(uint16 k) {  // 0xA2B654
  Enemy_Rinka *E = Get_Rinka(k);
  E->rinka_var_A = FUNC16(Rinka_5);
  E->rinka_var_F = 26;
  E->rinka_var_B = 0;
  E->rinka_var_C = 0;
  if (E->rinka_parameter_1) {
    if (Get_Rinka(0)->rinka_var_1D) {
      E->base.properties |= kEnemyProps_Deleted;
    } else {
      E->base.current_instruction = addr_kRinka_Ilist_BA0C;
      E->base.instruction_timer = 1;
      E->base.timer = 0;
    }
  } else {
    E->base.current_instruction = addr_kRinka_Ilist_B9E0;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

void Rinka_1(uint16 k) {  // 0xA2B69B
  uint16 v2;
  EnemySpawnData *ES = gEnemySpawnData(k);
  if (Rinka_10(ES->x_pos, ES->y_pos) || (v2 = Rinka_2(k), gRam8000_Default(v2)[31].var_3F & 1)) {
    uint16 v3 = 0;
    do {
      int v4 = v3 >> 1;
      uint16 x = g_word_A2B75B[v4];
      uint16 y = g_word_A2B75B[v4 + 1];
      if (!Rinka_10(x, y) && !(gRam8000_Default(g_word_A2B75B[v4 + 2])[31].var_3F & 1)) {
        Enemy_Rinka *E = Get_Rinka(k);
        ES->x_pos = x;
        E->base.x_pos = x;
        ES->y_pos = y;
        E->base.y_pos = y;
        uint16 v9 = g_word_A2B75B[(v3 >> 1) + 2];
        gRam8000_Default(v9)[31].var_3F = -1;
        E->rinka_var_D = v9;
        return;
      }
      v3 += 6;
    } while ((int16)(v3 - 66) < 0);
    uint16 v10 = 0;
    while (gRam8000_Default(g_word_A2B75B[(v10 >> 1) + 2])[31].var_3F & 1) {
      v10 += 6;
      if ((int16)(v10 - 66) >= 0)
        return;
    }
    int v11 = v10 >> 1;
    uint16 v12 = g_word_A2B75B[v11];
    ES->x_pos = v12;
    Enemy_Rinka *E = Get_Rinka(k);
    E->base.x_pos = v12;
    uint16 v15 = g_word_A2B75B[v11 + 1];
    ES->y_pos = v15;
    E->base.y_pos = v15;
    uint16 v16 = g_word_A2B75B[v11 + 2];
    E->rinka_var_D = v16;
    gRam8000_Default(v16)[31].var_3F = -1;
  } else {
    Get_Rinka(k)->rinka_var_D = v2;
    gRam8000_Default(v2)[31].var_3F = -1;
  }
}

uint16 Rinka_2(uint16 k) {  // 0xA2B79D
  EnemySpawnData *v3;

  uint16 v1 = 0;
  while (1) {
    int v2 = v1 >> 1;
    v3 = gEnemySpawnData(k);
    if (g_word_A2B75B[v2] == v3->x_pos && g_word_A2B75B[v2 + 1] == v3->y_pos)
      break;
    v1 += 6;
    if (!sign16(v1 - 66)) {
      v1 = 0;
      return g_word_A2B75B[(v1 >> 1) + 2];
    }
  }
  return g_word_A2B75B[(v1 >> 1) + 2];
}

void CallRinkaFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnRinka_3: Rinka_3(k); return;
  case fnRinka_4: Rinka_4(k); return;
  case fnRinka_5: Rinka_5(k); return;
  case fnRinka_B85B: Rinka_B85B(k); return;
  default: Unreachable();
  }
}

void Rinka_Main(void) {  // 0xA2B7C4
  uint16 v2;

  Enemy_Rinka *E = Get_Rinka(cur_enemy_index);
  if (E->rinka_parameter_1 && Get_Rinka(0)->rinka_var_1D) {
    Rinka_6(cur_enemy_index);
    Rinka_8(cur_enemy_index);
    //printf("A unknown\n");
    v2 = 0;
    RinkasDeathAnimation(v2);
  } else {
    CallRinkaFunc(E->rinka_var_A | 0xA20000, cur_enemy_index);
  }
}

void Rinka_3(uint16 k) {  // 0xA2B7DF
  Enemy_Rinka *E = Get_Rinka(k);
  if ((--E->rinka_var_F & 0x8000) != 0) {
    E->rinka_var_A = FUNC16(Rinka_B85B);
    uint16 v3;
    if (E->rinka_parameter_1)
      v3 = E->base.properties & ~kEnemyProps_Tangible;
    else
      v3 = E->base.properties & 0xF3FF | 0x800;
    E->base.properties = v3;
    uint16 r18 = (uint8)-(CalculateAngleFromXY(samus_x_pos - E->base.x_pos, samus_y_pos - E->base.y_pos) + 0x80);
    E->rinka_var_B = Math_MultBySin(0x120, r18);
    E->rinka_var_C = Math_MultByCos(0x120, r18);
  }
}

void Rinka_4(uint16 k) {  // 0xA2B844
  Enemy_Rinka *E = Get_Rinka(k);
  if ((--E->rinka_var_F & 0x8000) != 0) {
    E->base.health = 10;
    Rinka_Init2(k);
  }
}

void Rinka_5(uint16 k) {  // 0xA2B852
  if (Rinka_9(k) & 1)
    Rinka_B865(k);
}

void Rinka_B85B(uint16 k) {  // 0xA2B85B
  MoveEnemyWithVelocity();
  if (Rinka_9(k) & 1)
    Rinka_B865(k);
}

void Rinka_B865(uint16 k) {  // 0xA2B865
  if (Get_Rinka(k)->rinka_parameter_1 && (Rinka_8(k), Get_Rinka(0)->rinka_var_1D)) {
    Rinka_6(k);
    DeleteEnemyAndConnectedEnemies();
  } else {
    Rinka_6(k);
    Rinka_Init2(k);
  }
}

void Rinka_6(uint16 k) {  // 0xA2B880
  int16 v3;

  Enemy_Rinka *E = Get_Rinka(k);
  if (E->rinka_parameter_1 && (E->base.properties & kEnemyProps_Invisible) == 0) {
    Enemy_Rinka *E0 = Get_Rinka(0);
    v3 = E0->rinka_var_1E - 1;
    if (v3 < 0)
      v3 = 0;
    E0->rinka_var_1E = v3;
  }
}

void Rinka_7(uint16 k) {  // 0xA2B89C
  Enemy_Rinka *E = Get_Rinka(k);
  if ((random_enemy_counter & 3) == E->rinka_parameter_1)
    E->base.properties &= ~kEnemyProps_Tangible;
  else
    E->base.properties |= kEnemyProps_Tangible;
}

void Rinka_8(uint16 k) {  // 0xA2B8BB
  Enemy_Rinka *E = Get_Rinka(k);
  if (E->rinka_parameter_1) {
    uint16 rinka_var_D = E->rinka_var_D;
    if (rinka_var_D) {
      gRam8000_Default(rinka_var_D)[31].var_3F = 0;
      E->rinka_var_D = 0;
    }
  }
}

uint8 Rinka_9(uint16 k) {  // 0xA2B8D3
  int16 y_pos;
  int16 v3;
  int16 x_pos;
  int16 v5;

  Enemy_Rinka *E = Get_Rinka(k);
  y_pos = E->base.y_pos;
  uint8 result = 1;
  if (y_pos >= 0) {
    v3 = y_pos + 16 - layer1_y_pos;
    if (v3 >= 0) {
      if (sign16(v3 - 256)) {
        x_pos = E->base.x_pos;
        if (x_pos >= 0) {
          v5 = x_pos + 16 - layer1_x_pos;
          if (v5 >= 0) {
            if (sign16(v5 - 288))
              return 0;
          }
        }
      }
    }
  }
  return result;
}

bool Rinka_10(uint16 r18, uint16 r20) {  // 0xA2B8FF
  return (r20 & 0x8000) != 0 || (int16)(r20 - layer1_y_pos) < 0 || !sign16(r20 - layer1_y_pos - 224)
       || (r20 & 0x8000) != 0 || (int16)(r18 - layer1_x_pos) < 0 || !sign16(r18 - layer1_x_pos - 256);
}

void Rinka_Frozen(uint16 k) {  // 0xA2B929
  if (Rinka_9(k) & 1)
    Get_Rinka(k)->base.frozen_timer = 0;
  NormalEnemyFrozenAI();
  if (Get_Rinka(0)->rinka_var_1D) {
    Rinka_6(k);
    Rinka_8(k);
//    printf("A undefined!\n");
    uint16 v1 = 0;
    RinkasDeathAnimation(v1);
  }
}

void Rinka_Touch(void) {  // 0xA2B947
  NormalEnemyTouchAiSkipDeathAnim_CurEnemy();
  Rinka_B960(cur_enemy_index);
}

void Rinka_Shot(void) {  // 0xA2B94D
  NormalEnemyShotAiSkipDeathAnim_CurEnemy();
  Rinka_B960(cur_enemy_index);
}

void Rinka_Powerbomb(uint16 k) {  // 0xA2B953
  if ((Get_Rinka(k)->base.properties & kEnemyProps_Invisible) == 0) {
    NormalEnemyPowerBombAiSkipDeathAnim_CurEnemy();
    Rinka_B960(k);
  }
}

void Rinka_B960(uint16 k) {  // 0xA2B960
  Enemy_Rinka *E = Get_Rinka(k);
  if (!E->base.health) {
    Rinka_6(k);
    Rinka_8(k);
    if (E->rinka_parameter_1) {
      E->base.properties |= kEnemyProps_Tangible | kEnemyProps_Invisible;
      eproj_spawn_pt = (Point16U){ E->base.x_pos, E->base.y_pos };
      SpawnEprojWithRoomGfx(0xE509, 3);
      E->rinka_var_A = FUNC16(Rinka_4);
      E->rinka_var_F = 1;
    } else {
      RinkasDeathAnimation(0);
    }
  }
}

uint16 Rinka_Instr_B9A2(uint16 k, uint16 j) {  // 0xA2B9A2
  Enemy_Rinka *E = Get_Rinka(0);
  if (sign16(E->rinka_var_1E - 3))
    return j + 2;
  else
    return *(uint16 *)RomPtr_A2(j);
}

const uint16 *Rinka_Instr_B9B3(uint16 k, const uint16 *jp) {  // 0xA2B9B3
  Enemy_Rinka *E = Get_Rinka(k);
  E->base.properties |= kEnemyProps_Tangible | kEnemyProps_Invisible;
  return jp;
}

const uint16 *Rinka_Instr_B9BD(uint16 k, const uint16 *jp) {  // 0xA2B9BD
  Enemy_Rinka *E = Get_Rinka(k);
  E->base.properties |= kEnemyProps_ProcessedOffscreen | kEnemyProps_Tangible | kEnemyProps_Invisible;
  return jp;
}

const uint16 *Rinka_Instr_B9C7(uint16 k, const uint16 *jp) {  // 0xA2B9C7
  Enemy_Rinka *E = Get_Rinka(k);
  E->base.properties &= ~(kEnemyProps_Tangible | kEnemyProps_Invisible);
  E->rinka_var_A = FUNC16(Rinka_3);
  Enemy_Rinka *E0 = Get_Rinka(0);
  ++E0->rinka_var_1E;
  return jp;
}

const uint16 *EnemyInstr_Rio_Instr_1(uint16 k, const uint16 *jp) {  // 0xA2BBC3
  Get_Rio(cur_enemy_index)->rio_var_E = 1;
  return jp;
}

void Rio_Init(void) {  // 0xA2BBCD
  Enemy_Rio *E = Get_Rio(cur_enemy_index);
  E->rio_var_E = 0;
  E->rio_var_F = 0;
  E->base.current_instruction = addr_kRio_Ilist_BB4B;
  E->rio_var_B = FUNC16(Rio_1);
}

void CallRioFunc(uint32 ea) {
  uint16 k = cur_enemy_index;
  switch (ea) {
  case fnRio_1: Rio_1(k); return;  // 0x7c30a
  case fnRio_2: Rio_2(k); return;  // 0x7c396
  case fnRio_3: Rio_3(k); return;  // 0x7c3d4
  case fnRio_4: Rio_4(k); return;  // 0x7c4dc
  case fnRio_5: Rio_5(k); return;  // 0x7c599
  default: Unreachable();
  }
}

void Rio_Main(void) {  // 0xA2BBE3
  NextRandom();
  Enemy_Rio *E = Get_Rio(cur_enemy_index);
  CallRioFunc(E->rio_var_B | 0xA20000);
}

void Rio_1(uint16 k) {  // 0xA2BBED
  if (!(CompareDistToSamus_X(k, 0xA0) & 1)) {
    Enemy_Rio *E = Get_Rio(k);
    E->rio_var_C = g_word_A2BBBB;
    E->rio_var_D = g_word_A2BBBF;
    if ((int16)(samus_x_pos - E->base.x_pos) < 0)
      E->rio_var_D = -E->rio_var_D;
    Rio_6(addr_kRio_Ilist_BB7F);
    E->rio_var_B = FUNC16(Rio_3);
    if (!CheckIfEnemyIsOnScreen())
      QueueSfx2_Max6(0x65);
  }
}

void Rio_2(uint16 k) {  // 0xA2BC32
  Enemy_Rio *E = Get_Rio(k);
  if (E->rio_var_E) {
    E->rio_var_E = 0;
    Rio_6(addr_stru_A2BB53);
    E->rio_var_B = FUNC16(Rio_1);
  }
}

void Rio_3(uint16 k) {  // 0xA2BC48
  int16 v4;

  Enemy_Rio *E = Get_Rio(k);
  ;
  if (Enemy_MoveRight_IgnoreSlopes(k, INT16_SHL8(E->rio_var_D))) {
    E->rio_var_D = -E->rio_var_D;
LABEL_13:
    E->rio_var_C = -E->rio_var_C;
    E->rio_var_B = FUNC16(Rio_4);
    return;
  }
  if (Enemy_MoveDown(k, INT16_SHL8(E->rio_var_C)))
    goto LABEL_13;
  v4 = E->rio_var_C - 24;
  E->rio_var_C = v4;
  if (v4 < 0) {
    E->rio_var_A = E->rio_var_D;
    E->rio_var_D = 0;
    E->rio_var_C = 0;
    E->rio_var_B = FUNC16(Rio_5);
  } else if (E->rio_var_E) {
    E->rio_var_E = 0;
    Rio_6(addr_kRio_Ilist_BB97);
  }
}

void Rio_4(uint16 k) {  // 0xA2BCB7
  Enemy_Rio *E = Get_Rio(k);
  if (Enemy_MoveRight_IgnoreSlopes(k, INT16_SHL8(E->rio_var_D)))
    E->rio_var_D = -E->rio_var_D;
  if (Enemy_MoveDown(k, INT16_SHL8(E->rio_var_C))) {
    Rio_6(addr_kRio_Ilist_BBA3);
    E->rio_var_B = FUNC16(Rio_2);
  } else {
    E->rio_var_C -= 24;
  }
}

void Rio_5(uint16 k) {  // 0xA2BCFF
  Enemy_Rio *E = Get_Rio(k);
  if ((int16)(Get_Rio(k)->base.y_pos - samus_y_pos) >= 0) {
    E->rio_var_D = E->rio_var_A;
    E->rio_var_C = -1;
    E->rio_var_B = FUNC16(Rio_4);
  } else {
    int v1 = CalculateAngleOfSamusFromEnemy(k);
    E->rio_var_D = kSinCosTable8bit_Sext[v1 + 64];
    E->rio_var_C = kSinCosTable8bit_Sext[v1];
    Enemy_MoveRight_IgnoreSlopes(k, INT16_SHL8(E->rio_var_D));
    Enemy_MoveDown(k, INT16_SHL8(E->rio_var_C));
  }
}

void Rio_6(uint16 a) {  // 0xA2BD54
  Enemy_Rio *E = Get_Rio(cur_enemy_index);
  if (a != E->rio_var_F) {
    E->rio_var_F = a;
    E->base.current_instruction = a;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

const uint16 *NorfairLavajumpingEnemy_Instr_BE8E(uint16 k, const uint16 *jp) {  // 0xA2BE8E
  Get_NorfairLavajumpingEnemy(cur_enemy_index)->nley_var_00 = 1;
  return jp;
}

void NorfairLavajumpingEnemy_Init(void) {  // 0xA2BE99
  Enemy_NorfairLavajumpingEnemy *E = Get_NorfairLavajumpingEnemy(cur_enemy_index);
  E->nley_var_00 = 0;
  E->nley_var_01 = 0;
  if ((E->nley_parameter_1 & 0x8000) != 0) {
    E->base.current_instruction = addr_kNorfairLavajumpingEnemy_Ilist_BE62;
    E->nley_var_F = FUNC16(NorfairLavajumpingEnemy_Func_1);
  } else {
    E->nley_var_D = E->base.x_pos;
    E->nley_var_E = E->base.y_pos;
    E->base.current_instruction = addr_kNorfairLavajumpingEnemy_Ilist_BE3C;
    E->nley_var_F = FUNC16(NorfairLavajumpingEnemy_Func_2);
  }
}

void NorfairLavajumpingEnemy_Main(void) {  // 0xA2BED2
  NextRandom();
  Enemy_NorfairLavajumpingEnemy *E = Get_NorfairLavajumpingEnemy(cur_enemy_index);
  CallEnemyPreInstr(E->nley_var_F | 0xA20000);
}

void NorfairLavajumpingEnemy_Func_1(uint16 k) {  // 0xA2BEDC
  Enemy_NorfairLavajumpingEnemy *E = Get_NorfairLavajumpingEnemy(k);
  int v1 = k >> 1;
  if (enemy_drawing_queue[v1 + 100]) {
    uint16 v3 = enemy_drawing_queue[v1 + 109];
    E->base.frozen_timer = v3;
    if (v3 || (enemy_drawing_queue_sizes[v1 + 2] & 0x8000) == 0) {
      E->base.properties |= kEnemyProps_Invisible;
    } else {
      E->base.properties &= ~kEnemyProps_Invisible;
      E->base.y_pos = enemy_drawing_queue[v1 + 93];
    }
  } else {
    E->base.properties |= kEnemyProps_Deleted;
  }
}

void NorfairLavajumpingEnemy_Func_2(uint16 k) {  // 0xA2BF1A
  Enemy_NorfairLavajumpingEnemy *E = Get_NorfairLavajumpingEnemy(k);
  E->nley_var_C = g_word_A2BE86[(uint16)(HIBYTE(random_number) & 6) >> 1];
  E->nley_var_F = FUNC16(NorfairLavajumpingEnemy_Func_3);
  E->base.properties |= kEnemyProps_ProcessedOffscreen;
  QueueSfx2_Max6(0xD);
}

void NorfairLavajumpingEnemy_Func_3(uint16 k) {  // 0xA2BF3E
  Enemy_NorfairLavajumpingEnemy *E = Get_NorfairLavajumpingEnemy(k);
  int t = E->base.y_subpos + (LOBYTE(E->nley_var_C) << 8);
  E->base.y_subpos = t;
  E->base.y_pos += (int8)HIBYTE(E->nley_var_C) + (t >> 16);
  uint16 v3 = E->nley_var_C + 56;
  E->nley_var_C = v3;
  if (v3 >= 0xFC00) {
    NorfairLavajumpingEnemy_Func_6(addr_kNorfairLavajumpingEnemy_Ilist_BE42);
    E->nley_var_F = FUNC16(NorfairLavajumpingEnemy_Func_4);
  }
}

void NorfairLavajumpingEnemy_Func_4(uint16 k) {  // 0xA2BF7C
  Enemy_NorfairLavajumpingEnemy *E = Get_NorfairLavajumpingEnemy(k);
  int t = E->base.y_subpos + (LOBYTE(E->nley_var_C) << 8);
  E->base.y_subpos = t;
  E->base.y_pos += (int8)HIBYTE(E->nley_var_C) + (t >> 16);
  E->nley_var_C += 56;
  if (E->nley_var_00) {
    E->nley_var_00 = 0;
    E->nley_var_F = FUNC16(NorfairLavajumpingEnemy_Func_5);
  }
}

void NorfairLavajumpingEnemy_Func_5(uint16 k) {  // 0xA2BFBC
  Enemy_NorfairLavajumpingEnemy *E = Get_NorfairLavajumpingEnemy(k);
  int t = E->base.y_subpos + (LOBYTE(E->nley_var_C) << 8);
  E->base.y_subpos = t;
  E->base.y_pos = E->base.y_pos + (int8)HIBYTE(E->nley_var_C) + (t >> 16);
  if ((E->base.y_pos & 0xF0) == 240) {
    E->base.x_pos = E->nley_var_D;
    E->base.y_pos = E->nley_var_E;
    NorfairLavajumpingEnemy_Func_6(addr_kNorfairLavajumpingEnemy_Ilist_BE3C);
    E->nley_var_F = FUNC16(NorfairLavajumpingEnemy_Func_2);
    E->base.properties &= ~kEnemyProps_ProcessedOffscreen;
  } else {
    E->nley_var_C += 56;
  }
}

void NorfairLavajumpingEnemy_Func_6(uint16 a) {  // 0xA2C012
  Enemy_NorfairLavajumpingEnemy *E = Get_NorfairLavajumpingEnemy(cur_enemy_index);
  if (a != E->nley_var_01) {
    E->nley_var_01 = a;
    E->base.current_instruction = a;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

const uint16 *NorfairRio_Instr_C1C9(uint16 k, const uint16 *jp) {  // 0xA2C1C9
  Get_NorfairRio(cur_enemy_index)->nro_var_01 = 1;
  return jp;
}

const uint16 *NorfairRio_Instr_C1D4(uint16 k, const uint16 *jp) {  // 0xA2C1D4
  Get_NorfairRio(cur_enemy_index)->nro_var_02 = 8;
  return jp;
}

const uint16 *NorfairRio_Instr_C1DF(uint16 k, const uint16 *jp) {  // 0xA2C1DF
  Get_NorfairRio(cur_enemy_index)->nro_var_02 = 8;
  return jp;
}

const uint16 *NorfairRio_Instr_C1EA(uint16 k, const uint16 *jp) {  // 0xA2C1EA
  Get_NorfairRio(cur_enemy_index)->nro_var_02 = 12;
  return jp;
}

const uint16 *NorfairRio_Instr_C1F5(uint16 k, const uint16 *jp) {  // 0xA2C1F5
  Get_NorfairRio(cur_enemy_index)->nro_var_02 = -12;
  return jp;
}

const uint16 *NorfairRio_Instr_C200(uint16 k, const uint16 *jp) {  // 0xA2C200
  Get_NorfairRio(cur_enemy_index)->nro_var_02 = 4;
  return jp;
}

const uint16 *NorfairRio_Instr_C20B(uint16 k, const uint16 *jp) {  // 0xA2C20B
  Get_NorfairRio(cur_enemy_index)->nro_var_02 = 0;
  return jp;
}

const uint16 *NorfairRio_Instr_C216(uint16 k, const uint16 *jp) {  // 0xA2C216
  Get_NorfairRio(cur_enemy_index)->nro_var_02 = -4;
  return jp;
}

const uint16 *NorfairRio_Instr_C221(uint16 k, const uint16 *jp) {  // 0xA2C221
  Get_NorfairRio(cur_enemy_index)->nro_var_02 = -12;
  return jp;
}

const uint16 *NorfairRio_Instr_C22C(uint16 k, const uint16 *jp) {  // 0xA2C22C
  Get_NorfairRio(cur_enemy_index)->nro_var_02 = -16;
  return jp;
}

const uint16 *NorfairRio_Instr_C237(uint16 k, const uint16 *jp) {  // 0xA2C237
  Get_NorfairRio(cur_enemy_index)->nro_var_02 = 12;
  return jp;
}

void NorfairRio_Init(void) {  // 0xA2C242
  Enemy_NorfairRio *E = Get_NorfairRio(cur_enemy_index);
  E->nro_var_01 = 0;
  E->nro_var_02 = 0;
  if ((E->nro_parameter_1 & 0x8000) != 0) {
    E->nro_var_00 = addr_kNorfairRio_Ilist_C18F;
    E->base.current_instruction = addr_kNorfairRio_Ilist_C18F;
    E->nro_var_F = FUNC16(NorfairRio_Func_1);
  } else {
    E->nro_var_00 = addr_kNorfairRio_Ilist_C0F1;
    E->base.current_instruction = addr_kNorfairRio_Ilist_C0F1;
    E->nro_var_F = FUNC16(NorfairRio_Func_2);
  }
}

void NorfairRio_Main(void) {  // 0xA2C277
  NextRandom();
  Enemy_NorfairRio *E = Get_NorfairRio(cur_enemy_index);
  CallEnemyPreInstr(E->nro_var_F | 0xA20000);
}

void NorfairRio_Func_1(uint16 k) {  // 0xA2C281
  Enemy_NorfairRio *E = Get_NorfairRio(k);
  EnemySpawnData *v6;

  int v1 = k >> 1;
  if (enemy_drawing_queue[v1 + 100]) {
    uint16 v3 = enemy_drawing_queue[v1 + 109];
    E->base.frozen_timer = v3;
    if (v3
        || (E->base.properties &= ~kEnemyProps_Invisible,
            gEnemySpawnData(k)[31].some_flag == addr_kNorfairRio_Ilist_C0F1)) {
      E->base.properties |= kEnemyProps_Invisible;
    } else {
      uint16 r18 = addr_kNorfairRio_Ilist_C18F;
      v6 = gEnemySpawnData(k);
      if ((v6[31].field_4 & 0x8000) != 0)
        r18 = addr_kNorfairRio_Ilist_C1A3;
      NorfairRio_Func_7(r18);
      E->base.properties &= ~kEnemyProps_Invisible;
      E->base.x_pos = enemy_drawing_queue[v1 + 91];
      E->base.y_pos = v6[31].field_4 + enemy_drawing_queue[v1 + 93];
    }
  } else {
    E->base.properties |= kEnemyProps_Deleted;
  }
}

void NorfairRio_Func_2(uint16 k) {  // 0xA2C2E7
  Enemy_NorfairRio *E = Get_NorfairRio(k);
  if ((random_number & 0x101) != 0 && !(CompareDistToSamus_X(k, 0xC0) & 1)) {
    E->nro_var_A = g_word_A2C1C1[(uint16)((random_number >> 1) & 2) >> 1];
    E->nro_var_B = g_word_A2C1C5;
    if ((int16)(samus_x_pos - E->base.x_pos) < 0)
      E->nro_var_B = -E->nro_var_B;
    NorfairRio_Func_7(addr_kNorfairRio_Ilist_C107);
    E->nro_var_F = FUNC16(NorfairRio_Func_3);
  } else {
    if (E->nro_var_01) {
      E->nro_var_01 = 0;
      NorfairRio_Func_7(addr_kNorfairRio_Ilist_C0F1);
    }
  }
}

void NorfairRio_Func_3(uint16 k) {  // 0xA2C33F
  Enemy_NorfairRio *E = Get_NorfairRio(k);
  if (E->nro_var_01) {
    E->nro_var_01 = 0;
    NorfairRio_Func_7(addr_kNorfairRio_Ilist_C12F);
    E->nro_var_F = FUNC16(NorfairRio_Func_4);
    QueueSfx2_Max6(0x65);
  }
}

void NorfairRio_Func_4(uint16 k) {  // 0xA2C361
  int16 v4;

  Enemy_NorfairRio *E = Get_NorfairRio(k);
  if (Enemy_MoveRight_IgnoreSlopes(k, INT16_SHL8(E->nro_var_B)))
    E->nro_var_B = -E->nro_var_B;
  if (Enemy_MoveDown(k, INT16_SHL8(E->nro_var_A)) || (v4 = E->nro_var_A - 32, E->nro_var_A = v4, v4 < 0)) {
    E->nro_var_A = -1;
    NorfairRio_Func_7(addr_kNorfairRio_Ilist_C145);
    E->nro_var_F = FUNC16(NorfairRio_Func_5);
  }
}

void NorfairRio_Func_5(uint16 k) {  // 0xA2C3B1
  Enemy_NorfairRio *E = Get_NorfairRio(k);
  if (Enemy_MoveRight_IgnoreSlopes(k, INT16_SHL8(E->nro_var_B)))
    E->nro_var_B = -E->nro_var_B;
  if (Enemy_MoveDown(k, INT16_SHL8(E->nro_var_A))) {
    E->nro_var_F = FUNC16(NorfairRio_Func_6);
  } else {
    E->nro_var_A -= 32;
    if (E->nro_var_01) {
      E->nro_var_01 = 0;
      NorfairRio_Func_7(addr_kNorfairRio_Ilist_C179);
    }
  }
}

void NorfairRio_Func_6(uint16 k) {  // 0xA2C406
  Get_NorfairRio(k)->nro_var_F = FUNC16(NorfairRio_Func_2);
}

void NorfairRio_Func_7(uint16 a) {  // 0xA2C40D
  Enemy_NorfairRio *E = Get_NorfairRio(cur_enemy_index);
  if (a != E->nro_var_00) {
    E->nro_var_00 = a;
    E->base.current_instruction = a;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

const uint16 *LowerNorfairRio_Instr_C6D2(uint16 k, const uint16 *jp) {  // 0xA2C6D2
  Get_NorfairRio(cur_enemy_index)->nro_var_01 = 1;
  return jp;
}

const uint16 *LowerNorfairRio_Instr_C6DD(uint16 k, const uint16 *jp) {  // 0xA2C6DD
  Get_NorfairRio(cur_enemy_index)->nro_var_02 = 0;
  return jp;
}

const uint16 *LowerNorfairRio_Instr_C6E8(uint16 k, const uint16 *jp) {  // 0xA2C6E8
  Get_NorfairRio(cur_enemy_index)->nro_var_02 = 1;
  return jp;
}

void LowerNorfairRio_Init(void) {  // 0xA2C6F3
  Enemy_NorfairRio *E = Get_NorfairRio(cur_enemy_index);
  E->nro_var_01 = 0;
  if ((E->nro_parameter_1 & 0x8000) == 0) {
    E->nro_var_F = FUNC16(LowerNorfairRio_Func_2);
    E->nro_var_00 = addr_kLowerNorfairRio_Ilist_C61A;
    E->base.current_instruction = addr_kLowerNorfairRio_Ilist_C61A;
  } else {
    E->nro_var_F = FUNC16(LowerNorfairRio_Func_1);
    E->nro_var_00 = addr_kLowerNorfairRio_Ilist_C6B0;
    E->base.current_instruction = addr_kLowerNorfairRio_Ilist_C6B0;
  }
}

void LowerNorfairRio_Main(void) {  // 0xA2C724
  NextRandom();
  Enemy_LowerNorfairRio *E = Get_LowerNorfairRio(cur_enemy_index);
  CallEnemyPreInstr(E->lnro_var_F | 0xA20000);
}

void LowerNorfairRio_Func_1(uint16 k) {  // 0xA2C72E
  Enemy_LowerNorfairRio *E = Get_LowerNorfairRio(k);

  int v1 = k >> 1;
  if (enemy_drawing_queue[v1 + 100]) {
    uint16 v3 = enemy_drawing_queue[v1 + 109];
    E->base.frozen_timer = v3;
    if (!v3 && gEnemySpawnData(k)[31].field_4) {
      E->base.properties &= ~kEnemyProps_Invisible;
      E->base.x_pos = enemy_drawing_queue[v1 + 91];
      E->base.y_pos = enemy_drawing_queue[v1 + 93] + 12;
    } else {
      E->base.properties |= kEnemyProps_Invisible;
    }
  } else {
    E->base.properties |= kEnemyProps_Deleted;
  }
}

void LowerNorfairRio_Func_2(uint16 k) {  // 0xA2C771
  Enemy_NorfairRio *E = Get_NorfairRio(k);
  if ((random_number & 0x101) != 0 && !(CompareDistToSamus_X(k, 0x70) & 1)) {
    E->nro_var_C = g_word_A2C6CA;
    E->nro_var_D = g_word_A2C6CE;
    if ((int16)(samus_x_pos - E->base.x_pos) < 0)
      E->nro_var_D = -E->nro_var_D;
    LowerNorfairRio_Func_7(addr_kLowerNorfairRio_Ilist_C630);
    E->nro_var_F = FUNC16(LowerNorfairRio_Func_3);
  } else {
    E->nro_var_01 = 0;
    LowerNorfairRio_Func_7(addr_kLowerNorfairRio_Ilist_C61A);
  }
}

void LowerNorfairRio_Func_3(uint16 k) {  // 0xA2C7BB
  Enemy_NorfairRio *E = Get_NorfairRio(k);
  if (E->nro_var_01) {
    E->nro_var_01 = 0;
    LowerNorfairRio_Func_7(addr_kLowerNorfairRio_Ilist_C65A);
    E->nro_var_F = FUNC16(LowerNorfairRio_Func_4);
  }
}

void LowerNorfairRio_Func_4(uint16 k) {  // 0xA2C7D6
  int16 v4;

  Enemy_LowerNorfairRio *E = Get_LowerNorfairRio(k);
  if (Enemy_MoveRight_IgnoreSlopes(k, INT16_SHL8(E->lnro_var_D)))
    E->lnro_var_D = -E->lnro_var_D;
  if (Enemy_MoveDown(k, INT16_SHL8(E->lnro_var_C)) || (v4 = E->lnro_var_C - 32, E->lnro_var_C = v4, v4 < 0)) {
    E->lnro_var_C = -1;
    LowerNorfairRio_Func_7(addr_kLowerNorfairRio_Ilist_C662);
    E->lnro_var_F = FUNC16(LowerNorfairRio_Func_5);
    QueueSfx2_Max6(0x64);
  }
}

void LowerNorfairRio_Func_5(uint16 k) {  // 0xA2C82D
  Enemy_NorfairRio *E = Get_NorfairRio(k);
  if (Enemy_MoveRight_IgnoreSlopes(k, INT16_SHL8(E->nro_var_D)))
    E->nro_var_D = -E->nro_var_D;
  if (Enemy_MoveDown(k, INT16_SHL8(E->nro_var_C))) {
    LowerNorfairRio_Func_7(addr_kLowerNorfairRio_Ilist_C686);
    E->nro_var_F = FUNC16(LowerNorfairRio_Func_6);
  } else {
    E->nro_var_C -= 32;
    if (E->nro_var_01) {
      E->nro_var_01 = 0;
      LowerNorfairRio_Func_7(addr_kLowerNorfairRio_Ilist_C674);
    }
  }
}

void LowerNorfairRio_Func_6(uint16 k) {  // 0xA2C888
  Enemy_NorfairRio *E = Get_NorfairRio(k);
  if (E->nro_var_01) {
    E->nro_var_01 = 0;
    LowerNorfairRio_Func_7(addr_kLowerNorfairRio_Ilist_C686);
    E->nro_var_F = FUNC16(LowerNorfairRio_Func_2);
  }
}

void LowerNorfairRio_Func_7(uint16 a) {  // 0xA2C8A3
  Enemy_NorfairRio *E = Get_NorfairRio(cur_enemy_index);
  if (a != E->nro_var_00) {
    E->nro_var_00 = a;
    E->base.current_instruction = a;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

const uint16 *MaridiaLargeSnail_Instr_CB6B(uint16 k, const uint16 *jp) {  // 0xA2CB6B
  QueueSfx2_Max6(0xE);
  return jp;
}

const uint16 *MaridiaLargeSnail_Instr_CCB3(uint16 k, const uint16 *jp) {  // 0xA2CCB3
  Get_MaridiaLargeSnail(cur_enemy_index)->mlsl_var_02 = 1;
  return jp;
}

const uint16 *MaridiaLargeSnail_Instr_CCBE(uint16 k, const uint16 *jp) {  // 0xA2CCBE
  Get_MaridiaLargeSnail(cur_enemy_index)->mlsl_var_03 = 1;
  return jp;
}

const uint16 *MaridiaLargeSnail_Instr_CCC9(uint16 k, const uint16 *jp) {  // 0xA2CCC9
  Get_MaridiaLargeSnail(cur_enemy_index)->mlsl_var_03 = 0;
  return jp;
}

void MaridiaLargeSnail_Init(void) {  // 0xA2CCD4
  Enemy_MaridiaLargeSnail *E = Get_MaridiaLargeSnail(cur_enemy_index);
  E->mlsl_var_B = 0;
  E->mlsl_var_D = 0;
  E->mlsl_var_00 = 0;
  E->mlsl_var_01 = 0;
  E->mlsl_var_02 = 0;
  E->mlsl_var_03 = 0;
  E->mlsl_var_0A = 0;
  E->mlsl_var_E = 3;
  E->mlsl_var_C = 128;
  E->base.current_instruction = addr_kMaridiaLargeSnail_Ilist_CA4B;
  E->mlsl_var_A = FUNC16(MaridiaLargeSnail_Func_4);
  E->mlsl_var_F = FUNC16(MaridiaLargeSnail_Func_7);
}


void CallMaridiaLargeSnailFunc(uint32 ea) {
  switch (ea) {
  case fnMaridiaLargeSnail_Func_4: MaridiaLargeSnail_Func_4(); return;
  case fnMaridiaLargeSnail_Func_5: MaridiaLargeSnail_Func_5(); return;
  case fnMaridiaLargeSnail_Func_6: MaridiaLargeSnail_Func_6(); return;
  default: Unreachable();
  }
}
void MaridiaLargeSnail_Main(void) {  // 0xA2CD13
  MaridiaLargeSnail_Func_1(cur_enemy_index);
  Enemy_MaridiaLargeSnail *E = Get_MaridiaLargeSnail(cur_enemy_index);
  CallMaridiaLargeSnailFunc(E->mlsl_var_A | 0xA20000);
  MaridiaLargeSnail_Func_2(cur_enemy_index);
  MaridiaLargeSnail_Func_3(cur_enemy_index);
}

void MaridiaLargeSnail_Func_1(uint16 k) {  // 0xA2CD23
  MaridiaLargeSnail_Func_11();
  Enemy_MaridiaLargeSnail *E = Get_MaridiaLargeSnail(k);
  E->mlsl_var_06 = E->base.x_pos;
  E->mlsl_var_08 = E->base.y_pos;
}

void MaridiaLargeSnail_Func_2(uint16 k) {  // 0xA2CD35
  Enemy_MaridiaLargeSnail *E = Get_MaridiaLargeSnail(k);
  E->mlsl_var_0B = 0;
  if (CheckIfEnemyTouchesSamus(k))
    extra_samus_y_displacement = E->base.y_pos - E->mlsl_var_08;
  if (E->mlsl_var_04) {
    uint16 r18 = E->base.x_pos - E->mlsl_var_06;
    if ((r18 & 0x8000) != 0) {
      if (!E->mlsl_var_05)
        return;
    } else if (E->mlsl_var_05) {
      return;
    }
    extra_samus_x_displacement += r18;
  }
}

void MaridiaLargeSnail_Func_3(uint16 k) {  // 0xA2CD77
  int16 v2;

  Enemy_MaridiaLargeSnail *E = Get_MaridiaLargeSnail(k);
  if (E->mlsl_var_04) {
    v2 = E->base.x_pos - E->mlsl_var_06;
    if (!v2) {
      if (E->mlsl_var_05) {
        if (!E->mlsl_var_D || (joypad1_lastkeys & 0x100) == 0)
          return;
      } else if (E->mlsl_var_D || (joypad1_lastkeys & 0x200) == 0) {
        return;
      }
      E->mlsl_var_0B = 1;
      E->base.x_pos = E->mlsl_var_06;
      return;
    }
    if (v2 < 0) {
      if ((joypad1_lastkeys & 0x100) != 0) {
        E->mlsl_var_0B = 1;
        E->base.x_pos = E->mlsl_var_06;
      }
    } else if ((joypad1_lastkeys & 0x200) != 0) {
      E->mlsl_var_0B = 1;
      E->base.x_pos = E->mlsl_var_06;
    }
  }
}

void MaridiaLargeSnail_Func_4(void) {  // 0xA2CDE6
  Enemy_MaridiaLargeSnail *E = Get_MaridiaLargeSnail(cur_enemy_index);
  if (E->mlsl_var_E) {
    CallEnemyPreInstr(E->mlsl_var_F | 0xA20000);
  } else {
    E->mlsl_var_00 = 0;
    if ((GetSamusEnemyDelta_X(cur_enemy_index) & 0x8000) != 0) {
      E->mlsl_var_D = 1;
      E->mlsl_var_00 = 1;
    }
    MaridiaLargeSnail_Func_10(cur_enemy_index);
    if (IsSamusWithinEnemy_X(cur_enemy_index, 0x18)) {
      E->mlsl_var_00 |= 2;
      MaridiaLargeSnail_Func_10(cur_enemy_index);
      E->mlsl_var_A = FUNC16(MaridiaLargeSnail_Func_5);
    }
  }
}

void MaridiaLargeSnail_Func_5(void) {  // 0xA2CE2B
  Enemy_MaridiaLargeSnail *E = Get_MaridiaLargeSnail(cur_enemy_index);
  if (E->mlsl_var_E) {
    CallEnemyPreInstr(E->mlsl_var_F | 0xA20000);
  } else {
    if (!(Enemy_MoveDown(cur_enemy_index, INT16_SHL16(1)))) {
      E->mlsl_var_B = 0;
      E->mlsl_var_F = FUNC16(MaridiaLargeSnail_Func_7);
      E->mlsl_var_E = 3;
    }
  }
  if (!E->mlsl_var_0B) {
    uint16 R36 = 0;
    bool v4 = (--E->mlsl_var_C & 0x8000) != 0;
    if (v4 && (E->mlsl_var_C = 0, IsSamusWithinEnemy_X(cur_enemy_index, 0x20)) && E->mlsl_var_03 && !E->mlsl_var_E) {
      E->mlsl_var_00 = 0;
      E->mlsl_var_D = 0;
      if ((GetSamusEnemyDelta_X(cur_enemy_index) & 0x8000) != 0) {
        E->mlsl_var_00 = 1;
        E->mlsl_var_D = 1;
      }
      E->mlsl_var_C = 128;
      E->mlsl_var_00 = E->mlsl_var_00 & 1 | 4;
      MaridiaLargeSnail_Func_10(cur_enemy_index);
      E->mlsl_var_A = FUNC16(MaridiaLargeSnail_Func_6);
    } else {
      uint16 v5 = 128;
      if (E->mlsl_var_D)
        v5 = 132;
      int v6 = v5 >> 1;
      uint16 r18 = kCommonEnemySpeeds_Linear[v6 + 1];
      uint16 r20 = kCommonEnemySpeeds_Linear[v6];
      if (E->mlsl_var_04 && E->mlsl_var_D == E->mlsl_var_05) {
        if (E->mlsl_var_D)
          r20 -= 16;
        else
          r20 += 16;
      }
      if (Enemy_MoveRight_SlopesAsWalls(cur_enemy_index, __PAIR32__(r20, r18)))
        ++R36;
      if (E->mlsl_var_04 && E->mlsl_var_D == E->mlsl_var_05) {
        uint16 v7;
        if (E->mlsl_var_D)
          v7 = E->base.x_pos + 16;
        else
          v7 = E->base.x_pos - 16;
        E->base.x_pos = v7;
      }
      if (R36) {
        E->mlsl_var_D ^= 1;
        E->mlsl_var_00 ^= 4;
        MaridiaLargeSnail_Func_10(cur_enemy_index);
      }
    }
  }
}

void MaridiaLargeSnail_Func_6(void) {  // 0xA2CF40
  Enemy_MaridiaLargeSnail *E = Get_MaridiaLargeSnail(cur_enemy_index);
  if (E->mlsl_var_02) {
    E->mlsl_var_02 = 0;
    E->mlsl_var_00 -= 2;
    MaridiaLargeSnail_Func_10(cur_enemy_index);
    E->mlsl_var_A = FUNC16(MaridiaLargeSnail_Func_5);
  }
}

void MaridiaLargeSnail_Func_7(uint16 k) {  // 0xA2CF66
  Enemy_MaridiaLargeSnail *E = Get_MaridiaLargeSnail(k);
  uint16 v2 = E->mlsl_var_B + 384;
  if (!sign16(v2 - 0x4000))
    v2 = 0x4000;
  E->mlsl_var_B = v2;
  
  int v5 = (8 * (v2 >> 8)) >> 1;
  if (Enemy_MoveDown(k, kCommonEnemySpeeds_Quadratic32[v5 >> 1])) {
    --E->mlsl_var_E;
    int16 v6 = E->mlsl_var_B - 4096;
    if (v6 < 0)
      E->mlsl_var_E = 0;
    E->mlsl_var_B = v6;
    E->mlsl_var_F = FUNC16(MaridiaLargeSnail_Func_8);
  }
}

void MaridiaLargeSnail_Func_8(uint16 k) {  // 0xA2CFA9
  Enemy_MaridiaLargeSnail *E = Get_MaridiaLargeSnail(k);
  int16 v2 = E->mlsl_var_B - 384;
  E->mlsl_var_B = v2;
  if (v2 >= 0) {
    uint16 v3 = (v2 & 0x7F00) >> 8;
    int v5 = (8 * v3) >> 1;
    Enemy_MoveDown(k, kCommonEnemySpeeds_Quadratic32[(v5 + 2) >> 1]);
  } else {
    E->mlsl_var_B = 0;
    E->mlsl_var_F = FUNC16(MaridiaLargeSnail_Func_7);
  }
}
// 7DD25: mask 0xFF00 is shortened because ax.2 <= 0x7FFF

void MaridiaLargeSnail_Func_9(void) {  // 0xA2CFD7
  QueueSfx2_Max6(0x63);
}

void MaridiaLargeSnail_Func_10(uint16 k) {  // 0xA2CFDF
  Enemy_MaridiaLargeSnail *E = Get_MaridiaLargeSnail(k);
  uint16 mlsl_var_00 = E->mlsl_var_00;
  if (mlsl_var_00 != E->mlsl_var_01) {
    E->mlsl_var_01 = mlsl_var_00;
    E->base.current_instruction = g_off_A2CB77[mlsl_var_00];
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

void MaridiaLargeSnail_Func_11(void) {  // 0xA2CFFF
  Enemy_MaridiaLargeSnail *E = Get_MaridiaLargeSnail(cur_enemy_index);
  E->mlsl_var_04 = 0;
  E->mlsl_var_05 = 0;
  if (IsSamusWithinEnemy_Y(cur_enemy_index, 0x20) && IsSamusWithinEnemy_X(cur_enemy_index, 0x18)) {
    E->mlsl_var_04 = 1;
    if ((GetSamusEnemyDelta_X(cur_enemy_index) & 0x8000) != 0)
      E->mlsl_var_05 = 1;
  }
}

void MaridiaLargeSnail_Func_12(void) {  // 0xA2D388
  Enemy_NormalTouchAI_A2();
  MaridiaLargeSnail_Touch();
}

void MaridiaLargeSnail_Touch() {  // 0xA2D38C
  uint16 k = cur_enemy_index;
  if (!CheckIfEnemyTouchesSamus(k)) {
    if ((int16)(samus_x_pos - Get_MaridiaLargeSnail(cur_enemy_index)->base.x_pos) < 0)
      extra_samus_x_displacement -= 4;
    else
      extra_samus_x_displacement += 4;
  }
}

void MaridiaLargeSnail_Shot(void) {  // 0xA2D3B4
  NormalEnemyShotAi();
  QueueSfx2_Max6(0x57);
}

static uint16 HirisingSlowfalling_Func_1(uint16 k) {  // 0xA2DFCE
  Enemy_HirisingSlowfalling *E = Get_HirisingSlowfalling(k);
  return Mult8x8(E->hsg_parameter_1, E->hsg_var_F);
}

void HirisingSlowfalling_Init(void) {  // 0xA2DF76
  HirisingSlowfalling_Func_3(addr_kHirisingSlowfalling_Ilist_D82C);
  Enemy_HirisingSlowfalling *E = Get_HirisingSlowfalling(cur_enemy_index);
  E->hsg_var_A = FUNC16(HirisingSlowfalling_Func_4);
  E->hsg_var_00 = E->base.x_pos;
  E->hsg_var_01 = E->base.y_pos;
  int v2 = HIBYTE(E->hsg_parameter_1);
  E->hsg_var_E = g_off_A2DF5E[v2];
  E->hsg_var_F = *(uint16 *)RomPtr_A2(g_off_A2DF6A[v2]);
  uint16 r18 = HirisingSlowfalling_Func_1(cur_enemy_index);
  HirisingSlowfalling_Func_2(cur_enemy_index, r18);
  E->hsg_var_03 = E->hsg_var_01 - r18;
  E->hsg_var_02 = E->hsg_var_00;
  E->hsg_var_B = E->hsg_var_06;
}

void HirisingSlowfalling_Func_2(uint16 k, uint16 r18) {  // 0xA2DFE9
  uint32 v = 0;
  uint16 r24 = 0;
  do {
    r24 += 512;
    int v1 = 4 * (r24 >> 8);
    v += __PAIR32__(kCommonEnemySpeeds_Quadratic[v1 + 1], kCommonEnemySpeeds_Quadratic[v1]);
  } while (sign16((v >> 16) - r18));
  Get_HirisingSlowfalling(k)->hsg_var_06 = r24;
}

void HirisingSlowfalling_Func_3(uint16 a) {  // 0xA2E01E
  Enemy_HirisingSlowfalling *E = Get_HirisingSlowfalling(cur_enemy_index);
  E->base.current_instruction = a;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
}

void CallHirisingSlowfalling(uint32 ea) {
  switch (ea) {
  case fnHirisingSlowfalling_Func_4: HirisingSlowfalling_Func_4(); return;
  case fnHirisingSlowfalling_Func_5: HirisingSlowfalling_Func_5(); return;
  case fnHirisingSlowfalling_Func_6: HirisingSlowfalling_Func_6(); return;
  case fnHirisingSlowfalling_Func_7: HirisingSlowfalling_Func_7(); return;
  default: Unreachable();
  }
}

void HirisingSlowfalling_Main(void) {  // 0xA2E02E
  Enemy_HirisingSlowfalling *E = Get_HirisingSlowfalling(cur_enemy_index);
  CallHirisingSlowfalling(E->hsg_var_A | 0xA20000);
}

void HirisingSlowfalling_Func_4(void) {  // 0xA2E035
  if (IsSamusWithinEnemy_X(cur_enemy_index, 0x50)) {
    Enemy_HirisingSlowfalling *E = Get_HirisingSlowfalling(cur_enemy_index);
    E->hsg_var_07 = E->hsg_parameter_2;
    E->hsg_var_A = FUNC16(HirisingSlowfalling_Func_5);
  }
}

void HirisingSlowfalling_Func_5(void) {  // 0xA2E04F
  Enemy_HirisingSlowfalling *E = Get_HirisingSlowfalling(cur_enemy_index);
  int16 v1 = E->hsg_var_07 - 1;
  E->hsg_var_07 = v1;
  if (v1 < 0) {
    HirisingSlowfalling_Func_3(addr_kHirisingSlowfalling_Ilist_D834);
    E->hsg_var_A = FUNC16(HirisingSlowfalling_Func_6);
  }
}

void HirisingSlowfalling_Func_6(void) {  // 0xA2E06A
  Enemy_HirisingSlowfalling *E = Get_HirisingSlowfalling(cur_enemy_index);
  int v2 = 4 * HIBYTE(E->hsg_var_B);
  AddToHiLo(&E->base.y_pos, &E->base.y_subpos, __PAIR32__(kCommonEnemySpeeds_Quadratic[v2 + 3], kCommonEnemySpeeds_Quadratic[v2 + 2]));
  int16 v5 = E->hsg_var_B - 512;
  E->hsg_var_B = v5;
  if (v5 < 0) {
    E->base.x_pos = E->hsg_var_04 = E->hsg_var_02;
    E->base.y_pos = E->hsg_var_05 = E->hsg_var_03;
    E->hsg_var_C = 0;
    E->hsg_var_D = LOBYTE(E->hsg_parameter_1) - 1;
    HirisingSlowfalling_Func_3(addr_kHirisingSlowfalling_Ilist_D840);
    E->hsg_var_A = FUNC16(HirisingSlowfalling_Func_7);
  }
}

void HirisingSlowfalling_Func_7(void) {  // 0xA2E0CD
  Enemy_HirisingSlowfalling *E = Get_HirisingSlowfalling(cur_enemy_index);
  const uint8 *v1 = RomPtr_A2(E->hsg_var_E + 4 * HIBYTE(E->hsg_var_C));
  if (GET_WORD(v1) == 0x8000) {
    E->hsg_var_05 += E->hsg_var_F;
    E->hsg_var_C = 0;
    if ((--E->hsg_var_D & 0x8000) != 0) {
      E->hsg_var_B = E->hsg_var_06;
      E->base.x_pos = E->hsg_var_00;
      E->base.x_subpos = 0;
      E->base.y_pos = E->hsg_var_01;
      E->base.y_subpos = 0;
      HirisingSlowfalling_Func_3(addr_kHirisingSlowfalling_Ilist_D82C);
      E->hsg_var_A = FUNC16(HirisingSlowfalling_Func_4);
    }
  } else {
    E->base.x_pos = GET_WORD(v1) + E->hsg_var_04;
    E->base.y_pos = GET_WORD(v1 + 2) + E->hsg_var_05;
    E->hsg_var_C += 256;
  }
}

void Gripper_Init(void) {  // 0xA2E1D3
  Enemy_Gripper *E = Get_Gripper(cur_enemy_index);
  uint16 v1 = 8 * LOBYTE(E->base.current_instruction);
  E->gripper_var_E = v1;
  int v2 = v1 >> 1;
  uint16 v3;
  if ((E->base.current_instruction & 0xFEFF) != 0) {
    E->gripper_var_D = kCommonEnemySpeeds_Linear[v2];
    v3 = kCommonEnemySpeeds_Linear[v2 + 1];
  } else {
    E->gripper_var_D = kCommonEnemySpeeds_Linear[v2 + 2];
    v3 = kCommonEnemySpeeds_Linear[v2 + 3];
  }
  E->gripper_var_C = v3;
  uint16 v4 = addr_kGripper_Ilist_E19B;
  if ((E->gripper_var_D & 0x8000) == 0)
    v4 = addr_kGripper_Ilist_E1AF;
  E->base.current_instruction = v4;
  E->gripper_var_A = E->gripper_parameter_1;
  E->gripper_var_B = E->gripper_parameter_2;
}

void Gripper_Main(void) {  // 0xA2E221
  Enemy_Gripper *E = Get_Gripper(cur_enemy_index);
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, __PAIR32__(E->gripper_var_D, E->gripper_var_C)) ||
      Gripper_Func_1(cur_enemy_index) & 1 || Gripper_Func_2(cur_enemy_index) & 1) {
    int v3 = E->gripper_var_E >> 1;
    uint16 v4;
    if ((E->gripper_var_D & 0x8000) == 0) {
      E->gripper_var_D = kCommonEnemySpeeds_Linear[v3 + 2];
      E->gripper_var_C = kCommonEnemySpeeds_Linear[v3 + 3];
      v4 = addr_kGripper_Ilist_E19B;
    } else {
      E->gripper_var_D = kCommonEnemySpeeds_Linear[v3];
      E->gripper_var_C = kCommonEnemySpeeds_Linear[v3 + 1];
      v4 = addr_kGripper_Ilist_E1AF;
    }
    E->base.current_instruction = v4;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

uint8 Gripper_Func_1(uint16 k) {  // 0xA2E279
  Enemy_Gripper *E = Get_Gripper(k);
  return (E->gripper_var_D & 0x8000) != 0 && (int16)(E->base.x_pos - E->gripper_var_A) < 0;
}

uint8 Gripper_Func_2(uint16 k) {  // 0xA2E28A
  Enemy_Gripper *E = Get_Gripper(k);
  return (E->gripper_var_D & 0x8000) == 0 && (int16)(E->base.x_pos - E->gripper_var_B) >= 0;
}

void Gripper_Func_3(void) {  // 0xA2E29B
  Enemy_NormalFrozenAI_A2();
}

void Gripper_Func_4(void) {  // 0xA2E2A4
  NormalEnemyShotAi();
  Enemy_Gripper *E = Get_Gripper(cur_enemy_index);
  if (E->base.frozen_timer) {
    uint16 v1 = addr_kGripper_Sprmap_E43F;
    if ((E->gripper_var_D & 0x8000) == 0)
      v1 = addr_kGripper_Sprmap_E44B;
    E->base.spritemap_pointer = v1;
  }
}

void JetPowerRipper_Init(void) {  // 0xA2E318
  uint16 v0 = addr_kRipper_Ilist_E2E0;
  Enemy_JetPowerRipper *E = Get_JetPowerRipper(cur_enemy_index);
  if (E->jprr_parameter_2)
    v0 = addr_kRipper_Ilist_E2F4;
  E->base.current_instruction = v0;
  uint16 v2 = 8 * E->jprr_parameter_1;
  E->jprr_var_E = v2;
  int v3 = v2 >> 1;
  uint16 v4;
  if (E->jprr_parameter_2) {
    E->jprr_var_D = kCommonEnemySpeeds_Linear[v3];
    v4 = kCommonEnemySpeeds_Linear[v3 + 1];
  } else {
    E->jprr_var_D = kCommonEnemySpeeds_Linear[v3 + 2];
    v4 = kCommonEnemySpeeds_Linear[v3 + 3];
  }
  E->jprr_var_C = v4;
}

void JetPowerRipper_Main(void) {  // 0xA2E353
  int16 v3;

  Enemy_JetPowerRipper *E = Get_JetPowerRipper(cur_enemy_index);
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, __PAIR32__(E->jprr_var_D, E->jprr_var_C))) {
    int v2 = E->jprr_var_E >> 1;
    if ((E->jprr_var_D & 0x8000) == 0) {
      E->jprr_var_D = kCommonEnemySpeeds_Linear[v2 + 2];
      E->jprr_var_C = kCommonEnemySpeeds_Linear[v2 + 3];
      v3 = -7456;
    } else {
      E->jprr_var_D = kCommonEnemySpeeds_Linear[v2];
      E->jprr_var_C = kCommonEnemySpeeds_Linear[v2 + 1];
      v3 = -7436;
    }
    E->base.current_instruction = v3;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

void Ripper_Func_1(void) {  // 0xA2E3A0
  Enemy_NormalFrozenAI_A2();
}

void JetPowerRipper_Shot(void) {  // 0xA2E3A9
  int16 v1;

  NormalEnemyShotAi();
  Enemy_JetPowerRipper *E = Get_JetPowerRipper(cur_enemy_index);
  if (E->base.frozen_timer) {
    v1 = -7105;
    if ((E->jprr_var_D & 0x8000) == 0)
      v1 = -7093;
    E->base.spritemap_pointer = v1;
  }
}

void Ripper_Init(void) {  // 0xA2E49F
  uint16 v0 = addr_kRipper_Ilist_E477;
  Enemy_Ripper *E = Get_Ripper(cur_enemy_index);
  if (!E->ripper_parameter_2)
    v0 = addr_kRipper_Ilist_E48B;
  E->base.current_instruction = v0;
  uint16 v2 = 8 * E->ripper_parameter_1;
  E->ripper_var_E = v2;
  int v3 = v2 >> 1;
  uint16 v4;
  if (E->ripper_parameter_2) {
    E->ripper_var_D = kCommonEnemySpeeds_Linear[v3];
    v4 = kCommonEnemySpeeds_Linear[v3 + 1];
  } else {
    E->ripper_var_D = kCommonEnemySpeeds_Linear[v3 + 2];
    v4 = kCommonEnemySpeeds_Linear[v3 + 3];
  }
  E->ripper_var_C = v4;
}

void Ripper_Main(void) {  // 0xA2E4DA
  int16 v3;

  Enemy_Ripper *E = Get_Ripper(cur_enemy_index);
  if (Enemy_MoveRight_IgnoreSlopes(cur_enemy_index, __PAIR32__(E->ripper_var_D, E->ripper_var_C))) {
    int v2 = E->ripper_var_E >> 1;
    if ((E->ripper_var_D & 0x8000) == 0) {
      E->ripper_var_D = kCommonEnemySpeeds_Linear[v2 + 2];
      E->ripper_var_C = kCommonEnemySpeeds_Linear[v2 + 3];
      v3 = -7029;
    } else {
      E->ripper_var_D = kCommonEnemySpeeds_Linear[v2];
      E->ripper_var_C = kCommonEnemySpeeds_Linear[v2 + 1];
      v3 = -7049;
    }
    E->base.current_instruction = v3;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

const uint16 *LavaSeahorse_Instr_E5FB(uint16 k, const uint16 *jp) {  // 0xA2E5FB
  Get_LavaSeahorse(cur_enemy_index)->lse_var_02 = 1;
  return jp;
}

void LavaSeahorse_Init(void) {  // 0xA2E606
  Enemy_LavaSeahorse *E = Get_LavaSeahorse(cur_enemy_index);
  E->lse_var_02 = 0;
  if (E->lse_parameter_1) {
    E->lse_var_00 = 2;
    E->lse_var_01 = 2;
    E->base.current_instruction = addr_kLavaSeahorse_Ilist_E5A1;
    E->base.properties |= kEnemyProps_Tangible;
    E->lse_var_F = FUNC16(nullsub_196);
  } else {
    E->lse_var_00 = 0;
    E->lse_var_01 = 0;
    E->base.current_instruction = addr_kLavaSeahorse_Ilist_E59B;
    E->lse_var_F = FUNC16(LavaSeahorse_Func_1);
  }
}

void LavaSeahorse_Main(void) {  // 0xA2E64E
  Enemy_LavaSeahorse *E = Get_LavaSeahorse(cur_enemy_index);
  CallEnemyPreInstr(E->lse_var_F | 0xA20000);
}

void LavaSeahorse_Func_1(uint16 k) {  // 0xA2E654
  Enemy_LavaSeahorse *E = Get_LavaSeahorse(k);
  if ((--E->lse_var_D & 0x8000) != 0) {
    E->lse_var_D = 48;
    E->lse_var_F = FUNC16(LavaSeahorse_Func_2);
    uint16 t = GetSamusEnemyDelta_X(k);
    E->lse_var_A = (E->lse_var_A & 0x7fff) | (t & 0x8000);
    if ((E->lse_var_A & 0x8000) != 0) {
      LOBYTE(E->lse_var_00) &= ~1;
      Enemy_LavaSeahorse *E1 = Get_LavaSeahorse(k + 64);
      LOBYTE(E1->lse_var_00) &= ~1;
    } else {
      LOBYTE(E->lse_var_00) |= 1;
      Enemy_LavaSeahorse *E1 = Get_LavaSeahorse(k + 64);
      LOBYTE(E1->lse_var_00) |= 1;
    }
    LavaSeahorse_Func_6();
    LavaSeahorse_Func_7();
  }
}

void LavaSeahorse_Func_2(uint16 k) {  // 0xA2E6AD
  Enemy_LavaSeahorse *E = Get_LavaSeahorse(k);
  if ((--E->lse_var_D & 0x8000) != 0) {
    E->lse_var_00 += 4;
    E->lse_var_D = 3;
    E->lse_var_F = FUNC16(LavaSeahorse_Func_3);
  }
  --E->base.y_pos;
  Enemy_LavaSeahorse *E1 = Get_LavaSeahorse(k + 64);
  --E1->base.y_pos;
}

void LavaSeahorse_Func_3(uint16 k) {  // 0xA2E6F1
  LavaSeahorse_Func_6();
  Enemy_LavaSeahorse *E = Get_LavaSeahorse(k);
  if (E->lse_var_02) {
    E->lse_var_02 = 0;
    E->lse_var_01 = -1;
    SpawnEprojWithGfx(0xFFFF, k, addr_kEproj_LavaSeahorseFireball);
    QueueSfx2_Max6(0x61);
    if (E->lse_var_D-- == 1) {
      E->lse_var_00 -= 4;
      E->lse_var_D = 96;
      E->lse_var_F = FUNC16(LavaSeahorse_Func_4);
    }
  }
}

void LavaSeahorse_Func_4(uint16 k) {  // 0xA2E734
  Enemy_LavaSeahorse *E = Get_LavaSeahorse(k);
  if (E->lse_var_D-- == 1) {
    E->lse_var_D = 48;
    E->lse_var_F = FUNC16(LavaSeahorse_Func_5);
    LavaSeahorse_Func_6();
  }
}

void LavaSeahorse_Func_5(uint16 k) {  // 0xA2E749
  Enemy_LavaSeahorse *E = Get_LavaSeahorse(k);
  if ((--E->lse_var_D & 0x8000) != 0) {
    E->lse_var_D = 128;
    E->lse_var_F = FUNC16(LavaSeahorse_Func_1);
  }
  ++E->base.y_pos;
  Enemy_LavaSeahorse *E1 = Get_LavaSeahorse(k + 64);
  ++E1->base.y_pos;
}

void LavaSeahorse_Func_6(void) {  // 0xA2E782
  Enemy_LavaSeahorse *E = Get_LavaSeahorse(cur_enemy_index);
  uint16 lse_var_00 = E->lse_var_00;
  if (lse_var_00 != E->lse_var_01) {
    E->lse_var_01 = lse_var_00;
    E->base.current_instruction = g_off_A2E5EF[lse_var_00];
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

void LavaSeahorse_Func_7(void) {  // 0xA2E7A5
  Enemy_LavaSeahorse *E = Get_LavaSeahorse(cur_enemy_index + 64);
  uint16 lse_var_00 = E->lse_var_00;
  if (lse_var_00 != E->lse_var_01) {
    E->lse_var_01 = lse_var_00;
    E->base.current_instruction = g_off_A2E5EF[lse_var_00];
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

void LavaSeahorse_Touch(void) {  // 0xA2E7C8
  Enemy_NormalTouchAI_A2();
  LavaSeahorse_E7DA();
}

void LavaSeahorse_Shot(void) {  // 0xA2E7CE
  Enemy_NormalShotAI_A2();
  LavaSeahorse_E7DA();
}

void LavaSeahorse_Powerbomb(void) {  // 0xA2E7D4
  Enemy_NormalPowerBombAI_A2();
  LavaSeahorse_E7DA();
}

void LavaSeahorse_E7DA(void) {  // 0xA2E7DA
  Enemy_LavaSeahorse *E = Get_LavaSeahorse(cur_enemy_index);
  if (E->base.health) {
    uint16 shake_timer = E->base.shake_timer;
    Enemy_LavaSeahorse *E1 = Get_LavaSeahorse(cur_enemy_index + 64);
    E1->base.shake_timer = shake_timer;
    E1->base.invincibility_timer = E->base.invincibility_timer;
    E1->base.flash_timer = E->base.flash_timer;
    E1->base.frozen_timer = E->base.frozen_timer;
    E1->base.ai_handler_bits = E->base.ai_handler_bits;
  } else {
    Enemy_LavaSeahorse *E1 = Get_LavaSeahorse(cur_enemy_index + 64);
    E1->base.properties |= 0x200;
  }
}

void TimedShutter_Init(void) {  // 0xA2E9DA
  Enemy_TimedShutter *E = Get_TimedShutter(cur_enemy_index);
  E->tsr_var_A = g_off_A2EA4E[E->base.current_instruction + 2 * E->base.extra_properties];
  uint16 y_pos = E->base.y_pos;
  uint16 v3;
  if (E->base.extra_properties) {
    E->tsr_var_B = y_pos;
    uint16 v2 = y_pos - 8;
    E->tsr_var_C = v2;
    v2 -= 8;
    E->tsr_var_D = v2;
    v3 = v2 - 8;
  } else {
    E->tsr_var_B = y_pos;
    uint16 v4 = y_pos + 8;
    E->tsr_var_C = v4;
    v4 += 8;
    E->tsr_var_D = v4;
    v3 = v4 + 8;
  }
  E->tsr_var_E = v3;
  E->base.extra_properties = 0;
  E->tsr_var_F = 0;
  E->base.current_instruction = addr_kTimedShutter_Ilist_E998;
  int v5 = (uint16)(4 * LOBYTE(E->tsr_parameter_2)) >> 1;
  E->tsr_var_00 = g_word_A2EA56[v5];
  E->tsr_var_01 = g_word_A2EA56[v5 + 1];
}

void CallTimedShutterFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnTimedShutter_Func_1: TimedShutter_Func_1(k); return;
  case fnTimedShutter_Func_2: TimedShutter_Func_2(k); return;
  case fnTimedShutter_Func_3: TimedShutter_Func_3(k); return;
  case fnTimedShutter_Func_4: TimedShutter_Func_4(k); return;
  case fnTimedShutter_Func_5: TimedShutter_Func_5(k); return;
  case fnTimedShutter_Func_10: TimedShutter_Func_10(k); return;
  default: Unreachable();
  }
}

void TimedShutter_Main(void) {  // 0xA2EAB6
  Enemy_TimedShutter *E = Get_TimedShutter(cur_enemy_index);
  CallTimedShutterFunc(E->tsr_var_A | 0xA20000, cur_enemy_index);
}

void TimedShutter_Func_1(uint16 k) {  // 0xA2EABD
  Enemy_TimedShutter *E = Get_TimedShutter(k);
  uint16 tsr_parameter_1 = E->tsr_parameter_1;
  if (tsr_parameter_1) {
    E->tsr_parameter_1 = tsr_parameter_1 - 1;
  } else {
    RisingFallingPlatform_Func_8();
    E->tsr_var_A = FUNC16(TimedShutter_Func_10);
  }
}

void TimedShutter_Func_2(uint16 k) {  // 0xA2EAD1
  Enemy_TimedShutter *E = Get_TimedShutter(k);
  if (IsSamusWithinEnemy_X(k, E->tsr_parameter_1)) {
    RisingFallingPlatform_Func_8();
    E->tsr_var_A = FUNC16(TimedShutter_Func_10);
  }
}

void TimedShutter_Func_3(uint16 k) {  // 0xA2EAE7
  Enemy_TimedShutter *E = Get_TimedShutter(k);
  if (IsSamusWithinEnemy_X(k, E->tsr_parameter_1)) {
    RisingFallingPlatform_Func_8();
    E->tsr_var_A = FUNC16(TimedShutter_Func_5);
  }
}

void TimedShutter_Func_4(uint16 k) {  // 0xA2EAFD
  Enemy_TimedShutter *E = Get_TimedShutter(k);
  uint16 tsr_parameter_1 = E->tsr_parameter_1;
  if (tsr_parameter_1) {
    E->tsr_parameter_1 = tsr_parameter_1 - 1;
  } else {
    RisingFallingPlatform_Func_8();
    E->tsr_var_A = FUNC16(TimedShutter_Func_5);
  }
}

void TimedShutter_Func_Null(void) {
  ;
}


static Func_V *const off_A2EB1A[5] = { TimedShutter_Func_6, TimedShutter_Func_7, TimedShutter_Func_8, TimedShutter_Func_9, TimedShutter_Func_Null };

void TimedShutter_Func_5(uint16 k) {  // 0xA2EB11
  int v1 = Get_TimedShutter(k)->tsr_var_F;
  off_A2EB1A[v1]();
}

void TimedShutter_Func_6(void) {  // 0xA2EB25
  Enemy_TimedShutter *E = Get_TimedShutter(cur_enemy_index);
  AddToHiLo(&E->base.y_pos, &E->base.y_subpos, __PAIR32__(E->tsr_var_00, E->tsr_var_01));
  if ((int16)(E->tsr_var_B + 16 - E->base.y_pos) < 0) {
    E->base.y_pos = E->tsr_var_B + 9;
    ++E->tsr_var_F;
    E->base.instruction_timer = 1;
    E->base.current_instruction = addr_kTimedShutter_Ilist_E99E;
    E->base.y_height = 16;
  }
}

void TimedShutter_Func_7(void) {  // 0xA2EB66
  Enemy_TimedShutter *E = Get_TimedShutter(cur_enemy_index);
  AddToHiLo(&E->base.y_pos, &E->base.y_subpos, __PAIR32__(E->tsr_var_00, E->tsr_var_01));
  if ((int16)(E->tsr_var_C + 16 - E->base.y_pos) < 0) {
    E->base.y_pos = E->tsr_var_C + 9;
    ++E->tsr_var_F;
    E->base.instruction_timer = 1;
    E->base.current_instruction = addr_kTimedShutter_Ilist_E9A4;
    E->base.y_height = 24;
  }
}

void TimedShutter_Func_8(void) {  // 0xA2EBA7
  Enemy_TimedShutter *E = Get_TimedShutter(cur_enemy_index);
  AddToHiLo(&E->base.y_pos, &E->base.y_subpos, __PAIR32__(E->tsr_var_00, E->tsr_var_01));
  if ((int16)(E->tsr_var_D + 16 - E->base.y_pos) < 0) {
    E->base.y_pos = E->tsr_var_D + 9;
    ++E->tsr_var_F;
    E->base.instruction_timer = 1;
    E->base.current_instruction = addr_kTimedShutter_Ilist_E9AA;
    E->base.y_height = 32;
  }
}

void TimedShutter_Func_9(void) {  // 0xA2EBE8
  Enemy_TimedShutter *E = Get_TimedShutter(cur_enemy_index);
  AddToHiLo(&E->base.y_pos, &E->base.y_subpos, __PAIR32__(E->tsr_var_00, E->tsr_var_01));
  uint16 v3 = E->tsr_var_E + 16;
  if ((int16)(v3 - E->base.y_pos) < 0) {
    E->base.y_pos = v3;
    ++E->tsr_var_F;
  }
}

void TimedShutter_Func_Null2(void) {

}

static Func_V *const off_A2EC3A[5] = {  // 0xA2EC13
  TimedShutter_Func_11,
  TimedShutter_Func_12,
  TimedShutter_Func_13,
  TimedShutter_Func_14,
  TimedShutter_Func_Null2,
};

void TimedShutter_Func_10(uint16 k) {
  Enemy_TimedShutter *E = Get_TimedShutter(k);
  E->tsr_var_40 = E->base.y_pos;
  uint16 v2 = 2 * E->tsr_var_F;
  off_A2EC3A[v2 >> 1]();
  if (CheckIfEnemyTouchesSamus(k)) {
    int16 v4 = E->base.y_pos - E->tsr_var_40;
    if (v4 < 0)
      extra_samus_y_displacement += v4;
  }
}

void TimedShutter_Func_11(void) {  // 0xA2EC45
  Enemy_TimedShutter *E = Get_TimedShutter(cur_enemy_index);
  AddToHiLo(&E->base.y_pos, &E->base.y_subpos, -IPAIR32(E->tsr_var_00, E->tsr_var_01));
  int16 v3 = E->tsr_var_B - 16;
  if ((int16)(v3 - E->base.y_pos) >= 0) {
    E->base.y_pos = v3 + 7;
    ++E->tsr_var_F;
    E->base.instruction_timer = 1;
    E->base.current_instruction = addr_kTimedShutter_Ilist_E99E;
    E->base.y_height = 16;
  }
}

void TimedShutter_Func_12(void) {  // 0xA2EC86
  Enemy_TimedShutter *E = Get_TimedShutter(cur_enemy_index);
  AddToHiLo(&E->base.y_pos, &E->base.y_subpos, -IPAIR32(E->tsr_var_00, E->tsr_var_01));
  int16 v3 = E->tsr_var_C - 16;
  if ((int16)(v3 - E->base.y_pos) >= 0) {
    E->base.y_pos = v3 + 7;
    ++E->tsr_var_F;
    E->base.instruction_timer = 1;
    E->base.current_instruction = addr_kTimedShutter_Ilist_E9A4;
    E->base.y_height = 24;
  }
}

void TimedShutter_Func_13(void) {  // 0xA2ECC7
  Enemy_TimedShutter *E = Get_TimedShutter(cur_enemy_index);
  AddToHiLo(&E->base.y_pos, &E->base.y_subpos, -IPAIR32(E->tsr_var_00, E->tsr_var_01));
  int16 v3 = E->tsr_var_D - 16;
  if ((int16)(v3 - E->base.y_pos) >= 0) {
    E->base.y_pos = v3 + 7;
    ++E->tsr_var_F;
    E->base.instruction_timer = 1;
    E->base.current_instruction = addr_kTimedShutter_Ilist_E9AA;
    E->base.y_height = 32;
  }
}

void TimedShutter_Func_14(void) {  // 0xA2ED08
  Enemy_TimedShutter *E = Get_TimedShutter(cur_enemy_index);
  AddToHiLo(&E->base.y_pos, &E->base.y_subpos, -IPAIR32(E->tsr_var_00, E->tsr_var_01));
  uint16 v3 = E->tsr_var_E - 16;
  if ((int16)(v3 - E->base.y_pos) >= 0) {
    E->base.y_pos = v3;
    ++E->tsr_var_F;
  }
}

void TimedShutter_Func_15(void) {  // 0xA2ED33
  ;
}

void RisingFallingPlatform_Init(void) {  // 0xA2EE05
  RisingFallingPlatform_Func_1(cur_enemy_index);
  Get_RisingFallingPlatform(cur_enemy_index)->base.current_instruction = addr_kRisingFallingPlatform_Ilist_EDE7;
}

void ShootableShutter_Init(void) {  // 0xA2EE12
  RisingFallingPlatform_Func_1(cur_enemy_index);
  gEnemyData(cur_enemy_index)->current_instruction = addr_kTimedShutter_Ilist_E9AA;
}

void RisingFallingPlatform_Func_1(uint16 k) {  // 0xA2EE1F
  Enemy_RisingFallingPlatform *E = Get_RisingFallingPlatform(k);
  uint16 current_instruction_low = LOBYTE(E->base.current_instruction);
  E->rfpm_var_00 = current_instruction_low;
  int v3 = (8 * current_instruction_low) >> 1;
  E->rfpm_var_D = kCommonEnemySpeeds_Linear[v3];
  E->rfpm_var_C = kCommonEnemySpeeds_Linear[v3 + 1];
  E->rfpm_var_F = kCommonEnemySpeeds_Linear[v3 + 2];
  E->rfpm_var_E = kCommonEnemySpeeds_Linear[v3 + 3];
  uint16 current_instruction_high = HIBYTE(E->base.current_instruction);
  E->rfpm_var_01 = current_instruction_high;
  E->rfpm_var_20 = current_instruction_high;
  uint16 extra_properties_low = LOBYTE(E->base.extra_properties);
  E->rfpm_var_02 = extra_properties_low;
  E->rfpm_var_08 = 16 * extra_properties_low;
  uint16 extra_properties_high = HIBYTE(E->base.extra_properties);
  E->rfpm_var_03 = extra_properties_high;
  E->rfpm_var_09 = 16 * extra_properties_high;
  uint16 rfpm_parameter_1_low = LOBYTE(E->rfpm_parameter_1);
  E->rfpm_var_04 = rfpm_parameter_1_low;
  E->rfpm_var_07 = 2 * rfpm_parameter_1_low;
  E->rfpm_var_05 = HIBYTE(E->rfpm_parameter_1);
  uint16 rfpm_parameter_2 = E->rfpm_parameter_2;
  E->rfpm_var_06 = rfpm_parameter_2;
  E->rfpm_var_B = rfpm_parameter_2;
  uint16 y_pos = E->base.y_pos;
  E->rfpm_var_0F = y_pos;
  E->rfpm_var_10 = E->rfpm_var_05 + y_pos;
  if (!E->rfpm_var_01) {
    uint16 v10 = E->base.y_pos;
    E->rfpm_var_10 = v10;
    E->rfpm_var_0F = v10 - E->rfpm_var_05;
  }
  E->rfpm_var_A = FUNC16(RisingFallingPlatform_Func_2);
  E->base.extra_properties = 0;
  E->rfpm_var_0A = 0;
}

void CallRisingFallingPlatformFunc(uint32 ea) {
  switch (ea) {
  case fnRisingFallingPlatform_Func_2: RisingFallingPlatform_Func_2(); return;
  case fnRisingFallingPlatform_Func_4: RisingFallingPlatform_Func_4(); return;
  case fnRisingFallingPlatform_Func_9: RisingFallingPlatform_Func_9(); return;
  case fnRisingFallingPlatform_Func_10: RisingFallingPlatform_Func_10(); return;
  case fnRisingFallingPlatform_Func_11: RisingFallingPlatform_Func_11(); return;
  case fnRisingFallingPlatform_Func_12: RisingFallingPlatform_Func_12(); return;
  case fnRisingFallingPlatform_Func_13: RisingFallingPlatform_Func_13(); return;
  case fnHorizontalShootableShutter_Func_2: HorizontalShootableShutter_Func_2(); return;
  case fnHorizontalShootableShutter_Func_4: HorizontalShootableShutter_Func_4(); return;
  case fnHorizontalShootableShutter_Func_8: HorizontalShootableShutter_Func_8(); return;
  case fnHorizontalShootableShutter_Func_9: HorizontalShootableShutter_Func_9(); return;
  case fnHorizontalShootableShutter_Func_12: HorizontalShootableShutter_Func_12(); return;
  case fnHorizontalShootableShutter_Func_13: HorizontalShootableShutter_Func_13(); return;
  case fnHorizontalShootableShutter_Func_14: HorizontalShootableShutter_Func_14(); return;
  default: Unreachable();
  }
}

void RisingFallingPlatform_Main(void) {  // 0xA2EED1
  Enemy_RisingFallingPlatform *E = Get_RisingFallingPlatform(cur_enemy_index);
  CallRisingFallingPlatformFunc(E->rfpm_var_A | 0xA20000);
  if (E->rfpm_var_A != FUNC16(RisingFallingPlatform_Func_9)
      && E->rfpm_var_A != FUNC16(RisingFallingPlatform_Func_10)
      && (enemy_index_colliding_dirs[3] & (uint16)(enemy_index_colliding_dirs[2] & enemy_index_colliding_dirs[1] & enemy_index_colliding_dirs[0])) != 0xFFFF
      && (enemy_index_colliding_dirs[3] & (uint16)(enemy_index_colliding_dirs[2] & enemy_index_colliding_dirs[1] & enemy_index_colliding_dirs[0])) == cur_enemy_index) {
    if (samus_contact_damage_index)
      RisingFallingPlatform_Powerbomb();
  }
}

static Func_V *const off_A2EDFB[5] = {  // 0xA2EF09
  RisingFallingPlatform_Func_3,
  RisingFallingPlatform_Func_4,
  RisingFallingPlatform_Func_5,
  RisingFallingPlatform_Func_6,
  RisingFallingPlatform_Func_6,
};

void RisingFallingPlatform_Func_2(void) {

  int v0 = Get_RisingFallingPlatform(cur_enemy_index)->rfpm_var_07 >> 1;
  off_A2EDFB[v0]();
}

void RisingFallingPlatform_Func_3(void) {  // 0xA2EF15
  Enemy_RisingFallingPlatform *E = Get_RisingFallingPlatform(cur_enemy_index);
  if (E->rfpm_var_B-- == 1) {
    E->rfpm_var_B = E->rfpm_var_06;
    RisingFallingPlatform_Func_7(cur_enemy_index);
  }
}

void RisingFallingPlatform_Func_4(void) {  // 0xA2EF28
  Enemy_RisingFallingPlatform *E = Get_RisingFallingPlatform(cur_enemy_index);
  if (IsSamusWithinEnemy_X(cur_enemy_index, E->rfpm_var_06))
    RisingFallingPlatform_Func_7(cur_enemy_index);
}

void RisingFallingPlatform_Func_5(void) {  // 0xA2EF39
  RisingFallingPlatform_Func_7(cur_enemy_index);
}

void RisingFallingPlatform_Func_6(void) {  // 0xA2EF40
  ;
}

void RisingFallingPlatform_Func_7(uint16 k) {  // 0xA2EF44
  Enemy_RisingFallingPlatform *E = Get_RisingFallingPlatform(k);
  E->rfpm_var_A = FUNC16(RisingFallingPlatform_Func_9);
  if (E->rfpm_var_01)
    E->rfpm_var_A = FUNC16(RisingFallingPlatform_Func_10);
  RisingFallingPlatform_Func_8();
}

void RisingFallingPlatform_Func_8(void) {  // 0xA2EF5A
  if (!CheckIfEnemyIsOnScreen())
    QueueSfx3_Max6(0xE);
}

void RisingFallingPlatform_Func_9(void) {  // 0xA2EF68
  Enemy_RisingFallingPlatform *E = Get_RisingFallingPlatform(cur_enemy_index);
  E->rfpm_var_0E = E->base.y_pos;
  E->rfpm_var_0A = 0;
  if (CheckIfEnemyTouchesSamus(cur_enemy_index))
    E->rfpm_var_0A = 1;
  AddToHiLo(&E->base.y_pos, &E->base.y_subpos, __PAIR32__(E->rfpm_var_F, E->rfpm_var_E));
  if (E->rfpm_var_0A)
    extra_samus_y_displacement = E->base.y_pos - E->rfpm_var_0E;
  if ((int16)(E->rfpm_var_0F - E->base.y_pos) >= 0) {
    if (E->rfpm_var_08 == 4080) {
      E->rfpm_var_A = FUNC16(RisingFallingPlatform_Func_13);
    } else {
      E->rfpm_var_B = E->rfpm_var_08;
      E->rfpm_var_A = FUNC16(RisingFallingPlatform_Func_11);
    }
  }
}

void RisingFallingPlatform_Func_10(void) {  // 0xA2EFD4
  Enemy_RisingFallingPlatform *E = Get_RisingFallingPlatform(cur_enemy_index);
  E->rfpm_var_0E = E->base.y_pos;
  E->rfpm_var_0A = 0;
  if (CheckIfEnemyTouchesSamus(cur_enemy_index))
    E->rfpm_var_0A = 1;
  AddToHiLo(&E->base.y_pos, &E->base.y_subpos, __PAIR32__(E->rfpm_var_D, E->rfpm_var_C));
  if (E->rfpm_var_0A)
    extra_samus_y_displacement = E->base.y_pos - E->rfpm_var_0E;
  if ((int16)(E->base.y_pos - E->rfpm_var_10) >= 0) {
    if (E->rfpm_var_09 == 4080) {
      E->rfpm_var_A = FUNC16(RisingFallingPlatform_Func_13);
    } else {
      E->rfpm_var_B = E->rfpm_var_09;
      E->rfpm_var_A = FUNC16(RisingFallingPlatform_Func_12);
    }
  }
}

void RisingFallingPlatform_Func_11(void) {  // 0xA2F040
  Enemy_RisingFallingPlatform *E = Get_RisingFallingPlatform(cur_enemy_index);
  if ((--E->rfpm_var_B & 0x8000) != 0) {
    E->rfpm_var_A = FUNC16(RisingFallingPlatform_Func_10);
    if (E->rfpm_var_04 == 1 && E->rfpm_var_01)
      E->rfpm_var_A = FUNC16(RisingFallingPlatform_Func_4);
    if (Get_RisingFallingPlatform(cur_enemy_index)->base.enemy_ptr != 0xD83F)
      RisingFallingPlatform_Func_8();
  }
}

void RisingFallingPlatform_Func_12(void) {  // 0xA2F072
  Enemy_RisingFallingPlatform *E = Get_RisingFallingPlatform(cur_enemy_index);
  if ((--E->rfpm_var_B & 0x8000) != 0) {
    RisingFallingPlatform_Func_8();
    E->rfpm_var_A = FUNC16(RisingFallingPlatform_Func_9);
    if (E->rfpm_var_04 == 1 && !E->rfpm_var_01)
      E->rfpm_var_A = FUNC16(RisingFallingPlatform_Func_4);
  }
}

void RisingFallingPlatform_Func_13(void) {  // 0xA2F099
  ;
}

void RisingFallingPlatform_Touch(void) {  // 0xA2F09D
  RisingFallingPlatform_Powerbomb();
}

void RisingFallingPlatform_Shot(void) {  // 0xA2F0A2
  RisingFallingPlatform_Powerbomb();
}

void ShootableShutter_Shot(void) {  // 0xA2F0AA
  NormalEnemyShotAi();
  RisingFallingPlatform_Powerbomb();
}

void RisingFallingPlatform_Powerbomb(void) {  // 0xA2F0B6
  Enemy_RisingFallingPlatform *E = Get_RisingFallingPlatform(cur_enemy_index);
  if (sign16(E->rfpm_var_07 - 6)) {
LABEL_10:
    RisingFallingPlatform_Func_8();
    return;
  }
  if (E->rfpm_var_07 != 8) {
    if (E->rfpm_var_0C)
      return;
    E->rfpm_var_0C = 1;
  }
  if (E->rfpm_var_A != FUNC16(RisingFallingPlatform_Func_9)
      && E->rfpm_var_A != FUNC16(RisingFallingPlatform_Func_10)) {
    E->rfpm_var_A = FUNC16(RisingFallingPlatform_Func_9);
    if (E->rfpm_var_20)
      E->rfpm_var_A = FUNC16(RisingFallingPlatform_Func_10);
    E->rfpm_var_20 ^= 1;
    goto LABEL_10;
  }
}

void HorizontalShootableShutter_Init(void) {  // 0xA2F111
  HorizontalShootableShutter_Func_1(cur_enemy_index);
  Get_HorizontalShootableShutter(cur_enemy_index)->base.current_instruction = addr_kTimedShutter_Ilist_E9D4;
}

void HorizontalShootableShutter_Func_1(uint16 k) {  // 0xA2F11E
  Enemy_RisingFallingPlatform *E = Get_RisingFallingPlatform(k);
  uint16 current_instruction_low = LOBYTE(E->base.current_instruction);
  E->rfpm_var_00 = current_instruction_low;
  int v3 = (8 * current_instruction_low) >> 1;
  E->rfpm_var_D = kCommonEnemySpeeds_Linear[v3];
  E->rfpm_var_C = kCommonEnemySpeeds_Linear[v3 + 1];
  E->rfpm_var_F = kCommonEnemySpeeds_Linear[v3 + 2];
  E->rfpm_var_E = kCommonEnemySpeeds_Linear[v3 + 3];
  uint16 current_instruction_high = HIBYTE(E->base.current_instruction);
  E->rfpm_var_01 = current_instruction_high;
  E->rfpm_var_20 = current_instruction_high ^ 1;
  uint16 extra_properties_low = LOBYTE(E->base.extra_properties);
  E->rfpm_var_02 = extra_properties_low;
  E->rfpm_var_08 = 16 * extra_properties_low;
  uint16 extra_properties_high = HIBYTE(E->base.extra_properties);
  E->rfpm_var_03 = extra_properties_high;
  E->rfpm_var_09 = 16 * extra_properties_high;
  uint16 rfpm_parameter_1_low = LOBYTE(E->rfpm_parameter_1);
  E->rfpm_var_04 = rfpm_parameter_1_low;
  E->rfpm_var_07 = 2 * rfpm_parameter_1_low;
  E->rfpm_var_05 = HIBYTE(E->rfpm_parameter_1);
  uint16 rfpm_parameter_2 = E->rfpm_parameter_2;
  E->rfpm_var_06 = rfpm_parameter_2;
  E->rfpm_var_B = rfpm_parameter_2;
  uint16 x_pos = E->base.x_pos;
  E->rfpm_var_11 = x_pos;
  E->rfpm_var_12 = E->rfpm_var_05 + x_pos;
  if (!E->rfpm_var_01) {
    uint16 v10 = E->base.x_pos;
    E->rfpm_var_12 = v10;
    E->rfpm_var_11 = v10 - E->rfpm_var_05;
  }
  E->rfpm_var_A = FUNC16(HorizontalShootableShutter_Func_2);
  E->rfpm_var_15 = samus_x_pos;
  E->base.extra_properties = 0;
  E->rfpm_var_0A = 0;
  E->rfpm_var_0B = 0;
}

void HorizontalShootableShutter_Main(void) {  // 0xA2F1DE
  Enemy_RisingFallingPlatform *E = Get_RisingFallingPlatform(cur_enemy_index);
  CallRisingFallingPlatformFunc(E->rfpm_var_A | 0xA20000);
  if (E->rfpm_var_A != FUNC16(HorizontalShootableShutter_Func_8)
      && E->rfpm_var_A != FUNC16(HorizontalShootableShutter_Func_9)
      && (enemy_index_colliding_dirs[3] & (uint16)(enemy_index_colliding_dirs[2] & enemy_index_colliding_dirs[1] & enemy_index_colliding_dirs[0])) != 0xFFFF
      && (enemy_index_colliding_dirs[3] & (uint16)(enemy_index_colliding_dirs[2] & enemy_index_colliding_dirs[1] & enemy_index_colliding_dirs[0])) == cur_enemy_index
      && samus_contact_damage_index) {
    HorizontalShootableShutter_Powerbomb();
  }
  E->rfpm_var_15 = samus_x_pos;
  E->rfpm_var_16 = samus_x_subpos;
}

static Func_V *const off_A2F107[5] = {  // 0xA2F224
  HorizontalShootableShutter_Func_3,
  HorizontalShootableShutter_Func_4,
  HorizontalShootableShutter_Func_5,
  HorizontalShootableShutter_Func_6,
  HorizontalShootableShutter_Func_6,
};
void HorizontalShootableShutter_Func_2(void) {

  int v0 = Get_RisingFallingPlatform(cur_enemy_index)->rfpm_var_07 >> 1;
  off_A2F107[v0]();
}

void HorizontalShootableShutter_Func_3(void) {  // 0xA2F230
  Enemy_RisingFallingPlatform *E = Get_RisingFallingPlatform(cur_enemy_index);
  if (E->rfpm_var_B-- == 1) {
    E->rfpm_var_B = E->rfpm_var_06;
    HorizontalShootableShutter_Func_7(cur_enemy_index);
  }
}

void HorizontalShootableShutter_Func_4(void) {  // 0xA2F243
  Enemy_RisingFallingPlatform *E = Get_RisingFallingPlatform(cur_enemy_index);
  if (IsSamusWithinEnemy_X(cur_enemy_index, E->rfpm_var_06))
    HorizontalShootableShutter_Func_7(cur_enemy_index);
}

void HorizontalShootableShutter_Func_5(void) {  // 0xA2F254
  HorizontalShootableShutter_Func_7(cur_enemy_index);
}

void HorizontalShootableShutter_Func_6(void) {  // 0xA2F25B
  ;
}

void HorizontalShootableShutter_Func_7(uint16 k) {  // 0xA2F25F
  Enemy_RisingFallingPlatform *E = Get_RisingFallingPlatform(k);
  E->rfpm_var_A = FUNC16(HorizontalShootableShutter_Func_8);
  if (E->rfpm_var_01)
    E->rfpm_var_A = FUNC16(HorizontalShootableShutter_Func_9);
}

void HorizontalShootableShutter_Func_8(void) {  // 0xA2F272
  Enemy_RisingFallingPlatform *E = Get_RisingFallingPlatform(cur_enemy_index);
  E->rfpm_var_0D = E->base.x_pos;
  E->rfpm_var_13 = 0;
  if (EnemyFunc_AC67(cur_enemy_index) && (int16)(samus_x_pos - E->base.x_pos) < 0)
    E->rfpm_var_13 = 1;
  AddToHiLo(&E->base.x_pos, &E->base.x_subpos, __PAIR32__(E->rfpm_var_F, E->rfpm_var_E));
  if (E->rfpm_var_13) {
    extra_samus_x_subdisplacement = E->rfpm_var_E;
    extra_samus_x_displacement = E->rfpm_var_F;
    HorizontalShootableShutter_Func_10(cur_enemy_index);
  }
  if ((int16)(E->rfpm_var_11 - E->base.x_pos) >= 0) {
    if (E->rfpm_var_08 == 4080) {
      E->rfpm_var_A = FUNC16(HorizontalShootableShutter_Func_14);
    } else {
      E->rfpm_var_B = E->rfpm_var_08;
      E->rfpm_var_A = FUNC16(HorizontalShootableShutter_Func_12);
    }
  }
}

void HorizontalShootableShutter_Func_9(void) {  // 0xA2F2E4
  Enemy_RisingFallingPlatform *E = Get_RisingFallingPlatform(cur_enemy_index);
  E->rfpm_var_0D = E->base.x_pos;
  E->rfpm_var_13 = 0;
  if (EnemyFunc_AC67(cur_enemy_index) && (int16)(samus_x_pos - E->base.x_pos) >= 0)
    E->rfpm_var_13 = 1;
  AddToHiLo(&E->base.x_pos, &E->base.x_subpos, __PAIR32__(E->rfpm_var_D, E->rfpm_var_C));
  if (E->rfpm_var_13) {
    extra_samus_x_subdisplacement = E->rfpm_var_C;
    extra_samus_x_displacement = E->rfpm_var_D;
    HorizontalShootableShutter_Func_11(cur_enemy_index);
  }
  if ((int16)(E->base.x_pos - E->rfpm_var_12) >= 0) {
    if (E->rfpm_var_09 == 4080) {
      E->rfpm_var_A = FUNC16(HorizontalShootableShutter_Func_14);
    } else {
      E->rfpm_var_B = E->rfpm_var_09;
      E->rfpm_var_A = FUNC16(HorizontalShootableShutter_Func_13);
    }
  }
}

void HorizontalShootableShutter_Func_10(uint16 k) {  // 0xA2F356
  if (Get_RisingFallingPlatform(k)->rfpm_var_13) {
    if ((joypad1_lastkeys & 0x100) != 0) {
      extra_samus_x_displacement -= 4;
      extra_samus_y_subdisplacement = 0;
    }
  }
}

void HorizontalShootableShutter_Func_11(uint16 k) {  // 0xA2F371
  if (Get_RisingFallingPlatform(k)->rfpm_var_13) {
    if ((joypad1_lastkeys & 0x200) != 0) {
      extra_samus_x_displacement += 4;
      extra_samus_y_subdisplacement = 0;
    }
  }
}

void HorizontalShootableShutter_Func_12(void) {  // 0xA2F38C
  Enemy_RisingFallingPlatform *E = Get_RisingFallingPlatform(cur_enemy_index);
  if ((--E->rfpm_var_B & 0x8000) != 0) {
    E->rfpm_var_A = FUNC16(HorizontalShootableShutter_Func_9);
    if (E->rfpm_var_04 == 1) {
      if (E->rfpm_var_01)
        E->rfpm_var_A = FUNC16(HorizontalShootableShutter_Func_4);
    }
  }
}

void HorizontalShootableShutter_Func_13(void) {  // 0xA2F3B0
  Enemy_RisingFallingPlatform *E = Get_RisingFallingPlatform(cur_enemy_index);
  if ((--E->rfpm_var_B & 0x8000) != 0) {
    E->rfpm_var_A = FUNC16(HorizontalShootableShutter_Func_8);
    if (E->rfpm_var_04 == 1 && !E->rfpm_var_01)
      E->rfpm_var_A = FUNC16(HorizontalShootableShutter_Func_4);
  }
}

void HorizontalShootableShutter_Func_14(void) {  // 0xA2F3D4
  ;
}

void HorizontalShootableShutter_Touch(void) {  // 0xA2F3D8
  Enemy_HorizontalShootableShutter *E = Get_HorizontalShootableShutter(cur_enemy_index);
  if (E->hssr_var_A == FUNC16(HorizontalShootableShutter_Func_14)) {
    if ((int16)(samus_x_pos - E->base.x_pos) >= 0) {
      if ((joypad1_lastkeys & 0x200) != 0) {
        extra_samus_x_displacement = 4;
        extra_samus_y_subdisplacement = 0;
      }
    } else if ((joypad1_lastkeys & 0x100) != 0) {
      extra_samus_x_displacement = -4;
      extra_samus_y_subdisplacement = 0;
    }
  }
}

void HorizontalShootableShutter_Shot(void) {  // 0xA2F40E
  NormalEnemyShotAiSkipDeathAnim_CurEnemy();
  HorizontalShootableShutter_Powerbomb();
}

void HorizontalShootableShutter_Powerbomb(void) {  // 0xA2F41A
  Enemy_RisingFallingPlatform *E = Get_RisingFallingPlatform(cur_enemy_index);
  if (!sign16(E->rfpm_var_07 - 6)) {
    if (E->rfpm_var_07 != 8) {
      if (E->rfpm_var_0C)
        return;
      E->rfpm_var_0C = 1;
    }
    if (E->rfpm_var_A != FUNC16(HorizontalShootableShutter_Func_8)
        && E->rfpm_var_A != FUNC16(HorizontalShootableShutter_Func_9)) {
      E->rfpm_var_20 ^= 1;
      E->rfpm_var_A = FUNC16(HorizontalShootableShutter_Func_8);
      if (E->rfpm_var_20)
        E->rfpm_var_A = FUNC16(HorizontalShootableShutter_Func_9);
    }
  }
}
