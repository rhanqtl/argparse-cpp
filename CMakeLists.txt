cmake_minimum_required(VERSION 3.25)

project(argparse-cpp LANGUAGES CXX)

option(BUILD_TESTS "" ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif ()
