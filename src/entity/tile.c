/*
 * tile.c contains the entity tile spawner array initialization list and various entity tile spawners.
 *
 * For more info on entity tiles, see tile.h.
 */

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "../font.h"	// For FONT_CHAR_WIDTH & FONT_CHAR_HEIGHT
#include "../texture.h"
#include "all.h"
#include "tile.h"

// Macro for creating function headers for entity tile spawner functions
#define	ETS(name)	static bool ets_##name(const int x, const int y, const void *ptr)

// Entity tile spawner prototypes
ETS(player);
ETS(trumpet);
ETS(barrier);
ETS(groundguy);
ETS(groundguy_fast);
ETS(groundguy_careful);
ETS(slideguy);
ETS(slideguy_jumping);
ETS(jumpguy);
ETS(turret);
ETS(door0);
ETS(door1);
ETS(door2);
ETS(door3);
ETS(savebird);
ETS(coin);
ETS(none);

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
	g_ent_tile_def[ENT_TILE_BARRIER] = (EntTileDef) {
		"Barrier",
		'b',
		ets_barrier,
		{tex_cakico, {0, 0, 8, 8}},
	};
	g_ent_tile_def[ENT_TILE_GROUNDGUY] = (EntTileDef) {
		"Groundguy",
		'g',
		ets_groundguy,
		{tex_evilegg, {0, 32, 32, 32}},
	};
	g_ent_tile_def[ENT_TILE_GROUNDGUY_FAST] = (EntTileDef) {
		"Groundguy Fast",
		'f',
		ets_groundguy_fast,
		{tex_evilegg, {0, 32, 32, 32}},
	};
	g_ent_tile_def[ENT_TILE_GROUNDGUY_CAREFUL] = (EntTileDef) {
		"Groundguy Careful (doesn't walk off ledges)",
		'C',
		ets_groundguy_careful,
		{tex_evilegg, {0, 0, 32, 32}},
	};
	g_ent_tile_def[ENT_TILE_SLIDEGUY_JUMPING] = (EntTileDef) {
		"Slideguy Jumping",
		'S',
		ets_slideguy_jumping,
		{tex_evilegg, {0, 0, 32, 32}},
	};
	g_ent_tile_def[ENT_TILE_SLIDEGUY] = (EntTileDef) {
		"Slideguy",
		'v',
		ets_slideguy,
		{tex_evilegg, {0, 0, 32, 32}},
	};
	g_ent_tile_def[ENT_TILE_JUMPGUY] = (EntTileDef) {
		"Jumpguy",
		'j',
		ets_jumpguy,
		{tex_evilegg, {0, 0, 11, 17}},
	};
	g_ent_tile_def[ENT_TILE_TURRET] = (EntTileDef) {
		"Turret",
		'T',
		ets_turret,
		{tex_turret, {0, 0, 11, 17}},
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
	g_ent_tile_def[ENT_TILE_SAVEBIRD] = (EntTileDef) {
		"Savebird",
		'V',
		ets_savebird,
		{tex_egg, {SPR_EGG_W*3,SPR_EGG_H*0,SPR_EGG_W,SPR_EGG_H}},
	};
	g_ent_tile_def[ENT_TILE_COIN] = (EntTileDef) {
		"Coin",
		'+',
		ets_coin,
		{tex_cakico, {0, 0, 16, 16}},
	};
	g_ent_tile_def[ENT_TILE_NONE] = (EntTileDef) {
		"None",
		'N',
		ets_none,
		{tex_cakico, {0, 0, 16, 16}},
	};
}

// Entity tile spawner definitions
ETS(player)
{
	PINF("player spawned with VoidRectInt %d", * (VoidRectInt *) ptr);
	g_player.b.x = x;
	g_player.b.y = y;
	return 0;
}

ETS(trumpet)
{
	return ent_new_ITEM(x + 16, y + 32, ITEM_TRUMPET) == NULL;
}

ETS(barrier)
{
	return ent_new_BARRIER(x, y, * (BarrierTag *) ptr) == NULL;
}

ETS(groundguy)
{
	return ent_new_GROUNDGUY(x, y, 2.5f, 0.0f, false, * (BarrierTag *) ptr) == NULL;
}

ETS(groundguy_fast)
{
	return ent_new_GROUNDGUY(x, y, 6.0f, 0.0f, false,  * (BarrierTag *) ptr) == NULL;
}

ETS(groundguy_careful)
{
	return ent_new_GROUNDGUY(x, y, 3.2f, 0.0f, true, * (BarrierTag *) ptr) == NULL;
}

ETS(slideguy)
{
	return ent_new_SLIDEGUY(x, y, 8, 0.1f, 0.0f, * (BarrierTag *) ptr) == NULL;
}

ETS(slideguy_jumping)
{
	return ent_new_SLIDEGUY(x, y, 6, 0.08f, -5.0f, * (BarrierTag *) ptr) == NULL;
}

ETS(jumpguy)
{
	return ent_new_GROUNDGUY(x, y, 3.0f, -4.0f, false, * (BarrierTag *) ptr) == NULL;
}

ETS(turret)
{
	return ent_new_TURRET(x, y) == NULL;
}

ETS(door0)
{
	return ent_new_DOOR(x, y, 0) == NULL;
}

ETS(door1)
{
	return ent_new_DOOR(x, y, 1) == NULL;
}

ETS(door2)
{
	return ent_new_DOOR(x, y, 2) == NULL;
}

ETS(door3)
{
	return ent_new_DOOR(x, y, 3) == NULL;
}

ETS(savebird)
{
	return ent_new_SAVEBIRD(x, y) == NULL;
}

ETS(coin)
{
	return ent_new_ITEM(x + 16, y + 16, ITEM_COIN) == NULL;
}

ETS(none)
{
	// Should never be called
	return 1;
}
