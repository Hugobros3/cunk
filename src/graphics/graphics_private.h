#ifndef CUNK_GRAPHICS_PRIVATE_H
#define CUNK_GRAPHICS_PRIVATE_H

#include "cunk/graphics.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

struct Window_ {
    GLFWwindow* handle;
    int width, height;
};

#endif
