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
#include "editor/editor.h"
#include "editor/draw.h"

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

// The standard game loop
static inline void game_loop(void);

// The map editor game loop
static inline void editor_loop(void);

int main(int argc, char **argv)
{
	// First map to load
	char *map_start;

	// True when the first map is going to be edited
	bool ed_init = false;

	// Reading command line arguments
	if (argc == 2)
	{
		// Map to edit is (presumably) passed, start the map editor
		map_start = g_maped_file = argv[1];
		g_game_state = GAMESTATE_EDITOR;
		ed_init = true;
	}
	else
	{
		// No arguments passed, start the game normally
		map_start = "cool.map";
		g_game_state = GAMESTATE_INGAME;
	}
	
	// Initialize everything needed to start the game loop
	if (game_init_all())
		return EXIT_FAILURE;

	// Initializes misc systems that depend on game textures being loaded
	hud_init();
	ent_item_init();
	
	// Set random seed
	srand(time(NULL));

	// Load the map
	if (map_load_txt(map_start, ed_init))
	{
		game_quit_all();
		return EXIT_FAILURE;
	}

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
			case SDLK_e:
				g_game_state = GAMESTATE_EDITOR;
				if (maped_init())
					g_game_state = GAMESTATE_QUIT;
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
	static MapEd maped = {
		.tile = TILE_AIR,
		.ent = ENT_ID_PLAYER,
		.tile_type = MAPED_TILE_TILE,
		.state = MAPED_STATE_NONE
	};

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
			case SDLK_e:
				g_game_state = GAMESTATE_INGAME;
				break;
			case SDLK_q:
				g_game_state = GAMESTATE_QUIT;
				break;
			}
			maped_handle_keydown(&maped, g_sdlev.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			maped_handle_mbdown(&maped, g_sdlev.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			maped_handle_mbup(&maped, g_sdlev.button.button);
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

	// Place tiles in editor
	if (maped.state != MAPED_STATE_NONE)
		maped_tile(&maped);

	// Update test objects
	cam_update_position();
	cam_update_shifts();

	// Clear the screen
	SDL_SetRenderDrawColor(g_renderer, 180, 255, 230, 255);
	SDL_RenderClear(g_renderer);

	// Draw all tiles
	tile_draw_all();
	tile_draw_outside_all();
	maped_draw_entmap();

	maped_draw_status(&maped);

	// Render what's currently on the screen
	SDL_RenderPresent(g_renderer);

#ifndef	VSYNC
	// Manually delay the program if VSYNC is disabled
	uint32_t tick_diff = SDL_GetTicks() - g_tick_this_frame;
	if (tick_diff < FRAME_TICKS)
		SDL_Delay(FRAME_TICKS - tick_diff);
#endif
}
