cmake_minimum_required(VERSION 3.0)
project(tml-audio VERSION ${TML_API_VERSION})

file(GLOB_RECURSE SRC_FILES ./*.cpp)
add_library(tml-audio ${TML_LIBRARY_TYPE} ${SRC_FILES})
set_target_properties(tml-audio PROPERTIES VERSION ${TML_API_VERSION} SOVERSION ${TML_API_VERSION})