/*
 * ragdoll.c contains functions for manipulating ragdolls.
 */

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "../timestep.h"
#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "../collision.h"
#include "entity.h"
#include "particle.h"
#include "c_body.h"
#include "ragdoll.h"

EntRAGDOLL *ent_new_RAGDOLL(float x, float y, float hsp, float vsp, EntRagdollId rid)
{
	ENT_NEW(RAGDOLL);
	e->b.x = x;
	e->b.y = y;
	e->b.w = 31;
	e->b.h = 31;
	e->b.hsp = hsp;
	e->b.vsp = vsp;
	e->b.grv = 0.2f;
	e->bounce_frames = 0;
	e->rid = rid;
	return e;
}

void ent_update_RAGDOLL(EntRAGDOLL *e)
{
	if (ecm_body_tile_collide(&e->b, e->b.hsp * g_ts, 0))
		e->b.hsp *= -0.9f;
	else
		e->b.x += e->b.hsp * g_ts;
	e->b.vsp += e->b.grv;
	if (ecm_body_tile_collide(&e->b, 0, e->b.vsp * g_ts))
	{
		e->bounce_frames = 16;
		e->b.vsp *= -0.9f;
		e->b.hsp *= 0.98f;
	}
	else
		e->b.y += e->b.vsp * g_ts;
}

void ent_draw_RAGDOLL(EntRAGDOLL *e)
{
	SDL_Rect drect = {e->b.x + g_cam.xshift, e->b.y + g_cam.yshift, 32, 32};
	SDL_Rect srect = {.w = 32, .h = 32};
	if (e->bounce_frames > 0)
	{
		srect.x = 64;
		srect.y = 32;
		e->bounce_frames--;
	}
	else
	{
		srect.x = 64;
		srect.y = 0;
	}
	SDL_RenderCopy(g_renderer, e->rid == RAGDOLL_EGG ? tex_egg : tex_evilegg, &srect, &drect);
}

void ent_destroy_RAGDOLL(EntRAGDOLL *e)
{
	ENT_DEL(e);
}
