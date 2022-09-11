#ifndef CUNK_MEMORY_H
#define CUNK_MEMORY_H

#include <stddef.h>

typedef struct Arena_ Arena;

#define cunk_arena_alloc(T, a) ((T*) cunk_arena_alloc_bytes(a, sizeof(T)))

Arena* cunk_new_arena();
char* cunk_arena_alloc_bytes(Arena*, size_t);
char* cunk_arena_data(const Arena*);
void cunk_arena_destroy(Arena*);

#endif
