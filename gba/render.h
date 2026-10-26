// Shows the SNES picture the game sets up on the GBA's own video hardware:
// SNES backgrounds as GBA text backgrounds, SNES sprites as GBA sprites,
// and the SNES's 256x224 screen cropped to the GBA's 240x160.
#pragma once

void Render_Init(void);
// After a frame of the game: runs the SNES's display of it (HDMA and the
// game's IRQ handlers, which change registers partway down the screen),
// and prepares the GBA's registers for each line.
void Render_CaptureFrame(void);
// In VBlank: updates the GBA's video memory and registers.
void Render_Present(void);
// About how many of the GBA's lines the next Render_Present will take.
int Render_PresentLines(void);
// How many lines below the SNES's the GBA shows gameplay, under its HUD
// (which follows Samus): for sprites that stay put on the screen, as the
// escape timer.
int Render_GameplayYOffset(void);
