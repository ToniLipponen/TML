#include "NetworkContext.h"
#include <cstdio>

bool WinNetContext::s_initialized = false;

void WinNetContext::Create()
{
    if(!s_initialized)
    {
        WSADATA wsaData;
        if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            std::puts("[Error]: WSAStartup failed");
        else
            s_initialized = true;
    }
}

