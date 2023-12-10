#include "graphics_private.h"

#include <stdlib.h>

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

GfxShader* gfx_create_shader(GfxCtx* ctx, const char* vs, const char* fs) {
    GfxShader* shader = calloc(1, sizeof(GfxShader));

    shader->vertex = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vss [] = { vs };
    glShaderSource(shader->vertex, 1, vss, NULL);
    GL_CHECK(glCompileShader(shader->vertex), goto failure);
    print_shader_log(shader->vertex);

    shader->fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fss[] = { fs };
    glShaderSource(shader->fragment, 1, fss, NULL);
    GL_CHECK(glCompileShader(shader->fragment), goto failure);
    print_shader_log(shader->fragment);

    shader->program = glCreateProgram();
    glAttachShader(shader->program, shader->vertex);
    glAttachShader(shader->program, shader->fragment);
    GL_CHECK(glLinkProgram(shader->program), goto failure);
    print_program_log(shader->program);
    return shader;

    failure:
    free(shader);
    return NULL;
    // abort();
}

