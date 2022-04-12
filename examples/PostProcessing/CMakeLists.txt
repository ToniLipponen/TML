cmake_minimum_required(VERSION 3.0)
project(PostProcessing)

add_executable(PostProcessing main.cpp)

if(UNIX)
    target_link_libraries(PostProcessing tml-graphics tml-window tml-system dl pthread)
else()
    target_link_libraries(PostProcessing tml-graphics tml-window tml-system)
endif()