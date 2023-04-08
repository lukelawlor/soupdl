/*
 * data.c contains functions for manipulating tile data.
 */

#include <stdio.h>

#include "../camera.h"		// For updating the camera when the room dimensions change
#include "../entity/all.h"	// For creating entities from map files
#include "data.h"

// Array containing the metadata of each tile type
static const TileMetadata g_tile_metadata[TILE_MAX] = {
	// TILE_AIR (NOTE: spoint doesn't matter here since this tile is never drawn)
	{.map_char='.',	.spoint={0,0},.flags=0,.name="Air"},

	// TILE_STONE
	{.map_char='s',	.spoint={TILE_SIZE*0,TILE_SIZE*0},.flags=TFLAG_SOLID,.name="Stone"},

	// TILE_LIME
	{.map_char='l',	.spoint={TILE_SIZE*1,TILE_SIZE*0},.flags=TFLAG_SOLID,.name="Lime..?"},

	// TILE_LIMESTONE
	{.map_char='L',	.spoint={TILE_SIZE*0,TILE_SIZE*2},.flags=TFLAG_SOLID,.name="Limestone"},

	// TILE_IRON
	{.map_char='o',	.spoint={TILE_SIZE*2,TILE_SIZE*0},.flags=TFLAG_SOLID,.name="Iron Block"},

	// TILE_IRONBACK
	{.map_char='O',	.spoint={TILE_SIZE*0,TILE_SIZE*3},.flags=0,.name="Iron Block Background"},

	// TILE_SPIKE_UP
	{.map_char='x',	.spoint={TILE_SIZE*3,TILE_SIZE*0},.flags=TFLAG_SPIKE,.name="Spikes Pointing Up"},

	// TILE_SPIKE_DOWN
	{.map_char='d',	.spoint={TILE_SIZE*3,TILE_SIZE*0},.flags=TFLAG_SPIKE|TFLAG_ROT2,.name="Spikes Pointing Down"},

	// TILE_SPIKE_LEFT
	{.map_char='{',	.spoint={TILE_SIZE*3,TILE_SIZE*0},.flags=TFLAG_SPIKE|TFLAG_ROT3,.name="Spikes Pointing Left"},

	// TILE_SPIKE_RIGHT
	{.map_char='}',	.spoint={TILE_SIZE*3,TILE_SIZE*0},.flags=TFLAG_SPIKE|TFLAG_ROT1,.name="Spikes Pointing Right"},

	// TILE_SPIKE_BALL
	{.map_char='X',	.spoint={TILE_SIZE*2,TILE_SIZE*2},.flags=TFLAG_SPIKE,.name="Spike Ball"},

	// TILE_STRING
	{.map_char='|',	.spoint={TILE_SIZE*0,TILE_SIZE*1},.flags=0,.name="Pointless string"},

	// TILE_GRASS
	{.map_char=';',	.spoint={TILE_SIZE*1,TILE_SIZE*1},.flags=0,.name="Grass"},

	// TILE_GRASS
	{.map_char=':',	.spoint={TILE_SIZE*1,TILE_SIZE*2},.flags=0,.name="Tallgrass"},

	// TILE_INVIS (NOTE: spoint doesn't matter here since this tile is never drawn)
	{.map_char='i',	.spoint={0,0},.flags=TFLAG_SOLID,.name="Solid Air"},

	// TILE_WOOD
	{.map_char='W',	.spoint={TILE_SIZE*1,TILE_SIZE*3},.flags=TFLAG_SOLID,.name="Wood planks"},

	// TILE_WOODBACK
	{.map_char='w',	.spoint={TILE_SIZE*2,TILE_SIZE*3},.flags=0,.name="Wood background"},

	// TILE_WINDOW
	{.map_char='#',	.spoint={TILE_SIZE*3,TILE_SIZE*3},.flags=0,.name="Wood window"},
};

// Constant pointer to the first index of tile_property_list
const TileMetadata *const g_tile_md = g_tile_metadata;

// 2d array containing TileId objects
TileId **g_tile_map = NULL;

// Tile type for tiles outside the map
TileId g_tile_outside = TILE_LIME;
