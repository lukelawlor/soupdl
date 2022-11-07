/*
 * c_body.h contains the EcmBody struct and its associted functions.
 *
 * The EcmBody struct is used to represent an entity body in the world. It contains position, hitbox, and movement speed data.
 */

#ifndef	ENTITY_C_BODY_H
#define	ENTITY_C_BODY_H

#include <stdbool.h>

typedef struct{
	// Position
	double x, y;

	// Width and height
	int w, h;

	// Horizontal and vertical speed
	double hsp, vsp;

	// Gravity
	double grv;
} EcmBody;

// Checks for an entity body collision with a solid tile
bool ecm_body_tile_collide(EcmBody *b, double xshift, double yshift);

// Moves an entity body horizontally, returns true if a tile is collided with
bool ecm_body_move_hori(EcmBody *b);

// Moves an entity body vertically, returns true if a tile is collided with
bool ecm_body_move_vert(EcmBody *b);

// Get rectangle used to check for collisions
static inline SDL_Rect ecm_body_get_crect(EcmBody *b)
{
	SDL_Rect crect = {
		b->x,
		b->y,
		b->w,
		b->h,
	};
	return crect;
}

#endif
