/*
 * void_rect.h contains the types for void rectangles.
 *
 * See map.h for documentation of void rectangles.
 */

#ifndef	VOID_RECT_H
#define	VOID_RECT_H

#include <SDL2/SDL.h>	// For SDL_Rect

#include <stdint.h>

// Length of void rectangle value.s member variable
#define	VOID_RECT_STR_LEN	20

typedef int8_t VoidRectInt;
typedef char VoidRectChar;

typedef struct{
	// Dimensions of the rectangle as map tile coordinates
	SDL_Rect rect;

	union {
		VoidRectChar s[VOID_RECT_STR_LEN];
		VoidRectInt i;
	} value;
	bool value_is_str : 1;
} VoidRect;

#endif
