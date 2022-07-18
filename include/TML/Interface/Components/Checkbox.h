#pragma once
#include "../Component.h"

namespace tml::Interface
{
    class TML_API Checkbox : public Component
    {
    public:
        Checkbox(uint32_t = 20, int32_t x = 0, int32_t y = 0, bool checked = false) noexcept;
        bool GetValue() const noexcept;
        void SetValue(bool value) noexcept;

    private:
        void pDraw(RenderTarget& renderer) noexcept override;
        bool m_value = false;
        Color m_borderColor, m_bodyColor;
        double m_borderAnimationProgress = 0, m_bodyAnimationProgress = 0;
    };
}