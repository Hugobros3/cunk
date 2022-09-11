#include "cunk/memory.h"

#include <stdlib.h>

static size_t init_size = 4096;

struct Arena_ {
    char* buffer;
    size_t used;
    size_t size;
};

Arena* cunk_new_arena() {
    Arena* a = calloc(1, sizeof(Arena));
    *a = (Arena) {
        .buffer = calloc(1, init_size),
        .size = init_size,
        .used = 0
    };
    return a;
}

char* cunk_arena_alloc_bytes(Arena* a, size_t s) {
    char* old = a->buffer + a->used;
    a->used += s;
    while (a->used >= a->size) {
        a->size *= 2;
        a->buffer = realloc(a->buffer, a->size);
    }
    return old;
}

void cunk_arena_destroy(Arena* a) {
    free(a->buffer);
    free(a);
}

char* cunk_arena_data(const Arena* a) { return a->buffer; }
