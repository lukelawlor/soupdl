/*
 * data.c contains functions for manipulating tile data.
 */

#include <stdio.h>

#include "../dir.h"		// For loading map files
#include "../camera.h"		// For updating the camera when the room dimensions change
#include "../entity/all.h"	// For creating entities from map files
#include "data.h"

// Array containing the properties of different types of tiles
static const TileProperty tile_property_list[TILE_MAX] = {
	// TILE_AIR (NOTE: spoint doesn't matter here since air is never drawn)
	{.spoint={0,32},.flags=0},

	// TILE_STONE
	{.spoint={0,0},.flags=TFLAG_SOLID},

	// TILE_LIME
	{.spoint={TILE_SIZE*1,0},.flags=TFLAG_SOLID},

	// TILE_IRON
	{.spoint={TILE_SIZE*2,0},.flags=TFLAG_SOLID},

	// TILE_SPIKE
	{.spoint={TILE_SIZE*3,0},.flags=0},
};

// Constant pointer to the first index of tile_property_list
const TileProperty *const g_tile_property = tile_property_list;

// Room width and height in tiles, not pixels
int g_room_width = 1;
int g_room_height = 1;

// 2d array containing TileId objects
TileId **g_tile_map = NULL;

// Tile type for tiles outside the map
TileId g_tile_outside = TILE_LIME;

// Free all tile spaces and arrays containing pointers to them
static void tile_map_free(void);

/*
 * Loads tile data into an array from a text file and sets tile_space to a pointer to it.
 *
 * Prints errors and returns nonzero on error.
 */
int tile_map_load_txt(char *path)
{
	// Free any old data in g_tile_space if it exists
	tile_map_free();

	// Getting the full path from the path argument
	char fullpath[MAX_MAP_PATH_LEN];
	sprintf(fullpath, WORKING_DIR "res/%s", path);
	FILE *mapfile;
	if ((mapfile = fopen(fullpath, "r")) == NULL)
	{
		fprintf(stderr, "Failed to load text map file \"%s\"\n", fullpath);
		return 1;
	}

	// Get map width and height
	fscanf(mapfile, "%dx%d\n", &g_room_width, &g_room_height);
	cam_update_limits();
	
	// Allocate mem for the entire map
	if ((g_tile_map = calloc(g_room_width, sizeof(TileId *))) == NULL)
	{
		fprintf(stderr, "Failed to allocate mem for map\n");
		return 1;
	}
	for (int x = 0; x < g_room_width; x++)
	{
		if ((g_tile_map[x] = calloc(g_room_height, sizeof(TileId))) == NULL)
		{
			fprintf(stderr, "Failed to allocate mem for map\n");

			// Free all x tile id arrays allocated so far
			for (int xx = x - 1; xx >= 0; xx--)
				free(g_tile_map[xx]);
			free(g_tile_map);
			g_tile_map = NULL;
			return 1;
		}
	}

	// Current character being read from the file
	char c;

	for (int y = 0; y < g_room_height; y++)
	{
		for (int x = 0; x < g_room_width; x++)
		{
			TileId *ti = &g_tile_map[x][y];
			/*
			if ((ts = malloc(sizeof(TileSpace))) == NULL)
			{
				fprintf(stderr, "Failed to allocate mem for map tile space\n");

				// Free all TileSpace objects allocated so far
				for (int xx = x - 1; xx >= 0; xx--)
					free(g_tile_space[xx][y]);
				for (int yy = y - 1; yy >= 0; yy--)
					for (int xx = 0; xx < g_room_width; xx++)
						free(g_tile_space[xx][yy]);

				// Free all x tile space arrays
				for (int xx = 0; xx < g_room_width; xx++)
					free(g_tile_space[xx]);
				
				free(g_tile_space);
				g_tile_space = NULL;
				return 1;
			}
			*/
			switch (c = fgetc(mapfile))
			{
				case 'p':
					g_player.x = x * TILE_SIZE;
					g_player.y = y * TILE_SIZE;
					*ti = TILE_AIR;
					break;
				case 't':
					ent_item_new(x * TILE_SIZE + 16, y * TILE_SIZE + 32, ITEM_TRUMPET);
					*ti = TILE_AIR;
					break;
				default:
					*ti = c - '0';
					break;
			}
		}
		fgetc(mapfile);
	}
	return 0;
}

// Frees all data pointed to by g_tile_map
static void tile_map_free(void)
{
	if (g_tile_map == NULL)
		return;
	
	/*
	// Free all TileId arrays
	for (int x = 0; x < g_room_width; x++)
		for (int y = 0; y < g_room_height; y++)
			free(g_tile_space[x][y]);
	*/
	
	// Free all x tile space arrays
	for (int x = 0; x < g_room_width; x++)
		free(g_tile_map[x]);
	
	free(g_tile_map);
	g_tile_map = NULL;
}
