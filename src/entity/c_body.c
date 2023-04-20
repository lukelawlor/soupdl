/*
 * c_body.c contains functions that act on EcmBody structs.
 */

#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "../util/math.h"	// For signf()
#include "../tile/data.h"	// For tile flags
#include "../collision.h"	// check_tile_rect_flags()
#include "../error.h"
#include "../timestep.h"
#include "c_body.h"

// Checks for an entity body collision with a solid tile
bool ecm_body_tile_collide(EcmBody *b, double xshift, double yshift)
{
	// Collision rectangle
	SDL_Rect crect = {
		b->x + xshift,
		b->y + yshift,
		b->w,
		b->h,
	};

	return check_tile_rect_flags(&crect, TFLAG_SOLID);
}

// Moves an entity body horizontally, returns true if a tile is collided with
bool ecm_body_move_hori(EcmBody *b)
{
	// Check if there is a solid tile in front of us
	if (ecm_body_tile_collide(b, b->hsp * g_ts, 0))
	{
		// Current hsp sign
		int csign = signf(b->hsp);

		// Tile x coordinate of the tile we are moving into
		int tile_x = ((int) (b->x + b->hsp * g_ts)) / TILE_SIZE + 1;

		// Align the body with the tile based on the direction it's moving in
		if (csign == 1)
		{
			b->x = tile_x * TILE_SIZE - b->w - 1;
		}
		else if (csign == -1)
		{
			b->x = tile_x * TILE_SIZE + 1;
		}
		return true;
	}
	b->x += b->hsp * g_ts;
	return false;
}

// Moves an entity body vertically, returns true if a tile is collided with
bool ecm_body_move_vert(EcmBody *b)
{
	// Check if there is a solid tile in front of us
	if (ecm_body_tile_collide(b, 0, b->vsp * g_ts))
	{
		// Current vsp sign
		int csign = signf(b->vsp);

		// Tile y coordinate of the tile we are moving into
		int tile_y = ((int) (b->y + b->vsp * g_ts)) / TILE_SIZE + 1;
		// Align the body with the tile based on the direction it's moving in
		if (csign == 1)
		{
			b->y = tile_y * TILE_SIZE - b->h - 1;
		}
		else if (csign == -1)
		{
			b->y = tile_y * TILE_SIZE + 1;
		}
		return true;
	}
	b->y += b->vsp * g_ts;
	return false;
}
