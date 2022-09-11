#ifndef CUNK_MEMORY_H
#define CUNK_MEMORY_H

#include <stddef.h>

/// Growy buffer, a buffer that can grow.
/// Addresses not guaranteed to be stable.
typedef struct Growy_ Growy;

Growy* cunk_new_growy();
void cunk_growy_append_bytes(Growy*, size_t, char*);
#define cunk_growy_append(a, v) cunk_growy_append_bytes(a, sizeof(v), (char*) &v)
char* cunk_growy_data(const Growy*);
void cunk_growy_destroy(Growy*);

/// Arena buffer, stores data together.
/// Addresses are guaranteed to be stable.
typedef struct Arena_ Arena;

Arena* cunk_new_arena();
char* cunk_arena_alloc_bytes(Arena*, size_t);
#define cunk_arena_alloc(T, a) ((T*) cunk_arena_alloc_bytes(a, sizeof(T)))
void cunk_arena_destroy(Arena*);

#endif
