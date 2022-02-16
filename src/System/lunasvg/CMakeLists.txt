cmake_minimum_required(VERSION 3.3)

project(lunasvg VERSION 2.3.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

add_library(lunasvg OBJECT)
set_property(TARGET lunasvg PROPERTY POSITION_INDEPENDENT_CODE ON)

add_subdirectory(include)
add_subdirectory(source)
add_subdirectory(3rdparty/software)
add_subdirectory(3rdparty/plutovg)

set(LUNASVG_LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
set(LUNASVG_INCDIR ${CMAKE_INSTALL_PREFIX}/include)