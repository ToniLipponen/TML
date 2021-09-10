
#pragma once

#include "../../include/TML/Types.h"
#include <cstdio>

#if (TL_NO_DEBUGGING != 1)
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
	#define TML_ASSERT(expr, msd)
#endif

#include <iostream>
#include <stdarg.h>
#include <stdlib.h>

namespace tml
{
namespace Logger
{
	template<typename... Args> static void Message(cstring msg, Args... args)
	{
		std::printf("[\033[1;33mMessage\033[0m]: ");
		std::printf(msg, args...);
		std::puts("");
	}
	template<typename... Args> static void InfoMessage(cstring msg, Args... args)
	{
		std::printf("[\033[1;32m  Info \033[0m]: ");
		std::printf(msg, args...);
		std::puts("");
	}
	template<typename... Args> static void WarningMessage(cstring msg, Args... args)
	{
		std::printf("[\033[1;31mWarning\033[0m]: ");
		std::printf(msg, args...);
		std::puts("");
	}
	template<typename... Args> static void ErrorMessage(cstring msg, Args... args)
	{
		std::printf("[\033[1;31m Error \033[0m]: ");
		std::printf(msg, args...);
		std::puts("");
	}
}
};
