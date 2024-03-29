/*
 * array.c contains functions that maniupate entity arrays.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "../error.h"
#include "id.h"
#include "c_base.h"
#include "array.h"
#include "root.h"

// Creates a new entity array and returns a pointer to it, returns NULL on error
EntArray *ent_array_new(int len_max, size_t ent_size)
{
	EntArray *a;
	if ((a = malloc(sizeof(EntArray))) == NULL)
	{
		PERR("failed to allocate mem for entity array struct\n");
		return NULL;
	}
	if ((a->e = calloc(len_max, ent_size)) == NULL)
	{
		PERR("failed to allocate mem for entity array\n");
		free(a);
		return NULL;
	}
	a->status = ENT_ARRAY_NORM;
	a->ent_size = ent_size;
	a->len_max = len_max;
	a->len = 0;
	return a;
}

// Frees an entity array
void ent_array_free(EntArray *a)
{
	free(a->e);
	free(a);
}

// Returns a pointer to space for a new entity in an entity array, returns NULL on error
void *ent_array_add(EntArray *a)
{
	if (a->len == a->len_max)
	{
		// No more room in entity array
		return NULL;
	}

	// Spooky pointer arithmetic to find a pointer to the first available space in the entity array
	Byte *e = (Byte *) a->e;
	e += a->ent_size * a->len;

	a->len++;
	return (void *) e;
}

// Deletes an entity from an entity array at g_er[id] at index i
void ent_array_del(EntId id, int index)
{
	EntArray *a = g_er[id];
	a->len--;

	Byte *dest = (Byte *) a->e;
	Byte *src = dest;

	// Copy mem from last entity to entity being deleted
	dest += a->ent_size * index;
	src += a->ent_size * a->len;
	memcpy((void *) dest, (void *) src, a->ent_size);

	// Set the new index of the moved entity
	((EntBASE *) dest)->base.index = index;
}

// Deletes all entities from an entity array with a status of ENT_STAT_DEL (defined in c_base.h)
// This should only be called on an entity array with a status of ENT_ARRAY_CLEAN
void ent_array_clean(EntArray *a)
{
	Byte *e = (Byte *) a->e;
	EcmBase *b;
	for (int i = 0; i < a->len; i++)
	{
		if ((b = &(((EntBASE *) e)->base))->status == ENT_STAT_DEL)
		{
			// Entity is marked for deletion
			ent_array_del(b->id, b->index);

			// i must be decremented to not skip over the entity that has just taken the place of the deleted entity
			i--;
		}
		else
		{
			// Entity wasn't marked for deletion, move to next entity
			e += a->ent_size;
		}
	}

	// Remove ENT_ARRAY_CLEAN status
	a->status = ENT_ARRAY_NORM;
}

// Deletes all of the entities in an array
void ent_array_reset(EntArray *a)
{
	a->len = 0;
}
