cmake_minimum_required(VERSION 3.10)
project(Raycasting)

add_executable(Raycasting main.cpp)

if(UNIX)
    target_link_libraries(Raycasting TML-Graphics TML-Window TML-System dl pthread)
else()
    target_link_libraries(Raycasting TML-Graphics TML-Window TML-System)
endif()