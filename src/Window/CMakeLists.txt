cmake_minimum_required(VERSION 3.10)
project(tml-window VERSION ${TML_API_VERSION})

file(GLOB_RECURSE SRC_FILES ./*.cpp)
add_library(tml-window ${TML_LIBRARY_TYPE} ${SRC_FILES})
target_link_libraries(tml-window tml-system glfw)
set_target_properties(tml-window PROPERTIES VERSION ${TML_API_VERSION} SOVERSION ${TML_API_VERSION})