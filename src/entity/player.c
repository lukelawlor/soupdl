/*
 * player.c contains functions for updating the player.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "../timestep.h"
#include "../error.h"
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

// Player sprite width and height
#define	P_SPR_WIDTH	32
#define	P_SPR_HEIGHT	32

// The number of pixels a fireball will travel in a straight line at roughly 60fps
#define	P_FIREBALL_SPD	12

// Initialization of player (see player.h for more detailed comments on EntPlayer variables)
EntPlayer g_player = {
	// Position
	.x = 0,
	.y = 0,

	// Horizontal speed, acceleration, deceleration, and maximum speed
	.hsp = 0,
	.acc = 0.4,
	.dec = 0.4,
	.maxsp = 6,

	// Vertical speed and gravity
	.vsp = 0,
	.grv = 0.3,
	.jsp = -9.5,

	// Jump timer (# of frames allowed for the player to jump after walking off of a cliff)
	.jtmr = 0,

	// Health
	.hp = 20,
	.maxhp = 20,

	// On ground
	.on_ground = false,

	// Weapon
	.has_trumpet = false,

	// Hitbox
	.hrect = {6, 6, 20, 24},

	// Invincibility
	.iframes = 0,
	.iframes_active = false,

	// Drawing
	.flip = SDL_FLIP_HORIZONTAL,
	.sprite = EGGSPR_IDLE,
	.anim_step_frame = 0,
	.anim_step_tmr = 0,
	.anim_shoot_tmr = 0,
	.trumpet_offset = {0, 14}
};

// Handle horizontal and vertical tile collision for the player
static void p_move_hori(void);
static void p_move_vert(void);

// Returns true if a player collides with a solid tile at it's position + xshift and yshift
static bool p_tile_collide(float xshift, float yshift);

// Changes the player's sprite to one of 2 running frames, alternating on each call
static void p_anim_run(void);

// Damages the player by power points, gives iframes, and handles death
static void p_damage(int power);

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
		if (p.on_ground)
		{
			// Current sign of hsp
			int csign = fsign(p.hsp);

			// Decelerate
			p.hsp -= csign * p.dec * g_ts;
			if (csign != sign(p.hsp))
				p.hsp = 0;
		}
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

	// Setting on ground state
	if ((p.on_ground = p_tile_collide(0, 1)))
	{
		p.jtmr = 6;
	}
	else if (p.jtmr > 0)
		p.jtmr -= g_ts;
	
	// Set camera & draw position
	g_cam.x = p.x + 16;
	g_cam.y = p.y + 16;

	// Setting player sprite/animation
	if (p.anim_shoot_tmr > 0)
	{
		p.sprite = EGGSPR_SHOOT;
		p.anim_shoot_tmr--;
	}
	else
	{
		if (!p.on_ground)
		{
			// Mid air animation
			if (--p.anim_step_tmr <= 0)
			{
				p.anim_step_tmr = 2;
				p_anim_run();
			}
		}
		else if (msign == 0)
		{
			p.sprite = EGGSPR_IDLE;
			p.anim_step_tmr = 0;
		}
		else
		{
			// Running animation
			if ((p.anim_step_tmr -= abs((int) p.hsp)) <= 0)
			{
				p.anim_step_tmr = (int) p.maxsp;
				p_anim_run();
				Mix_PlayChannel(-1, snd_step, 0);
			}
		}
	}

	// Collision rectangle
	SDL_Rect crect = {p.x + p.hrect.x, p.y + p.hrect.y, p.hrect.w, p.hrect.h};

	// Picking up a trumpet
	EntItem *item;
	if (!p.has_trumpet && (item = check_ent_item(&crect)) != NULL)
	{
		ent_item_destroy(item);
		p.has_trumpet = true;
	}

	// Hitting a spike
	if (!p.iframes_active)
	{
		if (check_tile_rect_id(&crect, TILE_SPIKE))
			p_damage(1);
	}
	else
	{
		if ((p.iframes -= g_ts) <= 0.0f)
			p.iframes_active = false;
	}
}

// Render the player
void ent_player_draw(void)
{
	// If the player has iframes and this value is false, the player won't be drawn every other frame so that they "blink"
	static bool iframes_blink = false;

	// Player source and destination rectangles
	SDL_Rect p_srect = {ent_eggspr_offset[p.sprite].x, ent_eggspr_offset[p.sprite].y, 32, 32};
	SDL_Rect p_drect = {p.x + g_cam.xshift, p.y + g_cam.yshift, P_SPR_WIDTH, P_SPR_HEIGHT};

	if (p.iframes_active)
	{
		if ((iframes_blink = !iframes_blink))
			SDL_RenderCopyEx(g_renderer, tex_egg, &p_srect, &p_drect, 0, NULL, p.flip);
	}
	else
		SDL_RenderCopyEx(g_renderer, tex_egg, &p_srect, &p_drect, 0, NULL, p.flip);

	// Trumpet
	if (p.has_trumpet)
	{
		SDL_Rect trumpet_drect = {p_drect.x + p.trumpet_offset.x, p_drect.y + p.trumpet_offset.y, 19, 11};
		SDL_RenderCopyEx(g_renderer, tex_trumpet, NULL, &trumpet_drect, 0, NULL, p.flip);
	}

	// Hitbox
	//SDL_Rect frect = {p.x + p.hrect.x + g_cam.xshift, p.y + p.hrect.y + g_cam.yshift, p.hrect.w + 1, p.hrect.h + 2};
	//SDL_RenderFillRect(g_renderer, &frect);
}

// Handle player keydown events
void ent_player_keydown(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_k:
		// Test killing the player
		p_damage(1);
		break;
	case SDLK_b:
		// Test evil egg spawn
		ent_evilegg_new(p.x, p.y);
		break;
	case SDLK_z:
		// Jump
		if (p.jtmr > 0)
		{
			p.jtmr = 0;
			p.vsp = p.jsp;
			Mix_PlayChannel(-1, snd_step, 0);
		}
		break;
	case SDLK_x:
		// Shoot a fireball
		if (p.has_trumpet)
		{
			// Fireball speeds
			int fireball_hsp, fireball_vsp;
			if (g_key_state[SDL_SCANCODE_UP])
			{
				fireball_hsp = 0;
				fireball_vsp = -P_FIREBALL_SPD;
			}
			else if (g_key_state[SDL_SCANCODE_DOWN])
			{
				fireball_hsp = 0;
				fireball_vsp = P_FIREBALL_SPD;
			}
			else
			{
				fireball_hsp = (p.flip == SDL_FLIP_NONE ? 1 : -1) * P_FIREBALL_SPD;
				fireball_vsp = 0;
			}

			p.hsp += -fireball_hsp;
			p.vsp += -fireball_vsp;

			ent_fireball_new(p.x + P_SPR_WIDTH / 2, p.y + P_SPR_HEIGHT / 2, fireball_hsp, fireball_vsp);
			p.sprite = EGGSPR_SHOOT;
			p.anim_shoot_tmr = 5;
			Mix_PlayChannel(-1, snd_shoot, 0);
		}
		break;
	}
}

// Returns true if a player collides with a solid tile at it's position + xshift and yshift
static bool p_tile_collide(float xshift, float yshift)
{
	// Collision rectangle
	SDL_Rect crect = {p.x + p.hrect.x + xshift, p.y + p.hrect.y + yshift, p.hrect.w, p.hrect.h};

	return check_tile_rect_flags(&crect, TFLAG_SOLID);
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
				PERR();
				fprintf(stderr, "player's horizontal tile collision loop failed\n");
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
				PERR();
				fprintf(stderr, "player's vertical tile collision loop failed\n");
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
	if ((p.anim_step_frame = p.anim_step_frame ? 0 : 1) == 0)
	{
		p.sprite = EGGSPR_RUN1;
		p.trumpet_offset.y = 16;
	}
	else
	{
		p.sprite = EGGSPR_RUN2;
		p.trumpet_offset.y = 14;
	}
}

// Damages the player by power points, gives iframes, and handles death
static void p_damage(int power)
{
	p.hp -= power;
	p.iframes = 60.0f;
	p.iframes_active = true;
	for (int i = 0; i < 3; i++)
		ent_particle_new(p.x + 16, p.y + 16, PTCL_BUBBLE);
	Mix_PlayChannel(-1, snd_splode, 0);

	// Checking for player death
	if (p.hp <= 0)
	{
		for (int i = 0; i < 30; i++)
			ent_particle_new(p.x + 16, p.y + 16, PTCL_BUBBLE);
		ent_ragdoll_new(p.x, p.y, p.hsp * -1, -5, RAGDOLL_EGG);
	}
}
