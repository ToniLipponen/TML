cmake_minimum_required(VERSION 3.0)

# Set TML version
set(TML_API_VERSION_MAJOR 1)
set(TML_API_VERSION_MINOR 0)
set(TML_API_VERSION_PATCH 0)
set(TML_API_VERSION ${TML_API_VERSION_MAJOR}.${TML_API_VERSION_MINOR}.${TML_API_VERSION_PATCH})

project(tml VERSION ${TML_API_VERSION})

file(GLOB_RECURSE HEADERS_FILES include/TML/*.h)
include_directories(include external src/Headers)

include(cmake/Options.cmake)
include(cmake/CompileDefinitions.cmake)
include(cmake/GLFW.cmake)
include(cmake/External.cmake)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

if(TML_USE_SSE)
    if(MSVC)
        set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} /arch:SSE4.1)
        set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} /arch:SSE4.1)
    else()
        set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -msse4.1)
        set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -msse4.1)
    endif()
endif()

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
if(UNIX)
    set(CMAKE_INSTALL_PREFIX "/usr" CACHE STRING " " FORCE )
endif()

if(TML_BUILD_MONOLITHIC)
    include(cmake/MonolithicBuild.cmake)
else()
    include(cmake/ModularBuild.cmake)
endif()

message(STATUS "---------------- TML INFO ----------------")
message(STATUS "TML version: ${TML_API_VERSION}")
message(STATUS "TML library type: ${TML_LIBRARY_TYPE}")

if(TML_BUILD_MONOLITHIC)
    message(STATUS "TML build type: monolithic")
else()
    message(STATUS "TML build type: modular")
    message(STATUS "TML building modules: ${BUILT_MODULES_LIST}")
endif()

message(STATUS "TML SSE4.1: ${TML_USE_SSE}")
message(STATUS "------------------------------------------")