// System routines 

#include "sm_rtl.h"
#include "ida_types.h"
#include "variables.h"
#include "funcs.h"
#include "enemy_types.h"
#include "spc_player.h"


#define kMusicPointers (*(LongPtr*)RomFixedPtr(0x8fe7e1))
#define kTimerDigitsSpritemapPtr ((uint16*)RomFixedPtr(0x809fd4))
#define kLoadStationLists ((uint16*)RomFixedPtr(0x80c4b5))
#define off_80CD46 ((uint16*)RomFixedPtr(0x80cd46))




void APU_UploadBank(void) {  // 0x808028
  if (!g_use_my_apu_code)
    return;
  RtlApuUpload(RomPtr(Load24(&R0_)));
}

uint16 NextRandom(void) {  // 0x808111
  uint16 RegWord = LOBYTE(random_number) * 5;
  uint8 Reg = HIBYTE(random_number) * 5;

  int carry = HIBYTE(RegWord) + Reg + 1;
  HIBYTE(RegWord) = carry;
  uint16 result = (carry >> 8) + RegWord + 17;
  random_number = result;
  return result;
}

void ReleaseButtonsFilter(uint16 v0) {  // 0x808146
  timed_held_input_timer_reset = v0;
  bool v1 = ((uint16)~joypad1_newkeys & joypad1_lastkeys) == joypad_released_keys;
  joypad_released_keys = ~joypad1_newkeys & joypad1_lastkeys;
  if (!v1) {
    timed_held_input_timer = timed_held_input_timer_reset;
    timed_held_input = 0;
    goto LABEL_6;
  }
  if ((--timed_held_input_timer & 0x8000u) == 0) {
    timed_held_input = 0;
    goto LABEL_6;
  }
  timed_held_input_timer = 0;
  previous_timed_held_input = timed_held_input;
  timed_held_input = ~joypad1_newkeys & joypad1_lastkeys;
LABEL_6:
  newly_held_down_timed_held_input = timed_held_input & (previous_timed_held_input ^ timed_held_input);
}

uint16 PrepareBitAccess(uint16 a) {  // 0x80818E
  bitmask = 1 << (a & 7);
  return a >> 3;
}

void SetBossBitForCurArea(uint16 a) {  // 0x8081A6
  LOBYTE(bitmask) = a;
  boss_bits_for_area[area_index] |= a;
}

void ClearBossBitForCurArea(uint16 a) {  // 0x8081C0
  LOBYTE(bitmask) = ~a;
  boss_bits_for_area[area_index] &= ~a;
}

uint8 CheckBossBitForCurArea(uint16 a) {  // 0x8081DC
  LOBYTE(bitmask) = a;
  return ((uint8)a & boss_bits_for_area[area_index]) != 0;
}

void SetEventHappened(uint16 a) {  // 0x8081FA
  uint16 v1 = PrepareBitAccess(a);
  events_that_happened[v1] |= bitmask;
}

void ClearEventHappened(uint16 v0) {  // 0x808212
  uint16 v1 = PrepareBitAccess(v0);
  bitmask = ~bitmask;
  events_that_happened[v1] &= bitmask;
}

uint16 CheckEventHappened(uint16 a) {  // 0x808233
  uint16 idx = PrepareBitAccess(a);
  return (bitmask & events_that_happened[idx]) != 0;
}

void CopySuperMetroidString(void) {  // 0x80824F
  memcpy(&g_sram[0x1fe0], "supermetroid", 12);
  RtlWriteSram();
}

void VerifySRAM(void) {  // 0x808261
  num_demo_sets = 3;
  if (LoadFromSram(0) && LoadFromSram(1) && LoadFromSram(2)) {
    memcpy(&g_sram[0x1fe0], "madadameyohn", 12);
    RtlWriteSram();
  } else {
    if (!memcmp(&g_sram[0x1fe0], "supermetroid", 12))
      num_demo_sets = 4;
  }
}

void Multiply16x16(uint16 a, uint16 j) {  // 0x8082D6
  uint32 result = (uint32)a * (uint32)j;
  mult_product_lo = result;
  mult_product_hi = result >> 16;
}

CoroutineRet WaitForNMI_Async(void) {  // 0x808338
  // Return 0 from this routine as soon as the coroutine has finished
  if (coroutine_completion_flags) {
    coroutine_completion_flags = 0;
    return 0;
  }
  waiting_for_nmi = 1;
  coroutine_completion_flags = 1;
  return 1;
}

CoroutineRet WaitForNMI_NoUpdate_Async(void) {
  // Return 0 from this routine as soon as the coroutine has finished
  if (coroutine_completion_flags) {
    coroutine_completion_flags = 0;
    return 0;
  }
  coroutine_completion_flags = 1;
  return 1;
}

void EnableNMI(void) {  // 0x80834B
  uint8 v0 = reg_NMITIMEN | 0x80;
  WriteReg(NMITIMEN, reg_NMITIMEN | 0x80);
  reg_NMITIMEN = v0;
}

void DisableNMI(void) {  // 0x80835D
  uint8 v0 = reg_NMITIMEN & 0x7F;
  WriteReg(NMITIMEN, reg_NMITIMEN & 0x7F);
  reg_NMITIMEN = v0;
}

void ScreenOff(void) {
  reg_INIDISP |= 0x80;
}

void ScreenOn(void) {
  reg_INIDISP &= ~0x80;
}

void memset7E(uint16 *k, uint16 a, uint16 j) {  // 0x8083F6
  do {
    *k++ = a;
  } while (j -= 2);
}

CoroutineRet Vector_RESET_Async(void) {  // 0x80841C
  COROUTINE_BEGIN(coroutine_state_0, 1)
  WriteReg(MEMSEL, 1u);
  reg_MEMSEL = 1;
  // Removed code to wait 4 frames
  memset(g_ram, 0, 8192);
  COROUTINE_AWAIT(2, InitializeIoDisplayLogo_Async());
  COROUTINE_MANUAL_POS(3); // Soft reset position
  mov24(&R0_, 0xCF8000);
  APU_UploadBank();
  WriteReg(INIDISP, 0x8F);
  memset(g_ram, 0, 0x10000);
  WriteReg(NMITIMEN, 0);
  reg_NMITIMEN = 0;
  reg_INIDISP = 0x8f;
  InitializeCpuIoRegs();
  InitializePpuIoRegs();
  WriteLotsOf0x1c2f();
  sfx_readpos[0] = 0;
  sfx_readpos[1] = 0;
  sfx_readpos[2] = 0;
  sfx_writepos[0] = 0;
  sfx_writepos[1] = 0;
  sfx_writepos[2] = 0;
  sfx_state[0] = 0;
  sfx_state[1] = 0;
  sfx_state[2] = 0;
  sfx_cur[0] = 0;
  sfx_cur[1] = 0;
  sfx_cur[2] = 0;
  sfx_clear_delay[0] = 0;
  sfx_clear_delay[1] = 0;
  sfx_clear_delay[2] = 0;
  sfx1_queue[0] = 0;
  sfx2_queue[0] = 0;
  sfx3_queue[0] = 0;
  oam_next_ptr = 0;
  reg_OAMaddr_UNUSED = 0;
  ClearOamExt();
  ClearUnusedOam();
  nmi_copy_samus_halves = 0;
  nmi_copy_samus_top_half_src = 0;
  nmi_copy_samus_bottom_half_src = 0;
  EnableNMI();
  RtlApuWrite(APUI00, 0);
  RtlApuWrite(APUI02, 0);

  // Removed code to wait 4 frames
  random_number = 97;
  music_timer = 0;
  music_queue_delay[0] = 0;
  music_queue_delay[1] = 0;
  music_queue_delay[2] = 0;
  music_queue_delay[3] = 0;
  music_queue_delay[4] = 0;
  music_queue_delay[5] = 0;
  music_queue_delay[6] = 0;
  music_queue_delay[7] = 0;
  enable_debug = 0;
  VerifySRAM();
  debug_disable_sounds = 0;
  sound_handler_downtime = 0;
  COROUTINE_END(2);
}

void InvalidInterrupt_Crash(void) {  // 0x808573
  printf("InvalidInterrupt_Crash\n");
  Unreachable();
  for (;;);
}

void LoadMirrorOfExploredMapTiles(void) {  // 0x80858C
  uint16 v1 = swap16(area_index);
  uint16 v2 = 0;
  do {
    *(uint16 *)&map_tiles_explored[v2] = explored_map_tiles_saved[v1 >> 1];
    v1 += 2;
    v2 += 2;
  } while ((int16)(v2 - 256) < 0);
  has_area_map = map_station_byte_array[area_index];
}

void SaveExploredMapTilesToSaved(void) {  // 0x8085C6
  uint16 v1 = swap16(area_index);
  uint16 v2 = 0;
  do {
    explored_map_tiles_saved[v1 >> 1] = *(uint16 *)&map_tiles_explored[v2];
    v1 += 2;
    v2 += 2;
  } while ((int16)(v2 - 256) < 0);
  if (has_area_map)
    *(uint16 *)&map_station_byte_array[area_index] |= 0xFFu;
}

void InitializeCpuIoRegs(void) {  // 0x80875D
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

void InitializePpuIoRegs(void) {  // 0x808792
  WriteReg(INIDISP, 0x8F);
  reg_INIDISP = 0x8f;
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
  WriteReg(BG1SC, 0x40);
  reg_BG1SC = 64;
  WriteReg(BG2SC, 0x44);
  reg_BG2SC = 68;
  WriteReg(BG3SC, 0x48);
  reg_BG3SC = 72;
  WriteReg(BG4SC, 0);
  reg_BG4SC = 0;
  WriteReg(BG12NBA, 0);
  reg_BG12NBA = 0;
  WriteReg(BG34NBA, 5u);
  reg_BG34NBA = 5;
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
  WriteReg(WH1, 0xF8);
  reg_WH1 = -8;
  WriteReg(WH2, 0);
  reg_WH2 = 0;
  WriteReg(WH3, 0);
  reg_WH3 = 0;
  WriteReg(WBGLOG, 0);
  reg_WBGLOG = 0;
  WriteReg(WOBJLOG, 0);
  reg_WOBJLOG = 0;
  WriteReg(TM, 0x11);
  reg_TM = 17;
  WriteReg(TMW, 0x11);
  reg_TMW = 17;
  WriteReg(TS, 2u);
  reg_TS = 2;
  WriteReg(TSW, 2u);
  reg_TSW = 2;
  WriteReg(CGWSEL, 2u);
  next_gameplay_CGWSEL = 2;
  WriteReg(CGADSUB, 0xA1);
  next_gameplay_CGADSUB = -95;
  WriteReg(COLDATA, 0xE0);
  WriteReg(COLDATA, 0xE0);
  WriteReg(COLDATA, 0x80);
  reg_COLDATA[0] = 0x80;
  WriteReg(COLDATA, 0x40);
  reg_COLDATA[1] = 64;
  WriteReg(COLDATA, 0x20);
  reg_COLDATA[2] = 32;
  WriteReg(SETINI, 0);
  reg_SETINI = 0;
}

void WriteLotsOf0x1c2f(void) {  // 0x8088D1
  sub_8088EB(0x1C2Fu);
  sub_8088FE(0x1C2Fu);
  sub_808911(0x1C2Fu);
}

void sub_8088EB(uint16 a) {  // 0x8088EB
  memset7E((uint16*)(g_ram + 0x3000), a, 0x800);
}

void sub_8088FE(uint16 a) {  // 0x8088FE
  memset7E((uint16 *)(g_ram + 0x4000), a, 0x800);
}

void sub_808911(uint16 a) {  // 0x808911
  memset7E((uint16 *)(g_ram + 0x6000), a, 0x800);
}

void HandleFadeOut(void) {  // 0x808924
  if ((int16)(screen_fade_counter - 1) < 0) {
    screen_fade_counter = screen_fade_delay;
    if ((reg_INIDISP & 0xF) != 0) {
      if ((reg_INIDISP & 0xF) == 1)
        reg_INIDISP = 0x80;
      else
        reg_INIDISP = (reg_INIDISP & 0xF) - 1;
    }
  } else {
    --screen_fade_counter;
  }
}

void HandleFadeIn(void) {  // 0x80894D
  if ((int16)(screen_fade_counter - 1) < 0) {
    screen_fade_counter = screen_fade_delay;
    if (((reg_INIDISP + 1) & 0xF) != 0)
      reg_INIDISP = (reg_INIDISP + 1) & 0xF;
  } else {
    --screen_fade_counter;
  }
}

void ClearUnusedOam(void) {
  for (int i = oam_next_ptr >> 2; i < 0x80; i++)
    oam_ent[i].ycoord = 0xf0;
  oam_next_ptr = 0;
}

void ClearOamExt(void) {  // 0x808B1A
  memset(oam_ext, 0, sizeof(oam_ext[0]) * 16);
}

void QueueMode7Transfers(uint8 db, uint16 k) {  // 0x808B4F
  const uint8 *p = RomPtrWithBank(db, k);
  uint16 v2 = mode7_vram_write_queue_tail;
  for (;;) {
    int f = GET_BYTE(p);
    if (f & 0x80) {
      *(uint16 *)(&mode7_write_queue[0].field_0 + v2) = GET_WORD(p);
      *(uint16 *)((char *)&mode7_write_queue[0].field_1 + v2 + 1) = GET_WORD(p + 2);
      *(uint16 *)&mode7_write_queue[0].gap3[v2 + 1] = GET_WORD(p + 4);
      *(uint16 *)&mode7_write_queue[0].gap3[v2 + 3] = GET_WORD(p + 6);
      *(uint16 *)((char *)&mode7_write_queue[1].field_1 + v2) = p[8];
      p += 9, v2 += 9;
    } else if (f & 0x40) {
      *(uint16 *)(&mode7_write_queue[0].field_0 + v2) = GET_WORD(p);
      *(uint16 *)((char *)&mode7_write_queue[0].field_1 + v2 + 1) = GET_WORD(p + 2);
      *(uint16 *)&mode7_write_queue[0].gap3[v2 + 1] = GET_WORD(p + 4);
      *(uint16 *)&mode7_write_queue[0].gap3[v2 + 3] = p[6];
      p += 7, v2 += 7;
    } else {
      break;
    }
  }
  mode7_vram_write_queue_tail = v2;
}

void NMI_ProcessMode7Queue(void) {  // 0x808BBA
  if (mode7_vram_write_queue_tail) {
    NMI_ProcessMode7QueueInner(&g_ram[0x2D0]);
    *(uint16 *)&mode7_write_queue[0].field_0 = 0;
    mode7_vram_write_queue_tail = 0;
  }
}

void NMI_ProcessMode7QueueInner(const uint8 *p) {  // 0x808BD3
  while (1) {
    uint8 v2;
    while (1) {
      v2 = *p;
      if ((v2 & 0x80) == 0)
        break;
      WriteReg(DMAP1, v2 & 0x1F);
      Mode7CgvmWriteQueue *v5 = (Mode7CgvmWriteQueue *)p;
      WriteRegWord(A1T1L, v5->src_addr.addr);
      WriteReg(A1B1, v5->src_addr.bank);
      WriteRegWord(DAS1L, v5->count);
      if (v2 & 0x40)
        WriteReg(BBAD1, 0x19);
      else
        WriteReg(BBAD1, 0x18);
      WriteRegWord(VMADDL, v5->vram_addr);
      WriteReg(VMAIN, v5->vmain);
      WriteReg(MDMAEN, 2u);
      p += sizeof(Mode7CgvmWriteQueue);
    }
    if (!(v2 & 0x40))
      break;
    WriteReg(DMAP1, v2 & 0x1F);
    WriteRegWord(A1T1L, *(uint16 *)(p + 1));
    WriteReg(A1B1, p[3]);
    WriteRegWord(DAS1L, *((uint16 *)p + 2));
    WriteReg(BBAD1, 0x22);
    WriteReg(CGADD, p[6]);
    WriteReg(MDMAEN, 2u);
    p += 7;
  }
}

void NMI_ProcessVramWriteQueue(void) {  // 0x808C83
  if (vram_write_queue_tail) {
    gVramWriteEntry(vram_write_queue_tail)->size = 0;
    WriteRegWord(DMAP1, 0x1801);
    for (int i = 0; ; i += 7) {
      VramWriteEntry *e = gVramWriteEntry(i);
      if (!e->size)
        break;
      WriteRegWord(DAS1L, e->size);
      WriteRegWord(A1T1L, e->src.addr);
      WriteReg(A1B1, e->src.bank);
      WriteRegWord(VMAIN, sign16(e->vram_dst) ? 0x81 : 0x80);
      WriteRegWord(VMADDL, e->vram_dst);
      WriteReg(MDMAEN, 2u);
    }
  }
  vram_write_queue_tail = 0;
  Nmi_ProcessHorizScrollingDma();
  Nmi_ProcessVertScrollingDma();
}

void Nmi_ProcessHorizScrollingDma(void) {  // 0x808CD8
  WriteReg(VMAIN, 0x81);
  if ((uint8)bg1_update_col_enable) {
    LOBYTE(bg1_update_col_enable) = 0;
    uint16 v0 = bg1_update_col_unwrapped_dst;
    WriteRegWord(VMADDL, bg1_update_col_unwrapped_dst);
    WriteRegWord(DMAP1, 0x1801);
    WriteRegWord(A1T1L, ADDR16_OF_RAM(*bg1_column_update_tilemap_left_halves));
    WriteReg(A1B1, 0x7E);
    uint16 v1 = bg1_update_col_unwrapped_size;
    WriteRegWord(DAS1L, bg1_update_col_unwrapped_size);
    WriteReg(MDMAEN, 2u);
    WriteRegWord(VMADDL, v0 + 1);
    WriteRegWord(DAS1L, v1);
    WriteRegWord(A1T1L, ADDR16_OF_RAM(*bg1_column_update_tilemap_right_halves));
    WriteReg(MDMAEN, 2u);
    WriteRegWord(A1T1L, bg1_update_col_wrapped_left_src);
    uint16 v2 = bg1_update_col_wrapped_size;
    if (bg1_update_col_wrapped_size) {
      WriteRegWord(DAS1L, bg1_update_col_wrapped_size);
      uint16 v3 = bg1_update_col_wrapped_dst;
      WriteRegWord(VMADDL, bg1_update_col_wrapped_dst);
      WriteReg(MDMAEN, 2u);
      WriteRegWord(VMADDL, v3 + 1);
      WriteRegWord(DAS1L, v2);
      WriteRegWord(A1T1L, bg1_update_col_wrapped_right_src);
      WriteReg(MDMAEN, 2u);
    }
  }
  if ((uint8)bg2_update_col_enable) {
    LOBYTE(bg2_update_col_enable) = 0;
    uint16 v4 = bg2_update_col_unwrapped_dst;
    WriteRegWord(VMADDL, bg2_update_col_unwrapped_dst);
    WriteRegWord(DMAP1, 0x1801);
    WriteRegWord(A1T1L, ADDR16_OF_RAM(*bg2_column_update_tilemap_left_halves));
    WriteReg(A1B1, 0x7E);
    uint16 v5 = bg2_update_col_unwrapped_size;
    WriteRegWord(DAS1L, bg2_update_col_unwrapped_size);
    WriteReg(MDMAEN, 2u);
    WriteRegWord(VMADDL, v4 + 1);
    WriteRegWord(DAS1L, v5);
    WriteRegWord(A1T1L, ADDR16_OF_RAM(*bg2_column_update_tilemap_right_halves));
    WriteReg(MDMAEN, 2u);
    WriteRegWord(A1T1L, bg2_update_col_wrapped_left_src);
    uint16 v6 = bg2_update_col_wrapped_size;
    if (bg2_update_col_wrapped_size) {
      WriteRegWord(DAS1L, bg2_update_col_wrapped_size);
      uint16 v7 = bg2_update_col_wrapped_dst;
      WriteRegWord(VMADDL, bg2_update_col_wrapped_dst);
      WriteReg(MDMAEN, 2u);
      WriteRegWord(VMADDL, v7 + 1);
      WriteRegWord(DAS1L, v6);
      WriteRegWord(A1T1L, bg2_update_col_wrapped_right_src);
      WriteReg(MDMAEN, 2u);
    }
  }
}

void Nmi_ProcessVertScrollingDma(void) {  // 0x808DAC
  WriteReg(VMAIN, 0x80);
  if ((uint8)bg1_update_row_enable) {
    LOBYTE(bg1_update_row_enable) = 0;
    uint16 v0 = bg1_update_row_unwrapped_dst;
    WriteRegWord(VMADDL, bg1_update_row_unwrapped_dst);
    WriteRegWord(DMAP1, 0x1801);
    WriteRegWord(A1T1L, ADDR16_OF_RAM(*bg1_column_update_tilemap_top_halves));
    WriteReg(A1B1, 0x7E);
    uint16 v1 = bg1_update_row_unwrapped_size;
    WriteRegWord(DAS1L, bg1_update_row_unwrapped_size);
    WriteReg(MDMAEN, 2u);
    WriteRegWord(VMADDL, v0 | 0x20);
    WriteRegWord(DAS1L, v1);
    WriteRegWord(A1T1L, ADDR16_OF_RAM(*bg1_column_update_tilemap_bottom_halves));
    WriteReg(MDMAEN, 2u);
    WriteRegWord(A1T1L, bg1_update_row_wrapped_top_src);
    uint16 v2 = bg1_update_row_wrapped_size;
    if (bg1_update_row_wrapped_size) {
      WriteRegWord(DAS1L, bg1_update_row_wrapped_size);
      uint16 v3 = bg1_update_row_wrapped_dst;
      WriteRegWord(VMADDL, bg1_update_row_wrapped_dst);
      WriteReg(MDMAEN, 2u);
      WriteRegWord(VMADDL, v3 | 0x20);
      WriteRegWord(DAS1L, v2);
      WriteRegWord(A1T1L, bg1_update_row_wrapped_bottom_src);
      WriteReg(MDMAEN, 2u);
    }
  }
  if ((uint8)bg2_update_row_enable) {
    LOBYTE(bg2_update_row_enable) = 0;
    uint16 v4 = bg2_update_row_unwrapped_dst;
    WriteRegWord(VMADDL, bg2_update_row_unwrapped_dst);
    WriteRegWord(DMAP1, 0x1801);
    WriteRegWord(A1T1L, ADDR16_OF_RAM(*bg2_column_update_tilemap_top_halves));
    WriteReg(A1B1, 0x7E);
    uint16 v5 = bg2_update_row_unwrapped_size;
    WriteRegWord(DAS1L, bg2_update_row_unwrapped_size);
    WriteReg(MDMAEN, 2u);
    WriteRegWord(VMADDL, v4 | 0x20);
    WriteRegWord(DAS1L, v5);
    WriteRegWord(A1T1L, ADDR16_OF_RAM(*bg2_column_update_tilemap_bottom_halves));
    WriteReg(MDMAEN, 2u);
    WriteRegWord(A1T1L, bg2_update_row_wrapped_top_src);
    uint16 v6 = bg2_update_row_wrapped_size;
    if (bg2_update_row_wrapped_size) {
      WriteRegWord(DAS1L, bg2_update_row_wrapped_size);
      uint16 v7 = bg2_update_row_wrapped_dst;
      WriteRegWord(VMADDL, bg2_update_row_wrapped_dst);
      WriteReg(MDMAEN, 2u);
      WriteRegWord(VMADDL, v7 | 0x20);
      WriteRegWord(DAS1L, v6);
      WriteRegWord(A1T1L, bg2_update_row_wrapped_bottom_src);
      WriteReg(MDMAEN, 2u);
    }
  }
}

void NMI_ProcessVramReadQueue(void) {  // 0x808EA2
  if ((uint8)vram_read_queue_tail) {
    *((uint8 *)&vram_read_queue[0].vram_target + (uint8)vram_read_queue_tail) = 0;
    int v0 = 0;
    WriteReg(VMAIN, 0x80);
    while (vram_read_queue[v0].vram_target) {
      WriteRegWord(VMADDL, vram_read_queue[v0].vram_target);
      ReadRegWord(RDVRAML);
      WriteRegWord(DMAP1, vram_read_queue[v0].dma_parameters);
      WriteRegWord(A1T1L, vram_read_queue[v0].src.addr);
      WriteRegWord(A1T1H, *(VoidP *)((char *)&vram_read_queue[v0].src.addr + 1));
      WriteRegWord(DAS1L, vram_read_queue[v0].size);
      WriteRegWord(DAS10, 0);
      WriteRegWord(A2A1H, 0);
      WriteReg(MDMAEN, 2u);
      ++v0;
    }
    LOBYTE(vram_read_queue_tail) = 0;
  }
}

uint8 HasQueuedMusic(void) {  // 0x808EF4
  int v0 = 14;
  while (!music_queue_delay[v0 >> 1]) {
    v0 -= 2;
    if (v0 < 0)
      return 0;
  }
  return 1;
}

void HandleMusicQueue(void) {  // 0x808F0C
  bool v0 = music_timer-- == 1;
  if ((music_timer & 0x8000u) == 0) {
    if (!v0)
      return;
    if ((music_entry & 0x8000u) != 0) {
      music_data_index = (uint8)music_entry;
      cur_music_track = -1;
      copy24(&R0_, (LongPtr *)((char *)&kMusicPointers + (uint8)music_entry));
      APU_UploadBank();
      cur_music_track = 0;
      int v5 = music_queue_read_pos;
      music_queue_track[v5 >> 1] = 0;
      music_queue_delay[v5 >> 1] = 0;
      music_queue_read_pos = (v5 + 2) & 0xE;
      sound_handler_downtime = 8;
      return;
    }
    uint8 v1 = music_entry & 0x7F;
    music_track_index = music_entry & 0x7F;
    RtlApuWrite(APUI00, music_entry & 0x7F);
    cur_music_track = v1;
    sound_handler_downtime = 8;
    int v2 = music_queue_read_pos;
    music_queue_track[v2 >> 1] = 0;
    music_queue_delay[v2 >> 1] = 0;
    music_queue_read_pos = (v2 + 2) & 0xE;
  }
  if (music_queue_read_pos == music_queue_write_pos) {
    music_timer = 0;
  } else {
    int v4 = music_queue_read_pos >> 1;
    music_entry = music_queue_track[v4];
    music_timer = music_queue_delay[v4];
  }
}

void QueueMusic_Delayed8(uint16 a) {  // 0x808FC1
  char v1;

  if (game_state < kGameState_40_TransitionToDemo && (((uint8)music_queue_write_pos + 2) & 0xE) != music_queue_read_pos) {
    v1 = music_queue_write_pos;
    int v2 = music_queue_write_pos >> 1;
    music_queue_track[v2] = a;
    music_queue_delay[v2] = 8;
    music_queue_write_pos = (v1 + 2) & 0xE;
  }
}

void QueueMusic_DelayedY(uint16 a, uint16 j) {  // 0x808FF7
  if (game_state < 0x28u) {
    int v2 = music_queue_write_pos;
    music_queue_track[v2 >> 1] = a;
    int v4 = j;
    if (j < 8u)
      v4 = 8;
    music_queue_delay[v2 >> 1] = v4;
    music_queue_write_pos = (v2 + 2) & 0xE;
  }
}

void QueueSfx1_Max15(uint16 a) {  // 0x809021
  QueueSfx1_Internal(a << 8 | 15);
}

void QueueSfx1_Max9(uint16 a) {  // 0x80902B
  QueueSfx1_Internal(a << 8 | 9);
}

void QueueSfx1_Max3(uint16 a) {  // 0x809035
  QueueSfx1_Internal(a << 8 | 3);
}

void QueueSfx1_Max1(uint16 a) {  // 0x80903F
  QueueSfx1_Internal(a << 8 | 1);
}

void QueueSfx1_Max6(uint16 a) {  // 0x809049
  QueueSfx1_Internal(a << 8 | 6);
}

void QueueSfx1_Internal(uint16 a) {  // 0x809051
  sfx_max_queued[0] = a;
  if ((uint8)((sfx_writepos[0] - sfx_readpos[0]) & 0xF) < (uint8)a) {
    uint8 v2 = GET_HIBYTE(a);
    if (!debug_disable_sounds && game_state < 0x28u && (power_bomb_explosion_status & 0x8000u) == 0) {
      uint8 v1 = sfx_writepos[0];
      uint8 v3 = sfx_writepos[0] + 1;
      if (v3 >= 0x10u)
        v3 = 0;
      if (v3 == sfx_readpos[0]) {
        if (v2 < sfx1_queue[v1])
          sfx1_queue[v1] = v2;
      } else {
        sfx1_queue[sfx_writepos[0]] = v2;
        sfx_writepos[0] = v3;
        sfx1_queue[v3] = 0;
      }
    }
  }
}

void QueueSfx2_Max15(uint16 a) {  // 0x8090A3
  QueueSfx2_Internal(a << 8 | 15);
}

void QueueSfx2_Max9(uint16 a) {  // 0x8090AD
  QueueSfx2_Internal(a << 8 | 9);
}

void QueueSfx2_Max3(uint16 a) {  // 0x8090B7
  QueueSfx2_Internal(a << 8 | 3);
}

void QueueSfx2_Max1(uint16 a) {  // 0x8090C1
  QueueSfx2_Internal(a << 8 | 1);
}

void QueueSfx2_Max6(uint16 a) {  // 0x8090CB
  QueueSfx2_Internal(a << 8 | 6);
}

void QueueSfx2_Internal(uint16 a) {  // 0x8090D3
  sfx_max_queued[1] = a;
  if ((uint8)((sfx_writepos[1] - sfx_readpos[1]) & 0xF) < (uint8)a) {
    uint8 v2 = GET_HIBYTE(a);
    if (!debug_disable_sounds && game_state < 0x28u && (power_bomb_explosion_status & 0x8000u) == 0) {
      uint8 v1 = sfx_writepos[1];
      uint8 v3 = sfx_writepos[1] + 1;
      if (v3 >= 0x10u)
        v3 = 0;
      if (v3 == sfx_readpos[1]) {
        if (v2 < sfx2_queue[v1])
          sfx2_queue[v1] = v2;
      } else {
        sfx2_queue[sfx_writepos[1]] = v2;
        sfx_writepos[1] = v3;
        sfx2_queue[v3] = 0;
      }
    }
  }
}

void QueueSfx3_Max15(uint16 a) {  // 0x809125
  QueueSfx3_Internal(a << 8 | 15);
}

void QueueSfx3_Max9(uint16 a) {  // 0x80912F
  QueueSfx3_Internal(a << 8 | 9);
}

void QueueSfx3_Max3(uint16 a) {  // 0x809139
  QueueSfx3_Internal(a << 8 | 3);
}

void QueueSfx3_Max1(uint16 a) {  // 0x809143
  QueueSfx3_Internal(a << 8 | 1);
}

void QueueSfx3_Max6(uint16 a) {  // 0x80914D
  QueueSfx3_Internal(a << 8 | 6);
}

void QueueSfx3_Internal(uint16 a) {  // 0x809155
  sfx_max_queued[2] = a;
  if ((uint8)((sfx_writepos[2] - sfx_readpos[2]) & 0xF) < (uint8)a) {
    uint8 v2 = GET_HIBYTE(a);
    if (!debug_disable_sounds && game_state < 0x28u && (power_bomb_explosion_status & 0x8000u) == 0) {
      uint8 v1 = sfx_writepos[2];
      uint8 v3 = sfx_writepos[2] + 1;
      if (v3 >= 0x10u)
        v3 = 0;
      if (v3 == sfx_readpos[2]) {
        if (v2 < sfx3_queue[v1])
          sfx3_queue[v1] = v2;
      } else {
        sfx3_queue[sfx_writepos[2]] = v2;
        sfx_writepos[2] = v3;
        sfx3_queue[v3] = 0;
      }
    }
  }
}

void SetupDmaTransfer(const void *p) {  // 0x8091A9
  const StartDmaCopy *s = (const StartDmaCopy *)p;

  int v2 = s->chan * 16;
  WriteRegWord((SnesRegs)(v2 + DMAP0), *(uint16 *)&s->dmap);
  WriteRegWord((SnesRegs)(v2 + A1T0L), *(uint16 *)&s->a1.addr);
  WriteReg((SnesRegs)(v2 + A1B0), s->a1.bank);
  WriteRegWord((SnesRegs)(v2 + DAS0L), s->das);
}

void NmiUpdateIoRegisters(void) {  // 0x8091EE
  WriteReg(NMITIMEN, reg_NMITIMEN);
  WriteReg(INIDISP, reg_INIDISP);
  WriteReg(OBSEL, reg_OBSEL);
  WriteReg(BGMODE, reg_BGMODE);
  WriteReg(MOSAIC, reg_MOSAIC);
  WriteReg(BG1SC, reg_BG1SC);
  WriteReg(BG2SC, reg_BG2SC);
  WriteReg(BG3SC, reg_BG3SC);
  WriteReg(BG4SC, reg_BG4SC);
  WriteReg(BG12NBA, reg_BG12NBA);
  WriteReg(BG34NBA, reg_BG34NBA);
  WriteReg(M7SEL, reg_M7SEL);
  WriteReg(W12SEL, reg_W12SEL);
  WriteReg(W34SEL, reg_W34SEL);
  WriteReg(WOBJSEL, reg_WOBJSEL);
  WriteReg(WH0, reg_WH0);
  WriteReg(WH1, reg_WH1);
  WriteReg(WH2, reg_WH2);
  WriteReg(WH3, reg_WH3);
  WriteReg(WBGLOG, reg_WBGLOG);
  WriteReg(WOBJLOG, reg_WOBJLOG);
  gameplay_TM = reg_TM;
  WriteReg(TM, reg_TM);
  WriteReg(TMW, reg_TMW);
  WriteReg(TS, reg_TS);
  WriteReg(TSW, reg_TSW);
  WriteReg(CGWSEL, reg_CGWSEL);
  WriteReg(CGADSUB, reg_CGADSUB);
  gameplay_CGWSEL = next_gameplay_CGWSEL;
  gameplay_CGADSUB = next_gameplay_CGADSUB;
  WriteReg(COLDATA, reg_COLDATA[0]);
  WriteReg(COLDATA, reg_COLDATA[1]);
  WriteReg(COLDATA, reg_COLDATA[2]);
  WriteReg(SETINI, reg_SETINI);
  WriteReg(BG1HOFS, reg_BG1HOFS);
  WriteReg(BG1HOFS, HIBYTE(reg_BG1HOFS));
  WriteReg(BG1VOFS, reg_BG1VOFS);
  WriteReg(BG1VOFS, HIBYTE(reg_BG1VOFS));
  WriteReg(BG2HOFS, reg_BG2HOFS);
  WriteReg(BG2HOFS, HIBYTE(reg_BG2HOFS));
  WriteReg(BG2VOFS, reg_BG2VOFS);
  WriteReg(BG2VOFS, HIBYTE(reg_BG2VOFS));
  WriteReg(BG3HOFS, reg_BG3HOFS);
  WriteReg(BG3HOFS, HIBYTE(reg_BG3HOFS));
  WriteReg(BG3VOFS, reg_BG3VOFS);
  WriteReg(BG3VOFS, HIBYTE(reg_BG3VOFS));
  WriteReg(BG4HOFS, reg_BG4HOFS);
  WriteReg(BG4HOFS, HIBYTE(reg_BG4HOFS));
  WriteReg(BG4VOFS, reg_BG4VOFS);
  WriteReg(BG4VOFS, HIBYTE(reg_BG4VOFS));
  HIBYTE(hdma_data_table_in_ceres) = reg_BGMODE_fake;
  if ((reg_BGMODE & 7) == 7 || (reg_BGMODE_fake & 7) == 7) {
    WriteReg(M7A, reg_M7A);
    WriteReg(M7A, HIBYTE(reg_M7A));
    WriteReg(M7B, reg_M7B);
    WriteReg(M7B, HIBYTE(reg_M7B));
    WriteReg(M7C, reg_M7C);
    WriteReg(M7C, HIBYTE(reg_M7C));
    WriteReg(M7D, reg_M7D);
    WriteReg(M7D, HIBYTE(reg_M7D));
    WriteReg(M7X, reg_M7X);
    WriteReg(M7X, HIBYTE(reg_M7X));
    WriteReg(M7Y, reg_M7Y);
    WriteReg(M7Y, HIBYTE(reg_M7Y));
  }
}

void NmiUpdatePalettesAndOam(void) {  // 0x80933A
  WriteRegWord(DMAP0, 0x400);
  WriteRegWord(A1T0L, ADDR16_OF_RAM(*oam_ent));
  WriteReg(A1B0, 0);
  WriteRegWord(DAS0L, 0x220);
  WriteRegWord(OAMADDL, 0);
  WriteRegWord(DMAP1, 0x2200);
  WriteRegWord(A1T1L, ADDR16_OF_RAM(*palette_buffer));
  WriteReg(A1B1, 0x7E);
  WriteRegWord(DAS1L, 0x200);
  WriteReg(CGADD, 0);
  WriteReg(MDMAEN, 3u);
}

void NmiTransferSamusToVram(void) {  // 0x809376
  WriteReg(VMAIN, 0x80);
  if ((uint8)nmi_copy_samus_halves) {
    const uint8 *rp = RomPtr_92(nmi_copy_samus_top_half_src);
    WriteRegWord(VMADDL, 0x6000);
    WriteRegWord(DMAP1, 0x1801);
    uint16 v0 = GET_WORD(rp + 0);
    WriteRegWord(A1T1L, v0);
    WriteRegWord(A1B1, GET_WORD(rp + 2));
    uint16 v2 = GET_WORD(rp + 3);
    WriteRegWord(DAS1L, v2);
    WriteReg(MDMAEN, 2u);
    WriteRegWord(VMADDL, 0x6100);
    WriteRegWord(A1T1L, v0 + v2);
    if (GET_WORD(rp + 5)) {
      WriteRegWord(DAS1L, GET_WORD(rp + 5));
      WriteReg(MDMAEN, 2u);
    }
  }
  if (HIBYTE(nmi_copy_samus_halves)) {
    const uint8 *rp = RomPtr_92(nmi_copy_samus_bottom_half_src);
    WriteRegWord(VMADDL, 0x6080);
    WriteRegWord(DMAP1, 0x1801);
    uint16 v4 = GET_WORD(rp + 0);
    WriteRegWord(A1T1L, v4);
    WriteRegWord(A1B1, GET_WORD(rp + 2));
    uint16 v6 = GET_WORD(rp + 3);
    WriteRegWord(DAS1L, v6);
    WriteReg(MDMAEN, 2u);
    WriteRegWord(VMADDL, 0x6180);
    WriteRegWord(A1T1L, v4 + v6);
    if (GET_WORD(rp + 5)) {
      WriteRegWord(DAS1L, GET_WORD(rp + 5));
      WriteReg(MDMAEN, 2u);
    }
  }
}

void NmiProcessAnimtilesVramTransfers(void) {  // 0x809416
  if ((animtiles_enable_flag & 0x8000u) != 0) {
    for (int i = 10; (i & 0x80) == 0; i -= 2) {
      int v1 = i >> 1;
      if (animtiles_ids[v1]) {
        if (animtiles_src_ptr[v1]) {
          WriteRegWord(A1T0L, animtiles_src_ptr[v1]);
          WriteReg(A1B0, 0x87);
          WriteRegWord(DMAP0, 0x1801);
          WriteRegWord(DAS0L, animtiles_sizes[v1]);
          WriteRegWord(VMADDL, animtiles_vram_ptr[v1]);
          WriteReg(VMAIN, 0x80);
          WriteReg(MDMAEN, 1u);
          animtiles_src_ptr[v1] = 0;
        }
      }
    }
  }
}

void ReadJoypadInputs(void) {  // 0x809459
  uint16 RegWord = ReadRegWord(JOY1L);
  joypad1_lastkeys = RegWord;
  joypad1_newkeys = RegWord & (joypad1_prev ^ RegWord);
  joypad1_newkeys2_UNUSED = RegWord & (joypad1_prev ^ RegWord);
  if (RegWord && RegWord == joypad1_prev) {
    if (!--joypad1_keyrepeat_ctr) {
      joypad1_newkeys2_UNUSED = joypad1_lastkeys;
      joypad1_keyrepeat_ctr = joypad_ctr_repeat_next;
    }
  } else {
    joypad1_keyrepeat_ctr = joypad_ctr_repeat_first;
  }
  joypad1_prev = joypad1_lastkeys;
  if (enable_debug) {
    uint16 v1 = ReadRegWord(JOY2L);
    joypad2_last = v1;
    joypad2_new_keys = v1 & (joypad2_prev ^ v1);
    joypad2_newkeys2 = v1 & (joypad2_prev ^ v1);
    if (v1 && v1 == joypad2_prev) {
      if (!--joypad2_keyrepeat_ctr) {
        joypad2_newkeys2 = joypad2_last;
        joypad2_keyrepeat_ctr = joypad_ctr_repeat_next;
      }
    } else {
      joypad2_keyrepeat_ctr = joypad_ctr_repeat_first;
    }
    joypad2_prev = joypad2_last;
    if (is_uploading_apu || joypad1_lastkeys != (kButton_Select | kButton_Start | kButton_L | kButton_R)) {
      if (enable_debug) {
        joypad_dbg_1 = 0;
        joypad_dbg_2 = 0;
        if ((joypad_dbg_flags & 0x4000) == 0) {
          if ((joypad1_lastkeys & (kButton_Select | kButton_L)) == (kButton_Select | kButton_L)) {
            joypad_dbg_1 = joypad1_newkeys;
            joypad1_lastkeys = 0;
            joypad1_newkeys = 0;
          }
          if ((joypad1_lastkeys & (kButton_Select | kButton_R)) == (kButton_Select | kButton_R)) {
            joypad_dbg_2 = joypad1_newkeys;
            joypad1_lastkeys = 0;
            joypad1_newkeys = 0;
          }
          if ((joypad_dbg_2 & 0x80) != 0)
            *(uint16 *)&reg_NMITIMEN ^= 0x30u;
          if ((joypad_dbg_2 & 0x8000u) != 0) {
            bool v2 = (~joypad_dbg_flags & 0x8000u) != 0;
            joypad_dbg_flags ^= 0x8000u;
            if (v2) {
              joypad_dbg_missiles_swap = samus_missiles;
              joypad_dbg_super_missiles_swap = samus_super_missiles;
              joypad_dbg_power_bombs_swap = samus_power_bombs;
              samus_missiles = 0;
              samus_super_missiles = 0;
              samus_power_bombs = 0;
            } else {
              samus_missiles = joypad_dbg_missiles_swap;
              samus_super_missiles = joypad_dbg_super_missiles_swap;
              samus_power_bombs = joypad_dbg_power_bombs_swap;
            }
          }
          if ((joypad_dbg_2 & 0x40) != 0)
            joypad_dbg_flags ^= 0x2000u;
        }
      } else {
        joypad_dbg_1 = 0;
        joypad_dbg_2 = 0;
        joypad2_last &= 0x10u;
        joypad2_new_keys &= 0x10u;
      }
    } else {
      debug_disable_sounds = 0;
      SoftReset();
    }
  }
}

void Vector_NMI(void) {  // 0x809583
  if (waiting_for_nmi) {
    NmiUpdatePalettesAndOam();
    NmiTransferSamusToVram();
    NmiProcessAnimtilesVramTransfers();
    NmiUpdateIoRegisters();
    for (int i = 0; i != 6; ++i) {
      if (hdma_object_channels_bitmask[i])
        WriteRegWord((SnesRegs)(LOBYTE(hdma_object_bank_slot[i]) + 17154), hdma_object_table_pointers[i]);
    }
    if (reg_BGMODE == 7 || reg_BGMODE_fake == 7)
      NMI_ProcessMode7Queue();
    NMI_ProcessVramWriteQueue();
    NMI_ProcessVramReadQueue();
    WriteReg(HDMAEN, reg_HDMAEN);
    waiting_for_nmi = 0;
    nmi_frames_missed = 0;
    ++nmi_frame_counter_byte;
    ++nmi_frame_counter_word;
  } else if (++nmi_frames_missed >= nmi_frames_missed_max) {
    nmi_frames_missed_max = nmi_frames_missed;
  }
  ++nmi_frame_counter_including_lag;
}

void Irq_DoorTransitionVramUpdate(void) {  // 0x809632
  WriteReg(INIDISP, 0x80);
  WriteRegWord(VMADDL, door_transition_vram_update_dst);
  WriteRegWord(DMAP1, 0x1801);
  WriteRegWord(A1T1L, door_transition_vram_update_src.addr);
  WriteReg(A1B1, door_transition_vram_update_src.bank);
  WriteRegWord(DAS1L, door_transition_vram_update_size);
  WriteReg(VMAIN, 0x80);
  WriteReg(MDMAEN, 2u);
  door_transition_vram_update_enabled &= ~0x8000;
  WriteReg(INIDISP, 0xF);
}

void WaitForIrqDoorTransitionVramUpdate(void) {
  door_transition_vram_update_enabled |= 0x8000u;
  Irq_DoorTransitionVramUpdate();
}

void IrqHandler_SetResult(uint16 a, uint16 y, uint16 x) {
  //  printf("Setting irq next: %d, %d, %d\n", a, x, y);
  cur_irq_handler = a;
  WriteRegWord(VTIMEL, y);
  WriteRegWord(HTIMEL, x);
}

void IrqHandler_0_Nothing(void) {  // 0x80966E
  uint16 a = irqhandler_next_handler;
  irqhandler_next_handler = 0;
  IrqHandler_SetResult(a, 0, 0);
}

void IrqHandler_2_DisableIRQ(void) {  // 0x809680
  *(uint16 *)&reg_NMITIMEN &= ~0x30;
  IrqHandler_SetResult(0, 0, 0);
}

void IrqHandler_4_Main_BeginHudDraw(void) {  // 0x80968B
  WriteReg(BG3SC, 0x5A);
  WriteReg(CGWSEL, 0);
  WriteReg(CGADSUB, 0);
  WriteReg(TM, 4u);
  IrqHandler_SetResult(6, 31, 152);
}

void IrqHandler_6_Main_EndHudDraw(void) {  // 0x8096A9
  WriteReg(CGWSEL, gameplay_CGWSEL);
  WriteReg(CGADSUB, gameplay_CGADSUB);
  WriteReg(BG3SC, gameplay_BG3SC);
  WriteReg(TM, gameplay_TM);

  uint16 a = irqhandler_next_handler;
  irqhandler_next_handler = 0;
  IrqHandler_SetResult(a ? a : 4, 0, 152);
}

void IrqHandler_8_StartOfDoor_BeginHud(void) {  // 0x8096D3
  WriteReg(BG3SC, 0x5A);
  WriteReg(TM, 4u);
  WriteReg(CGWSEL, 0);
  WriteReg(CGADSUB, 0);
  IrqHandler_SetResult(10, 31, 152);
}

void IrqHandler_10_StartOfDoor_EndHud(void) {  // 0x8096F1
  uint8 v0;
  if ((((uint8)previous_cre_bitset | (uint8)cre_bitset) & 1) != 0)
    v0 = 16;
  else
    v0 = 17;
  WriteReg(TM, v0);
  uint16 a = irqhandler_next_handler;
  irqhandler_next_handler = 0;
  IrqHandler_SetResult(a ? a : 8, 0, 152);
}

void IrqHandler_12_Draygon_BeginHud(void) {  // 0x80971A
  WriteReg(TM, 4u);
  WriteReg(CGWSEL, 0);
  WriteReg(CGADSUB, 0);
  IrqHandler_SetResult(14, 31, 152);
}

void IrqHandler_14_Draygon_EndHud(void) {  // 0x809733
  WriteReg(BG3SC, gameplay_BG3SC);
  WriteReg(CGWSEL, gameplay_CGWSEL);
  WriteReg(CGADSUB, gameplay_CGADSUB);
  uint16 a = irqhandler_next_handler;
  irqhandler_next_handler = 0;
  IrqHandler_SetResult(a ? a : 12, 0, 152);
}

void IrqHandler_16_VerticalDoor_BeginHud(void) {  // 0x809758
  WriteReg(TM, 4u);
  WriteReg(CGWSEL, 0);
  WriteReg(CGADSUB, 0);
  IrqHandler_SetResult(18, 31, 152);
}

void IrqHandler_18_VerticalDoor_EndHud(void) {  // 0x809771
  uint8 v0;
  if (((previous_cre_bitset | cre_bitset) & 1) != 0)
    v0 = 16;
  else
    v0 = 17;
  WriteReg(TM, v0);
  WriteReg(CGWSEL, 0);
  WriteReg(CGADSUB, 0);
//  if ((door_transition_vram_update_enabled & 0x8000u) != 0)
//    Irq_DoorTransitionVramUpdate();
  if ((door_transition_flag & 0x8000u) == 0)
    Irq_FollowDoorTransition();
  IrqHandler_SetResult(20, 216, 152);
}

void IrqHandler_20_VerticalDoor_EndDraw(void) {  // 0x8097A9
//  *(uint16 *)&waiting_for_nmi = 1;
  uint16 a = irqhandler_next_handler;
  irqhandler_next_handler = 0;
  IrqHandler_SetResult(a ? a : 16, 0, 152);
}

void IrqHandler_22_HorizDoor_BeginHud(void) {  // 0x8097C1
  WriteReg(TM, 4u);
  WriteReg(CGWSEL, 0);
  WriteReg(CGADSUB, 0);
  IrqHandler_SetResult(24, 31, 152);
}

void IrqHandler_24_HorizDoor_EndHud(void) {  // 0x8097DA
  uint8 v0;

  if (((previous_cre_bitset | cre_bitset) & 1) != 0)
    v0 = 16;
  else
    v0 = 17;
  WriteReg(TM, v0);
  WriteReg(CGWSEL, 0);
  WriteReg(CGADSUB, 0);
  if ((door_transition_flag & 0x8000u) == 0)
    Irq_FollowDoorTransition();
  IrqHandler_SetResult(26, 160, 152);
}

void IrqHandler_26_HorizDoor_EndDraw(void) {  // 0x80980A
  uint16 a = irqhandler_next_handler;
  irqhandler_next_handler = 0;
  IrqHandler_SetResult(a ? a : 22, 0, 152);
}

void EnableIrqInterrupts(void) {  // 0x80982A
  WriteRegWord(VTIMEL, 0);
  WriteRegWord(HTIMEL, 152);
  *(uint16 *)&reg_NMITIMEN |= 0x30u;
}

void EnableIrqInterruptsNow(void) {  // 0x809841
  WriteRegWord(VTIMEL, 0);
  WriteRegWord(HTIMEL, 152);
  *(uint16 *)&reg_NMITIMEN |= 0x30u;
  WriteReg(NMITIMEN, reg_NMITIMEN);
}

void DisableIrqInterrupts(void) {  // 0x80985F
  *(uint16 *)&reg_NMITIMEN &= ~0x30;
}

static Func_V *const kIrqHandlers[14] = {  // 0x80986A
  IrqHandler_0_Nothing,
  IrqHandler_2_DisableIRQ,
  IrqHandler_4_Main_BeginHudDraw,
  IrqHandler_6_Main_EndHudDraw,
  IrqHandler_8_StartOfDoor_BeginHud,
  IrqHandler_10_StartOfDoor_EndHud,
  IrqHandler_12_Draygon_BeginHud,
  IrqHandler_14_Draygon_EndHud,
  IrqHandler_16_VerticalDoor_BeginHud,
  IrqHandler_18_VerticalDoor_EndHud,
  IrqHandler_20_VerticalDoor_EndDraw,
  IrqHandler_22_HorizDoor_BeginHud,
  IrqHandler_24_HorizDoor_EndHud,
  IrqHandler_26_HorizDoor_EndDraw,
};

void Vector_IRQ(void) {
  kIrqHandlers[cur_irq_handler >> 1]();
}

static const uint16 kHudTilemaps[32] = {  // 0x8099CF
  0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f,
  0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c1d, 0x2c1d, 0x2c1d, 0x2c1d, 0x2c1d, 0x2c1c,
};
static const uint16 kHudTilemaps_Row1to3[96] = {
  0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f,
  0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c12, 0x2c12, 0x2c23, 0x2c12, 0x2c12, 0x2c1e,
  0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f,
  0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2822, 0x2822, 0x2823, 0x2813, 0x2c14, 0x2c1e,
  0x2c0f, 0x2c0b, 0x2c0c, 0x2c0d, 0x2c32, 0x2c0f, 0x2c09, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f,
  0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c0f, 0x2c12, 0x2c12, 0xa824, 0x2815, 0x2c16, 0x2c1e,
};
static const uint16 kHudTilemaps_AutoReserve[12] = { 0x3c33, 0x3c46, 0x3c47, 0x3c48, 0xbc33, 0xbc46, 0x2c33, 0x2c46, 0x2c47, 0x2c48, 0xac33, 0xac46 };
static const uint16 kHudTilemaps_Missiles[22] = {
  0x344b, 0x3449, 0x744b, 0x344c, 0x344a, 0x744c, 0x3434, 0x7434, 0x3435, 0x7435, 0x3436, 0x7436, 0x3437, 0x7437, 0x3438, 0x7438,
  0x3439, 0x7439, 0x343a, 0x743a, 0x343b, 0x743b,
};

void AddMissilesToHudTilemap(void) {
  if ((hud_tilemap[10] & 0x3FF) == 15) {
    hud_tilemap[10] = kHudTilemaps_Missiles[0];
    hud_tilemap[11] = kHudTilemaps_Missiles[1];
    hud_tilemap[12] = kHudTilemaps_Missiles[2];
    hud_tilemap[42] = kHudTilemaps_Missiles[3];
    hud_tilemap[43] = kHudTilemaps_Missiles[4];
    hud_tilemap[44] = kHudTilemaps_Missiles[5];
  }
}

void AddSuperMissilesToHudTilemap(void) {  // 0x809A0E
  AddToTilemapInner(0x1C, (const uint16*)RomPtr_80(addr_kHudTilemaps_Missiles + 12));
}

void AddPowerBombsToHudTilemap(void) {  // 0x809A1E
  AddToTilemapInner(0x22, (const uint16 *)RomPtr_80(addr_kHudTilemaps_Missiles + 20));
}

void AddGrappleToHudTilemap(void) {  // 0x809A2E
  AddToTilemapInner(0x28, (const uint16 *)RomPtr_80(addr_kHudTilemaps_Missiles + 28));
}

void AddXrayToHudTilemap(void) {  // 0x809A3E
  AddToTilemapInner(0x2E, (const uint16 *)RomPtr_80(addr_kHudTilemaps_Missiles + 36));
}

void AddToTilemapInner(uint16 k, const uint16 *j) {  // 0x809A4C
  int v2 = k >> 1;
  if ((hud_tilemap[v2] & 0x3FF) == 15) {
    hud_tilemap[v2] = j[0];
    hud_tilemap[v2 + 1] = j[1];
    hud_tilemap[v2 + 32] = j[2];
    hud_tilemap[v2 + 33] = j[3];
  }
}

void InitializeHud(void) {  // 0x809A79
  WriteRegWord(VMADDL, addr_unk_605800);
  WriteRegWord(VMAIN, 0x80);
  static const StartDmaCopy unk_809A8F = { 1, 1, 0x18, LONGPTR(0x80988b), 0x0040 };
  SetupDmaTransfer(&unk_809A8F);
  WriteReg(MDMAEN, 2u);
  for (int i = 0; i != 192; i += 2)
    hud_tilemap[i >> 1] = kHudTilemaps_Row1to3[i >> 1];
  if ((equipped_items & 0x8000u) != 0)
    AddXrayToHudTilemap();
  if ((equipped_items & 0x4000) != 0)
    AddGrappleToHudTilemap();
  if (samus_max_missiles)
    AddMissilesToHudTilemap();
  if (samus_max_super_missiles)
    AddSuperMissilesToHudTilemap();
  if (samus_max_power_bombs)
    AddPowerBombsToHudTilemap();
  samus_prev_health = 0;
  samus_prev_missiles = 0;
  samus_prev_super_missiles = 0;
  samus_prev_power_bombs = 0;
  samus_prev_hud_item_index = 0;
  InitializeMiniMapBroken();
  R0_.addr = addr_kDigitTilesetsWeapon;
  *(uint16 *)&R0_.bank = 128;
  if (samus_max_missiles)
    DrawThreeHudDigits(R0_, samus_missiles, 0x94);
  if (samus_max_super_missiles)
    DrawTwoHudDigits(R0_, samus_super_missiles, 0x9C);
  if (samus_max_power_bombs)
    DrawTwoHudDigits(R0_, samus_power_bombs, 0xA2);
  ToggleHudItemHighlight(hud_item_index, 0x1000);
  ToggleHudItemHighlight(samus_prev_hud_item_index, 0x1400);
  HandleHudTilemap();
}

static const uint16 kEnergyTankIconTilemapOffsets[14] = { 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, 2, 4, 6, 8, 0xa, 0xc, 0xe };

void HandleHudTilemap(void) {  // 0x809B44
  R0_.bank = 0;
  if (reserve_health_mode == 1) {
    const uint16 *v1 = (const uint16 *)RomPtr_80(addr_kHudTilemaps_AutoReserve);
    if (!samus_reserve_health)
      v1 += 6;
    hud_tilemap[8] = v1[0];
    hud_tilemap[9] = v1[1];
    hud_tilemap[40] = v1[2];
    hud_tilemap[41] = v1[3];
    hud_tilemap[72] = v1[4];
    hud_tilemap[73] = v1[5];
  }
  if (samus_health != samus_prev_health) {
    samus_prev_health = samus_health;

    R20_ = SnesDivide(samus_health, 100);
    R18_ = SnesModulus(samus_health, 100);
    int v2 = 0;
    R22_ = SnesDivide(samus_max_health, 100) + 1;
    do {
      if (!--R22_)
        break;
      uint16 v3 = 13360;
      if (R20_) {
        --R20_;
        v3 = 10289;
      }
      hud_tilemap[kEnergyTankIconTilemapOffsets[v2 >> 1] >> 1] = v3;
      v2 += 2;
    } while ((int16)(v2 - 28) < 0);
    R0_.addr = addr_kDigitTilesetsHealth;
    DrawTwoHudDigits(R0_, R18_, 0x8C);
  }
  R0_.addr = addr_kDigitTilesetsWeapon;
  if (samus_max_missiles && samus_missiles != samus_prev_missiles) {
    samus_prev_missiles = samus_missiles;
    DrawThreeHudDigits(R0_, samus_missiles, 0x94);
  }
  if (samus_max_super_missiles && samus_super_missiles != samus_prev_super_missiles) {
    samus_prev_super_missiles = samus_super_missiles;
    if ((joypad_dbg_flags & 0x1F40) != 0)
      DrawThreeHudDigits(R0_, samus_prev_super_missiles, 0x9C);
    else
      DrawTwoHudDigits(R0_, samus_prev_super_missiles, 0x9C);
  }
  if (samus_max_power_bombs && samus_power_bombs != samus_prev_power_bombs) {
    samus_prev_power_bombs = samus_power_bombs;
    DrawTwoHudDigits(R0_, samus_power_bombs, 0xA2);
  }
  if (hud_item_index != samus_prev_hud_item_index) {
    ToggleHudItemHighlight(hud_item_index, 0x1000);
    ToggleHudItemHighlight(samus_prev_hud_item_index, 0x1400);
    samus_prev_hud_item_index = hud_item_index;
    if (samus_movement_type != 3
        && samus_movement_type != 20
        && grapple_beam_function == 0xC4F0
        && !time_is_frozen_flag) {
      QueueSfx1_Max6(0x39u);
    }
  }
  uint16 v4 = 5120;
  if ((nmi_frame_counter_byte & 0x10) != 0)
    v4 = 4096;
  ToggleHudItemHighlight(samus_auto_cancel_hud_item_index, v4);
  uint16 v5 = vram_write_queue_tail;
  gVramWriteEntry(vram_write_queue_tail)->size = 192;
  v5 += 2;
  gVramWriteEntry(v5)->size = ADDR16_OF_RAM(*hud_tilemap);
  v5 += 2;
  gVramWriteEntry(v5++)->size = 126;
  gVramWriteEntry(v5)->size = addr_unk_605820;
  vram_write_queue_tail = v5 + 2;
}

static const uint16 kHudItemTilemapOffsets[5] = { 0x14, 0x1c, 0x22, 0x28, 0x2e };

void ToggleHudItemHighlight(uint16 a, uint16 k) {  // 0x809CEA
  int16 v2;

  hud_item_tilemap_palette_bits = k;
  v2 = a - 1;
  if (v2 >= 0) {
    int v3 = kHudItemTilemapOffsets[v2] >> 1;
    if (hud_tilemap[v3] != 11279)
      hud_tilemap[v3] = hud_item_tilemap_palette_bits | hud_tilemap[v3] & 0xE3FF;
    if (hud_tilemap[v3 + 1] != 11279)
      hud_tilemap[v3 + 1] = hud_item_tilemap_palette_bits | hud_tilemap[v3 + 1] & 0xE3FF;
    if (hud_tilemap[v3 + 32] != 11279)
      hud_tilemap[v3 + 32] = hud_item_tilemap_palette_bits | hud_tilemap[v3 + 32] & 0xE3FF;
    if (hud_tilemap[v3 + 33] != 11279)
      hud_tilemap[v3 + 33] = hud_item_tilemap_palette_bits | hud_tilemap[v3 + 33] & 0xE3FF;
    if (!(2 * v2)) {
      if (hud_tilemap[v3 + 2] != 11279)
        hud_tilemap[v3 + 2] = hud_item_tilemap_palette_bits | hud_tilemap[v3 + 2] & 0xE3FF;
      if (hud_tilemap[v3 + 34] != 11279)
        hud_tilemap[v3 + 34] = hud_item_tilemap_palette_bits | hud_tilemap[v3 + 34] & 0xE3FF;
    }
  }
}

void DrawThreeHudDigits(LongPtr r0, uint16 a, uint16 k) {  // 0x809D78
  uint16 v2 = 2 * (a / 100);
  hud_tilemap[k >> 1] = IndirReadWord(r0, v2);
  uint16 RegWord = (a % 100);
  DrawTwoHudDigits(r0, RegWord, k + 2);
}

void DrawTwoHudDigits(LongPtr r0, uint16 a, uint16 k) {  // 0x809D98
  uint16 RegWord = a / 10;
  int v3 = k >> 1;
  hud_tilemap[v3] = IndirReadWord(r0, 2 * RegWord);
  uint16 v4 = 2 * (a % 10);
  hud_tilemap[v3 + 1] = IndirReadWord(r0, v4);
}

static Func_U8 *const kTimerProcessFuncs[7] = {  // 0x809DE7
  ProcessTimer_Empty,
  ProcessTimer_CeresStart,
  ProcessTimer_MotherBrainStart,
  ProcessTimer_InitialDelay,
  ProcessTimer_MovementDelayed,
  ProcessTimer_MovingIntoPlace,
  j_ProcessTimer_Decrement,
};

uint8 ProcessTimer(void) {
  return kTimerProcessFuncs[(uint8)timer_status]();
}

uint8 ProcessTimer_CeresStart(void) {  // 0x809E09
  ClearTimerRam();
  SetTimerMinutes(0x100);
  timer_status = -32765;
  return 0;
}

uint8 ProcessTimer_Empty(void) {  // 0x809E1A
  return 0;
}

uint8 ProcessTimer_MotherBrainStart(void) {  // 0x809E1C
  ClearTimerRam();
  SetTimerMinutes(0x300);
  timer_status = -32765;
  return 0;
}

uint8 ProcessTimer_InitialDelay(void) {  // 0x809E2F
  LOBYTE(timer_x_pos) = timer_x_pos + 1;
  if ((uint8)timer_x_pos >= 0x10u)
    LOBYTE(timer_status) = timer_status + 1;
  return 0;
}

uint8 ProcessTimer_MovementDelayed(void) {  // 0x809E41
  LOBYTE(timer_x_pos) = timer_x_pos + 1;
  if ((uint8)timer_x_pos >= 0x60u) {
    LOBYTE(timer_x_pos) = 0;
    LOBYTE(timer_status) = timer_status + 1;
  }
  return ProcessTimer_Decrement();
}

uint8 ProcessTimer_MovingIntoPlace(void) {  // 0x809E58
  int v0 = 0;
  uint16 v1 = timer_x_pos + 224;
  if ((uint16)(timer_x_pos + 224) >= 0xDC00u) {
    v0 = 1;
    v1 = -9216;
  }
  timer_x_pos = v1;
  uint16 v2 = timer_y_pos - 193;
  if ((uint16)(timer_y_pos - 193) < 0x3000u) {
    ++v0;
    v2 = 12288;
  }
  timer_y_pos = v2;
  if (v0 == 2)
    ++timer_status;
  return j_ProcessTimer_Decrement();
}

uint8 j_ProcessTimer_Decrement(void) {  // 0x809E89
  return ProcessTimer_Decrement();
}

void SetTimerMinutes(uint16 a) {  // 0x809E8C
  *(uint16 *)&timer_centiseconds = 0;
  *(uint16 *)&timer_seconds = a;
}

void ClearTimerRam(void) {  // 0x809E93
  timer_x_pos = 0x8000;
  timer_y_pos = 0x8000;
  *(uint16 *)&timer_centiseconds = 0;
  *(uint16 *)&timer_seconds = 0;
  timer_status = 0;
}

bool DecrementDecimal(uint8 *number, uint8 value) {
  int result = (*number & 0xf) + (~value & 0xf) + 1;
  if (result < 0x10) result = (result - 0x6) & ((result - 0x6 < 0) ? 0xf : 0x1f);
  result = (*number & 0xf0) + (~value & 0xf0) + result;
  if (result < 0x100) result -= 0x60;
  *number = result;
  return result < 0x100;
}

uint8 ProcessTimer_Decrement(void) {  // 0x809EA9
  static const uint8 kTimerCentisecondDecrements[128] = {
    1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 2,
    1, 2, 2, 1, 2, 2, 1, 2, 1, 2, 2, 1, 2, 2, 1, 2,
    1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 2,
    1, 2, 2, 1, 2, 2, 1, 2, 1, 2, 2, 1, 2, 2, 1, 2,
    1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 2,
    1, 2, 2, 1, 2, 2, 1, 2, 1, 2, 2, 1, 2, 2, 1, 2,
    1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 2,
    1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 2,
  };
  if (DecrementDecimal(&timer_centiseconds, kTimerCentisecondDecrements[nmi_frame_counter_word & 0x7f])) {
    if (DecrementDecimal(&timer_seconds, 1)) {
      if (DecrementDecimal(&timer_minutes, 1)) {
        timer_centiseconds = 0;
        timer_seconds = 0;
        timer_minutes = 0;
      } else {
        timer_seconds = 0x59;
      }
    }
  }
  return (timer_centiseconds | timer_seconds | timer_minutes) == 0;

}

void DrawTimer(void) {  // 0x809F6C
  DrawTimerSpritemap(0, addr_word_80A060);
  DrawTwoTimerDigits(*(uint16 *)&timer_minutes, 0xFFE4);
  DrawTwoTimerDigits(*(uint16 *)&timer_seconds, 0xFFFC);
  DrawTwoTimerDigits(*(uint16 *)&timer_centiseconds, 0x14);
}


void DrawTwoTimerDigits(uint16 a, uint16 k) {  // 0x809F95
  char v2;

  v2 = a;
  DrawTimerSpritemap(k, kTimerDigitsSpritemapPtr[(uint16)((uint8)(a & 0xF0) >> 3) >> 1]);
  DrawTimerSpritemap(k + 8, kTimerDigitsSpritemapPtr[v2 & 0xF]);
}

void DrawTimerSpritemap(uint16 a, uint16 j) {  // 0x809FB3
  R20_ = a + HIBYTE(timer_x_pos);
  R18_ = HIBYTE(timer_y_pos);
  R22_ = 2560;
  DrawSpritemap(0x80, j);
}

CoroutineRet StartGameplay_Async(void) {  // 0x80A07B
  COROUTINE_BEGIN(coroutine_state_2, 0)
  WriteRegWord(MDMAEN, 0);
  scrolling_finished_hook = 0;
  music_data_index = 0;
  music_track_index = 0;
  timer_status = 0;
  ResetSoundQueues();
  debug_disable_sounds = -1;
  DisableNMI();
  DisableIrqInterrupts();
  LoadDestinationRoomThings();
  COROUTINE_AWAIT(1, Play20FramesOfMusic_Async());
  ClearAnimtiles();
  WaitUntilEndOfVblankAndClearHdma();
  InitializeSpecialEffectsForNewRoom();
  ClearPLMs();
  ClearEnemyProjectiles();
  ClearPaletteFXObjects();
  UpdateBeamTilesAndPalette();
  LoadColorsForSpritesBeamsAndEnemies();
  LoadEnemies();
  LoadRoomMusic();
  COROUTINE_AWAIT(2, Play20FramesOfMusic_Async());
  UpdateMusicTrackIndex();
  NullFunc();
  ClearBG2Tilemap();
  LoadLevelDataAndOtherThings();
  LoadFXHeader();
  LoadLibraryBackground();
  CalculateLayer2Xpos();
  CalculateLayer2Ypos();
  bg2_x_scroll = layer2_x_pos;
  bg2_y_scroll = layer2_y_pos;
  CalculateBgScrolls();
  DisplayViewablePartOfRoom();
  EnableNMI();
  irqhandler_next_handler = (room_loading_irq_handler == 0) ? 4 : room_loading_irq_handler;
  EnableIrqInterrupts();
  COROUTINE_AWAIT(3, Play20FramesOfMusic_Async());
  static const SpawnHardcodedPlmArgs unk_80A11E = { 0x08, 0x08, 0xb7eb };
  SpawnHardcodedPlm(&unk_80A11E);
  door_transition_function = FUNC16(DoorTransition_FadeInScreenAndFinish);
  COROUTINE_END(0);
}

CoroutineRet Play20FramesOfMusic_Async(void) {  // 0x80A12B
  COROUTINE_BEGIN(coroutine_state_3, 0)
  EnableNMI();
  for(my_counter = 0; my_counter != 20; my_counter++) {
    HandleMusicQueue();
    COROUTINE_AWAIT(1, WaitForNMI_Async());
  }
  DisableNMI();
  COROUTINE_END(0);
}

void ResumeGameplay(void) {  // 0x80A149
  WriteRegWord(MDMAEN, 0);
  DisableNMI();
  DisableIrqInterrupts();
  LoadCRETilesTilesetTilesAndPalette();
  LoadLibraryBackground();
  DisplayViewablePartOfRoom();
  LoadRoomPlmGfx();
  EnableNMI();
  EnableIrqInterrupts();
}

void DisplayViewablePartOfRoom(void) {  // 0x80A176
  int v1, v2, v3;

  v1 = (reg_BG1SC - reg_BG2SC) << 8;
  size_of_bg2 = v1 & 0xF800;
  CalculateBgScrollAndLayerPositionBlocks();
  v2 = 0;
  do {
    v3 = v2;
    blocks_to_update_x_block = layer1_x_block;
    blocks_to_update_y_block = layer1_y_block;
    vram_blocks_to_update_x_block = bg1_x_block;
    vram_blocks_to_update_y_block = bg1_y_block;
    UploadLevelDataColumn();
    if (!(layer2_scroll_x & 1)) {
      blocks_to_update_x_block = layer2_x_block;
      blocks_to_update_y_block = layer2_y_block;
      vram_blocks_to_update_x_block = bg2_x_block;
      vram_blocks_to_update_y_block = bg2_y_block;
      UploadBackgroundDataColumn();
    }
    NMI_ProcessVramWriteQueue();
    ++layer1_x_block;
    ++bg1_x_block;
    ++layer2_x_block;
    ++bg2_x_block;
    ++v2;
  } while (v3 != 16);
}

void QueueClearingOfFxTilemap(void) {  // 0x80A211
  for (int i = 3838; i >= 0; i -= 2)
    *(uint16 *)((char *)ram4000.xray_tilemaps + (uint16)i) = 6222;
  uint16 v1 = vram_write_queue_tail;
  VramWriteEntry *v2 = gVramWriteEntry(vram_write_queue_tail);
  v2->size = 3840;
  v2->src.addr = ADDR16_OF_RAM(ram4000);
  *(uint16 *)&v2->src.bank = 126;
  v2->vram_dst = addr_unk_605880;
  vram_write_queue_tail = v1 + 7;
}

void ClearBG2Tilemap(void) {  // 0x80A23F
  WriteRegWord(VMADDL, 0x4800);
  WriteRegWord(DMAP1, 0x1808);
  WriteRegWord(A1T1L, 0xA29A);
  WriteRegWord(A1B1, 0x80);
  WriteRegWord(DAS1L, 0x800);
  WriteReg(VMAIN, 0);
  WriteReg(MDMAEN, 2u);
  WriteRegWord(VMADDL, 0x4800);
  WriteRegWord(DMAP1, 0x1908);
  WriteRegWord(A1T1L, 0xA29A);
  WriteRegWord(A1B1, 0x80);
  WriteRegWord(DAS1L, 0x800);
  WriteReg(VMAIN, 0x80);
  WriteReg(MDMAEN, 2u);
}

void ClearFXTilemap(void) {  // 0x80A29C
  WriteRegWord(VMADDL, 0x5880);
  WriteRegWord(DMAP1, 0x1808);
  WriteRegWord(A1T1L, 0xA2F7);
  WriteRegWord(A1B1, 0x80);
  WriteRegWord(DAS1L, 0x780);
  WriteReg(VMAIN, 0);
  WriteReg(MDMAEN, 2u);
  WriteRegWord(VMADDL, 0x5880);
  WriteRegWord(DMAP1, 0x1908);
  WriteRegWord(A1T1L, 0xA2F8);
  WriteRegWord(A1B1, 0x80);
  WriteRegWord(DAS1L, 0x780);
  WriteReg(VMAIN, 0x80);
  WriteReg(MDMAEN, 2u);
}

uint8 CalculateLayer2Xpos(void) {  // 0x80A2F9
  if (!layer2_scroll_x) {
    layer2_x_pos = layer1_x_pos;
    return 0;
  }
  if (layer2_scroll_x != 1) {
    int t = layer2_scroll_x & 0xFE;
    HIBYTE(temp933) = 0;
    LOBYTE(temp933) = t * LOBYTE(layer1_x_pos) >> 8;
    layer2_x_pos = t * HIBYTE(layer1_x_pos) + temp933;
    return 0;
  }
  return 1;
}

uint8 CalculateLayer2Ypos(void) {  // 0x80A33A
  if (!layer2_scroll_y) {
    layer2_y_pos = layer1_y_pos;
    return 0;
  }
  if (layer2_scroll_y != 1) {
    int t = layer2_scroll_y & 0xFE;
    HIBYTE(temp933) = 0;
    LOBYTE(temp933) = t * (uint8)layer1_y_pos >> 8;
    layer2_y_pos = t * HIBYTE(layer1_y_pos) + temp933;
    return 0;
  }
  return 1;
}

void CalculateBgScrolls(void) {  // 0x80A37B
  reg_BG1HOFS = bg1_x_offset + layer1_x_pos;
  reg_BG1VOFS = bg1_y_offset + layer1_y_pos;
  reg_BG2HOFS = bg2_x_scroll + layer2_x_pos;
  reg_BG2VOFS = bg2_y_scroll + layer2_y_pos;
}

void UpdateScrollVarsUpdateMap(void) {  // 0x80A3A0
  CalculateBgScrolls();
  UpdateBgGraphicsWhenScrolling();
}

void CalculateLayer2PosAndScrollsWhenScrolling(void) {  // 0x80A3AB
  if (!time_is_frozen_flag) {
    reg_BG1HOFS = bg1_x_offset + layer1_x_pos;
    reg_BG1VOFS = bg1_y_offset + layer1_y_pos;
    if (!CalculateLayer2Xpos())
      reg_BG2HOFS = bg2_x_scroll + layer2_x_pos;
    if (!CalculateLayer2Ypos())
      reg_BG2VOFS = bg2_y_scroll + layer2_y_pos;
    UpdateBgGraphicsWhenScrolling();
  }
}

void UpdateBgGraphicsWhenScrolling(void) {  // 0x80A3DF
  CalculateBgScrollAndLayerPositionBlocks();
  int v0 = 0;
  bool v1 = (int16)(layer1_x_block - previous_layer1_x_block) < 0;
  if (layer1_x_block != previous_layer1_x_block) {
    previous_layer1_x_block = layer1_x_block;
    if (!v1)
      v0 = 16;
    blocks_to_update_x_block = layer1_x_block + v0;
    vram_blocks_to_update_x_block = bg1_x_block + v0;
    blocks_to_update_y_block = layer1_y_block;
    vram_blocks_to_update_y_block = bg1_y_block;
    UploadLevelDataColumn();
  }
  if (!(layer2_scroll_x & 1)) {
    int v2 = 0;
    bool v3 = (int16)(layer2_x_block - previous_layer2_x_block) < 0;
    if (layer2_x_block != previous_layer2_x_block) {
      previous_layer2_x_block = layer2_x_block;
      if (!v3)
        v2 = 16;
      blocks_to_update_x_block = layer2_x_block + v2;
      vram_blocks_to_update_x_block = bg2_x_block + v2;
      blocks_to_update_y_block = layer2_y_block;
      vram_blocks_to_update_y_block = bg2_y_block;
      UploadBackgroundDataColumn();
    }
  }
  int v4 = 1;
  bool v5 = (int16)(layer1_y_block - previous_layer1_y_block) < 0;
  if (layer1_y_block != previous_layer1_y_block) {
    previous_layer1_y_block = layer1_y_block;
    if (!v5)
      v4 = 15;
    blocks_to_update_y_block = layer1_y_block + v4;
    vram_blocks_to_update_y_block = bg1_y_block + v4;
    blocks_to_update_x_block = layer1_x_block;
    vram_blocks_to_update_x_block = bg1_x_block;
    UpdateLevelDataRow();
  }
  if (!(layer2_scroll_y & 1)) {
    int v6 = 1;
    bool v7 = (int16)(layer2_y_block - previous_layer2_y_block) < 0;
    if (layer2_y_block != previous_layer2_y_block) {
      previous_layer2_y_block = layer2_y_block;
      if (!v7)
        v6 = 15;
      blocks_to_update_y_block = layer2_y_block + v6;
      vram_blocks_to_update_y_block = bg2_y_block + v6;
      blocks_to_update_x_block = layer2_x_block;
      vram_blocks_to_update_x_block = bg2_x_block;
      UpdateBackgroundDataRow();
    }
  }
}

void CalculateBgScrollAndLayerPositionBlocks(void) {  // 0x80A4BB
  bg1_x_block = reg_BG1HOFS >> 4;
  bg2_x_block = reg_BG2HOFS >> 4;
  uint16 v0 = layer1_x_pos >> 4;
  if (((layer1_x_pos >> 4) & 0x800) != 0)
    v0 |= 0xF000u;
  layer1_x_block = v0;
  uint16 v1 = layer2_x_pos >> 4;
  if (((layer2_x_pos >> 4) & 0x800) != 0)
    v1 |= 0xF000u;
  layer2_x_block = v1;
  bg1_y_block = reg_BG1VOFS >> 4;
  bg2_y_block = reg_BG2VOFS >> 4;
  uint16 v2 = layer1_y_pos >> 4;
  if (((layer1_y_pos >> 4) & 0x800) != 0)
    v2 |= 0xF000u;
  layer1_y_block = v2;
  uint16 v3 = layer2_y_pos >> 4;
  if (((layer2_y_pos >> 4) & 0x800) != 0)
    v3 |= 0xF000u;
  layer2_y_block = v3;
}

void HandleAutoscrolling_X(void) {  // 0x80A528
  int16 v1;
  int16 v3;
  int16 v6;
  uint16 v4;

  if (!time_is_frozen_flag) {
    loopcounter = layer1_x_pos;
    if ((layer1_x_pos & 0x8000u) != 0)
      layer1_x_pos = 0;
    uint16 v0 = swap16(room_width_in_scrolls - 1);
    if (v0 < layer1_x_pos)
      layer1_x_pos = v0;
    v1 = HIBYTE(layer1_x_pos);
    uint16 v2 = Mult8x8((uint16)(layer1_y_pos + 128) >> 8, room_width_in_scrolls) + v1;
    if (scrolls[v2]) {
      if (scrolls[(uint16)(v2 + 1)])
        return;
      temp933 = layer1_x_pos & 0xFF00;
      uint16 v5 = (__PAIR32__(loopcounter - absolute_moved_last_frame_x, loopcounter) - (absolute_moved_last_frame_x | 0x20000)) >> 16;

      if ((int16)(v5 - (layer1_x_pos & 0xFF00)) < 0) {
        v4 = temp933;
      } else {
        loopcounter = v5;
        v6 = HIBYTE(loopcounter);
        if (scrolls[(uint16)(Mult8x8((uint16)(layer1_y_pos + 128) >> 8, room_width_in_scrolls) + v6)])
          v4 = loopcounter;
        else
          v4 = (loopcounter & 0xFF00) + 256;
      }
    } else {
      temp933 = (layer1_x_pos & 0xFF00) + 256;
      if ((uint16)(absolute_moved_last_frame_x
                   + loopcounter
                   + 2) >= temp933) {
        v4 = temp933;
      } else {
        loopcounter += absolute_moved_last_frame_x + 2;
        v3 = (uint8)(HIBYTE(loopcounter) + 1);
        if (scrolls[(uint16)(Mult8x8((uint16)(layer1_y_pos + 128) >> 8, room_width_in_scrolls) + v3)])
          v4 = loopcounter;
        else
          v4 = loopcounter & 0xFF00;
      }
    }
    layer1_x_pos = v4;
  }
}

void HandleScrollingWhenTriggeringScrollRight(void) {  // 0x80A641
  int16 v1;

  loopcounter = layer1_x_pos;
  if ((int16)(ideal_layer1_xpos - layer1_x_pos) < 0) {
    layer1_x_pos = ideal_layer1_xpos;
    layer1_x_subpos = 0;
  }
  uint16 v0 = swap16(room_width_in_scrolls - 1);
  if (v0 >= layer1_x_pos) {
    v1 = HIBYTE(layer1_x_pos);
    uint16 RegWord = Mult8x8((uint16)(layer1_y_pos + 128) >> 8, room_width_in_scrolls);
    if (!scrolls[(uint16)(RegWord + 1 + v1)]) {
      temp933 = layer1_x_pos & 0xFF00;
      uint16 v4 = (__PAIR32__(loopcounter - absolute_moved_last_frame_x, loopcounter) - (absolute_moved_last_frame_x | 0x20000)) >> 16;
      if ((int16)(v4 - (layer1_x_pos & 0xFF00)) < 0)
        v4 = temp933;
      layer1_x_pos = v4;
    }
  } else {
    layer1_x_pos = v0;
  }
}

void HandleScrollingWhenTriggeringScrollLeft(void) {  // 0x80A6BB
  int16 v0;

  loopcounter = layer1_x_pos;
  if ((int16)(layer1_x_pos - ideal_layer1_xpos) < 0) {
    layer1_x_pos = ideal_layer1_xpos;
    layer1_x_subpos = 0;
  }
  if ((layer1_x_pos & 0x8000u) == 0) {
    v0 = HIBYTE(layer1_x_pos);
    uint16 prod = Mult8x8((uint16)(layer1_y_pos + 128) >> 8, room_width_in_scrolls);
    if (!scrolls[(uint16)(prod + v0)]) {
      temp933 = (layer1_x_pos & 0xFF00) + 256;
      uint16 v1 = absolute_moved_last_frame_x + loopcounter + 2;
      if (v1 >= temp933)
        v1 = temp933;
      layer1_x_pos = v1;
    }
  } else {
    layer1_x_pos = 0;
  }
}

void HandleAutoscrolling_Y(void) {  // 0x80A731
  int16 v1;
  int16 v2;
  int16 v4;
  int16 v7;
  int16 v10;
  uint16 v8;

  if (!time_is_frozen_flag) {
    uint16 v0 = 0;
    v1 = (uint16)(layer1_x_pos + 128) >> 8;
    R20_ = Mult8x8(HIBYTE(layer1_y_pos), room_width_in_scrolls) + v1;
    if (scrolls[R20_] != 1)
      v0 = 31;
    temp933 = v0;
    loopcounter = layer1_y_pos;
    if ((layer1_y_pos & 0x8000u) != 0)
      layer1_y_pos = 0;
    LOBYTE(v2) = (uint16)(room_height_in_scrolls - 1) >> 8;
    HIBYTE(v2) = room_height_in_scrolls - 1;
    uint16 v3 = temp933 + v2;
    if (v3 < layer1_y_pos)
      layer1_y_pos = v3;
    v4 = (uint16)(layer1_x_pos + 128) >> 8;
    uint16 v5 = Mult8x8(HIBYTE(layer1_y_pos), room_width_in_scrolls) + v4;
    if (!scrolls[v5]) {
      x_block_of_vram_blocks_to_update = (layer1_y_pos & 0xFF00) + 256;
      uint16 v6 = absolute_moved_last_frame_y + loopcounter + 2;
      if (v6 >= x_block_of_vram_blocks_to_update) {
        v8 = x_block_of_vram_blocks_to_update;
      } else {
        loopcounter += absolute_moved_last_frame_y + 2;
        v7 = (uint16)(layer1_x_pos + 128) >> 8;
        uint16 prod = Mult8x8(HIBYTE(v6) + 1, room_width_in_scrolls);
        if (scrolls[(uint16)(prod + v7)])
          v8 = loopcounter;
        else
          v8 = loopcounter & 0xFF00;
      }
      layer1_y_pos = v8;
      return;
    }
    if (!scrolls[(uint16)(room_width_in_scrolls + v5)]) {
      tmp_vram_base_addr = temp933 + (layer1_y_pos & 0xFF00);
      if (tmp_vram_base_addr < layer1_y_pos) {
        uint16 v9 = (__PAIR32__(loopcounter - absolute_moved_last_frame_y, loopcounter) - (absolute_moved_last_frame_y | 0x20000)) >> 16;
        if ((int16)(v9 - tmp_vram_base_addr) < 0) {
          v8 = tmp_vram_base_addr;
        } else {
          loopcounter = v9;
          uint16 prod = Mult8x8(HIBYTE(v9), room_width_in_scrolls);
          v10 = (uint16)(layer1_x_pos + 128) >> 8;
          if (scrolls[(uint16)(prod + v10)])
            v8 = loopcounter;
          else
            v8 = (loopcounter & 0xFF00) + 256;
        }
        layer1_y_pos = v8;
        return;
      }
    }
  }
}

void HandleScrollingWhenTriggeringScrollDown(void) {  // 0x80A893
  loopcounter = layer1_y_pos;
  int v0 = 0;
  uint16 prod = Mult8x8(HIBYTE(layer1_y_pos), room_width_in_scrolls);
  uint16 v1 = (uint16)(layer1_x_pos + 128) >> 8;
  R20_ = prod + v1;
  if (scrolls[R20_] != 1)
    v0 = 31;
  temp933 = v0;
  if ((int16)(ideal_layer1_ypos - layer1_y_pos) < 0) {
    layer1_y_pos = ideal_layer1_ypos;
    layer1_y_subpos = 0;
  }
  uint16 v2 = swap16(room_height_in_scrolls - 1);
  tmp_vram_base_addr = temp933 + v2;
  if ((uint16)(temp933 + v2) < layer1_y_pos
      || !scrolls[(uint16)(room_width_in_scrolls + R20_)]
      && (tmp_vram_base_addr = temp933 + (layer1_y_pos & 0xFF00), tmp_vram_base_addr < layer1_y_pos)) {
    uint16 v3 = (__PAIR32__(loopcounter - absolute_moved_last_frame_y, loopcounter) - (absolute_moved_last_frame_y | 0x20000)) >> 16;
    if ((int16)(v3 - tmp_vram_base_addr) < 0)
      v3 = tmp_vram_base_addr;
    layer1_y_pos = v3;
  }
}

void HandleScrollingWhenTriggeringScrollUp(void) {  // 0x80A936
  int16 v0;

  loopcounter = layer1_y_pos;
  if ((int16)(layer1_y_pos - ideal_layer1_ypos) < 0) {
    layer1_y_pos = ideal_layer1_ypos;
    layer1_y_subpos = 0;
  }
  if ((layer1_y_pos & 0x8000u) == 0) {
    uint16 prod = Mult8x8(HIBYTE(layer1_y_pos), room_width_in_scrolls);
    v0 = (uint16)(layer1_x_pos + 128) >> 8;
    if (!scrolls[(uint16)(prod + v0)]) {
      temp933 = (layer1_y_pos & 0xFF00) + 256;
      uint16 v1 = absolute_moved_last_frame_y + loopcounter + 2;
      if (v1 >= temp933)
        v1 = temp933;
      layer1_y_pos = v1;
    }
  } else {
    layer1_y_pos = 0;
  }
}

void DebugScrollPosSaveLoad(void) {  // 0x80A9AC
  if ((joypad2_new_keys & 0x40) != 0)
    ++debug_saveload_scrollpos_toggle;
  if (debug_saveload_scrollpos_toggle & 1) {
    layer1_x_pos = debug_saved_xscroll;
    layer1_y_pos = debug_saved_yscroll;
  } else {
    debug_saved_xscroll = layer1_x_pos;
    debug_saved_yscroll = layer1_y_pos;
  }
}

void UploadBackgroundDataColumn(void) {  // 0x80A9D6
  UpdateLevelOrBackgroundDataColumn(0x1c);
}

void UploadLevelDataColumn(void) {  // 0x80A9DB
  UpdateLevelOrBackgroundDataColumn(0);
}

void UpdateLevelOrBackgroundDataColumn(uint16 k) {  // 0x80A9DE
  if (!irq_enable_mode7) {
    uint16 prod = Mult8x8(blocks_to_update_y_block, room_width_in_blocks);
    uint16 v1 = blocks_to_update_x_block;
    uint16 v2 = 2 * (prod + v1) + 2;
    if (k)
      v2 += 0x9600;
    copywithflip_src.addr = v2;
    copywithflip_src.bank = 127;
    uint16 v3 = (4 * (uint8)vram_blocks_to_update_y_block) & 0x3C;
    *(uint16 *)((char *)&bg1_update_col_wrapped_size + k) = v3;
    *(uint16 *)((char *)&bg1_update_col_unwrapped_size + k) = (v3 ^ 0x3F) + 1;
    prod = Mult8x8(vram_blocks_to_update_y_block & 0xF, 0x40);
    x_block_of_vram_blocks_to_update = vram_blocks_to_update_x_block & 0x1F;
    uint16 v4 = 2 * x_block_of_vram_blocks_to_update;
    temp933 = prod + v4;
    uint16 v5 = addr_unk_605000;
    if (x_block_of_vram_blocks_to_update >= 0x10u)
      v5 = addr_unk_6053E0;
    if (k)
      v5 -= size_of_bg2;
    tmp_vram_base_addr = v5;
    *(uint16 *)((char *)&bg1_update_col_unwrapped_dst + k) = temp933 + v5;
    *(uint16 *)((char *)&bg1_update_col_wrapped_dst + k) = x_block_of_vram_blocks_to_update
      + x_block_of_vram_blocks_to_update
      + tmp_vram_base_addr;
    uint16 v6 = ADDR16_OF_RAM(*bg1_column_update_tilemap_left_halves);
    uint16 v7 = 0;
    if (k) {
      v6 = ADDR16_OF_RAM(*bg2_column_update_tilemap_left_halves);
      v7 = 264;
    }
    uint16 v8 = *(uint16 *)((char *)&bg1_update_col_unwrapped_size + k) + v6;
    *(uint16 *)((char *)&bg1_update_col_wrapped_left_src + k) = v8;
    *(uint16 *)((char *)&bg1_update_col_wrapped_right_src + k) = v8 + 64;
    tmp_vram_base_addr = v7;
    uint16 t2 = k;
    uint16 v9 = 0;
    loopcounter = 16;
    do {
      tmp_block_to_update = IndirReadWord(copywithflip_src, v9);
      uint16 v10 = tmp_block_to_update & 0x3FF;
      uint16 v17 = v9;
      uint16 v11 = tmp_vram_base_addr;
      uint16 v12 = tmp_block_to_update & 0xC00;
      if ((tmp_block_to_update & 0xC00) != 0) {
        if (v12 == 1024) {
          uint16 v14 = tmp_vram_base_addr >> 1;
          bg1_column_update_tilemap_left_halves[v14] = tile_table.tables[v10].top_right ^ 0x4000;
          bg1_column_update_tilemap_right_halves[v14] = tile_table.tables[v10].top_left ^ 0x4000;
          bg1_column_update_tilemap_left_halves[v14 + 1] = tile_table.tables[v10].bottom_right ^ 0x4000;
          bg1_column_update_tilemap_right_halves[v14 + 1] = tile_table.tables[v10].bottom_left ^ 0x4000;
        } else {
          uint16 v15 = tmp_vram_base_addr >> 1;
          uint16 v16;
          if (v12 == 2048) {
            bg1_column_update_tilemap_left_halves[v15] = tile_table.tables[v10].bottom_left ^ 0x8000;
            bg1_column_update_tilemap_right_halves[v15] = tile_table.tables[v10].bottom_right ^ 0x8000;
            bg1_column_update_tilemap_left_halves[v15 + 1] = tile_table.tables[v10].top_left ^ 0x8000;
            v16 = tile_table.tables[v10].top_right ^ 0x8000;
          } else {
            bg1_column_update_tilemap_left_halves[v15] = tile_table.tables[v10].bottom_right ^ 0xC000;
            bg1_column_update_tilemap_right_halves[v15] = tile_table.tables[v10].bottom_left ^ 0xC000;
            bg1_column_update_tilemap_left_halves[v15 + 1] = tile_table.tables[v10].top_right ^ 0xC000;
            v16 = tile_table.tables[v10].top_left ^ 0xC000;
          }
          bg1_column_update_tilemap_right_halves[v15 + 1] = v16;
        }
      } else {
        uint16 v13 = tmp_vram_base_addr >> 1;
        bg1_column_update_tilemap_left_halves[v13] = tile_table.tables[v10].top_left;
        bg1_column_update_tilemap_right_halves[v13] = tile_table.tables[v10].top_right;
        bg1_column_update_tilemap_left_halves[v13 + 1] = tile_table.tables[v10].bottom_left;
        bg1_column_update_tilemap_right_halves[v13 + 1] = tile_table.tables[v10].bottom_right;
      }
      tmp_vram_base_addr = v11 + 4;
      v9 = room_width_in_blocks * 2 + v17;
      --loopcounter;
    } while (loopcounter);
    ++ *(uint16 *)((char *)&bg1_update_col_enable + t2);
  }
}

void UpdateBackgroundDataRow(void) {  // 0x80AB70
  UpdateLevelOrBackgroundDataRow(0x1c);
}

void UpdateLevelDataRow(void) {  // 0x80AB75
  UpdateLevelOrBackgroundDataRow(0);
}

void UpdateLevelOrBackgroundDataRow(uint16 v0) {  // 0x80AB78
  if (!irq_enable_mode7) {
    uint16 prod = Mult8x8(blocks_to_update_y_block, room_width_in_blocks);
    uint16 v1 = blocks_to_update_x_block;
    uint16 v2 = 2 * (prod + v1) + 2;
    if (v0)
      v2 -= 27136;
    copywithflip_src.addr = v2;
    copywithflip_src.bank = 127;
    temp933 = vram_blocks_to_update_x_block & 0xF;
    *(uint16 *)((char *)&bg1_update_row_unwrapped_size + v0) = 4 * (16 - temp933);
    *(uint16 *)((char *)&bg1_update_row_wrapped_size + v0) = 4 * (temp933 + 1);
    prod = Mult8x8(vram_blocks_to_update_y_block & 0xF, 0x40);
    x_block_of_vram_blocks_to_update = vram_blocks_to_update_x_block & 0x1F;
    uint16 v3 = 2 * x_block_of_vram_blocks_to_update;
    temp933 = prod + v3;
    tmp_vram_base_addr = addr_unk_605400;
    uint16 v4 = addr_unk_605000;
    if (x_block_of_vram_blocks_to_update >= 0x10u) {
      tmp_vram_base_addr = addr_unk_605000;
      v4 = addr_unk_6053E0;
    }
    if (v0)
      v4 -= size_of_bg2;
    *(uint16 *)((char *)&bg1_update_row_unwrapped_dst + v0) = temp933 + v4;
    uint16 v5 = tmp_vram_base_addr;
    if (v0)
      v5 = tmp_vram_base_addr - size_of_bg2;
    *(uint16 *)((char *)&bg1_update_row_wrapped_dst + v0) = prod + v5;
    uint16 v6 = ADDR16_OF_RAM(*bg1_column_update_tilemap_top_halves);
    uint16 v7 = 0;
    if (v0) {
      v6 = ADDR16_OF_RAM(*bg2_column_update_tilemap_top_halves);
      v7 = 264;
    }
    uint16 v8 = *(uint16 *)((char *)&bg1_update_row_unwrapped_size + v0) + v6;
    *(uint16 *)((char *)&bg1_update_row_wrapped_top_src + v0) = v8;
    *(uint16 *)((char *)&bg1_update_row_wrapped_bottom_src + v0) = v8 + 68;
    tmp_vram_base_addr = v7;
    uint16 t2 = v0;
    uint16 v9 = 0;
    loopcounter = 17;
    do {
      tmp_block_to_update = IndirReadWord(copywithflip_src, v9);
      uint16 v10 = tmp_block_to_update & 0x3FF;
      uint16 v17 = v9;
      uint16 v11 = tmp_vram_base_addr;
      uint16 v12 = tmp_block_to_update & 0xC00;
      if ((tmp_block_to_update & 0xC00) != 0) {
        if (v12 == 1024) {
          uint16 v14 = tmp_vram_base_addr >> 1;
          bg1_column_update_tilemap_top_halves[v14] = tile_table.tables[v10].top_right ^ 0x4000;
          bg1_column_update_tilemap_top_halves[v14 + 1] = tile_table.tables[v10].top_left ^ 0x4000;
          bg1_column_update_tilemap_bottom_halves[v14] = tile_table.tables[v10].bottom_right ^ 0x4000;
          bg1_column_update_tilemap_bottom_halves[v14 + 1] = tile_table.tables[v10].bottom_left ^ 0x4000;
        } else {
          uint16 v15 = tmp_vram_base_addr >> 1;
          uint16 v16;
          if (v12 == 2048) {
            bg1_column_update_tilemap_top_halves[v15] = tile_table.tables[v10].bottom_left ^ 0x8000;
            bg1_column_update_tilemap_top_halves[v15 + 1] = tile_table.tables[v10].bottom_right ^ 0x8000;
            bg1_column_update_tilemap_bottom_halves[v15] = tile_table.tables[v10].top_left ^ 0x8000;
            v16 = tile_table.tables[v10].top_right ^ 0x8000;
          } else {
            bg1_column_update_tilemap_top_halves[v15] = tile_table.tables[v10].bottom_right ^ 0xC000;
            bg1_column_update_tilemap_top_halves[v15 + 1] = tile_table.tables[v10].bottom_left ^ 0xC000;
            bg1_column_update_tilemap_bottom_halves[v15] = tile_table.tables[v10].top_right ^ 0xC000;
            v16 = tile_table.tables[v10].top_left ^ 0xC000;
          }
          bg1_column_update_tilemap_bottom_halves[v15 + 1] = v16;
        }
      } else {
        uint16 v13 = tmp_vram_base_addr >> 1;
        bg1_column_update_tilemap_top_halves[v13] = tile_table.tables[v10].top_left;
        bg1_column_update_tilemap_top_halves[v13 + 1] = tile_table.tables[v10].top_right;
        bg1_column_update_tilemap_bottom_halves[v13] = tile_table.tables[v10].bottom_left;
        bg1_column_update_tilemap_bottom_halves[v13 + 1] = tile_table.tables[v10].bottom_right;
      }
      tmp_vram_base_addr = v11 + 4;
      v9 = v17 + 2;
      --loopcounter;
    } while (loopcounter);
    ++ *(uint16 *)((char *)&bg1_update_row_enable + t2);
  }
}

void FixDoorsMovingUp(void) {  // 0x80AD1D
  door_transition_frame_counter = 0;
  CalculateBgScrollAndLayerPositionBlocks();
  UpdatePreviousLayerBlocks();
  ++previous_layer1_y_block;
  ++previous_layer2_y_block;
  DoorTransition_Up();
}

static Func_V *const kDoorTransitionSetupFuncs[4] = {  // 0x80AD30
  DoorTransitionScrollingSetup_Right,
  DoorTransitionScrollingSetup_Left,
  DoorTransitionScrollingSetup_Down,
  DoorTransitionScrollingSetup_Up,
};

void DoorTransitionScrollingSetup(void) {
  layer1_x_pos = door_destination_x_pos;
  layer1_y_pos = door_destination_y_pos;
  kDoorTransitionSetupFuncs[door_direction & 3]();
}

void DoorTransitionScrollingSetup_Right(void) {  // 0x80AD4A
  CalculateLayer2Xpos();
  layer2_x_pos -= 256;
  CalculateLayer2Ypos();
  layer1_x_pos -= 256;
  UpdateBgScrollOffsets();
  CalculateBgScrollAndLayerPositionBlocks();
  UpdatePreviousLayerBlocks();
  --previous_layer1_x_block;
  --previous_layer2_x_block;
  DoorTransition_Right();
}

void DoorTransitionScrollingSetup_Left(void) {  // 0x80AD74
  CalculateLayer2Xpos();
  layer2_x_pos += 256;
  CalculateLayer2Ypos();
  layer1_x_pos += 256;
  UpdateBgScrollOffsets();
  CalculateBgScrollAndLayerPositionBlocks();
  UpdatePreviousLayerBlocks();
  ++previous_layer1_x_block;
  ++previous_layer2_x_block;
  DoorTransition_Left();
}

void DoorTransitionScrollingSetup_Down(void) {  // 0x80AD9E
  CalculateLayer2Xpos();
  CalculateLayer2Ypos();
  layer2_y_pos -= 224;
  layer1_y_pos -= 224;
  UpdateBgScrollOffsets();
  CalculateBgScrollAndLayerPositionBlocks();
  UpdatePreviousLayerBlocks();
  --previous_layer1_y_block;
  --previous_layer2_y_block;
  DoorTransition_Down();
}

void DoorTransitionScrollingSetup_Up(void) {  // 0x80ADC8
  CalculateLayer2Xpos();
  uint16 v1 = layer1_y_pos;
  layer1_y_pos += 31;
  CalculateLayer2Ypos();
  layer2_y_pos += 224;
  layer1_y_pos = v1 + 256;
  UpdateBgScrollOffsets();
  door_destination_y_pos += 32;
  CalculateBgScrollAndLayerPositionBlocks();
  UpdatePreviousLayerBlocks();
  ++previous_layer1_y_block;
  ++previous_layer2_y_block;
  --layer1_y_pos;
  DoorTransition_Up();
}

void UpdatePreviousLayerBlocks(void) {  // 0x80AE10
  previous_layer1_x_block = layer1_x_block;
  previous_layer2_x_block = layer2_x_block;
  previous_layer1_y_block = layer1_y_block;
  previous_layer2_y_block = layer2_y_block;
}

void UpdateBgScrollOffsets(void) {  // 0x80AE29
  bg1_x_offset = reg_BG1HOFS - layer1_x_pos;
  bg1_y_offset = reg_BG1VOFS - layer1_y_pos;
  bg2_x_scroll = reg_BG2HOFS - layer1_x_pos;
  bg2_y_scroll = reg_BG2VOFS - layer1_y_pos;
}

static Func_U8 *const kDoorTransitionFuncs[4] = {
  DoorTransition_Right,
  DoorTransition_Left,
  DoorTransition_Down,
  DoorTransition_Up,
};

void Irq_FollowDoorTransition(void) {
  if (kDoorTransitionFuncs[door_direction & 3]()) {
    layer1_x_pos = door_destination_x_pos;
    layer1_y_pos = door_destination_y_pos;
    door_transition_flag |= 0x8000u;
  }
}

uint8 DoorTransition_Right(void) {  // 0x80AE7E
  uint16 v2 = door_transition_frame_counter;
  uint16 v0 = (__PAIR32__(samus_door_transition_speed, samus_door_transition_subspeed) +
               __PAIR32__(samus_x_pos, samus_x_subpos)) >> 16;
  samus_x_subpos += samus_door_transition_subspeed;
  samus_x_pos = v0;
  samus_prev_x_pos = v0;
  layer1_x_pos += 4;
  layer2_x_pos += 4;
  UpdateScrollVarsUpdateMap();
  door_transition_frame_counter = v2 + 1;
  if (v2 != 63)
    return 0;
  UpdateScrollVarsUpdateMap();
  return 1;
}

uint8 DoorTransition_Left(void) {  // 0x80AEC2
  uint16 v2 = door_transition_frame_counter;
  uint16 v0 = (__PAIR32__(samus_x_pos, samus_x_subpos) - __PAIR32__(
    samus_door_transition_speed,
    samus_door_transition_subspeed)) >> 16;
  samus_x_subpos -= samus_door_transition_subspeed;
  samus_x_pos = v0;
  samus_prev_x_pos = v0;
  layer1_x_pos -= 4;
  layer2_x_pos -= 4;
  UpdateScrollVarsUpdateMap();
  door_transition_frame_counter = v2 + 1;
  return v2 == 63;
}

uint8 DoorTransition_Down(void) {  // 0x80AF02
  uint16 v6 = door_transition_frame_counter;
  if (door_transition_frame_counter) {
    if (door_transition_frame_counter < 0x39u) {
      uint16 v0 = (__PAIR32__(samus_door_transition_speed, samus_door_transition_subspeed)
                   + __PAIR32__(samus_y_pos, samus_y_subpos)) >> 16;
      samus_y_subpos += samus_door_transition_subspeed;
      samus_y_pos = v0;
      samus_prev_y_pos = v0;
      layer1_y_pos += 4;
      layer2_y_pos += 4;
      UpdateScrollVarsUpdateMap();
    }
  } else {
    uint16 v5 = reg_BG1VOFS;
    uint16 v4 = reg_BG2VOFS;
    uint16 v3 = layer1_y_pos;
    layer1_y_pos -= 15;
    uint16 v2 = layer2_y_pos;
    layer2_y_pos -= 15;
    CalculateBgScrollAndLayerPositionBlocks();
    UpdatePreviousLayerBlocks();
    --previous_layer1_y_block;
    --previous_layer2_y_block;
    UpdateScrollVarsUpdateMap();
    layer2_y_pos = v2;
    layer1_y_pos = v3;
    reg_BG2VOFS = v4;
    reg_BG1VOFS = v5;
  }
  door_transition_frame_counter = v6 + 1;
  if ((uint16)(v6 + 1) < 0x39u)
    return 0;
  UpdateScrollVarsUpdateMap();
  return 1;
}

uint8 DoorTransition_Up(void) {  // 0x80AF89
  uint16 v6 = door_transition_frame_counter;
  if (door_transition_frame_counter) {
    uint16 v0 = (__PAIR32__(samus_y_pos, samus_y_subpos)
                 - __PAIR32__(samus_door_transition_speed, samus_door_transition_subspeed)) >> 16;
    samus_y_subpos -= samus_door_transition_subspeed;
    samus_y_pos = v0;
    samus_prev_y_pos = v0;
    layer1_y_pos -= 4;
    layer2_y_pos -= 4;
    if (door_transition_frame_counter >= 5u) {
      UpdateScrollVarsUpdateMap();
    } else {
      reg_BG1HOFS = bg1_x_offset + layer1_x_pos;
      reg_BG1VOFS = bg1_y_offset + layer1_y_pos;
      reg_BG2HOFS = bg2_x_scroll + layer2_x_pos;
      reg_BG2VOFS = bg2_y_scroll + layer2_y_pos;
    }
  } else {
    uint16 v5 = reg_BG1VOFS;
    uint16 v4 = reg_BG2VOFS;
    uint16 v3 = layer1_y_pos;
    layer1_y_pos -= 16;
    uint16 v2 = layer2_y_pos;
    layer2_y_pos -= 16;
    CalculateBgScrollAndLayerPositionBlocks();
    UpdatePreviousLayerBlocks();
    ++previous_layer1_y_block;
    ++previous_layer2_y_block;
    UpdateScrollVarsUpdateMap();
    layer2_y_pos = v2;
    layer1_y_pos = v3;
    reg_BG2VOFS = v4;
    reg_BG1VOFS = v5;
  }
  door_transition_frame_counter = v6 + 1;
  return v6 == 56;
}

void ConfigureMode7RotationMatrix(void) {  // 0x80B0C2
  if (irq_enable_mode7) {
    if ((nmi_frame_counter_word & 7) == 0) {
      reg_M7B = kSinCosTable8bit_Sext[((uint8)mode7_rotation_angle) + 64];
      reg_M7C = -reg_M7B;
      reg_M7A = kSinCosTable8bit_Sext[((uint8)(mode7_rotation_angle + 64)) + 64];
      reg_M7D = reg_M7A;
      ++mode7_rotation_angle;
    }
  }
}

static uint32 decompress_src;

static uint8 DecompNextByte() {
  uint8 b = *RomPtrWithBank(decompress_src >> 16, decompress_src);
  if ((decompress_src++ & 0xffff) == 0xffff)
    decompress_src += 0x8000;
  return b;
}

void DecompressToMem(uint32 src, uint8 *decompress_dst) {  // 0x80B119
  decompress_src = src;

  int src_pos, dst_pos = 0;
  while (1) {
    int len;
    uint8 cmd, b;
    b = DecompNextByte();
    if (b == 0xFF)
      break;
    if ((b & 0xE0) == 0xE0) {
      cmd = (8 * b) & 0xE0;
      len = ((b & 3) << 8 | DecompNextByte()) + 1;
    } else {
      cmd = b & 0xE0;
      len = (b & 0x1F) + 1;
    }
    if (cmd & 0x80) {
      uint8 want_xor = cmd & 0x20 ? 0xff : 0;
      if (cmd >= 0xC0) {
        src_pos = dst_pos - DecompNextByte();
      } else {
        src_pos = DecompNextByte();
        src_pos += DecompNextByte() * 256;
      }
      do {
        b = decompress_dst[src_pos++] ^ want_xor;
        decompress_dst[dst_pos++] = b;
      } while (--len);
    } else {
      switch (cmd) {
      case 0x20:
        b = DecompNextByte();
        do {
          decompress_dst[dst_pos++] = b;
        } while (--len);
        break;
      case 0x40: {
        b = DecompNextByte();
        uint8 b2 = DecompNextByte();
        do {
          decompress_dst[dst_pos++] = b;
          if (!--len)
            break;
          decompress_dst[dst_pos++] = b2;
        } while (--len);
        break;
      }
      case 0x60:
        b = DecompNextByte();
        do {
          decompress_dst[dst_pos++] = b++;
        } while (--len);
        break;
      default:
        do {
          b = DecompNextByte();
          decompress_dst[dst_pos++] = b;
        } while (--len);
        break;
      }
    }
  }
}

static uint8 ReadPpuByte(uint16 addr) {
  WriteRegWord(VMADDL, addr >> 1);
  ReadRegWord(RDVRAML);  // latch
  uint16 data = ReadRegWord(RDVRAML);
  return (addr & 1) ? GET_HIBYTE(data) : data;
}

void DecompressToVRAM(uint32 src, uint16 dst_addr) {  // 0x80B271
  decompress_src = src;
  int src_pos, dst_pos = dst_addr;
  while (1) {
    int len;
    uint8 b = DecompNextByte(), cmd;
    if (b == 0xFF)
      break;
    if ((b & 0xE0) == 0xE0) {
      cmd = (8 * b) & 0xE0;
      len = ((b & 3) << 8 | DecompNextByte()) + 1;
    } else {
      cmd = b & 0xE0;
      len = (b & 0x1F) + 1;
    }
    if (cmd & 0x80) {
      uint8 want_xor = cmd & 0x20 ? 0xff : 0;
      if (cmd >= 0xC0) {
        src_pos = dst_pos - DecompNextByte();
      } else {
        src_pos = DecompNextByte();
        src_pos += DecompNextByte() * 256;
        src_pos += dst_addr;
      }
      do {
        b = ReadPpuByte(src_pos++) ^ want_xor;
        WriteRegWord(VMADDL, dst_pos >> 1);
        WriteReg(VMDATAL + (dst_pos++ & 1), b);
      } while (--len);
    } else {
      switch (cmd) {
      case 0x20:
        b = DecompNextByte();
        do {
          WriteReg(VMDATAL + (dst_pos++ & 1), b);
        } while (--len);
        break;
      case 0x40: {
        b = DecompNextByte();
        uint8 b2 = DecompNextByte();
        do {
          WriteReg(VMDATAL + (dst_pos++ & 1), b);
          if (!--len)
            break;
          WriteReg(VMDATAL + (dst_pos++ & 1), b2);
        } while (--len);
        break;
      }
      case 0x60:
        b = DecompNextByte();
        do {
          WriteReg(VMDATAL + (dst_pos++ & 1), b++);
        } while (--len);
        break;
      default:
        do {
          b = DecompNextByte();
          WriteReg(VMDATAL + (dst_pos++ & 1), b++);
        } while (--len);
        break;
      }
    }
  }
}


void LoadFromLoadStation(void) {  // 0x80C437
  save_station_lockout_flag = 1;
  R18_ = 2 * load_station_index;
  const uint16 *v0 = (const uint16 *)RomPtr_80(kLoadStationLists[area_index] + 14 * load_station_index);
  room_ptr = *v0;
  door_def_ptr = v0[1];
  door_bts = v0[2];
  layer1_x_pos = v0[3];
  bg1_x_offset = layer1_x_pos;
  layer1_y_pos = v0[4];
  bg1_y_offset = layer1_y_pos;
  samus_y_pos = layer1_y_pos + v0[5];
  samus_prev_y_pos = samus_y_pos;
  samus_x_pos = v0[6] + layer1_x_pos + 128;
  samus_prev_x_pos = samus_x_pos;
  reg_BG1HOFS = 0;
  reg_BG1VOFS = 0;
  LOBYTE(area_index) = get_RoomDefHeader(room_ptr)->area_index_;
  LOBYTE(debug_disable_minimap) = 0;
}


void SetElevatorsAsUsed(void) {  // 0x80CD07
  const uint8 *v0 = RomPtr_80(off_80CD46[area_index] + 4 * (((uint8)elevator_door_properties_orientation & 0xF) - 1));
  used_save_stations_and_elevators[v0[0]] |= v0[1];
  used_save_stations_and_elevators[v0[2]] |= v0[3];
}