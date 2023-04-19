/*
 * c_egg.h contains the EcmEgg entity component. This is commonly used by egg entities.
 */

#ifndef	ENTITY_C_EGG_H
#define	ENTITY_C_EGG_H

#include <stdbool.h>

#include "c_body.h"
#include "c_sprite.h"

typedef struct{
	EcmBody b;
	EcmEggSpr spr;

	// Hitpoints
	short hp;
} EcmEgg;

// NOTE: the following functions can be used on any EcmEgg objects. Some types of egg entities, such as evileggs, use their own versions of these functions, which are defined in the files "c_egg_*.*"

// Updates an egg's running animation
void ecm_egg_update_animation(EcmEgg *e);

// Draws an egg
void ecm_egg_draw(EcmEgg *e);

// Spawns bubble particles and an egg ragdoll, should be called before an egg entity is destroyed in its destroy function
void ecm_egg_die(EcmEgg *e);

// Handles collisions between the egg and spikes, fireballs, and the player
// Returns true if the egg dies from any damage it takes
bool ecm_egg_handle_collisions(EcmEgg *e);

// Returns true if the egg dies from any damage it takes
bool ecm_egg_damage(EcmEgg *e);

#endif
