/*
 * barrier.c contains functions for manipulating template entities.
 */

#include "../camera.h"
#include "../error.h"
#include "../texture.h"
#include "../tile/data.h"	// For g_tile_map
#include "../util/rep.h"
#include "../video.h"

#include "entity.h"

#include "barrier.h"
#include "particle.h"

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
	REP (10)
		ent_new_PARTICLE(e->x + TILE_SIZE / 2, e->y + TILE_SIZE / 2, PTCL_FLAME);
	g_tile_map[e->y / 32][e->x / 32] = TILE_AIR; ENT_DEL_MARK(e);
}
