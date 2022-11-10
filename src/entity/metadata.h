/*
 * metadata.h contains metadata for the different types of entities.
 */

#ifndef	ENTITY_METADATA_H
#define	ENTITY_METADATA_H

// Entity tile texture in map editor
typedef struct{
	SDL_Texture *tex;
	SDL_Rect srect;
} EntTileTex;

typedef struct{
	// The character used to represent the entity in a map file (make sure no 2 tileable objects (tiles or entities) have use same character)
	char map_char;

	// Proper name of the entity
	char *name;

	EntTileTex tile_tex;
} EntMetadata;

// Constant pointer to the entity metadata array
extern EntMetadata *const g_ent_md;

// Initializes the entity metadata array
void ent_metadata_init(void);

#endif
