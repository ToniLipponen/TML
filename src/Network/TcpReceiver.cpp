#include <TML/Network/TcpReceiver.h>

#include "PlatformHeaders.h"
#include "SocketUtils.h"

#include <iostream>

namespace tml
{
	TcpReceiver::TcpReceiver() = default;

	bool TcpReceiver::Listen(uint32_t port, const IpAddress& address)
	{
		Close();
		Create();

		sockaddr_in sockAddress = MakeAddress(port, address);

		if(bind(m_fd, reinterpret_cast<sockaddr*>(&sockAddress), sizeof(sockAddress)) == -1)
		{
			std::cerr << "Failed to bind socket\n";
			return false;
		}

		if(listen(m_fd, SOMAXCONN) == TML_SOCKET_ERROR)
		{
			std::cerr << "Failed to listen on socket\n";
			return false;
		}

		return true;
	}

	bool TcpReceiver::Accept(TcpSocket& socket)
	{
		sockaddr_in clientAddr = {};
		socklen_t clientAddrlen = sizeof(clientAddr);

		if(socket.m_fd = accept(m_fd, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrlen); socket.m_fd == TML_SOCKET_ERROR)
		{
			socket.Close();
			return false;
		}

		socket.m_address = IpAddress(ntohl(clientAddr.sin_addr.s_addr));

		return true;
	}
}