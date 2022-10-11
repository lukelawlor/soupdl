/*
 * main.c contains the game loop.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>	// For rand()
#include <time.h>	// For setting random seed

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "timestep.h"
#include "random.h"
#include "init.h"
#include "video.h"
#include "texture.h"
#include "sound.h"
#include "camera.h"
#include "hud.h"
#include "tile/data.h"
#include "tile/draw.h"
#include "tile/outside.h"
#include "entity/all.h"

static inline void game_loop(void);

int main(int argc, char **argv)
{
	// Initialize everything needed to start the game loop
	if (game_init_all())
		return EXIT_FAILURE;
	hud_init();
	
	// Set random seed
	srand(time(NULL));

	// Initialize test objects
	ent_item_init();

	// Load the map
	tile_map_load_txt("cool.map");
	for (int i = 0; i < 40; i++)
		ent_item_new(rand() % (g_room_width * TILE_SIZE), rand() % (g_room_height * TILE_SIZE), ITEM_TRUMPET);

	// game loop and exit
	game_loop();
	game_quit_all();
	return EXIT_SUCCESS;
}

static void game_loop(void)
{
	// Event used for polling
	SDL_Event e;

	// Ints to keep track of game tick
	uint32_t tick_this_frame;
	uint32_t tick_last_frame = 0;

	bool game_running = true;

	while (game_running)
	{
		// Set frame start ticks
		tick_this_frame = SDL_GetTicks();
		g_ts = (double) (tick_this_frame - tick_last_frame) / 16;
		tick_last_frame = tick_this_frame;

		// Handle SDL events
		while (SDL_PollEvent(&e) != 0)
		{
			// User requests quit
			switch (e.type)
			{
			case SDL_QUIT:
				game_running = false;
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_1:
					ent_ragdoll_new(g_player.x, g_player.y, (spdl_random() - 128) / 128.0f, -6, RAGDOLL_EGG);
					break;
				case SDLK_2:
					Mix_PlayChannel(-1, snd_shoot, 0);
					break;
				case SDLK_3:
					Mix_PlayChannel(-1, snd_splode, 0);
					break;
				case SDLK_4:
					Mix_PlayChannel(-1, snd_bubble, 0);
					break;
				case SDLK_5:
					tile_map_load_txt("cool.map");
					break;
				case SDLK_6:
					tile_map_load_txt("small.map");
					break;
				case SDLK_7:
					ent_particle_new(g_player.x, g_player.y, PTCL_BUBBLE);
					break;
				case SDLK_8:
					ent_particle_new(g_player.x, g_player.y, PTCL_FLAME);
					break;
				case SDLK_9:
					ent_particle_new(g_player.x, g_player.y, PTCL_STAR);
					break;
				case SDLK_q:
					game_running = false;
					break;
				}
				if (g_player.hp > 0)
					ent_player_keydown(e.key.keysym.sym);
				break;
			case SDL_WINDOWEVENT:
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					g_screen_width = e.window.data1;
					g_screen_height = e.window.data2;
					cam_update_limits();
					SDL_RenderPresent(g_renderer);
					break;
				case SDL_WINDOWEVENT_EXPOSED:
					SDL_RenderPresent(g_renderer);
					break;
				}
				break;
			}
		}

		// Update test objects
		if (g_player.hp > 0)
			ent_player_update();
		cam_update_shifts();
		for (int i = 0; i < ENT_LIST_MAX; i++)
		{
			EntFireball *fireball;
			if ((fireball = ent_fireball + i)->d.exists)
				ent_fireball_update(fireball);
			EntParticle *particle;
			if ((particle = ent_particle + i)->d.exists)
				ent_particle_update(particle);
			EntRagdoll *ragdoll;
			if ((ragdoll = ent_ragdoll + i)->d.exists)
				ent_ragdoll_update(ragdoll);
			EntEvilegg *evilegg;
			if ((evilegg = ent_evilegg + i)->d.exists)
				ent_evilegg_update(evilegg);
		}

		// Clear the screen
		SDL_SetRenderDrawColor(g_renderer, 180, 255, 230, 255);
		SDL_RenderClear(g_renderer);

		// Draw all tiles
		tile_draw_all();
		tile_draw_outside_all();

		// Render test objects
		if (g_player.hp > 0)
			ent_player_draw();
		for (int i = 0; i < ENT_LIST_MAX; i++)
		{
			EntItem *item;
			if ((item = ent_item + i)->d.exists)
				ent_item_draw(item);
			EntFireball *fireball;
			if ((fireball = ent_fireball + i)->d.exists)
				ent_fireball_draw(fireball);
			EntParticle *particle;
			if ((particle = ent_particle + i)->d.exists)
				ent_particle_draw(particle);
			EntRagdoll *ragdoll;
			if ((ragdoll = ent_ragdoll + i)->d.exists)
				ent_ragdoll_draw(ragdoll);
			EntEvilegg *evilegg;
			if ((evilegg = ent_evilegg + i)->d.exists)
				ent_evilegg_draw(evilegg);
		}

		// Draw HUD
		hud_draw_all();

		// Render what's currently on the screen
		SDL_RenderPresent(g_renderer);

	#ifndef	VSYNC
		// Manually delay the program if VSYNC is disabled
		uint32_t tick_diff = SDL_GetTicks() - tick_this_frame;
		if (tick_diff < FRAME_TICKS)
			SDL_Delay(FRAME_TICKS - tick_diff);
	#endif
	}
}
