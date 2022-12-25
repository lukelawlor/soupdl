/*
 * c_evilegg.h contains functions that are commonly used by enemy evil egg entities. These include most enemies with names ending in "guy".
 */

#ifndef	ENTITY_C_EVILEGG_H
#define	ENTITY_C_EVILEGG_H

#include <stdbool.h>

#include "c_body.h"
#include "c_sprite.h"

typedef struct{
	EcmBody b;
	EcmEvileggSpr spr;

	// Hitpoints
	short hp;
} EcmEvilegg;

// Updates an egg's running animation
void ecm_evilegg_update_animation(EcmEvilegg *e);

// Draws an egg
void ecm_evilegg_draw(EcmEvilegg *e);

// Spawns bubble particles and an egg ragdoll, should be called before an egg entity is destroyed in its destroy function
void ecm_evilegg_die(EcmEvilegg *e);

// Handles collisions between the egg and spikes, fireballs, and the player
// Returns true if the egg dies from any damage it takes
bool ecm_evilegg_handle_collisions(EcmEvilegg *e);

// Returns true if the egg dies from any damage it takes
bool ecm_evilegg_damage(EcmEvilegg *e);

#endif
