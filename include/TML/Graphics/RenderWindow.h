#pragma once
#include <TML/Window/Window.h>
#include <TML/Graphics/Renderer.h>
#include <TML/Graphics/RenderTexture.h>
#include <TML/System/Clock.h>

namespace tml
{
    class RenderWindow : public Window, public Renderer
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
        bool m_autoResizeView = true;
        bool m_autoResizeViewport = true;
    };
}