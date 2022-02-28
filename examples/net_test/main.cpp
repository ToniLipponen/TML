#include <TML/Network.h>
#include <iostream>

int main()
{
    tml::Net::HttpHost host("www.example.com", 80);
    tml::Net::HttpRequest request(tml::Net::HttpRequest::GET, "/index.html");

    if(!host.Send(request))
        std::cout << "Failed to send\n";

    tml::Net::HttpResponse response;
    if(!host.GetResponse(response))
        std::cout << "No response\n";

    std::cout << response.GetBody() << std::endl;
    std::cout << response.GetStatusText() << std::endl;
    return 0;
}