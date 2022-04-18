#include <TML/Graphics/RenderWindow.h>
#include <TML/Graphics/Core/Buffers.h>
#include <TML/Graphics/Core/Shader.h>
#include "../Headers/GLHeader.h" /// Needed for glad_glFlush()

namespace tml
{
    RenderWindow::RenderWindow(uint32_t w, uint32_t h, const std::string& title, uint32_t settings, const Window* shared)
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
        Window::Display();
    }

    void RenderWindow::Screenshot(Image& image)
    {
        auto w = GetWidth();
        auto h = GetHeight();
        uint32_t* buffer = new uint32_t[w*h];
        image.LoadFromMemory(w, h, 4, nullptr);
        glad_glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

        for(auto i = 0; i < h; ++i)
        {
            for(auto j = 0; j < w; ++j)
            {
                ((uint32_t*)image.GetData())[i * w + j] = buffer[((h - i) * w) + j];
            }
        }

        delete[] buffer;
    }
}
