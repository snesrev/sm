// Aran
#include "ida_types.h"
#include "variables.h"
#include "funcs.h"
#include "sm_rtl.h"

static Func_V *const kSamusInputHandlers[28] = {
  Samus_Input_00_Standing,
  Samus_Input_01_Running,
  Samus_Input_02_NormalJumping,
  Samus_Input_03_SpinJumping,
  Samus_Input_04_MorphBallOnGround,
  Samus_Input_05_Crouching,
  Samus_Input_06_Falling,
  Samus_Input_07_Unused,
  Samus_Input_08_MorphBallFalling,
  Samus_Input_09_Unused,
  Samus_Input_0A_KnockbackOrCrystalFlashEnding,
  Samus_Input_0B_Unused,
  Samus_Input_0C_Unused,
  Samus_Input_0D_Unused,
  Samus_Input_0E_TurningAroundOnGround,
  Samus_Input_0F_CrouchingEtcTransition,
  Samus_Input_10_Moonwalking,
  Samus_Input_11_SpringBallOnGround,
  Samus_Input_12_SpringBallInAir,
  Samus_Input_13_SpringBallFalling,
  Samus_Input_14_WallJumping,
  Samus_Input_15_RanIntoWall,
  Samus_Input_16_Grappling,
  Samus_Input_17_TurningAroundJumping,
  Samus_Input_18_TurningAroundFalling,
  Samus_Input_19_DamageBoost,
  Samus_Input_1A_GrabbedByDraygon,
  Samus_Input_1B_ShinesparkEtc,
};
void Samus_InputHandler(void) {  // 0x918000
  kSamusInputHandlers[samus_movement_type]();
}

void Samus_Input_00_Standing(void) {  // 0x91804D
  if (samus_pose && samus_pose != kPose_9B_FaceF_VariaGravitySuit || !elevator_status)
    Samus_LookupTransitionTable();
}

void Samus_Input_01_Running(void) {  // 0x918066
  Samus_LookupTransitionTable();
}

void Samus_Input_02_NormalJumping(void) {  // 0x91806E
  Samus_LookupTransitionTable();
}

void Samus_Input_03_SpinJumping(void) {  // 0x918076
  Samus_LookupTransitionTable();
}

void Samus_Input_04_MorphBallOnGround(void) {  // 0x91807E
  Samus_LookupTransitionTable();
}

void Samus_Input_07_Unused(void) {  // 0x918086
  ;
}

void Samus_Input_05_Crouching(void) {  // 0x918087
  if (time_is_frozen_flag) {
    Samus_Func20();
  } else {
    Samus_LookupTransitionTable();
    if (!samus_movement_type) {
      samus_y_pos -= 5;
      samus_prev_y_pos -= 5;
    }
  }
}

void Samus_Input_06_Falling(void) {  // 0x9180B6
  Samus_LookupTransitionTable();
}

void Samus_Input_08_MorphBallFalling(void) {  // 0x91810A
  Samus_LookupTransitionTable();
}

void Samus_Input_09_Unused(void) {  // 0x918112
  ;
}

void Samus_Input_0A_KnockbackOrCrystalFlashEnding(void) {  // 0x918113
  Samus_LookupTransitionTable();
  if (samus_movement_type != kMovementType_0A_KnockbackOrCrystalFlashEnding) {
    Samus_InitJump();
    samus_knockback_timer = 0;
  }
}

void Samus_Input_0B_Unused(void) {  // 0x91812D
  ;
}

void Samus_Input_0C_Unused(void) {  // 0x918132
  Samus_LookupTransitionTable();
}

void Samus_Input_0D_Unused(void) {  // 0x91813A
  Samus_LookupTransitionTable();
}

void Samus_Input_0E_TurningAroundOnGround(void) {  // 0x918142
  Samus_LookupTransitionTable();
}

void Samus_Input_0F_CrouchingEtcTransition(void) {  // 0x918146
  ;
}

void Samus_Input_10_Moonwalking(void) {  // 0x918147
  Samus_LookupTransitionTable();
}

void Samus_Input_11_SpringBallOnGround(void) {  // 0x91814F
  Samus_LookupTransitionTable();
}

void Samus_Input_12_SpringBallInAir(void) {  // 0x918157
  Samus_LookupTransitionTable();
}

void Samus_Input_13_SpringBallFalling(void) {  // 0x91815F
  Samus_LookupTransitionTable();
}

void Samus_Input_14_WallJumping(void) {  // 0x918167
  Samus_LookupTransitionTable();
}

void Samus_Input_15_RanIntoWall(void) {  // 0x91816F
  if (time_is_frozen_flag)
    Samus_Func20();
  else
    Samus_LookupTransitionTable();
}

void Samus_Input_16_Grappling(void) {  // 0x918181
  Samus_LookupTransitionTable();
}

void Samus_Input_17_TurningAroundJumping(void) {  // 0x918189
  Samus_LookupTransitionTable();
}

void Samus_Input_18_TurningAroundFalling(void) {  // 0x91818D
  Samus_LookupTransitionTable();
}

void Samus_Input_19_DamageBoost(void) {  // 0x918191
  Samus_LookupTransitionTable();
}

void Samus_Input_1A_GrabbedByDraygon(void) {  // 0x918199
  Samus_LookupTransitionTable();
}

void Samus_Input_1B_ShinesparkEtc(void) {  // 0x9181A1
  Samus_LookupTransitionTable();
}

void Samus_LookupTransitionTable(void) {  // 0x9181A9
  PoseEntry *pe;
  int16 v2;
  PoseEntry *v5;
  PoseEntry *v6;

  if (joypad1_lastkeys) {
    TranslateCustomControllerBindingsToDefault();
    uint16 v0 = kPoseTransitionTable[samus_pose];
    pe = get_PoseEntry(v0);
    v2 = pe->new_input + 1;
    if (pe->new_input != 0xFFFF) {
      while (1) {
        uint16 v3 = v2 - 1;
        if (!v3 || (R18_ & v3) == 0) {
          uint16 cur_input = get_PoseEntry(v0)->cur_input;
          if (!cur_input || (R20_ & cur_input) == 0)
            break;
        }
        v0 += 6;
        v5 = get_PoseEntry(v0);
        v2 = v5->new_input + 1;
        if (v5->new_input == 0xFFFF)
          goto LABEL_8;
      }
      v6 = get_PoseEntry(v0);
      if (v6->new_pose != samus_pose) {
        samus_new_pose = v6->new_pose;
        bomb_jump_dir = 0;
      }
    }
  } else {
LABEL_8:
    UNUSED_word_7E0A18 = 0;
    Samus_Pose_CancelGrapple();
  }
}

void TranslateCustomControllerBindingsToDefault(void) {  // 0x9181F4
  R18_ = joypad1_newkeys & (kButton_Up | kButton_Down | kButton_Left | kButton_Right);
  R20_ = joypad1_lastkeys & (kButton_Up | kButton_Down | kButton_Left | kButton_Right);
  uint16 v0 = joypad1_newkeys;
  if ((button_config_shoot_x & joypad1_newkeys) != 0) {
    R18_ |= kButton_X;
    v0 = joypad1_newkeys;
  }
  if ((button_config_jump_a & v0) != 0) {
    R18_ |= kButton_A;
    v0 = joypad1_newkeys;
  }
  if ((button_config_run_b & v0) != 0) {
    R18_ |= kButton_B;
    v0 = joypad1_newkeys;
  }
  if ((button_config_itemcancel_y & v0) != 0) {
    R18_ |= kButton_Y;
    v0 = joypad1_newkeys;
  }
  if ((button_config_aim_up_R & v0) != 0) {
    if ((button_config_aim_up_R & (kButton_L | kButton_R)) != 0)
      R18_ |= kButton_R;
    v0 = joypad1_newkeys;
  }
  if ((button_config_aim_down_L & v0) != 0 && (button_config_aim_down_L & (kButton_L | kButton_R)) != 0)
    R18_ |= kButton_L;
  R18_ = ~R18_;
  uint16 v1 = joypad1_lastkeys;
  if ((button_config_shoot_x & joypad1_lastkeys) != 0) {
    R20_ |= kButton_X;
    v1 = joypad1_lastkeys;
  }
  if ((button_config_jump_a & v1) != 0) {
    R20_ |= kButton_A;
    v1 = joypad1_lastkeys;
  }
  if ((button_config_run_b & v1) != 0) {
    R20_ |= kButton_B;
    v1 = joypad1_lastkeys;
  }
  if ((button_config_itemcancel_y & v1) != 0) {
    R20_ |= kButton_Y;
    v1 = joypad1_lastkeys;
  }
  if ((button_config_aim_up_R & v1) != 0) {
    if ((button_config_aim_up_R & (kButton_L | kButton_R)) != 0)
      R20_ |= kButton_R;
    v1 = joypad1_lastkeys;
  }
  if ((button_config_aim_down_L & v1) != 0 && (button_config_aim_down_L & (kButton_L | kButton_R)) != 0)
    R20_ |= kButton_L;
  R20_ = ~R20_;
}

void Samus_Pose_CancelGrapple(void) {  // 0x9182D9
  if (Samus_Pose_Func2() & 1 || *(&kPoseParams[0].new_pose_unless_buttons + (uint16)(8 * samus_pose)) == 255)
    samus_new_pose = samus_pose;
  else
    samus_new_pose = *(&kPoseParams[0].new_pose_unless_buttons + (uint16)(8 * samus_pose));
}

static const uint8 kSamus_Pose_Func2_Tab[28] = {  // 0x918304
  2, 1, 1, 0, 6, 2, 8, 2, 1, 6, 2, 2, 2, 6, 2, 2,
  2, 6, 6, 6, 6, 2, 6, 2, 2, 2, 2, 2,
};

uint8 Samus_Pose_Func2(void) {

  uint16 v0 = kSamus_Pose_Func2_Tab[samus_movement_type];
  if (v0 != 1)
    goto LABEL_2;
  if (!samus_x_base_speed && !samus_x_base_subspeed) {
    v0 = 2;
LABEL_2:
    samus_momentum_routine_index = v0;
    return 0;
  }
  samus_momentum_routine_index = 1;
  return 1;
}

void EnableDemoInput(void) {  // 0x91834E
  samus_input_handler = FUNC16(Samus_InputHandler_E91D);
  demo_enable |= 0x8000u;
}

void DisableDemoInput(void) {  // 0x91835F
  samus_input_handler = FUNC16(Samus_InputHandler_E913);
  demo_enable &= ~0x8000u;
}

void ResetDemoData(void) {  // 0x918370
  demo_input_pre_instr = 0;
  demo_input_instr_timer = 0;
  demo_input_instr_ptr = 0;
  demo_timer_counter = 0;
  xray_angle = 0;
  demo_input = 0;
  demo_input_new = 0;
  demo_input_prev = 0;
  demo_input_prev_new = 0;
  demo_enable = 0;
}

void LoadDemoInputObject(uint16 a, uint16 j) {  // 0x918395
  DemoInputObject *dio;
  xray_angle = a;
  dio = get_DemoInputObject(j);
  demo_input_pre_instr = dio->pre_instr;
  demo_input_instr_ptr = dio->instr_ptr;
  demo_input_instr_timer = 1;
  demo_timer_counter = 0;
  //  Call(dio->ptr | 0x910000);
}

void DemoObjectInputHandler(void) {  // 0x9183C0
  if ((demo_enable & 0x8000u) != 0) {
    if (demo_input_instr_ptr) {
      ProcessDemoInputObject();
      joypad1_input_samusfilter = demo_input_prev;
      joypad1_newinput_samusfilter = demo_input_prev_new;
      joypad1_lastkeys = demo_input;
      demo_input_prev = demo_input;
      joypad1_newkeys = demo_input_new;
      demo_input_prev_new = demo_input_new;
    }
  }
}

void CallDemoPreInstr(uint32 ea) {
  switch (ea) {
  case fnDemoPreInstr_nullsub_162: return;
  case fnDemoPreInstr_864F: DemoPreInstr_864F(); return;
  case fnDemoPreInstr_866A: DemoPreInstr_866A(); return;
  case fnDemoPreInstr_CheckLeaveDemo: DemoPreInstr_CheckLeaveDemo(); return;
  case fnDemoPreInstr_8AB0: DemoPreInstr_8AB0(); return;
  default: Unreachable();
  }
}

uint16 CallDemoInstr(uint32 ea, uint16 k, uint16 j) {
  switch (ea) {
  case fnDemoInstr_Finish: return DemoInstr_Finish(k, j);
  case fnDemoInstr_SetPreInstr: return DemoInstr_SetPreInstr(k, j);
  case fnDemoInstr_ClearPreInstr: return DemoInstr_ClearPreInstr(k, j);
  case fnDemoInstr_Goto: return DemoInstr_Goto(k, j);
  case fnDemoInstr_DecTimerAndGoto: return DemoInstr_DecTimerAndGoto(k, j);
  case fnDemoInstr_SetTimer: return DemoInstr_SetTimer(k, j);
  case fnDemoInstr_Func2: return DemoInstr_Func2(k, j);
  case fnDemoInstr_Disable: return DemoInstr_Disable(k, j);
  case fnDemoInstr_Func3: return DemoInstr_Func3(k, j);
  default: return Unreachable();
  }
}

void ProcessDemoInputObject(void) {  // 0x9183F2
  CallDemoPreInstr(demo_input_pre_instr | 0x910000);
  if (!--demo_input_instr_timer) {
    uint16 v0 = demo_input_instr_ptr;
    uint16 *v1;
    while (1) {
      v1 = (uint16 *)RomPtr_91(v0);
      uint16 v2 = *v1;
      if ((*v1 & 0x8000u) == 0)
        break;
      R18_ = *v1;
      v0 = CallDemoInstr(v2 | 0x910000, 0, v0 + 2);
      if (!v0)
        return;
    }
    demo_input_instr_timer = *v1;
    uint8 *v3 = RomPtr_91(v0);
    demo_input = *((uint16 *)v3 + 1);
    demo_input_new = *((uint16 *)v3 + 2);
    demo_input_instr_ptr = v0 + 6;
  }
}

uint16 DemoInstr_Finish(uint16 k, uint16 j) {  // 0x918427
  demo_input_instr_ptr = 0;
  demo_input = 0;
  demo_input_new = 0;
  return 0;
}

uint16 DemoInstr_SetPreInstr(uint16 k, uint16 j) {  // 0x918434
  demo_input_pre_instr = *(uint16 *)RomPtr_91(j);
  return j + 2;
}

uint16 DemoInstr_ClearPreInstr(uint16 k, uint16 j) {  // 0x91843F
  demo_input_pre_instr = 0x8447;
  return j;
}

uint16 DemoInstr_Goto(uint16 k, uint16 j) {  // 0x918448
  return *(uint16 *)RomPtr_91(j);
}

uint16 DemoInstr_DecTimerAndGoto(uint16 k, uint16 j) {  // 0x91844F
  if (--demo_timer_counter)
    return DemoInstr_Goto(k, j);
  else
    return j + 2;
}

uint16 DemoInstr_SetTimer(uint16 k, uint16 j) {  // 0x918459
  demo_timer_counter = *(uint16 *)RomPtr_91(j);
  return j + 2;
}

uint16 DemoInstr_Func2(uint16 k, uint16 j) {  // 0x9185FC
  frame_handler_alfa = FUNC16(Samus_FrameHandlerAlfa_Func11);
  frame_handler_beta = FUNC16(Samus_FrameHandlerBeta_Func17);
  DisableDemoInput();
  return j;
}

void DemoPreInstr_864F(void) {  // 0x91864F
  if (sign16(samus_x_pos - 178)) {
    demo_input_pre_instr = FUNC16(DemoPreInstr_866A);
    demo_input_instr_ptr = 0x8623;
    demo_input_instr_timer = 1;
  }
}

void DemoPreInstr_866A(void) {  // 0x91866A
  if (!enemy_projectile_x_pos[0]) {
    demo_input_pre_instr = FUNC16(DemoPreInstr_nullsub_162);
    demo_input_instr_ptr = addr_off_91864B;
    demo_input_instr_timer = 1;
  }
}

uint16 DemoInstr_Disable(uint16 k, uint16 j) {  // 0x918682
  frame_handler_alfa = FUNC16(EmptyFunction);
  frame_handler_beta = FUNC16(EmptyFunction);
  DisableDemoInput();
  return j;
}

void UNUSED_DemoInstr_Func4(void) {  // 0x9186FE
  frame_handler_alfa = FUNC16(Samus_FrameHandlerAlfa_Func13);
  samus_pose = kPose_02_FaceL_Normal;
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
  samus_last_different_pose = samus_prev_pose;
  *(uint16 *)&samus_last_different_pose_x_dir = *(uint16 *)&samus_prev_pose_x_dir;
  samus_prev_pose = samus_pose;
  *(uint16 *)&samus_prev_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
  DisableDemoInput();
  samus_input_handler = FUNC16(nullsub_152);
}

uint16 DemoInstr_Func3(uint16 k, uint16 j) {  // 0x918739
  frame_handler_alfa = FUNC16(Samus_FrameHandlerAlfa_Func13);
  samus_pose = kPose_02_FaceL_Normal;
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
  samus_last_different_pose = samus_prev_pose;
  *(uint16 *)&samus_last_different_pose_x_dir = *(uint16 *)&samus_prev_pose_x_dir;
  samus_prev_pose = samus_pose;
  *(uint16 *)&samus_prev_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
  DisableDemoInput();
  samus_input_handler = FUNC16(nullsub_152);
  return j;
}

static Func_V *const kDemoSetFuncPtrs_0[6] = {  // 0x918790
  DemoSetFunc_0,
  DemoSetFunc_1,
  DemoSetFunc_1,
  DemoSetFunc_2,
  DemoSetFunc_1,
  DemoSetFunc_3,
};
static Func_V *const kDemoSetFuncPtrs_1[6] = {
  DemoSetFunc_2,
  DemoSetFunc_2,
  DemoSetFunc_1,
  DemoSetFunc_2,
  DemoSetFunc_1,
  DemoSetFunc_4,
};
static Func_V *const kDemoSetFuncPtrs_2[6] = {
  DemoSetFunc_2,
  DemoSetFunc_5,
  DemoSetFunc_1,
  DemoSetFunc_1,
  DemoSetFunc_1,
  DemoSetFunc_1,
};
static Func_V *const kDemoSetFuncPtrs_3[5] = {
  DemoSetFunc_6,
  DemoSetFunc_2,
  DemoSetFunc_2,
  DemoSetFunc_1,
  DemoSetFunc_7,
};

static Func_V *const *const kDemoSetFuncPtrs[4] = {
  kDemoSetFuncPtrs_0,
  kDemoSetFuncPtrs_1,
  kDemoSetFuncPtrs_2,
  kDemoSetFuncPtrs_3,
};

void LoadDemoData(void) {
  DemoSetDef *DemoSetDef;

  R18_ = 16 * demo_scene;
  uint16 v0 = 16 * demo_scene + kDemoSetDefPtrs[demo_set];
  DemoSetDef = get_DemoSetDef(v0);
  collected_items = DemoSetDef->items;
  equipped_items = collected_items;
  samus_max_missiles = DemoSetDef->missiles;
  samus_missiles = samus_max_missiles;
  samus_max_super_missiles = DemoSetDef->super_missiles;
  samus_super_missiles = samus_max_super_missiles;
  samus_max_power_bombs = DemoSetDef->power_bombs;
  samus_power_bombs = samus_max_power_bombs;
  samus_max_health = DemoSetDef->health;
  samus_health = samus_max_health;
  collected_beams = DemoSetDef->beams;
  equipped_beams = DemoSetDef->equipped_beams_;
  samus_reserve_health = 0;
  ResetDemoData();
  EnableDemoInput();
  uint16 demo_obj = get_DemoSetDef(v0)->demo_obj;
  LoadDemoInputObject(demo_obj, demo_obj);
  R18_ = 2 * demo_scene;
  kDemoSetFuncPtrs[demo_set][demo_scene]();
  frame_handler_alfa = FUNC16(Samus_FrameHandlerAlfa_Func12);
  frame_handler_beta = FUNC16(Samus_FrameHandlerBeta_Func14);
  samus_momentum_routine_index = 0;
  samus_special_transgfx_index = 0;
  samus_hurt_switch_index = 0;
  Samus_LoadSuitPalette();
  UpdateBeamTilesAndPalette();
  button_config_up = kButton_Up;
  button_config_down = kButton_Down;
  button_config_left = kButton_Left;
  button_config_right = kButton_Right;
  button_config_shoot_x = kButton_X;
  button_config_jump_a = kButton_A;
  button_config_run_b = kButton_B;
  button_config_itemcancel_y = kButton_Y;
  button_config_itemswitch = kButton_Select;
  button_config_aim_up_R = kButton_R;
  button_config_aim_down_L = kButton_L;
  UNUSED_word_7E09E8 = 1;
  debug_flag = 1;
  moonwalk_flag = 0;
  UNUSED_word_7E0DF8 = 0;
  UNUSED_word_7E0DFA = 0;
  UNUSED_word_7E0DFC = 0;
}

void DemoSetFunc_0(void) {  // 0x918A33
  MakeSamusFaceForward();
  samus_draw_handler = FUNC16(SamusDrawHandler_Default);
}

void DemoSetFunc_3(void) {  // 0x918A3E
  DemoSetFunc_Common(0x1Fu);
}

void DemoSetFunc_7(void) {  // 0x918A43
  samus_health = 20;
  DemoSetFunc_2();
}

void DemoSetFunc_2(void) {  // 0x918A49
  DemoSetFunc_Common(2u);
}

void DemoSetFunc_4(void) {  // 0x918A4E
  DemoSetFunc_Common(0x2Au);
}

void DemoSetFunc_1(void) {  // 0x918A53
  DemoSetFunc_Common(1u);
}

void DemoSetFunc_Common(uint16 a) {  // 0x918A56
  samus_pose = a;
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
  samus_draw_handler = FUNC16(SamusDrawHandler_Default);
}

void DemoSetFunc_5(void) {  // 0x918A68
  samus_draw_handler = FUNC16(SamusDrawHandler_Default);
  Projectile_Func7_Shinespark();
  samus_pose = kPose_CD_FaceR_Shinespark_Diag;
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
}

void DemoSetFunc_6(void) {  // 0x918A81
  samus_draw_handler = FUNC16(SamusDrawHandler_Default);
  Projectile_Func7_Shinespark();
  samus_pose = kPose_CA_FaceL_Shinespark_Horiz;
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
}

void DemoPreInstr_CheckLeaveDemo(void) {  // 0x918A9B
  if (game_state == kGameState_44_TransitionFromDemo) {
    demo_input_instr_ptr = addr_kDemoInstrs_LeaveDemo;
    demo_input_instr_timer = 1;
  }
}

void DemoPreInstr_8AB0(void) {  // 0x918AB0
  if (samus_movement_type != kMovementType_1A_GrabbedByDraygon) {
    demo_input_pre_instr = FUNC16(DemoPreInstr_CheckLeaveDemo);
    demo_input_instr_ptr = addr_stru_919346;
    demo_input_instr_timer = 1;
  }
}


static Func_V *const kXrayHdmaFuncs[5] = {  // 0x91BE11
  XrayHdmaFunc_BeamAimedR,
  XrayHdmaFunc_BeamAimedU,
  XrayHdmaFunc_BeamAimedD,
  XrayHdmaFunc_BeamAimedL,
  XrayHdmaFunc_BeamHoriz,
};

void CalculateXrayHdmaTable_OriginOffScreen(uint16 k, uint16 j) {
  int16 v2;
  uint16 v3;

  LOBYTE(v2) = HIBYTE(k);
  HIBYTE(v2) = k;
  R22_ = v2 & 0xFF00;
  R24_ = j;
  R26_ = R18_ - R20_;
  R26_ += (R26_ & 0x8000u) != 0 ? 0x100 : 0;
  R28_ = R20_ + R18_;
  if (!sign16(R20_ + R18_ - 257))
    R28_ = R20_ + R18_ - 256;
  if (!R20_ && (R18_ == 64 || R18_ == 192)) {
    v3 = 8;
  } else {
    if (sign16(R26_ - 128)) {
      if (sign16(R26_ - 64)) {
        if (!sign16(R28_ - 64)) {
          v3 = 0;
          goto LABEL_16;
        }
        goto LABEL_13;
      }
    } else {
      if (!sign16(R26_ - 192)) {
LABEL_13:
        v3 = 2;
        goto LABEL_16;
      }
      if (!sign16(R28_ - 192)) {
        v3 = 6;
        goto LABEL_16;
      }
    }
    v3 = 4;
  }
LABEL_16:;
  uint16 v4 = R26_;
  if (!sign16(R26_ - 128))
    v4 = R26_ - 128;
  g_word_7E001E = kTanTable[v4];
  uint16 v5 = R28_;
  if (!sign16(R28_ - 128))
    v5 = R28_ - 128;
  R32 = kTanTable[v5];
  kXrayHdmaFuncs[v3 >> 1]();
}

void XrayHdmaFunc_BeamAimedR(void) {  // 0x91BEC2
  int16 v0;
  int16 v2;

  v0 = 2 * (R24_ - 1);
  R34 = R22_;
  R36 = R22_;
  while (1) {
    bool v1 = __CFADD__uint16(g_word_7E001E, R34);
    R34 += g_word_7E001E;
    if (v1)
      break;
    v0 -= 2;
    if (v0 < 0) {
      R18_ = 0;
      goto LABEL_10;
    }
  }
  IndirWriteWord(&R0_, v0, HIBYTE(R34) | 0xFF00);
  R18_ = v0 + 2;
  v2 = v0 - 2;
  while (1) {
    bool v1 = __CFADD__uint16(g_word_7E001E, R34);
    R34 += g_word_7E001E;
    if (v1)
      break;
    IndirWriteWord(&R0_, v2, HIBYTE(R34) | 0xFF00);
    v2 -= 2;
    if (v2 < 0)
      goto LABEL_10;
  }
  do {
    IndirWriteWord(&R0_, v2, 0xFFu);
    v2 -= 2;
  } while (v2 >= 0);
LABEL_10:;
  uint16 v3 = 2 * R24_;
  while (1) {
    bool v1 = __CFADD__uint16(R32, R36);
    R36 += R32;
    if (v1)
      break;
    v3 += 2;
    if ((int16)(v3 - 460) >= 0) {
      R20_ = v3;
      goto LABEL_19;
    }
  }
  IndirWriteWord(&R0_, v3, HIBYTE(R36) | 0xFF00);
  R20_ = v3 - 2;
  uint16 v4;
  v4 = v3 - 2 + 4;
  while (1) {
    bool v1 = __CFADD__uint16(R32, R36);
    R36 += R32;
    if (v1)
      break;
    IndirWriteWord(&R0_, v4, HIBYTE(R36) | 0xFF00);
    v4 += 2;
    if ((int16)(v4 - 460) >= 0)
      goto LABEL_19;
  }
  do {
    IndirWriteWord(&R0_, v4, 0xFFu);
    v4 += 2;
  } while ((int16)(v4 - 460) < 0);
LABEL_19:;
  uint16 v5 = R18_;
  do {
    do {
      IndirWriteWord(&R0_, v5, 0xFF00u);
      v5 += 2;
    } while ((int16)(v5 - R20_) < 0);
  } while (v5 == R20_);
}

void XrayHdmaFunc_BeamAimedL(void) {  // 0x91BF72
  int16 v0;
  int16 v2;

  v0 = 2 * (R24_ - 1);
  R34 = R22_;
  R36 = R22_;
  while (1) {
    bool v1 = R36 >= R32;
    R36 -= R32;
    if (!v1)
      break;
    v0 -= 2;
    if (v0 < 0) {
      R18_ = 0;
      goto LABEL_10;
    }
  }
  IndirWriteWord(&R0_, v0, R36 & 0xFF00);
  R18_ = v0 + 2;
  v2 = v0 - 2;
  while (1) {
    bool v1 = R36 >= R32;
    R36 -= R32;
    if (!v1)
      break;
    IndirWriteWord(&R0_, v2, R36 & 0xFF00);
    v2 -= 2;
    if (v2 < 0)
      goto LABEL_10;
  }
  do {
    IndirWriteWord(&R0_, v2, 0xFFu);
    v2 -= 2;
  } while (v2 >= 0);
LABEL_10:;
  uint16 v3 = 2 * R24_;
  while (1) {
    bool v1 = R34 >= g_word_7E001E;
    R34 -= g_word_7E001E;
    if (!v1)
      break;
    v3 += 2;
    if ((int16)(v3 - 460) >= 0) {
      R20_ = v3;
      goto LABEL_19;
    }
  }
  IndirWriteWord(&R0_, v3, R34 & 0xFF00);
  R20_ = v3 - 2;
  uint16 v4;
  v4 = v3 - 2 + 4;
  while (1) {
    bool v1 = R34 >= g_word_7E001E;
    R34 -= g_word_7E001E;
    if (!v1)
      break;
    IndirWriteWord(&R0_, v4, R34 & 0xFF00);
    v4 += 2;
    if ((int16)(v4 - 460) >= 0)
      goto LABEL_19;
  }
  do {
    IndirWriteWord(&R0_, v4, 0xFFu);
    v4 += 2;
  } while ((int16)(v4 - 460) < 0);
LABEL_19:;
  uint16 v5 = R18_;
  do {
    do {
      IndirWriteWord(&R0_, v5, 0xFF00u);
      v5 += 2;
    } while ((int16)(v5 - R20_) < 0);
  } while (v5 == R20_);
}

static Func_Y_V *const off_91C063[3] = {  // 0x91C022
  XrayHdmaFunc_BeamAimedUUR,
  XrayHdmaFunc_BeamAimedUU,
  XrayHdmaFunc_BeamAimedUUL,
};

void XrayHdmaFunc_BeamAimedU(void) {
  uint16 v0;
  R34 = R22_;
  R36 = R22_;
  if (sign16(R26_ - 192)) {
    v0 = 0;
  } else if (sign16(R28_ - 192)) {
    v0 = 2;
  } else {
    v0 = 4;
  }
  R26_ = 2 * R24_;
  off_91C063[v0 >> 1](2 * (R24_ - 1));
  uint16 v1 = R18_;
  do {
    IndirWriteWord(&R0_, v1, 0xFFu);
    v1 += 2;
  } while ((int16)(v1 - 460) < 0);
}

void XrayHdmaFunc_BeamAimedUUR(uint16 v0) {  // 0x91C069
  int16 v2;
  int16 v3;
  int16 v4;
  int16 v6;

  v6 = v0;
  while (1) {
    bool v1 = __CFADD__uint16(g_word_7E001E, R34);
    R34 += g_word_7E001E;
    if (v1)
      break;
    v0 -= 2;
    if (v0 < 0) {
      R18_ = 0;
      goto LABEL_12;
    }
  }
  IndirWriteByte(&R0_, v0, HIBYTE(R34));
  v2 = v0 - 2;
  if (v2 >= 0) {
    R18_ = v2 + 4;
    while (1) {
      bool v1 = __CFADD__uint16(g_word_7E001E, R34);
      R34 += g_word_7E001E;
      if (v1)
        break;
      IndirWriteByte(&R0_, v2, HIBYTE(R34));
      v2 -= 2;
      if (v2 < 0)
        goto LABEL_12;
    }
    do {
      IndirWriteByte(&R0_, v2, 0xFFu);
      v2 -= 2;
    } while (v2 >= 0);
  } else {
    R18_ = 2;
  }
LABEL_12:
  v3 = v6 + 1;
  while (1) {
    bool v1 = __CFADD__uint16(R32, R36);
    R36 += R32;
    if (v1)
      break;
    v3 -= 2;
    if (v3 < 0) {
      R20_ = 1;
      goto LABEL_23;
    }
  }
  IndirWriteByte(&R0_, v3, HIBYTE(R36));
  v4 = v3 - 2;
  if (v4 >= 0) {
    R20_ = v4 + 4;
    while (1) {
      bool v1 = __CFADD__uint16(R32, R36);
      R36 += R32;
      if (v1)
        break;
      IndirWriteByte(&R0_, v4, HIBYTE(R36));
      v4 -= 2;
      if (v4 < 0)
        goto LABEL_23;
    }
    do {
      IndirWriteByte(&R0_, v4, 0xFFu);
      v4 -= 2;
    } while (v4 >= 0);
  } else {
    R20_ = 3;
  }
LABEL_23:;
  uint16 v5 = R18_;
  do {
    IndirWriteByte(&R0_, v5, 0);
    v5 += 2;
  } while ((int16)(v5 - R20_) < 0);
  R18_ = R20_ - 1;
}

void XrayHdmaFunc_BeamAimedUU(uint16 j) {  // 0x91C123
  int16 v2;
  int16 v3;
  int16 v4;

  uint16 v5 = j;
  while (1) {
    bool v1 = R34 >= g_word_7E001E;
    R34 -= g_word_7E001E;
    if (!v1)
      break;
    j -= 2;
    if ((j & 0x8000u) != 0) {
      R18_ = 0;
      goto LABEL_12;
    }
  }
  IndirWriteByte(&R0_, j, HIBYTE(R34));
  v2 = j - 2;
  if (v2 >= 0) {
    R18_ = v2 + 4;
    while (1) {
      bool v1 = R34 >= g_word_7E001E;
      R34 -= g_word_7E001E;
      if (!v1)
        break;
      IndirWriteByte(&R0_, v2, HIBYTE(R34));
      v2 -= 2;
      if (v2 < 0)
        goto LABEL_12;
    }
    do {
      IndirWriteByte(&R0_, v2, 0);
      v2 -= 2;
    } while (v2 >= 0);
  } else {
    R18_ = 2;
  }
LABEL_12:
  v3 = v5 + 1;
  while (1) {
    bool v1 = __CFADD__uint16(R32, R36);
    R36 += R32;
    if (v1)
      break;
    v3 -= 2;
    if (v3 < 0) {
      R20_ = 1;
      return;
    }
  }
  IndirWriteByte(&R0_, v3, HIBYTE(R36));
  v4 = v3 - 2;
  if (v4 >= 0) {
    R20_ = v4 + 4;
    while (1) {
      bool v1 = __CFADD__uint16(R32, R36);
      R36 += R32;
      if (v1)
        break;
      IndirWriteByte(&R0_, v4, HIBYTE(R36));
      v4 -= 2;
      if (v4 < 0)
        return;
    }
    do {
      IndirWriteByte(&R0_, v4, 0xFFu);
      v4 -= 2;
    } while (v4 >= 0);
  } else {
    R20_ = 3;
  }
}

void XrayHdmaFunc_BeamAimedUUL(uint16 j) {  // 0x91C1CA
  int16 v2;
  int16 v3;
  int16 v4;

  uint16 v6 = j;
  while (1) {
    bool v1 = R34 >= g_word_7E001E;
    R34 -= g_word_7E001E;
    if (!v1)
      break;
    j -= 2;
    if ((j & 0x8000u) != 0) {
      R18_ = 0;
      goto LABEL_12;
    }
  }
  IndirWriteByte(&R0_, j, HIBYTE(R34));
  v2 = j - 2;
  if (v2 >= 0) {
    R18_ = v2 + 4;
    while (1) {
      bool v1 = R34 >= g_word_7E001E;
      R34 -= g_word_7E001E;
      if (!v1)
        break;
      IndirWriteByte(&R0_, v2, HIBYTE(R34));
      v2 -= 2;
      if (v2 < 0)
        goto LABEL_12;
    }
    do {
      IndirWriteByte(&R0_, v2, 0);
      v2 -= 2;
    } while (v2 >= 0);
  } else {
    R18_ = 2;
  }
LABEL_12:
  v3 = v6 + 1;
  while (1) {
    bool v1 = R36 >= R32;
    R36 -= R32;
    if (!v1)
      break;
    v3 -= 2;
    if (v3 < 0) {
      R20_ = 1;
      goto LABEL_23;
    }
  }
  IndirWriteByte(&R0_, v3, HIBYTE(R34));
  v4 = v3 - 2;
  if (v4 >= 0) {
    R20_ = v4 + 4;
    while (1) {
      bool v1 = R36 >= R32;
      R36 -= R32;
      if (!v1)
        break;
      IndirWriteByte(&R0_, v4, HIBYTE(R36));
      v4 -= 2;
      if (v4 < 0)
        goto LABEL_23;
    }
    do {
      IndirWriteByte(&R0_, v4, 0);
      v4 -= 2;
    } while (v4 >= 0);
  } else {
    R20_ = 3;
  }
LABEL_23:;
  uint16 v5 = R20_;
  do {
    IndirWriteByte(&R0_, v5, 0xFFu);
    v5 += 2;
  } while ((int16)(v5 - R18_) < 0);
}

static Func_Y_V *const off_91C2B7[3] = {  // 0x91C27F
  XrayHdmaFunc_BeamAimedDDR,
  XrayHdmaFunc_BeamAimedDD,
  XrayHdmaFunc_BeamAimedDDL,
};

void XrayHdmaFunc_BeamAimedD(void) {
  int16 v1;
  uint16 v0;

  R34 = R22_;
  R36 = R22_;
  if (sign16(R28_ - 128)) {
    v0 = 0;
  } else if (sign16(R26_ - 128)) {
    v0 = 2;
  } else {
    v0 = 4;
  }
  off_91C2B7[v0 >> 1](R24_ * 2);
  v1 = R18_;
  do {
    IndirWriteWord(&R0_, v1, 0xFFu);
    v1 -= 2;
  } while (v1 >= 0);
}

void XrayHdmaFunc_BeamAimedDDR(uint16 j) {  // 0x91C2BD
  uint16 v6 = j;
  while (1) {
    bool v1 = __CFADD__uint16(R32, R36);
    R36 += R32;
    if (v1)
      break;
    j += 2;
    if ((int16)(j - 460) >= 0) {
      R18_ = j;
      goto LABEL_12;
    }
  }
  IndirWriteWord(&R0_, j, *(uint16 *)((char *)&R36 + 1));
  uint16 v2;
  v2 = j + 2;
  if ((int16)(v2 - 460) < 0) {
    R18_ = v2 - 4;
    while (1) {
      bool v1 = __CFADD__uint16(R32, R36);
      R36 += R32;
      if (v1)
        break;
      IndirWriteWord(&R0_, v2, *(uint16 *)((char *)&R36 + 1));
      v2 += 2;
      if ((int16)(v2 - 460) >= 0)
        goto LABEL_12;
    }
    do {
      IndirWriteWord(&R0_, v2, 0xFFu);
      v2 += 2;
    } while ((int16)(v2 - 460) < 0);
  } else {
    R18_ = 458;
  }
LABEL_12:;
  uint16 v3 = v6 + 1;
  while (1) {
    bool v1 = __CFADD__uint16(g_word_7E001E, R34);
    R34 += g_word_7E001E;
    if (v1)
      break;
    v3 += 2;
    if ((int16)(v3 - 460) >= 0) {
      R20_ = v3;
      goto LABEL_23;
    }
  }
  IndirWriteByte(&R0_, v3, HIBYTE(R34));
  uint16 v4;
  v4 = v3 + 2;
  if ((int16)(v4 - 460) < 0) {
    R20_ = v4 - 4;
    while (1) {
      bool v1 = __CFADD__uint16(g_word_7E001E, R34);
      R34 += g_word_7E001E;
      if (v1)
        break;
      IndirWriteByte(&R0_, v4, HIBYTE(R34));
      v4 += 2;
      if ((int16)(v4 - 460) >= 0)
        goto LABEL_23;
    }
    do {
      IndirWriteByte(&R0_, v4, 0xFFu);
      v4 += 2;
    } while ((int16)(v4 - 460) < 0);
  } else {
    R20_ = 459;
  }
LABEL_23:;
  uint16 v5 = R18_;
  do {
    IndirWriteByte(&R0_, v5, 0);
    v5 -= 2;
  } while ((int16)(v5 - R20_) >= 0);
  R18_ = R20_ - 1;
}

void XrayHdmaFunc_BeamAimedDD(uint16 j) {  // 0x91C381
  uint16 v5 = j;
  if (R32) {
    while (1) {
      bool v1 = R36 >= R32;
      R36 -= R32;
      if (!v1)
        break;
      j += 2;
      if ((int16)(j - 460) >= 0) {
        R18_ = j;
        goto LABEL_15;
      }
    }
    IndirWriteWord(&R0_, j, *(uint16 *)((char *)&R36 + 1));
    uint16 v2 = j + 2;
    if ((int16)(v2 - 460) < 0) {
      R18_ = v2 - 4;
      while (1) {
        bool v1 = R36 >= R32;
        R36 -= R32;
        if (!v1)
          break;
        IndirWriteWord(&R0_, v2, *(uint16 *)((char *)&R36 + 1));
        v2 += 2;
        if ((int16)(v2 - 460) >= 0)
          goto LABEL_15;
      }
      do {
        IndirWriteWord(&R0_, v2, 0);
        v2 += 2;
      } while ((int16)(v2 - 460) < 0);
    } else {
      R18_ = 458;
    }
  } else {
    R18_ = j;
    do {
      IndirWriteWord(&R0_, j, 0);
      j += 2;
    } while ((int16)(j - 460) < 0);
  }
LABEL_15:;
  uint16 v3 = v5 + 1;
  while (1) {
    bool v1 = __CFADD__uint16(g_word_7E001E, R34);
    R34 += g_word_7E001E;
    if (v1)
      break;
    v3 += 2;
    if ((int16)(v3 - 460) >= 0) {
      R20_ = v3;
      return;
    }
  }
  IndirWriteByte(&R0_, v3, HIBYTE(R34));
  uint16 v4 = v3 + 2;
  if ((int16)(v4 - 460) < 0) {
    R20_ = v4 - 4;
    while (1) {
      bool v1 = __CFADD__uint16(g_word_7E001E, R34);
      R34 += g_word_7E001E;
      if (v1)
        break;
      IndirWriteByte(&R0_, v4, HIBYTE(R34));
      v4 += 2;
      if ((int16)(v4 - 460) >= 0)
        return;
    }
    do {
      IndirWriteByte(&R0_, v4, 0xFFu);
      v4 += 2;
    } while ((int16)(v4 - 460) < 0);
  } else {
    R20_ = 459;
  }
}

void XrayHdmaFunc_BeamAimedDDL(uint16 j) {  // 0x91C446
  uint16 v6 = j;
  while (1) {
    bool v1 = R36 >= R32;
    R36 -= R32;
    if (!v1)
      break;
    j += 2;
    if ((int16)(j - 460) >= 0) {
      R18_ = j;
      goto LABEL_12;
    }
  }
  IndirWriteWord(&R0_, j, *(uint16 *)((char *)&R36 + 1));
  uint16 v2;
  v2 = j + 2;
  if ((int16)(v2 - 460) < 0) {
    R18_ = v2 - 4;
    while (1) {
      bool v1 = R36 >= R32;
      R36 -= R32;
      if (!v1)
        break;
      IndirWriteWord(&R0_, v2, *(uint16 *)((char *)&R36 + 1));
      v2 += 2;
      if ((int16)(v2 - 460) >= 0)
        goto LABEL_12;
    }
    do {
      IndirWriteWord(&R0_, v2, 0);
      v2 += 2;
    } while ((int16)(v2 - 460) < 0);
  } else {
    R18_ = 458;
  }
LABEL_12:;
  uint16 v3 = v6 + 1;
  while (1) {
    bool v1 = R34 >= g_word_7E001E;
    R34 -= g_word_7E001E;
    if (!v1)
      break;
    v3 += 2;
    if ((int16)(v3 - 460) >= 0) {
      R20_ = v3;
      goto LABEL_23;
    }
  }
  IndirWriteByte(&R0_, v3, HIBYTE(R34));
  uint16 v4;
  v4 = v3 + 2;
  if ((int16)(v4 - 460) < 0) {
    R20_ = v4 - 4;
    while (1) {
      bool v1 = R34 >= g_word_7E001E;
      R34 -= g_word_7E001E;
      if (!v1)
        break;
      IndirWriteByte(&R0_, v4, HIBYTE(R34));
      v4 += 2;
      if ((int16)(v4 - 460) >= 0)
        goto LABEL_23;
    }
    do {
      IndirWriteByte(&R0_, v4, 0);
      v4 += 2;
    } while ((int16)(v4 - 460) < 0);
  } else {
    R20_ = 459;
  }
LABEL_23:;
  uint16 v5 = R20_;
  do {
    IndirWriteByte(&R0_, v5, 0xFFu);
    v5 -= 2;
  } while ((int16)(v5 - R18_) >= 0);
}

void XrayHdmaFunc_BeamHoriz(void) {  // 0x91C505
  int16 v1;

  uint16 v0 = 2 * (R24_ - 1);
  IndirWriteWord(&R0_, v0, 0xFF00u);
  v1 = v0 - 2;
  do {
    if (*(uint16 *)IndirPtr(&R0_, v1) == 255)
      break;
    IndirWriteWord(&R0_, v1, 0xFFu);
    v1 -= 2;
  } while (v1 >= 0);
  uint16 v2 = 2 * R24_;
  do {
    if (*(uint16 *)IndirPtr(&R0_, v2) == 255)
      break;
    IndirWriteWord(&R0_, v2, 0xFFu);
    v2 += 2;
  } while ((int16)(v2 - 460) < 0);
}

void CalculateXrayHdmaTable_OriginOnScreen(uint16 k, uint16 j) {  // 0x91C54B
  int16 v2;
  uint16 v3;

  LOBYTE(v2) = HIBYTE(k);
  HIBYTE(v2) = k;
  R22_ = v2 & 0xFF00;
  R24_ = j;
  R26_ = R18_ - R20_;
  R26_ += (R26_ & 0x8000u) != 0 ? 0x100 : 0;
  R28_ = R20_ + R18_;
  if (!sign16(R20_ + R18_ - 257))
    R28_ = R20_ + R18_ - 256;
  if (!R20_ && (R18_ == 64 || R18_ == 192)) {
    v3 = 8;
  } else {
    if (sign16(R26_ - 128)) {
      if (sign16(R26_ - 64)) {
        if (!sign16(R28_ - 64)) {
          v3 = 0;
          goto LABEL_16;
        }
        goto LABEL_13;
      }
    } else {
      if (!sign16(R26_ - 192)) {
LABEL_13:
        v3 = 2;
        goto LABEL_16;
      }
      if (!sign16(R28_ - 192)) {
        v3 = 6;
        goto LABEL_16;
      }
    }
    v3 = 4;
  }
LABEL_16:;
  uint16 v4 = R26_;
  if (!sign16(R26_ - 128))
    v4 = R26_ - 128;
  g_word_7E001E = kTanTable[v4];
  uint16 v5 = R28_;
  if (!sign16(R28_ - 128))
    v5 = R28_ - 128;
  R32 = kTanTable[v5];
  grapple_beam_unkD1E = 0;
  static Func_V *const kXrayHdmaOnScreen_Funcs[5] = {
    XrayHdmaOnScreen_BeamAimedR,
    XrayHdmaOnScreen_BeamAimedU,
    XrayHdmaOnScreen_BeamAimedD,
    XrayHdmaOnScreen_BeamAimedL,
    XrayHdmaOnScreen_BeamHoriz,
  };

  kXrayHdmaOnScreen_Funcs[v3 >> 1]();
}

void XrayHdmaOnScreen_BeamAimedR(void) {  // 0x91C5FF
  int16 v1;

  uint16 v0 = 2 * (R24_ - 1);
  IndirWriteWord(&R0_, v0, HIBYTE(R22_) | 0xFF00);
  v1 = v0 - 2;
  R34 = R22_;
  R36 = R22_;
  while (1) {
    bool v2 = __CFADD__uint16(g_word_7E001E, R34);
    R34 += g_word_7E001E;
    if (v2)
      break;
    IndirWriteWord(&R0_, v1, HIBYTE(R34) | 0xFF00);
    v1 -= 2;
    if (v1 < 0)
      goto LABEL_6;
  }
  do {
    IndirWriteWord(&R0_, v1, 0xFFu);
    v1 -= 2;
  } while (v1 >= 0);
LABEL_6:;
  uint16 v3 = 2 * R24_;
  while (1) {
    bool v2 = __CFADD__uint16(R32, R36);
    R36 += R32;
    if (v2)
      break;
    IndirWriteWord(&R0_, v3, HIBYTE(R36) | 0xFF00);
    v3 += 2;
    if ((int16)(v3 - 460) >= 0)
      return;
  }
  do {
    IndirWriteWord(&R0_, v3, 0xFFu);
    v3 += 2;
  } while ((int16)(v3 - 460) < 0);
}

void XrayHdmaOnScreen_BeamAimedL(void) {  // 0x91C660
  int16 v1;

  uint16 v0 = 2 * (R24_ - 1);
  IndirWriteWord(&R0_, v0, R22_ & 0xFF00);
  v1 = v0 - 2;
  R34 = R22_;
  R36 = R22_;
  while (1) {
    bool v2 = R36 >= R32;
    R36 -= R32;
    if (!v2)
      break;
    IndirWriteWord(&R0_, v1, R36 & 0xFF00);
    v1 -= 2;
    if (v1 < 0)
      goto LABEL_6;
  }
  do {
    IndirWriteWord(&R0_, v1, 0xFFu);
    v1 -= 2;
  } while (v1 >= 0);
LABEL_6:;
  uint16 v3 = 2 * R24_;
  while (1) {
    bool v2 = R34 >= g_word_7E001E;
    R34 -= g_word_7E001E;
    if (!v2)
      break;
    IndirWriteWord(&R0_, v3, R34 & 0xFF00);
    v3 += 2;
    if ((int16)(v3 - 460) >= 0)
      return;
  }
  do {
    IndirWriteWord(&R0_, v3, 0xFFu);
    v3 += 2;
  } while ((int16)(v3 - 460) < 0);
}

void XrayHdmaOnScreen_BeamAimedU(void) {  // 0x91C6C1
  uint16 v0 = 2 * (R24_ - 1);
  IndirWriteByte(&R0_, v0++, HIBYTE(R22_));
  IndirWriteByte(&R0_, v0, HIBYTE(R22_));
  uint16 v1 = v0 - 3;
  uint16 v2;
  R34 = R22_;
  R36 = R22_;
  if (sign16(R26_ - 192)) {
    v2 = 0;
  } else if (sign16(R28_ - 192)) {
    v2 = 2;
  } else {
    v2 = 4;
  }
  static Func_Y_V *const off_91C716[3] = {
    XrayHdmaOnScreen_BeamAimedUUR,
    XrayHdmaOnScreen_BeamAimedUU,
    XrayHdmaOnScreen_BeamAimedUUL,
  };
  off_91C716[v2 >> 1](v1);
  uint16 v3 = 2 * R24_;
  do {
    if (*(uint16 *)IndirPtr(&R0_, v3) == 255)
      break;
    IndirWriteWord(&R0_, v3, 0xFFu);
    v3 += 2;
  } while ((int16)(v3 - 460) < 0);
}

void XrayHdmaOnScreen_BeamAimedUUR(uint16 v0) {  // 0x91C71C
  int16 v2;
  int16 v4;

  v4 = v0;
  while (1) {
    bool v1 = __CFADD__uint16(g_word_7E001E, R34);
    R34 += g_word_7E001E;
    if (v1)
      break;
    IndirWriteByte(&R0_, v0, HIBYTE(R34));
    v0 -= 2;
    if ((v0 & 0x8000u) != 0)
      goto LABEL_7;
  }
  grapple_beam_unkD1E = v0;
  do {
    IndirWriteByte(&R0_, v0, 0xFFu);
    v0 -= 2;
  } while ((v0 & 0x8000u) == 0);
LABEL_7:
  v2 = v4 + 1;
  while (1) {
    bool v1 = __CFADD__uint16(R32, R36);
    R36 += R32;
    if (v1)
      break;
    IndirWriteByte(&R0_, v2, HIBYTE(R36));
    v2 -= 2;
    if (v2 < 0)
      return;
  }
  do {
    IndirWriteByte(&R0_, v2, 0xFFu);
    v2 -= 2;
  } while (v2 >= 0);
  if (grapple_beam_unkD1E) {
    for (int i = grapple_beam_unkD1E - 2; i >= 0; i -= 2)
      IndirWriteWord(&R0_, i, 0xFFu);
  }
}

void XrayHdmaOnScreen_BeamAimedUU(uint16 v0) {  // 0x91C77F
  int16 v2;
  int16 v3;

  v3 = v0;
  while (1) {
    bool v1 = R34 >= g_word_7E001E;
    R34 -= g_word_7E001E;
    if (!v1)
      break;
    IndirWriteByte(&R0_, v0, HIBYTE(R34));
    v0 -= 2;
    if (v0 < 0)
      goto LABEL_6;
  }
  do {
    IndirWriteByte(&R0_, v0, 0);
    v0 -= 2;
  } while (v0 >= 0);
LABEL_6:
  v2 = v3 + 1;
  while (1) {
    bool v1 = __CFADD__uint16(R32, R36);
    R36 += R32;
    if (v1)
      break;
    IndirWriteByte(&R0_, v2, HIBYTE(R36));
    v2 -= 2;
    if (v2 < 0)
      return;
  }
  do {
    IndirWriteByte(&R0_, v2, 0xFFu);
    v2 -= 2;
  } while (v2 >= 0);
}

void XrayHdmaOnScreen_BeamAimedUUL(uint16 j) {  // 0x91C7CB
  int16 v2;
  int16 v3;
  int16 v5;

  uint16 v4 = j;
  while (1) {
    bool v1 = R34 >= g_word_7E001E;
    R34 -= g_word_7E001E;
    if (!v1)
      break;
    IndirWriteByte(&R0_, j, HIBYTE(R34));
    j -= 2;
    if ((j & 0x8000u) != 0)
      goto LABEL_6;
  }
  do {
    IndirWriteByte(&R0_, j, 0);
    j -= 2;
  } while ((j & 0x8000u) == 0);
LABEL_6:
  v2 = v4 + 1;
  while (1) {
    bool v1 = R36 >= R32;
    R36 -= R32;
    if (!v1)
      break;
    IndirWriteByte(&R0_, v2, HIBYTE(R36));
    v2 -= 2;
    if (v2 < 0)
      return;
  }
  v5 = v2;
  do {
    IndirWriteByte(&R0_, v2, 0);
    v2 -= 2;
  } while (v2 >= 0);
  v3 = v5 - 1;
  do {
    IndirWriteByte(&R0_, v3, 0xFFu);
    v3 -= 2;
  } while (v3 >= 0);
}

void XrayHdmaOnScreen_BeamAimedD(void) {  // 0x91C822
  int16 v3;

  uint16 v2;
  uint16 v0 = 2 * (R24_ - 1);
  IndirWriteByte(&R0_, v0++, HIBYTE(R22_));
  IndirWriteByte(&R0_, v0, HIBYTE(R22_));
  uint16 v1 = v0 + 1;
  R34 = R22_;
  R36 = R22_;
  if (sign16(R28_ - 128)) {
    v2 = 0;
  } else if (sign16(R26_ - 128)) {
    v2 = 2;
  } else {
    v2 = 4;
  }
  static Func_Y_V *const off_91C874[3] = {
    XrayHdmaOnScreen_BeamAimedDDR,
    XrayHdmaOnScreen_BeamAimedDD,
    XrayHdmaOnScreen_BeamAimedDDL,
  };
  off_91C874[v2 >> 1](v1);
  v3 = 2 * (R24_ - 2);
  do {
    if (*(uint16 *)IndirPtr(&R0_, v3) == 255)
      break;
    IndirWriteWord(&R0_, v3, 0xFFu);
    v3 -= 2;
  } while (v3 >= 0);
}

void XrayHdmaOnScreen_BeamAimedDDR(uint16 j) {  // 0x91C87A
  uint16 v4 = j;
  while (1) {
    bool v1 = __CFADD__uint16(R32, R36);
    R36 += R32;
    if (v1)
      break;
    IndirWriteWord(&R0_, j, *(uint16 *)((char *)&R36 + 1));
    j += 2;
    if ((int16)(j - 460) >= 0)
      goto LABEL_7;
  }
  grapple_beam_unkD1E = j;
  do {
    IndirWriteWord(&R0_, j, 0xFFu);
    j += 2;
  } while ((int16)(j - 460) < 0);
LABEL_7:;
  uint16 v2 = v4 + 1;
  while (1) {
    bool v1 = __CFADD__uint16(g_word_7E001E, R34);
    R34 += g_word_7E001E;
    if (v1)
      break;
    IndirWriteByte(&R0_, v2, HIBYTE(R34));
    v2 += 2;
    if ((int16)(v2 - 460) >= 0)
      return;
  }
  do {
    IndirWriteByte(&R0_, v2, 0xFFu);
    v2 += 2;
  } while ((int16)(v2 - 460) < 0);
  if (grapple_beam_unkD1E) {
    uint16 v3 = grapple_beam_unkD1E + 2;
    if ((int16)(grapple_beam_unkD1E - 458) < 0) {
      do {
        IndirWriteWord(&R0_, v3, 0xFFu);
        v3 += 2;
      } while ((int16)(v3 - 460) < 0);
    }
  }
}

void XrayHdmaOnScreen_BeamAimedDD(uint16 j) {  // 0x91C8E8
  uint16 v3 = j;
  while (1) {
    bool v1 = R36 >= R32;
    R36 -= R32;
    if (!v1)
      break;
    IndirWriteWord(&R0_, j, *(uint16 *)((char *)&R36 + 1));
    j += 2;
    if ((int16)(j - 460) >= 0)
      goto LABEL_6;
  }
  do {
    IndirWriteWord(&R0_, j, 0);
    j += 2;
  } while ((int16)(j - 460) < 0);
LABEL_6:;
  uint16 v2 = v3 + 1;
  while (1) {
    bool v1 = __CFADD__uint16(g_word_7E001E, R34);
    R34 += g_word_7E001E;
    if (v1)
      break;
    IndirWriteByte(&R0_, v2, HIBYTE(R34));
    v2 += 2;
    if ((int16)(v2 - 460) >= 0)
      return;
  }
  do {
    IndirWriteByte(&R0_, v2, 0xFFu);
    v2 += 2;
  } while ((int16)(v2 - 460) < 0);
}

void XrayHdmaOnScreen_BeamAimedDDL(uint16 j) {  // 0x91C939
  uint16 v4 = j;
  while (1) {
    bool v1 = R36 >= R32;
    R36 -= R32;
    if (!v1)
      break;
    IndirWriteWord(&R0_, j, *(uint16 *)((char *)&R36 + 1));
    j += 2;
    if ((int16)(j - 460) >= 0)
      goto LABEL_6;
  }
  do {
    IndirWriteWord(&R0_, j, 0);
    j += 2;
  } while ((int16)(j - 460) < 0);
LABEL_6:;
  uint16 v2 = v4 + 1;
  while (1) {
    bool v1 = R34 >= g_word_7E001E;
    R34 -= g_word_7E001E;
    if (!v1)
      break;
    IndirWriteByte(&R0_, v2, HIBYTE(R34));
    v2 += 2;
    if ((int16)(v2 - 460) >= 0)
      return;
  }
  uint16 v5 = v2;
  do {
    IndirWriteByte(&R0_, v2, 0);
    v2 += 2;
  } while ((int16)(v2 - 460) < 0);
  uint16 v3 = v5 - 1;
  do {
    IndirWriteByte(&R0_, v3, 0xFFu);
    v3 += 2;
  } while ((int16)(v3 - 460) < 0);
}

void XrayHdmaOnScreen_BeamHoriz(void) {  // 0x91C998
  int16 v1;

  uint16 v0 = 2 * (R24_ - 1);
  if (R18_ == 64)
    IndirWriteWord(&R0_, v0, HIBYTE(R22_) | 0xFF00);
  else
    IndirWriteWord(&R0_, v0, R22_ & 0xFF00);
  v1 = v0 - 2;
  do {
    IndirWriteWord(&R0_, v1, 0xFFu);
    v1 -= 2;
  } while (v1 >= 0);
  uint16 v2 = 2 * R24_;
  do {
    IndirWriteWord(&R0_, v2, 0xFFu);
    v2 += 2;
  } while ((int16)(v2 - 460) < 0);
}

void XrayRunHandler(void) {  // 0x91CAD6
#define unk_91CAF2 (*(SpawnHdmaObject_Args*)RomPtr(0x91caf2))
  if (!time_is_frozen_flag && (button_config_run_b & joypad1_lastkeys) != 0) {
    if (Xray_Initialize() & 1)
      SpawnHdmaObject(0x91, &unk_91CAF2);
  }
}

void Xray_SetupStage1_FreezeTimeBackup(uint16 k) {  // 0x91CAF9
  LOBYTE(time_is_frozen_flag) = 1;
  *((uint8 *)hdma_object_A + (uint8)k) = reg_BG2HOFS;
  *((uint8 *)hdma_object_A + (uint8)k + 1) = HIBYTE(reg_BG2HOFS);
  *((uint8 *)hdma_object_B + (uint8)k) = reg_BG2VOFS;
  *((uint8 *)hdma_object_B + (uint8)k + 1) = HIBYTE(reg_BG2VOFS);
  *((uint8 *)hdma_object_C + (uint8)k) = reg_BG2SC;
}

void Xray_SetupStage2_ReadBg1_2ndScreen(void) {  // 0x91CB1C
  unsigned int v1;

  uint16 v0 = vram_read_queue_tail;
  v1 = vram_read_queue_tail;
  *(uint16 *)((char *)&vram_read_queue[0].vram_target + vram_read_queue_tail) = ((reg_BG1SC & 0xFC) << 8) + 1024;
  *(uint16 *)((char *)&vram_read_queue[0].dma_parameters + v1) = 129;
  *(uint16 *)((char *)&vram_read_queue[0].dma_parameters + v0 + 1) = 57;
  *(VoidP *)((char *)&vram_read_queue[0].src.addr + v0) = ADDR16_OF_RAM(ram4000) + 0x2800;
  *(uint16 *)(&vram_read_queue[0].src.bank + v0) = 126;
  *(uint16 *)((char *)&vram_read_queue[0].size + v0) = 2048;
  vram_read_queue_tail = v0 + 9;
}

void Xray_SetupStage3_ReadBg1_1stScreen(void) {  // 0x91CB57
  unsigned int v1;

  uint16 v0 = vram_read_queue_tail;
  v1 = vram_read_queue_tail;
  *(uint16 *)((char *)&vram_read_queue[0].vram_target + vram_read_queue_tail) = (reg_BG1SC & 0xFC) << 8;
  *(uint16 *)((char *)&vram_read_queue[0].dma_parameters + v1) = 129;
  *(uint16 *)((char *)&vram_read_queue[0].dma_parameters + v0 + 1) = 57;
  *(VoidP *)((char *)&vram_read_queue[0].src.addr + v0) = ADDR16_OF_RAM(ram4000) + 0x2000;
  *(uint16 *)(&vram_read_queue[0].src.bank + v0) = 126;
  *(uint16 *)((char *)&vram_read_queue[0].size + v0) = 2048;
  vram_read_queue_tail = v0 + 9;
}

void Xray_SetupStage4(void) {  // 0x91CB8E
  unsigned int v3;
  unsigned int v8;

  R24_ = 4 * (((uint8)layer1_y_pos + (uint8)bg1_y_offset) & 0xF0);
  R24_ += (uint8)((layer1_x_pos + bg1_x_offset) & 0xF0) >> 3;
  R24_ += 4 * ((layer1_x_pos + bg1_x_offset) & 0x100);
  R22_ = 0;
  R20_ = 16;
  do {
    R18_ = 16;
    R26_ = R24_ & 0x7E0;
    R28_ = R24_ & 0x1F;
    g_word_7E001E = 0;
    do {
      if (!sign16(g_word_7E001E + R28_ - 32)) {
        R26_ = (R26_ + 1024) & 0x7E0;
        R28_ = 0;
        g_word_7E001E = 0;
      }
      uint16 v0 = g_word_7E001E + R28_ + R26_;
      uint16 v11 = ram4000.xray_tilemaps[v0 + 4096];
      uint16 v10 = ram4000.xray_tilemaps[v0 + 4097];
      uint16 v9 = ram4000.xray_tilemaps[v0 + 4128];
      uint16 v1 = ram4000.xray_tilemaps[v0 + 4129];
      uint16 v2 = R22_;
      v3 = R22_;
      *(uint16 *)((char *)&ram4000.xray_tilemaps[33] + R22_) = v1;
      *(uint16 *)((char *)&ram4000.xray_tilemaps[32] + v3) = v9;
      *(uint16 *)((char *)&ram4000.xray_tilemaps[1] + v2) = v10;
      *(uint16 *)((char *)ram4000.xray_tilemaps + v2) = v11;
      R22_ += 4;
      g_word_7E001E += 2;
      --R18_;
    } while (R18_);
    Xray_SetupStage4_Func1();
    R32 = R24_ & 0x400;
    R24_ = (R24_ & 0x400) + ((R24_ + 64) & 0x3FF);
    R22_ += 64;
    --R20_;
  } while (R20_);
  assert((uint16)(layer1_y_pos >> 12) == 0);
  R34 = Mult8x8(layer1_y_pos >> 4, room_width_in_blocks) + (layer1_x_pos >> 4);
  R22_ = 0;
  R20_ = 16;
  do {
    Xray_SetupStage4_Func2();
    R18_ = 16;
    R36 = R34;
    do {
      Xray_SetupStage4_Func3();
      --R18_;
    } while (R18_);
    uint16 v12 = R22_;
    R22_ += 1984;
    Xray_SetupStage4_Func3();
    R22_ = v12 + 64;
    R34 += room_width_in_blocks;
    --R20_;
  } while (R20_);
  LoadXrayBlocks();
  uint16 v7 = vram_read_queue_tail;
  v8 = vram_read_queue_tail;
  *(uint16 *)((char *)&vram_read_queue[0].vram_target + vram_read_queue_tail) = (reg_BG2SC & 0xFC) << 8;
  *(uint16 *)((char *)&vram_read_queue[0].dma_parameters + v8) = 129;
  *(uint16 *)((char *)&vram_read_queue[0].dma_parameters + v7 + 1) = 57;
  *(VoidP *)((char *)&vram_read_queue[0].src.addr + v7) = 0x5000;
  *(uint16 *)(&vram_read_queue[0].src.bank + v7) = 126;
  *(uint16 *)((char *)&vram_read_queue[0].size + v7) = 2048;
  vram_read_queue_tail = v7 + 9;
}

void Xray_SetupStage4_Func1(void) {  // 0x91CCF1
  int16 v0;

  R32 = R26_;
  v0 = g_word_7E001E + R28_;
  if (!sign16(g_word_7E001E + R28_ - 32)) {
    R32 = (R32 + 1024) & 0x7E0;
    v0 = 0;
  }
  uint16 v1 = R32 + v0;
  uint16 v6 = ram4000.xray_tilemaps[v1 + 4096];
  uint16 v5 = ram4000.xray_tilemaps[v1 + 4097];
  uint16 v4 = ram4000.xray_tilemaps[v1 + 4128];
  uint16 v3 = ram4000.xray_tilemaps[v1 + 4129];
  uint16 v2 = R22_ + 1984;
  *(uint16 *)((char *)&ram4000.xray_tilemaps[33] + (uint16)(R22_ + 1984)) = v3;
  *(uint16 *)((char *)&ram4000.xray_tilemaps[32] + v2) = v4;
  *(uint16 *)((char *)&ram4000.xray_tilemaps[1] + v2) = v5;
  *(uint16 *)((char *)ram4000.xray_tilemaps + v2) = v6;
}

void Xray_SetupStage4_Func2(void) {  // 0x91CD42
  // bug: passing 0xffff to this function is invalid and will read invalid memory.
  if (R34 == 0)
    return;
  PairU16 pay = Xray_HandleXrayedBlock(R34 - 1);
  uint16 *dst = (uint16 *)((char *)ram4000.xray_tilemaps + R22_);

  if (pay.k != FUNC16(Xray_Func9)) {
    if (pay.k != FUNC16(Xray_Func11))
      return;
    TileTable *src = tile_table.tables + *(uint16 *)&RomPtr_91(R3_.addr)[pay.j + 8];
    dst[64] = src->top_left;
    dst[65] = src->top_right;
    dst[96] = src->bottom_left;
    dst[97] = src->bottom_right;
  }
  TileTable *src = tile_table.tables + *(uint16 *)&RomPtr_91(R3_.addr)[pay.j + 4];
  dst[0] = src->top_left;
  dst[1] = src->top_right;
  dst[32] = src->bottom_left;
  dst[33] = src->bottom_right;
}

void CallXrayFunc(uint32 ea, uint16 j) {
  switch (ea) {
  case fnXray_Func6: Xray_Func6(); return;
  case fnXray_Func6B: Xray_Func6B(); return;
  case fnXray_Func7: Xray_Func7(j); return;
  case fnXray_Func9: Xray_Func9(j); return;
  case fnXray_Func10: Xray_Func10(j); return;
  case fnXray_Func11: Xray_Func11(j); return;
  default: Unreachable();
  }
}
void Xray_SetupStage4_Func3(void) {  // 0x91CDBE
  PairU16 pay = Xray_HandleXrayedBlock(R36);
  if (pay.k == 0xFFFF)
    Xray_Func21(0);
  else
    CallXrayFunc(pay.k | 0x910000, pay.j);
}

uint16 Xray_Func21(uint16 j) {  // 0x91CDCB
  R22_ += 4;
  ++R36;
  return j;
}

#define stru_91D2D6 ((XrayBlockData*)RomPtr(0x91d2d6))

PairU16 Xray_HandleXrayedBlock(uint16 k) {  // 0x91CDD6
  uint16 value;
  uint16 j;

  R38 = BTS[k];
  R40 = level_data[k] & 0xF000;
  for (int i = 0; ; ++i) {
    value = stru_91D2D6[i].value;
    if (value == 0xFFFF)
      break;
    if (value == R40) {
      R0_.addr = stru_91D2D6[i].addr;
      for (j = 0; ; j += 4) {
        value = *(uint16 *)&RomPtr_91(R0_.addr)[j];
        if (value == 0xFFFF)
          break;
        if (value == 0xFF00 || value == R38) {
          R3_.addr = *(uint16 *)&RomPtr_91(R0_.addr)[j + 2];
          value = *(uint16 *)RomPtr_91(R3_.addr);
          R0_.addr = value;
          return MakePairU16(value, 0);
        }
      }
      return MakePairU16(value, j);
    }
  }
  return MakePairU16(value, 0);
}

void Xray_SomeDivide(void) {  // 0x91CE2C
  R44 = R36 / room_width_in_blocks;
  R42 = R36 % room_width_in_blocks;
}

uint16 Xray_Func5(void) {  // 0x91CE51
  uint16 v1 = R42;
  uint16 RegWord = room_width_in_blocks * (uint8)R44;
  R46 = BTS[(uint16)(RegWord + v1)];
  R48 = level_data[RegWord + v1] & 0xF000;
  return R48;
}

void Xray_Func6(void) {  // 0x91CE79
  Xray_SomeDivide();
  R48 = R40;
  uint16 v0 = R38;
  if (R38) {
    if ((R38 & 0x80) != 0)
      v0 = R38 | 0xFF00;
    R46 = v0;
    Xray_Func6D();
  } else {
    Xray_Func20();
  }
}

void Xray_Func6D(void) {  // 0x91CE8E
  uint16 v1;
  do {
    do {
      if ((int16)(R46 + R44) < 0) {
        Xray_Func13(0xFFu);
        Xray_Func21(0);
        return;
      }
      R44 += R46;
      v1 = Xray_Func5();
    } while (v1 == 0xD000);
    if (v1 != 20480) {
      Xray_Func20();
      return;
    }
  } while ((R46 & 0x80) == 0);
  R46 = (uint8)R46 ^ 0xFF00;
  Xray_Func6C();
}

void Xray_Func6B(void) {  // 0x91CEBB
  Xray_SomeDivide();
  R48 = R40;
  uint16 v0 = R38;
  if (R38) {
    if ((R38 & 0x80) != 0)
      v0 = R38 | 0xFF00;
    R46 = v0;
    Xray_Func6C();
  } else {
    Xray_Func20();
  }
}

void Xray_Func6C(void) {  // 0x91CED0
  while (1) {
    if ((int16)(R46 + R42) < 0) {
      Xray_Func13(0xFFu);
      Xray_Func21(0); // ??
      return;
    }
    R42 += R46;
    uint16 v1 = Xray_Func5();
    if (v1 == 0xD000) {
      Xray_Func6D();
      return;
    }
    if (v1 != 20480)
      break;
    if ((R46 & 0x80) != 0)
      R46 = (uint8)R46 ^ 0xFF00;
  }
  Xray_Func20();
}

#define stru_91D2D6 ((XrayBlockData*)RomPtr(0x91d2d6))

void Xray_Func20(void) {  // 0x91CEFD
  int i;
  int16 v1;

  if (stru_91D2D6[1].value == R48) {
    R0_.addr = stru_91D2D6[1].addr;
    for (i = 0; ; i += 4) {
      v1 = *(uint16 *)&RomPtr_91(R0_.addr)[i];
      if (v1 == -1)
        break;
      if (v1 == -256 || v1 == R46) {
        R3_.addr = *(uint16 *)&RomPtr_91(R0_.addr)[(uint16)(i + 2)] + 2;
        Xray_Func12(0);
        Xray_Func21(0);
        return;
      }
    }
  }
  Xray_Func21(i);
}

uint16 Xray_Func7(uint16 j) {  // 0x91CF36
  uint16 v1 = j + 2;
  Xray_Func12(v1);
  return Xray_Func21(v1);
}

uint16 Xray_Func8(uint16 j) {  // 0x91CF3E
  uint16 v1 = j + 2;
  if (area_index == 1)
    Xray_Func12(v1);
  return Xray_Func21(v1);
}

uint16 Xray_Func9(uint16 j) {  // 0x91CF4E
  uint16 v1 = j + 2;
  Xray_Func12(v1);
  if (R18_ != 1) {
    v1 += 2;
    Xray_Func14(v1);
  }
  return Xray_Func21(v1);
}

uint16 Xray_Func10(uint16 j) {  // 0x91CF62
  uint16 v1 = j + 2;
  Xray_Func12(v1);
  v1 += 2;
  Xray_Func15(v1);
  return Xray_Func21(v1);
}

uint16 Xray_Func11(uint16 j) {  // 0x91CF6F
  unsigned int v8;

  uint16 v1 = j + 2;
  Xray_Func12(v1);
  uint16 v2 = v1 + 2;
  if (R18_ != 1)
    Xray_Func14(v2);
  uint16 v3 = v2 + 2;
  Xray_Func15(v3);
  uint16 v4 = v3 + 2;
  if (R18_ != 1) {
    uint16 v5 = *(uint16 *)&RomPtr_91(R3_.addr)[v4];
    uint16 top_left = tile_table.tables[v5].top_left;
    uint16 top_right = tile_table.tables[v5].top_right;
    uint16 bottom_left = tile_table.tables[v5].bottom_left;
    uint16 bottom_right = tile_table.tables[v5].bottom_right;
    uint16 v7 = R22_;
    v8 = R22_;
    *(uint16 *)((char *)&ram4000.xray_tilemaps[99] + R22_) = bottom_right;
    *(uint16 *)((char *)&ram4000.xray_tilemaps[98] + v8) = bottom_left;
    *(uint16 *)((char *)&ram4000.xray_tilemaps[67] + v7) = top_right;
    *(uint16 *)((char *)&ram4000.xray_tilemaps[66] + v7) = top_left;
  }
  return Xray_Func21(v4);
}

void Xray_Func12(uint16 j) {  // 0x91CFBF
  uint8 *v1 = RomPtr_91(R3_.addr);
  Xray_Func13(*(uint16 *)&v1[j]);
}

void Xray_Func13(uint16 a) {  // 0x91CFC1
  unsigned int v3;

  uint16 v1 = a;
  uint16 top_left = tile_table.tables[v1].top_left;
  uint16 top_right = tile_table.tables[v1].top_right;
  uint16 bottom_left = tile_table.tables[v1].bottom_left;
  uint16 v2 = R22_;
  v3 = R22_;
  *(uint16 *)((char *)&ram4000.xray_tilemaps[33] + R22_) = tile_table.tables[v1].bottom_right;
  *(uint16 *)((char *)&ram4000.xray_tilemaps[32] + v3) = bottom_left;
  *(uint16 *)((char *)&ram4000.xray_tilemaps[1] + v2) = top_right;
  *(uint16 *)((char *)ram4000.xray_tilemaps + v2) = top_left;
}

void Xray_Func14(uint16 j) {  // 0x91CFEE
  unsigned int v4;

  uint16 v1 = *(uint16 *)&RomPtr_91(R3_.addr)[j];
  uint16 top_left = tile_table.tables[v1].top_left;
  uint16 top_right = tile_table.tables[v1].top_right;
  uint16 bottom_left = tile_table.tables[v1].bottom_left;
  uint16 bottom_right = tile_table.tables[v1].bottom_right;
  uint16 v3 = R22_;
  v4 = R22_;
  *(uint16 *)((char *)&ram4000.xray_tilemaps[35] + R22_) = bottom_right;
  *(uint16 *)((char *)&ram4000.xray_tilemaps[34] + v4) = bottom_left;
  *(uint16 *)((char *)&ram4000.xray_tilemaps[3] + v3) = top_right;
  *(uint16 *)((char *)&ram4000.xray_tilemaps[2] + v3) = top_left;
}

void Xray_Func15(uint16 j) {  // 0x91D01D
  unsigned int v4;

  uint16 v1 = *(uint16 *)&RomPtr_91(R3_.addr)[j];
  uint16 top_left = tile_table.tables[v1].top_left;
  uint16 top_right = tile_table.tables[v1].top_right;
  uint16 bottom_left = tile_table.tables[v1].bottom_left;
  uint16 bottom_right = tile_table.tables[v1].bottom_right;
  uint16 v3 = R22_;
  v4 = R22_;
  *(uint16 *)((char *)&ram4000.xray_tilemaps[97] + R22_) = bottom_right;
  *(uint16 *)((char *)&ram4000.xray_tilemaps[96] + v4) = bottom_left;
  *(uint16 *)((char *)&ram4000.xray_tilemaps[65] + v3) = top_right;
  *(uint16 *)((char *)&ram4000.xray_tilemaps[64] + v3) = top_left;
}

void LoadBlockToXrayTilemap(uint16 a, uint16 k, uint16 j) {  // 0x91D04C
  R40 = a;
  R24_ = layer1_x_pos >> 4;
  if ((int16)(k - (layer1_x_pos >> 4)) >= 0) {
    R24_ = k - (layer1_x_pos >> 4);
    if ((((__PAIR32__(k - (layer1_x_pos >> 4), k) - ((layer1_x_pos >> 4) | 0x100000)) >> 16) & 0x8000u) != 0) {
      R26_ = layer1_y_pos >> 4;
      if ((int16)(j - (layer1_y_pos >> 4)) >= 0) {
        R26_ = j - (layer1_y_pos >> 4);
        if ((((__PAIR32__(j - (layer1_y_pos >> 4), j) - ((layer1_y_pos >> 4) | 0x100000)) >> 16) & 0x8000u) != 0) {
          R22_ = 4 * (R24_ + 32 * R26_);
          if ((R40 & 0x800) != 0)
            Xray_Func16(R40 & 0x3FF);
          else
            Xray_Func13(R40 & 0x3FF);
        }
      }
    }
  }
}

void Xray_Func16(uint16 a) {  // 0x91D0A6
  unsigned int v3;

  uint16 v1 = a;
  uint16 top_left = tile_table.tables[v1].top_left;
  uint16 top_right = tile_table.tables[v1].top_right;
  uint16 bottom_left = tile_table.tables[v1].bottom_left;
  uint16 v2 = R22_;
  v3 = R22_;
  *(uint16 *)((char *)&ram4000.xray_tilemaps[1] + R22_) = tile_table.tables[v1].bottom_right;
  *(uint16 *)((char *)ram4000.xray_tilemaps + v3) = bottom_left;
  *(uint16 *)((char *)&ram4000.xray_tilemaps[33] + v2) = top_right;
  *(uint16 *)((char *)&ram4000.xray_tilemaps[32] + v2) = top_left;
}

void Xray_SetupStage5(void) {  // 0x91D0D3
  unsigned int v1;

  if (CanXrayShowBlocks()) {
    if (earthquake_timer) {
      reg_BG1HOFS = layer1_x_pos + bg1_x_offset;
      reg_BG1VOFS = layer1_y_pos + bg1_y_offset;
    }
    reg_BG2HOFS = reg_BG1HOFS & 0xF;
    reg_BG2VOFS = reg_BG1VOFS & 0xF;
    reg_BG2SC = 73;
  }
  uint16 v0 = vram_read_queue_tail;
  v1 = vram_read_queue_tail;
  *(uint16 *)((char *)&vram_read_queue[0].vram_target + vram_read_queue_tail) = ((reg_BG2SC & 0xFC) << 8) + 1024;
  *(uint16 *)((char *)&vram_read_queue[0].dma_parameters + v1 + 1) = 57;
  *(VoidP *)((char *)&vram_read_queue[0].src.addr + v0) = 0x5800;
  *(uint16 *)(&vram_read_queue[0].src.bank + v0) = 126;
  *(uint16 *)((char *)&vram_read_queue[0].size + v0) = 2048;
  vram_read_queue_tail = v0 + 9;
}

bool CanXrayShowBlocks(void) {  // 0x91D143
  if (room_ptr == addr_kRoom_a66a || room_ptr == addr_kRoom_cefb)
    return false;
  if (fx_type == 0x24)
    return false;
  if (boss_id == 3 || boss_id == 6 || boss_id == 7 || boss_id == 8 || boss_id == 10)
    return false;
  return true;
}

void Xray_SetupStage6(void) {  // 0x91D173
  VramWriteEntry *v1;

  if (CanXrayShowBlocks()) {
    uint16 v0 = vram_write_queue_tail;
    v1 = gVramWriteEntry(vram_write_queue_tail);
    v1->size = 2048;
    v1->src.addr = ADDR16_OF_RAM(ram4000);
    *(uint16 *)&v1->src.bank = 126;
    v1->vram_dst = (reg_BG2SC & 0xFC) << 8;
    vram_write_queue_tail = v0 + 7;
  }
}

void Xray_SetupStage7(void) {  // 0x91D1A0
  VramWriteEntry *v1;

  if (CanXrayShowBlocks()) {
    uint16 v0 = vram_write_queue_tail;
    v1 = gVramWriteEntry(vram_write_queue_tail);
    v1->size = 2048;
    v1->src.addr = 0x4800;
    *(uint16 *)&v1->src.bank = 126;
    v1->vram_dst = ((reg_BG2SC & 0xFC) << 8) + 1024;
    vram_write_queue_tail = v0 + 7;
  }
  mov24((LongPtr *)&demo_enable, 0x9800E4);
  mov24((LongPtr *)((char *)&demo_num_input_frames + 1), 0x98C8E4u);
  mov24((LongPtr *)&demo_input_prev_new, 0x999098u);
  *(uint16 *)((char *)&demo_backup_prev_controller_input + 1) = 0;
  demo_input_pre_instr = 0;
  demo_input_instr_timer = 0;
  demo_input_instr_ptr = 0;
  demo_input = 0;
  demo_input_new = 0;
  if (samus_pose_x_dir == 4)
    xray_angle = 192;
  else
    xray_angle = 64;
}

void HdmaobjPreInstr_XraySetup(uint16 k) {  // 0x91D27F
  int16 v1;

  v1 = 4096;
  if (fx_type == 36) {
    if (!sign16((reg_COLDATA[0] & 0x1F) - 7))
      goto LABEL_5;
    goto LABEL_4;
  }
  v1 = 0x2000;
  if (CanXrayShowBlocks()) {
    v1 = 0x4000;
LABEL_4:
    *(uint16 *)&reg_COLDATA[0] = 0x27;
    *(uint16 *)&reg_COLDATA[1] = 0x47;
    *(uint16 *)&reg_COLDATA[2] = 0x87;
  }
LABEL_5:
  fx_layer_blending_config_c |= v1;
}

void Xray_SetupStage8_SetBackdropColor(void) {  // 0x91D2BC
  palette_buffer[0] = 3171;
}

void sub_91D2D1(void) {  // 0x91D2D1
  ;
}

void GameState_28_Unused_(void) {  // 0x91D4DA
  Unreachable();
}

void VariaSuitPickup(void) {  // 0x91D4E4
  suit_pickup_color_math_R = 48;
  suit_pickup_color_math_G = 80;
  suit_pickup_color_math_B = 0x80;
  suit_pickup_palette_transition_color = 0;
  Samus_CancelSpeedBoost();
  samus_x_extra_run_speed = 0;
  samus_x_extra_run_subspeed = 0;
  samus_x_base_speed = 0;
  samus_x_base_subspeed = 0;
  samus_y_subspeed = 0;
  samus_y_speed = 0;
  samus_y_dir = 0;
  used_for_ball_bounce_on_landing = 0;
  samus_x_accel_mode = 0;
  elevator_status = 0;
  substate = 0;
  suit_pickup_light_beam_pos = 0;
  suit_pickup_light_beam_widening_speed = 256;
  for (int i = 510; i >= 0; i -= 2)
    hdma_table_1[i >> 1] = 255;
  if (samus_movement_type == 3 || samus_movement_type == 20)
    QueueSfx1_Max9(0x32u);
  if ((equipped_items & 0x20) != 0)
    samus_pose = kPose_9B_FaceF_VariaGravitySuit;
  else
    samus_pose = kPose_00_FaceF_Powersuit;
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
  CallSomeSamusCode(0x15u);
  samus_x_pos = layer1_x_pos + 120;
  samus_prev_x_pos = layer1_x_pos + 120;
  samus_y_pos = layer1_y_pos + 136;
  samus_prev_y_pos = layer1_y_pos + 136;
  QueueSfx2_Max6(0x56u);
  static const SpawnHdmaObject_Args unk_91D59B = { 0x41, 0x26, 0xd5a2 };
  SpawnHdmaObject(0x91, &unk_91D59B);
}

void GravitySuitPickup(void) {  // 0x91D5BA
  suit_pickup_color_math_R = 48;
  suit_pickup_color_math_G = 73;
  suit_pickup_color_math_B = -112;
  suit_pickup_palette_transition_color = 1;
  Samus_CancelSpeedBoost();
  samus_x_extra_run_speed = 0;
  samus_x_extra_run_subspeed = 0;
  samus_x_base_speed = 0;
  samus_x_base_subspeed = 0;
  samus_y_subspeed = 0;
  samus_y_speed = 0;
  samus_y_dir = 0;
  used_for_ball_bounce_on_landing = 0;
  samus_x_accel_mode = 0;
  elevator_status = 0;
  substate = 0;
  suit_pickup_light_beam_pos = 0;
  suit_pickup_light_beam_widening_speed = 256;
  for (int i = 510; i >= 0; i -= 2)
    hdma_table_1[i >> 1] = 255;
  if (samus_movement_type == kMovementType_03_SpinJumping || samus_movement_type == kMovementType_14_WallJumping)
    QueueSfx1_Max9(0x32u);
  if ((equipped_items & 1) != 0)
    samus_pose = kPose_9B_FaceF_VariaGravitySuit;
  else
    samus_pose = kPose_00_FaceF_Powersuit;
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
  CallSomeSamusCode(0x15u);
  samus_x_pos = layer1_x_pos + 120;
  samus_prev_x_pos = layer1_x_pos + 120;
  samus_y_pos = layer1_y_pos + 136;
  samus_prev_y_pos = layer1_y_pos + 136;
  QueueSfx2_Max6(0x56u);
  static const SpawnHdmaObject_Args unk_91D673 = { 0x41, 0x26, 0xd67a };
  SpawnHdmaObject(0x91, &unk_91D673);
}

void InitializeSuitPickupHdma(void) {  // 0x91D692
  reg_TM = 19;
  reg_TMW = 19;
  reg_TS = 4;
  reg_TSW = 4;
  next_gameplay_CGWSEL = 16;
  reg_W12SEL = 0;
  reg_W34SEL = 2;
  reg_WOBJSEL = 32;
  next_gameplay_CGADSUB = 51;
  reg_COLDATA[0] = suit_pickup_color_math_R;
  reg_COLDATA[1] = suit_pickup_color_math_G;
  reg_COLDATA[2] = suit_pickup_color_math_B;
  mov24((LongPtr *)&demo_enable, 0x9800E4u);
  mov24((LongPtr *)((char *)&demo_num_input_frames + 1), 0x98C8E4u);
  mov24((LongPtr *)&demo_input_prev_new, 0x999098u);
  *(uint16 *)((char *)&demo_backup_prev_controller_input + 1) = 0;
}

#define off_91D727 ((uint16*)RomPtr(0x91d727))

static Func_U8 *const off_91D72D[11] = {  // 0x91D6F7
  Samus_HandleScrewAttackSpeedBoostingPals,
  Samus_SpeedBoosterShinePals,
  (Func_U8 *)HandleMiscSamusPalette,
  0,
  0,
  0,
  Samus_HandleShinesparkingPals,
  Samus_HandleCrystalFlashPals,
  Samus_HandleXrayPals,
  HandleVisorPalette,
  nullsub_164,
};

void Samus_HandlePalette(void) {
  if ((samus_special_super_palette_flags & 0x8000u) == 0
      && (HandleBeamChargePalettes() & 1
          || !(off_91D72D[timer_for_shine_timer]() & 1))) {
    CopyToSamusSuitPalette(off_91D727[samus_suit_palette_index >> 1]);
  }
  HandleMiscSamusPalette();
}
#define kSamusPalette_HyperBeam ((uint16*)RomPtr(0x91d829))
#define kSamusPalette_NonPseudoScrew ((uint16*)RomPtr(0x91d7d5))
#define kSamusPalette_PseudoScrew ((uint16*)RomPtr(0x91d7ff))
uint8 HandleBeamChargePalettes(void) {  // 0x91D743
  if (charged_shot_glow_timer) {
    if (hyper_beam_flag) {
      if ((charged_shot_glow_timer & 1) == 0) {
        if ((charged_shot_glow_timer & 0x1E) == 0) {
          charged_shot_glow_timer = 0;
          return 1;
        }
        CopyToSamusSuitPalette(kSamusPalette_HyperBeam[(uint8)(charged_shot_glow_timer & 0x1E) >> 1]);
      }
      --charged_shot_glow_timer;
      return 0;
    }
    if (--charged_shot_glow_timer) {
      for (int i = 28; i >= 0; i -= 2)
        palette_buffer[(i >> 1) + 193] = 1023;
      return 0;
    } else {
      return 1;
    }
  } else if (grapple_beam_function == (uint16)FUNC16(GrappleBeamFunc_Inactive)
             && flare_counter
             && !sign16(flare_counter - 60)) {
    if (samus_contact_damage_index == 4)
      R36 = kSamusPalette_PseudoScrew[samus_suit_palette_index >> 1];
    else
      R36 = kSamusPalette_NonPseudoScrew[samus_suit_palette_index >> 1];
    uint16 v0 = *(uint16 *)RomPtr_91(R36 + samus_charge_palette_index);
    CopyToSamusSuitPalette(v0);
    uint16 v1 = samus_charge_palette_index + 2;
    if (!sign16(samus_charge_palette_index - 10))
      v1 = 0;
    samus_charge_palette_index = v1;
    return 0;
  } else {
    samus_charge_palette_index = 0;
    return HandleVisorPalette();
  }
}

#define word_9BA3C0 ((uint16*)RomPtr(0x9ba3c0))
uint8 HandleVisorPalette(void) {  // 0x91D83F
  int16 v2;
  uint16 v3;
  char v4; // t0

  if (timer_for_shine_timer == 8)
    return 0;
  if (fx_layer_blending_config_a == 40 || fx_layer_blending_config_a == 42) {
    uint16 v1 = samus_visor_palette_timer_index - 1;
    samus_visor_palette_timer_index = v1;
    if ((uint8)v1)
      return 0;
    samus_visor_palette_timer_index = v1 | 5;
    palette_buffer[196] = word_9BA3C0[HIBYTE(v1) >> 1];
    v2 = HIBYTE(v1) + 2;
    if (sign16(v2 - 12)) {
      v4 = v2;
      LOBYTE(v3) = HIBYTE(v2);
      HIBYTE(v3) = v4;
      R18_ = v3;
      samus_visor_palette_timer_index = v3 | (uint8)samus_visor_palette_timer_index;
    } else {
      samus_visor_palette_timer_index = (uint8)samus_visor_palette_timer_index | 0x600;
    }
    return 0;
  } else {
    samus_visor_palette_timer_index = 1537;
    return 0;
  }
}

#define kSamus_SpeedBoostingPalettes ((uint16*)RomPtr(0x91d998))
#define kSamus_HyperBeamPalettes ((uint16*)RomPtr(0x91d99e))

void HandleMiscSamusPalette(void) {  // 0x91D8A5
  if (!samus_special_super_palette_flags) {
    uint16 v0 = samus_hurt_flash_counter;
    if (!samus_hurt_flash_counter)
      return;
    if (samus_hurt_flash_counter == 2) {
      if (!cinematic_function
          && (frame_handler_beta != (uint16)FUNC16(j_HandleDemoRecorder_2_0)
              || samus_pose != kPose_54_FaceL_Knockback)) {
        QueueSfx1_Max6(0x35u);
        goto LABEL_14;
      }
      v0 = samus_hurt_flash_counter;
    }
    if (!sign16(v0 - 7))
      goto LABEL_17;
    if ((v0 & 1) != 0) {
      CopyToSamusSuitPalette(addr_word_9BA380);
      goto LABEL_17;
    }
LABEL_14:
    if (cinematic_function)
      CopyToSamusSuitPalette(addr_word_9BA3A0);
    else
      Samus_LoadSuitPalette();
LABEL_17:;
    uint16 v1 = samus_hurt_flash_counter + 1;
    samus_hurt_flash_counter = v1;
    if (v1 == 40) {
      if (grapple_beam_function == (uint16)FUNC16(GrappleBeamFunc_Inactive)) {
        if (samus_movement_type == kMovementType_03_SpinJumping || samus_movement_type == kMovementType_14_WallJumping) {
          CallSomeSamusCode(0x1Cu);
        } else if (!sign16(flare_counter - 16) && (button_config_shoot_x & joypad1_lastkeys) != 0) {
          play_resume_charging_beam_sfx = 1;
        }
      } else if (sign16(grapple_beam_function + 0x37AA)) {
        QueueSfx1_Max9(6u);
      }
    } else if (!sign16(v1 - 60)) {
      samus_hurt_flash_counter = 0;
    }
    return;
  }
  if ((samus_special_super_palette_flags & 0x8000u) != 0) {
    CopyToSamusSuitPalette(kSamus_HyperBeamPalettes[samus_charge_palette_index]);
    bool v2 = (--special_samus_palette_timer & 0x8000u) != 0;
    if (!special_samus_palette_timer || v2) {
      special_samus_palette_timer = special_samus_palette_frame;
      if (!sign16(++samus_charge_palette_index - 10))
        samus_charge_palette_index = 0;
    }
  } else {
    if ((samus_special_super_palette_flags & 1) != 0)
      CopyToSamusSuitPalette(kSamus_SpeedBoostingPalettes[samus_suit_palette_index >> 1]);
    else
      Samus_LoadSuitPalette();
    ++samus_special_super_palette_flags;
  }
}

#define kSamusPal_ScrewAttack ((uint16*)RomPtr(0x91da4a))
#define kSamusPal_SpeedBoost ((uint16*)RomPtr(0x91daa9))

uint8 Samus_HandleScrewAttackSpeedBoostingPals(void) {  // 0x91D9B2
  if ((samus_suit_palette_index & 4) == 0) {
    Samus_GetTopBottomBoundary();
    if ((fx_y_pos & 0x8000u) != 0) {
      if ((lava_acid_y_pos & 0x8000u) == 0 && sign16(lava_acid_y_pos - R20_))
        return 1;
    } else if (sign16(fx_y_pos - R20_) && (fx_liquid_options & 4) == 0) {
      return 1;
    }
  }
  if (samus_movement_type == kMovementType_03_SpinJumping) {
    if ((equipped_items & 8) == 0)
      goto LABEL_10;
    if (samus_anim_frame) {
      if (!sign16(samus_anim_frame - 27))
        return 0;
      goto LABEL_18;
    }
    goto LABEL_21;
  }
  if (samus_movement_type == kMovementType_14_WallJumping) {
    if ((equipped_items & 8) == 0)
      return 1;
    if (!sign16(samus_anim_frame - 3)) {
LABEL_18:
      R36 = kSamusPal_ScrewAttack[samus_suit_palette_index >> 1];
      uint16 v1 = *(uint16 *)RomPtr_91(R36 + special_samus_palette_frame);
      CopyToSamusSuitPalette(v1);
      uint16 v2 = special_samus_palette_frame + 2;
      if (!sign16(special_samus_palette_frame - 10))
        v2 = 0;
      special_samus_palette_frame = v2;
      return 1;
    }
LABEL_21:
    special_samus_palette_frame = 0;
    return 1;
  }
LABEL_10:
  if ((speed_boost_counter & 0xFF00) != 1024)
    return 1;
  bool v3 = (--special_samus_palette_timer & 0x8000u) != 0;
  if (!special_samus_palette_timer || v3) {
    special_samus_palette_timer = 4;
    R36 = kSamusPal_SpeedBoost[samus_suit_palette_index >> 1];
    uint16 v4 = *(uint16 *)RomPtr_91(R36 + special_samus_palette_frame);
    CopyToSamusSuitPalette(v4);
    uint16 v5 = special_samus_palette_frame + 2;
    if (!sign16(special_samus_palette_frame - 6))
      v5 = 6;
    special_samus_palette_frame = v5;
  }
  return 1;
}

#define kSamusPal_SpeedBoostShine ((uint16*)RomPtr(0x91db10))

uint8 Samus_SpeedBoosterShinePals(void) {  // 0x91DAC7
  uint16 v0 = samus_shine_timer;
  if (samus_shine_timer == 170) {
    uint16 v4 = samus_shine_timer;
    QueueSfx3_Max9(0xCu);
    v0 = v4;
  }
  samus_shine_timer = v0 - 1;
  if ((int16)(v0 - 1) <= 0) {
    special_samus_palette_frame = 0;
    timer_for_shine_timer = 0;
    return 0;
  } else {
    R36 = kSamusPal_SpeedBoostShine[samus_suit_palette_index >> 1];
    uint16 v1 = *(uint16 *)RomPtr_91(R36 + special_samus_palette_frame);
    CopyToSamusSuitPalette(v1);
    uint16 v2 = special_samus_palette_frame + 2;
    if (!sign16(special_samus_palette_frame - 10))
      v2 = 0;
    special_samus_palette_frame = v2;
    return 1;
  }
}

#define kSamusPal_Shinespark ((uint16*)RomPtr(0x91db75))
uint8 Samus_HandleShinesparkingPals(void) {  // 0x91DB3A
  bool v0 = (--samus_shine_timer & 0x8000u) != 0;
  if (!samus_shine_timer || v0) {
    timer_for_shine_timer = 0;
    special_samus_palette_frame = 0;
    return 0;
  } else {
    R36 = kSamusPal_Shinespark[samus_suit_palette_index >> 1];
    uint16 v1 = *(uint16 *)RomPtr_91(R36 + special_samus_palette_frame);
    CopyToSamusSuitPalette(v1);
    uint16 v2 = special_samus_palette_frame + 2;
    if (!sign16(special_samus_palette_frame - 6))
      v2 = 0;
    special_samus_palette_frame = v2;
    return 1;
  }
}

#define stru_91DC00 ((SamusCrystalFlashPalTable*)RomPtr(0x91dc00))
#define off_91DC28 ((uint16*)RomPtr(0x91dc28))
uint8 Samus_HandleCrystalFlashPals(void) {  // 0x91DB93
  if ((samus_shine_timer & 0x8000u) != 0) {
    WriteBeamPalette_A(equipped_beams);
    timer_for_shine_timer = 0;
    special_samus_palette_frame = 0;
    special_samus_palette_timer = 0;
    samus_shine_timer = 0;
    return 0;
  } else {
    if ((int16)--samus_shine_timer <= 0) {
      samus_shine_timer = 5;
      Samus_Copy6PalColors(off_91DC28[special_samus_palette_frame >> 1]);
      uint16 v0 = special_samus_palette_frame + 2;
      if (!sign16(special_samus_palette_frame - 10))
        v0 = 0;
      special_samus_palette_frame = v0;
    }
    bool v1 = (int16)-- * (uint16 *)&suit_pickup_color_math_B < 0;
    if (!*(uint16 *)&suit_pickup_color_math_B || v1) {
      *(uint16 *)&suit_pickup_color_math_B = *(uint16 *)((char *)&stru_91DC00[0].timer + special_samus_palette_timer);
      Samus_Copy10PalColors(*(VoidP *)((char *)&stru_91DC00[0].ptr + special_samus_palette_timer));
      uint16 v2 = special_samus_palette_timer + 4;
      if (!sign16(special_samus_palette_timer - 36))
        v2 = 0;
      special_samus_palette_timer = v2;
    }
    return 1;
  }
}

void Samus_Copy10PalColors(uint16 v0) {  // 0x91DC34
  uint16 *v1 = (uint16 *)RomPtr_9B(v0);
  palette_buffer[224] = *v1;
  palette_buffer[225] = v1[1];
  palette_buffer[226] = v1[2];
  palette_buffer[227] = v1[3];
  palette_buffer[228] = v1[4];
  palette_buffer[229] = v1[5];
  palette_buffer[230] = v1[6];
  palette_buffer[231] = v1[7];
  palette_buffer[232] = v1[8];
  palette_buffer[233] = v1[9];
}

void Samus_Copy6PalColors(uint16 j) {  // 0x91DC82
  uint16 *v1 = (uint16 *)RomPtr_9B(j);
  palette_buffer[234] = *v1;
  palette_buffer[235] = v1[1];
  palette_buffer[236] = v1[2];
  palette_buffer[237] = v1[3];
  palette_buffer[238] = v1[4];
  palette_buffer[239] = v1[5];
}

uint8 Samus_HandleXrayPals(void) {  // 0x91DCB4
  if ((demo_timer_counter & 0x8000u) != 0) {
    timer_for_shine_timer = 0;
    special_samus_palette_frame = 0;
    special_samus_palette_timer = 0;
    demo_timer_counter = 0;
    return 0;
  } else {
    if (!demo_timer_counter) {
      if (sign16(demo_input_pre_instr - 2)) {
        bool v0 = (--special_samus_palette_timer & 0x8000u) != 0;
        if (!special_samus_palette_timer || v0) {
          special_samus_palette_timer = 5;
          palette_buffer[196] = word_9BA3C0[special_samus_palette_frame >> 1];
          if (sign16(special_samus_palette_frame - 4))
            special_samus_palette_frame += 2;
        }
        return 1;
      }
      special_samus_palette_frame = 6;
      special_samus_palette_timer = 1;
      demo_timer_counter = 1;
    }
    bool v2 = (--special_samus_palette_timer & 0x8000u) != 0;
    if (special_samus_palette_timer && !v2)
      return 1;
    special_samus_palette_timer = 5;
    palette_buffer[196] = word_9BA3C0[special_samus_palette_frame >> 1];
    uint16 v3 = special_samus_palette_frame + 2;
    if (!sign16(special_samus_palette_frame - 10))
      v3 = 6;
    special_samus_palette_frame = v3;
    return 1;
  }
}

uint8 nullsub_164(void) {  // 0x91DD31
  return 0;
}

void CopyToSamusSuitPalette(uint16 k) {  // 0x91DD5B
  uint16 *v1 = (uint16 *)RomPtr_9B(k);
  palette_buffer[192] = *v1;
  palette_buffer[193] = v1[1];
  palette_buffer[194] = v1[2];
  palette_buffer[195] = v1[3];
  palette_buffer[196] = v1[4];
  palette_buffer[197] = v1[5];
  palette_buffer[198] = v1[6];
  palette_buffer[199] = v1[7];
  palette_buffer[200] = v1[8];
  palette_buffer[201] = v1[9];
  palette_buffer[202] = v1[10];
  palette_buffer[203] = v1[11];
  palette_buffer[204] = v1[12];
  palette_buffer[205] = v1[13];
  palette_buffer[206] = v1[14];
  palette_buffer[207] = v1[15];
}

void CopyToSamusSuitTargetPalette(uint16 k) {  // 0x91DDD7
  uint16 *v1 = (uint16 *)RomPtr_9B(k);
  target_palettes[192] = *v1;
  target_palettes[193] = v1[1];
  target_palettes[194] = v1[2];
  target_palettes[195] = v1[3];
  target_palettes[196] = v1[4];
  target_palettes[197] = v1[5];
  target_palettes[198] = v1[6];
  target_palettes[199] = v1[7];
  target_palettes[200] = v1[8];
  target_palettes[201] = v1[9];
  target_palettes[202] = v1[10];
  target_palettes[203] = v1[11];
  target_palettes[204] = v1[12];
  target_palettes[205] = v1[13];
  target_palettes[206] = v1[14];
  target_palettes[207] = v1[15];
}

void Samus_CancelSpeedBoost(void) {  // 0x91DE53
  if (samus_has_momentum_flag) {
    samus_has_momentum_flag = 0;
    speed_boost_counter = 0;
    special_samus_palette_frame = 0;
    special_samus_palette_timer = 0;
    if ((equipped_items & 0x20) != 0) {
      CopyToSamusSuitPalette(addr_kSamusPalette_GravitySuit);
    } else if ((equipped_items & 1) != 0) {
      CopyToSamusSuitPalette(addr_kSamusPalette_VariaSuit);
    } else {
      CopyToSamusSuitPalette(addr_kSamusPalette_PowerSuit);
    }
  }
  if ((speed_echoes_index & 0x8000u) == 0) {
    speed_echoes_index = -1;
    if (samus_pose_x_dir == 4) {
      speed_echo_xspeed[0] = -8;
      speed_echo_xspeed[1] = -8;
    } else {
      speed_echo_xspeed[0] = 8;
      speed_echo_xspeed[1] = 8;
    }
  }
}

void Samus_LoadSuitPalette(void) {  // 0x91DEBA
  if ((equipped_items & 0x20) != 0) {
    CopyToSamusSuitPalette(addr_kSamusPalette_GravitySuit);
  } else if ((equipped_items & 1) != 0) {
    CopyToSamusSuitPalette(addr_kSamusPalette_VariaSuit);
  } else {
    CopyToSamusSuitPalette(addr_kSamusPalette_PowerSuit);
  }
}

void Samus_LoadSuitTargetPalette(void) {  // 0x91DEE6
  if ((equipped_items & 0x20) != 0) {
    CopyToSamusSuitTargetPalette(addr_kSamusPalette_GravitySuit);
  } else if ((equipped_items & 1) != 0) {
    CopyToSamusSuitTargetPalette(addr_kSamusPalette_VariaSuit);
  } else {
    CopyToSamusSuitTargetPalette(addr_kSamusPalette_PowerSuit);
  }
}

void Samus_RestoreHealth(uint16 a) {  // 0x91DF12
  R18_ = a;
  uint16 v1 = a + samus_health;
  samus_health = v1;
  if ((int16)(v1 - samus_max_health) >= 0) {
    uint16 v2 = samus_reserve_health + v1 - samus_max_health;
    if ((int16)(v2 - samus_max_reserve_health) >= 0)
      v2 = samus_max_reserve_health;
    samus_reserve_health = v2;
    if (v2) {
      if (!reserve_health_mode)
        reserve_health_mode = 1;
    }
    samus_health = samus_max_health;
  }
}

void Samus_DealDamage(uint16 a) {  // 0x91DF51
  R18_ = a;
  if ((a & 0x8000u) == 0) {
    if (a != 300 && !time_is_frozen_flag) {
      samus_health -= R18_;
      if ((samus_health & 0x8000u) != 0)
        samus_health = 0;
    }
  } else {
    InvalidInterrupt_Crash();
  }
}

void Samus_RestoreMissiles(uint16 a) {  // 0x91DF80
  uint16 v1 = samus_missiles + a;
  samus_missiles = v1;
  if ((int16)(v1 - samus_max_missiles) >= 0) {
    R18_ = v1 - samus_max_missiles;
    if (sign16(samus_max_missiles - 99)) {
      samus_reserve_missiles += R18_;
      if ((int16)(samus_reserve_missiles - samus_max_missiles) >= 0)
        samus_reserve_missiles = samus_max_missiles;
    } else {
      samus_reserve_missiles += R18_;
      if (!sign16(samus_reserve_missiles - 99))
        samus_reserve_missiles = 99;
    }
    samus_missiles = samus_max_missiles;
  }
}

void Samus_RestoreSuperMissiles(uint16 a) {  // 0x91DFD3
  uint16 v1 = samus_super_missiles + a;
  samus_super_missiles = v1;
  if ((int16)(v1 - samus_max_super_missiles) >= 0 && v1 != samus_max_super_missiles)
    samus_super_missiles = samus_max_super_missiles;
}

void Samus_RestorePowerBombs(uint16 a) {  // 0x91DFF0
  uint16 v1 = samus_power_bombs + a;
  samus_power_bombs = v1;
  if ((int16)(v1 - samus_max_power_bombs) >= 0 && v1 != samus_max_power_bombs)
    samus_power_bombs = samus_max_power_bombs;
}

void Samus_Initialize(void) {  // 0x91E00D
  static const uint16 word_909EAF = 0;
  static const uint16 word_909EAD = 1;
  static const uint16 word_909EB3 = 0;
  static const uint16 word_909EB1 = 1;
  static const uint16 word_909EA1 = 0x1c00;
  static const uint16 word_909EA7 = 0;
  R18_ = debug_invincibility;
  uint16 v0 = 0xE0B;
  do
    *RomPtr_RAM(v0--) = 0;
  while ((int16)(v0 - 0xA02) >= 0);
  if (game_state != kGameState_40_TransitionToDemo) {
    frame_handler_alfa = FUNC16(EmptyFunction);
    if (loading_game_state == kGameState_34_CeresGoesBoom) {
      frame_handler_beta = FUNC16(SetContactDamageIndexAndUpdateMinimap);
      samus_draw_handler = FUNC16(SamusDrawHandler_Default);
      samus_momentum_routine_index = -1;
      samus_special_transgfx_index = 0;
      samus_hurt_switch_index = 0;
      Samus_LoadSuitPalette();
      samus_input_handler = FUNC16(Samus_InputHandler_E913);
    } else {
      frame_handler_beta = FUNC16(Samus_Func16);
      samus_draw_handler = FUNC16(SamusDrawHandler_Default);
      samus_momentum_routine_index = 0;
      samus_special_transgfx_index = 0;
      samus_hurt_switch_index = 0;
      samus_input_handler = FUNC16(Samus_InputHandler_E913);
      debug_invincibility = R18_;
    }
  }
  samus_new_pose = -1;
  samus_new_pose_interrupted = -1;
  samus_new_pose_transitional = -1;
  if (area_index == 6)
    frame_handler_gamma = FUNC16(Samus_Func3);
  else
    frame_handler_gamma = FUNC16(nullsub_152);
  samus_movement_handler = FUNC16(Samus_MovementHandler_Normal);
  UNUSED_word_7E0A5E = -2764;
  samus_prev_health_for_flash = 50;
  samus_visor_palette_timer_index = 1537;
  uint16 v1 = 0;
  do {
    projectile_bomb_pre_instructions[v1 >> 1] = FUNC16(ProjPreInstr_Empty);
    v1 += 2;
  } while ((int16)(v1 - 20) < 0);
  grapple_beam_function = FUNC16(GrappleBeamFunc_Inactive);
  enable_horiz_slope_coll = 3;
  samus_hurt_flash_counter = 0;
  samus_special_super_palette_flags = 0;
  absolute_moved_last_frame_x_fract = word_909EAF;
  absolute_moved_last_frame_x = word_909EAD;
  absolute_moved_last_frame_y_fract = word_909EB3;
  absolute_moved_last_frame_y = word_909EB1;
  for (int i = 510; i >= 0; i -= 2)
    hdma_table_1[i >> 1] = 255;
  samus_y_subaccel = word_909EA1;
  samus_y_accel = word_909EA7;
  fx_y_pos = -1;
  lava_acid_y_pos = -1;
  UpdateBeamTilesAndPalette();
  cinematic_function = 0;
  samus_pose = kPose_00_FaceF_Powersuit;
  *(uint16 *)&samus_pose_x_dir = 0;
  samus_prev_pose = 0;
  *(uint16 *)&samus_prev_pose_x_dir = 0;
  samus_last_different_pose = 0;
  *(uint16 *)&samus_last_different_pose_x_dir = 0;
  enemy_index_to_shake = -1;
  hud_item_index = 0;
  samus_auto_cancel_hud_item_index = 0;
  samus_invincibility_timer = 0;
  samus_knockback_timer = 0;
  samus_hurt_flash_counter = 0;
  debug_invincibility = 0;
  if (game_state == kGameState_40_TransitionToDemo)
    LoadDemoData();
  samus_prev_health_for_flash = samus_health;
}

uint8 Xray_Initialize(void) {  // 0x91E16D
  static const uint8 byte_91E291[28] = {
  1, 1, 0, 0, 0, 2, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 1, 0, 0,
  0, 0, 0, 0,
  };

  if (cooldown_timer == 7 && bomb_counter == 5 && samus_x_speed_divisor == 2
      || !sign16(samus_pose - kPose_A4_FaceR_LandJump)
      && (sign16(samus_pose - kPose_A8_FaceR_Grappling)
          || !sign16(samus_pose - kPose_E0_FaceR_LandJump_AimU) && sign16(samus_pose - kPose_E8_FaceR_Drained_CrouchFalling))
      || game_state != 8
      || power_bomb_explosion_status
      || samus_y_speed
      || samus_y_subspeed
      || !byte_91E291[samus_prev_movement_type]) {
    return 0;
  }
  if (byte_91E291[samus_movement_type] == 1) {
    if (samus_pose_x_dir == 4)
      samus_new_pose_interrupted = kPose_D6_FaceL_Xray_Stand;
    else
      samus_new_pose_interrupted = kPose_D5_FaceR_Xray_Stand;
  } else {
    if (byte_91E291[samus_movement_type] != 2)
      return 0;
    if (samus_pose_x_dir == 4)
      samus_new_pose_interrupted = kPose_DA_FaceL_Xray_Crouch;
    else
      samus_new_pose_interrupted = kPose_D9_FaceR_Xray_Crouch;
  }
  time_is_frozen_flag = 1;
  samus_special_transgfx_index = 5;
  for (int i = 510; i >= 0; i -= 2)
    hdma_table_1[i >> 1] = 255;
  DisableEnemyProjectiles();
  DisablePLMs();
  DisableAnimtiles();
  DisablePaletteFx();
  mov24((LongPtr *)&demo_enable, 0x980001);
  *(uint16 *)((char *)&demo_num_input_frames + 1) = 0;
  demo_input_prev = -26424;
  mov24((LongPtr *)&demo_input_prev_new, 0x999098);
  *(uint16 *)((char *)&demo_backup_prev_controller_input + 1) = 0;
  demo_input_pre_instr = 0;
  demo_input_instr_timer = 0;
  demo_input_instr_ptr = 0;
  demo_input = 0;
  demo_input_new = 0;
  if (samus_pose_x_dir == 4)
    xray_angle = 192;
  else
    xray_angle = 64;
  return 1;
}

void ResponsibleForXrayStandupGlitch(void) {  // 0x91E2AD
  if (samus_movement_type == kMovementType_05_Crouching) {
    if (samus_pose_x_dir == 4)
      samus_pose = kPose_28_FaceL_Crouch;
    else
      samus_pose = kPose_27_FaceR_Crouch;
  } else if (samus_pose_x_dir == kPose_04_FaceL_AimU) {
    samus_pose = kPose_02_FaceL_Normal;
  } else {
    samus_pose = kPose_01_FaceR_Normal;
  }
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
  samus_last_different_pose = samus_prev_pose;
  *(uint16 *)&samus_last_different_pose_x_dir = *(uint16 *)&samus_prev_pose_x_dir;
  samus_prev_pose = samus_pose;
  *(uint16 *)&samus_prev_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
  samus_movement_handler = FUNC16(Samus_MovementHandler_Normal);
  samus_input_handler = FUNC16(Samus_InputHandler_E913);
  demo_timer_counter = -1;
  R18_ = *(&kPoseParams[0].y_radius + (uint16)(8 * samus_pose)) - samus_y_radius;
  if ((R18_ & 0x8000u) == 0) {
    samus_y_pos -= R18_;
    samus_prev_y_pos = samus_y_pos;
  }
  EnableEnemyProjectiles();
  EnablePLMs();
  EnableAnimtiles();
  EnablePaletteFx();
}

void MakeSamusFaceForward(void) {  // 0x91E3F6
  if ((equipped_items & 0x20) != 0 || (equipped_items & 1) != 0)
    samus_pose = kPose_9B_FaceF_VariaGravitySuit;
  else
    samus_pose = kPose_00_FaceF_Powersuit;
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
  samus_last_different_pose = samus_prev_pose;
  *(uint16 *)&samus_last_different_pose_x_dir = *(uint16 *)&samus_prev_pose_x_dir;
  samus_prev_pose = samus_pose;
  *(uint16 *)&samus_prev_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
  if (samus_y_radius != 24) {
    samus_y_pos -= 3;
    samus_prev_y_pos = samus_y_pos;
  }
  frame_handler_alfa = FUNC16(Samus_FrameHandlerAlfa_Func13);
  frame_handler_beta = FUNC16(SetContactDamageIndexAndUpdateMinimap);
  samus_new_pose = -1;
  samus_new_pose_interrupted = -1;
  samus_new_pose_transitional = -1;
  samus_momentum_routine_index = 0;
  samus_special_transgfx_index = 0;
  samus_hurt_switch_index = 0;
  CallSomeSamusCode(0x1Fu);
  samus_x_extra_run_speed = 0;
  samus_x_extra_run_subspeed = 0;
  samus_x_base_speed = 0;
  samus_x_base_subspeed = 0;
  samus_y_subspeed = 0;
  samus_y_speed = 0;
  samus_y_dir = 0;
  used_for_ball_bounce_on_landing = 0;
  samus_x_accel_mode = 0;
  flare_counter = 0;
  flare_animation_frame = 0;
  flare_slow_sparks_anim_frame = 0;
  flare_fast_sparks_anim_frame = 0;
  flare_animation_timer = 0;
  flare_slow_sparks_anim_timer = 0;
  flare_fast_sparks_anim_timer = 0;
  Samus_LoadSuitPalette();
}

static Func_U8 *const kSomeMotherBrainScripts[5] = {  // 0x91E4AD
  SomeMotherBrainScripts_0,
  SomeMotherBrainScripts_1,
  SomeMotherBrainScripts_2,
  SomeMotherBrainScripts_3_EnableHyperBeam,
  SomeMotherBrainScripts_4,
};

void SomeMotherBrainScripts(uint16 a) {
  if (kSomeMotherBrainScripts[a]() & 1) {
    samus_last_different_pose = samus_prev_pose;
    *(uint16 *)&samus_last_different_pose_x_dir = *(uint16 *)&samus_prev_pose_x_dir;
    samus_prev_pose = samus_pose;
    *(uint16 *)&samus_prev_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
    samus_new_pose = -1;
    samus_new_pose_interrupted = -1;
    samus_new_pose_transitional = -1;
    samus_momentum_routine_index = 0;
    samus_special_transgfx_index = 0;
    samus_hurt_switch_index = 0;
  }
}

uint8 SomeMotherBrainScripts_0(void) {  // 0x91E4F8
  R18_ = 21 - samus_y_radius;
  samus_y_pos -= 21 - samus_y_radius;
  samus_prev_y_pos = samus_y_pos;
  if (samus_pose_x_dir == 4)
    samus_pose = kPose_E9_FaceL_Drained_CrouchFalling;
  else
    samus_pose = kPose_E8_FaceR_Drained_CrouchFalling;
  samus_anim_frame_skip = 2;
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
  samus_y_radius = *(&kPoseParams[0].y_radius + (uint16)(8 * samus_pose));
  samus_x_base_speed = 0;
  samus_x_base_subspeed = 0;
  samus_y_subspeed = 0;
  samus_y_speed = 0;
  samus_y_dir = 2;
  flare_counter = 0;
  flare_animation_frame = 0;
  flare_slow_sparks_anim_frame = 0;
  flare_fast_sparks_anim_frame = 0;
  flare_animation_timer = 0;
  flare_slow_sparks_anim_timer = 0;
  flare_fast_sparks_anim_timer = 0;
  Samus_LoadSuitPalette();
  return 1;
}

uint8 SomeMotherBrainScripts_1(void) {  // 0x91E571
  samus_anim_frame_timer = 16;
  samus_anim_frame = 0;
  if (samus_pose_x_dir == 4)
    samus_pose = kPose_EB_FaceL_Drained_Stand;
  else
    samus_pose = kPose_EA_FaceR_Drained_Stand;
  frame_handler_gamma = FUNC16(nullsub_152);
  return 1;
}

uint8 SomeMotherBrainScripts_2(void) {  // 0x91E59B
  if (samus_pose == kPose_E8_FaceR_Drained_CrouchFalling || samus_pose == kPose_E9_FaceL_Drained_CrouchFalling) {
    samus_anim_frame_timer = 1;
    samus_anim_frame = 13;
  } else if (samus_pose == kPose_EA_FaceR_Drained_Stand || samus_pose == kPose_EB_FaceL_Drained_Stand) {
    samus_anim_frame_timer = 1;
    samus_anim_frame = 4;
  }
  samus_y_radius = *(&kPoseParams[0].y_radius + (uint16)(8 * samus_pose));
  samus_x_base_speed = 0;
  samus_x_base_subspeed = 0;
  samus_y_subspeed = 0;
  samus_y_speed = 0;
  samus_y_dir = 2;
  return 1;
}

uint8 SomeMotherBrainScripts_3_EnableHyperBeam(void) {  // 0x91E5F0
  equipped_beams = 4105;
  UpdateBeamTilesAndPalette();
  SpawnPalfxObject(addr_stru_8DE1F0);
  hyper_beam_flag = FUNC16(Samus_InputHandler);
  play_resume_charging_beam_sfx = 0;
  return 0;
}

uint8 SomeMotherBrainScripts_4(void) {  // 0x91E60C
  samus_anim_frame_timer = 16;
  samus_anim_frame = 8;
  if (samus_pose_x_dir == 4)
    samus_pose = kPose_E9_FaceL_Drained_CrouchFalling;
  else
    samus_pose = kPose_E8_FaceR_Drained_CrouchFalling;
  return 1;
}

void nullsub_17(void) {}

static Func_V *const off_91E6E1[28] = {  // 0x91E633
  SamusFunc_E633_0,
  nullsub_17,
  nullsub_17,
  SamusFunc_E633_3,
  SamusFunc_E633_4,
  nullsub_17,
  nullsub_17,
  nullsub_17,
  SamusFunc_E633_4,
  nullsub_17,
  nullsub_17,
  nullsub_17,
  nullsub_17,
  nullsub_17,
  nullsub_17,
  nullsub_17,
  nullsub_17,
  SamusFunc_E633_17,
  SamusFunc_E633_17,
  SamusFunc_E633_17,
  SamusFunc_E633_20,
  nullsub_17,
  nullsub_17,
  nullsub_17,
  nullsub_17,
  nullsub_17,
  nullsub_17,
  nullsub_17,
};

void SamusFunc_E633(void) {
  off_91E6E1[samus_movement_type]();
  if ((equipped_items & 0x2000) != 0) {
    if (samus_has_momentum_flag && !speed_boost_counter) {
      special_samus_palette_timer = speed_boost_counter;
      special_samus_palette_frame = 0;
      speed_boost_counter = kSpeedBoostToCtr[0];
    }
  } else {
    speed_echoes_index = 0;
    speed_echo_xspeed[0] = 0;
    speed_echo_xspeed[1] = 0;
    samus_has_momentum_flag = 0;
    speed_boost_counter = 0;
    special_samus_palette_frame = 0;
    special_samus_palette_timer = 0;
    speed_echo_xpos[0] = 0;
    speed_echo_xpos[1] = 0;
    speed_echo_ypos[0] = 0;
    speed_echo_ypos[1] = 0;
  }
  if (grapple_beam_function == (uint16)FUNC16(GrappleBeamFunc_Inactive)) {
    if ((equipped_beams & 0x1000) != 0) {
      if (!sign16(flare_counter - 16))
        QueueSfx1_Max6(0x41u);
    } else {
      flare_counter = 0;
      flare_animation_frame = 0;
      flare_slow_sparks_anim_frame = 0;
      flare_fast_sparks_anim_frame = 0;
      flare_animation_timer = 0;
      flare_slow_sparks_anim_timer = 0;
      flare_fast_sparks_anim_timer = 0;
    }
  } else {
    LoadProjectilePalette(2u);
    QueueSfx1_Max6(6u);
  }
  Samus_LoadSuitPalette();
  if (sign16(samus_health - 31))
    QueueSfx3_Max6(2u);
}

void Samus_UpdatePreviousPose_0(void) {  // 0x91E719
  samus_last_different_pose = samus_prev_pose;
  *(uint16 *)&samus_last_different_pose_x_dir = *(uint16 *)&samus_prev_pose_x_dir;
  samus_prev_pose = samus_pose;
  *(uint16 *)&samus_prev_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
}

void SamusFunc_E633_0(void) {  // 0x91E733
  if (samus_pose) {
    if (samus_pose == kPose_9B_FaceF_VariaGravitySuit && (equipped_items & 1) == 0 && (equipped_items & 0x20) == 0) {
      samus_pose = kPose_00_FaceF_Powersuit;
      goto LABEL_10;
    }
  } else if ((equipped_items & 1) != 0 || (equipped_items & 0x20) != 0) {
    samus_pose = kPose_9B_FaceF_VariaGravitySuit;
LABEL_10:
    SamusFunc_F433();
    Samus_SetAnimationFrameIfPoseChanged();
    Samus_UpdatePreviousPose_0();
  }
}

void SamusFunc_E633_3(void) {  // 0x91E776
  if (samus_pose_x_dir == 4)
    *(uint16 *)&samus_prev_pose_x_dir = 260;
  else
    *(uint16 *)&samus_prev_pose_x_dir = 264;
  if (samus_pose != kPose_81_FaceR_Screwattack && samus_pose != kPose_82_FaceL_Screwattack) {
    if (samus_pose != kPose_1B_FaceR_SpaceJump && samus_pose != kPose_1C_FaceL_SpaceJump)
      goto LABEL_18;
    if ((equipped_items & 8) != 0) {
      if (samus_pose_x_dir == 4)
        samus_pose = kPose_82_FaceL_Screwattack;
      else
        samus_pose = kPose_81_FaceR_Screwattack;
      goto LABEL_18;
    }
    goto LABEL_15;
  }
  if ((equipped_items & 8) == 0) {
LABEL_15:
    if (samus_pose_x_dir == 4)
      samus_pose = kPose_1A_FaceL_SpinJump;
    else
      samus_pose = kPose_19_FaceR_SpinJump;
  }
LABEL_18:
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
  if (samus_pose_x_dir == 4)
    *(uint16 *)&samus_prev_pose_x_dir = 772;
  else
    *(uint16 *)&samus_prev_pose_x_dir = 776;
  Samus_UpdatePreviousPose_0();
}

void SamusFunc_E633_4(void) {  // 0x91E83A
  if ((equipped_items & 2) != 0) {
    if (samus_pose_x_dir == 4)
      samus_pose = kPose_7A_FaceL_Springball_Ground;
    else
      samus_pose = kPose_79_FaceR_Springball_Ground;
    SamusFunc_F433();
    Samus_SetAnimationFrameIfPoseChanged();
    Samus_UpdatePreviousPose_0();
  }
}

void SamusFunc_E633_17(void) {  // 0x91E867
  if ((equipped_items & 2) == 0) {
    if (samus_pose_x_dir == 4)
      samus_pose = kPose_41_FaceL_Morphball_Ground;
    else
      samus_pose = kPose_1D_FaceR_Morphball_Ground;
    SamusFunc_F433();
    Samus_SetAnimationFrameIfPoseChanged();
    Samus_UpdatePreviousPose_0();
  }
}

void SamusFunc_E633_20(void) {  // 0x91E894
  if ((equipped_items & 8) != 0)
    samus_anim_frame = 23;
  else
    samus_anim_frame = 3;
}

void nullsub_18(void) {
}

static Func_V *const kSamus_HandleTransFromBlockColl[6] = {  // 0x91E8B6
  nullsub_18,
  Samus_HandleTransFromBlockColl_1,
  Samus_HandleTransFromBlockColl_2,
  Samus_HandleTransFromBlockColl_3,
  Samus_HandleTransFromBlockColl_4,
  Samus_HandleTransFromBlockColl_5,
};

void Samus_HandleTransFromBlockColl(void) {
  if (input_to_pose_calc)
    kSamus_HandleTransFromBlockColl[(uint16)(2 * (uint8)input_to_pose_calc) >> 1]();
}
void Samus_HandleTransFromBlockColl_3(void) {  // 0x91E8D8
  samus_new_pose = samus_pose;
  samus_momentum_routine_index = 5;
}

void Samus_HandleTransFromBlockColl_4(void) {  // 0x91E8E5
  samus_new_pose = samus_pose;
  samus_momentum_routine_index = 5;
}

#define word_91E921 ((uint16*)RomPtr(0x91e921))

uint8 nullsub_18_U8(void) {
  return 0;
}

static Func_U8 *const off_91E951[6] = {  // 0x91E8F2
  Samus_HandleTransFromBlockColl_1_0,
  Samus_HandleTransFromBlockColl_1_1,
  Samus_HandleTransFromBlockColl_1_2,
  Samus_HandleTransFromBlockColl_1_3,
  nullsub_18_U8,
  Samus_HandleTransFromBlockColl_1_5,
};

void Samus_HandleTransFromBlockColl_2(void) {

  if (HIBYTE(input_to_pose_calc) != 4) {
    uint16 v0 = 4 * HIBYTE(input_to_pose_calc);
    if (samus_pose_x_dir == 4)
      samus_new_pose = word_91E921[(v0 >> 1) + 1];
    else
      samus_new_pose = word_91E921[v0 >> 1];
    samus_momentum_routine_index = 5;
  }
}

void Samus_HandleTransFromBlockColl_1(void) {  // 0x91E931
  if (HIBYTE(input_to_pose_calc) != 4) {
    if (off_91E951[(uint16)(2 * HIBYTE(input_to_pose_calc)) >> 1]() & 1)
      samus_momentum_routine_index = 0;
    else
      samus_momentum_routine_index = 5;
  }
}
#define word_91E9F3 ((uint16*)RomPtr(0x91e9f3))
uint8 Samus_HandleTransFromBlockColl_1_0(void) {  // 0x91E95D
  int16 v0;

  if (samus_prev_movement_type2 == kMovementType_03_SpinJumping
      || samus_prev_movement_type2 == kMovementType_14_WallJumping) {
    if (samus_pose_x_dir == 4)
      samus_new_pose = kPose_A7_FaceL_LandSpinJump;
    else
      samus_new_pose = kPose_A6_FaceR_LandSpinJump;
    return 0;
  } else {
    v0 = *(&kPoseParams[0].direction_shots_fired + (uint16)(8 * samus_pose));
    if (v0 == 255) {
      if (samus_pose_x_dir == 4)
        samus_new_pose = kPose_A5_FaceL_LandJump;
      else
        samus_new_pose = kPose_A4_FaceR_LandJump;
      return 0;
    } else {
      if (v0 != 2 && v0 != 7)
        goto LABEL_6;
      if ((button_config_shoot_x & joypad1_lastkeys) == 0) {
        v0 = *(&kPoseParams[0].direction_shots_fired + (uint16)(8 * samus_pose));
LABEL_6:
        samus_new_pose = word_91E9F3[v0];
        return 0;
      }
      if (samus_pose_x_dir == 4)
        samus_new_pose = kPose_E7_FaceL_LandJump_Fire;
      else
        samus_new_pose = kPose_E6_FaceR_LandJump_Fire;
      return 0;
    }
  }
}

uint8 Samus_HandleTransFromBlockColl_1_1(void) {  // 0x91EA07
  int16 v0;

  v0 = 2 * used_for_ball_bounce_on_landing;
  if (2 * used_for_ball_bounce_on_landing) {
    if (v0 == 2) {
      samus_new_pose = samus_pose;
      return 0;
    }
    if (v0 != 4) {
      Unreachable();
      while (1)
        ;
    }
  } else if (!sign16(samus_y_speed - 3)) {
    samus_new_pose = samus_pose;
    return 0;
  }
  if (samus_pose_x_dir == 4)
    samus_new_pose = kPose_41_FaceL_Morphball_Ground;
  else
    samus_new_pose = kPose_1D_FaceR_Morphball_Ground;
  return 0;
}

uint8 Samus_HandleTransFromBlockColl_1_2(void) {  // 0x91EA48
  if (samus_pose_x_dir == 4)
    samus_new_pose = 66;
  else
    samus_new_pose = 32;
  return 0;
}

uint8 Samus_HandleTransFromBlockColl_1_3(void) {  // 0x91EA63
  int16 v1;

  if ((button_config_jump_a & joypad1_lastkeys) != 0) {
    samus_new_pose = samus_pose;
    return 0;
  }
  v1 = 2 * (uint8)used_for_ball_bounce_on_landing;
  if (v1) {
    if (v1 == 2) {
      samus_new_pose = samus_pose;
      return 0;
    }
    if (v1 != 4) {
      Unreachable();
      while (1)
        ;
    }
  } else if (!sign16(samus_y_speed - 3)) {
    samus_new_pose = samus_pose;
    return 0;
  }
  if (samus_pose_x_dir == 4)
    samus_new_pose = kPose_7A_FaceL_Springball_Ground;
  else
    samus_new_pose = 121;
  return 0;
}

uint8 Samus_HandleTransFromBlockColl_1_5(void) {  // 0x91EAB6
  samus_new_pose = samus_pose;
  return 0;
}

void Samus_HandleTransFromBlockColl_5(void) {  // 0x91EABE
  if (samus_pose_x_dir == 4)
    samus_new_pose = kPose_84_FaceL_Walljump;
  else
    samus_new_pose = kPose_83_FaceR_Walljump;
  samus_momentum_routine_index = 5;
}

#define word_91EB74 ((uint16*)RomPtr(0x91eb74))

uint8 Samus_CheckWalkedIntoSomething(void) {  // 0x91EADE
  int16 v0;

  if (samus_collides_with_solid_enemy && samus_movement_type == 1) {
    v0 = samus_pose;
    goto LABEL_13;
  }
  if (samus_new_pose == 0xFFFF || *(&kPoseParams[0].movement_type + (uint16)(8 * samus_new_pose)) != 1)
    goto LABEL_14;
  if (samus_pose_x_dir != 4) {
    R18_ = 1;
    samus_collision_direction = 1;
    R20_ = 0;
    if (!Samus_CheckSolidEnemyColl()) {
      R18_ = 1;
      samus_collision_direction = 1;
      R20_ = 0;
      goto LABEL_10;
    }
LABEL_11:
    v0 = samus_new_pose;
LABEL_13:
    samus_new_pose = word_91EB74[(uint16)(2
                                          * *(&kPoseParams[0].direction_shots_fired
                                              + (uint16)(8 * v0))) >> 1];
    samus_collides_with_solid_enemy = 0;
    return 1;
  }
  R18_ = 1;
  R20_ = 0;
  samus_collision_direction = 0;
  if (Samus_CheckSolidEnemyColl())
    goto LABEL_11;
  R18_ = -1;
  R20_ = 0;
  samus_collision_direction = 0;
LABEL_10:
  if (Samus_MoveRight_NoSolidColl() & 1)
    goto LABEL_11;
LABEL_14:
  samus_collides_with_solid_enemy = 0;
  return 0;
}

void nullsub_19(void) {}
void nullsub_20(void) {}
void nullsub_21(void) {}
void nullsub_22(void) {}
static Func_V *const kSamus_HandleTransitionsA[9] = {  // 0x91EB88
  nullsub_18,
  Samus_HandleTransitionsA_1,
  Samus_HandleTransitionsA_2,
  nullsub_21,
  nullsub_22,
  Samus_HandleTransitionsA_5,
  Samus_HandleTransitionsA_6,
  Samus_HandleTransitionsA_7,
  Samus_HandleTransitionsA_8,
};
static Func_V *const kSamus_HandleTransitionsB[11] = {
  nullsub_18,
  Samus_HandleTransitionsB_1,
  Samus_HandleTransitionsB_2,
  Samus_HandleTransitionsB_3,
  Samus_HandleTransitionsB_4,
  Samus_HandleTransitionsB_5,
  nullsub_19,
  nullsub_20,
  Samus_HandleTransitionsB_8,
  Samus_HandleTransitionsB_9,
  Samus_HandleTransitionsB_10,
};
static Func_V *const kSamus_HandleTransitionsC[9] = {
  nullsub_18,
  Samus_HandleTransitionsC_1,
  Samus_HandleTransitionsC_2,
  Samus_HandleTransitionsC_3,
  Samus_HandleTransitionsC_4,
  Samus_HandleTransitionsC_5,
  Samus_HandleTransitionsC_6,
  Samus_HandleTransitionsC_7,
  Samus_HandleTransitionsC_8,
};

void Samus_HandleTransitions(void) {
  if ((samus_new_pose_transitional & 0x8000u) == 0) {
    if (samus_hurt_switch_index != 3) {
      if (samus_hurt_switch_index == 1)
        goto LABEL_7;
      goto LABEL_6;
    }
    if (samus_special_transgfx_index != 9) {
LABEL_6:
      samus_pose = samus_new_pose_transitional;
      SamusFunc_F433();
      Samus_SetAnimationFrameIfPoseChanged();
LABEL_7:
      kSamus_HandleTransitionsC[samus_hurt_switch_index]();
LABEL_15:
      samus_last_different_pose = samus_prev_pose;
      *(uint16 *)&samus_last_different_pose_x_dir = *(uint16 *)&samus_prev_pose_x_dir;
      samus_prev_pose = samus_pose;
      *(uint16 *)&samus_prev_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
      goto LABEL_16;
    }
  }
  if ((samus_new_pose_interrupted & 0x8000u) == 0) {
    samus_pose = samus_new_pose_interrupted;
    if (!(SamusFunc_F404() & 1))
      kSamus_HandleTransitionsB[samus_special_transgfx_index]();
    goto LABEL_15;
  }
  Samus_CheckWalkedIntoSomething();
  if ((samus_new_pose & 0x8000u) == 0) {
    samus_pose = samus_new_pose;
    if (!(SamusFunc_F404() & 1))
      kSamus_HandleTransitionsA[samus_momentum_routine_index]();
    goto LABEL_15;
  }
LABEL_16:
  input_to_pose_calc = 0;
}

void Samus_HandleTransitionsA_1(void) {  // 0x91EC50
  if (samus_x_base_speed || samus_x_base_subspeed) {
    uint16 v0 = (__PAIR32__(samus_x_extra_run_speed, samus_x_base_subspeed)
                 + __PAIR32__(samus_x_base_speed, samus_x_extra_run_subspeed)) >> 16;
    samus_x_base_subspeed += samus_x_extra_run_subspeed;
    samus_x_base_speed = v0;
    samus_x_accel_mode = 2;
    Samus_CancelSpeedBoost();
    samus_x_extra_run_subspeed = 0;
    samus_x_extra_run_speed = 0;
    SamusFunc_EC80();
  } else {
    Samus_HandleTransitionsA_2();
  }
}

void Samus_HandleTransitionsA_6(void) {  // 0x91EC85
  samus_x_accel_mode = 0;
  samus_x_base_speed = 0;
  samus_x_base_subspeed = 0;
  Samus_HandleTransitionsA_8();
}

void Samus_HandleTransitionsA_8(void) {  // 0x91EC8E
  Samus_CancelSpeedBoost();
  samus_x_extra_run_subspeed = 0;
  samus_x_extra_run_speed = 0;
  SamusFunc_EC80();
}



void Samus_HandleTransitionsA_2(void) {  // 0x91ECD0
  samus_x_accel_mode = 0;
  Samus_CancelSpeedBoost();
}

static const uint16 word_91ED36[12] = {  // 0x91ECDA
  5, 5, 9, 9, 0, 0, 0, 0,
  0, 0, 0, 0,
};

void Samus_HandleTransitionsA_7(void) {
  if (sign16(samus_pose - kPose_DB)) {
    R18_ = word_91ED36[samus_pose - 53];
    if (!R18_)
      goto LABEL_5;
    R20_ = 0;
LABEL_4:
    samus_y_radius = *(&kPoseParams[0].y_radius + (uint16)(8 * samus_pose));
    Samus_CollDetectChangedPose();
LABEL_5:
    samus_y_pos += R18_;
    samus_prev_y_pos = samus_y_pos;
    if (used_for_ball_bounce_on_landing) {
      used_for_ball_bounce_on_landing = 0;
      samus_y_subspeed = 0;
      samus_y_speed = 0;
      samus_y_dir = 0;
    }
    return;
  }
  if (!sign16(samus_pose - kPose_F1_FaceR_CrouchTrans_AimU) && sign16(samus_pose - kPose_F7_FaceR_StandTrans_AimU)) {
    R18_ = 5;
    R20_ = 0;
    goto LABEL_4;
  }
}

static Func_U8 *const kSamus_HandleTransitionsB_1[28] = {  // 0x91ED4E
  Samus_HandleTransitionsB_1_0,
  Samus_HandleTransitionsB_1_0,
  Samus_HandleTransitionsB_1_0,
  Samus_HandleTransitionsB_1_0,
  Samus_HandleTransitionsB_1_4,
  Samus_HandleTransitionsB_1_0,
  Samus_HandleTransitionsB_1_6,
  Samus_HandleTransitionsB_1_7,
  Samus_HandleTransitionsB_1_4,
  Samus_HandleTransitionsB_1_4,
  Samus_HandleTransitionsB_1_10,
  Samus_HandleTransitionsB_1_11,
  Samus_HandleTransitionsB_1_11,
  Samus_HandleTransitionsB_1_0,
  Samus_HandleTransitionsB_1_11,
  Samus_HandleTransitionsB_1_11,
  Samus_HandleTransitionsB_1_0,
  Samus_HandleTransitionsB_1_4,
  Samus_HandleTransitionsB_1_4,
  Samus_HandleTransitionsB_1_4,
  Samus_HandleTransitionsB_1_0,
  Samus_HandleTransitionsB_1_0,
  Samus_HandleTransitionsB_1_10,
  Samus_HandleTransitionsB_1_11,
  Samus_HandleTransitionsB_1_11,
  Samus_HandleTransitionsB_1_11,
  Samus_HandleTransitionsB_1_11,
  Samus_HandleTransitionsB_1_11,
};

void Samus_HandleTransitionsB_1(void) {
  kSamus_HandleTransitionsB_1[samus_prev_movement_type2]();
  Samus_SetSpeedForKnockback_Y();
  bomb_jump_dir = 0;
  samus_contact_damage_index = 0;
  samus_hurt_flash_counter = 1;
}

uint8 Samus_HandleTransitionsB_1_10(void) {  // 0x91EDA2
  return 0;
}

uint8 Samus_HandleTransitionsB_1_11(void) {  // 0x91EDA4
  return 0;
}

uint8 Samus_HandleTransitionsB_1_6(void) {  // 0x91EDA6
  if (frame_handler_gamma == (uint16)FUNC16(Samus_Func9))
    return 0;
  else
    return Samus_HandleTransitionsB_1_0();
}

uint8 Samus_HandleTransitionsB_1_0(void) {  // 0x91EDB0
  if (samus_pose_x_dir == 4) {
    if (knockback_x_dir) {
      if ((joypad1_lastkeys & 0x200) != 0)
        knockback_dir = 5;
      else
        knockback_dir = 2;
    } else if ((joypad1_lastkeys & 0x200) != 0) {
      knockback_dir = 4;
    } else {
      knockback_dir = 1;
    }
  } else if (knockback_x_dir) {
    if ((joypad1_lastkeys & 0x100) != 0)
      knockback_dir = 5;
    else
      knockback_dir = 2;
  } else if ((joypad1_lastkeys & 0x100) != 0) {
    knockback_dir = 4;
  } else {
    knockback_dir = 1;
  }
  samus_movement_handler = FUNC16(Samus_MoveHandler_Knockback);
  return 1;
}

uint8 Samus_HandleTransitionsB_1_4(void) {  // 0x91EE27
  if (samus_pose_x_dir == 4)
    knockback_dir = 1;
  else
    knockback_dir = 2;
  samus_movement_handler = FUNC16(Samus_MoveHandler_Knockback);
  return 0;
}

uint8 Samus_HandleTransitionsB_1_7(void) {  // 0x91EE48
  if (samus_pose_x_dir == 4)
    knockback_dir = 1;
  else
    knockback_dir = 2;
  samus_movement_handler = FUNC16(Samus_MoveHandler_Knockback);
  return 1;
}

void Samus_HandleTransitionsB_2(void) {  // 0x91EE69
  knockback_dir = 0;
  samus_movement_handler = FUNC16(Samus_MovementHandler_Normal);
  samus_y_speed = 0;
  samus_y_subspeed = 0;
  samus_y_dir = 0;
  Samus_AlignBottomWithPrevPose();
}

void Samus_HandleTransitionsB_3(void) {  // 0x91EE80
  bomb_jump_dir = (uint8)bomb_jump_dir | 0x800;
  samus_movement_handler = FUNC16(Samus_MoveHandler_BombJumpStart);
  if (samus_input_handler != (uint16)FUNC16(Samus_InputHandler_E91D))
    samus_input_handler = FUNC16(nullsub_152);
}

void Samus_HandleTransitionsB_4(void) {  // 0x91EEA1
  Samus_InitJump();
}

void Samus_HandleTransitionsB_5(void) {  // 0x91EEA6
  switch (samus_movement_type) {
  case kMovementType_00_Standing:
    goto LABEL_5;
  case kMovementType_05_Crouching:
    if (samus_pose_x_dir == 4)
      xray_angle = 192;
    else
      xray_angle = 64;
    goto LABEL_11;
  case kMovementType_15_RanIntoWall:
  case kMovementType_01_Running:
LABEL_5:
    if (samus_pose_x_dir == 4)
      xray_angle = 192;
    else
      xray_angle = 64;
LABEL_11:
    samus_anim_frame = 2;
    samus_anim_frame_timer = 63;
    samus_movement_handler = FUNC16(SamusMovementType_Xray);
    samus_input_handler = FUNC16(Samus_Func20_);
    timer_for_shine_timer = 8;
    special_samus_palette_timer = 1;
    special_samus_palette_frame = 0;
    samus_shine_timer = 0;
    flare_counter = 0;
    flare_animation_frame = 0;
    flare_slow_sparks_anim_frame = 0;
    flare_fast_sparks_anim_frame = 0;
    flare_animation_timer = 0;
    flare_slow_sparks_anim_timer = 0;
    flare_fast_sparks_anim_timer = 0;
    QueueSfx1_Max6(9u);
    break;
  }
}

void Samus_HandleTransitionsB_8(void) {  // 0x91EF3B
  samus_y_pos -= 5;
  samus_prev_y_pos = samus_y_pos;
  frame_handler_alfa = FUNC16(EmptyFunction);
}

void Samus_HandleTransitionsB_9(void) {  // 0x91EF4F
  int16 v0;
  int16 v1;

  GrappleBeamFunc_BD95();
  v0 = samus_x_pos - samus_prev_x_pos;
  if ((int16)(samus_x_pos - samus_prev_x_pos) < 0) {
    if (sign16(v0 + 12))
      samus_prev_x_pos = samus_x_pos + 12;
  } else if (!sign16(v0 - 13)) {
    samus_prev_x_pos = samus_x_pos - 12;
  }
  v1 = samus_y_pos - samus_prev_y_pos;
  if ((int16)(samus_y_pos - samus_prev_y_pos) < 0) {
    if (sign16(v1 + 12))
      samus_prev_y_pos = samus_y_pos + 12;
  } else if (!sign16(v1 - 13)) {
    samus_prev_y_pos = samus_y_pos - 12;
  }
  Samus_CancelSpeedBoost();
  samus_x_base_speed = 0;
  samus_x_base_subspeed = 0;
  samus_y_subspeed = 0;
  samus_y_speed = 0;
  samus_x_extra_run_speed = 0;
  samus_x_extra_run_subspeed = 0;
}

void Samus_HandleTransitionsB_10(void) {  // 0x91EFBC
  GrappleBeamFunc_BEEB();
  Samus_HandleTransitionsB_9B();
}

void Samus_HandleTransitionsB_9B(void) {  // 0x91EF53
  int16 v0;
  int16 v1;

  v0 = samus_x_pos - samus_prev_x_pos;
  if ((int16)(samus_x_pos - samus_prev_x_pos) < 0) {
    if (sign16(v0 + 12))
      samus_prev_x_pos = samus_x_pos + 12;
  } else if (!sign16(v0 - 13)) {
    samus_prev_x_pos = samus_x_pos - 12;
  }
  v1 = samus_y_pos - samus_prev_y_pos;
  if ((int16)(samus_y_pos - samus_prev_y_pos) < 0) {
    if (sign16(v1 + 12))
      samus_prev_y_pos = samus_y_pos + 12;
  } else if (!sign16(v1 - 13)) {
    samus_prev_y_pos = samus_y_pos - 12;
  }
  Samus_CancelSpeedBoost();
  samus_x_base_speed = 0;
  samus_x_base_subspeed = 0;
  samus_y_subspeed = 0;
  samus_y_speed = 0;
  samus_x_extra_run_speed = 0;
  samus_x_extra_run_subspeed = 0;
}


void nullsub_23(void) {  // 0x91EFDE
}


static Func_V *const kSamus_HandleTransitionsA_5[7] = {  // 0x91EFC4
  sub_91EFC3,
  Samus_HandleTransitionsA_5_1,
  Samus_HandleTransitionsA_5_2,
  nullsub_23,
  Samus_HandleTransitionsA_5_4,
  Samus_HandleTransitionsA_5_5,
  Samus_HandleTransitionsA_5_6,
};
void Samus_HandleTransitionsA_5(void) {
  kSamus_HandleTransitionsA_5[(uint16)(2 * (uint8)input_to_pose_calc) >> 1]();
}

void Samus_HandleTransitionsA_5_4(void) {  // 0x91EFDF
  samus_y_subspeed = 0;
  samus_y_speed = 0;
  UNUSED_word_7E0B1A = 0;
  samus_y_dir = 2;
}

void Samus_HandleTransitionsA_5_2(void) {  // 0x91EFEF
  if (samus_y_dir != 1) {
    used_for_ball_bounce_on_landing = 0;
    samus_y_subspeed = 0;
    samus_y_speed = 0;
    samus_is_falling_flag = 1;
    samus_y_dir = 2;
  }
  UNUSED_word_7E0A18 = 0;
}

void sub_91EFC3(void) {}
uint8 sub_91EFC3_rv(void) { return 0; }

static Func_U8 *const kSamus_HandleTransitionsA_5_1[6] = {  // 0x91F010
  Samus_HandleTransitionsA_5_1_0,
  Samus_MorphBallBounceNoSpringballTrans,
  Samus_HandleTransitionsA_5_1_2,
  Samus_MorphBallBounceSpringballTrans,
  sub_91EFC3_rv,
  Samus_HandleTransitionsA_5_1_5,
};

void Samus_HandleTransitionsA_5_1(void) {
  HandleLandingSoundEffectsAndGfx();
  if (HIBYTE(input_to_pose_calc) == 4) {
    SamusFunc_F1D3();
  } else if (!(kSamus_HandleTransitionsA_5_1[(uint16)(2 * HIBYTE(input_to_pose_calc)) >> 1]() & 1)) {
    UNUSED_word_7E0A18 = 0;
    samus_x_accel_mode = 0;
    samus_x_base_speed = 0;
    samus_x_base_subspeed = 0;
    SamusFunc_F1D3();
  }
}

void HandleLandingSoundEffectsAndGfx(void) {  // 0x91F046
  if ((samus_prev_movement_type2 == kMovementType_03_SpinJumping
       || samus_prev_movement_type2 == kMovementType_14_WallJumping)
      && !cinematic_function) {
    if (samus_prev_pose == kPose_81_FaceR_Screwattack || samus_prev_pose == kPose_82_FaceL_Screwattack)
      QueueSfx1_Max6(0x34u);
    else
      QueueSfx1_Max6(0x32u);
  }
  if (samus_y_speed && !sign16(samus_y_speed - 5)) {
    if (!cinematic_function)
      QueueSfx3_Max6(4u);
  } else {
    if (!samus_y_subspeed)
      return;
    if (!cinematic_function)
      QueueSfx3_Max6(5u);
  }
  HandleLandingGraphics();
}

static Func_V *const off_91F0AE[8] = {  // 0x91F0A5
  HandleLandingGraphics_Crateria,
  HandleLandingGraphics_Brinstar,
  HandleLandingGraphics_Norfair,
  HandleLandingGraphics_Norfair,
  HandleLandingGraphics_Maridia,
  HandleLandingGraphics_Tourian,
  HandleLandingGraphics_Ceres,
  HandleLandingGraphics_Ceres,
};
void HandleLandingGraphics(void) {
  off_91F0AE[(area_index)]();
}

void HandleLandingGraphics_Ceres(void) {  // 0x91F0BE
  atmospheric_gfx_frame_and_type[2] = 0;
  atmospheric_gfx_frame_and_type[3] = 0;
}
static const uint8 g_byte_91F0F3[17] = {  // 0x91F0C5
  1, 0, 0, 0, 0, 2, 0, 4,
  0, 4, 4, 4, 4, 0, 4, 0,
  0,
};
void HandleLandingGraphics_Crateria(void) {
  if (cinematic_function)
    goto LABEL_13;
  if (room_index == 28) {
    HandleLandingGraphics_Norfair();
    return;
  }
  if ((int16)(room_index - 16) >= 0)
    goto LABEL_13;
  if ((g_byte_91F0F3[room_index] & 1) == 0) {
    if ((g_byte_91F0F3[room_index] & 2) != 0) {
      if (!sign16(samus_y_pos - 944))
        goto LABEL_14;
    } else if ((g_byte_91F0F3[room_index] & 4) != 0) {
      goto LABEL_14;
    }
LABEL_13:
    HandleLandingGraphics_Ceres();
    return;
  }
  if (fx_type != 10)
    goto LABEL_13;
LABEL_14:
  HandleLandingGraphics_Maridia();
}

void HandleLandingGraphics_Maridia(void) {  // 0x91F116
  Samus_GetBottomBoundary();
  if ((fx_y_pos & 0x8000u) == 0) {
    if (sign16(fx_y_pos - R18_) && (fx_liquid_options & 4) == 0)
      return;
LABEL_7:
    atmospheric_gfx_frame_and_type[2] = 256;
    atmospheric_gfx_frame_and_type[3] = 256;
    atmospheric_gfx_anim_timer[2] = 3;
    atmospheric_gfx_anim_timer[3] = 3;
    atmospheric_gfx_x_pos[2] = samus_x_pos + 4;
    atmospheric_gfx_x_pos[3] = samus_x_pos - 3;
    atmospheric_gfx_y_pos[2] = R18_ - 4;
    atmospheric_gfx_y_pos[3] = R18_ - 4;
    return;
  }
  if ((lava_acid_y_pos & 0x8000u) != 0 || !sign16(lava_acid_y_pos - R18_))
    goto LABEL_7;
}

void HandleLandingGraphics_Norfair(void) {  // 0x91F166
  Samus_GetBottomBoundary();
  if ((fx_y_pos & 0x8000u) == 0) {
    if (sign16(fx_y_pos - R18_) && (fx_liquid_options & 4) == 0)
      return;
LABEL_7:
    atmospheric_gfx_frame_and_type[2] = 1536;
    atmospheric_gfx_frame_and_type[3] = 1536;
    atmospheric_gfx_anim_timer[2] = 3;
    atmospheric_gfx_anim_timer[3] = 3;
    atmospheric_gfx_x_pos[2] = samus_x_pos + 8;
    atmospheric_gfx_x_pos[3] = samus_x_pos - 8;
    atmospheric_gfx_y_pos[2] = R18_;
    atmospheric_gfx_y_pos[3] = R18_;
    return;
  }
  if ((lava_acid_y_pos & 0x8000u) != 0 || !sign16(lava_acid_y_pos - R18_))
    goto LABEL_7;
}

void HandleLandingGraphics_Brinstar(void) {  // 0x91F1B2
  if (room_index == 8)
    HandleLandingGraphics_Norfair();
  else
    HandleLandingGraphics_Tourian();
}

void HandleLandingGraphics_Tourian(void) {  // 0x91F1BA
  if (!sign16(room_index - 5) && (sign16(room_index - 9) || room_index == 11)) {
    HandleLandingGraphics_Norfair();
  } else {
    atmospheric_gfx_frame_and_type[2] = 0;
    atmospheric_gfx_frame_and_type[3] = 0;
  }
}

void SamusFunc_F1D3(void) {  // 0x91F1D3
  samus_is_falling_flag = 0;
  UNUSED_word_7E0B1A = 0;
  UNUSED_word_7E0B2A = 0;
  samus_y_subspeed = 0;
  samus_y_speed = 0;
  samus_y_dir = 0;
  UNUSED_word_7E0B38 = 0;
  used_for_ball_bounce_on_landing = 0;
}

uint8 Samus_HandleTransitionsA_5_1_0(void) {  // 0x91F1EC
  if (samus_input_handler != (uint16)FUNC16(Samus_InputHandler_E91D))
    samus_input_handler = FUNC16(HandleAutoJumpHack);
  return 0;
}
static const uint16 g_word_909EB5 = 1;
static const uint16 g_word_909EB7 = 0;
uint8 Samus_MorphBallBounceNoSpringballTrans(void) {  // 0x91F1FC
  int16 v0;

  v0 = 2 * used_for_ball_bounce_on_landing;
  if (2 * used_for_ball_bounce_on_landing) {
    if (v0 == 2) {
      ++used_for_ball_bounce_on_landing;
      samus_y_dir = 1;
      samus_y_subspeed = g_word_909EB7;
      samus_y_speed = g_word_909EB5 - 1;
      return 1;
    }
    if (v0 != 4) {
      Unreachable();
      while (1)
        ;
    }
  } else if (!sign16(samus_y_speed - 3)) {
    ++used_for_ball_bounce_on_landing;
    samus_y_dir = 1;
    samus_y_subspeed = g_word_909EB7;
    samus_y_speed = g_word_909EB5;
    return 1;
  }
  used_for_ball_bounce_on_landing = 0;
  samus_y_dir = 0;
  samus_y_subspeed = 0;
  samus_y_speed = 0;
  return 0;
}

uint8 Samus_HandleTransitionsA_5_1_2(void) {  // 0x91F253
  used_for_ball_bounce_on_landing = 0;
  enable_horiz_slope_coll = 3;
  return 0;
}

uint8 Samus_MorphBallBounceSpringballTrans(void) {  // 0x91F25E
  int16 v1;

  if ((button_config_jump_a & joypad1_lastkeys) != 0) {
    used_for_ball_bounce_on_landing = 0;
    Samus_InitJump();
    return 1;
  }
  v1 = 2 * (uint8)used_for_ball_bounce_on_landing;
  if (v1) {
    if (v1 == 2) {
      used_for_ball_bounce_on_landing = 1538;
      samus_y_dir = 1;
      samus_y_subspeed = g_word_909EB7;
      samus_y_speed = g_word_909EB5 - 1;
      return 1;
    }
    if (v1 != 4) {
      Unreachable();
      while (1)
        ;
    }
  } else if (!sign16(samus_y_speed - 3)) {
    used_for_ball_bounce_on_landing = 1537;
    samus_y_dir = 1;
    samus_y_subspeed = g_word_909EB7;
    samus_y_speed = g_word_909EB5;
    return 1;
  }
  used_for_ball_bounce_on_landing = 0;
  samus_y_dir = 0;
  samus_y_subspeed = 0;
  samus_y_speed = 0;
  return 0;
}

uint8 Samus_HandleTransitionsA_5_1_5(void) {  // 0x91F2CE
  used_for_ball_bounce_on_landing = 0;
  return 0;
}

void Samus_HandleTransitionsA_5_5(void) {  // 0x91F2D3
  samus_x_accel_mode = 0;
  samus_collides_with_solid_enemy = 0;
  samus_is_falling_flag = 0;
  UNUSED_word_7E0B1A = 0;
  samus_x_base_speed = 0;
  samus_x_base_subspeed = 0;
  UNUSED_word_7E0A18 = 0;
  QueueSfx3_Max6(5u);
}

void Samus_HandleTransitionsA_5_6(void) {  // 0x91F2F0
  if (samus_collides_with_solid_enemy) {
    if (samus_prev_movement_type2 == 9) {
      enable_horiz_slope_coll = samus_pose_x_dir != 4;
      UNUSED_word_7E0A18 = 0;
    }
  }
}

void Samus_HandleTransitionsC_1(void) {  // 0x91F31D
  knockback_dir = 0;
  samus_movement_handler = FUNC16(Samus_MovementHandler_Normal);
  used_for_ball_bounce_on_landing = 0;
  samus_y_subspeed = 0;
  samus_y_speed = 0;
  samus_is_falling_flag = 1;
  samus_y_dir = 2;
  Samus_AlignBottomWithPrevPose();
  if (samus_input_handler != (uint16)FUNC16(Samus_InputHandler_E91D))
    samus_input_handler = FUNC16(Samus_InputHandler_E913);
}

void Samus_HandleTransitionsC_2(void) {  // 0x91F34E
  Samus_AlignBottomWithPrevPose();
  samus_movement_handler = FUNC16(Samus_MovementHandler_Normal);
  if (game_state == kGameState_42_PlayingDemo)
    samus_input_handler = FUNC16(Samus_InputHandler_E91D);
  else
    samus_input_handler = FUNC16(Samus_InputHandler_E913);
}

void Samus_HandleTransitionsC_3(void) {  // 0x91F36E
  used_for_ball_bounce_on_landing = 0;
  samus_anim_frame_timer += samus_anim_frame_buffer;
}

void Samus_HandleTransitionsC_4(void) {  // 0x91F37C
  samus_x_pos = layer1_x_pos + 128;
  samus_prev_x_pos = layer1_x_pos + 128;
  samus_y_pos = layer1_y_pos + 128;
  samus_prev_y_pos = layer1_y_pos + 128;
}

void Samus_HandleTransitionsC_5(void) {  // 0x91F397
  samus_y_pos += 5;
  samus_prev_y_pos = samus_y_pos;
}

void Samus_HandleTransitionsC_6(void) {  // 0x91F3A5
  HandleJumpTransition();
}

void Samus_HandleTransitionsC_7(void) {  // 0x91F3AA
  int16 v0;
  int16 v1;

  v0 = samus_x_pos - samus_prev_x_pos;
  if ((int16)(samus_x_pos - samus_prev_x_pos) < 0) {
    if (sign16(v0 + 12))
      samus_prev_x_pos = samus_x_pos + 12;
  } else if (!sign16(v0 - 13)) {
    samus_prev_x_pos = samus_x_pos - 12;
  }
  v1 = samus_y_pos - samus_prev_y_pos;
  if ((int16)(samus_y_pos - samus_prev_y_pos) < 0) {
    if (sign16(v1 + 12))
      samus_prev_y_pos = samus_y_pos + 12;
  } else if (!sign16(v1 - 13)) {
    samus_prev_y_pos = samus_y_pos - 12;
  }
}

void Samus_HandleTransitionsC_8(void) {  // 0x91F3FD
  Samus_HandleTransitionsC_1();
  Samus_HandleTransitionsC_3();
}

uint8 SamusFunc_F404(void) {  // 0x91F404
  SamusPose v1;

  v1 = samus_pose;
  if (samus_pose != samus_prev_pose) {
    HandleCollDueToChangedPose();
    SamusFunc_F433();
    HandleJumpTransition();
    Samus_SetAnimationFrameIfPoseChanged();
    samus_anim_frame_skip = 0;
  }
  return v1 != samus_pose;
}

void SamusFunc_F433(void) {  // 0x91F433
  *(uint16 *)&samus_pose_x_dir = *(uint16 *)(&kPoseParams[0].pose_x_dir + (uint16)(8 * samus_pose));
  SamusFunc_F468();
  if ((samus_prev_movement_type2 == kMovementType_03_SpinJumping
       || samus_prev_movement_type2 == kMovementType_14_WallJumping)
      && (equipped_items & 8) != 0) {
    Samus_LoadSuitPalette();
  }
}

static Func_U8 *const off_91F4A2[28] = {  // 0x91F468
  SamusFunc_F468_Standing,
  SamusFunc_F468_Running,
  SamusFunc_F468_NormalJump,
  SamusFunc_F468_SpinJump,
  SamusFunc_F468_MorphBall,
  SamusFunc_F468_Crouching,
  SamusFunc_F468_Falling,
  SamusFunc_F468_Unused,
  SamusFunc_F468_MorphBall,
  SamusFunc_F468_Unused,
  SamusFunc_F468_Unused,
  SamusFunc_F468_Unused,
  SamusFunc_F468_Unused,
  SamusFunc_F468_Unused,
  SamusFunc_F468_TurningAroundOnGround,
  SamusFunc_F468_CrouchTransEtc,
  SamusFunc_F468_Moonwalking,
  SamusFunc_F468_Springball,
  SamusFunc_F468_Springball,
  SamusFunc_F468_Springball,
  SamusFunc_F468_WallJumping,
  SamusFunc_F468_Unused,
  SamusFunc_F468_Unused,
  SamusFunc_F468_TurnAroundJumping,
  SamusFunc_F468_TurnAroundFalling,
  SamusFunc_F468_DamageBoost,
  SamusFunc_F468_Unused,
  SamusFunc_F468_Shinespark,
};

void SamusFunc_F468(void) {
  if (off_91F4A2[samus_movement_type]() & 1) {
    *(uint16 *)&samus_pose_x_dir = *(uint16 *)(&kPoseParams[0].pose_x_dir + (uint16)(8 * samus_pose));
    if ((*(uint16 *)&samus_pose_x_dir & 0xFF00) == 3584) {
      off_91F4A2[14]();
      *(uint16 *)&samus_pose_x_dir = *(uint16 *)(&kPoseParams[0].pose_x_dir + (uint16)(8 * samus_pose));
    }
  }
}

uint8 SamusFunc_F468_Unused(void) {  // 0x91F4DA
  return 0;
}

uint8 SamusFunc_F468_Standing(void) {  // 0x91F4DC
  if ((!*(&kPoseParams[0].direction_shots_fired + (uint16)(8 * samus_pose))
       || *(&kPoseParams[0].direction_shots_fired + (uint16)(8 * samus_pose)) == 9)
      && (!*(&kPoseParams[0].direction_shots_fired + (uint16)(8 * samus_prev_pose))
          || *(&kPoseParams[0].direction_shots_fired + (uint16)(8 * samus_prev_pose)) == 9)) {
    samus_anim_frame_skip = 1;
  }
  return 0;
}

uint8 SamusFunc_F468_Running(void) {  // 0x91F50C
  if (samus_prev_movement_type2 == 1)
    samus_anim_frame_skip = FUNC16(Samus_InputHandler);
  if (!UNUSED_word_7E0DF8)
    return 0;
  if (samus_pose == (kPose_44_FaceL_Turn_Crouch | kPose_01_FaceR_Normal)) {
    samus_pose = kPose_25_FaceR_Turn_Stand;
    return 1;
  }
  if (samus_pose == (kPose_44_FaceL_Turn_Crouch | kPose_02_FaceL_Normal)) {
    samus_pose = kPose_26_FaceL_Turn_Stand;
    return 1;
  }
  return 0;
}

uint8 SamusFunc_F468_NormalJump(void) {  // 0x91F543
  if (samus_pose != kPose_4E_FaceL_Jump_NoAim_NoMove_NoGun) {
    if (samus_pose == kPose_4D_FaceR_Jump_NoAim_NoMove_NoGun || samus_pose == kPose_15_FaceR_Jump_AimU) {
LABEL_7:
      if (samus_shine_timer) {
        samus_pose = kPose_C7_FaceR_ShinesparkWindup_Vert;
LABEL_11:
        Projectile_Func7_Shinespark();
        if (samus_prev_movement_type2 == kMovementType_02_NormalJumping)
          samus_prev_y_pos = --samus_y_pos;
        return 1;
      }
      goto LABEL_14;
    }
    if (samus_pose != kPose_16_FaceL_Jump_AimU && samus_pose != kPose_6A_FaceL_Jump_AimUL) {
      if (samus_pose != kPose_69_FaceR_Jump_AimUR)
        goto LABEL_14;
      goto LABEL_7;
    }
  }
  if (samus_shine_timer) {
    samus_pose = kPose_C8_FaceL_ShinesparkWindup_Vert;
    goto LABEL_11;
  }
LABEL_14:
  if (samus_x_extra_run_speed || samus_x_extra_run_subspeed)
    samus_x_accel_mode = 2;
  else
    samus_x_accel_mode = 0;
  if ((samus_pose == kPose_15_FaceR_Jump_AimU || samus_pose == kPose_16_FaceL_Jump_AimU)
      && (samus_prev_pose == kPose_55_FaceR_Jumptrans_AimU || samus_prev_pose == kPose_56_FaceL_Jumptrans_AimU)) {
    samus_anim_frame_skip = 1;
  }
  if ((button_config_shoot_x & joypad1_newkeys) != 0)
    new_projectile_direction_changed_pose = *(&kPoseParams[0].direction_shots_fired + (uint16)(8 * samus_pose)) | 0x8000;
  return 0;
}

uint8 SamusFunc_F468_Crouching(void) {  // 0x91F5EB
  if ((samus_pose == kPose_85_FaceR_Crouch_AimU || samus_pose == kPose_86_FaceL_Crouch_AimU)
      && (samus_prev_pose == kPose_F1_FaceR_CrouchTrans_AimU || samus_prev_pose == kPose_F2_FaceL_CrouchTrans_AimU)) {
    samus_anim_frame_skip = 1;
  }
  return 0;
}

uint8 SamusFunc_F468_Falling(void) {  // 0x91F60D
  if (samus_x_extra_run_speed || samus_x_extra_run_subspeed)
    samus_x_accel_mode = 2;
  else
    samus_x_accel_mode = 0;
  return 0;
}

uint8 SamusFunc_F468_SpinJump(void) {  // 0x91F624
  if (samus_prev_movement_type2 == kMovementType_03_SpinJumping
      || samus_prev_movement_type2 == kMovementType_14_WallJumping) {
    samus_anim_frame_skip = 1;
    if ((samus_prev_pose_x_dir & 0xF) == 8) {
      if (*(uint16 *)&samus_pose_x_dir != 772)
        goto LABEL_9;
    } else if ((samus_prev_pose_x_dir & 0xF) != 4 || *(uint16 *)&samus_pose_x_dir != 776) {
      goto LABEL_9;
    }
    uint16 v0 = (__PAIR32__(samus_x_extra_run_speed, samus_x_base_subspeed)
                 + __PAIR32__(samus_x_base_speed, samus_x_extra_run_subspeed)) >> 16;
    samus_x_base_subspeed += samus_x_extra_run_subspeed;
    samus_x_base_speed = v0;
    Samus_CancelSpeedBoost();
    samus_x_extra_run_subspeed = 0;
    samus_x_extra_run_speed = 0;
    samus_x_accel_mode = 1;
  }
LABEL_9:
  if (samus_pose_x_dir == 4) {
    if ((equipped_items & 0x20) == 0) {
      Samus_GetTopBottomBoundary();
      if ((fx_y_pos & 0x8000u) != 0) {
        if ((lava_acid_y_pos & 0x8000u) == 0 && sign16(lava_acid_y_pos - R20_))
          return 0;
      } else if (sign16(fx_y_pos - R20_) && (fx_liquid_options & 4) == 0) {
        return 0;
      }
    }
    if ((equipped_items & 8) != 0) {
      samus_pose = kPose_82_FaceL_Screwattack;
      goto LABEL_40;
    }
    if ((equipped_items & 0x200) != 0) {
      QueueSfx1_Max6(0x3Eu);
      samus_pose = kPose_1C_FaceL_SpaceJump;
      return 0;
    }
    if (!samus_anim_frame_skip && !cinematic_function) {
LABEL_22:
      QueueSfx1_Max6(0x31u);
      return 0;
    }
    return 0;
  }
  if ((equipped_items & 0x20) == 0) {
    Samus_GetTopBottomBoundary();
    if ((fx_y_pos & 0x8000u) != 0) {
      if ((lava_acid_y_pos & 0x8000u) == 0 && sign16(lava_acid_y_pos - R20_))
        return 0;
    } else if (sign16(fx_y_pos - R20_) && (fx_liquid_options & 4) == 0) {
      return 0;
    }
  }
  if ((equipped_items & 8) != 0) {
    samus_pose = kPose_81_FaceR_Screwattack;
LABEL_40:
    if (!samus_anim_frame_skip)
      QueueSfx1_Max6(0x33u);
    return 0;
  }
  if ((equipped_items & 0x200) == 0) {
    if (samus_anim_frame_skip || cinematic_function)
      return 0;
    goto LABEL_22;
  }
  QueueSfx1_Max6(0x3Eu);
  samus_pose = kPose_1B_FaceR_SpaceJump;
  return 0;
}
static Func_U8 *const off_91F790[12] = {  // 0x91F758
  Samus_CrouchTrans,
  Samus_CrouchTrans,
  Samus_MorphTrans,
  Samus_MorphTrans,
  MaybeUnused_sub_91F7F4,
  MaybeUnused_sub_91F840,
  Samus_StandOrUnmorphTrans,
  Samus_StandOrUnmorphTrans,
  Samus_StandOrUnmorphTrans,
  Samus_StandOrUnmorphTrans,
  Samus_StandOrUnmorphTrans,
  Samus_StandOrUnmorphTrans,
};
static Func_U8 *const off_91F7A8[4] = {
  Samus_MorphTrans,
  Samus_MorphTrans,
  Samus_StandOrUnmorphTrans,
  Samus_StandOrUnmorphTrans,
};
uint8 SamusFunc_F468_CrouchTransEtc(void) {
  uint16 v0;
  if (sign16(samus_pose - kPose_F1_FaceR_CrouchTrans_AimU)) {
    if (sign16(samus_pose - kPose_DB)) {
      v0 = 2 * (samus_pose - 53);
LABEL_4:
      samus_momentum_routine_index = 7;
      return off_91F790[v0 >> 1]();
    }
    return off_91F7A8[samus_pose - 219]();
  } else {
    if (sign16(samus_pose - kPose_F7_FaceR_StandTrans_AimU)) {
      v0 = 0;
      goto LABEL_4;
    }
    samus_momentum_routine_index = 7;
  }
  return 0;
}

uint8 Samus_CrouchTrans(void) {  // 0x91F7B0
  if (!sign16((speed_boost_counter & 0xFF00) - 1024)) {
    samus_shine_timer = 180;
    timer_for_shine_timer = 1;
    special_samus_palette_frame = 0;
  }
  return 0;
}

uint8 Samus_StandOrUnmorphTrans(void) {  // 0x91F7CC
  return 0;
}

uint8 Samus_MorphTrans(void) {  // 0x91F7CE
  if ((equipped_items & 4) != 0) {
    if (samus_prev_movement_type2 == kMovementType_03_SpinJumping)
      samus_x_accel_mode = 2;
    bomb_spread_charge_timeout_counter = 0;
    return 0;
  } else {
    samus_pose = samus_prev_pose;
    return 1;
  }
}

uint8 MaybeUnused_sub_91F7F4(void) {  // 0x91F7F4
  if (samus_prev_movement_type2 == kMovementType_08_MorphBallFalling
      || samus_prev_movement_type2 == kMovementType_13_SpringBallFalling) {
    if ((equipped_items & 2) != 0)
      samus_pose = kPose_7D_FaceR_Springball_Fall;
    else
      samus_pose = kPose_31_FaceR_Morphball_Air;
  } else if ((equipped_items & 2) != 0) {
    samus_pose = kPose_79_FaceR_Springball_Ground;
  } else {
    samus_pose = kPose_1D_FaceR_Morphball_Ground;
  }
  return 1;
}

uint8 MaybeUnused_sub_91F840(void) {  // 0x91F840
  if (samus_prev_movement_type2 == kMovementType_08_MorphBallFalling
      || samus_prev_movement_type2 == kMovementType_13_SpringBallFalling) {
    if ((equipped_items & 2) != 0)
      samus_pose = kPose_7E_FaceL_Springball_Fall;
    else
      samus_pose = kPose_32_FaceL_Morphball_Air;
  } else if ((equipped_items & 2) != 0) {
    samus_pose = kPose_7A_FaceL_Springball_Ground;
  } else {
    samus_pose = kPose_41_FaceL_Morphball_Ground;
  }
  return 1;
}

uint8 SamusFunc_F468_Moonwalking(void) {  // 0x91F88C
  if (moonwalk_flag)
    return 0;
  if (samus_pose_x_dir == 4)
    samus_pose = kPose_25_FaceR_Turn_Stand;
  else
    samus_pose = kPose_26_FaceL_Turn_Stand;
  return 1;
}

uint8 SamusFunc_F468_DamageBoost(void) {  // 0x91F8AE
  return SamusFunc_F468_DamageBoost_();
}

uint8 MaybeUnused_sub_91F8B0(void) {  // 0x91F8B0
  if (samus_pose_x_dir == 4)
    samus_pose = kPose_54_FaceL_Knockback;
  else
    samus_pose = kPose_53_FaceR_Knockback;
  return 1;
}

uint8 SamusFunc_F468_DamageBoost_(void) {  // 0x91F8CB
  samus_movement_handler = FUNC16(Samus_MovementHandler_Normal);
  return 0;
}

#define kSamusTurnPose_Standing ((uint8*)RomPtr(0x91f9c2))
#define kSamusTurnPose_Crouching ((uint8*)RomPtr(0x91f9cc))
#define kSamusTurnPose_Jumping ((uint8*)RomPtr(0x91f9d6))
#define kSamusTurnPose_Falling ((uint8*)RomPtr(0x91f9e0))
#define kSamusTurnPose_Moonwalk ((uint8*)RomPtr(0x91f9ea))
uint8 SamusFunc_F468_TurningAroundOnGround(void) {  // 0x91F8D3
  if (samus_prev_pose && samus_prev_pose != kPose_9B_FaceF_VariaGravitySuit) {
    uint16 v0 = *(&kPoseParams[0].direction_shots_fired + (uint16)(8 * samus_prev_pose));
    if (samus_prev_movement_type2 == kMovementType_10_Moonwalking) {
      new_projectile_direction_changed_pose = *(&kPoseParams[0].direction_shots_fired
                                                + (uint16)(8 * samus_prev_pose)) | 0x100;
      if ((button_config_jump_a & joypad1_lastkeys) != 0) {
        samus_pose = kSamusTurnPose_Moonwalk[v0];
        goto LABEL_9;
      }
    } else if (samus_prev_movement_type2 == 5) {
      samus_pose = kSamusTurnPose_Crouching[*(&kPoseParams[0].direction_shots_fired
                                              + (uint16)(8 * samus_prev_pose))];
      goto LABEL_9;
    }
    samus_pose = kSamusTurnPose_Standing[v0];
  }
LABEL_9:;
  uint16 v1 = (__PAIR32__(samus_x_extra_run_speed, samus_x_base_subspeed)
               + __PAIR32__(samus_x_base_speed, samus_x_extra_run_subspeed)) >> 16;
  samus_x_base_subspeed += samus_x_extra_run_subspeed;
  samus_x_base_speed = v1;
  samus_x_extra_run_subspeed = 0;
  samus_x_extra_run_speed = 0;
  samus_x_accel_mode = 1;
  return 1;
}

uint8 SamusFunc_F468_TurnAroundJumping(void) {  // 0x91F952
  samus_pose = kSamusTurnPose_Jumping[*(&kPoseParams[0].direction_shots_fired + (uint16)(8 * samus_prev_pose))];
  uint16 v0 = (__PAIR32__(samus_x_extra_run_speed, samus_x_base_subspeed)
               + __PAIR32__(samus_x_base_speed, samus_x_extra_run_subspeed)) >> 16;
  samus_x_base_subspeed += samus_x_extra_run_subspeed;
  samus_x_base_speed = v0;
  samus_x_extra_run_subspeed = 0;
  samus_x_extra_run_speed = 0;
  samus_x_accel_mode = 1;
  return 1;
}

uint8 SamusFunc_F468_TurnAroundFalling(void) {  // 0x91F98A
  samus_pose = kSamusTurnPose_Falling[*(&kPoseParams[0].direction_shots_fired + (uint16)(8 * samus_prev_pose))];
  uint16 v0 = (__PAIR32__(samus_x_extra_run_speed, samus_x_base_subspeed)
               + __PAIR32__(samus_x_base_speed, samus_x_extra_run_subspeed)) >> 16;
  samus_x_base_subspeed += samus_x_extra_run_subspeed;
  samus_x_base_speed = v0;
  samus_x_extra_run_subspeed = 0;
  samus_x_extra_run_speed = 0;
  samus_x_accel_mode = 1;
  return 1;
}

uint8 SamusFunc_F468_MorphBall(void) {  // 0x91F9F4
  if (samus_prev_movement_type2 == kMovementType_04_MorphBallOnGround
      || samus_prev_movement_type2 == kMovementType_08_MorphBallFalling) {
    samus_anim_frame_skip = FUNC16(Samus_InputHandler);
  }
  SamusFunc_FA0A();
  return 0;
}

void SamusFunc_FA0A(void) {  // 0x91FA0F
  if (samus_prev_pose_x_dir == 8) {
    if (samus_pose_x_dir != 4)
      return;
    goto LABEL_5;
  }
  if (samus_pose_x_dir == 8) {
LABEL_5:;
    uint16 v0 = (__PAIR32__(samus_x_extra_run_speed, samus_x_base_subspeed)
                 + __PAIR32__(samus_x_base_speed, samus_x_extra_run_subspeed)) >> 16;
    samus_x_base_subspeed += samus_x_extra_run_subspeed;
    samus_x_base_speed = v0;
    Samus_CancelSpeedBoost();
    samus_x_extra_run_subspeed = 0;
    samus_x_extra_run_speed = 0;
    samus_x_accel_mode = 1;
  }
}

uint8 SamusFunc_F468_Springball(void) {  // 0x91FA56
  if (samus_prev_movement_type2 == kMovementType_11_SpringBallOnGround
      || samus_prev_movement_type2 == kMovementType_12_SpringBallInAir
      || samus_prev_movement_type2 == kMovementType_13_SpringBallFalling) {
    samus_anim_frame_skip = FUNC16(Samus_InputHandler);
  }
  SamusFunc_FA0A();
  return 0;
}

uint8 SamusFunc_F468_WallJumping(void) {  // 0x91FA76
  Samus_GetBottomBoundary();
  if ((fx_y_pos & 0x8000u) != 0) {
    if ((lava_acid_y_pos & 0x8000u) == 0 && sign16(lava_acid_y_pos - R18_))
      return 0;
  } else if (sign16(fx_y_pos - R18_) && (fx_liquid_options & 4) == 0) {
    return 0;
  }
  atmospheric_gfx_frame_and_type[3] = 1536;
  atmospheric_gfx_anim_timer[3] = 3;
  atmospheric_gfx_y_pos[3] = R18_;
  if (samus_pose_x_dir != 8) {
    atmospheric_gfx_x_pos[3] = samus_x_pos + 6;
    return 0;
  }
  atmospheric_gfx_x_pos[3] = samus_x_pos - 6;
  return 0;
}

static uint16 kSamusFunc_F468_Shinespark[6] = {  // 0x91FACA
  (uint16)fnSamus_MoveHandler_Shinespark_Horiz,
  (uint16)fnSamus_MoveHandler_Shinespark_Horiz,
  (uint16)fnSamus_MoveHandlerVerticalShinespark,
  (uint16)fnSamus_MoveHandlerVerticalShinespark,
  (uint16)fnSamus_MoveHandler_Shinespark_Diag,
  (uint16)fnSamus_MoveHandler_Shinespark_Diag,
};
uint8 SamusFunc_F468_Shinespark(void) {
  if (sign16(samus_pose - kPose_CF_FaceR_Ranintowall_AimUR)) {
    samus_movement_handler = kSamusFunc_F468_Shinespark[samus_pose - 201];
    samus_input_handler = FUNC16(nullsub_152);
    speed_echoes_index = 0;
    speed_echo_xspeed[0] = 0;
    speed_echo_xspeed[1] = 0;
    speed_echo_xpos[0] = 0;
    speed_echo_xpos[1] = 0;
    QueueSfx3_Max9(0xFu);
  }
  return 0;
}
static const uint16 kSamusPhys_AnimDelayInWater = 3;
static const uint16 kSamusPhys_AnimDelayInAcid = 2;

void Samus_SetAnimationFrameIfPoseChanged(void) {  // 0x91FB08
  if ((equipped_items & 0x20) != 0)
    goto LABEL_7;
  R18_ = samus_y_pos + *(&kPoseParams[0].y_radius + (uint16)(8 * samus_pose)) - 1;
  if ((fx_y_pos & 0x8000u) == 0) {
    if (sign16(fx_y_pos - R18_) && (fx_liquid_options & 4) == 0) {
      R18_ = kSamusPhys_AnimDelayInWater;
      goto LABEL_11;
    }
LABEL_7:
    R18_ = samus_x_speed_divisor;
    goto LABEL_11;
  }
  if ((lava_acid_y_pos & 0x8000u) != 0 || !sign16(lava_acid_y_pos - R18_))
    goto LABEL_7;
  R18_ = kSamusPhys_AnimDelayInAcid;
LABEL_11:
  if ((samus_anim_frame_skip & 0x8000u) == 0 && samus_pose != samus_prev_pose) {
    samus_anim_frame = samus_anim_frame_skip;
    samus_anim_frame_timer = R18_
      + *RomPtr_91(
        kSamusAnimationDelayData[samus_pose]
        + samus_anim_frame_skip);
  }
}

void SamusFunc_EC80(void) {  // 0x91FB8E
  if (samus_prev_movement_type2 != kMovementType_06_Falling && samus_movement_type == kMovementType_06_Falling) {
    samus_y_subspeed = 0;
    samus_y_speed = 0;
    samus_y_dir = 2;
  }
}

void UNUSED_sub_91FC42(void);

void nullsub_24(void) {}
void nullsub_25(void) {}

static Func_V *const kHandleJumpTrans[28] = {  // 0x91FBBB
  nullsub_24,
  nullsub_24,
  HandleJumpTransition_NormalJump,
  HandleJumpTransition_SpinJump,
  nullsub_24,
  nullsub_24,
  nullsub_24,
  nullsub_24,
  nullsub_24,
  nullsub_24,
  nullsub_24,
  nullsub_24,
  nullsub_24,
  UNUSED_sub_91FC42,
  nullsub_24,
  nullsub_24,
  nullsub_24,
  nullsub_24,
  HandleJumpTransition_SpringBallInAir,
  nullsub_24,
  HandleJumpTransition_WallJump,
  nullsub_24,
  nullsub_24,
  nullsub_24,
  nullsub_24,
  nullsub_25,
  nullsub_24,
  nullsub_24,
};

void HandleJumpTransition(void) {
  kHandleJumpTrans[(samus_movement_type)]();
}


void HandleJumpTransition_WallJump(void) {  // 0x91FC08
  if (samus_prev_movement_type2 != kMovementType_14_WallJumping)
    Samus_InitWallJump();
}

void HandleJumpTransition_SpringBallInAir(void) {  // 0x91FC18
  if (samus_pose == kPose_7F_FaceR_Springball_Air) {
    if (samus_prev_movement_type2 != kMovementType_11_SpringBallOnGround)
      return;
LABEL_6:
    Samus_InitJump();
    return;
  }
  if (samus_pose == kPose_80_FaceL_Springball_Air && samus_prev_movement_type2 == kMovementType_11_SpringBallOnGround)
    goto LABEL_6;
}

void UNUSED_sub_91FC42(void) {  // 0x91FC42
  if (samus_pose == (kPose_44_FaceL_Turn_Crouch | kPose_01_FaceR_Normal | 0x20)) {
    if (samus_prev_pose != 100)
      return;
LABEL_6:
    Samus_InitJump();
    return;
  }
  if (samus_pose == (kPose_44_FaceL_Turn_Crouch | kPose_02_FaceL_Normal | 0x20) && samus_prev_pose == 99)
    goto LABEL_6;
}

void HandleJumpTransition_NormalJump(void) {  // 0x91FC66
  if (samus_pose == kPose_4B_FaceR_Jumptrans
      || samus_pose == kPose_4C_FaceL_Jumptrans
      || !sign16(samus_pose - kPose_55_FaceR_Jumptrans_AimU) && sign16(samus_pose - kPose_5B)) {
    if (samus_prev_pose == kPose_27_FaceR_Crouch || samus_prev_pose == kPose_28_FaceL_Crouch)
      samus_y_pos -= 10;
    Samus_InitJump();
  }
}

void HandleJumpTransition_SpinJump(void) {  // 0x91FC99
  if (samus_prev_movement_type2 != kMovementType_03_SpinJumping
      && samus_prev_movement_type2 != kMovementType_14_WallJumping) {
    Samus_InitJump();
  }
}

void Samus_Func20(void) {  // 0x91FCAF
  if (samus_movement_type == 14) {
    if (samus_anim_frame == 2 && samus_anim_frame_timer == 1) {
      if (samus_pose_x_dir == 4) {
        if (samus_pose == kPose_25_FaceR_Turn_Stand)
          samus_pose = kPose_D6_FaceL_Xray_Stand;
        else
          samus_pose = kPose_DA_FaceL_Xray_Crouch;
      } else if (samus_pose == kPose_26_FaceL_Turn_Stand) {
        samus_pose = kPose_D5_FaceR_Xray_Stand;
      } else {
        samus_pose = kPose_D9_FaceR_Xray_Crouch;
      }
      SamusFunc_F433();
      Samus_SetAnimationFrameIfPoseChanged();
      samus_last_different_pose = samus_prev_pose;
      *(uint16 *)&samus_last_different_pose_x_dir = *(uint16 *)&samus_prev_pose_x_dir;
      samus_prev_pose = samus_pose;
      *(uint16 *)&samus_prev_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
    }
  } else {
    if (samus_pose_x_dir == 4) {
      if ((button_config_right & joypad1_lastkeys) == 0)
        return;
      xray_angle = 256 - xray_angle;
      if (samus_movement_type == 5)
        samus_pose = kPose_44_FaceL_Turn_Crouch;
      else
        samus_pose = kPose_26_FaceL_Turn_Stand;
    } else {
      if ((button_config_left & joypad1_lastkeys) == 0)
        return;
      xray_angle = 256 - xray_angle;
      if (samus_movement_type == kMovementType_05_Crouching)
        samus_pose = kPose_43_FaceR_Turn_Crouch;
      else
        samus_pose = kPose_25_FaceR_Turn_Stand;
    }
    SamusFunc_F433();
    Samus_SetAnimationFrameIfPoseChanged();
    samus_last_different_pose = samus_prev_pose;
    *(uint16 *)&samus_last_different_pose_x_dir = *(uint16 *)&samus_prev_pose_x_dir;
    samus_prev_pose = samus_pose;
    *(uint16 *)&samus_prev_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
  }
}

static Func_U8 *const off_91FE8A[4] = {  // 0x91FDAE
  HandleCollDueToChangedPose_Solid_NoColl,
  HandleCollDueToChangedPose_Solid_CollAbove,
  HandleCollDueToChangedPose_Solid_CollBelow,
  HandleCollDueToChangedPose_Solid_CollBoth,
};
static Func_U8 *const off_91FE92[4] = {
  HandleCollDueToChangedPose_Block_NoColl,
  HandleCollDueToChangedPose_Block_CollAbove,
  HandleCollDueToChangedPose_Block_CollBelow,
  HandleCollDueToChangedPose_Block_CollBoth,
};

void HandleCollDueToChangedPose(void) {
  int16 v0;

  if (samus_pose) {
    if (samus_pose != kPose_9B_FaceF_VariaGravitySuit) {
      solid_enemy_collision_flags = 0;
      block_collision_flags = 0;
      R18_ = *(&kPoseParams[0].y_radius + (uint16)(8 * samus_pose));
      v0 = *(&kPoseParams[0].y_radius + (uint16)(8 * samus_prev_pose));
      if (sign16(v0 - R18_)) {
        samus_y_radius = *(&kPoseParams[0].y_radius + (uint16)(8 * samus_prev_pose));
        samus_y_radius_diff = R18_ - v0;
        R18_ -= v0;
        R20_ = 0;
        samus_collision_direction = 2;
        samus_collision_flag = Samus_CheckSolidEnemyColl();
        if (samus_collision_flag)
          solid_enemy_collision_flags = 1;
        samus_space_to_move_up_enemy = R18_;
        R18_ = samus_y_radius_diff;
        R20_ = 0;
        samus_collision_direction = 3;
        samus_collision_flag = Samus_CheckSolidEnemyColl();
        if (samus_collision_flag)
          solid_enemy_collision_flags |= 2u;
        samus_space_to_move_down_enemy = R18_;
        if (off_91FE8A[solid_enemy_collision_flags]() & 1)
          goto LABEL_15;
        R18_ = -samus_y_radius_diff;
        R20_ = 0;
        if (Samus_CollDetectChangedPose() & 1)
          block_collision_flags = 1;
        samus_space_to_move_up_blocks = R18_;
        R18_ = samus_y_radius_diff;
        R20_ = 0;
        if (Samus_CollDetectChangedPose() & 1)
          block_collision_flags |= 2u;
        samus_space_to_move_down_blocks = R18_;
        if (off_91FE92[block_collision_flags]() & 1)
          LABEL_15:
        samus_pose = samus_prev_pose;
      }
    }
  }
}

uint8 HandleCollDueToChangedPose_Solid_NoColl(void) {  // 0x91FE9A
  return 0;
}

uint8 HandleCollDueToChangedPose_Solid_CollBoth(void) {  // 0x91FE9C
  return 0;
}

uint8 HandleCollDueToChangedPose_Solid_CollAbove(void) {  // 0x91FE9E
  R18_ = samus_y_radius_diff - samus_space_to_move_up_enemy;
  R20_ = 0;
  uint16 v1 = samus_y_radius;
  samus_y_radius = *(&kPoseParams[0].y_radius + (uint16)(8 * samus_pose));
  samus_collision_direction = 3;
  samus_collision_flag = Samus_CheckSolidEnemyColl();
  if (samus_collision_flag) {
    samus_y_radius = v1;
    return 1;
  } else {
    samus_space_to_move_up_enemy = R18_;
    samus_y_radius = v1;
    return 0;
  }
}

uint8 HandleCollDueToChangedPose_Solid_CollBelow(void) {  // 0x91FEDF
  R18_ = samus_y_radius_diff - samus_space_to_move_down_enemy;
  R20_ = 0;
  uint16 v1 = samus_y_radius;
  samus_y_radius = *(&kPoseParams[0].y_radius + (uint16)(8 * samus_pose));
  samus_collision_direction = 2;
  samus_collision_flag = Samus_CheckSolidEnemyColl();
  if (samus_collision_flag) {
    samus_y_radius = v1;
    return 1;
  } else {
    samus_space_to_move_down_enemy = R18_;
    samus_y_radius = v1;
    return 0;
  }
}

uint8 HandleCollDueToChangedPose_Block_CollAbove(void) {  // 0x91FF20
  R18_ = samus_y_radius_diff - samus_space_to_move_up_blocks;
  R20_ = 0;
  if (Samus_CollDetectChangedPose() & 1)
    return 1;
  if ((solid_enemy_collision_flags & 2) != 0)
    return HandleCollDueToChangedPose_Block_CollBoth();
  samus_y_pos += R18_;
  samus_prev_y_pos = samus_y_pos;
  return 0;
}

uint8 HandleCollDueToChangedPose_Block_CollBelow(void) {  // 0x91FF49
  R18_ = samus_space_to_move_down_blocks - samus_y_radius_diff;
  R20_ = 0;
  if (Samus_CollDetectChangedPose() & 1)
    return 1;
  if ((solid_enemy_collision_flags & 1) != 0)
    return HandleCollDueToChangedPose_Block_CollBoth();
  samus_y_pos -= R18_;
  samus_prev_y_pos = samus_y_pos;
  return 0;
}

uint8 HandleCollDueToChangedPose_Block_NoColl(void) {  // 0x91FF76
  int16 v0;

  v0 = 2 * solid_enemy_collision_flags;
  if (!(2 * solid_enemy_collision_flags))
    return 0;
  switch (v0) {
  case 2:
    samus_y_pos += samus_space_to_move_up_enemy;
    samus_prev_y_pos = samus_y_pos;
    return 0;
  case 4:
    samus_y_pos -= samus_space_to_move_down_enemy;
    samus_prev_y_pos = samus_y_pos;
    return 0;
  case 6:
    return HandleCollDueToChangedPose_Block_CollBoth();
  default:
    Unreachable();
    while (1)
      ;
  }
}

uint8 HandleCollDueToChangedPose_Block_CollBoth(void) {  // 0x91FFA7
  SamusPose v1;

  if (sign16(samus_y_radius - 8))
    return 1;
  if (samus_pose_x_dir == 4)
    v1 = kPose_28_FaceL_Crouch;
  else
    v1 = kPose_27_FaceR_Crouch;
  samus_pose = v1;
  R18_ = *(&kPoseParams[0].y_radius + (uint16)(8 * v1));
  if (sign16(samus_y_radius - R18_)) {
    R18_ = samus_y_radius - R18_;
    samus_y_pos += R18_;
    samus_prev_y_pos = samus_y_pos;
  }
  return 0;
}
