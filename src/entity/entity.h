/*
 * entity.h contains #includes for header files that are needed in entity source files.
 */

#ifndef	ENTITY_ENTITY_H
#define	ENTITY_ENTITY_H

#include <stdbool.h>

#include "array.h"
#include "root.h"
#include "all.h"

// Shorthand for creating a pointer to an entity named e
#define	ENT_NEW(name)	Ent##name *e; \
			if ((e = ent_array_add(g_er[ENT_ID_##name])) == NULL) \
				return NULL; \
			e->id.a = g_er[ENT_ID_##name]; \
			e->id.p = g_er[ENT_ID_##name]->len - 1

// Shorthand for deleting an entity
#define	ENT_DEL(e_ptr)	ent_array_del(&e_ptr->id)

#endif
