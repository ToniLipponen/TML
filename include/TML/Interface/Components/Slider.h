#pragma once
#include "../BaseComponent.h"

namespace tml::Interface
{
    template<ComponentAxis axis>
    class TML_API Slider : public BaseComponent
    {
    public:
        Slider(int32_t x, int32_t y, uint32_t size, uint32_t thickness = 20, float min = 0.0f, float max = 1.0f);
        void SetRoundness(float roundness);
        void SetValue(float value);
        inline float GetValue() const { return m_value; }
    private:
        void pDraw(RenderTarget& target) override;
        float m_min, m_max, m_value, m_roundness = 30;
    };
    using VSlider = Slider<Vertical>;
    using HSlider = Slider<Horizontal>;
}