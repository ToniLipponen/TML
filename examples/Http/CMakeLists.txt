cmake_minimum_required(VERSION 3.10)
project(Http)

add_executable(Http main.cpp)

if(UNIX)
    target_link_libraries(Http TML-Network TML-System dl pthread)
else()
    target_link_libraries(Http TML-Network TML-System)
endif()