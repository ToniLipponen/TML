cmake_minimum_required(VERSION 3.0)
project(tml-interface VERSION ${TML_API_VERSION})

file(GLOB_RECURSE SRC_FILES ./*.cpp)
add_library(tml-interface ${TML_LIBRARY_TYPE} ${SRC_FILES})
target_link_libraries(tml-interface tml-graphics tml-window tml-system)
set_target_properties(tml-interface PROPERTIES VERSION ${TML_API_VERSION} SOVERSION ${TML_API_VERSION})