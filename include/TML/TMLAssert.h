#pragma once
#include <cstdint>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <TML/System/Logger.h>

#ifndef TML_NO_DEBUGGING
    #define TML_ASSERT(expr, msg) if(!(expr)){ \
        tml::Logger::GetDefault().Error(msg); \
        assert(expr);\
    }
#else
	#define TML_ASSERT(expr, msg) expr
#endif