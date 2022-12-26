/*
 * turret.c contains functions for manipulating turret entities.
 */

#include <math.h>

#include <SDL2/SDL.h>

#include "../video.h"
#include "../texture.h"
#include "../camera.h"
#include "../sound.h"
#include "../timestep.h"
#include "../random.h"
#include "../tile/data.h"

#include "c_sprite.h"
#include "entity.h"

#include "player.h"
#include "evilball.h"
#include "turret.h"

#define	FIRE_TICK_RESET	32
#define	EVILBALL_SPD	4

EntTURRET *ent_new_TURRET(int x, int y)
{
	static int fire_offset = 0;
	fire_offset += 4;

	ENT_NEW(TURRET);
	e->x = x;
	e->y = y;
	e->fire_tick = FIRE_TICK_RESET + fire_offset;
	return e;
}

void ent_update_TURRET(EntTURRET *e)
{
	e->spr++;
	if (e->spr > SPR_TURRET_NORM4)
		e->spr = 0;
	
	if ((e->fire_tick -= g_ts) <= 0)
	{
		snd_play(snd_shoot);
		e->fire_tick = FIRE_TICK_RESET;

		// Center position of player
		const int pcx = g_player.b.x + 16;
		const int pcy = g_player.b.y + 16;

		// Center position of turret
		const int cx = e->x + 16;
		const int cy = e->y + 16;

		// x and y distance between the center of the turret and player
		int y = pcy - cy;
		int x = pcx - cx;

		// Angle measurement from turret to player
		double dir = atan((double) y / x);
		if (pcx < cx)
			dir += M_PI;

		// Fireball speeds
		float hsp = cos(dir) * EVILBALL_SPD;
		float vsp = sin(dir) * EVILBALL_SPD;

		ent_new_EVILBALL(e->x + TILE_SIZE / 2, e->y + TILE_SIZE / 2, hsp, vsp);
	}
}

void ent_draw_TURRET(EntTURRET *e)
{
	// Drawing turret body
	{
		const SDL_Rect *srect = &g_spr_turret[SPR_TURRET_BASE];
		const SDL_Rect drect = {e->x + g_cam.xshift, e->y + g_cam.yshift, TILE_SIZE, TILE_SIZE};
		SDL_RenderCopy(g_renderer, tex_turret, srect, &drect);
	}
	// Drawing turret face
	{
		const SDL_Rect *srect = &g_spr_turret[e->spr];
		const SDL_Rect drect = {e->x + g_cam.xshift + 6, e->y + g_cam.yshift + 8, SPR_TURRET_W, SPR_TURRET_H};
		SDL_RenderCopy(g_renderer, tex_turret, srect, &drect);
	}
}

void ent_destroy_TURRET(EntTURRET *e)
{
	ENT_DEL_MARK(e);
}
