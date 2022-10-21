/*
 * editor.c contains map editor code.
 */

#include <stdbool.h>
#include <stdio.h>

#include "../util.h"		// For MIN
#include "../tile/data.h"	// For room dimensions, TileId, and TILE_SIZE
#include "../entity/all.h"	// For EntId
#include "../map.h"		// For map_alloc and map_free
#include "editor.h"
#include "draw.h"

// Pointer to 2d array containing entity tiles
EntTile **g_ent_map;

// Initializes the map editor
int maped_init(void)
{
	if ((g_ent_map = (EntTile **) map_alloc(sizeof(EntTile))) == NULL)
	{
		fprintf(stderr, "failed to allocate mem for entity map\n");
		return 1;
	}

	// Fill set all entity spaces to inactive
	for (int y = 0; y < g_room_height; y++)
		for (int x = 0; x < g_room_width; x++)
			g_ent_map[x][y].active = false;
	
	// Initialize textures
	maped_init_ent_tile_tex();

	return 0;
}

// Resizes the map by (width_inc, height_inc), and returns nonzero on error
int maped_resize_map(int width_inc, int height_inc)
{
	g_room_width += width_inc;
	g_room_height += height_inc;

	// Automatically update camera limits to reflect
	TileId **temp_tile_map;
	EntTile **temp_ent_map;
	if ((temp_tile_map = (TileId **) map_alloc(sizeof(TileId))) == NULL)
	{
		fprintf(stderr, "failed to allocate temporary tile map\n");
		return 1;
	}
	if ((temp_ent_map = (EntTile **) map_alloc(sizeof(EntTile))) == NULL)
	{
		map_free((void **) temp_tile_map);
		fprintf(stderr, "failed to allocate temporary entity map\n");
		return 1;
	}

	// Max width & height to copy over
	int width_max, height_max;
	if (width_inc > 0)
		width_max = g_room_width - width_inc;
	else
		width_max = g_room_width;
	if (height_inc > 0)
		height_max = g_room_height - height_inc;
	else
		height_max = g_room_height;

	// Copy map data into temp maps (TODO: use memcpy for this)
	for (int x = 0; x < width_max; x++)
	{
		for (int y = 0; y < height_max; y++)
		{
			temp_tile_map[x][y] = g_tile_map[x][y];
			temp_ent_map[x][y] = g_ent_map[x][y];
		}
	}

	// Set newly allocated tiles to default values

	#define	SET_DEFAULT_TILE()	temp_tile_map[x][y] = TILE_AIR; \
					temp_ent_map[x][y] = (EntTile) {.active = false}

	for (int x = width_max; x < g_room_width; x++)
	{
		for (int y = 0; y < g_room_height; y++)
		{
			SET_DEFAULT_TILE();
		}
	}
	for (int y = height_max; y < g_room_height; y++)
	{
		for (int x = 0; x < g_room_width; x++)
		{
			SET_DEFAULT_TILE();
		}
	}

	// A little hack-y, but room width & height are manipulated to properly free the old maps
	g_room_width -= width_inc;
	g_room_height -= height_inc;
	map_free((void **) g_tile_map);
	map_free((void **) g_ent_map);
	g_room_width += width_inc;
	g_room_height += height_inc;

	g_tile_map = temp_tile_map;
	g_ent_map = temp_ent_map;
	return 0;
}
