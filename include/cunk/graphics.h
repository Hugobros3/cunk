#ifndef CUNK_GRAPHICS_H
#define CUNK_GRAPHICS_H

#include "cunk/math.h"

#include <stddef.h>
#include <stdbool.h>

typedef struct Window_ Window;
typedef struct GfxCtx_ GfxCtx;
typedef struct GfxShader_ GfxShader;
typedef struct GfxBuffer_ GfxBuffer;
typedef struct GfxTexture_ GfxTexture;

Window* gfx_create_window(const char* title, int width, int height, GfxCtx**);
void gfx_get_window_size(Window*, size_t*, size_t*);
GfxShader* gfx_create_shader(GfxCtx*, const char* vs, const char* fs);

GfxBuffer* gfx_create_buffer(GfxCtx*, size_t);
void gfx_copy_to_buffer(GfxBuffer*, void*, size_t);
void gfx_destroy_buffer(GfxBuffer*);

#define TexCompFormats(Mk) \
Mk(8, UNORM) \
Mk(32, I) \
Mk(32, F) \

#define TexCompFormat(W, T) GFX_TCF_##T##W,
typedef enum {
TexCompFormats(TexCompFormat)
} GfxTexCompFormat;
typedef struct {
    GfxTexCompFormat base;
    unsigned num_components;
} GfxTexFormat;
GfxTexture* gfx_create_texture(GfxCtx*, unsigned width, unsigned height, unsigned depth, GfxTexFormat);
void gfx_upload_texture(GfxTexture*, void* data);
void gfx_destroy_texture(GfxTexture*);

void gfx_cmd_resize_viewport(GfxCtx*, Window*);
void gfx_cmd_clear(GfxCtx*);
void gfx_cmd_use_shader(GfxCtx*, GfxShader*);

typedef struct {
    bool wireframe, face_culling, depth_testing;
} GfxState;

void gfx_cmd_set_draw_state(GfxCtx*, GfxState);
void gfx_cmd_set_shader_extern(GfxCtx*, const char*, void*);
void gfx_cmd_set_vertex_input(GfxCtx*, const char*, GfxBuffer*, int components, size_t stride, size_t offset);
void gfx_cmd_draw_arrays(GfxCtx*, size_t, size_t);

void gfx_wait_for_idle();
#endif
