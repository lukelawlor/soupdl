/*
 * all.h contains #includes for the headers of all entity types in the game and functions that affect all entity types.
 */

#ifndef	ENTITY_ALL_H
#define	ENTITY_ALL_H

#include "entity.h"

#include "player.h"
#include "item.h"
#include "fireball.h"
#include "particle.h"
#include "ragdoll.h"

// Number of different types of entities
#define	ENT_MAX	5

// Ids of all entities
typedef enum{
	ENT_ID_PLAYER,
	ENT_ID_ITEM,
	ENT_ID_FIREBALL,
	ENT_ID_PARTICLE,
	ENT_ID_RAGDOLL,
} EntId;

// Destroys all entities in the game
void ent_destroy_all(void);

#endif
