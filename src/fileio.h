/*
 * fileio.h contains miscellaneous functions for file input and output.
 */

#ifndef	FILEIO_H
#define	FILEIO_H

#include <stdio.h>

// Writes chars from *stream into the string that starts at *dest
// Stops when the delim character is found, and doesn't include the delim in the string
// Returns nonzero on failure
int spdl_readstr(char *dest, size_t len_max, int delim, FILE *stream);

#endif
