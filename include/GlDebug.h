#pragma once
// Disables OpenGL debugging
#ifndef NO_GL_DEBUGGING
    #include <cassert>
    #define GL_CALL(x) x; assert(CheckGLError() == 0)
#else
    #define GL_CALL(x) x
#endif

int CheckGLError();