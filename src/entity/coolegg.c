/*
 * coolegg.c contains functions for manipulating coolegg entities.
 */

#include <stdlib.h>

#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "../random.h"
#include "../timestep.h"

#include "entity.h"
#include "c_body.h"
#include "c_egg.h"

#include "coolegg.h"

EntCOOLEGG *ent_new_COOLEGG(int x, int y)
{
	ENT_NEW(COOLEGG);
	e->e.b = (EcmBody) {x, y, 31, 31, 4, 0, 0.075};
	e->e.spr = (EcmEggSpr) {SPR_EGG_IDLE, TEX_EGG_COOL, SDL_FLIP_NONE, 0};
	e->e.hp = 8;
	return e;
}

void ent_update_COOLEGG(EntCOOLEGG *e)
{
	static int tick = 0;
	if (--tick <= 0)
	{
		e->e.b.hsp = ((spdl_random() - 128) / 128.0f) * 10;
		e->e.b.vsp = -(spdl_random()/ 255.0f) * 6;
		tick = 35 + (spdl_random() / 255.0f) * 70;
	}
	e->e.b.hsp *= 0.97;
	if (fabs(e->e.b.hsp) < 0.5f)
		e->e.spr.spr = SPR_EGG_IDLE;
	e->e.spr.flip = e->e.b.hsp > 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

	if (ecm_body_move_hori(&e->e.b))
		e->e.b.hsp = 0;
	e->e.b.vsp += e->e.b.grv * g_ts;
	if (ecm_body_move_vert(&e->e.b))
		e->e.b.vsp = 0;
	ecm_egg_update_animation(&e->e);
	if (ecm_egg_handle_collisions(&e->e))
		ent_destroy_COOLEGG(e);
}

void ent_draw_COOLEGG(EntCOOLEGG *e)
{
	ecm_egg_draw(&e->e);
}

void ent_destroy_COOLEGG(EntCOOLEGG *e)
{
	ecm_egg_die(&e->e);
	ENT_DEL_MARK(e);
}
