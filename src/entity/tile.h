/*
 * tile.h contains the type for entity tile spawners.
 *
 * Entity tiles are special map tiles that contain entity spawning information. Entity tile spawners are functions that actually spawn the entities. They take x and y int values as a their only two parameters and return an int. They return nonzero on error.
 *
 * The entity tile spawner for an entity tile can be found by accessing the g_ent_tile_spawner array. This array contains entity tile spawners and is indexed by entity tile ids (type EntTileId).
 */

#ifndef	ENTITY_TILE_H
#define	ENTITY_TILE_H

// NOTE: this is the definition for entity tile ids, NOT entity ids. Entity ids are defined in id.h
typedef enum{
	ENT_TILE_PLAYER,
	ENT_TILE_TRUMPET,
	ENT_TILE_GROUNDGUY,
	ENT_TILE_SLIDEGUY,
	ENT_TILE_TURRET,

	ENT_TILE_MAX,
} EntTileId;

typedef int (*EntTileSpawner)(const int x, const int y);

extern EntTileSpawner *const g_ets;

#endif
