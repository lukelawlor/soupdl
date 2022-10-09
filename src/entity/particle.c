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

// Array containing all particle objects
static EntParticle ent_particle_list[ENT_LIST_MAX];

// Constant pointer to the first index of the particle array
EntParticle *const ent_particle = ent_particle_list;

// Sprite clips for different particle types
static SDL_Rect ent_particle_clip[PTCL_MAX] = {
	// PTCL_BUBBLE
	{0, 0, 10, 10},
	// PTCL_FLAME
	{10, 0, 10, 10},
	// PTCL_STAR
	{20, 0, 10, 10}
};

EntParticle *ent_particle_new(float x, float y, EntParticleId id)
{
	// Index of next entity object to create in the list
	static int next_index = 0;

	EntParticle *e = &ent_particle_list[next_index];
	e->x = x;
	e->y = y;
	e->dur = 360 + spdl_random();
	e->id = id;
	e->d.exists = true;
	
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
		e->hsp = (spdl_random() - 128) / (255.0f) * 0.5f;
		e->vsp = (spdl_random() - 128) / (255.0f) * 0.5f;
		break;
	case PTCL_STAR:
		e->grv = 0.01;
		e->hsp = (spdl_random() - 128) / (255.0f);
		e->vsp = -spdl_random() / (255.0f);
		break;
	}

	if (++next_index >= ENT_LIST_MAX)
		next_index = 0;
	return e;
}

void ent_particle_update(EntParticle *e)
{
	e->vsp += e->grv * g_ts;
	e->x += e->hsp * g_ts;
	e->y += e->vsp * g_ts;
	if ((e->dur -= g_ts) <= 0)
		ent_particle_destroy(e);
}

void ent_particle_draw(EntParticle *e)
{
	SDL_Rect *srect = &ent_particle_clip[e->id];
	SDL_Rect drect = {e->x + g_cam.xshift, e->y + g_cam.yshift, srect->w, srect->h};
	SDL_RenderCopy(g_renderer, tex_particle, srect, &drect);
}

void ent_particle_destroy(EntParticle *e)
{
	e->d.exists = false;
}
