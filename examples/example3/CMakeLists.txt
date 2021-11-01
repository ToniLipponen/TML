cmake_minimum_required(VERSION 3.10)
project(example3)

add_executable(example3 main.cpp)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)

if(UNIX)
    target_link_libraries(example3 TML dl pthread)
else()
    target_link_libraries(example3 TML)
endif()