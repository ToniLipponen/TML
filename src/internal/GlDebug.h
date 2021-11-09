#pragma once

// Disables OpenGL debugging
#ifndef TML_NO_GL_DEBUGGING
    #include <cassert>
    #define GL_CALL(x) x
//    #define GL_CALL(x) x; assert(CheckGLError() == 0)
#else
    #define GL_CALL(x) x
#endif

int CheckGLError();