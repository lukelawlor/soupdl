/*
 * c_base.h contains the EcmBase struct and EcmStat enum.
 *
 * The EcmBase struct contains basic data required in every entity struct. This includes the entity's status, id, and index in its entity array. For proper data alignment, it should also be the first variable declared in an entity struct.
 */

#ifndef	ENTITY_C_BASE_H
#define	ENTITY_C_BASE_H

#include "id.h"	// For EntId

// Entity status
typedef enum{
	// Normal status
	ENT_STAT_NORM,

	// The entity is marked for deletion
	ENT_STAT_DEL,
} EcmStat;

// See header comment for info on what this is
typedef struct{
	EcmStat s;

	// Pointer to entity array that the entity is stored in
	EntId id;

	// Index in entity array that the pointer to the entity is stored in
	int i;
} EcmBase;

#endif
