cmake_minimum_required(VERSION 2.8...3.20)
project(TML-Network)

if(WIN32)
    set(SRC_FILES Receiver.cpp Socket.cpp Win32/NetworkContext.cpp)
else()
    set(SRC_FILES Receiver.cpp Socket.cpp)
endif()

add_library(TML-Network ${SRC_FILES})
if(WIN32)
    target_link_libraries(TML-Network wsock32 ws2_32)
endif()