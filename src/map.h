/*
 * map.h contains functions for loading and saving maps.
 *
 * A map consists of tile data, entity tile data, and options. They are stored as text files in the ../res/map directory which end in .map. From top to bottom in the text file, tile data is stored first, and then the options. Below is an example map file:
 *
 * sssssssssssss
 * sssssssssssss
 * ss.........ss
 * ss.....+++.ss
 * ss.p...+++.ss
 * sssssssssssss
 * sssssssssssss
 * >ot l
 * >r 2 2 3 2 i5
 *
 * Tiles are used to construct the terrain of the map. They are defined in tile/data.c. Entity tiles are used to spawn entities. They are defined in entity/tile.c. Both types of tiles are represented as singular ASCII characters in map files, making it possible to edit maps in a text editor if it's preferred to using the visual editor.
 *
 * Options are text commands listed at the bottom of map files that specify map settings. They are stored on lines starting with the map option symbol (>). The first string of text after the symbol and before the first space is the option name. Following that are the option arguments, each separated by spaces.
 *
 * The r option can be used to create void rectangles. These are rectangles that affect the spawn values of entity tiles that lie within them. Void rectangles can hold integer or string values, which are passed to entity tile spawner functions (defined in entity/tile.c). The use of void rectangle values varies between each entity tile spawner function. The type for void rectangles is in void_rect.h.
 *
 * The process for loading a map is as follows:
 * 	1. The map file is opened with fopen()
 * 	2. The ascii representation of the tile data is read into the map_data variable
 * 	3. Old map data is freed and reallocated to fit the new map dimensions
 * 	4. Various game systems are updated
 * 	5. The map options are read
 * 	6. Tiles from map_data are placed
 * 	7. A space for entity tile data is created with the ent_tile_data variable
 *	8. An attempt to add the map to the collector (see collector.h) is made. If the map was already added to the collector, copy the collector data to ent_tile_data.
 *	9. Entities in void rectangles are spawned from ent_tile_data
 *	10. The remaining entities that aren't in void rectangles are spawned from ent_tile_data
 *	11. The player is placed at the door from which they are entering the map, if there is one
 *
 * Map terminology:
 * 	tile data = data representing tiles defined in tile/data.c
 * 	entity tile data = data representing entity tiles defined in entity/tile.c
 * 	void rectangle = a data structure defined in void_rect.h
 * 	map memory = a 2d array accessed by [y][x], where y and x are tile coordinates in the game world
 */

#ifndef	MAP_H
#define	MAP_H

#include <stdbool.h>

#include "error.h"
#include "void_rect.h"

// The maximum dimensions of a map in tiles
#define	MAP_WIDTH_MAX	2000
#define	MAP_HEIGHT_MAX	2000

// The maximum length of g_map
#define	MAP_PATH_MAX	20

// Maximum number of void rectangles that can be used in one map
#define	VOID_RECT_LIST_LEN	20

typedef struct{
	// String containing the file path of the map
	char path[MAP_PATH_MAX];

	// True if the map is being edited
	bool editing : 1;

	// Width and height of the map in tiles
	int width, height;

	// Void rectangle list
	struct {
		// Array of void rectangles
		VoidRect r[VOID_RECT_LIST_LEN];

		// Length of the array
		int len;
	} vr_list;
} MapInfo;

// Info about the current map loaded
extern MapInfo g_map;

// Loads a map from a text file
// The editing parameter is true when the map is being opened for editing
ErrCode map_load_txt(char *path, bool editing);

// Saves a map to a text file, returns nonzero on error
int map_save_txt(char *path);

// Allocates and returns a pointer to map memory with size bytes for each index, returns NULL on error
void *map_alloc(int map_width, int map_height, size_t size);

// Frees map memory
void map_free(int map_height, void *map_ptr_param);

// Copies map data from *src to *dest
// For this to work, both *dest and *src must have the same width, height, and size
void map_copy(int map_width, int map_height, size_t size, void *dest_param, void *src_param);

// Returns false if two entities or tiles share the same map character, should be used in an assert
bool map_assert_dupchars(void);

// Returns a pointer to a new void rectangle in g_map.vr_list
// Returns NULL on error
VoidRect *map_vr_list_add(void);

// Removes the void rectangle and index index of g_map.vr_list.r
void map_vr_list_del(int index);

// Returns the tile id of a character, -1 if no tile is matched
int map_get_tile_id(char c);

// Returns the entity tile id of a character, -1 if no entity is matched
int map_get_ent_id(char c);

#endif
