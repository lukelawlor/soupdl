/*
 * init.h contains functions for initializing and quitting systems needed to start the game loop.
 */

#ifndef	INIT_H
#define	INIT_H

/*
 * Initializes everything needed to start the game loop
 *
 * Returns nonzero on error
 */
int game_init_all(void);

// Frees everything allocated in game_init_all
void game_quit_all(void);

#endif
