// Item PLM graphics, FX loader

#include "ida_types.h"
#include "variables.h"
#include "funcs.h"


#define g_word_89AA02 ((uint16*)RomFixedPtr(0x89aa02))
#define kFxTypeTilemapPtrs ((uint16*)RomFixedPtr(0x83abf0))
#define kAreaPalFxListPointers ((uint16*)RomFixedPtr(0x83ac46))
#define kAreaAnimtilesListPtrs ((uint16*)RomFixedPtr(0x83ac56))
#define g_word_89AD5F ((uint16*)RomFixedPtr(0x89ad5f))



void LoadFxEntry(uint16 v0) {  // 0x89AB02
  FxDef *FD = get_FxDef(room_layer3_asm_ptr + 16 * (v0 & 7));
  fx_base_y_pos = FD->base_y_pos;
  fx_target_y_pos = FD->target_y_pos;
  fx_y_vel = FD->y_vel;
  fx_timer = FD->timer;
  fx_layer_blending_config_a = FD->default_layer_blend;
  fx_layer_blending_config_b = FD->layer3_layer_blend;
  fx_liquid_options = FD->fx_liquid_options_;
  if (FD->palette_blend) {
    int v2 = FD->palette_blend >> 1;
    palette_buffer[25] = g_word_89AA02[v2];
    palette_buffer[26] = g_word_89AA02[v2 + 1];
    palette_buffer[27] = g_word_89AA02[v2 + 2];
  } else {
    palette_buffer[27] = 0;
  }
}

void nullsub_106(void) {}

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
void LoadFXHeader(void) {
  uint16 entry_ptr = room_layer3_asm_ptr;
  if (entry_ptr == 0)
    return;
  for(;;) {
    uint16 door_ptr = get_FxDef(entry_ptr)->door_ptr;
    if (!door_ptr)
      break;
    if (door_ptr == 0xFFFF)
      return;
    if (door_ptr == door_def_ptr)
      break;
    entry_ptr += 16;
  }
  FxDef *FD = get_FxDef(entry_ptr);
  fx_base_y_pos = FD->base_y_pos;
  fx_target_y_pos = FD->target_y_pos;
  fx_y_vel = FD->y_vel;
  fx_timer = FD->timer;
  fx_layer_blending_config_a = FD->default_layer_blend;
  fx_layer_blending_config_b = FD->layer3_layer_blend;
  fx_liquid_options = FD->fx_liquid_options_;
  if (FD->palette_blend) {
    int v4 = FD->palette_blend >> 1;
    target_palettes[25] = g_word_89AA02[v4];
    target_palettes[26] = g_word_89AA02[v4 + 1];
    target_palettes[27] = g_word_89AA02[v4 + 2];
  } else {
    target_palettes[27] = 0;
  }
  fx_type = FD->type;
  if (FD->type) {
    fx_tilemap_ptr = kFxTypeTilemapPtrs[FD->type >> 1];
    kFxTypeFuncPtrs[FD->type >> 1]();
  }
  if (FD->palette_fx_bitset) {
    int bits = FD->palette_fx_bitset;
    const uint8 *v11 = RomPtr_83(kAreaPalFxListPointers[area_index]);
    for (int j = 0; j != 16; j += 2, bits >>= 1) {
      if (bits & 1)
        SpawnPalfxObject(*(uint16 *)&v11[j]);
    }
  }
  if (FD->animtiles_bitset) {
    int bits = FD->animtiles_bitset;
    const uint8 *v14 = RomPtr_83(kAreaAnimtilesListPtrs[area_index]);
    for (int k = 0; k != 16; k += 2, bits >>= 1) {
      if (bits & 1)
        SpawnAnimtiles(*(uint16 *)&v14[k]);
    }
  }
}
void RoomCode_CeresElevatorShaft(void) {  // 0x89ACC3
  int16 v1;

  if ((ceres_status & 0x8000) != 0) {
    if ((int16)(112 - samus_x_pos) < 0
        && (int16)(144 - samus_x_pos) >= 0
        && sign16(samus_y_pos - 128)
        && !sign16(samus_y_pos - 75)
        && !samus_y_speed
        && !samus_y_subspeed
        && game_state == kGameState_8_MainGameplay) {
      CallSomeSamusCode(2);
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
