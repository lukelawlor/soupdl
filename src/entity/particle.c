/*
 * particle.c contains functions for manipulating particles.
 */

#include <SDL2/SDL.h>

#include "../timestep.h"
#include "../random.h"
#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "entity.h"
#include "particle.h"

// Sprite clips for different particle types
static SDL_Rect ent_particle_clip[PTCL_MAX] = {
	// PTCL_BUBBLE
	{0, 0, 10, 10},
	// PTCL_FLAME
	{10, 0, 10, 10},
	// PTCL_STAR
	{20, 0, 10, 10}
};

EntPARTICLE *ent_new_PARTICLE(float x, float y, EntParticleId id)
{
	ENT_NEW(PARTICLE);
	e->x = x;
	e->y = y;
	e->dur = 360 + spdl_random();
	e->id = id;
	
	// Initialize variables for the particle's specific type
	switch (id)
	{
	case PTCL_BUBBLE:
		e->grv = 0.04;
		e->hsp = (spdl_random() - 128) / (255.0f) * 2;
		e->vsp = -spdl_random() / (255.0f) * 2;
		break;
	case PTCL_FLAME:
		e->grv = 0;
		e->dur = (e->dur - 360) / 2;
		e->hsp = (spdl_random() - 128) / (255.0f) * 1.6f;
		e->vsp = (spdl_random() - 128) / (255.0f) * 1.6f;
		break;
	case PTCL_STAR:
		e->grv = 0.01;
		e->hsp = (spdl_random() - 128) / (255.0f);
		e->vsp = -spdl_random() / (255.0f);
		break;
	}

	return e;
}

void ent_update_PARTICLE(EntPARTICLE *e)
{
	e->vsp += e->grv * g_ts;
	e->x += e->hsp * g_ts;
	e->y += e->vsp * g_ts;
	if ((e->dur -= g_ts) <= 0)
		ent_destroy_PARTICLE(e);
}

void ent_draw_PARTICLE(EntPARTICLE *e)
{
	SDL_Rect *srect = &ent_particle_clip[e->id];
	SDL_Rect drect = {e->x + g_cam.xshift, e->y + g_cam.yshift, srect->w, srect->h};
	SDL_RenderCopy(g_renderer, tex_particle, srect, &drect);
}

void ent_destroy_PARTICLE(EntPARTICLE *e)
{
	ENT_DEL(e);
}
