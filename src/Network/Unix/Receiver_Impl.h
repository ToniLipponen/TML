#include <TML/Network/Receiver.h>
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
        Receiver::Receiver() = default;

        bool Receiver::Listen(uint32_t port)
        {
            if(m_port != port)
            {
                m_port = port;
                auto result = Bind(); // Rebind.
                if(!result)
                    return false;
            }
            return listen(m_fd, 5) != -1;
        }

        bool Receiver::Accept(Socket& socket)
        {
            struct sockaddr_in cliaddr;
            socklen_t cliaddrlen = sizeof(cliaddr);
            socket.m_fd = accept(m_fd, reinterpret_cast<sockaddr *>(&cliaddr), &cliaddrlen);
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

            if(result == -1)
            {
                Logger::ErrorMessage("Failed to bind socket address");
                return false;
            }
            return true;
        }
    }
}