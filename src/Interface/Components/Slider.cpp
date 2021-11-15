#include <TML/Interface/Components/Slider.h>
#include <TML/Utilities/Utilities.h>
#include <TML/Graphics/Renderer.h>

namespace tml
{
    namespace Interface
    {
        template<ComponentAxis axis>
        Slider<axis>::Slider(i32 x, i32 y, ui32 size, ui32 thickness, float min, float max)
        {
            m_min = Util::Min<float>(min, 0);
            m_max = Util::Min<float>(max, 0);
            m_value = (m_min + m_max) / 2;

            if(axis == Horizontal)
            {
                m_pos = Vector2(x,y);
                m_size = Vector2(size, thickness);
                m_vSizePolicy = Fixed;
                m_hSizePolicy = Expand;
            }
            else
            {
                m_pos = Vector2(x,y);
                m_size = Vector2(thickness, size);
                m_vSizePolicy = Expand;
                m_hSizePolicy = Fixed;
            }
        }

        template<ComponentAxis axis>
        void Slider<axis>::SetValue(float value)
        {
            m_value = Util::Clamp(value, m_min, m_max);
        }

        template<ComponentAxis axis>
        void Slider<axis>::OnMouseDown(const Vector2& mp)
        {

        }

        template<ComponentAxis axis>
        void Slider<axis>::OnMouseDrag(const Vector2& mp)
        {
            if(axis == Horizontal)
                m_value = Util::Clamp((mp.x - m_pos.x) / m_size.x * m_max, m_min, m_max);
            else
                m_value = Util::Clamp(m_max - ((mp.y - m_pos.y) / m_size.y * m_max), m_min, m_max);
        }

        template<ComponentAxis axis>
        void Slider<axis>::Draw()
        {
            Renderer::DrawRect(m_pos, m_size, m_pColor);
            if(m_state.Focused)
                Renderer::DrawGrid(m_pos, m_size, 1, 1, m_activeColor);
            else
                Renderer::DrawGrid(m_pos, m_size, 1, 1, m_sColor);
            if(axis == Horizontal)
                Renderer::DrawRect(m_pos, {Util::Lerp({0,0}, m_size, m_value / m_max).x, m_size.y}, m_activeColor);
            else
                Renderer::DrawRect(m_pos + Vector2(0, Util::Lerp({0,0}, m_size, 1.0 - m_value / m_max).y),
                                   {m_size.x, m_size.y - Util::Lerp({0,0}, m_size, 1.0 - m_value / m_max).y}, m_activeColor);
        }
        template class Slider<Vertical>;
        template class Slider<Horizontal>;
    }
}