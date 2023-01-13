/*
 * error.h contains macros for printing errors.
 */

#ifndef	ERROR_H
#define	ERROR_H

// Simple error codes used by functions that have two types of errors: recoverable and non-recoverable
typedef enum{
	// No error occurred
	ERR_NONE,

	// A recoverable error occurred
	ERR_RECOVERABLE,

	// An unrecoverable error occurred
	ERR_UNRECOVERABLE,
} ErrCode;

// If defined, print errors
#define	PRINT_ERRORS

// If defined, print info
#define	PRINT_INFO

// Text to print before an error message
#ifdef	PRINT_ERRORS
	#define	PERR_START	"soupdl: error: "
	#define	PERR_END	"\n"
	#define	PERR(...)	{ \
					fprintf(stderr, PERR_START); \
					fprintf(stderr, __VA_ARGS__); \
					fprintf(stderr, PERR_END); \
				}
#else
	#define	PERR()	do{}while(0)
#endif

#ifdef	PRINT_INFO
	#define	PINF_START	"soupdl: info: "
	#define	PINF_END	"\n"
	#define	PINF(...)	{ \
					fprintf(stderr, PINF_START); \
					fprintf(stderr, __VA_ARGS__); \
					fprintf(stderr, PINF_END); \
				}
#else
	#define	PINF()	do{}while(0)
#endif

#endif
