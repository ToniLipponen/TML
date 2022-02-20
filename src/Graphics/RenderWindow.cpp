#include <TML/Graphics/RenderWindow.h>
#include <TML/Graphics/Core/Buffers.h>
#include <TML/Graphics/Core/Shader.h>
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
        Window::Display();
    }
}
