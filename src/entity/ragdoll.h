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
} EntRagdollId;

// Ragdoll type
typedef struct{
	EcmDefaults d;

	// True when the ragdoll is still moving
	bool active : 1;

	// Id of ragdoll
	EntRagdollId id : 1;

	// # of frames the bouncing sprite will be shown for
	short bounce_frames;

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

EntRagdoll *ent_ragdoll_new(float x, float y, float hsp, float vsp, EntRagdollId id);
void ent_ragdoll_update(EntRagdoll *e);
void ent_ragdoll_draw(EntRagdoll *e);
void ent_ragdoll_destroy(EntRagdoll *e);

#endif
