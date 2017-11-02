cmake_minimum_required(VERSION 2.8)

project(TetEdgeStar)

include_directories($ENV{MESHLIB2.0_PATH})

include_directories($ENV{EIGEN_PATH})
file(GLOB SRC
    "*.h"
    "*.cpp"
	"$ENV{MESHLIB2.0_PATH}/MeshLib/core/viewer/SimpleTetViewer.cpp"
)

add_executable(TetEdgeStar ${SRC})