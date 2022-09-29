/*
 * error.h contains macros for printing errors.
 */

#ifndef	ERROR_H
#define	ERROR_H

// Printing error messages with a string + an SDL error
#define	PERRS(msg, libmsg)	fprintf(stderr, "soupdl06: " msg ". SDL Error: %s\n", libmsg)

// Printing error messages with a single string
#define	PERR(msg)	fprintf(stderr, "soupdl06: " msg ".\n")

#endif
