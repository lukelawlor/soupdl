/*
 * camera.c contains functions for updating the camera's components.
 */

#include "util/math.h"	// For clamp()
#include "video.h"	// For screen dimensions
#include "input.h"	// For g_key_state
#include "tile/data.h"	// For room dimensions and TILE_SIZE
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
	// x and y stop are temporarily disabled
	/*
	g_cam.xstop = g_screen_width <= g_room_width * TILE_SIZE;
	g_cam.ystop = g_screen_height <= g_room_height * TILE_SIZE;
	*/
	g_cam.xstop = false;
	g_cam.ystop = false;
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
	g_cam.x = clamp(g_cam.x, 0, g_room_width * TILE_SIZE);
	g_cam.y = clamp(g_cam.y, 0, g_room_height * TILE_SIZE);
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
	if (*right > g_room_width)
		*right = g_room_width;
	if (*top < 0)
		*top = 0;
	if (*bottom > g_room_height)
		*bottom = g_room_height;
}
