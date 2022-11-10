/*
 * collision.c contains functions for handling collisions between different things in the game world.
 */

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "tile/data.h"		// For tile collision
#include "entity/item.h"	// For item collision
#include "entity/fireball.h"	// For fireball collision
#include "entity/root.h"
#include "collision.h"

// Returns true if there is a collision between two rectangles
bool check_rect(SDL_Rect *r1, SDL_Rect *r2)
{
	return r1->x < r2->x + r2->w && r1->x + r1->w > r2->x && r1->y < r2->y + r2->h && r1->y + r1->h > r2->y;
}

// Returns the tile space of the tile at an x and y position in the world
TileId check_tile_point(int x, int y)
{
	// x and y to check in the map
	int cx = x / TILE_SIZE;
	int cy = y / TILE_SIZE;
	
	if (	x < 0 || cx < 0 || cx >= g_room_width ||
		y < 0 || cy < 0 || cy >= g_room_height
		)
		return g_tile_outside;
	
	return g_tile_map[cx][cy];
}

/*
 * This function returns true if a tile with the id passed to it collides with a rectangle.
 *
 * NOTE: This function only checks the four corners of the rectangles passed. This means that it may not return accurate values if the rectangle has a width or height that is greater than or equal to TILE_SIZE (defined in tile/data.h)
 */
bool check_tile_rect_id(SDL_Rect *rect, TileId id)
{
	// Top left corner
	if (check_tile_point(rect->x, rect->y) == id)
		return true;

	// Top right corner
	if (check_tile_point(rect->x + rect->w, rect->y) == id)
		return true;

	// Bottom left corner
	if (check_tile_point(rect->x, rect->y + rect->h) == id)
		return true;

	// Bottom right corner
	if (check_tile_point(rect->x + rect->w, rect->y + rect->h) == id)
		return true;
	
	return false;
}

/*
 * This function returns the tile id of a tile that a rectangle collides with. The tile found must have the same flags as the flags passed to the function. If no desired tiles are found, TILE_AIR (zero) is returned.
 *
 * NOTE: This function only checks the four corners of the rectangles passed. This means that it may not return accurate values if the rectangle has a width or height that is greater than or equal to TILE_SIZE (defined in tile/data.h)
 */
TileId check_tile_rect_flags(SDL_Rect *rect, TileFlags flags)
{
	TileId id;

	// Top left corner
	if (g_tile_md[(id = check_tile_point(rect->x, rect->y))].flags & flags)
		return id;

	// Top right corner
	if (g_tile_md[(id = check_tile_point(rect->x + rect->w, rect->y))].flags & flags)
		return id;

	// Bottom left corner
	if (g_tile_md[(id = check_tile_point(rect->x, rect->y + rect->h))].flags & flags)
		return id;

	// Bottom right corner
	if (g_tile_md[(id = check_tile_point(rect->x + rect->w, rect->y + rect->h))].flags & flags)
		return id;
	
	return TILE_AIR;
}

// Returns a pointer to an entity if there is rectangular collision between the rectangle passed to the function and any item entity that currently exists, otherwise returns NULL
EntITEM *check_ent_item(SDL_Rect *rect)
{
	// Item rectangle
	SDL_Rect irect = {.w = 16, .h = 16};

	EntITEM *item = g_er[ENT_ID_ITEM]->e;
	for (int i = 0; i < g_er[ENT_ID_ITEM]->len; i++)
	{
		irect.x = item->x - 8;
		irect.y = item->y - 8;
		if (check_rect(&irect, rect))
			return item;
		item++;
	}
	return NULL;
}

// Returns a pointer to an entity if there is rectangular collision between the rectangle passed to the function and any fireball entity that currently exists, otherwise returns NULL (TODO: make this not just repeat code from the function above)
EntFIREBALL *check_ent_fireball(SDL_Rect *rect)
{
	// Fireball rectangle
	SDL_Rect frect = {.w = 16, .h = 16};

	EntFIREBALL *fireball = g_er[ENT_ID_FIREBALL]->e;
	for (int i = 0; i < g_er[ENT_ID_FIREBALL]->len; i++)
	{
		frect.x = fireball->x - 8;
		frect.y = fireball->y - 8;
		if (check_rect(&frect, rect))
			return fireball;
		fireball++;
	}
	return NULL;
}
