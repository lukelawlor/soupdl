/*
 * util.c contains functions for various basic, commonly-used utilities.
 */

#include "util.h"

// Returns the sign of a number (-1 if num is negative, 0 if num is 0, 1 is num is positive)
short sign(int num)
{
	if (num < 0)
		return -1;
	if (num > 0)
		return 1;
	return 0;
}

// Same as sign but for floats
short signf(double num)
{
	if (num < 0.0)
		return -1;
	else if (num > 0.0)
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

// Keeps a value in a certain range but for floats
double clampf(double num, double min, double max)
{
	if (num < min)
		return min;
	if (num > max)
		return max;
	return num;
}
