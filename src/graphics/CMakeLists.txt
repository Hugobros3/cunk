find_package(glfw3 REQUIRED)
link_libraries(glfw)

add_library(graphics SHARED graphics.c shader.c camera.c window.c)
target_link_libraries(graphics PRIVATE glad)

add_executable(test_graphics test_graphics.c)
target_link_libraries(test_graphics PRIVATE graphics incbin)
target_include_directories(test_graphics PRIVATE "shaders")

