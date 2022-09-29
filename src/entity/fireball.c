/*
 * fireball.c contains functions for manipulating fireball entities.
 */

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "../timestep.h"
#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "../collision.h"
#include "entity.h"
#include "fireball.h"

// Array containing all fireball entities in the game
static EntFireball ent_fireball_list[ENT_LIST_MAX];

// Constant pointer to the first index of the entity array
EntFireball *const ent_fireball = ent_fireball_list;

// Source rectangles for drawing the fireballs
static SDL_Rect *g_srect = NULL;

EntFireball *ent_fireball_new(int x, int y, float hsp, float vsp)
{
	// Index of next entity object to create in the list
	static int next_index = 0;

	EntFireball *e = &ent_fireball_list[next_index];
	e->x = x;
	e->y = y;
	e->hsp = hsp;
	e->vsp = vsp;
	e->d.exists = true;
	if (++next_index >= ENT_LIST_MAX)
		next_index = 0;
	return e;
}

void ent_fireball_update(EntFireball *e)
{
	e->x += e->hsp * g_ts;
	e->y += e->vsp * g_ts;
	if (check_tile_point(e->x, e->y) != TILE_AIR)
		ent_fireball_destroy(e);
}

void ent_fireball_draw(EntFireball *e)
{
	// Change frames of animation
	if (--e->frame_tmr == 0u)
	{
		e->frame_tmr = 2u;
		e->frame = !e->frame;
	}

	// Render fireball
	SDL_Rect drect = {
		e->x - 8 + g_cam.xshift,
		e->y - 8 + g_cam.yshift,
		16,
		16
	};
	SDL_RenderCopy(g_renderer, e->frame ? tex_fireball1 : tex_fireball2, g_srect, &drect);
}

void ent_fireball_destroy(EntFireball *e)
{
	e->d.exists = false;
}
