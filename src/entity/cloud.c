/*
 * cloud.c contains functions for manipulating cloud entities.
 */

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "../random.h"
#include "../timestep.h"
#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "../tile/data.h"
#include "entity.h"
#include "cloud.h"

// Maximum width and height a cloud sprite can have in pixels
#define	CLOUD_MAX_WIDTH		110
#define	CLOUD_MAX_HEIGHT	50

EntCLOUD *ent_new_CLOUD(int x, int y, float hsp)
{
	ENT_NEW(CLOUD);
	e->x = x;
	e->y = y;
	e->hsp = hsp;
	e->id = spdl_random() / 130;
	return e;
}

void ent_update_CLOUD(EntCLOUD *e)
{
	int min_x = g_cam.x - g_screen_width / 2 - CLOUD_MAX_WIDTH;
	int max_x = g_cam.x + g_screen_width / 2 + CLOUD_MAX_WIDTH;
	int min_y = g_cam.y - g_screen_height / 2 - CLOUD_MAX_HEIGHT;
	int max_y = g_cam.y + g_screen_height / 2 + CLOUD_MAX_HEIGHT;
	e->x += e->hsp * g_ts;

	// True if the cloud has just wrapped around the map
	bool wrapped = false;

	if (e->x > max_x)
	{
		e->x = min_x;
		wrapped = true;
	}
	else if (e->x < min_x)
	{
		e->x = max_x;
		wrapped = true;
	}
	else if (e->y > max_y)
		e->y = min_y;
	else if (e->y < min_y)
		e->y = max_y;

	if (wrapped)
	{
		e->y = ((g_screen_height) * (spdl_random() / 255.0f)) + g_cam.y - g_screen_height / 2;
		e->hsp = ENT_CLOUD_GET_RANDOM_HSP();
		e->id = spdl_random() / 130;
	}
}

void ent_draw_CLOUD(EntCLOUD *e)
{
	SDL_Rect srect;
	if (e->id == ENT_CLOUD_BIG)
		srect = (SDL_Rect) {0, 0, 102, 50};
	else
		srect = (SDL_Rect) {103, 0, 42, 23};
	SDL_Rect drect = {e->x + g_cam.xshift, e->y + g_cam.yshift, srect.w, srect.h};
	SDL_RenderCopy(g_renderer, tex_cloud, &srect, &drect);
}

void ent_destroy_CLOUD(EntCLOUD *e)
{
	ENT_DEL(e);
}
