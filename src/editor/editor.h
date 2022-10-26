/*
 * editor.h contains functions for map editing
 */

#ifndef	EDITOR_EDITOR_H
#define	EDITOR_EDITOR_H

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "../entity/all.h"	// for EntId

// Entity tile type
typedef struct{
	// True if there's anything in the tile
	bool active : 1;

	// Id of entity in the tile
	EntId eid : 3;
} EntTile;

// Tile type
typedef enum{
	MAPED_TILE_TILE,
	MAPED_TILE_ENT
} MapEdTileType;

// Holds ids of next tile or ent to place 
typedef union{
	TileId tile;
	EntId ent;
} MapEdTile;

// Map editor mouse states
typedef enum{
	MAPED_STATE_NONE,
	MAPED_STATE_TILING,
	MAPED_STATE_ERASING
} MapEdState;


// Map editor object
typedef struct{
	// Next tile or entity to place
	MapEdTile tile;

	// Type of next thing to place
	MapEdTileType tile_type;

	// Tiling state
	MapEdState state;
} MapEd;

// Path to map file being edited
extern char *g_maped_file;

// Pointer to 2d array containing entity tiles
extern EntTile **g_ent_map;

// Initializes the map editor, returns nonzero on error
int maped_init(void);

// Resizes the map by (width_inc, height_inc), and returns nonzero on error
int maped_resize_map(int width_inc, int height_inc);

// Places or erases tiles at the cursor's position
void maped_tile(MapEd *ed);

// Handles SDL keydown events
void maped_handle_keydown(MapEd *ed, SDL_Keycode key);

// Handles SDL mouse button down event
void maped_handle_mbdown(MapEd *ed, Uint8 button);

// Handles SDL mouse button up event
void maped_handle_mbup(MapEd *ed, Uint8 button);

#endif
