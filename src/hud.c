/*
 * hud.c contains functions for drawing parts of the game's HUD.
 */

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "error.h"
#include "video.h"
#include "texture.h"
#include "font.h"
#include "entity/player.h"
#include "hud.h"

#define	GAME_NAME_STR	"soupdl06\nby lukelawlor\nbuild " __DATE__
#define	GAME_NAME_WIDTH	(19 * FONT_CHAR_XSPACE)

#define	HEART_START_X			4
#define	HEART_START_Y			4
#define	HEART_SPRITE_WIDTH		16
#define	HEART_SPRITE_HEIGHT		16
#define	HEART_XSPACE			20

#define	FIREBALL_START_X		4
#define	FIREBALL_START_Y		24
#define	FIREBALL_SPRITE_WIDTH	16
#define	FIREBALL_SPRITE_HEIGHT	16
#define	FIREBALL_XSPACE			20

// Draw the game's name 
static void hud_draw_game_name(void);

// Draw the player's health
static void hud_draw_hearts(void);

// Draw the player's fireball count
static void hud_draw_fireballs(void);

// Draws every component of the HUD
void hud_draw_all(void)
{
	hud_draw_hearts();
	hud_draw_fireballs();
	hud_draw_game_name();
}

// Draw the game's name 
static void hud_draw_game_name(void)
{
	font_draw_text(
		GAME_NAME_STR,
		g_screen_width - GAME_NAME_WIDTH - 6,
		0
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
		const int fireballs_used = g_player.trumpet_shots_reset - g_player.trumpet_shots;
		SDL_Rect srect = {
			0,
			0,
			FIREBALL_SPRITE_WIDTH,
			FIREBALL_SPRITE_HEIGHT,
		};
		SDL_Rect drect = {
			FIREBALL_START_X,
			FIREBALL_START_Y,
			FIREBALL_SPRITE_WIDTH,
			FIREBALL_SPRITE_HEIGHT,
		};
		for (int i = 0; i < g_player.trumpet_shots; i++)
		{
			SDL_RenderCopy(g_renderer, tex_fireball, &srect, &drect);
			drect.x += FIREBALL_XSPACE;
		}
		srect.x = FIREBALL_SPRITE_WIDTH * 2;
		for (int i = 0; i < fireballs_used; i++)
		{
			SDL_RenderCopy(g_renderer, tex_fireball, &srect, &drect);
			drect.x += FIREBALL_XSPACE;
		}
}
