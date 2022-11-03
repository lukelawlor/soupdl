/*
 * outside.c contains functions for drawing tiles outside the map in the game world.
 */

#include <math.h>	// For ceil()

#include <SDL2/SDL.h>	// For drawing textures

#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "data.h"

/*
 * Fills a rectangle on the screen with a tile texture
 *
 * This function takes tile coordinates as arguments, not screen pixels.
 */
static inline void tile_draw_outside_rect(int left, int right, int top, int bottom, SDL_Rect *srect);

// Draw all outside tiles
void tile_draw_outside_all()
{
	// Don't draw air, because that would be the same as drawing nothing
	if (g_tile_outside == TILE_AIR)
		return;
	
	// Top outside tiles dimensions
	int ott_left, ott_right, ott_top, ott_bottom;
	ott_left = -g_cam.xshift / TILE_SIZE;
	if (g_cam.xshift > 0)
		ott_left -= 1;
	ott_right = ott_left + (ceil((double) g_screen_width / TILE_SIZE)) + 1;
	ott_top = -ceil((double) g_cam.yshift / TILE_SIZE);
	ott_bottom = 0;
	if (g_cam.yshift > g_screen_height)
		ott_bottom = -(g_cam.yshift - g_screen_height) / TILE_SIZE;

	// Bottom outside tiles dimensions
	int otb_left, otb_right, otb_top, otb_bottom;
	otb_left = ott_left;
	otb_right = ott_right;
	otb_top = g_room_height;
	if (-g_cam.yshift > g_room_height * TILE_SIZE)
	{
		otb_top += (-g_cam.yshift - g_room_height * TILE_SIZE) / TILE_SIZE;
		otb_bottom = otb_top + ceil((double) g_screen_height / TILE_SIZE) + 1;
	}
	else
	{
		otb_bottom = otb_top + ceil((double) (g_screen_height - g_room_height * TILE_SIZE - g_cam.yshift) / TILE_SIZE);
	}

	// Left outside tiles dimensions
	int otl_left, otl_right, otl_top, otl_bottom;
	otl_left = ott_left;
	otl_right = 0;
	if (g_cam.xshift > g_screen_width)
		otl_right = ott_right;
	otl_top = 0;
	if (g_cam.yshift < 0)
		otl_top = -g_cam.yshift / TILE_SIZE;
	otl_bottom = g_room_height;
	if (g_cam.yshift + g_room_height * TILE_SIZE > g_screen_height)
		otl_bottom = g_room_height - ((g_cam.yshift + g_room_height * TILE_SIZE) - g_screen_height) / TILE_SIZE;

	// Right outside tiles dimensions
	int otr_left, otr_right, otr_top, otr_bottom;
	otr_left = g_room_width;
	if (-g_cam.xshift > g_room_width * TILE_SIZE)
		otr_left += (-g_cam.xshift - g_room_width * TILE_SIZE) / TILE_SIZE;
	otr_right = ott_right;
	otr_top = otl_top;
	otr_bottom = otl_bottom;

	// Draw the tiles
	SDL_Rect srect = {g_tile_property[g_tile_outside].spoint.x, g_tile_property[g_tile_outside].spoint.y, 32, 32};
	tile_draw_outside_rect(ott_left, ott_right, ott_top, ott_bottom, &srect);
	tile_draw_outside_rect(otb_left, otb_right, otb_top, otb_bottom, &srect);
	tile_draw_outside_rect(otl_left, otl_right, otl_top, otl_bottom, &srect);
	tile_draw_outside_rect(otr_left, otr_right, otr_top, otr_bottom, &srect);
}

/*
 * Fills a rectangle on the screen with a tile texture
 *
 * This function takes tile coordinates as arguments, not screen pixels.
 */
static inline void tile_draw_outside_rect(int left, int right, int top, int bottom, SDL_Rect *srect)
{
	// Draw the tiles
	SDL_Rect drect = {.w = TILE_SIZE, .h = TILE_SIZE};
	for (int y = top; y < bottom; y++)
	{
		for (int x = left; x < right; x++)
		{
			drect.x = x * TILE_SIZE + g_cam.xshift;
			drect.y = y * TILE_SIZE + g_cam.yshift;
			SDL_RenderCopy(g_renderer, tex_tileset, srect, &drect);
		}
	}
}
