#include "NetworkContext.h"
#include <TML/System/Logger.h>

bool WinNetContext::s_initialized = false;

void WinNetContext::Create()
{
    if(!s_initialized)
    {
        WSADATA wsaData;
        if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            tml::Logger::ErrorMessage("WSAStartup failed");
        else
            s_initialized = true;
    }
}
