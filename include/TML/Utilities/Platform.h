#pragma once

#ifdef _WIN32
    #define PLATFORM 0
#elif __unix__
    #define PLATFORM 1
#elif __linux__
    #define PLATFORM 2
#elif __APPLE__
    #define PLATFORM 3
#else
    #define PLATFORM -1
#endif

#define PLATFORM_WINDOWS    (PLATFORM ==  0)
#define PLATFORM_UNIX       (PLATFORM ==  1)
#define PLATFORM_LINUX      (PLATFORM ==  2)
#define PLATFORM_APPLE      (PLATFORM ==  3)
#define PLATFORM_UNKNOWN    (PLATFORM == -1)
