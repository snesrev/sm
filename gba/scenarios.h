#pragma once
#include <stdbool.h>
#include <stdint.h>

// For testing: see scenarios.c. The GBA's buttons the game sees this frame, in
// place of the real |keys| (from Input_ReadJoypad).
uint16_t Scn_Keys(uint16_t keys);
// Before each frame of the game; and after, whether to hold the frame on
// screen for a screenshot (not, if the scenarios are for sound).
void Scn_BeforeFrame(void);
bool Scn_AfterFrame(void);
// Between scenarios (input.c): what R readies is missiles again.
void Input_Reset(void);
