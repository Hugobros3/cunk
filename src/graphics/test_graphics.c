#include "cunk/graphics.h"
#include "cunk/math.h"

#include "../common/common_private.h"
#include "../graphics/graphics_private.h"

#include "glad/glad.h"

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

#define GL_CHECK(x, f) { x; GLint errcode = glGetError(); if (errcode != GL_NO_ERROR) { fprintf(stderr, "GL error after: " #x); f; } }

static void print_shader_log(GLuint shader) {
    GLsizei info_log_length;
    GL_CHECK(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length), return);
    char* buf = calloc(info_log_length + 1, 1);
    glGetShaderInfoLog(shader, info_log_length, NULL, buf);
    buf[info_log_length] = '\0';
    fprintf(stderr, "Shader compilation log: %s\n", buf);
    free(buf);
}

static void print_program_log(GLuint program) {
    GLsizei info_log_length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
    char* buf = malloc(info_log_length + 1);
    glGetProgramInfoLog(program, info_log_length, NULL, buf);
    buf[info_log_length] = '\0';
    fprintf(stderr, "Program compilation log: %s\n", buf);
    free(buf);
}

static GLuint load_test_program() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vss [] = { test_vs_data };
    glShaderSource(vertexShader, 1, vss, &test_vs_size);
    GL_CHECK(glCompileShader(vertexShader), /* */);
    print_shader_log(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fss[] = { test_fs_data };
    glShaderSource(fragmentShader, 1, fss, &test_fs_size);
    GL_CHECK(glCompileShader(fragmentShader), /* */ );
    print_shader_log(fragmentShader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    print_program_log(program);
    return program;

    failure:
    abort();
}

static GLuint program;
static GLuint vbo;

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
    glGenBuffers(1, &vbo);

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

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(geometryData) * num_cubes, geom_tmp, GL_STATIC_DRAW);
    free(geom_tmp);
}

static Window window;

struct {
    bool wireframe;
    int render_mode;
} config;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
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

static float angle = 0.f;

static void draw_triangle() {
    glUseProgram(program);

    if (config.wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // angle = angle + 0.01f;

    Mat4f matrix = identity_mat4f;
    matrix = mul_mat4f(rotate_axis_mat4f(1, angle), matrix);
    matrix = mul_mat4f(translate_mat4f((Vec3f) { 0.f, 0.f, -3.f }), matrix);

    matrix = mul_mat4f(camera_get_view_mat4(&camera, &window), matrix);
    // float ratio = ((float) window.width) / ((float) window.height);
    // matrix = mul_mat4f(perspective_mat4f(ratio, 90.0f, 0.1f, 100.f), matrix);

    glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_FALSE, matrix.arr);

    GLsizei num_attributes;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &num_attributes);

    for (int i = 0; i < num_attributes; i++) {
        GLsizei len;
        char name[32];
        GLsizei size;
        GLenum type;
        glGetActiveAttrib(program, i, 32, &len, &size, &type, (char*) &name);
        int loc = glGetAttribLocation(program, name);
        // fprintf(stderr, "attribute %s %d in slot %d location %d\n", name, size, i, loc);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 3dlabs's driver seems unhappy drawing without a vertex array bound
    glEnableClientState(GL_VERTEX_ARRAY);
    float dummy[] = { 0 };
    glVertexPointer(3, GL_FLOAT, sizeof(float) * 1, dummy );

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(glGetAttribLocation(program, "vertexIn"));
    glVertexAttribPointer(glGetAttribLocation(program, "vertexIn"), 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, NULL);

    glUniform1i(glGetUniformLocation(program, "render_mode"), config.render_mode);

    glEnableVertexAttribArray(glGetAttribLocation(program, "texCoordIn"));
    glVertexAttribPointer(glGetAttribLocation(program, "texCoordIn"), 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*) (sizeof(float) * 3));

    GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 36 * num_cubes), return);
}

int main() {
    glfwInit();
    window.handle = glfwCreateWindow(640, 480, "glfw werks", NULL, NULL);
    glfwMakeContextCurrent(window.handle);
    glfwSetKeyCallback(window.handle, key_callback);

    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    const char* version = glGetString(GL_VERSION);
    const char* vendor = glGetString(GL_VENDOR);

    printf("Running on OpenGL\n");
    printf("Version: %s\n", version);
    printf("Vendor: %s\n", vendor);

    program = load_test_program();
    init_cube();

    fflush(stdout);
    fflush(stderr);

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window.handle)) {
        camera_move_freelook(&camera, &window, &camera_state);

        glfwGetFramebufferSize(window.handle, &window.width, &window.height);
        glViewport(0, 0, window.width, window.height);

        glClearColor(0.0f, 0.5f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw_triangle();

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
