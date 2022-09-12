add_library(common SHARED util.c io.c mat.c arena.c print.c zlib_wrap.c)
target_link_libraries(common PUBLIC m)

find_package(ZLIB REQUIRED)
target_link_libraries(common PRIVATE ZLIB::ZLIB)
