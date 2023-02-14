/*
 * fileio.c contains miscellaneous functions for file input and output.
 */

#include "fileio.h"

// Writes chars from *stream into the string that starts at *dest
// Stops when the delim character is found, and doesn't include the delim in the string
// Returns nonzero on failure
int spdl_readstr(char *dest, size_t len_max, int delim, FILE *stream)
{
	// Index of *dest to access next
	int i = 0;

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
			return 1;
		}
		dest[i] = c;
		if (++i == (int) len_max)
		{
			// The max amount of chars was read and the end of the string was not found
			return 1;
		}
	}

	// Success
	return 0;
}
