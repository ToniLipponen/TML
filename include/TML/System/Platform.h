#pragma once

#if defined(_WIN32)
    #define TML_PLATFORM_WINDOWS
#endif

#if defined(__linux__)
    #define TML_PLATFORM_LINUX
#endif

#if defined(__unix__)
    #define TML_PLATFORM_UNIX
    
    #if defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
        #define TML_PLATFORM_FREEBSD
    #elif defined(__ANDROID__)
        #define TML_PLATFORM_ANDROID
	#endif
#endif

#if defined(__APPLE__)
	#ifndef TML_PLATFORM_UNIX
    	#define TML_PLATFORM_UNIX
	#endif

    #define TML_PLATFORM_APPLE
#endif

