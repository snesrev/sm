// Projectiles
#include "ida_types.h"
#include "variables.h"
#include "funcs.h"


#define kProjectileData_UnchargedBeams ((uint16*)RomFixedPtr(0x9383c1))
#define kProjectileData_ChargedBeams ((uint16*)RomFixedPtr(0x9383d9))
#define kProjectileData_NonBeams ((uint16*)RomFixedPtr(0x9383f1))
#define kShinesparkEchoSpazer_ProjectileData ((uint16*)RomFixedPtr(0x938403))
#define kRunInstrForSuperMissile ((uint16*)RomFixedPtr(0x93842b))
#define g_stru_938691 (*(ProjectileDamagesAndInstrPtr*)RomFixedPtr(0x938691))
#define g_stru_938679 (*(ProjectileDamagesAndInstrPtr*)RomFixedPtr(0x938679))
#define kProjInstrList_Explosion (*(ProjectileDamagesAndInstrPtr*)RomFixedPtr(0x938681))
#define g_off_938413 ((uint16*)RomFixedPtr(0x938413))

void InitializeProjectile(uint16 k) {  // 0x938000
  ProjectileDataTable *ProjectileDataTable;

  int v1 = k >> 1;
  int r18 = 2 * (projectile_dir[v1] & 0xF);
  uint16 v2 = projectile_type[v1], v3;
  if ((v2 & 0xF00) != 0) {
    v3 = kProjectileData_NonBeams[HIBYTE(v2) & 0xF];
  } else if ((v2 & 0x10) != 0) {
    v3 = kProjectileData_ChargedBeams[projectile_type[v1] & 0xF];
  } else {
    v3 = kProjectileData_UnchargedBeams[projectile_type[v1] & 0xF];
  }
  ProjectileDataTable = get_ProjectileDataTable(v3);
  if (sign16(ProjectileDataTable->damage))
    InvalidInterrupt_Crash();
  projectile_damage[v1] = ProjectileDataTable->damage;
  uint16 v7 = GET_WORD(RomPtr_93(v3 + r18 + 2));
  projectile_bomb_instruction_ptr[v1] = v7;
  const uint8 *v8 = RomPtr_93(v7);
  projectile_x_radius[v1] = v8[4];
  projectile_y_radius[v1] = v8[5];
  projectile_bomb_instruction_timers[v1] = 1;
}

void InitializeInstrForSuperMissile(uint16 v0) {  // 0x938071
  int v1 = v0 >> 1;
  const uint8 *v3 = RomPtr_93(kRunInstrForSuperMissile[HIBYTE(projectile_type[v1]) & 0xF]);
  uint16 v4 = GET_WORD(v3);
  projectile_damage[v1] = v4;
  if (sign16(v4))
    InvalidInterrupt_Crash();
  projectile_bomb_instruction_ptr[v1] = GET_WORD(v3 + 2);
  projectile_bomb_instruction_timers[v1] = 1;
}

void InitializeInstrForMissile(uint16 v0) {  // 0x9380A0
  int v1 = v0 >> 1;
  const uint8 *v3 = RomPtr_93(kProjectileData_NonBeams[HIBYTE(projectile_type[v1]) & 0xF]);
  uint16 v4 = GET_WORD(v3);
  projectile_damage[v1] = GET_WORD(v3);
  if (sign16(v4))
    InvalidInterrupt_Crash();
  projectile_bomb_instruction_ptr[v1] = GET_WORD(v3 + 2);
  projectile_bomb_instruction_timers[v1] = 1;
}

void KillProjectileInner(uint16 k) {  // 0x9380CF
  int v1 = k >> 1;
  if ((projectile_type[v1] & 0xF00) != 0) {
    if (!cinematic_function)
      QueueSfx2_Max6(7);
    uint16 v2 = projectile_type[v1];
    projectile_type[v1] = v2 & 0xF0FF | 0x800;
    if ((v2 & 0x200) != 0) {
      projectile_bomb_instruction_ptr[v1] = HIWORD(g_stru_938691.damages);
      earthquake_type = 20;
      earthquake_timer = 30;
    } else {
      projectile_bomb_instruction_ptr[v1] = *(&g_stru_938679.instr_ptr + 1);
    }
    if (!sign16(cooldown_timer - 21))
      cooldown_timer = 20;
  } else {
    projectile_type[v1] = projectile_type[v1] & 0xF0FF | 0x700;
    projectile_bomb_instruction_ptr[v1] = HIWORD(g_stru_938679.damages);
    QueueSfx2_Max6(0xC);
  }
  projectile_bomb_instruction_timers[v1] = 1;
  projectile_damage[v1] = 8;
}

void InitializeBombExplosion(uint16 k) {  // 0x93814E
  int v1 = k >> 1;
  projectile_bomb_instruction_ptr[v1] = HIWORD(kProjInstrList_Explosion.damages);
  projectile_bomb_instruction_timers[v1] = 1;
}

void InitializeShinesparkEchoOrSpazerSba(uint16 k) {  // 0x938163
  int v1 = k >> 1;
  int r18 = 2 * (projectile_dir[v1] & 0xF);
  const uint8 *v3 = RomPtr_93(kShinesparkEchoSpazer_ProjectileData[LOBYTE(projectile_type[v1]) - 34]);
  uint16 v4 = GET_WORD(v3);
  projectile_damage[v1] = GET_WORD(v3);
  if (sign16(v4))
    InvalidInterrupt_Crash();
  projectile_bomb_instruction_ptr[v1] = GET_WORD(v3 + 2 + r18);
  projectile_bomb_instruction_timers[v1] = 1;
}

void InitializeSbaProjectile(uint16 k) {  // 0x9381A4
  int v1 = k >> 1;
  const uint8 *v2 = RomPtr_93(g_off_938413[projectile_type[v1] & 0xF]);
  uint16 v3 = GET_WORD(v2);
  projectile_damage[v1] = GET_WORD(v2);
  if (sign16(v3))
    InvalidInterrupt_Crash();
  projectile_bomb_instruction_ptr[v1] = GET_WORD(v2 + 2);
  projectile_bomb_instruction_timers[v1] = 1;
}

uint16 ProjectileInsts_GetValue(uint16 k) {  // 0x9381D1
  int ip = projectile_bomb_instruction_ptr[k >> 1];
  int delta = (projectile_bomb_instruction_timers[k >> 1] == 1 && !sign16(get_ProjectileInstr(ip)->timer)) ? 0 : -8;
  return get_ProjectileInstr(ip + delta)->field_6;
}

uint16 CallProj93Instr(uint32 ea, uint16 j, uint16 k) {
  switch (ea) {
  case fnProj93Instr_Delete: return Proj93Instr_Delete(j, k);
  case fnProj93Instr_Goto: return Proj93Instr_Goto(j, k);
  case fnProj93Instr_GotoIfLess: return Proj93Instr_GotoIfLess(j, k);
  default: return Unreachable();
  }
}

void RunProjectileInstructions(void) {  // 0x9381E9
  ProjectileInstr *PI;

  uint16 v0 = projectile_index;
  int v1 = projectile_index >> 1;
  if (projectile_bomb_instruction_timers[v1]-- == 1) {
    uint16 v3 = projectile_bomb_instruction_ptr[v1];
    while (1) {
      PI = get_ProjectileInstr(v3);
      if ((PI->timer & 0x8000) == 0)
        break;
      v3 = CallProj93Instr(PI->timer | 0x930000, v0, v3 + 2);
      if (!v3)
        return;
    }
    projectile_bomb_instruction_timers[v1] = PI->timer;
    projectile_spritemap_pointers[v1] = PI->spritemap_ptr;
    projectile_x_radius[v1] = PI->x_radius;
    projectile_y_radius[v1] = PI->y_radius;
    projectile_bomb_instruction_ptr[v1] = v3 + 8;
  }
}

uint16 Proj93Instr_Delete(uint16 k, uint16 j) {  // 0x93822F
  ClearProjectile(k);
  return 0;
}

uint16 Proj93Instr_Goto(uint16 k, uint16 j) {  // 0x938239
  return *(uint16 *)RomPtr_93(j);
}

uint16 Proj93Instr_GotoIfLess(uint16 k, uint16 j) {  // 0x938240
  const uint8 *v2 = RomPtr_93(j);
  if ((int16)(GET_WORD(v2) - projectile_variables[k >> 1]) >= 0)
    return GET_WORD(v2 + 2);
  else
    return GET_WORD(v2 + 4);
}

void DrawPlayerExplosions2(void) {  // 0x938254
  uint16 v0 = 8;
  uint16 r18, r20;
  projectile_index = 8;
  do {
    int v1 = v0 >> 1;
    if (!projectile_bomb_instruction_ptr[v1])
      goto LABEL_25;
    uint16 v2, v3;
    v2 = projectile_type[v1];
    if ((v2 & (kProjectileType_TypeMask | 0x10)) != 0) {
      if (!sign16((v2 & 0xF00) - 768))
        goto LABEL_25;
    } else if ((v2 & 0xC) != 0) {
      if ((v0 & 2) != 0) {
        if ((nmi_frame_counter_word & 2) == 0)
          goto LABEL_25;
      } else if ((nmi_frame_counter_word & 2) != 0) {
        goto LABEL_25;
      }
    } else if ((v0 & 2) != 0) {
      if ((nmi_frame_counter_word & 1) != 0)
        goto LABEL_25;
    } else if ((nmi_frame_counter_word & 1) == 0) {
      goto LABEL_25;
    }
    if ((ceres_status & 0x8000) == 0) {
      r20 = projectile_x_pos[v1] - layer1_x_pos;
      v3 = projectile_y_pos[v1] - layer1_y_pos;
      r18 = v3;
    } else {
      Point16U pt = CalcExplosion_Mode7(v0);
      v3 = pt.y;
      r20 = pt.x;
    }
    if ((v3 & 0xFF00) == 0 && (projectile_spritemap_pointers[v1] & 0x8000) != 0) {
      DrawProjectileSpritemap(v0, r20, r18);
    }
    v0 = projectile_index;
LABEL_25:
    v0 -= 2;
    projectile_index = v0;
  } while ((v0 & 0x8000) == 0);
  Samus_DrawShinesparkCrashEchoProjectiles();
  HandleProjectileTrails();
}

void sub_9382FD(void) {  // 0x9382FD
  uint16 v0 = 8;
  projectile_index = 8;
  do {
    int v1 = v0 >> 1;
    if (projectile_bomb_instruction_ptr[v1]) {
      uint16 r20 = projectile_x_pos[v1] - layer1_x_pos;
      uint16 r18 = projectile_y_pos[v1] - 8 - layer1_y_pos;
      if ((r18 & 0xFF00) != 0) {
      } else if ((projectile_spritemap_pointers[v1] & 0x8000) != 0) {
        DrawProjectileSpritemap(v0, r20, r18);
      }
      v0 = projectile_index;
    }
    v0 -= 2;
    projectile_index = v0;
  } while ((v0 & 0x8000) == 0);
  HandleProjectileTrails();
}

void DrawBombAndProjectileExplosions(void) {  // 0x93834D
  uint16 v0 = 18, v2;
  uint16 r18, r20;
  projectile_index = 18;
  do {
    int v1 = v0 >> 1;
    if (!projectile_bomb_instruction_ptr[v1] || (int16)((projectile_type[v1] & 0xF00) - 768) < 0)
      goto LABEL_16;
    if ((projectile_type[v1] & 0xF00) == 768) {
      if (!projectile_variables[v1])
        goto LABEL_16;
LABEL_9:;
      uint16 v3 = projectile_x_pos[v1] - layer1_x_pos;
      r20 = v3;
      if (!sign16(v3 - 304) || sign16(v3 + 48))
        goto LABEL_16;
      v2 = projectile_y_pos[v1] - layer1_y_pos;
      r18 = v2;
      goto LABEL_12;
    }
    if ((projectile_type[v1] & 0xF00) == 1280 || (ceres_status & 0x8000) == 0)
      goto LABEL_9;
    CalcExplosion_Mode7(v0);
    v2 = r18;
LABEL_12:
    if ((v2 & 0xFF00) != 0)
      ;
    else
      DrawProjectileSpritemap(v0, r20, r18);
    v0 = projectile_index;
LABEL_16:
    v0 -= 2;
    projectile_index = v0;
  } while ((v0 & 0x8000) == 0);
}
