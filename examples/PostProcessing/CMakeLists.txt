cmake_minimum_required(VERSION 3.10)
project(PostProcessing)

add_executable(PostProcessing main.cpp)

if(UNIX)
    target_link_libraries(PostProcessing TML-Graphics TML-Window TML-System dl pthread)
else()
    target_link_libraries(PostProcessing TML-Graphics TML-Window TML-System)
endif()