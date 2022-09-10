#include "graphics_private.h"

#include <assert.h>
#include <string.h>

Window* create_window(const char* title, int width, int height, GfxCtx** ctx) {
    Window* win = calloc(1, sizeof(Window));
    
    glfwInit();
    win->handle = glfwCreateWindow(width, height, "k", NULL, NULL);
    win->width = width;
    win->height = height;
    glfwMakeContextCurrent(win->handle);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    
    if (ctx) {
        *ctx = calloc(1, sizeof(GfxCtx));
        (*ctx)->window = win;
        
        const char* version = glGetString(GL_VERSION);
        const char* vendor = glGetString(GL_VENDOR);

        printf("Running on OpenGL\n");
        printf("Version: %s\n", version);
        printf("Vendor: %s\n", vendor);

        if (strcmp(vendor, "3Dlabs") == 0)
            (*ctx)->hacks.broken_3dlabs_driver = true;
    }
    
    return win;
}

GLFWwindow* get_glfw_handle(Window* w) { return w->handle; }

GfxBuffer* create_buffer(GfxCtx* ctx, size_t size) {
    GfxBuffer* buf = calloc(1, sizeof(GfxBuffer));
    buf->size = size;
    glGenBuffers(1, &buf->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, buf->vbo);
    return buf;
}

void copy_to_buffer(GfxBuffer* buffer, void* d, size_t s) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
    glBufferData(GL_ARRAY_BUFFER, s, d, GL_STATIC_DRAW);
}

void gfx_cmd_resize_viewport(GfxCtx* ctx, Window* window) {
    glfwGetFramebufferSize(get_glfw_handle(window), &window->width, &window->height);
    glViewport(0, 0, window->width, window->height);
}

void gfx_cmd_clear(GfxCtx* ctx) {
    glClearColor(0.0f, 0.5f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gfx_cmd_use_shader(GfxCtx* ctx, GfxShader* shader) {
    glUseProgram(shader->program);
    ctx->shader = shader;
}

void gfx_cmd_set_draw_fill_state(GfxCtx* ctx, bool filled) {
    if (filled)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // TODO:
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void gfx_cmd_set_shader_extern(GfxCtx* ctx, const char* name, void* data) {
    GLint num_active_uniforms;
    glGetProgramiv(ctx->shader->program, GL_ACTIVE_UNIFORMS, &num_active_uniforms);
    for (int i = 0; i < num_active_uniforms; i++) {
        char uname[32];
        int size;
        int len;
        GLenum type;
        GL_CHECK(glGetActiveUniform(ctx->shader->program, i, 32, &len, &size, &type, uname), return);
        int loc = glad_glGetUniformLocation(ctx->shader->program, name);
        if (strcmp(name, uname) == 0) {
            switch(type) {
                case GL_INT: GL_CHECK(glUniform1i(loc, *((int*) data)), return); break;
                case GL_FLOAT_MAT4: GL_CHECK(glUniformMatrix4fv(loc, 1, GL_FALSE, data), return); break;
                default: abort(); // todo
            }
            return;
        }
    }
    printf("could not find shader exterm %s amongst %d\n", name, num_active_uniforms);
}

void gfx_cmd_set_vertex_input(GfxCtx* ctx, const char* name, GfxBuffer* buf, int components, size_t stride, size_t offset) {
    GLint location = glGetAttribLocation(ctx->shader->program, name);
    assert(location >= 0);

    glBindBuffer(GL_ARRAY_BUFFER, buf->vbo);

    if (ctx->hacks.broken_3dlabs_driver) {
        // 3dlabs's driver seems unhappy drawing without a vertex array bound
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, sizeof(float) * 1, 0);
    }

    glEnableVertexAttribArray(location);
    GL_CHECK(glVertexAttribPointer(location, components, GL_FLOAT, GL_FALSE, stride, offset), return);
}

void gfx_cmd_draw_arrays(GfxCtx* ctx, size_t start, size_t num) {
    GL_CHECK(glDrawArrays(GL_TRIANGLES, (GLint) start, (GLint) num), return);
}

