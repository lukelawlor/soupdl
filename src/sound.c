/*
 * sound.c contains functions for loading and freeing all game sounds, including the music.
 */

#include <stdio.h>

#include <SDL2/SDL_mixer.h>

#include "dir.h"
#include "error.h"
#include "sound.h"

// Global variables for all game sounds
#define	USE_RES(name)	Mix_Chunk *snd_##name
	USE_RES(step);
	USE_RES(shoot);
	USE_RES(splode);
	USE_RES(bubble);
	USE_RES(coin);
#undef USE_RES

// Game music
Mix_Music *snd_music;

// Loads a sound from a .wav file
static Mix_Chunk *snd_load_wav(char *path);

// Loads a sound effect from a .wav file, returns NULL on error
// path should be the filename without ".wav"
static Mix_Chunk *snd_load_wav(char *path)
{
	Mix_Chunk *chunk;

	// Getting full_path from path
	char full_path[RES_PATH_MAX];
	snprintf(full_path, RES_PATH_MAX, DIR_SND "/%s.wav", path);

	// Loading wav
	if ((chunk = Mix_LoadWAV(full_path)) == NULL)
	{
		PERR("failed to load wav \"%s\". SDL Error: %s", full_path, Mix_GetError());
		return NULL;
	}

	return chunk;
}

// Loads the game music (NOTE: there's no music right now so this shouldn't be called)
int snd_load_music(void)
{
	if ((snd_music = Mix_LoadMUS(DIR_MUS "/test.xm")) == NULL)
	{
		PERR("failed to load music. SDL Error: %s", Mix_GetError());
		return 1;
	}
	return 0;
}

// Loads all game sounds, returns nonzero on error
int snd_load_all(void)
{
// Tries to load a sound
#define	USE_RES(name)	if ((snd_##name = snd_load_wav(#name)) == NULL) \
				goto l_error
	USE_RES(step);
	USE_RES(shoot);
	USE_RES(splode);
	USE_RES(bubble);
	USE_RES(coin);
#undef USE_RES
	/*
	if (snd_load_music())
		goto l_error;
	*/
	return 0;
l_error:
	snd_free_all();
	return 1;
}

// Frees all sounds, including music
void snd_free_all(void)
{
#define	USE_RES(name)	Mix_FreeChunk(snd_##name)
	USE_RES(step);
	USE_RES(shoot);
	USE_RES(splode);
	USE_RES(bubble);
	USE_RES(coin);
#undef USE_RES
	Mix_FreeMusic(snd_music);
}

// Plays a sound effect
void snd_play(Mix_Chunk *snd)
{
	Mix_PlayChannel(-1, snd, 0);
}
