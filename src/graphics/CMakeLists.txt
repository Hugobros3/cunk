add_library(graphics STATIC graphics.c shader.c camera.c window.c)

target_include_directories(graphics PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(graphics PRIVATE common)
find_package(glfw3 REQUIRED)
target_link_libraries(graphics PUBLIC glfw)
target_link_libraries(graphics PRIVATE glad)

add_executable(test_graphics test_graphics.c)
target_link_libraries(test_graphics PRIVATE graphics)
target_include_directories(test_graphics PRIVATE "shaders")
