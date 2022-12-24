#include <TML/Network/Http.h>

namespace tml
{
    HttpHost::HttpHost(std::string address, uint32_t port)
    : m_address(std::move(address))
    {
        m_socket.Connect(m_address, port);
    }

    bool HttpHost::Connect(std::string address, uint32_t port) noexcept
    {
        m_address = std::move(address);
        const auto result = m_socket.Connect(m_address, port);
        return result == Socket::Result::OK;
    }

    bool HttpHost::Send(HttpRequest& request) noexcept
    {
        request.SetProperty("Host", m_address);
        const auto message = request.GetRequestString();

        uint64_t sent = 0;
        Socket::Result result = m_socket.Send(message.data(), message.size(), sent);

        uint64_t totalSent = sent;

        while(result != Socket::Result::OK || totalSent < message.length())
        {
            result = m_socket.Send(message.data() + totalSent, message.size() - totalSent, sent);
            totalSent += sent;

            if(result == Socket::Result::Error)
            {
                return false;
            }
        }

        return true;
    }

    bool HttpHost::GetResponse(HttpResponse& response) noexcept
    {
        const auto responseString = GetResponseString();
        response.SetContent(responseString);
        return !responseString.empty();
    }

    std::string HttpHost::GetResponseString() noexcept
    {
        uint64_t received = 1;

        std::string response;
        char buffer[512];

        while(received != 0)
        {
            m_socket.Receive(&buffer[0], 512, received);
            response.append(buffer, received);
        }

        return response;
    }
}