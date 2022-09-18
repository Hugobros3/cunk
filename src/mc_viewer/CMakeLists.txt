add_executable(mc_viewer mc_viewer.c chunk.c chunk_mesh.c)
target_include_directories(mc_viewer PRIVATE "shaders")
target_link_libraries(mc_viewer PRIVATE graphics incbin enklume)
