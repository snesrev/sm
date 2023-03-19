// Animated tiles
#include "ida_types.h"
#include "funcs.h"
#include "variables.h"





void EnableAnimtiles(void) {  // 0x878000
  animtiles_enable_flag |= 0x8000;
}

void DisableAnimtiles(void) {  // 0x87800B
  animtiles_enable_flag &= ~0x8000u;
}

void ClearAnimtiles(void) {  // 0x878016
  for (int i = 10; i >= 0; i -= 2)
    animtiles_ids[i >> 1] = 0;
}

void SpawnAnimtiles(uint16 j) {  // 0x878027
  uint16 v0 = j;

  uint16 v1 = 10;
  while (animtiles_ids[v1 >> 1]) {
    v1 -= 2;
    if ((v1 & 0x8000u) != 0)
      return;
  }
  int v2 = v1 >> 1;
  animtiles_ids[v2] = v0;
  animtiles_timers[v2] = 0;
  const uint16 *v3 = (const uint16 *)RomPtr_87(v0);
  animtiles_instr_list_ptrs[v2] = *v3;
  animtiles_src_ptr[v2] = 0;
  animtiles_sizes[v2] = v3[1];
  animtiles_vram_ptr[v2] = v3[2];
  animtiles_instr_timers[v2] = 1;
}

void AnimtilesHandler(void) {  // 0x878064
  if ((animtiles_enable_flag & 0x8000u) != 0) {
    for (int i = 10; i >= 0; i -= 2) {
      animtiles_object_index = i;
      if (animtiles_ids[i >> 1]) {
        ProcessAnimtilesObject();
        i = animtiles_object_index;
      }
    }
  }
}

uint16 CallAnimtilesInstr(uint32 ea, uint16 j, uint16 k) {
  switch (ea) {
  case fnAnimtilesInstr_Delete: return AnimtilesInstr_Delete(j, k);
  case fnAnimtilesInstr_Goto: return AnimtilesInstr_Goto(j, k);
  case fnAnimtilesInstr_GotoRel: return AnimtilesInstr_GotoRel(j, k);
  case fnAnimtilesInstr_DecrementTimerAndGoto: return AnimtilesInstr_DecrementTimerAndGoto(j, k);
  case fnAnimtilesInstr_DecrementTimerAndGotoRel: return AnimtilesInstr_DecrementTimerAndGotoRel(j, k);
  case fnAnimtilesInstr_SetTimer: return AnimtilesInstr_SetTimer(j, k);
  case fnAnimtilesInstr_QueueMusic: return AnimtilesInstr_QueueMusic(j, k);
  case fnAnimtilesInstr_QueueSfx1: return AnimtilesInstr_QueueSfx1(j, k);
  case fnAnimtilesInstr_QueueSfx2: return AnimtilesInstr_QueueSfx2(j, k);
  case fnAnimtilesInstr_QueueSfx3: return AnimtilesInstr_QueueSfx3(j, k);
  case fnAnimtilesInstr_GotoIfBossBitSet: return AnimtilesInstr_GotoIfBossBitSet(j, k);
  case fnAnimtilesInstr_SetBossBit: return AnimtilesInstr_SetBossBit(j, k);
  case fnAnimtilesInstr_GotoIfEventHappened: return AnimtilesInstr_GotoIfEventHappened(j, k);
  case fnAnimtilesInstr_SetEventHappened: return AnimtilesInstr_SetEventHappened(j, k);
  case fnAnimtilesInstr_WaitUntilAreaBossDead_DoubleRet: return AnimtilesInstr_WaitUntilAreaBossDead_DoubleRet(j, k);
  case fnAnimtilesInstr_GotoIfBossBitSetInArea: return AnimtilesInstr_GotoIfBossBitSetInArea(j, k);
  case fnAnimtilesInstr_SpawnTourianStatueEyeGlow: return AnimtilesInstr_SpawnTourianStatueEyeGlow(j, k);
  case fnAnimtilesInstr_SpawnTourianStatueSoul: return AnimtilesInstr_SpawnTourianStatueSoul(j, k);
  case fnAnimtilesInstr_GotoIfTourianStatueBusy: return AnimtilesInstr_GotoIfTourianStatueBusy(j, k);
  case fnAnimtilesInstr_TourianStatueSetState: return AnimtilesInstr_TourianStatueSetState(j, k);
  case fnAnimtilesInstr_TourianStatueClearState: return AnimtilesInstr_TourianStatueClearState(j, k);
  case fnAnimtilesInstr_Clear3PaletteColors: return AnimtilesInstr_Clear3PaletteColors(j, k);
  case fnAnimtilesInstr_SpawnPalfxObj: return AnimtilesInstr_SpawnPalfxObj(j, k);
  case fnAnimtilesInstr_Write8PaletteColors: return AnimtilesInstr_Write8PaletteColors(j, k);
  default: return Unreachable();
  }
}

void ProcessAnimtilesObject(void) {  // 0x878085
  uint16 v0 = animtiles_object_index;
  int v1 = animtiles_object_index >> 1;
  if (animtiles_instr_timers[v1]-- == 1) {
    uint16 v3 = animtiles_instr_list_ptrs[v1], v5;
    while (1) {
      const uint16 *v4 = (const uint16 *)RomPtr_87(v3);
      v5 = *v4;
      if ((*v4 & 0x8000u) == 0)
        break;
      animtiles_instruction = *v4;
      v3 = CallAnimtilesInstr(v5 | 0x870000, v0, v3 + 2);
      if (!v3)
        return;
    }
    int v6 = v0 >> 1;
    animtiles_instr_timers[v6] = v5;
    animtiles_src_ptr[v6] = *((uint16 *)RomPtr_87(v3) + 1);
    animtiles_instr_list_ptrs[v6] = v3 + 4;
  }
}

uint16 AnimtilesInstr_Delete(uint16 k, uint16 j) {  // 0x8780B2
  animtiles_ids[k >> 1] = 0;
  return 0;
}

uint16 AnimtilesInstr_Goto(uint16 k, uint16 j) {  // 0x8780B7
  return *(uint16 *)RomPtr_87(j);
}

uint16 AnimtilesInstr_GotoRel(uint16 k, uint16 j) {  // 0x8780BC
  animtiles_instruction = j;
  return j + (int8)*RomPtr_87(j);
}

uint16 AnimtilesInstr_DecrementTimerAndGoto(uint16 k, uint16 j) {  // 0x8780D4
  int v2 = k >> 1;
  if (animtiles_timers[v2]-- == 1)
    return j + 2;
  else
    return AnimtilesInstr_Goto(k, j);
}

uint16 AnimtilesInstr_DecrementTimerAndGotoRel(uint16 k, uint16 j) {  // 0x8780DC
  int v2 = k >> 1;
  if (animtiles_timers[v2]-- == 1)
    return j + 1;
  else
    return AnimtilesInstr_GotoRel(k, j);
}

uint16 AnimtilesInstr_SetTimer(uint16 k, uint16 j) {  // 0x8780E3
  *((uint8 *)animtiles_timers + k) = *RomPtr_87(j);
  return j + 1;
}

uint16 AnimtilesInstr_QueueMusic(uint16 k, uint16 j) {  // 0x8780F0
  const uint8 *v2 = RomPtr_87(j);
  QueueMusic_Delayed8(*v2);
  return j + 1;
}

uint16 AnimtilesInstr_QueueSfx1(uint16 k, uint16 j) {  // 0x8780FC
  const uint8 *v2 = RomPtr_87(j);
  QueueSfx1_Max6(*v2);
  return j + 1;
}

uint16 AnimtilesInstr_QueueSfx2(uint16 k, uint16 j) {  // 0x878108
  const uint8 *v2 = RomPtr_87(j);
  QueueSfx2_Max6(*v2);
  return j + 1;
}

uint16 AnimtilesInstr_QueueSfx3(uint16 k, uint16 j) {  // 0x878114
  const uint8 *v2 = RomPtr_87(j);
  QueueSfx3_Max6(*v2);
  return j + 1;
}

uint16 AnimtilesInstr_GotoIfBossBitSet(uint16 k, uint16 j) {  // 0x878120
  const uint8 *v2 = RomPtr_87(j);
  uint16 v3 = j + 1;
  if (CheckBossBitForCurArea((uint8) * (uint16 *)v2) & 1)
    return AnimtilesInstr_Goto(k, v3);
  else
    return v3 + 2;
}

uint16 AnimtilesInstr_SetBossBit(uint16 k, uint16 j) {  // 0x878133
  const uint8 *v2 = RomPtr_87(j);
  SetBossBitForCurArea(*v2);
  return j + 1;
}

uint16 AnimtilesInstr_GotoIfEventHappened(uint16 k, uint16 j) {  // 0x87813F
  const uint16 *v2 = (const uint16 *)RomPtr_87(j);
  uint16 v3 = j + 2;
  if (CheckEventHappened(*v2) & 1)
    return AnimtilesInstr_Goto(k, v3);
  else
    return v3 + 2;
}

uint16 AnimtilesInstr_SetEventHappened(uint16 k, uint16 j) {  // 0x878150
  const uint16 *v2 = (const uint16 *)RomPtr_87(j);
  SetEventHappened(*v2);
  return j + 2;
}

void UNUSED_sub_87815A(void) {  // 0x87815A
  CallSomeSamusCode(0);
}

void UNUSED_sub_878162(void) {  // 0x878162
  CallSomeSamusCode(1u);
}

uint16 AnimtilesInstr_WaitUntilAreaBossDead_DoubleRet(uint16 k, uint16 j) {  // 0x8781BA
  if (!(CheckBossBitForCurArea(1u) & 1)) {
    animtiles_instr_timers[k >> 1] = 1;
    return 0;
  }
  return j;
}

uint16 AnimtilesInstr_GotoIfBossBitSetInArea(uint16 k, uint16 j) {  // 0x878303
  const uint8 *v2 = RomPtr_87(j);
  uint16 v3 = j + 2;
  if ((*v2 & boss_bits_for_area[v2[1]]) != 0)
    return AnimtilesInstr_Goto(k, v3);
  else
    return v3 + 2;
}

uint16 AnimtilesInstr_SpawnTourianStatueEyeGlow(uint16 k, uint16 j) {  // 0x878320
  const uint16 *v2 = (const uint16 *)RomPtr_87(j);
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_TourianStatueEyeGlow, *v2);
  return j + 2;
}

uint16 AnimtilesInstr_SpawnTourianStatueSoul(uint16 k, uint16 j) {  // 0x87832F
  const uint16 *v2 = (const uint16 *)RomPtr_87(j);
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_TourianStatueSoul, *v2);
  return j + 2;
}

uint16 AnimtilesInstr_GotoIfTourianStatueBusy(uint16 k, uint16 j) {  // 0x87833E
  if ((tourian_entrance_statue_animstate & 0x8000u) == 0)
    return j + 2;
  else
    return AnimtilesInstr_Goto(k, j);
}

uint16 AnimtilesInstr_TourianStatueSetState(uint16 k, uint16 j) {  // 0x878349
  tourian_entrance_statue_animstate |= *(uint16 *)RomPtr_87(j);
  return j + 2;
}

uint16 AnimtilesInstr_TourianStatueClearState(uint16 k, uint16 j) {  // 0x878352
  tourian_entrance_statue_animstate &= ~*(uint16 *)RomPtr_87(j);
  return j + 2;
}

uint16 AnimtilesInstr_Clear3PaletteColors(uint16 k, uint16 j) {  // 0x87835B
  int v2 = *(uint16 *)RomPtr_87(j) >> 1;
  palette_buffer[v2] = 0;
  palette_buffer[v2 + 1] = 0;
  palette_buffer[v2 + 2] = 0;
  return j + 2;
}

uint16 AnimtilesInstr_SpawnPalfxObj(uint16 k, uint16 j) {  // 0x878372
  const uint16 *v2 = (const uint16 *)RomPtr_87(j);
  SpawnPalfxObject(*v2);
  return j + 2;
}

uint16 AnimtilesInstr_Write8PaletteColors(uint16 k, uint16 j) {  // 0x87837F
  static const uint16 kAnimtilesInstr_Write8PaletteColors[8] = { 0x3800, 0x7f58, 0x6ed5, 0x5a71, 0x49ee, 0x356a, 0x24e7, 0x1083 };

  uint16 v2 = *(uint16 *)RomPtr_87(j);
  for (int i = 0; i != 16; i += 2) {
    target_palettes[v2 >> 1] = kAnimtilesInstr_Write8PaletteColors[i >> 1];
    v2 += 2;
  }
  return j + 2;
}