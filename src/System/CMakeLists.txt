cmake_minimum_required(VERSION 2.8...3.20)
project(TML-System)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(Clip)
add_library(TML-System Clipboard.cpp Image.cpp)
target_include_directories(TML-System PRIVATE Clip)
target_link_libraries(TML-System clip webp)