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

#ifdef	__EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "barrier.h"
#include "camera.h"
#include "editor/editor.h"
#include "editor/draw.h"
#include "entity/all.h"
#include "error.h"
#include "font.h"
#include "hud.h"
#include "init.h"
#include "input.h"
#include "map.h"
#include "random.h"
#include "sound.h"
#include "texture.h"
#include "tile/data.h"
#include "tile/draw.h"
#include "tile/outside.h"
#include "timestep.h"
#include "util/string.h"
#include "video.h"

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
static inline void game_loop_standard(void);

// The map editor game loop
static inline void game_loop_editor(void);

#ifdef	__EMSCRIPTEN__
// Emscripten main loop function
static void game_loop_emscripten(void *arg);
#endif

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
		map_start = "title.map";
		goto l_normal_startup;
	case 2:
		// Game is launched with the map from argv[1]
		map_start = argv[1];
		g_game_state = GAMESTATE_EDITOR;
		ed_init = true;
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
		// More than 2 arguments have been provided, assume a new map is being created
		g_game_state = GAMESTATE_EDITOR;

		if (memcmp(argv[1], "--new", 6) != 0)
		{
			PERR("unknown argument \"%s\" provided, expected \"--new\"", argv[1]);
			return EXIT_FAILURE;
		}

		// Update g_map
		g_map.editing = true;
		strncpy(g_map.path, argv[3], MAP_PATH_MAX);
		if (g_map.path[MAP_PATH_MAX - 1] != '\0')
		{
			PERR("error: map path longer than MAP_PATH_MAX (" STR(MAP_PATH_MAX) " chars)");
			return EXIT_FAILURE;
		}

		// Get map dimensions
		g_map.width = 0;
		g_map.height = 0;
		sscanf(argv[2], "%dx%d", &g_map.width, &g_map.height);
		if (g_map.width <= 0)
		{
			PERR("failed to extract new map width from command line arguments");
			return EXIT_FAILURE;
		}
		if (g_map.height <= 0)
		{
			PERR("failed to extract new map height from command line arguments");
			return EXIT_FAILURE;
		}

		// Init game
		if (game_init_all())
			return EXIT_FAILURE;

		// Instead of loading a map, map memory is manually allocated to fit the map dimensions
		if (maped_init())
		{
			game_quit_all();
			return EXIT_FAILURE;
		}
		if ((g_tile_map = (TileId **) map_alloc(g_map.width, g_map.height, sizeof(TileId))) == NULL)
		{
			game_quit_all();
			return EXIT_FAILURE;
		}
		break;
	default:
		PERR("wrong number of arguments provided");
		return EXIT_FAILURE;
		break;
	}

	// Set random seed
	srand(time(NULL));

	// Spawn clouds
	ent_cloud_update_count();

	// Game loops
#ifdef	__EMSCRIPTEN__
	// Emscripten way of handling game loops
	
	// Fps of game loop
	// If this is -1, the game loop's updates will match the monitor's refresh rate
	int fps;

	if (g_vsync)
		fps = -1;
	else
		fps = g_no_vsync_refresh_rate;
	emscripten_set_main_loop_arg(game_loop_emscripten, NULL, fps, true);
#else
	// Normal way of handling game loops
	while (g_game_state != GAMESTATE_QUIT)
	{
		while (g_game_state == GAMESTATE_INGAME)
			game_loop_standard();
		while (g_game_state == GAMESTATE_EDITOR)
			game_loop_editor();
	}
#endif

	game_quit_all();
	return EXIT_SUCCESS;
}

#ifdef	__EMSCRIPTEN__
// Emscripten main loop function
static void game_loop_emscripten(void *arg)
{
	switch (g_game_state)
	{
	case GAMESTATE_INGAME:
		game_loop_standard();
		break;
	case GAMESTATE_EDITOR:
		game_loop_editor();
		break;
	case GAMESTATE_QUIT:
		// Display a screen to show that the game has exited
		SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
		SDL_RenderClear(g_renderer);
		font_draw_text("the end", 0, 0);
		SDL_RenderPresent(g_renderer);

		// Stop the loop
		emscripten_cancel_main_loop();
		break;
	default:
		PERR("unknown game state");
		abort();
	}
}
#endif

// The standard game loop
static void game_loop_standard(void)
{
	//static double timestep_reset = 1.0;

	// Set frame start ticks
	g_tick_this_frame = SDL_GetTicks();
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
			// Test actions
			case SDLK_1:
				screen_scale(1, 1);
				break;
			case SDLK_2:
				screen_scale(2, 2);
				break;
			case SDLK_3:
				screen_scale(g_screen_xscale - 0.1f, g_screen_yscale - 0.1f);
				break;
			case SDLK_4:
				screen_scale(g_screen_xscale + 0.1f, g_screen_yscale + 0.1f);
				break;
			/*
			case SDLK_5:
				timestep_reset -= 0.1;
				goto l_show;
			case SDLK_6:
				timestep_reset += 0.1;
				goto l_show;
			case SDLK_7:
				timestep_reset *= timestep_reset;
			l_show:
				fprintf(stderr, "%lf\n", timestep_reset);
				break;
			*/
			}
			ent_player_keydown(g_sdlev.key.keysym.sym);
			break;
		case SDL_WINDOWEVENT:
			switch (g_sdlev.window.event)
			{
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				screen_update_dimensions();
				break;
			}
			break;
		}
	}

	// Update test objects
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
	ENT_UPDATE(COOLEGG);
	barrier_handle_check_requests();

	// Clear the screen
	SDL_SetRenderDrawColor(g_renderer, 180, 255, 230, 255);
	SDL_RenderClear(g_renderer);

	ENT_DRAW(CLOUD);

	// Draw all tiles
	tile_draw_all();
	tile_draw_outside_all();

	// Render test objects
	ENT_DRAW(DOOR);
	ENT_DRAW(TURRET);
	ENT_DRAW(ITEM);
	ENT_DRAW(PARTICLE);
	ENT_DRAW(RAGDOLL);
	ENT_DRAW(GROUNDGUY);
	ENT_DRAW(SLIDEGUY);
	ENT_DRAW(EVILBALL);
	ENT_DRAW(FIREBALL);
	ENT_DRAW(SAVEBIRD);
	ENT_DRAW(BARRIER);
	ENT_DRAW(COOLEGG);
	if (g_player.hp > 0)
		ent_player_draw();

	// Draw HUD
	hud_draw_all();

	// Render what's currently on the screen
	SDL_RenderPresent(g_renderer);

#ifndef	__EMSCRIPTEN__
	if (!g_vsync)
	{
		// Manually delay the program if VSYNC is disabled
		// Not needed on emscripten because we control the framerate when calling emscripten_set_main_loop_arg()
		uint32_t tick_diff = SDL_GetTicks() - g_tick_this_frame;
		if (tick_diff < g_no_vsync_frame_ticks)
			SDL_Delay(g_no_vsync_frame_ticks - tick_diff);
	}
#endif
}

// The map editor game loop
static inline void game_loop_editor(void)
{
	static MapEd maped = {
		.tile = {.tid = TILE_AIR},
		.tile_type = MAPED_TILE_TILE,
		.void_rect = {NULL, {0, 0, 0, 0}, 0, 0, 0},
		.state = MAPED_STATE_NONE,
		.w = 1,
		.h = 1,
		.alt = false,
	};

	// Set frame start ticks
	g_tick_this_frame = SDL_GetTicks();
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
				if (maped_init())
				{
					PERR("failed to initialize map editor");
					break;
				}
				g_game_state = GAMESTATE_INGAME;
				break;
			case SDLK_q:
				g_game_state = GAMESTATE_QUIT;
				break;
			}
			maped_handle_keydown(&maped, g_sdlev.key.keysym.sym);
			break;
		case SDL_KEYUP:
			maped_handle_keyup(&maped, g_sdlev.key.keysym.sym);
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
				screen_update_dimensions();
				break;
			}
			break;
		}
	}

	// Place tiles in editor
	switch (maped.state)
	{
	case MAPED_STATE_TILING:
	case MAPED_STATE_ERASING:
		maped_tile(&maped);
		break;
	case MAPED_STATE_VR_MOVING:
		maped_vr_move(&maped);
		break;
	case MAPED_STATE_VR_RESIZING:
		maped_vr_resize(&maped);
		break;
	default:
		break;
	}

	// Update test objects
	cam_update_position();
	cam_update_shifts();

	// Clear the screen
	SDL_SetRenderDrawColor(g_renderer, 180, 255, 230, 255);
	SDL_RenderClear(g_renderer);

	// Draw all tiles
	//ENT_DRAW(CLOUD);
	tile_draw_all();
	tile_draw_outside_all();
	maped_draw_all(&maped);

	// Render what's currently on the screen
	SDL_RenderPresent(g_renderer);

#ifndef	__EMSCRIPTEN__
	if (!g_vsync)
	{
		// Manually delay the program if VSYNC is disabled
		// Not needed on emscripten because we control the framerate when calling emscripten_set_main_loop_arg()
		uint32_t tick_diff = SDL_GetTicks() - g_tick_this_frame;
		if (tick_diff < g_no_vsync_frame_ticks)
			SDL_Delay(g_no_vsync_frame_ticks - tick_diff);
	}
#endif
}
