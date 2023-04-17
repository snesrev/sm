// Message boxes
#include "ida_types.h"
#include "variables.h"
#include "funcs.h"

uint16 message_box_das0l_value;


#define kMessageBoxDefs ((MsgBoxConfig*)RomFixedPtr(0x85869b))
static const uint16 kMsgBoxSpecialButtonTilemapOffs[27] = {  // 0x8583D1
  0, 0x12a, 0x12a, 0x12c, 0x12c, 0x12c, 0, 0, 0, 0, 0, 0, 0x120, 0, 0, 0,
  0,     0, 0x12a,     0,     0,     0, 0, 0, 0, 0, 0,
};
static const uint16 kTileNumbersForButtonLetters[8] = { 0x28e0, 0x3ce1, 0x2cf7, 0x38f8, 0x38d0, 0x38eb, 0x38f1, 0x284e };

static void InitializePpuForMessageBoxes(void) {  // 0x858143
  save_confirmation_selection = 0;
  WriteReg(HDMAEN, 0);
  WriteReg(CGADD, 0x19);
  WriteReg(CGDATA, 0xB1);
  WriteReg(CGDATA, 0xB);
  WriteReg(CGDATA, 0x1F);
  WriteReg(CGDATA, 0);
  ram3000.msgbox.backup_of_enabled_hdma_channels = reg_HDMAEN;
  ram3000.misc.field_3E8[3] = gameplay_BG3SC;
  gameplay_BG3SC = 88;
  gameplay_TM = 23;
  gameplay_CGWSEL = 0;
  gameplay_CGADSUB = 0;
  WriteReg(COLDATA, 0x20);
  WriteReg(COLDATA, 0x40);
  WriteReg(COLDATA, 0x80);
  ReadReg(BG3HOFS);
  WriteReg(BG3HOFS, 0);
  WriteReg(BG3HOFS, 0);
  ReadReg(BG3VOFS);
  WriteReg(BG3VOFS, 0);
  WriteReg(BG3VOFS, 0);
  for (int i = 128; i >= 0; i -= 2)
    *(uint16 *)((uint8 *)ram3000.pause_menu_map_tilemap + (uint16)i) = 0;
  WriteRegWord(VMADDL, addr_unk_605880);
  ReadRegWord(RDVRAML);
  WriteRegWord(DMAP1, 0x3981);
  WriteRegWord(A1T1L, ADDR16_OF_RAM(ram4000) + 256);
  WriteRegWord(A1B1, 0x7E);
  WriteRegWord(DAS1L, 0x700);
  WriteRegWord(DAS10, 0);
  WriteRegWord(A2A1H, 0);
  WriteReg(VMAIN, 0x80);
  WriteReg(MDMAEN, 2);
}

static void ClearMessageBoxBg3Tilemap(void) {  // 0x8581F3
  int16 v0;
  v0 = 1790;
  do {
    *(uint16 *)((uint8 *)ram3800.cinematic_bg_tilemap + (uint16)v0) = 0xe;
    v0 -= 2;
  } while (v0 >= 0);
  WriteRegWord(VMADDL, addr_unk_605880);
  WriteRegWord(DMAP1, 0x1801);
  WriteRegWord(A1T1L, ADDR16_OF_RAM(ram3800));
  WriteRegWord(A1B1, 0x7E);
  WriteRegWord(DAS1L, 0x700);
  WriteRegWord(DAS10, 0);
  WriteRegWord(A2A1H, 0);
  WriteReg(VMAIN, 0x80);
  WriteReg(MDMAEN, 2);
}

static const uint16 kLargeMsgBoxTopBottomBorderTilemap[32] = {  // 0x85825A
     0xe,    0xe,    0xe, 0x284e, 0x284e, 0x284e, 0x284e, 0x284e, 0x284e, 0x280f, 0x280f, 0x280f, 0x280f, 0x280f, 0x280f, 0x280f,
  0x280f, 0x280f, 0x280f, 0x280f, 0x280f, 0x280f, 0x280f, 0x280f, 0x280f, 0x280f, 0x280f, 0x284e, 0x284e,    0xe,    0xe,    0xe,
};
static const uint16 kSmallMsgBoxTopBottomBorderTilemap[32] = {
     0xe,    0xe,    0xe,    0xe,    0xe,    0xe, 0x284e, 0x284e, 0x284e, 0x284e, 0x284e, 0x284e, 0x284e, 0x284e, 0x284e, 0x284e,
  0x284e, 0x284e, 0x284e, 0x284e, 0x284e, 0x284e, 0x284e, 0x284e, 0x284e,    0xe,    0xe,    0xe,    0xe,    0xe,    0xe,    0xe,
};

static uint16 WriteMessageTilemap(void) {  // 0x8582B8
  message_box_animation_y1 = 112;
  message_box_animation_y0 = 124;
  message_box_animation_y_radius = 0;
  for (int i = 0; i != 112; ++i)
    ram3000.pause_menu_map_tilemap[i] = 0;
  uint16 r0 = kMessageBoxDefs[message_box_index - 1].message_tilemap;
  uint16 r9 = kMessageBoxDefs[message_box_index].message_tilemap - r0;
  int n = r9 >> 1;
  r9 += 128;
  message_box_das0l_value = r9;
  uint16 v1 = 32;
  uint16 v2 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v1 + 256] = *(uint16 *)&RomPtr_85(r0)[v2];
    ++v1;
    v2 += 2;
  } while (--n);
  return v1;
}

static void WriteLargeMessageBoxTilemap(void) {
  for (int i = 0; i != 32; ++i)
    ram3000.pause_menu_map_tilemap[i + 256] = kLargeMsgBoxTopBottomBorderTilemap[i];
  uint16 i = WriteMessageTilemap();
  int n = 32;
  uint16 v1 = 0;
  do {
    ram3000.pause_menu_map_tilemap[i + 256] = kLargeMsgBoxTopBottomBorderTilemap[v1 >> 1];
    v1 += 2;
    ++i;
  } while (--n);
}

static void WriteSmallMessageBoxTilemap(void) {  // 0x858289
  for (int i = 0; i != 32; ++i)
    ram3000.pause_menu_map_tilemap[i + 256] = kSmallMsgBoxTopBottomBorderTilemap[i];
  uint16 i = WriteMessageTilemap();
  int n = 32;
  uint16 v1 = 0;
  do {
    ram3000.pause_menu_map_tilemap[i + 256] = kSmallMsgBoxTopBottomBorderTilemap[v1 >> 1];
    v1 += 2;
    ++i;
  } while (--n);
}

static void CallMsgBoxDraw(uint32 ea) {
  switch (ea) {
  case fnWriteLargeMessageBoxTilemap: WriteLargeMessageBoxTilemap(); return;
  case fnWriteSmallMessageBoxTilemap: WriteSmallMessageBoxTilemap(); return;
  default: Unreachable();
  }
}

static void MsgBoxMakeHdmaTable(void) {
  message_box_animation_y2 = (uint16)(31488 - message_box_animation_y_radius) >> 8;
  message_box_animation_y3 = 99;
  message_box_animation_y0 = (uint16)(message_box_animation_y_radius + 31744) >> 8;
  message_box_animation_y1 = 148;
  uint16 v0 = 123;
  uint16 v1 = 124;
  uint16 r20 = 30;
  do {
    ram3000.pause_menu_map_tilemap[v0] = message_box_animation_y3 - message_box_animation_y2;
    --message_box_animation_y3;
    --message_box_animation_y2;
    ram3000.pause_menu_map_tilemap[v1] = message_box_animation_y1 - message_box_animation_y0;
    --v0;
    ++message_box_animation_y1;
    ++message_box_animation_y0;
    ++v1;
  } while (--r20);
  uint16 v2 = v1 * 2;
  do {
    *(uint16 *)((uint8 *)ram3000.pause_menu_map_tilemap + v2) = 0;
    v2 += 2;
  } while ((int16)(v2 - 480) < 0);
}

static void SetupMessageBoxBg3YscrollHdma(void) {  // 0x858363
  *(uint32 *)&ram3000.menu.palette_backup_in_menu[64] = -516947713;
  ram3000.pause_menu_map_tilemap[450] = 12542;
  ram3000.msgbox.indirect_hdma[6] = 0;
  WriteReg(DMAP6, 0x42);
  WriteReg(BBAD6, 0x12);
  WriteReg(A1T6L, 0x80);
  WriteReg(DAS6L, 0x80);
  WriteReg(A1T6H, 0x33);
  WriteReg(DAS6H, 0x33);
  WriteReg(A1B6, 0x7E);
  WriteReg(DAS60, 0x7E);
  WriteReg(A2A6L, 0);
  WriteReg(A2A6H, 0);
  WriteReg(NTRL6, 0);
  MsgBoxMakeHdmaTable();
  WriteReg(HDMAEN, 0x40);
}

static void SetupPpuForActiveMessageBox(uint16 r52) {  // 0x85831E
  SetupMessageBoxBg3YscrollHdma();
  r52 += 22528;
  WriteRegWord(VMADDL, r52);
  WriteRegWord(DMAP1, 0x1801);
  WriteRegWord(A1T1L, ADDR16_OF_RAM(ram3000) + 512);
  WriteRegWord(A1B1, 0x7E);
  WriteRegWord(DAS1L, message_box_das0l_value);
  WriteRegWord(DAS10, 0);
  WriteRegWord(A2A1H, 0);
  WriteReg(VMAIN, 0x80);
  WriteReg(MDMAEN, 2);
}

static void DrawSpecialButtonAndSetupPpuForLargeMessageBox(uint16 a) {
  uint16 v1 = 0;
  if ((a & kButton_A) == 0) {
    v1 = 2;
    if ((a & 0x8000) == 0) {
      v1 = 4;
      if ((a & kButton_X) == 0) {
        v1 = 6;
        if ((a & kButton_Y) == 0) {
          v1 = 8;
          if ((a & kButton_Select) == 0) {
            v1 = 10;
            if ((a & kButton_L) == 0) {
              v1 = 12;
              if ((a & kButton_R) == 0)
                v1 = 14;
            }
          }
        }
      }
    }
  }
  *(uint16 *)((uint8 *)&ram3000.pause_menu_map_tilemap[256] + kMsgBoxSpecialButtonTilemapOffs[message_box_index - 1]) = kTileNumbersForButtonLetters[v1 >> 1];
  SetupPpuForActiveMessageBox(416);
}

static void DrawShootButtonAndSetupPpuForLargeMessageBox(void) {  // 0x8583C5
  DrawSpecialButtonAndSetupPpuForLargeMessageBox(button_config_shoot_x);
}

static void DrawRunButtonAndSetupPpuForLargeMessageBox(void) {  // 0x8583CC
  DrawSpecialButtonAndSetupPpuForLargeMessageBox(button_config_run_b);
}

static void SetupPpuForSmallMessageBox(void) {  // 0x858436
  SetupPpuForActiveMessageBox(448);
}

static void SetupPpuForLargeMessageBox(void) {  // 0x858441
  SetupPpuForActiveMessageBox(416);
}

static void CallMsgBoxModify(uint32 ea) {
  switch (ea) {
  case fnDrawShootButtonAndSetupPpuForLargeMessageBox: DrawShootButtonAndSetupPpuForLargeMessageBox(); return;
  case fnDrawRunButtonAndSetupPpuForLargeMessageBox: DrawRunButtonAndSetupPpuForLargeMessageBox(); return;
  case fnSetupPpuForSmallMessageBox: SetupPpuForSmallMessageBox(); return;
  case fnSetupPpuForLargeMessageBox: SetupPpuForLargeMessageBox(); return;
  default: Unreachable();
  }
}

static void InitializeMessageBox(void) {  // 0x858241
  int v0 = message_box_index - 1;
  CallMsgBoxDraw(kMessageBoxDefs[v0].draw_initial_tilemap | 0x850000);
  CallMsgBoxModify(kMessageBoxDefs[v0].modify_box_func | 0x850000);
}


static const uint16 kSaveConfirmationSelectionTilemap[96] = {  // 0x858507
     0xe,    0xe,    0xe,    0xe,    0xe,    0xe, 0x3c4e, 0x3c4e, 0x38cc, 0x38cd, 0x3cf8, 0x3ce4, 0x3cf2, 0x3c4e, 0x3c4e, 0x3c4e,
  0x3c4e, 0x3c4e, 0x3c4e, 0x2ced, 0x2cee, 0x3c4e, 0x3c4e, 0x3c4e, 0x3c4e,    0xe,    0xe,    0xe,    0xe,    0xe,    0xe,    0xe,
     0xe,    0xe,    0xe,    0xe,    0xe,    0xe, 0x3c4e, 0x3c4e, 0x38cc, 0x38cd, 0x3cf8, 0x3ce4, 0x3cf2, 0x3c4e, 0x3c4e, 0x3c4e,
  0x3c4e, 0x3c4e, 0x3c4e, 0x2ced, 0x2cee, 0x3c4e, 0x3c4e, 0x3c4e, 0x3c4e,    0xe,    0xe,    0xe,    0xe,    0xe,    0xe,    0xe,
     0xe,    0xe,    0xe,    0xe,    0xe,    0xe, 0x3c4e, 0x3c4e, 0x3c4e, 0x3c4e, 0x2cf8, 0x2ce4, 0x2cf2, 0x3c4e, 0x3c4e, 0x3c4e,
  0x3c4e, 0x38cc, 0x38cd, 0x3ced, 0x3cee, 0x3c4e, 0x3c4e, 0x3c4e, 0x3c4e,    0xe,    0xe,    0xe,    0xe,    0xe,    0xe,    0xe,
};

static void ToggleSaveConfirmationSelection(void) {
  save_confirmation_selection ^= 2;
  uint16 v0 = 64;
  if (save_confirmation_selection == 2)
    v0 = 128;
  uint16 v1 = 128;
  int r52 = 32;
  do {
    ram3000.pause_menu_map_tilemap[v1 + 256] = kSaveConfirmationSelectionTilemap[v0 >> 1];
    ++v1;
    v0 += 2;
  } while (--r52);
  WriteRegWord(VMADDL, addr_unk_6059A0);
  WriteRegWord(DMAP1, 0x1801);
  WriteRegWord(A1T1L, ADDR16_OF_RAM(ram3000) + 512);
  WriteRegWord(A1B1, 0x7E);
  WriteRegWord(DAS1L, 0x180);
  WriteRegWord(DAS10, 0);
  WriteRegWord(A2A1H, 0);
  WriteReg(VMAIN, 0x80);
  WriteReg(MDMAEN, 2);
}

static void RestorePpuForMessageBox(void) {  // 0x85861A
  WriteRegWord(VMADDL, addr_unk_605880);
  WriteRegWord(DMAP1, 0x1801);
  WriteRegWord(A1T1L, ADDR16_OF_RAM(ram4000) + 256);
  WriteRegWord(A1B1, 0x7E);
  WriteRegWord(DAS1L, 0x700);
  WriteRegWord(DAS10, 0);
  WriteRegWord(A2A1H, 0);
  WriteReg(VMAIN, 0x80);
  WriteReg(MDMAEN, 2);
  reg_HDMAEN = ram3000.msgbox.backup_of_enabled_hdma_channels;
  WriteReg(HDMAEN, ram3000.msgbox.backup_of_enabled_hdma_channels);
  gameplay_BG3SC = ram3000.misc.field_3E8[3];
  gameplay_TM = reg_TM;
  gameplay_CGWSEL = next_gameplay_CGWSEL;
  gameplay_CGADSUB = next_gameplay_CGADSUB;
  WriteReg(CGADD, 0x19);
  WriteReg(CGDATA, palette_buffer[25]);
  WriteReg(CGDATA, HIBYTE(palette_buffer[25]));
  WriteReg(CGDATA, palette_buffer[26]);
  WriteReg(CGDATA, HIBYTE(palette_buffer[26]));
}

static CoroutineRet OpenMessageBox_Async(void) {  // 0x85844C
  COROUTINE_BEGIN(coroutine_state_4, 0);
  for (message_box_animation_y_radius = 0;; message_box_animation_y_radius += 512) {
    COROUTINE_AWAIT(1, WaitForNMI_NoUpdate_Async());
    HandleMusicQueue();
    HandleSoundEffects();
    MsgBoxMakeHdmaTable();
    if (message_box_animation_y_radius == 0x1800)
      break;
  }
  COROUTINE_END(0);
}

static CoroutineRet HandleMessageBoxInteraction_Async(void) {  // 0x85846D
  COROUTINE_BEGIN(coroutine_state_4, 0);

  if (message_box_index == 23 || message_box_index == 28) {
    save_confirmation_selection = 0;
    while (1) {
      do {
        COROUTINE_AWAIT(1, WaitForNMI_NoUpdate_Async());
        HandleMusicQueue();
        HandleSoundEffects();
        ReadJoypadInputs();
      } while (!joypad1_newkeys);
      if ((joypad1_newkeys & kButton_A) != 0)
        break;
      if ((joypad1_newkeys & kButton_B) != 0) {
        save_confirmation_selection = 2;
        goto GETOUT;
      }
      if ((joypad1_newkeys & (kButton_Select | kButton_Left | kButton_Right)) != 0) {
        ToggleSaveConfirmationSelection();
        QueueSfx1_Max6(0x37);
      }
    }
  } else {
    my_counter = 10;
    if (message_box_index != 20 && message_box_index != 21 && message_box_index != 22 && message_box_index != 24)
      my_counter = 360;
    do {
      COROUTINE_AWAIT(2, WaitForNMI_NoUpdate_Async());
      HandleMusicQueue();
      HandleSoundEffects();
    } while (--my_counter);
    do {
      COROUTINE_AWAIT(3, WaitForNMI_NoUpdate_Async());
      ReadJoypadInputs();
    } while ((bug_fix_counter < 1 ? joypad1_newkeys : joypad1_lastkeys) == 0);
  }
GETOUT:;
  COROUTINE_END(0);
}

static CoroutineRet CloseMessageBox_Async(void) {  // 0x858589
  COROUTINE_BEGIN(coroutine_state_4, 0);
  do {
    COROUTINE_AWAIT(1, WaitForNMI_NoUpdate_Async());
    HandleMusicQueue();
    HandleSoundEffects();
    MsgBoxMakeHdmaTable();
    message_box_animation_y_radius -= 512;
  } while ((message_box_animation_y_radius & 0x8000) == 0);
  COROUTINE_END(0);
}

CoroutineRet DisplayMessageBox_Async(uint16 a) {  // 0x858080
  COROUTINE_BEGIN(coroutine_state_3, 0)
  message_box_index = a;
  CancelSoundEffects();
  InitializePpuForMessageBoxes();
  ClearMessageBoxBg3Tilemap();
  InitializeMessageBox();
  COROUTINE_AWAIT(2, OpenMessageBox_Async());
  COROUTINE_AWAIT(3, HandleMessageBoxInteraction_Async());
  COROUTINE_AWAIT(4, CloseMessageBox_Async());

  if (message_box_index == 28 && save_confirmation_selection != 2) {
    message_box_index = 24;
    ClearMessageBoxBg3Tilemap();
    QueueSfx1_Max6(0x2E);

    my_counter = 160;
    do {
      HandleMusicQueue();
      HandleSoundEffects();
      COROUTINE_AWAIT(8, WaitForNMI_NoUpdate_Async());
    } while (--my_counter);

    InitializeMessageBox();
    COROUTINE_AWAIT(5, OpenMessageBox_Async());
    COROUTINE_AWAIT(6, HandleMessageBoxInteraction_Async());
    COROUTINE_AWAIT(7, CloseMessageBox_Async());
    message_box_index = 28;
  }
  ClearMessageBoxBg3Tilemap();
  RestorePpuForMessageBox();
  QueueSamusMovementSfx();

  if (message_box_index == 20) {
    game_state = kGameState_12_Pausing;
  }
  COROUTINE_END(0);
  //  else if (message_box_index == 28 || message_box_index == 23) {
  //    return save_confirmation_selection;
  //  }
  //  return 0;
}

int DisplayMessageBox_Poll(uint16 a) {
  if (a == message_box_index) {
    message_box_index = 0;
    return save_confirmation_selection;
  }
  queued_message_box_index = a;
  return -1;
}

void DisplayMessageBox(uint16 a) {  // 0x858080
  queued_message_box_index = a;
}
