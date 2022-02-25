#include <TML/Graphics/RenderWindow.h>
#include <TML/Graphics/Core/Buffers.h>
#include <TML/Graphics/Core/Shader.h>
#include "Core/Headers/GLHeader.h" /// Needed for glad_glFlush()
#include "Core/Headers/GlDebug.h"  /// Needed for GL_CALL()
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace tml
{
    RenderWindow::RenderWindow(ui32 w, ui32 h, const std::string& title, ui32 settings)
    : Window(w , h, title, settings)
    {

    }

    void RenderWindow::Clear() noexcept
    {
        auto* handle = static_cast<GLFWwindow*>(m_handle);
        if(handle == glfwGetCurrentContext())
            glfwMakeContextCurrent(handle);
        Renderer::SetViewport({0,0}, GetSize());
        Renderer::Clear();
    }

    void RenderWindow::Display()
    {
        EndBatch();
        GL_CALL(glad_glFlush()); /// Fixes an issue with GLFW, where the screen remains black even when swapping buffers.
        Window::Display();
    }
}
