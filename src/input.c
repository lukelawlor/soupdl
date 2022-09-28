/*
 * input.c contains the key_state pointer for handling keyboard input.
 */

#include <SDL2/SDL.h>

#include "input.h"

// Pointer to SDL's key_state array
const uint8_t *g_key_state;
