/*
 * groundguy.h contains the type for groundguy entities.
 *
 * This file is automatically included when you #include all.h
 */

#ifndef	ENTITY_GROUNDGUY_H
#define	ENTITY_GROUNDGUY_H

#include "entity.h"
#include "c_body.h"

// Entity groundguy type
typedef struct{
	EcmBase base;
	EcmBody b;
	SDL_RendererFlip flip;
} EntGROUNDGUY;

EntGROUNDGUY *ent_new_GROUNDGUY(int x, int y);
void ent_update_GROUNDGUY(EntGROUNDGUY *e);
void ent_draw_GROUNDGUY(EntGROUNDGUY *e);
void ent_destroy_GROUNDGUY(EntGROUNDGUY *e);

#endif
