#pragma once

#ifdef _WIN32
    #define PLATFORM_WINDOWS
#endif
#ifdef __unix__
    #define PLATFORM_UNIX
#endif
#ifdef __linux__
    #define PLATFORM_LINUX
#endif
#ifdef __APPLE__
    #define PLATFORM_APPLE
#endif
