/*
 * data.c contains functions for manipulating tile data.
 */

#include <stdio.h>

#include "../camera.h"		// For updating the camera when the room dimensions change
#include "../entity/all.h"	// For creating entities from map files
#include "data.h"

// Array containing the metadata of each tile type
static const TileMetadata g_tile_metadata[TILE_MAX] = {
	// TILE_AIR (NOTE: spoint doesn't matter here since air is never drawn)
	{.map_char='.',	.spoint={0,0},.flags=0,.name="Air"},

	// TILE_STONE
	{.map_char='s',	.spoint={TILE_SIZE*0,TILE_SIZE*0},.flags=TFLAG_SOLID,.name="Stone"},

	// TILE_LIME
	{.map_char='l',	.spoint={TILE_SIZE*1,TILE_SIZE*0},.flags=TFLAG_SOLID,.name="Lime..?"},

	// TILE_IRON
	{.map_char='o',	.spoint={TILE_SIZE*2,TILE_SIZE*0},.flags=TFLAG_SOLID,.name="Iron Block"},

	// TILE_SPIKE
	{.map_char='x',	.spoint={TILE_SIZE*3,TILE_SIZE*0},.flags=TFLAG_SPIKE,.name="Spikes"},

	// TILE_STRING
	{.map_char='|',	.spoint={TILE_SIZE*0,TILE_SIZE*1},.flags=0,.name="Pointless string"},

	// TILE_GRASS
	{.map_char=';',	.spoint={TILE_SIZE*1,TILE_SIZE*1},.flags=0,.name="Grass"},
};

// Constant pointer to the first index of tile_property_list
const TileMetadata *const g_tile_md = g_tile_metadata;

// Room width and height in tiles, not pixels
int g_room_width = 1;
int g_room_height = 1;

// 2d array containing TileId objects
TileId **g_tile_map = NULL;

// Tile type for tiles outside the map
TileId g_tile_outside = TILE_LIME;
