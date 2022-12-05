/*
 * error.h contains macros for printing errors.
 */

#ifndef	ERROR_H
#define	ERROR_H

#include <errno.h>

// Text to print before an error message
#define	PERR()	fprintf(stderr, "soupdl: ")

#endif
