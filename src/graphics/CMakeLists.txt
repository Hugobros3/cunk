find_package(GLFW3 REQUIRED)
link_libraries(glfw opengl32)

add_library(graphics STATIC graphics.c)

add_executable(test_graphics test_graphics.c)
target_link_libraries(test_graphics graphics)
