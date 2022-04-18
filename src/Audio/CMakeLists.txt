cmake_minimum_required(VERSION 3.0)
project(tml-audio)

file(GLOB_RECURSE SRC_FILES ./*.cpp)
add_library(tml-audio ${SRC_FILES})