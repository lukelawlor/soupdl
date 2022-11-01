/*
 * evilegg.c contains functions for manipulating evil egg entities.
 */

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "../timestep.h"
#include "../input.h"
#include "../util.h"
#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "../sound.h"
#include "../collision.h"
#include "all.h"

// Constants used by all evil egg entities
#define	ENT_EVILEGG_ACC		0.8f
#define	ENT_EVILEGG_DEC		0.1f
#define	ENT_EVILEGG_MAXSP	10.0f
#define	ENT_EVILEGG_GRV		0.1f
#define	ENT_EVILEGG_JSP		-6.0f

// Array containing all evil egg entities in the game
static EntEvilegg ent_evilegg_list[ENT_LIST_MAX];

// Constant pointer ot the first index of the evil egg entity array
EntEvilegg *const ent_evilegg = ent_evilegg_list;

// Hitbox rectangle (The x and y of this rectangle are relative to the actual position of the player)
static const SDL_Rect ent_evilegg_hrect = {6, 6, 20, 24};

EntEvilegg *ent_evilegg_new(float x, float y)
{
	// Index of next entity object to create in ent_evilegg_list
	static int next_index = 0;

	EntEvilegg *e = &ent_evilegg_list[next_index];
	e->x = x;
	e->y = y;

	// Initializing variables with default values
	e->d.exists = true;
	e->hsp = 0;
	e->vsp = 0;
	e->maxhp = 20;
	e->hp = e->maxhp;
	e->sprite = EGGSPR_IDLE;
	e->has_trumpet = (bool) (rand() % 2);
	e->trumpet_offset.x = 0;
	e->trumpet_offset.y = 14;
	e->flip = SDL_FLIP_HORIZONTAL;
	e->anim_step_frame = 0;
	e->anim_step_tmr = 0;
	e->anim_shoot_tmr = 0;

	if (++next_index >= ENT_LIST_MAX)
		next_index = 0;
	return e;
}

// Handle horizontal and vertical tile collision
static void ent_evilegg_move_hori(EntEvilegg *e);
static void ent_evilegg_move_vert(EntEvilegg *e);

// Returns true if an egg collides with a solid tile at it's position + xshift and yshift
static bool ent_evilegg_tile_collide(EntEvilegg *e, float xshift, float yshift);

// Changes the egg's sprite to one of 2 running frames, alternating on each call
static void ent_evilegg_anim_run(EntEvilegg *e);

// Damages the egg by power points and handles death
static void ent_evilegg_damage(EntEvilegg *e, int power);

void ent_evilegg_update(EntEvilegg *e)
{
	// Sign of movement speed
	short msign = 0;

	if (g_key_state[SDL_SCANCODE_LEFT])
	{
		e->flip = SDL_FLIP_HORIZONTAL;
		msign = -1;
		e->trumpet_offset.x = -10;
	}
	if (g_key_state[SDL_SCANCODE_RIGHT])
	{
		e->flip = SDL_FLIP_NONE;
		e->trumpet_offset.x = 24;
		if (msign == -1)
			msign = 0;
		else
			msign = 1;
	}
	
	// Affect horizontal speed
	if (msign == 0 && e->hsp != 0)
	{
		// Current sign of hsp
		int csign = signf(e->hsp);

		// Decelerate
		e->hsp -= csign * ENT_EVILEGG_ACC * g_ts;
		if (csign != sign(e->hsp))
			e->hsp = 0;
	}
	else
	{
		// Accelerate
		e->hsp += msign * ENT_EVILEGG_ACC * g_ts;
		if (e->hsp < -ENT_EVILEGG_MAXSP)
			e->hsp = -ENT_EVILEGG_MAXSP;
		else if (e->hsp > ENT_EVILEGG_MAXSP)
			e->hsp = ENT_EVILEGG_MAXSP;
	}

	// Add gravity to vertical speed
	e->vsp += ENT_EVILEGG_GRV * g_ts;

	// Movement and tile collision
	ent_evilegg_move_vert(e);
	ent_evilegg_move_hori(e);

	// Setting player sprite/animation
	if (e->anim_shoot_tmr > 0)
	{
		e->sprite = EGGSPR_SHOOT;
		e->anim_shoot_tmr--;
	}
	else
	{
		if (!ent_evilegg_tile_collide(e, 0, 1))
		{
			// Mid air animation
			if (--e->anim_step_tmr <= 0)
			{
				e->anim_step_tmr = 2;
				ent_evilegg_anim_run(e);
			}
		}
		else if (msign == 0)
		{
			e->sprite = EGGSPR_IDLE;
			e->anim_step_tmr = 0;
		}
		else
		{
			// Running animation
			if ((e->anim_step_tmr -= abs((int) e->hsp)) <= 0)
			{
				e->anim_step_tmr = (int) ENT_EVILEGG_MAXSP;
				ent_evilegg_anim_run(e);
				Mix_PlayChannel(-1, snd_step, 0);
			}
		}
	}

	// Collision rectangle
	SDL_Rect crect = {e->x + ent_evilegg_hrect.x, e->y + ent_evilegg_hrect.y, ent_evilegg_hrect.w, ent_evilegg_hrect.h};

	// Hitting a spike
	if (check_tile_rect_id(&crect, TILE_SPIKE))
		ent_evilegg_damage(e, 1);
	
}

void ent_evilegg_draw(EntEvilegg *e)
{
	// Egg source and destination rectangles
	SDL_Rect e_srect = {ent_eggspr_offset[e->sprite].x, ent_eggspr_offset[e->sprite].y, 32, 32};
	SDL_Rect e_drect = {e->x + g_cam.xshift, e->y + g_cam.yshift, 32, 32};

	SDL_RenderCopyEx(g_renderer, tex_evilegg, &e_srect, &e_drect, 0, NULL, e->flip);

	// Trumpet
	if (e->has_trumpet)
	{
		SDL_Rect trumpet_drect = {e_drect.x + e->trumpet_offset.x, e_drect.y + e->trumpet_offset.y, 19, 11};
		SDL_RenderCopyEx(g_renderer, tex_trumpet, NULL, &trumpet_drect, 0, NULL, e->flip);
	}

	// Hitbox
	//SDL_Rect frect = {p.x + p.hrect.x + g_cam.xshift, p.y + p.hrect.y + g_cam.yshift, p.hrect.w + 1, p.hrect.h + 2};
	//SDL_RenderFillRect(g_renderer, &frect);
}

void ent_evilegg_destroy(EntEvilegg *e)
{
	e->d.exists = false;
}

// Returns true if an egg collides with a solid tile at it's position + xshift and yshift
static bool ent_evilegg_tile_collide(EntEvilegg *e, float xshift, float yshift)
{
	// Collision rectangle
	SDL_Rect crect = {e->x + ent_evilegg_hrect.x + xshift, e->y + ent_evilegg_hrect.y + yshift, ent_evilegg_hrect.w, ent_evilegg_hrect.h};

	return check_tile_rect_flags(&crect, TFLAG_SOLID);
}

// Handle horizontal movement and tile collision
static void ent_evilegg_move_hori(EntEvilegg *e)
{
	if (ent_evilegg_tile_collide(e, e->hsp * g_ts, 0))
	{
		// Current hsp sign
		int csign = signf(e->hsp);

		// Number of loops
		int loops = 0;

		// Previous x value
		float previous_x = e->x;

		// Move closer to the tile until it is hit
		while (!ent_evilegg_tile_collide(e, csign, 0))
		{
			e->x += csign;
			
			// Break loop and print error if the loop continues for too long
			if (++loops > 100)
			{
				fprintf(stderr, "soupdl06: error: evilegg's horizontal tile collision loop failed\n");
				e->x = previous_x;
				break;
			}
		}
		e->hsp = 0;
	}
	else
		e->x += e->hsp * g_ts;
}

// Handle vertical movement and tile collision
static void ent_evilegg_move_vert(EntEvilegg *e)
{
	if (ent_evilegg_tile_collide(e, 0, e->vsp * g_ts))
	{
		// Current vsp sign
		int csign = signf(e->vsp);

		// Number of loops
		int loops = 0;

		// Previous y value
		float previous_y = e->y;
		
		// Move closer to the tile until it is hit
		while (!ent_evilegg_tile_collide(e, 0, csign))
		{
			e->y += csign;
			
			// Break loop and print error if the loop continues for too long
			if (++loops > 100)
			{
				fprintf(stderr, "soupdl06: error: evilegg's vertical tile collision loop failed\n");
				e->y = previous_y;
				break;
			}
		}
		e->vsp = 0;
	}
	else
		e->y += e->vsp * g_ts;
}

// Changes the egg's sprite to one of 2 running frames, alternating on each call
static void ent_evilegg_anim_run(EntEvilegg *e)
{
	if ((e->anim_step_frame = e->anim_step_frame ? 0 : 1) == 0)
	{
		e->sprite = EGGSPR_RUN1;
		e->trumpet_offset.y = 16;
	}
	else
	{
		e->sprite = EGGSPR_RUN2;
		e->trumpet_offset.y = 14;
	}
}

// Damages the egg by power points and handles death
static void ent_evilegg_damage(EntEvilegg *e, int power)
{
	e->hp -= power;
	for (int i = 0; i < 3; i++)
		ent_particle_new(e->x + 16, e->y + 16, PTCL_BUBBLE);
	Mix_PlayChannel(-1, snd_splode, 0);

	// Checking for player death
	if (e->hp <= 0)
	{
		for (int i = 0; i < 30; i++)
			ent_particle_new(e->x + 16, e->y + 16, PTCL_BUBBLE);
		ent_ragdoll_new(e->x, e->y, e->hsp * -1, -5, RAGDOLL_EVILEGG);
		ent_evilegg_destroy(e);
	}
}
