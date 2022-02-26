#include <TML/Network/Socket.h>

#define ASIO_STANDALONE
#include <asio/include/asio/ip/tcp.hpp>

using namespace asio;
using namespace ip;

namespace tml
{
    namespace Net
    {
        Socket::Socket(Context& context)
        {
            m_socket = new tcp::socket(*reinterpret_cast<io_context*>(context.m_context));
        }

        bool Socket::Connect(const std::string &address, ui16 port) noexcept
        {
            error_code errorCode;
            tcp::endpoint endpoint(make_address(address, errorCode), port);
            auto asioSocket = reinterpret_cast<tcp::socket*>(m_socket);
            asioSocket->connect(endpoint);
            return errorCode.value() == 0 && asioSocket->is_open();
        }

        bool Socket::Disconnect()
        {
            error_code errorCode;
            auto asioSocket = reinterpret_cast<tcp::socket*>(m_socket);
            asioSocket->close(errorCode);
            return errorCode.value() == 0;
        }

        bool Socket::IsConnected()
        {
            auto asioSocket = reinterpret_cast<tcp::socket*>(m_socket);
            return asioSocket->is_open();
        }

        bool Socket::Send(const void *data, ui64 size)
        {
            auto asioSocket = reinterpret_cast<tcp::socket*>(m_socket);
            return asioSocket->write_some(asio::buffer(data, size));
        }

        bool Socket::Receive(void *data, ui64 size, ui64& received)
        {
            auto asioSocket = reinterpret_cast<tcp::socket*>(m_socket);
            asio::mutable_buffer buffer(data, size);
            received = asioSocket->read_some(buffer);
            std::memcpy(const_cast<void *>(data), buffer.data(), buffer.size());
            return true;
        }
    }
}