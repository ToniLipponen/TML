#include <TML/Graphics/RenderWindow.h>
#include <TML/Graphics/Core/Buffers.h>
#include <TML/Graphics/Core/Shader.h>

#include "Core/Headers/GLHeader.h" /// Needed for glad_glFlush()

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace tml
{
    RenderWindow::RenderWindow(ui32 w, ui32 h, const std::string& title, ui32 settings, const Window* shared)
    : Window(w , h, title, settings, shared)
    {

    }

    void RenderWindow::Clear() noexcept
    {
        auto* handle = static_cast<GLFWwindow*>(m_handle);

        /// Seems to be faster to check if you first check if the handle of the window is the active one,
        /// and if not then make it the active one.
        if(handle != glfwGetCurrentContext())
            glfwMakeContextCurrent(handle);

        SetViewport({0,0}, GetSize());
        Renderer::Clear();
    }

    void RenderWindow::Display()
    {
        EndBatch();

        /// Fixes an issue with GLFW, where the screen remains black even when swapping buffers.
        /// This only seems to be an issue on some platforms.
        GL_CALL(glad_glFlush());
        Window::Display();
    }

    void RenderWindow::Screenshot(const String& filename)
    {
        auto w = GetWidth();
        auto h = GetHeight();
        ui32* buffer = new ui32[w*h];
        Image image(w, h, 4, nullptr);
        glad_glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        for(auto i = 0; i < h; ++i)
            for(auto j = 0; j < w; ++j)
            {
                ((ui32*)image.GetData())[i * w + j] = buffer[((h - i) * w) + j];
            }
        image.WriteToFile(filename);
    }
}
