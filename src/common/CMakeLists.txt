add_library(common STATIC util.c io.c mat.c arena.c print.c)
target_link_libraries(common PUBLIC m)

