#include <TML/Interface/Components/Scrollbar.h>
#include <TML/Graphics/Renderer.h>

namespace tml
{
    namespace Interface
    {
        template<ComponentAxis axis>
        Scrollbar<axis>::Scrollbar(i32 x, i32 y, ui32 length, ui32 thickness)
        {
            m_pos = Vector2(x,y);
            if(axis == Horizontal)
                m_size = Vector2(length, thickness);
            else
                m_size = Vector2(thickness, length);
            m_sColor = 0x4d8be4ff;
            m_pColor = 0xccccccff;
        }

        template<ComponentAxis axis>
        void Scrollbar<axis>::SetValue(ui32 value)
        {
            m_value = Util::Clamp<i32>(value, m_min, m_max);
        }

        template<ComponentAxis axis>
        void Scrollbar<axis>::Draw()
        {
            if(axis == Vertical)
            {
                const float barSize = m_size.y / m_max;
                const auto barPos = Util::Max(m_pos.y + barSize * m_value, m_pos.y + m_size.y - barSize);

                Renderer::DrawRect(m_pos, m_size, m_pColor);
                Renderer::DrawRect({m_pos.x, barPos}, {m_size.x, barSize}, m_sColor);
            }
            else
            {
                const float barSize = m_size.x / m_max;
                const auto barPos = Util::Max(m_pos.x + barSize * m_value, m_pos.x + m_size.x - barSize);

                Renderer::DrawRect(m_pos, m_size, m_pColor);
                Renderer::DrawRect({barPos, m_pos.y}, {barSize, m_size.y}, m_sColor);
            }

        }

        template<ComponentAxis axis>
        void Scrollbar<axis>::OnMouseClick(const Vector2& mousePos)
        {
            OnMouseDrag(mousePos);
        }

        template<ComponentAxis axis>
        void Scrollbar<axis>::OnMouseDrag(const Vector2 &mousePos)
        {
            if(axis == Horizontal)
                m_value = Util::Clamp<i32>((mousePos.x - m_pos.x) / m_size.x * m_max, m_min, m_max);
            else
                m_value = Util::Clamp<i32>((mousePos.y - m_pos.y) / m_size.y * m_max, m_min, m_max);
        }

        template class Scrollbar<Horizontal>;
        template class Scrollbar<Vertical>;
    }
}