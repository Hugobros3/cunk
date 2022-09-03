#include "cunk/graphics.h"

#include "GL/glew.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include <stdio.h>

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
    fflush(stdout);

    // glfwDestroyWindow(w);
    while (!glfwWindowShouldClose(w)) {
        glClearColor(0.0f, 0.5f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(w);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
