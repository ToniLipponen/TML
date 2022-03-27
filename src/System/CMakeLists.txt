cmake_minimum_required(VERSION 3.0)
project(tml-system)

add_library(tml-system Clipboard.cpp Image.cpp)
target_link_libraries(tml-system clip webp lunasvg)
set_target_properties(tml-system PROPERTIES VERSION ${TML_VERSION} SOVERSION ${TML_VERSION})