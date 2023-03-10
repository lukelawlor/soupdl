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

#define	SAVE_PATH	DIR_SAVE "/test.sav"

// Loads the game
ErrCode spdl_load(void)
{
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
	if (spdl_readstr(savefile_map, MAP_PATH_MAX, '\n', savefile))
	{
		PERR("error reading map name from save file \"%s\"", savefile_path);
		return ERR_RECOVER;
	}

	// Read numbers from the save file
	fscanf(
		savefile,
		"%d\n%d\n%d\n%d\n%d\n%d",
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

	// Close the save file
	if (fclose(savefile) != 0)
	{
		PERR("failed to close save file \"%s\"", savefile_path);
		return ERR_NO_RECOVER;
	}

	// Attempt to load save file map
	{
		ErrCode err = map_load_txt(savefile_map, false);
		if (err != ERR_NONE)
			return err;
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

	PINF("load game successful");
	return ERR_NONE;
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
		"%s\n%d\n%d\n%d\n%d\n%d\n%d",
		g_map,
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

	// Close file
	if (fclose(savefile) == EOF)
	{
		PERR("failed to close save file \"%s\"", savefile_path);
		return ERR_NO_RECOVER;
	}

	PINF("save game successful");
	return err_code;
}
