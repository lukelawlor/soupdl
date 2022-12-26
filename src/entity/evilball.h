/*
 * evilball.h contains the type for evilball entities.
 *
 * This file is automatically included when you #include all.h
 */

#ifndef	ENTITY_EVILBALL_H
#define	ENTITY_EVILBALL_H

#include "entity.h"

// Entity item type
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
	unsigned int frame_tmr : 3;

	// Number of ticks left until the evilball destroys itself
	short destroy_ticks;
} EntEVILBALL;

EntEVILBALL *ent_new_EVILBALL(int x, int y, float hsp, float vsp);
void ent_update_EVILBALL(EntEVILBALL *e);
void ent_draw_EVILBALL(EntEVILBALL *e);
void ent_destroy_EVILBALL(EntEVILBALL *e);

#endif
