cmake_minimum_required(VERSION 3.10)
project(TML)

file(MAKE_DIRECTORY build)
file(GLOB_RECURSE SRC_FILES src/*.cpp src/glad.c)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY build)
set(FREETYPE_LIBRARY ${PROJECT_BINARY_DIR}/external/freetype2/libfreetype.a)
set(GLFW_LIBRARY ${PROJECT_BINARY_DIR}/external/glfw/src/libglfw3.a)

include_directories(external/freetype2/include)
add_subdirectory(external/glfw)
#add_subdirectory(external/openal-soft)
add_subdirectory(external/freetype2)

add_library(TML STATIC ${SRC_FILES} include/GlDebug.h src/GLDebug.cpp)
target_link_libraries(TML glfw ${FREETYPE_LIBRARY} openal)
target_include_directories(TML PRIVATE external/freetype2/include)
