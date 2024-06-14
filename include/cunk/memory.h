#ifndef CUNK_MEMORY_H
#define CUNK_MEMORY_H

#include <stddef.h>

/// Growy buffer, a buffer that can grow.
/// Addresses not guaranteed to be stable.
typedef struct Growy_ Growy;

Growy* cunk_new_growy();
void cunk_growy_append_bytes(Growy*, size_t, const void*);
#define cunk_growy_append(a, v) cunk_growy_append_bytes(a, sizeof(v), (char*) &v)
size_t cunk_growy_size(const Growy*);
void* cunk_growy_data(const Growy*);
void cunk_growy_destroy(Growy*);
// Like destroy, but we scavenge the internal allocation for later use.
void* cunk_growy_deconstruct(Growy*);

/// Arena buffer, stores data together.
/// Addresses are guaranteed to be stable.
typedef struct Arena_ Arena;

Arena* cunk_new_arena();
void* cunk_arena_alloc_bytes(Arena*, size_t);
size_t cunk_arena_size(const Arena*);
#define cunk_arena_alloc(T, a) ((T*) cunk_arena_alloc_bytes(a, sizeof(T)))
char* cunk_arena_string(Arena*, const char*);
void cunk_arena_destroy(Arena*);

#endif
