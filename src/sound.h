/*
 * sound.h contains extern declarations for the game's sounds, and functions for loading and freeing all sounds and the game music.
 */

#ifndef	SOUND_H
#define	SOUND_H

#include <SDL2/SDL_mixer.h>

// Mixer sample rate
#define	G_MIX_SAMPLE_RATE	44100

// Max characters the path to a sound file can be
#define	MAX_SND_PATH_LEN	100

// All game sounds
extern Mix_Chunk *snd_step;
extern Mix_Chunk *snd_shoot;
extern Mix_Chunk *snd_splode;
extern Mix_Chunk *snd_bubble;

// Game music
extern Mix_Music *snd_music;

// Loads the music
int snd_load_music(void);

// Loads all game sounds
int snd_load_all(void);

// Frees all game sounds, including the music
void snd_free_all(void);

// Plays a sound effect
void snd_play(Mix_Chunk *snd);

#endif
