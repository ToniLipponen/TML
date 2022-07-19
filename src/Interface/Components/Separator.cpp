#include <TML/Interface/Components/Separator.h>

namespace tml::Interface
{
    template<ComponentAxis axis>
    Separator<axis>::Separator(uint32_t thickness) noexcept
    {
        if constexpr(axis == ComponentAxis::Horizontal)
        {
            m_hSizePolicy = SizePolicy::Dynamic;
            m_vSizePolicy = SizePolicy::Fixed;
        }
        else
        {
            m_hSizePolicy = SizePolicy::Fixed;
            m_vSizePolicy = SizePolicy::Dynamic;
        }

        m_thickness = thickness;
        m_size = {0, 0};
        m_pColor = Color::Black;
    }

    template<ComponentAxis axis>
    void Separator<axis>::pDraw(RenderTarget& renderer) noexcept
    {
        renderer.DrawLine(m_pos, m_pos + m_size, m_thickness, m_pColor);
    }

    template class Separator<ComponentAxis::Vertical>;
    template class Separator<ComponentAxis::Horizontal>;
}