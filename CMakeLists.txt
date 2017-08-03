cmake_minimum_required(VERSION 3.8)
project(3_in_1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Graph.cpp Graph.h)
add_executable(3_in_1 ${SOURCE_FILES})