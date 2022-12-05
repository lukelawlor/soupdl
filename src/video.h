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

#endif
