#include <TML/Interface/Components/Scrollbar.h>

namespace tml::Interface
{
    template<ComponentAxis axis>
    Scrollbar<axis>::Scrollbar(int32_t x, int32_t y, uint32_t length, uint32_t thickness) noexcept
    {
        m_pos = Vector2i(x,y);

        if constexpr(axis == ComponentAxis::Horizontal)
        {
            m_size = Vector2i(length, thickness);
        }
        else
        {
            m_size = Vector2i(thickness, length);
        }

        m_pColor = Color(0xccccccff);

        AddListener("Dragged", [&](Component* c, const Event& e)
        {
            if constexpr(axis == ComponentAxis::Horizontal)
            {
                m_value = Math::Clamp<float>(float(e.mouseButton.x - m_pos.x) / float(m_size.x) * m_max, m_min, m_max);
            }
            else
            {
                m_value = m_max - Math::Clamp<float>(m_max - float((e.mouseButton.y - m_pos.y) / float(m_size.y) * m_max), m_min, m_max);
            }

            return true;
        });
    }

    template<ComponentAxis axis>
    void Scrollbar<axis>::SetRange(uint32_t min, uint32_t max) noexcept
    {
        m_min = min;
        m_max = max;
    }

    template<ComponentAxis axis>
    Vector2i Scrollbar<axis>::GetRange() const noexcept
    {
        return {m_min, m_max};
    }

    template<ComponentAxis axis>
    int32_t Scrollbar<axis>::GetValue() const noexcept
    {
        return m_value;
    }

    template<ComponentAxis axis>
    void Scrollbar<axis>::SetValue(uint32_t value) noexcept
    {
        m_value = Math::Clamp<int32_t>(value, m_min, m_max);
    }

    template<ComponentAxis axis>
    void Scrollbar<axis>::pDraw(RenderTarget& target) noexcept
    {
        if constexpr(axis == ComponentAxis::Vertical)
        {
            const float barSize = m_size.y / static_cast<float>(m_max);
            const auto barPos = Math::Min(m_pos.y + barSize * m_value, m_pos.y + m_size.y - barSize);

            target.DrawRect(m_pos, m_size, m_pColor);
            target.DrawRect(Vector2f(m_pos.x, barPos), Vector2f(m_size.x, barSize), m_activeColor, m_size.x / 4);
        }
        else
        {
            const float barSize = m_size.x / static_cast<float>(m_max);
            const auto barPos = Math::Min(m_pos.x + barSize * m_value, m_pos.x + m_size.x - barSize);

            target.DrawRect(m_pos, m_size, m_pColor);
            target.DrawRect(Vector2f(barPos, m_pos.y), Vector2f(barSize, m_size.y), m_activeColor, m_size.y / 4);
        }
    }

    template class Scrollbar<ComponentAxis::Horizontal>;
    template class Scrollbar<ComponentAxis::Vertical>;
}