/*
 * fireball.c contains functions for manipulating fireball entities.
 */

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "../timestep.h"
#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "../collision.h"
#include "entity.h"
#include "fireball.h"

EntFIREBALL *ent_new_FIREBALL(int x, int y, float hsp, float vsp)
{
	ENT_NEW(FIREBALL);
	e->x = x;
	e->y = y;
	e->hsp = hsp;
	e->vsp = vsp;
	return e;
}

void ent_update_FIREBALL(EntFIREBALL *e)
{
	e->x += e->hsp * g_ts;
	e->y += e->vsp * g_ts;
	if (g_tile_property[check_tile_point(e->x, e->y)].flags & TFLAG_SOLID)
		ent_destroy_FIREBALL(e);
}

void ent_draw_FIREBALL(EntFIREBALL *e)
{
	// Change frames of animation
	if (--e->frame_tmr == 0u)
	{
		e->frame_tmr = 2u;
		e->frame = !e->frame;
	}

	SDL_Rect srect = {.w = 16, .h = 16};
	if (e->frame == 0u)
		srect.x = 0;
	else
		srect.x = 16;

	// Render fireball
	SDL_Rect drect = {
		e->x - 8 + g_cam.xshift,
		e->y - 8 + g_cam.yshift,
		16,
		16
	};
	SDL_RenderCopy(g_renderer, tex_fireball, &srect, &drect);
}

void ent_destroy_FIREBALL(EntFIREBALL *e)
{
	ENT_DEL(e);
}
