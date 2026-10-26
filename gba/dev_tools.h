// The developer tools: see dev_tools.c.
#pragma once

#include <stdbool.h>
#include <stdint.h>

// Turns the tools on, if the game is at the title or the file select: from
// Input_ReadJoypad, when Start goes down with L and R held.
void Dev_TryEnable(void);
// Once a frame while they're on (game's enable_debug): |keys| are the GBA's
// buttons held, |pressed| the ones that went down. Returns whether the
// frame's buttons are used up (a gameplay shortcut), to pass none to the game.
bool Dev_Frame(uint16_t keys, uint16_t pressed);
// Whether the game's SaveToSram should do nothing: a spawn with a loadout
// would write it over the player's file.
bool Dev_SaveBlocked(void);
// Whether the game may see the Select that stepped the room map this frame:
// not when it went on to a boss's door (Dev_Frame took it).
bool Dev_SelectFree(void);

#if SM_SCENARIOS
// For scenarios.c, on the room map: the ALL ITEMS loadout, with these beams
// (the game's bits, -1 to keep ALL ITEMS's), and the area's boss door |warp|,
// from 1, or 0 for a station: the way the buttons pick them.
void Dev_Pick(int beams, int warp);
// Whether a spawn still has to go through its boss's door.
bool Dev_SpawnPending(void);
#endif
