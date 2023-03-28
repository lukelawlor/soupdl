/*
 * barrier.h contains the barrier tag type and functions for sending and handling barrier check requests.
 */

#ifndef	BARRIER_H
#define	BARRIER_H

#include <stdbool.h>
#include <stddef.h>

#include "entity/id.h"
#include "void_rect.h"

typedef VoidRectInt BarrierTag;

// Sends a barrier check request
// Returns nonzero on error
bool barrier_send_check_request(BarrierTag btag);

// Handles check requests
void barrier_handle_check_requests(void);

#endif
