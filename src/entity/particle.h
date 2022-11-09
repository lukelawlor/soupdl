/*
 * particle.h contains the type for particles.
 *
 * This file is automatically included when you #include all.h
 */

#ifndef	ENTITY_PARTICLE_H
#define	ENTITY_PARTICLE_H

#include <SDL2/SDL.h>

// Total number of different types of particles
#define	PTCL_MAX	3

// Particle id type
typedef enum{
	PTCL_BUBBLE,
	PTCL_FLAME,
	PTCL_STAR
} EntParticleId;

// Particle type
typedef struct{
	EcmId id;

	// Position
	float x;
	float y;

	// Horizontal and vertical speed
	float hsp;
	float vsp;

	// Gravity
	float grv;

	// Duration in ticks
	int dur;

	// Id of particle (bubble, flame, star, etc.)
	EntParticleId pid;
} EntPARTICLE;

EntPARTICLE *ent_new_PARTICLE(float x, float y, EntParticleId pid);
void ent_update_PARTICLE(EntPARTICLE *e);
void ent_draw_PARTICLE(EntPARTICLE *e);
void ent_destroy_PARTICLE(EntPARTICLE *e);

#endif
