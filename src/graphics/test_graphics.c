#include "nasl/nasl.h"

#include "cunk/graphics.h"
#include "cunk/print.h"
#include "cunk/io.h"
#include "cunk/camera.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"


#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

static Window* window;
static GfxCtx* ctx;
static GfxShader* shader;
static GfxBuffer* buffer;
static GfxTexture* tex;

GLFWwindow* gfx_get_glfw_handle(Window*);

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

struct {
    bool wireframe, face_culling, depth_testing;
    bool finish, vsync;
    int render_mode;
    int num_cubes;
} config = {
    .depth_testing = true,
    .num_cubes = 64,
};

static float frand() {
    int r = rand();
    double rd = (double) r;
    rd /= (double) RAND_MAX;
    return ((float) rd);
}

static void init_cubes() {
    if (buffer)
        gfx_destroy_buffer(buffer);
    
    fprintf(stderr, "%d cubes, totalling %d KiB of data\n", config.num_cubes, config.num_cubes * sizeof(geometryData) / 1024);
    fflush(stderr);
    
    size_t buffer_size = sizeof(geometryData) * config.num_cubes;
    void* geom_tmp = malloc(buffer_size);
    float* fp_tmp = geom_tmp;
    for (size_t i = 0; i < config.num_cubes; i++) {
        vec3 offset = { frand(), frand(), frand() };
        offset = vec3_sub(vec3_scale(offset, 128.0f), vec3_ctor(64.0f));
        for (int j = 0; j < 36; j++) {
            *(fp_tmp++) = geometryData[j * 5 + 0] + offset.x;
            *(fp_tmp++) = geometryData[j * 5 + 1] + offset.y;
            *(fp_tmp++) = geometryData[j * 5 + 2] + offset.z;
            *(fp_tmp++) = geometryData[j * 5 + 3];
            *(fp_tmp++) = geometryData[j * 5 + 4];
        }
    }

    buffer = gfx_create_buffer(ctx, buffer_size);
    gfx_copy_to_buffer(buffer, geom_tmp, buffer_size);
    free(geom_tmp);
}

static void init_tex() {
    GfxTexFormat f = { .base = GFX_TCF_UNORM8, .num_components = 3 };
    unsigned char tex_data[256 * 3];
    for (size_t x = 0; x < 16; x++) {
        for (size_t y = 0; y < 16; y++) {
            tex_data[(x * 16 + y) * 3 + 0] = 255;
            tex_data[(x * 16 + y) * 3 + 1] = 48;
            tex_data[(x * 16 + y) * 3 + 2] = 48;
        }
    }
    for (size_t x = 0; x < 16; x++) {
        int y;
        y = 0;
        tex_data[(y * 16 + x) * 3 + 0] = 255;
        tex_data[(y * 16 + x) * 3 + 1] = 255;
        tex_data[(y * 16 + x) * 3 + 2] = 255;
        y = 8;
        tex_data[(y * 16 + x) * 3 + 0] = 255;
        tex_data[(y * 16 + x) * 3 + 1] = 255;
        tex_data[(y * 16 + x) * 3 + 2] = 255;
    }
    int x = 0, by = 0;
    for (size_t y = by; y < by + 8; y++) {
        tex_data[(y * 16 + x) * 3 + 0] = 255;
        tex_data[(y * 16 + x) * 3 + 1] = 255;
        tex_data[(y * 16 + x) * 3 + 2] = 255;
    }
    x = 8; by = 8;
    for (size_t y = by; y < by + 8; y++) {
        tex_data[(y * 16 + x) * 3 + 0] = 255;
        tex_data[(y * 16 + x) * 3 + 1] = 255;
        tex_data[(y * 16 + x) * 3 + 2] = 255;
    }
    tex = gfx_create_texture(ctx, 16, 16, 0, f);
    gfx_upload_texture(tex, tex_data);
}

static void key_callback(GLFWwindow* handle, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS)
        return;

    switch (key) {
        case GLFW_KEY_1:
            config.render_mode += 1;
            config.render_mode %= 3;
            break;
        case GLFW_KEY_2:
            config.wireframe ^= true;
            break;
        case GLFW_KEY_3:
            config.face_culling ^= true;
            break;
        case GLFW_KEY_4:
            config.depth_testing ^= true;
            break;
        case GLFW_KEY_5:
            config.finish ^= true;
            break;
        case GLFW_KEY_6:
            config.vsync ^= true;
            break;
        case GLFW_KEY_MINUS:
            config.num_cubes = config.num_cubes > 1 ? config.num_cubes / 2 : 1;
            init_cubes();
            break;
        case GLFW_KEY_EQUAL:
            config.num_cubes = config.num_cubes * 2;
            init_cubes();
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
static CameraInput camera_input;

void gfx_camera_update(Window* w, CameraInput* input);

static void draw_cubes() {
    gfx_cmd_resize_viewport(ctx, window);
    gfx_cmd_clear(ctx);
    GfxState state = {
        .wireframe = config.wireframe,
        .face_culling = config.face_culling,
        .depth_testing = config.depth_testing,
    };
    gfx_cmd_set_draw_state(ctx, state);
    gfx_cmd_use_shader(ctx, shader);

    mat4 matrix = identity_mat4;
    size_t width, height;
    gfx_get_window_size(window, &width, &height);
    matrix = mul_mat4(camera_get_view_mat4(&camera, width, height), matrix);

    for (int i = 0; i < 16; i++) {
        char* str = format_string("myMatrix._0._%d", i);
        gfx_cmd_set_shader_extern(ctx, str, &matrix.arr[i]);
        free(str);
    }
    //gfx_cmd_set_shader_extern(ctx, "render_mode", &config.render_mode);

    gfx_cmd_set_shader_extern(ctx, "the_texture", tex);

    gfx_cmd_set_vertex_input(ctx, "vertexIn", buffer, 3, sizeof(float) * 5, 0);
    gfx_cmd_set_vertex_input(ctx, "texCoordIn", buffer, 2, sizeof(float) * 5, sizeof(float) * 3);

    gfx_cmd_draw_arrays(ctx, 0, 36 * config.num_cubes);
}

#define SMOOTH_FPS_ACC_FRAMES 32
static double last_frames_times[SMOOTH_FPS_ACC_FRAMES] = { 0 };
static int frame = 0;

int main() {
    window = gfx_create_window("Hello", 640, 480, &ctx);
    glfwSetKeyCallback(gfx_get_glfw_handle(window), key_callback);

    char* test_vs, *test_fs;
    size_t test_vs_size, test_fs_size;
    read_file("../shaders/test.vert.glsl", &test_vs_size, &test_vs);
    read_file("../shaders/test.frag.glsl", &test_fs_size, &test_fs);
    shader = gfx_create_shader(ctx, test_vs, test_fs);
    init_cubes();
    init_tex();

    fflush(stdout);
    fflush(stderr);

    glfwSwapInterval(config.vsync ? 1 : 0);

    while (!glfwWindowShouldClose(gfx_get_glfw_handle(window))) {
        double then = glfwGetTime();
        gfx_camera_update(window, &camera_input);
        camera_move_freelook(&camera, &camera_input, &camera_state);

        draw_cubes();

        glfwSwapBuffers(gfx_get_glfw_handle(window));
        if (config.finish)
            gfx_wait_for_idle();

        double now = glfwGetTime();
        double frametime = 0.0;
        int available_frames = frame > SMOOTH_FPS_ACC_FRAMES ? SMOOTH_FPS_ACC_FRAMES : frame;
        int j = frame % SMOOTH_FPS_ACC_FRAMES;
        for (int i = 0; i < available_frames; i++) {
            double frame_start = last_frames_times[j];
            j = (j + 1) % SMOOTH_FPS_ACC_FRAMES;
            double frame_end;
            if (i + 1 == available_frames)
                frame_end = now;
            else
                frame_end = last_frames_times[j];
            double delta = frame_end - frame_start;
            frametime += delta;
        }

        frametime /= (double) available_frames;

        double fps = 1.0 / frametime;
        int ifps = (int) fps;

        const char* t = format_string("Cubes: %d, FPS: %d", config.num_cubes, ifps);
        //glfwSetWindowTitle(get_glfw_handle(window), t);
        free(t);

        last_frames_times[frame % SMOOTH_FPS_ACC_FRAMES] = now;
        frame++;

        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
