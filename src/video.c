/*
 * video.c contains definitions for the game's window and renderer.
 */

#include <SDL2/SDL.h>

#include "video.h"

// Game window and renderer
SDL_Window *g_window;
SDL_Renderer *g_renderer;

// Game fonts
TTF_Font *g_font;
SDL_Color g_font_color = {255, 0, 0, 255};

// Screen dimensions
int g_screen_width;
int g_screen_height;
