cmake_minimum_required(VERSION 3.0)
project(experiments)

add_executable(experiments main.cpp)

if(UNIX)
    target_link_libraries(experiments tml-interface tml-graphics tml-window tml-system dl pthread)
else()
    target_link_libraries(experiments tml-interface tml-graphics tml-window tml-system)
endif()