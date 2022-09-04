#include "cunk/graphics.h"
#include "../common/common_private.h"

#include "GL/glew.h"

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

static void init_triangle() {
    float geometryData[] = {-0.5F, -0.5F, 0.5F, -0.5F, 0.F, 0.5F};

    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(float), geometryData, GL_STATIC_DRAW);

    //glGenVertexArrays(1, &vao);
    //glBindVertexArray(vao);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}

static float angle = 0.0;

static void draw_triangle() {
    glUseProgram(program);
    // glUniform4f(myColorLocation, 1.0F, 1.0F, 0.0F, 1.0F);

    angle = angle + 0.01f;
    GLint myPositionLocation = glGetUniformLocation(program, "myPosition");
    glUniform2f(myPositionLocation, sin(angle) * 0.4f, cos(angle) * 0.4f);

    // glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main() {
    GLFWwindow* w;
    glfwInit();
    w = glfwCreateWindow(640, 480, "glfw werks", NULL, NULL);
    glfwMakeContextCurrent(w);
    glewInit();

    const char* version = glGetString(GL_VERSION);
    const char* vendor = glGetString(GL_VENDOR);

    printf("Running on OpenGL\n");
    printf("Version: %s\n", version);
    printf("Vendor: %s\n", vendor);

    program = load_test_program();
    init_triangle();

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
