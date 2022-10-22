/*
 * draw.h contains functions for drawing parts of the editor UI.
 */

#ifndef	EDITOR_DRAW_H
#define	EDITOR_DRAW_H

#include <SDL2/SDL.h>		// for SDL_Texture

#include "../entity/all.h"	// for ENT_MAX

// EntTile texture
typedef struct{
	SDL_Texture *tex;
	SDL_Rect srect;
} EntTileTex;

// Array of EntTile textures indexed by entity ids (EntId defined in entity/all.h)
extern EntTileTex g_ent_tile_tex[ENT_MAX];

// Initializes g_ent_tile_tex (defined in draw.c)
void maped_init_ent_tile_tex(void);

// Draw the entity map
void maped_draw_entmap(void);

// Draw sprites indicating the status of the map editor at the camera's position
void maped_draw_status(MapEd *ed);

#endif
