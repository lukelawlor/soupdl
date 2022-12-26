/*
 * video.c contains definitions for the game's window and renderer.
 */

#include <math.h>	// For ceil()

#include <SDL2/SDL.h>

#include "video.h"

// Game window and renderer
SDL_Window *g_window;
SDL_Renderer *g_renderer;

// Screen dimensions
int g_screen_width;
int g_screen_height;
float g_screen_xscale = 1.0f;
float g_screen_yscale = 1.0f;

// Scales SDL's renderer and updates screen dimensions
void screen_scale(float xscale, float yscale)
{
	g_screen_xscale = xscale;
	g_screen_yscale = yscale;
	SDL_RenderSetScale(g_renderer, g_screen_xscale, g_screen_yscale);
	screen_update_dimensions();
}

// Updates g_screen_width and g_screen_height
void screen_update_dimensions(void)
{
	SDL_GetWindowSize(g_window, &g_screen_width, &g_screen_height);
	g_screen_width = ceil((double) g_screen_width / g_screen_xscale);
	g_screen_height = ceil((double) g_screen_height / g_screen_yscale);
}
