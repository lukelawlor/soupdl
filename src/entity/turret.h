/*
 * turret.h contains the type for turret entities.
 *
 * This file is automatically included when you #include all.h
 */

#ifndef	ENTITY_TURRET_H
#define	ENTITY_TURRET_H

#include "c_sprite.h"
#include "entity.h"

// Entity item type
typedef struct{
	EcmBase base;
	SprTurret spr;

	// Position
	int x;
	int y;

	double fire_tick;
} EntTURRET;

EntTURRET *ent_new_TURRET(int x, int y);
void ent_update_TURRET(EntTURRET *e);
void ent_draw_TURRET(EntTURRET *e);
void ent_destroy_TURRET(EntTURRET *e);

#endif
