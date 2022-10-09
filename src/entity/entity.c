/*
 * entity.h contains data structures and functions that are used across multiple entities.
 */

#include <SDL2/SDL.h>

#include "entity.h"

// Points of offset used to find the positions of individual sprites in the texture tex_egg
const SDL_Point ent_eggspr_offset[4] = {
	// EGGSPR_IDLE
	{0, 32},
	// EGGSPR_RUN1
	{0, 0},
	// EGGSPR_RUN2
	{32, 0},
	// EGGSPR_SHOOT
	{32, 32}
};

