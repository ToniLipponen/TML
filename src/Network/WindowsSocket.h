/// Windows sockets implementation

#include <TML/Network/Socket.h>
#include <TML/System/Logger.h>

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

namespace tml
{
    namespace Net
    {
        Socket::Socket()
        {
            m_fd = INVALID_SOCKET;
            WSADATA wsaData;

            auto result = WSAStartup(MAKEWORD(2, 2), &wsaData);

            if (result != 0)
            {
                Logger::ErrorMessage("WSAStartup failed");
            }
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

            if (iResult)
            {
                Logger::ErrorMessage("sFailed to connect to host");
                WSACleanup();
                return false;
            }

            for (ptr = result; ptr != nullptr; ptr = ptr->ai_next)
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

            if (m_fd == INVALID_SOCKET)
            {
                Logger::ErrorMessage("fdFailed to connect to host");
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
            auto result = send(m_fd, reinterpret_cast<const char *>(data), size, 0);
            return result != SOCKET_ERROR;
        }

        bool Socket::Receive(void *data, uint64_t size, uint64_t &received)
        {
            int64_t bytes = recv(m_fd, reinterpret_cast<char *>(data), size, 0);
            if (bytes == -1)
            {
                received = 0;
                return false;
            }
            received = bytes;
            return true;
        }
    }
}