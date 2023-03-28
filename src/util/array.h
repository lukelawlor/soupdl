/*
 * array.h contains a macro for getting the length of an array at compile time.
 */

#ifndef	UTIL_ARRAY_H
#define	UTIL_ARRAY_H

#define	ARR_LEN(array)	(sizeof(array) / sizeof(array[0]))

#endif
