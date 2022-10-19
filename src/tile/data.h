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
#define	TILE_MAX		5

// Tile bit flags

// Tile rotations (ROT1 means the sprite rotated 90 degrees once, ROT2 means rotated 90 degrees twice, and so on)
#define	TFLAG_ROT1	1
#define	TFLAG_ROT2	2
#define	TFLAG_ROT3	4
#define	TFLAG_SOLID	8

// Type to hold all tile bit flags
typedef uint8_t TileFlags;

// Tile types (NOTE: the order that these are declared is important because these values are used to index the types of tiles in arrays)
typedef enum{
	TILE_AIR,
	TILE_STONE,
	TILE_LIME,
	TILE_IRON,
	TILE_SPIKE
} TileId;

// Struct containing all of the properties each type of tile has
typedef struct{
	// Point in the tilemap spritesheet where the sprite of the type of tile starts
	// This is used to construct a source rectangle for the sprite sheet to draw a sprite, the width and height of these rectangles will be TILE_SIZE
	SDL_Point spoint;

	TileFlags flags;
} TileProperty;

// Constant pointer to the first index of tile_property_list (defined in tile/data.c)
extern const TileProperty *const g_tile_property;

// Room width and height in tiles, not pixels
extern int g_room_width;
extern int g_room_height;

// 2d array containing tile ids
extern TileId **g_tile_map;

// Id of tile type to treat all tiles outside the map as
extern TileId g_tile_outside;

// Free all tile spaces and arrays containing pointers to them
void tile_map_free(void);

#endif
