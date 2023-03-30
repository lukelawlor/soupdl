/*
 * editor.h contains functions for map editing
 */

#ifndef	EDITOR_EDITOR_H
#define	EDITOR_EDITOR_H

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "../entity/tile.h"	// for EntTileId
#include "../tile/data.h"	// for TileId
#include "../void_rect.h"

// Entity tile type
typedef struct{
	// True if there's anything in the tile
	bool active : 1;

	// Id of entity tile
	EntTileId etid : 5;
} EntTile;

// Tile type
typedef enum{
	MAPED_TILE_TILE,
	MAPED_TILE_ENT
} MapEdTileType;

// Holds ids of next tile or ent to place 
typedef union{
	TileId tid;
	EntTileId etid;
} MapEdTile;

// Void rect type
typedef struct{
	// Current void rectangle being manipulated
	VoidRect *void_rect;

	// The void rectangle's SDL_Rect before being manipulated
	SDL_Rect rect;

	// Index in g_map.vr_list where the void rectangle resides
	int i;

	// Tile coordinates of where the mouse clicked on the void rectangle
	int x, y;
} MapEdVoidRect;

// Map editor mouse states
typedef enum{
	MAPED_STATE_NONE,
	MAPED_STATE_TILING,
	MAPED_STATE_ERASING,
	MAPED_STATE_VR_MOVING,
	MAPED_STATE_VR_RESIZING,
} MapEdState;

// Map editor object
typedef struct{
	// Next tile or entity to place
	MapEdTile tile;

	// Type of next thing to place
	MapEdTileType tile_type;

	// Info on the void rectangle being manipulated
	MapEdVoidRect void_rect;

	// Tiling state
	MapEdState state;

	// Width & height of tiling area
	int w, h;

	// True if the alt key is held
	bool alt;

} MapEd;

// Pointer to 2d array containing entity tiles
extern EntTile **g_ent_map;

// Initializes the map editor, returns nonzero on error
int maped_init(void);

// Resizes the map by (width_inc, height_inc), and returns nonzero on error
int maped_resize_map(int width_inc, int height_inc);

// Places or erases tiles at the cursor's position
// Called when ed->state == MAPED_STATE_TILING or MAPED_STATE_ERASING
void maped_tile(MapEd *ed);

// Moves void rectangles with the mouse
// Called when ed->state == MAPED_STATE_VR_MOVING
void maped_vr_move(MapEd *ed);

// Resizes void rectangles with the mouse
// Called when ed->state == MAPED_STATE_VR_RESIZING
void maped_vr_resize(MapEd *ed);

// Handles SDL keydown events
void maped_handle_keydown(MapEd *ed, SDL_Keycode key);

// Handles SDL keyup events
void maped_handle_keyup(MapEd *ed, SDL_Keycode key);

// Handles SDL mouse button down event
void maped_handle_mbdown(MapEd *ed, Uint8 button);

// Handles SDL mouse button up event
void maped_handle_mbup(MapEd *ed, Uint8 button);

// Ask the user to set contents of g_ent_door_map_path (defined in ../entity/door.h)
void maped_set_door_paths(void);

#endif
