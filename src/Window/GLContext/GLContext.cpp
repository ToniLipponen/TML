#define GLAD_GL_IMPLEMENTATION
#define GLAD_GLES2_IMPLEMENTATION
#define GLAD_EGL_IMPLEMENTATION
#define GLAD_WGL_IMPLEMENTATION
#define GLAD_GLX_IMPLEMENTATION

#include "GLContext.h"
#include "../../Headers/_Assert.h"
#include "../../Headers/GLHeader.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace tml
{
    GLContext::GLContext()
    {
        /// Initialize GLFW.
        TML_ASSERT(glfwInit() == GLFW_TRUE, "Failed to initialize GLFW");

        /// Set context hints.
#if defined(TML_USE_GLES)
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#else
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
        glfwWindowHint(GLFW_VISIBLE, 0);

        /// Create the actual context.
        m_contextHandle = glfwCreateWindow(640, 480, "", nullptr, nullptr);

        /// Check that the context was created successfully.
        TML_ASSERT(m_contextHandle, "Failed to create an OpenGL context");

        /// Make the context current. This is because when gladLoadGL is called, there needs to be a context active.
        glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_contextHandle));

        /// Set GLFW error callback.
        glfwSetErrorCallback([](int, const char* m){ Logger::ErrorMessage("GLFW ERROR: %s", m); });


#ifdef TML_USE_GLES
    #ifdef PLATFORM_WINDOWS
        TML_ASSERT(gladLoadGL((GLADloadfunc)glfwGetProcAddress), "Failed to load OpenGL functions");
    #else
        TML_ASSERT(gladLoadGLES2((GLADloadfunc)glfwGetProcAddress), "Failed to load OpenGL functions");
    #endif
#else
        TML_ASSERT(gladLoadGL((GLADloadfunc)glfwGetProcAddress), "Failed to load OpenGL functions");
#endif

#if !defined(TML_USE_GLES) && !defined(TML_NO_GL_DEBUGGING)
        glad_glEnable(GL_DEBUG_OUTPUT);
        glad_glDebugMessageCallback(GLMessageCallback, nullptr);
#endif
    }

    GLContext::~GLContext()
    {
        glfwDestroyWindow(static_cast<GLFWwindow*>(m_contextHandle));
        gladLoaderUnloadGL();
        glfwTerminate();
    }

    GLContext& GLContext::GetInstance() noexcept
    {
        static GLContext glContext;
        return glContext;
    }

    void* GLContext::GetContextHandle() const noexcept
    {
        return m_contextHandle;
    }
}