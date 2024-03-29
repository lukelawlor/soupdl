/*
 * root.c contains functions for initializing the root entity array.
 */

#include <stdio.h>

#include "../error.h"
#include "id.h"
#include "all.h"
#include "array.h"
#include "root.h"

// Shorthand for creating a new entity array and adding it to the root entity array
#define	EAN(name, len_max)		EntArray *name = ent_array_new(len_max, sizeof(Ent##name)); \
					g_ent_root_array[ENT_ID_##name] = name

// See root.h for an explanation of what the root entity array is
static EntArray *g_ent_root_array[ENT_MAX];

EntArray **const g_er = g_ent_root_array;

// Initializes the root entity array, returns nonzero on error
int ent_root_array_init(void)
{
	g_ent_root_array[ENT_ID_PLAYER] = NULL;
	EAN(ITEM, 5000);
	EAN(FIREBALL, 100);
	EAN(PARTICLE, 5000);
	EAN(RAGDOLL, 200);
	EAN(GROUNDGUY, 200);
	EAN(CLOUD, 160);
	EAN(SLIDEGUY, 100);
	EAN(EVILBALL, 200);
	EAN(TURRET, 60);
	EAN(DOOR, ENT_DOOR_MAX);
	EAN(SAVEBIRD, 20);
	EAN(BARRIER, 100);
	EAN(COOLEGG, 100);

	// Check for errors in allocating mem for entity arrays
	for (int i = 1; i < ENT_MAX; i++)
	{
		if (g_ent_root_array[i] == NULL)
		{
			PERR("failed to create entity array in ent_root_array_init()\n");
			return 1;
		}
	}
	return 0;
}

// Frees all entity arrays
void ent_root_array_free(void)
{
	for (int i = 1; i < ENT_MAX; i++)
		ent_array_free(g_ent_root_array[i]);
}
