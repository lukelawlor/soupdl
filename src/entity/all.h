/*
 * all.h contains #includes for the headers of all entity types in the game and functions that affect all entity types.
 */

#ifndef	ENTITY_ALL_H
#define	ENTITY_ALL_H

#include "player.h"
#include "item.h"
#include "fireball.h"
#include "particle.h"
#include "ragdoll.h"
#include "groundguy.h"
#include "cloud.h"
#include "slideguy.h"
#include "evilball.h"
#include "turret.h"
#include "door.h"
#include "savebird.h"
#include "barrier.h"

// Macros for updating and drawing lists of entities
#define	ENT_ARR(name)		g_er[ENT_ID_##name]

#define	ENT_UPDATE(name)	{ \
					Ent##name *name##_ptr; \
					name##_ptr = (Ent##name *) ENT_ARR(name)->e; \
					for (int i = 0; i < ENT_ARR(name)->len; i++) \
						ent_update_##name(name##_ptr++); \
					if (ENT_ARR(name)->status == ENT_ARRAY_CLEAN) \
						ent_array_clean(ENT_ARR(name)); \
				}

#define	ENT_DRAW(name)		{ \
					Ent##name *name##_ptr; \
					name##_ptr = (Ent##name *) ENT_ARR(name)->e; \
					for (int i = 0; i < ENT_ARR(name)->len; i++) \
						ent_draw_##name(name##_ptr++); \
				}

// Destroys all temporary entities (entities that don't continue to exist between map changes)
void ent_destroy_temp(void);

#endif
