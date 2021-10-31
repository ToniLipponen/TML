#pragma once
#include <iostream>
#include <cstdarg>
#include <cstdlib>
#include "Utilities/Platform.h"

namespace tml
{
    namespace Logger
    {
        #if PLATFORM_WINDOWS
            template<typename... Args> static void Message(cstring msg, Args... args) noexcept
            {
                std::printf("[Message]: %s\n", msg, args...);
            }

            template<typename... Args> static void InfoMessage(cstring msg, Args... args) noexcept
            {
                std::printf("[  Info ]: %s\n", msg, args...);
            }

            template<typename... Args> static void WarningMessage(cstring msg, Args... args) noexcept
            {
                std::printf("[Warning]: %s\n", msg, args...);
            }

            template<typename... Args> static void ErrorMessage(cstring msg, Args... args) noexcept
            {
                std::printf("[ Error ]: %s\n", msg, args...);
            }

        #else
            template<typename... Args> static void Message(cstring msg, Args... args) noexcept
            {
                std::printf("[\033[1;33mMessage\033[0m]: %s\n", msg, args...);
            }

            template<typename... Args> static void InfoMessage(cstring msg, Args... args) noexcept
            {
                std::printf("[\033[1;32m  Info \033[0m]: %s\n", msg, args...);
            }

            template<typename... Args> static void WarningMessage(cstring msg, Args... args) noexcept
            {
                std::printf("[\033[1;31mWarning\033[0m]: %s\n", msg, args...);
            }

            template<typename... Args> static void ErrorMessage(cstring msg, Args... args) noexcept
            {
                std::printf("[\033[1;33m Error \033[0m]: %s\n", msg, args...);
            }
        #endif
    }
}
