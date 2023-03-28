/*
 * all.c contains functions that affect all types of entities in the game.
 */

#include "id.h"
#include "array.h"
#include "root.h"
#include "all.h"

// Destroys all temporary entities (entities that don't continue to exist between map changes)
void ent_destroy_temp(void)
{
	ent_array_reset(g_er[ENT_ID_ITEM]);
	ent_array_reset(g_er[ENT_ID_FIREBALL]);
	ent_array_reset(g_er[ENT_ID_PARTICLE]);
	ent_array_reset(g_er[ENT_ID_RAGDOLL]);
	ent_array_reset(g_er[ENT_ID_GROUNDGUY]);
	ent_array_reset(g_er[ENT_ID_SLIDEGUY]);
	ent_array_reset(g_er[ENT_ID_EVILBALL]);
	ent_array_reset(g_er[ENT_ID_TURRET]);
	ent_array_reset(g_er[ENT_ID_DOOR]);
	ent_array_reset(g_er[ENT_ID_SAVEBIRD]);
	ent_array_reset(g_er[ENT_ID_BARRIER]);
}
