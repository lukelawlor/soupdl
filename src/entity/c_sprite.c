/*
 * c_sprite.c contains the definition for ecm_eggspr_offset.
 */

#include <SDL2/SDL.h>

#include "../texture.h"
#include "../tile/data.h"	// For TILE_SIZE
#include "c_sprite.h"

// Sprites
const SDL_Rect g_spr_egg[SPR_EGG_MAX] = {
	// SPR_EGG_IDLE
	{SPR_EGG_W*0,	SPR_EGG_H*1,	SPR_EGG_W, SPR_EGG_H},
	// SPR_EGG_RUN1
	{SPR_EGG_W*0,	SPR_EGG_H*0,	SPR_EGG_W, SPR_EGG_H},
	// SPR_EGG_RUN2
	{SPR_EGG_W*1,	SPR_EGG_H*0,	SPR_EGG_W, SPR_EGG_H},
	// SPR_EGG_SHOOT
	{SPR_EGG_W*1,	SPR_EGG_H*1,	SPR_EGG_W, SPR_EGG_H},
	// SPR_EGG_FALL
	{SPR_EGG_W*2,	SPR_EGG_H*0,	SPR_EGG_W, SPR_EGG_H},
	// SPR_EGG_BOUNCE
	{SPR_EGG_W*2,	SPR_EGG_H*1,	SPR_EGG_W, SPR_EGG_H},
	// SPR_EGG_DRIFT1
	{SPR_EGG_W*3,	SPR_EGG_H*0,	SPR_EGG_W, SPR_EGG_H},
	// SPR_EGG_DRIFT2
	{SPR_EGG_W*3,	SPR_EGG_H*1,	SPR_EGG_W, SPR_EGG_H},
	// SPR_EGG_WEIRD1
	{SPR_EGG_W*4,	SPR_EGG_H*0,	SPR_EGG_W, SPR_EGG_H},
	// SPR_EGG_WEIRD2
	{SPR_EGG_W*4,	SPR_EGG_H*1,	SPR_EGG_W, SPR_EGG_H},
};

const SDL_Rect g_spr_turret[SPR_TURRET_MAX] = {
	// SPR_TURRET_NORM1
	{SPR_TURRET_W*0,	SPR_TURRET_H*0,	SPR_TURRET_W, SPR_TURRET_H},
	// SPR_TURRET_NORM2
	{SPR_TURRET_W*1,	SPR_TURRET_H*0,	SPR_TURRET_W, SPR_TURRET_H},
	// SPR_TURRET_NORM3
	{SPR_TURRET_W*2,	SPR_TURRET_H*0,	SPR_TURRET_W, SPR_TURRET_H},
	// SPR_TURRET_NORM4
	{SPR_TURRET_W*3,	SPR_TURRET_H*0,	SPR_TURRET_W, SPR_TURRET_H},
	// SPR_TURRET_FIRE1
	{SPR_TURRET_W*0,	SPR_TURRET_H*1,	SPR_TURRET_W, SPR_TURRET_H},
	// SPR_TURRET_FIRE2
	{SPR_TURRET_W*1,	SPR_TURRET_H*1,	SPR_TURRET_W, SPR_TURRET_H},
	// SPR_TURRET_FIRE3
	{SPR_TURRET_W*2,	SPR_TURRET_H*1,	SPR_TURRET_W, SPR_TURRET_H},
	// SPR_TURRET_FIRE4
	{SPR_TURRET_W*3,	SPR_TURRET_H*1,	SPR_TURRET_W, SPR_TURRET_H},
	// SPR_TURRET_BASE
	{SPR_TURRET_W*4,		0, TILE_SIZE, TILE_SIZE},
	// SPR_TURRET_BASEX
	{SPR_TURRET_W*4 + TILE_SIZE,	0, TILE_SIZE, TILE_SIZE},
};

// Texture pointer array declarations
static SDL_Texture *g_internal_tex_egg[TEX_EGG_MAX];
SDL_Texture **const g_tex_egg = (void *) g_internal_tex_egg;

// Loads texture pointer arrays
void ecm_sprite_load_textures(void)
{
	g_internal_tex_egg[TEX_EGG_EGG] = tex_egg;
	g_internal_tex_egg[TEX_EGG_EVIL] = tex_evilegg;
	g_internal_tex_egg[TEX_EGG_COOL] = tex_coolegg;
}
