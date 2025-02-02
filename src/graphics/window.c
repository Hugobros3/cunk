#include "graphics_private.h"

#include "cunk/camera.h"

#include <string.h>
#include <assert.h>

static GLVersion parse_gl_version(const char* version) {
    size_t version_l = strlen(version);
    char* version_copy = calloc(version_l + 1, 1);
    memcpy(version_copy, version, version_l);
    char* major_str = strtok(version_copy, ".");
    int major = strtol(major_str, NULL, 10);
    char* minor_str = strtok(NULL, ".");
    int minor = strtol(minor_str, NULL, 10);
    free(version_copy);
    return (GLVersion) { .major = major, .minor = minor };
}

static bool is_ext_supported(const char* name) {
    const char* exts = glGetString(GL_EXTENSIONS);
    size_t exts_l = strlen(exts);
    char* exts_copy = calloc(exts_l + 1, 1);
    memcpy(exts_copy, exts, exts_l);

    const char* ext = strtok(exts_copy, " ");
    assert(ext);
    do {
        if (strcmp(name, ext) == 0) {
            free(exts_copy);
            return true;
        }

        ext = strtok(NULL, " ");
    } while(ext);

    free(exts_copy);
    return false;
}

static void debug_output_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar* message, const void* uptr) {
    fprintf(stderr, "Error: %s\n", message);
}

Window* gfx_create_window(const char* title, int width, int height, GfxCtx** ctx) {
    Window* win = calloc(1, sizeof(Window));

    glfwInit();
    win->handle = glfwCreateWindow(width, height, "k", NULL, NULL);
    win->width = width;
    win->height = height;
    glfwMakeContextCurrent(win->handle);
    glewInit();

    assert(ctx);
    *ctx = calloc(1, sizeof(GfxCtx));
    (*ctx)->window = win;

    const char* version = glGetString(GL_VERSION);
    const char* vendor = glGetString(GL_VENDOR);

    printf("Running on OpenGL\n");
    printf("Version: %s\n", version);
    printf("Vendor: %s\n", vendor);

    (*ctx)->version = parse_gl_version(version);
    if (is_ext_supported("GL_ARB_debug_output")) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback((GLDEBUGPROC) debug_output_callback, 0);
    }

    if (strcmp(vendor, "3Dlabs") == 0)
        (*ctx)->hacks.broken_3dlabs_driver = true;

    return win;
}

void gfx_get_window_size(Window* win, size_t* out_width, size_t* out_height) {
    int w, h;
    glfwGetWindowSize(win->handle, &w, &h);
    *out_width = w;
    *out_height = h;
}

GLFWwindow* gfx_get_glfw_handle(Window* w) { return w->handle; }
