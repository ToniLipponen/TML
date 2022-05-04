cmake_minimum_required(VERSION 3.0)
project(glad)

include_directories(include)
add_library(glad OBJECT src/gl.c)