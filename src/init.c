/*
 * init.c contains functions for initializing components of the game.
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "dir.h"
#include "video.h"
#include "input.h"
#include "texture.h"
#include "sound.h"
#include "hud.h"
#include "entity/item.h"
#include "error.h"

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
		PERR();
		fprintf(stderr, "failed to initialize SDL. SDL Error: %s\n", SDL_GetError());
		return 1;
	}

	// Getting window and renderer
#ifdef	VSYNC
	int renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
#else
	int renderer_flags = SDL_RENDERER_ACCELERATED;
#endif

	// Default window dimensions
	g_screen_width = 640;
	g_screen_height = 480;

	// Creating the window and renderer
	if ((g_window = SDL_CreateWindow("SoupDL 06", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, g_screen_width, g_screen_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)) == NULL)
	{
		PERR();
		fprintf(stderr, "failed to create window. SDL Error: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	if ((g_renderer = SDL_CreateRenderer(g_window, -1, renderer_flags)) == NULL)
	{
		PERR();
		fprintf(stderr, "failed to create renderer. SDL Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(g_window);
		SDL_Quit();
		return 1;
	}
	
	// Set renderer draw color
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);

	// Initialize SDL_image
	int img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(img_flags) & img_flags))
	{
		PERR();
		fprintf(stderr, "failed ot initialize SDL_image. SDL Error %s\n", IMG_GetError());
		SDL_DestroyRenderer(g_renderer);
		SDL_DestroyWindow(g_window);
		SDL_Quit();
		return 1;
	}

	// Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		PERR();
		fprintf(stderr, "failed to initialize SDL_ttf. SDL Error: %s\n", TTF_GetError());
		SDL_DestroyRenderer(g_renderer);
		SDL_DestroyWindow(g_window);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	// Initialize SDL_mixer
	if (Mix_OpenAudio(G_MIX_SAMPLE_RATE, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		PERR();
		fprintf(stderr, "failed to initialize SDL_mixer. SDL Error: %s\n", Mix_GetError());
		SDL_DestroyRenderer(g_renderer);
		SDL_DestroyWindow(g_window);
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	// Loading font
	if ((g_font = TTF_OpenFont(WORKING_DIR "res/freemono.ttf", 17)) == NULL)
	{
		PERR();
		fprintf(stderr, "failed to open font freemono.ttf. SDL Error: %s\n", TTF_GetError());
		TTF_CloseFont(g_font);
		SDL_DestroyRenderer(g_renderer);
		SDL_DestroyWindow(g_window);
		Mix_Quit();
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	// Loading game window icon
	SDL_Surface *surf;
	if ((surf = IMG_Load(WORKING_DIR "res/cakico.png")) == NULL)
	{
		PERR();
		fprintf(stderr, "failed to load window icon at \"res/cakico.png\". SDL Error: %s\n", IMG_GetError());
		TTF_CloseFont(g_font);
		SDL_DestroyRenderer(g_renderer);
		SDL_DestroyWindow(g_window);
		Mix_Quit();
		TTF_Quit();
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
	TTF_CloseFont(g_font);
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

// Initializes everything needed to start the game loop
// Returns nonzero on error
int game_init_all(void)
{
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

	// Initialize misc systems that depend on game textures being loaded
	hud_init();
	ent_item_init();

	return 0;
}

// Frees everything allocated in game_init_all
void game_quit_all(void)
{
	snd_free_all();
	tex_free_all();
	game_quit_sdl();
}
