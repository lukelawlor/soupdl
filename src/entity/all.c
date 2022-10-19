/*
 * all.c contains functions that affect all types of entities in the game.
 */

#include "all.h"

// Macros for destroying all existing entities of a certain type
#define	ENT_DESTROY(x)	if ((ent_##x + i)->d.exists) \
				ent_##x##_destroy(ent_##x + i)

// Destroys all entities in the game
void ent_destroy_all(void)
{
	g_player.hp = 0;
	for (int i = 0; i < ENT_LIST_MAX; i++)
	{
		ENT_DESTROY(item);
		ENT_DESTROY(fireball);
		ENT_DESTROY(particle);
		ENT_DESTROY(ragdoll);
		ENT_DESTROY(evilegg);
	}
}
