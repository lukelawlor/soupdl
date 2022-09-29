/*
 * player.h contains the player type.
 */

#ifndef	ENTITY_PLAYER_H
#define	ENTITY_PLAYER_H

typedef struct{
	int x;
	int y;
} Point;

// Player animation states
typedef enum{
	P_ANIM_IDLE,
	P_ANIM_RUN,
	P_ANIM_AIR
} p_anim;

#include <SDL2/SDL.h>
typedef struct{
	// True if the player has a trumpet
	bool has_trumpet;

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

	// Source and destination rectangles used for rendering the player
	SDL_Rect srect, drect;

	// Source and destination rectangles used for rendering the trumpet
	SDL_Rect trumpet_srect, trumpet_drect;

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
