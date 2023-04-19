/*
 * c_egg_evil.c contains functions that act on EcmEgg objects (defined in c_egg.h) that are used by evilegg entities.
 */

#include <SDL2/SDL.h>	// For SDL_Rect

#include "c_egg_evil.h"
#include "../collision.h"	// For check_rect()

// For player & fireball collision
#include "fireball.h"
#include "player.h"

// Handles collisions between the evilegg and fireballs or the player
// Returns true if the evilegg dies from any damage it takes
bool ecm_egg_evil_handle_collisions(EcmEgg *e)
{
	SDL_Rect crect = ECM_BODY_GET_CRECT(e->b);

	// Be damaged by fireballs
	{
		EntFIREBALL *fireball;
		if ((fireball = check_ent_fireball(&crect)) != NULL)
		{
			ent_destroy_FIREBALL(fireball);
			return ecm_egg_damage(e);
		}
	}

	// Damage the player when in contact with it
	{
		if (check_rect(&crect, &g_player.crect))
			ent_player_damage(1);
	}

	return false;
}
