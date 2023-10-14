#pragma once
#include <TML/Window/Window.h>
#include <TML/Graphics/RenderTarget.h>
#include <TML/Export.h>

namespace tml
{
    class TML_API RenderWindow : public Window, public RenderTarget
    {
    public:
        enum Settings : uint32_t
        {
            DontAutoResizeView      = (1 << 16),
            DontAutoResizeViewport  = (1 << 17)
        };
    public:
        RenderWindow(uint32_t w, uint32_t h, const String& title, uint32_t settings = tml::WindowSettings::VSync);
        explicit RenderWindow(const WindowSettings& settings);
        void Clear() noexcept override;
        void Clear(Color color) noexcept override;
        void Display() noexcept override;
        
        [[nodiscard]] 
        Image Screenshot() const noexcept;
        
        Vector2f GetRenderTargetSize() const noexcept override { return GetSize(); }
    private:
        bool m_autoResizeView = true;
        bool m_autoResizeViewport = true;
    };
}