/*
 * save.h contains functions for loading and saving player progress in the game.
 *
 * The format for a save file is as follows:
 * 	<string: map filename in DIR_SAVE (defined in dir.h)>
 * 	<int: player x coordinate>
 * 	<int: player y coordinate>
 * 	<int: player fireballs>
 * 	<int: player max hp>
 * 	<int: player current hp>
 * 	<int: player coin count>
 *	<collector data>
 *
 * Collector data is data stored in the collector (see in collector.h). It is stored like so:
 * 	<string: map filename in DIR_SAVE>
 *	<int: map height>x<int: map width>
 *	<map: entity tile data stored as ascii chars>
 *
 * The above sequence of data repeats for every map stored in the collector.
 *
 * Here's an example save file:
 * 	cool.map
 * 	36
 * 	90
 * 	4
 * 	8
 * 	4
 * 	1
 * 	sick.map
 * 	8x6
 * 	NNNNNN
 * 	NNNNNN
 * 	NNNN+N
 * 	NNN+++
 * 	NNNN+N
 * 	NNNNNN
 * 	NNNNNN
 * 	NNNNNN
 */

#ifndef	SPDL_SAVE_H
#define	SPDL_SAVE_H

#include "error.h"

// Saves the game
ErrCode spdl_save(void);

// Loads the game
ErrCode spdl_load(void);

#endif
