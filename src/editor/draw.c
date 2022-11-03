/*
 * draw.c contains functions for drawing parts of the editor UI.
 */

#include <stdbool.h>

#include <SDL2/SDL.h>		// for SDL_Texture

#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "../font.h"
#include "../tile/data.h"	// for TILE_SIZE
#include "../entity/all.h"	// for ENT_MAX and EntId
#include "editor.h"
#include "draw.h"

#define	EDSTAT_VERSION		"v0.0.0"
#define	EDSTAT_STRING		"SoupDL 06 Map Editor %s\n\n" \
				"Tile:     %s/%s\n" \
				"TileSize: %dx%d\n" \
				"Map:      %s\n" \
				"MapSize:  %dx%d"
#define	EDSTAT_STRING_LEN_MAX	128

// Names of all tiles and entities as shown in the editor interface, indexed by TileId or EntId
const char *maped_tile_name[] = {
	// TILE_AIR
	"Air",

	// TILE_STONE
	"Stone",

	// TILE_LIME
	"Lime..?",

	// TILE_IRON
	"Iron",

	// TILE_SPIKE
	"Spike",

	// TILE_STRING
	"String",
};
const char *maped_ent_name[] = {
	// ENT_ID_PLAYER
	"Player Spawn Point",

	// ENT_ID_ITEM
	"Item",

	// ENT_ID_FIREBALL
	"Fireball",

	// ENT_ID_PARTICLE
	"Particle",

	// ENT_ID_RAGDOLL
	"Ragdoll",

	// ENT_ID_EVILEGG
	"Evil Egg",
};


// Array of EntTile textures indexed by entity ids (EntId defined in entity/all.h)
EntTileTex g_ent_tile_tex[ENT_MAX];

// Initializes g_ent_tile_tex
void maped_init_ent_tile_tex(void)
{
	g_ent_tile_tex[ENT_ID_PLAYER].tex	= tex_egg;
	g_ent_tile_tex[ENT_ID_PLAYER].srect	= (SDL_Rect) {0, 32, 32, 32};
	g_ent_tile_tex[ENT_ID_ITEM].tex		= tex_trumpet;
	g_ent_tile_tex[ENT_ID_ITEM].srect	= (SDL_Rect) {0, 0, 19, 11};
	g_ent_tile_tex[ENT_ID_FIREBALL].tex	= tex_fireball;
	g_ent_tile_tex[ENT_ID_FIREBALL].srect	= (SDL_Rect) {0, 0, 16, 16};
	g_ent_tile_tex[ENT_ID_PARTICLE].tex	= tex_particle;
	g_ent_tile_tex[ENT_ID_PARTICLE].srect	= (SDL_Rect) {0, 0, 10, 10};
	g_ent_tile_tex[ENT_ID_RAGDOLL].tex	= tex_egg;
	g_ent_tile_tex[ENT_ID_RAGDOLL].srect	= (SDL_Rect) {64, 0, 32, 32};
	g_ent_tile_tex[ENT_ID_EVILEGG].tex	= tex_evilegg;
	g_ent_tile_tex[ENT_ID_EVILEGG].srect	= (SDL_Rect) {0, 32, 32, 32};
}

// Draw the entity map
void maped_draw_entmap(void)
{
	// Tile drawing dimensions
	int tile_left, tile_right, tile_top, tile_bottom;
	cam_get_tile_dimensions(&tile_left, &tile_right, &tile_top, &tile_bottom);

	// Draw all tiles in a loop
	for (int y = tile_top; y < tile_bottom; y++)
	{
		for (int x = tile_left; x < tile_right; x++)
		{
			EntTile et = g_ent_map[x][y];

			// Don't draw inactive tiles
			if (!et.active)
				continue;

			// Getting tile texture
			EntTileTex *tt = &g_ent_tile_tex[et.eid];
			
			// Drawing red box around tile
			SDL_Rect drect = {x * TILE_SIZE + g_cam.xshift, y * TILE_SIZE + g_cam.yshift, TILE_SIZE, TILE_SIZE};
			//SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
			//SDL_RenderDrawRect(g_renderer, &drect);

			// Drawing entity tile sprite
			SDL_RenderCopy(g_renderer, tt->tex, &(tt->srect), &drect);
		}
	}
}

// Draws map info text at the top left and an icon at the camera's position
void maped_draw_status(MapEd *ed)
{
	// Drawing info text
	char stat_string[EDSTAT_STRING_LEN_MAX];
	snprintf(stat_string, EDSTAT_STRING_LEN_MAX, EDSTAT_STRING, EDSTAT_VERSION,
		ed->tile_type == MAPED_TILE_TILE ? "Tile" : "Ent",
		ed->tile_type == MAPED_TILE_TILE ? maped_tile_name[ed->tile.tile] : maped_ent_name[ed->tile.ent],
		ed->w,
		ed->h,
		g_maped_file == NULL ? "NULL (not editing a file)" : g_maped_file,
		g_room_width,
		g_room_height
	);
	font_draw_text(stat_string, 0, 0);

	// Draw player heart at camera position
	{
		SDL_Rect srect = {0, 16, 16, 16};
		SDL_Rect drect = {g_cam.x + g_cam.xshift - 8, g_cam.y + g_cam.yshift - 8, 16, 16};
		SDL_RenderCopy(g_renderer, tex_heart, &srect, &drect);
	}
}
