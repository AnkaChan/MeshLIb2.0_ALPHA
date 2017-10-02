cmake_minimum_required(VERSION 2.8)

project(SphericalHarmonic2)

include_directories($ENV{MESHLIB2.0_PATH})

include_directories($ENV{EIGEN_PATH})
file(GLOB SRC
    "*.h"
    "*.cpp"
)
add_executable(SphericalHarmonic2 ${SRC})