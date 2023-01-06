/*
 * data.h contains the definitions for tile space types.
 */

#ifndef	TILE_DATA_H
#define	TILE_DATA_H

#include <SDL2/SDL.h>

// Tile types (NOTE: the order that these are declared is important because these values are used to index the types of tiles in arrays)
typedef enum{
	TILE_AIR,
	TILE_STONE,
	TILE_LIME,
	TILE_IRON,
	TILE_SPIKE,
	TILE_STRING,
	TILE_GRASS,

	// Total number of different tile types (must be listed last)
	TILE_MAX,
} TileId;

// Width & height in pixels of each tile
#define	TILE_SIZE	32

// Tile bit flags
typedef enum{
	// Tile rotations (ROT1 means the sprite rotated 90 degrees once, ROT2 means rotated 90 degrees twice, and so on)
	TFLAG_ROT1	= (1 << 0),
	TFLAG_ROT2	= (1 << 1),
	TFLAG_ROT3	= (1 << 2),
	TFLAG_SOLID	= (1 << 3),

	// Tile is a type of spike
	TFLAG_SPIKE	= (1 << 4),
} TileFlags;

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
