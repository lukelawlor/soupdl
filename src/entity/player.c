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
#include "../util/rep.h"
#include "../util/math.h"
#include "../video.h"
#include "../input.h"
#include "../texture.h"
#include "../sound.h"
#include "../camera.h"
#include "../collision.h"

#include "all.h"
#include "c_body.h"
#include "c_sprite.h"
#include "entity.h"

#include "player.h"

// Since g_player is used so frequently here, p is an alias for it
#define	p	g_player

// Player controls
#define	P_KEY_UP	SDL_SCANCODE_W
#define	P_KEY_DOWN	SDL_SCANCODE_S
#define	P_KEY_LEFT	SDL_SCANCODE_A
#define	P_KEY_RIGHT	SDL_SCANCODE_D
#define	P_KEY_JUMP	SDL_SCANCODE_K
#define	P_KEY_SHOOT	SDL_SCANCODE_J

// The number of pixels a fireball will travel in a straight line at roughly 60fps
#define	P_FIREBALL_SPD		8

// Fireball knockback (speeds subtracted from player's hsp and vsp when a fireball is fired vertically or horizontally)
#define	P_FIREBALL_HKB		4
#define	P_FIREBALL_VKB		8

// # of frames to wait between fireballs hots
#define	P_SHOOT_COOLDOWN_RESET	10

// # of fireballs the player can shoot once it hits the ground
#define	P_SHOOT_RESET		8

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
		.grv = 0.15,
	},

	// Horizontal speed, acceleration, deceleration, and maximum speed
	.acc = 0.4,
	.dec = 0.4,
	.maxhsp = 5,

	// Vertical speed and gravity
	.jsp = -5.6,
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
	.trumpet_shots = 0,
	.trumpet_shots_reset = 0,
	.shoot_cooldown = 0,

	// Invincibility
	.iframes = 0,

	// Drawing
	.flip = SDL_FLIP_HORIZONTAL,
	.sprite = SPR_EGG_IDLE,
	.anim_step_frame = 0,
	.anim_step_tmr = 0,
	.anim_shoot_tmr = 0,
	.trumpet_offset = {0, 14}
};

// Changes the player's sprite to one of 2 running frames, alternating on each call
static void p_anim_run(void);

// Update the player's variables
void ent_player_update(void)
{
	// Setting on ground flag
	p.on_ground = ecm_body_tile_collide(&p.b, 0, 1);

	// Sign of movement speed
	short msign = 0;

	// Detecting left & right movement
	if (g_key_state[P_KEY_LEFT])
	{
		if (g_key_state[P_KEY_RIGHT])
			goto l_move_done;

		p.flip = SDL_FLIP_HORIZONTAL;
		msign = -1;
		p.trumpet_offset.x = -10;
	}
	else if (g_key_state[P_KEY_RIGHT])
	{
		p.flip = SDL_FLIP_NONE;
		p.trumpet_offset.x = 24;
		msign = 1;
	}
l_move_done:

	// Jumping
	if (g_key_state[P_KEY_JUMP])
	{
		if (p.jtmr > 0)
		{
			p.jtmr = 0;
			p.b.vsp = p.jsp;
			snd_play(snd_step);
		}
	}
	else if (p.b.vsp < 0)
		p.b.vsp = 0;
	
	// Affect horizontal speed
	if (msign == 0 && p.b.hsp != 0 && p.on_ground)
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
	if (p.on_ground)
	{
		p.jtmr = 6;
		p.trumpet_shots = p.trumpet_shots_reset;
	}
	else if (p.jtmr > 0)
		p.jtmr -= g_ts;
	
	// Set camera & draw position
	g_cam.x = p.b.x + 10;
	g_cam.y = p.b.y + 10;

	// Setting player sprite/animation
	if (p.anim_shoot_tmr > 0)
	{
		p.sprite = SPR_EGG_SHOOT;
		p.anim_shoot_tmr--;
	}
	else
	{
		if (!p.on_ground)
		{
			// Mid air animation
			if (g_key_state[P_KEY_LEFT] && g_key_state[P_KEY_RIGHT])
			{
				if (--p.anim_step_tmr <= 0)
				{
					p.anim_step_tmr = 2;
					p.sprite = p.sprite == SPR_EGG_DRIFT1 ? SPR_EGG_DRIFT2 : SPR_EGG_DRIFT1;
				}
				if (p.b.hsp < 0)
				{
					p.flip = SDL_FLIP_NONE;
					p.trumpet_offset.x = 23;
					p.trumpet_offset.y = 11;
				}
				else
				{
					p.flip = SDL_FLIP_HORIZONTAL;
					p.trumpet_offset.x = -12;
					p.trumpet_offset.y = 11;
				}
			}
			else if (--p.anim_step_tmr <= 0)
			{
				p.anim_step_tmr = 2;
				p_anim_run();
			}
		}
		else if (msign == 0)
		{
			p.sprite = SPR_EGG_IDLE;
			p.anim_step_tmr = 0;
		}
		else
		{
			// Running animation
			if ((p.anim_step_tmr -= abs((int) p.b.hsp)) <= 0)
			{
				p.anim_step_tmr = (int) p.maxhsp;
				p_anim_run();
				snd_play(snd_step);
			}
		}
	}

	// Collision rectangle
	SDL_Rect crect = ECM_BODY_GET_CRECT(p.b);

	// Decrementing iframes so the player isn't invincible forever
	if (p.iframes > 0)
		p.iframes -= g_ts;

	// Hitting a spike
	if (check_tile_rect_flags(&crect, TFLAG_SPIKE))
		ent_player_damage(1);
	
	// Hitting an evilball
	{
		EntEVILBALL *evil;
		if ((evil = check_ent_evilball(&crect)) != NULL)
			ent_player_damage(1);
	}

	// Picking up a trumpet
	{
		EntITEM *item;
		if ((!p.has_trumpet || true) && (item = check_ent_item(&crect)) != NULL)
		{
			REP (10)
				ent_new_PARTICLE(p.b.x, p.b.y, PTCL_STAR);
			ent_destroy_ITEM(item);
			snd_play(snd_bubble);
			p.has_trumpet = true;
			p.trumpet_shots_reset++;
		}
	}

	// Shooting fireballs
	if (p.shoot_cooldown > 0)
		p.shoot_cooldown -= g_ts;

	if (g_key_state[P_KEY_SHOOT] && p.has_trumpet && p.shoot_cooldown <= 0 && p.trumpet_shots > 0)
	{
		// Shoot a fireball
		p.shoot_cooldown = P_SHOOT_COOLDOWN_RESET;
		p.trumpet_shots--;

		// Fireball speeds
		int fireball_hsp, fireball_vsp;
		if (g_key_state[P_KEY_UP])
		{
			fireball_hsp = 0;
			fireball_vsp = -P_FIREBALL_SPD;
		}
		else if (g_key_state[P_KEY_DOWN])
		{
			fireball_hsp = 0;
			fireball_vsp = P_FIREBALL_SPD;
		}
		else
		{
			fireball_hsp = (p.flip == SDL_FLIP_NONE ? 1 : -1) * P_FIREBALL_SPD;
			fireball_vsp = 0;
		}

		p.b.hsp += -sign(fireball_hsp) * P_FIREBALL_HKB;
		p.b.vsp += -sign(fireball_vsp) * P_FIREBALL_VKB;

		ent_new_FIREBALL(p.b.x + 16, p.b.y + 16, fireball_hsp, fireball_vsp);
		p.sprite = SPR_EGG_SHOOT;
		p.anim_shoot_tmr = 5;
		snd_play(snd_shoot);
	}
}

// Render the player
void ent_player_draw(void)
{
	// If the player has iframes and this value is false, the player won't be drawn every other frame so that they "blink"
	static bool iframes_blink = false;

	// Player source and destination rectangles
	const SDL_Rect *p_srect = &g_spr_egg[p.sprite];
	const SDL_Rect p_drect = {p.b.x + g_cam.xshift - 6, p.b.y + g_cam.yshift - 6, SPR_EGG_W, SPR_EGG_H};

	// Trumpet
	if (p.has_trumpet)
	{
		SDL_Rect trumpet_drect = {p_drect.x + p.trumpet_offset.x, p_drect.y + p.trumpet_offset.y, 19, 11};
		SDL_RenderCopyEx(g_renderer, tex_trumpet, NULL, &trumpet_drect, 0, NULL, p.flip);
	}

	// Player
	if (p.iframes > 0)
	{
		if ((iframes_blink = !iframes_blink))
			SDL_RenderCopyEx(g_renderer, tex_egg, p_srect, &p_drect, 0, NULL, p.flip);
	}
	else
		SDL_RenderCopyEx(g_renderer, tex_egg, p_srect, &p_drect, 0, NULL, p.flip);
}

// Handle player keydown events
void ent_player_keydown(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_c:
		ent_new_GROUNDGUY(p.b.x, p.b.y - 80, -5.0f);
		break;
	case SDLK_v:
		ent_new_SLIDEGUY(p.b.x, p.b.y - 80);
		break;
	case SDLK_b:
		ent_new_GROUNDGUY(p.b.x, p.b.y - 80, 0.0f);
		break;
	case SDLK_n:
		ent_new_EVILBALL(p.b.x, p.b.y - 120, (spdl_random() - 128) / 128.0f, (spdl_random() - 128) / 128.0f);
		break;
	case SDLK_m:
		ent_new_DOOR(p.b.x, p.b.y - 64, 0);
		break;
	case SDLK_y:
		ent_new_RAGDOLL(p.b.x, p.b.y - 80, 0, 0, RAGDOLL_EGG);
		break;
	case SDLK_h:
		{
			int num;
			fflush(stdin);
			printf("Hey egg, please input the number of bubbles you want: ");
			scanf("%d", &num);
			printf("Spawning %d bubbles...\n", num);
			REP (num)
				ent_new_PARTICLE(p.b.x, p.b.y, PTCL_BUBBLE);
		}
		break;
	}
}

// Damages the player by power points, gives iframes, and handles death
bool ent_player_damage(int power)
{
	// Don't damage the player when they have brief invincibility
	if (p.iframes > 0)
		return false;
	
	p.hp -= power;
	p.iframes = 60.0;
	REP (3)
		ent_new_PARTICLE(p.b.x + 16, p.b.y + 16, PTCL_BUBBLE);
	snd_play(snd_splode);

	// Checking for player death
	if (p.hp <= 0)
	{
		p.hp = 0;
		ent_new_RAGDOLL(p.b.x, p.b.y - 2, p.b.hsp * -1, -5, RAGDOLL_EGG);
		REP (30)
			ent_new_PARTICLE(p.b.x + 16, p.b.y + 16, PTCL_BUBBLE);
	}
	return true;
}

// Changes the player's sprite to one of 2 running frames, alternating on each call
static void p_anim_run(void)
{
	if ((p.anim_step_frame = p.anim_step_frame ? 0 : 1) == 0)
	{
		p.sprite = SPR_EGG_RUN1;
		p.trumpet_offset.y = 16;
	}
	else
	{
		p.sprite = SPR_EGG_RUN2;
		p.trumpet_offset.y = 14;
	}
}
