cmake_minimum_required(VERSION 3.0)
project(tml-system VERSION ${TML_API_VERSION})

add_library(tml-system ${TML_LIBRARY_TYPE} File.cpp Compression.cpp Clipboard.cpp Image.cpp)
target_link_libraries(tml-system glfw lunasvg fastlz)
set_target_properties(tml-system PROPERTIES VERSION ${TML_API_VERSION} SOVERSION ${TML_API_VERSION})