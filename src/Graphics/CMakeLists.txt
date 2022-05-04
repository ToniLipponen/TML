cmake_minimum_required(VERSION 3.0)
project(tml-graphics VERSION ${TML_API_VERSION})

file(GLOB_RECURSE SRC_FILES ./*.cpp)
add_library(tml-graphics ${TML_LIBRARY_TYPE} ${SRC_FILES})
target_link_libraries(tml-graphics tml-window tml-system glad)
set_target_properties(tml-graphics PROPERTIES VERSION ${TML_API_VERSION} SOVERSION ${TML_API_VERSION})