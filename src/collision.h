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

// Returns a pointer to an entity if there is rectangular collision between the rectangle passed to the function and any item entity that currently exists, otherwise returns NULL
EntItem *check_ent_item(SDL_Rect rect);

#endif
