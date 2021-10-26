cmake_minimum_required(VERSION 3.10)
project(example2)

add_executable(example2 main.cpp)

if(UNIX)
    target_link_libraries(example2 TML dl pthread)
else()
    target_link_libraries(example2 TML)
endif()