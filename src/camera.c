/*
 * camera.c contains functions for updating the camera's components.
 */

#include <stdlib.h>		// For abs()

#include "camera.h"
#include "entity/cloud.h"	// For ent_cloud_update_count()
#include "input.h"		// For g_key_state
#include "map.h"		// For map dimensions
#include "tile/data.h"		// For TILE_SIZE
#include "util/math.h"		// For clamp()
#include "video.h"		// For screen dimensions

// The number of pixels the camera can move horizontally or vertically each frame
#define	CAM_SPEED	20

// Initialize global game camera
GameCamera g_cam = {
	.x = 0,
	.y = 0,
	.xshift = 0,
	.yshift = 0,
	.scroll_stop = false,
	.xstop = false,
	.ystop = false
};

// Updates the camera's xshift and yshift based on its position & x/y stop
void cam_update_shifts(void)
{
	if (g_cam.xstop)
		g_cam.xshift = clamp(-g_cam.x + g_screen_width / 2, -g_map.width * TILE_SIZE + g_screen_width, 0);
	else
		g_cam.xshift = -g_cam.x + g_screen_width / 2;
	if (g_cam.ystop)
		g_cam.yshift = clamp(-g_cam.y + g_screen_height / 2, -g_map.height * TILE_SIZE + g_screen_height, 0);
	else
		g_cam.yshift = -g_cam.y + g_screen_height / 2;
}

// Updates the camera's xstop and ystop based on the screen dimensions
void cam_update_limits(void)
{
	if (g_cam.scroll_stop)
	{
		g_cam.xstop = g_screen_width <= g_map.width * TILE_SIZE;
		g_cam.ystop = g_screen_height <= g_map.height * TILE_SIZE;
	}
	else
	{
		g_cam.xstop = false;
		g_cam.ystop = false;
	}
	ent_cloud_update_count();
}

// Uses arrow keys to move the camera
void cam_update_position(void)
{
	if (g_key_state[SDL_SCANCODE_W])
		g_cam.y -= CAM_SPEED;
	if (g_key_state[SDL_SCANCODE_S])
		g_cam.y += CAM_SPEED;
	if (g_key_state[SDL_SCANCODE_A])
		g_cam.x -= CAM_SPEED;
	if (g_key_state[SDL_SCANCODE_D])
		g_cam.x += CAM_SPEED;
	g_cam.x = clamp(g_cam.x, 0, g_map.width * TILE_SIZE);
	g_cam.y = clamp(g_cam.y, 0, g_map.height * TILE_SIZE);
}

// Sets the dimensions for all visible tiles of any type to be drawn to the screen
void cam_get_tile_dimensions(int *left, int *right, int *top, int *bottom)
{
	*left = -g_cam.xshift / TILE_SIZE;
	*right = *left + g_screen_width / TILE_SIZE + 2;
	*top = -g_cam.yshift / TILE_SIZE;
	*bottom = *top + g_screen_height / TILE_SIZE + 2;

	if (*left < 0)
		*left = 0;
	if (*right > g_map.width)
		*right = g_map.width;
	if (*top < 0)
		*top = 0;
	if (*bottom > g_map.height)
		*bottom = g_map.height;
}

// Returns true if the camera can see point (x, y) in the game world
bool cam_can_see_point(int x, int y, int leeway)
{
	const int xdist = abs(g_cam.x - x);
	const int ydist = abs(g_cam.y - y);
	const int xdist_max = g_screen_width / 2 + leeway;
	const int ydist_max = g_screen_height / 2 + leeway;
	return xdist <= xdist_max && ydist <= ydist_max;
}
