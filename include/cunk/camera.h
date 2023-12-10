#ifndef CUNK_CAMERA_H
#define CUNK_CAMERA_H

#include "cunk/math.h"

typedef struct {
    Vec3f position;
    struct {
        float yaw, pitch;
    } rotation;
    float fov;
} Camera;

Vec3f camera_get_forward_vec(const Camera*);
Vec3f camera_get_left_vec(const Camera*);
Mat4f camera_get_view_mat4(const Camera*, size_t, size_t);

typedef struct {
    float fly_speed, mouse_sensitivity;
    double last_mouse_x, last_mouse_y;
    unsigned mouse_was_held: 1;
} CameraFreelookState;

#ifdef CUNK_GRAPHICS_H
/// Include graphics.h first to use this!
void camera_move_freelook(Camera*, Window*, CameraFreelookState*);
#endif

#endif
