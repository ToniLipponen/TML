cmake_minimum_required(VERSION 3.0)
project(tml-interface)

file(GLOB_RECURSE SRC_FILES ./*.cpp)
add_library(tml-interface ${SRC_FILES})
set_target_properties(tml-interface PROPERTIES VERSION ${TML_VERSION} SOVERSION ${TML_VERSION})