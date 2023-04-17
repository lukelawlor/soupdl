/*
 * c_evilegg.c
 */

#include <stdbool.h>
#include <stdlib.h>	// For abs()

#include <SDL2/SDL.h>

#include "../video.h"
#include "../camera.h"
#include "../texture.h"
#include "../sound.h"
#include "../collision.h"
#include "../util/rep.h"

#include "c_body.h"
#include "c_sprite.h"
#include "c_evilegg.h"

#include "player.h"
#include "particle.h"
#include "fireball.h"
#include "item.h"		// For dropping items
#include "ragdoll.h"

#define	E_TEX	tex_evilegg

// Updates an egg's running animation
void ecm_evilegg_update_animation(EcmEvilegg *e)
{
	e->spr.anim_tick -= abs((int) e->b.hsp);
	if (e->spr.anim_tick <= 0)
	{
		e->spr.anim_tick = 7;
		e->spr.spr = e->spr.spr == SPR_EGG_RUN1 ? SPR_EGG_RUN2 : SPR_EGG_RUN1;
	}
}

// Draws an egg
void ecm_evilegg_draw(EcmEvilegg *e)
{
	const SDL_Rect *srect = &g_spr_egg[e->spr.spr];
	const SDL_Rect drect = {e->b.x + g_cam.xshift, e->b.y + g_cam.yshift, SPR_EGG_W, SPR_EGG_H};
	SDL_RenderCopyEx(g_renderer, E_TEX, srect, &drect, 0, NULL, e->spr.flip);
}

// Spawns bubble particles and an egg ragdoll, should be called before an egg entity is destroyed in its destroy function
void ecm_evilegg_die(EcmEvilegg *e)
{
	snd_play(snd_splode);
	REP (6)
		ent_new_PARTICLE(e->b.x, e->b.y, PTCL_BUBBLE);
	ent_new_RAGDOLL(e->b.x, e->b.y, e->b.hsp * -1.0, e->b.vsp - 2, RAGDOLL_EVILEGG);
}

// Handles collisions between the egg and spikes, fireballs, and the player
// Returns true if the egg dies from any damage it takes
bool ecm_evilegg_handle_collisions(EcmEvilegg *e)
{
	SDL_Rect crect = ECM_BODY_GET_CRECT(e->b);

	// Hitting a spike
	/*
	if (check_tile_rect_flags(&crect, TFLAG_SPIKE))
		return ecm_evilegg_damage(e);
	*/
	
	// Hitting a fireball
	{
		EntFIREBALL *fireball;
		if ((fireball = check_ent_fireball(&crect)) != NULL)
		{
			ent_destroy_FIREBALL(fireball);
			return ecm_evilegg_damage(e);
		}
	}

	// Hitting the player
	{
		if (check_rect(&crect, &g_player.crect))
			ent_player_damage(1);
	}

	return false;
}

// Returns true if the egg dies from any damage it takes
bool ecm_evilegg_damage(EcmEvilegg *e)
{
	e->spr.spr = SPR_EGG_FALL;
	e->spr.anim_tick = abs((int) e->b.hsp * 4);

	if (--e->hp <= 0)
		return true;

	snd_play(snd_splode);
	REP (3)
		ent_new_PARTICLE(e->b.x, e->b.y, PTCL_BUBBLE);
	return false;
}
