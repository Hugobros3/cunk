#include "cunk/graphics.h"
#include "cunk/math.h"

#include "../common/common_private.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#define INCBIN_PREFIX
#define INCBIN_STYLE INCBIN_STYLE_SNAKE
#include "incbin.h"

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

INCTXT(test_fs, "test.fs");
INCTXT(test_vs, "test.vs");

static Window* window;
static GfxCtx* ctx;
static GfxShader* shader;
static GfxBuffer* buffer;

GLFWwindow* get_glfw_handle(Window*);

float geometryData[] = {
    -1.0, -1.0, -1.0,   0.0, 0.0,
    -1.0,  1.0,  1.0,   1.0, 1.0,
    -1.0,  1.0, -1.0,   0.0, 1.0,
    -1.0,  1.0,  1.0,   1.0, 1.0,
    -1.0, -1.0, -1.0,   0.0, 0.0,
    -1.0, -1.0,  1.0,   1.0, 0.0,

    -1.0, -1.0,  1.0,   0.0, 0.0,
    1.0,  -1.0,  1.0,   1.0, 0.0,
    1.0,   1.0,  1.0,   1.0, 1.0,
    -1.0, -1.0,  1.0,   0.0, 0.0,
    1.0,   1.0,  1.0,   1.0, 1.0,
    -1.0,  1.0,  1.0,   0.0, 1.0,

    1.0,  -1.0, -1.0,   1.0, 0.0,
    1.0,   1.0, -1.0,   1.0, 1.0,
    1.0,   1.0,  1.0,   0.0, 1.0,
    1.0,  -1.0, -1.0,   1.0, 0.0,
    1.0,   1.0,  1.0,   0.0, 1.0,
    1.0,  -1.0,  1.0,   0.0, 0.0,

    -1.0, -1.0, -1.0,   1.0, 0.0,
    1.0,   1.0, -1.0,   0.0, 1.0,
    1.0,  -1.0, -1.0,   0.0, 0.0,
    -1.0, -1.0, -1.0,   1.0, 0.0,
    -1.0,  1.0, -1.0,   1.0, 1.0,
    1.0,   1.0, -1.0,   0.0, 1.0,

    -1.0,  1.0, -1.0,   0.0, 1.0,
    1.0,   1.0,  1.0,   1.0, 0.0,
    1.0,   1.0, -1.0,   1.0, 1.0,
    -1.0,  1.0, -1.0,   0.0, 1.0,
    -1.0,  1.0,  1.0,   0.0, 0.0,
    1.0,   1.0,  1.0,   1.0, 0.0,

    -1.0, -1.0, -1.0,   0.0, 0.0,
    1.0,  -1.0, -1.0,   1.0, 0.0,
    1.0,  -1.0,  1.0,   1.0, 1.0,
    -1.0, -1.0, -1.0,   0.0, 0.0,
    1.0,  -1.0,  1.0,   1.0, 1.0,
    -1.0, -1.0,  1.0,   0.0, 1.0,
};

int num_cubes = 6400;

static float frand() {
    int r = rand();
    double rd = (double) r;
    rd /= (double) RAND_MAX;
    return ((float) rd);
}

static void init_cube() {
    void* geom_tmp = malloc(sizeof(geometryData) * num_cubes);
    float* fp_tmp = geom_tmp;
    for (size_t i = 0; i < num_cubes; i++) {
        Vec3f offset = { frand(), frand(), frand() };
        offset = vec3f_sub(vec3f_scale(offset, 128.0f), vec3f_ctor(64.0f));
        for (int j = 0; j < 36; j++) {
            *(fp_tmp++) = geometryData[j * 5 + 0] + offset.x;
            *(fp_tmp++) = geometryData[j * 5 + 1] + offset.y;
            *(fp_tmp++) = geometryData[j * 5 + 2] + offset.z;
            *(fp_tmp++) = geometryData[j * 5 + 3];
            *(fp_tmp++) = geometryData[j * 5 + 4];
        }
        // memcpy(geom_tmp + sizeof(geometryData) * i, )
    }

    buffer = create_buffer(ctx, sizeof(geometryData) * num_cubes);
    copy_to_buffer(buffer, geom_tmp, sizeof(geometryData) * num_cubes);
    free(geom_tmp);
}

struct {
    bool wireframe;
    int render_mode;
} config;

static void key_callback(GLFWwindow* handle, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS)
        return;

    switch (key) {
        case GLFW_KEY_1:
            config.wireframe ^= true;
            break;
        case GLFW_KEY_2:
            config.render_mode ^= 1;
            break;
        default: break;
    }
}

static Camera camera = {
    .position = { .x = 0, .y = 0, .z = 0 },
    .rotation = {
        .yaw = 0, .pitch = 0
    },
    .fov = 90,
};
static CameraFreelookState camera_state = {
    .fly_speed = 0.25f,
    .mouse_sensitivity = 2.0f
};

static void draw_triangle() {
    gfx_cmd_resize_viewport(ctx, window);
    gfx_cmd_clear(ctx);
    gfx_cmd_set_draw_fill_state(ctx, !config.wireframe);
    gfx_cmd_use_shader(ctx, shader);

    Mat4f matrix = identity_mat4f;
    matrix = mul_mat4f(camera_get_view_mat4(&camera, window), matrix);

    gfx_cmd_set_shader_extern(ctx, "myMatrix", &matrix.arr);
    gfx_cmd_set_shader_extern(ctx, "render_mode", &config.render_mode);

    gfx_cmd_set_vertex_input(ctx, "vertexIn", buffer, 3, sizeof(float) * 5, 0);
    gfx_cmd_set_vertex_input(ctx, "texCoordIn", buffer, 2, sizeof(float) * 5, sizeof(float) * 3);

    gfx_cmd_draw_arrays(ctx, 0, 36 * num_cubes);
}

int main() {
    window = create_window("Hello", 640, 480, &ctx);
    glfwSetKeyCallback(get_glfw_handle(window), key_callback);

    shader = create_shader(ctx, test_vs_data, test_fs_data);
    init_cube();

    fflush(stdout);
    fflush(stderr);

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(get_glfw_handle(window))) {
        camera_move_freelook(&camera, window, &camera_state);

        draw_triangle();

        glfwSwapBuffers(get_glfw_handle(window));
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
