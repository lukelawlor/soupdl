/*
 * font.h contains functions for drawing the sprite font, which is a texture stored in tex_font (defined in texture.c)
 */

#ifndef	FONT_H
#define	FONT_H

// Width and height of each character
#define	FONT_CHAR_HEIGHT	14
#define	FONT_CHAR_WIDTH		12

// Number of pixels between characters horizontally and vertically
#define	FONT_CHAR_XSPACE	(FONT_CHAR_WIDTH + 1)
#define	FONT_CHAR_YSPACE	(FONT_CHAR_HEIGHT + 1)

// Draws sprite font text
void font_draw_text(const char *str, int x, int y);

#endif
