cmake_minimum_required(VERSION 2.8...3.20)
project(TML)
option(TML_DEBUG_OPENGL_CALLS "Applies error checking around opengl function calls." no)
option(TML_ASSERT "Applies asserts around important functions" yes)
option(TML_MAKE_STATIC "Compiles TML as a static library" yes)
option(TML_BUILD_EXAMPLES "Builds some example programs" yes)
option(TML_USE_GLES "Use OpenGL ES 3.1 instead of OpenGL 4.5" no)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set some glfw options
set(GLFW_LIBRARY_TYPE OBJECT)
set(GLFW_INSTALL no)
set(GLFW_BUILD_DOCS no)
set(GLFW_BUILD_TESTS no)

set(CMAKE_CXX_STANDARD 14)

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

add_subdirectory(external/glfw/)
add_subdirectory(external/libwebp)
include_directories(external/glm external/glfw/include include external res external/libwebp/src/)

if(TML_MAKE_STATIC)
    add_library(TML STATIC ${SRC_FILES})
else()
    add_library(TML SHARED ${SRC_FILES})
endif()

if(WIN32)
    target_compile_features(TML PUBLIC cxx_std_14)

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(TML INTERFACE -msse3)
    else() # MSVC
        target_compile_options(TML INTERFACE /arch:SSE3)
    endif()

    target_link_libraries(TML glfw webp)
elseif(UNIX)
    set(CMAKE_INSTALL_PREFIX /usr)
    target_compile_features(TML PUBLIC cxx_std_14)
    target_compile_options(TML INTERFACE -msse3)
    target_link_libraries(TML glfw webp dl pthread)
    install(TARGETS ${PROJECT_NAME} DESTINATION /usr/lib)
    install(DIRECTORY include/TML DESTINATION /usr/include)
endif()

target_include_directories(TML PRIVATE src/Graphics/Core/Headers external/glfw/src)

if(TML_BUILD_EXAMPLES)
    add_subdirectory(examples/example1/)
    add_subdirectory(examples/example2/)
    add_subdirectory(examples/example3/)
    add_subdirectory(examples/example4/)
    add_subdirectory(examples/raycasting/)
    add_subdirectory(examples/GUI_TESTING)
endif()
