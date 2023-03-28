/*
 * barrier.c contains functions for sending and handling barrier check requests.
 */

#include <stdbool.h>
#include <stddef.h>

#include "barrier.h"
#include "entity/all.h"
#include "entity/array.h"
#include "entity/id.h"
#include "entity/root.h"
#include "util/array.h"

// Barrier carrier definition
// Provides info about entities that carry barrier tags
typedef struct{
	EntId id;
	size_t btag_offset;
} BarrierTagCarrierDef;

static BarrierTagCarrierDef g_btc_list[] = {
	{ENT_ID_GROUNDGUY, offsetof(EntGROUNDGUY, btag)},
	{ENT_ID_SLIDEGUY, offsetof(EntSLIDEGUY, btag)},
};

// Barrier check request stack
#define	BARRIER_CHECK_REQUEST_STACK_LEN	20

static struct{
	BarrierTag stack[BARRIER_CHECK_REQUEST_STACK_LEN];
	int len;
} g_bcr_stack;

// Sends a barrier check request
// Returns nonzero on error
bool barrier_send_check_request(BarrierTag btag)
{
	if (btag == 0)
	{
		PINF("btag 0 check request sent. ignoring.");
		return 1;
	}
	if (g_bcr_stack.len >= BARRIER_CHECK_REQUEST_STACK_LEN)
	{
		PERR("max barrier check requests sent. ignoring current request.");
		return 1;
	}

	// Check if the tag was already added to the stack
	for (int i = 0; i < g_bcr_stack.len; ++i)
	{
		if (g_bcr_stack.stack[i] == btag)
		{
			// Tag was already added to the stack
			return 1;
		}

	}

	// Add the tag to the stack
	g_bcr_stack.stack[g_bcr_stack.len++] = btag;

	return 0;
}

// Handles check requests
void barrier_handle_check_requests(void)
{
	// Loop through requests
	for (int i = g_bcr_stack.len - 1; i >= 0; --i)
	{
		// Current btag being checked
		BarrierTag btag = g_bcr_stack.stack[i];

		bool btag_should_clean = true;

		// Loop through entities that could have a matching btag
		for (int j = 0; j < (int) ARR_LEN(g_btc_list); ++j)
		{
			BarrierTagCarrierDef *d = &g_btc_list[j];

			// Loop through entity array
			EntArray *ea = g_er[d->id];
			void *e = ea->e + d->btag_offset;

			for (int k = 0; k < ea->len; ++k)
			{
				if (* (BarrierTag *) e == btag)
				{
					// Entity has matching btag, don't clean
					btag_should_clean = false;
					goto l_ent_loop_exit;
				}
				e += ea->ent_size;
			}
		}

	l_ent_loop_exit:
		// Clean the btag if necessary
		if (btag_should_clean)
		{
			PINF("cleaning barrier tag %d", btag);

			EntBARRIER *e = g_er[ENT_ID_BARRIER]->e;
			for (int i = 0; i < g_er[ENT_ID_BARRIER]->len; ++i)
			{
				if (e->btag == btag)
					ent_destroy_BARRIER(e);
				++e;
			}
		}
	}

	// Finished looping through requests, reset request stack
	g_bcr_stack.len = 0;
}
