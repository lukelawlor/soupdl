/*
 * c_egg_evil.h contains functions that act on EcmEgg objects (defined in c_egg.h) that are used by evilegg entities.
 */

#ifndef	ENTITY_C_EGG_EVIL_H
#define	ENTITY_C_EGG_EVIL_H

#include <stdbool.h>

#include "c_egg.h"	// For EcmEgg

// Handles collisions between the evilegg and fireballs or the player
// Returns true if the evilegg dies from any damage it takes
bool ecm_egg_evil_handle_collisions(EcmEgg *e);

#endif
