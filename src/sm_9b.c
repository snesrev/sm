#include "sm_cpu_infra.h"
#include "ida_types.h"
#include "variables.h"
#include "funcs.h"

#define g_off_9BA4B3 ((uint16*)RomFixedPtr(0x9ba4b3))
#define g_off_9BA4CB ((uint16*)RomFixedPtr(0x9ba4cb))
#define g_off_9BA4E3 ((uint16*)RomFixedPtr(0x9ba4e3))
#define g_off_9BB5C8 ((uint16*)RomFixedPtr(0x9bb5c8))
#define g_byte_9BB823 ((uint8*)RomFixedPtr(0x9bb823))
#define g_off_9BB6D2 ((uint16*)RomFixedPtr(0x9bb6d2))
#define kDeathSequencePals_PowerSuit ((uint16*)RomFixedPtr(0x9bb7d3))
#define kDeathSequencePals_VariaSuit ((uint16*)RomFixedPtr(0x9bb7e7))
#define kDeathSequencePals_GravitySuit ((uint16*)RomFixedPtr(0x9bb7fb))
#define kDeathSequencePals_Suitless ((uint16*)RomFixedPtr(0x9bb80f))
#define g_off_9BC3C6 ((uint16*)RomFixedPtr(0x9bc3c6))
#define g_off_9BC3EE ((uint16*)RomFixedPtr(0x9bc3ee))
#define g_off_9BC416 ((uint16*)RomFixedPtr(0x9bc416))
#define grapple_beam_special_angles ((GrappleBeamSpecialAngles*)RomFixedPtr(0x9bc43e))
#define kGrappleBeam_SwingingData ((uint8*)RomFixedPtr(0x9bc1c2))
#define kGrappleBeam_SwingingData2 ((uint8*)RomFixedPtr(0x9bc2c2))
#define kGrappleBeam_SwingingData3 ((uint8*)RomFixedPtr(0x9bc302))
#define kGrappleBeam_OriginX_NoRun ((uint16*)RomFixedPtr(0x9bc122))
#define kGrappleBeam_OriginY_NoRun ((uint16*)RomFixedPtr(0x9bc136))
#define kGrappleBeam_0x0d1a_offs_NoRun ((uint16*)RomFixedPtr(0x9bc14a))
#define kGrappleBeam_0x0d1c_offs_NoRun ((uint16*)RomFixedPtr(0x9bc15e))
#define kGrappleBeam_OriginX_Run ((uint16*)RomFixedPtr(0x9bc172))
#define kGrappleBeam_OriginY_Run ((uint16*)RomFixedPtr(0x9bc186))
#define kGrappleBeam_0x0d1a_offs_Run ((uint16*)RomFixedPtr(0x9bc19a))
#define kGrappleBeam_0x0d1c_offs_Run ((uint16*)RomFixedPtr(0x9bc1ae))
#define g_off_9BC344 (*(uint16*)RomFixedPtr(0x9bc344))
#define g_off_9BC342 (*(uint16*)RomFixedPtr(0x9bc342))
#define g_off_9BC346 ((uint16*)RomFixedPtr(0x9bc346))
#define kFlareAnimDelays ((uint16*)RomFixedPtr(0x90c481))
#define kFlareAnimDelays_Main ((uint8*)RomFixedPtr(0x90c487))
#define kFlareAnimDelays_SlowSparks ((uint8*)RomFixedPtr(0x90c4a7))
#define kFlareAnimDelays_FastSparks ((uint8*)RomFixedPtr(0x90c4ae))
#define g_word_93A22B ((uint16*)RomFixedPtr(0x93a22b))
#define g_word_93A225 ((uint16*)RomFixedPtr(0x93a225))
#define g_byte_9BC9BA ((uint8*)RomFixedPtr(0x9bc9ba))
#define g_byte_9BC9C4 ((uint8*)RomFixedPtr(0x9bc9c4))
#define kGrappleBeam_Ext_Xvel ((uint16*)RomFixedPtr(0x9bc0db))
#define kGrappleBeam_Ext_Yvel ((uint16*)RomFixedPtr(0x9bc0ef))
#define kGrappleBeam_Init_EndAngle ((uint16*)RomFixedPtr(0x9bc104))

static const uint8 kDeathAnimationFrames[28] = {
  5, 5, 5, 5, 1, 5, 5, 0, 1,
  0, 5, 5, 5, 5, 5, 5, 5, 1,
  1, 1, 5, 5, 5, 5, 5, 5, 5,
  5,
};

static const uint16 g_word_9BC118 = 24;
static const uint16 g_word_9BC11A = 0xc;
static const uint16 g_word_9BC11C = 5;
static const uint16 g_word_9BC11E = 0x480;
static const uint16 g_word_9BC120 = 0x300;

static const uint16 g_word_9BB7BF[5] = { 0x8400, 0x8800, 0x8c00, 0x9000, 0x8000 };
static const uint16 g_word_9BB7C9[5] = { 0x6200, 0x6400, 0x6600, 0x6800, 0x6000 };
static const uint16 kShadesOfWhite[22] = {
   0x421,  0xc63, 0x14a5, 0x1ce7, 0x2529, 0x2d6b, 0x35ad, 0x4210,
  0x4a52, 0x4e73, 0x5294, 0x56b5, 0x5ad6, 0x5ef7, 0x6318, 0x6739,
  0x6b5a, 0x6f7b, 0x739c, 0x77bd, 0x7bde, 0x7fff,
};
static const uint8 kIsGrappleBannedForMovementType[28] = {
  0, 0, 0, 1, 1, 0, 0, 1, 1, 1,
  1, 0, 0, 1, 1, 1, 0, 1, 1, 1,
  1, 0, 0, 1, 1, 1, 0, 1,
};

void ProjectileTrail_Func5(uint16 k, uint16 j) {  // 0x9BA3CC
  uint16 R22 = ProjectileInsts_GetValue(k);
  uint16 r18, r20;
  if ((ceres_status & 0x8000) == 0) {
    int v2 = k >> 1;
    r18 = projectile_x_pos[v2];
    r20 = projectile_y_pos[v2];
  } else {
    Point16U pt = CalcExplosion_Mode7(k);
    r18 = layer1_x_pos + pt.x;
    r20 = layer1_y_pos + pt.y;
  }
  int v3 = k >> 1;
  uint16 v4 = projectile_type[v3], v5;
  if ((v4 & 0x20) != 0) {
    v5 = g_off_9BA4E3[projectile_type[v3] & 0xF] + 2 * (projectile_dir[v3] & 0xF);
  } else if ((v4 & 0x10) != 0) {
    v5 = g_off_9BA4CB[projectile_type[v3] & 0xF] + 2 * (projectile_dir[v3] & 0xF);
  } else {
    v5 = g_off_9BA4B3[projectile_type[v3] & 0xF] + 2 * (projectile_dir[v3] & 0xF);
  }
  uint16 v6 = *(uint16 *)RomPtr_9B(v5) + 4 * R22;
  const uint8 *p = RomPtr_9B(v6);
  int v7 = j >> 1;
  projectiletrail_left_y_pos[v7] = r20 + (int8)p[1] - 4;
  projectiletrail_left_x_pos[v7] = r18 + (int8)p[0] - 4;
  projectiletrail_right_y_pos[v7] = r20 + (int8)p[3] - 4;
  projectiletrail_right_x_pos[v7] = r18 + (int8)p[2] - 4;
}

void StartSamusDeathAnimation(void) {  // 0x9BB3A7
  uint16 v0 = samus_movement_type;
  if (samus_movement_type == 3)
    QueueSfx1_Max6(0x32);
  uint16 v1 = kDeathAnimationFrames[v0];
  if (samus_pose_x_dir == 4)
    samus_pose = kPose_D8_FaceL_CrystalFlashEnd;
  else
    samus_pose = kPose_D7_FaceR_CrystalFlashEnd;
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
  samus_last_different_pose = samus_prev_pose;
  *(uint16 *)&samus_last_different_pose_x_dir = *(uint16 *)&samus_prev_pose_x_dir;
  samus_prev_pose = samus_pose;
  *(uint16 *)&samus_prev_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
  samus_anim_frame_skip = 0;
  samus_anim_frame = v1;
  samus_x_pos -= layer1_x_pos;
  samus_y_pos -= layer1_y_pos;
}

void DrawSamusStartingDeathAnim_(void) {  // 0x9BB43C
  Samus_DrawStartingDeathAnim();
}

uint16 HandleSamusDeathSequence(void) {  // 0x9BB441
  if (sign16(g_word_7E0DE6 - 4))
    QueueTransferOfSamusDeathSequence(2 * g_word_7E0DE6);
  if (sign16(++g_word_7E0DE6 - 60)) {
    bool v0 = (--game_options_screen_index & 0x8000) != 0;
    if (!game_options_screen_index || v0) {
      if (g_word_7E0DE4) {
        g_word_7E0DE4 = 0;
        game_options_screen_index = 3;
      } else {
        g_word_7E0DE4 = 1;
        game_options_screen_index = 1;
      }
      CopyPalettesForSamusDeath(g_word_7E0DE4 * 2);
    }
    return 0;
  } else {
    HandleSamusDeathSequence_Helper2();
    substate = (joypad2_last & 0xB0) == (kButton_A | kButton_L | kButton_R);
    return 1;
  }
}

void HandleSamusDeathSequence_Helper2(void) {  // 0x9BB4B6
  const uint16 *v0 = (const uint16 *)RomPtr_9B(g_off_9BB5C8[samus_suit_palette_index >> 1]);
  memcpy(&palette_buffer[192], RomPtr_9B(*v0), 32);
  memcpy(&palette_buffer[240], RomPtr_9B(addr_word_9BA120), 32);
  QueueTransferOfSamusDeathSequence(8);
  game_options_screen_index = g_byte_9BB823[0];
  g_word_7E0DE4 = 0;
  g_word_7E0DE6 = 0;
  GameState_24_SamusNoHealth_Explosion_2();
}

void CopyPalettesForSamusDeath(uint16 v0) {  // 0x9BB5CE
  int r20 = g_off_9BB6D2[samus_suit_palette_index >> 1];
  const uint16 *v1 = (const uint16 *)RomPtr_9B(r20 + v0);
  memcpy(&palette_buffer[192], RomPtr_9B(*v1), 32);
  memcpy(&palette_buffer[240], RomPtr_9B(kDeathSequencePals_Suitless[v0 >> 1]), 32);
}

void QueueTransferOfSamusDeathSequence(uint16 v0) {  // 0x9BB6D8
  uint16 v1 = vram_write_queue_tail;
  gVramWriteEntry(vram_write_queue_tail)->size = 1024;
  v1 += 2;
  int v2 = v0 >> 1;
  gVramWriteEntry(v1)->size = g_word_9BB7BF[v2];
  v1 += 2;
  LOBYTE(gVramWriteEntry(v1++)->size) = -101;
  gVramWriteEntry(v1)->size = g_word_9BB7C9[v2];
  vram_write_queue_tail = v1 + 2;
}

uint16 GameState_24_SamusNoHealth_Explosion_Helper(void) {  // 0x9BB701
  GameState_24_SamusNoHealth_Explosion_1();
  return GameState_24_SamusNoHealth_Explosion_2();
}

void GameState_24_SamusNoHealth_Explosion_1(void) {  // 0x9BB710
  if (!substate && g_word_7E0DE4) {
    int v0 = g_word_7E0DE6;
    uint16 *dst = (uint16*)(g_ram + 0xc000);
    for(int i = 0; i < 384/2; i++)
      dst[i] = kShadesOfWhite[v0];
    for(int i = 416/2; i < 480/2; i++)
      dst[i] = kShadesOfWhite[v0];
    if (sign16(g_word_7E0DE6 - 20))
      ++g_word_7E0DE6;
  }
}

uint16 GameState_24_SamusNoHealth_Explosion_2(void) {  // 0x9BB758
  bool v0 = (--game_options_screen_index & 0x8000) != 0;
  if (!game_options_screen_index || v0) {
    if (!sign16(++g_word_7E0DE4 - 9)) {
      g_word_7E0DE6 = 21;
      GameState_24_SamusNoHealth_Explosion_1();
      substate = 0;
      return 1;
    }
    if (!substate || sign16(g_word_7E0DE4 - 2)) {
      game_options_screen_index = g_byte_9BB823[(2 * g_word_7E0DE4)];
      CopyPalettesForSamusDeath(2 * g_byte_9BB823[(2 * g_word_7E0DE4) + 1]);
    } else {
      game_options_screen_index = g_byte_9BB823[(2 * g_word_7E0DE4)];
    }
  }
  DrawSamusSuitExploding();
  return 0;
}

void CancelGrappleBeamIfIncompatiblePose(void) {  // 0x9BB861
  int16 v0;

  if (kIsGrappleBannedForMovementType[samus_movement_type]) {
LABEL_2:
    if (grapple_beam_function != FUNC16(GrappleBeamFunc_Inactive))
      grapple_beam_function = FUNC16(GrappleBeamFunc_Cancel);
    return;
  }
  if (grapple_beam_function != FUNC16(GrappleBeamFunc_Inactive)
      && sign16(grapple_beam_function + 0x3882)) {
    v0 = kPoseParams[samus_pose].direction_shots_fired;
    if ((v0 & 0xF0) == 0) {
      if (v0 == grapple_beam_direction)
        return;
      if (grapple_varCF6) {
        QueueSfx1_Max6(7);
        grapple_beam_function = FUNC16(GrappleBeamFunc_FireGoToCancel);
        return;
      }
    }
    goto LABEL_2;
  }
}

uint8 CheckIfGrappleIsConnectedToBlock(void) {  // 0x9BB8F1
  grapple_beam_extension_x_velocity = 0;
  grapple_beam_extension_y_velocity = 0;
  if ((BlockReactGrapple() & 1) == 0)
    return 0;
  samus_grapple_flags = 1;
  return 1;
}

static int ProcessEnemyGrappleBeamColl(uint16 a, uint16 r18) {  // 0x9BB907
  switch (a) {
  case 0:
  case 2:
    return -1;  // clc
  case 3:
    return 1;
  case 1:
  case 4:
  case 5:
    return 0;
  case 6:
    r18 = *((uint16 *)RomPtr_A0(r18) + 3);
    if ((equipped_items & 0x20) != 0) {
      r18 >>= 2;
    } else if (equipped_items & 1) {
      r18 >>= 1;
    }
    Samus_DealDamage(r18);
    samus_invincibility_timer = 96;
    samus_knockback_timer = 5;
    knockback_x_dir = samus_pose_x_dir == 4;
    return 1;
  default:
    Unreachable();
    return 0;
  }
}

void CallGrappleNextFunc(uint32 ea) {
  switch (ea) {
  case fnGrappleNext_SwingClockwise: GrappleNext_SwingClockwise(); return;
  case fnGrappleNext_SwingAntiClockwise: GrappleNext_SwingAntiClockwise(); return;
  case fnGrappleNext_StandAimRight: GrappleNext_StandAimRight(); return;
  case fnGrappleNext_StandAimDownRight: GrappleNext_StandAimDownRight(); return;
  case fnGrappleNext_StandAimDownLeft: GrappleNext_StandAimDownLeft(); return;
  case fnGrappleNext_StandAimLeft: GrappleNext_StandAimLeft(); return;
  case fnGrappleNext_CrouchAimRight: GrappleNext_CrouchAimRight(); return;
  case fnGrappleNext_CrouchAimDownRight: GrappleNext_CrouchAimDownRight(); return;
  case fnGrappleNext_CrouchAimDownLeft: GrappleNext_CrouchAimDownLeft(); return;
  case fnGrappleNext_CrouchAimLeft: GrappleNext_CrouchAimLeft(); return;
  default: Unreachable();
  }
}

void HandleConnectingGrapple(void) {  // 0x9BB97C
  if (samus_movement_type == kMovementType_1A_GrabbedByDraygon) {
    grapple_beam_function = FUNC16(GrappleBeamFunc_ConnectedLockedInPlace);
    grapple_beam_length_delta = 0;
  } else {
    int v1 = 2 * grapple_beam_direction;
    if (samus_y_speed || samus_y_subspeed) {
      grapple_beam_function = g_off_9BC3EE[v1];
      CallGrappleNextFunc(g_off_9BC3EE[v1 + 1] | 0x9B0000);
    } else if (samus_movement_type == 5) {
      grapple_beam_function = g_off_9BC416[v1];
      CallGrappleNextFunc(g_off_9BC416[v1 + 1] | 0x9B0000);
    } else {
      grapple_beam_function = g_off_9BC3C6[v1];
      CallGrappleNextFunc(g_off_9BC3C6[v1 + 1] | 0x9B0000);
    }
  }
}

void GrappleNext_SwingClockwise(void) {  // 0x9BB9D9
  samus_new_pose_interrupted = kPose_B2_FaceR_Grapple_Air;
  HandleConnectingGrapple_Swinging();
}

void GrappleNext_SwingAntiClockwise(void) {  // 0x9BB9E2
  samus_new_pose_interrupted = kPose_B3_FaceL_Grapple_Air;
  HandleConnectingGrapple_Swinging();
}

void GrappleNext_StandAimRight(void) {  // 0x9BB9EA
  samus_new_pose_interrupted = kPose_A8_FaceR_Grappling;
  HandleConnectingGrapple_StuckInPlace();
}

void GrappleNext_StandAimDownRight(void) {  // 0x9BB9F3
  samus_new_pose_interrupted = kPose_AA_FaceR_Grappling_AimDR;
  HandleConnectingGrapple_StuckInPlace();
}

void GrappleNext_StandAimDownLeft(void) {  // 0x9BB9FC
  samus_new_pose_interrupted = kPose_AB_FaceL_Grappling_AimDL;
  HandleConnectingGrapple_StuckInPlace();
}

void GrappleNext_StandAimLeft(void) {  // 0x9BBA05
  samus_new_pose_interrupted = kPose_A9_FaceL_Grappling;
  HandleConnectingGrapple_StuckInPlace();
}

void GrappleNext_CrouchAimRight(void) {  // 0x9BBA0E
  samus_new_pose_interrupted = kPose_B4_FaceR_Grappling_Crouch;
  HandleConnectingGrapple_StuckInPlace();
}

void GrappleNext_CrouchAimDownRight(void) {  // 0x9BBA17
  samus_new_pose_interrupted = kPose_B6_FaceR_Grappling_Crouch_AimDR;
  HandleConnectingGrapple_StuckInPlace();
}

void GrappleNext_CrouchAimDownLeft(void) {  // 0x9BBA20
  samus_new_pose_interrupted = kPose_B7_FaceL_Grappling_Crouch_AimDL;
  HandleConnectingGrapple_StuckInPlace();
}

void GrappleNext_CrouchAimLeft(void) {  // 0x9BBA29
  samus_new_pose_interrupted = kPose_B5_FaceL_Grappling_Crouch;
  HandleConnectingGrapple_StuckInPlace();
}

void HandleConnectingGrapple_Swinging(void) {  // 0x9BBA61
  samus_special_transgfx_index = 9;
  uint16 v0 = swap16(CalculateAngleFromXY(samus_x_pos - grapple_beam_end_x_pos, samus_y_pos - grapple_beam_end_y_pos));
  grapple_beam_end_angle16 = v0;
  grapple_beam_end_angles_mirror = v0;
  grapple_beam_length_delta = 0;
  if (!sign16(grapple_beam_length - 64))
    grapple_beam_length -= 24;
  BlockFunc_AC11();
}

void HandleConnectingGrapple_StuckInPlace(void) {  // 0x9BBA9B
  samus_special_transgfx_index = 10;
  uint16 v0 = swap16(CalculateAngleFromXY(samus_x_pos - grapple_beam_end_x_pos, samus_y_pos - grapple_beam_end_y_pos));
  grapple_beam_end_angle16 = v0;
  grapple_beam_end_angles_mirror = v0;
  grapple_beam_length_delta = 0;
  if (!sign16(grapple_beam_length - 64))
    grapple_beam_length -= 24;
  BlockFunc_AC11();
}

uint8 HandleSpecialGrappleBeamAngles(void) {  // 0x9BBAD5
  uint16 v0 = 7;
  while (grapple_beam_end_angle16 != grapple_beam_special_angles[v0].field_0) {
    if ((--v0 & 0x8000) != 0)
      return 0;
  }
  samus_new_pose_interrupted = grapple_beam_special_angles[v0].field_2;
  samus_x_pos = grapple_beam_end_x_pos + grapple_beam_special_angles[v0].field_4;
  samus_y_pos = grapple_beam_end_y_pos + grapple_beam_special_angles[v0].field_6;
  grapple_beam_function = grapple_beam_special_angles[v0].field_8;
  samus_special_transgfx_index = 0;
  slow_grabble_scrolling_flag = 0;
  int16 v2 = samus_x_pos - samus_prev_x_pos;
  if ((int16)(samus_x_pos - samus_prev_x_pos) < 0) {
    if (sign16(v2 + 12))
      samus_prev_x_pos = samus_x_pos + 12;
  } else if (!sign16(v2 - 13)) {
    samus_prev_x_pos = samus_x_pos - 12;
  }
  int16 v3 = samus_y_pos - samus_prev_y_pos;
  if ((int16)(samus_y_pos - samus_prev_y_pos) < 0) {
    if (sign16(v3 + 12))
      samus_prev_y_pos = samus_y_pos + 12;
  } else if (!sign16(v3 - 13)) {
    samus_prev_y_pos = samus_y_pos - 12;
  }
  return 1;
}

void GrappleBeamFunc_BB64(void) {  // 0x9BBB64
  if ((joypad1_newkeys & kButton_Up) != 0) {
    if (grapple_beam_length)
      grapple_beam_length_delta = -2;
  } else if ((joypad1_newkeys & kButton_Down) != 0) {
    if (sign16(grapple_beam_length - 64))
      grapple_beam_length_delta = 2;
    else
      grapple_beam_length = 64;
  }
  if (sign16((grapple_beam_end_angle_hi << 8) - 0x4000) || !sign16((grapple_beam_end_angle_hi << 8) + 0x4000))
    goto LABEL_13;
  if ((joypad1_lastkeys & 0x200) != 0) {
    if (grapple_beam_end_angle_hi << 8 == 0x8000 && !grapple_beam_unkD26)
      grapple_beam_unkD26 = 256;
    if (grapple_beam_flags && (grapple_beam_flags & 1) != 0)
      grapple_beam_unkD2A = g_word_9BC11A >> 1;
    else
      grapple_beam_unkD2A = g_word_9BC11A;
  } else {
    if ((joypad1_lastkeys & 0x100) == 0) {
LABEL_13:
      grapple_beam_unkD2A = 0;
      return;
    }
    if (grapple_beam_end_angle_hi << 8 == 0x8000 && !grapple_beam_unkD26)
      grapple_beam_unkD26 = -256;
    if (grapple_beam_flags && (grapple_beam_flags & 1) != 0)
      grapple_beam_unkD2A = -(g_word_9BC11A >> 1);
    else
      grapple_beam_unkD2A = -g_word_9BC11A;
  }
}

void GrappleBeamFunc_BC1F(void) {  // 0x9BBC1F
  if ((grapple_beam_end_angle16 & 0xC000) == 0xC000) {
    grapple_beam_unkD2C = -(g_word_9BC11C >> 2);
    if (grapple_beam_flags && (grapple_beam_flags & 1) != 0)
      grapple_beam_unkD28 = -(g_word_9BC118 >> 3);
    else
      grapple_beam_unkD28 = -(g_word_9BC118 >> 2);
  } else if (sign16(grapple_beam_end_angle16)) {
    if (grapple_beam_end_angle_hi << 8 == 0x8000) {
      grapple_beam_unkD28 = 0;
      grapple_beam_unkD2C = 0;
      uint16 v0 = grapple_beam_unkD26;
      if ((grapple_beam_unkD26 & 0x8000) != 0)
        v0 = ~(grapple_beam_unkD26 - 1);
      if (sign16(HIBYTE(v0) - 1))
        grapple_beam_unkD26 = 0;
    } else {
      grapple_beam_unkD2C = -g_word_9BC11C;
      if (grapple_beam_flags && (grapple_beam_flags & 1) != 0)
        grapple_beam_unkD28 = -(g_word_9BC118 >> 1);
      else
        grapple_beam_unkD28 = -g_word_9BC118;
    }
  } else if ((grapple_beam_end_angle16 & 0x4000) != 0) {
    grapple_beam_unkD2C = g_word_9BC11C;
    if (grapple_beam_flags && (grapple_beam_flags & 1) != 0)
      grapple_beam_unkD28 = g_word_9BC118 >> 1;
    else
      grapple_beam_unkD28 = g_word_9BC118;
  } else {
    grapple_beam_unkD2C = g_word_9BC11C >> 2;
    if (grapple_beam_flags && (grapple_beam_flags & 1) != 0)
      grapple_beam_unkD28 = g_word_9BC118 >> 3;
    else
      grapple_beam_unkD28 = g_word_9BC118 >> 2;
  }
}

void GrappleBeamFunc_BCFF(void) {  // 0x9BBCFF
  grapple_beam_unkD26 += grapple_beam_unkD2A + grapple_beam_unkD28;
  if (((grapple_beam_unkD26 ^ grapple_beam_end_angle16) & 0x8000) != 0)
    grapple_beam_unkD26 += grapple_beam_unkD2C;
  if ((grapple_beam_unkD26 & 0x8000) == 0) {
    if (grapple_beam_unkD26 >= g_word_9BC11E)
      grapple_beam_unkD26 = g_word_9BC11E;
  } else if ((uint16)-grapple_beam_unkD26 >= g_word_9BC11E) {
    grapple_beam_unkD26 = -g_word_9BC11E;
  }
}

void GrappleBeamFunc_BD44(void) {  // 0x9BBD44
  if (grapple_beam_unkD30 && (button_config_jump_a & joypad1_newkeys) != 0) {
    if (grapple_beam_unkD26) {
      if ((grapple_beam_unkD26 & 0x8000) != 0) {
        if (grapple_beam_flags && (grapple_beam_flags & 1) != 0)
          grapple_beam_unkD2E = -(g_word_9BC120 >> 1);
        else
          grapple_beam_unkD2E = -g_word_9BC120;
      } else if (grapple_beam_flags && (grapple_beam_flags & 1) != 0) {
        grapple_beam_unkD2E = g_word_9BC120 >> 1;
      } else {
        grapple_beam_unkD2E = g_word_9BC120;
      }
    } else {
      grapple_beam_unkD2E = 0;
    }
  }
}

void GrappleBeamFunc_BD95(void) {  // 0x9BBD95
  uint16 v0 = abs16(grapple_beam_unkD26);
  uint16 v1;

  if (!sign16(v0 - 64)) {
    slow_grabble_scrolling_flag = 1;
LABEL_7:
    samus_anim_frame_timer = 15;
    v1 = kGrappleBeam_SwingingData[HIBYTE(grapple_beam_end_angles_mirror)];
    samus_anim_frame = v1;
    goto LABEL_8;
  }
  slow_grabble_scrolling_flag = 0;
  if (grapple_beam_end_angle_hi << 8 != 0x8000)
    goto LABEL_7;
  if (sign16(samus_anim_frame - 64)) {
    samus_anim_frame_timer = 8;
    samus_anim_frame = 64;
  }
  v1 = kGrappleBeam_SwingingData[HIBYTE(grapple_beam_end_angles_mirror)];
LABEL_8:;
  uint16 v2 = 2 * v1;
  if ((abs16(grapple_beam_unkD2E) & 0xFF00) == 256) {
    uint16 v3 = samus_anim_frame;
    if (!sign16(samus_anim_frame - 64))
      v3 = 16;
    samus_anim_frame = v3 + 32;
  }
  if (samus_pose_x_dir == 4) {
    samus_x_pos = x_pos_of_start_of_grapple_beam + (int8)kGrappleBeam_SwingingData2[v2];
    samus_y_pos = y_pos_of_start_of_grapple_beam + (int8)kGrappleBeam_SwingingData2[v2 + 1];
  } else {
    samus_x_pos = x_pos_of_start_of_grapple_beam + (int8)kGrappleBeam_SwingingData3[v2];
    samus_y_pos = y_pos_of_start_of_grapple_beam + (int8)kGrappleBeam_SwingingData3[v2 + 1];
  }
  x_pos_of_start_of_grapple_beam_prevframe = x_pos_of_start_of_grapple_beam;
  y_pos_of_start_of_grapple_beam_prevframe = y_pos_of_start_of_grapple_beam;
  GrappleBeamFunc_BE98();
}

void GrappleBeamFunc_BE98(void) {  // 0x9BBE98
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

void GrappleBeamFunc_BEEB(void) {  // 0x9BBEEB
  int v0 = grapple_beam_direction;
  samus_x_pos = x_pos_of_start_of_grapple_beam - kGrappleBeam_OriginX_NoRun[v0];
  x_pos_of_start_of_grapple_beam_prevframe = kGrappleBeam_0x0d1a_offs_NoRun[v0] + samus_x_pos;
  samus_y_pos = y_pos_of_start_of_grapple_beam - kGrappleBeam_OriginY_NoRun[v0];
  y_pos_of_start_of_grapple_beam_prevframe = kGrappleBeam_0x0d1c_offs_NoRun[v0] + samus_y_pos;
}

void GrappleBeamFunc_BF1B(void) {  // 0x9BBF1B
  uint16 r22 = kPoseParams[samus_pose].y_offset_to_gfx;
  uint16 v0 = 2 * grapple_beam_direction;
  if (samus_pose == kPose_49_FaceL_Moonwalk || samus_pose == kPose_4A_FaceR_Moonwalk || samus_movement_type != 1) {
    int v1 = v0 >> 1;
    x_pos_of_start_of_grapple_beam = kGrappleBeam_OriginX_NoRun[v1] + samus_x_pos;
    x_pos_of_start_of_grapple_beam_prevframe = kGrappleBeam_0x0d1a_offs_NoRun[v1] + samus_x_pos;
    y_pos_of_start_of_grapple_beam = kGrappleBeam_OriginY_NoRun[v1] + samus_y_pos - r22;
    y_pos_of_start_of_grapple_beam_prevframe = kGrappleBeam_0x0d1c_offs_NoRun[v1] + samus_y_pos - r22;
  } else {
    int v2 = v0 >> 1;
    x_pos_of_start_of_grapple_beam = kGrappleBeam_OriginX_Run[v2] + samus_x_pos;
    x_pos_of_start_of_grapple_beam_prevframe = kGrappleBeam_0x0d1a_offs_Run[v2] + samus_x_pos;
    y_pos_of_start_of_grapple_beam = kGrappleBeam_OriginY_Run[v2] + samus_y_pos - r22;
    y_pos_of_start_of_grapple_beam_prevframe = kGrappleBeam_0x0d1c_offs_Run[v2] + samus_y_pos - r22;
  }
}

void UpdateGrappleBeamTilesAndIncrFlameCtr(void) {  // 0x9BBFA5
  UpdateGrappleBeamTiles(); 
  if (sign16(flare_counter - 120))
    ++flare_counter;
}

void UpdateGrappleBeamTiles(void) {  // 0x9BBFBD
  if ((--grapple_point_anim_timer & 0x8000) != 0) {
    grapple_point_anim_timer = 5;
    grapple_point_anim_ptr += 512;
    if ((int16)(grapple_point_anim_ptr - g_off_9BC344) >= 0)
      grapple_point_anim_ptr = g_off_9BC342;
  }
  uint16 v0 = vram_write_queue_tail;
  gVramWriteEntry(vram_write_queue_tail)->size = 32;
  v0 += 2;
  gVramWriteEntry(v0)->size = grapple_point_anim_ptr;
  v0 += 2;
  LOBYTE(gVramWriteEntry(v0++)->size) = -102;
  gVramWriteEntry(v0)->size = 25088;
  vram_write_queue_tail = v0 + 2;
  uint16 v1 = (grapple_beam_end_angle_hi >> 1) & 0xFE;
  v0 += 2;
  gVramWriteEntry(v0)->size = 128;
  v0 += 2;
  gVramWriteEntry(v0)->size = g_off_9BC346[v1 >> 1];
  v0 += 2;
  LOBYTE(gVramWriteEntry(v0++)->size) = -102;
  gVramWriteEntry(v0)->size = 25104;
  vram_write_queue_tail = v0 + 2;
}

void HandleGrappleBeamFlare(void) {  // 0x9BC036
  if (flare_counter) {
    if (flare_counter == 1) {
      flare_animation_frame = 16;
      flare_animation_timer = 3;
    }
    if ((--flare_animation_timer & 0x8000) != 0) {
      uint16 v0 = ++flare_animation_frame;
      if (kFlareAnimDelays_Main[flare_animation_frame] == 254) {
        flare_animation_frame -= kFlareAnimDelays_Main[(uint16)(flare_animation_frame + 1)];
        v0 = flare_animation_frame;
      }
      flare_animation_timer = kFlareAnimDelays_Main[v0];
    }
    uint16 r22;
    if (samus_pose_x_dir == 4)
      r22 = flare_animation_frame + g_word_93A22B[0];
    else
      r22 = flare_animation_frame + g_word_93A225[0];
    uint16 r20 = x_pos_of_start_of_grapple_beam_prevframe - layer1_x_pos;
    uint16 r18 = y_pos_of_start_of_grapple_beam_prevframe - layer1_y_pos;
    if (((y_pos_of_start_of_grapple_beam_prevframe - layer1_y_pos) & 0xFF00) != 0)
      ;
    else
      DrawBeamGrappleSpritemap(r22, r20, r18);
  }
}

void CallGrappleBeamFunc(uint32 ea) {
  switch (ea) {
  case fnGrappleBeamFunc_BB64: GrappleBeamFunc_BB64(); return;
  case fnGrappleBeamFunc_BC1F: GrappleBeamFunc_BC1F(); return;
  case fnGrappleBeamFunc_BCFF: GrappleBeamFunc_BCFF(); return;
  case fnGrappleBeamFunc_BD44: GrappleBeamFunc_BD44(); return;
  case fnGrappleBeamFunc_BD95: GrappleBeamFunc_BD95(); return;
  case fnGrappleBeamFunc_BE98: GrappleBeamFunc_BE98(); return;
  case fnGrappleBeamFunc_BEEB: GrappleBeamFunc_BEEB(); return;
  case fnGrappleBeamFunc_BF1B: GrappleBeamFunc_BF1B(); return;
  case fnGrappleBeamFunc_Inactive: GrappleBeamFunc_Inactive(); return;
  case fnGrappleBeamFunc_FireGoToCancel: GrappleBeamFunc_FireGoToCancel(); return;
  case fnGrappleBeamFunc_Firing: GrappleBeamFunc_Firing(); return;
  case fnGrappleBeamFunc_ConnectedLockedInPlace: GrappleBeamFunc_ConnectedLockedInPlace(); return;
  case fnGrappleBeamFunc_Connected_Swinging: GrappleBeamFunc_Connected_Swinging(); return;
  case fnGrappleBeamFunc_Wallgrab: GrappleBeamFunc_Wallgrab(); return;
  case fnGrappleBeamFunc_C832: GrappleBeamFunc_C832(); return;
  case fnGrappleBeamFunc_Cancel: GrappleBeamFunc_Cancel(); return;
  case fnGrappleBeamFunc_C9CE: GrappleBeamFunc_C9CE(); return;
  case fnGrappleBeamFunc_ReleaseFromSwing: GrappleBeamFunc_ReleaseFromSwing(); return;
  case fnGrappleBeam_Func2: GrappleBeam_Func2(); return;
  default: Unreachable();
  }
}

void GrappleBeamHandler(void) {  // 0x9BC490
  uint16 r18;
  if (grapple_varCF6)
    --grapple_varCF6;
  samus_grapple_flags &= ~1;
  CancelGrappleBeamIfIncompatiblePose();
  CallGrappleBeamFunc(grapple_beam_function | 0x9B0000);
  if (grapple_beam_function != FUNC16(GrappleBeamFunc_Inactive)
      && sign16(grapple_beam_function - FUNC16(GrappleBeamFunc_Cancel))
      && (samus_suit_palette_index & 4) == 0
      && fx_type
      && (r18 = Samus_GetBottom_R18(), (fx_y_pos & 0x8000) == 0)
      && sign16(fx_y_pos - r18)) {
    grapple_beam_flags |= 1;
  } else {
    grapple_beam_flags &= ~1;
  }
}

void GrappleBeamFunc_Inactive(void) {  // 0x9BC4F0
  if ((button_config_shoot_x & joypad1_newkeys) != 0 || (button_config_shoot_x & joypad1_newinput_samusfilter) != 0) {
    GrappleBeamFunc_FireGoToCancel();
  } else if (flare_counter) {
    flare_counter = 0;
    flare_animation_frame = 0;
    flare_slow_sparks_anim_frame = 0;
    flare_fast_sparks_anim_frame = 0;
    flare_animation_timer = 0;
    flare_slow_sparks_anim_timer = 0;
    flare_fast_sparks_anim_timer = 0;
    Samus_LoadSuitPalette();
  }
}

void GrappleBeamFunc_FireGoToCancel(void) {  // 0x9BC51E
  uint16 r22;

  int v0;
  if (samus_pose == kPose_F0_FaceR_Draygon_Move || samus_pose == kPose_BE_FaceL_Draygon_Move) {
    v0 = CheckBannedDraygonGrappleDirs(samus_pose);
    r22 = 6;
  } else {
    r22 = kPoseParams[samus_pose].y_offset_to_gfx;
    v0 = kPoseParams[samus_pose].direction_shots_fired;
    if ((v0 & 0xF0) != 0) {
      grapple_beam_function = FUNC16(GrappleBeamFunc_Cancel);
      return;
    }
  }
  grapple_beam_direction = v0;
  int v1 = v0;
  grapple_beam_extension_x_velocity = kGrappleBeam_Ext_Xvel[v1];
  grapple_beam_extension_y_velocity = kGrappleBeam_Ext_Yvel[v1];
  grapple_beam_end_angle16 = kGrappleBeam_Init_EndAngle[v1];
  grapple_beam_end_angles_mirror = grapple_beam_end_angle16;
  grapple_varCF6 = 10;
  if (samus_pose == kPose_49_FaceL_Moonwalk || samus_pose == kPose_4A_FaceR_Moonwalk || samus_movement_type != 1) {
    grapple_beam_origin_x_offset = kGrappleBeam_OriginX_NoRun[v1];
    grapple_beam_origin_y_offset = kGrappleBeam_OriginY_NoRun[v1] - r22;
    grapple_beam_end_x_pos = kGrappleBeam_OriginX_NoRun[v1] + samus_x_pos;
    x_pos_of_start_of_grapple_beam = grapple_beam_end_x_pos;
    x_pos_of_start_of_grapple_beam_prevframe = kGrappleBeam_0x0d1a_offs_NoRun[v1] + samus_x_pos;
    grapple_beam_end_y_pos = kGrappleBeam_OriginY_NoRun[v1] + samus_y_pos - r22;
    y_pos_of_start_of_grapple_beam = grapple_beam_end_y_pos;
    y_pos_of_start_of_grapple_beam_prevframe = kGrappleBeam_0x0d1c_offs_NoRun[v1] + samus_y_pos - r22;
  } else {
    grapple_beam_origin_x_offset = kGrappleBeam_OriginX_Run[v1];
    grapple_beam_origin_y_offset = kGrappleBeam_OriginY_Run[v1] - r22;
    grapple_beam_end_x_pos = kGrappleBeam_OriginX_Run[v1] + samus_x_pos;
    x_pos_of_start_of_grapple_beam = grapple_beam_end_x_pos;
    x_pos_of_start_of_grapple_beam_prevframe = kGrappleBeam_0x0d1a_offs_Run[v1] + samus_x_pos;
    grapple_beam_end_y_pos = kGrappleBeam_OriginY_Run[v1] + samus_y_pos - r22;
    y_pos_of_start_of_grapple_beam = grapple_beam_end_y_pos;
    y_pos_of_start_of_grapple_beam_prevframe = kGrappleBeam_0x0d1c_offs_Run[v1] + samus_y_pos - r22;
  }
  grapple_beam_end_x_subpos = 0;
  grapple_beam_end_y_subpos = 0;
  grapple_beam_end_x_suboffset = 0;
  grapple_beam_end_x_offset = 0;
  grapple_beam_end_y_suboffset = 0;
  grapple_beam_end_y_offset = 0;
  grapple_beam_flags = 0;
  grapple_beam_length_delta = 12;
  grapple_beam_length = 0;
  grapple_beam_unkD26 = 0;
  grapple_beam_unkD28 = 0;
  grapple_beam_unkD2A = 0;
  grapple_beam_unkD2C = 0;
  grapple_beam_unkD2E = 0;
  grapple_beam_unkD30 = 0;
  grapple_beam_unkD1E = 0;
  grapple_beam_unkD20 = 0;
  grapple_beam_unkD3A = 2;
  grapple_beam_unkD3C = 0;
  grapple_point_anim_timer = 5;
  grapple_point_anim_ptr = g_off_9BC342;
  grapple_beam_grapple_start_x = 0;
  grapple_beam_unkD38 = 0;
  grapple_beam_unkD36 = 0;
  slow_grabble_scrolling_flag = 0;
  GrappleFunc_AF87();
  samus_draw_handler = FUNC16(sub_90EB86);
  grapple_walljump_timer = 0;
  LoadProjectilePalette(2);
  palette_buffer[223] = 32657;
  grapple_beam_function = FUNC16(GrappleBeamFunc_Firing);
  QueueSfx1_Max1(5);
  flare_counter = 1;
  play_resume_charging_beam_sfx = 0;
  if (samus_movement_handler == FUNC16(Samus_MoveHandler_ReleaseFromGrapple))
    samus_movement_handler = FUNC16(Samus_MovementHandler_Normal);
}

uint16 CheckBannedDraygonGrappleDirs(uint16 a) {  // 0x9BC6B2
  if (a == kPose_BE_FaceL_Draygon_Move) {
    if ((joypad1_lastkeys & kButton_Left) != 0) {
      if ((joypad1_lastkeys & kButton_Down) != 0)
        return 6;
      if ((joypad1_lastkeys & kButton_Up) != 0)
        return 8;
    }
    return 7;
  }
  if ((joypad1_lastkeys & kButton_Right) == 0)
    return 2;
  if ((joypad1_lastkeys & kButton_Down) != 0)
    return 3;
  if ((joypad1_lastkeys & kButton_Up) == 0)
    return 2;
  return 1;
}

uint8 ClearCarry_12(void) {  // 0x9BC701
  return 0;
}

void GrappleBeamFunc_Firing(void) {  // 0x9BC703
  if ((button_config_shoot_x & joypad1_lastkeys) == 0) {
    grapple_beam_function = FUNC16(GrappleBeamFunc_Cancel);
    return;
  }
  grapple_beam_length += grapple_beam_length_delta;
  if (!sign16(grapple_beam_length - 128)) {
    grapple_beam_function = FUNC16(GrappleBeamFunc_Cancel);
    return;
  }
  PairU16 pair = GrappleBeam_CollDetect_Enemy();
  int v1 = ProcessEnemyGrappleBeamColl(pair.k, pair.j);
  if (v1 >= 0) {
    if (v1) {
      grapple_beam_function = FUNC16(GrappleBeamFunc_Cancel);
      return;
    }
  } else {
    uint8 v2 = BlockCollGrappleBeam();
    if ((v2 & 1) == 0)
      return;
    if ((v2 & 0x40) == 0) {
      grapple_beam_function = FUNC16(GrappleBeamFunc_Cancel);
      return;
    }
  }
  QueueSfx1_Max6(6);
  HandleConnectingGrapple();
  grapple_beam_length_delta = -8;
  samus_grapple_flags |= 1;
}

void GrappleBeamFunc_ConnectedLockedInPlace(void) {  // 0x9BC77E
  if ((button_config_shoot_x & joypad1_lastkeys) != 0 && (GrappleBeam_CollDetect_Enemy().k || CheckIfGrappleIsConnectedToBlock())) {
  } else {
    grapple_beam_function = FUNC16(GrappleBeamFunc_Cancel);
  }
}

void GrappleBeamFunc_Connected_Swinging(void) {  // 0x9BC79D
  if ((button_config_shoot_x & joypad1_lastkeys) != 0) {
    GrappleBeamFunc_BB64();
    if (grapple_beam_length_delta)
      BlockFunc_AC31();
    GrappleBeamFunc_BC1F();
    GrappleBeamFunc_BCFF();
    GrappleBeamFunc_BD44();
    HandleMovementAndCollForSamusGrapple();
    if ((grapple_beam_unkD36 & 0x8000) != 0 && HandleSpecialGrappleBeamAngles() & 1) {
      return;
    }
    if (GrappleBeam_CollDetect_Enemy().k) {
      grapple_beam_flags |= 0x8000;
    } else if (!(CheckIfGrappleIsConnectedToBlock() & 1)) {
      goto LABEL_2;
    }
    BlockFunc_AC11();
    GrappleBeamFunc_BD95();
    return;
  }
LABEL_2:
  if (grapple_beam_unkD26 || grapple_beam_end_angle16 != 0x8000) {
    PropelSamusFromGrappleSwing();
    grapple_beam_function = FUNC16(GrappleBeamFunc_ReleaseFromSwing);
    samus_movement_handler = FUNC16(Samus_MoveHandler_ReleaseFromGrapple);
  } else {
    grapple_beam_function = FUNC16(GrappleBeam_Func2);
  }
}

void GrappleBeamFunc_Wallgrab(void) {  // 0x9BC814
  if ((button_config_shoot_x & joypad1_lastkeys) != 0 && CheckIfGrappleIsConnectedToBlock() & 1) {

  } else {
    grapple_walljump_timer = 30;
    grapple_beam_function = FUNC16(GrappleBeamFunc_C832);
  }
}

void GrappleBeamFunc_C832(void) {  // 0x9BC832
  if ((--grapple_walljump_timer & 0x8000) == 0) {
    if (Samus_GrappleWallJumpCheck(INT16_SHL16(16)))
      grapple_beam_function = FUNC16(GrappleBeamFunc_C9CE);
  } else {
    grapple_beam_function = FUNC16(GrappleBeam_Func2);
  }
}

void GrappleBeamFunc_Cancel(void) {  // 0x9BC856
  QueueSfx1_Max15(7);
  if (samus_movement_type == kMovementType_16_Grappling)
    Samus_Pose_CancelGrapple();
  else
    CallSomeSamusCode(0x1C);
  grapple_beam_unkD1E = 0;
  grapple_beam_unkD20 = 0;
  grapple_beam_direction = 0;
  grapple_beam_unkD36 = 0;
  grapple_walljump_timer = 0;
  slow_grabble_scrolling_flag = 0;
  grapple_varCF6 = 0;
  grapple_beam_flags = 0;
  flare_counter = 0;
  flare_animation_frame = 0;
  flare_slow_sparks_anim_frame = 0;
  flare_fast_sparks_anim_frame = 0;
  flare_animation_timer = 0;
  flare_slow_sparks_anim_timer = 0;
  flare_fast_sparks_anim_timer = 0;
  LoadProjectilePalette(equipped_beams);
  grapple_beam_function = FUNC16(GrappleBeamFunc_Inactive);
  samus_draw_handler = FUNC16(SamusDrawHandler_Default);
  Samus_PostGrappleCollisionDetect();
  if (samus_auto_cancel_hud_item_index) {
    hud_item_index = 0;
    samus_auto_cancel_hud_item_index = 0;
  }
}

void GrappleBeam_Func2(void) {  // 0x9BC8C5
  QueueSfx1_Max15(7);
  if (samus_pose == kPose_B2_FaceR_Grapple_Air)
    goto LABEL_5;
  if (samus_pose == kPose_B3_FaceL_Grapple_Air) {
LABEL_6:
    samus_new_pose_transitional = kPose_02_FaceL_Normal;
    goto LABEL_15;
  }
  if (sign16(samus_y_radius - 17)) {
    if ((kPoseParams[samus_pose].direction_shots_fired & 0xF0) != 0) {
      if (samus_pose_x_dir == 4)
        samus_new_pose_transitional = kPose_28_FaceL_Crouch;
      else
        samus_new_pose_transitional = kPose_27_FaceR_Crouch;
    } else {
      samus_new_pose_transitional = g_byte_9BC9C4[*(&kPoseParams[0].direction_shots_fired
                                                    + (8 * samus_pose))];
    }
    goto LABEL_15;
  }
  if ((kPoseParams[samus_pose].direction_shots_fired & 0xF0) != 0) {
    if (samus_pose_x_dir != 4) {
LABEL_5:
      samus_new_pose_transitional = kPose_01_FaceR_Normal;
      goto LABEL_15;
    }
    goto LABEL_6;
  }
  samus_new_pose_transitional = g_byte_9BC9BA[*(&kPoseParams[0].direction_shots_fired
                                                + (8 * samus_pose))];
LABEL_15:
  samus_hurt_switch_index = 0;
  input_to_pose_calc = 1;
  samus_x_base_speed = 0;
  samus_x_base_subspeed = 0;
  samus_y_subspeed = 0;
  samus_y_speed = 0;
  grapple_beam_unkD1E = 0;
  grapple_beam_unkD20 = 0;
  grapple_beam_direction = 0;
  grapple_beam_unkD36 = 0;
  grapple_walljump_timer = 0;
  slow_grabble_scrolling_flag = 0;
  grapple_varCF6 = 0;
  grapple_beam_flags = 0;
  flare_counter = 0;
  flare_animation_frame = 0;
  flare_slow_sparks_anim_frame = 0;
  flare_fast_sparks_anim_frame = 0;
  flare_animation_timer = 0;
  flare_slow_sparks_anim_timer = 0;
  flare_fast_sparks_anim_timer = 0;
  LoadProjectilePalette(equipped_beams);
  grapple_beam_function = FUNC16(GrappleBeamFunc_Inactive);
  samus_draw_handler = FUNC16(SamusDrawHandler_Default);
  Samus_PostGrappleCollisionDetect();
  if (samus_auto_cancel_hud_item_index) {
    hud_item_index = 0;
    samus_auto_cancel_hud_item_index = 0;
  }
}

void GrappleBeamFunc_C9CE(void) {  // 0x9BC9CE
  QueueSfx1_Max15(7);
  if (samus_pose_x_dir == 8)
    samus_new_pose_transitional = kPose_84_FaceL_Walljump;
  else
    samus_new_pose_transitional = kPose_83_FaceR_Walljump;
  samus_hurt_switch_index = 6;
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
  grapple_beam_unkD1E = 0;
  grapple_beam_unkD20 = 0;
  grapple_beam_direction = 0;
  grapple_beam_unkD36 = 0;
  grapple_walljump_timer = 0;
  slow_grabble_scrolling_flag = 0;
  grapple_varCF6 = 0;
  grapple_beam_flags = 0;
  flare_counter = 0;
  flare_animation_frame = 0;
  flare_slow_sparks_anim_frame = 0;
  flare_fast_sparks_anim_frame = 0;
  flare_animation_timer = 0;
  flare_slow_sparks_anim_timer = 0;
  flare_fast_sparks_anim_timer = 0;
  LoadProjectilePalette(equipped_beams);
  grapple_beam_function = FUNC16(GrappleBeamFunc_Inactive);
  samus_draw_handler = FUNC16(SamusDrawHandler_Default);
  Samus_PostGrappleCollisionDetect();
  if (samus_auto_cancel_hud_item_index) {
    hud_item_index = 0;
    samus_auto_cancel_hud_item_index = 0;
  }
}

void PropelSamusFromGrappleSwing(void) {  // 0x9BCA65
  int16 v1;
  int16 v2;
  uint16 v0;

  if ((grapple_beam_unkD26 & 0x8000) == 0) {
    v0 = 2 * grapple_beam_unkD26;
    v2 = kSinCosTable8bit_Sext[HIBYTE(grapple_beam_end_angle16) + 64];
    if (v2 >= 0) {
      SetHiLo(&samus_y_speed, &samus_y_subspeed, Multiply16x16(v2, v0));
      samus_y_dir = 2;
    } else {
      SetHiLo(&samus_y_speed, &samus_y_subspeed, Multiply16x16(-v2, v0));
      samus_y_dir = 1;
    }
  } else {
    v0 = -2 * grapple_beam_unkD26;
    v1 = kSinCosTable8bit_Sext[HIBYTE(grapple_beam_end_angle16) + 64];
    if (v1 < 0) {
      SetHiLo(&samus_y_speed, &samus_y_subspeed, Multiply16x16(-v1, v0));
      samus_y_dir = 2;
    } else {
      SetHiLo(&samus_y_speed, &samus_y_subspeed, Multiply16x16(v1, v0));
      samus_y_dir = 1;
    }
  }
  samus_x_accel_mode = 2;
  uint16 r18 = 3 * (v0 >> 9);
  r18 = 64 - r18;
  uint16 v3 = abs16(kSinCosTable8bit_Sext[(uint8)(grapple_beam_end_angle_hi - r18) + 64]);
  SetHiLo(&samus_x_base_speed, &samus_x_base_subspeed, Multiply16x16(v3, v0));
}

void GrappleBeamFunc_ReleaseFromSwing(void) {  // 0x9BCB8B
  QueueSfx1_Max15(7);
  if ((grapple_beam_unkD26 & 0x8000) == 0)
    samus_new_pose_transitional = kPose_52_FaceL_Jump_NoAim_MoveF;
  else
    samus_new_pose_transitional = kPose_51_FaceR_Jump_NoAim_MoveF;
  samus_hurt_switch_index = 7;
  grapple_beam_unkD1E = 0;
  grapple_beam_unkD20 = 0;
  grapple_beam_direction = 0;
  grapple_beam_unkD36 = 0;
  grapple_walljump_timer = 0;
  slow_grabble_scrolling_flag = 0;
  grapple_varCF6 = 0;
  grapple_beam_flags = 0;
  flare_counter = 0;
  flare_animation_frame = 0;
  flare_slow_sparks_anim_frame = 0;
  flare_fast_sparks_anim_frame = 0;
  flare_animation_timer = 0;
  flare_slow_sparks_anim_timer = 0;
  flare_fast_sparks_anim_timer = 0;
  LoadProjectilePalette(equipped_beams);
  grapple_beam_function = FUNC16(GrappleBeamFunc_Inactive);
  samus_draw_handler = FUNC16(SamusDrawHandler_Default);
  Samus_PostGrappleCollisionDetect();
  if (samus_auto_cancel_hud_item_index) {
    hud_item_index = 0;
    samus_auto_cancel_hud_item_index = 0;
  }
}
