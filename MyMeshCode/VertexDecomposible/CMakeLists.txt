
cmake_minimum_required(VERSION 2.8)
file(GLOB INCLUDEFILES
    "*.h"
)

project(VertexDecomposible)

include_directories($ENV{MESHLIB2.0_PATH})
add_executable(VertexDecomposible ${INCLUDEFILES} C2DMesh.cpp main.cpp)

project(TestC2DMesh)

include_directories($ENV{MESHLIB2.0_PATH})
add_executable(TestC2DMesh ${INCLUDEFILES} C2DMesh.cpp TestC2DMesh.cpp)