#include <TML/System/Platform.h>

#if defined(TML_PLATFORM_UNIX)
    #include "Unix/Socket_Impl.h"
#else
    #include "Win32/Socket_Impl.h"
#endif