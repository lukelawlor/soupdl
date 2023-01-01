/*
 * texture.h contains functions for loading and freeing all game textures
 */

#ifndef	TEXTURE_H
#define	TEXTURE_H

// All game textures
extern SDL_Texture *tex_tileset;
extern SDL_Texture *tex_egg;
extern SDL_Texture *tex_evilegg;
extern SDL_Texture *tex_fireball;
extern SDL_Texture *tex_particle;
extern SDL_Texture *tex_trumpet;
extern SDL_Texture *tex_heart;
extern SDL_Texture *tex_font;
extern SDL_Texture *tex_cloud;
extern SDL_Texture *tex_turret;

// Loads all game textures, returns nonzero on error
int tex_load_all(void);

// Frees all textures
void tex_free_all(void);

#endif
