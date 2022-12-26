/*
 * tile.h contains the type for entity tile spawners.
 *
 * Entity tiles are special map tiles that contain entity spawning information. Entity tile spawners are functions that actually spawn the entities. They take x and y int values as a their only two parameters and return an int. They return nonzero on error.
 *
 * The entity tile spawner for an entity tile can be found by accessing the g_ent_tile_spawner array. This array contains entity tile spawners and is indexed by entity tile ids (type EntTileId).
 */

#ifndef	ENTITY_TILE_H
#define	ENTITY_TILE_H

#include <SDL2/SDL.h>

typedef int (*EntTileSpawner)(const int x, const int y);

// NOTE: this is the definition for entity tile ids, NOT entity ids. Entity ids are defined in id.h
typedef enum{
	ENT_TILE_PLAYER,
	ENT_TILE_TRUMPET,
	ENT_TILE_GROUNDGUY,
	ENT_TILE_SLIDEGUY,
	ENT_TILE_TURRET,

	ENT_TILE_MAX,
} EntTileId;

// Entity tile texture in map editor
typedef struct{
	SDL_Texture *tex;
	SDL_Rect srect;
} EntTileTex;

// Entity tile definition
typedef struct{
	// Entity name
	char *name;

	// The character used to represent the entity in a map file
	char map_char;

	// Pointer to function that spawns the entity
	EntTileSpawner spawner;

	// Texture
	EntTileTex tex;
} EntTileDef;

// Constant pointer to the entity tile definition array
extern EntTileDef *const g_ent_tile;

// Initializes the entity tile definition array
void ent_tile_init(void);

#endif
