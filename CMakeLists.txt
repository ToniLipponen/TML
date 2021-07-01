cmake_minimum_required(VERSION 3.10)
project(TML)

file(GLOB_RECURSE SRC_FILES src/*.cpp src/glad.c)
add_subdirectory(external/glfw)
add_library(TML ${SRC_FILES})
link_libraries(TML glfw)