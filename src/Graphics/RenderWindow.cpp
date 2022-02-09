#include <TML/Graphics/RenderWindow.h>
#include "Core/Headers/GLHeader.h"
#include "TML/Graphics/Core/Buffers.h"
#include "TML/Graphics/Core/Shader.h"
#include <GLFW/glfw3.h>

namespace tml
{
    RenderWindow::RenderWindow(ui32 w, ui32 h, const std::string& title, ui32 settings)
    : Window(w , h, title, settings)
    {

    }

    RenderWindow::~RenderWindow()
    {

    }

    void RenderWindow::Clear() noexcept
    {
        glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_handle));
        Renderer::SetViewport({0,0}, {GetWidth(),GetHeight()});
        Renderer::Clear();
    }

    void RenderWindow::Display()
    {
        EndBatch();
        Window::Display();
    }

}
