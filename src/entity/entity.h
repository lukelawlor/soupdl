/*
 * entity.h contains ENT_LIST_MAX.
 */

#ifndef	ENTITY_ENTITY_H
#define	ENTITY_ENTITY_H

#define	ENT_LIST_MAX	200

// Default variables that every entity type should have
typedef struct{
	bool exists : 1;
} EntDefaults;

#endif
