/*
 * door.h contains the type for door entities.
 *
 * This file is automatically included when you #include all.h
 *
 * Doors are used by the player to enter new maps. They are assigned an ID in the range [0, ENT_DOOR_MAX) that tells the game what map file they lead to. Map file paths are stored in the 2d char array g_ent_door_map_path, which is indexed by door IDs.
 *
 *
 */

#ifndef	ENTITY_DOOR_H
#define	ENTITY_DOOR_H

#include "c_body.h"
#include "entity.h"

// The maximum number of doors in a map
// Valid door ids must be in the range [0, ENT_DOOR_MAX)
#define	ENT_DOOR_MAX	4

// The maximum length of a map filename to load
#define	ENT_DOOR_MAP_PATH_MAX	20

// Returns true if the door id passed to it is valid
#define	ENT_DOOR_ID_IS_VALID(did)	(did >= 0 && did < ENT_DOOR_MAX)

// The Id of a door links the door to the map it leads to
// The paths to map files that doors lead to are in g_ent_door_map_path, which is indexed by door ids
typedef int8_t EntDoorId;

// Entity item type
typedef struct{
	EcmBase base;
	EcmBody b;
	EntDoorId did;
} EntDOOR;

// Array of paths to map files that doors lead to, indexed by EntDoorIds
extern char g_ent_door_map_path[ENT_DOOR_MAX][ENT_DOOR_MAP_PATH_MAX];

// The Id of the door last used
extern EntDoorId g_ent_door_last_used;

EntDOOR *ent_new_DOOR(int x, int y, EntDoorId did);
void ent_update_DOOR(EntDOOR *e);
void ent_draw_DOOR(EntDOOR *e);
void ent_destroy_DOOR(EntDOOR *e);

// Returns true if the door id passed to it is valid
int ent_door_assert_valid_did(EntDoorId did);

#endif
