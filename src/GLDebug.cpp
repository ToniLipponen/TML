#include "../include/GlDebug.h"
#include "../include/Assert.h"
#include "../include/glad/glad.h"

int CheckGLError()
{
    const auto e = glad_glGetError();
    if(!e) return 0;

    switch(e)
    {
        case 1280: tl::Logger::ErrorMessage("GL_INVALID_ENUM");                 break;
        case 1281: tl::Logger::ErrorMessage("GL_INVALID_VALUE");                break;
        case 1282: tl::Logger::ErrorMessage("GL_INVALID_OPERATION");            break;
        case 1283: tl::Logger::ErrorMessage("GL_STACK_OVERFLOW");               break;
        case 1284: tl::Logger::ErrorMessage("GL_STACK_UNDERFLOW");              break;
        case 1285: tl::Logger::ErrorMessage("GL_OUT_OF_MEMORY");                break;
        case 1286: tl::Logger::ErrorMessage("GL_INVALID_FRAMEBUFFER_OPERATION");break;
        default:break;
    }
    return 1;
}