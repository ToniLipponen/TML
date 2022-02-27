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
            bool Disconnect();
            bool IsConnected();
            bool Send(const void* data, uint64_t size);
            bool Receive(void* data, uint64_t size, uint64_t& received);

            friend class Receiver;
        protected:
            uint32_t m_port = 0;
            int64_t m_fd;
        };
    }
}