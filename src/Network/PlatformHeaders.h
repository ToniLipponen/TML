#pragma once

#include <TML/System/Platform.h>
#include <exception>
#include <iostream>

#if defined(TML_PLATFORM_UNIX)
	#include <poll.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <arpa/inet.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <cerrno>
	#include <fcntl.h>
	#include <unistd.h>
	#define TML_INVALID_SOCKET (-1)
	#define TML_SOCKET_ERROR (-1)
#elif defined(TML_PLATFORM_WINDOWS)
	#include "Win32/NetworkContext.h"
	#define TML_INVALID_SOCKET INVALID_SOCKET
	#define TML_SOCKET_ERROR SOCKET_ERROR
#else
	#error "Unsupported platform"
#endif