#ifndef CUNK_GRAPHICS_PRIVATE_H
#define CUNK_GRAPHICS_PRIVATE_H

#include "cunk/graphics.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "glad/glad.h"

#include <stdio.h>
#include <stdlib.h>

struct Window_ {
    GLFWwindow* handle;
    const char* title;
    int width, height;
};

struct GfxCtx_ {
    Window* window;
    GfxShader* shader;
    struct {
        bool broken_3dlabs_driver;
    } hacks;
};

struct GfxShader_ {
    GLuint vertex, fragment, program;
};

struct GfxBuffer_ {
    size_t size;
    GLint vbo;
};

#define GL_CHECK(x, f) { x; GLint errcode = glGetError(); if (errcode != GL_NO_ERROR) { fprintf(stderr, "GL error after: " #x "\n"); f; } }

#endif
