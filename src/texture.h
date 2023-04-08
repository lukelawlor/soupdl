/*
 * texture.h contains functions for loading and freeing all game textures
 */

#ifndef	TEXTURE_H
#define	TEXTURE_H

#include <SDL2/SDL.h>	// For SDL_Texture

// Extern declarations for all game textures
#define	USE_RES(name)	extern SDL_Texture *tex_##name
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

// Loads all game textures, returns nonzero on error
int tex_load_all(void);

// Frees all textures
void tex_free_all(void);

#endif
