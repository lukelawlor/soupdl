/*
 * player.h contains the player type.
 */

#ifndef	ENTITY_PLAYER_H
#define	ENTITY_PLAYER_H

// Player animation states
typedef enum{
	P_ANIM_IDLE,
	P_ANIM_RUN,
	P_ANIM_AIR
} p_anim;

#include <SDL2/SDL.h>
typedef struct{
	// Position
 	double x, y;

	// Hitbox rectangle (The x & y of this rectangle are relative to the actual position of the player)
	SDL_Rect hrect;

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

	// Used to set flip the player horizontally
	SDL_RendererFlip flip;

	// Source and destination rectangles used for rendering
	SDL_Rect srect, drect;

	// Animation state
	p_anim anim;

	// Frame of animation
	int anim_frame;

	// Timer used to determine the length of frames
	int anim_tmr;
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
