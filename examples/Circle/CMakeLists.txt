cmake_minimum_required(VERSION 3.10)
project(Circle)

add_executable(Circle main.cpp)

if(UNIX)
    target_link_libraries(Circle TML-Graphics TML-Window TML-System dl pthread)
else()
    target_link_libraries(Circle TML-Graphics TML-Window TML-System)
endif()