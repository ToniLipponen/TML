#pragma once
#include <cstdint>
#include <cassert>
#include <cstdlib>
#include <cstdio>

#ifndef TML_NO_DEBUGGING
    #define TML_ASSERT(expr, msg) if(!(expr)){ \
        printf("[Error]: %s\n", msg);\
        exit(1);\
    }
#else
	#define TML_ASSERT(expr, msg) expr
#endif