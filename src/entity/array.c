/*
 * array.c contains functions that maniupate entity arrays.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "../error.h"
#include "id.h"
#include "array.h"
#include "root.h"

// Creates a new entity array and returns a pointer to it
EntArray *ent_array_new(int len_max, size_t ent_size)
{
	EntArray *a;
	if ((a = malloc(sizeof(EntArray))) == NULL)
	{
		PERR();
		fprintf(stderr, "failed to allocate mem for entity array struct\n");
	}
	if ((a->e = calloc(len_max, ent_size)) == NULL)
	{
		PERR();
		fprintf(stderr, "failed to allocate mem for entity array\n");
	}
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
	char *e = (char *) a->e;
	e += a->ent_size * a->len;

	a->len++;
	return (void *) e;
}

// Deletes an entity from an entity array at g_er[id] at index i
void ent_array_del(EntId id, int i)
{
	EntArray *a = g_er[id];
	a->len--;

	char *dest = (char *) a->e;
	char *src = dest;

	// Copy mem from last entity to entity being deleted
	dest += a->ent_size * i;
	src += a->ent_size * a->len;

	memcpy((void *) dest, (void *) src, a->ent_size);
}
