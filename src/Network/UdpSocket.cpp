#include <TML/Network/UdpSocket.h>
#include <TML/Network/Hton.h>
#include "PlatformHeaders.h"
#include "SocketUtils.h"

using namespace tml;

/*
 * X = IP header size = 20 bytes
 * Y = UDP header size = 8 bytes
 *
 * s_maximumDatagramSize = 2^16 - X - Y
 */
constexpr static uint64_t s_maximumDatagramSize = 65507;

struct UdpSocket::Data
{
	sockaddr_in address;
};

UdpSocket::UdpSocket()
: Socket(Socket::Type::Udp), m_data(new Data)
{

}

UdpSocket::UdpSocket(UdpSocket&& other) noexcept
: Socket(Socket::Type::Udp), m_data(new Data)
{
	std::swap(m_fd, other.m_fd);
}

UdpSocket::~UdpSocket() noexcept
{
	delete m_data;
}

UdpSocket& UdpSocket::operator=(UdpSocket&& other) noexcept
{
	m_fd = TML_INVALID_SOCKET;
	std::swap(m_fd, other.m_fd);

	return *this;
}

Socket::Result UdpSocket::Connect(const tml::IpAddress& address, uint32_t port)
{
	m_data->address = MakeAddress(address, port);
	return Socket::Result::OK;
}

Socket::Result UdpSocket::Bind(uint32_t port)
{
	m_data->address = MakeAddress("0.0.0.0", port);

	if(bind(m_fd, reinterpret_cast<sockaddr*>(&m_data->address), sizeof(m_data->address)) == -1)
	{
		std::cerr << "Failed to bind UDP socket\n";
		return Socket::Result::Error;
	}

	return Socket::Result::OK;
}

Socket::Result UdpSocket::Send(const void* data, uint64_t size, uint64_t& sent)
{
	if(size <= s_maximumDatagramSize)
	{
		auto bytes = sendto(
				m_fd,
				reinterpret_cast<const char*>(data),
				static_cast<size_t>(size),
				0,
				reinterpret_cast<sockaddr*>(&m_data->address),
				sizeof(m_data->address));

		if(bytes == TML_SOCKET_ERROR)
		{
			return CheckErrno();
		}

		sent = static_cast<uint64_t>(bytes);

		return Socket::Result::OK;
	}
	else
	{
		std::cerr << "Tried to send a datagram bigger than the theoretical maximum size of 65507\n";
		return Socket::Result::Error;
	}
}

Socket::Result UdpSocket::Send(const tml::Packet& packet)
{
	if(packet.GetSize() <= s_maximumDatagramSize)
	{
		/// Win32: https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-sendto
		/// Unix: https://www.unix.com/man-page/Linux/2/sendto/

		auto bytes = sendto(m_fd,
				static_cast<const char*>(packet.GetData()),
				static_cast<int>(packet.GetSize()),
				0,
				reinterpret_cast<sockaddr*>(&m_data->address),
				sizeof(m_data->address));

		if(bytes == TML_SOCKET_ERROR)
		{
			return CheckErrno();
		}

		return Socket::Result::OK;
	}

	std::cerr << "Tried to send a datagram bigger than the theoretical maximum size of 65507\n";
	return Socket::Result::Error;
}

Socket::Result UdpSocket::Receive(void* data, uint64_t size, uint64_t& received)
{
	if(size <= s_maximumDatagramSize)
	{
		sockaddr_in sockaddrIn = MakeAddress("0.0.0.0", 0);

		auto bytes = recvfrom(m_fd,
				static_cast<char*>(data),
				size,
				0,
				reinterpret_cast<sockaddr*>(&sockaddrIn),
				nullptr);

		if(bytes == TML_SOCKET_ERROR)
		{
			return Socket::Result::Error;
		}

		received = static_cast<uint64_t>(bytes);
		return Socket::Result::OK;
	}

	std::cerr << "Tried to receive a datagram bigger than the theoretical maximum size of 65507\n";
	return Socket::Result::Error;
}

Socket::Result UdpSocket::Receive(tml::Packet& packet)
{
	packet.ClearData();
	char buffer[s_maximumDatagramSize];
	uint64_t received = 0;

	if(Receive(buffer, s_maximumDatagramSize, received) == Socket::Result::Error)
	{
		return Socket::Result::Error;
	}

	packet.PushData(buffer, received);
	return Socket::Result::OK;
}