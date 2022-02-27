/// Windows sockets implementation

#include <TML/Network/Socket.h>
#include <TML/System/Logger.h>

#include "NetworkContext.h"

namespace tml
{
    namespace Net
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

        bool Socket::Connect(const std::string &address, uint32_t port) noexcept
        {
            struct addrinfo *result = NULL, *ptr = NULL, hints;

            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;

            auto iResult = getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &result);

            if(iResult != 0)
            {
                Logger::ErrorMessage("Failed to get host address info");
                WSACleanup();
                return false;
            }

            for(ptr = result; ptr != nullptr; ptr = ptr->ai_next)
            {
                m_fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
                if (m_fd == INVALID_SOCKET)
                {
                    Logger::ErrorMessage("Invalid socket");
                    WSACleanup();
                    return false;
                }

                iResult = connect(m_fd, ptr->ai_addr, ptr->ai_addrlen);
                if (iResult == SOCKET_ERROR)
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
                Logger::ErrorMessage("Failed to connect to host");
                WSACleanup();
                return false;
            }

            return true;
        }

        bool Socket::Disconnect()
        {
            return closesocket(m_fd);
        }

        bool Socket::IsConnected()
        {
            return m_fd != INVALID_SOCKET;
        }

        bool Socket::Send(const void *data, uint64_t size)
        {
            auto result = send(m_fd, reinterpret_cast<const char*>(data), static_cast<int>(size), 0);
            if(result == SOCKET_ERROR)
            {
                Logger::ErrorMessage("Failed to send data");
                closesocket(m_fd);
                return false;
            }
            return true;
        }

        bool Socket::Receive(void *data, uint64_t size, uint64_t &received)
        {
            int64_t bytes = recv(m_fd, reinterpret_cast<char *>(data), static_cast<int>(size), 0);
            if(bytes < 0)
            {
                Logger::ErrorMessage("Failed to receive data");
                received = 0;
                return false;
            }
            else if(bytes == 0)
            {
                received = 0;
                return false;
            }
            received = bytes;
            return true;
        }
    }
}