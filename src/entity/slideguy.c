/*
 * slideguy.c contains functions for manipulating template entities.
 */

#include <SDL2/SDL.h>

#include "../timestep.h"
#include "../util/math.h"	// For clampf()

#include "c_body.h"
#include "c_egg.h"
#include "c_egg_evil.h"
#include "entity.h"

#include "player.h"
#include "slideguy.h"

// Max horizontal and vertical speeds for slideguys
#define	E_MAX_HSP	(18 * g_ts)
#define	E_MAX_VSP	(13 * g_ts)

EntSLIDEGUY *ent_new_SLIDEGUY(int x, int y, int hp, float acc, float jsp, BarrierTag btag)
{
	ENT_NEW(SLIDEGUY);
	e->e.b = (EcmBody) {x, y, 31, 31, 0, 0, 0.2};
	e->e.spr = (EcmEggSpr) {SPR_EGG_IDLE, TEX_EGG_EVIL, SDL_FLIP_NONE, 0};
	e->e.hp = hp;
	e->acc = acc;
	e->jsp = jsp;
	e->btag = btag;
	return e;
}

void ent_update_SLIDEGUY(EntSLIDEGUY *e)
{
	// Accelerate towards player
	if (g_player.b.x > e->e.b.x)
	{
		e->e.b.hsp += e->acc * g_ts;
		e->e.spr.flip = SDL_FLIP_NONE;
	}
	else
	{
		e->e.b.hsp -= e->acc * g_ts;
		e->e.spr.flip = SDL_FLIP_HORIZONTAL;
	}

	// Cap speeds and apply movement
	e->e.b.hsp = clampf(e->e.b.hsp, -E_MAX_HSP, E_MAX_HSP);
	if (ecm_body_move_hori(&e->e.b))
		e->e.b.hsp *= -1;

	e->e.b.vsp = clampf(e->e.b.vsp + e->e.b.grv * g_ts, -E_MAX_VSP, E_MAX_VSP);
	if (ecm_body_move_vert(&e->e.b))
	{
		// Jump when the ground is hit
		if (e->e.b.vsp >= 0)
			e->e.b.vsp = e->jsp;
		else
			e->e.b.vsp = 0;
	}
	
	ecm_egg_update_animation(&e->e);
	if (ecm_egg_evil_handle_collisions(&e->e))
		ent_destroy_SLIDEGUY(e);
}

void ent_draw_SLIDEGUY(EntSLIDEGUY *e)
{
	ecm_egg_draw(&e->e);
}

void ent_destroy_SLIDEGUY(EntSLIDEGUY *e)
{
	barrier_send_check_request(e->btag);
	ecm_egg_die(&e->e);
	ENT_DEL_MARK(e);
}
