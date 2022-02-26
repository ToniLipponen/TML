#include <TML/Graphics.h>
#include <TML/Network/Socket.h>
#include <TML/Network/Receiver.h>
#include <iostream>

int main()
{
    tml::RenderWindow window(800, 600, "Net test");

    tml::Net::Socket socket;
    socket.Connect("93.184.216.34", 80);

    const char request[] = "GET /index.html HTTP/1.1\r\nHost: example.com\r\nConnection: close\r\n\r\n";
    if(!socket.Send(request, sizeof(request)))
        std::cout << "Failed to send\n";

    char buffer[8000];
    tml::ui64 read = 0;
    socket.Receive(buffer, 8000, read);
    std::cout << buffer << "\n\nReceived bytes: " << read << std::endl;

    while(!window.ShouldClose())
    {
        auto event = window.PollEvents();
        if(event.type == tml::Event::Closed)
            window.Close();

        window.Clear();
        window.DrawCircle(window.GetSize() / 2, 100, tml::Color::Green);
        window.Display();
    }
}