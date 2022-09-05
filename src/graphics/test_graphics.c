#include "cunk/graphics.h"
#include "cunk/math.h"

#include "../common/common_private.h"

#include "glad/glad.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#define INCBIN_PREFIX
#define INCBIN_STYLE INCBIN_STYLE_SNAKE
#include "incbin.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

INCTXT(test_fs, "test.fs");
INCTXT(test_vs, "test.vs");

static void print_shader_log(GLuint shader) {
    GLsizei info_log_length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
    char* buf = malloc(info_log_length + 1);
    glGetProgramInfoLog(shader, info_log_length, NULL, buf);
    buf[info_log_length] = '\0';
    fprintf(stderr, "Shader compilation log: %s\n", buf);
    free(buf);
}

static void print_program_log(GLuint program) {
    GLsizei info_log_length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
    char* buf = malloc(info_log_length + 1);
    glGetShaderInfoLog(program, info_log_length, NULL, buf);
    buf[info_log_length] = '\0';
    fprintf(stderr, "Program compilation log: %s\n", buf);
    free(buf);
}

static GLuint load_test_program() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vss [] = { test_vs_data };
    glShaderSource(vertexShader, 1, vss, &test_vs_size);
    glCompileShader(vertexShader);
    print_shader_log(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fss[] = { test_fs_data };
    glShaderSource(fragmentShader, 1, (const GLchar* []) { test_fs_data }, &test_fs_size);
    glCompileShader(fragmentShader);
    print_shader_log(fragmentShader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    print_program_log(program);
    return program;
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

static void init_cube() {
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(geometryData), geometryData, GL_STATIC_DRAW);

    //glGenVertexArrays(1, &vao);
    //glBindVertexArray(vao);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, NULL);
}

static float angle = 0.0;

static void draw_triangle() {
    glUseProgram(program);

    if (fmodf(angle, 1.0f) > 0.5f)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_CULL_FACE);

    angle = angle + 0.01f;

    Mat4f matrix = identity_mat4f;
    matrix = mul_mat4f(rotate_axis_mat4f(1, angle), matrix);
    matrix = mul_mat4f(translate_mat4f((Vec3f) { 0.f, 0.f, -3.f }), matrix);
    matrix = mul_mat4f(perspective_mat4f(640/480.f, 90.0f, 0.1f, 100.f), matrix);

    glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_FALSE, matrix.arr);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(geometryData) / sizeof(float));
}

int main() {
    GLFWwindow* w;
    glfwInit();
    w = glfwCreateWindow(640, 480, "glfw werks", NULL, NULL);
    glfwMakeContextCurrent(w);

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

    while (!glfwWindowShouldClose(w)) {
        glClearColor(0.0f, 0.5f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        draw_triangle();

        glfwSwapBuffers(w);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
