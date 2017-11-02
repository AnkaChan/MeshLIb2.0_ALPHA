cmake_minimum_required(VERSION 2.8)

project(TextureMapDir)

include_directories($ENV{MESHLIB2.0_PATH})

include_directories($ENV{EIGEN_PATH})
file(GLOB SRC
    "*.h"
    "*.cpp"
	"$ENV{MESHLIB2.0_PATH}/MeshLib/core/bmp/RgbImage.cpp"
)
add_executable(TextureMapDir ${SRC})