add_library(enklume nbt.c nbt_print.c enklume.c)
target_link_libraries(enklume PUBLIC cunk_common)

add_executable(nbt_test nbt_test.c)
target_link_libraries(nbt_test PRIVATE enklume)

add_executable(enklume_test enklume_test.c)
target_link_libraries(enklume_test PRIVATE enklume)
