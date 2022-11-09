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
	EcmBase base;

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
} EntFIREBALL;

EntFIREBALL *ent_new_FIREBALL(int x, int y, float hsp, float vsp);
void ent_update_FIREBALL(EntFIREBALL *e);
void ent_draw_FIREBALL(EntFIREBALL *e);
void ent_destroy_FIREBALL(EntFIREBALL *e);

#endif
