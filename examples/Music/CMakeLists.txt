cmake_minimum_required(VERSION 3.10)
project(Music)

add_executable(Music main.cpp)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/res/Kevin MacLeod - Study and Relax.mp3" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Kevin MacLeod - Study and Relax.mp3")

if(UNIX)
    target_link_libraries(Music TML-Graphics TML-Audio TML-Window TML-System dl pthread)
else()
    target_link_libraries(Music TML-Graphics TML-Audio TML-Window TML-System)
endif()