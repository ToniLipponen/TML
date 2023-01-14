#include <TML/Graphics/RenderWindow.h>
#include <TML/Graphics/Core/Buffers.h>
#include <TML/Graphics/Core/Shader.h>
#include "Headers/GLHeader.h"

namespace tml
{
    RenderWindow::RenderWindow(uint32_t w, uint32_t h, const String& title, uint32_t settings)
    : Window(w , h, title, settings)
    {
        SetView({0,0}, {w,h});
        m_autoResizeView = (settings & DontAutoResizeView) == 0;
        m_autoResizeViewport = (settings & DontAutoResizeViewport) == 0;
    }

    RenderWindow::RenderWindow(const WindowSettings &settings)
    : Window(settings)
    {
        m_autoResizeView = (settings.flags & DontAutoResizeView) == 0;
        m_autoResizeViewport = (settings.flags & DontAutoResizeViewport) == 0;
    }

    void RenderWindow::Clear() noexcept
    {
        SetActive();
        ResetCamera();

        if(m_autoResizeViewport)
        {
            SetViewport({0,0}, m_size);
        }

        if(m_autoResizeView)
        {
            SetView({0,0}, m_size);
        }

        RenderTarget::Clear();
    }

    void RenderWindow::Clear(Color color) noexcept
    {
        SetActive();
        ResetCamera();

        if(m_autoResizeViewport)
        {
            SetViewport({0,0}, m_size);
        }

        if(m_autoResizeView)
        {
            SetView({0,0}, m_size);
        }

        RenderTarget::Clear(color);
    }

    void RenderWindow::Display() noexcept
    {
        RenderTarget::EndBatch();
        GL_CALL(glad_glFlush());
        Window::Display();
    }

    Image RenderWindow::Screenshot() const noexcept
    {
        const auto w = GetWidth();
        const auto h = GetHeight();
        Image image(w, h, nullptr);
        GL_CALL(glad_glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, image.GetData()));
        image.FlipVertically();
        return image;
    }
}
