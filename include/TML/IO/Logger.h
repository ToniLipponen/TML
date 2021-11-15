#pragma once
#include <iostream>
#include <cstdarg>
#include <cstdlib>
#include "TML/Utilities/Platform.h"

namespace tml
{
    namespace Logger
    {
        #if PLATFORM_WINDOWS
            template<typename... Args> static void Message(cstring msg, Args... args) noexcept
            {
                std::printf("[Message]: ");
                std::printf(msg, args...);
                std::puts("");
            }

            template<typename... Args> static void InfoMessage(cstring msg, Args... args) noexcept
            {
                std::printf("[  Info ]: ");
                std::printf(msg, args...);
                std::puts("");
            }

            template<typename... Args> static void WarningMessage(cstring msg, Args... args) noexcept
            {
                std::printf("[Warning]: ");
                std::printf(msg, args...);
                std::puts("");
            }

            template<typename... Args> static void ErrorMessage(cstring msg, Args... args) noexcept
            {
                std::printf("[ Error ]: ");
                std::printf(msg, args...);
                std::puts("");
            }

        #else
            template<typename... Args> static void Message(cstring msg, Args... args) noexcept
            {
                std::printf("[\033[1;34mMessage\033[0m]: ");
                std::printf(msg, args...);
                std::puts("");
            }

            template<typename... Args> static void InfoMessage(cstring msg, Args... args) noexcept
            {
                std::printf("[\033[1;32m  Info \033[0m]: ");
                std::printf(msg, args...);
                std::puts("");
            }

            template<typename... Args> static void WarningMessage(cstring msg, Args... args) noexcept
            {
                std::printf("[\033[1;33mWarning\033[0m]: ");
                std::printf(msg, args...);
                std::puts("");
            }

            template<typename... Args> static void ErrorMessage(cstring msg, Args... args) noexcept
            {
                std::printf("[\033[1;31m Error \033[0m]: ");
                std::printf(msg, args...);
                std::puts("");
            }
        #endif
    }
}
