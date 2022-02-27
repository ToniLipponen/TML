/// Unix sockets implementation

#include <TML/Network/Socket.h>
#include <TML/System/Logger.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace tml
{
    namespace Net
    {
        Socket::Socket()
        {
            m_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if(m_fd == -1)
            {
                Logger::ErrorMessage("Could not create a socket");
                return;
            }
        }

        Socket::~Socket()
        {
            close(m_fd);
        }

        bool Socket::Connect(const std::string &address, uint32_t port) noexcept
        {
            struct sockaddr_in addr{};
            inet_pton(AF_INET, address.c_str(), &addr.sin_addr);
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            auto result = connect(m_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(struct sockaddr_in)) == 0;

            if(result == -1)
            {
                Logger::ErrorMessage("Failed to connect to host");
                return false;
            }
            return true;
        }

        bool Socket::Disconnect()
        {
            return close(m_fd) == 0;
        }

        bool Socket::IsConnected()
        {
            return true;
        }

        bool Socket::Send(const void *data, uint64_t size)
        {
            return write(m_fd, data, size) != -1;
        }

        bool Socket::Receive(void *data, uint64_t size, uint64_t &received)
        {
            int64_t bytes = read(m_fd, data, size);
            if(bytes == -1)
            {
                received = 0;
                return false;
            }
            received = bytes;
            return true;
        }
    }
}