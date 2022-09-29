/*
 * item.c contains functions for manipulating item entities.
 */

#include <SDL2/SDL.h>

#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "entity.h"
#include "item.h"

// Array containing all item entities in the game
static EntItem ent_item_list[ENT_LIST_MAX];

// Constant pointer to the first index of the item entity array
EntItem *const ent_item = ent_item_list;

// Array containing entity item type textures
static EntItemTex ent_item_tex[ENT_ITEM_MAX];

// Initializes the ent_item_tex array
int ent_item_init(void)
{
	ent_item_tex[0].tex = tex_trumpet;
	ent_item_tex[0].srect = NULL;
	ent_item_tex[0].w = 19;
	ent_item_tex[0].h = 11;
	return 0;
}

EntItem *ent_item_new(int x, int y, item_type type)
{
	// Index of next entity object to create in ent_item_list
	static int next_index = 0;

	EntItem *e = &ent_item_list[next_index];
	e->x = x;
	e->y = y;
	e->type = type;
	e->d.exists = true;
	if (++next_index >= ENT_LIST_MAX)
		next_index = 0;
	return e;
}

void ent_item_draw(EntItem *e)
{
	EntItemTex *tex = &ent_item_tex[e->type];
	SDL_Rect drect = {
		e->x - tex->w / 2 + g_cam.xshift,
		e->y - tex->h + g_cam.yshift,
		tex->w,
		tex->h
	};
	SDL_RenderCopy(g_renderer, tex->tex, tex->srect, &drect);

	// Draw a hitbox (not 100% accurate, see ../collision.c for hold item collision is handled)
	/*
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 255, 100);
	SDL_RenderFillRect(g_renderer, &drect);
	*/
}

void ent_item_destroy(EntItem *e)
{
	e->d.exists = false;
}
