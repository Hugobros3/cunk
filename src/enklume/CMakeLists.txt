add_library(enklume SHARED nbt.c nbt_print.c)

add_executable(enklume_test nbt_test.c)
target_link_libraries(enklume_test enklume common)
