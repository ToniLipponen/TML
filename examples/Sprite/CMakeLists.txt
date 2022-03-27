cmake_minimum_required(VERSION 3.0)
project(Sprite)

add_executable(Sprite main.cpp)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/res/Logo.png $<TARGET_FILE_DIR:${PROJECT_NAME}>/Logo.png)

if(UNIX)
    target_link_libraries(Sprite tml-graphics tml-window tml-system dl pthread)
else()
    target_link_libraries(Sprite tml-graphics tml-window tml-system)
endif()