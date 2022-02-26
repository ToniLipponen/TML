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
            bool GetClient(Socket& socket);
        };
    }
}