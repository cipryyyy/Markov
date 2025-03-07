cmake_minimum_required(VERSION 3.16)
project(Markov VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "build type" FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug mode")
    add_compile_options(-O0 -g3 -ggdb -Wall -Wextra -Wpedantic)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Release mode")
    add_compile_options(-O3 -march=native -Wall -Wextra -Wpedantic)
endif()

include_directories(libs)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(Markov ${SOURCES})
