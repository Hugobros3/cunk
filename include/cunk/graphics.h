#ifndef CUNK_GRAPHICS_H
#define CUNK_GRAPHICS_H

#include "cunk/math.h"

#include <stddef.h>
#include <stdbool.h>

typedef struct Window_ Window;
typedef struct GfxCtx_ GfxCtx;
typedef struct GfxShader_ GfxShader;
typedef struct GfxBuffer_ GfxBuffer;

Window* create_window(const char* title, int width, int height, GfxCtx**);
GfxShader* create_shader(GfxCtx*, const char* vs, const char* fs);
GfxBuffer* create_buffer(GfxCtx*, size_t);
void copy_to_buffer(GfxBuffer*, void*, size_t);
void destroy_buffer(GfxBuffer*);

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
