#include <TML/Network/Socket.h>
#include <TML/System/Platform.h>
#include "PlatformHeaders.h"

#if defined(TML_PLATFORM_WINDOWS)
	static WinNetContext s_winContext;
#endif

using namespace tml;

Socket::Socket(tml::Socket::Type type)
: m_fd(TML_INVALID_SOCKET), m_type(type)
{
    Create();
}

Socket::~Socket()
{
	Close();
}

void Socket::Create()
{
	if(m_fd == TML_INVALID_SOCKET)
	{
		auto sockType = (m_type == Type::Tcp) ? SOCK_STREAM : SOCK_DGRAM;
		m_fd = socket(AF_INET, sockType, 0);

		if(m_fd == TML_INVALID_SOCKET)
		{
			std::cerr << "Failed to create a socket\n";
		}

		if(m_type == Type::Udp)
		{
			int one = 1;
			setsockopt(m_fd, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&one), sizeof(one));
		}
		else
		{
			int one = 1;
			setsockopt(m_fd, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&one), sizeof(one));
		}

		SetBlocking(m_blocking);
	}
}

void Socket::Close()
{
#if defined(TML_PLATFORM_UNIX)
	close(m_fd);
#elif defined(TML_PLATFORM_WINDOWS)
	closesocket(m_fd);
#endif
	m_fd = TML_INVALID_SOCKET;
}

Socket::Result Socket::SetBlocking(bool blocking)
{
#if defined(TML_PLATFORM_UNIX)
	int status;

	const int flags = fcntl(m_fd, F_GETFL, 0);

	if(!blocking)
	{
		status = fcntl(m_fd, F_SETFL, flags | O_NONBLOCK);
	}
	else
	{
		status = fcntl(m_fd, F_SETFL, flags & ~O_NONBLOCK);
	}

	return status != -1 ? Socket::Result::OK : CheckErrno();
#elif defined(TML_PLATFORM_WINDOWS)
	u_long mode = static_cast<u_long>(m_blocking);
    return ioctlsocket(m_fd, FIONBIO, &mode) == 0 ? Socket::Result::OK : Socket::Result::Error;
#endif
}

bool Socket::IsBlocking() const
{
	return m_blocking;
}

tml::Socket::DescriptorType Socket::GetHandle() const
{
	return m_fd;
}

Socket::Result Socket::CheckErrno()
{
#if defined(TML_PLATFORM_UNIX)
	/// https://www.unix.com/man-page/Linux/3/errno/

	if(errno == EAGAIN) //!< Why is this a duplicate, when these are supposed to be distinct?
	{
		return Socket::Result::Incomplete;
	}

	switch(errno)
	{
		case EINPROGRESS:
		case EWOULDBLOCK:
			return Socket::Result::Incomplete;
		case ENOTCONN:
		case ECONNABORTED:
		case ETIMEDOUT:
		case ENETRESET:
		case EPIPE:		//!< Broken pipe
			return Socket::Result::Disconnected;
		default: break;
	}

	return Socket::Result::Error;
#else // Win32
	/// https://learn.microsoft.com/en-us/windows/win32/winsock/windows-sockets-error-codes-2

	switch(WSAGetLastError())
	{
		case WSAEWOULDBLOCK:
		case WSAEALREADY:
			return Socket::Result::Incomplete;
		case WSAENOTCONN:
		case WSAECONNABORTED:
		case WSAETIMEDOUT:
		case WSAENETRESET:
		case WSAECONNRESET:
			return Socket::Result::Disconnected;
		case WSAEISCONN:
			return Socket::Result::OK;
		default: break;
	}

	return Socket::Result::Error;
#endif
}