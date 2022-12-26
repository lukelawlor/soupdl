/*
 * map.c contains functions for loading and saving maps.
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>	// For memcmp()

#include "dir.h"
#include "error.h"
#include "camera.h"
#include "tile/data.h"
#include "entity/id.h"
#include "entity/metadata.h"
#include "entity/tile.h"
#include "editor/editor.h"
#include "map.h"

// Default outside tile value
#define	MAP_DEF_OT	TILE_LIME

// Length of string used to store current map option being read
#define	MAP_OPTION_LEN	100

// Returns the tile id of a character, -1 if no tile is matched
static inline TileId get_tile_id(char c);

// Returns the entity id of a character, -1 if no entity is matched
static inline EntId get_ent_id(char c);

// Loads a map from a text file, returns nonzero on error
// The editing paramter is true when the map is being opened for editing, make sure maped_init (from editor/editor.h) has been called before this is indicated
int map_load_txt(char *path, bool editing)
{
	// Free any old data in g_tile_map if it exists
	map_free((void **) g_tile_map);

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

	// Load the entity map
	if (editing)
	{
		if (maped_init())
		{
			fprintf(stderr, "failed to initialize map editor\n");
			return 1;
		}
	}

	// Update camera limits to reflect new width and height
	cam_update_limits();
	
	// Allocate mem for the entire map
	if ((g_tile_map = (TileId **) map_alloc(sizeof(TileId))) == NULL)
		return 1;

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

				// Default entity x & y position
				const int ex = x * TILE_SIZE;
				const int ey = y * TILE_SIZE;

				switch (id)
				{
				/*
				case ENT_ID_PLAYER:
					g_player.b.x = ex;
					g_player.b.y = ey;
					break;
				case ENT_ID_ITEM:
					ent_new_ITEM(ex + 16, ey + 32, ITEM_TRUMPET);
					break;
				case ENT_ID_GROUNDGUY:
					ent_new_GROUNDGUY(ex, ey, 0.0f);
					break;
				case ENT_ID_SLIDEGUY:
					ent_new_SLIDEGUY(ex, ey);
					break;
				case ENT_ID_TURRET:
					ent_new_TURRET(ex, ey);
					break;
				*/
				case ENT_ID_PLAYER:
					(g_ets[ENT_TILE_PLAYER])(ex, ey);
					break;
				case ENT_ID_ITEM:
					(g_ets[ENT_TILE_TRUMPET])(ex, ey);
					break;
				case ENT_ID_GROUNDGUY:
					(g_ets[ENT_TILE_GROUNDGUY])(ex, ey);
					break;
				case ENT_ID_SLIDEGUY:
					(g_ets[ENT_TILE_SLIDEGUY])(ex, ey);
					break;
				case ENT_ID_TURRET:
					(g_ets[ENT_TILE_TURRET])(ex, ey);
					break;
				/*
				case ENT_ID_FIREBALL:
					ent_new_FIREBALL(ex, ey, (rand() % 10) / 50.0f, (rand() % 10) / 50.0f);
					break;
				case ENT_ID_PARTICLE:
					ent_new_PARTICLE(ex, ey, rand() % PTCL_MAX);
					break;
				case ENT_ID_RAGDOLL:
					ent_new_RAGDOLL(ex, ey, (rand() % 40) / 4.0f, (rand() % 40) / 4.0f, RAGDOLL_EGG);
					break;
				case ENT_ID_CLOUD:
					ent_new_CLOUD(ex, ey, 1);
					break;
				*/
				default:
					// An entity was found, but no specific case for handling the entity was found
					PERR();
					fprintf(stderr, "unknown entity with id %d found at (%d, %d)\n", id, x, y);
					break;
				}
				
				// If the map is opened for editing, add the entity id to the map
				if (editing)
				{
					g_ent_map[x][y].active = true;
					g_ent_map[x][y].eid = id;
				}
			}
			else
			{
				// No entity or tile was found
				PERR();
				fprintf(stderr, "no entity nor tile found at (%d, %d)\n", x, y);
				*ti = TILE_AIR;
			}
		}
		// Move past newline at end of row
		fgetc(mapfile);
	}

	// Read misc map options at the bottom of the file
	while ((c = fgetc(mapfile)) == '.')
	{
		char option_str[MAP_OPTION_LEN];
		fscanf(mapfile, "%s ", option_str);
		
		if (memcmp(option_str, "ot", 3) == 0)
		{
			fscanf(mapfile, "%c\n", &c);

			int ti;
			if ((ti = get_tile_id(c)) != -1)
				g_tile_outside = ti;
			else
				g_tile_outside = MAP_DEF_OT;
		}
		else
		{
			// No option found
			PERR();
			fprintf(stderr, "no option found for \"%s\"\n", option_str);
			break;
		}
	}

	fclose(mapfile);
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
		PERR();
		fprintf(stderr, "failed to open map file \"%s\"\n", path);
		return 1;
	}

	// Write map dimensions to file
	fprintf(mapfile, "%dx%d\n", g_room_width, g_room_height);

	// Write tiles to file
	for (int y = 0; y < g_room_height; y++)
	{
		for (int x = 0; x < g_room_width; x++)
		{
			if (g_ent_map[x][y].active)
			{
				// Write an entity
				fputc(g_ent_md[g_ent_map[x][y].eid].map_char, mapfile);
			}
			else
			{
				// Write a tile
				fputc(g_tile_md[g_tile_map[x][y]].map_char, mapfile);
			}
		}
		fprintf(mapfile, "\n");
	}

	// Write options to file
	fprintf(mapfile, ".ot %c\n", g_tile_md[g_tile_outside].map_char);

	fclose(mapfile);

	return 0;
}

// Allocates and returns a pointer to map memory with size bytes for each index, returns NULL on error
void **map_alloc(size_t size)
{
	void **map_ptr;

	// Allocate mem for the x row
	if ((map_ptr = calloc(g_room_width, sizeof(void *))) == NULL)
	{
		fprintf(stderr, "failed to allocate mem for map\n");
		return NULL;
	}

	// Allocate mem for the y columns
	for (int x = 0; x < g_room_width; x++)
	{
		if ((map_ptr[x] = calloc(g_room_height, size)) == NULL)
		{
			fprintf(stderr, "failed to allocate mem for map\n");

			// Free all x tile id arrays allocated so far
			for (int xx = x - 1; xx >= 0; xx--)
				free(map_ptr[xx]);
			free(map_ptr);
			return NULL;
		}
	}

	return map_ptr;
}

// Frees map memory
void map_free(void **map_ptr)
{
	// Map never existed/was already freed
	if (map_ptr == NULL)
		return;
	
	// Free all x rows
	for (int x = 0; x < g_room_width; x++)
		free(map_ptr[x]);
	free(map_ptr);
}

// Returns false if two entities or tiles share the same map character, should be used in an assert
bool map_assert_dupchars(void)
{
	// Array of all map chars
	const int arr_len = TILE_MAX + ENT_MAX;
	char arr[arr_len];

	// Adding map chars to the array
	{
		// Index of next char to add
		int index = 0;

		for (int i = 0; i < TILE_MAX; i++)
			arr[index++] = g_tile_md[i].map_char;
		for (int i = 0; i < ENT_MAX; i++)
			arr[index++] = g_ent_md[i].map_char;
		assert(index == arr_len && "wrong number of map chars added to array");
	}

	// Checking for duplicates
	for (int i = 0; i < arr_len - 1; i++)
	{
		for (int j = i + 1; j < arr_len; j++)
		{
			if (arr[i] == arr[j])
			{
				PERR();
				fprintf(stderr, "char \'%c\' found in ids %d and %d\n", arr[i], i, j);
				return false;
			}
		}
	}
	return true;
}

// Returns the tile id of a character, -1 if no tile is matched
static inline TileId get_tile_id(char c)
{
	for (int i = 0; i < TILE_MAX; i++)
		if (g_tile_md[i].map_char == c)
			return i;
	return -1;
}

// Returns the entity id of a character, -1 if no entity is matched
static inline EntId get_ent_id(char c)
{
	for (int i = 0; i < ENT_MAX; i++)
		if (g_ent_md[i].map_char == c)
			return i;
	return -1;
}
