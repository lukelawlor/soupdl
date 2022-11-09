/*
 * ragdoll.h contains the type for ragdoll entities.
 *
 * This file is automatically included when you #include all.h
 */

#ifndef	ENTITY_RAGDOLL_H
#define	ENTITY_RAGDOLL_H

#include <stdbool.h>

#include "entity.h"
#include "c_body.h"

// Ragoll type type
typedef enum{
	RAGDOLL_EGG,
	RAGDOLL_EVILEGG
} EntRagdollId;

// Ragdoll type
typedef struct{
	EcmBase base;
	EcmBody b;
	
	// True when the ragdoll is still moving
	bool active : 1;

	// Id of ragdoll
	EntRagdollId rid : 1;

	// # of frames the bouncing sprite will be shown for
	short bounce_frames;
} EntRAGDOLL;

EntRAGDOLL *ent_new_RAGDOLL(float x, float y, float hsp, float vsp, EntRagdollId rid);
void ent_update_RAGDOLL(EntRAGDOLL *e);
void ent_draw_RAGDOLL(EntRAGDOLL *e);
void ent_destroy_RAGDOLL(EntRAGDOLL *e);

#endif
