/*
 * tile.c contains the entity tile spawner array initialization list and various entity tile spawners.
 *
 * For more info on entity tiles, see tile.h.
 */

#include "all.h"
#include "tile.h"

// Entity tile spawner prototypes
static inline int ets_player(const int x, const int y);
static inline int ets_trumpet(const int x, const int y);
static inline int ets_groundguy(const int x, const int y);
static inline int ets_slideguy(const int x, const int y);
static inline int ets_turret(const int x, const int y);

// Entity tile spawner array
static EntTileSpawner g_ent_tile_spawner[ENT_TILE_MAX] = {
	// ENT_TILE_PLAYER
	ets_player,

	// ENT_TILE_TRUMPET
	ets_trumpet,

	// ENT_TILE_GROUNDGUY
	ets_groundguy,

	// ENT_TILE_SLIDEGUY
	ets_slideguy,

	// ENT_TILE_TURRET
	ets_turret,
};

// Constant pointer to the entity tile spawner array
EntTileSpawner *const g_ets = g_ent_tile_spawner;

// Entity tile spawner definitions
static inline int ets_player(const int x, const int y)
{
	g_player.b.x = x;
	g_player.b.y = y;
	return 0;
}

static inline int ets_trumpet(const int x, const int y)
{
	return ent_new_ITEM(x + 16, y + 32, ITEM_TRUMPET) == NULL ? 1 : 0;
}

static inline int ets_groundguy(const int x, const int y)
{
	return ent_new_GROUNDGUY(x, y, 0.0f) == NULL ? 1 : 0;
}

static inline int ets_slideguy(const int x, const int y)
{
	return ent_new_SLIDEGUY(x, y) == NULL ? 1 : 0;
}

static inline int ets_turret(const int x, const int y)
{
	return ent_new_TURRET(x, y) == NULL ? 1 : 0;
}
