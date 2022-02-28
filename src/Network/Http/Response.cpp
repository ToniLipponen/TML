#include <TML/Network/Http.h>
#include <vector>
#include <iostream>

namespace tml
{
    namespace Net
    {
        std::string HttpResponse::GetStatusText() const noexcept
        {
            return m_statusText;
        }

        std::string HttpResponse::GetStatusLine() const noexcept
        {
            return "HTTP/" + std::to_string(m_versionMajor) + "." + std::to_string(m_versionMinor) +
            " " + std::to_string(m_statusCode) + m_statusText;
        }

        std::string HttpResponse::GetBody() const noexcept
        {
            return m_body;
        }

        HttpResponse::StatusCode HttpResponse::GetStatusCode() const noexcept
        {
            return m_statusCode;
        }

        std::string HttpResponse::GetProperty(const std::string& property) const noexcept
        {
            if(m_properties.find(property) != m_properties.end())
            {
                return m_properties.at(property);
            }
            return "";
        }

        void HttpResponse::SetContent(const std::string &content)
        {
            if(content.empty())
                return;
            std::vector<std::string> lines;
            const auto header = content.substr(0, content.find("\r\n\r\n"));
            m_body = content.substr(content.find("\r\n\r\n")+4, content.size());

            size_t pos = 0;
            while(pos < header.size())
            {
                auto lineEnd = header.find_first_of("\r\n", pos);
                if(lineEnd == std::basic_string<char>::npos)
                    break;
                auto line = header.substr(pos, lineEnd);
                pos = lineEnd+2;
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

        void HttpResponse::StatusFromString(const std::string &str)
        {
            auto pos = str.find_first_of('/')+1;

            m_versionMajor = std::stol(str.substr(pos, 1));
            pos += 2;

            m_versionMajor = std::stol(str.substr(pos, 1));
            pos += 2;

            m_statusCode   = (StatusCode)std::stol(str.substr(pos, 3));
            m_statusText   = str.substr(str.find_first_of(' ', pos)+1);
        }

    }
}