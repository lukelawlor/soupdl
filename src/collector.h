/*
 * collector.h contains functions and types for interfacing with the collector.
 *
 * The collector is a system used to keep track of what entities should appear in what maps. A common use for it is to stop items that the player has already picked up from spawning again the next time the map loads.
 *
 * The collector stores data about maps in ColMapData structs, which store the following:
 * 	A string containing the file path to the map
 * 	The width and height of the map
 * 	Map data of entity tile ids
 *
 * The path, width, and height are used to check if new maps that are being added to the collector have already been added. The map data contains entity ids which are used to spawn entities if a map whose data has been added to the collector is loaded again.
 *
 * To stop picked up items from spawning again, the collector map data of a map can be edited to remove the entity tile id where the item entity is spawned from.
 */

#ifndef	COLLECTOR_H
#define	COLLECTOR_H

#include <stdbool.h>
#include <stdint.h>

#include "entity/tile.h"	// For EntTileId

// Maximum number of maps to remember
#define	COL_MAP_MAX	100

// Must be able to hold values in the range [-1, COL_MAP_MAX)
typedef int8_t ColMapIndex;

// Data for the collector about a map
typedef struct{
	// File path to the map
	// Length should be <= MAP_PATH_MAX
	char *path;

	// Dimensions of the map
	int width, height;

	// Map data of entity tile ids
	EntTileId **map;
} ColMapData;

typedef struct{
	// Array of map data
	ColMapData data[COL_MAP_MAX];

	// Index of currently loaded map
	ColMapIndex active_index;

	// Length of data
	int len;
} ColMapDataList;

// Global collector map data list
extern ColMapDataList g_col;

// Attempts to add *cmd to g_col
// If the map was already added to g_col, the map is not added, and an index to the duplicate map in g_col.data is returned
// Else, the map is added to g_col and -1 is returned
// To test if a map is already added, this function checks if *cmd and a map in g_col have the same values for:
// 	width
// 	height
// 	path
ColMapIndex col_add_map(ColMapData *cmd);

// Frees all malloc-obtained memory held in the collector
// Resets g_col.len to 0
void col_free(void);

#endif
