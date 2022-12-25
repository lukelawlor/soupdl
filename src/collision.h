/*
 * collision.h contains functions for handling collision with things in the game world.
 *
 * This file will automatically include tile/data.h for tile collision functions.
 */

#ifndef	COLLISION_H
#define	COLLISION_H

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "tile/data.h"		

#include "entity/item.h"
#include "entity/fireball.h"
#include "entity/evilball.h"

// Returns true if there is a collision between two rectangles
bool check_rect(SDL_Rect *r1, SDL_Rect *r2);

// Returns the tile id of the tile at an x and y position in the tile map
TileId check_tile_point(int x, int y);

/*
 * This function returns true if a tile with the id passed to it collides with a rectangle.
 *
 * NOTE: This function only checks the four corners of the rectangles passed. This means that it may not return accurate values if the rectangle has a width or height that is greater than or equal to TILE_SIZE (defined in tile/data.h)
 */
bool check_tile_rect_id(SDL_Rect *rect, TileId id);

/*
 * This function returns the tile id of a tile that a rectangle collides with. The tile found must have the same flags as the flags passed to the function. If no desired tiles are found, TILE_AIR (zero) is returned.
 *
 * NOTE: This function only checks the four corners of the rectangles passed. This means that it may not return accurate values if the rectangle has a width or height that is greater than or equal to TILE_SIZE (defined in tile/data.h)
 */
TileId check_tile_rect_flags(SDL_Rect *rect, TileFlags flags);

// These functions return pointers to entities that intersect with the rectangle rect
// If no collision occurs, they return NULL
EntITEM *check_ent_item(SDL_Rect *rect);
EntFIREBALL *check_ent_fireball(SDL_Rect *rect);
EntEVILBALL *check_ent_evilball(SDL_Rect *rect);

#endif
