#include "GLContext.h"
#include "_Assert.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace tml
{
    GLContext::GLContext()
    {
        /// Initialize GLFW.
        TML_ASSERT(glfwInit() == GLFW_TRUE, "Failed to initialize GLFW");

        /// Set GLFW error callback.
        glfwSetErrorCallback([](int, const char* m){ std::printf("[Error]: %s", m); });

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

        glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_contextHandle));
    }

    GLContext::~GLContext() = default;

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