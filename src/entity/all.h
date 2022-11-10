/*
 * all.h contains #includes for the headers of all entity types in the game and functions that affect all entity types.
 */

#ifndef	ENTITY_ALL_H
#define	ENTITY_ALL_H

#include "player.h"
#include "item.h"
#include "fireball.h"
#include "particle.h"
#include "ragdoll.h"
#include "groundguy.h"

// Destroys all entities in the game
void ent_destroy_all(void);

#endif
