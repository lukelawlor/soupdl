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
#include "entity/tile.h"
#include "entity/all.h"
#include "editor/editor.h"
#include "util/string.h"
#include "map.h"

// Default outside tile value
#define	MAP_DEF_OT	TILE_LIME

// Length of string used to store current map option being read
#define	MAP_OPTION_LEN	100

// Returns the tile id of a character, -1 if no tile is matched
static inline int get_tile_id(char c);

// Returns the entity id of a character, -1 if no entity is matched
static inline int get_ent_id(char c);

// Loads a map from a text file, returns nonzero on error
// The editing paramter is true when the map is being opened for editing, make sure maped_init (from editor/editor.h) has been called before this is indicated
int map_load_txt(char *path, bool editing)
{
	// Getting the full path from the path argument
	char fullpath[RES_PATH_MAX];
	snprintf(fullpath, RES_PATH_MAX, DIR_MAP "/%s", path);
	FILE *mapfile;
	if ((mapfile = fopen(fullpath, "r")) == NULL)
	{
		fprintf(stderr, "Failed to load text map file \"%s\"\n", fullpath);
		return 1;
	}
	
	// Start changing the map
	ent_destroy_temp();
	g_player.door_stop = true;

	// Free any old data in g_tile_map if it exists
	map_free((void **) g_tile_map);
	map_free((void **) g_ent_map);
	g_ent_map = NULL;

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

				// Call entity spawner
				if ((g_ent_tile[id].spawner)(x * TILE_SIZE, y * TILE_SIZE) != 0)
				{
					PERR("entity spawner for id %d (%s) failed at (%d, %d)", id, g_ent_tile[id].name, x, y);
				}
				
				// If the map is opened for editing, add the entity id to the map
				if (editing)
				{
					g_ent_map[x][y].active = true;
					g_ent_map[x][y].etid = id;
				}
			}
			else
			{
				// No entity or tile was found
				PERR("no entity nor tile found at (%d, %d)", x, y);
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
		else if (memcmp(option_str, "d", 2) == 0)
		{
			// Door id
			int did;

			int map_path_len = 0;
			fscanf(mapfile, "%d ", &did);
			if (!ENT_DOOR_ID_IS_VALID(did))
			{
				PERR("d: invalid door id %d specified", did);
				goto l_skip_line;
			}
			while ((c = fgetc(mapfile)) != '\n')
			{
				g_ent_door_map_path[did][map_path_len] = c;
				if (++map_path_len >= ENT_DOOR_MAP_PATH_MAX)
				{
					PERR("d: map path for door id %d was too long (over " STR(MAP_PATH_MAX) " characters)", did);
					goto l_skip_line;
				}
			}
			g_ent_door_map_path[did][map_path_len] = '\0';
		}
		else
		{
			// No option found
			PERR("unknown option \"%s\" found", option_str);

			// Move to next line of input
		l_skip_line:
			while ((c = fgetc(mapfile)) != '\n')
				;
		}
	}
	fclose(mapfile);

	// Move the player to the door with the last id used
	EntDOOR *e = g_er[ENT_ID_DOOR]->e;
	for (int i = 0; i < g_er[ENT_ID_DOOR]->len; i++)
	{
		if (e->did == g_ent_door_last_used)
		{
			g_player.b.x = e->b.x;
			g_player.b.y = e->b.y;
			break;
		}
		e++;
	}
	return 0;
}

// Saves a map to a text file, returns nonzero on error
int map_save_txt(char *path)
{
	// Getting the full path from the path argument
	char fullpath[RES_PATH_MAX];
	snprintf(fullpath, RES_PATH_MAX, DIR_MAP "/%s", path);

	// Open the map file
	FILE *mapfile;
	if ((mapfile = fopen(fullpath, "w")) == NULL)
	{
		PERR("failed to open map file \"%s\"", path);
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
				fputc(g_ent_tile[g_ent_map[x][y].etid].map_char, mapfile);
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
	const int arr_len = TILE_MAX + ENT_TILE_MAX;
	char arr[arr_len];

	// Adding map chars to the array
	{
		// Index of next char to add
		int index = 0;

		// Add tile chars
		for (int i = 0; i < TILE_MAX; i++)
			arr[index++] = g_tile_md[i].map_char;

		// Add entity tile chars
		for (int i = 0; i < ENT_TILE_MAX; i++)
			arr[index++] = g_ent_tile[i].map_char;

		assert(index == arr_len && "wrong number of map chars added to array");
	}

	// Checking for duplicates
	for (int i = 0; i < arr_len - 1; i++)
	{
		for (int j = i + 1; j < arr_len; j++)
		{
			if (arr[i] == arr[j])
			{
				PERR("char \'%c\' found in ids %d and %d", arr[i], i, j);
				return false;
			}
		}
	}
	return true;
}

// Returns the tile id of a character, -1 if no tile is matched
static inline int get_tile_id(char c)
{
	for (int i = 0; i < TILE_MAX; i++)
		if (g_tile_md[i].map_char == c)
			return i;
	return -1;
}

// Returns the entity tile id of a character, -1 if no entity is matched
static inline int get_ent_id(char c)
{
	for (int i = 0; i < ENT_TILE_MAX; i++)
		if (g_ent_tile[i].map_char == c)
			return i;
	return -1;
}
