/*
 * random.c contains the function definition for random and the definition for all of g_random_value's values
 */

#include "random.h"

// Length of random value array
#define	G_RANDOM_VALUE_LEN	100

// Array containing random values (0 to 255)
static const int g_random_value[G_RANDOM_VALUE_LEN];

// Fast (kind of) random number returner
int spdl_random(void)
{
	// Position in random value array
	static int g_random_index = 0;

	if (++g_random_index >= G_RANDOM_VALUE_LEN)
		g_random_index = 0;
	
	return g_random_value[g_random_index];
}

// Array containing random values (0 to 255)
static const int g_random_value[G_RANDOM_VALUE_LEN] = {
	5,
	186,
	40,
	89,
	232,
	23,
	96,
	132,
	65,
	12,
	158,
	219,
	166,
	123,
	54,
	248,
	2,
	196,
	254,
	18,
	109,
	200,
	78,
	82,
	25,
	128,
	74,
	48,
	158,
	209,
	183,
	28,
	98,
	25,
	168,
	122,
	63,
	6,
	31,
	125,
	80,
	189,
	228,
	176,
	151,
	68,
	42,
	39,
	148,
	128,
	182,
	77,
	205,
	103,
	185,
	45,
	82,
	25,
	74,
	82,
	128,
	112,
	130,
	188,
	132,
	53,
	87,
	134,
	93,
	25,
	97,
	129,
	200,
	243,
	221,
	178,
	75,
	39,
	58,
	3,
	98,
	130,
	152,
	183,
	253,
	19,
	7,
	38,
	14,
	72,
	124,
	168,
	187,
	132,
	107,
	204,
	39,
	70,
	139,
	196
};
