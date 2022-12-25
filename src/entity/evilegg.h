/*
 * evilegg.h contains functions that are commonly used by enemy evil egg entities. These include most enemies with names ending in "guy".
 */

#ifndef	ENTITY_EVILEGG_H
#define	ENTITY_EVILEGG_H

void evilegg_handle_collisions(void);
void evilegg_handle_animation(void);
void evilegg_draw(void);
void evilegg_destroy(void);

#endif
