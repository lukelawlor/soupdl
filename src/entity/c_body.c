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
	if (ecm_body_tile_collide(b, b->hsp * g_ts, 0))
	{
		// Current hsp sign
		int csign = signf(b->hsp);

		// Number of loops
		int loops = 0;

		// Previous x value
		double previous_x = b->x;

		// Move closer to the tile until it is hit
		while (!ecm_body_tile_collide(b, csign, 0))
		{
			b->x += csign;
			
			// Break loop and print error if the loop continues for too long
			if (++loops > 100)
			{
				PERR();
				fprintf(stderr, "ecm_body_move_hori failed\n");
				b->x = previous_x;
				break;
			}
		}
		return true;
	}
	b->x += b->hsp * g_ts;
	return false;
}

// Moves an entity body vertically, returns true if a tile is collided with
bool ecm_body_move_vert(EcmBody *b)
{
	if (ecm_body_tile_collide(b, 0, b->vsp * g_ts))
	{
		// Current vsp sign
		int csign = signf(b->vsp);

		// Number of loops
		int loops = 0;

		// Previous y value
		double previous_y = b->y;

		// Move closer to the tile until it is hit
		while (!ecm_body_tile_collide(b, 0, csign))
		{
			b->y += csign;
			
			// Break loop and print error if the loop continues for too long
			if (++loops > 100)
			{
				PERR();
				fprintf(stderr, "ecm_body_move_vert failed\n");
				b->y = previous_y;
				break;
			}
		}
		return true;
	}
	b->y += b->vsp * g_ts;
	return false;
}
