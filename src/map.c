/*
 * map.c contains functions for loading and saving maps.
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>	// For memcmp() and strncpy()

#include "dir.h"
#include "error.h"
#include "camera.h"
#include "tile/data.h"
#include "entity/id.h"
#include "entity/tile.h"
#include "entity/all.h"
#include "entity/cloud.h"
#include "editor/editor.h"
#include "util/string.h"
#include "fileio.h"
#include "map.h"

// Default outside tile value
#define	MAP_DEF_OT	TILE_LIME

// Length of string used to store current map option being read
#define	MAP_OPTION_LEN	5

// Info about the current map loaded
MapInfo g_map;

// Returns the tile id of a character, -1 if no tile is matched
static inline int get_tile_id(char c);

// Returns the entity id of a character, -1 if no entity is matched
static inline int get_ent_id(char c);

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

// XXX: UNFINISHED FUNCTION
ErrCode map_load_txt_new(char *path, bool editing)
{
	// This will be used as the return code for the function if an error occurs
	ErrCode err_code = ERR_RECOVER;

	// Getting the full path from the path argument
	char fullpath[RES_PATH_MAX];
	snprintf(fullpath, RES_PATH_MAX, DIR_MAP "/%s", path);
	FILE *map_file;
	if ((map_file = fopen(fullpath, "r")) == NULL)
	{
		PERR("Failed to load text map file \"%s\"", fullpath);
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
	char **map_data = malloc(MAP_HEIGHT_MAX * sizeof(char *));
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
			goto l_exit_fmcf;
		}
			
		switch (c)
		{
		case '.':
			// End of map & start of options found
			goto l_heightloop_exit;
		case EOF:
		case '\n':
			// Reading error
			PERR("failed to read a char at the start of a map line");
			goto l_exit_fmcf;
		}

		// Allocate mem for a new line
		map_data[map_height] = malloc(map_width * sizeof(char));

		// Read a new line in
		if (map_read_line(map_data[map_height], map_width, map_file))
		{
			PERR("failed to read a map line");
			goto l_exit_fmcf;
		}
	}

	// Since the loop exited normally, MAP_HEIGHT_MAX was read
	// This will cause lines beyond the max height to not be read, which could be an error
	PERR("maximum height for a map was read in");

l_heightloop_exit:
	// Resize *map_data to the proper height
	{
		void *temp = reallocarray(map_data, map_height, sizeof(char *));
		if (temp == NULL)
		{
			PERR("failed to realloc map_data");
			goto l_exit_fmcf;
		}
		map_data = (char **) temp;
	}

	// *map_data is completely read into memory properly by this point

	// If an error occurs after this point, it is non-recoverable
	err_code = ERR_NO_RECOVER;

	// Free old map data
	map_free(g_map.height, g_tile_map);
	map_free(g_map.height, g_ent_map);

	// Allocate new space for the old maps
	g_tile_map = map_alloc(map_width, map_height, sizeof(TileId));
	if (g_tile_map == NULL)
	{
		PERR("failed to allocate mem for tile map");
		return err_code;
	}
	g_ent_map = map_alloc(map_width, map_height, sizeof(EntTile));
	if (g_ent_map == NULL)
	{
		PERR("failed to allocate mem for entity map");
		return err_code;
	}

	// Set the global values for map width and height
	g_map.width = map_width;
	g_map.height = map_height;

	// Last char read from the file
	int c;

	// Begin to read map options
	while ((c = fgetc(map_file)) == '.')
	{
		// Get the option name
		char option_str[MAP_OPTION_LEN];
		if (spdl_readstr(option_str, MAP_OPTION_LEN, ' ', map_file) == -1)
		{
			PERR("failed to read map option name");
			return err_code;
		}
		
		if (memcmp(option_str, "ot", 3) == 0)
		{
			// .ot
			// SET OUTSIDE TILE ID
			fscanf(map_file, "%c\n", (char *) &c);

			int ti;
			if ((ti = get_tile_id(c)) != -1)
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
		PERR("failed to close map file");
	
	// Test printing the map to stderr
	for (int i = 0; i < g_map.height; ++i)
	{
		for (int j = 0; j < g_map.width; ++j)
			fputc(map_data[i][j], stderr);
		fputc('\n', stderr);
	}

	// Create *ent_tile_data
	// This stores chars used to represent entity tiles
	char **ent_tile_data = map_alloc(g_map.width, g_map.height, sizeof(char));

	// Read *map_data
	// Copy tile data to **g_tile_map
	// Copy entity tile to **ent_tile_data
	for (int y = 0; y < g_map.height; ++y)
	{
		for (int x = 0; x < g_map.width; ++x)
		{
			// Current char read from the map
			c = map_data[y][x];

			int ti = get_tile_id(c);
			if (ti == -1)
			{
				if (get_ent_id(c) == -1)
				{
					// Entity found
					g_tile_map[y][x] = TILE_AIR;
					ent_tile_data[y][x] = c;
				}
				else
				{
					PERR("no tile or entity found at (%d, %d)", x, y);
					g_tile_map[y][x] = TILE_AIR;
					ent_tile_data[y][x] = g_tile_md[TILE_AIR].map_char;
				}
			}
			else
			{
				// Tile found
				g_tile_map[y][x] = ti;
				ent_tile_data[y][x] = g_tile_md[TILE_AIR].map_char;
			}
		}
	}

	// Map loaded successfully
	return ERR_NONE;

l_exit_fmcf:
	// Free map_data and close map_file
	// Return a recoverable error code
	map_free(map_height, map_data);
	if (fclose(map_file))
		PERR("failed to close map file");
	return err_code;
#if 0
	// Current character being read from the file
	int c;

	// True if the player is spawned at a player spawn point entity tile
	bool player_spawned = false;

	for (int y = 0; y < g_map.height; y++)
	{
		for (int x = 0; x < g_map.width; x++)
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
				
				// Special events for entity tiles
				switch (id)
				{
				case ENT_TILE_PLAYER:
					if (g_map.editing)
						player_spawned = true;
					break;
				default:
					break;
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
		if (spdl_readstr(option_str, MAP_OPTION_LEN, ' ', mapfile) == -1)
		{
			PERR("failed to read map option name");
			return ERR_NO_RECOVER;
		}
		
		if (memcmp(option_str, "ot", 3) == 0)
		{
			// Set outside tile id
			fscanf(mapfile, "%c\n", (char *) &c);

			int ti;
			if ((ti = get_tile_id(c)) != -1)
				g_tile_outside = ti;
			else
				g_tile_outside = MAP_DEF_OT;
		}
		else if (memcmp(option_str, "d", 2) == 0)
		{
			// Link door to map file

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
		else if (memcmp(option_str, "ss", 3) == 0)
		{
			// Enable/disable camera scroll stop
			int scroll_stop;
			fscanf(mapfile, "%d\n", &scroll_stop);
			g_cam.scroll_stop = scroll_stop;
			cam_update_limits();
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
	if (!player_spawned)
	{
		EntDOOR *e = g_er[ENT_ID_DOOR]->e;
		for (EntDoorId i = 0; i < g_er[ENT_ID_DOOR]->len; i++)
		{
			if (e->did == g_ent_door_last_used)
			{
				g_player.b.x = e->b.x;
				g_player.b.y = e->b.y;
				break;
			}
			e++;
		}
	}
	return ERR_NONE;
#endif
}

// Loads a map from a text file
// The editing member variable from the map parameter is true when the map is being opened for editing, make sure maped_init (from editor/editor.h) has been called before this is indicated
ErrCode map_load_txt(char *path, bool editing)
{
	// Getting the full path from the path argument
	char fullpath[RES_PATH_MAX];
	snprintf(fullpath, RES_PATH_MAX, DIR_MAP "/%s", path);
	FILE *mapfile;
	if ((mapfile = fopen(fullpath, "r")) == NULL)
	{
		fprintf(stderr, "Failed to load text map file \"%s\"\n", fullpath);
		return ERR_RECOVER;
	}

	// Start changing the map

	// Destroy entities from last map
	ent_destroy_temp();

	// Scatter clouds
	ent_cloud_scatter();

	// Set g_map
	strncpy(g_map.path, path, MAP_PATH_MAX);
	g_map.editing = editing;

	// Stop player from entering a door right away
	g_player.door_stop = true;

	// Free any old data in g_tile_map if it exists
	map_free(g_map.height, g_tile_map);
	map_free(g_map.height, g_ent_map);
	g_ent_map = NULL;

	// Get map width and height
	fscanf(mapfile, "%dx%d\n", &g_map.width, &g_map.height);

	// Load the entity map
	if (editing)
	{
		if (maped_init())
		{
			PERR("failed to initialize map editor");
			return ERR_NO_RECOVER;
		}
	}

	// Update camera limits to reflect new width and height
	cam_update_limits();
	
	// Allocate mem for the entire map
	if ((g_tile_map = map_alloc(g_map.width, g_map.height, sizeof(TileId))) == NULL)
		return ERR_NO_RECOVER;

	// Current character being read from the file
	int c;

	// True if the player is spawned at a player spawn point entity tile
	bool player_spawned = false;

	for (int y = 0; y < g_map.height; ++y)
	{
		for (int x = 0; x < g_map.width; ++x)
		{
			c = fgetc(mapfile);

			// Pointer to tile id being modified
			TileId *ti = &g_tile_map[y][x];
			
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
				
				// Special events for entity tiles
				switch (id)
				{
				case ENT_TILE_PLAYER:
					if (g_map.editing)
						player_spawned = true;
					break;
				default:
					break;
				}

				
				// If the map is opened for editing, add the entity id to the map
				if (editing)
				{
					g_ent_map[y][x].active = true;
					g_ent_map[y][x].etid = id;
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
		if (spdl_readstr(option_str, MAP_OPTION_LEN, ' ', mapfile) == -1)
		{
			PERR("failed to read map option name");
			return ERR_NO_RECOVER;
		}
		
		if (memcmp(option_str, "ot", 3) == 0)
		{
			// Set outside tile id
			fscanf(mapfile, "%c\n", (char *) &c);

			int ti;
			if ((ti = get_tile_id(c)) != -1)
				g_tile_outside = ti;
			else
				g_tile_outside = MAP_DEF_OT;
		}
		else if (memcmp(option_str, "d", 2) == 0)
		{
			// Link door to map file

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
		else if (memcmp(option_str, "ss", 3) == 0)
		{
			// Enable/disable camera scroll stop
			int scroll_stop;
			fscanf(mapfile, "%d\n", &scroll_stop);
			g_cam.scroll_stop = scroll_stop;
			cam_update_limits();
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
	if (!player_spawned)
	{
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
	}
	return ERR_NONE;
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
	fprintf(mapfile, "%dx%d\n", g_map.width, g_map.height);

	// Write tiles to file
	for (int y = 0; y < g_map.height; y++)
	{
		for (int x = 0; x < g_map.width; x++)
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

	// Outside tile id
	fprintf(mapfile, ".ot %c\n", g_tile_md[g_tile_outside].map_char);

	// Door map paths
	for (int i = 0; i < ENT_DOOR_MAX; i++)
		if (g_ent_door_map_path[i][0] != '\0')
			fprintf(mapfile, ".d %d %s\n", i, g_ent_door_map_path[i]);

	fclose(mapfile);

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
