cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(M2Obj)

include_directories($ENV{MESHLIB2.0_PATH})
add_executable(M2Obj main.cpp)
