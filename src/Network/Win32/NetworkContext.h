#pragma once

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <iostream>

/// https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup

class WinNetContext
{
public:
    WinNetContext()
    {
		WSADATA wsaData;

		if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			std::cerr << "WSAStartup failed" << std::endl;
		}
    }

	~WinNetContext()
	{
		WSACleanup();
	}
};