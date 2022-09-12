add_library(enklume SHARED nbt.c nbt_print.c)
target_link_libraries(enklume common)

add_executable(enklume_test nbt_test.c)
target_link_libraries(enklume_test enklume)
