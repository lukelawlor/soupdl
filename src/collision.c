/*
 * collision.c contains functions for handling collisions between different things in the game world.
 */

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "collision.h"
#include "tile/data.h"	
#include "map.h"

#include "entity/item.h"	
#include "entity/fireball.h"	
#include "entity/evilball.h"
#include "entity/root.h"

// Returns true if there is a collision between two rectangles
bool check_rect(const SDL_Rect *r1, const SDL_Rect *r2)
{
	return r1->x < r2->x + r2->w && r1->x + r1->w > r2->x && r1->y < r2->y + r2->h && r1->y + r1->h > r2->y;
}

// Returns the tile space of the tile at an x and y position in the world
TileId check_tile_point(const int x, const int y)
{
	// x and y to check in the map
	int cx = x / TILE_SIZE;
	int cy = y / TILE_SIZE;
	
	if (	x < 0 || cx < 0 || cx >= g_map.width ||
		y < 0 || cy < 0 || cy >= g_map.height
		)
		return g_tile_outside;
	
	return g_tile_map[cy][cx];
}

/*
 * This function returns true if a tile with the id passed to it collides with a rectangle.
 *
 * NOTE: This function only checks the four corners of the rectangles passed. This means that it may not return accurate values if the rectangle has a width or height that is greater than or equal to TILE_SIZE (defined in tile/data.h)
 */
bool check_tile_rect_id(const SDL_Rect *rect, const TileId id)
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
TileId check_tile_rect_flags(const SDL_Rect *rect, const TileFlags flags)
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

// Returns a pointer to an entity if the rectangle rect instersects with one, otherwise NULL is returned
EntITEM *check_ent_item(const SDL_Rect *rect)
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

// Returns a pointer to an entity if the rectangle rect instersects with one, otherwise NULL is returned
EntFIREBALL *check_ent_fireball(const SDL_Rect *rect)
{
	// Fireball rectangle
	SDL_Rect frect = {.w = 16, .h = 16};

	EntFIREBALL *fireball = g_er[ENT_ID_FIREBALL]->e;
	for (int i = 0; i < g_er[ENT_ID_FIREBALL]->len; i++)
	{
		frect.x = fireball->x - 8;
		frect.y = fireball->y - 8;
		if (check_rect(&frect, rect) && fireball->base.status != ENT_STAT_DEL)
			return fireball;
		fireball++;
	}
	return NULL;
}

// Returns a pointer to an entity if the rectangle rect instersects with one, otherwise NULL is returned
EntEVILBALL *check_ent_evilball(const SDL_Rect *rect)
{
	// Evilball rectangle
	SDL_Rect crect = {.w = 16, .h = 16};

	EntEVILBALL *e = g_er[ENT_ID_EVILBALL]->e;
	for (int i = 0; i < g_er[ENT_ID_EVILBALL]->len; i++)
	{
		crect.x = e->x - 8;
		crect.y = e->y - 8;
		if (check_rect(&crect, rect))
			return e;
		e++;
	}
	return NULL;
}
