cmake_minimum_required(VERSION 3.10)
project(raycasting)

add_executable(raycasting main.cpp)

if(UNIX)
    target_link_libraries(raycasting TML-Graphics TML-System dl pthread)
else()
    target_link_libraries(raycasting TML-Graphics TML-System)
endif()