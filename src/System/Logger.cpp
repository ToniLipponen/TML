#include <TML/System/Logger.h>

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/printf.h>
#include <fmt/color.h>

namespace tml
{
    void Logger::Message(const std::string& message)
    {
        if(m_logLevel != LogLevel::None && static_cast<int>(m_logLevel) <= 1)
        {
            LogMessage(message);
        }
    }

    void Logger::Warning(const std::string& message)
    {
        if(m_logLevel != LogLevel::None && static_cast<int>(m_logLevel) <= 2)
        {
            LogWarning(message);
        }
    }

    void Logger::Error(const std::string& message)
    {
        if(m_logLevel != LogLevel::None && static_cast<int>(m_logLevel) <= 3)
        {
            LogError(message);
        }
    }

    void Logger::SetLogLevel(LogLevel loggingLevel)
    {
        m_logLevel = loggingLevel;
    }

    Logger& Logger::GetDefault()
    {
        static Logger logger;
        return logger;
    }

    void Logger::LogMessage(const std::string& message)
    {
        fmt::print(fg(fmt::color::white), "[Message]: {}\n", message);
    } 

    void Logger::LogWarning(const std::string& message)
    {
        fmt::print(fg(fmt::color::yellow), "[Warning]: {}\n", message);
    } 

    void Logger::LogError(const std::string& message)
    {
        fmt::print(fg(fmt::color::red), "[Error]: {}\n", message);
    }
}