cmake_minimum_required(VERSION 3.0)
project(tml-window)

file(GLOB_RECURSE SRC_FILES ./*.cpp)
add_library(tml-window ${SRC_FILES})
target_link_libraries(tml-window glfw)