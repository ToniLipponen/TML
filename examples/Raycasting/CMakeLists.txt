cmake_minimum_required(VERSION 3.0)
project(Raycasting)

add_executable(Raycasting main.cpp)

if(UNIX)
    target_link_libraries(Raycasting tml-graphics tml-window tml-system dl pthread)
else()
    target_link_libraries(Raycasting tml-graphics tml-window tml-system)
endif()