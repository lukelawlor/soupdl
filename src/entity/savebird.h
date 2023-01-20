/*
 * savebird.h contains the type for savebird entities.
 *
 * This file is automatically included when you #include all.h
 */

#ifndef	ENTITY_SAVEBIRD_H
#define	ENTITY_SAVEBIRD_H

#include "entity.h"

// Entity item type
typedef struct{
	EcmBase base;
	int x;
	int y;
} EntSAVEBIRD;

EntSAVEBIRD *ent_new_SAVEBIRD(int x, int y);
void ent_update_SAVEBIRD(EntSAVEBIRD *e);
void ent_draw_SAVEBIRD(EntSAVEBIRD *e);
void ent_destroy_SAVEBIRD(EntSAVEBIRD *e);

#endif
