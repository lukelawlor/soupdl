/*
 * video.h contains extern declarations for the game's window and renderer.
 */

#ifndef	VIDEO_H
#define	VIDEO_H

#include <stdbool.h>

#include <SDL2/SDL.h>

// Game window and renderer
extern SDL_Window *g_window;
extern SDL_Renderer *g_renderer;

// Screen dimensions
extern int g_screen_width;
extern int g_screen_height;

// Screen x and y scale
extern float g_screen_xscale;
extern float g_screen_yscale;

// True if VSYNC is enabled
extern bool g_vsync;

// Display refresh rate of window if vsync is disabled
extern int g_no_vsync_refresh_rate;

// The number of SDL ticks a frame should last if vsync is disabled
extern unsigned int g_no_vsync_frame_ticks;

// Scales SDL's renderer and updates screen dimensions
void screen_scale(float xscale, float yscale);

// Updates g_screen_width and g_screen_height
void screen_update_dimensions(void);

#endif
