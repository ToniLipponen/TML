#include <TML/Interface/Components/Scrollbar.h>
#include <TML/Renderer.h>

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
        }

        template<ComponentAxis axis>
        void Scrollbar<axis>::Draw()
        {
            if(axis == Vertical)
            {
                Renderer::DrawRect(m_pos, m_size, m_pColor, m_size.x / 2);
                Renderer::DrawRect(m_pos + Vector2(0, (m_size.y / m_max) * m_value), {m_size.x, m_size.y / m_max}, m_sColor, m_size.x / 2);
            }
            else
            {
                Renderer::DrawRect(m_pos, m_size, m_pColor, m_size.y / 2);
                Renderer::DrawRect(m_pos + Vector2((m_size.x / m_max) * m_value, 0), {m_size.x / m_max, m_size.y}, m_sColor, m_size.y / 2);
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
                m_value = Util::Clamp<ui32>((mousePos.x - m_pos.x) / m_size.x * m_max, m_min, m_max-1);
            else
                m_value = Util::Clamp<ui32>((mousePos.y - m_pos.y) / m_size.y * m_max, m_min, m_max-1);
        }

        template class Scrollbar<Horizontal>;
        template class Scrollbar<Vertical>;
    }
}