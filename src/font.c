/*
 * font.c contains functions for drawing the sprite font, which is a texture stored in tex_font (defined in texture.c)
 */

#include <SDL2/SDL.h>

#include "video.h"
#include "texture.h"
#include "font.h"

// Number of rows and columns of character sprites in the font spritesheet
#define	FONT_SPR_ROWS		16
#define	FONT_SPR_COLS		8

// Draws sprite font text
void font_draw_text(const char *str, int x, int y)
{
	SDL_Rect drect = {x, y, FONT_CHAR_WIDTH, FONT_CHAR_HEIGHT};
	SDL_Rect srect = {.w = FONT_CHAR_WIDTH, .h = FONT_CHAR_HEIGHT};

	// Current character being read
	int c;

	for (int i = 0; (c = str[i]) != '\0'; i++)
	{
		switch (c)
		{
		case ' ':
			drect.x += FONT_CHAR_XSPACE;
			continue;
		case '\n':
			drect.x = x;
			drect.y += FONT_CHAR_YSPACE;
			continue;
		}
		srect.x = (c % FONT_SPR_ROWS) * FONT_CHAR_WIDTH;
		srect.y = (c / FONT_SPR_ROWS) * FONT_CHAR_HEIGHT;
		SDL_RenderCopy(g_renderer, tex_font, &srect, &drect);
		drect.x += FONT_CHAR_XSPACE;
	}
}
