#include <TML/Network/Connection.h>

namespace tml
{
    namespace Net
    {
        Connection::Connection(Context& context)
        : m_socket(context)
        {

        }

        void Connection::SetAddress(const std::string &address, ui16 port)
        {
            m_address = address;
            m_port = port;
        }

        bool Connection::Connect()
        {
            return m_socket.Connect(m_address, m_port);
        }

        bool Connection::Connect(const std::string &address, ui16 port)
        {
            SetAddress(address, port);
            return m_socket.Connect(m_address, m_port);
        }

        bool Connection::Disconnect()
        {
            return m_socket.Disconnect();
        }

        bool Connection::HasConnection()
        {
            return m_socket.IsConnected();
        }

        bool Connection::Send(const Packet& packet)
        {
            IdentityPacket identityPacket;
            identityPacket.packet = packet;
            m_outputQueue.Push(identityPacket);
        }
    }
}