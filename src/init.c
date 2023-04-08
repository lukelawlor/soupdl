/*
 * init.c contains functions for initializing components of the game.
 */

#include <assert.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "collector.h"	// For col_free()
#include "dir.h"
#include "entity/item.h"
#include "entity/tile.h"
#include "error.h"
#include "input.h"
#include "map.h"
#include "sound.h"
#include "texture.h"
#include "video.h"

// Initialize SDL and its subsystems, create the game window and renderer, and set the game's window's icon
// Returns nonzero on error
static int game_init_sdl(void);

// Frees everything allocated in game_init_sdl
static void game_quit_sdl(void);

// Initialize SDL and its subsystems, create the game window and renderer, and set the game's window's icon
// Returns nonzero on error
static int game_init_sdl(void)
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		PERR("failed to initialize SDL. SDL Error: %s", SDL_GetError());
		return 1;
	}

	// Getting window and renderer
#ifdef	VSYNC
	const int renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
#else
	const int renderer_flags = SDL_RENDERER_ACCELERATED;
#endif

	// Default window dimensions
	g_screen_width = 1200;
	g_screen_height = 800;

	// Creating the window and renderer
	if ((g_window = SDL_CreateWindow("SoupDL 06", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, g_screen_width, g_screen_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)) == NULL)
	{
		PERR("failed to create window. SDL Error: %s", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	if ((g_renderer = SDL_CreateRenderer(g_window, -1, renderer_flags)) == NULL)
	{
		PERR("failed to create renderer. SDL Error: %s", SDL_GetError());
		SDL_DestroyWindow(g_window);
		SDL_Quit();
		return 1;
	}
	
	// Set renderer draw color
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);

	// Initialize SDL_image
	const int img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(img_flags) & img_flags))
	{
		PERR("failed ot initialize SDL_image. SDL Error: %s", IMG_GetError());
		SDL_DestroyRenderer(g_renderer);
		SDL_DestroyWindow(g_window);
		SDL_Quit();
		return 1;
	}

	// Initialize SDL_mixer
	const int mix_flags = MIX_INIT_MOD;
	if (!(Mix_Init(mix_flags) & mix_flags))
	{
		PERR("failed to initialize SDL_mixer. SDL Error: %s", Mix_GetError());
		SDL_DestroyRenderer(g_renderer);
		SDL_DestroyWindow(g_window);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	if (Mix_OpenAudio(G_MIX_SAMPLE_RATE, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		PERR("failed to open mixer audio. SDL Error: %s", Mix_GetError());
		SDL_DestroyRenderer(g_renderer);
		SDL_DestroyWindow(g_window);
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	// Loading game window icon
	SDL_Surface *surf;
	if ((surf = IMG_Load(DIR_GFX "/cakico.png")) == NULL)
	{
		PERR("failed to load window icon at \"res/cakico.png\". SDL Error: %s", IMG_GetError());
		SDL_DestroyRenderer(g_renderer);
		SDL_DestroyWindow(g_window);
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
		return 1;
	}
	SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 0, 0, 0));

	// Setting game window icon
	SDL_SetWindowIcon(g_window, surf);
	SDL_FreeSurface(surf);

	// Getting keyboard state
	g_key_state = SDL_GetKeyboardState(NULL);

	return 0;
}

// Frees everything allocated in game_init_sdl
static void game_quit_sdl(void)
{
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

// Initializes everything needed to start the game loop
// Returns nonzero on error
int game_init_all(void)
{
	// Initializing systems
	if (game_init_sdl())
		return 1;
	if (tex_load_all())
	{
		game_quit_sdl();
		return 1;
	}
	if (snd_load_all())
	{
		tex_free_all();
		game_quit_sdl();
		return 1;
	}
	if (ent_root_array_init())
	{
		snd_free_all();
		tex_free_all();
		game_quit_sdl();
		return 1;
	}

	// Initialize misc systems that depend on game textures being loaded
	ent_tile_init();
	ent_item_init();

	// Asserts (some depend on the init calls from above to work)
	assert(map_assert_dupchars());

	return 0;
}

// Frees everything allocated in game_init_all
void game_quit_all(void)
{
	col_free();
	ent_root_array_free();
	snd_free_all();
	tex_free_all();
	game_quit_sdl();
}
