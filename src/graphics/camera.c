#include "graphics_private.h"

#include "stdio.h"
#include <assert.h>
#include <math.h>

static Mat4f camera_rotation_matrix(const Camera* camera) {
    Mat4f matrix = identity_mat4f;
    matrix = mul_mat4f(rotate_axis_mat4f(1, camera->rotation.yaw), matrix);
    matrix = mul_mat4f(rotate_axis_mat4f(0, camera->rotation.pitch), matrix);
    return matrix;
}

Mat4f camera_get_view_mat4(const Camera* camera, const Window* window) {
    Mat4f matrix = identity_mat4f;
    matrix = mul_mat4f(translate_mat4f(vec3f_neg(camera->position)), matrix);
    matrix = mul_mat4f(camera_rotation_matrix(camera), matrix);
    float ratio = ((float) window->width) / ((float) window->height);
    matrix = mul_mat4f(perspective_mat4f(ratio, camera->fov, 0.1f, 1000.f), matrix);
    return matrix;
}

Vec3f camera_get_forward_vec(const Camera* cam) {
    Vec4f initial_forward = { .z = -1, .w = 1 };
    // we invert the rotation matrix and use the front vector from the camera space to get the one in world space
    Mat4f matrix = invert_mat4(camera_rotation_matrix(cam));
    Vec4f result = mul_mat4f_vec4f(matrix, initial_forward);
    return vec3f_scale(result.xyz, 1.0f / result.w);
}

Vec3f camera_get_left_vec(const Camera* cam) {
    Vec4f initial_forward = { .x = -1, .w = 1 };
    Mat4f matrix = invert_mat4(camera_rotation_matrix(cam));
    Vec4f result = mul_mat4f_vec4f(matrix, initial_forward);
    return vec3f_scale(result.xyz, 1.0f / result.w);
}

void camera_move_freelook(Camera* cam, Window* w, CameraFreelookState* state) {
    assert(cam && w && state);
    bool mouse_held = glfwGetMouseButton(w->handle, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    if (mouse_held) {
        double mouse_x, mouse_y;
        glfwGetCursorPos(w->handle, &mouse_x, &mouse_y);
        if (state->mouse_was_held) {
            double diff_x = mouse_x - state->last_mouse_x;
            double diff_y = mouse_y - state->last_mouse_y;
            cam->rotation.yaw   += (float) diff_x / (180.0f * (float) M_PI) * state->mouse_sensitivity;
            cam->rotation.pitch += (float) diff_y / (180.0f * (float) M_PI) * state->mouse_sensitivity;
        } else
            glfwSetInputMode(w->handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        state->last_mouse_x = mouse_x;
        state->last_mouse_y = mouse_y;
    } else
        glfwSetInputMode(w->handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    state->mouse_was_held = mouse_held;

    if (glfwGetKey(w->handle, GLFW_KEY_W) == GLFW_PRESS)
        cam->position = vec3f_add(cam->position, vec3f_scale(camera_get_forward_vec(cam), state->fly_speed));
    else if (glfwGetKey(w->handle, GLFW_KEY_S) == GLFW_PRESS)
        cam->position = vec3f_sub(cam->position, vec3f_scale(camera_get_forward_vec(cam), state->fly_speed));

    if (glfwGetKey(w->handle, GLFW_KEY_D) == GLFW_PRESS)
        cam->position = vec3f_sub(cam->position, vec3f_scale(camera_get_left_vec(cam), state->fly_speed));
    else if (glfwGetKey(w->handle, GLFW_KEY_A) == GLFW_PRESS)
        cam->position = vec3f_add(cam->position, vec3f_scale(camera_get_left_vec(cam), state->fly_speed));
}
