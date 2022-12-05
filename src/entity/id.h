/*
 * id.h contains the ids of entities.
 */

#ifndef	ENTITY_ID_H
#define	ENTITY_ID_H

// Ids of all entities
typedef enum{
	ENT_ID_PLAYER,
	ENT_ID_ITEM,
	ENT_ID_FIREBALL,
	ENT_ID_PARTICLE,
	ENT_ID_RAGDOLL,
	ENT_ID_GROUNDGUY,
	ENT_ID_CLOUD,
	ENT_ID_SLIDEGUY,

	// Number of different types of entities (must be listed last)
	ENT_MAX,
} EntId;

#endif
