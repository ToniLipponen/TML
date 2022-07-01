#include <TML/Network.h>
#include <iostream>
#include <vector>

int main()
{
    tml::Net::Receiver receiver;
    std::vector<tml::Net::Socket> sockets;

    while(1)
    {
        tml::Net::Socket socket;
        receiver.Listen(12000);
        receiver.Accept(socket);
        sockets.emplace_back(std::move(socket));
        std::cout << "Connected\n";
    }
    return 0;
}