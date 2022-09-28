/*
 * collision.c contains tile collision functions.
 */

#include "data.h"
#include "collision.h"

// Returns the tile type of the tile at an x and y position in the world
tile_space_tile tile_check_point(int x, int y)
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
