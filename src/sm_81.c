// SRAM, spritemap processing & menus

#include "sm_rtl.h"
#include "ida_types.h"
#include "variables.h"
#include "funcs.h"
#include "enemy_types.h"


#define kOffsetToSaveSlot ((uint16*)RomFixedPtr(0x81812b))
#define kPackedBytesPerArea_Count ((uint8*)RomFixedPtr(0x818131))
#define kPackedBytesPerArea_PackedOffs ((uint16*)RomFixedPtr(0x818138))
#define kPackedBytesPerArea_UnpackedOffs ((uint16*)RomFixedPtr(0x8182d6))
#define kMenuPalettes ((uint16*)RomFixedPtr(0x8ee400))
#define kZebesAndStarsTilemap ((uint16*)RomFixedPtr(0x8edc00))
#define kAreaMapForegroundSetDefs ((uint16*)RomFixedPtr(0x81a4e6))
#define kAreaMapForegroundColors ((uint16*)RomFixedPtr(0x81a40e))
#define kBg2RoomSelectMapTilemap ((uint16*)RomFixedPtr(0xb6e000))
#define kFileSelectExpandingSquareTilemap ((uint16*)RomFixedPtr(0x81b14b))
#define kMapIconDataPointers ((MapIconDataPointers*)RomFixedPtr(0x82c7cb))
#define g_word_82C749 ((uint16*)RomFixedPtr(0x82c749))
#define kRoomState_aa82_aa8f (*(RoomDefRoomstate*)RomFixedPtr(0x8faa8f))
#define kLeftMapScrollArrowData (*(MapScrollArrowData*)RomFixedPtr(0x81af32))
#define kRightMapScrollArrowData (*(MapScrollArrowData*)RomFixedPtr(0x81af3c))
#define kUpMapScrollArrowData (*(MapScrollArrowData*)RomFixedPtr(0x81af46))
#define kDownMapScrollArrowData (*(MapScrollArrowData*)RomFixedPtr(0x81af50))
#define g_off_82C569 ((uint16*)RomFixedPtr(0x82c569))
#define kSamusSpritemapTable ((uint16*)RomFixedPtr(0x92808d))
#define g_off_93A1A1 ((uint16*)RomFixedPtr(0x93a1a1))
#define kExpandingSquareVels ((ExpandingSquareVels*)RomFixedPtr(0x81aa34))



static const uint16 kFileSelectMap_AreaIndexes[6] = { 0, 3, 5, 1, 4, 2 };

void SoftReset(void) {
  game_state = 0xffff;
}

void SaveToSram(uint16 a) {  // 0x818000
  char v0 = a;
  uint16 v2;
  int16 v7;
  int16 v11;

  R20_ = 0;
  R18_ = 2 * (v0 & 3);
  for (int i = 94; i >= 0; i -= 2)
    player_data_saved[i >> 1] = *(uint16 *)((char *)&equipped_items + i);
  LOBYTE(v2) = HIBYTE(area_index);
  HIBYTE(v2) = area_index;
  uint16 v3 = v2;
  uint16 v4 = 0;
  do {
    explored_map_tiles_saved[v3 >> 1] = *(uint16 *)&map_tiles_explored[v4];
    v4 += 2;
    v3 += 2;
  } while ((int16)(v4 - 256) < 0);
  PackMapToSave();
  sram_save_station_index = load_station_index;
  sram_area_index = area_index;
  uint16 v5 = kOffsetToSaveSlot[R18_ >> 1];
  uint16 *v6 = player_data_saved;
  do {
    v7 = *v6++;
    *(uint16 *)(&g_sram[2 * (v5 >> 1)]) = v7;
    R20_ += v7;
    v5 += 2;
  } while (v6 != plm_instruction_timer);
  uint16 v8 = R18_;
  uint16 v9 = R20_;
  int v10 = R18_ >> 1;
  *(uint16 *)(&g_sram[2 * v10]) = R20_;
  *(uint16 *)(&g_sram[2 * v10 + 0x1FF0]) = v9;
  v11 = ~v9;
  *(uint16 *)(&g_sram[2 * v10 + 8]) = v11;
  *(uint16 *)(&g_sram[2 * v10 + 0x1FF8]) = v11;

  RtlWriteSram();
}

uint8 LoadFromSram(uint16 a) {  // 0x818085
  R20_ = 0;
  R18_ = 2 * (a & 3);
  uint16 v1 = kOffsetToSaveSlot[a & 3];
  uint16 *v2 = player_data_saved;
  do {
    uint16 v3 = kSramChecksum[v1 >> 1];
    *v2++ = v3;
    R20_ += v3;
    v1 += 2;
  } while (v2 != plm_instruction_timer);
  int v4 = R18_ >> 1;
  if (R20_ == kSramChecksum[v4] && (R20_ ^ 0xffff) == kSramChecksumInverted[v4]
      || R20_ == kSramChecksumUpper[v4] && (R20_ ^ 0xffff) == kSramChecksumInvertedUpper[v4]) {
    for (int i = 94; i >= 0; i -= 2)
      *(uint16 *)((char *)&equipped_items + i) = player_data_saved[i >> 1];
    UnpackMapFromSave();
    load_station_index = sram_save_station_index;
    area_index = sram_area_index;
    return 0;
  } else {
    R20_ = 0;
    uint16 v7 = kOffsetToSaveSlot[R18_ >> 1];
    uint16 v8 = ADDR16_OF_RAM(*player_data_saved);
    uint16 v9 = 0;
    do {
      kSramChecksum[v7 >> 1] = v9;
      v9 += R20_;
      R20_ = v9;
      v7 += 2;
      v8 += 2;
    } while (v8 != ADDR16_OF_RAM(*plm_instruction_timer)); // 0xDE1C
    load_station_index = 0;
    area_index = 0;
    return 1;
  }
}

void UnpackMapFromSave(void) {  // 0x8182E4
  for (int i = 1792; i >= 0; i -= 2)
    explored_map_tiles_saved[i >> 1] = 0;
  R22_ = 0;
  do {
    R20_ = swap16(R22_);
    R18_ = kPackedBytesPerArea_Count[R22_];
    int v2 = (uint16)(2 * R22_) >> 1;
    R0_.addr = kPackedBytesPerArea_UnpackedOffs[v2];
    *(uint16 *)&R0_.bank = 129;
    int v3 = kPackedBytesPerArea_PackedOffs[v2];
    R3_.addr = ADDR16_OF_RAM(*explored_map_tiles_saved);
    *(uint16 *)&R3_.bank = 126;
    do {
      uint16 v4 = R20_ + *RomPtr_81(R0_.addr);
      IndirWriteByte(&R3_, v4, compressed_map_data[v3]);
      ++R0_.addr;
      ++v3;
      --R18_;
    } while (R18_);
    ++R22_;
  } while (sign16(R22_ - 6));
}

void PackMapToSave(void) {  // 0x81834B
  R26_ = 0;
  do {
    R22_ = kPackedBytesPerArea_Count[R26_];
    int v0 = R26_;
    R0_.addr = kPackedBytesPerArea_UnpackedOffs[v0];
    int v1 = kPackedBytesPerArea_PackedOffs[v0];
    R24_ = swap16(R26_);
    R3_.addr = ADDR16_OF_RAM(*explored_map_tiles_saved);
    *(uint16 *)&R3_.bank = 126;
    do {
      int v3 = R24_ + *RomPtr_81(R0_.addr);
      compressed_map_data[v1] = *IndirPtr(&R3_, v3);
      ++R0_.addr;
      ++v1;
      --R22_;
    } while (R22_);
    ++R26_;
  } while (sign16(R26_ - 6));
}

void DrawSpritemap(uint8 db, uint16 j) {  // 0x81879F
  int16 v7;
  OamEnt *v8;
  char v13;
  char v15;

  const uint16 *v2 = (const uint16 *)RomPtrWithBank(db, j);
  if (*v2) {
    R24_ = *v2;
    uint16 v3 = j + 2;
    uint16 v4 = oam_next_ptr;
    if ((oam_next_ptr & 0xFE00) != 0) {
      oam_next_ptr = v4;
      return;
    }
    uint16 v5 = oam_next_ptr;
    while (1) {

      const uint8 *v6 = RomPtrWithBank(db, v3);
      v7 = R20_ + GET_WORD(v6);
      v8 = gOamEnt(v5);
      *(uint16 *)&v8->xcoord = v7;
      if ((v7 & 0x100) != 0) {
        int v9 = v5 >> 1;
        if (*(int16 *)v6 >= 0) {
          *(uint16 *)RomPtr_RAM(kOamExtra_Address_And_X8Large[v9]) |= kOamExtra_X8Small_And_Large[v9];
        } else {
          *(uint16 *)RomPtr_RAM(kOamExtra_Address_And_X8Large[v9]) |= kOamExtra_Address_And_X8Large[v9 + 1];
        }
        
      } else if (*(int16 *)v6 < 0) {
        int v11 = v5 >> 1;
        *(uint16 *)RomPtr_RAM(kOamExtra_Address_And_X8Large[v11]) |= kOamExtra_X8Small_And_Large[v11 + 1];
      }
      v13 = v6[2];
      if (v13 >= 0) {
        bool v14 = __CFADD__uint8((uint8)R18_, v13);
        v15 = R18_ + v13;
        if (!v14 && (uint8)v15 < 0xE0u)
          goto LABEL_22;
        goto LABEL_21;
      }
      {
        bool v14;
        v14 = __CFADD__uint8((uint8)R18_, v13);
        v15 = R18_ + v13;
        if (v14) {
          if ((uint8)v15 >= 0xE0u)
            goto LABEL_21;
        } else if ((uint8)v15 < 0xE0u) {
LABEL_21:
          SetXCoordToInvalidPos(v5);
          v15 = -32;
        }
      }
      {
LABEL_22:
        v8->ycoord = v15;
        *(uint16 *)&v8->charnum = R22_ | GET_WORD(v6 + 3) & 0xF1FF;
        v4 = v5 + 4;
        if (((v5 + 4) & 0xFE00) != 0) {
          oam_next_ptr = v4;
          return;
        }
        v5 += 4;
        v3 += 5;
        if (!--R24_) {
          oam_next_ptr = v5;
          return;
        }
      }
    }
  }
}

void DrawSpritemapOffScreen(uint16 j) {  // 0x818853
  int16 v6;
  OamEnt *v7;
  char v12;
  char v14;

  const uint16 *v1 = (const uint16 *)RomPtr_8C(j);
  if (*v1) {
    R24_ = *v1;
    uint16 v2 = j + 2;
    uint16 v3 = oam_next_ptr;
    if ((oam_next_ptr & 0xFE00) != 0) {
      oam_next_ptr = v3;
      return;
    }
    uint16 v4 = oam_next_ptr;
    while (1) {
      const uint8 *v5 = RomPtrWithBank(0x8c, v2);
      v6 = R20_ + GET_WORD(v5);
      v7 = gOamEnt(v4);
      *(uint16 *)&v7->xcoord = v6;
      if ((v6 & 0x100) != 0) {
        int v8 = v4 >> 1;
        if (*(int16 *)v5 >= 0) {
          *(uint16 *)RomPtr_RAM(kOamExtra_Address_And_X8Large[v8]) |= kOamExtra_X8Small_And_Large[v8];
        } else {
          *(uint16 *)RomPtr_RAM(kOamExtra_Address_And_X8Large[v8]) |= kOamExtra_Address_And_X8Large[v8 + 1];
        }
        
      } else if (*(int16 *)v5 < 0) {
        int v10 = v4 >> 1;
        *(uint16 *)RomPtr_RAM(kOamExtra_Address_And_X8Large[v10]) |= kOamExtra_X8Small_And_Large[v10 + 1];
      }
      v12 = v5[2];
      if (v12 >= 0) {
        bool v13 = __CFADD__uint8((uint8)R18_, v12);
        v14 = R18_ + v12;
        if (v13 || (uint8)v14 >= 0xE0u)
          goto LABEL_22;
        goto LABEL_21;
      } else {
        bool v13;
        v13 = __CFADD__uint8((uint8)R18_, v12);
        v14 = R18_ + v12;
        if (v13) {
          if ((uint8)v14 < 0xE0u)
            goto LABEL_21;
        } else if ((uint8)v14 >= 0xE0u) {
LABEL_21:
          SetXCoordToInvalidPos(v4);
          v14 = -32;
        }
      }
LABEL_22:
      v7->ycoord = v14;
      *(uint16 *)&v7->charnum = R22_ | *(uint16 *)(v5 + 3) & 0xF1FF;
      v3 = v4 + 4;
      if (((v4 + 4) & 0xFE00) != 0) {
        oam_next_ptr = v3;
        return;
      }
      v4 += 4;
      v2 += 5;
      if (!--R24_) {
        oam_next_ptr = v4;
        return;
      }
    }
  }
}

void SetXCoordToInvalidPos(uint16 k) {  // 0x818907
  gOamEnt(k)->xcoord = 0x80;
  int v1 = k >> 1;
  *(uint16 *)RomPtr_RAM(kOamExtra_Address_And_X8Large[v1]) |= kOamExtra_X8Small_And_Large[v1];
}


void DrawMenuSpritemap(uint16 a, uint16 k, uint16 j) {  // 0x81891F
  const uint8 *pp = RomPtr_82(g_off_82C569[a]);
  if (GET_WORD(pp + 0)) {
    int n = GET_WORD(pp + 0);
    pp += 2;
    int v6 = oam_next_ptr;
    do {
      int v10 = v6 >> 1;
      uint16 *dst = (uint16 *)RomPtr_RAM(kOamExtra_Address_And_X8Large[v10]);
      uint16 v8 = k + GET_WORD(pp + 0);
      OamEnt *v9 = gOamEnt(v6);
      v9->xcoord = v8;
      if ((v8 & 0x100) != 0) {
        if (*(int16 *)pp >= 0) {
          *dst |= kOamExtra_X8Small_And_Large[v10];
        } else {
          *dst |= kOamExtra_Address_And_X8Large[v10 + 1];
        }
      } else if (*(int16 *)pp < 0) {
        *dst |= kOamExtra_X8Small_And_Large[v10 + 1];
      }
      v9->ycoord = j + GET_BYTE(pp + 2);
      *(uint16 *)&v9->charnum = R3_.addr | GET_WORD(pp + 3) & 0xF1FF;
      pp += 5;
      v6 = (v6 + 4) & 0x1FF;
    } while (--n);
    oam_next_ptr = v6;
  }
}


void DrawSamusSpritemap(uint16 a, uint16 k, uint16 j) {  // 0x8189AE
  uint16 v3 = kSamusSpritemapTable[a];
  if (v3 == 0)
    return;
  const uint8 *pp = RomPtr_92(v3);
  int n = GET_WORD(pp + 0);
  pp += 2;
  uint16 v6 = oam_next_ptr;
  for(; n; n--) {
    int v10 = v6 >> 1;
    uint16 *dst = (uint16 *)RomPtr_RAM(kOamExtra_Address_And_X8Large[v10]);
    uint16 v8 = k + GET_WORD(pp + 0);
    OamEnt *v9 = gOamEnt(v6);
    v9->xcoord = v8;
    if ((v8 & 0x100) != 0) {
      if (*(int16 *)pp >= 0) {
        *dst |= kOamExtra_X8Small_And_Large[v10];
      } else {
        *dst |= kOamExtra_Address_And_X8Large[v10 + 1];
      }
    } else if (*(int16 *)pp < 0) {
      *dst |= kOamExtra_X8Small_And_Large[v10 + 1];
    }
    v9->ycoord = j + GET_BYTE(pp + 2);
    *(uint16 *)&v9->charnum = GET_WORD(pp + 3);
    pp += 5;
    v6 = (v6 + 4) & 0x1FF;
  }
  oam_next_ptr = v6;
}


void DrawBeamGrappleSpritemap(uint16 a) {  // 0x818A37
  sub_818A5F(RomPtr_93(g_off_93A1A1[a]));
}

void DrawProjectileSpritemap(uint16 k) {  // 0x818A4B
  sub_818A5F(RomPtr_93(projectile_spritemap_pointers[k >> 1]));
}

void sub_818A5F(const uint8 *pp) {  // 0x818A5F
  int n = GET_WORD(pp);
  if (!n)
    return;
  pp += 2;
  uint16 idx = oam_next_ptr;
  do {
    int v5 = idx >> 1;
    uint16 v3 = R20_ + GET_WORD(pp);
    OamEnt *v4 = gOamEnt(idx);
    v4->xcoord = v3;
    uint16 *dst = (uint16 *)RomPtr_RAM(kOamExtra_Address_And_X8Large[v5]);
    if ((v3 & 0x100) != 0)
      *dst |= kOamExtra_X8Small_And_Large[v5];
    if (*(int16 *)pp < 0)
      *dst |= kOamExtra_X8Small_And_Large[v5 + 1];
    v4->ycoord = R18_ + GET_BYTE(pp + 2);
    *(uint16 *)&v4->charnum = GET_WORD(pp + 3);
    pp += 5;
    idx = (idx + 4) & 0x1FF;
  } while (--n);
  oam_next_ptr = idx;
}

void DrawSpritemapWithBaseTile(uint8 db, uint16 j) {  // 0x818AB8
  if (j == 0)
    return; // bug fix
  uint8 *pp = (uint8 *)RomPtrWithBank(db, j);
  int n = GET_WORD(pp);
  uint16 v3 = oam_next_ptr;
  pp += 2;
  for(; n != 0; n--) {
    int v7 = v3 >> 1;
    OamEnt *oam = gOamEnt(v3);
    uint16 v5 = R20_ + GET_WORD(pp + 0);
    oam->xcoord = v5;
    uint16 *dst = (uint16 *)RomPtr_RAM(kOamExtra_Address_And_X8Large[v7]);
    if ((v5 & 0x100) != 0)
      *dst |= kOamExtra_X8Small_And_Large[v7];
    if (*(int16 *)pp < 0)
      *dst |= kOamExtra_X8Small_And_Large[v7 + 1];
    oam->ycoord = R18_ + pp[2];
    *(uint16 *)&oam->charnum = R3_.addr | (R0_.addr + GET_WORD(pp + 3));
    pp += 5;
    v3 = (v3 + 4) & 0x1FF;
  }
  oam_next_ptr = v3;
}

void DrawSpritemapWithBaseTile2(uint8 db, uint16 j) {  // 0x818B22
  const uint8 *pp = RomPtrWithBank(db, j);
  uint16 v4 = oam_next_ptr;
  int n = GET_WORD(pp);
  pp += 2;
  for (; n != 0; n--) {
    int v8 = v4 >> 1;
    OamEnt *oam = gOamEnt(v4);
    uint16 v6 = R20_ + GET_WORD(pp + 0);
    oam->xcoord = v6;
    uint16 *dst = (uint16 *)RomPtr_RAM(kOamExtra_Address_And_X8Large[v8]);
    if ((v6 & 0x100) != 0)
      *dst |= kOamExtra_X8Small_And_Large[v8];
    if (*(int16 *)pp < 0)
      *dst |= kOamExtra_X8Small_And_Large[v8 + 1];
    int y = pp[2] + LOBYTE(R18_);
    if ((pp[2] & 0x80) ? (y & 0x100) == 0 : (y & 0x100) != 0)
      y = 0xf0;
    oam->ycoord = y;
    *(uint16 *)&oam->charnum = R3_.addr | (R0_.addr + GET_WORD(pp + 3));
    pp += 5;
    v4 = (v4 + 4) & 0x1FF;
  }
  oam_next_ptr = v4;
}

void DrawSpritemapWithBaseTileOffscreen(uint8 db, uint16 j, uint16 r20_x, uint16 r18_y) {  // 0x818B96
  const uint8 *pp = RomPtrWithBank(db, j);
  uint16 v4 = oam_next_ptr;
  int n = GET_WORD(pp); pp += 2;
  for (; n; n--) {
    int v8 = v4 >> 1;
    int v6 = r20_x + GET_WORD(pp + 0);
    OamEnt *v7 = gOamEnt(v4);
    v7->xcoord = v6;
    uint16 *dst = (uint16 *)RomPtr_RAM(kOamExtra_Address_And_X8Large[v8]);
    if ((v6 & 0x100) != 0)
      *dst |= kOamExtra_X8Small_And_Large[v8];
    if (*(int16 *)pp < 0)
      *dst |= kOamExtra_X8Small_And_Large[v8 + 1];
    uint8 v12 = pp[2];
    bool v13 = __CFADD__uint8((uint8)R18_, v12);
    v12 += r18_y;
    if (!!sign8(pp[2]) == v13)
      v12 = 0xf0;
    v7->ycoord = v12;
    *(uint16 *)&v7->charnum = R3_.addr | (R0_.addr + GET_WORD(pp + 3));
    pp += 5;
    v4 = (v4 + 4) & 0x1FF;
  }
  oam_next_ptr = v4;
}

void DrawEnemyProjectileSpritemapWithBaseTile(uint8 db, uint16 j) {  // 0x818C0A
  int16 v6;
  OamEnt *v7;
  char v10;
  char v12; // cf

  const uint16 *v2 = (const uint16 *)RomPtrWithBank(db, j);
  if (*v2) {
    R24_ = *v2;
    uint16 v3 = j + 2;
    uint16 v4 = oam_next_ptr;
    while (1) {
      const uint8 *v5 = RomPtrWithBank(db, v3);
      v6 = R20_ + GET_WORD(v5);
      v7 = gOamEnt(v4);
      *(uint16 *)&v7->xcoord = v6;
      if ((v6 & 0x100) != 0) {
        int v8 = v4 >> 1;
        *(uint16 *)RomPtr_RAM(kOamExtra_Address_And_X8Large[v8]) |= kOamExtra_X8Small_And_Large[v8];
      }
      v10 = v5[2];
      if (v10 >= 0) {
        bool v11 = __CFADD__uint8((uint8)R18_, v10);
        v10 += R18_;
        if (!v11)
          goto LABEL_10;
        if (v11)
          goto LABEL_9;
      }
      bool v11;
      v11 = __CFADD__uint8((uint8)R18_, v10);
      v10 += R18_;
      if (!v11)
        LABEL_9: v10 = -16;
LABEL_10:
      v7->ycoord = v10;
      v12 = 0;
      if (*(int16 *)v5 < 0) {
        int v13 = v4 >> 1;
        *(uint16 *)RomPtr_RAM(kOamExtra_Address_And_X8Large[v13]) |= kOamExtra_X8Small_And_Large[v13 + 1];
      }
      *(uint16 *)&v7->charnum = R28_ | (R26_ + v12 + GET_WORD(v5 + 3));
      v4 = (v4 + 4) & 0x1FF;
      v3 += 5;
      if (!--R24_) {
        oam_next_ptr = v4;
        return;
      }
    }
  }
}

void DrawEnemyProjectileSpritemapWithBaseTileOffscreen(uint8 db, uint16 j) {  // 0x818C7F
  int16 v6;
  OamEnt *v7;
  char v10;
  char v12; // cf

  const uint16 *v2 = (const uint16 *)RomPtrWithBank(db, j);
  if (*v2) {
    R24_ = *v2;
    uint16 v3 = j + 2;
    uint16 v4 = oam_next_ptr;
    while (1) {
      const uint8 *v5 = RomPtrWithBank(db, v3);
      v6 = R20_ + GET_WORD(v5);
      v7 = gOamEnt(v4);
      *(uint16 *)&v7->xcoord = v6;
      if ((v6 & 0x100) != 0) {
        int v8 = v4 >> 1;
        *(uint16 *)RomPtr_RAM(kOamExtra_Address_And_X8Large[v8]) |= kOamExtra_X8Small_And_Large[v8];
      }
      v10 = v5[2];
      if (v10 >= 0) {
        bool v11 = __CFADD__uint8((uint8)R18_, v10);
        v10 += R18_;
        if (v11)
          goto LABEL_10;
        if (!v11)
          goto LABEL_9;
      }
      bool v11;
      v11 = __CFADD__uint8((uint8)R18_, v10);
      v10 += R18_;
      if (v11)
        LABEL_9:
      v10 = -16;
LABEL_10:
      v7->ycoord = v10;
      v12 = 0;
      if (*(int16 *)v5 < 0) {
        int v13 = v4 >> 1;
        *(uint16 *)RomPtr_RAM(kOamExtra_Address_And_X8Large[v13]) |= kOamExtra_X8Small_And_Large[v13 + 1];
      }
      *(uint16 *)&v7->charnum = R28_ | (R26_ + v12 + GET_WORD(v5 + 3));
      v4 = (v4 + 4) & 0x1FF;
      v3 += 5;
      if (!--R24_) {
        oam_next_ptr = v4;
        return;
      }
    }
  }
}

void GameOverMenu_0_FadeOutConfigGfx(void) {  // 0x818D0F
  HandleFadeOut();
  if ((reg_INIDISP & 0xF) == 0) {
    ScreenOff();
    QueueSfx3_Max6(1u);
    DisableHdmaObjects();
    WaitUntilEndOfVblankAndClearHdma();
    uint16 v0 = 0;
    do {
      ram3000.pause_menu_map_tilemap[v0 + 384] = palette_buffer[v0];
      ++v0;
    } while ((int16)(v0 * 2 - 512) < 0);
    uint16 v1 = 0;
    do {
      ram3000.pause_menu_map_tilemap[v1 + 640] = *(uint16 *)(&reg_INIDISP + v1 * 2);
      ++v1;
    } while ((int16)(v1 * 2 - 54) < 0);
    MapVramForMenu();
    LoadInitialMenuTiles();
    reg_BG1HOFS = 0;
    reg_BG2HOFS = 0;
    reg_BG3HOFS = 0;
    reg_BG1VOFS = 0;
    reg_BG2VOFS = 0;
    reg_BG3VOFS = 0;
    LoadMenuPalettes();
    ++menu_index;
  }
}

void GameOverMenu_24_FadeIn(void) {  // 0x818DA6
  HandleFadeIn();
  if ((reg_INIDISP & 0xF) == 15)
    ++menu_index;
}

void MapVramForMenu(void) {  // 0x818DBA
  reg_TS = 0;
  reg_OBSEL = 3;
  reg_BG1SC = 81;
  reg_BG2SC = 88;
  reg_BG3SC = 92;
  reg_BG12NBA = 0;
  reg_BG34NBA = 4;
  reg_TM = 19;
}

void LoadInitialMenuTiles(void) {  // 0x818DDB
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_818DF1 = { 1, 1, 0x18, LONGPTR(0x8e8000), 0x5600 };
  SetupDmaTransfer(&unk_818DF1);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x30);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_818E11 = { 1, 1, 0x18, LONGPTR(0xb68000), 0x2000 };
  SetupDmaTransfer(&unk_818E11);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x60);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_818E31 = { 1, 1, 0x18, LONGPTR(0xb6c000), 0x2000 };
  SetupDmaTransfer(&unk_818E31);
  WriteReg(MDMAEN, 2u);
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x40);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_818E51 = { 1, 1, 0x18, LONGPTR(0x8ed600), 0x0600 };
  SetupDmaTransfer(&unk_818E51);
  WriteReg(MDMAEN, 2u);
}

void LoadMenuPalettes(void) {  // 0x818E60
  uint16 v0 = 0;
  do {
    int v1 = v0 >> 1;
    palette_buffer[v1] = kMenuPalettes[v1];
    palette_buffer[v1 + 1] = kMenuPalettes[v1 + 1];
    v0 += 4;
  } while ((int16)(v0 - 512) < 0);
}

void LoadDebugGameOverMenuTilemaps(void) {  // 0x818E7F
  int v0 = 0;
  do
    ram3800.cinematic_bg_tilemap[v0++] = 15;
  while ((int16)(v0 * 2 - 2048) < 0);
  uint16 v1 = vram_write_queue_tail;
  VramWriteEntry *v2 = gVramWriteEntry(vram_write_queue_tail);
  v2->size = 2048;
  v2->src.addr = 14336;
  *(uint16 *)&v2->src.bank = 126;
  v2->vram_dst = (reg_BG1SC & 0xFC) << 8;
  vram_write_queue_tail = v1 + 7;
  uint16 v3 = v1 + 7;
  for (int i = 0; ; i += 8) {
    uint16 v5 = *(uint16 *)IndirPtr(&R0_, i);
    if (v5 == 0xFFFF)
      break;
    VramWriteEntry *v6 = gVramWriteEntry(v3);
    v6->size = v5;
    v6->src.addr = *(uint16 *)IndirPtr(&R0_, i + 2);
    *(uint16 *)&v6->src.bank = *(uint16 *)IndirPtr(&R0_, i + 4);
    v6->vram_dst = *(uint16 *)IndirPtr(&R0_, i + 6);
    v3 += 7;
  }
  vram_write_queue_tail = v3;
}

void GameOverMenu_3_Main(void) {  // 0x819003
  int16 v0;
  int16 v1;

  if ((joypad1_newkeys & kButton_Select) != 0
      || (joypad1_newkeys & kButton_Up) != 0
      || (joypad1_newkeys & kButton_Down) != 0) {
    file_select_map_area_index ^= 1u;
  } else if ((joypad1_newkeys & (uint16)(kButton_B | kButton_Start | kButton_A)) != 0) {
    if (file_select_map_area_index) {
      ++menu_index;
    } else {
      SaveToSram(selected_save_slot);
      SoftReset();
    }
    return;
  }
  v0 = 30720;
  if (file_select_map_area_index)
    v0 = -30720;
  v1 = v0 | 0x28;
  OamEnt *v3 = gOamEnt(oam_next_ptr);
  *(uint16 *)&v3->xcoord = v1;
  *(uint16 *)&v3->charnum = 182;
  oam_next_ptr += 4;
}

void RestorePalettesAndIoAfterDebugGameover(void) {  // 0x81905B
  int v0 = 0;
  do {
    palette_buffer[v0] = ram3000.pause_menu_map_tilemap[v0 + 384];
    ++v0;
  } while ((int16)(v0 * 2 - 512) < 0);
  int v1 = 0;
  do {
    *(uint16 *)(&reg_INIDISP + v1 * 2) = ram3000.pause_menu_map_tilemap[v1 + 640];
    ++v1;
  } while ((int16)(v1 * 2 - 54) < 0);
}

void GameOverMenu_5_Continue(void) {  // 0x81907E
  WriteReg(VMADDL, 0);
  WriteReg(VMADDH, 0x40);
  WriteReg(VMAIN, 0x80);
  static const StartDmaCopy unk_819093 = { 1, 1, 0x18, LONGPTR(0x9ab200), 0x2000 };
  SetupDmaTransfer(&unk_819093);
  WriteReg(MDMAEN, 2u);
  game_state = 16;
  file_select_map_area_index = 0;
  RestorePalettesAndIoAfterDebugGameover();
}
static Func_V *const kGameOverMenuFuncs[8] = {  // 0x8190AE
  GameOverMenu_0_FadeOutConfigGfx,
  GameOverMenu_1_Init,
  GameOverMenu_2_PlayMusic,
  GameOverMenu_3_FadeIn,
  GameOverMenu_4_Main,
  GameOverMenu_5_FadeOutToGameMap,
  GameOverMenu_6_LoadGameMapView,
  GameOverMenu_7_FadeOutToSoftReset,
};
void GameOverMenu(void) {
  kGameOverMenuFuncs[menu_index]();
}

void GameOverMenu_3_FadeIn(void) {  // 0x8190CD
  HandleGameOverBabyMetroid();
  DrawMenuSelectionMissile();
  HandleFadeIn();
  if ((reg_INIDISP & 0xF) == 15)
    ++menu_index;
}

void GameOverMenu_5_FadeOutToGameMap(void) {  // 0x8190E7
  HandleGameOverBabyMetroid();
  DrawMenuSelectionMissile();
  HandleFadeOut();
  if ((reg_INIDISP & 0xF) == 0)
    ++menu_index;
}

void GameOverMenu_7_FadeOutToSoftReset(void) {  // 0x8190FE
  DrawMenuSelectionMissile();
  HandleGameOverBabyMetroid();
  HandleFadeOut();
  if ((reg_INIDISP & 0xF) == 0)
    SoftReset();
}

void GameOverMenu_6_LoadGameMapView(void) {  // 0x819116
  DisableHdmaObjects();
  WaitUntilEndOfVblankAndClearHdma();
  game_state = kGameState_5_FileSelectMap;
  menu_index = 0;
}

void GameOverMenu_4_Main(void) {  // 0x81912B
  HandleGameOverBabyMetroid();
  DrawMenuSelectionMissile();
  if ((joypad1_newkeys & kButton_Select) != 0
      || (joypad1_newkeys & kButton_Up) != 0
      || (joypad1_newkeys & kButton_Down) != 0) {
    QueueSfx1_Max6(0x37u);
    file_select_map_area_index ^= 1u;
  } else if ((joypad1_newkeys & kButton_A) != 0) {
    enemy_data[0].instruction_timer = 180;
    if (file_select_map_area_index) {
      menu_index = 7;
    } else {
      if (loading_game_state == kGameState_31_SetUpNewGame)
        game_state = loading_game_state;
      else
        ++menu_index;
      LoadFromSram(selected_save_slot);
    }
    return;
  }
  uint16 v0 = 160;
  if (file_select_map_area_index)
    v0 = 192;
  enemy_projectile_id[5] = 40;
  enemy_projectile_id[10] = v0;
}

void GameOverMenu_1_Init(void) {  // 0x8191A4
  reg_TM = 17;
  reg_COLDATA[0] = 32;
  reg_COLDATA[1] = 64;
  reg_COLDATA[2] = 0x80;
  QueueMusic_Delayed8(0);
  QueueMusic_Delayed8(0xFF03u);
  enemy_projectile_enable_flag = 1;
  enemy_projectile_id[0] = 0;
  int v0 = 0;
  do {
    ((uint16*)ram3000.menu.menu_tilemap)[v0] = 15;
    ++v0;
  } while ((int16)(v0 * 2 - 2048) < 0);
  int v1 = vram_write_queue_tail;
  VramWriteEntry *v2 = gVramWriteEntry(vram_write_queue_tail);
  v2->size = 2048;
  v2->src.addr = ADDR16_OF_RAM(ram3000.menu.menu_tilemap);
  *(uint16 *)&v2->src.bank = 126;
  v2->vram_dst = (reg_BG1SC & 0xFC) << 8;
  vram_write_queue_tail = v1 + 7;
  enemy_data[0].palette_index = 0;
  LoadMenuTilemap(0x156, addr_kMenuTilemap_GameOver);
  LoadMenuTilemap(0x38A, addr_kMenuTilemap_FindMetroidLarva);
  LoadMenuTilemap(0x414, addr_kMenuTilemap_TryAgain);
  LoadMenuTilemap(0x4CE, addr_kMenuTilemap_YesReturnToGame);
  LoadMenuTilemap(0x5CE, addr_kMenuTilemap_NoGoToTitle);
  enemy_data[0].current_instruction = 0;
  enemy_data[0].instruction_timer = 0;
  HandleGameOverBabyMetroid();
  *(uint16 *)&reg_CGWSEL = gameplay_CGWSEL << 8;
  DisableHdmaObjects();
  WaitUntilEndOfVblankAndClearHdma();
  EnableHdmaObjects();
  static const SpawnHdmaObject_Args unk_819254 = { 0x00, 0x32, 0x927d };
  static const SpawnHdmaObject_Args unk_81925C = { 0x00, 0x31, 0x928d };
  SpawnHdmaObject(0x81, &unk_819254);
  SpawnHdmaObject(0x81, &unk_81925C);
  ScreenOn();
  ++menu_index;
  screen_fade_delay = 0;
  screen_fade_counter = 0;
  file_select_map_area_index = 0;
  enemy_projectile_id[5] = 40;
  enemy_projectile_id[10] = 160;
}

void GameOverMenu_2_PlayMusic(void) {  // 0x8193E8
  if (!(HasQueuedMusic())) {
    ++menu_index;
    QueueMusic_Delayed8(4u);
  }
}

static Func_V *const kFileSelectMenuFuncs[34] = {  // 0x8193FB
  FileSelectMenu_0_FadeOutConfigGfx,
  FileSelectMenu_1_LoadFileSelectMenuBG2,
  FileSelectMenu_2_InitMain,
  FileSelectMenu_3_FadeInToMain,
  FileSelectMenu_4_Main,
  FileSelectMenu_5_FadeOutFromMain,
  FileSelectMenu_6_FileCopyInit,
  FileSelectMenu_7_FadeInFromMain,
  FileSelectMenu_8,
  FileSelectMenu_9_InitializeSelectDest,
  FileSelectMenu_10_FileCopySelectDest,
  FileSelectMenu_11_InitializeConfirm,
  FileSelectMenu_12_FileCopyConfirm,
  FileSelectMenu_13_FileCopyDoIt,
  FileSelectMenu_14_CopyCompleted,
  FileSelectMenu_15_FadeOutToMain,
  FileSelectMenu_16,
  FileSelectMenu_17_FadeInToMain,
  FileSelectMenu_18,
  FileSelectMenu_5_FadeOutFromMain,
  FileSelectMenu_20_FileClearInit,
  FileSelectMenu_7_FadeInFromMain,
  FileSelectMenu_22_FileClearSelectSlot,
  FileSelectMenu_23_FileClearInitConfirm,
  FileSelectMenu_24_FileClearConfirm,
  FileSelectMenu_25_FileClearDoClear,
  FileSelectMenu_26_ClearCompleted,
  FileSelectMenu_15_FadeOutToMain,
  FileSelectMenu_16,
  FileSelectMenu_17_FadeInToMain,
  FileSelectMenu_30,
  FileSelectMenu_31_TurnSamusHelmet,
  FileSelectMenu_32_FadeOutToOptions,
  FileSelectMenu_33_FadeOutToTitle,
};

void FileSelectMenu(void) {
  kFileSelectMenuFuncs[menu_index]();
}

void FileSelectMenu_0_FadeOutConfigGfx(void) {  // 0x81944E
  HandleFadeOut();
  if ((reg_INIDISP & 0xF) != 0)
    return;
  ScreenOff();
  QueueSfx3_Max6(1u);
  DisableHdmaObjects();
  WaitUntilEndOfVblankAndClearHdma();
  ++menu_index;
  MapVramForMenu();
  LoadInitialMenuTiles();
  reg_BG1HOFS = 0;
  reg_BG2HOFS = 0;
  reg_BG3HOFS = 0;
  reg_BG1VOFS = 0;
  reg_BG2VOFS = 0;
  reg_BG3VOFS = 0;
  LoadFileSelectPalettes();
}

void LoadFileSelectPalettes(void) {  // 0x819486
  int v0 = 0;
  do {
    int v1 = v0 >> 1;
    palette_buffer[v1] = kMenuPalettes[v1];
    palette_buffer[v1 + 1] = kMenuPalettes[v1 + 1];
    v0 += 4;
  } while ((int16)(v0 - 512) < 0);

}

void FileSelectMenu_32_FadeOutToOptions(void) {  // 0x8194A3
  DrawMenuSelectionMissile();
  DrawBorderAroundSamusData();
  DrawFileSelectSamusHelmets();
  HandleFadeOut();
  if ((reg_INIDISP & 0xF) == 0) {
    game_state = kGameState_2_GameOptionsMenu;
    menu_index = 0;
    int v0 = 0;
    do {
      *(uint16 *)((char *)&enemy_projectile_enable_flag + v0) = 0;
      v0 += 2;
    } while ((int16)(v0 - 48) < 0);
  }
}

void FileSelectMenu_33_FadeOutToTitle(void) {  // 0x8194D5
  DrawBorderAroundSamusData();
  DrawFileSelectSamusHelmets();
  HandleFadeOut();
  if ((reg_INIDISP & 0xF) == 0)
    SoftReset();
}

void FileSelectMenu_5_FadeOutFromMain(void) {  // 0x8194EE
  DrawMenuSelectionMissile();
  FileSelectMenu_15_FadeOutToMain();
}

void FileSelectMenu_15_FadeOutToMain(void) {  // 0x8194F4
  HandleFadeOut();
  R18_ = *(uint16 *)&reg_MOSAIC & 0xFF0F;
  *(uint16 *)&reg_MOSAIC = *(uint16 *)&reg_MOSAIC & 0xFF0F | (16 * (reg_INIDISP & 0xF)) ^ 0xF0;
  if ((reg_INIDISP & 0xF) == 0) {
    ScreenOff();
    ++menu_index;
  }
}

static const uint16 kMenuSelectionMissileXY[12] = {  // 0x81951E
  0x30, 0xe,
  0x58, 0xe,
  0x80, 0xe,
  0xa3, 0xe,
  0xbb, 0xe,
  0xd3, 0xe,
};

void FileSelectMenu_17_FadeInToMain(void) {
  int v0 = (uint16)(4 * selected_save_slot) >> 1;
  enemy_projectile_id[10] = kMenuSelectionMissileXY[v0];
  enemy_projectile_id[5] = kMenuSelectionMissileXY[v0 + 1];
  FileSelectMenu_7_FadeInFromMain();
}

void FileSelectMenu_7_FadeInFromMain(void) {  // 0x819532
  DrawMenuSelectionMissile();
  HandleFadeIn();
  R18_ = *(uint16 *)&reg_MOSAIC & 0xFF0F;
  *(uint16 *)&reg_MOSAIC = *(uint16 *)&reg_MOSAIC & 0xFF0F | (16 * (reg_INIDISP & 0xF)) ^ 0xF0;
  if ((reg_INIDISP & 0xF) == 15)
    ++menu_index;
}

void FileSelectMenu_6_FileCopyInit(void) {  // 0x819561
  ++menu_index;
  FileSelectMenu_Func1();
}

void FileSelectMenu_Func1(void) {  // 0x819566
  ClearMenuTilemap();
  enemy_data[0].palette_index = 0;
  LoadMenuTilemap(0x52, addr_kMenuTilemap_DataCopyMode);
  LoadMenuTilemap(0x150, addr_kMenuTilemap_CopyWhichData);
  LoadMenuExitTilemap();
  DrawFileCopySaveFileInfo();
  SetInitialFileCopyMenuSelection();
  SetFileCopyMenuSelectionMissilePosition();
  enemy_projectile_id[16] = 0;
  enemy_projectile_id[17] = 0;
}

void sub_819591(void) {  // 0x819591
  SetInitialFileCopyMenuSelection();
}

void SetInitialFileCopyMenuSelection(void) {  // 0x819593
  int v0 = nonempty_save_slots;
  int v1 = 0;
  do {
    if (v0 & 1)
      break;
    v0 >>= 1;
    ++v1;
  } while ((int16)(v1 - 3) < 0);
  enemy_projectile_id[15] = v1;
}

void ClearMenuTilemap(void) {  // 0x8195A6
  for (int i = 2046; i >= 0; i -= 2)
    *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[768] + (uint16)i) = 15;
}

void LoadMenuExitTilemap(void) {  // 0x8195B5
  LoadMenuTilemap(0x688, addr_kMenuTilemap_Exit);
}

void DrawFileCopySaveFileInfo(void) {  // 0x8195BE
  LoadFromSram_(0);
  enemy_data[0].palette_index = 0;
  if ((nonempty_save_slots & 1) == 0)
    enemy_data[0].palette_index = 1024;
  DrawFileCopySaveSlotAInfo();
  LoadFromSram_(1);
  enemy_data[0].palette_index = 0;
  if ((nonempty_save_slots & 2) == 0)
    enemy_data[0].palette_index = 1024;
  DrawFileCopySaveSlotBInfo();
  LoadFromSram_(2);
  enemy_data[0].palette_index = 0;
  if ((nonempty_save_slots & 4) == 0)
    enemy_data[0].palette_index = 1024;
  DrawFileCopySaveSlotCInfo();
  QueueTransferOfMenuTilemapToVramBG1();
}

void DrawFileCopySaveSlotAInfo(void) {  // 0x81960F
  DrawFileSelectionHealth((nonempty_save_slots & 1) == 0, 0x218);
  DrawFileSelectionTime((nonempty_save_slots & 1) == 0, 0x272);
  LoadMenuTilemap(0x234, addr_kMenuTilemap_TIME);
  LoadMenuTilemap(0x208, addr_kMenuTilemap_SamusA);
}

void DrawFileCopySaveSlotBInfo(void) {  // 0x81963F
  DrawFileSelectionHealth(~(uint8)nonempty_save_slots & 2, 0x318);
  DrawFileSelectionTime(~(uint8)nonempty_save_slots & 2, 0x372);
  LoadMenuTilemap(0x334, addr_kMenuTilemap_TIME);
  LoadMenuTilemap(0x308, addr_kMenuTilemap_SamusB);
}

void DrawFileCopySaveSlotCInfo(void) {  // 0x81966F
  DrawFileSelectionHealth(~(uint8)nonempty_save_slots & 4, 0x418);
  DrawFileSelectionTime(~(uint8)nonempty_save_slots & 4, 0x472);
  LoadMenuTilemap(0x434, addr_kMenuTilemap_TIME);
  LoadMenuTilemap(0x408, addr_kMenuTilemap_SamusC);
}

void QueueTransferOfMenuTilemapToVramBG1(void) {  // 0x81969F
  uint16 v0 = vram_write_queue_tail;
  VramWriteEntry *v1 = gVramWriteEntry(vram_write_queue_tail);
  v1->size = 2048;
  v1->src.addr = ADDR16_OF_RAM(ram3000.menu.menu_tilemap);
  *(uint16 *)&v1->src.bank = 126;
  v1->vram_dst = (reg_BG1SC & 0xFC) << 8;
  vram_write_queue_tail = v0 + 7;
}

static const uint8 kBitShl[3] = {  // 0x8196C2
  1,
  2,
  4,
};

void FileSelectMenu_8(void) {
  uint8 v0;

  DrawBorderAroundDataCopyMode();
  DrawMenuSelectionMissile();
  if ((joypad1_newkeys & (kButton_Start | kButton_A)) != 0) {
    QueueSfx1_Max6(0x37u);
    if (enemy_projectile_id[15] == 3) {
      menu_index += 7;
    } else {
      enemy_projectile_id[16] = enemy_projectile_id[15];
      ++menu_index;
    }
  } else if ((joypad1_newkeys & 0x8000u) != 0) {
    menu_index += 7;
    QueueSfx1_Max6(0x37u);
  } else {
    if ((joypad1_newkeys & kButton_Up) != 0) {
      v0 = enemy_projectile_id[15];
      while ((--v0 & 0x80) == 0) {
        if ((kBitShl[v0] & nonempty_save_slots) != 0) {
LABEL_16:
          LOBYTE(enemy_projectile_id[15]) = v0;
          QueueSfx1_Max6(0x37u);
          SetFileCopyMenuSelectionMissilePosition();
          return;
        }
      }
    } else if ((joypad1_newkeys & kButton_Down) != 0) {
      v0 = enemy_projectile_id[15];
      while ((int8)(++v0 - 4) < 0) {
        if (v0 == 3 || (kBitShl[v0] & nonempty_save_slots) != 0)
          goto LABEL_16;
      }
    }
    SetFileCopyMenuSelectionMissilePosition();
  }
}

void SetFileCopyMenuSelectionMissilePosition(void) {  // 0x81975E
  static const uint16 kFileCopyMissileY[4] = { 72, 104, 136, 211 };
  enemy_projectile_id[10] = kFileCopyMissileY[enemy_projectile_id[15]];
  enemy_projectile_id[5] = 22;
}

void FileSelectMenu_9_InitializeSelectDest(void) {  // 0x81977A
  DrawBorderAroundDataCopyMode();
  DrawFileCopySelectDestinationSaveFileInfo();
  ++menu_index;
  uint16 v0 = 0;
  do {
    if (v0 != enemy_projectile_id[16])
      break;
    ++v0;
  } while (sign16(v0 - 3));
  enemy_projectile_id[15] = v0;
  SetFileCopyMenuSelectionMissilePosition();
}

void DrawFileCopySelectDestinationSaveFileInfo(void) {  // 0x819799
  ClearMenuTilemap();
  enemy_data[0].palette_index = 0;
  LoadMenuTilemap(0x52, addr_kMenuTilemap_DataCopyMode);
  enemy_data[0].palette_index = 0;
  LoadMenuTilemap(0x148, addr_kMenuTilemap_CopySamusToWhere);
  ram3000.pause_menu_map_tilemap[944] = enemy_projectile_id[16] + 8298;
  LoadMenuExitTilemap();
  LoadFromSram_(0);
  uint16 v0 = 1024;
  if (enemy_projectile_id[16])
    v0 = 0;
  enemy_data[0].palette_index = v0;
  DrawFileCopySaveSlotAInfo();
  LoadFromSram_(1);
  uint16 v1 = 1024;
  if (enemy_projectile_id[16] != 1)
    v1 = 0;
  enemy_data[0].palette_index = v1;
  DrawFileCopySaveSlotBInfo();
  LoadFromSram_(2);
  uint16 v2 = 1024;
  if (enemy_projectile_id[16] != 2)
    v2 = 0;
  enemy_data[0].palette_index = v2;
  DrawFileCopySaveSlotCInfo();
  QueueTransferOfMenuTilemapToVramBG1();
}

void FileSelectMenu_10_FileCopySelectDest(void) {  // 0x819813
  static const uint16 kFileCopySelectDest_MissileY[4] = { 72, 104, 136, 212 };
  uint16 v0;

  DrawBorderAroundDataCopyMode();
  DrawMenuSelectionMissile();
  if ((joypad1_newkeys & (kButton_Start | kButton_A)) != 0) {
    QueueSfx1_Max6(0x37u);
    if (enemy_projectile_id[15] != 3) {
      enemy_projectile_id[17] = enemy_projectile_id[15];
      ++menu_index;
LABEL_9:
      enemy_projectile_id[10] = kFileCopySelectDest_MissileY[enemy_projectile_id[15]];
      enemy_projectile_id[5] = 22;
      return;
    }
    menu_index += 5;
  } else {
    if ((joypad1_newkeys & 0x8000u) == 0) {
      if ((joypad1_newkeys & kButton_Up) != 0) {
        QueueSfx1_Max6(0x37u);
        v0 = enemy_projectile_id[15];
        while ((--v0 & 0x8000u) == 0) {
          if (v0 != enemy_projectile_id[16]) {
LABEL_8:
            enemy_projectile_id[15] = v0;
            goto LABEL_9;
          }
        }
      } else if ((joypad1_newkeys & kButton_Down) != 0) {
        QueueSfx1_Max6(0x37u);
        v0 = enemy_projectile_id[15];
        while (++v0 != 4) {
          if (v0 != enemy_projectile_id[16])
            goto LABEL_8;
        }
      }
      goto LABEL_9;
    }
    menu_index -= 2;
    enemy_projectile_id[15] = enemy_projectile_id[16];
    QueueSfx1_Max6(0x37u);
    FileSelectMenu_Func1();
  }
}

void FileSelectMenu_11_InitializeConfirm(void) {  // 0x8198B7
  DrawBorderAroundDataCopyMode();
  enemy_data[0].palette_index = 0;
  LoadMenuTilemap(0x144, addr_kMenuTilemap_CopySamusToSamus);
  ram3000.pause_menu_map_tilemap[942] = enemy_projectile_id[16] + 8298;
  ram3000.pause_menu_map_tilemap[955] = enemy_projectile_id[17] + 8298;
  DrawFileCopyClearConfirmation();
  ++menu_index;
  enemy_projectile_id[15] = 0;
  enemy_projectile_unk198F = 8;
}

void DrawFileCopyClearConfirmation(void) {  // 0x8198ED
  enemy_data[0].palette_index = 0;
  LoadMenuTilemap(0x514, addr_kMenuTilemap_IsThisOk);
  enemy_data[0].palette_index = 0;
  LoadMenuTilemap(0x59C, addr_kMenuTilemap_Yes);
  int v0 = 832;
  do {
    ram3000.pause_menu_map_tilemap[v0 + 768] = 15;
    ++v0;
  } while ((int16)(v0 * 2 - 1728) < 0);
  enemy_data[0].palette_index = 0;
  LoadMenuTilemap(0x65C, addr_kMenuTilemap_No);
  DrawFileCopyConfirmationSaveFileInfo();
}

void DrawFileCopyConfirmationSaveFileInfo(void) {  // 0x819922
  LoadFromSram_(0);
  int v0 = 0;
  if (enemy_projectile_id[16] && enemy_projectile_id[17])
    v0 = 1024;
  enemy_data[0].palette_index = v0;
  DrawFileCopySaveSlotAInfo();
  LoadFromSram_(1);
  int v1 = 0;
  if (enemy_projectile_id[16] != 1 && enemy_projectile_id[17] != 1)
    v1 = 1024;
  enemy_data[0].palette_index = v1;
  DrawFileCopySaveSlotBInfo();
  LoadFromSram_(2);
  int v2 = 0;
  if (enemy_projectile_id[16] != 2 && enemy_projectile_id[17] != 2)
    v2 = 1024;
  enemy_data[0].palette_index = v2;
  DrawFileCopySaveSlotCInfo();
  QueueTransferOfMenuTilemapToVramBG1();
}

void FileSelectMenu_12_FileCopyConfirm(void) {  // 0x819984
  DrawBorderAroundDataCopyMode();
  DrawMenuSelectionMissile();
  HandleFileCopyArrowPalette();
  DrawFileCopyArrow();
  if ((joypad1_newkeys & (kButton_Up | kButton_Down)) != 0) {
    enemy_projectile_id[15] ^= 1u;
    QueueSfx1_Max6(0x37u);
  } else {
    if ((joypad1_newkeys & 0x8000u) != 0) {
      menu_index -= 3;
      enemy_projectile_id[15] = enemy_projectile_id[17];
      QueueSfx1_Max6(0x37u);
      return;
    }
    if ((joypad1_newkeys & (kButton_Start | kButton_A)) != 0) {
      QueueSfx1_Max6(0x38u);
      if (enemy_projectile_id[15]) {
        menu_index -= 4;
        FileSelectMenu_Func1();
      } else {
        ++menu_index;
      }
      return;
    }
  }
  uint16 v0 = 184;
  if (enemy_projectile_id[15])
    v0 = 208;
  enemy_projectile_id[10] = v0;
  enemy_projectile_id[5] = 94;
}

void HandleFileCopyArrowPalette(void) {  // 0x8199FE
  if (enemy_projectile_unk198F) {
    if (!--enemy_projectile_unk198F) {
      enemy_projectile_unk198F = 4;
      uint16 v0 = palette_buffer[145];
      uint16 v1 = 0;
      do {
        palette_buffer[(v1 >> 1) + 145] = palette_buffer[(v1 >> 1) + 146];
        v1 += 2;
      } while ((int16)(v1 - 12) < 0);
      palette_buffer[151] = v0;
    }
  }
}

void FileSelectMenu_13_FileCopyDoIt(void) {  // 0x819A2C
  static const uint16 kBitShl_16bit[3] = { 1, 2, 4 };

  DrawBorderAroundDataCopyMode();
  DrawMenuSelectionMissile();
  HandleFileCopyArrowPalette();
  DrawFileCopyArrow();
  *(uint16 *)&R0_.bank = 112;
  *(uint16 *)&R3_.bank = 112;
  uint16 src_addr = kOffsetToSaveSlot[enemy_projectile_id[16]];
  uint16 dst_addr = kOffsetToSaveSlot[enemy_projectile_id[17]];
  memcpy(&g_sram[dst_addr], &g_sram[src_addr], 1628);
  int v2 = enemy_projectile_id[16];
  int v10 = *(uint16 *)(&g_sram[2 * v2 + 0x1FF0]);
  int v9 = *(uint16 *)(&g_sram[2 * v2 + 0x1FF8]);
  int v8 = *(uint16 *)(&g_sram[2 * v2 + 0]);
  int v4 = enemy_projectile_id[17];
  *(uint16 *)(&g_sram[2 * v4 + 8]) = *(uint16 *)&g_sram[2 * v2 + 8];
  *(uint16 *)(&g_sram[2 * v4]) = v8;
  *(uint16 *)(&g_sram[2 * v4 + 0x1FF8]) = v9;
  *(uint16 *)(&g_sram[2 * v4 + 0x1FF0]) = v10;
  ++menu_index;
  int v5 = 640;
  do {
    ram3000.pause_menu_map_tilemap[v5 + 768] = 15;
    ++v5;
  } while ((int16)(v5 * 2 - 1856) < 0);
  nonempty_save_slots |= kBitShl_16bit[enemy_projectile_id[17]];
  int v6 = ((4 * enemy_projectile_id[17] + 9) << 6) + 24;
  int v7 = 0;
  do {
    *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[768] + v6) = 15;
    *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[736] + v6) = 15;
    v6 += 2;
    v7 += 2;
  } while ((int16)(v7 - 22) < 0);
  enemy_data[0].palette_index = 0;
  LoadMenuTilemap(0x510, addr_kMenuTilemap_CopyCompleted);
  DrawFileCopyConfirmationSaveFileInfo();
  RtlWriteSram();
}

void FileSelectMenu_14_CopyCompleted(void) {  // 0x819AFA
  DrawBorderAroundDataCopyMode();
  if (joypad1_newkeys) {
    QueueSfx1_Max6(0x37u);
    ++menu_index;
    int slot = sram_save_slot_selected;
    if (sign16(slot) || !sign16(sram_save_slot_selected - 3) ||
        (slot & sram_save_slot_selected_complement)) {
      slot = 0;
    }
    selected_save_slot = slot;
  }
}

void FileSelectMenu_18(void) {  // 0x819B28
  menu_index -= 14;
}

void FileSelectMenu_20_FileClearInit(void) {  // 0x819B33
  DrawBorderAroundDataClearMode();
  ++menu_index;
  InitFileSelectMenuFileClear();
}

void InitFileSelectMenuFileClear(void) {  // 0x819B3C
  ClearMenuTilemap();
  enemy_data[0].palette_index = 0;
  LoadMenuTilemap(0x50, addr_kMenuTilemap_DataClearMode);
  LoadMenuTilemap(0x140, addr_kMenuTilemap_ClearWhichData);
  LoadMenuExitTilemap();
  DrawFileCopySaveFileInfo();
  enemy_projectile_id[16] = 0;
  SetInitialFileCopyMenuSelection();
  SetFileClearMenuMissilePos();
}

void FileSelectMenu_22_FileClearSelectSlot(void) {  // 0x819B64
  static const uint8 kBitShl_[3] = { 1, 2, 4 };
  uint16 v0;
  uint8 v1;

  DrawBorderAroundDataClearMode();
  DrawMenuSelectionMissile();
  HIBYTE(v0) = HIBYTE(joypad1_newkeys);
  if ((joypad1_newkeys & (kButton_Start | kButton_A)) != 0) {
    QueueSfx1_Max6(0x37u);
    if (enemy_projectile_id[15] != 3) {
      enemy_projectile_id[16] = enemy_projectile_id[15];
      ++menu_index;
      return;
    }
LABEL_11:
    QueueSfx1_Max6(0x37u);
    menu_index += 5;
    return;
  }
  if ((joypad1_newkeys & 0x8000u) != 0)
    goto LABEL_11;
  if ((joypad1_newkeys & kButton_Up) != 0) {
    v1 = enemy_projectile_id[15];
    while ((--v1 & 0x80) == 0) {
      if ((kBitShl_[v1] & (uint8)nonempty_save_slots) != 0) {
LABEL_16:
        LOBYTE(enemy_projectile_id[15]) = v1;
        LOBYTE(v0) = 55;
        QueueSfx1_Max6(v0);
        SetFileClearMenuMissilePos();
        return;
      }
    }
  } else if ((joypad1_newkeys & kButton_Down) != 0) {
    v1 = enemy_projectile_id[15];
    while ((int8)(++v1 - 4) < 0) {
      if (v1 == 3 || (kBitShl_[v1] & (uint8)nonempty_save_slots) != 0)
        goto LABEL_16;
    }
  }
  SetFileClearMenuMissilePos();
}

void SetFileClearMenuMissilePos(void) {  // 0x819BEF
  static const uint16 kFileClear_MissileY[4] = { 72, 104, 136, 211 };
  enemy_projectile_id[10] = kFileClear_MissileY[enemy_projectile_id[15]];
  enemy_projectile_id[5] = 22;
}

void FileSelectMenu_23_FileClearInitConfirm(void) {  // 0x819C0B
  DrawBorderAroundDataClearMode();
  enemy_data[0].palette_index = 0;
  LoadMenuTilemap(0x140, addr_kMenuTilemap_ClearSamusA);
  ram3000.pause_menu_map_tilemap[949] = enemy_projectile_id[16] + 8298;
  enemy_projectile_id[17] = 3;
  DrawFileCopyClearConfirmation();
  ++menu_index;
  enemy_projectile_id[15] = 0;
}

void FileSelectMenu_24_FileClearConfirm(void) {  // 0x819C36
  uint16 v0;
  DrawBorderAroundDataClearMode();
  DrawMenuSelectionMissile();
  if ((joypad1_newkeys & (kButton_Up | kButton_Down)) != 0) {
    enemy_projectile_id[15] ^= 1u;
    QueueSfx1_Max6(0x37u);
LABEL_8:
    v0 = 184;
    if (enemy_projectile_id[15])
      v0 = 208;
    enemy_projectile_id[10] = v0;
    enemy_projectile_id[5] = 94;
    return;
  }
  if ((joypad1_newkeys & 0x8000u) != 0) {
LABEL_5:
    menu_index -= 2;
    enemy_projectile_id[15] = enemy_projectile_id[16];
    QueueSfx1_Max6(0x37u);
    InitFileSelectMenuFileClear();
    return;
  }
  if ((joypad1_newkeys & (kButton_Start | kButton_A)) == 0)
    goto LABEL_8;
  QueueSfx1_Max6(0x38u);
  if (enemy_projectile_id[15])
    goto LABEL_5;
  ++menu_index;
}

void FileSelectMenu_25_FileClearDoClear(void) {  // 0x819C9E
  static const uint16 kBitShl_Not[3] = { 0xfffe, 0xfffd, 0xfffb };

  DrawBorderAroundDataClearMode();
  *(uint16 *)&R0_.bank = 112;
  int sram_addr = kOffsetToSaveSlot[enemy_projectile_id[16]];
  memset(&g_sram[sram_addr], 0, 1628);

  uint16 v1 = 2 * enemy_projectile_id[16];
  int v2 = (uint16)(2 * enemy_projectile_id[16]) >> 1;
  *(uint16 *)(&g_sram[2 * v2]) = 0;
  *(uint16 *)(&g_sram[2 * v2 + 8]) = 0;
  *(uint16 *)(&g_sram[2 * v2 + 0x1FF0]) = 0;
  *(uint16 *)(&g_sram[2 * v2 + 0x1FF8]) = 0;
  ++menu_index;
  NewSaveFile();
  LoadFromSram(enemy_projectile_id[16]);
  area_index = enemy_projectile_id[16];
  LoadMirrorOfExploredMapTiles();
  uint16 v3 = 640;
  do {
    ram3000.pause_menu_map_tilemap[v3 + 768] = 15;
    ++v3;
  } while ((int16)(v3 * 2 - 1856) < 0);
  nonempty_save_slots &= kBitShl_Not[enemy_projectile_id[16]];
  enemy_data[0].palette_index = 0;
  LoadMenuTilemap(0x500, addr_kMenuTilemap_DataCleared);
  DrawFileCopyConfirmationSaveFileInfo();
  RtlWriteSram();
}

void FileSelectMenu_26_ClearCompleted(void) {  // 0x819D26
  DrawBorderAroundDataClearMode();
  if (joypad1_newkeys) {
    QueueSfx1_Max6(0x37u);
    ++menu_index;
    if (!(LoadFromSram(0))) {
LABEL_3:
      selected_save_slot = 0;
      return;
    }
    if (LoadFromSram(1)) {
      if (LoadFromSram(2))
        goto LABEL_3;
      selected_save_slot = 2;
    } else {
      selected_save_slot = 1;
    }
  }
}

void FileSelectMenu_30(void) {  // 0x819D68
  DrawBorderAroundSamusData();
  menu_index -= 26;
}

void FileSelectMenu_31_TurnSamusHelmet(void) {  // 0x819D77
  DrawMenuSelectionMissile();
  DrawBorderAroundSamusData();
  DrawFileSelectSlotSamusHelmet(4);
  DrawFileSelectSlotSamusHelmet(6);
  DrawFileSelectSlotSamusHelmet(8);
  if ((joypad1_newkeys & (kButton_Start | kButton_A)) != 0
      || enemy_projectile_id[2] == 7 && !enemy_projectile_index
      || enemy_projectile_id[3] == 7 && !enemy_projectile_init_param
      || enemy_projectile_id[4] == 7 && !enemy_projectile_unk1995) {
    ++menu_index;
  }
}

void DrawFileSelectSamusHelmets(void) {  // 0x819DC3
  enemy_projectile_index = 0;
  DrawFileSelectSlotSamusHelmet(4);
  enemy_projectile_init_param = 0;
  DrawFileSelectSlotSamusHelmet(6);
  enemy_projectile_unk1995 = 0;
  DrawFileSelectSlotSamusHelmet(8);
}

void DrawFileSelectSlotSamusHelmet(uint16 k) {  // 0x819DE4
  uint16 v0 = k;
  int16 v1;

  v1 = *(uint16 *)((char *)&enemy_projectile_enable_flag + v0);
  if (v1) {
    uint16 v2 = v1 - 1;
    *(uint16 *)((char *)&enemy_projectile_enable_flag + v0) = v2;
    if (!v2) {
      *(uint16 *)((char *)&enemy_projectile_enable_flag + v0) = 8;
      int v3 = v0 >> 1;
      uint16 v4 = enemy_projectile_id[v3] + 1;
      if (!sign16(enemy_projectile_id[v3] - 7)) {
        *(uint16 *)((char *)&enemy_projectile_enable_flag + v0) = 0;
        v4 = 7;
      }
      enemy_projectile_id[v3] = v4;
    }
  }
  int v5 = v0 >> 1;
  uint16 v6 = 2 * enemy_projectile_id[v5];
  R3_.addr = 3584;
  static const uint16 kDrawFileSlotHelmet_Spritemaps[9] = { 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x33 };
  DrawMenuSpritemap(kDrawFileSlotHelmet_Spritemaps[v6 >> 1], enemy_projectile_id[v5 + 5], enemy_projectile_id[v5 + 10]);
}
static Func_V *const kFileSelectMapFuncs[23] = {  // 0x819E3E
  FileSelectMap_0,
  FileSelectMap_1,
  FileSelectMap_2_LoadAreaSelectForegroundTilemap,
  FileSelectMap_3_LoadAreaSelectBackgroundTilemap,
  FileSelectMap_4_SetupExpandingSquareTransition,
  FileSelectMap_5_ExpandingSquare,
  FileSelectMap_6_AreaSelectMap,
  FileSelectMap_7_PrepExpandSquareTransToRoomMap,
  FileSelectMap_8_ExpandSquareTransToRoomSelectMap,
  FileSelectMap_9_InitRoomSelectMap,
  FileSelectMap_10_RoomSelectMap,
  FileSelectMap_11,
  FileSelectMap_11,
  FileSelectMap_13,
  FileSelectMap_14,
  FileSelectMap_15_ClearTileMap,
  FileSelectMap_16_LoadPalettes,
  FileSelectMap_2_LoadAreaSelectForegroundTilemap,
  FileSelectMap_18,
  FileSelectMap_4_SetupExpandingSquareTransition,
  FileSelectMap_20_SetupExpandingSquare,
  FileSelectMap_21_MoveExpandingSquare,
  FileSelectMap_22,
};
void FileSelectMap(void) {
  kFileSelectMapFuncs[menu_index]();
}

void FileSelectMap_22(void) {  // 0x819E7B
  DrawAreaSelectMapLabels();
  HandleFadeOut();
  if ((reg_INIDISP & 0xF) == 0) {
    game_state = kGameState_2_GameOptionsMenu;
    menu_index = 0;
  }
}

void FileSelectMenu_1_LoadFileSelectMenuBG2(void) {  // 0x819E93
  VramWriteEntry *v2;

  for (int i = 1023; i >= 0; --i)
    ram3000.pause_menu_map_tilemap[i + 768] = kZebesAndStarsTilemap[i];
  uint16 v1 = vram_write_queue_tail;
  v2 = gVramWriteEntry(vram_write_queue_tail);
  v2->size = 2048;
  v2->src.addr = ADDR16_OF_RAM(ram3000.menu.menu_tilemap);
  *(uint16 *)&v2->src.bank = 126;
  v2->vram_dst = (reg_BG2SC & 0xFC) << 8;
  vram_write_queue_tail = v1 + 7;
  ++menu_index;
  enemy_projectile_enable_flag = 1;
  enemy_projectile_id[0] = 0;
}

void FileSelectMenu_2_InitMain(void) {  // 0x819ED6
  uint16 v0;

  if (sign16(sram_save_slot_selected)
      || !sign16(sram_save_slot_selected - 3)
      || (v0 = sram_save_slot_selected, (sram_save_slot_selected_complement & sram_save_slot_selected) != 0)) {
    v0 = 0;
  }
  selected_save_slot = v0;
  FileSelectMenu_16();
}

void FileSelectMenu_16(void) {  // 0x819EF3
  for (int i = 2046; i >= 0; i -= 2)
    *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[768] + (uint16)i) = 15;
  nonempty_save_slots = -1;
  enemy_data[0].palette_index = 0;
  LoadMenuTilemap(0x56, addr_kMenuTilemap_SamusData);
  LoadMenuTilemap(0x146, addr_kMenuTilemap_SamusA);
  uint8 c = LoadFromSram_(0);
  nonempty_save_slots = (nonempty_save_slots >> 1) | (c << 15);
  DrawFileSelectionHealth(nonempty_save_slots & 0x8000, 0x15C);
  DrawFileSelectionTime(nonempty_save_slots & 0x8000, 0x1B4);
  LoadMenuTilemap(0x176, addr_kMenuTilemap_TIME);
  enemy_data[0].palette_index = 0;
  LoadMenuTilemap(0x286, addr_kMenuTilemap_SamusB);
  c = LoadFromSram_(1u);
  nonempty_save_slots = (nonempty_save_slots >> 1) | (c << 15);
  DrawFileSelectionHealth(nonempty_save_slots & 0x8000, 0x29C);
  DrawFileSelectionTime(nonempty_save_slots & 0x8000, 0x2F4);
  LoadMenuTilemap(0x2B6, addr_kMenuTilemap_TIME);
  enemy_data[0].palette_index = 0;
  LoadMenuTilemap(0x3C6, addr_kMenuTilemap_SamusC);
  c = LoadFromSram_(2u);
  nonempty_save_slots = (nonempty_save_slots >> 1) | (c << 15);
  DrawFileSelectionHealth(nonempty_save_slots & 0x8000, 0x3DC);
  DrawFileSelectionTime(nonempty_save_slots & 0x8000, 0x434);
  LoadMenuTilemap(0x3F6, addr_kMenuTilemap_TIME);
  nonempty_save_slots = swap16(~nonempty_save_slots) >> 5;
  if (nonempty_save_slots) {
    enemy_data[0].palette_index = 0;
    LoadMenuTilemap(0x508, addr_kMenuTilemap_DataCopy);
    enemy_data[0].palette_index = 0;
    LoadMenuTilemap(0x5C8, addr_kMenuTilemap_DataClear);
  }
  enemy_data[0].palette_index = 0;
  LoadMenuTilemap(0x688, addr_kMenuTilemap_Exit);
  QueueTransferOfMenuTilemapToVramBG1();
  enemy_projectile_enable_flag = 1;
  enemy_projectile_unk198F = 0;
  enemy_projectile_index = 0;
  enemy_projectile_init_param = 0;
  enemy_projectile_unk1995 = 0;
  enemy_projectile_id[0] = 0;
  enemy_projectile_id[1] = 0;
  enemy_projectile_id[2] = 0;
  enemy_projectile_id[3] = 0;
  enemy_projectile_id[4] = 0;
  enemy_projectile_id[5] = 0;
  enemy_projectile_id[10] = 0;
  enemy_projectile_id[6] = 0;
  enemy_projectile_id[11] = 0;
  enemy_projectile_id[7] = 100;
  enemy_projectile_id[8] = 100;
  enemy_projectile_id[9] = 100;
  enemy_projectile_id[12] = 47;
  enemy_projectile_id[13] = 87;
  enemy_projectile_id[14] = 127;
  screen_fade_delay = 1;
  screen_fade_counter = 1;
  ScreenOn();
  ++menu_index;
  enemy_projectile_id[15] = 0;
  enemy_projectile_id[16] = 0;
  enemy_projectile_id[17] = 0;
}

uint8 LoadFromSram_(uint16 a) {  // 0x81A053
  return LoadFromSram(a);
}

void FileSelectMenu_3_FadeInToMain(void) {  // 0x81A058
  DrawFileSelectSamusHelmets();
  int v0 = (uint16)(4 * selected_save_slot) >> 1;
  enemy_projectile_id[10] = kMenuSelectionMissileXY[v0];
  enemy_projectile_id[5] = kMenuSelectionMissileXY[v0 + 1];
  DrawMenuSelectionMissile();
  DrawBorderAroundSamusData();
  HandleFadeIn();
  if ((reg_INIDISP & 0xF) == 15)
    ++menu_index;
}

void DrawFileSelectionHealth(uint16 a, uint16 k) {  // 0x81A087
  int16 v2;
  int16 v4;
  unsigned int v5;

  if (a) {
    FileSelectClearRestOfMenuTilemapRow(k);
    LoadMenuTilemap(k + 64, addr_kMenuTilemap_NoData);
  } else {
    R26_ = k;
    LoadMenuTilemap(k, addr_kMenuTilemap_Energy);
    v2 = R26_ + 8;

    R20_ = samus_health / 100;
    R18_ = samus_health % 100;
    R22_ = samus_max_health / 100;
    R24_ = 7;
    uint16 v3 = v2 + 64;
    while ((--R22_ & 0x8000u) == 0) {
      v4 = 153;
      if (R20_) {
        --R20_;
        v4 = 152;
      }
      *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[768] + v3) = enemy_data[0].palette_index | v4;
      v3 += 2;
      if (!--R24_) {
        v3 -= 78;
        R24_ = 8;
      }
    }
    R20_ = R18_ / 10;
    R18_ = R18_ % 10;
    v5 = R26_;
    *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[802] + R26_) = enemy_data[0].palette_index | (R18_ + 8288);
    *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[801] + v5) = enemy_data[0].palette_index | (R20_ + 8288);
  }
}

void DrawFileSelectionTime(uint16 a, uint16 k) {  // 0x81A14E
  if (!a) {
    R26_ = k;
    int div_val = game_time_hours / 10;
    int mod_val = game_time_hours % 10;

    uint16 v2 = R26_;
    *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[769] + v2) = enemy_data[0].palette_index | (mod_val + 8288);
    *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[768] + v2) = enemy_data[0].palette_index | (div_val + 8288);
    LoadMenuTilemap(R26_ + 4, addr_word_81B4A8);

    int div_min = game_time_minutes / 10;
    int mod_min = game_time_minutes % 10;

    uint16 v3 = R26_;
    *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[772] + v3) = enemy_data[0].palette_index | (mod_min + 8288);
    *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[771] + v3) = enemy_data[0].palette_index | (div_min + 8288);
  }
}

void FileSelectMenu_4_Main(void) {  // 0x81A1C2
  int16 v0;
  uint16 v1, v2;
  int v3;

  DrawBorderAroundSamusData();
  DrawMenuSelectionMissile();
  DrawFileSelectSlotSamusHelmet(4);
  DrawFileSelectSlotSamusHelmet(6);
  DrawFileSelectSlotSamusHelmet(8);
  if ((joypad1_newkeys & (kButton_Start | kButton_A)) == 0) {
    if ((joypad1_newkeys & kButton_Up) != 0) {
      if (nonempty_save_slots) {
        v1 = selected_save_slot - 1;
        if ((int16)(selected_save_slot - 1) < 0)
          v1 = 5;
      } else {
        v1 = selected_save_slot - 1;
        if ((int16)(selected_save_slot - 1) >= 0) {
          if (!sign16(selected_save_slot - 5))
            v1 = 2;
        } else {
          v1 = 5;
        }
      }
      selected_save_slot = v1;
    } else {
      if ((joypad1_newkeys & kButton_Down) == 0) {
        if ((joypad1_newkeys & kButton_B) != 0) {
          QueueSfx1_Max6(0x37u);
          menu_index = 33;
          QueueSfx1_Max6(0x37u);
        }
        goto LABEL_28;
      }
      if (nonempty_save_slots) {
        v2 = selected_save_slot + 1;
        if (!sign16(selected_save_slot - 5))
          v2 = 0;
      } else {
        v2 = selected_save_slot + 1;
        if (!sign16(selected_save_slot - 2)) {
          if (sign16(selected_save_slot - 5))
            v2 = 5;
          else
            v2 = 0;
        }
      }
      selected_save_slot = v2;
    }
    QueueSfx1_Max6(0x37u);
LABEL_28:
    v3 = (uint16)(4 * selected_save_slot) >> 1;
    enemy_projectile_id[10] = kMenuSelectionMissileXY[v3];
    enemy_projectile_id[5] = kMenuSelectionMissileXY[v3 + 1];
    return;
  }
  v0 = selected_save_slot;
  if (sign16(selected_save_slot - 3)) {
    QueueSfx1_Max6(0x2Au);
    menu_index += 27;
    *(uint16 *)((char *)&enemy_projectile_enable_flag + (uint16)(2 * (selected_save_slot + 2))) = 1;
    *(uint16 *)&g_sram[0x1FEC] = selected_save_slot;
    *(uint16 *)&g_sram[0x1FEE] = ~selected_save_slot;
    RtlWriteSram();
    if (LoadFromSram(selected_save_slot)) {
      NewSaveFile();
      has_area_map = 0;
    } else {
      LoadMirrorOfExploredMapTiles();
    }
    goto LABEL_28;
  }
  if (selected_save_slot == 3) {
    QueueSfx1_Max6(0x37u);
    ++menu_index;
    *(uint16 *)&reg_MOSAIC = *(uint16 *)&reg_MOSAIC & 0xFF0C | 3;
  } else {
    if (selected_save_slot == 4) {
      QueueSfx1_Max6(0x37u);
      menu_index += 15;
      v0 = *(uint16 *)&reg_MOSAIC & 0xFF0C | 3;
      *(uint16 *)&reg_MOSAIC = v0;
    }
    if (v0 == 5)
      menu_index = 33;
  }
}

void FileSelectMap_0(void) {  // 0x81A32A
  VramWriteEntry *v1;

  ClearMenuTilemap();
  uint16 v0 = vram_write_queue_tail;
  v1 = gVramWriteEntry(vram_write_queue_tail);
  v1->size = 2048;
  v1->src.addr = ADDR16_OF_RAM(ram3000.menu.menu_tilemap);
  *(uint16 *)&v1->src.bank = 126;
  v1->vram_dst = (reg_BG2SC & 0xFC) << 8;
  vram_write_queue_tail = v0 + 7;
  palette_change_num = 0;
  LoadFileSelectPalettes();
  uint16 v2 = 0;
  do {
    target_palettes[v2 >> 1] = palette_buffer[v2 >> 1];
    v2 += 2;
  } while ((int16)(v2 - 64) < 0);
  target_palettes[14] = 0;
  target_palettes[30] = 0;
  ++menu_index;
}

void FileSelectMap_1(void) {  // 0x81A37C
  int16 v1;
  int16 v2;


  if (AdvanceGradualColorChangeOfPalette(0, 0x40)) {
    reg_BG1VOFS = 0;
    reg_BG1HOFS = 0;
    reg_BG2VOFS = 0;
    reg_BG2HOFS = 0;
    uint16 v0 = 0;
    while (area_index != kFileSelectMap_AreaIndexes[v0 >> 1]) {
      v0 += 2;
      if ((int16)(v0 - 12) >= 0) {
        v0 = 0;
        break;
      }
    }
    file_select_map_area_index = v0 >> 1;
    v1 = 0;
    do {
      v2 = v1;
      if (v1 == area_index)
        LoadActiveAreaMapForegroundColors(v1);
      else
        LoadInactiveAreaMapForegroundColors(v1);
      ++v1;
    } while ((int16)(v2 - 5) < 0);
    ++menu_index;
    g_word_7E0787 = 0;
    reg_TM = 2;
  }
}

void sub_81A3D1(uint16 k) {  // 0x81A3D1
  LoadActiveAreaMapForegroundColors(k);
}

static const uint16 kActiveAreaMapForegroundColors[6] = { 0, 0xa, 0x10, 0x16, 0x24, 0x2a };
static const uint16 kInactiveAreaMapForegroundColors[6] = {  // 0x81A3D3
  0x30, 0x3a,
  0x40, 0x46,
  0x54, 0x5a,
};

void LoadActiveAreaMapForegroundColors(uint16 v0) {
  LoadAreaMapForegroundColors(kActiveAreaMapForegroundColors[v0]);
}

void LoadInactiveAreaMapForegroundColors(uint16 v0) {  // 0x81A3DC
  LoadAreaMapForegroundColors(kInactiveAreaMapForegroundColors[v0]);
}

void LoadAreaMapForegroundColors(uint16 j) {  // 0x81A3E3
  while (1) {
    int v1 = j >> 1;
    if (kAreaMapForegroundSetDefs[v1] == 0xFFFF)
      break;
    uint16 v4 = j;
    uint16 v2 = kAreaMapForegroundSetDefs[v1 + 1];
    uint16 v3 = kAreaMapForegroundSetDefs[v1];
    R18_ = 5;
    do {
      palette_buffer[v2 >> 1] = kAreaMapForegroundColors[v3 >> 1];
      v2 += 2;
      v3 += 2;
      --R18_;
    } while (R18_);
    j = v4 + 4;
  }
}

void FileSelectMap_2_LoadAreaSelectForegroundTilemap(void) {  // 0x81A546
  VramWriteEntry *v1;

  uint16 v0 = vram_write_queue_tail;
  v1 = gVramWriteEntry(vram_write_queue_tail);
  v1->size = 2048;
  v1->src.addr = addr_kAreaSelectForegroundTilemap;
  *(uint16 *)&v1->src.bank = 129;
  v1->vram_dst = (reg_BG1SC & 0xFC) << 8;
  vram_write_queue_tail = v0 + 7;
  *(uint16 *)&reg_INIDISP = (reg_OBSEL << 8) | 0xF;
  ++menu_index;
}

void FileSelectMap_18(void) {  // 0x81A578
  ++menu_index;
  LoadAreaSelectBackgroundTilemap(area_index);
}

void FileSelectMap_3_LoadAreaSelectBackgroundTilemap(void) {  // 0x81A582
  ++menu_index;
  LoadAreaSelectBackgroundTilemap(area_index);
}

void LoadAreaSelectBackgroundTilemap(uint16 j) {  // 0x81A58A
  VramWriteEntry *v2;
  int16 v3;

  uint16 v1 = vram_write_queue_tail;
  v2 = gVramWriteEntry(vram_write_queue_tail);
  v2->size = 2048;
  LOBYTE(v3) = HIBYTE(j);
  HIBYTE(v3) = j;
  v2->src.addr = 8 * v3 - 16614;
  *(uint16 *)&v2->src.bank = 129;
  v2->vram_dst = (reg_BG3SC & 0xFC) << 8;
  vram_write_queue_tail = v1 + 7;
}

void FileSelectMap_4_SetupExpandingSquareTransition(void) {  // 0x81A5B3
  reg_TS = 4;
  WriteReg(TS, 4u);
  reg_TSW = 4;
  WriteReg(TSW, 4u);
  reg_CGWSEL = 2;
  WriteReg(CGWSEL, 2u);
  reg_CGADSUB = 37;
  WriteReg(CGADSUB, 0x25);
  reg_COLDATA[2] = 0x80;
  reg_COLDATA[1] = 64;
  reg_COLDATA[0] = 32;
  SetupInitialExpandingSquareHDMA();
  reg_HDMAEN = 12;
  WriteReg(HDMAEN, 0xC);
  ConfigureWindow1ForExpandingSquare();
  hdma_objects_enable_flag = 0;
  LOBYTE(menu_index) = menu_index + 1;
}

void ConfigureWindow1ForExpandingSquare(void) {  // 0x81A5F6
  reg_TM = 19;
  WriteReg(TM, 0x13);
  reg_TMW = 19;
  WriteReg(TMW, 0x13);
  reg_W12SEL = 35;
  WriteReg(W12SEL, 0x23);
  reg_W34SEL = 3;
  WriteReg(W34SEL, 3u);
  reg_WOBJSEL = 35;
  WriteReg(WOBJSEL, 0x23);
}

void SetupInitialExpandingSquareHDMA(void) {  // 0x81A61C
  expanding_square_topbottom_margin_right_pos = 0;
  expanding_square_topbottom_margin_left_pos = -1;
  LOBYTE(expanding_square_left_pos) = 127;
  LOBYTE(expanding_square_right_pos) = -127;
  hdma_window_1_left_pos[0].field_0 = 111;
  hdma_window_1_left_pos[0].field_1 = 34;
  hdma_window_1_left_pos[0].field_2 = -98;
  hdma_window_1_left_pos[1].field_0 = 1;
  hdma_window_1_left_pos[1].field_1 = 50;
  hdma_window_1_left_pos[1].field_2 = -98;
  hdma_window_1_left_pos[2].field_0 = 1;
  hdma_window_1_left_pos[2].field_1 = 50;
  hdma_window_1_left_pos[2].field_2 = -98;
  hdma_window_1_left_pos[3].field_0 = 111;
  hdma_window_1_left_pos[3].field_1 = 32;
  hdma_window_1_left_pos[3].field_2 = -98;
  WriteReg(DMAP2, 0x40);
  WriteReg(BBAD2, 0x26);
  WriteReg(A1T2L, 0);
  WriteReg(A1T2H, 0x9E);
  WriteReg(A1B2, 0x7E);
  WriteReg(DAS20, 0x7E);
  WriteReg(DAS2L, 0);
  WriteReg(DAS2H, 0);
  WriteReg(A2A2L, 0);
  WriteReg(A2A2H, 0);
  WriteReg(NTRL2, 0);
  hdma_window_1_right_pos[0].field_0 = 111;
  hdma_window_1_right_pos[0].field_1 = 32;
  hdma_window_1_right_pos[0].field_2 = -98;
  hdma_window_1_right_pos[1].field_0 = 1;
  hdma_window_1_right_pos[1].field_1 = 54;
  hdma_window_1_right_pos[1].field_2 = -98;
  hdma_window_1_right_pos[2].field_0 = 1;
  hdma_window_1_right_pos[2].field_1 = 54;
  hdma_window_1_right_pos[2].field_2 = -98;
  hdma_window_1_right_pos[3].field_0 = 111;
  hdma_window_1_right_pos[3].field_1 = 32;
  hdma_window_1_right_pos[3].field_2 = -98;
  WriteReg(DMAP3, 0x40);
  WriteReg(BBAD3, 0x27);
  WriteReg(A1T3L, 0x10);
  WriteReg(A1T3H, 0x9E);
  WriteReg(A1B3, 0x7E);
  WriteReg(DAS30, 0x7E);
  WriteReg(DAS3L, 0);
  WriteReg(DAS3H, 0);
  WriteReg(A2A3L, 0);
  WriteReg(A2A3H, 0);
  WriteReg(NTRL3, 0);
}
static const uint8 kExpandingSquareTransitionSpeed = 4;


void FileSelectMap_5_ExpandingSquare(void) {  // 0x81A725
  VramWriteEntry *v5;

  DrawAreaSelectMapLabels();
  uint8 v0 = hdma_window_1_left_pos[0].field_0 - kExpandingSquareTransitionSpeed;
  if ((int8)(hdma_window_1_left_pos[0].field_0 - kExpandingSquareTransitionSpeed) < 0) {
    LOBYTE(menu_index) = menu_index + 1;
    reg_TM &= ~2u;
    reg_TMW = 0;
    reg_TSW = 0;
    reg_BG2VOFS = 24;
    pausemenu_palette_animation_timer = 1;
    uint16 v1 = 0;
    do {
      ram4000.xray_tilemaps[v1] = kBg2RoomSelectMapTilemap[v1];
      ++v1;
    } while ((int16)(v1 * 2 - 1600) < 0);
    do
      ram4000.xray_tilemaps[v1++] = 10241;
    while ((int16)(v1 * 2 - 2048) < 0);

    R0_.addr = ADDR16_OF_RAM(ram4000) + 340;
    *(uint16 *)&R0_.bank = 126;
    DrawRoomSelectMapAreaLabel();
    uint16 v2 = 320;
    uint16 v3 = 959;
    do {
      ram4000.xray_tilemaps[v3--] = kFileSelectExpandingSquareTilemap[v2 >> 1];
      v2 -= 2;
    } while (v2);
    uint16 v4 = vram_write_queue_tail;
    v5 = gVramWriteEntry(vram_write_queue_tail);
    v5->size = 2048;
    v5->src.addr = ADDR16_OF_RAM(ram4000);
    *(uint16 *)&v5->src.bank = 126;
    v5->vram_dst = (reg_BG2SC & 0xFC) << 8;
    vram_write_queue_tail = v4 + 7;
  } else {
    hdma_window_1_left_pos[0].field_0 -= kExpandingSquareTransitionSpeed;
    hdma_window_1_left_pos[3].field_0 = v0;
    hdma_window_1_right_pos[0].field_0 = v0;
    hdma_window_1_right_pos[3].field_0 = v0;
    hdma_window_1_left_pos[1].field_0 += kExpandingSquareTransitionSpeed;
    hdma_window_1_left_pos[2].field_0 = hdma_window_1_left_pos[1].field_0;
    hdma_window_1_right_pos[1].field_0 = hdma_window_1_left_pos[1].field_0;
    hdma_window_1_right_pos[2].field_0 = hdma_window_1_left_pos[1].field_0;
    LOBYTE(expanding_square_left_pos) = expanding_square_left_pos - kExpandingSquareTransitionSpeed;
    LOBYTE(expanding_square_right_pos) = kExpandingSquareTransitionSpeed + expanding_square_right_pos;
  }
}

void FileSelectMap_6_AreaSelectMap(void) {  // 0x81A800
  int16 v0;
  uint16 v2;
  uint16 v3;

  v0 = joypad1_newkeys;
  if ((joypad1_newkeys & (kButton_Up | kButton_Left)) != 0) {
    v0 = enable_debug;
    if (enable_debug) {
      R24_ = R22_;
      uint16 v1 = DecAndWraparoundTo5(file_select_map_area_index);
      if (CheckIfFileSelectMapAreaCanBeSelected(v1)
          || (v2 = DecAndWraparoundTo5(R28_), CheckIfFileSelectMapAreaCanBeSelected(v2))
          || (v3 = DecAndWraparoundTo5(R28_), CheckIfFileSelectMapAreaCanBeSelected(v3))) {
        QueueSfx1_Max6(0x37u);
        SwitchActiveFileSelectMapArea();
        DrawAreaSelectMapLabels();
        return;
      }
      goto LABEL_18;
    }
LABEL_6:
    if (v0 < 0) {
      menu_index = 22;
      DrawAreaSelectMapLabels();
      return;
    }
    if ((v0 & (kButton_Start | kButton_A)) != 0) {
      QueueSfx1_Max6(0x38u);
      SelectFileSelectMapArea();
      return;
    }
LABEL_18:
    DrawAreaSelectMapLabels();
    return;
  }
  if ((joypad1_newkeys & (kButton_Select | kButton_Down | kButton_Right)) == 0)
    goto LABEL_6;
  v0 = enable_debug;
  if (!enable_debug)
    goto LABEL_6;
  R22_ = 6;
  R28_ = file_select_map_area_index;
  while (1) {
    uint16 v4 = WraparoundFrom6to0(R28_);
    if (CheckIfFileSelectMapAreaCanBeSelected(v4))
      break;
    if (!--R22_)
      goto LABEL_18;
  }
  SwitchActiveFileSelectMapArea();
  QueueSfx1_Max6(0x37u);
  DrawAreaSelectMapLabels();
}

uint16 DecAndWraparoundTo5(uint16 a) {  // 0x81A898
  uint16 result = a - 1;
  if ((result & 0x8000u) != 0)
    return 5;
  return result;
}

uint16 WraparoundFrom6to0(uint16 a) {  // 0x81A89F
  uint16 result = a + 1;
  if (!sign16(result - 6))
    return 0;
  return result;
}

void SelectFileSelectMapArea(void) {  // 0x81A8A9
  VoidP *v1;
  char v2; // cf
  int16 v3;
  int16 v4;

  ++menu_index;
  if (!enable_debug) {
    area_index = sram_area_index;
    load_station_index = sram_save_station_index;
    DrawAreaSelectMapLabels();
    return;
  }
  area_index = kFileSelectMap_AreaIndexes[file_select_map_area_index];
  R18_ = *(uint16 *)&used_save_stations_and_elevators[(uint16)(2 * area_index)];
  uint16 v0 = 0;
  *(uint16 *)&R0_.bank = 130;
  R0_.addr = addr_kMapIconDataPointers + 64;
  v1 = (VoidP *)IndirPtr(&R0_, 2 * area_index);
  while (!*v1)
    ;
  R0_.addr = *v1;
  R20_ = 16;
  while (1) {
    v2 = R18_ & 1;
    R18_ >>= 1;
    if (!v2)
      goto LABEL_10;
    v3 = *(uint16 *)IndirPtr(&R0_, 4 * v0);
    if (v3 == -2)
      goto LABEL_10;
    if (v3 != -1)
      break;
    v0 = -1;
LABEL_10:
    ++v0;
    if (!--R20_) {
      while (1) {
        v4 = *(uint16 *)IndirPtr(&R0_, 4 * v0);
        if (v4 != -2) {
          if (v4 != -1)
            goto LABEL_16;
          v0 = -1;
        }
        ++v0;
        if (!--R20_) {
          while (1)
            ;
        }
      }
    }
  }
LABEL_16:
  load_station_index = v0;
  DrawAreaSelectMapLabels();
}

uint16 CheckIfFileSelectMapAreaCanBeSelected(uint16 a) {  // 0x81A931
  R28_ = a;
  int v1 = 2 * kFileSelectMap_AreaIndexes[a];
  if (*(uint16 *)&used_save_stations_and_elevators[v1])
    return true;
  uint16 t = *(uint16 *)((char *)&kMapIconDataPointers[4].crateria + v1) + 64;
  return t != 0xffff;
}

void SwitchActiveFileSelectMapArea(void) {  // 0x81A958
  LoadInactiveAreaMapForegroundColors(kFileSelectMap_AreaIndexes[file_select_map_area_index]);
  file_select_map_area_index = R28_;
  LoadActiveAreaMapForegroundColors(kFileSelectMap_AreaIndexes[R28_]);
  LoadAreaSelectBackgroundTilemap(kFileSelectMap_AreaIndexes[file_select_map_area_index]);
}
static const uint16 kAreaSelectMapLabelPositions[12] = {  // 0x81A97E
  0x5b, 0x32,
  0x2a, 0x7f,
  0x5e, 0xb5,
  0xce, 0x50,
  0xce, 0x9f,
  0x87, 0x8b,
};
void DrawAreaSelectMapLabels(void) {
  R3_.addr = 0;
  DrawMenuSpritemap(g_word_82C749[0], 0x80, 0x10);
  for(int i = 0; i < 6; i++) {
    R3_.addr = (i == file_select_map_area_index) ? 0 : 512;
    uint16 v1 = 2 * kFileSelectMap_AreaIndexes[i];
    R36 = *(uint16 *)&used_save_stations_and_elevators[v1];
    const uint16 *v2 = (const uint16 *)RomPtr_82(*(VoidP *)((char *)&kMapIconDataPointers[4].crateria + v1));
    g_word_7E001E = 16;
    while (*v2 != 0xffff) {
      int v4 = R36 & 1;
      R36 >>= 1;
      if (v4 && *v2 != 0xfffe)
        goto LABEL_11;
      v2 += 2;
      if (!--g_word_7E001E) {
        if (enable_debug && *v2 != 0xFFFF) {
LABEL_11:;
          int j = 4 * kFileSelectMap_AreaIndexes[i] >> 1;
          DrawMenuSpritemap(
            g_word_82C749[0] + kFileSelectMap_AreaIndexes[i] + 1,
            kAreaSelectMapLabelPositions[j],
            kAreaSelectMapLabelPositions[j + 1]);
          break;
        }
        break;
      }
    }
  }
}


static const uint16 kRoomSelectMapExpandingSquareTimers[6] = { 0x33, 0x35, 0x2d, 0x33, 0x33, 0x22 };

void FileSelectMap_7_PrepExpandSquareTransToRoomMap(void) {  // 0x81AAAC
  VramWriteEntry *v1;

  DrawAreaSelectMapLabels();
  SetupInitialExpandingSquareHDMA();
  reg_TM = 19;
  reg_TMW = 19;
  reg_W12SEL = 50;
  WriteReg(W12SEL, 0x32);
  reg_W34SEL = 2;
  WriteReg(W34SEL, 2u);
  reg_CGADSUB = 5;
  WriteReg(CGADSUB, 5u);
  reg_WOBJSEL = 34;
  WriteReg(WOBJSEL, 0x22);
  reg_BG12NBA = 48;
  hdma_window_1_left_pos[3].field_0 = 0;
  hdma_window_1_right_pos[3].field_0 = 0;
  R0_.addr = ADDR16_OF_RAM(ram4000) + 340;
  *(uint16 *)&R0_.bank = 126;
  DrawRoomSelectMapAreaLabel();
  uint16 v0 = vram_write_queue_tail;
  v1 = gVramWriteEntry(vram_write_queue_tail);
  v1->size = 512;
  v1->src.addr = ADDR16_OF_RAM(ram4000);
  *(uint16 *)&v1->src.bank = 126;
  v1->vram_dst = (reg_BG2SC & 0xFC) << 8;
  vram_write_queue_tail = v0 + 7;
  expanding_square_timer = kRoomSelectMapExpandingSquareTimers[area_index];
  int v2 = (uint16)(4 * area_index) >> 1;
  expanding_square_left_pos = kAreaSelectMapLabelPositions[v2];
  expanding_square_right_pos = expanding_square_left_pos;
  expanding_square_top_pos = kAreaSelectMapLabelPositions[v2 + 1];
  expanding_square_bottom_pos = expanding_square_top_pos;
  expanding_square_left_subpos = 0;
  expanding_square_right_subpos = 0;
  expanding_square_top_subpos = 0;
  expanding_square_bottom_subpos = 0;
  expanding_square_left_subvel = *(uint16 *)((char *)&kExpandingSquareVels[0].left_subvel
                                             + (uint16)(16 * area_index));
  expanding_square_left_vel = *(uint16 *)((char *)&kExpandingSquareVels[0].left_vel + (uint16)(16 * area_index));
  expanding_square_right_subvel = *(uint16 *)((char *)&kExpandingSquareVels[0].right_subvel
                                              + (uint16)(16 * area_index));
  expanding_square_right_vel = *(uint16 *)((char *)&kExpandingSquareVels[0].right_vel
                                           + (uint16)(16 * area_index));
  expanding_square_top_subvel = *(uint16 *)((char *)&kExpandingSquareVels[0].top_subvel
                                            + (uint16)(16 * area_index));
  expanding_square_top_vel = *(uint16 *)((char *)&kExpandingSquareVels[0].top_vel + (uint16)(16 * area_index));
  expanding_square_bottom_subvel = *(uint16 *)((char *)&kExpandingSquareVels[0].bottom_subvel
                                               + (uint16)(16 * area_index));
  expanding_square_bottom_vel = *(uint16 *)((char *)&kExpandingSquareVels[0].bottom_vel
                                            + (uint16)(16 * area_index));
  SetupRoomSelectMapExpandingSquareTransHDMA();
  reg_HDMAEN = 12;
  WriteReg(HDMAEN, 0xC);
  ++menu_index;
  QueueSfx1_Max6(0x3Bu);
}

void SetupRoomSelectMapExpandingSquareTransHDMA(void) {  // 0x81ABA7
  uint16 k = 0;
  uint16 v0 = expanding_square_top_pos;
  AddExpandingSqTransLeftIndirHDMA(v0, k, 0x9E22);
  k = AddExpandingSqTransRightIndirHDMA(v0, k, 0x9E20);
  uint16 v2 = expanding_square_bottom_pos - expanding_square_top_pos;
  if ((uint8)expanding_square_bottom_pos == (uint8)expanding_square_top_pos)
    v2 = 1;
  AddExpandingSqTransLeftIndirHDMA(v2, k, 0x9E32);
  k = AddExpandingSqTransRightIndirHDMA(v2, k, 0x9E36);
  uint16 v4 = -32 - expanding_square_bottom_pos;
  if ((uint8)expanding_square_bottom_pos == 0xE0)
    v4 = 1;
  AddExpandingSqTransLeftIndirHDMA(v4, k, 0x9E22);
  k = AddExpandingSqTransRightIndirHDMA(v4, k, 0x9E20);
  *(&hdma_window_1_left_pos[0].field_0 + k) = 0;
  *(&hdma_window_1_right_pos[0].field_0 + k) = 0;
}

void AddExpandingSqTransLeftIndirHDMA(uint16 a, uint16 k, uint16 j) {  // 0x81ABF7
  if ((a & 0x80) != 0) {
    *(&hdma_window_1_left_pos[0].field_0 + k) = a - 127;
    *(&hdma_window_1_left_pos[1].field_0 + k) = 127;
    *(uint16 *)(&hdma_window_1_left_pos[0].field_1 + k) = j;
    *(uint16 *)(&hdma_window_1_left_pos[1].field_1 + k) = j;
  } else {
    *(&hdma_window_1_left_pos[0].field_0 + k) = a;
    *(uint16 *)(&hdma_window_1_left_pos[0].field_1 + k) = j;
  }
}

uint16 AddExpandingSqTransRightIndirHDMA(uint16 a, uint16 k, uint16 j) {  // 0x81AC2D
  if ((a & 0x80) != 0) {
    *(&hdma_window_1_right_pos[0].field_0 + k) = a - 127;
    *(&hdma_window_1_right_pos[1].field_0 + k) = 127;
    *(uint16 *)(&hdma_window_1_right_pos[0].field_1 + k) = j;
    *(uint16 *)(&hdma_window_1_right_pos[1].field_1 + k) = j;
    return k + 6;
  } else {
    *(&hdma_window_1_right_pos[0].field_0 + k) = a;
    *(uint16 *)(&hdma_window_1_right_pos[0].field_1 + k) = j;
    return k + 3;
  }
}

void FileSelectMap_8_ExpandSquareTransToRoomSelectMap(void) {  // 0x81AC66
  if ((HandleRoomSelectMapExpandingSquareTrans() & 0x8000u) != 0) {
    ++menu_index;
    reg_TM = 2;
    reg_TMW = 0;
    reg_TSW = 0;
    reg_TS = 0;
  }
  DrawAreaSelectMapLabels();
}

uint16 HandleRoomSelectMapExpandingSquareTrans(void) {  // 0x81AC84
  uint16 v0 = (__PAIR32__(expanding_square_left_vel, expanding_square_left_subvel)
               + __PAIR32__(expanding_square_left_pos, expanding_square_left_subpos)) >> 16;
  expanding_square_left_subpos += expanding_square_left_subvel;
  if (sign16(v0 - 1))
    v0 = 1;
  expanding_square_left_pos = v0;
  uint16 v1 = (__PAIR32__(expanding_square_right_vel, expanding_square_right_subvel)
               + __PAIR32__(expanding_square_right_pos, expanding_square_right_subpos)) >> 16;
  expanding_square_right_subpos += expanding_square_right_subvel;
  if (!sign16(v1 - 256))
    v1 = 255;
  expanding_square_right_pos = v1;
  uint16 v2 = (__PAIR32__(expanding_square_top_vel, expanding_square_top_subvel)
               + __PAIR32__(expanding_square_top_pos, expanding_square_top_subpos)) >> 16;
  expanding_square_top_subpos += expanding_square_top_subvel;
  if (sign16(v2 - 1))
    v2 = 1;
  expanding_square_top_pos = v2;
  uint16 v3 = (__PAIR32__(expanding_square_bottom_vel, expanding_square_bottom_subvel)
               + __PAIR32__(expanding_square_bottom_pos, expanding_square_bottom_subpos)) >> 16;
  expanding_square_bottom_subpos += expanding_square_bottom_subvel;
  if (!sign16(v3 - 224))
    v3 = 224;
  expanding_square_bottom_pos = v3;
  SetupRoomSelectMapExpandingSquareTransHDMA();
  return --expanding_square_timer;
}

void FileSelectMap_9_InitRoomSelectMap(void) {  // 0x81AD17
  LoadMirrorOfExploredMapTiles();
  DrawRoomSelectMap();
  LoadFromLoadStation();
  DisableHdmaObjects();
  WaitUntilEndOfVblankAndClearHdma();
  RoomDefHeader *RoomDefHeader = get_RoomDefHeader(room_ptr);
  LOBYTE(area_index) = RoomDefHeader->area_index_;
  LOBYTE(room_x_coordinate_on_map) = RoomDefHeader->x_coordinate_on_map;
  LOBYTE(room_y_coordinate_on_map) = RoomDefHeader->y_coordinate_on_map;
  SetupMapScrollingForFileSelectMap();
  map_min_y_scroll += 24;
  reg_BG2VOFS = 24;
  *(uint16 *)&reg_TM &= ~4u;
  ++menu_index;
  map_scrolling_direction = 0;
  map_scrolling_speed_index = 0;
  samus_position_indicator_animation_frame = 0;
  samus_position_indicator_animation_timer = 0;
  samus_position_indicator_animation_loop_counter = 0;
}

void FileSelectMap_10_RoomSelectMap(void) {  // 0x81AD7F
  int16 v1;
  VoidP *v2;
  int16 v3;
  char v4; // cf
  int16 v5;
  int16 v8;

  DrawFileSelectMapIcons();
  HandleFileSelectMapScrollArrows();
  MapScrolling();
  DisplayMapElevatorDestinations();
  if (enable_debug && (joypad2_new_keys & kButton_Select) != 0) {
    QueueSfx1_Max6(0x38u);
    R0_.bank = -126;
    R24_ = *(uint16 *)&used_save_stations_and_elevators[(uint16)(2 * area_index)];
    v1 = load_station_index;
    do {
      R24_ >>= 1;
      --v1;
    } while (v1 >= 0);
    v2 = (VoidP *)RomPtr_82(2 * area_index - 14325);
    while (!*v2)
      ;
    R0_.addr = *v2;
    v3 = 4 * load_station_index;
    R18_ = *(uint16 *)IndirPtr(&R0_, 4 * load_station_index);
    R20_ = *(uint16 *)IndirPtr(&R0_, v3 + 2);
    if (!sign16(load_station_index - 16))
      goto LABEL_23;
    do {
      if (!sign16(++load_station_index - 16)) {
        while (1) {
          v5 = *(uint16 *)IndirPtr(&R0_, 4 * load_station_index);
          if (v5 == -1)
            break;
          if (v5 != -2)
            goto LABEL_25;
LABEL_23:
          ++load_station_index;
        }
        load_station_index = 0;
        R24_ = *(uint16 *)&used_save_stations_and_elevators[(uint16)(2 * area_index)];
      }
      v4 = R24_ & 1;
      R24_ >>= 1;
    } while (!v4 || *(uint16 *)IndirPtr(&R0_, 4 * load_station_index) >= 0xFFFEu);
LABEL_25:;
    uint16 v6 = 4 * load_station_index;
    uint8 *v7 = IndirPtr(&R0_, 4 * load_station_index);
    if (sign16(GET_WORD(v7) - reg_BG1HOFS) || !sign16(GET_WORD(v7) - 256 - reg_BG1HOFS)) {
      v8 = reg_BG1HOFS + *(uint16 *)IndirPtr(&R0_, v6) - R18_;
      if (v8 >= 0) {
        if ((int16)(v8 - map_min_x_scroll) >= 0)
          v8 = map_min_x_scroll;
      } else {
        v8 = 0;
      }
      reg_BG1HOFS = v8;
    }
    uint16 v9 = v6 + 2;
    uint8 *v10 = IndirPtr(&R0_, v9);
    if (sign16(GET_WORD(v10) - reg_BG1VOFS) || !sign16(GET_WORD(v10) - 161 - reg_BG1VOFS)) {
      uint16 v11 = reg_BG1VOFS + *(uint16 *)IndirPtr(&R0_, v9) - R20_;
      if ((int16)(v11 - map_min_y_scroll) >= 0)
        v11 = map_min_y_scroll;
      reg_BG1VOFS = v11;
    }
  } else if ((joypad1_newkeys & 0x8000u) != 0) {
    menu_index += 5;
    uint16 v0 = 0;
    while (area_index != kFileSelectMap_AreaIndexes[v0 >> 1]) {
      v0 += 2;
      if ((int16)(v0 - 12) >= 0) {
        file_select_map_area_index = 0;
        return;
      }
    }
    file_select_map_area_index = v0 >> 1;
    QueueSfx1_Max6(0x3Cu);
  } else if ((joypad1_newkeys & (kButton_Start | kButton_A)) != 0) {
    ++menu_index;
    QueueSfx1_Max6(0x38u);
  }
}

void sub_81AEC8(void) {  // 0x81AEC8
  HandleFileSelectMapScrollArrows();
}

void HandleFileSelectMapScrollArrows(void) {  // 0x81AECA
  if (sign16(map_min_x_scroll - 24 - reg_BG1HOFS))
    DrawMapScrollArrowAndCheckToScroll(0x81, addr_kLeftMapScrollArrowData);
  if (!sign16(map_max_x_scroll - 232 - reg_BG1HOFS))
    DrawMapScrollArrowAndCheckToScroll(0x81, addr_kRightMapScrollArrowData);
  if (sign16(map_min_y_scroll - 64 - reg_BG1VOFS))
    DrawMapScrollArrowAndCheckToScroll(0x81, addr_kUpMapScrollArrowData);
  if (sign16(map_max_y_scroll - 145 - reg_BG1VOFS)) {
    if (map_scrolling_direction == kDownMapScrollArrowData.map_scroll_dir) {
      map_scrolling_gear_switch_timer = 0;
      map_scrolling_direction = 0;
      map_scrolling_speed_index = 0;
    }
  } else {
    DrawMapScrollArrowAndCheckToScroll(0x81, addr_kDownMapScrollArrowData);
  }
}

void FileSelectMap_11(void) {  // 0x81AF5A
  DrawFileSelectMapIcons();
  DisplayMapElevatorDestinations();
  ++menu_index;
}

void FileSelectMap_13(void) {  // 0x81AF66
  DrawFileSelectMapIcons();
  DisplayMapElevatorDestinations();
  HandleFadeOut();
  if ((reg_INIDISP & 0xF) == 0) {
    ++menu_index;
    enemy_data[0].x_pos = 32;
  }
}

void FileSelectMap_14(void) {  // 0x81AF83
  if (!--enemy_data[0].x_pos) {
    ScreenOff();
    ++game_state;
    menu_index = 0;
  }
}

void FileSelectMap_15_ClearTileMap(void) {  // 0x81AF97
  reg_TM = 18;
  for (int i = 2046; i >= 0; i -= 2)
    *(uint16 *)((char *)ram3000.pause_menu_map_tilemap + (uint16)i) = 15;
  uint16 v1 = vram_write_queue_tail;
  VramWriteEntry *v2 = gVramWriteEntry(vram_write_queue_tail);
  v2->size = 2048;
  v2->src.addr = ADDR16_OF_RAM(ram3000.pause_menu_map_tilemap);
  *(uint16 *)&v2->src.bank = 126;
  v2->vram_dst = (reg_BG1SC & 0xFC) << 8;
  vram_write_queue_tail = v1 + 7;
  ++menu_index;
}

void FileSelectMap_16_LoadPalettes(void) {  // 0x81AFD3
  int16 v0;
  int16 v1;

  LoadMenuPalettes();
  v0 = 0;
  do {
    v1 = v0;
    LoadInactiveAreaMapForegroundColors(v0++);
  } while ((int16)(v1 - 5) < 0);
  LoadActiveAreaMapForegroundColors(area_index);
  LoadAreaSelectBackgroundTilemap(area_index);
  ++menu_index;
}

void FileSelectMap_20_SetupExpandingSquare(void) {  // 0x81AFF6
  reg_HDMAEN = 0;
  QueueSfx1_Max6(0x3Cu);
  expanding_square_timer = kRoomSelectMapExpandingSquareTimers[area_index] - 12;
  expanding_square_left_subvel = *(uint16 *)((char *)&kExpandingSquareVels[0].left_subvel
                                             + (uint16)(16 * area_index));
  expanding_square_left_vel = *(uint16 *)((char *)&kExpandingSquareVels[0].left_vel + (uint16)(16 * area_index));
  expanding_square_right_subvel = *(uint16 *)((char *)&kExpandingSquareVels[0].right_subvel
                                              + (uint16)(16 * area_index));
  expanding_square_right_vel = *(uint16 *)((char *)&kExpandingSquareVels[0].right_vel
                                           + (uint16)(16 * area_index));
  expanding_square_top_subvel = *(uint16 *)((char *)&kExpandingSquareVels[0].top_subvel
                                            + (uint16)(16 * area_index));
  expanding_square_top_vel = *(uint16 *)((char *)&kExpandingSquareVels[0].top_vel + (uint16)(16 * area_index));
  expanding_square_bottom_subvel = *(uint16 *)((char *)&kExpandingSquareVels[0].bottom_subvel
                                               + (uint16)(16 * area_index));
  expanding_square_bottom_vel = *(uint16 *)((char *)&kExpandingSquareVels[0].bottom_vel
                                            + (uint16)(16 * area_index));
  expanding_square_left_pos = 8;
  expanding_square_right_pos = 248;
  expanding_square_top_pos = 8;
  expanding_square_bottom_pos = 216;
  expanding_square_left_subpos = 0;
  expanding_square_right_subpos = 0;
  expanding_square_top_subpos = 0;
  expanding_square_bottom_subpos = 0;
  SetupRoomSelectMapExpandingSquareTransHDMA();
  ++menu_index;
  reg_HDMAEN = 12;
  WriteReg(HDMAEN, 0xC);
  reg_W12SEL = 50;
  WriteReg(W12SEL, 0x32);
  reg_W34SEL = 2;
  WriteReg(W34SEL, 2u);
  reg_BG12NBA = 48;
  reg_WOBJSEL = 34;
  hdma_window_1_left_pos[3].field_0 = 0;
  hdma_window_1_right_pos[3].field_0 = 0;
  reg_BG1VOFS = 0;
  reg_BG1HOFS = 0;
}

void FileSelectMap_21_MoveExpandingSquare(void) {  // 0x81B0BB
  uint16 v0 = (__PAIR32__(expanding_square_left_pos, expanding_square_left_subpos)
               - __PAIR32__(expanding_square_left_vel, expanding_square_left_subvel)) >> 16;
  expanding_square_left_subpos -= expanding_square_left_subvel;
  expanding_square_left_pos = v0;
  uint16 v1 = (__PAIR32__(expanding_square_right_pos, expanding_square_right_subpos)
               - __PAIR32__(expanding_square_right_vel, expanding_square_right_subvel)) >> 16;
  expanding_square_right_subpos -= expanding_square_right_subvel;
  expanding_square_right_pos = v1;
  uint16 v2 = (__PAIR32__(expanding_square_top_pos, expanding_square_top_subpos)
               - __PAIR32__(expanding_square_top_vel, expanding_square_top_subvel)) >> 16;
  expanding_square_top_subpos -= expanding_square_top_subvel;
  expanding_square_top_pos = v2;
  uint16 v3 = (__PAIR32__(expanding_square_bottom_pos, expanding_square_bottom_subpos)
               - __PAIR32__(expanding_square_bottom_vel, expanding_square_bottom_subvel)) >> 16;
  expanding_square_bottom_subpos -= expanding_square_bottom_subvel;
  expanding_square_bottom_pos = v3;
  SetupRoomSelectMapExpandingSquareTransHDMA();
  DrawAreaSelectMapLabels();
  if ((--expanding_square_timer & 0x8000u) != 0) {
    menu_index -= 15;
    reg_TM = 17;
    reg_TMW = 0;
    reg_TSW = 0;
  }
}

void NewSaveFile(void) {  // 0x81B2CB
  samus_max_health = 99;
  samus_health = 99;
  samus_max_missiles = 0;
  samus_missiles = 0;
  samus_max_super_missiles = 0;
  samus_super_missiles = 0;
  samus_max_power_bombs = 0;
  samus_power_bombs = 0;
  hud_item_index = 0;
  collected_beams = 0;
  equipped_beams = 0;
  collected_items = 0;
  equipped_items = 0;
  reserve_health_mode = 0;
  samus_max_reserve_health = 0;
  samus_reserve_health = 0;
  samus_reserve_missiles = 0;
  button_config_up = kButton_Up;
  button_config_down = kButton_Down;
  button_config_left = kButton_Left;
  button_config_right = kButton_Right;
  button_config_jump_a = kButton_A;
  button_config_run_b = kButton_B;
  button_config_shoot_x = kButton_X;
  button_config_itemcancel_y = kButton_Y;
  button_config_itemswitch = kButton_Select;
  button_config_aim_up_R = kButton_R;
  button_config_aim_down_L = kButton_L;
  game_time_frames = 0;
  game_time_seconds = 0;
  game_time_minutes = 0;
  game_time_hours = 0;
  japanese_text_flag = 0;
  moonwalk_flag = 0;
  hud_auto_cancel_flag = 0;
  debug_flag = 1;
  UNUSED_word_7E09E8 = 1;
  uint16 v0 = 0;
  do {
    int v1 = v0 >> 1;
    WORD(room_chozo_bits[v0]) = 0;
    *(uint16 *)&item_bit_array[v0] = 0;
    *(uint16 *)&item_bit_array[v0] = 0;
    WORD(opened_door_bit_array[v0]) = 0;
    UNUSED_word_7ED8F0[v1] = 0;
    *(uint16 *)&map_station_byte_array[v0] = 0;
    *(uint16 *)&used_save_stations_and_elevators[v0] = 0;
    *(uint16 *)&used_save_stations_and_elevators[v0 + 8] = 0;
    v0 += 2;
  } while ((int16)(v0 - 8) < 0);
  do {
    int v2 = v0 >> 1;
    WORD(room_chozo_bits[v0]) = 0;
    *(uint16 *)&item_bit_array[v0] = 0;
    *(uint16 *)&item_bit_array[v0] = 0;
    WORD(opened_door_bit_array[v0]) = 0;
    v0 += 2;
  } while ((int16)(v0 - 64) < 0);
  uint16 v3 = 0;
  do {
    explored_map_tiles_saved[v3 >> 1] = 0;
    v3 += 2;
  } while ((int16)(v3 - 1792) < 0);
}

void FileSelectClearRestOfMenuTilemapRow(uint16 v0) {  // 0x81B3C5
  R18_ = 32 - ((uint8)(v0 & 0x3F) >> 1);
  do {
    *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[768] + v0) = 15;
    v0 += 2;
    --R18_;
  } while (R18_);
}

void LoadMenuTilemap(uint16 k, uint16 j) {  // 0x81B3E2
  int16 v2;
  int k_bak = k;

  while (1) {
    while (1) {
      v2 = *(uint16 *)RomPtr_81(j);
      if (v2 != -2)
        break;
      j += 2;
      k = (k_bak += 64);
    }
    if (v2 == -1)
      break;
    *(uint16 *)((char *)&ram3000.pause_menu_map_tilemap[768] + k) = enemy_data[0].palette_index | v2;
    k += 2;
    j += 2;
  }
}