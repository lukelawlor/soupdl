/*
 * draw.h contains functions for drawing parts of the editor UI.
 */

#ifndef	EDITOR_DRAW_H
#define	EDITOR_DRAW_H

#include <SDL2/SDL.h>		// for SDL_Texture

#include "../entity/id.h"	// for ENT_MAX

// Draw the entity map
void maped_draw_entmap(void);

// Draws map info text at the top left and an icon at the camera's position
void maped_draw_status(MapEd *ed);

#endif
