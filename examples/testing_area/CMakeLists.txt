cmake_minimum_required(VERSION 3.10)
project(test)

add_executable(test main.cpp)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/res/Logo.png $<TARGET_FILE_DIR:${PROJECT_NAME}>/Logo.png)

if(UNIX)
    target_link_libraries(test TML-Interface TML-Graphics TML-Window TML-System dl pthread)
else()
    target_link_libraries(test TML-Interface TML-Graphics TML-Window TML-System)
endif()