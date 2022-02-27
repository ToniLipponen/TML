#include <TML/System/Platform.h>

#if defined(PLATFORM_UNIX) || defined(PLATFORM_LINUX)
    #include "Unix/Receiver_Impl.h"
#elif defined(PLATFORM_WINDOWS)
    #include "Win32/Receiver_Impl.h"
#endif