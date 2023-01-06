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

	// Width and height
	int w, h;

	// Horizontal and vertical speed
	float hsp, vsp;

	// Gravity
	float grv;
} EcmBody;

// Checks for an entity body collision with a solid tile
bool ecm_body_tile_collide(EcmBody *b, double xshift, double yshift);

// Moves an entity body horizontally, returns true if a tile is collided with
bool ecm_body_move_hori(EcmBody *b);

// Moves an entity body vertically, returns true if a tile is collided with
bool ecm_body_move_vert(EcmBody *b);

// Get rectangle used to check for collisions
#define	ECM_BODY_GET_CRECT(b)	{b.x, b.y, b.w, b.h}

#endif
