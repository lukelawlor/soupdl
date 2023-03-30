/*
 * editor.c contains map editor code.
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>		// For strncpy()

#include <SDL2/SDL.h>

#include "../error.h"
#include "../video.h"
#include "../camera.h"
#include "../collision.h"
#include "../input.h"		// For spdl_input_string()
#include "../util/math.h"	// For MIN
#include "../tile/data.h"	// For room dimensions, TileId, and TILE_SIZE
#include "../entity/door.h"	// For ENT_DOOR_MAX and g_ent_door_map_path
#include "../entity/tile.h"
#include "../map.h"		// For map_alloc and map_free
#include "editor.h"
#include "draw.h"

// Pointer to 2d array containing entity tiles
EntTile **g_ent_map = NULL;

// Converts *x and *y from mouse coordinates on the window to tile coordinates
// If *x is set to -1, the mouse coordinates are out of bounds
static void maped_mouse_to_tile(int *x, int *y);

// Cycle through the tiles the map editor can place by num indexes
static inline void maped_pick_tile(MapEd *ed, int num);

// Cycle through the entity tiles the map editor can place by num indexes
static inline void maped_pick_ent(MapEd *ed, int num);

// Initializes the map editor
// This should be called whenever the map editor loop is about to start
int maped_init(void)
{
	if (g_ent_map != NULL)
		return 0;

	// Allocate mem for entity map
	if ((g_ent_map = map_alloc(g_map.width, g_map.height, sizeof(EntTile))) == NULL)
	{
		PERR("failed to allocate mem for entity map");
		return 1;
	}

	// Success
	return 0;
}

// Resizes the map by (width_inc, height_inc), and returns nonzero on error
int maped_resize_map(int width_inc, int height_inc)
{
	g_map.width += width_inc;
	g_map.height += height_inc;

	// Automatically update camera limits to reflect
	TileId **temp_tile_map;
	EntTile **temp_ent_map;
	if ((temp_tile_map = map_alloc(g_map.width, g_map.height, sizeof(TileId))) == NULL)
	{
		PERR("failed to allocate temporary tile map");
		return 1;
	}
	if ((temp_ent_map = map_alloc(g_map.width, g_map.height, sizeof(EntTile))) == NULL)
	{
		PERR("failed to allocate temporary entity map");
		map_free(g_map.height, temp_tile_map);
		return 1;
	}

	// Max width & height to copy over
	int width_max, height_max;
	if (width_inc > 0)
		width_max = g_map.width - width_inc;
	else
		width_max = g_map.width;
	if (height_inc > 0)
		height_max = g_map.height - height_inc;
	else
		height_max = g_map.height;

	// Copy map data into temp maps (TODO: use memcpy for this)
	for (int y = 0; y < height_max; ++y)
	{
		for (int x = 0; x < width_max; ++x)
		{
			temp_tile_map[y][x] = g_tile_map[y][x];
			temp_ent_map[y][x] = g_ent_map[y][x];
		}
	}

	// Set newly allocated tiles to default values

	#define	SET_DEFAULT_TILE()	{ \
						temp_tile_map[y][x] = TILE_AIR; \
						temp_ent_map[y][x] = (EntTile) {.active = false}; \
					}

	for (int y = 0; y < g_map.height; y++)
		for (int x = width_max; x < g_map.width; x++)
			SET_DEFAULT_TILE();
	for (int y = height_max; y < g_map.height; y++)
		for (int x = 0; x < g_map.width; x++)
			SET_DEFAULT_TILE();

	// Free old map data
	map_free(g_map.height - height_inc, g_tile_map);
	map_free(g_map.height - height_inc, g_ent_map);

	g_tile_map = temp_tile_map;
	g_ent_map = temp_ent_map;
	return 0;
}

// Handles SDL keydown events
void maped_handle_keydown(MapEd *ed, SDL_Keycode key)
{
	switch (key)
	{
	// Alt
	case SDLK_LALT:
		ed->alt = true;
		break;
	// Change tile selection
	case SDLK_1:
		maped_pick_tile(ed, -1);
		break;
	case SDLK_2:
		maped_pick_tile(ed, 1);
		break;
	// Change entity tile selection
	case SDLK_3:
		maped_pick_ent(ed, -1);
		break;
	case SDLK_4:
		maped_pick_ent(ed, 1);
		break;
	// Change outside tile
	case SDLK_5:
		if (g_tile_outside > 0)
			g_tile_outside--;
		break;
	case SDLK_6:
		if (g_tile_outside < TILE_MAX - 1)
			g_tile_outside++;
		break;
	// Change map editor selection dimensions
	case SDLK_7:
		if (ed->w > 1)
			ed->h = --ed->w;
		break;
	case SDLK_8:
		ed->h = ++ed->w;
		break;
	// Resize map
	case SDLK_MINUS:
		if (g_map.width >= 1)
			maped_resize_map(-1, 0);
		cam_update_limits();
		break;
	case SDLK_EQUALS:
		maped_resize_map(1, 0);
		cam_update_limits();
		break;
	case SDLK_LEFTBRACKET:
		if (g_map.height >= 1)
			maped_resize_map(0, -1);
		cam_update_limits();
		break;
	case SDLK_RIGHTBRACKET:
		maped_resize_map(0, 1);
		cam_update_limits();
		break;
	// Open another map
	case SDLK_o:
		{
			char map_buffer[MAP_PATH_MAX];
			if (spdl_input_string(map_buffer, MAP_PATH_MAX, "enter the path of the map to load") == -1)
				PERR("failed to get map path");
			switch (map_load_txt(map_buffer, true))
			{
			case ERR_NO_RECOVER:
				abort();
			case ERR_RECOVER:
			case ERR_NONE:
				break;
			}
		}
		break;
	// Try to save map
	case SDLK_p:
		if (g_map.editing)
		{
			if (map_save_txt(g_map.path))
			{
				PERR("map save fail: map_save_txt() failed");
			}
			else
			{
				PINF("map saved successfully!");
			}
		}
		else
		{
			// Map is not opened for editing
			PERR("map save fail: map is not opened for editing");
		}
		break;
	// Set up door linkage
	case SDLK_l:
		maped_set_door_paths();
		break;
	}
}

// Handles SDL keyup events
void maped_handle_keyup(MapEd *ed, SDL_Keycode key)
{
	switch (key)
	{
	// Alt
	case SDLK_LALT:
		ed->alt = false;
		break;
	}
}

// Handles SDL mouse button down event
void maped_handle_mbdown(MapEd *ed, Uint8 button)
{
	if (ed->alt)
	{
		int cx, cy;
		maped_mouse_to_tile(&cx, &cy);
		SDL_Rect crect = {cx, cy, 1, 1};
		for (int i = 0; i < g_map.vr_list.len; ++i)
		{
			if (check_rect(&crect, &g_map.vr_list.r[i].rect))
			{
				ed->void_rect.void_rect = &g_map.vr_list.r[i];
				ed->void_rect.rect = ed->void_rect.void_rect->rect;
				ed->void_rect.i = i;
				ed->void_rect.x = cx;
				ed->void_rect.y = cy;
				goto l_found_rect;
			}
		}

		// No selected rectangles found
		ed->state = MAPED_STATE_NONE;
		return;

	l_found_rect:
		// Selected rectangle found
		if (button == SDL_BUTTON_LEFT)
			ed->state = MAPED_STATE_VR_MOVING;
		else if (button == SDL_BUTTON_RIGHT)
			ed->state = MAPED_STATE_VR_RESIZING;
	}
	else
	{
		if (button == SDL_BUTTON_LEFT)
			ed->state = MAPED_STATE_TILING;
		else if (button == SDL_BUTTON_RIGHT)
			ed->state = MAPED_STATE_ERASING;
	}
}

// Handles SDL mouse button up event
void maped_handle_mbup(MapEd *ed, Uint8 button)
{
	ed->state = MAPED_STATE_NONE;
	/*
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
	*/
}

// Converts *x and *y from mouse coordinates on the window to tile coordinates
// If *x is set to -1, the mouse coordinates are out of bounds
static void maped_mouse_to_tile(int *x, int *y)
{
	// Mouse position on screen
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	mx /= g_screen_xscale;
	my /= g_screen_yscale;

	// Tile coordinates of mouse position
	*x = (mx - g_cam.xshift) / TILE_SIZE;
	*y = (my - g_cam.yshift) / TILE_SIZE;

	if (*x  < 0 || *x >= g_map.width || *y < 0 || *y >= g_map.height)
	{
		// Position is out of bounds
		*x = -1;
	}
}

// Places or erases tiles at the cursor's position
void maped_tile(MapEd *ed)
{
	int cx, cy;
	maped_mouse_to_tile(&cx, &cy);
	if (cx != -1)
	{
		// Dimensions of tile placing area
		const int left = cx;
		const int right = cx + ed->w > g_map.width ? g_map.width : cx + ed->w;
		const int top = cy;
		const int bottom = cy + ed->h > g_map.height ? g_map.height : cy + ed->h;

		// Placing different types of tiles
		if (ed->tile_type == MAPED_TILE_TILE)
		{
			// Placing tile

			// Tile id to place for every covered tile
			TileId tid = ed->state == MAPED_STATE_TILING ? ed->tile.tid : TILE_AIR;

			for (int y = top; y < bottom; ++y)
				for (int x = left; x < right; ++x)
					g_tile_map[y][x] = tid;
		}
		else
		{
			// Placing entity

			// Entity tile to place for every covered tile
			EntTile et;
			if (ed->state == MAPED_STATE_TILING)
				et = (EntTile) {true, ed->tile.etid};
			else
				et = (EntTile) {false, 0};

			for (int y = top; y < bottom; ++y)
				for (int x = left; x < right; ++x)
					g_ent_map[y][x] = et;
		}
	}
}

// Moves void rectangles with the mouse
// Called when ed->state == MAPED_STATE_VR_MOVING
void maped_vr_move(MapEd *ed)
{
	int cx, cy;
	maped_mouse_to_tile(&cx, &cy);
	if (cx == -1)
		return;

	ed->void_rect.void_rect->rect.x = clamp(ed->void_rect.rect.x + (cx - ed->void_rect.x), 0, g_map.width - ed->void_rect.rect.w);
	ed->void_rect.void_rect->rect.y = clamp(ed->void_rect.rect.y + (cy - ed->void_rect.y), 0, g_map.height - ed->void_rect.rect.h);
}

// Resizes void rectangles with the mouse
// Called when ed->state == MAPED_STATE_VR_RESIZING
void maped_vr_resize(MapEd *ed)
{
	int cx, cy;
	maped_mouse_to_tile(&cx, &cy);
	if (cx == -1)
		return;

	ed->void_rect.void_rect->rect.w = clamp(ed->void_rect.rect.w + (cx - ed->void_rect.x), 1, g_map.width - ed->void_rect.rect.x);
	ed->void_rect.void_rect->rect.h = clamp(ed->void_rect.rect.h + (cy - ed->void_rect.y), 1, g_map.height - ed->void_rect.rect.y);

}

// Ask the user to set contents of g_ent_door_map_path (defined in ../entity/door.h)
void maped_set_door_paths(void)
{
	// Buffer to store the current map name in
	char map_buffer[ENT_DOOR_MAP_PATH_MAX];

	const int prompt_len = 20;

	// String to store the prompt message in
	char prompt[prompt_len];

	for (int i = 0; i < ENT_DOOR_MAX; i++)
	{
		snprintf(prompt, prompt_len, "door %d map path", i);
		spdl_input_string(map_buffer, ENT_DOOR_MAP_PATH_MAX, prompt);
		strncpy(g_ent_door_map_path[i], map_buffer, ENT_DOOR_MAP_PATH_MAX);
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
	ed->tile.tid += num;
l_return:
	ed->tile.tid = clamp(ed->tile.tid, 0, TILE_MAX - 1);
}

// Cycle through the entity tiles the map editor can place by num indexes
static inline void maped_pick_ent(MapEd *ed, int num)
{
	if (ed->tile_type == MAPED_TILE_TILE)
	{
		ed->tile_type = MAPED_TILE_ENT;
		goto l_return;
	}
	ed->tile.etid += num;
l_return:
	ed->tile.etid = clamp(ed->tile.etid, 0, ENT_TILE_MAX - 1);
}
