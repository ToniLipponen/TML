#pragma once
#include "Socket.h"

namespace tml
{
    namespace Net
    {
        class Receiver : public Socket
        {
        public:
            Receiver();
            bool Listen(uint32_t port);
            bool Accept(Socket& socket);

        private:
            bool Bind();

        protected:
            uint32_t m_port = 8080;
        };
    }
}