cmake_minimum_required(VERSION 3.13)
project(cunk C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS ON)
#set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories("include")
include_directories("thirdparty")

add_subdirectory("src")
