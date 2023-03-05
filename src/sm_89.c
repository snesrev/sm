// Item PLM graphics, FX loader

#include "ida_types.h"
#include "variables.h"
#include "funcs.h"

#define g_word_89AA02 ((uint16*)RomPtr(0x89aa02))
void LoadFxEntry(uint16 v0) {  // 0x89AB02
  FxDef *FxDef;

  current_fx_entry_offset = 16 * (v0 & 7);
  current_fx_entry_ptr = room_layer3_asm_ptr + current_fx_entry_offset;
  FxDef = get_FxDef(room_layer3_asm_ptr + current_fx_entry_offset);
  fx_base_y_pos = FxDef->base_y_pos;
  fx_target_y_pos = FxDef->target_y_pos;
  fx_y_vel = FxDef->y_vel;
  fx_timer = FxDef->timer;
  fx_layer_blending_config_a = FxDef->default_layer_blend;
  fx_layer_blending_config_b = FxDef->layer3_layer_blend;
  fx_liquid_options = FxDef->fx_liquid_options_;
  if (FxDef->palette_blend) {
    int v2 = FxDef->palette_blend >> 1;
    palette_buffer[25] = g_word_89AA02[v2];
    palette_buffer[26] = g_word_89AA02[v2 + 1];
    palette_buffer[27] = g_word_89AA02[v2 + 2];
  } else {
    palette_buffer[27] = 0;
  }
}

void nullsub_106(void) {}

#define kFxTypeTilemapPtrs ((uint16*)RomPtr(0x83abf0))
static Func_V *const kFxTypeFuncPtrs[23] = {  // 0x89AB82
  nullsub_106,
  FxTypeFunc_2_Lava,
  FxTypeFunc_4_Acid,
  FxTypeFunc_6_Water,
  FxTypeFunc_8_Spores,
  FxTypeFunc_A_Rain,
  FxTypeFunc_C,
  nullsub_106,
  nullsub_106,
  nullsub_106,
  nullsub_106,
  nullsub_106,
  nullsub_106,
  nullsub_106,
  nullsub_106,
  nullsub_106,
  FxTypeFunc_20,
  FxTypeFunc_22_ScrollingSky,
  FxTypeFunc_24,
  FxTypeFunc_26_TourianEntranceStatue,
  FxTypeFunc_28_CeresRidley,
  FxTypeFunc_CeresElevator,
  FxTypeFunc_2C_Haze,
};
#define kAreaPalFxListPointers ((uint16*)RomPtr(0x83ac46))
#define kAreaAnimtilesListPtrs ((uint16*)RomPtr(0x83ac56))
void LoadFXHeader(void) {
  int i;
  FxDef *v6;
  FxDef *v8;
  uint16 j;
  char v10; // cf
  FxDef *v12;
  uint16 k;
  FxDef *FxDef;

  uint16 v0 = room_layer3_asm_ptr;
  if (room_layer3_asm_ptr) {
    for (i = 0; ; i += 16) {
      uint16 door_ptr = get_FxDef(v0)->door_ptr;
      if (!door_ptr)
        break;
      if (door_ptr == 0xFFFF)
        return;
      if (door_ptr == door_def_ptr)
        break;
      v0 += 16;
    }
    current_fx_entry_offset = i;
    current_fx_entry_ptr = v0;
    FxDef = get_FxDef(v0);
    fx_base_y_pos = FxDef->base_y_pos;
    fx_target_y_pos = FxDef->target_y_pos;
    fx_y_vel = FxDef->y_vel;
    fx_timer = FxDef->timer;
    fx_layer_blending_config_a = FxDef->default_layer_blend;
    fx_layer_blending_config_b = FxDef->layer3_layer_blend;
    fx_liquid_options = FxDef->fx_liquid_options_;
    if (FxDef->palette_blend) {
      int v4 = FxDef->palette_blend >> 1;
      target_palettes[25] = g_word_89AA02[v4];
      target_palettes[26] = g_word_89AA02[v4 + 1];
      target_palettes[27] = g_word_89AA02[v4 + 2];
    } else {
      target_palettes[27] = 0;
    }
    uint16 type = get_FxDef(current_fx_entry_ptr)->type;
    fx_type = type;
    if (type)
      fx_tilemap_ptr = kFxTypeTilemapPtrs[type >> 1];
    v6 = get_FxDef(current_fx_entry_ptr);
    uint16 v7 = v6->type;
    if (v6->type) {
      *(uint16 *)((char *)&R18_ + 1) = -30720;
      kFxTypeFuncPtrs[v7 >> 1]();
    }
    v8 = get_FxDef(current_fx_entry_ptr);
    if (v8->palette_fx_bitset) {
      current_fx_palette_fx_bitset = v8->palette_fx_bitset;
      current_region_animated_palette_fx = kAreaPalFxListPointers[area_index];
      for (j = 0; j != 16; j += 2) {
        while (1) {
          v10 = current_fx_palette_fx_bitset & 1;
          current_fx_palette_fx_bitset >>= 1;
          if (!v10)
            break;
          uint8 *v11 = RomPtr_83(current_region_animated_palette_fx);
          SpawnPalfxObject(*(uint16 *)&v11[j]);
          j += 2;
        }
      }
    }
    v12 = get_FxDef(current_fx_entry_ptr);
    if (v12->animtiles_bitset) {
      current_fx_palette_fx_bitset = v12->animtiles_bitset;
      current_region_animated_palette_fx = kAreaAnimtilesListPtrs[area_index];
      for (k = 0; k != 16; k += 2) {
        while (1) {
          v10 = current_fx_palette_fx_bitset & 1;
          current_fx_palette_fx_bitset >>= 1;
          if (!v10)
            break;
          uint8 *v14 = RomPtr_83(current_region_animated_palette_fx);
          SpawnAnimtiles(*(uint16 *)&v14[k]);
          k += 2;
        }
      }
    }
  }
}
#define g_word_89AD5F ((uint16*)RomPtr(0x89ad5f))
void RoomCode_CeresElevatorShaft(void) {  // 0x89ACC3
  int16 v1;

  if ((ceres_status & 0x8000u) != 0) {
    if ((int16)(112 - samus_x_pos) < 0
        && (int16)(144 - samus_x_pos) >= 0
        && sign16(samus_y_pos - 128)
        && !sign16(samus_y_pos - 75)
        && !samus_y_speed
        && !samus_y_subspeed
        && game_state == kGameState_8_MainGameplay) {
      CallSomeSamusCode(2u);
      screen_fade_delay = 0;
      screen_fade_counter = 0;
      game_state = 32;
    }
    -- *(uint16 *)&room_main_asm_variables[2];
    if (*(int16 *)&room_main_asm_variables[2] < 0) {
      int v0 = (uint16)(6 * *(uint16 *)room_main_asm_variables) >> 1;
      *(uint16 *)&room_main_asm_variables[2] = g_word_89AD5F[v0];
      reg_M7B = g_word_89AD5F[v0 + 1];
      reg_M7C = -reg_M7B;
      reg_M7A = g_word_89AD5F[v0 + 2];
      reg_M7D = reg_M7A;
      if (*(int16 *)room_main_asm_variables < 0) {
        v1 = *(uint16 *)room_main_asm_variables - 1;
        if (*(uint16 *)room_main_asm_variables == 0x8001)
          v1 = 0;
      } else {
        v1 = *(uint16 *)room_main_asm_variables + 1;
        if (*(uint16 *)room_main_asm_variables == 67)
          v1 = -32700;
      }
      *(uint16 *)room_main_asm_variables = v1;
    }
  }
}
