/*
 * fileio.h contains miscellaneous functions for file input and output.
 */

#ifndef	FILEIO_H
#define	FILEIO_H

#include <stdio.h>

// Writes chars from *stream (including \0) into *dest
// Stops when the delim character is found, and doesn't include the delim in the string
// Returns the number of chars written to *dest minus 1, or -1 on error
int spdl_readstr(char *dest, const size_t len_max, const int delim, FILE *stream);

#endif
