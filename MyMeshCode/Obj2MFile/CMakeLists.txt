cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(Obj2M)

include_directories($ENV{MESHLIB2.0_PATH})
add_executable(main main.cpp)
