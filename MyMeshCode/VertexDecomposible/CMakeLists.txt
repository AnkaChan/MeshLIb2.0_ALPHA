
cmake_minimum_required(VERSION 2.8)

project(Parameterization)

include_directories($ENV{MESHLIB2.0_PATH})

file(GLOB SRC
    "*.h"
    "*.cpp"
)
add_executable(Parameterization ${SRC})