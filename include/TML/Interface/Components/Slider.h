#pragma once
#include "../BaseComponent.h"

namespace tml
{
    namespace Interface
    {
        template<ComponentAxis axis>
        class Slider : public BaseComponent
        {
        public:
            Slider(i32 x, i32 y, ui32 size, ui32 thickness = 20,
                   float min = 0.0f, float max = 1.0f);
            void SetValue(float value);
            inline float GetValue() const { return m_value; }
        private:
            void OnMouseDown(const Vector2i& mp) override;
            void OnMouseDrag(const Vector2i& mousePos) override;
            void Draw() override;
            float m_min, m_max, m_value;
        };
        using VSlider = Slider<Vertical>;
        using HSlider = Slider<Horizontal>;
    }
}