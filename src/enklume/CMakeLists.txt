add_library(enklume SHARED nbt.c nbt_print.c)
target_link_libraries(enklume PUBLIC common)

add_executable(enklume_test nbt_test.c)
target_link_libraries(enklume_test PRIVATE enklume)
