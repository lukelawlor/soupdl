/*
 * video.c contains definitions for the game's window and renderer.
 */

#include <math.h>		// For ceil()

#include <SDL2/SDL.h>

#include "camera.h"		// For cam_update_limits()
#include "entity/cloud.h"	// For ent_cloud_update_count()
#include "video.h"

// The initial value of g_no_vsync_refresh_rate
#define	CONSTEXPR_G_NO_VSYNC_REFRESH_RATE	30

// Game window and renderer
SDL_Window *g_window;
SDL_Renderer *g_renderer;

// Screen dimensions
int g_screen_width;
int g_screen_height;
float g_screen_xscale = 1.0f;
float g_screen_yscale = 1.0f;

// True if VSYNC is enabled (it is by default)
bool g_vsync = false;

// Display refresh rate of window if vsync is disabled
int g_no_vsync_refresh_rate = CONSTEXPR_G_NO_VSYNC_REFRESH_RATE;

// The number of SDL ticks a frame should last if vsync is disabled
unsigned int g_no_vsync_frame_ticks = 1000.0 / CONSTEXPR_G_NO_VSYNC_REFRESH_RATE;

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

	// Update misc game systems

	// Update limits of camera
	cam_update_limits();

	// Update the number of clouds to reflect new screen dimensions
	ent_cloud_update_count();
}
