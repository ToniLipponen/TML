#pragma once

#include <cstdio>
#include "TML/Types.h"
#include <TML/System/Logger.h>

#if (TML_NO_DEBUGGING != 1)
	#include <cassert>
	#ifndef TML_ASSERT
		#define TML_ASSERT(expr, msg) if(!(expr)){ \
            tml::Logger::ErrorMessage("%s",msg);\
			assert(expr);\
			exit(1);\
		}
	#endif
#else
	#define TML_ASSERT(expr, msd) expr
#endif
