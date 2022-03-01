cmake_minimum_required(VERSION 2.8...3.20)
project(TML-Graphics)

file(GLOB_RECURSE SRC_FILES ./*.cpp)
add_library(TML-Graphics ${SRC_FILES})