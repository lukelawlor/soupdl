/*
 * draw.c handles the drawing of tiles within the game map.
 *
 * See outside.c for drawing tiles outside the map.
 */

#include <SDL2/SDL.h>	// For rendering textures

#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "data.h"
#include "draw.h"

// Draw all tiles in the game map in the view of the camera
void tile_draw_all()
{
	// Tile drawing dimensions
	int tile_left, tile_right, tile_top, tile_bottom;
	tile_left = -g_cam.xshift / TILE_SIZE;
	tile_right = tile_left + g_screen_width / TILE_SIZE + 2;
	tile_top = -g_cam.yshift / TILE_SIZE;
	tile_bottom = tile_top + g_screen_height / TILE_SIZE + 2;

	if (tile_left < 0)
		tile_left = 0;
	if (tile_right > g_room_width)
		tile_right = g_room_width;
	if (tile_top < 0)
		tile_top = 0;
	if (tile_bottom > g_room_height)
		tile_bottom = g_room_height;

	// Draw all tiles in a loop
	SDL_Rect drect = {.w = TILE_SIZE, .h = TILE_SIZE};
	SDL_Rect srect = {.w = TILE_SIZE, .h = TILE_SIZE};
	for (int y = tile_top; y < tile_bottom; y++)
	{
		for (int x = tile_left; x < tile_right; x++)
		{
			TileId ti = g_tile_map[x][y];

			// Don't draw air
			if (ti == TILE_AIR)
				continue;

			// Getting sprite of tile
			const TileProperty *tp = &g_tile_property[ti];
			srect.x = tp->spoint.x;
			srect.y = tp->spoint.y;

			// Getting tile rotation
			double rot = 0.0f;
			if (tp->flags & TFLAG_ROT1)
				rot = 90.0f;
			else if (tp->flags & TFLAG_ROT2)
				rot = 180.0f;
			else if (tp->flags & TFLAG_ROT3)
				rot = 270.0f;

			drect.x = x * TILE_SIZE + g_cam.xshift;
			drect.y = y * TILE_SIZE + g_cam.yshift;
			SDL_RenderCopyEx(g_renderer, tex_tileset, &srect, &drect, rot, NULL, SDL_FLIP_NONE);
		}
	}
}
