// Non gameplay routines
#include "sm_rtl.h"
#include "ida_types.h"
#include "funcs.h"
#include "variables.h"

#define kPalettes_Intro ((uint16*)RomPtr(0x8ce3e9))
#define kPalettes_Intro2 ((uint16*)RomPtr(0x8ce5e9))
#define kPalettes_Intro4 ((uint16*)RomPtr(0x8ce7e9))
#define kPalettes_Intro5 ((uint16*)RomPtr(0x8ce9e9))
#define kPalettes_Intro6 ((uint16*)RomPtr(0x8cebe9))
#define kPalettes_Intro3 ((uint16*)RomPtr(0x8cede9))
#define g_word_8CD81B ((uint16*)RomPtr(0x8cd81b))
#define g_word_8BA72B ((uint16*)RomPtr(0x8ba72b))
#define kLevelData_MotherBrainRoomFromCutscene ((uint16*)RomPtr(0x8cbec3))
#define kLevelData_RoomWithBabyMetroidHatching ((uint16*)RomPtr(0x8cc083))
#define g_word_8CDC9B ((uint16*)RomPtr(0x8cdc9b))
#define g_word_8CDEDB ((uint16*)RomPtr(0x8cdedb))
#define g_word_8CEFE9 ((uint16*)RomPtr(0x8cefe9))
#define g_off_8BE70D ((uint16*)RomPtr(0x8be70d))
#define g_word_8BE717 ((uint16*)RomPtr(0x8be717))
#define g_word_8BE721 ((uint16*)RomPtr(0x8be721))
#define g_word_8BE737 ((uint16*)RomPtr(0x8be737))
#define g_word_8BE741 ((uint16*)RomPtr(0x8be741))
#define g_word_8BE9CF ((uint16*)RomPtr(0x8be9cf))
#define kCinematicFunction_Intro_Func142_Tab0 ((uint16*)RomPtr(0x8be45a))
#define kCinematicFunction_Intro_Func144_Tab0 ((uint16*)RomPtr(0x8be5e7))
#define g_word_8CDF5B ((uint16*)RomPtr(0x8cdf5b))
#define g_word_8BE9A7 ((uint16*)RomPtr(0x8be9a7))
#define g_word_8BF6B8 ((uint16*)RomPtr(0x8bf6b8))
#define g_word_8BF6D8 ((uint16*)RomPtr(0x8bf6d8))
#define g_word_8CE1E9 ((uint16*)RomPtr(0x8ce1e9))

void SetupPpuForTitleSequence(void) {  // 0x8B8000
  WriteReg(INIDISP, 0x80);
  reg_INIDISP = 0x80;
  WriteReg(OBSEL, 3u);
  reg_OBSEL = 3;
  WriteReg(BGMODE, 7u);
  reg_BGMODE = 7;
  WriteReg(M7SEL, 0x80);
  reg_M7SEL = 0x80;
  reg_BG1SC = 0;
  WriteReg(BG1SC, 0);
  reg_BG2SC = 0;
  WriteReg(BG2SC, 0);
  reg_BG3SC = 0;
  WriteReg(BG3SC, 0);
  reg_BG4SC = 0;
  WriteReg(BG4SC, 0);
  reg_BG12NBA = 0;
  WriteReg(BG12NBA, 0);
  reg_BG34NBA = 0;
  WriteReg(BG34NBA, 0);
  WriteReg(TM, 0x10u);
  reg_TM = 16;
  WriteReg(TS, 0);
  reg_TS = 0;
  WriteReg(TMW, 0);
  reg_TMW = 0;
  WriteReg(VMAIN, 0);
  WriteReg(CGWSEL, 0);
  next_gameplay_CGWSEL = 0;
  WriteReg(CGADSUB, 0);
  next_gameplay_CGADSUB = 0;
  WriteReg(COLDATA, 0xE0u);
  WriteReg(SETINI, 0);
  reg_SETINI = 0;
  memset7E(0x3000u, 0, 0x7FEu);
  oam_next_ptr = 0;
  menu_option_index = 0;
  screen_fade_delay = 0;
  screen_fade_counter = 0;
  pausemenu_start_lr_pressed_highlight_timer = 0;
  pausemenu_lr_animation_timer = 0;
  pausemenu_item_selector_animation_timer = 0;
  pausemenu_reserve_tank_animation_timer = 0;
  UNUSED_word_7E0731 = 0;
  UNUSED_word_7E073D = 0;
  pausemenu_lr_animation_frame = 0;
  pausemenu_item_selector_animation_frame = 0;
  pausemenu_reserve_tank_animation_frame = 0;
  UNUSED_word_7E0745 = 0;
  pausemenu_shoulder_button_highlight = 0;
  pausemenu_button_label_mode = 0;
  pausemenu_equipment_category_item = 0;
  pausemenu_reserve_tank_delay_ctr = 0;
  UNUSED_word_7E0759 = 0;
  samus_reserve_health = 0;
  samus_reserve_missiles = 0;
  cinematic_var5 = 0;
  cinematic_var6 = 256;
  cinematic_var7 = 0;
  cinematic_var8 = 0;
  cinematic_var9 = 0;
  cinematic_var10 = 0;
  g_word_7E1999 = 0;
  g_word_7E199B = 0;
  g_word_7E199D = 0;
  g_word_7E199F = 0;
}

void SetupPpu_Intro(void) {  // 0x8B80DA
  WriteReg(INIDISP, 0x80);
  reg_INIDISP = 0x80;
  WriteReg(OBSEL, 3u);
  reg_OBSEL = 3;
  WriteReg(BGMODE, 9u);
  reg_BGMODE = 9;
  WriteReg(M7SEL, 0);
  reg_M7SEL = 0;
  reg_BG1SC = 80;
  WriteReg(BG1SC, 0x50u);
  reg_BG2SC = 72;
  WriteReg(BG2SC, 0x48u);
  reg_BG3SC = 76;
  WriteReg(BG3SC, 0x4Cu);
  reg_BG4SC = 0;
  WriteReg(BG4SC, 0);
  reg_BG12NBA = 0;
  WriteReg(BG12NBA, 0);
  reg_BG34NBA = 4;
  WriteReg(BG34NBA, 4u);
  WriteReg(TM, 4u);
  reg_TM = 4;
  WriteReg(TS, 0);
  reg_TS = 0;
  WriteReg(TMW, 0);
  reg_TMW = 0;
  WriteReg(VMAIN, 0);
  WriteReg(CGWSEL, 0);
  next_gameplay_CGWSEL = 0;
  WriteReg(CGADSUB, 0);
  next_gameplay_CGADSUB = 0;
  WriteReg(COLDATA, 0xE0u);
  WriteReg(SETINI, 0);
  reg_SETINI = 0;
  reg_CGWSEL = 0;
  reg_CGADSUB = 0;
  reg_COLDATA[0] = 32;
  reg_COLDATA[1] = 64;
  reg_COLDATA[2] = 0x80;
  reg_MOSAIC = 0;
  memset7E(0x3000u, 0x2Fu, 0x7FEu);
  oam_next_ptr = 0;
  cinematic_var5 = 0;
  cinematic_var6 = 256;
  cinematic_var7 = 0;
  cinematic_var8 = 0;
  cinematic_var9 = 0;
  cinematic_var10 = 0;
  g_word_7E1999 = 0;
  g_word_7E199B = 0;
  g_word_7E199D = 0;
  g_word_7E199F = 0;
  fx_layer_blending_config_a = 0;
  reg_BG2HOFS = 0;
  reg_BG2VOFS = 0;
  reg_BG3HOFS = 0;
  reg_BG3VOFS = 0;
}

void SetupPpu_3_Mode7(void) {  // 0x8B819B
  WriteReg(INIDISP, 0x80);
  reg_INIDISP = 0x80;
  WriteReg(OBSEL, 3u);
  reg_OBSEL = 3;
  WriteReg(BGMODE, 7u);
  reg_BGMODE = 7;
  WriteReg(M7SEL, 0x80);
  reg_M7SEL = 0x80;
  reg_BG1SC = 0;
  WriteReg(BG1SC, 0);
  reg_BG2SC = 0;
  WriteReg(BG2SC, 0);
  reg_BG3SC = 0;
  WriteReg(BG3SC, 0);
  reg_BG4SC = 0;
  WriteReg(BG4SC, 0);
  reg_BG12NBA = 0;
  WriteReg(BG12NBA, 0);
  reg_BG34NBA = 0;
  WriteReg(BG34NBA, 0);
  WriteReg(TM, 0x11u);
  reg_TM = 17;
  WriteReg(TS, 0);
  reg_TS = 0;
  WriteReg(TMW, 0);
  reg_TMW = 0;
  WriteReg(VMAIN, 0);
  WriteReg(CGWSEL, 0);
  next_gameplay_CGWSEL = 0;
  WriteReg(CGADSUB, 0);
  next_gameplay_CGADSUB = 0;
  WriteReg(COLDATA, 0xE0u);
  reg_CGWSEL = 0;
  reg_CGADSUB = 0;
  reg_COLDATA[0] = 32;
  reg_COLDATA[1] = 64;
  reg_COLDATA[2] = 0x80;
  reg_MOSAIC = 0;
  cinematic_var7 = 0;
  cinematic_var8 = 0;
  cinematic_var9 = 0;
  cinematic_var10 = 0;
  g_word_7E1999 = 0;
  g_word_7E199B = 0;
  g_word_7E199D = 0;
  g_word_7E199F = 0;
  fx_layer_blending_config_a = 0;
}

void SetupPpu_4_Mode1(void) {  // 0x8B8230
  WriteReg(INIDISP, 0x80);
  reg_INIDISP = 0x80;
  WriteReg(OBSEL, 3u);
  reg_OBSEL = 3;
  WriteReg(BGMODE, 1u);
  reg_BGMODE = 1;
  WriteReg(M7SEL, 0);
  reg_M7SEL = 0;
  reg_BG1SC = 92;
  WriteReg(BG1SC, 0x5Cu);
  reg_BG2SC = 0;
  WriteReg(BG2SC, 0);
  reg_BG3SC = 0;
  WriteReg(BG3SC, 0);
  reg_BG4SC = 0;
  WriteReg(BG4SC, 0);
  reg_BG12NBA = 6;
  WriteReg(BG12NBA, 6u);
  reg_BG34NBA = 0;
  WriteReg(BG34NBA, 0);
  WriteReg(TM, 0x11u);
  reg_TM = 17;
  WriteReg(TS, 0);
  reg_TS = 0;
  reg_W12SEL = 0;
  reg_W34SEL = 0;
  reg_WOBJSEL = 0;
  reg_TMW = 0;
  reg_TSW = 0;
  reg_CGWSEL = 0;
  reg_CGADSUB = 0;
  reg_BG1HOFS = 0;
  reg_BG1VOFS = 0;
}

void SetupPpu_5_Mode7(void) {  // 0x8B8293
  WriteReg(INIDISP, 0x80);
  reg_INIDISP = 0x80;
  WriteReg(OBSEL, 0xA3u);
  reg_OBSEL = -93;
  WriteReg(BGMODE, 7u);
  reg_BGMODE = 7;
  WriteReg(M7SEL, 0);
  reg_M7SEL = 0;
  reg_BG1SC = 0;
  WriteReg(BG1SC, 0);
  reg_BG2SC = 0;
  WriteReg(BG2SC, 0);
  reg_BG3SC = 0;
  WriteReg(BG3SC, 0);
  reg_BG4SC = 0;
  WriteReg(BG4SC, 0);
  reg_BG12NBA = 0;
  WriteReg(BG12NBA, 0);
  reg_BG34NBA = 0;
  WriteReg(BG34NBA, 0);
  WriteReg(TM, 0x11u);
  reg_TM = 17;
  WriteReg(TS, 0);
  reg_TS = 0;
  WriteReg(TMW, 0);
  reg_TMW = 0;
  WriteReg(VMAIN, 0);
  WriteReg(CGWSEL, 0);
  next_gameplay_CGWSEL = 0;
  WriteReg(CGADSUB, 0);
  next_gameplay_CGADSUB = 0;
  reg_CGWSEL = 0;
  reg_CGADSUB = 0;
  WriteReg(COLDATA, 0xE0u);
  WriteReg(SETINI, 0);
  reg_SETINI = 0;
  reg_COLDATA[0] = 32;
  reg_COLDATA[1] = 64;
  reg_COLDATA[2] = 0x80;
  oam_next_ptr = 0;
  screen_fade_delay = 0;
  screen_fade_counter = 0;
  cinematic_var5 = 0;
  cinematic_var6 = 256;
  cinematic_var7 = 0;
  cinematic_var8 = 0;
  cinematic_var9 = 0;
  cinematic_var10 = 0;
  g_word_7E1999 = 0;
  g_word_7E199B = 0;
  g_word_7E199D = 0;
  g_word_7E199F = 0;
}

void SetupPpu_6_Mode1(void) {  // 0x8B833A
  WriteReg(INIDISP, 0x80);
  reg_INIDISP = 0x80;
  WriteReg(OBSEL, 2u);
  reg_OBSEL = 2;
  WriteReg(BGMODE, 1u);
  reg_BGMODE = 1;
  WriteReg(M7SEL, 0);
  reg_M7SEL = 0;
  reg_BG1SC = 112;
  WriteReg(BG1SC, 0x70u);
  reg_BG2SC = 120;
  WriteReg(BG2SC, 0x78u);
  reg_BG3SC = 0;
  WriteReg(BG3SC, 0);
  reg_BG4SC = 0;
  WriteReg(BG4SC, 0);
  reg_BG12NBA = 68;
  WriteReg(BG12NBA, 0x44u);
  reg_BG34NBA = 0;
  WriteReg(BG34NBA, 0);
  WriteReg(TM, 0x10u);
  reg_TM = 16;
  WriteReg(TS, 0);
  reg_TS = 0;
  WriteReg(TMW, 0);
  reg_TMW = 0;
  WriteReg(VMAIN, 0);
  WriteReg(CGWSEL, 0);
  next_gameplay_CGWSEL = 0;
  WriteReg(CGADSUB, 0);
  next_gameplay_CGADSUB = 0;
  WriteReg(COLDATA, 0xE0u);
  reg_CGWSEL = 0;
  reg_CGADSUB = 0;
  reg_COLDATA[0] = 32;
  reg_COLDATA[1] = 64;
  reg_COLDATA[2] = 0x80;
  reg_MOSAIC = 0;
  cinematic_var7 = 0;
  cinematic_var8 = 0;
  cinematic_var9 = 0;
  cinematic_var10 = 0;
  g_word_7E1999 = 0;
  g_word_7E199B = 0;
  g_word_7E199D = 0;
  g_word_7E199F = 0;
  fx_layer_blending_config_a = 0;
  reg_BG2HOFS = 0;
  reg_BG2VOFS = 0;
}

void SetupPpu_7_Mode1(void) {  // 0x8B83D3
  WriteReg(INIDISP, 0x80);
  reg_INIDISP = 0x80;
  WriteReg(OBSEL, 0);
  reg_OBSEL = 0;
  WriteReg(BGMODE, 1u);
  reg_BGMODE = 1;
  WriteReg(M7SEL, 0);
  reg_M7SEL = 0;
  reg_BG1SC = 72;
  WriteReg(BG1SC, 0x48u);
  reg_BG2SC = 76;
  WriteReg(BG2SC, 0x4Cu);
  reg_BG3SC = 36;
  WriteReg(BG3SC, 0x24u);
  reg_BG4SC = 0;
  WriteReg(BG4SC, 0);
  reg_BG12NBA = 84;
  WriteReg(BG12NBA, 0x54u);
  reg_BG34NBA = 2;
  WriteReg(BG34NBA, 2u);
  WriteReg(TM, 1u);
  reg_TM = 1;
  WriteReg(TS, 0);
  reg_TS = 0;
  reg_W12SEL = 0;
  reg_W34SEL = 0;
  reg_WOBJSEL = 0;
  reg_TMW = 0;
  reg_TSW = 0;
  reg_CGWSEL = 0;
  reg_CGADSUB = 0;
  reg_COLDATA[0] = 32;
  reg_COLDATA[1] = 64;
  reg_COLDATA[2] = 0x80;
  oam_next_ptr = 0;
  WriteRegWord(M7A, 0x100u);
  reg_M7A = 256;
  WriteRegWord(M7B, 0);
  reg_M7B = 0;
  WriteRegWord(M7C, 0);
  reg_M7C = 0;
  WriteRegWord(M7D, 0x100u);
  reg_M7D = 256;
  WriteRegWord(M7X, 0x80);
  reg_M7X = 128;
  WriteRegWord(M7Y, 0x80);
  reg_M7Y = 128;
  cinematic_var5 = 0;
  cinematic_var6 = 1;
  reg_BG1HOFS = 0;
  reg_BG1VOFS = 0;
  reg_BG2HOFS = 0;
  reg_BG2VOFS = 0;
  cinematic_var7 = 0;
  cinematic_var8 = 0;
  cinematic_var9 = 0;
  cinematic_var10 = 0;
}

void HandleMode7TransformationNoRotation(void) {  // 0x8B8518
  reg_M7A = cinematic_var6;
  reg_M7D = cinematic_var6;
  reg_M7B = 0;
  reg_M7C = 0;
  reg_BG1HOFS = cinematic_var8;
  reg_BG1VOFS = cinematic_var10;
}

void HandleMode7TransformationMatrix(void) {  // 0x8B8532
  uint16 v0;
  uint16 v1;

  R38 = kSinCosTable8bit_Sext[((uint16)(2 * (uint8)(cinematic_var5 + 64)) >> 1) + 64];
  R40 = cinematic_var6;
  Smult16x16Shr16();
  HIBYTE(v0) = R42;
  LOBYTE(v0) = HIBYTE(R44);
  reg_M7A = v0;
  reg_M7D = v0;
  R38 = kSinCosTable8bit_Sext[((uint16)(2 * (uint8)cinematic_var5) >> 1) + 64];
  R40 = cinematic_var6;
  Smult16x16Shr16();
  HIBYTE(v1) = R42;
  LOBYTE(v1) = HIBYTE(R44);
  reg_M7B = v1;
  reg_M7C = -v1;
  reg_BG1HOFS = cinematic_var8;
  reg_BG1VOFS = cinematic_var10;
}


void Smult16x16Shr16(void) {  // 0x8B858F
  if ((R38 & 0x8000u) == 0) {
    if ((R40 & 0x8000u) != 0) {
      R40 = -R40;
      Umult16x16Shr16();
      Negate32(&R42, &R44, &R42, &R44);
    } else {
      Umult16x16Shr16();
    }
  } else {
    R38 = -R38;
    if ((R40 & 0x8000u) != 0) {
      R40 = -R40;
      Umult16x16Shr16();
    } else {
      Umult16x16Shr16();
      Negate32(&R42, &R44, &R42, &R44);
    }
  }
}

void Umult16x16Shr16(void) {  // 0x8B85EE
  R44 = Mult8x8(R38, R40);
  R46 = Mult8x8(HIBYTE(R38), R40);
  R48 = Mult8x8(R38, HIBYTE(R40));
  uint16 RegWord = Mult8x8(HIBYTE(R38), HIBYTE(R40));
  R42 = RegWord;
  R46 += R48;
  bool v1 = __CFADD__uint8(HIBYTE(R44), (uint8)R46);
  HIBYTE(R44) += R46;
  LOBYTE(R42) = RegWord + v1 + HIBYTE(R46);
  if (__CFADD__uint8(v1, HIBYTE(R46)) | __CFADD__uint8((uint8)RegWord, v1 + HIBYTE(R46)))
    ++HIBYTE(R42);
}

void HandleFadeIn_(void) {  // 0x8B8666
  HandleFadeIn();
}

void SetPpuBackdropSomeColor(void) {  // 0x8B866B
  reg_CGWSEL = 0;
  reg_CGADSUB = -127;
  reg_COLDATA[0] = 56;
  reg_COLDATA[1] = 88;
  reg_COLDATA[2] = 0x80;
}

void SetPpuBackdropBlack(void) {  // 0x8B8682
  reg_CGWSEL = 0;
  reg_CGADSUB = 0;
  reg_COLDATA[0] = 32;
  reg_COLDATA[1] = 64;
  reg_COLDATA[2] = 0x80;
}

void UpdateCinematicBgTilemap(void) {  // 0x8B87A0
  if (reg_BGMODE != 7) {
    uint16 v0 = vram_write_queue_tail;
    gVramWriteEntry(vram_write_queue_tail)->size = 1920;
    v0 += 2;
    gVramWriteEntry(v0)->size = 12288;
    v0 += 2;
    LOBYTE(gVramWriteEntry(v0++)->size) = 126;
    gVramWriteEntry(v0)->size = cinematicbg_var3;
    vram_write_queue_tail = v0 + 2;
  }
}

void UpdateSamusEyesTilemap(void) {  // 0x8B87D3
  if (reg_BGMODE != 7) {
    uint16 v0 = vram_write_queue_tail;
    gVramWriteEntry(vram_write_queue_tail)->size = 128;
    v0 += 2;
    gVramWriteEntry(v0)->size = ADDR16_OF_RAM(ram3800) + 832;
    v0 += 2;
    LOBYTE(gVramWriteEntry(v0++)->size) = 126;
    gVramWriteEntry(v0)->size = 18848;
    vram_write_queue_tail = v0 + 2;
  }
}

void CinematicUpdateSomeBg(void) {  // 0x8B8806
  if (reg_BGMODE != 7) {
    uint16 v0 = vram_write_queue_tail;
    gVramWriteEntry(vram_write_queue_tail)->size = 2048;
    v0 += 2;
    gVramWriteEntry(v0)->size = ADDR16_OF_RAM(ram3000);
    v0 += 2;
    LOBYTE(gVramWriteEntry(v0++)->size) = 126;
    gVramWriteEntry(v0)->size = cinematicbg_var3;
    vram_write_queue_tail = v0 + 2;
  }
}

void CallProcessCinematicBgObject(uint32 ea, uint16 k, uint16 j) {
  switch (ea) {
  case fnProcessCinematicBgObject_Nothing: ProcessCinematicBgObject_Nothing(k, j); return;
  case fnProcessCinematicBgObject_DrawChar: ProcessCinematicBgObject_DrawChar(k, j); return;
  case fnProcessCinematicBgObject_DrawToTextTilemap: ProcessCinematicBgObject_DrawToTextTilemap(k, j); return;
  case fnProcessCinematicBgObject_DrawToBgTilemap: ProcessCinematicBgObject_DrawToBgTilemap(k, j); return;
  case fnProcessCinematicBgObject_Unk1: ProcessCinematicBgObject_Unk1(k, j); return;
  case fnProcessCinematicBgObject_Unk2: ProcessCinematicBgObject_Unk2(k, j); return;
  default: Unreachable();
  }
}
void ProcessCinematicBgObject(uint16 v0) {  // 0x8B8839
  uint16 j = cinematicbg_arr1[v0 >> 1];
  R28_ = *(uint16 *)RomPtr_8C(j);
  CallProcessCinematicBgObject(R28_ | 0x8B0000, v0, j);
}

void ProcessCinematicBgObject_DrawChar(uint16 k, uint16 j) {  // 0x8B884D
  int16 v3;

  cinematicbg_var1 = cinematicbg_var1 == 0;
  SpawnTextGlowObject(j);
  uint8 *v2 = RomPtr_8C(cinematicbg_instr_ptr[k >> 1]);
  if (*((int16 *)v2 + 3) < 0) {
    cinematicbg_arr7[15] = 8;
    v3 = 8 * (v2[3] + 2);
  } else {
    cinematicbg_arr7[15] = 8 * v2[8];
    v3 = 8 * v2[9];
  }
  cinematicbg_arr8[15] = v3 - 8;
  if (*((uint16 *)v2 + 2) != 0xD67D && cinematicbg_var1)
    QueueSfx3_Max6(0xDu);
  ProcessCinematicBgObject_DrawToTextTilemap(0x1Eu, j);
}

void ProcessCinematicBgObject_DrawToTextTilemap(uint16 k, uint16 j) {  // 0x8B88B7
  uint16 jorg = j;

  uint16 TilemapOffsetForTile = CinematicGetTilemapOffsetForTile();
  uint8 *v3 = RomPtr_8C(j);
  R18_ = v3[2];
  R24_ = R18_;
  R20_ = v3[3];
  while (1) {
    do {
      *(uint16 *)((char *)ram3000.pause_menu_map_tilemap + TilemapOffsetForTile) = *((uint16 *)RomPtr_8C(j) + 2);
      j += 2;
      TilemapOffsetForTile += 2;
      --R18_;
    } while (R18_);
    R18_ = R24_;
    if (!--R20_)
      break;
    R22_ += 64;
    TilemapOffsetForTile = R22_;
  }
}

void ProcessCinematicBgObject_Nothing(uint16 k, uint16 j) {  // 0x8B8849
}

void ProcessCinematicBgObject_DrawToBgTilemap(uint16 k, uint16 j) {  // 0x8B88FD
  uint16 jorg = j;

  uint16 TilemapOffsetForTile = CinematicGetTilemapOffsetForTile();
  uint8 *v3 = RomPtr_8C(j);
  R18_ = v3[2];
  R24_ = R18_;
  R20_ = v3[3];
  while (1) {
    do {
      *(uint16 *)((char *)ram3800.cinematic_bg_tilemap + TilemapOffsetForTile) = *((uint16 *)RomPtr_8C(j) + 2);
      j += 2;
      TilemapOffsetForTile += 2;
      --R18_;
    } while (R18_);
    R18_ = R24_;
    if (!--R20_)
      break;
    R22_ += 64;
    TilemapOffsetForTile = R22_;
  }
}

uint16 CinematicGetTilemapOffsetForTile(void) {  // 0x8B8943
  R20_ = 2 * (uint8)R18_;
  R22_ = R20_ + HIBYTE(R18_) * 0x40;
  return R22_;
}

void ProcessCinematicBgObject_Unk1(uint16 k, uint16 j) {  // 0x8B896B
  Mult0x80Add();
  uint8 *v2 = RomPtr_8C(j);
  R18_ = v2[2];
  R20_ = v2[3];
  uint16 v3 = j + 4;
  uint16 v4 = mode7_vram_write_queue_tail;
  do {
    *(uint16 *)(&mode7_write_queue[0].field_0 + v4) = 128;
    *(uint16 *)((char *)&mode7_write_queue[0].field_1 + v4) = v3;
    *(uint16 *)&mode7_write_queue[0].gap3[v4] = 139;
    *(uint16 *)&mode7_write_queue[0].gap3[v4 + 1] = R18_;
    *(uint16 *)&mode7_write_queue[0].gap3[v4 + 3] = R22_;
    *(uint16 *)((char *)&mode7_write_queue[1].field_1 + v4) = 0;
    v4 += 9;
    v3 += R18_;
    R22_ += 128;
    --R20_;
  } while (R20_);
  mode7_vram_write_queue_tail = v4;
}

void ProcessCinematicBgObject_Unk2(uint16 k, uint16 j) {  // 0x8B89CF
  Mult0x80Add();
  uint8 *v2 = RomPtr_8C(j);
  R18_ = v2[2];
  R20_ = v2[3];
  uint16 v3 = j + 4;
  uint16 v4 = mode7_vram_write_queue_tail;
  do {
    *(uint16 *)(&mode7_write_queue[0].field_0 + v4) = 128;
    *(uint16 *)((char *)&mode7_write_queue[0].field_1 + v4) = v3;
    *(uint16 *)&mode7_write_queue[0].gap3[v4] = 139;
    *(uint16 *)&mode7_write_queue[0].gap3[v4 + 1] = R20_;
    *(uint16 *)&mode7_write_queue[0].gap3[v4 + 3] = R22_;
    *(uint16 *)((char *)&mode7_write_queue[1].field_1 + v4) = 2;
    v4 += 9;
    v3 += R20_;
    ++R22_;
    --R18_;
  } while (R18_);
  mode7_vram_write_queue_tail = v4;
}

void Mult0x80Add(void) {  // 0x8B8A2C
  R20_ = (uint8)R18_;
  R22_ = R20_ + (0x80 * HIBYTE(R18_));
}

void Samus_CalcPos_Mode7(void) {  // 0x8B8A52
  uint16 v0;
  int16 v1;
  uint16 v2;
  int16 v3;

  R34 = samus_x_pos - reg_M7X;
  R36 = reg_M7Y - samus_y_pos;
  R38 = samus_x_pos - reg_M7X;
  R40 = reg_M7A;
  Smult16x16Shr16();
  HIBYTE(v0) = R42;
  LOBYTE(v0) = HIBYTE(R44);
  R26_ = v0;
  R38 = reg_M7B;
  R40 = R36;
  Smult16x16Shr16();
  HIBYTE(v1) = R42;
  LOBYTE(v1) = HIBYTE(R44);
  R26_ += v1;
  samus_x_pos = R26_ + reg_M7X;
  R38 = reg_M7C;
  R40 = R34;
  Smult16x16Shr16();
  HIBYTE(v2) = R42;
  LOBYTE(v2) = HIBYTE(R44);
  R26_ = v2;
  R38 = reg_M7A;
  R40 = R36;
  Smult16x16Shr16();
  HIBYTE(v3) = R42;
  LOBYTE(v3) = HIBYTE(R44);
  R26_ += v3;
  samus_y_pos = reg_M7Y - R26_;
}

void CalcExplosion_Mode7(uint16 k) {  // 0x8B8AD9
  uint16 v2;
  int16 v3;
  uint16 v4;
  int16 v5;

  int v1 = k >> 1;
  R34 = projectile_x_pos[v1] - reg_M7X;
  R36 = reg_M7Y - projectile_y_pos[v1];
  R38 = R34;
  R40 = reg_M7A;
  Smult16x16Shr16();
  HIBYTE(v2) = R42;
  LOBYTE(v2) = HIBYTE(R44);
  R26_ = v2;
  R38 = reg_M7B;
  R40 = R36;
  Smult16x16Shr16();
  HIBYTE(v3) = R42;
  LOBYTE(v3) = HIBYTE(R44);
  R26_ += v3;
  R20_ = R26_ + reg_M7X - layer1_x_pos;
  R38 = reg_M7C;
  R40 = R34;
  Smult16x16Shr16();
  HIBYTE(v4) = R42;
  LOBYTE(v4) = HIBYTE(R44);
  R26_ = v4;
  R38 = reg_M7A;
  R40 = R36;
  Smult16x16Shr16();
  HIBYTE(v5) = R42;
  LOBYTE(v5) = HIBYTE(R44);
  R26_ += v5;
  R18_ = reg_M7Y - R26_ - layer1_y_pos;
}

void CalcCeresSteamPos_Mode7(void) {  // 0x8B8B66
  uint16 v0;
  int16 v1;
  uint16 v2;
  int16 v3;

  R34 = R18_ - reg_M7X;
  R36 = reg_M7Y - R20_;
  R38 = R18_ - reg_M7X;
  R40 = reg_M7A;
  Smult16x16Shr16();
  HIBYTE(v0) = R42;
  LOBYTE(v0) = HIBYTE(R44);
  R26_ = v0;
  R38 = reg_M7B;
  R40 = R36;
  Smult16x16Shr16();
  HIBYTE(v1) = R42;
  LOBYTE(v1) = HIBYTE(R44);
  R26_ += v1;
  R18_ = R26_ + reg_M7X;
  R38 = reg_M7C;
  R40 = R34;
  Smult16x16Shr16();
  HIBYTE(v2) = R42;
  LOBYTE(v2) = HIBYTE(R44);
  R26_ = v2;
  R38 = reg_M7A;
  R40 = R36;
  Smult16x16Shr16();
  HIBYTE(v3) = R42;
  LOBYTE(v3) = HIBYTE(R44);
  R26_ += v3;
  R20_ = reg_M7Y - R26_;
}

void CopyPalettesToFadingPalettes(void) {  // 0x8B8BE9
  int16 v0;

  v0 = 256;
  uint16 v1 = 0;
  do {
    tilemap_stuff[(v1 >> 1) + 256] = palette_buffer[v1 >> 1];
    v1 += 2;
    --v0;
  } while (v0);
}

void DecomposePaletteDataForFading(void) {  // 0x8B8C09
  int16 v1;
  uint16 v3;
  char v5; // t2
  char v7; // tt

  uint16 v0 = 0;
  v1 = 256;
  do {
    int v2 = v0 >> 1;
    R18_ = tilemap_stuff[v2 + 256];
    LOBYTE(v3) = 0;
    HIBYTE(v3) = R18_ & 0x1F;
    tilemap_stuff[v2 + 512] = v3;
    tilemap_stuff[v2 + 1280] = 8 * HIBYTE(v3);
    uint16 v4 = 8 * (R18_ & 0x3E0);
    tilemap_stuff[v2 + 768] = v4;
    v5 = v4;
    LOBYTE(v4) = HIBYTE(v4);
    HIBYTE(v4) = v5;
    tilemap_stuff[v2 + 1536] = 8 * v4;
    uint16 v6 = (uint16)(R18_ & 0x7C00) >> 2;
    tilemap_stuff[v2 + 1024] = v6;
    v7 = v6;
    LOBYTE(v6) = HIBYTE(v6);
    HIBYTE(v6) = v7;
    tilemap_stuff[v2 + 1792] = 8 * v6;
    v0 += 2;
    --v1;
  } while (v1);
}

void ClearYColorsFromIndexX(uint16 k, uint16 j) {  // 0x8B8C5E
  do {
    int v2 = k >> 1;
    tilemap_stuff[v2 + 256] = 0;
    tilemap_stuff[v2 + 512] = 0;
    tilemap_stuff[v2 + 768] = 0;
    tilemap_stuff[v2 + 1024] = 0;
    k += 2;
    --j;
  } while (j);
}

void FadeOutYColorsFromIndexX(uint16 k, uint16 j) {  // 0x8B8C83
  do {
    int v2 = k >> 1;
    tilemap_stuff[v2 + 512] -= tilemap_stuff[v2 + 1280];
    tilemap_stuff[v2 + 768] -= tilemap_stuff[v2 + 1536];
    tilemap_stuff[v2 + 1024] -= tilemap_stuff[v2 + 1792];
    k += 2;
    --j;
  } while (j);
}

void FadeInYColorsFromIndexX(uint16 k, uint16 j) {  // 0x8B8CB2
  do {
    int v2 = k >> 1;
    tilemap_stuff[v2 + 512] = (tilemap_stuff[v2 + 1280] + tilemap_stuff[v2 + 512]) & 0x1FFF;
    tilemap_stuff[v2 + 768] = (tilemap_stuff[v2 + 1536] + tilemap_stuff[v2 + 768]) & 0x1FFF;
    tilemap_stuff[v2 + 1024] = (tilemap_stuff[v2 + 1792] + tilemap_stuff[v2 + 1024]) & 0x1FFF;
    k += 2;
    --j;
  } while (j);
}

void ComposeFadingPalettes(void) {  // 0x8B8CEA
  int16 v1;

  uint16 v0 = 0;
  v1 = 256;
  do {
    int v2 = v0 >> 1;
    R18_ = HIBYTE(tilemap_stuff[v2 + 512]) & 0x1F;
    R18_ |= (tilemap_stuff[v2 + 768] >> 3) & 0x3E0;
    uint16 v3 = R18_ | (4 * tilemap_stuff[v2 + 1024]) & 0x7C00;
    tilemap_stuff[v2] = v3;
    palette_buffer[v2] = v3;
    v0 += 2;
    --v1;
  } while (v1);
}

void CinematicFunction_Intro_Func20(uint16 j) {  // 0x8B8D23
  uint16 *tt = (uint16 *)RomPtr_8B(j);
  uint16 v2 = tt[0];
  R18_ = tt[1];
  uint16 *dst = ram4000.intro_japanese_text_tiles;
  do {
    R22_ = tt[2];
    memcpy(dst + (v2 >> 1), (uint16 *)RomPtr_7F(R22_ + 0xa000), 16);
    R22_ = tt[3];
    memcpy(dst + ((v2 + 768) >> 1), (uint16 *)RomPtr_7F(R22_ + 0xa000), 16);
    v2 += 16;
    tt += 2;
  } while (--R18_);
}

void TransferJapaneseTextTilesToVram(void) {  // 0x8B8DE6
  uint16 v0 = vram_write_queue_tail;
  gVramWriteEntry(vram_write_queue_tail)->size = 1536;
  v0 += 2;
  gVramWriteEntry(v0)->size = 0x4000;
  v0 += 2;
  LOBYTE(gVramWriteEntry(v0++)->size) = 126;
  gVramWriteEntry(v0)->size = 16768;
  vram_write_queue_tail = v0 + 2;
}

void HandleSamusDuringIntro(void) {  // 0x8B8E0D
  if (cinematic_var15) {
    HandleControllerInputForGamePhysics();
    HandleSamusMovementAndPause();
    if (samus_invincibility_timer)
      --samus_invincibility_timer;
    if (samus_knockback_timer)
      --samus_knockback_timer;
  }
}

void DrawIntroSprites(void) {  // 0x8B8E2D
  if (cinematic_var15) {
    if ((cinematic_var15 & 0x8000u) != 0) {
      DrawSamusAndProjectiles();
      DrawPlayerExplosions2();
      DrawCinematicSpriteObjects_Intro();
    } else {
      DrawCinematicSpriteObjects_Intro();
      DrawSamusAndProjectiles();
      DrawPlayerExplosions2();
    }
  } else {
    DrawCinematicSpriteObjects_Intro();
  }
}

void CalcSinCos(uint16 a, uint16 j) {  // 0x8B8E52
  int16 v3;
  uint16 v2;
  uint16 v4;

  R24_ = a;
  R26_ = j;
  if (sign16(j - 128))
    v2 = CalcSinMult(2 * j);
  else
    v2 = -CalcSinMult(2 * (uint8)(j + 0x80));
  R20_ = v2;
  v3 = (uint8)(R26_ - 64);
  if (sign16(v3 - 128))
    v4 = CalcSinMult(2 * v3);
  else
    v4 = -CalcSinMult(2 * (uint8)(v3 + 0x80));
  R22_ = v4;
}

uint16 CalcSinMult(uint16 k) {  // 0x8B8EA3
  R18_ = Mult8x8(*((uint8 *)&kSinCosTable8bit_Sext[64] + k), R24_) >> 8;
  return R18_ + *((uint8 *)&kSinCosTable8bit_Sext[64] + k + 1) * (uint8)R24_;
}

static const uint16 kMoveUnusedSpritesMasks[8] = { 0x5555, 0x5554, 0x5550, 0x5540, 0x5500, 0x5400, 0x5000, 0x4000 };

void MoveUnusedSpritesOffScreen(void) {
  if (oam_next_ptr >= 0x200)
    return;
  int j = oam_next_ptr >> 2;
  int i = (oam_next_ptr >> 5);
  oam_ext[i++] |= kMoveUnusedSpritesMasks[j & 7];
  for (; i < 16; i++)
    oam_ext[i] = 0x5555;
  for (int i = oam_next_ptr >> 2; i < 0x80; i++)
    *(uint16 *)&oam_ent[i].xcoord = 0x80;
}

uint8 AdvanceFastScreenFadeOut(void) {  // 0x8B90B8
  if ((reg_INIDISP & 0xF) != 0) {
    if ((int8)((reg_INIDISP & 0xF) - screen_fade_counter) > 0) {
      reg_INIDISP = (reg_INIDISP & 0xF) - screen_fade_counter;
      return 0;
    }
    reg_INIDISP = 0x80;
  }
  return 1;
}

uint8 AdvanceSlowScreenFadeOut(void) {  // 0x8B90D5
  bool v0 = (int8)(screen_fade_delay - 1) < 0;
  LOBYTE(screen_fade_delay) = screen_fade_delay - 1;
  if ((uint8)screen_fade_delay && !v0)
    return 0;
  if ((reg_INIDISP & 0xF) != 0) {
    if ((reg_INIDISP & 0xF) != 1) {
      reg_INIDISP = (reg_INIDISP & 0xF) - 1;
      LOBYTE(screen_fade_delay) = screen_fade_counter;
      return 0;
    }
    reg_INIDISP = 0x80;
    LOBYTE(screen_fade_delay) = 0;
    LOBYTE(screen_fade_counter) = 0;
  }
  return 1;
}

uint8 AdvanceFastScreenFadeIn(void) {  // 0x8B9100
  if (sign8(((screen_fade_counter + reg_INIDISP) & 0x1F) - 15)) {
    reg_INIDISP = (screen_fade_counter + reg_INIDISP) & 0x1F;
    return 0;
  } else {
    reg_INIDISP = 15;
    return 1;
  }
}

uint8 AdvanceSlowScreenFadeIn(void) {  // 0x8B911B
  bool v0 = (int8)(screen_fade_delay - 1) < 0;
  LOBYTE(screen_fade_delay) = screen_fade_delay - 1;
  if ((uint8)screen_fade_delay && !v0)
    return 0;
  if (sign8(((reg_INIDISP + 1) & 0x1F) - 15)) {
    reg_INIDISP = (reg_INIDISP + 1) & 0x1F;
    LOBYTE(screen_fade_delay) = screen_fade_counter;
    return 0;
  }
  reg_INIDISP = 15;
  LOBYTE(screen_fade_delay) = 0;
  LOBYTE(screen_fade_counter) = 0;
  return 1;
}

CoroutineRet InitializeIoDisplayLogo_Async(void) {  // 0x8B9146
  COROUTINE_BEGIN(coroutine_state_1, 0);
  WriteReg(INIDISP, 0x8Fu);
  reg_INIDISP = 0x8f;
  oam_next_ptr = 0;
  ClearOamExt();
  ClearUnusedOam();
  nmi_copy_samus_halves = 0;
  nmi_copy_samus_top_half_src = 0;
  nmi_copy_samus_bottom_half_src = 0;
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
  WriteReg(OBSEL, 3u);
  reg_OBSEL = 3;
  WriteReg(OAMADDL, 0);
  LOBYTE(reg_OAMaddr_UNUSED) = 0;
  WriteReg(OAMADDH, 0x80);
  HIBYTE(reg_OAMaddr_UNUSED) = 0x80;
  WriteReg(OAMDATA, 0);
  WriteReg(OAMDATA, 0);
  WriteReg(BGMODE, 1u);
  reg_BGMODE = 1;
  WriteReg(MOSAIC, 0);
  reg_MOSAIC = 0;
  WriteReg(BG1SC, 0);
  reg_BG1SC = 0;
  WriteReg(BG2SC, 0);
  reg_BG2SC = 0;
  WriteReg(BG3SC, 0);
  reg_BG3SC = 0;
  WriteReg(BG4SC, 0);
  reg_BG4SC = 0;
  WriteReg(BG12NBA, 0);
  reg_BG12NBA = 0;
  WriteReg(BG34NBA, 0);
  reg_BG34NBA = 0;
  WriteReg(BG1HOFS, 0);
  WriteReg(BG1HOFS, 0);
  WriteReg(BG1VOFS, 0);
  WriteReg(BG1VOFS, 0);
  WriteReg(BG2HOFS, 0);
  WriteReg(BG2HOFS, 0);
  WriteReg(BG2VOFS, 0);
  WriteReg(BG2VOFS, 0);
  WriteReg(BG3HOFS, 0);
  WriteReg(BG3HOFS, 0);
  WriteReg(BG3VOFS, 0);
  WriteReg(BG3VOFS, 0);
  WriteReg(BG4HOFS, 0);
  WriteReg(BG4HOFS, 0);
  WriteReg(BG4VOFS, 0);
  WriteReg(BG4VOFS, 0);
  WriteReg(VMAIN, 0);
  WriteReg(M7SEL, 0);
  reg_M7SEL = 0;
  WriteReg(M7A, 0);
  WriteReg(M7B, 0);
  WriteReg(M7C, 0);
  WriteReg(M7D, 0);
  WriteReg(M7X, 0);
  WriteReg(M7Y, 0);
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
  WriteReg(TM, 0x10u);
  reg_TM = 16;
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
  WriteReg(COLDATA, 0xE0u);
  reg_COLDATA[0] = 32;
  reg_COLDATA[1] = 64;
  reg_COLDATA[2] = 0x80;
  WriteReg(SETINI, 0);
  reg_SETINI = 0;
  mov24(&decompress_src, 0x9580D8u);
  {
    static const LongPtr unk_8B92A1 = LONGPTR(0x7f5000);
    DecompressToMem_IpArg(&unk_8B92A1);
  }
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x60u);
  WriteReg(VMAIN, 0x80);
  {
    static const StartDmaCopy unk_8B92B9 = { 1, 1, 0x18, LONGPTR(0x7f5000), 0x4000 };
    SetupDmaTransfer(&unk_8B92B9);
  }
  WriteReg(MDMAEN, 2u);
  reg_INIDISP = 0x80;
  {
    uint16 v0 = 0;
    do {
      palette_buffer[v0 >> 1] = g_word_8CE1E9[v0 >> 1];
      v0 += 2;
    } while ((int16)(v0 - 512) < 0);
  }
  EnableNMI();
  screen_fade_delay = 1;
  screen_fade_counter = 1;
  while (1) {
    ClearOamExt();
    oam_next_ptr = 0;
    AddNintentoLogoToOam();
    if (AdvanceFastScreenFadeIn() & 1)
      break;
    ClearUnusedOam();
    COROUTINE_AWAIT(1, WaitForNMI_Async());
  }
  ClearUnusedOam();
  COROUTINE_AWAIT(2, WaitForNMI_Async());
  game_options_screen_index = 120;
  while (1) {
    ClearOamExt();
    oam_next_ptr = 0;
    AddNintentoLogoToOam();
    if (!--game_options_screen_index)
      break;
    ClearUnusedOam();
    COROUTINE_AWAIT(3, WaitForNMI_Async());
  }
  ClearUnusedOam();
  COROUTINE_AWAIT(4, WaitForNMI_Async());
  screen_fade_delay = 1;
  screen_fade_counter = 1;
  while (1) {
    ClearOamExt();
    oam_next_ptr = 0;
    AddNintentoLogoToOam();
    if (AdvanceFastScreenFadeOut() & 1)
      break;
    ClearUnusedOam();
    COROUTINE_AWAIT(5, WaitForNMI_Async());
  }
  ClearUnusedOam();
  COROUTINE_AWAIT(6, WaitForNMI_Async());
  WriteReg(INIDISP, 0x8Fu);
  reg_INIDISP = 0x8f;

  COROUTINE_END(0);
}

void AddNintentoLogoToOam(void) {  // 0x8B936B
  R22_ = 0;
  R20_ = 128;
  R18_ = 112;
  DrawSpritemap(0x8Cu, addr_kSpriteMap_NintendoLogo);
}

uint8 SpawnCinematicSpriteObject(uint16 j, uint16 a) {  // 0x8B938A
  cinematic_spawn_param = a;
  for (int i = 30; i >= 0; i -= 2) {
    if (!cinematicspr_instr_ptr[i >> 1])
      return SpawnCimenaticSpriteObjectInner(j, i);
  }
  return 1;
}

uint8 SpawnCinematicSpriteObjectToR18(uint16 j, uint16 a) {  // 0x8B93A2
  cinematic_spawn_param = a;
  return SpawnCimenaticSpriteObjectInner(j, R18_);
}

void CallCinematicSpriteObjectSetup(uint32 ea, uint16 j) {
  switch (ea) {
  case fnCinematicFunction_nullsub_116: return;
  case fnCinematicSpriteInit_7: CinematicSpriteInit_7(j); return;
  case fnCinematicSpriteInit_8: CinematicSpriteInit_8(j); return;
  case fnCinematicSpriteInit_9: CinematicSpriteInit_9(j); return;
  case fnCinematicSpriteInit_0: CinematicSpriteInit_0(j); return;
  case fnCinematicSpriteInit_1: CinematicSpriteInit_1(j); return;
  case fnCinematicSpriteInit_2: CinematicSpriteInit_2(j); return;
  case fnCinematicSpriteInit_3: CinematicSpriteInit_3(j); return;
  case fnCinematicSpriteInit_4: CinematicSpriteInit_4(j); return;
  case fnCinematicSpriteInit_5: CinematicSpriteInit_5(j); return;
  case fnCinematicSpriteInit_6: CinematicSpriteInit_6(j); return;
  case fnSetSomeStuffForSpriteObject_2: SetSomeStuffForSpriteObject_2(j); return;
  case fnSetSomeStuffForSpriteObject_3: SetSomeStuffForSpriteObject_3(j); return;
  case fnSetSomeStuffForSpriteObject_6: SetSomeStuffForSpriteObject_6(j); return;
  case fnSetSomeStuffForSpriteObject_8: SetSomeStuffForSpriteObject_8(j); return;
  case fnSetSomeStuffForSpriteObject_10: SetSomeStuffForSpriteObject_10(j); return;
  case fnSetSomeStuffForSpriteObject_12: SetSomeStuffForSpriteObject_12(j); return;
  case fnSetSomeStuffForSpriteObject_17: SetSomeStuffForSpriteObject_17(j); return;
  case fnCinematicFunction_Intro_Func35: CinematicFunction_Intro_Func35(j); return;
  case fnCinematicFunction_Intro_Func42: CinematicFunction_Intro_Func42(j); return;
  case fnCinematicFunction_Intro_Func46: CinematicFunction_Intro_Func46(j); return;
  case fnCinematicFunction_Intro_Func47: CinematicFunction_Intro_Func47(j); return;
  case fnCinematicFunction_Intro_Func49: CinematicFunction_Intro_Func49(j); return;
  case fnCinematicFunction_Intro_Func57: CinematicFunction_Intro_Func57(j); return;
  case fnCinematicFunction_Intro_Func59: CinematicFunction_Intro_Func59(j); return;
  case fnCinematicFunction_Intro_Func61: CinematicFunction_Intro_Func61(j); return;
  case fnCinematicFunction_Intro_Func63: CinematicFunction_Intro_Func63(j); return;
  case fnCinematicFunction_Intro_Func65: CinematicFunction_Intro_Func65(j); return;
  case fnCinematicFunction_Intro_Func68: CinematicFunction_Intro_Func68(j); return;
  case fnCinematicFunction_Intro_Func71: CinematicFunction_Intro_Func71(j); return;
  case fnCinematicFunction_Intro_Func78: CinematicFunction_Intro_Func78(j); return;
  case fnCinematicFunction_Intro_Func80: CinematicFunction_Intro_Func80(j); return;
  case fnCinematicFunction_Intro_Func81: CinematicFunction_Intro_Func81(j); return;
  case fnCinematicFunction_Intro_Func83: CinematicFunction_Intro_Func83(j); return;
  case fnCinematicFunction_Intro_Func89: CinematicFunction_Intro_Func89(j); return;
  case fnCinematicFunction_Intro_Func92: CinematicFunction_Intro_Func92(j); return;
  case fnCinematicFunction_Intro_Func98: CinematicFunction_Intro_Func98(j); return;
  case fnCinematicFunction_Intro_Func99: CinematicFunction_Intro_Func99(j); return;
  case fnCinematicFunction_Intro_Func100: CinematicFunction_Intro_Func100(j); return;
  case fnCinematicFunction_Intro_Func101: CinematicFunction_Intro_Func101(j); return;
  case fnCinematicFunction_Intro_Func102: CinematicFunction_Intro_Func102(j); return;
  case fnCinematicFunction_Intro_Func104: CinematicFunction_Intro_Func104(j); return;
  case fnCinematicFunction_Intro_Func150: CinematicFunction_Intro_Func150(j); return;
  case fnCinematicFunction_Intro_Func152: CinematicFunction_Intro_Func152(j); return;
  case fnCinematicFunction_Intro_Func153: CinematicFunction_Intro_Func153(j); return;
  case fnCinematicFunction_Intro_Func154: CinematicFunction_Intro_Func154(j); return;
  case fnCinematicFunction_Intro_Func155: CinematicFunction_Intro_Func155(j); return;
  case fnCinematicFunction_Intro_Func156: CinematicFunction_Intro_Func156(j); return;
  case fnCinematicFunction_Intro_Func157: CinematicFunction_Intro_Func157(j); return;
  case fnCinematicFunction_Intro_Func159: CinematicFunction_Intro_Func159(j); return;
  case fnCinematicFunction_Intro_Func161: CinematicFunction_Intro_Func161(j); return;
  case fnCinematicFunction_Intro_Func162: CinematicFunction_Intro_Func162(j); return;
  case fnCinematicFunction_Intro_Func163: CinematicFunction_Intro_Func163(j); return;
  case fnCinematicFunction_Intro_Func164: CinematicFunction_Intro_Func164(j); return;
  case fnCinematicFunction_Intro_Func165: CinematicFunction_Intro_Func165(j); return;
  case fnCinematicFunction_Intro_Func166: CinematicFunction_Intro_Func166(j); return;
  case fnCinematicFunction_Intro_Func167: CinematicFunction_Intro_Func167(j); return;
  case fnCinematicFunction_Intro_Func168: CinematicFunction_Intro_Func168(j); return;
  case fnCinematicFunction_Intro_Func169: CinematicFunction_Intro_Func169(j); return;
  case fnCinematicFunction_Intro_Func170: CinematicFunction_Intro_Func170(j); return;
  case fnCinematicFunction_Intro_Func171: CinematicFunction_Intro_Func171(j); return;
  case fnCinematicFunction_Intro_Func172: CinematicFunction_Intro_Func172(j); return;
  case fnCinematicFunction_Intro_Func173: CinematicFunction_Intro_Func173(j); return;
  case fnCinematicFunction_Intro_Func174: CinematicFunction_Intro_Func174(j); return;
  case fnCinematicFunction_Intro_Func175: CinematicFunction_Intro_Func175(j); return;
  case fnCinematicFunction_Intro_Func176: CinematicFunction_Intro_Func176(j); return;
  case fnCinematicFunction_Intro_Func177: CinematicFunction_Intro_Func177(j); return;
  case fnCinematicFunction_Intro_Func178: CinematicFunction_Intro_Func178(j); return;
  default: Unreachable();
  }
}

uint8 SpawnCimenaticSpriteObjectInner(uint16 k, uint16 j) {  // 0x8B93AC
  uint8 *v2 = RomPtr_8B(k);
  int v3 = j >> 1;
  cinematicspr_preinstr_func[v3] = *((uint16 *)v2 + 1);
  cinematicspr_instr_ptr[v3] = *((uint16 *)v2 + 2);
  cinematicspr_instr_timer[v3] = 1;
  cinematicspr_whattodraw[v3] = 0;
  cinematicspr_goto_timer[v3] = 0;
  cinematicspr_arr6[v3] = 0;
  cinematicspr_arr7[v3] = 0;
  CallCinematicSpriteObjectSetup(*(uint16 *)v2 | 0x8B0000, j);
  return 0;
}

void ClearCinematicSprites(void) {  // 0x8B93DA
  for (int i = 30; i >= 0; i -= 2) {
    int v1 = i >> 1;
    cinematicspr_instr_ptr[v1] = 0;
    cinematicspr_whattodraw[v1] = 0;
  }
}

void HandleCinematicSprites(void) {  // 0x8B93EF
  for (int i = 30; i >= 0; i -= 2) {
    cinematic_obj_index = i;
    if (cinematicspr_instr_ptr[i >> 1]) {
      ProcessCinematicSpriteInstructionList(i);
      i = cinematic_obj_index;
    }
  }
}

uint16 CallCinematicSprInstr(uint32 ea, uint16 k, uint16 j) {
  switch (ea) {
  case fnCinematicSprInstr_Delete: return CinematicSprInstr_Delete(k, j);
  case fnCinematicSprInstr_Sleep: return CinematicSprInstr_Sleep(k, j);
  case fnCinematicSprInstr_SetPreInstr: return CinematicSprInstr_SetPreInstr(k, j);
  case fnCinematicSprInstr_ClearPreInstr: return CinematicSprInstr_ClearPreInstr(k, j);
  case fnCinematicSprInstr_GotoRel: return CinematicSprInstr_GotoRel(k, j);
  case fnCinematicSprInstr_Goto: return CinematicSprInstr_Goto(k, j);
  case fnCinematicSprInstr_DecTimerGoto: return CinematicSprInstr_DecTimerGoto(k, j);
  case fnCinematicSprInstr_DecTimerGotoRel: return CinematicSprInstr_DecTimerGotoRel(k, j);
  case fnCinematicSprInstr_SetTimer: return CinematicSprInstr_SetTimer(k, j);
  case fnCinematicSprInstr_9CE1: return CinematicSprInstr_9CE1(k, j);
  case fnCinematicSprInstr_9D5D: return CinematicSprInstr_9D5D(k, j);
  case fnCinematicSprInstr_9DD6: return CinematicSprInstr_9DD6(k, j);
  case fnCinematicSprInstr_Func8: return CinematicSprInstr_Func8(k, j);
  case fnCinematicSprInstr_Func9: return CinematicSprInstr_Func9(k, j);
  case fnCinematicSprInstr_sub_8B9EF0: return CinematicSprInstr_sub_8B9EF0(k, j);
  case fnCinematicSprInstr_9F19: return CinematicSprInstr_9F19(k, j);
  case fnCinematicSprInstr_SpawnMetroidEggParticles: return CinematicSprInstr_SpawnMetroidEggParticles(k, j);
  case fnCinematicSprInstr_StartIntroPage2: return CinematicSprInstr_StartIntroPage2(k, j);
  case fnCinematicSprInstr_StartIntroPage3: return CinematicSprInstr_StartIntroPage3(k, j);
  case fnCinematicSprInstr_StartIntroPage4: return CinematicSprInstr_StartIntroPage4(k, j);
  case fnCinematicSprInstr_StartIntroPage5: return CinematicSprInstr_StartIntroPage5(k, j);
  case fnCinematicSprInstr_Func43: return CinematicSprInstr_Func43(k, j);
  case fnCinematicSprInstr_SpawnIntroRinkas01: return CinematicSprInstr_SpawnIntroRinkas01(k, j);
  case fnCinematicSprInstr_SpawnIntroRinkas23: return CinematicSprInstr_SpawnIntroRinkas23(k, j);
  case fnCinematicCommonInstr_Func69: return CinematicCommonInstr_Func69(k, j);
  case fnCinematicCommonInstr_Func70: return CinematicCommonInstr_Func70(k, j);
  case fnCinematicSprInstr_SpawnCeresExplosions1: return CinematicSprInstr_SpawnCeresExplosions1(k, j);
  case fnCinematicSprInstr_SpawnCeresExplosions3: return CinematicSprInstr_SpawnCeresExplosions3(k, j);
  case fnCinematicSprInstr_C9A5: return CinematicSprInstr_C9A5(k, j);
  case fnCinematicSprInstr_C9AF: return CinematicSprInstr_C9AF(k, j);
  case fnCinematicSprInstr_C9BD: return CinematicSprInstr_C9BD(k, j);
  case fnCinematicSprInstr_C9C7: return CinematicSprInstr_C9C7(k, j);
  case fnCinematicSprInstr_Func181: return CinematicSprInstr_Func181(k, j);
  case fnCinematicSprInstr_Func182: return CinematicSprInstr_Func182(k, j);
  case fnCinematicSprInstr_Func183: return CinematicSprInstr_Func183(k, j);
  case fnCinematicSprInstr_Func185: return CinematicSprInstr_Func185(k, j);
  case fnCinematicSprInstr_Func186: return CinematicSprInstr_Func186(k, j);
  case fnCinematicSprInstr_Func187: return CinematicSprInstr_Func187(k, j);
  case fnCinematicSprInstr_Func190: return CinematicSprInstr_Func190(k, j);
  case fnCinematicSprInstr_Func192: return CinematicSprInstr_Func192(k, j);
  case fnCinematicSprInstr_Func193: return CinematicSprInstr_Func193(k, j);
  case fnCinematicSprInstr_Func194: return CinematicSprInstr_Func194(k, j);
  case fnCinematicSprInstr_Func195: return CinematicSprInstr_Func195(k, j);
  case fnCinematicSprInstr_Func196: return CinematicSprInstr_Func196(k, j);
  case fnCinematicSprInstr_Func197: return CinematicSprInstr_Func197(k, j);
  case fnCinematicSprInstr_Func198: return CinematicSprInstr_Func198(k, j);
  case fnCinematicSprInstr_Func208: return CinematicSprInstr_Func208(k, j);
  case fnCinematicSprInstr_Func211: return CinematicSprInstr_Func211(k, j);
  case fnCinematicSprInstr_Func213: return CinematicSprInstr_Func213(k, j);
  case fnCinematicSprInstr_Func214: return CinematicSprInstr_Func214(k, j);
  case fnCinematicSprInstr_Func217: return CinematicSprInstr_Func217(k, j);
  case fnCinematicSprInstr_Func218: return CinematicSprInstr_Func218(k, j);
  default: return Unreachable();
  }
}
void CallCinematicSprPreInstr(uint32 ea, uint16 j) {
  switch (ea) {
  case fnCinematicSprPreInstr_nullsub_300: return;
  case fnCinematicFunction_nullsub_116: return;
  case fnCinematicFunction_nullsub_298: return;
  case fnsub_8B9CCF: sub_8B9CCF(j); return;
  case fnnullsub_122:  return;
  case fnSetSomeStuffForSpriteObject_4_MetroidEgg: SetSomeStuffForSpriteObject_4_MetroidEgg(j); return;
  case fnCinematicSprPreInstr_A903: CinematicSprPreInstr_A903(j); return;
  case fnSetSomeStuffForSpriteObject_7: SetSomeStuffForSpriteObject_7(j); return;
  case fnSetSomeStuffForSpriteObject_9: SetSomeStuffForSpriteObject_9(j); return;
  case fnSetSomeStuffForSpriteObject_11: SetSomeStuffForSpriteObject_11(j); return;
  case fnSetSomeStuffForSpriteObject_13: SetSomeStuffForSpriteObject_13(j); return;
  case fnSetSomeStuffForSpriteObject_18: SetSomeStuffForSpriteObject_18(j); return;
  case fnCinematicFunction_Intro_Func36: CinematicFunction_Intro_Func36(j); return;
  case fnCinematicFunction_Intro_Func39: CinematicFunction_Intro_Func39(j); return;
  case fnCinematicSprPreInstr_B82E: CinematicSprPreInstr_B82E(j); return;
  case fnCinematicFunction_Intro_Func44: CinematicFunction_Intro_Func44(j); return;
  case fnCinematicFunction_Intro_Func45: CinematicFunction_Intro_Func45(j); return;
  case fnCinematicFunction_Intro_Func48: CinematicFunction_Intro_Func48(j); return;
  case fnCinematicFunction_Intro_Func50: CinematicFunction_Intro_Func50(j); return;
  case fnCinematicFunction_Intro_Func51: CinematicFunction_Intro_Func51(j); return;
  case fnCinematicFunction_Intro_Func52: CinematicFunction_Intro_Func52(j); return;
  case fnCinematicFunction_Intro_Func53: CinematicFunction_Intro_Func53(j); return;
  case fnCinematicFunction_Intro_Func58: CinematicFunction_Intro_Func58(j); return;
  case fnCinematicFunction_Intro_Func60: CinematicFunction_Intro_Func60(j); return;
  case fnCinematicFunction_Intro_Func62: CinematicFunction_Intro_Func62(j); return;
  case fnCinematicFunction_Intro_Func64: CinematicFunction_Intro_Func64(j); return;
  case fnCinematicFunction_Intro_Func66: CinematicFunction_Intro_Func66(j); return;
  case fnCinematicSprPreInstr_C489: CinematicSprPreInstr_C489(j); return;
  case fnCinematicFunction_Intro_Func82: CinematicFunction_Intro_Func82(j); return;
  case fnCinematicFunction_Intro_Func90: CinematicFunction_Intro_Func90(j); return;
  case fnCinematicFunction_Intro_Func91: CinematicFunction_Intro_Func91(j); return;
  case fnCinematicFunction_Intro_Func93: CinematicFunction_Intro_Func93(j); return;
  case fnCinematicFunction_Intro_Func94: CinematicFunction_Intro_Func94(j); return;
  case fnCinematicFunction_Intro_Func96: CinematicFunction_Intro_Func96(j); return;
  case fnCinematicFunction_Intro_Func97: CinematicFunction_Intro_Func97(j); return;
  case fnCinematicFunction_Intro_Func151: CinematicFunction_Intro_Func151(j); return;
  case fnCinematicFunction_Intro_Func179: CinematicFunction_Intro_Func179(j); return;
  case fnCinematicFunction_Intro_Func180: CinematicFunction_Intro_Func180(j); return;
  case fnCinematicFunction_Intro_Func184: CinematicFunction_Intro_Func184(j); return;
  case fnCinematicSprPreInstr_F35A: CinematicSprPreInstr_F35A(j); return;
  case fnCinematicFunction_Intro_Func189: CinematicFunction_Intro_Func189(j); return;
  case fnCinematicFunction_Intro_Func189b: CinematicFunction_Intro_Func189b(j); return;
  case fnCinematicFunction_Intro_Func191: CinematicFunction_Intro_Func191(j); return;
  case fnCinematicFunction_Intro_Func199: CinematicFunction_Intro_Func199(j); return;
  case fnCinematicFunction_Intro_Func200: CinematicFunction_Intro_Func200(j); return;
  case fnCinematicFunction_Intro_Func201: CinematicFunction_Intro_Func201(j); return;
  case fnCinematicFunction_Intro_Func202: CinematicFunction_Intro_Func202(j); return;
  case fnCinematicFunction_Intro_Func203: CinematicFunction_Intro_Func203(j); return;
  case fnCinematicFunction_Intro_Func204: CinematicFunction_Intro_Func204(j); return;
  case fnCinematicFunction_Intro_Func205: CinematicFunction_Intro_Func205(j); return;
  case fnCinematicFunction_Intro_Func206: CinematicFunction_Intro_Func206(j); return;
  case fnCinematicFunction_Intro_Func207: CinematicFunction_Intro_Func207(j); return;
  case fnnullsub_128: return;
  case fnCinematicSprPreInstr_F528: CinematicSprPreInstr_F528(j); return;
  case fnCinematicSprPreInstr_F57F: CinematicSprPreInstr_F57F(j); return;
  case fnCinematicFunction_Intro_Func215: CinematicFunction_Intro_Func215(j); return;
  default: Unreachable();
  }
}
void ProcessCinematicSpriteInstructionList(uint16 k) {  // 0x8B9409
  CallCinematicSprPreInstr(cinematicspr_preinstr_func[k >> 1] | 0x8B0000, k);
  uint16 v1 = cinematic_obj_index;
  int v2 = cinematic_obj_index >> 1;
  if (cinematicspr_instr_timer[v2]-- == 1) {
    uint16 v4 = cinematicspr_instr_ptr[v2], v6;
    while (1) {
      uint16 *v5 = (uint16 *)RomPtr_8B(v4);
      v6 = *v5;
      if ((*v5 & 0x8000u) == 0)
        break;
      R18_ = *v5;
      v4 = CallCinematicSprInstr(v6 | 0x8B0000, v1, v4 + 2);
      if (!v4)
        return;
    }
    int v7 = v1 >> 1;
    cinematicspr_instr_timer[v7] = v6;
    cinematicspr_whattodraw[v7] = *((uint16 *)RomPtr_8B(v4) + 1);
    cinematicspr_instr_ptr[v7] = v4 + 4;
  }
}

uint16 CinematicSprInstr_Delete(uint16 k, uint16 j) {  // 0x8B9438
  int v2 = k >> 1;
  cinematicspr_whattodraw[v2] = 0;
  cinematicspr_instr_ptr[v2] = 0;
  return 0;
}

uint16 CinematicSprInstr_Sleep(uint16 k, uint16 j) {  // 0x8B9442
  cinematicspr_instr_ptr[k >> 1] = j - 2;
  return 0;
}

uint16 CinematicSprInstr_SetPreInstr(uint16 k, uint16 j) {  // 0x8B944C
  cinematicspr_preinstr_func[k >> 1] = *(uint16 *)RomPtr_8B(j);
  return j + 2;
}

uint16 CinematicSprInstr_ClearPreInstr(uint16 k, uint16 j) {  // 0x8B9457
  cinematicspr_preinstr_func[k >> 1] = FUNC16(CinematicFunction_nullsub_298);
  return j;
}


uint16 CinematicSprInstr_GotoRel(uint16 k, uint16 j) {  // 0x8B94A2
  int16 v2;

  R18_ = j;
  LOBYTE(v2) = HIBYTE(*(uint16 *)RomPtr_8B(j - 1));
  if ((v2 & 0x80) != 0)
    v2 |= 0xFF00u;
  else
    v2 = (uint8)v2;
  return R18_ + v2;
}

uint16 CinematicSprInstr_Goto(uint16 k, uint16 j) {  // 0x8B94BC
  return *(uint16 *)RomPtr_8B(j);
}

uint16 CinematicSprInstr_DecTimerGoto(uint16 k, uint16 j) {  // 0x8B94C3
  int v2 = k >> 1;
  if (cinematicspr_goto_timer[v2]-- == 1)
    return j + 2;
  else
    return CinematicSprInstr_Goto(k, j);
}

uint16 CinematicSprInstr_DecTimerGotoRel(uint16 k, uint16 j) {  // 0x8B94CD
  int v2 = k >> 1;
  if (cinematicspr_goto_timer[v2]-- == 1)
    return j + 1;
  else
    return CinematicSprInstr_GotoRel(k, j);
}

uint16 CinematicSprInstr_SetTimer(uint16 k, uint16 j) {  // 0x8B94D6
  cinematicspr_goto_timer[k >> 1] = *(uint16 *)RomPtr_8B(j);
  return j + 2;
}

void sub_8B94E1(void) {  // 0x8B94E1
  ;
}

void CallCinematicSpriteInit(uint32 ea, uint16 j) {
  switch (ea) {
  case fnCinematicSpriteInit_7: CinematicSpriteInit_7(j); return;
  case fnCinematicSpriteInit_8: CinematicSpriteInit_8(j); return;
  case fnCinematicSpriteInit_9: CinematicSpriteInit_9(j); return;
  case fnCinematicSpriteInit_0: CinematicSpriteInit_0(j); return;
  case fnCinematicSpriteInit_1: CinematicSpriteInit_1(j); return;
  case fnCinematicSpriteInit_2: CinematicSpriteInit_2(j); return;
  case fnCinematicSpriteInit_3: CinematicSpriteInit_3(j); return;
  case fnCinematicSpriteInit_4: CinematicSpriteInit_4(j); return;
  case fnCinematicSpriteInit_5: CinematicSpriteInit_5(j); return;
  case fnCinematicSpriteInit_6: CinematicSpriteInit_6(j); return;
  case fnCinematicFunction_nullsub_116: return;
  default: Unreachable();
  }
}

uint8 SpawnMode7Object(uint16 j, uint16 a) {  // 0x8B94E4
  Mode7ObjectDef *Mode7ObjectDef;

  mode7_spawn_param = a;
  uint16 v3 = 2;
  while (mode7_obj_instr_ptr[v3 >> 1]) {
    v3 -= 2;
    if ((v3 & 0x8000u) != 0)
      return 1;
  }
  Mode7ObjectDef = get_Mode7ObjectDef(j);
  int v6 = v3 >> 1;
  mode7_obj_preinstr_func[v6] = Mode7ObjectDef->field_2;
  mode7_obj_instr_ptr[v6] = Mode7ObjectDef->field_4;
  mode7_obj_instr_timer[v6] = 1;
  mode7_obj_goto_timer[v6] = 0;
  CallCinematicSpriteInit(Mode7ObjectDef->field_0 | 0x8B0000, v6);
  return 0;
}

void HandleMode7Objects(void) {  // 0x8B951D
  for (int i = 2; i >= 0; i -= 2) {
    mode7_cur_index = i;
    if (mode7_obj_instr_ptr[i >> 1]) {
      ProcessMode7ObjectInstructions(i);
      i = mode7_cur_index;
    }
  }
}
uint16 CallMode7Instr(uint32 ea, uint16 k, uint16 j) {
  switch (ea) {
  case fnMode7Instr_Delete: return Mode7Instr_Delete(k, j);
  case fnMode7Instr_SetPreInstr: return Mode7Instr_SetPreInstr(k, j);
  case fnMode7Instr_ClearPreInstr: return Mode7Instr_ClearPreInstr(k, j);
  case fnMode7Instr_Goto: return Mode7Instr_Goto(k, j);
  case fnMode7Instr_DecTimerAndGoto: return Mode7Instr_DecTimerAndGoto(k, j);
  case fnMode7Instr_SetTimer: return Mode7Instr_SetTimer(k, j);
  case fnPlayBabyMetroidCry1: return PlayBabyMetroidCry1(k, j);
  case fnPlayBabyMetroidCry2: return PlayBabyMetroidCry2(k, j);
  case fnPlayBabyMetroidCry3: return PlayBabyMetroidCry3(k, j);
  case fnCinematicSetPal1: return CinematicSetPal1(k, j);
  case fnCinematicSetPal2: return CinematicSetPal2(k, j);
  case fnCinematicSetPal3: return CinematicSetPal3(k, j);
  case fnCinematicSetPal4: return CinematicSetPal4(k, j);
  case fnsub_8BB51E: return sub_8BB51E(k, j);
  case fnEnableCinematicBgTilemapUpdates__0: return EnableCinematicBgTilemapUpdates__0(k, j);
  case fnCinematicFunction_Intro_Func21: return CinematicFunction_Intro_Func21(k, j);
  case fnCinematicFunction_Intro_ThenWaitInputSetupBabyMetroid: return CinematicFunction_Intro_ThenWaitInputSetupBabyMetroid(k, j);
  case fnCinematicFunction_Intro_Func23: return CinematicFunction_Intro_Func23(k, j);
  case fnCinematicFunction_Intro_Func25: return CinematicFunction_Intro_Func25(k, j);
  case fnCinematicFunction_Intro_Func26: return CinematicFunction_Intro_Func26(k, j);
  case fnCinematicFunction_Intro_Func28: return CinematicFunction_Intro_Func28(k, j);
  case fnCinematicFunction_Intro_Func29: return CinematicFunction_Intro_Func29(k, j);
  case fnCinematicFunction_Intro_Func31: return CinematicFunction_Intro_Func31(k, j);
  case fnCinematicFunction_Intro_Func32: return CinematicFunction_Intro_Func32(k, j);
  case fnEnableCinematicBgTilemapUpdates__: return EnableCinematicBgTilemapUpdates__(k, j);
  default: return Unreachable();
  }
}


void CallMode7PreInstr(uint32 ea, uint16 k) {
  switch (ea) {
  case fnCinematicFunction_nullsub_116: return;
  case fnCinematicFunction_Intro_Func22: CinematicFunction_Intro_Func22(k); return;
  case fnCinematicFunction_Intro_Func24: CinematicFunction_Intro_Func24(k); return;
  case fnCinematicFunction_Intro_Func27: CinematicFunction_Intro_Func27(k); return;
  case fnCinematicFunction_Intro_Func30: CinematicFunction_Intro_Func30(k); return;
  default: Unreachable();
  }
}

void ProcessMode7ObjectInstructions(uint16 k) {  // 0x8B9537
  CallMode7PreInstr(mode7_obj_preinstr_func[k >> 1] | 0x8B0000, k);
  uint16 v1 = mode7_cur_index;
  int v2 = mode7_cur_index >> 1;
  if (mode7_obj_instr_timer[v2]-- == 1) {
    uint16 v4 = mode7_obj_instr_ptr[v2], v6;
    while (1) {
      uint16 *v5 = (uint16 *)RomPtr_8B(v4);
      v6 = *v5;
      if ((*v5 & 0x8000u) == 0)
        break;
      R18_ = *v5;
      v4 = CallMode7Instr(v6 | 0x8B0000, v1, v4 + 2);
      if (!v4)
        return;
    }
    mode7_obj_instr_timer[v1 >> 1] = v6;
    uint8 *v7 = RomPtr_8B(v4);
    QueueMode7Transfers(0x8b, *((uint16 *)v7 + 1));
    mode7_obj_instr_ptr[v1 >> 1] = v4 + 4;
  }
}

uint16 Mode7Instr_Delete(uint16 k, uint16 j) {  // 0x8B956B
  mode7_obj_instr_ptr[k >> 1] = 0;
  return 0;
}

uint16 Mode7Instr_SetPreInstr(uint16 k, uint16 j) {  // 0x8B9572
  mode7_obj_preinstr_func[k >> 1] = *(uint16 *)RomPtr_8B(j);
  return j + 2;
}

uint16 Mode7Instr_ClearPreInstr(uint16 k, uint16 j) {  // 0x8B957D
  mode7_obj_preinstr_func[k >> 1] = FUNC16(locret_8B9585);
  return j;
}

uint16 Mode7Instr_Goto(uint16 k, uint16 j) {  // 0x8B9586
  return *(uint16 *)RomPtr_8B(j);
}

uint16 Mode7Instr_DecTimerAndGoto(uint16 k, uint16 j) {  // 0x8B958D
  int v2 = k >> 1;
  if (mode7_obj_goto_timer[v2]-- == 1)
    return j + 2;
  else
    return Mode7Instr_Goto(k, j);
}

uint16 Mode7Instr_SetTimer(uint16 k, uint16 j) {  // 0x8B9597
  mode7_obj_goto_timer[k >> 1] = *(uint16 *)RomPtr_8B(j);
  return j + 2;
}

void EnableCinematicBgObjects(void) {  // 0x8B95A2
  cinematic_enable_objs |= 0x8000u;
}

void DisableCinematicBgObjects(void) {  // 0x8B95AD
  cinematic_enable_objs &= ~0x8000u;
}

void EnableCinematicBgTilemapUpdates(void) {  // 0x8B95B8
  cinematic_enable_bg_tilemap |= 0x8000u;
}

void DisableCinematicBgTilemapUpdates(void) {  // 0x8B95C3
  cinematic_enable_bg_tilemap &= ~0x8000u;
}

void ClearCinematicBgObjects(uint16 a) {  // 0x8B95CE
  uint16 j;

  for (int i = 2046; i >= 0; i -= 2)
    *(uint16 *)((char *)ram3000.pause_menu_map_tilemap + (uint16)i) = a;
  for (j = 6; (j & 0x8000u) == 0; j -= 2) {
    int v3 = j >> 1;
    cinematicbg_arr1[v3] = 0;
    cinematicbg_instr_ptr[v3] = 0;
  }
  cinematicbg_var1 = 0;
}


void CallCinematicBgObjectSetup(uint32 ea, uint16 j) {
  switch (ea) {
  case fnCinematicFunction_nullsub_116: return;
  default: Unreachable();
  }
}
uint8 SpawnCinematicBgObject(uint16 j, uint16 a) {  // 0x8B95F0
  cinematicbg_var3 = a;
  uint16 v3 = 6;
  while (cinematicbg_instr_ptr[v3 >> 1]) {
    v3 -= 2;
    if ((v3 & 0x8000u) != 0)
      return 1;
  }
  uint8 *v5 = RomPtr_8B(j);
  int v6 = v3 >> 1;
  cinematicbg_preinstr[v6] = *((uint16 *)v5 + 1);
  cinematicbg_instr_ptr[v6] = *((uint16 *)v5 + 2);
  cinematicbg_instr_timer[v6] = 1;
  cinematicbg_arr1[v6] = 0;
  cinematicbg_arr6[v6] = 0;
  CallCinematicBgObjectSetup(*(uint16 *)v5 | 0x8B0000, v3);
  return 0;
}

void HandleCinematicBgObjects(void) {  // 0x8B962F
  if ((cinematic_enable_objs & 0x8000u) != 0) {
    for (int i = 6; i >= 0; i -= 2) {
      cinematicbg_var2 = i;
      if (cinematicbg_instr_ptr[i >> 1]) {
        ProcessCinematicBgObjectsInstrs(i);
        i = cinematicbg_var2;
      }
    }
    if ((cinematic_enable_bg_tilemap & 0x8000u) != 0)
      UpdateCinematicBgTilemap();
    UpdateSamusEyesTilemap();
  }
}

void CallCinematicBgPreInstr(uint32 ea, uint16 j) {
  switch (ea) {
  case fnCinematicFunction_nullsub_116: return;
  case fnCinematicBgPreInstr_SamusBlink: CinematicBgPreInstr_SamusBlink(j); return;
  default: Unreachable();
  }
}

uint16 CallCinematicBgInstr(uint32 ea, uint16 k, uint16 j) {
  switch (ea) {
  case fnCinematicBgInstr_Delete: return CinematicBgInstr_Delete(k, j);
  case fnCinematicBgInstr_Goto: return CinematicBgInstr_Goto(k, j);
  case fnCinematicBgInstr_SetSomeStuffForSpriteObject_14: return CinematicBgInstr_SetSomeStuffForSpriteObject_14(k, j);
  case fnCinematicBgInstr_HandleCreateJpnText_Page1: return CinematicBgInstr_HandleCreateJpnText_Page1(k, j);
  case fnCinematicBgInstr_SpawnMarkerWaitInput_Page1: return CinematicBgInstr_SpawnMarkerWaitInput_Page1(k, j);
  case fnCinematicBgInstr_HandleCreateJpnText_Page2: return CinematicBgInstr_HandleCreateJpnText_Page2(k, j);
  case fnCinematicBgInstr_SpawnMarkerWaitInput_Page2: return CinematicBgInstr_SpawnMarkerWaitInput_Page2(k, j);
  case fnCinematicBgInstr_HandleCreateJpnText_Page3: return CinematicBgInstr_HandleCreateJpnText_Page3(k, j);
  case fnCinematicBgInstr_SpawnMarkerWaitInput_Page3: return CinematicBgInstr_SpawnMarkerWaitInput_Page3(k, j);
  case fnCinematicBgInstr_HandleCreateJpnText_Page4: return CinematicBgInstr_HandleCreateJpnText_Page4(k, j);
  case fnCinematicBgInstr_SpawnMarkerWaitInput_Page4: return CinematicBgInstr_SpawnMarkerWaitInput_Page4(k, j);
  case fnCinematicBgInstr_HandleCreateJpnText_Page5: return CinematicBgInstr_HandleCreateJpnText_Page5(k, j);
  case fnCinematicBgInstr_SpawnMarkerWaitInput_Page5: return CinematicBgInstr_SpawnMarkerWaitInput_Page5(k, j);
  case fnCinematicBgInstr_Func16: return CinematicBgInstr_Func16(k, j);
  case fnCinematicBgInstr_Func17: return CinematicBgInstr_Func17(k, j);
  case fnCinematicCommonInstr_Func69: return CinematicCommonInstr_Func69(k, j);
  case fnCinematicCommonInstr_Func70: return CinematicCommonInstr_Func70(k, j);
  case fnCalcItemPercentageCount: return CalcItemPercentageCount(k, j);
  case fnCinematicFunction_Intro_Func146: return CinematicFunction_Intro_Func146(k, j);
  case fnCinematicFunction_Intro_Func147: return CinematicFunction_Intro_Func147(k, j);

  default: return Unreachable();
  }
}

void ProcessCinematicBgObjectsInstrs(uint16 k) {  // 0x8B9659
  CallCinematicBgPreInstr(cinematicbg_preinstr[k >> 1] | 0x8B0000, k);
  uint16 v1 = cinematicbg_var2;
  int v2 = cinematicbg_var2 >> 1;
  if (cinematicbg_instr_timer[v2]-- == 1) {
    uint16 v4 = cinematicbg_instr_ptr[v2], v6;
    while (1) {
      uint16 *v5 = (uint16 *)RomPtr_8C(v4);
      v6 = *v5;
      if ((*v5 & 0x8000u) == 0)
        break;
      R18_ = *v5;
      v4 = CallCinematicBgInstr(v6 | 0x8B0000, v1, v4 + 2);
      if (!v4)
        return;
    }
    int v7 = v1 >> 1;
    cinematicbg_instr_timer[v7] = v6;
    uint8 *v8 = RomPtr_8C(v4);
    R18_ = *((uint16 *)v8 + 1);
    cinematicbg_arr1[v7] = *((uint16 *)v8 + 2);
    ProcessCinematicBgObject(v1);
    cinematicbg_instr_ptr[v7] = v4 + 6;
  }
}

uint16 CinematicBgInstr_Delete(uint16 k, uint16 j) {  // 0x8B9698
  int v2 = k >> 1;
  cinematicbg_arr1[v2] = 0;
  cinematicbg_instr_ptr[v2] = 0;
  return 0;
}

uint16 CinematicBgInstr_Goto(uint16 k, uint16 j) {  // 0x8B971E
  return *(uint16 *)RomPtr_8C(j);
}

void sub_8B9743(void) {  // 0x8B9743
  ;
}

void DrawCinematicSpriteObjects_Intro(void) {  // 0x8B9746
  for (int i = 30; i >= 0; i -= 2) {
    int v1 = i >> 1;
    if (cinematicspr_whattodraw[v1]) {
      uint16 v2 = cinematicspr_whattodraw[v1];
      R22_ = cinematicbg_arr9[v1];
      R20_ = cinematicbg_arr7[v1] - layer1_x_pos;
      uint16 v3 = cinematicbg_arr8[v1] - layer1_y_pos;
      R18_ = v3;
      if ((v3 & 0xFF00) != 0) {
        if ((uint16)(v3 + 128) < 0x1FFu)
          DrawSpritemapOffScreen(v2);
      } else if ((uint16)(v3 + 128) < 0x1FFu) {
        DrawSpritemap(0x8Cu, v2);
      }
    }
  }
}

void DrawCinematicSpriteObjects_Ending(void) {  // 0x8B9799
  for (int i = 30; i >= 0; i -= 2) {
    int v1 = i >> 1;
    if (cinematicspr_whattodraw[v1]) {
      uint16 v2 = cinematicspr_whattodraw[v1];
      R22_ = cinematicbg_arr9[v1];
      R20_ = cinematicbg_arr7[v1] - layer1_x_pos;
      uint16 v3 = cinematicbg_arr8[v1] - layer1_y_pos;
      R18_ = v3;
      if ((v3 & 0xFF00) != 0) {
        if ((uint16)(v3 + 128) < 0x1FFu)
          DrawSpritemapOffScreen(v2);
      } else if ((uint16)(v3 + 128) < 0x1FFu) {
        DrawSpritemap(0x8Cu, v2);
      }
    }
  }
  if (sign16(cinematic_function - FUNC16(CinematicFunction_Intro_Func116)))
    MoveUnusedSpritesOffScreen();
}

void SpawnTextGlowObject(uint16 j) {  // 0x8B97F7
  uint16 v1 = 14;
  while (*(uint16 *)((char *)&cinematic_var21 + v1)) {
    v1 -= 2;
    if ((v1 & 0x8000u) != 0)
      return;
  }
  *(uint16 *)((char *)&cinematic_var21 + v1) = j;
  int v2 = v1 >> 1;
  enemy_projectile_pre_instr[v2 + 2] = 1;
  enemy_projectile_pre_instr[v2 + 10] = (uint8)R18_;
  *(uint16 *)((char *)enemy_projectile_1A27 + v1) = HIBYTE(R18_);
  *(uint16 *)((char *)&cinematic_var20 + v1) = 0;
}

void HandleTextGlowObjects(void) {  // 0x8B9828
  for (int i = 14; i >= 0; i -= 2) {
    cinematic_var19 = i;
    if (*(uint16 *)((char *)&cinematic_var21 + (uint16)i)) {
      ProcessTextGlowObject();
      i = cinematic_var19;
    }
  }
}

void ProcessTextGlowObject(void) {  // 0x8B9849
  uint16 v0 = cinematic_var19;
  int v1 = cinematic_var19 >> 1;
  if (enemy_projectile_pre_instr[v1 + 2]-- == 1) {
    R28_ = *(uint16 *)((char *)&cinematic_var20 + v0);
    uint16 v3 = *(uint16 *)((char *)&cinematic_var21 + v0);
    R20_ = 2 * LOBYTE(enemy_projectile_pre_instr[v1 + 10]);
    R22_ = R20_ + Mult8x8(*((uint8 *)enemy_projectile_1A27 + v0), 0x40);
    uint16 v4 = R22_;
    uint8 *v5 = RomPtr_8C(v3);
    R18_ = v5[2];
    R24_ = R18_;
    R20_ = v5[3];
    while (1) {
      do {
        *(uint16 *)((char *)ram3000.pause_menu_map_tilemap + v4) = R28_ | *(uint16 *)((uint8 *)ram3000.pause_menu_map_tilemap
                                                                                      + v4) & 0xE3FF;
        v3 += 2;
        v4 += 2;
        --R18_;
      } while (R18_);
      R18_ = R24_;
      if (!--R20_)
        break;
      R22_ += 64;
      v4 = R22_;
    }
    uint16 v6 = cinematic_var19;
    if (*(uint16 *)((char *)&cinematic_var20 + cinematic_var19) == 3072) {
      *(uint16 *)((char *)&cinematic_var21 + cinematic_var19) = 0;
    } else {
      *(uint16 *)((char *)&cinematic_var20 + cinematic_var19) += 1024;
      enemy_projectile_pre_instr[(v6 >> 1) + 2] = 5;
    }
  }
}

void EnableTextGlowObjects_(void) {
  cinematic_var25 |= 0x8000u;
}

void DisableTextGlowObjects_(void) {
  cinematic_var25 &= ~0x8000u;
}

void CinematicFunction_Intro_Func128(uint16 a) {  // 0x8B98F9
  for (int i = 2046; i >= 0; i -= 2)
    *(uint16 *)((char *)ram3000.pause_menu_map_tilemap + (uint16)i) = a;
  cinematic_var26 = 0;
  cinematic_var21 = 0;
  cinematic_var22 = 0;
  cinematic_var23 = 0;
  cinematic_var24 = 0;
  cinematicbg_var3 = 18432;
  enemy_projectile_pre_instr[0] = 0;
  cinematic_var7 = 0;
  cinematic_var8 = 0;
  cinematic_var9 = 0;
  cinematic_var10 = 0;
}

void CallCreditsObjectFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnCinematicFunction_nullsub_116: return;
  case fnCinematicBgPreInstr_SamusBlink: CinematicBgPreInstr_SamusBlink(k); return;
  default: Unreachable();
  }
}

void CreditsObject_Init(uint16 j) {  // 0x8B9932
  uint8 *v1 = RomPtr_8B(j);
  cinematic_var24 = *((uint16 *)v1 + 1);
  cinematic_var21 = *((uint16 *)v1 + 2);
  cinematic_var22 = 1;
  cinematic_var23 = 0;
  CallCreditsObjectFunc(*(uint16 *)v1 | 0x8B0000, 0);
}

void CreditsObject_Process(void) {  // 0x8B9955
  if ((cinematic_var25 & 0x8000u) != 0) {
    if (cinematic_var21)
      CreditsObject_ProcessOne();
    CinematicUpdateSomeBg();
  }
}

uint16 CallIntroObjectInstr(uint32 ea, uint16 k, uint16 j) {
  switch (ea) {
  case fnIntroObject_Delete: return IntroObject_Delete(k, j);
  case fnIntroObject_Goto: return IntroObject_Goto(k, j);
  case fnIntroObject_DecTimerGoto: return IntroObject_DecTimerGoto(k, j);
  case fnIntroObject_SetTimer: return IntroObject_SetTimer(k, j);
  case fnCinematicFunction_Intro_Func219: return CinematicFunction_Intro_Func219(k, j);
  default: return Unreachable();
  }
}

void CreditsObject_ProcessOne(void) {  // 0x8B996A
  int i;

  CallCreditsObjectFunc(cinematic_var24 | 0x8B0000, 0);
  uint16 v0 = (__PAIR32__(cinematic_var10, cinematic_var9) + 0x8000) >> 16;
  cinematic_var9 += 0x8000;
  cinematic_var10 = v0;
  if (!sign16(v0 - enemy_projectile_pre_instr[0] - 8)) {
    enemy_projectile_pre_instr[0] = cinematic_var10;
    for (i = cinematic_var21; ; ) {
      uint16 *v2 = (uint16 *)RomPtr_8C(i);
      uint16 v3 = *v2;
      if ((*v2 & 0x8000u) == 0)
        break;
      R18_ = *v2;
      i = CallIntroObjectInstr(v3 | 0x8B0000, 0, i + 2);
      if (!i)
        return;
    }
    CreditsObject_Func1(i);
    cinematic_var26 = ((uint8)cinematic_var26 + 1) & 0x1F;
    cinematic_var21 = i + 4;
  }
}

void CreditsObject_Func1(uint16 j) {  // 0x8B99C1
  *(VoidP *)((char *)&R0_.addr + 1) = 32512;
  R0_.addr = 0;
  uint16 RegWord = Mult8x8(cinematic_var26, 0x40);
  R20_ = 31;
  uint16 v2 = *((uint16 *)RomPtr_8C(j) + 1);
  do {
    *(uint16 *)((char *)ram3000.pause_menu_map_tilemap + RegWord) = *(uint16 *)IndirPtr(&R0_, v2);
    RegWord += 2;
    v2 += 2;
    --R20_;
  } while ((R20_ & 0x8000u) == 0);
}

uint16 IntroObject_Delete(uint16 k, uint16 j) {  // 0x8B99FE
  cinematic_var21 = 0;
  return 0;
}

uint16 IntroObject_Goto(uint16 k, uint16 j) {  // 0x8B9A06
  return *(uint16 *)RomPtr_8C(j);
}

uint16 IntroObject_DecTimerGoto(uint16 k, uint16 j) {  // 0x8B9A0D
  if (--cinematic_var23)
    return IntroObject_Goto(k, j);
  else
    return j + 2;
}

uint16 IntroObject_SetTimer(uint16 k, uint16 j) {  // 0x8B9A17
  cinematic_var23 = *(uint16 *)RomPtr_8C(j);
  return j + 2;
}

void CallCinematicFunction(uint32 ea) {
  switch (ea) {
  case fnCinematicFunctionNone: CinematicFunctionNone(); return;
  case fnCinematicFunctionOpening: CinematicFunctionOpening(); return;
  case fnCinematicFunc_Func2: CinematicFunc_Func2(); return;
  case fnCinematicFunc_Func4: CinematicFunc_Func4(); return;
  case fnCinematicFunc_Func5: CinematicFunc_Func5(); return;
  case fnCinematicFunc_Func7: CinematicFunc_Func7(); return;
  case fnnullsub_117: return;
  case fnCinematicFunc_Func1: CinematicFunc_Func1(); return;
  case fnCinematicFunc_Func10: CinematicFunc_Func10(); return;
  case fnCinematicFunc_Func9: CinematicFunc_Func9(); return;
  case fnnullsub_120: return;
  case fnnullsub_121: return;
  case fnCinematicFunc_Nothing: CinematicFunc_Nothing(); return;
  case fnCinematicFunction_Intro_Initial: CinematicFunction_Intro_Initial(); return;
  case fnCinematicFunction_Intro_FadeIn: CinematicFunction_Intro_FadeIn(); return;
  case fnCinematicFunction_Intro_DrawInitJpn: CinematicFunction_Intro_DrawInitJpn(); return;
  case fnCinematicFunction_Intro_LastMetroidCaptivity: CinematicFunction_Intro_LastMetroidCaptivity(); return;
  case fnCinematicFunc_Intro_QueueGalaxyIsAtPeace: CinematicFunc_Intro_QueueGalaxyIsAtPeace(); return;
  case fnCinematicFunc_Intro_WaitForQueueWait4secs: CinematicFunc_Intro_WaitForQueueWait4secs(); return;
  case fnCinematicFunc_Intro_FadeOut: CinematicFunc_Intro_FadeOut(); return;
  case fnCinematicFunc_Intro_WaitForFadeOut: CinematicFunc_Intro_WaitForFadeOut(); return;
  case fnCinematicFunc_Intro_SetupTextPage2: CinematicFunc_Intro_SetupTextPage2(); return;
  case fnCinematicFunc_Intro_WaitFadeinShowText: CinematicFunc_Intro_WaitFadeinShowText(); return;
  case fnCinematicFunc_Intro_WaitForFadeinSleep: CinematicFunc_Intro_WaitForFadeinSleep(); return;
  case fnCinematicFunction_Intro_WaitInputSetupMotherBrainFight: CinematicFunction_Intro_WaitInputSetupMotherBrainFight(); return;
  case fnCinematicFunction_Intro_WaitInputSetupBabyMetroid: CinematicFunction_Intro_WaitInputSetupBabyMetroid(); return;
  case fnCinematicFunction_Intro_Func11: CinematicFunction_Intro_Func11(); return;
  case fnCinematicFunction_Intro_Func12: CinematicFunction_Intro_Func12(); return;
  case fnCinematicFunction_Intro_Func15: CinematicFunction_Intro_Func15(); return;
  case fnCinematicFunction_Intro_Func15b: CinematicFunction_Intro_Func15b(); return;
  case fnCinematicFunction_Intro_XfadeGameplayFade: CinematicFunction_Intro_XfadeGameplayFade(); return;
  case fnCinematicFunction_Intro_XfadeScientistFade: CinematicFunction_Intro_XfadeScientistFade(); return;
  case fnCinematicFunction_Intro_Page2: CinematicFunction_Intro_Page2(); return;
  case fnCinematicFunction_Intro_Page3: CinematicFunction_Intro_Page3(); return;
  case fnCinematicFunction_Intro_Page4: CinematicFunction_Intro_Page4(); return;
  case fnCinematicFunction_Intro_Page5: CinematicFunction_Intro_Page5(); return;
  case fnCinematicFunction_Intro_Fadestuff: CinematicFunction_Intro_Fadestuff(); return;
  case fnCinematicFunction_Intro_Fadestuff2: CinematicFunction_Intro_Fadestuff2(); return;
  case fnCinematicFunction_Intro_Func34: CinematicFunction_Intro_Func34(); return;
  case fnCinematicFunction_Intro_Func54: CinematicFunction_Intro_Func54(); return;
  case fnCinematicFunction_Intro_Func55: CinematicFunction_Intro_Func55(); return;
  case fnCinematicFunction_Intro_Func56: CinematicFunction_Intro_Func56(); return;
  case fnCinematicFunction_Intro_Func67: CinematicFunction_Intro_Func67(); return;
  case fnCinematicFunction_Intro_Func72: CinematicFunction_Intro_Func72(); return;
  case fnCinematicFunction_Intro_Func73: CinematicFunction_Intro_Func73(); return;
  case fnCinematicFunctionBlackoutFromCeres: CinematicFunctionBlackoutFromCeres(); return;
  case fnCinematicFunction_Intro_Func74: CinematicFunction_Intro_Func74(); return;
  case fnCinematicFunction_Intro_Func75: CinematicFunction_Intro_Func75(); return;
  case fnCinematicFunction_Intro_Func76: CinematicFunction_Intro_Func76(); return;
  case fnCinematicFunction_Intro_Func77: CinematicFunction_Intro_Func77(); return;
  case fnCinematicFunction_Intro_Func84: CinematicFunction_Intro_Func84(); return;
  case fnCinematicFunction_Intro_Func85: CinematicFunction_Intro_Func85(); return;
  case fnCinematicFunction_Intro_Func86: CinematicFunction_Intro_Func86(); return;
  case fnCinematicFunction_Intro_Func87: CinematicFunction_Intro_Func87(); return;
  case fnCinematicFunction_Intro_Func88: CinematicFunction_Intro_Func88(); return;
  case fnCinematicFunction_Intro_Func105: CinematicFunction_Intro_Func105(); return;
  case fnCinematicFunction_Intro_Func106: CinematicFunction_Intro_Func106(); return;
  case fnCinematicFunction_Intro_Func107: CinematicFunction_Intro_Func107(); return;
  case fnCinematicFunction_Intro_Func108: CinematicFunction_Intro_Func108(); return;
  case fnnullsub_124: return;
  case fnCinematicFunction_Intro_Func95: CinematicFunction_Intro_Func95(); return;
  case fnCinematicFunctionEscapeFromCebes: CinematicFunctionEscapeFromCebes(); return;
  case fnCinematicFunction_Intro_Func109: CinematicFunction_Intro_Func109(); return;
  case fnCinematicFunction_Intro_Func110: CinematicFunction_Intro_Func110(); return;
  case fnCinematicFunction_Intro_Func111: CinematicFunction_Intro_Func111(); return;
  case fnCinematicFunction_Intro_Func112: CinematicFunction_Intro_Func112(); return;
  case fnCinematicFunction_Intro_Func113: CinematicFunction_Intro_Func113(); return;
  case fnCinematicFunction_Intro_Func114: CinematicFunction_Intro_Func114(); return;
  case fnCinematicFunction_Intro_Func115: CinematicFunction_Intro_Func115(); return;
  case fnCinematicFunction_Intro_Func116: CinematicFunction_Intro_Func116(); return;
  case fnCinematicFunction_Intro_Func117: CinematicFunction_Intro_Func117(); return;
  case fnCinematicFunction_Intro_Func118: CinematicFunction_Intro_Func118(); return;
  case fnnullsub_125: return;
  case fnCinematicFunction_Intro_Func119: CinematicFunction_Intro_Func119(); return;
  case fnCinematicFunction_Intro_Func120: CinematicFunction_Intro_Func120(); return;
  case fnCinematicFunction_Intro_Func121: CinematicFunction_Intro_Func121(); return;
  case fnCinematicFunction_Intro_Func123: CinematicFunction_Intro_Func123(); return;
  case fnCinematicFunction_Intro_Func124: CinematicFunction_Intro_Func124(); return;
  case fnnullsub_126: return;
  case fnCinematicFunction_Intro_Func125: CinematicFunction_Intro_Func125(); return;
  case fnCinematicFunction_Intro_Func126: CinematicFunction_Intro_Func126(); return;
  case fnCinematicFunction_Intro_Func129: CinematicFunction_Intro_Func129(); return;
  case fnCinematicFunction_Intro_Func130: CinematicFunction_Intro_Func130(); return;
  case fnCinematicFunction_Intro_Func131: CinematicFunction_Intro_Func131(); return;
  case fnCinematicFunction_Intro_Func132: CinematicFunction_Intro_Func132(); return;
  case fnCinematicFunction_Intro_Func134: CinematicFunction_Intro_Func134(); return;
  case fnCinematicFunction_Intro_Func136: CinematicFunction_Intro_Func136(); return;
  case fnCinematicFunction_Intro_Func135: CinematicFunction_Intro_Func135(); return;
  case fnCinematicFunction_Intro_Func137: CinematicFunction_Intro_Func137(); return;
  case fnCinematicFunction_Intro_Func138: CinematicFunction_Intro_Func138(); return;
  case fnCinematicFunction_Intro_Func145: CinematicFunction_Intro_Func145(); return;
  case fnCinematicFunction_Intro_Func139: CinematicFunction_Intro_Func139(); return;
  case fnCinematicFunction_Intro_Func141: CinematicFunction_Intro_Func141(); return;
  case fnCinematicFunction_Intro_Func143: CinematicFunction_Intro_Func143(); return;
  case fnCinematicFunction_Intro_Func144: CinematicFunction_Intro_Func144(); return;
  case fnCinematicFunction_Intro_Func148: CinematicFunction_Intro_Func148(); return;
  case fnnullsub_127: return;
  default: Unreachable();
  }
}

CoroutineRet GameState_1_OpeningCinematic_(void) {  // 0x8B9A22
  CallCinematicFunction(cinematic_function | 0x8b0000);
  HandleCinematicSprites();
  HandleMode7Objects();
  PaletteFxHandler();
  DrawCinematicSpriteObjects_Intro();
  MaybeSkipCinematics();
  HandleCinematicsTransitions();
  HandleMode7TransformationNoRotation();
  return kCoroutineNone;
}

void CinematicFunctionNone(void) {  // 0x8B9A47
  ;
}

void MaybeSkipCinematics(void) {  // 0x8B9A48
  if (sign16(cinematic_function + 0x60D8)
      && (joypad1_newkeys & (uint16)(kButton_B | kButton_Start | kButton_A)) != 0
      && !cinematic_var18) {
    cinematic_var18 = 1;
    screen_fade_delay = 0;
    screen_fade_counter = 2;
  }
}

static Func_V *const off_8B9A7B[4] = {  // 0x8B9A6C
  0,
  HandleCinematicsTransitions_1,
  HandleCinematicsTransitions_2,
  HandleCinematicsTransitions_3,
};

void HandleCinematicsTransitions(void) {
  if (cinematic_var18)
    off_8B9A7B[cinematic_var18]();
}

void HandleCinematicsTransitions_1(void) {  // 0x8B9A83
  if (AdvanceFastScreenFadeOut() & 1) {
    cinematic_var18 = 2;
    cinematic_function = FUNC16(nullsub_117);
    QueueMusic_Delayed8(6u);
  }
}

void HandleCinematicsTransitions_2(void) {  // 0x8B9A9C
  ClearCinematicSprites();
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BA119, 0);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BA125, 0);
  screen_fade_delay = 0;
  screen_fade_counter = 2;
  cinematic_var6 = 256;
  cinematic_var8 = 0;
  cinematic_var7 = 0;
  cinematic_var10 = 0;
  cinematic_var9 = 0;
  g_word_7E199B = 0;
  g_word_7E1999 = 0;
  g_word_7E199F = 0;
  g_word_7E199D = 0;
  cinematic_var18 = 3;
  uint16 v2 = 256;
  do {
    palette_buffer[v2 >> 1] = g_word_8CE1E9[v2 >> 1];
    v2 += 2;
  } while ((int16)(v2 - 512) < 0);
  palette_buffer[201] = 0x7FFF;
  palette_buffer[202] = 32128;
  ClearPaletteFXObjects();
  SpawnPalfxObject(addr_kPalfx_E1A0);
  SpawnPalfxObject(addr_kPalfx_E1A4);
  SetPpuBackdropBlack();
  reg_TM = 17;
  SpawnTitleScreenGradientObjs();
}

void CinematicSpriteInit_7(uint16 j) {  // 0x8B9B1A
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 128;
  cinematicbg_arr8[v1] = 48;
  cinematicbg_arr9[v1] = 1024;
}

void CinematicSpriteInit_8(uint16 j) {  // 0x8B9B2D
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 128;
  cinematicbg_arr8[v1] = 81;
  cinematicbg_arr9[v1] = 0;
}

void CinematicSpriteInit_9(uint16 j) {  // 0x8B9B40
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 128;
  cinematicbg_arr8[v1] = 196;
  cinematicbg_arr9[v1] = 2048;
}

void HandleCinematicsTransitions_3(void) {  // 0x8B9B53
  if (AdvanceFastScreenFadeIn() & 1) {
    cinematic_var18 = 0;
    demo_timer = 900;
    cinematic_function = FUNC16(CinematicFunc_Func1);
  }
}

void CinematicFunctionOpening(void) {  // 0x8B9B68
  LoadTitleSequenceGraphics();
  QueueMusic_Delayed8(0xFF03u);
  cinematic_function = FUNC16(CinematicFunctionNone);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BA0EF, FUNC16(CinematicFunctionNone));
  QueueMusic_Delayed8(5u);
}

void LoadTitleSequenceGraphics(void) {  // 0x8B9B87
  int16 v1;

  SetupPpuForTitleSequence();
  cur_irq_handler = 0;
  irqhandler_next_handler = 0;
  uint16 v0 = 0;
  do {
    palette_buffer[v0 >> 1] = g_word_8CE1E9[v0 >> 1];
    v0 += 2;
  } while ((int16)(v0 - 512) < 0);
  mov24(&decompress_src, 0x94E000u);
  static const LongPtr unk_8B9BB4 = LONGPTR(0x7f0000);
  static const LongPtr unk_8B9BC5 = LONGPTR(0x7f4000);
  static const LongPtr unk_8B9BD6 = LONGPTR(0x7f5000);
  static const LongPtr unk_8B9BE7 = LONGPTR(0x7f9000);
  DecompressToMem_IpArg(&unk_8B9BB4);
  mov24(&decompress_src, 0x96FC04u);
  DecompressToMem_IpArg(&unk_8B9BC5);
  mov24(&decompress_src, 0x9580D8u);
  DecompressToMem_IpArg(&unk_8B9BD6);
  mov24(&decompress_src, 0x95A5E1u);
  DecompressToMem_IpArg(&unk_8B9BE7);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8B9BFF = { 1, 0, 0x19, LONGPTR(0x7f0000), 0x4000 };
  SetupDmaTransfer(&unk_8B9BFF);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMAIN, 0);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0);
  v1 = 0x4000;
  do {
    WriteReg(VMDATAL, 0xFFu);
    --v1;
  } while (v1);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0);
  WriteReg(VMAIN, 0);
  static const StartDmaCopy unk_8B9C37 = { 1, 0, 0x18, LONGPTR(0x7f4000), 0x1000 };
  SetupDmaTransfer(&unk_8B9C37);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x60u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8B9C57 = { 1, 1, 0x18, LONGPTR(0x7f5000), 0x4000 };
  SetupDmaTransfer(&unk_8B9C57);
  WriteReg(MDMAEN, 2u);
  WriteRegWord(M7A, 0x100u);
  reg_M7A = 256;
  WriteRegWord(M7B, 0);
  reg_M7B = 0;
  WriteRegWord(M7C, 0);
  reg_M7C = 0;
  WriteRegWord(M7D, 0x100u);
  reg_M7D = 256;
  WriteRegWord(M7X, 0x80);
  reg_M7X = 128;
  WriteRegWord(M7Y, 0x80);
  reg_M7Y = 128;
  EnableNMI();
  screen_fade_delay = 1;
  screen_fade_counter = 1;
  EnableHdmaObjects();
  EnablePaletteFx();
  fx_layer_blending_config_a = 0;
  SpawnPalfxObject(addr_kPalfx_E1A0);
  SpawnPalfxObject(addr_kPalfx_E1A4);
  uint16 v2 = 0;  // undefined
  SpawnMode7Object(addr_kMode7ObjBabyMetroidInTitle, v2);
}

void CinematicSpriteInit_0(uint16 j) {  // 0x8B9CBC
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 129;
  cinematicbg_arr8[v1] = 112;
  cinematicbg_arr9[v1] = 512;
}

void sub_8B9CCF(uint16 k) {  // 0x8B9CCF
  reg_INIDISP = 15;
  cinematicspr_preinstr_func[k >> 1] = FUNC16(CinematicFunction_nullsub_116);
}

void sub_8B9CDE(void) {  // 0x8B9CDE
  ;
}

uint16 CinematicSprInstr_9CE1(uint16 k, uint16 j) {  // 0x8B9CE1
  reg_TM = 17;
  cinematic_function = FUNC16(CinematicFunc_Func2);
  cinematic_var6 = 72;
  cinematic_var8 = 315;
  cinematic_var7 = 0;
  cinematic_var10 = 225;
  cinematic_var9 = 0;
  g_word_7E199B = -2;
  g_word_7E1999 = 0x8000;
  SetPpuBackdropSomeColor();
  return j;
}

void CinematicFunc_Func2(void) {  // 0x8B9D17
  uint16 v0 = (__PAIR32__(g_word_7E199B, g_word_7E1999) + __PAIR32__(cinematic_var8, cinematic_var7)) >> 16;
  cinematic_var7 += g_word_7E1999;
  cinematic_var8 = v0;
  if (sign16(v0 + 7)) {
    reg_TM = 16;
    cinematic_function = FUNC16(CinematicFunctionNone);
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BA0F5, FUNC16(CinematicFunctionNone));
    SetPpuBackdropBlack();
  }
  MaybeSkipCinematics();
}

void CinematicSpriteInit_1(uint16 j) {  // 0x8B9D4A
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 129;
  cinematicbg_arr8[v1] = 112;
  cinematicbg_arr9[v1] = 512;
}

uint16 CinematicSprInstr_9D5D(uint16 k, uint16 j) {  // 0x8B9D5D
  reg_TM = 17;
  cinematic_function = FUNC16(CinematicFunc_Func4);
  cinematic_var6 = 96;
  cinematic_var8 = 44;
  cinematic_var7 = 0;
  cinematic_var10 = -155;
  cinematic_var9 = 0;
  g_word_7E199B = -2;
  g_word_7E1999 = 0x8000;
  return j;
}

void CinematicFunc_Func4(void) {  // 0x8B9D90
  uint16 v0 = (__PAIR32__(g_word_7E199B, g_word_7E1999) + __PAIR32__(cinematic_var8, cinematic_var7)) >> 16;
  cinematic_var7 += g_word_7E1999;
  cinematic_var8 = v0;
  if (sign16(v0 + 176)) {
    reg_TM = 16;
    cinematic_function = FUNC16(CinematicFunctionNone);
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BA0FB, FUNC16(CinematicFunctionNone));
    SetPpuBackdropBlack();
  }
  MaybeSkipCinematics();
}

void CinematicSpriteInit_2(uint16 j) {  // 0x8B9DC3
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 129;
  cinematicbg_arr8[v1] = 112;
  cinematicbg_arr9[v1] = 512;
}

uint16 CinematicSprInstr_9DD6(uint16 k, uint16 j) {  // 0x8B9DD6
  reg_TM = 17;
  cinematic_function = FUNC16(CinematicFunc_Func5);
  cinematic_var6 = 96;
  cinematic_var8 = -177;
  cinematic_var7 = 0;
  cinematic_var10 = -160;
  cinematic_var9 = 0;
  g_word_7E199B = 0;
  g_word_7E1999 = 0;
  g_word_7E199F = 1;
  g_word_7E199D = 0x8000;
  SetPpuBackdropSomeColor();
  return j;
}

void CinematicFunc_Func5(void) {  // 0x8B9E12
  uint16 v0 = (__PAIR32__(g_word_7E199F, g_word_7E199D) + __PAIR32__(cinematic_var10, cinematic_var9)) >> 16;
  cinematic_var9 += g_word_7E199D;
  cinematic_var10 = v0;
  if (!sign16(v0 - 163)) {
    reg_TM = 16;
    cinematic_function = FUNC16(CinematicFunctionNone);
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BA101, FUNC16(CinematicFunctionNone));
    SetPpuBackdropBlack();
  }
  MaybeSkipCinematics();
}

void CinematicSpriteInit_3(uint16 j) {  // 0x8B9E45
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 129;
  cinematicbg_arr8[v1] = 112;
  cinematicbg_arr9[v1] = 512;
}

uint16 CinematicSprInstr_Func8(uint16 k, uint16 j) {  // 0x8B9E58
  reg_TM = 17;
  cinematic_function = FUNC16(CinematicFunc_Func7);
  cinematic_var6 = 67;
  cinematic_var8 = 0;
  cinematic_var7 = 0;
  cinematic_var10 = 0;
  cinematic_var9 = 0;
  g_word_7E199B = 0;
  g_word_7E1999 = 0;
  g_word_7E199F = 0;
  g_word_7E199D = 0;
  SpawnTitleScreenGradientObjs();
  return j;
}


void CinematicFunc_Func7(void) {  // 0x8B9E8B
  if ((nmi_frame_counter_word & 1) == 0) {
    if (sign16(cinematic_var6 - 256)) {
      ++cinematic_var6;
    } else {
      cinematic_var6 = 256;
      cinematic_function = FUNC16(nullsub_117);
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BA107, FUNC16(nullsub_117));
    }
  }
}

void CinematicSpriteInit_4(uint16 j) {  // 0x8B9EB3
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 128;
  cinematicbg_arr8[v1] = 48;
  cinematicbg_arr9[v1] = 1024;
  SpawnPalfxObject(addr_kPalfx_E194);
}

uint16 CinematicSprInstr_Func9(uint16 k, uint16 j) {  // 0x8B9ECD
  uint16 v0 = 0x9ecd;
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BA113, v0);
  return j;
}

void CinematicSpriteInit_5(uint16 j) {  // 0x8B9ED6
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 128;
  cinematicbg_arr8[v1] = 81;
  cinematicbg_arr9[v1] = 512;
  SpawnPalfxObject(addr_kPalfx_E198);
}

uint16 CinematicSprInstr_sub_8B9EF0(uint16 k, uint16 j) {  // 0x8B9EF0
  cinematicbg_arr9[k >> 1] = 0;
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BA113, 0);
  return j;
}

void CinematicSpriteInit_6(uint16 j) {  // 0x8B9EFF
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 128;
  cinematicbg_arr8[v1] = 196;
  cinematicbg_arr9[v1] = 2048;
  SpawnPalfxObject(addr_kPalfx_E19C);
}

uint16 CinematicSprInstr_9F19(uint16 k, uint16 j) {  // 0x8B9F19
  demo_timer = 900;
  cinematic_function = FUNC16(CinematicFunc_Func1);
  return j;
}

void CinematicFunc_Func1(void) {  // 0x8B9F29
  bool v0 = (--demo_timer & 0x8000u) != 0;
  if (!demo_timer || v0) {
    cinematic_function = FUNC16(CinematicFunc_Func9);
LABEL_6:
    screen_fade_delay = 2;
    screen_fade_counter = 2;
    goto LABEL_7;
  }
  if ((joypad1_newkeys & (uint16)(kButton_B | kButton_Start | kButton_A)) != 0) {
    cinematic_function = FUNC16(CinematicFunc_Func10);
    goto LABEL_6;
  }
LABEL_7:;
}

void CinematicFunc_Func10(void) {  // 0x8B9F52
  uint16 j;

  if (AdvanceSlowScreenFadeOut() & 1) {
    EnableNMI();
    reg_BGMODE = 1;
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    reg_BG2HOFS = 0;
    reg_BG3HOFS = 0;
    reg_BG2VOFS = 0;
    reg_BG3VOFS = 0;
    cinematic_var5 = 0;
    cinematic_function = FUNC16(CinematicFunctionOpening);
    DisablePaletteFx();
    ClearPaletteFXObjects();
    for (int i = 656; i >= 0; i -= 2)
      *(uint16 *)((char *)&cinematic_var5 + (uint16)i) = 0;
    for (j = 510; (j & 0x8000u) == 0; j -= 2)
      hdma_table_1[j >> 1] = 0;
    game_state = 4;
    game_options_screen_index = 0;
  }
}

void CinematicFunc_Func9(void) {  // 0x8B9FAE
  uint16 j;

  if (AdvanceSlowScreenFadeOut() & 1) {
    EnableNMI();
    reg_BGMODE = 1;
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    reg_BG2HOFS = 0;
    reg_BG3HOFS = 0;
    reg_BG2VOFS = 0;
    reg_BG3VOFS = 0;
    cinematic_var5 = 0;
    cinematic_function = FUNC16(CinematicFunctionOpening);
    DisablePaletteFx();
    ClearPaletteFXObjects();
    for (int i = 656; i >= 0; i -= 2)
      *(uint16 *)((char *)&cinematic_var5 + (uint16)i) = 0;
    for (j = 510; (j & 0x8000u) == 0; j -= 2)
      hdma_table_1[j >> 1] = 0;
    game_state = 40;
    demo_scene = 0;
  }
}

void ConfigureTitleSequenceGradientHDMA(void) {  // 0x8BA00A
#define g_off_8CBC5D ((uint16*)RomPtr(0x8cbc5d))
  uint16 v0 = g_off_8CBC5D[(uint16)((uint8)(cinematic_var6 & 0xF0) >> 3) >> 1];
  for (int i = 0; ; i += 2) {
    uint16 v2 = *(uint16 *)RomPtr_8C(v0);
    hdma_table_1[i >> 1] = v2;
    if (!v2)
      break;
    v0 += 2;
  }
}

uint16 PlayBabyMetroidCry1(uint16 k, uint16 j) {  // 0x8BA25B
  QueueSfx3_Max6(0x23u);
  return j;
}

uint16 PlayBabyMetroidCry2(uint16 k, uint16 j) {  // 0x8BA263
  QueueSfx3_Max6(0x26u);
  return j;
}

uint16 PlayBabyMetroidCry3(uint16 k, uint16 j) {  // 0x8BA26B
  QueueSfx3_Max6(0x27u);
  return j;
}
static const uint16 kCinematicPal1[16] = { 0x1000, 0x6bf5, 0x2e41, 0x2da1, 0x2d01, 0x5e5f, 0x183f, 0x1014, 0x80a, 0x404, 0x4f9f, 0x3ed8, 0x2e12, 0x6f70, 0x7fff, 0x5ee0 };
static const uint16 kCinematicPal2[16] = { 0x3800, 0x6bf5, 0x6e1, 0x641, 0x5a1, 0x5e5f, 0x183f, 0x1014, 0x80a, 0x404, 0x4f9f, 0x3ed8, 0x2e12, 0x6f70, 0x7fff, 0x5ee0 };
static const uint16 kCinematicPal3[16] = { 0x3800, 0x77f8, 0x1344, 0x12a4, 0x1204, 0x6abf, 0x249f, 0x1c77, 0x146d, 0x1067, 0x5bff, 0x4b38, 0x3a72, 0x7bd3, 0x7fff, 0x6b43 };
static const uint16 kCinematicPal4[16] = { 0x3800, 0x7ffb, 0x1fa7, 0x1f07, 0x1e67, 0x771f, 0x30ff, 0x28da, 0x20d0, 0x1cca, 0x67ff, 0x579b, 0x46d5, 0x7bd6, 0x7fff, 0x77a6 };
uint16 CinematicSetPal1(uint16 k, uint16 j) {  // 0x8BA273
  for (int i = 30; i >= 0; i -= 2)
    palette_buffer[(i >> 1) + 48] = kCinematicPal1[i >> 1];
  return j;
}

uint16 CinematicSetPal2(uint16 k, uint16 j) {  // 0x8BA284
  for (int i = 30; i >= 0; i -= 2)
    palette_buffer[(i >> 1) + 48] = kCinematicPal2[i >> 1];
  return j;
}

uint16 CinematicSetPal3(uint16 k, uint16 j) {  // 0x8BA295
  for (int i = 30; i >= 0; i -= 2)
    palette_buffer[(i >> 1) + 48] = kCinematicPal3[i >> 1];
  return j;
}

uint16 CinematicSetPal4(uint16 k, uint16 j) {  // 0x8BA2A6
  for (int i = 30; i >= 0; i -= 2)
    palette_buffer[(i >> 1) + 48] = kCinematicPal4[i >> 1];
  return j;
}

CoroutineRet GameState_37_CeresGoesBoomWithSamus_(void) {  // 0x8BA35B
  CallCinematicFunction(cinematic_function | 0x8B0000);
  if ((cinematic_var11 & 0x8000u) == 0)
    ++cinematic_var11;
  ++cinematic_var12;
  HandleSamusDuringIntro();
  HandleCinematicSprites();
  HandleMode7Objects();
  HandleCinematicBgObjects();
  HandleTextGlowObjects();
  PaletteFxHandler();
  DrawIntroSprites();
  HandleMode7TransformationMatrix();
  return kCoroutineNone;
}

void CinematicFunc_Nothing(void) {  // 0x8BA391
}

void CinematicFunction_Intro_Initial(void) {  // 0x8BA395
  uint16 j;
  uint16 k;

  SetupPpu_Intro();
  cur_irq_handler = 0;
  irqhandler_next_handler = 0;
  area_index = 0;
  room_width_in_blocks = 16;
  room_height_in_blocks = 16;
  Samus_Initialize();
  UpdateBeamTilesAndPalette();
  samus_max_missiles = 900;
  samus_missiles = 900;
  cinematic_var15 = 0;
  samus_draw_handler = FUNC16(SamusDrawHandler_Default);
  ResetButtonAssignmentsToDefault();
  samus_invincibility_timer = 0;
  samus_knockback_timer = 0;
  for (int i = 656; i >= 0; i -= 2)
    *(uint16 *)((char *)&cinematic_var5 + (uint16)i) = 0;
  uint16 v1 = 0;
  do {
    palette_buffer[v1 >> 1] = kPalettes_Intro[v1 >> 1];
    v1 += 2;
  } while ((int16)(v1 - 512) < 0);
  static const LongPtr unk_8BA3FC = LONGPTR(0x7f0000);
  static const LongPtr unk_8BA40D = LONGPTR(0x7f8000);
  static const LongPtr unk_8BA41E = LONGPTR(0x7f9000);
  static const LongPtr unk_8BA42F = LONGPTR(0x7f9800);
  static const LongPtr unk_8BA440 = LONGPTR(0x7fb800);
  static const LongPtr unk_8BA451 = LONGPTR(0x7fe000);

  mov24(&decompress_src, 0x95f90e);
  DecompressToMem_IpArg(&unk_8BA3FC);
  mov24(&decompress_src, 0x95d089);
  DecompressToMem_IpArg(&unk_8BA40D);
  mov24(&decompress_src, 0x9788cc);
  DecompressToMem_IpArg(&unk_8BA41E);
  mov24(&decompress_src, 0x96ff14);
  DecompressToMem_IpArg(&unk_8BA42F);
  mov24(&decompress_src, 0x95e4c2);
  DecompressToMem_IpArg(&unk_8BA440);
  mov24(&decompress_src, 0x978d12);
  DecompressToMem_IpArg(&unk_8BA451);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BA469 = { 1, 1, 0x18, LONGPTR(0x7f0000), 0x8000 };
  SetupDmaTransfer(&unk_8BA469);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x40u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BA489 = { 1, 1, 0x18, LONGPTR(0x7f8000), 0x0900 };
  SetupDmaTransfer(&unk_8BA489);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x48u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BA4A9 = { 1, 1, 0x18, LONGPTR(0x7f9000), 0x0800 };
  SetupDmaTransfer(&unk_8BA4A9);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x4Cu);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BA4C9 = { 1, 1, 0x18, LONGPTR(0x7fe000), 0x0800 };
  SetupDmaTransfer(&unk_8BA4C9);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x50u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BA4E9 = { 1, 1, 0x18, LONGPTR(0x7f9800), 0x2000 };
  SetupDmaTransfer(&unk_8BA4E9);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x60u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BA509 = { 1, 1, 0x18, LONGPTR(0x9ad200), 0x2000 };
  SetupDmaTransfer(&unk_8BA509);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x6Eu);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BA529 = { 1, 1, 0x18, LONGPTR(0x7fb800), 0x2400 };
  SetupDmaTransfer(&unk_8BA529);
  WriteReg(MDMAEN, 2u);
  mov24(&decompress_src, 0x95D713u);
  static const LongPtr unk_8BA546 = LONGPTR(0x7fa000);
  DecompressToMem_IpArg(&unk_8BA546);
  EnableNMI();
  screen_fade_delay = 3;
  screen_fade_counter = 3;
  EnableHdmaObjects();
  ClearPaletteFXObjects();
  EnablePaletteFx();
  door_def_ptr = 0;
  layer1_x_pos = 0;
  layer1_y_pos = 0;
  cinematic_var16 = 0;
  for (j = 1023; (j & 0x8000u) == 0; --j)
    ram3800.cinematic_bg_tilemap[j] = *(uint16 *)&BTS[j * 2 + 11262];
  for (k = 127; (k & 0x8000u) == 0; --k)
    ram3000.pause_menu_map_tilemap[k] = *(uint16 *)((char *)g_word_8CD81B + k * 2);
  cinematic_var11 = -1;
  cinematic_function = FUNC16(CinematicFunction_Intro_FadeIn);
  QueueMusic_Delayed8(0);
  QueueMusic_Delayed8(0xFF3Fu);
}

void CinematicFunction_Intro_FadeIn(void) {  // 0x8BA5A7
  if (!(HasQueuedMusic() & 1)) {
    cinematic_function = FUNC16(CinematicFunction_Intro_DrawInitJpn);
    screen_fade_delay = 2;
    screen_fade_counter = 2;
  }
}

void CinematicFunction_Intro_DrawInitJpn(void) {  // 0x8BA5BD
  if (AdvanceSlowScreenFadeIn() & 1) {
    cinematic_function = FUNC16(CinematicFunction_Intro_LastMetroidCaptivity);
    cinematic_var4 = 60;
    if (japanese_text_flag) {
      uint16 v0 = vram_write_queue_tail;
      gVramWriteEntry(vram_write_queue_tail)->size = 256;
      v0 += 2;
      gVramWriteEntry(v0)->size = 12288;
      v0 += 2;
      LOBYTE(gVramWriteEntry(v0++)->size) = 126;
      gVramWriteEntry(v0)->size = 20192;
      vram_write_queue_tail = v0 + 2;
    }
  }
}

void CinematicFunction_Intro_LastMetroidCaptivity(void) {  // 0x8BA5F8
  bool v0 = (--cinematic_var4 & 0x8000u) != 0;
  if (!cinematic_var4 || v0) {
    cinematic_function = FUNC16(CinematicFunc_Intro_QueueGalaxyIsAtPeace);
    cinematic_var4 = 200;
    QueueMusic_Delayed8(5u);
  }
}

void CinematicFunc_Intro_QueueGalaxyIsAtPeace(void) {  // 0x8BA613
  bool v0 = (--cinematic_var4 & 0x8000u) != 0;
  if (!cinematic_var4 || v0) {
    QueueMusic_Delayed8(0);
    QueueMusic_Delayed8(0xFF42u);
    QueueMusic_DelayedY(5u, 0xEu);
    cinematic_function = FUNC16(CinematicFunc_Intro_WaitForQueueWait4secs);
  }
}

void CinematicFunc_Intro_WaitForQueueWait4secs(void) {  // 0x8BA639
  if (!(HasQueuedMusic() & 1)) {
    cinematic_function = FUNC16(CinematicFunc_Intro_FadeOut);
    cinematic_var4 = 240;
  }
}

void CinematicFunc_Intro_FadeOut(void) {  // 0x8BA64C
  bool v0 = (--cinematic_var4 & 0x8000u) != 0;
  if (!cinematic_var4 || v0) {
    cinematic_function = FUNC16(CinematicFunc_Intro_WaitForFadeOut);
    screen_fade_delay = 2;
    screen_fade_counter = 2;
  }
}

void CinematicFunc_Intro_WaitForFadeOut(void) {  // 0x8BA663
  if (AdvanceSlowScreenFadeOut() & 1)
    cinematic_function = FUNC16(CinematicFunc_Intro_SetupTextPage2);
}

void CinematicFunc_Intro_SetupTextPage2(void) {  // 0x8BA66F
  ClearJapaneseTextTiles();
  WriteReg(VMADDL, 0x80);
  WriteReg(VMADDH, 0x41u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BA689 = { 1, 1, 0x18, LONGPTR(0x7e4000), 0x0600 };
  SetupDmaTransfer(&unk_8BA689);
  WriteReg(MDMAEN, 2u);
  WriteReg(TM, 0x16u);
  reg_TM = 22;
  ClearCinematicBgObjects(0x2F);
  EnableCinematicBgObjects();
  EnableCinematicBgTilemapUpdates();
  cinematicbg_var3 = addr_unk_604C00;
  for (int i = 127; i >= 0; --i) {
    ram3000.pause_menu_map_tilemap[i] = 15401;
    ram3000.pause_menu_map_tilemap[i + 896] = 15401;
  }
  uint16 v1 = 768;
  uint16 v2 = 0;
  do {
    ram3000.pause_menu_map_tilemap[v1] = g_word_8BA72B[v2 >> 1];
    v2 += 2;
    ++v1;
  } while ((int16)(v1 * 2 - 1792) < 0);
  *(uint32 *)&ram3000.menu.menu_tilemap[286] = 472456233;
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCE6D, 0x1C29u);
  cinematic_var4 = 127;
  SpawnCinematicBgObject(addr_kCinematicBgObjectDef_8BCF63, 0x4C00u);
  cinematic_var10 = 8;
  reg_BG2VOFS = 8;
  reg_BG3VOFS = 8;
  ClearEnglishTextTilemap();
  SetSomeStuffForSpriteObject_16();
  QueueMusic_Delayed8(0);
  QueueMusic_Delayed8(0xFF36u);
  QueueMusic_DelayedY(5u, 0xEu);
  cinematic_function = FUNC16(CinematicFunc_Intro_WaitFadeinShowText);
}

void CinematicFunc_Intro_WaitFadeinShowText(void) {  // 0x8BA82B
  if (!(HasQueuedMusic() & 1)) {
    cinematic_function = FUNC16(CinematicFunc_Intro_WaitForFadeinSleep);
    screen_fade_delay = 2;
    screen_fade_counter = 2;
    SpawnCinematicBgObject(addr_kCinematicBgObjectDef_8BCF3F, 0x4C00u);
  }
}

void CinematicFunc_Intro_WaitForFadeinSleep(void) {  // 0x8BA84A
  if (AdvanceSlowScreenFadeIn() & 1)
    cinematic_function = FUNC16(CinematicFunc_Nothing);
}

void ClearEnglishTextTilemap(void) {  // 0x8BA856
  uint16 v0 = 128;
  do
    ram3000.pause_menu_map_tilemap[v0++] = 47;
  while ((int16)(v0 * 2 - 1536) < 0);
}

void ClearJapaneseTextTiles(void) {  // 0x8BA86A
  uint16 v0 = 0;
  do {
    ram4000.xray_tilemaps[v0] = *(uint16 *)&BTS[7822];
    ram4000.xray_tilemaps[v0 + 1] = *(uint16 *)&BTS[7824];
    ram4000.xray_tilemaps[v0 + 2] = *(uint16 *)&BTS[7826];
    ram4000.xray_tilemaps[v0 + 3] = *(uint16 *)&BTS[7828];
    ram4000.xray_tilemaps[v0 + 4] = *(uint16 *)&BTS[7830];
    ram4000.xray_tilemaps[v0 + 5] = *(uint16 *)&BTS[7832];
    ram4000.xray_tilemaps[v0 + 6] = *(uint16 *)&BTS[7834];
    ram4000.xray_tilemaps[v0 + 7] = *(uint16 *)&BTS[7836];
    v0 += 8;
  } while ((int16)(v0 * 2 - 1536) < 0);
}

void SetSomeStuffForSpriteObject_2(uint16 v0) {  // 0x8BA8C1
  int v1 = v0 >> 1;
  cinematicbg_arr7[v1] = 8;
  cinematicbg_arr8[v1] = 24;
  cinematicbg_arr9[v1] = 3072;
}

void SetSomeStuffForSpriteObject_3(uint16 j) {  // 0x8BA8D5
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 112;
  cinematicbg_arr8[v1] = 155;
  cinematicbg_arr9[v1] = 3584;
}

void SetSomeStuffForSpriteObject_4_MetroidEgg(uint16 k) {  // 0x8BA8E8
  if (sign16(samus_x_pos - 169)) {
    int v1 = k >> 1;
    cinematicspr_instr_timer[v1] = 1;
    cinematicspr_instr_ptr[v1] = addr_word_8BCB3B;
    cinematicspr_preinstr_func[v1] = FUNC16(CinematicFunction_nullsub_116);
  }
}

void CinematicSprPreInstr_A903(uint16 k) {  // 0x8BA903
  if (!cinematic_var13) {
    int v1 = k >> 1;
    cinematicspr_instr_timer[v1] = 1;
    cinematicspr_instr_ptr[v1] = addr_off_8BCE53;
    cinematic_var15 = 0;
  }
}

uint16 CinematicSprInstr_SpawnMetroidEggParticles(uint16 k, uint16 j) {  // 0x8BA918
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCECD, 0);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCED3, 1u);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCED9, 2u);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEDF, 3u);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEE5, 4u);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEEB, 5u);
  QueueSfx2_Max6(0xBu);
  return j;
}
static const int16 kSetSomeStuffForSpriteObject_6_Tab[12] = { 0x5c, 0x58, 0x63, 0x58, 0x59, 0x5d, 0x60, 0x5b, 0x66, 0x5e, 0x63, 0x60 };
void SetSomeStuffForSpriteObject_6(uint16 j) {  // 0x8BA958
  uint16 v1 = cinematic_spawn_param;
  int v2 = j >> 1;
  cinematicspr_goto_timer[v2] = cinematic_spawn_param;
  int v3 = (uint16)(4 * v1) >> 1;
  cinematicbg_arr7[v2] = kSetSomeStuffForSpriteObject_6_Tab[v3] + 16;
  cinematicbg_arr8[v2] = kSetSomeStuffForSpriteObject_6_Tab[v3 + 1] + 59;
  cinematicbg_arr9[v2] = 3584;
}
static const int16 kSetSomeStuffForSpriteObject_7_Tab0[12] = { -1, 0x4000, 0, 0x4000, -1, -0x8000, -1, 0x2000, 0, -0x8000, 0, 0x2000 };
static const uint16 kSetSomeStuffForSpriteObject_7_Tab1[82] = {  // 0x8BA994
  0xfffe,      0, 0xfffe, 0xc000, 0xfffe, 0xa000, 0xfffe, 0x8000, 0xfffe, 0x6000, 0xfffe, 0x4000,
  0xfffe, 0x2000, 0xffff,      0, 0xffff, 0xc000, 0xffff, 0xa000, 0xffff, 0x8000, 0xffff, 0x6000,
  0xffff, 0x4000, 0xffff, 0x2000,      0,      0,      0, 0x2000,      0, 0x4000,      0, 0x6000,
       0, 0x8000,      0, 0xa000,      0, 0xc000,      0, 0xe000,      1,      0,      1, 0x2000,
       1, 0x4000,      1, 0x6000,      1, 0x8000,      1, 0xa000,      1, 0xc000,      1, 0xe000,
       2,      0,      2, 0x2000,      2, 0x4000,      2, 0x6000,      2, 0x8000,      2, 0xa000,
       2, 0xc000,      2, 0xe000,  0x9dad, 0x991b, 0x1b7d,0x97ad, 0x991a, 0x1a7d,
};
void SetSomeStuffForSpriteObject_7(uint16 k) {

  int v1 = k >> 1;
  uint16 v2 = cinematicspr_arr6[v1];
  int v3 = (uint16)(4 * LOBYTE(cinematicspr_goto_timer[v1])) >> 1;
  assert(v3 + 1 < 12);
  bool v4 = __CFADD__uint16(kSetSomeStuffForSpriteObject_7_Tab0[v3 + 1], v2);
  cinematicspr_arr6[v1] = kSetSomeStuffForSpriteObject_7_Tab0[v3 + 1] + v2;
  cinematicbg_arr7[v1] += kSetSomeStuffForSpriteObject_7_Tab0[v3] + v4;
  uint16 v5 = cinematicspr_arr7[v1];
  int v6 = (uint16)(4 * HIBYTE(cinematicspr_goto_timer[v1])) >> 1;
  assert(v6 + 1 < 82);
  v4 = __CFADD__uint16(kSetSomeStuffForSpriteObject_7_Tab1[v6 + 1], v5);
  cinematicspr_arr7[v1] = kSetSomeStuffForSpriteObject_7_Tab1[v6 + 1] + v5;
  uint16 v7 = kSetSomeStuffForSpriteObject_7_Tab1[v6] + v4 + cinematicbg_arr8[v1];
  cinematicbg_arr8[v1] = v7;
  if (sign16(v7 - 168)) {
    cinematicspr_goto_timer[v1] += 256;
  } else {
    cinematicspr_instr_timer[v1] = 1;
    cinematicspr_instr_ptr[v1] = addr_off_8BCE53;
  }
}

void SetSomeStuffForSpriteObject_8(uint16 j) {  // 0x8BAA9A
  int v1 = j >> 1;
  cinematicspr_goto_timer[v1] = cinematic_spawn_param;
  cinematicbg_arr7[v1] = cinematicbg_arr7[13];
  cinematicbg_arr8[v1] = cinematicbg_arr8[13];
  cinematicbg_arr9[v1] = 3584;
}

static const int16 kSetSomeStuffForSpriteObject_9_Tab0[10] = { -1, 0, -1, -0x8000, 1, 0, 0, -0x8000, -1, -0x8000 };
static const int16 kSetSomeStuffForSpriteObject_9_Tab2[124] = {  // 0x8BAAB3
  -2,       0, -2, -0x4000, -2, -0x6000, -2, -0x8000, -2,  0x6000, -2,  0x4000,
  -2,  0x2000, -1,       0, -1, -0x4000, -1, -0x6000, -1, -0x8000, -1,  0x6000,
  -1,  0x4000, -1,  0x2000,  0,       0,  0,  0x2000,  0,  0x4000,  0,  0x6000,
   0, -0x8000,  0, -0x6000,  0, -0x4000,  0, -0x2000,  1,       0,  1,  0x2000,
   1,  0x4000,  1,  0x6000,  1, -0x8000,  1, -0x6000,  1, -0x4000,  1, -0x2000,
   2,       0,  2,  0x2000,  2,  0x4000,  2,  0x6000,  2, -0x8000,  2, -0x6000,
   2, -0x4000,  2, -0x2000,  3,       0,  3,  0x2000,  3,  0x4000,  3,  0x6000,
   3, -0x8000,  3, -0x6000,  3, -0x4000,  3, -0x2000,  4,       0,  4,  0x2000,
   4,  0x4000,  4,  0x6000,  4, -0x8000,  4, -0x6000,  4, -0x4000,  4, -0x2000,
   5,       0,  5,  0x2000,  5,  0x4000,  5,  0x6000,  5, -0x8000,  5, -0x6000,
   5, -0x4000,  5, -0x2000,
};
static const int16 kSetSomeStuffForSpriteObject_9_Tab1[138] = {
  -3,       0, -3, -0x4000, -3, -0x6000, -3, -0x8000, -3,  0x6000, -3,  0x4000,
  -3,  0x2000, -2,       0, -2, -0x4000, -2, -0x6000, -2, -0x8000, -2,  0x6000,
  -2,  0x4000, -2,  0x2000, -1,       0, -1, -0x4000, -1, -0x6000, -1, -0x8000,
  -1,  0x6000, -1,  0x4000, -1,  0x2000,  0,       0,  0,  0x2000,  0,  0x4000,
   0,  0x6000,  0, -0x8000,  0, -0x6000,  0, -0x4000,  0, -0x2000,  1,       0,
   1,  0x2000,  1,  0x4000,  1,  0x6000,  1, -0x8000,  1, -0x6000,  1, -0x4000,
   1, -0x2000,  2,       0,  2,  0x2000,  2,  0x4000,  2,  0x6000,  2, -0x8000,
   2, -0x6000,  2, -0x4000,  2, -0x2000,  3,       0,  3,  0x2000,  3,  0x4000,
   3,  0x6000,  3, -0x8000,  3, -0x6000,  3, -0x4000,  3, -0x2000,  4,       0,
   4,  0x2000,  4,  0x4000,  4,  0x6000,  4, -0x8000,  4, -0x6000,  4, -0x4000,
   4, -0x2000,  5,       0,  5,  0x2000,  5,  0x4000,  5,  0x6000,  5, -0x8000,
   5, -0x6000,  5, -0x4000,  5, -0x2000,
};

void SetSomeStuffForSpriteObject_9(uint16 k) {

  int v1 = k >> 1;
  uint16 v2 = cinematicspr_arr6[v1];
  int v3 = (uint16)(4 * LOBYTE(cinematicspr_goto_timer[v1])) >> 1;
  assert(v3 + 1 < 10);
  bool v4 = __CFADD__uint16(kSetSomeStuffForSpriteObject_9_Tab0[v3 + 1], v2);
  cinematicspr_arr6[v1] = kSetSomeStuffForSpriteObject_9_Tab0[v3 + 1] + v2;
  cinematicbg_arr7[v1] += kSetSomeStuffForSpriteObject_9_Tab0[v3] + v4;
  uint16 v5 = cinematicspr_goto_timer[v1];
  if ((v5 & 1) != 0) {
    uint16 v10, v11;
    v10 = 4 * HIBYTE(v5);
    v11 = cinematicspr_arr7[v1];
    int v12;
    v12 = v10 >> 1;
    assert(v12 + 1 < 124);
    v4 = __CFADD__uint16(kSetSomeStuffForSpriteObject_9_Tab2[v12 + 1], v11);
    cinematicspr_arr7[v1] = kSetSomeStuffForSpriteObject_9_Tab2[v12 + 1] + v11;
    uint16 v13;
    v13 = kSetSomeStuffForSpriteObject_9_Tab2[v12] + v4 + cinematicbg_arr8[v1];
    cinematicbg_arr8[v1] = v13;
    if (!sign16(v13 - 168)) {
LABEL_5:
      cinematicspr_instr_timer[v1] = 1;
      cinematicspr_instr_ptr[v1] = addr_word_8BCD71;
      cinematicspr_preinstr_func[v1] = FUNC16(CinematicFunction_nullsub_116);
      return;
    }
  } else {
    uint16 v6 = 4 * HIBYTE(v5);
    uint16 v7 = cinematicspr_arr7[v1];
    int v8 = v6 >> 1;
    assert(v8 + 1 < 138);
    v4 = __CFADD__uint16(kSetSomeStuffForSpriteObject_9_Tab1[v8 + 1], v7);
    cinematicspr_arr7[v1] = kSetSomeStuffForSpriteObject_9_Tab1[v8 + 1] + v7;
    uint16 v9 = kSetSomeStuffForSpriteObject_9_Tab1[v8] + v4 + cinematicbg_arr8[v1];
    cinematicbg_arr8[v1] = v9;
    if (!sign16(v9 - 168))
      goto LABEL_5;
  }
  cinematicspr_goto_timer[v1] += 256;
}

void SetSomeStuffForSpriteObject_10(uint16 j) {  // 0x8BAD55
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 84;
  cinematicbg_arr8[v1] = 139;
  cinematicbg_arr9[v1] = 3072;
}

void SetSomeStuffForSpriteObject_11(uint16 k) {  // 0x8BAD68
  if (cinematic_var13) {
    if ((cinematic_var4 & 3) == 0) {
      if (cinematic_var8) {
        --cinematic_var8;
        ++cinematicbg_arr7[k >> 1];
      }
    }
  } else {
    int v1 = k >> 1;
    cinematicspr_instr_timer[v1] = 1;
    cinematicspr_instr_ptr[v1] = addr_off_8BCE53;
  }
}

void SetSomeStuffForSpriteObject_12(uint16 j) {  // 0x8BAD93
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 112;
  cinematicbg_arr8[v1] = 111;
  cinematicbg_arr9[v1] = 3072;
}

void SetSomeStuffForSpriteObject_13(uint16 k) {  // 0x8BADA6
  if (cinematic_var13) {
    if ((cinematic_var4 & 3) == 0) {
      if (sign16(cinematic_var10 - 8)) {
        ++cinematic_var10;
        --cinematicbg_arr8[k >> 1];
      }
    }
  } else {
    int v1 = k >> 1;
    cinematicspr_instr_timer[v1] = 1;
    cinematicspr_instr_ptr[v1] = addr_off_8BCE53;
  }
}

uint16 CinematicBgInstr_SetSomeStuffForSpriteObject_14(uint16 k, uint16 j) {  // 0x8BADD4
  cinematicspr_instr_ptr[15] = addr_unk_8BCC03;
  cinematicspr_instr_timer[15] = 1;
  return j;
}

void SetSomeStuffForSpriteObject_15(void) {  // 0x8BADE1
  cinematicbg_arr7[15] = 8;
  cinematicbg_arr8[15] = 248;
}

void SetSomeStuffForSpriteObject_16(void) {  // 0x8BADEE
  cinematicbg_arr7[15] = 8;
  cinematicbg_arr8[15] = 24;
  cinematicspr_instr_ptr[15] = addr_word_8BCBFB;
  cinematicspr_instr_timer[15] = 1;
}

void SetSomeStuffForSpriteObject_17(uint16 j) {  // 0x8BAE07
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 128;
  cinematicbg_arr8[v1] = 216;
  cinematicbg_arr9[v1] = 3072;
}

void SetSomeStuffForSpriteObject_18(uint16 k) {  // 0x8BAE1A
  if (cinematic_var16 == 59) {
    int v1 = k >> 1;
    cinematicspr_instr_timer[v1] = 1;
    cinematicspr_instr_ptr[v1] = addr_word_8BCC23;
  }
  if (cinematic_var13 == 127) {
    int v2 = k >> 1;
    cinematicspr_instr_timer[v2] = 1;
    cinematicspr_instr_ptr[v2] = addr_off_8BCE53;
  }
}

uint16 CinematicBgInstr_HandleCreateJpnText_Page1(uint16 k, uint16 j) {  // 0x8BAE43
  cinematicbg_var1 = 0;
  if (japanese_text_flag)
    SpawnMode7Object(addr_kMode7ObjectDef_8BD401, japanese_text_flag);
  return j;
}

uint16 CinematicBgInstr_SpawnMarkerWaitInput_Page1(uint16 k, uint16 j) {  // 0x8BAE5B
  j = CinematicBgInstr_SetSomeStuffForSpriteObject_14(k, j);
  if (japanese_text_flag)
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCE73, japanese_text_flag);
  cinematic_function = FUNC16(CinematicFunction_Intro_WaitInputSetupMotherBrainFight);
  return j;
}

uint16 CinematicBgInstr_HandleCreateJpnText_Page2(uint16 k, uint16 j) {  // 0x8BAE79
  cinematicbg_var1 = 0;
  if (japanese_text_flag)
    SpawnMode7Object(addr_kMode7ObjectDef_8BD407, japanese_text_flag);
  return j;
}

uint16 CinematicBgInstr_SpawnMarkerWaitInput_Page2(uint16 k, uint16 j) {  // 0x8BAE91
  j = CinematicBgInstr_SetSomeStuffForSpriteObject_14(k, j);
  if (japanese_text_flag) {
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCE73, japanese_text_flag);
    SpawnMode7Object(addr_kMode7ObjectDef_8BD40D, 0);  // a undefined
  } else {
    cinematic_function = FUNC16(CinematicFunction_Intro_WaitInputSetupBabyMetroid);
  }
  return j;
}

void CinematicFunction_Intro_WaitInputSetupMotherBrainFight(void) {  // 0x8BAEB8
  if (cinematic_var16) {
    --cinematic_var16;
  } else if (joypad1_newkeys) {
    reg_BG1SC = 80;
    samus_pose = kPose_02_FaceL_Normal;
    SamusFunc_F433();
    Samus_SetAnimationFrameIfPoseChanged();
    samus_last_different_pose = samus_prev_pose;
    *(uint16 *)&samus_last_different_pose_x_dir = *(uint16 *)&samus_prev_pose_x_dir;
    samus_prev_pose = samus_pose;
    *(uint16 *)&samus_prev_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
    samus_x_pos = 155;
    samus_prev_x_pos = 155;
    samus_y_pos = 115;
    samus_prev_y_pos = 115;
    cinematic_var8 = 0;
    cinematic_var13 = 127;
    R18_ = 0;
    SpawnCinematicSpriteObjectToR18(addr_kCinematicSpriteObjectDef_8BCE55, 0x7Fu);
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCF27, 0);
    uint16 v1 = 0;
    do {
      level_data[v1 >> 1] = kLevelData_MotherBrainRoomFromCutscene[v1 >> 1];
      v1 += 2;
    } while ((int16)(v1 - 448) < 0);
    uint16 v2 = 0;
    do {
      *(uint16 *)&BTS[v2] = 0;
      v2 += 2;
    } while ((int16)(v2 - 512) < 0);
    hud_item_index = 1;
    frame_handler_alfa = FUNC16(Samus_FrameHandlerAlfa_Func12);
    frame_handler_beta = FUNC16(Samus_Func15);
    ResetDemoData();
    EnableDemoInput();
    LoadDemoInputObject(0, addr_stru_918784);  // undefined
    cinematic_var15 = -1;
    CinematicFunction_Intro_SetupTransitionToGameplay();
  }
}

void CinematicFunction_Intro_WaitInputSetupBabyMetroid(void) {  // 0x8BAF6C
  if (cinematic_var16) {
    --cinematic_var16;
  } else if (joypad1_newkeys) {
    reg_BG1SC = 84;
    room_width_in_blocks = 32;
    room_height_in_blocks = 16;
    samus_pose = kPose_02_FaceL_Normal;
    SamusFunc_F433();
    Samus_SetAnimationFrameIfPoseChanged();
    samus_last_different_pose = samus_prev_pose;
    *(uint16 *)&samus_last_different_pose_x_dir = *(uint16 *)&samus_prev_pose_x_dir;
    samus_prev_pose = samus_pose;
    *(uint16 *)&samus_prev_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
    samus_x_pos = 376;
    samus_prev_x_pos = 376;
    samus_y_pos = 147;
    samus_prev_y_pos = 147;
    cinematic_var8 = 0;
    cinematic_var13 = 127;
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCE5B, 0x7Fu);
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCE79, 0);
    cinematic_var11 = 0;
    uint16 v1 = 0;
    do {
      level_data[v1 >> 1] = kLevelData_RoomWithBabyMetroidHatching[v1 >> 1];
      v1 += 2;
    } while ((int16)(v1 - 768) < 0);
    hud_item_index = 0;
    frame_handler_alfa = FUNC16(Samus_FrameHandlerAlfa_Func12);
    frame_handler_beta = FUNC16(Samus_Func15);
    ResetDemoData();
    EnableDemoInput();
    LoadDemoInputObject(0, addr_stru_91877E);  // a undefined
    cinematic_var15 = 1;
    CinematicFunction_Intro_SetupTransitionToGameplay();
  }
}

void CinematicFunction_Intro_SetupTransitionToGameplay(void) {  // 0x8BB018
  reg_TM = 6;
  reg_TS = 17;
  reg_CGWSEL = 2;
  reg_CGADSUB = 0;
  SpawnIntroCutsceneCrossfadeHdma();
  SetSomeStuffForSpriteObject_15();
  cinematic_function = FUNC16(CinematicFunction_Intro_XfadeGameplayFade);
  uint16 v0 = 0;
  do {
    tilemap_stuff[(v0 >> 1) + 256] = kPalettes_Intro[v0 >> 1];
    v0 += 2;
  } while ((int16)(v0 - 512) < 0);
  DecomposePaletteDataForFading();
  ClearYColorsFromIndexX(0x28u, 3u);
  ClearYColorsFromIndexX(0xE0u, 0x10u);
  ClearYColorsFromIndexX(0x180u, 0x20u);
  ClearYColorsFromIndexX(0x1E0u, 0x10u);
  ComposeFadingPalettes();
}

uint16 CinematicBgInstr_HandleCreateJpnText_Page3(uint16 k, uint16 j) {  // 0x8BB074
  cinematicbg_var1 = 0;
  if (japanese_text_flag)
    SpawnMode7Object(addr_kMode7ObjectDef_8BD413, japanese_text_flag);
  return j;
}

uint16 CinematicBgInstr_SpawnMarkerWaitInput_Page3(uint16 k, uint16 j) {  // 0x8BB08C
  j = CinematicBgInstr_SetSomeStuffForSpriteObject_14(k, j);
  if (japanese_text_flag) {
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCE73, japanese_text_flag);
    SpawnMode7Object(addr_kMode7ObjectDef_8BD419, 0);
  } else {
    cinematic_function = FUNC16(CinematicFunction_Intro_Func11);
  }
  return j;
}

uint16 CinematicBgInstr_HandleCreateJpnText_Page4(uint16 k, uint16 j) {  // 0x8BB0B3
  cinematicbg_var1 = 0;
  if (japanese_text_flag)
    SpawnMode7Object(addr_kMode7ObjectDef_8BD41F, japanese_text_flag);
  return j;
}

uint16 CinematicBgInstr_SpawnMarkerWaitInput_Page4(uint16 k, uint16 j) {  // 0x8BB0CB
  j = CinematicBgInstr_SetSomeStuffForSpriteObject_14(k, j);
  if (japanese_text_flag) {
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCE73, japanese_text_flag);
    SpawnMode7Object(addr_kMode7ObjectDef_8BD425, 0);
  } else {
    cinematic_function = FUNC16(CinematicFunction_Intro_Func12);
  }
  return j;
}

void CinematicFunction_Intro_Func11(void) {  // 0x8BB0F2
  if (cinematic_var16) {
    --cinematic_var16;
  } else if (joypad1_newkeys) {
    reg_BG1SC = 88;
    cinematic_var8 = 32;
    cinematic_var10 = 8;
    cinematic_var13 = 127;
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCE61, 0x7Fu);
    CinematicFunction_Intro_Func13();
  }
}

void CinematicFunction_Intro_Func12(void) {  // 0x8BB123
  if (cinematic_var16) {
    --cinematic_var16;
  } else if (joypad1_newkeys) {
    reg_BG1SC = 92;
    cinematic_var8 = 0;
    cinematic_var10 = -24;
    cinematic_var13 = 127;
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCE67, 0x7Fu);
    CinematicFunction_Intro_Func13();
  }
}

void CinematicFunction_Intro_Func13(void) {  // 0x8BB151
  reg_TM = 6;
  reg_TS = 17;
  reg_CGWSEL = 2;
  reg_CGADSUB = 0;
  SpawnIntroCutsceneCrossfadeHdma();
  SetSomeStuffForSpriteObject_15();
  cinematic_function = FUNC16(CinematicFunction_Intro_XfadeScientistFade);
  uint16 v0 = 0;
  do {
    tilemap_stuff[(v0 >> 1) + 256] = kPalettes_Intro[v0 >> 1];
    v0 += 2;
  } while ((int16)(v0 - 512) < 0);
  DecomposePaletteDataForFading();
  ClearYColorsFromIndexX(0x40u, 0x10u);
  ClearYColorsFromIndexX(0x1C0u, 9u);
  ComposeFadingPalettes();
}

uint16 CinematicBgInstr_HandleCreateJpnText_Page5(uint16 k, uint16 j) {  // 0x8BB19B
  cinematicbg_var1 = 0;
  if (japanese_text_flag)
    SpawnMode7Object(addr_kMode7ObjectDef_8BD42B, japanese_text_flag);
  return j;
}

uint16 CinematicBgInstr_SpawnMarkerWaitInput_Page5(uint16 k, uint16 j) {  // 0x8BB1B3
  j = CinematicBgInstr_SetSomeStuffForSpriteObject_14(k, j);
  if (japanese_text_flag) {
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCE73, japanese_text_flag);
    SpawnMode7Object(addr_kMode7ObjectDef_8BD431, 0);
  } else {
    cinematic_function = FUNC16(CinematicFunction_Intro_Func15);
  }
  return j;
}

void CinematicFunction_Intro_Func15(void) {  // 0x8BB1DA
  if (cinematic_var16) {
    --cinematic_var16;
  } else if (joypad1_newkeys) {
    cinematic_var13 = 127;
    if (japanese_text_flag) {
      cinematic_function = FUNC16(CinematicFunction_Intro_Func15b);
      DisableCinematicBgTilemapUpdates();
      ClearJapaneseTextTiles();
      TransferJapaneseTextTilesToVram();
    } else {
      CinematicFunction_Intro_Func15b();
    }
  }
}

void CinematicFunction_Intro_Func15b(void) {  // 0x8BB207
  EnableCinematicBgTilemapUpdates();
  ClearEnglishTextTilemap();
  SpawnCinematicBgObject(addr_kCinematicBgObjectDef_8BCF5D, 0x4C00u);
  cinematic_function = FUNC16(nullsub_121);
  SetSomeStuffForSpriteObject_16();
  QueueMusic_Delayed8(0);
}

uint16 CinematicBgInstr_Func16(uint16 k, uint16 j) {  // 0x8BB228
  cinematicbg_var1 = 0;
  if (japanese_text_flag)
    SpawnMode7Object(addr_kMode7ObjectDef_8BD437, japanese_text_flag);
  return j;
}

uint16 CinematicBgInstr_Func17(uint16 k, uint16 j) {  // 0x8BB240
  cinematic_function = FUNC16(CinematicFunction_Intro_Func34);
  screen_fade_delay = 1;
  screen_fade_counter = 1;
  return j;
}

void CinematicFunction_Intro_XfadeGameplayFade(void) {  // 0x8BB250
  if ((cinematic_var4 & 3) == 0) {
    FadeOutYColorsFromIndexX(0, 0x14u);
    FadeOutYColorsFromIndexX(0x60u, 0x10u);
    FadeOutYColorsFromIndexX(0x1D2u, 6u);
    FadeInYColorsFromIndexX(0x28u, 3u);
    FadeInYColorsFromIndexX(0xE0u, 0x10u);
    FadeInYColorsFromIndexX(0x180u, 0x20u);
    FadeInYColorsFromIndexX(0x1E0u, 0x10u);
    ComposeFadingPalettes();
  }
  if ((--cinematic_var4 & 0x8000u) != 0) {
    reg_TM = 21;
    reg_TS = 0;
    reg_CGWSEL = 0;
    uint16 v0 = 128;
    do
      ram3000.pause_menu_map_tilemap[v0++] = 47;
    while ((int16)(v0 * 2 - 1536) < 0);
    ClearJapaneseTextTiles();
    cinematic_function = FUNC16(CinematicFunc_Nothing);
    if ((cinematic_var15 & 0x8000u) != 0)
      SpawnPalfxObject(addr_kPalfx_E1BC);
  }
}

void CinematicFunction_Intro_XfadeScientistFade(void) {  // 0x8BB2D2
  if ((cinematic_var4 & 3) == 0) {
    FadeOutYColorsFromIndexX(0, 0x14u);
    FadeOutYColorsFromIndexX(0x60u, 0x10u);
    FadeOutYColorsFromIndexX(0x1D2u, 6u);
    FadeInYColorsFromIndexX(0x40u, 0x10u);
    FadeInYColorsFromIndexX(0x1C0u, 9u);
    ComposeFadingPalettes();
  }
  if ((--cinematic_var4 & 0x8000u) != 0) {
    reg_TM = 21;
    reg_TS = 0;
    reg_CGWSEL = 0;
    uint16 v0 = 128;
    do
      ram3000.pause_menu_map_tilemap[v0++] = 47;
    while ((int16)(v0 * 2 - 1536) < 0);
    ClearJapaneseTextTiles();
    cinematic_function = FUNC16(CinematicFunc_Nothing);
  }
}

uint16 CinematicSprInstr_StartIntroPage2(uint16 k, uint16 j) {  // 0x8BB336
  cinematic_function = FUNC16(CinematicFunction_Intro_Page2);
  Instr_StartIntroPage_Common();
  return j;
}

uint16 CinematicSprInstr_StartIntroPage3(uint16 k, uint16 j) {  // 0x8BB33E
  cinematic_function = FUNC16(CinematicFunction_Intro_Page3);
  Instr_StartIntroPage_Common();
  return j;
}

uint16 CinematicSprInstr_StartIntroPage4(uint16 k, uint16 j) {  // 0x8BB346
  cinematic_function = FUNC16(CinematicFunction_Intro_Page4);
  Instr_StartIntroPage_Common();
  return j;
}

uint16 CinematicSprInstr_StartIntroPage5(uint16 k, uint16 j) {  // 0x8BB34E
  cinematic_function = FUNC16(CinematicFunction_Intro_Page5);
  Instr_StartIntroPage_Common();
  return j;
}

void Instr_StartIntroPage_Common(void) {  // 0x8BB354
  DisableCinematicBgTilemapUpdates();
  ClearJapaneseTextTiles();
  TransferJapaneseTextTilesToVram();
}

void CinematicFunction_Intro_Page2(void) {  // 0x8BB35F
  SpawnCinematicBgObject(addr_kCinematicBgObjectDef_8BCF45, 0x4C00u);
  cinematic_function = FUNC16(CinematicFunction_Intro_Fadestuff);
  CinematicFunction_Intro_SetupStuff();
}

void CinematicFunction_Intro_Page3(void) {  // 0x8BB370
  SpawnCinematicBgObject(addr_kCinematicBgObjectDef_8BCF4B, 0x4C00u);
  cinematic_function = FUNC16(CinematicFunction_Intro_Fadestuff);
  CinematicFunction_Intro_SetupStuff();
}

void CinematicFunction_Intro_Page4(void) {  // 0x8BB381
  SpawnCinematicBgObject(addr_kCinematicBgObjectDef_8BCF51, 0x4C00u);
  cinematic_function = FUNC16(CinematicFunction_Intro_Fadestuff2);
  CinematicFunction_Intro_SetupStuff();
}

void CinematicFunction_Intro_Page5(void) {  // 0x8BB392
  SpawnCinematicBgObject(addr_kCinematicBgObjectDef_8BCF57, 0x4C00u);
  cinematic_function = FUNC16(CinematicFunction_Intro_Fadestuff2);
  CinematicFunction_Intro_SetupStuff();
}

void CinematicFunction_Intro_SetupStuff(void) {  // 0x8BB3A1
  reg_TM = 6;
  reg_TS = 17;
  reg_CGWSEL = 2;
  reg_CGADSUB = 0;
  uint16 v0 = 0;
  do {
    tilemap_stuff[(v0 >> 1) + 256] = kPalettes_Intro[v0 >> 1];
    v0 += 2;
  } while ((int16)(v0 - 512) < 0);
  DecomposePaletteDataForFading();
  ClearYColorsFromIndexX(0, 0x10u);
  ClearYColorsFromIndexX(0x60u, 0x10u);
  ClearYColorsFromIndexX(0x1D2u, 6u);
  ComposeFadingPalettes();
  cinematic_var4 = 127;
  EnableCinematicBgTilemapUpdates();
  SetSomeStuffForSpriteObject_16();
}

void CinematicFunction_Intro_Fadestuff(void) {  // 0x8BB3F4
  if ((cinematic_var13 & 3) == 0) {
    FadeInYColorsFromIndexX(0, 0x10u);
    FadeInYColorsFromIndexX(0x60u, 0x10u);
    FadeInYColorsFromIndexX(0x1D2u, 6u);
    FadeOutYColorsFromIndexX(0x28u, 3u);
    FadeOutYColorsFromIndexX(0xE0u, 0x10u);
    FadeOutYColorsFromIndexX(0x180u, 0x20u);
    FadeOutYColorsFromIndexX(0x1E0u, 0x10u);
    ComposeFadingPalettes();
  }
  if ((--cinematic_var13 & 0x8000u) != 0) {
    reg_TM = 22;
    reg_TS = 0;
    reg_CGWSEL = 0;
    reg_CGADSUB = 0;
    cinematic_function = FUNC16(CinematicFunc_Nothing);
  }
}

void CinematicFunction_Intro_Fadestuff2(void) {  // 0x8BB458
  if ((cinematic_var13 & 3) == 0) {
    FadeInYColorsFromIndexX(0, 0x10u);
    FadeInYColorsFromIndexX(0x60u, 0x10u);
    FadeInYColorsFromIndexX(0x1D2u, 6u);
    FadeOutYColorsFromIndexX(0x40u, 0x10u);
    FadeOutYColorsFromIndexX(0x1C0u, 9u);
    ComposeFadingPalettes();
  }
  if ((--cinematic_var13 & 0x8000u) != 0) {
    reg_TM = 22;
    reg_TS = 0;
    reg_CGWSEL = 0;
    reg_CGADSUB = 0;
    uint16 v0 = 0;
    do {
      palette_buffer[(v0 >> 1) + 224] = kPalettes_Intro[(v0 >> 1) + 224];
      v0 += 2;
    } while ((int16)(v0 - 14) < 0);
    cinematic_function = FUNC16(CinematicFunc_Nothing);
  }
}

void CinematicBgPreInstr_SamusBlink(uint16 k) {  // 0x8BB4BC
  if (cinematic_function == (uint16)FUNC16(CinematicFunction_Intro_Func15b)
      || cinematic_function == (uint16)FUNC16(nullsub_121)) {
    int v1 = k >> 1;
    cinematicbg_instr_ptr[v1] = -10733;
    cinematicbg_instr_timer[v1] = 1;
    cinematicbg_preinstr[v1] = FUNC16(CinematicFunction_nullsub_116);
  }
}

void CinematicFunction_Intro_Func19(uint16 k) {  // 0x8BB4DC
  if (!sign16(cinematicspr_instr_ptr[15] + 0x33FD))
    cinematicbg_instr_timer[k >> 1] = 1;
}

void CinematicFunction_Intro_Func18(void) {  // 0x8BB4EB
  DisableCinematicBgTilemapUpdates();
  palette_buffer[17] = kPalettes_Intro[17];
  palette_buffer[18] = kPalettes_Intro[18];
  palette_buffer[19] = kPalettes_Intro[19];
  CinematicFunction_Intro_Func20(addr_word_8BD389);
  CinematicFunction_Intro_Func20(addr_word_8BD389);
  TransferJapaneseTextTilesToVram();
}

void EnableCinematicBgTilemapUpdates_(void) {  // 0x8BB519
  EnableCinematicBgTilemapUpdates();
}

uint16 sub_8BB51E(uint16 k, uint16 j) {  // 0x8BB51E
  DisableCinematicBgTilemapUpdates();
  palette_buffer[17] = kPalettes_Intro[17];
  palette_buffer[18] = kPalettes_Intro[18];
  palette_buffer[19] = kPalettes_Intro[19];
  ClearJapaneseTextTiles();
  CinematicFunction_Intro_Func20(addr_word_8BCF75);
  CinematicFunction_Intro_Func20(addr_word_8BCFBD);
  TransferJapaneseTextTilesToVram();
  return j;
}

uint16 EnableCinematicBgTilemapUpdates__0(uint16 k, uint16 j) {  // 0x8BB54F
  EnableCinematicBgTilemapUpdates();
  return j;
}

uint16 CinematicFunction_Intro_Func21(uint16 k, uint16 j) {  // 0x8BB554
  DisableCinematicBgTilemapUpdates();
  palette_buffer[17] = kPalettes_Intro[17];
  palette_buffer[18] = kPalettes_Intro[18];
  palette_buffer[19] = kPalettes_Intro[19];
  ClearJapaneseTextTiles();
  CinematicFunction_Intro_Func20(addr_word_8BCFFD);
  CinematicFunction_Intro_Func20(addr_word_8BD055);
  TransferJapaneseTextTilesToVram();
  return j;
}

void CinematicFunction_Intro_Func22(uint16 k) {  // 0x8BB585
  if (joypad1_newkeys) {
    int v1 = k >> 1;
    mode7_obj_instr_ptr[v1] = -11355;
    mode7_obj_instr_timer[v1] = 1;
    mode7_obj_preinstr_func[v1] = FUNC16(CinematicFunction_nullsub_116);
    DisableCinematicBgTilemapUpdates();
    ClearJapaneseTextTiles();
    CinematicFunction_Intro_Func20(addr_word_8BD085);
    CinematicFunction_Intro_Func20(addr_word_8BD0E1);
    TransferJapaneseTextTilesToVram();
    cinematic_var16 = 60;
  }
}

uint16 CinematicFunction_Intro_ThenWaitInputSetupBabyMetroid(uint16 k, uint16 j) {  // 0x8BB5B8
  EnableCinematicBgTilemapUpdates();
  cinematic_function = FUNC16(CinematicFunction_Intro_WaitInputSetupBabyMetroid);
  return j;
}

uint16 CinematicFunction_Intro_Func23(uint16 k, uint16 j) {  // 0x8BB5C3
  DisableCinematicBgTilemapUpdates();
  palette_buffer[17] = kPalettes_Intro[17];
  palette_buffer[18] = kPalettes_Intro[18];
  palette_buffer[19] = kPalettes_Intro[19];
  ClearJapaneseTextTiles();
  CinematicFunction_Intro_Func20(addr_word_8BD0F9);
  TransferJapaneseTextTilesToVram();
  return j;
}

void CinematicFunction_Intro_Func24(uint16 k) {  // 0x8BB5EE
  if (joypad1_newkeys) {
    int v1 = k >> 1;
    mode7_obj_instr_ptr[v1] = -11333;
    mode7_obj_instr_timer[v1] = 1;
    mode7_obj_preinstr_func[v1] = FUNC16(CinematicFunction_nullsub_116);
    DisableCinematicBgTilemapUpdates();
    ClearJapaneseTextTiles();
    CinematicFunction_Intro_Func20(addr_word_8BD15D);
    TransferJapaneseTextTilesToVram();
    cinematic_var16 = 60;
  }
}

uint16 CinematicFunction_Intro_Func25(uint16 k, uint16 j) {  // 0x8BB61B
  EnableCinematicBgTilemapUpdates();
  cinematic_function = FUNC16(CinematicFunction_Intro_Func11);
  return j;
}

uint16 CinematicFunction_Intro_Func26(uint16 k, uint16 j) {  // 0x8BB626
  DisableCinematicBgTilemapUpdates();
  palette_buffer[17] = kPalettes_Intro[17];
  palette_buffer[18] = kPalettes_Intro[18];
  palette_buffer[19] = kPalettes_Intro[19];
  ClearJapaneseTextTiles();
  CinematicFunction_Intro_Func20(addr_word_8BD1B9);
  CinematicFunction_Intro_Func20(addr_word_8BD215);
  TransferJapaneseTextTilesToVram();
  return j;
}

void CinematicFunction_Intro_Func27(uint16 k) {  // 0x8BB657
  if (joypad1_newkeys) {
    int v1 = k >> 1;
    mode7_obj_instr_ptr[v1] = -11311;
    mode7_obj_instr_timer[v1] = 1;
    mode7_obj_preinstr_func[v1] = FUNC16(CinematicFunction_nullsub_116);
    DisableCinematicBgTilemapUpdates();
    ClearJapaneseTextTiles();
    CinematicFunction_Intro_Func20(addr_word_8BD259);
    CinematicFunction_Intro_Func20(addr_word_8BD2A5);
    TransferJapaneseTextTilesToVram();
    cinematic_var16 = 60;
  }
}

uint16 CinematicFunction_Intro_Func28(uint16 k, uint16 j) {  // 0x8BB68A
  EnableCinematicBgTilemapUpdates();
  cinematic_function = FUNC16(CinematicFunction_Intro_Func12);
  return j;
}

uint16 CinematicFunction_Intro_Func29(uint16 k, uint16 j) {  // 0x8BB695
  DisableCinematicBgTilemapUpdates();
  palette_buffer[17] = kPalettes_Intro[17];
  palette_buffer[18] = kPalettes_Intro[18];
  palette_buffer[19] = kPalettes_Intro[19];
  ClearJapaneseTextTiles();
  CinematicFunction_Intro_Func20(addr_word_8BD2D5);
  TransferJapaneseTextTilesToVram();
  return j;
}

void CinematicFunction_Intro_Func30(uint16 k) {  // 0x8BB6C0
  if (joypad1_newkeys) {
    int v1 = k >> 1;
    mode7_obj_instr_ptr[v1] = addr_off_8BD3E7;
    mode7_obj_instr_timer[v1] = 1;
    mode7_obj_preinstr_func[v1] = FUNC16(CinematicFunction_nullsub_116);
    DisableCinematicBgTilemapUpdates();
    ClearJapaneseTextTiles();
    CinematicFunction_Intro_Func20(addr_word_8BD30D);
    TransferJapaneseTextTilesToVram();
    cinematic_var16 = 60;
  }
}

uint16 CinematicFunction_Intro_Func31(uint16 k, uint16 j) {  // 0x8BB6ED
  EnableCinematicBgTilemapUpdates();
  cinematic_function = FUNC16(CinematicFunction_Intro_Func15);
  return j;
}

uint16 CinematicFunction_Intro_Func32(uint16 k, uint16 j) {  // 0x8BB6F8
  DisableCinematicBgTilemapUpdates();
  ClearJapaneseTextTiles();
  CinematicFunction_Intro_Func20(addr_word_8BD371);
  TransferJapaneseTextTilesToVram();
  return j;
}

uint16 EnableCinematicBgTilemapUpdates__(uint16 k, uint16 j) {  // 0x8BB70B
  EnableCinematicBgTilemapUpdates();
  return j;
}

void CinematicFunction_Intro_Func33(void) {  // 0x8BB711
  if ((nmi_frame_counter_word & 1) != 0)
    reg_TM &= ~2u;
  else
    reg_TM |= 2u;
}

void CinematicFunction_Intro_Func34(void) {  // 0x8BB72F
  HandleFadeOut();
  if (reg_INIDISP == 0x80) {
    EnableNMI();
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    reg_BG2HOFS = 0;
    reg_BG3HOFS = 0;
    reg_BG2VOFS = 0;
    reg_BG3VOFS = 0;
    cinematic_var5 = 0;
    for (int i = 656; i >= 0; i -= 2)
      *(uint16 *)((char *)&cinematic_var5 + (uint16)i) = 0;
    cinematic_function = FUNC16(CinematicFunction_Intro_Func54);
    RevertButtonConfig();
    samus_max_missiles = 0;
    samus_missiles = 0;
  }
}

void CinematicFunction_Intro_Func35(uint16 j) {  // 0x8BB773
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 56;
  cinematicbg_arr8[v1] = 111;
  cinematicbg_arr9[v1] = 3584;
}

void CinematicFunction_Intro_Func36(uint16 k) {  // 0x8BB786
  CinematicFunction_Intro_Func37(k);
  uint16 v1 = 8;
  while ((projectile_type[v1 >> 1] & 0xFFF) != 256) {
    v1 -= 2;
    if ((v1 & 0x8000u) != 0)
      return;
  }
  if (sign16(projectile_x_pos[v1 >> 1] - 84)) {
    KillProjectile(v1);
    int v2 = k >> 1;
    cinematicspr_arr6[v2] = 8;
    uint16 v3 = cinematicspr_goto_timer[v2] + 1;
    cinematicspr_goto_timer[v2] = v3;
    if (v3 == 4) {
      cinematicspr_goto_timer[v2] = 0;
      cinematicspr_preinstr_func[v2] = FUNC16(CinematicFunction_Intro_Func39);
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCF1B, 0);
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCF1B, 1u);
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCF1B, 2u);
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCF15, 0);
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCF15, 1u);
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCF15, 2u);
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCF15, 3u);
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCF15, 4u);
    }
  }
}

void CinematicFunction_Intro_Func39(uint16 k) {  // 0x8BB80F
  CinematicFunction_Intro_Func37(k);
  CinematicFunction_Intro_Func40();
  int v1 = k >> 1;
  uint16 v2 = cinematicspr_goto_timer[v1] + 1;
  cinematicspr_goto_timer[v1] = v2;
  if (!sign16(v2 - 128)) {
    cinematicspr_instr_timer[v1] = 1;
    cinematicspr_instr_ptr[v1] = addr_off_8BCB19;
  }
}

void CinematicSprPreInstr_B82E(uint16 k) {  // 0x8BB82E
  CinematicFunction_Intro_Func40();
  if (!cinematic_var13) {
    int v1 = k >> 1;
    cinematicspr_instr_timer[v1] = 1;
    cinematicspr_instr_ptr[v1] = addr_off_8BCE53;
    cinematic_var15 = 0;
  }
}

void CinematicFunction_Intro_Func37(uint16 k) {  // 0x8BB846
  uint16 j, v3;
  uint16 v1 = cinematicspr_arr6[k >> 1];
  if (v1) {
    if ((v1 & 1) != 0) {
      for (int i = 30; i >= 0; i -= 2)
        palette_buffer[(i >> 1) + 240] = kPalettes_Intro[(i >> 1) + 240];
      v3 = k;
    } else {
      for (j = 30; (j & 0x8000u) == 0; j -= 2)
        palette_buffer[(j >> 1) + 240] = 0x7FFF;
      v3 = k;
    }
    --cinematicspr_arr6[v3 >> 1];
  }
}

void CinematicFunction_Intro_Func40(void) {  // 0x8BB877
  if ((cinematic_var12 & 1) != 0)
    cinematic_var10 -= 4;
  else
    cinematic_var10 += 4;
}
static const uint16 kCinematicFunction_Intro_Func42_Tab0[4] = { 0x70, 0xc0, 0x80, 0xe8 };
static const uint16 kCinematicFunction_Intro_Func42_Tab1[4] = { 0x50, 0x40, 0x38, 0x58 };
void CinematicFunction_Intro_Func42(uint16 j) {  // 0x8BB896
  uint16 v1 = cinematic_spawn_param;
  int v2 = j >> 1;
  cinematicspr_goto_timer[v2] = cinematic_spawn_param;
  int v3 = v1;
  cinematicbg_arr7[v2] = kCinematicFunction_Intro_Func42_Tab0[v3];
  cinematicbg_arr8[v2] = kCinematicFunction_Intro_Func42_Tab1[v3] - 8;
  cinematicbg_arr9[v2] = 3584;
}

uint16 CinematicSprInstr_Func43(uint16 k, uint16 j) {  // 0x8BB8C5
  int v1 = k >> 1;
  if (cinematicspr_goto_timer[v1])
    cinematicspr_preinstr_func[v1] = FUNC16(CinematicFunction_Intro_Func45);
  else
    cinematicspr_preinstr_func[v1] = FUNC16(CinematicFunction_Intro_Func44);
  return j;
}

void CinematicFunction_Intro_Func44(uint16 k) {  // 0x8BB8D8
  int v1 = k >> 1;
  uint16 v2 = cinematicspr_arr6[v1];
  cinematicspr_arr6[v1] = v2 + 0x8000;
  cinematicbg_arr7[v1] += __CFADD__uint16(v2, 0x8000);
  uint16 v3 = cinematicspr_arr7[v1];
  cinematicspr_arr7[v1] = v3 + 0x8000;
  cinematicbg_arr8[v1] += __CFADD__uint16(v3, 0x8000);
  R18_ = samus_x_pos - 5;
  if (sign16(cinematicbg_arr7[v1] + 8 - (samus_x_pos - 5))) {
    if (cinematicspr_preinstr_func[0] != (uint16)FUNC16(CinematicFunction_Intro_Func39))
      return;
  } else {
    samus_invincibility_timer = 11;
    samus_knockback_timer = 11;
    knockback_x_dir = 1;
  }
  cinematicspr_instr_timer[v1] = 1;
  cinematicspr_instr_ptr[v1] = addr_off_8BCE53;
}
static const uint16 kCinematicFunction_Intro_Func45_Tab0[4] = { 0, 0xffff, 0, 0xffff };
void CinematicFunction_Intro_Func45(uint16 k) {  // 0x8BB93B
  int v1 = k >> 1;
  uint16 v2 = 2 * cinematicspr_goto_timer[v1];
  cinematicbg_arr7[v1] += kCinematicFunction_Intro_Func45_Tab0[v2 >> 1] + __CFADD__uint16(cinematicspr_arr6[v1], 0x8000);
  cinematicspr_arr6[v1] += 0x8000;

  uint16 v3 = cinematicspr_arr7[v1];
  cinematicspr_arr7[v1] = v3 + 0x8000;
  uint16 v4 = __CFADD__uint16(v3, 0x8000) + cinematicbg_arr8[v1];
  cinematicbg_arr8[v1] = v4;
  if (sign16(v4 - 16)
      || !sign16(v4 - 208)
      || cinematicspr_preinstr_func[0] == FUNC16(CinematicFunction_Intro_Func39)) {
    cinematicspr_instr_timer[v1] = 1;
    cinematicspr_instr_ptr[v1] = addr_off_8BCE53;
  }
}
static const uint16 kCinematicFunction_Intro_Func46_Tab0[5] = { 0, 0x10, 0xfff0, 0xfff8, 8 };
static const uint16 kCinematicFunction_Intro_Func46_Tab1[5] = { 0, 0xfff0, 8, 0xfff0, 8 };
static const uint16 kCinematicFunction_Intro_Func46_Tab2[5] = { 1, 0x10, 0x20, 0x30, 0x40 };
void CinematicFunction_Intro_Func46(uint16 j) {  // 0x8BB98D
  uint16 v1 = cinematic_spawn_param;
  int v2 = j >> 1;
  cinematicspr_goto_timer[v2] = cinematic_spawn_param;
  int v3 = v1;
  cinematicbg_arr7[v2] = kCinematicFunction_Intro_Func46_Tab0[v3] + 56;
  cinematicbg_arr8[v2] = kCinematicFunction_Intro_Func46_Tab1[v3] + 111;
  cinematicspr_instr_timer[v2] = kCinematicFunction_Intro_Func46_Tab2[v3];
  cinematicbg_arr9[v2] = 2560;
}
static const uint16 kCinematicFunction_Intro_Func47_Tab0[3] = { 0x10, 0xfff0, 0xfff0 };
static const uint16 kCinematicFunction_Intro_Func47_Tab1[3] = { 0, 4, 0xfff8 };
static const uint16 kCinematicFunction_Intro_Func47_Tab2[3] = { 1, 8, 0x10 };
void CinematicFunction_Intro_Func47(uint16 j) {  // 0x8BB9D4
  uint16 v1 = cinematic_spawn_param;
  int v2 = j >> 1;
  cinematicspr_goto_timer[v2] = cinematic_spawn_param;
  int v3 = v1;
  cinematicbg_arr7[v2] = kCinematicFunction_Intro_Func47_Tab0[v3] + 56;
  cinematicbg_arr8[v2] = kCinematicFunction_Intro_Func47_Tab1[v3] + 111;
  cinematicspr_instr_timer[v2] = kCinematicFunction_Intro_Func47_Tab2[v3];
  cinematicbg_arr9[v2] = 2560;
}

void CinematicFunction_Intro_Func48(uint16 k) {  // 0x8BBA0F
  if (!cinematic_var13) {
    int v1 = k >> 1;
    cinematicspr_instr_timer[v1] = 1;
    cinematicspr_instr_ptr[v1] = addr_off_8BCE53;
  }
}

uint16 CinematicSprInstr_SpawnIntroRinkas01(uint16 k, uint16 j) {  // 0x8BBA21
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCF21, 0);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCF21, 1u);
  return j;
}

uint16 CinematicSprInstr_SpawnIntroRinkas23(uint16 k, uint16 j) {  // 0x8BBA36
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCF21, 2u);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCF21, 3u);
  return j;
}

void CinematicFunction_Intro_Func49(uint16 j) {  // 0x8BBA4B
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 112;
  cinematicbg_arr8[v1] = 155;
  cinematicbg_arr9[v1] = 3584;
}

void CinematicFunction_Intro_Func50(uint16 k) {  // 0x8BBA5E
  int v1 = k >> 1;
  if (!sign16(cinematicspr_instr_ptr[v1 + 1] + 0x3487)) {
    cinematicspr_preinstr_func[v1] = FUNC16(CinematicFunction_Intro_Func51);
    cinematic_var14 = 0;
    cinematic_var17 = 0;
  }
}

void CinematicFunction_Intro_Func51(uint16 k) {  // 0x8BBA73
  int16 v2;

  int v1 = k >> 1;
  if (cinematicbg_arr8[v1] == 145) {
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEF1, 0);
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEF1, 1u);
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEF1, 2u);
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEF1, 3u);
    QueueSfx3_Max6(0x23u);
  }
  if ((int16)(samus_y_pos - 32 - cinematicbg_arr8[v1]) < 0) {
    if (!sign16(cinematic_var17 + 543))
      cinematic_var17 -= 32;
  } else if (sign16(cinematic_var17 - 544)) {
    cinematic_var17 += 32;
  }
  LOBYTE(v2) = HIBYTE(cinematic_var17);
  HIBYTE(v2) = cinematic_var17;
  R20_ = v2 & 0xFF00;
  uint16 v3 = HIBYTE(cinematic_var17);
  if ((cinematic_var17 & 0x8000u) != 0)
    v3 = HIBYTE(cinematic_var17) | 0xFF00;
  R18_ = v3;
  uint16 v4 = cinematicspr_arr7[v1];
  bool v5 = __CFADD__uint16(R20_, v4);
  cinematicspr_arr7[v1] = R20_ + v4;
  cinematicbg_arr8[v1] += R18_ + v5;
  if ((cinematic_var17 & 0x8000u) == 0) {
    cinematic_var14 = 128;
    cinematicspr_preinstr_func[v1] = FUNC16(CinematicFunction_Intro_Func52);
  }
}

void CinematicFunction_Intro_Func52(uint16 k) {  // 0x8BBB0D
  bool v1 = (--cinematic_var14 & 0x8000u) != 0;
  if (!cinematic_var14 || v1) {
    int v2 = k >> 1;
    cinematicspr_preinstr_func[v2] = FUNC16(CinematicFunction_Intro_Func53);
    cinematic_var14 = 0;
    cinematic_var17 = 0;
    cinematicspr_goto_timer[v2] = 0;
  }
}

void CinematicFunction_Intro_Func53(uint16 k) {  // 0x8BBB24
  int16 v4;
  int16 v5;
  int16 v8;

  if (cinematic_var13) {
    int v2 = k >> 1;
    if (sign16(cinematicspr_goto_timer[v2] - 128)) {
      uint16 v3 = cinematicspr_goto_timer[v2] + 1;
      cinematicspr_goto_timer[v2] = v3;
      if ((v3 & 0x3F) == 0)
        QueueSfx3_Max6(0x23u);
    }
    if ((int16)(samus_x_pos - cinematicbg_arr7[v2]) < 0) {
      if (!sign16(cinematic_var14 + 639))
        cinematic_var14 -= 32;
    } else if (sign16(cinematic_var14 - 640)) {
      cinematic_var14 += 32;
    }
    LOBYTE(v4) = HIBYTE(cinematic_var14);
    HIBYTE(v4) = cinematic_var14;
    R20_ = v4 & 0xFF00;
    v5 = HIBYTE(cinematic_var14);
    if ((cinematic_var14 & 0x8000u) != 0)
      v5 = HIBYTE(cinematic_var14) | 0xFF00;
    R18_ = v5;
    if (v5 >= 0)
      cinematic_var15 = -1;
    else
      cinematic_var15 = 1;
    uint16 v6 = cinematicspr_arr6[v2];
    bool v7 = __CFADD__uint16(R20_, v6);
    cinematicspr_arr6[v2] = R20_ + v6;
    cinematicbg_arr7[v2] += R18_ + v7;
    if ((int16)(samus_y_pos - 8 - cinematicbg_arr8[v2]) < 0) {
      if (!sign16(cinematic_var17 + 543))
        cinematic_var17 -= 32;
    } else if (sign16(cinematic_var17 - 544)) {
      cinematic_var17 += 32;
    }
    LOBYTE(v8) = HIBYTE(cinematic_var17);
    HIBYTE(v8) = cinematic_var17;
    R20_ = v8 & 0xFF00;
    uint16 v9 = HIBYTE(cinematic_var17);
    if ((cinematic_var17 & 0x8000u) != 0)
      v9 = HIBYTE(cinematic_var17) | 0xFF00;
    R18_ = v9;
    uint16 v10 = cinematicspr_arr7[v2];
    v7 = __CFADD__uint16(R20_, v10);
    cinematicspr_arr7[v2] = R20_ + v10;
    cinematicbg_arr8[v2] += R18_ + v7;
  } else {
    int v1 = k >> 1;
    cinematicspr_instr_timer[v1] = 1;
    cinematicspr_instr_ptr[v1] = addr_off_8BCE53;
    cinematic_var15 = 0;
  }
}

void ResetButtonAssignmentsToDefault(void) {  // 0x8BBC08
  button_config_up = kButton_Up;
  button_config_down = kButton_Down;
  button_config_left = kButton_Left;
  button_config_right = kButton_Right;
  grapple_beam_tmpD82 = button_config_shoot_x;
  grapple_beam_tmpD84 = button_config_jump_a;
  grapple_beam_y_quarter_subvel = button_config_run_b;
  grapple_beam_y_quarter_vel = button_config_itemcancel_y;
  grapple_beam_tmpD8A = button_config_itemswitch;
  grapple_beam_varD8C = button_config_aim_down_L;
  grapple_beam_varD8E = button_config_aim_up_R;
  button_config_shoot_x = kButton_X;
  button_config_jump_a = kButton_A;
  button_config_run_b = kButton_B;
  button_config_itemcancel_y = kButton_Y;
  button_config_itemswitch = kButton_Select;
  button_config_aim_down_L = kButton_L;
  button_config_aim_up_R = kButton_R;
}

void RevertButtonConfig(void) {  // 0x8BBC75
  button_config_shoot_x = grapple_beam_tmpD82;
  button_config_jump_a = grapple_beam_tmpD84;
  button_config_run_b = grapple_beam_y_quarter_subvel;
  button_config_itemcancel_y = grapple_beam_y_quarter_vel;
  button_config_itemswitch = grapple_beam_tmpD8A;
  button_config_aim_down_L = grapple_beam_varD8C;
  button_config_aim_up_R = grapple_beam_varD8E;
}

void CinematicFunction_Intro_Func54(void) {  // 0x8BBCA0
  int16 v1;

  SetupPpu_3_Mode7();
  ClearCinematicSprites();
  cur_irq_handler = 0;
  irqhandler_next_handler = 0;
  uint16 v0 = 0;
  do {
    palette_buffer[v0 >> 1] = kPalettes_Intro2[v0 >> 1];
    v0 += 2;
  } while ((int16)(v0 - 512) < 0);
  static const DecompressToParams unk_8BBCCA = { LONGPTR(0x7f0000) };
  static const DecompressToParams unk_8BBCDB = { LONGPTR(0x7f4000) };
  static const DecompressToParams unk_8BBCEC = { LONGPTR(0x7f5000) };
  mov24(&decompress_src, 0x95A82F);
  DecompressToMem_IpArg(&unk_8BBCCA);
  mov24(&decompress_src, 0x96FE69u);
  DecompressToMem_IpArg(&unk_8BBCDB);
  mov24(&decompress_src, 0x96D10Au);
  DecompressToMem_IpArg(&unk_8BBCEC);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BBD04 = { 1, 0, 0x19, LONGPTR(0x7f0000), 0x4000 };
  SetupDmaTransfer(&unk_8BBD04);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMAIN, 0);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0);
  v1 = 0x4000;
  do {
    WriteReg(VMDATAL, 0x8Cu);
    --v1;
  } while (v1);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0);
  WriteReg(VMAIN, 0);
  static const StartDmaCopy unk_8BBD3C = { 1, 0, 0x18, LONGPTR(0x7f4000), 0x0300 };
  SetupDmaTransfer(&unk_8BBD3C);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x60u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BBD5C = { 1, 1, 0x18, LONGPTR(0x7f5000), 0x4000 };
  SetupDmaTransfer(&unk_8BBD5C);
  WriteReg(MDMAEN, 2u);
  WriteRegWord(M7A, 0x100u);
  reg_M7A = 256;
  WriteRegWord(M7B, 0);
  reg_M7B = 0;
  WriteRegWord(M7C, 0);
  reg_M7C = 0;
  WriteRegWord(M7D, 0x100u);
  reg_M7D = 256;
  WriteRegWord(M7X, 0x38u);
  reg_M7X = 56;
  WriteRegWord(M7Y, 0x18u);
  reg_M7Y = 24;
  cinematic_var8 = -72;
  cinematic_var10 = -104;
  cinematic_var5 = 224;
  cinematic_var6 = 512;
  ClearPaletteFXObjects();
  EnablePaletteFx();
  ClearCinematicBgObjects(0);
  cinematicbg_var3 = 23552;
  EnableNMI();
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCF0F, 0);
  cinematic_function = FUNC16(CinematicFunction_Intro_Func55);
  QueueMusic_Delayed8(0xFF2Du);
  QueueMusic_DelayedY(5u, 0xEu);
}

void CinematicFunction_Intro_Func55(void) {  // 0x8BBDE4
  if (!(HasQueuedMusic() & 1)) {
    reg_INIDISP = 15;
    cinematic_function = FUNC16(CinematicFunction_Intro_Func56);
  }
}

void CinematicFunction_Intro_Func56(void) {  // 0x8BBDF9
  if (sign16(cinematic_var6 - 32)) {
    reg_CGWSEL = 0;
    reg_CGADSUB = 49;
    reg_COLDATA[0] = 63;
    reg_COLDATA[1] = 95;
    reg_COLDATA[2] = -97;
    ClearCinematicSprites();
    QueueMode7Transfers(0x8b, addr_kCinematicFunction_Intro_Func56_M7);
    cinematic_var8 = -32;
    cinematic_var10 = -128;
    cinematic_var5 = 32;
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCF39, 0);
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCE85, 0);
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCE8B, 0);
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCE91, 1u);
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCF0F, 1u);
    cinematic_function = FUNC16(CinematicFunction_Intro_Func67);
    SpawnPalfxObject(addr_kPalfx_E1A8);
    SpawnPalfxObject(addr_kPalfx_E1AC);
  } else {
    cinematic_var6 -= 16;
  }
}

void CinematicFunction_Intro_Func57(uint16 j) {  // 0x8BBE7E
  int v1 = j >> 1;
  if (cinematic_spawn_param) {
    cinematicspr_preinstr_func[v1] = FUNC16(CinematicFunction_Intro_Func66);
    cinematicbg_arr7[v1] = -32;
  } else {
    cinematicspr_goto_timer[v1] = -1024;
    cinematicbg_arr7[v1] = 112;
  }
  cinematicbg_arr8[v1] = 87;
  cinematicbg_arr9[v1] = 2048;
}

void CinematicFunction_Intro_Func58(uint16 k) {  // 0x8BBEB5
  char v3; // t0
  unsigned int v8; // kr08_4
  unsigned int v9; // kr0C_4
  uint16 v4;

  if (cinematic_function == (uint16)FUNC16(CinematicFunction_Intro_Func56)) {
    int v1 = k >> 1;
    uint16 v2 = cinematicspr_goto_timer[v1] + 128;
    cinematicspr_goto_timer[v1] = v2;
    v3 = v2;
    LOBYTE(v4) = HIBYTE(v2);
    HIBYTE(v4) = v3;
    R20_ = v4 & 0xFF00;
    v4 = (uint8)v4;
    if ((v4 & 0x80) != 0)
      v4 |= 0xFF00u;
    R18_ = v4;
    uint16 v5 = cinematicspr_arr7[v1];
    bool v6 = __CFADD__uint16(R20_, v5);
    cinematicspr_arr7[v1] = R20_ + v5;
    cinematicbg_arr8[v1] += R18_ + v6;
    uint16 v7 = cinematicspr_arr6[v1];
    v6 = __CFADD__uint16(R20_, v7);
    cinematicspr_arr6[v1] = R20_ + v7;
    cinematicbg_arr7[v1] += R18_ + v6;
    v8 = __PAIR32__(R18_, R20_) + __PAIR32__(cinematic_var8, cinematic_var7);
    cinematic_var8 = HIWORD(v8);
    cinematic_var7 = v8;
    v9 = __PAIR32__(R18_, R20_) + __PAIR32__(cinematic_var10, cinematic_var9);
    cinematic_var10 = HIWORD(v9);
    cinematic_var9 = v9;
  }
}

void CinematicFunction_Intro_Func59(uint16 j) {  // 0x8BBF22
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 80;
  cinematicbg_arr8[v1] = 159;
  cinematicbg_arr9[v1] = 2048;
}

void CinematicFunction_Intro_Func60(uint16 k) {  // 0x8BBF35
  int v1 = k >> 1;
  uint16 v2 = cinematicspr_arr6[v1];
  cinematicspr_arr6[v1] = v2 + 0x4000;
  cinematicbg_arr7[v1] = (__CFADD__uint16(v2, 0x4000) + cinematicbg_arr7[v1]) & 0x1FF;
}

void CinematicFunction_Intro_Func61(uint16 j) {  // 0x8BBF4C
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 116;
  cinematicbg_arr8[v1] = 160;
  cinematicbg_arr9[v1] = 3072;
}

void CinematicFunction_Intro_Func62(uint16 k) {  // 0x8BBF5F
  int v1 = k >> 1;
  uint16 v2 = cinematicspr_arr6[v1];
  cinematicspr_arr6[v1] = v2 + 4096;
  cinematicbg_arr7[v1] = (__CFADD__uint16(v2, 4096) + cinematicbg_arr7[v1]) & 0x1FF;
}

void CinematicFunction_Intro_Func63(uint16 j) {  // 0x8BBF76
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 128;
  cinematicbg_arr8[v1] = 96;
  cinematicbg_arr9[v1] = 2048;
}

void CinematicFunction_Intro_Func64(uint16 k) {  // 0x8BBF89
  int v1 = k >> 1;
  uint16 v2 = cinematicspr_arr6[v1];
  cinematicspr_arr6[v1] = v2 + 2048;
  cinematicbg_arr7[v1] = (__CFADD__uint16(v2, 2048) + cinematicbg_arr7[v1]) & 0x1FF;
}

void CinematicFunction_Intro_Func65(uint16 j) {  // 0x8BBFA0
  if (cinematic_spawn_param) {
    cinematicbg_arr7[j >> 1] = 224;
  } else {
    int v1 = j >> 1;
    cinematicbg_arr7[v1] = 112;
    cinematicspr_preinstr_func[v1] = FUNC16(CinematicSprPreInstr_nullsub_300);
  }
  int v2 = j >> 1;
  cinematicbg_arr8[v2] = 87;
  cinematicbg_arr9[v2] = 2048;
}

void CinematicFunction_Intro_Func66(uint16 k) {  // 0x8BBFC6
  int v1 = k >> 1;
  uint16 v2 = cinematicspr_arr6[v1];
  cinematicspr_arr6[v1] = v2 - 0x2000;
  cinematicbg_arr7[v1] = (__PAIR32__(cinematicbg_arr7[v1], v2) - 0x2000) >> 16;
}

void CinematicFunction_Intro_Func67(void) {  // 0x8BBFDA
  unsigned int v0; // kr00_4

  if (sign8(--reg_COLDATA[0] - 32))
    reg_COLDATA[0] = 32;
  if (sign8(--reg_COLDATA[1] - 64))
    reg_COLDATA[1] = 64;
  if (sign8(--reg_COLDATA[2] + 0x80))
    reg_COLDATA[2] = 0x80;
  v0 = __PAIR32__(cinematic_var8, cinematic_var7) - 0x2000;
  cinematic_var8 = HIWORD(v0);
  cinematic_var7 = v0;
  if (sign16(cinematic_var6 - 3072)) {
    cinematic_var6 += 16;
  } else if (sign16(cinematic_var6 - 0x2000)) {
    cinematic_var6 += 32;
  } else {
    reg_TM = 17;
    reg_BGMODE = 1;
    reg_M7SEL = 0;
    reg_BG1SC = 92;
    reg_BG12NBA = 6;
    cinematic_var6 = 256;
    cinematic_var5 = 0;
    cinematic_var7 = 0;
    cinematic_var8 = 0;
    cinematic_var9 = 0;
    cinematic_var10 = 0;
    EnableCinematicBgObjects();
    EnableCinematicBgTilemapUpdates();
    SpawnCinematicBgObject(addr_kCinematicBgObjectDef_8BCF69, 0x5C00u);
    cinematic_function = FUNC16(nullsub_120);
  }
}

void CinematicFunction_Intro_Func68(uint16 j) {  // 0x8BC083
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 124;
  cinematicbg_arr8[v1] = 186;
  cinematicbg_arr9[v1] = 0;
}

uint16 CinematicCommonInstr_Func69(uint16 k, uint16 j) {  // 0x8BC096
  if (!japanese_text_flag)
    j += 6;
  return j;
}

uint16 CinematicCommonInstr_Func70(uint16 k, uint16 j) {  // 0x8BC0A2
  cinematic_function = FUNC16(CinematicFunction_Intro_Func72);
  screen_fade_delay = 1;
  screen_fade_counter = 1;
  return j;
}

void CinematicFunction_Intro_Func71(uint16 j) {  // 0x8BC0B2
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 124;
  cinematicbg_arr8[v1] = 204;
  cinematicbg_arr9[v1] = 0;
}

void CinematicFunction_Intro_Func72(void) {  // 0x8BC0C5
  HandleFadeOut();
  if (reg_INIDISP == 0x80) {
    EnableNMI();
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    reg_BG2HOFS = 0;
    reg_BG3HOFS = 0;
    reg_BG2VOFS = 0;
    reg_BG3VOFS = 0;
    cinematic_var5 = 0;
    for (int i = 656; i >= 0; i -= 2)
      *(uint16 *)((char *)&cinematic_var5 + (uint16)i) = 0;
    cinematic_function = FUNC16(CinematicFunction_Intro_Func73);
  }
}

void CinematicFunction_Intro_Func73(void) {  // 0x8BC100
  loading_game_state = kGameState_31_SetUpNewGame;
  game_state = kGameState_31_SetUpNewGame;
  area_index = 6;
  load_station_index = 0;
  SaveToSram(selected_save_slot);
}

void CinematicFunctionBlackoutFromCeres(void) {  // 0x8BC11B
  int16 j;
  int16 v3;

  SetupPpu_3_Mode7();
  for (j = 656; j >= 0; j -= 2)
    *(uint16 *)((char *)&cinematic_var5 + (uint16)j) = 0;
  door_def_ptr = 0;
  layer1_x_pos = 0;
  layer1_y_pos = 0;
  uint16 v2 = 0;
  do {
    palette_buffer[v2 >> 1] = kPalettes_Intro2[v2 >> 1];
    v2 += 2;
  } while ((int16)(v2 - 512) < 0);
  static const DecompressToParams unk_8BC162 = { LONGPTR(0x7f0000) };
  static const DecompressToParams unk_8BC173 = { LONGPTR(0x7f4000) };
  static const DecompressToParams unk_8BC184 = { LONGPTR(0x7f5000) };
  mov24(&decompress_src, 0x95A82F);
  DecompressToMem_IpArg(&unk_8BC162);
  mov24(&decompress_src, 0x96FE69);
  DecompressToMem_IpArg(&unk_8BC173);
  mov24(&decompress_src, 0x96D10A);
  DecompressToMem_IpArg(&unk_8BC184);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BC19C = { 1, 0, 0x19, LONGPTR(0x7f0000), 0x4000 };
  SetupDmaTransfer(&unk_8BC19C);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMAIN, 0);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0);
  v3 = 0x4000;
  do {
    WriteReg(VMDATAL, 0x8Cu);
    --v3;
  } while (v3);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0);
  WriteReg(VMAIN, 0);
  static const StartDmaCopy unk_8BC1D4 = { 1, 0, 0x18, LONGPTR(0x7f4600), 0x0600 };
  SetupDmaTransfer(&unk_8BC1D4);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x60u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BC1F4 = { 1, 1, 0x18, LONGPTR(0x7f5000), 0x4000 };
  SetupDmaTransfer(&unk_8BC1F4);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x60u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BC214 = { 1, 1, 0x18, LONGPTR(0x9ad200), 0x1a00 };
  SetupDmaTransfer(&unk_8BC214);
  WriteReg(MDMAEN, 2u);
  WriteRegWord(M7A, 0x100u);
  reg_M7A = 256;
  WriteRegWord(M7B, 0);
  reg_M7B = 0;
  WriteRegWord(M7C, 0);
  reg_M7C = 0;
  WriteRegWord(M7D, 0x100u);
  reg_M7D = 256;
  WriteRegWord(M7X, 0x34u);
  reg_M7X = 52;
  WriteRegWord(M7Y, 0x30u);
  reg_M7Y = 48;
  cinematic_var8 = -44;
  cinematic_var10 = -112;
  cinematic_var5 = 0;
  cinematic_var6 = 256;
  DisableHdmaObjects();
  WaitUntilEndOfVblankAndClearHdma();
  ClearPaletteFXObjects();
  EnablePaletteFx();
  ClearCinematicBgObjects(0x2f);
  cinematicbg_var3 = 0;
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCE7F, 0);
  R18_ = 2;
  SpawnCinematicSpriteObjectToR18(addr_kCinematicSpriteObjectDef_8BCE8B, 2u);
  R18_ = 0;
  SpawnCinematicSpriteObjectToR18(addr_kCinematicSpriteObjectDef_8BCE91, 0);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCF33, 0);
  SpawnPalfxObject(addr_kPalfx_E1B8);
  EnableNMI();
  screen_fade_delay = 1;
  screen_fade_counter = 1;
  cinematic_function = FUNC16(CinematicFunction_Intro_Func74);
  QueueMusic_Delayed8(0);
  QueueMusic_Delayed8(0xFF2Du);
  if (game_state == kGameState_37_CeresGoesBoomWithSamus)
    QueueMusic_DelayedY(8u, 0xEu);
  else
    QueueMusic_DelayedY(7u, 0xEu);
}

void CinematicFunction_Intro_Func74(void) {  // 0x8BC2E4
  if (!(HasQueuedMusic() & 1))
    cinematic_function = FUNC16(CinematicFunction_Intro_Func75);
}

void CinematicFunction_Intro_Func75(void) {  // 0x8BC2F1
  uint16 v0 = (__PAIR32__(cinematic_var10, cinematic_var9) + 4096) >> 16;
  cinematic_var9 += 4096;
  cinematic_var10 = v0;
  uint16 v1 = (__PAIR32__(cinematic_var8, cinematic_var7) - 0x4000) >> 16;
  cinematic_var7 -= 0x4000;
  cinematic_var8 = v1;
  ++cinematic_var6;
  HandleFadeIn();
  if (reg_INIDISP == 15) {
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    cinematic_function = FUNC16(CinematicFunction_Intro_Func76);
    cinematic_var4 = 1;
    cinematic_var13 = 0;
  }
}

void CinematicFunction_Intro_Func76(void) {  // 0x8BC345
  uint16 v0 = (__PAIR32__(cinematic_var10, cinematic_var9) + 4096) >> 16;
  cinematic_var9 += 4096;
  cinematic_var10 = v0;
  uint16 v1 = (__PAIR32__(cinematic_var8, cinematic_var7) - 0x4000) >> 16;
  cinematic_var7 -= 0x4000;
  cinematic_var8 = v1;
  if (sign16(cinematic_var6 - 640)) {
    ++cinematic_var6;
  } else {
    power_bomb_explosion_x_pos = reg_M7X - cinematic_var8;
    power_bomb_explosion_y_pos = reg_M7Y - cinematic_var10;
    EnableHdmaObjects();
    SpawnPowerBombExplosion();
    reg_CGWSEL = 16;
    reg_CGADSUB = 55;
    cinematic_var6 = 768;
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCF2D, 0);
    if (game_state == 37)
      QueueMode7Transfers(0x8b, addr_kCinematicFunction_Intro_Func76_M7_2);
    else
      QueueMode7Transfers(0x8b, addr_kCinematicFunction_Intro_Func76_M7_0);
    QueueMode7Transfers(0x8b, addr_kCinematicFunction_Intro_Func76_M7_1);
    cinematic_function = FUNC16(CinematicFunction_Intro_Func77);
  }
}

uint16 CinematicSprInstr_SpawnCeresExplosions1(uint16 k, uint16 j) {  // 0x8BC404
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEBB, 0);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEBB, 1u);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEBB, 2u);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEBB, 3u);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEBB, 4u);
  return j;
}
static const uint16 kCinematicFunction_Intro_Func78_Tab0[5] = { 1, 16, 32, 48, 64 };
static const int16 kCinematicFunction_Intro_Func78_Tab1[5] = { 16, -16, 16, -16, 0 };
static const int16 kCinematicFunction_Intro_Func78_Tab2[5] = { -16, 16, 16, -16, 0 };
void CinematicFunction_Intro_Func78(uint16 j) {  // 0x8BC434
  uint16 v1 = cinematic_spawn_param;
  int v2 = j >> 1;
  cinematicspr_goto_timer[v2] = cinematic_spawn_param;
  int v3 = v1;
  cinematicspr_instr_timer[v2] = kCinematicFunction_Intro_Func78_Tab0[v3];
  cinematicbg_arr7[v2] = kCinematicFunction_Intro_Func78_Tab1[v3] + reg_M7X - cinematic_var8;
  cinematicbg_arr8[v2] = kCinematicFunction_Intro_Func78_Tab2[v3] + reg_M7Y - cinematic_var10;
  cinematicbg_arr9[v2] = 2560;
}

void CinematicSprPreInstr_C489(uint16 k) {  // 0x8BC489
  if (cinematic_function == (uint16)FUNC16(CinematicFunction_Intro_Func77)) {
    cinematicspr_preinstr_func[k >> 1] = FUNC16(CinematicFunction_nullsub_116);
  } else {
    bool v1 = (--cinematic_var4 & 0x8000u) != 0;
    if (!cinematic_var4 || v1) {
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEC1, cinematic_var13);
      cinematic_var4 = 12;
      cinematic_var13 = ((uint8)cinematic_var13 + 1) & 7;
    }
  }
}
static const int16 g_word_8BC4EB[16] = {  // 0x8BC4B9
   14, -8,  8,  12, -16, 12, -8, -14, 0, 0, 16, 14,
  -12,  4, -8, -16,
};
void CinematicFunction_Intro_Func80(uint16 j) {

  uint16 v1 = cinematic_spawn_param;
  int v2 = j >> 1;
  cinematicspr_goto_timer[v2] = cinematic_spawn_param;
  int v3 = (uint16)(4 * v1) >> 1;
  cinematicbg_arr7[v2] = g_word_8BC4EB[v3] + reg_M7X - cinematic_var8;
  cinematicbg_arr8[v2] = g_word_8BC4EB[v3 + 1] + reg_M7Y - cinematic_var10;
  cinematicbg_arr9[v2] = 2560;
}

uint16 CinematicSprInstr_SpawnCeresExplosions3(uint16 k, uint16 j) {  // 0x8BC50C
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEC7, 0);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEC7, 1u);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEC7, 2u);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEC7, 3u);
  return j;
}
static const int16 kCinematicFunction_Intro_Func81_Tab0[4] = { 1, 4, 8, 16 };
static const int16 kCinematicFunction_Intro_Func81_Tab1[4] = { 8, 12, -8, -12 };
static const int16 kCinematicFunction_Intro_Func81_Tab2[4] = { -4, 8, -10, 12 };
void CinematicFunction_Intro_Func81(uint16 j) {  // 0x8BC533
  uint16 v1 = cinematic_spawn_param;
  int v2 = j >> 1;
  cinematicspr_goto_timer[v2] = cinematic_spawn_param;
  int v3 = v1;
  cinematicspr_instr_timer[v2] = kCinematicFunction_Intro_Func81_Tab0[v3];
  cinematicbg_arr7[v2] = kCinematicFunction_Intro_Func81_Tab1[v3] + reg_M7X - cinematic_var8;
  cinematicbg_arr8[v2] = kCinematicFunction_Intro_Func81_Tab2[v3] + reg_M7Y - cinematic_var10;
  cinematicbg_arr9[v2] = 2560;
}

void CinematicFunction_Intro_Func82(uint16 k) {  // 0x8BC582
  int v1 = k >> 1;
  uint16 v2 = cinematicspr_arr7[v1];
  cinematicspr_arr7[v1] = v2 - 4096;
  cinematicbg_arr8[v1] = (__PAIR32__(cinematicbg_arr8[v1], v2) - 4096) >> 16;
  uint16 v3 = cinematicspr_arr6[v1];
  cinematicspr_arr6[v1] = v3 + 0x4000;
  cinematicbg_arr7[v1] += __CFADD__uint16(v3, 0x4000);
}

void CinematicFunction_Intro_Func83(uint16 j) {  // 0x8BC5A9
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = reg_M7X - cinematic_var8;
  cinematicbg_arr8[v1] = reg_M7Y - cinematic_var10;
  cinematicbg_arr9[v1] = 2560;
}

void CinematicFunction_Intro_Func77(void) {  // 0x8BC5CA
  reg_CGWSEL = 16;
  reg_CGADSUB = 55;
  cinematic_var8 += 2;
  cinematic_var5 = (uint8)(cinematic_var5 - 1);
  if (sign16(cinematic_var6 - 16)) {
    cinematic_function = FUNC16(CinematicFunction_Intro_Func84);
    cinematic_var4 = 192;
  } else {
    cinematic_var6 -= 16;
  }
}

void CinematicFunction_Intro_Func84(void) {  // 0x8BC610
  bool v0 = (--cinematic_var4 & 0x8000u) != 0;
  if (!cinematic_var4 || v0) {
    screen_fade_delay = 1;
    screen_fade_counter = 1;
    cinematic_function = FUNC16(CinematicFunction_Intro_Func85);
  }
}

void CinematicFunction_Intro_Func85(void) {  // 0x8BC627
  HandleFadeOut();
  if (reg_INIDISP == 0x80) {
    EnableNMI();
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    if (game_state == kGameState_37_CeresGoesBoomWithSamus) {
      QueueMusic_Delayed8(0);
      hud_item_index = 0;
      samus_auto_cancel_hud_item_index = 0;
      samus_invincibility_timer = 0;
      samus_knockback_timer = 0;
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
      game_options_screen_index = 0;
      menu_index = 0;
      for (int i = 254; i >= 0; i -= 2)
        cinematicbg_arr7[(i >> 1) + 8] = 0;
      game_state = kGameState_25_SamusNoHealth;
    } else {
      cinematic_function = FUNC16(CinematicFunction_Intro_Func86);
    }
  }
}

void CinematicFunction_Intro_Func86(void) {  // 0x8BC699
  static const DecompressToParams unk_8BC6BB = { LONGPTR(0x7f9000) };
  static const DecompressToParams unk_8BC6CC = { LONGPTR(0x7f5000) };
  SetupPpu_4_Mode1();
  for (int i = 656; i >= 0; i -= 2)
    *(uint16 *)((char *)&cinematic_var5 + (uint16)i) = 0;
  mov24(&decompress_src, 0x978ADB);
  DecompressToMem_IpArg(&unk_8BC6BB);
  mov24(&decompress_src, 0x96EC76);
  DecompressToMem_IpArg(&unk_8BC6CC);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0);
  WriteReg(VMAIN, 0);
  static const StartDmaCopy unk_8BC6E4 = { 1, 0, 0x18, LONGPTR(0x7f4300), 0x0300 };
  SetupDmaTransfer(&unk_8BC6E4);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x5Cu);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BC704 = { 1, 1, 0x18, LONGPTR(0x7f9000), 0x0800 };
  SetupDmaTransfer(&unk_8BC704);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x60u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BC724 = { 1, 1, 0x18, LONGPTR(0x7f5000), 0x4000 };
  SetupDmaTransfer(&unk_8BC724);
  WriteReg(MDMAEN, 2u);
  WriteRegWord(M7A, 0x100u);
  reg_M7A = 256;
  WriteRegWord(M7B, 0);
  reg_M7B = 0;
  WriteRegWord(M7C, 0);
  reg_M7C = 0;
  WriteRegWord(M7D, 0x100u);
  reg_M7D = 256;
  WriteRegWord(M7X, 0x38u);
  reg_M7X = 56;
  WriteRegWord(M7Y, 0x18u);
  reg_M7Y = 24;
  cinematic_var8 = 0;
  cinematic_var10 = 0;
  cinematic_var5 = 0;
  cinematic_var6 = 256;
  DisableHdmaObjects();
  WaitUntilEndOfVblankAndClearHdma();
  ClearPaletteFXObjects();
  EnablePaletteFx();
  reg_MOSAIC = -127;
  SpawnPalfxObject(addr_kPalfx_E1A8);
  cinematic_function = FUNC16(CinematicFunction_Intro_Func87);
  EnableNMI();
  screen_fade_delay = 1;
  screen_fade_counter = 1;
}

void CinematicFunction_Intro_Func87(void) {  // 0x8BC79C
  if ((cinematic_var12 & 3) == 0)
    reg_MOSAIC -= 16;
  HandleFadeIn();
  if (reg_INIDISP == 15) {
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    cinematic_function = FUNC16(CinematicFunction_Intro_Func88);
  }
}

void CinematicFunction_Intro_Func88(void) {  // 0x8BC7CA
  if ((cinematic_var12 & 3) == 0) {
    reg_MOSAIC -= 16;
    if ((reg_MOSAIC & 0xF0) == 0) {
      reg_BGMODE = 7;
      reg_M7SEL = 0x80;
      reg_BG1SC = 0;
      reg_BG12NBA = 0;
      reg_TM = 17;
      cinematic_var8 = 128;
      cinematic_var10 = -104;
      cinematic_var5 = 32;
      cinematic_var6 = 256;
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEA3, 0);
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEF7, 0);
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEFD, 0);
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCF03, 0);
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCF09, 0);
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEAF, 0);
      cinematic_function = FUNC16(nullsub_120);
    }
  }
}

void CinematicFunction_Intro_Func89(uint16 j) {  // 0x8BC83B
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 136;
  cinematicbg_arr8[v1] = 111;
  cinematicbg_arr9[v1] = 3584;
}

void CinematicFunction_Intro_Func90(uint16 k) {  // 0x8BC84E
  if (cinematic_function == (uint16)FUNC16(nullsub_124))
    cinematicspr_preinstr_func[k >> 1] = FUNC16(CinematicFunction_Intro_Func91);
}

void CinematicFunction_Intro_Func91(uint16 k) {  // 0x8BC85D
  char v3; // t0

  int v1 = k >> 1;
  uint16 v2 = cinematicspr_goto_timer[v1] + 64;
  cinematicspr_goto_timer[v1] = v2;
  v3 = v2;
  LOBYTE(v2) = HIBYTE(v2);
  HIBYTE(v2) = v3;
  R20_ = v2 & 0xFF00;
  R18_ = (uint8)v2;
  uint16 v4 = cinematicspr_arr7[v1];
  bool v5 = v4 < R20_;
  cinematicspr_arr7[v1] = v4 - R20_;
  uint16 v6 = cinematicbg_arr8[v1] - (v5 + R18_);
  cinematicbg_arr8[v1] = v6;
  if (sign16(v6 + 128)) {
    cinematicspr_instr_timer[v1] = 1;
    cinematicspr_instr_ptr[v1] = addr_off_8BCE53;
  }
}

void CinematicFunction_Intro_Func92(uint16 j) {  // 0x8BC897
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 128;
  cinematicbg_arr8[v1] = 127;
  cinematicbg_arr9[v1] = 2048;
}

void CinematicFunction_Intro_Func93(uint16 k) {  // 0x8BC8AA
  if (cinematic_function == (uint16)FUNC16(nullsub_124))
    cinematicspr_preinstr_func[k >> 1] = FUNC16(CinematicFunction_Intro_Func94);
}

void CinematicFunction_Intro_Func94(uint16 k) {  // 0x8BC8B9
  char v3; // t0

  int v1 = k >> 1;
  uint16 v2 = cinematicspr_goto_timer[v1] + 32;
  cinematicspr_goto_timer[v1] = v2;
  v3 = v2;
  LOBYTE(v2) = HIBYTE(v2);
  HIBYTE(v2) = v3;
  R20_ = v2 & 0xFF00;
  R18_ = (uint8)v2;
  uint16 v4 = cinematicspr_arr7[v1];
  bool v5 = v4 < R20_;
  cinematicspr_arr7[v1] = v4 - R20_;
  uint16 v6 = cinematicbg_arr8[v1] - (v5 + R18_);
  cinematicbg_arr8[v1] = v6;
  if (sign16(v6 + 128)) {
    cinematicspr_instr_timer[v1] = 1;
    cinematicspr_instr_ptr[v1] = addr_off_8BCE53;
    cinematic_function = FUNC16(CinematicFunction_Intro_Func95);
  }
}

void CinematicFunction_Intro_Func96(uint16 k) {  // 0x8BC8F9
  if (cinematic_function == (uint16)FUNC16(nullsub_124))
    cinematicspr_preinstr_func[k >> 1] = FUNC16(CinematicFunction_Intro_Func97);
}

void CinematicFunction_Intro_Func97(uint16 k) {  // 0x8BC908
  char v3; // t0

  int v1 = k >> 1;
  uint16 v2 = cinematicspr_goto_timer[v1] + 32;
  cinematicspr_goto_timer[v1] = v2;
  v3 = v2;
  LOBYTE(v2) = HIBYTE(v2);
  HIBYTE(v2) = v3;
  R20_ = v2 & 0xFF00;
  R18_ = (uint8)v2;
  uint16 v4 = cinematicspr_arr7[v1];
  bool v5 = v4 < R20_;
  cinematicspr_arr7[v1] = v4 - R20_;
  uint16 v6 = cinematicbg_arr8[v1] - (v5 + R18_);
  cinematicbg_arr8[v1] = v6;
  if (sign16(v6 + 128)) {
    cinematicspr_instr_timer[v1] = 1;
    cinematicspr_instr_ptr[v1] = addr_off_8BCE53;
  }
}

void CinematicFunction_Intro_Func98(uint16 j) {  // 0x8BC942
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 48;
  cinematicbg_arr8[v1] = 47;
  cinematicbg_arr9[v1] = 2048;
}

void CinematicFunction_Intro_Func99(uint16 j) {  // 0x8BC956
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 208;
  cinematicbg_arr8[v1] = 47;
  cinematicbg_arr9[v1] = 2048;
}

void CinematicFunction_Intro_Func100(uint16 j) {  // 0x8BC96A
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 48;
  cinematicbg_arr8[v1] = 207;
  cinematicbg_arr9[v1] = 2048;
}

void CinematicFunction_Intro_Func101(uint16 j) {  // 0x8BC97E
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 208;
  cinematicbg_arr8[v1] = 207;
  cinematicbg_arr9[v1] = 2048;
}

void CinematicFunction_Intro_Func102(uint16 j) {  // 0x8BC992
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 128;
  cinematicbg_arr8[v1] = 186;
  cinematicbg_arr9[v1] = 0;
}

uint16 CinematicSprInstr_C9A5(uint16 k, uint16 j) {  // 0x8BC9A5
  SpawnPalfxObject(addr_kPalfx_E1B0);
  return j;
}

uint16 CinematicSprInstr_C9AF(uint16 k, uint16 j) {  // 0x8BC9AF
  if (japanese_text_flag)
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BCEB5, japanese_text_flag);
  return j;
}

uint16 CinematicSprInstr_C9BD(uint16 k, uint16 j) {  // 0x8BC9BD
  SpawnPalfxObject(addr_loc_8BE1B4);
  return j;
}

uint16 CinematicSprInstr_C9C7(uint16 k, uint16 j) {  // 0x8BC9C7
  cinematic_var8 = 62;
  cinematic_var10 = -112;
  cinematic_var5 = 32;
  cinematic_var6 = 16;
  cinematic_function = FUNC16(CinematicFunction_Intro_Func105);
  return j;
}

void CinematicFunction_Intro_Func104(uint16 j) {  // 0x8BC9E6
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 128;
  cinematicbg_arr8[v1] = 204;
  cinematicbg_arr9[v1] = 0;
}

void CinematicFunction_Intro_Func105(void) {  // 0x8BC9F9
  unsigned int v0; // kr00_4

  v0 = __PAIR32__(cinematic_var10, cinematic_var9) + 0x2000;
  cinematic_var10 = HIWORD(v0);
  cinematic_var9 = v0;
  bool v1 = cinematic_var7 >= 0x8000u;
  cinematic_var7 += 0x8000;
  cinematic_var8 -= !v1;
  if (sign16(cinematic_var6 - 1152))
    cinematic_var6 += 4;
  else
    cinematic_function = FUNC16(CinematicFunction_Intro_Func106);
}

void CinematicFunction_Intro_Func106(void) {  // 0x8BCA36
  unsigned int v0; // kr00_4

  v0 = __PAIR32__(cinematic_var10, cinematic_var9) + 0x2000;
  cinematic_var10 = HIWORD(v0);
  cinematic_var9 = v0;
  bool v1 = cinematic_var7 >= 0x8000u;
  cinematic_var7 += 0x8000;
  cinematic_var8 -= !v1;
  if (sign16(cinematic_var8 + 128)) {
    cinematic_function = FUNC16(CinematicFunction_Intro_Func107);
  } else {
    cinematic_var6 += 16;
    if (cinematic_var5 != 224)
      cinematic_var5 = (uint8)(cinematic_var5 - 1);
  }
}

void CinematicFunction_Intro_Func107(void) {  // 0x8BCA85
  unsigned int v0; // kr00_4
  unsigned int v1; // kr04_4

  v0 = __PAIR32__(cinematic_var10, cinematic_var9) + 0x2000;
  cinematic_var10 = HIWORD(v0);
  cinematic_var9 = v0;
  v1 = __PAIR32__(cinematic_var8, cinematic_var7) + 0x2000;
  cinematic_var8 = HIWORD(v1);
  cinematic_var7 = v1;
  if (sign16(cinematic_var6 - 0x2000)) {
    cinematic_var6 += 32;
  } else {
    reg_TM = 16;
    cinematic_function = FUNC16(CinematicFunction_Intro_Func108);
    cinematic_var4 = 64;
  }
}

void CinematicFunction_Intro_Func108(void) {  // 0x8BCAD0
  bool v0 = (--cinematic_var4 & 0x8000u) != 0;
  if (!cinematic_var4 || v0)
    cinematic_function = FUNC16(nullsub_124);
}

void CinematicFunction_Intro_Func95(void) {  // 0x8BCADF
  reg_INIDISP = 0x80;
  for (int i = 656; i >= 0; i -= 2)
    *(uint16 *)((char *)&cinematic_var5 + (uint16)i) = 0;
  game_state = kGameState_6_LoadingGameData;
  samus_health = samus_max_health;
}

CoroutineRet GameState_39_EndingAndCredits_(void) {  // 0x8BD443
  CallCinematicFunction(cinematic_function | 0x8B0000);
  if ((cinematic_var11 & 0x8000u) == 0)
    ++cinematic_var11;
  ++cinematic_var12;
  HandleCinematicSprites();
  MaybeHandleCinematicBgObjects();
  CreditsObject_Process();
  PaletteFxHandler();
  DrawCinematicSpriteObjects_Ending();
  CinematicFunction_Intro_Func149();
  HandleMode7TransformationMatrix();
  return kCoroutineNone;
}

void MaybeHandleCinematicBgObjects(void) {  // 0x8BD474
  if (!sign16(cinematic_function + 0x1AFC))
    HandleCinematicBgObjects();
}

void CinematicFunctionEscapeFromCebes(void) {  // 0x8BD480
  int16 j;

  cur_irq_handler = 0;
  irqhandler_next_handler = 0;
  SetupPpu_5_Mode7();
  for (j = 656; j >= 0; j -= 2)
    *(uint16 *)((char *)&cinematic_var5 + (uint16)j) = 0;
  uint16 v2 = 0;
  do {
    palette_buffer[v2 >> 1] = kPalettes_Intro3[v2 >> 1];
    v2 += 2;
  } while ((int16)(v2 - 512) < 0);
  static const DecompressToParams unk_8BD4C2 = { LONGPTR(0x7f0000) };
  static const DecompressToParams unk_8BD4D3 = { LONGPTR(0x7f4000) };
  static const DecompressToParams unk_8BD4E4 = { LONGPTR(0x7f8000) };
  mov24(&decompress_src, 0x98bcd6);
  DecompressToMem_IpArg(&unk_8BD4C2);
  mov24(&decompress_src, 0x99a56f);
  DecompressToMem_IpArg(&unk_8BD4D3);
  mov24(&decompress_src, 0x99d17e);
  DecompressToMem_IpArg(&unk_8BD4E4);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BD4FC = { 1, 1, 0x18, LONGPTR(0x7f8000), 0x4000 };
  SetupDmaTransfer(&unk_8BD4FC);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x20u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BD51C = { 1, 1, 0x18, LONGPTR(0x7f8000), 0x4000 };
  SetupDmaTransfer(&unk_8BD51C);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x60u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BD53C = { 1, 1, 0x18, LONGPTR(0x7f4000), 0x4000 };
  SetupDmaTransfer(&unk_8BD53C);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BD55C = { 1, 0, 0x19, LONGPTR(0x7f0000), 0x4000 };
  SetupDmaTransfer(&unk_8BD55C);
  WriteReg(MDMAEN, 2u);
  static const DecompressToParams unk_8BD579 = { LONGPTR(0x7f8000) };
  static const DecompressToParams unk_8BD58A = { LONGPTR(0x7f0000) };
  static const DecompressToParams unk_8BD59B = { LONGPTR(0x7f4000) };
  static const DecompressToParams unk_8BD5BD = { LONGPTR(0x7fe000) };
  static const DecompressToParams unk_8BD5CE = { LONGPTR(0x7fe800) };
  static const DecompressToParams unk_8BD5DF = { LONGPTR(0x7ff000) };
  static const DecompressToParams unk_8BD5F0 = { LONGPTR(0x7ff800) };
  static const DecompressToParams unk_8BD601 = { LONGPTR(0x7ea000) };
  static const DecompressToParams unk_8BD612 = { LONGPTR(0x7e2000) };
  static const DecompressToParams unk_8BD623 = { LONGPTR(0x7e6000) };
  mov24(&decompress_src, 0x988304);
  DecompressToMem_IpArg(&unk_8BD579);
  mov24(&decompress_src, 0x95a82f);
  DecompressToMem_IpArg(&unk_8BD58A);
  mov24(&decompress_src, 0x96fe69);
  DecompressToMem_IpArg(&unk_8BD59B);
  uint16 v3 = 768;
  do {
    *(uint16 *)((char *)&g_ram[0x14000] + v3) = 0x8c8c;
    v3 += 2;
  } while ((int16)(v3 - 0x4000) < 0);
  mov24(&decompress_src, 0x98b5c1);
  DecompressToMem_IpArg(&unk_8BD5BD);
  mov24(&decompress_src, 0x98b857);
  DecompressToMem_IpArg(&unk_8BD5CE);
  mov24(&decompress_src, 0x98baed);
  DecompressToMem_IpArg(&unk_8BD5DF);
  mov24(&decompress_src, 0x98bccd);
  DecompressToMem_IpArg(&unk_8BD5F0);
  mov24(&decompress_src, 0x97e7de);
  DecompressToMem_IpArg(&unk_8BD601);
  mov24(&decompress_src, 0x99d65b);
  DecompressToMem_IpArg(&unk_8BD612);
  mov24(&decompress_src, 0x99d932);
  DecompressToMem_IpArg(&unk_8BD623);
  WriteRegWord(M7A, 0x100u);
  reg_M7A = 256;
  WriteRegWord(M7B, 0);
  reg_M7B = 0;
  WriteRegWord(M7C, 0);
  reg_M7C = 0;
  WriteRegWord(M7D, 0x100u);
  reg_M7D = 256;
  WriteRegWord(M7X, 0x80);
  reg_M7X = 128;
  WriteRegWord(M7Y, 0x80);
  reg_M7Y = 128;
  cinematic_var5 = 32;
  cinematic_var6 = 64;
  cinematic_var8 = 0;
  cinematic_var10 = 0;
  door_def_ptr = 0;
  layer1_x_pos = 0;
  layer1_y_pos = 0;
  ClearPaletteFXObjects();
  EnablePaletteFx();
  EnableNMI();
  enemy_projectile_pre_instr[1] = 0;
  reg_TM = 17;
  reg_TS = 1;
  reg_CGWSEL = 2;
  reg_CGADSUB = 17;
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEEEB, 0);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEEF1, 0);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEEEB, 1u);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEEF1, 1u);
  screen_fade_delay = 0;
  screen_fade_counter = 2;
  cinematic_function = FUNC16(CinematicFunction_Intro_Func109);
  QueueMusic_Delayed8(0);
  QueueMusic_Delayed8(0xFF33u);
  QueueMusic_DelayedY(5u, 0xEu);
}

void CinematicFunction_Intro_Func109(void) {  // 0x8BD6D7
  if (!(HasQueuedMusic() & 1)) {
    SpawnPalfxObject(addr_kPalfx_E1D8);
    SpawnPalfxObject(addr_kPalfx_E1DC);
    cinematic_function = FUNC16(CinematicFunction_Intro_Func110);
  }
}

void CinematicFunction_Intro_Func110(void) {  // 0x8BD6F2
  CinematicFunction_Intro_Func111();
  if (AdvanceFastScreenFadeIn() & 1)
    cinematic_function = FUNC16(CinematicFunction_Intro_Func111);
}

void CinematicFunction_Intro_Func111(void) {  // 0x8BD701
  if ((cinematic_var12 & 1) == 0)
    cinematic_var5 = (uint8)(cinematic_var5 - 1);
  cinematic_var6 += 2;
  if (!sign16(cinematic_var6 - 384)) {
    if (AdvanceFastScreenFadeOut() & 1)
      cinematic_function = FUNC16(CinematicFunction_Intro_Func112);
  }
}

void CinematicFunction_Intro_Func112(void) {  // 0x8BD731
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BD746 = { 1, 1, 0x18, LONGPTR(0x7e2000), 0x4000 };
  SetupDmaTransfer(&unk_8BD746);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x20u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BD766 = { 1, 1, 0x18, LONGPTR(0x7e2000), 0x4000 };
  SetupDmaTransfer(&unk_8BD766);
  WriteReg(MDMAEN, 2u);
  mov24(&decompress_src, 0x98ED4Fu);
  static const DecompressToParams unk_8BD783 = { LONGPTR(0x7e2000) };
  DecompressToMem_IpArg(&unk_8BD783);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BD79B = { 1, 0, 0x19, LONGPTR(0x7e2000), 0x4000 };
  SetupDmaTransfer(&unk_8BD79B);
  WriteReg(MDMAEN, 2u);
  ClearPaletteFXObjects();
  ClearCinematicSprites();
  SpawnPalfxObject(addr_kPalfx_E1E0);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEED3, 0);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEED9, 0);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEEDF, 0);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEEE5, 0);
  cinematic_var5 = 32;
  cinematic_var6 = 64;
  cinematic_var8 = 0;
  cinematic_var10 = 0;
  cinematic_function = FUNC16(CinematicFunction_Intro_Func113);
}

void CinematicFunction_Intro_Func113(void) {  // 0x8BD7F8
  CinematicFunction_Intro_Func114();
  if (AdvanceFastScreenFadeIn() & 1)
    cinematic_function = FUNC16(CinematicFunction_Intro_Func114);
}

void CinematicFunction_Intro_Func114(void) {  // 0x8BD807
  if ((cinematic_var12 & 1) == 0)
    cinematic_var5 = (uint8)(cinematic_var5 - 1);
  cinematic_var6 += 3;
  if (!sign16(cinematic_var6 - 384)) {
    if (AdvanceFastScreenFadeOut() & 1)
      cinematic_function = FUNC16(CinematicFunction_Intro_Func115);
  }
}

void CinematicFunction_Intro_Func115(void) {  // 0x8BD837
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BD84C = { 1, 1, 0x18, LONGPTR(0x7e6000), 0x4000 };
  SetupDmaTransfer(&unk_8BD84C);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x20u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BD86C = { 1, 1, 0x18, LONGPTR(0x7e6000), 0x4000 };
  SetupDmaTransfer(&unk_8BD86C);
  WriteReg(MDMAEN, 2u);
  mov24(&decompress_src, 0x999101u);
  static const DecompressToParams unk_8BD889 = { LONGPTR(0x7e6000) };
  DecompressToMem_IpArg(&unk_8BD889);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BD8A1 = { 1, 0, 0x19, LONGPTR(0x7e6000), 0x4000 };
  SetupDmaTransfer(&unk_8BD8A1);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x40u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BD8C1 = { 1, 1, 0x18, LONGPTR(0x7f8000), 0x6000 };
  SetupDmaTransfer(&unk_8BD8C1);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x70u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BD8E1 = { 1, 1, 0x18, LONGPTR(0x7fe000), 0x0800 };
  SetupDmaTransfer(&unk_8BD8E1);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x74u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BD901 = { 1, 1, 0x18, LONGPTR(0x7fe800), 0x0800 };
  SetupDmaTransfer(&unk_8BD901);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x78u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BD921 = { 1, 1, 0x18, LONGPTR(0x7ff000), 0x0800 };
  SetupDmaTransfer(&unk_8BD921);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x7Cu);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BD941 = { 1, 1, 0x18, LONGPTR(0x7ff800), 0x0800 };
  SetupDmaTransfer(&unk_8BD941);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x50u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BD961 = { 1, 1, 0x18, LONGPTR(0x7ea000), 0x1000 };
  SetupDmaTransfer(&unk_8BD961);
  WriteReg(MDMAEN, 2u);
  reg_OBSEL = 2;
  ClearPaletteFXObjects();
  ClearCinematicSprites();
  uint16 v0 = 256;
  do {
    int v1 = v0 >> 1;
    uint16 v2 = kPalettes_Intro6[v1];
    palette_buffer[v1] = v2;
    v0 += 2;
  } while ((int16)(v0 - 512) < 0);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEE9D, 0);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEEAF, 0);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEEA3, 0);
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEEA9, 0);
  cinematic_var5 = 0;
  cinematic_var6 = 64;
  reg_M7A = 256;
  reg_M7B = 0;
  reg_M7C = 0;
  reg_M7D = 256;
  reg_M7X = 128;
  reg_M7Y = 128;
  cinematic_var8 = 0;
  cinematic_var10 = 0;
  reg_TM = 1;
  reg_TS = 16;
  reg_CGWSEL = 2;
  reg_CGADSUB = 33;
  CopyPalettesToFadingPalettes();
  DecomposePaletteDataForFading();
  ClearYColorsFromIndexX(0x1A0u, 0x10u);
  ClearYColorsFromIndexX(0x1E0u, 0x10u);
  ComposeFadingPalettes();
  cinematic_var4 = 63;
  cinematic_function = FUNC16(CinematicFunction_Intro_Func116);
}

void CinematicFunction_Intro_Func116(void) {  // 0x8BDA02
  CinematicFunction_Intro_Func117();
  if (AdvanceFastScreenFadeIn() & 1) {
    screen_fade_delay = 2;
    screen_fade_counter = 2;
    cinematic_function = FUNC16(CinematicFunction_Intro_Func117);
  }
}

void CinematicFunction_Intro_Func117(void) {  // 0x8BDA1A
  cinematic_var6 += 4;
  if ((cinematic_var4 & 1) == 0) {
    FadeOutYColorsFromIndexX(0xE0u, 0x10u);
    FadeInYColorsFromIndexX(0x1A0u, 0x10u);
    FadeInYColorsFromIndexX(0x1E0u, 0x10u);
    ComposeFadingPalettes();
  }
  if ((--cinematic_var4 & 0x8000u) != 0) {
    uint16 v0 = 0;
    do {
      palette_buffer[v0 >> 1] = kPalettes_Intro6[v0 >> 1];
      v0 += 2;
    } while ((int16)(v0 - 256) < 0);
    reg_M7SEL = 0;
    reg_BG1SC = 112;
    reg_BG2SC = 120;
    reg_BG3SC = 0;
    reg_BG4SC = 0;
    reg_BG12NBA = 68;
    reg_BG34NBA = 0;
    reg_TM = 16;
    reg_TS = 0;
    reg_TMW = 0;
    reg_TSW = 0;
    next_gameplay_CGWSEL = 0;
    next_gameplay_CGADSUB = 0;
    reg_CGWSEL = 0;
    reg_CGADSUB = 0;
    WriteReg(COLDATA, 0xE0u);
    reg_COLDATA[0] = 32;
    reg_COLDATA[1] = 64;
    reg_COLDATA[2] = 0x80;
    reg_MOSAIC = 0;
    cinematic_var7 = 0;
    cinematic_var8 = 0;
    cinematic_var9 = 0;
    cinematic_var10 = 0;
    g_word_7E1999 = 0;
    g_word_7E199B = 0;
    g_word_7E199D = 0;
    g_word_7E199F = 0;
    fx_layer_blending_config_a = 0;
    reg_BG2HOFS = 0;
    reg_BG2VOFS = 0;
    screen_fade_delay = 0;
    screen_fade_counter = 2;
    cinematic_function = FUNC16(CinematicFunction_Intro_Func118);
    cinematic_var4 = 0;
  }
}
static const uint16 kCinematicFunction_Intro_Func118_Tab0[16] = {  // 0x8BDAD3
       0,  0x800, 0x1000, 0x1800,
  0x2000, 0x2800, 0x3000, 0x3800,
  0x4000, 0x4800, 0x5000, 0x5800,
  0x6000, 0x6800, 0x7000, 0x7800,
};
static const uint16 kCinematicFunction_Intro_Func118_Tab1[16] = {
       0,  0x800, 0x1000, 0x1800,
  0x2000, 0x2800, 0x3000, 0x3800,
       0,  0x800, 0x1000, 0x1800,
  0x2000, 0x2800, 0x3000, 0x3800,
};
void CinematicFunction_Intro_Func118(void) {
  unsigned int v2;

  uint16 v0 = cinematic_var4;
  uint16 v1 = mode7_vram_write_queue_tail;
  v2 = mode7_vram_write_queue_tail;
  if (sign16(cinematic_var4 - 8)) {
    *(uint16 *)(&mode7_write_queue[0].field_0 + mode7_vram_write_queue_tail) = 192;
    int v3 = v0;
    *(uint16 *)((char *)&mode7_write_queue[0].field_1 + v2) = kCinematicFunction_Intro_Func118_Tab0[v3];
    *(uint16 *)&mode7_write_queue[0].gap3[v1] = 127;
    *(uint16 *)&mode7_write_queue[0].gap3[v1 + 1] = 2048;
    *(uint16 *)&mode7_write_queue[0].gap3[v1 + 3] = kCinematicFunction_Intro_Func118_Tab1[v3];
    *(uint16 *)((char *)&mode7_write_queue[1].field_1 + v1) = 128;
  } else {
    *(uint16 *)(&mode7_write_queue[0].field_0 + mode7_vram_write_queue_tail) = 128;
    int v4 = v0;
    *(uint16 *)((char *)&mode7_write_queue[0].field_1 + v2) = kCinematicFunction_Intro_Func118_Tab0[v4];
    *(uint16 *)&mode7_write_queue[0].gap3[v1] = 127;
    *(uint16 *)&mode7_write_queue[0].gap3[v1 + 1] = 2048;
    *(uint16 *)&mode7_write_queue[0].gap3[v1 + 3] = kCinematicFunction_Intro_Func118_Tab1[v4];
    *(uint16 *)((char *)&mode7_write_queue[1].field_1 + v1) = 0;
  }
  mode7_vram_write_queue_tail = v1 + 9;
  if (!sign16(++cinematic_var4 - 16)) {
    reg_BGMODE = 1;
    cinematic_var4 = 0;
    cinematic_function = FUNC16(nullsub_125);
  }
}

void CinematicFunction_Intro_Func119(void) {  // 0x8BDB9E
  bool v0 = (--cinematic_var4 & 0x8000u) != 0;
  if (!cinematic_var4 || v0) {
    QueueMusic_Delayed8(0);
    QueueMusic_Delayed8(0xFF3Cu);
    QueueMusic_DelayedY(5u, 0xEu);
    cinematic_function = FUNC16(CinematicFunction_Intro_Func120);
  }
}

void CinematicFunction_Intro_Func120(void) {  // 0x8BDBC4
  if (!(HasQueuedMusic() & 1)) {
    reg_BGMODE = 7;
    reg_M7SEL = 0x80;
    reg_TM = 17;
    reg_TS = 0;
    reg_CGWSEL = 0;
    reg_CGADSUB = 48;
    reg_COLDATA[0] = 63;
    reg_COLDATA[1] = 95;
    reg_COLDATA[2] = -97;
    reg_BG1SC = 0;
    reg_BG12NBA = 0;
    palette_buffer[0] = 0;
    palette_buffer[16] = 0;
    palette_buffer[128] = 0;
    SpawnPalfxObject(addr_kPalfx_E1D4);
    SpawnPalfxObject(addr_kPalfx_E1E4);
    reg_M7X = 56;
    reg_M7Y = 24;
    cinematic_var8 = -72;
    cinematic_var10 = -104;
    cinematic_var6 = 3072;
    cinematic_var5 = -112;
    cinematic_var4 = 1;
    cinematic_var13 = 192;
    cinematic_var14 = 0;
    cinematic_function = FUNC16(CinematicFunction_Intro_Func121);
  }
}

void CinematicFunction_Intro_Func122(void) {  // 0x8BDC4C
  bool v0 = (--cinematic_var4 & 0x8000u) != 0;
  if (!cinematic_var4 || v0) {
    if (sign8(--reg_COLDATA[0] - 32))
      reg_COLDATA[0] = 32;
    if (sign8(--reg_COLDATA[1] - 64))
      reg_COLDATA[1] = 64;
    if (sign8(--reg_COLDATA[2] + 0x80))
      reg_COLDATA[2] = 0x80;
    if (reg_COLDATA[0] == 32 && reg_COLDATA[1] == 64 && reg_COLDATA[2] == 0x80) {
      reg_CGWSEL = 0;
      reg_CGADSUB = 0;
    }
    cinematic_var4 = 8;
  }
}
static const uint16 kCinematicFunction_Intro_Func121_Tab0[32] = {  // 0x8BDCA5
  0, 0x8000, 0, 0x8000,      0, 0x8000,      0, 0x8000, 0xffff, 0x8000, 0xffff, 0x8000,      0, 0x8000,      0, 0x8000,
  0, 0x8000, 0, 0x8000, 0xffff, 0x8000, 0xffff, 0x8000,      0, 0x8000,      0, 0x8000, 0xffff, 0x8000, 0xffff, 0x8000,
};
void CinematicFunction_Intro_Func121(void) {
  int16 v1;

  if (cinematic_var13)
    --cinematic_var13;
  else
    CinematicFunction_Intro_Func122();
  cinematic_var5 = (uint8)(cinematic_var5 - 4);
  int v0 = (uint16)(4 * cinematic_var14) >> 1;
  v1 = (kCinematicFunction_Intro_Func121_Tab0[v0 + 1] + __PAIR32__(cinematic_var8, cinematic_var7)) >> 16;
  cinematic_var7 += kCinematicFunction_Intro_Func121_Tab0[v0 + 1];
  cinematic_var8 = kCinematicFunction_Intro_Func121_Tab0[v0] + v1;
  cinematic_var14 = ((uint8)cinematic_var14 + 1) & 0xF;
  cinematic_var6 -= 8;
  if (sign16(cinematic_var6 - 1456)) {
    cinematic_var13 = 1;
    cinematic_var14 = 0;
    cinematic_function = FUNC16(CinematicFunction_Intro_Func123);
  }
}
static const uint16 kCinematicFunction_Intro_Func123_Tab0[16] = { 1, 0, 1, 0, 1, 0, 0xffff, 0, 0xffff, 0, 1, 0, 1, 0, 0xffff, 0 };
void CinematicFunction_Intro_Func123(void) {  // 0x8BDD42
  int16 v1;

  CinematicFunction_Intro_Func122();
  if (cinematic_var5 != 224)
    cinematic_var5 = (uint8)(cinematic_var5 - 1);
  int v0 = (uint16)(4 * cinematic_var14) >> 1;
  v1 = (kCinematicFunction_Intro_Func123_Tab0[v0 + 1] + __PAIR32__(cinematic_var8, cinematic_var7)) >> 16;
  cinematic_var7 += kCinematicFunction_Intro_Func123_Tab0[v0 + 1];
  cinematic_var8 = kCinematicFunction_Intro_Func123_Tab0[v0] + v1;
  cinematic_var14 = ((uint8)cinematic_var14 + 1) & 7;
  cinematic_var6 -= 2;
  if (sign16(cinematic_var6 - 1184)) {
    cinematic_function = FUNC16(CinematicFunction_Intro_Func124);
    cinematic_var14 = 0x8000;
    cinematic_var13 = 0;
    if (CheckEventHappened(0xFu)) {
      R18_ = 4;
      SpawnCinematicSpriteObjectToR18(0xEF21u, 4u);
    }
  }
}
static const uint16 g_word_8BDE43[16] = { 0, 0x6f7b, 0x4a52, 0x1ce7, 0, 0x5ad6, 0x4a52, 0x35ad, 0x2529, 0x7fff, 0x56b5, 0x294a, 0x4210, 0x2d6b, 0x18c6, 0x7fff };
void CinematicFunction_Intro_Func124(void) {  // 0x8BDDCD
  unsigned int v2; // kr04_4

  CinematicFunction_Intro_Func122();
  uint16 v0 = (__PAIR32__(cinematic_var13, cinematic_var14) - 256) >> 16;
  cinematic_var14 -= 256;
  cinematic_var13 = v0;
  v2 = __PAIR32__(v0, cinematic_var14) + __PAIR32__(cinematic_var8, cinematic_var7);
  uint16 v1 = (__PAIR32__(v0, cinematic_var14) + __PAIR32__(cinematic_var8, cinematic_var7)) >> 16;
  cinematic_var7 = v2;
  cinematic_var8 = v1;
  if (sign16(cinematic_var6 - 384) && (cinematic_var12 & 3) == 0 && cinematic_var5 != 16)
    cinematic_var5 = (uint8)(cinematic_var5 + 2);
  if (sign16(cinematic_var6 - 32)) {
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEEC7, cinematic_var6);
    cinematic_function = FUNC16(nullsub_126);
    for (int i = 30; i >= 0; i -= 2)
      palette_buffer[(i >> 1) + 80] = g_word_8BDE43[i >> 1];
  } else {
    cinematic_var6 -= 4;
  }
}

void CinematicFunction_Intro_Func125(void) {  // 0x8BDE64
  screen_fade_counter = 1;
  if (AdvanceFastScreenFadeOut() & 1) {
    EnableNMI();
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    cinematic_function = FUNC16(CinematicFunction_Intro_Func126);
  }
}

void CinematicFunction_Intro_Func126(void) {  // 0x8BDE80
  SetupPpu_7_Mode1();
  ClearCinematicSprites();
  ClearPaletteFXObjects();
  uint16 v0 = 0;
  do {
    palette_buffer[v0 >> 1] = kPalettes_Intro5[v0 >> 1];
    v0 += 2;
  } while ((int16)(v0 - 256) < 0);
  static const DecompressToParams unk_8BDEAA = { LONGPTR(0x7f0000) };
  static const DecompressToParams unk_8BDEBB = { LONGPTR(0x7e2000) };
  static const DecompressToParams unk_8BDECC = { LONGPTR(0x7e6000) };
  static const DecompressToParams unk_8BDEDD = { LONGPTR(0x7fc000) };
  static const DecompressToParams unk_8BDEEE = { LONGPTR(0x7fe000) };
  static const DecompressToParams unk_8BDEFF = { LONGPTR(0x7f4000) };
  static const DecompressToParams unk_8BDF10 = { LONGPTR(0x7fe800) };
  static const DecompressToParams unk_8BDF21 = { LONGPTR(0x7ff000) };
  mov24(&decompress_src, 0x979803u);
  DecompressToMem_IpArg(&unk_8BDEAA);
  mov24(&decompress_src, 0x97B957u);
  DecompressToMem_IpArg(&unk_8BDEBB);
  mov24(&decompress_src, 0x97D7FCu);
  DecompressToMem_IpArg(&unk_8BDECC);
  mov24(&decompress_src, 0x97E7DEu);
  DecompressToMem_IpArg(&unk_8BDEDD);
  mov24(&decompress_src, 0x9796F4u);
  DecompressToMem_IpArg(&unk_8BDEEE);
  mov24(&decompress_src, 0x97F987u);
  DecompressToMem_IpArg(&unk_8BDEFF);
  mov24(&decompress_src, 0x99DA9Fu);
  DecompressToMem_IpArg(&unk_8BDF10);
  mov24(&decompress_src, 0x99DAB1u);
  DecompressToMem_IpArg(&unk_8BDF21);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x20u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BDF39 = { 1, 1, 0x18, LONGPTR(0x7fe800), 0x0100 };
  SetupDmaTransfer(&unk_8BDF39);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x24u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BDF59 = { 1, 1, 0x18, LONGPTR(0x7ff000), 0x0800 };
  SetupDmaTransfer(&unk_8BDF59);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x40u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BDF79 = { 1, 1, 0x18, LONGPTR(0x7fc000), 0x1000 };
  SetupDmaTransfer(&unk_8BDF79);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x60u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BDF99 = { 1, 1, 0x18, LONGPTR(0x7e6000), 0x4000 };
  SetupDmaTransfer(&unk_8BDF99);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x50u);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BDFB9 = { 1, 1, 0x18, LONGPTR(0x7f0000), 0x2000 };
  SetupDmaTransfer(&unk_8BDFB9);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x4Cu);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_8BDFD9 = { 1, 1, 0x18, LONGPTR(0x7fe000), 0x0800 };
  SetupDmaTransfer(&unk_8BDFD9);
  WriteReg(MDMAEN, 2u);

  mov24(&decompress_src, 0x99E089u);
  static const DecompressToParams unk_8BDFF6 = { LONGPTR(0x7e6000) };
  DecompressToMem_IpArg(&unk_8BDFF6);
  mov24(&decompress_src, 0x99ECC4u);
  static const DecompressToParams unk_8BE007 = { LONGPTR(0x7e8000) };
  DecompressToMem_IpArg(&unk_8BE007);
  if (sign16(game_time_hours - 3)) {
    WriteReg(VMADDL, 0);
    WriteReg(VMADDH, 0);
    WriteReg(VMAIN, 0x80);
    static const StartDmaCopy unk_8BE027 = { 1, 1, 0x18, LONGPTR(0x7e2000), 0x4000 };
    SetupDmaTransfer(&unk_8BE027);
    WriteReg(MDMAEN, 2u);
  } else {
    WriteReg(VMADDL, 0);
    WriteReg(VMADDH, 0);
    WriteReg(VMAIN, 0x80);
    static const StartDmaCopy unk_8BE04B = { 1, 1, 0x18, LONGPTR(0x7f0000), 0x4000 };
    SetupDmaTransfer(&unk_8BE04B);
    WriteReg(MDMAEN, 2u);
  }
  WriteRegWord(M7A, 0x100u);
  reg_M7A = 256;
  WriteRegWord(M7B, 0);
  reg_M7B = 0;
  WriteRegWord(M7C, 0);
  reg_M7C = 0;
  WriteRegWord(M7D, 0x100u);
  reg_M7D = 256;
  WriteRegWord(M7X, 0x68u);
  reg_M7X = 104;
  WriteRegWord(M7Y, 0x70u);
  reg_M7Y = 112;
  cinematic_var8 = -8;
  cinematic_var10 = -24;
  cinematic_var5 = 0;
  cinematic_var6 = 2304;
  EnableNMI();
  reg_INIDISP = 15;
  layer1_x_pos = 0;
  layer1_y_pos = 0;
  EnableHdmaObjects();
  for (int i = 510; i >= 0; i -= 2)
    hdma_table_1[i >> 1] = 0;
  CinematicFunction_Intro_Func127();
  mov24(&decompress_src, 0x97EEFFu);
  static const DecompressToParams unk_8BE0CB = { LONGPTR(0x7f0000) };
  DecompressToMem_IpArg(&unk_8BE0CB);
  ClearCinematicBgObjects(0x7F);
  CinematicFunction_Intro_Func128(0x7F);
  EnableTextGlowObjects_();
  screen_fade_delay = 3;
  screen_fade_counter = 3;
  CreditsObject_Init(addr_stru_8BF6F8);
  cinematic_function = FUNC16(nullsub_125);
}

void CinematicFunction_Intro_Func129(void) {  // 0x8BE0F4
  bool v0 = (--cinematic_var4 & 0x8000u) != 0;
  if (!cinematic_var4 || v0) {
    cinematic_function = FUNC16(CinematicFunction_Intro_Func130);
    cinematic_var4 = 120;
    reg_TM = 16;
  }
}

void CinematicFunction_Intro_Func130(void) {  // 0x8BE110
  HandleFadeIn();
  if (reg_INIDISP == 15) {
    reg_TM = 2;
    reg_TS = 16;
    reg_CGWSEL = 2;
    reg_CGADSUB = 34;
    screen_fade_delay = 0;
    screen_fade_counter = 0;
    CopyPalettesToFadingPalettes();
    DecomposePaletteDataForFading();
    ClearYColorsFromIndexX(0x40u, 0x10u);
    ComposeFadingPalettes();
    cinematic_var4 = 32;
    cinematic_function = FUNC16(CinematicFunction_Intro_Func131);
  }
}

void CinematicFunction_Intro_Func131(void) {  // 0x8BE158
  FadeInYColorsFromIndexX(0x40u, 0x10u);
  ComposeFadingPalettes();
  bool v0 = (--cinematic_var4 & 0x8000u) != 0;
  if (!cinematic_var4 || v0) {
    reg_TM = 18;
    reg_TS = 4;
    reg_CGWSEL = 2;
    reg_CGADSUB = 2;
    cinematic_var4 = 180;
    cinematic_function = FUNC16(CinematicFunction_Intro_Func132);
    CinematicFunction_Intro_Func133();
  }
}

void CinematicFunction_Intro_Func132(void) {  // 0x8BE190
  reg_BG3VOFS += 2;
  bool v0 = (--cinematic_var4 & 0x8000u) != 0;
  if (!cinematic_var4 || v0) {
    uint16 v1 = 0;
    do {
      ram3000.pause_menu_map_tilemap[v1 + 288] = g_word_8CDC9B[v1];
      ++v1;
    } while ((int16)(v1 * 2 - 576) < 0);
    CinematicUpdateSomeBg();
    reg_TM = 1;
    reg_TS = 0;
    reg_CGWSEL = 0;
    reg_CGADSUB = 0;
    cinematic_var4 = 180;
    cinematic_function = FUNC16(CinematicFunction_Intro_Func134);
    g_word_7E0D9C = 0;
  }
}

void CinematicFunction_Intro_Func134(void) {  // 0x8BE1D2
  if (!--cinematic_var4) {
    cinematic_var4 = 127;
    if (sign16(game_time_hours - 3)) {
      CopyPalettesToFadingPalettes();
      DecomposePaletteDataForFading();
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF27, 0);
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF2D, 0);
    } else {
      if (!sign16(game_time_hours - 10)) {
        SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF4B, 0);
        SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF45, 0);
        cinematic_function = FUNC16(CinematicFunction_Intro_Func135);
        reg_TM = 16;
        reg_TS = 0;
        reg_CGWSEL = 0;
        reg_CGADSUB = 0;
        return;
      }
      CopyPalettesToFadingPalettes();
      DecomposePaletteDataForFading();
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF51, 0);
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF45, 0);
      ClearYColorsFromIndexX(0x1C0u, 0x10u);
    }
    ClearYColorsFromIndexX(0x1A0u, 0x10u);
    ComposeFadingPalettes();
    reg_TM = 2;
    reg_TS = 16;
    reg_CGWSEL = 2;
    reg_CGADSUB = 34;
    cinematic_function = FUNC16(CinematicFunction_Intro_Func136);
  }
}

void CinematicFunction_Intro_Func136(void) {  // 0x8BE265
  if ((cinematic_var4 & 3) != 0) {
    CinematicFunction_Intro_Func135();
  } else {
    FadeOutYColorsFromIndexX(0x40u, 0x10u);
    if (!sign16(game_time_hours - 3))
      FadeInYColorsFromIndexX(0x1C0u, 0x10u);
    FadeInYColorsFromIndexX(0x1A0u, 0x10u);
    ComposeFadingPalettes();
    CinematicFunction_Intro_Func135();
  }
}

void CinematicFunction_Intro_Func135(void) {  // 0x8BE293
  if (sign16(--cinematic_var4 - 65)) {
    for (int i = 574; i >= 0; i -= 2)
      *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[288] + (uint16)i) = 127;
    uint16 v1 = 0;
    do {
      ram3000.pause_menu_map_tilemap[v1 + 384] = g_word_8CDEDB[v1];
      ++v1;
    } while ((int16)(v1 * 2 - 128) < 0);
    CinematicUpdateSomeBg();
    reg_TM = 1;
    reg_TS = 0;
    reg_CGWSEL = 0;
    reg_CGADSUB = 0;
    cinematic_var13 = 180;
    cinematic_function = FUNC16(CinematicFunction_Intro_Func137);
  }
}

void CinematicFunction_Intro_Func137(void) {  // 0x8BE2DD
  bool v0 = (--cinematic_var13 & 0x8000u) != 0;
  if (!cinematic_var13 || v0) {
    if (sign16(game_time_hours - 3) || sign16(game_time_hours - 10)) {
      reg_TM = 2;
      reg_TS = 16;
      reg_CGWSEL = 2;
      reg_CGADSUB = 34;
      cinematic_function = FUNC16(CinematicFunction_Intro_Func138);
    } else {
      cinematic_function = FUNC16(CinematicFunction_Intro_Func145);
    }
  }
}

void CinematicFunction_Intro_Func138(void) {  // 0x8BE314
  if ((cinematic_var4 & 3) != 0) {
    CinematicFunction_Intro_Func145();
  } else {
    FadeOutYColorsFromIndexX(0x40u, 0x10u);
    if (!sign16(game_time_hours - 3))
      FadeInYColorsFromIndexX(0x1C0u, 0x10u);
    FadeInYColorsFromIndexX(0x1A0u, 0x10u);
    ComposeFadingPalettes();
    CinematicFunction_Intro_Func145();
  }
}

void CinematicFunction_Intro_Func145(void) {  // 0x8BE342
  if ((--cinematic_var4 & 0x8000u) != 0) {
    reg_TM = 16;
    reg_TS = 0;
    reg_CGWSEL = 0;
    reg_CGADSUB = 0;
    reg_BGMODE = 7;
    reg_M7SEL = 0x80;
    reg_BG1SC = 0;
    reg_BG12NBA = 0;
    cinematic_function = FUNC16(nullsub_125);
    cinematic_var14 = 0;
    if (sign16(game_time_hours - 3)) {
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF33, 0);
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF39, 0);
    } else {
      if (sign16(game_time_hours - 10))
        SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF69, 0);
      else
        SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF63, 0);
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF5D, 0);
      SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF57, 0);
    }
  }
}

void CinematicFunction_Intro_Func139(void) {  // 0x8BE3AE
  if (cinematic_var14) {
    --cinematic_var14;
    FadeOutYColorsFromIndexX(0x180u, 0x10u);
    if (cinematic_var4)
      ComposeFadingPalettes();
  }
  if (cinematic_var4)
    --cinematic_var4;
  else
    CinematicFunction_Intro_Func140();
  cinematic_var5 = (uint8)(cinematic_var5 - 8);
  cinematic_var6 -= 64;
  if (sign16(cinematic_var6 - 24)) {
    cinematic_var6 = 24;
    cinematic_function = FUNC16(CinematicFunction_Intro_Func141);
    cinematic_var4 = 180;
    cinematic_var14 = 0;
  }
}

void CinematicFunction_Intro_Func140(void) {  // 0x8BE409
  if (cinematic_var17) {
    --cinematic_var17;
    FadeOutYColorsFromIndexX(0x1E0u, 0x10u);
    ComposeFadingPalettes();
  }
}

void CinematicFunction_Intro_Func142(void) {  // 0x8BE41F
  if (!cinematic_var17) {
    if (sign16(cinematic_var14 - 6)) {
      uint16 v0 = 4 * cinematic_var14;
      uint16 v1 = vram_write_queue_tail;
      int v2 = (uint16)(8 * cinematic_var14) >> 1;
      gVramWriteEntry(vram_write_queue_tail)->size = kCinematicFunction_Intro_Func142_Tab0[v2];
      v1 += 2;
      gVramWriteEntry(v1)->size = kCinematicFunction_Intro_Func142_Tab0[v2 + 1];
      v1 += 2;
      LOBYTE(gVramWriteEntry(v1++)->size) = kCinematicFunction_Intro_Func142_Tab0[v0 + 2];
      gVramWriteEntry(v1)->size = kCinematicFunction_Intro_Func142_Tab0[v2 + 3];
      vram_write_queue_tail = v1 + 2;
      ++cinematic_var14;
    }
  }
}

void CinematicFunction_Intro_Func141(void) {  // 0x8BE48A
  CinematicFunction_Intro_Func140();
  CinematicFunction_Intro_Func142();
  if (!--cinematic_var4) {
    reg_BGMODE = 1;
    reg_TM = 17;
    reg_TS = 0;
    reg_CGWSEL = 0;
    reg_CGADSUB = 49;
    reg_COLDATA[0] = 63;
    reg_COLDATA[1] = 95;
    reg_COLDATA[2] = -97;
    reg_M7SEL = 0;
    reg_BG1SC = 78;
    reg_BG12NBA = 84;
    uint16 v0 = 384;
    do {
      palette_buffer[v0 >> 1] = kPalettes_Intro4[v0 >> 1];
      v0 += 2;
    } while ((int16)(v0 - 416) < 0);
    cinematic_var8 = 0;
    cinematic_var10 = 0;
    cinematic_function = FUNC16(CinematicFunction_Intro_Func143);
    cinematic_var4 = 32;
    EnableCinematicBgObjects();
    EnableCinematicBgTilemapUpdates();
    cinematicbg_var3 = 19456;
    for (int i = 126; i >= 0; i -= 2)
      *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[384] + (uint16)i) = 127;
    CinematicUpdateSomeBg();
  }
}

void CinematicFunction_Intro_Func143(void) {  // 0x8BE504
  uint16 j;

  if (sign8(--reg_COLDATA[0] - 32))
    reg_COLDATA[0] = 32;
  if (sign8(--reg_COLDATA[1] - 64))
    reg_COLDATA[1] = 64;
  if (sign8(--reg_COLDATA[2] + 0x80))
    reg_COLDATA[2] = 0x80;
  bool v0 = (--cinematic_var4 & 0x8000u) != 0;
  if (!cinematic_var4 || v0) {
    cinematic_function = FUNC16(nullsub_127);
    cinematicbg_var3 = 20480;
    CinematicUpdateSomeBg();
    layer1_x_pos = 256;
    layer1_y_pos = 256;
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF81, 0);
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF87, 0);
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF8D, 0);
    SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF93, 0);
    for (int i = 30; i >= 0; i -= 2)
      palette_buffer[(i >> 1) + 16] = 0;
    for (j = 30; (j & 0x8000u) == 0; j -= 2)
      palette_buffer[(j >> 1) + 240] = g_word_8CEFE9[j >> 1];
  }
}

void CinematicFunction_Intro_Func144(void) {  // 0x8BE58A
  uint16 j;

  int v0 = (uint16)(4 * cinematic_var4) >> 1;
  uint16 v5 = kCinematicFunction_Intro_Func144_Tab0[v0 + 1];
  uint16 v1 = kCinematicFunction_Intro_Func144_Tab0[v0];
  for (int i = 30; i >= 0; i -= 2) {
    palette_buffer[(i >> 1) + 16] = *(uint16 *)RomPtr_8C(v1);
    v1 -= 2;
  }
  uint16 v3 = v5;
  for (j = 30; (j & 0x8000u) == 0; j -= 2) {
    palette_buffer[(j >> 1) + 240] = *(uint16 *)RomPtr_8C(v3);
    v3 -= 2;
  }
  if (!sign16(++cinematic_var4 - 16)) {
    cinematic_function = FUNC16(nullsub_127);
    ClearCinematicSprites();
    SpawnCinematicBgObject(addr_kCinematicBgObjectDef_8BF74E, 0x4C00u);
  }
}

uint16 CalcItemPercentageCount(uint16 k, uint16 instrp) {  // 0x8BE627
  uint16 j;

  uint16 v0 = 4;
  R18_ = 0;
  do {
    uint16 *v1 = (uint16 *)RomPtr_RAM(g_off_8BE70D[v0]);
    R18_ += SnesDivide(*v1, g_word_8BE717[v0]);
    --v0;
  } while ((v0 & 0x8000u) == 0);
  for (int i = 20; i >= 0; i -= 2) {
    if ((g_word_8BE721[i >> 1] & collected_items) != 0)
      ++R18_;
  }
  for (j = 8; (j & 0x8000u) == 0; j -= 2) {
    if ((g_word_8BE737[j >> 1] & collected_beams) != 0)
      ++R18_;
  }
  uint16 RegWord = R18_ / 10;
  R22_ = R18_ % 10;
  R18_ = RegWord / 10;
  R20_ = RegWord % 10;
  if (R18_) {
    int v5 = (uint16)(4 * R18_) >> 1;
    ram3000.pause_menu_map_tilemap[462] = g_word_8BE741[v5];
    ram3000.pause_menu_map_tilemap[494] = g_word_8BE741[v5 + 1];
  }
  uint16 v6 = R20_;
  if (R20_)
    goto LABEL_16;
  if (R18_) {
    v6 = R20_;
LABEL_16:;
    int v7 = (uint16)(4 * v6) >> 1;
    ram3000.pause_menu_map_tilemap[463] = g_word_8BE741[v7];
    ram3000.pause_menu_map_tilemap[495] = g_word_8BE741[v7 + 1];
  }
  int v8 = (uint16)(4 * R22_) >> 1;
  ram3000.pause_menu_map_tilemap[464] = g_word_8BE741[v8];
  ram3000.pause_menu_map_tilemap[496] = g_word_8BE741[v8 + 1];
  ram3000.pause_menu_map_tilemap[465] = 14442;
  ram3000.pause_menu_map_tilemap[497] = 14458;

  return instrp;
}

uint16 CinematicFunction_Intro_Func146(uint16 k, uint16 j) {  // 0x8BE769
  if (japanese_text_flag) {
    for (int i = 63; i >= 0; --i)
      ram3000.pause_menu_map_tilemap[i + 736] = g_word_8CDF5B[i];
  }
  return j;
}

uint16 CinematicFunction_Intro_Func147(uint16 k, uint16 j) {  // 0x8BE780
  for (int i = 126; i >= 0; i -= 2)
    *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[736] + (uint16)i) = 127;
  cinematic_function = FUNC16(CinematicFunction_Intro_Func148);
  return j;
}

void CinematicFunction_Intro_Func148(void) {  // 0x8BE797
  cinematic_var10 -= 2;
  if (sign16(cinematic_var10 + 79)) {
    SpawnCinematicBgObject(addr_kCinematicBgObjectDef_8BF748, 0x4C00u);
    cinematic_function = FUNC16(nullsub_127);
    CopySuperMetroidString();
  }
}

void CinematicFunction_Intro_Func127(void) {  // 0x8BE7BB
  int16 v1;
  int16 v4;

  uint16 v0 = ADDR16_OF_RAM(*g_byte_7E0E0C);
  v1 = 0;
  R18_ = 0;
  do {
    v4 = v1;
    uint8 *v2 = RomPtr_RAM(v0);
    *(uint16 *)v2 = v1;
    *((uint16 *)v2 + 6) = 0;
    *((uint16 *)v2 + 7) = 0;
    uint16 v3 = g_word_8BE9CF[((uint16)(8 * v1) >> 1) + 3];
    if (v3) {
      *((uint16 *)v2 + 5) = v3;
      *(uint16 *)v2 |= 0x8000u;
    } else {
      *((uint16 *)v2 + 5) = 32;
    }
    *((uint16 *)v2 + 1) = 128;
    *((uint16 *)v2 + 3) = 128;
    *((uint16 *)v2 + 2) = 0;
    *((uint16 *)v2 + 4) = 0;
    v0 += 16;
    ++v1;
  } while ((int16)(v4 - 39) < 0);
}

void CinematicFunction_Intro_Func149(void) {  // 0x8BE812
  int16 v2;
  int16 v5;
  int16 v7;
  int16 v8;
  int16 v11;
  int16 v12;
  int16 v17;
  OamEnt *v18;
  int16 v19;
  int16 v20;
  int16 v22;

  if (enemy_projectile_pre_instr[1]) {
    R22_ = 40;
    uint16 v0 = 0xe0c;
    do {
      uint8 *v3 = RomPtr_RAM(v0);
      if (*(int16 *)v3 < 0) {
        uint8 *v1 = RomPtr_RAM(v0);
        v2 = *((uint16 *)v1 + 5) - 1;
        *((uint16 *)v1 + 5) = v2;
        if (v2 < 0) {
          *((uint16 *)v1 + 5) = 32;
          *(uint16 *)v1 = *v1;
        }
      } else {
        int v4 = (uint16)(8 * (uint8) * (uint16 *)v3) >> 1;
        if (sign16((*(uint16 *)v3 & 0xFF00) - 1024)) {
          *((uint16 *)v3 + 6) += g_word_8BE9CF[v4];
          v5 = *((uint16 *)v3 + 7);
        } else {
          *((uint16 *)v3 + 6) += g_word_8BE9CF[v4] + g_word_8BE9CF[v4];
          v5 = g_word_8BE9CF[v4 + 1] + *((uint16 *)v3 + 7);
        }
        *((uint16 *)v3 + 7) = g_word_8BE9CF[v4 + 1] + v5;
        uint16 v23 = *((uint16 *)v3 + 6);
        uint16 v6 = HIBYTE(v23);
        if ((v23 & 0x8000u) != 0)
          v6 |= 0xFF00u;
        R18_ = v6;
        LOBYTE(v7) = HIBYTE(v23);
        HIBYTE(v7) = v23;
        R20_ = v7 & 0xFF00;
        v8 = *((uint16 *)v3 + 2);
        bool v9 = __CFADD__uint16(R20_, v8);
        *((uint16 *)v3 + 2) = R20_ + v8;
        *((uint16 *)v3 + 1) += R18_ + v9;
        uint16 v24 = *((uint16 *)v3 + 7);
        uint16 v10 = HIBYTE(v24);
        if ((v24 & 0x8000u) != 0)
          v10 |= 0xFF00u;
        R18_ = v10;
        LOBYTE(v11) = HIBYTE(v24);
        HIBYTE(v11) = v24;
        R20_ = v11 & 0xFF00;
        v12 = *((uint16 *)v3 + 4);
        v9 = __CFADD__uint16(R20_, v12);
        *((uint16 *)v3 + 4) = R20_ + v12;
        *((uint16 *)v3 + 3) += R18_ + v9;
      }
      v0 += 16;
      --R22_;
    } while (R22_);
    R22_ = 40;
    uint16 v13 = oam_next_ptr;
    uint16 v14 = 3596;
    do {
      uint8 *v15 = RomPtr_RAM(v14);
      uint8 *v16 = v15;
      if (*(int16 *)v15 >= 0) {
        v17 = *((uint16 *)v15 + 1);
        if (v15[3]
            || ((v17 - 4) & 0xFF00) != 0
            || (v18 = gOamEnt(v13), *(uint16 *)&v18->xcoord = v17 - 4, v19 = *((uint16 *)v16 + 3), v16[7])
            || ((v19 - 4) & 0xFF00) != 0) {
          uint8 *v21 = RomPtr_RAM(v14);
          *((uint16 *)v21 + 5) = 32;
          *((uint16 *)v21 + 1) = 128;
          *((uint16 *)v21 + 3) = 128;
          *((uint16 *)v21 + 2) = 0;
          *((uint16 *)v21 + 4) = 0;
          *((uint16 *)v21 + 6) = 0;
          *((uint16 *)v21 + 7) = 0;
          *(uint16 *)v21 = *v21;
        } else {
          *(uint16 *)&v18->ycoord = v19 - 4;
          v20 = *((uint16 *)v16 + 5) - 1;
          *((uint16 *)v16 + 5) = v20;
          if (v20 <= 0) {
            v22 = *(uint16 *)v16;
            *((uint16 *)v16 + 5) = g_word_8BE9CF[((uint16)(8 * (uint8) * (uint16 *)v16) >> 1) + 2];
            *(uint16 *)v16 = v22 + 512;
          }
          if (v16[1]) {
            *(uint16 *)&gOamEnt(v13)->charnum = g_word_8BE9A7[(uint16)HIBYTE(*(uint16 *)v16) >> 1];
            v13 += 4;
          }
        }
      }
      v14 += 16;
      --R22_;
    } while (R22_);
    oam_next_ptr = v13;
  }
}

void CinematicFunction_Intro_Func150(uint16 j) {  // 0x8BEF99
  int v1 = j >> 1;
  cinematicbg_arr8[v1] = 128;
  cinematicbg_arr7[v1] = 128;
  cinematicbg_arr9[v1] = 3584;
  cinematicspr_goto_timer[v1] = 260;
}

void CinematicFunction_Intro_Func151(uint16 k) {  // 0x8BEFB2
  int v1 = k >> 1;
  uint16 v2 = cinematicspr_arr7[v1];
  cinematicspr_arr7[v1] = v2 + 128;
  cinematicbg_arr8[v1] += __CFADD__uint16(v2, 128);
  cinematicspr_arr6[v1] = cinematicspr_arr6[v1];
  uint16 v3 = cinematicbg_arr7[v1] + 1;
  cinematicbg_arr7[v1] = v3;
  if (!sign16(v3 - 272)) {
    cinematicspr_instr_timer[v1] = 1;
    cinematicspr_instr_ptr[v1] = addr_off_8BEE9B;
  }
}

void CinematicFunction_Intro_Func152(uint16 j) {  // 0x8BEFEA
  cinematicbg_arr7[j >> 1] = 128;
  int v1 = j >> 1;
  cinematicbg_arr8[v1] = 128;
  cinematicbg_arr9[v1] = 3584;
}

void CinematicFunction_Intro_Func153(uint16 j) {  // 0x8BEFFD
  cinematicbg_arr7[j >> 1] = -128;

  int v1 = j >> 1;
  cinematicbg_arr8[v1] = 128;
  cinematicbg_arr9[v1] = 3584;
}

void CinematicFunction_Intro_Func154(uint16 j) {  // 0x8BF005
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 128;
  cinematicbg_arr8[v1] = 128;
  cinematicbg_arr9[v1] = 2560;
}

void CinematicFunction_Intro_Func155(uint16 j) {  // 0x8BF018
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 128;
  cinematicbg_arr8[v1] = 128;
  cinematicbg_arr9[v1] = 3072;
}

void CinematicFunction_Intro_Func156(uint16 j) {  // 0x8BF02B
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 128;
  cinematicbg_arr8[v1] = 96;
  cinematicbg_arr9[v1] = 1024;
}

void CinematicFunction_Intro_Func157(uint16 j) {  // 0x8BF03E
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 128;
  cinematicbg_arr8[v1] = 160;
  cinematicbg_arr9[v1] = 512;
}

void CinematicFunction_Intro_Func158(uint16 j) {  // 0x8BF051
  int v1 = j >> 1;
  cinematicbg_arr8[v1] = 160;
  cinematicbg_arr9[v1] = 0;
}

void CinematicFunction_Intro_Func159(uint16 j) {  // 0x8BF05E
  CinematicFunction_Intro_Func160(j, substate);
  cinematicbg_arr7[j >> 1] = 156;
  CinematicFunction_Intro_Func158(j);
}

void CinematicFunction_Intro_Func161(uint16 j) {  // 0x8BF06D
  CinematicFunction_Intro_Func160(j, suit_pickup_light_beam_pos);
  cinematicbg_arr7[j >> 1] = 164;
  CinematicFunction_Intro_Func158(j);
}

void CinematicFunction_Intro_Func162(uint16 j) {  // 0x8BF07C
  cinematicbg_arr7[j >> 1] = 172;
  CinematicFunction_Intro_Func158(j);
}

void CinematicFunction_Intro_Func163(uint16 j) {  // 0x8BF085
  CinematicFunction_Intro_Func160(j, *(uint16 *)&suit_pickup_color_math_R);
  cinematicbg_arr7[j >> 1] = 180;
  CinematicFunction_Intro_Func158(j);
}

void CinematicFunction_Intro_Func164(uint16 j) {  // 0x8BF094
  CinematicFunction_Intro_Func160(j, *(uint16 *)&suit_pickup_color_math_B);
  cinematicbg_arr7[j >> 1] = 188;
  CinematicFunction_Intro_Func158(j);
}

void CinematicFunction_Intro_Func160(uint16 j, uint16 a) {  // 0x8BF0A3
  R18_ = 8 * a;
  cinematicspr_instr_ptr[j >> 1] += 8 * a;
}

void CinematicFunction_Intro_Func165(uint16 j) {  // 0x8BF0B2
  cinematicbg_arr8[j >> 1] = -96;
  CinematicFunction_F0B8(j);
}

void CinematicFunction_F0B8(uint16 j) {  // 0x8BF0B8
  int v1 = j >> 1;
  if (cinematic_spawn_param) {
    cinematicbg_arr7[v1] = 384;
    cinematicbg_arr9[v1] = 2560;
    cinematicspr_instr_ptr[v1] += 4;
  } else {
    cinematicbg_arr7[v1] = 128;
    cinematicbg_arr9[v1] = 2560;
  }
}

void CinematicFunction_Intro_Func166(uint16 j) {  // 0x8BF0E1
  cinematicbg_arr8[j >> 1] = -32;
  CinematicFunction_F0B8(j);
}

void CinematicFunction_Intro_Func167(uint16 j) {  // 0x8BF0E9
  cinematicbg_arr8[j >> 1] = 288;
  CinematicFunction_F0B8(j);
}

void CinematicFunction_Intro_Func168(uint16 j) {  // 0x8BF0F1
  cinematicbg_arr8[j >> 1] = 352;
  CinematicFunction_F0B8(j);
}

void CinematicFunction_Intro_Func169(uint16 j) {  // 0x8BF0F9
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 320;
  if (cinematic_spawn_param)
    cinematicbg_arr8[v1] = 448;
  else
    cinematicbg_arr8[v1] = 192;
  cinematicbg_arr9[v1] = 2560;
}

void CinematicFunction_Intro_Func170(uint16 j) {  // 0x8BF11E
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = -64;
  if (cinematic_spawn_param)
    cinematicbg_arr8[v1] = -192;
  else
    cinematicbg_arr8[v1] = 64;
  cinematicbg_arr9[v1] = 2560;
}

void CinematicFunction_Intro_Func171(uint16 j) {  // 0x8BF143
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 120;
  cinematicbg_arr8[v1] = 136;
  cinematicbg_arr9[v1] = 2560;
}

void CinematicFunction_Intro_Func172(uint16 j) {  // 0x8BF156
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 120;
  cinematicbg_arr8[v1] = 152;
  cinematicbg_arr9[v1] = 3072;
}

void CinematicFunction_Intro_Func173(uint16 j) {  // 0x8BF169
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 124;
  cinematicbg_arr8[v1] = 108;
  cinematicbg_arr9[v1] = 3072;
}

void CinematicFunction_Intro_Func174(uint16 j) {  // 0x8BF17C
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 121;
  cinematicbg_arr8[v1] = 107;
  cinematicbg_arr9[v1] = 2560;
}

void CinematicFunction_Intro_Func175(uint16 j) {  // 0x8BF18F
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 530;
  cinematicbg_arr8[v1] = 231;
  cinematicbg_arr9[v1] = 3584;
  cinematicspr_goto_timer[v1] = 8;
}

void CinematicFunction_Intro_Func176(uint16 j) {  // 0x8BF1A8
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 246;
  cinematicbg_arr8[v1] = 519;
  cinematicbg_arr9[v1] = 3584;
  cinematicspr_goto_timer[v1] = 8;
}

void CinematicFunction_Intro_Func177(uint16 j) {  // 0x8BF1C1
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 385;
  cinematicbg_arr8[v1] = 366;
  cinematicbg_arr9[v1] = 3584;
}

void CinematicFunction_Intro_Func178(uint16 j) {  // 0x8BF1D4
  int v1 = j >> 1;
  cinematicbg_arr7[v1] = 391;
  cinematicbg_arr8[v1] = 384;
  cinematicbg_arr9[v1] = 3584;
}

void CinematicFunction_Intro_Func179(uint16 k) {  // 0x8BF1E7
  int v1 = k >> 1;
  cinematicbg_arr8[v1] += cinematicspr_goto_timer[v1];
  uint16 v2 = cinematicbg_arr7[v1] - cinematicspr_goto_timer[v1];
  cinematicbg_arr7[v1] = v2;
  if (sign16(v2 - 395)) {
    cinematicbg_arr7[v1] = 394;
    cinematicbg_arr8[v1] = 367;
    cinematicspr_preinstr_func[v1] = FUNC16(CinematicFunction_nullsub_116);
    SpawnPalfxObject(addr_kPalfx_E200);
  } else {
    cinematicspr_goto_timer[v1] += 2;
  }
}

void CinematicFunction_Intro_Func180(uint16 k) {  // 0x8BF227
  int v1 = k >> 1;
  cinematicbg_arr8[v1] -= cinematicspr_goto_timer[v1];
  uint16 v2 = cinematicspr_goto_timer[v1] + cinematicbg_arr7[v1];
  cinematicbg_arr7[v1] = v2;
  if (sign16(v2 - 382)) {
    cinematicspr_goto_timer[v1] += 2;
  } else {
    cinematicbg_arr7[v1] = 382;
    cinematicbg_arr8[v1] = 383;
    cinematicspr_preinstr_func[v1] = FUNC16(CinematicFunction_nullsub_116);
  }
}

uint16 CinematicSprInstr_Func181(uint16 k, uint16 j) {  // 0x8BF25E
  reg_TM = 3;
  reg_TS = 16;
  reg_CGWSEL = 2;
  reg_CGADSUB = 34;
  reg_BG12NBA = 100;
  reg_BG2SC = 84;
  cinematic_var4 = 0;
  cinematic_function = FUNC16(CinematicFunction_Intro_Func144);
  return j;
}

uint16 CinematicSprInstr_Func182(uint16 k, uint16 j) {  // 0x8BF284
  SpawnPalfxObject(addr_kPalfx_E1C4);
  palette_buffer[254] = 1;
  return j;
}

uint16 CinematicSprInstr_Func183(uint16 k, uint16 j) {  // 0x8BF295
  uint16 v0 = 0;
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEEB5, v0);
  palette_buffer[0] = 0x7FFF;
  return j;
}

void CinematicFunction_Intro_Func184(uint16 j) {  // 0x8BF2A5
  if (!cinematicspr_instr_ptr[15]) {
    int v2 = j >> 1;
    cinematicspr_instr_timer[v2] = 1;
    cinematicspr_instr_ptr[v2] = addr_off_8BEE9B;
  }
}

uint16 CinematicSprInstr_Func185(uint16 k, uint16 j) {  // 0x8BF2B7
  uint16 v0;

  reg_TM = 17;
  reg_TS = 2;
  reg_CGWSEL = 2;
  reg_CGADSUB = 17;
  LOBYTE(v0) = 124;
  reg_BG2SC = 124;
  R18_ = 0;
  SpawnCinematicSpriteObjectToR18(addr_kCinematicSpriteObjectDef_8BEEBB, 0);
  R18_ = 2;
  SpawnCinematicSpriteObjectToR18(addr_kCinematicSpriteObjectDef_8BEEF7, 0);
  SpawnPalfxObject(addr_kPalfx_E1C8);
  SpawnPalfxObject(addr_kPalfx_E1D0);
  SpawnPalfxObject(addr_kPalfx_E1E8);
  return j;
}

uint16 CinematicSprInstr_Func186(uint16 k, uint16 j) {  // 0x8BF2FA
  reg_TM = 3;
  reg_TS = 18;
  reg_CGWSEL = 2;
  reg_CGADSUB = 51;
  reg_BG1SC = 116;
  reg_BG2SC = 120;
  SpawnPalfxObject(addr_kPalfx_E1CC);
  R18_ = 6;
  SpawnCinematicSpriteObjectToR18(addr_kCinematicSpriteObjectDef_8BEEC1, 6u);
  return j;
}

uint16 CinematicSprInstr_Func187(uint16 k, uint16 j) {  // 0x8BF32B
  reg_TM = 0;
  reg_TS = 0;
  palette_buffer[0] = 0x7FFF;
  palette_buffer[128] = 0x7FFF;
  for (int i = 30; i >= 0; i -= 2)
    palette_buffer[(i >> 1) + 16] = 0x7FFF;
  cinematic_function = FUNC16(CinematicFunction_Intro_Func119);
  cinematic_var4 = 120;
  return j;
}


void CinematicSprPreInstr_F35A(uint16 k) {  // 0x8BF35A
  if (cinematic_function == (uint16)FUNC16(CinematicFunction_Intro_Func121)) {
    int v1 = k >> 1;
    cinematicspr_preinstr_func[v1] = FUNC16(CinematicFunction_Intro_Func189);
    cinematicspr_arr7[v1] = 0x4000;
    cinematicspr_goto_timer[v1] = 0;
  }
}

void CinematicFunction_Intro_Func189(uint16 k) {  // 0x8BF375
  int v1 = k >> 1;
  uint16 v2 = cinematicspr_arr7[v1];
  cinematicspr_arr7[v1] = v2 - 32;
  cinematicspr_goto_timer[v1] = __CFADD__uint16(v2, -32) + cinematicspr_goto_timer[v1] - 1;
  uint16 v3 = cinematicspr_arr6[v1];
  bool v4 = __CFADD__uint16(cinematicspr_arr7[v1], v3);
  cinematicspr_arr6[v1] = cinematicspr_arr7[v1] + v3;
  cinematicbg_arr7[v1] += cinematicspr_goto_timer[v1] + v4;
  CinematicFunction_Intro_Func189b(k);
}

void CinematicFunction_Intro_Func189b(uint16 k) {  // 0x8BF39B
  if (cinematic_function == FUNC16(nullsub_126)) {
    int v5 = k >> 1;
    cinematicspr_instr_timer[v5] = 1;
    cinematicspr_instr_ptr[v5] = addr_off_8BEE9B;
  }
}

uint16 CinematicSprInstr_Func190(uint16 k, uint16 j) {  // 0x8BF3B0
  uint16 v0 = 0;
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEECD, v0);
  return j;
}

void CinematicFunction_Intro_Func191(uint16 k) {  // 0x8BF3B9
  if (cinematic_function == (uint16)FUNC16(CinematicFunction_Intro_Func126)) {
    int v1 = k >> 1;
    cinematicspr_instr_timer[v1] = 1;
    cinematicspr_instr_ptr[v1] = addr_off_8BEE9B;
  }
}

uint16 CinematicSprInstr_Func192(uint16 k, uint16 j) {  // 0x8BF3CE
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEEFD, 0);

  int hours1 = game_time_hours / 10;
  int hours2 = game_time_hours % 10;
  substate = hours1;
  suit_pickup_light_beam_pos = hours2;

  int min1 = game_time_minutes / 10;
  int min2 = game_time_minutes % 10;

  *(uint16 *)&suit_pickup_color_math_R = min1;
  *(uint16 *)&suit_pickup_color_math_B = min2;
  return j;
}

uint16 CinematicSprInstr_Func193(uint16 k, uint16 j) {  // 0x8BF41B
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF03, 0);
  return j;
}

uint16 CinematicSprInstr_Func194(uint16 k, uint16 j) {  // 0x8BF424
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF09, 0);
  return j;
}

uint16 CinematicSprInstr_Func195(uint16 k, uint16 j) {  // 0x8BF42D
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF0F, 0);
  return j;
}

uint16 CinematicSprInstr_Func196(uint16 k, uint16 j) {  // 0x8BF436
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF15, 0);
  return j;
}

uint16 CinematicSprInstr_Func197(uint16 k, uint16 j) {  // 0x8BF43F
  SpawnCinematicSpriteObject(addr_kCinematicSpriteObjectDef_8BEF1B, 0);
  return j;
}

uint16 CinematicSprInstr_Func198(uint16 k, uint16 j) {  // 0x8BF448
  screen_fade_delay = 0;
  screen_fade_counter = 0;
  cinematic_function = FUNC16(CinematicFunction_Intro_Func125);
  return j;
}

void CinematicFunction_Intro_Func199(uint16 k) {  // 0x8BF455
  if (sign16(cinematic_var6 - 176))
    cinematicspr_preinstr_func[k >> 1] = FUNC16(CinematicFunction_Intro_Func200);
}

void CinematicFunction_Intro_Func200(uint16 k) {  // 0x8BF464
  int v1 = k >> 1;
  cinematicspr_arr7[v1] = cinematicspr_arr7[v1];
  ++cinematicbg_arr8[v1];
}

void CinematicFunction_Intro_Func201(uint16 k) {  // 0x8BF478
  if (sign16(cinematic_var6 - 176))
    cinematicspr_preinstr_func[k >> 1] = FUNC16(CinematicFunction_Intro_Func202);
}

void CinematicFunction_Intro_Func202(uint16 k) {  // 0x8BF487
  int v1 = k >> 1;
  cinematicspr_arr7[v1] = cinematicspr_arr7[v1];
  --cinematicbg_arr8[v1];
}

void CinematicFunction_Intro_Func203(uint16 k) {  // 0x8BF49B
  if (!sign16(cinematic_var6 - 96))
    cinematicspr_preinstr_func[k >> 1] = FUNC16(CinematicFunction_Intro_Func204);
}

void CinematicFunction_Intro_Func204(uint16 k) {  // 0x8BF4AA
  int v1 = k >> 1;
  cinematicspr_arr7[v1] = cinematicspr_arr7[v1];
  cinematicbg_arr8[v1] -= 2;
  cinematicspr_arr6[v1] = cinematicspr_arr6[v1];
  --cinematicbg_arr7[v1];
}

void CinematicFunction_Intro_Func205(uint16 k) {  // 0x8BF4D1
  if (!sign16(cinematic_var6 - 96))
    cinematicspr_preinstr_func[k >> 1] = FUNC16(CinematicFunction_Intro_Func206);
}

void CinematicFunction_Intro_Func206(uint16 k) {  // 0x8BF4E0
  int v1 = k >> 1;
  cinematicspr_arr7[v1] = cinematicspr_arr7[v1];
  cinematicbg_arr8[v1] += 2;
  cinematicspr_arr6[v1] = cinematicspr_arr6[v1];
  ++cinematicbg_arr7[v1];
}

void CinematicFunction_Intro_Func207(uint16 k) {  // 0x8BF507
  if (cinematic_function == (uint16)FUNC16(nullsub_125)) {
    int v1 = k >> 1;
    cinematicspr_instr_timer[v1] = 1;
    cinematicspr_instr_ptr[v1] = addr_off_8BEE9B;
  }
}

uint16 CinematicSprInstr_Func208(uint16 k, uint16 j) {  // 0x8BF51D
  R18_ = 0;
  SpawnCinematicSpriteObjectToR18(addr_kCinematicSpriteObjectDef_8BEF3F, 0);
  return j;
}

void CinematicSprPreInstr_F528(uint16 k) {  // 0x8BF528
  CinematicFunction_Intro_Func210_EndingSamusFall(k);
  int v1 = k >> 1;
  if (sign16(cinematicbg_arr8[v1] + 80)) {
    reg_OBSEL = 3;
    cinematicspr_instr_timer[v1] = 1;
    cinematicspr_instr_ptr[v1] = addr_word_8BED95;
    cinematicspr_preinstr_func[v1] = FUNC16(CinematicFunction_Intro_Func215);
    cinematicbg_arr9[v1] = 3072;
  }
}

uint16 CinematicSprInstr_Func211(uint16 k, uint16 j) {  // 0x8BF554
  R18_ = 2;
  if (sign16(game_time_hours - 10))
    SpawnCinematicSpriteObjectToR18(addr_kCinematicSpriteObjectDef_8BEF7B, 2);
  else
    SpawnCinematicSpriteObjectToR18(addr_kCinematicSpriteObjectDef_8BEF75, 2);
  R18_ = 0;
  SpawnCinematicSpriteObjectToR18(addr_kCinematicSpriteObjectDef_8BEF6F, 0);
  return j;
}

void CinematicSprPreInstr_F57F(uint16 k) {  // 0x8BF57F
  CinematicFunction_Intro_Func210_EndingSamusFall(k);
  int v1 = k >> 1;
  if (sign16(cinematicbg_arr8[v1] + 80)) {
    cinematicspr_instr_timer[v1] = 1;
    cinematicspr_instr_ptr[v1] = addr_off_8BEE9B;
  }
}

uint16 CinematicSprInstr_Func213(uint16 k, uint16 j) {  // 0x8BF597
  int v1 = k >> 1;
  if (sign16(game_time_hours - 10))
    cinematicbg_arr7[v1 + 1] = 120;
  else
    cinematicbg_arr7[v1 + 1] = 118;
  cinematicbg_arr8[v1 + 1] = 120;
  return j;
}

uint16 CinematicSprInstr_Func214(uint16 k, uint16 j) {  // 0x8BF5BA
  int v1 = k >> 1;
  if (sign16(game_time_hours - 10)) {
    cinematicbg_arr7[v1 + 1] = 121;
    cinematicbg_arr8[v1 + 1] = 116;
  } else {
    cinematicbg_arr7[v1 + 1] = 120;
    cinematicbg_arr8[v1 + 1] = 114;
  }
  return j;
}

void CinematicFunction_Intro_Func215(uint16 k) {  // 0x8BF5DD
  CinematicFunction_Intro_Func216();
  CinematicFunction_Intro_Func210_EndingSamusFall(k);
  int v1 = k >> 1;
  if (!sign16(cinematicbg_arr8[v1] - 136)) {
    cinematicbg_arr8[v1] = 136;
    cinematicspr_instr_timer[v1] = 1;
    cinematicspr_instr_ptr[v1] = addr_word_8BED9D;
    cinematicspr_preinstr_func[v1] = FUNC16(CinematicFunction_nullsub_116);
  }
}

uint16 CinematicSprInstr_Func217(uint16 k, uint16 j) {  // 0x8BF604
  cinematicbg_arr9[k >> 1] = 3584;
  cinematic_function = FUNC16(CinematicFunction_Intro_Func139);
  reg_TM = 17;
  reg_TS = 16;
  reg_CGWSEL = 2;
  reg_CGADSUB = -127;
  cinematic_var8 = -4;
  cinematic_var10 = -8;
  cinematic_var4 = 16;
  cinematic_var14 = 32;
  cinematic_var17 = 32;
  CopyPalettesToFadingPalettes();
  DecomposePaletteDataForFading();
  QueueSfx1_Max15(0x22u);
  return j;
}

uint16 CinematicSprInstr_Func218(uint16 k, uint16 j) {  // 0x8BF651
  samus_y_subspeed = 0;
  samus_y_speed = -16;
  return j;
}

void CinematicFunction_Intro_Func210_EndingSamusFall(uint16 k) {  // 0x8BF65B
  uint16 v1 = (__PAIR32__(samus_y_speed, samus_y_subspeed) + 14336) >> 16;
  samus_y_subspeed += 14336;
  samus_y_speed = v1;
  int v2 = k >> 1;
  uint16 v3 = cinematicspr_arr7[v2];
  bool v4 = __CFADD__uint16(samus_y_subspeed, v3);
  cinematicspr_arr7[v2] = samus_y_subspeed + v3;
  cinematicbg_arr8[v2] += samus_y_speed + v4;
}

void CinematicFunction_Intro_Func216(void) {  // 0x8BF682
  if (sign16(cinematic_var14 - 16)) {
    uint16 v0 = 2 * cinematic_var14;
    uint16 v1 = vram_write_queue_tail;
    gVramWriteEntry(vram_write_queue_tail)->size = 2048;
    v1 += 2;
    int v2 = v0 >> 1;
    gVramWriteEntry(v1)->size = g_word_8BF6B8[v2];
    v1 += 2;
    LOBYTE(gVramWriteEntry(v1++)->size) = 127;
    gVramWriteEntry(v1)->size = g_word_8BF6D8[v2];
    vram_write_queue_tail = v1 + 2;
    ++cinematic_var14;
  }
}

uint16 CinematicFunction_Intro_Func219(uint16 k, uint16 j) {  // 0x8BF6FE
  DisableTextGlowObjects_();
  uint16 v0 = 8;
  do {
    palette_buffer[v0 >> 1] = kPalettes_Intro4[v0 >> 1];
    v0 += 2;
  } while ((int16)(v0 - 512) < 0);
  reg_TM = 0;
  reg_INIDISP = 0x80;
  screen_fade_delay = 0;
  screen_fade_counter = 0;
  cinematic_var9 = 0;
  cinematic_var10 = 0;
  enemy_projectile_pre_instr[1] = 1;
  cinematic_function = FUNC16(CinematicFunction_Intro_Func129);
  cinematic_var4 = 60;
  return j;
}
