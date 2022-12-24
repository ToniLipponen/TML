#pragma once
#include <TML/Network/Socket.h>

namespace tml
{
    class TML_API TcpSocket : public Socket
    {
    public:
        TcpSocket();
        TcpSocket(TcpSocket&& other) noexcept;

        TcpSocket& operator=(TcpSocket&& other) noexcept;

        Result Connect(const IpAddress& address, uint32_t port) override;

        Result Send(const void* data, uint64_t size, uint64_t& sent) override;
        Result Send(const Packet& packet) override;

        Result Receive(void* data, uint64_t size, uint64_t& received) override;
        Result Receive(Packet& packet) override;

        [[nodiscard]] const IpAddress& GetAddress() const;

		friend class TcpReceiver;
    private:
		IpAddress m_address;
		std::vector<char> m_sendBuffer;
    };
}