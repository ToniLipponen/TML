#include <TML/System/Platform.h>

#if defined(PLATFORM_UNIX) || defined(PLATFORM_LINUX)
    #include "Unix/Socket_Impl.h"
#else
    #include "Win32/Socket_Impl.h"
#endif