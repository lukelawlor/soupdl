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

#define	FIRE_TICK_RESET	48.0
#define	EVILBALL_SPD	4.0f

EntTURRET *ent_new_TURRET(int x, int y)
{
	static int fire_offset = 0;
	fire_offset += 4;

	ENT_NEW(TURRET);
	e->x = x;
	e->y = y;
	e->fire_tick = FIRE_TICK_RESET + fire_offset;
	e->fire_spr_frames = 0.0f;
	return e;
}

void ent_update_TURRET(EntTURRET *e)
{
	e->spr++;
	if (e->spr > SPR_TURRET_NORM4)
		e->spr = 0;
	
	if (e->fire_spr_frames > 0.0f)
		e->fire_spr_frames -= g_ts;

	{
		// Center position of player
		const int pcx = g_player.b.x + 16;
		const int pcy = g_player.b.y + 16;

		// Center position of turret
		const int cx = e->x + 16;
		const int cy = e->y + 16;

		// x and y distance between the center of the turret and player
		const int y = pcy - cy;
		const int x = pcx - cx;

		// Calculate angle from turret to player
		e->dir = atan((double) y / x);
		if (pcx < cx)
			e->dir += M_PI;
	}
		
	if ((e->fire_tick -= g_ts) <= 0.0)
	{
		snd_play(snd_shoot);
		e->fire_tick = FIRE_TICK_RESET;
		e->fire_spr_frames = 8.0f;

		// Fireball speeds
		const float hsp = cos(e->dir) * EVILBALL_SPD;
		const float vsp = sin(e->dir) * EVILBALL_SPD;

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
		const SDL_Rect *srect = &g_spr_turret[e->spr + (e->fire_spr_frames > 0.0f ? 4 : 0)];
		const SDL_Rect drect = {
			e->x + 16 - SPR_TURRET_W / 2 + g_cam.xshift + cos(e->dir) * 6.0,
			e->y + 16 - SPR_TURRET_H / 2 + g_cam.yshift + sin(e->dir) * 3.0,
			SPR_TURRET_W,
			SPR_TURRET_H
		};
		SDL_RenderCopyEx(g_renderer, tex_turret, srect, &drect, 0, NULL, g_player.b.x > e->x ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
	}
}

void ent_destroy_TURRET(EntTURRET *e)
{
	ENT_DEL_MARK(e);
}
