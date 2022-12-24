#include <TML/Network/Http.h>
#include <vector>
#include <sstream>

namespace tml
{
    void HttpResponse::SetProperty(const std::string &property, const std::string &value) noexcept
    {
        m_properties[property] = value;
    }

    void HttpResponse::SetStatusCode(StatusCode code) noexcept
    {
        m_statusCode = code;
    }

    void HttpResponse::SetVersion(uint32_t major, uint32_t minor) noexcept
    {
        m_versionMajor = major;
        m_versionMinor = minor;
    }

    void HttpResponse::SetBody(const std::string& body) noexcept
    {
        m_body = body;
    }

    void HttpResponse::SetContent(const std::string& content)
    {
        if(content.empty())
        {
            return;
        }

        std::vector<std::string> lines;
        const auto header = content.substr(0, content.find("\r\n\r\n"));
        m_body = content.substr(content.find("\r\n\r\n") + 4, content.size());

        size_t pos = 0;

        while(pos < header.size())
        {
            auto lineEnd = header.find_first_of("\r\n", pos);

            if(lineEnd == std::basic_string<char>::npos)
            {
                break;
            }

            auto line = header.substr(pos, lineEnd);
            pos = lineEnd + 2;
            lines.push_back(line);
        }

        StatusFromString(lines.at(0));
        lines.erase(lines.begin());

        for(auto& line : lines)
        {
            const auto colPos = line.find_first_of(':');
            const auto property = line.substr(0, colPos);
            const auto value = line.substr(colPos, line.size());

            m_properties.insert(std::pair<std::string, std::string>(property, value));
        }
    }

    std::string HttpResponse::GetProperty(const std::string& property) const noexcept
    {
        if(m_properties.find(property) != m_properties.end())
        {
            return m_properties.at(property);
        }

        return "";
    }

    std::string HttpResponse::GetStatusText() const noexcept
    {
        return GetStatusTextFromStatusCode(m_statusCode);
    }

    std::string HttpResponse::GetStatusLine() const noexcept
    {
        return "HTTP/" + std::to_string(m_versionMajor) + "." + std::to_string(m_versionMinor) +
        " " + std::to_string(static_cast<uint32_t>(m_statusCode)) + GetStatusTextFromStatusCode(m_statusCode);
    }

    std::string HttpResponse::GetBody() const noexcept
    {
        return m_body;
    }

    HttpResponse::StatusCode HttpResponse::GetStatusCode() const noexcept
    {
        return m_statusCode;
    }

    std::string HttpResponse::GetContent() const noexcept
    {
        std::stringstream ss;
        ss << "HTTP/" << m_versionMajor << "." << m_versionMinor << " " << static_cast<uint32_t>(m_statusCode) <<
		" " << GetStatusTextFromStatusCode(m_statusCode) << "\n";

        for(auto& [property, value] : m_properties)
        {
            ss << property << ": " << value << "\n";
        }

        ss << "\r\n\r\n"; //!< End of header.
        ss << m_body;

        return ss.str();
    }

    void HttpResponse::StatusFromString(const std::string &str)
    {
        auto pos = str.find_first_of('/') + 1;

        m_versionMajor = std::stol(str.substr(pos, 1));
        pos += 2;

        m_versionMajor = std::stol(str.substr(pos, 1));
        pos += 2;

        m_statusCode   = static_cast<StatusCode>(std::stol(str.substr(pos, 3)));
    }

    std::string HttpResponse::GetStatusTextFromStatusCode(StatusCode code) noexcept
    {
        switch(code)
        {
            case StatusCode::Continue:                        return "Continue";
            case StatusCode::Switching_Protocols:             return "Switching Protocols";
            case StatusCode::Processing:                      return "Processing";
            case StatusCode::Early_Hints:                     return "Early Hints";
            case StatusCode::OK:                              return "OK";
            case StatusCode::Created:                         return "Created";
            case StatusCode::Accepted:                        return "Accepted";
            case StatusCode::Non_Authoritative_Information:   return "Non-Authoritative Information";
            case StatusCode::No_Content:                      return "No Content";
            case StatusCode::Reset_Content:                   return "Reset Content";
            case StatusCode::Partial_Content:                 return "Partial Content";
            case StatusCode::Multi_Status:                    return "Multi Status";
            case StatusCode::Already_Reported:                return "Already Reported";
            case StatusCode::IM_Used:                         return "IM Used";
            case StatusCode::Multiple_Choice:                 return "Continue";
            case StatusCode::Moved_Permanently:               return "Moved Permanently";
            case StatusCode::Found:                           return "Found";
            case StatusCode::See_Other:                       return "See Other";
            case StatusCode::Not_Modified:                    return "Not Modified";
            case StatusCode::Use_Proxy:                       return "Use Proxy";
            case StatusCode::Unused:                          return "Unused";
            case StatusCode::Temporary_Redirect:              return "Temporary Redirect";
            case StatusCode::Permanent_Redirect:              return "Permanent Redirect";
            case StatusCode::Bad_Request:                     return "Bad Request";
            case StatusCode::Unauthorized:                    return "Unauthorized";
            case StatusCode::Payment_Required:                return "Payment Required";
            case StatusCode::Forbidden:                       return "Forbidden";
            case StatusCode::Not_Found:                       return "Not Found";
            case StatusCode::Method_Not_Allowed:              return "Method Not Allowed";
            case StatusCode::Not_Acceptable:                  return "Not Accepted";
            case StatusCode::Proxy_Authentication_Failed:     return "Proxy Authentication Failed";
            case StatusCode::Request_Timeout:                 return "Request Timeout";
            case StatusCode::Conflict:                        return "Conflict";
            case StatusCode::Gone:                            return "Gone";
            case StatusCode::Length_Required:                 return "Length Required";
            case StatusCode::Precondition_Failed:             return "Precondition Failed";
            case StatusCode::Payload_Too_Large:               return "Payload Too Large";
            case StatusCode::URI_Too_Long:                    return "URI Too Long";
            case StatusCode::Unsupported_Media_Type:          return "Unsupported Media Type";
            case StatusCode::Range_Not_Satisfiable:           return "Range Not Satisfiable";
            case StatusCode::Expectation_Failed:              return "Expectation Failed";
            case StatusCode::Im_a_teapot:                     return "I'm A Teapot";
            case StatusCode::Misdirected_Request:             return "Misdirected Request";
            case StatusCode::Unprocessable_Entity:            return "Unprocessable Entity";
            case StatusCode::Locked:                          return "Locked";
            case StatusCode::Failed_Dependency:               return "Failed Dependency";
            case StatusCode::Too_Early:                       return "Too Early";
            case StatusCode::Upgrade_Required:                return "Upgrade Required";
            case StatusCode::Precondition_Required:           return "Precondition Required";
            case StatusCode::Too_Many_Requests:               return "Too Many Requests";
            case StatusCode::Request_Header_Fields_Too_Large: return "Request Header Fields Too Large";
            case StatusCode::Unavailable_For_Legal_Reasons:   return "Unavailable For Legal Reasons";
            case StatusCode::Internal_Server_Error:           return "Internal Server Error";
            case StatusCode::Not_Implemented:                 return "Not Implemented";
            case StatusCode::Bad_Gateway:                     return "Bad Gateway";
            case StatusCode::Service_Unavailable:             return "Service Unavailable";
            case StatusCode::Gateway_Timeout:                 return "Gateway Timeout";
            case StatusCode::HTTP_Version_Not_Supported:      return "HTTP Version Not Supported";
            case StatusCode::Variant_Also_Negotiates:         return "Variant Also Negotiates";
            case StatusCode::Insufficient_Storage:            return "Insufficient Storage";
            case StatusCode::Loop_Detected:                   return "Loop Detected";
            case StatusCode::Not_Extended:                    return "Not Extended";
			case StatusCode::Network_Authentication_Required: return "Network Authentication Required";
			default:                              			  return "Not Implemented";
        }
    }
}