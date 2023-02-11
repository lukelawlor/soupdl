/*
 * dir.h contains miscellaneous directories used by the game.
 */

#ifndef	DIR_H
#define	DIR_H

// The maximum size in bytes that a string containing a path to a game resource can be
#define	RES_PATH_MAX	100

// Program working directory relative to its normal working directory
#define	DIR_WORK	"."

// Resources directory
#define	DIR_RES		DIR_WORK "/res"

// Graphics directory
#define	DIR_GFX		DIR_RES "/gfx"

// Map directory
#define	DIR_MAP		DIR_RES "/map"

// Music directory
#define	DIR_MUS		DIR_RES "/mus"

// Sound effects directory
#define	DIR_SND		DIR_RES "/snd"

// Game saves directory
#define	DIR_SAVE	DIR_RES "/sav"

#endif
