// Block properties, some cutscene graphics
#include "sm_rtl.h"
#include "ida_types.h"
#include "variables.h"
#include "funcs.h"

static const uint8 kAlignPos_Tab1[512] = {
  16, 16, 16, 16, 16, 16, 16, 16,  0,  0,  0,  0,  0,  0,  0,  0,
   8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,
  16, 16, 16, 16, 16, 16, 16, 16,  8,  8,  8,  8,  8,  8,  8,  8,
   8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  16, 15, 14, 13, 12, 11, 10,  9,  9, 10, 11, 12, 13, 14, 15, 16,
  16, 14, 12, 10,  8,  6,  4,  2,  2,  4,  6,  8, 10, 12, 14, 16,
  16, 16, 16, 16, 16, 16, 16, 16,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  12, 12, 12, 12,  8,  8,  8,  8,  4,  4,  4,  4,  0,  0,  0,  0,
  14, 14, 12, 12, 10, 10,  8,  8,  6,  6,  4,  4,  2,  2,  0,  0,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 16, 16, 16,
  16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 15, 14, 13, 12, 11, 10,  9,
   8,  7,  6,  5,  4,  3,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 14, 12, 10,  8,  6,  4,  2,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  4,  6,  8, 10, 12, 14,
  16, 16, 16, 15, 15, 15, 14, 14, 14, 13, 13, 13, 12, 12, 12, 11,
  11, 11, 10, 10, 10,  9,  9,  9,  8,  8,  8,  7,  7,  7,  6,  6,
   6,  5,  5,  5,  4,  4,  4,  3,  3,  3,  2,  2,  2,  1,  1,  1,
  20, 20, 20, 20, 20, 20, 20, 20, 16, 14, 12, 10,  8,  6,  4,  2,
  16, 14, 12, 10,  8,  6,  4,  2,  0,  0,  0,  0,  0,  0,  0,  0,
  20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 15, 12,  9,  6,  3,
  20, 20, 20, 20, 20, 20, 14, 11,  8,  5,  2,  0,  0,  0,  0,  0,
  16, 13, 10,  7,  4,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

static const uint8 kAlignYPos_Tab0[512] = {
   8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,
  16, 16, 16, 16, 16, 16, 16, 16,  0,  0,  0,  0,  0,  0,  0,  0,
  16, 16, 16, 16, 16, 16, 16, 16,  8,  8,  8,  8,  8,  8,  8,  8,
   8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  16, 15, 14, 13, 12, 11, 10,  9,  9, 10, 11, 12, 13, 14, 15, 16,
  16, 14, 12, 10,  8,  6,  4,  2,  2,  4,  6,  8, 10, 12, 14, 16,
   8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  12, 12, 12, 12,  8,  8,  8,  8,  4,  4,  4,  4,  0,  0,  0,  0,
  14, 14, 12, 12, 10, 10,  8,  8,  6,  6,  4,  4,  2,  2,  0,  0,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 16, 16, 16,
  16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 15, 14, 13, 12, 11, 10,  9,
   8,  7,  6,  5,  4,  3,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,
  16, 16, 15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 10,  9,  9,
   8,  8,  7,  7,  6,  6,  5,  5,  4,  4,  3,  3,  2,  2,  1,  1,
  16, 16, 16, 15, 15, 15, 14, 14, 14, 13, 13, 13, 12, 12, 12, 11,
  11, 11, 10, 10, 10,  9,  9,  9,  8,  8,  8,  7,  7,  7,  6,  6,
   6,  5,  5,  5,  4,  4,  4,  3,  3,  3,  2,  2,  2,  1,  1,  1,
  20, 20, 20, 20, 20, 20, 20, 20, 16, 14, 12, 10,  8,  6,  4,  2,
  16, 14, 12, 10,  8,  6,  4,  2,  0,  0,  0,  0,  0,  0,  0,  0,
  20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 15, 12,  9,  6,  3,
  20, 20, 20, 20, 20, 20, 14, 11,  8,  5,  2,  0,  0,  0,  0,  0,
  16, 13, 10,  7,  4,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};


//////////////////////////////////////////////////////////////
// BANK 94
//////////////////////////////////////////////////////////////

static const uint8 kTab948E54[20] = {
     0,    0, 0x80, 0x80, 0, 0x80, 0, 0x80, 0, 0, 0, 0x80, 0, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80,
};

static const uint16 kBlockColl_Horiz_Slope_NonSquare_Tab[64] = {
       0, 0x100,
       0, 0x100,
       0, 0x100,
       0, 0x100,
       0, 0x100,
       0, 0x100,
       0, 0x100,
       0, 0x100,
       0, 0x100,
       0, 0x100,
       0, 0x100,
       0, 0x100,
       0, 0x100,
       0, 0x100,
  0x1000,  0xb0,
  0x1000,  0xb0,
       0, 0x100,
       0, 0x100,
  0x1000,  0xc0,
       0, 0x100,
  0x1000,  0xc0,
  0x1000,  0xc0,
   0x800,  0xd8,
   0x800,  0xd8,
   0x600,  0xf0,
   0x600,  0xf0,
   0x600,  0xf0,
  0x4000,  0x80,
  0x4000,  0x80,
  0x6000,  0x50,
  0x6000,  0x50,
  0x6000,  0x50,
};

#define fnkPlmHeaderDefPtrs 0x949139

#define off_9492D9 ((uint16*)RomPtr(0x9492d9))
#define off_9492E9 ((uint16*)RomPtr(0x9492e9))
#define kPlmHeaderDefPtrs ((uint16*)RomPtr(0x949139))

uint16 PostGrappleColl_Horiz_Slope_NonSquare(uint16 k) {  // 0x948000
  int16 v1;
  char v3;
  int16 v5;
  char v6;
  int16 v8;

  if (!(samus_collision_direction & 1)) {
    uint16 mod = SnesModulus(cur_block_index, room_width_in_blocks);
    v1 = samus_x_pos >> 4;
    if (v1 != mod)
      return -1;
    temp_collision_DD4 = R32 & 0xF;
    temp_collision_DD6 = 16 * (BTS[k] & 0x1F);
    if ((BTS[k] & 0x40) != 0) {
      if (BTS[k] & 0x80)
        v3 = samus_y_pos ^ 0xF;
      else
        v3 = samus_y_pos;
      uint16 v4 = temp_collision_DD6 + (v3 & 0xF);
      uint16 result = (kAlignPos_Tab1[v4] & 0x1F) - temp_collision_DD4 - 1;
      if ((kAlignPos_Tab1[v4] & 0x1F) - temp_collision_DD4 != 1 && (result & 0x8000u) != 0)
        return -1;
      return result;
    }
    return PostGrappleColl_Horiz_Solid();
  }
  uint16 mod = SnesModulus(cur_block_index, room_width_in_blocks);
  v5 = samus_x_pos >> 4;
  if (v5 != mod)
    return -1;
  temp_collision_DD4 = R32 & 0xF;
  temp_collision_DD6 = 16 * (BTS[k] & 0x1F);
  if (BTS[k] & 0x40)
    return PostGrappleColl_Horiz_Solid();
  if (BTS[k] & 0x80)
    v6 = samus_y_pos ^ 0xF;
  else
    v6 = samus_y_pos;
  uint16 v7 = temp_collision_DD6 + (v6 & 0xF);
  v8 = (kAlignPos_Tab1[v7] & 0x1F) - temp_collision_DD4 - 1;
  if ((kAlignPos_Tab1[v7] & 0x1F) - temp_collision_DD4 == 1 || v8 < 0)
    return ~v8;
  else
    return -1;
}

uint16 PostGrappleColl_Vert_Slope_NonSquare(uint16 k) {  // 0x9480E0
  int16 v1;
  int16 v3;
  char v4;
  int16 v6;
  int16 v7;
  char v8;
  int16 v10;

  if (!(samus_collision_direction & 1)) {
    uint16 mod = SnesModulus(cur_block_index, room_width_in_blocks);
    v1 = samus_x_pos >> 4;
    if (v1 != mod)
      return -1;
    temp_collision_DD4 = R32 & 0xF;
    temp_collision_DD6 = 16 * (BTS[k] & 0x1F);
    v3 = BTS[k];
    if (v3 & 0x80) {
      if ((v3 & 0x40) != 0)
        v4 = samus_x_pos ^ 0xF;
      else
        v4 = samus_x_pos;
      uint16 v5 = temp_collision_DD6 + (v4 & 0xF);
      uint16 result = (kAlignYPos_Tab0[v5] & 0x1F) - temp_collision_DD4 - 1;
      if ((kAlignYPos_Tab0[v5] & 0x1F) - temp_collision_DD4 != 1 && (result & 0x8000u) != 0)
        return -1;
      return result;
    }
    return PostGrappleColl_Vert_Solid();
  }
  uint16 mod = SnesModulus(cur_block_index, room_width_in_blocks);
  v6 = samus_x_pos >> 4;
  if (v6 != mod)
    return -1;
  temp_collision_DD4 = R32 & 0xF;
  temp_collision_DD6 = 16 * (BTS[k] & 0x1F);
  v7 = BTS[k];
  if (v7 & 0x80)
    return PostGrappleColl_Vert_Solid();
  if ((v7 & 0x40) != 0)
    v8 = samus_x_pos ^ 0xF;
  else
    v8 = samus_x_pos;
  uint16 v9 = temp_collision_DD6 + (v8 & 0xF);
  v10 = (kAlignYPos_Tab0[v9] & 0x1F) - temp_collision_DD4 - 1;
  if ((kAlignYPos_Tab0[v9] & 0x1F) - temp_collision_DD4 == 1 || v10 < 0)
    return ~v10;
  else
    return -1;
}

uint16 PostGrappleColl_Horiz_Slope_Square(uint16 k) {  // 0x9481B8
  temp_collision_DD4 = 4 * (BTS[k] & 0x1F);
  temp_collision_DD6 = BTS[k] >> 6;
  uint16 v1 = temp_collision_DD4 + (temp_collision_DD6 ^ ((uint8)(R32 & 8) >> 3));
  if (!R26_) {
    if ((((uint8)samus_y_radius + (uint8)samus_y_pos - 1) & 8) == 0) {
      if (!kTab948E54[v1])
        return -1;
      goto LABEL_10;
    }
    goto LABEL_7;
  }
  if (R26_ != R28_ || (((uint8)samus_y_pos - (uint8)samus_y_radius) & 8) == 0) {
LABEL_7:
    if (kTab948E54[v1])
      goto LABEL_10;
  }
  if (!kTab948E54[v1 ^ 2])
    return -1;
LABEL_10:
  if (samus_collision_direction & 1)
    return R32 & 7;
  else
    return R32 & 7 ^ 7;
}

uint16 PostGrappleColl_Vertical_Slope_Square(uint16 k) {  // 0x948230
  temp_collision_DD4 = 4 * (BTS[k] & 0x1F);
  temp_collision_DD6 = BTS[k] >> 6;
  uint16 v1 = temp_collision_DD4 + (temp_collision_DD6 ^ ((uint8)(R32 & 8) >> 2));
  if (!R26_) {
    if ((((uint8)samus_x_radius + (uint8)samus_x_pos - 1) & 8) == 0) {
      if (!kTab948E54[v1])
        return -1;
      goto LABEL_10;
    }
    goto LABEL_7;
  }
  if (R26_ != R28_ || (((uint8)samus_x_pos - (uint8)samus_x_radius) & 8) == 0) {
LABEL_7:
    if (kTab948E54[v1])
      goto LABEL_10;
  }
  if (!kTab948E54[v1 ^ 1])
    return -1;
LABEL_10:
  if (samus_collision_direction & 1)
    return R32 & 7;
  else
    return R32 & 7 ^ 7;
}

uint16 ClearCarry_0(void) {  // 0x9482A7
  return -1;
}

uint16 PostGrappleColl_Horiz_Slope(void) {  // 0x9482A9
  if ((BTS[cur_block_index] & 0x1Fu) < 5)
    return PostGrappleColl_Horiz_Slope_Square(cur_block_index);
  else
    return PostGrappleColl_Horiz_Slope_NonSquare(cur_block_index);
}

uint16 PostGrappleColl_Horiz_Solid(void) {  // 0x9482BE
  return R32 & 0xF;
}

uint16 PostGrappleColl_Vert_Slope(void) {  // 0x9482C5
  if ((BTS[cur_block_index] & 0x1Fu) < 5)
    return PostGrappleColl_Vertical_Slope_Square(cur_block_index);
  else
    return PostGrappleColl_Vert_Slope_NonSquare(cur_block_index);
}

uint16 PostGrappleColl_Vert_Solid(void) {  // 0x9482DA
  return R32 & 0xF;
}

static Func_U16 *const kPostGrappleColl_Horiz[16] = {  // 0x948321
  ClearCarry_0,
  PostGrappleColl_Horiz_Slope,
  ClearCarry_0,
  ClearCarry_0,
  ClearCarry_0,
  ClearCarry_0,
  ClearCarry_0,
  ClearCarry_0,
  PostGrappleColl_Horiz_Solid,
  PostGrappleColl_Horiz_Solid,
  PostGrappleColl_Horiz_Solid,
  PostGrappleColl_Horiz_Solid,
  PostGrappleColl_Horiz_Solid,
  PostGrappleColl_Horiz_Solid,
  PostGrappleColl_Horiz_Solid,
  PostGrappleColl_Horiz_Solid,
};
static Func_U16 *const kPostGrappleColl_Vert[16] = {
  ClearCarry_0,
  PostGrappleColl_Vert_Slope,
  ClearCarry_0,
  ClearCarry_0,
  ClearCarry_0,
  ClearCarry_0,
  ClearCarry_0,
  ClearCarry_0,
  PostGrappleColl_Vert_Solid,
  PostGrappleColl_Vert_Solid,
  PostGrappleColl_Vert_Solid,
  PostGrappleColl_Vert_Solid,
  PostGrappleColl_Vert_Solid,
  PostGrappleColl_Vert_Solid,
  PostGrappleColl_Vert_Solid,
  PostGrappleColl_Vert_Solid,
};
uint16 PostGrappleColl_Horiz(uint16 k) {
  cur_block_index = k >> 1;
  return kPostGrappleColl_Horiz[(uint16)((uint8)((uint16)(level_data[k >> 1] & 0xF000) >> 8) >> 3) >> 1]();
}

uint16 PostGrappleColl_Vert(uint16 k) {  // 0x948338
  cur_block_index = k >> 1;
  return kPostGrappleColl_Vert[(uint16)((uint8)((uint16)(level_data[k >> 1] & 0xF000) >> 8) >> 3) >> 1]();
}

void PostGrappleCollisionDetect_Right(void) {  // 0x94834F
  int16 v0;
  int16 v2;

  samus_collision_direction = 1;
  distance_to_eject_samus_left = 0;
  Samus_GetYposSpan();
  uint16 prod = Mult8x8((uint16)(samus_y_pos - samus_y_radius) >> 4, room_width_in_blocks);
  R22_ = samus_x_subpos;
  R24_ = samus_x_pos;
  R32 = samus_x_radius + samus_x_pos - 1;
  v0 = R32 >> 4;
  cur_block_index = prod + v0;
  uint16 v1 = 2 * cur_block_index;
  do {
    v2 = PostGrappleColl_Horiz(v1);
    if (v2 >= 0) {
      uint16 v3 = v2 + 1;
      if (v3 >= distance_to_eject_samus_left)
        distance_to_eject_samus_left = v3;
    }
    v1 += room_width_in_blocks * 2;
    --R26_;
  } while ((R26_ & 0x8000u) == 0);
}

void PostGrappleCollisionDetect_Left(void) {  // 0x9483B1
  int16 v0;
  int16 v2;

  samus_collision_direction = 0;
  distance_to_eject_samus_right = 0;
  Samus_GetYposSpan();
  uint16 prod = Mult8x8((uint16)(samus_y_pos - samus_y_radius) >> 4, room_width_in_blocks);
  R22_ = samus_x_subpos;
  R24_ = samus_x_pos;
  R32 = samus_x_pos - samus_x_radius;
  v0 = (uint16)(samus_x_pos - samus_x_radius) >> 4;
  cur_block_index = prod + v0;
  uint16 v1 = 2 * cur_block_index;
  do {
    v2 = PostGrappleColl_Horiz(v1);
    if (v2 >= 0) {
      uint16 v3 = v2 + 1;
      if (v3 >= distance_to_eject_samus_right)
        distance_to_eject_samus_right = v3;
    }
    v1 += room_width_in_blocks * 2;
    --R26_;
  } while ((R26_ & 0x8000u) == 0);
}

void PostGrappleCollisionDetect_Down(void) {  // 0x94840F
  int16 v0;
  int16 v2;

  samus_collision_direction = 3;
  distance_to_eject_samus_up = 0;
  Samus_GetXposSpan();
  R22_ = samus_y_subpos;
  R24_ = samus_y_pos;
  R32 = samus_y_radius + samus_y_pos - 1;
  uint16 prod = Mult8x8(R32 >> 4, room_width_in_blocks);
  v0 = (uint16)(samus_x_pos - samus_x_radius) >> 4;
  cur_block_index = prod + v0;
  uint16 v1 = 2 * cur_block_index;
  do {
    v2 = PostGrappleColl_Vert(v1);
    if (v2 >= 0) {
      uint16 v3 = v2 + 1;
      if (v3 >= distance_to_eject_samus_up)
        distance_to_eject_samus_up = v3;
    }
    v1 += 2;
    --R26_;
  } while ((R26_ & 0x8000u) == 0);
}

void PostGrappleCollisionDetect_Up(void) {  // 0x94846A
  int16 v0;
  int16 v2;

  samus_collision_direction = 2;
  distance_to_eject_samus_down = 0;
  Samus_GetXposSpan();
  R22_ = samus_y_subpos;
  R24_ = samus_y_pos;
  R32 = samus_y_pos - samus_y_radius;
  uint16 prod = Mult8x8((uint16)(samus_y_pos - samus_y_radius) >> 4, room_width_in_blocks);
  v0 = (uint16)(samus_x_pos - samus_x_radius) >> 4;
  cur_block_index = prod + v0;
  uint16 v1 = 2 * cur_block_index;
  do {
    v2 = PostGrappleColl_Vert(v1);
    if (v2 >= 0) {
      uint16 v3 = v2 + 1;
      if (v3 >= distance_to_eject_samus_down)
        distance_to_eject_samus_down = v3;
    }
    v1 += 2;
    --R26_;
  } while ((R26_ & 0x8000u) == 0);
}

void PostGrappleCollisionDetect_X(void) {  // 0x9484C4
  PostGrappleCollisionDetect_Right();
  PostGrappleCollisionDetect_Left();
}

void PostGrappleCollisionDetect_Y(void) {  // 0x9484CD
  PostGrappleCollisionDetect_Down();
  PostGrappleCollisionDetect_Up();
}

uint8 BlockColl_Horiz_Slope_NonSquare(void) {  // 0x9484D6
  uint16 v2;

  if ((current_slope_bts & 0x80) != 0 || __PAIR32__(samus_y_speed, samus_y_subspeed))
    return 0;
  uint16 v1 = 4 * (current_slope_bts & 0x1F);
  HIBYTE(v2) = R18_;
  LOBYTE(v2) = HIBYTE(R20_);
  if ((R18_ & 0x8000u) == 0) {
    Multiply16x16(v2, kBlockColl_Horiz_Slope_NonSquare_Tab[(v1 >> 1) + 1]);
    R20_ = mult_product_lo;
    R18_ = mult_product_hi;
  } else {
    Multiply16x16(-v2, kBlockColl_Horiz_Slope_NonSquare_Tab[(v1 >> 1) + 1]);
    Negate32(&mult_product_hi, &mult_product_lo, &R18_, &R20_);
  }
  return 0;
}


uint8 BlockColl_Vert_Slope_NonSquare(uint16 k) {  // 0x9486FE
  int16 v1;
  int16 v3;
  char v4;
  uint16 v5;
  int16 v6;
  int16 v7;
  int16 v8;
  int16 v9;
  char v10;
  uint16 v11;
  int16 v12;
  int16 v13;

  if (samus_collision_direction & 1) {
    uint16 mod = SnesModulus(cur_block_index, room_width_in_blocks);
    v8 = samus_x_pos >> 4;
    if (v8 == mod) {
      temp_collision_DD4 = ((uint8)samus_y_radius + (uint8)R24_ - 1) & 0xF;
      temp_collision_DD6 = 16 * (BTS[k] & 0x1F);
      v9 = BTS[k];
      if (!(v9 & 0x80)
          && ((v9 & 0x40) != 0 ? (v10 = samus_x_pos ^ 0xF) : (v10 = samus_x_pos),
              (v11 = temp_collision_DD6 + (v10 & 0xF),
               v12 = (kAlignYPos_Tab0[v11] & 0x1F) - temp_collision_DD4 - 1,
               (kAlignYPos_Tab0[v11] & 0x1F) - temp_collision_DD4 == 1)
              || v12 < 0)) {
        v13 = R18_ + v12;
        if (v13 < 0)
          v13 = 0;
        R18_ = v13;
        R20_ = 0;
        return 1;
      } else {
        return 0;
      }
    } else {
      return 0;
    }
  } else {
    uint16 mod = SnesModulus(cur_block_index, room_width_in_blocks);
    v1 = samus_x_pos >> 4;
    if (v1 == mod) {
      temp_collision_DD4 = ((uint8)R24_ - (uint8)samus_y_radius) & 0xF ^ 0xF;
      temp_collision_DD6 = 16 * (BTS[k] & 0x1F);
      v3 = BTS[k];
      if ((v3 & 0x80)
          && ((v3 & 0x40) != 0 ? (v4 = samus_x_pos ^ 0xF) : (v4 = samus_x_pos),
              (v5 = temp_collision_DD6 + (v4 & 0xF),
               v6 = (kAlignYPos_Tab0[v5] & 0x1F) - temp_collision_DD4 - 1,
               (kAlignYPos_Tab0[v5] & 0x1F) - temp_collision_DD4 == 1)
              || v6 < 0)) {
        v7 = R18_ + v6;
        if (v7 < 0)
          v7 = 0;
        R18_ = v7;
        R20_ = 0;
        return 1;
      } else {
        return 0;
      }
    } else {
      return 0;
    }
  }
}

void Samus_AlignYPosSlope(void) {  // 0x9487F4
  int16 v0;
  char v1;
  int16 v2;
  int16 v3;
  char v4;
  int16 v6;

  if ((enable_horiz_slope_coll & 2) != 0) {
    R26_ = samus_x_pos;
    R28_ = samus_y_radius + samus_y_pos - 1;
    g_word_7E001E = 0;
    R32 = 0;
    CalculateBlockAt();
    if ((level_data[cur_block_index] & 0xF000) == 4096
        && (BTS[cur_block_index] & 0x1Fu) >= 5) {
      temp_collision_DD4 = ((uint8)samus_y_radius + (uint8)samus_y_pos - 1) & 0xF;
      temp_collision_DD6 = 16 * (BTS[cur_block_index] & 0x1F);
      v0 = BTS[cur_block_index];
      if (!(v0 & 0x80)) {
        v1 = (v0 & 0x40) != 0 ? samus_x_pos ^ 0xF : samus_x_pos;
        v2 = (kAlignYPos_Tab0[(uint16)(temp_collision_DD6 + (v1 & 0xF))] & 0x1F) - temp_collision_DD4 - 1;
        if (v2 < 0) {
          samus_y_pos += v2;
          samus_pos_adjusted_by_slope_flag = 1;
        }
      }
    }
    R26_ = samus_x_pos;
    R28_ = samus_y_pos - samus_y_radius;
    g_word_7E001E = 0;
    R32 = 0;
    CalculateBlockAt();
    if ((level_data[cur_block_index] & 0xF000) == 4096
        && (BTS[cur_block_index] & 0x1Fu) >= 5) {
      temp_collision_DD4 = ((uint8)samus_y_pos - (uint8)samus_y_radius) & 0xF ^ 0xF;
      temp_collision_DD6 = 16 * (BTS[cur_block_index] & 0x1F);
      v3 = BTS[cur_block_index];
      if (v3 & 0x80) {
        if ((v3 & 0x40) != 0)
          v4 = samus_x_pos ^ 0xF;
        else
          v4 = samus_x_pos;
        uint16 v5 = temp_collision_DD6 + (v4 & 0xF);
        v6 = (kAlignYPos_Tab0[v5] & 0x1F) - temp_collision_DD4 - 1;
        if ((kAlignYPos_Tab0[v5] & 0x1F) - temp_collision_DD4 == 1 || v6 < 0) {
          samus_y_pos -= v6;
          samus_pos_adjusted_by_slope_flag = 1;
        }
      }
    }
  }
}

uint8 BlockColl_Horiz_Slope_Square(uint16 a, uint16 k) {  // 0x948D2B
  int16 v4;
  int16 v5;

  temp_collision_DD4 = 4 * a;
  temp_collision_DD6 = BTS[k] >> 6;
  uint16 v2 = 4 * a + (temp_collision_DD6 ^ ((uint8)(R32 & 8) >> 3));
  if (!R26_) {
    if ((((uint8)samus_y_radius + (uint8)samus_y_pos - 1) & 8) == 0) {
      if (!kTab948E54[v2])
        return 0;
      goto LABEL_10;
    }
    goto LABEL_7;
  }
  if (R26_ != R28_ || (((uint8)samus_y_pos - (uint8)samus_y_radius) & 8) == 0) {
LABEL_7:
    if (kTab948E54[v2])
      goto LABEL_10;
  }
  if (!kTab948E54[v2 ^ 2])
    return 0;
LABEL_10:
  R20_ = 0;
  if ((R18_ & 0x8000u) != 0) {
    v5 = samus_x_radius + (R32 | 7) + 1 - samus_x_pos;
    if (v5 >= 0)
      v5 = 0;
    R18_ = v5;
    samus_x_subpos = 0;
    return 1;
  } else {
    v4 = (__PAIR32__((R32 & 0xFFF8) - samus_x_radius, R32 & 0xFFF8) - __PAIR32__(samus_x_pos, samus_x_radius)) >> 16;
    if (v4 < 0)
      v4 = 0;
    R18_ = v4;
    samus_x_subpos = -1;
    return 1;
  }
}

uint8 BlockColl_Vert_Slope_Square(uint16 a, uint16 k) {  // 0x948DBD
  int16 v4;
  int16 v5;

  temp_collision_DD4 = 4 * a;
  temp_collision_DD6 = BTS[k] >> 6;
  uint16 v2 = 4 * a + (temp_collision_DD6 ^ ((uint8)(R32 & 8) >> 2));
  if (!R26_) {
    if ((((uint8)samus_x_radius + (uint8)samus_x_pos - 1) & 8) == 0) {
      if (!kTab948E54[v2])
        return 0;
      goto LABEL_10;
    }
    goto LABEL_7;
  }
  if (R26_ != R28_ || (((uint8)samus_x_pos - (uint8)samus_x_radius) & 8) == 0) {
LABEL_7:
    if (kTab948E54[v2])
      goto LABEL_10;
  }
  if (!kTab948E54[v2 ^ 1])
    return 0;
LABEL_10:
  R20_ = 0;
  if ((R18_ & 0x8000u) != 0) {
    v5 = samus_y_radius + (R32 | 7) + 1 - samus_y_pos;
    if (v5 >= 0)
      v5 = 0;
    R18_ = v5;
    samus_y_subpos = 0;
    return 1;
  } else {
    v4 = (__PAIR32__((R32 & 0xFFF8) - samus_y_radius, R32 & 0xFFF8) - __PAIR32__(samus_y_pos, samus_y_radius)) >> 16;
    if (v4 < 0)
      v4 = 0;
    R18_ = v4;
    samus_y_subpos = -1;
    samus_pos_adjusted_by_slope_flag = 1;
    return 1;
  }
}


uint8 ClearCarry_1(void) {  // 0x948E7D
  return 0;
}

uint8 ClearCarry_2(void) {  // 0x948E7F
  return 0;
}

uint8 ClearCarry_3(void) {  // 0x948E81
  return 0;
}

void BlockColl_SpikeBlock_BTS0(void) {  // 0x948E83
  if ((area_index != 3 || CheckBossBitForCurArea(1u) & 1) && !samus_invincibility_timer) {
    samus_invincibility_timer = 60;
    samus_knockback_timer = 10;
    samus_periodic_damage += 60;
    knockback_x_dir = ((*(uint16 *)&samus_pose_x_dir ^ 0xC) & 8) != 0;
  }
}

void BlockColl_SpikeBlock_BTS1(void) {  // 0x948ECF
  if (!samus_invincibility_timer) {
    samus_invincibility_timer = 60;
    samus_knockback_timer = 10;
    samus_periodic_damage += 16;
    knockback_x_dir = ((*(uint16 *)&samus_pose_x_dir ^ 0xC) & 8) != 0;
  }
}

void BlockColl_SpikeBlock_BTS3(void) {  // 0x948F0A
  if (!samus_invincibility_timer) {
    samus_invincibility_timer = 60;
    samus_knockback_timer = 10;
    samus_periodic_damage += 16;
    knockback_x_dir = ((*(uint16 *)&samus_pose_x_dir ^ 0xC) & 8) != 0;
  }
}

uint8 SetCarry(void) {
  return 1;
}

uint8 ClearCarry_4(void) {  // 0x948F47
  return 0;
}

uint8 BlockColl_Horiz_SolidShootGrappleBlock(void) {  // 0x948F49
  int16 v0;
  int16 v2;

  R20_ = 0;
  if ((R18_ & 0x8000u) != 0) {
    v2 = samus_x_radius + (R32 | 0xF) + 1 - samus_x_pos;
    if (v2 >= 0)
      v2 = 0;
    R18_ = v2;
    samus_x_subpos = 0;
    return 1;
  } else {
    v0 = (R32 & 0xFFF0) - samus_x_radius - samus_x_pos;
    if (v0 < 0)
      v0 = 0;
    R18_ = v0;
    samus_x_subpos = -1;
    return 1;
  }
}

uint8 BlockColl_Vert_SolidShootGrappleBlock(void) {  // 0x948F82
  int16 v0;
  int16 v2;

  R20_ = 0;
  if ((R18_ & 0x8000u) != 0) {
    v2 = samus_y_radius + (R32 | 0xF) + 1 - samus_y_pos;
    if (v2 >= 0)
      v2 = 0;
    R18_ = v2;
    samus_y_subpos = 0;
    return 1;
  } else {
    v0 = (R32 & 0xFFF0) - samus_y_radius - samus_y_pos;
    if (v0 < 0)
      v0 = 0;
    R18_ = v0;
    samus_y_subpos = -1;
    return 1;
  }
}

uint8 BlockColl_Horiz_Slope(void) {  // 0x948FBB
  uint16 v0 = BTS[cur_block_index] & 0x1F;
  if (v0 < 5u)
    return BlockColl_Horiz_Slope_Square(v0, cur_block_index);
  current_slope_bts = BTS[cur_block_index];
  return BlockColl_Horiz_Slope_NonSquare();
}

uint8 BlockColl_Vert_Slope(void) {  // 0x948FDA
  uint16 v0 = BTS[cur_block_index] & 0x1F;
  if (v0 < 5u)
    return BlockColl_Vert_Slope_Square(v0, cur_block_index);
  current_slope_bts = *(uint16 *)&BTS[cur_block_index];
  return BlockColl_Vert_Slope_NonSquare(cur_block_index);
}

uint8 ClearCarry_5(void) {  // 0x949018
  return 0;
}

uint8 BlockColl_Vert_SpikeAir(void) {  // 0x94901A
  return 0;
}

void SetCarry_Spikeblk(void) {
}

static Func_V *const kBlockColl_SpikeBlock[16] = {  // 0x94904B
  BlockColl_SpikeBlock_BTS0,
  BlockColl_SpikeBlock_BTS1,
  SetCarry_Spikeblk,
  BlockColl_SpikeBlock_BTS3,
  SetCarry_Spikeblk,
  SetCarry_Spikeblk,
  SetCarry_Spikeblk,
  SetCarry_Spikeblk,
  SetCarry_Spikeblk,
  SetCarry_Spikeblk,
  SetCarry_Spikeblk,
  SetCarry_Spikeblk,
  SetCarry_Spikeblk,
  SetCarry_Spikeblk,
  SetCarry_Spikeblk,
  SetCarry_Spikeblk,
};

uint8 BlockColl_Horiz_SpikeBlock(void) {
  kBlockColl_SpikeBlock[(uint16)((2 * *(uint16 *)&BTS[cur_block_index]) & 0x1FF) >> 1]();
  return BlockColl_Horiz_SolidShootGrappleBlock();
}

uint8 BlockColl_Vert_SpikeBlock(void) {  // 0x94905D
  kBlockColl_SpikeBlock[(uint16)((2 * *(uint16 *)&BTS[cur_block_index]) & 0x1FF) >> 1]();
  return BlockColl_Vert_SolidShootGrappleBlock();
}

uint8 BlockColl_Horiz_SpecialAir(void) {  // 0x94906F
  int16 v0;

  v0 = BTS[cur_block_index];
  if ((v0 & 0x80) != 0) {
    uint16 *v2 = (uint16 *)RomPtr_94(off_9492D9[area_index]);
    return SpawnPLM(v2[v0 & 0x7f]) & 1;
  } else {
    SpawnPLM(kPlmHeaderDefPtrs[v0]);
    return 0;
  }
}

uint8 BlockColl_Vert_SpecialAir(void) {  // 0x94909D
  int16 v0;

  v0 = BTS[cur_block_index];
  if ((v0 & 0x80) != 0) {
    uint16 *v2 = (uint16 *)RomPtr_94(off_9492D9[area_index]);
    return SpawnPLM(v2[v0 & 0x7f]) & 1;
  } else {
    SpawnPLM(kPlmHeaderDefPtrs[v0]);
    return 0;
  }
}

uint8 BlockColl_Horiz_SpecialBlock(void) {  // 0x9490CB
  int16 v0;

  v0 = BTS[cur_block_index];
  if ((v0 & 0x80) != 0) {
    uint16 *v2 = (uint16 *)RomPtr_94(off_9492E9[area_index]);
    uint8 v4 = SpawnPLM(v2[v0 & 0x7f]) & 1;
    if (v4)
      return BlockColl_Horiz_SolidShootGrappleBlock();
    return v4;
  } else {
    uint8 v1 = SpawnPLM(kPlmHeaderDefPtrs[v0]) & 1;
    if (v1)
      return BlockColl_Horiz_SolidShootGrappleBlock();
    return v1;
  }
}

uint8 BlockColl_Vert_SpecialBlock(void) {  // 0x949102
  int16 v0;

  v0 = BTS[cur_block_index];
  if ((v0 & 0x80) != 0) {
    uint16 *v3 = (uint16*)RomPtr_94(off_9492E9[area_index]);
    uint8 v4 = SpawnPLM(v3[v0 & 0x7F]) & 1;
    if (v4)
      return BlockColl_Vert_SolidShootGrappleBlock();
    return v4;
  } else {
    uint8 v1 = SpawnPLM(kPlmHeaderDefPtrs[v0]) & 1;
    if (v1)
      return BlockColl_Vert_SolidShootGrappleBlock();
    return v1;
  }
}

#define off_94936B ((uint16*)RomPtr(0x94936b))

uint8 BlockColl_Horiz_BombableAir(void) {  // 0x9492F9
  int16 v0;

  v0 = BTS[cur_block_index];
  if ((v0 & 0x80) == 0)
    SpawnPLM(off_94936B[v0]);
  return 0;
}

uint8 BlockColl_Vert_BombableAir(void) {  // 0x949313
  int16 v0;

  v0 = BTS[cur_block_index];
  if ((v0 & 0x80) == 0)
    SpawnPLM(off_94936B[v0]);
  return 0;
}

uint8 BlockColl_Horiz_BombBlock(void) {  // 0x94932D
  int16 v0;

  v0 = BTS[cur_block_index];
  if ((v0 & 0x80) != 0)
    return BlockColl_Horiz_SolidShootGrappleBlock();
  uint8 v1 = SpawnPLM(off_94936B[v0]) & 1;
  if (v1)
    return BlockColl_Horiz_SolidShootGrappleBlock();
  else
    return v1;
}

uint8 BlockColl_Vert_BombBlock(void) {  // 0x94934C
  int16 v0;

  v0 = BTS[cur_block_index];
  if ((v0 & 0x80) != 0)
    return BlockColl_Vert_SolidShootGrappleBlock();
  uint8 v1 = SpawnPLM(off_94936B[v0]) & 1;
  if (v1)
    return BlockColl_Vert_SolidShootGrappleBlock();
  else
    return v1;
}

uint8 BlockColl_Horiz_Door(void) {  // 0x94938B
  door_transition_function = FUNC16(DoorTransitionFunction_HandleElevator);
  door_bts = BTS[cur_block_index];
  uint16 v0 = *(uint16 *)RomPtr_8F(door_list_pointer + 2 * (door_bts & 0x7Fu));
  if ((get_DoorDef(v0)->room_definition_ptr & 0x8000u) == 0) {
    if (samus_pose < kGameState_9_HitDoorBlock)
      elevator_flags = 1;
    return BlockColl_Horiz_SolidShootGrappleBlock();
  } else {
    door_def_ptr = v0;
    game_state = kGameState_9_HitDoorBlock;
    return 0;
  }
}

uint8 BlockColl_Vert_Door(void) {  // 0x9493CE
  door_transition_function = FUNC16(DoorTransitionFunction_HandleElevator);
  door_bts = BTS[cur_block_index];
  uint16 v0 = *(uint16 *)RomPtr_8F(door_list_pointer + 2 * (door_bts & 0x7Fu));
  if ((get_DoorDef(v0)->room_definition_ptr & 0x8000u) == 0) {
    if (samus_pose < kPose_09_MoveR_NoAim)
      elevator_flags = 1;
    return BlockColl_Vert_SolidShootGrappleBlock();
  } else {
    door_def_ptr = v0;
    game_state = kPose_09_MoveR_NoAim;
    return 0;
  }
}

uint8 BlockReact_HorizExt(void) {  // 0x949411
  uint8 t = BTS[cur_block_index];
  if (t) {
    cur_block_index += (int8)t;
    return 0xff; // special
  }
  return 0;
}

uint8 BlockReact_VertExt(void) {  // 0x949447
  uint16 v0;
  if (BTS[cur_block_index]) {
    if ((BTS[cur_block_index] & 0x80) != 0) {
      temp_collision_DD4 = BTS[cur_block_index] | 0xFF00;
      v0 = cur_block_index;
      do {
        v0 -= room_width_in_blocks;
        ++temp_collision_DD4;
      } while (temp_collision_DD4);
    } else {
      temp_collision_DD4 = BTS[cur_block_index];
      v0 = cur_block_index;
      do {
        v0 += room_width_in_blocks;
        --temp_collision_DD4;
      } while (temp_collision_DD4);
    }
    cur_block_index = v0;
    return 0xff; // special
  }
  return 0;
}

void Samus_GetYposSpan(void) {  // 0x949495
  R26_ = (samus_y_pos - samus_y_radius) & 0xFFF0;
  R26_ = (uint16)(samus_y_radius + samus_y_pos - 1 - R26_) >> 4;
  R28_ = R26_;
}

void Samus_GetXposSpan(void) {  // 0x9494B5
  R26_ = (samus_x_pos - samus_x_radius) & 0xFFF0;
  R26_ = (uint16)(samus_x_radius + samus_x_pos - 1 - R26_) >> 4;
  R28_ = R26_;
}

static Func_U8 *const kBlockColl_Horiz_CheckColl[16] = {  // 0x949515
  ClearCarry_4,
  BlockColl_Horiz_Slope,
  ClearCarry_5,
  BlockColl_Horiz_SpecialAir,
  ClearCarry_4,
  BlockReact_HorizExt,
  ClearCarry_4,
  BlockColl_Horiz_BombableAir,
  BlockColl_Horiz_SolidShootGrappleBlock,
  BlockColl_Horiz_Door,
  BlockColl_Horiz_SpikeBlock,
  BlockColl_Horiz_SpecialBlock,
  BlockColl_Horiz_SolidShootGrappleBlock,
  BlockReact_VertExt,
  BlockColl_Horiz_SolidShootGrappleBlock,
  BlockColl_Horiz_BombBlock,
};
static Func_U8 *const kBlockColl_Vert_CheckColl[16] = {
  ClearCarry_4,
  BlockColl_Vert_Slope,
  BlockColl_Vert_SpikeAir,
  BlockColl_Vert_SpecialAir,
  ClearCarry_4,
  BlockReact_HorizExt,
  ClearCarry_4,
  BlockColl_Vert_BombableAir,
  BlockColl_Vert_SolidShootGrappleBlock,
  BlockColl_Vert_Door,
  BlockColl_Vert_SpikeBlock,
  BlockColl_Vert_SpecialBlock,
  BlockColl_Vert_SolidShootGrappleBlock,
  BlockReact_VertExt,
  BlockColl_Vert_SolidShootGrappleBlock,
  BlockColl_Vert_BombBlock,
};

uint8 BlockColl_Horiz_CheckColl(uint16 k) {
  uint8 rv;
  cur_block_index = k >> 1;
  do {
    rv = kBlockColl_Horiz_CheckColl[(level_data[cur_block_index] & 0xF000) >> 12]();
  } while (rv & 0x80);
  return rv;
}

uint8 BlockColl_Vert_CheckColl(uint16 k) {  // 0x94952C
  uint8 rv;
  cur_block_index = k >> 1;
  do {
    rv = kBlockColl_Vert_CheckColl[(level_data[cur_block_index] & 0xF000) >> 12]();
  } while (rv & 0x80);
  return rv;
}

uint8 BlockColl_Handle_Horiz(void) {  // 0x949543
  Samus_GetYposSpan();
  uint16 prod = Mult8x8((uint16)(samus_y_pos - samus_y_radius) >> 4, room_width_in_blocks);
  uint16 v1;
  R22_ = R20_ + samus_x_subpos;
  uint16 v0 = R18_ + __CFADD__uint16(R20_, samus_x_subpos) + samus_x_pos;
  R24_ = v0;
  if ((R18_ & 0x8000u) == 0)
    v1 = samus_x_radius + v0 - 1;
  else
    v1 = v0 - samus_x_radius;
  R32 = v1;
  uint16 v2 = 2 * (prod + (v1 >> 4));
  while (!(BlockColl_Horiz_CheckColl(v2) & 1)) {
    v2 += room_width_in_blocks * 2;
    if ((--R26_ & 0x8000u) != 0)
      return 0;
  }
  return 1;
}

uint8 BlockColl_Handle_Vert_LeftToRight(void) {  // 0x94959E
  int16 v2;

  Samus_GetXposSpan();
  R22_ = R20_ + samus_y_subpos;
  uint16 v0 = R18_ + __CFADD__uint16(R20_, samus_y_subpos) + samus_y_pos, v1;
  R24_ = v0;
  if ((R18_ & 0x8000u) == 0)
    v1 = samus_y_radius + v0 - 1;
  else
    v1 = v0 - samus_y_radius;
  R32 = v1;
  uint16 prod = Mult8x8(v1 >> 4, room_width_in_blocks);
  v2 = (uint16)(samus_x_pos - samus_x_radius) >> 4;
  cur_block_index = prod + v2;
  for (int i = 2 * cur_block_index; !(BlockColl_Vert_CheckColl(i) & 1); i += 2) {
    if ((--R26_ & 0x8000u) != 0)
      return 0;
  }
  return 1;
}

uint8 BlockColl_Handle_Vert_RightToLeft(void) {  // 0x9495F5
  int16 v2;

  Samus_GetXposSpan();
  R26_ = 0;
  R22_ = R20_ + samus_y_subpos;
  uint16 v0 = R18_ + __CFADD__uint16(R20_, samus_y_subpos) + samus_y_pos, v1;
  R24_ = v0;
  if ((R18_ & 0x8000u) == 0)
    v1 = samus_y_radius + v0 - 1;
  else
    v1 = v0 - samus_y_radius;
  R32 = v1;
  uint16 prod = Mult8x8(v1 >> 4, room_width_in_blocks);
  v2 = (uint16)(samus_x_radius + samus_x_pos - 1) >> 4;
  cur_block_index = prod + v2;
  for (int i = 2 * cur_block_index; !(BlockColl_Vert_CheckColl(i) & 1); i -= 2) {
    if (R28_ < ++R26_)
      return 0;
  }
  return 1;
}

void AbsoluteValueOfInt32(void) {  // 0x949653
  if ((R18_ & 0x8000u) != 0) {
    R18_ = ~R18_;
    bool v0 = R20_ == 0;
    R20_ = -R20_;
    if (v0)
      ++R18_;
  }
}

void AbsoluteValueOfInt32_0(void) {  // 0x949669
  if ((R18_ & 0x8000u) != 0) {
    R18_ = ~R18_;
    bool v0 = R20_ == 0;
    R20_ = -R20_;
    if (v0)
      ++R18_;
  }
}

uint8 WallJumpBlockCollDetect(void) {  // 0x94967F
  samus_collision_direction |= 0xFu;
  flag_samus_in_quicksand = 0;
  if (BlockColl_Handle_Horiz()) {
    AbsoluteValueOfInt32();
    samus_collision_flag = 1;
    return 1;
  } else {
    AbsoluteValueOfInt32();
    samus_collision_flag = 0;
    return 0;
  }
}

uint8 Samus_CollDetectChangedPose(void) {  // 0x9496AB
  if ((abs16(R18_) & 0xFFF8) == 0)
    return CollDetectDueToPoseChange_SingleBlock();
  uint16 v3 = R18_;
  uint16 v2 = R20_;
  R18_ = R18_ & 0xFFF0 | 8;
  R20_ = 0;
  uint8 v1 = CollDetectDueToPoseChange_SingleBlock();
  if (v1)
    return v1;
  R20_ = v2;
  R18_ = v3;
  return CollDetectDueToPoseChange_SingleBlock();
}

uint8 CollDetectDueToPoseChange_SingleBlock(void) {  // 0x9496E3
  char v0; // cf

  samus_collision_direction |= 0xFu;
  flag_samus_in_quicksand = 0;
  if (!(nmi_frame_counter_word & 1)) {
    v0 = BlockColl_Handle_Vert_LeftToRight();
    if (!v0)
      goto LABEL_6;
    if (v0) {
LABEL_5:
      AbsoluteValueOfInt32_0();
      samus_collision_flag = 1;
      return 1;
    }
  }
  if (BlockColl_Handle_Vert_RightToLeft())
    goto LABEL_5;
LABEL_6:
  AbsoluteValueOfInt32_0();
  samus_collision_flag = 0;
  return 0;
}

uint8 Samus_MoveRight_NoSolidColl(void) {  // 0x94971E
  if (__PAIR32__(R20_, R18_) && (flag_samus_in_quicksand = 0, BlockColl_Handle_Horiz())) {
    bool v0 = __CFADD__uint16(R20_, samus_x_subpos);
    samus_x_subpos += R20_;
    samus_x_pos += R18_ + v0;
    samus_collision_flag = 1;
    return 1;
  } else {
    bool v0 = __CFADD__uint16(R20_, samus_x_subpos);
    samus_x_subpos += R20_;
    samus_x_pos += R18_ + v0;
    samus_collision_flag = 0;
    return 0;
  }
}

uint8 Samus_MoveDown_NoSolidColl(void) {  // 0x949763
  if (__PAIR32__(R20_, R18_)) {
    samus_pos_adjusted_by_slope_flag = 0;
    flag_samus_in_quicksand = 0;
    if (!(nmi_frame_counter_word & 1)) {
      bool v0;
      v0 = BlockColl_Handle_Vert_LeftToRight();
      if (!v0)
        goto LABEL_8;
      if (v0) {
LABEL_6:
        v0 = __CFADD__uint16(R20_, samus_y_subpos);
        samus_y_subpos += R20_;
        samus_y_pos += R18_ + v0;
LABEL_7:
        samus_collision_flag = 1;
        return 1;
      }
    }
    if (BlockColl_Handle_Vert_RightToLeft())
      goto LABEL_6;
  }
LABEL_8:;
  bool v0 = __CFADD__uint16(R20_, samus_y_subpos);
  samus_y_subpos += R20_;
  samus_y_pos += R18_ + v0;
  if (flag_samus_in_quicksand)
    goto LABEL_7;
  samus_collision_flag = 0;
  return 0;
}

uint8 BlockInsideReact_Slope(void) {  // 0x9497BF
  return 0;
}

uint8 BlockInsideReact_ShootableAir(void) {  // 0x9497D0
  samus_x_speed_table_pointer = addr_kSamusSpeedTable_Normal_X__plus__12;
  return 0;
}

void ClearCarry_8(void) {  // 0x9497D8
}

uint8 MaybeUnused_sub_9497DA(void) {  // 0x9497DA
  ++samus_periodic_damage;
  samus_x_extra_run_speed = 0;
  samus_x_base_speed = 0;
  return 0;
}

uint8 MaybeUnused_sub_9497F2(void) {  // 0x9497F2
  if (samus_y_dir == 1) {
    samus_y_subaccel = 0;
    samus_y_accel = 2;
  } else {
    samus_y_speed = 0;
    samus_y_subspeed = 0;
  }
  return 0;
}

uint8 ClearCarry_6(void) {  // 0x949810
  return 0;
}

uint8 ClearCarry_7(void) {  // 0x949812
  return 0;
}

uint8 BlockInsideReact_SpikeAir_BTS1(void) {  // 0x949814
  ++samus_periodic_damage;
  samus_x_speed_table_pointer = addr_kSamusSpeedTable_LavaAcid_X;
  samus_y_subaccel = 0;
  samus_y_accel = 0;
  if (samus_y_dir == 1) {
    samus_y_subaccel = 0;
    samus_y_accel = 5;
  } else {
    samus_y_speed = 0;
    samus_y_subspeed = 0;
    extra_samus_y_subdisplacement = 0;
    extra_samus_y_displacement = 2;
  }
  return 0;
}

void nullsub_165(void) {  // 0x9497D7
}

void BlockInsideReact_SpikeAir_BTS2(void) {  // 0x949866
  if (!samus_contact_damage_index && !samus_invincibility_timer) {
    samus_invincibility_timer = 60;
    samus_knockback_timer = 10;
    samus_periodic_damage += 16;
    knockback_x_dir = ((*(uint16 *)&samus_pose_x_dir ^ 0xC) & 8) != 0;
  }
  samus_x_speed_table_pointer = addr_kSamusSpeedTable_Normal_X__plus__12;
}

static Func_V *const kBlockInsideReact_SpikeAir[16] = {  // 0x9498CC
  ClearCarry_8,
  ClearCarry_8,
  BlockInsideReact_SpikeAir_BTS2,
  nullsub_165,
  nullsub_165,
  nullsub_165,
  nullsub_165,
  nullsub_165,
  nullsub_165,
  nullsub_165,
  nullsub_165,
  nullsub_165,
  nullsub_165,
  nullsub_165,
  nullsub_165,
  nullsub_165,
};

uint8 BlockInsideReact_SpikeAir(void) {
  kBlockInsideReact_SpikeAir[BTS[cur_block_index]]();
  return 0;
}

uint8 BlockInsideReact_Special_(void) {  // 0x9498DC
  samus_x_speed_table_pointer = addr_kSamusSpeedTable_Normal_X__plus__12;
  return 0;
}

void BlockInsideReact_SpecialAir_Default(void) {  // 0x9498E3
  samus_x_speed_table_pointer = addr_kSamusSpeedTable_Normal_X__plus__12;
}

void BlockInsideReact_SpecialAir_8(void) {  // 0x9498EA
  if ((area_index != 3 || CheckBossBitForCurArea(1u) & 1) && !samus_y_speed) {
    extra_samus_x_subdisplacement = 0;
    extra_samus_x_displacement = 2;
  }
  samus_x_speed_table_pointer = addr_kSamusSpeedTable_Normal_X__plus__12;
}

void BlockInsideReact_SpecialAir_9(void) {  // 0x949910
  if ((area_index != 3 || CheckBossBitForCurArea(1u) & 1) && !samus_y_speed) {
    extra_samus_x_subdisplacement = 0;
    extra_samus_x_displacement = -2;
  }
  samus_x_speed_table_pointer = addr_kSamusSpeedTable_Normal_X__plus__12;
}

void BlockInsideReact_SpecialAir_10(void) {  // 0x949936
  extra_samus_x_subdisplacement = 0;
  extra_samus_x_displacement = 2;
  samus_x_speed_table_pointer = addr_kSamusSpeedTable_Normal_X__plus__12;
}

void BlockInsideReact_SpecialAir_11(void) {  // 0x949946
  extra_samus_x_subdisplacement = 0;
  extra_samus_x_displacement = -2;
  samus_x_speed_table_pointer = addr_kSamusSpeedTable_Normal_X__plus__12;
}

void BlockInsideReact_SpecialAir_70(void) {  // 0x949956
  if (inside_block_reaction_samus_point == 1)
    SpawnPLM(addr_kPlmHeader_B6FF);
}

static const uint16 g_off_949B06[8] = {  // 0x949B16
  0x9a06,
  0x9a26,
  0x9a46,
  0x9a66,
  0x9a86,
  0x9aa6,
  0x9ac6,
  0x9ae6,
};

static Func_V *const off_949966[80] = {
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_8,
  BlockInsideReact_SpecialAir_9,
  BlockInsideReact_SpecialAir_10,
  BlockInsideReact_SpecialAir_11,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_70,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
  BlockInsideReact_SpecialAir_Default,
};

uint8 BlockInsideReact_SpecialAir(void) {
  int16 v0;

  v0 = BTS[cur_block_index] << 8;
  if (v0 < 0) {
    R34 = g_off_949B06[area_index];
    uint8 *v3 = RomPtr_94(R34);
    SpawnPLM(*(uint16 *)&v3[(2 * (HIBYTE(v0) & 0x7F))]);
  } else {
    off_949966[v0 >> 8]();
  }
  return 0;
}

void BlockInsideDetection(void) {  // 0x949B60
  static Func_U8 *const kSamus_BlockInsideDetection[16] = {
    BlockInsideReact_ShootableAir,
    BlockInsideReact_Slope,
    BlockInsideReact_SpikeAir,
    BlockInsideReact_SpecialAir,
    BlockInsideReact_ShootableAir,
    BlockReact_HorizExt,
    BlockInsideReact_ShootableAir,
    BlockInsideReact_ShootableAir,
    BlockInsideReact_Special_,
    BlockInsideReact_Special_,
    BlockInsideReact_Special_,
    BlockInsideReact_Special_,
    BlockInsideReact_Special_,
    BlockReact_VertExt,
    BlockInsideReact_Special_,
    BlockInsideReact_Special_,
  };

  *(uint16 *)&samus_x_decel_mult = 0;
  extra_samus_x_subdisplacement = 0;
  extra_samus_x_displacement = 0;
  extra_samus_y_subdisplacement = 0;
  extra_samus_y_displacement = 0;
  inside_block_reaction_samus_point = 0;
  R26_ = samus_x_pos;
  samus_bottom_boundary_position = samus_y_radius + samus_y_pos - 1;
  R28_ = samus_y_radius + samus_y_pos - 1;
  g_word_7E001E = 0;
  R32 = 0;
  CalculateBlockAt();
  uint8 rv;
  do {
    rv = kSamus_BlockInsideDetection[(HIBYTE(level_data[cur_block_index]) & 0xF0) >> 4]();
  } while (rv & 0x80);
  inside_block_reaction_samus_point = 1;
  if (((samus_bottom_boundary_position ^ samus_y_pos) & 0xFFF0) != 0) {
    R26_ = samus_x_pos;
    R28_ = samus_y_pos;
    g_word_7E001E = 0;
    R32 = 0;
    CalculateBlockAt();
    do {
      rv = kSamus_BlockInsideDetection[(HIBYTE(level_data[cur_block_index]) & 0xF0) >> 4]();
    } while (rv & 0x80);
  }
  inside_block_reaction_samus_point = 2;
  if (((samus_bottom_boundary_position ^ (uint16)(samus_y_pos - samus_y_radius)) & 0xFFF0) != 0
      && ((samus_y_pos ^ (samus_bottom_boundary_position ^ (uint16)(samus_y_pos - samus_y_radius)) & 0xFFF0) & 0xFFF0) != 0) {
    R26_ = samus_x_pos;
    R28_ = samus_y_pos - samus_y_radius;
    g_word_7E001E = 0;
    R32 = 0;
    CalculateBlockAt();
    do {
      rv = kSamus_BlockInsideDetection[(HIBYTE(level_data[cur_block_index]) & 0xF0) >> 4]();
    } while (rv & 0x80);
  }
}

void CalculateBlockAt(void) {  // 0x949C1D
  int16 v0;
  int16 v1;

  v0 = g_word_7E001E + R26_;
  if ((int16)(g_word_7E001E + R26_) >= 0
      && sign16(v0 - 4096)
      && (temp_collision_DD4 = (uint16)(v0 & 0xFFF0) >> 4,
          v1 = R32 + R28_,
          (int16)(R32 + R28_) >= 0)
      && sign16(v1 - 4096)) {
    uint16 RegWord = (uint16)(v1 & 0xFFF0) >> 4;
    RegWord = Mult8x8(RegWord, room_width_in_blocks);
    cur_block_index = temp_collision_DD4 + RegWord;
  } else {
    cur_block_index = -1;
  }
}

static const uint8 kBlockFunc_9C73_Tab0[36] = {  // 0x949C73
   3,  20, 0,
  10,  40, 1,
   5,  30, 0,
  10,  40, 1,
  20,  80, 0,
  20,  80, 1,
  20,  80, 0,
  20,  80, 1,
  30, 100, 0,
  30, 100, 1,
  30, 100, 0,
  30, 100, 1,
};
static const int8 kBlockFunc_9C73_Tab1[18] = {
   0,  0,
  10,  0,
  50,  0,
  50,  3,
   0, -1,
  10,  2,
   0, -1,
   0, -1,
   0, -1,
};

void BlockFunc_9C73(uint16 v0) {

  int v1 = v0 >> 1;
  if ((projectile_type[v1] & 0xF00) != 0) {
    temp_collision_DD2 = kBlockFunc_9C73_Tab1[(uint16)(2 * (HIBYTE(projectile_type[v1]) & 0xF) + 1)];
  } else {
    R18_ = projectile_type[v1] & 0xF;
    temp_collision_DD2 = kBlockFunc_9C73_Tab0[(uint16)(R18_ + 2 * R18_ + 2)];
  }
}

void BombOrPowerBomb_Func1(uint16 v0) {  // 0x949CAC
  int16 v2;
  int16 v3;

  BlockFunc_9C73(v0);
  int v1 = v0 >> 1;
  v2 = projectile_x_pos[v1];
  if (v2 >= 0) {
    R26_ = projectile_x_pos[v1];
    if ((int16)(HIBYTE(v2) - room_width_in_scrolls) < 0) {
      v3 = projectile_y_pos[v1];
      if (v3 >= 0) {
        R28_ = projectile_y_pos[v1];
        if ((int16)(HIBYTE(v3) - room_height_in_scrolls) < 0) {
          g_word_7E001E = 0;
          R32 = 0;
          cur_block_index = 0;
          CalculateBlockAt();
          if (temp_collision_DD2 == 2)
            BlockColl_BombExplosion(v0);
          else
            BlockFunc_A06A(v0);
        }
      }
    }
  }
}

void BlockColl_BombExplosion(uint16 k) {  // 0x949CF4
  int16 v2;

  int v1 = k >> 1;
  if (!projectile_variables[v1]) {
    v2 = projectile_type[v1];
    if ((v2 & 1) == 0) {
      projectile_type[v1] = v2 | 1;
      if (cur_block_index != 0xFFFF) {
        uint16 v3 = 2 * cur_block_index;
        BlockBombedReact(2 * cur_block_index);
        uint16 idx = BlockColl_GetBlockIndexAbove(v3);
        BlockBombedReact(idx);
        uint16 idx2 = BlockColl_GetBlockIndexRight(idx);
        BlockBombedReact(idx2);
        idx = BlockColl_GetBlockIndexLeft(idx2);
        BlockBombedReact(idx);
        idx = BlockColl_GetBlockIndexBelow(idx);
        BlockBombedReact(idx);
      }
    }
  }
}

uint16 BlockColl_GetBlockIndexAbove(uint16 k) {  // 0x949D34
  return k - room_width_in_blocks - room_width_in_blocks;
}

uint16 BlockColl_GetBlockIndexRight(uint16 k) {  // 0x949D3E
  return room_width_in_blocks * 2 + k + 1 + 1;
}

uint16 BlockColl_GetBlockIndexLeft(uint16 k) {  // 0x949D49
  return k - 4;
}

uint16 BlockColl_GetBlockIndexBelow(uint16 k) {  // 0x949D4E
  return room_width_in_blocks * 2 + k + 1 + 1;
}

uint8 ClearCarry_9(void) {  // 0x949D59
  return 0;
}

uint8 SetCarry_0(void) {  // 0x949D5B
  return 1;
}

uint8 sub_949D5D(void) {  // 0x949D5D
  if ((BTS[cur_block_index] & 0x1Fu) < 5)
    return 1;
  else
    return BlockShotReactVert_Slope_NonSquare() & 1;
}

uint8 BlockBombedReact_Special(void) {  // 0x949D71
  int16 v0;

  v0 = BTS[cur_block_index];
  if ((v0 & 0x80) != 0) {
    uint16 *kBlockBombedReact_Region_Plm = (uint16 *)RomPtr_94(0x9e44);
    R18_ = kBlockBombedReact_Region_Plm[area_index];
    uint8 *v2 = RomPtr_94(R18_);
    SpawnPLM(*(uint16 *)&v2[(uint16)(2 * (v0 & 0x7F))]);
  } else {
    uint16 *kBlockBombedReact_Special_Plm = (uint16 *)RomPtr_94(0x9dA4);
    SpawnPLM(kBlockBombedReact_Special_Plm[v0]);
  }
  return 1;
}

uint8 BlockReact_ShootableAir(void) {  // 0x949E55
  int16 v0;
  uint16 *kBlockShotBombedReactionShootablePlm = (uint16 *)RomPtr_94(0x9ea6);
  v0 = BTS[cur_block_index];
  if ((v0 & 0x80) == 0)
    SpawnPLM(kBlockShotBombedReactionShootablePlm[v0]);
  return 0;
}

uint8 BlockReact_Shootable(void) {  // 0x949E73
  int16 v0;

  v0 = BTS[cur_block_index];
  if ((v0 & 0x80) != 0) {
    uint16 *kBlockShotBombGrappleReaction_ShootableBlock_RegionPlm = (uint16 *)RomPtr_94(0x9FC6);
    uint16 *v2 = (uint16*)RomPtr_94(kBlockShotBombGrappleReaction_ShootableBlock_RegionPlm[area_index]);
    SpawnPLM(v2[v0 & 0x7f]);
  } else {
    uint16 *kBlockShotBombedReactionShootablePlm = (uint16 *)RomPtr_94(0x9ea6);
    SpawnPLM(kBlockShotBombedReactionShootablePlm[v0]);
  }
  return 1;
}

uint8 BlockReact_BombableAir(void) {  // 0x949FD6
  int16 v0;

  uint16 *kBlockShotBombGrappleReact_BombableBlockPlm = (uint16 *)RomPtr_94(0xA012);
  v0 = BTS[cur_block_index];
  if ((v0 & 0x80) == 0)
    SpawnPLM(kBlockShotBombGrappleReact_BombableBlockPlm[v0]);
  return 0;
}

uint8 BlockReact_BombableBlock(void) {  // 0x949FF4
  int16 v0;

  uint16 *kBlockShotBombGrappleReact_BombableBlockPlm = (uint16 *)RomPtr_94(0xA012);
  v0 = BTS[cur_block_index];
  if ((v0 & 0x80) == 0)
    SpawnPLM(kBlockShotBombGrappleReact_BombableBlockPlm[v0]);
  return 1;
}

static Func_U8 *const kBlockBombedReact[16] = {  // 0x94A052
  ClearCarry_9,
  ClearCarry_9,
  ClearCarry_9,
  ClearCarry_9,
  BlockReact_ShootableAir,
  BlockReact_HorizExt,
  ClearCarry_9,
  BlockReact_BombableAir,
  SetCarry_0,
  SetCarry_0,
  SetCarry_0,
  BlockBombedReact_Special,
  BlockReact_Shootable,
  BlockReact_VertExt,
  SetCarry_0,
  BlockReact_BombableBlock,
};
void BlockBombedReact(uint16 v0) {
  cur_block_index = v0 >> 1;
  while (kBlockBombedReact[(level_data[cur_block_index] & 0xF000) >> 12]() & 0x80) {}
}

void BlockFunc_A06A(uint16 k) {  // 0x94A06A
  int16 v2;
  int16 v3;

  int v1 = k >> 1;
  v2 = projectile_variables[v1];
  if (v2) {
    if (v2 < 0)
      projectile_variables[v1] = 0;
  } else {
    R18_ = HIBYTE(power_bomb_explosion_radius);
    R20_ = (uint16)(HIBYTE(power_bomb_explosion_radius)
                    + 2 * HIBYTE(power_bomb_explosion_radius)) >> 2;
    v3 = power_bomb_explosion_x_pos - HIBYTE(power_bomb_explosion_radius);
    if (v3 < 0)
      v3 = 0;
    R22_ = (uint16)v3 >> 4;
    uint16 v4 = (uint16)(R18_ + power_bomb_explosion_x_pos) >> 4;
    if (v4 >= room_width_in_blocks)
      v4 = room_width_in_blocks - 1;
    R24_ = v4;
    uint16 v5 = power_bomb_explosion_y_pos - R20_;
    if ((int16)(power_bomb_explosion_y_pos - R20_) < 0)
      v5 = 0;
    R26_ = v5 >> 4;
    uint16 v6 = (uint16)(R20_ + power_bomb_explosion_y_pos) >> 4;
    if (v6 >= room_height_in_blocks)
      v6 = room_height_in_blocks - 1;
    R28_ = v6;
    uint16 a = R26_ | ((uint8)R22_ << 8);
    BlockFunc_A0F4(a);
    BlockFunc_A11A(a);
    BlockFunc_A0F4(R28_ | ((uint8)R22_ << 8));
    BlockFunc_A11A(R26_ | ((uint8)R24_ << 8));
  }
}

void BlockFunc_A0F4(uint16 a) {  // 0x94A0F4
  int16 v4;

  uint16 RegWord = Mult8x8(a, room_width_in_blocks);
  uint16 v3 = 2 * (RegWord + HIBYTE(a));
  v4 = R24_ - R22_;
  do {
    BlockBombedReact(v3);
    v3 += 2;
    --v4;
  } while (v4 >= 0);
}


void BlockFunc_A11A(uint16 a) {  // 0x94A11A
  int16 v4;

  uint16 RegWord = Mult8x8(a, room_width_in_blocks);
  uint16 v3 = 2 * (RegWord + HIBYTE(a));
  v4 = R28_ - R26_;
  do {
    BlockBombedReact(v3);
    v3 += room_width_in_blocks * 2;
    --v4;
  } while (v4 >= 0);
}


uint8 BlockShotReactHoriz_Slope(void) {  // 0x94A147
  if ((BTS[cur_block_index] & 0x1Fu) < 5)
    return BlockShotReactVert_Slope_Square(BTS[cur_block_index] & 0x1F, cur_block_index);
  else
    return BlockShotReactVert_Slope_NonSquare();
}

uint8 SetCarry_1(void) {  // 0x94A15C
  return 1;
}

uint8 BlockShotReactVert_Slope(void) {  // 0x94A15E
  if ((BTS[cur_block_index] & 0x1Fu) < 5)
    return BlockShotReactHoriz_Slope_Square(BTS[cur_block_index] & 0x1F, cur_block_index);
  else
    return BlockShotReactHoriz_Slope_NonSquare();
}

uint8 SetCarry_2(void) {  // 0x94A173
  return 1;
}

uint8 BlockShotReactHoriz(uint16 k) {  // 0x94A1B5
  static Func_U8 *const kBlockShotReactHoriz[16] = {
    ClearCarry_9,
    BlockShotReactHoriz_Slope,
    ClearCarry_9,
    ClearCarry_9,
    BlockReact_ShootableAir,
    BlockReact_HorizExt,
    ClearCarry_9,
    BlockReact_BombableAir,
    SetCarry_0,
    SetCarry_0,
    SetCarry_0,
    SetCarry_0,
    BlockReact_Shootable,
    BlockReact_VertExt,
    SetCarry_0,
    BlockReact_BombableBlock,
  };

  uint8 v1 = k >= room_size_in_blocks;
  if (k < room_size_in_blocks) {
    cur_block_index = k >> 1;
    do {
      v1 = kBlockShotReactHoriz[(level_data[cur_block_index] & 0xF000) >> 12]();
    } while (v1 & 0x80);
    if (v1)
      --R40;
  }
  return v1;
}

uint8 BlockShotReactVert(uint16 k) {  // 0x94A1D6
  static Func_U8 *const kBlockShotReactVert[16] = {
  ClearCarry_9,
  BlockShotReactVert_Slope,
  ClearCarry_9,
  ClearCarry_9,
  BlockReact_ShootableAir,
  BlockReact_HorizExt,
  ClearCarry_9,
  BlockReact_BombableAir,
  SetCarry_0,
  SetCarry_0,
  SetCarry_0,
  SetCarry_0,
  BlockReact_Shootable,
  BlockReact_VertExt,
  SetCarry_0,
  BlockReact_BombableBlock,
  };

  uint8 v1 = k >= room_size_in_blocks;
  if (k < room_size_in_blocks) {
    cur_block_index = k >> 1;
    do {
      v1 = kBlockShotReactVert[(level_data[cur_block_index] & 0xF000) >> 12]();
    } while (v1 & 0x80);
    if (v1)
      --R40;
  }
  return v1;
}

void BlockGetSomePos1(uint16 k) {  // 0x94A1F7
  int v1 = k >> 1;
  R38 = (projectile_y_pos[v1] - projectile_y_radius[v1]) & 0xFFF0;
  R38 = (uint16)(projectile_y_radius[v1] + projectile_y_pos[v1] - 1 - R38) >> 4;
  R26_ = R38;
  R40 = R38;
}

void BlockGetSomePos2(uint16 k) {  // 0x94A219
  int v1 = k >> 1;
  R38 = (projectile_x_pos[v1] - projectile_x_radius[v1]) & 0xFFF0;
  R38 = (uint16)(projectile_x_radius[v1] + projectile_x_pos[v1] - 1 - R38) >> 4;
  R26_ = R38;
  R40 = R38;
}

uint8 BlockCollNoWaveBeamHoriz(uint16 k) {  // 0x94A23B
  int16 v2;

  g_word_7E001E = 0;
  R18_ = 0;
  R20_ = 0;
  int v1 = k >> 1;
  v2 = projectile_bomb_x_speed[v1];
  if (v2 < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = v2;
  BlockGetSomePos1(k);
  uint16 prod = Mult8x8((uint16)(projectile_y_pos[v1] - projectile_y_radius[v1]) >> 4, room_width_in_blocks);
  uint16 v3 = projectile_bomb_x_subpos[v1];
  bool v4 = __CFADD__uint16(R18_, v3);
  uint16 v5 = R18_ + v3;
  projectile_bomb_x_subpos[v1] = v5;
  R22_ = v5;
  uint16 v6 = R20_ + v4 + projectile_x_pos[v1], v7;
  projectile_x_pos[v1] = v6;
  R24_ = v6;
  if ((R20_ & 0x8000u) != 0)
    v7 = v6 - projectile_x_radius[v1];
  else
    v7 = projectile_x_radius[v1] + v6 - 1;
  R28_ = v7;
  uint16 v8 = 2 * (prod + (v7 >> 4));
  if (!sign16(R38 - 16) || (int16)(HIBYTE(R28_) - room_width_in_scrolls) >= 0)
    return 0;
  do {
    BlockShotReactHoriz(v8);
    v8 += room_width_in_blocks * 2;
    --R38;
  } while ((R38 & 0x8000u) == 0);
  if ((R40 & 0x8000u) == 0)
    return 0;
  KillProjectile(k);
  return 1;
}

uint8 BlockCollNoWaveBeamVert(uint16 k) {  // 0x94A2CA
  int16 v2;
  int16 v8;
  int16 v9;

  g_word_7E001E = 0;
  R18_ = 0;
  R20_ = 0;
  int v1 = k >> 1;
  v2 = projectile_bomb_y_speed[v1];
  if (v2 < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = v2;
  BlockGetSomePos2(k);
  uint16 v3 = projectile_bomb_y_subpos[v1];
  bool v4 = __CFADD__uint16(R18_, v3);
  uint16 v5 = R18_ + v3;
  projectile_bomb_y_subpos[v1] = v5;
  R22_ = v5;
  uint16 v6 = R20_ + v4 + projectile_y_pos[v1], v7;
  projectile_y_pos[v1] = v6;
  R24_ = v6;
  if ((R20_ & 0x8000u) != 0)
    v7 = v6 - projectile_y_radius[v1];
  else
    v7 = projectile_y_radius[v1] + v6 - 1;
  R28_ = v7;
  v8 = (uint16)(projectile_x_pos[v1] - projectile_x_radius[v1]) >> 4;
  v9 = 2 * (Mult8x8(v7 >> 4, room_width_in_blocks) + v8);
  if (!sign16(R38 - 16) || (int16)(HIBYTE(R28_) - room_height_in_scrolls) >= 0)
    return 0;
  do {
    BlockShotReactVert(v9);
    v9 += 2;
    --R38;
  } while ((R38 & 0x8000u) == 0);
  if ((R40 & 0x8000u) == 0)
    return 0;
  KillProjectile(k);
  return 1;
}

uint8 BlockCollWaveBeamHoriz(uint16 k) {  // 0x94A352
  int16 v2;
  int16 v8;
  char v9;
  int16 v10;

  g_word_7E001E = 0;
  R18_ = 0;
  R20_ = 0;
  int v1 = k >> 1;
  v2 = projectile_bomb_x_speed[v1];
  if (v2 < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = v2;
  BlockGetSomePos1(k);
  uint16 prod = Mult8x8((uint16)(projectile_y_pos[v1] - projectile_y_radius[v1]) >> 4, room_width_in_blocks);
  uint16 v3 = projectile_bomb_x_subpos[v1];
  bool v4 = __CFADD__uint16(R18_, v3);
  uint16 v5 = R18_ + v3;
  projectile_bomb_x_subpos[v1] = v5;
  R22_ = v5;
  uint16 v6 = R20_ + v4 + projectile_x_pos[v1], v7;
  projectile_x_pos[v1] = v6;
  R24_ = v6;
  if ((R20_ & 0x8000u) != 0)
    v7 = v6 - projectile_x_radius[v1];
  else
    v7 = projectile_x_radius[v1] + v6 - 1;
  R28_ = v7;
  v8 = 2 * (prod + (v7 >> 4));
  if (sign16(R38 - 16)) {
    v9 = HIBYTE(projectile_y_pos[v1]);
    if (v9 >= 0
        && (int16)((uint8)v9 - room_height_in_scrolls) < 0
        && (int16)(HIBYTE(R28_) - room_width_in_scrolls) < 0) {
      v10 = v8;
      do {
        BlockShotReactHoriz(v10);
        v10 += room_width_in_blocks * 2;
        --R38;
      } while ((R38 & 0x8000u) == 0);
    }
  }
  return 0;
}

uint8 BlockCollWaveBeamVert(uint16 k) {  // 0x94A3E4
  int16 v2;
  int16 v8;
  int16 v9;
  char v10;
  int16 v11;

  g_word_7E001E = 0;
  R18_ = 0;
  R20_ = 0;
  int v1 = k >> 1;
  v2 = projectile_bomb_y_speed[v1];
  if (v2 < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = v2;
  BlockGetSomePos2(k);
  uint16 v3 = projectile_bomb_y_subpos[v1];
  bool v4 = __CFADD__uint16(R18_, v3);
  uint16 v5 = R18_ + v3;
  projectile_bomb_y_subpos[v1] = v5;
  R22_ = v5;
  uint16 v6 = R20_ + v4 + projectile_y_pos[v1], v7;
  projectile_y_pos[v1] = v6;
  R24_ = v6;
  if ((R20_ & 0x8000u) != 0)
    v7 = v6 - projectile_y_radius[v1];
  else
    v7 = projectile_y_radius[v1] + v6 - 1;
  R28_ = v7;
  uint16 prod = Mult8x8(v7 >> 4, room_width_in_blocks);
  v8 = (uint16)(projectile_x_pos[v1] - projectile_x_radius[v1]) >> 4;
  v9 = 2 * (prod + v8);
  if (sign16(R38 - 16)) {
    v10 = HIBYTE(projectile_x_pos[v1]);
    if (v10 >= 0
        && (int16)((uint8)v10 - room_width_in_scrolls) < 0
        && (int16)(HIBYTE(R28_) - room_height_in_scrolls) < 0) {
      v11 = v9;
      do {
        BlockShotReactVert(v11);
        v11 += 2;
        --R38;
      } while ((R38 & 0x8000u) == 0);
    }
  }
  return 0;
}

uint8 BlockCollMissileHoriz(uint16 k) {  // 0x94A46F
  int16 v2;

  g_word_7E001E = 1;
  R38 = 0;
  R26_ = 0;
  R18_ = 0;
  R20_ = 0;
  int v1 = k >> 1;
  v2 = projectile_bomb_x_speed[v1];
  if (v2 < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = v2;
  uint16 prod = Mult8x8(projectile_y_pos[v1] >> 4, room_width_in_blocks);
  uint16 v3 = projectile_bomb_x_subpos[v1];
  bool v4 = __CFADD__uint16(R18_, v3);
  uint16 v5 = R18_ + v3;
  projectile_bomb_x_subpos[v1] = v5;
  R22_ = v5;
  uint16 v6 = R20_ + v4 + projectile_x_pos[v1];
  projectile_x_pos[v1] = v6;
  R24_ = v6;
  R28_ = v6;
  uint16 v7 = 2 * (prod + (v6 >> 4));
  if ((int16)(HIBYTE(R24_) - room_width_in_scrolls) >= 0 || !(BlockShotReactHoriz(v7) & 1))
    return 0;
  KillProjectile(k);
  return 1;
}

uint8 BlockCollMissileVert(uint16 k) {  // 0x94A4D9
  int16 v2;
  int16 v7;

  g_word_7E001E = 1;
  R38 = 0;
  R26_ = 0;
  R18_ = 0;
  R20_ = 0;
  int v1 = k >> 1;
  v2 = projectile_bomb_y_speed[v1];
  if (v2 < 0)
    --R20_;
  *(uint16 *)((char *)&R18_ + 1) = v2;
  uint16 v3 = projectile_bomb_y_subpos[v1];
  bool v4 = __CFADD__uint16(R18_, v3);
  uint16 v5 = R18_ + v3;
  projectile_bomb_y_subpos[v1] = v5;
  R22_ = v5;
  uint16 v6 = R20_ + v4 + projectile_y_pos[v1];
  projectile_y_pos[v1] = v6;
  R24_ = v6;
  R28_ = v6;
  uint16 prod = Mult8x8(v6 >> 4, room_width_in_blocks);
  v7 = projectile_x_pos[v1] >> 4;
  uint16 v8 = 2 * (prod + v7);
  if ((int16)(HIBYTE(R24_) - room_height_in_scrolls) >= 0 || !(BlockShotReactVert(v8) & 1))
    return 0;
  KillProjectile(k);
  return 1;
}

uint8 BlockShotReactVert_Slope_NonSquare(void) {  // 0x94A543
  int16 v2;

  uint16 v0 = cur_block_index;
  uint16 v1 = projectile_index;
  uint16 div = SnesDivide(cur_block_index, room_width_in_blocks);
  v2 = projectile_y_pos[v1 >> 1] >> 4;
  if (v2 == div)
    return BlockShotReact_Slope_NonSquare(v1, v0) & 1;
  else
    return 0;
}

uint8 BlockShotReactHoriz_Slope_NonSquare(void) {  // 0x94A569
  int16 v2;

  uint16 v0 = cur_block_index;
  uint16 v1 = projectile_index;
  uint16 mod = SnesModulus(cur_block_index, room_width_in_blocks);
  v2 = projectile_x_pos[v1 >> 1] >> 4;
  if (v2 == mod)
    return BlockShotReact_Slope_NonSquare(v1, v0) & 1;
  else
    return 0;
}

uint8 BlockShotReact_Slope_NonSquare(uint16 j, uint16 k) {  // 0x94A58F
  int16 v3;
  int16 v5;
  uint16 v2;

  temp_collision_DD6 = 16 * (BTS[k] & 0x1F);
  if ((BTS[k] & 0x40) != 0)
    v2 = projectile_x_pos[j >> 1] ^ 0xF;
  else
    v2 = projectile_x_pos[j >> 1];
  uint16 v6 = temp_collision_DD6 + (v2 & 0xF);
  if (!(BTS[k] & 0x80)) {
    temp_collision_DD4 = projectile_y_pos[j >> 1] & 0xF;
    v5 = kAlignYPos_Tab0[v6] & 0x1F;
    if ((int16)(v5 - temp_collision_DD4) < 0 || v5 == temp_collision_DD4) {
      R38 = 0;
      R40 = 0;
      return 1;
    } else {
      return 0;
    }
  } else {
    temp_collision_DD4 = projectile_y_pos[j >> 1] & 0xF ^ 0xF;
    v3 = kAlignYPos_Tab0[v6] & 0x1F;
    if ((int16)(v3 - temp_collision_DD4) < 0 || v3 == temp_collision_DD4) {
      R38 = 0;
      R40 = 0;
      return 1;
    } else {
      return 0;
    }
  }
}

uint8 BlockCollSpreadBomb(uint16 k) {  // 0x94A621
  static Func_U8 *const kBlockCollSpreadBomb[16] = {
  ClearCarry_9,
  sub_949D5D,
  ClearCarry_9,
  ClearCarry_9,
  SetCarry_0,
  BlockReact_HorizExt,
  ClearCarry_9,
  ClearCarry_9,
  SetCarry_0,
  SetCarry_0,
  SetCarry_0,
  SetCarry_0,
  SetCarry_0,
  BlockReact_VertExt,
  SetCarry_0,
  SetCarry_0,
  };


  int v1 = k >> 1;
  R26_ = projectile_x_pos[v1];
  R28_ = projectile_y_pos[v1];
  g_word_7E001E = 0;
  R32 = 0;
  cur_block_index = 0;
  CalculateBlockAt();
  if (!projectile_variables[v1]) {
    BlockColl_BombExplosion(k);
    return 0;
  }
  if (cur_block_index == 0xFFFF)
    return 1;
  uint8 rv;
  do {
    rv = kBlockCollSpreadBomb[(level_data[cur_block_index] & 0xF000) >> 12]();
  } while (rv & 0x80);
  return rv;
}

uint8 BlockShotReactVert_Slope_Square(uint16 a, uint16 k) {  // 0x94A66A
  temp_collision_DD4 = 4 * a;
  temp_collision_DD6 = BTS[k] >> 6;
  uint16 v2 = 4 * a + (temp_collision_DD6 ^ ((uint8)(R28_ & 8) >> 3));
  if (!g_word_7E001E) {
    if (!R26_) {
      int v3 = projectile_index >> 1;
      if (((projectile_y_pos[v3] - projectile_y_radius[v3]) & 8) != 0
          || !kTab948E54[v2]) {
        uint16 v4 = v2 ^ 2;
        if (((LOBYTE(projectile_y_radius[v3]) + LOBYTE(projectile_y_pos[v3]) - 1) & 8) == 0
            || !kTab948E54[v4]) {
          return 0;
        }
      }
      return 1;
    }
    if (R38) {
      if (R38 == R26_
          && ((projectile_y_pos[projectile_index >> 1] - projectile_y_radius[projectile_index >> 1]) & 8) != 0) {
        return kTab948E54[v2 ^ 2] != 0;
      }
    } else if (((LOBYTE(projectile_y_radius[projectile_index >> 1]) + LOBYTE(projectile_y_pos[projectile_index >> 1]) - 1) & 8) == 0) {
      return kTab948E54[v2] != 0;
    }
    if (kTab948E54[v2] != 0)
      return 1;
    return kTab948E54[v2 ^ 2] != 0;
  }
  if ((projectile_y_pos[projectile_index >> 1] & 8) != 0)
    v2 ^= 2u;
  return kTab948E54[v2] != 0;
}


uint8 BlockShotReactHoriz_Slope_Square(uint16 a, uint16 k) {  // 0x94A71A
  temp_collision_DD4 = 4 * a;
  temp_collision_DD6 = BTS[k] >> 6;
  uint16 v2 = 4 * a + (temp_collision_DD6 ^ ((uint8)(R28_ & 8) >> 2));
  if (!g_word_7E001E) {
    if (!R26_) {
      int v3 = projectile_index >> 1;
      if (((projectile_x_pos[v3] - projectile_x_radius[v3]) & 8) != 0
          || kTab948E54[v2] == 0) {
        uint16 v4 = v2 ^ 1;
        if (((LOBYTE(projectile_x_radius[v3]) + LOBYTE(projectile_x_pos[v3]) - 1) & 8) == 0
            || kTab948E54[v4] == 0) {
          return 0;
        }
      }
      return 1;
    }
    if (R38) {
      if (R38 == R26_
          && ((projectile_x_pos[projectile_index >> 1] - projectile_x_radius[projectile_index >> 1]) & 8) != 0) {
        return kTab948E54[v2 ^ 1] != 0;
      }
    } else if (((LOBYTE(projectile_x_radius[projectile_index >> 1]) + LOBYTE(projectile_x_pos[projectile_index >> 1]) - 1) & 8) == 0) {
      return kTab948E54[v2] != 0;
    }
    if (kTab948E54[v2] != 0)
      return 1;
    return kTab948E54[v2 ^ 1] != 0;
  }
  if ((projectile_x_pos[projectile_index >> 1] & 8) != 0)
    v2 ^= 1u;
  return kTab948E54[v2] != 0;
}


uint8 ClearCarryZero(void) {  // 0x94A7C9
  return 0;
}

uint8 ClearCarrySetZero(void) {  // 0x94A7CD
  return 1;
}

uint8 BlockReactGrapple_GrappleBlock(void) {  // 0x94A7D1
  static const uint16 kBlockReactGrapple_GrappleBlockPlm[4] = {
    0xd0d8,
    0xd0dc,
    0xd0e0,
    0xd0d8,
  };

  char v0;

  grapple_beam_flags &= ~0x8000u;
  v0 = BTS[cur_block_index];
  if (v0 < 0)
    return 0;
  else
    return SpawnPLM(kBlockReactGrapple_GrappleBlockPlm[v0 & 0x7F]);
}

uint8 BlockReactGrapple_SpikeBlock(void) {  // 0x94A7FD
  static const uint16 kBlockReactGrapple_SpikeBlockPlm[16] = {
  0xd0e4,
  0xd0e4,
  0xd0e4,
  0xd0e8,
  0xd0e4,
  0xd0e4,
  0xd0e4,
  0xd0e4,
  0xd0e4,
  0xd0e4,
  0xd0e4,
  0xd0e4,
  0xd0e4,
  0xd0e4,
  0xd0e4,
  0xd0e4,
  };
  char v0;

  v0 = BTS[cur_block_index];
  if (v0 < 0)
    return 0;
  else
    return SpawnPLM(kBlockReactGrapple_SpikeBlockPlm[v0 & 0x7F]);
}

uint8 BlockCollGrappleBeam(void) {  // 0x94A85B
  uint8 result;

  *(uint16 *)((char *)&grapple_beam_tmpD82 + 1) = grapple_beam_extension_x_velocity;
  *(uint32 *)&grapple_beam_tmpD82 >>= 2;
  if ((grapple_beam_extension_x_velocity & 0x8000u) != 0)
    grapple_beam_tmpD84 |= 0xFFC0u;
  *(uint16 *)((char *)&grapple_beam_y_quarter_subvel + 1) = grapple_beam_extension_y_velocity;

  *(uint32 *)&grapple_beam_y_quarter_subvel >>= 2;
  if ((grapple_beam_extension_y_velocity & 0x8000u) != 0)
    grapple_beam_y_quarter_vel |= 0xFFC0u;
  grapple_beam_tmpD8A = 4;
  while (1) {
    uint16 v0 = (__PAIR32__(grapple_beam_tmpD84, grapple_beam_tmpD82) + __PAIR32__(grapple_beam_end_x_offset, grapple_beam_end_x_suboffset)) >> 16;
    grapple_beam_end_x_suboffset += grapple_beam_tmpD82;
    grapple_beam_end_x_offset = v0;
    uint16 v1 = (__PAIR32__(grapple_beam_y_quarter_vel, grapple_beam_y_quarter_subvel) + __PAIR32__(grapple_beam_end_y_offset, grapple_beam_end_y_suboffset)) >> 16;
    grapple_beam_end_y_suboffset += grapple_beam_y_quarter_subvel;
    grapple_beam_end_y_offset = v1;
    grapple_beam_end_x_subpos = grapple_beam_end_x_suboffset + samus_x_subpos;
    grapple_beam_end_x_pos = grapple_beam_origin_x_offset + ((__PAIR32__(grapple_beam_end_x_offset, grapple_beam_end_x_suboffset) +
                                                              __PAIR32__(samus_x_pos, samus_x_subpos)) >> 16);
    grapple_beam_end_y_subpos = grapple_beam_end_y_suboffset + samus_y_subpos;
    grapple_beam_end_y_pos = grapple_beam_origin_y_offset + ((__PAIR32__(v1, grapple_beam_end_y_suboffset) + __PAIR32__(samus_y_pos, samus_y_subpos)) >> 16);
    result = BlockReactGrapple();
    if ((result & 0x40) != 0 && (result & 1) != 0)
      break;
    if (!--grapple_beam_tmpD8A)
      return result;
  }
  grapple_beam_end_x_pos = grapple_beam_end_x_pos & 0xFFF0 | 8;
  grapple_beam_end_y_pos = grapple_beam_end_y_pos & 0xFFF0 | 8;
  return result;
}

uint8 BlockReactGrapple(void) {  // 0x94A91F
  int16 v0;
  static Func_U8 *const kBlockReactGrapple[16] = {
    ClearCarryZero,
    ClearCarrySetZero,
    ClearCarryZero,
    ClearCarryZero,
    BlockReact_ShootableAir,
    BlockReact_HorizExt,
    ClearCarryZero,
    BlockReact_BombableAir,
    ClearCarrySetZero,
    ClearCarrySetZero,
    BlockReactGrapple_SpikeBlock,
    ClearCarrySetZero,
    BlockReact_Shootable,
    BlockReact_VertExt,
    BlockReactGrapple_GrappleBlock,
    BlockReact_BombableBlock,
  };
  uint16 prod = Mult8x8(grapple_beam_end_y_pos >> 4, room_width_in_blocks);
  v0 = grapple_beam_end_x_pos >> 4;
  cur_block_index = prod + v0;
  uint8 rv;
  do {
    rv = kBlockReactGrapple[(level_data[cur_block_index] & 0xF000) >> 12]();
  } while (rv & 0x80);
  return rv;
}

const int16 kSinCosTable8bit_Sext[320] = {  // 0x94A957
  -256, -255, -255, -255, -254, -254, -253, -252, -251, -249, -248, -246, -244, -243, -241, -238,
  -236, -234, -231, -228, -225, -222, -219, -216, -212, -209, -205, -201, -197, -193, -189, -185,
  -181, -176, -171, -167, -162, -157, -152, -147, -142, -136, -131, -126, -120, -115, -109, -103,
   -97,  -92,  -86,  -80,  -74,  -68,  -62,  -56,  -49,  -43,  -37,  -31,  -25,  -18,  -12,   -6,
     0,    6,   12,   18,   25,   31,   37,   43,   49,   56,   62,   68,   74,   80,   86,   92,
    97,  103,  109,  115,  120,  126,  131,  136,  142,  147,  152,  157,  162,  167,  171,  176,
   181,  185,  189,  193,  197,  201,  205,  209,  212,  216,  219,  222,  225,  228,  231,  234,
   236,  238,  241,  243,  244,  246,  248,  249,  251,  252,  253,  254,  254,  255,  255,  255,
   256,  255,  255,  255,  254,  254,  253,  252,  251,  249,  248,  246,  244,  243,  241,  238,
   236,  234,  231,  228,  225,  222,  219,  216,  212,  209,  205,  201,  197,  193,  189,  185,
   181,  176,  171,  167,  162,  157,  152,  147,  142,  136,  131,  126,  120,  115,  109,  103,
    97,   92,   86,   80,   74,   68,   62,   56,   49,   43,   37,   31,   25,   18,   12,    6,
     0,   -6,  -12,  -18,  -25,  -31,  -37,  -43,  -49,  -56,  -62,  -68,  -74,  -80,  -86,  -92,
   -97, -103, -109, -115, -120, -126, -131, -136, -142, -147, -152, -157, -162, -167, -171, -176,
  -181, -185, -189, -193, -197, -201, -205, -209, -212, -216, -219, -222, -225, -228, -231, -234,
  -236, -238, -241, -243, -244, -246, -248, -249, -251, -252, -253, -254, -254, -255, -255, -255,
  -256, -255, -255, -255, -254, -254, -253, -252, -251, -249, -248, -246, -244, -243, -241, -238,
  -236, -234, -231, -228, -225, -222, -219, -216, -212, -209, -205, -201, -197, -193, -189, -185,
  -181, -176, -171, -167, -162, -157, -152, -147, -142, -136, -131, -126, -120, -115, -109, -103,
   -97,  -92,  -86,  -80,  -74,  -68,  -62,  -56,  -49,  -43,  -37,  -31,  -25,  -18,  -12,   -6,
};

void BlockFunc_A957(void) {
  int16 v5;
  int16 v7;

  uint16 v0 = grapple_beam_tmpD82, v2, v3;
  if ((grapple_beam_flags & 0x8000u) == 0) {
    int v1 = grapple_beam_tmpD82 >> 1;
    if ((kSinCosTable8bit_Sext[v1 + 64] & 0x8000u) != 0)
      v2 = grapple_beam_end_x_pos & 0xFFF0 | 7;
    else
      v2 = grapple_beam_end_x_pos & 0xFFF0 | 8;
    grapple_beam_end_x_pos = v2;
    if ((kSinCosTable8bit_Sext[v1] & 0x8000u) != 0)
      v3 = grapple_beam_end_y_pos & 0xFFF0 | 7;
    else
      v3 = grapple_beam_end_y_pos & 0xFFF0 | 8;
    grapple_beam_end_y_pos = v3;
  }
  WriteReg(WRMPYA, grapple_beam_tmpD84);
  int v4 = v0 >> 1;
  v5 = kSinCosTable8bit_Sext[v4 + 64];
  uint16 v6, v8;
  if (v5 < 0) {
    if (v5 == -256) {
      v6 = grapple_beam_end_x_pos - grapple_beam_tmpD84;
    } else {
      uint16 prod = Mult8x8(grapple_beam_tmpD84, -(int8)v5);
      v6 = grapple_beam_end_x_pos - (prod >> 8);
    }
  } else if (v5 == 256) {
    v6 = grapple_beam_tmpD84 + grapple_beam_end_x_pos;
  } else {
    uint16 prod = Mult8x8(grapple_beam_tmpD84, kSinCosTable8bit_Sext[v4 + 64]);
    v6 = grapple_beam_end_x_pos + (prod >> 8);
  }
  grapple_beam_grapple_start_x = v6;
  grapple_beam_grapple_start_block_x = (uint8)(v6 >> 4);
  v7 = kSinCosTable8bit_Sext[v4];
  if (v7 < 0) {
    if (v7 == -256) {
      v8 = grapple_beam_end_y_pos - grapple_beam_tmpD84;
    } else {
      uint16 prod = Mult8x8(grapple_beam_tmpD84, -v7);
      v8 = grapple_beam_end_y_pos - (prod >> 8);
    }
  } else if (v7 == 256) {
    v8 = grapple_beam_tmpD84 + grapple_beam_end_y_pos;
  } else {
    uint16 prod = Mult8x8(grapple_beam_tmpD84, kSinCosTable8bit_Sext[v4]);
    v8 = grapple_beam_end_y_pos + (prod >> 8);
  }
  grapple_beam_grapple_start_y = v8;
  grapple_beam_grapple_start_block_y = (uint8)(v8 >> 4);
}

uint8 BlockReact_AA64(void) {  // 0x94AA64
  static Func_U8 *const kBlockReactLut_AB90[16] = {
  ClearCarry_10,
  SetCarry_3,
  BlockReact_AA64_SpikeAir,
  ClearCarry_10,
  ClearCarry_10,
  BlockReact_HorizExt,
  ClearCarry_10,
  ClearCarry_10,
  SetCarry_3,
  SetCarry_3,
  BlockReact_AA64_SpikeBlock,
  SetCarry_3,
  SetCarry_3,
  BlockReact_VertExt,
  SetCarry_3,
  SetCarry_3,
  };

  uint16 v0 = grapple_beam_grapple_start_block_x;
  uint16 RegWord = Mult8x8(grapple_beam_grapple_start_block_y, room_width_in_blocks);
  cur_block_index = RegWord + v0;
  uint8 rv;
  do {
    rv = kBlockReactLut_AB90[(level_data[cur_block_index] & 0xF000) >> 12]();
  } while (rv & 0x80);
  return rv;
}

uint8 ClearCarry_10(void) {  // 0x94AA9A
  return 0;
}

uint8 SetCarry_3(void) {  // 0x94AA9C
  return 1;
}

static const uint16 g_word_94AAD7[16] = {  // 0x94AA9E
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
};
static const uint16 g_word_94AAF7[16] = {
  0, 0, 0x10, 0,
  0, 0,    0, 0,
  0, 0,    0, 0,
  0, 0,    0, 0,
};

uint8 BlockReact_AA64_SpikeAir(void) {
  int16 v0;
  int16 v2;

  if (!samus_invincibility_timer) {
    v0 = *(uint16 *)&BTS[cur_block_index];
    if (v0 >= 0) {
      int v1 = v0;
      if (g_word_94AAF7[v1] | g_word_94AAD7[v1]) {
        v2 = (g_word_94AAD7[v1] + __PAIR32__(samus_periodic_damage, samus_periodic_subdamage)) >> 16;
        samus_periodic_subdamage += g_word_94AAD7[v1];
        samus_periodic_damage = g_word_94AAF7[v1] + v2;
        samus_invincibility_timer = 60;
        samus_knockback_timer = 10;
      }
    }
  }
  return 0;
}

static const uint16 g_word_94AB50[16] = {  // 0x94AB17
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
};
static const uint16 g_word_94AB70[16] = {
  60, 16, 0, 0,
   0,  0, 0, 0,
   0,  0, 0, 0,
   0,  0, 0, 0,
};

uint8 BlockReact_AA64_SpikeBlock(void) {
  int16 v0;
  int16 v2;

  if (!samus_invincibility_timer) {
    v0 = *(uint16 *)&BTS[cur_block_index];
    if (v0 >= 0) {
      int v1 = v0;
      if (g_word_94AB70[v1] | g_word_94AB50[v1]) {
        v2 = (g_word_94AB50[v1] + __PAIR32__(samus_periodic_damage, samus_periodic_subdamage)) >> 16;
        samus_periodic_subdamage += g_word_94AB50[v1];
        samus_periodic_damage = g_word_94AB70[v1] + v2;
        samus_invincibility_timer = 60;
        samus_knockback_timer = 10;
      }
    }
  }
  return 1;
}

static Func_U8 *const kBlockReactLut_AB90[16] = {  // 0x94ABB0
  ClearCarry_10,
  SetCarry_3,
  BlockReact_AA64_SpikeAir,
  ClearCarry_10,
  ClearCarry_10,
  BlockReact_HorizExt,
  ClearCarry_10,
  ClearCarry_10,
  SetCarry_3,
  SetCarry_3,
  BlockReact_AA64_SpikeBlock,
  SetCarry_3,
  SetCarry_3,
  BlockReact_VertExt,
  SetCarry_3,
  SetCarry_3,
};

uint8 BlockFunc_ABB0(void) {
  uint8 rv;
  uint16 v0 = grapple_beam_grapple_start_block_x;
  uint16 RegWord = Mult8x8(grapple_beam_grapple_start_block_y, room_width_in_blocks);
  cur_block_index = RegWord + v0;
  do {
    rv = kBlockReactLut_AB90[(level_data[cur_block_index] & 0xF000) >> 12]();
  } while (rv & 0x80);
  return rv;
}

uint8 BlockFunc_ABE6(void) {  // 0x94ABE6
  uint8 v0;
  g_word_7E0D98 = 6;
  grapple_beam_tmpD84 = grapple_beam_length + 8;
  while (1) {
    BlockFunc_A957();
    v0 = BlockFunc_ABB0() & 1;
    if (v0)
      break;
    grapple_beam_tmpD84 += 8;
    if (!--g_word_7E0D98)
      return 0;
  }
  return v0;
}

void BlockFunc_AC11(void) {  // 0x94AC11
  grapple_beam_tmpD82 = 2 * grapple_beam_end_angle;
  grapple_beam_tmpD84 = grapple_beam_length;
  BlockFunc_A957();
  x_pos_of_start_of_grapple_beam = grapple_beam_grapple_start_x;
  y_pos_of_start_of_grapple_beam = grapple_beam_grapple_start_y;
}

uint8 BlockFunc_AC31(void) {  // 0x94AC31
  if (!grapple_beam_length_delta)
    return 0;
  if ((grapple_beam_length_delta & 0x8000u) != 0) {
    uint16 v0 = grapple_beam_length_delta + grapple_beam_length;
    if ((uint16)(grapple_beam_length_delta + grapple_beam_length) < 8u) {
      grapple_beam_length_delta = 0;
      v0 = 8;
    }
    grapple_beam_varD8C = v0;
    grapple_beam_varD8E = 8;
    grapple_beam_tmpD82 = 2 * grapple_beam_end_angle;
    uint16 v1 = grapple_beam_length;
    if (grapple_beam_length == grapple_beam_varD8C) {
LABEL_8:
      grapple_beam_length = grapple_beam_varD8C;
      return 0;
    }
    while (1) {
      grapple_beam_tmpD8A = v1;
      grapple_beam_tmpD84 = grapple_beam_varD8E + v1 - 1;
      BlockFunc_A957();
      if (BlockReact_AA64() & 1)
        goto LABEL_9;
      v1 = --grapple_beam_tmpD8A;
      if (grapple_beam_tmpD8A == grapple_beam_varD8C)
        goto LABEL_8;
    }
  }
  uint16 v3;
  v3 = grapple_beam_length_delta + grapple_beam_length;
  if ((uint16)(grapple_beam_length_delta + grapple_beam_length) >= 0x3Fu) {
    grapple_beam_length_delta = 0;
    v3 = 63;
  }
  grapple_beam_varD8C = v3;
  grapple_beam_varD8E = 56;
  grapple_beam_tmpD82 = 2 * grapple_beam_end_angle;
  uint16 v4;
  v4 = grapple_beam_length;
  if (grapple_beam_length != grapple_beam_varD8C) {
    while (1) {
      grapple_beam_tmpD8A = v4;
      grapple_beam_tmpD84 = grapple_beam_varD8E + v4 + 1;
      BlockFunc_A957();
      if (BlockReact_AA64() & 1)
        break;
      v4 = ++grapple_beam_tmpD8A;
      if (grapple_beam_tmpD8A == grapple_beam_varD8C)
        goto LABEL_15;
    }
LABEL_9:
    grapple_beam_length = grapple_beam_tmpD8A;
    return 1;
  }
LABEL_15:
  grapple_beam_length = grapple_beam_varD8C;
  return 0;
}

uint8 HandleMovementAndCollForSamusGrapple(void) {  // 0x94ACFE
  int16 v4;
  int16 v7;
  uint16 v3, v6;

  uint16 v0 = 256;
  if ((grapple_beam_flags & 1) != 0)
    v0 = 160;
  uint16 v1 = grapple_beam_unkD2E + grapple_beam_unkD26;
  if ((int16)(grapple_beam_unkD2E + grapple_beam_unkD26) >= 0) {
    Multiply16x16(v1, v0);
    if (!*(uint16 *)((char *)&mult_product_lo + 1))
      return 0;
    g_word_7E0D9C = *(uint16 *)((char *)&mult_product_lo + 1);
    grapple_beam_y_quarter_vel = 2
      * ((uint16)(*(uint16 *)&grapple_beam_end_subangle
                  + *(uint16 *)((char *)&mult_product_lo + 1)) >> 8);
    grapple_beam_tmpD84 = grapple_beam_length;
    uint16 v2;
    v2 = 2 * grapple_beam_end_angle;
    if (v2 != grapple_beam_y_quarter_vel) {
      while (1) {
        grapple_beam_y_quarter_subvel = v2;
        grapple_beam_tmpD82 = (v2 + 2) & 0x1FF;
        if (BlockFunc_ABE6() & 1)
          break;
        v2 = (grapple_beam_y_quarter_subvel + 2) & 0x1FF;
        if (v2 == grapple_beam_y_quarter_vel)
          goto LABEL_12;
      }
      LOBYTE(v4) = 0;
      HIBYTE(v4) = grapple_beam_y_quarter_subvel >> 1;
      *(uint16 *)&grapple_beam_end_subangle = v4 | 0x80;
      grapple_beam_end_angles_mirror = v4 | 0x80;
      if (g_word_7E0D98 != 6 && g_word_7E0D98 != 5 || grapple_beam_length != 8)
        goto LABEL_39;
LABEL_23:
      grapple_beam_unkD36 |= 0x8000u;
      grapple_beam_unkD26 = 0;
      grapple_beam_unkD2E = 0;
      return 1;
    }
LABEL_12:
    *(uint16 *)&grapple_beam_end_subangle += g_word_7E0D9C;
    grapple_beam_end_angles_mirror = *(uint16 *)&grapple_beam_end_subangle;
    grapple_beam_unkD36 &= ~0x8000u;
    if ((--grapple_beam_unkD30 & 0x8000u) != 0)
      grapple_beam_unkD30 = 0;
    if ((grapple_beam_unkD2E & 0x8000u) == 0) {
      v3 = grapple_beam_unkD2E - 6;
      if ((int16)(grapple_beam_unkD2E - 6) >= 0) {
LABEL_19:
        grapple_beam_unkD2E = v3;
        return 0;
      }
    } else {
      v3 = grapple_beam_unkD2E + 6;
      if ((int16)(grapple_beam_unkD2E + 6) < 0)
        goto LABEL_19;
    }
    v3 = 0;
    goto LABEL_19;
  }
  Multiply16x16(-v1, v0);
  if (!*(uint16 *)((char *)&mult_product_lo + 1))
    return 0;
  g_word_7E0D9C = -*(uint16 *)((char *)&mult_product_lo + 1);
  grapple_beam_y_quarter_vel = 2
    * ((uint16)(*(uint16 *)&grapple_beam_end_subangle
                - *(uint16 *)((char *)&mult_product_lo + 1)) >> 8);
  grapple_beam_tmpD84 = grapple_beam_length;
  uint16 v5;
  v5 = 2 * grapple_beam_end_angle;
  if (v5 == grapple_beam_y_quarter_vel) {
LABEL_28:
    *(uint16 *)&grapple_beam_end_subangle += g_word_7E0D9C;
    grapple_beam_end_angles_mirror = *(uint16 *)&grapple_beam_end_subangle;
    grapple_beam_unkD36 &= ~0x8000u;
    if ((--grapple_beam_unkD30 & 0x8000u) != 0)
      grapple_beam_unkD30 = 0;
    if ((grapple_beam_unkD2E & 0x8000u) == 0) {
      v6 = grapple_beam_unkD2E - 6;
      if ((int16)(grapple_beam_unkD2E - 6) >= 0) {
LABEL_35:
        grapple_beam_unkD2E = v6;
        return 0;
      }
    } else {
      v6 = grapple_beam_unkD2E + 6;
      if ((int16)(grapple_beam_unkD2E + 6) < 0)
        goto LABEL_35;
    }
    v6 = 0;
    goto LABEL_35;
  }
  while (1) {
    grapple_beam_y_quarter_subvel = v5;
    grapple_beam_tmpD82 = (v5 - 2) & 0x1FF;
    if (BlockFunc_ABE6() & 1)
      break;
    v5 = (grapple_beam_y_quarter_subvel - 2) & 0x1FF;
    if (v5 == grapple_beam_y_quarter_vel)
      goto LABEL_28;
  }
  LOBYTE(v7) = 0;
  HIBYTE(v7) = grapple_beam_y_quarter_subvel >> 1;
  *(uint16 *)&grapple_beam_end_subangle = v7 | 0x80;
  grapple_beam_end_angles_mirror = v7 | 0x80;
  if ((g_word_7E0D98 == 6 || g_word_7E0D98 == 5) && grapple_beam_length == 8)
    goto LABEL_23;
LABEL_39:
  grapple_beam_unkD30 = 16;
  grapple_beam_unkD26 = -((int16)grapple_beam_unkD26 >> 1);
  grapple_beam_unkD2E = -((int16)grapple_beam_unkD2E >> 1);
  return 1;
}

uint8 BlockFunc_AEE3(void) {  // 0x94AEE3
  if (((grapple_beam_unkD26 ^ *(uint16 *)&grapple_beam_end_subangle) & 0x8000u) != 0) {
    grapple_beam_unkD38 = 0;
    return 1;
  } else {
    if (++grapple_beam_unkD38 == 32)
      grapple_beam_function = FUNC16(GrappleBeam_Func2);
    grapple_beam_unkD26 = 0;
    grapple_beam_unkD2E = 0;
    return 1;
  }
}

uint8 ClearCarry_11(void) {  // 0x94AF0B
  return 0;
}

void GrappleFunc_AF87(void) {  // 0x94AF87
  for (int i = 30; i >= 0; i -= 8) {
    int v1 = i >> 1;
    grapple_segment_anim_instr_ptrs[v1] = addr_word_94B197;
    grapple_segment_anim_instr_timers[v1 + 15] = addr_word_94B193;
    grapple_segment_anim_instr_timers[v1 + 14] = addr_word_94B18F;
    grapple_segment_anim_instr_timers[v1 + 13] = addr_word_94B18B;
    grapple_segment_anim_instr_timers[v1] = 1;
    *(uint16 *)((char *)&grapple_point_anim_ptr + i) = 1;
    *(uint16 *)((char *)&grapple_point_anim_timer + i) = 1;
    *(uint16 *)((char *)&grapple_beam_unkD3C + i) = 1;
  }
}

uint16 CallGrappleInstr(uint32 ea, uint16 j) {
  switch (ea) {
  case fnGrappleInstr_Goto: return GrappleInstr_Goto(j);  // 0x94b0f4
  default: return Unreachable();
  }
}

void HandleGrappleBeamGfx(void) {  // 0x94AFBA
  int16 RegWord; // dx

  R18_ = grapple_beam_end_x_pos - x_pos_of_start_of_grapple_beam_prevframe;
  R20_ = grapple_beam_end_y_pos - y_pos_of_start_of_grapple_beam_prevframe;
  uint16 v0 = 2 * CalculateAngleFromXY();
  R26_ = 0;
  R28_ = 0;
  int v1 = v0 >> 1;
  uint16 v2 = 8 * kSinCosTable8bit_Sext[v1 + 64];
  if ((kSinCosTable8bit_Sext[v1 + 64] & 0x1000) != 0)
    --R28_;
  *(uint16 *)((char *)&R26_ + 1) = v2;
  g_word_7E001E = 0;
  R32 = 0;
  uint16 v3 = 8 * kSinCosTable8bit_Sext[v1];
  if ((kSinCosTable8bit_Sext[v1] & 0x1000) != 0)
    --R32;
  *(uint16 *)((char *)&g_word_7E001E + 1) = v3;
  R38 = (uint16)(*(uint16 *)&grapple_beam_end_subangle & 0x8000) >> 1;
  R38 |= 2 * ((*(uint16 *)&grapple_beam_end_subangle ^ R38) & 0x4000 ^ 0x4000);
  R20_ = x_pos_of_start_of_grapple_beam_prevframe - layer1_x_pos - 4;
  R18_ = 0;
  R24_ = y_pos_of_start_of_grapple_beam_prevframe - layer1_y_pos - 4;
  R22_ = 0;
  if ((grapple_beam_length & 0x8000u) == 0) {
    RegWord = grapple_beam_length / 8;
    if (RegWord >= 0) {
      R40 = (RegWord & 0xF) - 1;
      uint16 v5 = 30;
      do {
        int i, v6 = v5 >> 1;
        uint16 v10;
        if (grapple_segment_anim_instr_timers[v6]-- == 1) {
          for (i = grapple_segment_anim_instr_ptrs[v6]; ; ) {
            uint16 *v9 = (uint16 *)RomPtr_94(i);
            v10 = *v9;
            if ((*v9 & 0x8000u) == 0)
              break;
            R36 = *v9;
            i = CallGrappleInstr(v10 | 0x940000, i + 2);
          }
          int v11 = v5 >> 1;
          grapple_segment_anim_instr_timers[v11] = v10;
          grapple_segment_anim_instr_ptrs[v11] = i + 4;
        }
        if (((R24_ | R20_) & 0xFF00) != 0)
          break;
        uint16 v12 = *(uint16 *)RomPtr_94(grapple_segment_anim_instr_ptrs[v5 >> 1] - 2);
        DrawGrappleOams(v12);
        v5 -= 2;
        --R40;
      } while ((R40 & 0x8000u) == 0);
      if (samus_pose == kPose_B2_FaceR_Grapple_Air || samus_pose == kPose_B3_FaceL_Grapple_Air)
        DrawGrappleOams3();
      else
        DrawGrappleOams2();
    }
  }
}

void DrawGrappleOams(uint16 j) {  // 0x94B0AA
  OamEnt *v2;
  unsigned int v3; // kr00_4
  unsigned int v4; // kr04_4

  uint16 v1 = oam_next_ptr;
  v2 = gOamEnt(oam_next_ptr);
  *(uint16 *)&v2->xcoord = R20_;
  v3 = __PAIR32__(R28_, R26_) + __PAIR32__(R20_, R18_);
  R20_ = HIWORD(v3);
  R18_ = v3;
  *(uint16 *)&v2->ycoord = R24_;
  v4 = __PAIR32__(R32, g_word_7E001E) + __PAIR32__(R24_, R22_);
  R24_ = HIWORD(v4);
  R22_ = v4;
  *(uint16 *)&v2->charnum = R38 | j;
  oam_next_ptr = v1 + 4;
}

uint16 GrappleInstr_Goto(uint16 j) {  // 0x94B0F4
  return *(uint16 *)RomPtr_94(j);
}


void DrawGrappleOams2(void) {  // 0x94B0F9
  int16 v1;
  OamEnt *v2;
  int16 v4;

  if (((grapple_beam_end_y_pos - layer1_y_pos) & 0xFF00) == 0) {
    uint16 v0 = oam_next_ptr;
    v1 = grapple_beam_end_x_pos - layer1_x_pos - 4;
    v2 = gOamEnt(oam_next_ptr);
    *(uint16 *)&v2->xcoord = v1;
    if ((v1 & 0x100) != 0) {
      int v3 = v0 >> 1;
      R34 = kOamExtra_Address_And_X8Large[v3];
      v4 = kOamExtra_X8Small_And_Large[v3] | *(uint16 *)RomPtr_RAM(R34);
      *(uint16 *)RomPtr_RAM(R34) = v4;
    }
    *(uint16 *)&v2->ycoord = grapple_beam_end_y_pos - layer1_y_pos - 4;
    *(uint16 *)&v2->charnum = 14880;
    oam_next_ptr = v0 + 4;
  }
}

void DrawGrappleOams3(void) {  // 0x94B14B
  int16 v1;
  OamEnt *v2;
  int16 v4;

  uint16 v0 = oam_next_ptr;
  v1 = grapple_beam_end_x_pos - layer1_x_pos - 4;
  v2 = gOamEnt(oam_next_ptr);
  *(uint16 *)&v2->xcoord = v1;
  if ((v1 & 0x100) != 0) {
    int v3 = v0 >> 1;
    R34 = kOamExtra_Address_And_X8Large[v3];
    v4 = kOamExtra_X8Small_And_Large[v3] | *(uint16 *)RomPtr_RAM(R34);
    *(uint16 *)RomPtr_RAM(R34) = v4;
  }
  *(uint16 *)&v2->ycoord = grapple_beam_end_y_pos - layer1_y_pos - 4;
  *(uint16 *)&v2->charnum = 14880;
  oam_next_ptr = v0 + 4;
}
