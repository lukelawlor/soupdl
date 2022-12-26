/*
 * string.h contains macros for manipulating strings with the preprocessor.
 */

#ifndef	UTIL_STRING_H
#define	UTIL_STRING_H

// Converts the literal characters passed to it into a string
#define	STR_LIT(x)	#x

// Converts a value passed to it into a string
#define	STR(x)		STR_LIT(x)

#endif
