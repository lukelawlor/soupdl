/*
 * fireball.h contains the type for fireball entities.
 *
 * This file is automatically included when you #include all.h
 */

#ifndef	ENTITY_FIREBALL_H
#define	ENTITY_FIREBALL_H

#include "entity.h"

// Entity fireball type
typedef struct{
	EcmDefaults d;

	// Position
	float x;
	float y;

	// Horizontal and vertical speed
	float hsp;
	float vsp;

	// Stores 0 or 1 to indicate which fireball sprite to render
	unsigned int frame : 1;

	// Number of frames left for the current animation frame to change
	unsigned int frame_tmr : 2;
} EntFireball;

// Constant pointer to the first index of the fireball entity array
extern EntFireball *const ent_fireball;

EntFireball *ent_fireball_new(int x, int y, float hsp, float vsp);
void ent_fireball_update(EntFireball *e);
void ent_fireball_draw(EntFireball *e);
void ent_fireball_destroy(EntFireball *e);

#endif
