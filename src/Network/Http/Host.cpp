#include <TML/Network/Http.h>
#include <TML/System/Logger.h>

namespace tml
{
    namespace Net
    {
        HttpHost::HttpHost()
        {

        }

        HttpHost::HttpHost(std::string address, uint32_t port)
        : m_address(std::move(address))
        {
            auto result = m_socket.Connect(m_address, port);
            if(!result)
            {
                Logger::ErrorMessage("Failed to connect to host -> %s", address.c_str());
            }
        }

        bool HttpHost::Send(HttpRequest &request) noexcept
        {
            request.SetProperty("Host", m_address);
            const auto message = std::move(request.GetRequestString());
            return m_socket.Send(message.data(), message.size());
        }

        bool HttpHost::GetResponse(HttpResponse& response) noexcept
        {
            const auto responseString = std::move(GetResponseString());
            response.SetContent(responseString);
            return !responseString.empty();
        }

        std::string HttpHost::GetResponseString() noexcept
        {
            uint64_t received = 1;

            std::string response;
            std::string buffer;
            buffer.reserve(512);

            while(received != 0)
            {
                m_socket.Receive(&buffer[0], 512, received);
                response += buffer;
            }
            return std::move(response);
        }
    }
}