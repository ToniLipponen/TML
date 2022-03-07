cmake_minimum_required(VERSION 2.8...3.20)
project(TML)
option(TML_DEBUG_OPENGL_CALLS "Applies error checking around opengl function calls." no)
option(TML_ASSERT "Applies asserts around important functions" yes)
option(TML_BUILD_EXAMPLES "Builds some example programs" yes)
option(TML_USE_GLES "Use OpenGL ES 3.1 instead of OpenGL 4.5" no)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set some glfw options
set(GLFW_LIBRARY_TYPE OBJECT)
set(GLFW_INSTALL no)
set(GLFW_BUILD_DOCS no)
set(GLFW_BUILD_TESTS no)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)

# SSE3
if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
    if(UNIX)
        add_compile_options(-msse3)
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

file(GLOB_RECURSE SRC_FILES src/*.cpp external/incbin/incbin.c)
file(GLOB_RECURSE HEADERS_FILES include/TML/*.h)
include_directories(external/glm external/glfw/include include external res external/libwebp/src/ src/Graphics/Core/ src/Graphics/Core/Headers external/asio/include)

# Add external libraries
add_subdirectory(external/glfw/)
add_subdirectory(external/libwebp)

# Add modules
add_subdirectory(src/Audio)
add_subdirectory(src/Graphics)
add_subdirectory(src/Interface)
add_subdirectory(src/System)
add_subdirectory(src/Window)
add_subdirectory(src/Network)

add_library(TML src/TOKEN_SOURCE_FILE.cpp ${HEADER_FILES})
target_link_libraries(TML TML-Audio TML-Interface TML-Graphics TML-Window TML-System TML-Network)

set(CMAKE_INSTALL_PREFIX /usr/)
install(TARGETS TML-System TML-Graphics TML-Interface TML-Audio TML-Network TML-Window DESTINATION /usr/lib)
install(DIRECTORY include/TML DESTINATION /usr/include)

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
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libTML.a
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libTML.so
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/TML.lib)