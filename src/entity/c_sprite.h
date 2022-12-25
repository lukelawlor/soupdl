/*
 * c_sprite.h contains types for entity sprites.
 */

#ifndef	ENTITY_C_SPRITE_H
#define	ENTITY_C_SPRITE_H

#include <SDL2/SDL.h>

// Spritesheet access
#define	SPR_EGG_W	32
#define	SPR_EGG_H	32
typedef enum{
	SPR_EGG_IDLE,
	SPR_EGG_RUN1,
	SPR_EGG_RUN2,
	SPR_EGG_SHOOT,
	SPR_EGG_FALL,
	SPR_EGG_BOUNCE,
	SPR_EGG_MAX,
} SprEgg;
extern const SDL_Rect g_spr_egg[SPR_EGG_MAX];

// Entity specific sprite structs
typedef struct{
	SprEgg spr;
	SDL_RendererFlip flip;

	// Used to store relative # of frames before the next animation frame is displayed
	short anim_tick;
} EcmEvileggSpr;

#endif
