/*
 * jumpguy.c contains functions for manipulating jumpguy entities.
 */

#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "../collision.h"
#include "../timestep.h"
#include "../random.h"
#include "../util.h"

#include "c_sprite.h"
#include "entity.h"

#include "player.h"
#include "particle.h"
#include "jumpguy.h"

EntJUMPGUY *ent_new_JUMPGUY(int x, int y)
{
	ENT_NEW(JUMPGUY);
	e->b = (EcmBody) {x, y, 31, 31, 1 + (spdl_random() / 255.0f) * 2, 0, 0.2};
	e->s = (EcmEvileggSpr) {SPR_EGG_IDLE, SDL_FLIP_NONE, 0};
	e->jsp = -5;
	return e;
}

void ent_update_JUMPGUY(EntJUMPGUY *e)
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
	{
		if (e->b.vsp > 0)
			e->b.vsp = e->jsp;
		else
			e->b.vsp = 0;
	}
	
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
		ent_destroy_JUMPGUY(e);
	
	// Hitting a fireball
	{
		EntFIREBALL *fireball;
		if ((fireball = check_ent_fireball(&crect)) != NULL)
		{
			ent_destroy_FIREBALL(fireball);
			ent_destroy_JUMPGUY(e);
		}
	}

	// Hitting the player
	{
		SDL_Rect prect = ECM_BODY_GET_CRECT(g_player.b);
		if (check_rect(&crect, &prect))
			ent_player_damage(1);
	}
}

/*
void ent_draw_JUMPGUY(EntJUMPGUY *e)
{
	
}
*/

void ent_destroy_JUMPGUY(EntJUMPGUY *e)
{
	ENT_DEL_MARK(e);
}
