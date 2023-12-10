#include "graphics_private.h"

#include <string.h>

Window* gfx_create_window(const char* title, int width, int height, GfxCtx** ctx) {
    Window* win = calloc(1, sizeof(Window));

    glfwInit();
    win->handle = glfwCreateWindow(width, height, "k", NULL, NULL);
    win->width = width;
    win->height = height;
    glfwMakeContextCurrent(win->handle);
    glewInit();

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

void gfx_get_window_size(Window* win, size_t* out_width, size_t* out_height) {
    int w, h;
    glfwGetWindowSize(win->handle, &w, &h);
    *out_width = w;
    *out_height = h;
}

GLFWwindow* gfx_get_glfw_handle(Window* w) { return w->handle; }
