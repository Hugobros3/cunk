add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC "include")
if (UNIX AND NOT APPLE)
target_link_libraries(glad dl)
endif()

