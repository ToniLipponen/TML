cmake_minimum_required(VERSION 3.0)

# Set TML version
set(TML_API_VERSION_MAJOR 1)
set(TML_API_VERSION_MINOR 0)
set(TML_API_VERSION_PATCH 0)
set(TML_API_VERSION ${TML_API_VERSION_MAJOR}.${TML_API_VERSION_MINOR}.${TML_API_VERSION_PATCH})

project(tml VERSION ${TML_API_VERSION})

file(GLOB_RECURSE HEADERS_FILES include/TML/*.h)
include_directories(include external src/Headers external/glm-0.9.9.8/)

include(cmake/Options.cmake)
include(cmake/CompileDefinitions.cmake)
include(cmake/GLFW.cmake)
include(cmake/External.cmake)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

if(UNIX)
    set(CMAKE_INSTALL_PREFIX "/usr" CACHE STRING " " FORCE )
endif()


message(STATUS "---------------TML INFO----------------")
message(STATUS "TML version: ${TML_API_VERSION}")
message(STATUS "TML type: ${TML_LIBRARY_TYPE}")
message(STATUS "TML monolithic: ${TML_BUILD_MONOLITHIC}")
message(STATUS "---------------------------------------")

if(TML_BUILD_MONOLITHIC)
    include(cmake/MonolithicBuild.cmake)
else()
    include(cmake/ModularBuild.cmake)
endif()