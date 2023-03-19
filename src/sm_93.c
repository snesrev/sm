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
  int16 damage;

  int v1 = k >> 1;
  R18_ = 2 * (projectile_dir[v1] & 0xF);
  uint16 v2 = projectile_type[v1], v3;
  if ((v2 & 0xF00) != 0) {
    v3 = kProjectileData_NonBeams[HIBYTE(v2) & 0xF];
  } else if ((v2 & 0x10) != 0) {
    v3 = kProjectileData_ChargedBeams[projectile_type[v1] & 0xF];
  } else {
    v3 = kProjectileData_UnchargedBeams[projectile_type[v1] & 0xF];
  }
  ProjectileDataTable = get_ProjectileDataTable(v3);
  damage = ProjectileDataTable->damage;
  projectile_damage[v1] = ProjectileDataTable->damage;
  if (damage < 0)
    InvalidInterrupt_Crash();
  const uint8 *v6 = RomPtr_93(R18_ + v3 + 2);
  uint16 v7 = *(uint16 *)v6;
  projectile_bomb_instruction_ptr[v1] = *(uint16 *)v6;
  const uint8 *v8 = RomPtr_93(v7);
  projectile_x_radius[v1] = v8[4];
  projectile_y_radius[v1] = v8[5];
  projectile_bomb_instruction_timers[v1] = 1;
}

void InitializeInstrForSuperMissile(uint16 v0) {  // 0x938071
  int16 v4;

  int v1 = v0 >> 1;
  uint16 v2 = kRunInstrForSuperMissile[HIBYTE(projectile_type[v1]) & 0xF];
  const uint8 *v3 = RomPtr_93(v2);
  v4 = *(uint16 *)v3;
  projectile_damage[v1] = *(uint16 *)v3;
  if (v4 < 0)
    InvalidInterrupt_Crash();
  projectile_bomb_instruction_ptr[v1] = *(uint16 *)RomPtr_93(v2 + 2);
  projectile_bomb_instruction_timers[v1] = 1;
}

void InitializeInstrForMissile(uint16 v0) {  // 0x9380A0
  int16 v4;

  int v1 = v0 >> 1;
  uint16 v2 = kProjectileData_NonBeams[HIBYTE(projectile_type[v1]) & 0xF];
  const uint8 *v3 = RomPtr_93(v2);
  v4 = *(uint16 *)v3;
  projectile_damage[v1] = *(uint16 *)v3;
  if (v4 < 0)
    InvalidInterrupt_Crash();
  projectile_bomb_instruction_ptr[v1] = *(uint16 *)RomPtr_93(v2 + 2);
  projectile_bomb_instruction_timers[v1] = 1;
}

void KillProjectileInner(uint16 k) {  // 0x9380CF
  int v1 = k >> 1;
  if ((projectile_type[v1] & 0xF00) != 0) {
    if (!cinematic_function)
      QueueSfx2_Max6(7u);
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
    QueueSfx2_Max6(0xCu);
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
  int16 v4;

  int v1 = k >> 1;
  R18_ = 2 * (projectile_dir[v1] & 0xF);
  uint16 v2 = kShinesparkEchoSpazer_ProjectileData[LOBYTE(projectile_type[v1]) - 34];
  const uint8 *v3 = RomPtr_93(v2);
  v4 = *(uint16 *)v3;
  projectile_damage[v1] = *(uint16 *)v3;
  if (v4 < 0)
    InvalidInterrupt_Crash();
  projectile_bomb_instruction_ptr[v1] = *(uint16 *)RomPtr_93(R18_ + v2 + 2);
  projectile_bomb_instruction_timers[v1] = 1;
}

void InitializeSbaProjectile(uint16 k) {  // 0x9381A4
  int16 v3;

  int v1 = k >> 1;
  const uint8 *v2 = RomPtr_93(g_off_938413[projectile_type[v1] & 0xF]);
  v3 = *(uint16 *)v2;
  projectile_damage[v1] = *(uint16 *)v2;
  if (v3 < 0)
    InvalidInterrupt_Crash();
  projectile_bomb_instruction_ptr[v1] = *((uint16 *)v2 + 1);
  projectile_bomb_instruction_timers[v1] = 1;
}

void ProjectileInsts_GetValue(uint16 k) {  // 0x9381D1
  int ip = projectile_bomb_instruction_ptr[k >> 1];
  int delta = (projectile_bomb_instruction_timers[k >> 1] == 1 && !sign16(get_ProjectileInstr(ip)->timer)) ? 0 : -8;
  R22_ = get_ProjectileInstr(ip + delta)->field_6;
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
  ProjectileInstr *v7;
  ProjectileInstr *ProjectileInstr;

  uint16 v0 = projectile_index;
  int v1 = projectile_index >> 1;
  if (projectile_bomb_instruction_timers[v1]-- == 1) {
    uint16 v3 = projectile_bomb_instruction_ptr[v1], timer;
    while (1) {
      ProjectileInstr = get_ProjectileInstr(v3);
      timer = ProjectileInstr->timer;
      if ((ProjectileInstr->timer & 0x8000u) == 0)
        break;
      R18_ = ProjectileInstr->timer;
      v3 = CallProj93Instr(timer | 0x930000, v0, v3 + 2);
      if (!v3)
        return;
    }
    int v6 = v0 >> 1;
    projectile_bomb_instruction_timers[v6] = timer;
    v7 = get_ProjectileInstr(v3);
    projectile_spritemap_pointers[v6] = v7->spritemap_ptr;
    projectile_x_radius[v6] = v7->x_radius;
    projectile_y_radius[v6] = v7->y_radius;
    projectile_bomb_instruction_ptr[v6] = v3 + 8;
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
  if ((int16)(*(uint16 *)v2 - projectile_variables[k >> 1]) >= 0)
    return *((uint16 *)RomPtr_93(j) + 1);
  else
    return *((uint16 *)v2 + 2);
}

void DrawPlayerExplosions2(void) {  // 0x938254
  uint16 v0 = 8;
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
    if ((ceres_status & 0x8000u) == 0) {
      R20_ = projectile_x_pos[v1] - layer1_x_pos;
      v3 = projectile_y_pos[v1] - layer1_y_pos;
      R18_ = v3;
    } else {
      CalcExplosion_Mode7(v0);
      v3 = R18_;
    }
    if ((v3 & 0xFF00) != 0) {
      if ((projectile_spritemap_pointers[v1] & 0x8000u) != 0){
      }
    } else if ((projectile_spritemap_pointers[v1] & 0x8000u) != 0) {
      DrawProjectileSpritemap(v0);
    }
    v0 = projectile_index;
LABEL_25:
    v0 -= 2;
    projectile_index = v0;
  } while ((v0 & 0x8000u) == 0);
  Samus_DrawShinesparkCrashEchoProjectiles();
  HandleProjectileTrails();
}

void sub_9382FD(void) {  // 0x9382FD
  uint16 v0 = 8;
  projectile_index = 8;
  do {
    int v1 = v0 >> 1;
    if (projectile_bomb_instruction_ptr[v1]) {
      R20_ = projectile_x_pos[v1] - layer1_x_pos;
      R18_ = projectile_y_pos[v1] - 8 - layer1_y_pos;
      if ((R18_ & 0xFF00) != 0) {
      } else if ((projectile_spritemap_pointers[v1] & 0x8000u) != 0) {
        DrawProjectileSpritemap(v0);
      }
      v0 = projectile_index;
    }
    v0 -= 2;
    projectile_index = v0;
  } while ((v0 & 0x8000u) == 0);
  HandleProjectileTrails();
}

void DrawBombAndProjectileExplosions(void) {  // 0x93834D
  uint16 v0 = 18, v2;
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
      R20_ = v3;
      if (!sign16(v3 - 304) || sign16(v3 + 48))
        goto LABEL_16;
      v2 = projectile_y_pos[v1] - layer1_y_pos;
      R18_ = v2;
      goto LABEL_12;
    }
    if ((projectile_type[v1] & 0xF00) == 1280 || (ceres_status & 0x8000u) == 0)
      goto LABEL_9;
    CalcExplosion_Mode7(v0);
    v2 = R18_;
LABEL_12:
    if ((v2 & 0xFF00) != 0)
      ;
    else
      DrawProjectileSpritemap(v0);
    v0 = projectile_index;
LABEL_16:
    v0 -= 2;
    projectile_index = v0;
  } while ((v0 & 0x8000u) == 0);
}