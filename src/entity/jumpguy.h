/*
 * jumpguy.h contains the type for jumpguy entities.
 *
 * This file is automatically included when you #include all.h
 */

#ifndef	ENTITY_JUMPGUY_H
#define	ENTITY_JUMPGUY_H

#include "entity.h"

// Entity item type
typedef struct{
	EcmBase base;
	EcmBody b;
	EcmEvileggSpr s;
	float jsp;
} EntJUMPGUY;

EntJUMPGUY *ent_new_JUMPGUY(int x, int y);
void ent_update_JUMPGUY(EntJUMPGUY *e);
void ent_destroy_JUMPGUY(EntJUMPGUY *e);

/*
void ent_draw_JUMPGUY(EntJUMPGUY *e);
*/
#define	ent_draw_JUMPGUY(e)	ent_draw_GROUNDGUY((EntGROUNDGUY *) e)

#endif

/////////////////////////////////////////////////////////////// additions to make in other files
//
// id.h		add ENT_ID_JUMPGUY to EntId
// metadata.h 	add metadata
// all.h	add #include "jumpguy.h"
// root.c	add EAN(JUMPGUY, 100);
// ../main.c	add ENT_UPDATE(JUMPGUY); and ENT_DRAW(JUMPGUY);
