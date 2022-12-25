/*
 * groundguy.c contains functions for manipulating groundguy entities.
 */

#include <stdlib.h>		// For abs()

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

#include "c_body.h"
#include "entity.h"

#include "particle.h"
#include "ragdoll.h"
#include "player.h"
#include "groundguy.h"

static void ent_damage_GROUNDGUY(EntGROUNDGUY *e);

EntGROUNDGUY *ent_new_GROUNDGUY(int x, int y)
{
	ENT_NEW(GROUNDGUY);
	e->b = (EcmBody) {x, y, 31, 31, 1 + (spdl_random() / 255.0f) * 2, 0, 0.05};
	e->s = (EcmEvileggSpr) {SPR_EGG_IDLE, SDL_FLIP_NONE, 0};
	e->hp = 4;
	return e;
}

void ent_update_GROUNDGUY(EntGROUNDGUY *e)
{
	// Movement
	if (ecm_body_move_hori(&e->b))
	{
		// Changing movement direction
		if (signf(e->b.hsp *= -1) == 1)
			e->s.flip = SDL_FLIP_NONE;
		else
			e->s.flip = SDL_FLIP_HORIZONTAL;
	}
	e->b.vsp += e->b.grv * g_ts;
	if (ecm_body_move_vert(&e->b))
		e->b.vsp = 0;
	
	// Updating animation
	e->s.anim_tick -= abs((int) e->b.hsp);
	if (e->s.anim_tick <= 0)
	{
		e->s.anim_tick = 7;
		e->s.spr = e->s.spr == SPR_EGG_RUN1 ? SPR_EGG_RUN2 : SPR_EGG_RUN1;
	}
	
	// Collision
	SDL_Rect crect = ECM_BODY_GET_CRECT(e->b);

	// Hitting a spike
	if (check_tile_rect_flags(&crect, TFLAG_SPIKE))
		ent_damage_GROUNDGUY(e);
	
	// Hitting a fireball
	{
		EntFIREBALL *fireball;
		if ((fireball = check_ent_fireball(&crect)) != NULL)
		{
			ent_destroy_FIREBALL(fireball);
			ent_damage_GROUNDGUY(e);
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
	const SDL_Rect *srect = &g_spr_egg[e->s.spr];
	const SDL_Rect drect = {e->b.x + g_cam.xshift, e->b.y + g_cam.yshift, SPR_EGG_W, SPR_EGG_H};
	SDL_RenderCopyEx(g_renderer, tex_evilegg, srect, &drect, 0, NULL, e->s.flip);
}

void ent_destroy_GROUNDGUY(EntGROUNDGUY *e)
{
	snd_play(snd_splode);
	REP (6)
		ent_new_PARTICLE(e->b.x, e->b.y, PTCL_BUBBLE);
	ent_new_RAGDOLL(e->b.x, e->b.y, e->b.hsp * -1.0, e->b.vsp - 2, RAGDOLL_EVILEGG);
	ENT_DEL_MARK(e);
}

static void ent_damage_GROUNDGUY(EntGROUNDGUY *e)
{
	if (--e->hp <= 0)
	{
		ent_destroy_GROUNDGUY(e);
		return;
	}
	snd_play(snd_splode);
	REP (3)
		ent_new_PARTICLE(e->b.x, e->b.y, PTCL_BUBBLE);
}
