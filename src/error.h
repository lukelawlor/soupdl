/*
 * error.h contains macros for printing errors.
 */

#ifndef	ERROR_H
#define	ERROR_H

// Printing error messages with SDL subsystem errors
#define	PERRS(msg, libmsg)	fprintf(stderr, "soupdl06: " msg ". SDL Error: %s\n", libmsg)

// Printing error messages like normal
#define	PERR(msg)	fprintf(stderr, "soupdl06: " msg ".\n")

#endif
