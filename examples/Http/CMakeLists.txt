cmake_minimum_required(VERSION 3.0)
project(Http)

add_executable(Http main.cpp)

if(UNIX)
    target_link_libraries(Http tml-network tml-system dl pthread)
else()
    target_link_libraries(Http tml-network tml-system)
endif()