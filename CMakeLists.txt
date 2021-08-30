cmake_minimum_required(VERSION 3.10)
project(TML)

file(MAKE_DIRECTORY build)
file(GLOB_RECURSE SRC_FILES src/*.cpp src/glad.c)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY build)
set(GLFW_LIBRARY ${PROJECT_BINARY_DIR}/external/glfw/src/libglfw3.a)

add_subdirectory(external/glfw)

add_library(TML STATIC ${SRC_FILES} include/GlDebug.h src/GLDebug.cpp)
target_link_libraries(TML glfw)
