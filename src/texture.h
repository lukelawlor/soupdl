/*
 * texture.h contains functions for loading and freeing all game textures
 */

#ifndef	TEXTURE_H
#define	TEXTURE_H

// Max characters the path to an image file can be
#define	MAX_IMG_PATH_LEN	100

// All game textures
extern SDL_Texture *tex_egg;
extern SDL_Texture *tex_trumpet;
extern SDL_Texture *tex_stone;
extern SDL_Texture *tex_fireball1;
extern SDL_Texture *tex_fireball2;
extern SDL_Texture *tex_lime;

// Loads all game textures, returns nonzero on error
int tex_load_all(void);

// Frees all textures
void tex_free_all(void);

#endif
