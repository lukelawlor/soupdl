/*
 * savebird.c contains functions for manipulating savebird entities.
 */

#include "../video.h"
#include "../texture.h"
#include "../camera.h"

#include "c_sprite.h"
#include "entity.h"

#include "savebird.h"

EntSAVEBIRD *ent_new_SAVEBIRD(int x, int y)
{
	ENT_NEW(SAVEBIRD);
	e->x = x;
	e->y = y;
	return e;
}

void ent_update_SAVEBIRD(EntSAVEBIRD *e)
{
	// Does nothing for now
}

void ent_draw_SAVEBIRD(EntSAVEBIRD *e)
{
	SDL_Rect drect = {e->x + g_cam.xshift, e->y + g_cam.yshift, SPR_EGG_W, SPR_EGG_H};
	SDL_RenderCopy(g_renderer, tex_evilegg, &g_spr_egg[SPR_EGG_WEIRD1], &drect);
}

void ent_destroy_SAVEBIRD(EntSAVEBIRD *e)
{
	ENT_DEL_MARK(e);
}
