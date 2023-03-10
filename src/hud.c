/*
 * hud.c contains functions for drawing parts of the game's HUD.
 */

#include <stdbool.h>
#include <stdlib.h>		// For abs()

#include <SDL2/SDL.h>

#include "error.h"
#include "video.h"
#include "texture.h"
#include "font.h"
#include "entity/player.h"
#include "hud.h"

#define	COINS_STR_LEN	10
#define	GAME_NAME_STR	"soupdl06\nby lukelawlor\nbuild " __DATE__
#define	GAME_NAME_WIDTH	(19 * FONT_CHAR_XSPACE)

#define	HEART_START_X			4
#define	HEART_START_Y			(FONT_CHAR_YSPACE + 8)
#define	HEART_SPRITE_WIDTH		16
#define	HEART_SPRITE_HEIGHT		16
#define	HEART_XSPACE			20

#define	FIREBALL_START_X		HEART_START_X
#define	FIREBALL_START_Y		(HEART_START_Y + HEART_SPRITE_HEIGHT + 4)
#define	FIREBALL_SPRITE_WIDTH	16
#define	FIREBALL_SPRITE_HEIGHT	16
#define	FIREBALL_XSPACE			20

// # of frames to blink a fireball for
#define	BLINK_TICK_INC			4

// # of BLINK_TICK_INCs to go by before the blink tick is reset to 0
#define	BLINK_TICK_RESET_WAIT	4

// The y value added to fireballs that blink normally
// Causes a bobbing appearance
#define	BLINK_BOB_Y				2

// Draw the # of coins collected & the game name
static void hud_draw_text(void);

// Draw the player's health
static void hud_draw_hearts(void);

// Draw the player's fireball count
static void hud_draw_fireballs(void);

// Draws every component of the HUD
void hud_draw_all(void)
{
	hud_draw_hearts();
	hud_draw_fireballs();
	hud_draw_text();
}

// Draw the # of coins collected & the game name
static void hud_draw_text(void)
{
	// Game name
	font_draw_text(
		GAME_NAME_STR,
		g_screen_width - GAME_NAME_WIDTH - 6,
		0
	);

	// # of coins collected
	char coins_str[COINS_STR_LEN];
	snprintf(coins_str, COINS_STR_LEN, "coins: %d", g_player.coins);
	font_draw_text(
		coins_str,
		4,
		4
	);
}

// Draw the player's health
static void hud_draw_hearts(void)
{
	// The number of full hearts to draw
	int full_hearts = g_player.hp / 2;

	// The number of empty hearts to draw
	int empty_hearts = g_player.maxhp / 2 - full_hearts;

	// True if a half heart will be drawn
	bool half_heart;

	if (g_player.hp % 2 == 0)
		half_heart = false;
	else
	{
		half_heart = true;

		// If a half heart is being drawn the # of empty hearts calculated before will be one off, so we decrement it
		empty_hearts--;
	}

	SDL_Rect srect = {
			0,
			0,
			HEART_SPRITE_WIDTH,
			HEART_SPRITE_HEIGHT,
	};
	SDL_Rect drect = {
			HEART_START_X,
			HEART_START_Y,
			HEART_SPRITE_WIDTH,
			HEART_SPRITE_HEIGHT,
	};

	// Draw full hearts
	for (int i = 0; i < full_hearts; i++)
	{
		
		SDL_RenderCopy(g_renderer, tex_heart, &srect, &drect);
		drect.x += HEART_XSPACE;
	}

	// Draw half heart
	if (half_heart)
	{
		srect.x = HEART_SPRITE_WIDTH;
		SDL_RenderCopy(g_renderer, tex_heart, &srect, &drect);
		drect.x += HEART_XSPACE;
	}

	// Draw empty hearts
	srect.x = HEART_SPRITE_WIDTH * 2;
	for (int i = 0; i < empty_hearts; i++)
	{
		SDL_RenderCopy(g_renderer, tex_heart, &srect, &drect);
		drect.x += HEART_XSPACE;
	}
}

// Draw the player's fireball count
static void hud_draw_fireballs(void)
{
		// Blink tick
		// This controls the fireball blinking animation of the fireball count
		// When a fireball "blinks" its bright sprite is shown
		static int blink_tick = 0;
		if (++blink_tick > (g_player.trumpet_shots_reset + BLINK_TICK_RESET_WAIT) * BLINK_TICK_INC)
			blink_tick = 0;
		
		// Source rectangle of the fireball texture
		SDL_Rect srect = {
			.y = 0,
			.w = FIREBALL_SPRITE_WIDTH,
			.h = FIREBALL_SPRITE_HEIGHT,
		};

		// Destination rectangle to draw the fireball with
		SDL_Rect drect = {
			.x = FIREBALL_START_X,
			.w = FIREBALL_SPRITE_WIDTH,
			.h = FIREBALL_SPRITE_HEIGHT,
		};

		// When i in the following loop reaches this value, draw a blinking & bobbing fireball
		int i_bob = blink_tick / BLINK_TICK_INC;

		// When i in the following loop reaches this value, draw a blinking fireball
		int i_light = i_bob - 1;

		for (int i = 0; i < g_player.trumpet_shots; i++)
		{
			if (i == i_bob)
			{
				// Draw a blinking and bobbing fireball
				drect.y = FIREBALL_START_Y + BLINK_BOB_Y;
				srect.x = FIREBALL_SPRITE_WIDTH;
			}
			else if (i == i_light)
			{
				// Draw a blinking fireball
				drect.y = FIREBALL_START_Y;
				srect.x = FIREBALL_SPRITE_WIDTH;
			}
			else if (g_player.anim_fireblink_tmr > 0)
			{
				// Draw all fireballs bobbing when the player's fireblink timer is on
				drect.y = FIREBALL_START_Y + g_player.anim_fireblink_tmr;
				srect.x = FIREBALL_SPRITE_WIDTH;
			}
			else
			{
				// Draw a fireball normally
				drect.y = FIREBALL_START_Y;
				srect.x = 0;
			}
			SDL_RenderCopy(g_renderer, tex_fireball, &srect, &drect);
			drect.x += FIREBALL_XSPACE;
		}

		// Draw used fireballs
		const int fireballs_used = g_player.trumpet_shots_reset - g_player.trumpet_shots;
		drect.y = FIREBALL_START_Y;
		srect.x = FIREBALL_SPRITE_WIDTH * 2;
		for (int i = 0; i < fireballs_used; i++)
		{
			SDL_RenderCopy(g_renderer, tex_fireball, &srect, &drect);
			drect.x += FIREBALL_XSPACE;
		}
}
