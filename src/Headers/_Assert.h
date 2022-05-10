#pragma once
#include <cstdint>

#ifndef TML_NO_DEBUGGING
	#include <cassert>
    #include <cstdlib>
    #include <cstdio>

	#ifndef TML_ASSERT
		#define TML_ASSERT(expr, msg) if(!(expr)){ \
            printf("[Error]: %s\n", msg);\
			exit(1);\
		}
	#endif
#else
	#define TML_ASSERT(expr, msg) expr
#endif