cmake_minimum_required(VERSION 3.25)

project(argparse-cpp LANGUAGES CXX)

option(BUILD_TESTS "" ON)
option(BUILD_EXAMPLES "" ON)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

include(FindDeps)

include_directories(${CMAKE_SOURCE_DIR}/include)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

if (BUILD_TESTS)
    add_subdirectory(tests)
endif ()
