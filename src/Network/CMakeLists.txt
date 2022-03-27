cmake_minimum_required(VERSION 3.0)
project(tml-network)

if(WIN32)
    set(SRC_FILES Receiver.cpp Socket.cpp Http/Request.cpp Http/Response.cpp Http/Host.cpp Win32/NetworkContext.cpp)
else()
    set(SRC_FILES Receiver.cpp Socket.cpp Http/Request.cpp Http/Response.cpp Http/Host.cpp)
endif()

add_library(tml-network ${SRC_FILES})
if(WIN32)
    target_link_libraries(tml-network wsock32 ws2_32)
endif()
set_target_properties(tml-network PROPERTIES VERSION ${TML_VERSION} SOVERSION ${TML_VERSION})