#pragma once
#include "Packet.h"
#include "Queue.h"
#include "Socket.h"
#include "Context.h"

/*
 *
 * int main()
 * {
 *      Context context;
 *      Connection connection("192.168.0.1", 1445);
 *      Packet packet;
 *
 *      if(connection.IsConnected())
 *          connection.Send(packet);
 *
 *      connection.Close();
 *
 *
 *      Context context;
 *      Receiver receiver(1445);
 *      receiver.OnConnect([](IP& ip)
 *      {
 *          return true; /// Accept
 *      });
 * }
 */


namespace tml
{
    namespace Net
    {
        class Connection
        {
        public:
            Connection(Context& context);
            void SetAddress(const std::string& address, ui16 port);
            bool Connect();
            bool Connect(const std::string& address, ui16 port);
            bool Disconnect();
            bool HasConnection();
            bool Send(const Packet& packet);

        protected:
            std::string m_address;
            ui16 m_port = 0;
            Socket m_socket;
            Queue<Packet> m_inputQueue;
            Queue<IdentityPacket> m_outputQueue;
        };
    }
}