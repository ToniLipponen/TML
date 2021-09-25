#pragma once
#include "../InterfaceComponent.h"

namespace tml::Interface
{
    class Slider : public BaseComponent
    {
    public:
        enum Type {Vertical, Horizontal};
    public:
        Slider(ui32 type, i32 x, i32 y, ui32 size, ui32 thickness = 20,
               float min = 0.0f, float max = 1.0f);
        void SetValue(float value);
        inline float GetValue() const { return m_value; }
    private:
        void OnMouseDown(const Vector2& mp) override;
        void Draw() override;
        float m_min, m_max, m_value;
        ui32 m_type; // Vertical, Horizontal
    };
}