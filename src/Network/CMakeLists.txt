cmake_minimum_required(VERSION 3.0)
project(tml-network VERSION ${TML_API_VERSION})

set(SRC_FILES
        SocketMonitor.cpp
        IpAddress.cpp
        SocketUtils.cpp
        UdpSocket.cpp
        TcpSocket.cpp
        Packet.cpp
        TcpReceiver.cpp
        Socket.cpp
        Http/Request.cpp
        Http/Response.cpp
        Http/Client.cpp)

add_library(tml-network ${TML_LIBRARY_TYPE} ${SRC_FILES})
set_target_properties(tml-network PROPERTIES VERSION ${TML_API_VERSION} SOVERSION ${TML_API_VERSION})

if(WIN32)
    target_link_libraries(tml-network tml-system wsock32 ws2_32)
else()
    target_link_libraries(tml-network tml-system)
endif()