#include "../Headers/GLHeader.h"
#include <cstdio>

int CheckGLError()
{
    const auto e = glad_glGetError();

    if(e != GL_NO_ERROR)
    {
        switch(e)
        {
            case 1280: std::perror("[Error]: GL_INVALID_ENUM");                 break;
            case 1281: std::perror("[Error]: GL_INVALID_VALUE");                break;
            case 1282: std::perror("[Error]: GL_INVALID_OPERATION");            break;
            case 1283: std::perror("[Error]: GL_STACK_OVERFLOW");               break;
            case 1284: std::perror("[Error]: GL_STACK_UNDERFLOW");              break;
            case 1285: std::perror("[Error]: GL_OUT_OF_MEMORY");                break;
            case 1286: std::perror("[Error]: GL_INVALID_FRAMEBUFFER_OPERATION");break;
            default:                                                               break;
        }

        return 1;
    }

    return 0;
}

#ifndef TML_USE_GLES
void GLAPIENTRY GLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const GLchar* message, const void*)
{
    auto const src_str = [source]()
    {
        switch (source)
        {
            case GL_DEBUG_SOURCE_API:               return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     return "WINDOW SYSTEM";
            case GL_DEBUG_SOURCE_SHADER_COMPILER:   return "SHADER COMPILER";
            case GL_DEBUG_SOURCE_THIRD_PARTY:       return "THIRD PARTY";
            case GL_DEBUG_SOURCE_APPLICATION:       return "APPLICATION";
            case GL_DEBUG_SOURCE_OTHER:             return "OTHER";
            default:                                return "UNKNOWN";
        }
    }();

    auto const type_str = [type]()
    {
        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR:                   return "ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:     return "DEPRECATED_BEHAVIOR";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:      return "UNDEFINED_BEHAVIOR";
            case GL_DEBUG_TYPE_PORTABILITY:             return "PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE:             return "PERFORMANCE";
            case GL_DEBUG_TYPE_MARKER:                  return "MARKER";
            case GL_DEBUG_TYPE_OTHER:                   return "OTHER";
            default:                                    return "UNKNOWN";
        }
    }();

    auto const severity_str = [severity]()
    {
        switch(severity)
        {
            case GL_DEBUG_SEVERITY_NOTIFICATION:    return "NOTIFICATION";
            case GL_DEBUG_SEVERITY_LOW:             return "LOW";
            case GL_DEBUG_SEVERITY_MEDIUM:          return "MEDIUM";
            case GL_DEBUG_SEVERITY_HIGH:            return "HIGH";
            default:                                return "UNKNOWN";
        }
    }();

    std::fprintf(stderr, "[%s]: %s, %s, %d, %s\n", type_str, src_str,severity_str, id, message);
}
#endif