#pragma once

#include <cstdint>
#include "TML/System/Logger.h"

#ifndef TML_NO_DEBUGGING
	#include <cassert>
	#ifndef TML_ASSERT
		#define TML_ASSERT(expr, msg) if(!(expr)){ \
            tml::Logger::ErrorMessage("%s", msg);\
			assert(expr);\
			exit(1);\
		}
	#endif
#else
	#define TML_ASSERT(expr, msd) expr
#endif
