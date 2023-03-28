/*
 * barrier.h contains the type for template entities.
 *
 * This file is automatically included when you #include all.h
 */

#ifndef	ENTITY_BARRIER_H
#define	ENTITY_BARRIER_H

#include "../barrier.h"

#include "entity.h"

// Entity item type
typedef struct{
	EcmBase base;

	// Position
	int x, y;

	BarrierTag btag;
} EntBARRIER;

EntBARRIER *ent_new_BARRIER(int x, int y, BarrierTag btag);
void ent_update_BARRIER(EntBARRIER *e);
void ent_draw_BARRIER(EntBARRIER *e);
void ent_destroy_BARRIER(EntBARRIER *e);

#endif
