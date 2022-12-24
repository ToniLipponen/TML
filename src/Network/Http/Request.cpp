#include <TML/Network/Http.h>

namespace tml
{
    HttpRequest::HttpRequest() = default;

    HttpRequest::HttpRequest(Method method, std::string URI, std::string body)
    : m_method(method), m_uri(std::move(URI)), m_body(std::move(body))
    {

    }

    void HttpRequest::SetVersion(uint32_t major, uint32_t minor)
    {
        m_versionMajor = major;
        m_versionMinor = minor;
    }

    void HttpRequest::SetMethod(Method method)
    {
        m_method = method;
    }

    void HttpRequest::SetURI(std::string URI)
    {
        m_uri = std::move(URI);
    }

    void HttpRequest::SetProperty(std::string property, std::string value)
    {
        m_properties.insert(std::pair<std::string, std::string>(std::move(property), std::move(value)));
    }

    void HttpRequest::SetBody(std::string body)
    {
        m_body = std::move(body);
    }

    std::string HttpRequest::GetRequestString() const noexcept
    {
        std::string request;
        request.reserve(512);

        switch(m_method)
        {
            case Method::Get:       request += "GET ";      break;
            case Method::Head:      request += "HEAD ";     break;
            case Method::Post:      request += "POST ";     break;
            case Method::Put:       request += "PUT ";      break;
            case Method::Delete:    request += "DELETE ";   break;
            case Method::Connect:   request += "CONNECT ";  break;
            case Method::Options:   request += "OPTIONS ";  break;
            case Method::Trace:     request += "TRACE ";    break;
			case Method::Patch:     request += "PATCH ";    break;
            default:                                break;
        }

        request += m_uri;
        request += " HTTP/" + std::to_string(m_versionMajor) + "." + std::to_string(m_versionMinor) + "\r\n";

        for(auto& i : m_properties)
        {
            request += i.first + ": " + i.second + "\r\n";
        }

        request += m_body;
        request += "\r\n\r\n";
        return request;
    }
}