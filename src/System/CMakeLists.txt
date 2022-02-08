cmake_minimum_required(VERSION 2.8...3.20)
project(TML-System)
set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SRC_FILES ./*.cpp)
add_library(TML-System ${SRC_FILES})
target_link_libraries(TML-System glfw webp)