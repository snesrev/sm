#include "ida_types.h"
#include "variables.h"
#include "funcs.h"
#include "enemy_types.h"


#define kMotherBrain_HealthBasedPalettes_Brain ((uint16*)RomFixedPtr(0xade6a2))
#define kMotherBrain_HealthBasedPalettes_BackLeg ((uint16*)RomFixedPtr(0xade742))
#define kMotherBrain_FadePalToBlack ((uint16*)RomFixedPtr(0xade9e8))
#define kMotherBrain_TransitionToFromGrey_Incr ((uint16*)RomFixedPtr(0xaded8a))
#define kMotherBrain_TransitionToFromGrey_Decr ((uint16*)RomFixedPtr(0xaded9c))
#define kMotherBrain_FadeToGray_Drained ((uint16*)RomFixedPtr(0xadef87))
#define kMotherBrain_FadeToGray_RealDeath ((uint16*)RomFixedPtr(0xadf107))
#define kMotherBrain_Phase3_TurnLightsBackOn ((uint16*)RomFixedPtr(0xadf273))

void nullsub_341(void) {}

static Func_V *const funcs_BE56D[16] = {
  MotherBrain_CalcHdma_Down, MotherBrain_CalcHdma_BeamAimedRight2,                                0,                         0,
                          0,     MotherBrain_CalcHdma_BeamAimedUp, MotherBrain_CalcHdma_BeamAimedUp,                         0,
                          0,                                    0, MotherBrain_CalcHdma_BeamAimedUp,               nullsub_341,
  MotherBrain_CalcHdma_Down,                                    0,                                0, MotherBrain_CalcHdma_Down,
};

void MotherBrain_CalcHdma(void) {  // 0xADDE00
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  Enemy_MotherBrain *E1 = Get_MotherBrain(0x40);
  uint16 r18 = HIBYTE(E->mbn_var_33) >> 1;
  E->mbn_var_3A = E->mbn_var_31 - r18;
  E->mbn_var_3B = r18 + E->mbn_var_31;
  uint16 v2 = (*(uint16 *)((uint8 *)&E1->base.enemy_ptr + 1) + 3584) & 0xFF00;
  E->mbn_var_3C = v2;
  E->mbn_var_3E = v2;
  uint16 v3 = E1->base.y_pos + 5;
  E->mbn_var_3D = v3;
  E->mbn_var_3F = v3;
  r18 = 4 * (E->mbn_var_3A & 0xC0);
  int i = (r18 | E->mbn_var_3B & 0xC0) >> 6;
  funcs_BE56D[i]();
}

void MotherBrain_CalcHdma_BeamAimedRight2(void) {  // 0xADDE7F
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  MotherBrain_CalcHdma_BeamAimedRight(E->mbn_var_3C, E->mbn_var_3E);
  *(uint16 *)mother_brain_indirect_hdma = 16;
  *(uint16 *)&mother_brain_indirect_hdma[1] = -25600;
  *(uint16 *)&mother_brain_indirect_hdma[3] = 16;
  *(uint16 *)&mother_brain_indirect_hdma[4] = -25597;
  *(uint16 *)&mother_brain_indirect_hdma[6] = 240;
  *(uint16 *)&mother_brain_indirect_hdma[7] = -25340;
  *(uint16 *)&mother_brain_indirect_hdma[9] = 244;
  *(uint16 *)&mother_brain_indirect_hdma[10] = -25108;
  *(uint16 *)&mother_brain_indirect_hdma[12] = 0;
}

void MotherBrain_CalcHdma_BeamAimedRight(uint16 r22, uint16 r24) {  // 0xADDECE
  hdma_table_2[0] = 255;
  hdma_table_2[1] = 255;
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  uint16 r18 = kTanTable[LOBYTE(E->mbn_var_3A)];
  uint16 r20 = kTanTable[LOBYTE(E->mbn_var_3B)];
  uint16 mbn_var_3F = E->mbn_var_3F;
  uint16 *v2 = hdma_table_2 + (mbn_var_3F - 32) + 1;
  v2[1] = 255;
  v2[2] = 255;
  uint16 *v8 = v2;
  do {
    uint16 v4 = r20 + r24;
    if (__CFADD__uint16(r20, r24))
      break;
    r24 += r20;
    *v2-- = HIBYTE(v4) | 0xFF00;
    --mbn_var_3F;
  } while (mbn_var_3F != 32);
  do {
    *v2-- = 255;
    --mbn_var_3F;
  } while (mbn_var_3F != 32);
  uint16 *v5 = v8 + 1;
  uint16 var_3F = gRam8000_Default(0)->var_3F;
  while (1) {
    uint16 v7 = r18 + r22;
    if (__CFADD__uint16(r18, r22))
      break;
    r22 += r18;
    *v5++ = HIBYTE(v7) | 0xFF00;
    if (++var_3F == 232)
      return;
  }
  do {
    *v5++ = 255;
    ++var_3F;
  } while (var_3F != 232);
}

typedef void Func_CalcHdma(uint16 r22, uint16 r24);
static Func_CalcHdma *const off_ADE024[4] = {  // 0xADDF6E
  MotherBrain_CalcHdma_Up_UpRight,
  MotherBrain_CalcHdma_Up_Up,
  0,
  MotherBrain_CalcHdma_Up_UpLeft,
};

void MotherBrain_CalcHdma_BeamAimedUp(void) {
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  off_ADE024[(E->mbn_var_3A >> 6) & 2 | (E->mbn_var_3B >> 7) & 1](E->mbn_var_3C, E->mbn_var_3E);
  *(uint16 *)mother_brain_indirect_hdma = 16;
  *(uint16 *)&mother_brain_indirect_hdma[1] = -25344;
  *(uint16 *)&mother_brain_indirect_hdma[3] = 16;
  *(uint16 *)&mother_brain_indirect_hdma[4] = -25342;

  uint16 v2 = E->mbn_var_3D - 32;
  printf("Carry crap!\n");
  if (v2 >= 0x80) {
    *(uint16 *)&mother_brain_indirect_hdma[6] = (v2 - 127) | 0x80;
    *(uint16 *)&mother_brain_indirect_hdma[7] = -25340;
    *(uint16 *)&mother_brain_indirect_hdma[9] = 255;
    *(uint16 *)&mother_brain_indirect_hdma[10] = 2 * (v2 - 127) - 25340;
    *(uint16 *)&mother_brain_indirect_hdma[12] = 127;
    *(uint16 *)&mother_brain_indirect_hdma[13] = 2 * (v2 - 127) - 25340 + 254;
    *(uint16 *)&mother_brain_indirect_hdma[15] = 0;
  } else {
    *(uint16 *)&mother_brain_indirect_hdma[6] = v2 | 0x80;
    *(uint16 *)&mother_brain_indirect_hdma[7] = -25340;
    *(uint16 *)&mother_brain_indirect_hdma[9] = 127;
    *(uint16 *)&mother_brain_indirect_hdma[10] = 2 * v2 - 25340;
    *(uint16 *)&mother_brain_indirect_hdma[12] = 0;
  }
}

void MotherBrain_CalcHdma_Up_UpRight(uint16 r22, uint16 r24) {  // 0xADE02C
  hdma_table_2[0] = 255;
  hdma_table_2[1] = 255;
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  uint16 r18 = kTanTable[LOBYTE(E->mbn_var_3B)];
  uint16 r20 = kTanTable[LOBYTE(E->mbn_var_3A)];
  uint16 mbn_var_3D = E->mbn_var_3D;
  uint16 *v2 = hdma_table_2 + (mbn_var_3D - 32) + 1;
  v2[1] = 255;
  v2[2] = 255;
  do {
    uint16 v4 = r18 + r22;
    if (__CFADD__uint16(r18, r22))
      v4 = -1;
    r22 = v4;
    uint16 R36 = (v4 >> 8) & 0xff;
    uint16 v7 = r20 + r24;
    if (__CFADD__uint16(r20, r24))
      v7 = -1;
    r24 = v7;
    uint16 v8 = R36 | v7 & 0xFF00;
    if (v8 == 0xffff)
      v8 = 255;
    *v2-- = v8;
    --mbn_var_3D;
  } while (mbn_var_3D != 32);
}

void MotherBrain_CalcHdma_Up_Up(uint16 r22, uint16 r24) {  // 0xADE0A6
  hdma_table_2[0] = 255;
  hdma_table_2[1] = 255;
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  uint16 r18 = kTanTable[(uint8)-LOBYTE(E->mbn_var_3B)];
  uint16 r20 = kTanTable[LOBYTE(E->mbn_var_3A)];
  uint16 mbn_var_3D = E->mbn_var_3D;
  uint16 *v2 = hdma_table_2 + (mbn_var_3D - 32) + 1;
  v2[1] = 255;
  v2[2] = 255;
  do {
    uint16 v4 = r22 - r18;
    if (r22 < r18)
      v4 = 0;
    r22 = v4;
    uint16 r26 = (uint8)(v4 >> 8);
    uint16 v7 = r20 + r24;
    if (__CFADD__uint16(r20, r24))
      v7 = -1;
    r24 = v7;
    uint16 v8 = r26 | v7 & 0xFF00;
    if (v8 == 0xffff)
      v8 = 255;
    *v2-- = v8;
    --mbn_var_3D;
  } while (mbn_var_3D != 32);
}

void MotherBrain_CalcHdma_Up_UpLeft(uint16 r22, uint16 r24) {  // 0xADE124
  hdma_table_2[0] = 255;
  hdma_table_2[1] = 255;
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  uint16 r18 = kTanTable[(uint8)-LOBYTE(E->mbn_var_3B)];
  uint16 r20 = kTanTable[(uint8)-LOBYTE(E->mbn_var_3A)];
  uint16 mbn_var_3D = E->mbn_var_3D;
  uint16 *v2 = hdma_table_2 + (mbn_var_3D - 32) + 1;
  v2[1] = 255;
  v2[2] = 255;
  do {
    uint16 v4 = r22 - r18;
    if (r22 < r18)
      v4 = 0;
    r22 = v4;
    uint16 R36 = v4 >> 8;
    uint16 v7 = r24 - r20;
    if (r24 < r20)
      v7 = 0;
    r24 = v7;
    uint16 v8 = R36 | v7 & 0xFF00;
    if (v8 == 0xffff)
      v8 = 255;
    *v2-- = v8;
    --mbn_var_3D;
  } while (mbn_var_3D != 32);
}

static Func_CalcHdma *const g_off_ADE20E[4] = { MotherBrain_CalcHdma_Down_DownRight, 0, MotherBrain_CalcHdma_Down_Down, MotherBrain_CalcHdma_Down_DownLeft };
void MotherBrain_CalcHdma_Down(void) {  // 0xADE1A6
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  g_off_ADE20E[(E->mbn_var_3A >> 6) & 2 | (E->mbn_var_3B >> 7) & 1](E->mbn_var_3C, E->mbn_var_3E);
  *(uint16 *)mother_brain_indirect_hdma = 16;
  *(uint16 *)&mother_brain_indirect_hdma[1] = -25600;
  *(uint16 *)&mother_brain_indirect_hdma[3] = 16;
  *(uint16 *)&mother_brain_indirect_hdma[4] = -25597;
  *(uint16 *)&mother_brain_indirect_hdma[6] = 240;
  *(uint16 *)&mother_brain_indirect_hdma[7] = -25340;
  *(uint16 *)&mother_brain_indirect_hdma[9] = 244;
  *(uint16 *)&mother_brain_indirect_hdma[10] = -25114;
  *(uint16 *)&mother_brain_indirect_hdma[12] = 0;
}

void MotherBrain_CalcHdma_Down_DownRight(uint16 r22, uint16 r24) {  // 0xADE216
  hdma_table_2[0] = 255;
  hdma_table_2[1] = 255;
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  uint16 r18 = kTanTable[LOBYTE(E->mbn_var_3A)];
  uint16 r20 = kTanTable[LOBYTE(E->mbn_var_3B)];
  uint16 v1 = E->mbn_var_3D - 32;
  uint16 *dst = hdma_table_2 + 2;
  do {
    *dst++ = 255;
  } while (--v1);
  uint16 mbn_var_3D = Get_MotherBrain(0)->mbn_var_3D;
  do {
    uint16 v4 = r18 + r22;
    if (__CFADD__uint16(r18, r22))
      v4 = -1;
    r22 = v4;
    uint16 r26 = v4 >> 8;
    uint16 v7 = r20 + r24;
    if (__CFADD__uint16(r20, r24))
      v7 = -1;
    r24 = v7;
    uint16 v8 = r26 | v7 & 0xFF00;
    if (v8 == 0xffff)
      v8 = 255;
    *dst++ = v8;
    ++mbn_var_3D;
  } while (mbn_var_3D != 232);
}

void MotherBrain_CalcHdma_Down_Down(uint16 r22, uint16 r24) {  // 0xADE293
  hdma_table_2[0] = 255;
  hdma_table_2[1] = 255;
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  uint16 r18 = kTanTable[(uint8)-LOBYTE(E->mbn_var_3A)];
  uint16 r20 = kTanTable[LOBYTE(E->mbn_var_3B)];
  uint16 v1 = E->mbn_var_3D - 32;
  uint16 *dst = hdma_table_2 + 2;
  do {
    *dst++ = 255;
  } while (--v1);
  uint16 mbn_var_3D = Get_MotherBrain(0)->mbn_var_3D;
  do {
    uint16 v4 = r22 - r18;
    if (r22 < r18)
      v4 = 0;
    r22 = v4;
    uint16 r26 = v4 >> 8;
    uint16 v7 = r20 + r24;
    if (__CFADD__uint16(r20, r24))
      v7 = -1;
    r24 = v7;
    uint16 v8 = r26 | v7 & 0xFF00;
    if (v8 == 0xffff)
      v8 = 255;
    *dst++ = v8;
    ++mbn_var_3D;
  } while (mbn_var_3D != 232);
}

void MotherBrain_CalcHdma_Down_DownLeft(uint16 r22, uint16 r24) {  // 0xADE314
  hdma_table_2[0] = 255;
  hdma_table_2[1] = 255;
  Enemy_MotherBrain *E = Get_MotherBrain(0);
  uint16 r18 = kTanTable[(uint8)-LOBYTE(E->mbn_var_3A)];
  uint16 r20 = kTanTable[(uint8)-LOBYTE(E->mbn_var_3B)];
  uint16 v1 = E->mbn_var_3D - 32;
  uint16 *dst = hdma_table_2 + 2;
  do {
    *dst++ = 255;
  } while (--v1);
  uint16 mbn_var_3D = Get_MotherBrain(0)->mbn_var_3D;
  do {
    uint16 v4 = r22 - r18;
    if (r22 < r18)
      v4 = 0;
    r22 = v4;
    uint16 v5 = v4 >> 8;
    uint16 r26 = v5;
    uint16 v7 = r24 - r20;
    if (r24 < r20)
      v7 = 0;
    r24 = v7;
    uint16 v8 = r26 | v7 & 0xFF00;
    if (!v8)
      v8 = 255;
    *dst++ = v8;
    ++mbn_var_3D;
  } while (mbn_var_3D != 232);
}

void MotherBrain_SealWall(void) {  // 0xADE396
  eproj_spawn_pt = (Point16U){ 248, 72 };
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 9);
  eproj_spawn_pt = (Point16U){ 248, 152 };
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 9);
  SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x0f, 0x04, 0xb673 });
  SpawnHardcodedPlm((SpawnHardcodedPlmArgs) { 0x0f, 0x09, 0xb673 });
  Get_MotherBrain(0)->mbn_var_A = FUNC16(MotherBrainBody_FakeDeath_Descent_0_Pause);
}

void MotherBrain_HealthBasedPaletteHandling(void) {  // 0xADE3D5
  if (Get_MotherBrain(0)->mbn_var_1F >= 2) {
    uint16 v0 = 0;
    uint16 health = Get_MotherBrain(0x40)->base.health;
    if (health < 0x2328) {
      v0 = 2;
      if (health < 0x1518) {
        v0 = 4;
        if (health < 0x708)
          v0 = 6;
      }
    }
    WriteColorsToPalette(0x82, 0xad, kMotherBrain_HealthBasedPalettes_Brain[v0 >> 1], 0xF);
    WriteColorsToPalette(0x122, 0xad, kMotherBrain_HealthBasedPalettes_Brain[v0 >> 1], 0xF);
    WriteColorsToPalette(0x162, 0xad, kMotherBrain_HealthBasedPalettes_BackLeg[v0 >> 1], 0xF);
  }
}

uint8 MotherBrain_FadePalToBlack(uint16 a) {  // 0xADE9B4
  int v1 = a;
  if (!kMotherBrain_FadePalToBlack[v1])
    return 1;
  uint16 j = kMotherBrain_FadePalToBlack[v1];
  WriteColorsToPalette(0x82, 0xad, j, 14);
  WriteColorsToPalette(0x122, 0xad, j, 14);
  WriteColorsToPalette(0x162, 0xad, j + 28, 14);
  return 0;
}

uint8 MotherBrain_FadeToGray_FakeDeath(uint16 a) {  // 0xADEEDE
  uint16 v1 = kMotherBrain_TransitionToFromGrey_Incr[a];
  if (v1)
    return MotherBrain_EEF6(v1) & 1;
  else
    return 1;
}

uint8 MotherBrain_FadeFromGray_FakeDeath(uint16 a) {  // 0xADEEEA
  uint16 v1 = kMotherBrain_TransitionToFromGrey_Decr[a];
  if (v1)
    return MotherBrain_EEF6(v1) & 1;
  else
    return 1;
}

uint8 MotherBrain_EEF6(uint16 a) {  // 0xADEEF6
  WriteColorsToPalette(0x122, 0xad, a, 3);
  return 0;
}

uint8 MotherBrain_FadeFromGray_Drained(uint16 a) {  // 0xADEF0D
  int v1 = a;
  if (!kMotherBrain_TransitionToFromGrey_Decr[v1])
    return 1;
  uint16 j = kMotherBrain_TransitionToFromGrey_Decr[v1];
  WriteColorsToPalette(0x82, 0xad, j, 13);
  WriteColorsToPalette(0x122, 0xad, j, 13);
  WriteColorsToPalette(0x168, 0xad, j + 26, 5);
  *(uint16 *)&g_ram[0x17C] = *(uint16 *)RomPtr_AD(j + 36);
  return 0;
}

uint8 MotherBrain_FadeToGray_Drained(uint16 a) {  // 0xADEF4A
  int v1 = a;
  if (!kMotherBrain_FadeToGray_Drained[v1])
    return 1;
  uint16 j = kMotherBrain_FadeToGray_Drained[v1];
  WriteColorsToPalette(0x82, 0xad, j, 15);
  WriteColorsToPalette(0x122, 0xad, j, 15);
  WriteColorsToPalette(0x168, 0xad, j + 30, 5);
  *(uint16 *)&g_ram[0x17C] = *(uint16 *)RomPtr_AD(j + 40);
  return 0;
}

uint8 MotherBrain_FadeToGray_RealDeath(uint16 a) {  // 0xADF0E9
  int v1 = a;
  if (!kMotherBrain_FadeToGray_RealDeath[v1])
    return 1;
  WriteColorsToPalette(0x1E2, 0xad, kMotherBrain_FadeToGray_RealDeath[v1], 0xF);
  return 0;
}

void TurnOffLightsForShitroidDeath(void) {  // 0xADF209
  WriteColorsToPalette(0x62, 0xad, addr_kTurnOffLightsForShitroidDeath, 14);
  WriteColorsToPalette(0xA2, 0xad, addr_kTurnOffLightsForShitroidDeath, 14);
}

uint8 MotherBrain_Phase3_TurnLightsBackOn(uint16 a) {  // 0xADF24B
  int v1 = a;
  if (!kMotherBrain_Phase3_TurnLightsBackOn[v1])
    return 1;
  uint16 v3 = kMotherBrain_Phase3_TurnLightsBackOn[v1];
  WriteColorsToPalette(0x62, 0xad, v3, 14);
  WriteColorsToPalette(0xA2, 0xad, v3 + 28, 14);
  return 0;
}

void EnableEarthquakeAframes(uint16 a) {  // 0xADF40B
  earthquake_type = a;
  earthquake_timer = 20;
  QueueSfx2_Max6(0x25);
}

void HandleMotherBrainBodyFlickering(void) {  // 0xADF41C
  EnemyData *v0 = gEnemyData(0);
  if (v0->frame_counter & 1) {
    *(uint16 *)&reg_TM |= 2;
    v0->properties &= ~kEnemyProps_Invisible;
  } else {
    *(uint16 *)&reg_TM &= ~2;
    v0->properties |= kEnemyProps_Invisible;
  }
}
