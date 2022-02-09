#pragma once
#include "TML/System/Platform.h"

#ifdef TML_USE_GLES
    #ifdef PLATFORM_WINDOWS
        #include <glad/wgl.h>
    #else
        #include <glad/gles2.h>
    #endif
#else
    #include <glad/gl.h>
#endif