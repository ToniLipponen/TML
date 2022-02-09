#pragma once
#include "TML/Window/Window.h"
#include <TML/Graphics/Renderer.h>

namespace tml
{
    class RenderWindow : public Window, public Renderer
    {
    public:
        RenderWindow(ui32 w, ui32 h, const std::string& title, ui32 settings = None);
        virtual ~RenderWindow();

        void Clear() noexcept override;
        void Display() override;

    private:
        double m_deltaTime = 0;
    };
}