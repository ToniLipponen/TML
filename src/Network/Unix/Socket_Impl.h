/// Unix sockets implementation

#include <TML/Network/Socket.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>

namespace tml::Net
{
    Socket::Socket()
    {
        m_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }

    Socket::~Socket()
    {
        close(m_fd);
    }

    SocketResult Socket::Connect(const std::string& address, uint32_t port) noexcept
    {
        struct sockaddr_in sockAddress{};
        sockAddress.sin_family = AF_INET;
        sockAddress.sin_port = htons(port);

        auto result = inet_pton(AF_INET, address.c_str(), &sockAddress.sin_addr);

        if(result == 0)
        {
            auto ipAddress = IpFromHostname(address);
            result = inet_pton(AF_INET, ipAddress.c_str(), &sockAddress.sin_addr);

            /// Invalid address
            if(result != 1)
            {
                return SocketResult::InvalidAddress;
            }
        }

        result = connect(m_fd, reinterpret_cast<struct sockaddr*>(&sockAddress), sizeof(sockAddress));

        /// Failed to connect to the host
        if(result == -1)
        {
            close(m_fd);
            m_fd = 0;
            return SocketResult::FailedToConnect;
        }

        return SocketResult::OK;
    }

    SocketResult Socket::Disconnect()
    {
        const auto result = close(m_fd) == 0 ? SocketResult::OK : SocketResult::Error;
        m_fd = 0;
        return result;
    }

    [[maybe_unused]] bool Socket::IsConnected() const
    {
        return m_fd != 0;
    }

    SocketResult Socket::Send(const void *data, uint64_t size, uint64_t& sent) const
    {
        const int64_t written = write(m_fd, data, size);
        sent = written;

        if(written == -1) //!< No data was sent.
        {
            sent = 0;
            return SocketResult::Error;
        }
        else if(written == size) //!< All data was sent.
        {
            return SocketResult::OK;
        }

        /// Only a portion of the data was sent.
        return SocketResult::Incomplete;
    }

    SocketResult Socket::Receive(void *data, uint64_t size, uint64_t &received) const
    {
        int64_t bytes = read(m_fd, data, size);
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
        int status = 0;

        if(blocking != m_blocking)
        {
            const int flags = fcntl(m_fd, F_GETFL, 0);

            if(!blocking)
            {
                status = fcntl(m_fd, F_SETFL, flags | O_NONBLOCK);
            }
            else
            {
                status = fcntl(m_fd, F_SETFL, flags & ~O_NONBLOCK);
            }

            if(status != -1)
            {
                m_blocking = blocking;
            }
        }

        return status != -1 ? SocketResult::OK : SocketResult::Error;
    }

    bool Socket::GetBlocking() const
    {
        return m_blocking;
    }

    std::string Socket::IpFromHostname(const std::string& hostname)
    {
        struct hostent* he = gethostbyname(hostname.c_str());

        if(he == nullptr)
        {
            return "";
        }

        auto addressList  = reinterpret_cast<struct in_addr**>(he->h_addr_list);

        if(addressList[0])
        {
            return inet_ntoa(*addressList[0]);
        }

        return "";
    }
}