#pragma once

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <cstdio>

class WinNetContext
{
public:
    static void Create()
    {
        if(!s_initialized)
        {
            WSADATA wsaData;

            if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            {
                std::puts("[Error]: WSAStartup failed");
            }
            else
            {
                s_initialized = true;
            }
        }
    }
private:
    inline static bool s_initialized = false;
};