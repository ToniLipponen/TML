#include <TML/Graphics/RenderWindow.h>
#include <TML/Graphics/Core/Buffers.h>
#include <TML/Graphics/Core/Shader.h>
#include "../Headers/GLHeader.h" /// Needed for glad_glFlush()

namespace tml
{
    RenderWindow::RenderWindow(ui32 w, ui32 h, const std::string& title, ui32 settings, const Window* shared)
    : Window(w , h, title, settings, shared)
    {
        SetView({0,0}, {w,h});
        m_autoResizeView = (settings & DontAutoResizeView) == 0;
        m_autoResizeViewport = (settings & DontAutoResizeViewport) == 0;
    }

    void RenderWindow::Clear() noexcept
    {
        SetActive();
        ResetCamera();

        if(m_autoResizeViewport)
            SetViewport({0,0}, m_size);

        if(m_autoResizeView)
            SetView({0,0}, m_size);

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

    void RenderWindow::Screenshot(Image& image)
    {
        auto w = GetWidth();
        auto h = GetHeight();
        ui32* buffer = new ui32[w*h];
        image.LoadFromMemory(w, h, 4, nullptr);
        glad_glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

        for(auto i = 0; i < h; ++i)
        {
            for(auto j = 0; j < w; ++j)
            {
                ((ui32*)image.GetData())[i * w + j] = buffer[((h - i) * w) + j];
            }
        }

        delete[] buffer;
    }
}
