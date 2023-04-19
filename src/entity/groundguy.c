/*
 * groundguy.c contains functions for manipulating groundguy entities.
 */

#include <SDL2/SDL.h>

#include "../barrier.h"
#include "../error.h"
#include "../random.h"
#include "../timestep.h"
#include "../util/math.h"	// For signf()

#include "c_body.h"
#include "c_egg.h"
#include "entity.h"

#include "groundguy.h"

EntGROUNDGUY *ent_new_GROUNDGUY(int x, int y, float hsp, float jsp, bool stay_on_ledge, BarrierTag btag)
{
	ENT_NEW(GROUNDGUY);
	e->e.b = (EcmBody) {x, y, 31, 31, hsp, 0, 0.05*2};
	e->e.spr = (EcmEggSpr) {SPR_EGG_IDLE, TEX_EGG_EVIL, SDL_FLIP_NONE, 0};
	e->e.hp = 4;
	e->jsp = jsp;
	e->stay_on_ledge = stay_on_ledge;
	e->btag = btag;
	PINF("groundguy with btag %d created", (int) btag);
	return e;
}

void ent_update_GROUNDGUY(EntGROUNDGUY *e)
{
	// Movement
	if (ecm_body_move_hori(&e->e.b))
	{
		// Changing movement direction
		if (signf(e->e.b.hsp *= -1) == 1)
			e->e.spr.flip = SDL_FLIP_NONE;
		else
			e->e.spr.flip = SDL_FLIP_HORIZONTAL;
	}
	else if (e->stay_on_ledge && !ecm_body_tile_collide(&e->e.b, e->e.b.hsp * g_ts, 1))
	{
		// Don't walk off of ledges
		// Changing movement direction
		if (signf(e->e.b.hsp *= -1) == 1)
			e->e.spr.flip = SDL_FLIP_NONE;
		else
			e->e.spr.flip = SDL_FLIP_HORIZONTAL;
	}
	e->e.b.vsp += e->e.b.grv * g_ts;
	if (ecm_body_move_vert(&e->e.b))
	{
		// Jump when the ground is hit
		if (e->e.b.vsp >= 0)
			e->e.b.vsp = e->jsp;
		else
			e->e.b.vsp = 0;
	}
	
	ecm_egg_update_animation(&e->e);
	if (ecm_egg_handle_collisions(&e->e))
		ent_destroy_GROUNDGUY(e);
}

void ent_draw_GROUNDGUY(EntGROUNDGUY *e)
{
	ecm_egg_draw(&e->e);
}

void ent_destroy_GROUNDGUY(EntGROUNDGUY *e)
{
	barrier_send_check_request(e->btag);
	ecm_egg_die(&e->e);
	ENT_DEL_MARK(e);
}
