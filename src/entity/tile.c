/*
 * tile.c contains the entity tile spawner array initialization list and various entity tile spawners.
 *
 * For more info on entity tiles, see tile.h.
 */

#include <SDL2/SDL.h>

#include "../font.h"	// For FONT_CHAR_WIDTH & FONT_CHAR_HEIGHT
#include "../texture.h"
#include "all.h"
#include "tile.h"

// Entity tile spawner prototypes
static inline int ets_player(const int x, const int y);
static inline int ets_trumpet(const int x, const int y);
static inline int ets_groundguy(const int x, const int y);
static inline int ets_slideguy(const int x, const int y);
static inline int ets_turret(const int x, const int y);
static inline int ets_jumpguy(const int x, const int y);
static inline int ets_door0(const int x, const int y);
static inline int ets_door1(const int x, const int y);
static inline int ets_door2(const int x, const int y);
static inline int ets_door3(const int x, const int y);

// Entity tile spawner array
static EntTileDef g_ent_tile_def[ENT_TILE_MAX];

// Constant pointer to the entity tile spawner array
EntTileDef *const g_ent_tile = g_ent_tile_def;

// Initializes the entity tile definition array
void ent_tile_init(void)
{
	g_ent_tile_def[ENT_TILE_PLAYER] = (EntTileDef) {
		"Player Spawn Point",
		'p',
		ets_player,
		{tex_egg, {0, 32, 32, 32}},
	};
	g_ent_tile_def[ENT_TILE_TRUMPET] = (EntTileDef) {
		"Trumpet",
		't',
		ets_trumpet,
		{tex_trumpet, {0, 0, 19, 11}},
	};
	g_ent_tile_def[ENT_TILE_GROUNDGUY] = (EntTileDef) {
		"Groundguy",
		'g',
		ets_groundguy,
		{tex_evilegg, {0, 32, 32, 32}},
	};
	g_ent_tile_def[ENT_TILE_SLIDEGUY] = (EntTileDef) {
		"Slideguy",
		'v',
		ets_slideguy,
		{tex_evilegg, {0, 0, 32, 32}},
	};
	g_ent_tile_def[ENT_TILE_TURRET] = (EntTileDef) {
		"Turret",
		'T',
		ets_turret,
		{tex_turret, {0, 0, 11, 17}},
	};
	g_ent_tile_def[ENT_TILE_JUMPGUY] = (EntTileDef) {
		"Jumpguy",
		'j',
		ets_jumpguy,
		{tex_evilegg, {0, 0, 11, 17}},
	};
	g_ent_tile_def[ENT_TILE_DOOR0] = (EntTileDef) {
		"Map Change Door #0",
		'0',
		ets_door0,
		{tex_font, {FONT_CHAR_WIDTH*0, FONT_CHAR_HEIGHT*3, FONT_CHAR_WIDTH, FONT_CHAR_HEIGHT}},
	};
	g_ent_tile_def[ENT_TILE_DOOR1] = (EntTileDef) {
		"Map Change Door #1",
		'1',
		ets_door1,
		{tex_font, {FONT_CHAR_WIDTH*1, FONT_CHAR_HEIGHT*3, FONT_CHAR_WIDTH, FONT_CHAR_HEIGHT}},
	};
	g_ent_tile_def[ENT_TILE_DOOR2] = (EntTileDef) {
		"Map Change Door #2",
		'2',
		ets_door2,
		{tex_font, {FONT_CHAR_WIDTH*2, FONT_CHAR_HEIGHT*3, FONT_CHAR_WIDTH, FONT_CHAR_HEIGHT}},
	};
	g_ent_tile_def[ENT_TILE_DOOR3] = (EntTileDef) {
		"Map Change Door #3",
		'3',
		ets_door3,
		{tex_font, {FONT_CHAR_WIDTH*3, FONT_CHAR_HEIGHT*3, FONT_CHAR_WIDTH, FONT_CHAR_HEIGHT}},
	};
}

// Entity tile spawner definitions
static inline int ets_player(const int x, const int y)
{
	g_player.b.x = x;
	g_player.b.y = y;
	return 0;
}

static inline int ets_trumpet(const int x, const int y)
{
	return ent_new_ITEM(x + 16, y + 32, ITEM_TRUMPET) == NULL;
}

static inline int ets_groundguy(const int x, const int y)
{
	return ent_new_GROUNDGUY(x, y, 0.0f) == NULL;
}

static inline int ets_slideguy(const int x, const int y)
{
	return ent_new_SLIDEGUY(x, y) == NULL;
}

static inline int ets_turret(const int x, const int y)
{
	return ent_new_TURRET(x, y) == NULL;
}

static inline int ets_jumpguy(const int x, const int y)
{
	return ent_new_GROUNDGUY(x, y, -5.0f) == NULL;
}

static inline int ets_door0(const int x, const int y)
{
	return ent_new_DOOR(x, y, 0) == NULL;
}

static inline int ets_door1(const int x, const int y)
{
	return ent_new_DOOR(x, y, 1) == NULL;
}

static inline int ets_door2(const int x, const int y)
{
	return ent_new_DOOR(x, y, 2) == NULL;
}

static inline int ets_door3(const int x, const int y)
{
	return ent_new_DOOR(x, y, 3) == NULL;
}
