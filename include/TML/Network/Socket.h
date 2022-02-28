#pragma once
#include <TML/Types.h>
#include <TML/System/Platform.h>

namespace tml
{
    namespace Net
    {
        class Socket
        {
        public:
            Socket();
            virtual ~Socket();
            bool Connect(const std::string& address, uint32_t port) noexcept;
            bool Disconnect() const;
            bool IsConnected();
            bool Send(const void* data, uint64_t size) const;
            bool Receive(void* data, uint64_t size, uint64_t& received) const;

            friend class Receiver;
        protected:
            static std::string IpFromHostname(const std::string& hostname);
        protected:
            uint32_t m_port = 0;
            int64_t m_fd;
        };
    }
}