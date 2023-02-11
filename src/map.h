/*
 * map.h contains functions for loading and saving maps.
 *
 * A map consists of tile data and entity spawn points. Currently they can only be saved as text files though I plan on implementing a binary file format for them.
 *
 * "map memory" refers to a 2d array accessed by [x][y] with dimensions g_room_width by g_room_height (defined in tile/data.h)
 */

#ifndef	MAP_H
#define	MAP_H

#include <stdbool.h>

#include "error.h"

// The maximum length of g_map
#define	MAP_PATH_MAX	20

// String containing the name of the currently loaded map
extern char g_map[MAP_PATH_MAX];

// Loads a map from a text file
// The editing paramter is true when the map is being opened for editing, make sure maped_init (from editor/editor.h) has been called before this is indicated
ErrCode map_load_txt(char *path, bool editing);

// Saves a map to a text file, returns nonzero on error
int map_save_txt(char *path);

// Allocates and returns a pointer to map memory with size bytes for each index, returns NULL on error
void **map_alloc(size_t size);

// Frees map memory
void map_free(void **map_ptr);

// Returns false if two entities or tiles share the same map character, should be used in an assert
bool map_assert_dupchars(void);

#endif
