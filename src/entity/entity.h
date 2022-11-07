/*
 * entity.h contains ENT_LIST_MAX and the default entity component struct.
 */

#ifndef	ENTITY_ENTITY_H
#define	ENTITY_ENTITY_H

#include <stdbool.h>

#include <SDL2/SDL.h>

// The number of entities that every array of entities will hold
#define	ENT_LIST_MAX	200

// Default variables that every entity type should have
typedef struct{
	bool exists : 1;
} EcmDefaults;

#endif
