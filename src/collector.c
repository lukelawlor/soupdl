/*
 * collector.c contains functions a for interfacing with the collector.
 *
 * See collector.h for a detailed overview of the collector.
 */

#include <stdlib.h>	// For free()

#include "map.h"	// For MAP_PATH_MAX and map_free()
#include "collector.h"

// Global collector map data list
ColMapDataList g_col;

// Attempts to add *cmd to g_col
// If the map was already added to g_col, the map is not added and nonzero is returned
// Else, the map is added to g_col and zero is returned
// To test if a map is already added, this function checks if *cmd and a map in g_col have the same values for:
//	width
// 	height
// 	path
ColMapIndex col_add_map(ColMapData *cmd)
{
	// Check if there is room in g_col
	if (g_col.len >= COL_MAP_MAX)
	{
		// There is no room
		return -1;
	}

	// Check if the map was already added
	for (int i = 0; i < g_col.len; ++i)
	{
		// Map data being examined from the list
		ColMapData *cmd_list = &g_col.data[i];

		if (
			cmd->width == cmd_list->width &&
			cmd->height == cmd_list->height &&
			strncmp(cmd->path, cmd_list->path, MAP_PATH_MAX) == 0)
		{
			// Map was already added
			return i;
		}
	}

	// Map wasn't added yet, add it
	g_col.data[g_col.len++] = *cmd;
	return -1;
}

// Frees all malloc-obtained memory held in the collector
void col_free(void)
{
	for (int i = 0; i < g_col.len; ++i)
	{
		free(g_col.data[i].path);
		map_free(g_col.data[i].height, g_col.data[i].map);
	}
}
