cmake_minimum_required(VERSION 3.13)
project(cunk C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS ON)
#set(CMAKE_C_STANDARD_REQUIRED ON)

include(CTest)

option(BUILD_SHARED_LIBS "Whether to use shared objects. Defaults to OFF because Windows DLL hell" OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(nasl)

include_directories("include")
add_subdirectory("src")
