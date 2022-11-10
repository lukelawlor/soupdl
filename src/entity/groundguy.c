/*
 * groundguy.c contains functions for manipulating groundguy entities.
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "../sound.h"
#include "../collision.h"
#include "../random.h"
#include "../util.h"		// For signf()
#include "entity.h"
#include "particle.h"
#include "c_body.h"
#include "groundguy.h"

EntGROUNDGUY *ent_new_GROUNDGUY(int x, int y)
{
	ENT_NEW(GROUNDGUY);
	e->b = (EcmBody) {x, y, 32, 32, 1.5, 0, 0.05};
	e->flip = SDL_FLIP_NONE;
	return e;
}

void ent_update_GROUNDGUY(EntGROUNDGUY *e)
{
	// Movement
	if (ecm_body_move_hori(&e->b))
	{
		// Changing movement direction
		if (signf(e->b.hsp *= -1) == 1)
			e->flip = SDL_FLIP_NONE;
		else
			e->flip = SDL_FLIP_HORIZONTAL;
	}
	e->b.vsp += e->b.grv;
	if (ecm_body_move_vert(&e->b))
	{
		if (e->b.vsp >= 0)
			e->b.vsp = -3;
		else
			e->b.vsp = 0;
	}
	
	// Collision
	SDL_Rect crect = ECM_BODY_GET_CRECT(e->b);

	// Hitting a spike
	if (check_tile_rect_flags(&crect, TFLAG_SPIKE))
		ent_destroy_GROUNDGUY(e);
	
	// Hitting a fireball
	if (check_ent_fireball(&crect) != NULL)
		ent_destroy_GROUNDGUY(e);
}

void ent_draw_GROUNDGUY(EntGROUNDGUY *e)
{
	SDL_Rect srect = {spdl_random() / 128 == 0 ? 0 : 32, 0, 32, 32};
	SDL_Rect drect = {e->b.x + g_cam.xshift, e->b.y + g_cam.yshift, 32, 32};
	SDL_RenderCopyEx(g_renderer, tex_evilegg, &srect, &drect, 0, NULL, e->flip);
}

void ent_destroy_GROUNDGUY(EntGROUNDGUY *e)
{
	for (int i = 0; i < 6; i++)
		ent_new_PARTICLE(e->b.x, e->b.y, PTCL_BUBBLE);
	Mix_PlayChannel(-1, snd_splode, 0);
	ENT_DEL_MARK(e);
}
