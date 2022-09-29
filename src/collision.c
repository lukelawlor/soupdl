/*
 * collision.c contains functions for handling collisions between different things in the game world.
 */

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "tile/data.h"		// For tile collision
#include "entity/item.h"	// For item collision
#include "collision.h"

// Returns true if there is a collision between two rectangles
bool check_rect(SDL_Rect *r1, SDL_Rect *r2)
{
	return r1->x < r2->x + r2->w && r1->x + r1->w > r2->x && r1->y < r2->y + r2->h && r1->y + r1->h > r2->y;
}

// Returns the tile type of the tile at an x and y position in the world
tile_space_tile check_tile_point(int x, int y)
{
	// x and y to check in the map
	int cx = x / TILE_SIZE;
	int cy = y / TILE_SIZE;
	
	if (	x < 0 || cx < 0 || cx >= g_room_width ||
		y < 0 || cy < 0 || cy >= g_room_height
		)
		return g_tile_outside;
	
	return g_tile_space[cx][cy]->tile;
}

// Returns a pointer to an entity if there is rectangular collision between the rectangle passed to the function and any item entity that currently exists, otherwise returns NULL
EntItem *check_ent_item(SDL_Rect rect)
{
	// Item rectangle
	SDL_Rect irect = {.w = 16, .h = 16};

	for (int i = 0; i < ENT_LIST_MAX; i++)
	{
		EntItem *item;
		if ((item = ent_item + i)->d.exists)
		{
			irect.x = item->x - 8;
			irect.y = item->y - 8;
			if (check_rect(&irect, &rect))
				return item;
		}
	}
	return NULL;
}
