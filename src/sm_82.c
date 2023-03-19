// Top level main game routines

#include "sm_rtl.h"
#include "ida_types.h"
#include "variables.h"
#include "funcs.h"
#include "enemy_types.h"


#define kDemoRoomData ((uint16*)RomFixedPtr(0x82876c))
#define kPauseScreenSpriteAnimationData_0 (*(PauseScreenSpriteAnimationData*)RomFixedPtr(0x82c0b2))
#define kPauseScreenSpriteAnimationData_1 (*(PauseScreenSpriteAnimationData*)RomFixedPtr(0x82c0c4))
#define kPauseScreenSpriteAnimationData_2 (*(PauseScreenSpriteAnimationData*)RomFixedPtr(0x82c0d6))
#define kPauseScreenSpriteAnimationData_3 (*(PauseScreenSpriteAnimationData*)RomFixedPtr(0x82c0e8))
#define kPAuseSpritePaletteIndexValues ((uint16*)RomFixedPtr(0x82c0fa))
#define kPausePtsToAnimationSpritemapBaseIds ((uint16*)RomFixedPtr(0x82c1e4))
#define kInitialPalette ((uint16*)RomFixedPtr(0x9a8000))
#define kDemoRoomData ((uint16*)RomFixedPtr(0x82876c))
#define kPauseScreenPalettes ((uint16*)RomFixedPtr(0xb6f000))
#define kPauseLrHighlightAnimData ((uint8*)RomFixedPtr(0x82c10c))
#define kPauseAreaLabelTilemap ((uint16*)RomFixedPtr(0x82965f))
#define kPauseMenuMapTilemaps ((LongPtr*)RomFixedPtr(0x82964a))
#define kPauseMenuMapData ((uint16*)RomFixedPtr(0x829717))
#define kEquipmentTilemaps_Tanks ((uint16*)RomFixedPtr(0x82c088))
#define kEquipmentTilemaps_Weapons ((uint16*)RomFixedPtr(0x82c08c))
#define kEquipmentTilemaps_Suits ((uint16*)RomFixedPtr(0x82c096))
#define kEquipmentTilemaps_Boots ((uint16*)RomFixedPtr(0x82c0a2))
#define kHyperBeamWeaponsTilemaps ((uint16*)RomFixedPtr(0x82c0a8))
#define kEquipmentBitmasks_Weapons ((uint16*)RomFixedPtr(0x82c04c))
#define kEquipmentBitmasks_Suits ((uint16*)RomFixedPtr(0x82c056))
#define kEquipmentBitmasks_Boots ((uint16*)RomFixedPtr(0x82c062))
#define kPauseAnimatedPalette ((uint16*)RomFixedPtr(0x82a987))
#define kPauseReserveTankAnimationData ((uint8*)RomFixedPtr(0x82c165))
#define kEquipmentTilemapOffs_Tanks ((uint16*)RomFixedPtr(0x82c068))
#define kEquipmentTilemapOffs_Weapons ((uint16*)RomFixedPtr(0x82c06c))
#define kEquipmentTilemapOffs_Suits ((uint16*)RomFixedPtr(0x82c076))
#define kEquipmentTilemapOffs_Boots ((uint16*)RomFixedPtr(0x82c082))
#define kEquipmentTilemaps_Tanks ((uint16*)RomFixedPtr(0x82c088))
#define kEquipmentTilemaps_Weapons ((uint16*)RomFixedPtr(0x82c08c))
#define kEquipmentTilemaps_Suits ((uint16*)RomFixedPtr(0x82c096))
#define kEquipmentTilemaps_Boots ((uint16*)RomFixedPtr(0x82c0a2))
#define kEquipmentScreenWireframeCmp ((uint16*)RomFixedPtr(0x82b257))
#define kEquipmentScreenWireframePtrs ((uint16*)RomFixedPtr(0x82b25f))
#define kEquipmentScreenPtrsToItemXYpos ((uint16*)RomFixedPtr(0x82c18e))
#define kEquipmentPtrsToRamTilemapOffsets ((uint16*)RomFixedPtr(0x82c02c))
#define kEquipmentPtrsToBitmasks ((uint16*)RomFixedPtr(0x82c034))
#define kEquipmentPtrsToBitsets ((uint16*)RomFixedPtr(0x82c03c))
#define kEquipmentPtrsToEquipmentTilemaps ((uint16*)RomFixedPtr(0x82c044))
#define kMapIconDataPointers ((MapIconDataPointers*)RomFixedPtr(0x82c7cb))
#define g_stru_82B9A0 ((MapScrollArrowData*)RomFixedPtr(0x82b9a0))
#define g_stru_82B9AA ((MapScrollArrowData*)RomFixedPtr(0x82b9aa))
#define g_stru_82B9B4 ((MapScrollArrowData*)RomFixedPtr(0x82b9b4))
#define g_stru_82B9BE (*(MapScrollArrowData*)RomFixedPtr(0x82b9be))
#define file_copy_arrow_stuff ((FileCopyArrowStuff*)RomFixedPtr(0x82bb0c))
#define kMapElevatorDests ((uint16*)RomFixedPtr(0x82c74d))
#define kStateHeaderGraphicsSets ((uint16*)RomFixedPtr(0x8fe7a7))
#define kCommonSpritesPalette1 ((uint16*)RomFixedPtr(0x9afc00))
#define kMenuPalettes ((uint16*)RomFixedPtr(0x8ee400))
#define kOptionsMenuSpecialPtrs ((uint16*)RomFixedPtr(0x82f0ae))
#define off_82F2ED ((uint16*)RomFixedPtr(0x82f2ed))
#define off_82F54A ((uint16*)RomFixedPtr(0x82f54a))
#define g_word_82F639 ((uint16*)RomFixedPtr(0x82f639))
#define g_off_82F647 ((uint16*)RomFixedPtr(0x82f647))




void CallDemoRoomDataFunc(uint32 ea) {
  switch (ea) {
  case fnDemoRoom_ChargeBeamRoomScroll21: DemoRoom_ChargeBeamRoomScroll21(); return;
  case fnnullsub_291: return;
  case fnDemoRoom_SetBG2TilemapBase: DemoRoom_SetBG2TilemapBase(); return;
  case fnDemoRoom_SetKraidFunctionTimer: DemoRoom_SetKraidFunctionTimer(); return;
  case fnDemoRoom_SetBrinstarBossBits: DemoRoom_SetBrinstarBossBits(); return;
  default: Unreachable();
  }
}

CoroutineRet InitAndLoadGameData_Async(void) {  // 0x828000
  int16 v0;
  int16 v2;
  int16 v4;
  int16 v7;

  COROUTINE_BEGIN(coroutine_state_1, 0)

  if (game_state == kGameState_40_TransitionToDemo) {
    InitIoForGameplay();
    LoadStdBG3andSpriteTilesClearTilemaps();
    LoadInitialPalette();
    Samus_Initialize();
    LoadDemoRoomData();
  } else {
    if (loading_game_state != kLoadingGameState_5_Main) {
      if (loading_game_state == kLoadingGameState_1F_StartingAtCeres) {
        area_index = 6;
        load_station_index = 0;
        ClearTimerRam();
      } else if (loading_game_state == kLoadingGameState_22_EscapingCeres) {
        area_index = 0;
        load_station_index = 18;
        LoadMirrorOfExploredMapTiles();
      }
    }
    InitIoForGameplay();
    LoadStdBG3andSpriteTilesClearTilemaps();
    LoadInitialPalette();
    Samus_Initialize();
    LoadFromLoadStation();
  }
  COROUTINE_AWAIT(1, StartGameplay_Async());
  InitializeHud();
  //  nullsub_4();
  {
    v0 = 32;
    uint16 v1 = 0;
    do {
      target_palettes[(v1 >> 1) + 192] = palette_buffer[(v1 >> 1) + 192];
      v1 += 2;
      v0 -= 2;
    } while (v0);
  }
  screen_fade_delay = 1;
  screen_fade_counter = 1;
  EnableNMI();
  EnableEnemyProjectiles();
  EnablePLMs();
  EnablePaletteFx();
  EnableHdmaObjects();
  EnableAnimtiles();
  SetLiquidPhysicsType();
  if (game_state == kGameState_40_TransitionToDemo) {
    loop_counter_transfer_enemies_to_vram = 6;
    do {
      TransferEnemyTilesToVramAndInit();
      COROUTINE_AWAIT(2, WaitForNMI_Async());
      --loop_counter_transfer_enemies_to_vram;
    } while ((loop_counter_transfer_enemies_to_vram & 0x8000u) == 0);
    R18_ = 18 * (demo_scene - 1);
    R18_ = get_DemoRoomData(R18_ + kDemoRoomData[demo_set])->demo_code_ptr;
    CallDemoRoomDataFunc(R18_ | 0x820000);
    ++game_state;
    v7 = 512;
    uint16 v8 = 0;
    do {
      palette_buffer[v8 >> 1] = target_palettes[v8 >> 1];
      v8 += 2;
      v7 -= 2;
    } while (v7);
  } else if (loading_game_state == kLoadingGameState_22_EscapingCeres) {
    QueueMusic_Delayed8(5u);
    loop_counter_transfer_enemies_to_vram = 15;
    do {
      TransferEnemyTilesToVramAndInit();
      COROUTINE_AWAIT(3, WaitForNMI_Async());
      --loop_counter_transfer_enemies_to_vram;
    } while ((loop_counter_transfer_enemies_to_vram & 0x8000u) == 0);
    ++game_state;
    v2 = 512;
    uint16 v3 = 0;
    do {
      palette_buffer[v3 >> 1] = target_palettes[v3 >> 1];
      v3 += 2;
      v2 -= 2;
    } while (v2);
  } else {
    loop_counter_transfer_enemies_to_vram = 6;
    do {
      TransferEnemyTilesToVramAndInit();
      COROUTINE_AWAIT(4, WaitForNMI_Async());
      --loop_counter_transfer_enemies_to_vram;
    } while ((loop_counter_transfer_enemies_to_vram & 0x8000u) == 0);
    game_state = kGameState_7_MainGameplayFadeIn;
    v4 = 512;
    uint16 v5 = 0;
    do {
      palette_buffer[v5 >> 1] = target_palettes[v5 >> 1];
      v5 += 2;
      v4 -= 2;
    } while (v4);
    if (loading_game_state == kLoadingGameState_1F_StartingAtCeres) {
      palette_buffer[223] = 0;
      CallSomeSamusCode(8u);
    } else {
      CallSomeSamusCode(9u);
    }
  }

  COROUTINE_END(0);
}

void InitIoForGameplay(void) {  // 0x82819B
  InitCpuForGameplay();
  InitPpuForGameplay();
}

void InitCpuForGameplay(void) {  // 0x8281A4
  WriteReg(NMITIMEN, 1u);
  reg_NMITIMEN = 1;
  WriteReg(WRIO, 0);
  WriteReg(WRMPYA, 0);
  WriteReg(WRMPYB, 0);
  WriteReg(WRDIVL, 0);
  WriteReg(WRDIVH, 0);
  WriteReg(WRDIVB, 0);
  WriteReg(HTIMEL, 0);
  WriteReg(HTIMEH, 0);
  WriteReg(VTIMEL, 0);
  WriteReg(VTIMEH, 0);
  WriteReg(MDMAEN, 0);
  WriteReg(HDMAEN, 0);
  reg_HDMAEN = 0;
  WriteReg(MEMSEL, 1u);
  reg_MEMSEL = 1;
}

void InitPpuForGameplay(void) {  // 0x8281DD
  WriteReg(INIDISP, 0x80);
  reg_INIDISP = 0x80;
  WriteReg(OBSEL, 3u);
  reg_OBSEL = 3;
  WriteReg(OAMADDL, 0);
  LOBYTE(reg_OAMaddr_UNUSED) = 0;
  WriteReg(OAMADDH, 0x80);
  HIBYTE(reg_OAMaddr_UNUSED) = 0x80;
  WriteReg(OAMDATA, 0);
  WriteReg(OAMDATA, 0);
  WriteReg(BGMODE, 9u);
  reg_BGMODE = 9;
  WriteReg(MOSAIC, 0);
  reg_MOSAIC = 0;
  reg_BG12NBA = 0;
  WriteReg(BG12NBA, 0);
  reg_BG34NBA = 4;
  WriteReg(BG34NBA, 4u);
  reg_BG1SC = 81;
  WriteReg(BG1SC, 0x51);
  reg_BG2SC = 73;
  WriteReg(BG2SC, 0x49);
  reg_BG3SC = 90;
  WriteReg(BG3SC, 0x5A);
  reg_BG4SC = 0;
  WriteReg(BG4SC, 0);
  WriteReg(VMAIN, 0);
  WriteReg(W12SEL, 0);
  reg_W12SEL = 0;
  WriteReg(W12SEL, 0);
  reg_W12SEL = 0;
  WriteReg(W34SEL, 0);
  reg_W34SEL = 0;
  WriteReg(WOBJSEL, 0);
  reg_WOBJSEL = 0;
  WriteReg(WH0, 0);
  reg_WH0 = 0;
  WriteReg(WH1, 0);
  reg_WH1 = 0;
  WriteReg(WH2, 0);
  reg_WH2 = 0;
  WriteReg(WH3, 0);
  reg_WH3 = 0;
  WriteReg(WBGLOG, 0);
  reg_WBGLOG = 0;
  WriteReg(WOBJLOG, 0);
  reg_WOBJLOG = 0;
  WriteReg(TM, 0x17);
  reg_TM = 23;
  WriteReg(TMW, 0);
  reg_TMW = 0;
  WriteReg(TS, 0);
  reg_TS = 0;
  WriteReg(TSW, 0);
  reg_TSW = 0;
  WriteReg(CGWSEL, 0);
  next_gameplay_CGWSEL = 0;
  WriteReg(CGADSUB, 0);
  next_gameplay_CGADSUB = 0;
  WriteReg(COLDATA, 0xE0);
  WriteReg(SETINI, 0);
  reg_SETINI = 0;
  oam_next_ptr = 0;
  memset7E((uint16*)&ram3000, 0, 0x7FE);
  memset7E((uint16 *)&ram4000, 0x6F, 0x7FE);
  memset7E(ram4000.bg2_tilemap, 0x2C0F, 0xFE);
}


void LoadInitialPalette(void) {  // 0x8282C5
  int16 v0;

  v0 = 512;
  uint16 v1 = 0;
  do {
    palette_buffer[v1 >> 1] = kInitialPalette[v1 >> 1];
    v1 += 2;
    v0 -= 2;
  } while (v0);
}

void LoadStdBG3andSpriteTilesClearTilemaps(void) {  // 0x8282E2
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x40);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8282F8 = { 1, 1, 0x18, LONGPTR(0x9ab200), 0x2000 };
  SetupDmaTransfer(&unk_8282F8);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x60);
  WriteReg(VMAIN, 0x80);
  // Bug fix: Asm code does DMA from RAM here.
  static const StartDmaCopy unk_828318 = { 1, 1, 0x18, LONGPTR(0x9ad200), 0x2e00 };
  SetupDmaTransfer(&unk_828318);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x50);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_828338 = { 1, 1, 0x18, LONGPTR(0x7e4000), 0x1000 };
  SetupDmaTransfer(&unk_828338);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x58);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_828358 = { 1, 1, 0x18, LONGPTR(0x7e4000), 0x0800 };
  SetupDmaTransfer(&unk_828358);
  WriteReg(MDMAEN, 2u);
}

CoroutineRet GameState_32_MadeItToCeresElevator(void) {  // 0x828367
  if (timer_status)
    DrawTimer();
  COROUTINE_AWAIT_ONLY(GameState_8_MainGameplay());
  bool v0 = (--reached_ceres_elevator_fade_timer & 0x8000u) != 0;
  if (!reached_ceres_elevator_fade_timer || v0) {
    ++game_state;
    screen_fade_delay = 0;
    screen_fade_counter = 0;
  }
  return kCoroutineNone;
}

CoroutineRet GameState_33_BlackoutFromCeres(void) {  // 0x828388
  if (timer_status)
    DrawTimer();
  COROUTINE_AWAIT_ONLY(GameState_8_MainGameplay());
  HandleFadeOut();
  if (reg_INIDISP == 0x80) {
    EnableNMI();
    WaitUntilEndOfVblankAndClearHdma();
    DisableIrqInterrupts();
    fx_layer_blending_config_a = 0;
    cur_irq_handler = 0;
    next_gameplay_CGWSEL = 0;
    next_gameplay_CGADSUB = 0;
    reg_TM = 16;
    reg_TS = 0;
    reg_TMW = 0;
    reg_TSW = 0;
    reg_BGMODE = 9;
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    loading_game_state = kLoadingGameState_22_EscapingCeres;
    game_state = 34;
    SaveToSram(selected_save_slot);
    cinematic_function = FUNC16(CinematicFunctionBlackoutFromCeres);
    ceres_status = 0;
    timer_status = 0;
    QueueMusic_Delayed8(0);
    QueueSfx1_Max15(2u);
    QueueSfx2_Max15(0x71u);
    QueueSfx3_Max15(1u);
  }
  return kCoroutineNone;
}

CoroutineRet GameState_35_TimeUp(void) {  // 0x828411
  COROUTINE_AWAIT_ONLY(GameState_8_MainGameplay());
  palette_change_denom = 8;
  if (AdvancePaletteFadeForAllPalettes()) {
    game_state = kGameState_36_WhitingOutFromTimeUp;
    screen_fade_delay = 0;
    screen_fade_counter = 0;
  }
  return kCoroutineNone;
}

CoroutineRet GameState_36_WhitingOutFromTimeUp(void) {  // 0x828431
  HandleFadeOut();
  if (reg_INIDISP == 0x80) {
    EnableNMI();
    WaitUntilEndOfVblankAndClearHdma();
    DisableIrqInterrupts();
    fx_layer_blending_config_a = 0;
    cur_irq_handler = 0;
    next_gameplay_CGWSEL = 0;
    next_gameplay_CGADSUB = 0;
    reg_TM = 16;
    reg_TS = 0;
    reg_TMW = 0;
    reg_TSW = 0;
    reg_BGMODE = 9;
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    ceres_status = 0;
    timer_status = 0;
    QueueSfx1_Max15(2u);
    QueueSfx2_Max15(0x71u);
    QueueSfx3_Max15(1u);
    if (CheckEventHappened(0xEu)) {
      game_options_screen_index = 0;
      menu_index = 0;
      for (int i = 254; i >= 0; i -= 2)
        enemy_projectile_y_subpos[(i >> 1) + 15] = 0;
      game_state = kGameState_25_SamusNoHealth;
    } else {
      game_state = kGameState_37_CeresGoesBoomWithSamus;
      cinematic_function = FUNC16(CinematicFunctionBlackoutFromCeres);
    }
  }
  return kCoroutineNone;
}

CoroutineRet GameState_38_SamusEscapesFromZebes(void) {  // 0x8284BD
  COROUTINE_AWAIT_ONLY(GameState_8_MainGameplay());
  HandleFadeOut();
  if (reg_INIDISP == 0x80) {
    EnableNMI();
    WaitUntilEndOfVblankAndClearHdma();
    DisableIrqInterrupts();
    fx_layer_blending_config_a = 0;
    next_gameplay_CGWSEL = 0;
    next_gameplay_CGADSUB = 0;
    reg_TM = 16;
    reg_TS = 0;
    reg_TMW = 0;
    reg_TSW = 0;
    reg_BGMODE = 9;
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    game_state = kGameState_39_EndingAndCredits;
    cinematic_function = FUNC16(CinematicFunctionEscapeFromCebes);
    timer_status = 0;
    QueueMusic_Delayed8(0);
    QueueSfx1_Max15(2u);
    QueueSfx2_Max15(0x71u);
    QueueSfx3_Max15(1u);
  }
  return kCoroutineNone;
}

CoroutineRet GameState_41_TransitionToDemo(void) {  // 0x82852D
  COROUTINE_AWAIT_ONLY(GameState_8_MainGameplay());
  HdmaObjectHandler();
  ++game_state;
  reg_INIDISP = 15;
  return kCoroutineNone;
}

CoroutineRet GameState_42_PlayingDemo_Async(void) {  // 0x828548
  COROUTINE_BEGIN(coroutine_state_3, 0)
//  assert(0);
  COROUTINE_AWAIT(2, GameState_8_MainGameplay());
  if (joypad1_newkeys) {
    substate = 1;
    goto LABEL_10;
  }
  demo_timer--;
  if (!demo_timer || (demo_timer & 0x8000u) != 0) {
    substate = 0;
    my_counter = 90;
    while (1) {
      COROUTINE_AWAIT(1, WaitForNMI_Async());
      if (joypad1_newkeys)
        break;
      if (!--my_counter)
        goto LABEL_10;
    }
    substate = 1;

LABEL_10:
    ++game_state;
    debug_disable_sounds = 0;
    reg_INIDISP = 0x80;
    screen_fade_delay = 1;
    screen_fade_counter = 1;
  }
  COROUTINE_END(0);
}

CoroutineRet GameState_43_TransitionFromDemo(void) {  // 0x828593
  if (substate != 1)
    CheckForNextDemo();
  EnableNMI();
  ++game_state;
  screen_fade_delay = 0;
  screen_fade_counter = 0;
  WaitUntilEndOfVblankAndClearHdma();
  DisableIrqInterrupts();
  fx_layer_blending_config_a = 0;
  cur_irq_handler = 0;
  irqhandler_next_handler = 0;
  DisablePaletteFx();
  ClearPaletteFXObjects();
  for (int i = 656; i >= 0; i -= 2)
    *(uint16 *)((char *)&enemy_projectile_enable_flag + i) = 0;
  for (int j = 538; j >= 0; j -= 2)
    *(uint16 *)((char *)&hud_item_tilemap_palette_bits + j) = 0;
  next_gameplay_CGWSEL = 0;
  next_gameplay_CGADSUB = 0;
  reg_TM = 16;
  reg_TS = 0;
  reg_TMW = 0;
  reg_TSW = 0;
  return kCoroutineNone;
}

CoroutineRet GameState_44_TransitionFromDemo(void) {  // 0x8285FB
  game_state = kGameState_1_OpeningCinematic;
  if ((substate & 0x8000u) != 0) {
    game_state = kGameState_40_TransitionToDemo;
  } else if (substate) {
    LoadTitleSequenceGraphics();
    enemy_projectile_x_pos[4] = 2;
    cinematic_function = FUNC16(CinematicFunctionNone);
  } else {
    QueueMusic_Delayed8(0);
    debug_disable_sounds = 0;
    cinematic_function = FUNC16(CinematicFunctionOpening);
  }
  return kCoroutineNone;
}


void CheckForNextDemo(void) {  // 0x828637
  R18_ = 2 * (demo_scene + 8 * demo_scene);
  if (*(uint16 *)RomPtr_82(R18_ + kDemoRoomData[demo_set]) == 0xFFFF) {
    substate = 0;
    uint16 v0 = demo_set + 1;
    if ((uint16)(demo_set + 1) >= num_demo_sets)
      v0 = 0;
    demo_set = v0;
    demo_scene = 0;
  } else {
    substate = 0x8000;
  }
}

void LoadDemoRoomData(void) {  // 0x828679
  DemoRoomData *drd;

  door_def_ptr = 0;
  R18_ = 18 * demo_scene;
  drd = get_DemoRoomData(18 * demo_scene + kDemoRoomData[demo_set]);
  room_ptr = drd->room_ptr_;
  door_def_ptr = drd->door_ptr;
  door_bts = drd->door_slot;
  layer1_x_pos = drd->screen_x_pos;
  bg1_x_offset = layer1_x_pos;
  layer1_y_pos = drd->screen_y_pos;
  bg1_y_offset = layer1_y_pos;
  samus_y_pos = layer1_y_pos + drd->samus_x_offs;
  samus_prev_y_pos = samus_y_pos;
  samus_x_pos = drd->samus_y_offs + layer1_x_pos + 128;
  samus_prev_x_pos = samus_x_pos;
  demo_timer = drd->demo_length;
  LOBYTE(area_index) = get_RoomDefHeader(room_ptr)->area_index_;
  reg_BG1HOFS = 0;
  reg_BG1VOFS = 0;
  ++demo_scene;
  uint16 v1 = 0;
  do {
    int v2 = v1 >> 1;
    WORD(room_chozo_bits[v1]) = -1;
    *(uint16 *)&item_bit_array[v1] = -1;
    UNUSED_word_7ED8F0[v2] = -1;
    *(uint16 *)&map_station_byte_array[v1] = -1;
    *(uint16 *)&used_save_stations_and_elevators[v1] = -1;
    *(uint16 *)&used_save_stations_and_elevators[v1 + 8] = -1;
    WORD(opened_door_bit_array[v1]) = 0;
    WORD(events_that_happened[v1]) = 0;
    *(uint16 *)&boss_bits_for_area[v1] = 0;
    v1 += 2;
  } while ((int16)(v1 - 8) < 0);
  do {
    int v3 = v1 >> 1;
    WORD(room_chozo_bits[v1]) = -1;
    *(uint16 *)&item_bit_array[v1] = -1;
    WORD(opened_door_bit_array[v1]) = 0;
    v1 += 2;
  } while ((int16)(v1 - 64) < 0);
  uint16 v4 = 0;
  do {
    explored_map_tiles_saved[v4 >> 1] = 0;
    v4 += 2;
  } while ((int16)(v4 - 1536) < 0);
  samus_max_reserve_health = 0;
  samus_reserve_health = 0;
  reserve_health_mode = 0;
  loading_game_state = kLoadingGameState_0_Intro;
  debug_disable_minimap = 0;
}

void DemoRoom_ChargeBeamRoomScroll21(void) {  // 0x82891A
  scrolls[33] = 0;
}

void DemoRoom_SetBG2TilemapBase(void) {  // 0x828925
  *(uint16 *)&reg_BG2SC = 74;
}

void DemoRoom_SetKraidFunctionTimer(void) {  // 0x82892B
  enemy_data[0].ai_preinstr = 60;
}

void DemoRoom_SetBrinstarBossBits(void) {  // 0x828932
  boss_bits_for_area[1] = 1;
}

static Func_V_Coroutine *const kGameStateFuncs[45] = {
  GameState_0_Reset_Async,
  GameState_1_OpeningCinematic,
  GameState_2_GameOptionsMenu,
  GameState_3_Null,
  GameState_4_FileSelectMenus,
  GameState_5_FileSelectMap,
  InitAndLoadGameData_Async,
  GameState_7_MainGameplayFadeIn,
  GameState_8_MainGameplay,
  GameState_9_HitDoorBlock,
  GameState_10_LoadingNextRoom_Async,
  GameState_11_LoadingNextRoom_Async,
  GameState_12_Pausing_Darkening_Async,
  GameState_13_Pausing_Async,
  GameState_14_Paused_Async,
  GameState_15_Paused_Async,
  GameState_16_Unpausing_Async,
  GameState_17_Unpausing_Async,
  GameState_18_Unpausing,
  GameState_19_SamusNoHealth,
  GameState_20_SamusNoHealth_BlackOut,
  GameState_21_SamusNoHealth,
  GameState_22_SamusNoHealth_Dying,
  GameState_23_SamusNoHealth_Flashing,
  GameState_24_SamusNoHealth_Explosion,
  GameState_25_SamusNoHealth_BlackOut,
  GameState_26_GameOverMenu,
  GameState_27_ReserveTanksAuto,
  GameState_28_Unused,
  GameState_29_DebugGameOverMenu,
  GameState_37_CeresGoesBoomWithSamus,
  InitAndLoadGameData_Async,
  GameState_32_MadeItToCeresElevator,
  GameState_33_BlackoutFromCeres,
  GameState_37_CeresGoesBoomWithSamus,
  GameState_35_TimeUp,
  GameState_36_WhitingOutFromTimeUp,
  GameState_37_CeresGoesBoomWithSamus,
  GameState_38_SamusEscapesFromZebes,
  GameState_39_EndingAndCredits,
  InitAndLoadGameData_Async,
  GameState_41_TransitionToDemo,
  GameState_42_PlayingDemo_Async,
  GameState_43_TransitionFromDemo,
  GameState_44_TransitionFromDemo,
};

CoroutineRet RunOneFrameOfGameInner(void) {
  int st = coroutine_state_0;
  // This uses manual coroutine handling because of the 
  // kGameStateFuncs[game_state]() thing, we need to make
  // sure we resume at the same position even if it changes
  // behind our back.
  if (st >= 1 && st <= 3) {
    COROUTINE_AWAIT_ONLY(Vector_RESET_Async())
  } else if (st >= 10) {
    goto RESUME_AT_SWITCH;
  } else if (st != 0) {
    Die("Incorrect coroutine_state_0");
  }

  // Ensure these are reset to a known state.
  // There was a bug where these would have the wrong value.
  coroutine_state_1 = coroutine_state_2 = coroutine_state_3 = coroutine_state_4 = 0;

  ReadJoypadInputs();
  HdmaObjectHandler();
  NextRandom();
  ClearOamExt();
  oam_next_ptr = 0;
  nmi_copy_samus_halves = 0;
  nmi_copy_samus_top_half_src = 0;
  nmi_copy_samus_bottom_half_src = 0;

  coroutine_state_0 = st = game_state + 10;
RESUME_AT_SWITCH:
  COROUTINE_AWAIT_ONLY(kGameStateFuncs[st - 10]());

  HandleSoundEffects();
  ClearUnusedOam();
  ShowSpareCpu();
  
  if (coroutine_state_1 | coroutine_state_2 | coroutine_state_3 | coroutine_state_4) {
    printf("Coroutine state: %d, %d, %d, %d\n",
      coroutine_state_1, coroutine_state_2, coroutine_state_3, coroutine_state_4);
    Warning("Coroutine State is broken!");
  }

  coroutine_state_0 = 0;
  return kCoroutineNone;
}

void RunOneFrameOfGame(void) {  // 0x828948
  CoroutineRet ret = RunOneFrameOfGameInner();
  if (ret == 0)
    waiting_for_nmi = 1;

  Vector_NMI();
}

CoroutineRet GameState_29_DebugGameOverMenu(void) {  // 0x8289DB
  DebugGameOverMenu();
  return kCoroutineNone;
}

CoroutineRet GameState_26_GameOverMenu(void) {  // 0x8289E0
  GameOverMenu();
  return kCoroutineNone;
}

CoroutineRet GameState_4_FileSelectMenus(void) {  // 0x8289E5
  FileSelectMenu();
  return kCoroutineNone;
}

CoroutineRet GameState_5_FileSelectMap(void) {  // 0x8289EA
  FileSelectMap();
  return kCoroutineNone;
}

static Func_Y_V *const kSfxHandlers[5] = {  // 0x8289EF
  SfxHandlers_0_SendToApu,
  SfxHandlers_1_WaitForAck,
  SfxHandlers_2_ClearRequest,
  SfxHandlers_3_WaitForAck,
  SfxHandlers_4_Reset,
};

void HandleSoundEffects(void) {
  if ((int8)(sound_handler_downtime - 1) >= 0) {
    LOBYTE(sound_handler_downtime) = sound_handler_downtime - 1;
    RtlApuWrite(APUI01, 0);
    sfx_cur[0] = 0;
    RtlApuWrite(APUI02, 0);
    sfx_cur[1] = 0;
    RtlApuWrite(APUI03, 0);
    sfx_cur[2] = 0;
  } else {
    for (int i = 0; i < 3; ++i)
      kSfxHandlers[sfx_state[i]](i);
  }
}

void SfxHandlers_0_SendToApu(uint16 j) {  // 0x828A2C
  if (sfx_readpos[j] != sfx_writepos[j]) {
    uint8 v2 = sfx_readpos[j] + j * 16;
    uint8 v3 = sfx1_queue[v2];
    RtlApuWrite((SnesRegs)(j + APUI01), v3);
    sfx_cur[j] = v3;
    sfx_readpos[j] = (v2 + 1) & 0xF;
    ++sfx_state[j];
  }
}

void SfxHandlers_1_WaitForAck(uint16 j) {  // 0x828A55
  // Modified from original
  uint8 v1 = sfx_cur[j];
  ++sfx_state[j];
  sfx_clear_delay[j] = 6;
}

void SfxHandlers_2_ClearRequest(uint16 j) {  // 0x828A6C
  // Modified from original
  if (sfx_clear_delay[j]-- == 1) {
    sfx_cur[j] = 0;
    ++sfx_state[j];
  }
}

void SfxHandlers_3_WaitForAck(uint16 j) {  // 0x828A7C
  // Modified from original
  uint8 v1 = sfx_cur[j];
  sfx_state[j] = 0;
  SfxHandlers_0_SendToApu(j);
}

void SfxHandlers_4_Reset(uint16 j) {  // 0x828A90
  if (sfx_clear_delay[j]-- == 1)
    sfx_state[j] = 0;
}

void ResetSoundQueues(void) {  // 0x828A9A
  *(uint16 *)sfx_readpos = 0;
  *(uint16 *)&sfx_readpos[2] = 0;
  *(uint16 *)&sfx_writepos[1] = 0;
  *(uint16 *)sfx_state = 0;
  sfx_state[2] = 0;
}

void ShowSpareCpu(void) {  // 0x828AB0
  if ((joypad_dbg_1 & 0x4000) != 0)
    debug_spare_cpu_display_flag = debug_spare_cpu_display_flag == 0;
  if (debug_spare_cpu_display_flag)
    WriteReg(INIDISP, reg_INIDISP & 0xF0 | 5);
  joypad1_input_samusfilter = joypad1_lastkeys;
}

CoroutineRet GameState_0_Reset_Async(void) {  // 0x828AE4
  UNUSED_word_7E0DF8 = 0;
  UNUSED_word_7E0DFA = 0;
  UNUSED_word_7E0DFC = 0;
  cinematic_function = FUNC16(CinematicFunctionOpening);
  demo_set = 0;
  if (num_demo_sets == 4)
    demo_set = 3;
  ++game_state;
  return kCoroutineNone;
}

CoroutineRet GameState_1_OpeningCinematic(void) {  // 0x828B08
  return GameState_1_OpeningCinematic_();
}

CoroutineRet GameState_3_Null(void) {
  return kCoroutineNone;
}

CoroutineRet GameState_37_CeresGoesBoomWithSamus(void) {  // 0x828B0E
  return GameState_37_CeresGoesBoomWithSamus_();
}

CoroutineRet GameState_39_EndingAndCredits(void) {  // 0x828B13
  return GameState_39_EndingAndCredits_();
}

CoroutineRet GameState_7_MainGameplayFadeIn(void) {  // 0x828B20
  COROUTINE_AWAIT_ONLY(GameState_8_MainGameplay());
  HandleFadeIn();
  if (reg_INIDISP == 15) {
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    ++game_state;
  }
  return kCoroutineNone;
}

CoroutineRet GameState_28_Unused(void) {  // 0x828B3F
  GameState_28_Unused_();
  return kCoroutineNone;
}

CoroutineRet GameState_8_MainGameplay(void) {  // 0x828B44
  COROUTINE_BEGIN(coroutine_state_1, 0);
  DetermineWhichEnemiesToProcess();
  if (1) { // !DebugHandler()) {
    PaletteFxHandler();
    HandleControllerInputForGamePhysics();
    if (!debug_disable_sprite_interact)
      SamusProjectileInteractionHandler();
    EnemyMain();

    if (queued_message_box_index) {
      COROUTINE_AWAIT(1, DisplayMessageBox_Async(queued_message_box_index));
      queued_message_box_index = 0;
    }

    HandleSamusMovementAndPause();
    EprojRunAll();
    COROUTINE_AWAIT(2, PlmHandler_Async());

    AnimtilesHandler();
    if (!debug_disable_sprite_interact) {
      EprojSamusCollDetect();
      EprojProjCollDet();
      ProcessEnemyPowerBombInteraction();
    }
    MainScrollingRoutine();
    int debug_scrolling_enabled = 0;
    if (debug_scrolling_enabled)
      DebugScrollPosSaveLoad();
    DrawSamusEnemiesAndProjectiles();
    QueueEnemyBG2TilemapTransfers();
  }
  HandleHudTilemap();
  CalculateLayer2PosAndScrollsWhenScrolling();
  RunRoomMainCode();
  HandleSamusOutOfHealthAndGameTile();
  HandleRoomShaking();
  DecrementSamusTimers();
  COROUTINE_END(0);

}

void DeleteAllOptionsMenuObjects_(void) {
  for (int i = 14; i >= 0; i -= 2) {
    int v1 = i >> 1;
    enemy_projectile_y_pos[v1 + 5] = 0;
    enemy_projectile_y_vel[v1 + 17] = 0;
  }
}

void CallOptionsEntryFunc(uint32 ea, uint16 j) {
  switch (ea) {
  case fnsub_82F296: sub_82F296(j); return;
  case fnsub_82F34B: sub_82F34B(j); return;
  case fnsub_82F353: sub_82F353(j); return;
  case fnsub_82F35B: sub_82F35B(j); return;
  case fnsub_82F363: sub_82F363(j); return;
  case fnsub_82F419: sub_82F419(j); return;
  default: Unreachable();
  }
}

uint8 CreateOptionsMenuObject_(uint16 a, uint16 j) {
  options_menu_init_param = a;
  uint16 v3 = 14;
  while (optionsmenu_instr_ptr[v3 >> 1]) {
    v3 -= 2;
    if ((v3 & 0x8000u) != 0)
      return 1;
  }
  const uint8 *v5 = RomPtr_82(j);
  int v6 = v3 >> 1;
  optionsmenu_arr1[v6] = GET_WORD(v5 + 2);
  optionsmenu_instr_ptr[v6] = GET_WORD(v5 + 4);
  optionsmenu_instr_timer[v6] = 1;
  optionsmenu_cur_data[v6] = 0;
  optionsmenu_arr5[v6] = 0;
  optionsmenu_arr6[v6] = 0;
  optionsmenu_arr7[v6] = 0;
  CallOptionsEntryFunc(GET_WORD(v5) | 0x820000, v3);
  return 0;
}

void OptionsPreInstr_nullsub_57(uint16 k) {  // 0x828C10
  ;
}

void OptionsMenuFunc1(void) {  // 0x828C11
  for (int i = 14; i >= 0; i -= 2) {
    optionsmenu_index = i;
    if (optionsmenu_instr_ptr[i >> 1]) {
      OptionsMenuFunc2(i);
      i = optionsmenu_index;
    }
  }
}

void CallOptionsPreInstr(uint32 ea, uint16 k) {
  switch (ea) {
  case fnOptionsPreInstr_nullsub_57: OptionsPreInstr_nullsub_57(k); return;
  case fnOptionsPreInstr_F2A9: OptionsPreInstr_F2A9(k); return;
  case fnOptionsPreInstr_F376: OptionsPreInstr_F376(k); return;
  case fnOptionsPreInstr_F3A0: OptionsPreInstr_F3A0(k); return;
  case fnOptionsPreInstr_F3E2: OptionsPreInstr_F3E2(k); return;
  case fnOptionsPreInstr_F42C: OptionsPreInstr_F42C(k); return;
  default: Unreachable();
  }
}

uint16 CallOptionsInstr(uint32 ea, uint16 k, uint16 j) {
  switch (ea) {
  case fnOptionsInstr_Destroy: return OptionsInstr_Destroy(k, j);
  case fnOptionsInstr_8C64: return OptionsInstr_8C64(k, j);
  case fnOptionsInstr_SetPreInstr: return OptionsInstr_SetPreInstr(k, j);
  case fnOptionsInstr_8C79: return OptionsInstr_8C79(k, j);
  case fnOptionsInstr_Goto: return OptionsInstr_Goto(k, j);
  case fnOptionsInstr_8C89: return OptionsInstr_8C89(k, j);
  case fnOptionsInstr_8C93: return OptionsInstr_8C93(k, j);
  default: return Unreachable();
  }
}

void OptionsMenuFunc2(uint16 k) {  // 0x828C2B
  CallOptionsPreInstr(optionsmenu_arr1[k >> 1] | 0x820000, k);
  uint16 v1 = optionsmenu_index;
  int v2 = optionsmenu_index >> 1;
  if (optionsmenu_instr_timer[v2]-- == 1) {
    uint16 v4 = optionsmenu_instr_ptr[v2];
    uint16 *v5;
    while (1) {
      v5 = (uint16 *)RomPtr_82(v4);
      if ((v5[0] & 0x8000u) == 0)
        break;
      v4 = CallOptionsInstr(v5[0] | 0x820000, v1, v4 + 2);
      if (!v4)
        return;
    }
    int v7 = v1 >> 1;
    optionsmenu_instr_timer[v7] = v5[0];
    optionsmenu_cur_data[v7] = v5[1];
    optionsmenu_instr_ptr[v7] = v4 + 4;
  }
}

uint16 OptionsInstr_Destroy(uint16 k, uint16 j) {  // 0x828C5A
  int v2 = k >> 1;
  optionsmenu_cur_data[v2] = 0;
  optionsmenu_instr_ptr[v2] = 0;
  return 0;
}

uint16 OptionsInstr_8C64(uint16 k, uint16 j) {  // 0x828C64
  optionsmenu_instr_ptr[k >> 1] = j - 2;
  return 0;
}

uint16 OptionsInstr_SetPreInstr(uint16 k, uint16 j) {  // 0x828C6E
  optionsmenu_arr1[k >> 1] = *(uint16 *)RomPtr_82(j);
  return j + 2;
}

uint16 OptionsInstr_8C79(uint16 k, uint16 j) {  // 0x828C79
  optionsmenu_arr1[k >> 1] = FUNC16(locret_828C81);
  return j;
}

uint16 OptionsInstr_Goto(uint16 k, uint16 j) {  // 0x828C82
  return *(uint16 *)RomPtr_82(j);
}

uint16 OptionsInstr_8C89(uint16 k, uint16 j) {  // 0x828C89
  int v2 = k >> 1;
  if (optionsmenu_arr5[v2]-- == 1)
    return j + 2;
  else
    return OptionsInstr_Goto(k, j);
}

uint16 OptionsInstr_8C93(uint16 k, uint16 j) {  // 0x828C93
  optionsmenu_arr5[k >> 1] = *(uint16 *)RomPtr_82(j);
  return j + 2;
}

void DrawOptionsMenuSpritemaps(void) {  // 0x828CA1
  for (int i = 14; i >= 0; i -= 2) {
    int v1 = i >> 1;
    if (optionsmenu_cur_data[v1]) {
      uint16 v2 = optionsmenu_cur_data[v1];
      R22_ = optionsmenu_arr10[v1];
      R20_ = optionsmenu_arr8[v1];
      R18_ = optionsmenu_arr9[v1];
      DrawSpritemap(0x82, v2);
    }
  }
}

CoroutineRet GameState_12_Pausing_Darkening_Async(void) {  // 0x828CCF
  COROUTINE_AWAIT_ONLY(GameState_8_MainGameplay());
  HandleFadeOut();
  if ((reg_INIDISP & 0xF) == 0) {
    EnableNMI();
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    ++game_state;
  }
  return kCoroutineNone;
}

void CallPauseHook(uint32 ea) {
  switch (ea) {
  case fnPauseHook_Empty: return;
  case fnPauseHook_DraygonRoom: PauseHook_DraygonRoom(); return;
  case fnPauseHook_Kraid: PauseHook_Kraid(); return;
  default: Unreachable();
  }
}

CoroutineRet CallUnpauseHook_Async(uint32 ea) {
  switch (ea) {
  case fnPauseHook_Empty: return kCoroutineNone;
  case fnUnpauseHook_DraygonRoom: return UnpauseHook_DraygonRoom();
  case fnUnpauseHook_Kraid_IsDead: return UnpauseHook_Kraid_IsDead();
  case fnUnpauseHook_Kraid_IsAlive: return UnpauseHook_Kraid_IsAlive();
  case fnKraid_UnpauseHook_IsSinking: return Kraid_UnpauseHook_IsSinking();
  case fnMotherBrainsBody_UnpauseHook: return MotherBrainsBody_UnpauseHook();
  default: Unreachable(); return kCoroutineNone;
  }
}

CoroutineRet GameState_13_Pausing_Async(void) {  // 0x828CEF
  DisableHdmaObjects();
  reg_HDMAEN = 0;
  WriteReg(HDMAEN, 0);
  DisableAnimtiles();
  BackupBG2TilemapForPauseMenu();
  CallPauseHook(Load24(&pause_hook));
  CancelSoundEffects();
  BackupSomeGfxStateForPause();
  LoadPauseMenuTilesAndClearBG2();
  LoadPauseScreenBaseTilemaps();
  LoadPauseMenuMapTilemapAndAreaLabel();
  BackupGameplayPalettesAndLoadForPause();
  ContinueInitPauseMenu();
  screen_fade_delay = 1;
  screen_fade_counter = 1;
  pausemenu_leftarrow_animation_frame = 0;
  pausemenu_palette_animation_timer = 1;
  map_scrolling_direction = 0;
  map_scrolling_speed_index = 0;
  QueueClearingOfFxTilemap();
  ++game_state;
  return kCoroutineNone;
}

void BackupBG2TilemapForPauseMenu(void) {  // 0x828D51
  WriteRegWord(VMADDL, (reg_BG2SC & 0xFC) << 8 | 1);
  // fixed: this was missing
  WriteReg(VMAIN, 0x80);
  WriteReg(DMAP1, 0x81);
  WriteReg(BBAD1, 0x39);
  WriteRegWord(A1T1L, 0xDF5C);
  WriteReg(A1B1, 0x7E);
  WriteRegWord(DAS1L, 0x1000);
  WriteReg(DAS10, 0);
  WriteRegWord(A2A1L, 0);
  WriteReg(NTRL1, 0);
  WriteReg(MDMAEN, 2u);
}

void RestoreBG2TilemapFromPauseScreen(void) {  // 0x828D96
  WriteRegWord(VMADDL, (reg_BG2SC & 0xFC) << 8);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_828DAE = { 1, 1, 0x18, LONGPTR(0x7edf5c), 0x1000 };
  SetupDmaTransfer(&unk_828DAE);
  WriteReg(MDMAEN, 2u);
}

void BackupSomeGfxStateForPause(void) {  // 0x828DBD
  pausemenu_bak_BG1SC = reg_BG1SC;
  pausemenu_bak_BG2SC = reg_BG2SC;
  pausemenu_bak_BG3SC = reg_BG3SC;
  pausemenu_bak_BG12NBA = reg_BG12NBA;
  pausemenu_bak_BG34NBA = reg_BG34NBA;
  pausemenu_bak_OBSEL = reg_OBSEL;
  pausemenu_bak_BG1HOFS = reg_BG1HOFS;
  pausemenu_bak_BG2HOFS = reg_BG2HOFS;
  pausemenu_bak_BG3HOFS = reg_BG3HOFS;
  pausemenu_bak_BG1VOFS = reg_BG1VOFS;
  pausemenu_bak_BG2VOFS = reg_BG2VOFS;
  pausemenu_bak_BG3VOFS = reg_BG3VOFS;
  pausemenu_bak_BGMODE = reg_BGMODE;
  pausemenu_bak_layer2_scroll_x = layer2_scroll_x;
  pausemenu_bak_layer2_scroll_y = layer2_scroll_y;
  pausemenu_bak_MOSAIC = reg_MOSAIC;
  pausemenu_bak_CGADSUB = next_gameplay_CGADSUB;
}

void RestoreSomeGfxStateForPause(void) {  // 0x828E19
  layer2_scroll_x = pausemenu_bak_layer2_scroll_x;
  layer2_scroll_y = pausemenu_bak_layer2_scroll_y;
  reg_BGMODE = pausemenu_bak_BGMODE;
  LOBYTE(reg_BG3VOFS) = pausemenu_bak_BG3VOFS;
  LOBYTE(reg_BG2VOFS) = pausemenu_bak_BG2VOFS;
  LOBYTE(reg_BG1VOFS) = pausemenu_bak_BG1VOFS;
  LOBYTE(reg_BG3HOFS) = pausemenu_bak_BG3HOFS;
  LOBYTE(reg_BG2HOFS) = pausemenu_bak_BG2HOFS;
  LOBYTE(reg_BG1HOFS) = pausemenu_bak_BG1HOFS;
  reg_OBSEL = pausemenu_bak_OBSEL;
  reg_BG34NBA = pausemenu_bak_BG34NBA;
  reg_BG12NBA = pausemenu_bak_BG12NBA;
  reg_BG3SC = pausemenu_bak_BG3SC;
  reg_BG2SC = pausemenu_bak_BG2SC;
  reg_BG1SC = pausemenu_bak_BG1SC;
  reg_MOSAIC = pausemenu_bak_MOSAIC;
  next_gameplay_CGADSUB = pausemenu_bak_CGADSUB;
}

void LoadPauseMenuTilesAndClearBG2(void) {  // 0x828E75
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_828E8B = { 1, 1, 0x18, LONGPTR(0xb68000), 0x4000 };
  SetupDmaTransfer(&unk_828E8B);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x20);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_828EAB = { 1, 1, 0x18, LONGPTR(0xb6c000), 0x2000 };
  SetupDmaTransfer(&unk_828EAB);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x40);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_828ECB = { 1, 1, 0x18, LONGPTR(0x9ab200), 0x2000 };
  SetupDmaTransfer(&unk_828ECB);
  WriteReg(MDMAEN, 2u);
}

void LoadPauseScreenBaseTilemaps(void) {  // 0x828EDA
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x38);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_828EF0 = { 1, 1, 0x18, LONGPTR(0xb6e000), 0x0800 };
  SetupDmaTransfer(&unk_828EF0);
  WriteReg(MDMAEN, 2u);
  WriteReg(WMADDL, 0);
  WriteReg(WMADDM, 0x34);
  WriteReg(WMADDH, 0x7E);
  static const StartDmaCopy unk_828F10 = { 1, 0, 0x80, LONGPTR(0xb6e400), 0x0400 };
  SetupDmaTransfer(&unk_828F10);
  WriteReg(MDMAEN, 2u);
  WriteReg(WMADDL, 0);
  WriteReg(WMADDM, 0x38);
  WriteReg(WMADDH, 0x7E);
  static const StartDmaCopy unk_828F30 = { 1, 0, 0x80, LONGPTR(0xb6e800), 0x0800 };
  SetupDmaTransfer(&unk_828F30);
  WriteReg(MDMAEN, 2u);
  uint16 v0 = addr_kDummySamusWireframeTilemap;
  uint16 v1 = 236;
  R20_ = 17;
  do {
    R18_ = 8;
    do {
      ram3000.pause_menu_map_tilemap[v1++] = *(uint16 *)RomPtr_82(v0);
      v0 += 2;
      --R18_;
    } while (R18_);
    v1 += 8;
    --R20_;
  } while (R20_);
  WriteSamusWireframeTilemap();
  LoadEqupmentScreenReserveHealthTilemap();
}

void LoadEqupmentScreenReserveHealthTilemap(void) {  // 0x828F70
  if (samus_max_reserve_health) {
    R42 = samus_reserve_health / 100;
    int rest = samus_reserve_health % 100;
    int div10 = rest / 10;
    int mod10 = rest % 10;

    ram3800.cinematic_bg_tilemap[394] = mod10 + 2052;
    ram3800.cinematic_bg_tilemap[393] = div10 + 2052;
    ram3800.cinematic_bg_tilemap[392] = R42 + 2052;
  }
}

void BackupGameplayPalettesAndLoadForPause(void) {  // 0x828FD4
  int16 v0;
  int16 v2;

  v0 = 512;
  uint16 v1 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v1 + 384] = palette_buffer[v1];
    ++v1;
    v0 -= 2;
  } while (v0);
  v2 = 512;
  uint16 v3 = 0;
  do {
    palette_buffer[v3 >> 1] = kPauseScreenPalettes[v3 >> 1];
    v3 += 2;
    v2 -= 2;
  } while (v2);
}

void ContinueInitPauseMenu(void) {  // 0x829009
  SetupPpuForPauseMenu();
  ResetPauseMenuAnimations();
  LoadEquipmentScreenEquipmentTilemaps();
  SetPauseScreenButtonLabelPalettes_0();
  UpdatePauseMenuLRStartVramTilemap();
  DetermineMapScrollLimits();
  SetupMapScrollingForPauseMenu(0x80);
}

void SetupMapScrollingForFileSelectMap(void) {  // 0x829028
  int16 v0;
  int16 v1;

  ResetPauseMenuAnimations();
  DetermineMapScrollLimits();
  reg_BG1HOFS = map_min_x_scroll + ((uint16)(map_max_x_scroll - map_min_x_scroll) >> 1) - 128;
  R18_ = 8 * (room_x_coordinate_on_map + (uint8)((uint16)(samus_x_pos & 0xFF00) >> 8)) - reg_BG1HOFS;
  v0 = 224 - R18_;
  if ((int16)(224 - R18_) >= 0) {
    R18_ = 32 - R18_;
    if ((R18_ & 0x8000u) == 0)
      reg_BG1HOFS -= R18_;
  } else {
    R18_ = 224 - R18_;
    reg_BG1HOFS -= v0;
  }
  R18_ = map_min_y_scroll + ((uint16)(map_max_y_scroll - map_min_y_scroll) >> 1) + 16;
  reg_BG1VOFS = -((112 - R18_) & 0xFFF8);
  R18_ = 8 * (room_y_coordinate_on_map + HIBYTE(samus_y_pos) + 1) + ((112 - R18_) & 0xFFF8);
  v1 = 64 - R18_;
  if ((int16)(64 - R18_) >= 0) {
    R18_ = 64 - R18_;
    reg_BG1VOFS -= v1;
    if (sign16(reg_BG1VOFS + 40))
      reg_BG1VOFS = -40;
  }
}

CoroutineRet GameState_14_Paused_Async(void) {  // 0x8290C8
  DrawPauseMenuDuringFadeIn();
  HandleFadeIn();
  if (reg_INIDISP == 15) {
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    ++game_state;
  }
  return kCoroutineNone;
}

CoroutineRet GameState_15_Paused_Async(void) {  // 0x8290E8
  ReleaseButtonsFilter(3);
  MainPauseRoutine();
  HandleHudTilemap();
  HandlePauseScreenPaletteAnimation();
  return kCoroutineNone;
}

static Func_V *const kPauseMenuFuncs[8] = {  // 0x8290FF
  PauseMenu_0_MapScreen,
  PauseMenu_1_EquipmentScreen,
  PauseMenu_2,
  PauseMenu_3_MapToEquipment_Load,
  PauseMenu_4_MapToEquipment_FadeIn,
  PauseMenu_5,
  PauseMenu_6_EquipmentToMap_Load,
  PauseMenu_7_EquipmentToMap_FadeIn,
};
void MainPauseRoutine(void) {
  kPauseMenuFuncs[menu_index]();
}

void PauseMenu_0_MapScreen(void) {  // 0x829120
  HandlePauseScreenLR();
  HandlePauseScreenStart();
  HandleMapScrollArrows();
  MapScrolling();
  MapScreenDrawSamusPositionIndicator();
  DrawMapIcons();
  DisplayMapElevatorDestinations();
  pause_screen_mode = 0;
}

void PauseMenu_1_EquipmentScreen(void) {  // 0x829142
  reg_BG1HOFS = 0;
  reg_BG1VOFS = 0;
  EquipmentScreenMain();
  HandlePauseScreenLR();
  HandlePauseScreenStart();
  pause_screen_mode = 1;
}

void PauseMenu_2(void) {  // 0x829156
  DisplayMapElevatorDestinations();
  MapScreenDrawSamusPositionIndicator();
  DrawMapIcons();
  HandlePauseMenuLRPressHighlight();
  pause_screen_mode = 0;
  HandleFadeOut();
  if (reg_INIDISP == 0x80) {
    EnableNMI();
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    ++menu_index;
  }
}

void PauseMenu_5(void) {  // 0x829186
  EquipmentScreenDrawItemSelector();
  EquipmentScreenDisplayReserveTankAmount();
  HandlePauseMenuLRPressHighlight();
  HandleFadeOut();
  if (reg_INIDISP == 0x80) {
    EnableNMI();
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    ++menu_index;
  }
}
void PauseMenu_3_MapToEquipment_Load(void) {  // 0x8291AB
  DisplayMapElevatorDestinations();
  EquipmentScreenSetupReserveMode();
  EquipmentScreenTransferBG1Tilemap();
  pause_screen_mode = 1;
  SetPauseScreenButtonLabelPalettes();
  pausemenu_lr_animation_frame = 0;
  pausemenu_lr_animation_timer = *(uint16 *)kPauseLrHighlightAnimData;
  screen_fade_delay = 1;
  screen_fade_counter = 1;
  ++menu_index;
}

void PauseMenu_6_EquipmentToMap_Load(void) {  // 0x8291D7
  DisplayMapElevatorDestinations();
  LoadPauseMenuMapTilemapAndAreaLabel();
  SetPauseScreenButtonLabelPalettes();
  pausemenu_lr_animation_frame = 0;
  pausemenu_lr_animation_timer = *(uint16 *)kPauseLrHighlightAnimData;
  screen_fade_delay = 1;
  screen_fade_counter = 1;
  pause_screen_mode = 0;
  ++menu_index;
}

void PauseMenu_7_EquipmentToMap_FadeIn(void) {  // 0x829200
  MapScreenDrawSamusPositionIndicator();
  DrawMapIcons();
  DisplayMapElevatorDestinations();
  pause_screen_mode = 0;
  HandleFadeIn();
  if (reg_INIDISP == 15) {
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    uint16 v0 = pausemenu_button_label_mode;
    if (pausemenu_button_label_mode)
      v0 = 1;
    menu_index = v0;
  }
}

void PauseMenu_4_MapToEquipment_FadeIn(void) {  // 0x829231
  EquipmentScreenDrawItemSelector();
  EquipmentScreenDisplayReserveTankAmount();
  pause_screen_mode = 1;
  HandleFadeIn();
  if (reg_INIDISP == 15) {
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    uint16 v0 = pausemenu_button_label_mode;
    if (pausemenu_button_label_mode)
      v0 = 1;
    menu_index = v0;
  }
}

static Func_V *const kMapScrollingFuncs[5] = {  // 0x82925D
  MapScrolling_0_None,
  MapScrolling_1_Left,
  MapScrolling_2_Right,
  MapScrolling_3_Up,
  MapScrolling_4_Down,
};
void MapScrolling(void) {
  kMapScrollingFuncs[map_scrolling_direction]();
}

void MapScrolling_0_None(void) {  // 0x829278
  map_scrolling_gear_switch_timer = 4;
}

uint16 MapScrolling_GetSpeedIndex(void) {  // 0x82927F
  uint16 result = map_scrolling_speed_index;
  if (!map_scrolling_gear_switch_timer)
    return map_scrolling_speed_index + 32;
  return result;
}

static const uint16 kMapScrollingSpeedTable[32] = {  // 0x82928E
  0, 0, 0, 8,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 8,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
};

void MapScrolling_1_Left(void) {
  reg_BG1HOFS -= kMapScrollingSpeedTable[MapScrolling_GetSpeedIndex() >> 1];
  MapScrolling_Common();
}

void MapScrolling_Common(void) {  // 0x829299
  ++map_scrolling_speed_index;
  if ((++map_scrolling_speed_index & 0xF) == 0) {
    QueueSfx1_Max6(0x36u);
    map_scrolling_direction = 0;
    map_scrolling_speed_index = 0;
    if (map_scrolling_gear_switch_timer)
      --map_scrolling_gear_switch_timer;
  }
}

void MapScrolling_2_Right(void) {  // 0x8292BD
  reg_BG1HOFS += kMapScrollingSpeedTable[MapScrolling_GetSpeedIndex() >> 1];
  MapScrolling_Common();
}

void MapScrolling_3_Up(void) {  // 0x8292CA
  reg_BG1VOFS -= kMapScrollingSpeedTable[MapScrolling_GetSpeedIndex() >> 1];
  MapScrolling_Common();
}

void MapScrolling_4_Down(void) {  // 0x8292D7
  reg_BG1VOFS += kMapScrollingSpeedTable[MapScrolling_GetSpeedIndex() >> 1];
  MapScrolling_Common();
}

CoroutineRet GameState_16_Unpausing_Async(void) {  // 0x829324
  HighlightPauseScreenButton();
  DrawPauseMenuDuringFadeout();
  HandleFadeOut();
  if (reg_INIDISP == 0x80) {
    EnableNMI();
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    ++game_state;
  }
  return kCoroutineNone;
}

void DrawPauseMenuDuringFadeout(void) {  // 0x82934B
  if (pause_screen_mode == 1) {
    EquipmentScreenDrawItemSelector();
    EquipmentScreenDisplayReserveTankAmount();
    HandlePauseMenuLRPressHighlight();
  } else {
    DisplayMapElevatorDestinations();
    DrawMapIcons();
    MapScreenDrawSamusPositionIndicator();
  }
}

CoroutineRet GameState_17_Unpausing_Async(void) {  // 0x829367
  COROUTINE_BEGIN(coroutine_state_1, 0); 
  ClearSamusBeamTiles();
  ContinueInitGameplayResume();
  ResumeGameplay();
  RestoreSomeGfxStateForPause();
  RestoreBG2TilemapFromPauseScreen();
  screen_fade_delay = 1;
  screen_fade_counter = 1;
  COROUTINE_AWAIT(1, CallUnpauseHook_Async(Load24(&unpause_hook)));
  EnableHdmaObjects();
  EnableAnimtiles();
  QueueSamusMovementSfx();
  ++game_state;
  COROUTINE_END(0);
}

CoroutineRet GameState_18_Unpausing(void) {  // 0x8293A1
  COROUTINE_AWAIT_ONLY(GameState_8_MainGameplay());
  HandleFadeIn();
  if (reg_INIDISP == 15) {
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    game_state = kGameState_8_MainGameplay;
  }
  return kCoroutineNone;
}

void LoadPauseMenuMapTilemapAndAreaLabel(void) {  // 0x8293C3
  reg_BG1HOFS = reg_BG4HOFS;
  reg_BG1VOFS = reg_BG4VOFS;
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x30);
  WriteReg(VMAIN, 0x80);
  LoadPauseMenuMapTilemap();
  static const StartDmaCopy unk_8293E9 = { 1, 1, 0x18, LONGPTR(0x7e4000), 0x1000 };
  SetupDmaTransfer(&unk_8293E9);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0xAA);
  WriteReg(VMADDH, 0x38);
  WriteReg(VMAIN, 0x80);
  WriteReg(DMAP1, 1u);
  WriteReg(BBAD1, 0x18);
  WriteReg(DAS1L, 0x18);
  WriteReg(DAS1H, 0);
  uint16 v0 = area_index;
  if (!sign16(area_index - 7))
    v0 = 0;
  WriteRegWord(A1T1L, kPauseAreaLabelTilemap[v0]);
  WriteReg(A1B1, 0x82);
  WriteReg(MDMAEN, 2u);
}

void LoadPauseMenuMapTilemap(void) {  // 0x82943D
  uint16 v5;
  uint16 v6;
  int16 v9;
  uint16 v13;

  uint16 v0 = area_index;
  if (!sign16(area_index - 7))
    v0 = 0;
  R18_ = v0;
  R0_.addr = *(VoidP *)((char *)&kPauseMenuMapTilemaps[0].addr + (uint16)(3 * v0));
  *(uint16 *)&R0_.bank = *(uint16 *)(&kPauseMenuMapTilemaps[0].bank + (uint16)(3 * v0));
  R3_.addr = ADDR16_OF_RAM(ram4000);
  *(uint16 *)&R3_.bank = 126;
  R8_ = 130;
  R6_ = kPauseMenuMapData[v0];
  if (map_station_byte_array[area_index]) {
    uint8 *v4 = IndirPtr(&R6_, 0);
    LOBYTE(v5) = GET_HIBYTE(GET_WORD(v4));
    HIBYTE(v5) = GET_WORD(v4);
    R38 = v5;
    ++R6_;
    ++R6_;
    v6 = ADDR16_OF_RAM(*map_tiles_explored);
    *(uint16 *)((char *)&R10_ + 1) = 0;
    *(uint16 *)((char *)&R8_ + 1) = v6;
    uint8 *v7 = IndirPtr((char *)&R8_ + 1, 0);
    LOBYTE(v5) = GET_HIBYTE(GET_WORD(v7));
    HIBYTE(v5) = GET_WORD(v7);
    R40 = v5;
    ++ *(uint16 *)((char *)&R8_ + 1);
    ++ *(uint16 *)((char *)&R8_ + 1);
    uint16 v8 = 0;
    v9 = 16;
    do {
      uint16 v10 = *(uint16 *)IndirPtr(&R0_, v8);
      bool v11 = R40 >> 15;
      R40 *= 2;
      if (v11) {
        v10 &= ~0x400u;
        R38 *= 2;
      } else {
        v11 = R38 >> 15;
        R38 *= 2;
        if (!v11)
          v10 = 31;
      }
      IndirWriteWord(&R3_, v8, v10);
      if (!--v9) {
        v9 = 16;
        uint8 *v12 = IndirPtr(&R6_, 0);
        LOBYTE(v13) = GET_HIBYTE(GET_WORD(v12));
        HIBYTE(v13) = GET_WORD(v12);
        R38 = v13;
        ++R6_;
        ++R6_;
        uint8 *v14 = IndirPtr((char *)&R8_ + 1, 0);
        LOBYTE(v13) = GET_HIBYTE(GET_WORD(v14));
        HIBYTE(v13) = GET_WORD(v14);
        R40 = v13;
        ++ *(uint16 *)((char *)&R8_ + 1);
        ++ *(uint16 *)((char *)&R8_ + 1);
      }
      v8 += 2;
    } while ((int16)(v8 - 4096) < 0);
  } else {
    uint16 v1 = 0;
    uint16 v2 = 0;
    LOBYTE(R18_) = 0;
    while (1) {
      uint8 t = map_tiles_explored[v2];
      map_tiles_explored[v2] <<= 1;
      if (!(t & 0x80)) {
        IndirWriteWord(&R3_, v1, 0x1F);
      } else {
        ++map_tiles_explored[v2];
        uint8 *v3 = IndirPtr(&R0_, v1);
        IndirWriteWord(&R3_, v1, GET_WORD(v3) & 0xFBFF);
      }
      v1 += 2;
      LOBYTE(R18_) = R18_ + 1;
      if (!sign8(R18_ - 8)) {
        LOBYTE(R18_) = 0;
        if ((int16)(++v2 - 256) >= 0)
          break;
      }
    }
  }
}

void DrawRoomSelectMap(void) {  // 0x829517
  uint16 v3; // r8
  uint16 v6;
  unsigned int v7; // kr00_4
  int16 v10;
  uint16 v14;
  VramWriteEntry *v17;

  reg_BG12NBA = 51;
  reg_TM = 19;
  reg_BG1VOFS = -40;
  uint16 v0 = area_index;
  if (!sign16(area_index - 7))
    v0 = 0;
  R18_ = v0;
  R0_.addr = *(VoidP *)((char *)&kPauseMenuMapTilemaps[0].addr + (uint16)(3 * v0));
  *(uint16 *)&R0_.bank = *(uint16 *)(&kPauseMenuMapTilemaps[0].bank + (uint16)(3 * v0));
  R3_.addr = ADDR16_OF_RAM(ram3000);
  R3_.bank = 126;
  R8_ = 130;
  R6_ = kPauseMenuMapData[v0];
  if (map_station_byte_array[area_index]) {
    uint8 *v5 = IndirPtr(&R6_, 0);
    LOBYTE(v6) = GET_HIBYTE(GET_WORD(v5));
    HIBYTE(v6) = GET_WORD(v5);
    R38 = v6;
    ++R6_;
    ++R6_;
    v7 = 2039;
    *(uint16 *)((char *)&R10_ + 1) = HIWORD(v7);
    *(uint16 *)((char *)&R8_ + 1) = v7;
    uint8 *v8 = IndirPtr((char *)&R8_ + 1, 0);
    LOBYTE(v6) = GET_HIBYTE(GET_WORD(v8));
    HIBYTE(v6) = GET_WORD(v8);
    R40 = v6;
    ++ *(uint16 *)((char *)&R8_ + 1);
    ++ *(uint16 *)((char *)&R8_ + 1);
    uint16 v9 = 0;
    v10 = 16;
    do {
      uint16 v11 = *(uint16 *)IndirPtr(&R0_, v9);
      bool v12 = R40 >> 15;
      R40 *= 2;
      if (v12) {
        v11 &= ~0x400u;
        R38 *= 2;
      } else {
        v12 = R38 >> 15;
        R38 *= 2;
        if (!v12)
          v11 = 31;
      }
      IndirWriteWord(&R3_, v9, v11);
      if (!--v10) {
        v10 = 16;
        uint8 *v13 = IndirPtr(&R6_, 0);
        LOBYTE(v14) = GET_HIBYTE(GET_WORD(v13));
        HIBYTE(v14) = GET_WORD(v13);
        R38 = v14;
        ++R6_;
        ++R6_;
        uint8 *v15 = IndirPtr((char *)&R8_ + 1, 0);
        LOBYTE(v14) = GET_HIBYTE(GET_WORD(v15));
        HIBYTE(v14) = GET_WORD(v15);
        R40 = v14;
        ++ *(uint16 *)((char *)&R8_ + 1);
        ++ *(uint16 *)((char *)&R8_ + 1);
      }
      v9 += 2;
    } while ((int16)(v9 - 4096) < 0);
  } else {
    uint16 v1 = 0;
    uint16 v2 = 0;
    LOBYTE(R18_) = 0;
    while (1) {
      uint8 what = map_tiles_explored[v2];
      map_tiles_explored[v2] <<= 1;
      if (!(what & 0x80)) {
        IndirWriteWord(&R3_, v1, 0xF);
      } else {
        ++map_tiles_explored[v2];
        IndirWriteWord(&R3_, v1, *(uint16 *)IndirPtr(&R0_, v1) & ~0x400);
      }
      v1 += 2;
      LOBYTE(R18_) = R18_ + 1;
      LOBYTE(v3) = R18_ - 8;
      if (!(uint8)sign8(v3)) {
        LOBYTE(R18_) = 0;
        if ((int16)(++v2 - 256) >= 0)
          break;
      }
    }
  }
  uint16 v16 = vram_write_queue_tail;
  v17 = gVramWriteEntry(vram_write_queue_tail);
  v17->size = 4096;
  v17->src.addr = ADDR16_OF_RAM(ram3000);
  *(uint16 *)&v17->src.bank = 126;
  v17->vram_dst = (reg_BG1SC & 0xFC) << 8;
  vram_write_queue_tail = v16 + 7;
}

void DrawRoomSelectMapAreaLabel(void) {  // 0x829628
  const uint8 *v2 = RomPtr_82(kPauseAreaLabelTilemap[area_index]);
  for(int i = 0; i < 24; i += 2)
    IndirWriteWord(&R0_, i, *(uint16 *)(v2 + i) & 0xEFFF);
}

void SetupMapScrollingForPauseMenu(uint16 a) {  // 0x829E27
  int16 v1;
  int16 v2;

  R20_ = a;
  reg_BG1HOFS = map_min_x_scroll + ((uint16)(map_max_x_scroll - map_min_x_scroll) >> 1) - 128;
  R18_ = 8 * (room_x_coordinate_on_map + (uint8)((uint16)(samus_x_pos & 0xFF00) >> 8)) - reg_BG1HOFS;
  v1 = 224 - R18_;
  if ((int16)(224 - R18_) >= 0) {
    R18_ = 32 - R18_;
    if ((R18_ & 0x8000u) == 0)
      reg_BG1HOFS -= R18_;
  } else {
    R18_ = 224 - R18_;
    reg_BG1HOFS -= v1;
  }
  R18_ = map_min_y_scroll + ((uint16)(map_max_y_scroll - map_min_y_scroll) >> 1) + 16;
  reg_BG1VOFS = -((R20_ - R18_) & 0xFFF8);
  R18_ = 8 * (room_y_coordinate_on_map + HIBYTE(samus_y_pos) + 1) + ((R20_ - R18_) & 0xFFF8);
  v2 = 64 - R18_;
  if ((int16)(64 - R18_) >= 0) {
    R18_ = 64 - R18_;
    reg_BG1VOFS -= v2;
    if (sign16(reg_BG1VOFS + 40))
      reg_BG1VOFS = -40;
  }
}

void DetermineMapScrollLimits(void) {  // 0x829EC4
  if (has_area_map) {
    R8_ = 130;
    R6_ = addr_kPauseMenuMapData;
    R6_ = *(uint16 *)IndirPtr(&R6_, 2 * area_index);
  } else {
    R8_ = 0;
    R6_ = 2039;
  }
  R0_.bank = R8_;
  R0_.addr = R6_;
  uint16 v0 = DetermineLeftmostMapColumn();
  map_min_x_scroll = MultiplyBy8(v0);
  if (area_index == 4)
    map_min_x_scroll -= 24;
  R0_.addr = R6_ + 131;
  uint16 v1 = DetermineRightmostMapColumn();
  map_max_x_scroll = MultiplyBy8(v1);
  R0_.addr = R6_;
  uint16 v2 = DetermineTopmostMapColumn();
  map_min_y_scroll = MultiplyBy8(v2);
  R0_.addr = R6_ + 124;
  uint16 v3 = DetermineBottommostMapColumn();
  map_max_y_scroll = MultiplyBy8(v3);
}

uint16 MultiplyBy8(uint16 k) {  // 0x829F45
  return 8 * k;
}
static const uint8 k0x80Shr[8] = {  // 0x829F4A
  0x80, 0x40, 0x20, 0x10,
     8,    4,    2,    1,
};
static const uint8 k0x80Shr_0[8] = { 0x80, 0x40, 0x20, 0x10, 8, 4, 2, 1 };
uint16 DetermineLeftmostMapColumn(void) {
  uint16 v0;

  HIBYTE(v0) = 0;
  uint16 result = 0;
LABEL_2:
  LOBYTE(v0) = result & 7;
  LOBYTE(R18_) = k0x80Shr[v0];
  uint16 v2 = 0;
  while (((uint8)R18_ & *IndirPtr(&R0_, v2)) == 0) {
    v2 += 4;
    if ((int16)(v2 - 128) >= 0) {
      if ((int16)(++result - 64) >= 0)
        return 26;
      if ((result & 7) == 0) {
        R0_.addr += 1;
      }
      if (result == 32) {
        R0_.addr += 123;
      }
      goto LABEL_2;
    }
  }
  return result;
}

uint16 DetermineRightmostMapColumn(void) {  // 0x829FA9
  uint16 result = 63;
LABEL_2:
  LOBYTE(R18_) = k0x80Shr_0[result & 7];
  uint16 v1 = 0;
  while (((uint8)R18_ & *IndirPtr(&R0_, v1)) == 0) {
    v1 += 4;
    if ((int16)(v1 - 128) >= 0) {
      if ((--result & 0x8000u) != 0)
        return 28;
      if ((result & 7) == 7) {
        bool v2 = LOBYTE(R0_.addr)-- != 0;
        HIBYTE(R0_.addr) -= !v2;
      }
      if (result == 31)
        R0_.addr -= 124;
      goto LABEL_2;
    }
  }
  return result;
}

uint16 DetermineTopmostMapColumn(void) {  // 0x82A009
  R3_.addr = R0_.addr + 128;
  R3_.bank = R0_.bank;
  uint16 result = 0;
  uint16 v1 = 0;
  while (!*IndirPtr(&R0_, v1) && !*IndirPtr(&R3_, v1)) {
    if ((int16)(++v1 - 4) >= 0) {
      v1 = 0;
      R0_.addr += 4;
      R3_.addr += 4;
      if ((int16)(++result - 31) >= 0)
        return 1;
    }
  }
  return result;
}

uint16 DetermineBottommostMapColumn(void) {  // 0x82A053
  R3_.addr = R0_.addr + 128;
  R3_.bank = R0_.bank;
  uint16 result = 31;
  uint16 v1 = 0;
  while (!*IndirPtr(&R0_, v1) && !*IndirPtr(&R3_, v1)) {
    if ((int16)(++v1 - 4) >= 0) {
      v1 = 0;
      R0_.addr -= 4;
      R3_.addr -= 4;
      if (!--result)
        return 11;
    }
  }
  return result;
}

void SetupPpuForPauseMenu(void) {  // 0x82A09A
  WriteReg(OBSEL, 1u);
  reg_OBSEL = 1;
  WriteReg(BGMODE, 9u);
  reg_BGMODE = 9;
  reg_BG12NBA = 0;
  WriteReg(BG12NBA, 0);
  reg_BG34NBA = 4;
  WriteReg(BG34NBA, 4u);
  reg_BG1SC = 49;
  WriteReg(BG1SC, 0x31);
  reg_BG2SC = 56;
  WriteReg(BG2SC, 0x38);
  reg_BG3SC = 88;
  WriteReg(BG3SC, 0x58);
  reg_BG4SC = 0;
  WriteReg(BG4SC, 0);
  WriteReg(TM, 0x17);
  reg_TM = 23;
  WriteReg(MOSAIC, 0);
  reg_MOSAIC = 0;
  reg_COLDATA[0] &= 0xE0u;
  reg_COLDATA[1] &= 0xE0u;
  reg_COLDATA[2] &= 0xE0u;
  next_gameplay_CGADSUB = 0;
}

void ResetPauseMenuAnimations(void) {  // 0x82A0F7
  pausemenu_reserve_tank_delay_ctr = 0;
  reg_BG1HOFS = 0;
  reg_BG2HOFS = 0;
  reg_BG3HOFS = 0;
  reg_BG2VOFS = 0;
  reg_BG3VOFS = 0;
  pausemenu_button_label_mode = 0;
  pausemenu_lr_animation_frame = 0;
  UNUSED_word_7E0745 = 0;
  samus_position_indicator_animation_frame = 0;
  samus_position_indicator_animation_timer = 0;
  samus_position_indicator_animation_loop_counter = 0;
  pausemenu_lr_animation_timer = *(uint16 *)kPauseLrHighlightAnimData;
  pausemenu_palette_animation_timer = 1;
  pausemenu_palette_animation_frame = 0;
}
void LoadEquipmentScreenEquipmentTilemaps(void) {  // 0x82A12B
  if (samus_max_reserve_health) {
    R3_.addr = addr_kEquipmentTilemapOffs_Tanks;
    *(uint16 *)&R3_.bank = 130;
    R0_.addr = *(uint16 *)IndirPtr(&R3_, 0);
    R22_ = 14;
    Copy_R22_Bytes(kEquipmentTilemaps_Tanks[0]);
    R3_.addr = addr_kEquipmentTilemapOffs_Tanks;
    *(uint16 *)&R3_.bank = 130;
    R0_.addr = *(uint16 *)IndirPtr(&R3_, 2u);
    R22_ = 14;
    Copy_R22_Bytes(kEquipmentTilemaps_Tanks[1]);
  }
  uint16 v0 = 0;
  R3_.addr = addr_kEquipmentTilemapOffs_Weapons;
  *(uint16 *)&R3_.bank = 130;
  R0_.addr = *(uint16 *)IndirPtr(&R3_, 0);
  if (hyper_beam_flag) {
    uint16 v3 = 0;
    do {
      uint16 v4 = kHyperBeamWeaponsTilemaps[v3 >> 1];
      R22_ = 10;
      Copy_R22_Bytes(v4);
      v3 += 2;
      R0_.addr = *(uint16 *)IndirPtr(&R3_, v3);
    } while ((int16)(v3 - 10) < 0);
  } else {
    do {
      int v1 = v0 >> 1;
      if ((collected_beams & kEquipmentBitmasks_Weapons[v1]) != 0) {
        uint16 v2 = kEquipmentTilemaps_Weapons[v1];
        R22_ = 10;
        Copy_R22_Bytes(v2);
        if ((equipped_beams & kEquipmentBitmasks_Weapons[v1]) == 0) {
          R18_ = 3072;
          R22_ = 10;
          SetPaletteOfR22TilemapBytesToR18();
        }
      } else {
        R22_ = 10;
        Copy_R22_Bytes(addr_kEquipmentScreenTilemap_Blank);
      }
      v0 += 2;
      R0_.addr = *(uint16 *)IndirPtr(&R3_, v0);
    } while ((int16)(v0 - 12) < 0);
  }
  uint16 v5 = 0;
  R3_.addr = addr_kEquipmentTilemapOffs_Suits;
  *(uint16 *)&R3_.bank = 130;
  R0_.addr = *(uint16 *)IndirPtr(&R3_, 0);
  do {
    int v6 = v5 >> 1;
    if ((collected_items & kEquipmentBitmasks_Suits[v6]) != 0) {
      uint16 v7 = kEquipmentTilemaps_Suits[v6];
      R22_ = 18;
      Copy_R22_Bytes(v7);
      if ((equipped_items & kEquipmentBitmasks_Suits[v6]) == 0) {
        R18_ = 3072;
        R22_ = 18;
        SetPaletteOfR22TilemapBytesToR18();
      }
    } else {
      R22_ = 18;
      Copy_R22_Bytes(addr_kEquipmentScreenTilemap_Blank);
    }
    v5 += 2;
    R0_.addr = *(uint16 *)IndirPtr(&R3_, v5);
  } while ((int16)(v5 - 12) < 0);
  uint16 v8 = 0;
  R3_.addr = addr_kEquipmentTilemapOffs_Boots;
  *(uint16 *)&R3_.bank = 130;
  R0_.addr = *(uint16 *)IndirPtr(&R3_, 0);
  do {
    int v9 = v8 >> 1;
    if ((collected_items & kEquipmentBitmasks_Boots[v9]) != 0) {
      R22_ = 18;
      Copy_R22_Bytes(kEquipmentTilemaps_Boots[v9]);
      if ((equipped_items & kEquipmentBitmasks_Boots[v9]) == 0) {
        R18_ = 3072;
        R22_ = 18;
        SetPaletteOfR22TilemapBytesToR18();
      }
    } else {
      R22_ = 18;
      Copy_R22_Bytes(addr_kEquipmentScreenTilemap_Blank);
    }
    v8 += 2;
    R0_.addr = *(uint16 *)IndirPtr(&R3_, v8);
  } while ((int16)(v8 - 6) < 0);
}

void Copy_R22_Bytes(uint16 k) {  // 0x82A27E
  R0_.bank = 0x7E;
  uint16 v1 = 0;
  do {
    const uint16 *v2 = (const uint16 *)RomPtr_82(k);
    IndirWriteWord(&R0_, v1, *v2);
    k += 2;
    v1 += 2;
    R22_ -= 2;
  } while (R22_);
}

void SetPaletteOfR22TilemapBytesToR18(void) {  // 0x82A29D
  R0_.bank = 0x7E;
  uint16 v0 = 0;
  do {
    uint8 *v1 = IndirPtr(&R0_, v0);
    IndirWriteWord(&R0_, v0, R18_ | *(uint16 *)v1 & 0xE3FF);
    v0 += 2;
    R22_ -= 2;
  } while (R22_);
}

void ClearSamusBeamTiles(void) {  // 0x82A2BE
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x60);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_82A2D4 = { 1, 1, 0x18, LONGPTR(0x9ad200), 0x1000 };
  SetupDmaTransfer(&unk_82A2D4);
  WriteReg(MDMAEN, 2u);
}

void ContinueInitGameplayResume(void) {  // 0x82A2E3
  int16 v0;

  v0 = 512;
  uint16 v1 = 0;
  do {
    palette_buffer[v1] = ram3000.pause_menu_map_tilemap[v1 + 384];
    ++v1;
    v0 -= 2;
  } while (v0);
  SetupPpuForGameplayResume();
  CalculateBgScrolls_Unpause();
  UpdateBeamTilesAndPalette_Unpause();
  ClearPauseMenuData();
  CallSomeSamusCode(0xCu);
}

void SetupPpuForGameplayResume(void) {  // 0x82A313
  WriteReg(OBSEL, 3u);
  reg_OBSEL = 3;
  WriteReg(BGMODE, 9u);
  reg_BGMODE = 9;
  reg_BG12NBA = 0;
  WriteReg(BG12NBA, 0);
  reg_BG34NBA = 4;
  WriteReg(BG34NBA, 4u);
  reg_BG1SC = 81;
  WriteReg(BG1SC, 0x51);
  reg_BG2SC = 73;
  WriteReg(BG2SC, 0x49);
  reg_BG3SC = 90;
  WriteReg(BG3SC, 0x5A);
  reg_BG4SC = 0;
  WriteReg(BG4SC, 0);
}

void CalculateBgScrolls_Unpause(void) {  // 0x82A34E
  reg_BG1HOFS = bg1_x_offset + layer1_x_pos;
  reg_BG1VOFS = bg1_y_offset + layer1_y_pos;
  reg_BG2HOFS = bg2_x_scroll + layer2_x_pos;
  reg_BG2VOFS = bg2_y_scroll + layer2_y_pos;
}

void UpdateBeamTilesAndPalette_Unpause(void) {  // 0x82A377
  UpdateBeamTilesAndPalette();
}

void ClearPauseMenuData(void) {  // 0x82A380
  menu_index = 0;
  pausemenu_start_lr_pressed_highlight_timer = 0;
  pausemenu_lr_animation_timer = 0;
  pausemenu_item_selector_animation_timer = 0;
  pausemenu_reserve_tank_animation_timer = 0;
  UNUSED_word_7E0731 = 0;
  pausemenu_uparrow_animation_timer = 0;
  pausemenu_downarrow_animation_timer = 0;
  pausemenu_rightarrow_animation_timer = 0;
  pausemenu_leftarrow_animation_timer = 0;
  UNUSED_word_7E073D = 0;
  pausemenu_lr_animation_frame = 0;
  pausemenu_item_selector_animation_frame = 0;
  pausemenu_reserve_tank_animation_frame = 0;
  UNUSED_word_7E0745 = 0;
  pausemenu_uparrow_animation_frame = 0;
  pausemenu_downarrow_animation_frame = 0;
  pausemenu_rightarrow_animation_frame = 0;
  pausemenu_leftarrow_animation_frame = 0;
  pausemenu_shoulder_button_highlight = 0;
  pausemenu_button_label_mode = 0;
  pausemenu_equipment_category_item = 0;
  pausemenu_reserve_tank_delay_ctr = 0;
  UNUSED_word_7E0759 = 0;
  UNUSED_word_7E075B = 0;
  UNUSED_word_7E075D = 0;
  UNUSED_word_7E075F = 0;
  UNUSED_word_7E0761 = 0;
}


void sub_82A425(void) {  // 0x82A425
  ;
}

void sub_82A42A(void) {  // 0x82A42A
  if (samus_pose == kPose_81_FaceR_Screwattack || samus_pose == kPose_82_FaceL_Screwattack) {
    if ((equipped_items & 8) != 0)
      return;
    goto LABEL_8;
  }
  if ((samus_pose == kPose_1B_FaceR_SpaceJump || samus_pose == kPose_1C_FaceL_SpaceJump)
      && (equipped_items & 0x20) == 0) {
LABEL_8:
    if (samus_pose_x_dir == 4)
      samus_pose = kPose_1A_FaceL_SpinJump;
    else
      samus_pose = kPose_19_FaceR_SpinJump;
    SamusFunc_F433();
    Samus_SetAnimationFrameIfPoseChanged();
  }
}

void sub_82A47B(void) {  // 0x82A47B
  if ((equipped_items & 4) == 0) {
    if (samus_pose_x_dir == 4)
      samus_pose = kPose_41_FaceL_Morphball_Ground;
    else
      samus_pose = kPose_1D_FaceR_Morphball_Ground;
    SamusFunc_F433();
    Samus_SetAnimationFrameIfPoseChanged();
  }
}

void sub_82A4A9(void) {  // 0x82A4A9
  if ((equipped_items & 2) != 0) {
    if (samus_pose_x_dir == 4)
      samus_pose = kPose_7A_FaceL_Springball_Ground;
    else
      samus_pose = kPose_79_FaceR_Springball_Ground;
    SamusFunc_F433();
    Samus_SetAnimationFrameIfPoseChanged();
  }
}

void HandlePauseScreenLR(void) {  // 0x82A505
  HandlePauseScreenLrInput();
  DrawLrHighlight();
}

void HandlePauseScreenLrInput(void) {  // 0x82A50C
  if ((newly_held_down_timed_held_input & kButton_L) != 0) {
    if (!pausemenu_button_label_mode)
      return;
    pausemenu_start_lr_pressed_highlight_timer = 5;
    menu_index = 5;
    pausemenu_shoulder_button_highlight = 1;
    pausemenu_button_label_mode = 0;
    SetPauseScreenButtonLabelPalettes();
    goto LABEL_7;
  }
  if ((newly_held_down_timed_held_input & kButton_R) != 0 && pausemenu_button_label_mode != 2) {
    pausemenu_start_lr_pressed_highlight_timer = 5;
    menu_index = 2;
    pausemenu_shoulder_button_highlight = 2;
    pausemenu_button_label_mode = 2;
    SetPauseScreenButtonLabelPalettes();
LABEL_7:
    QueueSfx1_Max6(0x38u);
  }
}
static const uint16 kPauseLrButtonPressedHighlight_Spritemap[2] = { 0x28, 0x29 };
static const uint16 kPauseLrButtonPressedHighlight_Y[2] = { 0x18, 0xe8 };
static const uint16 kPauseLrButtonPressedHighlight_X[2] = { 0xd0, 0xd0 };
void HandlePauseMenuLRPressHighlight(void) {  // 0x82A56D
  if (pausemenu_start_lr_pressed_highlight_timer) {
    --pausemenu_start_lr_pressed_highlight_timer;
    if (pausemenu_shoulder_button_highlight) {
      R3_.addr = 0;
      int v0 = pausemenu_shoulder_button_highlight - 1;
      DrawMenuSpritemap(
        kPauseLrButtonPressedHighlight_Spritemap[v0],
        kPauseLrButtonPressedHighlight_Y[v0],
        kPauseLrButtonPressedHighlight_X[v0] - 1);
    }
  }
}

void DrawLrHighlight(void) {  // 0x82A59A
  DrawPauseScreenSpriteAnim(2u, 0x18, 0xD0);
  DrawPauseScreenSpriteAnim(2u, 0xE8, 0xD0);
}

void HandlePauseScreenStart(void) {  // 0x82A5B7
  if ((newly_held_down_timed_held_input & kButton_Start) != 0) {
    QueueSfx1_Max6(0x38u);
    screen_fade_delay = 1;
    screen_fade_counter = 1;
    uint16 v0 = pausemenu_button_label_mode;
    pausemenu_button_label_mode = 1;
    SetPauseScreenButtonLabelPalettes();
    pausemenu_button_label_mode = v0;
    pausemenu_start_lr_pressed_highlight_timer = 11;
    ++game_state;
  }
  UpdatePauseMenuLRStartVramTilemap();
}

void HighlightPauseScreenButton(void) {  // 0x82A5F1
  if (pausemenu_start_lr_pressed_highlight_timer) {
    --pausemenu_start_lr_pressed_highlight_timer;
    R3_.addr = 0;
    DrawMenuSpritemap(0x2B, 0x90, 0xD0);
  }
}
static Func_V *const kSetPauseScreenButtonLabelPalettes_Funcs[3] = {  // 0x82A615
  SetPauseScreenButtonLabelPalettes_0,
  SetPauseScreenButtonLabelPalettes_1,
  SetPauseScreenButtonLabelPalettes_2,
};
void SetPauseScreenButtonLabelPalettes(void) {
  kSetPauseScreenButtonLabelPalettes_Funcs[pausemenu_button_label_mode]();
}

void SetPauseScreenButtonLabelPalettes_2(void) {  // 0x82A628
  int16 v0;
  int16 v2;
  int16 v4;
  int16 v6;
  int16 v8;
  int16 v10;

  v0 = 10;
  uint16 v1 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v1 + 805] = ram3000.pause_menu_map_tilemap[v1 + 805] & 0xE3FF | 0x800;
    ++v1;
    v0 -= 2;
  } while (v0);
  v2 = 10;
  uint16 v3 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v3 + 837] = ram3000.pause_menu_map_tilemap[v3 + 837] & 0xE3FF | 0x800;
    ++v3;
    v2 -= 2;
  } while (v2);
  v4 = 8;
  uint16 v5 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v5 + 812] = ram3000.pause_menu_map_tilemap[v5 + 812] & 0xE3FF | 0x800;
    ++v5;
    v4 -= 2;
  } while (v4);
  v6 = 8;
  uint16 v7 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v7 + 844] = ram3000.pause_menu_map_tilemap[v7 + 844] & 0xE3FF | 0x800;
    ++v7;
    v6 -= 2;
  } while (v6);
  v8 = 10;
  uint16 v9 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v9 + 822] = ram3000.pause_menu_map_tilemap[v9 + 822] & 0xE3FF | 0x1400;
    ++v9;
    v8 -= 2;
  } while (v8);
  v10 = 10;
  uint16 v11 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v11 + 854] = ram3000.pause_menu_map_tilemap[v11 + 854] & 0xE3FF | 0x1400;
    ++v11;
    v10 -= 2;
  } while (v10);
}

void SetPauseScreenButtonLabelPalettes_1(void) {  // 0x82A6DF
  int16 v0;
  int16 v2;
  int16 v4;
  int16 v6;
  int16 v8;
  int16 v10;

  v0 = 8;
  uint16 v1 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v1 + 812] = ram3000.pause_menu_map_tilemap[v1 + 812] & 0xE3FF | 0x800;
    ++v1;
    v0 -= 2;
  } while (v0);
  v2 = 8;
  uint16 v3 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v3 + 844] = ram3000.pause_menu_map_tilemap[v3 + 844] & 0xE3FF | 0x800;
    ++v3;
    v2 -= 2;
  } while (v2);
  v4 = 10;
  uint16 v5 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v5 + 805] = ram3000.pause_menu_map_tilemap[v5 + 805] & 0xE3FF | 0x1400;
    ++v5;
    v4 -= 2;
  } while (v4);
  v6 = 10;
  uint16 v7 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v7 + 837] = ram3000.pause_menu_map_tilemap[v7 + 837] & 0xE3FF | 0x1400;
    ++v7;
    v6 -= 2;
  } while (v6);
  v8 = 10;
  uint16 v9 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v9 + 822] = ram3000.pause_menu_map_tilemap[v9 + 822] & 0xE3FF | 0x1400;
    ++v9;
    v8 -= 2;
  } while (v8);
  v10 = 10;
  uint16 v11 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v11 + 854] = ram3000.pause_menu_map_tilemap[v11 + 854] & 0xE3FF | 0x1400;
    ++v11;
    v10 -= 2;
  } while (v10);
}

void SetPauseScreenButtonLabelPalettes_0(void) {  // 0x82A796
  int16 v0;
  int16 v2;
  int16 v4;
  int16 v6;
  int16 v8;
  int16 v10;

  v0 = 10;
  uint16 v1 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v1 + 822] = ram3000.pause_menu_map_tilemap[v1 + 822] & 0xE3FF | 0x800;
    ++v1;
    v0 -= 2;
  } while (v0);
  v2 = 10;
  uint16 v3 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v3 + 854] = ram3000.pause_menu_map_tilemap[v3 + 854] & 0xE3FF | 0x800;
    ++v3;
    v2 -= 2;
  } while (v2);
  v4 = 8;
  uint16 v5 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v5 + 812] = ram3000.pause_menu_map_tilemap[v5 + 812] & 0xE3FF | 0x800;
    ++v5;
    v4 -= 2;
  } while (v4);
  v6 = 8;
  uint16 v7 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v7 + 844] = ram3000.pause_menu_map_tilemap[v7 + 844] & 0xE3FF | 0x800;
    ++v7;
    v6 -= 2;
  } while (v6);
  v8 = 10;
  uint16 v9 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v9 + 805] = ram3000.pause_menu_map_tilemap[v9 + 805] & 0xE3FF | 0x1400;
    ++v9;
    v8 -= 2;
  } while (v8);
  v10 = 10;
  uint16 v11 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v11 + 837] = ram3000.pause_menu_map_tilemap[v11 + 837] & 0xE3FF | 0x1400;
    ++v11;
    v10 -= 2;
  } while (v10);
}

void UpdatePauseMenuLRStartVramTilemap(void) {  // 0x82A84D
  uint16 v0 = vram_write_queue_tail;
  gVramWriteEntry(vram_write_queue_tail)->size = 128;
  v0 += 2;
  gVramWriteEntry(v0)->size = 0x3640;
  v0 += 2;
  LOBYTE(gVramWriteEntry(v0++)->size) = 126;
  gVramWriteEntry(v0)->size = ((reg_BG2SC & 0xFC) << 8) + 800;
  vram_write_queue_tail = v0 + 2;
}

void DrawPauseScreenSpriteAnim(uint16 a, uint16 input_k, uint16 input_j) {  // 0x82A881
  int t = a - 1;
  uint16 *v8 = (uint16*)RomPtr_RAM(kPauseScreenSpriteAnimationData_1.arr[t]);
  uint16 *v5 = (uint16*)RomPtr_RAM(kPauseScreenSpriteAnimationData_0.arr[t]);
  const uint8 *v3 = RomPtr_82(kPauseScreenSpriteAnimationData_3.arr[t]);
  if ((int16)--(*v5) <= 0) {
    uint16 v10 = v3[3 * ++(*v8)];
    if (v10 == 255) {
      *v8 = 0;
      v10 = v3[0];
    }
    *v5 = v10;
  }
  R3_.addr = kPAuseSpritePaletteIndexValues[3];
  int r24 = v3[3 * *v8 + 2];
  int r26 = 2 * *RomPtr_RAM(kPauseScreenSpriteAnimationData_2.arr[t]);
  const uint8 *v11 = RomPtr_82(r26 + kPausePtsToAnimationSpritemapBaseIds[t]);
  DrawMenuSpritemap(r24 + GET_WORD(v11), input_k, input_j - 1);
}
void HandlePauseScreenPaletteAnimation(void) {  // 0x82A92B
  uint16 v2;
  uint16 j;
  uint8 v1;

  if ((uint8)pausemenu_palette_animation_timer) {
    LOBYTE(pausemenu_palette_animation_timer) = pausemenu_palette_animation_timer - 1;
    if (!(uint8)pausemenu_palette_animation_timer) {
      for (int i = pausemenu_palette_animation_frame + 1; ; i = 0) {
        LOBYTE(pausemenu_palette_animation_frame) = i;
        v1 = kPauseLrHighlightAnimData[(uint8)(3 * i)];
        if (v1 != 0xFF)
          break;
        QueueSfx3_Max6(0x2Au);
      }
      LOBYTE(pausemenu_palette_animation_timer) = v1;
      LOBYTE(v2) = 0;
      HIBYTE(v2) = pausemenu_palette_animation_frame;
      uint16 v3 = (v2 >> 3) + 30;
      for (j = 30; (j & 0x8000u) == 0; j -= 2) {
        palette_buffer[(j >> 1) + 176] = kPauseAnimatedPalette[v3 >> 1];
        v3 -= 2;
      }
    }
  }
}
void EquipmentScreenSetupReserveMode(void) {  // 0x82AB47
  VoidP v0;
  int16 v3;
  int16 v5;
  int16 v7;
  int16 v8;

  reg_BG4HOFS = reg_BG1HOFS;
  reg_BG4VOFS = reg_BG1VOFS;
  reg_BG1HOFS = 0;
  reg_BG1VOFS = 0;
  if (samus_max_reserve_health && reserve_health_mode) {
    v0 = addr_kEquipmentScreenTilemap_AUTO;
    if (reserve_health_mode != 1)
      v0 = addr_kEquipmentScreenTilemap_MANUAL;
    *(uint16 *)&R0_.bank = 130;
    R0_.addr = v0;
    R18_ = 4;
    uint16 v1 = 0;
    uint16 v2 = 0;
    do {
      v3 = ram3800.cinematic_bg_tilemap[v1 + 327] & 0xFC00;
      ram3800.cinematic_bg_tilemap[v1 + 327] = *(uint16 *)IndirPtr(&R0_, v2) | v3;
      v2 += 2;
      ++v1;
      --R18_;
    } while (R18_);
  }
  pausemenu_item_selector_animation_frame = 0;
  pausemenu_item_selector_animation_timer = kPauseLrHighlightAnimData[0];
  pausemenu_reserve_tank_animation_frame = 0;
  pausemenu_reserve_tank_animation_timer = kPauseReserveTankAnimationData[0];
  if (samus_max_reserve_health) {
    pausemenu_equipment_category_item = 0;
  } else if (hyper_beam_flag) {
LABEL_15:;
    uint16 v6 = 0;
    do {
      if ((kEquipmentBitmasks_Suits[v6 >> 1] & collected_items) != 0) {
        LOBYTE(v7) = (uint16)(v6 >> 1) >> 8;
        HIBYTE(v7) = v6 >> 1;
        pausemenu_equipment_category_item = v7 | 2;
        goto LABEL_21;
      }
      v6 += 2;
    } while ((int16)(v6 - 12) < 0);
    v8 = 0;
    do
      v8 += 2;
    while ((int16)(v8 - 6) < 0);
  } else {
    uint16 v4 = 0;
    while ((kEquipmentBitmasks_Weapons[v4 >> 1] & collected_beams) == 0) {
      v4 += 2;
      if ((int16)(v4 - 10) >= 0)
        goto LABEL_15;
    }
    LOBYTE(v5) = (uint16)(v4 >> 1) >> 8;
    HIBYTE(v5) = v4 >> 1;
    pausemenu_equipment_category_item = v5 | 1;
  }
LABEL_21:
  if (samus_reserve_health) {
    EquipmentScreenGlowingArrowSolidOn();
    WriteSamusWireframeTilemapAndQueue();
  }
}

void EquipmentScreenTransferBG1Tilemap(void) {  // 0x82AC22
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x30);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_82AC3B = { 1, 1, 0x18, LONGPTR(0x7e3800), 0x0800 };
  SetupDmaTransfer(&unk_82AC3B);
  WriteReg(MDMAEN, 2u);
  reg_BG1VOFS = 0;
}
static Func_V *const kEquipmentScreenCategories[4] = {  // 0x82AC4F
  EquipmentScreenCategory_Tanks,
  EquipmentScreenCategory_Weapons,
  EquipmentScreenCategory_Suit,
  EquipmentScreenCategory_Boots,
};
void EquipmentScreenMain(void) {
  kEquipmentScreenCategories[(uint8)pausemenu_equipment_category_item]();
  EquipmentScreenDrawItemSelector();
  EquipmentScreenDisplayReserveTankAmount();
  WriteSamusWireframeTilemapAndQueue();
}
static Func_V *const kEquipmentScreenCategory_TanksFuncs[2] = {  // 0x82AC70
  EquipmentScreenCategory_Tanks_0,
  EquipmentScreenCategory_Tanks_1,
};
void EquipmentScreenCategory_Tanks(void) {
  kEquipmentScreenCategory_TanksFuncs[HIBYTE(pausemenu_equipment_category_item)]();
  EquipmentScreenHandleDpad();
  EquipmentScreenGlowingArrow();
}

void EquipmentScreenHandleDpad(void) {  // 0x82AC8B
  R18_ = pausemenu_equipment_category_item;
  if ((joypad1_newkeys & kButton_Right) != 0) {
    if ((joypad1_newkeys & kButton_Down) != 0 || EquipmentScreenMoveLowerOnSuitsMisc(0) == 0xFFFF)
      EquipmentScreenMoveToHighJumpOrLowerInBoots(0);
  } else if ((joypad1_newkeys & kButton_Up) != 0) {
    if ((pausemenu_equipment_category_item & 0xFF00) != 0) {
      QueueSfx1_Max6(0x37u);
      pausemenu_equipment_category_item -= 256;
    }
  } else if ((joypad1_newkeys & kButton_Down) != 0) {
    if ((pausemenu_equipment_category_item & 0xFF00) == 256
        || reserve_health_mode == 1
        || (pausemenu_equipment_category_item += 256, !samus_reserve_health)) {
      EquipmentScreenMoveToBeams(0);
    } else {
      QueueSfx1_Max6(0x37u);
    }
  }
}
static Func_V *const kEquipmentScreenGlowingArrowFuncs[2] = {  // 0x82AD0A
  EquipmentScreenGlowingArrowAnimated,
  EquipmentScreenGlowingArrowSolidOn,
};

void EquipmentScreenGlowingArrow(void) {
  if ((uint8)pausemenu_equipment_category_item)
    EquipmentScreenGlowingArrowSolidOff();
  else
    kEquipmentScreenGlowingArrowFuncs[HIBYTE(pausemenu_equipment_category_item)]();
}

static const uint16 kEquipmentScreenGlowingArrowPalettes0[32] = { 0x39e, 0x77d, 0xb5c, 0xf5b, 0x133a, 0x171a, 0x1f19, 0x22f8, 0x26d7, 0x2ad6, 0x2eb6, 0x3695, 0x3a94, 0x3e73, 0x4253, 0x4a52, 0x4a52, 0x4253, 0x3e73, 0x3a94, 0x3695, 0x2eb6, 0x2ad6, 0x26d7, 0x22f8, 0x1f19, 0x171a, 0x133a, 0xf5b, 0xb5c, 0x77d, 0x39e };
static const uint16 kEquipmentScreenGlowingArrowPalettes1[32] = { 0x156, 0x155, 0x554, 0x954, 0xd53, 0xd52, 0x1152, 0x1551, 0x1970, 0x1d70, 0x1d6f, 0x216e, 0x256e, 0x296d, 0x296c, 0x318c, 0x318c, 0x296c, 0x296d, 0x256e, 0x216e, 0x1d6f, 0x1d70, 0x1970, 0x1551, 0x1152, 0xd52, 0xd53, 0x954, 0x554, 0x155, 0x156 };

void EquipmentScreenGlowingArrowAnimated(void) {  // 0x82AD29
  if (reserve_health_mode == 1) {
    int v0 = nmi_frame_counter_byte & 0x1F;
    palette_buffer[102] = kEquipmentScreenGlowingArrowPalettes0[v0];
    palette_buffer[107] = kEquipmentScreenGlowingArrowPalettes1[v0];
    EquipmentScreenEnergyArrowGlow_On();
  } else {
    palette_buffer[107] = 926;
    palette_buffer[102] = 342;
    EquipmentScreenEnergyArrowGlow_Off();
  }
}

void EquipmentScreenGlowingArrowSolidOn(void) {  // 0x82ADDD
  palette_buffer[107] = 926;
  palette_buffer[102] = 342;
  EquipmentScreenEnergyArrowGlow_On();
}

void EquipmentScreenGlowingArrowSolidOff(void) {  // 0x82ADEF
  palette_buffer[107] = 926;
  palette_buffer[102] = 342;
  EquipmentScreenEnergyArrowGlow_Off();
}

void EquipmentScreenEnergyArrowGlow_On(void) {  // 0x82AE01
  for (int i = 0; i < 8; i++)
    ram3800.equipment_screen_bg1_tilemap[129 + i * 32] = ram3800.equipment_screen_bg1_tilemap[129 + i * 32] & ~0x1C00 | 0x1800;
  for(int i = 0; i < 2; i++)
    ram3800.equipment_screen_bg1_tilemap[385 + i] = ram3800.equipment_screen_bg1_tilemap[385 + i] & ~0x1C00 | 0x1800;
}

void EquipmentScreenEnergyArrowGlow_Off(void) {  // 0x82AE46
  for(int i = 0; i < 8; i++)
    ram3800.equipment_screen_bg1_tilemap[129 + i * 32] = ram3800.equipment_screen_bg1_tilemap[129 + i * 32] & ~0x1C00 | 0x1C00;
  for (int i = 0; i < 2; i++)
    ram3800.equipment_screen_bg1_tilemap[385 + i] = ram3800.equipment_screen_bg1_tilemap[385 + i] & ~0x1C00 | 0x1C00;
}

static const uint16 kEquipmentScreenTilemap_MODE_MANUAL[7] = { 0x2519, 0x251a, 0x251b, 0x3d46, 0x3d47, 0x3d48, 0x3d49 };
static const uint16 kEquipmentScreenTilemap_RESERVE_TANK[7] = { 0x3c80, 0x3c81, 0x3c82, 0x3c83, 0x3c84, 0x3c85, 0x3c86 };
static const uint16 kEquipmentScreenTilemap_MANUAL[4] = { 0x3d46, 0x3d47, 0x3d48, 0x3d49 };
static const uint16 kEquipmentScreenTilemap_AUTO[4] = { 0x3d56, 0x3d57, 0x3d58, 0x3d59 };
static const uint16 kEquipmentScreenTilemap_CHARGE[5] = { 0x8ff, 0x8d8, 0x8d9, 0x8da, 0x8e7 };
static const uint16 kEquipmentScreenTilemap_ICE[5] = { 0x8ff, 0x8db, 0x8dc, 0x8d4, 0x8d4 };
static const uint16 kEquipmentScreenTilemap_WAVE[5] = { 0x8ff, 0x8dd, 0x8de, 0x8df, 0x8d4 };
static const uint16 kEquipmentScreenTilemap_SPAZER[5] = { 0x8ff, 0x8e8, 0x8e9, 0x8ea, 0x8eb };
static const uint16 kEquipmentScreenTilemap_PLASMA[5] = { 0x8ff, 0x8ec, 0x8ed, 0x8ee, 0x8ef };
static const uint16 kEquipmentScreenTilemap_VARIASUIT[9] = {  // 0x82AE8B
  0x8ff, 0x900, 0x901, 0x902, 0x903, 0x904, 0x905, 0x8d4,
  0x8d4,
};
static const uint16 kEquipmentScreenTilemap_GRAVITYSUIT[9] = {
  0x8ff, 0x8d0, 0x8d1, 0x8d2, 0x8d3, 0x903, 0x904, 0x905,
  0x8d4,
};
static const uint16 kEquipmentScreenTilemap_MORPHINGBALL[9] = {
  0x8ff, 0x920, 0x921, 0x922, 0x923, 0x917, 0x918, 0x90f,
  0x91f,
};
static const uint16 kEquipmentScreenTilemap_BOMBS[9] = {
  0x8ff, 0x8d5, 0x8d6, 0x8d7, 0x8d4, 0x8d4, 0x8d4, 0x8d4,
  0x8d4,
};
static const uint16 kEquipmentScreenTilemap_SPRING_BALL[9] = {
  0x8ff, 0x910, 0x911, 0x912, 0x913, 0x914, 0x915, 0x916,
  0x8d4,
};
static const uint16 kEquipmentScreenTilemap_SCREW_ATTACK[9] = {
  0x8ff, 0x8e0, 0x8e1, 0x8e2, 0x8e3, 0x8e4, 0x8e5, 0x8e6,
  0x8d4,
};
static const uint16 kEquipmentScreenTilemap_HIJUMP_BOOTS[9] = {
  0x8ff, 0x930, 0x931, 0x932, 0x933, 0x934, 0x935, 0x936,
  0x8d4,
};
static const uint16 kEquipmentScreenTilemap_SPACE_JUMP[9] = {
  0x8ff, 0x8f0, 0x8f1, 0x8f2, 0x8f3, 0x8f4, 0x8f5, 0x8d4,
  0x8d4,
};
static const uint16 kEquipmentScreenTilemap_SPEED_BOOSTER[9] = {
  0x8ff, 0x924, 0x925, 0x926, 0x927, 0x928, 0x929, 0x92a,
  0x92b,
};
static const uint16 kEquipmentScreenTilemap_HYPER[9] = {
  0x8ff, 0x937, 0x938, 0x939, 0x92f, 0x8d4, 0x8d4, 0x8d4,
  0x8d4,
};
static const uint16 kEquipmentScreenTilemap_Blank[9] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0,
};
void EquipmentScreenCategory_Tanks_0(void) {
  int16 v0;
  int16 v2;

  if ((joypad1_newkeys & kButton_A) != 0 && samus_max_reserve_health) {
    QueueSfx1_Max6(0x37u);
    if (reserve_health_mode == 1) {
      reserve_health_mode = 2;
      EquipmentScreenHudReserveAutoTilemap_Off();
      v0 = 8;
      uint16 v1 = 0;
      do {
        ram3800.cinematic_bg_tilemap[v1 + 327] = kEquipmentScreenTilemap_MANUAL[v1] | ram3800.cinematic_bg_tilemap[v1 + 327] & 0xFC00;
        ++v1;
        v0 -= 2;
      } while (v0);
    } else {
      reserve_health_mode = 1;
      EquipmentScreenHudReserveAutoTilemap_On_BUGGY();
      v2 = 8;
      uint16 v3 = 0;
      do {
        ram3800.cinematic_bg_tilemap[v3 + 327] = kEquipmentScreenTilemap_AUTO[v3] | ram3800.cinematic_bg_tilemap[v3 + 327] & 0xFC00;
        ++v3;
        v2 -= 2;
      } while (v2);
    }
  }
}

void EquipmentScreenHudReserveAutoTilemap_On_BUGGY(void) {  // 0x82AEFD
  // loads garbage...
  uint16 v0 = -26229;
  if (!samus_reserve_health)
    v0 = -26217;
  const uint16 *v1 = (const uint16 *)RomPtr_82(v0);
  hud_tilemap[8] = *v1;
  hud_tilemap[9] = v1[1];
  hud_tilemap[40] = v1[2];
  hud_tilemap[41] = v1[3];
  hud_tilemap[72] = v1[4];
  hud_tilemap[73] = v1[5];
}

void EquipmentScreenHudReserveAutoTilemap_Off(void) {  // 0x82AF33
  hud_tilemap[8] = 11279;
  hud_tilemap[9] = 11279;
  hud_tilemap[40] = 11279;
  hud_tilemap[41] = 11279;
  hud_tilemap[72] = 11279;
  hud_tilemap[73] = 11279;
}
static const uint16 kReserveTankEnergyTransferPerFrame = 1;
void EquipmentScreenCategory_Tanks_1(void) {  // 0x82AF4F
  if (!pausemenu_reserve_tank_delay_ctr) {
    if ((joypad1_newkeys & kButton_A) == 0)
      return;
    pausemenu_reserve_tank_delay_ctr = (samus_reserve_health + 7) & 0xFFF8;
  }
  if ((--pausemenu_reserve_tank_delay_ctr & 7) == 7)
    QueueSfx3_Max6(0x2Du);
  samus_health += kReserveTankEnergyTransferPerFrame;
  if ((int16)(samus_health - samus_max_health) < 0) {
    samus_reserve_health -= kReserveTankEnergyTransferPerFrame;
    if ((int16)samus_reserve_health > 0)
      return;
    samus_health += samus_reserve_health;
  } else {
    samus_health = samus_max_health;
  }
  samus_reserve_health = 0;
  pausemenu_reserve_tank_delay_ctr = 0;
  EquipmentScreenEnergyArrowGlow_Off();
  pausemenu_equipment_category_item = 0;
}

void EquipmentScreenCategory_Weapons(void) {  // 0x82AFBE
  EquipmentScreenCategory_Weapons_MoveButtons();
  R36 = equipped_beams;
  if (collected_beams) {
    R24_ = 10;
    // Fixed var bug
    if ((uint8)pausemenu_equipment_category_item == 1) {
      EquipmentScreenCategory_ButtonResponse();
      EquipmentScreenCategory_Weapons_PlazmaSpazerCheck();
    }
  }
}

void EquipmentScreenCategory_Weapons_MoveButtons(void) {  // 0x82AFDB
  R18_ = pausemenu_equipment_category_item;
  if ((joypad1_newkeys & 0x100) != 0) {
    if ((joypad1_newkeys & 0x800) != 0) {
      EquipmentScreenMoveLowerOnSuitsMisc(0);
    } else if (EquipmentScreenMoveLowerOnSuitsMisc(4u)) {
      EquipmentScreenMoveToHighJumpOrLowerInBoots(0);
    }
  } else if ((joypad1_newkeys & 0x400) != 0) {
    if (!hyper_beam_flag && pausemenu_equipment_category_item != 1025) {
      pausemenu_equipment_category_item += 256;
      EquipmentScreenMoveToBeams(2 * HIBYTE(pausemenu_equipment_category_item));
    }
  } else if ((joypad1_newkeys & 0x800) != 0) {
    if ((pausemenu_equipment_category_item & 0xFF00) == 0
        || (pausemenu_equipment_category_item -= 256,
            EquipmentScreenMoveToBottomOfBeams(2 * HIBYTE(pausemenu_equipment_category_item)) == 0xFFFF)) {
      if (!EquipmentScreenMoveToReserveTanks())
        pausemenu_equipment_category_item = R18_;
    }
  }
}

void EquipmentScreenCategory_Weapons_PlazmaSpazerCheck(void) {  // 0x82B068
  int t = equipped_beams & ~R36;
  if ((t & 4) != 0) {
    if ((R36 & 4) == 0 && (equipped_beams & 8) != 0) {
      equipped_beams &= ~8;
      R0_.addr = kEquipmentTilemapOffs_Weapons[4];
      goto LABEL_9;
    }
  } else if ((t & 8) != 0 && (R36 & 8) == 0 && (equipped_beams & 4) != 0) {
    equipped_beams &= ~4;
    R0_.addr = kEquipmentTilemapOffs_Weapons[3];
LABEL_9:
    R18_ = 3072;
    R22_ = 10;
    SetPaletteOfR22TilemapBytesToR18();
  }
}

void EquipmentScreenCategory_Suit(void) {  // 0x82B0C2
  EquipmentScreenCategory_Suit_MoveResponse();
  R24_ = 18;
  // Fixed var bug
  if ((uint8)pausemenu_equipment_category_item == 2)
    EquipmentScreenCategory_ButtonResponse();
}

void EquipmentScreenCategory_Suit_MoveResponse(void) {  // 0x82B0D2
  R18_ = pausemenu_equipment_category_item;
  if ((joypad1_newkeys & kButton_Left) != 0) {
    if ((joypad1_newkeys & kButton_Down) == 0) {
      if (EquipmentScreenMoveToReserveTanks())
        return;
      pausemenu_equipment_category_item = R18_;
    }
    EquipmentScreenMoveToBeams(0);
  } else if ((joypad1_newkeys & kButton_Up) != 0) {
    if ((pausemenu_equipment_category_item & 0xFF00) != 0) {
      pausemenu_equipment_category_item -= 256;
      EquipmentScreenMoveToScrewOrHigherOnSuits(2 * HIBYTE(pausemenu_equipment_category_item));
    }
  } else if ((joypad1_newkeys & kButton_Down) != 0) {
    if ((pausemenu_equipment_category_item & 0xFF00) == 1280
        || (pausemenu_equipment_category_item += 256,
            EquipmentScreenMoveLowerOnSuitsMisc(2 * HIBYTE(pausemenu_equipment_category_item)) == 0xFFFF)) {
      EquipmentScreenMoveToHighJumpOrLowerInBoots(0);
    }
  }
}

void EquipmentScreenCategory_Boots(void) {  // 0x82B150
  EquipmentScreenCategory_Boots_MoveResponse();
  R24_ = 18;
  // Fixed var bug
  if ((uint8)pausemenu_equipment_category_item == 3)
    EquipmentScreenCategory_ButtonResponse();
}

void EquipmentScreenCategory_Boots_MoveResponse(void) {  // 0x82B160
  R18_ = pausemenu_equipment_category_item;
  if ((joypad1_newkeys & kButton_Left) != 0) {
    if (((joypad1_newkeys & kButton_Up) != 0 || EquipmentScreenMoveToBottomOfBeams(8u) == 0xFFFF)
        && !EquipmentScreenMoveToReserveTanks()) {
      pausemenu_equipment_category_item = R18_;
    }
  } else if ((joypad1_newkeys & kButton_Down) != 0) {
    if (pausemenu_equipment_category_item != 515) {
      pausemenu_equipment_category_item += 256;
      EquipmentScreenMoveToHighJumpOrLowerInBoots(2 * HIBYTE(pausemenu_equipment_category_item));
    }
  } else if ((joypad1_newkeys & kButton_Up) != 0) {
    if ((pausemenu_equipment_category_item & 0xFF00) == 0
        || (pausemenu_equipment_category_item -= 256,
            EquipmentScreenCategory_Boots_MoveUpInBoots(2 * HIBYTE(pausemenu_equipment_category_item)) == 0xFFFF)) {
      EquipmentScreenMoveToScrewOrHigherOnSuits(0xAu);
    }
  }
}

void WriteSamusWireframeTilemapAndQueue(void) {  // 0x82B1E0
  WriteSamusWireframeTilemap();
  uint16 v0 = vram_write_queue_tail;
  gVramWriteEntry(vram_write_queue_tail)->size = 1280;
  v0 += 2;
  gVramWriteEntry(v0)->size = ADDR16_OF_RAM(ram3800) + 256;
  v0 += 2;
  LOBYTE(gVramWriteEntry(v0++)->size) = 126;
  gVramWriteEntry(v0)->size = 0x3080;
  vram_write_queue_tail = v0 + 2;
}


void WriteSamusWireframeTilemap(void) {  // 0x82B20C
  uint16 i;

  for (i = 0; (equipped_items & 0x101) != kEquipmentScreenWireframeCmp[i >> 1]; i += 2)
    ;
  R0_.addr = kEquipmentScreenWireframePtrs[i >> 1];
  *(uint16 *)&R0_.bank = 130;
  R20_ = 17;
  uint16 v1 = 0;
  uint16 v2 = 472;
  do {
    uint16 v3 = v2;
    R18_ = 8;
    do {
      *(uint16 *)((char *)ram3800.cinematic_bg_tilemap + v2) = *(uint16 *)IndirPtr(&R0_, v1);
      v2 += 2;
      v1 += 2;
      --R18_;
    } while (R18_);
    v2 = v3 + 64;
    --R20_;
  } while (R20_);
}


void EquipmentScreenDrawItemSelector(void) {  // 0x82B267
  if (samus_max_reserve_health | (uint16)(collected_items | collected_beams)) {
    uint16 *t = (uint16*)RomPtr_82(kEquipmentScreenPtrsToItemXYpos[(uint8)pausemenu_equipment_category_item] + 
        4 * HIBYTE(pausemenu_equipment_category_item));
    DrawPauseScreenSpriteAnim(3, t[0] - 1, t[1]);
  }
}

void EquipmentScreenDisplayReserveTankAmount(void) {  // 0x82B2A2
  EquipmentScreenDisplayReserveTankAmount_();
}
static const uint16 kEquipmentScreenReserveTank_X[6] = {  // 0x82B2AA
  0x18, 0x20,
  0x28, 0x30,
  0x38, 0x40,
};
static const uint16 kEquipmentScreenReserveTank_Y = 0x60;
static const uint16 kPartialReserveTankSpritemapIds[16] = { 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27 };

void EquipmentScreenDisplayReserveTankAmount_(void) {
  uint16 a;

  R3_.addr = 0;
  EquipmentScreenDisplayReserves_PaletteSetup();
  bg3_tilemap_offset = 0;
  if (samus_max_reserve_health) {
    R44 = samus_max_reserve_health / 100;

    int mod_value = samus_reserve_health % 100;
    R50 = mod_value;
    R42 = samus_reserve_health / 100;

    R48 = R42;
    R46 = R42;
    if (R42) {
      uint16 v0 = 0;
      do {
        a = v0;
        DrawMenuSpritemap(0x1B, kEquipmentScreenReserveTank_X[v0 >> 1], kEquipmentScreenReserveTank_Y - 1);
        v0 += 2;
        --R46;
      } while (R46);
      bg3_tilemap_offset = a + 2;
    }
    uint16 RegWord = mod_value;
    if (RegWord) {
      uint16 v2 = 2 * (RegWord / 14);
      if (sign16(v2 - 7) && RegWord % 14 && (nmi_frame_counter_byte & 4) == 0)
        v2 += 2;
      if (!sign16(samus_reserve_health - 100))
        v2 += 16;
      DrawMenuSpritemap(
        kPartialReserveTankSpritemapIds[v2 >> 1],
        kEquipmentScreenReserveTank_X[bg3_tilemap_offset >> 1],
        kEquipmentScreenReserveTank_Y - 1);
      ++R48;
      ++bg3_tilemap_offset;
      ++bg3_tilemap_offset;
    }
    while (sign16(R48 - R44)) {
      DrawMenuSpritemap(
        0x20u,
        kEquipmentScreenReserveTank_X[bg3_tilemap_offset >> 1],
        kEquipmentScreenReserveTank_Y - 1);
      ++bg3_tilemap_offset;
      ++bg3_tilemap_offset;
      ++R48;
    }
    DrawMenuSpritemap(
      0x1Fu,
      kEquipmentScreenReserveTank_X[bg3_tilemap_offset >> 1],
      kEquipmentScreenReserveTank_Y - 1);
    int div_val = R50 / 10;
    int mod_val = R50 % 10;
    ram3800.cinematic_bg_tilemap[394] = mod_val + 2052;
    ram3800.cinematic_bg_tilemap[393] = div_val + 2052;
    ram3800.cinematic_bg_tilemap[392] = R42 + 2052;
  }
}

void EquipmentScreenDisplayReserves_PaletteSetup(void) {  // 0x82B3F9
  R3_.addr = 1536;
  if (samus_reserve_health) {
    bool v0 = (--pausemenu_reserve_tank_animation_timer & 0x8000u) != 0;
    if (!pausemenu_reserve_tank_animation_timer || v0) {
      ++pausemenu_reserve_tank_animation_frame;
      uint16 v1 = kPauseReserveTankAnimationData[(uint16)(2 * pausemenu_reserve_tank_animation_frame)];
      if (v1 == 255) {
        pausemenu_reserve_tank_animation_frame = 0;
        v1 = kPauseReserveTankAnimationData[0];
      }
      pausemenu_reserve_tank_animation_timer = v1;
    }
    R3_.addr = kPAuseSpritePaletteIndexValues[3];
  }
}

uint16 EquipmentScreenMoveToReserveTanks(void) {  // 0x82B43F
  uint16 result = samus_max_reserve_health;
  if (samus_max_reserve_health) {
    pausemenu_equipment_category_item = 0;
    QueueSfx1_Max6(0x37u);
    return 1;
  }
  return result;
}

void EquipmentScreenMoveToBeams(uint16 v0) {  // 0x82B456
  int16 v1;

  if (hyper_beam_flag) {
LABEL_4:
    pausemenu_equipment_category_item = R18_;
  } else {
    while ((kEquipmentBitmasks_Weapons[v0 >> 1] & collected_beams) == 0) {
      v0 += 2;
      if ((int16)(v0 - 10) >= 0)
        goto LABEL_4;
    }
    LOBYTE(v1) = (uint16)(v0 >> 1) >> 8;
    HIBYTE(v1) = v0 >> 1;
    pausemenu_equipment_category_item = v1 & 0xFF00 | 1;
    QueueSfx1_Max6(0x37u);
  }
}

uint16 EquipmentScreenMoveToBottomOfBeams(uint16 k) {  // 0x82B489
  int16 v2;

  if (hyper_beam_flag)
    return -1;
  while ((kEquipmentBitmasks_Weapons[k >> 1] & collected_beams) == 0) {
    k -= 2;
    if ((k & 0x8000u) != 0)
      return -1;
  }
  QueueSfx1_Max6(0x37u);
  LOBYTE(v2) = (uint16)(k >> 1) >> 8;
  HIBYTE(v2) = k >> 1;
  uint16 result = v2 & 0xFF00 | 1;
  pausemenu_equipment_category_item = result;
  return result;
}

uint16 EquipmentScreenMoveLowerOnSuitsMisc(uint16 v0) {  // 0x82B4B7
  int16 v1;

  while ((kEquipmentBitmasks_Suits[v0 >> 1] & collected_items) == 0) {
    v0 += 2;
    if ((int16)(v0 - 10) >= 0)
      return -1;
  }
  QueueSfx1_Max6(0x37u);
  LOBYTE(v1) = (uint16)(v0 >> 1) >> 8;
  HIBYTE(v1) = v0 >> 1;
  pausemenu_equipment_category_item = v1 & 0xFF00 | 2;
  return 0;
}

void EquipmentScreenMoveToScrewOrHigherOnSuits(uint16 v0) {  // 0x82B4E6
  int16 v1;

  while ((kEquipmentBitmasks_Suits[v0 >> 1] & collected_items) == 0) {
    v0 -= 2;
    if ((v0 & 0x8000u) != 0) {
      pausemenu_equipment_category_item = R18_;
      return;
    }
  }
  QueueSfx1_Max6(0x37u);
  LOBYTE(v1) = (uint16)(v0 >> 1) >> 8;
  HIBYTE(v1) = v0 >> 1;
  pausemenu_equipment_category_item = v1 & 0xFF00 | 2;
}

void EquipmentScreenMoveToHighJumpOrLowerInBoots(uint16 v0) {  // 0x82B511
  int16 v1;

  while ((kEquipmentBitmasks_Boots[v0 >> 1] & collected_items) == 0) {
    v0 += 2;
    if ((int16)(v0 - 6) >= 0) {
      pausemenu_equipment_category_item = R18_;
      return;
    }
  }
  QueueSfx1_Max6(0x37u);
  LOBYTE(v1) = (uint16)(v0 >> 1) >> 8;
  HIBYTE(v1) = v0 >> 1;
  pausemenu_equipment_category_item = v1 & 0xFF00 | 3;
}

uint16 EquipmentScreenCategory_Boots_MoveUpInBoots(uint16 k) {  // 0x82B53F
  int16 v2;

  while ((kEquipmentBitmasks_Boots[k >> 1] & collected_items) == 0) {
    k -= 2;
    if ((k & 0x8000u) != 0)
      return -1;
  }
  QueueSfx1_Max6(0x37u);
  LOBYTE(v2) = (uint16)(k >> 1) >> 8;
  HIBYTE(v2) = k >> 1;
  uint16 result = v2 & 0xFF00 | 3;
  pausemenu_equipment_category_item = result;
  return result;
}
void EquipmentScreenCategory_ButtonResponse(void) {  // 0x82B568
  if ((joypad1_newkeys & kButton_A) != 0) {
    QueueSfx1_Max6(0x38u);
    R26_ = 2 * (uint8)pausemenu_equipment_category_item;
    R18_ = 2 * HIBYTE(pausemenu_equipment_category_item);
    int item = HIBYTE(pausemenu_equipment_category_item);
    int category = (uint8)pausemenu_equipment_category_item;
    R0_.addr = *(uint16 *)RomPtr_82(kEquipmentPtrsToRamTilemapOffsets[category] + item * 2);
    uint16 *var = (uint16 *)RomPtr_RAM(kEquipmentPtrsToBitsets[category]);
    uint16 mask = *(uint16 *)RomPtr_82(kEquipmentPtrsToBitmasks[category] + item * 2);
    if ((*var & mask) != 0) {
      *var &= ~mask;
      R18_ = 0xC00;
      R22_ = R24_;
      SetPaletteOfR22TilemapBytesToR18();
    } else {
      *var |= mask;
      R22_ = R24_;
      Copy_R22_Bytes(*(uint16 *)RomPtr_82(kEquipmentPtrsToEquipmentTilemaps[category] + item * 2));
    }
  }
}


void DrawPauseMenuDuringFadeIn(void) {  // 0x82B62B
  if (menu_index) {
    EquipmentScreenDrawItemSelector();
    EquipmentScreenDisplayReserveTankAmount();
  } else {
    MapScreenDrawSamusPositionIndicator();
    DrawMapIcons();
    DisplayMapElevatorDestinations();
  }
}

void UNKNOWN_sub_82B650(void) {  // 0x82B650
  if (pausemenu_button_label_mode) {
    DisplayMapElevatorDestinations();
    MapScreenDrawSamusPositionIndicator();
    DrawMapIcons();
  } else {
    EquipmentScreenDrawItemSelector();
    EquipmentScreenDisplayReserveTankAmount();
  }
}
static const uint16 kMap_Criteria_SavePoints[16] = { 0xd8, 0x28, 0x90, 0x38, 0xfffe, 0xfffe, 0xfffe, 0xfffe, 0xfffe, 0xfffe, 0xfffe, 0xfffe, 0xfffe, 0xfffe, 0xfffe, 0xfffe };
void DrawMapIcons(void) {  // 0x82B672
  DrawBossMapIcons(9u, addr_kMapIconDataPointers);
  R3_.addr = 3584;
  DrawSimpleMapIcons(0xB, addr_kMapIconDataPointers + 0x10);
  DrawSimpleMapIcons(0xA, addr_kMapIconDataPointers + 0x20);
  DrawSimpleMapIcons(0x4E, addr_kMapIconDataPointers + 0x30);
  R3_.addr = 1024;
  DrawSaveStationMapIcon(8u, 0xC80B);
  if (enable_debug)
    DrawSimpleMapIcons(8u, 0xC82B);
  if (!area_index) {
    R3_.addr = 3584;
    DrawMenuSpritemap(
      0x63u,
      kMap_Criteria_SavePoints[0] - reg_BG1HOFS,
      kMap_Criteria_SavePoints[1] - reg_BG1VOFS);
  }
}
void DrawFileSelectMapIcons(void) {  // 0x82B6DD
  int16 v0;

  HandlePauseScreenPaletteAnimation();
  DrawBossMapIcons(9u, 0xC7CB);
  R3_.addr = 3584;
  DrawSimpleMapIcons(0xB, 0xC7DB);
  DrawSimpleMapIcons(0xA, 0xC7EB);
  DrawSimpleMapIcons(0x4E, 0xC7FB);
  R3_.addr = 3584;
  uint16 a = UpdateSamusPositionIndicatorAnimation();
  *(uint16 *)&R0_.bank = 130;
  R0_.addr = *(VoidP *)((char *)&kMapIconDataPointers[4].crateria + (uint16)(2 * area_index));
  v0 = 4 * load_station_index;
  uint16 v1 = *(uint16 *)IndirPtr(&R0_, 4 * load_station_index) - reg_BG1HOFS;
  uint16 v2 = *(uint16 *)IndirPtr(&R0_, v0 + 2) - reg_BG1VOFS;
  if ((samus_position_indicator_animation_loop_counter & 1) == 0)
    DrawMenuSpritemap(0x12, v1, v2);
  DrawMenuSpritemap(a, v1, v2);
  if (enable_debug) {
    R3_.addr = 1536;
    DrawDebugSaveMapIcons(0xC, 0xC80B);
    DrawDebugElevatorMapIcons(0x17, 0xC81B);
    DrawSimpleMapIcons(0xC, 0xC82B);
  }
  if (!area_index) {
    R3_.addr = 3584;
    DrawMenuSpritemap(
      0x63u,
      kMap_Criteria_SavePoints[0] - reg_BG1HOFS,
      kMap_Criteria_SavePoints[1] - reg_BG1VOFS);
  }
}

void DrawSaveStationMapIcon(uint16 a, uint16 k) {  // 0x82B798
  static const uint8 kShlBit[8] = { 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80 };
  R34 = a;
  if (area_index == sram_area_index) {
    R36 = *(uint16 *)&used_save_stations_and_elevators[(uint16)(2 * area_index)];
    LOBYTE(R36) = R36 & kShlBit[load_station_index];
    uint16 v2 = *(uint16 *)RomPtr_82(k + 2 * area_index);
    if (v2)
      DrawMapIconsOfType(v2);
  }
}

void DrawDebugSaveMapIcons(uint16 a, uint16 k) {  // 0x82B7D1
  R34 = a;
  R36 = *(uint16 *)&used_save_stations_and_elevators[(uint16)(2 * area_index)];
  uint16 v2 = *(uint16 *)RomPtr_82(k + 2 * area_index);
  if (v2)
    DrawMapIconsOfType(v2);
}

void DrawDebugElevatorMapIcons(uint16 a, uint16 k) {  // 0x82B7EB
  R34 = a;
  R36 = *(uint16 *)&used_save_stations_and_elevators[(uint16)(2 * area_index) + 1];
  uint16 v2 = *(uint16 *)RomPtr_82(k + 2 * area_index);
  if (v2)
    DrawMapIconsOfType(v2);
}

void DrawSimpleMapIcons(uint16 a, uint16 k) {  // 0x82B805
  R34 = a;
  R36 = -1;
  uint16 v2 = *(uint16 *)RomPtr_82(k + 2 * area_index);
  if (v2)
    DrawMapIconsOfType(v2);
}

void DrawMapIconsOfType(uint16 a) {  // 0x82B81C
  char v3; // cf
  int16 v4;

  while (1) {
    const uint16 *v2 = (const uint16 *)RomPtr_82(a);
    if ((*v2 & 0x8000u) != 0)
      break;
    v3 = R36 & 1;
    R36 >>= 1;
    if (v3) {
      v4 = CheckIfMapPositionIsExplored(v2[0], v2[1]);
      if (v4)
        DrawMenuSpritemap(R34, v2[0] - reg_BG1HOFS, v2[1] - reg_BG1VOFS);
    }
    a += 4;
  }
}

uint16 CheckIfMapPositionIsExplored(uint16 k, uint16 j) {  // 0x82B855
  static const uint8 kBits0x80Shr[8] = { 0x80, 0x40, 0x20, 0x10, 8, 4, 2, 1 };
  uint16 v2;
  uint16 v4; // dx

  R18_ = (uint16)(k & 0xFF00) >> 1;
  R18_ += (uint8)k >> 6;
  R18_ += (uint16)(j & 0xFFF8) >> 1;
  uint16 v3 = (k >> 3) & 7;
  HIBYTE(v2) = HIBYTE(v3);
  LOBYTE(v2) = map_tiles_explored[R18_];
  HIBYTE(v4) = HIBYTE(v3);
  LOBYTE(v4) = kBits0x80Shr[v3];
  return (v4 & v2) != 0;
}

void DrawBossMapIcons(uint16 a, uint16 k) {  // 0x82B892
  int bits = boss_bits_for_area[area_index];
  int t = *(uint16 *)RomPtr_82(k + 2 * area_index);
  if (t == 0)
    return;
  const uint16 *v4 = (const uint16 *)RomPtr_82(t);
  for (; ; v4 += 2) {
    if (v4[0] == 0xffff)
      return;
    if (v4[0] != 0xfffe) {
      R3_.addr = 3584;
      int v5 = bits & 1;
      bits >>= 1;
      if (v5) {
        DrawMenuSpritemap(0x62, v4[0] - reg_BG1HOFS, v4[1] - reg_BG1VOFS);
        R3_.addr = 3072;
        DrawMenuSpritemap(a, v4[0] - reg_BG1HOFS, v4[1] - reg_BG1VOFS);
        continue;
      }
      if (has_area_map) {
        DrawMenuSpritemap(a, v4[0] - reg_BG1HOFS, v4[1] - reg_BG1VOFS);
        continue;
      }
    }
    bits >>= 1;
  }
}

void DrawMapScrollArrowAndCheckToScroll(uint8 db, uint16 k) {  // 0x82B90A
  const uint16 *v1 = (const uint16 *)RomPtrWithBank(db, k);
  DrawPauseScreenSpriteAnim(v1[2], *v1, v1[1]);
  const uint8 *v2 = RomPtrWithBank(db, k);
  if ((joypad1_lastkeys & GET_WORD(v2 + 6)) != 0 && !map_scrolling_direction)
    map_scrolling_direction = GET_WORD(v2 + 8);
}

void sub_82B932(void) {  // 0x82B932
  HandleMapScrollArrows();
}


void HandleMapScrollArrows(void) {  // 0x82B934
  if (sign16(map_min_x_scroll - 24 - reg_BG1HOFS))
    DrawMapScrollArrowAndCheckToScroll(0x82, addr_stru_82B9A0);
  if (!sign16(map_max_x_scroll - 232 - reg_BG1HOFS))
    DrawMapScrollArrowAndCheckToScroll(0x82, addr_stru_82B9AA);
  if (sign16(map_min_y_scroll - 56 - reg_BG1VOFS))
    DrawMapScrollArrowAndCheckToScroll(0x82, addr_stru_82B9B4);
  if (sign16(map_max_y_scroll - 177 - reg_BG1VOFS)) {
    if (map_scrolling_direction == g_stru_82B9BE.map_scroll_dir) {
      map_scrolling_gear_switch_timer = 0;
      map_scrolling_direction = 0;
      map_scrolling_speed_index = 0;
    }
  } else {
    DrawMapScrollArrowAndCheckToScroll(0x82, addr_stru_82B9BE);
  }
}

void MapScreenDrawSamusPositionIndicator(void) {  // 0x82B9C8
  R3_.addr = 3584;
  uint16 a = UpdateSamusPositionIndicatorAnimation();
  DrawMenuSpritemap(
    a,
    8 * (room_x_coordinate_on_map + HIBYTE(samus_x_pos)) - reg_BG1HOFS,
    8 * (room_y_coordinate_on_map + HIBYTE(samus_y_pos) + 1) - reg_BG1VOFS);
}

static const uint16 kMap_SamusPositionIndicator_Delays[4] = { 8, 4, 8, 4 };
static const uint16 kMap_SamusPositionIndicator_SpritemapIds[4] = { 0x5f, 0x60, 0x61, 0x60 };

uint16 UpdateSamusPositionIndicatorAnimation(void) {  // 0x82B9FC
  uint16 v0 = samus_position_indicator_animation_timer;
  if (!v0) {
    uint16 v1 = samus_position_indicator_animation_frame + 2;
    if ((int16)(v1 - 8) >= 0) {
      ++samus_position_indicator_animation_loop_counter;
      v1 = 0;
    }
    samus_position_indicator_animation_frame = v1;
    v0 = kMap_SamusPositionIndicator_Delays[v1 >> 1];
    samus_position_indicator_animation_timer = v0;
  }
  samus_position_indicator_animation_timer = v0 - 1;
  return kMap_SamusPositionIndicator_SpritemapIds[samus_position_indicator_animation_frame >> 1];
}

void DrawBorderAroundSamusData(void) {  // 0x82BA35
  R3_.addr = 3584;
  DrawMenuSpritemap(0x48, 0x80, 0x10);
}

void DrawBorderAroundDataCopyMode(void) {  // 0x82BA48
  R3_.addr = 3584;
  DrawMenuSpritemap(0x49, 0x80, 0x10);
}

void DrawBorderAroundDataClearMode(void) {  // 0x82BA5B
  R3_.addr = 3584;
  DrawMenuSpritemap(0x4A, 0x7C, 0x10);
}
static const uint16 kDrawMenuSelectionMissile_Enable[4] = { 8, 8, 8, 8 };
static const uint16 kDrawMenuSelectionMissile_SpriteMap[4] = { 0x37, 0x36, 0x35, 0x34 };
void DrawMenuSelectionMissile(void) {  // 0x82BA6E
  if (enemy_projectile_enable_flag) {
    if (!--enemy_projectile_enable_flag) {
      enemy_projectile_id[0] = (LOBYTE(enemy_projectile_id[0]) + 1) & 3;
      enemy_projectile_enable_flag = kDrawMenuSelectionMissile_Enable[enemy_projectile_id[0]];
    }
  }
  R3_.addr = 3584;
  DrawMenuSpritemap(
    kDrawMenuSelectionMissile_SpriteMap[enemy_projectile_id[0]],
    enemy_projectile_id[5],
    enemy_projectile_id[10]);
}
void DrawFileCopyArrow(void) {  // 0x82BABA
  uint16 v0;
  if ((int16)(enemy_projectile_id[16] - enemy_projectile_id[17]) >= 0) {
    v0 = 3;
    if (sign16(enemy_projectile_id[16] - enemy_projectile_id[17] - 2)) {
      v0 = 4;
      if (enemy_projectile_id[16] != 1)
        v0 = 5;
    }
  } else {
    v0 = 0;
    if (sign16(enemy_projectile_id[17] - enemy_projectile_id[16] - 2)) {
      v0 = 1;
      if (enemy_projectile_id[16])
        v0 = 2;
    }
  }
  R3_.addr = 512;
  DrawMenuSpritemap(
    file_copy_arrow_stuff[v0].spritemap,
    file_copy_arrow_stuff[v0].xpos,
    file_copy_arrow_stuff[v0].ypos);
}
void DisplayMapElevatorDestinations(void) {  // 0x82BB30
  R3_.addr = 0;
  if (map_station_byte_array[area_index]) {
    for (int i = kMapElevatorDests[area_index]; ; i += 6) {
      const uint8 *v1 = RomPtr_82(i);
      if (GET_WORD(v1) == 0xFFFF)
        break;
      DrawMenuSpritemap(GET_WORD(v1 + 4), GET_WORD(v1) - reg_BG1HOFS, GET_WORD(v1 + 2) - reg_BG1VOFS);
    }
  }
}

void HandleGameOverBabyMetroid(void) {  // 0x82BB75
  if (enemy_data[0].instruction_timer)
    sub_82BB7F(enemy_data[0].instruction_timer);
  else
    sub_82BBDD();
}

void CallBabyMetroidPlaySfx(uint32 ea) {
  switch (ea) {
  case fnBabyMetroidPlaySfx0x23: BabyMetroidPlaySfx0x23(); return;
  case fnBabyMetroidPlaySfx0x26: BabyMetroidPlaySfx0x26(); return;
  case fnBabyMetroidPlaySfx0x27: BabyMetroidPlaySfx0x27(); return;
  default: Unreachable();
  }
}

void sub_82BB7F(uint16 a) {  // 0x82BB7F
  uint16 current_instruction = enemy_data[0].current_instruction;
  enemy_data[0].instruction_timer = a - 1;
  if (a == 1) {
    const uint8 *v2 = RomPtr_82(enemy_data[0].current_instruction);
    if (GET_WORD(v2 + 6) == 0xFFFF) {
      sub_82BBDD();
    } else if ((int16)(GET_WORD(v2 + 6) + 1) >= 0) {
      enemy_data[0].instruction_timer = GET_WORD(v2 + 6);
      enemy_data[0].current_instruction += 6;
      DrawBabyMetroid(current_instruction + 6);
    } else {
      R18_ = GET_WORD(v2 + 6);
      CallBabyMetroidPlaySfx(R18_ | 0x820000);
    }
  } else {
    DrawBabyMetroid(enemy_data[0].current_instruction);
  }
}

void sub_82BBDD(void) {  // 0x82BBDD
  enemy_data[0].current_instruction = addr_kgameOverBabyMetridInstructionList;
  enemy_data[0].instruction_timer = 10;
  sub_82BB7F(0xAu);
}

void DrawBabyMetroid(uint16 k) {  // 0x82BB9E
  uint16 v1 = *((uint16 *)RomPtr_82(k) + 2);
  for(int i = 0; i < 32; i += 2)
    palette_buffer[(i >> 1) + 192] = *(uint16 *)RomPtr_82(v1 + i);
  R3_.addr = 2048;
  const uint8 *v3 = RomPtr_82(k);
  DrawMenuSpritemap(GET_WORD(v3 + 2), 0x7C, 0x50);
  R3_.addr = 2560;
  DrawMenuSpritemap(0x64, 0x7C, 0x50);
}

void FinishProcessingGameOverBabyMetroidAsm(void) {  // 0x82BBF0
  uint16 t = *((uint16 *)RomPtr_82(enemy_data[0].current_instruction) + 4);
  enemy_data[0].instruction_timer = t;
  enemy_data[0].current_instruction += 8;
  if (t == 0xFFFF)
    sub_82BBDD();
  else
    DrawBabyMetroid(enemy_data[0].current_instruction);
}

void BabyMetroidPlaySfx0x23(void) {  // 0x82BC0C
  QueueSfx3_Max6(0x23u);
  FinishProcessingGameOverBabyMetroidAsm();
}

void BabyMetroidPlaySfx0x26(void) {  // 0x82BC15
  QueueSfx3_Max6(0x26u);
  FinishProcessingGameOverBabyMetroidAsm();
}

void BabyMetroidPlaySfx0x27(void) {  // 0x82BC1E
  QueueSfx3_Max6(0x27u);
  FinishProcessingGameOverBabyMetroidAsm();
}

void CancelSoundEffects(void) {  // 0x82BE17
  QueueSfx1_Max6(2u);
  QueueSfx2_Max6(0x71u);
  QueueSfx3_Max6(1u);
}

void QueueSamusMovementSfx(void) {  // 0x82BE2F
  if ((speed_boost_counter & 0xFF00) == 1024)
    QueueSfx3_Max6(0x2Bu);
  if (!sign16(flare_counter - 16))
    QueueSfx1_Max6(0x41u);
  CallSomeSamusCode(0x14u);
}


uint8 AdvancePaletteFadeForAllPalettes_0xc(void) {  // 0x82D961
  palette_change_denom = 12;
  return AdvancePaletteFadeForAllPalettes();
}

uint8 AdvancePaletteFade_BgPalette6(void) {  // 0x82D96C
  palette_change_denom = 12;
  if (palette_change_num <= 0xDu) {
    for (int i = 192; i < 0xE0u; i += 2)
      palette_buffer[i >> 1] = CalculateNthTransitionColorFromXtoY(
        palette_change_num,
        palette_buffer[i >> 1],
        target_palettes[i >> 1]);
    ++palette_change_num;
    return 0;
  } else {
    palette_change_num = 0;
    return 1;
  }
}

uint8 AdvanceGradualColorChangeOfPalette(uint16 k, uint16 j) {  // 0x82D9B8
  palette_change_denom = 15;
  if ((int16)(15 - palette_change_num) >= 0) {
    R34 = j;
    do {
      palette_buffer[k >> 1] = CalculateNthTransitionColorFromXtoY(
        palette_change_num + 1,
        palette_buffer[k >> 1],
        target_palettes[k >> 1]);
      k += 2;
    } while (k < R34);
    ++palette_change_num;
    return 0;
  } else {
    palette_change_num = 0;
    return 1;
  }
}

uint8 AdvancePaletteFadeForAllPalettes(void) {  // 0x82DA02
  if ((uint16)(palette_change_denom + 1) >= palette_change_num) {
    for (int i = 0; i < 0x200u; i += 2) {
      g_word_7EC404 = i;
      int v2 = i >> 1;
      if (target_palettes[v2] != palette_buffer[v2]) {
        uint16 v3 = CalculateNthTransitionColorFromXtoY(palette_change_num, palette_buffer[v2], target_palettes[v2]);
        i = g_word_7EC404;
        palette_buffer[g_word_7EC404 >> 1] = v3;
      }
    }
    ++palette_change_num;
    return 0;
  } else {
    palette_change_num = 0;
    return 1;
  }
}

uint16 CalculateNthTransitionColorFromXtoY(uint16 a, uint16 k, uint16 j) {  // 0x82DA4A
  int16 v3;
  char v4; // t0
  int16 v8;

  uint16 v7 = CalculateNthTransitionColorComponentFromXtoY(a, k & 0x1F, j & 0x1F);
  v8 = v7 | (32 * CalculateNthTransitionColorComponentFromXtoY(a, (k >> 5) & 0x1F, (j >> 5) & 0x1F));
  v3 = 4
    * CalculateNthTransitionColorComponentFromXtoY(
      a,
      ((uint16)(k >> 2) >> 8) & 0x1F,
      ((uint16)(j >> 2) >> 8) & 0x1F);
  v4 = v3;
  LOBYTE(v3) = HIBYTE(v3);
  HIBYTE(v3) = v4;
  return v8 | v3;
}

uint16 CalculateNthTransitionColorComponentFromXtoY(uint16 a, uint16 k, uint16 j) {  // 0x82DAA6
  int16 v4;
  char v6; // t0
  int16 v8;

  if (!a)
    return k;
  v4 = a - 1;
  if (v4 == palette_change_denom)
    return j;
  R20_ = v4 + 1;
  R18_ = j - k;
  uint16 v5 = abs16(j - k);
  v6 = v5;
  LOBYTE(v5) = HIBYTE(v5);
  HIBYTE(v5) = v6;
  uint16 RegWord = SnesDivide(v5 & 0xFF00, palette_change_denom - R20_ + 1);
  R18_ = sign16(R18_) ? -RegWord : RegWord;
  LOBYTE(v8) = HIBYTE(k);
  HIBYTE(v8) = k;
  return (uint16)(R18_ + v8) >> 8;
}

uint8 sub_82DAF7(uint16 a) {  // 0x82DAF7
  palette_change_denom = 12;
  return sub_82DB0C(a);
}

uint8 sub_82DB0C(uint16 a) {  // 0x82DB0C
  if ((uint16)(palette_change_denom + 1) >= palette_change_num) {
    g_word_7EC404 = 0;
    while (a) {
      if (!(a & 1))
        g_word_7EC404 += 32;
      else
        sub_82DB41();
      a >>= 1;
    }
    ++palette_change_num;
    return 0;
  } else {
    palette_change_num = 0;
    return 1;
  }
}


void sub_82DB41(void) {  // 0x82DB41
  uint16 v0 = g_word_7EC404;
  do {
    int v1 = v0 >> 1;
    if (target_palettes[v1] != palette_buffer[v1]) {
      uint16 v2 = CalculateNthTransitionColorFromXtoY(palette_change_num, palette_buffer[v1], target_palettes[v1]);
      v0 = g_word_7EC404;
      palette_buffer[g_word_7EC404 >> 1] = v2;
    }
    v0 += 2;
    g_word_7EC404 = v0;
  } while ((v0 & 0x1F) != 0);
}

void HandleSamusOutOfHealthAndGameTile(void) {  // 0x82DB69
  if ((int16)samus_health <= 0) {
    if ((reserve_health_mode & 1) != 0 && samus_reserve_health) {
      time_is_frozen_flag = 0x8000;
      game_state = kGameState_27_ReserveTanksAuto;
      CallSomeSamusCode(0x1Bu);
    } else {
      if (game_state != kGameState_8_MainGameplay)
        return;
      time_is_frozen_flag = 0x8000;
      CallSomeSamusCode(0x11u);
      game_state = kGameState_19_SamusNoHealth;
    }
  }
  if (!sign16(++game_time_frames - 60)) {
    game_time_frames = 0;
    if (!sign16(++game_time_seconds - 60)) {
      game_time_seconds = 0;
      if (!sign16(++game_time_minutes - 60)) {
        game_time_minutes = 0;
        ++game_time_hours;
      }
    }
  }
  if (!sign16(game_time_hours - 100)) {
    game_time_frames = 59;
    game_time_seconds = 59;
    game_time_minutes = 59;
    game_time_hours = 99;
  }
}

CoroutineRet GameState_27_ReserveTanksAuto(void) {  // 0x82DC10
  // todo: Ugly to inspect GameState_8_MainGameplay's coroutine_state_1 like this.
  if (coroutine_state_1 == 0 && RefillHealthFromReserveTanks()) {
    time_is_frozen_flag = 0;
    game_state = kGameState_8_MainGameplay;
    CallSomeSamusCode(0x10u);
  }
  COROUTINE_AWAIT_ONLY(GameState_8_MainGameplay());
  Samus_LowHealthCheck_0();
  return kCoroutineNone;
}

uint8 RefillHealthFromReserveTanks(void) {  // 0x82DC31
  if (samus_reserve_health) {
    if ((nmi_frame_counter_word & 7) == 0)
      QueueSfx3_Max3(0x2Du);
    if ((int16)(++samus_health - samus_max_health) >= 0) {
      samus_health = samus_max_health;
LABEL_9:
      samus_reserve_health = 0;
      return samus_reserve_health == 0;
    }
    bool v0 = (--samus_reserve_health & 0x8000u) != 0;
    if (!samus_reserve_health)
      goto LABEL_9;
    if (v0) {
      samus_health += samus_reserve_health;
      goto LABEL_9;
    }
  }
  return samus_reserve_health == 0;
}

CoroutineRet GameState_19_SamusNoHealth(void) {  // 0x82DC80
  COROUTINE_AWAIT_ONLY(GameState_8_MainGameplay());
  for (int i = 255; i >= 0; --i)
    ram3000.pause_menu_map_tilemap[i + 384] = palette_buffer[i];
  for (int j = 382; (j & 0x8000u) == 0; j -= 2)
    target_palettes[j >> 1] = 0;
  for (int k = 94; (k & 0x8000u) == 0; k -= 2)
    target_palettes[(k >> 1) + 208] = 0;
  for (int m = 30; (m & 0x8000u) == 0; m -= 2)
    target_palettes[(m >> 1) + 192] = palette_buffer[(m >> 1) + 192];
  game_options_screen_index = 3;
  g_word_7E0DE4 = 0;
  g_word_7E0DE6 = 0;
  g_word_7E0DE8 = 0;
  hud_item_index = 0;
  samus_auto_cancel_hud_item_index = 0;
  samus_invincibility_timer = 0;
  samus_knockback_timer = 0;
  ++game_state;

  return kCoroutineNone;
}

CoroutineRet GameState_20_SamusNoHealth_BlackOut(void) {  // 0x82DCE0
  COROUTINE_AWAIT_ONLY(GameState_8_MainGameplay());
  palette_change_denom = 6;
  if (AdvancePaletteFadeForAllPalettes()) {
    WaitUntilEndOfVblankAndClearHdma();
    DisableIrqInterrupts();
    fx_layer_blending_config_a = 0;
    cur_irq_handler = 0;
    next_gameplay_CGWSEL = 0;
    next_gameplay_CGADSUB = 0;
    reg_TM = 16;
    reg_TS = 0;
    reg_TMW = 0;
    reg_TSW = 0;
    reg_BGMODE = 9;
    game_options_screen_index = 0;
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    for (int i = 254; i >= 0; i -= 2)
      enemy_projectile_y_subpos[(i >> 1) + 15] = 0;
    g_word_7E0DE8 = 16;
    game_options_screen_index = 3;
    g_word_7E0DE4 = 0;
    g_word_7E0DE6 = 0;
    ++game_state;
    power_bomb_explosion_status = 0;
    QueueSfx1_Max15(2u);
    QueueSfx2_Max15(0x71u);
    QueueSfx3_Max15(1u);
    QueueMusic_Delayed8(0);
    QueueMusic_Delayed8(0xFF39u);
    QueueMusic_DelayedY(5u, 0xE);
  }
  return kCoroutineNone;
}

CoroutineRet GameState_21_SamusNoHealth(void) {  // 0x82DD71
  Samus_DrawWhenNotAnimatingOrDying();
  if (!HasQueuedMusic()) {
    StartSamusDeathAnimation();
    ++game_state;
  }
  return kCoroutineNone;
}

CoroutineRet GameState_22_SamusNoHealth_Dying(void) {  // 0x82DD87
  DrawSamusStartingDeathAnim_();
  bool v0 = (--g_word_7E0DE8 & 0x8000u) != 0;
  if (!g_word_7E0DE8 || v0)
    ++game_state;
  return kCoroutineNone;
}

CoroutineRet GameState_23_SamusNoHealth_Flashing(void) {  // 0x82DD9A
  if (HandleSamusDeathSequence())
    ++game_state;
  else
    Samus_DrawDuringDeathAnim();
  return kCoroutineNone;
}

CoroutineRet GameState_24_SamusNoHealth_Explosion(void) {  // 0x82DDAF
  if (GameState_24_SamusNoHealth_Explosion_Helper()) {
    screen_fade_delay = 1;
    screen_fade_counter = 1;
    ++game_state;
  }
  return kCoroutineNone;
}

CoroutineRet GameState_25_SamusNoHealth_BlackOut(void) {  // 0x82DDC7
  HandleFadeOut();
  if (reg_INIDISP == 0x80) {
    EnableNMI();
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    ++game_state;
    menu_index = 0;
    debug_disable_sounds = 0;
  }
  return kCoroutineNone;
}

void LoaadDesinationRoomCreBitset(void) {  // 0x82DDF1
  uint16 room_definition_ptr = get_DoorDef(door_def_ptr)->room_definition_ptr;
  previous_cre_bitset = cre_bitset;
  cre_bitset = get_RoomDefHeader(room_definition_ptr)->cre_bitset_;
}

void LoadDoorHeader(void) {  // 0x82DE12
  DoorDef *DoorDef;
  int16 samus_distance_from_door;

  DoorDef = get_DoorDef(door_def_ptr);
  room_ptr = DoorDef->room_definition_ptr;
  elevator_door_properties_orientation = *(uint16 *)&DoorDef->door_bitflags;
  elevator_flags = elevator_door_properties_orientation & 0x80;
  door_direction = DoorDef->door_orientation;
  door_destination_x_pos = DoorDef->x_pos_in_room << 8;
  door_destination_y_pos = DoorDef->y_pos_in_room << 8;
  R18_ = 0;
  R20_ = 0;
  samus_distance_from_door = DoorDef->samus_distance_from_door;
  if (samus_distance_from_door < 0) {
    if ((door_direction & 2) != 0)
      samus_distance_from_door = 384;
    else
      samus_distance_from_door = 200;
  }
  R19_ = samus_distance_from_door;
  samus_door_transition_subspeed = R18_;
  samus_door_transition_speed = R20_;
}

void LoadRoomHeader(void) {  // 0x82DE6F
  RoomDefHeader *RoomDefHeader;

  RoomDefHeader = get_RoomDefHeader(room_ptr);
  room_index = RoomDefHeader->semiunique_room_number;
  area_index = RoomDefHeader->area_index_;
  room_x_coordinate_on_map = RoomDefHeader->x_coordinate_on_map;
  room_y_coordinate_on_map = RoomDefHeader->y_coordinate_on_map;
  room_width_in_scrolls = RoomDefHeader->width;
  room_width_in_blocks = 16 * room_width_in_scrolls;
  room_height_in_scrolls = RoomDefHeader->height;
  room_height_in_blocks = 16 * room_height_in_scrolls;
  up_scroller = RoomDefHeader->up_scroller_;
  down_scroller = RoomDefHeader->down_scroller_;
  door_list_pointer = RoomDefHeader->ptr_to_doorout;
  HandleRoomDefStateSelect(room_ptr);
  uint16 prod = Mult8x8(room_width_in_blocks, room_height_in_blocks);
  room_size_in_blocks = 2 * prod;
}
void LoadStateHeader(void) {  // 0x82DEF2
  StateHeaderTiles *StateHeaderTiles; // r10
  RoomDefRoomstate *RoomDefRoomstate; // r11

  int v0 = (uint16)(2 * get_RoomDefRoomstate(roomdefroomstate_ptr)->graphics_set) >> 1;
  StateHeaderTiles = get_StateHeaderTiles(kStateHeaderGraphicsSets[v0]);
  copy24(&tileset_tile_table_pointer, &StateHeaderTiles->tile_table_ptr);
  copy24(&tileset_tiles_pointer, &StateHeaderTiles->tiles_ptr);
  copy24(&tileset_compr_palette_ptr, &StateHeaderTiles->palette_ptr);
  RoomDefRoomstate = get_RoomDefRoomstate(roomdefroomstate_ptr);
  copy24(&room_compr_level_data_ptr, &RoomDefRoomstate->compressed_room_map_ptr);
  room_music_data_index = RoomDefRoomstate->music_track;
  room_music_track_index = RoomDefRoomstate->music_control;
  room_layer3_asm_ptr = RoomDefRoomstate->room_layer3_fx_ptr;
  room_enemy_population_ptr = RoomDefRoomstate->enemy_population_ptr_;
  room_enemy_tilesets_ptr = RoomDefRoomstate->enemy_tilesets_ptr;
  *(uint16 *)&layer2_scroll_x = RoomDefRoomstate->vertical_screen_nudge_limit;
  room_main_code_ptr = RoomDefRoomstate->main_code_ptr;
}

void WaitUntilEndOfVblankAndEnableIrq(void) {  // 0x82DF69
  if ((reg_NMITIMEN & 0x30) != 0x30)
    EnableIrqInterruptsNow();
}

void PointlessFunctionStupidToo(void) {  // 0x82DF80
}

void SaveMapExploredifElevator(void) {  // 0x82DF99
  if ((elevator_door_properties_orientation & 0xF) != 0)
    SetElevatorsAsUsed();
  if ((get_DoorDef(door_def_ptr)->door_bitflags & 0x40) != 0)
    SaveExploredMapTilesToSaved();
}

void LoadMapExploredIfElevator(void) {  // 0x82DFB6
  if ((get_DoorDef(door_def_ptr)->door_bitflags & 0x40) != 0)
    LoadMirrorOfExploredMapTiles();
}

void EnsureSamusDrawnEachFrame(void) {  // 0x82DFC7
  if (!elevator_flags)
    Samus_DrawWhenNotAnimatingOrDying();
}

void LoadEnemyGfxToVram(void) {  // 0x82DFD1
  EnemyDef_A2 *EnemyDef_A2;

  R18_ = 28672;
  uint16 v0 = room_enemy_tilesets_ptr;
  if (room_enemy_tilesets_ptr) {
    for (int i = room_enemy_tilesets_ptr; ; v0 = i) {
      uint16 enemy_def = get_EnemyTileset(v0)->enemy_def;
      if (enemy_def == 0xFFFF)
        break;
      EnemyDef_A2 = get_EnemyDef_A2(enemy_def);
      copy24(&door_transition_vram_update_src, &EnemyDef_A2->tile_data);
      if ((EnemyDef_A2->tile_data_size & 0x8000u) != 0) {
        door_transition_vram_update_size = EnemyDef_A2->tile_data_size & 0x7FFF;
        door_transition_vram_update_dst = ((uint16)(get_EnemyTileset(i)->vram_dst & 0xF000) >> 4) | 0x6000;
      } else {
        door_transition_vram_update_size = EnemyDef_A2->tile_data_size;
        door_transition_vram_update_dst = R18_;
        R18_ += EnemyDef_A2->tile_data_size >> 1;
      }
      WaitForIrqDoorTransitionVramUpdate();
      i += 4;
    }
  }
}

void CopyToVramAtNextInterrupt(const void *p) {  // 0x82E039
  const CopyToVramAtNextInterruptArgs *pp = (const CopyToVramAtNextInterruptArgs *)p;
  door_transition_vram_update_src = pp->source;
  door_transition_vram_update_dst = pp->dest;
  door_transition_vram_update_size = pp->size;
  WaitForIrqDoorTransitionVramUpdate();
}

void LoadRoomMusic(void) {  // 0x82E071
  if (game_state < kGameState_40_TransitionToDemo && room_music_data_index && room_music_data_index != music_data_index) {
    QueueMusic_Delayed8(0);
    QueueMusic_Delayed8((uint8)room_music_data_index | 0xFF00);
  }
}

void UpdateMusicTrackIndex(void) {  // 0x82E09B
  if (game_state < kGameState_40_TransitionToDemo && room_music_track_index) {
    R18_ = (uint8)room_music_data_index << 8;
    R18_ |= room_music_track_index;
    R20_ = (uint8)music_data_index << 8;
    R20_ |= music_track_index;
    if (R18_ != R20_)
      music_track_index = room_music_track_index;
  }
}

void LoadNewMusicTrackIfChanged(void) {  // 0x82E0D5
  if (game_state < 0x28u && room_music_track_index) {
    R18_ = (uint8)room_music_data_index << 8;
    R18_ |= room_music_track_index;
    R20_ = (uint8)music_data_index << 8;
    R20_ |= music_track_index;
    if (R18_ != R20_)
      QueueMusic_DelayedY(room_music_track_index, 6u);
  }
}

void NullFunc(void) {  // 0x82E113
  ;
}

void PlayRoomMusicTrackAfterAFrames(uint16 a) {  // 0x82E118
  if (game_state < kGameState_40_TransitionToDemo) {
    QueueMusic_DelayedY(0, a);
    QueueMusic_Delayed8(music_track_index);
  }
}

void LoadColorsForSpritesBeamsAndEnemies(void) {  // 0x82E139
  uint16 j;
  uint16 k;

  for (int i = 30; i >= 0; i -= 2)
    target_palettes[(i >> 1) + 208] = kInitialPalette[(i >> 1) + 208];
  for (j = 30; (j & 0x8000u) == 0; j -= 2)
    target_palettes[(j >> 1) + 224] = palette_buffer[(j >> 1) + 224];
  for (k = 30; (k & 0x8000u) == 0; k -= 2)
    target_palettes[(k >> 1) + 128] = kCommonSpritesPalette1[k >> 1];
}

CoroutineRet CallDoorTransitionFunction_Async(uint32 ea) {
  switch (ea) {
  case fnDoorTransitionFunction_HandleElevator: return DoorTransitionFunction_HandleElevator();
  case fnDoorTransitionFunction_Wait48frames: return DoorTransitionFunction_Wait48frames();
  case fnDoorTransitionFunction_WaitForSoundsToFinish: return DoorTransitionFunction_WaitForSoundsToFinish();
  case fnDoorTransitionFunction_FadeOutScreen: return DoorTransitionFunction_FadeOutScreen();
  case fnDoorTransitionFunction_LoadDoorHeaderEtc: return DoorTransitionFunction_LoadDoorHeaderEtc();
  case fnDoorTransitionFunction_ScrollScreenToAlignment: return DoorTransitionFunction_ScrollScreenToAlignment();
  case fnDoorTransitionFunction_FixDoorsMovingUp: return DoorTransitionFunction_FixDoorsMovingUp();
  case fnDoorTransitionFunction_SetupNewRoom: return DoorTransitionFunction_SetupNewRoom();
  case fnDoorTransitionFunction_SetupScrolling: return DoorTransitionFunction_SetupScrolling();
  case fnDoorTransitionFunction_PlaceSamusLoadTiles: return DoorTransitionFunction_PlaceSamusLoadTiles();
  case fnDoorTransitionFunction_LoadMoreThings_Async: return DoorTransitionFunction_LoadMoreThings_Async();
  case fnDoorTransition_C_HandleAnimTiles: return DoorTransition_C_HandleAnimTiles();
  case fnDoorTransition_WaitForMusicToClear: return DoorTransition_WaitForMusicToClear();
  case fnDoorTransition_HandleTransition: return DoorTransition_HandleTransition();
  case fnDoorTransition_FadeInScreenAndFinish: return DoorTransition_FadeInScreenAndFinish();
  default: return Unreachable();
  }
}

CoroutineRet GameState_9_HitDoorBlock(void) {  // 0x82E169
  return CallDoorTransitionFunction_Async(door_transition_function | 0x820000);
}

CoroutineRet DoorTransitionFunction_HandleElevator(void) {  // 0x82E17D
  if (!elevator_flags) {
    ++game_state;
    return GameState_10_LoadingNextRoom_Async();
  }
  CallSomeSamusCode(0);
  if ((elevator_direction & 0x8000u) != 0) {
    ++game_state;
    return GameState_10_LoadingNextRoom_Async();
  }
  downwards_elevator_delay_timer = 48;
  door_transition_function = FUNC16(DoorTransitionFunction_Wait48frames);
  return DoorTransitionFunction_Wait48frames();
}

CoroutineRet DoorTransitionFunction_Wait48frames(void) {  // 0x82E19F
  if ((--downwards_elevator_delay_timer & 0x8000u) != 0) {
    ++game_state;
    return GameState_10_LoadingNextRoom_Async();
  } else {
    DetermineWhichEnemiesToProcess();
    EnemyMain();
    DrawSamusEnemiesAndProjectiles();
    EnsureSamusDrawnEachFrame();
  }
  return kCoroutineNone;
}

CoroutineRet GameState_10_LoadingNextRoom_Async(void) {  // 0x82E1B7
  door_transition_flag_enemies = 1;
  door_transition_flag_elevator_zebetites = 1;
  debug_disable_minimap = 0;
  save_station_lockout_flag = 0;
  DetermineWhichEnemiesToProcess();
  EnemyMain();
  DrawSamusEnemiesAndProjectiles();
  EnsureSamusDrawnEachFrame();
  LoaadDesinationRoomCreBitset();
  for (int i = 254; i >= 0; i -= 2) {
    int v1 = i >> 1;
    target_palettes[v1] = 0;
    target_palettes[v1 + 128] = 0;
  }
  target_palettes[9] = palette_buffer[9];
  target_palettes[10] = palette_buffer[10];
  target_palettes[13] = palette_buffer[13];
  target_palettes[14] = palette_buffer[14];
  target_palettes[17] = palette_buffer[17];
  target_palettes[18] = palette_buffer[18];
  target_palettes[19] = palette_buffer[19];
  target_palettes[29] = palette_buffer[29];
  if (((previous_cre_bitset | cre_bitset) & 1) == 0) {
    target_palettes[20] = palette_buffer[20];
    target_palettes[21] = palette_buffer[21];
    target_palettes[22] = palette_buffer[22];
    target_palettes[23] = palette_buffer[23];
    target_palettes[28] = palette_buffer[28];
    if (timer_status) {
      target_palettes[209] = palette_buffer[209];
      target_palettes[210] = palette_buffer[210];
      target_palettes[212] = palette_buffer[212];
      target_palettes[221] = palette_buffer[221];
      DrawTimer();
    }
  }
  ClearSoundsWhenGoingThroughDoor();
  QueueSfx2_Max15(0x71u);
  debug_disable_sounds = -1;
  door_transition_function = FUNC16(DoorTransitionFunction_WaitForSoundsToFinish);
  ++game_state;
  return kCoroutineNone;
}

CoroutineRet GameState_11_LoadingNextRoom_Async(void) {  // 0x82E288
  COROUTINE_AWAIT_ONLY(CallDoorTransitionFunction_Async(door_transition_function | 0x820000));
  if (timer_status)
    DrawTimer();
  return kCoroutineNone;
}

CoroutineRet DoorTransitionFunction_WaitForSoundsToFinish(void) {  // 0x82E29E
  DetermineWhichEnemiesToProcess();
  EnemyMain();
  DrawSamusEnemiesAndProjectiles();
  EnsureSamusDrawnEachFrame();
  if (((sfx_writepos[0] - sfx_readpos[0]) & 0xF) == 0
      && ((sfx_writepos[1] - sfx_readpos[1]) & 0xF) == 0
      && ((sfx_writepos[2] - sfx_readpos[2]) & 0xF) == 0) {
    door_transition_function = FUNC16(DoorTransitionFunction_FadeOutScreen);
  }
  return kCoroutineNone;
}

CoroutineRet DoorTransitionFunction_FadeOutScreen(void) {  // 0x82E2DB
  if (AdvancePaletteFadeForAllPalettes_0xc()) {
    door_transition_function = FUNC16(DoorTransitionFunction_LoadDoorHeaderEtc);
  } else {
    DetermineWhichEnemiesToProcess();
    EnemyMain();
    DrawSamusEnemiesAndProjectiles();
    EnsureSamusDrawnEachFrame();
  }
  return kCoroutineNone;
}

CoroutineRet DoorTransitionFunction_LoadDoorHeaderEtc(void) {  // 0x82E2F7
  LoadDoorHeader();
  sub_8882AC();
  hdma_objects_enable_flag &= ~0x8000u;
  irqhandler_next_handler = 8;
  door_transition_function = FUNC16(DoorTransitionFunction_ScrollScreenToAlignment);
  return kCoroutineNone;
}

CoroutineRet DoorTransitionFunction_ScrollScreenToAlignment(void) {  // 0x82E310
  if ((door_direction & 2) != 0) {
    if ((uint8)layer1_x_pos) {
      if ((layer1_x_pos & 0x80) != 0)
        ++layer1_x_pos;
      else
        --layer1_x_pos;
      goto LABEL_10;
    }
  } else if ((uint8)layer1_y_pos) {
    if ((layer1_y_pos & 0x80) != 0)
      ++layer1_y_pos;
    else
      --layer1_y_pos;
LABEL_10:
    CalculateLayer2PosAndScrollsWhenScrolling();
    return kCoroutineNone;
  }
  CalculateLayer2PosAndScrollsWhenScrolling();
  door_transition_function = FUNC16(DoorTransitionFunction_FixDoorsMovingUp);
  return kCoroutineNone;
}

CoroutineRet DoorTransitionFunction_FixDoorsMovingUp(void) {  // 0x82E353
  if ((door_direction & 3) == 3)
    FixDoorsMovingUp();
  door_transition_function = FUNC16(DoorTransitionFunction_SetupNewRoom);
  return kCoroutineNone;
}

CoroutineRet DoorTransitionFunction_SetupNewRoom(void) {  // 0x82E36E
  SaveMapExploredifElevator();
  LoadRoomHeader();
  LoadStateHeader();
  LoadMapExploredIfElevator();
  InitializeSpecialEffectsForNewRoom();
  LoadLevelScrollAndCre();
  door_transition_function = FUNC16(DoorTransitionFunction_SetupScrolling);
  return kCoroutineNone;
}

CoroutineRet DoorTransitionFunction_SetupScrolling(void) {  // 0x82E38E
  reg_BG2HOFS = 0;
  reg_BG2VOFS = 0;
  scrolling_finished_hook = 0;
  if ((door_direction & 3) == 2)
    ++reg_BG1VOFS;
  if ((door_direction & 3) != 3)
    door_transition_frame_counter = 0;
  DoorTransitionScrollingSetup();
  door_transition_function = FUNC16(DoorTransitionFunction_PlaceSamusLoadTiles);
  return kCoroutineNone;
}

CoroutineRet DoorTransitionFunction_PlaceSamusLoadTiles(void) {  // 0x82E3C0
  static const LongPtr unk_82E421 = LONGPTR(0x7e7000);
  static const LongPtr unk_82E432 = LONGPTR(0x7e2000);
  static const LongPtr unk_82E443 = LONGPTR(0x7ec200);
  static const CopyToVramAtNextInterruptArgs unk_82E449 = { LONGPTR(0x7e2000), 0x0000, 0x2000 };
  static const CopyToVramAtNextInterruptArgs unk_82E453 = { LONGPTR(0x7e4000), 0x1000, 0x2000 };
  static const CopyToVramAtNextInterruptArgs unk_82E45D = { LONGPTR(0x7e6000), 0x2000, 0x1000 };
  static const CopyToVramAtNextInterruptArgs unk_82E477 = { LONGPTR(0x7e7000), 0x2800, 0x1000 };
  static const CopyToVramAtNextInterruptArgs unk_82E481 = { LONGPTR(0x7e8000), 0x3000, 0x2000 };
  static const CopyToVramAtNextInterruptArgs unk_82E48B = { LONGPTR(0x9ab200), 0x4000, 0x1000 };
  uint16 v0;

  samus_x_pos = layer1_x_pos + (uint8)samus_x_pos;
  samus_prev_x_pos = samus_x_pos;
  samus_y_pos = layer1_y_pos + (uint8)samus_y_pos;
  samus_prev_y_pos = samus_y_pos;
  door_transition_flag = 0;
  door_transition_vram_update_enabled = 0;
  if ((door_direction & 3) == 2)
    v0 = 16;
  else
    v0 = 22;
  irqhandler_next_handler = v0;
  WaitUntilEndOfVblankAndEnableIrq();
  if ((cre_bitset & 2) != 0 && door_def_ptr != addr_kDoorDef_947a) {
    mov24(&decompress_src, 0xb98000);
    DecompressToMem_IpArg(&unk_82E421);
  }
  copy24(&decompress_src, &tileset_tiles_pointer);
  DecompressToMem_IpArg(&unk_82E432);
  copy24(&decompress_src, &tileset_compr_palette_ptr);
  DecompressToMem_IpArg(&unk_82E443);
  CopyToVramAtNextInterrupt(&unk_82E449);
  CopyToVramAtNextInterrupt(&unk_82E453);
  CopyToVramAtNextInterrupt(&unk_82E45D);
  if ((cre_bitset & 6) != 0 && door_def_ptr != addr_kDoorDef_947a) {
    CopyToVramAtNextInterrupt(&unk_82E477);
    CopyToVramAtNextInterrupt(&unk_82E481);
    CopyToVramAtNextInterrupt(&unk_82E48B);
  }
  if ((door_direction & 3) == 3)
    irqhandler_next_handler = 16;
  door_transition_function = FUNC16(DoorTransitionFunction_LoadMoreThings_Async);
  return kCoroutineNone;
}

static Func_Y_Y *const kUpdateBackgroundCommands[8] = {
  UpdateBackgroundCommand_0_Finish,
  UpdateBackgroundCommand_2_TransferToVram,
  UpdateBackgroundCommand_4_Decompression,
  UpdateBackgroundCommand_6_ClearFxTilemap,
  UpdateBackgroundCommand_8_TransferToVramAndSetBg3,
  UpdateBackgroundCommand_A_ClearBg2Tilemap,
  UpdateBackgroundCommand_C_ClearKraidBg2Tilemap,
  UpdateBackgroundCommand_E_DoorDependentTransferToVRAM,
};

CoroutineRet DoorTransitionFunction_LoadMoreThings_Async(void) {
  uint16 bg_data_ptr;

  COROUTINE_BEGIN(coroutine_state_1, 0);

  LoadEnemyGfxToVram();
  LoadRoomMusic();
  ClearEnemyProjectiles();
  ClearAnimtiles();
  ClearPaletteFXObjects();
  ClearPLMs();
  CreatePlmsExecuteDoorAsmRoomSetup();
  LoadFXHeader();
  SpawnDoorClosingPLM();
  UpdateBeamTilesAndPalette();
  LoadColorsForSpritesBeamsAndEnemies();
  LoadEnemies();
  InitializeEnemies();
  ResetProjectileData();
  Samus_LoadSuitTargetPalette();
  ClearFxTilemap();
  *(VoidP *)((char *)&door_transition_vram_update_src.addr + 1) = -30208;
  if (fx_tilemap_ptr) {
    door_transition_vram_update_src.addr = fx_tilemap_ptr;
    door_transition_vram_update_dst = addr_unk_605BE0;
    door_transition_vram_update_size = 2112;
    WaitForIrqDoorTransitionVramUpdate();
  }
  bg_data_ptr = get_RoomDefRoomstate(roomdefroomstate_ptr)->bg_data_ptr;
  if (bg_data_ptr & 0x8000) {
    do {
      int v1 = *(uint16 *)RomPtr_8F(bg_data_ptr) >> 1;
      bg_data_ptr = kUpdateBackgroundCommands[v1](bg_data_ptr + 2);
    } while (bg_data_ptr);
  }
  // Wait until the screen has finished scrolling as a result of the door opening
  while ((door_transition_flag & 0x8000u) == 0) {
    COROUTINE_AWAIT(1, WaitForNMI_Async());
  }
  palette_buffer[196] = 15328;
  SpawnBG3ScrollHdmaObject();
  hdma_objects_enable_flag |= 0x8000;
  COROUTINE_AWAIT(2, PlmHandler_Async());

  if ((door_direction & 2) == 0) {
    if ((door_direction & 3) != 0)
      samus_x_pos &= 0xFFF8u;
    else
      samus_x_pos |= 7u;
  }
  door_transition_function = FUNC16(DoorTransition_C_HandleAnimTiles);
  COROUTINE_END(0);
}

static const CopyToVramAtNextInterruptArgs unk_82E5B5 = { LONGPTR(0x7e4000), 0x4000, 0x1000 };
static const CopyToVramAtNextInterruptArgs unk_82E5BF = { LONGPTR(0x7e4000), 0x4800, 0x1000 };
static const CopyToVramAtNextInterruptArgs unk_82E57B = { LONGPTR(0x7e4000), 0x5880, 0x0f00 };
static const CopyToVramAtNextInterruptArgs unk_82E598 = { LONGPTR(0x7e4000), 0x4800, 0x1000 };

void ClearFxTilemap(void) {  // 0x82E566
  for (int i = 959; i >= 0; --i) {
    ram4000.xray_tilemaps[i] = 6222;
    ram4000.xray_tilemaps[i + 960] = 6222;
  }
  CopyToVramAtNextInterrupt(&unk_82E57B);
}

void ClearBg2Tilemap(void) {  // 0x82E583
  for (int i = 1023; i >= 0; --i) {
    ram4000.xray_tilemaps[i] = 824;
    ram4000.xray_tilemaps[i + 1024] = 824;
  }
  CopyToVramAtNextInterrupt(&unk_82E598);
}

void ClearKraidBg2Tilemap(void) {  // 0x82E5A0
  for (int i = 1023; i >= 0; --i) {
    ram4000.xray_tilemaps[i] = 824;
    ram4000.xray_tilemaps[i + 1024] = 824;
  }
  CopyToVramAtNextInterrupt(&unk_82E5B5);
  CopyToVramAtNextInterrupt(&unk_82E5BF);
}

uint16 UpdateBackgroundCommand_0_Finish(uint16 y) {  // 0x82E5D7
  return 0;
}

uint16 UpdateBackgroundCommand_E_DoorDependentTransferToVRAM(uint16 j) {  // 0x82E5D9
  if (door_def_ptr == *(uint16 *)RomPtr_8F(j))
    return UpdateBackgroundCommand_2_TransferToVram(j + 2);
  else
    return j + 9;
}

uint16 UpdateBackgroundCommand_2_TransferToVram(uint16 j) {  // 0x82E5EB
  const uint8 *v1 = RomPtr_8F(j);
  door_transition_vram_update_dst = GET_WORD(v1 + 3);
  copy24(&door_transition_vram_update_src, (LongPtr *)v1);
  door_transition_vram_update_size = GET_WORD(v1 + 5);
  WaitForIrqDoorTransitionVramUpdate();
  return j + 7;
}

uint16 UpdateBackgroundCommand_4_Decompression(uint16 j) {  // 0x82E616
  LongPtr *v1;

  v1 = (LongPtr *)RomPtr_8F(j);
  copy24(&decompress_src, v1);
  *(VoidP *)((char *)&decompress_dst.addr + 1) = 32256;
  decompress_dst.addr = v1[1].addr;
  DecompressToMem();
  return j + 5;
}

uint16 UpdateBackgroundCommand_6_ClearFxTilemap(uint16 j) {  // 0x82E637
  ClearFxTilemap();
  return j;
}

uint16 UpdateBackgroundCommand_8_TransferToVramAndSetBg3(uint16 j) {  // 0x82E63E
  j = UpdateBackgroundCommand_2_TransferToVram(j);
  reg_BG34NBA = 2;
  return j;
}

uint16 UpdateBackgroundCommand_A_ClearBg2Tilemap(uint16 j) {  // 0x82E64B
  ClearBg2Tilemap();
  return j;
}

uint16 UpdateBackgroundCommand_C_ClearKraidBg2Tilemap(uint16 j) {  // 0x82E652
  ClearKraidBg2Tilemap();
  return j;
}

CoroutineRet DoorTransition_C_HandleAnimTiles(void) {  // 0x82E659
  AnimtilesHandler();
  door_transition_function = FUNC16(DoorTransition_WaitForMusicToClear);
  return kCoroutineNone;
}

CoroutineRet DoorTransition_WaitForMusicToClear(void) {  // 0x82E664
  if (!HasQueuedMusic()) {
    door_transition_function = FUNC16(DoorTransition_HandleTransition);
    LoadNewMusicTrackIfChanged();
  }
  return kCoroutineNone;
}

CoroutineRet DoorTransition_HandleTransition(void) {  // 0x82E6A2
  if ((samus_x_pos & 0xF0) == 16) {
    samus_x_pos = (samus_x_pos | 0xF) + 8;
  } else if ((samus_x_pos & 0xF0) == 224) {
    samus_x_pos = (samus_x_pos & 0xFFF0) - 8;
  }
  if ((samus_y_pos & 0xF0) == 16)
    samus_y_pos = (samus_y_pos | 0xF) + 8;
  for (int i = 510; i >= 0; i -= 2) {
    *(uint16 *)&mother_brain_indirect_hdma[i] = 0;
    *(uint16 *)(&hdma_window_1_left_pos[0].field_0 + i) = 0;
  }
  uint16 v1 = room_loading_irq_handler;
  if (!room_loading_irq_handler)
    v1 = 4;
  irqhandler_next_handler = v1;
  PointlessFunctionStupidToo();
  if (elevator_flags) {
    if ((elevator_direction & 0x8000u) == 0)
      CallSomeSamusCode(7u);
    else
      CallSomeSamusCode(0);
  }
  SetLiquidPhysicsType();
  door_transition_function = FUNC16(DoorTransition_FadeInScreenAndFinish);
  *(uint16 *)&reg_INIDISP |= 0x1Fu;
  return kCoroutineNone;
}

CoroutineRet DoorTransition_FadeInScreenAndFinish(void) {  // 0x82E737
  AnimtilesHandler();
  DetermineWhichEnemiesToProcess();
  EnemyMain();
  EprojRunAll();
  DrawSamusEnemiesAndProjectiles();
  EnsureSamusDrawnEachFrame();
  QueueEnemyBG2TilemapTransfers();
  if (AdvancePaletteFadeForAllPalettes_0xc()) {
    debug_disable_sounds = 0;
    PlaySpinJumpSoundIfSpinJumping();
    door_transition_flag_elevator_zebetites = 0;
    door_transition_flag_enemies = 0;
    game_state = kGameState_8_MainGameplay;
  }
  return kCoroutineNone;
}

void LoadDestinationRoomThings(void) {  // 0x82E76B
  LoaadDesinationRoomCreBitset();
  LoadDoorHeader();
  LoadRoomHeader();
  LoadStateHeader();
  LoadCRETilesTilesetTilesAndPalette();
}

void LoadCRETilesTilesetTilesAndPalette(void) {  // 0x82E783
  elevator_flags = 0;
  WriteRegWord(VMAIN, 0x80);
  mov24(&decompress_src, 0xb98000);
  decompress_dst.addr = addr_unk_605000;
  WriteRegWord(VMADDL, 0x2800);
  DecompressToVRAM();
  copy24(&decompress_src, &tileset_tiles_pointer);
  WriteRegWord(VMADDL, 0);
  decompress_dst.addr = 0;
  DecompressToVRAM();
  *(VoidP *)((char *)&decompress_src.addr + 1) = *(VoidP *)((char *)&tileset_compr_palette_ptr.addr + 1);
  decompress_src.addr = tileset_compr_palette_ptr.addr;
  static const LongPtr unk_82E7CD = LONGPTR(0x7ec200);
  DecompressToMem_IpArg(&unk_82E7CD);
}

void LoadLevelDataAndOtherThings(void) {  // 0x82E7D3
  uint16 j;
  int16 v3;
  uint16 k;
  RoomDefRoomstate *v12;
  RoomDefRoomstate *RoomDefRoomstate;
  int16 rdf_scroll_ptr;
  uint16 m;
  char v10;
  char v11;

  uint16 n;
  char v14;

  static const LongPtr unk_82E849 = LONGPTR(0x7ea000);
  static const LongPtr unk_82E85A = LONGPTR(0x7ea800);
  static const LongPtr unk_82E86D = LONGPTR(0x7ea000);

  for (int i = 25598; i >= 0; i -= 2)
    level_data[i >> 1] = 0x8000;
  copy24(&decompress_src, &room_compr_level_data_ptr);
  static const DecompressToParams unk_82E7F8 = { LONGPTR(0x7f0000) };
  DecompressToMem_IpArg(&unk_82E7F8);
  uint16 v1 = ram7F_start;
  for (j = ram7F_start + ram7F_start + (ram7F_start >> 1); ; custom_background[v1 >> 1] = level_data[j >> 1]) {
    j -= 2;
    v1 -= 2;
    if ((v1 & 0x8000u) != 0)
      break;
  }
  v3 = ram7F_start >> 1;
  for (k = ram7F_start + (ram7F_start >> 1); ; *(uint16 *)&BTS[(uint16)v3] = level_data[k >> 1]) {
    k -= 2;
    v3 -= 2;
    if (v3 < 0)
      break;
  }
  if (area_index == 6) {
    copy24(&decompress_src, &tileset_tile_table_pointer);
    DecompressToMem_IpArg(&unk_82E86D);
  } else {
    mov24(&decompress_src, 0xb9a09d);
    DecompressToMem_IpArg(&unk_82E849);
    copy24(&decompress_src, &tileset_tile_table_pointer);
    DecompressToMem_IpArg(&unk_82E85A);
  }
  RoomDefRoomstate = get_RoomDefRoomstate(roomdefroomstate_ptr);
  rdf_scroll_ptr = RoomDefRoomstate->rdf_scroll_ptr;
  if (rdf_scroll_ptr >= 0) {
    R18_ = RoomDefRoomstate->rdf_scroll_ptr;
    LOBYTE(R20_) = room_height_in_scrolls - 1;
    uint8 v8 = 2;
    uint8 v9 = 0;
    v10 = 0;
    do {
      if (v10 == (uint8)R20_)
        v8 = R18_ + 1;
      v14 = v10;
      v11 = 0;
      do {
        scrolls[v9++] = v8;
        ++v11;
      } while (v11 != (uint8)room_width_in_scrolls);
      v10 = v14 + 1;
    } while (v14 + 1 != (uint8)room_height_in_scrolls);
  } else {
    for (m = 0; m != 50; m += 2) {
      *(uint16 *)&scrolls[m] = *(uint16 *)RomPtr_8F(rdf_scroll_ptr);
      rdf_scroll_ptr += 2;
    }
  }
  v12 = get_RoomDefRoomstate(roomdefroomstate_ptr);
  if (v12->room_plm_header_ptr) {
    for (n = v12->room_plm_header_ptr; get_RoomPlmEntry(n)->plm_header_ptr_; n += 6)
      SpawnRoomPLM(n);
  }
  RunDoorSetupCode();
  RunRoomSetupCode();
  if (elevator_flags)
    elevator_status = 2;
}

void SpawnDoorClosingPLM(void) {  // 0x82E8EB
  if (!CheckIfColoredDoorCapSpawned()) {
    const uint16 *v0 = (const uint16 *)RomPtr_8F(2 * door_direction + addr_kDoorClosingPlmIds);
    if (*v0) {
      R18_ = *v0;
      R20_ = *(uint16 *)&get_DoorDef(door_def_ptr)->x_pos_plm;
      SpawnRoomPLM(0x12u); // This reads from R18
    }
  }
}

uint8 CheckIfColoredDoorCapSpawned(void) {  // 0x82E91C
  DoorDef *DD = get_DoorDef(door_def_ptr);
  uint16 prod = Mult8x8(DD->y_pos_plm, room_width_in_blocks);
  uint16 v2 = 2 * (prod + DD->x_pos_plm);
  uint16 v3 = 78;
  while (v2 != plm_block_indices[v3 >> 1]) {
    v3 -= 2;
    if ((v3 & 0x8000u) != 0)
      return 0;
  }
  int v5 = v3 >> 1;
  if (!plm_header_ptr[v5])
    return 0;
  int16 v6 = plm_room_arguments[v5];
  if (v6 >= 0) {
    int idx = PrepareBitAccess(v6);
    if ((bitmask & opened_door_bit_array[idx]) != 0)
      return 0;
  }
  plm_instruction_timer[v5] = 1;
  uint16 v8 = plm_header_ptr[v5];
  if (!v8)
    return 0;
  plm_instr_list_ptrs[v5] = get_PlmHeader_Size6(v8)->instr_list_2_ptr;
  return 1;
}

static Func_Y_Y *const kLoadLibraryBackgroundFuncs[8] = {  // 0x82E97C
  LoadLibraryBackgroundFunc_0_DONE,
  LoadLibraryBackgroundFunc_2_TransferToVram,
  LoadLibraryBackgroundFunc_4_Decompress,
  LoadLibraryBackgroundFunc_6_ClearFxTilemap,
  LoadLibraryBackgroundFunc_8_TransferToVramAndSetBg3,
  LoadLibraryBackgroundFunc_A_ClearBG2Tilemap,
  LoadLibraryBackgroundFunc_C_ClearKraidLayer2,
  LoadLibraryBackgroundFunc_E_DoorDependentTransferToVram,
};

void LoadLibraryBackground(void) {
  uint16 bg_data_ptr;

  ClearFXTilemap();
  if (fx_tilemap_ptr) {
    WriteRegWord(A1T1L, fx_tilemap_ptr);
    WriteRegWord(VMADDL, addr_unk_605BE0);
    WriteRegWord(DMAP1, 0x1801);
    WriteRegWord(A1B1, 0x8A);
    WriteRegWord(DAS1L, 0x840);
    WriteReg(VMAIN, 0x80);
    WriteReg(MDMAEN, 2u);
  }
  bg_data_ptr = get_RoomDefRoomstate(roomdefroomstate_ptr)->bg_data_ptr;
  if (bg_data_ptr & 0x8000) {
    do {
      uint16 v1 = *(uint16 *)RomPtr_8F(bg_data_ptr);
      bg_data_ptr += 2;
      bg_data_ptr = kLoadLibraryBackgroundFuncs[v1 >> 1](bg_data_ptr);
    } while (bg_data_ptr);
  }
}

uint16 LoadLibraryBackgroundFunc_0_DONE(uint16 j) {  // 0x82E9E5
  return 0;
}

uint16 LoadLibraryBackgroundFunc_E_DoorDependentTransferToVram(uint16 j) {  // 0x82E9E7
  if (door_def_ptr == get_LoadBg_E(j)->field_0)
    return LoadLibraryBackgroundFunc_2_TransferToVram(j + 2);
  else
    return j + 9;
}

uint16 LoadLibraryBackgroundFunc_2_TransferToVram(uint16 j) {  // 0x82E9F9
  uint16 *LoadBg_28 = (uint16 *)get_LoadBg_28(j);
  WriteRegWord(VMADDL, *(uint16 *)((char *)LoadBg_28 + 3));
  WriteRegWord(DMAP1, 0x1801);
  WriteRegWord(A1T1L, *LoadBg_28);
  WriteRegWord(A1B1, LoadBg_28[1]);
  WriteRegWord(DAS1L, *(uint16 *)((char *)LoadBg_28 + 5));
  WriteReg(VMAIN, 0x80);
  WriteReg(MDMAEN, 2u);
  return j + 7;
}

uint16 LoadLibraryBackgroundFunc_4_Decompress(uint16 j) {  // 0x82EA2D
  LongPtr *LoadBg_28;

  LoadBg_28 = (LongPtr *)get_LoadBg_28(j);
  copy24(&decompress_src, LoadBg_28);
  *(VoidP *)((char *)&decompress_dst.addr + 1) = 32256;
  decompress_dst.addr = LoadBg_28[1].addr;
  DecompressToMem();
  return j + 5;
}

uint16 LoadLibraryBackgroundFunc_6_ClearFxTilemap(uint16 j) {  // 0x82EA4E
  ClearFXTilemap();
  return j;
}

uint16 LoadLibraryBackgroundFunc_A_ClearBG2Tilemap(uint16 j) {  // 0x82EA56
  ClearBG2Tilemap();
  return j;
}

uint16 LoadLibraryBackgroundFunc_C_ClearKraidLayer2(uint16 j) {  // 0x82EA5E
  ClearBG2Tilemap();
  return j;
}

uint16 LoadLibraryBackgroundFunc_8_TransferToVramAndSetBg3(uint16 j) {  // 0x82EA66
  j = LoadLibraryBackgroundFunc_2_TransferToVram(j);
  reg_BG34NBA = 2;
  return j;
}

void LoadLevelScrollAndCre(void) {  // 0x82EA73
  uint16 j;
  int16 v3;
  uint16 k;
  RoomDefRoomstate *RoomDefRoomstate;
  int16 rdf_scroll_ptr;
  uint16 m;
  char v10;
  char v11;
  char v12;

  static const LongPtr unk_82EAA0 = LONGPTR(0x7f0000);
  static const LongPtr unk_82EAF9 = LONGPTR(0x7ea000);
  static const LongPtr unk_82EB0A = LONGPTR(0x7ea800);
  static const LongPtr unk_82EB1D = LONGPTR(0x7ea000);

  for (int i = 6398; i >= 0; i -= 2) {
    level_data[i >> 1] = 0x8000;
    level_data[(i >> 1) + 3200 * 1] = 0x8000;
    level_data[(i >> 1) + 3200 * 2] = 0x8000;
    level_data[(i >> 1) + 3200 * 3] = 0x8000;
  }
  copy24(&decompress_src, &room_compr_level_data_ptr);
  DecompressToMem_IpArg(&unk_82EAA0);
  uint16 v1 = ram7F_start;
  for (j = ram7F_start + ram7F_start + (ram7F_start >> 1); ; custom_background[v1 >> 1] = level_data[j >> 1]) {
    j -= 2;
    v1 -= 2;
    if ((v1 & 0x8000u) != 0)
      break;
  }
  v3 = ram7F_start >> 1;
  for (k = ram7F_start + (ram7F_start & 1) + (ram7F_start >> 1);
       ;
       *(uint16 *)&BTS[(uint16)v3] = level_data[k >> 1]) {
    k -= 2;
    v3 -= 2;
    if (v3 < 0)
      break;
  }
  if (area_index == 6) {
    copy24(&decompress_src, &tileset_tile_table_pointer);
    DecompressToMem_IpArg(&unk_82EB1D);
  } else {
    if ((cre_bitset & 2) != 0) {
      mov24(&decompress_src, 0xb9a09d);
      DecompressToMem_IpArg(&unk_82EAF9);
    }
    copy24(&decompress_src, &tileset_tile_table_pointer);
    DecompressToMem_IpArg(&unk_82EB0A);
  }
  RoomDefRoomstate = get_RoomDefRoomstate(roomdefroomstate_ptr);
  rdf_scroll_ptr = RoomDefRoomstate->rdf_scroll_ptr;
  if (rdf_scroll_ptr >= 0) {
    R18_ = RoomDefRoomstate->rdf_scroll_ptr;
    LOBYTE(R20_) = room_height_in_scrolls - 1;
    uint8 v8 = 2;
    uint8 v9 = 0;
    v10 = 0;
    do {
      if (v10 == (uint8)R20_)
        v8 = R18_ + 1;
      v12 = v10;
      v11 = 0;
      do {
        scrolls[v9++] = v8;
        ++v11;
      } while (v11 != (uint8)room_width_in_scrolls);
      v10 = v12 + 1;
    } while (v12 + 1 != (uint8)room_height_in_scrolls);
  } else {
    for (m = 0; m != 50; m += 2) {
      *(uint16 *)&scrolls[m] = *(uint16 *)RomPtr_8F(rdf_scroll_ptr);
      rdf_scroll_ptr += 2;
    }
  }
}

void CreatePlmsExecuteDoorAsmRoomSetup(void) {  // 0x82EB6C
  RoomDefRoomstate *RoomDefRoomstate;
  RoomDefRoomstate = get_RoomDefRoomstate(roomdefroomstate_ptr);
  if (RoomDefRoomstate->room_plm_header_ptr) {
    for (int i = RoomDefRoomstate->room_plm_header_ptr; get_RoomPlmEntry(i)->plm_header_ptr_; i += 6)
      SpawnRoomPLM(i);
  }
  RunDoorSetupCode();
  RunRoomSetupCode();
  if (elevator_flags)
    elevator_status = 2;
}

static Func_V *const kGameOptionsMenuFuncs[13] = {  // 0x82EB9F
  GameOptionsMenuFunc_0,
  GameOptionsMenu_1_LoadingOptionsScreen,
  GameOptionsMenu_2_FadeInOptionsScreen,
  GameOptionsMenu_3_OptionsScreen,
  GameOptionsMenu_4_StartGame,
  GameOptionsMenu_5_DissolveOutScreen,
  GameOptionsMenu_6_DissolveInScreen,
  GameOptionsMenu_7_ControllerSettings,
  GameOptionsMenu_8_SpecialSettings,
  GameOptionsMenu_9_ScrollControllerSettingsDown,
  GameOptionsMenu_A_ScrollControllerSettingsUp,
  GameOptionsMenu_B_TransitionBackToFileSelect,
  GameOptionsMenu_C_FadeOutOptionsScreenToStart,
};

CoroutineRet GameState_2_GameOptionsMenu(void) {
  kGameOptionsMenuFuncs[game_options_screen_index]();
  OptionsMenuFunc1();
  DrawOptionsMenuSpritemaps();
  if (!sign16(game_options_screen_index - 2))
    OptionsMenu_AddToVramQueue();
  return kCoroutineNone;
}

void GameOptionsMenuFunc_0(void) {  // 0x82EBDB
  HandleFadeOut();
  if (reg_INIDISP == 0x80) {
    EnableNMI();
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    ++game_options_screen_index;
  } else if (reg_INIDISP == 14) {
    uint16 v0 = reg_TS;
    if ((reg_TS & 4) == 0)
      CreateOptionsMenuObject_(v0, addr_stru_82F4D6);
  }
}

void GameOptionsMenu_1_LoadingOptionsScreen(void) {  // 0x82EC11
  uint16 j;

  static const DecompressToParams unk_82EC74 = { LONGPTR(0x7fc000) };
  static const DecompressToParams unk_82EC85 = { LONGPTR(0x7fc800) };
  static const DecompressToParams unk_82EC96 = { LONGPTR(0x7fd000) };
  static const DecompressToParams unk_82ECA7 = { LONGPTR(0x7fd800) };
  static const DecompressToParams unk_82ECB8 = { LONGPTR(0x7fe000) };

  reg_BG12NBA = 0;
  reg_TM = 19;
  reg_TS = 0;
  reg_TMW = 0;
  reg_TSW = 0;
  next_gameplay_CGWSEL = 0;
  next_gameplay_CGADSUB = 0;
  reg_CGWSEL = 0;
  reg_CGADSUB = 0;
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x58);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_82EC3D = { 1, 1, 0x18, LONGPTR(0x8edc00), 0x0800 };
  SetupDmaTransfer(&unk_82EC3D);
  WriteReg(MDMAEN, 2u);
  reg_BG1HOFS = 0;
  reg_BG1VOFS = 0;
  reg_BG2HOFS = 0;
  reg_BG2VOFS = 0;
  debug_invincibility = 0;
  for (int i = 510; i >= 0; i -= 2)
    palette_buffer[i >> 1] = kMenuPalettes[i >> 1];
  mov24(&decompress_src, 0x978DF4);
  DecompressToMem_IpArg(&unk_82EC74);
  mov24(&decompress_src, 0x978FCD);
  DecompressToMem_IpArg(&unk_82EC85);
  mov24(&decompress_src, 0x9791C4);
  DecompressToMem_IpArg(&unk_82EC96);
  mov24(&decompress_src, 0x97938D);
  DecompressToMem_IpArg(&unk_82ECA7);
  mov24(&decompress_src, 0x97953A);
  DecompressToMem_IpArg(&unk_82ECB8);
  for (j = 1023; (j & 0x8000u) == 0; --j)
    ram3000.pause_menu_map_tilemap[j] = custom_background[j + 5375];
  menu_option_index = 0;
  DeleteAllOptionsMenuObjects_();
  CreateOptionsMenuObject_(0, addr_stru_82F4B8);
  CreateOptionsMenuObject_(0, addr_stru_82F4C4);
  ++game_options_screen_index;
  OptionsMenuFunc4();
}

void GameOptionsMenu_2_FadeInOptionsScreen(void) {  // 0x82ECE4
  HandleFadeIn();
  if (reg_INIDISP == 15) {
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    ++game_options_screen_index;
  }
}

void OptionsMenu_AddToVramQueue(void) {  // 0x82ECFF
  uint16 v0 = vram_write_queue_tail;
  gVramWriteEntry(vram_write_queue_tail)->size = 2048;
  v0 += 2;
  gVramWriteEntry(v0)->size = 12288;
  v0 += 2;
  LOBYTE(gVramWriteEntry(v0++)->size) = 126;
  gVramWriteEntry(v0)->size = 20480;
  vram_write_queue_tail = v0 + 2;
}

void OptionsMenuFunc5(uint16 a, uint16 k, uint16 j) {  // 0x82ED28
  R18_ = a;
  do {
    *(uint16 *)((char *)ram3000.pause_menu_map_tilemap + k) = R18_ | *(uint16 *)((uint8 *)ram3000.pause_menu_map_tilemap
                                                                                 + k) & 0xE3FF;
    k += 2;
    j -= 2;
  } while (j);
}
static Func_V *const kGameOptionsMenuItemFuncs[5] = {  // 0x82ED42
  GameOptionsMenuItemFunc_0,
  GameOptionsMenuItemFunc_2_ToggleJapanese,
  GameOptionsMenuItemFunc_2_ToggleJapanese,
  GameOptionsMenuItemFunc_4,
  GameOptionsMenuItemFunc_4,
};
void GameOptionsMenu_3_OptionsScreen(void) {
  if ((joypad1_newkeys & kButton_Up) != 0) {
    QueueSfx1_Max6(0x37u);
    if ((--menu_option_index & 0x8000u) != 0)
      menu_option_index = 4;
  } else if ((joypad1_newkeys & kButton_Down) != 0) {
    QueueSfx1_Max6(0x37u);
    if (++menu_option_index == 5)
      menu_option_index = 0;
  }
  if ((joypad1_newkeys & 0x8000u) != 0) {
    game_options_screen_index = 11;
  } else if ((joypad1_newkeys & kButton_A) != 0 || (joypad1_newkeys & kButton_Start) != 0) {
    QueueSfx1_Max6(0x38u);
    kGameOptionsMenuItemFuncs[menu_option_index]();
  }
}

void GameOptionsMenuItemFunc_0(void) {  // 0x82EDB1
  if (enable_debug && (joypad1_lastkeys & kButton_L) == 0 || loading_game_state == kGameState_5_FileSelectMap) {
    game_options_screen_index = 4;
  } else {
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    game_options_screen_index = 12;
  }
}

void GameOptionsMenuItemFunc_2_ToggleJapanese(void) {  // 0x82EDDA
  menu_option_index = 0;
  japanese_text_flag = japanese_text_flag == 0;
  OptionsMenuFunc4();
}

void OptionsMenuFunc4(void) {  // 0x82EDED
  if (japanese_text_flag) {
    OptionsMenuFunc5(0x400, 0x288, 0x18);
    OptionsMenuFunc5(0x400, 0x2C8, 0x18);
    OptionsMenuFunc5(0, 0x348, 0x32);
    OptionsMenuFunc5(0, 0x388, 0x32);
  } else {
    OptionsMenuFunc5(0, 0x288, 0x18);
    OptionsMenuFunc5(0, 0x2C8, 0x18);
    OptionsMenuFunc5(0x400, 0x348, 0x32);
    OptionsMenuFunc5(0x400, 0x388, 0x32);
  }
}

void GameOptionsMenuItemFunc_4(void) {  // 0x82EE55
  reg_MOSAIC = 3;
  screen_fade_delay = 0;
  screen_fade_counter = 0;
  game_options_screen_index = 5;
}

void GameOptionsMenu_B_TransitionBackToFileSelect(void) {  // 0x82EE6A
  HandleFadeOut();
  if (reg_INIDISP == 0x80) {
    EnableNMI();
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    game_state = kGameState_4_FileSelectMenus;
    menu_index = 0;
    game_options_screen_index = 0;
  }
}

void GameOptionsMenu_C_FadeOutOptionsScreenToStart(void) {  // 0x82EE92
  HandleFadeOut();
  if (reg_INIDISP == 0x80) {
    EnableNMI();
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    game_options_screen_index = 4;
  }
}

void GameOptionsMenu_4_StartGame(void) {  // 0x82EEB4
  game_options_screen_index = 0;
  if (enable_debug && (joypad1_lastkeys & 0x20) == 0) {
    game_state = kGameState_5_FileSelectMap;
    if (loading_game_state != kLoadingGameState_5_Main) {
      loading_game_state = kGameState_5_FileSelectMap;
      SaveToSram(selected_save_slot);
    }
  } else if (loading_game_state) {
    game_state = loading_game_state;
    if (loading_game_state == kGameState_34_CeresGoesBoom)
      cinematic_function = FUNC16(CinematicFunctionBlackoutFromCeres);
    menu_option_index = 0;
    game_options_screen_index = 0;
  } else {
    game_state = kGameState_30_IntroCinematic;
    cinematic_function = FUNC16(CinematicFunction_Intro_Initial);
    menu_option_index = 0;
    screen_fade_delay = 0;
    screen_fade_counter = 0;
  }
}

void GameOptionsMenu_5_DissolveOutScreen(void) {  // 0x82EF18
  uint16 m;
  uint16 k;
  uint16 j;
  uint16 n;
  uint16 v1;

  HandleFadeOut();
  if (reg_MOSAIC != 0xF3)
    reg_MOSAIC += 16;
  if (reg_INIDISP == 0x80) {
    EnableNMI();
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    reg_BG1VOFS = 0;
    ++game_options_screen_index;
    if (menu_option_index) {
      if ((menu_option_index & 4) != 0) {
        if (japanese_text_flag) {
          for (int i = 1023; i >= 0; --i)
            ram3000.pause_menu_map_tilemap[i] = custom_background[i + 9471];
        } else {
          for (j = 1023; (j & 0x8000u) == 0; --j)
            ram3000.pause_menu_map_tilemap[j] = custom_background[j + 8447];
        }
        menu_option_index = 0;
        OptionsMenuFunc7();
        menu_option_index = 1;
        OptionsMenuFunc7();
        menu_option_index = 4;
        CreateOptionsMenuObject_(4u, addr_stru_82F4D0);
      } else {
        if (japanese_text_flag) {
          for (k = 1023; (k & 0x8000u) == 0; --k) {
            v1 = custom_background[k + 7423];
            ram3000.pause_menu_map_tilemap[k] = v1;
          }
        } else {
          for (m = 1023; (m & 0x8000u) == 0; --m) {
            v1 = custom_background[m + 6399];
            ram3000.pause_menu_map_tilemap[m] = v1;
          }
        }
        CreateOptionsMenuObject_(v1, addr_stru_82F4CA);
        LoadControllerOptionsFromControllerBindings();
        OptionsMenuFunc6();
      }
    } else {
      for (n = 1023; (n & 0x8000u) == 0; --n)
        ram3000.pause_menu_map_tilemap[n] = custom_background[n + 5375];
      OptionsMenuFunc4();
      uint16 v6 = 0; // bug
      CreateOptionsMenuObject_(v6, addr_stru_82F4C4);
    }
  }
}

void GameOptionsMenu_6_DissolveInScreen(void) {  // 0x82EFDB
  HandleFadeIn();
  if (reg_MOSAIC != 3)
    reg_MOSAIC -= 16;
  if (reg_INIDISP == 15) {
    reg_MOSAIC = 0;
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    if (menu_option_index) {
      if ((menu_option_index & 4) != 0)
        game_options_screen_index = 8;
      else
        game_options_screen_index = 7;
      menu_option_index = 0;
    } else {
      game_options_screen_index = 3;
    }
  }
}
static Func_V *const kGameOptionsMenuSpecialSettings[3] = {  // 0x82F024
  GameOptionsMenuSpecialSettings_0,
  GameOptionsMenuSpecialSettings_0,
  GameOptionsMenuSpecialSettings_2,
};
void GameOptionsMenu_8_SpecialSettings(void) {
  if ((joypad1_newkeys & kButton_Up) != 0) {
    QueueSfx1_Max6(0x37u);
    if ((--menu_option_index & 0x8000u) != 0)
      menu_option_index = 2;
  } else if ((joypad1_newkeys & kButton_Down) != 0) {
    QueueSfx1_Max6(0x37u);
    if (++menu_option_index == 3)
      menu_option_index = 0;
  }
  if ((joypad1_newkeys & 0x8000u) == 0) {
    if ((joypad1_newkeys & (kButton_Start | kButton_Left | kButton_Right | kButton_A)) != 0) {
      QueueSfx1_Max6(0x38u);
      kGameOptionsMenuSpecialSettings[menu_option_index]();
    }
  } else {
    QueueSfx1_Max6(0x38u);
    menu_option_index = 0;
    GameOptionsMenuItemFunc_4();
  }
}
void GameOptionsMenuSpecialSettings_0(void) {  // 0x82F08E
  uint8 *v0 = RomPtr_RAM(kOptionsMenuSpecialPtrs[menu_option_index]);
  if (GET_WORD(v0))
    *(uint16 *)v0 = 0;
  else
    *(uint16 *)v0 = 1;
  OptionsMenuFunc7();
}

void GameOptionsMenuSpecialSettings_2(void) {  // 0x82F0B2
  menu_option_index = 0;
  GameOptionsMenuItemFunc_4();
}

static const uint16 g_word_82F149[4] = {  // 0x82F0B9
  0x1e0, 0x220,
  0x360, 0x3a0,
};
static const uint16 g_word_82F151[4] = {
  0x1ee, 0x22e,
  0x36e, 0x3ae,
};
void OptionsMenuFunc7(void) {

  uint16 v0 = 4 * menu_option_index;
  if (*(uint16 *)RomPtr_RAM(kOptionsMenuSpecialPtrs[menu_option_index])) {
    OptionsMenuFunc5(0, g_word_82F149[(uint16)(4 * menu_option_index) >> 1], 0xC);
    OptionsMenuFunc5(0, g_word_82F149[(v0 >> 1) + 1], 0xC);
    OptionsMenuFunc5(0x400, g_word_82F151[v0 >> 1], 0xC);
    OptionsMenuFunc5(0x400, g_word_82F151[(v0 >> 1) + 1], 0xC);
  } else {
    OptionsMenuFunc5(0x400, g_word_82F149[(uint16)(4 * menu_option_index) >> 1], 0xC);
    OptionsMenuFunc5(0x400, g_word_82F149[(v0 >> 1) + 1], 0xC);
    OptionsMenuFunc5(0, g_word_82F151[v0 >> 1], 0xC);
    OptionsMenuFunc5(0, g_word_82F151[(v0 >> 1) + 1], 0xC);
  }
}

static const uint16 word_82F204[16] = {  // 0x82F159
  0x20, 0x20,   0x20, 0x20,
  0x10, 0x10,   0x10, 0x10,
  0x10, 0x10, 0x4000, 0x40,
  0x40, 0x80,   0x80, 0x80,
};

void GameOptionsMenu_7_ControllerSettings(void) {

  if ((joypad1_newkeys & kButton_Up) != 0) {
    QueueSfx1_Max6(0x37u);
    uint16 v0 = --menu_option_index;
    if ((menu_option_index & 0x8000u) == 0) {
      if (v0 != 6)
        return;
      game_options_screen_index = 10;
      return;
    }
    menu_option_index = 8;
    game_options_screen_index = 9;
    return;
  }
  if ((joypad1_newkeys & kButton_Down) != 0) {
    QueueSfx1_Max6(0x37u);
    uint16 v1 = menu_option_index + 1;
    menu_option_index = v1;
    if (v1 == 7) {
      game_options_screen_index = 9;
      return;
    }
    if (v1 == 9) {
      menu_option_index = 0;
      game_options_screen_index = 10;
      return;
    }
  } else if (joypad1_newkeys) {
    QueueSfx1_Max6(0x38u);
    static Func_V *const kOptionsMenuControllerFuncs[9] = {
      OptionsMenuControllerFunc_0,
      OptionsMenuControllerFunc_0,
      OptionsMenuControllerFunc_0,
      OptionsMenuControllerFunc_0,
      OptionsMenuControllerFunc_0,
      OptionsMenuControllerFunc_0,
      OptionsMenuControllerFunc_0,
      OptionsMenuControllerFunc_7,
      OptionsMenuControllerFunc_8,
    };
    kOptionsMenuControllerFuncs[menu_option_index]();
  } else if (joypad2_new_keys && menu_option_index == 8 && sign16(debug_invincibility - 16)) {
    if ((word_82F204[debug_invincibility] & joypad2_new_keys) == word_82F204[debug_invincibility])
      ++debug_invincibility;
    else
      debug_invincibility = 0;
  }
}

void OptionsMenuControllerFunc_8(void) {  // 0x82F224
  if ((joypad1_newkeys & (kButton_Start | kButton_A)) != 0) {
    button_config_shoot_x = 64;
    button_config_jump_a = 128;
    button_config_run_b = 0x8000;
    button_config_itemcancel_y = 0x4000;
    button_config_itemswitch = 0x2000;
    button_config_aim_up_R = 16;
    button_config_aim_down_L = 32;
    LoadControllerOptionsFromControllerBindings();
    OptionsMenuFunc6();
  }
}

void OptionsMenuControllerFunc_7(void) {  // 0x82F25D
  if ((joypad1_newkeys & (kButton_Start | kButton_A)) != 0 && !OptionsMenuFunc8()) {
    menu_option_index = 0;
    GameOptionsMenuItemFunc_4();
  }
}

void GameOptionsMenu_9_ScrollControllerSettingsDown(void) {  // 0x82F271
  reg_BG1VOFS += 2;
  if (reg_BG1VOFS == 32)
    game_options_screen_index = 7;
}

void GameOptionsMenu_A_ScrollControllerSettingsUp(void) {  // 0x82F285
  reg_BG1VOFS -= 2;
  if (!reg_BG1VOFS)
    game_options_screen_index = 7;
}

void sub_82F296(uint16 j) {  // 0x82F296
  int v1 = j >> 1;
  enemy_projectile_y_pos[v1 + 13] = 24;
  enemy_projectile_x_vel[v1 + 3] = 56;
  enemy_projectile_x_vel[v1 + 11] = 3584;
}
void OptionsPreInstr_F2A9(uint16 v0) {  // 0x82F2A9
  if (game_state == kGameState_2_GameOptionsMenu) {
    int v2 = game_options_screen_index;
    uint16 v3 = off_82F2ED[v2];
    if (v3) {
      R18_ = off_82F2ED[v2];
      const uint16 *v4 = (const uint16 *)RomPtr_82(v3 + 4 * menu_option_index);
      int v5 = v0 >> 1;
      enemy_projectile_y_pos[v5 + 13] = *v4;
      enemy_projectile_x_vel[v5 + 3] = v4[1];
    } else {
      int v6 = v0 >> 1;
      enemy_projectile_y_pos[v6 + 13] = 384;
      enemy_projectile_x_vel[v6 + 3] = 16;
    }
  } else {
    int v1 = v0 >> 1;
    enemy_projectile_E[v1 + 15] = 1;
    enemy_projectile_y_vel[v1 + 17] = addr_off_82F4B6;
  }
}

void sub_82F34B(uint16 j) {  // 0x82F34B
  enemy_projectile_y_pos[(j >> 1) + 13] = 124;
  sub_82F369(j);
}
void sub_82F353(uint16 j) {  // 0x82F353
  enemy_projectile_y_pos[(j >> 1) + 13] = 132;
  sub_82F369(j);
}
void sub_82F35B(uint16 j) {  // 0x82F35B
  enemy_projectile_y_pos[(j >> 1) + 13] = 128;
  sub_82F369(j);
}
void sub_82F363(uint16 j) {  // 0x82F363
  enemy_projectile_y_pos[(j >> 1) + 13] = 128;
  sub_82F369(j);
}

void sub_82F369(uint16 j) {  // 0x82F369
  int v1 = j >> 1;
  enemy_projectile_x_vel[v1 + 3] = 16;
  enemy_projectile_x_vel[v1 + 11] = 3584;
}


void OptionsPreInstr_F376(uint16 k) {  // 0x82F376
  if (game_state != kGameState_2_GameOptionsMenu || game_options_screen_index == 6 && reg_INIDISP == 0x80) {
    int v1 = k >> 1;
    enemy_projectile_E[v1 + 15] = 1;
    enemy_projectile_y_vel[v1 + 17] = addr_off_82F4B6;
  }
}

void OptionsPreInstr_F3A0(uint16 k) {  // 0x82F3A0
  switch (game_options_screen_index) {
  case 6u:
    if (reg_INIDISP == 0x80) {
      int v1 = k >> 1;
      enemy_projectile_E[v1 + 15] = 1;
      enemy_projectile_y_vel[v1 + 17] = -2890;
    }
    break;
  case 9u:
    enemy_projectile_x_vel[(k >> 1) + 3] -= 2;
    break;
  case 0xAu:
    enemy_projectile_x_vel[(k >> 1) + 3] += 2;
    break;
  }
}

void OptionsPreInstr_F3E2(uint16 k) {  // 0x82F3E2
  if (game_options_screen_index == 6 && reg_INIDISP == 0x80) {
    int v1 = k >> 1;
    enemy_projectile_E[v1 + 15] = 1;
    enemy_projectile_y_vel[v1 + 17] = -2890;
  }
}

void sub_82F404(uint16 k) {  // 0x82F404
  if (game_options_screen_index == 1) {
    int v1 = k >> 1;
    enemy_projectile_E[v1 + 15] = 1;
    enemy_projectile_y_vel[v1 + 17] = -2890;
  }
}

void sub_82F419(uint16 j) {  // 0x82F419
  int v1 = j >> 1;
  enemy_projectile_y_pos[v1 + 13] = 216;
  enemy_projectile_x_vel[v1 + 3] = 16;
  enemy_projectile_x_vel[v1 + 11] = 3584;
}

void OptionsPreInstr_F42C(uint16 k) {  // 0x82F42C
  if (game_state != 2) {
    int v1 = k >> 1;
    enemy_projectile_E[v1 + 15] = 1;
    enemy_projectile_y_vel[v1 + 17] = addr_off_82F4B6;
  }
}
void LoadControllerOptionsFromControllerBindings(void) {  // 0x82F4DC
  int16 v2;

  uint16 v0 = 0;
  do {
    int v1 = v0 >> 1;
    v2 = *(uint16 *)&g_ram[off_82F54A[v1]];
    if ((v2 & kButton_X) != 0) {
LABEL_9:
      enemy_projectile_F[v1 + 13] = 0;
      goto LABEL_16;
    }
    if ((v2 & kButton_A) != 0) {
      enemy_projectile_F[v1 + 13] = 1;
    } else if (v2 < 0) {
      enemy_projectile_F[v1 + 13] = 2;
    } else if ((v2 & kButton_Select) != 0) {
      enemy_projectile_F[v1 + 13] = 3;
    } else if ((v2 & kButton_Y) != 0) {
      enemy_projectile_F[v1 + 13] = 4;
    } else if ((v2 & kButton_L) != 0) {
      enemy_projectile_F[v1 + 13] = 5;
    } else {
      if ((v2 & kButton_R) == 0)
        goto LABEL_9;
      enemy_projectile_F[v1 + 13] = 6;
    }
LABEL_16:
    v0 += 2;
  } while ((int16)(v0 - 14) < 0);
}
static const Buttons word_82F575[9] = {  // 0x82F558
  kButton_X,
  kButton_A,
  kButton_B,
  kButton_Select,
  kButton_Y,
  kButton_L,
  kButton_R,
  kButton_Left,
  kButton_Right,
};
uint8 OptionsMenuFunc8(void) {
  int v0 = 0, v2;
  do {
    v2 = v0;
    *(uint16 *)&g_ram[off_82F54A[v0 >> 1]] = word_82F575[(uint16)(2 * enemy_projectile_F[(v0 >> 1) + 13]) >> 1];
    v0 += 2;
  } while (v2 < 12);
  return 0;
}
static const uint16 g_word_82F6AD[6] = {  // 0x82F587
  0,
  0xe,
  0xe,
  0x10,
  0x1f,
  0x1f,
};

void OptionsMenuFunc6(void) {
  uint16 v0 = 0, v4;
  do {
    v4 = v0;
    int v1 = v0 >> 1;
    uint16 v2 = g_word_82F639[v1];
    const uint16 *v3 = (const uint16 *)RomPtr_82(g_off_82F647[(uint16)(2 * enemy_projectile_F[v1 + 13]) >> 1]);
    *(uint16 *)((char *)ram3000.pause_menu_map_tilemap + v2) = *v3;
    *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[1] + v2) = v3[1];
    *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[2] + v2) = v3[2];
    *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[32] + v2) = v3[3];
    *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[33] + v2) = v3[4];
    *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[34] + v2) = v3[5];
    v0 = v4 + 2;
  } while ((int16)(v4 - 12) < 0);
  if (enemy_projectile_instr_list_ptr[0] != 5 && enemy_projectile_instr_list_ptr[0] != 6) {
    *(uint32 *)&ram3000.menu.backup_of_io_registers_in_gameover[46] = *(uint32 *)g_word_82F6AD;
    ram3000.pause_menu_map_tilemap[665] = g_word_82F6AD[2];
    *(uint32 *)&ram3000.menu.field_536[56] = *(uint32 *)&g_word_82F6AD[3];
    ram3000.pause_menu_map_tilemap[697] = g_word_82F6AD[5];
  }
  if (enemy_projectile_instr_list_ptr[1] != 5 && enemy_projectile_instr_list_ptr[1] != 6) {
    *(uint32 *)&ram3000.menu.field_536[184] = *(uint32 *)g_word_82F6AD;
    ram3000.pause_menu_map_tilemap[761] = g_word_82F6AD[2];
    *(uint32 *)&ram3000.menu.menu_tilemap[46] = *(uint32 *)&g_word_82F6AD[3];
    ram3000.pause_menu_map_tilemap[793] = g_word_82F6AD[5];
  }
}

void OptionsMenuControllerFunc_0(void) {  // 0x82F6B9
  uint16 v0 = 12;
  while ((word_82F575[v0 >> 1] & joypad1_newkeys) == 0) {
    v0 -= 2;
    if ((v0 & 0x8000u) != 0)
      return;
  }
  R18_ = v0 >> 1;
  uint16 v1 = 2 * menu_option_index + 2;
  if (!sign16(2 * menu_option_index - 12))
    v1 = 0;
  uint16 v2 = v1;
  for (int i = 5; i >= 0; --i) {
    if (enemy_projectile_F[(v2 >> 1) + 13] == R18_)
      break;
    v2 += 2;
    if ((int16)(v2 - 14) >= 0)
      v2 = 0;
  }
  int v4 = menu_option_index;
  R20_ = enemy_projectile_F[v4 + 13];
  enemy_projectile_F[v4 + 13] = R18_;
  enemy_projectile_F[(v2 >> 1) + 13] = R20_;
  OptionsMenuFunc6();
}