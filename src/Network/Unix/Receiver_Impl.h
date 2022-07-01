#include <TML/Network/Receiver.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace tml::Net
{
    Receiver::Receiver() = default;

    bool Receiver::Listen(uint32_t port)
    {
        if(m_port != port)
        {
            m_port = port;
            auto result = Bind(); // Rebind.

            if(!result)
            {
                return false;
            }
        }

        return listen(m_fd, 5) != -1;
    }

    /// Note to self: Maybe clean this up at some point?
    bool Receiver::Accept(Socket& socket)
    {
        //// Accept the connection
        struct sockaddr_in clientAddr = {};
        socklen_t clientAddrlen = sizeof(clientAddr);
        socket.m_fd = accept(m_fd, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrlen);
        ////

        //// Get the ip address
        auto* addr = dynamic_cast<struct sockaddr_in*>(&clientAddr);
        struct in_addr inAddr = addr->sin_addr;

        std::string ipAddress(INET_ADDRSTRLEN, 0);
        inet_ntop(AF_INET, &inAddr, &ipAddress[0], INET_ADDRSTRLEN);
        socket.m_address = ipAddress;

        return (socket.m_fd != -1);
    }

    bool Receiver::Bind()
    {
        close(m_fd);
        m_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        struct sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(m_port);
        addr.sin_addr.s_addr = INADDR_ANY;

        int result = bind(m_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));

        /// Failed to bind socket address
        if(result == -1)
        {
            return false;
        }

        return true;
    }
}