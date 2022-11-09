/*
 * entity.h contains #includes for header files and macros that are needed in entity source files.
 */

#ifndef	ENTITY_ENTITY_H
#define	ENTITY_ENTITY_H

#include <stdbool.h>

#include "id.h"
#include "array.h"
#include "root.h"
#include "c_base.h"

// Shorthand for creating a pointer to an entity named e
#define	ENT_NEW(name)	Ent##name *e; \
			if ((e = ent_array_add(g_er[ENT_ID_##name])) == NULL) \
				return NULL; \
			e->base.s = ENT_STAT_NORM; \
			e->base.id = ENT_ID_##name; \
			e->base.i = g_er[ENT_ID_##name]->len - 1

// Shorthand for deleting an entity
#define	ENT_DEL(e)	ent_array_del(e->base.id, e->base.i)

// Shorthand for marking an entity for deletion, but not actually deleting it yet
#define	ENT_DEL_MARK(e)	e->base.s = ENT_STAT_DEL

#endif
