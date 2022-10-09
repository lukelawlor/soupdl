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
	EntDefaults d;

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
	EntParticleId id;
} EntParticle;

// Constant pointer to the first index of the particle array
extern EntParticle *const ent_particle;

EntParticle *ent_particle_new(float x, float y, EntParticleId id);
void ent_particle_update(EntParticle *e);
void ent_particle_draw(EntParticle *e);
void ent_particle_destroy(EntParticle *e);

#endif
