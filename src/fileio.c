/*
 * fileio.c contains miscellaneous functions for file input and output.
 */

#include "fileio.h"

// Writes chars from *stream (including \0) into *dest
// Stops when the delim character is found, and doesn't include the delim in the string
// Returns the number of chars written to *dest minus 1, or -1 on error
int spdl_readstr(char *dest, const size_t len_max, const int delim, FILE *stream)
{
	// Index of *dest to access next
	int i = 0;

	// Stop reading when i reaches this value
	const int i_stop = len_max - 1;

	// Current character being processed
	int c;

	// Reading loop
	for (;;)
	{
		c = fgetc(stream);
		if (c == delim)
		{
			dest[i] = '\0';
			break;
		}
		if (c == EOF)
		{
			// Reading error
			return -1;
		}
		dest[i] = c;
		if (++i == i_stop)
		{
			// The max amount of chars was read and the end of the string was not found
			dest[i] = '\0';
			return -1;
		}
	}

	// Success
	return i;
}
