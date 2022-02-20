cmake_minimum_required(VERSION 3.10)
project(ImageViewer)

add_executable(ImageViewer main.cpp)

if(UNIX)
    target_link_libraries(ImageViewer TML-Graphics TML-Window TML-System dl pthread)
else()
    target_link_libraries(ImageViewer TML-Graphics TML-Window TML-System)
endif()