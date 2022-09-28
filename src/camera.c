/*
 * camera.c contains functions for updating the camera's components.
 */

#include "util.h"	// For clamp
#include "video.h"	// For screen dimensions
#include "input.h"	// For g_key_state
#include "tile/data.h"	// For room dimensions
#include "camera.h"

// The number of pixels the camera can move horizontally or vertically each frame
#define	CAM_SPEED	12

// Initialize global game camera
GameCamera g_cam = {
	.x = 0,
	.y = 0,
	.xshift = 0,
	.yshift = 0,
	.xstop = false,
	.ystop = false
};

// Updates the camera's xshift and yshift based on its position & x/y stop
void cam_update_shifts(void)
{
	if (g_cam.xstop)
		g_cam.xshift = clamp(-g_cam.x + g_screen_width / 2, -g_room_width * TILE_SIZE + g_screen_width, 0);
	else
		g_cam.xshift = -g_cam.x + g_screen_width / 2;
	if (g_cam.ystop)
		g_cam.yshift = clamp(-g_cam.y + g_screen_height / 2, -g_room_height * TILE_SIZE + g_screen_height, 0);
	else
		g_cam.yshift = -g_cam.y + g_screen_height / 2;
}

// Updates the camera's xstop and ystop based on the screen dimensions
void cam_update_limits(void)
{
	g_cam.xstop = g_screen_width <= g_room_width * TILE_SIZE;
	g_cam.ystop = g_screen_height <= g_room_height * TILE_SIZE;
}

// Uses arrow keys to move the camera
void cam_update_position(void)
{
	if (g_key_state[SDL_SCANCODE_UP])
		g_cam.y -= CAM_SPEED;
	if (g_key_state[SDL_SCANCODE_DOWN])
		g_cam.y += CAM_SPEED;
	if (g_key_state[SDL_SCANCODE_LEFT])
		g_cam.x -= CAM_SPEED;
	if (g_key_state[SDL_SCANCODE_RIGHT])
		g_cam.x += CAM_SPEED;
}
