#include <TML/System/Platform.h>

#if defined(TML_PLATFORM_UNIX)
    #include "Unix/Socket_Impl.h"
#elif defined(TML_PLATFORM_WINDOWS)
    #include "Win32/Socket_Impl.h"
#endif