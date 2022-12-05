/*
 * groundguy.c contains functions for manipulating groundguy entities.
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "../util.h"		// For signf()
#include "../timestep.h"
#include "../random.h"
#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "../sound.h"
#include "../collision.h"
#include "entity.h"
#include "particle.h"
#include "player.h"
#include "c_body.h"
#include "groundguy.h"

EntGROUNDGUY *ent_new_GROUNDGUY(int x, int y)
{
	ENT_NEW(GROUNDGUY);
	e->b = (EcmBody) {x, y, 31, 31, 2, 0, 0.05};
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
	e->b.vsp += e->b.grv * g_ts;
	if (ecm_body_move_vert(&e->b))
		e->b.vsp = 0;
	
	// Collision
	SDL_Rect crect = ECM_BODY_GET_CRECT(e->b);

	// Hitting a spike
	if (check_tile_rect_flags(&crect, TFLAG_SPIKE))
		ent_destroy_GROUNDGUY(e);
	
	// Hitting a fireball
	{
		EntFIREBALL *fireball;
		if ((fireball = check_ent_fireball(&crect)) != NULL)
		{
			ent_destroy_FIREBALL(fireball);
			ent_destroy_GROUNDGUY(e);
		}
	}

	// Hitting the player
	{
		SDL_Rect prect = ECM_BODY_GET_CRECT(g_player.b);
		if (check_rect(&crect, &prect))
			ent_player_damage(1);
	}
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
	snd_play(snd_splode);
	ENT_DEL_MARK(e);
}
