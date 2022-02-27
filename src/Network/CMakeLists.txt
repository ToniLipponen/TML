cmake_minimum_required(VERSION 2.8...3.20)
project(TML-Network)

file(GLOB_RECURSE SRC_FILES ./*.cpp)
add_library(TML-Network ${SRC_FILES})
if(WIN32)
    target_link_libraries(TML-Network wsock32 ws2_32)
endif()