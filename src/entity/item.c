/*
 * item.c contains functions for manipulating item entities.
 */

#include <SDL2/SDL.h>

#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "entity.h"
#include "item.h"

// Array containing entity item type textures
static EntItemTex ent_item_tex[ENT_ITEM_MAX];

// Initializes the ent_item_tex array
void ent_item_init(void)
{
	ent_item_tex[ITEM_TRUMPET].tex = tex_trumpet;
	ent_item_tex[ITEM_TRUMPET].srect = NULL;
	ent_item_tex[ITEM_TRUMPET].w = 19;
	ent_item_tex[ITEM_TRUMPET].h = 11;
}

EntITEM *ent_new_ITEM(int x, int y, EntItemId id)
{
	ENT_NEW(ITEM);
	e->x = x;
	e->y = y;
	e->id = id;
	return e;
}

void ent_draw_ITEM(EntITEM *e)
{
	EntItemTex *tex = &ent_item_tex[e->id];
	SDL_Rect drect = {
		e->x - tex->w / 2 + g_cam.xshift,
		e->y - tex->h + g_cam.yshift,
		tex->w,
		tex->h
	};
	SDL_RenderCopy(g_renderer, tex->tex, tex->srect, &drect);
}

void ent_destroy_ITEM(EntITEM *e)
{
	ENT_DEL(e);
}
