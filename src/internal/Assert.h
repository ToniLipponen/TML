#pragma once

#include "../../include/TML/Types.h"
#include <cstdio>

#if (TML_NO_DEBUGGING != 1)
	#include <cassert>
	#ifndef TML_ASSERT
		#define TML_ASSERT(expr, msg) if(!(expr)){ \
			std::printf("[\033[1;31m Error \033[0m]: "); \
			std::puts(msg); \
			assert(expr);\
			exit(1);\
		}
	#endif
#else
	#define TML_ASSERT(expr, msd) expr
#endif

#include <TML/Logger.h>