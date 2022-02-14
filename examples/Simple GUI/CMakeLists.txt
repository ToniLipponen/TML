cmake_minimum_required(VERSION 3.10)
project(SimpleGUI)

add_executable(SimpleGUI main.cpp)

if(UNIX)
    target_link_libraries(SimpleGUI TML-Graphics TML-Interface TML-Window TML-System dl pthread)
else()
    target_link_libraries(SimpleGUI TML-Graphics TML-Interface TML-Window TML-System)
endif()