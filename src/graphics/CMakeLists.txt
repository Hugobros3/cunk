find_package(GLFW3 REQUIRED)
link_libraries(glfw)

set(GLEW_USE_STATIC_LIBS true)
set(GLEW_VERBOSE true)
find_package(GLEW REQUIRED)
link_libraries(GLEW::GLEW opengl32)

add_library(graphics STATIC graphics.c)

add_executable(test_graphics test_graphics.c)
target_link_libraries(test_graphics graphics)
target_include_directories(test_graphics PRIVATE "shaders")
