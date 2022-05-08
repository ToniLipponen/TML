#pragma once
#include <TML/System/Platform.h>
#include <string>
#include <cstdint>
#include <TML/Export.h>

namespace tml
{
    namespace Net
    {
        class TML_API Socket
        {
        public:
            Socket();
            virtual ~Socket();
            bool Connect(const std::string& address, uint32_t port) noexcept;
            bool Disconnect() const;
            bool IsConnected() const;
            bool Send(const void* data, uint64_t size) const;
            bool Receive(void* data, uint64_t size, uint64_t& received) const;

            friend class Receiver;
        protected:
            static std::string IpFromHostname(const std::string& hostname);
        protected:
            uint32_t m_port = 0;
#if defined(TML_PLATFORM_UNIX)
            int32_t m_fd;
#elif defined(TML_PLATFORM_WINDOWS)
            uint64_t m_fd;
#endif
        };
    }
}