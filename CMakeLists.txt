cmake_minimum_required(VERSION 3.10)
project(image_delivery VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Compiler Flags: ${CMAKE_CXX_FLAGS}")


add_library(common_pch STATIC pch.cpp)
target_precompile_headers(common_pch PRIVATE pch.h)

add_subdirectory(server)
add_subdirectory(client)
