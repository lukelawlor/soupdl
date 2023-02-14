/*
 * input.c contains the key_state pointer for handling keyboard input.
 */

#include <stdint.h>
#include <stdio.h>

#include "fileio.h"
#include "input.h"

// Pointer to SDL's key_state array
const uint8_t *g_key_state;

// Asks the player to input a string
// The string input is stored in *dest, which should be a pointer to a char array with size len_max
void spdl_input_string(char *dest, size_t len_max, char *prompt)
{
	fprintf(stderr, "soupdl: input: %s: (max %d chars)\n", prompt, (int) (len_max - 1));
	spdl_readstr(dest, len_max, '\n', stdin);
}
