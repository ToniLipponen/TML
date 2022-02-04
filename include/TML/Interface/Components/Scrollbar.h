#pragma once
#include <TML/Interface/BaseComponent.h>

namespace tml
{
    namespace Interface
    {
        template<ComponentAxis axis>
        class Scrollbar : public BaseComponent
        {
        public:
            Scrollbar(i32 x, i32 y, ui32 length, ui32 thickness = 20);
            void SetRange(ui32 min, ui32 max) noexcept { m_min = min; m_max = max; }
            Vector2i GetRange() noexcept { return Vector2i(m_min, m_max); }
            i32 GetValue() const noexcept { return i32(m_value); }
            void SetValue(ui32 value);
        private:
            void Draw(RenderWindow& renderWindow) override;

            i32 m_value = 0;
            ui32 m_max = 1;
            ui32 m_min = 0;
        };

        using HScrollbar = Scrollbar<Horizontal>;     // Vertical scrollbar.
        using VScrollbar = Scrollbar<Vertical>;     // Vertical scrollbar.
    }
}

