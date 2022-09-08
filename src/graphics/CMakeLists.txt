find_package(GLFW3 REQUIRED)
link_libraries(glfw)

link_libraries(glad)

add_library(graphics STATIC graphics.c camera.c window.c)

add_executable(test_graphics test_graphics.c)
target_link_libraries(test_graphics graphics incbin)
target_include_directories(test_graphics PRIVATE "shaders")
