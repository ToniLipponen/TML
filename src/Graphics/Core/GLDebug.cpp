#include "../../Headers/_Assert.h"
#include "../../Headers/GLHeader.h"
#include <iostream>
#include <TML/System/Logger.h>

int CheckGLError()
{
    const auto e = glad_glGetError();
    if(e == GL_NO_ERROR)
        return 0;

    switch(e)
    {
        case 1280: tml::Logger::ErrorMessage("GL_INVALID_ENUM");                 break;
        case 1281: tml::Logger::ErrorMessage("GL_INVALID_VALUE");                break;
        case 1282: tml::Logger::ErrorMessage("GL_INVALID_OPERATION");            break;
        case 1283: tml::Logger::ErrorMessage("GL_STACK_OVERFLOW");               break;
        case 1284: tml::Logger::ErrorMessage("GL_STACK_UNDERFLOW");              break;
        case 1285: tml::Logger::ErrorMessage("GL_OUT_OF_MEMORY");                break;
        case 1286: tml::Logger::ErrorMessage("GL_INVALID_FRAMEBUFFER_OPERATION");break;
        default:                                                                      break;
    }
    return 1;
}

#ifndef TML_USE_GLES
void GLAPIENTRY GLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param)
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

    std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << "\n";
}
#endif