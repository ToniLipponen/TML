#include <TML/Network/Http.h>
#include <vector>
#include <iostream>
#include <sstream>

namespace tml::Net
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
        " " + std::to_string(m_statusCode) + GetStatusTextFromStatusCode(m_statusCode);
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
        ss << "HTTP/" << m_versionMajor << "." << m_versionMinor << " " << m_statusCode << " " << GetStatusTextFromStatusCode(m_statusCode) << "\n";

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
            case Continue:                        return "Continue";
            case Switching_Protocols:             return "Switching Protocols";
            case Processing:                      return "Processing";
            case Early_Hints:                     return "Early Hints";
            case OK:                              return "OK";
            case Created:                         return "Created";
            case Accepted:                        return "Accepted";
            case Non_Authoritative_Information:   return "Non-Authoritative Information";
            case No_Content:                      return "No Content";
            case Reset_Content:                   return "Reset Content";
            case Partial_Content:                 return "Partial Content";
            case Multi_Status:                    return "Multi Status";
            case Already_Reported:                return "Already Reported";
            case IM_Used:                         return "IM Used";
            case Multiple_Choice:                 return "Continue";
            case Moved_Permanently:               return "Moved Permanently";
            case Found:                           return "Found";
            case See_Other:                       return "See Other";
            case Not_Modified:                    return "Not Modified";
            case Use_Proxy:                       return "Use Proxy";
            case Unused:                          return "Unused";
            case Temporary_Redirect:              return "Temporary Redirect";
            case Permanent_Redirect:              return "Permanent Redirect";
            case Bad_Request:                     return "Bad Request";
            case Unauthorized:                    return "Unauthorized";
            case Payment_Required:                return "Payment Required";
            case Forbidden:                       return "Forbidden";
            case Not_Found:                       return "Not Found";
            case Method_Not_Allowed:              return "Method Not Allowed";
            case Not_Acceptable:                  return "Not Accepted";
            case Proxy_Authentication_Failed:     return "Proxy Authentication Failed";
            case Request_Timeout:                 return "Request Timeout";
            case Conflict:                        return "Conflict";
            case Gone:                            return "Gone";
            case Length_Required:                 return "Length Required";
            case Precondtion_Failed:              return "Precondition Failed";
            case Payload_Too_Large:               return "Payload Too Large";
            case URI_Too_Long:                    return "URI Too Long";
            case Unsupported_Media_Type:          return "Unsupported Media Type";
            case Range_Not_Satisfiable:           return "Range Not Satisfiable";
            case Expectation_Failed:              return "Expectation Failed";
            case Im_a_teapot:                     return "I'm A Teapot";
            case Misdirected_Request:             return "Misdirected Request";
            case Unprocessable_Entity:            return "Unprocessable Entity";
            case Locked:                          return "Locked";
            case Failed_Dependency:               return "Failed Dependency";
            case Too_Early:                       return "Too Early";
            case Upgrade_Required:                return "Upgrade Required";
            case Precondition_Required:           return "Precondition Required";
            case Too_Many_Requests:               return "Too Many Requests";
            case Request_Header_Fields_Too_Large: return "Request Header Fields Too Large";
            case Unavailable_For_Legal_Reasons:   return "Unavailable For Legal Reasons";
            case Internal_Server_Error:           return "Internal Server Error";
            case Not_Implemented:                 return "Not Implemented";
            case Bad_Gateway:                     return "Bad Gateway";
            case Service_Unavailable:             return "Service Unavailable";
            case Gateway_Timeout:                 return "Gateway Timeout";
            case HTTP_Version_Not_Supported:      return "HTTP Version Not Supported";
            case Variant_Also_Negotiates:         return "Variant Also Negotiates";
            case Insufficient_Storage:            return "Insufficient Storage";
            case Loop_Detected:                   return "Loop Detected";
            case Not_Extended:                    return "Not Extended";
            case Network_Authentication_Required: return "Network Authentication Required";
            default:                              return "Not Implemented";
        }
    }
}