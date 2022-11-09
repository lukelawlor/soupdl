/*
 * root.h contains the extern definition for g_er, a pointer to the root entity array.
 *
 * The root entity array is an array of pointers to EntArray objects that contain arrays of all entity types in the game. The root entity array is indexed by entity ids defined in all.h. 
 *
 * For example, g_er[ENT_ID_ITEM] is a pointer to the entity array for item objects.
 */

#ifndef	ENTITY_ROOT_H
#define	ENTITY_ROOT_H

extern EntArray **const g_er;

// Initializes the root entity array, returns nonzero on error
int ent_root_array_init(void);

#endif
