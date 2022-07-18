#pragma once
#include "../Component.h"
#include <TML/Graphics/Drawable/Text.h>

namespace tml::Interface
{
    class TML_API Button : public Component
    {
    public:
        explicit Button(
                const std::string& text,
                uint32_t height = 20,
                uint32_t width = 0,
                int32_t x = 0,
                int32_t y = 0,
                bool expand = false);

        void SetText(const std::string& str);
        void SetTextSize(float s) noexcept;
        void SetTextColor(const Color& color) noexcept override;
        const String& GetText() const noexcept;

    private:
        void pDraw(RenderTarget& renderer) noexcept override;
        Text m_text;
        Color m_borderColor, m_bodyColor;
        bool m_mouseDown = false;
        double m_borderAnimationProgress = 0, m_bodyAnimationProgress = 0;
    };
}