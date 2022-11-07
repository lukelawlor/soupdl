/*
 * c_body.h contains the EcmBody struct and its associted functions.
 *
 * The EcmBody struct is used to represent an entity body in the world. It contains position, hitbox, and movement speed data.
 */

#ifndef	ENTITY_C_BODY_H
#define	ENTITY_C_BODY_H

#include <stdbool.h>

#include <SDL2/SDL.h>

typedef struct{
	// Position
	double x, y;

	// Horizontal and vertical speed
	double hsp, vsp;

	// Gravity
	double grv;

	// Hitbox rectangle (The x and y of this rectangle are relative to the actual position of the entity, the w and h are the width and height of the hitbox)
	SDL_Rect hrect;
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
		b->x + b->hrect.x,
		b->y + b->hrect.y,
		b->hrect.w,
		b->hrect.h,
	};
	return crect;
}

#endif
