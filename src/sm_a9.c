// Enemy AI - Mother Brain(complete), Shitroid &dead monsters

#include "ida_types.h"
#include "variables.h"
#include "funcs.h"
#include "enemy_types.h"
#include "sm_rtl.h"

#define g_word_A98929 ((uint16*)RomPtr(0xa98929))
static const uint16 g_word_A98B5D[5] = { 0x10, 0x10, 8, 8, 0x10 };
static const uint16 g_word_A98B67[5] = { 0x20, 0x20, 0x18, 0x18, 0x20 };
static const uint16 g_word_A98B71[5] = { 0xf8, 0xf8, 0xf0, 0xf0, 0xf6 };
#define g_off_A98B7B ((uint16*)RomPtr(0xa98b7b))
static const uint16 g_word_A98C61[4] = { 0xfff8, 2, 0xfffc, 6 };
static const uint8 g_byte_A98F7D[2] = { 9, 0x12 };
static const uint16 g_word_A98F7F[8] = { 0x3d, 0x54, 0x20, 0x35, 0x5a, 0x43, 0x67, 0x29 };

static const int16 g_word_A993BB[4] = { 0, -1, 0, 1 };
static const int16 g_word_A993C3[4] = { 0, 1, -1, 1 };

static const uint16 g_word_A99E0F[13] = { 0x6f, 0x6f, 0x6f, 0x7e, 0x6f, 0x6f, 0x7e, 0x6f, 0x6f, 0x7e, 0x7e, 0x6f, 0x6f };
#define g_word_A9B099 ((uint16*)RomPtr(0xa9b099))
#define g_word_A9B109 ((uint16*)RomPtr(0xa9b109))
#define g_word_A9B10F ((uint16*)RomPtr(0xa9b10f))
static const uint16 g_word_A9B393[8] = { 8, 0x6c, 0x18, 0x80, 9, 0x90, 0x18, 0x74 };
static const uint8 g_byte_A9B546[8] = { 0, 1, 1, 0, 0, 0, 0, 0 };
static const uint8 g_byte_A9B5A1[8] = { 2, 1, 1, 0, 0, 0, 0, 0 };
#define g_off_A9B6D4 ((uint16*)RomPtr(0xa9b6d4))
static const uint8 g_byte_A9B6DC[3] = { 0x40, 0x80, 0xc0 };
static const uint8 g_byte_A9B6DF[3] = { 0x10, 0x20, 0xd0 };
static const int16 g_word_A9B72C[28] = {
  -1, -17, 0, 0, -1,  -1,  0, -1,  0,  0, -1,  0, 0, 0, -17, -1,
  -1,  -1, 0, 0,  0, -17, -1, -1, -1, -1, -1, -1,
};
static const int16 g_word_A9BCA6[8] = { -8, 6, -4, 2, 3, -6, 8, 0 };
static const int16 g_word_A9BCB6[8] = { -7, 2, 5, -4, 6, -2, -6, 7 };
#define MotherBrain_RainbowBeamPalettes ((uint16*)RomPtr(0xade434))
static const uint8 g_byte_A9BEEE[16] = { 2, 0, 2, 0, 6, 0, 6, 0, 8, 0, 8, 0, 10, 0, 10, 0 };
static const uint16 g_word_A9BEFE[8] = { 0x500, 0x500, 0x200, 0x200, 0xc0, 0xc0, 0x40, 0x40 };
static const int16 g_word_A9C049[8] = { 0x10, 0x10, 0x20, 0x20, 0x30, 0x30, 0x40, 0x40 };
static const uint16 g_word_A9C544 = 1;
#define g_off_A9C61E ((uint16*)RomPtr(0xa9c61e))
#define g_off_A9C664 ((uint16*)RomPtr(0xa9c664))
#define kShitroid_FadingToBlack ((uint16*)RomPtr(0xade8e2))
#define g_word_A9CDFC ((uint16*)RomPtr(0xa9cdfc))

#define kShitroid_HealthBasedPalettes_Shell ((uint16*)RomPtr(0xade7e2))
#define kShitroid_HealthBasedPalettes_Innards ((uint16*)RomPtr(0xade882))
#define g_off_A9D260 ((uint16*)RomPtr(0xa9d260))
#define g_word_A9D583 ((uint16*)RomPtr(0xa9d583))
#define g_word_A9D549 ((uint16*)RomPtr(0xa9d549))
#define g_word_A9D67C ((uint16*)RomPtr(0xa9d67c))
#define g_word_A9D69C ((uint16*)RomPtr(0xa9d69c))
#define kDeadTorizo_TileData ((uint16*)RomPtr(0xb7a800))

#define g_off_A9D86A ((uint16*)RomPtr(0xa9d86a))
#define g_off_A9D870 ((uint16*)RomPtr(0xa9d870))
#define g_off_A9D897 ((uint16*)RomPtr(0xa9d897))
#define g_off_A9D89B ((uint16*)RomPtr(0xa9d89b))
#define g_off_A9D8C0 ((uint16*)RomPtr(0xa9d8c0))
#define g_off_A9D8C6 ((uint16*)RomPtr(0xa9d8c6))
#define g_word_A9D951 ((uint16*)RomPtr(0xa9d951))
#define g_word_A9D959 ((uint16*)RomPtr(0xa9d959))
static const int8 g_byte_A9F56A[16] = { 0x10, 0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

void CallCorpseRottingMove(uint32 ea, uint16 k, uint16 j) {
  switch (ea) {
  case fnTorizo_CorpseRottingCopyFunc: Torizo_CorpseRottingCopyFunc(k, j); return;
  case fnSidehopper_CorpseRottingCopyFunc_0: Sidehopper_CorpseRottingCopyFunc_0(k, j); return;
  case fnSidehopper_CorpseRottingCopyFunc_2: Sidehopper_CorpseRottingCopyFunc_2(k, j); return;
  case fnZoomer_CorpseRottingCopyFunc_0: Zoomer_CorpseRottingCopyFunc_0(k, j); return;
  case fnZoomer_CorpseRottingCopyFunc_2: Zoomer_CorpseRottingCopyFunc_2(k, j); return;
  case fnZoomer_CorpseRottingCopyFunc_4: Zoomer_CorpseRottingCopyFunc_4(k, j); return;
  case fnRipper_CorpseRottingCopyFunc_0: Ripper_CorpseRottingCopyFunc_0(k, j); return;
  case fnRipper_CorpseRottingCopyFunc_2: Ripper_CorpseRottingCopyFunc_2(k, j); return;
  case fnSkree_CorpseRottingCopyFunc_0: Skree_CorpseRottingCopyFunc_0(k, j); return;
  case fnSkree_CorpseRottingCopyFunc_2: Skree_CorpseRottingCopyFunc_2(k, j); return;
  case fnSkree_CorpseRottingCopyFunc_4: Skree_CorpseRottingCopyFunc_4(k, j); return;
  case fnMotherBrain_CorpseRottingCopyFunc: MotherBrain_CorpseRottingCopyFunc(k, j); return;

  case fnTorizo_CorpseRottingMoveFunc: Torizo_CorpseRottingMoveFunc(k, j); return;
  case fnSidehopper_CorpseRottingMoveFunc_0: Sidehopper_CorpseRottingMoveFunc_0(k, j); return;
  case fnSidehopper_CorpseRottingMoveFunc_2: Sidehopper_CorpseRottingMoveFunc_2(k, j); return;
  case fnZoomer_CorpseRottingMoveFunc_0: Zoomer_CorpseRottingMoveFunc_0(k, j); return;
  case fnZoomer_CorpseRottingMoveFunc_2: Zoomer_CorpseRottingMoveFunc_2(k, j); return;
  case fnZoomer_CorpseRottingMoveFunc_4: Zoomer_CorpseRottingMoveFunc_4(k, j); return;
  case fnRipper_CorpseRottingMoveFunc_0: Ripper_CorpseRottingMoveFunc_0(k, j); return;
  case fnRipper_CorpseRottingMoveFunc_2: Ripper_CorpseRottingMoveFunc_2(k, j); return;
  case fnSkree_CorpseRottingMoveFunc_0: Skree_CorpseRottingMoveFunc_0(k, j); return;
  case fnSkree_CorpseRottingMoveFunc_2: Skree_CorpseRottingMoveFunc_2(k, j); return;
  case fnSkree_CorpseRottingMoveFunc_4: Skree_CorpseRottingMoveFunc_4(k, j); return;
  case fnMotherBrain_CorpseRottingMoveFunc: MotherBrain_CorpseRottingMoveFunc(k, j); return;
  default: Unreachable();
  }
}
void CallCorpseRottingInit(uint32 ea) {
  switch (ea) {
  case fnTorizo_CorpseRottingInitFunc: Torizo_CorpseRottingInitFunc(); return;
  case fnSidehopper_CorpseRottingInitFunc_0: Sidehopper_CorpseRottingInitFunc_0(); return;
  case fnSidehopper_CorpseRottingInitFunc_2: Sidehopper_CorpseRottingInitFunc_2(); return;
  case fnZoomer_CorpseRottingInitFunc_0: Zoomer_CorpseRottingInitFunc_0(); return;
  case fnZoomer_CorpseRottingInitFunc_2: Zoomer_CorpseRottingInitFunc_2(); return;
  case fnZoomer_CorpseRottingInitFunc_4: Zoomer_CorpseRottingInitFunc_4(); return;
  case fnRipper_CorpseRottingInitFunc_0: Ripper_CorpseRottingInitFunc_0(); return;
  case fnRipper_CorpseRottingInitFunc_2: Ripper_CorpseRottingInitFunc_2(); return;
  case fnSkree_CorpseRottingInitFunc_0: Skree_CorpseRottingInitFunc_0(); return;
  case fnSkree_CorpseRottingInitFunc_2: Skree_CorpseRottingInitFunc_2(); return;
  case fnSkree_CorpseRottingInitFunc_4: Skree_CorpseRottingInitFunc_4(); return;
  case fnMotherBrain_CorpseRottingInitFunc: MotherBrain_CorpseRottingInitFunc(); return;
  default: Unreachable();
  }
}
void CallCorpseRottingFinish(uint32 ea) {
  switch (ea) {
  case fnMotherBrain_CorpseRottingFinished: MotherBrain_CorpseRottingFinished(); return;
  case fnDeadTorizo_CorpseRottingFinished: DeadTorizo_CorpseRottingFinished(); return;
  case fnCorpseRottingRotEntryFinishedHook: CorpseRottingRotEntryFinishedHook(); return;
  default: Unreachable();
  }
}

void CallMotherBrainFunc(uint32 ea) {
  switch (ea) {
  case fnMotherBrainsBrain_SetupBrainToDraw: MotherBrainsBrain_SetupBrainToDraw(); return;
  case fnMotherBrainsBody_FirstPhase_DoubleRet: Unreachable(); return;
  case fnMotherBrainBody_0_Wait: MotherBrainBody_0_Wait(); return;
  case fnMotherBrainBody_1_ClearBottomLeftTube: MotherBrainBody_1_ClearBottomLeftTube(); return;
  case fnMotherBrainBody_2_SpawnTopRightTubeFalling: MotherBrainBody_2_SpawnTopRightTubeFalling(); return;
  case fnMotherBrainBody_3_ClearCeilingBlock9: MotherBrainBody_3_ClearCeilingBlock9(); return;
  case fnMotherBrainBody_4_SpawnTopLeftTubeFalling: MotherBrainBody_4_SpawnTopLeftTubeFalling(); return;
  case fnMotherBrainBody_4_ClearCeilingBlock6: MotherBrainBody_4_ClearCeilingBlock6(); return;
  case fnMotherBrainBody_5_SpawnTubeFallingEnemy1: MotherBrainBody_5_SpawnTubeFallingEnemy1(); return;
  case fnMotherBrainBody_6_ClearBottomRightTube: MotherBrainBody_6_ClearBottomRightTube(); return;
  case fnMotherBrainBody_7_SpawnTubeFallingEnemy2: MotherBrainBody_7_SpawnTubeFallingEnemy2(); return;
  case fnMotherBrainBody_8_ClearBottomMiddleLeftTube: MotherBrainBody_8_ClearBottomMiddleLeftTube(); return;
  case fnMotherBrainBody_9_SpawnTopMiddleLeftFalling: MotherBrainBody_9_SpawnTopMiddleLeftFalling(); return;
  case fnMotherBrainBody_10_ClearCeilingTubeColumn7: MotherBrainBody_10_ClearCeilingTubeColumn7(); return;
  case fnMotherBrainBody_11_SpawnTopMiddleRightFalling: MotherBrainBody_11_SpawnTopMiddleRightFalling(); return;
  case fnMotherBrainBody_12_ClearCeilingTubeColumn8: MotherBrainBody_12_ClearCeilingTubeColumn8(); return;
  case fnMotherBrainBody_13_SpawnTubeFallingEnemy3: MotherBrainBody_13_SpawnTubeFallingEnemy3(); return;
  case fnMotherBrainBody_14_ClearBottomMiddleRightTube: MotherBrainBody_14_ClearBottomMiddleRightTube(); return;
  case fnMotherBrainBody_15_SpawnTubeFallingEnemy4: MotherBrainBody_15_SpawnTubeFallingEnemy4(); return;
  case fnMotherBrainBody_16_ClearBottomMiddleTubes: MotherBrainBody_16_ClearBottomMiddleTubes(); return;
  case fnMotherBrainBody_FakeDeath_Descent_0_Pause: MotherBrainBody_FakeDeath_Descent_0_Pause(); return;
  case fnMotherBrainBody_FakeDeath_Descent_1: MotherBrainBody_FakeDeath_Descent_1(); return;
  case fnMotherBrainBody_FakeDeath_Descent_2: MotherBrainBody_FakeDeath_Descent_2(); return;
  case fnMotherBrainBody_FakeDeath_Descent_3: MotherBrainBody_FakeDeath_Descent_3(); return;
  case fnMotherBrainBody_FakeDeath_Descent_4: MotherBrainBody_FakeDeath_Descent_4(); return;
  case fnMotherBrainBody_FakeDeath_Descent_5: MotherBrainBody_FakeDeath_Descent_5(); return;
  case fnMotherBrainBody_FakeDeath_Descent_6: MotherBrainBody_FakeDeath_Descent_6(); return;
  case fnMotherBrainBody_FakeDeath_Ascent_0_DrawBG1Row23: MotherBrainBody_FakeDeath_Ascent_0_DrawBG1Row23(); return;
  case fnMotherBrainBody_FakeDeath_Ascent_1_DrawBG1Row45: MotherBrainBody_FakeDeath_Ascent_1_DrawBG1Row45(); return;
  case fnMotherBrainBody_FakeDeath_Ascent_2_DrawBG1Row67: MotherBrainBody_FakeDeath_Ascent_2_DrawBG1Row67(); return;
  case fnMotherBrainBody_FakeDeath_Ascent_3_DrawBG1Row89: MotherBrainBody_FakeDeath_Ascent_3_DrawBG1Row89(); return;
  case fnMotherBrainBody_FakeDeath_Ascent_4_DrawBG1RowAB: MotherBrainBody_FakeDeath_Ascent_4_DrawBG1RowAB(); return;
  case fnMotherBrainBody_FakeDeath_Ascent_5_DrawBG1RowCD: MotherBrainBody_FakeDeath_Ascent_5_DrawBG1RowCD(); return;
  case fnMotherBrainBody_FakeDeath_Ascent_6_SetupPhase2Gfx: MotherBrainBody_FakeDeath_Ascent_6_SetupPhase2Gfx(); return;
  case fnMotherBrainBody_FakeDeath_Ascent_7_SetupPhase2Brain: MotherBrainBody_FakeDeath_Ascent_7_SetupPhase2Brain(); return;
  case fnMotherBrainBody_FakeDeath_Ascent_8_Pause: MotherBrainBody_FakeDeath_Ascent_8_Pause(); return;
  case fnMotherBrainBody_FakeDeath_Ascent_9_PrepareRise: MotherBrainBody_FakeDeath_Ascent_9_PrepareRise(); return;
  case fnMotherBrainBody_FakeDeath_Ascent_10_LoadLegTiles: MotherBrainBody_FakeDeath_Ascent_10_LoadLegTiles(); return;
  case fnMotherBrainBody_FakeDeath_Ascent_11_ContinuePause: MotherBrainBody_FakeDeath_Ascent_11_ContinuePause(); return;
  case fnMotherBrainBody_FakeDeath_Ascent_12_StartMusic: MotherBrainBody_FakeDeath_Ascent_12_StartMusic(); return;
  case fnMotherBrainBody_FakeDeath_Ascent_13_Raise: MotherBrainBody_FakeDeath_Ascent_13_Raise(); return;
  case fnMotherBrainBody_FakeDeath_Ascent_14_WaitForUncouching: MotherBrainBody_FakeDeath_Ascent_14_WaitForUncouching(); return;
  case fnMotherBrainBody_FakeDeath_Ascent_15_TransitionFromGrey: MotherBrainBody_FakeDeath_Ascent_15_TransitionFromGrey(); return;
  case fnMotherBrainBody_2ndphase_16_ShakeHeadMenacingly: MotherBrainBody_2ndphase_16_ShakeHeadMenacingly(); return;
  case fnMotherBrainBody_2ndphase_17_BringHeadBackUp: MotherBrainBody_2ndphase_17_BringHeadBackUp(); return;
  case fnMotherBrainBody_2ndphase_18_FinishStretching: MotherBrainBody_2ndphase_18_FinishStretching(); return;
  case fnMotherBrain_Phase3_Death_0: MotherBrain_Phase3_Death_0(); return;
  case fnMotherBrain_Phase3_Death_1: MotherBrain_Phase3_Death_1(); return;
  case fnMotherBrain_Phase3_Death_2: MotherBrain_Phase3_Death_2(); return;
  case fnMotherBrain_Phase3_Death_3: MotherBrain_Phase3_Death_3(); return;
  case fnMotherBrain_Phase3_Death_4: MotherBrain_Phase3_Death_4(); return;
  case fnMotherBrain_Phase3_Death_5: MotherBrain_Phase3_Death_5(); return;
  case fnMotherBrain_Phase3_Death_6: MotherBrain_Phase3_Death_6(); return;
  case fnMotherBrain_Phase3_Death_7: MotherBrain_Phase3_Death_7(); return;
  case fnMotherBrain_Phase3_Death_8: MotherBrain_Phase3_Death_8(); return;
  case fnMotherBrain_Phase3_Death_9: MotherBrain_Phase3_Death_9(); return;
  case fnMotherBrain_Phase3_Death_10: MotherBrain_Phase3_Death_10(); return;
  case fnMotherBrain_Phase3_Death_11: MotherBrain_Phase3_Death_11(); return;
  case fnMotherBrain_Phase3_Death_12: MotherBrain_Phase3_Death_12(); return;
  case fnMotherBrain_Phase3_Death_13: MotherBrain_Phase3_Death_13(); return;
  case fnMotherBrain_Phase3_Death_14_20framedelay: MotherBrain_Phase3_Death_14_20framedelay(); return;
  case fnMotherBrain_Phase3_Death_15_LoadEscapeTimerTiles: MotherBrain_Phase3_Death_15_LoadEscapeTimerTiles(); return;
  case fnMotherBrain_Phase3_Death_16_StartEscape: MotherBrain_Phase3_Death_16_StartEscape(); return;
  case fnMotherBrain_Phase3_Death_17_SpawnTimeBomb: MotherBrain_Phase3_Death_17_SpawnTimeBomb(); return;
  case fnMotherBrain_Phase3_Death_18_TypesZebesText: MotherBrain_Phase3_Death_18_TypesZebesText(); return;
  case fnMotherBrain_Phase3_Death_19_EscapeDoorExploding: MotherBrain_Phase3_Death_19_EscapeDoorExploding(); return;
  case fnMotherBrain_Phase3_Death_20_BlowUpEscapeDoor: MotherBrain_Phase3_Death_20_BlowUpEscapeDoor(); return;
  case fnMotherBrain_Phase3_Death_21_KeepEarthquakeGoing: MotherBrain_Phase3_Death_21_KeepEarthquakeGoing(); return;
  case fnMotherBrain_Body_Phase2_Thinking: MotherBrain_Body_Phase2_Thinking(); return;
  case fnMotherBrain_Body_Phase2_TryAttack: MotherBrain_Body_Phase2_TryAttack(); return;
  case fnMotherBrain_FiringBomb_DecideOnWalking: MotherBrain_FiringBomb_DecideOnWalking(); return;
  case fnMotherBrain_FiringBomb_WalkingBackwards: MotherBrain_FiringBomb_WalkingBackwards(); return;
  case fnMotherBrain_FiringBomb_Crouch: MotherBrain_FiringBomb_Crouch(); return;
  case fnMotherBrain_FiringBomb_Fired: MotherBrain_FiringBomb_Fired(); return;
  case fnMotherBrain_FiringBomb_Standup: MotherBrain_FiringBomb_Standup(); return;
  case fnMotherBomb_FiringLaser_PositionHead: MotherBomb_FiringLaser_PositionHead(); return;
  case fnMotherBomb_FiringLaser_PositionHeadSlowlyFire: MotherBomb_FiringLaser_PositionHeadSlowlyFire(); return;
  case fnMotherBomb_FiringLaser_FinishAttack: MotherBomb_FiringLaser_FinishAttack(); return;
  case fnMotherBomb_FiringDeathBeam: MotherBomb_FiringDeathBeam(); return;
  case fnMotherBomb_FiringRainbowBeam_0: MotherBomb_FiringRainbowBeam_0(); return;
  case fnMotherBomb_FiringRainbowBeam_1_StartCharge: MotherBomb_FiringRainbowBeam_1_StartCharge(); return;
  case fnMotherBomb_FiringRainbowBeam_2_RetractNeck: MotherBomb_FiringRainbowBeam_2_RetractNeck(); return;
  case fnMotherBomb_FiringRainbowBeam_3_Wait: MotherBomb_FiringRainbowBeam_3_Wait(); return;
  case fnMotherBomb_FiringRainbowBeam_4_ExtendNeckDown: MotherBomb_FiringRainbowBeam_4_ExtendNeckDown(); return;
  case fnMotherBomb_FiringRainbowBeam_5_StartFiring: MotherBomb_FiringRainbowBeam_5_StartFiring(); return;
  case fnMotherBomb_FiringRainbowBeam_6_MoveSamusToWall: MotherBomb_FiringRainbowBeam_6_MoveSamusToWall(); return;
  case fnMotherBomb_FiringRainbowBeam_7_DelayFrame: MotherBomb_FiringRainbowBeam_7_DelayFrame(); return;
  case fnMotherBomb_FiringRainbowBeam_8_StartDrainSamus: MotherBomb_FiringRainbowBeam_8_StartDrainSamus(); return;
  case fnMotherBomb_FiringRainbowBeam_9_DrainingSamus: MotherBomb_FiringRainbowBeam_9_DrainingSamus(); return;
  case fnMotherBomb_FiringRainbowBeam_10_FinishFiringRainbow: MotherBomb_FiringRainbowBeam_10_FinishFiringRainbow(); return;
  case fnMotherBomb_FiringRainbowBeam_11_LetSamusFall: MotherBomb_FiringRainbowBeam_11_LetSamusFall(); return;
  case fnMotherBomb_FiringRainbowBeam_12_WaitForSamusHitGround: MotherBomb_FiringRainbowBeam_12_WaitForSamusHitGround(); return;
  case fnMotherBomb_FiringRainbowBeam_13_LowerHead: MotherBomb_FiringRainbowBeam_13_LowerHead(); return;
  case fnMotherBomb_FiringRainbowBeam_14_DecideNextAction: MotherBomb_FiringRainbowBeam_14_DecideNextAction(); return;
  case fnMotherBrain_Phase2Cut_0: MotherBrain_Phase2Cut_0(); return;
  case fnMotherBrain_Phase2Cut_1: MotherBrain_Phase2Cut_1(); return;
  case fnMotherBrain_Phase2Cut_2: MotherBrain_Phase2Cut_2(); return;
  case fnMotherBrain_Phase2Cut_3: MotherBrain_Phase2Cut_3(); return;
  case fnMotherBrain_Phase2Cut_4: MotherBrain_Phase2Cut_4(); return;
  case fnMotherBrain_Phase2Cut_5: MotherBrain_Phase2Cut_5(); return;
  case fnnullsub_364: return;
  case fnMotherBrain_DrainedByShitroid_0: MotherBrain_DrainedByShitroid_0(); return;
  case fnMotherBrain_DrainedByShitroid_1: MotherBrain_DrainedByShitroid_1(); return;
  case fnMotherBrain_DrainedByShitroid_2: MotherBrain_DrainedByShitroid_2(); return;
  case fnMotherBrain_DrainedByShitroid_3: MotherBrain_DrainedByShitroid_3(); return;
  case fnMotherBrain_DrainedByShitroid_4: MotherBrain_DrainedByShitroid_4(); return;
  case fnMotherBrain_DrainedByShitroid_5: MotherBrain_DrainedByShitroid_5(); return;
  case fnMotherBrain_DrainedByShitroid_6: MotherBrain_DrainedByShitroid_6(); return;
  case fnMotherBrain_DrainedByShitroid_7: MotherBrain_DrainedByShitroid_7(); return;
  case fnMotherBrain_Phase2_Revive_0: MotherBrain_Phase2_Revive_0(); return;
  case fnMotherBrain_Phase2_Revive_1: MotherBrain_Phase2_Revive_1(); return;
  case fnMotherBrain_Phase2_Revive_2: MotherBrain_Phase2_Revive_2(); return;
  case fnMotherBrain_Phase2_Revive_3: MotherBrain_Phase2_Revive_3(); return;
  case fnMotherBrain_Phase2_Revive_4: MotherBrain_Phase2_Revive_4(); return;
  case fnMotherBrain_Phase2_Revive_5: MotherBrain_Phase2_Revive_5(); return;
  case fnMotherBrain_Phase2_Revive_6: MotherBrain_Phase2_Revive_6(); return;
  case fnMotherBrain_Phase2_Revive_7: MotherBrain_Phase2_Revive_7(); return;
  case fnMotherBrain_Phase2_Revive_8: MotherBrain_Phase2_Revive_8(); return;
  case fnMotherBrain_Phase2_MurderShitroid_1: MotherBrain_Phase2_MurderShitroid_1(); return;
  case fnMotherBrain_Phase2_MurderShitroid_2: MotherBrain_Phase2_MurderShitroid_2(); return;
  case fnMotherBrain_Phase2_PrepareForFinalShitroid: MotherBrain_Phase2_PrepareForFinalShitroid(); return;
  case fnMotherBrain_Phase2_ExecuteFinalkShitroid: MotherBrain_Phase2_ExecuteFinalkShitroid(); return;
  case fnnullsub_363: return;
  case fnMotherBrain_Phase3_Recover_MakeDistance: MotherBrain_Phase3_Recover_MakeDistance(); return;
  case fnMotherBrain_Phase3_Recover_SetupForFight: MotherBrain_Phase3_Recover_SetupForFight(); return;
  case fnMotherBrain_Phase3_Fighting_Main: MotherBrain_Phase3_Fighting_Main(); return;
  case fnMotherBrain_Phase3_Fighting_Cooldown: MotherBrain_Phase3_Fighting_Cooldown(); return;
  case fnnullsub_365: return;
  case fnnullsub_367: return;
  case fnMotherBrainsBrain_SetupBrainAndNeckToDraw: MotherBrainsBrain_SetupBrainAndNeckToDraw(); return;
  default: Unreachable();
  }
}
void Enemy_GrappleReact_CancelBeam_A9(void) {  // 0xA9800F
  Enemy_SwitchToFrozenAi();
}

void Enemy_NormalFrozenAI_A9(void) {  // 0xA98041
  NormalEnemyFrozenAI();
}

uint16 EnemyInstr_Sleep_A9(uint16 k, uint16 j) {  // 0xA9812F
  gEnemyData(k)->current_instruction = j - 2;
  return 0;
}

void MotherBrainsBody_Init(void) {  // 0xA98687
  for (int i = 4094; i >= 0; i -= 2)
    tilemap_stuff[i >> 1] = 824;
  MotherBrain_SetBodyInstrs(addr_kMotherBrain_Ilist_9C13);
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->base.vram_tiles_index = 0;
  E->base.properties |= kEnemyProps_BlockPlasmaBeam | kEnemyProps_Tangible | kEnemyProps_Invisible;
  E->base.palette_index = 0;
  WriteColorsToTargetPalette(0xa9, 0x162, addr_kMotherBrainPalette_4 + 2, 0xF);
  WriteColorsToTargetPalette(0xa9, 0x1E2, addr_kMotherBrainPalette_3 + 2, 0xF);
  E->mbn_var_00 = 0;

  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  E1->mbn_var_02 = 0;
  E->mbn_var_04 = 2;
  E1->mbn_var_A = FUNC16(MotherBrainsBrain_SetupBrainToDraw);
  E->mbn_var_A = FUNC16(MotherBrainsBody_FirstPhase_DoubleRet);
  LoadFxEntry(1u);
  uint16 v3 = 0, v4;
  do {
    v4 = v3;
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_MotherBrainRoomTurrets, v3);
    v3 = v4 + 1;
  } while ((uint16)(v4 + 1) < 0xCu);
}

void MotherBrainsBrain_Init(void) {  // 0xA98705
  InitializeEnemyCorpseRotting(0x40, addr_stru_A9DE08);
  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  E1->base.health = 3000;
  MotherBrain_SetBrainUnusedInstrs(addr_kMotherBrain_Ilist_9C13);
  E1->base.vram_tiles_index = 0;
  E1->base.properties |= 0x1100u;
  E1->base.palette_index = 512;
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->mbn_var_0C = 512;
  E->mbn_var_0D = 512;
  MotherBrain_SetBrainInstrs(addr_stru_A99C21);
  MotherBrain_SetupBrainNormalPal();
}

void MotherBrainsBody_Hurt(void) {  // 0xA9873E
  MotherBrain_Pal_HandleRoomPal();
  Enemy_MotherBrain *E = Get_MotherBrain(0);

  if (E->mbn_var_A == (uint16)fnMotherBrainsBody_FirstPhase_DoubleRet) {
    MotherBrainsBody_FirstPhase_DoubleRet();
    return;
  }

  CallMotherBrainFunc(E->mbn_var_A | 0xA90000);
  MotherBrain_HandlePalette();
  MotherBrain_SamusCollDetect();
  MotherBrain_Pal_ProcessInvincibility();
  if (Get_MotherBrain(0x40)->mbn_var_02)
    mov24(&unpause_hook, fnMotherBrainsBody_UnpauseHook);
}

void MotherBrainsBody_UnpauseHook(void) {  // 0xA98763
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if (E->mbn_var_16)
    QueueSfx1_Max6(0x40);
  if ((E->base.extra_properties & 4) != 0) {
    enemy_bg2_tilemap_size = 2048;
    nmi_flag_bg2_enemy_vram_transfer = 1;
  }
}

void MotherBrainsBody_Powerbomb(void) {  // 0xA98787
  NormalEnemyPowerBombAiSkipDeathAnim_CurEnemy();
  MotherBrainsBrain_Hurt();
}

void MotherBrainsBrain_Hurt(void) {  // 0xA9878B
  mov24(&enemy_gfx_drawn_hook, 0xA98786);
  EnemyData *v0 = gEnemyData(0);
  if ((v0[1].properties & 0x100) != 0)
    CallMotherBrainFunc(v0[1].ai_var_A | 0xA90000);
}

void MotherBrainsBrain_SetupBrainAndNeckToDraw(void) {  // 0xA987A2
  if (!time_is_frozen_flag)
    MotherBrain_HandleNeck();
  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  E1->base.x_pos = E1->mbn_var_2E;
  E1->base.y_pos = E1->mbn_var_2F - 21;
  mov24(&enemy_gfx_drawn_hook, 0xA987C9);
}

void MotherBrain_DrawBrainNeck_EnemyGfxDrawHook(void) {  // 0xA987C9
  MotherBrain_DrawBrain();
  MotherBrain_DrawNeck();
}

void MotherBrainsBrain_SetupBrainToDraw(void) {  // 0xA987D0
  mov24(&enemy_gfx_drawn_hook, 0xA987DD);
}

void MotherBrainsBrain_GfxDrawHook(void) {  // 0xA987DD
  MotherBrain_DrawBrain();
}

void MotherBrainsBody_FirstPhase_DoubleRet(void) {  // 0xA987E1
  if (CheckEventHappened(2u)) {
    Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
    E1->mbn_var_00 = earthquake_timer;
    if (sign16(samus_x_pos - 236)) {
      if (!E1->base.health) {
        Enemy_MotherBrain *E = Get_MotherBrain(0);
        E->mbn_var_1D = 1;
        E->mbn_var_00 = 1;
        DisableMinimapAndMarkBossRoomAsExplored();
        QueueMusic_Delayed8(6u);
        MotherBrain_SealWall();
      }
    }
  }
  MotherBrain_SamusCollDetect();
}

void MotherBrainBody_FakeDeath_Descent_0_Pause(void) {  // 0xA9881D
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0);
  E->mbby_var_A = FUNC16(MotherBrainBody_FakeDeath_Descent_1);
  E->mbby_var_F = 64;
  MotherBrainBody_FakeDeath_Descent_1();
}

void MotherBrainBody_FakeDeath_Descent_1(void) {  // 0xA98829
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0);
  if ((--E->mbby_var_F & 0x8000) != 0) {
    CallSomeSamusCode(0);
    *(uint16 *)scrolls = scrolls[0];
    E->mbby_var_A = FUNC16(MotherBrainBody_FakeDeath_Descent_2);
    E->mbby_var_F = 32;
    MotherBrainBody_FakeDeath_Descent_2();
  }
}

void MotherBrainBody_FakeDeath_Descent_2(void) {  // 0xA9884D
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0);
  if ((--E->mbby_var_F & 0x8000) != 0) {
    QueueMusic_Delayed8(0);
    QueueMusic_Delayed8(0xFF21u);
    E->mbby_var_A = FUNC16(MotherBrainBody_FakeDeath_Descent_3);
    E->mbby_var_F = 12;
    MotherBrainBody_FakeDeath_Descent_3();
  }
}

void MotherBrainBody_FakeDeath_Descent_3(void) {  // 0xA9886C
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0);
  if ((--E->mbby_var_F & 0x8000) != 0) {
    CallSomeSamusCode(1u);
    E->mbby_var_A = FUNC16(MotherBrainBody_FakeDeath_Descent_4);
    E->mbby_var_F = 8;
    MotherBrainBody_FakeDeath_Descent_4();
  }
}

void MotherBrainBody_FakeDeath_Descent_4(void) {  // 0xA98884
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    MotherBrain_Pal_BeginScreenFlash();
    LoadFxEntry(2u);
    Get_MotherBrain(0x40)->mbn_var_E = FUNC16(MotherBrainBody_0_Wait);
    E->mbn_var_A = FUNC16(MotherBrainBody_FakeDeath_Descent_5);
    E->mbn_var_F = 0;
    E->mbn_var_37 = 0;
    static const SpawnHardcodedPlmArgs unk_A988AD = { 0x0e, 0x02, 0xb6b3 };
    SpawnHardcodedPlm(&unk_A988AD);
  }
}

void MotherBrainBody_FakeDeath_Descent_5(void) {  // 0xA988B2
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    E->mbn_var_F = 8;
    uint16 v2 = E->mbn_var_37 + 1;
    E->mbn_var_37 = v2;
    if (MotherBrain_FadeToGray_FakeDeath(v2 - 1) & 1)
      E->mbn_var_A = FUNC16(MotherBrainBody_FakeDeath_Descent_6);
  }
  MotherBrainBody_FakeDeath_Descent_6();
}

void MotherBrainBody_FakeDeath_Descent_6(void) {  // 0xA988D3
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0x40);
  CallMotherBrainFunc(E->mbby_var_E | 0xA90000);
  MotherBrain_HandleFakeDeathExplosions();
}

void MotherBrain_HandleFakeDeathExplosions(void) {  // 0xA988DD
  int16 v1;
  int16 v2;

  Enemy_MotherBrain *E = Get_MotherBrain(0);
  v1 = E->mbn_var_38 - 1;
  if (v1 < 0) {
    E->mbn_var_38 = 8;
    v2 = E->mbn_var_39 - 1;
    if (v2 < 0)
      v2 = 7;
    E->mbn_var_39 = v2;
    int v3 = (uint16)(4 * v2) >> 1;
    R18_ = g_word_A98929[v3];
    R20_ = g_word_A98929[v3 + 1];
    uint16 v4 = 3;
    if (random_number < 0x4000)
      v4 = 12;
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, v4);
    QueueSfx2_Max3(0x24u);
  } else {
    E->mbn_var_38 = v1;
  }
}

void MotherBrainBody_0_Wait(void) {  // 0xA98949
  int16 v1;

  uint16 v0 = 0;
  v1 = 0;
  do {
    if (!*(uint16 *)((char *)enemy_projectile_id + v0))
      ++v1;
    v0 += 2;
  } while ((int16)(v0 - 36) < 0);
  if ((int16)(v1 - 4) >= 0) {
    SpawnEnemy(0xA9, addr_stru_A98AE5);
    Get_MotherBrainBody(0x40)->mbby_var_E = FUNC16(MotherBrainBody_1_ClearBottomLeftTube);
  }
}

static const SpawnHardcodedPlmArgs unk_A98972 = { 0x05, 0x09, 0xb6c3 };
static const SpawnHardcodedPlmArgs unk_A989D6 = { 0x06, 0x02, 0xb6b3 };
static const SpawnHardcodedPlmArgs unk_A989FE = { 0x0a, 0x09, 0xb6c7 };
static const SpawnHardcodedPlmArgs unk_A98A26 = { 0x06, 0x0a, 0xb6bb };
static const SpawnHardcodedPlmArgs unk_A98A58 = { 0x07, 0x02, 0xb6b7 };
static const SpawnHardcodedPlmArgs unk_A98A8A = { 0x08, 0x02, 0xb6b7 };
static const SpawnHardcodedPlmArgs unk_A989A4 = { 0x09, 0x02, 0xb6b3 };
static const SpawnHardcodedPlmArgs unk_A98AB2 = { 0x09, 0x0a, 0xb6bb };
static const SpawnHardcodedPlmArgs unk_A98ADA = { 0x07, 0x07, 0xb6bf };

void MotherBrainBody_1_ClearBottomLeftTube(void) {  // 0xA9896E
  SpawnHardcodedPlm(&unk_A98972);
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0x40);
  E->mbby_var_E = FUNC16(MotherBrainBody_2_SpawnTopRightTubeFalling);
  E->mbby_var_F = 32;
}

void MotherBrainBody_2_SpawnTopRightTubeFalling(void) {  // 0xA98983
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0x40);
  if ((--E->mbby_var_F & 0x8000) != 0) {
    R18_ = 152;
    R20_ = 47;
    SpawnEnemyProjectileWithRoomGfx(0xCC5B, 0x2F);
    E->mbby_var_E = -30304;
  }
}

void MotherBrainBody_3_ClearCeilingBlock9(void) {  // 0xA989A0
  SpawnHardcodedPlm(&unk_A989A4);
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0x40);
  E->mbby_var_E = FUNC16(MotherBrainBody_4_SpawnTopLeftTubeFalling);
  E->mbby_var_F = 32;
}

void MotherBrainBody_4_SpawnTopLeftTubeFalling(void) {  // 0xA989B5
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0x40);
  if ((--E->mbby_var_F & 0x8000) != 0) {
    R18_ = 104;
    R20_ = 47;
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_MotherBrainTubeFalling_TopLeft, 0x2F);
    E->mbby_var_E = FUNC16(MotherBrainBody_4_ClearCeilingBlock6);
  }
}

void MotherBrainBody_4_ClearCeilingBlock6(void) {  // 0xA989D2
  SpawnHardcodedPlm(&unk_A989D6);
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0x40);
  E->mbby_var_E = FUNC16(MotherBrainBody_5_SpawnTubeFallingEnemy1);
  E->mbby_var_F = 32;
}

void MotherBrainBody_5_SpawnTubeFallingEnemy1(void) {  // 0xA989E7
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0x40);
  if ((--E->mbby_var_F & 0x8000) != 0) {
    SpawnEnemy(0xA9, addr_stru_A98AF5);
    E->mbby_var_E = FUNC16(MotherBrainBody_6_ClearBottomRightTube);
  }
}

void MotherBrainBody_6_ClearBottomRightTube(void) {  // 0xA989FA
  SpawnHardcodedPlm(&unk_A989FE);
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0x40);
  E->mbby_var_E = FUNC16(MotherBrainBody_7_SpawnTubeFallingEnemy2);
  E->mbby_var_F = 32;
}

void MotherBrainBody_7_SpawnTubeFallingEnemy2(void) {  // 0xA98A0F
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0x40);
  if ((--E->mbby_var_F & 0x8000) != 0) {
    SpawnEnemy(0xA9, addr_stru_A98B05);
    E->mbby_var_E = FUNC16(MotherBrainBody_8_ClearBottomMiddleLeftTube);
  }
}

void MotherBrainBody_8_ClearBottomMiddleLeftTube(void) {  // 0xA98A22
  SpawnHardcodedPlm(&unk_A98A26);
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0x40);
  E->mbby_var_E = FUNC16(MotherBrainBody_9_SpawnTopMiddleLeftFalling);
  E->mbby_var_F = 32;
}

void MotherBrainBody_9_SpawnTopMiddleLeftFalling(void) {  // 0xA98A37
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0x40);
  if ((--E->mbby_var_F & 0x8000) != 0) {
    R18_ = 120;
    R20_ = 59;
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_MotherBrainTubeFalling_TopMiddleLeft, 0x3B);
    E->mbby_var_E = FUNC16(MotherBrainBody_10_ClearCeilingTubeColumn7);
  }
}

void MotherBrainBody_10_ClearCeilingTubeColumn7(void) {  // 0xA98A54
  SpawnHardcodedPlm(&unk_A98A58);
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0x40);
  E->mbby_var_E = FUNC16(MotherBrainBody_11_SpawnTopMiddleRightFalling);
  E->mbby_var_F = 32;
}

void MotherBrainBody_11_SpawnTopMiddleRightFalling(void) {  // 0xA98A69
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0x40);
  if ((--E->mbby_var_F & 0x8000) != 0) {
    R18_ = 136;
    R20_ = 59;
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_MotherBrainTubeFalling_TopMiddleRight, 0x3B);
    E->mbby_var_E = FUNC16(MotherBrainBody_12_ClearCeilingTubeColumn8);
  }
}

void MotherBrainBody_12_ClearCeilingTubeColumn8(void) {  // 0xA98A86
  SpawnHardcodedPlm(&unk_A98A8A);
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0x40);
  E->mbby_var_E = FUNC16(MotherBrainBody_13_SpawnTubeFallingEnemy3);
  E->mbby_var_F = 32;
}

void MotherBrainBody_13_SpawnTubeFallingEnemy3(void) {  // 0xA98A9B
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0x40);
  if ((--E->mbby_var_F & 0x8000) != 0) {
    SpawnEnemy(0xA9, FUNC16(EnemyProj_Init_0x8bde_SkreeDownLeft));
    E->mbby_var_E = FUNC16(MotherBrainBody_14_ClearBottomMiddleRightTube);
  }
}

void MotherBrainBody_14_ClearBottomMiddleRightTube(void) {  // 0xA98AAE
  SpawnHardcodedPlm(&unk_A98AB2);
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0x40);
  E->mbby_var_E = FUNC16(MotherBrainBody_15_SpawnTubeFallingEnemy4);
  E->mbby_var_F = 2;
}

void MotherBrainBody_15_SpawnTubeFallingEnemy4(void) {  // 0xA98AC3
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0x40);
  if ((--E->mbby_var_F & 0x8000) != 0) {
    SpawnEnemy(0xA9, addr_stru_A98B25);
    E->mbby_var_E = FUNC16(MotherBrainBody_16_ClearBottomMiddleTubes);
  }
}

void MotherBrainBody_16_ClearBottomMiddleTubes(void) {  // 0xA98AD6
  SpawnHardcodedPlm(&unk_A98ADA);
  Get_MotherBrainBody(0x40)->mbby_var_E = addr_locret_A98AE4;
}

void MotherBrainsTubesFalling_Init(void) {  // 0xA98B35
  Enemy_MotherBrainsTubesFalling *E = Get_MotherBrainsTubesFalling(cur_enemy_index);
  int v1 = E->mbtfg_parameter_1 >> 1;
  E->base.x_width = g_word_A98B5D[v1];
  E->base.y_height = g_word_A98B67[v1];
  E->mbtfg_var_B = g_word_A98B71[v1];
  E->mbtfg_var_D = 0;
  E->mbtfg_var_E = 0;
  E->mbtfg_var_C = 0;
  E->mbtfg_var_A = g_off_A98B7B[v1];
}

void CallMotherBrainsTubesFalling(uint32 ea, uint16 k) {
  switch (ea) {
  case fnMotherBrainsTubesFalling_Main_NonMain: MotherBrainsTubesFalling_Main_NonMain(k); return;
  case fnMotherBrainsTubesFalling_WaitToFall: MotherBrainsTubesFalling_WaitToFall(k); return;
  case fnMotherBrainsTubesFalling_Falling: MotherBrainsTubesFalling_Falling(k); return;
  default: Unreachable();
  }
}

void MotherBrainsTubesFalling_Main(uint16 k) {  // 0xA98B85
  Enemy_MotherBrainsTubesFalling *E = Get_MotherBrainsTubesFalling(k);
  CallMotherBrainsTubesFalling(E->mbtfg_var_A | 0xA90000, k);
}

void MotherBrainsTubesFalling_Main_NonMain(uint16 k) {  // 0xA98B88
  Enemy_MotherBrainsTubesFalling *E = Get_MotherBrainsTubesFalling(k);
  uint16 v2 = E->mbtfg_var_C + 6;
  E->mbtfg_var_C = v2;
  Enemy_IncreaseYpos(k, v2);
  if ((int16)(E->base.y_pos - E->mbtfg_var_B) < 0)
    MotherBrainsTubesFalling_HandleSmoke(k);
  else
    MotherBrainsTubesFalling_Explode(k);
}

void MotherBrainsTubesFalling_HandleSmoke(uint16 k) {  // 0xA98B9D
  Enemy_MotherBrainsTubesFalling *E = Get_MotherBrainsTubesFalling(k);
  if ((--E->mbtfg_var_D & 0x8000) != 0)
    MotherBrainsTubesFalling_SpawnSmoke(k);
}

void MotherBrainsTubesFalling_Explode(uint16 k) {  // 0xA98BA6
  Enemy_MotherBrainsTubesFalling *E = Get_MotherBrainsTubesFalling(k);
  E->base.properties |= kEnemyProps_Deleted;
  R18_ = E->base.x_pos;
  R20_ = E->base.y_pos;
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 3u);
  QueueSfx2_Max3(0x24u);
}

void MotherBrainsTubesFalling_WaitToFall(uint16 k) {  // 0xA98BCB
  Enemy_MotherBrainsTubesFalling *E = Get_MotherBrainsTubesFalling(k);
  if ((--E->mbtfg_parameter_2 & 0x8000) != 0) {
    E->mbtfg_var_A = FUNC16(MotherBrainsTubesFalling_Falling);
    MotherBrainsTubesFalling_Falling(k);
  }
}

void MotherBrainsTubesFalling_Falling(uint16 k) {  // 0xA98BD6
  Enemy_MotherBrainsTubesFalling *E = Get_MotherBrainsTubesFalling(k);
  uint16 v2 = E->mbtfg_var_C + 6;
  E->mbtfg_var_C = v2;
  Enemy_IncreaseYpos(k, v2);
  uint16 ypos = E->base.y_pos;
  if (!sign16(ypos - 244))
    E->base.properties |= kEnemyProps_Invisible;
  Enemy_MotherBrainsTubesFalling *E1 = Get_MotherBrainsTubesFalling(0x40);
  E1->base.y_pos = ypos - 56;
  if (sign16(E1->base.y_pos - 0xc4)) {
    MotherBrainsTubesFalling_HandleSmoke(k);
  } else {
    MotherBrain_Pal_EndScreenFlash();
    EnableEarthquakeAframes(0x19u);
    hdma_object_channels_bitmask[0] = 0;
    hdma_object_channels_bitmask[1] = 0;
    // BUG!
    //E->mbtfg_var_C = 0;
    E1->base.y_pos = 196;
    Enemy_MotherBrainsTubesFalling *E0 = Get_MotherBrainsTubesFalling(0);
    E0->base.x_pos = 59;
    E0->base.y_pos = 279;
    MotherBrain_SetupNeckForFakeAscent();
    E0->mbtfg_var_A = FUNC16(MotherBrainBody_FakeDeath_Ascent_0_DrawBG1Row23);
    MotherBrainsTubesFalling_Explode(cur_enemy_index);
  }
}

void MotherBrainsTubesFalling_SpawnSmoke(uint16 k) {  // 0xA98C36
  Enemy_MotherBrainsTubesFalling *E = Get_MotherBrainsTubesFalling(k);
  E->mbtfg_var_D = 8;
  uint16 v2 = ((uint8)E->mbtfg_var_E + 1) & 3;
  E->mbtfg_var_E = v2;
  R18_ = E->base.x_pos + g_word_A98C61[v2];
  R20_ = 208;
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 9u);
}

static const SpawnHardcodedPlmArgs unk_A98C8B = { 0x02, 0x02, 0xb67b };
static const SpawnHardcodedPlmArgs unk_A98C93 = { 0x02, 0x03, 0xb67f };
static const SpawnHardcodedPlmArgs unk_A98CA2 = { 0x02, 0x04, 0xb683 };
static const SpawnHardcodedPlmArgs unk_A98CAA = { 0x02, 0x05, 0xb687 };
static const SpawnHardcodedPlmArgs unk_A98CB9 = { 0x02, 0x06, 0xb68b };
static const SpawnHardcodedPlmArgs unk_A98CC1 = { 0x02, 0x07, 0xb68f };
static const SpawnHardcodedPlmArgs unk_A98CD0 = { 0x02, 0x08, 0xb693 };
static const SpawnHardcodedPlmArgs unk_A98CD8 = { 0x02, 0x09, 0xb697 };
static const SpawnHardcodedPlmArgs unk_A98CE7 = { 0x02, 0x0a, 0xb69b };
static const SpawnHardcodedPlmArgs unk_A98CEF = { 0x02, 0x0b, 0xb69f };
static const SpawnHardcodedPlmArgs unk_A98CFE = { 0x02, 0x0c, 0xb6a3 };
static const SpawnHardcodedPlmArgs unk_A98D06 = { 0x02, 0x0d, 0xb6a7 };

void MotherBrainBody_FakeDeath_Ascent_0_DrawBG1Row23(void) {  // 0xA98C87
  SpawnHardcodedPlm(&unk_A98C8B);
  SpawnHardcodedPlm(&unk_A98C93);
  Get_MotherBrainBody(0)->mbby_var_A = FUNC16(MotherBrainBody_FakeDeath_Ascent_1_DrawBG1Row45);
}

void MotherBrainBody_FakeDeath_Ascent_1_DrawBG1Row45(void) {  // 0xA98C9E
  SpawnHardcodedPlm(&unk_A98CA2);
  SpawnHardcodedPlm(&unk_A98CAA);
  Get_MotherBrainBody(0)->mbby_var_A = FUNC16(MotherBrainBody_FakeDeath_Ascent_2_DrawBG1Row67);
}

void MotherBrainBody_FakeDeath_Ascent_2_DrawBG1Row67(void) {  // 0xA98CB5
  SpawnHardcodedPlm(&unk_A98CB9);
  SpawnHardcodedPlm(&unk_A98CC1);
  Get_MotherBrainBody(0)->mbby_var_A = FUNC16(MotherBrainBody_FakeDeath_Ascent_3_DrawBG1Row89);
}

void MotherBrainBody_FakeDeath_Ascent_3_DrawBG1Row89(void) {  // 0xA98CCC
  SpawnHardcodedPlm(&unk_A98CD0);
  SpawnHardcodedPlm(&unk_A98CD8);
  Get_MotherBrainBody(0)->mbby_var_A = FUNC16(MotherBrainBody_FakeDeath_Ascent_4_DrawBG1RowAB);
}

void MotherBrainBody_FakeDeath_Ascent_4_DrawBG1RowAB(void) {  // 0xA98CE3
  SpawnHardcodedPlm(&unk_A98CE7);
  SpawnHardcodedPlm(&unk_A98CEF);
  Get_MotherBrainBody(0)->mbby_var_A = FUNC16(MotherBrainBody_FakeDeath_Ascent_5_DrawBG1RowCD);
}

void MotherBrainBody_FakeDeath_Ascent_5_DrawBG1RowCD(void) {  // 0xA98CFA
  SpawnHardcodedPlm(&unk_A98CFE);
  SpawnHardcodedPlm(&unk_A98D06);
  Get_MotherBrainBody(0)->mbby_var_A = FUNC16(MotherBrainBody_FakeDeath_Ascent_6_SetupPhase2Gfx);
}

void MotherBrainBody_FakeDeath_Ascent_6_SetupPhase2Gfx(void) {  // 0xA98D11
  *(uint16 *)&layer2_scroll_x = 257;
  *(uint16 *)&reg_BG2SC &= 0xFFFCu;
  WriteColorsToPalette(0x142, 0xa9, addr_kMotherBrainPalette_1 + 2, 0xF);
  WriteColorsToPalette(0x162, 0xa9, addr_kMotherBrainPalette_0 + 2, 0xF);
  Get_MotherBrain(0)->mbn_var_A = FUNC16(MotherBrainBody_FakeDeath_Ascent_7_SetupPhase2Brain);
  nmi_flag_bg2_enemy_vram_transfer = 1;
  Get_MotherBrain(0x40)->mbn_var_02 = 1;
}

void MotherBrainBody_FakeDeath_Ascent_7_SetupPhase2Brain(void) {  // 0xA98D49
  fx_layer_blending_config_a = 52;
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0x40);
  E->mbby_var_A = FUNC16(MotherBrainsBrain_SetupBrainAndNeckToDraw);
  Enemy_MotherBrainBody *E0 = Get_MotherBrainBody(0);
  E0->base.properties &= ~kEnemyProps_Tangible;
  E->base.properties &= ~0x400u;
  E->base.health = 18000;
  E0->mbby_var_A = FUNC16(MotherBrainBody_FakeDeath_Ascent_8_Pause);
  E0->mbby_var_F = 128;
  MotherBrainBody_FakeDeath_Ascent_8_Pause();
}

void MotherBrainBody_FakeDeath_Ascent_8_Pause(void) {  // 0xA98D79
  Enemy_MotherBrainBody *E = Get_MotherBrainBody(0);
  if ((--E->mbby_var_F & 0x8000) != 0) {
    E->mbby_var_A = FUNC16(MotherBrainBody_FakeDeath_Ascent_9_PrepareRise);
    E->mbby_var_F = 32;
    MotherBrainBody_FakeDeath_Ascent_9_PrepareRise();
  }
}

void MotherBrainBody_FakeDeath_Ascent_9_PrepareRise(void) {  // 0xA98D8B
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    E->mbn_var_09 = MotherBrainRisingHdmaObject();
    Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
    E1->base.properties |= 0x100u;
    MotherBrain_SetBrainInstrs(addr_stru_A99C21);
    E->mbn_var_A = FUNC16(MotherBrainBody_FakeDeath_Ascent_10_LoadLegTiles);
    E->mbn_var_F = 256;
    MotherBrainBody_FakeDeath_Ascent_10_LoadLegTiles();
  }
}

void MotherBrainBody_FakeDeath_Ascent_10_LoadLegTiles(void) {  // 0xA98DB4
  if (ProcessSpriteTilesTransfers(0xa9, addr_stru_A98F8F)) {
    Get_MotherBrainBody(0)->mbby_var_A = FUNC16(MotherBrainBody_FakeDeath_Ascent_11_ContinuePause);
    MotherBrainBody_FakeDeath_Ascent_11_ContinuePause();
  }
}

void MotherBrainBody_FakeDeath_Ascent_11_ContinuePause(void) {  // 0xA98DC3
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    E->base.x_pos = 59;
    E->base.y_pos = 279;
    reg_BG2HOFS = -27;
    reg_BG2VOFS = -217;
    E->mbn_var_04 = 7;
    E->mbn_var_A = FUNC16(MotherBrainBody_FakeDeath_Ascent_12_StartMusic);
  }
}

void MotherBrainBody_FakeDeath_Ascent_12_StartMusic(void) {  // 0xA98DEC
  MotherBrain_SetBodyInstrs(addr_kMotherBrain_Ilist_9A02);
  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  E1->base.instruction_timer = 1;
  Enemy_MotherBrain *E0 = Get_MotherBrain(0);
  E0->base.properties &= ~kEnemyProps_Invisible;
  E0->base.x_pos = 59;
  E0->base.y_pos = 279;
  reg_BG2HOFS = -27;
  reg_BG2VOFS = -217;
  QueueMusic_Delayed8(5u);
  earthquake_type = 2;
  earthquake_timer = 256;
  E1->mbn_var_34 = 80;
  E1->mbn_var_31 = 1;
  E1->mbn_var_32 = 8;
  E1->mbn_var_33 = 6;
  E0->mbn_var_A = FUNC16(MotherBrainBody_FakeDeath_Ascent_13_Raise);
}

void MotherBrainBody_FakeDeath_Ascent_13_Raise(void) {  // 0xA98E4D
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  uint16 v1;
  if ((nmi_frame_counter_word & 3) != 0
      || (MotherBrain_SpawnDustCloudsForAscent(),
          R18_ = 2,
          reg_BG2VOFS += 2,
          v1 = E->base.y_pos - 2,
          E->base.y_pos = v1,
          v1 >= 0xBDu)) {
  } else {
    enemy_bg2_tilemap_size = 320;
    E->base.y_pos = 188;
    earthquake_timer = 0;
    hdma_object_channels_bitmask[E->mbn_var_09 >> 1] = 0;
    MotherBrain_SetBodyInstrs(addr_kMotherBrain_Ilist_99AA);
    E->mbn_var_A = FUNC16(MotherBrainBody_FakeDeath_Ascent_14_WaitForUncouching);
    MotherBrainBody_FakeDeath_Ascent_14_WaitForUncouching();
  }
}

void MotherBrainBody_FakeDeath_Ascent_14_WaitForUncouching(void) {  // 0xA98E95
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if (!E->mbn_var_02) {
    E->mbn_var_37 = 0;
    E->mbn_var_A = FUNC16(MotherBrainBody_FakeDeath_Ascent_15_TransitionFromGrey);
    E->mbn_var_F = 0;
  }
}

void MotherBrainBody_FakeDeath_Ascent_15_TransitionFromGrey(void) {  // 0xA98EAA
  Enemy_MotherBrain *E0 = Get_MotherBrain(0);
  if ((--E0->mbn_var_F & 0x8000) != 0) {
    E0->mbn_var_F = 4;
    uint16 v3 = E0->mbn_var_37 + 1;
    E0->mbn_var_37 = v3;
    if (MotherBrain_FadeFromGray_FakeDeath(v3 - 1) & 1) {
      Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
      E1->mbn_var_10 = 1;
      E0->mbn_var_00 = 2;
      E1->mbn_var_12 = 1;
      E1->mbn_var_32 = 6;
      E1->mbn_var_33 = 6;
      E1->mbn_var_34 = 1280;
      E0->mbn_var_A = FUNC16(MotherBrainBody_2ndphase_16_ShakeHeadMenacingly);
      E0->mbn_var_F = 23;
    }
  }
}

void MotherBrainBody_2ndphase_16_ShakeHeadMenacingly(void) {  // 0xA98EF5
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    MotherBrain_SetBrainInstrs(addr_kMotherBrain_Ilist_9B7F);
    E->mbn_var_A = FUNC16(MotherBrainBody_2ndphase_17_BringHeadBackUp);
    Get_MotherBrain(0x40)->mbn_var_34 = 64;
    E->mbn_var_F = 256;
    MotherBrainBody_2ndphase_17_BringHeadBackUp();
  }
}

void MotherBrainBody_2ndphase_17_BringHeadBackUp(void) {  // 0xA98F14
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
    E1->mbn_var_32 = 2;
    E1->mbn_var_33 = 4;
    E->mbn_var_A = FUNC16(MotherBrainBody_2ndphase_18_FinishStretching);
    E->mbn_var_F = 64;
    MotherBrainBody_2ndphase_18_FinishStretching();
  }
}

void MotherBrainBody_2ndphase_18_FinishStretching(void) {  // 0xA98F33
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    Get_MotherBrain(0x40)->mbn_var_14 = 1;
    E->mbn_var_A = FUNC16(MotherBrain_Body_Phase2_Thinking);
  }
}

void MotherBrain_SpawnDustCloudsForAscent(void) {  // 0xA98F46
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  if ((--E->mbn_var_F & 0x8000) != 0)
    E->mbn_var_F = 7;
  R18_ = g_word_A98F7F[E->mbn_var_F];
  R20_ = 212;
  SpawnEnemyProjectileWithRoomGfx(
    addr_kEproj_DustCloudExplosion,
    g_byte_A98F7D[(uint16)(random_number & 0x100) >> 8]);
  QueueSfx2_Max3(0x29u);
}

void MotherBrain_SetupNeckForFakeAscent(void) {  // 0xA9903F
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  E->mbn_var_24 = 2;
  E->mbn_var_27 = 10;
  E->mbn_var_2D = 10;
  E->mbn_var_2A = 20;
  E->mbn_var_30 = 20;
  E->mbn_var_20 = 18432;
  E->mbn_var_21 = 20480;
  E->mbn_var_34 = 256;
}

void MotherBrain_HandleNeckLower(void) {  // 0xA99072
  uint16 mbn_var_32 = Get_MotherBrain(0x40)->mbn_var_32;
  if (mbn_var_32) {
    switch (mbn_var_32) {
    case 2u:
      MotherBrain_HandleNeckLower_2_BobDown();
      break;
    case 4u:
      MotherBrain_HandleNeckLower_4_BobUp();
      break;
    case 6u:
      MotherBrain_HandleNeckLower_6_Lower();
      break;
    case 8u:
      MotherBrain_HandleNeckLower_8_Raise();
      break;
    default:
      Unreachable();
      while (1)
        ;
    }
  } else {
    MotherBrain_HandleNeckLower_0();
  }
}

void MotherBrain_HandleNeckLower_0(void) {  // 0xA99084
  ;
}

void MotherBrain_HandleNeckLower_2_BobDown(void) {  // 0xA99085
  int16 v1;

  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  v1 = E->mbn_var_20 - E->mbn_var_34;
  if ((uint16)v1 < 0x2800) {
    E->mbn_var_32 = 4;
    v1 = 10240;
  }
  E->mbn_var_20 = v1;
}

void MotherBrain_HandleNeckLower_4_BobUp(void) {  // 0xA990A2
  int16 v1;

  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  if (sign16(E->base.y_pos - 60)) {
    E->mbn_var_32 = 2;
  } else {
    v1 = E->mbn_var_34 + E->mbn_var_20;
    if ((uint16)v1 >= 0x9000) {
      E->mbn_var_32 = 2;
      v1 = -28672;
    }
    E->mbn_var_20 = v1;
  }
}

void MotherBrain_HandleNeckLower_6_Lower(void) {  // 0xA990CF
  int16 v1;

  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  v1 = E->mbn_var_20 - E->mbn_var_34;
  if ((uint16)v1 < 0x3000) {
    E->mbn_var_32 = 0;
    v1 = 12288;
  }
  E->mbn_var_20 = v1;
}

void MotherBrain_HandleNeckLower_8_Raise(void) {  // 0xA990EC
  int16 v1;

  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  v1 = E->mbn_var_34 + E->mbn_var_20;
  if ((uint16)v1 >= 0x9000) {
    E->mbn_var_32 = 0;
    v1 = -28672;
  }
  E->mbn_var_20 = v1;
}

void MotherBrain_HandleNeckUpper(void) {  // 0xA99109
  int16 v2;
  int16 v6;
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  uint16 mbn_var_33 = E->mbn_var_33;
  if (mbn_var_33) {
    switch (mbn_var_33) {
    case 2u:
      if ((int16)(E->base.y_pos + 4 - samus_y_pos) < 0) {
        v2 = E->mbn_var_21 - E->mbn_var_34;
        if ((uint16)v2 < 0x2000) {
          E->mbn_var_33 = 4;
          v2 = 0x2000;
        }
        E->mbn_var_21 = v2;
      } else {
        E->mbn_var_32 = 4;
        E->mbn_var_33 = 4;
      }
      break;
    case 4u: {
      R18_ = E->mbn_var_20 + 2048;
      uint16 v4 = E->mbn_var_34 + E->mbn_var_21;
      if (v4 >= R18_) {
        E->mbn_var_33 = 2;
        v4 = R18_;
      }
      E->mbn_var_21 = v4;
      break;
    }
    case 6u: {
      v6 = E->mbn_var_21 - E->mbn_var_34;
      if ((uint16)v6 < 0x2000) {
        E->mbn_var_33 = 0;
        v6 = 0x2000;
      }
      E->mbn_var_21 = v6;
      break;
    }
    case 8u: {
      R18_ = E->mbn_var_20 + 2048;
      uint16 v8 = E->mbn_var_34 + E->mbn_var_21;
      if (v8 >= R18_) {
        E->mbn_var_33 = 0;
        v8 = R18_;
      }
      E->mbn_var_21 = v8;
      break;
    }
    default:
      Unreachable();
      while (1)
        ;
    }
  }
}

void MotherBrain_HandleNeck(void) {  // 0xA991B8
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->mbn_var_0A = E->base.x_pos - 80;
  E->mbn_var_0B = E->base.y_pos + 46;

  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  if (E1->mbn_var_31) {
    MotherBrain_HandleNeckLower();
    MotherBrain_HandleNeckUpper();
  }
  R18_ = HIBYTE(E1->mbn_var_20);
  E1->mbn_var_22 = E->mbn_var_0A + ComputeSinMult(E1->mbn_var_24) + 112;
  E1->mbn_var_23 = E->mbn_var_0B + ComputeCosMult(E1->mbn_var_24) - 96;
  E1->mbn_var_25 = E->mbn_var_0A + ComputeSinMult(E1->mbn_var_27) + 112;
  E1->mbn_var_26 = E->mbn_var_0B + ComputeCosMult(E1->mbn_var_27) - 96;
  E1->mbn_var_28 = E->mbn_var_0A + ComputeSinMult(E1->mbn_var_2A) + 112;
  E1->mbn_var_29 = E->mbn_var_0B + ComputeCosMult(E1->mbn_var_2A) - 96;
  R18_ = HIBYTE(E1->mbn_var_21);
  E1->mbn_var_2B = E1->mbn_var_28 + ComputeSinMult(E1->mbn_var_2D);
  E1->mbn_var_2C = E1->mbn_var_29 + ComputeCosMult(E1->mbn_var_2D);
  E1->mbn_var_2E = E1->mbn_var_28 + ComputeSinMult(E1->mbn_var_30);
  E1->mbn_var_2F = E1->mbn_var_29 + ComputeCosMult(E1->mbn_var_30);
}


uint16 CallMotherBrainInstr(uint32 ea, uint16 k) {
  switch (ea) {
  case fnMotherBrain_Instr_Goto: return MotherBrain_Instr_Goto(k);
  case fnMotherBrain_Instr_EnableNeckMovementGoto: return MotherBrain_Instr_EnableNeckMovementGoto(k);
  case fnMotherBrain_Instr_DisableNeckMovement: return MotherBrain_Instr_DisableNeckMovement(k);
  case fnMotherBrain_Instr_QueueSfx2: return MotherBrain_Instr_QueueSfx2(k);
  case fnMotherBrain_Instr_QueueSfx3: return MotherBrain_Instr_QueueSfx3(k);
  case fnMotherBrain_Instr_SpawnDroolEproj: return MotherBrain_Instr_SpawnDroolEproj(k);
  case fnMotherBrain_Instr_SpawnPurpleBreath: return MotherBrain_Instr_SpawnPurpleBreath(k);
  case fnMotherBrain_Instr_SetMainShakeTimer50: return MotherBrain_Instr_SetMainShakeTimer50(k);
  case fnMotherBrain_Instr_GotoEitherOr: return MotherBrain_Instr_GotoEitherOr(k);
  case fnMotherBrain_Instr_MaybeGoto: return MotherBrain_Instr_MaybeGoto(k);
  case fnMotherBrain_Instr_MaybeGoto2: return MotherBrain_Instr_MaybeGoto2(k);
  case fnMotherBrain_Instr_Goto2: return MotherBrain_Instr_Goto2(k);
  case fnMotherBrain_Instr_QueueShitroidAttackSfx: return MotherBrain_Instr_QueueShitroidAttackSfx(k);
  case fnMotherBrain_Instr_SpawnBlueRingEproj: return MotherBrain_Instr_SpawnBlueRingEproj(k);
  case fnMotherBrain_Instr_AimRingsAtShitroid: return MotherBrain_Instr_AimRingsAtShitroid(k);
  case fnMotherBrain_Instr_AimRingsAtSamus: return MotherBrain_Instr_AimRingsAtSamus(k);
  case fnMotherBrain_Instr_IncrShitroidAttackCtr: return MotherBrain_Instr_IncrShitroidAttackCtr(k);
  case fnMotherBrain_Instr_SetShitroidAttackCtr0: return MotherBrain_Instr_SetShitroidAttackCtr0(k);
  case fnMotherBrain_Instr_SpawnBombEproj: return MotherBrain_Instr_SpawnBombEproj(k);
  case fnMotherBrain_Instr_SpawnLaserEproj: return MotherBrain_Instr_SpawnLaserEproj(k);
  case fnMotherBrain_Instr_SpawnRainbowEproj: return MotherBrain_Instr_SpawnRainbowEproj(k);
  case fnMotherBrain_Instr_SetupFxForRainbowBeam: return MotherBrain_Instr_SetupFxForRainbowBeam(k);
  default: return Unreachable();
  }
}

int MotherBrain_Func_1_DoubleRet(void) {
  int16 v1;
  int16 mbn_var_21;
  int16 v5;

  Enemy_MotherBrain *E = Get_MotherBrain(0);

  if (time_is_frozen_flag) {
    mbn_var_21 = E->mbn_var_21;
    if (mbn_var_21 >= 0)
      return -1;
    return *((uint16 *)RomPtr_A9(mbn_var_21) + 1);
  } else {
    v1 = E->mbn_var_21;
    if (v1 >= 0)
      return -1;
    uint16 v4 = E->mbn_var_21;
    v5 = *(uint16 *)RomPtr_A9(v1);
    if (v5 < 0)
      goto LABEL_10;
    if ((int16)(v5 - E->mbn_var_20) >= 0) {
      ++E->mbn_var_20;
      return *((uint16 *)RomPtr_A9(v4) + 1);
    } else {
      for (v4 += 4; ; ) {
        v5 = *(uint16 *)RomPtr_A9(v4);
        if (v5 >= 0)
          break;
LABEL_10:
        R0_.addr = v5;
        v4 = CallMotherBrainInstr((uint16)v5 | 0xA90000, v4 + 2);
      }
      E->mbn_var_20 = 1;
      E->mbn_var_21 = v4;
      return *((uint16 *)RomPtr_A9(v4) + 1);
    }
  }
}

void MotherBrain_DrawNeck(void) {  // 0xA99303
  if ((Get_MotherBrain(0)->base.properties & kEnemyProps_Invisible) == 0) {
    Enemy_MotherBrain *E = Get_MotherBrain(0x40);
    R18_ = E->mbn_var_2E;
    R20_ = E->mbn_var_2F;
    MotherBrain_DrawNeckSegment();
    R18_ = E->mbn_var_2B;
    R20_ = E->mbn_var_2C;
    MotherBrain_DrawNeckSegment();
    R18_ = E->mbn_var_28;
    R20_ = E->mbn_var_29;
    MotherBrain_DrawNeckSegment();
    R18_ = E->mbn_var_25;
    R20_ = E->mbn_var_26;
    MotherBrain_DrawNeckSegment();
    R18_ = E->mbn_var_22;
    R20_ = E->mbn_var_23;
    MotherBrain_DrawNeckSegment();
  }
}

void MotherBrain_DrawBrain(void) {  // 0xA99357
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  if (E->mbn_var_14 && !E->mbn_var_15 && (random_number & 0x8000) == 0)
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_MotherBrainPurpleBreathSmall, random_number);
  int tt = MotherBrain_Func_1_DoubleRet();
  if (tt < 0)
    return;
  uint16 v2 = tt;
  uint16 mbn_var_0D = Get_MotherBrain(0)->mbn_var_0D;
  if (E->base.flash_timer & 1)
    mbn_var_0D = 0;
  R22_ = mbn_var_0D;
  uint16 mbn_var_00 = E->mbn_var_00;
  uint16 flash_timer;
  if (mbn_var_00) {
    flash_timer = mbn_var_00 - 1;
    E->mbn_var_00 = flash_timer;
  } else {
    flash_timer = E->base.flash_timer;
    if (!flash_timer)
      flash_timer = E->base.shake_timer;
  }
  int v6 = (uint8)(flash_timer & 6) >> 1;
  R18_ = E->base.x_pos + g_word_A993BB[v6];
  if ((int16)(R18_ + 32 - layer1_x_pos) >= 0) {
    R20_ = E->base.y_pos + g_word_A993C3[v6];
    MotherBrain_AddSpritemapToOam(v2);
  }
}

void MotherBrain_DrawNeckSegment(void) {  // 0xA993CB
  int v0 = (uint8)(Get_MotherBrain(0x40)->base.flash_timer & 6) >> 1;
  R18_ += g_word_A993BB[v0];
  R20_ += g_word_A993C3[v0];
  R22_ = Get_MotherBrain(0)->mbn_var_0C;
  MotherBrain_AddSpritemapToOam(addr_kMotherBrain_Sprmap_A694);
}

void MotherBrain_AddSpritemapToOam(uint16 j) {  // 0xA993EE
  int16 *v5;
  int16 v6;
  int16 v7;
  char v8; // t0
  int16 v10;
  int16 v12;
  OamEnt *v13;
  int16 v15;
  int16 v17;

  uint16 *v1 = (uint16 *)RomPtr_A9(j);
  uint16 v2 = j + 2;
  R24_ = *v1;
  uint16 v3 = oam_next_ptr;
  do {
    uint8 *v4 = RomPtr_A9(v2);
    v5 = (int16 *)v4;
    v6 = v4[2] << 8;
    if ((v4[2] & 0x80) != 0)
      v6 |= 0xFFu;
    v8 = v6;
    LOBYTE(v7) = HIBYTE(v6);
    HIBYTE(v7) = v8;
    uint16 v9 = R20_ + v7;
    bool v11 = v9 < layer1_y_pos;
    v10 = v9 - layer1_y_pos;
    v11 = !v11;
    if (v10 >= 0) {
      R26_ = v10;
      v12 = R18_ + v11 + *(uint16 *)v4 - layer1_x_pos;
      v13 = gOamEnt(v3);
      *(uint16 *)&v13->xcoord = v12;
      if ((v12 & 0x100) != 0) {
        int v14 = v3 >> 1;
        R28_ = kOamExtra_Address_And_X8Large[v14];
        v15 = kOamExtra_X8Small_And_Large[v14] | *(uint16 *)RomPtr_RAM(R28_);
        *(uint16 *)RomPtr_RAM(R28_) = v15;
      }
      if (*v5 < 0) {
        int v16 = v3 >> 1;
        R28_ = kOamExtra_Address_And_X8Large[v16];
        v17 = kOamExtra_X8Small_And_Large[v16 + 1] | *(uint16 *)RomPtr_RAM(R28_);
        *(uint16 *)RomPtr_RAM(R28_) = v17;
      }
      *(uint16 *)&v13->ycoord = R26_;
      *(uint16 *)&v13->charnum = R22_ | *(int16 *)((char *)v5 + 3);
      v3 = (v3 + 4) & 0x1FF;
    }
    v2 += 5;
    --R24_;
  } while (R24_);
  oam_next_ptr = v3;
}

void MotherBrain_CalculateRainbowBeamHdma(void) {  // 0xA99466
  MotherBrain_CalcHdma();
}

void MotherBrain_MoveBodyDownScrollLeft(uint16 k, uint16 a) {  // 0xA99552
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->base.y_pos += a;
  reg_BG2VOFS -= a;
  R20_ = k + 34;
  reg_BG2HOFS = k + 34 - E->base.x_pos;
}

void MotherBrain_MoveBodyDown(uint16 a) {  // 0xA99579
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->base.y_pos += a;
  reg_BG2VOFS -= a;
  reg_BG2HOFS = 34 - E->base.x_pos;
}

void MotherBrain_FootstepEffect(void) {  // 0xA99599
  earthquake_type = 1;
  earthquake_timer = 4;
  if (Get_MotherBrain(0)->mbn_var_00 == 3)
    printf("Write to rom!\n");
  //    word_80914D = 22;
}

uint16 MotherBrain_Instr_MoveBodyUp10Left4(uint16 k, uint16 j) {  // 0xA995B6
  MotherBrain_MoveBodyDownScrollLeft(4u, 0xFFF6);
  return j;
}

uint16 MotherBrain_Instr_MoveBodyUp16Left4(uint16 k, uint16 j) {  // 0xA995C0
  MotherBrain_MoveBodyDownScrollLeft(4u, 0xFFF0);
  return j;
}

uint16 MotherBrain_Instr_MoveBodyUp12Right2(uint16 k, uint16 j) {  // 0xA995CA
  MotherBrain_MoveBodyDownScrollLeft(0xFFFE, 0xFFF4);
  return j;
}

uint16 MotherBrain_Instr_MoveDown12Left4(uint16 k, uint16 j) {  // 0xA995DE
  MotherBrain_MoveBodyDownScrollLeft(4u, 0xC);
  return j;
}

uint16 MotherBrain_Instr_MoveDown16Right2(uint16 k, uint16 j) {  // 0xA995E8
  MotherBrain_MoveBodyDownScrollLeft(0xFFFE, 0x10);
  return j;
}

uint16 MotherBrain_Instr_MoveDown10Right2(uint16 k, uint16 j) {  // 0xA995F2
  MotherBrain_MoveBodyDownScrollLeft(0xFFFE, 0xA);
  return j;
}

uint16 MotherBrain_Instr_MoveUp2Right1(uint16 k, uint16 j) {  // 0xA995FC
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  ++E->base.x_pos;
  MotherBrain_MoveBodyDown(0xFFFEu);
  return j;
}

uint16 MotherBrain_Instr_MoveRight2(uint16 k, uint16 j) {  // 0xA9960C
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->base.x_pos += 2;
  MotherBrain_MoveBodyDown(0);
  return j;
}

uint16 MotherBrain_Instr_MoveUp1(uint16 k, uint16 j) {  // 0xA9961C
  MotherBrain_MoveBodyDown(1u);
  return j;
}

uint16 MotherBrain_Instr_MoveUp1Right3_Sfx(uint16 k, uint16 j) {  // 0xA99622
  MotherBrain_FootstepEffect();
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->base.x_pos += 3;
  MotherBrain_MoveBodyDown(1u);
  return j;
}

uint16 MotherBrain_Instr_Down2Right15(uint16 k, uint16 j) {  // 0xA99638
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->base.x_pos += 15;
  MotherBrain_MoveBodyDown(0xFFFEu);
  return j;
}

uint16 MotherBrain_Instr_Down4Right6(uint16 k, uint16 j) {  // 0xA99648
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->base.x_pos += 6;
  MotherBrain_MoveBodyDown(0xFFFCu);
  return j;
}

uint16 MotherBrain_Instr_Up4Left2(uint16 k, uint16 j) {  // 0xA99658
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->base.x_pos -= 2;
  MotherBrain_MoveBodyDown(4u);
  return j;
}

uint16 MotherBrain_Instr_Up2Left1_Sfx(uint16 k, uint16 j) {  // 0xA99668
  MotherBrain_FootstepEffect();
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  --E->base.x_pos;
  MotherBrain_MoveBodyDown(2u);
  return j;
}

uint16 MotherBrain_Instr_Up2Left1_Sfx2(uint16 k, uint16 j) {  // 0xA9967E
  MotherBrain_FootstepEffect();
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  --E->base.x_pos;
  MotherBrain_MoveBodyDown(2u);
  return j;
}

uint16 MotherBrain_Instr_MoveLeft2(uint16 k, uint16 j) {  // 0xA99694
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->base.x_pos -= 2;
  MotherBrain_MoveBodyDown(0);
  return j;
}

uint16 MotherBrain_Instr_MoveDown1(uint16 k, uint16 j) {  // 0xA996A4
  MotherBrain_MoveBodyDown(0xFFFFu);
  return j;
}

uint16 MotherBrain_Instr_MoveDown1Left3(uint16 k, uint16 j) {  // 0xA996AA
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->base.x_pos -= 3;
  MotherBrain_MoveBodyDown(0xFFFFu);
  return j;
}

uint16 MotherBrain_Instr_MoveUp2Left15_Sfx(uint16 k, uint16 j) {  // 0xA996BA
  MotherBrain_FootstepEffect();
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->base.x_pos -= 15;
  MotherBrain_MoveBodyDown(2u);
  return j;
}

uint16 MotherBrain_Instr_MoveUp4Left6(uint16 k, uint16 j) {  // 0xA996D0
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->base.x_pos -= 6;
  MotherBrain_MoveBodyDown(4u);
  return j;
}

uint16 MotherBrain_Instr_MoveDown4Right2(uint16 k, uint16 j) {  // 0xA996E0
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->base.x_pos += 2;
  MotherBrain_MoveBodyDown(0xFFFCu);
  return j;
}

uint16 MotherBrain_Instr_MoveDown2Right1(uint16 k, uint16 j) {  // 0xA996F0
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  ++E->base.x_pos;
  MotherBrain_MoveBodyDown(0xFFFEu);
  return j;
}

uint16 MotherBrain_Instr_SetPose_Standing(uint16 k, uint16 j) {  // 0xA99700
  Get_MotherBrain(0)->mbn_var_02 = 0;
  return j;
}

uint16 MotherBrain_Instr_SetPose_Walking(uint16 k, uint16 j) {  // 0xA99708
  Get_MotherBrain(0)->mbn_var_02 = 1;
  return j;
}

uint16 MotherBrain_Instr_SetPose_Crouched(uint16 k, uint16 j) {  // 0xA99710
  Get_MotherBrain(0)->mbn_var_02 = 3;
  return j;
}

uint16 MotherBrain_Instr_SetPose_CrouchedTrans(uint16 k, uint16 j) {  // 0xA99718
  Get_MotherBrain(0)->mbn_var_02 = 2;
  return j;
}

uint16 MotherBrain_Instr_SetPose_DeathBeamMode(uint16 k, uint16 j) {  // 0xA99720
  Get_MotherBrain(0)->mbn_var_02 = 4;
  return j;
}

uint16 MotherBrain_Instr_SetPose_LeaningDown(uint16 k, uint16 j) {  // 0xA99728
  Get_MotherBrain(0)->mbn_var_02 = 6;
  return j;
}

uint16 MotherBrain_Instr_SpawnEprojToOffset(uint16 k, uint16 j) {  // 0xA99AC8
  uint8 *v2 = RomPtr_A9(j);
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  R18_ = E->base.x_pos + *(uint16 *)v2;
  R20_ = E->base.y_pos + *((uint16 *)v2 + 1);
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, *((uint16 *)v2 + 2));
  return j + 6;
}

uint16 MotherBrain_Instr_SpawnDeathBeamEproj(uint16 k, uint16 j) {  // 0xA99AEF
  uint16 v2;
  printf("A undefined\n"); v2 = 0;
  QueueSfx2_Max6(0x63u);
  SpawnEnemyProjectileWithGfx(v2, 0x40, addr_kEproj_MotherBrainDeathBeamCharging);
  return j;
}

uint16 MotherBrain_Instr_IncrBeamAttackPhase(uint16 k, uint16 j) {  // 0xA99B05
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  ++E->mbn_var_17;
  return j;
}

uint16 MotherBrain_Instr_Goto(uint16 k) {  // 0xA99B0F
  return *(uint16 *)RomPtr_A9(k);
}

uint16 MotherBrain_Instr_EnableNeckMovementGoto(uint16 k) {  // 0xA99B14
  Get_MotherBrain(0x40)->mbn_var_31 = 1;
  return *(uint16 *)RomPtr_A9(k);
}

uint16 MotherBrain_Instr_DisableNeckMovement(uint16 k) {  // 0xA99B20
  Get_MotherBrain(0x40)->mbn_var_31 = 0;
  return k;
}

uint16 MotherBrain_Instr_QueueSfx2(uint16 k) {  // 0xA99B28
  uint16 *v2 = (uint16 *)RomPtr_A9(k);
  QueueSfx2_Max6(*v2);
  return k + 2;
}

uint16 MotherBrain_Instr_QueueSfx3(uint16 k) {  // 0xA99B32
  uint16 *v2 = (uint16 *)RomPtr_A9(k);
  QueueSfx3_Max6(*v2);
  return k + 2;
}

uint16 MotherBrain_Instr_SpawnDroolEproj(uint16 k) {  // 0xA99B3C
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  if (E->mbn_var_12) {
    uint16 v2 = E->mbn_var_13 + 1;
    if (!sign16(E->mbn_var_13 - 5))
      v2 = 0;
    E->mbn_var_13 = v2;
    uint16 v3 = addr_kEproj_MotherBrainDrool;
    if (!sign16(E->mbn_var_34 - 128))
      v3 = addr_kEproj_MotherBrainDyingDrool;
    SpawnEnemyProjectileWithRoomGfx(v3, E->mbn_var_13);
  }
  return k;
}

uint16 MotherBrain_Instr_SpawnPurpleBreath(uint16 k) {  // 0xA99B6D
  uint16 v2;
  printf("A undefined\n"); v2 = 0;
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_MotherBrainPurpleBreathBig, v2);
  return k;
}

uint16 MotherBrain_Instr_SetMainShakeTimer50(uint16 k) {  // 0xA99B77
  Get_MotherBrain(0x40)->mbn_var_00 = 50;
  return k;
}

uint16 MotherBrain_Instr_GotoEitherOr(uint16 k) {  // 0xA99C65
  uint16 result = addr_stru_A99C5F;
  if ((random_number & 0xFFFu) >= 0xFE0)
    return addr_stru_A99C47;
  return result;
}

uint16 MotherBrain_Instr_MaybeGoto(uint16 k) {  // 0xA99CAD
  if (random_number < 0xF000)
    return addr_stru_A99C9F;
  return k;
}

uint16 MotherBrain_Instr_MaybeGoto2(uint16 k) {  // 0xA99D0D
  if ((random_number & 0xFFFu) < 0xEC0)
    return MotherBrain_Instr_Goto2(k);
  return k;
}

uint16 MotherBrain_Instr_QueueShitroidAttackSfx(uint16 k) {  // 0xA99DF7
  if (Get_MotherBrain(0)->mbn_var_13 != 11)
    QueueSfx2_Max6(g_word_A99E0F[0]);
  return k;
}

uint16 MotherBrain_Instr_Goto2(uint16 k) {  // 0xA99D21
  return addr_stru_A99CD1;
}

uint16 MotherBrain_Instr_SpawnBlueRingEproj(uint16 k) {  // 0xA99E29
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_MotherBrainBlueRingLasers, E->mbn_var_1A);
  return k;
}

uint16 MotherBrain_Instr_AimRingsAtShitroid(uint16 k) {  // 0xA99E37
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  Enemy_MotherBrain *E1 = Get_MotherBrain(E->mbn_var_0A);
  R18_ = E1->base.x_pos - E->base.x_pos - 10;
  R20_ = E1->base.y_pos - E->base.y_pos - 16;
  MotherBrain_Instr_AimRings();
  return k;
}

uint16 MotherBrain_Instr_AimRingsAtSamus(uint16 k) {  // 0xA99E5B
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  R18_ = samus_x_pos - E->base.x_pos - 10;
  R20_ = samus_y_pos - E->base.y_pos - 16;
  MotherBrain_Instr_AimRings();
  return k;
}

void MotherBrain_Instr_AimRings(void) {  // 0xA99E77
  uint16 v0 = (uint8)-(int8)(CalculateAngleFromXY() + 0x80);
  if (!sign8(v0 - 16)) {
    if ((uint8)v0 < 0x48u)
      goto LABEL_6;
LABEL_5:
    LOBYTE(v0) = 72;
    goto LABEL_6;
  }
  if (sign8(v0 + 64))
    goto LABEL_5;
  LOBYTE(v0) = 16;
LABEL_6:
  Get_MotherBrain(0)->mbn_var_1A = v0;
}

uint16 MotherBrain_Instr_IncrShitroidAttackCtr(uint16 k) {  // 0xA99EA3
  int16 v3;

  Enemy_MotherBrain *E = Get_MotherBrain(0);
  v3 = E->mbn_var_13 + 1;
  if ((uint16)v3 >= 0xCu)
    v3 = 12;
  E->mbn_var_13 = v3;
  return k;
}

uint16 MotherBrain_Instr_SetShitroidAttackCtr0(uint16 k) {  // 0xA99EB5
  Get_MotherBrain(0)->mbn_var_13 = 0;
  return k;
}

uint16 MotherBrain_Instr_SpawnBombEproj(uint16 k) {  // 0xA99EBD
  uint16 *v2 = (uint16 *)RomPtr_A9(k);
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_MotherBrainBomb, *v2);
  return k + 2;
}

uint16 MotherBrain_Instr_SpawnLaserEproj(uint16 k) {  // 0xA99F46
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  E->mbn_var_31 = 0;
  R18_ = E->base.x_pos + 16;
  R20_ = E->base.y_pos + 4;
  SpawnEnemyProjectileWithRoomGfx(addr_stru_86A17B, 1u);
  return k;
}

uint16 MotherBrain_Instr_SpawnRainbowEproj(uint16 k) {  // 0xA99F84
  uint16 v2;
  printf("A undefined\n"); v2 = 0;
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_MotherBrainRainbowBeamCharging, v2);
  return k;
}

uint16 MotherBrain_Instr_SetupFxForRainbowBeam(uint16 k) {  // 0xA99F8E
  Get_MotherBrain(0x40)->mbn_var_14 = 0;
  MotherBrain_SetupBrainPalForLaser();
  QueueSfx2_Max6(0x7Fu);
  return k;
}

void MotherBrain_Phase3_Death_0(void) {  // 0xA9AEE1
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->base.properties |= kEnemyProps_Tangible;
  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  E1->base.properties |= 0x400u;
  E->mbn_var_04 = 0;
  if (MotherBrain_MakeWalkBackwards(0x28, 6u) & 1) {
    E->mbn_var_A = FUNC16(MotherBrain_Phase3_Death_1);
    E->mbn_var_F = 128;
    MotherBrain_Phase3_Death_1();
  }
}

void MotherBrain_Phase3_Death_1(void) {  // 0xA9AF12
  MotherBrain_GenerateSmokyExplosions();
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0)
    E->mbn_var_A = FUNC16(MotherBrain_Phase3_Death_2);
}

void MotherBrain_Phase3_Death_2(void) {  // 0xA9AF21
  MotherBrain_GenerateSmokyExplosions();
  if (MotherBrain_MakeWalkForwards(2u, 0x60) & 1) {
    MotherBrain_SetBrainInstrs(addr_kMotherBrain_Ilist_9C39);
    Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
    E1->mbn_var_32 = 6;
    E1->mbn_var_33 = 6;
    E1->mbn_var_34 = 1280;
    Enemy_MotherBrain *E = Get_MotherBrain(0);
    E->mbn_var_A = FUNC16(MotherBrain_Phase3_Death_3);
    E->mbn_var_F = 32;
  }
}

void MotherBrain_Phase3_Death_3(void) {  // 0xA9AF54
  MotherBrain_GenerateSmokyExplosions();
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
    E1->mbn_var_32 = 0;
    E1->mbn_var_33 = 0;
    E1->mbn_var_12 = 0;
    E1->mbn_var_14 = 0;
    E1->mbn_var_10 = 0;
    E1->mbn_var_11 = 0;
    for (int i = 28; i >= 0; i -= 2)
      palette_buffer[(i >> 1) + 241] = palette_buffer[(i >> 1) + 145];
    MotherBrain_HealthBasedPaletteHandling();
    E->mbn_var_0D = 3584;
    E1->mbn_var_E = 0;
    E1->mbn_var_F = 0;
    E->mbn_var_A = FUNC16(MotherBrain_Phase3_Death_4);
    MotherBrain_Phase3_Death_4();
  }
}

void MotherBrain_Phase3_Death_4(void) {  // 0xA9AF9D
  MotherBrain_GenerateMixedExplosions();
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    E->mbn_var_37 = 0;
    E->mbn_var_A = FUNC16(MotherBrain_Phase3_Death_5);
    E->mbn_var_F = 0;
    MotherBrain_Phase3_Death_5();
  }
}

void MotherBrain_Phase3_Death_5(void) {  // 0xA9AFB6
  HandleMotherBrainBodyFlickering();
  MotherBrain_GenerateMixedExplosions();
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    E->mbn_var_F = 16;
    uint16 v2 = E->mbn_var_37 + 1;
    E->mbn_var_37 = v2;
    if (MotherBrain_FadePalToBlack(v2 - 1) & 1) {
      enemy_bg2_tilemap_size = 710;
      for (int i = 710; i >= 0; i -= 2)
        tilemap_stuff[i >> 1] = 824;
      nmi_flag_bg2_enemy_vram_transfer = 1;
      E->base.properties = E->base.properties & 0xDEFF | 0x100;
      E->base.extra_properties = 0;
      E->mbn_var_A = FUNC16(MotherBrain_Phase3_Death_6);
      E->mbn_var_F = 16;
    }
  }
}

void MotherBrain_Phase3_Death_6(void) {  // 0xA9B013
  MotherBrain_GenerateMixedExplosions();
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0)
    E->mbn_var_A = FUNC16(MotherBrain_Phase3_Death_7);
}

void MotherBrain_GenerateSmokyExplosions(void) {  // 0xA9B022
  R22_ = addr_word_A9B10F;
  R24_ = 2;
  MotherBrain_GenerateExplosions(0x10);
}

void MotherBrain_GenerateMixedExplosions(void) {  // 0xA9B031
  R22_ = addr_word_A9B109;
  R24_ = 4;
  MotherBrain_GenerateExplosions(8u);
}

void MotherBrain_GenerateExplosions(uint16 a) {  // 0xA9B03E
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  bool v2 = (--E->mbn_var_E & 0x8000) != 0;
  if (v2) {
    E->mbn_var_E = a;
    v2 = (--E->mbn_var_F & 0x8000) != 0;
    if (v2)
      E->mbn_var_F = 6;
    uint16 v3 = 16 * E->mbn_var_F;
    uint16 v4 = R24_;
    uint16 v9;
    do {
      v9 = v4;
      int v5 = v3 >> 1;
      R18_ = g_word_A9B099[v5];
      R20_ = g_word_A9B099[v5 + 1];
      uint16 *v6 = (uint16 *)RomPtr_A9(R22_);
      uint16 v7 = *v6;
      uint16 Random = NextRandom();
      if (Random >= 0x4000) {
        v7 = v6[1];
        if (Random >= 0xE000)
          v7 = v6[2];
      }
      SpawnEnemyProjectileWithRoomGfx(addr_kEproj_MotherBrainDeathExplosion, v7);
      v3 += 4;
      v4 = v9 - 1;
    } while (v9 != 1);
    QueueSfx3_Max3(0x13u);
  }
}

void MotherBrain_Phase3_Death_7(void) {  // 0xA9B115
  MotherBrain_SetBrainInstrs(addr_kMotherBrain_Ilist_9C29);
  Get_MotherBrain(0x40)->mbn_var_A = FUNC16(MotherBrainsBrain_SetupBrainToDraw);
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->mbn_var_F = 0;
  E->mbn_var_A = FUNC16(MotherBrain_Phase3_Death_8);
  MotherBrain_Phase3_Death_8();
}

void MotherBrain_Phase3_Death_8(void) {  // 0xA9B12D
  int16 v3;

  Enemy_MotherBrain *E = Get_MotherBrain(0);
  uint16 v1 = E->mbn_var_F + 32;
  E->mbn_var_F = v1;
  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  v3 = E1->base.y_pos + HIBYTE(v1);
  if ((uint16)v3 >= 0xC4u) {
    EnableEarthquakeAframes(2u);
    E->mbn_var_A = FUNC16(MotherBrain_Phase3_Death_9);
    E->mbn_var_F = 256;
    v3 = 196;
  }
  E1->base.y_pos = v3;
}

void MotherBrain_Phase3_Death_9(void) {  // 0xA9B15E
  if (ProcessSpriteTilesTransfers(0xa9, addr_stru_A99003)) {
    Enemy_MotherBrain *E = Get_MotherBrain(0);
    E->mbn_var_A = FUNC16(MotherBrain_Phase3_Death_10);
    E->mbn_var_F = 32;
  }
}

void MotherBrain_Phase3_Death_10(void) {  // 0xA9B173
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    E->mbn_var_37 = 0;
    E->mbn_var_A = FUNC16(MotherBrain_Phase3_Death_11);
    E->mbn_var_F = 0;
  }
}

void MotherBrain_Phase3_Death_11(void) {  // 0xA9B189
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    uint16 v2 = E->mbn_var_37 + 1;
    E->mbn_var_37 = v2;
    if (MotherBrain_FadeToGray_RealDeath(v2 - 1) & 1) {
      MotherBrain_SetBrainInstrs(addr_kMotherBrain_Ilist_9D25);
      E->mbn_var_A = FUNC16(MotherBrain_Phase3_Death_12);
      E->mbn_var_F = 256;
    } else {
      E->mbn_var_F = 16;
    }
  }
}

void MotherBrain_Phase3_Death_12(void) {  // 0xA9B1B8
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    E->mbn_var_A = FUNC16(MotherBrain_Phase3_Death_13);
    Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
    E1->base.properties |= 0x400u;
    E->mbn_var_04 = 0;
  }
}

void MotherBrain_Phase3_Death_13(void) {  // 0xA9B1D5
  if (ProcessCorpseRotting(0x40) & 1) {
    uint16 dms_var_53 = Get_DeadMonsters(0x40)->dms_var_53;
    ProcessCorpseRottingVramTransfers(dms_var_53);
  } else {
    Enemy_DeadMonsters *E1 = Get_DeadMonsters(0x40);
    E1->base.properties = E1->base.properties & 0xDEFF | 0x100;
    E1->base.extra_properties = 0;
    QueueMusic_Delayed8(0);
    QueueMusic_Delayed8(0xFF24u);
    E1 = Get_DeadMonsters(0);
    E1->dms_var_A = FUNC16(MotherBrain_Phase3_Death_14_20framedelay);
    E1->dms_var_F = 20;
    MotherBrain_Phase3_Death_14_20framedelay();
  }
}

void MotherBrain_Phase3_Death_14_20framedelay(void) {  // 0xA9B211
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
    E1->base.x_pos = 0;
    E1->base.y_pos = 0;
    E->mbn_var_A = FUNC16(MotherBrain_Phase3_Death_15_LoadEscapeTimerTiles);
  }
}

void MotherBrain_CorpseRottingFinished(void) {  // 0xA9B223
  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  R18_ = E1->base.x_pos + (random_number & 0x1F) - 16;
  R20_ = E1->base.y_pos + 16;
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 0xA);
  if ((enemy_damage_routine_exec_count & 7) == 0)
    QueueSfx2_Max3(0x10);
}

void MotherBrain_Phase3_Death_15_LoadEscapeTimerTiles(void) {  // 0xA9B258
  if (ProcessSpriteTilesTransfers(0xa6, addr_stru_A6C4CB)) {
    Get_MotherBrain(0)->mbn_var_A = FUNC16(MotherBrain_Phase3_Death_16_StartEscape);
    MotherBrain_Phase3_Death_16_StartEscape();
  }
}

void MotherBrain_Phase3_Death_16_StartEscape(void) {  // 0xA9B26D
  if (ProcessSpriteTilesTransfers(0xa9, addr_stru_A9902F)) {
    WriteColorsToPalette(0x122, 0xa9, addr_kMotherBrainPalette_5 + 2, 0xE);
    QueueMusic_Delayed8(7u);
    earthquake_type = 5;
    earthquake_timer = -1;
    SpawnPalfxObject(addr_kPalfx_FFC9);
    SpawnPalfxObject(addr_kPalfx_FFCD);
    SpawnPalfxObject(addr_kPalfx_FFD1);
    SpawnPalfxObject(addr_kPalfx_FFD5);
    Get_MotherBrain(0x40)->mbn_var_02 = 0;
    SetupZebesEscapeTypewriter();
    Enemy_MotherBrain *E = Get_MotherBrain(0);
    E->mbn_var_F = 32;
    uint16 v1 = FUNC16(MotherBrain_Phase3_Death_17_SpawnTimeBomb);
    if (!japanese_text_flag)
      v1 = FUNC16(MotherBrain_Phase3_Death_18_TypesZebesText);
    E->mbn_var_A = v1;
  }
}

void MotherBrain_Phase3_Death_17_SpawnTimeBomb(void) {  // 0xA9B2D1
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    E->mbn_var_A = FUNC16(MotherBrain_Phase3_Death_18_TypesZebesText);
    SpawnEnemyProjectileWithRoomGfx(
      addr_kEproj_TimeBombSetJapaneseText,
      FUNC16(MotherBrain_Phase3_Death_18_TypesZebesText));
  }
  MotherBrain_Phase3_Death_18_TypesZebesText();
}

void MotherBrain_Phase3_Death_18_TypesZebesText(void) {  // 0xA9B2E3
  if (HandleTypewriterText_Ext(0x2610) & 1) {
    Enemy_MotherBrain *E = Get_MotherBrain(0);
    E->mbn_var_A = FUNC16(MotherBrain_Phase3_Death_19_EscapeDoorExploding);
    E->mbn_var_F = 32;
  }
}

void MotherBrain_Phase3_Death_19_EscapeDoorExploding(void) {  // 0xA9B2F9
  MotherBrain_GenerateEscapeDoorExploding();
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    CallSomeSamusCode(0xFu);
    timer_status = 2;
    SetBossBitForCurArea(2u);
    SetEventHappened(0xEu);
    E->mbn_var_A = FUNC16(MotherBrain_Phase3_Death_20_BlowUpEscapeDoor);
    Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
    E1->mbn_var_E = 0;
    E1->mbn_var_F = 0;
  }
}

void MotherBrain_Phase3_Death_20_BlowUpEscapeDoor(void) {  // 0xA9B32A
  static const SpawnHardcodedPlmArgs unk_A9B337 = { 0x00, 0x06, 0xb677 };
  MotherBrain_ExplodeEscapeDoor();
  Get_MotherBrain(0)->mbn_var_A = FUNC16(MotherBrain_Phase3_Death_21_KeepEarthquakeGoing);
  SpawnHardcodedPlm(&unk_A9B337);
}

void MotherBrain_Phase3_Death_21_KeepEarthquakeGoing(void) {  // 0xA9B33C
  if (!earthquake_timer)
    --earthquake_timer;
}

void MotherBrain_GenerateEscapeDoorExploding(void) {  // 0xA9B346
  int16 v1;
  int16 v2;

  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  v1 = E->mbn_var_E - 1;
  E->mbn_var_E = v1;
  if (v1 < 0) {
    E->mbn_var_E = 4;
    v2 = E->mbn_var_F - 1;
    E->mbn_var_F = v2;
    if (v2 < 0)
      E->mbn_var_F = 3;
    int v3 = (uint16)(4 * E->mbn_var_F) >> 1;
    R18_ = g_word_A9B393[v3];
    R20_ = g_word_A9B393[v3 + 1];
    uint16 v4 = 3;
    if (NextRandom() < 0x4000)
      v4 = 12;
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, v4);
    QueueSfx2_Max3(0x24u);
  }
}

void MotherBrain_ExplodeEscapeDoor(void) {  // 0xA9B3A3
  uint16 v0 = 0, v1;
  do {
    v1 = v0;
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_MotherBrainExplodedEscapeDoor, v0);
    v0 = v1 + 1;
  } while ((uint16)(v1 + 1) < 8u);
}

void MotherBrain_SamusCollDetect(void) {  // 0xA9B3B6
  char v1; // cf

  Enemy_MotherBrain *E0 = Get_MotherBrain(0);
  R26_ = E0->mbn_var_04;
  v1 = R26_ & 1;
  R26_ >>= 1;
  if (!v1
      || (R18_ = E0->base.x_pos,
          R20_ = E0->base.y_pos,
          !(MotherBrain_SamusCollDetectPart(addr_word_A9B427) & 1))) {

    Enemy_MotherBrain *E = Get_MotherBrain(0x40);
    v1 = R26_ & 1;
    R26_ >>= 1;
    if (!v1
        || (R18_ = E->base.x_pos,
            R20_ = E->base.y_pos,
            !(MotherBrain_SamusCollDetectPart(0xB439u) & 1))) {
      v1 = R26_ & 1;
      R26_ >>= 1;
      if (v1) {
        R18_ = E->mbn_var_25;
        R20_ = E->mbn_var_26;
        if (!(MotherBrain_SamusCollDetectPart(0xB44Bu) & 1)) {
          R18_ = E->mbn_var_28;
          R20_ = E->mbn_var_29;
          if (!(MotherBrain_SamusCollDetectPart(0xB44Bu) & 1)) {
            R18_ = E->mbn_var_2B;
            R20_ = E->mbn_var_2C;
            MotherBrain_SamusCollDetectPart(0xB44Bu);
          }
        }
      }
    }
  }
}

uint8 MotherBrain_SamusCollDetectPart(uint16 k) {  // 0xA9B455
  int16 v5;

  uint16 *v1 = (uint16 *)RomPtr_A9(k);
  if (!*v1)
    return 0;
  R22_ = *v1;
  for (int i = k + 2; ; i += 8) {
    uint16 v3, v4;
    if ((int16)(samus_y_pos - R20_) >= 0) {
      R24_ = samus_y_pos - R20_;
      v3 = *((uint16 *)RomPtr_A9(i) + 3);
    } else {
      R24_ = R20_ - samus_y_pos;
      v3 = *((uint16 *)RomPtr_A9(i) + 1);
    }
    if ((int16)(samus_y_radius + abs16(v3) - R24_) >= 0) {
      if ((int16)(samus_x_pos - R18_) >= 0) {
        R24_ = samus_x_pos - R18_;
        v4 = *((uint16 *)RomPtr_A9(i) + 2);
      } else {
        R24_ = R18_ - samus_x_pos;
        v4 = *(uint16 *)RomPtr_A9(i);
      }
      v5 = samus_x_radius + abs16(v4) - R24_;
      if (v5 >= 0)
        break;
    }
    if (!--R22_)
      return 0;
  }
  if (sign16(v5 - 4))
    v5 = 4;
  extra_samus_x_displacement = v5;
  extra_samus_y_displacement = 4;
  extra_samus_x_subdisplacement = 0;
  extra_samus_y_subdisplacement = 0;
  samus_invincibility_timer = 96;
  samus_knockback_timer = 5;
  knockback_x_dir = 1;
  if (sign16(samus_y_pos - 192))
    samus_y_dir = 2;
  if ((int16)(Get_MotherBrain(0)->base.x_pos + 24 - samus_x_pos) < 0)
    MotherBrain_HurtSamus();
  return 1;
}

void MotherBrainsBody_Shot(void) {  // 0xA9B503
  CreateDudShot();
}

void MotherBrainsBrain_Shot(void) {  // 0xA9B507
  int16 v2;

  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if (E->mbn_var_00) {
    MotherBrain_Phase23_ShotReaction();
    if (E->mbn_var_00 == 1)
      CreateDudShot();
    else
      NormalEnemyShotAiSkipDeathAnim_CurEnemy();
  } else {
    uint16 v1 = HIBYTE(projectile_type[collision_detection_index]) & 7;
    if (g_byte_A9B546[v1]) {
      plm_room_arguments[39] += g_byte_A9B546[v1];
      QueueSfx2_Max6(0x6Eu);
      v2 = 13;
      Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
      uint16 flash_timer = E1->base.flash_timer;
      if (flash_timer) {
        if (flash_timer & 1)
          v2 = 14;
      }
      E1->base.flash_timer = v2;
      NormalEnemyShotAiSkipDeathAnim_CurEnemy();
    }
  }
}

void MotherBrain_Phase23_ShotReaction(void) {  // 0xA9B562
  int16 v1;

  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if (E->mbn_var_00 == 4 && MotherBrain_DetermineShotReactionType() == 2) {
    MotherBrain_Phase3_BeamShotReaction();
  } else {
    if (MotherBrain_DetermineShotReactionType() == 1 || (v1 = E->mbn_var_07 - 256, v1 < 0))
      v1 = 0;
    E->mbn_var_07 = v1;
  }
}

uint16 MotherBrain_DetermineShotReactionType(void) {  // 0xA9B58E
  return g_byte_A9B5A1[HIBYTE(projectile_type[collision_detection_index]) & 7];
}

void MotherBrain_Phase3_BeamShotReaction(void) {  // 0xA9B5A9
  int16 v1;

  Enemy_MotherBrain *E = Get_MotherBrain(0);
  v1 = E->mbn_var_07 - 266;
  if (v1 < 0) {
    Get_MotherBrain(0x40)->mbn_var_18 = FUNC16(MotherBrain_Phase3_Neck_SetupHyperBeamRecoil);
    v1 = 0;
    E->mbn_var_F = 0;
  }
  E->mbn_var_07 = v1;
}

void MotherBrainsBrain_Touch(void) {  // 0xA9B5C6
  int16 v0;

  if (samus_movement_type == 3) {
    v0 = 13;
    EnemyData *v1 = gEnemyData(0);
    uint16 flash_timer = v1[1].flash_timer;
    if (flash_timer) {
      if (flash_timer & 1)
        v0 = 14;
    }
    v1[1].flash_timer = v0;
  }
}

void MotherBrain_HurtSamus(void) {  // 0xA9B5E1
  Ridley_Func_98();
  samus_invincibility_timer = 96;
  samus_knockback_timer = 5;
  knockback_x_dir = (int16)(samus_x_pos - Get_MotherBrain(cur_enemy_index)->base.x_pos) >= 0;
}

void MotherBrain_Body_Phase2_Thinking(void) {  // 0xA9B605
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  if (!E1->base.health) {
    E->mbn_var_A = FUNC16(MotherBomb_FiringRainbowBeam_0);
    MotherBomb_FiringRainbowBeam_0();
    return;
  }
  if (!E->mbn_var_02) {
    if (!sign16(E1->base.health - 4500)) {
      if (random_number >= 0x1000) {
LABEL_6:
        MotherBrain_HandleWalking();
        return;
      }
LABEL_7:
      E->mbn_var_A = FUNC16(MotherBrain_Body_Phase2_TryAttack);
      return;
    }
    if (random_number < 0x2000)
      goto LABEL_6;
    if (random_number >= 0xA000)
      goto LABEL_7;
    E->mbn_var_A = FUNC16(MotherBomb_FiringDeathBeam);
  }
}

void MotherBrain_Body_Phase2_TryAttack(void) {  // 0xA9B64B
  int16 v0;
  Enemy_MotherBrain *E = Get_MotherBrain(0);

  v0 = 2 * E->mbn_var_18;
  if (v0) {
    if (v0 == 2) {
      MotherBrain_Phase2_Attack_Cooldown();
    } else {
      if (v0 != 4) {
        Unreachable();
        while (1)
          ;
      }
      MotherBrain_Phase2_Attack_End();
    }
  } else {
    E->mbn_parameter_1 = 64;
    ++E->mbn_var_18;
    if (MotherBrain_Phase2_DecideAttackStrategy_DoubleRet())
      return;
    uint16 v2 = addr_byte_A9B6DC;
    Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
    if (abs16(E1->base.y_pos + 4 - samus_y_pos) < 0x20)
      v2 = addr_byte_A9B6DF;
    uint16 v4 = 0;
    uint8 *v5 = RomPtr_A9(v2);
    if ((uint8)random_number >= *v5) {
      v4 = 2;
      if ((uint8)random_number >= v5[1]) {
        v4 = 4;
        if ((uint8)random_number >= v5[2])
          v4 = 6;
      }
    }
    uint16 v6 = g_off_A9B6D4[v4 >> 1];
    if (v6 == (uint16)addr_kMotherBrain_Ilist_9ECC) {
      if (sign16(E1->mbn_var_05 - 1)) {
        E->mbn_var_A = FUNC16(MotherBrain_FiringBomb_DecideOnWalking);
        MotherBrain_FiringBomb_DecideOnWalking();
      }
    } else if (v6 == (uint16)addr_kMotherBrain_Ilist_9F34) {
      E->mbn_var_A = FUNC16(MotherBomb_FiringLaser_PositionHead);
      MotherBomb_FiringLaser_PositionHead();
    } else {
      MotherBrain_SetBrainInstrs(v6);
    }
  }
}

uint8 MotherBrain_Phase2_DecideAttackStrategy_DoubleRet(void) {  // 0xA9B6E2
  if (g_word_A9B72C[samus_movement_type]) {
    if (sign16((uint8)random_number - 128)) {
      return 0;
    } else {
      Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
      if (sign16(E1->mbn_var_05 - 1)) {
        Get_MotherBrain(0)->mbn_var_A = FUNC16(MotherBrain_FiringBomb_DecideOnWalking);
        MotherBrain_FiringBomb_DecideOnWalking();
        return 1;
      }
      return 0;
    }
  } else {
    if (sign16((uint8)random_number - 128)) {
      Get_MotherBrain(0)->mbn_var_A = FUNC16(MotherBomb_FiringLaser_PositionHead);
      MotherBomb_FiringLaser_PositionHead();
    } else {
      MotherBrain_SetBrainInstrs(addr_kMotherBrain_Ilist_9D3D);
    }
    return 1;
  }
}

void MotherBrain_Phase2_Attack_Cooldown(void) {  // 0xA9B764
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if (E->mbn_parameter_1-- == 1)
    ++E->mbn_var_18;
}

void MotherBrain_Phase2_Attack_End(void) {  // 0xA9B773
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->mbn_var_18 = 0;
  E->mbn_var_A = FUNC16(MotherBrain_Body_Phase2_Thinking);
}

void MotherBrain_FiringBomb_DecideOnWalking(void) {  // 0xA9B781
  if (random_number >= 0xFF80) {
    MotherBrain_FiringBomb_DecideOnCrouching();
    return;
  }
  uint16 v0 = 64;
  if (random_number < 0x6000)
    v0 = 96;
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((int16)(v0 - E->base.x_pos) >= 0
      || (E->mbn_var_F = v0, MotherBrain_MakeWalkBackwards(v0, 6u) & 1)) {
    MotherBrain_FiringBomb_DecideOnCrouching();
  } else {
    E->mbn_var_A = FUNC16(MotherBrain_FiringBomb_WalkingBackwards);
  }
}

void MotherBrain_FiringBomb_WalkingBackwards(void) {  // 0xA9B7AC
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if (MotherBrain_MakeWalkBackwards(E->mbn_var_F, 6u) & 1)
    MotherBrain_FiringBomb_DecideOnCrouching();
}

void MotherBrain_FiringBomb_DecideOnCrouching(void) {  // 0xA9B7B7
  if (NextRandom() >= 0x8000) {
    Get_MotherBrain(0)->mbn_var_A = FUNC16(MotherBrain_FiringBomb_Crouch);
    MotherBrain_FiringBomb_Crouch();
  } else {
    MotherBrain_B7CB();
  }
}

void MotherBrain_FiringBomb_Crouch(void) {  // 0xA9B7C6
  if (MotherBrain_MakeHerCrouch() & 1)
    MotherBrain_B7CB();
}

void MotherBrain_B7CB(void) {  // 0xA9B7CB
  uint16 v0 = addr_kMotherBrain_Ilist_9ECC;
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if (E->mbn_var_1F)
    v0 = addr_kMotherBrain_Ilist_9F00;
  MotherBrain_SetBrainInstrs(v0);
  E->mbn_var_A = FUNC16(MotherBrain_FiringBomb_Fired);
  E->mbn_var_F = 44;
}

void MotherBrain_FiringBomb_Fired(void) {  // 0xA9B7E8
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    if (MotherBrain_MakeHerStandUp() & 1) {
      MotherBrain_FiringBomb_Finish();
    } else {
      E->mbn_var_A = FUNC16(MotherBrain_FiringBomb_Standup);
      MotherBrain_FiringBomb_Standup();
    }
  }
}

void MotherBrain_FiringBomb_Standup(void) {  // 0xA9B7F8
  if (MotherBrain_MakeHerStandUp() & 1)
    MotherBrain_FiringBomb_Finish();
}

void MotherBrain_FiringBomb_Finish(void) {  // 0xA9B7FD
  uint16 v0 = FUNC16(MotherBrain_Body_Phase2_Thinking);
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if (E->mbn_var_1F)
    v0 = FUNC16(MotherBrain_Phase2_MurderShitroid_1);
  E->mbn_var_A = v0;
}

void MotherBomb_FiringLaser_PositionHead(void) {  // 0xA9B80E
  int16 v0;

  v0 = 8;
  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  if ((int16)(E1->base.y_pos - samus_y_pos) < 0)
    v0 = 6;
  E1->mbn_var_32 = v0;
  E1->mbn_var_33 = v0;
  E1->mbn_var_34 = 512;
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->mbn_var_A = FUNC16(MotherBomb_FiringLaser_PositionHeadSlowlyFire);
  E->mbn_var_F = 4;
}

void MotherBomb_FiringLaser_PositionHeadSlowlyFire(void) {  // 0xA9B839
  int16 v2;

  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    v2 = 256;
    Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
    if ((E1->mbn_var_34 & 0x8000) != 0)
      v2 = -256;
    E1->mbn_var_34 = v2;
    MotherBrain_SetBrainInstrs(addr_kMotherBrain_Ilist_9F34);
    E->mbn_var_A = FUNC16(MotherBomb_FiringLaser_FinishAttack);
    E->mbn_var_F = 16;
  }
}

void MotherBomb_FiringLaser_FinishAttack(void) {  // 0xA9B863
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
    E1->mbn_var_32 = 4;
    E1->mbn_var_33 = 4;
    E->mbn_var_A = FUNC16(MotherBrain_Body_Phase2_Thinking);
    MotherBrain_Body_Phase2_Thinking();
  }
}

static Func_V *const off_A9B887[4] = {  // 0xA9B87D
  MotherBomb_FiringDeathBeam_0,
  MotherBomb_FiringDeathBeam_1,
  MotherBomb_FiringDeathBeam_2,
  MotherBomb_FiringDeathBeam_3,
};
void MotherBomb_FiringDeathBeam(void) {

  int v0 = (uint16)(2 * Get_MotherBrain(0)->mbn_var_17) >> 1;
  off_A9B887[v0]();
}

void MotherBomb_FiringDeathBeam_0(void) {  // 0xA9B88F
  if (MotherBrain_MakeWalkBackwards(0x28, 8u) & 1) {
    Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
    E1->mbn_var_32 = 8;
    E1->mbn_var_33 = 6;
    Enemy_MotherBrain *E = Get_MotherBrain(0);
    ++E->mbn_var_17;
  }
}

void MotherBomb_FiringDeathBeam_1(void) {  // 0xA9B8B2
  if (!Get_MotherBrain(0x40)->mbn_var_05) {
    MotherBrain_SetBodyInstrs(addr_kMotherBrain_Ilist_9A42);
    Enemy_MotherBrain *E = Get_MotherBrain(0);
    ++E->mbn_var_17;
  }
}

void MotherBomb_FiringDeathBeam_2(void) {  // 0xA9B8C8
  ;
}

void MotherBomb_FiringDeathBeam_3(void) {  // 0xA9B8C9
  MotherBrain_SetBrainInstrs(addr_kMotherBrain_Ilist_9C87);
  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  E1->mbn_var_32 = 2;
  E1->mbn_var_33 = 4;
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->mbn_var_17 = 0;
  E->mbn_var_A = FUNC16(MotherBrain_Body_Phase2_Thinking);
}

void MotherBomb_FiringRainbowBeam_0(void) {  // 0xA9B8EB
  MotherBrain_SetBrainInstrs(addr_kMotherBrain_Ilist_9C87);
  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  E1->mbn_var_34 = 64;
  E1->mbn_var_31 = 1;
  E1->mbn_var_32 = 2;
  E1->mbn_var_33 = 4;
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->mbn_var_A = FUNC16(MotherBomb_FiringRainbowBeam_1_StartCharge);
  E->mbn_var_F = 256;

}

void MotherBomb_FiringRainbowBeam_1_StartCharge(void) {  // 0xA9B91A
  Enemy_MotherBomb *E = Get_MotherBomb(0);
  if ((--E->mbb_var_F & 0x8000) != 0) {
    MotherBrain_SetBrainInstrs(addr_kMotherBrain_Ilist_9F6C);
    E->mbb_var_A = FUNC16(MotherBomb_FiringRainbowBeam_2_RetractNeck);
    MotherBomb_FiringRainbowBeam_2_RetractNeck();
  }
}

void MotherBomb_FiringRainbowBeam_2_RetractNeck(void) {  // 0xA9B92B
  if (MotherBrain_WalkBackwardsSlowlyAndRetractHead(0x28u) & 1) {
    Enemy_MotherBomb *E = Get_MotherBomb(0);
    E->mbb_var_A = FUNC16(MotherBomb_FiringRainbowBeam_3_Wait);
    E->mbb_var_F = 256;
    MotherBomb_FiringRainbowBeam_3_Wait();
  }
}

void MotherBomb_FiringRainbowBeam_3_Wait(void) {  // 0xA9B93F
  Enemy_MotherBomb *E = Get_MotherBomb(0);
  if ((--E->mbb_var_F & 0x8000) != 0) {
    QueueSfx2_Max6(0x71u);
    E->mbb_var_A = FUNC16(MotherBomb_FiringRainbowBeam_4_ExtendNeckDown);
    MotherBomb_FiringRainbowBeam_4_ExtendNeckDown();
  }
}

void MotherBomb_FiringRainbowBeam_4_ExtendNeckDown(void) {  // 0xA9B951
  cooldown_timer = 8;
  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  E1->mbn_var_32 = 6;
  E1->mbn_var_33 = 6;
  E1->mbn_var_34 = 1280;
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->mbn_var_A = FUNC16(MotherBomb_FiringRainbowBeam_5_StartFiring);
  E->mbn_var_F = 16;
  MotherBomb_FiringRainbowBeam_5_StartFiring();
}

void MotherBomb_FiringRainbowBeam_5_StartFiring(void) {  // 0xA9B975
  MotherBrain_AimBeamAndIncrWidth();
  if (!power_bomb_flag) {
    Enemy_MotherBrain *E = Get_MotherBrain(0);
    if ((--E->mbn_var_F & 0x8000) != 0 && !power_bomb_flag) {
      cooldown_timer = 0;
      MotherBrain_BodyRainbowBeamPalAnimIndex0();
      MotherBrain_SetBrainInstrs(addr_kMotherBrain_Ilist_9C77);
      E->mbn_var_33 = 512;
      E->mbn_var_09 = SpawnMotherBrainRainbowBeamHdma();
      Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
      E1->mbn_var_34 = 64;
      E1->mbn_var_31 = 1;
      E1->mbn_var_32 = 2;
      E1->mbn_var_33 = 4;
      E->mbn_parameter_1 = 0;
      E->mbn_parameter_2 = 0;
      uint16 v4 = 5;
      if ((int16)(samus_health - 700) < 0)
        v4 = 24;
      CallSomeSamusCode(v4);
      E->mbn_var_15 = 6;
      E->mbn_var_A = FUNC16(MotherBomb_FiringRainbowBeam_6_MoveSamusToWall);
    }
  }
}

void MotherBomb_FiringRainbowBeam_6_MoveSamusToWall(void) {  // 0xA9B9E5
  MotherBrain_PlayRainbowBeamSfx();
  MotherBrain_HandleRainbowBeamPalette();
  MotherBrain_AimBeamAndIncrWidth();
  MotherBrain_HandleRainbowBeamExplosions();
  if (MotherBrain_MoveSamusTowardsWallDueToBeam() & 1) {
    Enemy_MotherBomb *E = Get_MotherBomb(0);
    E->mbb_var_A = FUNC16(MotherBomb_FiringRainbowBeam_7_DelayFrame);
    E->mbb_var_F = 0;
  }
}

void MotherBomb_FiringRainbowBeam_7_DelayFrame(void) {  // 0xA9BA00
  MotherBrain_PlayRainbowBeamSfx();
  MotherBrain_HandleRainbowBeamPalette();
  MotherBrain_AimBeamAndIncrWidth();
  MotherBrain_HandleRainbowBeamExplosions();
  MotherBrain_MoveSamusTowardsWallDueToBeam();
  Enemy_MotherBomb *E = Get_MotherBomb(0);
  if ((--E->mbb_var_F & 0x8000) != 0) {
    earthquake_type = 8;
    earthquake_timer = 8;
    E->mbb_var_A = FUNC16(MotherBomb_FiringRainbowBeam_8_StartDrainSamus);
  }
}

void MotherBomb_FiringRainbowBeam_8_StartDrainSamus(void) {  // 0xA9BA27
  Enemy_MotherBomb *E = Get_MotherBomb(0);
  E->mbb_var_A = FUNC16(MotherBomb_FiringRainbowBeam_9_DrainingSamus);
  E->mbb_var_F = 299;
  earthquake_timer = 299;
  earthquake_type = 8;
  MotherBomb_FiringRainbowBeam_9_DrainingSamus();
}

void MotherBomb_FiringRainbowBeam_9_DrainingSamus(void) {  // 0xA9BA3C
  MotherBrain_PlayRainbowBeamSfx();
  MotherBrain_HandleRainbowBeamPalette();
  MotherBrain_AimBeamAndIncrWidth();
  MotherBrain_HandleRainbowBeamExplosions();
  Samus_DamageDueToRainbowBeam();
  Samus_DecrementAmmoDueToRainbowBeam();
  MotherBrain_MoveSamusTowardsMiddleOfWall();
  Enemy_MotherBomb *E = Get_MotherBomb(0);
  if ((--E->mbb_var_F & 0x8000) != 0)
    E->mbb_var_A = FUNC16(MotherBomb_FiringRainbowBeam_10_FinishFiringRainbow);
}

void MotherBomb_FiringRainbowBeam_10_FinishFiringRainbow(void) {  // 0xA9BA5E
  MotherBrain_PlayRainbowBeamSfx();
  MotherBrain_HandleRainbowBeamPalette();
  MotherBrain_AimBeam();
  MotherBrain_HandleRainbowBeamExplosions();
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  uint16 v1 = E->mbn_var_33 - 384;
  E->mbn_var_33 = v1;
  if (sign16(v1 - 512)) {
    E->mbn_var_33 = 512;
    E->mbn_parameter_1 = -256;
    E->mbn_parameter_2 = 0;
    hdma_object_channels_bitmask[E->mbn_var_09 >> 1] = 0;
    earthquake_timer = 0;
    MotherBrain_SetBrainInstrs(addr_kMotherBrain_Ilist_9C87);
    MotherBrain_SetupBrainNormalPal();
    MotherBrain_WriteDefaultPalette();
    QueueSfx1_Max6(2u);
    E->mbn_var_16 = 0;
    CallSomeSamusCode(1u);
    cooldown_timer = 8;
    E->mbn_var_A = FUNC16(MotherBomb_FiringRainbowBeam_11_LetSamusFall);
  }
}

void MotherBomb_FiringRainbowBeam_11_LetSamusFall(void) {  // 0xA9BAC4
  SomeMotherBrainScripts(0);
  Get_MotherBomb(0)->mbb_var_A = FUNC16(MotherBomb_FiringRainbowBeam_12_WaitForSamusHitGround);
  MotherBomb_FiringRainbowBeam_12_WaitForSamusHitGround();
}

void MotherBomb_FiringRainbowBeam_12_WaitForSamusHitGround(void) {  // 0xA9BAD1
  if (MotherBrain_MoveSamusForFallingAfterBeam() & 1)
    Get_MotherBomb(0)->mbb_var_A = FUNC16(MotherBomb_FiringRainbowBeam_13_LowerHead);
}

void MotherBomb_FiringRainbowBeam_13_LowerHead(void) {  // 0xA9BADD
  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  E1->mbn_var_34 = 64;
  E1->mbn_var_31 = 1;
  E1->mbn_var_32 = 2;
  E1->mbn_var_33 = 4;
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->mbn_var_A = FUNC16(MotherBomb_FiringRainbowBeam_14_DecideNextAction);
  E->mbn_var_F = 128;
}

void MotherBomb_FiringRainbowBeam_14_DecideNextAction(void) {  // 0xA9BB06
  Enemy_MotherBomb *E = Get_MotherBomb(0);
  if ((--E->mbb_var_F & 0x8000) != 0) {
    if (sign16(samus_health - 400)) {
      MotherBrain_MakeWalkForwards(0xA, E->base.x_pos + 16);
      E->mbb_var_A = FUNC16(MotherBrain_Phase2Cut_0);
    } else {
      E->mbb_var_A = FUNC16(MotherBomb_FiringRainbowBeam_0);
    }
  }
}

void MotherBrain_PlayRainbowBeamSfx(void) {  // 0xA9BB2E
  int16 mbn_var_15;

  Enemy_MotherBrain *E = Get_MotherBrain(0);
  mbn_var_15 = E->mbn_var_15;
  if (mbn_var_15 >= 0) {
    E->mbn_var_15 = mbn_var_15 - 1;
    QueueSfx1_Max6(0x40);
    E->mbn_var_16 = 1;
  }
}

uint8 MotherBrain_WalkBackwardsSlowlyAndRetractHead(uint16 a) {  // 0xA9BB48
  uint8 rv = MotherBrain_MakeWalkBackwards(a, 0xA);
  if (rv)
    MotherBrain_RetractHead();
  return rv;
}

void MotherBrain_RetractHead(void) {  // 0xA9BB51
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  E->mbn_var_34 = 80;
  E->mbn_var_31 = 1;
  E->mbn_var_32 = 8;
  E->mbn_var_33 = 6;
}

void MotherBrain_AimBeamAndIncrWidth(void) {  // 0xA9BB6E
  int16 v1;

  Enemy_MotherBrain *E = Get_MotherBrain(0);
  v1 = E->mbn_var_33 + 384;
  if (!sign16(E->mbn_var_33 - 2688))
    v1 = 3072;
  E->mbn_var_33 = v1;
  MotherBrain_AimBeam();
}

void MotherBrain_AimBeam(void) {  // 0xA9BB82
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  R18_ = samus_x_pos - E->base.x_pos - 16;
  R20_ = samus_y_pos - E->base.y_pos - 4;
  uint16 v1 = (uint8)-(int8)(CalculateAngleFromXY() + 0x80);
  Get_MotherBrain(0)->mbn_var_31 = v1;
}

void MotherBrain_CalculateRainbowBeamHdma_(void) {  // 0xA9BBB0
  MotherBrain_CalculateRainbowBeamHdma();
}

uint8 MotherBrain_MoveSamusTowardsWallDueToBeam(void) {  // 0xA9BBB5
  uint8 v0 = MotherBrain_MoveSamusHorizTowardsWall(0x1000);
  if (!v0) {
    R18_ = Get_MotherBrain(0)->mbn_var_31;
    uint16 v1 = Math_MultByCos(0x1000);
    MotherBrain_MoveSamusVerticallyTowardsCeilingFloor(v1);
  }
  return v0;
}

void MotherBrain_MoveSamusTowardsMiddleOfWall(void) {  // 0xA9BBCF
  uint16 v0 = 64;
  if ((int16)(124 - samus_y_pos) < 0)
    v0 = -64;
  MotherBrain_MoveSamusVerticallyTowardsCeilingFloor(v0);
}

uint8 MotherBrain_MoveSamusForFallingAfterBeam(void) {  // 0xA9BBE1
  int16 v1;

  Enemy_MotherBrain *E = Get_MotherBrain(0);
  v1 = E->mbn_parameter_1 + 2;
  if (v1 >= 0)
    v1 = 0;
  E->mbn_parameter_1 = v1;
  MotherBrain_MoveSamusHorizTowardsWall(v1);
  uint16 v2 = E->mbn_parameter_2 + 24;
  E->mbn_parameter_2 = v2;
  return MotherBrain_MoveSamusVerticallyTowardsCeilingFloor(v2) & 1;
}

uint8 MotherBrain_MoveSamusVerticallyTowardsCeilingFloor(uint16 a) {  // 0xA9BBFD
  int carry = HIBYTE(samus_y_subpos) + LOBYTE(a);
  HIBYTE(samus_y_subpos) = carry;
  HIBYTE(samus_prev_y_subpos) = carry;
  uint16 v4 = samus_y_pos + (int8)(a >> 8) + (carry >> 8), v6;
  if (sign16(v4 - 48)) {
    v6 = 48;
  } else {
    if (sign16(v4 - 192)) {
      samus_prev_y_pos = samus_y_pos = v4;
      return 0;
    }
    v6 = 192;
  }
  samus_prev_y_pos = samus_y_pos = v6;
  samus_y_subpos = 0;
  samus_prev_y_subpos = 0;
  return 1;
}

uint8 MotherBrain_MoveSamusHorizTowardsWall(uint16 a) {  // 0xA9BC3F
  int carry = HIBYTE(samus_x_subpos) + LOBYTE(a);
  HIBYTE(samus_x_subpos) = carry;
  HIBYTE(samus_prev_x_subpos) = carry;
  uint16 v4 = samus_x_pos + (int8)(a >> 8) + (carry >> 8);
  if (sign16(v4 - 235)) {
    samus_x_pos = v4;
    samus_prev_x_pos = v4;
    return 0;
  } else {
    samus_x_pos = 235;
    samus_prev_x_pos = 235;
    samus_x_subpos = 0;
    samus_prev_x_subpos = 0;
    return 1;
  }
}

void MotherBrain_HandleRainbowBeamExplosions(void) {  // 0xA9BC76
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_parameter_2 & 0x8000) != 0) {
    E->mbn_parameter_2 = 8;
    ++E->mbn_parameter_1;
    int v2 = E->mbn_parameter_1 & 7;
    R18_ = g_word_A9BCA6[v2];
    R20_ = g_word_A9BCB6[v2];
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_MotherBrainRainbowBeamExplosion, R20_);
    QueueSfx2_Max6(0x24u);
  }
}

void MotherBrain_BodyRainbowBeamPalAnimIndex0(void) {  // 0xA9BCC6
  Get_MotherBrain(0x40)->mbn_var_01 = 0;
}

void MotherBrain_WriteDefaultPalette(void) {  // 0xA9BCCE
  WriteColorsToPalette(0x82, 0xa9, addr_kMotherBrainsBrain_Palette + 2, 0xF);
  WriteColorsToPalette(0x122, 0xa9, addr_kMotherBrainsBrain_Palette + 2, 0xF);
  WriteColorsToPalette(0x162, 0xa9, addr_kMotherBrainPalette_0 + 2, 0xF);
}

void MotherBrain_WritePhase2DeathPalette(void) {  // 0xA9BCF6
  MotherBrain_WritePalette(MotherBrain_RainbowBeamPalettes[6]);
}

void MotherBrain_HandleRainbowBeamPalette(void) {  // 0xA9BCFD
  if ((Get_MotherBrain(0)->base.frame_counter & 2) != 0) {
    uint16 mbn_var_01 = Get_MotherBrain(0x40)->mbn_var_01;
    uint16 v1;
    do {
      v1 = mbn_var_01;
      mbn_var_01 = MotherBrain_RainbowBeamPalettes[mbn_var_01 >> 1];
    } while (!mbn_var_01);
    uint16 v2 = v1 + 2;
    Get_MotherBrain(0x40)->mbn_var_01 = v2;
    MotherBrain_WritePalette(*(uint16 *)((char *)MotherBrain_RainbowBeamPalettes + v2 - 2));
  }
}

void MotherBrain_WritePalette(uint16 j) {  // 0xA9BD1D
  WriteColorsToPalette(0x82, 0xad, j, 15);
  WriteColorsToPalette(0x122, 0xad, j, 15);
  WriteColorsToPalette(0x162, 0xad, j + 30, 15);
}

void MotherBrain_Phase2Cut_0(void) {  // 0xA9BD45
  uint16 v0 = SuitDamageDivision(0x50);
  if ((int16)(4 * v0 + 20 - samus_health) >= 0) {
    Get_MotherBrain(0)->mbn_var_A = FUNC16(MotherBrain_Phase2Cut_1);
  } else if ((random_number & 0xFFFu) < 0xFA0) {
    if ((Get_MotherBrain(0)->base.frame_counter & 0x1F) == 0)
      MotherBrain_MaybeStandupOrLeanDown();
  } else {
    uint16 v1;
    if ((int16)(SuitDamageDivision(0xA0) + 20 - samus_health) >= 0
        || (v1 = addr_kMotherBrain_Ilist_9ECC, (random_number & 0xFFFu) < 0xFF0)) {
      v1 = addr_kMotherBrain_Ilist_9D7F;
    }
    MotherBrain_SetBrainInstrs(v1);
  }
}

void MotherBrain_Phase2Cut_1(void) {  // 0xA9BD98
  if (MotherBrain_MakeHerStandUp() & 1) {
    Enemy_MotherBrain *E = Get_MotherBrain(0);
    E->mbn_var_A = FUNC16(MotherBrain_Phase2Cut_2);
    E->mbn_var_F = 16;
    MotherBrain_Phase2Cut_2();
  }
}

void MotherBrain_Phase2Cut_2(void) {  // 0xA9BDA9
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    MotherBrain_SetBrainInstrs(addr_kMotherBrain_Ilist_9B7F);
    E->mbn_var_A = FUNC16(MotherBrain_Phase2Cut_3);
    E->mbn_var_F = 256;
  }
}

void MotherBrain_Phase2Cut_3(void) {  // 0xA9BDC1
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    MotherBrain_SetBrainInstrs(addr_kMotherBrain_Ilist_9F6C);
    E->mbn_var_A = FUNC16(MotherBrain_Phase2Cut_4);
    MotherBrain_Phase2Cut_4();
  }
}

void MotherBrain_Phase2Cut_4(void) {  // 0xA9BDD2
  if (ProcessSpriteTilesTransfers(0xa9, addr_stru_A98FE5)) {
    MotherBrain_RetractHead();
    MotherBrain_SpawnShitroidInCutscene();
    Enemy_MotherBrain *E = Get_MotherBrain(0);
    E->mbn_var_A = FUNC16(MotherBrain_Phase2Cut_5);
    E->mbn_var_F = 256;
  }
}

void MotherBrain_Phase2Cut_5(void) {  // 0xA9BDED
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    MotherBrain_BodyRainbowBeamPalAnimIndex0();
    MotherBrain_SetBrainInstrs(addr_kMotherBrain_Ilist_9C77);
    Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
    E1->mbn_var_32 = 6;
    E1->mbn_var_33 = 6;
    E1->mbn_var_34 = 1280;
    QueueSfx2_Max6(0x71u);
    E->mbn_var_A = FUNC16(nullsub_364);
  }
}

void MotherBrain_SpawnShitroidInCutscene(void) {  // 0xA9BE1B
  Get_MotherBrain(0x40)->mbn_var_0A = SpawnEnemy(0xA9, addr_stru_A9BE28);
}

void MotherBrain_DrainedByShitroid_0(void) {  // 0xA9BE38
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->mbn_var_00 = 3;
  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  E1->mbn_var_32 = 8;
  E1->mbn_var_33 = 8;
  E1->mbn_var_34 = 1792;
  E->mbn_var_A = FUNC16(MotherBrain_DrainedByShitroid_1);
  E->mbn_var_F = 48;
  MotherBrain_DrainedByShitroid_1();
}

void MotherBrain_DrainedByShitroid_1(void) {  // 0xA9BE5D
  MotherBrain_HandleRainbowBeamPalette();
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    E->mbn_var_A = FUNC16(MotherBrain_DrainedByShitroid_2);
    Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
    E1->mbn_var_08 = FUNC16(MotherBrain_PainfulWalkForwards);
    E1->mbn_var_06 = 0;
    E1->mbn_var_07 = 2;
    E1->mbn_var_32 = 2;
    E1->mbn_var_33 = 4;
  }
}

void CallMotherBrainPainfulWalk(uint32 ea) {
  switch (ea) {
  case fnMotherBrain_PainfulWalkForwards: MotherBrain_PainfulWalkForwards(); return;
  case fnMotherBrain_PainfulWalkingForwards: MotherBrain_PainfulWalkingForwards(); return;
  case fnMotherBrain_PainfulWalkBackwards: MotherBrain_PainfulWalkBackwards(); return;
  case fnMotherBrain_PainfulWalkingBackwards: MotherBrain_PainfulWalkingBackwards(); return;
  default: Unreachable();
  }
}

void MotherBrain_DrainedByShitroid_2(void) {  // 0xA9BE96
  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  if (!E1->mbn_var_00)
    E1->mbn_var_00 = 50;
  MotherBrain_HandleRainbowBeamPalette();
  R18_ = E1->mbn_var_08;
  CallMotherBrainPainfulWalk(R18_ | 0xA90000);
  uint16 v2 = 2 * E1->mbn_var_06;
  E1->mbn_var_07 = g_byte_A9BEEE[v2];
  E1->mbn_var_34 = g_word_A9BEFE[v2 >> 1];
  if (E1->mbn_var_06 == 6) {
    Enemy_MotherBrain *E = Get_MotherBrain(0);
    E->mbn_var_16 = 0;
    E1->mbn_var_10 = 0;
    MotherBrain_WritePhase2DeathPalette();
    QueueSfx1_Max6(2u);
    E->mbn_var_A = FUNC16(MotherBrain_DrainedByShitroid_3);
  }
}

void MotherBrain_DrainedByShitroid_3(void) {  // 0xA9BF0E
  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  R18_ = E1->mbn_var_08;
  CallMotherBrainPainfulWalk(R18_ | 0xA90000);
  if (sign16(E1->mbn_var_06 - 8)) {
  } else {
    E1->mbn_var_34 = 64;
    E1->mbn_var_32 = 8;
    E1->mbn_var_33 = 8;
    MotherBrain_SetBrainInstrs(addr_kMotherBrain_Ilist_9C39);
    Get_MotherBrain(0)->mbn_var_A = FUNC16(MotherBrain_DrainedByShitroid_4);
    MotherBrain_DrainedByShitroid_4();
  }
}

void MotherBrain_DrainedByShitroid_4(void) {  // 0xA9BF41
  if (MotherBrain_MakeWalkBackwards(0x28, 0) & 1) {
    Get_MotherBrain(0)->mbn_var_A = FUNC16(MotherBrain_DrainedByShitroid_5);
    Get_MotherBrain(0x40)->mbn_var_33 = 0;
    MotherBrain_DrainedByShitroid_5();
  }
}

void MotherBrain_DrainedByShitroid_5(void) {  // 0xA9BF56
  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  if (!*(uint32 *)&E1->mbn_var_32) {
    E1->mbn_var_12 = E1->mbn_var_33 | E1->mbn_var_32;
    Enemy_MotherBrain *E = Get_MotherBrain(0);
    if (!E->mbn_var_02) {
      MotherBrain_SetBodyInstrs(addr_kMotherBrain_Ilist_9A26);
      E->mbn_var_A = FUNC16(MotherBrain_DrainedByShitroid_6);
      E->mbn_var_F = 64;
    }
  }
}

void MotherBrain_DrainedByShitroid_6(void) {  // 0xA9BF7D
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    E->mbn_var_37 = 0;
    E->mbn_var_A = FUNC16(MotherBrain_DrainedByShitroid_7);
    E->mbn_var_F = 16;
    MotherBrain_DrainedByShitroid_7();
  }
}

void MotherBrain_DrainedByShitroid_7(void) {  // 0xA9BF95
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  bool v2 = (--E->mbn_var_F & 0x8000) != 0;
  uint16 v3;
  if (v2
      && (E->mbn_var_F = 16,
          v3 = E->mbn_var_37 + 1,
          E->mbn_var_37 = v3,
          MotherBrain_FadeToGray_Drained(v3 - 1) & 1)) {
    Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
    E1->base.health = -29536;
    E->mbn_var_1F = 1;
    E1->mbn_var_14 = 0;
    E->mbn_var_00 = 2;
    E->mbn_var_A = FUNC16(MotherBrain_Phase2_Revive_0);
  }
}

void MotherBrain_PainfulWalkForwards(void) {  // 0xA9BFD0
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  if (MotherBrain_MakeWalkForwards(E->mbn_var_07, 0x48) & 1) {
    E->mbn_var_08 = FUNC16(MotherBrain_PainfulWalkingForwards);
    MotherBrain_SetPainfulWalkingTimer();
  }
}

void MotherBrain_PainfulWalkingForwards(void) {  // 0xA9BFE8
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  uint16 v1 = E->mbn_var_09 - 1;
  E->mbn_var_09 = v1;
  if (!v1) {
    ++E->mbn_var_06;
    E->mbn_var_08 = FUNC16(MotherBrain_PainfulWalkBackwards);
  }
}

void MotherBrain_PainfulWalkBackwards(void) {  // 0xA9C004
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  if (MotherBrain_MakeWalkBackwards(0x28, E->mbn_var_07) & 1) {
    E->mbn_var_08 = FUNC16(MotherBrain_PainfulWalkingBackwards);
    MotherBrain_SetPainfulWalkingTimer();
  }
}

void MotherBrain_PainfulWalkingBackwards(void) {  // 0xA9C01C
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  uint16 v1 = E->mbn_var_09 - 1;
  E->mbn_var_09 = v1;
  if (!v1) {
    ++E->mbn_var_06;
    E->mbn_var_08 = FUNC16(MotherBrain_PainfulWalkForwards);
  }
}

void MotherBrain_SetPainfulWalkingTimer(void) {  // 0xA9C038
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  E->mbn_var_09 = LOBYTE(g_word_A9C049[E->mbn_var_06]);
}

void MotherBrain_Phase2_Revive_0(void) {  // 0xA9C059
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->mbn_var_A = FUNC16(MotherBrain_Phase2_Revive_1);
  E->mbn_var_F = 768;
}

void MotherBrain_Phase2_Revive_1(void) {  // 0xA9C066
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
    E1->mbn_var_14 = 1;
    E1->mbn_var_12 = 1;
    E->mbn_var_A = FUNC16(MotherBrain_Phase2_Revive_2);
    E->mbn_var_F = 224;
    MotherBrain_Phase2_Revive_2();
  }
}

void MotherBrain_Phase2_Revive_2(void) {  // 0xA9C082
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->mbn_var_A = FUNC16(MotherBrain_Phase2_Revive_3);
  E->mbn_var_37 = 0;
  MotherBrain_Phase2_Revive_3();
}

void MotherBrain_Phase2_Revive_3(void) {  // 0xA9C08F
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  bool v1 = (--E->mbn_var_F & 0x8000) != 0;
  uint16 v2;
  if (v1
      && (E->mbn_var_F = 16,
          v2 = E->mbn_var_37 + 1,
          E->mbn_var_37 = v2,
          MotherBrain_FadeFromGray_Drained(v2 - 1) & 1)) {
    E->mbn_var_A = FUNC16(MotherBrain_Phase2_Revive_4);
    Get_MotherBrain(0x40)->mbn_var_10 = 1;
    MotherBrain_SetupBrainNormalPal();
    MotherBrain_Phase2_Revive_4();
  }
}

void MotherBrain_Phase2_Revive_4(void) {  // 0xA9C0BA
  if (MotherBrain_MakeHerStandUp() & 1) {
    Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
    E1->mbn_var_32 = 6;
    E1->mbn_var_33 = 6;
    E1->mbn_var_34 = 1280;
    E1->mbn_var_31 = 1;
    Enemy_MotherBrain *E = Get_MotherBrain(0);
    E->mbn_var_A = FUNC16(MotherBrain_Phase2_Revive_5);
    E->mbn_var_F = 16;
    MotherBrain_Phase2_Revive_5();
  }
}

void MotherBrain_Phase2_Revive_5(void) {  // 0xA9C0E4
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    MotherBrain_SetBrainInstrs(addr_kMotherBrain_Ilist_9BB3);
    E->mbn_var_A = FUNC16(MotherBrain_Phase2_Revive_6);
    E->mbn_var_F = 128;
    MotherBrain_Phase2_Revive_6();
  }
}

void MotherBrain_Phase2_Revive_6(void) {  // 0xA9C0FB
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  bool v1 = (--E->mbn_var_F & 0x8000) != 0;
  if (v1 && MotherBrain_MakeWalkForwards(4u, 0x50) & 1) {
    E->mbn_var_A = FUNC16(MotherBrain_Phase2_Revive_7);
    E->mbn_var_1F = 2;
    Get_MotherBrain(0x40)->mbn_var_11 = 1;
  }
}

void MotherBrain_Phase2_Revive_7(void) {  // 0xA9C11E
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->mbn_var_13 = 0;
  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  E1->mbn_var_31 = 1;
  E1->mbn_var_32 = 2;
  E1->mbn_var_33 = 4;
  E1->mbn_var_34 = 64;
  E->mbn_var_A = FUNC16(MotherBrain_Phase2_Revive_8);
  MotherBrain_Phase2_Revive_8();
}

void MotherBrain_Phase2_Revive_8(void) {  // 0xA9C147
  if (MotherBrain_MakeHerStandUp() & 1) {
    Get_MotherBrain(0)->mbn_var_A = FUNC16(MotherBrain_Phase2_MurderShitroid_1);
    MotherBrain_MakeWalkForwards(0xA, 0x50);
    MotherBrain_Phase2_MurderShitroid_1();
  }
}

void MotherBrain_Phase2_MurderShitroid_1(void) {  // 0xA9C15C
  MotherBrain_MaybeStandupOrLeanDown();
  if ((random_number & 0x8000) != 0) {
    uint16 v0 = addr_kMotherBrain_Ilist_9DBB;
    if (Get_MotherBrain(0x40)->mbn_var_0A)
      v0 = addr_kMotherBrain_Ilist_9DB1;
    MotherBrain_SetBrainInstrs(v0);
    Enemy_MotherBrain *E = Get_MotherBrain(0);
    E->mbn_var_A = FUNC16(MotherBrain_Phase2_MurderShitroid_2);
    E->mbn_var_F = 64;
  }
}

void MotherBrain_Phase2_MurderShitroid_2(void) {  // 0xA9C182
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0)
    E->mbn_var_A = FUNC16(MotherBrain_Phase2_MurderShitroid_1);
}

void MotherBrain_Phase2_PrepareForFinalShitroid(void) {  // 0xA9C18E
  MotherBrain_MakeHerStandUp();
  MotherBrain_MakeWalkBackwards(0x40, 4u);
}

void MotherBrain_Phase2_ExecuteFinalkShitroid(void) {  // 0xA9C19A
  MotherBrain_SetBrainInstrs(addr_kMotherBrain_Ilist_9DB1);
  Get_MotherBrain(0)->mbn_var_A = FUNC16(nullsub_363);
}

void MotherBrain_MaybeStandupOrLeanDown(void) {  // 0xA9C1A7
  uint16 mbn_var_02 = Get_MotherBrain(0)->mbn_var_02;
  if (mbn_var_02) {
    if (mbn_var_02 == 6 && (uint8)random_number >= 0xC0)
      MotherBrain_MakeHerStandUp();
  } else if ((uint8)random_number >= 0xC0) {
    MotherBrain_MakeHerLeanDown();
  }
}

void MotherBrain_Phase3_Recover_MakeDistance(void) {  // 0xA9C1CF
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->mbn_var_00 = 4;
  E->mbn_var_A = FUNC16(MotherBrain_Phase3_Recover_SetupForFight);
  E->mbn_var_F = 32;
  MotherBrain_MakeWalkBackwards(E->base.x_pos - 14, 2u);
}

void MotherBrain_Phase3_Recover_SetupForFight(void) {  // 0xA9C1F0
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0) {
    E->mbn_var_A = FUNC16(MotherBrain_Phase3_Fighting_Main);
    Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
    E1->mbn_var_18 = FUNC16(MotherBrain_Phase3_Neck_Normal);
    E1->mbn_var_1A = FUNC16(MotherBrain_Phase3_Walk_TryToInchForward);
    MotherBrain_Phase3_Fighting_Main();
  }
}

void MotherBrain_Phase3_Fighting_Main(void) {  // 0xA9C209
  if (Get_MotherBrain(0x40)->base.health) {
    MotherBrain_Phase3_NeckHandler();
    MotherBrain_Phase3_WalkHandler();
    Enemy_MotherBrain *E = Get_MotherBrain(0);
    if (!E->mbn_var_02 && !E->mbn_var_06 && (random_number & 0x8000) != 0) {
      uint16 v1 = addr_kMotherBrain_Ilist_9F00;
      if ((uint8)random_number >= 0x80)
        v1 = addr_kMotherBrain_Ilist_9DBB;
      MotherBrain_SetBrainInstrs(v1);
      E->mbn_var_A = FUNC16(MotherBrain_Phase3_Fighting_Cooldown);
      E->mbn_var_F = 64;
    }
  } else {
    Get_MotherBrain(0)->mbn_var_A = FUNC16(MotherBrain_Phase3_Death_0);
  }
}

void MotherBrain_Phase3_Fighting_Cooldown(void) {  // 0xA9C24E
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((--E->mbn_var_F & 0x8000) != 0)
    E->mbn_var_A = FUNC16(MotherBrain_Phase3_Fighting_Main);
}

void CallMotherBrainWallkFunc1A(uint32 ea) {
  switch (ea) {
  case fnMotherBrain_Phase3_Walk_TryToInchForward: MotherBrain_Phase3_Walk_TryToInchForward(); return;
  case fnMotherBrain_Phase3_Walk_RetreatQuickly: MotherBrain_Phase3_Walk_RetreatQuickly(); return;
  case fnMotherBrain_Phase3_Walk_RetreatSlowly: MotherBrain_Phase3_Walk_RetreatSlowly(); return;
  default: Unreachable();
  }
}

void MotherBrain_Phase3_WalkHandler(void) {  // 0xA9C25A
  if (!Get_MotherBrain(0)->mbn_var_02) {
    R18_ = Get_MotherBrain(0x40)->mbn_var_1A;
    CallMotherBrainWallkFunc1A(R18_ | 0xA90000);
  }
}

void MotherBrain_Phase3_Walk_TryToInchForward(void) {  // 0xA9C26A
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  uint16 mbn_var_07 = E->mbn_var_07;
  if (mbn_var_07) {
    uint16 v3 = mbn_var_07 + 32;
    E->mbn_var_07 = v3;
    if (v3 >= 0x100) {
      uint16 v4 = E->base.x_pos + 1;
      Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
      E1->mbn_var_1B = v4;
      if (MotherBrain_MakeWalkForwards((random_number & 2) + 4, E1->mbn_var_1B) & 1)
        E->mbn_var_07 = 128;
    }
  } else {
    uint16 v6 = E->base.x_pos - 14;
    Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
    E1->mbn_var_1B = v6;
    E1->mbn_var_1A = FUNC16(MotherBrain_Phase3_Walk_RetreatQuickly);
    MotherBrain_Phase3_Walk_RetreatQuickly();
  }
}

void MotherBrain_Phase3_Walk_RetreatQuickly(void) {  // 0xA9C2B3
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  if (MotherBrain_MakeWalkBackwards(E->mbn_var_1B, 2u) & 1) {
    E->mbn_var_1B = Get_MotherBrain(0)->base.x_pos - 14;
    E->mbn_var_1A = FUNC16(MotherBrain_Phase3_Walk_RetreatSlowly);
  }
}

void MotherBrain_Phase3_Walk_RetreatSlowly(void) {  // 0xA9C2D2
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  if (MotherBrain_MakeWalkBackwards(E->mbn_var_1B, 4u) & 1)
    MotherBrain_SetToTryToInchForward(0x40);
}

void MotherBrain_SetToTryToInchForward(uint16 a) {  // 0xA9C313
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->mbn_var_07 = a;
  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  E1->mbn_var_1A = FUNC16(MotherBrain_Phase3_Walk_TryToInchForward);
  E1->mbn_var_1B = E->base.x_pos + 1;
}

void CallMotherBrainNeckFunc(uint32 ea) {
  switch (ea) {
  case fnnullsub_369: return;
  case fnMotherBrain_Phase3_Neck_Normal: MotherBrain_Phase3_Neck_Normal(); return;
  case fnMotherBrain_Phase3_Neck_SetupRecoilRecovery: MotherBrain_Phase3_Neck_SetupRecoilRecovery(); return;
  case fnMotherBrain_Phase3_Neck_RecoilRecovery: MotherBrain_Phase3_Neck_RecoilRecovery(); return;
  case fnMotherBrain_Phase3_Neck_SetupHyperBeamRecoil: MotherBrain_Phase3_Neck_SetupHyperBeamRecoil(); return;
  case fnMotherBrain_Phase3_Neck_HyperBeamRecoil: MotherBrain_Phase3_Neck_HyperBeamRecoil(); return;
  default: Unreachable();
  }
}

void MotherBrain_Phase3_NeckHandler(void) {  // 0xA9C327
  R18_ = Get_MotherBrain(0x40)->mbn_var_18;
  CallMotherBrainNeckFunc(R18_ | 0xA90000);
}

void MotherBrain_Phase3_Neck_Normal(void) {  // 0xA9C330
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  E->mbn_var_32 = 1;
  E->mbn_var_34 = 128;
  E->mbn_var_32 = 2;
  E->mbn_var_33 = 4;
  E->mbn_var_18 = addr_locret_A9C353;
}

void MotherBrain_Phase3_Neck_SetupRecoilRecovery(void) {  // 0xA9C354
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  E->mbn_var_31 = 1;
  E->mbn_var_34 = 1280;
  E->mbn_var_32 = 6;
  E->mbn_var_33 = 6;
  E->mbn_var_18 = FUNC16(MotherBrain_Phase3_Neck_RecoilRecovery);
  E->mbn_var_19 = 16;
  MotherBrain_Phase3_Neck_RecoilRecovery();
}

void MotherBrain_Phase3_Neck_RecoilRecovery(void) {  // 0xA9C37B
  int16 v1;

  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  v1 = E->mbn_var_19 - 1;
  if (v1 < 0) {
    MotherBrain_SetBrainInstrs(addr_kMotherBrain_Ilist_9DBB);
    E->mbn_var_18 = FUNC16(MotherBrain_Phase3_Neck_Normal);
  } else {
    E->mbn_var_19 = v1;
  }
}

void MotherBrain_Phase3_Neck_SetupHyperBeamRecoil(void) {  // 0xA9C395
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  E->mbn_var_31 = 1;
  Get_MotherBrain(0)->mbn_var_06 = 1;
  MotherBrain_SetBrainInstrs(addr_kMotherBrain_Ilist_9BE7);
  E->mbn_var_00 = 50;
  E->mbn_var_34 = 2304;
  E->mbn_var_32 = 8;
  E->mbn_var_33 = 8;
  E->mbn_var_18 = FUNC16(MotherBrain_Phase3_Neck_HyperBeamRecoil);
  E->mbn_var_19 = 11;
  MotherBrain_Phase3_Neck_HyperBeamRecoil();
}

void MotherBrain_Phase3_Neck_HyperBeamRecoil(void) {  // 0xA9C3CD
  int16 v2;

  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  v2 = E->mbn_var_19 - 1;
  if (v2 < 0) {
    E->mbn_var_34 = 128;
    Get_MotherBrain(0)->mbn_var_06 = 0;
    E->mbn_var_18 = FUNC16(MotherBrain_Phase3_Neck_SetupRecoilRecovery);
  } else {
    E->mbn_var_19 = v2;
  }
}

void MoveEnemyWithVelocity(void) {  // 0xA9C3EF
  EnemyData *v0 = gEnemyData(cur_enemy_index);
  uint16 ai_var_B = v0->ai_var_B;
  int carry = HIBYTE(v0->x_subpos) + LOBYTE(ai_var_B);
  HIBYTE(v0->x_subpos) = carry;
  v0->x_pos += (int8)(ai_var_B >> 8) + (carry >> 8);

  uint16 ai_var_C = v0->ai_var_C;
  carry = HIBYTE(v0->y_subpos) + LOBYTE(ai_var_C);
  HIBYTE(v0->y_subpos) = carry;
  v0->y_pos += (int8)(ai_var_C >> 8) + (carry >> 8);
}

void MotherBrain_SetBodyInstrs(uint16 a) {  // 0xA9C42D
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->base.current_instruction = a;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
}

void MotherBrain_SetBrainUnusedInstrs(uint16 a) {  // 0xA9C43A
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  E->base.current_instruction = a;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
}

void MotherBrain_SetBrainInstrs(uint16 a) {  // 0xA9C447
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->mbn_var_21 = a;
  E->mbn_var_20 = 1;
}

void Enemy_SetInstrList(uint16 k, uint16 a) {  // 0xA9C453
  EnemyData *v2 = gEnemyData(k);
  v2->current_instruction = a;
  v2->instruction_timer = 1;
  v2->timer = 0;
}

uint16 ComputeSinMult(uint16 a) {  // 0xA9C460
  return sub_A9C46C(R18_, a);
}

uint16 ComputeCosMult(uint16 a) {  // 0xA9C465
  return sub_A9C46C(R18_ + 64, a);
}

uint16 sub_A9C46C(uint16 a, uint16 j) {  // 0xA9C46C
  uint16 v2 = kSinCosTable8bit_Sext[((uint16)((2 * a) & 0x1FE) >> 1) + 64];
  WriteReg(M7A, v2);
  WriteReg(M7A, HIBYTE(v2));
  WriteReg(M7B, j);
  return ReadRegWord(MPYM);
}

void Enemy_IncreaseYpos(uint16 k, uint16 a) {  // 0xA9C4A9
  EnemyData *v2 = gEnemyData(k);
  int carry = HIBYTE(v2->y_subpos) + LOBYTE(a);
  HIBYTE(v2->y_subpos) = carry;
  v2->y_pos += (int8)(a >> 8) + (carry >> 8);
}

void Samus_DecrementAmmoDueToRainbowBeam(void) {  // 0xA9C4C4
  if ((enemy_damage_routine_exec_count & 3) == 0 && samus_missiles) {
    uint16 v0 = samus_missiles - g_word_A9C544;
    if (sign16(samus_missiles - g_word_A9C544 - 1)) {
      if (hud_item_index == 1)
        hud_item_index = 0;
      v0 = 0;
      samus_auto_cancel_hud_item_index = 0;
    }
    samus_missiles = v0;
  }
  if ((enemy_damage_routine_exec_count & 3) == 0 && samus_super_missiles) {
    uint16 v1 = samus_super_missiles - g_word_A9C544;
    if (sign16(samus_super_missiles - g_word_A9C544 - 1)) {
      if (hud_item_index == 2)
        hud_item_index = 0;
      v1 = 0;
      samus_auto_cancel_hud_item_index = 0;
    }
    samus_super_missiles = v1;
  }
  if (samus_power_bombs) {
    uint16 v2 = samus_power_bombs - g_word_A9C544;
    if (sign16(samus_power_bombs - g_word_A9C544 - 1)) {
      if (hud_item_index == 3)
        hud_item_index = 0;
      v2 = 0;
      samus_auto_cancel_hud_item_index = 0;
    }
    samus_power_bombs = v2;
  }
}

void Samus_PlayGainingLosingHealthSfx(void) {  // 0xA9C546
  if (!sign16(samus_health - 81) && (enemy_damage_routine_exec_count & 7) == 0)
    QueueSfx3_Max3(0x2Du);
}

void Samus_DamageDueToShitroid(void) {  // 0xA9C560
  int16 v0;

  v0 = -4;
  if (equipped_items & 1)
    v0 = -3;
  uint16 v1 = samus_health + (equipped_items & 1) + v0;
  if (sign16(v1 - 2))
    v1 = 1;
  samus_health = v1;
}

void Samus_DamageDueToRainbowBeam(void) {  // 0xA9C57D
  uint16 v0 = samus_health + (equipped_items & 1) - 2;
  if (sign16(v0 - 1))
    v0 = 0;
  samus_health = v0;
  Samus_PlayGainingLosingHealthSfx();
}

uint8 Samus_HealDueToShitroid(void) {  // 0xA9C59F
  if ((int16)(samus_health + 1 - samus_max_health) < 0) {
    ++samus_health;
    Samus_PlayGainingLosingHealthSfx();
    return 1;
  } else {
    samus_health = samus_max_health;
    Samus_PlayGainingLosingHealthSfx();
    return 0;
  }
}

uint8 ProcessSpriteTilesTransfers(uint8 db, uint16 k) {  // 0xA9C5BE
  VramWriteEntry *v7;

  Enemy_MotherBrain *E = Get_MotherBrain(0);
  uint16 mbn_var_22 = E->mbn_var_22;
  if (!mbn_var_22)
    mbn_var_22 = k;
  uint16 v3 = mbn_var_22;
  uint16 v4 = vram_write_queue_tail;
  uint8 *v5 = RomPtrWithBank(db, mbn_var_22);
  uint16 v6 = *(uint16 *)v5;
  if (*(uint16 *)v5) {
    v7 = gVramWriteEntry(vram_write_queue_tail);
    v7->size = v6;
    *(VoidP *)((char *)&v7->src.addr + 1) = *(uint16 *)(v5 + 3);
    v7->src.addr = *((uint16 *)v5 + 1);
    v7->vram_dst = *(uint16 *)(v5 + 5);
    vram_write_queue_tail = v4 + 7;
    E->mbn_var_22 = v3 + 7;
    v6 = *(uint16 *)RomPtrWithBank(db, E->mbn_var_22);
    if (v6)
      return 0;
  }
  E->mbn_var_22 = v6;
  return 1;
}

uint8 MotherBrain_MakeWalkForwards(uint16 j, uint16 a) {  // 0xA9C601
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((int16)(a - E->base.x_pos) >= 0) {
    if (E->mbn_var_02)
      return 0;
    if (sign16(E->base.x_pos - 128)) {
      MotherBrain_SetBodyInstrs(g_off_A9C61E[j >> 1]);
      return 0;
    }
  }
  return 1;
}

uint8 MotherBrain_MakeWalkBackwards(uint16 a, uint16 j) {  // 0xA9C647
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if ((int16)(a - E->base.x_pos) < 0) {
    if (E->mbn_var_02)
      return 0;
    if (!sign16(E->base.x_pos - 48)) {
      MotherBrain_SetBodyInstrs(g_off_A9C664[j >> 1]);
      return 0;
    }
  }
  return 1;
}

uint8 MotherBrain_MakeHerStandUp(void) {  // 0xA9C670
  uint16 mbn_var_02 = Get_MotherBrain(0)->mbn_var_02;
  if (mbn_var_02) {
    uint16 v1 = addr_kMotherBrain_Ilist_99C6;
    if (mbn_var_02 != 3) {
      if (mbn_var_02 != 6)
        return 0;
      v1 = addr_kMotherBrain_Ilist_99E2;
    }
    MotherBrain_SetBodyInstrs(v1);
    return 0;
  }
  return 1;
}

uint8 MotherBrain_MakeHerCrouch(void) {  // 0xA9C68E
  uint16 mbn_var_02 = Get_MotherBrain(0)->mbn_var_02;
  if (mbn_var_02) {
    if (mbn_var_02 == 3)
      return 1;
  } else {
    MotherBrain_SetBodyInstrs(addr_kMotherBrain_Ilist_9A0A);
  }
  return 0;
}

uint8 MotherBrain_MakeHerLeanDown(void) {  // 0xA9C6A3
  uint16 mbn_var_02 = Get_MotherBrain(0)->mbn_var_02;
  if (mbn_var_02) {
    if (mbn_var_02 == 6)
      return 1;
  } else {
    MotherBrain_SetBodyInstrs(addr_kMotherBrain_Ilist_99F2);
  }
  return 0;
}

void MotherBrain_HandleWalking(void) {  // 0xA9C6B8
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if (!E->mbn_var_02) {
    uint16 mbn_var_07 = E->mbn_var_07;
    if (mbn_var_07) {
      uint16 v2;
      v2 = mbn_var_07 + 6;
      E->mbn_var_07 = v2;
      if (v2 >= 0x100) {
LABEL_4:
        E->mbn_var_07 = 128;
        if (sign16(E->base.x_pos - 128))
          MotherBrain_SetBodyInstrs(addr_kMotherBrain_Ilist_97A4);
        return;
      }
      if (!sign16(E->base.x_pos - 48))
        return;
    } else {
      E->mbn_var_07 = 1;
      if (!sign16(E->base.x_pos - 48)) {
        MotherBrain_SetBodyInstrs(addr_kMotherBrain_Ilist_98C6);
        return;
      }
    }
    if (sign16((random_number & 0xFFF) - 4032))
      return;
    goto LABEL_4;
  }
}

void ShitroidInCutscene_Init(void) {  // 0xA9C710
  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(cur_enemy_index);
  E->base.properties |= kEnemyProps_DisableSamusColl | kEnemyProps_BlockPlasmaBeam;
  E->base.palette_index = 3584;
  E->base.current_instruction = addr_kShitroid_Ilist_CFA2;
  E->base.instruction_timer = 1;
  E->sice_var_04 = 1;
  E->base.timer = 0;
  E->sice_var_E = 10;
  E->base.vram_tiles_index = 160;
  E->base.x_pos = 320;
  E->base.y_pos = 96;
  E->sice_var_B = 0;
  E->sice_var_C = 0;
  E->sice_var_09 = 0;
  E->sice_var_A = FUNC16(ShitroidInCutscene_DashOntoScreen);
  E->sice_var_F = 248;
  E->sice_var_0F = FUNC16(Shitroid_HandleCutscenePalette);
  WriteColorsToPalette(0x1E2, 0xa9, addr_kMotherBrainPalette_2 + 2, 0xF);
}

void CallShitroidCutsceneFuncA(uint32 ea, uint16 k) {
  switch (ea) {
  case fnnullsub_367: return;
  case fnShitroidInCutscene_DashOntoScreen: ShitroidInCutscene_DashOntoScreen(k); return;
  case fnShitroidInCutscene_CurveTowardsBrain: ShitroidInCutscene_CurveTowardsBrain(k); return;
  case fnShitroidInCutscene_GetIntoFace: ShitroidInCutscene_GetIntoFace(k); return;
  case fnShitroidInCutscene_LatchOntoBrain: ShitroidInCutscene_LatchOntoBrain(k); return;
  case fnShitroidInCutscene_SetMotherBrainToStumbleBack: ShitroidInCutscene_SetMotherBrainToStumbleBack(k); return;
  case fnShitroidInCutscene_ActivateRainbowBeam: ShitroidInCutscene_ActivateRainbowBeam(k); return;
  case fnShitroidInCutscene_BrainTurnsToCorpse: ShitroidInCutscene_BrainTurnsToCorpse(k); return;
  case fnShitroidInCutscene_StopDraining: ShitroidInCutscene_StopDraining(k); return;
  case fnShitroidInCutscene_LetGoAndSpawnDust: ShitroidInCutscene_LetGoAndSpawnDust(k); return;
  case fnShitroidInCutscene_MoveUpToCeiling: ShitroidInCutscene_MoveUpToCeiling(k); return;
  case fnShitroidInCutscene_MoveToSamus: ShitroidInCutscene_MoveToSamus(k); return;
  case fnShitroidInCutscene_LatchOntoSamus: ShitroidInCutscene_LatchOntoSamus(k); return;
  case fnShitroidInCutscene_HealSamusToFullHealth: ShitroidInCutscene_HealSamusToFullHealth(k); return;
  case fnShitroidInCutscene_IdleUntilToNoHealth: ShitroidInCutscene_IdleUntilToNoHealth(k); return;
  case fnShitroidInCutscene_ReleaseSamus: ShitroidInCutscene_ReleaseSamus(k); return;
  case fnShitroidInCutscene_StareDownMotherBrain: ShitroidInCutscene_StareDownMotherBrain(k); return;
  case fnShitroidInCutscene_FlyOffScreen: ShitroidInCutscene_FlyOffScreen(k); return;
  case fnShitroidInCutscene_MoveToFinalChargeStart: ShitroidInCutscene_MoveToFinalChargeStart(k); return;
  case fnShitroidInCutscene_InitiateFinalCharge: ShitroidInCutscene_InitiateFinalCharge(k); return;
  case fnShitroidInCutscene_FinalCharge: ShitroidInCutscene_FinalCharge(k); return;
  case fnShitroidInCutscene_ShitroidFinalBelow: ShitroidInCutscene_ShitroidFinalBelow(k); return;
  case fnShitroidInCutscene_PlaySamusTheme: ShitroidInCutscene_PlaySamusTheme(k); return;
  case fnShitroidInCutscene_PrepareSamusHyperbeam: ShitroidInCutscene_PrepareSamusHyperbeam(k); return;
  case fnShitroidInCutscene_DeathSequence: ShitroidInCutscene_DeathSequence(k); return;
  case fnShitroidInCutscene_UnloadShitroid: ShitroidInCutscene_UnloadShitroid(k); return;
  case fnShitroidInCutscene_LetSamusRainbowMore: ShitroidInCutscene_LetSamusRainbowMore(k); return;
  case fnShitroidInCutscene_FinishCutscene: ShitroidInCutscene_FinishCutscene(k); return;
  default: Unreachable();
  }
}

void CallShitroidCutsceneFunc0F(uint32 ea) {
  switch (ea) {
  case fnnullsub_368: return;
  case fnnullsub_338: return;
  case fnShitroid_HandleCutscenePalette: Shitroid_HandleCutscenePalette(); return;
  case fnShitroid_HandleCutscenePalette_LowHealth: Shitroid_HandleCutscenePalette_LowHealth(); return;
  default: Unreachable();
  }
}

void ShitroidInCutscene_Main(void) {  // 0xA9C779
  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(cur_enemy_index);
  E->base.shake_timer = 0;
  CallShitroidCutsceneFuncA(E->sice_var_A | 0xA90000, cur_enemy_index);
  MoveEnemyWithVelocity();
  ShitroidInCutscene_Flashing(cur_enemy_index);
  ShitroidInCutscene_HandleHealthBasedPalette(cur_enemy_index);
  R18_ = Get_ShitdroidInCutscene(cur_enemy_index)->sice_var_0F;
  CallShitroidCutsceneFunc0F(R18_ | 0xA90000);
}

void ShitroidInCutscene_Flashing(uint16 k) {  // 0xA9C79C
  int16 v1;

  v1 = 3584;
  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(k);
  uint16 sice_var_06 = E->sice_var_06;
  if (sice_var_06) {
    uint16 v4 = sice_var_06 - 1;
    E->sice_var_06 = v4;
    if ((v4 & 2) != 0)
      v1 = 0;
  }
  E->base.palette_index = v1;
}

void ShitroidInCutscene_HandleCry(void) {  // 0xA9C7B7
  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(0);
  if (E->sice_var_14) {
    E->sice_var_14 = 0;
    QueueSfx2_Max6(0x72u);
  }
}

void ShitroidInCutscene_DashOntoScreen(uint16 k) {  // 0xA9C7CC
  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(k);
  if ((--E->sice_var_F & 0x8000) != 0) {
    E->sice_var_0A = -10240;
    E->sice_var_0B = 2560;
    E->sice_var_A = FUNC16(ShitroidInCutscene_CurveTowardsBrain);
    E->sice_var_F = 10;
    ShitroidInCutscene_CurveTowardsBrain(k);
  }
}

void ShitroidInCutscene_CurveTowardsBrain(uint16 k) {  // 0xA9C7EC
  R18_ = -384;
  R20_ = -20480;
  R22_ = 2560;
  ShitroidInCutscene_UpdateSpeedAndAngle(k);
  Enemy_ShitroidInCutscene *E = Get_ShitroidInCutscene(k);
  if ((--E->sice_var_F & 0x8000) != 0) {
    E->sice_var_A = FUNC16(ShitroidInCutscene_GetIntoFace);
    E->sice_var_F = 9;
  }
}

void ShitroidInCutscene_GetIntoFace(uint16 k) {  // 0xA9C811
  bool v3; // sf

  R18_ = -1536;
  R20_ = -32256;
  R22_ = 3584;
  ShitroidInCutscene_UpdateSpeedAndAngle(k);
  Enemy_ShitroidInCutscene *E = Get_ShitroidInCutscene(0x40);
  R18_ = E->base.x_pos;
  R20_ = E->base.y_pos;
  R22_ = 4;
  R24_ = 4;
  if (!(Shitroid_Func_2(k) & 1)
      || (E = Get_ShitroidInCutscene(k), v3 = (int16)(E->sice_var_F - 1) < 0, --E->sice_var_F, v3)) {
    Get_ShitroidInCutscene(k)->sice_var_A = FUNC16(ShitroidInCutscene_LatchOntoBrain);
    SomeMotherBrainScripts(1u);
  }
}

void ShitroidInCutscene_LatchOntoBrain(uint16 k) {  // 0xA9C851
  Enemy_ShitroidInCutscene *E = Get_ShitroidInCutscene(0x40);
  R18_ = E->base.x_pos;
  R20_ = E->base.y_pos - 24;
  Shitroid_Func_GraduallyAccelerateTowards0x400(k, 0);
  R22_ = 8;
  R24_ = 8;
  if (!(Shitroid_Func_2(k) & 1))
    Get_ShitroidInCutscene(k)->sice_var_A = FUNC16(ShitroidInCutscene_SetMotherBrainToStumbleBack);
}

void ShitroidInCutscene_SetMotherBrainToStumbleBack(uint16 k) {  // 0xA9C879
  Enemy_ShitroidInCutscene *E = Get_ShitroidInCutscene(0);
  MotherBrain_MakeWalkBackwards(E->base.x_pos - 1, 2u);
  Get_ShitroidInCutscene(k)->sice_var_A = FUNC16(ShitroidInCutscene_ActivateRainbowBeam);
  ShitroidInCutscene_ActivateRainbowBeam(k);
}

void ShitroidInCutscene_ActivateRainbowBeam(uint16 k) {  // 0xA9C889
  Enemy_ShitroidInCutscene *E1 = Get_ShitdroidInCutscene(0x40);
  R18_ = E1->base.x_pos;
  R20_ = E1->base.y_pos - 24;
  if (Shitroid_AccelerateTowardsPoint(k, 0x200) & 1) {
    Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(k);
    E->sice_var_B = 0;
    E->sice_var_C = 0;
    E->base.x_pos = E1->base.x_pos;
    E->base.y_pos = E1->base.y_pos - 24;
    Enemy_SetInstrList(k, addr_kShitroid_Ilist_CFB8);
    E->sice_var_A = FUNC16(ShitroidInCutscene_BrainTurnsToCorpse);
    E->sice_var_E = 1;
    Enemy_ShitroidInCutscene *E0 = Get_ShitdroidInCutscene(0);
    E0->sice_var_A = FUNC16(MotherBrain_DrainedByShitroid_0);
    QueueSfx1_Max6(0x40);
    E0->sice_var_16 = 1;
  }
}

void ShitroidInCutscene_BrainTurnsToCorpse(uint16 k) {  // 0xA9C8E2
  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(k);
  Enemy_ShitroidInCutscene *E1 = Get_ShitdroidInCutscene(0x40);

  int v2 = (uint8)(E->base.frame_counter & 6) >> 1;
  E->base.x_pos = E1->base.x_pos + g_word_A993BB[v2];
  E->base.y_pos = E1->base.y_pos + g_word_A993C3[v2] - 24;
  if (Get_ShitdroidInCutscene(0)->sice_var_1F) {
    E->sice_var_A = FUNC16(ShitroidInCutscene_StopDraining);
    E->sice_var_F = 64;
  }
}

void ShitroidInCutscene_StopDraining(uint16 k) {  // 0xA9C915
  Enemy_ShitroidInCutscene *E = Get_ShitroidInCutscene(k);
  Enemy_ShitroidInCutscene *E1 = Get_ShitroidInCutscene(0x40);
  E->base.x_pos = E1->base.x_pos;
  E->base.y_pos = E1->base.y_pos - 24;
  if ((--E->sice_var_F & 0x8000) != 0) {
    Enemy_SetInstrList(k, addr_kShitroid_Ilist_CFA2);
    E->sice_var_E = 10;
    E->sice_var_A = FUNC16(ShitroidInCutscene_LetGoAndSpawnDust);
    E->sice_var_F = 32;
    E->sice_var_B = 0;
    E->sice_var_C = 0;
  }
}

void ShitroidInCutscene_LetGoAndSpawnDust(uint16 k) {  // 0xA9C94B
  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(k);
  if ((--E->sice_var_F & 0x8000) != 0) {
    ShitroidInCutscene_SpawnThreeDustClouds();
    E->sice_var_A = addr_loc_A9C959;
  }
  ShitroidInCutscene_MoveUpToCeiling(k);
}

void ShitroidInCutscene_MoveUpToCeiling(uint16 k) {  // 0xA9C959
  R18_ = Get_ShitdroidInCutscene(0x40)->base.x_pos;
  R20_ = 0;
  Shitroid_Func_GraduallyAccelerateTowards0x400(k, 0);
  R22_ = 4;
  R24_ = 4;
  if (!(Shitroid_Func_2(k) & 1)) {
    SomeMotherBrainScripts(4u);
    Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(k);
    E->sice_var_A = FUNC16(ShitroidInCutscene_MoveToSamus);
    E->sice_var_0E = addr_stru_A9CA24;
  }
}

void ShitroidInCutscene_SpawnThreeDustClouds(void) {  // 0xA9C98C
  R20_ = -8;
  ShitroidInCutscene_SpawnOneDustCloudAt(0xFFF0);
  R20_ = -16;
  ShitroidInCutscene_SpawnOneDustCloudAt(0);
  R20_ = -8;
  ShitroidInCutscene_SpawnOneDustCloudAt(0x10);
}

void ShitroidInCutscene_SpawnOneDustCloudAt(uint16 a) {  // 0xA9C9AA
  Enemy_ShitroidInCutscene *E = Get_ShitroidInCutscene(0x40);
  R18_ = E->base.x_pos + a;
  R20_ += E->base.y_pos;
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 9u);
}

void CallShitroidMoveFunc(uint32 ea, uint16 k, uint16 j) {
  switch (ea) {
  case fnShitroid_GraduallyAccelerateTowards0x8: Shitroid_GraduallyAccelerateTowards0x8(k, j); return;
  case fnShitroid_GraduallyAccelerateTowards0x10: Shitroid_GraduallyAccelerateTowards0x10(k, j); return;
  default: Unreachable();
  }
}

void ShitroidInCutscene_MoveToSamus(uint16 k) {  // 0xA9C9C3
  int16 v4;

  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(k);
  E->sice_var_04 = 0;
  E->sice_var_09 = 1;
  if ((random_number & 0xFFFu) >= 0xFA0)
    QueueSfx2_Max6(0x52u);
  uint16 sice_var_0E = E->sice_var_0E;
  uint16 *v3 = (uint16 *)RomPtr_A9(sice_var_0E);
  R18_ = *v3;
  R20_ = v3[1];
  R22_ = v3[3];
  CallShitroidMoveFunc(R22_ | 0xA90000, k, v3[2]);
  R22_ = 4;
  R24_ = 4;
  if (!(Shitroid_Func_2(k) & 1)) {
    v4 = *((uint16 *)RomPtr_A9(sice_var_0E) + 4);
    if (v4 < 0) {
      E->sice_var_A = v4;
    } else {
      E->sice_var_0E += 8;
    }
  }
}

void ShitroidInCutscene_LatchOntoSamus(uint16 k) {  // 0xA9CA66
  R18_ = samus_x_pos;
  R20_ = samus_y_pos - 20;
  Shitroid_Func_GraduallyAccelerateTowards0x400(k, 0);
}

void ShitroidInCutscene_HealSamusToFullHealth(uint16 k) {  // 0xA9CA7A
  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(k);
  E->sice_var_04 = 0;
  ShitroidInCutscene_HandleCry();
  int v2 = (uint8)(E->base.frame_counter & 6) >> 1;
  E->base.x_pos = samus_x_pos + g_word_A993BB[v2];
  E->base.y_pos = samus_y_pos + g_word_A993C3[v2] - 20;
  if (!(Samus_HealDueToShitroid() & 1)) {
    samus_reserve_health = samus_max_reserve_health;
    E->sice_var_A = FUNC16(ShitroidInCutscene_IdleUntilToNoHealth);
    E->sice_var_0F = FUNC16(nullsub_368);
  }
}

void ShitroidInCutscene_IdleUntilToNoHealth(uint16 k) {  // 0xA9CABD
  ShitroidInCutscene_HandleCry();
  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(k);
  uint16 sice_var_06 = E->sice_var_06;
  if (sice_var_06) {
    int v3 = (uint8)(sice_var_06 & 6) >> 1;
    E->base.x_pos = samus_x_pos + 2 * g_word_A993BB[v3];
    E->base.y_pos = samus_y_pos + 2 * g_word_A993C3[v3] - 20;
  }
  if (!E->base.health) {
    E->base.health = 320;
    E->sice_var_A = FUNC16(ShitroidInCutscene_ReleaseSamus);
    E->sice_var_E = 10;
    E->sice_var_D = 0;
    E->sice_var_0F = FUNC16(Shitroid_HandleCutscenePalette_LowHealth);
    E->sice_var_04 = 1;
    E->sice_var_09 = 0;
  }
}

void ShitroidInCutscene_ReleaseSamus(uint16 k) {  // 0xA9CB13
  QueueSfx2_Max6(0x72u);
  Get_ShitdroidInCutscene(0x40)->sice_var_0B = 1;
  Get_ShitdroidInCutscene(k)->sice_var_A = FUNC16(ShitroidInCutscene_StareDownMotherBrain);
  Get_ShitdroidInCutscene(0)->sice_var_A = FUNC16(MotherBrain_Phase2_PrepareForFinalShitroid);
  ShitroidInCutscene_StareDownMotherBrain(k);
}

void ShitroidInCutscene_StareDownMotherBrain(uint16 k) {  // 0xA9CB2D
  R18_ = samus_x_pos - 4;
  R20_ = 96;
  Shitroid_GraduallyAccelerateTowards0x10(k, 0);
  R22_ = 4;
  R24_ = 4;
  if (!(Shitroid_Func_2(k) & 1))
    Get_ShitroidInCutscene(k)->sice_var_A = FUNC16(ShitroidInCutscene_FlyOffScreen);
}

void ShitroidInCutscene_FlyOffScreen(uint16 k) {  // 0xA9CB56
  R18_ = 272;
  R20_ = 64;
  Shitroid_GraduallyAccelerateTowards0x10(k, 0);
  R22_ = 4;
  R24_ = 4;
  if (!(Shitroid_Func_2(k) & 1))
    Get_ShitroidInCutscene(k)->sice_var_A = FUNC16(ShitroidInCutscene_MoveToFinalChargeStart);
}

void ShitroidInCutscene_MoveToFinalChargeStart(uint16 k) {  // 0xA9CB7B
  R18_ = 305;
  R20_ = 160;
  Shitroid_GraduallyAccelerateTowards0x10(k, 0);
  R22_ = 4;
  R24_ = 4;
  if (!(Shitroid_Func_2(k) & 1)) {
    Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(k);
    E->base.health = 79;
    Get_ShitdroidInCutscene(0x40)->sice_var_0B = 0;
    Get_ShitdroidInCutscene(0)->sice_var_A = FUNC16(MotherBrain_Phase2_ExecuteFinalkShitroid);
    E->sice_var_A = FUNC16(ShitroidInCutscene_InitiateFinalCharge);
  }
}

void ShitroidInCutscene_InitiateFinalCharge(uint16 k) {  // 0xA9CBB3
  R18_ = 290;
  R20_ = 128;
  Shitroid_Func_GraduallyAccelerateTowards0x400(k, 0xA);
  R22_ = 4;
  R24_ = 4;
  if (!(Shitroid_Func_2(k) & 1))
    Get_ShitroidInCutscene(k)->sice_var_A = FUNC16(ShitroidInCutscene_FinalCharge);
}

void ShitroidInCutscene_FinalCharge(uint16 k) {  // 0xA9CBD8
  Enemy_ShitroidInCutscene *E1 = Get_ShitdroidInCutscene(0x40);
  R18_ = E1->base.x_pos;
  R20_ = E1->base.y_pos - 32;
  Shitroid_Func_GraduallyAccelerateTowards0x400(k, 0xC);
  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(k);
  if (!E->base.health) {
    E->sice_var_0F = FUNC16(nullsub_338);
    E->base.vram_tiles_index = 4256;
    QueueSfx3_Max6(0x19u);
    TurnOffLightsForShitroidDeath();
    Enemy_SetInstrList(k, addr_kShitroid_Ilist_CFCE);
    E->sice_var_B = 0;
    E->sice_var_C = 0;
    Get_ShitdroidInCutscene(0)->sice_var_A = addr_locret_A9C18D;
    QueueMusic_Delayed8(0);
    E->sice_var_A = FUNC16(ShitroidInCutscene_ShitroidFinalBelow);
    E->sice_var_F = 16;
    E->sice_var_10 = E->base.x_pos;
    E->sice_var_11 = E->base.y_pos;
    ShitroidInCutscene_ShitroidFinalBelow(k);
  }
}

void ShitroidInCutscene_ShitroidFinalBelow(uint16 k) {  // 0xA9CC3E
  ShitroidInCutscene_Shake(k);
  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(k);
  if ((--E->sice_var_F & 0x8000) != 0) {
    E->base.x_pos = E->sice_var_10;
    E->base.y_pos = E->sice_var_11;
    E->sice_var_A = FUNC16(ShitroidInCutscene_PlaySamusTheme);
    E->sice_var_F = 56;
    ShitroidInCutscene_PlaySamusTheme(k);
  }
}

void ShitroidInCutscene_PlaySamusTheme(uint16 k) {  // 0xA9CC60
  Enemy_ShitroidInCutscene *E = Get_ShitroidInCutscene(k);
  if ((--E->sice_var_F & 0x8000) != 0) {
    QueueMusic_Delayed8(0xFF48u);
    QueueMusic_Delayed8(5u);
    E->sice_var_A = FUNC16(ShitroidInCutscene_PrepareSamusHyperbeam);
    E->sice_var_F = 12;
    ShitroidInCutscene_PrepareSamusHyperbeam(k);
  }
}

void ShitroidInCutscene_PrepareSamusHyperbeam(uint16 k) {  // 0xA9CC7F
  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(k);
  if ((--E->sice_var_F & 0x8000) != 0) {
    CallSomeSamusCode(0x19u);
    Get_ShitdroidInCutscene(0x40)->sice_var_16 = FUNC16(SamusRainbowPaletteFunc_ActivateWhenEnemyLow);
    E->sice_var_A = FUNC16(ShitroidInCutscene_DeathSequence);
  }
}

void ShitroidInCutscene_DeathSequence(uint16 k) {  // 0xA9CC99
  HandleSamusRainbowPaletteAnimation(k);
  ShitroidInCutscene_AccelerateDownwards(k);
  if (ShitroidInCutscene_FadeShitroidToBlack(k) & 1) {
    Enemy_ShitroidInCutscene *E = Get_ShitroidInCutscene(k);
    E->base.properties |= kEnemyProps_Invisible;
    E->sice_var_A = FUNC16(ShitroidInCutscene_UnloadShitroid);
    E->sice_var_F = 128;
  } else {
    ShitroidInCutscene_HandleShitroidDeathExplosions(k);
    ShitroidInCutscene_HandleEnemyBlinking(k);
  }
}

void ShitroidInCutscene_UnloadShitroid(uint16 k) {  // 0xA9CCC0
  HandleSamusRainbowPaletteAnimation(k);
  Enemy_ShitroidInCutscene *E = Get_ShitroidInCutscene(k);
  bool v2 = (--E->sice_var_F & 0x8000) != 0;
  if (v2 && ProcessSpriteTilesTransfers(0xa9, addr_stru_A98FC7)) {
    E->sice_var_A = FUNC16(ShitroidInCutscene_LetSamusRainbowMore);
    E->sice_var_F = 176;
    ShitroidInCutscene_LetSamusRainbowMore(k);
  }
}

void ShitroidInCutscene_LetSamusRainbowMore(uint16 k) {  // 0xA9CCDE
  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(k);
  if ((--E->sice_var_F & 0x8000) != 0) {
    E->sice_var_A = FUNC16(ShitroidInCutscene_FinishCutscene);
    Get_ShitdroidInCutscene(0)->sice_var_37 = 0;
    ShitroidInCutscene_FinishCutscene(k);
  }
}

void ShitroidInCutscene_FinishCutscene(uint16 k) {  // 0xA9CCF0
  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(0);
  uint16 v2 = E->sice_var_37 + 1;
  E->sice_var_37 = v2;
  if (MotherBrain_Phase3_TurnLightsBackOn(v2 - 1) & 1) {
    E->sice_var_A = FUNC16(MotherBrain_Phase3_Recover_MakeDistance);
    CallSomeSamusCode(0x17u);
    SomeMotherBrainScripts(3u);
    Get_ShitdroidInCutscene(k)->base.properties |= kEnemyProps_Deleted;
    Get_ShitdroidInCutscene(0x40)->sice_var_0A = 0;
  }
}

void CallSamusRainbowPaletteFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnSamusRainbowPaletteFunc_ActivateWhenEnemyLow: SamusRainbowPaletteFunc_ActivateWhenEnemyLow(k); return;
  case fnSamusRainbowPaletteFunc_GraduallySlowDown: SamusRainbowPaletteFunc_GraduallySlowDown(); return;
  default: Unreachable();
  }
}

void HandleSamusRainbowPaletteAnimation(uint16 k) {  // 0xA9CD27
  R18_ = Get_ShitdroidInCutscene(0x40)->sice_var_16;
  CallSamusRainbowPaletteFunc(R18_ | 0xA90000, k);
}

void SamusRainbowPaletteFunc_ActivateWhenEnemyLow(uint16 k) {  // 0xA9CD30
  if ((int16)(Get_ShitdroidInCutscene(k)->base.y_pos + 16 - samus_y_pos) >= 0) {
    CallSomeSamusCode(0x16u);
    Get_ShitdroidInCutscene(0x40)->sice_var_16 = FUNC16(SamusRainbowPaletteFunc_GraduallySlowDown);
  }
}

void SamusRainbowPaletteFunc_GraduallySlowDown(void) {  // 0xA9CD4B
  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(0x40);
  uint16 sice_var_1D = E->sice_var_1D;
  E->sice_var_1D = sice_var_1D + 768;
  if (sice_var_1D >= 0xFD00) {
    uint16 v2 = special_samus_palette_frame + 1;
    if (!sign16(special_samus_palette_frame - 9))
      v2 = 10;
    special_samus_palette_frame = v2;
  }
}

uint8 ShitroidInCutscene_FadeShitroidToBlack(uint16 k) {  // 0xA9CD69
  int16 v2;

  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(k);
  if (sign16(E->base.y_pos - 128))
    return 0;
  v2 = E->sice_var_0C - 1;
  if (v2 >= 0) {
    E->sice_var_0C = v2;
    return 0;
  }
  E->sice_var_0C = 8;
  uint16 v4 = E->sice_var_0D + 1;
  if (!sign16(E->sice_var_0D - 6))
    return 1;
  E->sice_var_0D = v4;
  WriteColorsToPalette(0x1E2, 0xad, kShitroid_FadingToBlack[v4], 0xE);
  return 0;
}

void ShitroidInCutscene_HandleShitroidDeathExplosions(uint16 k) {  // 0xA9CDB1
  int16 v2;

  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(k);
  v2 = E->sice_var_08 - 1;
  if (v2 < 0) {
    E->sice_var_08 = 4;
    uint16 v3 = E->sice_var_07 + 1;
    if (!sign16(E->sice_var_07 - 9))
      v3 = 0;
    E->sice_var_07 = v3;
    int v4 = (uint16)(4 * v3) >> 1;
    R18_ = E->base.x_pos + g_word_A9CDFC[v4];
    R20_ = E->base.y_pos + g_word_A9CDFC[v4 + 1];
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 3u);
    QueueSfx3_Max3(0x13u);
  } else {
    E->sice_var_08 = v2;
  }
}

void ShitroidInCutscene_HandleEnemyBlinking(uint16 k) {  // 0xA9CE24
  Enemy_ShitroidInCutscene *E = Get_ShitroidInCutscene(k);
  if ((E->base.frame_counter & 1) != 0)
    E->base.properties &= ~kEnemyProps_Invisible;
  else
    E->base.properties |= kEnemyProps_Invisible;
}

void ShitroidInCutscene_AccelerateDownwards(uint16 k) {  // 0xA9CE40
  int16 v2;

  Enemy_ShitroidInCutscene *E = Get_ShitroidInCutscene(k);
  v2 = abs16(E->sice_var_B) - 32;
  if (v2 < 0)
    v2 = 0;
  E->sice_var_B = sign16(E->sice_var_B) ? -v2 : v2;
  E->sice_var_C += 2;
}

void ShitroidInCutscene_HandleHealthBasedPalette(uint16 k) {  // 0xA9CE69
  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(k);
  if (E->sice_var_09) {
    uint16 health = E->base.health;
    if (sign16(health - 2560)) {
      uint16 v3 = 2;
      if (sign16(health - 2240)) {
        v3 = 4;
        if (sign16(health - 1920)) {
          v3 = 6;
          if (sign16(health - 1600)) {
            v3 = 8;
            if (sign16(health - 1280)) {
              v3 = 10;
              if (sign16(health - 960)) {
                v3 = 12;
                if (sign16(health - 640))
                  v3 = 14;
              }
            }
          }
        }
      }
      uint16 v5 = v3;
      uint16 v4 = kShitroid_HealthBasedPalettes_Shell[v3 >> 1];
      WriteColorsToPalette(0x1E2, 0xad, v4, 4);
      WriteColorsToPalette(0x1F4, 0xad, v4 + 8, 5);
      WriteColorsToPalette(0x1EA, 0xad, kShitroid_HealthBasedPalettes_Innards[v5 >> 1], 5u);
    }
  }
}

void ShitroidInCutscene_Shake(uint16 k) {  // 0xA9CEDB
  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(k);
  --E->sice_var_C;
  int v2 = (uint8)(E->base.frame_counter & 6) >> 1;
  E->base.x_pos = E->sice_var_10 + g_word_A993BB[v2];
  E->base.y_pos = E->sice_var_11 + g_word_A993C3[v2];
}

void ShitroidInCutscene_Touch(void) {  // 0xA9CF03
  Enemy_ShitroidInCutscene *E = Get_ShitroidInCutscene(cur_enemy_index);
  if (E->sice_var_A == (uint16)FUNC16(ShitroidInCutscene_LatchOntoSamus)) {
    R18_ = samus_x_pos;
    R20_ = samus_y_pos - 20;
    if (Shitroid_AccelerateTowardsPoint(cur_enemy_index, 0x10) & 1) {
      E->sice_var_B = 0;
      E->sice_var_C = 0;
      E->sice_var_A = FUNC16(ShitroidInCutscene_HealSamusToFullHealth);
    }
  }
}

void ShitroidInCutscene_UpdateSpeedAndAngle(uint16 k) {  // 0xA9CF31
  Enemy_ShitroidInCutscene *E = Get_ShitdroidInCutscene(k);
  if (R22_ != E->sice_var_0B) {
    if ((int16)(R22_ - E->sice_var_0B) >= 0) {
      uint16 v3 = E->sice_var_0B + 32;
      if (!sign16(v3 - R22_))
        v3 = R22_;
      E->sice_var_0B = v3;
    } else {
      uint16 v2 = E->sice_var_0B - 32;
      if (sign16(v2 - R22_))
        v2 = R22_;
      E->sice_var_0B = v2;
    }
  }
  uint16 v4;
  if ((R18_ & 0x8000) != 0) {
    v4 = E->sice_var_0A + R18_;
    if (sign16(v4 - R20_))
      LABEL_13:
    v4 = R20_;
  } else {
    v4 = E->sice_var_0A + R18_;
    if (!sign16(v4 - R20_))
      goto LABEL_13;
  }
  E->sice_var_0A = v4;
  R18_ = HIBYTE(v4);
  E->sice_var_B = Math_MultBySin(E->sice_var_0B);
  E->sice_var_C = Math_MultByCos(E->sice_var_0B);
}

uint16 Shitroid_Instr_1(uint16 k, uint16 j) {  // 0xA9CFB4
  return addr_kShitroid_Ilist_CFA2;
}

uint16 Shitroid_Instr_2(uint16 k, uint16 j) {  // 0xA9CFCA
  return addr_kShitroid_Ilist_CFB8;
}

void MotherBrain_Pal_ProcessInvincibility(void) {  // 0xA9CFD4
  if (Get_MotherBrain(0)->mbn_var_00 == 4) {
    uint16 flash_timer = Get_MotherBrain(0x40)->base.flash_timer;
    if (flash_timer >> 1) {
      if (!(flash_timer & 1)) {
        for (int i = 28; i >= 0; i -= 2) {
          int v2 = i >> 1;
          uint16 v3 = palette_buffer[v2 + 129];
          palette_buffer[v2 + 145] = v3;
          palette_buffer[v2 + 177] = v3;
          palette_buffer[v2 + 65] = v3;
        }
      }
    }
  }
}

void MotherBrain_Pal_BeginScreenFlash(void) {  // 0xA9CFFD
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->mbn_var_0E = addr_stru_A9D046;
  E->mbn_var_0F = 1;
}

void MotherBrain_Pal_EndScreenFlash(void) {  // 0xA9D00C
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  E->mbn_var_0E = 0;
  E->mbn_var_0F = 0;
  MotherBrain_Pal_WriteRoomPal(addr_word_A9D082);
}

void MotherBrain_Pal_HandleRoomPal(void) {  // 0xA9D01C
  uint16 j = HandleRoomPaletteInstructionList(ADDR16_OF_RAM(*enemy_ram7800) + 28);
  if (j)
    MotherBrain_Pal_WriteRoomPal(j);
}

void MotherBrain_Pal_WriteRoomPal(uint16 j) {  // 0xA9D025
  WriteColorsToPalette(0x68, 0xa9, j, 12);
  WriteColorsToPalette(0xA6, 0xa9, j + 24, 12);
  WriteColorsToPalette(0xE6, 0xa9, j + 24, 12);
}

uint16 HandleRoomPaletteInstructionList(uint16 a) {  // 0xA9D192
  int16 v4;

  R0_.addr = a;
  R3_.addr = a + 2;
  R0_.bank = 126;
  R3_.bank = 126;
  uint16 *v1 = (uint16 *)IndirPtr(&R0_, 0);
  if ((*v1 & 0x8000) == 0)
    return 0;
  uint16 v3 = *v1;
  v4 = *(uint16 *)RomPtr_A9(*v1);
  if (v4 < 0)
    goto LABEL_8;
  if (v4 == *(uint16 *)IndirPtr(&R3_, 0)) {
    for (v3 += 4; ; ) {
      v4 = *(uint16 *)RomPtr_A9(v3);
      if (!v4)
        break;
      if (v4 >= 0) {
        IndirWriteWord(&R3_, 0, 1u);
        IndirWriteWord(&R0_, 0, v3);
        return *((uint16 *)RomPtr_A9(v3) + 1);
      }
LABEL_8:
      R6_ = v4;
      v3 = CallMotherBrainInstr((uint16)v4 | 0xA90000, v3 + 2);
    }
    IndirWriteWord(&R0_, 0, 0);
    IndirWriteWord(&R3_, 0, 0);
    return 0;
  } else {
    uint8 *v5 = IndirPtr(&R3_, 0);
    IndirWriteWord(&R3_, 0, *(uint16 *)v5 + 1);
    return *((uint16 *)RomPtr_A9(v3) + 1);
  }
}

void MotherBrain_HandlePalette(void) {  // 0xA9D1E4
  Enemy_MotherBrain *E = Get_MotherBrain(0x40);
  if (E->mbn_var_10)
    MotherBrain_HandleBrainPal();
  if (E->mbn_var_11)
    MotherBrain_HealthBasedPaletteHandling();
}

void MotherBrain_SetupBrainNormalPal(void) {  // 0xA9D1F8
  Get_MotherBrain(0)->mbn_var_E = 10;
}

void MotherBrain_SetupBrainPalForLaser(void) {  // 0xA9D1FF
  Get_MotherBrain(0)->mbn_var_E = 514;
}

void MotherBrain_HandleBrainPal(void) {  // 0xA9D206
  char mbn_var_D_high;
  char mbn_var_D;

  Enemy_MotherBrain *E = Get_MotherBrain(0);
  if (E->mbn_var_00 != 1) {
    mbn_var_D_high = HIBYTE(E->mbn_var_D);
    if (mbn_var_D_high) {
      HIBYTE(E->mbn_var_D) = mbn_var_D_high - 1;
    } else {
      R18_ = g_off_A9D260[HIBYTE(E->mbn_var_E) >> 1];
      HIBYTE(E->mbn_var_D) = E->mbn_var_E;
      mbn_var_D = E->mbn_var_D;
      if (mbn_var_D
          || Get_MotherBrain(0x40)->mbn_var_A == (uint16)FUNC16(MotherBrainsBrain_SetupBrainAndNeckToDraw)) {
        uint8 v4 = (mbn_var_D + 1) & 7;
        LOBYTE(E->mbn_var_D) = v4;
        uint16 v5 = 290;
        if (E->mbn_var_0D != 512)
          v5 = 482;
        WriteColorsToPalette(v5, 0xa9, R18_ + 8 * v4, 3u);
      }
    }
  }
}

void WriteColorsToPalette(uint16 k, uint8 db, uint16 j, uint16 a) {  // 0xA9D2E4
  R18_ = a;
  do {
    palette_buffer[k >> 1] = *(uint16 *)RomPtrWithBank(db, j);
    k += 2;
    j += 2;
    --R18_;
  } while (R18_);
}

void WriteColorsToTargetPalette(uint8 db, uint16 k, uint16 j, uint16 a) {  // 0xA9D2F6
  R18_ = a;
  do {
    target_palettes[k >> 1] = *(uint16 *)RomPtrWithBank(db, j);
    k += 2;
    j += 2;
    --R18_;
  } while (R18_);
}

void DeadTorizo_Init(void) {  // 0xA9D308
  for (int i = 4094; i >= 0; i -= 2)
    tilemap_stuff[i >> 1] = 0;
  Enemy_DeadTorizo *E = Get_DeadTorizo(0);
  E->dto_var_A = FUNC16(DeadTorizo_WaitForSamusColl);
  E->base.properties |= 0xA000u;
  E->base.current_instruction = addr_kDeadTorizo_Ilist_D6DC;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  E->base.palette_index = 512;
  E->dto_var_B = 0;
  E->dto_var_C = 8;
  E->dto_var_04 = 0;
  E->dto_var_02 = 15;
  E->dto_var_03 = 0;
  InitializeEnemyCorpseRotting(0, addr_stru_A9DD58);
}

void CallDeadTorizoFuncA(uint32 ea) {
  switch (ea) {
  case fnDeadTorizo_WaitForSamusColl: DeadTorizo_WaitForSamusColl(cur_enemy_index); return;
  case fnDeadTorizo_Rotting: DeadTorizo_Rotting(); return;
  case fnDeadTorizo_PreRotDelay: DeadTorizo_PreRotDelay(); return;
  case fnnullsub_361: return;
  default: Unreachable();
  }
}

void DeadTorizo_Main(void) {  // 0xA9D368
  Enemy_DeadTorizo *E = Get_DeadTorizo(0);
  if ((E->base.properties & kEnemyProps_Tangible) == 0 && DeadTorizo_Func_0() & 1) {
    E->base.properties |= kEnemyProps_Tangible;
    E->dto_var_A = FUNC16(DeadTorizo_Rotting);
  }
  mov24(&enemy_gfx_drawn_hook, 0xA9D39A);
  CallDeadTorizoFuncA(E->dto_var_A | 0xA90000);
  DeadTorizo_Func_1();
}

void DeadTorizo_MainGfxHook(void) {  // 0xA9D39A
  R22_ = 0;
  R18_ = 296;
  R20_ = 187;
  MotherBrain_AddSpritemapToOam(addr_kDeadTorizo_Sprmap_D761);
}

void DeadTorizo_WaitForSamusColl(uint16 k) {  // 0xA9D3AD
  if (k == enemy_index_colliding_dirs[0]
      || k == enemy_index_colliding_dirs[1]
      || k == enemy_index_colliding_dirs[2]
      || k == enemy_index_colliding_dirs[3]) {
    Get_DeadTorizo(0)->dto_var_A = FUNC16(DeadTorizo_PreRotDelay);
  }
}

void DeadTorizo_PreRotDelay(void) {  // 0xA9D3C8
  Enemy_DeadTorizo *E = Get_DeadTorizo(0);
  uint16 v1 = E->dto_var_04 + 1;
  E->dto_var_04 = v1;
  if (v1 >= 0x10) {
    E->base.properties |= kEnemyProps_Tangible;
    E->dto_var_A = FUNC16(DeadTorizo_Rotting);
    DeadTorizo_Rotting();
  }
}

void DeadTorizo_Rotting(void) {  // 0xA9D3E6
  Enemy_DeadTorizo *E = Get_DeadTorizo(0);
  uint16 v1 = E->dto_var_03 + 1;
  E->dto_var_03 = v1;
  if (v1 >= 0xFu) {
    E->dto_var_03 = 0;
    uint16 dto_var_02 = E->dto_var_02;
    if (dto_var_02) {
      DeadTorizo_CopyLineOfSandHeapTileData(dto_var_02);
      --E->dto_var_02;
    }
  }
  ++E->dto_var_C;
  MoveEnemyWithVelocity();
  if (!(ProcessCorpseRotting(0) & 1))
    E->dto_var_A = FUNC16(nullsub_361);
}

void DeadTorizo_Powerbomb(void) {  // 0xA9D42A
  if ((Get_DeadTorizo(0)->base.properties & kEnemyProps_Tangible) == 0)
    DeadTorizo_Shot();
}

void DeadTorizo_Shot(void) {  // 0xA9D433
  Enemy_DeadTorizo *E = Get_DeadTorizo(0);
  E->base.properties |= kEnemyProps_Tangible;
  E->dto_var_A = FUNC16(DeadTorizo_Rotting);
}

uint8 DeadTorizo_Func_0(void) {  // 0xA9D443
  int16 v5;
  uint16 v3, v4;

  Enemy_DeadTorizo *E = Get_DeadTorizo(0);
  R18_ = E->base.x_pos;
  R20_ = E->base.y_pos;
  uint16 *v1 = (uint16 *)RomPtr_A9(addr_kDeadTorizo_Hitbox_D77C);
  if (!*v1)
    return 0;
  R22_ = *v1;
  for (int i = 0xd77c + 2; ; i += 8) {
    if ((int16)(samus_y_pos - R20_) >= 0) {
      R24_ = samus_y_pos - R20_;
      v3 = *((uint16 *)RomPtr_A9(i) + 3);
    } else {
      R24_ = R20_ - samus_y_pos;
      v3 = *((uint16 *)RomPtr_A9(i) + 1);
    }
    if ((int16)(samus_y_radius + abs16(v3) - R24_) >= 0) {
      if ((int16)(samus_x_pos - R18_) >= 0) {
        R24_ = samus_x_pos - R18_;
        v4 = *((uint16 *)RomPtr_A9(i) + 2);
      } else {
        R24_ = R18_ - samus_x_pos;
        v4 = *(uint16 *)RomPtr_A9(i);
      }
      v5 = samus_x_radius + abs16(v4) - R24_;
      if (v5 >= 0)
        break;
    }
    if (!--R22_)
      return 0;
  }
  if (sign16(v5 - 4))
    v5 = 4;
  extra_samus_x_displacement = v5;
  extra_samus_y_displacement = 4;
  extra_samus_x_subdisplacement = 0;
  extra_samus_y_subdisplacement = 0;
  return 1;
}

void DeadTorizo_Func_1(void) {  // 0xA9D4CF
  VramWriteEntry *v5;
  VramWriteEntry *v10;
  Enemy_DeadTorizo *E = Get_DeadTorizo(0);
  uint16 v1 = E->dto_var_00 + 1;
  E->dto_var_00 = v1;
  uint16 v3 = vram_write_queue_tail;
  if (v1 & 1) {
    uint16 v8 = 0;
    uint16 v9 = g_word_A9D583[0];
    do {
      v10 = gVramWriteEntry(v3);
      v10->size = v9;
      int v11 = v8 >> 1;
      *(VoidP *)((char *)&v10->src.addr + 1) = g_word_A9D583[v11 + 1];
      v10->src.addr = g_word_A9D583[v11 + 2];
      v10->vram_dst = g_word_A9D583[v11 + 3];
      v3 += 7;
      v8 += 8;
      v9 = g_word_A9D583[v8 >> 1];
    } while (v9);
  } else {
    uint16 v2 = 0;
    uint16 v4 = g_word_A9D549[0];
    do {
      v5 = gVramWriteEntry(v3);
      v5->size = v4;
      int v6 = v2 >> 1;
      *(VoidP *)((char *)&v5->src.addr + 1) = g_word_A9D549[v6 + 1];
      v5->src.addr = g_word_A9D549[v6 + 2];
      v5->vram_dst = g_word_A9D549[v6 + 3];
      v3 += 7;
      v2 += 8;
      v4 = g_word_A9D549[v2 >> 1];
    } while (v4);
  }
  Get_DeadEnemy(0)->dey_var_22 = 0;
  vram_write_queue_tail = v3;
}

void DeadTorizo_CorpseRottingFinished(void) {  // 0xA9D5BD
  R18_ = (random_number & 0x1F) + 272;
  R20_ = 188;
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 0xA);
  if ((enemy_damage_routine_exec_count & 7) == 0)
    QueueSfx2_Max6(0x10);
}

void DeadTorizo_CopyLineOfSandHeapTileData(uint16 a) {  // 0xA9D5EA
  uint16 v1 = g_word_A9D67C[a];
  int v2 = g_word_A9D69C[a] >> 1;
  *(uint16 *)(&g_byte_7E9500 + v1) = kDeadTorizo_TileData[v2];
  *(uint16 *)(&g_byte_7E9510 + v1) = kDeadTorizo_TileData[v2 + 8];
  *(uint16 *)(&g_byte_7E9520 + v1) = kDeadTorizo_TileData[v2 + 16];
  *(uint16 *)(&g_byte_7E9530 + v1) = kDeadTorizo_TileData[v2 + 24];
  *(uint16 *)(&g_byte_7E9540 + v1) = kDeadTorizo_TileData[v2 + 32];
  *(uint16 *)(&g_byte_7E9550 + v1) = kDeadTorizo_TileData[v2 + 40];
  *(uint16 *)(&g_byte_7E9560 + v1) = kDeadTorizo_TileData[v2 + 48];
  *(uint16 *)(&g_byte_7E9570 + v1) = kDeadTorizo_TileData[v2 + 56];
  *(uint16 *)(&g_byte_7E9580 + v1) = kDeadTorizo_TileData[v2 + 64];
  *(uint16 *)(&g_byte_7E9590 + v1) = kDeadTorizo_TileData[v2 + 72];
  *(uint16 *)(&g_byte_7E95A0 + v1) = kDeadTorizo_TileData[v2 + 80];
  *(uint16 *)(&g_byte_7E95B0 + v1) = kDeadTorizo_TileData[v2 + 88];
  *(uint16 *)(&g_byte_7E95C0 + v1) = kDeadTorizo_TileData[v2 + 96];
  *(uint16 *)(&g_byte_7E95D0 + v1) = kDeadTorizo_TileData[v2 + 104];
  *(uint16 *)(&g_byte_7E95E0 + v1) = kDeadTorizo_TileData[v2 + 112];
  *(uint16 *)(&g_byte_7E95F0 + v1) = kDeadTorizo_TileData[v2 + 120];
  *(uint16 *)(&g_byte_7E9600 + v1) = kDeadTorizo_TileData[v2 + 128];
  *(uint16 *)(&g_byte_7E9610 + v1) = kDeadTorizo_TileData[v2 + 136];
}

void DeadSidehopper_Init(void) {  // 0xA9D7B6
  uint16 dsr_parameter_1 = Get_DeadSidehopper(cur_enemy_index)->dsr_parameter_1;
  if (dsr_parameter_1) {
    if (dsr_parameter_1 != 2) {
      Unreachable();
      while (1)
        ;
    }
    DeadSidehopper_Init_1();
  } else {
    DeadSidehopper_Init_0();
  }
}

void DeadSidehopper_Init_0(void) {  // 0xA9D7C4
  Enemy_DeadSidehopper *E = Get_DeadSidehopper(cur_enemy_index);
  E->base.properties = E->base.properties & 0x77FF | kEnemyProps_ProcessedOffscreen;
  if ((Get_DeadSidehopper(0)->base.properties & kEnemyProps_Invisible) != 0)
    E->base.properties |= kEnemyProps_Deleted;
  E->dsr_var_08 = 0;
  E->dsr_var_0A = 96;
  E->dsr_var_0B = 256;
  E->base.x_pos = 488;
  E->base.y_pos = 184;
  E->dsr_var_A = FUNC16(DeadSidehopper_Alive_WaitForActivate);
  E->base.palette_index = 512;
  E->base.y_height = 21;
  Enemy_SetInstrList(cur_enemy_index, addr_kDeadMonsters_Ilist_ECE3);
  InitializeEnemyCorpseRotting(cur_enemy_index, addr_stru_A9DD68);
}

void DeadSidehopper_Init_1(void) {  // 0xA9D825
  Enemy_DeadSidehopper *E = Get_DeadSidehopper(cur_enemy_index);
  E->dsr_var_08 = -1;
  E->dsr_var_A = FUNC16(DeadSidehopper_WaitForSamusColl);
  E->base.palette_index = 3584;
  Enemy_SetInstrList(cur_enemy_index, addr_kDeadMonsters_Ilist_ECEF);
  InitializeEnemyCorpseRotting(cur_enemy_index, addr_stru_A9DD78);
}

void DeadZoomer_Init(void) {  // 0xA9D849
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  v1->palette_index = 3584;
  v1->ai_var_A = FUNC16(DeadZoomer_WaitForSamusColl);
  int v2 = v1->parameter_1 >> 1;
  Enemy_SetInstrList(cur_enemy_index, g_off_A9D86A[v2]);
  InitializeEnemyCorpseRotting(cur_enemy_index, g_off_A9D870[v2]);
}

void DeadRipper_Init(void) {  // 0xA9D876
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  v1->palette_index = 3584;
  v1->ai_var_A = FUNC16(DeadRipper_WaitForSamusColl);
  int v2 = v1->parameter_1 >> 1;
  Enemy_SetInstrList(cur_enemy_index, g_off_A9D897[v2]);
  InitializeEnemyCorpseRotting(cur_enemy_index, g_off_A9D89B[v2]);
}

void DeadSkree_Init(void) {  // 0xA9D89F
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  v1->palette_index = 3584;
  v1->ai_var_A = FUNC16(DeadSkree_WaitForSamusColl);
  int v2 = v1->parameter_1 >> 1;
  Enemy_SetInstrList(cur_enemy_index, g_off_A9D8C0[v2]);
  InitializeEnemyCorpseRotting(cur_enemy_index, g_off_A9D8C6[v2]);
}

void DeadSidehopper_Powerbomb(void) {  // 0xA9D8CC
  if (Get_DeadSidehopper(cur_enemy_index)->dsr_var_08 >= 8u)
    DeadSidehopper_Shot();
  else
    DeadSidehopper_Main();
}

void CallDeadSidehopperFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnDeadSidehopper_Activated: DeadSidehopper_Activated(k); return;
  case fnDeadSidehopper_Alive_WaitForActivate: DeadSidehopper_Alive_WaitForActivate(k); return;

  case fnDeadSidehopper_WaitForSamusColl: DeadSidehopper_WaitForSamusColl(k); return;
  case fnDeadZoomer_WaitForSamusColl: DeadZoomer_WaitForSamusColl(k); return;
  case fnDeadSkree_WaitForSamusColl: DeadSkree_WaitForSamusColl(k); return;
  case fnDeadRipper_WaitForSamusColl: DeadRipper_WaitForSamusColl(k); return;

  case fnDeadSidehopper_PreRotDelay: DeadSidehopper_PreRotDelay(k); return;
  case fnDeadZoomer_PreRotDelay: DeadZoomer_PreRotDelay(k); return;
  case fnDeadSkree_PreRotDelay: DeadSkree_PreRotDelay(k); return;
  case fnDeadRipper_PreRotDelay: DeadRipper_PreRotDelay(k); return;

  case fnDeadSidehopper_Rotting: DeadSidehopper_Rotting(k); return;
  case fnDeadZoomer_Rotting: DeadZoomer_Rotting(k); return;
  case fnDeadSkree_Rotting: DeadSkree_Rotting(k); return;
  case fnDeadRipper_Rotting: DeadRipper_Rotting(k); return;

  case fnDeadMonsters_Func_1: DeadMonsters_Func_1(k); return;
  case fnDeadMonsters_Func_2: DeadMonsters_Func_2(k); return;
  case fnDeadMonsters_Func_5: DeadMonsters_Func_5(k); return;

  case fnnullsub_362: return;
  case fnnullsub_268: return;

  default: Unreachable();
  }
}
void DeadSidehopper_Main(void) {  // 0xA9D8DB
  Enemy_DeadSidehopper *E = Get_DeadSidehopper(cur_enemy_index);
  CallDeadSidehopperFunc(E->dsr_var_A | 0xA90000, cur_enemy_index);
}

void DeadSidehopper_Alive_WaitForActivate(uint16 k) {  // 0xA9D8E2
  if (sign16(layer1_x_pos - 513)) {
    Get_DeadSidehopper(k)->dsr_var_A = FUNC16(DeadSidehopper_Activated);
    DeadSidehopper_Activated(k);
  }
}

void DeadSidehopper_Activated(uint16 k) {  // 0xA9D8F1
  if (DeadMonsters_Func_3(k) & 1) {
    Enemy_DeadSidehopper *E = Get_DeadSidehopper(k);
    E->dsr_var_06 = ((uint8)E->dsr_var_06 + 1) & 3;
    Enemy_SetInstrList(k, addr_kDeadMonsters_Ilist_ECAC);
    E->dsr_var_A = FUNC16(nullsub_362);
  }
}

void DeadMonsters_Func_1(uint16 k) {  // 0xA9D910
  Enemy_DeadMonsters *E = Get_DeadMonsters(k);
  E->dms_var_A = FUNC16(DeadMonsters_Func_2);
  E->dms_var_F = 64;
}

void DeadMonsters_Func_2(uint16 k) {  // 0xA9D91D
  Enemy_DeadMonsters *E = Get_DeadMonsters(k);
  if ((--E->dms_var_F & 0x8000) != 0) {
    if (E->dms_var_08) {
      E->dms_var_A = FUNC16(DeadMonsters_Func_5);
    } else {
      E->dms_var_A = FUNC16(DeadSidehopper_Activated);
      Enemy_SetInstrList(k, addr_kDeadMonsters_Ilist_ECE3);
      int v3 = (uint16)(2 * E->dms_var_06) >> 1;
      E->dms_var_0B = g_word_A9D951[v3];
      E->dms_var_0A = g_word_A9D959[v3];
    }
  }
}

uint8 DeadMonsters_Func_3(uint16 k) {  // 0xA9D961
  Enemy_DeadMonsters *E = Get_DeadMonsters(k);
  DeadMonsters_Func_4(k, E->dms_var_0A);
  uint16 v2 = 32;
  if ((E->dms_var_0B & 0x8000) == 0)
    v2 = 128;
  uint16 v3 = E->dms_var_0B + v2;
  E->dms_var_0B = v3;
  R18_ = v3;
  if (sign16(E->base.x_pos - 544)) {
    int full = HIBYTE(E->base.y_subpos) + LOBYTE(v3);
    HIBYTE(E->base.y_subpos) = full;
    uint16 v12 = E->base.y_pos + (int8)(v3 >> 8) + (full >> 8);
    E->base.y_pos = v12;
    return sign16(v12 - 184) == 0;
  } else {
    R18_ = v3 << 8;
    R20_ = (int8)(v3 >> 8);
    return Enemy_MoveDown(k);
  }
}

void DeadMonsters_Func_4(uint16 k, uint16 a) {  // 0xA9D9C7
  R18_ = a;
  Enemy_DeadMonsters *E = Get_DeadMonsters(k);
  if (sign16(E->base.x_pos - 544)) {
    int full = HIBYTE(E->base.x_subpos) + LOBYTE(a);
    HIBYTE(E->base.x_subpos) = full;
    E->base.x_pos += (int8)(a >> 8) + (full >> 8);
  } else {
    R18_ = a << 8;
    R20_ = (int8)(a >> 8);
    Enemy_MoveRight_IgnoreSlopes(k);
  }
}

void DeadMonsters_Func_5(uint16 k) {  // 0xA9DA08
  Enemy_DeadMonsters *E = Get_DeadMonsters(k);
  uint16 v2 = E->dms_var_07 + 1;
  E->dms_var_07 = v2;
  if (v2 >= 8u) {
    E->dms_var_07 = 0;
    WriteColorsToPalette(
      0x122u,
      0xa9, 32 * (E->dms_var_08 - 1) - 0x1434,
      0xFu);
    uint16 v3 = cur_enemy_index;
    uint16 v5 = E->dms_var_08 + 1;
    E->dms_var_08 = v5;
    if (v5 >= 8u) {
      Enemy_SetInstrList(v3, addr_kDeadMonsters_Ilist_ECE9);
      E->dms_var_A = FUNC16(DeadSidehopper_WaitForSamusColl);
      E->base.properties |= 0x8000u;
      E->base.y_height = 12;
    }
  }
}

void DeadSidehopper_WaitForSamusColl(uint16 k) {  // 0xA9DA64
  DeadMonsters_WaitForSamusColl(k, FUNC16(DeadSidehopper_PreRotDelay));
}

void DeadZoomer_WaitForSamusColl(uint16 k) {  // 0xA9DA69
  DeadMonsters_WaitForSamusColl(k, FUNC16(DeadZoomer_PreRotDelay));
}

void DeadSkree_WaitForSamusColl(uint16 k) {  // 0xA9DA6E
  DeadMonsters_WaitForSamusColl(k, FUNC16(DeadSkree_PreRotDelay));
}

void DeadRipper_WaitForSamusColl(uint16 k) {  // 0xA9DA73
  DeadMonsters_WaitForSamusColl(k, FUNC16(DeadRipper_PreRotDelay));
}

void DeadMonsters_WaitForSamusColl(uint16 k, uint16 j) {  // 0xA9DA76
  if (k == enemy_index_colliding_dirs[0]
      || k == enemy_index_colliding_dirs[1]
      || k == enemy_index_colliding_dirs[2]
      || k == enemy_index_colliding_dirs[3]) {
    Get_DeadMonsters(k)->dms_var_A = j;
  }
}

void DeadSidehopper_PreRotDelay(uint16 k) {  // 0xA9DA8F
  DeadMonsters_PreRotDelay_Common(k, FUNC16(DeadSidehopper_Rotting));
}

void DeadZoomer_PreRotDelay(uint16 k) {  // 0xA9DA94
  DeadMonsters_PreRotDelay_Common(k, FUNC16(DeadZoomer_Rotting));
}

void DeadRipper_PreRotDelay(uint16 k) {  // 0xA9DA99
  DeadMonsters_PreRotDelay_Common(k, FUNC16(DeadRipper_Rotting));
}

void DeadSkree_PreRotDelay(uint16 k) {  // 0xA9DA9E
  DeadMonsters_PreRotDelay_Common(k, FUNC16(DeadSkree_Rotting));
}

void DeadMonsters_PreRotDelay_Common(uint16 k, uint16 j) {  // 0xA9DAA1
  Enemy_DeadMonsters *E = Get_DeadMonsters(k);
  if (++E->dms_var_B >= 0x10) {
    E->dms_var_A = j;
    E->base.properties |= kEnemyProps_Tangible;
  }
}

void DeadSidehopper_Rotting(uint16 k) {  // 0xA9DABA
  uint8 v1 = ProcessCorpseRotting(k);
  if (!(v1 & 1))
    Get_DeadMonsters(cur_enemy_index)->dms_var_A = FUNC16(DeadSidehopper_WaitForSamusColl);
  uint16 dms_var_53 = Get_DeadMonsters(cur_enemy_index)->dms_var_53;
  ProcessCorpseRottingVramTransfers(dms_var_53);
}

void DeadZoomer_Rotting(uint16 k) {  // 0xA9DAD0
  uint8 v1 = ProcessCorpseRotting(k);
  if (!(v1 & 1))
    Get_DeadMonsters(cur_enemy_index)->dms_var_A = FUNC16(nullsub_268);
  uint16 dms_var_53 = Get_DeadMonsters(cur_enemy_index)->dms_var_53;
  ProcessCorpseRottingVramTransfers(dms_var_53);
}

void DeadRipper_Rotting(uint16 k) {  // 0xA9DAE6
  uint8 v1 = ProcessCorpseRotting(k);
  if (!(v1 & 1))
    Get_DeadMonsters(cur_enemy_index)->dms_var_A = FUNC16(nullsub_268);
  uint16 dms_var_53 = Get_DeadMonsters(cur_enemy_index)->dms_var_53;
  ProcessCorpseRottingVramTransfers(dms_var_53);
}

void DeadSkree_Rotting(uint16 k) {  // 0xA9DAFC
  uint8 v1 = ProcessCorpseRotting(k);
  if (!(v1 & 1))
    Get_DeadMonsters(cur_enemy_index)->dms_var_A = FUNC16(nullsub_268);
  uint16 dms_var_53 = Get_DeadMonsters(cur_enemy_index)->dms_var_53;
  ProcessCorpseRottingVramTransfers(dms_var_53);
}

uint8 ProcessCorpseRotting(uint16 k) {  // 0xA9DB12
  int16 *v5;
  int16 v7;
  int16 v8;

  Enemy_DeadMonsters *EK = Get_DeadMonsters(k);
  Enemy_DeadMonsters *E0 = Get_DeadMonsters(0);
  E0->dms_var_45 = EK->dms_var_57;
  E0->dms_var_46 = EK->dms_var_58;
  E0->dms_var_47 = EK->dms_var_59;
  E0->dms_var_48 = EK->dms_var_5A;
  E0->dms_var_49 = EK->dms_var_5B;
  E0->dms_var_44 = EK->dms_var_56;
  E0->dms_var_42 = EK->dms_var_54;
  E0->dms_var_43 = EK->dms_var_55;
  uint16 dms_var_52 = EK->dms_var_52;
  uint16 v4 = 0;
  uint16 v15;
  uint16 *v6;
  while (1) {
    v15 = v4;
    v5 = (int16 *)RomPtr_7E(dms_var_52);
    v6 = (uint16 *)v5;
    if (*v5 >= 0)
      break;
LABEL_12:
    dms_var_52 += 4;
    v4 = v15 + 1;
    E0 = Get_DeadMonsters(0);
    if ((int16)(v15 + 1 - E0->dms_var_45) >= 0)
      return (uint16)(v15 + 1) >= E0->dms_var_45;
  }
  v7 = v5[1];
  if (v7) {
    v8 = v7 - 1;
    v5[1] = v8;
    if ((uint16)v8 < 4u) {
      E0 = Get_DeadMonsters(0);
      R18_ = E0->dms_var_42;
      if (v15 >= E0->dms_var_47)
        R18_ = E0->dms_var_43;
      R20_ = E0->dms_var_44;
      R22_ = 169;
      CopyMoveCorpseRottingRotEntry(*v6);
    }
    goto LABEL_12;
  }
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  R18_ = E->dms_var_43;
  R20_ = E->dms_var_44;
  R22_ = 169;
  uint16 *v11 = (uint16 *)RomPtr_7E(dms_var_52);
  CopyMoveCorpseRottingRotEntry(*v11);
  uint16 v12 = *(uint16 *)RomPtr_7E(dms_var_52) + 2;
  if (v12 < E->dms_var_46) {
LABEL_11:
    *(uint16 *)RomPtr_7E(dms_var_52) = v12;
    goto LABEL_12;
  }
  R18_ = E->dms_var_49;
  CallCorpseRottingFinish(R18_ | 0xA90000);
  if (v15 < Get_DeadMonsters(0)->dms_var_46) {
    v12 = -1;
    goto LABEL_11;
  }
  return 0;
}

void CopyMoveCorpseRottingRotEntry(uint16 a) {  // 0xA9DBE0
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  E->dms_var_41 = a;
  uint16 v1 = (uint16)(a & 0xFFF8) >> 2;
  uint16 v2 = a & 7;
  uint8 *v4 = IndirPtr(&R20_, v1);
  if (v2 >= 6u) {
    uint16 v6 = *(uint16 *)v4 + 2 * v2;
    CallCorpseRottingMove(R18_ | 0xA90000, E->dms_var_48 + v6, v6);
  } else {
    uint16 v5 = *(uint16 *)v4 + 2 * v2;
    CallCorpseRottingMove(R18_ | 0xA90000, v5, v5);
  }
}

void CorpseRottingRotEntryFinishedHook(void) {  // 0xA9DC08
  EnemyData *v0 = gEnemyData(cur_enemy_index);
  R18_ = v0->x_pos + (random_number & 0x1A) - 14;
  R20_ = v0->y_pos + 16;
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 0xA);
  if ((enemy_damage_routine_exec_count & 7) == 0)
    QueueSfx2_Max3(0x10);
}

void InitializeCorpseRottingDataTable(uint16 k, uint16 a) {  // 0xA9DC40
  int16 v2;

  v2 = a - 1;
  R18_ = 0;
  do {
    *(uint16 *)RomPtr_7E(k) = v2;
    uint16 v3 = k + 2;
    *(uint16 *)RomPtr_7E(v3) = R18_;
    k = v3 + 2;
    R18_ += 2;
    --v2;
  } while (v2 >= 0);
}

void InitializeEnemyCorpseRotting(uint16 k, uint16 j) {  // 0xA9DC5F
  uint8 *v2 = RomPtr_A9(j);
  Enemy_DeadMonsters *E = Get_DeadMonsters(k);
  E->dms_var_52 = *(uint16 *)v2;
  E->dms_var_53 = *((uint16 *)v2 + 1);
  E->dms_var_54 = *((uint16 *)v2 + 2);
  E->dms_var_55 = *((uint16 *)v2 + 3);
  E->dms_var_5B = *((uint16 *)v2 + 7);
  uint16 v4 = *((uint16 *)v2 + 6);
  E->dms_var_56 = v4;
  E->dms_var_5A = *((uint16 *)RomPtr_A9(v4) + 1) - 12;
  uint16 *v5 = (uint16 *)RomPtr_A9(j);
  uint16 v6 = v5[4];
  E->dms_var_57 = v6--;
  E->dms_var_58 = v6;
  E->dms_var_59 = v6 - 1;
  InitializeCorpseRottingDataTable(*v5, v5[4]);
  R18_ = v5[5];
  CallCorpseRottingInit(R18_ | 0xA90000);
}

void ProcessCorpseRottingVramTransfers(uint16 k) {  // 0xA9DCB9
  VramWriteEntry *v3;

  uint16 v1 = vram_write_queue_tail;
  uint16 v2 = *(uint16 *)RomPtr_A9(k);
  do {
    v3 = gVramWriteEntry(v1);
    v3->size = v2;
    uint8 *v4 = RomPtr_A9(k);
    *(VoidP *)((char *)&v3->src.addr + 1) = *((uint16 *)v4 + 1);
    v3->src.addr = *((uint16 *)v4 + 2);
    v3->vram_dst = *((uint16 *)v4 + 3);
    v1 += 7;
    k += 8;
    v2 = *(uint16 *)RomPtr_A9(k);
  } while (v2);
  Get_DeadEnemy(0)->dey_var_22 = 0;
  vram_write_queue_tail = v1;
}

void DeadZoomer_Powerbomb(void) {  // 0xA9DCED
  if ((gEnemyData(cur_enemy_index)->properties & kEnemyProps_Tangible) == 0)
    DeadZoomer_Shot();
}

void DeadZoomer_Shot(void) {  // 0xA9DCF8
  DeadSidehopper_DD34(FUNC16(DeadZoomer_Rotting));
}

void DeadRipper_Powerbomb(void) {  // 0xA9DCFD
  if ((gEnemyData(cur_enemy_index)->properties & kEnemyProps_Tangible) == 0)
    DeadRipper_Shot();
}

void DeadRipper_Shot(void) {  // 0xA9DD08
  DeadSidehopper_DD34(FUNC16(DeadRipper_Rotting));
}

void DeadSkree_Powerbomb(void) {  // 0xA9DD0D
  if ((gEnemyData(cur_enemy_index)->properties & kEnemyProps_Tangible) == 0)
    DeadSkree_Shot();
}

void DeadSkree_Shot(void) {  // 0xA9DD18
  DeadSidehopper_DD34(FUNC16(DeadSkree_Rotting));
}

void DeadSidehopper_Shot(void) {  // 0xA9DD1D
  Enemy_DeadSidehopper *E = Get_DeadSidehopper(cur_enemy_index);
  if ((E->base.properties & kEnemyProps_Tangible) != 0 || E->dsr_var_08 < 8u)
    ;
  else
    DeadSidehopper_DD31();
}

void DeadSidehopper_DD31(void) {  // 0xA9DD31
  DeadSidehopper_DD34(FUNC16(DeadSidehopper_Rotting));
}

void DeadSidehopper_DD34(uint16 a) {  // 0xA9DD34
  Enemy_DeadSidehopper *E = Get_DeadSidehopper(cur_enemy_index);
  E->dsr_var_A = a;
  E->base.properties |= kEnemyProps_ProcessedOffscreen | kEnemyProps_Tangible;
}

void DeadSidehopper_Touch(void) {  // 0xA9DD44
  if (Get_DeadSidehopper(cur_enemy_index)->dsr_var_08 < 8u)
    NormalEnemyTouchAiSkipDeathAnim_CurEnemy();
  else
    DeadSidehopper_DD31();
}

#define TILEMAP_ADDR(x) RomPtr_7E(0x2000 + x)

void Torizo_CorpseRottingInitFunc(void) {  // 0xA9DE18
  uint8 *p = RomPtr_B7(addr_kDeadTorizo_TileData);
  MemCpy(TILEMAP_ADDR(0x060), p + 288, 0xC0);
  MemCpy(TILEMAP_ADDR(0x1A0), p + 800, 0xC0);
  MemCpy(TILEMAP_ADDR(0x2C0), p + 1280, 0x100);
  MemCpy(TILEMAP_ADDR(0x400), p + 1792, 0x100);
  MemCpy(TILEMAP_ADDR(0x540), p + 2304, 0x100);
  MemCpy(TILEMAP_ADDR(0x680), p + 2816, 0x100);
  MemCpy(TILEMAP_ADDR(0x7C0), p + 3328, 0x100);
  MemCpy(TILEMAP_ADDR(0x900), p + 3840, 0x100);
  MemCpy(TILEMAP_ADDR(0xA40), p + 4352, 0x100);
  MemCpy(TILEMAP_ADDR(0xB60), p + 4832, 0x120);
  MemCpy(TILEMAP_ADDR(0xC80), p + 5312, 0x140);
  MemCpy(TILEMAP_ADDR(0xDC0), p + 5824, 0x140);
}

void Sidehopper_CorpseRottingInitFunc_0(void) {  // 0xA9DEC1
  uint8 *p = RomPtr_B7(addr_kDeadMonsters_TileData);
  MemCpy(TILEMAP_ADDR(0x040), p + 64, 0x60);
  MemCpy(TILEMAP_ADDR(0x0A0), p + 512, 0xA0);
  MemCpy(TILEMAP_ADDR(0x140), p + 1024, 0xA0);
  MemCpy(TILEMAP_ADDR(0x1E0), p + 1536, 0xA0);
  MemCpy(TILEMAP_ADDR(0x280), p + 2048, 0xA0);
}

void Sidehopper_CorpseRottingInitFunc_2(void) {  // 0xA9DF08
  uint8 *p = RomPtr_B7(addr_kDeadMonsters_TileData);
  MemCpy(TILEMAP_ADDR(0x320), p + 288, 0x40);
  MemCpy(TILEMAP_ADDR(0x3C0), p + 800, 0xA0);
  MemCpy(TILEMAP_ADDR(0x460), p + 1312, 0xA0);
  MemCpy(TILEMAP_ADDR(0x500), p + 1824, 0xA0);
  MemCpy(TILEMAP_ADDR(0x5A0), p + 2336, 0xA0);
}

void Zoomer_CorpseRottingInitFunc_0(void) {  // 0xA9DF4F
  uint8 *p = RomPtr_B7(addr_kDeadMonsters_TileData);
  MemCpy(TILEMAP_ADDR(0x940), p + 2656, 0x60);
  MemCpy(TILEMAP_ADDR(0x9A0), p + 3168, 0x60);
}

void Zoomer_CorpseRottingInitFunc_2(void) {  // 0xA9DF6C
  uint8 *p = RomPtr_B7(addr_kDeadMonsters_TileData);
  MemCpy(TILEMAP_ADDR(0xA00), p + 2752, 0x60);
  MemCpy(TILEMAP_ADDR(0xA60), p + 3264, 0x60);
}

void Zoomer_CorpseRottingInitFunc_4(void) {  // 0xA9DF89
  uint8 *p = RomPtr_B7(addr_kDeadMonsters_TileData);
  MemCpy(TILEMAP_ADDR(0xAC0), p + 2848, 0x60);
  MemCpy(TILEMAP_ADDR(0xB20), p + 3360, 0x60);
}

void Ripper_CorpseRottingInitFunc_0(void) {  // 0xA9DFA6
  uint8 *p = RomPtr_B7(addr_kDeadMonsters_TileData);
  MemCpy(TILEMAP_ADDR(0xB80), p + 2560, 0x60);
  MemCpy(TILEMAP_ADDR(0xBE0), p + 3072, 0x60);
}

void Ripper_CorpseRottingInitFunc_2(void) {  // 0xA9DFC3
  uint8 *p = RomPtr_B7(addr_kDeadMonsters_TileData);
  MemCpy(TILEMAP_ADDR(0xC40), p + 2944, 0x60);
  MemCpy(TILEMAP_ADDR(0xCA0), p + 3456, 0x60);
}

void Skree_CorpseRottingInitFunc_0(void) {  // 0xA9DFE0
  uint8 *p = RomPtr_B7(addr_kDeadMonsters_TileData);
  MemCpy(TILEMAP_ADDR(0x640), p + 672, 0x40);
  MemCpy(TILEMAP_ADDR(0x680), p + 1184, 0x40);
  MemCpy(TILEMAP_ADDR(0x6C0), p + 1696, 0x40);
  MemCpy(TILEMAP_ADDR(0x700), p + 2208, 0x40);
}

void Skree_CorpseRottingInitFunc_2(void) {  // 0xA9E019
  uint8 *p = RomPtr_B7(addr_kDeadMonsters_TileData);
  MemCpy(TILEMAP_ADDR(0x740), p + 224, 0x40);
  MemCpy(TILEMAP_ADDR(0x780), p + 736, 0x40);
  MemCpy(TILEMAP_ADDR(0x7C0), p + 1248, 0x40);
  MemCpy(TILEMAP_ADDR(0x800), p + 1760, 0x40);
}

void Skree_CorpseRottingInitFunc_4(void) {  // 0xA9E052
  uint8 *p = RomPtr_B7(addr_kDeadMonsters_TileData);
  MemCpy(TILEMAP_ADDR(0x840), p + 448, 0x40);
  MemCpy(TILEMAP_ADDR(0x880), p + 960, 0x40);
  MemCpy(TILEMAP_ADDR(0x8C0), p + 1472, 0x40);
  MemCpy(TILEMAP_ADDR(0x900), p + 1984, 0x40);
}

void MotherBrain_CorpseRottingInitFunc(void) {  // 0xA9E08B
  uint8 *v0 = RomPtr_7E(0x9000);
  uint8 *v1 = RomPtr_B7(addr_kMotherBrain_Misc_TileData + 192);
  MemCpy(v0, v1, 0xC0);
  uint8 *v2 = RomPtr_7E(0x90E0);
  uint8 *v3 = RomPtr_B7(addr_kMotherBrain_Misc_TileData + 704);
  MemCpy(v2, v3, 0xC0);
  uint8 *v4 = RomPtr_7E(0x91C0);
  uint8 *v5 = RomPtr_B7(addr_kMotherBrain_Misc_TileData + 1216);
  MemCpy(v4, v5, 0xC0);
  uint8 *v6 = RomPtr_7E(0x92A0);
  uint8 *v7 = RomPtr_B7(addr_kMotherBrain_Misc_TileData + 1728);
  MemCpy(v6, v7, 0xC0);
  uint8 *v8 = RomPtr_7E(0x9380);
  uint8 *v9 = RomPtr_B7(addr_kMotherBrain_Misc_TileData + 2240);
  MemCpy(v8, v9, 0xE0);
  uint8 *v10 = RomPtr_7E(0x9460);
  uint8 *v11 = RomPtr_B7(addr_kMotherBrain_Misc_TileData + 2752);
  MemCpy(v10, v11, 0xE0);
}

void Torizo_CorpseRottingMoveFunc(uint16 j, uint16 k) {  // 0xA9E272
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (E->dms_var_41 >= 0x50) {
    if (sign16(E->dms_var_41 - 94)) {
      int v3 = k >> 1;
      int v4 = j >> 1;
      tilemap_stuff[v4 + 1] = tilemap_stuff[v3];
      tilemap_stuff[v4 + 9] = tilemap_stuff[v3 + 8];
    }
    int v5 = k >> 1;
    tilemap_stuff[v5] = 0;
    tilemap_stuff[v5 + 8] = 0;
  }
  if (E->dms_var_41 >= 0x48u) {
    if (sign16(E->dms_var_41 - 94)) {
      int v6 = k >> 1;
      int v7 = j >> 1;
      tilemap_stuff[v7 + 17] = tilemap_stuff[v6 + 16];
      tilemap_stuff[v7 + 25] = tilemap_stuff[v6 + 24];
    }
    int v8 = k >> 1;
    tilemap_stuff[v8 + 16] = 0;
    tilemap_stuff[v8 + 24] = 0;
  }
  if (E->dms_var_41 >= 0x10) {
    if (sign16(E->dms_var_41 - 94)) {
      int v9 = k >> 1;
      int v10 = j >> 1;
      tilemap_stuff[v10 + 33] = tilemap_stuff[v9 + 32];
      tilemap_stuff[v10 + 41] = tilemap_stuff[v9 + 40];
    }
    int v11 = k >> 1;
    tilemap_stuff[v11 + 32] = 0;
    tilemap_stuff[v11 + 40] = 0;
  }
  if (sign16(E->dms_var_41 - 94)) {
    int v12 = k >> 1;
    int v13 = j >> 1;
    tilemap_stuff[v13 + 49] = tilemap_stuff[v12 + 48];
    tilemap_stuff[v13 + 57] = tilemap_stuff[v12 + 56];
  }
  int v14 = k >> 1;
  tilemap_stuff[v14 + 48] = 0;
  tilemap_stuff[v14 + 56] = 0;
  if (sign16(E->dms_var_41 - 94)) {
    int v15 = j >> 1;
    tilemap_stuff[v15 + 65] = tilemap_stuff[v14 + 64];
    tilemap_stuff[v15 + 73] = tilemap_stuff[v14 + 72];
  }
  tilemap_stuff[v14 + 64] = 0;
  tilemap_stuff[v14 + 72] = 0;
  if (sign16(E->dms_var_41 - 94)) {
    int v16 = j >> 1;
    tilemap_stuff[v16 + 81] = tilemap_stuff[v14 + 80];
    tilemap_stuff[v16 + 89] = tilemap_stuff[v14 + 88];
  }
  tilemap_stuff[v14 + 80] = 0;
  tilemap_stuff[v14 + 88] = 0;
  if (sign16(E->dms_var_41 - 94)) {
    int v17 = j >> 1;
    tilemap_stuff[v17 + 97] = tilemap_stuff[v14 + 96];
    tilemap_stuff[v17 + 105] = tilemap_stuff[v14 + 104];
  }
  tilemap_stuff[v14 + 96] = 0;
  tilemap_stuff[v14 + 104] = 0;
  if (sign16(E->dms_var_41 - 94)) {
    int v18 = j >> 1;
    tilemap_stuff[v18 + 113] = tilemap_stuff[v14 + 112];
    tilemap_stuff[v18 + 121] = tilemap_stuff[v14 + 120];
  }
  tilemap_stuff[v14 + 112] = 0;
  tilemap_stuff[v14 + 120] = 0;
  if (sign16(E->dms_var_41 - 94)) {
    int v19 = j >> 1;
    tilemap_stuff[v19 + 129] = tilemap_stuff[v14 + 128];
    tilemap_stuff[v19 + 137] = tilemap_stuff[v14 + 136];
  }
  tilemap_stuff[v14 + 128] = 0;
  tilemap_stuff[v14 + 136] = 0;
  if (E->dms_var_41 >= 0x10) {
    if (sign16(E->dms_var_41 - 94)) {
      int v20 = j >> 1;
      tilemap_stuff[v20 + 145] = tilemap_stuff[v14 + 144];
      tilemap_stuff[v20 + 153] = tilemap_stuff[v14 + 152];
    }
    tilemap_stuff[v14 + 144] = 0;
    tilemap_stuff[v14 + 152] = 0;
  }
}

void Torizo_CorpseRottingCopyFunc(uint16 j, uint16 k) {  // 0xA9E38B
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (E->dms_var_41 >= 0x50u && sign16(E->dms_var_41 - 94)) {
    int v3 = k >> 1;
    int v4 = j >> 1;
    tilemap_stuff[v4 + 1] = tilemap_stuff[v3];
    tilemap_stuff[v4 + 9] = tilemap_stuff[v3 + 8];
  }
  if (E->dms_var_41 >= 0x48u && sign16(E->dms_var_41 - 94)) {
    int v5 = k >> 1;
    int v6 = j >> 1;
    tilemap_stuff[v6 + 17] = tilemap_stuff[v5 + 16];
    tilemap_stuff[v6 + 25] = tilemap_stuff[v5 + 24];
  }
  if (E->dms_var_41 >= 0x10u && sign16(E->dms_var_41 - 94)) {
    int v7 = k >> 1;
    int v8 = j >> 1;
    tilemap_stuff[v8 + 33] = tilemap_stuff[v7 + 32];
    tilemap_stuff[v8 + 41] = tilemap_stuff[v7 + 40];
  }
  if (sign16(E->dms_var_41 - 94)) {
    int v9 = k >> 1;
    int v10 = j >> 1;
    tilemap_stuff[v10 + 49] = tilemap_stuff[v9 + 48];
    tilemap_stuff[v10 + 57] = tilemap_stuff[v9 + 56];
  }
  if (sign16(E->dms_var_41 - 94)) {
    int v11 = k >> 1;
    int v12 = j >> 1;
    tilemap_stuff[v12 + 65] = tilemap_stuff[v11 + 64];
    tilemap_stuff[v12 + 73] = tilemap_stuff[v11 + 72];
  }
  if (sign16(E->dms_var_41 - 94)) {
    int v13 = k >> 1;
    int v14 = j >> 1;
    tilemap_stuff[v14 + 81] = tilemap_stuff[v13 + 80];
    tilemap_stuff[v14 + 89] = tilemap_stuff[v13 + 88];
  }
  if (sign16(E->dms_var_41 - 94)) {
    int v15 = k >> 1;
    int v16 = j >> 1;
    tilemap_stuff[v16 + 97] = tilemap_stuff[v15 + 96];
    tilemap_stuff[v16 + 105] = tilemap_stuff[v15 + 104];
  }
  if (sign16(E->dms_var_41 - 94)) {
    int v17 = k >> 1;
    int v18 = j >> 1;
    tilemap_stuff[v18 + 113] = tilemap_stuff[v17 + 112];
    tilemap_stuff[v18 + 121] = tilemap_stuff[v17 + 120];
  }
  if (sign16(E->dms_var_41 - 94)) {
    int v19 = k >> 1;
    int v20 = j >> 1;
    tilemap_stuff[v20 + 129] = tilemap_stuff[v19 + 128];
    tilemap_stuff[v20 + 137] = tilemap_stuff[v19 + 136];
  }
  if (E->dms_var_41 >= 0x10) {
    if (sign16(E->dms_var_41 - 94)) {
      int v21 = k >> 1;
      int v22 = j >> 1;
      tilemap_stuff[v22 + 145] = tilemap_stuff[v21 + 144];
      tilemap_stuff[v22 + 153] = tilemap_stuff[v21 + 152];
    }
  }
}

void Sidehopper_CorpseRottingMoveFunc_0(uint16 j, uint16 k) {  // 0xA9E468
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (E->dms_var_41 >= 8u) {
    if (sign16(E->dms_var_41 - 38)) {
      int v3 = k >> 1;
      int v4 = j >> 1;
      tilemap_stuff[v4 + 1] = tilemap_stuff[v3];
      tilemap_stuff[v4 + 9] = tilemap_stuff[v3 + 8];
    }
    int v5 = k >> 1;
    tilemap_stuff[v5] = 0;
    tilemap_stuff[v5 + 8] = 0;
  }
  if (E->dms_var_41 >= 8u) {
    if (sign16(E->dms_var_41 - 38)) {
      int v6 = k >> 1;
      int v7 = j >> 1;
      tilemap_stuff[v7 + 17] = tilemap_stuff[v6 + 16];
      tilemap_stuff[v7 + 25] = tilemap_stuff[v6 + 24];
    }
    int v8 = k >> 1;
    tilemap_stuff[v8 + 16] = 0;
    tilemap_stuff[v8 + 24] = 0;
  }
  if (sign16(E->dms_var_41 - 38)) {
    int v9 = k >> 1;
    int v10 = j >> 1;
    tilemap_stuff[v10 + 33] = tilemap_stuff[v9 + 32];
    tilemap_stuff[v10 + 41] = tilemap_stuff[v9 + 40];
  }
  int v11 = k >> 1;
  tilemap_stuff[v11 + 32] = 0;
  tilemap_stuff[v11 + 40] = 0;
  if (sign16(E->dms_var_41 - 38)) {
    int v12 = j >> 1;
    tilemap_stuff[v12 + 49] = tilemap_stuff[v11 + 48];
    tilemap_stuff[v12 + 57] = tilemap_stuff[v11 + 56];
  }
  tilemap_stuff[v11 + 48] = 0;
  tilemap_stuff[v11 + 56] = 0;
  if (sign16(E->dms_var_41 - 38)) {
    int v13 = j >> 1;
    tilemap_stuff[v13 + 65] = tilemap_stuff[v11 + 64];
    tilemap_stuff[v13 + 73] = tilemap_stuff[v11 + 72];
  }
  tilemap_stuff[v11 + 64] = 0;
  tilemap_stuff[v11 + 72] = 0;
}

void Sidehopper_CorpseRottingCopyFunc_0(uint16 j, uint16 k) {  // 0xA9E4F5
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (E->dms_var_41 >= 8u && sign16(E->dms_var_41 - 38)) {
    int v3 = k >> 1;
    int v4 = j >> 1;
    tilemap_stuff[v4 + 1] = tilemap_stuff[v3];
    tilemap_stuff[v4 + 9] = tilemap_stuff[v3 + 8];
  }
  if (E->dms_var_41 >= 8u && sign16(E->dms_var_41 - 38)) {
    int v5 = k >> 1;
    int v6 = j >> 1;
    tilemap_stuff[v6 + 17] = tilemap_stuff[v5 + 16];
    tilemap_stuff[v6 + 25] = tilemap_stuff[v5 + 24];
  }
  if (sign16(E->dms_var_41 - 38)) {
    int v7 = k >> 1;
    int v8 = j >> 1;
    tilemap_stuff[v8 + 33] = tilemap_stuff[v7 + 32];
    tilemap_stuff[v8 + 41] = tilemap_stuff[v7 + 40];
  }
  if (sign16(E->dms_var_41 - 38)) {
    int v9 = k >> 1;
    int v10 = j >> 1;
    tilemap_stuff[v10 + 49] = tilemap_stuff[v9 + 48];
    tilemap_stuff[v10 + 57] = tilemap_stuff[v9 + 56];
  }
  if (sign16(E->dms_var_41 - 38)) {
    int v11 = k >> 1;
    int v12 = j >> 1;
    tilemap_stuff[v12 + 65] = tilemap_stuff[v11 + 64];
    tilemap_stuff[v12 + 73] = tilemap_stuff[v11 + 72];
  }
}

void Sidehopper_CorpseRottingMoveFunc_2(uint16 j, uint16 k) {  // 0xA9E564
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (sign16(E->dms_var_41 - 38)) {
    int v3 = k >> 1;
    int v4 = j >> 1;
    tilemap_stuff[v4 + 401] = tilemap_stuff[v3 + 400];
    tilemap_stuff[v4 + 409] = tilemap_stuff[v3 + 408];
  }
  int v5 = k >> 1;
  tilemap_stuff[v5 + 400] = 0;
  tilemap_stuff[v5 + 408] = 0;
  if (sign16(E->dms_var_41 - 38)) {
    int v6 = j >> 1;
    tilemap_stuff[v6 + 417] = tilemap_stuff[v5 + 416];
    tilemap_stuff[v6 + 425] = tilemap_stuff[v5 + 424];
  }
  tilemap_stuff[v5 + 416] = 0;
  tilemap_stuff[v5 + 424] = 0;
  if (E->dms_var_41 >= 8u) {
    if (sign16(E->dms_var_41 - 38)) {
      int v7 = j >> 1;
      tilemap_stuff[v7 + 433] = tilemap_stuff[v5 + 432];
      tilemap_stuff[v7 + 441] = tilemap_stuff[v5 + 440];
    }
    tilemap_stuff[v5 + 432] = 0;
    tilemap_stuff[v5 + 440] = 0;
  }
  if (E->dms_var_41 >= 8u) {
    if (sign16(E->dms_var_41 - 38)) {
      int v8 = j >> 1;
      tilemap_stuff[v8 + 449] = tilemap_stuff[v5 + 448];
      tilemap_stuff[v8 + 457] = tilemap_stuff[v5 + 456];
    }
    tilemap_stuff[v5 + 448] = 0;
    tilemap_stuff[v5 + 456] = 0;
  }
  if (E->dms_var_41 >= 8u) {
    if (sign16(E->dms_var_41 - 38)) {
      int v9 = j >> 1;
      tilemap_stuff[v9 + 465] = tilemap_stuff[v5 + 464];
      tilemap_stuff[v9 + 473] = tilemap_stuff[v5 + 472];
    }
    tilemap_stuff[v5 + 464] = 0;
    tilemap_stuff[v5 + 472] = 0;
  }
}

void Sidehopper_CorpseRottingCopyFunc_2(uint16 j, uint16 k) {  // 0xA9E5F6
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (sign16(E->dms_var_41 - 38)) {
    int v3 = k >> 1;
    int v4 = j >> 1;
    tilemap_stuff[v4 + 401] = tilemap_stuff[v3 + 400];
    tilemap_stuff[v4 + 409] = tilemap_stuff[v3 + 408];
  }
  if (sign16(E->dms_var_41 - 38)) {
    int v5 = k >> 1;
    int v6 = j >> 1;
    tilemap_stuff[v6 + 417] = tilemap_stuff[v5 + 416];
    tilemap_stuff[v6 + 425] = tilemap_stuff[v5 + 424];
  }
  if (E->dms_var_41 >= 8u && sign16(E->dms_var_41 - 38)) {
    int v7 = k >> 1;
    int v8 = j >> 1;
    tilemap_stuff[v8 + 433] = tilemap_stuff[v7 + 432];
    tilemap_stuff[v8 + 441] = tilemap_stuff[v7 + 440];
  }
  if (E->dms_var_41 >= 8u && sign16(E->dms_var_41 - 38)) {
    int v9 = k >> 1;
    int v10 = j >> 1;
    tilemap_stuff[v10 + 449] = tilemap_stuff[v9 + 448];
    tilemap_stuff[v10 + 457] = tilemap_stuff[v9 + 456];
  }
  if (E->dms_var_41 >= 8u) {
    if (sign16(E->dms_var_41 - 38)) {
      int v11 = k >> 1;
      int v12 = j >> 1;
      tilemap_stuff[v12 + 465] = tilemap_stuff[v11 + 464];
      tilemap_stuff[v12 + 473] = tilemap_stuff[v11 + 472];
    }
  }
}

void Zoomer_CorpseRottingMoveFunc_0(uint16 j, uint16 k) {  // 0xA9E66A
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (sign16(E->dms_var_41 - 14)) {
    int v3 = k >> 1;
    int v4 = j >> 1;
    tilemap_stuff[v4 + 1185] = tilemap_stuff[v3 + 1184];
    tilemap_stuff[v4 + 1193] = tilemap_stuff[v3 + 1192];
  }
  int v5 = k >> 1;
  tilemap_stuff[v5 + 1184] = 0;
  tilemap_stuff[v5 + 1192] = 0;
  if (sign16(E->dms_var_41 - 14)) {
    int v6 = j >> 1;
    tilemap_stuff[v6 + 1201] = tilemap_stuff[v5 + 1200];
    tilemap_stuff[v6 + 1209] = tilemap_stuff[v5 + 1208];
  }
  tilemap_stuff[v5 + 1200] = 0;
  tilemap_stuff[v5 + 1208] = 0;
  if (sign16(E->dms_var_41 - 14)) {
    int v7 = j >> 1;
    tilemap_stuff[v7 + 1217] = tilemap_stuff[v5 + 1216];
    tilemap_stuff[v7 + 1225] = tilemap_stuff[v5 + 1224];
  }
  tilemap_stuff[v5 + 1216] = 0;
  tilemap_stuff[v5 + 1224] = 0;
}

void Zoomer_CorpseRottingCopyFunc_0(uint16 j, uint16 k) {  // 0xA9E6B9
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (sign16(E->dms_var_41 - 14)) {
    int v3 = k >> 1;
    int v4 = j >> 1;
    tilemap_stuff[v4 + 1185] = tilemap_stuff[v3 + 1184];
    tilemap_stuff[v4 + 1193] = tilemap_stuff[v3 + 1192];
  }
  if (sign16(E->dms_var_41 - 14)) {
    int v5 = k >> 1;
    int v6 = j >> 1;
    tilemap_stuff[v6 + 1201] = tilemap_stuff[v5 + 1200];
    tilemap_stuff[v6 + 1209] = tilemap_stuff[v5 + 1208];
  }
  if (sign16(E->dms_var_41 - 14)) {
    int v7 = k >> 1;
    int v8 = j >> 1;
    tilemap_stuff[v8 + 1217] = tilemap_stuff[v7 + 1216];
    tilemap_stuff[v8 + 1225] = tilemap_stuff[v7 + 1224];
  }
}

void Zoomer_CorpseRottingMoveFunc_2(uint16 j, uint16 k) {  // 0xA9E6F6
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (sign16(E->dms_var_41 - 14)) {
    int v3 = k >> 1;
    int v4 = j >> 1;
    tilemap_stuff[v4 + 1281] = tilemap_stuff[v3 + 1280];
    tilemap_stuff[v4 + 1289] = tilemap_stuff[v3 + 1288];
  }
  int v5 = k >> 1;
  tilemap_stuff[v5 + 1280] = 0;
  tilemap_stuff[v5 + 1288] = 0;
  if (sign16(E->dms_var_41 - 14)) {
    int v6 = j >> 1;
    tilemap_stuff[v6 + 1297] = tilemap_stuff[v5 + 1296];
    tilemap_stuff[v6 + 1305] = tilemap_stuff[v5 + 1304];
  }
  tilemap_stuff[v5 + 1296] = 0;
  tilemap_stuff[v5 + 1304] = 0;
  if (sign16(E->dms_var_41 - 14)) {
    int v7 = j >> 1;
    tilemap_stuff[v7 + 1313] = tilemap_stuff[v5 + 1312];
    tilemap_stuff[v7 + 1321] = tilemap_stuff[v5 + 1320];
  }
  tilemap_stuff[v5 + 1312] = 0;
  tilemap_stuff[v5 + 1320] = 0;
}

void Zoomer_CorpseRottingCopyFunc_2(uint16 j, uint16 k) {  // 0xA9E745
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (sign16(E->dms_var_41 - 14)) {
    int v3 = k >> 1;
    int v4 = j >> 1;
    tilemap_stuff[v4 + 1281] = tilemap_stuff[v3 + 1280];
    tilemap_stuff[v4 + 1289] = tilemap_stuff[v3 + 1288];
  }
  if (sign16(E->dms_var_41 - 14)) {
    int v5 = k >> 1;
    int v6 = j >> 1;
    tilemap_stuff[v6 + 1297] = tilemap_stuff[v5 + 1296];
    tilemap_stuff[v6 + 1305] = tilemap_stuff[v5 + 1304];
  }
  if (sign16(E->dms_var_41 - 14)) {
    int v7 = k >> 1;
    int v8 = j >> 1;
    tilemap_stuff[v8 + 1313] = tilemap_stuff[v7 + 1312];
    tilemap_stuff[v8 + 1321] = tilemap_stuff[v7 + 1320];
  }
}

void Zoomer_CorpseRottingMoveFunc_4(uint16 j, uint16 k) {  // 0xA9E782
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (sign16(E->dms_var_41 - 14)) {
    int v3 = k >> 1;
    int v4 = j >> 1;
    tilemap_stuff[v4 + 1377] = tilemap_stuff[v3 + 1376];
    tilemap_stuff[v4 + 1385] = tilemap_stuff[v3 + 1384];
  }
  int v5 = k >> 1;
  tilemap_stuff[v5 + 1376] = 0;
  tilemap_stuff[v5 + 1384] = 0;
  if (sign16(E->dms_var_41 - 14)) {
    int v6 = j >> 1;
    tilemap_stuff[v6 + 1393] = tilemap_stuff[v5 + 1392];
    tilemap_stuff[v6 + 1401] = tilemap_stuff[v5 + 1400];
  }
  tilemap_stuff[v5 + 1392] = 0;
  tilemap_stuff[v5 + 1400] = 0;
  if (sign16(E->dms_var_41 - 14)) {
    int v7 = j >> 1;
    tilemap_stuff[v7 + 1409] = tilemap_stuff[v5 + 1408];
    tilemap_stuff[v7 + 1417] = tilemap_stuff[v5 + 1416];
  }
  tilemap_stuff[v5 + 1408] = 0;
  tilemap_stuff[v5 + 1416] = 0;
}

void Zoomer_CorpseRottingCopyFunc_4(uint16 j, uint16 k) {  // 0xA9E7D1
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (sign16(E->dms_var_41 - 14)) {
    int v3 = k >> 1;
    int v4 = j >> 1;
    tilemap_stuff[v4 + 1377] = tilemap_stuff[v3 + 1376];
    tilemap_stuff[v4 + 1385] = tilemap_stuff[v3 + 1384];
  }
  if (sign16(E->dms_var_41 - 14)) {
    int v5 = k >> 1;
    int v6 = j >> 1;
    tilemap_stuff[v6 + 1393] = tilemap_stuff[v5 + 1392];
    tilemap_stuff[v6 + 1401] = tilemap_stuff[v5 + 1400];
  }
  if (sign16(E->dms_var_41 - 14)) {
    int v7 = k >> 1;
    int v8 = j >> 1;
    tilemap_stuff[v8 + 1409] = tilemap_stuff[v7 + 1408];
    tilemap_stuff[v8 + 1417] = tilemap_stuff[v7 + 1416];
  }
}

void Ripper_CorpseRottingMoveFunc_0(uint16 j, uint16 k) {  // 0xA9E80E
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (sign16(E->dms_var_41 - 14)) {
    int v3 = k >> 1;
    int v4 = j >> 1;
    tilemap_stuff[v4 + 1473] = tilemap_stuff[v3 + 1472];
    tilemap_stuff[v4 + 1481] = tilemap_stuff[v3 + 1480];
  }
  int v5 = k >> 1;
  tilemap_stuff[v5 + 1472] = 0;
  tilemap_stuff[v5 + 1480] = 0;
  if (sign16(E->dms_var_41 - 14)) {
    int v6 = j >> 1;
    tilemap_stuff[v6 + 1489] = tilemap_stuff[v5 + 1488];
    tilemap_stuff[v6 + 1497] = tilemap_stuff[v5 + 1496];
  }
  tilemap_stuff[v5 + 1488] = 0;
  tilemap_stuff[v5 + 1496] = 0;
  if (sign16(E->dms_var_41 - 14)) {
    int v7 = j >> 1;
    tilemap_stuff[v7 + 1505] = tilemap_stuff[v5 + 1504];
    tilemap_stuff[v7 + 1513] = tilemap_stuff[v5 + 1512];
  }
  tilemap_stuff[v5 + 1504] = 0;
  tilemap_stuff[v5 + 1512] = 0;
}

void Ripper_CorpseRottingCopyFunc_0(uint16 j, uint16 k) {  // 0xA9E85D
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (sign16(E->dms_var_41 - 14)) {
    int v3 = k >> 1;
    int v4 = j >> 1;
    tilemap_stuff[v4 + 1473] = tilemap_stuff[v3 + 1472];
    tilemap_stuff[v4 + 1481] = tilemap_stuff[v3 + 1480];
  }
  if (sign16(E->dms_var_41 - 14)) {
    int v5 = k >> 1;
    int v6 = j >> 1;
    tilemap_stuff[v6 + 1489] = tilemap_stuff[v5 + 1488];
    tilemap_stuff[v6 + 1497] = tilemap_stuff[v5 + 1496];
  }
  if (sign16(E->dms_var_41 - 14)) {
    int v7 = k >> 1;
    int v8 = j >> 1;
    tilemap_stuff[v8 + 1505] = tilemap_stuff[v7 + 1504];
    tilemap_stuff[v8 + 1513] = tilemap_stuff[v7 + 1512];
  }
}

void Ripper_CorpseRottingMoveFunc_2(uint16 j, uint16 k) {  // 0xA9E89A
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (sign16(E->dms_var_41 - 14)) {
    int v3 = k >> 1;
    int v4 = j >> 1;
    tilemap_stuff[v4 + 1569] = tilemap_stuff[v3 + 1568];
    tilemap_stuff[v4 + 1577] = tilemap_stuff[v3 + 1576];
  }
  int v5 = k >> 1;
  tilemap_stuff[v5 + 1568] = 0;
  tilemap_stuff[v5 + 1576] = 0;
  if (sign16(E->dms_var_41 - 14)) {
    int v6 = j >> 1;
    tilemap_stuff[v6 + 1585] = tilemap_stuff[v5 + 1584];
    tilemap_stuff[v6 + 1593] = tilemap_stuff[v5 + 1592];
  }
  tilemap_stuff[v5 + 1584] = 0;
  tilemap_stuff[v5 + 1592] = 0;
  if (sign16(E->dms_var_41 - 14)) {
    int v7 = j >> 1;
    tilemap_stuff[v7 + 1601] = tilemap_stuff[v5 + 1600];
    tilemap_stuff[v7 + 1609] = tilemap_stuff[v5 + 1608];
  }
  tilemap_stuff[v5 + 1600] = 0;
  tilemap_stuff[v5 + 1608] = 0;
}

void Ripper_CorpseRottingCopyFunc_2(uint16 j, uint16 k) {  // 0xA9E8E9
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (sign16(E->dms_var_41 - 14)) {
    int v3 = k >> 1;
    int v4 = j >> 1;
    tilemap_stuff[v4 + 1569] = tilemap_stuff[v3 + 1568];
    tilemap_stuff[v4 + 1577] = tilemap_stuff[v3 + 1576];
  }
  if (sign16(E->dms_var_41 - 14)) {
    int v5 = k >> 1;
    int v6 = j >> 1;
    tilemap_stuff[v6 + 1585] = tilemap_stuff[v5 + 1584];
    tilemap_stuff[v6 + 1593] = tilemap_stuff[v5 + 1592];
  }
  if (sign16(E->dms_var_41 - 14)) {
    int v7 = k >> 1;
    int v8 = j >> 1;
    tilemap_stuff[v8 + 1601] = tilemap_stuff[v7 + 1600];
    tilemap_stuff[v8 + 1609] = tilemap_stuff[v7 + 1608];
  }
}

void Skree_CorpseRottingMoveFunc_0(uint16 j, uint16 k) {  // 0xA9E926
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (sign16(E->dms_var_41 - 30)) {
    int v3 = k >> 1;
    int v4 = j >> 1;
    tilemap_stuff[v4 + 801] = tilemap_stuff[v3 + 800];
    tilemap_stuff[v4 + 809] = tilemap_stuff[v3 + 808];
  }
  int v5 = k >> 1;
  tilemap_stuff[v5 + 800] = 0;
  tilemap_stuff[v5 + 808] = 0;
  if (sign16(E->dms_var_41 - 30)) {
    int v6 = j >> 1;
    tilemap_stuff[v6 + 817] = tilemap_stuff[v5 + 816];
    tilemap_stuff[v6 + 825] = tilemap_stuff[v5 + 824];
  }
  tilemap_stuff[v5 + 816] = 0;
  tilemap_stuff[v5 + 824] = 0;
}

void Skree_CorpseRottingCopyFunc_0(uint16 j, uint16 k) {  // 0xA9E95B
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (sign16(E->dms_var_41 - 30)) {
    int v3 = k >> 1;
    int v4 = j >> 1;
    tilemap_stuff[v4 + 801] = tilemap_stuff[v3 + 800];
    tilemap_stuff[v4 + 809] = tilemap_stuff[v3 + 808];
  }
  if (sign16(E->dms_var_41 - 30)) {
    int v5 = k >> 1;
    int v6 = j >> 1;
    tilemap_stuff[v6 + 817] = tilemap_stuff[v5 + 816];
    tilemap_stuff[v6 + 825] = tilemap_stuff[v5 + 824];
  }
}

void Skree_CorpseRottingMoveFunc_2(uint16 j, uint16 k) {  // 0xA9E984
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (sign16(E->dms_var_41 - 30)) {
    int v3 = k >> 1;
    int v4 = j >> 1;
    tilemap_stuff[v4 + 929] = tilemap_stuff[v3 + 928];
    tilemap_stuff[v4 + 937] = tilemap_stuff[v3 + 936];
  }
  int v5 = k >> 1;
  tilemap_stuff[v5 + 928] = 0;
  tilemap_stuff[v5 + 936] = 0;
  if (sign16(E->dms_var_41 - 30)) {
    int v6 = j >> 1;
    tilemap_stuff[v6 + 945] = tilemap_stuff[v5 + 944];
    tilemap_stuff[v6 + 953] = tilemap_stuff[v5 + 952];
  }
  tilemap_stuff[v5 + 944] = 0;
  tilemap_stuff[v5 + 952] = 0;
}

void Skree_CorpseRottingCopyFunc_2(uint16 j, uint16 k) {  // 0xA9E9B9
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (sign16(E->dms_var_41 - 30)) {
    int v3 = k >> 1;
    int v4 = j >> 1;
    tilemap_stuff[v4 + 929] = tilemap_stuff[v3 + 928];
    tilemap_stuff[v4 + 937] = tilemap_stuff[v3 + 936];
  }
  if (sign16(E->dms_var_41 - 30)) {
    int v5 = k >> 1;
    int v6 = j >> 1;
    tilemap_stuff[v6 + 945] = tilemap_stuff[v5 + 944];
    tilemap_stuff[v6 + 953] = tilemap_stuff[v5 + 952];
  }
}

void Skree_CorpseRottingMoveFunc_4(uint16 j, uint16 k) {  // 0xA9E9E2
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (sign16(E->dms_var_41 - 30)) {
    int v3 = k >> 1;
    int v4 = j >> 1;
    tilemap_stuff[v4 + 1057] = tilemap_stuff[v3 + 1056];
    tilemap_stuff[v4 + 1065] = tilemap_stuff[v3 + 1064];
  }
  int v5 = k >> 1;
  tilemap_stuff[v5 + 1056] = 0;
  tilemap_stuff[v5 + 1064] = 0;
  if (sign16(E->dms_var_41 - 30)) {
    int v6 = j >> 1;
    tilemap_stuff[v6 + 1073] = tilemap_stuff[v5 + 1072];
    tilemap_stuff[v6 + 1081] = tilemap_stuff[v5 + 1080];
  }
  tilemap_stuff[v5 + 1072] = 0;
  tilemap_stuff[v5 + 1080] = 0;
}

void Skree_CorpseRottingCopyFunc_4(uint16 j, uint16 k) {  // 0xA9EA17
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (sign16(E->dms_var_41 - 30)) {
    int v3 = k >> 1;
    int v4 = j >> 1;
    tilemap_stuff[v4 + 1057] = tilemap_stuff[v3 + 1056];
    tilemap_stuff[v4 + 1065] = tilemap_stuff[v3 + 1064];
  }
  if (sign16(E->dms_var_41 - 30)) {
    int v5 = k >> 1;
    int v6 = j >> 1;
    tilemap_stuff[v6 + 1073] = tilemap_stuff[v5 + 1072];
    tilemap_stuff[v6 + 1081] = tilemap_stuff[v5 + 1080];
  }
}

void MotherBrain_CorpseRottingMoveFunc(uint16 j, uint16 k) {  // 0xA9EA40
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (E->dms_var_41 >= 0x10) {
    if (sign16(E->dms_var_41 - 46)) {
      *(uint16 *)((char *)&g_word_7E9002 + j) = *(uint16 *)((char *)&kraid_unk9000 + k);
      *(uint16 *)((char *)&g_word_7E9012 + j) = *(uint16 *)((char *)&g_word_7E900F + k + 1);
    }
    *(uint16 *)((char *)&kraid_unk9000 + k) = 0;
    *(uint16 *)((char *)&g_word_7E900F + k + 1) = 0;
  }
  if (E->dms_var_41 >= 8u) {
    if (sign16(E->dms_var_41 - 46)) {
      *(uint16 *)((char *)&g_word_7E9022 + j) = *(uint16 *)((char *)&g_word_7E9020 + k);
      *(uint16 *)((char *)&g_word_7E9032 + j) = *(uint16 *)((char *)&g_word_7E9030 + k);
    }
    *(uint16 *)((char *)&g_word_7E9020 + k) = 0;
    *(uint16 *)((char *)&g_word_7E9030 + k) = 0;
  }
  if (sign16(E->dms_var_41 - 46)) {
    *(uint16 *)((char *)&g_word_7E9042 + j) = *(uint16 *)((char *)&g_word_7E9040 + k);
    *(uint16 *)((char *)&g_word_7E9052 + j) = *(uint16 *)((char *)&g_word_7E9050 + k);
  }
  *(uint16 *)((char *)&g_word_7E9040 + k) = 0;
  *(uint16 *)((char *)&g_word_7E9050 + k) = 0;
  if (sign16(E->dms_var_41 - 46)) {
    *(uint16 *)((char *)&g_word_7E9062 + j) = *(uint16 *)((char *)&g_word_7E9060 + k);
    *(uint16 *)((char *)&g_word_7E9072 + j) = *(uint16 *)((char *)&g_word_7E9070 + k);
  }
  *(uint16 *)((char *)&g_word_7E9060 + k) = 0;
  *(uint16 *)((char *)&g_word_7E9070 + k) = 0;
  if (sign16(E->dms_var_41 - 46)) {
    *(uint16 *)((char *)&g_word_7E9082 + j) = *(uint16 *)((char *)&g_word_7E9080 + k);
    *(uint16 *)((char *)&g_word_7E9092 + j) = *(uint16 *)((char *)&g_word_7E9090 + k);
  }
  *(uint16 *)((char *)&g_word_7E9080 + k) = 0;
  *(uint16 *)((char *)&g_word_7E9090 + k) = 0;
  if (E->dms_var_41 >= 8u) {
    if (sign16(E->dms_var_41 - 46)) {
      *(uint16 *)((char *)&g_word_7E90A2 + j) = *(uint16 *)((char *)&g_word_7E90A0 + k);
      *(uint16 *)((char *)&g_word_7E90B2 + j) = *(uint16 *)((char *)&g_word_7E90B0 + k);
    }
    *(uint16 *)((char *)&g_word_7E90A0 + k) = 0;
    *(uint16 *)((char *)&g_word_7E90B0 + k) = 0;
  }
  if (E->dms_var_41 >= 0x20) {
    if (sign16(E->dms_var_41 - 46)) {
      *(uint16 *)((char *)&g_word_7E90C2 + j) = *(uint16 *)((char *)&g_word_7E90C0 + k);
      *(uint16 *)((char *)&g_word_7E90D2 + j) = *(uint16 *)((char *)&g_word_7E90D0 + k);
    }
    *(uint16 *)((char *)&g_word_7E90C0 + k) = 0;
    *(uint16 *)((char *)&g_word_7E90D0 + k) = 0;
  }
}

void MotherBrain_CorpseRottingCopyFunc(uint16 j, uint16 k) {  // 0xA9EB0B
  Enemy_DeadMonsters *E = Get_DeadMonsters(0);
  if (E->dms_var_41 >= 0x10u && sign16(E->dms_var_41 - 46)) {
    *(uint16 *)((char *)&g_word_7E9002 + j) = *(uint16 *)((char *)&kraid_unk9000 + k);
    *(uint16 *)((char *)&g_word_7E9012 + j) = *(uint16 *)((char *)&g_word_7E900F + k + 1);
  }
  if (E->dms_var_41 >= 8u && sign16(E->dms_var_41 - 46)) {
    *(uint16 *)((char *)&g_word_7E9022 + j) = *(uint16 *)((char *)&g_word_7E9020 + k);
    *(uint16 *)((char *)&g_word_7E9032 + j) = *(uint16 *)((char *)&g_word_7E9030 + k);
  }
  if (sign16(E->dms_var_41 - 46)) {
    *(uint16 *)((char *)&g_word_7E9042 + j) = *(uint16 *)((char *)&g_word_7E9040 + k);
    *(uint16 *)((char *)&g_word_7E9052 + j) = *(uint16 *)((char *)&g_word_7E9050 + k);
  }
  if (sign16(E->dms_var_41 - 46)) {
    *(uint16 *)((char *)&g_word_7E9062 + j) = *(uint16 *)((char *)&g_word_7E9060 + k);
    *(uint16 *)((char *)&g_word_7E9072 + j) = *(uint16 *)((char *)&g_word_7E9070 + k);
  }
  if (sign16(E->dms_var_41 - 46)) {
    *(uint16 *)((char *)&g_word_7E9082 + j) = *(uint16 *)((char *)&g_word_7E9080 + k);
    *(uint16 *)((char *)&g_word_7E9092 + j) = *(uint16 *)((char *)&g_word_7E9090 + k);
  }
  if (E->dms_var_41 >= 8u && sign16(E->dms_var_41 - 46)) {
    *(uint16 *)((char *)&g_word_7E90A2 + j) = *(uint16 *)((char *)&g_word_7E90A0 + k);
    *(uint16 *)((char *)&g_word_7E90B2 + j) = *(uint16 *)((char *)&g_word_7E90B0 + k);
  }
  if (E->dms_var_41 >= 0x20) {
    if (sign16(E->dms_var_41 - 46)) {
      *(uint16 *)((char *)&g_word_7E90C2 + j) = *(uint16 *)((char *)&g_word_7E90C0 + k);
      *(uint16 *)((char *)&g_word_7E90D2 + j) = *(uint16 *)((char *)&g_word_7E90D0 + k);
    }
  }
}

uint16 sub_A9ECD0(uint16 k, uint16 j) {  // 0xA9ECD0
  uint16 v2 = FUNC16(DeadMonsters_Func_1);
  Enemy_DeadMonsters *E = Get_DeadMonsters(k);
  if (E->dms_var_08)
    v2 = FUNC16(DeadMonsters_Func_5);
  E->dms_var_A = v2;
  return j;
}

uint8 Shitroid_Func_1(uint16 k, uint16 j) {  // 0xA9EED1
  Enemy_Shitroid *E = Get_Shitroid(j);
  Enemy_Shitroid *G = Get_Shitroid(k);
  R18_ = G->base.x_width + E->base.x_width + 1;
  uint16 v4 = abs16(E->base.x_pos - G->base.x_pos);
  bool v5 = v4 >= R18_;
  if (v4 < R18_) {
    R18_ = G->base.y_height + E->base.y_height + 1;
    return abs16(E->base.y_pos - G->base.y_pos) >= R18_;
  }
  return v5;
}

uint8 Shitroid_Func_2(uint16 k) {  // 0xA9EF06
  Enemy_Shitroid *E = Get_Shitroid(k);
  R26_ = E->base.x_width + R22_ + 1;
  uint16 v2 = abs16(R18_ - E->base.x_pos);
  bool v3 = v2 >= R26_;
  if (v2 < R26_) {
    R26_ = E->base.y_height + R24_ + 1;
    return abs16(R20_ - E->base.y_pos) >= R26_;
  }
  return v3;
}

void Shitroid_Init(void) {  // 0xA9EF37
  for (int i = 4094; i >= 0; i -= 2)
    tilemap_stuff[i >> 1] = 0;
  Enemy_Shitroid *E = Get_Shitroid(cur_enemy_index);
  E->base.properties |= kEnemyProps_DisableSamusColl | kEnemyProps_BlockPlasmaBeam;
  E->base.palette_index = 1024;
  E->base.current_instruction = addr_kShitroid_Ilist_F90E;
  E->base.instruction_timer = 1;
  E->base.timer = 0;
  uint16 v2 = FUNC16(Shitroid_Func_4);
  if ((layer1_x_pos & 0x8000) != 0) {
    E->base.properties |= kEnemyProps_Tangible | kEnemyProps_Invisible;
    v2 = FUNC16(Shitroid_Func_3);
  }
  E->shitr_var_A = v2;
  E->shitr_var_B = 0;
  E->shitr_var_C = 0;
  E->shitr_var_E = 10;
  E->shitr_parameter_2 = 0;
  WriteColorsToTargetPalette(0xa9, 0x120, addr_word_A9F8C6, 0x10);
  WriteColorsToTargetPalette(0xa9, 0x140, addr_kShitroidInCutscene_Palette, 0x10);
  WriteColorsToTargetPalette(0xa9, 0x1E0, addr_kDeadSidehopper_Palette_0, 0x10);
}

void Shitroid_Powerbomb(void) {  // 0xA9EFBA
  if (Get_Shitroid(cur_enemy_index)->shitr_parameter_2)
    Shitroid_Func_26(cur_enemy_index);
  Shitroid_Main();
}

void CallShitroidFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnShitroid_Func_3: Shitroid_Func_3(k); return;
  case fnShitroid_Func_4: Shitroid_Func_4(); return;
  case fnShitroid_Func_5: Shitroid_Func_5(k); return;
  case fnShitroid_Func_6: Shitroid_Func_6(k); return;
  case fnShitroid_Func_7: Shitroid_Func_7(k); return;
  case fnShitroid_Func_8: Shitroid_Func_8(k); return;
  case fnShitroid_Func_9: Shitroid_Func_9(k); return;
  case fnShitroid_Func_10: Shitroid_Func_10(k); return;
  case fnShitroid_Func_11: Shitroid_Func_11(k); return;
  case fnShitroid_Func_12: Shitroid_Func_12(k); return;
  case fnShitroid_Func_13: Shitroid_Func_13(k); return;
  case fnShitroid_Func_14: Shitroid_Func_14(k); return;
  case fnShitroid_Func_15: Shitroid_Func_15(k); return;
  case fnShitroid_Func_16: Shitroid_Func_16(k); return;
  case fnShitroid_Func_17: Shitroid_Func_17(k); return;
  case fnShitroid_Func_18: Shitroid_Func_18(k); return;
  case fnShitroid_Func_19: Shitroid_Func_19(k); return;
  case fnShitroid_Func_20: Shitroid_Func_20(); return;
  case fnShitroid_Func_21: Shitroid_Func_21(); return;
  case fnShitroid_Func_22: Shitroid_Func_22(k); return;
  case fnShitroid_Func_23: Shitroid_Func_23(); return;
  case fnShitroid_Func_24: Shitroid_Func_24(k); return;
  case fnShitroid_Func_25: Shitroid_Func_25(k); return;
  default: Unreachable();
  }
}

void Shitroid_Main(void) {  // 0xA9EFC5
  Enemy_Shitroid *E = Get_Shitroid(cur_enemy_index);
  E->base.health = 0x7FFF;
  CallShitroidFunc(E->shitr_var_A | 0xA90000, cur_enemy_index);
  MoveEnemyWithVelocity();
  if (!palette_change_num)
    Shitroid_HandleNormalPalette();
}

void Shitroid_Func_3(uint16 k) {  // 0xA9EFDF
  Enemy_Shitroid *E = Get_Shitroid(k);
  E->shitr_var_B = 0;
  E->shitr_var_C = 0;
}

static const SpawnHardcodedPlmArgs unk_A9F00E = { 0x30, 0x03, 0xb767 };
static const SpawnHardcodedPlmArgs unk_A9F016 = { 0x1f, 0x03, 0xb767 };

void Shitroid_Func_4(void) {  // 0xA9EFE6
  if (sign16(layer1_x_pos - 513)) {
    layer1_x_pos = 512;
    *(uint16 *)scrolls = scrolls[0];
    *(uint16 *)&scrolls[2] = scrolls[2];
    SpawnHardcodedPlm(&unk_A9F00E);
    SpawnHardcodedPlm(&unk_A9F016);
    Enemy_Shitroid *E = Get_Shitroid(cur_enemy_index);
    E->shitr_var_A = FUNC16(Shitroid_Func_5);
    E->shitr_var_04 = 1;
  }
}

void Shitroid_Func_5(uint16 k) {  // 0xA9F02B
  Enemy_Shitroid *E = Get_Shitroid(k);
  E->shitr_var_A = FUNC16(Shitroid_Func_6);
  E->shitr_var_F = 464;
  Shitroid_Func_6(k);
}

void Shitroid_Func_6(uint16 k) {  // 0xA9F037
  Enemy_Shitroid *E = Get_Shitroid(k);
  if ((--E->shitr_var_F & 0x8000) != 0) {
    QueueMusic_Delayed8(5u);
    E->shitr_var_A = FUNC16(Shitroid_Func_7);
    Shitroid_Func_7(k);
  }
}

void Shitroid_Func_7(uint16 k) {  // 0xA9F049
  R18_ = 584;
  R20_ = 74;
  Shitroid_Func_GraduallyAccelerateTowards0x400(k, 0xF);
  R22_ = 1;
  R24_ = 1;
  if (!(Shitroid_Func_2(k) & 1))
    Get_Shitroid(k)->shitr_var_A = FUNC16(Shitroid_Func_8);
}

void Shitroid_Func_8(uint16 k) {  // 0xA9F06D
  Enemy_Shitroid *E = Get_Shitroid(k + 64);
  R18_ = E->base.x_pos;
  R20_ = E->base.y_pos - 32;
  Shitroid_Func_GraduallyAccelerateTowards0x400(k, 0xF);
  if (!(Shitroid_Func_1(k, k + 64) & 1))
    Get_Shitroid(k)->shitr_var_A = FUNC16(Shitroid_Func_9);
}

void Shitroid_Func_9(uint16 k) {  // 0xA9F094
  Enemy_Shitroid *E1 = Get_Shitroid(k + 64);
  R18_ = E1->base.x_pos;
  R20_ = E1->base.y_pos - 32;
  if (Shitroid_AccelerateTowardsPoint(k, 0x200) & 1) {
    Enemy_Shitroid *E = Get_Shitroid(k);
    E->shitr_var_B = 0;
    E->shitr_var_C = 0;
    E->base.x_pos = E1->base.x_pos;
    E->base.y_pos = E1->base.y_pos - 32;
    E->base.current_instruction = addr_kShitroid_Ilist_F924;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
    E->shitr_var_A = FUNC16(Shitroid_Func_10);
    E->shitr_var_E = 1;
    E->shitr_parameter_2 = 0;
    E->shitr_var_F = 320;
  }
}

void Shitroid_Func_10(uint16 k) {  // 0xA9F0E6
  Enemy_Shitroid *E = Get_Shitroid(k);
  int v2 = (uint8)(E->base.frame_counter & 6) >> 1;
  Enemy_Shitroid *E1 = Get_Shitroid(k + 64);
  E->base.x_pos = E1->base.x_pos + g_word_A993BB[v2];
  E->base.y_pos = E1->base.y_pos + g_word_A993C3[v2] - 32;
  if (E->shitr_var_F-- == 1) {
    E->shitr_var_A = FUNC16(Shitroid_Func_11);
    E->base.current_instruction = addr_kShitroid_Ilist_F906;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
    E->shitr_var_E = 10;
  }
}

void Shitroid_Func_11(uint16 k) {  // 0xA9F125
  Get_Shitroid(0x40)->shitr_var_08 = 1;
  Enemy_Shitroid *E = Get_Shitroid(k);
  E->shitr_var_A = FUNC16(Shitroid_Func_12);
  E->shitr_var_F = 192;
  Shitroid_Func_12(k);
}

static const SpawnHardcodedPlmArgs unk_A9F173 = { 0x30, 0x03, 0xb763 };
static const SpawnHardcodedPlmArgs unk_A9F17B = { 0x1f, 0x03, 0xb763 };

void Shitroid_Func_12(uint16 k) {  // 0xA9F138
  Enemy_Shitroid *E = Get_Shitroid(k);
  R18_ = E->base.x_pos;
  R20_ = 104;
  Shitroid_Func_GraduallyAccelerateTowards0x400(k, 0);
  if ((--E->shitr_var_F & 0x8000) != 0) {
    E->shitr_var_A = FUNC16(Shitroid_Func_13);
    E->shitr_parameter_2 = 1;
    *(uint16 *)scrolls |= 0x100u;
    *(uint16 *)&scrolls[2] |= 0x100u;
    SpawnHardcodedPlm(&unk_A9F173);
    SpawnHardcodedPlm(&unk_A9F17B);
  }
}

void Shitroid_Func_13(uint16 k) {  // 0xA9F180
  Enemy_Shitroid *E = Get_Shitroid(k);
  uint16 v2 = abs16(E->base.x_pos - samus_x_pos);
  uint16 shitro_var_02;
  if (v2 >= 8u) {
    shitro_var_02 = E->shitr_var_02;
    if (shitro_var_02) {
      if ((--shitro_var_02 & 0x8000) != 0)
        shitro_var_02 = 0;
    }
  } else {
    shitro_var_02 = (v2 >= 8u) + E->shitr_var_02 + 2;
  }
  E->shitr_var_02 = shitro_var_02;
  if (shitro_var_02 >= 0x100u || sign16(samus_x_pos - 512)) {
    Get_Shitroid(cur_enemy_index)->shitr_var_A = FUNC16(Shitroid_Func_14);
  } else {
    uint16 shitro_var_01 = E->shitr_var_01;
    uint16 v5;
    if (shitro_var_01) {
      E->shitr_var_01 = shitro_var_01 - 1;
      v5 = samus_y_pos;
    } else {
      v5 = 80;
      if ((random_number & 0xFFFu) >= 0xFE0)
        E->shitr_var_01 = 32;
    }
    R20_ = v5;
    R18_ = samus_x_pos;
    Shitroid_Func_GraduallyAccelerateTowards0x400(cur_enemy_index, 0xA);
  }
}

void Shitroid_Func_14(uint16 k) {  // 0xA9F1FA
  R18_ = samus_x_pos;
  R20_ = samus_y_pos - 32;
  Shitroid_Func_GraduallyAccelerateTowards0x400(k, 0xF);
}

void Shitroid_Func_15(uint16 k) {  // 0xA9F20E
  CallSomeSamusCode(0x12u);
  Get_Shitroid(0)->shitr_var_A = FUNC16(Shitroid_Func_16);
  Shitroid_Func_16(k);
}

void Shitroid_Func_16(uint16 k) {  // 0xA9F21B
  Enemy_Shitroid *E = Get_Shitroid(k);
  if (samus_health < 2u) {
    samus_x_speed_divisor = 0;
    bomb_counter = 0;
    E->shitr_var_A = FUNC16(Shitroid_Func_17);
    E->shitr_var_B = 0;
    E->shitr_var_C = 0;
    Enemy_SetInstrList(k, addr_kShitroid_Ilist_F906);
    E->shitr_var_E = 10;
    CallSomeSamusCode(0x13u);
    SomeMotherBrainScripts(0);
    E->shitr_var_04 = 0;
    QueueMusic_Delayed8(7u);
  } else {
    cooldown_timer = 8;
    bomb_counter = 5;
    samus_x_speed_divisor = 2;
    if (!sign16(samus_y_speed - 4))
      samus_y_speed = 2;
    int v2 = (uint8)(E->base.frame_counter & 6) >> 1;
    E->base.x_pos = samus_x_pos + g_word_A993BB[v2];
    E->base.y_pos = samus_y_pos + g_word_A993C3[v2] - 20;
    Samus_DamageDueToShitroid();
  }
}

void Shitroid_Func_17(uint16 k) {  // 0xA9F2A2
  Enemy_Shitroid *E = Get_Shitroid(k);
  E->shitr_var_A = FUNC16(Shitroid_Func_18);
  E->shitr_var_F = 120;
  Shitroid_Func_18(k);
}

void Shitroid_Func_18(uint16 k) {  // 0xA9F2AE
  Enemy_Shitroid *E = Get_Shitroid(k);
  if ((--E->shitr_var_F & 0x8000) != 0) {
    E->shitr_var_A = FUNC16(Shitroid_Func_19);
    E->shitr_var_F = 192;
    Shitroid_Func_19(k);
  }
}

void Shitroid_Func_19(uint16 k) {  // 0xA9F2C0
  R18_ = samus_x_pos;
  R20_ = 104;
  Shitroid_Func_GraduallyAccelerateTowards0x400(cur_enemy_index, 0);
  Enemy_Shitroid *E = Get_Shitroid(cur_enemy_index);
  if ((--E->shitr_var_F & 0x8000) != 0) {
    QueueSfx2_Max6(0x7Du);
    E->shitr_var_A = FUNC16(Shitroid_Func_20);
    E->shitr_var_F = 88;
    E->base.current_instruction = addr_kShitroid_Ilist_F924;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
    Shitroid_Func_20();
  }
}

void Shitroid_Func_20(void) {  // 0xA9F2FB
  R18_ = samus_x_pos - 64;
  R20_ = 100;
  Shitroid_Func_GraduallyAccelerateTowards0x400(cur_enemy_index, 0);
  Enemy_Shitroid *E = Get_Shitroid(cur_enemy_index);
  if ((--E->shitr_var_F & 0x8000) != 0) {
    E->shitr_var_A = FUNC16(Shitroid_Func_21);
    E->shitr_var_F = 88;
    Shitroid_Func_21();
  }
}

void Shitroid_Func_21(void) {  // 0xA9F324
  R18_ = samus_x_pos + 96;
  R20_ = 104;
  Shitroid_Func_GraduallyAccelerateTowards0x400(cur_enemy_index, 0);
  Enemy_Shitroid *E = Get_Shitroid(cur_enemy_index);
  if ((--E->shitr_var_F & 0x8000) != 0) {
    E->shitr_var_A = FUNC16(Shitroid_Func_24);
    E->shitr_var_F = 256;
    E->base.current_instruction = addr_kShitroid_Ilist_F93A;
    E->base.instruction_timer = 1;
    E->base.timer = 0;
  }
}

void Shitroid_Func_22(uint16 k) {  // 0xA9F360
  QueueSfx2_Max6(0x52u);
  Get_Shitroid(k)->shitr_var_A = FUNC16(Shitroid_Func_23);
  Shitroid_Func_23();
}

void Shitroid_Func_23(void) {  // 0xA9F36D
  R18_ = -128;
  R20_ = 64;
  Shitroid_Func_GraduallyAccelerateTowards0x400(cur_enemy_index, 0);
  R22_ = 8;
  R24_ = 8;
  if (!(Shitroid_Func_2(cur_enemy_index) & 1)) {
    Enemy_Shitroid *E = Get_Shitroid(cur_enemy_index);
    E->shitr_var_B = 0;
    E->shitr_var_C = 0;
    E->base.properties &= ~(kEnemyProps_DisableSamusColl | kEnemyProps_Invisible);
    E->shitr_var_A = FUNC16(Shitroid_Func_3);
  }
}

void Shitroid_Func_24(uint16 k) {  // 0xA9F3A3
  Enemy_Shitroid *E = Get_Shitroid(k);
  if ((--E->shitr_var_F & 0x8000) != 0) {
    SomeMotherBrainScripts(2u);
    E->shitr_parameter_2 = 1;
    E->shitr_var_A = FUNC16(Shitroid_Func_25);
    Shitroid_Func_25(k);
  } else {
    Shitroid_Func_27(k);
  }
}

void Shitroid_Func_25(uint16 k) {  // 0xA9F3BE
  if (Shitroid_Func_27(k) & 1)
    Shitroid_F3C4(k);
}

void Shitroid_F3C4(uint16 k) {  // 0xA9F3C4
  Get_Shitroid(k)->shitr_var_A = FUNC16(Shitroid_Func_22);
}

void Shitroid_Func_26(uint16 k) {  // 0xA9F3CB
  if (Get_Shitroid(k)->shitr_var_A == (uint16)FUNC16(Shitroid_Func_25))
    Shitroid_F3C4(k);
}

uint8 Shitroid_Func_27(uint16 k) {  // 0xA9F3D4
  Enemy_Shitroid *E = Get_Shitroid(k);
  uint16 v2 = abs16(E->base.x_pos - samus_x_pos);
  uint16 shitro_var_02;
  if (v2 >= 2u) {
    shitro_var_02 = E->shitr_var_02;
    if (shitro_var_02) {
      if ((--shitro_var_02 & 0x8000) != 0)
        shitro_var_02 = 0;
    }
  } else {
    shitro_var_02 = (v2 >= 2u) + E->shitr_var_02 + 2;
  }
  E->shitr_var_02 = shitro_var_02;
  uint16 shitro_var_01 = E->shitr_var_01;
  uint16 v5;
  if (shitro_var_01) {
    E->shitr_var_01 = shitro_var_01 - 1;
    v5 = samus_y_pos - 18;
  } else {
    v5 = 80;
    if ((random_number & 0xFFFu) >= 0xFE0)
      E->shitr_var_01 = 32;
  }
  R20_ = v5;
  R18_ = samus_x_pos;
  Shitroid_Func_GraduallyAccelerateTowards0x400(cur_enemy_index, 8u);
  return Get_Shitroid(cur_enemy_index)->shitr_var_02 >= 0x400u || sign16(samus_x_pos - 128);
}

void Shitroid_Func_GraduallyAccelerateTowards0x400(uint16 k, uint16 j) {  // 0xA9F451
  R26_ = 1024;
  Shitroid_GraduallyAccelerateTowardsPt(k, j);
}

void Shitroid_GraduallyAccelerateTowards0x4(uint16 k, uint16 j) {  // 0xA9F458
  R26_ = 4;
  Shitroid_GraduallyAccelerateTowardsPt(k, j);
}

void Shitroid_GraduallyAccelerateTowards0x8(uint16 k, uint16 j) {  // 0xA9F45F
  R26_ = 8;
  Shitroid_GraduallyAccelerateTowardsPt(k, j);
}

void Shitroid_GraduallyAccelerateTowards0x10(uint16 k, uint16 j) {  // 0xA9F466
  R26_ = 16;
  Shitroid_GraduallyAccelerateTowardsPt(k, j);
}

void Shitroid_GraduallyAccelerateTowardsPt(uint16 k, uint16 j) {  // 0xA9F46B
  int16 v3;
  int16 v5;
  int16 v6;
  int16 shitr_var_C;
  int16 v9;

  R24_ = g_byte_A9F56A[j];
  Shitroid_GraduallyAccelerateHoriz(k);
  Enemy_Shitroid *E = Get_Shitroid(k);
  v3 = E->base.y_pos - R20_;
  if (v3) {
    if (v3 >= 0) {
      uint16 RegWord = SnesDivide(v3, R24_);
      if (!RegWord)
        RegWord = 1;
      R22_ = RegWord;
      shitr_var_C = E->shitr_var_C;
      if (shitr_var_C >= 0)
        shitr_var_C = shitr_var_C - 8 - R22_;
      v9 = shitr_var_C - R22_;
      if (sign16(v9 + 1280))
        v9 = -1280;
      E->shitr_var_C = v9;
    } else {
      uint16 v4 = SnesDivide(R20_ - E->base.y_pos, R24_);
      if (!v4)
        v4 = 1;
      R22_ = v4;
      v5 = E->shitr_var_C;
      if (v5 < 0)
        v5 += R22_ + 8;
      v6 = R22_ + v5;
      if (!sign16(v6 - 1280))
        v6 = 1280;
      E->shitr_var_C = v6;
    }
  }
}

void Shitroid_GraduallyAccelerateHoriz(uint16 k) {  // 0xA9F4E6
  int16 v2;
  int16 v4;
  char v5; // cf
  int16 v7;
  int16 shitr_var_B;
  int16 v11;

  Enemy_Shitroid *E = Get_Shitroid(k);
  v2 = E->base.x_pos - R18_;
  if (v2) {
    if (v2 >= 0) {
      uint16 RegWord = SnesDivide(v2,  R24_);
      if (!RegWord)
        RegWord = 1;
      R22_ = RegWord;
      shitr_var_B = E->shitr_var_B;
      if (shitr_var_B >= 0) {
        uint16 v13 = E->shitr_var_B;
        v5 = Shitroid_CheckIfOnScreen(k) & 1;
        uint16 v10 = v13;
        if (v5)
          v10 = v13 - R26_;
        shitr_var_B = v10 - 8 - R22_;
      }
      v11 = shitr_var_B - R22_;
      if (sign16(v11 + 2048))
        v11 = -2048;
      E->shitr_var_B = v11;
    } else {
      uint16 v3 = SnesDivide(R18_ - E->base.x_pos, R24_);
      if (!v3)
        v3 = 1;
      R22_ = v3;
      v4 = E->shitr_var_B;
      if (v4 < 0) {
        uint16 v12 = E->shitr_var_B;
        v5 = Shitroid_CheckIfOnScreen(k) & 1;
        uint16 v6 = v12;
        if (v5)
          v6 = R26_ + v12;
        v4 = R22_ + v6 + 8;
      }
      v7 = R22_ + v4;
      if (!sign16(v7 - 2048))
        v7 = 2048;
      E->shitr_var_B = v7;
    }
  }
}


uint8 Shitroid_CheckIfOnScreen(uint16 k) {  // 0xA9F57A
  int16 y_pos;
  int16 v3;
  int16 x_pos;
  int16 v5;

  Enemy_Shitroid *E = Get_Shitroid(k);
  y_pos = E->base.y_pos;
  uint8 result = 1;
  if (y_pos >= 0) {
    v3 = y_pos + 96 - layer1_y_pos;
    if (v3 >= 0) {
      if (sign16(v3 - 416)) {
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

uint8 Shitroid_AccelerateTowardsPoint(uint16 k, uint16 a) {  // 0xA9F5A6
  R22_ = a;
  R28_ = 0;
  Shitroid_AccelerateTowardsX(k);
  Shitroid_AccelerateTowardsY(k);
  R28_ >>= 1;
  uint8 v2 = R28_ & 1;
  R28_ >>= 1;
  return v2;
}

void Shitroid_AccelerateTowardsY(uint16 k) {  // 0xA9F5B5
  Enemy_Shitroid *E = Get_Shitroid(k);
  int16 v2 = E->base.y_pos - R20_;
  if (!v2) {
    ++R28_;
    return;
  }
  if (v2 >= 0) {
    uint16 v7 = E->shitr_var_C - R22_;
    if (sign16(v7 + 1280))
      v7 = -1280;
    E->shitr_var_C = v7;
    if ((int16)(E->base.y_pos + (int8)(v7 >> 8) - R20_) <= 0) {
      E->shitr_var_C = 0;
      ++R28_;
    }
  } else {
    uint16 v3 = R22_ + E->shitr_var_C;
    if (!sign16(v3 - 1280))
      v3 = 1280;
    E->shitr_var_C = v3;
    if ((int16)(E->base.y_pos + (int8)(v3 >> 8) - R20_) >= 0) {
      E->shitr_var_C = 0;
      ++R28_;
    }
  }
}

void Shitroid_AccelerateTowardsX(uint16 k) {  // 0xA9F615
  Enemy_Shitroid *E = Get_Shitroid(k);
  if ((int16)(E->base.x_pos - R18_) >= 0) {
    uint16 v6 = E->shitr_var_B - R22_;
    if (sign16(v6 + 1280))
      v6 = -1280;
    E->shitr_var_B = v6;
    if ((int16)(E->base.x_pos + (int8)(v6 >> 8) - R18_) <= 0) {
      E->shitr_var_B = 0;
      ++R28_;
    }
  } else {
    uint16 v2 = R22_ + E->shitr_var_B;
    if (!sign16(v2 - 1280))
      v2 = 1280;
    E->shitr_var_B = v2;
    if ((int16)(E->base.x_pos + (int8)(v2 >> 8) - R18_) >= 0) {
      E->shitr_var_B = 0;
      ++R28_;
    }
  }
}

void Shitroid_HandleNormalPalette(void) {  // 0xA9F677
  R18_ = 330;
  R22_ = addr_word_A9F6D1;
  Shitroid_HandleCutscenePalette_Common();
}

void Shitroid_HandleCutscenePalette(void) {  // 0xA9F683
  R18_ = 490;
  R22_ = addr_word_A9F6D1;
  Shitroid_HandleCutscenePalette_Common();
}

void Shitroid_HandleCutscenePalette_LowHealth(void) {  // 0xA9F68F
  R18_ = 490;
  R22_ = addr_word_A9F711;
  Shitroid_HandleCutscenePalette_Common();
}

void Shitroid_HandleCutscenePalette_Common(void) {  // 0xA9F699
  char shitro_var_D_high;

  Enemy_Shitroid *E = Get_Shitroid(cur_enemy_index);
  shitro_var_D_high = HIBYTE(E->shitr_var_D);
  if (shitro_var_D_high) {
    HIBYTE(E->shitr_var_D) = shitro_var_D_high - 1;
  } else {
    HIBYTE(E->shitr_var_D) = E->shitr_var_E;
    uint8 v3 = (LOBYTE(E->shitr_var_D) + 1) & 7;
    LOBYTE(E->shitr_var_D) = v3;
    uint16 v4 = Shitroid_HandleCrySoundEffect(cur_enemy_index, v3);
    WriteColorsToPalette(R18_, 0xa9, R22_ + 8 * v4, 4u);
  }
}

uint16 Shitroid_HandleCrySoundEffect(uint16 k, uint16 a) {  // 0xA9F751
  if (a == 5) {
    Enemy_Shitroid *E = Get_Shitroid(k);
    if (E->shitr_var_04) {
      uint16 v3 = E->shitr_var_05 + 1;
      E->shitr_var_05 = v3;
      if (v3 >= 4u) {
        E->shitr_var_05 = 0;
        uint16 v4 = 114;
        if (E->shitr_var_E < 0xAu)
          v4 = 120;
        QueueSfx2_Max6(v4);
      }
    }
    return 5;
  }
  return a;
}

void Shitroid_Touch(void) {  // 0xA9F789
  Enemy_Shitroid *E = Get_Shitroid(cur_enemy_index);
  if (E->shitr_parameter_2) {
    Shitroid_Func_26(cur_enemy_index);
    if (samus_movement_type == 3 && !sign16(samus_x_pos - 512)) {
      R18_ = samus_x_pos - E->base.x_pos;
      R20_ = samus_y_pos - E->base.y_pos;
      R18_ = (uint8)(0x80 - CalculateAngleFromXY() + 0x80);
      uint16 v4 = Math_MultBySin(0x40);
      E->shitr_var_B += v4;
      uint16 v6 = Math_MultByCos(0x40);
      E->shitr_var_C += v6;
    } else if (E->shitr_var_A == (uint16)FUNC16(Shitroid_Func_14)) {
      R18_ = samus_x_pos;
      R20_ = samus_y_pos - 32;
      if (Shitroid_AccelerateTowardsPoint(cur_enemy_index, 0x200) & 1) {
        E->base.current_instruction = addr_kShitroid_Ilist_F924;
        E->base.instruction_timer = 1;
        E->base.timer = 0;
        E->shitr_var_E = 1;
        E->shitr_parameter_2 = 0;
        E->shitr_var_B = 0;
        E->shitr_var_C = 0;
        E->shitr_var_A = FUNC16(Shitroid_Func_15);
      }
    } else if (E->shitr_var_A == (uint16)FUNC16(Shitroid_Func_13)) {
      E->shitr_var_A = FUNC16(Shitroid_Func_14);
    }
  }
}

void Shitroid_Shot(void) {  // 0xA9F842
  Enemy_Shitroid *E = Get_Shitroid(cur_enemy_index);
  if (E->shitr_parameter_2) {
    Shitroid_Func_26(cur_enemy_index);
    R18_ = projectile_x_pos[0] - E->base.x_pos;
    R20_ = projectile_y_pos[0] - E->base.y_pos;
    R18_ = (uint8)-CalculateAngleFromXY();
    uint16 v1 = 8 * projectile_damage[collision_detection_index];
    if (v1 >= 0xF0)
      v1 = 240;
    uint16 a = v1;
    uint16 v2 = Math_MultBySin(v1);
    E->shitr_var_B += v2;
    uint16 v4 = Math_MultByCos(a);
    E->shitr_var_C += v4;
  }
}

uint16 Shitroid_Instr_3(uint16 k, uint16 j) {  // 0xA9F920
  return addr_kShitroid_Ilist_F90E;
}

uint16 Shitroid_Instr_4(uint16 k, uint16 j) {  // 0xA9F936
  return addr_kShitroid_Ilist_F924;
}

uint16 Shitroid_Instr_6(uint16 k, uint16 j) {  // 0xA9F990
  return addr_kShitroid_Ilist_F93A;
}

uint16 Shitroid_Instr_5(uint16 k, uint16 j) {  // 0xA9F994
  if ((random_number & 0x8000) == 0)
    return *(uint16 *)RomPtr_A9(j);
  QueueSfx2_Max6(0x52u);
  return j + 2;
}
