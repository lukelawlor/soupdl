/*
 * texture.c contains functions for loading and freeing all game textures.
 */

#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "dir.h"
#include "error.h"
#include "video.h"
#include "texture.h"

// Global variables for all game textures
#define	USE_RES(name)	SDL_Texture *tex_##name
	USE_RES(tileset);
	USE_RES(egg);
	USE_RES(evilegg);
	USE_RES(coolegg);
	USE_RES(fireball);
	USE_RES(particle);
	USE_RES(trumpet);
	USE_RES(heart);
	USE_RES(font);
	USE_RES(cloud);
	USE_RES(turret);
	USE_RES(cakico);
	USE_RES(barrier);
#undef USE_RES

// Loads texture from path, returns pointer to that texture or null on error
static SDL_Texture *tex_load_file(char *path);

// This function loads a texture into tex from path.
// path represents a file in the res directory of the game.
// It prints errors and returns NULL on error.
// If successful, the pointer to the texture is returned
static SDL_Texture *tex_load_file(char *path)
{
	// Temp surface used to load image
	SDL_Surface *surf;

	SDL_Texture *tex;

	// Convert path from a filename in resource directory to the full path to that file
	char full_path[RES_PATH_MAX];
	snprintf(full_path, RES_PATH_MAX, DIR_GFX "/%s", path);

	// Load image, color key it, and create a texture from it
	if ((surf = IMG_Load(full_path)) == NULL)
	{
		PERR("failed to load image \"%s\". SDL Error: %s", full_path, IMG_GetError());
		return NULL;
	}
	SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 0, 0, 0));
	if ((tex = SDL_CreateTextureFromSurface(g_renderer, surf)) == NULL)
	{
		PERR("failed to create texture for image \"%s\". SDL_Error: %s", full_path, SDL_GetError());
		SDL_FreeSurface(surf);
		return NULL;
	}
	SDL_FreeSurface(surf);

	return tex;
}

// This functions loads all of the textures in the game.
// It returns nonzero on error.
int tex_load_all(void)
{
// Tries to load a texture
#define	USE_RES(name)	if ((tex_##name = tex_load_file(#name ".png")) == NULL) \
					goto l_error
	USE_RES(tileset);
	USE_RES(egg);
	USE_RES(evilegg);
	USE_RES(coolegg);
	USE_RES(fireball);
	USE_RES(particle);
	USE_RES(trumpet);
	USE_RES(heart);
	USE_RES(font);
	USE_RES(cloud);
	USE_RES(turret);
	USE_RES(cakico);
	USE_RES(barrier);
#undef USE_RES
	if (SDL_SetTextureColorMod(tex_font, 0xff, 0x00, 0x00) == -1)
		PERR("texture color mod for font unavailable");
	if (SDL_SetTextureColorMod(tex_barrier, 0xb6, 0x0f, 0xff) == -1)
		PERR("texture color mod for barrier unavailable");
	return 0;
l_error:
	tex_free_all();
	return 1;
}

// Frees all textures
void tex_free_all(void)
{
#define	USE_RES(name)	SDL_DestroyTexture(tex_##name)
	USE_RES(tileset);
	USE_RES(egg);
	USE_RES(evilegg);
	USE_RES(coolegg);
	USE_RES(fireball);
	USE_RES(particle);
	USE_RES(trumpet);
	USE_RES(heart);
	USE_RES(font);
	USE_RES(cloud);
	USE_RES(turret);
	USE_RES(cakico);
	USE_RES(barrier);
#undef USE_RES
}
