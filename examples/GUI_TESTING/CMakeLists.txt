cmake_minimum_required(VERSION 3.10)
project(GUI_TEST)

add_executable(GUI_TEST main.cpp)

if(UNIX)
    target_link_libraries(GUI_TEST  TML-Interface TML-Graphics TML-System dl pthread)
else()
    target_link_libraries(GUI_TEST TML-Graphics TML-System TML-Interface)
endif()