/*
 * collision.h contains functions for handling collision with things in the game world.
 *
 * This file will automatically include tile/data.h for tile collision functions.
 */

#ifndef	COLLISION_H
#define	COLLISION_H

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "tile/data.h"		// For tile collision
#include "entity/item.h"	// For item collision

// Returns true if there is a collision between two rectangles
bool check_rect(SDL_Rect *r1, SDL_Rect *r2);

// Returns the tile type of the tile at an x and y position in the world
tile_space_tile check_tile_point(int x, int y);

// Returns true if the rectangle passed to it collides with a solid tile
// This only checks the 4 corners of the rectangle and nowhere in between, meaning it may not work if the rectangle passed is >= TILE_SIZE in tile/data.h.
bool check_tile_rect(SDL_Rect *rect);

// Returns a pointer to an entity if there is rectangular collision between the rectangle passed to the function and any item entity that currently exists, otherwise returns NULL
EntItem *check_ent_item(SDL_Rect *rect);

#endif
