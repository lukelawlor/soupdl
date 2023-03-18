/*
 * input.h contains variables and functions for getting player input.
 */

#ifndef	INPUT_H
#define	INPUT_H

#include <stdint.h>
#include <stdio.h>

// Pointer to SDL's key_state array
extern const uint8_t *g_key_state;

// Asks the player to input a string
// The string input is stored in *dest, which should be a pointer to a char array with size len_max
// Returns the length of the string (without \0) or -1 on error
int spdl_input_string(char *dest, size_t len_max, char *prompt);

#endif
