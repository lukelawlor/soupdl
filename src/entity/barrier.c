
///////////////////////////////////////////////////////////////// barrier.c

/*
 * barrier.c contains functions for manipulating template entities.
 */

#include "../camera.h"
#include "../error.h"
#include "../texture.h"
#include "../tile/data.h"	// For g_tile_map
#include "../video.h"

#include "entity.h"

#include "barrier.h"

EntBARRIER *ent_new_BARRIER(int x, int y, BarrierTag btag)
{
	ENT_NEW(BARRIER);
	e->x = x;
	e->y = y;
	e->btag = btag;
	g_tile_map[e->y / 32][e->x / 32] = TILE_LIME;
	PINF("barrier with btag %d created", (int) btag);
	return e;
}

void ent_update_BARRIER(EntBARRIER *e)
{

}

void ent_draw_BARRIER(EntBARRIER *e)
{
	
}

void ent_destroy_BARRIER(EntBARRIER *e)
{
	g_tile_map[e->y / 32][e->x / 32] = TILE_AIR;
	ENT_DEL_MARK(e);
}

/////////////////////////////////////////////////////////////// additions to make in other files
//
// id.h		add ENT_ID_BARRIER to EntId
// all.h	add #include "barrier.h"
// root.c	add EAN(BARRIER, 100);
// ../main.c	add ENT_UPDATE(BARRIER); and ENT_DRAW(BARRIER);
//
// OPTIONAL:
// 
// tile.h	add ENT_TILE_BARRIER to EntTileId
// tile.c	add entity tile definition in ent_tile_init()
// all.c	add ent_array_reset(g_er[ENT_ID_BARRIER]); in ent_destroy_temp() if the entity doesn't persistent between map changes
