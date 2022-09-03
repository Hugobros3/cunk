#include "cunk/graphics.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "GL/gl.h"

#include <stdio.h>

int main() {
    GLFWwindow* w;
    glfwInit();
    w = glfwCreateWindow(640, 480, "glfw werks", NULL, NULL);
    glfwMakeContextCurrent(w);
    printf("Hello, World!\n");
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
