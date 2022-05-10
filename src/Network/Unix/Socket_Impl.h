/// Unix sockets implementation

#include <TML/Network/Socket.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>

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

    bool Socket::Connect(const std::string& address, uint32_t port) noexcept
    {
        struct sockaddr_in sockAddress{};
        auto result = inet_pton(AF_INET, address.c_str(), &sockAddress.sin_addr);
        if(result == 0)
        {
            auto ipAddress = IpFromHostname(address);
            result = inet_pton(AF_INET, ipAddress.c_str(), &sockAddress.sin_addr);

            /// Invalid address
            if(result != 1)
                return false;
        }

        sockAddress.sin_family = AF_INET;
        sockAddress.sin_port = htons(port);

        result = connect(m_fd, reinterpret_cast<struct sockaddr*>(&sockAddress), sizeof(struct sockaddr_in));

        /// Failed to connect to the host
        if(result == -1)
        {
            close(m_fd);
            m_fd = 0;
            return false;
        }
        return true;
    }

    bool Socket::Disconnect() const
    {
        return close(m_fd) == 0;
    }

    [[maybe_unused]] bool Socket::IsConnected() const
    {
        return m_fd != 0;
    }

    bool Socket::Send(const void *data, uint64_t size) const
    {
        return write(m_fd, data, size) != -1;
    }

    bool Socket::Receive(void *data, uint64_t size, uint64_t &received) const
    {
        int64_t bytes = read(m_fd, data, size);

        if(bytes < 0)
        {
            received = 0;
            return false;
        }

        received = bytes;
        return true;
    }

    std::string Socket::IpFromHostname(const std::string& hostname)
    {
        struct hostent *he;
        struct in_addr **addr_list;

        if((he = gethostbyname(hostname.c_str())) == nullptr)
            return "";

        addr_list = (struct in_addr **) he->h_addr_list;

        for(int i = 0; addr_list[i] != nullptr; i++)
            return inet_ntoa(*addr_list[i]);

        return "";
    }
}