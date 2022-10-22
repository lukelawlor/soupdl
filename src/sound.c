/*
 * sound.c contains functions for loading and freeing all game sounds, including the music.
 */

#include <stdio.h>

#include <SDL2/SDL_mixer.h>

#include "dir.h"
#include "error.h"
#include "sound.h"

// All game sounds
Mix_Chunk *snd_step = NULL;
Mix_Chunk *snd_shoot = NULL;
Mix_Chunk *snd_splode = NULL;
Mix_Chunk *snd_bubble = NULL;

// Game music
Mix_Music *snd_music = NULL;

// Loads a sound from a .wav file
static Mix_Chunk *snd_load_wav(char *path);

/*
 * Loads a sound effect from a .wav file, returns NULL on error
 *
 * path should be the filename without ".wav"
 */
static Mix_Chunk *snd_load_wav(char *path)
{
	Mix_Chunk *chunk;

	// Getting full_path from path
	char full_path[MAX_SND_PATH_LEN];
	sprintf(full_path, WORKING_DIR "res/%s.wav", path);

	// Loading wav
	if ((chunk = Mix_LoadWAV(full_path)) == NULL)
	{
		PERR();
		fprintf(stderr, "failed to load wav \"%s\". SDL Error: %s\n", full_path, Mix_GetError());
		return NULL;
	}

	return chunk;
}

/*
 * Loads the game music (NOTE: there's no music right now so this shouldn't be called)
 */
int snd_load_music(void)
{
	if ((snd_music = Mix_LoadMUS(WORKING_DIR "res/intermission.ogg")) == NULL)
	{
		PERR();
		fprintf(stderr, "failed to load music. SDL Error: %s\n", Mix_GetError());
		return 1;
	}
	return 0;
}

/*
 * Loads all game sounds, returns nonzero on error
 */
int snd_load_all(void)
{
	if ((snd_step = snd_load_wav("step")) == NULL)
		goto l_error;
	if ((snd_shoot = snd_load_wav("shoot")) == NULL)
		goto l_error;
	if ((snd_splode = snd_load_wav("splode")) == NULL)
		goto l_error;
	if ((snd_bubble = snd_load_wav("bubble")) == NULL)
		goto l_error;
	/*
	if (snd_load_music())
		goto l_error;
	*/
	return 0;
l_error:
	snd_free_all();
	return 1;
}

/*
 * Frees all sounds, including music
 */
void snd_free_all(void)
{
	Mix_FreeChunk(snd_step);
	Mix_FreeChunk(snd_shoot);
	Mix_FreeChunk(snd_splode);
	Mix_FreeChunk(snd_bubble);
	Mix_FreeMusic(snd_music);
}
