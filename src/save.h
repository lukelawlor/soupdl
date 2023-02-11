/*
 * save.h contains functions for loading and saving player progress in the game.
 */

#ifndef	SPDL_SAVE_H
#define	SPDL_SAVE_H

#include "error.h"

// Saves the game
ErrCode spdl_save(void);

// Loads the game
ErrCode spdl_load(void);

#endif
