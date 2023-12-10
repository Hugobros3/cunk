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
GfxShader* gfx_create_shader(GfxCtx*, const char* vs, const char* fs);

GfxBuffer* gfx_create_buffer(GfxCtx*, size_t);
void gfx_copy_to_buffer(GfxBuffer*, void*, size_t);
void gfx_destroy_buffer(GfxBuffer*);

typedef enum {
    GFX_TCF_U8_UNORM,
    GFX_TCF_I32,
    GFX_TCF_F32,
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

typedef struct {
    Vec3f position;
    struct {
        float yaw, pitch;
    } rotation;
    float fov;
} Camera;

Vec3f camera_get_forward_vec(const Camera*);
Vec3f camera_get_left_vec(const Camera*);
Mat4f camera_get_view_mat4(const Camera*, const Window*);

typedef struct {
    float fly_speed, mouse_sensitivity;
    double last_mouse_x, last_mouse_y;
    unsigned mouse_was_held: 1;
} CameraFreelookState;

void camera_move_freelook(Camera*, Window*, CameraFreelookState*);
#endif
