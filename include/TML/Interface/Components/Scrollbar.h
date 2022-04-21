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
            Scrollbar(int32_t x, int32_t y, uint32_t length, uint32_t thickness = 20);
            void SetRange(uint32_t min, uint32_t max) noexcept { m_min = min; m_max = max; }
            Vector2i GetRange() noexcept { return Vector2i(m_min, m_max); }
            int32_t GetValue() const noexcept { return int32_t(m_value); }
            void SetValue(uint32_t value);
        private:
            void pDraw(Renderer &renderer) override;

            int32_t m_value = 0;
            uint32_t m_max = 1;
            uint32_t m_min = 0;
        };

        using HScrollbar = Scrollbar<Horizontal>;     // Vertical scrollbar.
        using VScrollbar = Scrollbar<Vertical>;     // Vertical scrollbar.
    }
}

