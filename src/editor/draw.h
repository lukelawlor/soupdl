/*
 * draw.h contains functions for drawing parts of the editor UI.
 */

#ifndef	EDITOR_DRAW_H
#define	EDITOR_DRAW_H

#include <SDL2/SDL.h>		// for SDL_Texture

#include "../entity/id.h"	// for ENT_MAX

// Draws all components of the map editor
void maped_draw_all(MapEd *ed);

#endif
