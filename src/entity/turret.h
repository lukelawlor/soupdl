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

	// Ticks remaining until the turret fires next
	double fire_tick;

	// Ticks remaining to show the turret's firing face
	short fire_spr_frames;

	// Angle measurement from turret to player
	double dir;
} EntTURRET;

EntTURRET *ent_new_TURRET(int x, int y);
void ent_update_TURRET(EntTURRET *e);
void ent_draw_TURRET(EntTURRET *e);
void ent_destroy_TURRET(EntTURRET *e);

#endif
