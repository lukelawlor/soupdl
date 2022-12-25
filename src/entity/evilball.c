/*
 * evilball.c contains functions for manipulating evilball entities.
 */

#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "../timestep.h"
#include "../util.h"

#include "entity.h"

#include "particle.h"
#include "evilball.h"

EntEVILBALL *ent_new_EVILBALL(int x, int y, float hsp, float vsp)
{
	ENT_NEW(EVILBALL);
	e->x = x;
	e->y = y;
	e->hsp = hsp;
	e->vsp = vsp;
	e->destroy_ticks = 300;
	return e;
}

void ent_update_EVILBALL(EntEVILBALL *e)
{
	e->x += e->hsp * g_ts;
	e->y += e->vsp * g_ts;
	if ((e->destroy_ticks -= g_ts) <= 0)
		ent_destroy_EVILBALL(e);
}

void ent_draw_EVILBALL(EntEVILBALL *e)
{
	// Change frames of animation
	if (--e->frame_tmr == 0u)
	{
		e->frame_tmr = 5u;
		e->frame = !e->frame;
	}

	SDL_Rect srect = {.y = 16, .w = 16, .h = 16};
	if (e->frame == 0u)
		srect.x = 0;
	else
		srect.x = 16;

	// Render evilball
	SDL_Rect drect = {
		e->x - 8 + g_cam.xshift,
		e->y - 8 + g_cam.yshift,
		16,
		16
	};
	SDL_RenderCopy(g_renderer, tex_fireball, &srect, &drect);
	return;
}

void ent_destroy_EVILBALL(EntEVILBALL *e)
{
	REP (4)
		ent_new_PARTICLE(e->x, e->y, PTCL_BUBBLE);
	ENT_DEL_MARK(e);
}
