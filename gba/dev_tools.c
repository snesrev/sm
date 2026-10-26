// Developer tools: spawn Samus at any load station with a chosen loadout, to
// test a room, a fight or a weapon without playing up to it.
//
// The game has this in it already, as its developers' debug mode: with
// enable_debug set (nothing sets it; the developers poked it in), starting a
// game from a file goes to the load-game map (game state 5), even for a new
// file, where the player can choose any area and then step with Select
// through its load stations: the save stations and elevators the file has
// used, then the debug stations, indexes 16 and up in kLoadStationLists, the
// developers' own spawn points. Some are in rooms next to the bosses', and
// in the Golden Torizo's. The SNES had a second controller whose
// Select the game reads for the stepping; the GBA's Select stands in for it
// (Input_ReadJoypad2). This file adds the rest:
//   - turning it on: hold L and R and press Start on the title screen or the
//     file select (Dev_TryEnable). Until reset.
//   - every station selectable, and every room of the map to see, whether the
//     file has visited them or not: the map's used bits and the map stations'
//     bytes (what shows an area's whole map) are all set while it's on show,
//     and put back after.
//   - the bosses' rooms, which the debug stations are next to, not in (but for
//     the Golden Torizo's): after an area's last station, Select steps on to
//     them (Warp), and the spawn goes through the room's door when the station
//     it loaded has Samus ready.
//   - a loadout, toggled with L on the room map: what the file has, or ALL
//     ITEMS (Samus with everything the game lets her have at once). Drawn on
//     the map's BG2 tilemap, in the map's own font; applied as the spawn
//     loads.
//   - in gameplay, hold L + R + Select and press A (invincibility, the game's
//     debug_invincibility), B (refill), Left (freeze enemies, the game's
//     debug_time_frozen_for_enemies) or Up (die: the game over's "yes" goes
//     back to the map, to spawn again).
//   - the SNES debug pad off (Dev_Frame says why).
//   - the game's own Start + Select + L + R soft reset (ReadJoypadInputs
//     checks it with enable_debug set) works in menus too.
// A loadout spawn doesn't save: SaveToSram would write it over the player's
// file (Dev_SaveBlocked).
//
// Not here, and why: the debug scroll save/load, the Draygon puppet and the
// demo recorder's pause and hide (they read the controller 2 in code nothing
// calls, or in a mode nothing enters), and the Golden Torizo room's X+Y+A+B
// cheat (it gives its loadout, nearly ALL ITEMS).
#include "dev_tools.h"

#include <stdio.h>
#include <string.h>

#include "funcs.h"
#include "gba_hw.h"
#include "ida_types.h"
#include "input.h"
#include "sm_rtl.h"
#include "snes_hw.h"
#include "variables.h"

enum {
  kGameStateTitle = 1, kGameStateOptions = 2, kGameStateFileSelect = 4,
  kGameStateMap = 5, kGameStateLoading = 6, kGameStateGameplay = 8, kGameStateHitDoorBlock = 9,
  // The room map's last step: waiting for the player.
  kMenuRoomMap = 10,
};

// What ALL ITEMS gives Samus, as the game keeps it. Items: varia 1, spring
// ball 2, morph ball 4, screw attack 8, gravity suit 0x20, hi-jump 0x100,
// space jump 0x200, bombs 0x1000, speed booster 0x2000, grapple 0x4000, x-ray
// 0x8000. Beams: wave 1, ice 2, spazer 4, plasma 8, charge 0x1000. Not both
// spazer and plasma equipped: the game's pickups and equipment screen never
// allow it (a plasma beam takes the spazer off), and with both on no shots
// showed (tried on the GBA).
enum {
  kAllHealth = 1499, kAllReserve = 400, kAllMissiles = 230, kAllSupers = 50, kAllBombs = 50,
  kAllItems = 0xf32f, kAllBeamsCollected = 0x100f, kAllBeamsEquipped = 0x100b,
};

// The boss rooms, which the stations are not in (the debug ones are in the
// rooms next to them): the map steps on to these, after an area's last
// station, and the spawn goes through the door into the room as Samus
// would, once the station's loaded (the door as the neighbor's door list
// has it, bank $83). (Spore Spawn's is the one from below: the camera in his
// room is held at the arena, so Samus arriving at the top would be off screen.)
typedef struct Warp {
  uint8_t area;
  uint16_t door;
  const char *name;
} Warp;

static const Warp kWarps[] = {
  {0, 0x8bc2, "BOMB TORIZO"},
  {1, 0x8e3e, "SPORE SPAWN"},
  {1, 0x91b6, "KRAID"},
  {2, 0x93d2, "CROCOMIRE"},
  {2, 0x9a62, "RIDLEY"},
  {3, 0xa2ac, "PHANTOON"},
  {4, 0xa774, "BOTWOON"},
  {4, 0xa924, "DRAYGON"},
  {5, 0xaae0, "MOTHER BRAIN"},
};
enum { kWarpCount = sizeof(kWarps) / sizeof(kWarps[0]) };

// These are in .ewram_bss, after all other statics, so as not to move them:
// with Samus held outside a room (the room tour does), the game reads past
// the end of g_ram, into what follows it, and where things are changes what
// it finds.
// Whether the loadout is ALL ITEMS, not what the file has.
static bool loadout EWRAM_BSS;
// On the room map, which of the area's warps is picked, from 1, or 0 for the
// station; and the door of the one a spawn goes on through.
static uint8_t warp EWRAM_BSS;
static uint16_t picked_door EWRAM_BSS, pending_door EWRAM_BSS;
// Whether Select is the map's this frame (not taken for the warps).
static bool select_free EWRAM_BSS;
static bool save_blocked EWRAM_BSS;
#if SM_SCENARIOS
// The beams a scenario has, in the game's bits (-1: the loadout's).
static int16_t scn_beams EWRAM_BSS;
#endif
// The map's used station bits (8 save stations, 8 elevators, by area) and the
// map stations' bytes (by area) as the file has them, while the map shows all.
static uint8_t used_backup[16] EWRAM_BSS, map_backup[8] EWRAM_BSS;
static bool used_set EWRAM_BSS;
static uint16_t prev_state EWRAM_BSS;
static bool drawn EWRAM_BSS;

void Dev_TryEnable(void) {
  uint16_t state = game_state;
  if (enable_debug || (state != kGameStateTitle && state != kGameStateOptions &&
                       state != kGameStateFileSelect))
    return;
  enable_debug = 1;
  // As if for the first time: a reset may have come with the map up.
  loadout = false;
  warp = 0;
  picked_door = pending_door = 0;
  save_blocked = used_set = drawn = false;
  prev_state = 0;
  QueueSfx1_Max6(0x3c);
  printf("sm: developer tools on\n");
}

bool Dev_SaveBlocked(void) {
  return enable_debug && save_blocked;
}

// The tile of the room map's font, in BG2's tilemap: letters, digits, or a
// blank, with the palette and priority the map's own text has.
static uint16_t Glyph(char c) {
  if (c >= 'A' && c <= 'Z')
    return 0x2830 + (c - 'A');
  if (c >= '0' && c <= '9')
    return 0x2804 + (c - '0');
  return 0x2801;
}

enum { kBg2Map = 0x5800, kColumns = 32 };

// Writes a line of text to a row of BG2's map, centered. (The map's rows 25,
// 26 and 29 are blank, in view, with the room map showing; rows 27 and 28
// hold the game's own buttons, which DrawMap replaces.) Lines on
// neighboring rows touch, and squashed they ran together, so DrawMap leaves
// a row between them.
static void DrawRow(int row, const char *text) {
  uint16_t line[kColumns];
  int len = 0;
  while (text[len])
    len++;
  if (len > kColumns - 2)
    len = kColumns - 2;
  int start = (kColumns - len) >> 1;
  for (int i = 0; i < kColumns; i++)
    line[i] = i >= start && i < start + len ? Glyph(text[i - start]) : 0x2801;
  SnesHw_WriteVram(kBg2Map + row * kColumns, line, kColumns);
}

typedef struct Text {
  char s[kColumns + 1];
  int n;
} Text;

static void Put(Text *t, const char *s) {
  while (*s && t->n < kColumns)
    t->s[t->n++] = *s++;
  t->s[t->n] = 0;
}

static void PutNumber(Text *t, int v) {
  // Two digits are enough: load stations are numbered under 32.
  if (v >= 10) {
    int tens = v >= 30 ? 3 : v >= 20 ? 2 : 1;
    char d[2] = {'0' + tens, 0};
    Put(t, d);
    v -= tens * 10;
  }
  char d[2] = {'0' + v, 0};
  Put(t, d);
}

// The debug stations that are worth a name, by area (the game's number, not
// the map's order) and index - 16: the rooms next to a boss's (a door of
// theirs leads into it), and the Golden Torizo's. The rest are shown by their
// room. (Norfair 21's spot, (640, 512), is outside its room of 2 by 2
// screens, as the developers' list has it: see Apply.)
static const char *const kDebugStations[6][8] = {
  /* Crateria */ {"LANDING SITE"},
  /* Brinstar */ {0, "NEXT TO KRAID"},
  /* Norfair */ {0, "NEXT TO CROCOMIRE", "NEXT TO RIDLEY", 0, 0, "GOLDEN TORIZO (AS 22)",
                 "GOLDEN TORIZO"},
  /* Wrecked Ship */ {0, "NEXT TO PHANTOON"},
  /* Maridia */ {0, "NEXT TO DRAYGON", "NEXT TO BOTWOON"},
  /* Tourian */ {"NEXT TO MOTHER BRAIN", "NEXT TO MOTHER BRAIN"},
};

// The area's |n|th warp (from 1), or null past its last.
static const Warp *WarpOf(int area, int n) {
  for (int i = 0; i < kWarpCount; i++)
    if (kWarps[i].area == area && --n == 0)
      return &kWarps[i];
  return 0;
}

// The area's last station on the map: the last icon before the list's end
// that isn't a blank (0xfffe).
static int LastStation(int area) {
  const uint8_t *p = RomPtr_82(0xc7cb + 64 + 2 * area);
  const uint8_t *icons = RomPtr_82(p[0] | p[1] << 8);
  int last = 0;
  for (int i = 0; i < 32 && (icons[4 * i] & icons[4 * i + 1]) != 0xff; i++)
    if (icons[4 * i + 1] != 0xff)
      last = i;
  return last;
}

static void PutHex(Text *t, uint16_t v) {
  for (int shift = 12; shift >= 0; shift -= 4) {
    int d = v >> shift & 15;
    char c[2] = {d < 10 ? '0' + d : 'A' + d - 10, 0};
    Put(t, c);
  }
}

// What the map has picked: a boss's door, or a station by its number and its
// name, or else its room's.
static void PutStation(Text *t) {
  int idx = load_station_index;
  int area = area_index;
  const Warp *w = warp ? WarpOf(area, warp) : 0;
  if (w) {
    Put(t, "BOSS ");
    Put(t, w->name);
    return;
  }
  Put(t, idx >= 16 ? "DEBUG " : idx >= 8 ? "ELEVATOR " : "SAVE ");
  PutNumber(t, idx >= 8 && idx < 16 ? idx - 8 : idx);
  Put(t, " ");
  const char *name = idx >= 16 && idx < 24 && area < 6 ? kDebugStations[area][idx - 16] : 0;
  if (name) {
    Put(t, name);
  } else {
    // The room, by its pointer (a ROM word at an odd address is read by
    // bytes).
    const uint8_t *list = RomPtr_80(0xc4b5 + 2 * area);
    const uint8_t *room = RomPtr_80((list[0] | list[1] << 8) + 14 * idx);
    Put(t, "ROOM ");
    PutHex(t, room[0] | room[1] << 8);
  }
}

static void DrawMap(void) {
  Text t = {{0}, 0};
  PutStation(&t);
  DrawRow(25, t.s);
  // The game's buttons here (the map's scrolling, A start, B cancel, with
  // their icons on two rows) left out Select and L; these say all of them,
  // a row apart.
  Text l = {{0}, 0};
  Put(&l, "SELECT NEXT   L ");
  Put(&l, loadout ? "ALL ITEMS" : "AS SAVED");
  DrawRow(27, l.s);
  DrawRow(28, "");
  DrawRow(29, "DPAD SCROLL  A SPAWN  B BACK");
}

static void Apply(void) {
  // Norfair's station 21 is out of its room's bounds, where the game reads
  // past the end of its RAM: 22 is the same room, with Samus in it.
  if (area_index == 2 && load_station_index == 21)
    load_station_index = 22;
  if (loadout) {
    // Every boss alive again, to fight: the file may have killed them.
    memset(boss_bits_for_area, 0, 8);
    samus_max_health = samus_health = kAllHealth;
    samus_max_reserve_health = samus_reserve_health = kAllReserve;
    reserve_health_mode = 1;
    samus_max_missiles = samus_missiles = kAllMissiles;
    samus_max_super_missiles = samus_super_missiles = kAllSupers;
    samus_max_power_bombs = samus_power_bombs = kAllBombs;
    collected_items = equipped_items = kAllItems;
    collected_beams = kAllBeamsCollected;
    equipped_beams = kAllBeamsEquipped;
  }
#if SM_SCENARIOS
  // (The scenarios' beams, which no button picks.)
  if (scn_beams >= 0)
    collected_beams = equipped_beams = scn_beams;
#endif
  save_blocked = loadout;
  pending_door = picked_door;
  picked_door = 0;
}

static void Refill(void) {
  samus_health = samus_max_health;
  samus_reserve_health = samus_max_reserve_health;
  samus_missiles = samus_max_missiles;
  samus_super_missiles = samus_max_super_missiles;
  samus_power_bombs = samus_max_power_bombs;
}

bool Dev_Frame(uint16_t keys, uint16_t pressed) {
  uint16_t state = game_state;
  // The map shows every station and every room: the file's used bits and map
  // stations' bytes all set while it's up.
  if (state == kGameStateMap) {
    if (!used_set) {
      memcpy(used_backup, used_save_stations_and_elevators, sizeof(used_backup));
      memset(used_save_stations_and_elevators, 0xff, sizeof(used_backup));
      memcpy(map_backup, map_station_byte_array, sizeof(map_backup));
      memset(map_station_byte_array, 0xff, sizeof(map_backup));
      used_set = true;
    }
  } else if (used_set) {
    memcpy(used_save_stations_and_elevators, used_backup, sizeof(used_backup));
    memcpy(map_station_byte_array, map_backup, sizeof(map_backup));
    // (The room map's load took it from the bytes, for the pause screen's
    // map and the save: the file's.)
    has_area_map = map_station_byte_array[area_index];
    used_set = false;
  }
  // The SNES's debug pad (ReadJoypadInputs: Select with L or R, and a
  // third button) is off: it swallows those buttons, and its A toggles the
  // IRQ (a jump through whatever handler a menu left), its B swaps Samus's
  // missiles away and keeps them out of the file's reach. Select and R go
  // together on the GBA: the map's Select step, R's item.
  joypad_dbg_flags |= 0x4000;
  // The planet map shows the areas by this index (the game's, past the six
  // when Samus's room, or a new file's start, is in Ceres): none of them
  // lit, if it's one of those.
  if (state == kGameStateMap && menu_index < kMenuRoomMap && area_index > 5)
    area_index = 0;
  select_free = true;
  if (state == kGameStateMap && menu_index == kMenuRoomMap) {
    if (pressed & kKeyL)
      loadout = !loadout;
    if (pressed & kKeySelect) {
      // The warps come after the area's last station, and then it starts
      // over: the game's step would have gone to its first.
      if (warp) {
        select_free = false;
        if (!WarpOf(area_index, ++warp)) {
          warp = 0;
          load_station_index = 0;
        }
      } else if (WarpOf(area_index, 1) && load_station_index == LastStation(area_index)) {
        select_free = false;
        warp = 1;
      }
    }
    static uint32_t shown EWRAM_BSS;
    uint32_t now = loadout | load_station_index << 9 | area_index << 14 | warp << 17;
    if (!drawn || now != shown) {
      DrawMap();
      shown = now;
      drawn = true;
    }
    const Warp *w = warp ? WarpOf(area_index, warp) : 0;
    picked_door = w ? w->door : 0;
  } else {
    drawn = false;
    warp = 0;
    // (It's taken as the map hands over to the load: not before, if the
    // player goes back from the room map.)
    if (state == kGameStateMap ? menu_index < kMenuRoomMap : state != kGameStateLoading)
      picked_door = 0;
  }
  // The map's last step hands over to the load, which the loadout is
  // applied to before it reads it.
  if (state == kGameStateLoading && prev_state == kGameStateMap)
    Apply();
  prev_state = state;
  if (pending_door && state == kGameStateGameplay &&
      frame_handler_beta == (uint16_t)fnSamus_FrameHandlerBeta_Func17) {
    // Through the door, as if Samus had walked into it, once her arrival
    // (the fanfare, with its sound) is over and she has her controls.
    // The transition takes where she is in the doorway from the low bytes
    // of her position: the screen's far side for a door she goes right
    // through, the near side for one she goes left through, and standing on
    // the floor below the door's lowest block (the door's top row is
    // y_pos_plm, in the room she leaves; four blocks tall): her centre is 24
    // above her feet.
    const DoorDef *door = get_DoorDef(pending_door);
    int dir = door->door_orientation & 3;
    int x_low, y_low;
    if (dir < 2) {
      x_low = dir ? 0x18 : 0xe8;
      y_low = (door->y_pos_plm + 4) * 16 - 24;
    } else {
      x_low = (door->x_pos_plm + 2) * 16;
      y_low = dir == 2 ? 0xc0 : 0x30;
    }
    samus_x_pos = (samus_x_pos & 0xff00) | (x_low & 0xff);
    samus_y_pos = (samus_y_pos & 0xff00) | (y_low & 0xff);
    door_def_ptr = pending_door;
    door_transition_function = (uint16_t)fnDoorTransitionFunction_HandleElevator;
    elevator_flags = 0;
    game_state = kGameStateHitDoorBlock;
    pending_door = 0;
  }
  enum { kCombo = kKeyL | kKeyR | kKeySelect };
  if (state == kGameStateGameplay && (keys & kCombo) == kCombo && !queued_message_box_index) {
    if (pressed & kKeyA) {
      debug_invincibility = debug_invincibility >= 7 ? 0 : 7;
      QueueSfx1_Max6(debug_invincibility ? 0x38 : 0x37);
    }
    if (pressed & kKeyB) {
      Refill();
      QueueSfx1_Max6(0x38);
    }
    if (pressed & kKeyLeft) {
      debug_time_frozen_for_enemies = !debug_time_frozen_for_enemies;
      QueueSfx1_Max6(debug_time_frozen_for_enemies ? 0x38 : 0x37);
    }
    if (pressed & kKeyUp) {
      debug_invincibility = 0;
      // (No reserve tank to step in.)
      samus_reserve_health = 0;
      samus_health = samus_prev_health_for_flash = 0;
    }
    return true;
  }
  return false;
}

bool Dev_SelectFree(void) {
  return select_free;
}

#if SM_SCENARIOS
void Dev_Pick(int beams, int w) {
  loadout = true;
  scn_beams = beams;
  warp = w;
  // (The warps' spawn is at the area's last station.)
  if (w)
    load_station_index = LastStation(area_index);
}

bool Dev_SpawnPending(void) {
  return pending_door != 0;
}
#endif
