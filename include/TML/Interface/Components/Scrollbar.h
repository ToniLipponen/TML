#pragma once
#include <TML/Interface/Component.h>

namespace tml::Interface
{
    template<ComponentAxis axis>
    class TML_API Scrollbar : public Component
    {
    public:
        Scrollbar(int32_t x, int32_t y, uint32_t length, uint32_t thickness = 20) noexcept;
        void SetRange(uint32_t min, uint32_t max) noexcept;
        Vector2i GetRange() const noexcept;
        int32_t GetValue() const noexcept;
        void SetValue(uint32_t value) noexcept;
    private:
        void pDraw(RenderTarget& target) noexcept override;

        int32_t m_value = 0;
        uint32_t m_max = 1;
        uint32_t m_min = 0;
    };

    using HScrollbar = Scrollbar<ComponentAxis::Horizontal>;   // Horizontal scrollbar.
    using VScrollbar = Scrollbar<ComponentAxis::Vertical>;     // Vertical scrollbar.
}