/*
 * item.h contains the type for item entities.
 *
 * Items are stationary objects that trigger an event when a player collides with them.
 *
 * Some example items are:
 * 	Trumpets
 * 	Coins
 *
 * This file is automatically included when you #include all.h
 */

#ifndef	ENTITY_ITEM_H
#define	ENTITY_ITEM_H

#include <SDL2/SDL.h>

#include "entity.h"

// The total number of item types in the game
#define	ENT_ITEM_MAX	2

// Item id type
typedef enum{
	ITEM_TRUMPET,
	ITEM_CATFACE,
	ITEM_COIN,

	// The total number of item types in the game
	ITEM_MAX,
} EntItemId;

// Item texture type
typedef struct{
	SDL_Texture *tex;
	SDL_Rect *srect;
	int w;
	int h;
} EntItemTex;

// Entity item type
typedef struct{
	EcmBase base;
	int x;
	int y;
	EntItemId id;
} EntITEM;

// Initializes the ent_item_tex array (defined in item.c)
void ent_item_init(void);

EntITEM *ent_new_ITEM(int x, int y, EntItemId id);
void ent_draw_ITEM(EntITEM *e);
void ent_destroy_ITEM(EntITEM *e);

#endif
