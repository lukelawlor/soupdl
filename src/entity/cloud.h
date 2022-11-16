/*
 * cloud.h contains the type for cloud entities.
 *
 * This file is automatically included when you #include all.h
 */

#ifndef	ENTITY_CLOUD_H
#define	ENTITY_CLOUD_H

#include "../random.h"
#include "entity.h"

// Get a random hsp value for a cloud
#define	ENT_CLOUD_GET_RANDOM_HSP()	(spdl_random() / 255.0f) * 6 + 6

// Cloud id
typedef enum{
	ENT_CLOUD_BIG,
	ENT_CLOUD_SMALL,
} EntCloudId;

// Entity item type
typedef struct{
	EcmBase base;

	// Position
	float x, y;

	// Horizontal speed
	float hsp;

	// Id
	EntCloudId id;
} EntCLOUD;

EntCLOUD *ent_new_CLOUD(int x, int y, float hsp);
void ent_update_CLOUD(EntCLOUD *e);
void ent_draw_CLOUD(EntCLOUD *e);
void ent_destroy_CLOUD(EntCLOUD *e);

#endif
