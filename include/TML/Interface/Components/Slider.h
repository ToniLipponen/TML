#pragma once
#include "../InterfaceComponent.h"

namespace tml
{
    namespace Interface
    {
        class Slider : public BaseComponent
        {
        public:
            enum Type {Vertical, Horizontal};
        public:
            Slider(ui32 type, i32 x, i32 y, ui32 size, ui32 thickness = 5, float min = 0.0f, float max = 1.0f);
            void SetValue(float value);
            float GetValue() const { return m_value; }
        private:
            virtual void OnMouseClick(const Vector2& mp) override;
            virtual void OnMouseDown(const Vector2& mp) override;
            virtual void OnUpdate(float dt) override;
            void Draw() override;
            float m_min, m_max, m_value, m_target;
            float m_thickness, m_size;
            ui32 m_type; // Vertical, Horizontal
        };
    }
}