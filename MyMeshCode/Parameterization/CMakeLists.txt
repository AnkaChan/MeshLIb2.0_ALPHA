
cmake_minimum_required(VERSION 2.8)

file(GLOB SRC
    "*.h"
)

project(Test)
include_directories($ENV{MESHLIB2.0_PATH})
add_executable(Test ${SRC} test_main.cpp)

project(Parameterization)
include_directories($ENV{MESHLIB2.0_PATH})
add_executable(Parameterization ${SRC} main.cpp)
