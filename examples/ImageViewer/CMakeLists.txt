cmake_minimum_required(VERSION 3.0)
project(ImageViewer)

add_executable(ImageViewer main.cpp)

if(UNIX)
    target_link_libraries(ImageViewer tml-graphics tml-window tml-system dl pthread)
else()
    target_link_libraries(ImageViewer tml-graphics tml-window tml-system)
endif()