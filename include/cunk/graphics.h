#ifndef CUNK_GRAPHICS_H
#define CUNK_GRAPHICS_H

#include "cunk/math.h"

#include <stdbool.h>

typedef struct DrawCtx_ DrawCtx;
typedef struct Window_ Window;

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
