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

// Draw the game's name 
static void hud_draw_game_name(void);

// Draw the player's health
static void hud_draw_health(void);

// Draws every component of the HUD
void hud_draw_all(void)
{
	hud_draw_health();
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
static void hud_draw_health(void)
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

	SDL_Rect srect = {0, 0, 16, 16};
	SDL_Rect drect = {4, 4, 16, 16};

	// Draw full hearts
	for (int i = 0; i < full_hearts; i++)
	{
		
		SDL_RenderCopy(g_renderer, tex_heart, &srect, &drect);
		drect.x += 20;
	}

	// Draw half heart
	if (half_heart)
	{
		srect.x = 16;
		SDL_RenderCopy(g_renderer, tex_heart, &srect, &drect);
		drect.x += 20;
	}

	// Draw empty hearts
	srect.x = 32;
	for (int i = 0; i < empty_hearts; i++)
	{
		SDL_RenderCopy(g_renderer, tex_heart, &srect, &drect);
		drect.x += 20;
	}
}
