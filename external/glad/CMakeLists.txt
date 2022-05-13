cmake_minimum_required(VERSION 3.0)
project(glad)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
include_directories(include)
add_library(glad OBJECT src/gl.c src/egl.c)
target_include_directories(glad PUBLIC include)