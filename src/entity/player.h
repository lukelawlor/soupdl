/*
 * player.h contains the player type.
 */

#ifndef	ENTITY_PLAYER_H
#define	ENTITY_PLAYER_H

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "entity.h"

typedef struct{
	// Position
 	double x, y;

	// Horizontal speed
	double hsp;

	// Horizontal acceleration and decleration
	double acc, dec;

	// Maximum horizontal speed
	double maxsp;

	// Vertical speed
	double vsp;

	// Vertical acceleration (gravity)
	double grv;

	// vsp value when the player jumps (jump speed)
	double jsp;

	// Jump timer (# of frames allowed for the player to jump after walking off of a cliff)
	double jtmr;

	// Health (if <= 0, the player is considered dead)
	int hp;
	
	// Max health
	int maxhp;

	// True if the player is on the ground
	bool on_ground;

	// True if the player has a trumpet
	bool has_trumpet;

	// Hitbox rectangle (The x and y of this rectangle are relative to the actual position of the player)
	SDL_Rect hrect;

	// iframes = invincibility frames, the relative (since timestep is used) number of frames of invincibility the player has after being damaged
	double iframes;

	// True when iframes > 0
	bool iframes_active;

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

} EntPlayer;

// Global player
extern EntPlayer g_player;

// Update the player's variables
void ent_player_update(void);

// Render the player
void ent_player_draw(void);

// Handle player keydown events
void ent_player_keydown(SDL_Keycode key);

#endif
