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

        bool Receiver::GetClient(Socket &socket)
        {
            struct sockaddr_in cliaddr;
            socklen_t cliaddrlen = sizeof(cliaddr);
            socket.m_fd = accept(m_fd, reinterpret_cast<sockaddr *>(&cliaddr), &cliaddrlen);
            if(socket.m_fd == -1)
                return false;
            return true;
        }
#else

#endif
    }
}