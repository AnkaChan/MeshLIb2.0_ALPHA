cmake_minimum_required(VERSION 2.8)

project(FLANNDemo)

include_directories($ENV{MESHLIB2.0_PATH} $ENV{MESHLIB2.0_PATH}/MeshLib/3rdParty/FLANN/include)

link_directories($ENV{MESHLIB2.0_PATH}/MeshLib/3rdParty/FLANN/bin)
link_libraries($ENV{MESHLIB2.0_PATH}/MeshLib/3rdParty/FLANN/lib)

include_directories($ENV{EIGEN_PATH})
file(GLOB SRC
    "*.h"
    "*.cpp"
)
add_executable(FLANNDemo ${SRC})