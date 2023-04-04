//  Rooms definitions
#include "ida_types.h"
#include "variables.h"
#include "sm_rtl.h"
#include "funcs.h"

void CallDoorDefSetupCode(uint32 ea);
void CallRoomSetupCode(uint32 ea);
void RoomCode_GenRandomExplodes(uint16 x_r18, uint16 y_r20);

void RoomSetup_AfterSavingAnimals(void) {  // 0x8F9194
  SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x3d, 0x0b, 0xbb30 });
  earthquake_type = 24;
  earthquake_timer = -1;
}

void RoomSetup_AutoDestroyWallAfterEscape(void) {  // 0x8F91A9
  SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x10, 0x87, 0xb964 });
}

void RoomSetup_TurnWallIntoShotblocks(void) {  // 0x8F91B2
  SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x0f, 0x0a, 0xb9ed });
}

void RoomSetup_ShakeDuringEscape(void) {  // 0x8F91BD
  earthquake_type = 6;
  earthquake_timer = -1;
  FxTypeFunc_20();
}

void RoomSetup_ScrollingSkyLand(void) {  // 0x8F91C9
  FxTypeFunc_20();
}

void RoomSetup_ScrollingSkyOcean(void) {  // 0x8F91CE
  RoomSetupAsm_ScrollingSkyOcean();
}

void RoomSetup_RunStatueUnlockAnims(void) {  // 0x8F91D7
  SpawnAnimtiles(addr_kAnimtiles_TourianStatue_Kraid);
  SpawnAnimtiles(addr_kAnimtiles_TourianStatue_Phantoon);
  SpawnAnimtiles(addr_kAnimtiles_TourianStatue_Draygon);
  SpawnAnimtiles(addr_kAnimtiles_TourianStatue_Ridley);
}

void DoorCode_StartWreckedShipTreadmillWest(void) {  // 0x8FB971
  SpawnAnimtiles(addr_kAnimtiles_WreckedShipTradmillRight);
  SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x04, 0x09, 0xb64b });
}

void DoorCode_Scroll6_Green(void) {  // 0x8FB981
  scrolls[6] = 2;
}

void DoorCode_Scroll0_Blue(void) {  // 0x8FB98C
  scrolls[0] = 1;
}

void DoorCode_Scroll13_Blue(void) {  // 0x8FB997
  scrolls[19] = 1;
}

void DoorCode_Scroll_4Blue_8Green(void) {  // 0x8FB9A2
  scrolls[4] = 0;
  scrolls[8] = 2;
}

void DoorCode_Scroll_8toB_Red(void) {  // 0x8FB9B3
  scrolls[8] = 0;
  scrolls[9] = 0;
  scrolls[10] = 0;
  scrolls[11] = 0;
}

void DoorCode_Scroll_LotsRed(void) {  // 0x8FB9CA
  scrolls[2] = 0;
  scrolls[3] = 0;
  scrolls[4] = 0;
  scrolls[5] = 0;
  scrolls[11] = 0;
  scrolls[12] = 0;
  scrolls[13] = 0;
  scrolls[17] = 0;
}

void DoorCode_Scroll_1_4_Green(void) {  // 0x8FB9F1
  scrolls[1] = 2;
  scrolls[4] = 2;
}

void DoorCode_Scroll_2_Blue(void) {  // 0x8FBA00
  scrolls[2] = 1;
}

void DoorCode_Scroll_17_Blue(void) {  // 0x8FBA0B
  scrolls[23] = 1;
}

void DoorCode_Scroll_4_Blue(void) {  // 0x8FBA16
  scrolls[4] = 1;
}

void DoorCode_Scroll_6_Green(void) {  // 0x8FBA21
  scrolls[6] = 2;
}

void DoorCode_Scroll_3_Green(void) {  // 0x8FBA2C
  scrolls[3] = 2;
}

void DoorCode_SetScroll_0(void) {  // 0x8FBD07
  scrolls[24] = 2;
  scrolls[28] = 2;
}

void DoorCode_SetScroll_1(void) {  // 0x8FBD16
  scrolls[5] = 1;
  scrolls[6] = 1;
}

void DoorCode_SetScroll_2(void) {  // 0x8FBD25
  scrolls[29] = 1;
}

void DoorCode_SetScroll_3(void) {  // 0x8FBD30
  scrolls[2] = 2;
  scrolls[3] = 2;
}

void DoorCode_SetScroll_4(void) {  // 0x8FBD3F
  scrolls[0] = 0;
  scrolls[1] = 2;
}

void DoorCode_SetScroll_5(void) {  // 0x8FBD50
  scrolls[11] = 2;
}

void DoorCode_SetScroll_6(void) {  // 0x8FBD5B
  scrolls[28] = 0;
  scrolls[29] = 1;
}

void DoorCode_SetScroll_7(void) {  // 0x8FBD6C
  scrolls[4] = 0;
}

void DoorCode_SetScroll_8(void) {  // 0x8FBD77
  scrolls[32] = 2;
  scrolls[36] = 2;
  scrolls[37] = 2;
}

void DoorCode_SetScroll_9(void) {  // 0x8FBD8A
  scrolls[2] = 1;
}

void DoorCode_SetScroll_10(void) {  // 0x8FBD95
  scrolls[0] = 2;
}

void DoorCode_SetScroll_11(void) {  // 0x8FBDA0
  scrolls[6] = 2;
  scrolls[7] = 2;
}

void DoorCode_SetScroll_12(void) {  // 0x8FBDAF
  scrolls[1] = 1;
  scrolls[2] = 0;
}

void DoorCode_SetScroll_13(void) {  // 0x8FBDC0
  scrolls[1] = 1;
  scrolls[3] = 0;
}

void DoorCode_SetScroll_14(void) {  // 0x8FBDD1
  scrolls[0] = 0;
  scrolls[4] = 1;
}

void DoorCode_SetScroll_15(void) {  // 0x8FBDE2
  scrolls[2] = 1;
  scrolls[3] = 1;
}

void DoorCode_SetScroll_16(void) {  // 0x8FBDF1
  scrolls[0] = 2;
  scrolls[1] = 2;
}

void DoorCode_SetScroll_17(void) {  // 0x8FBE00
  scrolls[1] = 2;
}

void DoorCode_SetScroll_18(void) {  // 0x8FBE0B
  scrolls[15] = 2;
  scrolls[18] = 2;
}

void DoorCode_SetScroll_19(void) {  // 0x8FBE1A
  scrolls[6] = 2;
}

void DoorCode_SetScroll_20(void) {  // 0x8FBE25
  scrolls[0] = 2;
  scrolls[1] = 1;
}

void DoorCode_SetScroll_21(void) {  // 0x8FBE36
  scrolls[2] = 2;
}

void DoorCode_SetScroll_22(void) {  // 0x8FBF9E
  scrolls[3] = 0;
  scrolls[4] = 0;
  scrolls[6] = 1;
  scrolls[7] = 1;
  scrolls[8] = 1;
}

void DoorCode_SetScroll_23(void) {  // 0x8FBFBB
  scrolls[1] = 1;
  scrolls[2] = 1;
  scrolls[3] = 1;
  scrolls[4] = 2;
  scrolls[6] = 0;
}

void DoorCode_SetScroll_24(void) {  // 0x8FBFDA
  scrolls[0] = 1;
  scrolls[1] = 1;
}

void DoorCode_SetScroll_25(void) {  // 0x8FBFE9
  scrolls[1] = 0;
  scrolls[0] = 1;
}

void DoorCode_SetScroll_26(void) {  // 0x8FBFFA
  scrolls[10] = 2;
}

void DoorCode_SetScroll_27(void) {  // 0x8FC005
  scrolls[0] = 1;
  scrolls[2] = 0;
}

void DoorCode_SetScroll_28(void) {  // 0x8FC016
  scrolls[0] = 2;
  scrolls[2] = 2;
}

void DoorCode_SetScroll_29(void) {  // 0x8FC025
  scrolls[6] = 1;
  scrolls[7] = 1;
  scrolls[8] = 0;
}

void DoorCode_SetScroll_30(void) {  // 0x8FC03A
  scrolls[2] = 0;
  scrolls[3] = 1;
}

void DoorCode_SetScroll_31(void) {  // 0x8FC04B
  scrolls[7] = 2;
}

void DoorCode_SetScroll_32(void) {  // 0x8FC056
  scrolls[1] = 0;
  scrolls[2] = 1;
}

void DoorCode_SetScroll_33(void) {  // 0x8FC067
  scrolls[3] = 0;
  scrolls[0] = 1;
}

void DoorCode_SetScroll_34(void) {  // 0x8FC078
  scrolls[1] = 1;
  scrolls[4] = 0;
}

void DoorCode_SetScroll_35(void) {  // 0x8FC089
  scrolls[0] = 1;
  scrolls[1] = 0;
  scrolls[2] = 0;
  scrolls[3] = 0;
}

void DoorCode_SetScroll_36(void) {  // 0x8FC0A2
  scrolls[0] = 2;
}

void DoorCode_SetScroll_37(void) {  // 0x8FC0AD
  scrolls[0] = 1;
  scrolls[1] = 1;
  scrolls[4] = 0;
}

void DoorCode_SetScroll_38(void) {  // 0x8FC0C2
  scrolls[0] = 1;
  scrolls[3] = 0;
}

void DoorCode_SetScroll_39(void) {  // 0x8FC0D3
  scrolls[0] = 1;
}

void DoorCode_SetScroll_40(void) {  // 0x8FC0DE
  scrolls[0] = 1;
  scrolls[1] = 0;
}

void DoorCode_SetScroll_41(void) {  // 0x8FC0EF
  scrolls[24] = 1;
}

void DoorCode_SetScroll_42(void) {  // 0x8FC0FA
  scrolls[3] = 0;
  scrolls[2] = 1;
}

void DoorCode_SetScroll_43(void) {  // 0x8FC10B
  scrolls[14] = 0;
}

void RoomCode_ScrollingSkyLand_(void) {  // 0x8FC116
  RoomCode_ScrollingSkyLand();
}

void RoomCode_ScrollingSkyOcean_(void) {  // 0x8FC11B
  RoomMainAsm_ScrollingSkyOcean();
}

void RoomCode_GenRandomExplodes_Nonblank(void) {  // 0x8FC131
  if (!time_is_frozen_flag && (nmi_frame_counter_word & 1) == 0) {
    uint16 Random = NextRandom();
    uint16 x = layer1_x_pos + (uint8)Random;
    uint16 y = layer1_y_pos + HIBYTE(Random);
    uint16 prod = Mult8x8(y >> 4, room_width_in_blocks);
    if ((level_data[prod + (x >> 4)] & 0x3FF) != 255)
      RoomCode_GenRandomExplodes(x, y);
  }
}

void RoomCode_ScrollingSkyLand_Shakes(void) {  // 0x8FC120
  RoomCode_ScrollingSkyLand();
  RoomCode_GenRandomExplodes_Nonblank();
  earthquake_timer |= 0x8000;
}

void RoomCode_ExplodeShakes(void) {  // 0x8FC124
  RoomCode_GenRandomExplodes_Nonblank();
  earthquake_timer |= 0x8000;
}

void RoomCode_GenRandomExplodes_4th(void) {  // 0x8FC183
  if (!time_is_frozen_flag && (nmi_frame_counter_word & 3) == 0) {
    uint16 Random = NextRandom();
    RoomCode_GenRandomExplodes(layer1_x_pos + (uint8)Random, layer1_y_pos + HIBYTE(Random));
  }
}

void RoomCode_GenRandomExplodes(uint16 x_r18, uint16 y_r20) {  // 0x8FC1A9
  static const uint8 kRoomCode_GenRandomExplodes_Sprite[8] = { 3, 3, 9, 12, 12, 18, 18, 21 };
  static const uint8 kRoomCode_GenRandomExplodes_Sfx[8] = { 0x24, 0, 0, 0x25, 0, 0, 0, 0 };

  // Bugfix, X is garbage
  uint16 v1 = NextRandom() & 0xF;
  if (v1 < 8) {
    uint16 v2 = kRoomCode_GenRandomExplodes_Sfx[v1];
    if (v2)
      QueueSfx2_Max6(v2);
  }
  CreateSpriteAtPos(x_r18, y_r20, kRoomCode_GenRandomExplodes_Sprite[v1 & 7], 0);
}

void RoomCode_ScrollRightDachora(void) {  // 0x8FC1E6
  if (scrolls[11] == 2 && layer1_y_pos < 0x500 && layer1_x_pos < 0x380)
    layer1_x_pos += (layer1_x_pos >= 0x380) + 3;
}

void RoomCode_8FC8C8(void) {  // 0x8FC8C8
  SpawnEprojWithRoomGfx(addr_kEproj_PrePhantomRoom, 0);
}

void RoomCode_SetupShaktoolRoomPlm(void) {  // 0x8FC8D3
  SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x00, 0x00, 0xb8eb });
}

void RoomCode_SetPauseCodeForDraygon(void) {  // 0x8FC8DD
  mov24(&pause_hook, fnPauseHook_DraygonRoom);
  mov24(&unpause_hook, fnUnpauseHook_DraygonRoom);
}

void PauseHook_DraygonRoom(void) {  // 0x8FC8F6
  irqhandler_next_handler = 4;
}

CoroutineRet UnpauseHook_DraygonRoom(void) {  // 0x8FC8FC
  if (hdma_object_channels_bitmask[1] == 8)
    irqhandler_next_handler = 12;
  return kCoroutineNone;
}

void RoomCode_SetCollectedMap(void) {  // 0x8FC90A
  uint16 v0 = *(uint16 *)&map_station_byte_array[area_index] | 1;
  *(uint16 *)&map_station_byte_array[area_index] = v0;
  has_area_map = v0;
}

void RoomCode_SetZebesTimebombEvent(void) {  // 0x8FC91F
  SetEventHappened(0xE);
  earthquake_type = 18;
  earthquake_timer = -1;
}

void RoomCode_SetLightHorizRoomShake(void) {  // 0x8FC933
  earthquake_type = 18;
  *(uint16 *)&room_main_asm_variables[2] = 18;
  *(uint16 *)room_main_asm_variables = 0;
  earthquake_timer = -1;
}

void RoomCode_SetMediumHorizRoomShake(void) {  // 0x8FC946
  earthquake_type = 21;
  earthquake_timer = -1;
}

void RoomCode_Escape4_SetMediumHorizRoomShake(void) {  // 0x8FC953
  SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x10, 0x10, 0xb968 });
  earthquake_type = 21;
  *(uint16 *)&room_main_asm_variables[2] = 21;
  *(uint16 *)room_main_asm_variables = 0;
  earthquake_timer = -1;
}

void RoomCode_SetCeresDoorSolid(void) {  // 0x8FC96E
  SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x0f, 0x26, 0xba48 });
  FxTypeFunc_2C_Haze();
}

void RoomCode_CeresColorMathHdma(void) {  // 0x8FC976
  FxTypeFunc_2C_Haze();
}

void RoomCode_CeresColorMathHdma_BgBase(void) {  // 0x8FC97B
  reg_BG12NBA = 102;
  FxTypeFunc_2C_Haze();
  hdma_data_table_in_ceres = 9;
}

void DoorCode_StartWreckedSkipTreadmill_East(void) {  // 0x8FE1D8
  SpawnAnimtiles(addr_kAnimtiles_WreckedShipTradmillLeft);
  SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x04, 0x09, 0xb64f });
}

void DoorCode_SetScroll_44(void) {  // 0x8FE1E8
  scrolls[1] = 1;
}

void DoorCode_SetScroll_45(void) {  // 0x8FE1F3
  scrolls[0] = 2;
}

void DoorCode_SetScroll_46(void) {  // 0x8FE1FE
  scrolls[3] = 0;
  scrolls[4] = 1;
}

void DoorCode_SetScroll_47(void) {  // 0x8FE20F
  scrolls[41] = 1;
}

void DoorCode_SetScroll_48(void) {  // 0x8FE21A
  scrolls[40] = 2;
  scrolls[46] = 2;
}

void DoorCode_SetScroll_49(void) {  // 0x8FE229
  scrolls[6] = 0;
  scrolls[7] = 0;
  scrolls[8] = 0;
  scrolls[9] = 0;
  scrolls[10] = 0;
  scrolls[11] = 0;
}

void DoorCode_SetupElevatubeFromSouth(void) {  // 0x8FE26C
  *(uint16 *)&room_main_asm_variables[4] = -256;
  *(uint16 *)&room_main_asm_variables[2] = 2496;
  *(uint16 *)&room_main_asm_variables[6] = -32;
  CallSomeSamusCode(0);
  SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x01, 0x00, 0xb8f9 });
}

void DoorCode_SetupElevatubeFromNorth(void) {  // 0x8FE291
  *(uint16 *)&room_main_asm_variables[4] = 256;
  strcpy((uint8 *)&room_main_asm_variables[2], "@");
  strcpy((uint8 *)&room_main_asm_variables[6], " ");
  CallSomeSamusCode(0);
  SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x01, 0x00, 0xb8f9 });
}

void RoomCode_Elevatube(void) {  // 0x8FE2B6
  uint16 v0;

  samus_x_pos = 128;
  samus_x_subpos = 0;
  int32 amt = INT16_SHL8(*(uint16 *)&room_main_asm_variables[4]);
  v0 = (amt + *(uint32 *)room_main_asm_variables) >> 16;
  *(uint16 *)room_main_asm_variables += amt;
  *(uint16 *)&room_main_asm_variables[2] = v0;
  Samus_MoveDown_NoSolidColl(amt);
  if ((uint16)(*(uint16 *)&room_main_asm_variables[6] + *(uint16 *)&room_main_asm_variables[4] + 3616) < 0x1C41)
    *(uint16 *)&room_main_asm_variables[4] += *(uint16 *)&room_main_asm_variables[6];
}

void DoorCode_ResetElevatubeNorthExit(void) {  // 0x8FE301
  CallSomeSamusCode(1);
}

void DoorCode_ResetElevatubeSouthExit(void) {  // 0x8FE309
  *(uint16 *)scrolls = 514;
  CallSomeSamusCode(1);
}

void DoorCode_SetScroll_50(void) {  // 0x8FE318
  scrolls[10] = 0;
  scrolls[11] = 1;
}

void UNUSED_DoorCode_SetScroll_51(void) {  // 0x8FE329
  scrolls[17] = 0;
  scrolls[16] = 1;
}

void UNUSED_DoorCode_SetScroll_52(void) {  // 0x8FE33A
  scrolls[4] = 2;
}

void DoorCode_SetScroll_53(void) {  // 0x8FE345
  scrolls[0] = 0;
  scrolls[4] = 1;
}

void DoorCode_SetScroll_54(void) {  // 0x8FE356
  scrolls[0] = 0;
  scrolls[1] = 1;
}

void DoorCode_SetScroll_55(void) {  // 0x8FE367
  scrolls[9] = 0;
  scrolls[10] = 1;
}

void DoorCode_SetScroll_56(void) {  // 0x8FE378
  scrolls[0] = 0;
  scrolls[2] = 0;
  scrolls[1] = 1;
}

void DoorCode_SetScroll_57(void) {  // 0x8FE38D
  scrolls[1] = 1;
}

void DoorCode_SetScroll_58(void) {  // 0x8FE398
  scrolls[6] = 1;
}

void DoorCode_SetScroll_59(void) {  // 0x8FE3A3
  scrolls[4] = 0;
}

void DoorCode_SetScroll_60(void) {  // 0x8FE3AE
  scrolls[1] = 0;
}

void DoorCode_SetScroll_61(void) {  // 0x8FE3B9
  scrolls[4] = 0;
  scrolls[7] = 0;
}

void DoorCode_SetScroll_62(void) {  // 0x8FE3C8
  scrolls[2] = 0;
  scrolls[1] = 1;
}

void DoorCode_SetScroll_63(void) {  // 0x8FE3D9
  scrolls[0] = 2;
  scrolls[2] = 2;
}

void DoorCode_SetScroll_64(void) {  // 0x8FE4C0
  scrolls[0] = 2;
  scrolls[1] = 2;
}

void DoorCode_SetScroll_65(void) {  // 0x8FE4CF
  scrolls[24] = 1;
  scrolls[25] = 0;
}

void DoorCode_CeresElevatorShaft(void) {  // 0x8FE4E0
  reg_BGMODE_fake = 7;
  WriteReg(BGMODE, 7);
  reg_M7A = 256;
  reg_M7D = 256;
  reg_M7B = 0;
  reg_M7C = 0;
  reg_M7X = 128;
  reg_M7Y = 1008;
  irq_enable_mode7 = 1;
  strcpy((uint8 *)room_main_asm_variables, "\"");
  strcpy((uint8 *)&room_main_asm_variables[2], "<");
}

void DoorCode_CeresElevatorShaft_2(void) {  // 0x8FE513
  reg_BGMODE_fake = 9;
  irq_enable_mode7 = 0;
}

void RoomCode_CeresElevatorShaft_(void) {  // 0x8FE51F
  RoomCode_CeresElevatorShaft();
}

static const uint16 kRoomCode_SpawnCeresFallingDebris_Tab[16] = {  // 0x8FE525
   0x50,  0x60,  0x70,  0x80,
   0x90,  0xa0,  0xb0,  0xc0,
   0xd0,  0xe0,  0xf0, 0x110,
  0x130, 0x150, 0x170, 0x190,
};

void RoomCode_SpawnCeresFallingDebris(void) {
  if (ceres_status && (-- * (uint16 *)room_main_asm_variables, *(int16 *)room_main_asm_variables < 0)) {
    room_main_asm_variables[0] = 8;
    room_main_asm_variables[1] = 0;
    uint16 v0 = (random_number & 0x8000) ? addr_stru_869742 : addr_stru_869734;
    SpawnEprojWithRoomGfx(v0, kRoomCode_SpawnCeresFallingDebris_Tab[random_number & 0xF]);
  }
}

void RoomCode_HandleCeresRidleyGetaway(void) {  // 0x8FE571
  if (ceres_status & 1)
    HandleCeresRidleyGetawayCutscene();
}

void RoomCode_ShakeScreenHorizDiag(void) {  // 0x8FE57C
  uint16 v0;
  if (*(uint16 *)room_main_asm_variables) {
    if (!--*(uint16 *)room_main_asm_variables) {
      v0 = 18;
LABEL_6:
      earthquake_type = v0;
    }
  } else if (NextRandom() < 0x200) {
    strcpy((uint8 *)room_main_asm_variables, "*");
    v0 = 23;
    goto LABEL_6;
  }
  RoomCode_GenRandomExplodes_4th();
}

void RoomCode_ShakeScreenHorizDiagStrong(void) {  // 0x8FE5A4
  int16 v0;

  if (*(uint16 *)room_main_asm_variables) {
    if (!--*(uint16 *)room_main_asm_variables) {
      v0 = 21;
LABEL_6:
      *(uint16 *)&room_main_asm_variables[2] = v0;
    }
  } else if (NextRandom() < 0x180) {
    strcpy((uint8 *)room_main_asm_variables, "*");
    v0 = 26;
    goto LABEL_6;
  }
  earthquake_type = *(uint16 *)&room_main_asm_variables[2];
  RoomCode_GenRandomExplodes_4th();
}

uint16 CallRoomDefStateSelect(uint32 ea, uint16 k) {
  switch (ea) {
  case fnRoomDefStateSelect_Finish: return RoomDefStateSelect_Finish(k);
  case fnRoomDefStateSelect_TourianBoss01: return RoomDefStateSelect_TourianBoss01(k);
  case fnRoomDefStateSelect_IsEventSet: return RoomDefStateSelect_IsEventSet(k);
  case fnRoomDefStateSelect_IsBossDead: return RoomDefStateSelect_IsBossDead(k);
  case fnRoomDefStateSelect_MorphBallMissiles: return RoomDefStateSelect_MorphBallMissiles(k);
  case fnRoomDefStateSelect_PowerBombs: return RoomDefStateSelect_PowerBombs(k);
  default: return Unreachable();
  }
}

void HandleRoomDefStateSelect(uint16 k) {  // 0x8FE5D2
  uint16 v1 = k + 11;
  do {
    event_pointer = get_RoomDefStateSelect_E6E5_Finish(v1)->code_ptr;
    v1 = CallRoomDefStateSelect(event_pointer | 0x8F0000, v1 + 2);
  } while (v1);
}

uint16 RoomDefStateSelect_Finish(uint16 k) {  // 0x8FE5E6
  roomdefroomstate_ptr = k;
  return 0;
}

uint16 RoomDefStateSelect_Door(uint16 k) {  // 0x8FE5EB
  const uint8 *v1 = RomPtr_8F(k);
  if (GET_WORD(v1) == door_def_ptr)
    return RoomDefStateSelect_Finish(GET_WORD(v1 + 2));
  else
    return k + 4;
}

uint16 RoomDefStateSelect_TourianBoss01(uint16 k) {  // 0x8FE5FF
  if (!(CheckBossBitForCurArea(1) & 1))
    return k + 2;
  const uint16 *v1 = (const uint16 *)RomPtr_8F(k);
  return RoomDefStateSelect_Finish(*v1);
}

uint16 RoomDefStateSelect_IsEventSet(uint16 k) {  // 0x8FE612
  const uint8 *v1 = RomPtr_8F(k);
  if (CheckEventHappened(*v1))
    return RoomDefStateSelect_Finish(GET_WORD(v1 + 1));
  else
    return k + 3;
}

uint16 RoomDefStateSelect_IsBossDead(uint16 k) {  // 0x8FE629
  const uint8 *v1 = RomPtr_8F(k);
  if (CheckBossBitForCurArea(*v1) & 1)
    return RoomDefStateSelect_Finish(GET_WORD(v1 + 1));
  else
    return k + 3;
}

uint16 RoomDefStateSelect_MorphBallMissiles(uint16 k) {  // 0x8FE652
  if ((collected_items & 4) == 0 || !samus_max_missiles)
    return k + 2;
  const uint16 *v1 = (const uint16 *)RomPtr_8F(k);
  return RoomDefStateSelect_Finish(*v1);
}

uint16 RoomDefStateSelect_PowerBombs(uint16 k) {  // 0x8FE669
  if (!samus_max_power_bombs)
    return k + 2;
  const uint16 *v1 = (const uint16 *)RomPtr_8F(k);
  return RoomDefStateSelect_Finish(*v1);
}

void CallRoomSetupCode(uint32 ea) {
  switch (ea) {
  case fnRoomSetup_AfterSavingAnimals: RoomSetup_AfterSavingAnimals(); return;
  case fnRoomSetup_AutoDestroyWallAfterEscape: RoomSetup_AutoDestroyWallAfterEscape(); return;
  case fnRoomSetup_TurnWallIntoShotblocks: RoomSetup_TurnWallIntoShotblocks(); return;
  case fnRoomSetup_ShakeDuringEscape: RoomSetup_ShakeDuringEscape(); return;
  case fnRoomSetup_ScrollingSkyLand: RoomSetup_ScrollingSkyLand(); return;
  case fnRoomSetup_ScrollingSkyOcean: RoomSetup_ScrollingSkyOcean(); return;
  case fnRoomSetup_RunStatueUnlockAnims: RoomSetup_RunStatueUnlockAnims(); return;
  case fnRoomCode_8FC8C8: RoomCode_8FC8C8(); return;
  case fnRoomCode_SetupShaktoolRoomPlm: RoomCode_SetupShaktoolRoomPlm(); return;
  case fnRoomCode_SetPauseCodeForDraygon: RoomCode_SetPauseCodeForDraygon(); return;
  case fnRoomCode_SetCollectedMap: RoomCode_SetCollectedMap(); return;
  case fnnullsub_132:
  case fnnullsub_133:
  case fnnullsub_134:
  case fnnullsub_135:
  case fnnullsub_136:
  case fnnullsub_137:
  case fnnullsub_138:
  case fnnullsub_139:
  case fnnullsub_140:
  case fnnullsub_141:
  case fnnullsub_142:
  case fnnullsub_143:
  case fnnullsub_144:
  case fnnullsub_145:
  case fnnullsub_146:
  case fnnullsub_147: return;
  case fnRoomCode_SetZebesTimebombEvent: RoomCode_SetZebesTimebombEvent(); return;
  case fnRoomCode_SetLightHorizRoomShake: RoomCode_SetLightHorizRoomShake(); return;
  case fnRoomCode_SetMediumHorizRoomShake: RoomCode_SetMediumHorizRoomShake(); return;
  case fnRoomCode_Escape4_SetMediumHorizRoomShake: RoomCode_Escape4_SetMediumHorizRoomShake(); return;
  case fnRoomCode_SetCeresDoorSolid: RoomCode_SetCeresDoorSolid(); return;
  case fnRoomCode_CeresColorMathHdma: RoomCode_CeresColorMathHdma(); return;
  case fnRoomCode_CeresColorMathHdma_BgBase: RoomCode_CeresColorMathHdma_BgBase(); return;
  default: Unreachable();
  }
}

void RunRoomSetupCode(void) {  // 0x8FE88F
  RoomDefRoomstate *RoomDefRoomstate;

  RoomDefRoomstate = get_RoomDefRoomstate(roomdefroomstate_ptr);
  if (RoomDefRoomstate->room_setup_code)
    CallRoomSetupCode(RoomDefRoomstate->room_setup_code | 0x8F0000);
}

void CallDoorDefSetupCode(uint32 ea) {
  switch (ea) {
  case fnDoorCode_StartWreckedShipTreadmillWest: DoorCode_StartWreckedShipTreadmillWest(); return;
  case fnDoorCode_Scroll6_Green: DoorCode_Scroll6_Green(); return;
  case fnDoorCode_Scroll0_Blue: DoorCode_Scroll0_Blue(); return;
  case fnDoorCode_Scroll13_Blue: DoorCode_Scroll13_Blue(); return;
  case fnDoorCode_Scroll_4Blue_8Green: DoorCode_Scroll_4Blue_8Green(); return;
  case fnDoorCode_Scroll_8toB_Red: DoorCode_Scroll_8toB_Red(); return;
  case fnDoorCode_Scroll_LotsRed: DoorCode_Scroll_LotsRed(); return;
  case fnDoorCode_Scroll_1_4_Green: DoorCode_Scroll_1_4_Green(); return;
  case fnDoorCode_Scroll_2_Blue: DoorCode_Scroll_2_Blue(); return;
  case fnDoorCode_Scroll_17_Blue: DoorCode_Scroll_17_Blue(); return;
  case fnDoorCode_Scroll_4_Blue: DoorCode_Scroll_4_Blue(); return;
  case fnDoorCode_Scroll_6_Green: DoorCode_Scroll_6_Green(); return;
  case fnDoorCode_Scroll_3_Green: DoorCode_Scroll_3_Green(); return;
  case fnDoorCode_SetScroll_0: DoorCode_SetScroll_0(); return;
  case fnDoorCode_SetScroll_1: DoorCode_SetScroll_1(); return;
  case fnDoorCode_SetScroll_2: DoorCode_SetScroll_2(); return;
  case fnDoorCode_SetScroll_3: DoorCode_SetScroll_3(); return;
  case fnDoorCode_SetScroll_4: DoorCode_SetScroll_4(); return;
  case fnDoorCode_SetScroll_5: DoorCode_SetScroll_5(); return;
  case fnDoorCode_SetScroll_6: DoorCode_SetScroll_6(); return;
  case fnDoorCode_SetScroll_7: DoorCode_SetScroll_7(); return;
  case fnDoorCode_SetScroll_8: DoorCode_SetScroll_8(); return;
  case fnDoorCode_SetScroll_9: DoorCode_SetScroll_9(); return;
  case fnDoorCode_SetScroll_10: DoorCode_SetScroll_10(); return;
  case fnDoorCode_SetScroll_11: DoorCode_SetScroll_11(); return;
  case fnDoorCode_SetScroll_12: DoorCode_SetScroll_12(); return;
  case fnDoorCode_SetScroll_13: DoorCode_SetScroll_13(); return;
  case fnDoorCode_SetScroll_14: DoorCode_SetScroll_14(); return;
  case fnDoorCode_SetScroll_15: DoorCode_SetScroll_15(); return;
  case fnDoorCode_SetScroll_16: DoorCode_SetScroll_16(); return;
  case fnDoorCode_SetScroll_17: DoorCode_SetScroll_17(); return;
  case fnDoorCode_SetScroll_18: DoorCode_SetScroll_18(); return;
  case fnDoorCode_SetScroll_19: DoorCode_SetScroll_19(); return;
  case fnDoorCode_SetScroll_20: DoorCode_SetScroll_20(); return;
  case fnDoorCode_SetScroll_21: DoorCode_SetScroll_21(); return;
  case fnDoorCode_SetScroll_22: DoorCode_SetScroll_22(); return;
  case fnDoorCode_SetScroll_23: DoorCode_SetScroll_23(); return;
  case fnDoorCode_SetScroll_24: DoorCode_SetScroll_24(); return;
  case fnDoorCode_SetScroll_25: DoorCode_SetScroll_25(); return;
  case fnDoorCode_SetScroll_26: DoorCode_SetScroll_26(); return;
  case fnDoorCode_SetScroll_28: DoorCode_SetScroll_28(); return;
  case fnDoorCode_SetScroll_29: DoorCode_SetScroll_29(); return;
  case fnDoorCode_SetScroll_30: DoorCode_SetScroll_30(); return;
  case fnDoorCode_SetScroll_31: DoorCode_SetScroll_31(); return;
  case fnDoorCode_SetScroll_32: DoorCode_SetScroll_32(); return;
  case fnDoorCode_SetScroll_33: DoorCode_SetScroll_33(); return;
  case fnDoorCode_SetScroll_34: DoorCode_SetScroll_34(); return;
  case fnDoorCode_SetScroll_35: DoorCode_SetScroll_35(); return;
  case fnDoorCode_SetScroll_36: DoorCode_SetScroll_36(); return;
  case fnDoorCode_SetScroll_37: DoorCode_SetScroll_37(); return;
  case fnDoorCode_SetScroll_38: DoorCode_SetScroll_38(); return;
  case fnDoorCode_SetScroll_39: DoorCode_SetScroll_39(); return;
  case fnDoorCode_SetScroll_40: DoorCode_SetScroll_40(); return;
  case fnDoorCode_SetScroll_41: DoorCode_SetScroll_41(); return;
  case fnDoorCode_SetScroll_42: DoorCode_SetScroll_42(); return;
  case fnDoorCode_SetScroll_43: DoorCode_SetScroll_43(); return;
  case fnDoorCode_StartWreckedSkipTreadmill_East: DoorCode_StartWreckedSkipTreadmill_East(); return;
  case fnDoorCode_SetScroll_44: DoorCode_SetScroll_44(); return;
  case fnDoorCode_SetScroll_45: DoorCode_SetScroll_45(); return;
  case fnDoorCode_SetScroll_46: DoorCode_SetScroll_46(); return;
  case fnDoorCode_SetScroll_47: DoorCode_SetScroll_47(); return;
  case fnDoorCode_SetScroll_48: DoorCode_SetScroll_48(); return;
  case fnDoorCode_SetScroll_49: DoorCode_SetScroll_49(); return;
  case fnDoorCode_SetupElevatubeFromSouth: DoorCode_SetupElevatubeFromSouth(); return;
  case fnDoorCode_SetupElevatubeFromNorth: DoorCode_SetupElevatubeFromNorth(); return;
  case fnDoorCode_ResetElevatubeNorthExit: DoorCode_ResetElevatubeNorthExit(); return;
  case fnDoorCode_ResetElevatubeSouthExit: DoorCode_ResetElevatubeSouthExit(); return;
  case fnDoorCode_SetScroll_50: DoorCode_SetScroll_50(); return;
  case fnDoorCode_SetScroll_53: DoorCode_SetScroll_53(); return;
  case fnDoorCode_SetScroll_54: DoorCode_SetScroll_54(); return;
  case fnDoorCode_SetScroll_55: DoorCode_SetScroll_55(); return;
  case fnDoorCode_SetScroll_56: DoorCode_SetScroll_56(); return;
  case fnDoorCode_SetScroll_57: DoorCode_SetScroll_57(); return;
  case fnDoorCode_SetScroll_58: DoorCode_SetScroll_58(); return;
  case fnDoorCode_SetScroll_59: DoorCode_SetScroll_59(); return;
  case fnDoorCode_SetScroll_61: DoorCode_SetScroll_61(); return;
  case fnDoorCode_SetScroll_62: DoorCode_SetScroll_62(); return;
  case fnDoorCode_SetScroll_63: DoorCode_SetScroll_63(); return;
  case fnDoorCode_SetScroll_64: DoorCode_SetScroll_64(); return;
  case fnDoorCode_SetScroll_65: DoorCode_SetScroll_65(); return;
  case fnDoorCode_CeresElevatorShaft: DoorCode_CeresElevatorShaft(); return;
  case fnDoorCode_CeresElevatorShaft_2: DoorCode_CeresElevatorShaft_2(); return;
  default: Unreachable();
  }
}

void RunDoorSetupCode(void) {  // 0x8FE8A3
  DoorDef *DD = get_DoorDef(door_def_ptr);
  if (DD->door_setup_code)
    CallDoorDefSetupCode(DD->door_setup_code | 0x8F0000);
}

void RoomCode_CrocomireShaking(void) {  // 0x8FE8CD
  uint16 ai_var_D;

  if ((enemy_data[0].properties & 0x200) == 0) {
    if (enemy_data[0].ai_var_A == 64) {
      reg_BG1VOFS = enemy_data[1].ai_var_D + bg1_y_offset + layer1_y_pos;
    } else if ((enemy_data[0].ai_var_B & 0x400) != 0) {
      if (sign16(--enemy_data[1].ai_var_D + 7)) {
        ai_var_D = enemy_data[1].ai_var_D - 2 * (enemy_data[1].ai_var_D + 7);
      } else {
        ai_var_D = enemy_data[1].ai_var_D;
      }
      reg_BG1VOFS += ai_var_D;
      reg_BG2VOFS = ai_var_D - 48;
    } else if (enemy_data[0].ai_var_C == 34 && enemy_data[0].ai_var_D) {
      --enemy_data[0].ai_var_D;
      if ((enemy_data[0].ai_var_D & 1) != 0)
        layer1_x_pos -= 4;
      else
        layer1_x_pos += 4;
    }
  }
}

static const int16 kRoomCode_RidleyRoomShaking_X[8] = { 0, 2, 2, 2, 0, -2, -2, -2 };
static const int16 kRoomCode_RidleyRoomShaking_Y[8] = { -2, -2, 0, 2, 2, 2, 0, -2 };

void RoomCode_RidleyRoomShaking(void) {  // 0x8FE950
  uint16 ai_var_A = enemy_data[4].ai_var_A;
  if (enemy_data[4].ai_var_A) {
    --enemy_data[4].ai_var_A;
    int v1 = ai_var_A - 1;
    reg_BG1HOFS += kRoomCode_RidleyRoomShaking_X[v1];
    reg_BG2HOFS += kRoomCode_RidleyRoomShaking_X[v1];
    reg_BG1VOFS += kRoomCode_RidleyRoomShaking_Y[v1];
    reg_BG2VOFS += kRoomCode_RidleyRoomShaking_Y[v1];
  }
}

void CallRoomCode(uint32 ea) {
  switch (ea) {
  case fnRoomCode_ScrollingSkyLand_: RoomCode_ScrollingSkyLand_(); return;
  case fnRoomCode_ScrollingSkyOcean_: RoomCode_ScrollingSkyOcean_(); return;
  case fnRoomCode_ScrollingSkyLand_Shakes: RoomCode_ScrollingSkyLand_Shakes(); return;
  case fnRoomCode_ExplodeShakes: RoomCode_ExplodeShakes(); return;
  case fnRoomCode_ScrollRightDachora: RoomCode_ScrollRightDachora(); return;
  case fnRoomCode_Elevatube: RoomCode_Elevatube(); return;
  case fnRoomCode_CeresElevatorShaft_: RoomCode_CeresElevatorShaft_(); return;
  case fnnullsub_148: return;
  case fnRoomCode_SpawnCeresFallingDebris: RoomCode_SpawnCeresFallingDebris(); return;
  case fnRoomCode_HandleCeresRidleyGetaway: RoomCode_HandleCeresRidleyGetaway(); return;
  case fnRoomCode_ShakeScreenHorizDiag: RoomCode_ShakeScreenHorizDiag(); return;
  case fnRoomCode_GenRandomExplodes_4th_: RoomCode_GenRandomExplodes_4th(); return;
  case fnRoomCode_ShakeScreenHorizDiagStrong: RoomCode_ShakeScreenHorizDiagStrong(); return;
  case fnRoomCode_CrocomireShaking: RoomCode_CrocomireShaking(); return;
  case fnRoomCode_RidleyRoomShaking: RoomCode_RidleyRoomShaking(); return;
  default: Unreachable();
  }
}

void RunRoomMainCode(void) {  // 0x8FE8BD
  if (room_main_code_ptr)
    CallRoomCode(room_main_code_ptr | 0x8F0000);
}
