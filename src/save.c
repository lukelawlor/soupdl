/*
 * save.c contains functions for loading and saving player progress in the game.
 */


#include <stdio.h>
#include <stdbool.h>

#include "dir.h"
#include "error.h"
#include "entity/player.h"	// For g_player
#include "fileio.h"		// For spdl_getline()
#include "map.h"		// For map_load_txt() and g_map
#include "save.h"

#include "entity/tile.h"
#include "collector.h"

#define	SAVE_PATH	DIR_SAVE "/test.sav"

// Loads the game
ErrCode spdl_load(void)
{
	ErrCode err_code = ERR_NO_RECOVER;

	// Don't load a game if a map is being edited
	if (g_map.editing)
	{
		PERR("failed to load game: map is being edited");
		return ERR_RECOVER;
	}

	// Open the save file
	const char *savefile_path = SAVE_PATH;
	FILE *savefile = fopen(savefile_path, "r");
	if (savefile == NULL)
	{
		PERR("failed to open save file \"%s\"", savefile_path);
		return ERR_RECOVER;
	}
	
	// Data to get from the save file

	// Map to load
	char savefile_map[MAP_PATH_MAX];

	// X position of the player
	int savefile_x = -1;

	// Y position of the player
	int savefile_y = -1;

	// Fireballs the player has
	int savefile_fireballs = -1;

	// Max hp of the player
	int savefile_maxhp = -1;

	// Current hp of the player
	int savefile_hp = -1;

	// Current coin count of the player
	int savefile_coins = -1;

	// Read map name from save file
	if (spdl_readstr(savefile_map, MAP_PATH_MAX, '\n', savefile) == -1)
	{
		PERR("error reading map name from save file \"%s\"", savefile_path);
		return ERR_RECOVER;
	}

	// Read numbers from the save file
	fscanf(
		savefile,
		"%d\n%d\n%d\n%d\n%d\n%d\n",
		&savefile_x,
		&savefile_y,
		&savefile_fireballs,
		&savefile_maxhp,
		&savefile_hp,
		&savefile_coins
	);

	// Check for data reading errors
	if (
		savefile_x == -1 ||
		savefile_y == -1 ||
		savefile_fireballs == -1 || 
		savefile_maxhp == -1 ||
		savefile_hp == -1 ||
		savefile_coins == -1
		)
	{
		PERR("failed to read numbers from save file \"%s\"", savefile_path);
		return ERR_RECOVER;
	}

	// Buffer for reading in collector map data objects
	ColMapData cmd = {
		.path = NULL,
		.width = -1,
		.height = -1,
		.map = NULL,
	};

	// Reset collector
	col_free();

	// Read collector data
	for (;;)
	{
		int c = fgetc(savefile);
		if (c != EOF)
			ungetc(c, savefile);
		else
		{
			// End of collector data found
			break;
		}

		// Allocate space for map name
		cmd.path = calloc(MAP_PATH_MAX, sizeof(char));
		if (cmd.path == NULL)
		{
			PERR("failed to allocate mem for cmd.path");
			goto l_exit;
		}

		// Read map name
		if (spdl_readstr(cmd.path, MAP_PATH_MAX, '\n', savefile) == -1)
		{
			PERR("failed to read in cmd.path");
			goto l_exit;
		}

		// Read map dimensions
		fscanf(
			savefile,
			"%dx%d\n",
			&cmd.height,
			&cmd.width
		);

		if (
			cmd.height == -1 ||
			cmd.width == -1
			)
		{
			PERR("failed to read cmd dimensions");
			goto l_exit;
		}

		// Allocate space for map
		cmd.map = map_alloc(cmd.width, cmd.height, sizeof(EntTileId));
		if (cmd.map == NULL)
		{
			PERR("failed to allocate mem for cmd.map");
			goto l_exit;
		}

		// Read entity tile data
		for (int y = 0; y < cmd.height; ++y)
		{
			for (int x = 0; x < cmd.width; ++x)
			{
				c = fgetc(savefile);
				if (c == EOF)
				{
					PERR("failed to read entity tile data");
					goto l_exit;
				}
				int ei = map_get_ent_id(c);
				if (ei == -1)
					cmd.map[y][x] = ENT_TILE_NONE;
				else
					cmd.map[y][x] = ei;
			}

			// Skip newline
			if (fgetc(savefile) == EOF)
			{
				PERR("failed to read entity tile data");
				goto l_exit;
			}
		}

		// Add the cmd to the collector
		if (col_add_map(&cmd) != -1)
		{
			PERR("failed to add cmd");
		}

		// Reset pointers in cmd to NULL so they aren't freed
		cmd.path = NULL;
		cmd.map = NULL;
	}

	// Close the save file
	if (fclose(savefile) != 0)
	{
		PERR("failed to close save file \"%s\"", savefile_path);
		return ERR_NO_RECOVER;
	}

	// Attempt to load save file map
	{
		err_code = map_load_txt(savefile_map, false);
		if (err_code != ERR_NONE)
			goto l_exit;
	}

	// Apply save file data to the running game
	g_player.b.x = savefile_x;
	g_player.b.y = savefile_y;
	g_player.has_trumpet = (g_player.trumpet_shots = g_player.trumpet_shots_reset = savefile_fireballs) > 0;
	g_player.maxhp = savefile_maxhp;
	g_player.hp = savefile_hp;
	g_player.coins = savefile_coins;

	// Reset player variables
	g_player.b.hsp = 0;
	g_player.b.vsp = 0;

	err_code = ERR_NONE;
	PINF("load game successful");
l_exit:
	free(cmd.path);
	map_free(cmd.height, cmd.map);
	return err_code;
}

// Saves the game
ErrCode spdl_save(void)
{
	ErrCode err_code = ERR_NONE;

	// Open save file
	const char *savefile_path = SAVE_PATH;
	FILE *savefile = fopen(savefile_path, "w");
	if (savefile == NULL)
	{
		PERR("failed to open save file \"%s\"", savefile_path);
		return ERR_RECOVER;
	}

	// Write data to file
	int err = fprintf(
		savefile,
		"%s\n%d\n%d\n%d\n%d\n%d\n%d\n",
		g_map.path,
		(int) g_player.b.x,
		(int) g_player.b.y,
		g_player.trumpet_shots_reset,
		g_player.maxhp,
		g_player.hp,
		g_player.coins
	);
	if (err < 0)
	{
		PERR("failed to write data to save file \"%s\"", savefile_path);
		err_code = ERR_RECOVER;
	}

	// Write collector data
	for (int i = 0; i < g_col.len; ++i)
	{
		ColMapData *cmd = &g_col.data[i];

		// Write map path and dimensions
		err = fprintf(
			savefile,
			"%s\n%dx%d\n",
			cmd->path,
			cmd->height,
			cmd->width
		);
		if (err < 0)
		{
			PERR("failed to write data to save file \"%s\"", savefile_path);
			err_code = ERR_RECOVER;
			break;
		}

		// Write entity tile data
		for (int y = 0; y < cmd->height; ++y)
		{
			for (int x = 0; x < cmd->width; ++x)
			{
				if (fputc(g_ent_tile[cmd->map[y][x]].map_char, savefile) == EOF)
				{
					PERR("failed to write collector data to save file \"%s\"", savefile_path);
					err_code = ERR_RECOVER;
					goto l_collectorloop_exit;
				}
			}
			if (fputc('\n', savefile) == EOF)
			{
				PERR("failed to write collector data to save file \"%s\"", savefile_path);
				err_code = ERR_RECOVER;
				goto l_collectorloop_exit;
			}
		}
	}
l_collectorloop_exit:

	// Close file
	if (fclose(savefile) == EOF)
	{
		PERR("failed to close save file \"%s\"", savefile_path);
		return ERR_NO_RECOVER;
	}

	if (err_code == ERR_NONE)
		PINF("save game successful");
	return err_code;
}
