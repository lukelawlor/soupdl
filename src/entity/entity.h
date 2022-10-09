/*
 * entity.h contains types and functions that are used across multiple entities.
 */

#ifndef	ENTITY_ENTITY_H
#define	ENTITY_ENTITY_H

#include <stdbool.h>

#include <SDL2/SDL.h>

// The number of entities that every array of entities will hold
#define	ENT_LIST_MAX	200

// Default variables that every entity type should have
typedef struct{
	bool exists : 1;
} EntDefaults;

// Egg texture (tex_egg defined in texture.c) sprites
typedef enum{
	EGGSPR_IDLE,
	EGGSPR_RUN1,
	EGGSPR_RUN2,
	EGGSPR_SHOOT
} SprEgg;

// Points of offset used to find the positions of individual sprites in the texture tex_egg
extern const SDL_Point ent_eggspr_offset[4];

#endif
