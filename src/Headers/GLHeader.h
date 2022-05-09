#pragma once
#include <TML/System/Platform.h>
#include <glad/include/glad/gl.h>

#ifndef TML_NO_GL_DEBUGGING
#include <cassert>
    #define GL_CALL(x) x; assert(CheckGLError() == 0)
#else
    #define GL_CALL(x) x
#endif

int CheckGLError();
void GLAPIENTRY GLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param);