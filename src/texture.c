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

// Tries to load a texture in tex_load_all
#define	TEX_LOAD(name)	if ((tex_##name = tex_load_file(#name ".png")) == NULL) \
				goto l_error;

// All game textures
SDL_Texture *tex_tileset;
SDL_Texture *tex_egg;
SDL_Texture *tex_evilegg;
SDL_Texture *tex_fireball;
SDL_Texture *tex_particle;
SDL_Texture *tex_trumpet;
SDL_Texture *tex_heart;
SDL_Texture *tex_font;
SDL_Texture *tex_cloud;
SDL_Texture *tex_turret;

// Loads texture from path, returns pointer to that texture or null on error
static SDL_Texture *tex_load_file(char *path);

/*
 * This function loads a texture into tex from path.
 *
 * path represents a file in the res directory of the game.
 *
 * It prints errors and returns NULL on error.
 *
 * If successful, the pointer to the texture is returned
 */
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

/*
 * This functions loads all of the textures in the game.
 *
 * It returns nonzero on error.
 */
int tex_load_all(void)
{
	TEX_LOAD(tileset);
	TEX_LOAD(egg);
	TEX_LOAD(evilegg);
	TEX_LOAD(fireball);
	TEX_LOAD(particle);
	TEX_LOAD(trumpet);
	TEX_LOAD(heart);
	TEX_LOAD(font);
	TEX_LOAD(cloud);
	TEX_LOAD(turret);
	if (SDL_SetTextureColorMod(tex_font, 255, 0, 0) == -1)
	{
		PERR("texture color mod for font unavailable");
	}
	return 0;
l_error:
	tex_free_all();
	return 1;
}

/*
 * Frees all textures
 */
void tex_free_all(void)
{
	SDL_DestroyTexture(tex_tileset);
	SDL_DestroyTexture(tex_egg);
	SDL_DestroyTexture(tex_evilegg);
	SDL_DestroyTexture(tex_fireball);
	SDL_DestroyTexture(tex_particle);
	SDL_DestroyTexture(tex_trumpet);
	SDL_DestroyTexture(tex_heart);
	SDL_DestroyTexture(tex_font);
	SDL_DestroyTexture(tex_cloud);
	SDL_DestroyTexture(tex_turret);
}
