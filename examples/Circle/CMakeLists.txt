cmake_minimum_required(VERSION 3.10)
project(example1)

add_executable(example1 main.cpp)

if(UNIX)
    target_link_libraries(example1 TML-Graphics TML-Window TML-System dl pthread)
else()
    target_link_libraries(example1 TML-Graphics TML-Window TML-System)
endif()