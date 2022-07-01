#pragma once
#include "Socket.h"
#include <TML/Export.h>

namespace tml::Net
{
    class TML_API Receiver : public Socket
    {
    public:
        Receiver();
        bool Listen(uint32_t port);
        bool Accept(Socket& socket);

    private:
        bool Bind();
    };
}