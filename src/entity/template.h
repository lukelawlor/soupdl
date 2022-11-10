//
// This is a blueprint for creating new entities in SoupDL 06
//
///////////////////////////////////////////////////////////////// template.h

/*
 * template.h contains the type for template entities.
 *
 * This file is automatically included when you #include all.h
 */

#ifndef	ENTITY_TEMPLATE_H
#define	ENTITY_TEMPLATE_H

#include "entity.h"

// Entity item type
typedef struct{
	EcmBase base;
} EntTEMPLATE;

EntTEMPLATE *ent_new_TEMPLATE(void);
void ent_update_TEMPLATE(EntTEMPLATE *e);
void ent_draw_TEMPLATE(EntTEMPLATE *e);
void ent_destroy_TEMPLATE(EntTEMPLATE *e);

#endif

///////////////////////////////////////////////////////////////// template.c

/*
 * template.c contains functions for manipulating template entities.
 */

#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "entity.h"
#include "template.h"

EntTEMPLATE *ent_new_TEMPLATE(void)
{
	ENT_NEW(TEMPLATE);
	return e;
}

void ent_update_TEMPLATE(EntTEMPLATE *e)
{

}

void ent_draw_TEMPLATE(EntTEMPLATE *e)
{
	
}

void ent_destroy_TEMPLATE(EntTEMPLATE *e)
{
	ENT_DEL_MARK(e);
}

/////////////////////////////////////////////////////////////// additions to make in other files
//
// id.h		add 1 to ENT_MAX and ENT_ID_TEMPLATE to EntId
// metadata.h 	add metadata
// all.h	add #include "template.h"
// root.c	add EAN(TEMPLATE, 100);
// ../main.c	add ENT_UPDATE(TEMPLATE); and ENT_DRAW(TEMPLATE);