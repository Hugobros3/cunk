#include "graphics_private.h"

#include <stdlib.h>
#include <assert.h>

#pragma GCC diagnostic error "-Wswitch"

GLuint gfx_classify_texture(GfxTexture* t) {
    if (t->depth > 0)
        return GL_TEXTURE_3D;
    if (t->height > 0)
        return GL_TEXTURE_2D;
    assert(t->width > 0 && "empty textures are not supported");
    return GL_TEXTURE_1D;
}

typedef enum {
    CFK_FLOAT,
    CFK_INTEGER,
} GfxComponentFormatKind;

static GfxComponentFormatKind get_component_format_kind(GfxTexCompFormat cf) {
    switch (cf) {
        case GFX_TCF_U8_UNORM: return CFK_FLOAT;
        case GFX_TCF_I32: return CFK_INTEGER;
        case GFX_TCF_F32: return CFK_FLOAT;
    }
}

static GLenum kind_to_format[][4] = {
    [CFK_FLOAT] = { GL_RED, GL_RG, GL_RGB, GL_RGBA},
    [CFK_INTEGER] = { GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_RGBA_INTEGER},
};
/// Provides the 'format' parameter for texImage
static GLenum get_texture_format_format(GfxTexFormat* f) {
    return kind_to_format[get_component_format_kind(f->base)][f->num_components - 1];
}

static GLenum get_texture_format_type(GfxTexCompFormat cf) {
    switch (cf) {
        case GFX_TCF_U8_UNORM: return GL_UNSIGNED_BYTE;
        case GFX_TCF_I32: return GL_UNSIGNED_INT;
        case GFX_TCF_F32: return GL_FLOAT;
    }
}

static GLint get_texture_format_internal_format(GfxTexFormat* f) {
    // TODO: use explicitly sized formats where available
    GfxTexFormat internal = *f;
    internal.base = GFX_TCF_F32;
    return get_texture_format_format(&internal);
}

GfxTexture* gfx_create_texture(GfxCtx* ctx, unsigned width, unsigned height, unsigned depth, GfxTexFormat f) {
    GfxTexture* t = calloc(sizeof(GfxTexture), 1);
    GL_CHECK(glGenTextures(1, &t->handle), return NULL);
    t->width = width;
    t->height = height;
    t->depth = depth;
    t->format = f;
    GLuint class = gfx_classify_texture(t);
    glBindTexture(class, t->handle);
    GL_CHECK(glTexParameteri(class, GL_TEXTURE_MIN_FILTER, GL_NEAREST), );
    GL_CHECK(glTexParameteri(class, GL_TEXTURE_MAG_FILTER, GL_NEAREST), );
    return t;
}

void gfx_upload_texture(GfxTexture* t, void* data) {
    GLuint class = gfx_classify_texture(t);
    glBindTexture(class, t->handle);
    GLint internal_format = get_texture_format_internal_format(&t->format);
    GLenum pixel_data_format = get_texture_format_format(&t->format);
    GLenum pixel_data_type = get_texture_format_type(t->format.base);
    switch (class) {
        case GL_TEXTURE_1D: {
            GL_CHECK(glTexImage1D(class, 0, internal_format, t->width, 0, pixel_data_format, pixel_data_type, data), return;);
            return;
        }
        case GL_TEXTURE_2D: {
            GL_CHECK(glTexImage2D(class, 0, internal_format, t->width, t->height, 0, pixel_data_format, pixel_data_type, data), return;);
            return;
        }
        case GL_TEXTURE_3D: {
            GL_CHECK(glTexImage3D(class, 0, internal_format, t->width, t->height, t->depth, 0, pixel_data_format, pixel_data_type, data), return;);
            return;
        }
        default: assert(false);
    }
}

void gfx_destroy_texture(GfxTexture* t) {
    GL_CHECK(glDeleteTextures(1, &t->handle), return;);
    free(t);
}