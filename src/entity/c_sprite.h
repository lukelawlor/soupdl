/*
 * c_sprite.h contains types for entity sprites.
 */

#ifndef	ENTITY_C_SPRITE_H
#define	ENTITY_C_SPRITE_H

#include <SDL2/SDL.h>

// Egg texture (tex_egg defined in texture.c) sprites
typedef enum{
	EGGSPR_IDLE,
	EGGSPR_RUN1,
	EGGSPR_RUN2,
	EGGSPR_SHOOT,
} EcmSprEgg;

// Points of offset used to find the positions of individual sprites in the texture tex_egg
extern const SDL_Point ent_eggspr_offset[4];

#endif
