/*
 * player.h contains the player type.
 */

#ifndef	ENTITY_PLAYER_H
#define	ENTITY_PLAYER_H

typedef struct{
	int x;
	int y;
} Point;

// Player sprites
typedef enum{
	P_SPR_IDLE,
	P_SPR_RUN1,
	P_SPR_RUN2,
	P_SPR_SHOOT
} p_sprite;

#include <SDL2/SDL.h>
typedef struct{
	// True if the player has a trumpet
	bool has_trumpet;

	// Health (if <= 0, the player is considered dead)
	int hp;
	
	// Max health
	int maxhp;

	// Position
 	double x, y;

	// Hitbox rectangle (The x and y of this rectangle are relative to the actual position of the player)
	SDL_Rect hrect;

	// Similarly to the hitbox rectangle, the x and y values of this point are used to position the trumpet relative to the player's position
	Point trumpet_offset;

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

	// Used to set flip the player (and trumpet) horizontally
	SDL_RendererFlip flip;

	// Player sprite to draw
	p_sprite sprite;

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
