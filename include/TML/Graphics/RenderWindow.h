#pragma once
#include <TML/Graphics/Window.h>
#include <TML/Graphics/Renderer.h>
#include <TML/System/Clock.h>

namespace tml
{
    class RenderWindow : public Window, public Renderer
    {
    public:
        RenderWindow(ui32 w, ui32 h, const std::string& title, ui32 settings = None);
        virtual ~RenderWindow();

        void Clear() noexcept override;
        void DrawFps(const Vector2f& position, float size = 20, const Color& color = Color::Green) noexcept;
        void Display() override;

    private:
        Clock m_clock;
        double m_deltaTime = 0;
    };
}