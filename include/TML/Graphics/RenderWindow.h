#pragma once
#include <TML/Window/Window.h>
#include <TML/Graphics/RenderTarget.h>

namespace tml
{
    class RenderWindow : public Window, public RenderTarget
    {
    public:
        enum Settings : ui32
        {
            DontAutoResizeView      = (1 << 16),
            DontAutoResizeViewport  = (1 << 17)
        };
    public:
        RenderWindow(ui32 w, ui32 h, const std::string& title, ui32 settings = Window::Settings::None, const Window* shared = nullptr);
        void Clear() noexcept override;
        void Display() override;
        void Screenshot(Image& image);
    private:
        Vector2f GetRenderTargetSize() const noexcept override { return GetSize(); }
        bool m_autoResizeView = true;
        bool m_autoResizeViewport = true;
    };
}