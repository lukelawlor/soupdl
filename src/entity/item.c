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
static EntItemTex ent_item_tex[ITEM_MAX];

// Source rectangles for item textures
static SDL_Rect srect_heart = {
	0,
	16,
	16,
	16,
};
		

// Initializes the ent_item_tex array
void ent_item_init(void)
{
	ent_item_tex[ITEM_TRUMPET] = (EntItemTex) {
		.tex = tex_trumpet,
		.srect = NULL,
		.w = 19,
		.h = 11,
	};
	ent_item_tex[ITEM_CATFACE] = ent_item_tex[ITEM_COIN] = (EntItemTex) {
		.tex = tex_cakico,
		.srect = NULL,
		.w = 16,
		.h = 16,
	};
	ent_item_tex[ITEM_HEART] = (EntItemTex) {
		.tex = tex_heart,
		.srect = &srect_heart,
		.w = 16,
		.h = 16,
	};
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
