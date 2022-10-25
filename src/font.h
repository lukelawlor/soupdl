/*
 * font.h contains functions for drawing the sprite font, which is a texture stored in tex_font (defined in texture.c)
 */

#ifndef	FONT_H
#define	FONT_H

// Draws sprite font text
void font_draw_text(const char *str, int x, int y);

#endif
