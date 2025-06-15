add_library(cunk_common SHARED util.c io.c mat.c arena.c print.c zlib_wrap.c)
target_include_directories(cunk_common PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(cunk_common PUBLIC m nasl)
target_include_directories(cunk_common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

find_package(ZLIB REQUIRED)
target_link_libraries(cunk_common PRIVATE ZLIB::ZLIB)

link_libraries(cunk_common)
add_executable(test_common_misc test_common_misc.c)