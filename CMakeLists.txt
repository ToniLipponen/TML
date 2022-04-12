cmake_minimum_required(VERSION 3.0)
project(tml)

option(TML_DEBUG_OPENGL_CALLS "Applies error checking around opengl function calls." no)
option(TML_ASSERT "Applies asserts around important functions." yes)
option(TML_BUILD_EXAMPLES "Builds some example programs." yes)
option(TML_USE_GLES "Use OpenGL ES 3.1 instead of OpenGL 4.5." no)
option(TML_DONT_USE_DSA "Use non-DSA functions for OpenGL 4.5." no)

# Set some glfw options
set(GLFW_LIBRARY_TYPE OBJECT)
set(GLFW_INSTALL no)
set(GLFW_BUILD_DOCS no)
set(GLFW_BUILD_TESTS no)
set(GLFW_BUILD_EXAMPLES no)
if(UNIX)
    set(GLFW_BUILD_X11 yes)
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)

# Set output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set API version
set(TML_VERSION 0.0.1)

# SSE3
if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
    if(UNIX)
        add_compile_options(-msse3 -Wall)
    elseif(WIN32)
        # Add some options here
    endif()
endif()

if(NOT TML_ASSERT)
    add_compile_definitions(TML_NO_DEBUGGING=1)
endif()
if(NOT TML_DEBUG_OPENGL_CALLS)
    add_compile_definitions(TML_NO_GL_DEBUGGING=1)
endif()
if(TML_USE_GLES)
    add_compile_definitions(TML_USE_GLES=1)
endif()
if(TML_DONT_USE_DSA)
    add_compile_definitions(TML_DONT_USE_DSA=1)
endif()

file(GLOB_RECURSE HEADERS_FILES include/TML/*.h)
include_directories(include external)

# Add external libraries
add_subdirectory(external/glm)
add_subdirectory(external/glfw/)
add_subdirectory(external/Clip)
add_subdirectory(external/lunasvg)

# Add modules
add_subdirectory(src/Audio)
add_subdirectory(src/Graphics)
add_subdirectory(src/Interface)
add_subdirectory(src/System)
add_subdirectory(src/Window)
add_subdirectory(src/Network)

add_library(tml src/TOKEN_SOURCE_FILE.cpp ${HEADER_FILES})
target_link_libraries(tml tml-audio tml-interface tml-graphics tml-window tml-system tml-network)

if(UNIX)
    set(CMAKE_INSTALL_PREFIX /usr/)
    install(TARGETS tml-audio tml-interface tml-graphics tml-window tml-system tml-network DESTINATION /usr/lib)
    install(DIRECTORY include/TML DESTINATION /usr/include)
endif()

if(TML_BUILD_EXAMPLES)
    add_subdirectory(examples/Circle/)
    add_subdirectory(examples/Sprite/)
    add_subdirectory(examples/Music/)
    add_subdirectory(examples/Raycasting/)
    add_subdirectory(examples/Compute)
    add_subdirectory(examples/ImageViewer)
    add_subdirectory(examples/Http)
    add_subdirectory(examples/PostProcessing)
    add_subdirectory(examples/testing_area)
endif()

# Removing TML library file, because it doesn't actually contain anything.
# It is here just so that the TML library modules build.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libtml.a
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libtml.so
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/tml.lib)