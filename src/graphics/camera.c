#include "graphics_private.h"

#include "GLFW/glfw3.h"

#include "stdio.h"
#include <assert.h>

Mat4f camera_get_view_mat4(const Camera* camera, const Window* window) {
    Mat4f matrix = identity_mat4f;
    matrix = mul_mat4f(translate_mat4f(camera->position), matrix);
    matrix = mul_mat4f(rotate_axis_mat4f(1, camera->rotation.yaw), matrix);
    matrix = mul_mat4f(rotate_axis_mat4f(0, camera->rotation.pitch), matrix);
    float ratio = ((float) window->width) / ((float) window->height);
    matrix = mul_mat4f(perspective_mat4f(ratio, camera->fov, 0.1f, 100.f), matrix);
    return matrix;
}

void camera_move_freelook(Camera* cam, Window* w, CameraFreelookState* state) {
    assert(cam && w && state);
    bool mouse_held = glfwGetMouseButton(w->handle, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    if (mouse_held) {
        double mouse_x, mouse_y;
        glfwGetCursorPos(w->handle, &mouse_x, &mouse_y);
        if (state->mouse_was_held) {
            double diff_x = state->last_mouse_x - mouse_x;
            double diff_y = state->last_mouse_y - mouse_y;
            cam->rotation.yaw   += (float) diff_x * 0.01f;
            cam->rotation.pitch += (float) diff_y * 0.01f;
        }
        float scale_x, scale_y;
        // glfwGetWindowContentScale(w->handle, &scale_x, &scale_y);
        // glfwSetCursorPos(w->handle, (double) ((float) w->width / scale_x) / 2, (double) w->height / 2);
        glfwSetInputMode(w->handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        state->last_mouse_x = mouse_x;
        state->last_mouse_y = mouse_y;
    }
    glfwSetInputMode(w->handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    state->mouse_was_held = mouse_held;

    if (glfwGetKey(w->handle, GLFW_KEY_W) == GLFW_PRESS) {
        cam->position = vec3f_elemwise_add(cam->position, camera_get_forward_vec(cam));
        printf("pos: %f\n", cam->position.x);
    } else if (glfwGetKey(w->handle, GLFW_KEY_S) == GLFW_PRESS) {
        cam->position = vec3f_elemwise_add(cam->position, vec3f_elemwise_neg(camera_get_forward_vec(cam)));
        printf("pos: %f\n", cam->position.x);
    }
}

Vec3f camera_get_forward_vec(const Camera* cam) {
    Vec4f initial_forward = { .z = 1, .w = 1 };
    Mat4f matrix = identity_mat4f;
    matrix = mul_mat4f(rotate_axis_mat4f(0, -cam->rotation.pitch), matrix);
    matrix = mul_mat4f(rotate_axis_mat4f(1, -cam->rotation.yaw), matrix);
    Vec4f result = mul_mat4f_vec4f(matrix, initial_forward);
    printf("hi %f %f %f %f\n", result.x, result.y, result.z, result.w);
    printf("ho %f %f %f %f\n", vec3f_elemwise_scale(result.xyz, 1.0f / result.w).x, result.y, result.z, result.w);
    return vec3f_elemwise_scale(result.xyz, 1.0f / result.w);
}
