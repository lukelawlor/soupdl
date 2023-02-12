/*
 * error.h contains functions for printing error & info messages.
 */

#include <stdio.h>

#include "error.h"

void spdl_perr_start(void)
{
	fprintf(stderr, "soupdl: error: ");
}

void spdl_perr_end(void)
{
	fputc('\n', stderr);
}

void spdl_pinf_start(void)
{
	fprintf(stderr, "soupdl: info: ");
}

void spdl_pinf_end(void)
{
	fputc('\n', stderr);
}
