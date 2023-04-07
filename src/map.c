/*
 * map.c contains functions for loading and saving maps.
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>	// For memcmp() and strncpy()

#include <SDL2/SDL.h>	// For SDL_Rect

#include "camera.h"
#include "collector.h"
#include "dir.h"
#include "editor/editor.h"
#include "entity/id.h"
#include "entity/tile.h"
#include "entity/all.h"
#include "entity/cloud.h"
#include "error.h"
#include "fileio.h"
#include "map.h"
#include "tile/data.h"
#include "util/string.h"
#include "void_rect.h"

// Shorthand for the char used to represent TILE_AIR
#define	AIR_CHAR	g_tile_md[TILE_AIR].map_char

// Used in map files to denote a map option specification
#define	MAP_OPT_SYMBOL	'>'

// Default outside tile value
#define	MAP_DEF_OT	TILE_LIME

// Length of string used to store current map option being read
#define	MAP_OPTION_LEN	5

// Info about the current map loaded
MapInfo g_map;

// Returns nonzero on error
static int map_read_line(char *dest, int expected_width, FILE *map_file)
{
	// Read all chars except the last
	for (int i = 0; i < expected_width; ++i)
	{
		switch (dest[i] = fgetc(map_file))
		{
		case EOF:
		case '\n':
			// Width of line was shorter than expected width
			return -1;
		}
	}

	// Read last char
	if (fgetc(map_file) == '\n')
	{
		// Width of line was as expected
		return 0;
	}

	// Width of line was greater than expected with
	return -1;
}

// Loads a map from a text file
// The editing parameter is true when the map is being opened for editing
ErrCode map_load_txt(char *path, bool editing)
{
	// This will be used as the return code for the function if an error occurs
	ErrCode err_code = ERR_RECOVER;

	// Pointers that must be freed when the function exits
		
		// Contains the textual representation of the map from the file as map memory
		char **map_data = NULL;

		// Contains entities from **map_data
		EntTileId **ent_tile_data = NULL;

	// Getting the full path from the path argument
	char fullpath[RES_PATH_MAX];
	snprintf(fullpath, RES_PATH_MAX, DIR_MAP "/%s", path);
	FILE *map_file;
	if ((map_file = fopen(fullpath, "r")) == NULL)
	{
		PERR("failed to load text map file \"%s\"", fullpath);
		return err_code;
	}

	// Begin to read the map tile data into memory
	
	// Buffer used to read a line of text from the map
	char *map_line = malloc((MAP_WIDTH_MAX + 1) * sizeof(char));

	// Read the first line of text from the map INCLUDING \0
	// Get the map width from the return value of spdl_readstr()
	int map_width = spdl_readstr(
		map_line,
		MAP_WIDTH_MAX + 1,
		'\n',
		map_file
	);
	if (map_width == -1)
	{
		PERR("error reading the first line of map tile data");
		return err_code;
	}

	// Resize *map_line to fit the estimated map width
	// Store the map chars WITHOUT \0
	{
		char *temp = reallocarray(map_line, map_width, sizeof(char));
		if (temp == NULL)
		{
			PERR("failed to realloc map_line");
			free(map_line);
			if (fclose(map_file))
				PERR("failed to close map file");
			return err_code;
		}
		map_line = temp;
	}

	// Buffer used to store the chars that make up the map in the file
	map_data = malloc(MAP_HEIGHT_MAX * sizeof(char *));
	map_data[0] = map_line;
	int map_height = 1;
	
	// Read in more lines of map data, keeping track of map_height along the way
	for (; map_height < MAP_HEIGHT_MAX; ++map_height)
	{
		// Check the first character of the line
		int c = fgetc(map_file);
		if (ungetc(c, map_file) == EOF)
		{
			PERR("failed to ungetc first character of map line");
			goto l_exit;
		}
			
		switch (c)
		{
		case MAP_OPT_SYMBOL:
			// End of map & start of options found
			goto l_heightloop_exit;
		case EOF:
		case '\n':
			// Reading error
			PERR("failed to read a char at the start of a map line");
			goto l_exit;
		}

		// Allocate mem for a new line
		map_data[map_height] = malloc(map_width * sizeof(char));

		// Read a new line in
		if (map_read_line(map_data[map_height], map_width, map_file))
		{
			PERR("failed to read map data from line %d", map_height + 1);
			goto l_exit;
		}
	}

	// Since the loop exited normally, MAP_HEIGHT_MAX was read
	// This will cause lines beyond the max height to not be read, which could be an error
	PERR("maximum height for a map was read in");

l_heightloop_exit:
	// Resize **map_data to the proper height
	{
		void *temp = reallocarray(map_data, map_height, sizeof(char *));
		if (temp == NULL)
		{
			PERR("failed to realloc map_data");
			goto l_exit;
		}
		map_data = (char **) temp;
	}

	// **map_data is completely read into memory properly by this point

	// If an error occurs after this point, it is non-recoverable
	err_code = ERR_NO_RECOVER;

	// Free old map data
	map_free(g_map.height, g_tile_map);
	map_free(g_map.height, g_ent_map);
	g_ent_map = NULL;
	
	// Allocate new space for the old maps
	g_tile_map = map_alloc(map_width, map_height, sizeof(TileId));
	if (g_tile_map == NULL)
	{
		PERR("failed to allocate mem for tile map");
		goto l_exit;
	}
	g_ent_map = map_alloc(map_width, map_height, sizeof(EntTile));
	if (g_ent_map == NULL)
	{
		PERR("failed to allocate mem for entity map");
		goto l_exit;
	}

	// Set the global values for map width and height
	g_map.width = map_width;
	g_map.height = map_height;

	// Update misc game systems
	
	// Update g_map
	strncpy(g_map.path, path, MAP_PATH_MAX);
	g_map.editing = editing;
	g_map.vr_list.len = 0;
	
	// Destroy leftover entities from last map
	ent_destroy_temp();

	// Scatter clouds across the screen
	ent_cloud_scatter();

	// Stop player from entering a door right away
	g_player.door_stop = true;

	// Update camera limits to reflect new map width and height
	cam_update_limits();

	// Last char read from the file
	int c;

	// Begin to read map options
	while ((c = fgetc(map_file)) == MAP_OPT_SYMBOL)
	{
		// Get the option name
		char option_str[MAP_OPTION_LEN];
		if (spdl_readstr(option_str, MAP_OPTION_LEN, ' ', map_file) == -1)
		{
			PERR("failed to read map option name");
			goto l_exit;
		}
		
		if (memcmp(option_str, "ot", 3) == 0)
		{
			// .ot
			// SET OUTSIDE TILE ID
			fscanf(map_file, "%c\n", (char *) &c);

			int ti;
			if ((ti = map_get_tile_id(c)) != -1)
				g_tile_outside = ti;
			else
				g_tile_outside = MAP_DEF_OT;
		}
		else if (memcmp(option_str, "d", 2) == 0)
		{
			// .d
			// LINK DOOR TO MAP PATH

			// Door id
			int did;

			int map_path_len = 0;
			fscanf(map_file, "%d ", &did);
			if (!ENT_DOOR_ID_IS_VALID(did))
			{
				PERR("d: invalid door id %d specified", did);
				goto l_skip_line;
			}
			while ((c = fgetc(map_file)) != '\n')
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
		else if (memcmp(option_str, "ss", 3) == 0)
		{
			// .ss
			// ENABLE/DISABLE CAMERA SCROLL STOP
			int scroll_stop;
			fscanf(map_file, "%d\n", &scroll_stop);
			g_cam.scroll_stop = (bool) scroll_stop;
			cam_update_limits();
		}
		else if (memcmp(option_str, "r", 2) == 0)
		{
			// .r
			// CREATE A VOID RECTANGLE

			// Don't read over the max number of void rectangles
			if (g_map.vr_list.len >= VOID_RECT_LIST_LEN)
			{
				PERR("max number of void rectangles read. ignoring this one");
				goto l_skip_line;
			}
			
			// Current void rectangle being created
			VoidRect *r = &g_map.vr_list.r[g_map.vr_list.len];

			// Get the dimensions of the rectangle
			if (fscanf(
				map_file,
				"%d %d %d %d ",
				&r->rect.y,
				&r->rect.x,
				&r->rect.h,
				&r->rect.w
			) != 4)
			{
				PERR("failed to read void rectangle dimensions");
				goto l_skip_line;
			}

			if (fgetc(map_file) == 'i')
			{
				// Get integer value
				if (fscanf(map_file, "%d\n", (int *) &r->value.i) == 0)
				{
					PERR("failed to read void rectangle integer value");
					goto l_skip_line;
				}
				r->value_is_str = false;
			}
			else
			{
				// Get string value
				if (spdl_readstr(
					r->value.s,
					VOID_RECT_STR_LEN,
					'\n',
					map_file
				) == -1)
				{
					PERR("failed to read void rectangle string value");
					goto l_skip_line;
				}
				r->value_is_str = true;
			}
			++g_map.vr_list.len;
		}
		else
		{
			// No option found
			PERR("unknown option \"%s\" found", option_str);

			// Move to the next line of input
		l_skip_line:
			while ((c = fgetc(map_file)) != '\n')
				;
		}
	}

	// All reading from the map file has finished
	if (fclose(map_file))
	{
		PERR("failed to close map file");
	}
	else
		map_file = NULL;
	
	// Create *ent_tile_data
	// This stores chars used to represent entity tiles
	ent_tile_data = map_alloc(g_map.width, g_map.height, sizeof(EntTileId));
	if (ent_tile_data == NULL)
	{
		PERR("failed to allocate mem for ent_tile_data");
		goto l_exit;
	}

	// Read **map_data
	// Copy tile data to **g_tile_map
	// Copy entity tile to **ent_tile_data
	for (int y = 0; y < g_map.height; ++y)
	{
		for (int x = 0; x < g_map.width; ++x)
		{
			// Current char read from the map
			c = map_data[y][x];

			int ti = map_get_tile_id(c);
			if (ti == -1)
			{
				int ei = map_get_ent_id(c);
				if (ei == -1)
				{
					PERR("no tile or entity found at (%d, %d)", x, y);
					g_tile_map[y][x] = TILE_AIR;
					ent_tile_data[y][x] = ENT_TILE_NONE;
				}
				else
				{
					// Entity found
					g_tile_map[y][x] = TILE_AIR;
					ent_tile_data[y][x] = ei;
				}
			}
			else
			{
				// Tile found
				g_tile_map[y][x] = ti;
				ent_tile_data[y][x] = ENT_TILE_NONE;
			}
		}
	}

	// The current data in **ent_tile_data will be sent to the collector, so a duplicate of that data is needed so that we can write changes to it when spawning entities that doesn't effect the data in the collector
	// For now, we just need to create space for the duplicate data
	EntTileId **new_ent_tile_data = map_alloc(map_width, map_height, sizeof(EntTileId));
	if (new_ent_tile_data == NULL)
	{
		PERR("failed to allocate mem for new_ent_tile_data");
		goto l_exit;
	}

	// Create a collector map data object to send to the collector
	ColMapData cmd = {
		.path = strndup(g_map.path, MAP_PATH_MAX),
		.width = map_width,
		.height = map_height,
		.map = ent_tile_data,
	};

	// Check if a memory error occured when duplicating g_map.path
	if (cmd.path == NULL)
	{
		PERR("failed to duplicate g_map.path");
		map_free(map_height, new_ent_tile_data);
		goto l_exit;
	}

	// Attempt to add the loaded map data to the collector
	ColMapIndex dup_index = col_add_map(&cmd);
	if (dup_index != -1)
	{
		// Map was already added
		
		// Free current ent_tile_data
		// It is no longer needed because we will be using the tile data from the collector instead
		map_free(map_height, ent_tile_data);

		// Use ent_tile_data from the collector
		g_col.active_index = dup_index;
	}
	else
	{
		// Map was newly added
		g_col.active_index = g_col.len - 1;
	}

	// Copy entity tile data from the collector to use to spawn entities
	map_copy(map_width, map_height, sizeof(EntTileId), new_ent_tile_data, g_col.data[g_col.active_index].map);
	ent_tile_data = new_ent_tile_data;

	// Read and write to **ent_tile_data
	// Call entity spawners
	// If editing a map, copy editable entity tile data to **g_ent_map
	
	// Spawn entities in void rectangles
	for (int i = 0; i < g_map.vr_list.len; ++i)
	{
		VoidRect *r = &g_map.vr_list.r[i];
		for (int y = r->rect.y; y < r->rect.y + r->rect.h; ++y)
		{
			for (int x = r->rect.x; x < r->rect.x + r->rect.w; ++x)
			{
				EntTileId ei = ent_tile_data[y][x];
				if (ei == ENT_TILE_NONE)
					continue;

				// An entity tile exists here
				
				// Spawn the entity with the void rectangle pointer value
				if ((g_ent_tile[ei].spawner)(x * TILE_SIZE, y * TILE_SIZE, &r->value))
				{
					PERR("entity tile spawner for entity id %d (%s) failed at (%d, %d)", ei, g_ent_tile[ei].name, x, y);
					PERR("the failed entity spawner was called with ptr=%p", (void *) &r->value);
				}
					
				// Add the entity to the entity tile map
				if (editing)
				{
					g_ent_map[y][x].active = true;
					g_ent_map[y][x].etid = ei;
				}

				// Overwrite the entity tile at the entity position so the entity isn't spawned again by the next loop through **ent_tile_data
				ent_tile_data[y][x] = ENT_TILE_NONE;
			}
		}
	}

	// Null value to pass to spawners so that they don't dereference a NULL pointer
	VoidRectInt null = 0;
	VoidRectInt *ptr_to_null = &null;

	// Spawn the remaining entities
	for (int y = 0; y < g_map.height; ++y)
	{
		for (int x = 0; x < g_map.width; ++x)
		{
			EntTileId ei = ent_tile_data[y][x];
			if (ei == ENT_TILE_NONE)
				continue;

			if ((g_ent_tile[ei].spawner)(x * TILE_SIZE, y * TILE_SIZE, ptr_to_null))
				PERR("entity tile spawner for entity id %d (%s) failed at (%d, %d)", ei, g_ent_tile[ei].name, x, y);

			if (editing)
			{
				g_ent_map[y][x].active = true;
				g_ent_map[y][x].etid = ei;
			}
		}
	}

	// Move the player to the door with the last id used
	EntDOOR *e = g_er[ENT_ID_DOOR]->e;
	for (EntDoorId i = 0; i < g_er[ENT_ID_DOOR]->len; i++)
	{
		if (e->did == g_ent_door_last_used)
		{
			g_player.b.x = e->b.x;
			g_player.b.y = e->b.y;
			break;
		}
		++e;
	}

	// Map loaded successfully
	err_code = ERR_NONE;

l_exit:
	// Free map_data and ent_tile_data and close map_file
	map_free(map_height, map_data);
	map_free(map_height, ent_tile_data);
	if (map_file != NULL)
		if (fclose(map_file))
			PERR("failed to close map file");
	return err_code;
}

// Saves a map to a text file, returns nonzero on error
int map_save_txt(char *path)
{
	// Getting the full path from the path argument
	char fullpath[RES_PATH_MAX];
	snprintf(fullpath, RES_PATH_MAX, DIR_MAP "/%s", path);

	// Open the map file
	FILE *map_file;
	if ((map_file = fopen(fullpath, "w")) == NULL)
	{
		PERR("failed to open map file \"%s\"", path);
		return 1;
	}

	// Write tiles to file
	for (int y = 0; y < g_map.height; y++)
	{
		for (int x = 0; x < g_map.width; x++)
		{
			if (g_ent_map[y][x].active)
			{
				// Write an entity
				fputc(g_ent_tile[g_ent_map[y][x].etid].map_char, map_file);
			}
			else
			{
				// Write a tile
				fputc(g_tile_md[g_tile_map[y][x]].map_char, map_file);
			}
		}
		fprintf(map_file, "\n");
	}

	// Write options to file

	// Outside tile id
	fputc(MAP_OPT_SYMBOL, map_file);
	fprintf(map_file, "ot %c\n", g_tile_md[g_tile_outside].map_char);

	// Door map paths
	for (int i = 0; i < ENT_DOOR_MAX; i++)
	{
		if (g_ent_door_map_path[i][0] != '\0')
		{
			fputc(MAP_OPT_SYMBOL, map_file);
			fprintf(map_file, "d %d %s\n", i, g_ent_door_map_path[i]);
		}
	}

	// Void rectangles
	for (int i = 0; i < g_map.vr_list.len; ++i)
	{
		VoidRect *r = &g_map.vr_list.r[i];
		fputc(MAP_OPT_SYMBOL, map_file);
		fprintf(map_file, "r %d %d %d %d ",
			r->rect.y,
			r->rect.x,
			r->rect.h,
			r->rect.w
		);
		if (r->value_is_str)
			fprintf(map_file, "s%s", r->value.s);
		else
			fprintf(map_file, "i%d", r->value.i);
		fputc('\n', map_file);
	}

	if (fclose(map_file))
		PERR("failed to close map file");
	return 0;
}

// Allocates and returns a pointer to map memory with size bytes for each index, returns NULL on error
void *map_alloc(int map_width, int map_height, size_t size)
{
	void **map_ptr;

	// Allocate mem for the y rows
	if ((map_ptr = calloc(map_height, sizeof(void *))) == NULL)
	{
		fprintf(stderr, "failed to allocate mem for map\n");
		return NULL;
	}

	// Allocate mem for the x columns
	for (int y = 0; y < map_height; ++y)
	{
		if ((map_ptr[y] = calloc(map_width, size)) == NULL)
		{
			fprintf(stderr, "failed to allocate mem for map\n");

			// Free all x tile id arrays allocated so far
			for (int yy = y - 1; yy >= 0; --yy)
				free(map_ptr[yy]);
			free(map_ptr);
			return NULL;
		}
	}

	return (void *) map_ptr;
}

// Frees map memory
void map_free(int map_height, void *map_ptr_param)
{
	// Use map pointer parameter as double pointer
	void **map_ptr = (void **) map_ptr_param;

	// Map never existed/was already freed
	if (map_ptr == NULL)
		return;
	
	// Free all x rows
	for (int y = 0; y < map_height; ++y)
		free(map_ptr[y]);
	free(map_ptr);
}

// Copies map data from *src to *dest
// For this to work, both *dest and *src must have the same width, height, and size
void map_copy(int map_width, int map_height, size_t size, void *dest_param, void *src_param)
{
	// Use map pointer parameters as double pointers
	void **dest = (void **) dest_param;
	void **src = (void **) src_param;

	// Copy rows
	for (int i = 0; i < map_height; ++i)
		memcpy(dest[i], src[i], map_width * size);
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

// Returns a pointer to a new void rectangle in g_map.vr_list
// Returns NULL on error
VoidRect *map_vr_list_add(void)
{
	if (g_map.vr_list.len >= VOID_RECT_LIST_LEN)
		return NULL;
	return &g_map.vr_list.r[g_map.vr_list.len++];
}

// Removes the void rectangle and index index of g_map.vr_list.r
void map_vr_list_del(int index)
{
	--g_map.vr_list.len;
	if (index != g_map.vr_list.len)
		g_map.vr_list.r[index] = g_map.vr_list.r[g_map.vr_list.len];
}

// Returns the tile id of a character, -1 if no tile is matched
int map_get_tile_id(char c)
{
	for (int i = 0; i < TILE_MAX; i++)
		if (g_tile_md[i].map_char == c)
			return i;
	return -1;
}

// Returns the entity tile id of a character, -1 if no entity is matched
int map_get_ent_id(char c)
{
	for (int i = 0; i < ENT_TILE_MAX; i++)
		if (g_ent_tile[i].map_char == c)
			return i;
	return -1;
}
