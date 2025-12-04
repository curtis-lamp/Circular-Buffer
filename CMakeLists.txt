cmake_minimum_required(VERSION 4.2)
project(CircularBuffer CXX)

set(CMAKE_CXX_STANDARD 17)
enable_testing()

# Include Conan's generated toolchain file
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build")
include(${CMAKE_CURRENT_SOURCE_DIR}/build/conan_toolchain.cmake)

add_library(
    circular_buffer
    STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/circular_buffer.cpp"
)
target_include_directories(
    circular_buffer
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_subdirectory(tests)
