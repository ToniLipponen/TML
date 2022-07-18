#pragma once
#include <TML/Interface/Component.h>

namespace tml::Interface
{
    class TML_API ToggleSwitch : public Component
    {
    public:
        explicit ToggleSwitch(int32_t x = 0, int32_t y = 0, uint32_t size = 20, bool state = false) noexcept;

        bool GetValue() const noexcept;
        void SetValue(bool value) noexcept;

    private:
        void pDraw(RenderTarget& target) noexcept override;
        bool m_toggleState;
        float m_knobValue = 0;
        double m_borderAnimationProgress = 0;
        Color m_borderColor;
    };
}