cmake_minimum_required(VERSION 2.8...3.20)
project(TML-System)

add_library(TML-System Clipboard.cpp Image.cpp)
target_link_libraries(TML-System clip webp lunasvg)