cmake_minimum_required(VERSION 3.10)
project(net_test)

add_executable(net_test main.cpp)

if(UNIX)
    target_link_libraries(net_test TML-Network TML-Graphics TML-Window TML-System dl pthread)
else()
    target_link_libraries(net_test TML-Network TML-Graphics TML-Window TML-System)
endif()