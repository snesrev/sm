// Samus

#include "ida_types.h"
#include "variables.h"
#include "sm_rtl.h"
#include "funcs.h"


#define kSamusFramesForUnderwaterSfx ((uint8*)RomFixedPtr(0x90a514))
#define kPauseMenuMapData ((uint16*)RomFixedPtr(0x829717))
#define kPauseMenuMapTilemaps ((LongPtr*)RomFixedPtr(0x82964a))
#define kBeamTilePtrs ((uint16*)RomFixedPtr(0x90c3b1))
#define kBeamPalettePtrs ((uint16*)RomFixedPtr(0x90c3c9))
#define off_90B5BB ((uint16*)RomFixedPtr(0x90b5bb))
#define off_90B609 ((uint16*)RomFixedPtr(0x90b609))
#define kFlareAnimDelays ((uint16*)RomFixedPtr(0x90c481))



static const uint16 kUnchargedProjectile_Sfx[12] = { 0xb, 0xd, 0xc, 0xe, 0xf, 0x12, 0x10, 0x11, 0x13, 0x16, 0x14, 0x15 };
static const uint16 kChargedProjectile_Sfx[12] = { 0x17, 0x19, 0x18, 0x1a, 0x1b, 0x1e, 0x1c, 0x1d, 0x1f, 0x22, 0x20, 0x21 };
static const uint16 kNonBeamProjectile_Sfx[9] = { 0, 3, 4, 0, 0, 0, 0, 0, 0 };
static const uint8 kProjectileCooldown_Uncharged[38] = {
  15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 12, 15, 0, 0, 0, 0,
  30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 0, 0, 0, 0,
   0,  0,  0,  0,  0,  0,
};
static const uint8 kNonBeamProjectileCooldowns[9] = { 0, 0xa, 0x14, 0x28, 0, 0x10, 0, 0, 0 };
static const uint8 kBeamAutoFireCooldowns[12] = { 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19 };

void nullsub_12(void) {}

static Func_V *const kSamusFxHandlers[8] = {
  Samus_Animate_NoFx,
  Samus_Animate_LavaFx,
  Samus_Animate_AcidFx,
  Samus_Animate_WaterFx,
  nullsub_12,
  nullsub_12,
  nullsub_12,
  nullsub_12,
};

void Samus_Animate(void) {  // 0x908000
  Samus_GetTopBottomBoundary();
  kSamusFxHandlers[(uint8)(fx_type & 0xF) >> 1]();
  if (samus_pose == kPose_4D_FaceR_Jump_NoAim_NoMove_NoGun || samus_pose == kPose_4E_FaceL_Jump_NoAim_NoMove_NoGun) {
    if (samus_y_dir != 2 && samus_anim_frame == 1 && samus_anim_frame_timer == 1)
      samus_anim_frame_timer = 4;
    bool v1 = (--samus_anim_frame_timer & 0x8000u) != 0;
    if (!samus_anim_frame_timer || v1) {
      ++samus_anim_frame;
      Samus_HandleAnimDelay();
    }
  } else {
    bool v0 = (--samus_anim_frame_timer & 0x8000u) != 0;
    if (!samus_anim_frame_timer || v0) {
      ++samus_anim_frame;
      Samus_HandleAnimDelay();
    }
  }
}

void Samus_Animate_NoFx(void) {  // 0x908078
  samus_anim_frame_buffer = samus_x_speed_divisor;
  if (liquid_physics_type) {
    if ((liquid_physics_type & 1) != 0) {
      liquid_physics_type = 0;
      QueueSfx2_Max6(0xEu);
      if ((samus_suit_palette_index & 4) == 0 && (samus_movement_type == 3 || samus_movement_type == 20))
        QueueSfx1_Max6(0x30u);
      Samus_SpawnWaterSplash();
    } else {
      liquid_physics_type = 0;
    }
  }
}

void Samus_Animate_WaterFx(void) {  // 0x9080B8
  static const uint16 kSamusPhys_AnimDelayInWater = 3;
  if ((fx_y_pos & 0x8000u) == 0 && sign16(fx_y_pos - R18_) && (fx_liquid_options & 4) == 0) {
    samus_anim_frame_buffer = kSamusPhys_AnimDelayInWater;
    if (liquid_physics_type == kLiquidPhysicsType_Water) {
      Samus_SpawnAirBubbles();
    } else {
      liquid_physics_type = kLiquidPhysicsType_Water;
      QueueSfx2_Max6(0xDu);
      Samus_SpawnWaterSplash();
    }
  } else {
    Samus_Animate_NoFx();
  }
}

void Samus_SpawnWaterSplash(void) {  // 0x9080E6
  static const uint8 kWaterSplashTypeTable[28] = {
    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
  };
  if (kWaterSplashTypeTable[samus_movement_type]) {
    atmospheric_gfx_frame_and_type[0] = 256;
    atmospheric_gfx_frame_and_type[1] = 256;
    atmospheric_gfx_anim_timer[0] = 3;
    atmospheric_gfx_anim_timer[1] = 3;
    atmospheric_gfx_x_pos[0] = samus_x_pos + 4;
    atmospheric_gfx_x_pos[1] = samus_x_pos - 3;
    atmospheric_gfx_y_pos[0] = R18_ - 4;
    atmospheric_gfx_y_pos[1] = R18_ - 4;
  } else {
    atmospheric_gfx_frame_and_type[0] = 768;
    atmospheric_gfx_anim_timer[0] = 2;
    atmospheric_gfx_x_pos[0] = samus_x_pos;
    atmospheric_gfx_y_pos[0] = fx_y_pos;
  }
  Samus_SpawnAirBubbles();
}

void Samus_SpawnAirBubbles(void) {  // 0x90813E
  uint16 v0;

  if ((int16)(R20_ - 24 - fx_y_pos) >= 0
      && (nmi_frame_counter_word & 0x7F) == 0
      && !atmospheric_gfx_frame_and_type[2]) {
    atmospheric_gfx_frame_and_type[2] = 1280;
    atmospheric_gfx_anim_timer[2] = 3;
    atmospheric_gfx_x_pos[2] = samus_x_pos;
    atmospheric_gfx_y_pos[2] = samus_y_pos - samus_y_radius + 6;
    if ((NextRandom() & 1) != 0)
      v0 = 15;
    else
      v0 = 17;
    QueueSfx2_Max6(v0);
  }
  if (samus_pose == kPose_00_FaceF_Powersuit
      || samus_pose == kPose_9B_FaceF_VariaGravitySuit
      || (equipped_items & 0x20) != 0) {
    samus_anim_frame_buffer = 0;
  }
}

void Samus_Animate_LavaFx(void) {  // 0x9081C0
  static const uint16 kSamusPhys_LavaDamagePerFrame = 0;
  static const uint16 kSamusPhys_LavaSubdamagePerFrame = 0x8000;


  if ((lava_acid_y_pos & 0x8000u) == 0 && sign16(lava_acid_y_pos - R18_)) {
    if (speed_boost_counter) {
      Samus_CancelSpeedBoost();
      samus_x_extra_run_speed = 0;
      samus_x_extra_run_subspeed = 0;
    }
    if ((equipped_items & 0x20) != 0) {
      samus_anim_frame_buffer = 0;
      liquid_physics_type = 2;
    } else {
      if ((game_time_frames & 7) == 0 && !sign16(samus_health - 71))
        QueueSfx3_Max3(0x2Du);
      uint16 v0 = (__PAIR32__(kSamusPhys_LavaDamagePerFrame, kSamusPhys_LavaSubdamagePerFrame)
            + __PAIR32__(samus_periodic_damage, samus_periodic_subdamage)) >> 16;
      samus_periodic_subdamage += kSamusPhys_LavaSubdamagePerFrame;
      samus_periodic_damage = v0;
      Samus_Animate_SubmergedLavaAcid();
    }
  } else {
    Samus_Animate_NoFx();
  }
}

void Samus_Animate_AcidFx(void) {  // 0x908219
  static const uint16 kSamusPhys_AcidSubdamagePerFrame = 0x8000;
  static const uint16 kSamusPhys_AcidDamagePerFrame = 1;


  if ((lava_acid_y_pos & 0x8000u) == 0 && sign16(lava_acid_y_pos - R18_)) {
    if ((game_time_frames & 7) == 0 && !sign16(samus_health - 71))
      QueueSfx3_Max3(0x2Du);
    uint16 v0 = (__PAIR32__(kSamusPhys_AcidDamagePerFrame, kSamusPhys_AcidSubdamagePerFrame)
          + __PAIR32__(samus_periodic_damage, samus_periodic_subdamage)) >> 16;
    samus_periodic_subdamage += kSamusPhys_AcidSubdamagePerFrame;
    samus_periodic_damage = v0;
    Samus_Animate_SubmergedLavaAcid();
  } else {
    Samus_Animate_NoFx();
  }
}

void Samus_Animate_SubmergedLavaAcid(void) {  // 0x90824C
  static const uint16 kSamusPhys_AnimDelayInAcid = 2;

  samus_anim_frame_buffer = kSamusPhys_AnimDelayInAcid;
  liquid_physics_type = 2;
  if ((int16)(R20_ - lava_acid_y_pos) < 0 && (atmospheric_gfx_frame_and_type[0] & 0x400) == 0) {
    atmospheric_gfx_frame_and_type[0] = 1024;
    atmospheric_gfx_frame_and_type[1] = 1024;
    atmospheric_gfx_frame_and_type[2] = 1024;
    atmospheric_gfx_frame_and_type[3] = 1024;
    atmospheric_gfx_anim_timer[0] = 3;
    atmospheric_gfx_anim_timer[3] = 3;
    atmospheric_gfx_anim_timer[1] = -32766;
    atmospheric_gfx_anim_timer[2] = -32766;
    atmospheric_gfx_y_pos[0] = lava_acid_y_pos;
    atmospheric_gfx_y_pos[1] = lava_acid_y_pos;
    atmospheric_gfx_y_pos[2] = lava_acid_y_pos;
    atmospheric_gfx_y_pos[3] = lava_acid_y_pos;
    atmospheric_gfx_x_pos[0] = samus_x_pos + 6;
    atmospheric_gfx_x_pos[1] = samus_x_pos;
    *(uint32 *)&atmospheric_gfx_x_pos[2] = __PAIR32__(samus_x_pos - (samus_x_pos >= 0xFFFAu), samus_x_pos)
      - ((int)(samus_x_pos >= 0xFFFAu) | 0x60000);
    if ((game_time_frames & 1) == 0)
      QueueSfx2_Max6(0x10u);
  }
  if (samus_pose == kPose_00_FaceF_Powersuit
      || samus_pose == kPose_9B_FaceF_VariaGravitySuit
      || (equipped_items & 0x20) != 0) {
    samus_anim_frame_buffer = 0;
  }
}

static Func_Y_Y *const kAnimDelayFuncs[16] = {  // 0x9082DC
  Samus_AnimDelayFunc_0to5,
  Samus_AnimDelayFunc_0to5,
  Samus_AnimDelayFunc_0to5,
  Samus_AnimDelayFunc_0to5,
  Samus_AnimDelayFunc_0to5,
  Samus_AnimDelayFunc_0to5,
  Samus_AnimDelayFunc_6_GotoStartIfLittleHealth,
  Samus_AnimDelayFunc_7,
  Samus_AnimDelayFunc_8_AutoJumpHack,
  Samus_AnimDelayFunc_9_TransToPose,
  UNUSED_Samus_AnimDelayFunc_10,
  Samus_AnimDelayFunc_11_SelectDelaySequenceWalljump,
  Samus_AnimDelayFunc_12_TransToPose,
  Samus_AnimDelayFunc_13_TransToPose,
  Samus_AnimDelayFunc_14_Goto,
  Samus_AnimDelayFunc_15_GotoStart,
};

void Samus_HandleAnimDelay(void) {
  int16 v2;

  byte_7E0002 = -111;
  uint16 v0 = samus_anim_frame;
  R0_.addr = kSamusAnimationDelayData[samus_pose];
  if ((*IndirPtr(&R0_, samus_anim_frame) & 0x80) != 0) {
    uint16 v1 = Samus_HandleSpeedBoosterAnimDelay(v0);
    if (v1) {
      v2 = kAnimDelayFuncs[v1 & 0xF](v0);
      if (v2 >= 0)
        samus_anim_frame_timer = samus_anim_frame_buffer + *IndirPtr(&R0_, v2);
    }
  } else {
    Samus_HandleNormalAnimDelay(v0);
  }
}

uint16 Samus_AnimDelayFunc_0to5(uint16 j) {  // 0x908344
  return -1;
}

uint16 Samus_AnimDelayFunc_6_GotoStartIfLittleHealth(uint16 j) {  // 0x908346
  if (sign16(samus_health - 30)) {
    return ++samus_anim_frame;
  } else {
    samus_anim_frame = 0;
    return 0;
  }
}

uint16 Samus_AnimDelayFunc_7(uint16 j) {  // 0x908360
  samus_movement_handler = FUNC16(Samus_HandleMovement_DrainedCrouching);
  return ++samus_anim_frame;
}

uint16 Samus_AnimDelayFunc_8_AutoJumpHack(uint16 j) {  // 0x908370
  if (samus_input_handler == FUNC16(Samus_InputHandler_E91D))
    return Samus_AnimDelayFunc_13_TransToPose(j);
  if (samus_new_pose != kPose_4B_FaceR_Jumptrans
      && samus_new_pose != kPose_4C_FaceL_Jumptrans
      && samus_new_pose != kPose_19_FaceR_SpinJump
      && samus_new_pose != kPose_1A_FaceL_SpinJump) {
    samus_input_handler = FUNC16(HandleAutoJumpHack);
    return Samus_AnimDelayFunc_13_TransToPose(j);
  }
  return -1;
}

uint16 Samus_AnimDelayFunc_9_TransToPose(uint16 j) {  // 0x90839A
  uint16 v1 = j + 1;
  R18_ = *(uint16 *)IndirPtr(&R0_, v1);
  if ((R18_ & equipped_items) != 0) {
    if (samus_y_speed || samus_y_subspeed)
      samus_new_pose_transitional = *IndirPtr(&R0_, v1 + 5);
    else
      samus_new_pose_transitional = *IndirPtr(&R0_, v1 + 4);
  } else if (samus_y_speed || samus_y_subspeed) {
    samus_new_pose_transitional = *IndirPtr(&R0_, v1 + 3);
  } else {
    samus_new_pose_transitional = *IndirPtr(&R0_, v1 + 2);
  }
  samus_hurt_switch_index = 3;
  return -1;
}

uint16 UNUSED_Samus_AnimDelayFunc_10(uint16 j) {  // 0x9083F6
  if (samus_y_speed || samus_y_subspeed)
    samus_new_pose_transitional = *IndirPtr(&R0_, j + 2);
  else
    samus_new_pose_transitional = *IndirPtr(&R0_, j + 1);
  samus_hurt_switch_index = 3;
  return -1;
}

uint16 Samus_AnimDelayFunc_11_SelectDelaySequenceWalljump(uint16 j) {  // 0x90841D
  if ((equipped_items & 0x20) == 0) {
    Samus_GetTopBottomBoundary();
    if ((fx_y_pos & 0x8000u) != 0) {
      if ((lava_acid_y_pos & 0x8000u) == 0 && sign16(lava_acid_y_pos - R20_))
        goto LABEL_10;
    } else if (sign16(fx_y_pos - R20_) && (fx_liquid_options & 4) == 0) {
      goto LABEL_10;
    }
  }
  if ((equipped_items & 8) != 0) {
    QueueSfx1_Max6(0x33u);
    samus_anim_frame += 21;
    return samus_anim_frame;
  } else {
    if ((equipped_items & 0x200) == 0) {
LABEL_10:
      QueueSfx1_Max6(0x31u);
      return ++samus_anim_frame;
    }
    QueueSfx1_Max6(0x3Eu);
    samus_anim_frame += 11;
    return samus_anim_frame;
  }
}

uint16 Samus_AnimDelayFunc_12_TransToPose(uint16 j) {  // 0x90848B
  uint16 v1 = j + 1;
  R18_ = *(uint16 *)IndirPtr(&R0_, v1);
  if ((R18_ & equipped_items) != 0)
    samus_new_pose_transitional = *IndirPtr(&R0_, v1 + 3);
  else
    samus_new_pose_transitional = *IndirPtr(&R0_, v1 + 2);
  samus_hurt_switch_index = 3;
  return -1;
}

uint16 Samus_AnimDelayFunc_13_TransToPose(uint16 j) {  // 0x9084B6
  samus_new_pose_transitional = *IndirPtr(&R0_, j + 1);
  samus_hurt_switch_index = 3;
  return -1;
}

uint16 Samus_AnimDelayFunc_14_Goto(uint16 j) {  // 0x9084C7
  R18_ = *IndirPtr(&R0_, j + 1);
  samus_anim_frame -= R18_;
  return samus_anim_frame;
}

uint16 Samus_AnimDelayFunc_15_GotoStart(uint16 j) {  // 0x9084DB
  uint16 result = 0;
  samus_anim_frame = 0;
  return result;
}

void Samus_HandleNormalAnimDelay(uint16 j) {  // 0x9084E3
  byte_7E0002 = -111;
  if (samus_has_momentum_flag && samus_movement_type == 1) {
    uint16 *kDefaultAnimFramePtr = (uint16 *)RomPtr(0x91B5D1);

    if ((equipped_items & 0x2000) != 0)
      R0_.addr = kSpeedBoostToAnimFramePtr[HIBYTE(speed_boost_counter)];
    else
      R0_.addr = *kDefaultAnimFramePtr;
  }
  samus_anim_frame_timer = samus_anim_frame_buffer + *IndirPtr(&R0_, j);
}



uint16 Samus_HandleSpeedBoosterAnimDelay(uint16 j) {  // 0x90852C
  uint16 *kDefaultAnimFramePtr = (uint16 *)RomPtr(0x91B5D1);

  byte_7E0002 = 0x91;
  if (!samus_has_momentum_flag || (button_config_run_b & joypad1_lastkeys) == 0 || samus_movement_type != 1)
    return *IndirPtr(&R0_, j);
  if ((equipped_items & 0x2000) == 0) {
    samus_anim_frame = 0;
    R0_.addr = *kDefaultAnimFramePtr;
    samus_anim_frame_timer = samus_anim_frame_buffer + *IndirPtr(&R0_, 0);
    return 0;
  }
  if ((uint8)--speed_boost_counter)
    return *IndirPtr(&R0_, j);
  uint16 v2 = speed_boost_counter;
  if ((speed_boost_counter & 0x400) == 0) {
    v2 = speed_boost_counter + 256;
    speed_boost_counter = v2;
    if ((v2 & 0x400) != 0) {
      // The original code forgets to preserve A here.
      samus_echoes_sound_flag = 1;
      QueueSfx3_Max6(3u);
      //v2 = 0x103; // bug!
    }
  }
  int v3 = HIBYTE(v2);
  speed_boost_counter = kSpeedBoostToCtr[v3] | speed_boost_counter & 0xFF00;
  samus_anim_frame = 0;
  R0_.addr = kSpeedBoostToAnimFramePtr[v3];
  samus_anim_frame_timer = samus_anim_frame_buffer + *IndirPtr(&R0_, 0);
  return 0;
}

static Func_U8 *const kSamusIsBottomDrawnFuncs[28] = {  // 0x9085E2
  SamusBottomDrawn_0_Standing,
  SamusBottomDrawn_1,
  SamusBottomDrawn_1,
  SamusBottomDrawn_3_SpinJump,
  SamusBottomDrawn_4,
  SamusBottomDrawn_1,
  SamusBottomDrawn_1,
  SamusBottomDrawn_4,
  SamusBottomDrawn_4,
  SamusBottomDrawn_4,
  SamusBottomDrawn_A_Knockback,
  SamusBottomDrawn_1,
  SamusBottomDrawn_1,
  UNUSED_SamusBottomDrawn_D,
  SamusBottomDrawn_1,
  SamusBottomDrawn_F_Transitions,
  SamusBottomDrawn_1,
  SamusBottomDrawn_4,
  SamusBottomDrawn_4,
  SamusBottomDrawn_4,
  SamusBottomDrawn_14_WallJump,
  SamusBottomDrawn_1,
  SamusBottomDrawn_1,
  SamusBottomDrawn_1,
  SamusBottomDrawn_1,
  SamusBottomDrawn_19_DamageBoost,
  SamusBottomDrawn_1,
  SamusBottomDrawn_1B,
};

void Samus_Draw(void) {
  PairU16 v0;

  if (samus_knockback_timer || !samus_invincibility_timer || samus_shine_timer || (nmi_frame_counter_word & 1) == 0) {
    uint16 v2 = 2 * samus_pose;
    samus_top_half_spritemap_index = samus_anim_frame
      + kSamusPoseToBaseSpritemapIndexTop[samus_pose];
    uint16 a = samus_top_half_spritemap_index;
    v0 = Samus_CalcSpritemapPos(2 * samus_pose);
    DrawSamusSpritemap(a, v0.k, v0.j);
    R36 = v2;
    if (kSamusIsBottomDrawnFuncs[samus_movement_type]() & 1) {
      samus_bottom_half_spritemap_index = samus_anim_frame + kSamusPoseToBaseSpritemapIndexBottom[R36 >> 1];
      DrawSamusSpritemap(samus_bottom_half_spritemap_index, samus_spritemap_x_pos, samus_spritemap_y_pos);
    }
  }
  SetSamusTilesDefsForCurAnim();
}

uint8 SamusBottomDrawn_1(void) {  // 0x908686
  return 1;
}

uint8 SamusBottomDrawn_4(void) {  // 0x908688
  samus_bottom_half_spritemap_index = 0;
  return 0;
}

uint8 SamusBottomDrawn_0_Standing(void) {  // 0x90868D
  OamEnt *v1;

  if (samus_pose == kPose_00_FaceF_Powersuit) {
    uint16 v0 = oam_next_ptr;
    v1 = gOamEnt(oam_next_ptr);
    *(uint16 *)&v1->xcoord = samus_x_pos - 7 - layer1_x_pos;
    *(uint16 *)&v1->ycoord = samus_y_pos - 17 - layer1_y_pos;
    *(uint16 *)&v1->charnum = 14369;
    oam_next_ptr = v0 + 4;
  }
  return 1;
}

uint8 SamusBottomDrawn_3_SpinJump(void) {  // 0x9086C6
  if (samus_pose == kPose_81_FaceR_Screwattack
      || samus_pose == kPose_82_FaceL_Screwattack
      || samus_pose == kPose_1B_FaceR_SpaceJump
      || samus_pose == kPose_1C_FaceL_SpaceJump
      || !samus_anim_frame
      || !sign16(samus_anim_frame - kPose_0B_MoveR_Gun)) {
    return 1;
  }
  samus_bottom_half_spritemap_index = 0;
  return 0;
}

uint8 SamusBottomDrawn_A_Knockback(void) {  // 0x9086EE
  if (samus_pose != kPose_D7_FaceR_CrystalFlashEnd && samus_pose != kPose_D8_FaceL_CrystalFlashEnd
      || !sign16(samus_anim_frame - 3)) {
    return 1;
  }
  samus_bottom_half_spritemap_index = 0;
  return 0;
}

uint8 SamusBottomDrawn_F_Transitions(void) {  // 0x90870C
  if (!sign16(samus_pose - kPose_F1_FaceR_CrouchTrans_AimU))
    return 1;
  if (!sign16(samus_pose - 219)) {
    if (sign16(samus_pose - 221)) {
      if (samus_anim_frame)
        goto LABEL_8;
    } else if (samus_anim_frame != 2) {
      goto LABEL_8;
    }
    return 1;
  }
  if (samus_pose == kPose_35_FaceR_CrouchTrans
      || samus_pose == kPose_36_FaceL_CrouchTrans
      || samus_pose == kPose_3B_FaceR_StandTrans
      || samus_pose == kPose_3C_FaceL_StandTrans) {
    return 1;
  }
LABEL_8:
  samus_bottom_half_spritemap_index = 0;
  return 0;
}

uint8 UNUSED_SamusBottomDrawn_D(void) {  // 0x90874C
  if (samus_pose != (kPose_44_FaceL_Turn_Crouch | kPose_01_FaceR_Normal | 0x20)
      && samus_pose != (kPose_44_FaceL_Turn_Crouch | kPose_02_FaceL_Normal | 0x20)
      || sign16(samus_anim_frame - 1)) {
    return 1;
  }
  samus_bottom_half_spritemap_index = 0;
  return 0;
}

uint8 SamusBottomDrawn_14_WallJump(void) {  // 0x908768
  if (sign16(samus_anim_frame - 3) || !sign16(samus_anim_frame - 13))
    return 1;
  samus_bottom_half_spritemap_index = 0;
  return 0;
}

uint8 SamusBottomDrawn_19_DamageBoost(void) {  // 0x90877C
  if (sign16(samus_anim_frame - 2) || !sign16(samus_anim_frame - 9))
    return 1;
  samus_bottom_half_spritemap_index = 0;
  return 0;
}

uint8 SamusBottomDrawn_1B(void) {  // 0x908790
  if (sign16(samus_pose - kPose_CF_FaceR_Ranintowall_AimUR)) {
    if (samus_pose != kPose_CB_FaceR_Shinespark_Vert && samus_pose != kPose_CC_FaceL_Shinespark_Vert)
      return 1;
  } else if (samus_pose != kPose_E8_FaceR_Drained_CrouchFalling && samus_pose != kPose_E9_FaceL_Drained_CrouchFalling
             || !sign16(samus_anim_frame - 2)) {
    return 1;
  }
  samus_bottom_half_spritemap_index = 0;
  return 0;
}

void Samus_DrawEchoes(void) {  // 0x9087BD
  if ((speed_echoes_index & 0x8000u) == 0) {
    if ((speed_boost_counter & 0xFF00) == 1024) {
      if (speed_echo_xpos[1])
        Samus_DrawEcho(2u);
      if (speed_echo_xpos[0])
        Samus_DrawEcho(0);
    }
    return;
  }
  for (int i = 2; i >= 0; i -= 2) {
    int v1 = i >> 1;
    if (!speed_echo_xpos[v1])
      continue;
    uint16 v2 = speed_echo_ypos[v1], v3;
    if (v2 != samus_y_pos) {
      if ((int16)(v2 - samus_y_pos) < 0)
        v3 = v2 + 2;
      else
        v3 = v2 - 2;
      speed_echo_ypos[v1] = v3;
    }
    if ((speed_echo_xspeed[v1] & 0x8000u) != 0) {
      uint16 v5 = speed_echo_xspeed[v1] + speed_echo_xpos[v1];
      speed_echo_xpos[v1] = v5;
      if ((int16)(v5 - samus_x_pos) < 0) {
        speed_echo_xpos[v1] = 0;
        continue;
      }
    } else {
      uint16 v4 = speed_echo_xspeed[v1] + speed_echo_xpos[v1];
      speed_echo_xpos[v1] = v4;
      if ((int16)(v4 - samus_x_pos) >= 0) {
        speed_echo_xpos[v1] = 0;
        continue;
      }
    }
    Samus_DrawEcho(i);
  }
  if (!speed_echo_xpos[1] && !speed_echo_xpos[0])
    speed_echoes_index = 0;
}

void Samus_DrawEcho(uint16 j) {  // 0x908855
  int16 v2;

  R18_ = *(&kPoseParams[0].y_offset_to_gfx + (uint16)(8 * samus_pose));
  int v1 = j >> 1;
  v2 = (__PAIR32__(speed_echo_ypos[v1] - R18_, speed_echo_ypos[v1]) - __PAIR32__(layer1_y_pos, R18_)) >> 16;
  if (v2 >= 0 && sign16(v2 - 248)) {
    DrawSamusSpritemap(samus_top_half_spritemap_index, speed_echo_xpos[v1] - layer1_x_pos, v2);
    if (samus_bottom_half_spritemap_index) {
      R18_ = *(&kPoseParams[0].y_offset_to_gfx + (uint16)(8 * samus_pose));
      DrawSamusSpritemap(
        samus_bottom_half_spritemap_index,
        speed_echo_xpos[j >> 1] - layer1_x_pos,
        (__PAIR32__(speed_echo_ypos[j >> 1] - R18_, speed_echo_ypos[j >> 1]) - __PAIR32__(layer1_y_pos, R18_)) >> 16);
    }
  }
}

void Samus_DrawShinesparkCrashEchoes(uint16 k) {  // 0x9088BA
  int16 v2;

  if ((nmi_frame_counter_word & 1) != 0) {
    uint16 v4 = 2 * samus_pose;
    uint16 a = samus_anim_frame + kSamusPoseToBaseSpritemapIndexTop[samus_pose];
    R18_ = *(&kPoseParams[0].y_offset_to_gfx + (uint16)(8 * samus_pose));
    int v1 = k >> 1;
    v2 = speed_echo_ypos[v1] - R18_ - layer1_y_pos;
    if (v2 >= 0) {
      if (sign16(v2 - 248)) {
        DrawSamusSpritemap(a, speed_echo_xpos[v1] - layer1_x_pos, v2);
        R36 = v4;
        if (kSamusIsBottomDrawnFuncs[samus_movement_type]() & 1) {
          uint16 v5 = samus_anim_frame + kSamusPoseToBaseSpritemapIndexBottom[R36 >> 1];
          R18_ = *(&kPoseParams[0].y_offset_to_gfx + (uint16)(4 * R36));
          DrawSamusSpritemap(v5, speed_echo_xpos[k >> 1] - layer1_x_pos, speed_echo_ypos[k >> 1] - R18_ - layer1_y_pos);
        }
      }
    }
  }
}

void Samus_DrawShinesparkCrashEchoProjectiles(void) {  // 0x908953
  if ((nmi_frame_counter_word & 1) != 0) {
    if (speed_echo_xspeed[3])
      Samus_DrawEcho(6u);
    if (speed_echo_xspeed[2])
      Samus_DrawEcho(4u);
  }
}

void Samus_DrawStartingDeathAnim(void) {  // 0x908976
  bool v0 = (--samus_anim_frame_timer & 0x8000u) != 0;
  if (!samus_anim_frame_timer || v0) {
    ++samus_anim_frame;
    Samus_HandleAnimDelay();
  }
  Samus_DrawDuringDeathAnim();
}

void Samus_DrawDuringDeathAnim(void) {  // 0x908998
  uint16 v1 = 2 * samus_pose;
  uint16 a = samus_anim_frame + kSamusPoseToBaseSpritemapIndexTop[samus_pose];
  Samus_CalcSpritemapPos(2 * samus_pose);
  DrawSamusSpritemap(a, layer1_x_pos + samus_spritemap_x_pos, layer1_y_pos + samus_spritemap_y_pos);
  R36 = v1;
  if (kSamusIsBottomDrawnFuncs[samus_movement_type]() & 1)
    DrawSamusSpritemap(
      samus_anim_frame + kSamusPoseToBaseSpritemapIndexBottom[R36 >> 1],
      layer1_x_pos + samus_spritemap_x_pos,
      layer1_y_pos + samus_spritemap_y_pos);
  SetSamusTilesDefsForCurAnim();
}

void Samus_DrawWhenNotAnimatingOrDying(void) {  // 0x908A00
  PairU16 v0;

  uint16 v2 = 2 * samus_pose;
  uint16 a = samus_anim_frame + kSamusPoseToBaseSpritemapIndexTop[samus_pose];
  v0 = Samus_CalcSpritemapPos(2 * samus_pose);
  DrawSamusSpritemap(a, v0.k, v0.j);
  R36 = v2;
  if (kSamusIsBottomDrawnFuncs[samus_movement_type]() & 1)
    DrawSamusSpritemap(
      samus_anim_frame + kSamusPoseToBaseSpritemapIndexBottom[R36 >> 1],
      samus_spritemap_x_pos,
      samus_spritemap_y_pos);
  SetSamusTilesDefsForCurAnim();
}

static FuncXY_V *const kAtmosphericTypeFuncs[8] = {  // 0x908A4C
  0,
  AtmosphericTypeFunc_1_FootstepSplash,
  AtmosphericTypeFunc_1_FootstepSplash,
  AtmosphericTypeFunc_3_DivingSplash,
  AtmosphericTypeFunc_4_LavaSurfaceDmg,
  AtmosphericTypeFunc_5_Bubbles,
  AtmosphericTypeFunc_67_Dust,
  AtmosphericTypeFunc_67_Dust,
};

void HandleAtmosphericEffects(void) {
  int16 v4;

  for (int i = 6; i >= 0; i -= 2) {
    int v1 = i >> 1;
    uint16 v2 = atmospheric_gfx_frame_and_type[v1];
    if (!v2)
      continue;
    R18_ = 2 * (uint8)v2;
    uint16 v3 = 2 * HIBYTE(v2);
    v4 = atmospheric_gfx_anim_timer[v1] - 1;
    atmospheric_gfx_anim_timer[v1] = v4;
    if (v4) {
      if (v4 < 0) {
        if (v4 != (int16)0x8000)
          continue;
        atmospheric_gfx_anim_timer[v1] = *(uint16 *)RomPtr_90(R18_ + kAtmosphericGraphicAnimationTimers[v3 >> 1]);
      }
    } else {
      atmospheric_gfx_anim_timer[v1] = *(uint16 *)RomPtr_90(R18_ + kAtmosphericGraphicAnimationTimers[v3 >> 1]);
      uint16 v5 = atmospheric_gfx_frame_and_type[v1] + 1;
      atmospheric_gfx_frame_and_type[v1] = v5;
      if ((int16)((uint8)v5 - kAtmosphericTypeNumFrames[v3 >> 1]) >= 0) {
        atmospheric_gfx_frame_and_type[v1] = 0;
        continue;
      }
    }
    kAtmosphericTypeFuncs[v3 >> 1](v3, i);
  }
}

void AtmosphericTypeFunc_1_FootstepSplash(uint16 k, uint16 j) {  // 0x908AC5
  int16 v4;
  OamEnt *v5;
  int16 v6;

  int v2 = j >> 1;
  R18_ = 2 * LOBYTE(atmospheric_gfx_frame_and_type[v2]);
  uint16 v3 = oam_next_ptr;
  v4 = atmospheric_gfx_x_pos[v2] - layer1_x_pos - 4;
  if (v4 >= 0) {
    if (sign16(atmospheric_gfx_x_pos[v2] - layer1_x_pos - 260)) {
      v5 = gOamEnt(oam_next_ptr);
      *(uint16 *)&v5->xcoord = v4;
      v6 = atmospheric_gfx_y_pos[v2] - layer1_y_pos - 4;
      if (v6 >= 0) {
        if (sign16(atmospheric_gfx_y_pos[v2] - layer1_y_pos - 260)) {
          *(uint16 *)&v5->ycoord = v6;
          *(uint16 *)&v5->charnum = *(uint16 *)RomPtr_90(R18_ + g_off_908BFF[k >> 1]);
          oam_next_ptr = v3 + 4;
        }
      }
    }
  }
}

void AtmosphericTypeFunc_Common(uint16 j, uint16 a) {  // 0x908B74
  int v1 = j >> 1;
  if (((atmospheric_gfx_y_pos[v1] - layer1_y_pos) & 0xFF00) == 0)
    DrawSamusSpritemap(a, atmospheric_gfx_x_pos[v1] - layer1_x_pos, atmospheric_gfx_y_pos[v1] - layer1_y_pos);
}

void AtmosphericTypeFunc_3_DivingSplash(uint16 k, uint16 j) {  // 0x908B16
  int v2 = j >> 1;
  R18_ = LOBYTE(atmospheric_gfx_frame_and_type[v2]);
  atmospheric_gfx_y_pos[v2] = fx_y_pos;
  AtmosphericTypeFunc_Common(j, R18_ + 399);
}

void AtmosphericTypeFunc_4_LavaSurfaceDmg(uint16 k, uint16 j) {  // 0x908B2E
  int v2 = j >> 1;
  uint16 v3;
  if ((j & 4) != 0)
    v3 = atmospheric_gfx_x_pos[v2] - 1;
  else
    v3 = atmospheric_gfx_x_pos[v2] + 1;
  atmospheric_gfx_x_pos[v2] = v3;
  --atmospheric_gfx_y_pos[j >> 1];
  AtmosphericTypeFunc_1_FootstepSplash(k, j);
}

void AtmosphericTypeFunc_67_Dust(uint16 k, uint16 j) {  // 0x908B57
  --atmospheric_gfx_y_pos[j >> 1];
  AtmosphericTypeFunc_1_FootstepSplash(k, j);
}

void AtmosphericTypeFunc_5_Bubbles(uint16 k, uint16 j) {  // 0x908B64
  R18_ = LOBYTE(atmospheric_gfx_frame_and_type[j >> 1]);
  AtmosphericTypeFunc_Common(j, R18_ + 390);
}

static Func_Y_To_PairU16 *const kSamus_CalcSpritemapPos[28] = {  // 0x908C1F
  &Samus_CalcSpritemapPos_Standing,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Crouch,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Default,
  &Samus_CalcSpritemapPos_Special,
};

PairU16 Samus_CalcSpritemapPos(uint16 k) {
  PairU16 v1;

  if ((ceres_status & 0x8000u) == 0) {
    v1 = kSamus_CalcSpritemapPos[samus_movement_type](k);
  } else {
    grapple_beam_tmpD82 = samus_x_pos;
    grapple_beam_tmpD84 = samus_y_pos;
    Samus_CalcPos_Mode7();
    v1 = kSamus_CalcSpritemapPos[samus_movement_type](k);
    samus_y_pos = grapple_beam_tmpD84;
    samus_x_pos = grapple_beam_tmpD82;
  }
  return MakePairU16(v1.k, v1.j);
}

PairU16 Samus_CalcSpritemapPos_Default(uint16 j) {  // 0x908C94
  int v1 = (int8)*(&kPoseParams[0].y_offset_to_gfx + 4 * j);
  R18_ = v1;
  samus_spritemap_y_pos = samus_y_pos - v1 - layer1_y_pos;
  samus_spritemap_x_pos = samus_x_pos - layer1_x_pos;
  return MakePairU16(samus_spritemap_x_pos, samus_spritemap_y_pos);
}

PairU16 Samus_CalcSpritemapPos_Standing(uint16 j) {  // 0x908CC3
  static const uint8 g_byte_908D28[16] = {
    3, 6, 0, 0,
    3, 6, 0, 0,
    3, 3, 6, 0,
    3, 3, 6, 0,
  };

  int16 v1;
  PairU16 v3;

  v1 = j >> 1;
  if (!(j >> 1) || v1 == kPose_9B_FaceF_VariaGravitySuit) {
    if (!sign16(samus_anim_frame - 2)) {
      samus_spritemap_y_pos = samus_y_pos - 1 - layer1_y_pos;
      samus_spritemap_x_pos = samus_x_pos - layer1_x_pos;
      return MakePairU16(samus_x_pos - layer1_x_pos, samus_y_pos - 1 - layer1_y_pos);
    }
  } else if (!sign16(v1 - kPose_A4_FaceR_LandJump) && sign16(v1 - kPose_A8_FaceR_Grappling)) {
    R18_ = *(uint16 *)&g_byte_908D28[(uint16)(samus_anim_frame + 4 * (v1 - 164))];
    samus_spritemap_y_pos = samus_y_pos - R18_ - layer1_y_pos;
    samus_spritemap_x_pos = samus_x_pos - layer1_x_pos;
    return MakePairU16(samus_x_pos - layer1_x_pos, samus_y_pos - R18_ - layer1_y_pos);
  }
  v3 = Samus_CalcSpritemapPos_Default(j);
  return MakePairU16(v3.k, v3.j);
}

PairU16 Samus_CalcSpritemapPos_Crouch(uint16 j) {  // 0x908D3C
  static const int8 byte_908D80[24] = {
    -8,  0, -8,  0,
    -4, -2, -4, -2,
     0,  0,  0,  0,
    -4,  0, -4,  0,
     5,  4,  5,  4,
     0,  0,  0,  0,
  };

  int16 v1;

  v1 = j >> 1;
  if (sign16((j >> 1) - kPose_35_FaceR_CrouchTrans) || !sign16(v1 - kPose_41_FaceL_Morphball_Ground)) {
    return Samus_CalcSpritemapPos_Default(j);
  } else {
    int v4 = byte_908D80[(uint16)(samus_anim_frame + 2 * (v1 - 53))];
    R18_ = v4;
    samus_spritemap_y_pos = v4 + samus_y_pos - layer1_y_pos;
    samus_spritemap_x_pos = samus_x_pos - layer1_x_pos;
    return MakePairU16(samus_x_pos - layer1_x_pos, samus_spritemap_y_pos);
  }
}

PairU16 Samus_CalcSpritemapPos_Special(uint16 j) {  // 0x908D98
  static const int8 byte_908DEF[32] = {
   7, 5, -8, -8, -8, -8, -8, -5,
   4, 4,  4,  4,  0,  0,  4, -3,
  -5, 0,  0,  4, -3, -5, -3,  4,
   0, 0,  4,  0,  0,  4,  0,  0,
  };
  int v1, v2;

  v1 = j >> 1;
  if (v1 == kPose_E8_FaceR_Drained_CrouchFalling || v1 == kPose_E9_FaceL_Drained_CrouchFalling) {
    v2 = byte_908DEF[samus_anim_frame];
  } else {
    if (v1 != kPose_EA_FaceR_Drained_Stand && v1 != kPose_EB_FaceL_Drained_Stand || (int16)(samus_anim_frame - 5) < 0) {
      return Samus_CalcSpritemapPos_Default(j);
    }
    v2 = -3;
  }
  R18_ = v2;
  samus_spritemap_y_pos = v2 + samus_y_pos - layer1_y_pos;
  samus_spritemap_x_pos = samus_x_pos - layer1_x_pos;
  return MakePairU16(samus_x_pos - layer1_x_pos, v2 + samus_y_pos - layer1_y_pos);
}

static Func_V *const kLiquidPhysicsType[8] = {  // 0x908E0F
  LiquidPhysicsType_0,
  LiquidPhysicsType_24_LavaAcid,
  LiquidPhysicsType_24_LavaAcid,
  LiquidPhysicsType_6_Water,
  LiquidPhysicsType_0,
  LiquidPhysicsType_0,
  LiquidPhysicsType_0,
  LiquidPhysicsType_0,
};

void SetLiquidPhysicsType(void) {
  Samus_GetTopBottomBoundary();
  kLiquidPhysicsType[(uint8)(fx_type & 0xF) >> 1]();
}

void LiquidPhysicsType_0(void) {  // 0x908E36
  liquid_physics_type = 0;
}

void LiquidPhysicsType_24_LavaAcid(void) {  // 0x908E3A
  if ((lava_acid_y_pos & 0x8000u) == 0 && sign16(lava_acid_y_pos - R18_))
    liquid_physics_type = kLiquidPhysicsType_LavaAcid;
  else
    LiquidPhysicsType_0();
}

void LiquidPhysicsType_6_Water(void) {  // 0x908E4A
  if ((fx_y_pos & 0x8000u) == 0 && sign16(fx_y_pos - R18_) && (fx_liquid_options & 4) == 0)
    liquid_physics_type = kLiquidPhysicsType_Water;
  else
    LiquidPhysicsType_0();
}

void Samus_HandleMovement_X(void) {  // 0x908E64
  Samus_HandleExtraRunspeedX();
  Samus_CalcBaseSpeed_X(Samus_DetermineSpeedTableEntryPtr_X());
  Samus_MoveX();
}

void Samus_MoveX(void) {  // 0x908EA9
  if (!samus_x_accel_mode || samus_x_accel_mode == 2) {
    if (samus_pose_x_dir != 4) {
LABEL_6:
      Samus_CalcDisplacementMoveRight();
      goto LABEL_8;
    }
  } else if (samus_pose_x_dir != 8) {
    goto LABEL_6;
  }
  Samus_CalcDisplacementMoveLeft();
LABEL_8:
  if ((R18_ & 0x8000u) != 0)
    Samus_MoveLeft();
  else
    Samus_MoveRight();
}

void Samus_BombJumpRisingXMovement_(void) {
  if (knockback_dir) {
    Samus_CalcBaseSpeed_X(Samus_DetermineSpeedTableEntryPtr_X());
    if (!knockback_x_dir)
      goto LABEL_6;
LABEL_5:
    Samus_CalcDisplacementMoveRight();
    goto LABEL_7;
  }
  Samus_CalcBaseSpeed_X(addr_stru_909F25);
  if ((uint8)bomb_jump_dir != 1)
    goto LABEL_5;
LABEL_6:
  Samus_CalcDisplacementMoveLeft();
LABEL_7:
  if ((R18_ & 0x8000u) != 0)
    Samus_MoveLeft();
  else
    Samus_MoveRight();
}

void Samus_BombJumpRisingYMovement_(void) {
  if (samus_y_dir == 1) {
    if ((samus_y_speed & 0x8000u) != 0) {
      samus_y_subspeed = 0;
      samus_y_speed = 0;
      samus_y_dir = 2;
      if ((uint8)bomb_jump_dir != 2)
        samus_x_accel_mode = 2;
    } else if (sign16(samus_y_speed - 1)) {
      if (samus_input_handler != FUNC16(Samus_InputHandler_E91D))
        samus_input_handler = FUNC16(Samus_InputHandler_E913);
    }
  }
}

void Samus_BombJumpFallingXMovement_(void) {
  Samus_CalcBaseSpeed_X(Samus_DetermineSpeedTableEntryPtr_X());
  if (samus_var62 == 1)
    Samus_CalcDisplacementMoveLeft();
  else
    Samus_CalcDisplacementMoveRight();
  if ((R18_ & 0x8000u) != 0)
    Samus_MoveLeft();
  else
    Samus_MoveRight();
}

void Samus_BombJumpFallingYMovement_(void) {
  if (sign16(samus_y_speed - 5)) {
    uint16 v0 = (__PAIR32__(samus_y_accel, samus_y_subaccel) + __PAIR32__(samus_y_speed, samus_y_subspeed)) >> 16;
    samus_y_subspeed += samus_y_subaccel;
    samus_y_speed = v0;
  }
  R20_ = samus_y_subspeed;
  R18_ = samus_y_speed;
  Samus_MoveDown();
}

void Samus_JumpingMovement(void) {  // 0x908FB3
  Samus_HandleExtraRunspeedX();
  if (samus_pose == kPose_4B_FaceR_Jumptrans
      || samus_pose == kPose_4C_FaceL_Jumptrans
      || !sign16(samus_pose - kPose_55_FaceR_Jumptrans_AimU) && sign16(samus_pose - kPose_5B)) {
    samus_x_accel_mode = 0;
    Samus_Move_NoBaseSpeed_X();
    Samus_MoveExtraY();
    return;
  }
  if (samus_y_dir == 1 && ((button_config_jump_a & joypad1_lastkeys) == 0 || (samus_y_speed & 0x8000u) != 0)) {
    samus_y_subspeed = 0;
    samus_y_speed = 0;
    samus_y_dir = 2;
  }
  Samus_CalcBaseSpeed_NoDecel_X(Samus_DetermineSpeedTableEntryPtr_X());
  if (samus_movement_type == 20) {
    if (!samus_x_accel_mode)
      samus_x_accel_mode = 2;
    goto LABEL_17;
  }
  if (samus_x_accel_mode || (joypad1_lastkeys & 0x100) != 0 || (joypad1_lastkeys & 0x200) != 0) {
LABEL_17:
    Samus_MoveX();
    goto LABEL_18;
  }
  R18_ = 0;
  R20_ = 0;
  samus_x_base_speed = 0;
  samus_x_base_subspeed = 0;
  samus_collision_flag = 0;
LABEL_18:
  Samus_MoveY_WithSpeedCalc();
}

void Samus_SpinJumpMovement(void) {  // 0x909040
  static const uint16 g_word_909E9F = 8;

  Samus_HandleExtraRunspeedX();
  if (samus_y_dir == 1 && ((button_config_jump_a & joypad1_lastkeys) == 0 || (samus_y_speed & 0x8000u) != 0)) {
    samus_y_subspeed = 0;
    samus_y_speed = 0;
    samus_y_dir = 2;
  }
  if (Samus_CalcBaseSpeed_NoDecel_X(Samus_DetermineSpeedTableEntryPtr_X()) & 1 || samus_x_accel_mode == 1)
    goto LABEL_12;
  if (samus_pose_x_dir == 4) {
    if ((joypad1_lastkeys & kButton_Left) != 0)
      goto LABEL_12;
LABEL_11:
    R18_ = 0;
    R20_ = 0;
    samus_x_base_speed = 0;
    samus_x_base_subspeed = 0;
    samus_collision_flag = 0;
    samus_x_accel_mode = 0;
    goto LABEL_14;
  }
  if ((joypad1_lastkeys & kButton_Right) == 0)
    goto LABEL_11;
LABEL_12:
  if (!samus_x_accel_mode)
    samus_x_accel_mode = 2;
LABEL_14:
  Samus_MoveX();
  R18_ = g_word_909E9F;
  R20_ = 0;
  if (!(Samus_WallJumpCheck() & 1))
    Samus_MoveY_WithSpeedCalc();
}

void Samus_CheckStartFalling(void) {  // 0x9090C4
  if (samus_y_dir == 1 && (samus_y_speed & 0x8000u) != 0) {
    samus_y_subspeed = 0;
    samus_y_speed = 0;
    samus_y_dir = 2;
  }
}

void Samus_MoveY_WithSpeedCalc(void) {  // 0x9090E2
  int16 v3;

  R20_ = samus_y_subspeed;
  R18_ = samus_y_speed;
  if (samus_y_dir == 2) {
    if (samus_y_speed != 5) {
      uint16 v1 = (__PAIR32__(samus_y_accel, samus_y_subaccel) + __PAIR32__(samus_y_speed, samus_y_subspeed)) >> 16;
      samus_y_subspeed += samus_y_subaccel;
      samus_y_speed = v1;
    }
  } else {
    uint16 v0 = (__PAIR32__(samus_y_speed, samus_y_subspeed) - __PAIR32__(samus_y_accel, samus_y_subaccel)) >> 16;
    samus_y_subspeed -= samus_y_subaccel;
    samus_y_speed = v0;
  }
  if (samus_y_dir != 2) {
    R18_ = ~R18_;
    bool v2 = R20_ == 0;
    R20_ = -R20_;
    if (v2)
      ++R18_;
  }
  v3 = (__PAIR32__(R18_, R20_) + __PAIR32__(extra_samus_y_displacement, extra_samus_y_subdisplacement)) >> 16;
  R20_ += extra_samus_y_subdisplacement;
  R18_ = v3;
  if (v3 < 0)
    Samus_MoveUp();
  else
    Samus_MoveDown();
}

void Samus_FallingMovement(void) {  // 0x909168
  Samus_HandleExtraRunspeedX();
  Samus_CalcBaseSpeed_NoDecel_X(Samus_DetermineSpeedTableEntryPtr_X());
  if (samus_x_accel_mode || (joypad1_lastkeys & kButton_Right) != 0 || (joypad1_lastkeys & kButton_Left) != 0) {
    Samus_MoveX();
  } else {
    R18_ = 0;
    R20_ = 0;
    samus_x_base_speed = 0;
    samus_x_base_subspeed = 0;
    samus_collision_flag = 0;
  }
  Samus_CheckStartFalling();
  Samus_MoveY_WithSpeedCalc();
}

void Samus_MorphedFallingMovement(void) {  // 0x90919F
  Samus_CalcBaseSpeed_NoDecel_X(Samus_DetermineSpeedTableEntryPtr_X());
  if (!samus_x_accel_mode && (joypad1_lastkeys & kButton_Right) == 0 && (joypad1_lastkeys & kButton_Left) == 0) {
    R18_ = 0;
    R20_ = 0;
    samus_x_base_speed = 0;
    samus_x_base_subspeed = 0;
    samus_collision_flag = 0;
  }
  Samus_MoveX();
  Samus_CheckStartFalling();
  Samus_MoveY_WithSpeedCalc();
}

void Samus_MorphedBouncingMovement(void) {  // 0x9091D1
  Samus_CalcBaseSpeed_NoDecel_X(Samus_DetermineSpeedTableEntryPtr_X());
  if (!samus_x_accel_mode && (joypad1_lastkeys & kButton_Right) == 0 && (joypad1_lastkeys & kButton_Left) == 0) {
    R18_ = 0;
    R20_ = 0;
    samus_x_base_speed = 0;
    samus_x_base_subspeed = 0;
    samus_collision_flag = 0;
  }
  Samus_MoveX();
  if (!knockback_dir) {
    if (extra_samus_y_displacement || extra_samus_y_subdisplacement) {
      samus_y_dir = 2;
      samus_y_subspeed = 0;
      samus_y_speed = 0;
      R20_ = extra_samus_y_subdisplacement;
      R18_ = extra_samus_y_displacement;
      if ((extra_samus_y_displacement & 0x8000u) != 0) {
        Samus_MoveUp();
      } else {
        ++R18_;
        Samus_MoveDown();
      }
    } else {
      Samus_CheckStartFalling();
      Samus_MoveY_WithSpeedCalc();
    }
  }
}

void Samus_Move_NoSpeedCalc_Y(void) {  // 0x90923F
  if (!extra_samus_y_displacement && !extra_samus_y_subdisplacement) {
    if (samus_pos_adjusted_by_slope_flag) {
      R20_ = 0;
      R18_ = 1;
    } else {
      R20_ = samus_total_x_subspeed;
      R18_ = samus_total_x_speed + 1;
    }
    goto LABEL_8;
  }
  R20_ = extra_samus_y_subdisplacement;
  R18_ = extra_samus_y_displacement;
  if ((extra_samus_y_displacement & 0x8000u) == 0) {
    ++R18_;
LABEL_8:
    Samus_MoveDown();
    return;
  }
  Samus_MoveUp();
}

void Samus_MoveExtraY(void) {  // 0x909288
  if (extra_samus_y_displacement || extra_samus_y_subdisplacement) {
    R20_ = extra_samus_y_subdisplacement;
    R18_ = extra_samus_y_displacement;
    if ((extra_samus_y_displacement & 0x8000u) != 0) {
      Samus_MoveUp();
    } else {
      ++R18_;
      Samus_MoveDown();
    }
  }
}

uint8 Samus_CheckAndMoveY(void) {  // 0x9092B8
  if (!samus_y_dir)
    return 0;
  Samus_CheckStartFalling();
  Samus_MoveY_WithSpeedCalc();
  return 1;
}

uint8 Samus_MoveY_Simple_(void) {  // 0x9092C7
  if (!samus_y_dir)
    return 0;
  Samus_CheckStartFalling();
  Samus_MoveY_WithSpeedCalc();
  return 1;
}

void UNUSED_sub_9092D6(void) {  // 0x9092D6
  samus_y_pos += R18_;
  samus_prev_y_pos = samus_y_pos;
}

void Samus_Move_NoBaseSpeed_X(void) {  // 0x909348
  R18_ = 0;
  R20_ = 0;
  Samus_MoveX();
}

void Samus_MoveLeft(void) {  // 0x909350
  R18_ = ~R18_;
  bool v0 = R20_ == 0;
  R20_ = -R20_;
  if (v0)
    ++R18_;
  samus_collision_flag = Samus_CheckSolidEnemyColl();
  if (samus_collision_flag) {
    Samus_ClearXSpeedIfColl();
    Samus_MoveLeft_NoColl();
    Samus_AlignYPosSlope();
  } else {
    R18_ = ~R18_;
    v0 = R20_ == 0;
    R20_ = -R20_;
    if (v0)
      ++R18_;
    Samus_MoveRight_NoSolidColl();
    projectile_init_speed_samus_moved_left_fract = R20_;
    projectile_init_speed_samus_moved_left = R18_;
    if ((samus_collision_direction & 1) != 0)
      samus_collision_flag = 0;
    Samus_ClearXSpeedIfColl();
    Samus_AlignYPosSlope();
  }
}

void Samus_MoveRight(void) {  // 0x9093B1
  samus_collision_flag = Samus_CheckSolidEnemyColl();
  if (samus_collision_flag) {
    Samus_ClearXSpeedIfColl();
    Samus_MoveRight_NoColl();
    Samus_AlignYPosSlope();
  } else {
    Samus_MoveRight_NoSolidColl();
    projectile_init_speed_samus_moved_right_fract = R20_;
    projectile_init_speed_samus_moved_right = R18_;
    if ((samus_collision_direction & 1) == 0)
      samus_collision_flag = 0;
    Samus_ClearXSpeedIfColl();
    Samus_AlignYPosSlope();
  }
}

void Samus_MoveUp(void) {  // 0x9093EC
  samus_collision_direction = 2;
  R18_ = ~R18_;
  bool v0 = R20_ == 0;
  R20_ = -R20_;
  if (v0)
    ++R18_;
  samus_collision_flag = Samus_CheckSolidEnemyColl();
  if (samus_collision_flag) {
    Samus_MoveUp_SetPoseCalcInput();
    Samus_MoveUp_NoColl();
  } else {
    R18_ = ~R18_;
    v0 = R20_ == 0;
    R20_ = -R20_;
    if (v0)
      ++R18_;
    Samus_MoveDown_NoSolidColl();
    projectile_init_speed_samus_moved_up_fract = R20_;
    projectile_init_speed_samus_moved_up = R18_;
    Samus_MoveUp_SetPoseCalcInput();
  }
}

void Samus_MoveDown(void) {  // 0x909440
  samus_collision_direction = 3;
  samus_collision_flag = Samus_CheckSolidEnemyColl();
  if (samus_collision_flag) {
    Samus_MoveDown_SetPoseCalcInput();
    Samus_MoveDown_NoColl();
  } else {
    Samus_MoveDown_NoSolidColl();
    projectile_init_speed_samus_moved_down_fract = R20_;
    projectile_init_speed_samus_moved_down = R18_;
    Samus_MoveDown_SetPoseCalcInput();
  }
}

void Samus_MoveHandler_ReleaseFromGrapple(void) {  // 0x90946E
  if (samus_y_dir == 1 && (samus_y_speed & 0x8000u) != 0) {
    samus_y_subspeed = 0;
    samus_y_speed = 0;
    samus_y_dir = 2;
    samus_movement_handler = FUNC16(Samus_MovementHandler_Normal);
  }
  samus_x_accel_mode = 2;
  Samus_CalcBaseSpeed_X(Samus_DetermineGrappleSwingSpeed_X());
  if (samus_x_accel_mode || (joypad1_lastkeys & kButton_Right) != 0 || (joypad1_lastkeys & kButton_Left) != 0) {
    Samus_MoveX();
  } else {
    R18_ = 0;
    R20_ = 0;
    samus_x_base_speed = 0;
    samus_x_base_subspeed = 0;
    samus_collision_flag = 0;
  }
  Samus_MoveY_WithSpeedCalc();
  if (input_to_pose_calc)
    samus_movement_handler = FUNC16(Samus_MovementHandler_Normal);
}

void Samus_HandleMovement_DrainedCrouching(void) {  // 0x9094CB
  Samus_MoveY_WithSpeedCalc();
  if (samus_collision_flag) {
    samus_movement_handler = FUNC16(Samus_MovementHandler_Normal);
    samus_anim_frame_timer = 8;
    samus_anim_frame = 7;
    samus_y_subspeed = 0;
    samus_y_speed = 0;
  }
}

void CallScrollingFinishedHook(uint32 ea) {
  switch (ea) {
  case fnSamus_ScrollFinishedHook_SporeSpawnFight: Samus_ScrollFinishedHook_SporeSpawnFight(); return;
  default: Unreachable();
  }
}

void MainScrollingRoutine(void) {  // 0x9094EC
  if (slow_grabble_scrolling_flag) {
    if ((samus_x_pos & 0x8000u) != 0)
      goto LABEL_14;
    uint16 v0;
    v0 = samus_x_pos - layer1_x_pos;
    if (samus_x_pos < layer1_x_pos)
      goto LABEL_7;
    if (v0 >= 0xA0u) {
      layer1_x_pos += 3;
      goto LABEL_8;
    }
    if (v0 < 0x60u)
      LABEL_7:
    layer1_x_pos -= 3;
LABEL_8:
    if ((samus_y_pos & 0x8000u) == 0) {
      uint16 v1 = samus_y_pos - layer1_y_pos;
      if (samus_y_pos >= layer1_y_pos) {
        if (v1 >= 0x90u) {
          layer1_y_pos += 3;
          goto LABEL_14;
        }
        if (v1 >= 0x70u)
          goto LABEL_14;
      }
      layer1_y_pos -= 3;
    }
LABEL_14:
    HandleAutoscrolling_X();
    HandleAutoscrolling_Y();
    goto LABEL_16;
  }
  Samus_CalcDistanceMoved_X();
  Samus_HandleScroll_X();
  Samus_CalcDistanceMoved_Y();
  Samus_HandleScroll_Y();
LABEL_16:
  if (scrolling_finished_hook)
    CallScrollingFinishedHook(scrolling_finished_hook | 0x900000);
  samus_prev_x_pos = samus_x_pos;
  samus_prev_x_subpos = samus_x_subpos;
  samus_prev_y_pos = samus_y_pos;
  samus_prev_y_subpos = samus_y_subpos;
}

void Samus_ScrollFinishedHook_SporeSpawnFight(void) {  // 0x909589
  if (layer1_y_pos <= 0x1D0u)
    layer1_y_pos = 464;
}

void Samus_HandleScroll_X(void) {  // 0x9095A0
  static const uint16 kSamus_HandleScroll_X_FaceLeft[4] = { 0xa0, 0x50, 0x20, 0xe0 };
  static const uint16 kSamus_HandleScroll_X_FaceRight[4] = { 0x60, 0x40, 0x20, 0xe0 };


  if (samus_prev_x_pos == samus_x_pos) {
    HandleAutoscrolling_X();
    return;
  }
  R18_ = layer1_x_pos;
  if (knockback_dir || samus_movement_type == 16 || samus_x_accel_mode == 1) {
    if (samus_pose_x_dir == 4) {
LABEL_9:
      ideal_layer1_xpos = samus_x_pos - kSamus_HandleScroll_X_FaceRight[camera_distance_index >> 1];
      goto LABEL_11;
    }
  } else if (samus_pose_x_dir != 4) {
    goto LABEL_9;
  }
  ideal_layer1_xpos = samus_x_pos - kSamus_HandleScroll_X_FaceLeft[camera_distance_index >> 1];
LABEL_11:
  if (ideal_layer1_xpos != layer1_x_pos) {
    if ((int16)(ideal_layer1_xpos - layer1_x_pos) < 0) {
      uint16 v1 = (__PAIR32__(layer1_x_pos, layer1_x_subpos) - __PAIR32__(absolute_moved_last_frame_x, absolute_moved_last_frame_x_fract)) >> 16;
      layer1_x_subpos -= absolute_moved_last_frame_x_fract;
      layer1_x_pos = v1;
      HandleScrollingWhenTriggeringScrollLeft();
    } else {
      uint16 v0 = (__PAIR32__(absolute_moved_last_frame_x, absolute_moved_last_frame_x_fract) + __PAIR32__(layer1_x_pos, layer1_x_subpos)) >> 16;
      layer1_x_subpos += absolute_moved_last_frame_x_fract;
      layer1_x_pos = v0;
      HandleScrollingWhenTriggeringScrollRight();
    }
  }
}

void Samus_HandleScroll_Y(void) {  // 0x90964F
  if (samus_prev_y_pos == samus_y_pos) {
    HandleAutoscrolling_Y();
  } else {
    R18_ = layer1_y_pos;
    if (samus_y_dir == 1)
      ideal_layer1_ypos = samus_y_pos - down_scroller;
    else
      ideal_layer1_ypos = samus_y_pos - up_scroller;
    if (ideal_layer1_ypos != layer1_y_pos) {
      if ((int16)(ideal_layer1_ypos - layer1_y_pos) < 0) {
        uint16 v1 = (__PAIR32__(layer1_y_pos, layer1_y_subpos)
              - __PAIR32__(absolute_moved_last_frame_y, absolute_moved_last_frame_y_fract)) >> 16;
        layer1_y_subpos -= absolute_moved_last_frame_y_fract;
        layer1_y_pos = v1;
        HandleScrollingWhenTriggeringScrollUp();
      } else {
        uint16 v0 = (__PAIR32__(absolute_moved_last_frame_y, absolute_moved_last_frame_y_fract)
              + __PAIR32__(layer1_y_pos, layer1_y_subpos)) >> 16;
        layer1_y_subpos += absolute_moved_last_frame_y_fract;
        layer1_y_pos = v0;
        HandleScrollingWhenTriggeringScrollDown();
      }
    }
  }
}

static const uint16 g_word_909EAD = 1;

void Samus_CalcDistanceMoved_X(void) {  // 0x9096C0
  if ((int16)(samus_x_pos - samus_prev_x_pos) >= 0) {
    absolute_moved_last_frame_x_fract = samus_x_subpos - samus_prev_x_subpos;
    absolute_moved_last_frame_x = g_word_909EAD
      + ((__PAIR32__(samus_x_pos, samus_x_subpos)
          - __PAIR32__(samus_prev_x_pos, samus_prev_x_subpos)) >> 16);
  } else {
    absolute_moved_last_frame_x_fract = samus_prev_x_subpos - samus_x_subpos;
    absolute_moved_last_frame_x = g_word_909EAD
      + ((__PAIR32__(samus_prev_x_pos, samus_prev_x_subpos)
          - __PAIR32__(samus_x_pos, samus_x_subpos)) >> 16);
  }
}

void Samus_CalcDistanceMoved_Y(void) {  // 0x9096FF
  if ((int16)(samus_y_pos - samus_prev_y_pos) >= 0) {
    absolute_moved_last_frame_y_fract = samus_y_subpos - samus_prev_y_subpos;
    absolute_moved_last_frame_y = g_word_909EAD
      + ((__PAIR32__(samus_y_pos, samus_y_subpos)
          - __PAIR32__(samus_prev_y_pos, samus_prev_y_subpos)) >> 16);
  } else {
    absolute_moved_last_frame_y_fract = samus_prev_y_subpos - samus_y_subpos;
    absolute_moved_last_frame_y = g_word_909EAD
      + ((__PAIR32__(samus_prev_y_pos, samus_prev_y_subpos)
          - __PAIR32__(samus_y_pos, samus_y_subpos)) >> 16);
  }
}

static const uint16 kSamus_HandleExtraRunspeedX_Tab0[3] = { 0, 0, 0 };
static const uint16 kSamus_HandleExtraRunspeedX_Tab1[3] = { 0x1000, 0x400, 0x400 };
static const uint16 kSamus_HandleExtraRunspeedX_Tab2[3] = { 7, 4, 4 };
static const uint16 kSamus_HandleExtraRunspeedX_Tab3[3] = { 0, 0, 0 };
static const uint16 kSamus_HandleExtraRunspeedX_Tab4[3] = { 2, 1, 0 };
static const uint16 kSamus_HandleExtraRunspeedX_Tab5[3] = { 0, 0, 0 };

void Samus_HandleExtraRunspeedX(void) {  // 0x90973E
  if ((equipped_items & 0x20) == 0) {
    Samus_GetBottomBoundary();
    if ((fx_y_pos & 0x8000u) != 0) {
      if ((lava_acid_y_pos & 0x8000u) == 0 && sign16(lava_acid_y_pos - R18_)) {
LABEL_24:
        if (!samus_has_momentum_flag) {
          samus_x_extra_run_speed = 0;
          samus_x_extra_run_subspeed = 0;
        }
        goto LABEL_26;
      }
    } else if (sign16(fx_y_pos - R18_) && (fx_liquid_options & 4) == 0) {
      goto LABEL_24;
    }
  }
  if (samus_movement_type != 1 || (button_config_run_b & joypad1_lastkeys) == 0)
    goto LABEL_24;
  if ((equipped_items & 0x2000) != 0) {
    if (!samus_has_momentum_flag) {
      samus_has_momentum_flag = 1;
      special_samus_palette_timer = 1;
      special_samus_palette_frame = 0;
      speed_boost_counter = kSpeedBoostToCtr[0];
    }
    if ((int16)(samus_x_extra_run_speed - kSamus_HandleExtraRunspeedX_Tab2[0]) >= 0
        && (int16)(samus_x_extra_run_subspeed - kSamus_HandleExtraRunspeedX_Tab3[0]) >= 0) {
      samus_x_extra_run_speed = kSamus_HandleExtraRunspeedX_Tab2[0];
      samus_x_extra_run_subspeed = kSamus_HandleExtraRunspeedX_Tab3[0];
      goto LABEL_26;
    }
  } else {
    if (!samus_has_momentum_flag) {
      samus_has_momentum_flag = 1;
      speed_boost_counter = 0;
    }
    if ((int16)(samus_x_extra_run_speed - kSamus_HandleExtraRunspeedX_Tab4[0]) >= 0
        && (int16)(samus_x_extra_run_subspeed - kSamus_HandleExtraRunspeedX_Tab5[0]) >= 0) {
      samus_x_extra_run_speed = kSamus_HandleExtraRunspeedX_Tab4[0];
      samus_x_extra_run_subspeed = kSamus_HandleExtraRunspeedX_Tab5[0];
      goto LABEL_26;
    }
  }
  bool v0;
  v0 = __CFADD__uint16(kSamus_HandleExtraRunspeedX_Tab1[0], samus_x_extra_run_subspeed);
  samus_x_extra_run_subspeed += kSamus_HandleExtraRunspeedX_Tab1[0];
  samus_x_extra_run_speed += kSamus_HandleExtraRunspeedX_Tab0[0] + v0;
LABEL_26:
  if ((speed_boost_counter & 0xFF00) == 1024)
    samus_contact_damage_index = 1;
}

void Samus_MoveRight_NoColl(void) {  // 0x909826
  bool v0 = __CFADD__uint16(R20_, samus_x_subpos);
  samus_x_subpos += R20_;
  samus_x_pos += R18_ + v0;
  projectile_init_speed_samus_moved_right_fract = R20_;
  projectile_init_speed_samus_moved_right = R18_;
}

void Samus_MoveLeft_NoColl(void) {  // 0x909842
  uint16 v0 = (__PAIR32__(samus_x_pos, samus_x_subpos) - __PAIR32__(R18_, R20_)) >> 16;
  samus_x_subpos -= R20_;
  samus_x_pos = v0;
  R18_ = ~R18_;
  bool v1 = R20_ == 0;
  R20_ = -R20_;
  if (v1)
    ++R18_;
  projectile_init_speed_samus_moved_left_fract = R20_;
  projectile_init_speed_samus_moved_left = R18_;
}

void Samus_MoveDown_NoColl(void) {  // 0x909871
  bool v0 = __CFADD__uint16(R20_, samus_y_subpos);
  samus_y_subpos += R20_;
  samus_y_pos += R18_ + v0;
  projectile_init_speed_samus_moved_down_fract = R20_;
  projectile_init_speed_samus_moved_down = R18_;
}

void Samus_MoveUp_NoColl(void) {  // 0x90988D
  uint16 v0 = (__PAIR32__(samus_y_pos, samus_y_subpos) - __PAIR32__(R18_, R20_)) >> 16;
  samus_y_subpos -= R20_;
  samus_y_pos = v0;
  R18_ = ~R18_;
  bool v1 = R20_ == 0;
  R20_ = -R20_;
  if (v1)
    ++R18_;
  projectile_init_speed_samus_moved_up_fract = R20_;
  projectile_init_speed_samus_moved_up = R18_;
}

void Samus_InitJump(void) {  // 0x9098BC
  static const uint16 kSamus_InitJump_Speed_Y[3] = { 4, 1, 2 };
  static const uint16 kSamus_InitJump_Subspeed_Y[3] = { 0xe000, 0xc000, 0xc000 };
  static const uint16 kSamus_InitJump_Tab2[3] = { 6, 2, 3 };
  static const uint16 kSamus_InitJump_Tab3[3] = { 0, 0x8000, 0x8000 };
  uint16 v0;

  if ((equipped_items & 0x20) != 0)
    goto LABEL_7;
  Samus_GetBottomBoundary();
  if ((fx_y_pos & 0x8000u) == 0) {
    if (sign16(fx_y_pos - R18_) && (fx_liquid_options & 4) == 0) {
      v0 = 2;
      goto LABEL_11;
    }
LABEL_7:
    v0 = 0;
    goto LABEL_11;
  }
  if ((lava_acid_y_pos & 0x8000u) != 0 || !sign16(lava_acid_y_pos - R18_))
    goto LABEL_7;
  v0 = 4;
LABEL_11:;
  int v1 = v0 >> 1;
  if ((equipped_items & 0x100) != 0) {
    samus_y_subspeed = kSamus_InitJump_Tab3[v1];
    samus_y_speed = kSamus_InitJump_Tab2[v1];
  } else {
    samus_y_subspeed = kSamus_InitJump_Subspeed_Y[v1];
    samus_y_speed = kSamus_InitJump_Speed_Y[v1];
  }
  if ((equipped_items & 0x2000) != 0) {
    R18_ = samus_x_extra_run_speed >> 1;
    samus_y_subspeed += samus_x_extra_run_subspeed;
    samus_y_speed += samus_x_extra_run_speed >> 1;
  }
  grapple_walljump_timer = 0;
  reached_ceres_elevator_fade_timer = 0;
  samus_y_dir = 1;
}

void Samus_InitWallJump(void) {  // 0x909949
  static const uint16 kSamus_InitWallJump_0[3] = { 4, 0, 2 };
  static const uint16 kSamus_InitWallJump_1[3] = { 0xa000, 0x4000, 0xa000 };
  static const uint16 kSamus_InitWallJump_2[3] = { 5, 0, 3 };
  static const uint16 kSamus_InitWallJump_3[3] = { 0x8000, 0x8000, 0x8000 };
  uint16 v0;

  if ((equipped_items & 0x20) != 0)
    goto LABEL_7;
  Samus_GetBottomBoundary();
  if ((fx_y_pos & 0x8000u) == 0) {
    if (sign16(fx_y_pos - R18_) && (fx_liquid_options & 4) == 0) {
      v0 = 2;
      goto LABEL_11;
    }
LABEL_7:
    v0 = 0;
    goto LABEL_11;
  }
  if ((lava_acid_y_pos & 0x8000u) != 0 || !sign16(lava_acid_y_pos - R18_))
    goto LABEL_7;
  v0 = 4;
LABEL_11:;
  int v1 = v0 >> 1;
  if ((equipped_items & 0x100) != 0) {
    samus_y_subspeed = kSamus_InitWallJump_3[v1];
    samus_y_speed = kSamus_InitWallJump_2[v1];
  } else {
    samus_y_subspeed = kSamus_InitWallJump_1[v1];
    samus_y_speed = kSamus_InitWallJump_0[v1];
  }
  if ((equipped_items & 0x2000) != 0) {
    R18_ = samus_x_extra_run_speed >> 1;
    samus_y_subspeed += samus_x_extra_run_subspeed;
    samus_y_speed += samus_x_extra_run_speed >> 1;
  }
  grapple_walljump_timer = 0;
  reached_ceres_elevator_fade_timer = 0;
  samus_y_dir = 1;
}

static const uint16 kSamus_SetSpeedForKnockback_Y_Speed[3] = { 5, 2, 2 };
static const uint16 kSamus_SetSpeedForKnockback_Y_Subspeed[3] = { 0, 0, 0 };

void Samus_SetSpeedForKnockback_Y(void) {  // 0x9099D6
  uint16 v0;

  if ((equipped_items & 0x20) == 0) {
    Samus_GetBottomBoundary();
    if ((fx_y_pos & 0x8000u) != 0) {
      if ((lava_acid_y_pos & 0x8000u) == 0 && sign16(lava_acid_y_pos - R18_)) {
        v0 = 4;
        goto LABEL_11;
      }
    } else if (sign16(fx_y_pos - R18_) && (fx_liquid_options & 4) == 0) {
      v0 = 2;
      goto LABEL_11;
    }
  }
  v0 = 0;
LABEL_11:;
  int v1 = v0 >> 1;
  samus_y_subspeed = kSamus_SetSpeedForKnockback_Y_Subspeed[v1];
  samus_y_speed = kSamus_SetSpeedForKnockback_Y_Speed[v1];
  grapple_walljump_timer = 0;
  reached_ceres_elevator_fade_timer = 0;
  samus_y_dir = 1;
}

void Samus_InitBombJump(void) {  // 0x909A2C
  static const uint16 kSamus_InitBombJump_Speed[3] = { 2, 0, 0 };
  static const uint16 kSamus_InitBombJump_Subspeed[3] = { 0xc000, 0x1000, 0x1000 };
  uint16 v0;

  if ((equipped_items & 0x20) == 0) {
    Samus_GetBottomBoundary();
    if ((fx_y_pos & 0x8000u) != 0) {
      if ((lava_acid_y_pos & 0x8000u) == 0 && sign16(lava_acid_y_pos - R18_)) {
        v0 = 4;
        goto LABEL_11;
      }
    } else if (sign16(fx_y_pos - R18_) && (fx_liquid_options & 4) == 0) {
      v0 = 2;
      goto LABEL_11;
    }
  }
  v0 = 0;
LABEL_11:;
  int v1 = v0 >> 1;
  samus_y_subspeed = kSamus_InitBombJump_Subspeed[v1];
  samus_y_speed = kSamus_InitBombJump_Speed[v1];
  grapple_walljump_timer = 0;
  reached_ceres_elevator_fade_timer = 0;
  samus_y_dir = 1;
}

void Samus_CalcBaseSpeed_X(uint16 k) {  // 0x909A7E
  SamusSpeedTableEntry *v1;
  SamusSpeedTableEntry *v4;
  SamusSpeedTableEntry *v5;
  SamusSpeedTableEntry *sste;
  SamusSpeedTableEntry *v7;
  int16 v8;

  if (samus_x_accel_mode) {
    if (samus_x_decel_mult) {
      sste = get_SamusSpeedTableEntry(k);
      R20_ = Mult8x8(samus_x_decel_mult, HIBYTE(sste->decel_sub));
      R18_ = Mult8x8(samus_x_decel_mult, sste->decel) >> 8;
    } else {
      v7 = get_SamusSpeedTableEntry(k);
      R20_ = v7->decel_sub;
      R18_ = v7->decel;
    }
    v8 = (__PAIR32__(samus_x_base_speed, samus_x_base_subspeed) - __PAIR32__(R18_, R20_)) >> 16;
    samus_x_base_subspeed -= R20_;
    samus_x_base_speed = v8;
    if (v8 < 0) {
      samus_x_base_speed = 0;
      samus_x_base_subspeed = 0;
      samus_x_accel_mode = 0;
    }
  } else {
    v1 = get_SamusSpeedTableEntry(k);
    bool v2 = __CFADD__uint16(v1->accel_sub, samus_x_base_subspeed);
    samus_x_base_subspeed += v1->accel_sub;
    uint16 v3 = v1->accel + v2 + samus_x_base_speed;
    samus_x_base_speed = v3;
    if ((int16)(v3 - v1->max_speed) >= 0) {
      if (v3 != v1->max_speed
          || (v5 = get_SamusSpeedTableEntry(k), (int16)(samus_x_base_subspeed - v5->max_speed_sub) >= 0)
          && samus_x_base_subspeed != v5->max_speed_sub) {
        v4 = get_SamusSpeedTableEntry(k);
        samus_x_base_speed = v4->max_speed;
        samus_x_base_subspeed = v4->max_speed_sub;
      }
    }
  }
  R18_ = samus_x_base_speed;
  R20_ = samus_x_base_subspeed;
}

uint8 Samus_CalcBaseSpeed_NoDecel_X(uint16 k) {  // 0x909B1F
  SamusSpeedTableEntry *v1;
  SamusSpeedTableEntry *v4;
  SamusSpeedTableEntry *v5;
  SamusSpeedTableEntry *sste;
  SamusSpeedTableEntry *v7;
  int16 v8;

  if ((samus_x_accel_mode & 1) != 0) {
    if (samus_x_decel_mult) {
      sste = get_SamusSpeedTableEntry(k);
      R20_ = Mult8x8(samus_x_decel_mult, HIBYTE(sste->decel_sub));
      R18_ = Mult8x8(samus_x_decel_mult, sste->decel) >> 8;
    } else {
      v7 = get_SamusSpeedTableEntry(k);
      R20_ = v7->decel_sub;
      R18_ = v7->decel;
    }
    v8 = (__PAIR32__(samus_x_base_speed, samus_x_base_subspeed) - __PAIR32__(R18_, R20_)) >> 16;
    samus_x_base_subspeed -= R20_;
    samus_x_base_speed = v8;
    if (v8 < 0) {
      samus_x_base_speed = 0;
      samus_x_base_subspeed = 0;
      samus_x_accel_mode = 0;
    }
  } else {
    v1 = get_SamusSpeedTableEntry(k);
    bool v2 = __CFADD__uint16(v1->accel_sub, samus_x_base_subspeed);
    samus_x_base_subspeed += v1->accel_sub;
    uint16 v3 = v1->accel + v2 + samus_x_base_speed;
    samus_x_base_speed = v3;
    if ((int16)(v3 - v1->max_speed) >= 0) {
      if (v3 != v1->max_speed
          || (v5 = get_SamusSpeedTableEntry(k), (int16)(samus_x_base_subspeed - v5->max_speed_sub) >= 0)
          && samus_x_base_subspeed != v5->max_speed_sub) {
        v4 = get_SamusSpeedTableEntry(k);
        samus_x_base_speed = v4->max_speed;
        samus_x_base_subspeed = v4->max_speed_sub;
        R18_ = samus_x_base_speed;
        R20_ = samus_x_base_subspeed;
        return 1;
      }
    }
  }
  R18_ = samus_x_base_speed;
  R20_ = samus_x_base_subspeed;
  return 0;
}

uint16 Samus_DetermineSpeedTableEntryPtr_X(void) {  // 0x909BD1
  if ((equipped_items & 0x20) == 0) {
    Samus_GetBottomBoundary();
    if ((fx_y_pos & 0x8000u) != 0) {
      if ((lava_acid_y_pos & 0x8000u) == 0 && sign16(lava_acid_y_pos - R18_))
        samus_x_speed_table_pointer = addr_kSamusSpeedTable_LavaAcid_X;
    } else if (sign16(fx_y_pos - R18_) && (fx_liquid_options & 4) == 0) {
      samus_x_speed_table_pointer = addr_kSamusSpeedTable_Water_X;
    }
  }
  R20_ = samus_movement_type;
  return samus_x_speed_table_pointer + 12 * samus_movement_type;
}

uint16 Samus_DetermineGrappleSwingSpeed_X(void) {  // 0x909C21
  if ((equipped_items & 0x20) != 0)
    return addr_stru_909F31;
  Samus_GetBottomBoundary();
  if ((fx_y_pos & 0x8000u) == 0) {
    if (sign16(fx_y_pos - R18_) && (fx_liquid_options & 4) == 0)
      return addr_stru_909F3D;
    return addr_stru_909F31;
  }
  if ((lava_acid_y_pos & 0x8000u) != 0 || !sign16(lava_acid_y_pos - R18_))
    return addr_stru_909F31;
  return addr_kSamusSpeedTable_Normal_X;
}

void Samus_DetermineAccel_Y(void) {  // 0x909C5B
  static const uint16 g_word_909EA1 = 0x1c00;
  static const uint16 g_word_909EA3 = 0x800;
  static const uint16 g_word_909EA5 = 0x900;
  static const uint16 g_word_909EA7 = 0;
  static const uint16 g_word_909EA9 = 0;
  static const uint16 g_word_909EAB = 0;
  static const uint16 g_word_909EAD = 1;

  if ((equipped_items & 0x20) != 0)
    goto LABEL_7;
  Samus_GetBottomBoundary();
  if ((fx_y_pos & 0x8000u) == 0) {
    if (sign16(fx_y_pos - R18_) && (fx_liquid_options & 4) == 0) {
      samus_y_subaccel = g_word_909EA3;
      samus_y_accel = g_word_909EA9;
      return;
    }
LABEL_7:
    samus_y_subaccel = g_word_909EA1;
    samus_y_accel = g_word_909EA7;
    return;
  }
  if ((lava_acid_y_pos & 0x8000u) != 0 || !sign16(lava_acid_y_pos - R18_))
    goto LABEL_7;
  samus_y_subaccel = g_word_909EA5;
  samus_y_accel = g_word_909EAB;
}

uint8 Samus_GrappleWallJumpCheck(void) {  // 0x909CAC
  enemy_index_to_shake = -1;
  if (samus_pose_x_dir != 4) {
    if (samus_pose_x_dir != 8)
      return 0;
    samus_collision_direction = 1;
    if (!Samus_CheckSolidEnemyColl())
      return WallJumpBlockCollDetect() & 1 && (button_config_jump_a & joypad1_newkeys) != 0;
    if ((button_config_jump_a & joypad1_newkeys) == 0)
      return 0;
    goto LABEL_17;
  }
  samus_collision_direction = 0;
  if (!Samus_CheckSolidEnemyColl()) {
    R18_ = ~R18_;
    bool v0 = R20_ == 0;
    R20_ = -R20_;
    if (v0)
      ++R18_;
    return WallJumpBlockCollDetect() & 1 && (button_config_jump_a & joypad1_newkeys) != 0;
  }
  if ((button_config_jump_a & joypad1_newkeys) != 0) {
LABEL_17:
    enemy_index_to_shake = R22_;
    return 1;
  }
  return 0;
}

uint8 Samus_WallJumpCheck(void) {  // 0x909D35
  if (samus_last_different_pose_movement_type != 3 && samus_last_different_pose_movement_type != 20)
    return 0;
  if (samus_pose == kPose_81_FaceR_Screwattack || samus_pose == kPose_82_FaceL_Screwattack) {
    if (!sign16(samus_anim_frame - 27))
      goto LABEL_22;
    goto LABEL_8;
  }
  if (sign16(samus_anim_frame - 11)) {
LABEL_8:
    if ((joypad1_lastkeys & kButton_Left) == 0) {
      if ((joypad1_lastkeys & kButton_Right) == 0)
        return 0;
      samus_collision_direction = 0;
      if (!Samus_CheckSolidEnemyColl()) {
        R18_ = ~R18_;
        bool v1 = R20_ == 0;
        R20_ = -R20_;
        if (v1)
          ++R18_;
        if (!(WallJumpBlockCollDetect() & 1))
          return 0;
      }
      goto LABEL_18;
    }
    samus_collision_direction = 1;
    if (Samus_CheckSolidEnemyColl() || WallJumpBlockCollDetect() & 1) {
LABEL_18:
      if (samus_pose == kPose_81_FaceR_Screwattack || samus_pose == kPose_82_FaceL_Screwattack) {
        samus_anim_frame_timer = 1;
        samus_anim_frame = 26;
      } else {
        samus_anim_frame_timer = 1;
        samus_anim_frame = 10;
      }
    }
    return 0;
  }
LABEL_22:
  enemy_index_to_shake = -1;
  if ((joypad1_lastkeys & kButton_Left) != 0) {
    samus_collision_direction = 1;
    if (!Samus_CheckSolidEnemyColl()) {
      if (!(WallJumpBlockCollDetect() & 1) || (button_config_jump_a & joypad1_newkeys) == 0)
        return 0;
      goto LABEL_40;
    }
    if ((button_config_jump_a & joypad1_newkeys) == 0)
      return 0;
LABEL_38:
    if (sign16(R18_ - 8)) {
      input_to_pose_calc = 5;
      enemy_index_to_shake = R22_;
      return 1;
    }
    return 0;
  }
  if ((joypad1_lastkeys & kButton_Right) == 0)
    return 0;
  samus_collision_direction = 0;
  if (Samus_CheckSolidEnemyColl()) {
    if ((button_config_jump_a & joypad1_newkeys) == 0)
      return 0;
    goto LABEL_38;
  }
  R18_ = ~R18_;
  bool v1;
  v1 = R20_ == 0;
  R20_ = -R20_;
  if (v1)
    ++R18_;
  if (!(WallJumpBlockCollDetect() & 1) || (button_config_jump_a & joypad1_newkeys) == 0)
    return 0;
LABEL_40:
  if (sign16(R18_ - 8)) {
    input_to_pose_calc = 5;
    return 1;
  }
  return 0;
}

void Samus_Movement_07_Unused(void) {  // 0x90A32D
  input_to_pose_calc = 0;
}

static HandlerFunc *const kSamusMovementHandlers[28] = {  // 0x90A337
  &Samus_Movement_00_Standing,
  &Samus_Movement_01_Running,
  &Samus_Movement_02_NormalJumping,
  &Samus_Movement_03_SpinJumping,
  &Samus_Movement_04_MorphBallOnGround,
  &Samus_Movement_05_Crouching,
  &Samus_Movement_06_Falling,
  &Samus_Movement_07_Unused,
  &Samus_Movement_08_MorphBallFalling,
  &Samus_Movement_09_Unused,
  &Samus_Movement_0A_KnockbackOrCrystalFlashEnding,
  &Samus_Movement_0B_Unused,
  &Samus_Movement_0C_Unused,
  &Samus_Movement_0D_Unused,
  &Samus_Movement_0E_TurningAroundOnGround,
  &Samus_Movement_0F_CrouchingEtcTransition,
  &Samus_Movement_10_Moonwalking,
  &Samus_Movement_11_SpringBallOnGround,
  &Samus_Movement_12_SpringBallInAir,
  &Samus_Movement_13_SpringBallFalling,
  &Samus_Movement_14_WallJumping,
  &Samus_Movement_15_RanIntoWall,
  &Samus_Movement_16_Grappling,
  &Samus_Movement_17_TurningAroundJumping,
  &Samus_Movement_18_TurningAroundFalling,
  &Samus_Movement_19_DamageBoost,
  &Samus_Movement_1A_GrabbedByDraygon,
  &Samus_Movement_1B_ShinesparkEtc,
};

void Samus_MovementHandler_Normal(void) {
  if (!time_is_frozen_flag) {
    kSamusMovementHandlers[samus_movement_type]();
    Samus_UpdateSpeedEchoPos();
  }
}

void Samus_Movement_00_Standing(void) {  // 0x90A383
  if (samus_pose && samus_pose != kPose_9B_FaceF_VariaGravitySuit) {
    if ((samus_pose == kPose_01_FaceR_Normal || samus_pose == kPose_02_FaceL_Normal)
        && (button_config_shoot_x & joypad1_lastkeys) != 0) {
      samus_anim_frame_timer = 16;
      samus_anim_frame = 0;
    }
    Samus_Move_NoBaseSpeed_X();
    Samus_Move_NoSpeedCalc_Y();
    Samus_CancelSpeedBoost();
    samus_x_extra_run_speed = 0;
    samus_x_extra_run_subspeed = 0;
    samus_x_base_speed = 0;
    samus_x_base_subspeed = 0;
    samus_x_accel_mode = 0;
  } else {
    if (elevator_status) {
      samus_collision_direction = 2;
      R18_ = 1;
      R20_ = 0;
      Samus_MoveDown_NoSolidColl();
    }
    input_to_pose_calc = 0;
  }
}

void Samus_Movement_01_Running(void) {  // 0x90A3E5
  static const uint8 kSamusFootstepFrame[10] = { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 };
  Samus_HandleMovement_X();
  Samus_Move_NoSpeedCalc_Y();
  if (samus_anim_frame_timer == 1) {
    if (kSamusFootstepFrame[samus_anim_frame]) {
      Samus_FootstepGraphics();
      if (!cinematic_function && !boss_id && !samus_shine_timer && (speed_boost_counter & 0x400) == 0)
        QueueSfx3_Max6(6u);
    }
  }
}

void Samus_Movement_02_NormalJumping(void) {  // 0x90A42E
  Samus_JumpingMovement();
}

// Warning: OOB

void Samus_Movement_03_SpinJumping(void) {  // 0x90A436
  static const uint16 kSamusPhys_JumpMinYVelAir = 0x280;
  static const uint16 kSamusPhys_JumpMaxYVelAir = 0x500;
  static const uint16 kSamusPhys_JumpMinYVelWater = 0x80;
  static const uint16 kSamusPhys_JumpMaxYVelWater = 0x500;

  if ((samus_suit_palette_index & 4) == 0) {
    Samus_GetTopBottomBoundary();
    if ((fx_y_pos & 0x8000u) != 0) {
      if ((lava_acid_y_pos & 0x8000u) == 0 && sign16(lava_acid_y_pos - R20_))
        goto LABEL_8;
    } else if (sign16(fx_y_pos - R20_) && (fx_liquid_options & 4) == 0) {
LABEL_8:
      R18_ = 1;
      goto LABEL_10;
    }
  }
  R18_ = 0;
LABEL_10:
  if (R18_)
    goto LABEL_26;
  if ((equipped_items & 0x200) != 0) {
    if (samus_y_dir != 2)
      goto LABEL_24;
    if (liquid_physics_type) {
      if ((int16)(*(uint16 *)((char *)&samus_y_subspeed + 1) - kSamusPhys_JumpMinYVelWater) < 0
          || (int16)(*(uint16 *)((char *)&samus_y_subspeed + 1) - kSamusPhys_JumpMaxYVelWater) >= 0) {
        goto LABEL_24;
      }
    } else if ((int16)(*(uint16 *)((char *)&samus_y_subspeed + 1) - kSamusPhys_JumpMinYVelAir) < 0
               || (int16)(*(uint16 *)((char *)&samus_y_subspeed + 1) - kSamusPhys_JumpMaxYVelAir) >= 0) {
      goto LABEL_24;
    }
    UNUSED_word_7E0DFA = UNUSED_word_7E0DFA & 0xFF00 | 1;
    if ((button_config_jump_a & joypad1_newkeys) != 0)
      Samus_InitJump();
LABEL_24:
    if (samus_pose == kPose_81_FaceR_Screwattack || samus_pose == kPose_82_FaceL_Screwattack)
      goto LABEL_33;
    goto LABEL_26;
  }
  if (samus_pose == kPose_81_FaceR_Screwattack || samus_pose == kPose_82_FaceL_Screwattack) {
LABEL_33:
    samus_contact_damage_index = 3;
    goto LABEL_34;
  }
LABEL_26:
  if (R18_ && samus_anim_frame_timer == 1 && kSamusFramesForUnderwaterSfx[samus_anim_frame])
    QueueSfx1_Max6(0x2Fu);
  if (!sign16(flare_counter - 60) && !R18_)
    samus_contact_damage_index = 4;
LABEL_34:
  Samus_SpinJumpMovement();
}

void Samus_Movement_04_MorphBallOnGround(void) {  // 0x90A521
  if (!samus_x_accel_mode) {
    if (samus_pose_x_dir == 4) {
      if (samus_pose == kPose_41_FaceL_Morphball_Ground)
        goto LABEL_6;
    } else if (samus_pose == kPose_1D_FaceR_Morphball_Ground) {
LABEL_6:
      Samus_Move_NoBaseSpeed_X();
      if (!(Samus_CheckAndMoveY() & 1)) {
        Samus_Move_NoSpeedCalc_Y();
        Samus_CancelSpeedBoost();
        samus_x_extra_run_speed = 0;
        samus_x_extra_run_subspeed = 0;
        samus_x_base_speed = 0;
        samus_x_base_subspeed = 0;
        samus_x_accel_mode = 0;
      }
      return;
    }
  }
  Samus_HandleMovement_X();
  if (!(Samus_CheckAndMoveY() & 1))
    Samus_Move_NoSpeedCalc_Y();
}

void Samus_Movement_05_Crouching(void) {  // 0x90A573
  Samus_Move_NoBaseSpeed_X();
  Samus_Move_NoSpeedCalc_Y();
  samus_x_extra_run_speed = 0;
  samus_x_extra_run_subspeed = 0;
  samus_x_base_speed = 0;
  samus_x_base_subspeed = 0;
  samus_x_accel_mode = 0;
}

void Samus_Movement_06_Falling(void) {  // 0x90A58D
  Samus_FallingMovement();
  if ((samus_pose == kPose_29_FaceR_Fall
       || samus_pose == kPose_2A_FaceL_Fall
       || samus_pose == kPose_67_FaceR_Fall_Gun
       || samus_pose == kPose_68_FaceL_Fall_Gun)
      && !sign16(samus_y_speed - 5)) {
    if (sign16(samus_anim_frame - 5)) {
      samus_anim_frame_timer = 8;
      samus_anim_frame = 5;
    }
  }
}

void Samus_Movement_08_MorphBallFalling(void) {  // 0x90A5CA
  if ((joypad1_lastkeys & (kButton_Left | kButton_Right)) == 0 && !samus_x_accel_mode) {
    Samus_CancelSpeedBoost();
    samus_x_extra_run_speed = 0;
    samus_x_extra_run_subspeed = 0;
    samus_x_base_speed = 0;
    samus_x_base_subspeed = 0;
    samus_x_accel_mode = 0;
  }
  if (used_for_ball_bounce_on_landing)
    Samus_MorphedBouncingMovement();
  else
    Samus_MorphedFallingMovement();
}

void Samus_Movement_09_Unused(void) {  // 0x90A5FB
  ;
}

void Samus_Movement_0A_KnockbackOrCrystalFlashEnding(void) {  // 0x90A5FC
  input_to_pose_calc = 0;
  Samus_Move_NoSpeedCalc_Y();
}

void Samus_Movement_0B_Unused(void) {  // 0x90A607
  input_to_pose_calc = 0;
}

void Samus_Movement_0C_Unused(void) {  // 0x90A60F
  input_to_pose_calc = 0;
}

void Samus_Movement_0D_Unused(void) {  // 0x90A617
  ;
}

void nullsub_13(void) {}

static HandlerFunc *const kSamusCrouchingEtcFuncs[12] = {  // 0x90A61C
  nullsub_13,
  nullsub_13,
  nullsub_13,
  nullsub_13,
  SamusCrouchingEtcFunc,
  SamusCrouchingEtcFunc,
  nullsub_13,
  nullsub_13,
  nullsub_13,
  nullsub_13,
  SamusCrouchingEtcFunc,
  SamusCrouchingEtcFunc,
};

void Samus_Movement_0F_CrouchingEtcTransition(void) {
  if (sign16(samus_pose - kPose_F1_FaceR_CrouchTrans_AimU)) {
    if (!sign16(samus_pose - kPose_DB))
      goto LABEL_6;
    kSamusCrouchingEtcFuncs[samus_pose - 53]();
  }
  Samus_Move_NoBaseSpeed_X();
  if (!(Samus_MoveY_Simple_() & 1))
    Samus_Move_NoSpeedCalc_Y();
LABEL_6:
  if (input_to_pose_calc == 1025) {
    samus_y_subspeed = 0;
    samus_y_speed = 0;
    samus_y_dir = 0;
    used_for_ball_bounce_on_landing = 0;
  }
  input_to_pose_calc = 0;
}

void SamusCrouchingEtcFunc(void) {  // 0x90A672
  enable_horiz_slope_coll = 3;
  UNUSEDword_7E0AA4 = 0;
}

void Samus_Movement_0E_TurningAroundOnGround(void) {  // 0x90A67C
  Samus_HandleMovement_X();
  Samus_Move_NoSpeedCalc_Y();
  Samus_CancelSpeedBoost();
  samus_x_extra_run_speed = 0;
  samus_x_extra_run_subspeed = 0;
  input_to_pose_calc = 0;
}

void Samus_Movement_10_Moonwalking(void) {  // 0x90A694
  Samus_HandleMovement_X();
  Samus_Move_NoSpeedCalc_Y();
}

void Samus_Movement_11_SpringBallOnGround(void) {  // 0x90A69F
  if (!samus_x_accel_mode) {
    if (samus_pose_x_dir == 4) {
      if (samus_pose == kPose_7A_FaceL_Springball_Ground)
        goto LABEL_6;
    } else if (samus_pose == kPose_79_FaceR_Springball_Ground) {
LABEL_6:
      Samus_Move_NoBaseSpeed_X();
      if (!(Samus_CheckAndMoveY() & 1)) {
        Samus_Move_NoSpeedCalc_Y();
        Samus_CancelSpeedBoost();
        samus_x_extra_run_speed = 0;
        samus_x_extra_run_subspeed = 0;
        samus_x_base_speed = 0;
        samus_x_base_subspeed = 0;
        samus_x_accel_mode = 0;
      }
      return;
    }
  }
  Samus_HandleMovement_X();
  if (!(Samus_CheckAndMoveY() & 1))
    Samus_Move_NoSpeedCalc_Y();
}

void Samus_Movement_12_SpringBallInAir(void) {  // 0x90A6F1
  if (used_for_ball_bounce_on_landing)
    Samus_MorphedBouncingMovement();
  else
    Samus_JumpingMovement();
}

void Samus_Movement_13_SpringBallFalling(void) {  // 0x90A703
  if ((joypad1_lastkeys & (kButton_Left | kButton_Right)) == 0 && !samus_x_accel_mode) {
    Samus_CancelSpeedBoost();
    samus_x_extra_run_speed = 0;
    samus_x_extra_run_subspeed = 0;
    samus_x_base_speed = 0;
    samus_x_base_subspeed = 0;
    samus_x_accel_mode = 0;
  }
  if (used_for_ball_bounce_on_landing)
    Samus_MorphedBouncingMovement();
  else
    Samus_MorphedFallingMovement();
}

void Samus_Movement_14_WallJumping(void) {  // 0x90A734
  if (sign16(samus_anim_frame - 23)) {
    if (!sign16(samus_anim_frame - 3) && !sign16(flare_counter - 60))
      samus_contact_damage_index = 4;
  } else {
    samus_contact_damage_index = 3;
  }
  Samus_JumpingMovement();
}

void Samus_Movement_15_RanIntoWall(void) {  // 0x90A75F
  Samus_Move_NoBaseSpeed_X();
  Samus_Move_NoSpeedCalc_Y();
  Samus_CancelSpeedBoost();
  samus_x_extra_run_speed = 0;
  samus_x_extra_run_subspeed = 0;
  samus_x_base_speed = 0;
  samus_x_base_subspeed = 0;
  samus_x_accel_mode = 0;
}

void Samus_Movement_16_Grappling(void) {  // 0x90A780
  if (input_to_pose_calc != 5)
    input_to_pose_calc = 0;
}

void Samus_Movement_17_TurningAroundJumping(void) {  // 0x90A790
  Samus_HandleMovement_X();
  if (!(Samus_CheckAndMoveY() & 1))
    Samus_Move_NoSpeedCalc_Y();
  Samus_CancelSpeedBoost();
  samus_x_extra_run_speed = 0;
  samus_x_extra_run_subspeed = 0;
  input_to_pose_calc = 0;
}

void Samus_Movement_18_TurningAroundFalling(void) {  // 0x90A7AD
  Samus_HandleMovement_X();
  if (!(Samus_CheckAndMoveY() & 1))
    Samus_Move_NoSpeedCalc_Y();
  Samus_CancelSpeedBoost();
  samus_x_extra_run_speed = 0;
  samus_x_extra_run_subspeed = 0;
  input_to_pose_calc = 0;
}

void Samus_Movement_19_DamageBoost(void) {  // 0x90A7CA
  Samus_JumpingMovement();
}

void Samus_Movement_1A_GrabbedByDraygon(void) {  // 0x90A7D2
  input_to_pose_calc = 0;
}

void Samus_Movement_1B_ShinesparkEtc(void) {  // 0x90A7DA
  input_to_pose_calc = 0;
}

void DisableMinimapAndMarkBossRoomAsExplored(void) {  // 0x90A7E2
  debug_disable_minimap = 1;
  uint16 v0 = 0;
  do {
    int v1 = v0 >> 1;
    hud_tilemap[v1 + 26] = 11295;
    hud_tilemap[v1 + 58] = 11295;
    hud_tilemap[v1 + 90] = 11295;
    v0 += 2;
  } while ((int16)(v0 - 10) < 0);
  uint16 v2 = 5;
  while (boss_id != g_stru_90A83A[v2].boss_id_) {
    if ((--v2 & 0x8000u) != 0)
      return;
  }
  for (int i = g_stru_90A83A[v2].ptrs; ; i += 4) {
    uint16 *v4 = (uint16 *)RomPtr_90(i);
    if ((*v4 & 0x8000u) != 0)
      break;
    R18_ = *v4;
    R24_ = v4[1];
    MarkMapTileAsExplored();
  }
}

static const uint8 kShr0x80[8] = { 0x80, 0x40, 0x20, 0x10, 8, 4, 2, 1 };
static const uint16 kShr0xFc00[8] = { 0xfc00, 0x7e00, 0x3f00, 0x1f80, 0xfc0, 0x7e0, 0x3f0, 0x1f8 };

void MarkMapTileAsExplored(void) {  // 0x90A8A6
  uint8 v0 = (uint16)(R18_ & 0xFF00) >> 8;
  R34 = (room_x_coordinate_on_map + v0) & 0x20;
  R18_ = ((uint8)room_x_coordinate_on_map + v0) & 0x1F;
  R20_ = (uint8)((room_x_coordinate_on_map + v0) & 0x1F) >> 3;
  R22_ = room_y_coordinate_on_map + (uint8)((uint16)(R24_ & 0xFF00) >> 8) + 1;
  map_tiles_explored[(uint16)(R20_ + 4 * (R34 + R22_))] |= kShr0x80[((uint8)room_x_coordinate_on_map + v0) & 7];
}

void InitializeMiniMapBroken(void) {  // 0x90A8EF
  R18_ = room_x_coordinate_on_map + (uint8)((uint16)(samus_x_pos & 0xFF00) >> 8);
  R20_ = R18_ >> 3;
  R22_ = room_y_coordinate_on_map + (uint8)((uint16)(samus_y_pos & 0xFF00) >> 8) + 1;
  UpdateMinimapInside();
}


void UpdateMinimap(void) {  // 0x90A91B
  int16 v4;
  uint16 v5;
  int16 v10;
  int16 v11;

  if (!debug_disable_minimap
      && (uint16)(samus_x_pos >> 4) < room_width_in_blocks
      && (uint16)(samus_y_pos >> 4) < room_height_in_blocks) {
    R46 = 0;
    uint8 v0 = (uint16)(samus_x_pos & 0xFF00) >> 8;
    R34 = (room_x_coordinate_on_map + v0) & 0x20;
    R18_ = ((uint8)room_x_coordinate_on_map + v0) & 0x1F;
    uint16 v1 = ((uint8)room_x_coordinate_on_map + v0) & 7;
    R20_ = (uint8)((room_x_coordinate_on_map + v0) & 0x1F) >> 3;
    R22_ = room_y_coordinate_on_map + (uint8)((uint16)(samus_y_pos & 0xFF00) >> 8) + 1;
    uint16 v2 = R20_ + 4 * (R34 + R22_);
    map_tiles_explored[v2] |= kShr0x80[((uint8)room_x_coordinate_on_map + v0) & 7];
    R32 = v1;
    g_word_7E001E = v2;
    uint16 v3 = v2 - 4;
    v4 = v1 - 2;
    if ((int16)(v1 - 2) < 0) {
      v4 &= 7u;
      --v3;
      ++R46;
    }
    R50 = v3;
    bg3_tilemap_offset = 2 * v4;
    LOBYTE(v5) = HIBYTE(*(uint16 *)&map_tiles_explored[v3]);
    HIBYTE(v5) = *(uint16 *)&map_tiles_explored[v3];
    int v6 = bg3_tilemap_offset >> 1;
    R24_ = kShr0xFc00[v6] & v5;
    LOBYTE(v5) = HIBYTE(*(uint16 *)&map_tiles_explored[v3 + 4]);
    HIBYTE(v5) = *(uint16 *)&map_tiles_explored[v3 + 4];
    R26_ = kShr0xFc00[v6] & v5;
    LOBYTE(v5) = HIBYTE(*(uint16 *)&map_tiles_explored[v3 + 8]);
    HIBYTE(v5) = *(uint16 *)&map_tiles_explored[v3 + 8];
    R28_ = kShr0xFc00[v6] & v5;
    *(uint16 *)((char *)&R10_ + 1) = 130;
    *(uint16 *)((char *)&R8_ + 1) = kPauseMenuMapData[area_index];
    *(uint16 *)((char *)&R14_ + 1) = *(uint16 *)((char *)&R8_ + 1);
    *(uint16 *)((char *)&R8_ + 1) += v3;
    uint8 *v7 = IndirPtr((char *)&R8_ + 1, 0);
    LOBYTE(v5) = HIBYTE(*(uint16 *)v7);
    HIBYTE(v5) = *(uint16 *)v7;
    R38 = v5;
    *(uint16 *)((char *)&R8_ + 1) += 4;
    uint8 *v8 = IndirPtr((char *)&R8_ + 1, 0);
    LOBYTE(v5) = HIBYTE(*(uint16 *)v8);
    HIBYTE(v5) = *(uint16 *)v8;
    R40 = v5;
    *(uint16 *)((char *)&R8_ + 1) += 4;
    uint8 *v9 = IndirPtr((char *)&R8_ + 1, 0);
    LOBYTE(v5) = HIBYTE(*(uint16 *)v9);
    HIBYTE(v5) = *(uint16 *)v9;
    R42 = v5;
    if ((R50 & 3) == 3) {
      v10 = R46 ? bg3_tilemap_offset >> 1 : R32;
      if (!sign16(v10 - 6)) {
        if (R34)
          LOBYTE(R48) = R50 - 124;
        else
          LOBYTE(R48) = R50 + 125;
        AdjustMapBitsForMapPageSpill();
      }
    }
    v11 = bg3_tilemap_offset >> 1;
    for (int i = bg3_tilemap_offset >> 1 == 0; !i; i = v11 == 0) {
      R24_ *= 2;
      R38 *= 2;
      R26_ *= 2;
      R40 *= 2;
      R28_ *= 2;
      R42 *= 2;
      --v11;
    }
    UpdateMinimapInside();
  }
}

void UpdateMinimapInside(void) {  // 0x90AA43
  uint16 v0;
  int16 v1;
  int16 v5;
  int16 v7;
  int16 v8;

  LOBYTE(v0) = (uint16)(R34 + R22_) >> 8;
  HIBYTE(v0) = R34 + R22_;
  remaining_enemy_spritemap_entries = R18_ + (v0 >> 3);
  if (R34 && sign16((remaining_enemy_spritemap_entries & 0x1F) - 2))
    v1 = remaining_enemy_spritemap_entries - 1026;
  else
    v1 = remaining_enemy_spritemap_entries - 34;
  uint16 v2 = 2 * v1;
  *(uint16 *)&byte_7E0002 = *(uint16 *)((char *)kPauseMenuMapTilemaps + 2 + (uint16)(3 * area_index));
  R3_.bank = *(uint16 *)&byte_7E0002;
  R8_ = *(uint16 *)&byte_7E0002;
  R0_.addr = *(uint16 *)((char *)kPauseMenuMapTilemaps + (uint16)(3 * area_index));
  R3_.addr = R0_.addr + 64;
  R6_ = R0_.addr + 128;
  R18_ = 5;
  uint16 v3 = 0;
  do {
    bool v4 = R38 >> 15;
    R38 *= 2;
    if (!v4 || (v5 = *(uint16 *)IndirPtr(&R0_, v2), !has_area_map))
      v5 = 31;
    int v6 = v3 >> 1;
    hud_tilemap[v6 + 26] = v5 & 0xC3FF | 0x2C00;
    v4 = R24_ >> 15;
    R24_ *= 2;
    if (v4)
      hud_tilemap[v6 + 26] = *(uint16 *)IndirPtr(&R0_, v2) & 0xC3FF | 0x2800;
    v4 = R40 >> 15;
    R40 *= 2;
    if (!v4 || (v7 = *(uint16 *)IndirPtr(&R3_, v2), !has_area_map))
      v7 = 31;
    hud_tilemap[v6 + 58] = v7 & 0xC3FF | 0x2C00;
    v4 = R26_ >> 15;
    R26_ *= 2;
    if (v4) {
      hud_tilemap[v6 + 58] = *(uint16 *)IndirPtr(&R3_, v2) & 0xC3FF | 0x2800;
      if (R18_ == 3 && (hud_tilemap[v6 + 58] & 0x1FF) == 40)
        MarkMapTileAboveSamusAsExplored();
    }
    v4 = R42 >> 15;
    R42 *= 2;
    if (!v4 || (v8 = *(uint16 *)IndirPtr(&R6_, v2), !has_area_map))
      v8 = 31;
    hud_tilemap[v6 + 90] = v8 & 0xC3FF | 0x2C00;
    v4 = R28_ >> 15;
    R28_ *= 2;
    if (v4)
      hud_tilemap[v6 + 90] = *(uint16 *)IndirPtr(&R6_, v2) & 0xC3FF | 0x2800;
    v3 += 2;
    v2 += 2;
    if ((v2 & 0x3F) == 0)
      v2 += 1984;
    --R18_;
  } while (R18_);
  if ((nmi_frame_counter_byte & 8) == 0)
    hud_tilemap[60] |= 0x1C00u;
}

void MarkMapTileAboveSamusAsExplored(void) {  // 0x90AB5F
  *((uint8 *)&music_data_index + g_word_7E001E) |= kShr0x80[R32];
}

void AdjustMapBitsForMapPageSpill(void) {  // 0x90AB75
  uint16 v0 = (uint8)R48;
  *(uint16 *)((char *)&R8_ + 1) = *(uint16 *)((char *)&R14_ + 1) + (uint8)R48;
  LOBYTE(R44) = map_tiles_explored[(uint8)R48];
  HIBYTE(R44) = *IndirPtr((char *)&R8_ + 1, 0);
  if ((uint8)R34 == 32) {
    uint16 v1 = R44;
    HIBYTE(R38) = HIBYTE(v1);
    HIBYTE(R24_) = v1;
  } else {
    uint16 v2 = R44;
    LOBYTE(R38) = HIBYTE(v2);
    LOBYTE(R24_) = v2;
  }
  LOBYTE(R44) = map_tiles_explored[v0 + 4];
  *(uint16 *)((char *)&R8_ + 1) += 4;
  HIBYTE(R44) = *IndirPtr((char *)&R8_ + 1, 0);
  if ((uint8)R34 == 32) {
    uint16 v3 = R44;
    HIBYTE(R40) = HIBYTE(v3);
    HIBYTE(R26_) = v3;
  } else {
    uint16 v4 = R44;
    LOBYTE(R40) = HIBYTE(v4);
    LOBYTE(R26_) = v4;
  }
  LOBYTE(R44) = map_tiles_explored[v0 + 8];
  *(uint16 *)((char *)&R8_ + 1) += 4;
  HIBYTE(R44) = *IndirPtr((char *)&R8_ + 1, 0);
  if ((uint8)R34 == 32) {
    uint16 v5 = R44;
    HIBYTE(R42) = HIBYTE(v5);
    HIBYTE(R28_) = v5;
  } else {
    uint16 v6 = R44;
    LOBYTE(R42) = HIBYTE(v6);
    LOBYTE(R28_) = v6;
  }
}

void Samus_HandleCooldown(void) {  // 0x90AC1C
  if (time_is_frozen_flag) {
    cooldown_timer = 32;
  } else if (cooldown_timer) {
    if ((cooldown_timer & 0x8000u) != 0 || (--cooldown_timer, (cooldown_timer & 0x8000u) != 0))
      cooldown_timer = 0;
  }
}

uint8 Samus_CanFireBeam(void) {  // 0x90AC39
  if (!sign16(projectile_counter - 5) || (uint8)cooldown_timer)
    return 0;
  cooldown_timer = 1;
  ++projectile_counter;
  return 1;
}

uint8 Samus_CanFireSuperMissile(void) {  // 0x90AC5A
  if (hud_item_index != 2) {
    if (!sign16(projectile_counter - 5))
      return 0;
LABEL_3:
    if (!(uint8)cooldown_timer) {
      cooldown_timer = 1;
      ++projectile_counter;
      return 1;
    }
    return 0;
  }
  if (sign16(projectile_counter - 4))
    goto LABEL_3;
  return 0;
}


void UpdateBeamTilesAndPalette(void) {  // 0x90AC8D
  uint16 v0 = 2 * (equipped_beams & 0xFFF);
  uint16 v1 = vram_write_queue_tail;
  gVramWriteEntry(vram_write_queue_tail)->size = 256;
  v1 += 2;
  gVramWriteEntry(v1)->size = kBeamTilePtrs[v0 >> 1];
  v1 += 2;
  LOBYTE(gVramWriteEntry(v1++)->size) = -102;
  gVramWriteEntry(v1)->size = addr_unk_606300;
  vram_write_queue_tail = v1 + 2;
  WriteBeamPalette_Y(v0);
}

void WriteBeamPalette_A(uint16 a) {  // 0x90ACC2
  WriteBeamPalette_Y(2 * (a & 0xFFF));
}

void WriteBeamPalette_Y(uint16 j) {  // 0x90ACCD
  R0_.bank = 0x90;
  R0_.addr = kBeamPalettePtrs[j >> 1];
  uint16 v1 = 0;
  uint16 v2 = 0;
  do {
    palette_buffer[(v2 >> 1) + 224] = *(uint16 *)IndirPtr(&R0_, v1);
    v2 += 2;
    v1 += 2;
  } while ((int16)(v1 - 32) < 0);
}

void LoadProjectilePalette(uint16 a) {  // 0x90ACFC
  R0_.bank = 0x90;
  R0_.addr = kBeamPalettePtrs[a & 0xFFF];
  uint16 v1 = 0;
  uint16 v2 = 0;
  do {
    palette_buffer[(v2 >> 1) + 224] = *(uint16 *)IndirPtr(&R0_, v1);
    v2 += 2;
    v1 += 2;
  } while ((int16)(v1 - 32) < 0);
}

void ResetProjectileData(void) {  // 0x90AD22
  uint16 v0 = 0;
  do {
    int v1 = v0 >> 1;
    projectile_timers[v1] = 0;
    projectile_x_pos[v1] = 0;
    projectile_y_pos[v1] = 0;
    projectile_dir[v1] = 0;
    projectile_bomb_x_speed[v1] = 0;
    projectile_bomb_y_speed[v1] = 0;
    projectile_x_radius[v1] = 0;
    projectile_y_radius[v1] = 0;
    projectile_type[v1] = 0;
    projectile_damage[v1] = 0;
    projectile_bomb_instruction_ptr[v1] = 0;
    projectile_bomb_instruction_timers[v1] = 0;
    projectile_variables[v1] = 0;
    projectile_spritemap_pointers[v1] = 0;
    projectile_bomb_pre_instructions[v1] = FUNC16(ProjPreInstr_Empty);
    v0 += 2;
  } while ((int16)(v0 - 20) < 0);
  bomb_counter = 0;
  cooldown_timer = 0;
  projectile_counter = 0;
  power_bomb_flag = 0;
  if (hud_auto_cancel_flag) {
    hud_item_index = 0;
    samus_auto_cancel_hud_item_index = 0;
  }
  speed_echo_xpos[0] = 0;
  speed_echo_xpos[1] = 0;
  speed_echo_xpos[2] = 0;
  speed_echo_xpos[3] = 0;
  speed_echo_ypos[0] = 0;
  speed_echo_ypos[1] = 0;
  speed_echo_ypos[2] = 0;
  speed_echo_ypos[3] = 0;
  speed_echo_xspeed[0] = 0;
  speed_echo_xspeed[1] = 0;
  speed_echo_xspeed[2] = 0;
  speed_echo_xspeed[3] = 0;
  speed_echoes_index = 0;
  if (samus_special_super_palette_flags) {
    samus_special_super_palette_flags = 0;
    Samus_LoadSuitTargetPalette();
  }
  if (hyper_beam_flag)
    SpawnPalfxObject(addr_stru_8DE1F0);
}

void ClearProjectile(uint16 k) {  // 0x90ADB7
  int v1 = k >> 1;
  projectile_x_pos[v1] = 0;
  projectile_y_pos[v1] = 0;
  projectile_bomb_x_subpos[v1] = 0;
  projectile_bomb_y_subpos[v1] = 0;
  projectile_dir[v1] = 0;
  projectile_bomb_x_speed[v1] = 0;
  projectile_bomb_y_speed[v1] = 0;
  projectile_x_radius[v1] = 0;
  projectile_y_radius[v1] = 0;
  projectile_type[v1] = 0;
  projectile_damage[v1] = 0;
  projectile_bomb_instruction_ptr[v1] = 0;
  projectile_bomb_instruction_timers[v1] = 0;
  projectile_variables[v1] = 0;
  projectile_spritemap_pointers[v1] = 0;
  projectile_bomb_pre_instructions[v1] = FUNC16(ProjPreInstr_Empty);
  if ((int16)(k - 10) >= 0) {
    if ((--bomb_counter & 0x8000u) != 0)
      bomb_counter = 0;
  } else if ((--projectile_counter & 0x8000u) != 0) {
    projectile_counter = 0;
  }
}

static Func_Y_V *const kKillProjectileFuncs[10] = {  // 0x90AE06
  KillProjectileFunc_0,
  KillProjectileFunc_1,
  KillProjectileFunc_2,
  KillProjectileFunc_3,
  KillProjectileFunc_4,
  KillProjectileFunc_4,
  KillProjectileFunc_6,
  KillProjectileFunc_7,
  KillProjectileFunc_8,
  KillProjectileFunc_0,
};

void KillProjectile(uint16 k) {

  int v1 = k >> 1;
  if ((projectile_type[v1] & 0xF00) != 0) {
    if (!sign16((HIBYTE(projectile_type[v1]) & 0xF) - 3)) {
      ClearProjectile(k);
      return;
    }
  } else {
    kKillProjectileFuncs[projectile_dir[v1] & 0xF](k);
  }
  KillProjectileInner(k);
  projectile_bomb_pre_instructions[k >> 1] = FUNC16(ProjPreInstr_Empty);
}

void KillProjectileFunc_0(uint16 j) {  // 0x90AE4E
  projectile_y_pos[j >> 1] -= projectile_y_radius[j >> 1];
}

void KillProjectileFunc_1(uint16 j) {  // 0x90AE59
  int v1 = j >> 1;
  projectile_x_pos[v1] += projectile_x_radius[v1];
  projectile_y_pos[v1] -= projectile_y_radius[v1];
}

void KillProjectileFunc_2(uint16 j) {  // 0x90AE6E
  projectile_x_pos[j >> 1] += projectile_x_radius[j >> 1];
}

void KillProjectileFunc_3(uint16 j) {  // 0x90AE79
  int v1 = j >> 1;
  projectile_x_pos[v1] += projectile_x_radius[v1];
  projectile_y_pos[v1] += projectile_y_radius[v1];
}

void KillProjectileFunc_4(uint16 j) {  // 0x90AE8E
  projectile_y_pos[j >> 1] += projectile_y_radius[j >> 1];
}

void KillProjectileFunc_6(uint16 j) {  // 0x90AE99
  int v1 = j >> 1;
  projectile_x_pos[v1] -= projectile_x_radius[v1];
  projectile_y_pos[v1] += projectile_y_radius[v1];
}

void KillProjectileFunc_7(uint16 j) {  // 0x90AEAE
  projectile_x_pos[j >> 1] -= projectile_x_radius[j >> 1];
}

void KillProjectileFunc_8(uint16 j) {  // 0x90AEB9
  int v1 = j >> 1;
  projectile_x_pos[v1] -= projectile_x_radius[v1];
  projectile_y_pos[v1] -= projectile_y_radius[v1];
}

void CallProjPreInstr(uint32 ea, uint16 k) {
  switch (ea) {
  case fnProjPreInstr_Empty: return;
  case fnProjPreInstr_Beam_NoWaveBeam: ProjPreInstr_Beam_NoWaveBeam(k); return;
  case fnProjPreInstr_Dir0459: ProjPreInstr_Dir0459(k); return;
  case fnProjPreInstr_Dir1368: ProjPreInstr_Dir1368(k); return;
  case fnProjPreInstr_Dir27: ProjPreInstr_Dir27(k); return;
  case fnProjPreInstr_Missile: ProjPreInstr_Missile(k); return;
  case fnProjPreInstr_Missile_Func0459: ProjPreInstr_Missile_Func0459(k); return;
  case fnProjPreInstr_Missile_Func1368: ProjPreInstr_Missile_Func1368(k); return;
  case fnProjPreInstr_Missile_Func27: ProjPreInstr_Missile_Func27(k); return;
  case fnProjPreInstr_SuperMissile: ProjPreInstr_SuperMissile(k); return;
  case fnProjPreInstr_SuperMissile_Func0459: ProjPreInstr_SuperMissile_Func0459(k); return;
  case fnProjPreInstr_SuperMissile_Func1368: ProjPreInstr_SuperMissile_Func1368(k); return;
  case fnProjPreInstr_SuperMissile_Func27: ProjPreInstr_SuperMissile_Func27(k); return;
  case fnProjPreInstr_Func1: ProjPreInstr_Func1(k); return;
  case fnProjPreInstr_Bomb: ProjPreInstr_Bomb(k); return;
  case fnProjPreInstr_PowerBomb: ProjPreInstr_PowerBomb(k); return;
  case fnProjPreInstr_WavePlasmaEtc: ProjPreInstr_WavePlasmaEtc(k); return;
  case fnProjPreInstr_BeamOrIceWave: ProjPreInstr_BeamOrIceWave(k); return;
  case fnProjPreInstr_Wave_Shared: ProjPreInstr_Wave_Shared(k); return;
  case fnProjPreInstr_WavePlasmaEtc_0459: ProjPreInstr_WavePlasmaEtc_0459(k); return;
  case fnProjPreInstr_WavePlasmaEtc_1368: ProjPreInstr_WavePlasmaEtc_1368(k); return;
  case fnProjPreInstr_WavePlasmaEtc_27: ProjPreInstr_WavePlasmaEtc_27(k); return;
  case fnProjPreInstr_HyperBeam: ProjPreInstr_HyperBeam(k); return;
  case fnProjPreInstr_IceSba: ProjPreInstr_IceSba(k); return;
  case fnProjPreInstr_IceSba2: ProjPreInstr_IceSba2(k); return;
  case fnProjPreInstr_SpeedEcho: ProjPreInstr_SpeedEcho(k); return;
  case fnProjPreInstr_PlasmaSba: ProjPreInstr_PlasmaSba(k); return;
  case fnProjPreInstr_PlasmaSbaFunc_0: ProjPreInstr_PlasmaSbaFunc_0(k); return;
  case fnProjPreInstr_PlasmaSbaFunc_1: ProjPreInstr_PlasmaSbaFunc_1(k); return;
  case fnProjPreInstr_PlasmaSbaFunc_2: ProjPreInstr_PlasmaSbaFunc_2(k); return;
  case fnProjPreInstr_SpreadBomb: ProjPreInstr_SpreadBomb(k); return;
  case fnProjPreInstr_WaveSba: ProjPreInstr_WaveSba(k); return;
  case fnProjPreInstr_SpazerSba: ProjPreInstr_SpazerSba(k); return;
  case fnProjPreInstr_SpazerSba_FuncA_0: ProjPreInstr_SpazerSba_FuncA_0(k); return;
  case fnProjPreInstr_SpazerSba_FuncA_1: ProjPreInstr_SpazerSba_FuncA_1(k); return;
  case fnProjPreInstr_SpazerSba_FuncA_2: ProjPreInstr_SpazerSba_FuncA_2(k); return;
  case fnProjPreInstr_SpazerSba_FuncA_3: ProjPreInstr_SpazerSba_FuncA_3(k); return;
  case fnProjPreInstr_SpazerSba_FuncB_0: ProjPreInstr_SpazerSba_FuncB_0(k); return;
  case fnProjPreInstr_SpazerSba_FuncB_1: ProjPreInstr_SpazerSba_FuncB_1(k); return;
  case fnProjPreInstr_SpazerSba_FuncB_2: ProjPreInstr_SpazerSba_FuncB_2(k); return;
  case fnProjPreInstr_EndOfSpazerSba: ProjPreInstr_EndOfSpazerSba(k); return;
  case fnProjPreInstr_UnknownProj8027: ProjPreInstr_UnknownProj8027(k); return;
  default: Unreachable();
  }
}

void HandleProjectile(void) {  // 0x90AECE
  projectile_index = 18;
  for (int i = 18; i >= 0; projectile_index = i) {
    int v1 = i >> 1;
    if (projectile_bomb_instruction_ptr[v1]) {
      CallProjPreInstr(projectile_bomb_pre_instructions[v1] | 0x900000, i);
      RunProjectileInstructions();
      i = projectile_index;
    }
    i -= 2;
  }
  temp_collision_DD2 = 0;
}

static const int16 kDirToVelMult16_X[10] = { 0, 16, 16, 16, 0, 0, -16, -16, -16, 0 };
static const int16 kDirToVelMult16_Y[10] = { -16, -16, 0, 16, 16, 16, 16, 0, -16, -16 };

static Func_Y_V *const kProjPreInstr_Beam_Funcs[10] = {  // 0x90AEF3
  ProjPreInstr_Dir0459,
  ProjPreInstr_Dir1368,
  ProjPreInstr_Dir27,
  ProjPreInstr_Dir1368,
  ProjPreInstr_Dir0459,
  ProjPreInstr_Dir0459,
  ProjPreInstr_Dir1368,
  ProjPreInstr_Dir27,
  ProjPreInstr_Dir1368,
  ProjPreInstr_Dir0459,
};

void ProjPreInstr_Beam_NoWaveBeam(uint16 k) {

  int v1 = k >> 1;
  if ((projectile_dir[v1] & 0xF0) != 0) {
    ClearProjectile(k);
  } else {
    if (projectile_timers[v1]-- == 1) {
      projectile_timers[v1] = 4;
      SpawnProjectileTrail(k);
      k = projectile_index;
    }
    int v3 = k >> 1;
    uint16 v4 = 2 * (projectile_dir[v3] & 0xF);
    int v5 = v4 >> 1;
    projectile_bomb_x_speed[v3] += kDirToVelMult16_X[v5];
    projectile_bomb_y_speed[v3] += kDirToVelMult16_Y[v5];
    kProjPreInstr_Beam_Funcs[v5](v4);
    DeleteProjectileIfFarOffScreen();
  }
}

void ProjPreInstr_Dir0459(uint16 k) {  // 0x90AF4A
  BlockCollNoWaveBeamVert(projectile_index);
}

void ProjPreInstr_Dir1368(uint16 k) {  // 0x90AF52
  uint16 v1 = projectile_index;
  if (!(BlockCollNoWaveBeamHoriz(projectile_index) & 1))
    BlockCollNoWaveBeamVert(v1);
}

void ProjPreInstr_Dir27(uint16 k) {  // 0x90AF60
  BlockCollNoWaveBeamHoriz(projectile_index);
}

static Func_Y_V *const kProjPreInstr_Missile_Funcs[10] = {  // 0x90AF68
  ProjPreInstr_Missile_Func0459,
  ProjPreInstr_Missile_Func1368,
  ProjPreInstr_Missile_Func27,
  ProjPreInstr_Missile_Func1368,
  ProjPreInstr_Missile_Func0459,
  ProjPreInstr_Missile_Func0459,
  ProjPreInstr_Missile_Func1368,
  ProjPreInstr_Missile_Func27,
  ProjPreInstr_Missile_Func1368,
  ProjPreInstr_Missile_Func0459,
};

void ProjPreInstr_Missile(uint16 k) {

  int v1 = k >> 1;
  if ((projectile_dir[v1] & 0xF0) != 0) {
    ClearProjectile(k);
  } else {
    if (projectile_timers[v1]-- == 1) {
      projectile_timers[v1] = 4;
      SpawnProjectileTrail(k);
      k = projectile_index;
    }
    int v3 = k >> 1;
    uint16 v4 = 2 * (projectile_dir[v3] & 0xF);
    int v5 = v4 >> 1;
    projectile_bomb_x_speed[v3] += kDirToVelMult16_X[v5];
    projectile_bomb_y_speed[v3] += kDirToVelMult16_Y[v5];
    Missile_Func1(k);
    kProjPreInstr_Missile_Funcs[v4 >> 1](v4);
    DeleteProjectileIfFarOffScreen();
  }
}

void ProjPreInstr_Missile_Func0459(uint16 k) {  // 0x90AFC7
  BlockCollMissileVert(projectile_index);
}

void ProjPreInstr_Missile_Func1368(uint16 k) {  // 0x90AFCF
  uint16 v1 = projectile_index;
  if (!(BlockCollMissileHoriz(projectile_index) & 1))
    BlockCollMissileVert(v1);
}

void ProjPreInstr_Missile_Func27(uint16 k) {  // 0x90AFDD
  BlockCollMissileHoriz(projectile_index);
}

static Func_Y_V *const kProjPreInstr_SuperMissile_Funcs[10] = {  // 0x90AFE5
  ProjPreInstr_SuperMissile_Func0459,
  ProjPreInstr_SuperMissile_Func1368,
  ProjPreInstr_SuperMissile_Func27,
  ProjPreInstr_SuperMissile_Func1368,
  ProjPreInstr_SuperMissile_Func0459,
  ProjPreInstr_SuperMissile_Func0459,
  ProjPreInstr_SuperMissile_Func1368,
  ProjPreInstr_SuperMissile_Func27,
  ProjPreInstr_SuperMissile_Func1368,
  ProjPreInstr_SuperMissile_Func0459,
};

void ProjPreInstr_SuperMissile(uint16 k) {
  int16 v3;

  int v1 = k >> 1;
  if ((projectile_dir[v1] & 0xF0) != 0) {
    ClearProjectile(k);
LABEL_7:
    for (int i = 8; i >= 0; i -= 2) {
      if ((projectile_type[i >> 1] & 0xFFF) == 512)
        ClearProjectile(i);
    }
    return;
  }
  if (projectile_timers[v1]-- == 1) {
    projectile_timers[v1] = 2;
    SpawnProjectileTrail(k);
    k = projectile_index;
  }
  v3 = projectile_dir[k >> 1] & 0xF;
  Missile_Func1(k);
  kProjPreInstr_SuperMissile_Funcs[v3](v3);
  if (DeleteProjectileIfFarOffScreen() & 1)
    goto LABEL_7;
}

void ProjPreInstr_SuperMissile_Func0459(uint16 k) {  // 0x90B047
  BlockCollMissileVert(projectile_index);
  SuperMissileBlockCollDetect_Y();
}

void ProjPreInstr_SuperMissile_Func1368(uint16 k) {  // 0x90B052
  uint16 v1 = projectile_index;
  if (BlockCollMissileHoriz(projectile_index) & 1) {
    SuperMissileBlockCollDetect_X();
  } else {
    SuperMissileBlockCollDetect_X();
    BlockCollMissileVert(v1);
    SuperMissileBlockCollDetect_Y();
  }
}

void ProjPreInstr_SuperMissile_Func27(uint16 k) {  // 0x90B06A
  BlockCollMissileHoriz(projectile_index);
  SuperMissileBlockCollDetect_X();
}

void ProjPreInstr_Func1(uint16 k) {  // 0x90B075
  if ((projectile_dir[k >> 1] & 0xF0) != 0) {
    ClearProjectile(k);
    for (int i = 8; i >= 0; i -= 2) {
      if ((projectile_type[i >> 1] & 0xFFF) == 512)
        ClearProjectile(i);
    }
  }
}

void ProjPreInstr_Bomb(uint16 k) {  // 0x90B099
  if ((projectile_dir[k >> 1] & 0xF0) != 0) {
    ClearProjectile(k);
  } else {
    Bomb_Func2();
    BombOrPowerBomb_Func1(k);
  }
}

void ProjPreInstr_PowerBomb(uint16 k) {  // 0x90B0AE
  if ((projectile_dir[k >> 1] & 0xF0) != 0) {
    ClearProjectile(k);
  } else {
    PowerBomb_Func3();
    BombOrPowerBomb_Func1(k);
  }
}

void ProjPreInstr_WavePlasmaEtc(uint16 k) {  // 0x90B0C3
  int v1 = k >> 1;
  if ((projectile_dir[v1] & 0xF0) != 0) {
    ClearProjectile(k);
  } else if (projectile_timers[v1]-- == 1) {
    projectile_timers[v1] = 4;
    SpawnProjectileTrail(k);
    ProjPreInstr_Wave_Shared(projectile_index);
  } else {
    ProjPreInstr_Wave_Shared(k);
  }
}

void ProjPreInstr_BeamOrIceWave(uint16 k) {  // 0x90B0E4
  int v1 = k >> 1;
  if ((projectile_dir[v1] & 0xF0) != 0) {
    ClearProjectile(k);
  } else if (projectile_timers[v1]-- == 1) {
    projectile_timers[v1] = 3;
    SpawnProjectileTrail(k);
    ProjPreInstr_Wave_Shared(projectile_index);
  } else {
    ProjPreInstr_Wave_Shared(k);
  }
}

static Func_Y_V *const kProjPreInstr_WavePlasmaEtcFuncs[10] = {  // 0x90B103
  ProjPreInstr_WavePlasmaEtc_0459,
  ProjPreInstr_WavePlasmaEtc_1368,
  ProjPreInstr_WavePlasmaEtc_27,
  ProjPreInstr_WavePlasmaEtc_1368,
  ProjPreInstr_WavePlasmaEtc_0459,
  ProjPreInstr_WavePlasmaEtc_0459,
  ProjPreInstr_WavePlasmaEtc_1368,
  ProjPreInstr_WavePlasmaEtc_27,
  ProjPreInstr_WavePlasmaEtc_1368,
  ProjPreInstr_WavePlasmaEtc_0459,
};

void ProjPreInstr_Wave_Shared(uint16 k) {

  int v1 = k >> 1;
  uint16 v2 = 2 * (projectile_dir[v1] & 0xF);
  int v3 = v2 >> 1;
  projectile_bomb_x_speed[v1] += kDirToVelMult16_X[v3];
  projectile_bomb_y_speed[v1] += kDirToVelMult16_Y[v3];
  kProjPreInstr_WavePlasmaEtcFuncs[v3](v2);
  DeleteProjectileIfFarOffScreen();
}

void ProjPreInstr_WavePlasmaEtc_0459(uint16 k) {  // 0x90B13B
  BlockCollWaveBeamVert(projectile_index);
}

void ProjPreInstr_WavePlasmaEtc_1368(uint16 k) {  // 0x90B143
  uint16 v1 = projectile_index;
  if (!(BlockCollWaveBeamHoriz(projectile_index) & 1))
    BlockCollWaveBeamVert(v1);
}

void ProjPreInstr_WavePlasmaEtc_27(uint16 k) {  // 0x90B151
  BlockCollWaveBeamHoriz(projectile_index);
}

void ProjPreInstr_HyperBeam(uint16 k) {  // 0x90B159
  if ((projectile_dir[k >> 1] & 0xF0) != 0)
    ClearProjectile(k);
  else
    ProjPreInstr_Wave_Shared(k);
}

uint8 DeleteProjectileIfFarOffScreen(void) {  // 0x90B16A
  int16 v0;
  int16 v2;

  v0 = projectile_x_pos[projectile_index >> 1] - layer1_x_pos;
  if (!sign16(v0 + 64)) {
    if (sign16(v0 - 320)) {
      v2 = projectile_y_pos[projectile_index >> 1] - layer1_y_pos;
      if (!sign16(v2 + 64)) {
        if (sign16(v2 - 320))
          return 0;
      }
    }
  }
  ClearProjectile(projectile_index);
  return 1;
}

static const uint16 kInitializeProjectileSpeed_XY_Diag[24] = {  // 0x90B197
  0x400, 0x2ab,
  0x400, 0x2ab,
  0x400, 0x2ab,
  0x400, 0x2ab,
  0x400, 0x2ab,
  0x400, 0x2ab,
  0x400, 0x2ab,
  0x400, 0x2ab,
  0x400, 0x2ab,
  0x400, 0x2ab,
  0x400, 0x2ab,
  0x400, 0x2ab,
};

void SetInitialProjectileSpeed(void) {
  int16 v2;

  int v0 = R20_ >> 1;
  uint16 v1 = 4 * (projectile_type[v0] & 0xF);
  v2 = 2 * (projectile_dir[v0] & 0xF);
  if (!v2 || v2 == 4 || v2 == 8 || v2 == 10 || v2 == 14 || v2 == 18) {
    R22_ = kInitializeProjectileSpeed_XY_Diag[v1 >> 1];
  } else {
    if (v2 != 2 && v2 != 6 && v2 != 12 && v2 != 16) {
      Unreachable();
      while (1)
        ;
    }
    R22_ = kInitializeProjectileSpeed_XY_Diag[(v1 >> 1) + 1];
  }
  R18_ = R20_;
  InitializeProjectileSpeed(R20_);
}

void InitializeProjectileSpeedOfType(void) {  // 0x90B1DD
  R22_ = 0;
  R18_ = R20_;
  InitializeProjectileSpeed(R20_);
}

void InitializeProjectileSpeed(uint16 k) {  // 0x90B1F3
  int16 v2;

  int v1 = k >> 1;
  projectile_bomb_x_subpos[v1] = 0;
  projectile_bomb_y_subpos[v1] = 0;
  v2 = 2 * (projectile_dir[v1] & 0xF);
  switch (v2) {
  case 0:
  case 18: {
    uint16 v3 = R18_;
    if ((uint8)projectile_init_speed_samus_moved_up)
      R18_ = (*(uint16 *)((char *)&projectile_init_speed_samus_moved_right_fract + 1) >> 2) | 0xC000;
    else
      R18_ = 0;
    int v4 = v3 >> 1;
    projectile_bomb_y_speed[v4] = R18_ - R22_;
    projectile_bomb_x_speed[v4] = 0;
    break;
  }
  case 2: {
    uint16 v5 = R18_;
    if ((uint8)projectile_init_speed_samus_moved_up)
      R18_ = (*(uint16 *)((char *)&projectile_init_speed_samus_moved_right_fract + 1) >> 2) | 0xC000;
    else
      R18_ = 0;
    int v6 = v5 >> 1;
    projectile_bomb_y_speed[v6] = R18_ - R22_;
    projectile_bomb_x_speed[v6] = *(uint16 *)((char *)&projectile_init_speed_samus_moved_left_fract + 1) + R22_;
    break;
  }
  case 4: {
    int v7 = R18_ >> 1;
    projectile_bomb_y_speed[v7] = 0;
    projectile_bomb_x_speed[v7] = *(uint16 *)((char *)&projectile_init_speed_samus_moved_left_fract + 1) + R22_;
    break;
  }
  case 6: {
    int v8 = R18_ >> 1;
    projectile_bomb_y_speed[v8] = *(uint16 *)((char *)&projectile_init_speed_samus_moved_up_fract + 1) + R22_;
    projectile_bomb_x_speed[v8] = *(uint16 *)((char *)&projectile_init_speed_samus_moved_left_fract + 1) + R22_;
    break;
  }
  case 8:
  case 10: {
    int v9 = R18_ >> 1;
    projectile_bomb_y_speed[v9] = *(uint16 *)((char *)&projectile_init_speed_samus_moved_up_fract + 1) + R22_;
    projectile_bomb_x_speed[v9] = 0;
    break;
  }
  case 12: {
    int v10 = R18_ >> 1;
    projectile_bomb_y_speed[v10] = *(uint16 *)((char *)&projectile_init_speed_samus_moved_up_fract + 1) + R22_;
    projectile_bomb_x_speed[v10] = *(uint16 *)((char *)&absolute_moved_last_frame_y_fract + 1) - R22_;
    break;
  }
  case 14: {
    int v11 = R18_ >> 1;
    projectile_bomb_y_speed[v11] = 0;
    projectile_bomb_x_speed[v11] = *(uint16 *)((char *)&absolute_moved_last_frame_y_fract + 1) - R22_;
    break;
  }
  case 16: {
    uint16 v12 = R18_;
    if ((uint8)projectile_init_speed_samus_moved_up)
      R18_ = (*(uint16 *)((char *)&projectile_init_speed_samus_moved_right_fract + 1) >> 2) | 0xC000;
    else
      R18_ = 0;
    int v13 = v12 >> 1;
    projectile_bomb_y_speed[v13] = R18_ - R22_;
    projectile_bomb_x_speed[v13] = *(uint16 *)((char *)&absolute_moved_last_frame_y_fract + 1) - R22_;
    break;
  }
  default:
    Unreachable();
    while (1)
      ;
  }
}

void Missile_Func1(uint16 k) {  // 0x90B2F6
  static const uint16 word_90C301 = 0x100;
  uint16 v3;

  int v1 = k >> 1;
  if ((projectile_variables[v1] & 0xFF00) != 0) {
    R18_ = 4 * (projectile_dir[v1] & 0xF);
    if ((projectile_type[v1] & 0x200) != 0)
      v3 = R18_ - 0x3CD5;
    else
      v3 = R18_ - 0x3CFD;
    const uint8 *v4 = RomPtr_90(v3);
    projectile_bomb_x_speed[v1] += *(uint16 *)v4;
    projectile_bomb_y_speed[v1] += *((uint16 *)v4 + 1);
  } else {
    uint16 v2 = word_90C301 + projectile_variables[v1];
    projectile_variables[v1] = v2;
    if ((v2 & 0xFF00) != 0) {
      R22_ = projectile_variables[v1];
      R18_ = k;
      InitializeProjectileSpeed(k);
      if ((projectile_type[v1] & 0x200) != 0)
        Missile_Func2();
    }
  }
}

void SuperMissileBlockCollDetect_Y(void) {  // 0x90B366
  int16 v1;
  int16 v2;
  char v3; // t0

  int v0 = projectile_index >> 1;
  if ((projectile_type[v0] & 0xF00) == 512 || (projectile_type[v0] & 0xF00) == 2048) {
    uint8 v5 = projectile_variables[v0];
    if (*((uint8 *)projectile_variables + projectile_index + 1)) {
      v1 = abs16(projectile_bomb_y_speed[v0]) & 0xFF00;
      if (sign16(v1 - 2816)) {
        int v4 = projectile_index >> 1;
        if ((projectile_type[v4] & 0xF00) != 2048) {
          projectile_y_pos[v5 >> 1] = projectile_y_pos[v4];
          return;
        }
      } else {
        v3 = v1;
        LOBYTE(v2) = HIBYTE(v1);
        HIBYTE(v2) = v3;
        R18_ = v2 - 10;
        if ((projectile_bomb_y_speed[v0] & 0x8000u) == 0) {
          uint16 v6 = projectile_index;
          projectile_y_pos[v5 >> 1] = projectile_y_pos[v0] - R18_;
          projectile_index = v5;
          BlockCollMissileVert(v5);
          projectile_index = v6;
          if ((projectile_type[v6 >> 1] & 0xF00) == 2048)
            ClearProjectile(v5);
          return;
        }
        uint16 v7 = projectile_index;
        projectile_y_pos[v5 >> 1] = R18_ + projectile_y_pos[projectile_index >> 1];
        projectile_index = v5;
        BlockCollMissileVert(v5);
        projectile_index = v7;
        if ((projectile_type[v7 >> 1] & 0xF00) != 2048)
          return;
      }
      ClearProjectile(v5);
    }
  }
}

void Projectile_Func4(uint16 k) {  // 0x90B4A6
  int v1 = k >> 1;
  if (*((uint8 *)projectile_variables + k + 1)) {
    if ((projectile_type[v1] & 0xF00) == 2048)
      ClearProjectile((uint8)projectile_variables[v1]);
  }
}

void SuperMissileBlockCollDetect_X(void) {  // 0x90B406
  int16 v1;
  int16 v2;
  char v3; // t0

  int v0 = projectile_index >> 1;
  if ((projectile_type[v0] & 0xF00) == 512 || (projectile_type[v0] & 0xF00) == 2048) {
    uint8 v5 = projectile_variables[v0];
    if (*((uint8 *)projectile_variables + projectile_index + 1)) {
      v1 = abs16(projectile_bomb_x_speed[v0]) & 0xFF00;
      if (sign16(v1 - 2816)) {
        int v4 = projectile_index >> 1;
        if ((projectile_type[v4] & 0xF00) != 2048) {
          projectile_x_pos[v5 >> 1] = projectile_x_pos[v4];
          return;
        }
      } else {
        v3 = v1;
        LOBYTE(v2) = HIBYTE(v1);
        HIBYTE(v2) = v3;
        R18_ = v2 - 10;
        if ((projectile_bomb_x_speed[v0] & 0x8000u) == 0) {
          uint16 v6 = projectile_index;
          projectile_x_pos[v5 >> 1] = projectile_x_pos[v0] - R18_;
          projectile_index = v5;
          BlockCollMissileHoriz(v5);
          projectile_index = v6;
          if ((projectile_type[v6 >> 1] & 0xF00) == 2048)
            ClearProjectile(v5);
          return;
        }
        uint16 v7 = projectile_index;
        projectile_x_pos[v5 >> 1] = R18_ + projectile_x_pos[projectile_index >> 1];
        projectile_index = v5;
        BlockCollMissileHoriz(v5);
        projectile_index = v7;
        if ((projectile_type[v7 >> 1] & 0xF00) != 2048)
          return;
      }
      ClearProjectile(v5);
    }
  }
}

void ProjInstr_MoveLeftProjectileTrailDown(uint16 j) {  // 0x90B525
  ++projectiletrail_left_y_pos[j >> 1];
}

void ProjInstr_MoveRightProjectileTrailDown(uint16 j) {  // 0x90B587
  ++projectiletrail_right_y_pos[j >> 1];
}

void ProjInstr_MoveLeftProjectileTrailUp(uint16 j) {  // 0x90B5B3
  --projectiletrail_left_y_pos[j >> 1];
}

void SpawnProjectileTrail(uint16 k) {  // 0x90B657
  int16 v2;

  uint16 v1 = projectile_type[k >> 1];
  if ((v1 & 0xF00) != 0) {
    uint16 v3 = HIBYTE(v1) & 0xF;
    if (v3 >= 3u)
      return;
    v2 = v3 + 31;
  } else {
    v2 = projectile_type[k >> 1] & 0x3F;
  }
  uint16 v4 = 34;
  while (projectiletrail_left_instr_timer[v4 >> 1]) {
    v4 -= 2;
    if ((v4 & 0x8000u) != 0)
      return;
  }
  int v5 = v4 >> 1;
  projectiletrail_left_instr_timer[v5] = 1;
  projectiletrail_right_instr_timer[v5] = 1;
  int v6 = v2;
  projectiletrail_left_instr_list_ptr[v5] = off_90B5BB[v6];
  projectiletrail_right_instr_list_ptr[v5] = off_90B609[v6];
  ProjectileTrail_Func5(projectile_index, v4);
}

void CallProjInstr(uint32 ea, uint16 j) {
  switch (ea) {
  case fnProjInstr_MoveLeftProjectileTrailDown: ProjInstr_MoveLeftProjectileTrailDown(j); return;  // 0x90b525
  case fnProjInstr_MoveRightProjectileTrailDown: ProjInstr_MoveRightProjectileTrailDown(j); return;  // 0x90b587
  case fnProjInstr_MoveLeftProjectileTrailUp: ProjInstr_MoveLeftProjectileTrailUp(j); return;  // 0x90b5b3
  default: Unreachable();
  }
}

void HandleProjectileTrails(void) {  // 0x90B6A9
  int i;
  int16 v10;
  OamEnt *v11;
  int16 v12;
  uint16 j;
  int16 v22;
  OamEnt *v23;
  int16 v24;
  uint16 k;
  int16 v28;
  OamEnt *v29;
  int16 v30;
  int16 v33;
  OamEnt *v34;
  int16 v35;
  uint16 v6, v18;
  int v7;

  if (!time_is_frozen_flag) {
    uint16 v0 = 34;
    while (1) {
      uint16 v2;
      int v1;
      v1 = v0 >> 1;
      v2 = projectiletrail_left_instr_timer[v1];
      if (v2) {
        uint16 v3 = v2 - 1;
        projectiletrail_left_instr_timer[v1] = v3;
        if (v3)
          goto LABEL_10;
        for (i = projectiletrail_left_instr_list_ptr[v1]; ; i += 2) {
          uint16 *v5 = (uint16 *)RomPtr_90(i);
          v6 = *v5;
          if ((*v5 & 0x8000u) == 0)
            break;
          R18_ = *v5;
          CallProjInstr(v6 | 0x900000, v0);
        }
        v7 = v0 >> 1;
        projectiletrail_left_instr_timer[v7] = v6;
        if (v6)
          break;
      }
LABEL_14:;
      int v13 = v0 >> 1;
      uint16 v14 = projectiletrail_right_instr_timer[v13];
      if (v14) {
        uint16 v15 = v14 - 1;
        projectiletrail_right_instr_timer[v13] = v15;
        if (v15)
          goto LABEL_21;
        for (j = projectiletrail_right_instr_list_ptr[v13]; ; j += 2) {
          uint16 *v17 = (uint16 *)RomPtr_90(j);
          v18 = *v17;
          if ((*v17 & 0x8000u) == 0)
            break;
          R18_ = *v17;
          CallProjInstr(v18 | 0x900000, v0);
        }
        int v19;
        v19 = v0 >> 1;
        projectiletrail_right_instr_timer[v19] = v18;
        if (v18) {
          projectiletrail_right_tile_and_attribs[v19] = *((uint16 *)RomPtr_90(j) + 1);
          projectiletrail_right_instr_list_ptr[v19] = j + 4;
LABEL_21:;
          uint16 v20 = oam_next_ptr;
          if ((int16)(oam_next_ptr - 512) < 0) {
            int v21 = v0 >> 1;
            v22 = projectiletrail_right_x_pos[v21] - layer1_x_pos;
            if ((v22 & 0xFF00) == 0) {
              v23 = gOamEnt(oam_next_ptr);
              *(uint16 *)&v23->xcoord = v22;
              v24 = projectiletrail_right_y_pos[v21] - layer1_y_pos;
              if ((v24 & 0xFF00) == 0) {
                *(uint16 *)&v23->ycoord = v24;
                *(uint16 *)&v23->charnum = projectiletrail_right_tile_and_attribs[v21];
                oam_next_ptr = v20 + 4;
              }
            }
          }
        }
      }
      v0 -= 2;
      if ((v0 & 0x8000u) != 0)
        return;
    }
    projectiletrail_left_tile_and_attribs[v7] = *((uint16 *)RomPtr_90(i) + 1);
    projectiletrail_left_instr_list_ptr[v7] = i + 4;
LABEL_10:;
    uint16 v8 = oam_next_ptr;
    if ((int16)(oam_next_ptr - 512) < 0) {
      int v9 = v0 >> 1;
      v10 = projectiletrail_left_x_pos[v9] - layer1_x_pos;
      if ((v10 & 0xFF00) == 0) {
        v11 = gOamEnt(oam_next_ptr);
        *(uint16 *)&v11->xcoord = v10;
        v12 = projectiletrail_left_y_pos[v9] - layer1_y_pos;
        if ((v12 & 0xFF00) == 0) {
          *(uint16 *)&v11->ycoord = v12;
          *(uint16 *)&v11->charnum = projectiletrail_left_tile_and_attribs[v9];
          oam_next_ptr = v8 + 4;
        }
      }
    }
    goto LABEL_14;
  }
  for (k = 34; (k & 0x8000u) == 0; k -= 2) {
    uint16 v26 = oam_next_ptr;
    if ((int16)(oam_next_ptr - 512) < 0) {
      int v27 = k >> 1;
      if (projectiletrail_left_instr_timer[v27]) {
        v28 = projectiletrail_left_x_pos[v27] - layer1_x_pos;
        if ((v28 & 0xFF00) == 0) {
          v29 = gOamEnt(oam_next_ptr);
          *(uint16 *)&v29->xcoord = v28;
          v30 = projectiletrail_left_y_pos[v27] - layer1_y_pos;
          if ((v30 & 0xFF00) == 0) {
            *(uint16 *)&v29->ycoord = v30;
            *(uint16 *)&v29->charnum = projectiletrail_left_tile_and_attribs[v27];
            oam_next_ptr = v26 + 4;
          }
        }
      }
    }
    uint16 v31 = oam_next_ptr;
    if ((int16)(oam_next_ptr - 512) < 0) {
      int v32 = k >> 1;
      if (projectiletrail_right_instr_timer[v32]) {
        v33 = projectiletrail_right_x_pos[v32] - layer1_x_pos;
        if ((v33 & 0xFF00) == 0) {
          v34 = gOamEnt(oam_next_ptr);
          *(uint16 *)&v34->xcoord = v33;
          v35 = projectiletrail_right_y_pos[v32] - layer1_y_pos;
          if ((v35 & 0xFF00) == 0) {
            *(uint16 *)&v34->ycoord = v35;
            *(uint16 *)&v34->charnum = projectiletrail_right_tile_and_attribs[v32];
            oam_next_ptr = v31 + 4;
          }
        }
      }
    }
  }
}

void HudSelectionHandler_NothingOrPowerBombs(void) {  // 0x90B80D
  prev_beam_charge_counter = flare_counter;
  if (hyper_beam_flag || (equipped_beams & 0x1000) == 0) {
    if ((button_config_shoot_x & joypad1_lastkeys) != 0)
      FireUnchargedBeam();
    return;
  }
  if (new_projectile_direction_changed_pose) {
    if (sign16(flare_counter - 60)) {
LABEL_14:
      flare_counter = 0;
      ClearFlareAnimationState();
      FireUnchargedBeam();
      return;
    }
LABEL_15:
    flare_counter = 0;
    ClearFlareAnimationState();
    FireChargedBeam();
    return;
  }
  if ((button_config_shoot_x & joypad1_lastkeys) == 0) {
    if (!flare_counter)
      return;
    if (sign16(flare_counter - 60))
      goto LABEL_14;
    goto LABEL_15;
  }
  if (sign16(flare_counter - 120)) {
    if (++flare_counter == 1) {
      ClearFlareAnimationState();
      FireUnchargedBeam();
    }
  } else if (FireSba() & 1) {
    flare_counter = 0;
    ClearFlareAnimationState();
    Samus_LoadSuitPalette();
  }
}

static const uint16 kProjectileBombPreInstr[12] = {  // 0x90B887
  (uint16)fnProjPreInstr_Beam_NoWaveBeam,
  (uint16)fnProjPreInstr_BeamOrIceWave,
  (uint16)fnProjPreInstr_Beam_NoWaveBeam,
  (uint16)fnProjPreInstr_BeamOrIceWave,
  (uint16)fnProjPreInstr_Beam_NoWaveBeam,
  (uint16)fnProjPreInstr_WavePlasmaEtc,
  (uint16)fnProjPreInstr_Beam_NoWaveBeam,
  (uint16)fnProjPreInstr_WavePlasmaEtc,
  (uint16)fnProjPreInstr_Beam_NoWaveBeam,
  (uint16)fnProjPreInstr_WavePlasmaEtc,
  (uint16)fnProjPreInstr_Beam_NoWaveBeam,
  (uint16)fnProjPreInstr_WavePlasmaEtc,
};

void FireUnchargedBeam(void) {
  char v3;

  if (hyper_beam_flag) {
    FireHyperBeam();
    return;
  }
  if (Samus_CanFireBeam() & 1) {
    uint16 v0 = 0;
    while (projectile_damage[v0 >> 1]) {
      v0 += 2;
      if ((int16)(v0 - 10) >= 0) {
        v0 -= 2;
        break;
      }
    }
    R20_ = v0;
    if (!(InitProjectilePositionDirection() & 1)) {
      projectile_invincibility_timer = 10;
      uint16 v1 = R20_;
      int v2 = R20_ >> 1;
      projectile_timers[v2] = 4;
      v3 = equipped_beams;
      projectile_type[v2] = equipped_beams | 0x8000;
      QueueSfx1_Max15(kUnchargedProjectile_Sfx[v3 & 0xF]);
      play_resume_charging_beam_sfx = 0;
      InitializeProjectile(v1);
      if ((equipped_beams & 0x1000) != 0
          || (button_config_shoot_x & joypad1_newkeys) != 0
          || (button_config_shoot_x & joypad1_newinput_samusfilter) != 0) {
        uint16 v6;
        v6 = projectile_type[v2];
        cooldown_timer = kProjectileCooldown_Uncharged[v6 & 0x3F];
        if ((v6 & 1) == 0)
          goto LABEL_17;
      } else {
        uint16 v5;
        v5 = projectile_type[v2];
        cooldown_timer = kBeamAutoFireCooldowns[v5 & 0x3F];
        if ((v5 & 1) == 0) {
LABEL_17:
          projectile_bomb_x_speed[v2] = 0;
          projectile_bomb_y_speed[v2] = 0;
          projectile_index = v1;
          CheckBeamCollByDir(v1);
          v1 = projectile_index;
          if ((projectile_type[projectile_index >> 1] & 0xF00) != 0)
            return;
          goto LABEL_20;
        }
      }
      int v4;
      v4 = v1 >> 1;
      projectile_bomb_x_speed[v4] = 0;
      projectile_bomb_y_speed[v4] = 0;
      projectile_index = v1;
      WaveBeam_CheckColl(v1);
LABEL_20:
      R20_ = v1;
      projectile_bomb_pre_instructions[v1 >> 1] = kProjectileBombPreInstr[projectile_type[v1 >> 1] & 0xF];
      SetInitialProjectileSpeed();
      return;
    }
  }
  if (!sign16(prev_beam_charge_counter - 16)) {
    play_resume_charging_beam_sfx = 0;
    QueueSfx1_Max15(2u);
  }
}

static const uint16 kFireChargedBeam_Funcs[12] = {  // 0x90B986
  (uint16)fnProjPreInstr_Beam_NoWaveBeam,
  (uint16)fnProjPreInstr_WavePlasmaEtc,
  (uint16)fnProjPreInstr_Beam_NoWaveBeam,
  (uint16)fnProjPreInstr_WavePlasmaEtc,
  (uint16)fnProjPreInstr_Beam_NoWaveBeam,
  (uint16)fnProjPreInstr_WavePlasmaEtc,
  (uint16)fnProjPreInstr_Beam_NoWaveBeam,
  (uint16)fnProjPreInstr_WavePlasmaEtc,
  (uint16)fnProjPreInstr_Beam_NoWaveBeam,
  (uint16)fnProjPreInstr_WavePlasmaEtc,
  (uint16)fnProjPreInstr_Beam_NoWaveBeam,
  (uint16)fnProjPreInstr_WavePlasmaEtc,
};

void FireChargedBeam(void) {

  if (Samus_CanFireBeam() & 1) {
    uint16 v0 = 0;
    while (projectile_damage[v0 >> 1]) {
      v0 += 2;
      if ((int16)(v0 - 10) >= 0) {
        v0 -= 2;
        break;
      }
    }
    R20_ = v0;
    if (!(InitProjectilePositionDirection() & 1)) {
      projectile_invincibility_timer = 10;
      uint16 v1 = R20_;
      int v2 = R20_ >> 1;
      projectile_timers[v2] = 4;
      uint16 v3 = equipped_beams & 0x100F | 0x8010;
      projectile_type[v2] = v3;
      QueueSfx1_Max15(kChargedProjectile_Sfx[v3 & 0xF]);
      play_resume_charging_beam_sfx = 0;
      InitializeProjectile(v1);
      uint16 v5 = projectile_type[v2];
      cooldown_timer = kProjectileCooldown_Uncharged[v5 & 0x3F];
      if ((v5 & 1) != 0) {
        int v4 = v1 >> 1;
        projectile_bomb_x_speed[v4] = 0;
        projectile_bomb_y_speed[v4] = 0;
        projectile_index = v1;
        WaveBeam_CheckColl(v1);
      } else {
        projectile_bomb_x_speed[v2] = 0;
        projectile_bomb_y_speed[v2] = 0;
        projectile_index = v1;
        CheckBeamCollByDir(v1);
        v1 = projectile_index;
        if ((projectile_type[projectile_index >> 1] & 0xF00) != 0) {
LABEL_14:
          charged_shot_glow_timer = 4;
          return;
        }
      }
      R20_ = v1;
      projectile_bomb_pre_instructions[v1 >> 1] = kFireChargedBeam_Funcs[(uint16)(2
                                                                                  * (projectile_type[v1 >> 1] & 0xF)) >> 1];
      SetInitialProjectileSpeed();
      goto LABEL_14;
    }
  }
  if (!sign16(prev_beam_charge_counter - 16)) {
    play_resume_charging_beam_sfx = 0;
    QueueSfx1_Max15(2u);
  }
}

static const int16 kProjectileOriginOffsets3_X[10] = { 2, 13, 11, 13, 2, -5, -14, -11, -19, -2 };
static const int16 kProjectileOriginOffsets3_Y[10] = { -8, -13, 1, 4, 13, 13, 4, 1, -19, -8 };
static const int16 kProjectileOriginOffsets4_X[10] = { 2, 15, 15, 13, 2, -5, -13, -13, -15, -2 };
static const int16 kProjectileOriginOffsets4_Y[10] = { -8, -16, -2, 1, 13, 13, 1, -2, -16, -8 };

uint8 InitProjectilePositionDirection(void) {  // 0x90BA56
  uint16 v0 = samus_pose;
  uint16 direction_shots_fired;

  if (new_projectile_direction_changed_pose) {
    direction_shots_fired = (uint8)new_projectile_direction_changed_pose;
    new_projectile_direction_changed_pose = 0;
  } else {
    direction_shots_fired = kPoseParams[v0].direction_shots_fired;
    if ((direction_shots_fired & 0xF0) != 0) {
      if (direction_shots_fired != 16
          || (LOBYTE(direction_shots_fired) = *(&kPoseParams[0].direction_shots_fired
                                                + (uint16)(8 * samus_last_different_pose)),
              (direction_shots_fired & 0xF0) != 0)) {
        --projectile_counter;
        return 1;
      }
      direction_shots_fired = (uint8)direction_shots_fired;
      v0 = samus_pose;
    }
  }
  int v2 = R20_ >> 1;
  projectile_dir[v2] = direction_shots_fired;
  R22_ = kPoseParams[v0].y_offset_to_gfx;
  uint16 v3 = 2 * (projectile_dir[v2] & 0xF);
  if (samus_pose == kPose_75_FaceL_Moonwalk_AimUL
      || samus_pose == kPose_76_FaceR_Moonwalk_AimUR
      || samus_movement_type == 1) {
    int v6 = v3 >> 1;
    projectile_x_pos[v2] = samus_x_pos + kProjectileOriginOffsets4_X[v6];
    projectile_y_pos[v2] = samus_y_pos + kProjectileOriginOffsets4_Y[v6] - R22_;
    return 0;
  } else {
    int v4 = v3 >> 1;
    projectile_x_pos[v2] = samus_x_pos + kProjectileOriginOffsets3_X[v4];
    projectile_y_pos[v2] = samus_y_pos + kProjectileOriginOffsets3_Y[v4] - R22_;
    return 0;
  }
}

void HandleChargingBeamGfxAudio(void) {  // 0x90BAFC
  int16 v1;
  int16 v4;
  uint16 v9;

  if (hyper_beam_flag) {
    if (flare_counter) {
      for (int i = 4; i >= 0; i -= 2) {
        bool v7 = *(uint16 *)((char *)&flare_animation_timer + i) == 1;
        bool v8 = (-- * (uint16 *)((char *)&flare_animation_timer + i) & 0x8000u) != 0;
        if (v7 || v8) {
          v7 = (*(uint16 *)((char *)&flare_animation_frame + i))-- == 1;
          if (v7) {
            if (i == 4)
              flare_counter = 0;
          } else {
            *(uint16 *)((char *)&flare_animation_timer + i) = 3;
          }
        }
        DrawFlareAnimationComponent(i);
      }
    }
  } else if ((int16)flare_counter > 0) {
    if (flare_counter == 1) {
      flare_animation_frame = 0;
      flare_slow_sparks_anim_frame = 0;
      flare_fast_sparks_anim_frame = 0;
      flare_animation_timer = 3;
      flare_slow_sparks_anim_timer = 5;
      flare_fast_sparks_anim_timer = 4;
    }
    if (!sign16(flare_counter - 15)) {
      if (flare_counter == 16)
        QueueSfx1_Max9(8u);
      uint16 v0 = 0;
      do {
        byte_7E0002 = -112;
        v1 = *(uint16 *)((char *)&flare_animation_timer + v0) - 1;
        *(uint16 *)((char *)&flare_animation_timer + v0) = v1;
        if (v1 < 0) {
          uint16 v2 = *(uint16 *)((char *)&flare_animation_frame + v0) + 1;
          *(uint16 *)((char *)&flare_animation_frame + v0) = v2;
          uint16 v3 = v2;
          R0_.addr = kFlareAnimDelays[v0 >> 1];
          v4 = *IndirPtr(&R0_, v2);
          if (v4 == 255) {
            *(uint16 *)((char *)&flare_animation_frame + v0) = 0;
            v3 = 0;
          } else if (v4 == 254) {
            R18_ = *IndirPtr(&R0_, v3 + 1);
            uint16 v5 = *(uint16 *)((char *)&flare_animation_frame + v0) - R18_;
            *(uint16 *)((char *)&flare_animation_frame + v0) = v5;
            v3 = v5;
          }
          *(uint16 *)((char *)&flare_animation_timer + v0) = *IndirPtr(&R0_, v3);
        }
        v9 = v0;
        DrawFlareAnimationComponent(v0);
        if (sign16(flare_counter - 30))
          break;
        v0 += 2;
      } while ((int16)(v9 - 4) < 0);
    }
  }
}

static const int16 kProjectileOriginOffsets_X[13] = { 2, 18, 15, 17, 3, -4, -17, -15, -18, -2, -4, -4, -4 };
static const int16 kProjectileOriginOffsets_Y[13] = { -28, -19, 1, 6, 17, 17, 6, 1, -20, -28, -20, -2, 8 };
static const int16 kProjectileOriginOffsets2_X[10] = { 2, 19, 20, 18, 3, -4, -18, -20, -19, -2 };
static const int16 kProjectileOriginOffsets2_Y[10] = { -32, -22, -3, 6, 25, 25, 6, -3, -20, -32 };


void DrawFlareAnimationComponent(uint16 k) {  // 0x90BBE1
  static const uint16 word_93A225[3] = { 0, 0x1e, 0x24 };
  static const uint16 word_93A22B[3] = { 0, 0x2a, 0x30 };
  int16 v2;
  uint16 v1;

  byte_7E0002 = -109;
  R18_ = *((uint8 *)&flare_animation_frame + k);
  if (samus_pose_x_dir == 4)
    v1 = word_93A22B[k >> 1];
  else
    v1 = word_93A225[k >> 1];
  R22_ = R18_ + v1;
  R24_ = *(&kPoseParams[0].y_offset_to_gfx + (uint16)(8 * samus_pose));
  v2 = *(&kPoseParams[0].direction_shots_fired + (uint16)(8 * samus_pose));
  if (v2 != 255 && v2 != 16) {
    uint16 v3 = 2 * (v2 & 0xF);
    if ((ceres_status & 0x8000u) != 0) {
      grapple_beam_tmpD82 = samus_x_pos;
      grapple_beam_tmpD84 = samus_y_pos;
      Samus_CalcPos_Mode7();
    }
    int v4 = v3 >> 1;
    uint16 v5;
    if (samus_movement_type == 1) {
      R20_ = samus_x_pos + kProjectileOriginOffsets2_X[v4] - layer1_x_pos;
      v5 = (__PAIR32__(
        samus_y_pos + kProjectileOriginOffsets2_Y[v4] - R24_,
        samus_y_pos + kProjectileOriginOffsets2_Y[v4])
        - __PAIR32__(layer1_y_pos, R24_)) >> 16;
    } else {
      R20_ = samus_x_pos + kProjectileOriginOffsets_X[v4] - layer1_x_pos;
      v5 = (__PAIR32__(
        samus_y_pos + kProjectileOriginOffsets_Y[v4] - R24_,
        samus_y_pos + kProjectileOriginOffsets_Y[v4])
        - __PAIR32__(layer1_y_pos, R24_)) >> 16;
    }
    R18_ = v5;
    if ((v5 & 0xFF00) != 0)
      ;
    else
      DrawBeamGrappleSpritemap(R22_);
    if ((ceres_status & 0x8000u) != 0) {
      samus_y_pos = grapple_beam_tmpD84;
      samus_x_pos = grapple_beam_tmpD82;
    }
  }
}

void ClearFlareAnimationState(void) {  // 0x90BCBE
  flare_animation_frame = 0;
  flare_slow_sparks_anim_frame = 0;
  flare_fast_sparks_anim_frame = 0;
  flare_animation_timer = 0;
  flare_slow_sparks_anim_timer = 0;
  flare_fast_sparks_anim_timer = 0;
}

void FireHyperBeam(void) {  // 0x90BCD1
  if (Samus_CanFireBeam() & 1) {
    uint16 v0 = 0;
    while (projectile_damage[v0 >> 1]) {
      v0 += 2;
      if ((int16)(v0 - 10) >= 0) {
        v0 -= 2;
        break;
      }
    }
    R20_ = v0;
    if (!(InitProjectilePositionDirection() & 1)) {
      projectile_invincibility_timer = 10;
      uint16 k = R20_;
      projectile_type[R20_ >> 1] = -28648;
      QueueSfx1_Max15(kChargedProjectile_Sfx[8]);
      play_resume_charging_beam_sfx = 0;
      InitializeProjectile(k);
      int v1 = k >> 1;
      projectile_bomb_x_speed[v1] = 0;
      projectile_bomb_y_speed[v1] = 0;
      projectile_index = k;
      WaveBeam_CheckColl(k);
      uint16 v2 = projectile_index;
      int v3 = projectile_index >> 1;
      projectile_damage[v3] = 1000;
      projectile_bomb_pre_instructions[v3] = FUNC16(ProjPreInstr_HyperBeam);
      R20_ = v2;
      SetInitialProjectileSpeed();
      cooldown_timer = 21;
      charged_shot_glow_timer = -32748;
      flare_animation_frame = 29;
      flare_slow_sparks_anim_frame = 5;
      flare_fast_sparks_anim_frame = 5;
      flare_animation_timer = 3;
      flare_slow_sparks_anim_timer = 3;
      flare_fast_sparks_anim_timer = 3;
      flare_counter = 0x8000;
    }
  }
}

static Func_V *const kCheckBeamCollByDir[10] = {  // 0x90BD64
  CheckBeamCollByDir_0459,
  CheckBeamCollByDir_1368,
  CheckBeamCollByDir_2,
  CheckBeamCollByDir_1368,
  CheckBeamCollByDir_0459,
  CheckBeamCollByDir_0459,
  CheckBeamCollByDir_1368,
  CheckBeamCollByDir_7,
  CheckBeamCollByDir_1368,
  CheckBeamCollByDir_0459,
};

void CheckBeamCollByDir(uint16 k) {
  kCheckBeamCollByDir[projectile_dir[k >> 1] & 0xF]();
}

void CheckBeamCollByDir_0459(void) {  // 0x90BD86
  BlockCollNoWaveBeamVert(projectile_index);
}

void CheckBeamCollByDir_1368(void) {  // 0x90BD8E
  uint16 v0 = projectile_index;
  if (!(BlockCollNoWaveBeamHoriz(projectile_index) & 1))
    BlockCollNoWaveBeamVert(v0);
}

void CheckBeamCollByDir_2(void) {  // 0x90BD9C
  BlockCollNoWaveBeamHoriz(projectile_index);
}

void CheckBeamCollByDir_7(void) {  // 0x90BDA4
  uint16 v0 = projectile_index;
  projectile_bomb_x_speed[projectile_index >> 1] = -1;
  BlockCollNoWaveBeamHoriz(v0);
}

static Func_V *const kWaveBeam_CheckColl_Funcs[10] = {  // 0x90BDB2
  WaveBeam_CheckColl_0459,
  WaveBeam_CheckColl_1368,
  WaveBeam_CheckColl_2,
  WaveBeam_CheckColl_1368,
  WaveBeam_CheckColl_0459,
  WaveBeam_CheckColl_0459,
  WaveBeam_CheckColl_1368,
  WaveBeam_CheckColl_7,
  WaveBeam_CheckColl_1368,
  WaveBeam_CheckColl_0459,
};
void WaveBeam_CheckColl(uint16 k) {
  kWaveBeam_CheckColl_Funcs[projectile_dir[k >> 1] & 0xF]();
}

void WaveBeam_CheckColl_0459(void) {  // 0x90BDD4
  BlockCollWaveBeamVert(projectile_index);
}

void WaveBeam_CheckColl_1368(void) {  // 0x90BDDC
  uint16 v0 = projectile_index;
  if (!(BlockCollWaveBeamHoriz(projectile_index) & 1))
    BlockCollWaveBeamVert(v0);
}

void WaveBeam_CheckColl_2(void) {  // 0x90BDEA
  BlockCollWaveBeamHoriz(projectile_index);
}

void WaveBeam_CheckColl_7(void) {  // 0x90BDF2
  uint16 v0 = projectile_index;
  projectile_bomb_x_speed[projectile_index >> 1] = -1;
  BlockCollWaveBeamHoriz(v0);
}

void ProjectileReflection(void) {  // 0x90BE00
  uint16 v0 = R20_;
  int v1 = R20_ >> 1;
  uint16 v2 = projectile_type[v1];
  if ((v2 & 0x100) != 0) {
    InitializeProjectile(R20_);
    projectile_bomb_pre_instructions[v1] = FUNC16(ProjPreInstr_Missile);
    projectile_variables[v1] = 240;
  } else if ((v2 & 0x200) != 0) {
    uint16 k = R20_;
    ClearProjectile(LOBYTE(projectile_variables[v1]));
    InitializeProjectile(k);
    int v3 = k >> 1;
    projectile_bomb_pre_instructions[v3] = FUNC16(ProjPreInstr_SuperMissile);
    projectile_variables[v3] = 240;
  } else {
    SetInitialProjectileSpeed();
    InitializeProjectile(v0);
    projectile_bomb_pre_instructions[v1] = kFireChargedBeam_Funcs[projectile_type[v1] & 0xF];
  }
}

void HudSelectionHandler_MissilesOrSuperMissiles(void) {  // 0x90BE62
  uint16 v2;
  char v6;

  if ((button_config_shoot_x & joypad1_newkeys) == 0 && (button_config_shoot_x & joypad1_newinput_samusfilter) == 0
      || !(Samus_CanFireSuperMissile() & 1)) {
    return;
  }
  if (hud_item_index != 2) {
    if (samus_missiles)
      goto LABEL_10;
LABEL_5:
    --projectile_counter;
    return;
  }
  if (!samus_super_missiles)
    goto LABEL_5;
LABEL_10:;
  uint16 v0 = 0;
  while (projectile_damage[v0 >> 1]) {
    v0 += 2;
    if ((int16)(v0 - 10) >= 0)
      goto LABEL_5;
  }
  R20_ = v0;
  if (!(InitProjectilePositionDirection() & 1)) {
    projectile_invincibility_timer = 20;
    if (hud_item_index == 2)
      --samus_super_missiles;
    else
      --samus_missiles;
    int v1 = R20_ >> 1;
    projectile_timers[v1] = 4;
    uint16 v3 = hud_item_index;
    LOBYTE(v2) = HIBYTE(v3);
    v6 = v3;
    HIBYTE(v2) = hud_item_index;
    R18_ = v2;
    projectile_type[v1] |= v2 | 0x8000;
    uint16 v4 = 2 * (v6 & 0xF);
    if (!cinematic_function)
      QueueSfx1_Max6(kNonBeamProjectile_Sfx[v4 >> 1]);
    InitializeProjectileSpeedOfType();
    InitializeProjectile(R20_);
    uint16 v7 = projectile_type[v1];
    if ((v7 & 0x200) != 0)
      projectile_bomb_pre_instructions[v1] = FUNC16(ProjPreInstr_SuperMissile);
    else
      projectile_bomb_pre_instructions[v1] = FUNC16(ProjPreInstr_Missile);
    cooldown_timer = kNonBeamProjectileCooldowns[HIBYTE(v7) & 0xF];
    if (samus_auto_cancel_hud_item_index) {
      hud_item_index = 0;
      samus_auto_cancel_hud_item_index = 0;
      return;
    }
    if (hud_item_index == 2) {
      if (samus_super_missiles)
        return;
    } else if (samus_missiles) {
      return;
    }
    hud_item_index = 0;
  }
}

void Missile_Func2(void) {  // 0x90BF46
  uint16 v0 = 0;
  while (projectile_damage[v0 >> 1]) {
    v0 += 2;
    if ((int16)(v0 - 10) >= 0)
      return;
  }
  R20_ = v0;
  int v1 = v0 >> 1;
  projectile_type[v1] |= 0x8200u;
  int v2 = projectile_index >> 1;
  projectile_x_pos[v1] = projectile_x_pos[v2];
  projectile_y_pos[v1] = projectile_y_pos[v2];
  projectile_dir[v1] = projectile_dir[v2];
  InitProjectilePositionDirection();
  InitializeInstrForSuperMissile(v0);
  projectile_bomb_pre_instructions[v1] = FUNC16(ProjPreInstr_Func1);
  R18_ = v0;
  projectile_variables[projectile_index >> 1] = v0 + (projectile_variables[projectile_index >> 1] & 0xFF00);
  ++projectile_counter;
}

void HudSelectionHandler_MorphBall(void) {  // 0x90BF9D
  uint16 v3;
  char v6;

  if ((button_config_shoot_x & joypad1_lastkeys) != 0) {
    if (hud_item_index == 3) {
      if ((power_bomb_flag & 0x8000u) == 0) {
        if (HudSelectionHandler_MorphBall_Helper2() & 1) {
          if (samus_power_bombs) {
            if ((--samus_power_bombs & 0x8000u) == 0) {
              power_bomb_flag = -1;
              uint16 v2 = 10;
              while (projectile_type[v2 >> 1]) {
                v2 += 2;
                if ((int16)(v2 - 20) >= 0) {
                  v2 -= 2;
                  break;
                }
              }
              R20_ = v2;
              LOBYTE(v3) = HIBYTE(hud_item_index);
              HIBYTE(v3) = hud_item_index;
              R18_ = v3;
              int v4 = v2 >> 1;
              uint16 v5 = v3 | projectile_type[v4];
              projectile_type[v4] = v5;
              v6 = HIBYTE(v5);
              projectile_dir[v4] = 0;
              projectile_x_pos[v4] = samus_x_pos;
              projectile_y_pos[v4] = samus_y_pos;
              projectile_variables[v4] = 60;
              InitializeInstrForMissile(v2);
              projectile_bomb_pre_instructions[v4] = FUNC16(ProjPreInstr_PowerBomb);
              cooldown_timer = kNonBeamProjectileCooldowns[v6 & 0xF];
              if (samus_auto_cancel_hud_item_index) {
                hud_item_index = 0;
                samus_auto_cancel_hud_item_index = 0;
              } else if (hud_item_index == 3 && !samus_power_bombs) {
                hud_item_index = 0;
              }
            }
          }
        }
      }
    } else if (HudSelectionHandler_MorphBall_Helper() & 1) {
      uint16 v0 = 10;
      while (projectile_type[v0 >> 1]) {
        v0 += 2;
        if ((int16)(v0 - 20) >= 0) {
          v0 -= 2;
          break;
        }
      }
      R20_ = v0;
      int v1 = v0 >> 1;
      projectile_type[v1] = 1280;
      projectile_dir[v1] = 0;
      projectile_x_pos[v1] = samus_x_pos;
      projectile_y_pos[v1] = samus_y_pos;
      projectile_variables[v1] = 60;
      InitializeInstrForMissile(v0);
      projectile_bomb_pre_instructions[v1] = FUNC16(ProjPreInstr_Bomb);
      cooldown_timer = kNonBeamProjectileCooldowns[5];
    }
  } else if (flare_counter) {
    QueueSfx1_Max9(2u);
    flare_counter = 0;
    ClearFlareAnimationState();
    Samus_LoadSuitPalette();
  }
}

uint8 HudSelectionHandler_MorphBall_Helper(void) {  // 0x90C0AB
  if ((equipped_items & 0x1000) != 0) {
    if (sign16(flare_counter - 60) || bomb_counter)
      return HudSelectionHandler_MorphBall_Helper2();
    if ((joypad1_lastkeys & kButton_Down) != 0 && sign16((bomb_spread_charge_timeout_counter & 0xC0) - 192)) {
      ++bomb_spread_charge_timeout_counter;
    } else {
      BombSpread();
      Samus_LoadSuitPalette();
      QueueSfx1_Max9(2u);
    }
  }
  return 0;
}

uint8 HudSelectionHandler_MorphBall_Helper2(void) {  // 0x90C0E7
  if ((button_config_shoot_x & joypad1_newkeys) == 0
      || bomb_counter && (!sign16(bomb_counter - 5) || (uint8)cooldown_timer)) {
    if (flare_counter) {
      QueueSfx1_Max9(2u);
      flare_counter = 0;
      ClearFlareAnimationState();
      Samus_LoadSuitPalette();
    }
    return 0;
  } else {
    ++cooldown_timer;
    ++bomb_counter;
    return 1;
  }
}

void Bomb_Func2(void) {  // 0x90C128
  uint16 v0 = projectile_index;
  int v1 = projectile_index >> 1;
  uint16 v2 = projectile_variables[v1];
  if (v2) {
    uint16 v3 = v2 - 1;
    projectile_variables[v1] = v3;
    if (v3) {
      if (v3 == 15)
        projectile_bomb_instruction_ptr[v1] += 28;
    } else {
      QueueSfx2_Max6(8u);
      InitializeBombExplosion(v0);
    }
  }
}

void PowerBomb_Func3(void) {  // 0x90C157
  uint16 v0 = projectile_index;
  int v1 = projectile_index >> 1;
  uint16 v2 = projectile_variables[v1];
  if (v2) {
    uint16 v3 = v2 - 1;
    projectile_variables[v1] = v3;
    if (v3) {
      if (v3 == 15)
        projectile_bomb_instruction_ptr[v1] += 28;
    } else {
      power_bomb_explosion_x_pos = projectile_x_pos[v1];
      power_bomb_explosion_y_pos = projectile_y_pos[v1];
      EnableHdmaObjects();
      SpawnPowerBombExplosion();
      projectile_variables[v0 >> 1] = -1;
    }
  } else if (!power_bomb_flag) {
    ClearProjectile(projectile_index);
  }
}

static Func_U8 *const kRunSwitchedToHudHandler[6] = {  // 0x90C4B5
  SwitchToHudHandler_Nothing,
  SwitchToHudHandler_Missiles,
  SwitchToHudHandler_SuperMissiles,
  SwitchToHudHandler_PowerBombs,
  SwitchToHudHandler_Grapple,
  SwitchToHudHandler_Xray,
};
void HandleSwitchingHudSelection(void) {
  uint16 v0;
  R18_ = hud_item_index;
  if ((button_config_itemcancel_y & joypad1_newkeys) != 0) {
    samus_auto_cancel_hud_item_index = 0;
LABEL_5:
    v0 = 0;
    goto LABEL_6;
  }
  R22_ = (button_config_itemcancel_y & joypad1_lastkeys) != 0;
  if ((button_config_itemswitch & joypad1_newkeys) == 0)
    goto LABEL_13;
  v0 = hud_item_index + 1;
  if (!sign16(hud_item_index - 5))
    goto LABEL_5;
LABEL_6:
  hud_item_index = v0;
  while (kRunSwitchedToHudHandler[v0]() & 1) {
    v0 = hud_item_index + 1;
    hud_item_index = v0;
    if (!sign16(v0 - 6)) {
      v0 = 0;
      hud_item_index = 0;
    }
  }
  if (R22_)
    samus_auto_cancel_hud_item_index = hud_item_index;
  else
    samus_auto_cancel_hud_item_index = 0;
LABEL_13:
  if (hud_item_index == R18_) {
    uint16 v1 = hud_item_changed_this_frame + 1;
    if (!sign16(hud_item_changed_this_frame - 2))
      v1 = 2;
    hud_item_changed_this_frame = v1;
  } else {
    hud_item_changed_this_frame = 1;
  }
}

uint8 SwitchToHudHandler_Nothing(void) {  // 0x90C545
  flare_counter = 0;
  ClearFlareAnimationState();
  Samus_LoadSuitPalette();
  return 0;
}

uint8 SwitchToHudHandler_Missiles(void) {  // 0x90C551
  if (!samus_missiles)
    return 1;
  flare_counter = 0;
  ClearFlareAnimationState();
  Samus_LoadSuitPalette();
  return 0;
}

uint8 SwitchToHudHandler_SuperMissiles(void) {  // 0x90C564
  if (!samus_super_missiles)
    return 1;
  flare_counter = 0;
  ClearFlareAnimationState();
  Samus_LoadSuitPalette();
  return 0;
}

uint8 SwitchToHudHandler_PowerBombs(void) {  // 0x90C577
  if (!samus_power_bombs)
    return 1;
  flare_counter = 0;
  ClearFlareAnimationState();
  Samus_LoadSuitPalette();
  return 0;
}

uint8 SwitchToHudHandler_Grapple(void) {  // 0x90C58A
  if ((equipped_items & 0x4000) == 0)
    return 1;
  if (grapple_beam_function == FUNC16(GrappleBeamFunc_Inactive)) {
    Samus_LoadSuitPalette();
    flare_counter = 0;
    ClearFlareAnimationState();
    grapple_beam_function = FUNC16(GrappleBeamFunc_Inactive);
  }
  return 0;
}

uint8 SwitchToHudHandler_Xray(void) {  // 0x90C5AE
  if ((equipped_items & 0x8000u) == 0)
    return 1;
  flare_counter = 0;
  ClearFlareAnimationState();
  Samus_LoadSuitPalette();
  return 0;
}

void HandleArmCannonOpenState(void) {  // 0x90C5C4
  if (flag_arm_cannon_opening_or_closing || UpdateArmCannonIsOpenFlag() & 1)
    AdvanceArmCannonFrame();
  arm_cannon_drawing_mode = RomPtr_90(kPlayerPoseToPtr[samus_pose])[1];
}

uint8 UpdateArmCannonIsOpenFlag(void) {  // 0x90C5EB
  static const int8 kFlagShouldArmCannonBeOpen[6] = { 0, 1, 1, 0, 1, 0 };
  if (sign16(hud_item_changed_this_frame - 2))
    return 0;
  R18_ = kFlagShouldArmCannonBeOpen[hud_item_index];
  if (flag_arm_cannon_open_or_opening == R18_)
    return 0;
  if (R18_)
    arm_cannon_frame = 0;
  else
    arm_cannon_frame = 4;
  *(uint16 *)&flag_arm_cannon_open_or_opening = R18_ | 0x100;
  return 1;
}

void AdvanceArmCannonFrame(void) {  // 0x90C627
  if (flag_arm_cannon_open_or_opening) {
    if (sign16(arm_cannon_frame - 2)) {
      ++arm_cannon_frame;
      return;
    }
    arm_cannon_frame = 3;
  } else {
    if (arm_cannon_frame != 1 && (int16)(arm_cannon_frame - 1) >= 0) {
      --arm_cannon_frame;
      return;
    }
    arm_cannon_frame = 0;
  }
  *(uint16 *)&flag_arm_cannon_open_or_opening = flag_arm_cannon_open_or_opening;
}

void Samus_ArmCannon_Draw(void) {  // 0x90C663
  static const uint16 kDrawArmCannon_Char[10] = { 0x281f, 0x281f, 0x281f, 0x681f, 0xa81f, 0xe81f, 0x281f, 0x681f, 0x681f, 0x681f };
  int16 v2;
  int16 v8;
  OamEnt *v9;
  int16 v10;
  int16 v12;
  uint16 v3;

  if (arm_cannon_frame && (!samus_invincibility_timer || (nmi_frame_counter_word & 1) == 0)) {
    uint16 v0 = kPlayerPoseToPtr[samus_pose];
    const uint8 *v1 = RomPtr_90(v0);
    v2 = *v1;
    if ((v2 & 0x80) != 0) {
      if (samus_anim_frame)
        v3 = 2 * (v1[2] & 0x7F);
      else
        v3 = 2 * (*v1 & 0x7F);
      R22_ = v0 + 4;
    } else {
      v3 = 2 * v2;
      R22_ = v0 + 2;
    }
    R24_ = kDrawArmCannon_Char[v3 >> 1];
    const uint8 *v4 = RomPtr_90(R22_ + 2 * samus_anim_frame);
    R18_ = (int8)v4[0];
    R20_ = (int8)v4[1];
    R22_ = *(&kPoseParams[0].y_offset_to_gfx + (uint16)(8 * samus_pose));
    uint16 v7 = oam_next_ptr;
    v8 = R18_ + samus_x_pos - layer1_x_pos;
    if (v8 >= 0) {
      if (sign16(v8 - 256)) {
        v9 = gOamEnt(oam_next_ptr);
        *(uint16 *)&v9->xcoord = v8;
        v10 = (__PAIR32__(R20_ + samus_y_pos - R22_, R20_ + samus_y_pos) - __PAIR32__(layer1_y_pos, R22_)) >> 16;
        if (v10 >= 0) {
          if (sign16(v10 - 256)) {
            *(uint16 *)&v9->ycoord = v10;
            *(uint16 *)&v9->charnum = R24_;
            oam_next_ptr = v7 + 4;
          }
        }
      }
    }
    const uint8 *v11 = RomPtr_90(kPlayerPoseToPtr[samus_pose]);
    v12 = *v11;
    if ((v12 & 0x80) != 0) {
      if (samus_anim_frame)
        v12 = *RomPtr_90(kPlayerPoseToPtr[samus_pose] + 2) & 0x7F;
      else
        v12 = *v11 & 0x7F;
    }
    R22_ = kDrawArmCannon_Tab2[v12];
    uint16 v13 = R22_ + 2 * arm_cannon_frame;
    uint16 v14 = vram_write_queue_tail;
    gVramWriteEntry(vram_write_queue_tail)->size = 32;
    v14 += 2;
    uint16 v15 = *(uint16 *)RomPtr_90(v13);
    gVramWriteEntry(v14)->size = v15;
    v14 += 2;
    LOBYTE(gVramWriteEntry(v14++)->size) = -102;
    gVramWriteEntry(v14)->size = addr_unk_6061F0;
    vram_write_queue_tail = v14 + 2;
  }
}

void Projectile_SinLookup(uint16 j, uint16 a) {  // 0x90CC39
  int16 v3;
  uint16 v2, v4;

  R24_ = a;
  R26_ = j;
  if (sign16(j - 128))
    v2 = Projectile_SinLookup_Inner(2 * j);
  else
    v2 = -Projectile_SinLookup_Inner(2 * (uint8)(j + 0x80));
  R20_ = v2;
  v3 = (uint8)(R26_ - 64);
  if (sign16(v3 - 128))
    v4 = Projectile_SinLookup_Inner(2 * v3);
  else
    v4 = -Projectile_SinLookup_Inner(2 * (uint8)(v3 + 0x80));
  R22_ = v4;
}


uint16 Projectile_SinLookup_Inner(uint16 k) {  // 0x90CC8A
  uint16 prod = Mult8x8(*((uint8 *)&kSinCosTable8bit_Sext[64] + k), R24_);
  R18_ = prod >> 8;
  prod = Mult8x8(*((uint8 *)&kSinCosTable8bit_Sext[64] + k + 1), R24_);
  return R18_ + prod;
}

static const uint16 kCostOfSbaInPowerBombs[12] = {  // 0x90CCC0
  0, 1, 1, 0, 1, 0, 0, 0,
  1, 0, 0, 0,
};

static Func_V_A *const kFireSbaFuncs[12] = {
  FireSba_ClearCarry,
  FireSba_FireWave,
  FireSba_FireIce,
  FireSba_ClearCarry,
  FireSba_FireSpazer,
  FireSba_ClearCarry,
  FireSba_ClearCarry,
  FireSba_ClearCarry,
  FireSba_FirePlasma,
  FireSba_ClearCarry,
  FireSba_ClearCarry,
  FireSba_ClearCarry,
};

uint8 FireSba(void) {
  int16 v2;

  if (hud_item_index != 3)
    return 0;
  uint16 v1 = 2 * (equipped_beams & 0xF);
  v2 = samus_power_bombs - kCostOfSbaInPowerBombs[v1 >> 1];
  if (v2 < 0)
    v2 = 0;
  samus_power_bombs = v2;
  uint8 rv = kFireSbaFuncs[v1 >> 1]();
  if (!samus_power_bombs) {
    hud_item_index = 0;
    samus_auto_cancel_hud_item_index = 0;
  }
  return rv;
}

uint8 FireSba_ClearCarry(void) {  // 0x90CD18
  return 0;
}

uint8 FireSba_FireWave(void) {  // 0x90CD1A
  static const int16 kFireSba_FireWave_X[4] = { 128, 128, -128, -128 };
  static const int16 kFireSba_FireWave_Y[4] = { 128, -128, -128, 128 };

  for (int i = 6; i >= 0; i -= 2) {
    int v1 = i >> 1;
    projectile_timers[v1] = 4;
    projectile_type[v1] = equipped_beams & 0x100F | 0x8010;
    projectile_dir[v1] = 0;
    projectile_bomb_pre_instructions[v1] = FUNC16(ProjPreInstr_WaveSba);
    projectile_bomb_y_speed[v1] = 600;
    projectile_bomb_x_speed[v1] = 0;
    projectile_variables[v1] = 0;
    projectile_bomb_x_subpos[v1] = 0;
    projectile_bomb_y_subpos[v1] = 0;
    projectile_x_pos[v1] = kFireSba_FireWave_X[v1] + samus_x_pos;
    projectile_y_pos[v1] = kFireSba_FireWave_Y[v1] + samus_y_pos;
    InitializeSbaProjectile(i);
  }
  projectile_counter = 4;
  cooldown_timer = kProjectileCooldown_Uncharged[projectile_type[0] & 0x3F];
  used_for_sba_attacksB60 = 4;
  QueueSfx1_Max6(0x28u);
  return 1;
}

uint8 FireSba_FireIce(void) {  // 0x90CD9B
  static const uint16 kIcePlasmaSbaProjectileOriginAngles[8] = { 0, 0x40, 0x80, 0xc0, 0x20, 0x60, 0xa0, 0xe0 };

  if (projectile_bomb_pre_instructions[0] == FUNC16(ProjPreInstr_IceSba2)
      || projectile_bomb_pre_instructions[0] == FUNC16(ProjPreInstr_IceSba)) {
    return 0;
  }
  for (int i = 6; i >= 0; i -= 2) {
    int v2 = i >> 1;
    projectile_timers[v2] = 4;
    projectile_type[v2] = equipped_beams & 0x100F | 0x8010;
    projectile_dir[v2] = 0;
    projectile_bomb_pre_instructions[v2] = FUNC16(ProjPreInstr_IceSba);
    projectile_variables[v2] = kIcePlasmaSbaProjectileOriginAngles[v2];
    projectile_bomb_y_speed[v2] = 600;
    InitializeProjectile(i);
  }
  projectile_counter = 4;
  cooldown_timer = kProjectileCooldown_Uncharged[projectile_type[0] & 0x3F];
  if (samus_pose_x_dir == 4)
    used_for_sba_attacksB60 = -4;
  else
    used_for_sba_attacksB60 = 4;
  QueueSfx1_Max6(0x23u);
  return 1;
}

uint8 FireSba_FireSpazer(void) {  // 0x90CE14
  static const int16 kFireSpazer_Timer[4] = { 0, 0, 4, 4 };
  static const int16 kFireSpazer_Yspeed[4] = { 4, -4, 4, -4 };

  for (int i = 6; i >= 0; i -= 2) {
    int v1 = i >> 1;
    projectile_timers[v1] = kFireSpazer_Timer[v1];
    projectile_dir[v1] = 5;
    projectile_bomb_pre_instructions[v1] = FUNC16(ProjPreInstr_SpazerSba);
    projectile_bomb_x_speed[v1] = 40;
    projectile_bomb_y_speed[v1] = kFireSpazer_Yspeed[v1];
    projectile_variables[v1] = 0;
    projectile_unk_A[v1] = 0;
    projectile_bomb_x_subpos[v1] = 0;
    projectile_bomb_y_subpos[v1] = 0;
    if ((int16)(i - 4) >= 0) {
      projectile_type[i >> 1] = -32732;
      InitializeShinesparkEchoOrSpazerSba(i);
    } else {
      projectile_type[i >> 1] = equipped_beams & 0x100F | 0x8010;
      InitializeSbaProjectile(i);
    }
  }
  projectile_counter = 4;
  cooldown_timer = kProjectileCooldown_Uncharged[projectile_type[0] & 0x3F];
  used_for_sba_attacksB60 = 0;
  QueueSfx1_Max6(0x25u);
  return 1;
}

uint8 FireSba_FirePlasma(void) {  // 0x90CE98
  static const uint16 kIcePlasmaSbaProjectileOriginAngles[8] = { 0, 0x40, 0x80, 0xc0, 0x20, 0x60, 0xa0, 0xe0 };


  if (projectile_bomb_pre_instructions[0] == FUNC16(ProjPreInstr_PlasmaSba))
    return 0;
  for (int i = 6; i >= 0; i -= 2) {
    int v2 = i >> 1;
    projectile_type[v2] = equipped_beams & 0x100F | 0x8010;
    projectile_dir[v2] = 0;
    projectile_bomb_pre_instructions[v2] = FUNC16(ProjPreInstr_PlasmaSba);
    projectile_variables[v2] = kIcePlasmaSbaProjectileOriginAngles[v2];
    projectile_bomb_x_speed[v2] = 40;
    projectile_bomb_y_speed[v2] = 0;
    InitializeSbaProjectile(i);
  }
  projectile_counter = 4;
  cooldown_timer = kProjectileCooldown_Uncharged[projectile_type[0] & 0x3F];
  if (samus_pose_x_dir == 4)
    used_for_sba_attacksB60 = -4;
  else
    used_for_sba_attacksB60 = 4;
  QueueSfx1_Max6(0x27u);
  return 1;
}

void ProjPreInstr_IceSba(uint16 k) {  // 0x90CF09
  int v1 = k >> 1;
  if ((projectile_dir[v1] & 0xF0) != 0) {
    QueueSfx1_Max6(0x24u);
    ClearProjectile(k);
  } else {
    bool v2 = projectile_timers[v1]-- == 1;
    if (v2) {
      projectile_timers[v1] = 4;
      SpawnProjectileTrail(k);
      k = projectile_index;
    }
    int v3 = k >> 1;
    Projectile_SinLookup(projectile_variables[v3], 0x20);
    projectile_x_pos[v3] = R20_ + samus_x_pos;
    projectile_y_pos[v3] = R22_ + samus_y_pos;
    projectile_variables[v3] = (uint8)(used_for_sba_attacksB60 + projectile_variables[v3]);
    v2 = projectile_bomb_y_speed[v3]-- == 1;
    if (v2) {
      projectile_bomb_pre_instructions[v3] = FUNC16(ProjPreInstr_IceSba2);
      projectile_bomb_x_speed[v3] = 40;
      QueueSfx1_Max6(0x24u);
    }
    cooldown_timer = 2;
    flare_counter = 0;
  }
}

void ProjPreInstr_IceSba2(uint16 k) {  // 0x90CF7A
  int16 v4;
  int16 v5;
  int16 v6;
  int16 v7;

  int v1 = k >> 1;
  if ((projectile_dir[v1] & 0xF0) != 0)
    goto LABEL_9;
  if (projectile_timers[v1]-- == 1) {
    projectile_timers[v1] = 4;
    SpawnProjectileTrail(k);
    k = projectile_index;
  }
  int v3;
  v3 = k >> 1;
  Projectile_SinLookup(projectile_variables[v3], projectile_bomb_x_speed[v3]);
  v4 = R20_ + samus_x_pos;
  projectile_x_pos[v3] = R20_ + samus_x_pos;
  v5 = v4 - layer1_x_pos;
  if (sign16(v5 + 32)
      || !sign16(v5 - 288)
      || (v6 = R22_ + samus_y_pos, projectile_y_pos[v3] = R22_ + samus_y_pos, v7 = v6 - layer1_y_pos, sign16(v7 - 16))
      || !sign16(v7 - 256)) {
LABEL_9:
    ClearProjectile(k);
  } else {
    projectile_variables[v3] = (uint8)(used_for_sba_attacksB60 + projectile_variables[v3]);
    projectile_bomb_x_speed[v3] = (uint8)(projectile_bomb_x_speed[v3] + 8);
    cooldown_timer = 2;
    flare_counter = 0;
  }
}

void Projectile_Func7_Shinespark(void) {  // 0x90CFFA
  samus_movement_handler = FUNC16(Samus_MoveHandlerShinesparkWindup);
  samus_y_dir = 1;
  speed_boost_counter = 1024;
  samus_y_subspeed = 0;
  samus_y_speed = 0;
  knockback_dir = 0;
  samus_x_extra_run_speed = 8;
  samus_x_extra_run_subspeed = 0;
  samus_x_base_speed = 0;
  samus_x_base_subspeed = 0;
  substate = 7;
  suit_pickup_light_beam_pos = 0;
  cooldown_timer = 0;
  timer_for_shinesparks_startstop = 30;
  samus_shine_timer = 60;
  timer_for_shine_timer = 6;
  special_samus_palette_frame = 0;
  bomb_jump_dir = 0;
  if (flare_counter) {
    if (!sign16(flare_counter - 16))
      QueueSfx1_Max9(2u);
    flare_counter = 0;
    ClearFlareAnimationState();
  }
}

void Samus_MoveHandlerShinesparkWindup(void) {  // 0x90D068
  bool v0 = (--timer_for_shinesparks_startstop & 0x8000u) != 0;
  if (!timer_for_shinesparks_startstop || v0) {
    if (samus_pose_x_dir == 4)
      samus_new_pose_interrupted = 204;
    else
      samus_new_pose_interrupted = 203;
    samus_movement_handler = FUNC16(Samus_MoveHandlerVerticalShinespark);
    samus_input_handler = FUNC16(nullsub_152);
    speed_echoes_index = 0;
    speed_echo_xspeed[0] = 0;
    speed_echo_xspeed[1] = 0;
    speed_echo_xpos[0] = 0;
    speed_echo_xpos[1] = 0;
    QueueSfx3_Max9(0xFu);
  }
}

void Samus_MoveHandlerVerticalShinespark(void) {  // 0x90D0AB
  samus_contact_damage_index = 2;
  samus_hurt_flash_counter = 8;
  Samus_UpdateSpeedEchoPos();
  Samus_ShinesparkMove_Y();
  Samus_EndSuperJump();
  if (!sign16(samus_health - 30) && (--samus_health & 0x8000u) != 0)
    samus_health = 0;
}

void Samus_MoveHandler_Shinespark_Diag(void) {  // 0x90D0D7
  samus_contact_damage_index = 2;
  samus_hurt_flash_counter = 8;
  Samus_UpdateSpeedEchoPos();
  Samus_ShinesparkMove_X();
  Samus_ShinesparkMove_Y();
  Samus_EndSuperJump();
  if (!sign16(samus_health - 30) && (--samus_health & 0x8000u) != 0)
    samus_health = 0;
}

void Samus_MoveHandler_Shinespark_Horiz(void) {  // 0x90D106
  samus_contact_damage_index = 2;
  samus_hurt_flash_counter = 8;
  Samus_UpdateSpeedEchoPos();
  Samus_ShinesparkMove_X();
  Samus_EndSuperJump();
  if (!sign16(samus_health - 30) && (--samus_health & 0x8000u) != 0)
    samus_health = 0;
}

void Samus_ShinesparkMove_X(void) {  // 0x90D132
  int16 v4;

  samus_shine_timer = 15;
  uint16 v0 = (__PAIR32__(samus_y_accel, samus_y_subaccel) + __PAIR32__(samus_x_extra_run_speed, samus_x_extra_run_subspeed)) >> 16;
  samus_x_extra_run_subspeed += samus_y_subaccel;
  samus_x_extra_run_speed = v0;
  if (!sign16(v0 - 15)) {
    samus_x_extra_run_speed = 15;
    samus_x_extra_run_subspeed = 0;
  }
  R18_ = 0;
  R20_ = 0;
  if (samus_pose_x_dir == 4) {
    Samus_CalcDisplacementMoveLeft();
    R18_ = ~R18_;
    bool v2 = R20_ == 0;
    R20_ = -R20_;
    if (v2)
      ++R18_;
    if (!sign16(R18_ - 15))
      R18_ = 15;
    uint16 v3 = Samus_CheckSolidEnemyColl();
    if (v3) {
      samus_collision_flag = v3;
      goto LABEL_18;
    }
    R18_ = ~R18_;
    v2 = R20_ == 0;
    R20_ = -R20_;
    if (v2)
      ++R18_;
  } else {
    Samus_CalcDisplacementMoveRight();
    if (!sign16(R18_ - 15))
      R18_ = 15;
    uint16 v1 = Samus_CheckSolidEnemyColl();
    if (v1) {
      samus_collision_flag = v1;
      goto LABEL_18;
    }
  }
  Samus_MoveRight_NoSolidColl();
  Samus_AlignYPosSlope();
LABEL_18:
  v4 = samus_x_pos - samus_prev_x_pos;
  if ((int16)(samus_x_pos - samus_prev_x_pos) < 0) {
    if (sign16(v4 + 15))
      samus_prev_x_pos = samus_x_pos + 15;
  } else if (!sign16(v4 - 16)) {
    samus_prev_x_pos = samus_x_pos - 15;
  }
}

void Samus_ShinesparkMove_Y(void) {  // 0x90D1FF
  int16 v3;

  samus_shine_timer = 15;
  uint16 v0 = (__PAIR32__(samus_y_accel, samus_y_subaccel) + __PAIR32__(substate, suit_pickup_light_beam_pos)) >> 16;
  suit_pickup_light_beam_pos += samus_y_subaccel;
  substate = v0;
  bool v1 = __CFADD__uint16(suit_pickup_light_beam_pos, samus_y_subspeed);
  samus_y_subspeed += suit_pickup_light_beam_pos;
  R20_ = samus_y_subspeed;
  samus_y_speed += v0 + v1;
  R18_ = samus_y_speed;
  if (!sign16(samus_y_speed - 14))
    R18_ = 14;
  R18_ = ~R18_;
  bool v2 = R20_ == 0;
  R20_ = -R20_;
  if (v2)
    ++R18_;
  v3 = (__PAIR32__(R18_, R20_) + __PAIR32__(extra_samus_y_displacement, extra_samus_y_subdisplacement)) >> 16;
  R20_ += extra_samus_y_subdisplacement;
  samus_collision_direction = 2;
  R18_ = ~v3;
  v2 = R20_ == 0;
  R20_ = -R20_;
  if (v2)
    ++R18_;
  if (!sign16(R18_ - 15))
    R18_ = 15;
  uint16 v4 = Samus_CheckSolidEnemyColl();
  if (v4) {
    samus_collision_flag = v4;
  } else {
    R18_ = ~R18_;
    v2 = R20_ == 0;
    R20_ = -R20_;
    if (v2)
      ++R18_;
    Samus_MoveDown_NoSolidColl();
  }
  if (sign16(samus_y_pos - samus_prev_y_pos + 14))
    samus_prev_y_pos = samus_y_pos + 14;
}

uint8 Samus_EndSuperJump(void) {  // 0x90D2BA
  if (!sign16(samus_health - 30) && !samus_collision_flag)
    return 0;
  if (samus_pose_x_dir == 4) {
    speed_echo_xspeed[0] = 32;
    speed_echo_xspeed[1] = 160;
    speed_echo_xpos[2] = 4;
  } else {
    speed_echo_xspeed[0] = 224;
    speed_echo_xspeed[1] = 96;
    speed_echo_xpos[2] = -4;
  }
  samus_y_subspeed = 0;
  samus_y_speed = 0;
  samus_x_extra_run_speed = 0;
  samus_x_extra_run_subspeed = 0;
  speed_boost_counter = 0;
  UNUSED_word_7E0B1A = 0;
  samus_y_dir = 0;
  samus_movement_handler = FUNC16(Samus_MoveHandler_ShinesparkCrash);
  samus_draw_handler = FUNC16(Samus_DrawHandler_EndOfShinespark);
  speed_echoes_index = 0;
  speed_echo_xpos[0] = samus_x_pos;
  speed_echo_xpos[1] = samus_x_pos;
  speed_echo_ypos[0] = samus_y_pos;
  speed_echo_ypos[1] = samus_y_pos;
  speed_echo_xspeed[2] = 0;
  samus_hurt_flash_counter = 0;
  QueueSfx1_Max6(0x35u);
  QueueSfx3_Max6(0x10u);
  return 1;
}

static Func_V *const kSamus_MoveHandler_ShinesparkCrash[3] = {  // 0x90D346
  Samus_MoveHandler_ShinesparkCrash_0,
  Samus_MoveHandler_ShinesparkCrash_1,
  Samus_MoveHandler_ShinesparkCrash_2,
};

void Samus_MoveHandler_ShinesparkCrash(void) {

  samus_shine_timer = 15;
  kSamus_MoveHandler_ShinesparkCrash[HIBYTE(speed_echoes_index)]();
  for (int i = 2; i >= 0; i -= 2) {
    int v1 = i >> 1;
    Projectile_SinLookup(speed_echo_xspeed[v1], (uint8)speed_echoes_index);
    speed_echo_xpos[v1] = R20_ + samus_x_pos;
    speed_echo_ypos[v1] = R22_ + samus_y_pos;
  }
}

void Samus_MoveHandler_ShinesparkCrash_0(void) {  // 0x90D383
  uint16 v0 = speed_echoes_index + 4;
  if (!sign16(speed_echoes_index - 12))
    v0 |= 0x100u;
  speed_echoes_index = v0;
}

void Samus_MoveHandler_ShinesparkCrash_1(void) {  // 0x90D396
  speed_echo_xspeed[0] = (uint8)(LOBYTE(speed_echo_xpos[2]) + LOBYTE(speed_echo_xspeed[0]));
  speed_echo_xspeed[1] = (uint8)(LOBYTE(speed_echo_xpos[2]) + LOBYTE(speed_echo_xspeed[1]));
  speed_echo_ypos[2] += 4;
  if (!sign16(speed_echo_ypos[2] - 128))
    speed_echoes_index = (uint8)speed_echoes_index | 0x200;
}

void Samus_MoveHandler_ShinesparkCrash_2(void) {  // 0x90D3CC
  speed_echoes_index -= 4;
  if (!(uint8)speed_echoes_index) {
    samus_movement_handler = FUNC16(Samus_MoveHandler_ShinesparkCrashEchoCircle);
    timer_for_shinesparks_startstop = 30;
    speed_echoes_index = 0;
    speed_echo_xspeed[0] = 0;
    speed_echo_xspeed[1] = 0;
  }
}

void Samus_MoveHandler_ShinesparkCrashEchoCircle(void) {  // 0x90D3F3
  samus_shine_timer = 15;
  bool v0 = (--timer_for_shinesparks_startstop & 0x8000u) != 0;
  if (!timer_for_shinesparks_startstop || v0) {
    samus_movement_handler = FUNC16(Samus_MoveHandler_ShinesparkCrashFinish);
    samus_draw_handler = FUNC16(SamusDrawHandler_Default);
  }
}

void Samus_MoveHandler_ShinesparkCrashFinish(void) {  // 0x90D40D
  static const uint8 kShinesparkCrashFinish_Tab0[12] = { 0, 0x80, 0, 0x80, 0x40, 0xc0, 0x40, 0xc0, 0xe0, 0x60, 0x20, 0xa0 };
  speed_echoes_index = 0;
  if (sign16(projectile_counter - 5)) {
    if (sign16(projectile_counter - 4)) {
      ++projectile_counter;
      speed_echo_xspeed[2] = 64;
      speed_echo_xpos[2] = samus_x_pos;
      speed_echo_ypos[2] = samus_y_pos;
      projectile_type[3] = addr_loc_908029;
      InitializeShinesparkEchoOrSpazerSba(6u);
      projectile_bomb_pre_instructions[3] = FUNC16(ProjPreInstr_SpeedEcho);
      projectile_variables[3] = kShinesparkCrashFinish_Tab0[(uint16)(2 * (samus_pose - 201))];
      projectile_bomb_x_speed[3] = 0;
    }
    ++projectile_counter;
    speed_echo_xspeed[3] = 64;
    speed_echo_xpos[3] = samus_x_pos;
    speed_echo_ypos[3] = samus_y_pos;
    projectile_type[4] = addr_loc_908029;
    InitializeShinesparkEchoOrSpazerSba(8u);
    projectile_bomb_pre_instructions[4] = FUNC16(ProjPreInstr_SpeedEcho);
    projectile_variables[4] = kShinesparkCrashFinish_Tab0[(uint16)(2 * (samus_pose - 201)) + 1];
    projectile_bomb_x_speed[4] = 0;
  }
  cooldown_timer = 0;
  samus_shine_timer = 1;
  if (samus_pose_x_dir == 4)
    samus_new_pose_transitional = 2;
  else
    samus_new_pose_transitional = 1;
  samus_hurt_switch_index = 2;
  substate = 0;
  suit_pickup_light_beam_pos = 0;
}

void ProjPreInstr_SpeedEcho(uint16 k) {  // 0x90D4D2
  int16 v3;
  uint16 v4;
  int16 v5;

  int v1 = k >> 1;
  projectile_bomb_x_speed[v1] += 8;
  Projectile_SinLookup(projectile_variables[v1], LOBYTE(projectile_bomb_x_speed[v1]));
  uint16 v2 = R20_ + samus_x_pos;
  *(uint16 *)((char *)&speed_echoes_index + k) = R20_ + samus_x_pos;
  projectile_x_pos[v1] = v2;
  v3 = v2 - layer1_x_pos;
  if (v3 < 0
      || !sign16(v3 - 256)
      || (v4 = R22_ + samus_y_pos,
          speed_echo_xpos[v1 + 3] = R22_ + samus_y_pos,
          projectile_y_pos[v1] = v4,
          v5 = v4 - layer1_y_pos,
          v5 < 0)
      || !sign16(v5 - 256)) {
    speed_echo_ypos[v1 + 3] = 0;
    *(uint16 *)((char *)&speed_echoes_index + k) = 0;
    speed_echo_xpos[v1 + 3] = 0;
    ClearProjectile(k);
  }
}

void Grapple_Func1(void) {  // 0x90D525
  bool v0; // sf

  if ((button_config_shoot_x & joypad1_lastkeys) != 0
      && (v0 = (int16)(grapple_walljump_timer - 1) < 0, --grapple_walljump_timer, grapple_walljump_timer)
      && !v0) {
    if ((grapple_beam_length_delta & 0x8000u) == 0) {
      grapple_beam_length += grapple_beam_length_delta;
      if (!sign16(grapple_beam_length - 96))
        grapple_beam_length_delta = -16;
    }
    Projectile_SinLookup(HIBYTE(*(uint16 *)&grapple_beam_end_subangle), grapple_beam_length);
    grapple_beam_end_x_pos = R20_ + x_pos_of_start_of_grapple_beam;
    grapple_beam_end_y_pos = R22_ + y_pos_of_start_of_grapple_beam;
    *(uint16 *)&grapple_beam_end_subangle += 2048;
    GrappleBeamFunc_BF1B();
  } else {
    grapple_beam_function = FUNC16(GrappleBeamFunc_Cancel);
  }
}

uint8 Hdmaobj_CrystalFlash(void) {  // 0x90D5A2
  SamusPose v1;

  if (sign16(game_state - 40)) {
    R18_ = button_config_shoot_x | 0x430;
    if (joypad1_lastkeys != (button_config_shoot_x | 0x430))
      return 1;
  }
  if (samus_y_speed
      || samus_y_subspeed
      || !sign16(samus_health - 51)
      || samus_reserve_health
      || sign16(samus_missiles - 10)
      || sign16(samus_super_missiles - 10)
      || sign16(samus_power_bombs - 10)) {
    return 1;
  }
  v1 = samus_pose_x_dir == 4 ? kPose_D4_FaceL_CrystalFlash : kPose_D3_FaceR_CrystalFlash;
  samus_pose = v1;
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
  if (samus_movement_type != 27)
    return 1;
  samus_last_different_pose = samus_prev_pose;
  *(uint16 *)&samus_last_different_pose_x_dir = *(uint16 *)&samus_prev_pose_x_dir;
  samus_prev_pose = samus_pose;
  *(uint16 *)&samus_prev_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
  samus_movement_handler = FUNC16(SamusMoveHandler_CrystalFlashStart);
  if (samus_input_handler != FUNC16(Samus_InputHandler_E91D))
    samus_input_handler = FUNC16(nullsub_152);
  timer_for_shinesparks_startstop = 9;
  which_item_to_pickup = 0;
  substate = 10;
  suit_pickup_light_beam_pos = 0;
  *(uint16 *)&suit_pickup_color_math_R = 0;
  timer_for_shine_timer = 7;
  special_samus_palette_frame = 0;
  samus_shine_timer = 1;
  *(uint16 *)&suit_pickup_color_math_B = 1;
  samus_invincibility_timer = 0;
  samus_knockback_timer = 0;
  knockback_dir = 0;
  return 0;
}

void SamusMoveHandler_CrystalFlashStart(void) {  // 0x90D678
  samus_y_pos -= 2;
  if ((--timer_for_shinesparks_startstop & 0x8000u) != 0) {
    samus_anim_frame_timer = 3;
    samus_anim_frame = 6;
    *(uint16 *)&suit_pickup_color_math_R = samus_y_pos;
    samus_movement_handler = FUNC16(SamusMoveHandler_CrystalFlashMain);
    samus_invincibility_timer = 0;
    samus_knockback_timer = 0;
    QueueSfx3_Max15(1u);
    power_bomb_flag = 0;
    power_bomb_explosion_x_pos = samus_x_pos;
    power_bomb_explosion_y_pos = samus_y_pos;
    EnableHdmaObjects();
    SpawnCrystalFlashHdmaObjs();
  }
}

static Func_V *const kSamusMoveHandler_CrystalFlashMainFuncs[3] = {  // 0x90D6CE
  SamusMoveHandler_CrystalFlashMain_0,
  SamusMoveHandler_CrystalFlashMain_1,
  SamusMoveHandler_CrystalFlashMain_2,
};

void SamusMoveHandler_CrystalFlashMain(void) {
  kSamusMoveHandler_CrystalFlashMainFuncs[which_item_to_pickup]();
  samus_invincibility_timer = 0;
  samus_knockback_timer = 0;
}

void SamusMoveHandler_CrystalFlashMain_0(void) {  // 0x90D6E3
  if ((nmi_frame_counter_word & 7) == 0) {
    --samus_missiles;
    Samus_RestoreHealth(0x32u);
    bool v0 = (int16)(substate - 1) < 0;
    if (!--substate || v0) {
      substate = 10;
      ++which_item_to_pickup;
    }
  }
}

void SamusMoveHandler_CrystalFlashMain_1(void) {  // 0x90D706
  if ((nmi_frame_counter_word & 7) == 0) {
    --samus_super_missiles;
    Samus_RestoreHealth(0x32u);
    bool v0 = (int16)(substate - 1) < 0;
    if (!--substate || v0) {
      substate = 10;
      ++which_item_to_pickup;
    }
  }
}

void SamusMoveHandler_CrystalFlashMain_2(void) {  // 0x90D729
  if ((nmi_frame_counter_word & 7) == 0) {
    --samus_power_bombs;
    Samus_RestoreHealth(0x32u);
    bool v0 = (int16)(substate - 1) < 0;
    if (!--substate || v0) {
      samus_movement_handler = FUNC16(kSamusMoveHandler_CrystalFlashFinish);
      samus_draw_handler = FUNC16(SamusDrawHandler_Default);
      samus_anim_frame_timer = 3;
      samus_anim_frame = 12;
    }
  }
}

void kSamusMoveHandler_CrystalFlashFinish(void) {  // 0x90D75B
  if (samus_y_pos != *(uint16 *)&suit_pickup_color_math_R)
    ++samus_y_pos;
  if (!samus_movement_type) {
    power_bomb_flag = 0;
    samus_shine_timer = -1;
    samus_movement_handler = FUNC16(Samus_MovementHandler_Normal);
    if (samus_input_handler != FUNC16(Samus_InputHandler_E91D)) {
      samus_input_handler = FUNC16(Samus_InputHandler_E913);
      samus_invincibility_timer = 0;
      samus_knockback_timer = 0;
    }
  }
}

void ProjPreInstr_PlasmaSba(uint16 k) {  // 0x90D793
  int16 v3;

  int v1 = k >> 1;
  if ((projectile_dir[v1] & 0xF0) != 0) {
    ClearProjectile(k);
  } else {
    cooldown_timer = 2;
    flare_counter = 0;
    Projectile_SinLookup(projectile_variables[v1], projectile_bomb_x_speed[v1]);
    projectile_x_pos[v1] = R20_ + samus_x_pos;
    projectile_y_pos[v1] = R22_ + samus_y_pos;
    projectile_variables[v1] = (uint8)(used_for_sba_attacksB60 + projectile_variables[v1]);
    v3 = 2 * projectile_bomb_y_speed[v1];
    if (v3) {
      if (v3 == 2) {
        ProjPreInstr_PlasmaSbaFunc_1(k);
      } else {
        if (v3 != 4) {
          Unreachable();
          while (1)
            ;
        }
        ProjPreInstr_PlasmaSbaFunc_2(k);
      }
    } else {
      ProjPreInstr_PlasmaSbaFunc_0(k);
    }
  }
}

void ProjPreInstr_PlasmaSbaFunc_0(uint16 j) {  // 0x90D7E1
  int v1 = j >> 1;
  uint16 v2 = (uint8)(projectile_bomb_x_speed[v1] + 4);
  projectile_bomb_x_speed[v1] = v2;
  if (!sign16(v2 - 192))
    projectile_bomb_y_speed[v1] = 1;
}

void ProjPreInstr_PlasmaSbaFunc_1(uint16 j) {  // 0x90D7FA
  int v1 = j >> 1;
  uint16 v2 = (uint8)(projectile_bomb_x_speed[v1] - 4);
  projectile_bomb_x_speed[v1] = v2;
  if (sign16(v2 - 45))
    projectile_bomb_y_speed[v1] = 2;
}

void ProjPreInstr_PlasmaSbaFunc_2(uint16 j) {  // 0x90D813
  int v1 = j >> 1;
  if (sign16(projectile_x_pos[v1] - layer1_x_pos + 32)
      || !sign16(projectile_x_pos[v1] - layer1_x_pos - 288)
      || sign16(projectile_y_pos[v1] - layer1_y_pos - 16)
      || !sign16(projectile_y_pos[v1] - layer1_y_pos - 256)) {
    ClearProjectile(j);
  } else {
    projectile_bomb_x_speed[v1] = (uint8)(projectile_bomb_x_speed[v1] + 4);
  }
}

static const uint16 kBombSpread_Tab0[5] = { 0x78, 0x6e, 0x64, 0x6e, 0x78 };
static const uint16 kBombSpread_Tab1[5] = { 0x8100, 0x8080, 0, 0x80, 0x100 };
static const uint16 kBombSpread_Tab2[5] = { 0, 1, 2, 1, 0 };
static const uint16 kBombSpread_Tab3[5] = { 0, 0, 0x8000, 0, 0 };

void ProjPreInstr_SpreadBomb(uint16 k) {  // 0x90D8F7
  int16 v13;
  int16 v17;
  uint16 v9;

  int v1 = k >> 1;
  if ((projectile_dir[v1] & 0xF0) != 0) {
    ClearProjectile(k);
    return;
  }
  Bomb_Func2();
  if (projectile_variables[v1]) {
    uint16 v2 = projectile_timers[v1];
    bool v3 = __CFADD__uint16(samus_y_subaccel, v2);
    projectile_timers[v1] = samus_y_subaccel + v2;
    projectile_bomb_y_speed[v1] += samus_y_accel + v3;
    uint16 v4 = projectile_bomb_y_subpos[v1];
    v3 = __CFADD__uint16(projectile_timers[v1], v4);
    projectile_bomb_y_subpos[v1] = projectile_timers[v1] + v4;
    projectile_y_pos[v1] += projectile_bomb_y_speed[v1] + v3;
    if (BlockCollSpreadBomb(k) & 1) {
      uint16 v5 = projectile_index - 10;
      int v6 = projectile_index >> 1;
      uint16 v7 = projectile_bomb_y_subpos[v6];
      v3 = v7 < projectile_timers[v6];
      projectile_bomb_y_subpos[v6] = v7 - projectile_timers[v6];
      projectile_y_pos[v6] -= v3 + projectile_bomb_y_speed[v6];
      if ((projectile_bomb_y_speed[v6] & 0x8000u) != 0) {
        projectile_bomb_y_speed[v6] = 0;
        projectile_y_radius[v6] = 0;
      } else {
        projectile_timers[v6] = kBombSpread_Tab3[v5 >> 1];
        projectile_bomb_y_speed[v6] = projectile_unk_A[v6];
      }
      return;
    }
    k = projectile_index;
    int v8 = projectile_index >> 1;
    LOBYTE(v9) = HIBYTE(projectile_bomb_x_speed[v8]);
    HIBYTE(v9) = projectile_bomb_x_speed[v8];
    R20_ = v9 & 0xFF00;
    v9 = (uint8)v9;
    R18_ = v9;
    if ((v9 & 0x80) != 0) {
      R18_ = v9 & 0x7F;
      uint16 v11 = projectile_bomb_x_subpos[v8];
      v3 = v11 < R20_;
      projectile_bomb_x_subpos[v8] = v11 - R20_;
      projectile_x_pos[v8] -= v3 + R18_;
    } else {
      uint16 v10 = projectile_bomb_x_subpos[v8];
      v3 = __CFADD__uint16(R20_, v10);
      projectile_bomb_x_subpos[v8] = R20_ + v10;
      projectile_x_pos[v8] += R18_ + v3;
    }
  }
  if (BlockCollSpreadBomb(k) & 1) {
    int v12 = projectile_index >> 1;
    v17 = projectile_bomb_x_speed[v12];
    LOBYTE(v13) = HIBYTE(v17);
    HIBYTE(v13) = v17;
    R20_ = v13 & 0xFF00;
    R18_ = HIBYTE(v17) & 0x7F;
    uint16 v15;
    if (v17 >= 0) {
      projectile_bomb_x_speed[v12] = v17 | 0x8000;
      uint16 v16 = projectile_bomb_x_subpos[v12];
      bool v3 = v16 < R20_;
      projectile_bomb_x_subpos[v12] = v16 - R20_;
      v15 = projectile_x_pos[v12] - (v3 + R18_);
    } else {
      projectile_bomb_x_speed[v12] = v17 & 0x7FFF;
      uint16 v14 = projectile_bomb_x_subpos[v12];
      bool v3 = __CFADD__uint16(R20_, v14);
      projectile_bomb_x_subpos[v12] = R20_ + v14;
      v15 = R18_ + v3 + projectile_x_pos[v12];
    }
    projectile_x_pos[v12] = v15;
  }
}

void ProjPreInstr_WaveSba(uint16 k) {  // 0x90DA08
  int v1 = k >> 1;
  bool v2, v3;
  if ((projectile_dir[v1] & 0xF0) != 0
      || (v2 = projectile_bomb_y_speed[v1] == 1,
          v3 = (int16)(projectile_bomb_y_speed[v1] - 1) < 0,
          --projectile_bomb_y_speed[v1],
          v2)
      || v3) {
    QueueSfx1_Max6(0x29u);
    ClearProjectile(k);
    return;
  }
  R34 = projectile_bomb_x_speed[v1];
  R36 = projectile_variables[v1];
  v2 = projectile_timers[v1]-- == 1;
  if (v2) {
    projectile_timers[v1] = 4;
    SpawnProjectileTrail(k);
    k = projectile_index;
  }
  int v4 = k >> 1;
  if ((int16)(samus_x_pos - projectile_x_pos[v4]) < 0) {
    if (!sign16(projectile_bomb_x_speed[v4] + 2047))
      projectile_bomb_x_speed[v4] -= 64;
  } else if (sign16(projectile_bomb_x_speed[v4] - 2048)) {
    projectile_bomb_x_speed[v4] += 64;
  }
  uint16 v5 = swap16(projectile_bomb_x_speed[v4]);
  R20_ = v5 & 0xFF00;
  R18_ = (int8)v5;
  uint16 v6 = projectile_bomb_x_subpos[v4];
  bool v7 = __CFADD__uint16(R20_, v6);
  projectile_bomb_x_subpos[v4] = R20_ + v6;
  projectile_x_pos[v4] += R18_ + v7;
  if ((int16)(samus_y_pos - projectile_y_pos[v4]) < 0) {
    if (!sign16(projectile_variables[v4] + 2047))
      projectile_variables[v4] -= 64;
  } else if (sign16(projectile_variables[v4] - 2048)) {
    projectile_variables[v4] += 64;
  }
  uint16 v8 = swap16(projectile_variables[v4]);
  R20_ = v8 & 0xFF00;
  R18_ = (int8)v8;
  uint16 v9 = projectile_bomb_y_subpos[v4];
  v7 = __CFADD__uint16(R20_, v9);
  projectile_bomb_y_subpos[v4] = R20_ + v9;
  projectile_y_pos[v4] += R18_ + v7;
  if (k == 6) {
    if ((projectile_bomb_x_speed[3] & 0x8000u) != 0) {
      if ((R34 & 0x8000u) == 0)
        LABEL_26:
      QueueSfx1_Max6(0x28u);
    } else if ((R34 & 0x8000u) != 0) {
      goto LABEL_26;
    }
  }
  cooldown_timer = 2;
  flare_counter = 0;
}


void BombSpread(void) {  // 0x90D849
  if (bomb_functions[0] != FUNC16(ProjPreInstr_SpreadBomb)) {
    uint16 v0 = 10;
    do {
      int v1 = v0 >> 1;
      projectile_type[v1] = -31488;
      projectile_dir[v1] = 0;
      projectile_bomb_pre_instructions[v1] = FUNC16(ProjPreInstr_SpreadBomb);
      InitializeInstrForMissile(v0);
      projectile_x_pos[v1] = samus_x_pos;
      projectile_bomb_x_subpos[v1] = 0;
      projectile_y_pos[v1] = samus_y_pos;
      projectile_bomb_y_subpos[v1] = 0;
      int v2 = (uint16)(v0 - 10) >> 1;
      projectile_bomb_x_speed[v1] = kBombSpread_Tab1[v2];
      projectile_timers[v1] = kBombSpread_Tab3[v2];
      uint16 v3 = -(int16)(kBombSpread_Tab2[v2] + ((bomb_spread_charge_timeout_counter >> 6) & 3));
      projectile_bomb_y_speed[v1] = v3;
      projectile_unk_A[v1] = v3;
      projectile_variables[v1] = kBombSpread_Tab0[v2];
      v0 += 2;
    } while ((int16)(v0 - 20) < 0);
    bomb_counter = 5;
    cooldown_timer = kNonBeamProjectileCooldowns[HIBYTE(projectile_type[5]) & 0xF];
    bomb_spread_charge_timeout_counter = 0;
    flare_counter = 0;
  }
}

static Func_Y_V *const kProjPreInstr_SpazerSba_FuncsB[3] = {  // 0x90DB06
  ProjPreInstr_SpazerSba_FuncB_0,
  ProjPreInstr_SpazerSba_FuncB_1,
  ProjPreInstr_SpazerSba_FuncB_2,
};
static Func_Y_V *const kProjPreInstr_SpazerSba_FuncsA[4] = {
  ProjPreInstr_SpazerSba_FuncA_0,
  ProjPreInstr_SpazerSba_FuncA_1,
  ProjPreInstr_SpazerSba_FuncA_2,
  ProjPreInstr_SpazerSba_FuncA_3,
};

void ProjPreInstr_SpazerSba(uint16 k) {

  int v1 = k >> 1;
  if ((projectile_dir[v1] & 0xF0) != 0) {
    kProjPreInstr_SpazerSba_FuncsA[v1](k);
  } else {
    int v2 = k >> 1;
    if (projectile_timers[v2]-- == 1) {
      projectile_timers[v2] = 4;
      SpawnProjectileTrail(k);
      k = projectile_index;
    }
    int v4 = k >> 1;
    Projectile_SinLookup(projectile_variables[v4], projectile_bomb_x_speed[v4]);
    projectile_x_pos[v4] = R20_ + samus_x_pos;
    kProjPreInstr_SpazerSba_FuncsB[projectile_unk_A[v4] >> 1](k);
    cooldown_timer = 2;
    flare_counter = 0;
  }
}

void ProjPreInstr_SpazerSba_FuncA_0(uint16 k) {  // 0x90DB57
  ClearProjectile(k);
  ClearProjectile(4u);
}

void ProjPreInstr_SpazerSba_FuncA_1(uint16 k) {  // 0x90DB66
  ClearProjectile(k);
  ClearProjectile(6u);
}

void ProjPreInstr_SpazerSba_FuncA_2(uint16 k) {  // 0x90DB75
  ClearProjectile(k);
  ClearProjectile(0);
}

void ProjPreInstr_SpazerSba_FuncA_3(uint16 k) {  // 0x90DB84
  ClearProjectile(k);
  ClearProjectile(2u);
}

void ProjPreInstr_SpazerSba_FuncB_0(uint16 j) {  // 0x90DB93
  static const int16 kProjPreInstr_SpazerSba_Yspeed[4] = { 2, -2, 2, -2 };

  int v1 = j >> 1;
  projectile_y_pos[v1] = R22_ + samus_y_pos;
  uint16 v2 = (uint8)(LOBYTE(projectile_bomb_y_speed[v1]) + LOBYTE(projectile_variables[v1]));
  projectile_variables[v1] = v2;
  if (v2 == 128) {
    projectile_bomb_x_speed[v1] = 160;
    projectile_bomb_y_speed[v1] = kProjPreInstr_SpazerSba_Yspeed[v1];
    projectile_dir[v1] = 0;
    projectile_unk_A[v1] = 2;
  }
}

void ProjPreInstr_SpazerSba_FuncB_1(uint16 j) {  // 0x90DBCF
  static const int16 kSpazerProjectileYSpeed[4] = { -2, 2, -2, 2 };

  uint16 v1 = R22_ + samus_y_pos - 114;
  int v2 = j >> 1;
  projectile_y_pos[v2] = v1;
  if (sign16(v1 - layer1_y_pos - 16)) {
    FireEndOfSpazerSba(j);
  } else {
    projectile_variables[v2] = (uint8)(LOBYTE(projectile_bomb_y_speed[v2]) + LOBYTE(projectile_variables[v2]));
    uint16 v3 = projectile_bomb_x_speed[v2] - 5;
    projectile_bomb_x_speed[v2] = v3;
    if (!v3) {
      projectile_bomb_y_speed[v2] = kSpazerProjectileYSpeed[v2];
      projectile_variables[v2] = (uint8)(projectile_variables[v2] + 0x80);
      projectile_unk_A[v2] = 4;
      if (!j)
        QueueSfx1_Max6(0x26u);
    }
  }
}

void ProjPreInstr_SpazerSba_FuncB_2(uint16 j) {  // 0x90DC30
  int v2;
  uint16 v3;

  uint16 v1 = R22_ + samus_y_pos - 114;
  projectile_y_pos[j >> 1] = v1;
  if (sign16(v1 - layer1_y_pos - 16)
      || (v2 = j >> 1,
          projectile_variables[v2] = (uint8)(LOBYTE(projectile_bomb_y_speed[v2])
                                             + LOBYTE(projectile_variables[v2])),
          v3 = projectile_bomb_x_speed[v2] + 5,
          projectile_bomb_x_speed[v2] = v3,
          !sign16(v3 - 96))) {
    FireEndOfSpazerSba(j);
  }
}

void FireEndOfSpazerSba(uint16 j) {  // 0x90DC67
  static const int16 kFireEndOfSpazerSba[4] = { 16, 16, -16, -16 };

  int v1 = j >> 1;
  projectile_x_pos[v1] += kFireEndOfSpazerSba[v1];
  projectile_dir[v1] = 5;
  projectile_timers[v1] = 4;
  projectile_bomb_pre_instructions[v1] = FUNC16(ProjPreInstr_EndOfSpazerSba);
  if ((int16)(j - 4) < 0) {
    projectile_type[j >> 1] = -32732;
    InitializeShinesparkEchoOrSpazerSba(j);
  }
}

void ProjPreInstr_EndOfSpazerSba(uint16 k) {  // 0x90DC9C
  int v1 = k >> 1;
  if ((projectile_dir[v1] & 0xF0) != 0)
    goto LABEL_2;
  if (projectile_timers[v1]-- == 1) {
    projectile_timers[v1] = 4;
    SpawnProjectileTrail(k);
    k = projectile_index;
  }
  int v3;
  v3 = k >> 1;
  uint16 v4;
  v4 = projectile_y_pos[v3] + 8;
  projectile_y_pos[v3] = v4;
  if (!sign16(v4 - layer1_y_pos - 248)) {
LABEL_2:
    ClearProjectile(k);
  } else {
    cooldown_timer = 2;
    flare_counter = 0;
  }
}

static Func_V *kHandleHudSpecificBehaviorAndProjs[28] = {  // 0x90DCDD
  HudSelectionHandler_Normal,
  HudSelectionHandler_Normal,
  HudSelectionHandler_Normal,
  HudSelectionHandler_JumpEtc,
  HudSelectionHandler_MorphBall,
  HudSelectionHandler_Normal,
  HudSelectionHandler_Normal,
  HudSelectionHandler_MorphBall,
  HudSelectionHandler_MorphBall,
  HudSelectionHandler_MorphBall,
  HudSelectionHandler_JumpEtc,
  HudSelectionHandler_Grappling,
  HudSelectionHandler_Grappling,
  HudSelectionHandler_JumpEtc,
  HudSelectionHandler_TurningAround,
  HudSelectionHandler_CrouchEtcTrans,
  HudSelectionHandler_Normal,
  HudSelectionHandler_MorphBall,
  HudSelectionHandler_MorphBall,
  HudSelectionHandler_MorphBall,
  HudSelectionHandler_JumpEtc,
  HudSelectionHandler_Normal,
  HudSelectionHandler_Grappling,
  HudSelectionHandler_TurningAround,
  HudSelectionHandler_TurningAround,
  HudSelectionHandler_JumpEtc,
  HudSelectionHandler_GrabbedByDraygon,
  HudSelectionHandler_JumpEtc,
};

void Samus_HandleHudSpecificBehaviorAndProjs(void) {
  if (samus_pose == kPose_00_FaceF_Powersuit
      || samus_pose == kPose_9B_FaceF_VariaGravitySuit
      || (Samus_HandleCooldown(),
          HandleSwitchingHudSelection(),
          kHandleHudSpecificBehaviorAndProjs[samus_movement_type](),
          !time_is_frozen_flag)) {
    HandleProjectile();
  }
}

void HudSelectionHandler_Normal(void) {  // 0x90DD3D
  static Func_V *const kHudSelectionHandler_Normal[7] = {
  HudSelectionHandler_NothingOrPowerBombs,
  HudSelectionHandler_MissilesOrSuperMissiles,
  HudSelectionHandler_MissilesOrSuperMissiles,
  HudSelectionHandler_NothingOrPowerBombs,
  HudSelectionHandler_Grappling,
  HudSelectionHandler_Xray,
  HudSelectionHandler_TurningAround,
  };
  uint16 v0;
  if (grapple_beam_function == FUNC16(GrappleBeamFunc_Inactive)) {
    if (time_is_frozen_flag)
      v0 = 10;
    else
      v0 = 2 * hud_item_index;
  } else {
    v0 = 8;
  }
  kHudSelectionHandler_Normal[v0 >> 1]();
}

void HudSelectionHandler_Grappling(void) {  // 0x90DD6F
  GrappleBeamHandler();
}

void HudSelectionHandler_TurningAround(void) {  // 0x90DD74
  if (new_projectile_direction_changed_pose) {
    HudSelectionHandler_Normal();
  } else if (grapple_beam_function != FUNC16(GrappleBeamFunc_Inactive)) {
    grapple_beam_function = FUNC16(GrappleBeamFunc_Cancel);
  }
}

void HudSelectionHandler_CrouchEtcTrans(void) {  // 0x90DD8C
  static const uint8 byte_90DDAA[12] = { 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1 };
  if (!sign16(samus_pose - kPose_F1_FaceR_CrouchTrans_AimU))
    goto LABEL_4;
  if (!sign16(samus_pose - kPose_DB))
    return;
  if (byte_90DDAA[(uint16)(samus_pose - 53)]) {
    if (grapple_beam_function != FUNC16(GrappleBeamFunc_Inactive)) {
      grapple_beam_function = FUNC16(GrappleBeamFunc_Cancel);
      HudSelectionHandler_Normal();
    }
  } else {
LABEL_4:
    HudSelectionHandler_Normal();
  }
}

void HudSelectionHandler_JumpEtc(void) {  // 0x90DDB6
  if (grapple_beam_function != FUNC16(GrappleBeamFunc_Inactive)) {
    grapple_beam_function = FUNC16(GrappleBeamFunc_Cancel);
    HudSelectionHandler_Normal();
  }
}

void HudSelectionHandler_Xray(void) {  // 0x90DDC8
  if ((button_config_run_b & joypad1_lastkeys) != 0)
    XrayRunHandler();
  else
    HudSelectionHandler_NothingOrPowerBombs();
}

void HudSelectionHandler_GrabbedByDraygon(void) {  // 0x90DDD8
  if (samus_pose == (kPose_DB | kPose_04_FaceL_AimU))
    HudSelectionHandler_MorphBall();
  else
    HudSelectionHandler_Normal();
}

static Func_U8 *const kSamusHitInterrupts[28] = {  // 0x90DDE9
  Samus_HitInterrupt_Stand,
  Samus_HitInterrupt_Stand,
  Samus_HitInterrupt_Stand,
  Samus_HitInterrupt_Stand,
  Samus_HitInterrupt_Ball,
  Samus_HitInterrupt_Stand,
  Samus_HitInterrupt_Falling,
  Samus_HitInterrupt_Unused,
  Samus_HitInterrupt_Ball,
  Samus_HitInterrupt_Ball,
  Samus_HitInterrupt_KnockbackGrapple,
  Samus_HitInterrupt_Turning,
  Samus_HitInterrupt_Turning,
  Samus_HitInterrupt_Stand,
  Samus_HitInterrupt_Turning,
  Samus_HitInterrupt_Turning,
  Samus_HitInterrupt_Stand,
  Samus_HitInterrupt_Ball,
  Samus_HitInterrupt_Ball,
  Samus_HitInterrupt_Ball,
  Samus_HitInterrupt_Stand,
  Samus_HitInterrupt_Stand,
  Samus_HitInterrupt_KnockbackGrapple,
  Samus_HitInterrupt_Turning,
  Samus_HitInterrupt_Turning,
  Samus_HitInterrupt_Turning,
  Samus_HitInterrupt_Turning,
  Samus_HitInterrupt_Shinespark,
};

void Samus_HitInterruption(void) {
  if (samus_knockback_timer) {
    if (sign16(debug_invincibility - 7)) {
      if (!time_is_frozen_flag && !knockback_dir) {
        if (kSamusHitInterrupts[samus_movement_type]() & 1)
          samus_special_transgfx_index = 1;
      }
    } else {
      samus_invincibility_timer = 0;
      samus_knockback_timer = 0;
    }
  } else if (knockback_dir) {
    if (samus_movement_type == kPose_0A_MoveL_NoAim) {
      if (!sign16(flare_counter - 16))
        QueueSfx1_Max6(0x41u);
      if (samus_pose_x_dir == 4)
        samus_new_pose_transitional = kPose_2A_FaceL_Fall;
      else
        samus_new_pose_transitional = kPose_29_FaceR_Fall;
    } else {
      if (samus_hurt_switch_index == 3) {
        samus_hurt_switch_index = 8;
        return;
      }
      samus_new_pose_transitional = samus_pose;
    }
    samus_hurt_switch_index = 1;
  } else if (bomb_jump_dir) {
    SetupBombJump();
  }
}

uint8 Samus_HitInterrupt_Shinespark(void) {  // 0x90DEBA
  if (samus_pose == kPose_E8_FaceR_Drained_CrouchFalling || samus_pose == kPose_E9_FaceL_Drained_CrouchFalling) {
    samus_anim_frame_timer = 17;
    samus_anim_frame = 26;
  }
  samus_special_transgfx_index = 0;
  knockback_dir = 0;
  return 0;
}

uint8 Samus_HitInterrupt_KnockbackGrapple(void) {  // 0x90DEDD
  samus_special_transgfx_index = 0;
  return 0;
}

uint8 Samus_HitInterrupt_Turning(void) {  // 0x90DEE2
  samus_special_transgfx_index = 0;
  knockback_dir = 0;
  return 0;
}

uint8 Samus_HitInterrupt_Falling(void) {  // 0x90DEEA
  if (frame_handler_gamma != FUNC16(Samus_Func9))
    return Samus_HitInterrupt_Stand();
  samus_special_transgfx_index = 0;
  knockback_dir = 0;
  return 0;
}

uint8 Samus_HitInterrupt_Stand(void) {  // 0x90DEFA
  if (samus_pose_x_dir == 4)
    samus_new_pose_interrupted = 84;
  else
    samus_new_pose_interrupted = 83;
  return 1;
}

uint8 Samus_HitInterrupt_Ball(void) {  // 0x90DF15
  samus_new_pose_interrupted = samus_pose;
  return 1;
}

uint8 Samus_HitInterrupt_Unused(void) {  // 0x90DF1D
  if (samus_pose_x_dir == 4)
    samus_new_pose_interrupted = 52;
  else
    samus_new_pose_interrupted = 51;
  return 1;
}

static Func_V *const kSamus_MoveHandler_Knockback[6] = {  // 0x90DF38
  Samus_MoveHandler_Knockback_0,
  Samus_MoveHandler_Knockback_Up,
  Samus_MoveHandler_Knockback_Up,
  Samus_MoveHandler_Knockback_3,
  Samus_MoveHandler_Knockback_Down,
  Samus_MoveHandler_Knockback_Down,
};

void Samus_MoveHandler_Knockback(void) {
  kSamus_MoveHandler_Knockback[knockback_dir]();
  input_to_pose_calc = 0;
}

void Samus_MoveHandler_Knockback_0(void) {  // 0x90DF50
  InvalidInterrupt_Crash();
}

void Samus_MoveHandler_Knockback_Up(void) {  // 0x90DF53
  Samus_BombJumpRisingXMovement_();
  Samus_MoveY_WithSpeedCalc();
  Samus_ClearMoveVars();
}

void Samus_MoveHandler_Knockback_3(void) {  // 0x90DF5D
  Samus_MoveY_WithSpeedCalc();
  Samus_ClearMoveVars();
}

void Samus_MoveHandler_Knockback_Down(void) {  // 0x90DF64
  Samus_BombJumpRisingXMovement_();
  Samus_Move_NoSpeedCalc_Y();
  Samus_ClearMoveVars();
}

void Samus_ClearMoveVars(void) {  // 0x90DF6E
  if (samus_collision_flag) {
    samus_x_accel_mode = 0;
    samus_collides_with_solid_enemy = 0;
    samus_is_falling_flag = 0;
    UNUSED_word_7E0B1A = 0;
    UNUSED_word_7E0B2A = 0;
    samus_y_subspeed = 0;
    samus_y_speed = 0;
    samus_y_dir = 0;
    UNUSED_word_7E0B38 = 0;
    samus_x_base_speed = 0;
    samus_x_base_subspeed = 0;
    Samus_AlignBottomWithPrevPose();
  }
}

static Func_U8 *const kSetupBombJump[28] = {  // 0x90DF99
  SetupBombJump_StandCrouch,
  SetupBombJump_1,
  SetupBombJump_2,
  SetupBombJump_2,
  SetupBombJump_4,
  SetupBombJump_StandCrouch,
  SetupBombJump_1,
  SetupBombJump_4,
  SetupBombJump_4,
  SetupBombJump_4,
  SetupBombJump_4,
  SetupBombJump_1,
  SetupBombJump_1,
  SetupBombJump_1,
  SetupBombJump_2,
  SetupBombJump_2,
  SetupBombJump_1,
  SetupBombJump_4,
  SetupBombJump_4,
  SetupBombJump_4,
  SetupBombJump_1,
  SetupBombJump_1,
  SetupBombJump_1,
  SetupBombJump_2,
  SetupBombJump_2,
  SetupBombJump_2,
  SetupBombJump_1A,
  SetupBombJump_1A,
};
void SetupBombJump(void) {
  if (!HIBYTE(bomb_jump_dir)) {
    if (kSetupBombJump[samus_movement_type]() & 1)
      samus_special_transgfx_index = 3;
  }
}

uint8 SetupBombJump_StandCrouch(void) {  // 0x90DFED
  if (!time_is_frozen_flag)
    return SetupBombJump_1();
  bomb_jump_dir = 0;
  return 0;
}

uint8 SetupBombJump_1(void) {  // 0x90DFF7
  if (samus_pose_x_dir == 4)
    samus_new_pose_interrupted = kPose_52_FaceL_Jump_NoAim_MoveF;
  else
    samus_new_pose_interrupted = kPose_51_FaceR_Jump_NoAim_MoveF;
  return 1;
}

uint8 SetupBombJump_4(void) {  // 0x90E012
  samus_new_pose_interrupted = samus_pose;
  return 1;
}

uint8 SetupBombJump_2(void) {  // 0x90E01A
  samus_input_handler = FUNC16(Samus_InputHandler_E913);
  return SetupBombJump_1A();
}

uint8 SetupBombJump_1A(void) {  // 0x90E020
  bomb_jump_dir = 0;
  return 0;
}

void Samus_MoveHandler_BombJumpStart(void) {  // 0x90E025
  Samus_InitBombJump();
  samus_movement_handler = FUNC16(Samus_MoveHandler_BombJumpMain);
  input_to_pose_calc = 0;
}

static Func_V *const kSamus_MoveHandler_BombJumpMain[4] = {  // 0x90E032
  Samus_MoveHandler_Knockback_0,
  Samus_HorizontalBombJump,
  Samus_VerticalBombJump,
  Samus_HorizontalBombJump,
};

void Samus_MoveHandler_BombJumpMain(void) {
  if (bomb_jump_dir)
    kSamus_MoveHandler_BombJumpMain[(uint8)bomb_jump_dir]();
  else
    Samus_MoveHandler_BombJumpFunc1();
}

void Samus_HorizontalBombJump(void) {  // 0x90E04C
  Samus_BombJumpRisingXMovement_();
  Samus_BombJumpRisingYMovement_();
  if (samus_y_dir == 2 || (Samus_MoveY_WithSpeedCalc(), samus_collision_flag))
    Samus_MoveHandler_BombJumpFunc1();
}

void Samus_VerticalBombJump(void) {  // 0x90E066
  Samus_BombJumpRisingYMovement_();
  if (samus_y_dir == 2 || (Samus_MoveY_WithSpeedCalc(), samus_collision_flag))
    Samus_MoveHandler_BombJumpFunc1();
}

void Samus_MoveHandler_BombJumpFunc1(void) {  // 0x90E07D
  samus_movement_handler = FUNC16(Samus_MovementHandler_Normal);
  if (samus_input_handler != FUNC16(Samus_InputHandler_E91D))
    samus_input_handler = FUNC16(Samus_InputHandler_E913);
  bomb_jump_dir = 0;
}

void CallFrameHandlerGamma(uint32 ea) {
  switch (ea) {
  case fnSamus_Func1: Samus_Func1(); return;
  case fnSamus_Func2: Samus_Func2(); return;
  case fnSamus_Func3: Samus_Func3(); return;
  case fnDrawTimer_: DrawTimer_(); return;
  case fnSamus_PushOutOfRidleysWay: Samus_PushOutOfRidleysWay(); return;
  case fnSamus_Func4: Samus_Func4(); return;
  case fnSamus_GrabbedByDraygonFrameHandler: Samus_GrabbedByDraygonFrameHandler(); return;
  case fnnullsub_151: return;
  case fnSamus_Func7: Samus_Func7(); return;
  case fnSamus_Func9: Samus_Func9(); return;
  case fnnullsub_152: return;
  default: Unreachable();
  }
}
void RunFrameHandlerGamma(void) {  // 0x90E097
  CallFrameHandlerGamma(frame_handler_gamma | 0x900000);
}

void Samus_Func1(void) {  // 0x90E09B
  if (samus_pose == kPose_E9_FaceL_Drained_CrouchFalling
      && !sign16(samus_anim_frame - 8)
      && (joypad1_newkeys & kButton_Up) != 0) {
    samus_anim_frame_timer = 1;
    samus_anim_frame = 13;
    frame_handler_gamma = FUNC16(nullsub_152);
  }
}

void Samus_Func2(void) {  // 0x90E0C5
  if (!sign16(samus_anim_frame - 8) && sign16(samus_anim_frame - 12) && (joypad1_newkeys & kButton_Up) != 0) {
    samus_anim_frame_timer = 1;
    samus_anim_frame = 18;
  }
}

void Samus_Func3(void) {  // 0x90E0E6
  if (ProcessTimer() & 1) {
    game_state = kGameState_35_TimeUp;
    for (int i = 510; i >= 0; i -= 2)
      target_palettes[i >> 1] = 0x7FFF;
    frame_handler_gamma = FUNC16(DrawTimer_);
    DisablePaletteFx();
  }
  if (timer_status)
    DrawTimer();
}

void DrawTimer_(void) {  // 0x90E114
  DrawTimer();
}

void Samus_SetPushedOutOfCeresRidley(void) {  // 0x90E119
  samus_movement_handler = FUNC16(nullsub_152);
  frame_handler_gamma = FUNC16(Samus_PushOutOfRidleysWay);
}

void Samus_PushOutOfRidleysWay(void) {  // 0x90E12E
  if (samus_pose_x_dir == 4)
    samus_pose = kPose_54_FaceL_Knockback;
  else
    samus_pose = kPose_53_FaceR_Knockback;
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
  samus_last_different_pose = samus_prev_pose;
  *(uint16 *)&samus_last_different_pose_x_dir = *(uint16 *)&samus_prev_pose_x_dir;
  samus_prev_pose = samus_pose;
  *(uint16 *)&samus_prev_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
  R18_ = 21 - samus_y_radius;
  samus_y_pos -= 21 - samus_y_radius;
  if (sign16(samus_x_pos - layer1_x_pos - 128))
    samus_var62 = 1;
  else
    samus_var62 = 2;
  samus_y_speed = 5;
  samus_y_subspeed = 0;
  bomb_jump_dir = 0;
  frame_handler_gamma = FUNC16(Samus_Func4);
  samus_new_pose = -1;
  samus_new_pose_interrupted = -1;
  samus_new_pose_transitional = -1;
  samus_momentum_routine_index = 0;
  samus_special_transgfx_index = 0;
  samus_hurt_switch_index = 0;
  ProcessTimer();
  if (timer_status)
    DrawTimer();
}

void Samus_Func4(void) {  // 0x90E1C8
  static Func_V *const off_90E1F7[3] = {
    0,
    Samus_Func5,
    Samus_Func6,
  };

  if (samus_new_pose == kPose_4F_FaceL_Dmgboost || samus_new_pose == kPose_50_FaceR_Dmgboost) {
    samus_new_pose = -1;
    samus_momentum_routine_index = 0;
  }
  off_90E1F7[samus_var62]();
  input_to_pose_calc = 0;
  ProcessTimer();
  if (timer_status)
    DrawTimer();
}

void Samus_Func5(void) {  // 0x90E1FD
  Samus_BombJumpFallingXMovement_();
  if (samus_collision_flag) {
    samus_movement_handler = FUNC16(Samus_MovementHandler_Normal);
    frame_handler_gamma = FUNC16(Samus_Func3);
    samus_var62 = 0;
    Samus_ClearMoveVars();
  } else {
    Samus_BombJumpFallingYMovement_();
  }
}
void Samus_Func6(void) {  // 0x90E21C
  Samus_BombJumpFallingXMovement_();
  if (samus_collision_flag) {
    samus_movement_handler = FUNC16(Samus_MovementHandler_Normal);
    frame_handler_gamma = FUNC16(Samus_Func3);
    samus_var62 = 0;
    Samus_ClearMoveVars();
  } else {
    Samus_BombJumpFallingYMovement_();
  }
}
void Samus_GrabbedByDraygonFrameHandler(void) {  // 0x90E2A1
  if (grapple_beam_function == FUNC16(GrappleBeamFunc_ConnectedLockedInPlace)) {
    samus_new_pose = -1;
    samus_momentum_routine_index = 0;
  }
  if ((joypad1_newkeys & (kButton_Up | kButton_Down | kButton_Left | kButton_Right)) != 0
      && (joypad1_newkeys & (kButton_Up | kButton_Down | kButton_Left | kButton_Right)) != suit_pickup_light_beam_pos) {
    suit_pickup_light_beam_pos = joypad1_newkeys & (kButton_Up | kButton_Down | kButton_Left | kButton_Right);
    if ((int16)(++substate - 60) >= 0)
      Samus_ReleaseFromDraygon();
  }
}

void Samus_SetGrabbedByDraygonPose(uint16 a) {  // 0x90E23B
  if ((a & 1) != 0)
    samus_pose = kPose_EC_FaceR_Draygon_NoMove_NoAim;
  else
    samus_pose = kPose_BA_FaceL_Draygon_NoMove_NoAim;
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
  frame_handler_gamma = FUNC16(Samus_GrabbedByDraygonFrameHandler);
  samus_movement_handler = FUNC16(nullsub_152);
  substate = 0;
  suit_pickup_light_beam_pos = 0;
  *(uint16 *)&suit_pickup_color_math_R = 0;
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

void Samus_ReleaseFromDraygon_(void) {  // 0x90E2D4
  Samus_ReleaseFromDraygon();
}

void Samus_ReleaseFromDraygon(void) {  // 0x90E2DE
  if (samus_pose_x_dir == 4)
    samus_pose = kPose_02_FaceL_Normal;
  else
    samus_pose = kPose_01_FaceR_Normal;
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
  samus_movement_handler = FUNC16(Samus_MovementHandler_Normal);
  frame_handler_gamma = FUNC16(nullsub_152);
  samus_last_different_pose = samus_prev_pose;
  *(uint16 *)&samus_last_different_pose_x_dir = *(uint16 *)&samus_prev_pose_x_dir;
  samus_prev_pose = samus_pose;
  *(uint16 *)&samus_prev_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
  samus_x_base_speed = 0;
  samus_x_base_subspeed = 0;
  samus_new_pose = -1;
  samus_new_pose_interrupted = -1;
  samus_new_pose_transitional = -1;
  samus_momentum_routine_index = 0;
  samus_special_transgfx_index = 0;
  samus_hurt_switch_index = 0;
  samus_y_speed = 0;
  samus_y_subspeed = 0;
  samus_y_dir = 0;
  used_for_ball_bounce_on_landing = 0;
  samus_x_accel_mode = 0;
  samus_grapple_flags = samus_grapple_flags & 0xFFFD | 2;
}

void Samus_Func7(void) {  // 0x90E3A3
  Samus_BombJumpFallingXMovement_();
  if (samus_collision_flag || (Samus_BombJumpFallingYMovement_(), samus_collision_flag)) {
    samus_movement_handler = FUNC16(Samus_MovementHandler_Normal);
    frame_handler_gamma = FUNC16(nullsub_152);
    samus_var62 = 0;
    Samus_ClearMoveVars();
    samus_new_pose_interrupted = 65;
    samus_special_transgfx_index = 0;
  }
}

void Samus_Func8(void) {  // 0x90E400
  samus_y_subspeed = 0;
  samus_y_speed = 0;
  samus_movement_handler = FUNC16(Samus_MovementHandler_Normal);
  frame_handler_gamma = FUNC16(nullsub_152);
}

void Samus_Func9(void) {  // 0x90E41B
  unsigned int v0; // kr00_4

  if (sign16(samus_y_speed - 5)) {
    v0 = __PAIR32__(samus_y_accel, samus_y_subaccel) + __PAIR32__(samus_y_speed, samus_y_subspeed);
    samus_y_speed = HIWORD(v0);
    samus_y_subspeed = v0;
  }
  if ((samus_pose == kPose_29_FaceR_Fall
       || samus_pose == kPose_2A_FaceL_Fall
       || samus_pose == kPose_67_FaceR_Fall_Gun
       || samus_pose == kPose_68_FaceL_Fall_Gun)
      && !sign16(samus_y_speed - 5)) {
    samus_anim_frame_timer = 16;
    samus_anim_frame = 4;
  }
}

void Samus_CalcDisplacementMoveLeft(void) {  // 0x90E464
  int16 v1;

  Samus_CalcSpeed_X();
  samus_collision_direction = 0;
  R18_ = ~R18_;
  bool v0 = R20_ == 0;
  R20_ = -R20_;
  if (v0)
    ++R18_;
  v1 = (__PAIR32__(R18_, R20_) + __PAIR32__(extra_samus_x_displacement, extra_samus_x_subdisplacement)) >> 16;
  R20_ += extra_samus_x_subdisplacement;
  R18_ = v1;
  if (v1 < 0) {
    if (sign16(v1 + 15))
      R18_ = -15;
  } else if (!sign16(v1 - 16)) {
    R18_ = 15;
  }
}

void Samus_CalcDisplacementMoveRight(void) {  // 0x90E4AD
  int16 v0;

  Samus_CalcSpeed_X();
  samus_collision_direction = 1;
  v0 = (__PAIR32__(R18_, R20_) + __PAIR32__(extra_samus_x_displacement, extra_samus_x_subdisplacement)) >> 16;
  R20_ += extra_samus_x_subdisplacement;
  R18_ = v0;
  if (v0 < 0) {
    if (sign16(v0 + 15))
      R18_ = -15;
  } else if (!sign16(v0 - 16)) {
    R18_ = 15;
  }
}

void Samus_CalcSpeed_X(void) {  // 0x90E4E6
  int16 v1;
  uint16 v3;
  char v4; // t1
  uint16 v5;
  char v6; // tt
  uint16 v7;
  char v8; // t1
  uint16 v9;
  char v10; // tt

  uint16 v0 = samus_x_speed_divisor;
  if (!sign16(samus_x_speed_divisor - 5))
    v0 = 4;
  v1 = 2 * v0;
  if (v1) {
    switch (v1) {
    case 2: {
      bool v2 = __CFADD__uint16(samus_x_extra_run_subspeed, R20_);
      R20_ += samus_x_extra_run_subspeed;
      LOBYTE(v3) = (uint16)(samus_x_extra_run_speed + v2 + R18_) >> 8;
      HIBYTE(v3) = samus_x_extra_run_speed + v2 + R18_;
      v3 >>= 1;
      v4 = v3;
      LOBYTE(v3) = HIBYTE(v3);
      HIBYTE(v3) = v4;
      R18_ = (uint8)v3;
      samus_total_x_speed = (uint8)v3;
      R22_ = v3 & 0xFF00;
      R20_ = (v3 & 0xFF00) + (R20_ >> 1);
      samus_total_x_subspeed = R20_;
      break;
    }
    case 4: {
      bool v2 = __CFADD__uint16(samus_x_extra_run_subspeed, R20_);
      R20_ += samus_x_extra_run_subspeed;
      LOBYTE(v5) = (uint16)(samus_x_extra_run_speed + v2 + R18_) >> 8;
      HIBYTE(v5) = samus_x_extra_run_speed + v2 + R18_;
      v5 >>= 2;
      v6 = v5;
      LOBYTE(v5) = HIBYTE(v5);
      HIBYTE(v5) = v6;
      R18_ = (uint8)v5;
      samus_total_x_speed = (uint8)v5;
      R22_ = v5 & 0xFF00;
      R20_ = (v5 & 0xFF00) + (R20_ >> 2);
      samus_total_x_subspeed = R20_;
      break;
    }
    case 6: {
      bool v2 = __CFADD__uint16(samus_x_extra_run_subspeed, R20_);
      R20_ += samus_x_extra_run_subspeed;
      LOBYTE(v7) = (uint16)(samus_x_extra_run_speed + v2 + R18_) >> 8;
      HIBYTE(v7) = samus_x_extra_run_speed + v2 + R18_;
      v7 >>= 3;
      v8 = v7;
      LOBYTE(v7) = HIBYTE(v7);
      HIBYTE(v7) = v8;
      R18_ = (uint8)v7;
      samus_total_x_speed = (uint8)v7;
      R22_ = v7 & 0xFF00;
      R20_ = (v7 & 0xFF00) + (R20_ >> 3);
      samus_total_x_subspeed = R20_;
      break;
    }
    case 8: {
      bool v2 = __CFADD__uint16(samus_x_extra_run_subspeed, R20_);
      R20_ += samus_x_extra_run_subspeed;
      LOBYTE(v9) = (uint16)(samus_x_extra_run_speed + v2 + R18_) >> 8;
      HIBYTE(v9) = samus_x_extra_run_speed + v2 + R18_;
      v9 >>= 4;
      v10 = v9;
      LOBYTE(v9) = HIBYTE(v9);
      HIBYTE(v9) = v10;
      R18_ = (uint8)v9;
      samus_total_x_speed = (uint8)v9;
      R22_ = v9 & 0xFF00;
      R20_ = (v9 & 0xFF00) + (R20_ >> 4);
      samus_total_x_subspeed = R20_;
      break;
    }
    default:
      Unreachable();
      while (1)
        ;
    }
  } else {
    bool v2 = __CFADD__uint16(samus_x_extra_run_subspeed, R20_);
    R20_ += samus_x_extra_run_subspeed;
    samus_total_x_subspeed = R20_;
    R18_ += samus_x_extra_run_speed + v2;
    samus_total_x_speed = R18_;
  }
}

void Samus_ClearXSpeedIfColl(void) {  // 0x90E5CE
  if (samus_collision_flag) {
    if (samus_collision_direction)
      samus_collides_with_solid_enemy = 8;
    else
      samus_collides_with_solid_enemy = 4;
    Samus_CancelSpeedBoost();
    samus_x_extra_run_speed = 0;
    samus_x_extra_run_subspeed = 0;
    samus_x_base_speed = 0;
    samus_x_base_subspeed = 0;
    samus_x_accel_mode = 0;
  } else {
    input_to_pose_calc = 0;
    samus_collides_with_solid_enemy = 0;
  }
}

void Samus_MoveUp_SetPoseCalcInput(void) {  // 0x90E606
  if (samus_collision_flag)
    input_to_pose_calc = 4;
  else
    input_to_pose_calc = 0;
}

static const uint8 kSamus_MoveDown_SetPoseCalcInput_Tab0[28] = {  // 0x90E61B
  0, 0, 4, 4, 1, 0, 4, 2,
  4, 4, 0, 0, 0, 0, 4, 4,
  0, 3, 4, 4, 4, 0, 4, 4,
  4, 4, 4, 4,
};
static const uint8 kSamus_MoveDown_SetPoseCalcInput_Tab1[28] = {
  4, 4, 0, 0, 4, 4, 0, 4,
  1, 2, 0, 4, 4, 0, 4, 4,
  4, 4, 3, 3, 0, 4, 4, 4,
  4, 0, 4, 4,
};
void Samus_MoveDown_SetPoseCalcInput(void) {
  if (samus_collision_flag) {
    input_to_pose_calc = 1;
    HIBYTE(input_to_pose_calc) = kSamus_MoveDown_SetPoseCalcInput_Tab1[samus_movement_type];
  } else if ((uint8)input_to_pose_calc != 5) {
    input_to_pose_calc = 2;
    HIBYTE(input_to_pose_calc) = kSamus_MoveDown_SetPoseCalcInput_Tab0[samus_movement_type];
  }
}

void CallFrameHandlerAlfa(uint32 ea) {
  switch (ea) {
  case fnSamus_FrameHandlerAlfa_Func11: Samus_FrameHandlerAlfa_Func11(); return;
  case fnSamus_FrameHandlerAlfa_Func12: Samus_FrameHandlerAlfa_Func12(); return;
  case fnSamus_FrameHandlerAlfa_Func13: Samus_FrameHandlerAlfa_Func13(); return;
  case fnEmptyFunction: return;
  default: Unreachable();
  }
}
void HandleControllerInputForGamePhysics(void) {  // 0x90E692
  CallFrameHandlerAlfa(frame_handler_alfa | 0x900000);
}

void Samus_FrameHandlerAlfa_Func11(void) {  // 0x90E695
  samus_new_pose = -1;
  samus_new_pose_interrupted = -1;
  samus_new_pose_transitional = -1;
  samus_momentum_routine_index = 0;
  samus_special_transgfx_index = 0;
  samus_hurt_switch_index = 0;
  Samus_SetRadius();
  Samus_CallInputHandler();
  Samus_UpdateSuitPaletteIndex();
  Samus_DetermineAccel_Y();
  BlockInsideDetection();
  Samus_HandleHudSpecificBehaviorAndProjs();
  Samus_Func10();
}

void Samus_FrameHandlerAlfa_Func12(void) {  // 0x90E6C9
  samus_new_pose = -1;
  samus_new_pose_interrupted = -1;
  samus_new_pose_transitional = -1;
  samus_momentum_routine_index = 0;
  samus_special_transgfx_index = 0;
  samus_hurt_switch_index = 0;
  controller1_input_for_demo = joypad1_lastkeys;
  controller1_new_input_for_demo = joypad1_newkeys;
  demo_backup_prev_controller_input = joypad1_input_samusfilter;
  demo_backup_prev_controller_input_new = joypad1_newinput_samusfilter;
  Samus_SetRadius();
  Samus_UpdateSuitPaletteIndex();
  Samus_CallInputHandler();
  Samus_DetermineAccel_Y();
  BlockInsideDetection();
  Samus_HandleHudSpecificBehaviorAndProjs();
  Samus_Func10();
}

void Samus_FrameHandlerAlfa_Func13(void) {  // 0x90E713
  HandleProjectile();
  Samus_Func10();
}

void CallFrameHandlerBeta(uint32 ea) {
  switch (ea) {
  case fnSamus_FrameHandlerBeta_Func17: Samus_FrameHandlerBeta_Func17(); return;
  case fnHandleDemoRecorder_3: HandleDemoRecorder_3(); return;
  case fnSamus_FrameHandlerBeta_Func14: Samus_FrameHandlerBeta_Func14(); return;
  case fnSamus_Func15: Samus_Func15(); return;
  case fnSamus_Func16: Samus_Func16(); return;
  case fnSamus_Func18: Samus_Func18(); return;
  case fnEmptyFunction: return;
  case fnj_HandleDemoRecorder_2: return;
  case fnj_HandleDemoRecorder_2_0: return;
  case fnSetContactDamageIndexAndUpdateMinimap: SetContactDamageIndexAndUpdateMinimap(); return;
  case fnSamus_Func19: Samus_Func19(); return;
  case fnSamus_LowHealthCheck: Samus_LowHealthCheck(); return;
  default: Unreachable();
  }
}

void HandleSamusMovementAndPause(void) {  // 0x90E722
  CallFrameHandlerBeta(frame_handler_beta | 0x900000);
}

void Samus_FrameHandlerBeta_Func17(void) {  // 0x90E725
  samus_contact_damage_index = 0;
  RunSamusMovementHandler();
  UpdateMinimap();
  RunFrameHandlerGamma();
  Samus_Animate();
  Samus_HitInterruption();
  Samus_HandleTransFromBlockColl();
  Samus_HandleTransitions();
  Samus_HandlePalette();
  Samus_HandlePeriodicDamage();
  Samus_PauseCheck();
  Samus_LowHealthCheck_();
}

void HandleDemoRecorder_1(void) {  // 0x90E786
  if ((joypad2_new_keys & 0x8000u) == 0) {
    if (!debug_flag && (joypad2_new_keys & 0x80) != 0) {
      DisableEnemyProjectiles();
      time_is_frozen_flag = 1;
      frame_handler_alfa = FUNC16(EmptyFunction);
      frame_handler_beta = FUNC16(HandleDemoRecorder_3);
    }
  } else if (debug_flag) {
    samus_draw_handler = FUNC16(nullsub_152);
    debug_flag = 0;
  } else {
    debug_flag = 1;
    samus_draw_handler = FUNC16(SamusDrawHandler_Default);
  }
}

void HandleDemoRecorder_3(void) {  // 0x90E7D2
  if ((joypad2_new_keys & 0x80) != 0) {
    frame_handler_alfa = FUNC16(Samus_FrameHandlerAlfa_Func11);
    frame_handler_beta = FUNC16(Samus_FrameHandlerBeta_Func17);
    EnableEnemyProjectiles();
    time_is_frozen_flag = 0;
  }
}

void Samus_FrameHandlerBeta_Func14(void) {  // 0x90E7F5
  samus_contact_damage_index = 0;
  RunSamusMovementHandler();
  UpdateMinimap();
  Samus_Animate();
  Samus_HitInterruption();
  Samus_HandleTransFromBlockColl();
  Samus_HandleTransitions();
  Samus_HandlePalette();
  joypad1_lastkeys = controller1_input_for_demo;
  joypad1_newkeys = controller1_new_input_for_demo;
  joypad1_input_samusfilter = demo_backup_prev_controller_input;
  joypad1_newinput_samusfilter = demo_backup_prev_controller_input_new;
}

void Samus_Func15(void) {  // 0x90E833
  samus_contact_damage_index = 0;
  RunSamusMovementHandler();
  Samus_Animate();
  Samus_HitInterruption();
  Samus_HandleTransFromBlockColl();
  Samus_HandleTransitions();
  Samus_HandlePalette();
  joypad1_lastkeys = controller1_input_for_demo;
  joypad1_newkeys = controller1_new_input_for_demo;
  joypad1_input_samusfilter = demo_backup_prev_controller_input;
  joypad1_newinput_samusfilter = demo_backup_prev_controller_input_new;
}

void Samus_Func16(void) {  // 0x90E86A
  Samus_SetRadius();
  UpdateMinimap();
  Samus_Animate();
  elevator_status = 0;
  samus_prev_y_pos = samus_y_pos;
  if (PlaySamusFanfare() & 1) {
    if (sign16(debug_invincibility - 7) || (joypad2_last & 0x8000u) == 0)
      debug_invincibility = 0;
    frame_handler_alfa = FUNC16(Samus_FrameHandlerAlfa_Func11);
    frame_handler_beta = FUNC16(Samus_FrameHandlerBeta_Func17);
  }
}

void Samus_Func18(void) {  // 0x90E8AA
  Samus_FrameHandlerBeta_Func17();
  if (frame_handler_gamma == FUNC16(DrawTimer_) && game_state != kGameState_35_TimeUp)
    game_state = kGameState_35_TimeUp;
}

void SetContactDamageIndexAndUpdateMinimap(void) {  // 0x90E8DC
  samus_contact_damage_index = 0;
  UpdateMinimap();
}

void Samus_Func19(void) {  // 0x90E8EC
  samus_contact_damage_index = 0;
  RunSamusMovementHandler();
  UpdateMinimap();
  Samus_Animate();
}

void Samus_LowHealthCheck(void) {  // 0x90E902
  Samus_LowHealthCheck_();
}

void CallSamusInputHandler(uint32 ea) {
  switch (ea) {
  case fnnullsub_152: return;
  case fnSamus_InputHandler_E913: Samus_InputHandler_E913(); return;
  case fnSamus_Func20_: Samus_Func20_(); return;
  case fnSamus_InputHandler_E91D: Samus_InputHandler_E91D(); return;
  case fnHandleAutoJumpHack: HandleAutoJumpHack(); return;
  default: Unreachable();
  }
}

void Samus_CallInputHandler(void) {  // 0x90E90F
  CallSamusInputHandler(samus_input_handler | 0x900000);
}

void Samus_InputHandler_E913(void) {  // 0x90E913
  Samus_InputHandler();
}

void Samus_Func20_(void) {  // 0x90E918
  Samus_Func20();
}

void Samus_InputHandler_E91D(void) {  // 0x90E91D
  DemoObjectInputHandler();
  Samus_InputHandler();
}

void HandleAutoJumpHack(void) {  // 0x90E926
  uint16 v0 = joypad1_newkeys;
  if (autojump_timer && sign16(autojump_timer - 9)) {
    joypad1_newkeys |= button_config_jump_a;
    autojump_timer = 0;
  }
  Samus_InputHandler();
  joypad1_newkeys = v0;
  samus_input_handler = FUNC16(Samus_InputHandler_E913);
}

void CallSamusMovementHandler(uint32 ea) {
  switch (ea) {
  case fnSamus_MoveHandler_ReleaseFromGrapple: Samus_MoveHandler_ReleaseFromGrapple(); return;
  case fnSamus_HandleMovement_DrainedCrouching: Samus_HandleMovement_DrainedCrouching(); return;
  case fnSamus_MovementHandler_Normal: Samus_MovementHandler_Normal(); return;
  case fnSamus_MoveHandlerShinesparkWindup: Samus_MoveHandlerShinesparkWindup(); return;
  case fnSamus_MoveHandlerVerticalShinespark: Samus_MoveHandlerVerticalShinespark(); return;
  case fnSamus_MoveHandler_Shinespark_Diag: Samus_MoveHandler_Shinespark_Diag(); return;
  case fnSamus_MoveHandler_Shinespark_Horiz: Samus_MoveHandler_Shinespark_Horiz(); return;
  case fnSamus_MoveHandler_ShinesparkCrash: Samus_MoveHandler_ShinesparkCrash(); return;
  case fnSamus_MoveHandler_ShinesparkCrash_0: Samus_MoveHandler_ShinesparkCrash_0(); return;
  case fnSamus_MoveHandler_ShinesparkCrash_1: Samus_MoveHandler_ShinesparkCrash_1(); return;
  case fnSamus_MoveHandler_ShinesparkCrash_2: Samus_MoveHandler_ShinesparkCrash_2(); return;
  case fnSamus_MoveHandler_ShinesparkCrashEchoCircle: Samus_MoveHandler_ShinesparkCrashEchoCircle(); return;
  case fnSamus_MoveHandler_ShinesparkCrashFinish: Samus_MoveHandler_ShinesparkCrashFinish(); return;
  case fnSamusMoveHandler_CrystalFlashStart: SamusMoveHandler_CrystalFlashStart(); return;
  case fnSamusMoveHandler_CrystalFlashMain: SamusMoveHandler_CrystalFlashMain(); return;
  case fnkSamusMoveHandler_CrystalFlashFinish: kSamusMoveHandler_CrystalFlashFinish(); return;
  case fnSamus_MoveHandler_Knockback: Samus_MoveHandler_Knockback(); return;
  case fnSamus_MoveHandler_Knockback_0: Samus_MoveHandler_Knockback_0(); return;
  case fnSamus_MoveHandler_Knockback_Up: Samus_MoveHandler_Knockback_Up(); return;
  case fnSamus_MoveHandler_Knockback_3: Samus_MoveHandler_Knockback_3(); return;
  case fnSamus_MoveHandler_Knockback_Down: Samus_MoveHandler_Knockback_Down(); return;
  case fnSamus_MoveHandler_BombJumpStart: Samus_MoveHandler_BombJumpStart(); return;
  case fnSamus_MoveHandler_BombJumpMain: Samus_MoveHandler_BombJumpMain(); return;
  case fnSamus_MoveHandler_BombJumpFunc1: Samus_MoveHandler_BombJumpFunc1(); return;
  case fnnullsub_152: return;
  case fnSamusMovementType_Xray: SamusMovementType_Xray(); return;
  case fnSamus_Func25_ShineSpark: Samus_Func25_ShineSpark(); return;
  case fnSamus_MoveHandler_F072: Samus_MoveHandler_F072(); return;
  default: Unreachable();
  }
}

void RunSamusMovementHandler(void) {  // 0x90E94B
  CallSamusMovementHandler(samus_movement_handler | 0x900000);
}

void SamusMovementType_Xray(void) {  // 0x90E94F
  uint16 v0;
  if (samus_movement_type != kMovementType_0E_TurningAroundOnGround) {
    samus_anim_frame_timer = 15;
    if (samus_pose_x_dir == 4) {
      if (sign16(xray_angle - 153)) {
        v0 = 4;
      } else if (sign16(xray_angle - 178)) {
        v0 = 3;
      } else if (sign16(xray_angle - 203)) {
        v0 = 2;
      } else {
        v0 = sign16(xray_angle - 228) != 0;
      }
    } else if (sign16(xray_angle - 25)) {
      v0 = 0;
    } else if (sign16(xray_angle - 50)) {
      v0 = 1;
    } else if (sign16(xray_angle - 75)) {
      v0 = 2;
    } else if (sign16(xray_angle - 100)) {
      v0 = 3;
    } else {
      v0 = 4;
    }
    samus_anim_frame = v0;
  }
}

void Samus_HandlePeriodicDamage(void) {  // 0x90E9CE
  int16 v0;
  int16 v1;
  int16 v2;

  if (time_is_frozen_flag)
    goto LABEL_10;
  if ((equipped_items & 0x20) != 0) {
    uint16 v4 = *(uint16 *)((char *)&samus_periodic_subdamage + 1) >> 2;
    LOBYTE(v1) = (uint16)(*(uint16 *)((char *)&samus_periodic_subdamage + 1) >> 2) >> 8;
    HIBYTE(v1) = *(uint16 *)((char *)&samus_periodic_subdamage + 1) >> 2;
    samus_periodic_subdamage = v1 & 0xFF00;
    samus_periodic_damage = HIBYTE(v4);
  } else if ((equipped_items & 1) != 0) {
    uint16 v3 = *(uint16 *)((char *)&samus_periodic_subdamage + 1) >> 1;
    LOBYTE(v0) = (uint16)(*(uint16 *)((char *)&samus_periodic_subdamage + 1) >> 1) >> 8;
    HIBYTE(v0) = *(uint16 *)((char *)&samus_periodic_subdamage + 1) >> 1;
    samus_periodic_subdamage = v0 & 0xFF00;
    samus_periodic_damage = HIBYTE(v3);
  }
  if ((samus_periodic_damage & 0x8000u) != 0) {
    InvalidInterrupt_Crash();
    return;
  }
  v2 = (__PAIR32__(samus_health, samus_subunit_health) - __PAIR32__(samus_periodic_damage, samus_periodic_subdamage)) >> 16;
  samus_subunit_health -= samus_periodic_subdamage;
  samus_health = v2;
  if (v2 >= 0) {
LABEL_10:
    samus_periodic_subdamage = 0;
    samus_periodic_damage = 0;
  } else {
    samus_subunit_health = 0;
    samus_health = 0;
    samus_periodic_subdamage = 0;
    samus_periodic_damage = 0;
  }
}

void Samus_PauseCheck(void) {  // 0x90EA45
  if (!power_bomb_flag
      && !time_is_frozen_flag
      && !door_transition_flag_enemies
      && area_index != 6
      && game_state == kGameState_8_MainGameplay
      && (joypad1_newkeys & kButton_Start) != 0) {
    screen_fade_delay = 1;
    screen_fade_counter = 1;
    game_state = kGameState_12_Pausing;
  }
}

void Samus_LowHealthCheck_(void) {  // 0x90EA7F
  if (sign16(samus_health - 31)) {
    if (!samus_health_warning) {
      QueueSfx3_Max6(2u);
      samus_health_warning = 1;
    }
  } else if (samus_health_warning) {
    samus_health_warning = 0;
    QueueSfx3_Max6(1u);
  }
}

void Samus_LowHealthCheck_0(void) {  // 0x90EAAB
  Samus_LowHealthCheck_();
}

void Samus_JumpCheck(void) {  // 0x90EAB3
  if ((button_config_jump_a & joypad1_lastkeys) != 0 && (button_config_jump_a & joypad1_input_samusfilter) != 0)
    ++autojump_timer;
  else
    autojump_timer = 0;
  joypad1_input_samusfilter = joypad1_lastkeys;
  joypad1_newinput_samusfilter = joypad1_newkeys;
  if ((int16)(samus_health - samus_prev_health_for_flash) >= 0)
    goto LABEL_10;
  if (!samus_hurt_flash_counter)
    samus_hurt_flash_counter = 1;
  if (sign16(debug_invincibility - 7))
    LABEL_10:
  samus_prev_health_for_flash = samus_health;
  else
    samus_health = samus_prev_health_for_flash;
}

void Samus_Func10(void) {  // 0x90EB02
  int16 v0;

  projectile_init_speed_samus_moved_left = 0;
  projectile_init_speed_samus_moved_left_fract = 0;
  projectile_init_speed_samus_moved_right = 0;
  projectile_init_speed_samus_moved_right_fract = 0;
  projectile_init_speed_samus_moved_up = 0;
  projectile_init_speed_samus_moved_up_fract = 0;
  projectile_init_speed_samus_moved_down = 0;
  projectile_init_speed_samus_moved_down_fract = 0;
  samus_anim_frame_skip = 0;
  new_projectile_direction_changed_pose = 0;
  LOBYTE(v0) = HIBYTE(UNUSED_word_7E0DFA);
  HIBYTE(v0) = UNUSED_word_7E0DFA;
  UNUSED_word_7E0DFA = v0 & 0xFF00;
  WORD(g_ram[0xa10]) = *(uint16 *)&samus_pose_x_dir;
}

void DrawSamusAndProjectiles(void) {  // 0x90EB35
  SamusDrawSprites();
  DrawPlayerExplosions2();
  Samus_JumpCheck();
  Samus_ShootCheck();
}

void CallSamusDrawHandler(uint32 ea) {
  switch (ea) {
  case fnSamusDrawHandler_Default: SamusDrawHandler_Default(); return;
  case fnsub_90EB86: sub_90EB86(); return;
  case fnnullsub_152:
  case fnnullsub_156: return;
  case fnSamus_DrawHandler_EndOfShinespark: Samus_DrawHandler_EndOfShinespark(); return;
  case fnSamusDisplayHandler_UsingElevator: SamusDisplayHandler_UsingElevator(); return;
  case fnSamusDisplayHandler_SamusReceivedFatal: SamusDisplayHandler_SamusReceivedFatal(); return;
  default: Unreachable();
  }
}

void SamusDrawSprites(void) {  // 0x90EB4B
  HandleArmCannonOpenState();
  CallSamusDrawHandler(samus_draw_handler | 0x900000);
}

void SamusDrawHandler_Default(void) {  // 0x90EB52
  HandleChargingBeamGfxAudio();
  sub_90EB55();
}

void sub_90EB55(void) {  // 0x90EB55
  if ((arm_cannon_drawing_mode & 0xF) != 0) {
    if ((arm_cannon_drawing_mode & 0xF) == 2) {
      HandleAtmosphericEffects();
      Samus_Draw();
      Samus_ArmCannon_Draw();
    } else {
      HandleAtmosphericEffects();
      Samus_ArmCannon_Draw();
      Samus_Draw();
    }
    Samus_DrawEchoes();
  } else {
    HandleAtmosphericEffects();
    Samus_Draw();
    Samus_DrawEchoes();
  }
}

void sub_90EB86(void) {  // 0x90EB86
  if (!sign16(grapple_beam_function + 0x37AA)) {
    sub_90EB55();
    return;
  }
  if (grapple_beam_function == FUNC16(GrappleBeamFunc_Firing)
      || grapple_beam_function == FUNC16(UNUSED_sub_9BC759)) {
    GrappleBeamFunc_BF1B();
  }
  HandleGrappleBeamFlare();
  if ((arm_cannon_drawing_mode & 0xF) != 0) {
    if ((arm_cannon_drawing_mode & 0xF) == 2) {
      HandleAtmosphericEffects();
      Samus_Draw();
      Samus_ArmCannon_Draw();
      UpdateGrappleBeamTilesAndIncrFlameCtr();
      if (!grapple_beam_length)
        return;
    } else {
      HandleAtmosphericEffects();
      Samus_ArmCannon_Draw();
      Samus_Draw();
      UpdateGrappleBeamTilesAndIncrFlameCtr();
      if (!grapple_beam_length)
        return;
    }
  } else {
    HandleAtmosphericEffects();
    Samus_Draw();
    UpdateGrappleBeamTilesAndIncrFlameCtr();
    if (!grapple_beam_length)
      return;
  }
  HandleGrappleBeamGfx();
}

void Samus_DrawHandler_EndOfShinespark(void) {  // 0x90EBF3
  Samus_Draw();
  for (int i = 2; i >= 0; i -= 2)
    Samus_DrawShinesparkCrashEchoes(i);
}

void Samus_DrawManyEchoes(void) {  // 0x90EC03
  SamusDrawHandler_Default();
  for (int i = 2; i >= 0; i -= 2)
    Samus_DrawEchoes();
}

void SamusDisplayHandler_UsingElevator(void) {  // 0x90EC14
  if ((nmi_frame_counter_word & 1) == 0)
    SamusDisplayHandler_SamusReceivedFatal();
}

void SamusDisplayHandler_SamusReceivedFatal(void) {  // 0x90EC1D
  Samus_DrawWhenNotAnimatingOrDying();
}

void Samus_SetRadius(void) {  // 0x90EC22
  samus_y_radius = *(&kPoseParams[0].y_radius + (uint16)(8 * samus_pose));
  samus_x_radius = 5;
}

void Samus_GetBottomBoundary(void) {  // 0x90EC3E
  R18_ = samus_y_pos + *(&kPoseParams[0].y_radius + (uint16)(8 * samus_pose)) - 1;
}

void Samus_GetTopBottomBoundary(void) {  // 0x90EC58
  int16 v0;

  v0 = *(&kPoseParams[0].y_radius + (uint16)(8 * samus_pose));
  R18_ = samus_y_pos + v0 - 1;
  R20_ = samus_y_pos - v0;
}

void Samus_AlignBottomWithPrevPose(void) {  // 0x90EC7E
  R18_ = *(&kPoseParams[0].y_radius + (uint16)(8 * samus_pose));
  R18_ = *(&kPoseParams[0].y_radius + (uint16)(8 * samus_prev_pose)) - R18_;
  samus_y_pos += R18_;
  samus_prev_y_pos += R18_;
}

void Samus_UpdateSuitPaletteIndex(void) {  // 0x90ECB6
  if ((equipped_items & 0x20) != 0) {
    samus_suit_palette_index = 4;
  } else if ((equipped_items & 1) != 0) {
    samus_suit_palette_index = 2;
  } else {
    samus_suit_palette_index = 0;
  }
}

void MoveSamusWithControlPad(void) {  // 0x90ECD5
  if ((joypad1_lastkeys & 0x800) != 0) {
    R18_ = -4;
    R20_ = 0;
    Samus_MoveUp();
  }
  if ((joypad1_lastkeys & 0x400) != 0) {
    R18_ = 4;
    R20_ = 0;
    Samus_MoveDown();
  }
  if ((joypad1_lastkeys & 0x200) != 0) {
    R18_ = -4;
    R20_ = 0;
    Samus_MoveLeft();
  }
  if ((joypad1_lastkeys & 0x100) != 0) {
    R18_ = 4;
    R20_ = 0;
    Samus_MoveRight();
  }
}

static const uint8 byte_90ED50[28] = {  // 0x90ED26
  0, 0, 0, 0, 1, 0, 0, 1,
  1, 1, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0,
};

void SwappedAmmoRoutine(void) {
  if (byte_90ED50[samus_movement_type]) {
    samus_missiles = samus_x_pos >> 4;
    samus_max_missiles = samus_x_pos >> 4;
    samus_super_missiles = samus_y_pos >> 4;
    samus_max_super_missiles = samus_y_pos >> 4;
  }
}

void UNUSEDsub_90ED6C(void) {  // 0x90ED6C
  samus_missiles = game_time_hours;
  samus_max_missiles = game_time_hours;
  samus_super_missiles = game_time_minutes;
  samus_max_super_missiles = game_time_minutes;
  samus_power_bombs = game_time_seconds;
  samus_max_power_bombs = game_time_seconds;
}

static Func_V *const kSamus_FootstepGraphics[8] = {  // 0x90ED88
  Samus_FootstepGraphics_Crateria,
  Samus_FootstepGraphics_1,
  Samus_FootstepGraphics_1,
  Samus_FootstepGraphics_1,
  Samus_FootstepGraphics_Maridia,
  Samus_FootstepGraphics_1,
  Samus_FootstepGraphics_1,
  Samus_FootstepGraphics_1,
};
void Samus_FootstepGraphics(void) {
  kSamus_FootstepGraphics[area_index]();
}

static const uint8 byte_90EDC9[16] = {  // 0x90EDA1
  1, 0, 0, 0, 0, 2, 0, 4,
  0, 4, 4, 4, 4, 0, 4, 0,
};
void Samus_FootstepGraphics_Crateria(void) {
  if (cinematic_function || (int16)(room_index - 16) >= 0)
    goto LABEL_11;
  if ((byte_90EDC9[room_index] & 1) == 0) {
    if ((byte_90EDC9[room_index] & 2) != 0) {
      if (!sign16(samus_y_pos - 944))
        goto LABEL_12;
    } else if ((byte_90EDC9[room_index] & 4) != 0) {
      goto LABEL_12;
    }
LABEL_11:
    Samus_FootstepGraphics_1();
    return;
  }
  if (fx_type != 10)
    goto LABEL_11;
LABEL_12:
  Samus_FootstepGraphics_Maridia();
}

void Samus_FootstepGraphics_Maridia(void) {  // 0x90EDEC
  Samus_GetBottomBoundary();
  if ((fx_y_pos & 0x8000u) != 0) {
    if ((lava_acid_y_pos & 0x8000u) == 0 && sign16(lava_acid_y_pos - R18_))
      return;
  } else if (sign16(fx_y_pos - R18_) && (fx_liquid_options & 4) == 0) {
    return;
  }
  if (samus_pose_x_dir == 4) {
    atmospheric_gfx_x_pos[0] = samus_x_pos - 12;
    atmospheric_gfx_x_pos[1] = samus_x_pos + 8;
  } else {
    atmospheric_gfx_x_pos[0] = samus_x_pos + 12;
    atmospheric_gfx_x_pos[1] = samus_x_pos - 8;
  }
  atmospheric_gfx_y_pos[0] = samus_y_pos + 16;
  atmospheric_gfx_y_pos[1] = samus_y_pos + 16;
  atmospheric_gfx_frame_and_type[0] = 256;
  atmospheric_gfx_frame_and_type[1] = 256;
  atmospheric_gfx_anim_timer[0] = -32766;
  atmospheric_gfx_anim_timer[1] = 3;
}

void Samus_FootstepGraphics_1(void) {  // 0x90EE64
  if ((speed_boost_counter & 0xFF00) == 1024) {
    Samus_GetBottomBoundary();
    if ((fx_y_pos & 0x8000u) != 0) {
      if ((lava_acid_y_pos & 0x8000u) == 0 && sign16(lava_acid_y_pos - R18_))
        return;
    } else if (sign16(fx_y_pos - R18_) && (fx_liquid_options & 4) == 0) {
      return;
    }
    if (samus_pose_x_dir == 4) {
      atmospheric_gfx_x_pos[0] = samus_x_pos - 12;
      atmospheric_gfx_x_pos[1] = samus_x_pos + 8;
    } else {
      atmospheric_gfx_x_pos[0] = samus_x_pos + 12;
      atmospheric_gfx_x_pos[1] = samus_x_pos - 8;
    }
    atmospheric_gfx_y_pos[0] = samus_y_pos + 16;
    atmospheric_gfx_y_pos[1] = samus_y_pos + 16;
    atmospheric_gfx_frame_and_type[0] = 1792;
    atmospheric_gfx_frame_and_type[1] = 1792;
    atmospheric_gfx_anim_timer[0] = -32766;
    atmospheric_gfx_anim_timer[1] = 3;
  }
}

void Samus_UpdateSpeedEchoPos(void) {  // 0x90EEE7
  if ((speed_boost_counter & 0xFF00) == 1024 && (speed_echoes_index & 0x8000u) == 0 && (game_time_frames & 3) == 0) {
    uint16 v0 = speed_echoes_index;
    int v1 = speed_echoes_index >> 1;
    speed_echo_xpos[v1] = samus_x_pos;
    speed_echo_ypos[v1] = samus_y_pos;
    uint16 v2 = v0 + 2;
    if ((int16)(v2 - 4) >= 0)
      v2 = 0;
    speed_echoes_index = v2;
  }
}

void Samus_PostGrappleCollisionDetect(void) {  // 0x90EF22
  PostGrappleCollisionDetect_X();
  PostGrappleCollisionDetect_Y();
  if (!distance_to_eject_samus_down || !distance_to_eject_samus_up) {
    if (distance_to_eject_samus_up) {
      samus_y_pos -= distance_to_eject_samus_up;
      if (!sign16(samus_y_radius - 16)) {
        PostGrappleCollisionDetect_Y();
        samus_y_pos -= distance_to_eject_samus_up;
      }
    }
  }
}


void ProjPreInstr_UnknownProj8027(uint16 k) {  // 0x90EFD3
  static const int16 kProjPreInstr_UnknownProj8027_X[4] = { -4, -4, 4, 4 };
  static const int16 kProjPreInstr_UnknownProj8027_Y[4] = { 4, -4, -4, 4 };
  static const int16 kProjPreInstr_UnknownProj8027_X2[4] = { 0x80, 0x80, -0x80, -0x80 };
  static const int16 kProjPreInstr_UnknownProj8027_Y2[4] = { -0x80, 0x80, 0x80, -0x80 };

  int v1 = k >> 1;
  projectile_x_pos[v1] += kProjPreInstr_UnknownProj8027_X[v1];
  uint16 v2 = kProjPreInstr_UnknownProj8027_Y[v1] + projectile_y_pos[v1];
  projectile_y_pos[v1] = v2;
  if (v2 == samus_y_pos) {
    if (projectile_variables[v1] == 1) {
      if (k == 6)
        samus_movement_handler = FUNC16(Samus_Func25_ShineSpark);
      ClearProjectile(k);
    } else {
      int v3 = k >> 1;
      ++projectile_variables[v3];
      samus_shine_timer = 180;
      timer_for_shine_timer = 1;
      special_samus_palette_frame = 0;
      projectile_x_pos[v3] = kProjPreInstr_UnknownProj8027_X2[v3] + samus_x_pos;
      projectile_y_pos[v3] = kProjPreInstr_UnknownProj8027_Y2[v3] + samus_y_pos;
    }
  }
}

void Samus_Func25_ShineSpark(void) {  // 0x90F04B
  if (!samus_shine_timer) {
    special_samus_palette_frame = 6;
    special_samus_palette_timer = 1;
    demo_timer_counter = 1;
    timer_for_shine_timer = 10;
    samus_shine_timer = 120;
    samus_movement_handler = FUNC16(Samus_MoveHandler_F072);
  }
}

void Samus_MoveHandler_F072(void) {  // 0x90F072
  if (!samus_shine_timer) {
    frame_handler_alfa = FUNC16(Samus_FrameHandlerAlfa_Func11);
    samus_movement_handler = FUNC16(Samus_MovementHandler_Normal);
  }
}

static Func_U8 *const kSamusCodeHandlers[32] = {  // 0x90F084
  SamusCode_00_LockSamus,
  SamusCode_01_UnlockSamus,
  SamusCode_02_ReachCeresElevator,
  SamusCode_03,
  SamusCode_04,
  SamusCode_05_SetupDrained,
  SamusCode_06_LockToStation,
  SamusCode_07_SetupForElevator,
  SamusCode_08_SetupForCeresStart,
  SamusCode_08_SetupForZebesStart,
  SamusCode_0A_ClearDrawHandler,
  SamusCode_0B_DrawHandlerDefault,
  SamusCode_0C_UnlockFromMapStation,
  SamusCode_0D_IsGrappleActive,
  SamusCode_0E,
  SamusCode_0F_EnableTimerHandling,
  SamusCode_10,
  SamusCode_11_SetupForDeath,
  SamusCode_12_SetSuperPaletteFlag1,
  SamusCode_12_SetSuperPaletteFlag0,
  SamusCode_14,
  SamusCode_15_CalledBySuitAcquision,
  SamusCode_16,
  SamusCode_17_DisableRainbowSamusAndStandUp,
  SamusCode_18,
  SamusCode_17_FreezeDrainedSamus,
  SamusCode_1A,
  SamusCode_1B_CheckedLockSamus,
  SamusCode_1C,
  SamusCode_1D_ClearSoundInDoor,
  SamusCode_1E,
  SamusCode_1F,
};

uint16 CallSomeSamusCode(uint16 a) {
  uint16 code = kSamusCodeHandlers[a & 0x1F]();
  if (!(code & 1))
    return code >> 1;
  samus_new_pose = -1;
  samus_new_pose_interrupted = -1;
  samus_new_pose_transitional = -1;
  samus_momentum_routine_index = 0;
  samus_special_transgfx_index = 0;
  samus_hurt_switch_index = 0;
  return -1;
}

void Samus_UpdatePreviousPose(void) {  // 0x90F0EE
  samus_last_different_pose = samus_prev_pose;
  *(uint16 *)&samus_last_different_pose_x_dir = *(uint16 *)&samus_prev_pose_x_dir;
  samus_prev_pose = samus_pose;
  *(uint16 *)&samus_prev_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
}

uint8 ClearCarry(void) {  // 0x90F107
  return 0;
}

uint8 SamusCode_00_LockSamus(void) {  // 0x90F109
  frame_handler_alfa = FUNC16(Samus_FrameHandlerAlfa_Func13);
  frame_handler_beta = FUNC16(SetContactDamageIndexAndUpdateMinimap);
  return 1;
}

uint8 SamusCode_01_UnlockSamus(void) {  // 0x90F117
  frame_handler_alfa = FUNC16(Samus_FrameHandlerAlfa_Func11);
  frame_handler_beta = FUNC16(Samus_FrameHandlerBeta_Func17);
  return 1;
}

uint8 SamusCode_02_ReachCeresElevator(void) {  // 0x90F125
  if (samus_pose_x_dir == 4)
    samus_pose = kPose_02_FaceL_Normal;
  else
    samus_pose = kPose_01_FaceR_Normal;
  samus_anim_frame_skip = 0;
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
  reached_ceres_elevator_fade_timer = 60;
  return SamusCode_00_LockSamus();
}

uint8 SamusCode_03(void) {  // 0x90F152
  if (grapple_beam_function != FUNC16(GrappleBeamFunc_Inactive)) {
    grapple_beam_function = FUNC16(GrappleBeam_Func2);
    return 0;
  }
  if (samus_movement_type != kMovementType_03_SpinJumping && samus_movement_type != kMovementType_14_WallJumping)
    return 0;
  if (samus_pose_x_dir == kMovementType_04_MorphBallOnGround)
    samus_pose = kPose_02_FaceL_Normal;
  else
    samus_pose = kPose_01_FaceR_Normal;
  samus_anim_frame_skip = 0;
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
  Samus_UpdatePreviousPose();
  return 1;
}

uint8 SamusCode_04_06_Common(void) {  // 0x90F19E
  flare_counter = 0;
  ClearFlareAnimationState();
  Samus_LoadSuitPalette();
  return 0;
}

uint8 SamusCode_04(void) {  // 0x90F19B
  samus_charge_palette_index = 0;
  return SamusCode_04_06_Common();
}

uint8 SamusCode_06_LockToStation(void) {  // 0x90F1AA
  frame_handler_alfa = FUNC16(Samus_FrameHandlerAlfa_Func13);
  frame_handler_beta = FUNC16(j_HandleDemoRecorder_2);
  if (!sign16(flare_counter - 15))
    QueueSfx1_Max15(2u);
  return SamusCode_04_06_Common();
}

uint8 SamusCode_07_SetupForElevator(void) {  // 0x90F1C8
  MakeSamusFaceForward();
  frame_handler_beta = FUNC16(Samus_Func19);
  samus_movement_handler = FUNC16(Samus_MovementHandler_Normal);
  samus_draw_handler = FUNC16(SamusDisplayHandler_UsingElevator);
  samus_input_handler = FUNC16(Samus_InputHandler_E913);
  bomb_jump_dir = 0;
  return 1;
}

uint8 SamusCode_08_SetupForCeresStart(void) {  // 0x90F1E9
  uint16 v1;
  uint16 v2;

  frame_handler_alfa = FUNC16(EmptyFunction);
  frame_handler_beta = FUNC16(SetContactDamageIndexAndUpdateMinimap);
  samus_pose = kPose_00_FaceF_Powersuit;
  samus_anim_frame_skip = 0;
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
  Samus_LoadSuitPalette();
  samus_draw_handler = FUNC16(SamusDrawHandler_Default);
  samus_prev_pose = samus_pose;
  samus_last_different_pose = samus_pose;
  *(uint16 *)&samus_prev_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
  *(uint16 *)&samus_last_different_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
  samus_y_pos = 0;
  uint16 v0 = v1 = v2 = 0;
  SpawnEnemyProjectileWithGfx(0, 0, addr_kEproj_CeresElevatorPad);
  SpawnEnemyProjectileWithGfx(0, 0, addr_kEproj_CeresElevatorPlatform);
  debug_disable_minimap = 0;
  PlayRoomMusicTrackAfterAFrames(0x20);
  return 1;
}

uint8 SamusCode_08_SetupForZebesStart(void) {  // 0x90F23C
  if ((equipped_items & 0x20) != 0) {
    SpawnPalfxObject(addr_stru_8DE1FC);
    samus_pose = kPose_9B_FaceF_VariaGravitySuit;
  } else if ((equipped_items & 1) != 0) {
    SpawnPalfxObject(addr_stru_8DE1F8);
    samus_pose = kPose_9B_FaceF_VariaGravitySuit;
  } else {
    SpawnPalfxObject(addr_stru_8DE1F4);
    samus_pose = kPose_00_FaceF_Powersuit;
  }
  Samus_LoadSuitPalette();
  SamusFunc_F433();
  samus_anim_frame_timer = 3;
  samus_anim_frame = 2;
  substate = 0;
  return 1;
}

uint8 SamusCode_0A_ClearDrawHandler(void) {  // 0x90F28D
  samus_draw_handler = FUNC16(nullsub_152);
  return 0;
}

uint8 SamusCode_0B_DrawHandlerDefault(void) {  // 0x90F295
  samus_draw_handler = FUNC16(SamusDrawHandler_Default);
  return SamusCode_01_UnlockSamus();
}

uint8 SamusCode_0C_UnlockFromMapStation(void) {  // 0x90F29E
  SamusFunc_E633();
  if (frame_handler_beta == FUNC16(j_HandleDemoRecorder_2)) {
    frame_handler_alfa = FUNC16(Samus_FrameHandlerAlfa_Func11);
    frame_handler_beta = FUNC16(Samus_FrameHandlerBeta_Func17);
  }
  return 1;
}

uint8 SamusCode_0D_IsGrappleActive(void) {  // 0x90F2B8
  return (grapple_beam_function != FUNC16(GrappleBeamFunc_Inactive)) ? 2 : 0;
}

uint8 SamusCode_0D_IsGrappleActive_A(void) {
  return grapple_beam_function != FUNC16(GrappleBeamFunc_Inactive);
}


uint8 SamusCode_0E(void) {  // 0x90F2CA
  frame_handler_alfa = FUNC16(Samus_FrameHandlerAlfa_Func11);
  frame_handler_beta = FUNC16(Samus_Func18);
  return 1;
}

uint8 SamusCode_0F_EnableTimerHandling(void) {  // 0x90F2D8
  frame_handler_gamma = FUNC16(Samus_Func3);
  return 0;
}

uint8 SamusCode_10(void) {  // 0x90F2E0
  if (frame_handler_beta != FUNC16(j_HandleDemoRecorder_2_0)) {
    frame_handler_alfa = FUNC16(Samus_FrameHandlerAlfa_Func11);
    frame_handler_beta = FUNC16(Samus_FrameHandlerBeta_Func17);
  }
  return 1;
}

uint8 SamusCode_11_15_Common(void) {  // 0x90F2FC
  frame_handler_alfa = FUNC16(Samus_FrameHandlerAlfa_Func13);
  frame_handler_beta = FUNC16(EmptyFunction);
  samus_draw_handler = FUNC16(SamusDisplayHandler_SamusReceivedFatal);
  return 1;
}
uint8 SamusCode_11_SetupForDeath(void) {  // 0x90F2F8
  DisablePaletteFx();
  return SamusCode_11_15_Common();
}


uint8 SamusCode_15_CalledBySuitAcquision(void) {  // 0x90F310
  Samus_UpdatePreviousPose();
  flare_counter = 0;
  ClearFlareAnimationState();
  Samus_LoadSuitPalette();
  return SamusCode_11_15_Common();
}

uint8 SamusCode_12_SetSuperPaletteFlag1(void) {  // 0x90F320
  samus_special_super_palette_flags = 1;
  return 0;
}

uint8 SamusCode_12_SetSuperPaletteFlag0(void) {  // 0x90F328
  samus_special_super_palette_flags = 0;
  Samus_LoadSuitPalette();
  return 0;
}

uint8 SamusCode_14(void) {  // 0x90F331
  if (sign16(samus_health - 31))
    QueueSfx3_Max6(2u);
  if (!SamusCode_0D_IsGrappleActive_A()) {
    if (samus_pose_x_dir == 3) {
      if (samus_pose == kPose_81_FaceR_Screwattack || samus_pose == kPose_82_FaceL_Screwattack) {
        QueueSfx1_Max6(0x33u);
        return 0;
      } else if (samus_pose == kPose_1B_FaceR_SpaceJump || samus_pose == kPose_1C_FaceL_SpaceJump) {
        QueueSfx1_Max6(0x3Eu);
        return 0;
      } else {
        QueueSfx1_Max6(0x31u);
        return 0;
      }
    } else {
      return 0;
    }
  } else {
    QueueSfx1_Max6(6u);
    return 0;
  }
}

uint8 SamusCode_05_SetupDrained(void) {  // 0x90F38E
  frame_handler_gamma = FUNC16(Samus_Func1);
  return Samus_SetupForBeingDrained();
}

uint8 Samus_SetupForBeingDrained(void) {  // 0x90F394
  samus_pose = kPose_54_FaceL_Knockback;
  samus_anim_frame_skip = 0;
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
  Samus_UpdatePreviousPose();
  flare_counter = 0;
  ClearFlareAnimationState();
  Samus_LoadSuitPalette();
  frame_handler_alfa = FUNC16(Samus_FrameHandlerAlfa_Func13);
  frame_handler_beta = FUNC16(j_HandleDemoRecorder_2_0);
  return 1;
}

uint8 SamusCode_18(void) {  // 0x90F3C0
  frame_handler_gamma = FUNC16(Samus_Func2);
  return Samus_SetupForBeingDrained();
}

uint8 SamusCode_16(void) {  // 0x90F3C9
  samus_special_super_palette_flags = 0x8000;
  special_samus_palette_frame = 1;
  special_samus_palette_timer = 1;
  samus_charge_palette_index = 0;
  return 0;
}

uint8 SamusCode_17_DisableRainbowSamusAndStandUp(void) {  // 0x90F3DD
  samus_special_super_palette_flags = 0;
  special_samus_palette_frame = 0;
  special_samus_palette_timer = 0;
  samus_charge_palette_index = 0;
  Samus_LoadSuitPalette();
  samus_anim_frame_timer = 1;
  samus_anim_frame = 13;
  return 0;
}

uint8 SamusCode_17_FreezeDrainedSamus(void) {  // 0x90F3FB
  samus_anim_frame_timer = 1;
  samus_anim_frame = 28;
  return 1;
}

uint8 SamusCode_1A(void) {  // 0x90F409
  frame_handler_beta = FUNC16(Samus_LowHealthCheck);
  return 0;
}

uint8 SamusCode_1B_CheckedLockSamus(void) {  // 0x90F411
  if (frame_handler_beta == FUNC16(j_HandleDemoRecorder_2_0))
    return 1;
  else
    return SamusCode_00_LockSamus();
}

uint8 SamusCode_1C(void) {  // 0x90F41E
  if (samus_movement_type == kMovementType_14_WallJumping) {
    if (sign16(samus_anim_frame - 23)) {
      if (sign16(samus_anim_frame - 13)) {
LABEL_11:
        QueueSfx1_Max9(0x31u);
        return 0;
      }
      goto LABEL_12;
    }
  } else {
    if (samus_movement_type != kMovementType_03_SpinJumping)
      return 0;
    if (samus_pose != kPose_81_FaceR_Screwattack && samus_pose != kPose_82_FaceL_Screwattack) {
      if (samus_pose != kPose_1B_FaceR_SpaceJump && samus_pose != kPose_1C_FaceL_SpaceJump)
        goto LABEL_11;
LABEL_12:
      QueueSfx1_Max9(0x3Eu);
      return 0;
    }
  }
  QueueSfx1_Max9(0x33u);
  return 0;
}

uint8 SamusCode_1D_ClearSoundInDoor(void) {  // 0x90F471
  if (samus_movement_type == 3 || samus_movement_type == 20) {
    QueueSfx1_Max15(0x32u);
    return 0;
  } else {
    if ((button_config_shoot_x & joypad1_lastkeys) == 0) {
      if (sign16(flare_counter - 16))
        QueueSfx1_Max15(2u);
    }
    return 0;
  }
}

uint8 SamusCode_1E(void) {  // 0x90F4A2
  if (game_state == 8) {
    if (samus_movement_type == 3 || samus_movement_type == 20) {
      SamusCode_1C();
      return 0;
    }
    if (!sign16(flare_counter - 16))
      QueueSfx1_Max9(0x41u);
  }
  return 0;
}

uint8 SamusCode_1F(void) {  // 0x90F4D0
  if (grapple_beam_function != (uint16)addr_loc_90C4F0) {
    grapple_beam_unkD1E = 0;
    grapple_beam_unkD20 = 0;
    grapple_beam_direction = 0;
    grapple_beam_unkD36 = 0;
    grapple_walljump_timer = 0;
    slow_grabble_scrolling_flag = 0;
    grapple_varCF6 = 0;
    grapple_beam_flags = 0;
    LoadProjectilePalette(equipped_beams);
    grapple_beam_function = -15120;
    samus_draw_handler = FUNC16(SamusDrawHandler_Default);
  }
  return 0;
}

uint8 Samus_Func26(void) {  // 0x90F507
  if (samus_movement_type != kMovementType_03_SpinJumping
      && samus_movement_type != kMovementType_14_WallJumping
      && (button_config_shoot_x & joypad1_lastkeys) != 0
      && !sign16(flare_counter - 16)) {
    QueueSfx1_Max9(0x41u);
  }
  return 0;
}

void Samus_ShootCheck(void) {  // 0x90F576
  if ((play_resume_charging_beam_sfx & 0x8000u) != 0)
    goto LABEL_15;
  if (play_resume_charging_beam_sfx) {
    if ((button_config_shoot_x & joypad1_lastkeys) != 0)
      QueueSfx1_Max9(0x41u);
    play_resume_charging_beam_sfx = 0;
  }
  if (samus_echoes_sound_flag && (speed_boost_counter & 0x400) == 0) {
    samus_echoes_sound_flag = 0;
    QueueSfx3_Max15(0x25u);
  }
  if ((samus_prev_movement_type == 3 || samus_prev_movement_type == 20)
      && samus_movement_type != kMovementType_03_SpinJumping
      && samus_movement_type != kMovementType_14_WallJumping) {
    QueueSfx1_Max15(0x32u);
    if (!sign16(flare_counter - 16) && (button_config_shoot_x & joypad1_lastkeys) != 0)
      LABEL_15:
    play_resume_charging_beam_sfx = 1;
  }
  if (enable_debug) {
    if (samus_pose == kPose_00_FaceF_Powersuit || samus_pose == kPose_9B_FaceF_VariaGravitySuit) {
      if ((joypad2_last & 0x30) == 48 && (joypad2_new_keys & 0x80) != 0)
        debug_invincibility = 7;
    } else {
      if (!sign16(debug_invincibility - 7))
        return;
      debug_invincibility = 0;
    }
  }
  if (CheckEventHappened(0xEu) & 1
      && frame_handler_gamma == FUNC16(DrawTimer_)
      && game_state != kGameState_35_TimeUp) {
    game_state = kGameState_35_TimeUp;
  }
}