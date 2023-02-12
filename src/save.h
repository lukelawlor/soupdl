/*
 * save.h contains functions for loading and saving player progress in the game.
 *
 * The format for a save file is as follows:
 * 	(string) map filename in DIR_SAVE (defined in dir.h)
 * 	(int) player x coordinate
 * 	(int) player y coordinate
 * 	(int) player fireballs
 * 	(int) player max hp
 * 	(int) player current hp
 *
 * Here's an example save file:
 * 	cool.map
 * 	36
 * 	90
 * 	4
 */

#ifndef	SPDL_SAVE_H
#define	SPDL_SAVE_H

#include "error.h"

// Saves the game
ErrCode spdl_save(void);

// Loads the game
ErrCode spdl_load(void);

#endif
