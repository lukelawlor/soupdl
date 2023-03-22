/*
 * error.h contains macros for printing errors, most notably PERR() and PINF(). PERR() prints error messages and PINF() prints info messages. Both have parameters that follow printf() syntax.
 */

#ifndef	ERROR_H
#define	ERROR_H

// Simple error codes used by functions that have two types of errors: recoverable and non-recoverable
typedef enum{
	// No error occurred
	ERR_NONE,

	// A recoverable error occurred
	ERR_RECOVER,

	// An unrecoverable error occurred
	ERR_NO_RECOVER,
} ErrCode;

// If defined, print errors
#define	PRINT_ERRORS

// If defined, print info
#define	PRINT_INFO

#ifdef	PRINT_ERRORS
	#define	PERR(...)	{ \
					spdl_perr_start(); \
					fprintf(stderr, __FILE__ ":%d ", __LINE__); \
					fprintf(stderr, __VA_ARGS__); \
					spdl_perr_end(); \
				}
#else
	#define	PERR()	do{}while(0)
#endif

#ifdef	PRINT_INFO
	#define	PINF(...)	{ \
					spdl_pinf_start(); \
					fprintf(stderr, __VA_ARGS__); \
					spdl_pinf_end(); \
				}
#else
	#define	PINF()	do{}while(0)
#endif

// Print the start/end of an error/info message
void spdl_perr_start(void);
void spdl_perr_end(void);
void spdl_pinf_start(void);
void spdl_pinf_end(void);

#endif
