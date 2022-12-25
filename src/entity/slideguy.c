/*
 * slideguy.c contains functions for manipulating template entities.
 */

#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "../sound.h"
#include "../random.h"
#include "../collision.h"
#include "../timestep.h"
#include "../util.h"		// For clampf()

#include "c_body.h"
#include "entity.h"

#include "player.h"
#include "particle.h"
#include "ragdoll.h"
#include "slideguy.h"

// Max horizontal and vertical speeds for slideguys
#define	E_MAX_HSP	18
#define	E_MAX_VSP	13

EntSLIDEGUY *ent_new_SLIDEGUY(int x, int y)
{
	ENT_NEW(SLIDEGUY);
	e->b = (EcmBody) {x, y, 31, 31, 0, 0, 0.2};
	e->s = (EcmEvileggSpr) {SPR_EGG_IDLE, SDL_FLIP_NONE, 0};
	e->acc = 0.1;
	return e;
}

void ent_update_SLIDEGUY(EntSLIDEGUY *e)
{
	// x position of the center of the slideguy
	int cx = e->b.x + 16;
	
	// x position of the center of the player
	int pcx = g_player.b.x + 16;

	// Move towards player
	if (pcx > cx)
	{
		e->b.hsp += e->acc;
		e->s.flip = SDL_FLIP_NONE;
	}
	else
	{
		e->b.hsp -= e->acc;
		e->s.flip = SDL_FLIP_HORIZONTAL;
	}

	e->b.hsp = clampf(e->b.hsp, -E_MAX_HSP, E_MAX_HSP);
	if (ecm_body_move_hori(&e->b))
		e->b.hsp *= -0.9;

	e->b.vsp = clampf(e->b.vsp + e->b.grv * g_ts, -E_MAX_VSP, E_MAX_VSP);
	if (ecm_body_move_vert(&e->b))
		e->b.vsp = 0;
	
	// Update animation
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
		ent_destroy_SLIDEGUY(e);
	
	// Hitting a fireball
	{
		EntFIREBALL *fireball;
		if ((fireball = check_ent_fireball(&crect)) != NULL)
		{
			ent_destroy_FIREBALL(fireball);
			ent_destroy_SLIDEGUY(e);
		}
	}

	// Hitting the player
	{
		SDL_Rect prect = ECM_BODY_GET_CRECT(g_player.b);
		if (check_rect(&crect, &prect))
			ent_player_damage(1);
	}
}

void ent_draw_SLIDEGUY(EntSLIDEGUY *e)
{
	const SDL_Rect *srect = &g_spr_egg[e->s.spr];
	const SDL_Rect drect = {e->b.x + g_cam.xshift, e->b.y + g_cam.yshift, SPR_EGG_W, SPR_EGG_H};
	SDL_RenderCopyEx(g_renderer, tex_evilegg, srect, &drect, 0, NULL, e->s.flip);
}

void ent_destroy_SLIDEGUY(EntSLIDEGUY *e)
{
	snd_play(snd_splode);
	REP (6)
		ent_new_PARTICLE(e->b.x, e->b.y, PTCL_BUBBLE);
	ent_new_RAGDOLL(e->b.x, e->b.y, e->b.hsp * -1.0, e->b.vsp - 2, RAGDOLL_EVILEGG);
	ENT_DEL_MARK(e);
}
