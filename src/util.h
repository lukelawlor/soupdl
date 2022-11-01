/*
 * util.h contains basic miscellaneous macros and functions.
 */

#ifndef	UTIL_H
#define	UTIL_H

// Converts the literal characters passed to it into a string
#define	STR_LIT(x)	#x

// Converts a value passed to it into a string
#define	STR(x)		STR_LIT(x)

// Returns minimum value
#define	MIN(x, y)	x < y ? x : y

// Returns the sign of a number (-1 if num is negative, 0 if num is 0, 1 is num is positive)
short sign(int num);

// Same as sign but for floats
short signf(double num);

// Keeps a value in a certain range
int clamp(int num, int min, int max);

// Keeps a value in a certain range but for floats
double clampf(double num, double min, double max);

#endif
