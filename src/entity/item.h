/*
 * item.h contains the type for item entities.
 *
 * This file is automatically included when you #include all.h
 */

#ifndef	ENTITY_ITEM_H
#define	ENTITY_ITEM_H

#include <SDL2/SDL.h>

#include "entity.h"

// The total number of item types in the game
#define	ENT_ITEM_MAX	2

// Item texture type
typedef struct{
	SDL_Texture *tex;
	SDL_Rect *srect;
	int w;
	int h;
} EntItemTex;

// Item id type
typedef enum{
	ITEM_TRUMPET,
	ITEM_CATFACE
} EntItemId;

// Entity item type
typedef struct{
	EcmDefaults d;
	int x;
	int y;
	EntItemId id;
} EntItem;

// Constant pointer to the first index of the item entity array
extern EntItem *const ent_item;

// Initializes the ent_item_tex array (defined in item.c)
void ent_item_init(void);

EntItem *ent_item_new(int x, int y, EntItemId id);
void ent_item_draw(EntItem *e);
void ent_item_destroy(EntItem *e);

#endif
