#pragma once
#include <TML/Export.h>
#include <string>

namespace tml
{
    class TML_API Logger
    {
    public:
        enum class LogLevel
        {
            None    = 0, // Don't log anything
            Verbose = 1, // All messages
            Warning = 2, // Warnings and errors
            Error   = 3, // Errors only
        };
    public:
        void Message(const std::string& message);
        void Warning(const std::string& message);
        void Error(const std::string& message);
        void SetLogLevel(LogLevel logLevel);
        static Logger& GetDefault();
    private:
        virtual void LogMessage(const std::string& message); 
        virtual void LogWarning(const std::string& message); 
        virtual void LogError(const std::string& message);
    private:
        LogLevel m_logLevel = LogLevel::Error;
    };
}