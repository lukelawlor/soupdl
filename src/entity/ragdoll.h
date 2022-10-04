/*
 * ragdoll.h contains the type for ragdoll entities.
 *
 * This file is automatically included when you #include all.h
 */

#ifndef	ENTITY_RAGDOLL_H
#define	ENTITY_RAGDOLL_H

#include <stdbool.h>

#include "entity.h"

// Ragoll type type
typedef enum{
	RAGDOLL_EGG,
	RAGDOLL_EVILEGG
} ent_ragdoll_type;

// Ragdoll type
typedef struct{
	EntDefaults d;

	// True when the ragdoll is still moving
	bool active : 1;

	// Type of ragdoll
	ent_ragdoll_type type : 1;

	// Position
	float x;
	float y;

	// Speeds
	float hsp;
	float vsp;
	float grv;
} EntRagdoll;

// Constant pointer ot the first index of the ragdoll entity array
extern EntRagdoll *const ent_ragdoll;

EntRagdoll *ent_ragdoll_new(float x, float y, float hsp, float vsp, ent_ragdoll_type type);
void ent_ragdoll_update(EntRagdoll *e);
void ent_ragdoll_draw(EntRagdoll *e);
void ent_ragdoll_destroy(EntRagdoll *e);

#endif
