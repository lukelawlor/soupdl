/*
 * sound.c contains functions for loading and freeing all game sounds, including the music.
 */

#include <stdio.h>

#include <SDL2/SDL_mixer.h>

#include "dir.h"
#include "error.h"
#include "sound.h"

// Global variables for all game sounds & music
#define	USE_RES(name)	Mix_Chunk *snd_##name
	USE_RES(step);
	USE_RES(shoot);
	USE_RES(splode);
	USE_RES(bubble);
	USE_RES(coin);
#undef USE_RES
#define USE_RES(name) Mix_Music *snd_mus_##name
USE_RES(egg06);
USE_RES(grianduineog);
#undef USE_RES

// The music that is currently playing, or NULL if there is none
Mix_Music *snd_mus_current;

// Load a sound from a .wav file
static Mix_Chunk *snd_load_wav(char *path);

// Load music
static Mix_Music *snd_load_mus(char *path);

// Load a sound effect from a .wav file, returns NULL on error
// path should be the filename without ".wav"
static Mix_Chunk *snd_load_wav(char *path)
{
	Mix_Chunk *chunk;

	// Get full_path from path
	char full_path[RES_PATH_MAX];
	snprintf(full_path, RES_PATH_MAX, DIR_SND "/%s.wav", path);

	if ((chunk = Mix_LoadWAV(full_path)) == NULL)
	{
		PERR("failed to load wav \"%s\". SDL Error: %s", full_path, Mix_GetError());
		return NULL;
	}

	return chunk;
}

// Load music & returns NULL on error
// path should be the filename without ".xm"
static Mix_Music *snd_load_mus(char *path)
{
        Mix_Music *mus;

        // Get full_path from path
        char full_path[RES_PATH_MAX];
        snprintf(full_path, RES_PATH_MAX, DIR_MUS "/%s.xm", path);

	if ((mus = Mix_LoadMUS(full_path)) == NULL)
	{
		PERR("failed to load music. SDL Error: %s", Mix_GetError());
		return NULL;
	}
	return mus;
}

// Load all game sounds & return nonzero on error
int snd_load_all(void)
{
// Try to load a sound
#define	USE_RES(name)	if ((snd_##name = snd_load_wav(#name)) == NULL) \
				goto l_error
	USE_RES(step);
	USE_RES(shoot);
	USE_RES(splode);
	USE_RES(bubble);
	USE_RES(coin);
#undef USE_RES
#define USE_RES(name) if ((snd_mus_##name = snd_load_mus(#name)) == NULL) \
                        goto l_error
        USE_RES(egg06);
        USE_RES(grianduineog);
#undef USE_RES
        snd_mus_current = NULL;
        snd_play_mus(snd_mus_egg06);
	return 0;
l_error:
	snd_free_all();
	return 1;
}

// Free all sounds, including music
void snd_free_all(void)
{
#define	USE_RES(name)	Mix_FreeChunk(snd_##name)
	USE_RES(step);
	USE_RES(shoot);
	USE_RES(splode);
	USE_RES(bubble);
	USE_RES(coin);
#undef USE_RES
#define	USE_RES(name) Mix_FreeMusic(snd_mus_##name)
        USE_RES(egg06);
        USE_RES(grianduineog);
#undef USE_RES
}

// Play a sound effect
void snd_play(Mix_Chunk *snd)
{
	Mix_PlayChannel(-1, snd, 0);
}

// Play music
void snd_play_mus(Mix_Music *mus)
{
	Mix_PlayMusic(mus, -1);
        snd_mus_current = mus;
}
