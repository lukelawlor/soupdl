/*
 * map.h contains functions for loading and saving maps.
 *
 * A map consists of tile data and entity spawn points. Currently they can only be saved as text files though I plan on implementing a binary file format for them.
 *
 * "map memory" refers to a 2d array accessed by [y][x], where y and x are tile coordinates in the game world
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

// Returns false if two entities or tiles share the same map character, should be used in an assert
bool map_assert_dupchars(void);

#endif
