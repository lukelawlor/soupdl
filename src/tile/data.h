/*
 * data.h contains the definitions for tile space types.
 */

#ifndef	TILE_H
#define	TILE_H

// Game tile size
#define	TILE_SIZE	32

// Maximum length in chars for a path to a map file
#define	MAX_MAP_PATH_LEN	100

// Tile space tile types
typedef enum{
	TILE_AIR,
	TILE_STONE
} tile_space_tile;

typedef struct{
	tile_space_tile tile;
} TileSpace;

// Room width and height in tiles, not pixels
extern int g_room_width;
extern int g_room_height;

// 2d array containing pointers to tile spaces
extern TileSpace ***g_tile_space;

// Tile type for tiles outside the map
extern tile_space_tile g_tile_outside;

// Loads a map from a text file
int tile_map_load_txt(char *path);

#endif
