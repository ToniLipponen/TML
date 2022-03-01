#include <TML/Network/Receiver.h>
#include <TML/System/Logger.h>
#include "NetworkContext.h"

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
            if(listen(m_fd, 5) == SOCKET_ERROR)
            {
                Logger::ErrorMessage("Failed to listen to port");
                closesocket(m_fd);
                return false;
            }
            return true;
        }

        bool Receiver::Accept(Socket& socket)
        {
            socket.m_fd = accept(m_fd, nullptr, nullptr);
            if(socket.m_fd == INVALID_SOCKET)
            {
                Logger::ErrorMessage("Failed to accept client");
                closesocket(m_fd);
                return false;
            }
            return true;
        }

        bool Receiver::Bind()
        {
            struct addrinfo *result = NULL, *ptr = NULL, hints{};
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;
            hints.ai_flags = AI_PASSIVE;

            auto iResult = getaddrinfo(nullptr, std::to_string(m_port).c_str(), &hints, &result);

            if(iResult != 0)
            {
                Logger::ErrorMessage("Failed to get address info");
                return false;
            }

            m_fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
            if(m_fd == INVALID_SOCKET)
            {
                Logger::ErrorMessage("Failed to create a host socket");
                freeaddrinfo(result);
                return false;
            }

            iResult = bind(m_fd, result->ai_addr, result->ai_addrlen);
            if(iResult == SOCKET_ERROR)
            {
                Logger::ErrorMessage("Failed to bind socket");
                freeaddrinfo(result);
                closesocket(m_fd);
                return false;
            }
            freeaddrinfo(result);
            return true;
        }
    }
}