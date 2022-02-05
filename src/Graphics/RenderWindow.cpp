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

    void RenderWindow::DrawFps(const Vector2f &position, float size, const Color& color) noexcept
    {
        DrawText("FPS: " + std::to_string(int(1.0 / m_deltaTime)), position, size, color);
    }

    void RenderWindow::Display()
    {
        EndBatch();
        if(m_useVSync)
            glfwSwapBuffers(static_cast<GLFWwindow*>(m_handle));
        else
            glad_glFlush();
        m_deltaTime = m_clock.Reset();
    }

}
