cmake_minimum_required(VERSION 3.10)
project(TML)

add_subdirectory(external/glfw)
add_subdirectory(external/openal-soft)
file(GLOB_RECURSE SRC_FILES src/*.cpp src/glad.c)

add_library(TML ${SRC_FILES})
link_libraries(TML glfw openal freetype2)
target_include_directories(TML PRIVATE external/freetype2/include)