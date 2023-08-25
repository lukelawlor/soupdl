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
#include "../random.h"
#include "entity.h"
#include "particle.h"
#include "c_body.h"
#include "c_sprite.h"
#include "ragdoll.h"

EntRAGDOLL *ent_new_RAGDOLL(float x, float y, float hsp, float vsp, TexEgg tex)
{
	ENT_NEW(RAGDOLL);
	e->b.x = x;
	e->b.y = y;
	e->b.w = 30;
	e->b.h = 30;
	e->b.hsp = hsp;
	e->b.vsp = vsp;
	e->tex = tex;
	e->b.grv = 0.2f;
	e->bounce_frames = 0;
	return e;
}

void ent_update_RAGDOLL(EntRAGDOLL *e)
{
	// Movement and collision
	if (ecm_body_tile_collide(&e->b, e->b.hsp * g_ts, 0))
		e->b.hsp *= -0.9f;
	else
		e->b.x += e->b.hsp * g_ts;
	e->b.vsp += e->b.grv * g_ts;
	if (ecm_body_tile_collide(&e->b, 0, e->b.vsp * g_ts))
	{
		e->bounce_frames = 16;
		e->b.vsp *= -0.9f;
		e->b.hsp *= 0.98f;
	}
	else
		e->b.y += e->b.vsp * g_ts;

	SDL_Rect crect = ECM_BODY_GET_CRECT(e->b);

	// Be knocked around by fireballs
	{
		EntFIREBALL *fireball;
		if ((fireball = check_ent_fireball(&crect)) != NULL)
		{
			ent_destroy_FIREBALL(fireball);
			e->b.hsp += fireball->hsp * 2;
			e->b.vsp += fireball->vsp * 2;
			e->b.vsp -= spdl_random() / 128.0f;
		}
	}
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

		// Move the falling egg sprite downwards so that the egg body in both sprites is displayed at the same position
		drect.y  += 10;
	}
	SDL_RenderCopy(g_renderer, g_tex_egg[e->tex], &srect, &drect);
}

void ent_destroy_RAGDOLL(EntRAGDOLL *e)
{
	ENT_DEL(e);
}
