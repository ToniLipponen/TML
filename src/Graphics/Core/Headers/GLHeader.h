#pragma once
#include <TML/System/Platform.h>

#ifdef PLATFORM_WINDOWS
    #include <glad/gl.h>
#else
    #ifdef TML_USE_GLES
        #include <glad/gles2.h>
    #else
        #include <glad/gl.h>
    #endif
#endif
