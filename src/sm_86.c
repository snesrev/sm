// Enemy projectiles
#include "ida_types.h"
#include "variables.h"
#include "sm_rtl.h"
#include "funcs.h"
#include "enemy_types.h"

#define kScreenShakeOffsets ((uint16*)RomPtr(0x86846b))
#define kAlignYPos_Tab0 ((uint8*)RomPtr(0x948b2b))
#define kAlignPos_Tab1 ((uint8*)RomPtr(0x94892b))
#define off_868A75 ((uint16*)RomPtr(0x868a75))
#define word_869105 ((uint16*)RomPtr(0x869105))
#define off_86A64D ((uint16*)RomPtr(0x86a64d))
#define stru_86BB50 (*(EnemyProjectileDef*)RomPtr(0x86bb50))
#define off_86BB1E ((uint16*)RomPtr(0x86bb1e))
#define kCommonEnemySpeeds_Quadratic_Copy ((uint16*)RomPtr(0xa0cbc7))
#define off_86C040 ((uint16*)RomPtr(0x86c040))
#define off_86C929 ((uint16*)RomPtr(0x86c929))

void EnableEnemyProjectiles(void) {  // 0x868000
  enemy_projectile_enable_flag |= 0x8000u;
}

void DisableEnemyProjectiles(void) {  // 0x86800B
  enemy_projectile_enable_flag &= ~0x8000u;
}

void ClearEnemyProjectiles(void) {  // 0x868016
  for (int i = 34; i >= 0; i -= 2)
    enemy_projectile_id[i >> 1] = 0;
}

void CallEnemyProjectileInit(uint32 ea, uint16 j) {
  switch (ea) {
  case fnEnemyProj_Init_0x8aaf: EnemyProj_Init_0x8aaf(j); return;
  case fnEnemyProj_Init_0x8bc2_SkreeDownRight: EnemyProj_Init_0x8bc2_SkreeDownRight(j); return;
  case fnEnemyProj_Init_0x8bd0_SkreeUpRight: EnemyProj_Init_0x8bd0_SkreeUpRight(j); return;
  case fnEnemyProj_Init_0x8bde_SkreeDownLeft: EnemyProj_Init_0x8bde_SkreeDownLeft(j); return;
  case fnEnemyProj_Init_0x8bec_SkreeUpLeft: EnemyProj_Init_0x8bec_SkreeUpLeft(j); return;
  case fnEprojInit_DraygonsGunk: EprojInit_DraygonsGunk(j); return;
  case fnEprojInit_DraygonsWallTurretProjs: EprojInit_DraygonsWallTurretProjs(j); return;
  case fnEprojInit_8E6C: EprojInit_8E6C(j); return;
  case fnEprojInit_CrocomireProjectile: EprojInit_CrocomireProjectile(j); return;
  case fnEprojInit_CrocomireSpikeWallPieces: EprojInit_CrocomireSpikeWallPieces(j); return;
  case fnEprojInit_CrocomireBridgeCrumbling: EprojInit_CrocomireBridgeCrumbling(j); return;
  case fnEprojInit_9634: EprojInit_9634(j); return;
  case fnEprojInit_9642_RidleysFireball: EprojInit_9642_RidleysFireball(j); return;
  case fnEprojInit_9660_FireballExplosion: EprojInit_9660_FireballExplosion(j); return;
  case fnEprojInit_9688: EprojInit_9688(j); return;
  case fnEprojInit_9696: EprojInit_9696(j); return;
  case fnEprojInit_966C: EprojInit_966C(j); return;
  case fnEprojInit_967A: EprojInit_967A(j); return;
  case fnEprojInit_9734_CeresFallingDebris: EprojInit_9734_CeresFallingDebris(j); return;
  case fnEprojInit_PhantoonDestroyableFireballs: EprojInit_PhantoonDestroyableFireballs(j); return;
  case fnEprojInit_PhantoonStartingFireballs: EprojInit_PhantoonStartingFireballs(j); return;
  case fnEprojInit_RocksKraidSpits: EprojInit_RocksKraidSpits(j); return;
  case fnEprojInit_RocksFallingKraidCeiling: EprojInit_RocksFallingKraidCeiling(j); return;
  case fnEprojInit_RocksWhenKraidRises: EprojInit_RocksWhenKraidRises(j); return;
  case fnEprojInit_MiniKraidSpit: EprojInit_MiniKraidSpit(j); return;
  case fnEprojInit_MiniKraidSpikesLeft: EprojInit_MiniKraidSpikesLeft(j); return;
  case fnEprojInit_MiniKraidSpikesRight: EprojInit_MiniKraidSpikesRight(j); return;
  case fnEprojInit_WalkingLavaSeahorseFireball: EprojInit_WalkingLavaSeahorseFireball(j); return;
  case fnEprojInit_PirateMotherBrainLaser: EprojInit_PirateMotherBrainLaser(j); return;
  case fnEprojInit_PirateClaw: EprojInit_PirateClaw(j); return;
  case fnEprojInit_A379: EprojInit_A379(j); return;
  case fnEprojInit_CeresElevatorPad: EprojInit_CeresElevatorPad(j); return;
  case fnEprojInit_CeresElevatorPlatform: EprojInit_CeresElevatorPlatform(j); return;
  case fnEprojPreInstr_PrePhantomRoom: EprojPreInstr_PrePhantomRoom(j); return;
  case fnEprojInit_BombTorizoLowHealthDrool: EprojInit_BombTorizoLowHealthDrool(j); return;
  case fnEprojInit_BombTorizoLowHealthInitialDrool: EprojInit_BombTorizoLowHealthInitialDrool(j); return;
  case fnEprojInit_A977: EprojInit_A977(j); return;
  case fnEprojInit_BombTorizoExplosiveSwipe: EprojInit_BombTorizoExplosiveSwipe(j); return;
  case fnEprojInit_BombTorizoStatueBreaking: EprojInit_BombTorizoStatueBreaking(j); return;
  case fnEprojInit_BombTorizoLowHealthExplode: EprojInit_BombTorizoLowHealthExplode(j); return;
  case fnEprojInit_BombTorizoDeathExplosion: EprojInit_BombTorizoDeathExplosion(j); return;
  case fnEprojInit_AB07: EprojInit_AB07(j); return;
  case fnEprojInit_BombTorizosChozoOrbs: EprojInit_BombTorizosChozoOrbs(j); return;
  case fnEprojInit_GoldenTorizosChozoOrbs: EprojInit_GoldenTorizosChozoOrbs(j); return;
  case fnEprojInit_TorizoSonicBoom: EprojInit_TorizoSonicBoom(j); return;
  case fnEprojInit_WreckedShipChozoSpikeFootsteps: EprojInit_WreckedShipChozoSpikeFootsteps(j); return;
  case fnEprojInit_TourianStatueDustClouds: EprojInit_TourianStatueDustClouds(j); return;
  case fnEprojInit_TourianLandingDustCloudsRightFoot: EprojInit_TourianLandingDustCloudsRightFoot(j); return;
  case fnEprojInit_TorizoLandingDustCloudLeftFoot: EprojInit_TorizoLandingDustCloudLeftFoot(j); return;
  case fnEprojInit_GoldenTorizoEgg: EprojInit_GoldenTorizoEgg(j); return;
  case fnEprojInit_GoldenTorizoSuperMissile: EprojInit_GoldenTorizoSuperMissile(j); return;
  case fnEprojInit_GoldenTorizoEyeBeam: EprojInit_GoldenTorizoEyeBeam(j); return;
  case fnEprojInit_TourianEscapeShaftFakeWallExplode: EprojInit_TourianEscapeShaftFakeWallExplode(j); return;
  case fnEprojInit_LavaSeahorseFireball: EprojInit_LavaSeahorseFireball(j); return;
  case fnEprojInit_EyeDoorProjectile: EprojInit_EyeDoorProjectile(j); return;
  case fnEprojInit_EyeDoorSweat: EprojInit_EyeDoorSweat(j); return;
  case fnEprojInit_TourianStatueUnlockingParticleWaterSplash: EprojInit_TourianStatueUnlockingParticleWaterSplash(j); return;
  case fnEprojInit_TourianStatueEyeGlow: EprojInit_TourianStatueEyeGlow(j); return;
  case fnEprojInit_TourianStatueUnlockingParticle: EprojInit_TourianStatueUnlockingParticle(j); return;
  case fnEprojIni_TourianStatueUnlockingParticleTail: EprojIni_TourianStatueUnlockingParticleTail(j); return;
  case fnEprojInit_TourianStatueSoul: EprojInit_TourianStatueSoul(j); return;
  case fnEprojInit_TourianStatueBaseDecoration: EprojInit_TourianStatueBaseDecoration(j); return;
  case fnEprojInit_TourianStatueRidley: EprojInit_TourianStatueRidley(j); return;
  case fnEprojInit_TourianStatuePhantoon: EprojInit_TourianStatuePhantoon(j); return;
  case fnsub_86BB30: sub_86BB30(j); return;
  case fnEprojInit_NuclearWaffleBody: EprojInit_NuclearWaffleBody(j); return;
  case fnEprojInit_NorfairLavaquakeRocks: EprojInit_NorfairLavaquakeRocks(j); return;
  case fnEprojInit_ShaktoolAttackMiddleBackCircle: EprojInit_ShaktoolAttackMiddleBackCircle(j); return;
  case fnEprojInit_BDA2: EprojInit_BDA2(j); return;
  case fnEprojInit_MotherBrainRoomTurrets: EprojInit_MotherBrainRoomTurrets(j); return;
  case fnEprojInit_MotherBrainRoomTurretBullets: EprojInit_MotherBrainRoomTurretBullets(j); return;
  case fnEproj_MotherBrainsBlueRingLasers: Eproj_MotherBrainsBlueRingLasers(j); return;
  case fnEprojInit_MotherBrainBomb: EprojInit_MotherBrainBomb(j); return;
  case fnsub_86C605: sub_86C605(j); return;
  case fnEprojInit_MotherBrainDeathBeemFired: EprojInit_MotherBrainDeathBeemFired(j); return;
  case fnEprojInit_MotherBrainRainbowBeam: EprojInit_MotherBrainRainbowBeam(j); return;
  case fnEprojInit_MotherBrainsDrool: EprojInit_MotherBrainsDrool(j); return;
  case fnEprojInit_MotherBrainsDeathExplosion: EprojInit_MotherBrainsDeathExplosion(j); return;
  case fnEprojInit_MotherBrainsRainbowBeamExplosion: EprojInit_MotherBrainsRainbowBeamExplosion(j); return;
  case fnEprojInit_MotherBrainEscapeDoorParticles: EprojInit_MotherBrainEscapeDoorParticles(j); return;
  case fnEprojInit_MotherBrainPurpleBreathBig: EprojInit_MotherBrainPurpleBreathBig(j); return;
  case fnEprojInit_MotherBrainPurpleBreathSmall: EprojInit_MotherBrainPurpleBreathSmall(j); return;
  case fnEprojInit_TimeBombSetJapaneseText: EprojInit_TimeBombSetJapaneseText(j); return;
  case fnEprojInit_MotherBrainTubeFalling: EprojInit_MotherBrainTubeFalling(j); return;
  case fnEprojInit_MotherBrainGlassShatteringShard: EprojInit_MotherBrainGlassShatteringShard(j); return;
  case fnEprojInit_MotherBrainGlassShatteringSparkle: EprojInit_MotherBrainGlassShatteringSparkle(j); return;
  case fnEprojInit_KiHunterAcidSpitLeft: EprojInit_KiHunterAcidSpitLeft(j); return;
  case fnEprojInit_KiHunterAcidSpitRight: EprojInit_KiHunterAcidSpitRight(j); return;
  case fnEprojInit_KagosBugs: EprojInit_KagosBugs(j); return;
  case fnEprojInit_MaridiaFloatersSpikes: EprojInit_MaridiaFloatersSpikes(j); return;
  case fnEprojInit_WreckedShipRobotLaserDown: EprojInit_WreckedShipRobotLaserDown(j); return;
  case fnEprojInit_WreckedShipRobotLaserHorizontal: EprojInit_WreckedShipRobotLaserHorizontal(j); return;
  case fnEprojInit_WreckedShipRobotLaserUp: EprojInit_WreckedShipRobotLaserUp(j); return;
  case fnEprojInit_N00bTubeCrack: EprojInit_N00bTubeCrack(j); return;
  case fnEprojInit_N00bTubeShards: EprojInit_N00bTubeShards(j); return;
  case fnEprojInit_N00bTubeReleasedAirBubbles: EprojInit_N00bTubeReleasedAirBubbles(j); return;
  case fnsub_86D992: sub_86D992(j); return;
  case fnEprojInit_DBF2: EprojInit_DBF2(j); return;
  case fnEprojInit_Spores: EprojInit_Spores(j); return;
  case fnEprojInit_SporeSpawnStalk: EprojInit_SporeSpawnStalk(j); return;
  case fnEprojInit_SporeSpawners: EprojInit_SporeSpawners(j); return;
  case fnEprojInit_NamiFuneFireball: EprojInit_NamiFuneFireball(j); return;
  case fnEprojInit_LavaThrownByLavaman: EprojInit_LavaThrownByLavaman(j); return;
  case fnEprojInst_DustCloudOrExplosion: EprojInst_DustCloudOrExplosion(j); return;
  case fnEprojInit_EyeDoorSmoke: EprojInit_EyeDoorSmoke(j); return;
  case fnEprojInit_SpawnedShotGate: EprojInit_SpawnedShotGate(j); return;
  case fnEprojInit_ClosedDownwardsShotGate: EprojInit_ClosedDownwardsShotGate(j); return;
  case fnEprojInit_ClosedUpwardsShotGate: EprojInit_ClosedUpwardsShotGate(j); return;
  case fnEprojInit_SaveStationElectricity: EprojInit_SaveStationElectricity(j); return;
  case fnEprojInit_BotwoonsBody: EprojInit_BotwoonsBody(j); return;
  case fnEprojInit_BotwoonsSpit: EprojInit_BotwoonsSpit(j); return;
  case fnEprojInit_YappingMawsBody: EprojInit_YappingMawsBody(j); return;
  case fnEprojInit_F337: EprojInit_F337(j); return;
  case fnEprojInit_EnemyDeathExplosion: EprojInit_EnemyDeathExplosion(j); return;
  case fnEprojInit_Sparks: EprojInit_Sparks(j); return;
  default: Unreachable();
  }
}
void SpawnEnemyProjectileWithGfx(uint16 a, uint16 k, uint16 j) {  // 0x868027
  EnemyProjectileDef *EnemyProjectileDef;

  enemy_projectile_init_param = a;
  EnemyData *v3 = gEnemyData(k);
  uint16 v5 = 34;
  while (enemy_projectile_id[v5 >> 1]) {
    v5 -= 2;
    if ((v5 & 0x8000u) != 0)
      return;
  }
  int v6 = v5 >> 1;
  enemy_projectile_gfx_idx[v6] = v3->vram_tiles_index | v3->palette_index;
  enemy_projectile_id[v6] = j;
  EnemyProjectileDef = get_EnemyProjectileDef(j);
  enemy_projectile_pre_instr[v6] = EnemyProjectileDef->pre_instr_ptr;
  enemy_projectile_instr_list_ptr[v6] = EnemyProjectileDef->instr_list;
  enemy_projectile_radius[v6] = EnemyProjectileDef->radius;
  enemy_projectile_properties[v6] = EnemyProjectileDef->properties;
  enemy_projectile_instr_timers[v6] = 1;
  enemy_projectile_spritemap_ptr[v6] = 0x8000;
  enemy_projectile_E[v6] = 0;
  enemy_projectile_F[v6] = 0;
  enemy_projectile_timers[v6] = 0;
  enemy_projectile_1A27[v6] = 0;
  enemy_projectile_y_subpos[v6] = 0;
  enemy_projectile_G[v6] = 0;
  CallEnemyProjectileInit(EnemyProjectileDef->init_code_ptr | 0x860000, v5);
}

void SpawnEnemyProjectileWithRoomGfx(uint16 j, uint16 a) {  // 0x868097
  EnemyProjectileDef *EnemyProjectileDef;

  enemy_projectile_init_param = a;
  uint16 v3 = 34;
  while (enemy_projectile_id[v3 >> 1]) {
    v3 -= 2;
    if ((v3 & 0x8000u) != 0)
      return;
  }
  int v4 = v3 >> 1;
  enemy_projectile_gfx_idx[v4] = 0;
  enemy_projectile_id[v4] = j;
  EnemyProjectileDef = get_EnemyProjectileDef(j);
  enemy_projectile_pre_instr[v4] = EnemyProjectileDef->pre_instr_ptr;
  enemy_projectile_instr_list_ptr[v4] = EnemyProjectileDef->instr_list;
  enemy_projectile_radius[v4] = EnemyProjectileDef->radius;
  enemy_projectile_properties[v4] = EnemyProjectileDef->properties;
  enemy_projectile_instr_timers[v4] = 1;
  enemy_projectile_spritemap_ptr[v4] = 0x8000;
  enemy_projectile_E[v4] = 0;
  enemy_projectile_F[v4] = 0;
  enemy_projectile_timers[v4] = 0;
  enemy_projectile_timers[v4] = 0;
  enemy_projectile_1A27[v4] = 0;
  enemy_projectile_y_subpos[v4] = 0;
  enemy_projectile_G[v4] = 0;
  CallEnemyProjectileInit(EnemyProjectileDef->init_code_ptr | 0x860000, v3);
}

void EprojRunAll(void) {  // 0x868104
  if ((enemy_projectile_enable_flag & 0x8000u) != 0) {
    for (int i = 34; i >= 0; i -= 2) {
      enemy_projectile_index = i;
      if (enemy_projectile_id[i >> 1]) {
        EprojRunOne(i);
        i = enemy_projectile_index;
      }
    }
  }
}

void CallEprojPreInstr(uint32 ea, uint16 k) {
  switch (ea) {
  case fnnullsub_352: return;
  case fnEprojPreInstr_nullsub_297: return;
  case fnEprojPreInstr_nullsub_83: return;
  case fnEnemyProj_PreInit_0x8aaf: EnemyProj_PreInit_0x8aaf(k); return;
  case fnEnemyProj_PreInstr_SkreeParticle: EnemyProj_PreInstr_SkreeParticle(k); return;
  case fnnullsub_84: return;
  case fnEprojPreInstr_8DCA: EprojPreInstr_8DCA(k); return;
  case fnEprojPreInstr_DraygonsTurret_8DFF: EprojPreInstr_DraygonsTurret_8DFF(k); return;
  case fnEprojPreInstr_DraygonsGunk_8E0F: EprojPreInstr_DraygonsGunk_8E0F(k); return;
  case fnEprojPreInstr_8E6C: EprojPreInstr_8E6C(k); return;
  case fnEprojPreInstr_CrocomireProjectile: EprojPreInstr_CrocomireProjectile(k); return;
  case fnsub_8690B3: sub_8690B3(k); return;
  case fnEprojPreInstr_CrocomireSpikeWallPieces: EprojPreInstr_CrocomireSpikeWallPieces(k); return;
  case fnEprojPreInstr_CrocomireBridgeCrumbling: EprojPreInstr_CrocomireBridgeCrumbling(k); return;
  case fnEprojPreInstr_9634: EprojPreInstr_9634(k); return;
  case fnEprojPreInstr_9642_RidleysFireball: EprojPreInstr_9642_RidleysFireball(k); return;
  case fnnullsub_85: return;
  case fnEprojPreInstr_966C: EprojPreInstr_966C(k); return;
  case fnEprojPreInstr_9688: EprojPreInstr_9688(k); return;
  case fnEprojPreInstr_96A4: EprojPreInstr_96A4(k); return;
  case fnEprojPreInstr_96C0: EprojPreInstr_96C0(k); return;
  case fnEprojPreInstr_96CE: EprojPreInstr_96CE(k); return;
  case fnEprojPreInstr_9734_CeresFallingDebris: EprojPreInstr_9734_CeresFallingDebris(k); return;
  case fnEprojPreInstr_PhantoonStartingFireballs: EprojPreInstr_PhantoonStartingFireballs(k); return;
  case fnEprojPreInstr_PhantoonStartingFireballs2: EprojPreInstr_PhantoonStartingFireballs2(k); return;
  case fnEprojPreInstr_PhantoonStartingFireballs3: EprojPreInstr_PhantoonStartingFireballs3(k); return;
  case fnnullsub_86: return;
  case fnEprojPreInstr_PhantoonDestroyableFireballs: EprojPreInstr_PhantoonDestroyableFireballs(k); return;
  case fnEprojPreInstr_PhantoonDestroyableFireballs_2: EprojPreInstr_PhantoonDestroyableFireballs_2(k); return;
  case fnEprojPreInstr_PhantoonDestroyableFireballs_3: EprojPreInstr_PhantoonDestroyableFireballs_3(k); return;
  case fnEprojPreInstr_PhantoonStartingFireballsB: EprojPreInstr_PhantoonStartingFireballsB(k); return;
  case fnEprojPreInstr_PhantoonStartingFireballsB_2: EprojPreInstr_PhantoonStartingFireballsB_2(k); return;
  case fnEprojPreInstr_KraidRocks: EprojPreInstr_KraidRocks(k); return;
  case fnEprojPreInstr_RocksFallingKraidCeiling: EprojPreInstr_RocksFallingKraidCeiling(k); return;
  case fnsub_869DA5: sub_869DA5(k); return;
  case fnEprojPreInit_MiniKraidSpit: EprojPreInit_MiniKraidSpit(k); return;
  case fnEprojPreInstr_MiniKraidSpikes: EprojPreInstr_MiniKraidSpikes(k); return;
  case fnEprojPreInstr_WalkingLavaSeahorseFireball: EprojPreInstr_WalkingLavaSeahorseFireball(k); return;
  case fnnullsub_87: return;
  case fnEprojPreInstr_PirateMotherBrainLaser_MoveLeft: EprojPreInstr_PirateMotherBrainLaser_MoveLeft(k); return;
  case fnEprojPreInstr_PirateMotherBrainLaser_MoveRight: EprojPreInstr_PirateMotherBrainLaser_MoveRight(k); return;
  case fnEprojPreInstr_PirateClawThrownLeft: EprojPreInstr_PirateClawThrownLeft(k); return;
  case fnEprojPreInstr_PirateClawThrownRight: EprojPreInstr_PirateClawThrownRight(k); return;
  case fnnullsub_88: return;
  case fnEprojPreInstr_CeresElevatorPad: EprojPreInstr_CeresElevatorPad(k); return;
  case fnEprojPreInstr_CeresElevatorPlatform: EprojPreInstr_CeresElevatorPlatform(k); return;
  case fnEprojPreInstr_PrePhantomRoom: EprojPreInstr_PrePhantomRoom(k); return;
  case fnsub_86A887: sub_86A887(k); return;
  case fnEprojPreInstr_A977: EprojPreInstr_A977(k); return;
  case fnnullsub_89: return;
  case fnEprojPreInstr_AB07: EprojPreInstr_AB07(k); return;
  case fnEprojPreInstr_BombTorizosChozoOrbs: EprojPreInstr_BombTorizosChozoOrbs(k); return;
  case fnEprojPreInstr_GoldenTorizosChozoOrbs: EprojPreInstr_GoldenTorizosChozoOrbs(k); return;
  case fnEprojPreInstr_TorizoSonicBoom: EprojPreInstr_TorizoSonicBoom(k); return;
  case fnEprojPreInstr_GoldenTorizoEgg: EprojPreInstr_GoldenTorizoEgg(k); return;
  case fnsub_86B0B9: sub_86B0B9(k); return;
  case fnsub_86B0DD: sub_86B0DD(k); return;
  case fnEprojPreInstr_GoldenTorizoSuperMissile: EprojPreInstr_GoldenTorizoSuperMissile(k); return;
  case fnEprojPreInstr_B237: EprojPreInstr_B237(k); return;
  case fnEprojPreInstr_GoldenTorizoEyeBeam: EprojPreInstr_GoldenTorizoEyeBeam(k); return;
  case fnnullsub_90: return;
  case fnsub_86B535: sub_86B535(k); return;
  case fnEprojPreInstr_EyeDoorProjectile: EprojPreInstr_EyeDoorProjectile(k); return;
  case fnEprojPreInstr_EyeDoorSweat: EprojPreInstr_EyeDoorSweat(k); return;
  case fnEprojPreInstr_TourianStatueUnlockingParticleWaterSplash: EprojPreInstr_TourianStatueUnlockingParticleWaterSplash(k); return;
  case fnEprojPreInstr_TourianStatueUnlockingParticle: EprojPreInstr_TourianStatueUnlockingParticle(k); return;
  case fnEprojPreInstr_TourianStatueSoul: EprojPreInstr_TourianStatueSoul(k); return;
  case fnEprojPreInstr_TourianStatueStuff: EprojPreInstr_TourianStatueStuff(k); return;
  case fnEprojPreInstr_BA42: EprojPreInstr_BA42(k); return;
  case fnnullsub_91: return;
  case fnnullsub_92: return;
  case fnEprojPreInstr_NorfairLavaquakeRocks: EprojPreInstr_NorfairLavaquakeRocks(k); return;
  case fnEprojPreInstr_NorfairLavaquakeRocks_Inner: EprojPreInstr_NorfairLavaquakeRocks_Inner(k); return;
  case fnEprojPreInstr_NorfairLavaquakeRocks_Inner2: EprojPreInstr_NorfairLavaquakeRocks_Inner2(k); return;
  case fnEprojInit_ShaktoolAttackFrontCircle: EprojInit_ShaktoolAttackFrontCircle(k); return;
  case fnEprojPreInstr_BE12: EprojPreInstr_BE12(k); return;
  case fnEprojPreInstr_MotherBrainRoomTurrets: EprojPreInstr_MotherBrainRoomTurrets(k); return;
  case fnEprojPreInstr_MotherBrainRoomTurretBullets: EprojPreInstr_MotherBrainRoomTurretBullets(k); return;
  case fnEproj_MoveToBlueRingSpawnPosition: Eproj_MoveToBlueRingSpawnPosition(k); return;
  case fnEprojPreInstr_MotherBrainBomb: EprojPreInstr_MotherBrainBomb(k); return;
  case fnEprojPreInstr_MotherBrainRainbowBeam: EprojPreInstr_MotherBrainRainbowBeam(k); return;
  case fnEprojPreInstr_C84D: EprojPreInstr_C84D(k); return;
  case fnEprojInit_MotherBrainsDrool_Falling: EprojInit_MotherBrainsDrool_Falling(k); return;
  case fnEprojPreInit_MotherBrainsDeathExplosion_0: EprojPreInit_MotherBrainsDeathExplosion_0(k); return;
  case fnEprojPreInstr_MotherBrainsRainbowBeamExplosion: EprojPreInstr_MotherBrainsRainbowBeamExplosion(k); return;
  case fnEprojPreInstr_MotherBrainsExplodedDoorParticles: EprojPreInstr_MotherBrainsExplodedDoorParticles(k); return;
  case fnnullsub_94: return;
  case fnEprojPreInstr_TimeBombSetJapaneseText: EprojPreInstr_TimeBombSetJapaneseText(k); return;
  case fnEprojPreInstr_MotherBrainTubeFalling: EprojPreInstr_MotherBrainTubeFalling(k); return;
  case fnEprojPreInstr_MotherBrainGlassShatteringShard: EprojPreInstr_MotherBrainGlassShatteringShard(k); return;
  case fnsub_86CFD5: sub_86CFD5(k); return;
  case fnsub_86CFE6: sub_86CFE6(k); return;
  case fnnullsub_95: return;
  case fnsub_86CFF8: sub_86CFF8(k); return;
  case fnEprojPreInstr_KagosBugs_Func1: EprojPreInstr_KagosBugs_Func1(k); return;
  case fnEprojPreInstr_KagosBugs: EprojPreInstr_KagosBugs(k); return;
  case fnnullsub_302: return;
  case fnEprojPreInstr_D0EC: EprojPreInstr_D0EC(k); return;
  case fnEprojPreInstr_D128: EprojPreInstr_D128(k); return;
  case fnEprojPreInstr_KagosBugs_Func2: EprojPreInstr_KagosBugs_Func2(k); return;
  case fnEprojPreInstr_MaridiaFloatersSpikes: EprojPreInstr_MaridiaFloatersSpikes(k); return;
  case fnEprojPreInstr_WreckedShipRobotLaser: EprojPreInstr_WreckedShipRobotLaser(k); return;
  case fnsub_86D7BF: sub_86D7BF(k); return;
  case fnsub_86D7DE: sub_86D7DE(k); return;
  case fnEprojPreInstr_N00bTubeShards: EprojPreInstr_N00bTubeShards(k); return;
  case fnsub_86D83D: sub_86D83D(k); return;
  case fnsub_86D89F: sub_86D89F(k); return;
  case fnsub_86D8DF: sub_86D8DF(k); return;
  case fnEprojPreInstr_SpikeShootingPlantSpikes: EprojPreInstr_SpikeShootingPlantSpikes(k); return;
  case fnEprojPreInstr_SpikeShootingPlantSpikes_MoveY1: EprojPreInstr_SpikeShootingPlantSpikes_MoveY1(k); return;
  case fnEprojPreInstr_SpikeShootingPlantSpikes_MoveY2: EprojPreInstr_SpikeShootingPlantSpikes_MoveY2(k); return;
  case fnEprojPreInstr_SpikeShootingPlantSpikes_MoveX1: EprojPreInstr_SpikeShootingPlantSpikes_MoveX1(k); return;
  case fnEprojPreInstr_SpikeShootingPlantSpikes_MoveX2: EprojPreInstr_SpikeShootingPlantSpikes_MoveX2(k); return;
  case fnEprojPreInstr_SpikeShootingPlantSpikes_0_MoveX1: EprojPreInstr_SpikeShootingPlantSpikes_0_MoveX1(k); return;
  case fnEprojPreInstr_SpikeShootingPlantSpikes_2_MoveX2: EprojPreInstr_SpikeShootingPlantSpikes_2_MoveX2(k); return;
  case fnEprojPreInstr_SpikeShootingPlantSpikes_1_MoveY1: EprojPreInstr_SpikeShootingPlantSpikes_1_MoveY1(k); return;
  case fnEprojPreInstr_SpikeShootingPlantSpikes_4_MoveY2: EprojPreInstr_SpikeShootingPlantSpikes_4_MoveY2(k); return;
  case fnEprojPreInstr_SpikeShootingPlantSpikes_6_MoveX1Y1: EprojPreInstr_SpikeShootingPlantSpikes_6_MoveX1Y1(k); return;
  case fnEprojPreInstr_SpikeShootingPlantSpikes_8_MoveX1Y2: EprojPreInstr_SpikeShootingPlantSpikes_8_MoveX1Y2(k); return;
  case fnEprojPreInstr_SpikeShootingPlantSpikes_7_MoveX2Y1: EprojPreInstr_SpikeShootingPlantSpikes_7_MoveX2Y1(k); return;
  case fnEprojPreInstr_SpikeShootingPlantSpikes_9_MoveX2Y2: EprojPreInstr_SpikeShootingPlantSpikes_9_MoveX2Y2(k); return;
  case fnEprojPreInstr_DBF2: EprojPreInstr_DBF2(k); return;
  case fnEprojPreInstr_DBF2_MoveX1: EprojPreInstr_DBF2_MoveX1(k); return;
  case fnEprojPreInstr_DBF2_MoveX2: EprojPreInstr_DBF2_MoveX2(k); return;
  case fnEprojPreInstr_DBF2_Func1: EprojPreInstr_DBF2_Func1(k); return;
  case fnEprojPreInstr_Spores: EprojPreInstr_Spores(k); return;
  case fnnullsub_96: return;
  case fnEprojPreInstr_SporeSpawners: EprojPreInstr_SporeSpawners(k); return;
  case fnEprojPreInstr_NamiFuneFireball: EprojPreInstr_NamiFuneFireball(k); return;
  case fnsub_86E049: sub_86E049(k); return;
  case fnEprojPreInstr_DustCloudOrExplosion: EprojPreInstr_DustCloudOrExplosion(k); return;
  case fnEprojPreInstr_nullsub_98: return;
  case fnEprojPreInstr_nullsub_99: return;
  case fnEprojPreInstr_E605: EprojPreInstr_E605(k); return;
  case fnnullsub_100: return;
  case fnEprojPreInstr_BotwoonsBody: EprojPreInstr_BotwoonsBody(k); return;
  case fnEprojPreInstr_BotwoonsSpit: EprojPreInstr_BotwoonsSpit(k); return;
  case fnEprojPreInstr_Empty2: return;
  case fnEprojPreInstr_Empty: return;
  case fnEprojPreInstr_Pickup: EprojPreInstr_Pickup(k); return;
  case fnEprojPreInstr_Sparks: EprojPreInstr_Sparks(k); return;
  case fnnullsub_366: return;
  default: Unreachable();
  }
}
uint16 CallEprojInstr(uint32 ea, uint16 k, uint16 j) {
  switch (ea) {
  case fnEprojInstr_Delete: return EprojInstr_Delete(k, j);
  case fnEprojInstr_Sleep: return EprojInstr_Sleep(k, j);
  case fnEprojInstr_SetPreInstr_: return EprojInstr_SetPreInstr_(k, j);
  case fnEprojInstr_ClearPreInstr: return EprojInstr_ClearPreInstr(k, j);
  case fnEprojInstr_CallFunc: return EprojInstr_CallFunc(k, j);
  case fnEprojInstr_Goto: return EprojInstr_Goto(k, j);
  case fnEprojInstr_GotoRel: return EprojInstr_GotoRel(k, j);
  case fnEprojInstr_DecTimerAndGotoIfNonZero: return EprojInstr_DecTimerAndGotoIfNonZero(k, j);
  case fnEprojInstr_DecTimerAndGotoRelIfNonZero: return EprojInstr_DecTimerAndGotoRelIfNonZero(k, j);
  case fnEprojInstr_SetTimer: return EprojInstr_SetTimer(k, j);
  case fnEprojInstr_MoveRandomlyWithinRadius: return EprojInstr_MoveRandomlyWithinRadius(k, j);
  case fnEprojInstr_SetProjectileProperties: return EprojInstr_SetProjectileProperties(k, j);
  case fnEprojInstr_ClearProjectileProperties: return EprojInstr_ClearProjectileProperties(k, j);
  case fnEprojInstr_EnableCollisionWithSamusProj: return EprojInstr_EnableCollisionWithSamusProj(k, j);
  case fnEprojInstr_DisableCollisionWithSamusProj: return EprojInstr_DisableCollisionWithSamusProj(k, j);
  case fnEprojInstr_DisableCollisionWithSamus: return EprojInstr_DisableCollisionWithSamus(k, j);
  case fnEprojInstr_EnableCollisionWithSamus: return EprojInstr_EnableCollisionWithSamus(k, j);
  case fnEprojInstr_SetToNotDieOnContact: return EprojInstr_SetToNotDieOnContact(k, j);
  case fnEprojInstr_SetToDieOnContact: return EprojInstr_SetToDieOnContact(k, j);
  case fnEprojInstr_SetLowPriority: return EprojInstr_SetLowPriority(k, j);
  case fnEprojInstr_SetHighPriority: return EprojInstr_SetHighPriority(k, j);
  case fnEprojInstr_SetXyRadius: return EprojInstr_SetXyRadius(k, j);
  case fnEprojInstr_SetXyRadiusZero: return EprojInstr_SetXyRadiusZero(k, j);
  case fnEprojInstr_CalculateDirectionTowardsSamus: return EprojInstr_CalculateDirectionTowardsSamus(k, j);
  case fnEprojInstr_WriteColorsToPalette: return EprojInstr_WriteColorsToPalette(k, j);
  case fnEprojInstr_QueueMusic: return EprojInstr_QueueMusic(k, j);
  case fnEprojInstr_QueueSfx1_Max6: return EprojInstr_QueueSfx1_Max6(k, j);
  case fnEprojInstr_QueueSfx2_Max6: return EprojInstr_QueueSfx2_Max6(k, j);
  case fnEprojInstr_QueueSfx3_Max6: return EprojInstr_QueueSfx3_Max6(k, j);
  case fnEprojInstr_QueueSfx1_Max15: return EprojInstr_QueueSfx1_Max15(k, j);
  case fnEprojInstr_QueueSfx2_Max15: return EprojInstr_QueueSfx2_Max15(k, j);
  case fnEprojInstr_QueueSfx3_Max15: return EprojInstr_QueueSfx3_Max15(k, j);
  case fnEprojInstr_QueueSfx1_Max3: return EprojInstr_QueueSfx1_Max3(k, j);
  case fnEprojInstr_QueueSfx2_Max3: return EprojInstr_QueueSfx2_Max3(k, j);
  case fnEprojInstr_QueueSfx3_Max3: return EprojInstr_QueueSfx3_Max3(k, j);
  case fnEprojInstr_QueueSfx1_Max9: return EprojInstr_QueueSfx1_Max9(k, j);
  case fnEprojInstr_QueueSfx2_Max9: return EprojInstr_QueueSfx2_Max9(k, j);
  case fnEprojInstr_QueueSfx3_Max9: return EprojInstr_QueueSfx3_Max9(k, j);
  case fnEprojInstr_QueueSfx1_Max1: return EprojInstr_QueueSfx1_Max1(k, j);
  case fnEprojInstr_QueueSfx2_Max1: return EprojInstr_QueueSfx2_Max1(k, j);
  case fnEprojInstr_QueueSfx3_Max1: return EprojInstr_QueueSfx3_Max1(k, j);
  case fnEprojInstr_SpawnEnemyDropsWithDraygonsEyeDrops: return EprojInstr_SpawnEnemyDropsWithDraygonsEyeDrops(k, j);
  case fnEprojInstr_868D55: return EprojInstr_868D55(k, j);
  case fnEprojInstr_868D99: return EprojInstr_868D99(k, j);
  case fnEprojInstr_DisableCollisionsWithSamus: return EprojInstr_DisableCollisionsWithSamus(k, j);
  case fnEprojInstr_95BA: return EprojInstr_95BA(k, j);
  case fnEprojInstr_95ED: return EprojInstr_95ED(k, j);
  case fnEprojInstr_9620: return EprojInstr_9620(k, j);
  case fnEprojInstr_980E: return EprojInstr_980E(k, j);
  case fnEprojInstr_SetPreInstrAndRun: return EprojInstr_SetPreInstrAndRun(k, j);
  case fnEprojInstr_GotoWithProbability25: return EprojInstr_GotoWithProbability25(k, j);
  case fnEprojInstr_SpawnEnemyDrops: return EprojInstr_SpawnEnemyDrops(k, j);
  case fnEprojInstr_GotoDependingOnXDirection: return EprojInstr_GotoDependingOnXDirection(k, j);
  case fnEprojInstr_ResetXYpos1: return EprojInstr_ResetXYpos1(k, j);
  case fnEprojInstr_MoveY_Minus4: return EprojInstr_MoveY_Minus4(k, j);
  case fnEprojInstr_SetVelTowardsSamus1: return EprojInstr_SetVelTowardsSamus1(k, j);
  case fnEprojInstr_SetVelTowardsSamus2: return EprojInstr_SetVelTowardsSamus2(k, j);
  case fnEprojInstr_GotoIfFunc1: return EprojInstr_GotoIfFunc1(k, j);
  case fnEprojInstr_ResetXYpos2: return EprojInstr_ResetXYpos2(k, j);
  case fnEprojInstr_SpawnTourianStatueUnlockingParticle: return EprojInstr_SpawnTourianStatueUnlockingParticle(k, j);
  case fnEprojInstr_Earthquake: return EprojInstr_Earthquake(k, j);
  case fnEprojInstr_SpawnTourianStatueUnlockingParticleTail: return EprojInstr_SpawnTourianStatueUnlockingParticleTail(k, j);
  case fnEprojInstr_AddToYpos: return EprojInstr_AddToYpos(k, j);
  case fnEprojInstr_BB24: return EprojInstr_BB24(k, j);
  case fnEprojInstr_SwitchJump: return EprojInstr_SwitchJump(k, j);
  case fnEprojInstr_UserPalette0: return EprojInstr_UserPalette0(k, j);
  case fnEprojInstr_Add12ToY: return EprojInstr_Add12ToY(k, j);
  case fnEprojInstr_MotherBrainPurpleBreathIsActive: return EprojInstr_MotherBrainPurpleBreathIsActive(k, j);
  case fnEprojInstr_D15C: return EprojInstr_D15C(k, j);
  case fnEprojInstr_D1B6: return EprojInstr_D1B6(k, j);
  case fnEprojInstr_D1C7: return EprojInstr_D1C7(k, j);
  case fnEprojInstr_D1CE: return EprojInstr_D1CE(k, j);
  case fnEprojInstr_AssignNewN00bTubeShardVelocity: return EprojInstr_AssignNewN00bTubeShardVelocity(k, j);
  case fnEprojInstr_SetN00bTubeShardX: return EprojInstr_SetN00bTubeShardX(k, j);
  case fnEprojInstr_D62A: return EprojInstr_D62A(k, j);
  case fnEprojInstr_SetXvelRandom: return EprojInstr_SetXvelRandom(k, j);
  case fnEprojInstr_DC5A: return EprojInstr_DC5A(k, j);
  case fnEprojInstr_SpawnEnemyDrops_0: return EprojInstr_SpawnEnemyDrops_0(k, j);
  case fnEprojInstr_SpawnSporesEproj: return EprojInstr_SpawnSporesEproj(k, j);
  case fnEprojInstr_DFEA: return EprojInstr_DFEA(k, j);
  case fnEprojInstr_SetYVel: return EprojInstr_SetYVel(k, j);
  case fnEprojInstr_ECE3: return EprojInstr_ECE3(k, j);
  case fnEprojInstr_ED17: return EprojInstr_ED17(k, j);
  case fnEprojInstr_QueueSfx2_9: return EprojInstr_QueueSfx2_9(k, j);
  case fnEprojInstr_QueueSfx2_24: return EprojInstr_QueueSfx2_24(k, j);
  case fnEprojInstr_QueueSfx2_B: return EprojInstr_QueueSfx2_B(k, j);
  case fnEprojInstr_EEAF: return EprojInstr_EEAF(k, j);
  case fnEprojInstr_HandleRespawningEnemy: return EprojInstr_HandleRespawningEnemy(k, j);
  case fnEprojInstr_SetPreInstrA: return EprojInstr_SetPreInstrA(k, j);
  case fnEprojInstr_SetPreInstrB: return EprojInstr_SetPreInstrB(k, j);
    // EXTRA!!!
  case fnEprojPreInstr_PirateMotherBrainLaser_MoveRight:
    EprojPreInstr_PirateMotherBrainLaser_MoveRight(k);
    return j;
  case fnEprojPreInstr_PirateMotherBrainLaser_MoveLeft:
    EprojPreInstr_PirateMotherBrainLaser_MoveLeft(k);
    return j;

  case fnEprojInstr_A3BE: return EprojInstr_A3BE(k, j);
  case fnEprojInstr_9270: return EprojInstr_9270(k, j);
  case fnnullsub_82: return j; // really j
  case fnsub_86B13E: return sub_86B13E(k, j);
  default: return Unreachable();
  }
}

void EprojRunOne(uint16 k) {  // 0x868125
  CallEprojPreInstr(enemy_projectile_pre_instr[k >> 1] | 0x860000, k);
  uint16 v1 = enemy_projectile_index;
  int v2 = enemy_projectile_index >> 1;
  if (enemy_projectile_instr_timers[v2]-- == 1) {
    uint16 v4 = enemy_projectile_instr_list_ptr[v2], v6;
    while (1) {
      uint16 *v5 = (uint16 *)RomPtr_86(v4);
      v6 = *v5;
      if ((*v5 & 0x8000u) == 0)
        break;
      R18_ = *v5;
      v4 = CallEprojInstr(v6 | 0x860000, v1, v4 + 2);
      if (!v4)
        return;
    }
    if (v4 == 0) {

      v4 = 0;
    }
    int v7 = v1 >> 1;
    enemy_projectile_instr_timers[v7] = v6;
    enemy_projectile_spritemap_ptr[v7] = *((uint16 *)RomPtr_86(v4) + 1);
    enemy_projectile_instr_list_ptr[v7] = v4 + 4;
  }
}

uint16 EprojInstr_Delete(uint16 k, uint16 j) {  // 0x868154
  enemy_projectile_id[k >> 1] = 0;
  return 0;
}

uint16 EprojInstr_Sleep(uint16 k, uint16 j) {  // 0x868159
  enemy_projectile_instr_list_ptr[k >> 1] = j - 2;
  return 0;
}

uint16 EprojInstr_SetPreInstr_(uint16 k, uint16 j) {  // 0x868161
  enemy_projectile_pre_instr[k >> 1] = *(uint16 *)RomPtr_86(j);
  return j + 2;
}

uint16 EprojInstr_ClearPreInstr(uint16 k, uint16 j) {  // 0x86816A
  enemy_projectile_pre_instr[k >> 1] = 0x8170;
  return j;
}

void CallEprojFunc(uint32 ea, uint32 k) {
  switch (ea) {
  case fnSpawnMotherBrainDeathBeam: SpawnMotherBrainDeathBeam(k); return;
  default: Unreachable();
  }
}

uint16 EprojInstr_CallFunc(uint16 k, uint16 j) {  // 0x868171
  uint8 *v2 = RomPtr_86(j);
  copy24((LongPtr *)&R18_, (LongPtr *)v2);

  uint32 ea = Load24((LongPtr *)&R18_);
  CallEprojFunc(ea, k);

  //CallFar((LongPtr *)&R18_);
  return j + 3;
}

uint16 EprojInstr_Goto(uint16 k, uint16 j) {  // 0x8681AB
  return *(uint16 *)RomPtr_86(j);
}

uint16 EprojInstr_GotoRel(uint16 k, uint16 j) {  // 0x8681B0
  R18_ = j;
  return R18_ + (int8)*RomPtr_86(j);
}

uint16 EprojInstr_DecTimerAndGotoIfNonZero(uint16 k, uint16 j) {  // 0x8681C6
  int v2 = k >> 1;
  if (enemy_projectile_timers[v2]-- == 1)
    return j + 2;
  else
    return EprojInstr_Goto(k, j);
}

uint16 EprojInstr_DecTimerAndGotoRelIfNonZero(uint16 k, uint16 j) {  // 0x8681CE
  int v2 = k >> 1;
  if (enemy_projectile_timers[v2]-- == 1)
    return j + 1;
  else
    return EprojInstr_GotoRel(k, j);
}

uint16 EprojInstr_SetTimer(uint16 k, uint16 j) {  // 0x8681D5
  enemy_projectile_timers[k >> 1] = *(uint16 *)RomPtr_86(j);
  return j + 2;
}

uint16 EprojInstr_MoveRandomlyWithinRadius(uint16 k, uint16 j) {  // 0x8681DF
  char Random;
  char v4;
  int16 v6;

  R18_ = NextRandom();
  do {
    Random = NextRandom();
    uint8 *v3 = RomPtr_86(j);
    v4 = (*v3 & Random) - v3[1];
  } while (v4 < 0);
  enemy_projectile_x_pos[k >> 1] += sign16(R18_) ? -(uint8)v4 : (uint8)v4;
  do {
    LOBYTE(v6) = NextRandom();
    uint8 *v5 = RomPtr_86(j);
    LOBYTE(v6) = (v5[2] & v6) - v5[3];
  } while ((v6 & 0x80) != 0);
  v6 = (uint8)v6;
  if ((R18_ & 0x4000) != 0)
    v6 = -(uint8)v6;
  enemy_projectile_y_pos[k >> 1] += v6;
  return j + 4;
}

uint16 EprojInstr_SetProjectileProperties(uint16 k, uint16 j) {  // 0x868230
  enemy_projectile_properties[k >> 1] |= *(uint16 *)RomPtr_86(j);
  return j + 2;
}

uint16 EprojInstr_ClearProjectileProperties(uint16 k, uint16 j) {  // 0x86823C
  enemy_projectile_properties[k >> 1] &= *(uint16 *)RomPtr_86(j);
  return j + 2;
}

uint16 EprojInstr_EnableCollisionWithSamusProj(uint16 k, uint16 j) {  // 0x868248
  enemy_projectile_properties[k >> 1] |= 0x8000u;
  return j;
}

uint16 EprojInstr_DisableCollisionWithSamusProj(uint16 k, uint16 j) {  // 0x868252
  enemy_projectile_properties[k >> 1] &= ~0x8000u;
  return j;
}

uint16 EprojInstr_DisableCollisionWithSamus(uint16 k, uint16 j) {  // 0x86825C
  enemy_projectile_properties[k >> 1] |= 0x2000u;
  return j;
}

uint16 EprojInstr_EnableCollisionWithSamus(uint16 k, uint16 j) {  // 0x868266
  enemy_projectile_properties[k >> 1] &= ~0x2000u;
  return j;
}

uint16 EprojInstr_SetToNotDieOnContact(uint16 k, uint16 j) {  // 0x868270
  enemy_projectile_properties[k >> 1] |= 0x4000u;
  return j;
}

uint16 EprojInstr_SetToDieOnContact(uint16 k, uint16 j) {  // 0x86827A
  enemy_projectile_properties[k >> 1] &= ~0x4000u;
  return j;
}

uint16 EprojInstr_SetLowPriority(uint16 k, uint16 j) {  // 0x868284
  enemy_projectile_properties[k >> 1] |= 0x1000u;
  return j;
}

uint16 EprojInstr_SetHighPriority(uint16 k, uint16 j) {  // 0x86828E
  enemy_projectile_properties[k >> 1] &= ~0x1000u;
  return j;
}

uint16 EprojInstr_SetXyRadius(uint16 k, uint16 j) {  // 0x868298
  enemy_projectile_radius[k >> 1] = *(uint16 *)RomPtr_86(j);
  return j + 2;
}

uint16 EprojInstr_SetXyRadiusZero(uint16 k, uint16 j) {  // 0x8682A1
  enemy_projectile_radius[k >> 1] = 0;
  return j;
}

uint16 EprojInstr_CalculateDirectionTowardsSamus(uint16 k, uint16 j) {  // 0x8682A5
  int v2 = k >> 1;
  R18_ = samus_x_pos - enemy_projectile_x_pos[v2];
  R20_ = samus_y_pos - enemy_projectile_y_pos[v2];
  uint16 v3 = 2 * CalculateAngleFromXY();
  int v4 = enemy_projectile_index >> 1;
  enemy_projectile_E[v4] = v3;
  int v5 = v3 >> 1;
  enemy_projectile_x_vel[v4] = kSinCosTable8bit_Sext[v5 + 64];
  enemy_projectile_y_vel[v4] = kSinCosTable8bit_Sext[v5];
  return j;
}

uint16 EprojInstr_WriteColorsToPalette(uint16 k, uint16 j) {  // 0x8682D5
  uint8 *v2 = RomPtr_86(j);
  uint16 v3 = *((uint16 *)v2 + 1);
  R18_ = v2[4];
  uint16 v4 = *(uint16 *)v2;
  do {
    palette_buffer[v3 >> 1] = *(uint16 *)RomPtr_86(v4);
    v4 += 2;
    v3 += 2;
    --R18_;
  } while ((R18_ & 0x8000u) == 0);
  return j + 5;
}

uint16 EprojInstr_QueueMusic(uint16 k, uint16 j) {  // 0x8682FD
  uint8 *v2 = RomPtr_86(j);
  QueueMusic_Delayed8(*v2);
  return j + 1;
}

uint16 EprojInstr_QueueSfx1_Max6(uint16 k, uint16 j) {  // 0x868309
  uint16 *v2 = (uint16 *)RomPtr_86(j);
  QueueSfx1_Max6(*v2);
  return j + 1;
}
uint16 EprojInstr_QueueSfx2_Max6(uint16 k, uint16 j) {  // 0x868312
  uint16 *v2 = (uint16 *)RomPtr_86(j);
  QueueSfx2_Max6(*v2);
  return j + 1;
}
uint16 EprojInstr_QueueSfx3_Max6(uint16 k, uint16 j) {  // 0x86831B
  uint16 *v2 = (uint16 *)RomPtr_86(j);
  QueueSfx3_Max6(*v2);
  return j + 1;
}
uint16 EprojInstr_QueueSfx1_Max15(uint16 k, uint16 j) {  // 0x868324
  uint16 *v2 = (uint16 *)RomPtr_86(j);
  QueueSfx1_Max15(*v2);
  return j + 1;
}

uint16 EprojInstr_QueueSfx2_Max15(uint16 k, uint16 j) {  // 0x86832D
  uint16 *v2 = (uint16 *)RomPtr_86(j);
  QueueSfx2_Max15(*v2);
  return j + 1;
}

uint16 EprojInstr_QueueSfx3_Max15(uint16 k, uint16 j) {  // 0x868336
  uint16 *v2 = (uint16 *)RomPtr_86(j);
  QueueSfx3_Max15(*v2);
  return j + 1;
}

uint16 EprojInstr_QueueSfx1_Max3(uint16 k, uint16 j) {  // 0x86833F
  uint16 *v2 = (uint16 *)RomPtr_86(j);
  QueueSfx1_Max3(*v2);
  return j + 1;
}

uint16 EprojInstr_QueueSfx2_Max3(uint16 k, uint16 j) {  // 0x868348
  uint16 *v2 = (uint16 *)RomPtr_86(j);
  QueueSfx2_Max3(*v2);
  return j + 1;
}

uint16 EprojInstr_QueueSfx3_Max3(uint16 k, uint16 j) {  // 0x868351
  uint16 *v2 = (uint16 *)RomPtr_86(j);
  QueueSfx3_Max3(*v2);
  return j + 1;
}

uint16 EprojInstr_QueueSfx1_Max9(uint16 k, uint16 j) {  // 0x86835A
  uint16 *v2 = (uint16 *)RomPtr_86(j);
  QueueSfx1_Max9(*v2);
  return j + 1;
}

uint16 EprojInstr_QueueSfx2_Max9(uint16 k, uint16 j) {  // 0x868363
  uint16 *v2 = (uint16 *)RomPtr_86(j);
  QueueSfx2_Max9(*v2);
  return j + 1;
}

uint16 EprojInstr_QueueSfx3_Max9(uint16 k, uint16 j) {  // 0x86836C
  uint16 *v2 = (uint16 *)RomPtr_86(j);
  QueueSfx3_Max9(*v2);
  return j + 1;
}
uint16 EprojInstr_QueueSfx1_Max1(uint16 k, uint16 j) {  // 0x868375
  uint16 *v2 = (uint16 *)RomPtr_86(j);
  QueueSfx1_Max1(*v2);
  return j + 1;
}
uint16 EprojInstr_QueueSfx2_Max1(uint16 k, uint16 j) {  // 0x86837E
  uint16 *v2 = (uint16 *)RomPtr_86(j);
  QueueSfx2_Max1(*v2);
  return j + 1;
}
uint16 EprojInstr_QueueSfx3_Max1(uint16 k, uint16 j) {  // 0x868387
  uint16 *v2 = (uint16 *)RomPtr_86(j);
  QueueSfx3_Max1(*v2);
  return j + 1;
}

void DrawLowPriorityEnemyProjectiles(void) {  // 0x868390
  GetValuesForScreenShaking();
  for (int i = 34; i >= 0; i -= 2) {
    int v1 = i >> 1;
    if (enemy_projectile_id[v1]) {
      if ((enemy_projectile_properties[v1] & 0x1000) != 0)
        DrawEnemyProjectiles(i);
    }
  }
}

void DrawHighPriorityEnemyProjectiles(void) {  // 0x8683B2
  GetValuesForScreenShaking();
  for (int i = 34; i >= 0; i -= 2) {
    int v1 = i >> 1;
    if (enemy_projectile_id[v1]) {
      if ((enemy_projectile_properties[v1] & 0x1000) == 0)
        DrawEnemyProjectiles(i);
    }
  }
}

void DrawEnemyProjectiles(uint16 k) {  // 0x8683D6
  int v1 = k >> 1;
  uint16 v2 = enemy_projectile_spritemap_ptr[v1];
  R26_ = LOBYTE(enemy_projectile_gfx_idx[v1]);
  R28_ = enemy_projectile_gfx_idx[v1] & 0xFF00;
  R20_ = R36 + enemy_projectile_x_pos[v1] - layer1_x_pos;
  if (((R20_ + 128) & 0xFE00) == 0) {
    uint16 v3 = R34 + enemy_projectile_y_pos[v1] - layer1_y_pos;
    R18_ = v3;
    if ((v3 & 0xFF00) != 0) {
      if (((v3 + 128) & 0xFE00) == 0)
        DrawEnemyProjectileSpritemapWithBaseTileOffscreen(0x8D, v2);
    } else {
      DrawEnemyProjectileSpritemapWithBaseTile(0x8D, v2);
    }
  }
}

void GetValuesForScreenShaking(void) {  // 0x868427
  if (earthquake_timer && !time_is_frozen_flag && sign16(earthquake_type - 36)) {
    int v0 = (uint16)(4 * earthquake_type) >> 1;
    if ((earthquake_timer & 2) != 0) {
      R36 = -kScreenShakeOffsets[v0];
      R34 = -kScreenShakeOffsets[v0 + 1];
    } else {
      R36 = kScreenShakeOffsets[v0];
      R34 = kScreenShakeOffsets[v0 + 1];
    }
  } else {
    R34 = 0;
    R36 = 0;
  }
}

uint8 EprojColl_8506(void) {  // 0x868506
  uint8 t = BTS[cur_block_index];
  if (t) {
    cur_block_index += (int8)t;
    return 0xff;
  }
  return 0;
}

uint8 EprojColl_Unknown8536(void) {  // 0x86853C
  if (BTS[cur_block_index]) {
    uint16 v1;
    if ((BTS[cur_block_index] & 0x80) != 0) {
      temp_collision_DD4 = BTS[cur_block_index] | 0xFF00;
      v1 = cur_block_index;
      do {
        v1 -= room_width_in_blocks;
        ++temp_collision_DD4;
      } while (temp_collision_DD4);
    } else {
      temp_collision_DD4 = BTS[cur_block_index];
      v1 = cur_block_index;
      do {
        v1 += room_width_in_blocks;
        --temp_collision_DD4;
      } while (temp_collision_DD4);
    }
    cur_block_index = v1;
    return 0xff;
  }
  return 0;
}

uint8 EprojColl_ClearCarry(void) {  // 0x86858A
  return 0;
}

uint8 EprojColl_SetCarry(void) {  // 0x86858C
  return 1;
}

uint8 EprojColl_858E(void) {  // 0x86858E
  uint16 v0 = BTS[cur_block_index] & 0x1F;
  if (v0 < 5u)
    return EprojColl_85C2(v0, cur_block_index);
  current_slope_bts = BTS[cur_block_index];
  return EprojColl_873D();
}

uint8 EprojColl_85AD(void) {  // 0x8685AD
  uint16 v0 = BTS[cur_block_index] & 0x1F;
  if (v0 >= 5u)
    return EprojColl_874E();
  else
    return EprojColl_8676(v0, cur_block_index, 0);
}

static const uint8 unk_868729[20] = {  // 0x8685C2
     0,    1, 0x82, 0x83, 0, 0x81, 2, 0x83, 0, 1, 2, 0x83, 0, 0x81, 0x82, 0x83,
  0x80, 0x81, 0x82, 0x83,
};


#define CHECK_locret_868728(i) (unk_868729[i] & 0x80 ? -1 : 0)

uint8 EprojColl_85C2(uint16 a, uint16 k) {

  temp_collision_DD4 = 4 * a;
  temp_collision_DD6 = BTS[k] >> 6;
  uint16 v2 = 4 * a + (temp_collision_DD6 ^ ((uint8)(R34 & 8) >> 3));
  if (!R32) {
    int v3 = enemy_projectile_index >> 1;
    if ((((uint8)enemy_projectile_y_pos[v3] - (uint8)g_word_7E001E) & 8) != 0
        || CHECK_locret_868728(v2) >= 0) {
      uint16 v4 = v2 ^ 2;
      if ((((uint8)g_word_7E001E + (uint8)enemy_projectile_y_pos[v3] - 1) & 8) == 0
          || CHECK_locret_868728(v4) >= 0) {
        return 0;
      }
    }
    goto LABEL_17;
  }
  if (!R26_) {
    if ((((uint8)g_word_7E001E + (uint8)enemy_projectile_y_pos[enemy_projectile_index >> 1] - 1) & 8) == 0) {
      if (CHECK_locret_868728(v2) >= 0)
        return 0;
      goto LABEL_17;
    }
    goto LABEL_14;
  }
  if (R26_ != R32
      || (((uint8)enemy_projectile_y_pos[enemy_projectile_index >> 1] - (uint8)g_word_7E001E) & 8) == 0) {
LABEL_14:
    if (CHECK_locret_868728(v2) < 0)
      goto LABEL_17;
  }
  if (CHECK_locret_868728(v2 ^ 2) >= 0)
    return 0;
LABEL_17:;
  int v6 = enemy_projectile_index >> 1;
  uint16 v7;
  enemy_projectile_1A27[v6] = 0;
  if ((R20_ & 0x8000u) != 0)
    v7 = R28_ + (R34 | 7) + 1;
  else
    v7 = (R34 & 0xFFF8) - R28_;
  enemy_projectile_x_pos[v6] = v7;
  return 1;
}

uint8 EprojColl_8676(uint16 a, uint16 k, uint16 j) {  // 0x868676
  uint16 v2 = enemy_projectile_index;

  temp_collision_DD4 = 4 * a;
  temp_collision_DD6 = BTS[k] >> 6;
  uint16 v3 = 4 * a + (temp_collision_DD6 ^ ((uint8)(R34 & 8) >> 2));
  if (!R32) {
    printf("Y unknown!\n");
    int v4 = v2 >> 1;
    if ((((uint8)enemy_projectile_x_pos[v4] - (uint8)R28_) & 8) != 0
        || CHECK_locret_868728(v3) >= 0) {
      uint16 v5 = v3 ^ 1;
      if ((((uint8)R28_ + (uint8)enemy_projectile_x_pos[v4] - 1) & 8) == 0
          || CHECK_locret_868728(v5) >= 0) {
        return 0;
      }
    }
    goto LABEL_17;
  }
  if (!R26_) {
    if ((((uint8)R28_ + (uint8)enemy_projectile_x_pos[enemy_projectile_index >> 1] - 1) & 8) == 0) {
      if (CHECK_locret_868728(v3) >= 0)
        return 0;
      goto LABEL_17;
    }
    goto LABEL_14;
  }
  if (R26_ != R32 || (((uint8)enemy_projectile_x_pos[enemy_projectile_index >> 1] - (uint8)R28_) & 8) == 0) {
LABEL_14:
    if (CHECK_locret_868728(v3) < 0)
      goto LABEL_17;
  }
  if (CHECK_locret_868728(v3 ^ 1) >= 0)
    return 0;
LABEL_17:;
  int v7 = enemy_projectile_index >> 1;
  uint16 v8;
  enemy_projectile_y_subpos[v7] = 0;
  if ((R20_ & 0x8000u) != 0)
    v8 = g_word_7E001E + (R34 | 7) + 1;
  else
    v8 = (R34 & 0xFFF8) - g_word_7E001E;
  enemy_projectile_y_pos[v7] = v8;
  return 1;
}

uint8 EprojColl_873D(void) {  // 0x86873D
  return 0;
}

uint8 EprojColl_874E(void) {  // 0x86874E
  int16 v3;
  int16 v5;
  uint16 v6;
  uint16 v7;
  int16 v8;
  int16 v11;
  int16 v12;
  uint16 v13;
  uint16 v14;
  int16 v15;

  uint16 v0 = enemy_projectile_index;
  if ((R20_ & 0x8000u) != 0) {
    uint16 v9 = cur_block_index;
    uint16 mod = SnesModulus(cur_block_index, room_width_in_blocks);
    int v10 = v0 >> 1;
    v11 = enemy_projectile_x_pos[v10] >> 4;
    if (v11 == mod) {
      temp_collision_DD4 = ((uint8)R24_ - (uint8)g_word_7E001E) & 0xF ^ 0xF;
      temp_collision_DD6 = 16 * (BTS[v9] & 0x1F);
      v12 = BTS[v9] << 8;
      if (v12 < 0
          && ((v12 & 0x4000) != 0 ? (v13 = enemy_projectile_x_pos[v10] ^ 0xF) : (v13 = enemy_projectile_x_pos[v10]),
              (v14 = temp_collision_DD6 + (v13 & 0xF),
               v15 = (kAlignYPos_Tab0[v14] & 0x1F) - temp_collision_DD4 - 1,
               (kAlignYPos_Tab0[v14] & 0x1F) - temp_collision_DD4 == 1)
              || v15 < 0)) {
        enemy_projectile_y_pos[v10] = R24_ - v15;
        enemy_projectile_y_subpos[v10] = 0;
        return 1;
      } else {
        return 0;
      }
    } else {
      return 0;
    }
  } else {
    uint16 v1 = cur_block_index;
    uint16 mod = SnesModulus(cur_block_index, room_width_in_blocks);
    int v2 = v0 >> 1;
    v3 = enemy_projectile_x_pos[v2] >> 4;
    if (v3 == mod) {
      temp_collision_DD4 = ((uint8)g_word_7E001E + (uint8)R24_ - 1) & 0xF;
      temp_collision_DD6 = 16 * (BTS[v1] & 0x1F);
      v5 = BTS[v1] << 8;
      if (v5 >= 0
          && ((v5 & 0x4000) != 0 ? (v6 = enemy_projectile_x_pos[v2] ^ 0xF) : (v6 = enemy_projectile_x_pos[v2]),
              (v7 = temp_collision_DD6 + (v6 & 0xF),
               v8 = (kAlignYPos_Tab0[v7] & 0x1F) - temp_collision_DD4 - 1,
               (kAlignYPos_Tab0[v7] & 0x1F) - temp_collision_DD4 == 1)
              || v8 < 0)) {
        enemy_projectile_y_pos[v2] = R24_ + v8;
        enemy_projectile_y_subpos[v2] = -1;
        return 1;
      } else {
        return 0;
      }
    } else {
      return 0;
    }
  }
}

static Func_U8 *const kEnemyProjectileBlockCollisition_FuncA[16] = {  // 0x868886
  EprojColl_ClearCarry,
  EprojColl_858E,
  EprojColl_ClearCarry,
  EprojColl_ClearCarry,
  EprojColl_ClearCarry,
  EprojColl_8506,
  EprojColl_ClearCarry,
  EprojColl_ClearCarry,
  EprojColl_SetCarry,
  EprojColl_SetCarry,
  EprojColl_SetCarry,
  EprojColl_SetCarry,
  EprojColl_SetCarry,
  EprojColl_Unknown8536,
  EprojColl_ClearCarry,
  EprojColl_SetCarry,
};
static Func_U8 *const kEnemyProjectileBlockCollisition_FuncB[16] = {
  EprojColl_ClearCarry,
  EprojColl_85AD,
  EprojColl_ClearCarry,
  EprojColl_ClearCarry,
  EprojColl_ClearCarry,
  EprojColl_8506,
  EprojColl_ClearCarry,
  EprojColl_ClearCarry,
  EprojColl_SetCarry,
  EprojColl_SetCarry,
  EprojColl_SetCarry,
  EprojColl_SetCarry,
  EprojColl_SetCarry,
  EprojColl_Unknown8536,
  EprojColl_ClearCarry,
  EprojColl_SetCarry,
};

uint8 EnemyProjectileBlockCollisition_CheckHorizontal(uint16 k) {
  uint8 rv;
  cur_block_index = k >> 1;
  do {
    rv = kEnemyProjectileBlockCollisition_FuncA[(level_data[cur_block_index] & 0xF000) >> 12]();
  } while (rv & 0x80);
  return rv;
}

uint8 EnemyProjectileBlockCollisition_CheckVertical(uint16 k) {  // 0x86889E
  uint8 rv;
  cur_block_index = k >> 1;
  do {
    rv = kEnemyProjectileBlockCollisition_FuncB[(level_data[cur_block_index] & 0xF000) >> 12]();
  } while (rv & 0x80);
  return rv;
}

uint8 EnemyProjectileBlockCollisition_Horiz(uint16 k) {  // 0x8688B6
  int16 v2;

  R18_ = 0;
  R20_ = 0;
  int v1 = k >> 1;
  v2 = enemy_projectile_x_vel[v1];
  if (v2 < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = v2;
  g_word_7E001E = HIBYTE(enemy_projectile_radius[v1]);
  R28_ = LOBYTE(enemy_projectile_radius[v1]);
  R26_ = (enemy_projectile_y_pos[v1] - g_word_7E001E) & 0xFFF0;
  R26_ = (uint16)(g_word_7E001E + enemy_projectile_y_pos[v1] - 1 - R26_) >> 4;
  R32 = R26_;
  uint16 prod = Mult8x8((uint16)(enemy_projectile_y_pos[v1] - g_word_7E001E) >> 4, room_width_in_blocks);
  uint16 v3 = (__PAIR32__(R20_, R18_) + __PAIR32__(enemy_projectile_x_pos[v1], enemy_projectile_1A27[v1])) >> 16;
  R22_ = R18_ + enemy_projectile_1A27[v1];
  R24_ = v3;
  uint16 v4;
  if ((R20_ & 0x8000u) != 0)
    v4 = v3 - R28_;
  else
    v4 = R28_ + v3 - 1;
  R34 = v4;
  uint16 v5 = 2 * (prod + (v4 >> 4));
  while (!(EnemyProjectileBlockCollisition_CheckHorizontal(v5) & 1)) {
    v5 += room_width_in_blocks * 2;
    if ((--R26_ & 0x8000u) != 0) {
      int v6 = k >> 1;
      enemy_projectile_1A27[v6] = R22_;
      enemy_projectile_x_pos[v6] = R24_;
      return 0;
    }
  }
  int v8 = k >> 1;
  enemy_projectile_1A27[v8] = 0;
  if ((R20_ & 0x8000u) != 0) {
    uint16 v10 = R28_ + (R34 | 0xF) + 1;
    if (v10 < enemy_projectile_x_pos[v8] || v10 == enemy_projectile_x_pos[v8])
      enemy_projectile_x_pos[v8] = v10;
    return 1;
  } else {
    uint16 v9 = (R34 & 0xFFF0) - R28_;
    if (v9 >= enemy_projectile_x_pos[v8])
      enemy_projectile_x_pos[v8] = v9;
    return 1;
  }
}

uint8 EnemyProjectileBlockCollisition_Vertical(uint16 k) {  // 0x86897B
  int16 v2;
  int16 v5;

  R18_ = 0;
  R20_ = 0;
  int v1 = k >> 1;
  v2 = enemy_projectile_y_vel[v1];
  if (v2 < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = v2;
  g_word_7E001E = HIBYTE(enemy_projectile_radius[v1]);
  R28_ = LOBYTE(enemy_projectile_radius[v1]);
  R26_ = (enemy_projectile_x_pos[v1] - R28_) & 0xFFF0;
  R26_ = (uint16)(R28_ + enemy_projectile_x_pos[v1] - 1 - R26_) >> 4;
  R32 = R26_;
  uint16 v3 = (__PAIR32__(R20_, R18_) + __PAIR32__(enemy_projectile_y_pos[v1], enemy_projectile_y_subpos[v1])) >> 16;
  R22_ = R18_ + enemy_projectile_y_subpos[v1];
  R24_ = v3;
  uint16 v4;
  if ((R20_ & 0x8000u) != 0)
    v4 = v3 - g_word_7E001E;
  else
    v4 = g_word_7E001E + v3 - 1;
  R34 = v4;
  uint16 prod = Mult8x8(v4 >> 4, room_width_in_blocks);
  v5 = (uint16)(enemy_projectile_x_pos[v1] - R28_) >> 4;
  for (int i = 2 * (prod + v5); !(EnemyProjectileBlockCollisition_CheckVertical(i) & 1); i += 2) {
    if ((--R26_ & 0x8000u) != 0) {
      int v7 = k >> 1;
      enemy_projectile_y_subpos[v7] = R22_;
      enemy_projectile_y_pos[v7] = R24_;
      return 0;
    }
  }
  int v9 = k >> 1;
  enemy_projectile_y_subpos[v9] = 0;
  if ((R20_ & 0x8000u) != 0) {
    uint16 v11 = g_word_7E001E + (R34 | 0xF) + 1;
    if (v11 < enemy_projectile_y_pos[v9] || v11 == enemy_projectile_y_pos[v9])
      enemy_projectile_y_pos[v9] = v11;
    return 1;
  } else {
    uint16 v10 = (R34 & 0xFFF0) - g_word_7E001E;
    if (v10 >= enemy_projectile_y_pos[v9])
      enemy_projectile_y_pos[v9] = v10;
    return 1;
  }
}

void EnemyProj_Init_0x8aaf(uint16 j) {  // 0x868A39
  NextRandom();
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  uint16 v2 = v1->y_pos + 12;
  int v3 = j >> 1;
  enemy_projectile_y_pos[v3] = v2;
  enemy_projectile_timers[v3] = v2 + 72;
  enemy_projectile_y_vel[v3] = v1->ai_var_D;
  enemy_projectile_x_pos[v3] = v1->x_pos + (random_number & 0x1F) - 16;
  enemy_projectile_instr_list_ptr[v3] = off_868A75[(uint16)(HIBYTE(enemy_projectile_y_vel[v3]) & 6) >> 1];
}

void EnemyProj_PreInit_0x8aaf(uint16 k) {  // 0x868A7D
  int v1 = k >> 1;
  R20_ = LOBYTE(enemy_projectile_y_vel[v1]) << 8;
  R18_ = (int8)HIBYTE(enemy_projectile_y_vel[v1]);
  uint16 v3 = enemy_projectile_y_subpos[v1];
  bool v4 = __CFADD__uint16(R20_, v3);
  enemy_projectile_y_subpos[v1] = R20_ + v3;
  uint16 v5 = R18_ + v4 + enemy_projectile_y_pos[v1];
  enemy_projectile_y_pos[v1] = v5;
  if (v5 >= enemy_projectile_timers[v1])
    enemy_projectile_id[v1] = 0;
}

void EnemyProj_Init_0x8bc2_SkreeDownRight(uint16 j) {  // 0x868ACD
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  int v2 = j >> 1;
  enemy_projectile_y_pos[v2] = v1->y_pos;
  enemy_projectile_y_vel[v2] = -769;
  enemy_projectile_x_pos[v2] = v1->x_pos + 6;
  enemy_projectile_x_vel[v2] = 320;
}

void EnemyProj_Init_0x8bd0_SkreeUpRight(uint16 j) {  // 0x868AF1
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  int v2 = j >> 1;
  enemy_projectile_y_pos[v2] = v1->y_pos;
  enemy_projectile_y_vel[v2] = -1025;
  enemy_projectile_x_pos[v2] = v1->x_pos + 6;
  enemy_projectile_x_vel[v2] = 96;
}

void EnemyProj_Init_0x8bde_SkreeDownLeft(uint16 j) {  // 0x868B15
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  int v2 = j >> 1;
  enemy_projectile_y_pos[v2] = v1->y_pos;
  enemy_projectile_y_vel[v2] = -769;
  enemy_projectile_x_pos[v2] = v1->x_pos - 6;
  enemy_projectile_x_vel[v2] = -320;
}

void EnemyProj_Init_0x8bec_SkreeUpLeft(uint16 j) {  // 0x868B39
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  int v2 = j >> 1;
  enemy_projectile_y_pos[v2] = v1->y_pos;
  enemy_projectile_y_vel[v2] = -1025;
  enemy_projectile_x_pos[v2] = v1->x_pos - 6;
  enemy_projectile_x_vel[v2] = -96;
}

void EnemyProj_PreInstr_SkreeParticle(uint16 k) {  // 0x868B5D
  int v1 = k >> 1;
  R20_ = LOBYTE(enemy_projectile_x_vel[v1]) << 8;
  R18_ = (int8)HIBYTE(enemy_projectile_x_vel[v1]);
  uint16 v4 = enemy_projectile_1A27[v1];
  bool v3 = __CFADD__uint16(R20_, v4);
  enemy_projectile_1A27[v1] = R20_ + v4;
  enemy_projectile_x_pos[v1] += R18_ + v3;
  R20_ = LOBYTE(enemy_projectile_y_vel[v1]) << 8;
  R18_ = (int8)HIBYTE(enemy_projectile_y_vel[v1]);
  uint16 v5 = enemy_projectile_y_subpos[v1];
  v3 = __CFADD__uint16(R20_, v5);
  enemy_projectile_y_subpos[v1] = R20_ + v5;
  enemy_projectile_y_pos[v1] += R18_ + v3;
  enemy_projectile_y_vel[v1] += 80;
  if (CheckIfEnemyProjectileIsOffScreen(k))
    enemy_projectile_id[v1] = 0;
}

uint16 EprojInstr_SpawnEnemyDropsWithDraygonsEyeDrops(uint16 k, uint16 j) {  // 0x868C68
  int v1 = k >> 1;
  R18_ = enemy_projectile_x_pos[v1];
  R20_ = enemy_projectile_y_pos[v1];
  SpawnEnemyDrops(addr_kEnemyDef_DE7F, k);
  return j;
}

uint16 EprojInstr_SetPreInstrA(uint16 k, uint16 j) {  // 0x868CF6
  enemy_projectile_pre_instr[k >> 1] = FUNC16(EprojPreInstr_DraygonsTurret_8DFF);
  return j;
}

uint16 EprojInstr_SetPreInstrB(uint16 k, uint16 j) {  // 0x868CFD
  enemy_projectile_pre_instr[k >> 1] = FUNC16(EprojPreInstr_8DCA);
  return j;
}

void EprojInit_DraygonsGunk(uint16 j) {  // 0x868D04
  int v1 = j >> 1;
  enemy_projectile_x_pos[v1] = R18_;
  enemy_projectile_y_pos[v1] = R20_;
  uint16 v2 = enemy_projectile_unk1995;
  g_word_7E97DC[v1] = enemy_projectile_unk1995;
  R18_ = v2;
  R20_ = enemy_projectile_init_param;
  ConvertAngleToXy();
  enemy_projectile_x_vel[v1] = R22_;
  enemy_projectile_E[v1] = R24_;
  enemy_projectile_y_vel[v1] = R26_;
  enemy_projectile_F[v1] = R28_;
  enemy_projectile_gfx_idx[v1] = 1024;
}

void EprojInit_DraygonsWallTurretProjs(uint16 j) {  // 0x868D40
  Eproj_AngleToSamus(j);
  int v1 = j >> 1;
  enemy_projectile_gfx_idx[v1] = 2560;
  enemy_projectile_pre_instr[v1] = FUNC16(nullsub_84);
}

uint16 EprojInstr_868D55(uint16 k, uint16 j) {  // 0x868D55
  enemy_projectile_id[j >> 1] = 0;
  return j;
}

void EprojPowerBombCollision(uint16 k) {  // 0x868D5C
  R18_ = HIBYTE(power_bomb_explosion_radius);
  if (HIBYTE(power_bomb_explosion_radius)) {
    R20_ = (uint16)(R18_ + (HIBYTE(power_bomb_explosion_radius) & 1) + (power_bomb_explosion_radius >> 9)) >> 1;
    int v1 = k >> 1;
    if (abs16(power_bomb_explosion_x_pos - enemy_projectile_x_pos[v1]) < R18_
        && abs16(power_bomb_explosion_y_pos - enemy_projectile_y_pos[v1]) < R20_) {
      enemy_projectile_id[v1] = 0;
      samus_x_speed_divisor = 0;
    }
  }
}

uint16 EprojInstr_868D99(uint16 k, uint16 j) {  // 0x868D99
  EprojPowerBombCollision(k);
  uint16 v2 = samus_x_speed_divisor + 1;
  if (sign16(samus_x_speed_divisor - 5)) {
    ++samus_x_speed_divisor;
    int v3 = k >> 1;
    enemy_projectile_F[v3] = v2;
    enemy_projectile_E[v3] = 256;
    enemy_projectile_pre_instr[v3] = FUNC16(EprojPreInstr_8DCA);
    enemy_projectile_properties[v3] = enemy_projectile_properties[v3] & 0x5FFF | 0x2000;
    samus_invincibility_timer = 0;
    samus_knockback_timer = 0;
  }
  return j;
}

void EprojPreInstr_8DCA(uint16 k) {  // 0x868DCA
  int v1;

  EprojPowerBombCollision(k);
  if (samus_contact_damage_index
      || (v1 = k >> 1,
          enemy_projectile_x_pos[v1] = samus_x_pos,
          enemy_projectile_y_pos[v1] = samus_y_pos + 4 * enemy_projectile_F[v1] - 12,
          --enemy_projectile_E[v1],
          !enemy_projectile_E[v1])) {
    enemy_projectile_id[k >> 1] = 0;
    if ((--samus_x_speed_divisor & 0x8000u) != 0)
      samus_x_speed_divisor = 0;
  }
}

void EprojPreInstr_DraygonsTurret_8DFF(uint16 k) {  // 0x868DFF
  int16 v1;

  EprojPowerBombCollision(k);
  Eproj_FuncE73E_MoveXY(k);
  v1 = Eproj_FuncE722(k);
  if (v1)
    enemy_projectile_id[k >> 1] = 0;
}

void EprojPreInstr_DraygonsGunk_8E0F(uint16 k) {  // 0x868E0F
  uint16 v3;

  EprojPowerBombCollision(k);
  Eproj_FuncE73E_MoveXY(k);
  int v1 = k >> 1;
  uint16 v2 = abs16(samus_x_pos - enemy_projectile_x_pos[v1]);
  if (sign16(v2 - 16) && (v3 = abs16(samus_y_pos - enemy_projectile_y_pos[v1]), sign16(v3 - 20))) {
    enemy_projectile_instr_list_ptr[v1] = 0x8C38;
    enemy_projectile_instr_timers[v1] = 1;
  } else {
    if (Eproj_FuncE722(k))
      enemy_projectile_id[v1] = 0;
  }
}

void EprojInit_8E6C(uint16 j) {  // 0x868E7A
  int v1 = j >> 1;
  enemy_projectile_gfx_idx[v1] = 1008;
  enemy_projectile_x_pos[v1] = R18_;
  enemy_projectile_y_pos[v1] = R20_;
  enemy_projectile_instr_list_ptr[v1] = addr_word_868EDF;
  enemy_projectile_instr_timers[v1] = 1;
}

void EprojPreInstr_8E6C(uint16 k) {  // 0x868E99
  R18_ = enemy_data[0].x_pos - R18_;
  R20_ = enemy_data[0].y_pos - R20_;
  uint16 v1 = (uint8)(64 - CalculateAngleFromXY());
  int v2 = k >> 1;
  g_word_7E97DC[v2] = v1;
  R18_ = v1;
  R20_ = 1;
  ConvertAngleToXy();
  enemy_projectile_x_vel[v2] = R22_;
  enemy_projectile_E[v2] = R24_;
  enemy_projectile_y_vel[v2] = R26_;
  enemy_projectile_F[v2] = R28_;
  Eproj_FuncE73E_MoveXY(k);
}

void EprojInit_CrocomireProjectile(uint16 j) {  // 0x869023
  int v2 = j >> 1;
  enemy_projectile_x_vel[v2] = -512;
  enemy_projectile_y_vel[v2] = 1;
  EnemyData *v3 = gEnemyData(cur_enemy_index);
  enemy_projectile_x_pos[v2] = v3->x_pos - 32;
  enemy_projectile_y_pos[v2] = v3->y_pos - 16;
  enemy_projectile_timers[v2] = 0;
  enemy_projectile_1A27[v2] = 0;
  enemy_projectile_y_subpos[v2] = 0;
  enemy_projectile_gfx_idx[v2] = 2560;
}

static const int16 word_869059[9] = { -16, 0, 32, -16, 0, 32, -16, 0, 32 }; // bug: oob read
#define g_word_869059 ((uint16*)RomPtr(0x869059))
void EprojPreInstr_CrocomireProjectile(uint16 k) {  // 0x86906B
  EnemyProjectileBlockCollisition_Horiz(k);
  enemy_projectile_gfx_idx[0] = 2560;
  enemy_projectile_timers[k >> 1] += enemy_projectile_x_vel[k >> 1];
  R18_ = -64;
  R20_ = g_word_869059[enemy_data[0].ai_preinstr >> 1]; // bug: out of bounds read...
  int v1 = (uint16)(2 * CalculateAngleFromXY()) >> 1;
  R18_ = kSinCosTable8bit_Sext[v1 + 64];
  int v2 = k >> 1;
  enemy_projectile_x_vel[v2] = 4 * R18_;
  R18_ = kSinCosTable8bit_Sext[v1];
  enemy_projectile_y_vel[v2] = 4 * R18_;
  enemy_projectile_pre_instr[v2] = FUNC16(sub_8690B3);
}

void sub_8690B3(uint16 k) {  // 0x8690B3
  if (EnemyProjectileBlockCollisition_Horiz(k) & 1 || EnemyProjectileBlockCollisition_Vertical(k) & 1)
    enemy_projectile_id[k >> 1] = 0;
}

void EprojInit_CrocomireSpikeWallPieces(uint16 j) {  // 0x8690CF
  int v1 = j >> 1;
  enemy_projectile_y_pos[v1] = word_869105[(uint16)(j - 20) >> 1];
  enemy_projectile_x_pos[v1] = 528;
  enemy_projectile_x_vel[v1] = 0;
  enemy_projectile_y_vel[v1] = 0;
  enemy_projectile_E[v1] = 0;
  enemy_projectile_F[v1] = 0;
  enemy_projectile_1A27[v1] = 0;
  enemy_projectile_y_subpos[v1] = 0;
  enemy_projectile_y_vel[v1] = -5;
  enemy_projectile_F[v1] = -30720;
}

static const uint16 CrocomireSpikeWallPieces_Tab2[18] = {  // 0x869115
      0,     0, 0xff0, 0xee0, 0xcc0, 0xaa0, 0x880, 0x660, 0x440, 0x220, 0xff0, 0xee0,
  0xcc0, 0xaa0, 0x880, 0x660, 0x440, 0x220,
};
static const uint16 CrocomireSpikeWallPieces_Tab1[18] = {
       0,      0, 0xff00, 0xee00, 0xcc00, 0xaa00, 0x8800, 0x6600, 0x4400, 0x2200, 0xff00, 0xee00,
  0xcc00, 0xaa00, 0x8800, 0x6600, 0x4400, 0x2200,
};
static const int8 CrocomireSpikeWallPieces_Tab3[36] = {
  0, 0, 0, 0, 4, 0, 4, 0, 3, 0, 3, 0,
  2, 0, 2, 0, 1, 0, 1, 0, 6, 0, 5, 0,
  4, 0, 3, 0, 2, 0, 2, 0, 1, 0, 1, 0,
};

void EprojPreInstr_CrocomireSpikeWallPieces(uint16 k) {
  char v3;
  char v6;
  char v7;
  char v8;

  int v1 = k >> 1;
  uint16 v2 = enemy_projectile_E[v1];
  if (v2 != CrocomireSpikeWallPieces_Tab1[v1]) {
    v2 += CrocomireSpikeWallPieces_Tab2[v1];
    if (v2 >= CrocomireSpikeWallPieces_Tab1[v1])
      v2 = CrocomireSpikeWallPieces_Tab1[v1];
  }
  enemy_projectile_E[v1] = v2;
  v3 = *((uint8 *)enemy_projectile_E + k + 1);
  bool v4 = __CFADD__uint8(*((uint8 *)enemy_projectile_x_vel + k), v3);
  *((uint8 *)enemy_projectile_x_vel + k) += v3;
  uint8 v5 = v4 + *((uint8 *)enemy_projectile_x_vel + k + 1);
  if ((int8)(v5 - CrocomireSpikeWallPieces_Tab3[k]) >= 0)
    v5 = CrocomireSpikeWallPieces_Tab3[k];
  *((uint8 *)enemy_projectile_x_vel + k + 1) = v5;
  v6 = *((uint8 *)enemy_projectile_x_vel + k);
  v4 = __CFADD__uint8(*((uint8 *)enemy_projectile_1A27 + k + 1), v6);
  *((uint8 *)enemy_projectile_1A27 + k + 1) += v6;
  v7 = *((uint8 *)enemy_projectile_x_vel + k + 1);
  bool v9 = v4;
  v4 = __CFADD__uint8(v4, v7);
  v8 = v9 + v7;
  v4 |= __CFADD__uint8(*((uint8 *)enemy_projectile_x_pos + k), v8);
  *((uint8 *)enemy_projectile_x_pos + k) += v8;
  *((uint8 *)enemy_projectile_x_pos + k + 1) += v4;
  uint16 v10 = enemy_projectile_F[v1];
  enemy_projectile_F[v1] = v10 + 12288;
  enemy_projectile_y_vel[v1] += __CFADD__uint16(v10, 12288);
  uint16 v11 = enemy_projectile_y_subpos[v1];
  v4 = __CFADD__uint16(enemy_projectile_F[v1], v11);
  enemy_projectile_y_subpos[v1] = enemy_projectile_F[v1] + v11;
  enemy_projectile_y_pos[v1] += enemy_projectile_y_vel[v1] + v4;
  if (enemy_projectile_y_pos[v1] >= 0xA8u) {
    enemy_projectile_id[v1] = 0;
    if ((k & 2) == 0)
      QueueSfx2_Max6(0x29u);
    int v12 = k >> 1;
    R18_ = enemy_projectile_x_pos[v12];
    R20_ = enemy_projectile_y_pos[v12];
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 0x15u);
    QueueSfx2_Max6(0x25u);
  }
}

uint16 EprojInstr_9270(uint16 v0, uint16 j) {  // 0x869270
  int v1 = v0 >> 1;
  R18_ = enemy_projectile_x_pos[v1];
  R20_ = enemy_projectile_y_pos[v1];
  SpawnEnemyDrops(addr_kEnemyDef_DDBF, v0);
  return j;
}

void EprojInit_CrocomireBridgeCrumbling(uint16 j) {  // 0x869286
  int v1 = j >> 1;
  enemy_projectile_x_pos[v1] = enemy_projectile_init_param;
  enemy_projectile_y_pos[v1] = 187;
  enemy_projectile_y_subpos[v1] = 0;
  enemy_projectile_1A27[v1] = 0;
  enemy_projectile_x_vel[v1] = 0;
  enemy_projectile_y_vel[v1] = (random_number & 0x3F) + 64;
  enemy_projectile_gfx_idx[v1] = 1024;
}

void EprojPreInstr_CrocomireBridgeCrumbling(uint16 k) {  // 0x8692BA
  if (EnemyProjectileBlockCollisition_Vertical(k) & 1)
    enemy_projectile_id[k >> 1] = 0;
  else
    enemy_projectile_y_vel[k >> 1] = (enemy_projectile_y_vel[k >> 1] + 24) & 0x3FFF;
}

uint16 MoveEprojWithVelocity(uint16 k) {  // 0x8692D6
  int v1 = k >> 1;
  uint16 v2 = enemy_projectile_x_vel[v1];
  int carry = *((uint8 *)enemy_projectile_1A27 + k + 1) + (v2 & 0xff);
  *((uint8 *)enemy_projectile_1A27 + k + 1) = carry;
  enemy_projectile_x_pos[v1] += (int8)(v2 >> 8) + (carry >> 8);
  return MoveEprojWithVelocityY(k);
}

uint16 MoveEprojWithVelocityY(uint16 k) {  // 0x8692F3
  int v1 = k >> 1;
  uint16 v2 = enemy_projectile_y_vel[v1];
  int carry = *((uint8 *)enemy_projectile_y_subpos + k + 1) + (v2 & 0xff);
  *((uint8 *)enemy_projectile_y_subpos + k + 1) = carry;
  uint16 result = enemy_projectile_y_pos[v1] + (int8)(v2 >> 8) + (carry >> 8);
  enemy_projectile_y_pos[v1] = result;
  return result;
}

uint16 MoveEprojWithVelocityX(uint16 k) {  // 0x869311
  int v1 = k >> 1;
  uint16 v2 = enemy_projectile_x_vel[v1];
  int carry = *((uint8 *)enemy_projectile_1A27 + k + 1) + (v2 & 0xff);
  *((uint8 *)enemy_projectile_1A27 + k + 1) = carry;
  uint16 result = enemy_projectile_x_pos[v1] + (int8)(v2 >> 8) + (carry >> 8);
  enemy_projectile_x_pos[v1] = result;
  return result;
}

void SetAreaDependentEprojPropertiesEx(uint16 k, uint16 j) {  // 0x86932F
  uint16 v2;
  if (area_index == 2) {
    v2 = *((uint16 *)RomPtr_86(k) + 1);
  } else if (area_index == 5) {
    v2 = *((uint16 *)RomPtr_86(k) + 2);
  } else {
    v2 = *(uint16 *)RomPtr_86(k);
  }
  enemy_projectile_properties[j >> 1] = v2;
}

static const int16 kEprojInit_9634_Xvel[4] = { -0x200, -0x1f0, -0x1bc, -0x16a };
static const int16 kEprojInit_9634_Yvel[4] = { 0, 0x88, 0xfc, 0x16a };

void EprojInit_9634(uint16 j) {  // 0x86934D
  int v1 = j >> 1;
  enemy_projectile_E[v1] = 0;
  enemy_projectile_F[v1] = 0;
  enemy_projectile_x_pos[v1] = enemy_data[0].x_pos - 29;
  enemy_projectile_y_pos[v1] = enemy_data[0].y_pos - 35;
  enemy_projectile_gfx_idx[v1] = 2560;
  int v2 = (uint16)(2 * enemy_data[0].parameter_1) >> 1;
  enemy_projectile_x_vel[v1] = kEprojInit_9634_Xvel[v2];
  enemy_projectile_y_vel[v1] = kEprojInit_9634_Yvel[v2];
}

void EprojPreInstr_9634(uint16 k) {  // 0x869392
  int v1 = k >> 1;
  if (enemy_projectile_E[v1] >= 8u) {
    MoveEprojWithVelocity(k);
    if (EnemyProjectileBlockCollisition_Vertical(k) & 1) {
      enemy_projectile_instr_list_ptr[v1] = addr_off_869574;
      ++enemy_projectile_E[v1];
      enemy_projectile_instr_timers[v1] = 1;
      enemy_projectile_x_vel[v1] = 0;
      enemy_projectile_y_vel[v1] = 0;
      enemy_projectile_gfx_idx[v1] = 2560;
      QueueSfx2_Max6(0x2Bu);
    }
  } else {
    ++enemy_projectile_E[v1];
  }
}

void EprojInit_9642_RidleysFireball(uint16 j) {  // 0x8693CA
  int16 v2;

  int v1 = j >> 1;
  enemy_projectile_F[v1] = enemy_projectile_unk1995;
  if (enemy_projectile_init_param)
    v2 = 25;
  else
    v2 = -25;
  enemy_projectile_x_pos[v1] = enemy_data[0].x_pos + v2;
  enemy_projectile_y_pos[v1] = enemy_data[0].y_pos - 43;
  enemy_projectile_gfx_idx[v1] = 2560;

  Ram7800_Default *v4 = gRam7800_Default(0);
  enemy_projectile_x_vel[v1] = v4->var_19;
  enemy_projectile_y_vel[v1] = v4->var_1A;
  SetAreaDependentEprojPropertiesEx(addr_kRidleysFireball_Tab0, j);
}

void SetAreaDependentEprojProperties(uint16 j) {  // 0x869402
  SetAreaDependentEprojPropertiesEx(addr_kRidleysFireball_Tab0, j);
}

void EprojPreInstr_9642_RidleysFireball(uint16 k) {  // 0x86940E
  uint16 v1;
  if (EnemyProjectileBlockCollisition_Horiz(k) & 1) {
    v1 = -27042;
  } else {
    if (!(EnemyProjectileBlockCollisition_Vertical(k) & 1))
      return;
    v1 = -27056;
  }
  int v2 = k >> 1;
  enemy_projectile_id[v2] = 0;
  if (!enemy_projectile_F[v2]) {
    R18_ = enemy_projectile_x_pos[v2];
    R20_ = enemy_projectile_y_pos[v2];
    SpawnEnemyProjectileWithRoomGfx(v1, 3u);
    QueueSfx2_Max6(0x2Bu);
  }
}

uint16 EprojInstr_DisableCollisionsWithSamus(uint16 k, uint16 j) {  // 0x869475
  enemy_projectile_properties[k >> 1] |= 0x2000u;
  return j;
}

void EprojInit_9660_FireballExplosion(uint16 j) {  // 0x86947F
  int v1 = j >> 1;
  enemy_projectile_gfx_idx[v1] = 2560;
  enemy_projectile_x_pos[v1] = R18_;
  enemy_projectile_y_pos[v1] = R20_;
  enemy_projectile_E[v1] = enemy_projectile_init_param;
  enemy_projectile_F[v1] = 0;
  enemy_projectile_x_vel[v1] = 0;
  enemy_projectile_y_vel[v1] = 0;
}

void EprojInit_9688(uint16 j) {  // 0x8694A0
  uint16 v1 = enemy_projectile_init_param;
  int v2 = j >> 1;
  enemy_projectile_x_vel[v2] = 0;
  enemy_projectile_y_vel[v2] = -3584;
  EprojInit_Common(j, v1, 0x9688u);
}

void EprojInit_9696(uint16 j) {  // 0x8694B4
  uint16 v1 = enemy_projectile_init_param;
  int v2 = j >> 1;
  enemy_projectile_x_vel[v2] = 0;
  enemy_projectile_y_vel[v2] = 3584;
  EprojInit_Common(j, v1, 0x9696);
}

void EprojInit_966C(uint16 j) {  // 0x8694C8
  uint16 v1 = enemy_projectile_init_param;
  int v2 = j >> 1;
  enemy_projectile_x_vel[v2] = 3584;
  enemy_projectile_y_vel[v2] = 0;
  EprojInit_Common(j, v1, 0x966C);
}

void EprojInit_967A(uint16 j) {  // 0x8694DC
  uint16 v1 = enemy_projectile_init_param;
  int v2 = j >> 1;
  enemy_projectile_x_vel[v2] = -3584;
  enemy_projectile_y_vel[v2] = 0;
  EprojInit_Common(j, v1, 0x967Au);
}

void EprojInit_Common(uint16 j, uint16 k, uint16 a) {  // 0x8694EE
  int v3 = j >> 1;
  enemy_projectile_F[v3] = a;
  int v4 = k >> 1;
  enemy_projectile_x_pos[v3] = enemy_projectile_x_pos[v4];
  enemy_projectile_y_pos[v3] = enemy_projectile_y_pos[v4];
  enemy_projectile_E[v3] = enemy_projectile_E[v4];
  enemy_projectile_gfx_idx[v3] = 2560;
  SetAreaDependentEprojProperties(j);
}

void EprojPreInstr_966C(uint16 k) {  // 0x86950D
  MoveEprojWithVelocityX(k);
  if (EnemyProjectileBlockCollisition_Vertical(k) & 1) {
    int v1 = k >> 1;
    enemy_projectile_instr_list_ptr[v1] = addr_off_869574;
    enemy_projectile_instr_timers[v1] = 1;
  }
}

void EprojPreInstr_9688(uint16 k) {  // 0x869522
  MoveEprojWithVelocityY(k);
  if (EnemyProjectileBlockCollisition_Horiz(k) & 1) {
    int v1 = k >> 1;
    enemy_projectile_instr_list_ptr[v1] = addr_off_869574;
    enemy_projectile_instr_timers[v1] = 1;
  }
}

void EprojPreInstr_96A4(uint16 k) {  // 0x869537
  if (EnemyProjectileBlockCollisition_Horiz(k) & 1)
    enemy_projectile_id[k >> 1] = 0;
}

void EprojPreInstr_96C0(uint16 k) {  // 0x869540
  if (EnemyProjectileBlockCollisition_Vertical(k) & 1)
    enemy_projectile_id[k >> 1] = 0;
}

void EprojPreInstr_96CE(uint16 k) {  // 0x869549
  if (EnemyProjectileBlockCollisition_Vertical(k) & 1)
    enemy_projectile_id[k >> 1] = 0;
}

uint16 EprojInstr_95BA(uint16 k, uint16 j) {  // 0x8695BA
  enemy_projectile_unk1995 = 0;
  SpawnEnemyProjectileWithRoomGfx(addr_stru_86966C, k);
  enemy_projectile_unk1995 = 0;
  SpawnEnemyProjectileWithRoomGfx(addr_stru_86967A, k);
  return j;
}

uint16 EprojInstr_95ED(uint16 k, uint16 j) {  // 0x8695ED
  enemy_projectile_unk1995 = 0;
  SpawnEnemyProjectileWithRoomGfx(addr_stru_869688, k);
  enemy_projectile_unk1995 = 0;
  SpawnEnemyProjectileWithRoomGfx(addr_stru_869696, k);
  return j;
}

uint16 EprojInstr_9620(uint16 k, uint16 j) {  // 0x869620
  if ((int8)-- * ((uint8 *)enemy_projectile_E + k) >= 0)
    SpawnEnemyProjectileWithRoomGfx(enemy_projectile_F[k >> 1], k);
  return j;
}

void EprojInit_9734_CeresFallingDebris(uint16 j) {  // 0x8696DC
  int v1 = j >> 1;
  enemy_projectile_E[v1] = 0;
  enemy_projectile_F[v1] = 0;
  enemy_projectile_x_vel[v1] = 0;
  enemy_projectile_gfx_idx[v1] = 3584;
  enemy_projectile_x_pos[v1] = enemy_projectile_init_param;
  enemy_projectile_y_pos[v1] = 42;
  enemy_projectile_y_vel[v1] = 16;
}

void EprojPreInstr_9734_CeresFallingDebris(uint16 k) {  // 0x869701
  int v1 = k >> 1;
  enemy_projectile_y_vel[v1] += 16;
  if (EnemyProjectileBlockCollisition_Vertical(k) & 1) {
    enemy_projectile_id[v1] = 0;
    R18_ = enemy_projectile_x_pos[v1];
    R20_ = enemy_projectile_y_pos[v1];
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 9u);
    QueueSfx2_Max6(0x6Du);
  }
}

uint16 EprojInstr_980E(uint16 k, uint16 j) {  // 0x86980E
  int v2 = k >> 1;
  R18_ = enemy_projectile_x_pos[v2];
  R20_ = enemy_projectile_y_pos[v2];
  SpawnEnemyDrops(addr_kEnemyDef_E4FF, k);
  return j;
}

static const uint8 byte_8698B4[16] = { 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0xf0, 0xe0, 0xd0, 0xc0, 0xb0, 0xa0, 0x90, 0x80 };
static const uint8 byte_8698F7[9] = { 0x30, 0x44, 0x58, 0x6c, 0x80, 0x94, 0xa8, 0xbc, 0xd0 };
static const uint8 byte_869979[8] = { 0, 0x20, 0x40, 0x60, 0x80, 0xa0, 0xc0, 0xe0 };


void EprojInit_PhantoonDestroyableFireballs(uint16 j) {  // 0x869824
  char v1;

  v1 = (uint16)(enemy_projectile_init_param & 0xFF00) >> 8;
  if (v1) {
    switch (v1) {
    case 2: {
      int v3 = j >> 1;
      enemy_projectile_1A27[v3] = 0;
      enemy_projectile_y_subpos[v3] = 0;
      enemy_projectile_y_vel[v3] = 0;
      uint16 v4 = (uint8)enemy_projectile_init_param;
      if ((int16)((uint8)enemy_projectile_init_param - 8) >= 0)
        enemy_projectile_x_vel[v3] = -2;
      else
        enemy_projectile_x_vel[v3] = 2;
      enemy_projectile_E[v3] = byte_8698B4[v4];
      enemy_projectile_x_pos[v3] = enemy_data[0].x_pos;
      enemy_projectile_y_pos[v3] = enemy_data[0].y_pos + 32;
      enemy_projectile_pre_instr[v3] = FUNC16(EprojPreInstr_PhantoonDestroyableFireballs);
      break;
    }
    case 4: {
      int v5 = j >> 1;
      enemy_projectile_1A27[v5] = 0;
      enemy_projectile_y_subpos[v5] = 0;
      enemy_projectile_y_vel[v5] = 0;
      uint16 v6 = enemy_projectile_init_param & 0xF;
      enemy_projectile_x_vel[v5] = (uint8)(enemy_projectile_init_param & 0xF0) >> 1;
      enemy_projectile_x_pos[v5] = byte_8698F7[v6];
      enemy_projectile_y_pos[v5] = 40;
      enemy_projectile_pre_instr[v5] = FUNC16(EprojPreInstr_PhantoonDestroyableFireballs_2);
      break;
    }
    case 6: {
      int v7 = j >> 1;
      enemy_projectile_1A27[v7] = 0;
      enemy_projectile_y_subpos[v7] = 0;
      enemy_projectile_y_vel[v7] = 0;
      enemy_projectile_x_vel[v7] = 128;
      enemy_projectile_E[v7] = byte_869979[(uint8)enemy_projectile_init_param];
      enemy_projectile_x_pos[v7] = enemy_data[0].x_pos;
      enemy_projectile_y_pos[v7] = enemy_data[0].y_pos + 16;
      enemy_projectile_pre_instr[v7] = FUNC16(EprojPreInstr_PhantoonDestroyableFireballs_3);
      break;
    }
    default:
      Unreachable();
      while (1)
        ;
    }
  } else {
    int v2 = j >> 1;
    enemy_projectile_1A27[v2] = 0;
    enemy_projectile_y_subpos[v2] = 0;
    enemy_projectile_x_vel[v2] = 0;
    enemy_projectile_y_vel[v2] = 0;
    enemy_projectile_x_pos[v2] = enemy_data[0].x_pos;
    enemy_projectile_y_pos[v2] = enemy_data[0].y_pos + 32;
    enemy_projectile_instr_list_ptr[v2] = addr_off_8697B4;
    enemy_projectile_properties[v2] = enemy_projectile_properties[v2] & 0xFFF | 0x2000;
  }
}

void EprojInit_PhantoonStartingFireballs(uint16 j) {  // 0x86993A
  int v1 = j >> 1;
  enemy_projectile_1A27[v1] = 0;
  enemy_projectile_y_subpos[v1] = 0;
  enemy_projectile_x_vel[v1] = 0;
  enemy_projectile_y_vel[v1] = 0;
  uint16 v2 = byte_869979[enemy_projectile_init_param];
  enemy_projectile_E[v1] = v2;
  Eproj_PhantomFireballs_Func1(v2, 0x30u);
  enemy_projectile_x_pos[v1] = R20_ + enemy_data[0].x_pos;
  enemy_projectile_y_pos[v1] = R22_ + enemy_data[0].y_pos + 16;
}

void EprojPreInstr_PhantoonStartingFireballs(uint16 k) {  // 0x869981
  int v1 = k >> 1;
  enemy_projectile_y_vel[v1] += 16;
  if (EnemyProjectileBlockCollisition_Vertical(k) & 1) {
    enemy_projectile_properties[v1] = enemy_projectile_properties[v1] & 0xFFF | 0x8000;
    enemy_projectile_pre_instr[v1] = FUNC16(EprojPreInstr_PhantoonStartingFireballs2);
    enemy_projectile_instr_list_ptr[v1] = addr_word_86976C;
    enemy_projectile_instr_timers[v1] = 1;
    enemy_projectile_E[v1] = 8;
    enemy_projectile_y_pos[v1] += 8;
  }
}

void EprojPreInstr_PhantoonStartingFireballs2(uint16 k) {  // 0x8699BF
  int v1 = k >> 1;
  bool v2 = enemy_projectile_E[v1] == 1;
  bool v3 = (--enemy_projectile_E[v1] & 0x8000u) != 0;
  if (v2 || v3) {
    enemy_projectile_pre_instr[v1] = FUNC16(EprojPreInstr_PhantoonStartingFireballs3);
    enemy_projectile_instr_list_ptr[v1] = addr_word_869772;
    enemy_projectile_instr_timers[v1] = 1;
    enemy_projectile_y_pos[v1] -= 8;
    enemy_projectile_y_vel[v1] = -768;
    enemy_projectile_E[v1] = 0;
    if ((nmi_frame_counter_word & 1) != 0)
      enemy_projectile_x_vel[v1] = -128;
    else
      enemy_projectile_x_vel[v1] = 128;
  }
}

static const uint16 word_869A3E[3] = { 0xfd00, 0xfe00, 0xff00 };

void EprojPreInstr_PhantoonStartingFireballs3(uint16 k) {  // 0x869A01
  int v1 = k >> 1;
  enemy_projectile_y_vel[v1] += 16;
  if (EnemyProjectileBlockCollisition_Vertical(k) & 1) {
    uint16 v2 = enemy_projectile_E[v1] + 1;
    enemy_projectile_E[v1] = v2;
    if (sign16(v2 - 3)) {
      enemy_projectile_y_vel[v1] = word_869A3E[v2];
      return;
    }
    goto LABEL_6;
  }
  if (EnemyProjectileBlockCollisition_Horiz(k) & 1) {
LABEL_6:
    enemy_projectile_instr_list_ptr[v1] = addr_word_869782;
    enemy_projectile_instr_timers[v1] = 1;
    enemy_projectile_pre_instr[v1] = FUNC16(nullsub_86);
  }
}

void EprojPreInstr_PhantoonDestroyableFireballs(uint16 k) {  // 0x869A45
  int16 v4;
  uint16 v5;
  int v1 = k >> 1;
  enemy_projectile_y_vel[v1] += 4;
  uint16 v2 = (uint8)(LOBYTE(enemy_projectile_x_vel[v1]) + enemy_projectile_E[v1]);
  enemy_projectile_E[v1] = v2;
  Eproj_PhantomFireballs_Func1(v2, enemy_projectile_y_vel[v1]);
  bool v3 = (int16)(R20_ + enemy_data[0].x_pos) < 0;
  v4 = R20_ + enemy_data[0].x_pos;
  enemy_projectile_x_pos[v1] = R20_ + enemy_data[0].x_pos;
  if (v3
      || !sign16(v4 - 256)
      || (v3 = (int16)(R22_ + enemy_data[0].y_pos + 16) < 0,
          v5 = R22_ + enemy_data[0].y_pos + 16,
          enemy_projectile_y_pos[v1] = v5,
          v3)
      || !sign16(v5 - 256)) {
    enemy_projectile_instr_list_ptr[v1] = addr_off_8697F8;
    enemy_projectile_instr_timers[v1] = 1;
  }
}

void EprojPreInstr_PhantoonDestroyableFireballs_2(uint16 k) {  // 0x869A94
  int v1 = k >> 1;
  if (!enemy_projectile_x_vel[v1])
    goto LABEL_5;
  bool v2, v3;
  v2 = enemy_projectile_x_vel[v1] == 1;
  v3 = (--enemy_projectile_x_vel[v1] & 0x8000u) != 0;
  if (v2 || v3) {
    QueueSfx3_Max6(0x1Du);
LABEL_5:
    enemy_projectile_y_vel[v1] += 16;
    if (EnemyProjectileBlockCollisition_Vertical(k) & 1) {
      enemy_projectile_instr_list_ptr[v1] = addr_word_8697AC;
      enemy_projectile_instr_timers[v1] = 1;
      enemy_projectile_y_pos[v1] += 8;
      enemy_projectile_pre_instr[v1] = FUNC16(nullsub_86);
      QueueSfx3_Max6(0x1Du);
    }
  }
}

void EprojPreInstr_PhantoonDestroyableFireballs_3(uint16 k) {  // 0x869ADA
  int16 v4;
  uint16 v5;
  int v1 = k >> 1;
  enemy_projectile_y_vel[v1] += 2;
  uint16 v2 = (uint8)(enemy_projectile_E[v1] + 2);
  enemy_projectile_E[v1] = v2;
  Eproj_PhantomFireballs_Func1(v2, enemy_projectile_y_vel[v1]);
  bool v3 = (int16)(R20_ + enemy_data[0].x_pos) < 0;
  v4 = R20_ + enemy_data[0].x_pos;
  enemy_projectile_x_pos[v1] = R20_ + enemy_data[0].x_pos;
  if (v3
      || !sign16(v4 - 256)
      || (v3 = (int16)(R22_ + enemy_data[0].y_pos + 16) < 0,
          v5 = R22_ + enemy_data[0].y_pos + 16,
          enemy_projectile_y_pos[v1] = v5,
          v3)
      || !sign16(v5 - 256)) {
    enemy_projectile_instr_list_ptr[v1] = addr_off_8697F8;
    enemy_projectile_instr_timers[v1] = 1;
  }
}

void EprojPreInstr_PhantoonStartingFireballsB(uint16 k) {  // 0x869B29
  if (enemy_data[0].ai_var_B) {
    int v1 = k >> 1;
    enemy_projectile_pre_instr[v1] = FUNC16(EprojPreInstr_PhantoonStartingFireballsB_2);
    enemy_projectile_x_vel[v1] = 180;
    enemy_projectile_y_vel[v1] = 48;
  }
}

void EprojPreInstr_PhantoonStartingFireballsB_2(uint16 k) {  // 0x869B41
  int v1 = k >> 1;
  uint16 v2 = enemy_projectile_x_vel[v1];
  if (v2) {
    enemy_projectile_x_vel[v1] = v2 - 1;
LABEL_5:;
    uint8 v4 = enemy_projectile_E[v1] + 1;
    enemy_projectile_E[v1] = v4;
    Eproj_PhantomFireballs_Func1(v4, enemy_projectile_y_vel[v1]);
    enemy_projectile_x_pos[v1] = R20_ + enemy_data[0].x_pos;
    enemy_projectile_y_pos[v1] = R22_ + enemy_data[0].y_pos + 16;
    return;
  }
  if ((nmi_frame_counter_word & 1) == 0)
    goto LABEL_5;
  uint16 v3 = enemy_projectile_y_vel[v1] - 1;
  enemy_projectile_y_vel[v1] = v3;
  if (v3)
    goto LABEL_5;
  enemy_projectile_x_pos[v1] = enemy_data[0].x_pos;
  enemy_projectile_y_pos[v1] = enemy_data[0].y_pos + 16;
  enemy_projectile_instr_timers[v1] = 1;
  enemy_projectile_instr_list_ptr[v1] = addr_off_8697F8;
}

void Eproj_PhantomFireballs_Func1(uint16 j, uint16 a) {  // 0x869BA2
  int16 v3;
  uint16 v2, v4;

  R24_ = a;
  R26_ = j;
  if (sign16(j - 128))
    v2 = Eproj_PhantomFireballs_Func2(2 * j);
  else
    v2 = -Eproj_PhantomFireballs_Func2(2 * (uint8)(j + 0x80));
  R20_ = v2;
  v3 = (uint8)(R26_ - 64);
  if (sign16(v3 - 128))
    v4 = Eproj_PhantomFireballs_Func2(2 * v3);
  else
    v4 = -Eproj_PhantomFireballs_Func2(2 * (uint8)(v3 + 0x80));
  R22_ = v4;
}


uint16 Eproj_PhantomFireballs_Func2(uint16 k) {  // 0x869BF3
  R18_ = Mult8x8(*((uint8 *)&kSinCosTable8bit_Sext[64] + k), R24_) >> 8;
  return R18_ + Mult8x8(*((uint8 *)&kSinCosTable8bit_Sext[64] + k + 1), R24_);
}

void EprojInit_RocksKraidSpits(uint16 j) {  // 0x869CA3
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  int v2 = j >> 1;
  enemy_projectile_x_pos[v2] = v1->x_pos + 16;
  enemy_projectile_y_pos[v2] = v1->y_pos - 96;
  enemy_projectile_y_subpos[v2] = 0;
  enemy_projectile_1A27[v2] = 0;
  enemy_projectile_x_vel[v2] = enemy_projectile_init_param;
  enemy_projectile_y_vel[v2] = -1024;
  enemy_projectile_gfx_idx[v2] = 1536;
}

void EprojInit_RocksFallingKraidCeiling(uint16 j) {  // 0x869CD8
  int v1 = j >> 1;
  enemy_projectile_x_pos[v1] = enemy_projectile_init_param;
  enemy_projectile_y_pos[v1] = 312;
  enemy_projectile_y_subpos[v1] = 0;
  enemy_projectile_1A27[v1] = 0;
  enemy_projectile_x_vel[v1] = 0;
  enemy_projectile_y_vel[v1] = (random_number & 0x3F) + 64;
  enemy_projectile_gfx_idx[v1] = 1536;
}

void EprojInit_RocksWhenKraidRises(uint16 j) {  // 0x869D0C
  int16 v1;

  v1 = random_number & 0x3F;
  if ((random_number & 1) == 0)
    v1 = ~v1;
  int v2 = j >> 1;
  enemy_projectile_x_pos[v2] = gEnemyData(cur_enemy_index)->x_pos + v1;
  enemy_projectile_y_pos[v2] = 432;
  enemy_projectile_y_subpos[v2] = 0;
  enemy_projectile_1A27[v2] = 0;
  enemy_projectile_x_vel[v2] = enemy_projectile_init_param;
  enemy_projectile_y_vel[v2] = -1280;
  enemy_projectile_gfx_idx[v2] = 1536;
  QueueSfx3_Max6(0x1Eu);
}

void EprojPreInstr_KraidRocks(uint16 k) {  // 0x869D56
  if (EnemyProjectileBlockCollisition_Horiz(k) & 1 || EnemyProjectileBlockCollisition_Vertical(k) & 1) {
    enemy_projectile_id[k >> 1] = 0;
  } else {
    int v1 = k >> 1;
    uint16 v2 = enemy_projectile_x_vel[v1] + 8;
    enemy_projectile_x_vel[v1] = v2;
    if (!sign16(v2 - 256))
      v2 = -256;
    enemy_projectile_x_vel[v1] = v2;
    enemy_projectile_y_vel[v1] += 64;
  }
}

void EprojPreInstr_RocksFallingKraidCeiling(uint16 k) {  // 0x869D89
  if (EnemyProjectileBlockCollisition_Vertical(k) & 1)
    enemy_projectile_id[k >> 1] = 0;
  else
    enemy_projectile_y_vel[k >> 1] = (enemy_projectile_y_vel[k >> 1] + 24) & 0x3FFF;
}

void sub_869DA5(uint16 k) {  // 0x869DA5
  enemy_projectile_gfx_idx[k >> 1] = 0;
}

void EprojInit_MiniKraidSpit(uint16 j) {  // 0x869DEC
  ExtraEnemyRam7800 *v2;

  EnemyData *v1 = gEnemyData(cur_enemy_index);
  v2 = gExtraEnemyRam7800(cur_enemy_index);
  int v3 = j >> 1;
  enemy_projectile_x_pos[v3] = v2->kraid.field_4 + v1->x_pos;
  enemy_projectile_y_pos[v3] = v1->y_pos - 16;
  enemy_projectile_y_subpos[v3] = 0;
  enemy_projectile_1A27[v3] = 0;
  enemy_projectile_x_vel[v3] = v2->kraid.kraid_next;
  enemy_projectile_y_vel[v3] = v2->kraid.field_2;
}

void EprojPreInit_MiniKraidSpit(uint16 k) {  // 0x869E1E
  int16 v2;

  if (EnemyProjectileBlockCollisition_Horiz(k) & 1 || EnemyProjectileBlockCollisition_Vertical(k) & 1) {
    enemy_projectile_id[k >> 1] = 0;
  } else {
    int v1 = k >> 1;
    v2 = enemy_projectile_y_vel[v1] + 64;
    if (v2 >= 0 && !sign16(enemy_projectile_y_vel[v1] - 960))
      v2 = 1024;
    enemy_projectile_y_vel[v1] = v2;
  }
}

void EprojInit_MiniKraidSpikesLeft(uint16 j) {  // 0x869E46
  EprojInit_MiniKraidSpikes(j, -0x200);
}

void EprojInit_MiniKraidSpikesRight(uint16 j) {  // 0x869E4B
  EprojInit_MiniKraidSpikes(j, 0x200);
}
static const int16 kEprojInit_MiniKraidSpikes_Tab0[3] = { -2, 12, 24 };
void EprojInit_MiniKraidSpikes(uint16 j, uint16 a) {  // 0x869E4E
  int v2 = j >> 1;
  enemy_projectile_x_vel[v2] = a;
  R18_ = kEprojInit_MiniKraidSpikes_Tab0[gExtraEnemyRam7800(cur_enemy_index)->kraid.kraid_healths_8ths[0] >> 1];
  EnemyData *v3 = gEnemyData(cur_enemy_index);
  enemy_projectile_x_pos[v2] = v3->x_pos;
  enemy_projectile_y_pos[v2] = R18_ + v3->y_pos;
  enemy_projectile_y_subpos[v2] = 0;
  enemy_projectile_1A27[v2] = 0;
  enemy_projectile_y_vel[v2] = 0;
}

void EprojPreInstr_MiniKraidSpikes(uint16 k) {  // 0x869E83
  if (EnemyProjectileBlockCollisition_Horiz(k) & 1)
    enemy_projectile_id[k >> 1] = 0;
}

void EprojInit_WalkingLavaSeahorseFireball(uint16 j) {  // 0x869EB2
  static const int16 word_869EF9[3] = { -0x100, 0, 0x100 };

  EnemyData *v1 = gEnemyData(cur_enemy_index);
  int v2 = j >> 1;
  enemy_projectile_y_pos[v2] = v1->y_pos - 12;
  enemy_projectile_x_vel[v2] = -1024;
  enemy_projectile_x_pos[v2] = v1->x_pos - 16;
  if ((v1->ai_var_D & 0x8000u) == 0) {
    enemy_projectile_x_vel[v2] = 1024;
    enemy_projectile_x_pos[v2] = v1->x_pos + 16;
  }
  enemy_projectile_y_vel[v2] = word_869EF9[enemy_projectile_init_param >> 1];
  enemy_projectile_y_subpos[v2] = 0;
  enemy_projectile_1A27[v2] = 0;
}

void EprojPreInstr_WalkingLavaSeahorseFireball(uint16 k) {  // 0x869EFF
  uint16 v2;
  if (EnemyProjectileBlockCollisition_Vertical(k) & 1 || EnemyProjectileBlockCollisition_Horiz(k) & 1) {
    enemy_projectile_id[k >> 1] = 0;
  } else {
    int v1 = k >> 1;
    if ((enemy_projectile_x_vel[v1] & 0x8000u) == 0) {
      v2 = enemy_projectile_x_vel[v1] - 64;
      enemy_projectile_x_vel[v1] = v2;
      if (sign16(v2 - 512))
        v2 = 512;
    } else {
      v2 = enemy_projectile_x_vel[v1] + 64;
      enemy_projectile_x_vel[v1] = v2;
      if (!sign16(v2 + 512))
        v2 = -512;
    }
    enemy_projectile_x_vel[v1] = v2;
  }
}

void EprojInit_PirateMotherBrainLaser(uint16 j) {  // 0x86A009
  uint16 v1 = addr_word_869F41;
  if (R22_)
    v1 = addr_word_869F7D;
  int v2 = j >> 1;
  enemy_projectile_instr_list_ptr[v2] = v1;
  enemy_projectile_pre_instr[v2] = FUNC16(nullsub_87);
  enemy_projectile_x_pos[v2] = R18_;
  enemy_projectile_y_pos[v2] = R20_;
  EnemyData *v3 = gEnemyData(cur_enemy_index);
  enemy_projectile_properties[v2] = *((uint16 *)RomPtr_A0(v3->enemy_ptr) + 3) | 0x1000;
  enemy_projectile_E[v2] = gEnemyData(cur_enemy_index)->parameter_1;
  QueueSfx2_Max6(0x67u);
}

uint16 EprojInstr_SetPreInstrAndRun(uint16 k, uint16 j) {  // 0x86A050
  enemy_projectile_pre_instr[k >> 1] = *(uint16 *)RomPtr_86(j);
  return j;
}

void EprojPreInstr_PirateMotherBrainLaser_MoveLeft(uint16 k) {  // 0x86A05C
  int v1 = k >> 1;
  --enemy_projectile_x_pos[v1];
  --enemy_projectile_x_pos[v1];
  if ((enemy_projectile_E[v1] & 0x8000u) == 0) {
    --enemy_projectile_x_pos[v1];
    --enemy_projectile_x_pos[v1];
  }
  if (CheckIfEnemyProjectileIsOffScreen(k))
    enemy_projectile_id[v1] = 0;
}

void EprojPreInstr_PirateMotherBrainLaser_MoveRight(uint16 k) {  // 0x86A07A
  int v1 = k >> 1;
  ++enemy_projectile_x_pos[v1];
  ++enemy_projectile_x_pos[v1];
  if ((enemy_projectile_E[v1] & 0x8000u) == 0) {
    ++enemy_projectile_x_pos[v1];
    ++enemy_projectile_x_pos[v1];
  }
  if (CheckIfEnemyProjectileIsOffScreen(k))
    enemy_projectile_id[v1] = 0;
}

void EprojInit_PirateClaw(uint16 j) {  // 0x86A098
  int v1 = j >> 1;
  enemy_projectile_y_pos[v1] = R24_ + R20_;
  enemy_projectile_x_pos[v1] = R22_ + R18_;
  uint16 v2 = addr_off_869FB9;
  if (enemy_projectile_init_param)
    v2 = addr_off_869FE1;
  enemy_projectile_instr_list_ptr[v1] = v2;
  enemy_projectile_pre_instr[v1] = FUNC16(nullsub_87);
  enemy_projectile_E[v1] = 2048;
  enemy_projectile_F[v1] = 1;
}

void EprojPreInstr_PirateClawThrownLeft(uint16 k) {  // 0x86A0D1
  int v1 = k >> 1;
  if (enemy_projectile_F[v1]) {
    R18_ = HIBYTE(enemy_projectile_E[v1]);
    enemy_projectile_x_pos[v1] -= R18_;
    uint16 v2 = enemy_projectile_E[v1] - 32;
    enemy_projectile_E[v1] = v2;
    if (!v2)
      enemy_projectile_F[v1] = 0;
  } else {
    enemy_projectile_x_pos[v1] += HIBYTE(enemy_projectile_E[v1]);
    enemy_projectile_E[v1] += 32;
  }
  ++enemy_projectile_y_pos[v1];
  if (CheckIfEnemyProjectileIsOffScreen(k))
    enemy_projectile_id[v1] = 0;
}

void EprojPreInstr_PirateClawThrownRight(uint16 k) {  // 0x86A124
  int v1 = k >> 1;
  if (enemy_projectile_F[v1]) {
    R18_ = HIBYTE(enemy_projectile_E[v1]);
    enemy_projectile_x_pos[v1] += R18_;
    uint16 v2 = enemy_projectile_E[v1] - 32;
    enemy_projectile_E[v1] = v2;
    if (!v2)
      enemy_projectile_F[v1] = 0;
  } else {
    R18_ = HIBYTE(enemy_projectile_E[v1]);
    enemy_projectile_x_pos[v1] -= R18_;
    enemy_projectile_E[v1] += 32;
  }
  ++enemy_projectile_y_pos[v1];
  if (CheckIfEnemyProjectileIsOffScreen(k))
    enemy_projectile_id[v1] = 0;
}

static const int16 word_86A2D6[6] = { 64, 72, 80, -64, -72, -80 };
#define off_86A2E2 ((uint16*)RomPtr(0x86a2e2))

void EprojInit_A379(uint16 j) {  // 0x86A2A1
  int v1 = j >> 1;
  enemy_projectile_1A27[v1] = 0;
  enemy_projectile_y_subpos[v1] = 0;
  enemy_projectile_x_vel[v1] = 0;
  enemy_projectile_y_vel[v1] = 0;
  uint16 v2 = enemy_projectile_init_param;
  int v3 = enemy_projectile_init_param >> 1;
  enemy_projectile_x_pos[v1] = word_86A2D6[v3] + samus_x_pos;
  enemy_projectile_y_pos[v1] = samus_y_pos + 80;
  enemy_projectile_instr_list_ptr[v1] = off_86A2E2[v3];
  enemy_projectile_E[v1] = v2;
}

void EprojInit_CeresElevatorPad(uint16 j) {  // 0x86A2EE
  int v1 = j >> 1;
  enemy_projectile_y_pos[v1] = samus_y_pos + 28;
  enemy_projectile_E[v1] = 60;
  sub_86A301(j);
}

void sub_86A301(uint16 j) {  // 0x86A301
  int v1 = j >> 1;
  enemy_projectile_1A27[v1] = 0;
  enemy_projectile_y_subpos[v1] = 0;
  enemy_projectile_x_vel[v1] = 0;
  enemy_projectile_y_vel[v1] = 0;
  enemy_projectile_gfx_idx[v1] = 0;
  enemy_projectile_x_pos[v1] = samus_x_pos;
}


void EprojInit_CeresElevatorPlatform(uint16 j) {  // 0x86A31B
  enemy_projectile_y_pos[j >> 1] = 97;
  sub_86A301(j);
}


void EprojPreInstr_CeresElevatorPad(uint16 k) {  // 0x86A328
  bool v2; // zf
  bool v3; // sf

  int v1 = k >> 1;
  if (!enemy_projectile_E[v1]
      || (v2 = enemy_projectile_E[v1] == 1, v3 = (int16)(enemy_projectile_E[v1] - 1) < 0, --enemy_projectile_E[v1], v2)
      || v3) {
    enemy_projectile_y_pos[v1] = samus_y_pos + 28;
    if (!sign16(++samus_y_pos - 73)) {
      samus_y_pos = 72;
      enemy_projectile_instr_timers[v1] = 1;
      enemy_projectile_instr_list_ptr[v1] = addr_off_86A28B;
      CallSomeSamusCode(0xEu);
    }
  }
}

void EprojPreInstr_CeresElevatorPlatform(uint16 k) {  // 0x86A364
  if (samus_y_pos == 72) {
    int v1 = k >> 1;
    enemy_projectile_instr_timers[v1] = 1;
    enemy_projectile_instr_list_ptr[v1] = addr_off_86A28B;
  }
}

void EprojPreInstr_PrePhantomRoom(uint16 j) {  // 0x86A3A3
  bg2_y_scroll = 0;
}

uint16 EprojInstr_A3BE(uint16 k, uint16 j) {  // 0x86A3BE
  int v1 = k >> 1;
  enemy_projectile_x_pos[v1] = enemy_projectile_E[v1];
  enemy_projectile_y_pos[v1] = enemy_projectile_F[v1];
  return j;
}

uint16 EprojInstr_GotoWithProbability25(uint16 k, uint16 j) {  // 0x86A456
  if ((NextRandom() & 0xC000) == 0xC000)
    return *(uint16 *)RomPtr_86(j);
  else
    return j + 2;
}

void EprojInit_BombTorizoLowHealthDrool(uint16 j) {  // 0x86A5D3
  int16 parameter_1; // dx
  uint16 v4, v5, v8;
  int v1 = j >> 1;
  enemy_projectile_gfx_idx[v1] = 0;
  enemy_projectile_instr_list_ptr[v1] = off_86A64D[(uint16)((NextRandom() >> 1) & 0xE) >> 1];
  NextRandom();
  EnemyData *v2 = gEnemyData(cur_enemy_index);
  enemy_projectile_y_pos[v1] = v2->y_pos - 5;
  parameter_1 = v2->parameter_1;
  if ((parameter_1 & 0x4000) != 0) {
    v4 = random_number & 0x1FE;
  } else {
    if (parameter_1 < 0)
      v5 = 32;
    else
      v5 = 224;
    R18_ = v5;
    v4 = 2 * (v5 + (random_number & 0xF) - 8);
  }
  int v6 = v4 >> 1;
  enemy_projectile_x_vel[v1] = kSinCosTable8bit_Sext[v6 + 64];
  enemy_projectile_y_vel[v1] = kSinCosTable8bit_Sext[v6];
  EnemyData *v7 = gEnemyData(cur_enemy_index);
  if ((v7->parameter_1 & 0x8000u) != 0)
    v8 = v7->x_pos + 8;
  else
    v8 = v7->x_pos - 8;
  enemy_projectile_x_pos[v1] = v8;
}

void EprojInit_BombTorizoLowHealthInitialDrool(uint16 j) {  // 0x86A65D
  int16 v3;
  int16 parameter_1; // dx

  int v1 = j >> 1;
  enemy_projectile_gfx_idx[v1] = 0;
  NextRandom();
  EnemyData *v2 = gEnemyData(cur_enemy_index);
  enemy_projectile_y_pos[v1] = v2->y_pos + (random_number & 3) - 5;
  enemy_projectile_y_vel[v1] = (random_number & 0x1F) + 48;
  NextRandom();
  v3 = random_number & 3;
  parameter_1 = v2->parameter_1;
  if ((parameter_1 & 0x4000) != 0) {
    enemy_projectile_x_pos[v1] = v2->x_pos + v3;
    enemy_projectile_x_vel[v1] = 0;
  } else {
    if (parameter_1 < 0)
      enemy_projectile_x_pos[v1] = v2->x_pos + v3 + 8;
    else
      enemy_projectile_x_pos[v1] = v2->x_pos + v3 - 8;
    enemy_projectile_x_vel[v1] = 0;
  }
}

void EprojInit_A977(uint16 j) {  // 0x86A6C7
  int v1 = j >> 1;
  enemy_projectile_gfx_idx[v1] = 0;
  CalculatePlmBlockCoords(plm_id);
  enemy_projectile_x_pos[v1] = 16 * plm_x_block;
  enemy_projectile_y_pos[v1] = 16 * plm_y_block - 4;
  enemy_projectile_1A27[v1] = 0;
  enemy_projectile_y_subpos[v1] = 0;
}

static const int16 kEprojInit_BombTorizoExplosiveSwipe_Tab0[11] = { -30, -40, -47, -31, -21, -1, -28, -43, -48, -31, -21 };
static const int16 kEprojInit_BombTorizoExplosiveSwipe_Tab1[11] = { -52, -28, -11, 9, 21, 20, -52, -27, -10, 9, 20 };
#define kEprojInit_BombTorizoStatueBreaking_InstrList ((uint16*)RomPtr(0x86a7ab))
static const int16 kEprojInit_BombTorizoStatueBreaking_Xpos[16] = { 8, 0x18, -8, 8, 0x18, -8, 8, 0x18, 8, -8, 0x18, 8, -8, 0x18, 8, -8 };
static const int16 kEprojInit_BombTorizoStatueBreaking_Ypos[8] = { -8, -8, 8, 8, 8, 0x18, 0x18, 0x18 };
static const int16 kEprojInit_BombTorizoStatueBreaking_Yvel[8] = { 256, 256, 256, 256, 256, 256, 256, 256 };
static const int16 kEprojInit_BombTorizoStatueBreaking_F[8] = { 16, 16, 16, 16, 16, 16, 16, 16 };
static const int16 kEprojInit_BombTorizoLowHealthExplode_X[6] = { 0, 12, -12, 0, 16, -16 };
static const int16 kEprojInit_BombTorizoLowHealthExplode_Y[6] = { -8, -8, -8, -20, -20, -20 };

void EprojInit_BombTorizoExplosiveSwipe(uint16 j) {  // 0x86A6F6
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  R18_ = v1->x_pos;
  R20_ = v1->y_pos;
  if ((v1->parameter_1 & 0x8000u) != 0) {
    int v4 = enemy_projectile_init_param >> 1;
    int v5 = j >> 1;
    enemy_projectile_x_pos[v5] = R18_ - kEprojInit_BombTorizoExplosiveSwipe_Tab0[v4];
    enemy_projectile_y_pos[v5] = R20_ + kEprojInit_BombTorizoExplosiveSwipe_Tab1[v4];
  } else {
    int v2 = enemy_projectile_init_param >> 1;
    int v3 = j >> 1;
    enemy_projectile_x_pos[v3] = R18_ + kEprojInit_BombTorizoExplosiveSwipe_Tab0[v2];
    enemy_projectile_y_pos[v3] = R20_ + kEprojInit_BombTorizoExplosiveSwipe_Tab1[v2];
  }
}

void EprojInit_BombTorizoStatueBreaking(uint16 j) {  // 0x86A764
  char v1;

  CalculatePlmBlockCoords(plm_id);
  v1 = enemy_projectile_init_param;
  int v2 = enemy_projectile_init_param >> 1;
  int v3 = j >> 1;
  enemy_projectile_instr_list_ptr[v3] = kEprojInit_BombTorizoStatueBreaking_InstrList[v2];
  enemy_projectile_x_pos[v3] = kEprojInit_BombTorizoStatueBreaking_Xpos[v2] + 16 * plm_x_block;
  int v4 = (uint8)(v1 & 0xF) >> 1;
  enemy_projectile_y_pos[v3] = kEprojInit_BombTorizoStatueBreaking_Ypos[v4] + 16 * plm_y_block;
  enemy_projectile_y_vel[v3] = kEprojInit_BombTorizoStatueBreaking_Yvel[v4];
  enemy_projectile_F[v3] = kEprojInit_BombTorizoStatueBreaking_F[v4];
  enemy_projectile_properties[v3] |= 0x1000u;
}

void EprojInit_BombTorizoLowHealthExplode(uint16 j) {  // 0x86A81B
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  int v2 = j >> 1;
  enemy_projectile_x_pos[v2] = v1->x_pos;
  enemy_projectile_y_pos[v2] = v1->y_pos;
  if ((v1->parameter_1 & 0x8000u) == 0) {
    ++enemy_projectile_init_param;
    ++enemy_projectile_init_param;
  }
  ++enemy_projectile_init_param;
  int v3 = ++enemy_projectile_init_param >> 1;
  uint16 v4 = kEprojInit_BombTorizoLowHealthExplode_X[v3] + enemy_projectile_x_pos[v2];
  enemy_projectile_x_pos[v2] = v4;
  enemy_projectile_E[v2] = v4;
  uint16 v5 = kEprojInit_BombTorizoLowHealthExplode_Y[v3] + enemy_projectile_y_pos[v2];
  enemy_projectile_y_pos[v2] = v5;
  enemy_projectile_F[v2] = v5;
}

void EprojInit_BombTorizoDeathExplosion(uint16 j) {  // 0x86A871
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  uint16 x_pos = v1->x_pos;
  int v3 = j >> 1;
  enemy_projectile_x_pos[v3] = x_pos;
  enemy_projectile_E[v3] = x_pos;
  uint16 y_pos = v1->y_pos;
  enemy_projectile_y_pos[v3] = y_pos;
  enemy_projectile_F[v3] = y_pos;
}

void sub_86A887(uint16 v0) {  // 0x86A887
  int16 v2;
  int16 v3;
  int16 v4;

  if (EnemyProjectileBlockCollisition_Horiz(v0) & 1) {
    int v6 = v0 >> 1;
    enemy_projectile_instr_list_ptr[v6] = 0xA48A;
    enemy_projectile_instr_timers[v6] = 1;
  } else {
    int v1 = v0 >> 1;
    v2 = enemy_projectile_x_vel[v1];
    if (v2 >= 0) {
      v4 = v2 - 4;
      if (v4 < 0)
        v4 = 3;
      enemy_projectile_x_vel[v1] = v4;
    } else {
      v3 = v2 + 4;
      if (v3 >= 0)
        v3 = 3;
      enemy_projectile_x_vel[v1] = v3;
    }
    uint8 carry = EnemyProjectileBlockCollisition_Vertical(v0);
    if ((enemy_projectile_y_vel[v1] & 0x8000u) != 0 || !carry) {
      uint16 v5 = enemy_projectile_y_vel[v1] + 16;
      enemy_projectile_y_vel[v1] = v5;
      if ((v5 & 0xF000) == 4096)
        enemy_projectile_id[v1] = 0;
    } else {
      int v7 = v0 >> 1;
      enemy_projectile_y_pos[v7] -= 3;
      enemy_projectile_instr_list_ptr[v7] = 0xA48E;
      enemy_projectile_instr_timers[v7] = 1;
    }
  }
}

void EprojPreInstr_A977(uint16 k) {  // 0x86A8EF
  uint8 carry = EnemyProjectileBlockCollisition_Vertical(k);
  int v1 = k >> 1;
  if ((enemy_projectile_y_vel[v1] & 0x8000u) != 0 || !carry) {
    uint16 v2 = enemy_projectile_F[0] + enemy_projectile_y_vel[v1];
    enemy_projectile_y_vel[v1] = v2;
    if ((v2 & 0xF000) == 4096)
      enemy_projectile_y_vel[v1] = 4096;
  } else {
    enemy_projectile_pre_instr[v1] = 0xA918;
  }
}

void sub_86A91A(uint16 v0) {  // 0x86A91A
  int v1 = v0 >> 1;
  enemy_projectile_x_vel[v1] = 0;
  if ((joypad2_last & 0x100) != 0)
    enemy_projectile_x_vel[v1] = 256;
  if ((joypad2_last & 0x200) != 0)
    enemy_projectile_x_vel[v1] = -256;
  EnemyProjectileBlockCollisition_Horiz(v0);
  enemy_projectile_y_vel[v1] = 0;
  if ((joypad2_last & 0x400) != 0)
    enemy_projectile_y_vel[v1] = 256;
  if ((joypad2_last & 0x800) != 0)
    enemy_projectile_y_vel[v1] = -256;
  EnemyProjectileBlockCollisition_Vertical(v0);
}

void EprojInit_AB07(uint16 j) {  // 0x86AA3D
  VramWriteEntry *v2;
  VramWriteEntry *v3;

  uint16 v1 = vram_write_queue_tail;
  v2 = gVramWriteEntry(vram_write_queue_tail);
  v2->size = 64;
  v2->src.addr = addr_kEprojInit_AB07_Tile0;
  *(uint16 *)&v2->src.bank = 134;
  v2->vram_dst = 28160;
  v1 += 7;
  v3 = gVramWriteEntry(v1);
  v3->size = 64;
  v3->src.addr = addr_kEprojInit_AB07_Tile1;
  *(uint16 *)&v3->src.bank = 134;
  v3->vram_dst = 28416;
  vram_write_queue_tail = v1 + 7;
  int v4 = j >> 1;
  enemy_projectile_x_pos[v4] = samus_x_pos;
  enemy_projectile_y_pos[v4] = samus_y_pos - 36;
}

void EprojPreInstr_AB07(uint16 k) {  // 0x86AA8C
  R18_ = 0;
  R20_ = 0;
  int v1 = k >> 1;
  uint16 v2 = 8 * (samus_x_pos - enemy_projectile_x_pos[v1]);
  if (((samus_x_pos - enemy_projectile_x_pos[v1]) & 0x1000) != 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = v2;
  bool v3 = __CFADD__uint16(enemy_projectile_1A27[v1], R18_);
  enemy_projectile_1A27[v1] += R18_;
  enemy_projectile_x_pos[v1] += v3 + R20_;
  R18_ = 0;
  R20_ = 0;
  uint16 v4 = 8 * (samus_y_pos - 36 - enemy_projectile_y_pos[v1]);
  if (((samus_y_pos - 36 - enemy_projectile_y_pos[v1]) & 0x1000) != 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = v4;
  v3 = __CFADD__uint16(enemy_projectile_y_subpos[v1], R18_);
  enemy_projectile_y_subpos[v1] += R18_;
  enemy_projectile_y_pos[v1] += v3 + R20_;
}

uint16 EprojInstr_SpawnEnemyDrops(uint16 k, uint16 j) {  // 0x86AB8A
  int v2 = k >> 1;
  uint16 v3;
  R18_ = enemy_projectile_x_pos[v2];
  R20_ = enemy_projectile_y_pos[v2];
  if (area_index)
    v3 = *((uint16 *)RomPtr_86(j) + 1);
  else
    v3 = *(uint16 *)RomPtr_86(j);
  SpawnEnemyDrops(v3, k);
  return j + 4;
}

void Eproj_InitXYVelRandom(uint16 j, uint16 k) {  // 0x86ABAE
  uint16 *v2 = (uint16 *)RomPtr_86(k);
  int v3 = j >> 1;
  enemy_projectile_instr_list_ptr[v3] = *v2;
  enemy_projectile_x_pos[v3] = v2[1] + R18_;
  enemy_projectile_x_vel[v3] = v2[2] + (uint8)NextRandom() - 128;
  enemy_projectile_y_pos[v3] = v2[3] + R20_;
  enemy_projectile_y_vel[v3] = v2[4] + (uint8)NextRandom() - 128;
}

void EprojInit_BombTorizosChozoOrbs(uint16 j) {  // 0x86ABEB
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  uint16 v2;
  R18_ = v1->x_pos;
  R20_ = v1->y_pos;
  if ((v1->parameter_1 & 0x8000u) != 0)
    v2 = addr_kEprojInit_BombTorizosChozoOrbs_init0;
  else
    v2 = addr_kEprojInit_BombTorizosChozoOrbs_init1;
  Eproj_InitXYVelRandom(j, v2);
}

void EprojInit_GoldenTorizosChozoOrbs(uint16 j) {  // 0x86AC7C
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  uint16 v2;
  R18_ = v1->x_pos;
  R20_ = v1->y_pos;
  if ((v1->parameter_1 & 0x8000u) != 0)
    v2 = addr_kEprojInit_GoldenTorizosChozoOrbs_init0;
  else
    v2 = addr_kEprojInit_GoldenTorizosChozoOrbs_init1;
  Eproj_InitXYVelRandom(j, v2);
}

void EprojPreInstr_BombTorizosChozoOrbs(uint16 k) {  // 0x86ACAD
  if (EnemyProjectileBlockCollisition_Horiz(k)) {
    int v3 = k >> 1;
    enemy_projectile_instr_list_ptr[v3] = addr_off_86AB25;
    enemy_projectile_instr_timers[v3] = 1;
  } else {
    uint8 carry = EnemyProjectileBlockCollisition_Vertical(k);
    int v1 = k >> 1;
    if ((enemy_projectile_y_vel[v1] & 0x8000u) != 0 || !carry) {
      uint16 v2 = enemy_projectile_y_vel[v1] + 18;
      enemy_projectile_y_vel[v1] = v2;
      if ((v2 & 0xF000) == 4096)
        enemy_projectile_id[v1] = 0;
    } else {
      int v4 = k >> 1;
      enemy_projectile_y_pos[v4] = (enemy_projectile_y_pos[v4] & 0xFFF0 | 8) - 2;
      enemy_projectile_instr_list_ptr[v4] = addr_off_86AB41;
      enemy_projectile_instr_timers[v4] = 1;
    }
  }
}

void EprojPreInstr_GoldenTorizosChozoOrbs(uint16 k) {  // 0x86ACFA
  int v1;
  int16 v2;
  uint16 v3;
  uint16 v4;

  if (EnemyProjectileBlockCollisition_Horiz(k) & 1)
    enemy_projectile_x_vel[k >> 1] = -enemy_projectile_x_vel[k >> 1];
  if (EnemyProjectileBlockCollisition_Vertical(k) & 1
      && (v1 = k >> 1, (enemy_projectile_y_vel[v1] & 0x8000u) == 0)
      && ((v2 = enemy_projectile_x_vel[v1], v2 >= 0) ? (v3 = v2 - 64) : (v3 = v2 + 64),
          enemy_projectile_x_vel[v1] = v3,
          v4 = -(enemy_projectile_y_vel[v1] >> 1),
          enemy_projectile_y_vel[v1] = v4,
          (v4 & 0xFF80) == 0xFF80)) {
    int v5 = k >> 1;
    enemy_projectile_y_pos[v5] = (enemy_projectile_y_pos[v5] & 0xFFF0 | 8) - 2;
    enemy_projectile_instr_list_ptr[v5] = addr_off_86AB41;
    enemy_projectile_instr_timers[v5] = 1;
  } else {
    enemy_projectile_y_vel[k >> 1] += 24;
  }
}

uint16 EprojInstr_GotoDependingOnXDirection(uint16 k, uint16 j) {  // 0x86AD92
  int16 v3;

  R18_ = 0;
  R20_ = 0;
  int v2 = k >> 1;
  v3 = enemy_projectile_x_vel[v2];
  if (v3 < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = v3;
  uint16 v4 = enemy_projectile_1A27[v2];
  bool v5 = __CFADD__uint16(R18_, v4);
  enemy_projectile_1A27[v2] = R18_ + v4;
  enemy_projectile_x_pos[v2] += R20_ + v5;
  if ((enemy_projectile_x_vel[v2] & 0x8000u) == 0)
    return *((uint16 *)RomPtr_86(j) + 1);
  else
    return *(uint16 *)RomPtr_86(j);
}

void EprojInit_TorizoSonicBoom(uint16 j) {  // 0x86AE15
  int16 v1;

  NextRandom();
  if ((random_number & 1) != 0)
    v1 = -12;
  else
    v1 = 20;
  EnemyData *v2 = gEnemyData(cur_enemy_index);
  int v3 = j >> 1;
  enemy_projectile_y_pos[v3] = v2->y_pos + v1;
  enemy_projectile_y_vel[v3] = 0;
  if ((v2->parameter_1 & 0x8000u) != 0) {
    enemy_projectile_x_pos[v3] = v2->x_pos + 32;
    enemy_projectile_x_vel[v3] = 624;
    enemy_projectile_instr_list_ptr[v3] = addr_off_86ADD2;
  } else {
    enemy_projectile_x_pos[v3] = v2->x_pos - 32;
    enemy_projectile_x_vel[v3] = -624;
    enemy_projectile_instr_list_ptr[v3] = addr_off_86ADBF;
  }
}

void EprojPreInstr_TorizoSonicBoom(uint16 k) {  // 0x86AE6C
  int16 v2;
  uint16 v3;

  if (EnemyProjectileBlockCollisition_Horiz(k) & 1) {
    int v4 = k >> 1;
    enemy_projectile_instr_list_ptr[v4] = addr_off_86ADE5;
    enemy_projectile_instr_timers[v4] = 1;
    enemy_projectile_E[v4] = enemy_projectile_x_pos[v4];
    enemy_projectile_F[v4] = enemy_projectile_y_pos[v4];
  } else {
    int v1 = k >> 1;
    v2 = enemy_projectile_x_vel[v1];
    if (v2 < 0)
      v3 = v2 - 16;
    else
      v3 = v2 + 16;
    enemy_projectile_x_vel[v1] = v3;
    if ((v3 & 0xF000) == 4096)
      enemy_projectile_id[v1] = 0;
  }
}

void EprojInit_WreckedShipChozoSpikeFootsteps(uint16 j) {  // 0x86AEFC
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  int v2 = j >> 1;
  enemy_projectile_x_pos[v2] = enemy_projectile_init_param + v1->x_pos;
  enemy_projectile_y_pos[v2] = v1->y_pos + 28;
}

uint16 EprojInstr_ResetXYpos1(uint16 k, uint16 j) {  // 0x86AF36
  int v2 = k >> 1;
  enemy_projectile_x_pos[v2] = enemy_projectile_E[v2];
  enemy_projectile_y_pos[v2] = enemy_projectile_F[v2];
  return j;
}

void EprojInit_TourianStatueDustClouds(uint16 j) {  // 0x86AF43
  int v1 = j >> 1;
  enemy_projectile_E[v1] = 128;
  enemy_projectile_F[v1] = 188;
}

void EprojInit_TourianLandingDustCloudsRightFoot(uint16 j) {  // 0x86AF50
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  int v2 = j >> 1;
  enemy_projectile_y_pos[v2] = v1->y_pos + 48;
  enemy_projectile_x_pos[v2] = v1->x_pos + 24;
}

uint16 EprojInstr_MoveY_Minus4(uint16 k, uint16 j) {  // 0x86AF92
  enemy_projectile_y_pos[k >> 1] -= 4;
  return j;
}

void EprojInit_TorizoLandingDustCloudLeftFoot(uint16 j) {  // 0x86AFCD
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  int v2 = j >> 1;
  enemy_projectile_y_pos[v2] = v1->y_pos + 48;
  enemy_projectile_x_pos[v2] = v1->x_pos - 24;
}

void EprojInit_GoldenTorizoEgg(uint16 j) {  // 0x86B001
  int16 parameter_1;
  uint16 v4;
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  R18_ = v1->x_pos;
  R20_ = v1->y_pos;
  int v2 = j >> 1;
  enemy_projectile_F[v2] = (0xe2 & 0x1F) + 64;
  parameter_1 = v1->parameter_1;
  enemy_projectile_E[v2] = parameter_1;
  if (parameter_1 < 0)
    v4 = addr_kEprojInit_GoldenTorizoEgg0;
  else
    v4 = addr_kEprojInit_GoldenTorizoEgg1;
  Eproj_InitXYVelRandom(j, v4);
}

void EprojPreInstr_GoldenTorizoEgg(uint16 k) {  // 0x86B043
  int16 v3;
  uint16 v4, v6;
  int v1 = k >> 1;
  if ((--enemy_projectile_F[v1] & 0x8000u) != 0) {
    ++enemy_projectile_instr_list_ptr[v1];
    ++enemy_projectile_instr_list_ptr[v1];
    enemy_projectile_instr_timers[v1] = 1;
    if ((enemy_projectile_E[v1] & 0x8000u) != 0)
      v6 = 256;
    else
      v6 = -256;
    enemy_projectile_x_vel[v1] = v6;
  } else {
    if (EnemyProjectileBlockCollisition_Horiz(k) & 1) {
      enemy_projectile_x_vel[v1] = -enemy_projectile_x_vel[v1];
      enemy_projectile_E[v1] ^= 0x8000u;
    }
    if (EnemyProjectileBlockCollisition_Vertical(k) & 1 && (enemy_projectile_y_vel[v1] & 0x8000u) == 0) {
      v3 = enemy_projectile_x_vel[v1];
      if (v3 >= 0)
        v4 = v3 - 32;
      else
        v4 = v3 + 32;
      enemy_projectile_x_vel[v1] = v4;
      enemy_projectile_y_vel[v1] = -enemy_projectile_y_vel[v1];
    }
    uint16 v5 = enemy_projectile_y_vel[v1] + 48;
    enemy_projectile_y_vel[v1] = v5;
    if ((v5 & 0xF000) == 4096)
      enemy_projectile_id[v1] = 0;
  }
}

void sub_86B0B9(uint16 k) {  // 0x86B0B9
  int16 v2;

  if (EnemyProjectileBlockCollisition_Horiz(k) & 1) {
    int v3 = k >> 1;
    enemy_projectile_pre_instr[v3] = 0xB0DD;
    enemy_projectile_y_vel[v3] = 0;
  } else {
    int v1 = k >> 1;
    if ((enemy_projectile_E[v1] & 0x8000u) != 0)
      v2 = 48;
    else
      v2 = -48;
    enemy_projectile_x_vel[v1] += v2;
  }
}

void sub_86B0DD(uint16 k) {  // 0x86B0DD
  uint16 v2;
  if (EnemyProjectileBlockCollisition_Vertical(k) & 1) {
    int v1 = k >> 1;
    if ((enemy_projectile_E[v1] & 0x8000u) != 0)
      v2 = addr_off_86B1A8;
    else
      v2 = addr_off_86B190;
    enemy_projectile_instr_list_ptr[v1] = v2;
    enemy_projectile_instr_timers[v1] = 1;
  } else {
    enemy_projectile_y_vel[k >> 1] += 48;
  }
}

uint16 sub_86B13E(uint16 k, uint16 j) {  // 0x86B13E
  if ((enemy_projectile_E[k >> 1] & 0x8000u) != 0)
    return addr_off_86B166;
  else
    return addr_off_86B14B;
}

uint16 sub_86B183(uint16 k, uint16 j) {  // 0x86B183
  if ((enemy_projectile_E[k >> 1] & 0x8000u) != 0)
    return addr_off_86B1A8;
  else
    return addr_off_86B190;
}

static const int16 word_86B205[2] = { -0x1e, 0x1e };
#define off_86B209 ((uint16*)RomPtr(0x86b209))

void EprojInit_GoldenTorizoSuperMissile(uint16 j) {  // 0x86B1CE
  uint16 v4;

  int v2 = j >> 1;
  enemy_projectile_E[v2] = cur_enemy_index;
  EnemyData *v3 = gEnemyData(cur_enemy_index);
  R18_ = v3->x_pos;
  R20_ = v3->y_pos;
  if ((v3->parameter_1 & 0x8000u) != 0)
    v4 = 2;
  else
    v4 = 0;
  int v5 = v4 >> 1;
  enemy_projectile_x_pos[v2] = R18_ + word_86B205[v5];
  enemy_projectile_y_pos[v2] = R20_ - 52;
  enemy_projectile_instr_list_ptr[v2] = off_86B209[v5];
}

void EprojPreInstr_GoldenTorizoSuperMissile(uint16 k) {  // 0x86B20D
  int16 v3;

  int v1 = k >> 1;
  EnemyData *v2 = gEnemyData(enemy_projectile_E[v1]);
  R18_ = v2->x_pos;
  R20_ = v2->y_pos;
  if ((v2->parameter_1 & 0x8000u) != 0)
    v3 = 32;
  else
    v3 = -32;
  enemy_projectile_x_pos[v1] = R18_ + v3;
  enemy_projectile_y_pos[v1] = R20_ - 52;
}

void EprojPreInstr_B237(uint16 k) {  // 0x86B237
  int v1;
  uint8 carry;

  if (EnemyProjectileBlockCollisition_Horiz(k)
      || (carry = EnemyProjectileBlockCollisition_Vertical(k), v1 = k >> 1, (enemy_projectile_y_vel[v1] & 0x8000u) == 0)
      && carry) {
    int v3 = k >> 1;
    enemy_projectile_instr_list_ptr[v3] = addr_off_86B2EF;
    enemy_projectile_instr_timers[v3] = 1;
  } else {
    uint16 v2 = enemy_projectile_y_vel[v1] + 16;
    enemy_projectile_y_vel[v1] = v2;
    if ((v2 & 0xF000) == 4096)
      enemy_projectile_id[v1] = 0;
  }
}

uint16 EprojInstr_SetVelTowardsSamus1(uint16 k, uint16 j) {  // 0x86B269
  sub_86B279(k, CalculateAngleOfSamusFromEproj(k) & 0x7F);
  return j;
}

uint16 EprojInstr_SetVelTowardsSamus2(uint16 k, uint16 j) {  // 0x86B272
  sub_86B279(k, CalculateAngleOfSamusFromEproj(k) | 0x80);
  return j;
}

void sub_86B279(uint16 k, uint16 a) {  // 0x86B279
  int v2 = a;
  int v3 = k >> 1;
  enemy_projectile_x_vel[v3] = 4 * kSinCosTable8bit_Sext[v2 + 64];
  enemy_projectile_y_vel[v3] = 4 * kSinCosTable8bit_Sext[v2];
}
void EprojInit_GoldenTorizoEyeBeam(uint16 j) {  // 0x86B328
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  uint16 v2;
  R18_ = v1->x_pos;
  R20_ = v1->y_pos;
  if ((v1->parameter_1 & 0x8000u) != 0)
    v2 = addr_stru_86B376;
  else
    v2 = addr_stru_86B380;
  Eproj_InitXYVelRandom(j, v2);
  uint16 v3 = (NextRandom() & 0x1E) - 16 + 192;
  if ((gEnemyData(cur_enemy_index)->parameter_1 & 0x8000u) == 0)
    v3 += 128;
  int v4 = v3 >> 1;
  int v5 = j >> 1;
  enemy_projectile_x_vel[v5] = 8 * kSinCosTable8bit_Sext[v4 + 64];
  enemy_projectile_y_vel[v5] = 8 * kSinCosTable8bit_Sext[v4];
}

void EprojPreInstr_GoldenTorizoEyeBeam(uint16 k) {  // 0x86B38A
  if (EnemyProjectileBlockCollisition_Horiz(k) & 1) {
    enemy_projectile_instr_list_ptr[k >> 1] = addr_off_86B3CD;
LABEL_6:
    enemy_projectile_instr_timers[k >> 1] = 1;
    return;
  }
  if (EnemyProjectileBlockCollisition_Vertical(k) & 1) {
    int v1 = k >> 1;
    enemy_projectile_y_pos[v1] = (enemy_projectile_y_pos[v1] & 0xFFF0 | 8) - 2;
    enemy_projectile_instr_list_ptr[v1] = addr_off_86B3E5;
    goto LABEL_6;
  }
}

uint16 EprojInstr_GotoIfFunc1(uint16 k, uint16 j) {  // 0x86B3B8
  if ((gExtraEnemyRam7800(enemy_projectile_F[k >> 1])->kraid.kraid_healths_8ths[0] & 0x8000u) == 0)
    return *(uint16 *)RomPtr_86(j);
  else
    return j + 2;
}

uint16 EprojInstr_ResetXYpos2(uint16 k, uint16 j) {  // 0x86B436
  int v2 = k >> 1;
  enemy_projectile_x_pos[v2] = enemy_projectile_E[v2];
  enemy_projectile_y_pos[v2] = enemy_projectile_F[v2];
  return j;
}

void EprojInit_TourianEscapeShaftFakeWallExplode(uint16 j) {  // 0x86B49D
  int v1 = j >> 1;
  enemy_projectile_x_pos[v1] = 272;
  enemy_projectile_E[v1] = 272;
  enemy_projectile_y_pos[v1] = 2184;
  enemy_projectile_F[v1] = 2184;
}

void EprojInit_LavaSeahorseFireball(uint16 j) {  // 0x86B4EF
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  int v2 = j >> 1;
  enemy_projectile_y_pos[v2] = v1->y_pos - 28;
  enemy_projectile_y_vel[v2] = -961;
  if ((v1->ai_var_A & 0x8000u) == 0) {
    enemy_projectile_x_pos[v2] = v1->x_pos + 12;
    enemy_projectile_x_vel[v2] = 704;
    enemy_projectile_instr_list_ptr[v2] = addr_word_86B4CB;
  } else {
    enemy_projectile_x_pos[v2] = v1->x_pos - 12;
    enemy_projectile_x_vel[v2] = -704;
    enemy_projectile_instr_list_ptr[v2] = addr_word_86B4BF;
  }
}

void sub_86B535(uint16 k) {  // 0x86B535
  int16 v6;
  uint16 v7;

  int v1 = k >> 1;
  R20_ = LOBYTE(enemy_projectile_x_vel[v1]) << 8;
  R18_ = (int8)HIBYTE(enemy_projectile_x_vel[v1]);
  uint16 v4 = enemy_projectile_1A27[v1];
  bool v3 = __CFADD__uint16(R20_, v4);
  enemy_projectile_1A27[v1] = R20_ + v4;
  enemy_projectile_x_pos[v1] += R18_ + v3;
  R20_ = LOBYTE(enemy_projectile_y_vel[v1]) << 8;
  R18_ = (int8)HIBYTE(enemy_projectile_y_vel[v1]);
  uint16 v5 = enemy_projectile_y_subpos[v1];
  v3 = __CFADD__uint16(R20_, v5);
  enemy_projectile_y_subpos[v1] = R20_ + v5;
  enemy_projectile_y_pos[v1] += R18_ + v3;
  v6 = enemy_projectile_y_vel[v1];
  if (v6 >= 0) {
    enemy_projectile_y_vel[v1] = v6 + 32;
    Eproj_DeleteIfYposOutside(k);
  } else {
    enemy_projectile_y_vel[v1] = v6 + 32;
    if ((int16)(v6 + 32) >= 0) {
      if ((enemy_projectile_x_vel[v1] & 0x8000u) == 0)
        v7 = addr_word_86B4E3;
      else
        v7 = addr_word_86B4D7;
      enemy_projectile_instr_list_ptr[v1] = v7;
      enemy_projectile_instr_timers[v1] = 1;
    }
  }
}

void Eproj_DeleteIfYposOutside(uint16 k) {  // 0x86B5B9
  int v1 = k >> 1;
  if ((int16)(enemy_projectile_y_pos[v1] - layer1_y_pos) >= 288)
    enemy_projectile_id[v1] = 0;
}

static const int16 word_86B65B[20] = {  // 0x86B62D
  -16,  16, -96, -64, -128, -32, -96, 64, -128, 32, 16, 16, 96, -64, 112, -64,
  128, -64, 144, -64,
};

void EprojInit_EyeDoorProjectile(uint16 j) {

  uint16 v1 = plm_id;
  int v2 = j >> 1;
  enemy_projectile_F[v2] = plm_room_arguments[plm_id >> 1];
  CalculatePlmBlockCoords(v1);
  int v3 = enemy_projectile_init_param >> 1;
  enemy_projectile_x_pos[v2] = word_86B65B[v3] + 8 * (2 * plm_x_block + 1);
  enemy_projectile_y_pos[v2] = word_86B65B[v3 + 1] + 16 * plm_y_block;
}

static const int16 word_86B6B1[4] = { -64, 512, 64, 512 };

void EprojInit_EyeDoorSweat(uint16 j) {  // 0x86B683
  CalculatePlmBlockCoords(plm_id);
  int v1 = j >> 1;
  enemy_projectile_x_pos[v1] = 8 * (2 * (plm_x_block - 1) + 1);
  enemy_projectile_y_pos[v1] = 16 * (plm_y_block + 1);
  int v2 = enemy_projectile_init_param >> 1;
  enemy_projectile_x_vel[v1] = word_86B6B1[v2];
  enemy_projectile_y_vel[v1] = word_86B6B1[v2 + 1];
}

void EprojPreInstr_EyeDoorProjectile(uint16 k) {  // 0x86B6B9
  int16 v2;
  int16 v4;

  if (EnemyProjectileBlockCollisition_Horiz(k) & 1 || EnemyProjectileBlockCollisition_Vertical(k) & 1) {
LABEL_8:;
    int v5 = k >> 1; // bug fixed
    enemy_projectile_instr_list_ptr[v5] = addr_off_86B5F3;
    enemy_projectile_instr_timers[v5] = 1;
    return;
  }
  int v1 = enemy_projectile_E[k >> 1] >> 1;
  v2 = kSinCosTable8bit_Sext[v1 + 64] >> 4;
  if ((v2 & 0x800) != 0)
    v2 |= 0xF000u;
  int v3 = k >> 1;
  enemy_projectile_x_vel[v3] += v2;
  v4 = kSinCosTable8bit_Sext[v1] >> 4;
  if ((v4 & 0x800) != 0)
    v4 |= 0xF000u;
  enemy_projectile_y_vel[v3] += v4;
  int t = PrepareBitAccess(enemy_projectile_F[v3]);
  if ((bitmask & opened_door_bit_array[t]) != 0) {
    goto LABEL_8;
  }
}

void EprojPreInstr_EyeDoorSweat(uint16 k) {  // 0x86B714
  EnemyProjectileBlockCollisition_Horiz(k);
  uint8 carry = EnemyProjectileBlockCollisition_Vertical(k);
  int v1 = k >> 1;
  if ((enemy_projectile_y_vel[v1] & 0x8000u) != 0 || !carry) {
    enemy_projectile_y_vel[v1] += 12;
  } else {
    enemy_projectile_y_pos[v1] -= 4;
    enemy_projectile_instr_list_ptr[v1] = addr_off_86B61D;
    enemy_projectile_instr_timers[v1] = 1;
  }
}

uint16 EprojInstr_SpawnTourianStatueUnlockingParticle(uint16 k, uint16 j) {  // 0x86B7EA
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_TourianStatueUnlockingParticle, k);
  return j;
}

uint16 EprojInstr_Earthquake(uint16 k, uint16 j) {  // 0x86B7F5
  earthquake_type = 1;
  earthquake_timer |= 0x20u;
  return j;
}

uint16 EprojInstr_SpawnTourianStatueUnlockingParticleTail(uint16 k, uint16 j) {  // 0x86B818
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_TourianStatueUnlockingParticleTail, k);
  return j;
}

uint16 EprojInstr_AddToYpos(uint16 k, uint16 j) {  // 0x86B841
  enemy_projectile_y_pos[k >> 1] += *(uint16 *)RomPtr_86(j);
  return j + 2;
}

void EprojInit_TourianStatueUnlockingParticleWaterSplash(uint16 j) {  // 0x86B87A
  int v1 = j >> 1;
  enemy_projectile_x_pos[v1] = enemy_projectile_x_pos[enemy_projectile_init_param >> 1];
  enemy_projectile_y_pos[v1] = fx_y_pos - 4;
}

static const uint16 kEprojInit_TourianStatueEyeGlow_X[4] = { 0x84, 0x7a, 0x9e, 0x68 };
static const uint16 kEprojInit_TourianStatueEyeGlow_Y[4] = { 0x90, 0x51, 0x80, 0x72 };
static const uint16 kEprojInit_TourianStatueEyeGlow_Colors[16] = { 0x6bff, 0x33b, 0x216, 0x173, 0x7f5f, 0x7c1f, 0x5816, 0x300c, 0x7f5a, 0x7ec0, 0x6de0, 0x54e0, 0x6bfa, 0x3be0, 0x2680, 0x1580 };

void EprojInit_TourianStatueEyeGlow(uint16 j) {  // 0x86B88E
  uint16 v1 = enemy_projectile_init_param;
  int v2 = enemy_projectile_init_param >> 1;
  int v3 = j >> 1;
  enemy_projectile_x_pos[v3] = kEprojInit_TourianStatueEyeGlow_X[v2];
  enemy_projectile_y_pos[v3] = kEprojInit_TourianStatueEyeGlow_Y[v2];
  uint16 v4 = 4 * v1;
  for (int i = 498; i != 506; i += 2) {
    palette_buffer[i >> 1] = kEprojInit_TourianStatueEyeGlow_Colors[v4 >> 1];
    v4 += 2;
  }
}

void EprojInit_TourianStatueUnlockingParticle(uint16 j) {  // 0x86B8B5
  int v1 = enemy_projectile_init_param >> 1;
  int v2 = j >> 1;
  enemy_projectile_x_pos[v2] = enemy_projectile_x_pos[v1];
  enemy_projectile_y_pos[v2] = enemy_projectile_y_pos[v1];
  uint16 v3 = 2 * (uint8)((NextRandom() & 0x3F) - 32);
  enemy_projectile_E[v2] = v3;
  int v4 = v3 >> 1;
  enemy_projectile_x_vel[v2] = kSinCosTable8bit_Sext[v4 + 64];
  enemy_projectile_y_vel[v2] = 4 * kSinCosTable8bit_Sext[v4];
}

void EprojIni_TourianStatueUnlockingParticleTail(uint16 v0) {  // 0x86B8E8
  int v1 = enemy_projectile_init_param >> 1;
  int v2 = v0 >> 1;
  enemy_projectile_x_pos[v2] = enemy_projectile_x_pos[v1];
  enemy_projectile_y_pos[v2] = enemy_projectile_y_pos[v1];
}

void EprojInit_TourianStatueSoul(uint16 j) {  // 0x86B8F8
  int v1 = enemy_projectile_init_param >> 1;
  int v2 = j >> 1;
  enemy_projectile_x_pos[v2] = kEprojInit_TourianStatueEyeGlow_X[v1];
  enemy_projectile_y_pos[v2] = kEprojInit_TourianStatueEyeGlow_Y[v1];
  enemy_projectile_y_vel[v2] = -1024;
}

void EprojInit_TourianStatueBaseDecoration(uint16 j) {  // 0x86B93E
  int v1 = j >> 1;
  enemy_projectile_E[v1] = 120;
  enemy_projectile_x_pos[v1] = 120;
  enemy_projectile_F[v1] = 184;
  enemy_projectile_y_pos[v1] = 184;
}

void EprojInit_TourianStatueRidley(uint16 j) {  // 0x86B951
  int v1 = j >> 1;
  enemy_projectile_E[v1] = 142;
  enemy_projectile_x_pos[v1] = 142;
  enemy_projectile_F[v1] = 85;
  enemy_projectile_y_pos[v1] = 85;
}

void EprojInit_TourianStatuePhantoon(uint16 j) {  // 0x86B964
  int v1 = j >> 1;
  enemy_projectile_E[v1] = 132;
  enemy_projectile_x_pos[v1] = 132;
  enemy_projectile_F[v1] = 136;
  enemy_projectile_y_pos[v1] = 136;
}

void EprojPreInstr_TourianStatueUnlockingParticleWaterSplash(uint16 k) {  // 0x86B977
  enemy_projectile_y_pos[k >> 1] = fx_y_pos - 4;
}

void EprojPreInstr_TourianStatueUnlockingParticle(uint16 k) {  // 0x86B982
  int16 v2;
  int16 v5;
  int16 v7;

  R18_ = 0;
  R20_ = 0;
  int v1 = k >> 1;
  v2 = enemy_projectile_x_vel[v1];
  if (v2 < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = v2;
  uint16 v3 = enemy_projectile_1A27[v1];
  bool v4 = __CFADD__uint16(R18_, v3);
  enemy_projectile_1A27[v1] = R18_ + v3;
  enemy_projectile_x_pos[v1] += R20_ + v4;
  v7 = fx_y_pos - enemy_projectile_y_pos[v1];
  R18_ = 0;
  R20_ = 0;
  v5 = enemy_projectile_y_vel[v1];
  if (v5 < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = v5;
  uint16 v6 = enemy_projectile_y_subpos[v1];
  v4 = __CFADD__uint16(R18_, v6);
  enemy_projectile_y_subpos[v1] = R18_ + v6;
  enemy_projectile_y_pos[v1] += R20_ + v4;
  if (((v7 ^ (fx_y_pos - enemy_projectile_y_pos[v1])) & 0x8000u) != 0)
    SpawnEnemyProjectileWithRoomGfx(addr_stru_86BA5C, k);
  if ((enemy_projectile_y_pos[v1] & 0xFF00) == 256) {
    enemy_projectile_instr_list_ptr[v1] = addr_off_86B79F;
    enemy_projectile_instr_timers[v1] = 1;
  } else {
    enemy_projectile_y_vel[v1] += 16;
  }
}

void EprojPreInstr_TourianStatueSoul(uint16 k) {  // 0x86B9FD
  int16 v2;

  R18_ = 0;
  R20_ = 0;
  int v1 = k >> 1;
  v2 = enemy_projectile_y_vel[v1];
  if (v2 < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = v2;
  uint16 v3 = enemy_projectile_y_subpos[v1];
  bool v4 = __CFADD__uint16(R18_, v3);
  enemy_projectile_y_subpos[v1] = R18_ + v3;
  uint16 v5 = R20_ + v4 + enemy_projectile_y_pos[v1];
  enemy_projectile_y_pos[v1] = v5;
  if ((v5 & 0x100) != 0) {
    enemy_projectile_instr_list_ptr[v1] = addr_off_86B79F;
    enemy_projectile_instr_timers[v1] = 1;
  }
  enemy_projectile_y_vel[v1] -= 128;
}

void EprojPreInstr_TourianStatueStuff(uint16 k) {  // 0x86BA37
  if (!tourian_entrance_statue_animstate)
    tourian_entrance_statue_finished |= 0x8000;
  EprojPreInstr_BA42(k);
}

void EprojPreInstr_BA42(uint16 k) {  // 0x86BA42
  int v1 = k >> 1;
  enemy_projectile_x_pos[v1] = enemy_projectile_E[v1];
  enemy_projectile_y_pos[v1] = enemy_projectile_F[v1] + layer1_y_pos - *(uint16 *)&hdma_window_1_left_pos[0].field_0;
}

uint16 EprojInstr_BB24(uint16 k, uint16 j) {  // 0x86BB24
  enemy_projectile_instr_list_ptr[stru_86BB50.init_code_ptr >> 1] = 0;
  return j;
}

void sub_86BB30(uint16 j) {  // 0x86BB30
  ExtraEnemyRam8000 *v1;

  v1 = gExtraEnemyRam8000(cur_enemy_index);
  int v2 = j >> 1;
  enemy_projectile_x_pos[v2] = *(uint16 *)&v1->pad[34];
  enemy_projectile_y_pos[v2] = *(uint16 *)&v1->pad[36];
  enemy_projectile_instr_list_ptr[v2] = off_86BB1E[enemy_projectile_init_param];
}

void EprojInit_NuclearWaffleBody(uint16 j) {  // 0x86BB92
  ExtraEnemyRam8000 *v4;

  EnemyData *v2 = gEnemyData(cur_enemy_index);
  int v3 = j >> 1;
  enemy_projectile_x_pos[v3] = v2->x_pos;
  enemy_projectile_1A27[v3] = v2->x_subpos;
  enemy_projectile_y_pos[v3] = v2->y_pos;
  enemy_projectile_y_subpos[v3] = v2->y_subpos;
  v4 = gExtraEnemyRam8000(cur_enemy_index);
  gExtraEnemyRam7800(cur_enemy_index + *(uint16 *)&v4->pad[20])->kraid.kraid_next = j;
  enemy_projectile_flags[v3] = 1;
}

void EprojInit_NorfairLavaquakeRocks(uint16 j) {  // 0x86BBDB
  int v1 = j >> 1;
  enemy_projectile_instr_list_ptr[v1] = addr_word_86BBD5;
  enemy_projectile_E[v1] = FUNC16(Eproj_NorfairLavaquakeRocks_Func1);
  enemy_projectile_y_vel[v1] = enemy_projectile_init_param;
  enemy_projectile_x_vel[v1] = enemy_projectile_unk1995;
  EnemyData *v2 = gEnemyData(cur_enemy_index);
  enemy_projectile_x_pos[v1] = v2->x_pos;
  enemy_projectile_1A27[v1] = v2->x_subpos;
  enemy_projectile_y_pos[v1] = v2->y_pos;
  enemy_projectile_y_subpos[v1] = v2->y_subpos;
}

void CallNorfairLavaquakeRocksFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnEproj_NorfairLavaquakeRocks_Func1: Eproj_NorfairLavaquakeRocks_Func1(k); return;
  case fnEproj_NorfairLavaquakeRocks_Func2: Eproj_NorfairLavaquakeRocks_Func2(k); return;
  default: Unreachable();
  }
}

void EprojPreInstr_NorfairLavaquakeRocks(uint16 k) {  // 0x86BC0F
  CallNorfairLavaquakeRocksFunc(enemy_projectile_E[k >> 1] | 0x860000, k);
  EprojPreInstr_NorfairLavaquakeRocks_Inner(k);
}

void Eproj_NorfairLavaquakeRocks_Func1(uint16 k) {  // 0x86BC16
  int16 v2;
  int16 v3;
  int16 v8;

  int v1 = k >> 1;
  v2 = enemy_projectile_y_vel[v1] - 2;
  enemy_projectile_y_vel[v1] = v2;
  if (v2 >= 0) {
    R18_ = 2;
    do {
      v3 = R18_ + enemy_projectile_y_vel[k >> 1] - 1;
      if (v3 < 0)
        v3 = 0;
      int v4 = k >> 1;
      enemy_projectile_F[v4] = kCommonEnemySpeeds_Quadratic_Copy[(uint16)(8 * v3 + 4) >> 1];
      uint16 v5 = enemy_projectile_y_subpos[v4];
      bool v6 = __CFADD__uint16(enemy_projectile_F[v4], v5);
      uint16 v7 = enemy_projectile_F[v4] + v5;
      if (v6)
        ++enemy_projectile_y_pos[v4];
      enemy_projectile_y_subpos[v4] = v7;
      v8 = R18_ + enemy_projectile_y_vel[v4] - 1;
      if (v8 < 0)
        v8 = 0;
      int v9 = k >> 1;
      enemy_projectile_F[v9] = kCommonEnemySpeeds_Quadratic_Copy[(uint16)(8 * v8 + 6) >> 1];
      enemy_projectile_y_pos[v9] += enemy_projectile_F[v9];
      --R18_;
    } while (R18_);
    Eproj_NorfairLavaquakeRocks_Func3(k);
  } else {
    enemy_projectile_y_vel[v1] = 0;
    enemy_projectile_E[v1] = FUNC16(Eproj_NorfairLavaquakeRocks_Func2);
  }
}

void Eproj_NorfairLavaquakeRocks_Func2(uint16 k) {  // 0x86BC8F
  int v1 = k >> 1;
  uint16 v2 = enemy_projectile_y_vel[v1] + 2;
  enemy_projectile_y_vel[v1] = v2;
  if (!sign16(v2 - 64))
    enemy_projectile_y_vel[v1] = 64;
  R18_ = 2;
  do {
    int v3 = k >> 1;
    enemy_projectile_F[v3] = kCommonEnemySpeeds_Quadratic_Copy[(uint16)(8
                                                                        * (enemy_projectile_y_vel[v3] - R18_ + 1)) >> 1];
    uint16 v4 = enemy_projectile_y_subpos[v3];
    bool v5 = __CFADD__uint16(enemy_projectile_F[v3], v4);
    uint16 v6 = enemy_projectile_F[v3] + v4;
    if (v5)
      ++enemy_projectile_y_pos[v3];
    enemy_projectile_y_subpos[v3] = v6;
    int v7 = k >> 1;
    enemy_projectile_F[v7] = kCommonEnemySpeeds_Quadratic_Copy[(uint16)(8
                                                                        * (enemy_projectile_y_vel[v3] - R18_ + 1)
                                                                        + 2) >> 1];
    enemy_projectile_y_pos[v7] += enemy_projectile_F[v7];
    --R18_;
  } while (R18_);
  Eproj_NorfairLavaquakeRocks_Func3(k);
}

void Eproj_NorfairLavaquakeRocks_Func3(uint16 k) {  // 0x86BCF4
  int v1 = k >> 1;
  enemy_projectile_x_pos[v1] += SignExtend8((uint8)((uint16)(enemy_projectile_x_vel[v1] & 0xFF00) >> 8));
  uint16 v2 = SignExtend8(LOBYTE(enemy_projectile_x_vel[v1]) << 8);
  bool v3 = __CFADD__uint16(enemy_projectile_1A27[v1], v2);
  uint16 v4 = enemy_projectile_1A27[v1] + v2;
  if (v3)
    ++enemy_projectile_x_pos[v1];
  enemy_projectile_1A27[v1] = v4;
}

void EprojPreInstr_NorfairLavaquakeRocks_Inner(uint16 k) {  // 0x86BD1E
  if (EprojPreInstr_NorfairLavaquakeRocks_Inner2(k))
    enemy_projectile_id[k >> 1] = 0;
}

uint16 EprojPreInstr_NorfairLavaquakeRocks_Inner2(uint16 k) {  // 0x86BD2A
  int v1 = k >> 1;
  return (int16)(enemy_projectile_x_pos[v1] - layer1_x_pos) < 0
    || (int16)(layer1_x_pos + 256 - enemy_projectile_x_pos[v1]) < 0
    || (int16)(enemy_projectile_y_pos[v1] - layer1_y_pos) < 0
    || (int16)(layer1_y_pos + 256 - enemy_projectile_y_pos[v1]) < 0;
}

static const int16 kEprojInit_ShaktoolAttackMiddleBackCircle_X[8] = { 0, 12, 16, 12, 0, -12, -16, -12 };
static const int16 kEprojInit_ShaktoolAttackMiddleBackCircle_Y[8] = { -16, -12, 0, 12, 16, 12, 0, -12 };

void EprojInit_ShaktoolAttackMiddleBackCircle(uint16 j) {  // 0x86BD9C
  enemy_projectile_E[j >> 1] = enemy_projectile_init_param;
  EprojInit_BDA2(j);
}

void EprojInit_BDA2(uint16 j) {  // 0x86BDA2
  int v1 = j >> 1;
  EnemyData *v2 = gEnemyData(cur_enemy_index);
  enemy_projectile_x_pos[v1] = v2->x_pos;
  enemy_projectile_y_pos[v1] = v2->y_pos;
  uint16 v3 = 2 * LOBYTE(v2->ai_var_D);
  int v4 = v3 >> 1;
  enemy_projectile_x_vel[v1] = kSinCosTable8bit_Sext[v4 + 64];
  enemy_projectile_y_vel[v1] = kSinCosTable8bit_Sext[v4];
  int v5 = (uint16)(v3 >> 5) >> 1;
  enemy_projectile_x_pos[v1] += kEprojInit_ShaktoolAttackMiddleBackCircle_X[v5];
  enemy_projectile_y_pos[v1] += kEprojInit_ShaktoolAttackMiddleBackCircle_Y[v5];
}

void EprojInit_ShaktoolAttackFrontCircle(uint16 v1) {  // 0x86BE03
  if (EnemyProjectileBlockCollisition_Horiz(v1) & 1 || EnemyProjectileBlockCollisition_Vertical(v1) & 1)
    enemy_projectile_id[v1 >> 1] = 0;
}

void EprojPreInstr_BE12(uint16 k) {  // 0x86BE12
  int v1 = k >> 1;
  if (enemy_projectile_id[enemy_projectile_E[v1] >> 1]) {
    EnemyProjectileBlockCollisition_Horiz(k);
    EnemyProjectileBlockCollisition_Vertical(k);
  } else {
    enemy_projectile_id[v1] = 0;
  }
}

#define kEproj_MotherBrainRoomTurrets_DirectionIndexes ((uint16*)RomPtr(0x86bee1))
#define kEproj_MotherBrainRoomTurrets_AllowedRotations ((uint16*)RomPtr(0x86bec9))
#define kEproj_MotherBrainRoomTurrets_InstrLists ((uint16*)RomPtr(0x86beb9))
static const int16 kEprojInit_MotherBrainRoomTurrets_X[12] = { 0x398, 0x348, 0x328, 0x2d8, 0x288, 0x268, 0x218, 0x1c8, 0x1a8, 0x158, 0x108, 0xe8 };
static const int16 EprojInit_MotherBrainRoomTurrets_Y[12] = { 0x30, 0x40, 0x40, 0x30, 0x40, 0x40, 0x30, 0x40, 0x40, 0x30, 0x40, 0x40 };

void EprojInit_MotherBrainRoomTurrets(uint16 j) {  // 0x86BE4F
  int v1 = j >> 1;
  enemy_projectile_gfx_idx[v1] = 1024;
  uint16 v2 = 2 * enemy_projectile_init_param;
  uint16 v3 = kEproj_MotherBrainRoomTurrets_DirectionIndexes[enemy_projectile_init_param] | 0x100;
  enemy_projectile_y_subpos[v1] = v3;
  enemy_projectile_instr_list_ptr[v1] = kEproj_MotherBrainRoomTurrets_InstrLists[(uint16)(2
                                                                                          * (uint8)v3) >> 1];
  int v4 = v2 >> 1;
  enemy_projectile_x_pos[v1] = kEprojInit_MotherBrainRoomTurrets_X[v4];
  enemy_projectile_y_pos[v1] = EprojInit_MotherBrainRoomTurrets_Y[v4];
  enemy_projectile_1A27[v1] = kEproj_MotherBrainRoomTurrets_AllowedRotations[v4];
  Eproj_SetXvelRandom(j);
  Eproj_SetYvelRandom(j);
}

static const int16 kEproj_MotherBrainRoomTurretBullets_X[8] = { -17, -12, 0, 12, 17, 12, 0, -12 };
static const int16 kEproj_MotherBrainRoomTurretBullets_Y[8] = { -9, 3, 7, 3, -9, -19, -21, -19 };
static const int16 kEproj_MotherBrainRoomTurretBullets_Xvel[8] = { -704, -498, 0, 498, 704, 498, 0, -498 };
static const int16 kEproj_MotherBrainRoomTurretBullets_Yvel[8] = { 0, 498, 704, 498, 0, -498, -704, -498 };

void EprojInit_MotherBrainRoomTurretBullets(uint16 j) {  // 0x86BF59
  int v1 = j >> 1;
  enemy_projectile_F[v1] = 0;
  enemy_projectile_gfx_idx[v1] = 1024;
  uint16 v2 = 2 * LOBYTE(enemy_projectile_y_subpos[enemy_projectile_init_param >> 1]);
  enemy_projectile_E[v1] = v2;
  int v3 = v2 >> 1;
  R18_ = kEproj_MotherBrainRoomTurretBullets_X[v3];
  R20_ = kEproj_MotherBrainRoomTurretBullets_Y[v3];
  enemy_projectile_x_vel[v1] = kEproj_MotherBrainRoomTurretBullets_Xvel[v3];
  enemy_projectile_y_vel[v1] = kEproj_MotherBrainRoomTurretBullets_Yvel[v3];
  int v4 = enemy_projectile_init_param >> 1;
  enemy_projectile_x_pos[v1] = R18_ + enemy_projectile_x_pos[v4];
  enemy_projectile_y_pos[v1] = R20_ + enemy_projectile_y_pos[v4];
}

#define g_off_86C040 ((uint16*)RomPtr(0x86c040))

void EprojPreInstr_MotherBrainRoomTurrets(uint16 k) {  // 0x86BFDF
  if (Eproj_MotherBrainRoomTurretBullets_CheckIfTurretOnScreen(k) & 1) {
    if (gRam7800_Default(0)->var_1D)
      *(uint16 *)((char *)enemy_projectile_id + k) = 0;
  } else if (gRam7800_Default(0)->var_1D) {
    *(uint16 *)((char *)enemy_projectile_id + k) = 0;
    int v5 = k >> 1;
    R18_ = enemy_projectile_x_pos[v5];
    R20_ = enemy_projectile_y_pos[v5];
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 0xCu);
  } else {
    int v1 = k >> 1;
    bool v2 = enemy_projectile_x_vel[v1]-- == 1;
    if (v2) {
      Eproj_SetXvelRandom(k);
      Eproj_MotherBrainRoomTurretBullets_Func2(k);
      int v3 = k >> 1;
      enemy_projectile_instr_list_ptr[v3] = g_off_86C040[(uint16)(2 * LOBYTE(enemy_projectile_y_subpos[v3])) >> 1];
      enemy_projectile_instr_timers[v3] = 1;
    }
    int v4 = k >> 1;
    v2 = enemy_projectile_y_vel[v4]-- == 1;
    if (v2) {
      Eproj_SetYvelRandom(k);
      SpawnEnemyProjectileWithRoomGfx(addr_kEproj_MotherBrainRoomTurretBullets, k);
    }
  }
}

void Eproj_MotherBrainRoomTurretBullets_Func2(uint16 v0) {  // 0x86C050
  char v1;

  LOBYTE(R20_) = (*((uint8 *)enemy_projectile_y_subpos + v0 + 1) + *((uint8 *)enemy_projectile_y_subpos + v0)) & 7;
  R18_ = enemy_projectile_1A27[v0 >> 1];
  if (RomPtr_86(R18_)[R20_ & 7]) {
    *((uint8 *)enemy_projectile_y_subpos + v0) = R20_;
  } else {
    v1 = -*((uint8 *)enemy_projectile_y_subpos + v0 + 1);
    *((uint8 *)enemy_projectile_y_subpos + v0 + 1) = v1;
    *((uint8 *)enemy_projectile_y_subpos + v0) += v1;
  }
}

void Eproj_SetXvelRandom(uint16 v0) {  // 0x86C08E
  uint16 Random = (uint8)NextRandom();
  if (sign16((uint8)Random - 32))
    Random = 32;
  enemy_projectile_x_vel[v0 >> 1] = Random;
}

void Eproj_SetYvelRandom(uint16 v0) {  // 0x86C0A1
  uint16 Random = (uint8)NextRandom();
  if (sign16((uint8)Random - 128))
    Random = 128;
  enemy_projectile_y_vel[v0 >> 1] = Random;
}

uint8 Eproj_MotherBrainRoomTurretBullets_CheckIfTurretOnScreen(uint16 k) {  // 0x86C0B4
  int16 v2;
  int16 v3;
  int16 v4;
  int16 v5;

  int v1 = k >> 1;
  v2 = enemy_projectile_y_pos[v1];
  uint8 result = 1;
  if (v2 >= 0) {
    v3 = v2 + 16 - layer1_y_pos;
    if (v3 >= 0) {
      if (sign16(v3 - 256)) {
        v4 = enemy_projectile_x_pos[v1];
        if (v4 >= 0) {
          v5 = v4 + 4 - layer1_x_pos;
          if (v5 >= 0) {
            if (sign16(v5 - 264))
              return 0;
          }
        }
      }
    }
  }
  return result;
}

void EprojPreInstr_MotherBrainRoomTurretBullets(uint16 k) {  // 0x86C0E0
  int v1 = k >> 1;
  enemy_projectile_properties[v1] ^= 0x8000u;
  MoveEprojWithVelocity(k);
  if (Ridley_Func_103(enemy_projectile_x_pos[v1], enemy_projectile_y_pos[v1]) & 1)
    enemy_projectile_id[enemy_projectile_index >> 1] = 0;
}

static const int16 kMotherBrainsBomb_Yaccel[10] = { 7, 0x10, 0x20, 0x40, 0x70, 0xb0, 0xf0, 0x130, 0x170, 0 };

void EprojPreInstr_MotherBrainBomb(uint16 k) {  // 0x86C4C8
  int16 v2;

  if (MotherBrainBomb_Bomb_CollDetect_DoubleRet(k))
    return;
  int v1 = k >> 1;
  if (enemy_projectile_F[v1]) {
    int v4 = k >> 1;
    uint16 v5 = kMotherBrainsBomb_Yaccel[enemy_projectile_F[v4] >> 1];
    if (!v5) {
      enemy_projectile_x_vel[v4] = 0;
      enemy_projectile_y_vel[v4] = 0;
      --enemy_ram7800[1].kraid.kraid_mouth_flags;
      enemy_projectile_id[v4] = 0;
      R18_ = enemy_projectile_x_pos[v4];
      R20_ = enemy_projectile_y_pos[v4];
      SpawnEnemyProjectileWithRoomGfx(addr_stru_869650, LOBYTE(enemy_projectile_1A27[v4]));
      R18_ = enemy_projectile_x_pos[v4];
      R20_ = enemy_projectile_y_pos[v4];
      SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 3u);
      QueueSfx3_Max6(0x13u);
      return;
    }
    if (MoveMotherBrainBomb(k, v5) & 1)
      goto LABEL_5;
  } else {
    v2 = abs16(enemy_projectile_x_vel[v1]) - 2;
    if (v2 < 0)
      v2 = 0;
    enemy_projectile_x_vel[v1] = sign16(enemy_projectile_x_vel[v1]) ? -v2 : v2;
    if (MoveMotherBrainBomb(k, 7u) & 1) {
LABEL_5:;
      int v3 = k >> 1;
      ++enemy_projectile_F[v3];
      ++enemy_projectile_F[v3];
    }
  }
}

uint16 EprojInstr_SwitchJump(uint16 k, uint16 j) {  // 0x86C173
  return *(uint16 *)RomPtr_86(enemy_projectile_E[k >> 1] + j);
}

uint16 EprojInstr_UserPalette0(uint16 k, uint16 j) {  // 0x86C1B4
  enemy_projectile_gfx_idx[k >> 1] = 0;
  return j;
}

uint8 Eproj_CheckForBombCollisionWithRect(void) {  // 0x86C1B8
  if (!bomb_counter)
    return 0;
  uint16 v1 = 10;
  while (1) {
    int v2 = v1 >> 1;
    if ((projectile_type[v2] & 0xF00) == 1280 && !projectile_variables[v2]) {
      uint16 v3 = abs16(projectile_x_pos[v2] - R18_);
      bool v4 = v3 < projectile_x_radius[v2];
      uint16 v5 = v3 - projectile_x_radius[v2];
      if (v4 || v5 < R22_) {
        uint16 v6 = abs16(projectile_y_pos[v2] - R20_);
        v4 = v6 < projectile_y_radius[v2];
        uint16 v7 = v6 - projectile_y_radius[v2];
        if (v4 || v7 < R24_)
          break;
      }
    }
    v1 += 2;
    if ((int16)(v1 - 20) >= 0)
      return 0;
  }
  return 1;
}

uint8 Eproj_CheckForEnemyCollisionWithRect(uint16 k) {  // 0x86C209
  EnemyData *v1 = gEnemyData(k);
  uint16 v2 = abs16(v1->x_pos - R18_);
  bool v3 = v2 < v1->x_width;
  uint16 v4 = v2 - v1->x_width;
  uint8 result = 0;
  if (v3 || v4 < R22_) {
    uint16 v5 = abs16(v1->y_pos - R20_);
    v3 = v5 < v1->y_height;
    uint16 v6 = v5 - v1->y_height;
    if (v3 || v6 < R24_)
      return 1;
  }
  return result;
}

uint8 Eproj_CheckForCollisionWithSamus(uint16 v0) {  // 0x86C239
  Eproj_InitForCollDetect(v0);
  uint16 v1 = abs16(samus_x_pos - R18_);
  bool v2 = v1 < samus_x_radius;
  uint16 v3 = v1 - samus_x_radius;
  uint8 result = 0;
  if (v2 || v3 < R22_) {
    uint16 v4 = abs16(samus_y_pos - R20_);
    v2 = v4 < samus_y_radius;
    uint16 v5 = v4 - samus_y_radius;
    if (v2 || v5 < R24_)
      return 1;
  }
  return result;
}

uint16 Math_MultBySin(uint16 a) {  // 0x86C26C
  R38 = a;
  return Math_MultBySinCos(R18_);
}

uint16 Math_MultByCos(uint16 a) {  // 0x86C272
  R38 = a;
  return Math_MultBySinCos(R18_ + 64);
}

uint16 Math_MultBySinCos(uint16 a) {  // 0x86C27A
  R46 = kSinCosTable8bit_Sext[((uint16)((2 * a) & 0x1FE) >> 1) + 64];
  R40 = abs16(R46);
  Math_Mult16U();
  uint16 r = *(uint16 *)((char *)&R42 + 1);
  return sign16(R46) ? -r : r;
}


void Math_Mult16U(void) {  // 0x86C29B
  uint32 t = (uint32)R38 * (uint32)R40;
  R42 = t;
  R44 = (t >> 16);
}

void Eproj_MotherBrainsBlueRingLasers(uint16 j) {  // 0x86C2F3
  int v1 = j >> 1;
  enemy_projectile_E[v1] = 8;
  enemy_projectile_F[v1] = 0;
  enemy_projectile_gfx_idx[v1] = 1024;
  R18_ = enemy_projectile_init_param;
  enemy_projectile_x_vel[v1] = Math_MultBySin(0x450u);
  enemy_projectile_y_vel[v1] = Math_MultByCos(0x450u);
  enemy_projectile_x_pos[v1] = enemy_data[1].x_pos + 10;
  enemy_projectile_y_pos[v1] = enemy_data[1].y_pos + 16;
  sub_86C320(j);
}

void sub_86C320(uint16 k) {  // 0x86C320
  int v1 = k >> 1;
  enemy_projectile_x_pos[v1] = enemy_data[1].x_pos + 10;
  enemy_projectile_y_pos[v1] = enemy_data[1].y_pos + 16;
}

void Eproj_MoveToBlueRingSpawnPosition(uint16 k) {  // 0x86C335
  int16 v5;

  int v1 = k >> 1;
  if (enemy_projectile_E[v1]) {
    --enemy_projectile_E[v1];
    sub_86C320(k);
  } else {
    MoveEprojWithVelocity(k);
    uint8 t = CheckForCollisionWithShitroid_DoubleRet(k);
    if (t & 0x80)
      return;
    if (t) {
      ++enemy_ram7800[0].kraid.field_28;
      BlueRingContactExplosion(k);
      uint16 v3 = enemy_ram7800[1].kraid.kraid_healths_8ths[4];
      gExtraEnemyRam7800(enemy_ram7800[1].kraid.kraid_healths_8ths[4])->kraid.kraid_healths_8ths[0] = 16;
      EnemyData *v4 = gEnemyData(v3);
      v5 = v4->health - 80;
      if (v5 < 0)
        v5 = 0;
      v4->health = v5;
    } else if (Eproj_CheckForCollisionWithSamus(k) & 1) {
      BlueRingContactExplosion(k);
      uint16 v2 = SuitDamageDivision(0x50u);
      Samus_DealDamage(v2);
      samus_invincibility_timer = 96;
      samus_knockback_timer = 5;
      knockback_x_dir = (int16)(samus_x_pos - enemy_projectile_x_pos[k >> 1]) >= 0;
    } else if (CheckForBlueRingCollisionWithRoom(k) & 1) {
      Eproj_Earthqhake5(k);
    }
  }
}

uint8 CheckForCollisionWithShitroid_DoubleRet(uint16 v0) {  // 0x86C3A9
  if (!enemy_ram7800[1].kraid.kraid_healths_8ths[4])
    return 0;
  if (gEnemyData(enemy_ram7800[1].kraid.kraid_healths_8ths[4])->health) {
    Eproj_InitForCollDetect(v0);
    return Eproj_CheckForEnemyCollisionWithRect(enemy_ram7800[1].kraid.kraid_healths_8ths[4]);
  }
  enemy_projectile_id[v0 >> 1] = 0;
  return 0xff;
}

uint8 CheckForBlueRingCollisionWithRoom(uint16 k) {  // 0x86C3C9
  int16 v2;
  int16 v3;

  int v1 = k >> 1;
  uint8 result = 1;
  if (!sign16(enemy_projectile_y_pos[v1] - 32) && enemy_projectile_y_pos[v1] < 0xD8u) {
    v2 = enemy_projectile_x_pos[v1];
    if (v2 >= 0) {
      v3 = v2 - layer1_x_pos;
      if (v3 >= 0) {
        if (sign16(v3 - 248))
          return 0;
      }
    }
  }
  return result;
}

void Eproj_InitForCollDetect(uint16 k) {  // 0x86C3E9
  int v1 = k >> 1;
  R18_ = enemy_projectile_x_pos[v1];
  R20_ = enemy_projectile_y_pos[v1];
  R22_ = LOBYTE(enemy_projectile_radius[v1]);
  R24_ = HIBYTE(enemy_projectile_radius[v1]);
}

void Eproj_Earthqhake5(uint16 k) {  // 0x86C404
  earthquake_timer = 10;
  earthquake_type = 5;
  BlueRingContactExplosion(k);
}

void BlueRingContactExplosion(uint16 k) {  // 0x86C410
  int v1 = k >> 1;
  enemy_projectile_id[v1] = 0;
  R18_ = enemy_projectile_x_pos[v1];
  R20_ = enemy_projectile_y_pos[v1];
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 3u);
  QueueSfx3_Max6(0x13u);
  sub_86C42E(k);
}

void sub_86C42E(uint16 k) {  // 0x86C42E
  enemy_projectile_gfx_idx[k >> 1] = 0;
}

void EprojInit_MotherBrainBomb(uint16 j) {  // 0x86C482
  *((uint8 *)enemy_projectile_1A27 + j) = enemy_projectile_init_param;
  int v1 = j >> 1;
  enemy_projectile_y_vel[v1] = 256;
  enemy_projectile_x_vel[v1] = 224;
  enemy_projectile_x_pos[v1] = enemy_data[1].x_pos + 12;
  enemy_projectile_y_pos[v1] = enemy_data[1].y_pos + 16;
  enemy_projectile_gfx_idx[v1] = 1024;
  enemy_projectile_E[v1] = 112;
  enemy_projectile_F[v1] = 0;
  ++enemy_ram7800[1].kraid.kraid_mouth_flags;
}

uint8 MotherBrainBomb_Bomb_CollDetect_DoubleRet(uint16 k) {  // 0x86C564
  int v1 = k >> 1;
  R18_ = enemy_projectile_x_pos[v1];
  R20_ = enemy_projectile_y_pos[v1];
  R22_ = LOBYTE(enemy_projectile_radius[v1]);
  R24_ = HIBYTE(enemy_projectile_radius[v1]);
  if (!Eproj_CheckForBombCollisionWithRect())
    return 0;

  --enemy_ram7800[1].kraid.kraid_mouth_flags;
  enemy_projectile_x_vel[v1] = 0;
  enemy_projectile_y_vel[v1] = 0;
  enemy_projectile_id[v1] = 0;
  R18_ = enemy_projectile_x_pos[v1];
  R20_ = enemy_projectile_y_pos[v1];
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 9u);
  R18_ = enemy_projectile_x_pos[v1];
  R20_ = enemy_projectile_y_pos[v1];
  SpawnEnemyDrops(addr_kEnemyDef_EC3F, k);
  return 1;  // double return
}

uint8 MoveMotherBrainBomb(uint16 k, uint16 a) {  // 0x86C5C2
  int v2 = k >> 1;
  enemy_projectile_y_vel[v2] += a;
  MoveEprojWithVelocity(k);
  if (!sign16(enemy_projectile_x_pos[v2] - 240))
    enemy_projectile_x_vel[v2] = -enemy_projectile_x_vel[v2];
  if (sign16(enemy_projectile_y_pos[v2] - 208))
    return 0;
  enemy_projectile_y_pos[v2] = 208;
  enemy_projectile_x_vel[v2] = sign16(enemy_projectile_x_vel[v2]) ? -enemy_projectile_E[v2] : enemy_projectile_E[v2];
  enemy_projectile_y_vel[v2] = -512;
  return 1;
}

void sub_86C605(uint16 j) {  // 0x86C605
  int16 v2;
  int16 v3;

  int v1 = j >> 1;
  enemy_projectile_E[v1] = 0;
  enemy_projectile_F[v1] = 0;
  enemy_projectile_x_vel[v1] = 0;
  enemy_projectile_y_vel[v1] = 0;
  enemy_projectile_1A27[v1] = 0;
  enemy_projectile_y_subpos[v1] = 0;
  *(uint16 *)&extra_enemy_ram8000[0].pad[6] = 0;
  *(uint16 *)&extra_enemy_ram8000[0].pad[10] = 0;
  enemy_projectile_gfx_idx[v1] = 1024;
  v2 = enemy_data[0].x_pos + 64;
  enemy_projectile_x_pos[v1] = enemy_data[0].x_pos + 64;
  *(uint16 *)&extra_enemy_ram8000[0].pad[8] = v2;
  R18_ = samus_x_pos - v2;
  v3 = enemy_data[0].y_pos - 48;
  enemy_projectile_y_pos[v1] = enemy_data[0].y_pos - 48;
  *(uint16 *)&extra_enemy_ram8000[0].pad[12] = v3;
  R20_ = samus_y_pos - v3;
  R18_ = (uint8)-(int8)(CalculateAngleFromXY() + 0x80);
  *(uint16 *)&extra_enemy_ram8000[0].pad[18] = R18_;
  *(uint16 *)&extra_enemy_ram8000[0].pad[14] = Math_MultBySin(0xC00u);
  *(uint16 *)&extra_enemy_ram8000[0].pad[16] = Math_MultByCos(0xC00u);
}

void EprojInit_MotherBrainDeathBeemFired(uint16 j) {  // 0x86C684
  int v1 = j >> 1;
  enemy_projectile_x_pos[v1] = *(uint16 *)&extra_enemy_ram8000[0].pad[8];
  enemy_projectile_1A27[v1] = *(uint16 *)&extra_enemy_ram8000[0].pad[6];
  enemy_projectile_y_pos[v1] = *(uint16 *)&extra_enemy_ram8000[0].pad[12];
  enemy_projectile_y_subpos[v1] = *(uint16 *)&extra_enemy_ram8000[0].pad[10];
  enemy_projectile_x_vel[v1] = *(uint16 *)&extra_enemy_ram8000[0].pad[14];
  enemy_projectile_y_vel[v1] = *(uint16 *)&extra_enemy_ram8000[0].pad[16];
  MoveEprojWithVelocity(j);
  *(uint16 *)&extra_enemy_ram8000[0].pad[8] = enemy_projectile_x_pos[v1];
  *(uint16 *)&extra_enemy_ram8000[0].pad[6] = enemy_projectile_1A27[v1];
  *(uint16 *)&extra_enemy_ram8000[0].pad[12] = enemy_projectile_y_pos[v1];
  *(uint16 *)&extra_enemy_ram8000[0].pad[10] = enemy_projectile_y_subpos[v1];
  R18_ = (uint8)(extra_enemy_ram8000[0].pad[18] + NextRandom());
  uint16 rv = NextRandom();
  enemy_projectile_x_vel[v1] = Math_MultBySin(rv & 0x700);
  enemy_projectile_y_vel[v1] = Math_MultByCos(random_number & 0x700);
  MoveEprojWithVelocity(j);
  if (sign16(enemy_projectile_y_pos[v1] - 34)
      || !sign16(enemy_projectile_y_pos[v1] - 206)
      || sign16(enemy_projectile_x_pos[v1] - 2)
      || !sign16(enemy_projectile_x_pos[v1] - 238)) {
    enemy_projectile_id[v1] = 0;
    R18_ = enemy_projectile_x_pos[v1];
    R20_ = enemy_projectile_y_pos[v1];
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 0x1Du);
    QueueSfx3_Max6(0x13u);
    earthquake_timer = 10;
    earthquake_type = 5;
  } else {
    enemy_projectile_E[v1] = ((uint8)enemy_projectile_E[v1] + 1) & 3;
    enemy_projectile_F[v1] = 0;
    enemy_projectile_x_vel[v1] = 0;
    enemy_projectile_y_vel[v1] = 0;
  }
}

void SpawnMotherBrainDeathBeam(uint16 x) {  // 0x86C7FB
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_MotherBrainDeathBeamFired, enemy_projectile_E[x >> 1]);
}

void EprojInit_MotherBrainRainbowBeam(uint16 j) {  // 0x86C80A
  int v1 = j >> 1;
  enemy_projectile_gfx_idx[v1] = 0;
  enemy_projectile_x_vel[v1] = 0;
  enemy_projectile_y_vel[v1] = 0;
  EprojPreInstr_MotherBrainRainbowBeam(j);
}

void EprojPreInstr_MotherBrainRainbowBeam(uint16 k) {  // 0x86C814
  int v2 = k >> 1;
  enemy_projectile_x_pos[v2] = enemy_data[1].x_pos;
  enemy_projectile_y_pos[v2] = enemy_data[1].y_pos;
}

static const int16 kEprojInit_MotherBrainsDrool[12] = { 6, 0x14, 0xe, 0x12, 8, 0x17, 0xa, 0x13, 0xb, 0x19, 0xc, 0x12 };

void EprojInit_MotherBrainsDrool(uint16 j) {  // 0x86C843
  int v1 = j >> 1;
  enemy_projectile_gfx_idx[v1] = 0;
  enemy_projectile_E[v1] = enemy_projectile_init_param;
  EprojPreInstr_C84D(j);
}

void EprojPreInstr_C84D(uint16 k) {  // 0x86C84D
  int v1 = k >> 1;
  int v2 = (uint16)(4 * enemy_projectile_E[v1]) >> 1;
  enemy_projectile_x_pos[v1] = enemy_data[1].x_pos + kEprojInit_MotherBrainsDrool[v2];
  enemy_projectile_y_pos[v1] = enemy_data[1].y_pos + kEprojInit_MotherBrainsDrool[v2 + 1];
  enemy_projectile_x_vel[v1] = 0;
  enemy_projectile_y_vel[v1] = 0;

}

void EprojInit_MotherBrainsDrool_Falling(uint16 k) {  // 0x86C886
  int v1 = k >> 1;
  enemy_projectile_y_vel[v1] += 12;
  if (MoveEprojWithVelocityY(k) >= 0xD7u) {
    enemy_projectile_y_pos[v1] -= 4;
    enemy_projectile_instr_list_ptr[v1] = addr_off_86C8E1;
    enemy_projectile_instr_timers[v1] = 1;
  }
}

uint16 EprojInstr_Add12ToY(uint16 k, uint16 j) {  // 0x86C8D0
  enemy_projectile_y_pos[k >> 1] += 12;
  return j;
}

void EprojInit_MotherBrainsDeathExplosion(uint16 j) {  // 0x86C8F5
  int v1 = j >> 1;
  enemy_projectile_instr_list_ptr[v1] = off_86C929[enemy_projectile_init_param];
  enemy_projectile_instr_timers[v1] = 1;
  enemy_projectile_gfx_idx[v1] = 0;
  enemy_projectile_x_vel[v1] = R18_;
  enemy_projectile_y_vel[v1] = R20_;
  EprojPreInit_MotherBrainsDeathExplosion_0(j);
}

void EprojPreInit_MotherBrainsDeathExplosion_0(uint16 k) {  // 0x86C914
  int v2 = k >> 1;
  enemy_projectile_x_pos[v2] = enemy_data[0].x_pos + enemy_projectile_x_vel[v2];
  enemy_projectile_y_pos[v2] = enemy_data[0].y_pos + enemy_projectile_y_vel[v2];
}

void EprojInit_MotherBrainsRainbowBeamExplosion(uint16 j) {  // 0x86C92F
  int v1 = j >> 1;
  enemy_projectile_gfx_idx[v1] = 0;
  uint16 v2 = R18_;
  enemy_projectile_x_vel[v1] = R18_;
  enemy_projectile_x_pos[v1] = samus_x_pos + v2;
  uint16 v3 = R20_;
  enemy_projectile_y_vel[v1] = R20_;
  enemy_projectile_y_pos[v1] = samus_y_pos + v3;
}

void EprojPreInstr_MotherBrainsRainbowBeamExplosion(uint16 k) {  // 0x86C94C
  int v1 = k >> 1;
  enemy_projectile_x_pos[v1] = samus_x_pos + enemy_projectile_x_vel[v1];
  enemy_projectile_y_pos[v1] = samus_y_pos + enemy_projectile_y_vel[v1];
}

void EprojInit_MotherBrainEscapeDoorParticles(uint16 j) {  // 0x86C961
  static const int16 kEprojInit_MotherBrainEscapeDoorParticles_X[16] = {
    0, -0x20, 0, -0x18, 0, -0x10, 0, -8, 0, 0, 0, 8,
    0,  0x10, 0,  0x18,
  };
  static const int16 kEprojInit_MotherBrainEscapeDoorParticles_Xvel[16] = {
    0x500, -0x200, 0x500, -0x100, 0x500, -0x100, 0x500, -0x80, 0x500, -0x80, 0x500, 0x80,
    0x500, -0x100, 0x500,  0x200,
  };


  int v1 = j >> 1;
  enemy_projectile_gfx_idx[v1] = 0;
  int v2 = (uint16)(4 * enemy_projectile_init_param) >> 1;
  enemy_projectile_x_pos[v1] = kEprojInit_MotherBrainEscapeDoorParticles_X[v2] + 16;
  enemy_projectile_y_pos[v1] = kEprojInit_MotherBrainEscapeDoorParticles_X[v2 + 1] + 128;
  enemy_projectile_x_vel[v1] = kEprojInit_MotherBrainEscapeDoorParticles_Xvel[v2];
  enemy_projectile_y_vel[v1] = kEprojInit_MotherBrainEscapeDoorParticles_Xvel[v2 + 1];
  enemy_projectile_E[v1] = 32;
}

void EprojPreInstr_MotherBrainsExplodedDoorParticles(uint16 k) {  // 0x86C9D2
  int16 v2;

  int v1 = k >> 1;
  v2 = abs16(enemy_projectile_x_vel[v1]) - 16;
  if (v2 < 0)
    v2 = 0;
  enemy_projectile_x_vel[v1] = sign16(enemy_projectile_x_vel[v1]) ? -v2 : v2;
  enemy_projectile_y_vel[v1] += 32;
  MoveEprojWithVelocity(k);
  if ((--enemy_projectile_E[v1] & 0x8000u) != 0) {
    enemy_projectile_id[v1] = 0;
    uint16 v4 = enemy_projectile_y_pos[v1] - 4;
    enemy_projectile_y_pos[v1] = v4;
    R20_ = v4;
    R18_ = enemy_projectile_x_pos[v1];
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 9u);
  }
}

void EprojInit_MotherBrainPurpleBreathBig(uint16 j) {  // 0x86CA6A
  int v1 = j >> 1;
  enemy_projectile_gfx_idx[v1] = 0;
  enemy_projectile_x_pos[v1] = enemy_data[1].x_pos + 6;
  enemy_projectile_y_pos[v1] = enemy_data[1].y_pos + 16;
}

void EprojInit_MotherBrainPurpleBreathSmall(uint16 j) {  // 0x86CA83
  int v1 = j >> 1;
  enemy_projectile_gfx_idx[v1] = 0;
  enemy_projectile_x_pos[v1] = enemy_data[1].x_pos + 6;
  enemy_projectile_y_pos[v1] = enemy_data[1].y_pos + 16;
  enemy_ram7800[1].kraid.kraid_hurt_frame = 1;
}

uint16 EprojInstr_MotherBrainPurpleBreathIsActive(uint16 k, uint16 j) {  // 0x86CAEE
  enemy_ram7800[1].kraid.kraid_hurt_frame = 0;
  return j;
}

void EprojInit_TimeBombSetJapaneseText(uint16 j) {  // 0x86CAF6
  enemy_projectile_gfx_idx[j >> 1] = 0;
  EprojPreInstr_TimeBombSetJapaneseText(j);
}

void EprojPreInstr_TimeBombSetJapaneseText(uint16 k) {  // 0x86CAFA
  int v1 = k >> 1;
  enemy_projectile_x_vel[v1] = 0;
  enemy_projectile_y_vel[v1] = 0;
  enemy_projectile_x_pos[v1] = 128;
  enemy_projectile_y_pos[v1] = 192;
}
void EprojInit_MotherBrainTubeFalling(uint16 j) {  // 0x86CBC9
  int v1 = j >> 1;
  enemy_projectile_gfx_idx[v1] = 3584;
  enemy_projectile_x_vel[v1] = 0;
  enemy_projectile_y_vel[v1] = 0;
  enemy_projectile_x_pos[v1] = R18_;
  enemy_projectile_y_pos[v1] = R20_;
  enemy_projectile_E[v1] = FUNC16(MotherBrainTubeFallingFunc_GenerateExplosion);
}

void CallMotherBrainTubeFallingFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnMotherBrainTubeFallingFunc_GenerateExplosion: MotherBrainTubeFallingFunc_GenerateExplosion(k); return;
  case fnMotherBrainTubeFallingFunc_Falling: MotherBrainTubeFallingFunc_Falling(k); return;
  default: Unreachable();
  }
}

void EprojPreInstr_MotherBrainTubeFalling(uint16 k) {  // 0x86CBE7
  CallMotherBrainTubeFallingFunc(enemy_projectile_E[k >> 1] | 0x860000, k);
}

void MotherBrainTubeFallingFunc_GenerateExplosion(uint16 k) {  // 0x86CBEA
  int v1 = k >> 1;
  R18_ = enemy_projectile_x_pos[v1];
  R20_ = enemy_projectile_y_pos[v1] + 8;
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 9u);
  enemy_projectile_E[v1] = FUNC16(MotherBrainTubeFallingFunc_Falling);
  MotherBrainTubeFallingFunc_Falling(k);
}

void MotherBrainTubeFallingFunc_Falling(uint16 k) {  // 0x86CC08
  int16 v2;

  int v1 = k >> 1;
  enemy_projectile_y_vel[v1] += 6;
  v2 = MoveEprojWithVelocityY(k);
  if (!sign16(v2 - 208)) {
    enemy_projectile_id[v1] = 0;
    R18_ = enemy_projectile_x_pos[v1];
    R20_ = enemy_projectile_y_pos[v1];
    SpawnEnemyProjectileWithRoomGfx(addr_kEproj_DustCloudExplosion, 0xCu);
  }
}

#define kEprojInit_MotherBrainGlassShatteringShard_InstrPtrs ((uint16*)RomPtr(0x86ce41))
static const int16 kEprojInit_MotherBrainGlassShatteringShard_X[3] = { 8, -40, -16 };
static const int16 kEprojInit_MotherBrainGlassShatteringShard_Y[3] = { 32, 32, 32 };

void EprojInit_MotherBrainGlassShatteringShard(uint16 j) {  // 0x86CDC5
  uint16 v1 = (2 * NextRandom()) & 0x1FE;
  int v2 = j >> 1;
  enemy_projectile_E[v2] = v1;
  int v3 = v1 >> 1;
  enemy_projectile_x_vel[v2] = kSinCosTable8bit_Sext[v3 + 64];
  enemy_projectile_y_vel[v2] = 4 * kSinCosTable8bit_Sext[v3];
  enemy_projectile_instr_list_ptr[v2] = kEprojInit_MotherBrainGlassShatteringShard_InstrPtrs[(uint16)((v1 >> 4) & 0x1E) >> 1];
  enemy_projectile_gfx_idx[v2] = 1600;
  CalculatePlmBlockCoords(plm_id);
  int v4 = enemy_projectile_init_param >> 1;
  enemy_projectile_x_pos[v2] = kEprojInit_MotherBrainGlassShatteringShard_X[v4] + 16 * plm_x_block;
  enemy_projectile_y_pos[v2] = kEprojInit_MotherBrainGlassShatteringShard_Y[v4] + 16 * plm_y_block;
  enemy_projectile_x_pos[v2] += (NextRandom() & 0xF) - 8;
  enemy_projectile_y_pos[v2] += (NextRandom() & 0xF) - 8;
}

void EprojInit_MotherBrainGlassShatteringSparkle(uint16 j) {  // 0x86CE6D
  int v1 = enemy_projectile_init_param >> 1;
  int v2 = j >> 1;
  enemy_projectile_x_pos[v2] = enemy_projectile_x_pos[v1] + (NextRandom() & 0x1F) - 16;
  enemy_projectile_y_pos[v2] = enemy_projectile_y_pos[v1] + (NextRandom() & 0x1F) - 16;
  enemy_projectile_gfx_idx[v2] = 1600;
}

void EprojPreInstr_MotherBrainGlassShatteringShard(uint16 k) {  // 0x86CE9B
  int16 v2;
  int16 v5;

  R18_ = 0;
  R20_ = 0;
  int v1 = k >> 1;
  v2 = enemy_projectile_x_vel[v1];
  if (v2 < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = v2;
  uint16 v3 = enemy_projectile_1A27[v1];
  bool v4 = __CFADD__uint16(R18_, v3);
  enemy_projectile_1A27[v1] = R18_ + v3;
  enemy_projectile_x_pos[v1] += R20_ + v4;
  R18_ = 0;
  R20_ = 0;
  v5 = enemy_projectile_y_vel[v1];
  if (v5 < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = v5;
  uint16 v6 = enemy_projectile_y_subpos[v1];
  v4 = __CFADD__uint16(R18_, v6);
  enemy_projectile_y_subpos[v1] = R18_ + v6;
  uint16 v7 = R20_ + v4 + enemy_projectile_y_pos[v1];
  enemy_projectile_y_pos[v1] = v7;
  if ((v7 & 0xFF00) != 0) {
    enemy_projectile_id[v1] = 0;
  } else {
    enemy_projectile_y_vel[v1] += 32;
    if ((NextRandom() & 0x420) == 0)
      SpawnEnemyProjectileWithRoomGfx(addr_kEproj_MotherBrainGlassShatteringSparkle, k);
  }
}

void EprojInit_KiHunterAcidSpitLeft(uint16 j) {  // 0x86CF90
  int v2 = j >> 1;
  enemy_projectile_x_vel[v2] = -768;
  enemy_projectile_x_pos[v2] = gEnemyData(cur_enemy_index)->x_pos - 22;
  EprojInit_KiHunterAcidSpitCommon(cur_enemy_index, j);
}

void EprojInit_KiHunterAcidSpitRight(uint16 j) {  // 0x86CFA6
  int v2 = j >> 1;
  enemy_projectile_x_vel[v2] = 768;
  enemy_projectile_x_pos[v2] = gEnemyData(cur_enemy_index)->x_pos + 22;
  EprojInit_KiHunterAcidSpitCommon(cur_enemy_index, j);
}

void EprojInit_KiHunterAcidSpitCommon(uint16 k, uint16 j) {  // 0x86CFBA
  int v2 = j >> 1;
  enemy_projectile_y_vel[v2] = 0;
  enemy_projectile_y_pos[v2] = gEnemyData(k)->y_pos - 16;
  enemy_projectile_y_subpos[v2] = 0;
  enemy_projectile_1A27[v2] = 0;
}

void sub_86CFD5(uint16 k) {  // 0x86CFD5
  int v1 = k >> 1;
  enemy_projectile_pre_instr[v1] = FUNC16(sub_86CFF8);
  enemy_projectile_x_pos[v1] -= 19;
}

void sub_86CFE6(uint16 k) {  // 0x86CFE6
  int v1 = k >> 1;
  enemy_projectile_pre_instr[v1] = FUNC16(sub_86CFF8);
  enemy_projectile_x_pos[v1] += 19;
}

void sub_86CFF8(uint16 k) {  // 0x86CFF8
  if (EnemyProjectileBlockCollisition_Vertical(k) & 1) {
    int v3 = k >> 1;
    enemy_projectile_instr_list_ptr[v3] = addr_off_86CF56;
    enemy_projectile_instr_timers[v3] = 1;
  } else if (EnemyProjectileBlockCollisition_Horiz(k) & 1) {
    enemy_projectile_x_vel[k >> 1] = 0;
  } else {
    int v1 = k >> 1;
    uint16 v2 = enemy_projectile_y_vel[v1] + 16;
    enemy_projectile_y_vel[v1] = v2;
    if (!sign16(v2 - 512))
      v2 = 512;
    enemy_projectile_y_vel[v1] = v2;
  }
}

void EprojInit_KagosBugs(uint16 j) {  // 0x86D088
  int v2 = j >> 1;
  enemy_projectile_F[v2] = cur_enemy_index;
  EnemyData *v3 = gEnemyData(cur_enemy_index);
  enemy_projectile_x_pos[v2] = v3->x_pos;
  enemy_projectile_y_pos[v2] = v3->y_pos;
  uint16 v4 = (random_number & 7) + 1;
  enemy_projectile_G[v2] = v4;
  enemy_projectile_E[v2] = v4 + 4;
  enemy_projectile_pre_instr[v2] = FUNC16(EprojPreInstr_KagosBugs);
}

void EprojPreInstr_KagosBugs_Func1(uint16 k) {  // 0x86D0B3
  int v1 = k >> 1;
  uint16 v2 = enemy_projectile_E[v1];
  if (v2) {
    uint16 v3 = v2 - 1;
    enemy_projectile_E[v1] = v3;
    if (!v3)
      QueueSfx2_Max6(0x6Cu);
  }
}

void EprojPreInstr_KagosBugs(uint16 k) {  // 0x86D0CA
  EprojPreInstr_KagosBugs_Func1(k);
  EprojPreInstr_KagosBugs_Func2(k);
  int v1 = k >> 1;
  if (enemy_projectile_G[v1]) {
    --enemy_projectile_G[v1];
  } else {
    enemy_projectile_instr_list_ptr[v1] = addr_word_86D052;
    enemy_projectile_instr_timers[v1] = 1;
    enemy_projectile_pre_instr[v1] = FUNC16(nullsub_302);
  }
}

static const uint16 word_86D082 = 0xe0;

void EprojPreInstr_D0EC(uint16 k) {  // 0x86D0EC
  EprojPreInstr_KagosBugs_Func1(k);
  EprojPreInstr_KagosBugs_Func2(k);
  if (EnemyProjectileBlockCollisition_Horiz(k)) {
    enemy_projectile_x_vel[k >> 1] = 0;
    goto LABEL_6;
  }
  if (EnemyProjectileBlockCollisition_Vertical(k)) {
LABEL_6:
    enemy_projectile_y_vel[k >> 1] = 256;
LABEL_7:;
    enemy_projectile_pre_instr[k >> 1] = FUNC16(EprojPreInstr_D128);
    enemy_projectile_instr_list_ptr[k >> 1] = addr_word_86D04A;
    enemy_projectile_instr_timers[k >> 1] = 1;
    return;
  }
  uint16 v2 = enemy_projectile_y_vel[k >> 1];
  bool v3 = (int16)(word_86D082 + v2) < 0;
  enemy_projectile_y_vel[k >> 1] = word_86D082 + v2;
  if (!v3) {
    goto LABEL_7;
  }
}

void EprojPreInstr_D128(uint16 v0) {  // 0x86D128
  EprojPreInstr_KagosBugs_Func1(v0);
  EprojPreInstr_KagosBugs_Func2(v0);
  if (EnemyProjectileBlockCollisition_Horiz(v0) & 1) {
    enemy_projectile_x_vel[v0 >> 1] = 0;
  } else if (EnemyProjectileBlockCollisition_Vertical(v0) & 1) {
    int v1 = v0 >> 1;
    enemy_projectile_pre_instr[v1] = FUNC16(nullsub_302);
    enemy_projectile_instr_list_ptr[v1] = addr_word_86D03C;
    enemy_projectile_instr_timers[v1] = 1;
  } else {
    enemy_projectile_y_vel[v0 >> 1] += word_86D082;
  }
}

static const uint16 g_word_86D086 = 0x200;
static const uint16 g_word_86D084 = 0x30;

uint16 EprojInstr_D15C(uint16 k, uint16 j) {  // 0x86D15C
  EprojPreInstr_KagosBugs_Func1(k);
  EprojPreInstr_KagosBugs_Func2(k);
  int v2 = k >> 1;

  uint16 t = (random_number & 0x300) + 2048;
  enemy_projectile_y_vel[v2] = -t;
  EnemyData *v3 = gEnemyData(enemy_projectile_F[v2]);
  uint16 t2 = v3->x_pos - enemy_projectile_x_pos[v2];

  if ((int16)(abs16(t2) - g_word_86D084) >= 0)
    t = sign16(t2) ? -1 : 0;

  uint16 v4 = g_word_86D086;
  if (t & 0x100)
    v4 = -v4;
  enemy_projectile_x_vel[v2] = v4;
  enemy_projectile_pre_instr[v2] = FUNC16(EprojPreInstr_D0EC);

  return j;
}

uint16 EprojInstr_D1B6(uint16 k, uint16 j) {  // 0x86D1B6
  int v1 = k >> 1;
  enemy_projectile_G[v1] = (random_number & 0x1F) + 1;
  enemy_projectile_pre_instr[v1] = FUNC16(EprojPreInstr_KagosBugs);

  return j;
}


uint16 EprojInstr_D1C7(uint16 v0, uint16 j) {  // 0x86D1C7
  enemy_projectile_gfx_idx[v0 >> 1] = 0;
  return j;
}

uint16 EprojInstr_D1CE(uint16 v0, uint16 j) {  // 0x86D1CE
  int v1 = v0 >> 1;
  R18_ = enemy_projectile_x_pos[v1];
  R20_ = enemy_projectile_y_pos[v1];
  SpawnEnemyDrops(addr_kEnemyDef_E7FF, v0);
  return j;
}

void EprojPreInstr_KagosBugs_Func2(uint16 k) {  // 0x86D1E4
  int v1 = k >> 1;
  EnemyData *v2 = gEnemyData(enemy_projectile_F[v1]);
  uint16 v3 = abs16(v2->x_pos - enemy_projectile_x_pos[v1]);
  if (!sign16(v3 - 23))
    enemy_projectile_properties[v1] |= 0x8000u;
}

void EprojInit_MaridiaFloatersSpikes(uint16 j) {  // 0x86D23A
  int v1 = j >> 1;
  enemy_projectile_1A27[v1] = 0;
  enemy_projectile_y_subpos[v1] = 0;
  enemy_projectile_x_vel[v1] = 0;
  enemy_projectile_y_vel[v1] = 0;
  EnemyData *v2 = gEnemyData(cur_enemy_index);
  enemy_projectile_x_pos[v1] = v2->x_pos;
  enemy_projectile_y_pos[v1] = v2->y_pos;
  enemy_projectile_E[v1] = enemy_projectile_init_param;
}

static const int16 word_86D21A[8] = { 0, 32, 32, 32, 0, -32, -32, -32 };
static const int16 word_86D22A[8] = { -32, -32, 0, 32, 32, 32, 0, -32 };

void EprojPreInstr_MaridiaFloatersSpikes(uint16 k) {  // 0x86D263
  int v1 = k >> 1;
  enemy_projectile_x_vel[v1] += word_86D21A[enemy_projectile_E[v1]];
  if (EnemyProjectileBlockCollisition_Horiz(k) & 1
      || (enemy_projectile_y_vel[v1] += word_86D22A[enemy_projectile_E[v1]],
          EnemyProjectileBlockCollisition_Vertical(k) & 1)) {
    enemy_projectile_instr_list_ptr[v1] = addr_off_86D218;
    enemy_projectile_instr_timers[v1] = 1;
  }
}

void EprojInit_WreckedShipRobotLaserDown(uint16 j) {  // 0x86D30C
  int v2 = j >> 1;
  enemy_projectile_x_vel[v2] = gEnemyData(cur_enemy_index)->ai_var_A;
  enemy_projectile_y_vel[v2] = 128;
  enemy_projectile_gfx_idx[v2] = 0;
  EprojInit_WreckedShipRobotLaserCommon(cur_enemy_index, j);
}

void EprojInit_WreckedShipRobotLaserHorizontal(uint16 j) {  // 0x86D32E
  int v2 = j >> 1;
  enemy_projectile_x_vel[v2] = gEnemyData(cur_enemy_index)->ai_var_A;
  enemy_projectile_y_vel[v2] = 0;
  EprojInit_WreckedShipRobotLaserCommon(cur_enemy_index, j);
}

void EprojInit_WreckedShipRobotLaserUp(uint16 j) {  // 0x86D341
  int v2 = j >> 1;
  enemy_projectile_x_vel[v2] = gEnemyData(cur_enemy_index)->ai_var_A;
  enemy_projectile_y_vel[v2] = -128;
  EprojInit_WreckedShipRobotLaserCommon(cur_enemy_index, j);
}

void EprojInit_WreckedShipRobotLaserCommon(uint16 k, uint16 j) {  // 0x86D35B
  int16 x_pos;
  uint16 v5;

  EnemyData *v2 = gEnemyData(k);
  int v3 = j >> 1;
  enemy_projectile_y_pos[v3] = v2->y_pos - 16;
  x_pos = v2->x_pos;
  if (sign16(enemy_projectile_x_vel[v3]))
    v5 = x_pos - 4;
  else
    v5 = x_pos + 4;
  enemy_projectile_x_pos[v3] = v5;
  enemy_projectile_y_subpos[v3] = 0;
  enemy_projectile_1A27[v3] = 0;
  if ((int16)(v2->x_width + v2->x_pos - layer1_x_pos) >= 0
      && (int16)(v2->x_pos - v2->x_width - 257 - layer1_x_pos) < 0
      && (int16)(v2->y_height + v2->y_pos - layer1_y_pos) >= 0
      && (int16)(v2->y_pos - v2->y_height - 224) < 0) {
    QueueSfx2_Max6(0x67u);
  }
}

void EprojPreInstr_WreckedShipRobotLaser(uint16 k) {  // 0x86D3BF
  int v1 = k >> 1;
  enemy_projectile_gfx_idx[v1] = 0;
  if (EnemyProjectileBlockCollisition_Horiz(k) & 1 || EnemyProjectileBlockCollisition_Vertical(k) & 1)
    enemy_projectile_id[v1] = 0;
}

uint16 EprojInstr_AssignNewN00bTubeShardVelocity(uint16 k, uint16 j) {  // 0x86D5E1
  NextRandom();
  int v2 = k >> 1;
  enemy_projectile_x_vel[v2] = *(uint16 *)((char *)&random_number + 1);
  enemy_projectile_y_vel[v2] = 192;
  return j;
}

uint16 EprojInstr_SetN00bTubeShardX(uint16 k, uint16 j) {  // 0x86D5F2
  int v2 = k >> 1;
  if (nmi_frame_counter_word & 1) {
    enemy_projectile_x_pos[v2] = enemy_projectile_F[v2];
    enemy_projectile_spritemap_ptr[v2] = *(uint16 *)RomPtr_86(j);
  } else {
    enemy_projectile_x_pos[v2] = 128 - enemy_projectile_F[v2] + 128;
    enemy_projectile_spritemap_ptr[v2] = *((uint16 *)RomPtr_86(j) + 1);
  }
  int v3 = k >> 1;
  enemy_projectile_instr_list_ptr[v3] = j + 4;
  enemy_projectile_instr_timers[v3] = 1;
  return 0;
}

uint16 EprojInstr_D62A(uint16 k, uint16 j) {  // 0x86D62A
  if (nmi_frame_counter_word & 1)
    enemy_projectile_x_pos[k >> 1] = enemy_projectile_F[k >> 1];
  else
    enemy_projectile_x_pos[k >> 1] = -4608;
  int v2 = k >> 1;
  enemy_projectile_spritemap_ptr[v2] = *(uint16 *)RomPtr_86(j);
  enemy_projectile_instr_list_ptr[v2] = j + 2;
  enemy_projectile_instr_timers[v2] = 1;
  return 0;
}

uint16 EprojInstr_SetXvelRandom(uint16 k, uint16 j) {  // 0x86D69A
  NextRandom();
  enemy_projectile_x_vel[k >> 1] = *(uint16 *)((char *)&random_number + 1);
  return j;
}

void EprojInit_N00bTubeCrack(uint16 j) {  // 0x86D6A5
  CalculatePlmBlockCoords(plm_id);
  int v1 = j >> 1;
  enemy_projectile_x_pos[v1] = 16 * plm_x_block + 96;
  enemy_projectile_y_pos[v1] = 16 * plm_y_block + 48;
}

static const int16 kEprojInit_N00bTubeShards_X[10] = { -56, -64, -20, -40, -64, -48, -24, -40, 0, -8 };
static const int16 kEprojInit_N00bTubeShards_Y[10] = { 8, -12, -26, -24, -32, 28, 16, -8, -24, 16 };
static const int16 kEprojInit_N00bTubeShards_Xvel[10] = { -384, -384, -160, -288, -288, -320, -96, -352, 0, -64 };
static const int16 kEprojInit_N00bTubeShards_Yvel[10] = { 320, -256, -416, -288, -288, 448, 576, -96, -288, 384 };
#define kEprojInit_N00bTubeShards_InstrPtrs ((uint16*)RomPtr(0x86d760))

void EprojInit_N00bTubeShards(uint16 j) {  // 0x86D6C9
  CalculatePlmBlockCoords(plm_id);
  int v1 = enemy_projectile_init_param >> 1;
  int v2 = j >> 1;
  enemy_projectile_F[v2] = kEprojInit_N00bTubeShards_X[v1] + 16 * plm_x_block + 96;
  enemy_projectile_E[v2] = 0;
  enemy_projectile_y_pos[v2] = kEprojInit_N00bTubeShards_Y[v1] + 16 * plm_y_block + 48;
  enemy_projectile_instr_list_ptr[v2] = kEprojInit_N00bTubeShards_InstrPtrs[v1];
  enemy_projectile_x_vel[v2] = kEprojInit_N00bTubeShards_Xvel[v1];
  enemy_projectile_y_vel[v2] = kEprojInit_N00bTubeShards_Yvel[v1];
}

static const uint16 kEprojInit_N00bTubeReleasedAirBubbles_X[6] = { 40, 80, 104, 120, 152, 184 };
static const uint16 kEprojInit_N00bTubeReleasedAirBubbles_Y[6] = { 80, 72, 84, 32, 64, 84 };

void EprojInit_N00bTubeReleasedAirBubbles(uint16 j) {  // 0x86D774
  CalculatePlmBlockCoords(plm_id);
  int v1 = enemy_projectile_init_param >> 1;
  int v2 = j >> 1;
  enemy_projectile_F[v2] = kEprojInit_N00bTubeReleasedAirBubbles_X[v1] + 16 * plm_x_block;
  enemy_projectile_E[v2] = 0;
  enemy_projectile_y_pos[v2] = kEprojInit_N00bTubeReleasedAirBubbles_Y[v1] + 16 * plm_y_block;
  enemy_projectile_y_vel[v2] = -1280;
}

void sub_86D7BF(uint16 k) {  // 0x86D7BF
  int v1 = k >> 1;
  if (enemy_projectile_x_pos[v1] != 0xEE00)
    enemy_projectile_E[v1] = enemy_projectile_x_pos[v1];
  if (nmi_frame_counter_word & 1)
    enemy_projectile_x_pos[v1] = -4608;
  else
    enemy_projectile_x_pos[v1] = enemy_projectile_E[v1];
}

void sub_86D7DE(uint16 k) {  // 0x86D7DE
  R18_ = 0;
  R20_ = 0;
  *(uint16 *)((char *)&R18_ + 1) = 192;
  int v1 = k >> 1;
  uint16 v2 = enemy_projectile_y_subpos[v1];
  bool v3 = __CFADD__uint16(R18_, v2);
  enemy_projectile_y_subpos[v1] = R18_ + v2;
  enemy_projectile_y_pos[v1] += R20_ + v3;
}

void EprojPreInstr_N00bTubeShards(uint16 k) {  // 0x86D7FD
  int16 v2;
  int16 v5;

  R18_ = 0;
  R20_ = 0;
  int v1 = k >> 1;
  v2 = enemy_projectile_x_vel[v1];
  if (v2 < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = v2;
  uint16 v3 = enemy_projectile_E[v1];
  bool v4 = __CFADD__uint16(R18_, v3);
  enemy_projectile_E[v1] = R18_ + v3;
  enemy_projectile_F[v1] += R20_ + v4;
  R18_ = 0;
  R20_ = 0;
  v5 = enemy_projectile_y_vel[v1];
  if (v5 < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = v5;
  uint16 v6 = enemy_projectile_y_subpos[v1];
  v4 = __CFADD__uint16(R18_, v6);
  enemy_projectile_y_subpos[v1] = R18_ + v6;
  enemy_projectile_y_pos[v1] += R20_ + v4;
  Eproj_DeleteIfYposOutside(k);
}

void sub_86D83D(uint16 k) {  // 0x86D83D
  int16 v8;

  uint16 v1 = enemy_projectile_x_vel[k >> 1] & 0x17E | 0x80;
  R18_ = 0;
  R20_ = 0;
  int v2 = v1 >> 1;
  uint16 v3 = kSinCosTable8bit_Sext[v2 + 64];
  if ((kSinCosTable8bit_Sext[v2 + 64] & 0x8000u) != 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = (int16)v3 >> 2;
  uint16 v5 = enemy_projectile_index;
  int v6 = enemy_projectile_index >> 1;
  uint16 v7 = enemy_projectile_E[v6];
  bool v4 = __CFADD__uint16(R18_, v7);
  enemy_projectile_E[v6] = R18_ + v7;
  enemy_projectile_F[v6] += R20_ + v4;
  enemy_projectile_x_vel[v6] += 2;
  R18_ = 0;
  R20_ = 0;
  v8 = enemy_projectile_y_vel[v6];
  if (v8 < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = v8;
  uint16 v9 = enemy_projectile_y_subpos[v6];
  v4 = __CFADD__uint16(R18_, v9);
  enemy_projectile_y_subpos[v6] = R18_ + v9;
  enemy_projectile_y_pos[v6] += R20_ + v4;
  Eproj_DeleteIfYposOutside(v5);
}

void sub_86D89F(uint16 k) {  // 0x86D89F
  uint16 v1 = enemy_projectile_x_vel[k >> 1] & 0x17E | 0x80;
  R18_ = 0;
  R20_ = 0;
  int v2 = v1 >> 1;
  uint16 v3 = kSinCosTable8bit_Sext[v2 + 64];
  if ((kSinCosTable8bit_Sext[v2 + 64] & 0x8000u) != 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = (int16)v3 >> 2;
  int v5 = enemy_projectile_index >> 1;
  uint16 v6 = enemy_projectile_E[v5];
  bool v4 = __CFADD__uint16(R18_, v6);
  enemy_projectile_E[v5] = R18_ + v6;
  enemy_projectile_F[v5] += R20_ + v4;
  enemy_projectile_x_vel[v5] += 4;
  sub_86D8DF(enemy_projectile_index);
}

void sub_86D8DF(uint16 k) {  // 0x86D8DF
  int v5 = k >> 1;
  int16 v7;
  R18_ = 0;
  R20_ = 0;
  v7 = enemy_projectile_y_vel[v5];
  if (v7 < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = v7;
  uint16 v8 = enemy_projectile_y_subpos[v5];
  bool v4 = __CFADD__uint16(R18_, v8);
  enemy_projectile_y_subpos[v5] = R18_ + v8;
  enemy_projectile_y_pos[v5] += R20_ + v4;
  enemy_projectile_x_pos[v5] = enemy_projectile_F[v5];
}

#define off_86D96A ((uint16*)RomPtr(0x86d96a))

void sub_86D992(uint16 v0) {  // 0x86D992
  uint16 v1 = enemy_projectile_init_param;
  int v2 = v0 >> 1;
  enemy_projectile_E[v2] = enemy_projectile_init_param;
  enemy_projectile_instr_list_ptr[v2] = off_86D96A[v1 >> 1];
  EnemyData *v3 = gEnemyData(cur_enemy_index);
  enemy_projectile_x_pos[v2] = v3->x_pos;
  enemy_projectile_1A27[v2] = v3->x_subpos;
  enemy_projectile_y_pos[v2] = v3->y_pos;
  enemy_projectile_y_subpos[v2] = v3->y_subpos;
  enemy_projectile_y_vel[v2] = -512;
  enemy_projectile_x_vel[v2] = 512;
  if (!sign16(enemy_projectile_init_param - 12)) {
    enemy_projectile_y_vel[v2] = -384;
    enemy_projectile_x_vel[v2] = 384;
  }
}

static Func_X_V *const kEprojPreInstr_SpikeShootingPlantSpikes[10] = {  // 0x86D9DB
  EprojPreInstr_SpikeShootingPlantSpikes_0_MoveX1,
  EprojPreInstr_SpikeShootingPlantSpikes_1_MoveY1,
  EprojPreInstr_SpikeShootingPlantSpikes_2_MoveX2,
  EprojPreInstr_SpikeShootingPlantSpikes_0_MoveX1,
  EprojPreInstr_SpikeShootingPlantSpikes_4_MoveY2,
  EprojPreInstr_SpikeShootingPlantSpikes_2_MoveX2,
  EprojPreInstr_SpikeShootingPlantSpikes_6_MoveX1Y1,
  EprojPreInstr_SpikeShootingPlantSpikes_7_MoveX2Y1,
  EprojPreInstr_SpikeShootingPlantSpikes_8_MoveX1Y2,
  EprojPreInstr_SpikeShootingPlantSpikes_9_MoveX2Y2,
};

void EprojPreInstr_SpikeShootingPlantSpikes(uint16 k) {
  kEprojPreInstr_SpikeShootingPlantSpikes[enemy_projectile_E[k >> 1] >> 1](k);
  EprojPreInstrHelper_SpikeShootingPlantSpikes_Func1(k);
}

void EprojPreInstr_SpikeShootingPlantSpikes_MoveY1(uint16 k) {  // 0x86D9E6
  int16 v2;

  int v1 = k >> 1;
  enemy_projectile_y_pos[v1] += SignExtend8((uint8)((uint16)(enemy_projectile_y_vel[v1] & 0xFF00) >> 8));
  v2 = LOBYTE(enemy_projectile_y_vel[v1]) << 8;
  bool v3 = __CFADD__uint16(enemy_projectile_y_subpos[v1], v2);
  enemy_projectile_y_subpos[v1] += v2;
  if (v3)
    ++enemy_projectile_y_pos[v1];
}

void EprojPreInstr_SpikeShootingPlantSpikes_MoveY2(uint16 k) {  // 0x86DA10
  int16 v2;

  int v1 = k >> 1;
  enemy_projectile_y_pos[v1] += SignExtend8((uint8)((uint16)(enemy_projectile_x_vel[v1] & 0xFF00) >> 8));
  v2 = LOBYTE(enemy_projectile_x_vel[v1]) << 8;
  bool v3 = __CFADD__uint16(enemy_projectile_y_subpos[v1], v2);
  enemy_projectile_y_subpos[v1] += v2;
  if (v3)
    ++enemy_projectile_y_pos[v1];
}

void EprojPreInstr_SpikeShootingPlantSpikes_MoveX1(uint16 k) {  // 0x86DA3A
  int16 v2;

  int v1 = k >> 1;
  enemy_projectile_x_pos[v1] += SignExtend8((uint8)((uint16)(enemy_projectile_y_vel[v1] & 0xFF00) >> 8));
  v2 = LOBYTE(enemy_projectile_y_vel[v1]) << 8;
  bool v3 = __CFADD__uint16(enemy_projectile_1A27[v1], v2);
  enemy_projectile_1A27[v1] += v2;
  if (v3)
    ++enemy_projectile_x_pos[v1];
}

void EprojPreInstr_SpikeShootingPlantSpikes_MoveX2(uint16 k) {  // 0x86DA64
  int16 v2;

  int v1 = k >> 1;
  enemy_projectile_x_pos[v1] += SignExtend8((uint8)((uint16)(enemy_projectile_x_vel[v1] & 0xFF00) >> 8));
  v2 = LOBYTE(enemy_projectile_x_vel[v1]) << 8;
  bool v3 = __CFADD__uint16(enemy_projectile_1A27[v1], v2);
  enemy_projectile_1A27[v1] += v2;
  if (v3)
    ++enemy_projectile_x_pos[v1];
}

void EprojPreInstr_SpikeShootingPlantSpikes_0_MoveX1(uint16 k) {  // 0x86DA8E
  EprojPreInstr_SpikeShootingPlantSpikes_MoveX1(k);
}

void EprojPreInstr_SpikeShootingPlantSpikes_2_MoveX2(uint16 k) {  // 0x86DA93
  EprojPreInstr_SpikeShootingPlantSpikes_MoveX2(k);
}

void EprojPreInstr_SpikeShootingPlantSpikes_1_MoveY1(uint16 k) {  // 0x86DA98
  EprojPreInstr_SpikeShootingPlantSpikes_MoveY1(k);
}

void EprojPreInstr_SpikeShootingPlantSpikes_4_MoveY2(uint16 k) {  // 0x86DA9D
  EprojPreInstr_SpikeShootingPlantSpikes_MoveY2(k);
}

void EprojPreInstr_SpikeShootingPlantSpikes_6_MoveX1Y1(uint16 k) {  // 0x86DAA2
  EprojPreInstr_SpikeShootingPlantSpikes_MoveX1(k);
  EprojPreInstr_SpikeShootingPlantSpikes_MoveY1(k);
}

void EprojPreInstr_SpikeShootingPlantSpikes_8_MoveX1Y2(uint16 k) {  // 0x86DAAA
  EprojPreInstr_SpikeShootingPlantSpikes_MoveX1(k);
  EprojPreInstr_SpikeShootingPlantSpikes_MoveY2(k);
}

void EprojPreInstr_SpikeShootingPlantSpikes_7_MoveX2Y1(uint16 k) {  // 0x86DAB2
  EprojPreInstr_SpikeShootingPlantSpikes_MoveX2(k);
  EprojPreInstr_SpikeShootingPlantSpikes_MoveY1(k);
}

void EprojPreInstr_SpikeShootingPlantSpikes_9_MoveX2Y2(uint16 k) {  // 0x86DABA
  EprojPreInstr_SpikeShootingPlantSpikes_MoveX2(k);
  EprojPreInstr_SpikeShootingPlantSpikes_MoveY2(k);
}

void EprojPreInstrHelper_SpikeShootingPlantSpikes_Func1(uint16 k) {  // 0x86DAC2
  if (EprojPreInstrHelper_SpikeShootingPlantSpikes_Func2(k))
    enemy_projectile_id[k >> 1] = 0;
}

uint16 EprojPreInstrHelper_SpikeShootingPlantSpikes_Func2(uint16 k) {  // 0x86DACE
  int v1 = k >> 1;
  return (int16)(enemy_projectile_x_pos[v1] - layer1_x_pos) < 0
    || (int16)(layer1_x_pos + 256 - enemy_projectile_x_pos[v1]) < 0
    || (int16)(enemy_projectile_y_pos[v1] - layer1_y_pos) < 0
    || (int16)(layer1_y_pos + 256 - enemy_projectile_y_pos[v1]) < 0;
}

void EprojInit_DBF2(uint16 j) {  // 0x86DB18
  int v2 = j >> 1;
  enemy_projectile_instr_list_ptr[v2] = addr_word_86DB0C;
  enemy_projectile_E[v2] = FUNC16(EprojPreInstr_DBF2_MoveX1);
  if (enemy_projectile_init_param)
    enemy_projectile_E[v2] = FUNC16(EprojPreInstr_DBF2_MoveX2);
  EnemyData *v3 = gEnemyData(cur_enemy_index);
  enemy_projectile_x_pos[v2] = v3->x_pos;
  enemy_projectile_1A27[v2] = v3->x_subpos;
  enemy_projectile_y_pos[v2] = v3->y_pos + 2;
  enemy_projectile_y_subpos[v2] = v3->y_subpos;
  enemy_projectile_y_vel[v2] = -256;
  enemy_projectile_x_vel[v2] = 256;
}

void EprojPreInstr_DBF2(uint16 k) {  // 0x86DB5B
  CallEprojPreInstr(enemy_projectile_E[k >> 1] | 0x860000, k);
  EprojPreInstr_DBF2_Func1(k);
}

void EprojPreInstr_DBF2_MoveX1(uint16 k) {  // 0x86DB62
  int16 v2;

  int v1 = k >> 1;
  enemy_projectile_x_pos[v1] += SignExtend8((uint8)((uint16)(enemy_projectile_y_vel[v1] & 0xFF00) >> 8));
  v2 = LOBYTE(enemy_projectile_y_vel[v1]) << 8;
  bool v3 = __CFADD__uint16(enemy_projectile_1A27[v1], v2);
  enemy_projectile_1A27[v1] += v2;
  if (v3)
    ++enemy_projectile_x_pos[v1];
}

void EprojPreInstr_DBF2_MoveX2(uint16 k) {  // 0x86DB8C
  int16 v2;

  int v1 = k >> 1;
  enemy_projectile_x_pos[v1] += SignExtend8((uint8)((uint16)(enemy_projectile_x_vel[v1] & 0xFF00) >> 8));
  v2 = LOBYTE(enemy_projectile_x_vel[v1]) << 8;
  bool v3 = __CFADD__uint16(enemy_projectile_1A27[v1], v2);
  enemy_projectile_1A27[v1] += v2;
  if (v3)
    ++enemy_projectile_x_pos[v1];
}

void EprojPreInstr_DBF2_Func1(uint16 k) {  // 0x86DBB6
  if (EprojPreInstrHelper_DBF2_Func2(k))
    enemy_projectile_id[k >> 1] = 0;
}

uint16 EprojPreInstrHelper_DBF2_Func2(uint16 k) {  // 0x86DBC2
  int v1 = k >> 1;
  return (int16)(enemy_projectile_x_pos[v1] - layer1_x_pos) < 0
    || (int16)(layer1_x_pos + 256 - enemy_projectile_x_pos[v1]) < 0
    || (int16)(enemy_projectile_y_pos[v1] - layer1_y_pos) < 0
    || (int16)(layer1_y_pos + 256 - enemy_projectile_y_pos[v1]) < 0;
}

uint16 EprojInstr_DC5A(uint16 k, uint16 j) {  // 0x86DC5A
  enemy_projectile_properties[k >> 1] = 12288;
  return j;
}

uint16 EprojInstr_SpawnEnemyDrops_0(uint16 k, uint16 j) {  // 0x86DC61
  int v2 = k >> 1;
  R18_ = enemy_projectile_x_pos[v2];
  R20_ = enemy_projectile_y_pos[v2];
  SpawnEnemyDrops(addr_kEnemyDef_DF7F, k);
  return j;
}

uint16 EprojInstr_SpawnSporesEproj(uint16 k, uint16 j) {  // 0x86DC77
  int v2 = k >> 1;
  R18_ = enemy_projectile_x_pos[v2];
  R20_ = enemy_projectile_y_pos[v2];
  SpawnEnemyProjectileWithRoomGfx(addr_kEproj_Spores, R20_);
  return j;
}

void EprojInit_Spores(uint16 j) {  // 0x86DC8D
  uint16 v1 = R18_;
  int v2 = j >> 1;
  enemy_projectile_x_pos[v2] = R18_;
  enemy_projectile_F[v2] = v1;
  enemy_projectile_y_pos[v2] = R20_;
  enemy_projectile_gfx_idx[v2] = 512;
}

static const int16 word_86DCB9[5] = { -64, -56, -48, -40, -32 };

void EprojInit_SporeSpawnStalk(uint16 j) {  // 0x86DCA3
  int v1 = j >> 1;
  enemy_projectile_y_pos[v1] = enemy_data[0].y_pos
    + word_86DCB9[enemy_projectile_init_param];
  enemy_projectile_x_pos[v1] = enemy_data[0].x_pos;
}

void sub_86DCC3(uint16 v0) {  // 0x86DCC3
  int v1 = v0 >> 1;
  enemy_projectile_y_pos[v1] = enemy_data[0].y_pos - 96;
  enemy_projectile_x_pos[v1] = enemy_data[0].x_pos;
}

static const int16 kEprojInit_SporeSpawners_X[4] = { 0x20, 0x60, 0xa0, 0xe0 };

void EprojInit_SporeSpawners(uint16 j) {  // 0x86DCD4
  int v1 = j >> 1;
  enemy_projectile_x_pos[v1] = kEprojInit_SporeSpawners_X[enemy_projectile_init_param];
  enemy_projectile_y_pos[v1] = 520;
}

#define kSporeMovementData ((uint8*)RomPtr(0x86dd6c))

void EprojPreInstr_Spores(uint16 k) {  // 0x86DCEE
  int v1 = k >> 1;
  uint16 v2 = LOBYTE(enemy_projectile_E[v1]);
  R18_ = SignExtend8(kSporeMovementData[LOBYTE(enemy_projectile_E[v1])]);
  if ((enemy_projectile_F[v1] & 0x80) != 0)
    R18_ = -R18_;
  enemy_projectile_x_pos[v1] += R18_;
  R18_ = SignExtend8(kSporeMovementData[v2 + 1]);
  uint16 v3 = R18_ + enemy_projectile_y_pos[v1] + R18_;
  enemy_projectile_y_pos[v1] = v3;
  if (!sign16(v3 - 768))
    enemy_projectile_id[v1] = 0;
  enemy_projectile_E[v1] = (uint8)(LOBYTE(enemy_projectile_E[v1]) + 2);
}

void EprojPreInstr_SporeSpawners(uint16 k) {  // 0x86DD46
  if (!kraid_unk9000) {
    int v1 = k >> 1;
    if (!enemy_projectile_F[v1]) {
      enemy_projectile_instr_list_ptr[v1] = addr_word_86DC06;
      enemy_projectile_instr_timers[v1] = 1;
      enemy_projectile_F[v1] = NextRandom() & 0x1FF;
    }
    --enemy_projectile_F[v1];
  }
}

#define word_86DEB6 ((uint16*)RomPtr(0x86deb6))

void EprojInit_NamiFuneFireball(uint16 j) {  // 0x86DED6
  int v2 = j >> 1;
  enemy_projectile_instr_list_ptr[v2] = addr_word_86DE96;
  enemy_projectile_E[v2] = FUNC16(EprojInit_NamiFuneFireball_MoveX1);
  if (enemy_projectile_init_param) {
    enemy_projectile_instr_list_ptr[v2] = addr_word_86DEA6;
    enemy_projectile_E[v2] = FUNC16(EprojInit_NamiFuneFireball_MoveX2);
  }
  EnemyData *v3 = gEnemyData(cur_enemy_index);
  enemy_projectile_x_pos[v2] = v3->x_pos;
  enemy_projectile_1A27[v2] = v3->x_subpos;
  enemy_projectile_y_pos[v2] = v3->y_pos;
  enemy_projectile_y_subpos[v2] = v3->y_subpos;
  if ((v3->parameter_1 & 0xF) != 0)
    enemy_projectile_y_pos[v2] += 4;
  int v4 = (uint16)(4 * LOBYTE(v3->parameter_2)) >> 1;
  enemy_projectile_y_vel[v2] = word_86DEB6[v4];
  enemy_projectile_x_vel[v2] = word_86DEB6[v4 + 1];
}

void CallNamiFuneFireballFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnEprojInit_NamiFuneFireball_MoveX1: EprojInit_NamiFuneFireball_MoveX1(k); return;
  case fnEprojInit_NamiFuneFireball_MoveX2: EprojInit_NamiFuneFireball_MoveX2(k); return;
  default: Unreachable();
  }
}
void EprojPreInstr_NamiFuneFireball(uint16 v0) {  // 0x86DF39
  CallNamiFuneFireballFunc(enemy_projectile_E[v0 >> 1] | 0x860000, v0);
  Eproj_NamiFuneFireball_After(v0);
}

void EprojInit_NamiFuneFireball_MoveX1(uint16 v1) {  // 0x86DF40
  int16 v3;

  int v2 = v1 >> 1;
  enemy_projectile_x_pos[v2] += SignExtend8((uint8)((uint16)(enemy_projectile_y_vel[v2] & 0xFF00) >> 8));
  v3 = LOBYTE(enemy_projectile_y_vel[v2]) << 8;
  bool v4 = __CFADD__uint16(enemy_projectile_1A27[v2], v3);
  enemy_projectile_1A27[v2] += v3;
  if (v4)
    ++enemy_projectile_x_pos[v2];
}

void EprojInit_NamiFuneFireball_MoveX2(uint16 v1) {  // 0x86DF6A
  int16 v3;

  int v2 = v1 >> 1;
  enemy_projectile_x_pos[v2] += SignExtend8((uint8)((uint16)(enemy_projectile_x_vel[v2] & 0xFF00) >> 8));
  v3 = LOBYTE(enemy_projectile_x_vel[v2]) << 8;
  bool v4 = __CFADD__uint16(enemy_projectile_1A27[v2], v3);
  enemy_projectile_1A27[v2] += v3;
  if (v4)
    ++enemy_projectile_x_pos[v2];
}

void Eproj_NamiFuneFireball_After(uint16 v0) {  // 0x86DF94
  if (EprojPreInstrHelper_DBF2_Func2(v0))
    enemy_projectile_id[v0 >> 1] = 0;
}

uint16 sub_86DFA0(uint16 k) {  // 0x86DFA0
  int v1 = k >> 1;
  return (int16)(enemy_projectile_x_pos[v1] - layer1_x_pos) < 0
    || (int16)(layer1_x_pos + 256 - enemy_projectile_x_pos[v1]) < 0;
}

uint16 EprojInstr_DFEA(uint16 k, uint16 j) {  // 0x86DFEA
  int v1 = k >> 1;
  R18_ = enemy_projectile_x_pos[v1];
  R20_ = enemy_projectile_y_pos[v1];
  SpawnEnemyDrops(addr_kEnemyDef_E83F, k);
  return j;
}

void EprojInit_LavaThrownByLavaman(uint16 j) {  // 0x86E000
  int v2 = j >> 1;
  enemy_projectile_instr_list_ptr[v2] = addr_word_86DFD8;
  enemy_projectile_E[v2] = FUNC16(Eproj_LavaThrownByLavaman_MoveX1);
  if (enemy_projectile_init_param) {
    enemy_projectile_instr_list_ptr[v2] = addr_word_86DFDE;
    enemy_projectile_E[v2] = FUNC16(Eproj_LavaThrownByLavaman_MoveX2);
  }
  EnemyData *v3 = gEnemyData(cur_enemy_index);
  enemy_projectile_x_pos[v2] = v3->x_pos;
  enemy_projectile_1A27[v2] = v3->x_subpos;
  enemy_projectile_y_pos[v2] = v3->y_pos + 2;
  enemy_projectile_y_subpos[v2] = v3->y_subpos;
  enemy_projectile_y_vel[v2] = -768;
  enemy_projectile_x_vel[v2] = 768;
}

void CallLavamanFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnEproj_LavaThrownByLavaman_MoveX1: Eproj_LavaThrownByLavaman_MoveX1(k); return;
  case fnEproj_LavaThrownByLavaman_MoveX2: Eproj_LavaThrownByLavaman_MoveX2(k); return;
  default: Unreachable();
  }
}

void sub_86E049(uint16 v0) {  // 0x86E049
  CallLavamanFunc(enemy_projectile_E[v0 >> 1] | 0x860000, v0);
  sub_86E0A4(v0);
}

void Eproj_LavaThrownByLavaman_MoveX1(uint16 k) {  // 0x86E050
  int16 v2;

  int v1 = k >> 1;
  enemy_projectile_x_pos[v1] += SignExtend8((enemy_projectile_y_vel[v1] & 0xFF00) >> 8);
  v2 = LOBYTE(enemy_projectile_y_vel[v1]) << 8;
  bool v3 = __CFADD__uint16(enemy_projectile_1A27[v1], v2);
  enemy_projectile_1A27[v1] += v2;
  if (v3)
    ++enemy_projectile_x_pos[v1];
}

void Eproj_LavaThrownByLavaman_MoveX2(uint16 k) {  // 0x86E07A
  int16 v2;

  int v1 = k >> 1;
  enemy_projectile_x_pos[v1] += SignExtend8((enemy_projectile_x_vel[v1] & 0xFF00) >> 8);
  v2 = LOBYTE(enemy_projectile_x_vel[v1]) << 8;
  bool v3 = __CFADD__uint16(enemy_projectile_1A27[v1], v2);
  enemy_projectile_1A27[v1] += v2;
  if (v3)
    ++enemy_projectile_x_pos[v1];
}

void sub_86E0A4(uint16 v0) {  // 0x86E0A4
  int16 v1;

  v1 = sub_86E0B0(v0);
  if (v1)
    enemy_projectile_id[v0 >> 1] = 0;
}

uint16 sub_86E0B0(uint16 k) {  // 0x86E0B0
  int v1 = k >> 1;
  return (int16)(enemy_projectile_x_pos[v1] - layer1_x_pos) < 0
    || (int16)(layer1_x_pos + 256 - enemy_projectile_x_pos[v1]) < 0
    || (int16)(enemy_projectile_y_pos[v1] - layer1_y_pos) < 0
    || (int16)(layer1_y_pos + 256 - enemy_projectile_y_pos[v1]) < 0;
}

#define off_86E42C ((uint16*)RomPtr(0x86e42c))
#define word_86E47E ((uint16*)RomPtr(0x86e47e))

void EprojInst_DustCloudOrExplosion(uint16 v0) {  // 0x86E468
  int v1 = v0 >> 1;
  enemy_projectile_instr_list_ptr[v1] = off_86E42C[enemy_projectile_init_param];
  enemy_projectile_x_pos[v1] = R18_;
  enemy_projectile_y_pos[v1] = R20_;
}

void EprojInit_EyeDoorSmoke(uint16 j) {  // 0x86E4A6
  int v1 = j >> 1;
  enemy_projectile_instr_list_ptr[v1] = off_86E42C[(uint16)(2 * (uint8)enemy_projectile_init_param) >> 1];
  int v2 = (uint16)(8 * HIBYTE(enemy_projectile_init_param)) >> 1;
  R18_ = word_86E47E[v2 + 2] + (word_86E47E[v2] & random_number);
  R20_ = word_86E47E[v2 + 3] + (word_86E47E[v2 + 1] & *(uint16 *)((char *)&random_number + 1));
  CalculatePlmBlockCoords(plm_id);
  enemy_projectile_x_pos[v1] = R18_ + 8 * (2 * plm_x_block + 1);
  enemy_projectile_y_pos[v1] = R20_ + 8 * (2 * plm_y_block + 1);
  NextRandom();
}

void EprojPreInstr_DustCloudOrExplosion(uint16 k) {  // 0x86E4FE
  if (CheckIfEnemyProjectileIsOffScreen(k))
    enemy_projectile_id[k >> 1] = 0;
}

uint16 EprojInstr_SetYVel(uint16 k, uint16 j) {  // 0x86E533
  enemy_projectile_y_vel[k >> 1] = *(uint16 *)RomPtr_86(j);
  return j + 2;
}

void EprojInit_SpawnedShotGate(uint16 j) {  // 0x86E5D0
  EprojInit_SpawnedGate_Common(j, 0);
}

void EprojInit_ClosedDownwardsShotGate(uint16 j) {  // 0x86E5D5
  EprojInit_SpawnedGate_Common(j, 0x40);
}

void EprojInit_ClosedUpwardsShotGate(uint16 j) {  // 0x86E5DA
  EprojInit_SpawnedGate_Common(j, -0x40);
}

void EprojInit_SpawnedGate_Common(uint16 j, uint16 v1) {  // 0x86E5DD
  R18_ = v1;
  uint16 v2 = plm_id;
  CalculatePlmBlockCoords(plm_id);
  int v3 = j >> 1;
  enemy_projectile_E[v3] = plm_block_indices[v2 >> 1];
  enemy_projectile_x_pos[v3] = 16 * plm_x_block;
  enemy_projectile_y_pos[v3] = R18_ + 16 * plm_y_block;
}

void EprojPreInstr_E605(uint16 k) {  // 0x86E605
  int v1 = k >> 1;
  uint16 v2 = enemy_projectile_timers[v1] + abs16(enemy_projectile_y_vel[v1]);
  if (v2 >= 0x1000u) {
    enemy_projectile_instr_timers[v1] = 1;
    ++enemy_projectile_instr_list_ptr[v1];
    ++enemy_projectile_instr_list_ptr[v1];
    v2 = 0;
  }
  enemy_projectile_timers[v1] = v2;
  enemy_projectile_y_subpos[v1] += LOBYTE(enemy_projectile_y_vel[v1]) << 8;
  enemy_projectile_y_pos[v1] += (int8)HIBYTE(enemy_projectile_y_vel[v1]);
}

void EprojInit_SaveStationElectricity(uint16 j) {  // 0x86E6AD
  CalculatePlmBlockCoords(plm_id);
  int v1 = j >> 1;
  enemy_projectile_x_pos[v1] = 16 * (plm_x_block + 1);
  enemy_projectile_y_pos[v1] = 16 * (plm_y_block - 2);
}

uint16 CheckIfEnemyProjectileIsOffScreen(uint16 k) {  // 0x86E6E0
  int v1 = k >> 1;
  uint16 result = 1;
  if ((int16)(enemy_projectile_x_pos[v1] - layer1_x_pos) >= 0) {
    enemy_population_ptr = layer1_x_pos + 256;
    if ((int16)(enemy_projectile_x_pos[v1] - (layer1_x_pos + 256)) < 0
        && (int16)(enemy_projectile_y_pos[v1] - layer1_y_pos) >= 0) {
      enemy_population_ptr = layer1_y_pos + 256;
      if ((int16)(enemy_projectile_y_pos[v1] - (layer1_y_pos + 256)) < 0)
        return 0;
    }
  }
  return result;
}

uint16 Eproj_FuncE722(uint16 k) {  // 0x86E722
  int16 v2;
  int16 v3;

  int v1 = k >> 1;
  v2 = enemy_projectile_x_pos[v1];
  uint16 result = 1;
  if (v2 >= 0) {
    if (sign16(v2 - 512)) {
      v3 = enemy_projectile_y_pos[v1];
      if (v3 >= 0) {
        if (sign16(v3 - 512))
          return 0;
      }
    }
  }
  return result;
}

void Eproj_FuncE73E_MoveXY(uint16 k) {  // 0x86E73E
  int v1 = k >> 1;
  uint16 v2 = enemy_projectile_1A27[v1];
  uint16 v4;
  if (((g_word_7E97DC[v1] + 64) & 0x80) != 0) {
    bool v3 = v2 < enemy_projectile_E[v1];
    enemy_projectile_1A27[v1] = v2 - enemy_projectile_E[v1];
    v4 = enemy_projectile_x_pos[v1] - (v3 + enemy_projectile_x_vel[v1]);
  } else {
    bool v3 = __CFADD__uint16(enemy_projectile_E[v1], v2);
    enemy_projectile_1A27[v1] = enemy_projectile_E[v1] + v2;
    v4 = enemy_projectile_x_vel[v1] + v3 + enemy_projectile_x_pos[v1];
  }
  enemy_projectile_x_pos[v1] = v4;
  uint16 v5 = enemy_projectile_y_subpos[v1], v6;
  if (((g_word_7E97DC[v1] + 128) & 0x80) != 0) {
    bool v3 = v5 < enemy_projectile_F[v1];
    enemy_projectile_y_subpos[v1] = v5 - enemy_projectile_F[v1];
    v6 = enemy_projectile_y_pos[v1] - (v3 + enemy_projectile_y_vel[v1]);
  } else {
    bool v3 = __CFADD__uint16(enemy_projectile_F[v1], v5);
    enemy_projectile_y_subpos[v1] = enemy_projectile_F[v1] + v5;
    v6 = enemy_projectile_y_vel[v1] + v3 + enemy_projectile_y_pos[v1];
  }
  enemy_projectile_y_pos[v1] = v6;
}

void Eproj_AngleToSamus(uint16 j) {  // 0x86E7AB
  int v1 = j >> 1;
  enemy_projectile_x_pos[v1] = R18_;
  enemy_projectile_y_pos[v1] = R20_;
  R18_ = samus_x_pos - enemy_projectile_x_pos[v1];
  R20_ = samus_y_pos - enemy_projectile_y_pos[v1];
  uint16 v2 = (uint8)(64 - CalculateAngleFromXY());
  g_word_7E97DC[v1] = v2;
  R18_ = v2;
  R20_ = enemy_projectile_init_param;
  ConvertAngleToXy();
  enemy_projectile_x_vel[v1] = R22_;
  enemy_projectile_E[v1] = R24_;
  enemy_projectile_y_vel[v1] = R26_;
  enemy_projectile_F[v1] = R28_;
}

#define kEprojInit_BotwoonsBody_InstrLists ((uint16*)RomPtr(0x86e9f1))

void EprojInit_BotwoonsBody(uint16 j) {  // 0x86EA31
  int v1 = j >> 1;
  enemy_projectile_x_pos[v1] = enemy_data[0].x_pos;
  enemy_projectile_y_pos[v1] = enemy_data[0].y_pos;
  enemy_projectile_y_vel[v1] = 0;
  uint16 v2 = 16;
  if (!enemy_data[0].ai_var_A)
    v2 = 48;
  R18_ = v2;
  uint16 v3 = kEprojInit_BotwoonsBody_InstrLists[v2 >> 1];
  int v4 = j >> 1;
  enemy_projectile_instr_list_ptr[v4] = v3;
  enemy_projectile_F[v4] = v3;
  enemy_projectile_E[v4] = R18_;
  enemy_projectile_x_vel[v4] = FUNC16(Eproj_BotwoonsBody_Main);
  uint16 ai_var_A = enemy_data[0].ai_var_A;
  gExtraEnemyRam7800(enemy_data[0].ai_var_A)->kraid.kraid_next = j;
  enemy_projectile_flags[v4] = 2;
  gExtraEnemyRam7800(ai_var_A)->kraid.kraid_healths_4ths[0] = 1;
}

void CallBotwoonEprojFunc(uint32 ea, uint16 k) {
  switch (ea) {
  case fnEproj_BotwoonsBody_Main: Eproj_BotwoonsBody_Main(k); return;
  case fnEproj_BotwonsBodyFunction_Dying: Eproj_BotwonsBodyFunction_Dying(k); return;
  case fnEproj_BotwonsBodyFunction_Dying2: Eproj_BotwonsBodyFunction_Dying2(k); return;
  case fnEproj_BotwonsBodyFunction_DyingFalling: Eproj_BotwonsBodyFunction_DyingFalling(k); return;
  case fnnullsub_101: return;
  default: Unreachable();
  }
}

void EprojPreInstr_BotwoonsBody(uint16 k) {  // 0x86EA80
  if (*(uint16 *)&extra_enemy_ram8000[0].pad[32]) {
    int v1 = k >> 1;
    if (enemy_projectile_x_vel[v1] == (uint16)FUNC16(Eproj_BotwoonsBody_Main))
      enemy_projectile_x_vel[v1] = FUNC16(Eproj_BotwonsBodyFunction_Dying);
  }
  CallBotwoonEprojFunc(enemy_projectile_x_vel[k >> 1] | 0x860000, k);
}

void Eproj_BotwoonsBody_Main(uint16 k) {  // 0x86EA98
  int v1 = k >> 1;
  uint16 v2 = kEprojInit_BotwoonsBody_InstrLists[enemy_projectile_E[v1] >> 1];
  if (v2 != enemy_projectile_F[v1]) {
    enemy_projectile_instr_list_ptr[v1] = v2;
    enemy_projectile_F[v1] = v2;
    enemy_projectile_instr_timers[v1] = 1;
  }
  Eproj_BotwoonsBodyHurtFlashHandling1(k);
}

void Eproj_BotwoonsBodyHurtFlashHandling1(uint16 k) {  // 0x86EAB4
  int v1 = k >> 1;
  enemy_projectile_gfx_idx[v1] |= 0xE00u;
  if (enemy_data[0].flash_timer) {
    if ((enemy_damage_routine_exec_count & 2) != 0)
      enemy_projectile_gfx_idx[v1] &= 0xF1FFu;
  }
}

void Eproj_BotwoonsBodyHurtFlashHandling2(uint16 j) {  // 0x86EAD4
  int v1 = j >> 1;
  enemy_projectile_gfx_idx[v1] |= 0xE00u;
  if (enemy_data[0].flash_timer) {
    if ((enemy_damage_routine_exec_count & 2) != 0)
      enemy_projectile_gfx_idx[v1] &= 0xF1FFu;
  }
}

void Eproj_BotwonsBodyFunction_Dying(uint16 v0) {  // 0x86EAF4
  int v1 = v0 >> 1;
  enemy_projectile_E[v1] = 4 * v0 + 96;
  enemy_projectile_x_vel[v1] = 0xEB04;
  Eproj_BotwonsBodyFunction_Dying2(v0);
}

void Eproj_BotwonsBodyFunction_Dying2(uint16 v0) {  // 0x86EB04
  int v1 = v0 >> 1;
  if (!sign16(++enemy_projectile_E[v1] - 256))
    enemy_projectile_x_vel[v1] = FUNC16(Eproj_BotwonsBodyFunction_DyingFalling);
  enemy_projectile_instr_timers[v1] = 0;
  Eproj_BotwoonsBodyHurtFlashHandling1(v0);
}

void Eproj_BotwonsBodyFunction_DyingFalling(uint16 v0) {  // 0x86EB1F
  int v1 = v0 >> 1;
  uint16 v2 = enemy_projectile_y_subpos[v1];
  int v3 = (uint16)(8 * (uint8)((uint16)(enemy_projectile_y_vel[v1] & 0xFF00) >> 8)) >> 1;
  bool v4 = __CFADD__uint16(kCommonEnemySpeeds_Quadratic_Copy[v3], v2);
  uint16 v5 = kCommonEnemySpeeds_Quadratic_Copy[v3] + v2;
  if (v4)
    ++enemy_projectile_y_pos[v1];
  enemy_projectile_y_subpos[v1] = v5;
  uint16 v6 = kCommonEnemySpeeds_Quadratic_Copy[v3 + 1] + enemy_projectile_y_pos[v1];
  enemy_projectile_y_pos[v1] = v6;
  if (sign16(v6 - 200)) {
    int v7 = v0 >> 1;
    enemy_projectile_y_vel[v7] += 192;
    enemy_projectile_instr_timers[v7] = 0;
    Eproj_BotwoonsBodyHurtFlashHandling2(v0);
  } else {
    enemy_projectile_y_pos[v1] = 200;
    enemy_projectile_x_vel[v1] = FUNC16(nullsub_101);
    enemy_projectile_instr_list_ptr[v1] = addr_word_86E208;
    enemy_projectile_gfx_idx[v1] = 2560;
    enemy_projectile_instr_timers[v1] = 1;
    QueueSmallExplosionSfx();
    if (v0 == 10)
      *(uint16 *)&extra_enemy_ram8800[0].pad[62] = 1;
  }
}

void QueueSmallExplosionSfx(void) {  // 0x86EB94
  QueueSfx2_Max6(0x24u);
}

void EprojInit_BotwoonsSpit(uint16 j) {  // 0x86EBC6
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  int v2 = j >> 1;
  enemy_projectile_x_pos[v2] = v1->x_pos;
  enemy_projectile_y_pos[v2] = v1->y_pos;
  enemy_projectile_instr_list_ptr[v2] = addr_word_86EBAE;
  uint16 v3 = remaining_enemy_spritemap_entries;
  g_word_7E97DC[v2] = remaining_enemy_spritemap_entries;
  R18_ = v3;
  R20_ = enemy_projectile_init_param;
  ConvertAngleToXy();
  enemy_projectile_x_vel[v2] = R22_;
  enemy_projectile_E[v2] = R24_;
  enemy_projectile_y_vel[v2] = R26_;
  enemy_projectile_F[v2] = R28_;
}

void EprojPreInstr_BotwoonsSpit(uint16 k) {  // 0x86EC05
  Eproj_FuncE73E_MoveXY(k);
  sub_86EC0C(k);
}

void sub_86EC0C(uint16 k) {  // 0x86EC0C
  int16 v1;

  v1 = sub_86EC18(k);
  if (v1)
    enemy_projectile_id[k >> 1] = 0;
}

uint16 sub_86EC18(uint16 k) {  // 0x86EC18
  int v1 = k >> 1;
  return (int16)(enemy_projectile_x_pos[v1] - layer1_x_pos) < 0
    || (int16)(layer1_x_pos + 256 - enemy_projectile_x_pos[v1]) < 0
    || (int16)(enemy_projectile_y_pos[v1] - layer1_y_pos) < 0
    || (int16)(layer1_y_pos + 256 - enemy_projectile_y_pos[v1]) < 0;
}

void EprojInit_YappingMawsBody(uint16 j) {  // 0x86EC62
  EnemyData *v2 = gEnemyData(cur_enemy_index);
  int v3 = j >> 1;
  enemy_projectile_x_pos[v3] = v2->x_pos;
  enemy_projectile_y_pos[v3] = v2->y_pos;
  enemy_projectile_instr_list_ptr[v3] = addr_word_86EC5C;
  if (!v2->parameter_2)
    enemy_projectile_instr_list_ptr[v3] = addr_word_86EC56;
  *(uint16 *)&extra_enemy_ram8800[0].pad[(uint16)(cur_enemy_index
                                                 + 2 * *(uint16 *)&extra_enemy_ram8800[0].pad[cur_enemy_index + 8])] = j;
}

uint16 EprojInstr_ECE3(uint16 k, uint16 j) {  // 0x86ECE3
  int v2 = k >> 1;
  R18_ = enemy_projectile_x_pos[v2] + (NextRandom() & 0x3F) - 32;
  R20_ = enemy_projectile_y_pos[v2] + ((uint16)(random_number & 0x3F00) >> 8) - 32;
  R22_ = *(uint16 *)RomPtr_86(j);
  R24_ = 0;
  CreateSpriteAtPos();
  return j + 2;
}

uint16 EprojInstr_ED17(uint16 k, uint16 j) {  // 0x86ED17
  int v2 = k >> 1;
  R18_ = enemy_projectile_x_pos[v2] + (NextRandom() & 0x1F) - 16;
  R20_ = enemy_projectile_y_pos[v2] + ((uint16)(random_number & 0x1F00) >> 8) - 16;
  R22_ = *(uint16 *)RomPtr_86(j);
  R24_ = 0;
  CreateSpriteAtPos();
  return j + 2;
}

uint16 EprojInstr_QueueSfx2_9(uint16 k, uint16 j) {  // 0x86EE8B
  QueueSfx2_Max1(9u);
  return j;
}

uint16 EprojInstr_QueueSfx2_24(uint16 k, uint16 j) {  // 0x86EE97
  QueueSfx2_Max1(0x24u);
  return j;
}

uint16 EprojInstr_QueueSfx2_B(uint16 k, uint16 j) {  // 0x86EEA3
  QueueSfx2_Max1(0xBu);
  return j;
}

#define off_86EF04 ((uint16*)RomPtr(0x86ef04))

uint16 EprojInstr_EEAF(uint16 k, uint16 j) {  // 0x86EEAF
  uint16 v2 = RandomDropRoutine(k);
  if (k != 0 && sign16(v2 - 6)) {
    uint16 v3 = 2 * v2;
    int v4 = k >> 1;
    enemy_projectile_E[v4] = v3;
    enemy_projectile_instr_list_ptr[v4] = off_86EF04[v3 >> 1];
    enemy_projectile_instr_timers[v4] = 1;
    enemy_projectile_F[v4] = 400;
    enemy_projectile_pre_instr[v4] = FUNC16(EprojPreInstr_Pickup);
    enemy_projectile_properties[v4] &= ~0x4000u;
    return enemy_projectile_instr_list_ptr[v4];
  } else {
    int v6 = k >> 1;
    enemy_projectile_instr_timers[v6] = 1;
    enemy_projectile_properties[v6] = 12288;
    enemy_projectile_pre_instr[v6] = FUNC16(EprojPreInstr_Empty);
    enemy_projectile_instr_list_ptr[v6] = 0xECA3;
    return 0xECA3;
  }
}

uint16 EprojInstr_HandleRespawningEnemy(uint16 k, uint16 j) {  // 0x86EF10
  if ((int16)enemy_projectile_killed_enemy_index[k >> 1] <= -2)
    RespawnEnemy(enemy_projectile_killed_enemy_index[k >> 1] & 0x7fff);
  return j;
}

void EprojInit_F337(uint16 j) {  // 0x86EF29
  int v2 = j >> 1;
  enemy_projectile_x_pos[v2] = R18_;
  enemy_projectile_y_pos[v2] = R20_;
  enemy_projectile_gfx_idx[v2] = 0;
  enemy_projectile_enemy_header_ptr[v2] = varE24; // this is X?!
  uint16 v3 = RandomDropRoutine(j);
  if (v3 != 0 && sign16(v3 - 6)) { // bug, why does it compare x here.
    uint16 v4 = 2 * v3;
    enemy_projectile_E[v2] = v4;
    enemy_projectile_instr_list_ptr[v2] = off_86EF04[v4 >> 1];
    enemy_projectile_instr_timers[v2] = 1;
    enemy_projectile_F[v2] = 400;
    enemy_projectile_killed_enemy_index[j >> 1] = -1;
  } else {
    int v5 = j >> 1;
    enemy_projectile_instr_list_ptr[v5] = addr_word_86ECA3;
    enemy_projectile_instr_timers[v5] = 1;
    enemy_projectile_properties[v5] = 12288;
    enemy_projectile_pre_instr[v5] = FUNC16(EprojPreInstr_Empty);
  }
}

#define off_86EFD5 ((uint16*)RomPtr(0x86efd5))

void EprojInit_EnemyDeathExplosion(uint16 j) {  // 0x86EF89
  EnemyData *v1 = gEnemyData(cur_enemy_index);
  int v2 = j >> 1;
  enemy_projectile_x_pos[v2] = v1->x_pos;
  enemy_projectile_y_pos[v2] = v1->y_pos;
  enemy_projectile_killed_enemy_index[v2] = cur_enemy_index;
  if ((v1->properties & 0x4000) != 0)
    enemy_projectile_killed_enemy_index[v2] = cur_enemy_index | 0x8000;
  enemy_projectile_enemy_header_ptr[v2] = v1->enemy_ptr;
  enemy_projectile_gfx_idx[v2] = 0;
  enemy_projectile_instr_list_ptr[v2] = off_86EFD5[enemy_projectile_init_param];
  enemy_projectile_instr_timers[v2] = 1;
}

static Func_V *const off_86F0AD[7] = {  // 0x86EFE0
  0,
  Eproj_Pickup_SmallHealth,
  Eproj_Pickup_BigHealth,
  Eproj_Pickup_PowerBombs,
  Eproj_Pickup_Missiles,
  Eproj_Pickup_SuperMissiles,
  0,
};

void EprojPreInstr_Pickup(uint16 k) {

  int v1 = k >> 1;
  if (!--enemy_projectile_F[v1])
    goto LABEL_7;
  if (CallSomeSamusCode(0xDu)) {
    if (sign16(enemy_projectile_F[v1] - 384)) {
      uint16 v2, v3;
      v2 = abs16(enemy_projectile_x_pos[v1] - grapple_beam_end_x_pos);
      if (sign16(v2 - 16)) {
        v3 = abs16(enemy_projectile_y_pos[v1] - grapple_beam_end_y_pos);
        if (sign16(v3 - 16)) {
          off_86F0AD[enemy_projectile_E[v1] >> 1]();
LABEL_7:;
          int v4 = k >> 1;
          enemy_projectile_instr_list_ptr[v4] = addr_word_86ECA3;
          enemy_projectile_instr_timers[v4] = 1;
          enemy_projectile_properties[v4] = 12288;
          enemy_projectile_pre_instr[v4] = FUNC16(EprojPreInstr_Empty);
          return;
        }
      }
    }
  }
  int v5 = k >> 1;
  enemy_population_ptr = LOBYTE(enemy_projectile_radius[v5]);
  draw_oam_x_offset = HIBYTE(enemy_projectile_radius[v5]);
  uint16 v6 = abs16(samus_x_pos - enemy_projectile_x_pos[v5]);
  bool v7 = v6 < samus_x_radius;
  uint16 v8 = v6 - samus_x_radius;
  if (v7 || v8 < enemy_population_ptr) {
    uint16 v9 = abs16(samus_y_pos - enemy_projectile_y_pos[v5]);
    v7 = v9 < samus_y_radius;
    uint16 v10 = v9 - samus_y_radius;
    if (v7 || v10 < draw_oam_x_offset) {
      off_86F0AD[enemy_projectile_E[v5] >> 1]();
      int v11 = k >> 1;
      enemy_projectile_instr_list_ptr[v11] = addr_word_86ECA3;
      enemy_projectile_instr_timers[v11] = 1;
      enemy_projectile_properties[v11] = 12288;
      enemy_projectile_pre_instr[v11] = FUNC16(EprojPreInstr_Empty);
    }
  }
}

void Eproj_Pickup_SmallHealth(void) {  // 0x86F0BB
  Samus_RestoreHealth(5u);
  QueueSfx2_Max1(1u);
}

void Eproj_Pickup_BigHealth(void) {  // 0x86F0CA
  Samus_RestoreHealth(0x14u);
  QueueSfx2_Max1(2u);
}

void Eproj_Pickup_PowerBombs(void) {  // 0x86F0D9
  Samus_RestorePowerBombs(1u);
  QueueSfx2_Max1(5u);
}

void Eproj_Pickup_Missiles(void) {  // 0x86F0E8
  Samus_RestoreMissiles(2u);
  QueueSfx2_Max1(3u);
}

void Eproj_Pickup_SuperMissiles(void) {  // 0x86F0F7
  Samus_RestoreSuperMissiles(1u);
  QueueSfx2_Max1(4u);
}

static const uint8 byte_86F25E[6] = { 1, 2, 4, 6, 5, 3 };

uint16 RandomDropRoutine(uint16 k) {  // 0x86F106
  char v9; // cf

  int v1 = k >> 1;
  varE2A = enemy_projectile_killed_enemy_index[v1] & 0x7FFF;
  varE28 = enemy_projectile_enemy_header_ptr[v1];
  if (varE28 == 0)
    goto LABEL_30;
  uint16 v2;
  v2 = *((uint16 *)RomPtr_A0(varE28) + 29);
  if (!v2)
    goto LABEL_30;
  uint8 Random;
  do
    Random = NextRandom();
  while (!Random);
  R26_ = Random;
  R20_ = 255;
  R24_ = 0;
  uint16 v5;
  v5 = 1;
  if ((uint16)(samus_reserve_health + samus_health) >= 0x1Eu) {
    if ((uint16)(samus_reserve_health + samus_health) < 0x32u)
      goto LABEL_7;
    v5 = 0;
  }
  health_drop_bias_flag = v5;
LABEL_7:
  if ((uint8)health_drop_bias_flag) {
    uint8 *v6 = RomPtr_B4(v2);
    LOBYTE(R18_) = v6[1] + *v6;
    LOBYTE(R22_) = 3;
  } else {
    uint8 *v7 = RomPtr_B4(v2);
    LOBYTE(R18_) = v7[3];
    LOBYTE(R22_) = 8;
    if (samus_health != samus_max_health || samus_reserve_health != samus_max_reserve_health) {
      LOBYTE(R18_) = v7[1] + *v7 + R18_;
      LOBYTE(R22_) = R22_ | 3;
    }
    if (samus_missiles != samus_max_missiles) {
      LOBYTE(R18_) = v7[2] + R18_;
      LOBYTE(R22_) = R22_ | 4;
    }
    if (samus_super_missiles != samus_max_super_missiles) {
      LOBYTE(R20_) = R20_ - v7[4];
      LOBYTE(R22_) = R22_ | 0x10;
    }
    if (samus_power_bombs != samus_max_power_bombs) {
      LOBYTE(R20_) = R20_ - v7[5];
      LOBYTE(R22_) = R22_ | 0x20;
    }
  }
  int i;
  for (i = 0; i != 4; ++i) {
    if (!(uint8)R18_) {
      LOBYTE(R22_) = (uint8)R22_ >> 4;
      v2 += 4;
      i = 4;
      goto LABEL_26;
    }
    v9 = R22_ & 1;
    LOBYTE(R22_) = (uint8)R22_ >> 1;
    if (v9) {
      uint8 *v10 = RomPtr_B4(v2);
      uint16 RegWord = Mult8x8(R20_, *v10);
      int divved = SnesDivide(RegWord, R18_);
      uint16 v12 = R24_;
      uint16 v13 = divved + v12;
      if (v13 >= R26_)
        return byte_86F25E[i];
      R24_ = v13;
    }
    ++v2;
  }
  do {
LABEL_26:
    v9 = R22_ & 1;
    LOBYTE(R22_) = (uint8)R22_ >> 1;
    if (v9) {
      uint8 *v14 = RomPtr_B4(v2);
      if ((uint16)(R24_ + *v14) >= R26_)
        return byte_86F25E[i];
      R24_ += *v14;
    }
    ++v2;
    ++i;
  } while (i != 6);
LABEL_30:
  i = 3;
  return byte_86F25E[i];
}

void RespawnEnemy(uint16 v0) {  // 0x86F264
  EnemySpawnData *v3; // r10
  EnemyDef_A2 *EnemyDef_A2; // r10
  int16 v7;

  cur_enemy_index = v0;
  uint8 *v1 = RomPtr_A1(room_enemy_population_ptr + (v0 >> 2));
  EnemyData *v2 = gEnemyData(v0);
  v2->enemy_ptr = *(uint16 *)v1;
  v2->x_pos = *((uint16 *)v1 + 1);
  v2->y_pos = *((uint16 *)v1 + 2);
  v2->current_instruction = *((uint16 *)v1 + 3);
  v2->properties = *((uint16 *)v1 + 4);
  v2->extra_properties = *((uint16 *)v1 + 5);
  v2->parameter_1 = *((uint16 *)v1 + 6);
  v2->parameter_2 = *((uint16 *)v1 + 7);
  v3 = gEnemySpawnData(v0);
  EnemyData *v4 = gEnemyData(v0);
  v4->palette_index = v3->palette_index;
  v4->vram_tiles_index = v3->vram_tiles_index;
  v2->frozen_timer = 0;
  v2->flash_timer = 0;
  v2->invincibility_timer = 0;
  v2->timer = 0;
  v2->frame_counter = 0;
  v2->ai_var_A = 0;
  v2->ai_var_B = 0;
  v2->ai_var_C = 0;
  v2->ai_var_D = 0;
  v2->ai_var_E = 0;
  v2->ai_preinstr = 0;
  v2->instruction_timer = 1;
  uint16 enemy_ptr = gEnemyData(cur_enemy_index)->enemy_ptr;
  EnemyDef_A2 = get_EnemyDef_A2(enemy_ptr);
  enemy_ai_pointer.addr = EnemyDef_A2->ai_init;
  v2->x_width = EnemyDef_A2->x_radius;
  v2->y_height = EnemyDef_A2->y_radius;
  v2->health = EnemyDef_A2->health;
  v2->layer = RomPtr_A0(enemy_ptr)[57];
  v7 = *(uint16 *)&EnemyDef_A2->bank;
  *(uint16 *)&v2->bank = v7;
  enemy_ai_pointer.bank = v7;
  CallEnemyAi(Load24(&enemy_ai_pointer));
}

#define kEnemyDef_F3D3 (*(EnemyDef_B2*)RomPtr(0xa0f3d3))

void EprojInit_Sparks(uint16 j) {  // 0x86F391
  static const uint16 word_86F3D4[14] = { 0xffff, 0xb800, 0xffff, 0xc000, 0xffff, 0xe000, 0xffff, 0xff00, 0, 0x100, 0, 0x2000, 0, 0x4000 };
#define g_word_86F3D4 ((uint16*)RomPtr(0x86f3d4))  // bug:: oob read
  int v2 = j >> 1;
  enemy_projectile_instr_list_ptr[v2] = addr_kEnemyDef_F353;
  EnemyData *v3 = gEnemyData(cur_enemy_index);
  enemy_projectile_x_pos[v2] = v3->x_pos;
  enemy_projectile_1A27[v2] = v3->x_subpos;
  enemy_projectile_y_pos[v2] = v3->y_pos + 8;
  enemy_projectile_y_subpos[v2] = v3->y_subpos;
  enemy_projectile_x_vel[v2] = 0;
  enemy_projectile_y_vel[v2] = 0;
  uint16 v4 = (NextRandom() & 0x1C) >> 1;
  enemy_projectile_F[v2] = g_word_86F3D4[v4 + 0];
  enemy_projectile_E[v2] = g_word_86F3D4[v4 + 1];
}

void EprojPreInstr_Sparks(uint16 k) {  // 0x86F3F0
  int v1 = k >> 1;
  if ((enemy_projectile_y_vel[v1] & 0x8000u) == 0) {
    if (EnemyProjectileBlockCollisition_Vertical(k) & 1) {
      enemy_projectile_instr_list_ptr[v1] = 0xF363;
      enemy_projectile_instr_timers[v1] = 1;

      enemy_projectile_F[v1] = enemy_projectile_F[v1] * 2 + (enemy_projectile_E[v1] >> 15);
      enemy_projectile_E[v1] *= 2;

      enemy_projectile_F[v1] = enemy_projectile_F[v1] * 2 + (enemy_projectile_E[v1] >> 15);
      enemy_projectile_E[v1] *= 2;

      enemy_projectile_x_vel[v1] = 0x8000;
      enemy_projectile_y_vel[v1] = -1;
      enemy_projectile_y_pos[v1] -= 2;
      return;
    }
    uint16 v2 = enemy_projectile_x_vel[v1];
    enemy_projectile_x_vel[v1] = v2 + 0x4000;
    uint16 v3 = __CFADD__uint16(v2, 0x4000) + enemy_projectile_y_vel[v1];
    if (v3 < 4u)
      enemy_projectile_y_vel[v1] = v3;
  }
  uint16 v4 = enemy_projectile_y_subpos[v1];
  bool v5 = __CFADD__uint16(enemy_projectile_x_vel[v1], v4);
  enemy_projectile_y_subpos[v1] = enemy_projectile_x_vel[v1] + v4;
  enemy_projectile_y_pos[v1] += enemy_projectile_y_vel[v1] + v5;
  uint16 v6 = enemy_projectile_1A27[v1];
  v5 = __CFADD__uint16(enemy_projectile_E[v1], v6);
  enemy_projectile_1A27[v1] = enemy_projectile_E[v1] + v6;
  enemy_projectile_x_pos[v1] += enemy_projectile_F[v1] + v5;
  if ((nmi_frame_counter_byte & 3) == 0) {
    R18_ = enemy_projectile_x_pos[v1];
    R20_ = enemy_projectile_y_pos[v1];
    R22_ = 48;
    R24_ = enemy_projectile_gfx_idx[v1];
    CreateSpriteAtPos();
  }
}
