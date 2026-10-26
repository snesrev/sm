#pragma once
#include <stdbool.h>

// For testing: see tour.c. Before each frame of the game; and after, whether
// to hold the frame on screen for a screenshot.
// The joypad the game sees, in place of the GBA's (|real|): the tour's
// own way through the title and the file select, by the game's frames, so
// that what it does doesn't hang on when smrun's keys land; and nothing
// once it's in the game, but with -DSM_TOUR_FREE, the GBA's.
uint16_t Tour_Joypad(uint16_t real);
bool Tour_BeforeFrame(void);
bool Tour_AfterFrame(void);
