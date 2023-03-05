// Enemy projectile spritemaps, palette FX objects
#include "ida_types.h"
#include "variables.h"
#include "funcs.h"

void EnablePaletteFx(void) {  // 0x8DC4C2
  flag_for_palette_fx_objects |= 0x8000u;
}

void DisablePaletteFx(void) {  // 0x8DC4CD
  flag_for_palette_fx_objects &= ~0x8000u;
}

void ClearPaletteFXObjects(void) {  // 0x8DC4D8
  for (int i = 14; i >= 0; i -= 2)
    palettefx_ids[i >> 1] = 0;
}

void CallPalfxFunc(uint32 ea, uint16 k, uint16 j) {
  switch (ea) {
  case fnnullsub_131: return;
  case fnPalInit_E1BC: PalInit_E1BC(k, j); return;
  case fnPalInit_F761_Norfair1: PalInit_F761_Norfair1(k, j); return;
  case fnPalInit_F779_Brinstar8: PalInit_F779_Brinstar8(k, j); return;
  default: Unreachable();
  }
}

void SpawnPalfxObject(uint16 j) {  // 0x8DC4E9
  PalFxDef *PalFxDef;

  uint16 v1 = 14;
  while (palettefx_ids[v1 >> 1]) {
    v1 -= 2;
    if ((v1 & 0x8000u) != 0)
      return;
  }
  int v2 = v1 >> 1;
  palettefx_ids[v2] = j;
  palettefx_color_indexes[v2] = 0;
  palettefx_pre_instr[v2] = FUNC16(PalPreInstr_nullsub_129);
  palettefx_instr_list_ptrs[v2] = get_PalFxDef(j)->instrs;
  palettefx_instr_timers[v2] = 1;
  palettefx_timers[v2] = 0;
  PalFxDef = get_PalFxDef(j);
  CallPalfxFunc(PalFxDef->func | 0x8D0000, -1, v1);
}

void PalPreInstr_nullsub_129(uint16 k) {  // 0x8DC526
}

void PaletteFxHandler(void) {  // 0x8DC527
  if ((flag_for_palette_fx_objects & 0x8000u) != 0) {
    for (int i = 14; i >= 0; i -= 2) {
      palettefx_index = i;
      if (palettefx_ids[i >> 1]) {
        PalFx_ProcessOne(i);
        i = palettefx_index;
      }
    }
  }
}

void CallPalFxPreInstr(uint32 ea, uint16 k) {
  switch (ea) {
  case fnPalPreInstr_nullsub_129: PalPreInstr_nullsub_129(k); return;
  case fnPalPreInstr_E1BC: PalPreInstr_E1BC(k); return;
  case fnPalPreInstr_CheckEnemy0Health: PalPreInstr_CheckEnemy0Health(k); return;
  case fnPalPreInstr_SamusInHeat: PalPreInstr_SamusInHeat(k); return;
  case fnPalPreInstr_DeletePalfxIfMinibossDead: PalPreInstr_DeletePalfxIfMinibossDead(k); return;
  case fnPalPreInstr_SwitchIfYpos: PalPreInstr_SwitchIfYpos(k); return;
  case fnPalPreInstr_SwitchIfYpos2: PalPreInstr_SwitchIfYpos2(k); return;
  case fnPalPreInstr_F621: PalPreInstr_F621(k); return;
  default: Unreachable();
  }
}

PairU16 CallPalInstr(uint32 ea, uint16 k, uint16 j) {
  switch (ea) {
  case fnPalInstr_Finish: return PalInstr_Finish(k, j);
  case fnPalInstr_ColorPlus2: return PalInstr_ColorPlus2(k, j);
  case fnPalInstr_ColorPlus3: return PalInstr_ColorPlus3(k, j);
  case fnPalInstr_ColorPlus4: return PalInstr_ColorPlus4(k, j);
  case fnPalInstr_ColorPlus8: return PalInstr_ColorPlus8(k, j);
  case fnPalInstr_ColorPlus9: return PalInstr_ColorPlus9(k, j);
  case fnPalInstr_ColorPlus15: return PalInstr_ColorPlus15(k, j);
  case fnPalInstr_Delete: return PalInstr_Delete(k, j);
  case fnPalInstr_SetPreInstr: return PalInstr_SetPreInstr(k, j);
  case fnPalInstr_ClearPreInstr: return PalInstr_ClearPreInstr(k, j);
  case fnPalInstr_Goto: return PalInstr_Goto(k, j);
  case fnPalInstr_GotoRel: return PalInstr_GotoRel(k, j);
  case fnPalInstr_DecTimerGoto: return PalInstr_DecTimerGoto(k, j);
  case fnPalInstr_DecTimerGotoRel: return PalInstr_DecTimerGotoRel(k, j);
  case fnPalInstr_SetTimer: return PalInstr_SetTimer(k, j);
  case fnPalInstr_SetColorIndex: return PalInstr_SetColorIndex(k, j);
  case fnPalInstr_QueueMusic: return PalInstr_QueueMusic(k, j);
  case fnPalInstr_QueueSfx1: return PalInstr_QueueSfx1(k, j);
  case fnPalInstr_QueueSfx2: return PalInstr_QueueSfx2(k, j);
  case fnPalInstr_QueueSfx3: return PalInstr_QueueSfx3(k, j);
  case fnPalInstr_SetTimerIfBossBit: return PalInstr_SetTimerIfBossBit(k, j);
  case fnPalInstr_SetPalfxIndex: return PalInstr_SetPalfxIndex(k, j);
  default: Unreachable(); return (PairU16){0, 0};
  }
}

void PalFx_ProcessOne(uint16 k) {  // 0x8DC54A
  PairU16 v7;
  int16 v11;
  PairU16 v12;

  CallPalFxPreInstr(palettefx_pre_instr[k >> 1] | 0x8D0000, k);
  uint16 v1 = palettefx_index;
  int v2 = palettefx_index >> 1;
  if (palettefx_instr_timers[v2]-- == 1) {
    uint16 j = palettefx_instr_list_ptrs[v2], v6;
    while (1) {
      uint16 *v5 = (uint16 *)RomPtr_8D(j);
      v6 = *v5;
      if ((*v5 & 0x8000u) == 0)
        break;
      R18_ = *v5;
      v7 = CallPalInstr(v6 | 0x8D0000, v1, j + 2);
      v1 = v7.k;
      j = v7.j;
      if (!v7.j)
        return;
    }
    int v8 = v1 >> 1;
    palettefx_instr_timers[v8] = v6;
    uint16 v9 = palettefx_color_indexes[v8];
    uint8 *v10;
    do {
      while (1) {
        v10 = RomPtr_8D(j);
        v11 = *((uint16 *)v10 + 1);
        if (v11 & 0x8000)
          break;
        palette_buffer[v9 >> 1] = v11;
        v9 += 2;
        j += 2;
      }
      R18_ = *((uint16 *)v10 + 1);
      v12 = CallPalInstr((uint16)v11 | 0x8D0000, v9, j);
      v9 = v12.k;
      j = v12.j;
    } while (v12.j);
  }
}

PairU16 PalInstr_Finish(uint16 k, uint16 j) {  // 0x8DC595
  palettefx_instr_list_ptrs[palettefx_index >> 1] = j + 4;
  return MakePairU16(0, 0);
}

PairU16 PalInstr_ColorPlus2(uint16 k, uint16 j) {  // 0x8DC599
  return MakePairU16(k + 4, j + 2);
}

PairU16 PalInstr_ColorPlus3(uint16 k, uint16 j) {  // 0x8DC5A2
  return MakePairU16(k + 6, j + 2);
}

PairU16 PalInstr_ColorPlus4(uint16 k, uint16 j) {  // 0x8DC5AB
  return MakePairU16(k + 8, j + 2);
}

PairU16 PalInstr_ColorPlus8(uint16 k, uint16 j) {  // 0x8DC5B4
  return MakePairU16(k + 16, j + 2);
}

PairU16 PalInstr_ColorPlus9(uint16 k, uint16 j) {  // 0x8DC5BD
  return MakePairU16(k + 18, j + 2);
}

PairU16 PalInstr_ColorPlus15(uint16 k, uint16 j) {  // 0x8DC5C6
  return MakePairU16(k + 30, j + 2);
}

PairU16 PalInstr_Delete(uint16 k, uint16 j) {  // 0x8DC5CF
  palettefx_ids[k >> 1] = 0;
  return MakePairU16(k, 0);
}

PairU16 PalInstr_SetPreInstr(uint16 k, uint16 j) {  // 0x8DC5D4
  palettefx_pre_instr[k >> 1] = *(uint16 *)RomPtr_8D(j);
  return MakePairU16(k, j + 2);
}

PairU16 PalInstr_ClearPreInstr(uint16 k, uint16 j) {  // 0x8DC5DD
  palettefx_pre_instr[k >> 1] = addr_locret_8DC5E3;
  return MakePairU16(k, j);
}

PairU16 PalInstr_Goto(uint16 k, uint16 j) {  // 0x8DC61E
  uint16 v2 = *(uint16 *)RomPtr_8D(j);
  return MakePairU16(k, v2);
}

PairU16 PalInstr_GotoRel(uint16 k, uint16 j) {  // 0x8DC623
  int16 v2;

  R18_ = j;
  LOBYTE(v2) = HIBYTE(*(uint16 *)RomPtr_8D(j - 1));
  if ((v2 & 0x80) != 0)
    v2 |= 0xFF00u;
  else
    v2 = (uint8)v2;
  return MakePairU16(k, R18_ + v2);
}

PairU16 PalInstr_DecTimerGoto(uint16 k, uint16 j) {  // 0x8DC639
  PairU16 v4;

  int v2 = k >> 1;
  if (palettefx_timers[v2]-- == 1)
    return MakePairU16(k, j + 2);
  v4 = PalInstr_Goto(k, j);
  return MakePairU16(v4.k, v4.j);
}

PairU16 PalInstr_DecTimerGotoRel(uint16 k, uint16 j) {  // 0x8DC641
  PairU16 v4;

  int v2 = k >> 1;
  if (palettefx_timers[v2]-- == 1)
    return MakePairU16(k, j + 1);
  v4 = PalInstr_GotoRel(k, j);
  return MakePairU16(v4.k, v4.j);
}

PairU16 PalInstr_SetTimer(uint16 k, uint16 j) {  // 0x8DC648
  *((uint8 *)palettefx_timers + k) = *RomPtr_8D(j);
  return MakePairU16(k, j + 1);
}

PairU16 PalInstr_SetColorIndex(uint16 k, uint16 j) {  // 0x8DC655
  palettefx_color_indexes[k >> 1] = *(uint16 *)RomPtr_8D(j);
  return MakePairU16(k, j + 2);
}

PairU16 PalInstr_QueueMusic(uint16 k, uint16 j) {  // 0x8DC65E
  uint8 *v2 = RomPtr_8D(j);
  QueueMusic_Delayed8(*v2);
  return MakePairU16(k, j + 1);
}

PairU16 PalInstr_QueueSfx1(uint16 k, uint16 j) {  // 0x8DC66A
  uint16 *v2 = (uint16 *)RomPtr_8D(j);
  QueueSfx1_Max6(*v2);
  return MakePairU16(k, j + 1);
}

PairU16 PalInstr_QueueSfx2(uint16 k, uint16 j) {  // 0x8DC673
  uint16 *v2 = (uint16 *)RomPtr_8D(j);
  QueueSfx2_Max6(*v2);
  return MakePairU16(k, j + 1);
}

PairU16 PalInstr_QueueSfx3(uint16 k, uint16 j) {  // 0x8DC67C
  uint16 *v2 = (uint16 *)RomPtr_8D(j);
  QueueSfx3_Max6(*v2);
  return MakePairU16(k, j + 1);
}

void PalInit_E1BC(uint16 k, uint16 j) {  // 0x8DE204
  palettefx_pre_instr[j >> 1] = FUNC16(PalPreInstr_E1BC);
}

void PalPreInstr_E1BC(uint16 k) {  // 0x8DE20B
  if (cinematic_function == (uint16)FUNC16(CinematicFunction_Intro_Page2)) {
    int v1 = k >> 1;
    palettefx_instr_list_ptrs[v1] = addr_off_8DE192;
    palettefx_instr_timers[v1] = 1;
  }
}

void PalPreInstr_CheckEnemy0Health(uint16 k) {  // 0x8DE2E0
  if (!enemy_data[0].health)
    palettefx_ids[k >> 1] = 0;
}

void PalPreInstr_SamusInHeat(uint16 k) {  // 0x8DE379
  if ((equipped_items & 0x21) == 0) {
    uint16 v1 = (__PAIR32__(samus_periodic_damage, samus_periodic_subdamage) + 0x4000) >> 16;
    samus_periodic_subdamage += 0x4000;
    samus_periodic_damage = v1;
    if ((nmi_frame_counter_word & 7) == 0 && samus_health > 0x46u)
      QueueSfx3_Max6(0x2Du);
  }
  if (samus_in_heat_palfx_index != samus_in_heat_palettefx_prev_index) {
    samus_in_heat_palettefx_prev_index = samus_in_heat_palfx_index;
    uint16 v2 = 2 * samus_in_heat_palfx_index, v4;
    int v3 = k >> 1;
    palettefx_instr_timers[v3] = 1;
    if ((equipped_items & 0x20) != 0) {
      v4 = addr_off_8DE3E0;
    } else if ((equipped_items & 1) != 0) {
      v4 = addr_off_8DE400;
    } else {
      v4 = addr_off_8DE420;
    }
    R18_ = v4;
    palettefx_instr_list_ptrs[v3] = *(uint16 *)&RomPtr_8D(v4)[v2];
  }
}

void PalInit_F761_Norfair1(uint16 k, uint16 j) {  // 0x8DE440
  uint16 v2;
  if ((equipped_items & 0x20) != 0) {
    v2 = addr_off_8DE8B6;
  } else if ((equipped_items & 1) != 0) {
    v2 = addr_off_8DE68A;
  } else {
    v2 = addr_kPalfxInstrList_E45E;
  }
  palettefx_instr_list_ptrs[j >> 1] = v2;
}

PairU16 PalInstr_SetTimerIfBossBit(uint16 k, uint16 j) {  // 0x8DEB2A
  if (!(CheckBossBitForCurArea(1u) & 1)) {
    palettefx_instr_timers[k >> 1] = 1;
    return MakePairU16(k, 0);
  }
  return MakePairU16(k, j);
}

void PalPreInstr_SwitchIfYpos(uint16 k) {  // 0x8DEC59
  if (samus_y_pos < 0x380u) {
    int v2 = k >> 1;
    palettefx_instr_timers[v2] = 1;
    palettefx_instr_list_ptrs[v2] = addr_word_8DEB43;
  }
}

void PalPreInstr_SwitchIfYpos2(uint16 k) {  // 0x8DED84
  if (samus_y_pos < 0x380u) {
    int v2 = k >> 1;
    palettefx_instr_timers[v2] = 1;
    palettefx_instr_list_ptrs[v2] = addr_word_8DEC76;
  }
}

void PalPreInstr_DeletePalfxIfMinibossDead(uint16 k) {  // 0x8DEEC5
  if ((*(uint16 *)&boss_bits_for_area[area_index] & 2) != 0)
    palettefx_ids[k >> 1] = 0;
}

PairU16 PalInstr_SetPalfxIndex(uint16 k, uint16 j) {  // 0x8DF1C6
  samus_in_heat_palfx_index = *RomPtr_8D(j);
  return MakePairU16(k, j + 1);
}

void PalPreInstr_F621(uint16 k) {  // 0x8DF621
  if (*(uint16 *)((char *)&flag_for_palette_fx_objects + k))
    palettefx_ids[k >> 1] = 0;
}

void PalInit_F779_Brinstar8(uint16 k, uint16 j) {  // 0x8DF730
  if ((*(uint16 *)&boss_bits_for_area[area_index] & 2) != 0)
    palettefx_ids[j >> 1] = 0;
}
