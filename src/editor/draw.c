/*
 * draw.c contains functions for drawing parts of the editor UI.
 */

#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>		// For SDL_Texture

#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "../font.h"
#include "../tile/data.h"	// For TILE_SIZE
#include "../entity/tile.h"
#include "../entity/door.h"	// For g_ent_door_map_path & door macros
#include "../map.h"		// For g_map
#include "../void_rect.h"
#include "editor.h"
#include "draw.h"

#define	EDSTAT_VERSION		"v0.1.0"
#define	EDSTAT_STRING		"SoupDL 06 Map Editor %s\n\n" \
				"Tile:     %s/%s\n" \
				"TileSize: %dx%d\n" \
				"Map:      %s\n" \
				"MapSize:  %dx%d\n" \
				"CamPos:   %d,%d"
#define	EDSTAT_STRING_LEN_MAX	256
#define	VRVAL_STRING_LEN_MAX	(VOID_RECT_STR_LEN + 2)
#define	PATH_STRING_LEN_MAX	(8 + ENT_DOOR_MAP_PATH_MAX)

// Draw the entity map
static void maped_draw_entmap(void);

// Draws map info text at the top left and an icon at the camera's position
static void maped_draw_status(MapEd *ed);

// Draws void rectangles
static void maped_draw_vrlist(void);

// Draws all components of the editor ui
void maped_draw_all(MapEd *ed)
{
	maped_draw_entmap();
	maped_draw_status(ed);
	maped_draw_vrlist();
}

// Draw the entity map
static void maped_draw_entmap(void)
{
	// Tile drawing dimensions
	int tile_left, tile_right, tile_top, tile_bottom;
	cam_get_tile_dimensions(&tile_left, &tile_right, &tile_top, &tile_bottom);

	// Draw all tiles in a loop
	for (int y = tile_top; y < tile_bottom; y++)
	{
		for (int x = tile_left; x < tile_right; x++)
		{
			EntTile et = g_ent_map[y][x];

			// Don't draw inactive tiles
			if (!et.active)
				continue;

			// Getting tile texture
			EntTileTex *tt = &g_ent_tile[et.etid].tex;
			
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
static void maped_draw_status(MapEd *ed)
{
	// Draw info text
	char stat_string[EDSTAT_STRING_LEN_MAX];
	snprintf(stat_string, EDSTAT_STRING_LEN_MAX, EDSTAT_STRING, EDSTAT_VERSION,
		ed->tile_type == MAPED_TILE_TILE ? "Tile" : "Ent",
		ed->tile_type == MAPED_TILE_TILE ? g_tile_md[ed->tile.tid].name : g_ent_tile[ed->tile.etid].name,
		ed->w,
		ed->h,
		g_map.editing ? g_map.path : "MAP NOT OPENED FOR EDITING!!!",
		g_map.width,
		g_map.height,
		g_cam.x / TILE_SIZE,
		g_cam.y / TILE_SIZE
	);
	font_draw_text(stat_string, 0, 0);

	// Draw door map paths
	char path_string[PATH_STRING_LEN_MAX];
	for (EntDoorId i = 0; i < ENT_DOOR_MAX; i++)
	{
		snprintf(path_string, PATH_STRING_LEN_MAX, "d %d %s", (int) i, g_ent_door_map_path[i]);
		font_draw_text(path_string, 0, 8 * FONT_CHAR_YSPACE + i * FONT_CHAR_YSPACE);
	}

	// Draw image at camera position
	{
		const SDL_Rect srect = {66, 43, 28, 22};
		SDL_Rect drect = {g_cam.x + g_cam.xshift - srect.w / 2, g_cam.y + g_cam.yshift - srect.h / 2, srect.w, srect.h};
		SDL_RenderCopy(g_renderer, tex_coolegg, &srect, &drect);
	}
}

// Draws void rectangles
static void maped_draw_vrlist(void)
{
	for (int i = 0; i < g_map.vr_list.len; ++i)
	{
		VoidRect *r = &g_map.vr_list.r[i];
		SDL_Rect drect = r->rect;
		drect.x = drect.x * TILE_SIZE + g_cam.xshift;
		drect.y = drect.y * TILE_SIZE + g_cam.yshift;
		drect.w *= TILE_SIZE;
		drect.h *= TILE_SIZE;
		SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(g_renderer, &drect);

		char val_str[VRVAL_STRING_LEN_MAX];
		if (r->value_is_str)
			snprintf(val_str, VRVAL_STRING_LEN_MAX, "s%s", r->value.s);
		else
			snprintf(val_str, VRVAL_STRING_LEN_MAX, "i%d", r->value.i);
		font_draw_text(val_str, drect.x, drect.y);
	}
}
