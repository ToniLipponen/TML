cmake_minimum_required(VERSION 3.0)
project(tml-network VERSION ${TML_API_VERSION})

if(WIN32)
    set(SRC_FILES Receiver.cpp Socket.cpp Http/Request.cpp Http/Response.cpp Http/Host.cpp Win32/NetworkContext.cpp)
else()
    set(SRC_FILES Receiver.cpp Socket.cpp Http/Request.cpp Http/Response.cpp Http/Host.cpp)
endif()

add_library(tml-network ${TML_LIBRARY_TYPE} ${SRC_FILES})
set_target_properties(tml-network PROPERTIES VERSION ${TML_API_VERSION} SOVERSION ${TML_API_VERSION})
if(WIN32)
    target_link_libraries(tml-network wsock32 ws2_32)
endif()