#pragma once
#include <TML/Window/Window.h>
#include <TML/Graphics/RenderTarget.h>

namespace tml
{
    class RenderWindow : public Window, public RenderTarget
    {
    public:
        enum Settings : uint32_t
        {
            DontAutoResizeView      = (1 << 16),
            DontAutoResizeViewport  = (1 << 17)
        };
    public:
        RenderWindow(uint32_t w, uint32_t h, const String& title, uint32_t settings = Window::Settings::None);
        void Clear() noexcept override;
        void Display() override;
        Image Screenshot() const noexcept;
    private:
        Vector2f GetRenderTargetSize() const noexcept override { return GetSize(); }
        bool m_autoResizeView = true;
        bool m_autoResizeViewport = true;
    };
}