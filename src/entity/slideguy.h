/*
 * slideguy.h contains the type for template entities.
 *
 * This file is automatically included when you #include all.h
 */

#ifndef	ENTITY_SLIDEGUY_H
#define	ENTITY_SLIDEGUY_H

#include <SDL2/SDL.h>

#include "c_body.h"
#include "entity.h"

// Entity item type
typedef struct{
	EcmBase base;
	EcmBody b;
	SDL_RendererFlip flip;

	// Horizontal acceleration
	float acc;
} EntSLIDEGUY;

EntSLIDEGUY *ent_new_SLIDEGUY(int x, int y);
void ent_update_SLIDEGUY(EntSLIDEGUY *e);
void ent_draw_SLIDEGUY(EntSLIDEGUY *e);
void ent_destroy_SLIDEGUY(EntSLIDEGUY *e);

#endif
