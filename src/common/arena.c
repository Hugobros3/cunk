#include "cunk/memory.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

static size_t init_size = 4096;

struct Growy_ {
    void* buffer;
    size_t used, size;
};

Growy* cunk_new_growy() {
    Growy* g = calloc(1, sizeof(Growy));
    *g = (Growy) {
        .buffer = calloc(1, init_size),
        .size = init_size,
        .used = 0
    };
    return g;
}

void cunk_growy_append_bytes(Growy* g, size_t s, const void* bytes) {
    size_t old_used = g->used;
    g->used += s;
    while (g->used >= g->size) {
        g->size *= 2;
        g->buffer = realloc(g->buffer, g->size);
    }
    memcpy(g->buffer + old_used, bytes, s);
}

void cunk_growy_destroy(Growy* g) {
    free(g->buffer);
    free(g);
}

void* cunk_growy_deconstruct(Growy* g) {
    void* buf = g->buffer;
    free(g);
    return buf;
}

size_t cunk_growy_size(const Growy* g) { return g->used; }
void* cunk_growy_data(const Growy* g) { return g->buffer; }

typedef struct {
    void* alloc;
    size_t used, size;
} Page;

struct Arena_ {
    Growy* pages;
    int pages_count;
    int page_size;
    size_t overall_size;
};

Arena* cunk_new_arena() {
    Arena* a = calloc(1, sizeof(Arena));
    *a = (Arena) {
        .page_size = 32768,
        .pages_count = 0,
        .pages = cunk_new_growy(),
    };
    return a;
}

size_t cunk_arena_size(const Arena* a) {
    return a->overall_size;
}

void* cunk_arena_alloc_bytes(Arena* a, size_t size) {
    Page* last_page = NULL;
    if (a->pages_count > 0)
        last_page = &((Page*) cunk_growy_data(a->pages))[a->pages_count - 1];

    if (!last_page || (last_page->used + size) >= last_page->size) {
        size_t new_page_size = size < a->page_size ? a->page_size : size;
        Page new_page = {
            .size = new_page_size,
            .used = 0,
            .alloc = calloc(1, new_page_size)
        };
        cunk_growy_append(a->pages, new_page);
        a->pages_count++;
        last_page = &((Page*) cunk_growy_data(a->pages))[a->pages_count - 1];
    }

    char* ptr = last_page->alloc + last_page->used;
    last_page->used += size;
    assert(last_page->used <= last_page->size);
    a->overall_size += size;
    return ptr;
}

char* cunk_arena_string(Arena* a, const char* str) {
    size_t len = strlen(str);
    char* s = cunk_arena_alloc_bytes(a, len + 1);
    memcpy(s, str, len + 1);
    return s;
}

void cunk_arena_destroy(Arena* a) {
    for (int i = 0; i < a->pages_count; i++)
        free(((Page*) cunk_growy_data(a->pages))[i].alloc);
    cunk_growy_destroy(a->pages);
    free(a);
}
