#include "cunk/camera.h"

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

static Mat4f camera_rotation_matrix(const Camera* camera) {
    Mat4f matrix = identity_mat4f;
    matrix = mul_mat4f(rotate_axis_mat4f(1, camera->rotation.yaw), matrix);
    matrix = mul_mat4f(rotate_axis_mat4f(0, camera->rotation.pitch), matrix);
    return matrix;
}

Mat4f camera_get_view_mat4(const Camera* camera, size_t width, size_t height) {
    Mat4f matrix = identity_mat4f;
    matrix = mul_mat4f(translate_mat4f(vec3f_neg(camera->position)), matrix);
    matrix = mul_mat4f(camera_rotation_matrix(camera), matrix);
    float ratio = ((float) width) / ((float) height);
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

void camera_move_freelook(Camera* cam, CameraInput* input, CameraFreelookState* state) {
    assert(cam && input && state);
    if (input->mouse_held) {
        if (state->mouse_was_held) {
            double diff_x = input->mouse_x - state->last_mouse_x;
            double diff_y = input->mouse_y - state->last_mouse_y;
            cam->rotation.yaw   += (float) diff_x / (180.0f * (float) M_PI) * state->mouse_sensitivity;
            cam->rotation.pitch += (float) diff_y / (180.0f * (float) M_PI) * state->mouse_sensitivity;
        } else
            input->should_capture = true;

        state->last_mouse_x = input->mouse_x;
        state->last_mouse_y = input->mouse_y;
    } else
        input->should_capture = false;
    state->mouse_was_held = input->mouse_held;

    if (input->keys.forward)
        cam->position = vec3f_add(cam->position, vec3f_scale(camera_get_forward_vec(cam), state->fly_speed));
    else if (input->keys.back)
        cam->position = vec3f_sub(cam->position, vec3f_scale(camera_get_forward_vec(cam), state->fly_speed));

    if (input->keys.right)
        cam->position = vec3f_sub(cam->position, vec3f_scale(camera_get_left_vec(cam), state->fly_speed));
    else if (input->keys.left)
        cam->position = vec3f_add(cam->position, vec3f_scale(camera_get_left_vec(cam), state->fly_speed));
}

#include "graphics_private.h"

void gfx_camera_update(Window* w, CameraInput* input) {
    input->mouse_held = glfwGetMouseButton(w->handle, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    glfwGetCursorPos(w->handle, &input->mouse_x, &input->mouse_y);
    input->keys.forward = glfwGetKey(w->handle, GLFW_KEY_W) == GLFW_PRESS;
    input->keys.back = glfwGetKey(w->handle, GLFW_KEY_S) == GLFW_PRESS;
    input->keys.left = glfwGetKey(w->handle, GLFW_KEY_A) == GLFW_PRESS;
    input->keys.right = glfwGetKey(w->handle, GLFW_KEY_D) == GLFW_PRESS;
    if (input->should_capture)
        glfwSetInputMode(w->handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
        glfwSetInputMode(w->handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
