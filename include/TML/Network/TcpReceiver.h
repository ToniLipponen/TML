#pragma once
#include "TcpSocket.h"
#include <TML/Export.h>
#include <optional>

namespace tml
{
    class TML_API TcpReceiver : public TcpSocket
    {
    public:
        TcpReceiver();
        bool Listen(uint32_t port, const IpAddress& address = IpAddress(0,0,0,0));
        bool Accept(TcpSocket& socket);
    };
}