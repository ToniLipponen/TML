cmake_minimum_required(VERSION 3.20)
project(webp C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
file(GLOB_RECURSE SRC_FILES
        src/dec/*.c
        src/demux/*.c
        src/dsp/*.c
        src/enc/*.c
        src/mux/*.c
        src/utils/*.c
        src/webp/*.c)

include_directories(./)
add_library(webp OBJECT ${SRC_FILES})
