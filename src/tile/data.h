/*
 * data.h contains the definitions for tile space types.
 */

#ifndef	TILE_DATA_H
#define	TILE_DATA_H

#include <stdint.h>

#include <SDL2/SDL.h>

// Width & height in pixels of each tile
#define	TILE_SIZE	32

// Maximum length in chars for a path to a map file
#define	MAX_MAP_PATH_LEN	100

// Total number of different tile types
#define	TILE_MAX		6

// Tile bit flags

// Tile rotations (ROT1 means the sprite rotated 90 degrees once, ROT2 means rotated 90 degrees twice, and so on)
#define	TFLAG_ROT1	1
#define	TFLAG_ROT2	2
#define	TFLAG_ROT3	4
#define	TFLAG_SOLID	8

// Tile is a type of spike
#define	TFLAG_SPIKE	16

// Type to hold all tile bit flags
typedef uint8_t TileFlags;

// Tile types (NOTE: the order that these are declared is important because these values are used to index the types of tiles in arrays)
typedef enum{
	TILE_AIR,
	TILE_STONE,
	TILE_LIME,
	TILE_IRON,
	TILE_SPIKE,
	TILE_STRING,
} TileId;

// Struct containing the metadata of each tile type
typedef struct{
	// The character used to represent the tile in a map file (make sure no 2 tileable objects (tiles or entities) have use same character)
	char map_char;

	// Point in the tilemap spritesheet where the sprite of the type of tile starts
	// This is used to construct a source rectangle for the sprite sheet to draw a sprite, the width and height of these rectangles will be TILE_SIZE
	SDL_Point spoint;

	TileFlags flags;

	// Proper name of the tile type (used for the map editor)
	char *name;
} TileMetadata;

// Constant pointer to the first index of tile_property_list (defined in tile/data.c)
extern const TileMetadata *const g_tile_md;

// Room width and height in tiles, not pixels
extern int g_room_width;
extern int g_room_height;

// Pointer to map memory (term defined in map.h) containing tile ids
extern TileId **g_tile_map;

// Id of tile type to treat all tiles outside the map as
extern TileId g_tile_outside;

#endif
