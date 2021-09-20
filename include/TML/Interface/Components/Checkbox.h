#pragma once
#include "../InterfaceComponent.h"

namespace tml
{
    namespace Interface
    {
        class Checkbox : public BaseComponent
        {
        public:
            Checkbox(i32 x, i32 y, ui32 size, bool checked = false);
            bool GetValue() const { return m_value; }
        private:
            virtual void Draw();
            virtual void OnMouseClick(const Vector2& mousePos);
            bool m_value = false;
        };
    }
}