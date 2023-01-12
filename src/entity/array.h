/*
 * array.h contains types and functions for entity arrays.
 */

#ifndef	ENTITY_ARRAY_H
#define	ENTITY_ARRAY_H

#include <stdint.h>
#include <stddef.h>

#include "id.h"

typedef uint8_t Byte;

typedef enum{
	ENT_ARRAY_NORM,

	// This status indicates that one or more entities in the array has been marked for deletion
	ENT_ARRAY_CLEAN,
} EntArrayStatus;

typedef struct{
	EntArrayStatus status;
	
	// Array of entities
	void *e;

	// Size of a single entity in the array
	size_t ent_size;

	// Max # of entities the array can hold
	int len_max;

	// Current # of entities in the array
	int len;
} EntArray;

// Creates a new entity array and returns a pointer to it, returns NULL on error
EntArray *ent_array_new(int len_max, size_t ent_size);

// Frees an entity array
void ent_array_free(EntArray *a);

// Returns a pointer to space for a new entity in an entity array, returns NULL on error
void *ent_array_add(EntArray *a);

// Deletes an entity from an entity array at g_er[id] at index i
void ent_array_del(EntId id, int i);

// Deletes all entities from an entity array with a status of ENT_STAT_DEL (defined in c_base.h)
void ent_array_clean(EntArray *a);

// Deletes all of the entities in an array
void ent_array_reset(EntArray *a);

#endif
