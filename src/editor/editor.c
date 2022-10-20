/*
 * editor.c contains map editor code.
 */

#include <stdbool.h>

#include "../tile/data.h"	// for TILE_SIZE
#include "../entity/all.h"	// for EntId
#include "../map.h"		// for map_alloc
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
