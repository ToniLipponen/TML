#pragma once
#include "../BaseComponent.h"

namespace tml::Interface
{
    template<ComponentAxis axis>
    class TML_API Slider : public BaseComponent
    {
    public:
        Slider(uint32_t size, uint32_t thickness = 20, int32_t x = 0, int32_t y = 0, float min = 0.0f, float max = 1.0f) noexcept;
        void SetValue(float value) noexcept;
        float GetValue() const noexcept;

    private:
        void pDraw(RenderTarget& target) noexcept override;
        float m_min, m_max, m_value, m_roundness = 0, m_targetValue = 0.5f, m_sliderValue = 0.5f;
        Color m_borderColor, m_bodyColor;
        double m_borderAnimationProgress = 0;
    };

    using VSlider = Slider<ComponentAxis::Vertical>;
    using HSlider = Slider<ComponentAxis::Horizontal>;
}