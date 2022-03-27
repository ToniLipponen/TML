cmake_minimum_required(VERSION 3.0)
project(Circle)

add_executable(Circle main.cpp)

if(UNIX)
    target_link_libraries(Circle tml-graphics tml-window tml-system dl pthread)
else()
    target_link_libraries(Circle tml-graphics tml-window tml-system)
endif()