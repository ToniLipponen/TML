#pragma once
#include <TML/System/Platform.h>

#ifdef TML_STATIC
    #define TML_API
#else
    #if defined(PLATFORM_WINDOWS)
        #if defined(TML_EXPORTING)
            #define TML_API __declspec(dllexport)
        #else
            #define TML_API __declspec(dllimport)
        #endif

    #if defined(_MSC_VER)
        #pragma warning(disable: 4251)
    #endif

    #else //!< Unix platforms
        /// I'm not sure this is even needed
        #define TML_API __attribute__ ((__visibility__ ("default")))
    #endif
#endif