cmake_minimum_required(VERSION 3.0)
project(tml-graphics)

file(GLOB_RECURSE SRC_FILES ./*.cpp)
add_library(tml-graphics ${SRC_FILES})