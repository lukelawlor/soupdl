/*
 * metadata.c contains the entity metadata array.
 */

#include <SDL2/SDL.h>

#include "../texture.h"
#include "id.h"
#include "metadata.h"

// Entity metadata array
static EntMetadata g_ent_metadata[ENT_MAX];

// Constant pointer to the entity metadata array
EntMetadata *const g_ent_md = g_ent_metadata;

// Initializes the entity metadata array
void ent_metadata_init(void)
{
	g_ent_metadata[ENT_ID_PLAYER] = (EntMetadata) {
		'p',
		"Player Spawn Point",
		{tex_egg, {0, 32, 32, 32}},
	};
	g_ent_metadata[ENT_ID_ITEM] = (EntMetadata) {
		't',
		"Item",
		{tex_trumpet, {0, 0, 19, 11}},
	};
	g_ent_metadata[ENT_ID_FIREBALL] = (EntMetadata) {
		'f',
		"Fireball",
		{tex_fireball, {0, 0, 16, 16}},
	};
	g_ent_metadata[ENT_ID_PARTICLE] = (EntMetadata) {
		'*',
		"Particle",
		{tex_particle, {0, 0, 10, 10}},
	};
	g_ent_metadata[ENT_ID_RAGDOLL] = (EntMetadata) {
		'r',
		"Ragdoll",
		{tex_egg, {64, 0, 32, 32}},
	};
	g_ent_metadata[ENT_ID_GROUNDGUY] = (EntMetadata) {
		'g',
		"Groundguy",
		{tex_evilegg, {0, 0, 32, 32}},
	};
	g_ent_metadata[ENT_ID_CLOUD] = (EntMetadata) {
		'c',
		"Cloud",
		{tex_cloud, {103, 0, 42, 23}},
	};
	g_ent_metadata[ENT_ID_SLIDEGUY] = (EntMetadata) {
		'v',
		"Slideguy",
		{tex_evilegg, {0, 0, 32, 32}},
	};
	g_ent_metadata[ENT_ID_JUMPGUY] = (EntMetadata) {
		'j',
		"Jumpguy",
		{tex_evilegg, {0, 0, 32, 32}},
	};
}
