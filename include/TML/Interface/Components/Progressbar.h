#pragma once
#include <TML/Interface/Component.h>

namespace tml::Interface
{
    class TML_API Progressbar : public Component
    {
    public:
        Progressbar(int32_t x, int32_t y, uint32_t w, uint32_t h = 30) noexcept;
        float GetValue() const noexcept;
        void SetValue(float value) noexcept;
        void SetTextColor(const Color& color) noexcept override;

    private:
        void pDraw(RenderTarget& renderer) noexcept override;
        float m_value = 0;
        Text m_text;
    };
}