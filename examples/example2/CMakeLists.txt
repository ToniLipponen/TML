cmake_minimum_required(VERSION 3.10)
project(example2)

add_executable(example2 main.cpp)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)

if(UNIX)
    target_link_libraries(example2 TML-Graphics TML-System dl pthread)
else()
    target_link_libraries(example2 TML-Graphics TML-System)
endif()