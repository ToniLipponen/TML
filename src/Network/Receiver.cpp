#include <TML/Network/Receiver.h>
#include <TML/System/Logger.h>

#if defined(PLATFORM_UNIX) || defined(PLATFORM_LINUX)
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#else

#endif
namespace tml
{
    namespace Net
    {
#if defined(PLATFORM_UNIX) || defined(PLATFORM_LINUX)
        Receiver::Receiver() = default;

        bool Receiver::Listen(uint32_t port)
        {
            struct sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
            bind(m_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
            return listen(m_fd, 1) != -1;
        }

        bool Receiver::Accept(Socket& socket)
        {
            struct sockaddr_in cliaddr;
            socklen_t cliaddrlen = sizeof(cliaddr);
            socket.m_fd = accept(socket.m_fd, reinterpret_cast<sockaddr *>(&cliaddr), &cliaddrlen);
            return (socket.m_fd != -1);
        }
#else

#endif
    }
}