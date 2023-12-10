add_library(common SHARED util.c io.c mat.c arena.c print.c)
target_include_directories(common PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(common PUBLIC m)

find_package(ZLIB REQUIRED)
target_link_libraries(common PRIVATE ZLIB::ZLIB)

link_libraries(common)
add_executable(test_bit_utils test_bit_utils.c)
add_test(NAME test_bit_utils COMMAND test_bit_utils)
