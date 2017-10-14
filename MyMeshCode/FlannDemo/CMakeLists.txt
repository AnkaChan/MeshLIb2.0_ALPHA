cmake_minimum_required(VERSION 2.8)

project(FLANNDemo)

include_directories($ENV{MESHLIB2.0_PATH})
include_directories("$ENV{MESHLIB2.0_PATH}/MeshLib/3rdParty/flann/src/cpp")
file(GLOB SRC
    "*.h"
    "*.cpp"
)
add_executable(FLANNDemo ${SRC})