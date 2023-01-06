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

EntSLIDEGUY *ent_new_SLIDEGUY(int x, int y)
{
	ENT_NEW(SLIDEGUY);
	e->e.b = (EcmBody) {x, y, 31, 31, 0, 0, 0.2};
	e->e.spr = (EcmEvileggSpr) {SPR_EGG_IDLE, SDL_FLIP_NONE, 0};
	e->e.hp = 8;
	e->acc = 0.1f;
	return e;
}

void ent_update_SLIDEGUY(EntSLIDEGUY *e)
{
	// x position of the center of the slideguy
	int cx = e->e.b.x + 16;
	
	// x position of the center of the player
	int pcx = g_player.b.x + 16;

	// Accelerate towards player
	if (pcx > cx)
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
		e->e.b.hsp *= -0.9;

	e->e.b.vsp = clampf(e->e.b.vsp + e->e.b.grv * g_ts, -E_MAX_VSP, E_MAX_VSP);
	if (ecm_body_move_vert(&e->e.b))
		e->e.b.vsp = 0;
	
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
