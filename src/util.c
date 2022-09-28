/*
 * util.c contains functions for various basic, commonly-used utilities.
 */

#include "util.h"

// Returns the sign of a number (-1 if num is negative, 0 if num is 0, 1 is num is positive)
int sign(int num)
{
	if (num < 0)
		return -1;
	if (num > 0)
		return 1;
	return 0;
}

// Same as sign but for floats
int fsign(float num)
{
	if (num < 0.0f)
		return -1;
	else if (num > 0.0f)
		return 1;
	return 0;
}

// Keeps a value in a certain range
int clamp(int num, int min, int max)
{
	if (num < min)
		return min;
	if (num > max)
		return max;
	return num;
}
