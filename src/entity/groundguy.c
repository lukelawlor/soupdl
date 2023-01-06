/*
 * groundguy.c contains functions for manipulating groundguy entities.
 */

#include <SDL2/SDL.h>

#include "../util/math.h"	// For signf()
#include "../timestep.h"
#include "../random.h"

#include "c_body.h"
#include "c_evilegg.h"
#include "entity.h"

#include "groundguy.h"

EntGROUNDGUY *ent_new_GROUNDGUY(int x, int y, float jsp)
{
	ENT_NEW(GROUNDGUY);
	e->e.b = (EcmBody) {x, y, 31, 31, 1 + (spdl_random() / 255.0f) * 2, 0, 0.05};
	e->e.spr = (EcmEvileggSpr) {SPR_EGG_IDLE, SDL_FLIP_NONE, 0};
	e->e.hp = 4;
	e->jsp = jsp;
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
	e->e.b.vsp += e->e.b.grv * g_ts;
	if (ecm_body_move_vert(&e->e.b))
	{
		if (e->e.b.vsp >= 0)
			e->e.b.vsp = e->jsp;
		else
			e->e.b.vsp = 0;
	}
	
	ecm_evilegg_update_animation(&e->e);
	if (ecm_evilegg_handle_collisions(&e->e))
		ent_destroy_GROUNDGUY(e);
}

void ent_draw_GROUNDGUY(EntGROUNDGUY *e)
{
	ecm_evilegg_draw(&e->e);
}

void ent_destroy_GROUNDGUY(EntGROUNDGUY *e)
{
	ecm_evilegg_die(&e->e);
	ENT_DEL_MARK(e);
}
