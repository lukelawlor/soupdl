/*
 * c_sprite.c contains the definition for ecm_eggspr_offset.
 */

#include <SDL2/SDL.h>

#include "c_sprite.h"

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
};
