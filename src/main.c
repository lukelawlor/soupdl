/*
 * main.c handles command line arguments and the game loops.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>	// For rand()
#include <time.h>	// For setting random seed
#include <string.h>	// For strncmp()

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "error.h"
#include "timestep.h"
#include "random.h"
#include "input.h"
#include "init.h"
#include "video.h"
#include "texture.h"
#include "sound.h"
#include "camera.h"
#include "font.h"
#include "hud.h"
#include "map.h"
#include "tile/data.h"
#include "tile/draw.h"
#include "tile/outside.h"
#include "entity/all.h"
#include "editor/editor.h"
#include "editor/draw.h"

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
	// Map to load upon game startup
	char *map_start;

	// True if the first map will be edited
	bool ed_init = false;

	switch (argc)
	{
	case 1:
		// Game is launched like normal
		map_start = "cool.map";
		goto l_normal_startup;
	case 2:
		map_start = g_maped_file = argv[1];
		g_game_state = GAMESTATE_EDITOR;
		ed_init = true;
		// Fallthrough
	l_normal_startup:
		if (game_init_all())
			return EXIT_FAILURE;
		if (map_load_txt(map_start, ed_init))
		{
			game_quit_all();
			return EXIT_FAILURE;
		}
		break;
	case 4:
		g_game_state = GAMESTATE_EDITOR;

		// More than 2 arguments have been provided, assume a new map is being created
		if (memcmp(argv[1], "--new", 6) != 0)
		{
			PERR();
			fprintf(stderr, "unknown argument \"%s\" provided, expected \"--new\"\n", argv[1]);
			return EXIT_FAILURE;
		}

		// Get map dimensions
		g_room_width = 0;
		g_room_height = 0;
		sscanf(argv[2], "%dx%d", &g_room_width, &g_room_height);
		if (g_room_width <= 0)
		{
			PERR();
			fprintf(stderr, "failed to extract new map width from command line arguments\n");
			return EXIT_FAILURE;
		}
		if (g_room_height <= 0)
		{
			PERR();
			fprintf(stderr, "failed to extract new map height from command line arguments\n");
			return EXIT_FAILURE;
		}

		// Get new map filename
		g_maped_file = argv[3];
		if (game_init_all())
			return EXIT_FAILURE;

		// Instead of loading a map, map memory is manually allocated to fit the map dimensions
		if (maped_init())
		{
			game_quit_all();
			return EXIT_FAILURE;
		}
		if ((g_tile_map = (TileId **) map_alloc(sizeof(TileId))) == NULL)
		{
			game_quit_all();
			return EXIT_FAILURE;
		}

		// Fill tile map with zeros
		for (int x = 0; x < g_room_width; x++)
			for (int y = 0; y < g_room_height; y++)
				g_tile_map[x][y] = TILE_AIR;

		break;
	default:
		PERR();
		fprintf(stderr, "wrong number of arguments provided\n");
		return EXIT_FAILURE;
		break;
	}

	// Set random seed
	srand(time(NULL));

	for (int i = 0; i < g_er[ENT_ID_CLOUD]->len_max; i++)
		ent_new_CLOUD(g_cam.x, g_cam.y, ENT_CLOUD_GET_RANDOM_HSP());

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
#ifndef	NDEBUG
	g_ts = 1.0;
#else
	g_ts = (double) (g_tick_this_frame - g_tick_last_frame) / 16.6666666666666;
#endif
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
				break;
			}
			break;
		}
	}

	// Update test objects
	if (g_player.hp > 0)
		ent_player_update();
	cam_update_shifts();
	ENT_UPDATE(FIREBALL);
	ENT_UPDATE(EVILBALL);
	ENT_UPDATE(PARTICLE);
	ENT_UPDATE(RAGDOLL);
	ENT_UPDATE(GROUNDGUY);
	ENT_UPDATE(CLOUD);
	ENT_UPDATE(SLIDEGUY);
	ENT_UPDATE(TURRET);

	// Clear the screen
	SDL_SetRenderDrawColor(g_renderer, 180, 255, 230, 255);
	SDL_RenderClear(g_renderer);

	ENT_DRAW(CLOUD);

	// Draw all tiles
	tile_draw_all();
	tile_draw_outside_all();

	// Render test objects
	ENT_DRAW(ITEM);
	ENT_DRAW(PARTICLE);
	ENT_DRAW(RAGDOLL);
	ENT_DRAW(GROUNDGUY);
	ENT_DRAW(SLIDEGUY);
	ENT_DRAW(TURRET);
	ENT_DRAW(EVILBALL);
	ENT_DRAW(FIREBALL);
	if (g_player.hp > 0)
		ent_player_draw();

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
		.tile = {.tile = TILE_AIR},
		.tile_type = MAPED_TILE_TILE,
		.state = MAPED_STATE_NONE,
		.w = 1,
		.h = 1,
	};

	// Set frame start ticks
	g_tick_this_frame = SDL_GetTicks();
	g_ts = (double) (g_tick_this_frame - g_tick_last_frame) / 16.6666666666666;
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
	ENT_DRAW(CLOUD);
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
