/*
 * fileio.h contains miscellaneous functions for file input and output.
 */

#ifndef	FILEIO_H
#define	FILEIO_H

#include <stdio.h>

// Writes chars from *stream into the string that starts at *dest
// Stops when a newline is found, and doesn't include the newline in the string
// Returns nonzero on failure
int spdl_getline(char *dest, size_t len_max, FILE *stream);

#endif
