cmake_minimum_required(VERSION 3.10)
project(TML)

option(TML_USE_OPENGL_VERSION_330 "[Not working]Use opengl version 3.3. Else use version 4.6." no)
option(TML_DEBUG_OPENGL_CALLS "Applies error checking around opengl function calls." no)

if(TML_USE_OPENGL_VERSION_330)
    add_compile_definitions(TML_GL_VERSION_330=1)
endif()
if(!TML_DEBUG_OPENGL_CALLS)
    add_compile_definitions(NO_GL_DEBUGGING=1)
endif()

add_compile_options(-msse3 -O2)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O2")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O2")

file(MAKE_DIRECTORY build)
file(GLOB_RECURSE SRC_FILES src/*.cpp src/glad.c)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY build)
set(GLFW_LIBRARY ${PROJECT_BINARY_DIR}/external/glfw/src/libglfw3.a)

add_subdirectory(external/glfw/)

add_library(TML SHARED ${SRC_FILES})
target_link_libraries(TML ${GLFW_LIBRARY} dl pthread)
