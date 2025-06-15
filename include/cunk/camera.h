#ifndef CUNK_CAMERA_H
#define CUNK_CAMERA_H

#include "math.h"

#include "nasl/nasl.h"
#include "nasl/nasl_mat.h"

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    vec3 position;
    struct {
        float yaw, pitch;
    } rotation;
    float fov;
} Camera;

vec3 camera_get_forward_vec(const Camera*);
vec3 camera_get_left_vec(const Camera*);
mat4 camera_get_view_mat4(const Camera*, size_t, size_t);

typedef struct {
    float fly_speed, mouse_sensitivity;
    double last_mouse_x, last_mouse_y;
    unsigned mouse_was_held: 1;
} CameraFreelookState;

typedef struct {
    bool mouse_held;
    bool should_capture;
    double mouse_x, mouse_y;
    struct {
        bool forward, back, left, right;
    } keys;
} CameraInput;

void camera_move_freelook(Camera*, CameraInput*, CameraFreelookState*);

#ifdef CUNK_GRAPHICS_H
/// Include graphics.h first to use this!
void gfx_camera_update(Window* w, CameraInput* input);
#endif

#endif
