add_library(graphics STATIC graphics.c shader.c camera.c texture.c window.c)

target_link_libraries(graphics PRIVATE common)
find_package(glfw3 REQUIRED)
target_link_libraries(graphics PUBLIC glfw)
target_link_libraries(graphics PRIVATE glad)

add_executable(test_graphics test_graphics.c)
target_link_libraries(test_graphics PRIVATE graphics)
target_include_directories(test_graphics PRIVATE "shaders")
