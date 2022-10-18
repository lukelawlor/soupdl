/*
 * map.h contains functions for loading and saving maps.
 *
 * A map consists of tile data and entity spawn points. Currently they can only be saved as text files though I plan on implementing a binary file format for them.
 */

#ifndef	MAP_H
#define	MAP_H

// Loads a map from a text file, returns nonzero on error
int map_load_txt(char *path);

// Saves a map to a text file, returns nonzero on error
int map_save_txt(char *path);

#endif
