/*
 * slideguy.h contains the type for template entities.
 *
 * This file is automatically included when you #include all.h
 */

#ifndef	ENTITY_SLIDEGUY_H
#define	ENTITY_SLIDEGUY_H

#include <SDL2/SDL.h>

#include "../barrier.h"

#include "c_evilegg.h"
#include "entity.h"

// Entity item type
typedef struct{
	EcmBase base;
	EcmEvilegg e;

	// Horizontal acceleration
	float acc;

	// Jump speed
	float jsp;

	BarrierTag btag;
} EntSLIDEGUY;

EntSLIDEGUY *ent_new_SLIDEGUY(int x, int y, int hp, float acc, float jsp, BarrierTag btag);
void ent_update_SLIDEGUY(EntSLIDEGUY *e);
void ent_draw_SLIDEGUY(EntSLIDEGUY *e);
void ent_destroy_SLIDEGUY(EntSLIDEGUY *e);

#endif
