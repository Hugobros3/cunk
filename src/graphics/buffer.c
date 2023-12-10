#include "graphics_private.h"

#include <stdlib.h>

GfxBuffer* gfx_create_buffer(GfxCtx* ctx, size_t size) {
    GfxBuffer* buf = calloc(1, sizeof(GfxBuffer));
    buf->size = size;
    glGenBuffers(1, &buf->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, buf->vbo);
    return buf;
}

void gfx_copy_to_buffer(GfxBuffer* buffer, void* d, size_t s) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
    glBufferData(GL_ARRAY_BUFFER, s, d, GL_STATIC_DRAW);
}

void gfx_destroy_buffer(GfxBuffer* buffer) {
    glDeleteBuffers(1, &buffer->vbo);
    free(buffer);
}
