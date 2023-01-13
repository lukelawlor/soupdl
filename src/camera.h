/*
 * camera.h contains the GameCamera type and the extern declaration for the game camera.
 */

#ifndef	CAMERA_H
#define	CAMERA_H

#include <stdbool.h>

typedef struct{
	// Position
	int x;
	int y;

	// Number of pixels to shift anything drawn on the screen by horizontally
	int xshift;

	// Number of pixels to shift anything drawn on the screen by vertically
	int yshift;

	// True if scroll stopping is enabled
	bool scroll_stop;

	// True if the camera should stop at the left and right sides of the map
	bool xstop;

	// True if the camera should stop at the top and bottom sides of the map
	bool ystop;
} GameCamera;

// Global game camera
extern GameCamera g_cam;

// Updates the camera's xstop and ystop based on the screen dimensions
void cam_update_limits(void);

// Updates the camera's xshift and yshift based on its position & x/y stop
void cam_update_shifts(void);

// Uses arrow keys to move the camera
void cam_update_position(void);

// Sets the dimensions for all visible tiles of any type to be drawn to the screen
void cam_get_tile_dimensions(int *left, int *right, int *top, int *bottom);

#endif
