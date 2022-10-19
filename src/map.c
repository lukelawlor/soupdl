/*
 * map.c contains functions for loading and saving maps.
 */

#include "dir.h"
#include "error.h"
#include "camera.h"
#include "tile/data.h"
#include "entity/all.h"
#include "map.h"

// Array containing all character representations of tiles in text map files
static const char g_tile_char_list[TILE_MAX] = {
	// TILE_AIR
	'.',
	// TILE_STONE
	's',
	// TILE_LIME
	'l',
	// TILE_IRON
	'o',
	// TILE_SPIKE
	'x'
};

// Array containing all character representations of entities in text map files (. is ignored, meaning the entity type won't normally be placed in a map file)
static const char g_ent_char_list[ENT_MAX] = {
	// ENT_ID_PLAYER
	'p',
	// ENT_ID_ITEM
	't',
	// ENT_ID_FIREBALL
	'.',
	// ENT_ID_PARTICLE
	'.',
	// ENT_ID_RAGDOLL
	'r',
	// ENT_ID_EVILEGG
	'e'
};

// Returns the tile id of a character, -1 if no tile is matched
static TileId get_tile_id(char c);

// Returns the entity id of a character, -1 if no entity is matched
static EntId get_ent_id(char c);

// Loads a map from a text file, returns nonzero on error
int map_load_txt(char *path)
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

	// Update camera limits to reflect new width and height
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
			c = fgetc(mapfile);

			// Pointer to tile id being modified
			TileId *ti = &g_tile_map[x][y];
			
			// Id of tile or entity found
			int id;

			if ((id = get_tile_id(c)) != -1)
			{
				// Tile found
				*ti = id;
			}
			else if ((id = get_ent_id(c)) != -1)
			{
				// Entity found
				*ti = TILE_AIR;
				switch (id)
				{
				case ENT_ID_PLAYER:
					g_player.x = x * TILE_SIZE;
					g_player.y = y * TILE_SIZE;
					break;
				case ENT_ID_ITEM:
					ent_item_new(x * TILE_SIZE + 16, y * TILE_SIZE + 32, ITEM_TRUMPET);
					break;
				default:
					// An entity was found, but no specific case for handling the entity was found
					PERR("unknown entity found");
					break;
				}
			}
			else
			{
				// No entity or tile was found
				PERR("no entity nor tile was found");
				*ti = TILE_AIR;
			}
		}
		// Move past newline at end of row
		fgetc(mapfile);
	}
	return 0;
}

// Saves a map to a text file, returns nonzero on error
int map_save_txt(char *path)
{
	// Getting the full path from the path argument
	char fullpath[MAX_MAP_PATH_LEN];
	sprintf(fullpath, WORKING_DIR "res/%s", path);

	// Open the map file
	FILE *mapfile;
	if ((mapfile = fopen(fullpath, "w")) == NULL)
	{
		PERRE("failed to open map file");
		return 1;
	}

	// Write map dimensions to file
	fprintf(mapfile, "%dx%d\n", g_room_width, g_room_height);

	// Write tiles to file
	for (int y = 0; y < g_room_height; y++)
	{
		for (int x = 0; x < g_room_width; x++)
		{
			fputc(g_tile_char_list[g_tile_map[x][y]], mapfile);
		}
		fprintf(mapfile, "\n");
	}

	return 0;
}

// Returns the tile id of a character, -1 if no tile is matched
static TileId get_tile_id(char c)
{
	for (int i = 0; i < TILE_MAX; i++)
		if (g_tile_char_list[i] == c)
			return i;
	return -1;
}

// Returns the entity id of a character, -1 if no entity is matched
static EntId get_ent_id(char c)
{
	for (int i = 0; i < TILE_MAX; i++)
		if (g_ent_char_list[i] == c)
			return i;
	return -1;
}
