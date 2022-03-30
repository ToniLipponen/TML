#pragma once
#include "TML/System/Platform.h"

#ifdef PLATFORM_WINDOWS
    #include <glad/gl.h>
#else
    #ifdef TML_USE_GLES
        #include <glad/gles2.h>
    #else
        #include "glad/gl.h"
    #endif
#endif

// Disables OpenGL debugging
#ifndef TML_NO_GL_DEBUGGING
#include <cassert>
    #define GL_CALL(x) x; assert(CheckGLError() == 0)
#else
    #define GL_CALL(x) x
#endif

int CheckGLError();
void GLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param);