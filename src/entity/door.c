/*
 * door.c contains functions for manipulating door entities.
 */

#include <assert.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "../camera.h"
#include "../collision.h"
#include "../map.h"
#include "../random.h"
#include "../texture.h"
#include "../tile/data.h"	// For TILE_SIZE
#include "../video.h"

#include "c_body.h"
#include "entity.h"

#include "player.h"
#include "door.h"

static const SDL_Rect g_srect = {TILE_SIZE*2, TILE_SIZE*1, TILE_SIZE, TILE_SIZE};

EntDoorId g_ent_door_last_used = 0;
char g_ent_door_map_path[ENT_DOOR_MAX][ENT_DOOR_MAP_PATH_MAX] = {
	"intro.map",
	"ed.map",
	"big.map",
	"new.map",
};

EntDOOR *ent_new_DOOR(int x, int y, EntDoorId did)
{
	assert(ENT_DOOR_ID_IS_VALID(did));
	ENT_NEW(DOOR);
	e->b = (EcmBody) {x, y, TILE_SIZE, TILE_SIZE, 0, 0, 0};
	e->did = did;
	return e;
}

void ent_update_DOOR(EntDOOR *e)
{
	// Does nothing for now
	const SDL_Rect crect = ECM_BODY_GET_CRECT(e->b);
	const SDL_Rect prect = ECM_BODY_GET_CRECT(g_player.b);
	if (check_rect(&crect, &prect))
	{
		// Player entered door
		map_load_txt(g_ent_door_map_path[e->did], false);
	}
}

void ent_draw_DOOR(EntDOOR *e)
{
	const SDL_Rect drect = {e->b.x + g_cam.xshift, e->b.y + g_cam.yshift, TILE_SIZE, TILE_SIZE};
	SDL_RenderCopy(g_renderer, tex_tileset, &g_srect, &drect);
}

void ent_destroy_DOOR(EntDOOR *e)
{
	ENT_DEL_MARK(e);
}
