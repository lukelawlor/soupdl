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

#define	GAME_NAME_STR	"SoupDL 06 alpha v0.00001 (c) Luke Lawlor 2022"
#define	GAME_NAME_LEN	45

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

#include "entity/root.h"
#include "entity/id.h"

// Draw the game's name 
static void hud_draw_game_name(void)
{
	// Parenthesis needed for proper expansion of macro
	const int x = g_screen_width - (FONT_CHAR_XSPACE) * GAME_NAME_LEN - 2;
	const int y = g_screen_height - FONT_CHAR_YSPACE - 2;
	font_draw_text(GAME_NAME_STR, x, y);

	// Show # of guys remaining
	{
		#define	SIZ	30
		char buf[SIZ];
		snprintf(buf,SIZ,"Guys left: %d", g_er[ENT_ID_GROUNDGUY]->len + g_er[ENT_ID_SLIDEGUY]->len);
		font_draw_text(buf, 20, 20);
	}
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
