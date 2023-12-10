#ifndef CUNK_GRAPHICS_PRIVATE_H
#define CUNK_GRAPHICS_PRIVATE_H

#include "cunk/graphics.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "GL/glew.h"

#include <stdio.h>
#include <stdlib.h>

struct Window_ {
    GLFWwindow* handle;
    const char* title;
    int width, height;
};

typedef struct {
    int major, minor;
} GLVersion;

struct GfxCtx_ {
    Window* window;
    GLVersion version;
    GfxShader* shader;
    struct {
        bool broken_3dlabs_driver;
    } hacks;
};

struct GfxShader_ {
    GLuint vertex, fragment, program;
    GLuint* texture_slots;
};

struct GfxBuffer_ {
    size_t size;
    GLuint vbo;
};

struct GfxTexture_ {
    GfxCtx* ctx;
    GLuint handle;
    size_t width, height, depth;
    GfxTexFormat format;
};

#define GL_CHECK(x, f) { x; GLint errcode = glGetError(); if (errcode != GL_NO_ERROR) { fprintf(stderr, "GL error after: " #x "\n"); f; } }

GLuint gfx_classify_texture(GfxTexture* t);

#endif
