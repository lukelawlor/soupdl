/*
 * coolegg.h contains the type for coolegg entities.
 *
 * These are friendly NPC eggs that move around randomly. You can talk to them.
 *
 * This file is automatically included when you #include all.h
 */

#ifndef	ENTITY_COOLEGG_H
#define	ENTITY_COOLEGG_H

#include "entity.h"

// Entity item type
typedef struct{
	EcmBase base;
	EcmEgg e;
} EntCOOLEGG;

EntCOOLEGG *ent_new_COOLEGG(int x, int y);
void ent_update_COOLEGG(EntCOOLEGG *e);
void ent_draw_COOLEGG(EntCOOLEGG *e);
void ent_destroy_COOLEGG(EntCOOLEGG *e);

#endif
