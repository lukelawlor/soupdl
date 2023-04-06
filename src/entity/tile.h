/*
 * tile.h contains the type for entity tile spawners.
 *
 * Entity tiles are special map tiles that contain entity spawning information.
 *
 * Entity tile spawners are functions that actually spawn the entities. They take x and y int values, which represent the position of the entity to spawn, as their first two parameters. The last paramter is a void pointer whose use varies between different spawn functions and is ultimately up to the spawn functions to use however they want. They return nonzero on error.
 *
 * Steps to create a new entity tile:
 * 	tile.h		add ENT_TILE_<entity tile name> to EntTileId
 * 	tile.c		add spawner function
 * 	tile.c		add entity tile definition in ent_tile_init()
 */

#ifndef	ENTITY_TILE_H
#define	ENTITY_TILE_H

#include <SDL2/SDL.h>

// Function pointer to an entity tile spawner function
typedef bool (*EntTileSpawner)(const int x, const int y, const void *ptr);

// NOTE: this is the definition for entity tile ids, NOT entity ids. Entity ids are defined in id.h
typedef enum{
	ENT_TILE_PLAYER,
	ENT_TILE_TRUMPET,
	ENT_TILE_GROUNDGUY,
	ENT_TILE_GROUNDGUY_FAST,
	ENT_TILE_GROUNDGUY_CAREFUL,
	ENT_TILE_SLIDEGUY,
	ENT_TILE_SLIDEGUY_JUMPING,
	ENT_TILE_JUMPGUY,
	ENT_TILE_TURRET,
	ENT_TILE_DOOR0,
	ENT_TILE_DOOR1,
	ENT_TILE_DOOR2,
	ENT_TILE_DOOR3,
	ENT_TILE_SAVEBIRD,
	ENT_TILE_COIN,
	ENT_TILE_BARRIER,
	ENT_TILE_NONE,

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
