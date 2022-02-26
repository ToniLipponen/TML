#pragma once
#include <TML/Types.h>
#include "Context.h"

namespace tml
{
    namespace Net
    {
        class Socket
        {
        public:
            explicit Socket(tml::Net::Context& context);
            bool Connect(const std::string& address, ui16 port) noexcept;
            bool Disconnect();
            bool IsConnected();
            bool Send(const void* data, ui64 size);
            bool Receive(void* data, ui64 size, ui64& received);

        protected:
            void* m_socket = nullptr;
        };
    }
}