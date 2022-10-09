/*
 * texture.c contains functions for loading and freeing all game textures.
 */

#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "dir.h"
#include "video.h"
#include "texture.h"

// All game textures
SDL_Texture *tex_tileset = NULL;
SDL_Texture *tex_egg = NULL;
SDL_Texture *tex_evilegg = NULL;
SDL_Texture *tex_fireball = NULL;
SDL_Texture *tex_particle = NULL;
SDL_Texture *tex_trumpet = NULL;
SDL_Texture *tex_heart = NULL;

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
	char full_path[MAX_IMG_PATH_LEN];
	sprintf(full_path, WORKING_DIR "res/%s", path);

	// Load image, color key it, and create a texture from it
	if ((surf = IMG_Load(full_path)) == NULL)
	{
		
		fprintf(stderr, "Failed to load image \"%s\" SDL Error: %s\n", full_path, IMG_GetError());
		return NULL;
	}
	SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 0, 0, 0));
	if ((tex = SDL_CreateTextureFromSurface(g_renderer, surf)) == NULL)
	{
		fprintf(stderr, "Failed to create texture for image \"%s\" SDL_Error: %s\n", full_path, SDL_GetError());
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
	if ((tex_tileset = tex_load_file("tileset.png")) == NULL)
		goto l_error;
	if ((tex_egg = tex_load_file("egg.png")) == NULL)
		goto l_error;
	if ((tex_evilegg = tex_load_file("evilegg.png")) == NULL)
		goto l_error;
	if ((tex_fireball = tex_load_file("fireball.png")) == NULL)
		goto l_error;
	if ((tex_particle = tex_load_file("particle.png")) == NULL)
		goto l_error;
	if ((tex_trumpet = tex_load_file("trumpet.png")) == NULL)
		goto l_error;
	if ((tex_heart = tex_load_file("heart.png")) == NULL)
		goto l_error;
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
}
