#pragma once
#include <TML/Network/Socket.h>
#include <memory>

namespace tml
{
	class TML_API UdpSocket : public Socket
	{
	public:
		UdpSocket();
		UdpSocket(UdpSocket&& other) noexcept;
		~UdpSocket() noexcept;

		UdpSocket& operator=(UdpSocket&& other) noexcept;

		Socket::Result Connect(const IpAddress& address, uint32_t port) override;
		Socket::Result Bind(uint32_t port);

		Socket::Result Send(const void* data, uint64_t size, uint64_t& sent) override;
		Socket::Result Send(const Packet& packet) override;

		Socket::Result Receive(void* data, uint64_t size, uint64_t& received) override;
		Socket::Result Receive(Packet& packet) override;

	private:
		struct Data;
		Data* m_data;
//		std::unique_ptr<Data> m_data;
	};
}