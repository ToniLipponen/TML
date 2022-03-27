cmake_minimum_required(VERSION 3.0)
project(Compute)

add_executable(Compute main.cpp)

if(UNIX)
    target_link_libraries(Compute tml-graphics tml-window tml-system dl pthread)
else()
    target_link_libraries(Compute tml-graphics tml-window tml-system)
endif()