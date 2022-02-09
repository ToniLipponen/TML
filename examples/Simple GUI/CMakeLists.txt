cmake_minimum_required(VERSION 3.10)
project(example4)

add_executable(example4 main.cpp)

if(UNIX)
    target_link_libraries(example4 TML-Graphics TML-Interface TML-Window TML-System dl pthread)
else()
    target_link_libraries(example4 TML-Graphics TML-Interface TML-Window TML-System)
endif()