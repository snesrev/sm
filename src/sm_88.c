// HDMA
#include "ida_types.h"
#include "variables.h"
#include "sm_rtl.h"
#include "funcs.h"


#define g_byte_88A206 ((uint8*)RomFixedPtr(0x88a206))
#define g_byte_88A286 ((uint8*)RomFixedPtr(0x88a286))
#define kPowerBombExplosionColors ((uint8*)RomFixedPtr(0x888d85))
#define g_byte_889079 ((uint8*)RomFixedPtr(0x889079))
#define g_word_88A938 ((uint16*)RomFixedPtr(0x88a938))
#define kHdmaScrollEntrys ((HdmaScrollEntry*)RomFixedPtr(0x88aec1))
#define g_word_88B589 ((uint16*)RomFixedPtr(0x88b589))
#define g_word_88B60A ((uint16*)RomFixedPtr(0x88b60a))
#define g_word_88D992 ((uint16*)RomFixedPtr(0x88d992))
#define g_byte_88E3C9 ((uint8*)RomFixedPtr(0x88e3c9))
#define g_word_88E833 ((uint16*)RomFixedPtr(0x88e833))
#define g_byte_88EA8B ((uint8*)RomFixedPtr(0x88ea8b))

void CallHdmaobjPreInstr(uint32 ea, uint16 k);
const uint8 *CallHdmaobjInstr(uint32 ea, uint16 k, const uint8 *j);
void HdmaobjPreInstr_FxType22_BG3Yscroll(uint16 k);
void HdmaobjPreInstr_WaterBG2XScroll_Func2(uint16 k);
void HdmaobjPreInstr_WaterBG2XScroll_Func1(uint16 k);
void HdmaobjPreInstr_HazeColorMathSubscreen_FadingIn(uint16 k);

uint16 nullsub_9(uint16 j) {  // 0x888074
  return j;
}

void InitializeLayerBlending(void) {  // 0x888075
  reg_W12SEL = 0;
  reg_W34SEL = 0;
  reg_WOBJSEL = 0;
  reg_TM = 19;
  reg_TS = 4;
  reg_TMW = 0;
  reg_TSW = 0;
  next_gameplay_CGWSEL = 2;
  next_gameplay_CGADSUB = 51;
}

uint16 LayerBlendFunc_4_PhantoonIntro(uint16 j) {  // 0x888090
  reg_TM = 17;
  reg_TS = 4;
  return j;
}

uint16 LayerBlendFunc_6(uint16 j) {  // 0x888099
  reg_TS = 20;
  next_gameplay_CGADSUB = 39;
  return j;
}

uint16 LayerBlendFunc_8(uint16 j) {  // 0x8880A2
  reg_TS = 20;
  next_gameplay_CGADSUB = 34;
  return j;
}

uint16 LayerBlendFunc_A(uint16 j) {  // 0x8880AB
  next_gameplay_CGADSUB = 50;
  return j;
}

uint16 LayerBlendFunc_C(uint16 j) {  // 0x8880B0
  next_gameplay_CGWSEL = 0;
  next_gameplay_CGADSUB = -94;
  return j;
}

uint16 nullsub_10(uint16 j) {  // 0x8880B7
  return j;
}

uint16 LayerBlendFunc_10(uint16 j) {  // 0x8880B8
  reg_W34SEL = 2;
  reg_WOBJSEL = 32;
  reg_TSW = 4;
  return j;
}

uint16 LayerBlendFunc_14(uint16 j) {  // 0x8880C5
  next_gameplay_CGADSUB = -77;
  return j;
}

uint16 LayerBlendFunc_16(uint16 j) {  // 0x8880CA
  LOBYTE(j) = 4;
  reg_TM = 17;
  reg_TS = 6;
  next_gameplay_CGADSUB = -79;
  return j;
}

uint16 LayerBlendFunc_1A(uint16 j) {  // 0x8880D9
  LOBYTE(j) = 4;
  reg_TM = 21;
  reg_TS = 2;
  next_gameplay_CGADSUB = 53;
  return j;
}

uint16 LayerBlendFunc_1C(uint16 j) {  // 0x8880E8
  reg_TM = 21;
  reg_TS = 2;
  next_gameplay_CGADSUB = 85;
  return j;
}

uint16 LayerBlendFunc_18(uint16 j) {  // 0x8880F5
  j = 2;
  if ((reg_NMITIMEN & 0x30) == 0x30) {
    next_gameplay_CGADSUB = 36;
    reg_TS = 19;
    reg_TM = 4;
  }
  return j;
}

uint16 nullsub_11(uint16 j) {  // 0x88810C
  return j;
}

uint16 LayerBlendFunc_26(uint16 j) {  // 0x88810D
  next_gameplay_CGADSUB = 119;
  return j;
}

uint16 LayerBlendFunc_28(uint16 j) {  // 0x888112
  next_gameplay_CGWSEL = 0;
  next_gameplay_CGADSUB = -77;
  if ((fx_layer_blending_config_c & 0x8000) == 0) {
    reg_COLDATA[0] = 37;
    reg_COLDATA[1] = 64;
    reg_COLDATA[2] = 0x80;
  }
  return j;
}

uint16 LayerBlendFunc_2A(uint16 j) {  // 0x88812A
  next_gameplay_CGWSEL = 0;
  next_gameplay_CGADSUB = -77;
  if ((fx_layer_blending_config_c & 0x8000) == 0) {
    reg_COLDATA[0] = 38;
    reg_COLDATA[1] = 66;
    reg_COLDATA[2] = 0x80;
  }
  return j;
}

uint16 LayerBlendFunc_2C(uint16 j) {  // 0x888142
  next_gameplay_CGWSEL = 0;
  return j;
}

uint16 LayerBlendFunc_2E(uint16 j) {  // 0x888145
  next_gameplay_CGADSUB = -77;
  return j;
}

uint16 LayerBlendFunc_32(uint16 j) {  // 0x88814A
  reg_TS = 68;
  next_gameplay_CGADSUB = -78;
  return j;
}

uint16 LayerBlendFunc_34(uint16 j) {  // 0x888153
  j = 6;
  return j;
}

uint16 LayerBlendFunc_24(uint16 j) {  // 0x888156
  reg_W12SEL = 0;
  reg_W34SEL = 2;
  reg_WOBJSEL = 32;
  reg_TM = 19;
  reg_TS = 4;
  reg_TMW = 19;
  reg_TSW = 4;
  next_gameplay_CGWSEL = 16;
  next_gameplay_CGADSUB = 51;
  return j;
}

void HandleLayerBlendingXrayCanShowBlocks(void) {  // 0x88817B
  reg_W12SEL = -56;
  reg_W34SEL = 8;
  reg_WOBJSEL = 0x80;
  reg_TM = 19;
  reg_TS = 4;
  reg_TMW = 3;
  reg_TSW = 4;
  next_gameplay_CGWSEL = 34;
  next_gameplay_CGADSUB = next_gameplay_CGADSUB & 0x80 | 0x73;
}

void HandleLayerBlendingXrayCantShowBlocks(void) {  // 0x8881A4
  reg_W12SEL = 0;
  reg_W34SEL = 8;
  reg_WOBJSEL = 0x80;
  reg_TM = 19;
  reg_TS = 4;
  reg_TMW = 3;
  reg_TSW = 4;
  next_gameplay_CGWSEL = 34;
  next_gameplay_CGADSUB = next_gameplay_CGADSUB & 0x80 | 0x61;
  if (room_ptr == addr_kRoom_cefb)
    reg_TM = 17;
}

void HandleLayerBlendingXrayFirefleaRoom(void) {  // 0x8881DB
  reg_W12SEL = 0;
  reg_W34SEL = 8;
  reg_WOBJSEL = 0x80;
  reg_TM = 19;
  reg_TS = 0;
  reg_TMW = 3;
  reg_TSW = 4;
  next_gameplay_CGWSEL = 32;
  next_gameplay_CGADSUB = -77;
}

void LayerBlendPowerBombFunc_0(void) {  // 0x888219
  reg_W12SEL = 0;
  reg_W34SEL = 8;
  reg_WOBJSEL = 0x80;
  next_gameplay_CGWSEL = 2;
  next_gameplay_CGADSUB = 55;
  reg_TMW = 0;
  reg_TSW = 4;
  reg_TM = 19;
  reg_TS = 4;
}

void LayerBlendPowerBombFunc_4(void) {  // 0x88823E
  reg_W12SEL = 0x80;
  reg_W34SEL = 8;
  reg_WOBJSEL = 0x80;
  next_gameplay_CGWSEL = 2;
  next_gameplay_CGADSUB = 55;
  reg_TMW = 0;
  reg_TSW = 6;
  reg_TM = 17;
  reg_TS = 6;
}

void LayerBlendPowerBombFunc_6(void) {  // 0x888263
  reg_W12SEL = 0;
  reg_W34SEL = 8;
  reg_WOBJSEL = 0x80;
  next_gameplay_CGWSEL = 2;
  next_gameplay_CGADSUB = 49;
  reg_TMW = 0;
  reg_TSW = 4;
  reg_TM = 19;
  reg_TS = 4;
}

static Func_Y_Y *const kLayerBlendFuncTable[27] = {
  nullsub_9,
  nullsub_9,
  LayerBlendFunc_4_PhantoonIntro,
  LayerBlendFunc_6,
  LayerBlendFunc_8,
  LayerBlendFunc_A,
  LayerBlendFunc_C,
  nullsub_10,
  LayerBlendFunc_10,
  LayerBlendFunc_10,
  LayerBlendFunc_14,
  LayerBlendFunc_16,
  LayerBlendFunc_18,
  LayerBlendFunc_1A,
  LayerBlendFunc_1C,
  LayerBlendFunc_18,
  nullsub_11,
  LayerBlendFunc_14,
  LayerBlendFunc_24,
  LayerBlendFunc_26,
  LayerBlendFunc_28,
  LayerBlendFunc_2A,
  LayerBlendFunc_2C,
  LayerBlendFunc_2E,
  LayerBlendFunc_18,
  LayerBlendFunc_32,
  LayerBlendFunc_34,
};

void LayerBlendingHandler(void) {  // 0x888000
  uint16 v0 = 0;
  uint8 v1 = fx_layer_blending_config_c;
  if ((uint8)fx_layer_blending_config_c) {
    InitializeLayerBlending();
    v0 = kLayerBlendFuncTable[v1 >> 1](v0);
  }
  if ((fx_layer_blending_config_c & 0x8000) == 0) {
    if ((fx_layer_blending_config_c & 0x4000) != 0) {
      HandleLayerBlendingXrayCanShowBlocks();
    } else if ((fx_layer_blending_config_c & 0x2000) != 0) {
      HandleLayerBlendingXrayCantShowBlocks();
    } else if ((fx_layer_blending_config_c & 0x1000) != 0) {
      HandleLayerBlendingXrayFirefleaRoom();
    }
  } else {
    HandleLayerBlendingPowerBomb(v0);
  }
}

static Func_V *const kLayerBlendPowerBombFuncs[4] = {  // 0x8881FE
  LayerBlendPowerBombFunc_0,
  LayerBlendPowerBombFunc_0,
  LayerBlendPowerBombFunc_4,
  LayerBlendPowerBombFunc_6,
};

void HandleLayerBlendingPowerBomb(uint16 j) {
  if (room_ptr == addr_kRoom_a66a)
    j = 6;
  kLayerBlendPowerBombFuncs[j >> 1]();
}

void EnableHdmaObjects(void) {  // 0x888288
  hdma_objects_enable_flag |= 0x8000;
}

void DisableHdmaObjects(void) {  // 0x888293
  hdma_objects_enable_flag &= ~0x8000;
}

void WaitUntilEndOfVblankAndClearHdma(void) {  // 0x88829E
// Patched away wait, it will wait until the very start of the next frame until it disabled hdma
// but we run the frames instantly so no need.
  WriteReg(MDMAEN, 0);
  WriteReg(HDMAEN, 0);
  sub_8882AC();
}

void sub_8882AC(void) {  // 0x8882AC
  reg_HDMAEN = 0;
  for (int i = 10; i >= 0; i -= 2)
    hdma_object_channels_bitmask[i >> 1] = 0;
}

void InitializeSpecialEffectsForNewRoom(void) {  // 0x8882C1
  earthquake_sfx_index = 0;
  earthquake_sfx_timer = 0;
  if (room_ptr == addr_kRoom_9804
      || room_ptr == addr_kRoom_96ba
      || room_ptr == addr_kRoom_b32e
      || room_ptr == addr_kRoom_b457
      || room_ptr == addr_kRoom_dd58
      || room_ptr == addr_kRoom_dede) {
    earthquake_sfx_timer = -1;
  }
  debug_disable_minimap = 0;
  for (int i = 32; i != 0x80; i += 16) {
    WriteReg((SnesRegs)(i + 17152), 0);
    WriteReg((SnesRegs)(i + 17153), 0x13);
    WriteReg((SnesRegs)(i + 17154), 0);
    WriteReg((SnesRegs)(i + 17155), 0);
    WriteReg((SnesRegs)(i + 17156), 0);
    WriteReg((SnesRegs)(i + 17157), 0);
    WriteReg((SnesRegs)(i + 17158), 0);
    WriteReg((SnesRegs)(i + 17160), 0);
    WriteReg((SnesRegs)(i + 17161), 0);
  }
  fx_y_subpos = 0;
  fx_y_pos = -1;
  lava_acid_y_subpos = 0;
  lava_acid_y_pos = -1;
  hud_bg3_xpos = 0;
  hud_bg3_ypos = 0;
  bg3_xpos = 0;
  bg3_ypos = 0;
  irq_enable_mode7 = 0;
  camera_distance_index = 0;
  tourian_entrance_statue_animstate = 0;
  tourian_entrance_statue_finished = 0;
  earthquake_timer = 0;
  phantom_related_layer_flag = 0;
  power_bomb_explosion_status = 0;
  power_bomb_flag = 0;
  power_bomb_explosion_radius = 0;
  fx_tilemap_ptr = 0;
  fx_type = 0;
  fx_base_y_subpos = 0x8000;
  fx_base_y_pos = 0;
  fx_target_y_pos = 0;
  fx_y_vel = 0;
  fx_liquid_options = 0;
  fx_timer = 0;
  tide_phase = 0;
  fx_y_suboffset = 0;
  fx_y_offset = 0;
  fx_layer_blending_config_a = 2;
  reg_BG3HOFS = 0;
  reg_BG3VOFS = 0;
  layer2_x_pos = 0;
  layer2_y_pos = 0;
  room_loading_irq_handler = 0;
  *(VoidP *)((uint8 *)&pause_hook.addr + 1) = -30720;
  *(VoidP *)((uint8 *)&unpause_hook.addr + 1) = -30720;
  pause_hook.addr = FUNC16(PauseHook_Empty);
  unpause_hook.addr = FUNC16(PauseHook_Empty);
  WriteReg(WMADDL, 0xF0);
  WriteReg(WMADDM, 0xFF);
  WriteReg(WMADDH, 1);
  reg_HDMAEN = 0;
  reg_COLDATA[0] = 32;
  reg_COLDATA[1] = 64;
  reg_COLDATA[2] = 0x80;
  reg_MOSAIC = 0;
  reg_TM = 19;
  reg_CGWSEL = 0;
  reg_CGADSUB = 0;
  reg_BG12NBA = 0;
  reg_BG34NBA = 4;
  reg_BG2SC = 73;
  reg_BG3SC = 90;
  gameplay_BG3SC = 90;
}

static uint16 SpawnHdmaObjectInner(uint16 k, uint16 *p, uint16 r18, uint16 r20, uint16 r24) {  // 0x888477
  int v2 = k >> 1;
  hdma_object_pre_instructions[v2] = FUNC16(nullsub_293);
  hdma_object_pre_instruction_bank[v2] = 136;
  hdma_object_instruction_list_pointers[v2] = p[1];
  hdma_object_instruction_timers[v2] = 1;
  hdma_object_timers[v2] = 0;
  hdma_object_A[v2] = 0;
  hdma_object_B[v2] = 0;
  hdma_object_C[v2] = 0;
  hdma_object_D[v2] = 0;
  hdma_object_channels_bitmask[v2] = swap16(r18);
  hdma_object_bank_slot[v2] = r24 | r20;
  WriteRegWord((SnesRegs)(r20 + DMAP0), *p);
  return k;
}

uint16 SpawnHdmaObject(uint8 db, const void *p) {  // 0x888435
  uint16 r18 = 1024, r20 = 32;
  uint16 v3 = 0;
  for (;;) {
    if (!hdma_object_channels_bitmask[v3 >> 1])
      return SpawnHdmaObjectInner(v3, (uint16 *)p, r18, r20, db << 8);
    bool v4 = r18 >> 15;
    r18 *= 2;
    if (v4)
      return -1;
    r20 += 16;
    v3 += 2;
    if (v3 == 12)
      return -1;
  }
}

void SpawnHdmaObjectToSlot0xA(uint8 db, const void *p) {  // 0x88840A
  SpawnHdmaObjectInner(0xa, (uint16 *)p, 0x8000, 0x70, db << 8);
}

void HdmaObjectHandler(void) {  // 0x8884B9
  int8 v1;

  HandleMusicQueue();
  if (!time_is_frozen_flag && (power_bomb_explosion_status & 0x4000) != 0) {
    power_bomb_explosion_status = 0x8000;
    static const SpawnHdmaObject_Args unk_8884D5 = { 0x40, 0x28, 0x8ace };
    static const SpawnHdmaObject_Args unk_8884DD = { 0x40, 0x29, 0x8b80 };
    SpawnHdmaObject(0x88, &unk_8884D5);
    SpawnHdmaObject(0x88, &unk_8884DD);
  }
  fx_layer_blending_config_c = fx_layer_blending_config_a;
  if ((hdma_objects_enable_flag & 0x8000) != 0) {
    reg_HDMAEN = 0;
    int i = 0;
    do {
      hdma_object_index = i;
      v1 = *((uint8 *)hdma_object_channels_bitmask + i);
      if (v1) {
        reg_HDMAEN |= v1;
        HdmaobjInstructionHandler(i);
        i = hdma_object_index;
      }
      i += 2;
    } while (i != 12);
    LayerBlendingHandler();
  }
}

void HdmaobjInstructionHandler(uint8 k) {  // 0x88851C
  int kh = k >> 1;
  CallHdmaobjPreInstr(hdma_object_pre_instruction_bank[kh] << 16 | hdma_object_pre_instructions[kh], k);
  if (hdma_object_instruction_timers[kh]-- == 1) {
    const uint8 *base = RomBankBase(*((uint8 *)hdma_object_bank_slot + k + 1));
    const uint8 *p = base + hdma_object_instruction_list_pointers[kh];
    while (GET_WORD(p) & 0x8000) {
      p = CallHdmaobjInstr(GET_WORD(p) | 0x880000, k, p + 2);
      if ((uintptr_t)p < 0x10000) {
        if (!p)
          return;
        p = base + (uintptr_t)p;
      }
    }
    hdma_object_instruction_timers[kh] = GET_WORD(p);
    hdma_object_table_pointers[kh] = GET_WORD(p + 2);
    hdma_object_instruction_list_pointers[kh] = p + 4 - base;
  }
}

const uint8 *HdmaobjInstr_Delete(uint16 k, const uint8 *hdp) {  // 0x888569
  hdma_object_channels_bitmask[k >> 1] = 0;
  return 0;
}

const uint8 *HdmaobjInstr_SetPreInstr(uint16 k, const uint8 *hdp) {  // 0x888570
  hdma_object_pre_instructions[k >> 1] = GET_WORD(hdp);
  *((uint8 *)hdma_object_pre_instruction_bank + k) = hdp[2];
  return hdp + 3;
}

const uint8 *HdmaobjInstr_ClearPreInstr(uint16 k, const uint8 *hdp) {  // 0x888584
  hdma_object_pre_instructions[k >> 1] = addr_locret_88858A;
  return hdp;
}

void CallHdmaobjInstrFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnHdmaobj_PreExplodeWhite: Hdmaobj_PreExplodeWhite(); return;
  case fnHdmaobj_PreExplodeYellow: Hdmaobj_PreExplodeYellow(); return;
  case fnHdmaobj_ExplodeYellow: Hdmaobj_ExplodeYellow(); return;
  case fnHdmaobj_ExplodeWhite: Hdmaobj_ExplodeWhite(); return;
  case fnHdmaobj_CleanUpTryCrystalFlash: Hdmaobj_CleanUpTryCrystalFlash(k); return;
  case fnCrystalFlashSetupPart1: CrystalFlashSetupPart1(); return;
  case fnCrystalFlashSetupPart2: CrystalFlashSetupPart2(); return;
  case fnCrystalFlashCleanup: CrystalFlashCleanup(k); return;
  case fnnullsub_113: return;
  case fnnullsub_114: return;
  case fnnullsub_357: return;
  case fnInitializeRainbowBeam: InitializeRainbowBeam(); return;
  case fnXray_SetupStage1_FreezeTimeBackup: Xray_SetupStage1_FreezeTimeBackup(k); return;
  case fnXray_SetupStage2_ReadBg1_2ndScreen: Xray_SetupStage2_ReadBg1_2ndScreen(); return;
  case fnXray_SetupStage3_ReadBg1_1stScreen: Xray_SetupStage3_ReadBg1_1stScreen(); return;
  case fnXray_SetupStage4: Xray_SetupStage4(); return;
  case fnXray_SetupStage5: Xray_SetupStage5(); return;
  case fnXray_SetupStage6: Xray_SetupStage6(); return;
  case fnXray_SetupStage7: Xray_SetupStage7(); return;
  case fnXray_SetupStage8_SetBackdropColor: Xray_SetupStage8_SetBackdropColor(); return;
  case fnInitializeSuitPickupHdma: InitializeSuitPickupHdma(); return;
  default: Unreachable();
  }
}

const uint8 *HdmaobjInstr_CallFarFunc(uint16 k, const uint8 *hdp) {  // 0x8885B4
  CallHdmaobjInstrFunc(Load24((LongPtr *)hdp), k);
  return hdp + 3;
}

const uint8 *HdmaobjInstr_Goto(uint16 k, const uint8 *hdp) {  // 0x8885EC
  return INSTRB_RETURN_ADDR(GET_WORD(hdp));
}

const uint8 *HdmaobjInstr_GotoRel(uint16 k, const uint8 *hdp) {  // 0x8885F1
  return hdp + (int8)*hdp;
}

const uint8 *HdmaobjInstr_DecrementAndGoto(uint16 k, const uint8 *hdp) {  // 0x888607
  int v2 = k >> 1;
  if (hdma_object_timers[v2]-- == 1)
    return hdp + 2;
  else
    return INSTRB_RETURN_ADDR(GET_WORD(hdp));
}

const uint8 *HdmaobjInstr_DecrementAndGotoRel(uint16 k, const uint8 *hdp) {  // 0x88860F
  int v2 = k >> 1;
  if (hdma_object_timers[v2]-- == 1)
    return hdp + 1;
  else
    return hdp + (int8)*hdp;
}

const uint8 *HdmaobjInstr_SetTimer(uint16 k, const uint8 *hdp) {  // 0x888616
  *((uint8 *)hdma_object_timers + k) = *hdp;
  return hdp + 1;
}

const uint8 *HdmaobjInstr_SetHdmaControl(uint16 k, const uint8 *hdp) {  // 0x888622
  WriteReg((SnesRegs)(LOBYTE(hdma_object_bank_slot[k >> 1]) + DMAP0), *hdp);
  return hdp + 1;
}

const uint8 *HdmaobjInstr_SetHdmaTarget(uint16 k, const uint8 *hdp) {  // 0x888637
  WriteReg((SnesRegs)(LOBYTE(hdma_object_bank_slot[k >> 1]) + BBAD0), *hdp);
  return hdp + 1;
}

const uint8 *HdmaobjInstr_SetHdmaTablePtr(uint16 k, const uint8 *hdp) {  // 0x88864C
  hdma_object_table_pointers[k >> 1] = GET_WORD(hdp);
  return hdp + 2;
}

const uint8 *HdmaobjInstr_SetHdmaTableBank(uint16 k, const uint8 *hdp) {  // 0x888655
  WriteReg((SnesRegs)(LOBYTE(hdma_object_bank_slot[k >> 1]) + A1B0), *hdp);
  return hdp + 1;
}

const uint8 *HdmaobjInstr_SetIndirectHdmaDataBank(uint16 k, const uint8 *hdp) {  // 0x88866A
  WriteReg((SnesRegs)(LOBYTE(hdma_object_bank_slot[k >> 1]) + DAS00), *hdp);
  return hdp + 1;
}

const uint8 *HdmaobjInstr_Sleep(uint16 k, const uint8 *hdp) {  // 0x888682
  const uint8 *base = RomBankBase(*((uint8 *)hdma_object_bank_slot + k + 1));
  hdma_object_instruction_list_pointers[k >> 1] = hdp - base - 2;
  return 0;
}

uint8 RaiseOrLowerFx(void) {  // 0x88868C
  if ((fx_target_y_pos & 0x8000) != 0)
    return 1;
  uint32 r24_r22 = (int16)fx_y_vel << 8;
  if ((fx_y_vel & 0x8000) == 0) {
    uint16 v2 = (r24_r22 + __PAIR32__(fx_base_y_pos, fx_base_y_subpos)) >> 16;
    fx_base_y_subpos += r24_r22;
    if ((v2 & 0x8000) != 0)
      v2 = -1;
    fx_base_y_pos = v2;
    if (fx_target_y_pos >= v2) {
      return 0;
    } else {
      fx_base_y_pos = fx_target_y_pos;
      fx_base_y_subpos = 0;
      return 1;
    }
  } else {
    uint16 v0 = (r24_r22 + __PAIR32__(fx_base_y_pos, fx_base_y_subpos)) >> 16;
    fx_base_y_subpos += r24_r22;
    if ((v0 & 0x8000) != 0)
      v0 = 0;
    fx_base_y_pos = v0;
    if (fx_target_y_pos >= v0) {
      fx_base_y_pos = fx_target_y_pos;
      fx_base_y_subpos = 0;
    }
    return fx_target_y_pos >= v0;
  }
}

void HdmaobjPreInstr_XrayFunc0_NoBeam(uint16 k) {  // 0x888732
  if ((button_config_run_b & joypad1_lastkeys) != 0) {
    CalculateXrayHdmaTable();
    ++demo_input_pre_instr;
  } else {
    demo_input_pre_instr = 3;
  }
}

void HdmaobjPreInstr_XrayFunc1_BeamWidening(uint16 k) {  // 0x888754
  uint32 v1; // kr00_4

  if ((button_config_run_b & joypad1_lastkeys) != 0) {
    v1 = __PAIR32__(demo_input_instr_timer, demo_input_instr_ptr) + 2048;
    demo_input_instr_timer = v1 >> 16;
    demo_input_instr_ptr = v1;

    bool v2 = __CFADD__uint16((uint16)v1, demo_input_new);
    demo_input_new += v1;
    demo_input += demo_input_instr_timer + v2;
    if (!sign16(demo_input - 11)) {
      demo_input_new = 0;
      demo_input = 10;
      ++demo_input_pre_instr;
    }
    CalculateXrayHdmaTable();
  } else {
    demo_input_pre_instr = 3;
  }
}

void HdmaobjPreInstr_XrayFunc2_FullBeam(uint16 k) {  // 0x8887AB
  if ((button_config_run_b & joypad1_lastkeys) != 0) {
    HandleMovingXrayUpDown();
    CalculateXrayHdmaTable();
  } else {
    ++demo_input_pre_instr;
  }
}

void HandleMovingXrayUpDown(void) {  // 0x8887C5
  if ((button_config_up & joypad1_lastkeys) != 0) {
    MoveXrayUp();
  } else if ((button_config_down & joypad1_lastkeys) != 0) {
    MoveXrayDown();
  }
}

void MoveXrayUp(void) {  // 0x8887E0
  bool v0; // cf
  int16 v1;

  if (sign16(xray_angle - 128)) {
    if (xray_angle != demo_input) {
      if ((int16)(xray_angle - demo_input) < 0
          || (v0 = xray_angle != 0, --xray_angle, (int16)(xray_angle - (!v0 + demo_input)) < 0)) {
        xray_angle = demo_input;
      }
    }
  } else if (demo_input + xray_angle != 256) {
    if ((int16)(demo_input + xray_angle - 256) >= 0
        || (v1 = (__PAIR32__(demo_input, xray_angle) + __PAIR32__(xray_angle, 1)) >> 16, ++xray_angle, v1 != 256)
        && (int16)(v1 - 256) >= 0) {
      xray_angle = 256 - demo_input;
    }
  }
}

void MoveXrayDown(void) {  // 0x888835
  int16 v0;
  bool v1; // cf

  if (sign16(xray_angle - 128)) {
    if (demo_input + xray_angle != 128) {
      if ((int16)(demo_input + xray_angle - 128) >= 0
          || (v0 = (__PAIR32__(demo_input, xray_angle) + __PAIR32__(xray_angle, 1)) >> 16, ++xray_angle, v0 != 128)
          && (int16)(v0 - 128) >= 0) {
        xray_angle = 128 - demo_input;
      }
    }
  } else if (xray_angle - demo_input != 128) {
    if ((int16)(xray_angle - demo_input - 128) < 0
        || (v1 = xray_angle != 0, --xray_angle, xray_angle - (!v1 + demo_input) != 128)
        && (int16)(xray_angle - (!v1 + demo_input) - 128) < 0) {
      xray_angle = demo_input + 128;
    }
  }
}

void CalculateXrayHdmaTable(void) {  // 0x888896
  int16 v0;
  if (samus_pose_x_dir == 4)
    v0 = (__PAIR32__(samus_x_pos - layer1_x_pos, samus_x_pos) - (layer1_x_pos | 0x30000)) >> 16;
  else
    v0 = samus_x_pos - layer1_x_pos + 3;
  uint16 v1;
  if (samus_movement_type == 5)
    v1 = (__PAIR32__(samus_y_pos - layer1_y_pos, samus_y_pos) - (layer1_y_pos | 0xC0000)) >> 16;
  else
    v1 = (__PAIR32__(samus_y_pos - layer1_y_pos, samus_y_pos) - (layer1_y_pos | 0x100000)) >> 16;
  if (v0 < 0) {
    if (samus_pose_x_dir != 4) {
off_screen:
      CalculateXrayHdmaTableInner(v0, v1, xray_angle, demo_input, true, (uint16*)(g_ram + 0x9800));
      return;
    }
  } else {
    if ((int16)(v0 - 256) < 0) {
      CalculateXrayHdmaTableInner(v0, v1, xray_angle, demo_input, false, (uint16 *)(g_ram + 0x9800));
      return;
    }
    if (samus_pose_x_dir != 8)
      goto off_screen;
  }

  for (int i = 510; i >= 0; i -= 2)    // Clear
    hdma_table_1[i >> 1] = 255;
}

void HdmaobjPreInstr_XrayFunc3_DeactivateBeam(uint16 k) {  // 0x888934
  int16 v1;
  VramWriteEntry *v4;

  mov24(&hdma_ptr_1, 0x980001);
  *(uint16 *)((uint8 *)&demo_num_input_frames + 1) = 0;
  demo_input_prev = 0;
  demo_input_prev_new = 0;
  demo_backup_prev_controller_input = 0;
  hdma_table_1[0] = 255;
  v1 = 4096;
  if (fx_type != 36) {
    v1 = 0x2000;
    if (CanXrayShowBlocks())
      v1 = 0x4000;
  }
  fx_layer_blending_config_c |= v1;
  palette_buffer[0] = 0;
  int v2 = hdma_object_index >> 1;
  reg_BG2HOFS = hdma_object_A[v2];
  reg_BG2VOFS = hdma_object_B[v2];
  reg_BG2SC = *((uint8 *)hdma_object_C + hdma_object_index);
  uint16 v3 = vram_write_queue_tail;
  if ((int16)(vram_write_queue_tail - 240) < 0) {
    v4 = gVramWriteEntry(vram_write_queue_tail);
    v4->size = 2048;
    v4->src.addr = ADDR16_OF_RAM(ram4000) + 4096;
    *(uint16 *)&v4->src.bank = 126;
    v4->vram_dst = (reg_BG2SC & 0xFC) << 8;
    vram_write_queue_tail = v3 + 7;
    ++demo_input_pre_instr;
  }
}

void HdmaobjPreInstr_XrayFunc4_DeactivateBeam(uint16 k) {  // 0x8889BA
  int16 v1;
  VramWriteEntry *v3;

  v1 = 4096;
  if (fx_type != 36) {
    v1 = 0x2000;
    if (CanXrayShowBlocks())
      v1 = 0x4000;
  }
  fx_layer_blending_config_c |= v1;
  uint16 v2 = vram_write_queue_tail;
  if ((int16)(vram_write_queue_tail - 240) < 0) {
    v3 = gVramWriteEntry(vram_write_queue_tail);
    v3->size = 2048;
    v3->src.addr = ADDR16_OF_RAM(ram4000) + 6144;
    *(uint16 *)&v3->src.bank = 126;
    v3->vram_dst = ((reg_BG2SC & 0xFC) << 8) + 1024;
    vram_write_queue_tail = v2 + 7;
    ++demo_input_pre_instr;
  }
}

void HdmaobjPreInstr_XrayFunc5_DeactivateBeam(uint16 k) {  // 0x888A08
  int16 v1;
  v1 = 4096;
  if (fx_type != 36) {
    v1 = 0x2000;
    if (CanXrayShowBlocks())
      v1 = 0x4000;
  }
  fx_layer_blending_config_c |= v1;
  if (time_is_frozen_flag) {
    time_is_frozen_flag = 0;
    demo_input_pre_instr = 0;
    demo_input_instr_timer = 0;
    demo_input_instr_ptr = 0;
    xray_angle = 0;
    demo_input = 0;
    demo_input_new = 0;
    mov24(&hdma_ptr_1, 0x980001);
    *(uint16 *)((uint8 *)&demo_num_input_frames + 1) = 0;
    demo_input_prev = 0;
    demo_input_prev_new = 0;
    demo_backup_prev_controller_input = 0;
    ResponsibleForXrayStandupGlitch();
    hdma_object_channels_bitmask[hdma_object_index >> 1] = 0;
    QueueSfx1_Max6(0xA);
    if ((uint8)fx_type != 36) {
      reg_COLDATA[2] = 0x80;
      reg_COLDATA[1] = 64;
      reg_COLDATA[0] = 32;
    }
    for (int i = 510/2; i >= 0; i--)
      hdma_table_1[i] = 0xff;
    if (samus_auto_cancel_hud_item_index) {
      hud_item_index = 0;
      samus_auto_cancel_hud_item_index = 0;
    }
  }
}

static Func_X_V *const kHdmaobjPreInstr_XrayFuncs[6] = {  // 0x8886EF
  HdmaobjPreInstr_XrayFunc0_NoBeam,
  HdmaobjPreInstr_XrayFunc1_BeamWidening,
  HdmaobjPreInstr_XrayFunc2_FullBeam,
  HdmaobjPreInstr_XrayFunc3_DeactivateBeam,
  HdmaobjPreInstr_XrayFunc4_DeactivateBeam,
  HdmaobjPreInstr_XrayFunc5_DeactivateBeam,
};

void HdmaobjPreInstr_Xray(uint16 k) {
  int16 v1;

  v1 = 4096;
  if (fx_type != 36) {
    v1 = 0x2000;
    if (CanXrayShowBlocks()) {
      v1 = 0x4000;
      *(uint16 *)&reg_COLDATA[0] = 0x27;
      *(uint16 *)&reg_COLDATA[1] = 0x47;
      *(uint16 *)&reg_COLDATA[2] = 0x87;
    }
  }
  fx_layer_blending_config_c |= v1;
  kHdmaobjPreInstr_XrayFuncs[demo_input_pre_instr](2 * demo_input_pre_instr);
}

void SpawnPowerBombExplosion(void) {  // 0x888AA4
  if (time_is_frozen_flag) {
    power_bomb_explosion_status = 0x4000;
  } else {
    QueueSfx1_Max15(1);
    power_bomb_explosion_status = 0x8000;

    static const SpawnHdmaObject_Args unk_888ABA = { 0x40, 0x28, 0x8ace };
    static const SpawnHdmaObject_Args unk_888AC2 = { 0x40, 0x29, 0x8b80 };
    SpawnHdmaObject(0x88, &unk_888ABA);
    SpawnHdmaObject(0x88, &unk_888AC2);
  }
}

static const uint16 kPowerBombPreExplosionRadiusSpeed = 0x3000;
static const uint16 kPowerBombPreExplosionRadiusAccel = 0x80;
static const uint16 g_word_888B96 = 3;
static const uint16 kPowerBombExplosionRadiusSpeed = 0;
static const uint16 kPowerBombExplosionRadiusAccel = 0x30;

void Hdmaobj_PreExplodeWhite(void) {  // 0x888B14
  offscreen_power_bomb_left_hdma = -1;
  offscreen_power_bomb_right_hdma = 0;
  power_bomb_pre_explosion_flash_radius = 1024;
  power_bomb_pre_explosion_radius_speed = kPowerBombPreExplosionRadiusSpeed;
}

void Hdmaobj_PreExplodeYellow(void) {  // 0x888B32
  pre_scaled_power_bomb_explosion_shape_def_ptr = addr_byte_889F06;
}

void Hdmaobj_ExplodeYellow(void) {  // 0x888B39
  power_bomb_explosion_radius = 1024;
  power_bomb_pre_explosion_radius_speed = kPowerBombExplosionRadiusSpeed;
}

void Hdmaobj_ExplodeWhite(void) {  // 0x888B47
  pre_scaled_power_bomb_explosion_shape_def_ptr = addr_kPowerBombExplosionShapeDef0;
}

void Hdmaobj_CleanUpTryCrystalFlash(uint16 v0) {  // 0x888B4E
  if (samus_x_pos != power_bomb_explosion_x_pos
      || samus_y_pos != power_bomb_explosion_y_pos
      || Hdmaobj_CrystalFlash() & 1) {
    power_bomb_flag = 0;
  }
  power_bomb_explosion_status = 0;
  int v1 = v0 >> 1;
  hdma_object_channels_bitmask[v1] = 0;
  hdma_object_channels_bitmask[v1 + 1] = 0;
  power_bomb_pre_explosion_flash_radius = 0;
  power_bomb_explosion_radius = 0;
  CallSomeSamusCode(0x1E);
}

void HdmaobjPreInstr_PowerBombExplode_SetWindowConf(uint16 k) {  // 0x888B8F
  fx_layer_blending_config_c |= 0x8000;
}

void HdmaobjPreInstr_PowerBombExplode_Stage5_Afterglow(uint16 k) {  // 0x888B98
  if ((power_bomb_explosion_status & 0x8000) != 0) {
    int v1 = (uint8)k >> 1;
    if ((--hdma_object_timers[v1] & 0x8000) != 0) {
      if ((*((uint8 *)hdma_object_D + (uint8)k))-- == 1) {
        hdma_object_instruction_timers[v1] = 1;
        hdma_object_instruction_list_pointers[v1] += 2;
      } else {
        if ((reg_COLDATA[0] & 0x1F) != 0)
          reg_COLDATA[0] = ((reg_COLDATA[0] & 0x1F) - 1) | 0x20;
        if ((reg_COLDATA[1] & 0x1F) != 0)
          reg_COLDATA[1] = ((reg_COLDATA[1] & 0x1F) - 1) | 0x40;
        if ((reg_COLDATA[2] & 0x1F) != 0)
          reg_COLDATA[2] = ((reg_COLDATA[2] & 0x1F) - 1) | 0x80;
        *((uint8 *)hdma_object_timers + (uint8)k) = g_word_888B96;
      }
    }
  }
}

void CalculatePowerBombHdma_LeftOfScreen(uint16 k, const uint8 *j) {  // 0x888BEA
  do {
    uint8 w = *j;
    uint8 c = power_bomb_explosion_x_pos_plus_0x100;
    if (__CFADD__uint8(w, c)) {
      power_bomb_explosion_right_hdma[k] = w + c;
      power_bomb_explosion_left_hdma[k] = 0;
    } else {
      power_bomb_explosion_right_hdma[k] = 0;
      power_bomb_explosion_left_hdma[k] = 1;
    }
    ++j;
    ++k;
  } while (k != 192);
}

void CalculatePowerBombHdma_OnScreen(uint16 k, const uint8 *j) {  // 0x888C12
  do {
    uint8 w = *j;
    if (!w)
      break;
    uint8 c = power_bomb_explosion_x_pos_plus_0x100;
    uint8 right = __CFADD__uint8(c, w) ? 255 : c + w;
    uint8 left = (c >= w) ? c - w : 0;
    power_bomb_explosion_left_hdma[k] = left;
    power_bomb_explosion_right_hdma[k] = right;
    ++j;
    ++k;
  } while (k != 192);
}

void CalculatePowerBombHdma_RightOfScreen(uint16 k, const uint8 *j) {  // 0x888C3A
  do {
    uint8 w = *j;
    if ((uint8)power_bomb_explosion_x_pos_plus_0x100 < w) {
      power_bomb_explosion_left_hdma[k] = power_bomb_explosion_x_pos_plus_0x100 - w;
      power_bomb_explosion_right_hdma[k] = -1;
    } else {
      power_bomb_explosion_left_hdma[k] = -1;
      power_bomb_explosion_right_hdma[k] = -2;
    }
    ++j;
    ++k;
  } while (k != 192);
}

void CalculatePowerBombHdmaObjectTablePtrs(uint16 k) {  // 0x888C62
  uint16 v1;
  if ((uint16)(power_bomb_explosion_x_pos - layer1_x_pos + 256) >= 0x300
      || (power_bomb_explosion_x_pos_plus_0x100 = power_bomb_explosion_x_pos - layer1_x_pos + 256,
          v1 = power_bomb_explosion_y_pos - layer1_y_pos + 256,
          v1 >= 0x300)) {
    v1 = 0;
  }
  power_bomb_explosion_y_pos_rsub_0x1ff = (v1 ^ 0x3FF) - 256;
  if ((power_bomb_explosion_radius & 0xFF00) == 0)
    power_bomb_explosion_y_pos_rsub_0x1ff = 0;
  hdma_object_table_pointers[(k >> 1) + 0] = 3 * power_bomb_explosion_y_pos_rsub_0x1ff + addr_kIndirectHdmaTable_PowerBombExplodeLeft;
  hdma_object_table_pointers[(k >> 1) + 1] = 3 * power_bomb_explosion_y_pos_rsub_0x1ff + addr_kIndirectHdmaTable_PowerBombExplodeRight;
}


static uint16 k_out;

uint16 CalculatePowerBombHdmaScaled_LeftOfScreen(uint16 k, uint16 j, uint8 multval) {  // 0x888CC6
  int8 v2;
  uint8 v6, v7;
  do {
    uint8 r20 = Mult8x8(multval, g_byte_88A206[(uint8)j + 32]) >> 8;
    v2 = power_bomb_explosion_x_pos_plus_0x100;
    uint8 Reg = Mult8x8(multval, g_byte_88A206[(uint8)j]) >> 8;
    bool v4 = __CFADD__uint8(Reg, v2);
    uint8 v5 = Reg + v2;
    if (v4) {
      v6 = v5;
      v7 = 0;
    } else {
      v6 = 0;
      v7 = -1;
    }
    while (1) {
      power_bomb_explosion_left_hdma[(uint8)k] = v7;
      power_bomb_explosion_right_hdma[(uint8)k] = v6;
      if ((uint8)k == r20)
        break;
      LOBYTE(k) = k - 1;
    }
    LOBYTE(j) = j + 1;
  } while ((j & 0x80) == 0);
  k_out = LOBYTE(k);
  return v6 << 8 | v7;
}

uint16 CalculatePowerBombHdmaScaled_OnScreen(uint16 k, uint16 j, uint8 multval) {  // 0x888D04
  int8 v2;
  int8 v5;
  uint8 v6, v9;
  do {
    uint8 r20 = Mult8x8(multval, g_byte_88A206[(uint8)j + 32]) >> 8;
    v2 = power_bomb_explosion_x_pos_plus_0x100;
    uint8 Reg = Mult8x8(multval, g_byte_88A206[(uint8)j]) >> 8;
    bool v4 = __CFADD__uint8(Reg, v2);
    v5 = Reg + v2;
    if (v4)
      v5 = -1;
    v6 = v5;
    uint8 v7 = power_bomb_explosion_x_pos_plus_0x100;
    uint8 v8 = Reg;
    v4 = v7 < v8;
    v9 = v7 - v8;
    if (v4)
      v9 = 0;
    while (1) {
      power_bomb_explosion_left_hdma[(uint8)k] = v9;
      power_bomb_explosion_right_hdma[(uint8)k] = v6;
      if ((uint8)k == r20)
        break;
      LOBYTE(k) = k - 1;
    }
    LOBYTE(j) = j + 1;
  } while ((j & 0x80) == 0);
  k_out = LOBYTE(k);
  return v6 << 8 | v9;
}

uint16 CalculatePowerBombHdmaScaled_RightOfScreen(uint16 k, uint16 j, uint8 multval) {  // 0x888D46
  int8 v5;
  int8 v6; // ah
  int8 v7;
  int8 v10; // t2
  uint8 v8, v9;
  do {
    uint8 r20 = Mult8x8(multval, g_byte_88A206[(uint8)j + 32]) >> 8;
    uint8 v2 = power_bomb_explosion_x_pos_plus_0x100;
    uint8 Reg = Mult8x8(multval, g_byte_88A206[(uint8)j]) >> 8;
    bool v4 = v2 < Reg;
    v5 = v2 - Reg;
    if (v4) {
      v6 = v5;
      v7 = -1;
    } else {
      v6 = -1;
      v7 = 0;
    }
    v10 = v7;
    v8 = v6;
    v9 = v10;
    while (1) {
      power_bomb_explosion_left_hdma[(uint8)k] = v8;
      power_bomb_explosion_right_hdma[(uint8)k] = v9;
      if ((uint8)k == r20)
        break;
      LOBYTE(k) = k - 1;
    }
    LOBYTE(j) = j + 1;
  } while ((j & 0x80) == 0);
  k_out = LOBYTE(k);
  return v9 << 8 | v8;
}

void HdmaobjPreInstr_PowerBombExplode_ExplosionYellow(uint16 k) {  // 0x888DE9
  uint16 v2;

  if ((power_bomb_explosion_status & 0x8000) == 0)
    return;
  CalculatePowerBombHdmaObjectTablePtrs(k);
  int kk = Mult8x8(GET_HIBYTE(power_bomb_explosion_radius), g_byte_88A286[0]) >> 8;

  if (GET_HIBYTE(power_bomb_explosion_x_pos_plus_0x100)) {
    if (GET_HIBYTE(power_bomb_explosion_x_pos_plus_0x100) == 1)
      v2 = CalculatePowerBombHdmaScaled_OnScreen(kk, 96, GET_HIBYTE(power_bomb_explosion_radius));
    else
      v2 = CalculatePowerBombHdmaScaled_RightOfScreen(kk, 96, GET_HIBYTE(power_bomb_explosion_radius));
  } else {
    v2 = CalculatePowerBombHdmaScaled_LeftOfScreen(kk, 96, GET_HIBYTE(power_bomb_explosion_radius));
  }
  int i = k_out;
  do {
    power_bomb_explosion_left_hdma[i] = v2;
    power_bomb_explosion_right_hdma[i] = GET_HIBYTE(v2);
  } while ((--i & 0x80) == 0);
  for (int v3 = kk + 1; v3 != 0xc0; v3++) {
    power_bomb_explosion_left_hdma[v3] = 255;
    power_bomb_explosion_right_hdma[v3] = 0;
  }
  int t = 3 * (GET_HIBYTE(power_bomb_explosion_radius) >> 3);
  reg_COLDATA[0] = kPowerBombExplosionColors[t + 0] | 0x20;
  reg_COLDATA[1] = kPowerBombExplosionColors[t + 1] | 0x40;
  reg_COLDATA[2] = kPowerBombExplosionColors[t + 2] | 0x80;
  power_bomb_explosion_radius += power_bomb_pre_explosion_radius_speed;
  if (power_bomb_explosion_radius < 0x8600) {
    power_bomb_pre_explosion_radius_speed += kPowerBombExplosionRadiusAccel;
  } else {
    int v7 = k >> 1;
    hdma_object_instruction_timers[v7] = 1;
    hdma_object_instruction_list_pointers[v7] += 2;
    hdma_object_timers[v7] = 0;
  }
}

void HdmaobjPreInstr_PowerBombExplode_ExplosionWhite(uint16 k) {  // 0x888EB2
  if ((power_bomb_explosion_status & 0x8000) == 0)
    return;

  CalculatePowerBombHdmaObjectTablePtrs(k);
  const uint8 *v1 = RomPtr_88(pre_scaled_power_bomb_explosion_shape_def_ptr);
  if (GET_HIBYTE(power_bomb_explosion_x_pos_plus_0x100)) {
    if (GET_HIBYTE(power_bomb_explosion_x_pos_plus_0x100) == 1)
      CalculatePowerBombHdma_OnScreen(0, v1);
    else
      CalculatePowerBombHdma_RightOfScreen(0, v1);
  } else {
    CalculatePowerBombHdma_LeftOfScreen(0, v1);
  }
  int t = 3 * (GET_HIBYTE(power_bomb_explosion_radius) >> 3);
  reg_COLDATA[0] = kPowerBombExplosionColors[t] | 0x20;
  reg_COLDATA[1] = kPowerBombExplosionColors[t + 1] | 0x40;
  reg_COLDATA[2] = kPowerBombExplosionColors[t + 2] | 0x80;
  pre_scaled_power_bomb_explosion_shape_def_ptr += 192;
  if (pre_scaled_power_bomb_explosion_shape_def_ptr == addr_byte_889F06) {
    int v3 = k >> 1;
    hdma_object_instruction_timers[v3] = 1;
    hdma_object_instruction_list_pointers[v3] += 2;
    hdma_object_timers[v3] = 0;
    hdma_object_D[v3] = 32;
  }
  if (!__CFADD__uint16(power_bomb_pre_explosion_radius_speed, power_bomb_explosion_radius)) {
    power_bomb_explosion_radius += power_bomb_pre_explosion_radius_speed;
    power_bomb_pre_explosion_radius_speed += kPowerBombExplosionRadiusAccel;
  }
}

void CalculatePowerBombHdmaTablePointers(uint16 v0) {  // 0x888F56
  uint16 v1;
  if ((power_bomb_explosion_status & 0x8000) != 0) {
    if ((uint16)(power_bomb_explosion_x_pos - layer1_x_pos + 256) >= 0x300
        || (power_bomb_explosion_x_pos_plus_0x100 = power_bomb_explosion_x_pos - layer1_x_pos + 256,
            v1 = power_bomb_explosion_y_pos - layer1_y_pos + 256,
            v1 >= 0x300)) {
      v1 = 0;
    }
    power_bomb_explosion_y_pos_rsub_0x1ff = (v1 ^ 0x3FF) - 256;
    if ((power_bomb_pre_explosion_flash_radius & 0xFF00) == 0)
      power_bomb_explosion_y_pos_rsub_0x1ff = 0;
    int v2 = v0 >> 1;
    hdma_object_table_pointers[v2] = 3 * power_bomb_explosion_y_pos_rsub_0x1ff + addr_kIndirectHdmaTable_PowerBombExplodeLeft;
    hdma_object_table_pointers[v2 + 1] = 3 * power_bomb_explosion_y_pos_rsub_0x1ff + addr_kIndirectHdmaTable_PowerBombExplodeRight;
  }
}


void HdmaobjPreInstr_PowerBombExplode_PreExplosionWhite(uint16 k) {  // 0x8890DF
  if ((power_bomb_explosion_status & 0x8000) == 0)
    return;
  CalculatePowerBombHdmaTablePointers(k);
  uint16 v1 = 96, v2;
  int kk = Mult8x8(HIBYTE(power_bomb_pre_explosion_flash_radius), g_byte_88A286[0]) >> 8;
  if (HIBYTE(power_bomb_explosion_x_pos_plus_0x100)) {
    if (HIBYTE(power_bomb_explosion_x_pos_plus_0x100) == 1)
      v2 = CalculatePowerBombHdmaScaled_OnScreen(kk, v1, HIBYTE(power_bomb_pre_explosion_flash_radius));
    else
      v2 = CalculatePowerBombHdmaScaled_RightOfScreen(kk, v1, HIBYTE(power_bomb_pre_explosion_flash_radius));
  } else {
    v2 = CalculatePowerBombHdmaScaled_LeftOfScreen(kk, v1, HIBYTE(power_bomb_pre_explosion_flash_radius));
  }
  uint8 i = k_out;
  do {
    power_bomb_explosion_left_hdma[i] = v2;
    power_bomb_explosion_right_hdma[i] = HIBYTE(v2);
    i--;
  } while ((i & 0x80) == 0);
  for(uint8 v3 = kk + 1; v3 != 0xc0; v3++) {
    power_bomb_explosion_left_hdma[v3] = 255;
    power_bomb_explosion_right_hdma[v3] = 0;
  }
  int t = (HIBYTE(power_bomb_pre_explosion_flash_radius) >> 3) & 0xF;
  reg_COLDATA[0] = g_byte_889079[t * 3 + 0] | 0x20;
  reg_COLDATA[1] = g_byte_889079[t * 3 + 1] | 0x40;
  reg_COLDATA[2] = g_byte_889079[t * 3 + 2] | 0x80;
  power_bomb_pre_explosion_flash_radius += power_bomb_pre_explosion_radius_speed;
  if (power_bomb_pre_explosion_flash_radius < 0x9200) {
    power_bomb_pre_explosion_radius_speed -= kPowerBombPreExplosionRadiusAccel;
  } else {
    hdma_object_instruction_timers[k >> 1] = 1;
    hdma_object_instruction_list_pointers[k >> 1] += 2;
    hdma_object_timers[k >> 1] = 0;
  }
}

void HdmaobjPreInstr_PowerBombExplode_PreExplosionYellow(uint16 k) {  // 0x8891A8
  if ((power_bomb_explosion_status & 0x8000) == 0)
    return;
  CalculatePowerBombHdmaTablePointers(k);
  const uint8 *v1 = RomPtr_88(pre_scaled_power_bomb_explosion_shape_def_ptr);
  if (GET_HIBYTE(power_bomb_explosion_x_pos_plus_0x100)) {
    if (GET_HIBYTE(power_bomb_explosion_x_pos_plus_0x100) == 1)
      CalculatePowerBombHdma_OnScreen(0, v1);
    else
      CalculatePowerBombHdma_RightOfScreen(0, v1);
  } else {
    CalculatePowerBombHdma_LeftOfScreen(0, v1);
  }
  int t = (GET_HIBYTE(power_bomb_pre_explosion_flash_radius) >> 3) & 0xF;
  reg_COLDATA[0] = g_byte_889079[3 * t] | 0x20;
  reg_COLDATA[1] = g_byte_889079[3 * t + 1] | 0x40;
  reg_COLDATA[2] = g_byte_889079[3 * t + 2] | 0x80;
  pre_scaled_power_bomb_explosion_shape_def_ptr += 192;
  if (pre_scaled_power_bomb_explosion_shape_def_ptr == 0xA206) {
    hdma_object_instruction_timers[k >> 1] = 1;
    hdma_object_instruction_list_pointers[k >> 1] += 2;
    hdma_object_timers[k >> 1] = 0;
  }
  if (!__CFADD__uint16(power_bomb_pre_explosion_radius_speed, power_bomb_pre_explosion_flash_radius)) {
    power_bomb_pre_explosion_flash_radius += power_bomb_pre_explosion_radius_speed;
    power_bomb_pre_explosion_radius_speed -= kPowerBombPreExplosionRadiusAccel;
  }
}

void SpawnCrystalFlashHdmaObjs(void) {  // 0x88A2A6
  power_bomb_explosion_status = FUNC16(LayerBlendingHandler);
  static const SpawnHdmaObject_Args unk_88A2B0 = { 0x40, 0x28, 0xa2bd };
  static const SpawnHdmaObject_Args unk_88A2B8 = { 0x40, 0x29, 0xa32a };
  SpawnHdmaObject(0x88, &unk_88A2B0);
  SpawnHdmaObject(0x88, &unk_88A2B8);
}

void CrystalFlashSetupPart1(void) {  // 0x88A2E4
  offscreen_power_bomb_left_hdma = -1;
  offscreen_power_bomb_right_hdma = 0;
  power_bomb_pre_explosion_flash_radius = 1024;
  power_bomb_pre_explosion_radius_speed = kPowerBombPreExplosionRadiusSpeed;
  QueueSfx1_Max6(1);
}

void CrystalFlashSetupPart2(void) {  // 0x88A309
  power_bomb_explosion_radius = 1024;
  power_bomb_pre_explosion_radius_speed = kPowerBombExplosionRadiusSpeed;
}

void CrystalFlashCleanup(uint16 k) {  // 0x88A317
  uint16 v0 = k;

  power_bomb_flag = 0;
  power_bomb_explosion_status = 0;
  int v1 = v0 >> 1;
  hdma_object_channels_bitmask[v1] = 0;
  hdma_object_channels_bitmask[v1 + 1] = 0;
  power_bomb_pre_explosion_flash_radius = 0;
  power_bomb_explosion_radius = 0;
}

void HdmaobjPreInstr_CrystalFlash_CustomLayerBlend(uint16 k) {  // 0x88A339
  reg_W12SEL = 0;
  reg_W34SEL = 8;
  reg_WOBJSEL = 0x80;
  next_gameplay_CGWSEL = 2;
  next_gameplay_CGADSUB = 51;
  reg_TMW = 0;
  reg_TSW = 4;
}

void HdmaobjPreInstr_CrystalFlash_Stage2_AfterGlow(uint16 k) {  // 0x88A35D
  if ((power_bomb_explosion_status & 0x8000) != 0) {
    int v1 = k >> 1;
    if ((--hdma_object_timers[v1] & 0x8000) != 0) {
      if (((reg_COLDATA[2] | reg_COLDATA[1] | reg_COLDATA[0]) & 0x1F) != 0) {
        if ((reg_COLDATA[0] & 0x1F) != 0)
          reg_COLDATA[0] = ((reg_COLDATA[0] & 0x1F) - 1) | 0x20;
        if ((reg_COLDATA[1] & 0x1F) != 0)
          reg_COLDATA[1] = ((reg_COLDATA[1] & 0x1F) - 1) | 0x40;
        if ((reg_COLDATA[2] & 0x1F) != 0)
          reg_COLDATA[2] = ((reg_COLDATA[2] & 0x1F) - 1) | 0x80;
        *((uint8 *)hdma_object_timers + k) = g_word_888B96;
      } else {
        hdma_object_instruction_timers[v1] = 1;
        hdma_object_instruction_list_pointers[v1] += 2;
      }
    }
  }
}


void CalculateCrystalFlashHdmaObjectTablePtrs(uint16 k) {  // 0x88A42F
  uint16 v1;
  if ((uint16)(power_bomb_explosion_x_pos - layer1_x_pos + 256) >= 0x300
      || (power_bomb_explosion_x_pos_plus_0x100 = power_bomb_explosion_x_pos - layer1_x_pos + 256,
          v1 = power_bomb_explosion_y_pos - layer1_y_pos + 256,
          v1 >= 0x300)) {
    v1 = 0;
  }
  power_bomb_explosion_y_pos_rsub_0x1ff = (v1 ^ 0x3FF) - 256;
  if ((power_bomb_explosion_radius & 0xFF00) == 0)
    power_bomb_explosion_y_pos_rsub_0x1ff = 0;
  hdma_object_table_pointers[(k >> 1)] = 3 * power_bomb_explosion_y_pos_rsub_0x1ff + addr_kIndirectHdmaTable_PowerBombExplodeLeft;
  hdma_object_table_pointers[(k >> 1) + 1] = 3 * power_bomb_explosion_y_pos_rsub_0x1ff + addr_kIndirectHdmaTable_PowerBombExplodeRight;
}

uint16 CalculateCrystalFlashHdmaDataTablesScaled_LeftOfScreen(uint16 k, uint16 j) {  // 0x88A493
  uint8 right, left;
  do {
    uint8 r20 = Mult8x8(GET_HIBYTE(power_bomb_explosion_radius), g_byte_88A206[(uint8)j + 32]) >> 8;
    uint8 w = Mult8x8(GET_HIBYTE(power_bomb_explosion_radius), g_byte_88A206[(uint8)j]) >> 8;
    uint8 c = power_bomb_explosion_x_pos_plus_0x100;
    left = (w + c >= 256) ? 0 : 255;
    right = (w + c >= 256) ? w + c : 0;
    while (1) {
      power_bomb_explosion_left_hdma[(uint8)k] = left;
      power_bomb_explosion_right_hdma[(uint8)k] = right;
      if ((uint8)k == r20)
        break;
      k--;
    }
    j++;  // note ++
  } while ((j & 0x80) == 0);
  k_out = LOBYTE(k);
  return right << 8 | left;
}

uint16 CalculateCrystalFlashHdmaDataTablesScaled_OnScreen(uint16 k, uint16 j) {  // 0x88A4D1
  uint8 right, left;
  do {
    uint8 r20 = Mult8x8(GET_HIBYTE(power_bomb_explosion_radius), g_byte_88A206[(uint8)j + 32]) >> 8;
    uint8 w = Mult8x8(GET_HIBYTE(power_bomb_explosion_radius), g_byte_88A206[(uint8)j]) >> 8;
    uint8 c = power_bomb_explosion_x_pos_plus_0x100;
    right = c + w < 256 ? c + w : 255;
    left = c - w >= 0 ? c - w : 0;
    while (1) {
      power_bomb_explosion_left_hdma[(uint8)k] = left;
      power_bomb_explosion_right_hdma[(uint8)k] = right;
      if ((uint8)k == r20)
        break;
      k--;
    }
    j++;  // note ++
  } while ((j & 0x80) == 0);
  k_out = LOBYTE(k);
  return right << 8 | left;
}

uint16 CalculateCrystalFlashHdmaDataTablesScaled_RightOfScreen(uint16 k, uint16 j) {  // 0x88A513
  uint8 left, right;
  do {
    uint8 r20 = Mult8x8(GET_HIBYTE(power_bomb_explosion_radius), g_byte_88A206[(uint8)j + 32]) >> 8;
    uint8 w = Mult8x8(GET_HIBYTE(power_bomb_explosion_radius), g_byte_88A206[(uint8)j]) >> 8;
    uint8 c = power_bomb_explosion_x_pos_plus_0x100;
    right = (c < w) ? 255 : 0;
    left = (c < w) ? c - w : 255;
    while (1) {
      power_bomb_explosion_left_hdma[(uint8)k] = left;
      power_bomb_explosion_right_hdma[(uint8)k] = right;
      if ((uint8)k == r20)
        break;
      k--;
    }
    j++;  // note ++
  } while ((j & 0x80) == 0);
  k_out = LOBYTE(k);
  return right << 8 | left;
}

void HdmaobjPreInstr_CrystalFlash_Stage1_Explosion(uint16 k) {  // 0x88A552
  if ((power_bomb_explosion_status & 0x8000) == 0)
    return;
  CalculateCrystalFlashHdmaObjectTablePtrs(k);
  
  uint16 v2;
  int kk = Mult8x8(GET_HIBYTE(power_bomb_explosion_radius), g_byte_88A286[0]) >> 8;
  if (GET_HIBYTE(power_bomb_explosion_x_pos_plus_0x100)) {
    if (GET_HIBYTE(power_bomb_explosion_x_pos_plus_0x100) == 1)
      v2 = CalculateCrystalFlashHdmaDataTablesScaled_OnScreen(kk, 96);
    else
      v2 = CalculateCrystalFlashHdmaDataTablesScaled_RightOfScreen(kk, 96);
  } else {
    v2 = CalculateCrystalFlashHdmaDataTablesScaled_LeftOfScreen(kk, 96);
  }
  int i = k_out;
  do {
    power_bomb_explosion_left_hdma[(uint8)i] = v2;
    power_bomb_explosion_right_hdma[(uint8)i] = GET_HIBYTE(v2);
    i--;
  } while ((i & 0x80) == 0);
  for (uint8 v3 = kk + 1; v3 != 0xc0; v3++) {
    power_bomb_explosion_left_hdma[v3] = 255;
    power_bomb_explosion_right_hdma[v3] = 0;
  }
  int pos = (HIBYTE(power_bomb_explosion_radius) >> 3) * 3;
  reg_COLDATA[0] = kPowerBombExplosionColors[pos + 0] | 0x20;
  reg_COLDATA[1] = kPowerBombExplosionColors[pos + 1] | 0x40;
  reg_COLDATA[2] = kPowerBombExplosionColors[pos + 2] | 0x80;
  power_bomb_explosion_radius += power_bomb_pre_explosion_radius_speed;
  if (power_bomb_explosion_radius < 0x2000) {
    power_bomb_pre_explosion_radius_speed += kPowerBombExplosionRadiusAccel;
  } else {
    hdma_object_instruction_timers[k >> 1] = 1;
    hdma_object_instruction_list_pointers[k >> 1] += 2;
    hdma_object_timers[k >> 1] = 0;
  }
}

void FxTypeFunc_22_ScrollingSky(void) {  // 0x88A61B
  fx_y_pos = 1248;
  fx_type = 6;

  static const SpawnHdmaObject_Args unk_88A62E = { 0x02, 0x12, 0xad63 };
  static const SpawnHdmaObject_Args unk_88A636 = { 0x42, 0x11, 0xad4e };
  static const SpawnHdmaObject_Args unk_88A63E = { 0x42, 0x0d, 0xad39 };
  SpawnHdmaObject(0x88, &unk_88A62E);
  SpawnHdmaObject(0x88, &unk_88A636);
  SpawnHdmaObject(0x88, &unk_88A63E);
  HdmaobjPreInstr_FxType22_BG3Yscroll(0);
}

static uint16 SetupSomeHdmaTablesBG3(uint16 r24, uint16 r0, uint16 r3, uint16 r6, uint16 r9) {  // 0x88A81C
  int16 v1;

  g_word_7E0598 = 32;
  *(uint16 *)mother_brain_indirect_hdma = 31;
  *(uint16 *)&mother_brain_indirect_hdma[1] = 0;
  uint16 v0 = 3;
  v1 = layer1_y_pos + 32;
  uint16 r18 = layer1_y_pos + 32;
  uint16 v2 = 0;
  while ((int16)(v1 - *(uint16 *)&RomPtr_88(r0)[v2]) < 0
    || (int16)(v1 - *(uint16 *)&RomPtr_88(r9)[v2]) >= 0) {
    v2 += 6;
    if ((int16)(v2 - r24) >= 0)
      return v0;
  }
  uint16 v3;
  if (sign16(r18 - 1248))
    v3 = r18 & 0xF;
  else
    v3 = r18 & 0x1F;
  uint16 R22_ = v3;
  *(uint16 *)&mother_brain_indirect_hdma[3] = *(uint16 *)&RomPtr_88(r3)[v2] - v3;
  *(uint16 *)&mother_brain_indirect_hdma[4] = R22_ + *(uint16 *)&RomPtr_88(r6)[v2] - g_word_7E0598;
  while (1) {
    uint16 r20 = mother_brain_indirect_hdma[v0];
    r18 += r20;
    v0 += 3;
    g_word_7E0598 += r20;
    if (!sign16(g_word_7E0598 - 224))
      break;
    while ((int16)(r18 - *(uint16 *)&RomPtr_88(r0)[v2]) < 0
      || (int16)(r18 - *(uint16 *)&RomPtr_88(r9)[v2]) >= 0) {
      v2 += 6;
      if ((int16)(v2 - r24) >= 0)
        return v0;
    }
    *(uint16 *)&mother_brain_indirect_hdma[v0] = *(uint16 *)&RomPtr_88(r3)[v2];
    *(uint16 *)&mother_brain_indirect_hdma[v0 + 1] = *(uint16 *)&RomPtr_88(r6)[v2] - g_word_7E0598;
  }
  return v0;
}

void HdmaobjPreInstr_FxType22_BG3Yscroll(uint16 k) {  // 0x88A643
  DamageSamusInTopRow();
  k = SetupSomeHdmaTablesBG3(78, addr_word_88A8E8, addr_word_88A8E8 + 2, addr_word_88A8E8 + 4, addr_word_88A8E8 + 6);
  *(uint16 *)&mother_brain_indirect_hdma[k] = 0;
}

const uint8 *HdmaobjInstr_SetFlagB(uint16 k, const uint8 *hdp) {  // 0x88A66C
  hdma_object_B[k >> 1] = 1;
  return hdp;
}
static const int16 g_word_88C46E[16] = {  // 0x88A673
  0, 1, 1, 0, 0, -1, -1, 0,
  0, 1, 1, 0, 0, -1, -1, 0,
};
void HdmaobjPreInstr_BG3Xscroll(uint16 k) {
  int16 v8;
  int16 v9;

  if (sign16(layer1_y_pos - 1024)) {
    if ((nmi_frame_counter_byte & 1) == 0)
      g_word_7E0596 = (g_word_7E0596 + 2) & 0x1E;
    uint16 v1 = g_word_7E0596;
    uint16 v2 = 0;
    do {
      g_word_7E9E80[v2 >> 1] = g_word_88A938[v1 >> 1];
      v2 += 2;
      v1 += 2;
    } while ((int16)(v1 - 32) < 0);
  } else {
    int v3 = k >> 1;
    if (hdma_object_B[v3]-- == 1) {
      hdma_object_B[v3] = 6;
      hdma_object_A[v3] = (hdma_object_A[v3] + 2) & 0x1F;
    }
    uint16 v5 = hdma_object_A[v3];
    for (int i = 30; i >= 0; i -= 2) {
      g_word_7E9E80[v5 >> 1] = g_word_88C46E[i >> 1] + reg_BG1HOFS;
      v5 = (v5 - 2) & 0x1F;
    }
  }
  *(uint16 *)&hdma_window_1_left_pos[0].field_0 = 0;
  *(uint16 *)scrolling_sky_bg2_indirect_hdma = 0;
  *(uint16 *)&hdma_window_1_left_pos[0].field_2 = 31;
  *(uint16 *)&scrolling_sky_bg2_indirect_hdma[2] = 31;
  uint16 r18 = 31;
  *(uint16 *)&hdma_window_1_left_pos[1].field_0 = -25088;
  *(uint16 *)&scrolling_sky_bg2_indirect_hdma[3] = -25088;
  if (sign16(layer1_y_pos - 1024)) {
    *(uint16 *)&scrolling_sky_bg2_indirect_hdma[2] = 0;
  } else {
    r18 = HdmaFunc_A786(0x105, r18, 177, 0);
  }
  uint16 v7 = 5;
  r18 = HdmaFunc_A786(5, r18, -24960, 128);
  v8 = 224 - r18;
  while ((int16)(v8 - 16) >= 0) {
    v8 -= 16;
    *(uint16 *)(&hdma_window_1_left_pos[0].field_0 + v7) = 144;
    *(uint16 *)&scrolling_sky_bg2_indirect_hdma[v7] = 144;
    *(uint16 *)(&hdma_window_1_left_pos[0].field_1 + v7) = -24960;
    *(uint16 *)&scrolling_sky_bg2_indirect_hdma[v7 + 1] = -24960;
    v7 += 3;
  }
  v9 = v8;
  if (v8)
    v9 = v8 + 128;
  *(uint16 *)(&hdma_window_1_left_pos[0].field_0 + v7) = v9;
  *(uint16 *)&scrolling_sky_bg2_indirect_hdma[v7] = v9;
  *(uint16 *)(&hdma_window_1_left_pos[0].field_1 + v7) = -24960;
  *(uint16 *)&scrolling_sky_bg2_indirect_hdma[v7 + 1] = -24960;
  *(uint16 *)(&hdma_window_1_left_pos[1].field_0 + v7) = 0;
  *(uint16 *)&scrolling_sky_bg2_indirect_hdma[v7 + 3] = 0;
}

uint16 HdmaFunc_A786(uint16 k, uint16 r18, uint16 r20, uint16 r22) {  // 0x88A786
  uint16 v0 = k;
  int16 v2;

  uint16 v1 = 1216 - layer1_y_pos;
  uint16 r24 = 1216 - layer1_y_pos;
  if ((int16)(1216 - layer1_y_pos) >= 0 && layer1_y_pos != 1216) {
    if (sign16(v1 - 128)) {
      r18 = 1216 - layer1_y_pos;
    } else {
      r18 = 1216 - layer1_y_pos;
      if (!sign16(v1 - 193))
        v1 = 193;
      r18 = v1;
      r24 = v1;
      while (1) {
        v2 = v1 - 16;
        if (v2 < 0)
          break;
        r24 = v2;
        *(uint16 *)(&hdma_window_1_left_pos[0].field_0 + v0) = r22 | 0x10;
        *(uint16 *)(&hdma_window_1_left_pos[0].field_1 + v0) = r20;
        v1 = r24;
        v0 += 3;
      }
    }
    *(uint16 *)(&hdma_window_1_left_pos[0].field_0 + v0) = r22 | r24;
    *(uint16 *)(&hdma_window_1_left_pos[0].field_1 + v0) = r20;
  }
  return r18;
}

static const SpawnHdmaObject_Args unk_88A7EF = { 0x42, 0x0f, 0xad76 };
static const SpawnHdmaObject_Args unk_88A80B = { 0x42, 0x0f, 0xad89 };

void FxTypeFunc_20(void) {  // 0x88A7D8
  layer2_scroll_x |= 1;
  layer2_scroll_y |= 1;
  SpawnHdmaObject(0x88, &unk_88A7EF);
  set_to_e0_by_scrolling_sky = 224;
  UNUSED_word_7E059C = 0;
}

void RoomSetupAsm_ScrollingSkyOcean(void) {  // 0x88A800
  reg_BG2SC = 74;
  SpawnHdmaObject(0x88, &unk_88A80B);
  set_to_e0_by_scrolling_sky = 224;
  UNUSED_word_7E059C = 0;
}

void DamageSamusInTopRow(void) {  // 0x88A8C4
  if ((int16)(samus_y_pos - samus_y_radius) < 0 || sign16(samus_y_pos - samus_y_radius - 17))
    samus_periodic_damage = 8;
}

void HdmaobjPreInstr_SkyLandBG2XscrollInner(uint16 k) {  // 0x88ADC2
  uint16 i;
  uint16 r24 = 0;
  reg_BG2SC = 74;
  uint16 v1 = 0;
  do {
    uint16 scroll_subspeed = kHdmaScrollEntrys[v1].scroll_subspeed;
    uint8 *v3 = &g_ram[kHdmaScrollEntrys[v1].hdma_data_table_entry];
    bool v4 = __CFADD__uint16(GET_WORD(v3), scroll_subspeed);
    *(uint16 *)v3 += scroll_subspeed;
    *((uint16 *)v3 + 1) += v4 + kHdmaScrollEntrys[v1++].scroll_speed;
  } while (sign16(v1 * 8 - 184));
  scrolling_sky_bg2_hdma_data[44] = 0;
  scrolling_sky_bg2_hdma_data[45] = 0;
  *(uint16 *)scrolling_sky_bg2_indirect_hdma = 31;
  *(uint16 *)&scrolling_sky_bg2_indirect_hdma[1] = 1438;
  uint16 r18 = layer1_y_pos + 32;
  uint16 r20 = layer1_y_pos + 224;
  uint16 v5 = 0;
  uint16 v6 = 3;
  do {
    while ((int16)(r18 - kHdmaScrollEntrys[v5].top_pos) >= 0 && (int16)(r18 - kHdmaScrollEntrys[v5 + 1].top_pos) < 0) {
      uint16 v8 = kHdmaScrollEntrys[v5 + 1].top_pos - r18;
      r24 = v8;
      if (!sign16(v8 - 128)) {
        *(uint16 *)&scrolling_sky_bg2_indirect_hdma[v6] = 127;
        *(uint16 *)&scrolling_sky_bg2_indirect_hdma[v6 + 1] = kHdmaScrollEntrys[v5].hdma_data_table_entry + 2;
        v6 += 3;
        v8 = r24 - 127;
      }
      *(uint16 *)&scrolling_sky_bg2_indirect_hdma[v6] = v8;
      *(uint16 *)&scrolling_sky_bg2_indirect_hdma[v6 + 1] = kHdmaScrollEntrys[v5].hdma_data_table_entry + 2;
      r18 += r24;
      v6 += 3;
      if (!sign16(r18 - r20)) {
        *(uint16 *)&scrolling_sky_bg2_indirect_hdma[v6 + 3] = 0;
        return;
      }
    }
    ++v5;
  } while (sign16(v5 * 8 - 184));
  for (i = 1535 - r18; ; i = r24 - 127) {
    r24 = i;
    if (sign16(i - 128))
      break;
    *(uint16 *)&scrolling_sky_bg2_indirect_hdma[v6] = 127;
    *(uint16 *)&scrolling_sky_bg2_indirect_hdma[v6 + 1] = 181;
    v6 += 3;
  }
  *(uint16 *)&scrolling_sky_bg2_indirect_hdma[v6] = i;
  *(uint16 *)&scrolling_sky_bg2_indirect_hdma[v6 + 1] = 181;
  *(uint16 *)&scrolling_sky_bg2_indirect_hdma[v6 + 3] = 0;
}


void HdmaobjPreInstr_SkyLandBG2Xscroll(uint16 k) {  // 0x88ADB2
  if (!time_is_frozen_flag)
    HdmaobjPreInstr_SkyLandBG2XscrollInner(k);
}

void HdmaobjPreInstr_SkyLandBG2Xscroll2(uint16 k) {  // 0x88ADBA
  if (!time_is_frozen_flag)
    HdmaobjPreInstr_SkyLandBG2XscrollInner(k);
}

void RoomMainAsm_ScrollingSky(LongPtr r0) {  // 0x88AFA3
  if (time_is_frozen_flag) {
    WORD(scrolling_sky_bg2_indirect_hdma[0]) = 0;
  } else {
    reg_BG2VOFS = layer1_y_pos;
    uint16 v0 = vram_write_queue_tail;
    VramWriteEntry *v1 = gVramWriteEntry(vram_write_queue_tail);
    v1[0].size = 64;
    v1[1].size = 64;
    v1[2].size = 64;
    v1[3].size = 64;
    int tt = (layer1_y_pos & 0x7F8) - 16;
    // fixed: prevent out of bounds read when tt is negative.
    if (sign16(tt))
      tt = 0;
    int v2 = 8 * (uint8)tt;
    int y1 = (uint8)(tt >> 8);
    int y2 = (uint8)((tt >> 8) + 1);
    VoidP v3 = IndirReadWord(r0, 2 * y1) + v2;
    v1[0].src.addr = v3;
    v1[1].src.addr = v3 + 64;
    VoidP v5 = IndirReadWord(r0, 2 * y2) + v2;
    v1[2].src.addr = v5;
    v1[3].src.addr = v5 + 64;
    v1[0].src.bank = 0x8a;
    v1[1].src.bank = 0x8a;
    v1[2].src.bank = 0x8a;
    v1[3].src.bank = 0x8a;
    int t = (reg_BG2SC & 0xFC) << 8;
    uint16 v6 = t + 4 * ((layer1_y_pos - 16) & 0x1F8);
    v1[0].vram_dst = v6;
    v1[1].vram_dst = v6 + 32;
    uint16 v7 = t + 4 * ((layer1_y_pos + 240) & 0x1F8);
    v1[2].vram_dst = v7;
    v1[3].vram_dst = v7 + 32;
    vram_write_queue_tail = v0 + 28;
  }
}

void RoomCode_ScrollingSkyLand(void) {  // 0x88AF8D
  RoomMainAsm_ScrollingSky((LongPtr) { addr_off_88AD9C, 0x88 });
}

void RoomMainAsm_ScrollingSkyOcean(void) {  // 0x88AF99
  RoomMainAsm_ScrollingSky((LongPtr) { addr_off_88ADA6, 0x88 });
}

static const SpawnHdmaObject_Args unk_88B08C = { 0x42, 0x11, 0xb0ac };

static const uint16 kFirefleaFlashingShades[12] = { 0, 0x100, 0x200, 0x300, 0x400, 0x500, 0x600, 0x500, 0x400, 0x300, 0x200, 0x100 };
static const uint16 kFirefleaDarknessShades[6] = { 0, 0x600, 0xc00, 0x1200, 0x1800, 0x1900 };

void FxTypeFunc_24(void) {  // 0x88B07C
  fireflea_flashing_timer = 6;
  fireflea_flashing_index = 0;
  SpawnHdmaObject(0x88, &unk_88B08C);
  *(uint16 *)&hdma_window_1_left_pos[0].field_0 = 0;
  UNUSED_word_7E177C = 0;
  fireflea_darkness_level = 0;
  UNUSED_word_7E1780 = 24;
  UNUSED_word_7E1782 = kFirefleaFlashingShades[0];
}

void HdmaobjPreInstr_FirefleaBG3XScroll(uint16 k) {  // 0x88B0BC
  int8 v2;
  uint16 v1;

  fx_layer_blending_config_c = fx_layer_blending_config_c & 0xFF00 | 0xC;
  if (!time_is_frozen_flag) {
    if (!--fireflea_flashing_timer) {
      fireflea_flashing_timer = 6;
      if (sign16(fireflea_darkness_level - 10)) {
        v1 = fireflea_flashing_index + 1;
        if ((uint16)(fireflea_flashing_index + 1) >= 0xC)
          v1 = 0;
      } else {
        v1 = 6;
      }
      fireflea_flashing_index = v1;
    }
    v2 = (uint16)(kFirefleaDarknessShades[fireflea_darkness_level >> 1]
                  + kFirefleaFlashingShades[fireflea_flashing_index]) >> 8;
    reg_COLDATA[1] = v2 | 0x80;
    reg_COLDATA[2] = v2 | 0x40;
    reg_COLDATA[0] = v2 | 0x20;
  }
}


void ExpandingContractingHdmaEffect(void) {  // 0x88B17F
  if (!sign16(++set_to_e0_by_scrolling_sky - 4)) {
    set_to_e0_by_scrolling_sky = 0;
    if (UNUSED_hdma_contracting_flag) {
      message_box_animation_y_radius -= 1024;
      if (sign16(message_box_animation_y_radius - 0x2000)) {
        message_box_animation_y_radius = 0x2000;
        UNUSED_hdma_contracting_flag = 0;
      }
    } else {
      message_box_animation_y_radius += 1024;
      if (!sign16(message_box_animation_y_radius + 0x8000))
        ++UNUSED_hdma_contracting_flag;
    }
    message_box_animation_y0 = 128;
    message_box_animation_y1 = 128;
    message_box_animation_y2 = 127;
    message_box_animation_y3 = 127;
  }
  uint16 v0 = 2 * message_box_animation_y0;
  uint16 v1 = 2 * message_box_animation_y2;
  uint16 r18 = 0, r20 = 32;
  do {
    *(uint16 *)&mother_brain_indirect_hdma[v0] = message_box_animation_y1 - message_box_animation_y0;
    *(uint16 *)&mother_brain_indirect_hdma[v1] = message_box_animation_y3 - message_box_animation_y2;
    r18 += message_box_animation_y_radius;
    if (Unreachable()) {
      ++message_box_animation_y1;
      --message_box_animation_y3;
    }
    ++message_box_animation_y0;
    --message_box_animation_y2;
    v1 -= 2;
    v0 += 2;
  } while (--r20);
}

static const uint16 g_word_88B256[17] = { 0x46, 1, 0x46, 3, 0x46, 2, 0x46, 1, 0x46, 1, 0x46, 2, 0x46, 2, 0x46, 1, 0x8000 };

void HandleEarthquakeSoundEffect(void) {  // 0x88B21D
  int16 v1;

  if ((earthquake_sfx_timer & 0x8000) == 0 && (--earthquake_sfx_timer & 0x8000) != 0) {
    uint16 v0 = earthquake_sfx_index;
    v1 = g_word_88B256[earthquake_sfx_index >> 1];
    if (v1 < 0) {
      v0 = 0;
      v1 = g_word_88B256[0];
    }
    QueueSfx2_Max6(v1);
    earthquake_sfx_timer = g_word_88B256[(v0 >> 1) + 1] + (v0 & 1) + (random_number & 3);
    earthquake_sfx_index = v0 + 4;
  }
}

static const SpawnHdmaObject_Args unk_88B289 = { 0x42, 0x12, 0xc3e1 };
static const SpawnHdmaObject_Args unk_88B291 = { 0x42, 0x10, 0xc3f0 };
static const SpawnHdmaObject_Args unk_88B2B1 = { 0x42, 0x12, 0xc3e1 };
static const SpawnHdmaObject_Args unk_88B2B9 = { 0x42, 0x10, 0xc3f0 };
void FxTypeFunc_2_Lava(void) {  // 0x88B279
  fx_rising_function_bank_88 = FUNC16(FxRisingFunction_LavaAcid);
  lava_acid_y_pos = fx_base_y_pos;
  SpawnHdmaObject(0x88, &unk_88B289);
  SpawnHdmaObject(0x88, &unk_88B291);
  SpawnBG3ScrollHdmaObject();
  SpawnAnimtiles(addr_kAnimtiles_Lava);
}

void FxTypeFunc_4_Acid(void) {  // 0x88B2A1
  fx_rising_function_bank_88 = FUNC16(FxRisingFunction_LavaAcid);
  lava_acid_y_pos = fx_base_y_pos;
  SpawnHdmaObject(0x88, &unk_88B2B1);
  SpawnHdmaObject(0x88, &unk_88B2B9);
  SpawnBG3ScrollHdmaObject();
  SpawnAnimtiles(addr_kAnimtiles_Acid);
}

void FxHandleTide(void) {  // 0x88B2C9
  uint16 v2, v5;

  if (*(int16 *)((uint8 *)&fx_y_vel + 1) < 0) {
    fx_y_suboffset = 0;
    fx_y_offset = 0;
    int v0 = HIBYTE(tide_phase);
    uint16 v1 = 8 * kSinCosTable8bit_Sext[v0];
    if ((kSinCosTable8bit_Sext[v0] & 0x1000) != 0)
      --fx_y_offset;
    *(uint16 *)((uint8 *)&fx_y_suboffset + 1) = v1;
    if ((kSinCosTable8bit_Sext[v0] & 0x8000) == 0)
      v2 = tide_phase + 288;
    else
      v2 = tide_phase + 192;
    tide_phase = v2;
  } else if ((*(uint16 *)((uint8 *)&fx_y_vel + 1) & 0x4000) != 0) {
    fx_y_suboffset = 0;
    fx_y_offset = 0;
    int v3 = HIBYTE(tide_phase);
    uint16 v4 = 32 * kSinCosTable8bit_Sext[v3];
    if ((kSinCosTable8bit_Sext[v3] & 0x400) != 0)
      --fx_y_offset;
    *(uint16 *)((uint8 *)&fx_y_suboffset + 1) = v4;
    if ((kSinCosTable8bit_Sext[v3] & 0x8000) == 0)
      v5 = tide_phase + 224;
    else
      v5 = tide_phase + 128;
    tide_phase = v5;
  }
}

void FxRisingFunction_LavaAcid(void) {  // 0x88B343
  if (fx_y_vel) {
    if ((fx_y_vel & 0x8000) == 0) {
      if (fx_target_y_pos < fx_base_y_pos || fx_target_y_pos == fx_base_y_pos)
        return;
LABEL_8:
      fx_rising_function_bank_88 = FUNC16(FxRisingFunction_LavaAcid_WaitToRise);
      return;
    }
    if (fx_target_y_pos != fx_base_y_pos && fx_target_y_pos < fx_base_y_pos)
      goto LABEL_8;
  }
}

void FxRisingFunction_LavaAcid_WaitToRise(void) {  // 0x88B367
  HandleEarthquakeSoundEffect();
  earthquake_type = 21;
  earthquake_timer |= 0x20;
  if (!--fx_timer)
    fx_rising_function_bank_88 = FUNC16(FxRisingFunction_LavaAcid_Raising);
}

void FxRisingFunction_LavaAcid_Raising(void) {  // 0x88B382
  HandleEarthquakeSoundEffect();
  earthquake_type = 21;
  earthquake_timer |= 0x20;
  if (RaiseOrLowerFx() & 1) {
    fx_y_vel = 0;
    fx_rising_function_bank_88 = FUNC16(FxRisingFunction_LavaAcid);
  }
}

const uint8 *HdmaobjInstr_B3A9(uint16 k, const uint8 *hdp) {  // 0x88B3A9
  hdma_object_C[k >> 1] = 112;
  return hdp;
}

void CallFxRisingFunc(uint32 ea) {
  switch (ea) {
  case fnFxRisingFunction_LavaAcid: FxRisingFunction_LavaAcid(); return;
  case fnFxRisingFunction_LavaAcid_WaitToRise: FxRisingFunction_LavaAcid_WaitToRise(); return;
  case fnFxRisingFunction_LavaAcid_Raising: FxRisingFunction_LavaAcid_Raising(); return;
  case fnFxRisingFunction_C428_WaterNormal: FxRisingFunction_C428_WaterNormal(); return;
  case fnFxRisingFunction_WaterWaitToRise: FxRisingFunction_WaterWaitToRise(); return;
  case fnFxRisingFunction_WaterRising: FxRisingFunction_WaterRising(); return;
  default: Unreachable();
  }
}

static const uint8 kLavaSoundEffects[8] = { 0x12, 0x13, 0x14, 0x12, 0x13, 0x14, 0x12, 0x13 };

void HdmaobjPreInstr_LavaAcidBG3YScroll(uint16 k) {  // 0x88B3B0
  unsigned int v1; // kr00_4
  int16 v2;
  int16 v3;
  int16 v4;
  int16 v8;

  fx_layer_blending_config_c = fx_layer_blending_config_b;
  if (!time_is_frozen_flag) {
    CallFxRisingFunc(fx_rising_function_bank_88 | 0x880000);
    FxHandleTide();
    v1 = __PAIR32__(fx_y_offset, fx_y_suboffset) + __PAIR32__(fx_base_y_pos, fx_base_y_subpos);
    lava_acid_y_pos = HIWORD(v1);
    lava_acid_y_subpos = v1;
    bg3_xpos = reg_BG1HOFS;
    *(uint16 *)mother_brain_indirect_hdma = 0;
    bg3_ypos = 0;
    v2 = (__PAIR32__(fx_y_offset, fx_y_suboffset) + __PAIR32__(fx_base_y_pos, fx_base_y_subpos)) >> 16;
    if (v2 >= 0) {
      v3 = v2 - layer1_y_pos;
      if (v3 <= 0) {
        v4 = (v3 ^ 0x1F) & 0x1F | 0x100;
LABEL_8:
        *(uint16 *)&mother_brain_indirect_hdma[2] = v4;
        if (fx_type == 2 && (lava_acid_y_pos & 0x8000) == 0) {
          int v5 = (uint8)hdma_object_index >> 1;
          if (hdma_object_C[v5]-- == 1) {
            hdma_object_C[v5] = 112;
            QueueSfx2_Max6(kLavaSoundEffects[random_number & 7]);
          }
        }
        random_number = swap16(random_number);
        if ((lava_acid_y_pos & 0x8000) == 0) {
          v8 = lava_acid_y_pos - layer1_y_pos + 256;
          if (v8 < 0) {
            v8 = 255;
LABEL_17:
            hdma_object_table_pointers[(uint8)hdma_object_index >> 1] = 3 * (((v8 ^ 0x1FF) + 1) & 0x3FF) + 0xB62A;
            return;
          }
          if ((uint16)v8 < 0x200)
            goto LABEL_17;
        }
        v8 = 511;
        goto LABEL_17;
      }
      if ((uint16)v3 < 0x100) {
        v4 = (uint8)~v3;
        goto LABEL_8;
      }
    }
    v4 = 0;
    goto LABEL_8;
  }
}

const uint8 *HdmaobjInstr_SetFlagB_Copy(uint16 k, const uint8 *hdp) {  // 0x88B4CE
  hdma_object_B[k >> 1] = 1;
  return hdp;
}

void HdmaobjPreInstr_LavaAcidBG2YScroll(uint16 k) {  // 0x88B4D5
  g_word_7E9C44 = reg_BG2VOFS;
  if (!time_is_frozen_flag && (fx_liquid_options & 6) != 0) {
    if ((fx_liquid_options & 2) != 0)
      Handle_LavaAcidBG2YScroll_Func3(k);
    else
      Handle_LavaAcidBG2YScroll_Func2(k);
  } else {
    Handle_LavaAcidBG2YScroll_Func1(k);
  }
  hdma_object_table_pointers[(uint8)hdma_object_index >> 1] = 3 * (reg_BG2VOFS & 0xF) + 0xC0B1;
}

void Handle_LavaAcidBG2YScroll_Func1(uint16 v0) {  // 0x88B51D
  WriteReg((SnesRegs)(*((uint8 *)hdma_object_bank_slot + v0) + 17153), 0x10);
  uint8 v1 = 30;
  uint16 v2 = reg_BG2VOFS & 0x1FF;
  do {
    g_word_7E9C46[v1 >> 1] = v2;
    v1 -= 2;
  } while ((v1 & 0x80) == 0);
}
void Handle_LavaAcidBG2YScroll_Func2(uint16 v0) {  // 0x88B53B
  WriteReg((SnesRegs)(*((uint8 *)hdma_object_bank_slot + v0) + 17153), 0xF);
  int v1 = v0 >> 1;
  if (hdma_object_B[v1]-- == 1) {
    hdma_object_B[v1] = 6;
    hdma_object_A[v1] = (hdma_object_A[v1] - 2) & 0x1E;
  }
  uint8 v3 = *((uint8 *)hdma_object_A + v0);
  uint8 v4 = 30;
  int n = 15;
  do {
    g_word_7E9C46[v4 >> 1] = (g_word_88B589[v3 >> 1] + reg_BG2HOFS) & 0x1FF;
    v3 = (v3 - 2) & 0x1E;
    v4 = (v4 - 2) & 0x1E;
  } while (--n >= 0);
}
void Handle_LavaAcidBG2YScroll_Func3(uint16 v0) {  // 0x88B5A9
  WriteReg((SnesRegs)(*((uint8 *)hdma_object_bank_slot + v0) + BBAD0), 0x10);
  int v1 = v0 >> 1;
  if (hdma_object_B[v1]-- == 1) {
    hdma_object_B[v1] = 4;
    hdma_object_A[v1] = (hdma_object_A[v1] - 2) & 0x1E;
  }
  uint8 v3 = (LOBYTE(hdma_object_A[v1]) + 2 * (reg_BG2VOFS & 0xF)) & 0x1E;
  uint8 v4 = (2 * (reg_BG2VOFS & 0xF) + 30) & 0x1E;
  int n = 15;
  do {
    g_word_7E9C46[v4 >> 1] = (g_word_88B60A[v3 >> 1] + reg_BG2VOFS) & 0x1FF;
    v4 = (v4 - 2) & 0x1E;
    v3 = (v3 - 2) & 0x1E;
  } while (--n >= 0);
}
static const SpawnHdmaObject_Args unk_88C40F = { 0x42, 0x11, 0xd856 };
static const SpawnHdmaObject_Args unk_88C41F = { 0x42, 0x0f, 0xd847 };
void FxTypeFunc_6_Water(void) {  // 0x88C3FF
  fx_rising_function_bank_88 = FUNC16(FxRisingFunction_C428_WaterNormal);
  fx_y_pos = fx_base_y_pos;
  SpawnHdmaObject(0x88, &unk_88C40F);
  if ((fx_liquid_options & 2) != 0)
    SpawnHdmaObject(0x88, &unk_88C41F);
  SpawnBG3ScrollHdmaObject();
}

void FxRisingFunction_C428_WaterNormal(void) {  // 0x88C428
  if (fx_y_vel) {
    if ((fx_y_vel & 0x8000) == 0) {
      if (fx_target_y_pos < fx_base_y_pos || fx_target_y_pos == fx_base_y_pos)
        return;
LABEL_8:
      fx_rising_function_bank_88 = FUNC16(FxRisingFunction_WaterWaitToRise);
      return;
    }
    if (fx_target_y_pos != fx_base_y_pos && fx_target_y_pos < fx_base_y_pos)
      goto LABEL_8;
  }
}

void FxRisingFunction_WaterWaitToRise(void) {  // 0x88C44C
  if (!--fx_timer)
    fx_rising_function_bank_88 = FUNC16(FxRisingFunction_WaterRising);
}

void FxRisingFunction_WaterRising(void) {  // 0x88C458
  if (RaiseOrLowerFx() & 1) {
    fx_rising_function_bank_88 = FUNC16(FxRisingFunction_C428_WaterNormal);
    fx_y_vel = 0;
  }
}

const uint8 *HdmaobjInstr_SetFlagB_Copy2(uint16 k, const uint8 *hdp) {  // 0x88C467
  hdma_object_B[k >> 1] = 1;
  return hdp;
}

void CallFxRisingFunction(uint32 ea) {
  switch (ea) {
  case fnFxRisingFunction_LavaAcid: FxRisingFunction_LavaAcid(); return;
  case fnFxRisingFunction_LavaAcid_WaitToRise: FxRisingFunction_LavaAcid_WaitToRise(); return;
  case fnFxRisingFunction_LavaAcid_Raising: FxRisingFunction_LavaAcid_Raising(); return;
  case fnFxRisingFunction_C428_WaterNormal: FxRisingFunction_C428_WaterNormal(); return;
  case fnFxRisingFunction_WaterWaitToRise: FxRisingFunction_WaterWaitToRise(); return;
  case fnFxRisingFunction_WaterRising: FxRisingFunction_WaterRising(); return;
  default: Unreachable();
  }
}

void HdmaobjPreInstr_WaterBG3XScroll(uint16 k) {  // 0x88C48E
  unsigned int v1; // kr00_4
  int16 v2;
  int16 v3;
  int16 v9;
  uint16 v4;

  fx_layer_blending_config_c = fx_layer_blending_config_b;
  if (!time_is_frozen_flag) {
    CallFxRisingFunction(fx_rising_function_bank_88 | 0x880000);
    FxHandleTide();
    v1 = __PAIR32__(fx_y_offset, fx_y_suboffset) + __PAIR32__(fx_base_y_pos, fx_base_y_subpos);
    fx_y_pos = HIWORD(v1);
    fx_y_subpos = v1;
    v2 = (__PAIR32__(fx_y_offset, fx_y_suboffset) + __PAIR32__(fx_base_y_pos, fx_base_y_subpos)) >> 16;
    if (v2 < 0)
      goto LABEL_6;
    v3 = v2 - layer1_y_pos;
    if (v3 <= 0) {
      v4 = (v3 ^ 0x1F) & 0x1F | 0x100;
      goto LABEL_8;
    }
    if ((uint16)v3 < 0x100)
      v4 = (uint8)~(uint8)v3;
    else
      LABEL_6:
    v4 = 0;
LABEL_8:
    bg3_ypos = v4;
    int v5 = (uint8)hdma_object_index >> 1;
    uint16 r20 = layer1_x_pos + (int8)HIBYTE(hdma_object_C[v5]);
    if (hdma_object_B[v5]-- == 1) {
      hdma_object_B[v5] = 10;
      hdma_object_A[v5] = (hdma_object_A[v5] + 2) & 0x1E;
    }
    uint8 v7 = hdma_object_A[v5];
    for (int i = 30; (i & 0x80) == 0; i -= 2) {
      *(uint16 *)&mother_brain_indirect_hdma[v7 + 4] = g_word_88C46E[i >> 1] + r20;
      v7 = (v7 - 2) & 0x1E;
    }
    if ((fx_liquid_options & 1) != 0)
      hdma_object_C[(uint8)hdma_object_index >> 1] += 64;
    if ((fx_y_pos & 0x8000) == 0) {
      v9 = fx_y_pos - layer1_y_pos + 256;
      if (v9 < 0) {
        v9 = (fx_y_pos - layer1_y_pos) & 0xF | 0x100;
LABEL_22:
        hdma_object_table_pointers[(uint8)hdma_object_index >> 1] = 3 * (((v9 ^ 0x1FF) + 1) & 0x3FF) + 0xC645;
        return;
      }
      if ((uint16)v9 < 0x200)
        goto LABEL_22;
    }
    v9 = 511;
    goto LABEL_22;
  }
}

const uint8 *HdmaobjInstr_SetFlagB_Copy3(uint16 k, const uint8 *hdp) {  // 0x88C582
  hdma_object_B[k >> 1] = 1;
  return hdp;
}

void HdmaobjPreInstr_WaterBG2XScroll(uint16 k) {  // 0x88C589
  int16 v1;

  g_word_7E9C44 = reg_BG2HOFS;
  if (time_is_frozen_flag)
    goto LABEL_4;
  if ((fx_liquid_options & 2) == 0) {
    hdma_object_channels_bitmask[(uint8)k >> 1] = 0;
LABEL_4:
    HdmaobjPreInstr_WaterBG2XScroll_Func1(k);
    goto LABEL_6;
  }
  HdmaobjPreInstr_WaterBG2XScroll_Func2(k);
LABEL_6:
  if ((fx_y_pos & 0x8000) != 0)
    goto LABEL_10;
  v1 = fx_y_pos - layer1_y_pos + 256;
  if (v1 >= 0) {
    if ((uint16)v1 < 0x200)
      goto LABEL_11;
LABEL_10:
    v1 = 511;
    goto LABEL_11;
  }
  v1 = (fx_y_pos - layer1_y_pos) & 0xF | 0x100;
LABEL_11:
  hdma_object_table_pointers[(uint8)k >> 1] = 3 * ((v1 ^ 0x1FF) & 0x3FF) + 0xCF46;
}

void HdmaobjPreInstr_WaterBG2XScroll_Func2(uint16 k) {  // 0x88C5E4
  int v1 = (uint8)k >> 1;
  if (hdma_object_B[v1]-- == 1) {
    hdma_object_B[v1] = 6;
    hdma_object_A[v1] = (hdma_object_A[v1] + 2) & 0x1E;
  }
  uint8 v3 = (LOBYTE(hdma_object_A[v1]) + 2 * (reg_BG2VOFS & 0xF)) & 0x1E;
  uint8 v4 = (2 * (reg_BG2VOFS & 0xF) + 30) & 0x1E;
  int n = 15;
  do {
    g_word_7E9C46[(v4 >> 1) + 1] = g_word_88C46E[v3 >> 1] + reg_BG2HOFS;
    v4 = (v4 - 2) & 0x1E;
    v3 = (v3 - 2) & 0x1E;
  } while (--n >= 0);
}

void HdmaobjPreInstr_WaterBG2XScroll_Func1(uint16 k) {  // 0x88C636
  uint8 v1 = 30;
  uint16 v2 = reg_BG2HOFS;
  do {
    g_word_7E9C46[(v1 >> 1) + 1] = v2;
    v1 -= 2;
  } while ((v1 & 0x80) == 0);
}

void SpawnBG3ScrollHdmaObject(void) {  // 0x88D865
  static const SpawnHdmaObject_Args unk_88D869 = { 0x43, 0x11, 0xd8d0 };
  SpawnHdmaObjectToSlot0xA(0x88, &unk_88D869);
}

void FxTypeFunc_28_CeresRidley(void) {  // 0x88D8DE
  static const SpawnHdmaObject_Args unk_88D8E2 = { 0x40, 0x05, 0xd906 };
  static const SpawnHdmaObject_Args unk_88D8EA = { 0x00, 0x2c, 0xd91d };
  SpawnHdmaObject(0x88, &unk_88D8E2);
  SpawnHdmaObject(0x88, &unk_88D8EA);
}

void sub_88D916(void) {  // 0x88D916
  hdma_data_table_in_ceres = 9;
}

void FxTypeFunc_CeresElevator(void) {  // 0x88D928
  static const SpawnHdmaObject_Args unk_88D92C = { 0x40, 0x05, 0xd939 };
  SpawnHdmaObject(0x88, &unk_88D92C);
}

const uint8 *HdmaobjInstr_SetVideoMode1(uint16 k, const uint8 *hdp) {  // 0x88D949
  hdma_data_table_in_ceres = 9;
  return hdp;
}

void FxTypeFunc_A_Rain(void) {  // 0x88D950
  static const SpawnHdmaObject_Args unk_88D95C = { 0x43, 0x11, 0xd96c };
  gameplay_BG3SC = 92;
  SpawnHdmaObject(0x88, &unk_88D95C);
  SpawnBG3ScrollHdmaObject();
  SpawnAnimtiles(addr_kAnimtiles_Rain);
}


const uint8 *HdmaobjInstr_1938_RandomNumber(uint16 k, const uint8 *hdp) {  // 0x88D981
  hdma_object_D[k >> 1] = g_word_88D992[(uint16)((random_number >> 1) & 6) >> 1];
  return hdp;
}

void HdmaobjPreInstr_RainBg3Scroll(uint16 k) {  // 0x88D9A1
  gameplay_BG3SC = 92;
  fx_layer_blending_config_c = fx_layer_blending_config_b;
  if (!time_is_frozen_flag) {
    int v1 = (uint8)k >> 1;
    bg3_ypos = hdma_object_E[v1] - layer1_y_pos + (int8)HIBYTE(hdma_object_A[v1]);
    hdma_object_A[v1] -= 1536;
    hdma_object_E[v1] = layer1_y_pos;
    bg3_xpos = hdma_object_F[v1] - layer1_x_pos + (int8)HIBYTE(hdma_object_B[v1]);
    hdma_object_B[v1] += hdma_object_D[v1];
    hdma_object_F[v1] = layer1_x_pos;
  }
}

void FxTypeFunc_8_Spores(void) {  // 0x88DA11
  gameplay_BG3SC = 92;
  static const SpawnHdmaObject_Args unk_88DA1D = { 0x42, 0x11, 0xda2d };
  SpawnHdmaObject(0x88, &unk_88DA1D);
  SpawnBG3ScrollHdmaObject();
  SpawnAnimtiles(addr_kAnimtiles_Spores);
}

void HdmaobjPreInstr_SporesBG3Xscroll(uint16 k) {  // 0x88DA47
  gameplay_BG3SC = 92;
  fx_layer_blending_config_c = fx_layer_blending_config_b;
  if (!time_is_frozen_flag) {
    int v1 = (uint8)k >> 1;
    bg3_ypos = layer1_y_pos + (int8)HIBYTE(hdma_object_C[v1]);
    uint16 v3 = hdma_object_C[v1] - 64;
    hdma_object_C[v1] = v3;
//    R20 = v3;
    bg3_xpos = layer1_x_pos + (int8)HIBYTE(hdma_object_D[v1]);
//    R18 = layer1_x_pos + v3;
    hdma_object_D[v1] = hdma_object_D[v1];
  }
}

void FxTypeFunc_C(void) {  // 0x88DB08
  gameplay_BG3SC = 92;
  static const SpawnHdmaObject_Args unk_88DB14 = { 0x43, 0x11, 0xdb19 };
  SpawnHdmaObject(0x88, &unk_88DB14);
}

void HdmaobjPreInstr_FogBG3Scroll(uint16 k) {  // 0x88DB36
  gameplay_BG3SC = 92;
  fx_layer_blending_config_c = fx_layer_blending_config_b;
  if (!time_is_frozen_flag) {
    int v1 = (uint8)k >> 1;
    bg3_ypos = layer1_y_pos + (int8)HIBYTE(hdma_object_A[v1]);
    hdma_object_A[v1] -= 64;
    bg3_xpos = layer1_x_pos + (int8)HIBYTE(hdma_object_B[v1]);
    hdma_object_B[v1] += 80;
  }
}

void FxTypeFunc_26_TourianEntranceStatue(void) {  // 0x88DB8A
  static const SpawnHdmaObject_Args unk_88DBBA = { 0x42, 0x11, 0xd856 };
  static const SpawnHdmaObject_Args unk_88DBC2 = { 0x42, 0x10, 0xdcfa };

  if (CheckEventHappened(0xA) & 1) {
    SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x06, 0x0c, 0xb777 });
    *(uint16 *)scrolls = 514;
  }
  reg_BG2SC = 74;
  fx_rising_function_bank_88 = FUNC16(FxRisingFunction_C428_WaterNormal);
  fx_y_pos = fx_base_y_pos;
  SpawnHdmaObject(0x88, &unk_88DBBA);
  SpawnHdmaObject(0x88, &unk_88DBC2);
  SpawnBG3ScrollHdmaObject();
}

void sub_88DBCB(uint16 k) {  // 0x88DBCB
  *(uint16 *)&hdma_window_1_left_pos[0].field_0 = layer1_y_pos + hdma_object_B[k >> 1];
}

void HdmaobjPreInstr_CheckLotsOfEventsHappened(uint16 v0) {  // 0x88DBD7
  if (CheckEventHappened(6) & 1) {
    if (CheckEventHappened(7) & 1) {
      if (CheckEventHappened(8) & 1) {
        if (CheckEventHappened(9) & 1) {
          tourian_entrance_statue_animstate |= 0x10;
          if ((tourian_entrance_statue_animstate & 0x8000) == 0) {
            hdma_object_C[v0 >> 1] = 300;
            v0 = hdma_object_index;
            int v1 = hdma_object_index >> 1;
            hdma_object_instruction_timers[v1] = 1;
            hdma_object_instruction_list_pointers[v1] += 2;
          }
        }
      }
    }
  }
  sub_88DBCB(v0);
}

void HdmaobjPreInstr_DC23(uint16 k) {  // 0x88DC23
  uint16 v0 = k;

  HandleEarthquakeSoundEffect();
  earthquake_type = 13;
  earthquake_timer |= 0x20;
  int v1 = (uint8)v0 >> 1;
  if ((--hdma_object_C[v1] & 0x8000) != 0) {
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_TourianStatueDustClouds, 0); // ?
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_TourianStatueDustClouds, 0);
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_TourianStatueDustClouds, 0);
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_TourianStatueDustClouds, 0);
    hdma_object_instruction_timers[v1] = 1;
    hdma_object_instruction_list_pointers[v1] += 2;
  }
  sub_88DBCB(v0);
}

void HdmaobjPreInstr_DC69(uint16 k) {  // 0x88DC69
  uint16 v0 = k;

  HandleEarthquakeSoundEffect();
  earthquake_type = 13;
  earthquake_timer |= 0x20;
  if (!time_is_frozen_flag) {
    int v1 = (uint8)v0 >> 1;
    uint16 v2 = hdma_object_A[v1];
    hdma_object_A[v1] = v2 - 0x4000;
    uint16 v3 = __CFADD__uint16(v2, -16384) + hdma_object_B[v1] - 1;
    hdma_object_B[v1] = v3;
    if (v3 == 0xFF10) {
      SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x06, 0x0c, 0xb773 });
      SetEventHappened(0xA);
      int v4 = (uint8)v0 >> 1;
      hdma_object_instruction_timers[v4] = 1;
      hdma_object_instruction_list_pointers[v4] += 2;
    }
    sub_88DBCB(v0);
  }
}

void HdmaobjPreInstr_DCBA(uint16 v0) {  // 0x88DCBA
  tourian_entrance_statue_finished = 0x8000;
  *(uint16 *)scrolls = 514;
  sub_88DBCB(v0);
}

const uint8 *HdmaobjInstr_GotoIfEventHappened(uint16 k, const uint8 *hdp) {  // 0x88DCCB
  int v2 = k >> 1;
  hdma_object_C[v2] = 0;
  hdma_object_A[v2] = 0;
  if (CheckEventHappened(0xA)) {
    hdma_object_B[v2] = -240;
    *(uint16 *)&hdma_window_1_left_pos[0].field_0 = -240;
    return INSTRB_RETURN_ADDR(GET_WORD(hdp));
  } else {
    hdma_object_B[v2] = 0;
    *(uint16 *)&hdma_window_1_left_pos[0].field_0 = 0;
    *(uint16 *)scrolls = 1;
    return hdp + 2;
  }
}

void SpawnBombTorizoHaze(void) {  // 0x88DD32
  static const SpawnHdmaObject_Args unk_88DD36 = { 0x02, 0x32, 0xdd4a };
  static const SpawnHdmaObject_Args unk_88DD3E = { 0x00, 0x32, 0xdd62 };
  SpawnHdmaObject(0x88, &unk_88DD36);
  SpawnHdmaObject(0x88, &unk_88DD3E);
}

void HdmaobjPreInstr_BombTorizoHazeColorMathBgColor(uint16 k) {  // 0x88DD43
  fx_layer_blending_config_c = 44;
}

void FxTypeFunc_2C_Haze(void) {  // 0x88DDC7
  static const SpawnHdmaObject_Args unk_88DDD4 = { 0x40, 0x32, 0xded3 };
  static const SpawnHdmaObject_Args unk_88DDDD = { 0x40, 0x32, 0xdeeb };
  if (CheckBossBitForCurArea(1) & 1)
    SpawnHdmaObject(0x88, &unk_88DDDD);
  else
    SpawnHdmaObject(0x88, &unk_88DDD4);
}

void HdmaobjPreInstr_HazeColorMathSubscreen_CeresRidleyAlive(uint16 k) {  // 0x88DE10
  sub_88DE18(k, 0x80);
}

void HdmaobjPreInstr_HazeColorMathSubscreen_CeresRidleyDead(uint16 k) {  // 0x88DE15
  sub_88DE18(k, 0x20);
}

void sub_88DE18(uint16 k, uint16 a) {  // 0x88DE18
  int v2 = (uint8)k >> 1;
  hdma_object_B[v2] = a;
  hdma_object_A[v2] = 0;
  if (door_transition_function == FUNC16(DoorTransition_FadeInScreenAndFinish)) {
    hdma_object_pre_instructions[v2] = FUNC16(HdmaobjPreInstr_HazeColorMathSubscreen_FadingIn);
    HdmaobjPreInstr_HazeColorMathSubscreen_FadingIn(k);
  }
}

void HdmaobjPreInstr_HazeColorMathSubscreen_FadingIn(uint16 k) {  // 0x88DE2D
  int8 v1;

  reg_COLDATA[0] = 32;
  reg_COLDATA[1] = 64;
  reg_COLDATA[2] = 0x80;
  fx_layer_blending_config_c = 44;
  if (hdma_object_A[(uint8)k >> 1] == 16) {
    hdma_object_pre_instructions[(uint8)k >> 1] = FUNC16(HdmaobjPreInstr_HazeColorMathSubscreen_FadedIn);
  } else {
    uint8 v3 = k;
    uint8 r20 = *((uint8 *)hdma_object_B + (uint8)k);
    v1 = *((uint8 *)hdma_object_A + (uint8)k);
    for (int i = 15; i >= 0; --i) {
      uint8 r18 = v1;
      *((uint8 *)hdma_table_2 + (uint8)i) = r20 | v1;
      v1 = r18 - 1;
      if ((int8)(r18 - 1) < 0)
        v1 = 0;
    }
    ++hdma_object_A[v3 >> 1];
  }
}

void HdmaobjPreInstr_HazeColorMathSubscreen_FadedIn(uint16 k) {  // 0x88DE74
  reg_COLDATA[0] = 32;
  reg_COLDATA[1] = 64;
  reg_COLDATA[2] = 0x80;
  fx_layer_blending_config_c = 44;
  if (door_transition_function == FUNC16(DoorTransitionFunction_FadeOutScreen))
    hdma_object_pre_instructions[(uint8)k >> 1] = FUNC16(HdmaobjPreInstr_HazeColorMathSubscreen_FadingOut);
}

void HdmaobjPreInstr_HazeColorMathSubscreen_FadingOut(uint16 k) {  // 0x88DE96
  int8 v1;

  reg_COLDATA[0] = 32;
  reg_COLDATA[1] = 64;
  reg_COLDATA[2] = 0x80;
  fx_layer_blending_config_c = 44;
  if (hdma_object_A[(uint8)k >> 1]) {
    uint8 v3 = k;
    uint8 r20 = *((uint8 *)hdma_object_B + (uint8)k);
    v1 = *((uint8 *)hdma_object_A + (uint8)k);
    for (int i = 15; i >= 0; --i) {
      uint8 r18 = v1;
      *((uint8 *)hdma_table_2 + (uint8)i) = r20 | v1;
      v1 = r18 - 1;
      if ((int8)(r18 - 1) < 0)
        v1 = 0;
    }
    --hdma_object_A[v3 >> 1];
  }
}

static const SpawnHdmaObject_Args unk_88DF38 = { 0x00, 0x2c, 0xdf4f };
static const SpawnHdmaObject_Args unk_88DF41 = { 0x00, 0x2c, 0xdf6b };

static const SpawnHdmaObject_Args unk_88DF4A = { 0x02, 0x12, 0xdf77 };
void sub_88DF34(void) {  // 0x88DF34
  SpawnHdmaObject(0x88, &unk_88DF38);
}

void sub_88DF3D(void) {  // 0x88DF3D
  SpawnHdmaObject(0x88, &unk_88DF41);
}

void sub_88DF46(void) {  // 0x88DF46
  SpawnHdmaObject(0x88, &unk_88DF4A);
}

void HdmaobjPreInstr_DF94(uint16 v0) {  // 0x88DF94
  int16 v1;

  if ((enemy_data[0].properties & 0x200) == 0
      && (int16)(enemy_data[0].x_pos - layer1_x_pos + 64) >= 0
      && sign16(enemy_data[0].x_pos - layer1_x_pos - 320)
      && (int16)(enemy_data[0].y_pos - layer1_y_pos + 16) >= 0
      && (v1 = enemy_data[0].y_pos - layer1_y_pos, sign16(enemy_data[0].y_pos - layer1_y_pos - 304))) {
    if (sign16(v1 - 40)) {
      int v2 = v0 >> 1;
      hdma_object_instruction_timers[v2] = 1;
      hdma_object_instruction_list_pointers[v2] = addr_word_88DF6B;
    } else {
      int v4 = v0 >> 1;
      hdma_object_instruction_timers[v4] = 1;
      if (sign16(v1 - 192))
        hdma_object_instruction_list_pointers[v4] = addr_word_88DF5F;
      else
        hdma_object_instruction_list_pointers[v4] = addr_word_88DF65;
    }
  } else {
    int v3 = v0 >> 1;
    hdma_object_instruction_timers[v3] = 1;
    hdma_object_instruction_list_pointers[v3] = addr_word_88DF71;
  }
}

static Func_U8 *const off_88E04E[7] = {  // 0x88E026
  VariaSuitPickup_0_LightBeamAppears,
  VariaSuitPickup_1,
  VariaSuitPickup_2_LightBeamWidens,
  VariaSuitPickup_3,
  VariaSuitPickup_4_LightBeamShrinks,
  VariaSuitPickup_5_LightBeamDissipates,
  VariaSuitPickup_6,
};

void HdmaobjPreInstr_VariaSuitPickup(uint16 k) {
  if (off_88E04E[substate]() & 1) {
    reg_COLDATA[0] = suit_pickup_color_math_R;
    reg_COLDATA[1] = suit_pickup_color_math_G;
    reg_COLDATA[2] = suit_pickup_color_math_B;
    fx_layer_blending_config_c = 18;
  }
}

static Func_U8 *const off_88E084[7] = {  // 0x88E05C
  VariaSuitPickup_0_LightBeamAppears,
  VariaSuitPickup_1,
  VariaSuitPickup_2_LightBeamWidens,
  GravitySuitPickup_3,
  VariaSuitPickup_4_LightBeamShrinks,
  VariaSuitPickup_5_LightBeamDissipates,
  GravitySuitPickup_6,
};

void HdmaobjPreInstr_GravitySuitPickup(uint16 k) {
  if (off_88E084[substate]() & 1) {
    reg_COLDATA[0] = suit_pickup_color_math_R;
    reg_COLDATA[1] = suit_pickup_color_math_G;
    reg_COLDATA[2] = suit_pickup_color_math_B;
    fx_layer_blending_config_c = 18;
  }
}
static const uint16 g_word_88E0D5 = 0x7878;
uint8 VariaSuitPickup_0_LightBeamAppears(void) {  // 0x88E092
  int16 v0;
  int16 v3;

  suit_pickup_light_beam_pos += 8;
  v0 = suit_pickup_light_beam_pos;
  uint16 v1 = 0;
  do {
    hdma_table_1[v1 >> 1] = g_word_88E0D5;
    v1 += 2;
    --v0;
  } while (v0 > 0);
  uint16 v2 = 510;
  v3 = suit_pickup_light_beam_pos;
  do {
    hdma_table_1[v2 >> 1] = g_word_88E0D5;
    v2 -= 2;
    --v3;
  } while (v3 > 0);
  if (!sign16(suit_pickup_light_beam_pos - 128)) {
    ++substate;
    suit_pickup_light_beam_pos = 30840;
  }
  return 1;
}

uint8 VariaSuitPickup_1(void) {  // 0x88E0D7
  LOBYTE(suit_pickup_light_beam_pos) = suit_pickup_light_beam_pos - HIBYTE(suit_pickup_light_beam_widening_speed);
  HIBYTE(suit_pickup_light_beam_pos) += HIBYTE(suit_pickup_light_beam_widening_speed);
  for (int i = 510; i >= 0; i -= 2)
    hdma_table_1[i >> 1] = suit_pickup_light_beam_pos;
  if (sign16((uint8)suit_pickup_light_beam_pos - 97)) {
    ++substate;
    suit_pickup_light_beam_pos = 0x846C;
  }
  return 1;
}


uint8 VariaSuitPickup_2_LightBeamWidens(void) {  // 0x88E113
  int8 v1;
  int16 v2;
  int8 v4;
  int8 v6;
  int8 v8;
  int8 v10;

  AdvanceSuitPickupColorMathSubscreenToWhite();
  bool v0 = (int8)(suit_pickup_light_beam_pos - HIBYTE(suit_pickup_light_beam_widening_speed)) < 0;
  LOBYTE(suit_pickup_light_beam_pos) = suit_pickup_light_beam_pos - HIBYTE(suit_pickup_light_beam_widening_speed);
  if (!(uint8)suit_pickup_light_beam_pos || v0) {
    suit_pickup_light_beam_pos = -256;
  } else {
    v1 = HIBYTE(suit_pickup_light_beam_widening_speed) + HIBYTE(suit_pickup_light_beam_pos);
    if (__CFADD__uint8(HIBYTE(suit_pickup_light_beam_widening_speed), HIBYTE(suit_pickup_light_beam_pos)))
      v1 = -1;
    HIBYTE(suit_pickup_light_beam_pos) = v1;
  }
  v2 = 0;
  uint16 v3 = 0;
  do {
    v4 = suit_pickup_light_beam_pos - g_byte_88E3C9[v3];
    if (v4 < 0)
      v4 = 0;
    *((uint8 *)hdma_table_1 + (uint16)v2) = v4;
    uint16 v5 = v2 + 1;
    v6 = g_byte_88E3C9[v3] + HIBYTE(suit_pickup_light_beam_pos);
    if (__CFADD__uint8(g_byte_88E3C9[v3], HIBYTE(suit_pickup_light_beam_pos)))
      v6 = -1;
    *((uint8 *)hdma_table_1 + v5) = v6;
    v2 = v5 + 1;
    ++v3;
  } while (v2 < 0x100);
  uint16 v7 = v3 - 1;
  do {
    v8 = suit_pickup_light_beam_pos - g_byte_88E3C9[v7];
    if (v8 < 0)
      v8 = 0;
    *((uint8 *)hdma_table_1 + (uint16)v2) = v8;
    uint16 v9 = v2 + 1;
    v10 = g_byte_88E3C9[v7] + HIBYTE(suit_pickup_light_beam_pos);
    if (__CFADD__uint8(g_byte_88E3C9[v7], HIBYTE(suit_pickup_light_beam_pos)))
      v10 = -1;
    *((uint8 *)hdma_table_1 + v9) = v10;
    v2 = v9 + 1;
    --v7;
  } while (v2 < 0x200);
  suit_pickup_light_beam_widening_speed += 96;
  if (suit_pickup_light_beam_pos == 0xFF00) {
    ++substate;
    suit_pickup_light_beam_widening_speed >>= 1;
    suit_pickup_light_beam_pos = 0;
  }
  return 1;
}

uint8 VariaSuitPickup_4_LightBeamShrinks(void) {  // 0x88E1BA
  if (suit_pickup_palette_transition_color)
    AdvanceSuitPickupColorMathToBlue();
  else
    AdvanceSuitPickupColorMathToOrange();
  suit_pickup_light_beam_pos += (suit_pickup_light_beam_widening_speed & 0xFF00) >> 8;
  int16 v0 = suit_pickup_light_beam_pos;
  uint16 v1 = 0;
  do {
    hdma_table_1[v1 >> 1] = 255;
    v1 += 2;
    --v0;
  } while (v0 > 0);
  uint16 v2 = 510;
  int16 v3 = suit_pickup_light_beam_pos - 1;
  do {
    hdma_table_1[v2 >> 1] = 255;
    v2 -= 2;
    --v3;
  } while (v3 > 0);
  suit_pickup_light_beam_widening_speed -= 32;
  if (sign16(suit_pickup_light_beam_widening_speed - 256))
    suit_pickup_light_beam_widening_speed = 256;
  if (!sign16(suit_pickup_light_beam_pos - 128)) {
    ++substate;
    suit_pickup_light_beam_pos = -1793;
  }
  return 1;
}

uint8 VariaSuitPickup_5_LightBeamDissipates(void) {  // 0x88E22B
  LOBYTE(suit_pickup_light_beam_pos) = suit_pickup_light_beam_pos + 8;
  HIBYTE(suit_pickup_light_beam_pos) -= 8;
  hdma_table_1[128] = suit_pickup_light_beam_pos;
  if (!sign16((uint8)suit_pickup_light_beam_pos - 112))
    ++substate;
  return 1;
}

uint8 VariaSuitPickup_6(void) {  // 0x88E258
  QueueMusic_Delayed8(3);
  return GravitySuitPickup_6();
}

uint8 GravitySuitPickup_6(void) {  // 0x88E25F
  reg_COLDATA[2] = 0x80;
  reg_COLDATA[1] = 64;
  reg_COLDATA[0] = 32;
  mov24(&hdma_ptr_1, 0x980001);
  *(uint16 *)((uint8 *)&demo_num_input_frames + 1) = 0;
  demo_input_prev = 0;
  demo_input_prev_new = 0;
  demo_backup_prev_controller_input = 0;
  hdma_table_1[0] = 255;
  substate = 0;
  suit_pickup_light_beam_pos = 0;
  *(uint16 *)&suit_pickup_color_math_R = 0;
  *(uint16 *)&suit_pickup_color_math_B = 0;
  int v0 = hdma_object_index >> 1;
  hdma_object_instruction_list_pointers[v0] += 2;
  hdma_object_instruction_timers[v0] = 1;
  CallSomeSamusCode(0xB);
  return 0;
}

uint8 AdvanceSuitPickupColorMathSubscreenToWhite(void) {  // 0x88E2B4
  if (suit_pickup_color_math_R != 63) {
    suit_pickup_color_math_R += 2;
    if (!sign8(suit_pickup_color_math_R - 64))
      suit_pickup_color_math_R = 63;
  }
  if (suit_pickup_color_math_G != 95) {
    suit_pickup_color_math_G += 2;
    if (!sign8(suit_pickup_color_math_G - 96))
      suit_pickup_color_math_G = 95;
  }
  if (suit_pickup_color_math_B != 0x9F) {
    suit_pickup_color_math_B += 2;
    if (!sign8(suit_pickup_color_math_B + 96))
      suit_pickup_color_math_B = -97;
  }
  return 1;
}

uint8 AdvanceSuitPickupColorMathToOrange(void) {  // 0x88E2F9
  if (suit_pickup_color_math_R != 63)
    --suit_pickup_color_math_R;
  if (suit_pickup_color_math_G != 77)
    --suit_pickup_color_math_G;
  if (suit_pickup_color_math_B != 0x83)
    --suit_pickup_color_math_B;
  return 1;
}

uint8 VariaSuitPickup_3(void) {  // 0x88E320
  equipped_items |= 1;
  collected_items |= 1;
  samus_pose = kPose_9B_FaceF_VariaGravitySuit;
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
  samus_last_different_pose = samus_prev_pose;
  *(uint16 *)&samus_last_different_pose_x_dir = *(uint16 *)&samus_prev_pose_x_dir;
  samus_prev_pose = samus_pose;
  *(uint16 *)&samus_prev_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
  Samus_LoadSuitPalette();
  ++substate;
  return 1;
}

uint8 GravitySuitPickup_3(void) {  // 0x88E361
  equipped_items |= 0x20;
  collected_items |= 0x20;
  samus_pose = kPose_9B_FaceF_VariaGravitySuit;
  SamusFunc_F433();
  Samus_SetAnimationFrameIfPoseChanged();
  samus_last_different_pose = samus_prev_pose;
  *(uint16 *)&samus_last_different_pose_x_dir = *(uint16 *)&samus_prev_pose_x_dir;
  samus_prev_pose = samus_pose;
  *(uint16 *)&samus_prev_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
  Samus_LoadSuitPalette();
  ++substate;
  return 1;
}

uint8 AdvanceSuitPickupColorMathToBlue(void) {  // 0x88E3A2
  if (suit_pickup_color_math_R != 48)
    --suit_pickup_color_math_R;
  if (suit_pickup_color_math_G != 73)
    --suit_pickup_color_math_G;
  if (suit_pickup_color_math_B != 0x90)
    --suit_pickup_color_math_B;
  return 1;
}

void HdmaobjPreInstr_E449(uint16 k) {  // 0x88E449
  if ((phantom_related_layer_flag & 0x4000) != 0) {
    fx_layer_blending_config_c = 26;
  } else if (LOBYTE(enemy_data[3].parameter_1)) {
    if (LOBYTE(enemy_data[3].parameter_1) == 0xFF) {
      fx_layer_blending_config_c = 4;
      int v0 = hdma_object_index >> 1;
      hdma_object_instruction_list_pointers[v0] += 2;
      hdma_object_instruction_timers[v0] = 1;
    }
  } else {
    fx_layer_blending_config_c = 4;
  }
}
static const SpawnHdmaObject_Args unk_88E4A0 = { 0x42, 0x0f, 0xe4a8 };
void sub_88E487(uint16 v0, uint16 r22) {  // 0x88E487
  enemy_data[2].parameter_1 = v0;
  enemy_data[3].ai_var_D = 0;
  enemy_data[3].ai_var_E = 0;
  enemy_data[3].ai_preinstr = r22;
  SpawnHdmaObject(0x88, &unk_88E4A0);
}

const uint8 *HdmaobjInstr_E4BD(uint16 k, const uint8 *hdp) {  // 0x88E4BD
  unsigned int v2; // kr00_4
  unsigned int v3; // kr04_4
  unsigned int v4; // kr08_4
  unsigned int v5; // kr0C_4

  enemy_data[1].parameter_1 = enemy_data[2].parameter_1;
  if ((enemy_data[2].parameter_1 & 1) != 0) {
    kraid_unk9000 = 192;
    g_word_7E9006 = 192;
    v4 = 12620032;
    *(uint16 *)((uint8 *)&g_word_7E9002 + 1) = HIWORD(v4);
    *(uint16 *)((uint8 *)&kraid_unk9000 + 1) = v4;
    g_word_7E9004 = -28288;
    v5 = 12620032;
    *(uint16 *)((uint8 *)&g_word_7E9008 + 1) = HIWORD(v5);
    *(uint16 *)((uint8 *)&g_word_7E9006 + 1) = v5;
    g_word_7E900A = addr_loc_889180;
    g_word_7E900C = 0;
  } else {
    kraid_unk9000 = 160;
    g_word_7E9006 = 160;
    g_word_7E900C = 160;
    g_word_7E900F = 160;
    g_word_7E9012 = 160;
    g_word_7E9015 = 160;
    v2 = 10522880;
    *(uint16 *)((uint8 *)&g_word_7E9002 + 1) = HIWORD(v2);
    *(uint16 *)((uint8 *)&kraid_unk9000 + 1) = v2;
    v3 = 10522880;
    *(uint16 *)((uint8 *)&g_word_7E9008 + 1) = HIWORD(v3);
    *(uint16 *)((uint8 *)&g_word_7E9006 + 1) = v3;
    *(uint16 *)((uint8 *)&g_word_7E900C + 1) = -28416;
    *(uint16 *)((uint8 *)&g_word_7E9012 + 1) = -28416;
    g_word_7E9004 = -28352;
    g_word_7E900A = -28352;
    *(uint16 *)((uint8 *)&g_word_7E900F + 1) = -28352;
    *(uint16 *)((uint8 *)&g_word_7E9015 + 1) = -28352;
    g_word_7E9018 = 0;
  }
  int v6 = k >> 1;
  hdma_object_A[v6] = -2;
  hdma_object_B[v6] = 1;
  hdma_object_C[v6] = 0;
  hdma_object_D[v6] = 0;
  return hdp;
}

void HdmaobjPreInstr_E567(uint16 v0) {  // 0x88E567
  int16 v7;
  uint16 j;
  uint16 v8;
  uint16 r28;
  uint16 r30;

  if (enemy_data[1].parameter_1) {
    if ((enemy_data[1].parameter_1 & 1) != 0) {
      r28 = 4;
      r30 = 128;
    } else {
      r28 = 8;
      r30 = 64;
    }
    int v2 = v0 >> 1;
    if (hdma_object_B[v2]-- == 1) {
      hdma_object_B[v2] = 1;
      hdma_object_A[v2] = (2 * enemy_data[3].ai_preinstr + hdma_object_A[v2]) & 0x1FF;
    }
    uint16 r20 = hdma_object_A[v2];
    uint16 v4 = 0;
    do {
      uint16 v11 = v4;
      uint16 v5 = r20;
      int v6 = r20 >> 1;
      v7 = kSinCosTable8bit_Sext[v6 + 64];
      if (v7 < 0) {
        uint16 r18 = -v7;
        uint16 r22 = Mult8x8(-(int8)v7, enemy_data[3].ai_var_D) >> 8;
        r22 += Mult8x8(HIBYTE(r18), enemy_data[3].ai_var_D);
        uint16 r24 = Mult8x8(r18, HIBYTE(enemy_data[3].ai_var_D));
        uint8 mult = Mult8x8(HIBYTE(r18), HIBYTE(enemy_data[3].ai_var_D));
        r22 += r24;
        r18 = ((r22 + (mult << 8)) & 0xFF00) >> 8;
        r20 = (r28 + v5) & 0x1FF;
        v8 = v11;
        *(uint16 *)((uint8 *)&g_word_7E9100 + v11) = reg_BG2HOFS - r18;
      } else {
        uint16 r18 = kSinCosTable8bit_Sext[v6 + 64];
        uint16 r22 = Mult8x8(v7, enemy_data[3].ai_var_D) >> 8;
        r22 += Mult8x8(HIBYTE(r18), enemy_data[3].ai_var_D);
        uint16 r24 = Mult8x8(r18, HIBYTE(enemy_data[3].ai_var_D));
        uint8 mult = Mult8x8(HIBYTE(r18), HIBYTE(enemy_data[3].ai_var_D));
        r22 += r24;
        r18 = ((r22 + (mult << 8)) & 0xFF00) >> 8;
        r20 = (r28 + v5) & 0x1FF;
        v8 = v11;
        *(uint16 *)((uint8 *)&g_word_7E9100 + v11) = r18 + reg_BG2HOFS;
      }
      v4 = v8 + 2;
    } while ((int16)(v4 - r30) < 0);
    if ((enemy_data[1].parameter_1 & 1) != 0) {
      for (int i = 126; i >= 0; i -= 2)
        *(uint16 *)((uint8 *)&g_word_7E9180 + i) = reg_BG2HOFS + reg_BG2HOFS - *(uint16 *)((uint8 *)&g_word_7E9100 + i);
    } else {
      for (j = 62; (j & 0x8000) == 0; j -= 2)
        *(uint16 *)((uint8 *)&g_word_7E9140 + j) = reg_BG2HOFS + reg_BG2HOFS - *(uint16 *)((uint8 *)&g_word_7E9100 + j);
    }
  } else {
    int v1 = hdma_object_index >> 1;
    hdma_object_instruction_list_pointers[v1] += 2;
    hdma_object_instruction_timers[v1] = 1;
  }
}
static const SpawnHdmaObject_Args unk_88E722 = { 0x00, 0x2c, 0xe727 };
static const SpawnHdmaObject_Args unk_88E74C = { 0x41, 0x26, 0xe751 };
uint16 MotherBrainRisingHdmaObject(void) {  // 0x88E71E
  return SpawnHdmaObject(0x88, &unk_88E722);
}

uint16 SpawnMotherBrainRainbowBeamHdma(void) {  // 0x88E748
  return SpawnHdmaObject(0x88, &unk_88E74C);
}

void InitializeRainbowBeam(void) {  // 0x88E767
  reg_COLDATA[0] = 32;
  reg_COLDATA[1] = 71;
  reg_COLDATA[2] = 0x8f;
  fx_layer_blending_config_c = 36;
  mother_brain_indirect_hdma[0] = 100;
  *(uint16 *)&mother_brain_indirect_hdma[1] = -25344;
  *(uint16 *)&mother_brain_indirect_hdma[3] = 0;
  hdma_table_2[0] = 0;
  hdma_table_2[1] = -32736;
  hdma_table_2[2] = 0;
  MotherBrain_CalculateRainbowBeamHdma();
}

void HdmaobjPreInstr_E7BC(uint16 k) {  // 0x88E7BC
  if (game_state == 19) {
    hdma_object_channels_bitmask[hdma_object_index >> 1] = 0;
  } else {
    fx_layer_blending_config_c = 36;
    if (game_state != 27) {
      MotherBrain_CalculateRainbowBeamHdma();
      sub_88E7ED();
    }
  }
}


void sub_88E7ED(void) {  // 0x88E7ED
  uint16 v0 = g_word_88E833[hdma_object_A[0] >> 1];
  if ((v0 & 0x8000) == 0) {
    ++hdma_object_A[0];
    ++hdma_object_A[0];
    ++hdma_object_A[0];
    ++hdma_object_A[0];
  } else {
    hdma_object_A[0] = 0;
    v0 = g_word_88E833[0];
  }
  reg_COLDATA[0] = v0 & 0x1F | 0x20;
  reg_COLDATA[1] = (v0 >> 5) & 0x1F | 0x40;
  reg_COLDATA[2] = ((uint16)(v0 >> 2) >> 8) & 0x1F | 0x80;
}

void SpawnMorphBallEyeBeamHdma(void) {  // 0x88E8D9
  static const SpawnHdmaObject_Args unk_88E8E4 = { 0x41, 0x26, 0xe8ec };
  SpawnHdmaObject(0x88, &unk_88E8E4);
}

const uint8 *HdmaobjInstr_InitMorphBallEyeBeamHdma(uint16 k, const uint8 *hdp) {  // 0x88E917
  unsigned int v2; // kr00_4
  unsigned int v3; // kr04_4

  *((uint8 *)hdma_object_A + k) = 48;
  reg_COLDATA[0] = 48;
  *((uint8 *)hdma_object_A + k + 1) = 80;
  reg_COLDATA[1] = 80;
  *((uint8 *)hdma_object_B + k) = 0x80;
  reg_COLDATA[2] = 0x80;
  fx_layer_blending_config_c = 16;
  kraid_unk9000 = 228;
  v2 = 14979328;
  *(uint16 *)((uint8 *)&g_word_7E9002 + 1) = HIWORD(v2);
  *(uint16 *)((uint8 *)&kraid_unk9000 + 1) = v2;
  g_word_7E9004 = -28216;
  g_word_7E9006 = 152;
  v3 = 37520;
  *(uint16 *)((uint8 *)&g_word_7E9008 + 1) = HIWORD(v3);
  *(uint16 *)((uint8 *)&g_word_7E9006 + 1) = v3;
  enemy_data[1].ai_var_C = 1;
  int v4 = k >> 1;
  hdma_object_C[v4] = 0;
  hdma_object_D[v4] = 0;
  g_word_7E9080 = 0;
  g_word_7E9082 = 0;
  g_word_7E9090 = 0;
  return hdp;
}

void sub_88E987(uint16 v0) {  // 0x88E987
  uint16 v3 = v0;
  uint16 r18 = enemy_data[1].ai_var_D;
  uint16 r20 = hdma_object_C[v0 >> 1];
  uint16 v1 = enemy_data[1].y_pos - layer1_y_pos;
  uint16 v2 = enemy_data[1].x_pos - layer1_x_pos;
  if ((int16)(enemy_data[1].x_pos - layer1_x_pos) >= 0 && sign16(enemy_data[1].x_pos - layer1_x_pos - 256)) {
    CalculateXrayHdmaTableInner(v2, v1, r18, r20, false, &g_word_7E9100);
  } else {
    CalculateXrayHdmaTableInner(v2, v1, r18, r20, true, &g_word_7E9100);
  }
  reg_COLDATA[0] = *((uint8 *)hdma_object_A + v3);
  reg_COLDATA[1] = *((uint8 *)hdma_object_A + v3 + 1);
  reg_COLDATA[2] = *((uint8 *)hdma_object_B + v3);
}

void HdmaobjPreInstr_E9E6(uint16 k) {  // 0x88E9E6
  unsigned int v1; // kr00_4

  fx_layer_blending_config_c = 16;
  v1 = __PAIR32__(g_word_7E9080, g_word_7E9082) + 0x4000;
  g_word_7E9080 = (__PAIR32__(g_word_7E9080, g_word_7E9082) + 0x4000) >> 16;
  g_word_7E9082 = v1;
  int v2 = k >> 1;
  uint16 v3 = hdma_object_D[v2];
  hdma_object_D[v2] = v1 + v3;
  uint16 v4 = g_word_7E9080 + __CFADD__uint16((uint16)v1, v3) + hdma_object_C[v2];
  hdma_object_C[v2] = v4;
  if (!sign16(v4 - 4)) {
    hdma_object_C[v2] = 4;
    int v5 = hdma_object_index >> 1;
    hdma_object_instruction_list_pointers[v5] += 2;
    hdma_object_instruction_timers[v5] = 1;
  }
  sub_88E987(k);
}


void HdmaobjPreInstr_EA3C(uint16 k) {  // 0x88EA3C
  fx_layer_blending_config_c = 16;
  if (enemy_data[1].ai_var_C) {
    sub_88E987(k);
    uint16 v2 = 4 * g_word_7E9090;
    *((uint8 *)hdma_object_A + k) = g_byte_88EA8B[(uint16)(4 * g_word_7E9090)];
    *((uint8 *)hdma_object_A + k + 1) = g_byte_88EA8B[v2 + 1];
    *((uint8 *)hdma_object_B + k) = g_byte_88EA8B[v2 + 2];
    g_word_7E9090 = (g_word_7E9090 + 1) & 0xF;
  } else {
    int v1 = hdma_object_index >> 1;
    hdma_object_instruction_list_pointers[v1] += 2;
    hdma_object_instruction_timers[v1] = 1;
  }
}

void HdmaobjPreInstr_EACB(uint16 k) {  // 0x88EACB
  fx_layer_blending_config_c = 16;
  if (*((uint8 *)hdma_object_A + k + 1) == 64) {
    reg_COLDATA[0] = 32;
    reg_COLDATA[1] = 64;
    reg_COLDATA[2] = 0x80;
    int v1 = k >> 1;
    hdma_object_D[v1] = 0;
    hdma_object_C[v1] = 0;
    for (int i = 510; i >= 0; i -= 2)
      *(uint16 *)((uint8 *)&g_word_7E9100 + (uint16)i) = 255;
    kraid_unk9000 = 0;
    g_word_7E9002 = 0;
    g_word_7E9004 = 0;
    g_word_7E9006 = 0;
    g_word_7E9008 = 0;
    int v3 = hdma_object_index >> 1;
    hdma_object_instruction_list_pointers[v3] += 2;
    hdma_object_instruction_timers[v3] = 1;
  } else {
    sub_88E987(k);
    uint16 v4 = hdma_object_index;
    if (*((uint8 *)hdma_object_A + hdma_object_index) != 32)
      -- *((uint8 *)hdma_object_A + hdma_object_index);
    if (*((uint8 *)hdma_object_A + v4 + 1) != 64)
      -- *((uint8 *)hdma_object_A + v4 + 1);
    if (*((uint8 *)hdma_object_B + v4) != 0x80)
      -- *((uint8 *)hdma_object_B + v4);
  }
}

void SpawnTitleScreenGradientObjs(void) {  // 0x88EB58
  static const SpawnHdmaObject_Args unk_88EB6B = { 0x00, 0x31, 0xeb85 };
  static const SpawnHdmaObject_Args unk_88EB63 = { 0x00, 0x32, 0xeb73 };
  SpawnHdmaObject(0x88, &unk_88EB63);
  SpawnHdmaObject(0x88, &unk_88EB6B);
}

const uint8 *HdmaobjInsr_ConfigTitleSequenceGradientHDMA(uint16 k, const uint8 *hdp) {  // 0x88EB9F
  reg_CGWSEL = 0;
  ConfigureTitleSequenceGradientHDMA();
  return hdp;
}

void HdmaobjPreInstr_Backdrop_TitleSequenceGradient(uint16 k) {  // 0x88EBB0
  ConfigureTitleSequenceGradientHDMA();
  if (cinematic_function == FUNC16(CinematicFunctionOpening)) {
    int v1 = hdma_object_index >> 1;
    hdma_object_instruction_list_pointers[v1] += 2;
    hdma_object_instruction_timers[v1] = 1;
  }
}

void HdmaobjPreInstr_ColorMathControlB_TitleGradient(uint16 k) {  // 0x88EBD2
  if (cinematic_function == FUNC16(CinematicFunctionOpening)) {
    int v1 = hdma_object_index >> 1;
    hdma_object_instruction_list_pointers[v1] += 2;
    hdma_object_instruction_timers[v1] = 1;
  }
}

void SpawnIntroCutsceneCrossfadeHdma(void) {  // 0x88EBF0
  static const SpawnHdmaObject_Args unk_88EBFB = { 0x00, 0x31, 0xec03 };
  SpawnHdmaObject(0x88, &unk_88EBFB);
}

void HdmaobjPreInstr_IntroCutsceneCrossfade(uint16 k) {  // 0x88EC1D
  if (enemy_projectile_x_pos[0] == 1) {
    int v1 = hdma_object_index >> 1;
    hdma_object_instruction_list_pointers[v1] += 2;
    hdma_object_instruction_timers[v1] = 1;
  }
}

void CinematicFunction_Intro_Func133(void) {  // 0x88EC3B
  unsigned int v0; // kr00_4
  unsigned int v1; // kr04_4

  g_word_7E0D9C = 1;
  *(uint16 *)&g_byte_7E0D9E = 0x4000;
  loop_counter_transfer_enemies_to_vram = 8;
  button_config_shoot_x_saved = 192;
  button_config_itemcancel_y_saved = 192;
  v0 = 12621824;
  *(uint16 *)((uint8 *)&button_config_jump_a_saved + 1) = HIWORD(v0);
  *(uint16 *)((uint8 *)&button_config_shoot_x_saved + 1) = v0;
  button_config_run_b_saved = -26496;
  v1 = 12621824;
  *(uint16 *)((uint8 *)&button_config_itemswitch_saved + 1) = HIWORD(v1);
  *(uint16 *)((uint8 *)&button_config_itemcancel_y_saved + 1) = v1;
  button_config_aim_down_L_saved = -26496;
  button_config_aim_up_R_saved = 0;
  static const SpawnHdmaObject_Args unk_88EC82 = { 0x42, 0x11, 0xec8a };
  SpawnHdmaObject(0x88, &unk_88EC82);
}

const uint8 *HdmaobjInstr_EC9F_ClearVars(uint16 k, const uint8 *hdp) {  // 0x88EC9F
  int v2 = k >> 1;
  hdma_object_A[v2] = -2;
  hdma_object_B[v2] = 1;
  hdma_object_C[v2] = 0;
  hdma_object_D[v2] = 0;
  return hdp;
}

void HdmaobjPreInstr_ECB6(uint16 k) {  // 0x88ECB6
  int16 v7;
  uint16 v8;

  if (g_word_7E0D9C) {
    uint16 r28 = 4;
    int n = 128;
    int v3 = k >> 1;
    hdma_object_A[v3] = (2 * loop_counter_transfer_enemies_to_vram + hdma_object_A[v3]) & 0x1FF;
    uint16 r20 = hdma_object_A[v3];
    uint16 v4 = 0;
    do {
      uint16 v10 = v4;
      uint16 v5 = r20;
      int v6 = r20 >> 1;
      v7 = kSinCosTable8bit_Sext[v6 + 64];
      if (v7 < 0) {
        uint16 r18 = -v7;
        uint16 r22 = Mult8x8(-(int8)v7, g_byte_7E0D9E) >> 8;
        r22 += Mult8x8(HIBYTE(r18), g_byte_7E0D9E);
        uint16 r24 = Mult8x8(r18, g_byte_7E0D9F);
        uint8 mult = Mult8x8(HIBYTE(r18), g_byte_7E0D9F);
        r22 += r24;
        r18 = ((r22 + (mult << 8)) & 0xFF00) >> 8;
        r20 = (r28 + v5) & 0x1FF;
        v8 = v10;
        hdma_table_1[v10 >> 1] = reg_BG3HOFS - r18;
      } else {
        uint16 r18 = kSinCosTable8bit_Sext[v6 + 64];
        uint16 r22 = Mult8x8(v7, g_byte_7E0D9E) >> 8;
        r22 += Mult8x8(HIBYTE(r18), g_byte_7E0D9E);
        uint16 r24 = Mult8x8(r18, g_byte_7E0D9F);
        uint8 mult = Mult8x8(HIBYTE(r18), g_byte_7E0D9F);
        r22 += r24;
        r18 = ((r22 + (mult << 8)) & 0xFF00) >> 8;
        r20 = (r28 + v5) & 0x1FF;
        v8 = v10;
        hdma_table_1[v10 >> 1] = r18 + reg_BG3HOFS;
      }
      v4 = v8 + 2;
    } while ((int16)(v4 - n) < 0);
    for (int i = 126; i >= 0; i -= 2)
      hdma_table_1[(i >> 1) + 64] = reg_BG3HOFS + reg_BG3HOFS - hdma_table_1[i >> 1];
  } else {
    int v2 = hdma_object_index >> 1;
    hdma_object_instruction_list_pointers[v2] += 2;
    hdma_object_instruction_timers[v2] = 1;
  }
}

void CallHdmaobjPreInstr(uint32 ea, uint16 k) {
  switch (ea) {
  case fnnullsub_56: return;
  case fnnullsub_293: return;
  case fnnullsub_309: return;
  case fnHdmaobjPreInstr_XraySetup: HdmaobjPreInstr_XraySetup(k); return;
  case fnHdmaobjPreInstr_Xray: HdmaobjPreInstr_Xray(k); return;
  case fnHdmaobjPreInstr_XrayFunc0_NoBeam: HdmaobjPreInstr_XrayFunc0_NoBeam(k); return;
  case fnHdmaobjPreInstr_XrayFunc1_BeamWidening: HdmaobjPreInstr_XrayFunc1_BeamWidening(k); return;
  case fnHdmaobjPreInstr_XrayFunc2_FullBeam: HdmaobjPreInstr_XrayFunc2_FullBeam(k); return;
  case fnHdmaobjPreInstr_XrayFunc3_DeactivateBeam: HdmaobjPreInstr_XrayFunc3_DeactivateBeam(k); return;
  case fnHdmaobjPreInstr_XrayFunc4_DeactivateBeam: HdmaobjPreInstr_XrayFunc4_DeactivateBeam(k); return;
  case fnHdmaobjPreInstr_XrayFunc5_DeactivateBeam: HdmaobjPreInstr_XrayFunc5_DeactivateBeam(k); return;
  case fnHdmaobjPreInstr_PowerBombExplode_SetWindowConf: HdmaobjPreInstr_PowerBombExplode_SetWindowConf(k); return;
  case fnHdmaobjPreInstr_PowerBombExplode_Stage5_Afterglow: HdmaobjPreInstr_PowerBombExplode_Stage5_Afterglow(k); return;
  case fnHdmaobjPreInstr_PowerBombExplode_ExplosionYellow: HdmaobjPreInstr_PowerBombExplode_ExplosionYellow(k); return;
  case fnHdmaobjPreInstr_PowerBombExplode_ExplosionWhite: HdmaobjPreInstr_PowerBombExplode_ExplosionWhite(k); return;
  case fnHdmaobjPreInstr_PowerBombExplode_PreExplosionWhite: HdmaobjPreInstr_PowerBombExplode_PreExplosionWhite(k); return;
  case fnHdmaobjPreInstr_PowerBombExplode_PreExplosionYellow: HdmaobjPreInstr_PowerBombExplode_PreExplosionYellow(k); return;
  case fnHdmaobjPreInstr_CrystalFlash_CustomLayerBlend: HdmaobjPreInstr_CrystalFlash_CustomLayerBlend(k); return;
  case fnHdmaobjPreInstr_CrystalFlash_Stage2_AfterGlow: HdmaobjPreInstr_CrystalFlash_Stage2_AfterGlow(k); return;
  case fnHdmaobjPreInstr_CrystalFlash_Stage1_Explosion: HdmaobjPreInstr_CrystalFlash_Stage1_Explosion(k); return;
  case fnHdmaobjPreInstr_FxType22_BG3Yscroll: HdmaobjPreInstr_FxType22_BG3Yscroll(k); return;
  case fnHdmaobjPreInstr_BG3Xscroll: HdmaobjPreInstr_BG3Xscroll(k); return;
  case fnHdmaobjPreInstr_SkyLandBG2Xscroll: HdmaobjPreInstr_SkyLandBG2Xscroll(k); return;
  case fnHdmaobjPreInstr_SkyLandBG2Xscroll2: HdmaobjPreInstr_SkyLandBG2Xscroll2(k); return;
  case fnHdmaobjPreInstr_SkyLandBG2XscrollInner: HdmaobjPreInstr_SkyLandBG2XscrollInner(k); return;
  case fnHdmaobjPreInstr_FirefleaBG3XScroll: HdmaobjPreInstr_FirefleaBG3XScroll(k); return;
  case fnHdmaobjPreInstr_LavaAcidBG3YScroll: HdmaobjPreInstr_LavaAcidBG3YScroll(k); return;
  case fnHdmaobjPreInstr_LavaAcidBG2YScroll: HdmaobjPreInstr_LavaAcidBG2YScroll(k); return;
  case fnHdmaobjPreInstr_WaterBG3XScroll: HdmaobjPreInstr_WaterBG3XScroll(k); return;
  case fnHdmaobjPreInstr_WaterBG2XScroll: HdmaobjPreInstr_WaterBG2XScroll(k); return;
  case fnHdmaobjPreInstr_WaterBG2XScroll_Func2: HdmaobjPreInstr_WaterBG2XScroll_Func2(k); return;
  case fnHdmaobjPreInstr_WaterBG2XScroll_Func1: HdmaobjPreInstr_WaterBG2XScroll_Func1(k); return;
  case fnHdmaobjPreInstr_RainBg3Scroll: HdmaobjPreInstr_RainBg3Scroll(k); return;
  case fnHdmaobjPreInstr_SporesBG3Xscroll: HdmaobjPreInstr_SporesBG3Xscroll(k); return;
  case fnHdmaobjPreInstr_FogBG3Scroll: HdmaobjPreInstr_FogBG3Scroll(k); return;
  case fnHdmaobjPreInstr_CheckLotsOfEventsHappened: HdmaobjPreInstr_CheckLotsOfEventsHappened(k); return;
  case fnHdmaobjPreInstr_DC23: HdmaobjPreInstr_DC23(k); return;
  case fnHdmaobjPreInstr_DC69: HdmaobjPreInstr_DC69(k); return;
  case fnHdmaobjPreInstr_DCBA: HdmaobjPreInstr_DCBA(k); return;
  case fnHdmaobjPreInstr_BombTorizoHazeColorMathBgColor: HdmaobjPreInstr_BombTorizoHazeColorMathBgColor(k); return;
  case fnHdmaobjPreInstr_HazeColorMathSubscreen_CeresRidleyAlive: HdmaobjPreInstr_HazeColorMathSubscreen_CeresRidleyAlive(k); return;
  case fnHdmaobjPreInstr_HazeColorMathSubscreen_CeresRidleyDead: HdmaobjPreInstr_HazeColorMathSubscreen_CeresRidleyDead(k); return;
  case fnHdmaobjPreInstr_HazeColorMathSubscreen_FadingIn: HdmaobjPreInstr_HazeColorMathSubscreen_FadingIn(k); return;
  case fnHdmaobjPreInstr_HazeColorMathSubscreen_FadedIn: HdmaobjPreInstr_HazeColorMathSubscreen_FadedIn(k); return;
  case fnHdmaobjPreInstr_HazeColorMathSubscreen_FadingOut: HdmaobjPreInstr_HazeColorMathSubscreen_FadingOut(k); return;
  case fnHdmaobjPreInstr_DF94: HdmaobjPreInstr_DF94(k); return;
  case fnHdmaobjPreInstr_VariaSuitPickup: HdmaobjPreInstr_VariaSuitPickup(k); return;
  case fnHdmaobjPreInstr_GravitySuitPickup: HdmaobjPreInstr_GravitySuitPickup(k); return;
  case fnHdmaobjPreInstr_E449: HdmaobjPreInstr_E449(k); return;
  case fnHdmaobjPreInstr_E567: HdmaobjPreInstr_E567(k); return;
  case fnHdmaobjPreInstr_E7BC: HdmaobjPreInstr_E7BC(k); return;
  case fnHdmaobjPreInstr_E9E6: HdmaobjPreInstr_E9E6(k); return;
  case fnHdmaobjPreInstr_EA3C: HdmaobjPreInstr_EA3C(k); return;
  case fnHdmaobjPreInstr_EACB: HdmaobjPreInstr_EACB(k); return;
  case fnHdmaobjPreInstr_Backdrop_TitleSequenceGradient: HdmaobjPreInstr_Backdrop_TitleSequenceGradient(k); return;
  case fnHdmaobjPreInstr_ColorMathControlB_TitleGradient: HdmaobjPreInstr_ColorMathControlB_TitleGradient(k); return;
  case fnHdmaobjPreInstr_IntroCutsceneCrossfade: HdmaobjPreInstr_IntroCutsceneCrossfade(k); return;
  case fnnullsub_357: return;
  case fnHdmaobjPreInstr_ECB6: HdmaobjPreInstr_ECB6(k); return;
  default: Unreachable();
  }
}

const uint8 *CallHdmaobjInstr(uint32 ea, uint16 k, const uint8 *j) {
  switch (ea) {
  case fnnullsub_112: return j;
  case fnHdmaobjInstr_Delete: return HdmaobjInstr_Delete(k, j);
  case fnHdmaobjInstr_SetPreInstr: return HdmaobjInstr_SetPreInstr(k, j);
  case fnHdmaobjInstr_ClearPreInstr: return HdmaobjInstr_ClearPreInstr(k, j);
  case fnHdmaobjInstr_CallFarFunc: return HdmaobjInstr_CallFarFunc(k, j);
  case fnHdmaobjInstr_Goto: return HdmaobjInstr_Goto(k, j);
  case fnHdmaobjInstr_GotoRel: return HdmaobjInstr_GotoRel(k, j);
  case fnHdmaobjInstr_DecrementAndGoto: return HdmaobjInstr_DecrementAndGoto(k, j);
  case fnHdmaobjInstr_DecrementAndGotoRel: return HdmaobjInstr_DecrementAndGotoRel(k, j);
  case fnHdmaobjInstr_SetTimer: return HdmaobjInstr_SetTimer(k, j);
  case fnHdmaobjInstr_SetHdmaControl: return HdmaobjInstr_SetHdmaControl(k, j);
  case fnHdmaobjInstr_SetHdmaTarget: return HdmaobjInstr_SetHdmaTarget(k, j);
  case fnHdmaobjInstr_SetHdmaTablePtr: return HdmaobjInstr_SetHdmaTablePtr(k, j);
  case fnHdmaobjInstr_SetHdmaTableBank: return HdmaobjInstr_SetHdmaTableBank(k, j);
  case fnHdmaobjInstr_SetIndirectHdmaDataBank: return HdmaobjInstr_SetIndirectHdmaDataBank(k, j);
  case fnHdmaobjInstr_Sleep: return HdmaobjInstr_Sleep(k, j);
  case fnHdmaobjInstr_SetFlagB: return HdmaobjInstr_SetFlagB(k, j);
  case fnHdmaobjInstr_SetFlagB_Copy: return HdmaobjInstr_SetFlagB_Copy(k, j);
  case fnHdmaobjInstr_SetFlagB_Copy2: return HdmaobjInstr_SetFlagB_Copy2(k, j);
  case fnHdmaobjInstr_SetFlagB_Copy3: return HdmaobjInstr_SetFlagB_Copy3(k, j);
  case fnHdmaobjInstr_SetVideoMode1: return HdmaobjInstr_SetVideoMode1(k, j);
  case fnHdmaobjInstr_1938_RandomNumber: return HdmaobjInstr_1938_RandomNumber(k, j);
  case fnHdmaobjInstr_GotoIfEventHappened: return HdmaobjInstr_GotoIfEventHappened(k, j);
  case fnHdmaobjInstr_E4BD: return HdmaobjInstr_E4BD(k, j);
  case fnHdmaobjInstr_InitMorphBallEyeBeamHdma: return HdmaobjInstr_InitMorphBallEyeBeamHdma(k, j);
  case fnHdmaobjInstr_EC9F_ClearVars: return HdmaobjInstr_EC9F_ClearVars(k, j);
  case fnHdmaobjInstr_B3A9: return HdmaobjInstr_B3A9(k, j);
  case fnHdmaobjInsr_ConfigTitleSequenceGradientHDMA: return HdmaobjInsr_ConfigTitleSequenceGradientHDMA(k, j);
  case fnsub_88D916: sub_88D916(); return j;
  default: Unreachable(); return NULL;
  }
}
