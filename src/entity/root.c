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
	EAN(ITEM, 100);
	EAN(FIREBALL, 100);
	EAN(PARTICLE, 400);
	EAN(RAGDOLL, 100);

	// Check for errors in allocating mem for entity arrays
	for (int i = 1; i < ENT_MAX; i++)
	{
		if (g_ent_root_array[i] == NULL)
		{
			PERR();
			fprintf(stderr, "failed to create entity array in ent_root_array_init()\n");
			return 1;
		}
	}
	return 0;
}
