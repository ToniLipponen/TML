#include <TML/System/Platform.h>

#if defined(TML_PLATFORM_UNIX)
    #include "Unix/Receiver_Impl.h"
#elif defined(TML_PLATFORM_WINDOWS)
    #include "Win32/Receiver_Impl.h"
#endif