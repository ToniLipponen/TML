#include <TML/Network/TcpSocket.h>
#include <TML/Network/Hton.h>
#include <TML/System/Platform.h>
#include "PlatformHeaders.h"
#include "SocketUtils.h"

using namespace tml;

TcpSocket::TcpSocket()
: Socket(Socket::Type::Tcp)
{

}

TcpSocket::TcpSocket(tml::TcpSocket&& other) noexcept
: Socket(Socket::Type::Tcp)
{
	std::swap(m_fd,         other.m_fd);
	std::swap(m_address,    other.m_address);
}

TcpSocket& TcpSocket::operator=(tml::TcpSocket &&other) noexcept
{
	std::swap(m_fd,         other.m_fd);
	std::swap(m_address,    other.m_address);

	other.m_fd = TML_INVALID_SOCKET;
	other.m_address = IpAddress();
	return *this;
}

Socket::Result TcpSocket::Connect(const IpAddress& address, uint32_t port)
{
	Close();
	Create();
	sockaddr_in sockAddress = MakeAddress(address, port);

	const auto result = connect(m_fd, reinterpret_cast<sockaddr*>(&sockAddress), sizeof(sockAddress));

	/// Failed to connect to the host
	if(result != 0)
	{
		Close();
		return CheckErrno();
	}

	return Socket::Result::OK;
}

Socket::Result TcpSocket::Send(const void *data, uint64_t size, uint64_t& sent)
{
	sent = 0;

	while(sent < size)
	{
#if defined(TML_PLATFORM_UNIX)
		const int64_t written = send(m_fd, static_cast<const char*>(data) + sent, size - sent, MSG_NOSIGNAL);
#else // Win32
		const int64_t written = send(m_fd, static_cast<const char*>(data) + sent, static_cast<int32_t>(size - sent), 0);
#endif
		if(written < 0)
		{
			return CheckErrno();
		}

		sent += written;
	}

	return Socket::Result::OK;
}

Socket::Result TcpSocket::Send(const Packet& packet)
{
	uint64_t size = tml::Htonll(packet.GetSize());
	uint64_t sent = 0;

	/// Apparently you cant do two separate back to back sends, with one
	/// being the size of the packet & the other being the data.
	m_sendBuffer.resize(packet.GetSize() + sizeof(size));
	memcpy(&m_sendBuffer[0], &size, sizeof(size));
	memcpy(&m_sendBuffer[sizeof(size)], packet.GetData(), packet.GetSize());

	return Send(m_sendBuffer.data(), m_sendBuffer.size(), sent);
}

Socket::Result TcpSocket::Receive(void* data, uint64_t size, uint64_t& received)
{
	received = 0;
#if defined(TML_PLATFORM_UNIX)
	int64_t bytes = recv(m_fd, data, size, 0);
#else // Win32
	int64_t bytes = recv(m_fd, static_cast<char*>(data), static_cast<int32_t>(size), 0);
#endif
	if(bytes <= 0) //!< Error
	{
		return CheckErrno();
	}
	else if(bytes == size) //!< Potentially more data available.
	{
		received = bytes;
		return Socket::Result::Incomplete;
	}

	received = bytes;

	return Socket::Result::OK;
}

Socket::Result TcpSocket::Receive(Packet& packet)
{
	packet.ClearData();
	uint64_t receivedBytes = 0;
	uint64_t packetSize = 0;

	while(receivedBytes < sizeof(packetSize))
	{
		uint64_t size = 0;
		auto result = Receive(reinterpret_cast<char*>(&packetSize) + receivedBytes, sizeof(packetSize) - receivedBytes, size);

		if(size == 0)
		{
			return Socket::Result::Disconnected;
		}
		if(result > Socket::Result::Incomplete)
		{
			return result;
		}

		receivedBytes += size;
	}

	receivedBytes = 0;
	packetSize = tml::Ntohll(packetSize);

	char tempBuffer[1024];

	while(receivedBytes < packetSize)
	{
		uint64_t received = 0;
		auto result = Receive(tempBuffer, sizeof(tempBuffer), received);

		if(received == 0)
		{
			packet.ClearData();
			return Socket::Result::Disconnected;
		}
		if(result > Socket::Result::Incomplete)
		{
			packet.ClearData();
			return result;
		}

		packet.PushData(tempBuffer, received);
		receivedBytes += received;
	}

	return Socket::Result::OK;
}

const IpAddress& TcpSocket::GetAddress() const
{
	return m_address;
}