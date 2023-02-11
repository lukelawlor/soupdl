/*
 * fileio.c contains miscellaneous functions for file input and output.
 */

#include "fileio.h"

// Writes chars from *stream into the array that starts at *dest with size len_max
// Stops when a newline is found, and doesn't include the newline in the string
// Returns nonzero on failure
int spdl_getline(char *dest, size_t len_max, FILE *stream)
{
	// Index of *dest to access next
	int i = 0;

	// Current character being processed
	int c;

	// Reading loop
	do
	{
		switch (c = fgetc(stream))
		{
		case '\n':
			dest[i] = '\0';
			break;
		case EOF:
			// Reading error
			return 1;
			break;
		default:
			dest[i] = (char) c;
		}
		if (++i == len_max)
		{
			// The max amount of chars was read and the end of the string was not found
			return 1;
		}
	} while (c != '\n');

	// Success
	return 0;
}
