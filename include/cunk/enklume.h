#ifndef CUNK_ENKLUME_H
#define CUNK_ENKLUME_H

#include <stdbool.h>

typedef struct McRegion_ McRegion;
typedef struct McChunk_ McChunk;
typedef struct McWorld_ McWorld;

McWorld* cunk_open_mcworld(const char* folder);
void cunk_close_mcworld(McWorld*);

McRegion* cunk_open_mcregion(McWorld* world, int x, int z);
McChunk* cunk_open_mcchunk(McRegion* world, unsigned int x, unsigned int z);

typedef struct NBT_Object_ NBT_Object;
const NBT_Object* cunk_mcchunk_get_root(const McChunk*);

#endif
