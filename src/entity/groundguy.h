/*
 * groundguy.h contains the type for groundguy entities.
 *
 * This file is automatically included when you #include all.h
 */

#ifndef	ENTITY_GROUNDGUY_H
#define	ENTITY_GROUNDGUY_H

#include "c_evilegg.h"
#include "entity.h"

// Entity groundguy type
typedef struct{
	EcmBase base;
	EcmEvilegg e;

	// Jump speed
	float jsp;

	// True if the egg won't walk off of ledges
	bool stay_on_ledge : 1;
} EntGROUNDGUY;

EntGROUNDGUY *ent_new_GROUNDGUY(int x, int y, float hsp, float jsp, bool stay_on_ledge);
void ent_update_GROUNDGUY(EntGROUNDGUY *e);
void ent_draw_GROUNDGUY(EntGROUNDGUY *e);
void ent_destroy_GROUNDGUY(EntGROUNDGUY *e);

#endif
