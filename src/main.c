/*
 * main.c handles command line arguments and the game loops.
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
#include "error.h"
#include "random.h"
#include "input.h"
#include "init.h"
#include "video.h"
#include "texture.h"
#include "sound.h"
#include "camera.h"
#include "hud.h"
#include "map.h"
#include "tile/data.h"
#include "tile/draw.h"
#include "tile/outside.h"
#include "entity/all.h"

// Macros for updating and drawing lists of entities
#define	ENT_UPDATE(x)	if ((ent_##x + i)->d.exists) \
					ent_##x##_update(ent_##x + i)
#define	ENT_DRAW(x)	if ((ent_##x + i)->d.exists) \
					ent_##x##_draw(ent_##x + i)

// Game states
typedef enum{
	GAMESTATE_INGAME,
	GAMESTATE_EDITOR,
	GAMESTATE_QUIT
} GameState;

static GameState g_game_state;

// Event for handling input
static SDL_Event g_sdlev;

// Game ticks for handling timestep
static uint32_t g_tick_last_frame = 0;
static uint32_t g_tick_this_frame = 0;

// Map file being edited
static char *g_ed_filename = NULL;

// The standard game loop
static inline void game_loop(void);

// The map editor game loop
static inline void editor_loop(void);

int main(int argc, char **argv)
{
	// Reading command line arguments
	if (argc == 2)
	{
		g_ed_filename = argv[1];
		g_game_state = GAMESTATE_EDITOR;
	}
	else
		g_game_state = GAMESTATE_INGAME;
	// Initialize everything needed to start the game loop
	if (game_init_all())
		return EXIT_FAILURE;
	hud_init();
	
	// Set random seed
	srand(time(NULL));

	// Initialize test objects
	ent_item_init();

	// Load the map
	map_load_txt("cool.map");
	for (int i = 0; i < 40; i++)
		ent_item_new(rand() % (g_room_width * TILE_SIZE), rand() % (g_room_height * TILE_SIZE), ITEM_TRUMPET);
	
	// Game loops
	while (g_game_state != GAMESTATE_QUIT)
	{
		while (g_game_state == GAMESTATE_INGAME)
			game_loop();
		while (g_game_state == GAMESTATE_EDITOR)
			editor_loop();
	}

	game_quit_all();
	return EXIT_SUCCESS;
}

// The standard game loop
static void game_loop(void)
{
	// Set frame start ticks
	g_tick_this_frame = SDL_GetTicks();
	g_ts = (double) (g_tick_this_frame - g_tick_last_frame) / 16;
	g_tick_last_frame = g_tick_this_frame;

	// Handle SDL events
	while (SDL_PollEvent(&g_sdlev) != 0)
	{
		// User requests quit
		switch (g_sdlev.type)
		{
		case SDL_QUIT:
			g_game_state = GAMESTATE_QUIT;
			break;
		case SDL_KEYDOWN:
			switch (g_sdlev.key.keysym.sym)
			{
			case SDLK_3:
				g_game_state = GAMESTATE_EDITOR;
				break;
			case SDLK_q:
				g_game_state = GAMESTATE_QUIT;
				break;
			}
			if (g_player.hp > 0)
				ent_player_keydown(g_sdlev.key.keysym.sym);
			break;
		case SDL_WINDOWEVENT:
			switch (g_sdlev.window.event)
			{
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				g_screen_width = g_sdlev.window.data1;
				g_screen_height = g_sdlev.window.data2;
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
		ENT_UPDATE(fireball);
		ENT_UPDATE(particle);
		ENT_UPDATE(ragdoll);
		ENT_UPDATE(evilegg);
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
		ENT_DRAW(item);
		ENT_DRAW(fireball);
		ENT_DRAW(particle);
		ENT_DRAW(ragdoll);
		ENT_DRAW(evilegg);
	}

	// Draw HUD
	hud_draw_all();

	// Render what's currently on the screen
	SDL_RenderPresent(g_renderer);

#ifndef	VSYNC
	// Manually delay the program if VSYNC is disabled
	uint32_t tick_diff = SDL_GetTicks() - g_tick_this_frame;
	if (tick_diff < FRAME_TICKS)
		SDL_Delay(FRAME_TICKS - tick_diff);
#endif
}

// The map editor game loop
static inline void editor_loop(void)
{
	static TileId ed_tile = TILE_AIR;
	static bool ed_tiling = false;

	// Set frame start ticks
	g_tick_this_frame = SDL_GetTicks();
	g_ts = (double) (g_tick_this_frame - g_tick_last_frame) / 16;
	g_tick_last_frame = g_tick_this_frame;

	// Handle SDL events
	while (SDL_PollEvent(&g_sdlev) != 0)
	{
		// User requests quit
		switch (g_sdlev.type)
		{
		case SDL_QUIT:
			g_game_state = GAMESTATE_QUIT;
			break;
		case SDL_KEYDOWN:
			switch (g_sdlev.key.keysym.sym)
			{
			case SDLK_3:
				g_game_state = GAMESTATE_INGAME;
				break;
			case SDLK_q:
				g_game_state = GAMESTATE_QUIT;
				break;
			case SDLK_e:
				if (ed_tile != TILE_AIR)
					ed_tile--;
				break;
			case SDLK_r:
				if (++ed_tile >= TILE_MAX)
					ed_tile = TILE_MAX - 1;
				break;
			case SDLK_p:
				// Try to save map
				if (g_ed_filename == NULL)
				{
					// No filename provided
					PERR("map save fail: no filename provided");
					Mix_PlayChannel(-1, snd_splode, 0);
				}
				else
				{
					map_save_txt(g_ed_filename);
				}
			}
			break;
		case SDL_WINDOWEVENT:
			switch (g_sdlev.window.event)
			{
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				g_screen_width = g_sdlev.window.data1;
				g_screen_height = g_sdlev.window.data2;
				cam_update_limits();
				SDL_RenderPresent(g_renderer);
				break;
			case SDL_WINDOWEVENT_EXPOSED:
				SDL_RenderPresent(g_renderer);
				break;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (g_sdlev.button.button == SDL_BUTTON_LEFT)
				ed_tiling = true;
			break;
		case SDL_MOUSEBUTTONUP:
			if (g_sdlev.button.button == SDL_BUTTON_LEFT)
				ed_tiling = false;
			break;
		}
	}

	// Place tiles in editor
	if (ed_tiling)
	{
		int mx, my;
		SDL_GetMouseState(&mx, &my);
		int cx = (mx - g_cam.xshift) / TILE_SIZE;
		int cy = (my - g_cam.yshift) / TILE_SIZE;
		if (	cx >= 0 && cx < g_room_width &&
			cy >= 0 && cy < g_room_height
			)
			g_tile_map[cx][cy] = ed_tile;
	}

	// Update test objects
	cam_update_position();
	cam_update_shifts();

	// Clear the screen
	SDL_SetRenderDrawColor(g_renderer, 180, 255, 230, 255);
	SDL_RenderClear(g_renderer);

	// Draw all tiles
	tile_draw_all();
	tile_draw_outside_all();

	// Draw selected tile at camera position
	{
		SDL_Rect srect = {g_tile_property[ed_tile].spoint.x, g_tile_property[ed_tile].spoint.y, 32, 32};
		SDL_Rect drect = {g_cam.x + g_cam.xshift - 16, g_cam.y + g_cam.yshift - 16, 32, 32};
		SDL_RenderCopy(g_renderer, tex_tileset, &srect, &drect);
	}

	// Draw player heart at camera position
	{
		SDL_Rect srect = {0, 0, 16, 16};
		SDL_Rect drect = {g_cam.x + g_cam.xshift - 8, g_cam.y + g_cam.yshift - 8, 16, 16};
		SDL_RenderCopy(g_renderer, tex_heart, &srect, &drect);
	}

	// Render what's currently on the screen
	SDL_RenderPresent(g_renderer);

#ifndef	VSYNC
	// Manually delay the program if VSYNC is disabled
	uint32_t tick_diff = SDL_GetTicks() - g_tick_this_frame;
	if (tick_diff < FRAME_TICKS)
		SDL_Delay(FRAME_TICKS - tick_diff);
#endif
}
