/*
 * array.h contains types and functions for entity arrays.
 */

#ifndef	ENTITY_ARRAY_H
#define	ENTITY_ARRAY_H

#include <stddef.h>

typedef struct{
	// Array of entities
	void *e;

	// Size of a single entity in the array
	size_t ent_size;

	// Max # of entities the array can hold
	int len_max;

	// Current # of entities in the array
	int len;
} EntArray;

typedef struct{
	// Pointer to entity array that the entity is stored in
	EntArray *a;

	// Index in entity array that the pointer to the entity is stored in
	int p;
} EcmId;

// Creates a new entity array and returns a pointer to it
EntArray *ent_array_new(int len_max, size_t ent_size);

// Frees an entity array
void ent_array_free(EntArray *a);

// Returns a pointer to space for a new entity in an entity array, returns NULL on error
void *ent_array_add(EntArray *a);

// Deletes an entity from an entity array at index ent_id in the array
void ent_array_del(EcmId *id);

#endif
