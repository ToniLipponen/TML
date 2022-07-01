#pragma once
#include <TML/System/Platform.h>
#include <string>
#include <cstdint>
#include <TML/Export.h>

namespace tml::Net
{
    enum class SocketResult
    {
        OK,                 //!< All good
        Incomplete,
        InvalidAddress,
        FailedToConnect,
        FailedToBind,
        Error,              //!< Unknown error
    };

    class TML_API Socket
    {
    public:
        Socket();
        Socket(const Socket&) = delete;
        Socket(Socket&& other) = default;
        virtual ~Socket();

        Socket& operator=(const Socket&) = delete;
        Socket& operator=(Socket&& other) = default;

        SocketResult Connect(const std::string& address, uint32_t port) noexcept;
        SocketResult Disconnect();
        bool IsConnected() const;
        SocketResult Send(const void* data, uint64_t size, uint64_t& sent) const;
        SocketResult Receive(void* data, uint64_t size, uint64_t& received) const;
        SocketResult SetBlocking(bool blocking);
        bool GetBlocking() const; //!< Is the socket blocking?
        friend class Receiver;

    protected:
        static std::string IpFromHostname(const std::string& hostname);

    protected:
        bool m_blocking = false;
        uint32_t m_port = 0;
        std::string m_address; //!< IP address
#if defined(TML_PLATFORM_UNIX)
        int32_t m_fd;
#elif defined(TML_PLATFORM_WINDOWS)
        uint64_t m_fd;
#endif
    };
}