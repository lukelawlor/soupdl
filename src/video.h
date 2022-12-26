/*
 * video.h contains extern declarations for the game's window and renderer.
 */

#ifndef	VIDEO_H
#define	VIDEO_H

#include <SDL2/SDL.h>

// Comment to disable vsync
#define	VSYNC

// How many ticks a frame should be if vsync isn't enabled
#define	FRAME_TICKS	1000/30

// Game window and renderer
extern SDL_Window *g_window;
extern SDL_Renderer *g_renderer;

// Screen dimensions
extern int g_screen_width;
extern int g_screen_height;

// Screen x and y scale
extern float g_screen_xscale;
extern float g_screen_yscale;

// Scales SDL's renderer and updates screen dimensions
void screen_scale(float xscale, float yscale);

// Updates g_screen_width and g_screen_height
void screen_update_dimensions(void);

#endif
