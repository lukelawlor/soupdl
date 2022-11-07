/*
 * evilegg.h contains the type for evil egg entities.
 *
 * This file is automatically included when you #include all.h
 */

#ifndef	ENTITY_EVILEGG_H
#define	ENTITY_EVILEGG_H

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "entity.h"

typedef struct{
	EcmDefaults d;

	// Position
 	double x, y;

	// Horizontal and vertical speed
	double hsp, vsp;

	// Health (if <= 0, the player is considered dead)
	int hp;
	
	// Max health
	int maxhp;

	// True if the player has a trumpet
	bool has_trumpet;

	// Egg sprite to draw
	EcmSprEgg sprite;

	// Similarly to the hitbox rectangle, the x and y values of this point are used to position the trumpet relative to the player's position
	SDL_Point trumpet_offset;

	// Used to set flip the player (and trumpet) horizontally
	SDL_RendererFlip flip;

	// Step animation frame
	short anim_step_frame;

	// Step animation tmr (relative # of frames to show each step animation frame for)
	short anim_step_tmr;

	// Shoot animation tmr (# of frames to show the shooting sprite for)
	short anim_shoot_tmr;

} EntEvilegg;

// Constant pointer ot the first index of the evil egg entity array
extern EntEvilegg *const ent_evilegg;

EntEvilegg *ent_evilegg_new(float x, float y);
void ent_evilegg_update(EntEvilegg *e);
void ent_evilegg_draw(EntEvilegg *e);
void ent_evilegg_destroy(EntEvilegg *e);

#endif
