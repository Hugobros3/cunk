#include "graphics_private.h"

#include <assert.h>
#include <string.h>

void gfx_cmd_resize_viewport(GfxCtx* ctx, Window* window) {
    glfwGetFramebufferSize(window->handle, &window->width, &window->height);
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

void gfx_cmd_set_draw_state(GfxCtx* ctx, GfxState state) {
#define toggle_state(c, s) if (c) glEnable(s); else glDisable(s);
    glPolygonMode(GL_FRONT_AND_BACK, state.wireframe ? GL_LINE : GL_FILL);
    toggle_state(state.face_culling, GL_CULL_FACE);
    toggle_state(state.depth_testing, GL_DEPTH_TEST);
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
        int loc = glGetUniformLocation(ctx->shader->program, name);
        if (strcmp(name, uname) == 0) {
            switch(type) {
                case GL_FLOAT_MAT4: GL_CHECK(glUniformMatrix4fv(loc, 1, GL_FALSE, data), return); break;
                case GL_INT: GL_CHECK(glUniform1i(loc, *((int*) data)), return); break;
                case GL_INT_VEC2: GL_CHECK(glUniform2iv(loc, 1, data), return); break;
                case GL_INT_VEC3: GL_CHECK(glUniform3iv(loc, 1, data), return); break;
                case GL_FLOAT: GL_CHECK(glUniform1f(loc, *((float*)data)), return); break;
                case GL_SAMPLER_1D:
                case GL_SAMPLER_2D:
                case GL_SAMPLER_3D:
                case GL_INT_SAMPLER_1D:
                case GL_INT_SAMPLER_2D:
                case GL_INT_SAMPLER_3D:
                {
                    GLuint slot = ctx->shader->texture_slots[i];
                    glActiveTexture(GL_TEXTURE0 + slot);
                    GfxTexture* tex = (GfxTexture*) data;
                    glBindTexture(gfx_classify_texture(tex), tex->handle);
                    GL_CHECK(glUniform1i(loc, slot), return); break;
                }
                default: abort(); // todo
            }
            return;
        }
    }
    printf("could not find shader extern %s amongst %d\n", name, num_active_uniforms);
}

void gfx_cmd_set_vertex_input(GfxCtx* ctx, const char* name, GfxBuffer* buf, int components, size_t stride, size_t offset) {
    GLint location = glGetAttribLocation(ctx->shader->program, name);
    assert(location >= 0);

    glBindBuffer(GL_ARRAY_BUFFER, buf->vbo);

    if (ctx->hacks.broken_3dlabs_driver) {
        // 3dlabs's driver seems unhappy drawing without a vertex array bound
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_SHORT, stride, 0);
    }

    glEnableVertexAttribArray(location);
    GL_CHECK(glVertexAttribPointer(location, components, GL_FLOAT, GL_FALSE, stride, (void*) offset), return);
}

void gfx_cmd_draw_arrays(GfxCtx* ctx, size_t start, size_t num) {
    GL_CHECK(glDrawArrays(GL_TRIANGLES, (GLint) start, (GLint) num), return);
}

void gfx_wait_for_idle() {
    glFinish();
}
