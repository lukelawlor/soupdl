/*
 * editor.h contains functions for map editing
 */

#ifndef	EDITOR_EDITOR_H
#define	EDITOR_EDITOR_H

#include <stdbool.h>

#include "../entity/all.h"	// for EntId

// Entity tile type
typedef struct{
	// True if there's anything in the tile
	bool active : 1;

	// Id of entity in the tile
	EntId eid : 3;
} EntTile;

// Pointer to 2d array containing entity tiles
extern EntTile **g_ent_map;

// Initializes the map editor
int maped_init(void);

#endif