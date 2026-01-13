/*
 * sound.h contains extern declarations for the game's sounds, and functions for loading and freeing all sounds and the game music.
 */

#ifndef	SOUND_H
#define	SOUND_H

#include <SDL2/SDL_mixer.h>

// Mixer sample rate
#define	G_MIX_SAMPLE_RATE	44100

// Extern declarations for all game sounds & music
#define	USE_RES(name)	extern Mix_Chunk *snd_##name
	USE_RES(step);
	USE_RES(shoot);
	USE_RES(splode);
	USE_RES(bubble);
	USE_RES(coin);
#undef USE_RES
#define USE_RES(name) extern Mix_Music *snd_mus_##name
USE_RES(egg06);
USE_RES(grianduineog);
#undef USE_RES

// The music that is currently playing, or NULL if there is none
extern Mix_Music *snd_mus_current;

// Loads all game sounds & music
int snd_load_all(void);

// Frees all game sounds & music
void snd_free_all(void);

// Plays a sound effect
void snd_play(Mix_Chunk *snd);

// Play music
void snd_play_mus(Mix_Music *music);

#endif
