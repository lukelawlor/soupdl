/*
 * slideguy.c contains functions for manipulating template entities.
 */

#include <SDL2/SDL.h>

#include "../timestep.h"
#include "../util/math.h"	// For clampf()

#include "c_body.h"
#include "c_evilegg.h"
#include "entity.h"

#include "player.h"
#include "slideguy.h"

// Max horizontal and vertical speeds for slideguys
#define	E_MAX_HSP	18
#define	E_MAX_VSP	13

EntSLIDEGUY *ent_new_SLIDEGUY(int x, int y, int hp, float acc, float jsp)
{
	ENT_NEW(SLIDEGUY);
	e->e.b = (EcmBody) {x, y, 31, 31, 0, 0, 0.2};
	e->e.spr = (EcmEvileggSpr) {SPR_EGG_IDLE, SDL_FLIP_NONE, 0};
	e->e.hp = hp;
	e->acc = acc;
	e->jsp = jsp;
	return e;
}

void ent_update_SLIDEGUY(EntSLIDEGUY *e)
{
	// Accelerate towards player
	if (g_player.b.x > e->e.b.x)
	{
		e->e.b.hsp += e->acc;
		e->e.spr.flip = SDL_FLIP_NONE;
	}
	else
	{
		e->e.b.hsp -= e->acc;
		e->e.spr.flip = SDL_FLIP_HORIZONTAL;
	}

	// Cap speeds and apply movement
	e->e.b.hsp = clampf(e->e.b.hsp, -E_MAX_HSP, E_MAX_HSP);
	if (ecm_body_move_hori(&e->e.b))
		e->e.b.hsp *= -1;

	e->e.b.vsp = clampf(e->e.b.vsp + e->e.b.grv * g_ts, -E_MAX_VSP, E_MAX_VSP);
	if (ecm_body_move_vert(&e->e.b))
	{
		if (e->e.b.vsp >= 0)
			e->e.b.vsp = e->jsp;
		else
			e->e.b.vsp = 0;
	}
	
	ecm_evilegg_update_animation(&e->e);
	if (ecm_evilegg_handle_collisions(&e->e))
		ent_destroy_SLIDEGUY(e);
}

void ent_draw_SLIDEGUY(EntSLIDEGUY *e)
{
	ecm_evilegg_draw(&e->e);
}

void ent_destroy_SLIDEGUY(EntSLIDEGUY *e)
{
	ecm_evilegg_die(&e->e);
	ENT_DEL_MARK(e);
}
