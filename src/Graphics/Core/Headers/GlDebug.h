#pragma once

// Disables OpenGL debugging
#ifndef TML_NO_GL_DEBUGGING
    #include <cassert>
    #define GL_CALL(x) x; assert(CheckGLError() == 0)
#else
    #define GL_CALL(x) x
#endif

int CheckGLError();
void GLMessageCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, void const* user_param);