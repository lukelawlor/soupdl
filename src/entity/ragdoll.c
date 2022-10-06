/*
 * ragdoll.c contains functions for manipulating ragdolls.
 */

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "../timestep.h"
#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "../collision.h"
#include "entity.h"
#include "particle.h"
#include "ragdoll.h"

// Array containing all ragdoll entities
static EntRagdoll ent_ragdoll_list[ENT_LIST_MAX];

// Constant pointer to the first index of the entity array
EntRagdoll *const ent_ragdoll = ent_ragdoll_list;

// Returns true if a ragdoll collides with a tile at it's position + xshift and yshift
static bool ent_ragdoll_tile_collide(EntRagdoll *e, float xshift, float yshift);

EntRagdoll *ent_ragdoll_new(float x, float y, float hsp, float vsp, ent_ragdoll_type type)
{
	// Index of next entity object to create in the list
	static int next_index = 0;

	EntRagdoll *e = &ent_ragdoll_list[next_index];
	e->x = x;
	e->y = y;
	e->hsp = hsp;
	e->vsp = vsp;
	e->bounce_frames = 0;
	e->grv = 0.2f;
	e->type = type;
	e->d.exists = true;
	if (++next_index >= ENT_LIST_MAX)
		next_index = 0;
	return e;
}

void ent_ragdoll_update(EntRagdoll *e)
{
	if (ent_ragdoll_tile_collide(e, e->hsp * g_ts, 0))
		e->hsp *= -0.9f;
	else
		e->x += e->hsp * g_ts;
	e->vsp += e->grv;
	if (ent_ragdoll_tile_collide(e, 0, e->vsp * g_ts))
	{
		e->bounce_frames = 14;
		e->vsp *= -0.9f;
		e->hsp *= 0.98f;
		ent_particle_new(e->x, e->y, PTCL_BUBBLE);
	}
	else
		e->y += e->vsp * g_ts;
}

void ent_ragdoll_draw(EntRagdoll *e)
{
	SDL_Rect drect = {e->x + g_cam.xshift, e->y + g_cam.yshift, 32, 32};
	SDL_Rect srect = {.w = 32, .h = 32};
	if (e->bounce_frames > 0)
	{
		srect.x = 64;
		srect.y = 32;
		e->bounce_frames--;
	}
	else
	{
		drect.x += 4;
		drect.y += 9;
		srect.x = 64;
		srect.y = 0;
	}
	SDL_RenderCopy(g_renderer, tex_egg, &srect, &drect);
}

void ent_ragdoll_destroy(EntRagdoll *e)
{
	e->d.exists = false;
}

// Returns true if a ragdoll collides with a tile at it's position + xshift and yshift
static bool ent_ragdoll_tile_collide(EntRagdoll *e, float xshift, float yshift)
{
	// Collision rectangle
	SDL_Rect crect = {e->x + xshift + 4, e->y + yshift + 4, 24, 24};

	return check_tile_rect(&crect);
}
