/*
 * player.h contains the player type.
 */

#ifndef	ENTITY_PLAYER_H
#define	ENTITY_PLAYER_H

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "c_body.h"
#include "c_sprite.h"

typedef struct{
	// Body
	EcmBody b;

	// Horizontal acceleration and decleration
	double acc, dec;

	// vsp value when the player jumps (jump speed) (also the min vsp value)
	double jsp;

	// Max vsp when falling (not rising)
	double maxvsp;

	// Maximum horizontal speed
	double maxhsp;

	// Jump timer (# of frames allowed for the player to jump after walking off of a cliff)
	double jtmr;

	// Health (if <= 0, the player is considered dead)
	int hp;
	
	// Max health
	int maxhp;

	// True if the player is on the ground
	bool on_ground : 1;

	// True if the player has a trumpet
	bool has_trumpet : 1;

	// Number of shots the player has left
	short trumpet_shots;
	short trumpet_shots_reset;

	// iframes = invincibility frames, the relative (since timestep is used) number of frames of invincibility the player has after being damaged
	double iframes;

	// Relative # of frames to wait between trumpet shots
	double shoot_cooldown;

	// Egg sprite to draw
	SprEgg sprite;

	// Similarly to the hitbox rectangle, the x and y values of this point are used to position the trumpet relative to the player's position
	SDL_Point trumpet_offset;

	// Used to set flip the player (and trumpet) horizontally
	SDL_RendererFlip flip;

	// Step animation frame
	short anim_step_frame;

	// Step animation tmr (relative # of frames to show each step animation frame for)
	short anim_step_tmr;

	// Shoot animation tmr (# of frames to show the shooting sprite for)
	short anim_shoot_tmr;

	// # of frames to show all fireballs in the HUD blink for 
	// The code to draw this is in ../hud.c
	short anim_fireblink_tmr;

	// True if the player isn't allowed to open doors
	// This is set to true when a new map is loaded to stop the player from entering the door it just entered
	// When the player loses contact with the door, this is set to false
	bool door_stop : 1;

	// Player collision rectangle
	// This is updated when the player finishes moving in ent_player_update()
	SDL_Rect crect;

	// Number of coins the player has
	int coins;
} EntPlayer;

// Global player
extern EntPlayer g_player;

// Update the player's variables
void ent_player_update(void);

// Render the player
void ent_player_draw(void);

// Handle player keydown events
void ent_player_keydown(SDL_Keycode key);

// Damages the player by power points, gives iframes, and handles death; returns false when the player has iframes (meaning no damage was done)
bool ent_player_damage(int power);

#endif
