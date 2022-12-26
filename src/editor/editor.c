/*
 * editor.c contains map editor code.
 */

#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "../error.h"
#include "../camera.h"
#include "../util/math.h"	// For MIN
#include "../tile/data.h"	// For room dimensions, TileId, and TILE_SIZE
#include "../entity/id.h"
#include "../map.h"		// For map_alloc and map_free
#include "editor.h"
#include "draw.h"

// Path to map file being edited
char *g_maped_file = NULL;

// Pointer to 2d array containing entity tiles
EntTile **g_ent_map;

// Cycle through the tiles the map editor can place by num indexes
static inline void maped_pick_tile(MapEd *ed, int num);

// Cycle through the entity tiles the map editor can place by num indexes
static inline void maped_pick_ent(MapEd *ed, int num);

// Initializes the map editor
int maped_init(void)
{
	// Don't initialize more than once
	static bool first_exec = true;
	if (!first_exec)
		return 0;

	// Allocate mem for entity map
	if ((g_ent_map = (EntTile **) map_alloc(sizeof(EntTile))) == NULL)
	{
		PERR();
		fprintf(stderr, "failed to allocate mem for entity map\n");
		return 1;
	}

	// Set all entity spaces to inactive
	for (int y = 0; y < g_room_height; y++)
		for (int x = 0; x < g_room_width; x++)
			g_ent_map[x][y].active = false;
	
	// Success
	first_exec = false;
	return 0;
}

// Resizes the map by (width_inc, height_inc), and returns nonzero on error
int maped_resize_map(int width_inc, int height_inc)
{
	g_room_width += width_inc;
	g_room_height += height_inc;

	// Automatically update camera limits to reflect
	TileId **temp_tile_map;
	EntTile **temp_ent_map;
	if ((temp_tile_map = (TileId **) map_alloc(sizeof(TileId))) == NULL)
	{
		PERR();
		fprintf(stderr, "failed to allocate temporary tile map\n");
		return 1;
	}
	if ((temp_ent_map = (EntTile **) map_alloc(sizeof(EntTile))) == NULL)
	{
		PERR();
		fprintf(stderr, "failed to allocate temporary entity map\n");
		map_free((void **) temp_tile_map);
		return 1;
	}

	// Max width & height to copy over
	int width_max, height_max;
	if (width_inc > 0)
		width_max = g_room_width - width_inc;
	else
		width_max = g_room_width;
	if (height_inc > 0)
		height_max = g_room_height - height_inc;
	else
		height_max = g_room_height;

	// Copy map data into temp maps (TODO: use memcpy for this)
	for (int x = 0; x < width_max; x++)
	{
		for (int y = 0; y < height_max; y++)
		{
			temp_tile_map[x][y] = g_tile_map[x][y];
			temp_ent_map[x][y] = g_ent_map[x][y];
		}
	}

	// Set newly allocated tiles to default values

	#define	SET_DEFAULT_TILE()	{ \
						temp_tile_map[x][y] = TILE_AIR; \
						temp_ent_map[x][y] = (EntTile) {.active = false}; \
					}

	for (int x = width_max; x < g_room_width; x++)
		for (int y = 0; y < g_room_height; y++)
			SET_DEFAULT_TILE();
	for (int y = height_max; y < g_room_height; y++)
		for (int x = 0; x < g_room_width; x++)
			SET_DEFAULT_TILE();

	// A little hack-y, but room width & height are manipulated to properly free the old maps
	g_room_width -= width_inc;
	g_room_height -= height_inc;
	map_free((void **) g_tile_map);
	map_free((void **) g_ent_map);
	g_room_width += width_inc;
	g_room_height += height_inc;

	g_tile_map = temp_tile_map;
	g_ent_map = temp_ent_map;
	return 0;
}

// Handles SDL keydown events
void maped_handle_keydown(MapEd *ed, SDL_Keycode key)
{
	switch (key)
	{
		case SDLK_1:
			maped_pick_tile(ed, -1);
			break;
		case SDLK_2:
			maped_pick_tile(ed, 1);
			break;
		case SDLK_3:
			maped_pick_ent(ed, -1);
			break;
		case SDLK_4:
			maped_pick_ent(ed, 1);
			break;
		case SDLK_5:
			if (g_tile_outside > 0)
				g_tile_outside--;
			break;
		case SDLK_6:
			if (g_tile_outside < TILE_MAX - 1)
				g_tile_outside++;
			break;
		case SDLK_7:
			if (ed->w > 1)
				ed->h = --ed->w;
			break;
		case SDLK_8:
			ed->h = ++ed->w;
			break;
		case SDLK_MINUS:
			if (g_room_width >= 1)
				maped_resize_map(-1, 0);
			cam_update_limits();
			break;
		case SDLK_EQUALS:
			maped_resize_map(1, 0);
			cam_update_limits();
			break;
		case SDLK_LEFTBRACKET:
			if (g_room_height >= 1)
				maped_resize_map(0, -1);
			cam_update_limits();
			break;
		case SDLK_RIGHTBRACKET:
			maped_resize_map(0, 1);
			cam_update_limits();
			break;
		case SDLK_p:
			// Try to save map
			if (g_maped_file == NULL)
			{
				// No filename provided
				PERR();
				fprintf(stderr, "map save fail: no filename provided\n");
			}
			else
				map_save_txt(g_maped_file);
			break;
	}
}

// Handles SDL mouse button down event
void maped_handle_mbdown(MapEd *ed, Uint8 button)
{
	if (button == SDL_BUTTON_LEFT)
		ed->state = MAPED_STATE_TILING;
	else if (button == SDL_BUTTON_RIGHT)
		ed->state = MAPED_STATE_ERASING;
}

// Handles SDL mouse button up event
void maped_handle_mbup(MapEd *ed, Uint8 button)
{
	if (button == SDL_BUTTON_LEFT)
	{
		if (ed->state == MAPED_STATE_TILING)
			ed->state = MAPED_STATE_NONE;
	}
	else if (button == SDL_BUTTON_RIGHT)
	{
		if (ed->state == MAPED_STATE_ERASING)
			ed->state = MAPED_STATE_NONE;
	}
}

// Places or erases tiles at the cursor's position
void maped_tile(MapEd *ed)
{
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	const int cx = (mx - g_cam.xshift) / TILE_SIZE;
	const int cy = (my - g_cam.yshift) / TILE_SIZE;
	if (	cx >= 0 && cx < g_room_width &&
		cy >= 0 && cy < g_room_height
		)
	{
		// Dimensions of tile placing area
		const int left = cx;
		const int right = cx + ed->w > g_room_width ? g_room_width : cx + ed->w;
		const int top = cy;
		const int bottom = cy + ed->h > g_room_height ? g_room_height : cy + ed->h;

		// Placing different types of tiles
		if (ed->tile_type == MAPED_TILE_TILE)
		{
			// Placing tile

			// Tile id to place for every covered tile
			TileId ti = ed->state == MAPED_STATE_TILING ? ed->tile.tile : TILE_AIR;

			for (int x = left; x < right; x++)
				for (int y = top; y < bottom; y++)
					g_tile_map[x][y] = ti;
		}
		else
		{
			// Placing entity

			// Entity tile to place for every covered tile
			EntTile et;
			if (ed->state == MAPED_STATE_TILING)
				et = (EntTile) {true, ed->tile.ent};
			else
				et = (EntTile) {false, 0};

			for (int x = left; x < right; x++)
				for (int y = top; y < bottom; y++)
					g_ent_map[x][y] = et;
		}
	}
}

// Cycle through the tiles the map editor can place by num indexes
static inline void maped_pick_tile(MapEd *ed, int num)
{
	if (ed->tile_type == MAPED_TILE_ENT)
	{
		ed->tile_type = MAPED_TILE_TILE;
		goto l_return;
	}
	ed->tile.tile += num;
l_return:
	ed->tile.tile = clamp(ed->tile.tile, 0, TILE_MAX - 1);
}

// Cycle through the entity tiles the map editor can place by num indexes
static inline void maped_pick_ent(MapEd *ed, int num)
{
	if (ed->tile_type == MAPED_TILE_TILE)
	{
		ed->tile_type = MAPED_TILE_ENT;
		goto l_return;
	}
	ed->tile.ent += num;
l_return:
	ed->tile.ent = clamp(ed->tile.ent, 0, ENT_MAX - 1);
}
