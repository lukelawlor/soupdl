/*
 * data.c contains functions for manipulating tile data.
 */

#include <stdio.h>

#include "../camera.h"		// For updating the camera when the room dimensions change
#include "../entity/all.h"	// For creating entities from map files
#include "data.h"

// Array containing the properties of different types of tiles
static const TileProperty g_tile_property_list[TILE_MAX] = {
	// TILE_AIR (NOTE: spoint doesn't matter here since air is never drawn)
	{.spoint={0,32},.flags=0},

	// TILE_STONE
	{.spoint={0,0},.flags=TFLAG_SOLID},

	// TILE_LIME
	{.spoint={TILE_SIZE*1,0},.flags=TFLAG_SOLID},

	// TILE_IRON
	{.spoint={TILE_SIZE*2,0},.flags=TFLAG_SOLID},

	// TILE_SPIKE
	{.spoint={TILE_SIZE*3,0},.flags=TFLAG_SPIKE},

	// TILE_STRING
	{.spoint={0,TILE_SIZE*1},.flags=0},
};

// Constant pointer to the first index of tile_property_list
const TileProperty *const g_tile_property = g_tile_property_list;

// Room width and height in tiles, not pixels
int g_room_width = 1;
int g_room_height = 1;

// 2d array containing TileId objects
TileId **g_tile_map = NULL;

// Tile type for tiles outside the map
TileId g_tile_outside = TILE_LIME;
