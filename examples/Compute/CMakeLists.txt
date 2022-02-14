cmake_minimum_required(VERSION 3.10)
project(Compute)

add_executable(Compute main.cpp)

if(UNIX)
    target_link_libraries(Compute TML-Graphics TML-Window TML-System dl pthread)
else()
    target_link_libraries(Compute TML-Graphics TML-Window TML-System)
endif()