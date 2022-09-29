/*
 * player.c contains functions for updating the player.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "../timestep.h"
#include "../util.h"
#include "../video.h"
#include "../input.h"
#include "../texture.h"
#include "../sound.h"
#include "../camera.h"
#include "../collision.h"
#include "../entity/all.h"
#include "player.h"

// Since g_player is used so frequently here, p is an alias for it
#define	p	g_player

// Initialization of player (see player.h for more detailed comments on EntPlayer variables)
EntPlayer g_player = {
	// Position
	.x = 0,
	.y = 0,

	// Hitbox
	.hrect = {6, 6, 20, 24},

	// Horizontal speed, acceleration, deceleration, and maximum speed
	.hsp = 0,
	.acc = 0.4,
	.dec = 0.1,
	.maxsp = 10,

	// Vertical speed and gravity
	.vsp = 0,
	.grv = 0.1,
	.jsp = -12,

	// Weapon
	.has_trumpet = false,

	// Drawing
	.flip = SDL_FLIP_HORIZONTAL,
	.srect = {0, 32, 32, 32},
	.drect = {.w = 32, .h = 32},
	.trumpet_srect = {0, 0, 19, 11},
	.trumpet_drect = {.w = 19, .h = 11},
	.trumpet_offset = {0, 15},

	// Animation
	.anim = P_ANIM_IDLE,
	.anim_frame = 0,
	.anim_tmr = 0
};

// Handle horizontal and vertical tile collision for the player
static void p_move_hori(void);
static void p_move_vert(void);

// Returns true if a player collides with a tile at it's position + xshift and yshift
static bool p_tile_collide(float xshift, float yshift);

// Changes the player's sprite to one of 2 running frames, alternating on each call
static void p_anim_run(void);

// Update the player's variables
void ent_player_update(void)
{
	// Sign of movement speed
	short msign = 0;

	if (g_key_state[SDL_SCANCODE_LEFT])
	{
		p.flip = SDL_FLIP_HORIZONTAL;
		msign = -1;
		p.trumpet_offset.x = -10;
	}
	if (g_key_state[SDL_SCANCODE_RIGHT])
	{
		p.flip = SDL_FLIP_NONE;
		p.trumpet_offset.x = 24;
		if (msign == -1)
			msign = 0;
		else
			msign = 1;
	}
	
	// Affect horizontal speed
	if (msign == 0 && p.hsp != 0)
	{
		// Current sign of hsp
		int csign = fsign(p.hsp);

		// Decelerate
		p.hsp -= csign * p.acc * g_ts;
		if (csign != sign(p.hsp))
			p.hsp = 0;
	}
	else
	{
		// Accelerate
		p.hsp += msign * p.acc * g_ts;
		if (p.hsp > p.maxsp)
			p.hsp = p.maxsp;
		else if (p.hsp < -p.maxsp)
			p.hsp = -p.maxsp;
	}

	// Add gravity to vertical speed
	p.vsp += p.grv * g_ts;
	
	// Movement and tile collision
	p_move_vert();
	p_move_hori();
	
	// Set camera & draw position
	g_cam.x = p.x + 16;
	g_cam.y = p.y + 16;

	// Setting player sprite/animation
	if (!p_tile_collide(0, 1))
	{
		// Mid air animation
		p.anim = P_ANIM_AIR;
		if (--p.anim_tmr <= 0)
		{
			p.anim_tmr = 2;
			p_anim_run();
		}
	}
	else if (msign == 0)
	{
		// Idle animation
		p.anim = P_ANIM_IDLE;
		p.srect.x = 0;
		p.srect.y = 32;
	}
	else
	{
		// Running animation
		if (p.anim == P_ANIM_IDLE)
			p.anim_tmr = 0;
		p.anim = P_ANIM_RUN;
		if ((p.anim_tmr -= abs((int) p.hsp)) <= 0)
		{
			p.anim_tmr = (int) p.maxsp;
			p_anim_run();
			Mix_PlayChannel(-1, snd_step, 0);
		}
	}

	// Collision rectangle
	SDL_Rect crect = {p.x + p.hrect.x, p.y + p.hrect.y, p.hrect.w, p.hrect.h};

	// Picking up a trumpet
	EntItem *item;
	if (!p.has_trumpet && (item = check_ent_item(crect)) != NULL)
	{
		ent_item_destroy(item);
		p.has_trumpet = true;
	}
}

// Render the player
void ent_player_draw(void)
{
	// Player
	p.drect.x = p.x + g_cam.xshift;
	p.drect.y = p.y + g_cam.yshift;
	SDL_RenderCopyEx(g_renderer, tex_egg, &p.srect, &p.drect, 0, NULL, p.flip);

	// Trumpet
	if (p.has_trumpet)
	{
		p.trumpet_drect.x = p.drect.x + p.trumpet_offset.x;
		p.trumpet_drect.y = p.drect.y + p.trumpet_offset.y;
		SDL_RenderCopyEx(g_renderer, tex_trumpet, &p.trumpet_srect, &p.trumpet_drect, 0, NULL, p.flip);
	}

	// Hitbox
	SDL_Rect frect = {p.x + p.hrect.x + g_cam.xshift, p.y + p.hrect.y + g_cam.yshift, p.hrect.w + 1, p.hrect.h + 2};
	//SDL_RenderFillRect(g_renderer, &frect);
}

// Handle player keydown events
void ent_player_keydown(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_z:
		if (p_tile_collide(0, 1))
		{
			p.vsp = p.jsp;
			Mix_PlayChannel(-1, snd_step, 0);
		}
		break;
	case SDLK_g:
		for (int y = 0; y < g_room_height; y++)
		{
			for (int x = 0; x < g_room_width; x++)
			{
				fprintf(stderr, "%d", g_tile_space[x][y]->tile);
			}
			fprintf(stderr, "\n");
		}
		break;
	}
}

// Returns true if a player collides with a tile at it's position + xshift and yshift
static bool p_tile_collide(float xshift, float yshift)
{
	// Point to check
	int x = p.x + p.hrect.x + xshift;
	int y = p.y + p.hrect.y + yshift;

	// Top left corner
	if (check_tile_point(x, y) != TILE_AIR)
		return true;

	// Top right corner
	if (check_tile_point(x + p.hrect.w, y) != TILE_AIR)
		return true;

	// Bottom left corner
	if (check_tile_point(x, y + p.hrect.h) != TILE_AIR)
		return true;

	// Bottom right corner
	if (check_tile_point(x + p.hrect.w, y + p.hrect.h) != TILE_AIR)
		return true;
	
	return false;
}

// Handle horizontal tile collision for the player
static void p_move_hori(void)
{
	if (p_tile_collide(p.hsp * g_ts, 0))
	{
		// Current hsp sign
		int csign = fsign(p.hsp);

		// Number of loops
		int loops = 0;

		// Previous x value
		float previous_x = p.x;

		// Move closer to the tile until it is hit
		while (!p_tile_collide(csign, 0))
		{
			p.x += csign;
			
			// Break loop and print error if the loop continues for too long
			if (++loops > 100)
			{
				fprintf(stderr, "soupdl06: error: player's horizontal tile collision loop failed\n");
				p.x = previous_x;
				break;
			}
		}
		p.hsp = 0;
	}
	else
		p.x += p.hsp * g_ts;
}

// Handle vertical tile collision for the player
static void p_move_vert(void)
{
	if (p_tile_collide(0, p.vsp * g_ts))
	{
		// Current vsp sign
		int csign = fsign(p.vsp);

		// Number of loops
		int loops = 0;

		// Previous y value
		float previous_y = p.y;
		
		// Move closer to the tile until it is hit
		while (!p_tile_collide(0, csign))
		{
			p.y += csign;
			
			// Break loop and print error if the loop continues for too long
			if (++loops > 100)
			{
				fprintf(stderr, "soupdl06: error: player's vertical tile collision loop failed\n");
				p.y = previous_y;
				break;
			}
		}
		p.vsp = 0;
	}
	else
		p.y += p.vsp * g_ts;
}

// Changes the player's sprite to one of 2 running frames, alternating on each call
static void p_anim_run(void)
{
	if ((p.anim_frame = p.anim_frame ? 0 : 1) == 0)
	{
		p.srect.x = 32;
		p.srect.y = 0;
		p.trumpet_offset.y = 16;
	}
	else
	{
		p.srect.x = 0;
		p.srect.y = 0;
		p.trumpet_offset.y = 14;
	}
}
