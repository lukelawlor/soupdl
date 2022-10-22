/*
 * hud.c contains functions for drawing parts of the game's HUD.
 */

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "error.h"
#include "video.h"
#include "texture.h"
#include "entity/player.h"
#include "hud.h"

// Texture pointer to the game's name
static SDL_Texture *g_text_name;

// Width and height of the game name texture
static int g_text_name_w, g_text_name_h;

// Draw the game's name 
static void hud_draw_game_name(void);

// Draw the player's health
static void hud_draw_health(void);

// Initializes variables needed to draw parts of the HUD, returns nonzero on error
int hud_init(void)
{
	// Initializing game name text
	SDL_Surface *surf;
	
	if ((surf = TTF_RenderText_Solid(g_font, "SoupDL 06 pre pre alpha v0.0000000001 (c) Luke Lawlor 2022", g_font_color)) == NULL)
	{
		PERR();
		fprintf(stderr, "failed to render game name text\n");
		return 1;
	}
	if ((g_text_name = SDL_CreateTextureFromSurface(g_renderer, surf)) == NULL)
	{
		SDL_FreeSurface(surf);
		PERR();
		fprintf(stderr, "failed to create texture from game name text surface. SDL Error: %s\n", SDL_GetError());
		return 1;
	}

	// Setting text dimensions
	g_text_name_w = surf->w;
	g_text_name_h = surf->h;

	SDL_FreeSurface(surf);
	return 0;
}

// Draws every component of the HUD
void hud_draw_all(void)
{
	hud_draw_health();
	hud_draw_game_name();
}

// Draw the game's name 
static void hud_draw_game_name(void)
{
	SDL_Rect drect = {g_screen_width - g_text_name_w, g_screen_height - g_text_name_h, g_text_name_w, g_text_name_h};
	SDL_RenderCopy(g_renderer, g_text_name, NULL, &drect);
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
