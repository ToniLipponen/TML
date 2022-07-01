/// Windows sockets implementation

#include <TML/Network/Socket.h>
#include "NetworkContext.h"

namespace tml::Net
{
    Socket::Socket()
    {
        m_fd = INVALID_SOCKET;
        WinNetContext::Create();
    }

    Socket::~Socket()
    {
        closesocket(m_fd);
        WSACleanup();
    }

    SocketResult Socket::Connect(const std::string &address, uint32_t port) noexcept
    {
        m_port = port;
        struct addrinfo *result = NULL, *ptr = NULL, hints{};

        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        int iResult = getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &result);

        /// Failed to get host address info
        if(iResult != 0)
        {
            return SocketResult::InvalidAddress;
        }

        for(ptr = result; ptr != nullptr; ptr = ptr->ai_next)
        {
            m_fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

            if(m_fd == INVALID_SOCKET)
            {
                return SocketResult::FailedToConnect;
            }

            iResult = connect(m_fd, ptr->ai_addr, ptr->ai_addrlen);

            if(iResult == SOCKET_ERROR)
            {
                closesocket(m_fd);
                m_fd = INVALID_SOCKET;
                continue;
            }

            break;
        }

        freeaddrinfo(result);

        if(m_fd == INVALID_SOCKET)
        {
            return SocketResult::FailedToConnect;
        }

        return SocketResult::OK;
    }

    SocketResult Socket::Disconnect()
    {
        const auto result = closesocket(m_fd) == SOCKET_ERROR ? SocketResult::Error : SocketResult::OK;
        m_fd = INVALID_SOCKET;
        return result;
    }

    bool Socket::IsConnected() const
    {
        return m_fd != INVALID_SOCKET;
    }

    SocketResult Socket::Send(const void *data, uint64_t size, uint64_t& sent) const
    {
        const int64_t written = send(m_fd, reinterpret_cast<const char*>(data), static_cast<int>(size), 0);
        sent = written;

        if(written == -1)
        {
            sent = 0;
            return SocketResult::Error;
        }
        else if(written == size)
        {
            return SocketResult::OK;
        }

        return SocketResult::Incomplete;
    }

    SocketResult Socket::Receive(void *data, uint64_t size, uint64_t &received) const
    {
        int64_t bytes = recv(m_fd, reinterpret_cast<char *>(data), static_cast<int>(size), 0);
        received = bytes;

        if(bytes < 0) //!< Error
        {
            received = 0;
            return SocketResult::Error;
        }
        else if(bytes == size) //!< Potentially more data available.
        {
            return SocketResult::Incomplete;
        }
        else if(bytes < size) //!< All data was received
        {
            return SocketResult::OK;
        }

        return SocketResult::OK;
    }

    SocketResult Socket::SetBlocking(bool blocking)
    {
        u_long mode = static_cast<u_long>(!blocking);
        const auto result = ioctlsocket(m_fd, FIONBIO, &mode);

        if(result == SOCKET_ERROR)
        {
            return SocketResult::Error;
        }

        m_blocking = blocking;
        return SocketResult::OK;
    }

    bool Socket::GetBlocking() const
    {
        return m_blocking;
    }

    std::string Socket::IpFromHostname(const std::string &hostname)
    {
        return "";
    }
}