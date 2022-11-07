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
#include "entity.h"
#include "all.h"
#include "c_body.h"
#include "c_sprite.h"
#include "player.h"

// Since g_player is used so frequently here, p is an alias for it
#define	p	g_player

// Player sprite width and height
#define	P_SPR_WIDTH		32
#define	P_SPR_HEIGHT		32

// The number of pixels a fireball will travel in a straight line at roughly 60fps
#define	P_FIREBALL_SPD		8

// Fireball knockback (speeds subtracted from player's hsp and vsp when a fireball is fired vertically or horizontally)
#define	P_FIREBALL_HKB		6
#define	P_FIREBALL_VKB		8

// # of frames to wait between fireballs hots
#define	P_SHOOT_COOLDOWN_RESET	10.0

// Initialization of player (see player.h for more detailed comments on EntPlayer variables)
EntPlayer g_player = {
	// Body
	.b = {
		.x = 0,
		.y = 0,
		.w = 20,
		.h = 24,
		.hsp = 0,
		.vsp = 0,
		.grv = 0.2,
	},

	// Horizontal speed, acceleration, deceleration, and maximum speed
	.acc = 0.6,
	.dec = 0.8,
	.maxhsp = 6,

	// Vertical speed and gravity
	.jsp = -7.5,
	.maxvsp = 6,

	// Jump timer (# of frames allowed for the player to jump after walking off of a cliff)
	.jtmr = 0,

	// Health
	.maxhp = 20,
	.hp = 20,

	// On ground
	.on_ground = false,

	// Weapon
	.has_trumpet = false,
	.trumpet_shots = 8,
	.shoot_cooldown = 0,

	// Invincibility
	.iframes = 0,

	// Drawing
	.flip = SDL_FLIP_HORIZONTAL,
	.sprite = EGGSPR_IDLE,
	.anim_step_frame = 0,
	.anim_step_tmr = 0,
	.anim_shoot_tmr = 0,
	.trumpet_offset = {0, 14}
};

// Changes the player's sprite to one of 2 running frames, alternating on each call
static void p_anim_run(void);

// Damages the player by power points, gives iframes, and handles death
static void p_damage(int power);

// Update the player's variables
void ent_player_update(void)
{
	// Sign of movement speed
	short msign = 0;

	// Detecting left & right movement
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

	// Jumping
	if (g_key_state[SDL_SCANCODE_Z])
	{
		if (p.jtmr > 0)
		{
			p.jtmr = 0;
			p.b.vsp = p.jsp;
			Mix_PlayChannel(-1, snd_step, 0);
		}
	}
	else if (p.b.vsp < 0)
		p.b.vsp = 0;
	
	// Affect horizontal speed
	if (msign == 0 && p.b.hsp != 0)
	{
		// Current sign of hsp
		int csign = signf(p.b.hsp);

		// Decelerate
		p.b.hsp -= csign * p.dec * g_ts;
		if (csign != signf(p.b.hsp))
			p.b.hsp = 0;
	}
	else
	{
		// Accelerate
		p.b.hsp += msign * p.acc * g_ts;
	}

	// Capping hsp
	p.b.hsp = clampf(p.b.hsp, -p.maxhsp, p.maxhsp);

	// Add gravity to vertical speed and capping it
	p.b.vsp = clampf(p.b.vsp + p.b.grv * g_ts, p.jsp, p.maxvsp);

	// Movement and tile collision
	if (ecm_body_move_vert(&p.b))
		p.b.vsp = 0; 
	if (ecm_body_move_hori(&p.b))
		p.b.hsp = 0;

	// Setting on ground state
	if ((p.on_ground = ecm_body_tile_collide(&p.b, 0, 1)))
	{
		p.jtmr = 6;
		p.trumpet_shots = 8;
	}
	else if (p.jtmr > 0)
		p.jtmr -= g_ts;
	
	// Set camera & draw position
	g_cam.x = p.b.x + 10;
	g_cam.y = p.b.y + 10;

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
			if ((p.anim_step_tmr -= abs((int) p.b.hsp)) <= 0)
			{
				p.anim_step_tmr = (int) p.maxhsp;
				p_anim_run();
				Mix_PlayChannel(-1, snd_step, 0);
			}
		}
	}

	// Collision rectangle
	SDL_Rect crect = ecm_body_get_crect(&p.b);

	// Picking up a trumpet
	EntItem *item;
	if (!p.has_trumpet && (item = check_ent_item(&crect)) != NULL)
	{
		ent_item_destroy(item);
		p.has_trumpet = true;
	}

	// Hitting a spike
	if (p.iframes <= 0)
	{
		if (check_tile_rect_flags(&crect, TFLAG_SPIKE))
			p_damage(1);
	}
	else
		p.iframes -= g_ts;

	// Shooting fireballs
	if (p.shoot_cooldown > 0)
		p.shoot_cooldown -= g_ts;

	if (g_key_state[SDL_SCANCODE_X] && p.has_trumpet && p.shoot_cooldown <= 0 && p.trumpet_shots > 0)
	{
		// Shoot a fireball
		p.shoot_cooldown = P_SHOOT_COOLDOWN_RESET;
		p.trumpet_shots--;

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

		p.b.hsp += -sign(fireball_hsp) * P_FIREBALL_HKB * g_ts;
		p.b.vsp += -sign(fireball_vsp) * P_FIREBALL_VKB * g_ts;

		ent_fireball_new(p.b.x + P_SPR_WIDTH / 2, p.b.y + P_SPR_HEIGHT / 2, fireball_hsp, fireball_vsp);
		p.sprite = EGGSPR_SHOOT;
		p.anim_shoot_tmr = 5;
		Mix_PlayChannel(-1, snd_shoot, 0);
	}
}

// Render the player
void ent_player_draw(void)
{
	// If the player has iframes and this value is false, the player won't be drawn every other frame so that they "blink"
	static bool iframes_blink = false;

	// Player source and destination rectangles
	SDL_Rect p_srect = {ent_eggspr_offset[p.sprite].x, ent_eggspr_offset[p.sprite].y, 32, 32};
	SDL_Rect p_drect = {p.b.x + g_cam.xshift - 6, p.b.y + g_cam.yshift - 6, P_SPR_WIDTH, P_SPR_HEIGHT};

	if (p.iframes > 0)
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
		ent_evilegg_new(p.b.x, p.b.y);
		break;
	}
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
	p.iframes = 60.0;
	for (int i = 0; i < 3; i++)
		ent_particle_new(p.b.x + 16, p.b.y + 16, PTCL_BUBBLE);
	Mix_PlayChannel(-1, snd_splode, 0);

	// Checking for player death
	if (p.hp <= 0)
	{
		for (int i = 0; i < 30; i++)
			ent_particle_new(p.b.x + 16, p.b.y + 16, PTCL_BUBBLE);
		ent_ragdoll_new(p.b.x, p.b.y, p.b.hsp * -1, -5, RAGDOLL_EGG);
		p.hp = 0;
	}
}
